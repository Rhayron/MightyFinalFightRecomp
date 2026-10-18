/*
 * boss_abigail.c - Abigail, the boss of round 3 (id $90, bank $03; the
 * scaffold's bank 1). The ring fight: he walks at the player, punches in
 * combos, charges, catches a jumping player with an uppercut and grabs to
 * pound the player three times and throw; his own grab cannot be broken
 * and he slips out of the player's at once.
 *
 * Evidence: obj_kind 9 = Abigail (written here every frame, 03:B408;
 * docs/objects.md), Data Crystal's id $90 = Abigail; in the base logs he
 * lives in slot $0C of round 3's last area (round 2, sub_area 2) from
 * frame 16306 of base_cody_tour.flog, and base_guy_alt.flog has the fight:
 * the screenshots of frames 26395 (animation $A3 under the jumping player)
 * and 26612 (animation $DE, head down, rushing across the ring) show the
 * uppercut and the charge.
 *
 * Each frame (03:B3F4) counts obj_invuln and obj_tier (here a grab
 * cooldown) down and clears the attack; then, by priority: standing still
 * while the boss-talk object (id $6E) is in slot $0D; breaking a grab;
 * reacting to a hit (half the time a counter-grab, half a charge); backing
 * off while the player lies down; walking back when off the player's
 * screen; else the phase (obj_state & $0F, table $B536/$B53E):
 *   0 start ($B546)         attack at once if close, else approach
 *   1 approach ($B57B)      walk at the player (1 px a frame, 16 ways)
 *   2 back off ($B666)      walk away $30-$60 frames, then decide
 *   3 punches ($B6F5)       one or three punches, the last of three strong
 *   4 charge ($B7C9)        wind-up, a 2.3 px/frame rush that slows down
 *   5 grab ($B879)          catch, three blows, throw
 *   6 decide ($BA1C)        charge (likelier the farther the player) or approach
 *   7 uppercut ($BA88)      against a jumping player
 * obj_timer is the step within a phase, obj_ai_timer its countdown,
 * obj_ai_count the punches or blows left, obj_ai_flag the grab choice of
 * phase 1, obj_timer2/obj_ground_y the charge's slow-down (and the
 * player's line during the grab).
 */
#include "enemy.h"

#define OBJ_ABIGAIL         0x90

/* animations, with the sprite bank (Y of 0F:F443) */
#define ANIM_STAND          0xD9    /* bank $24 */
#define ANIM_WALK           0xDA    /* bank $24 */
#define ANIM_PUNCH          0xDC    /* bank $25 */
#define ANIM_WIND_UP        0xDD    /* bank $24: before the charge */
#define ANIM_CHARGE         0xDE    /* bank $25 */
#define ANIM_HURT           0xDF    /* bank $26 */
#define ANIM_GRAB           0x91    /* bank $3E */
#define ANIM_UPPERCUT       0xA3    /* bank $30 */
#define CHR_ABIGAIL         0x24
#define CHR_ABIGAIL_PUNCH   0x25
#define CHR_ABIGAIL_HURT    0x26
#define CHR_ABIGAIL_GRAB    0x3E
#define CHR_ABIGAIL_UPPER   0x30

/* obj_state values: live + phase */
#define ST_APPROACH         0x81
#define ST_BACK_OFF         0x82
#define ST_PUNCHES          0x83
#define ST_CHARGE           0x84
#define ST_GRAB             0x85
#define ST_DECIDE           0x86
#define ST_UPPERCUT         0x87

/* attacks: obj_attack (out | reach box) and obj_attack_damage */
#define ATK_PUNCH           0x8C
#define DMG_PUNCH           0x07
#define DMG_PUNCH_LAST      0x0F    /* the third punch of a combo */
#define ATK_CHARGE          0x8B
#define DMG_CHARGE          0x0E
#define ATK_UPPERCUT_LOW    0x96    /* animation frames 0-1 */
#define ATK_UPPERCUT_HIGH   0x97    /* frames 2-8 */
#define DMG_UPPERCUT        0x26
#define DMG_GRAB_BLOW       0x07
#define DMG_GRAB_LAST       0x0F

#define SFX_PUNCH_HIT       0x18
#define SFX_GRAB_BLOW       0x1F

/* reach boxes of 0F:FB3E (|dx| <= A, |dy| <= Y to the player's line) */
#define REACH_PUNCH_X       0x22
#define REACH_PUNCH_Y       0x09
#define REACH_JUMP_Y        0x08
#define REACH_GRAB_X        0x25
#define REACH_GRAB_Y        0x0C
#define NEAR_X              0x30
#define NEAR_Y              0x10

/* the ring: he turns around outside x $17-$F0 and walks back to the
 * player's screen outside $16-$F1 */
#define RING_LEFT           0x17
#define RING_RIGHT          0xF1

/* the phase table */
#define PHASE_LO            0xB536
#define PHASE_HI            0xB53E
/* walking speeds, 1 px a frame, by direction index 0-15 (clockwise from
 * up, as $FBF0 maps them to pad bits): vx, vx_frac, vy, vy_frac */
#define WALK_VX             0xB3B4
#define WALK_VX_FRAC        0xB3C4
#define WALK_VY             0xB3D4
#define WALK_VY_FRAC        0xB3E4
#define WALK_UP             0x00    /* direction index: straight up/down */
#define WALK_ACROSS         0x04    /* straight right/left */
/* charge speeds, 2.3 px a frame */
#define CHARGE_VX           0xBAE9
#define CHARGE_VX_FRAC      0xBAF9
#define CHARGE_VY           0xBB09
#define CHARGE_VY_FRAC      0xBB19
#define CHARGE_STEEP        0xBB81  /* index -> a direction at least 2 steps off vertical */
/* the held player's place: x and y offsets by (frame + 1) / 2 */
#define HOLD_DX             0xBA16
#define HOLD_DY             0xBA19
/* the held and thrown player's poses by player_id (bank $0F) */
#define PLAYER_HELD_ANIM    0xE207
#define PLAYER_HELD_CHR     0xE24F
#define PLAYER_THROWN_ANIM  0xE20A
#define PLAYER_THROWN_CHR   0xE252

void func_B660_b1(void);
void func_B925_b1(void);
void func_B977_b1(void);
void func_BA01_b1(void);
void func_B912_b1(void);

/*
 * abigail_step, $B331 (bank $03; name proposed): one step along obj_dir:
 * up (0F:F0F7), down (F0A2), right (F000), left (F051), each a move with
 * a background probe that sets the carry when the step was blocked. TMP15
 * = $10 when a blocked step's tile has bit 4 set (TMP16 & $10: the ring's
 * ropes), else 0. A blocked step right ends the routine there (never
 * executed, nor is a blocked step left).
 * In: X = slot. Out: TMP15, the RTS.
 * Callers: 03:B5F6, B60F, B660, B6E0, B802, B85F.
 */
void func_B331_b1(void)
{
    REGS_IN;

    mff_cycles(0xB331, 13);
    a = 0x00;
    RAM(TMP15) = a;
    a = RAM(OBJ_DIR + x) & DIR_UP;
    if (a != 0) {
        mff_cycles(0xB33C, 8);
        y = 0x01;
        CALL_FIXED(0xB33E, func_F0F7);          /* step up */
        mff_cycles(0xB341, 2);
        if (c) {
            mff_cycles(0xB343, 7);
            a = RAM(TMP16) & 0x10;
            if (a != 0) {
                mff_cycles(0xB349, 5);
                RAM(TMP15) = a;
                mff_cycles(0xB34B, 1);          /* BNE, always taken */
            } else {
                mff_cycles(0xB347, 1);
            }
        } else {
            mff_cycles(0xB341, 1);
        }
    } else {
        mff_cycles(0xB33A, 1);
        mff_cycles(0xB34D, 8);
        a = RAM(OBJ_DIR + x) & DIR_DOWN;
        if (a != 0) {
            mff_cycles(0xB354, 8);
            y = 0x00;
            CALL_FIXED(0xB356, func_F0A2);      /* step down */
            mff_cycles(0xB359, 2);
            if (c) {
                mff_cycles(0xB35B, 7);
                a = RAM(TMP16) & 0x10;
                if (a != 0) {
                    mff_cycles(0xB361, 3);
                    RAM(TMP15) = a;
                } else {
                    mff_cycles(0xB35F, 1);
                }
            } else {
                mff_cycles(0xB359, 1);
            }
        } else {
            mff_cycles(0xB352, 1);
        }
    }
    mff_cycles(0xB363, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    if (a != 0) {
        mff_cycles(0xB36A, 8);
        y = 0x00;
        CALL_FIXED(0xB36C, func_F000);          /* step right */
        mff_cycles(0xB36F, 2);
        if (c) {                                /* never executed */
            mff_cycles(0xB371, 7);
            a = RAM(TMP16) & 0x10;
            if (a != 0) {
                mff_cycles(0xB377, 5);
                RAM(TMP15) = a;
                mff_cycles(0xB379, 1);
            } else {
                mff_cycles(0xB375, 1);
            }
            mff_cycles(0xB38F, 6);
            RTS_RETURN();
        }
        mff_cycles(0xB36F, 1);
    } else {
        mff_cycles(0xB368, 1);
    }
    mff_cycles(0xB37B, 8);
    a = RAM(OBJ_DIR + x) & DIR_LEFT;
    if (a != 0) {
        mff_cycles(0xB382, 8);
        y = 0x01;
        CALL_FIXED(0xB384, func_F051);          /* step left */
        mff_cycles(0xB387, 2);
        if (c) {                                /* never executed */
            mff_cycles(0xB389, 8);
            a = RAM(TMP16) & 0x10;
            RAM(TMP15) = a;
        } else {
            mff_cycles(0xB387, 1);
        }
    } else {
        mff_cycles(0xB380, 1);
    }
    mff_cycles(0xB38F, 6);
    RTS_RETURN();
}

/*
 * abigail_face_player, $B390 (bank $03; name proposed): JMP obj_face_right
 * when the player is to the right (0F:F374's carry), else obj_face_left.
 * In: X = slot. Out: as 0F:F090/F099.
 * Callers: 03:B5F9, B612, B663 (JMP); 03:B6E3, BA6E.
 */
void func_B390_b1(void)
{
    REGS_IN;

    mff_cycles(0xB390, 6);
    CALL_FIXED(0xB390, func_F374);              /* obj_dist_x: carry = player to the right */
    mff_cycles(0xB393, 2);
    if (!c) {
        mff_cycles(0xB395, 3);
        GO_FIXED(0xF099, func_F099);            /* obj_face_left */
    }
    mff_cycles(0xB393, 1);
    mff_cycles(0xB398, 3);
    GO_FIXED(0xF090, func_F090);                /* obj_face_right */
}

/*
 * abigail_walk_speed, $B39B (bank $03; name proposed): obj_vx/obj_vy for
 * direction index Y, 1 px a frame (tables $B3B4-$B3E4).
 * In: X = slot, Y = index. Out: the RTS.
 * Callers: 03:B5E5, B5FE, B657, B68C, B69D, BA73.
 */
void func_B39B_b1(void)
{
    REGS_IN;

    mff_cycles(0xB39B, 4);
    a = prg_read_ix(WALK_VX_FRAC, y);
    mff_cycles(0xB39E, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = prg_read_ix(WALK_VX, y);
    mff_cycles(0xB3A4, 9);
    RAM(OBJ_VX + x) = a;
    a = prg_read_ix(WALK_VY_FRAC, y);
    mff_cycles(0xB3AA, 9);
    RAM(OBJ_VY_FRAC + x) = a;
    a = prg_read_ix(WALK_VY, y);
    mff_cycles(0xB3B0, 11);
    RAM(OBJ_VY + x) = a;
    RTS_RETURN();
}

/*
 * The step and the turn to the player that end a walking frame: JSR
 * abigail_step at `pc`, JMP abigail_face_player at pc + 3. The copies at
 * 03:B5F6, B60F and B660.
 */
static void step_then_face(uint16_t pc)
{
    REGS_IN;

    mff_cycles(pc, 6);
    CALL_IN(pc, 0xB331, BANK_03);
    mff_cycles((uint16_t)(pc + 0x03), 3);
    GO_IN(0xB390, BANK_03);
}

/*
 * abigail, $B3F4 (bank $03; name proposed): behaviour of id $90. See the
 * file header for the order of the tests. The details:
 * - held by the player (obj_held set, game_action 4 or $21 with him in
 *   grab_slot): he stands (or flinches at a grapple hit, $DF), phase 1,
 *   facing the player. obj_held $FF (released) is cleared; Haggar's
 *   release puts him on the player's y.
 * - hit (obj_hit_timer, set by the hit code): the flinch animation at
 *   $1E/$1A, then, by rng bit 7, a grab (phase 5) or a charge (phase 4,
 *   straight to the rush); obj_hit_timer - 1.
 * - the player lying (game_action $1F): no grab for $18 frames
 *   (obj_tier), phase 2 unless already backing off or deciding.
 *   game_action $0B (Cody's throw) and $25 would clear the hit flag or
 *   knock him down: never executed.
 * - off the player's screen page, or outside x $16-$F1: $BA53.
 * In: X = slot. Out: as the path taken.
 * Caller: the behaviour table (id $90, 03:B300).
 */
void func_B3F4_b1(void)
{
    REGS_IN;

    mff_cycles(0xB3F4, 6);
    if (RAM(OBJ_INVULN + x) != 0) {
        mff_cycles(0xB3F9, 7);
        RAM(OBJ_INVULN + x)--;
    } else {
        mff_cycles(0xB3F7, 1);
    }
    mff_cycles(0xB3FC, 6);
    if (RAM(OBJ_TIER + x) != 0) {
        mff_cycles(0xB401, 7);
        RAM(OBJ_TIER + x)--;
    } else {
        mff_cycles(0xB3FF, 1);
    }
    mff_cycles(0xB404, 22);
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = KIND_ABIGAIL;
    RAM(OBJ_KIND + x) = a;
    a = RAM(BOSS_TALK_ID);
    c = a >= OBJ_BOSS_TALK;
    if (a == OBJ_BOSS_TALK) {                   /* the talk: stand, untouchable */
        mff_cycles(0xB415, 14);
        a = 0x01;
        RAM(OBJ_INVULN + x) = a;
        a = ANIM_STAND;
        y = CHR_ABIGAIL;
        GO_FIXED(0xF443, func_F443);            /* obj_set_anim, sprite bank R3 = Y */
    }
    mff_cycles(0xB413, 1);

    /* held by the player? */
    mff_cycles(0xB421, 6);
    a = RAM(OBJ_HELD + x);
    if (a == 0) {
        mff_cycles(0xB424, 1);
    } else if (mff_cycles(0xB426, 4), a == 0xFF) {
        mff_cycles(0xB428, 1);
        mff_cycles(0xB45B, 14);
        a = 0x00;
        RAM(OBJ_HELD + x) = a;
        a = RAM(PLAYER_ID);
        if (a == PLAYER_HAGGAR) {
            mff_cycles(0xB466, 9);
            a = RAM(OBJ_Y);
            RAM(OBJ_Y + x) = a;
        } else {
            mff_cycles(0xB464, 1);
        }
    } else {
        int grabbed = 0;
        mff_cycles(0xB42A, 7);
        a = RAM(GAME_ACTION);
        if (a == ACT_GRAB) {
            mff_cycles(0xB42E, 1);
            grabbed = 1;
        } else {
            mff_cycles(0xB430, 4);
            if (a == ACT_GRAPPLE_HIT)
                grabbed = 1;
            else
                mff_cycles(0xB432, 1);
        }
        if (grabbed) {
            mff_cycles(0xB434, 5);
            if (x == RAM(GRAB_SLOT)) {
                mff_cycles(0xB438, 7);
                a = RAM(GAME_ACTION);
                c = a >= ACT_GRAPPLE_HIT;
                if (a == ACT_GRAPPLE_HIT) {
                    mff_cycles(0xB43E, 6);
                    a = ANIM_HURT;
                    y = CHR_ABIGAIL_HURT;
                    mff_cycles(0xB442, 1);
                } else {
                    mff_cycles(0xB43C, 1);
                    mff_cycles(0xB444, 4);
                    a = ANIM_STAND;
                    y = CHR_ABIGAIL;
                }
                mff_cycles(0xB448, 6);
                CALL_FIXED(0xB448, func_F443);
                mff_cycles(0xB44B, 22);
                a = 0x00;
                RAM(OBJ_TIMER + x) = a;
                RAM(OBJ_HIT_TIMER + x) = a;
                a = ST_APPROACH;
                RAM(OBJ_STATE + x) = a;
                GO_FIXED(0xF343, func_F343);    /* obj_face_player */
            }
            mff_cycles(0xB436, 1);
        }
    }

    /* hit: flinch, then counter */
    mff_cycles(0xB46C, 6);
    a = RAM(OBJ_HIT_TIMER + x);
    if (a != 0) {
        int flinch = 1;
        mff_cycles(0xB471, 4);
        c = a >= 0x1E;
        if (a == 0x1E) {
            mff_cycles(0xB473, 1);
        } else {
            mff_cycles(0xB475, 4);
            c = a >= 0x1A;
            if (a == 0x1A) {
                mff_cycles(0xB477, 1);
                flinch = 2;
            } else {
                mff_cycles(0xB479, 4);
                c = a >= 0x10;
                if (c) {
                    mff_cycles(0xB47B, 1);
                    flinch = 0;
                }
            }
        }
        if (flinch == 1) {
            mff_cycles(0xB47D, 6);
            a = ANIM_STAND;
            y = CHR_ABIGAIL;
            mff_cycles(0xB481, 1);
        } else if (flinch == 2) {
            mff_cycles(0xB483, 4);
            a = ANIM_HURT;
            y = CHR_ABIGAIL_HURT;
        }
        if (flinch) {
            mff_cycles(0xB487, 6);
            CALL_FIXED(0xB487, func_F443);
        }
        mff_cycles(0xB48A, 7);
        a = RAM(RNG);
        c = a >> 7;                             /* ASL */
        a = (uint8_t)(a << 1);
        if (c) {
            mff_cycles(0xB48F, 6);
            a = ST_GRAB;
            y = 0x00;
            mff_cycles(0xB493, 1);
        } else {
            mff_cycles(0xB48D, 1);
            mff_cycles(0xB495, 4);
            a = ST_CHARGE;
            y = 0x01;                           /* obj_timer 1: straight to the aim */
        }
        mff_cycles(0xB499, 22);
        RAM(OBJ_STATE + x) = a;
        a = y;
        RAM(OBJ_TIMER + x) = a;
        RAM(OBJ_HIT_TIMER + x)--;
        GO_FIXED(0xF343, func_F343);
    }
    mff_cycles(0xB46F, 1);

    /* what the player is doing */
    mff_cycles(0xB4A6, 7);
    a = RAM(GAME_ACTION);
    if (a == ACT_LYING) {
        mff_cycles(0xB4AA, 1);
        mff_cycles(0xB4BD, 22);
        a = 0x00;
        RAM(OBJ_HIT_PLAYER + x) = a;
        a = 0x18;
        RAM(OBJ_TIER + x) = a;                  /* no grab for $18 frames */
        a = RAM(OBJ_STATE + x);
        if (a == ST_BACK_OFF) {
            mff_cycles(0xB4CC, 2);
        } else {
            mff_cycles(0xB4CE, 4);
            c = a >= ST_DECIDE;
            if (a == ST_DECIDE) {
                mff_cycles(0xB4D2, 7);
                a = ANIM_STAND;
                y = CHR_ABIGAIL;
                GO_FIXED(0xF443, func_F443);
            }
            mff_cycles(0xB4D0, 1);
            mff_cycles(0xB4D9, 20);
            a = ST_BACK_OFF;
            RAM(OBJ_STATE + x) = a;
            a = 0x00;
            RAM(OBJ_TIMER + x) = a;
            RTS_RETURN();
        }
    } else if (mff_cycles(0xB4AC, 4), a == 0x0B) {
        /* Cody's throw: never executed */
        mff_cycles(0xB4AE, 1);
        mff_cycles(0xB4B6, 9);
        a = 0x00;
        RAM(OBJ_HIT_PLAYER + x) = a;
        mff_cycles(0xB4BB, 2);
    } else if (mff_cycles(0xB4B0, 4), a == ACT_FALL_INVULN) {
        /* knocked down like a common enemy: never executed */
        mff_cycles(0xB4B2, 1);
        mff_cycles(0xB4E4, 59);
        a = RAM(OBJ_Y + x);
        RAM(OBJ_LAND_Y + x) = a;
        a = KNOCK_VY_FRAC;
        RAM(OBJ_VY_FRAC + x) = a;
        a = KNOCK_VY;
        RAM(OBJ_VY + x) = a;
        a = RAM(OBJ_ID + x);
        RAM(OBJ_SAVED_ID + x) = a;
        a = OBJ_KNOCKED_DOWN;
        RAM(OBJ_ID + x) = a;
        a = 0x80;
        RAM(OBJ_STATE + x) = a;
        a = 0xFF;
        RAM(OBJ_INVULN + x) = a;
        RTS_RETURN();
    } else {
        mff_cycles(0xB4B4, 4);                  /* BNE $B50A, always taken */
    }

    /* off the player's screen: walk back */
    mff_cycles(0xB50A, 10);
    a = RAM(OBJ_X_HI);
    if (a != RAM(OBJ_X_HI + x)) {
        mff_cycles(0xB512, 3);
        GO_IN(0xBA53, BANK_03);
    }
    mff_cycles(0xB510, 1);
    mff_cycles(0xB515, 8);
    a = RAM(OBJ_X + x);
    c = a >= 0xF2;
    if (!c) {
        mff_cycles(0xB51C, 4);
        c = a >= 0x16;
        if (c) {
            mff_cycles(0xB51E, 1);
            SYNC_OUT;
            obj_phase_jump(0xB523, PHASE_LO, PHASE_HI);
            return;
        }
    } else {
        mff_cycles(0xB51A, 1);
    }
    mff_cycles(0xB520, 3);
    GO_IN(0xBA53, BANK_03);
}

/*
 * abigail_start, $B546, phase 0: obj_ai_timer, obj_ai_count, obj_timer 0;
 * with the player in punching reach ($22 x $09) the uppercut (phase 7)
 * if the player is jumping, else the punches (phase 3), facing the
 * player; otherwise phase 1, standing.
 */
void func_B546_b1(void)
{
    REGS_IN;

    mff_cycles(0xB546, 27);
    a = 0x00;
    RAM(OBJ_AI_TIMER + x) = a;
    RAM(OBJ_AI_COUNT + x) = a;
    RAM(OBJ_TIMER + x) = a;
    a = REACH_PUNCH_X;
    y = REACH_PUNCH_Y;
    CALL_FIXED(0xB555, func_FB3E);              /* player_near */
    mff_cycles(0xB558, 2);
    if (!c) {
        mff_cycles(0xB558, 1);
        mff_cycles(0xB571, 14);
        RAM(OBJ_STATE + x)++;
        a = ANIM_STAND;
        y = CHR_ABIGAIL;
        GO_FIXED(0xF443, func_F443);
    }
    mff_cycles(0xB55A, 14);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = RAM(GAME_ACTION);
    if (a == ACT_JUMP) {
        mff_cycles(0xB563, 1);
        mff_cycles(0xB569, 2);
        a = ST_UPPERCUT;
    } else {
        mff_cycles(0xB565, 4);
        a = ST_PUNCHES;
        mff_cycles(0xB567, 1);
    }
    mff_cycles(0xB56B, 8);
    RAM(OBJ_STATE + x) = a;
    GO_FIXED(0xF343, func_F343);
}

/*
 * abigail_approach, $B57B, phase 1: the walk animation; a jumping player
 * within $22 x $08 gets the uppercut (phase 7). Once a close-in has begun
 * (obj_timer set) $B615 goes on with it. Within $30 x $10 he picks, by
 * rng & $0F: 0-5 close in to punch, 6-11 close in to grab (obj_ai_flag
 * $FF), 12-15 back off (phase 2). Farther, he walks at the player: along
 * y while the player's line is 8 or more away, else along x; a step
 * ($B331) and a turn ($B390).
 */
void func_B57B_b1(void)
{
    REGS_IN;

    mff_cycles(0xB57B, 8);
    a = ANIM_WALK;
    c = a >= RAM(OBJ_ANIM + x);
    if (a != RAM(OBJ_ANIM + x)) {
        mff_cycles(0xB582, 8);
        y = CHR_ABIGAIL;
        CALL_FIXED(0xB584, func_F443);
    } else {
        mff_cycles(0xB580, 1);
    }
    mff_cycles(0xB587, 7);
    a = RAM(GAME_ACTION);
    if (a == ACT_JUMP) {
        mff_cycles(0xB58D, 10);
        a = REACH_PUNCH_X;
        y = REACH_JUMP_Y;
        CALL_FIXED(0xB591, func_FB3E);
        mff_cycles(0xB594, 2);
        if (c) {
            mff_cycles(0xB596, 6);
            CALL_FIXED(0xB596, func_F343);
            mff_cycles(0xB599, 13);
            a = ST_UPPERCUT;
            RAM(OBJ_STATE + x) = a;
            RTS_RETURN();
        }
        mff_cycles(0xB594, 1);
    } else {
        mff_cycles(0xB58B, 1);
    }
    mff_cycles(0xB59F, 6);
    a = RAM(OBJ_TIMER + x);
    if (a != 0) {
        mff_cycles(0xB5A4, 3);
        GO_IN(0xB615, BANK_03);
    }
    mff_cycles(0xB5A2, 1);
    mff_cycles(0xB5A7, 10);
    a = NEAR_X;
    y = NEAR_Y;
    CALL_FIXED(0xB5AB, func_FB3E);
    mff_cycles(0xB5AE, 2);
    if (c) {
        mff_cycles(0xB5B0, 21);
        a = 0x00;
        RAM(OBJ_AI_FLAG + x) = a;
        RAM(OBJ_AI_TIMER + x) = a;
        a = RAM(RNG) & 0x0F;
        if (a < 0x06) {
            mff_cycles(0xB5C0, 10);
            RAM(OBJ_TIMER + x)++;
            GO_IN(0xB615, BANK_03);
        }
        mff_cycles(0xB5BE, 1);
        mff_cycles(0xB5C6, 4);
        if (a < 0x0C) {
            mff_cycles(0xB5CA, 17);
            RAM(OBJ_TIMER + x)++;
            RAM(OBJ_AI_FLAG + x)--;
            GO_IN(0xB615, BANK_03);
        }
        mff_cycles(0xB5C8, 1);
        mff_cycles(0xB5D3, 20);
        a = 0x00;
        RAM(OBJ_TIMER + x) = a;
        RAM(OBJ_STATE + x)++;
        RTS_RETURN();
    }
    mff_cycles(0xB5AE, 1);
    mff_cycles(0xB5DC, 6);
    CALL_FIXED(0xB5DC, func_FBD9);              /* A = y distance to the player's line */
    mff_cycles(0xB5DF, 4);
    c = a >= 0x08;
    if (c) {                                    /* walk along y */
        mff_cycles(0xB5E3, 8);
        y = WALK_UP;
        CALL_IN(0xB5E5, 0xB39B, BANK_03);
        mff_cycles(0xB5E8, 6);
        CALL_FIXED(0xB5E8, func_FBD9);          /* carry: the line is below */
        mff_cycles(0xB5EB, 2);
        if (c) {
            mff_cycles(0xB5EB, 1);
            mff_cycles(0xB5F1, 2);
            a = DIR_DOWN;
        } else {
            mff_cycles(0xB5ED, 4);
            a = DIR_UP;
            mff_cycles(0xB5EF, 1);
        }
        mff_cycles(0xB5F3, 5);
        RAM(OBJ_DIR + x) = a;
        SYNC_OUT;
        step_then_face(0xB5F6);
        return;
    }
    mff_cycles(0xB5E1, 1);                      /* walk along x */
    mff_cycles(0xB5FC, 8);
    y = WALK_ACROSS;
    CALL_IN(0xB5FE, 0xB39B, BANK_03);
    mff_cycles(0xB601, 6);
    CALL_FIXED(0xB601, func_F374);
    mff_cycles(0xB604, 2);
    if (c) {
        mff_cycles(0xB604, 1);
        mff_cycles(0xB60A, 2);
        a = DIR_RIGHT;
    } else {
        mff_cycles(0xB606, 4);
        a = DIR_LEFT;
        mff_cycles(0xB608, 1);
    }
    mff_cycles(0xB60C, 5);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    step_then_face(0xB60F);
}

/*
 * abigail_close_in, $B615 (bank $03; name proposed): the close-in of
 * phase 1. In reach (punching: $22 x $09; for a grab, obj_ai_flag set:
 * $25 x $0C) and the player not falling (game_action 6): the punches
 * (phase 3) or the grab (phase 5). Otherwise a step toward the player's
 * line, re-aimed every 8 frames (0F:FB5B's direction index: speed $B39B,
 * obj_dir $FBF0), and a turn.
 * Callers: JMPs at 03:B5A4, B5C3, B5D0.
 */
void func_B615_b1(void)
{
    REGS_IN;

    mff_cycles(0xB615, 6);
    a = RAM(OBJ_AI_FLAG + x);
    if (a == 0) {
        mff_cycles(0xB61A, 6);
        a = REACH_PUNCH_X;
        y = REACH_PUNCH_Y;
        mff_cycles(0xB61E, 1);
    } else {
        mff_cycles(0xB618, 1);
        mff_cycles(0xB620, 4);
        a = REACH_GRAB_X;
        y = REACH_GRAB_Y;
    }
    mff_cycles(0xB624, 6);
    CALL_FIXED(0xB624, func_FB3E);
    mff_cycles(0xB627, 2);
    if (c) {
        mff_cycles(0xB629, 7);
        a = RAM(GAME_ACTION);
        c = a >= ACT_FALL;
        if (a != ACT_FALL) {
            mff_cycles(0xB62F, 6);
            a = RAM(OBJ_AI_FLAG + x);
            if (a == 0) {
                mff_cycles(0xB634, 4);
                a = ST_PUNCHES;
                mff_cycles(0xB636, 1);
            } else {
                mff_cycles(0xB632, 1);
                mff_cycles(0xB638, 2);
                a = ST_GRAB;
            }
            mff_cycles(0xB63A, 18);
            RAM(OBJ_STATE + x) = a;
            a = 0x00;
            RAM(OBJ_TIMER + x) = a;
            RTS_RETURN();
        }
        mff_cycles(0xB62D, 1);
    } else {
        mff_cycles(0xB627, 1);
    }
    mff_cycles(0xB643, 6);
    a = RAM(OBJ_AI_TIMER + x);
    if (a != 0) {
        mff_cycles(0xB648, 10);
        RAM(OBJ_AI_TIMER + x)--;
        GO_IN(0xB660, BANK_03);
    }
    mff_cycles(0xB646, 1);
    mff_cycles(0xB64E, 13);
    a = 0x08;
    RAM(OBJ_AI_TIMER + x) = a;
    CALL_FIXED(0xB653, func_FB5B);              /* obj_dir_index */
    mff_cycles(0xB656, 8);
    y = a;
    CALL_IN(0xB657, 0xB39B, BANK_03);
    mff_cycles(0xB65A, 4);
    a = prg_read_ix(DIR_BY_INDEX, y);
    mff_cycles(0xB65D, 5);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    func_B660_b1();
}

/*
 * $B660 (bank $03): a step and a turn to the player (step_then_face).
 * Callers: JMP at 03:B64B; the fall-through from 03:B65D.
 */
void func_B660_b1(void)
{
    step_then_face(0xB660);
}

/*
 * abigail_back_off, $B666, phase 2: the walk animation. obj_timer 0 picks
 * a way away from the player, by rng & $0F: 0-4 the diagonal up
 * (direction index 3), 5-10 straight across (4), 11-15 the diagonal down
 * (5), mirrored (+8) when the player is to the right, and walks it
 * $30, $48 or $60 frames (rng+1 & 7: 0, odd, even). Then each frame a step
 * and a turn; leaving x $17-$F0, the time running out or a rope (TMP15)
 * end it: phase 6.
 */
void func_B666_b1(void)
{
    REGS_IN;

    mff_cycles(0xB666, 8);
    a = ANIM_WALK;
    c = a >= RAM(OBJ_ANIM + x);
    if (a != RAM(OBJ_ANIM + x)) {
        mff_cycles(0xB66D, 8);
        y = CHR_ABIGAIL;
        CALL_FIXED(0xB66F, func_F443);
    } else {
        mff_cycles(0xB66B, 1);
    }
    mff_cycles(0xB672, 6);
    a = RAM(OBJ_TIMER + x);
    if (a != 0) {
        mff_cycles(0xB675, 1);
        mff_cycles(0xB6D0, 8);
        a = RAM(OBJ_X + x);
        c = a >= RING_RIGHT;
        if (!c) {
            mff_cycles(0xB6D7, 4);
            c = a >= RING_LEFT;
            if (c) {
                mff_cycles(0xB6DB, 9);
                RAM(OBJ_AI_TIMER + x)--;
                if (RAM(OBJ_AI_TIMER + x) != 0) {
                    mff_cycles(0xB6E0, 6);
                    CALL_IN(0xB6E0, 0xB331, BANK_03);
                    mff_cycles(0xB6E3, 6);
                    CALL_IN(0xB6E3, 0xB390, BANK_03);
                    mff_cycles(0xB6E6, 5);
                    a = RAM(TMP15);
                    if (a == 0) {
                        mff_cycles(0xB6E8, 1);
                        mff_cycles(0xB6F4, 6);
                        RTS_RETURN();
                    }
                } else {
                    mff_cycles(0xB6DE, 1);
                }
            } else {
                mff_cycles(0xB6D9, 1);
            }
        } else {
            mff_cycles(0xB6D5, 1);
        }
        mff_cycles(0xB6EA, 14);
        a = 0x00;
        RAM(OBJ_TIMER + x) = a;
        a = ST_DECIDE;
        RAM(OBJ_STATE + x) = a;
        mff_cycles(0xB6F4, 6);
        RTS_RETURN();
    }
    mff_cycles(0xB677, 9);
    a = RAM(RNG) & 0x0F;
    c = a >= 0x05;
    if (!c) {
        mff_cycles(0xB67F, 9);
        a = 0x03;
        RAM(OBJ_DIR + x) = a;
        mff_cycles(0xB684, 1);
        mff_cycles(0xB69B, 8);
        y = 0x05;
        CALL_IN(0xB69D, 0xB39B, BANK_03);
    } else {
        mff_cycles(0xB67D, 1);
        mff_cycles(0xB686, 4);
        c = a >= 0x0B;
        if (!c) {
            mff_cycles(0xB68A, 8);
            y = 0x04;
            CALL_IN(0xB68C, 0xB39B, BANK_03);
            mff_cycles(0xB68F, 9);
            a = 0x04;
            RAM(OBJ_DIR + x) = a;
            mff_cycles(0xB694, 1);
        } else {
            mff_cycles(0xB688, 1);
            mff_cycles(0xB696, 7);
            a = 0x05;
            RAM(OBJ_DIR + x) = a;
            mff_cycles(0xB69B, 8);
            y = 0x05;
            CALL_IN(0xB69D, 0xB39B, BANK_03);
        }
    }
    mff_cycles(0xB6A0, 6);
    CALL_FIXED(0xB6A0, func_F374);
    mff_cycles(0xB6A3, 2);
    if (c) {                                    /* the player to the right: away = left */
        mff_cycles(0xB6A5, 11);
        a = RAM(OBJ_DIR + x);
        ADC_A(0x07);                            /* + 7 + carry */
        RAM(OBJ_DIR + x) = a;
    } else {
        mff_cycles(0xB6A3, 1);
    }
    mff_cycles(0xB6AD, 8);
    y = RAM(OBJ_DIR + x);
    a = prg_read_ix(DIR_BY_INDEX, y);
    mff_cycles(0xB6B3, 12);
    RAM(OBJ_DIR + x) = a;
    a = RAM(RNG + 1) & 0x07;
    if (a == 0) {
        mff_cycles(0xB6BC, 4);
        a = 0x30;
        mff_cycles(0xB6BE, 1);
    } else {
        mff_cycles(0xB6BA, 1);
        mff_cycles(0xB6C0, 4);
        c = a & 0x01;                           /* LSR */
        a >>= 1;
        if (c) {
            mff_cycles(0xB6C1, 1);
            mff_cycles(0xB6C7, 2);
            a = 0x60;
        } else {
            mff_cycles(0xB6C3, 4);
            a = 0x48;
            mff_cycles(0xB6C5, 1);
        }
    }
    mff_cycles(0xB6C9, 18);
    RAM(OBJ_AI_TIMER + x) = a;
    RAM(OBJ_TIMER + x)++;
    RTS_RETURN();
}

/*
 * $B7A0: the punches give up: phase 2 (DEC obj_state), obj_timer 0; RTS.
 */
static void punches_give_up(void)
{
    REGS_IN;

    mff_cycles(0xB7A0, 20);
    RAM(OBJ_STATE + x)--;
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    RTS_RETURN();
}

/*
 * abigail_punches, $B6F5, phase 3. obj_timer 0: obj_ai_count = 3 punches
 * (rng & 7 not 0) or 1, obj_timer2 0, facing the player. obj_timer 1: a
 * pause of obj_ai_timer frames standing (a player who jumps on its last
 * frame gets the uppercut), then the punch animation; on its frame 1 the
 * attack is out ($8C, damage 7, $0F for the last of three, marked by
 * obj_timer2 $FF), a hit sounds $18. When a punch ends (frame not 0 and
 * its timer at 7): with punches left and the player still in reach, a
 * pause of $12 frames; out of reach, back off (phase 2); after the last
 * one, obj_timer 2 ($B7BD) waits $10 frames before backing off.
 */
void func_B6F5_b1(void)
{
    REGS_IN;

    mff_cycles(0xB6F5, 6);
    a = RAM(OBJ_TIMER + x);
    if (a == 0) {
        mff_cycles(0xB6F8, 2);
        mff_cycles(0xB701, 14);
        a = 0x00;
        RAM(OBJ_TIMER2 + x) = a;
        a = RAM(RNG) & 0x07;
        if (a != 0) {
            mff_cycles(0xB70C, 7);
            a = 0x02;
            RAM(OBJ_AI_COUNT + x) = a;
        } else {
            mff_cycles(0xB70A, 1);
        }
        mff_cycles(0xB711, 24);
        RAM(OBJ_AI_COUNT + x)++;
        a = 0x00;
        RAM(OBJ_AI_TIMER + x) = a;
        RAM(OBJ_TIMER + x)++;
        GO_FIXED(0xF343, func_F343);
    }
    mff_cycles(0xB6FA, 4);
    c = a >= 0x01;
    if (a != 0x01) {
        mff_cycles(0xB6FE, 3);
        GO_IN(0xB7BD, BANK_03);
    }
    mff_cycles(0xB6FC, 2);

    /* the pause before a punch */
    mff_cycles(0xB71F, 6);
    a = RAM(OBJ_AI_TIMER + x);
    if (a != 0) {
        mff_cycles(0xB724, 11);
        RAM(OBJ_AI_TIMER + x)--;
        c = a >= 0x01;
        if (a == 0x01) {
            mff_cycles(0xB72B, 5);
            c = a >= RAM(GAME_ACTION);
            if (a == RAM(GAME_ACTION)) {        /* game_action 1: jumping */
                mff_cycles(0xB72F, 6);
                CALL_FIXED(0xB72F, func_F343);
                mff_cycles(0xB732, 20);
                a = 0x00;
                RAM(OBJ_TIMER + x) = a;
                a = ST_UPPERCUT;
                RAM(OBJ_STATE + x) = a;
                RTS_RETURN();
            }
            mff_cycles(0xB72D, 1);
        } else {
            mff_cycles(0xB729, 1);
        }
        mff_cycles(0xB73D, 7);
        a = ANIM_STAND;
        y = CHR_ABIGAIL;
        GO_FIXED(0xF443, func_F443);
    }
    mff_cycles(0xB722, 1);

    /* the punch */
    mff_cycles(0xB744, 8);
    a = ANIM_PUNCH;
    c = a >= RAM(OBJ_ANIM + x);
    if (a != RAM(OBJ_ANIM + x)) {
        mff_cycles(0xB74B, 8);
        y = CHR_ABIGAIL_PUNCH;
        CALL_FIXED(0xB74D, func_F443);
        mff_cycles(0xB750, 7);
        RAM(OBJ_ANIM_TIMER + x)++;
    } else {
        mff_cycles(0xB749, 1);
    }
    mff_cycles(0xB753, 6);
    a = RAM(OBJ_ANIM_FRAME + x);
    if (a != 0) {
        mff_cycles(0xB758, 8);
        a = RAM(OBJ_ANIM_TIMER + x);
        if (a == 0x07) {                        /* the punch is over */
            mff_cycles(0xB75D, 1);
            mff_cycles(0xB789, 9);
            RAM(OBJ_AI_COUNT + x)--;
            if (RAM(OBJ_AI_COUNT + x) == 0) {
                mff_cycles(0xB78E, 20);
                RAM(OBJ_TIMER + x)++;
                a = 0x10;
                RAM(OBJ_AI_TIMER + x) = a;
                RTS_RETURN();
            }
            mff_cycles(0xB78C, 1);
            mff_cycles(0xB797, 10);
            a = REACH_PUNCH_X;
            y = REACH_PUNCH_Y;
            CALL_FIXED(0xB79B, func_FB3E);
            mff_cycles(0xB79E, 2);
            if (!c) {
                SYNC_OUT;
                punches_give_up();
                return;
            }
            mff_cycles(0xB79E, 1);
            mff_cycles(0xB7A9, 8);
            a = RAM(OBJ_AI_COUNT + x);
            if (a == 0x01) {                    /* the last one hits harder */
                mff_cycles(0xB7B0, 7);
                a = 0xFF;
                RAM(OBJ_TIMER2 + x) = a;
            } else {
                mff_cycles(0xB7AE, 1);
            }
            mff_cycles(0xB7B5, 10);
            a = 0x12;
            RAM(OBJ_AI_TIMER + x) = a;
            GO_FIXED(0xF343, func_F343);
        }
    } else {
        mff_cycles(0xB756, 1);
    }
    mff_cycles(0xB75F, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    if (a == 0x01) {
        mff_cycles(0xB766, 13);
        a = ATK_PUNCH;
        RAM(OBJ_ATTACK + x) = a;
        a = RAM(OBJ_TIMER2 + x);
        if (a == 0) {
            mff_cycles(0xB770, 4);
            a = DMG_PUNCH;
            mff_cycles(0xB772, 1);
        } else {
            mff_cycles(0xB76E, 1);
            mff_cycles(0xB774, 2);
            a = DMG_PUNCH_LAST;
        }
        mff_cycles(0xB776, 5);
        RAM(OBJ_ATTACK_DAMAGE + x) = a;
    } else {
        mff_cycles(0xB764, 1);
    }
    mff_cycles(0xB779, 6);
    a = RAM(OBJ_HIT_PLAYER + x);
    if (a != 0) {
        mff_cycles(0xB77E, 8);
        a = SFX_PUNCH_HIT;
        CALL_FIXED(0xB780, func_F34C);          /* sound_queue_push_sfx */
        mff_cycles(0xB783, 7);
        a = 0x00;
        RAM(OBJ_HIT_PLAYER + x) = a;
    } else {
        mff_cycles(0xB77C, 1);
    }
    mff_cycles(0xB788, 6);
    RTS_RETURN();
}

/*
 * $B7BD (bank $03): phase 3, obj_timer 2: stand obj_ai_timer frames after
 * the last punch, then back off.
 * Caller: JMP at 03:B6FE.
 */
void func_B7BD_b1(void)
{
    REGS_IN;

    mff_cycles(0xB7BD, 9);
    RAM(OBJ_AI_TIMER + x)--;
    if (RAM(OBJ_AI_TIMER + x) == 0) {
        mff_cycles(0xB7A0, 1);
        SYNC_OUT;
        punches_give_up();
        return;
    }
    mff_cycles(0xB7C2, 7);
    a = ANIM_STAND;
    y = CHR_ABIGAIL;
    GO_FIXED(0xF443, func_F443);
}

/*
 * abigail_charge, $B7C9, phase 4, by obj_timer:
 *   0 stand facing the player until the animation comes round (frame and
 *     timer 0), then 1;
 *   1 aim ($BB29: a direction at least two steps off vertical, the rush
 *     speed and its slow-down), the wind-up ($DD), obj_timer 2;
 *   2 step until near the ring's side (x $17-$F0) or within $30 of the
 *     player in x: the rush animation ($DE), damage $0E, obj_timer 3;
 *   3 the rush: attack $8B while obj_vx is not 0; obj_vx and obj_vy lose
 *     obj_timer2 and obj_ground_y /256 a frame; while both stay positive a
 *     step, then phase 1, no grab for 4 frames (obj_tier).
 * A hit sends him here with obj_timer 1 (03:B495).
 */
void func_B7C9_b1(void)
{
    REGS_IN;

    mff_cycles(0xB7C9, 8);
    a = RAM(OBJ_TIMER + x);
    if (a != 0x01) {
        mff_cycles(0xB7D0, 4);
        c = a >= 0x02;
        if (a == 0x02) {
            mff_cycles(0xB7D2, 2);
            /* 2: run up to the player */
            mff_cycles(0xB802, 6);
            CALL_IN(0xB802, 0xB331, BANK_03);
            mff_cycles(0xB805, 8);
            a = RAM(OBJ_X + x);
            c = a >= RING_RIGHT;
            if (!c) {
                mff_cycles(0xB80C, 4);
                c = a >= RING_LEFT;
                if (c) {
                    mff_cycles(0xB810, 6);
                    CALL_FIXED(0xB810, func_F374);
                    mff_cycles(0xB813, 4);
                    c = a >= NEAR_X;
                    if (c) {
                        mff_cycles(0xB815, 1);
                        mff_cycles(0xB826, 6);
                        RTS_RETURN();
                    }
                } else {
                    mff_cycles(0xB80E, 1);
                }
            } else {
                mff_cycles(0xB80A, 1);
            }
            mff_cycles(0xB817, 10);
            a = ANIM_CHARGE;
            y = CHR_ABIGAIL_PUNCH;
            CALL_FIXED(0xB81B, func_F443);
            mff_cycles(0xB81E, 14);
            a = DMG_CHARGE;
            RAM(OBJ_ATTACK_DAMAGE + x) = a;
            RAM(OBJ_TIMER + x)++;
            mff_cycles(0xB826, 6);
            RTS_RETURN();
        }
        mff_cycles(0xB7D4, 4);
        c = a >= 0x03;
        if (a == 0x03) {
            mff_cycles(0xB7D6, 2);
            /* 3: the rush, slowing down */
            mff_cycles(0xB827, 6);
            a = RAM(OBJ_VX + x);
            if (a != 0) {
                mff_cycles(0xB82C, 2);
                a = ATK_CHARGE;
            } else {
                mff_cycles(0xB82A, 1);
            }
            mff_cycles(0xB82E, 63);
            RAM(OBJ_ATTACK + x) = a;
            a = RAM(OBJ_VX_FRAC + x);
            c = 1;
            SBC_A(RAM(OBJ_TIMER2 + x));
            RAM(OBJ_VX_FRAC + x) = a;
            a = RAM(OBJ_VX + x);
            SBC_A(0x00);
            RAM(OBJ_VX + x) = a;
            a = RAM(OBJ_VY_FRAC + x);
            c = 1;
            SBC_A(RAM(OBJ_GROUND_Y + x));
            RAM(OBJ_VY_FRAC + x) = a;
            a = RAM(OBJ_VY + x);
            SBC_A(0x00);
            RAM(OBJ_VY + x) = a;
            a = RAM(OBJ_VX + x);
            if (!(a & 0x80)) {
                mff_cycles(0xB85A, 6);
                a = RAM(OBJ_VY + x);
                if (!(a & 0x80)) {
                    mff_cycles(0xB85F, 6);
                    CALL_IN(0xB85F, 0xB331, BANK_03);
                    mff_cycles(0xB862, 6);
                    RTS_RETURN();
                }
                mff_cycles(0xB85D, 1);
            } else {
                mff_cycles(0xB858, 1);
            }
            mff_cycles(0xB863, 37);
            a = ST_APPROACH;
            RAM(OBJ_STATE + x) = a;
            a = 0x00;
            RAM(OBJ_TIMER + x) = a;
            RAM(OBJ_TIMER2 + x) = a;
            RAM(OBJ_GROUND_Y + x) = a;
            a = 0x04;
            RAM(OBJ_TIER + x) = a;
            RTS_RETURN();
        }
        /* 0: wait for the stand animation to come round */
        mff_cycles(0xB7D8, 6);
        CALL_FIXED(0xB7D8, func_F343);
        mff_cycles(0xB7DB, 8);
        a = ANIM_STAND;
        c = a >= RAM(OBJ_ANIM + x);
        if (a != RAM(OBJ_ANIM + x)) {
            mff_cycles(0xB7E2, 8);
            y = CHR_ABIGAIL;
            CALL_FIXED(0xB7E4, func_F443);
            mff_cycles(0xB7E7, 7);
            RAM(OBJ_ANIM_TIMER + x)++;
        } else {
            mff_cycles(0xB7E0, 1);
        }
        mff_cycles(0xB7EA, 10);
        a = RAM(OBJ_ANIM_FRAME + x) | RAM(OBJ_ANIM_TIMER + x);
        if (a != 0) {
            mff_cycles(0xB7F0, 2);
            mff_cycles(0xB801, 6);
            RTS_RETURN();
        }
    } else {
        mff_cycles(0xB7CE, 1);
    }
    /* 1: aim and wind up */
    mff_cycles(0xB7F2, 6);
    CALL_IN(0xB7F2, 0xBB29, BANK_03);
    mff_cycles(0xB7F5, 10);
    a = ANIM_WIND_UP;
    y = CHR_ABIGAIL;
    CALL_FIXED(0xB7F9, func_F443);
    mff_cycles(0xB7FC, 7);
    a = 0x02;
    RAM(OBJ_TIMER + x) = a;
    mff_cycles(0xB801, 6);
    RTS_RETURN();
}

/*
 * abigail_grab, $B879, phase 5, obj_timer 0: facing the player. No grab
 * while obj_tier counts, while the player cannot be hit (player_no_hit
 * bit 7) or is in game_action $0B, $25, $16, $17 or $1F, or with Abigail
 * at the ring's side he faces (x >= $ED facing right, < $13 facing left):
 * back off (phase 2). A player off the ground line (obj_y not
 * player_ground_y) gets the punches, or the uppercut when jumping.
 * Otherwise the catch: slots 1 and 2 emptied (the dropped weapon, display
 * objects), Abigail untouchable (obj_invuln $FF), his y kept in
 * obj_timer2, the player turned to face him (player_dir, 0F:F048/F03F),
 * game_action $0A (held), three blows to come (obj_ai_count), obj_timer
 * 1, the grab animation ($91, bank $3E) at $B912. Later frames: $B925.
 */
static const uint8_t k_no_grab_actions[] = { 0x0B, ACT_FALL_INVULN, 0x16, ACT_SPECIAL, ACT_LYING };

void func_B879_b1(void)
{
    REGS_IN;

    mff_cycles(0xB879, 6);
    a = RAM(OBJ_TIMER + x);
    if (a != 0) {
        mff_cycles(0xB87E, 3);
        GO_IN(0xB925, BANK_03);
    }
    mff_cycles(0xB87C, 1);
    mff_cycles(0xB881, 6);
    CALL_FIXED(0xB881, func_F343);
    mff_cycles(0xB884, 6);
    a = RAM(OBJ_TIER + x);
    if (a == 0) {
        mff_cycles(0xB889, 5);
        a = RAM(PLAYER_NO_HIT);
        if (!(a & 0x80)) {
            int at_side;
            mff_cycles(0xB88D, 3);
            a = RAM(GAME_ACTION);
            if (cmp_list(0xB88F, 0xB8BA, a, k_no_grab_actions, 5) < 5)
                goto back_off;
            mff_cycles(0xB8A3, 8);
            a = RAM(OBJ_FLAGS + x) & OBJF_FACE_RIGHT;
            if (a != 0) {
                mff_cycles(0xB8AA, 8);
                a = RAM(OBJ_X + x);
                at_side = a >= 0xED;
                if (at_side)
                    mff_cycles(0xB8AF, 1);
                else
                    mff_cycles(0xB8B1, 3);      /* BCS not taken, BCC taken */
            } else {
                mff_cycles(0xB8A8, 1);
                mff_cycles(0xB8B3, 8);
                a = RAM(OBJ_X + x);
                at_side = a < 0x13;
                if (!at_side)
                    mff_cycles(0xB8B8, 1);
            }
            if (!at_side) {
                mff_cycles(0xB8C5, 9);
                a = RAM(OBJ_Y);
                if (a != RAM(PLAYER_GROUND_Y)) {   /* the player is off the ground */
                    mff_cycles(0xB8CC, 7);
                    a = RAM(GAME_ACTION);
                    if (a == ACT_JUMP) {
                        mff_cycles(0xB8D0, 1);
                        mff_cycles(0xB8D6, 2);
                        a = ST_UPPERCUT;
                    } else {
                        mff_cycles(0xB8D2, 4);
                        a = ST_PUNCHES;
                        mff_cycles(0xB8D4, 1);
                    }
                    mff_cycles(0xB8D8, 18);
                    RAM(OBJ_STATE + x) = a;
                    a = 0x00;
                    RAM(OBJ_TIMER + x) = a;
                    RTS_RETURN();
                }
                /* the catch */
                mff_cycles(0xB8CA, 1);
                mff_cycles(0xB8E1, 34);
                a = 0x00;
                RAM(OBJ_STATE + 1) = a;
                RAM(OBJ_STATE + 2) = a;
                a = 0xFF;
                RAM(OBJ_INVULN + x) = a;
                a = RAM(OBJ_Y + x);
                RAM(OBJ_TIMER2 + x) = a;
                a = RAM(OBJ_DIR + x);
                c = a & 0x01;                   /* LSR */
                a >>= 1;
                if (!c) {
                    mff_cycles(0xB8FA, 4);
                    a = DIR_RIGHT;
                    mff_cycles(0xB8FC, 2);
                } else {
                    mff_cycles(0xB8F8, 1);
                    mff_cycles(0xB8FE, 2);
                    a = DIR_LEFT;
                }
                mff_cycles(0xB900, 11);
                RAM(PLAYER_DIR) = a;
                a = RAM(OBJ_FLAGS + x) & OBJF_FACE_RIGHT;
                if (a != 0) {
                    mff_cycles(0xB909, 6);
                    CALL_FIXED(0xB909, func_F048);  /* player_face_left */
                    mff_cycles(0xB90C, 3);
                    GO_IN(0xB912, BANK_03);
                }
                mff_cycles(0xB907, 1);
                mff_cycles(0xB90F, 6);
                CALL_FIXED(0xB90F, func_F03F);  /* player_face_right */
                SYNC_OUT;
                func_B912_b1();
                return;
            }
            goto back_off;
        }
        mff_cycles(0xB88B, 1);
    } else {
        mff_cycles(0xB887, 1);
    }
back_off:
    mff_cycles(0xB8BA, 20);
    a = ST_BACK_OFF;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    RTS_RETURN();
}

/*
 * $B912 (bank $03): the catch made: game_action $0A, three blows, obj_timer
 * 1, the grab animation.
 * Callers: JMP at 03:B90C; the fall-through from 03:B911.
 */
void func_B912_b1(void)
{
    REGS_IN;

    mff_cycles(0xB912, 26);
    a = ACT_HELD;
    RAM(GAME_ACTION) = a;
    a = 0x03;
    RAM(OBJ_AI_COUNT + x) = a;
    RAM(OBJ_TIMER + x)++;
    a = ANIM_GRAB;
    y = CHR_ABIGAIL_GRAB;
    GO_FIXED(0xF443, func_F443);
}

/*
 * abigail_grab_blows, $B925 (bank $03; name proposed): phase 5, obj_timer
 * 1 and on. Past the first frame the player shows the held pose ($E207 /
 * $E24F by player_id). On the grab animation's frame 3 obj_timer counts
 * on, and when that frame starts a blow lands: sound $1F, obj_ai_count -
 * 1, player_invuln $3C and player_take_hit with damage 7, the third with
 * damage $0F, the player's obj_state $80 and game_action 6 (thrown).
 * Then $B977.
 * Caller: JMP at 03:B87E.
 */
void func_B925_b1(void)
{
    REGS_IN;

    mff_cycles(0xB925, 8);
    a = RAM(OBJ_TIMER + x);
    c = a >= 0x01;
    if (a != 0x01) {
        mff_cycles(0xB92C, 7);
        y = RAM(PLAYER_ID);
        a = prg_read_ix(PLAYER_HELD_ANIM, y);
        mff_cycles(0xB931, 6);
        CALL_FIXED(0xB931, func_F294);          /* player_set_anim */
        mff_cycles(0xB934, 4);
        a = prg_read_ix(PLAYER_HELD_CHR, y);
        SET_NZ(a);
        mff_cycles(0xB937, 6);
        CALL_FIXED(0xB937, func_F440);          /* sprite bank R2 = A */
    } else {
        mff_cycles(0xB92A, 1);
    }
    mff_cycles(0xB93A, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    if (a == 0x03) {
        mff_cycles(0xB941, 13);
        RAM(OBJ_TIMER + x)++;
        a = RAM(OBJ_ANIM_TIMER + x);
        if (a == 0) {
            mff_cycles(0xB949, 8);
            a = SFX_GRAB_BLOW;
            CALL_FIXED(0xB94B, func_F34C);
            mff_cycles(0xB94E, 9);
            RAM(OBJ_AI_COUNT + x)--;
            if (RAM(OBJ_AI_COUNT + x) != 0) {
                mff_cycles(0xB953, 18);
                a = DMG_GRAB_BLOW;
                RAM(OBJ_ATTACK_DAMAGE + x) = a;
                a = PLAYER_HIT_INVULN;
                RAM(PLAYER_INVULN) = a;
                CALL_FIXED(0xB95C, func_F8D5);  /* player_take_hit */
                mff_cycles(0xB95F, 3);
                GO_IN(0xB977, BANK_03);
            }
            mff_cycles(0xB951, 1);
            mff_cycles(0xB962, 18);
            a = DMG_GRAB_LAST;
            RAM(OBJ_ATTACK_DAMAGE + x) = a;
            a = PLAYER_HIT_INVULN;
            RAM(PLAYER_INVULN) = a;
            CALL_FIXED(0xB96B, func_F8D5);
            mff_cycles(0xB96E, 11);
            a = 0x80;
            RAM(OBJ_STATE) = a;
            a = ACT_FALL;
            RAM(GAME_ACTION) = a;
        } else {
            mff_cycles(0xB947, 1);
        }
    } else {
        mff_cycles(0xB93F, 1);
    }
    SYNC_OUT;
    func_B977_b1();
}

/*
 * $BA08-$BA15: the held player's y, attack and combo off, game_action $0A;
 * RTS. In: A = the player's y.
 */
static void hold_player_tail(void)
{
    REGS_IN;

    mff_cycles(0xBA08, 23);
    RAM(OBJ_Y) = a;
    a = 0x00;
    RAM(ATTACK_LIVE) = a;
    RAM(COMBO_TIMER) = a;
    a = ACT_HELD;
    RAM(GAME_ACTION) = a;
    RTS_RETURN();
}

/*
 * abigail_hold, $B977 (bank $03; name proposed): the rest of a grab frame.
 * Thrown (game_action 6): the player falls toward Abigail's line
 * (player_ground_y = obj_timer2) in the thrown pose ($E20A/$E252); when
 * the grab animation reaches frame 4, timer 5, back to phase 1,
 * obj_invuln 0. Still held: after the first frame the animation loops
 * frames 1-4, and the player is held at x +/- $BA16 and y - $BA19 by
 * (frame + 1) / 2, in front of him, with game_action $0A.
 * Callers: JMP at 03:B95F; the fall-through from 03:B976.
 */
void func_B977_b1(void)
{
    REGS_IN;

    mff_cycles(0xB977, 7);
    a = RAM(GAME_ACTION);
    c = a >= ACT_FALL;
    if (a == ACT_FALL) {
        mff_cycles(0xB97D, 14);
        a = RAM(OBJ_TIMER2 + x);
        RAM(PLAYER_GROUND_Y) = a;
        y = RAM(PLAYER_ID);
        a = prg_read_ix(PLAYER_THROWN_ANIM, y);
        mff_cycles(0xB987, 6);
        CALL_FIXED(0xB987, func_F294);
        mff_cycles(0xB98A, 4);
        a = prg_read_ix(PLAYER_THROWN_CHR, y);
        SET_NZ(a);
        mff_cycles(0xB98D, 6);
        CALL_FIXED(0xB98D, func_F440);
        mff_cycles(0xB990, 8);
        a = RAM(OBJ_ANIM_FRAME + x);
        if (a == 0x04) {
            mff_cycles(0xB997, 8);
            a = RAM(OBJ_ANIM_TIMER + x);
            if (a == 0x05) {
                mff_cycles(0xB99E, 19);
                a = ST_APPROACH;
                RAM(OBJ_STATE + x) = a;
                a = 0x00;
                RAM(OBJ_TIMER + x) = a;
                RAM(OBJ_INVULN + x) = a;
            } else {
                mff_cycles(0xB99C, 1);
            }
        } else {
            mff_cycles(0xB995, 1);
        }
        mff_cycles(0xB9AB, 6);
        RTS_RETURN();
    }
    mff_cycles(0xB97B, 1);
    mff_cycles(0xB9AC, 8);
    a = RAM(OBJ_TIMER + x);
    if (a != 0x01) {
        mff_cycles(0xB9B3, 8);
        a = RAM(OBJ_ANIM_FRAME + x);
        if (a == 0x04) {
            mff_cycles(0xB9BA, 8);
            a = RAM(OBJ_ANIM_TIMER + x);
            if (a == 0x05) {
                mff_cycles(0xB9C1, 14);
                a = 0x01;
                RAM(OBJ_ANIM_FRAME + x) = a;
                a = 0x00;
                RAM(OBJ_ANIM_TIMER + x) = a;
            } else {
                mff_cycles(0xB9BF, 1);
            }
        } else {
            mff_cycles(0xB9B8, 1);
        }
    } else {
        mff_cycles(0xB9B1, 1);
    }
    mff_cycles(0xB9CB, 20);
    a = (uint8_t)(RAM(OBJ_ANIM_FRAME + x) + 1);
    a >>= 1;
    y = a;
    a = RAM(OBJ_FLAGS + x) & OBJF_FACE_RIGHT;
    if (a != 0) {                               /* in front: to the right */
        mff_cycles(0xB9DA, 10);
        a = RAM(OBJ_X + x);
        c = 0;
        ADC_A(prg_read_ix(HOLD_DX, y));
        mff_cycles(0xB9E1, 17);
        RAM(OBJ_X) = a;
        a = RAM(OBJ_X_HI + x);
        ADC_A(0x00);
        RAM(OBJ_X_HI) = a;
        GO_IN(0xBA01, BANK_03);
    }
    mff_cycles(0xB9D8, 1);
    mff_cycles(0xB9EF, 10);
    a = RAM(OBJ_X + x);
    c = 1;
    SBC_A(prg_read_ix(HOLD_DX, y));
    mff_cycles(0xB9F6, 24);
    RAM(OBJ_X) = a;
    a = RAM(OBJ_X_HI + x);
    SBC_A(0x00);
    RAM(OBJ_X_HI) = a;
    a = RAM(OBJ_Y + x);
    c = 1;
    SBC_A(prg_read_ix(HOLD_DY, y));
    SYNC_OUT;
    hold_player_tail();
}

/*
 * $BA01 (bank $03): the held player's y = Abigail's - $BA19[Y], then
 * hold_player_tail.
 * Caller: JMP at 03:B9EC.
 */
void func_BA01_b1(void)
{
    REGS_IN;

    mff_cycles(0xBA01, 10);
    a = RAM(OBJ_Y + x);
    c = 1;
    SBC_A(prg_read_ix(HOLD_DY, y));
    SYNC_OUT;
    hold_player_tail();
}

/*
 * abigail_decide, $BA1C, phase 6: the odds of a charge grow with the
 * distance to the player in x: rng & $0F below 4 (closer than $30), 8
 * (closer than $48), $0A (closer than $60) or $0E: the charge (phase 4),
 * unless the player is falling; else the approach (phase 1).
 */
void func_BA1C_b1(void)
{
    REGS_IN;

    mff_cycles(0xBA1C, 6);
    CALL_FIXED(0xBA1C, func_F374);              /* A = |dx| */
    mff_cycles(0xBA1F, 4);
    if (a < 0x30) {
        mff_cycles(0xBA23, 4);
        a = 0x04;
        mff_cycles(0xBA25, 1);
    } else if (mff_cycles(0xBA21, 1), mff_cycles(0xBA27, 4), a < 0x48) {
        mff_cycles(0xBA2B, 4);
        a = 0x08;
        mff_cycles(0xBA2D, 1);
    } else if (mff_cycles(0xBA29, 1), mff_cycles(0xBA2F, 4), a < 0x60) {
        mff_cycles(0xBA33, 4);
        a = 0x0A;
        mff_cycles(0xBA35, 1);
    } else {
        mff_cycles(0xBA31, 1);
        mff_cycles(0xBA37, 2);
        a = 0x0E;
    }
    mff_cycles(0xBA39, 13);
    RAM(TMP0) = a;
    a = RAM(RNG) & 0x0F;
    if (a < RAM(TMP0)) {
        mff_cycles(0xBA43, 7);
        a = RAM(GAME_ACTION);
        if (a != ACT_FALL) {
            mff_cycles(0xBA49, 4);
            a = ST_CHARGE;
            mff_cycles(0xBA4B, 1);
            mff_cycles(0xBA4F, 11);
            RAM(OBJ_STATE + x) = a;
            RTS_RETURN();
        }
        mff_cycles(0xBA47, 1);
    } else {
        mff_cycles(0xBA41, 1);
    }
    mff_cycles(0xBA4D, 2);
    a = ST_APPROACH;
    mff_cycles(0xBA4F, 11);
    RAM(OBJ_STATE + x) = a;
    RTS_RETURN();
}

/*
 * abigail_walk_back, $BA53 (bank $03; name proposed): Abigail off the
 * player's screen page, or outside x $16-$F1: the walk animation, 8 frames
 * untouchable, phase 1, facing the player, a straight walk across toward
 * the player (right when his x high byte is $FF).
 * Callers: JMPs at 03:B512, B520.
 */
void func_BA53_b1(void)
{
    REGS_IN;

    mff_cycles(0xBA53, 8);
    a = ANIM_WALK;
    c = a >= RAM(OBJ_ANIM + x);
    if (a != RAM(OBJ_ANIM + x)) {
        mff_cycles(0xBA5A, 8);
        y = CHR_ABIGAIL;
        CALL_FIXED(0xBA5C, func_F443);
    } else {
        mff_cycles(0xBA58, 1);
    }
    mff_cycles(0xBA5F, 27);
    a = 0x08;
    RAM(OBJ_INVULN + x) = a;
    a = ST_APPROACH;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    CALL_IN(0xBA6E, 0xB390, BANK_03);
    mff_cycles(0xBA71, 8);
    y = WALK_ACROSS;
    CALL_IN(0xBA73, 0xB39B, BANK_03);
    mff_cycles(0xBA76, 8);
    a = RAM(OBJ_X_HI + x);
    if (a != 0xFF) {
        mff_cycles(0xBA7D, 6);
        CALL_FIXED(0xBA7D, func_F374);
        mff_cycles(0xBA80, 2);
        if (!c) {
            mff_cycles(0xBA82, 3);
            GO_FIXED(0xF1A1, func_F1A1);        /* obj_move_left */
        }
        mff_cycles(0xBA80, 1);
    } else {
        mff_cycles(0xBA7B, 1);
    }
    mff_cycles(0xBA85, 3);
    GO_FIXED(0xF183, func_F183);                /* obj_move_right */
}

/*
 * abigail_uppercut, $BA88, phase 7: the uppercut animation ($A3, bank
 * $30; damage $26 when it starts); untouchable for a frame at a time
 * while the frame is below 5; attack $96 on frames 0-1, $97 after; at
 * frame 9 it ends: attack off, phase 1.
 */
void func_BA88_b1(void)
{
    REGS_IN;

    mff_cycles(0xBA88, 8);
    a = ANIM_UPPERCUT;
    c = a >= RAM(OBJ_ANIM + x);
    if (a != RAM(OBJ_ANIM + x)) {
        mff_cycles(0xBA8F, 8);
        y = CHR_ABIGAIL_UPPER;
        CALL_FIXED(0xBA91, func_F443);
        mff_cycles(0xBA94, 7);
        a = DMG_UPPERCUT;
        RAM(OBJ_ATTACK_DAMAGE + x) = a;
    } else {
        mff_cycles(0xBA8D, 1);
    }
    mff_cycles(0xBA99, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    if (a < 0x05) {
        mff_cycles(0xBAA0, 6);
        a = RAM(OBJ_INVULN + x);
        if (a == 0) {
            mff_cycles(0xBAA5, 7);
            a = 0x01;
            RAM(OBJ_INVULN + x) = a;
        } else {
            mff_cycles(0xBAA3, 1);
        }
    } else {
        mff_cycles(0xBA9E, 1);
    }
    mff_cycles(0xBAAA, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    if (a < 0x02) {
        mff_cycles(0xBAB1, 4);
        a = ATK_UPPERCUT_LOW;
        mff_cycles(0xBAB3, 1);
    } else {
        mff_cycles(0xBAAF, 1);
        mff_cycles(0xBAB5, 2);
        a = ATK_UPPERCUT_HIGH;
    }
    mff_cycles(0xBAB7, 13);
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_ANIM_FRAME + x);
    if (a < 0x09) {
        mff_cycles(0xBAC1, 6);
        RTS_RETURN();
    }
    mff_cycles(0xBABF, 1);
    mff_cycles(0xBAC2, 25);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    RAM(OBJ_ATTACK + x) = a;
    a = ST_APPROACH;
    RAM(OBJ_STATE + x) = a;
    RTS_RETURN();
}

/*
 * abigail_charge_speed, $BAD0 (bank $03; name proposed): obj_vx/obj_vy of
 * the rush for direction index Y (tables $BAE9-$BB19, 2.3 px a frame).
 * Caller: abigail_charge_aim (03:BB53).
 */
void func_BAD0_b1(void)
{
    REGS_IN;

    mff_cycles(0xBAD0, 4);
    a = prg_read_ix(CHARGE_VX_FRAC, y);
    mff_cycles(0xBAD3, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = prg_read_ix(CHARGE_VX, y);
    mff_cycles(0xBAD9, 9);
    RAM(OBJ_VX + x) = a;
    a = prg_read_ix(CHARGE_VY_FRAC, y);
    mff_cycles(0xBADF, 9);
    RAM(OBJ_VY_FRAC + x) = a;
    a = prg_read_ix(CHARGE_VY, y);
    mff_cycles(0xBAE5, 11);
    RAM(OBJ_VY + x) = a;
    RTS_RETURN();
}

/*
 * abigail_charge_aim, $BB29 (bank $03; name proposed): the rush's
 * direction: 0F:FB5B's index toward the player's line, kept at least two
 * steps off vertical ($BB81; straight up or down becomes 2 or 6, mirrored
 * when the player is to the left); obj_dir from $FBF0, the speed
 * ($BAD0), and the slow-down per frame in obj_timer2 (x) / obj_ground_y
 * (y): $12/$07 on the odd (steep) indices, $0E/$0E on the diagonals,
 * $14/$00 straight across.
 * In: X = slot. Out: the RTS, carry as the path left it.
 * Caller: abigail_charge (03:B7F2).
 */
void func_BB29_b1(void)
{
    REGS_IN;
    int steep;

    mff_cycles(0xBB29, 6);
    CALL_FIXED(0xBB29, func_FB5B);              /* obj_dir_index */
    mff_cycles(0xBB2C, 6);
    y = a;
    a &= 0x07;
    steep = 1;
    if (a == 0) {                               /* straight up or down */
        steep = 0;
        mff_cycles(0xBB31, 4);
        a = y;
        if (a == 0) {
            mff_cycles(0xBB34, 6);
            y = (uint8_t)(y + 2);
            mff_cycles(0xBB36, 1);
        } else {
            mff_cycles(0xBB32, 1);
            mff_cycles(0xBB38, 4);
            y = (uint8_t)(y - 2);
        }
        mff_cycles(0xBB3A, 6);
        CALL_FIXED(0xBB3A, func_F374);
        mff_cycles(0xBB3D, 2);
        if (c) {
            mff_cycles(0xBB3D, 1);
        } else {
            mff_cycles(0xBB3F, 12);             /* mirror: -Y & $0F */
            a = y ^ 0xFF;
            ADC_A(0x01);
            a &= 0x0F;
            y = a;
            if (y != 0)
                mff_cycles(0xBB47, 1);
            else
                steep = 1;
        }
    } else {
        mff_cycles(0xBB2F, 1);
    }
    if (steep) {
        mff_cycles(0xBB49, 4);
        a = prg_read_ix(CHARGE_STEEP, y);
        mff_cycles(0xBB4C, 2);
        y = a;
    }
    mff_cycles(0xBB4D, 4);
    a = prg_read_ix(DIR_BY_INDEX, y);
    SET_NZ(a);
    mff_cycles(0xBB50, 11);
    RAM(OBJ_DIR + x) = a;
    CALL_IN(0xBB53, 0xBAD0, BANK_03);
    mff_cycles(0xBB56, 6);
    a = y & 0x01;
    if (a != 0) {
        mff_cycles(0xBB5B, 16);
        a = 0x12;
        RAM(OBJ_TIMER2 + x) = a;
        a = 0x07;
        RAM(OBJ_GROUND_Y + x) = a;
        mff_cycles(0xBB65, 1);
    } else {
        mff_cycles(0xBB59, 1);
        mff_cycles(0xBB67, 8);
        a = (uint8_t)(y >> 2);                  /* TYA, LSR, LSR */
        c = (y >> 1) & 0x01;
        if (c) {
            mff_cycles(0xBB6C, 14);
            a = 0x0E;
            RAM(OBJ_TIMER2 + x) = a;
            RAM(OBJ_GROUND_Y + x) = a;
            mff_cycles(0xBB74, 1);
        } else {
            mff_cycles(0xBB6A, 1);
            mff_cycles(0xBB76, 14);
            a = 0x14;
            RAM(OBJ_TIMER2 + x) = a;
            a = 0x00;
            RAM(OBJ_GROUND_Y + x) = a;
        }
    }
    mff_cycles(0xBB80, 6);
    RTS_RETURN();
}
