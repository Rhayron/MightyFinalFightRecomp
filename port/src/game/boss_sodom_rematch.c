/*
 * boss_sodom_rematch.c - id $83: Sodom again, the boss of round 4 (area
 * 11) and the second area of round 5 (area 13). The stage list's type $16
 * spawns it with Sodom's kind (8), his standing animation ($F0) and more
 * life ($68 against $58); boss_b01.h has the evidence. No P4 route goes
 * far enough into those areas, so the routes never run this file; the
 * coverage sessions did (cyc_seeds.txt, 4k:02:AA33).
 *
 * Unlike the round 2 Sodom it changes with its life: obj_tier is 1 while
 * obj_life is $30 or more, 2 below that, and 3 once enraged (phase 0
 * enrages a tier-2 Sodom: $7C frames unhittable and a $74-frame show).
 * Every frame ($AA33): the tier, obj_kind 8, obj_attack cleared,
 * obj_invuln counted down; the wait while the talk object is in slot $0D;
 * boss_check (01:A6D6); then, while the player lies on the ground, the
 * enraged Sodom pauses between charges and the others back off; then the
 * phase, obj_state & $0F, through $AAC6/$AACF:
 *   0 $AAD8 decide: tier 1 approach, tier 2 enrage, tier 3 charge
 *   1 $AB0A approach: line up with the player and close in
 *   2 $A2ED attack (boss_thrasher_sodom.c), Sodom's swing
 *   3 $AC4E the enraged charges, the pause after them, the show
 *   4 $A473 rush (boss_thrasher_sodom.c)
 *   5 $ABB6 back off
 *   6 $ADBA wait, then rush or approach
 *   7 $AD8C hurt
 *   8 $AE19 the follow-up blow
 */
#include "boss_b01.h"

/* the phase table */
#define SR_PHASE_LO         0xAAC6
#define SR_PHASE_HI         0xAACF

/* the enraged charge: x shake by obj_ai_timer & 3, y shake for the
 * even and odd aim indexes */
#define CHARGE_SHAKE_X      0xAD80  /* $10 $08 $08 $10 */
#define CHARGE_SHAKE_Y_EVEN 0xAD84  /* $10 $08 $08 $10 */
#define CHARGE_SHAKE_Y_ODD  0xAD88  /* $08 $04 $04 $08 */
#define CHARGE_AIM          OBJ_TIMER3      /* the direction index of the charge */

#define REACH_X_SODOM       0x24    /* 0F:FB3E: the swing's reach */
#define REACH_Y_SODOM       0x09

static void lying_check(void);

/*
 * sodom_rematch_update, $AA33 (name proposed): the behaviour of id $83.
 * In: X = slot. Caller: the JMP at 01:A009.
 */
void func_AA33_b0(void)
{
    REGS_IN;

    mff_cycles(0xAA33, 8);
    a = RAM(OBJ_LIFE + x);
    if (a >= SR_LIFE_RAGE) {
        mff_cycles(0xAA38, 1);
        mff_cycles(0xAA45, 2);
        a = SR_TIER_HEALTHY;
        mff_cycles(0xAA47, 5);
        RAM(OBJ_TIER + x) = a;
    } else {
        mff_cycles(0xAA3A, 8);
        a = RAM(OBJ_TIER + x);
        if (a == SR_TIER_HEALTHY) {
            mff_cycles(0xAA41, 4);
            a = SR_TIER_HURT;
            mff_cycles(0xAA43, 1);
            mff_cycles(0xAA47, 5);
            RAM(OBJ_TIER + x) = a;
        } else {
            mff_cycles(0xAA3F, 1);
        }
    }
    mff_cycles(0xAA4A, 20);
    a = KIND_SODOM;
    RAM(OBJ_KIND + x) = a;
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_INVULN + x);
    if (a != 0) {
        mff_cycles(0xAA59, 7);
        RAM(OBJ_INVULN + x) = (uint8_t)(RAM(OBJ_INVULN + x) - 1);
    } else {
        mff_cycles(0xAA57, 1);
    }
    mff_cycles(0xAA5C, 8);
    a = RAM(OBJ_ID + TALK_SLOT);
    CMP_FLAGS(a, OBJ_TALK);
    if (z) {
        mff_cycles(0xAA61, 1);
        mff_cycles(0xAA69, 13);
        a = 0x01;
        SET_NZ(a);
        RAM(OBJ_INVULN + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xAA63, 6);
    CALL_B01(0xAA63, 0xA6D6);
    mff_cycles(0xAA66, 2);
    if (c) {
        mff_cycles(0xAA68, 6);
        RTS_RETURN();
    }
    mff_cycles(0xAA66, 1);
    RUN_ON(lying_check);
}

/*
 * $AA6F: while the player lies on the ground (game_action $1F): the
 * enraged Sodom (tier 3), unless in the middle of a charge (phase 3,
 * obj_timer 1), waits $20 frames (phase 3, obj_timer 2); the others back
 * off (phase 5) unless already waiting (phase 6) or backing off. Both
 * show the stance ($A83A, the JMP at $AAB0) without running a phase.
 * Otherwise the phase runs.
 */
static void lying_check(void)
{
    REGS_IN;

    mff_cycles(0xAA6F, 7);
    a = RAM(GAME_ACTION);
    CMP_FLAGS(a, ACT_LYING);
    if (a != ACT_LYING) {
        mff_cycles(0xAA73, 1);
        SYNC_OUT;
        obj_phase_jump(0xAAB3, SR_PHASE_LO, SR_PHASE_HI);
        return;
    }
    mff_cycles(0xAA75, 8);
    a = RAM(OBJ_TIER + x);
    if (a == SR_TIER_ENRAGED) {
        mff_cycles(0xAA7C, 8);
        a = RAM(OBJ_STATE + x);
        CMP_FLAGS(a, SR_RAGE);
        if (a == SR_RAGE) {
            mff_cycles(0xAA83, 8);
            a = RAM(OBJ_TIMER + x);
            CMP_FLAGS(a, 0x01);
            if (a == 0x01) {
                mff_cycles(0xAA88, 1);
                SYNC_OUT;
                obj_phase_jump(0xAAB3, SR_PHASE_LO, SR_PHASE_HI);
                return;
            }
        } else {
            mff_cycles(0xAA81, 1);
        }
        mff_cycles(0xAA8A, 23);
        a = SR_RAGE;
        RAM(OBJ_STATE + x) = a;
        a = 0x02;
        RAM(OBJ_TIMER + x) = a;
        a = 0x20;
        SET_NZ(a);
        RAM(OBJ_AI_TIMER + x) = a;
        mff_cycles(0xAA99, 1);
    } else {
        mff_cycles(0xAA7A, 1);
        mff_cycles(0xAA9B, 8);
        a = RAM(OBJ_STATE + x);
        CMP_FLAGS(a, SR_WAIT);
        if (z) {
            mff_cycles(0xAAA0, 1);
        } else {
            mff_cycles(0xAAA2, 4);
            CMP_FLAGS(a, SR_BACK_OFF);
            if (a == SR_BACK_OFF) {
                mff_cycles(0xAAA4, 1);
                SYNC_OUT;
                obj_phase_jump(0xAAB3, SR_PHASE_LO, SR_PHASE_HI);
                return;
            }
            mff_cycles(0xAAA6, 14);
            a = SR_BACK_OFF;
            RAM(OBJ_STATE + x) = a;
            a = 0x00;
            SET_NZ(a);
            RAM(OBJ_TIMER + x) = a;
        }
    }
    mff_cycles(0xAAB0, 3);
    JMP_B01(0xA83A);
}

/*
 * Phase 0, $AAD8: tier 1 approaches (phase 1, with the stance); tier 2
 * enrages: obj_invuln $7C, tier 3, and the show of phase 3 (obj_timer 3,
 * $74 frames); tier 3 charges (phase 3, obj_timer 0).
 */
void func_AAD8_b0(void)
{
    REGS_IN;

    mff_cycles(0xAAD8, 8);
    a = RAM(OBJ_TIER + x);
    CMP_FLAGS(a, SR_TIER_ENRAGED);
    if (a == SR_TIER_ENRAGED) {
        mff_cycles(0xAADD, 1);
        mff_cycles(0xAAF4, 2);
        a = 0x00;
    } else {
        mff_cycles(0xAADF, 4);
        CMP_FLAGS(a, SR_TIER_HURT);
        if (a != SR_TIER_HURT) {
            mff_cycles(0xAAE1, 1);
            mff_cycles(0xAAFF, 17);
            a = 0x00;
            RAM(OBJ_TIMER + x) = a;
            t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
            RAM(OBJ_STATE + x) = t;
            SET_NZ(t);
            JMP_B01(0xA83A);
        }
        mff_cycles(0xAAE3, 25);
        a = 0x7C;
        RAM(OBJ_INVULN + x) = a;
        a = 0x74;
        RAM(OBJ_AI_TIMER + x) = a;
        RAM(OBJ_TIER + x) = (uint8_t)(RAM(OBJ_TIER + x) + 1);
        a = 0x03;
        mff_cycles(0xAAF2, 1);
    }
    mff_cycles(0xAAF6, 18);
    RAM(OBJ_TIMER + x) = a;
    a = SR_RAGE;
    SET_NZ(a);
    RAM(OBJ_STATE + x) = a;
    RTS_RETURN();
}

/*
 * Phase 1, $AB0A: a player in reach ($24 x 9) starts Sodom's swing (phase
 * 2, set 1). Otherwise the walk ($A882) and, with obj_timer set, the
 * drift of $AB93. Within $30 x $10 of the player: back off (phase 5) when
 * the player is down or on an even rng, else start drifting (obj_timer
 * 1). Farther: 8 px or more off the player's line, walk up or down
 * (0F:FBD9's carry) at index 0's speed; else walk across toward the
 * player at index 4's.
 */
void func_AB0A_b0(void)
{
    REGS_IN;

    mff_cycles(0xAB0A, 10);
    a = REACH_X_SODOM;
    y = REACH_Y_SODOM;
    SET_NZ(y);
    CALL_FIXED(0xAB0E, func_FB3E);
    mff_cycles(0xAB11, 2);
    if (c) {
        mff_cycles(0xAB13, 27);
        a = SR_ATTACK;
        RAM(OBJ_STATE + x) = a;
        a = 0x00;
        RAM(OBJ_TIMER + x) = a;
        a = 0x01;
        SET_NZ(a);
        RAM(BOSS_ATTACK_SET + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xAB11, 1);
    mff_cycles(0xAB23, 6);
    CALL_B01(0xAB23, 0xA882);
    mff_cycles(0xAB26, 6);
    a = RAM(OBJ_TIMER + x);
    SET_NZ(a);
    if (!z) {
        mff_cycles(0xAB2B, 3);
        JMP_B01(0xAB93);
    }
    mff_cycles(0xAB29, 1);
    mff_cycles(0xAB2E, 10);
    a = 0x30;
    y = 0x10;
    SET_NZ(y);
    CALL_FIXED(0xAB32, func_FB3E);
    mff_cycles(0xAB35, 2);
    if (c) {
        int back_off = 1;
        mff_cycles(0xAB37, 7);
        a = RAM(GAME_ACTION);
        CMP_FLAGS(a, ACT_KNOCKDOWN);
        if (a == ACT_KNOCKDOWN) {
            mff_cycles(0xAB3B, 1);
        } else {
            mff_cycles(0xAB3D, 4);
            CMP_FLAGS(a, ACT_LYING);
            if (a == ACT_LYING) {
                mff_cycles(0xAB3F, 1);
            } else {
                mff_cycles(0xAB41, 7);
                a = RAM(RNG);
                c = a & 1;
                a >>= 1;
                if (c) {
                    mff_cycles(0xAB44, 1);
                    back_off = 0;
                }
            }
        }
        if (back_off) {
            mff_cycles(0xAB46, 20);
            a = 0x00;
            RAM(OBJ_TIMER + x) = a;
            a = SR_BACK_OFF;
            SET_NZ(a);
            RAM(OBJ_STATE + x) = a;
            RTS_RETURN();
        }
        mff_cycles(0xAB51, 20);
        a = 0x00;
        RAM(OBJ_AI_TIMER + x) = a;
        t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        RAM(OBJ_TIMER + x) = t;
        SET_NZ(t);
        RTS_RETURN();
    }
    mff_cycles(0xAB35, 1);

    mff_cycles(0xAB5A, 6);
    CALL_FIXED(0xAB5A, func_FBD9);
    mff_cycles(0xAB5D, 4);
    CMP_FLAGS(a, 0x08);
    if (c) {
        mff_cycles(0xAB61, 8);
        y = DIR_INDEX_UP;
        SET_NZ(y);
        CALL_B01(0xAB63, 0xA7E1);
        mff_cycles(0xAB66, 6);
        CALL_FIXED(0xAB66, func_FBD9);
        mff_cycles(0xAB69, 2);
        if (c) {
            mff_cycles(0xAB69, 1);
            mff_cycles(0xAB6F, 2);
            a = DIR_DOWN;
            SET_NZ(a);
        } else {
            mff_cycles(0xAB6B, 4);
            a = DIR_UP;
            SET_NZ(a);
            mff_cycles(0xAB6D, 1);
        }
        mff_cycles(0xAB71, 11);
        RAM(OBJ_DIR + x) = a;
        CALL_B01(0xAB74, 0xA031);
        mff_cycles(0xAB77, 3);
        JMP_B01(0xA090);
    }
    mff_cycles(0xAB5F, 1);
    mff_cycles(0xAB7A, 8);
    y = DIR_INDEX_RIGHT;
    SET_NZ(y);
    CALL_B01(0xAB7C, 0xA7E1);
    mff_cycles(0xAB7F, 6);
    CALL_FIXED(0xAB7F, func_F374);
    mff_cycles(0xAB82, 2);
    if (c) {
        mff_cycles(0xAB82, 1);
        mff_cycles(0xAB88, 2);
        a = DIR_RIGHT;
        SET_NZ(a);
    } else {
        mff_cycles(0xAB84, 4);
        a = DIR_LEFT;
        SET_NZ(a);
        mff_cycles(0xAB86, 1);
    }
    mff_cycles(0xAB8A, 11);
    RAM(OBJ_DIR + x) = a;
    CALL_B01(0xAB8D, 0xA031);
    mff_cycles(0xAB90, 3);
    JMP_B01(0xA090);
}

/*
 * $AB93: the drift of phase 1: every 8 frames (obj_ai_timer) the aim at a
 * point beside the player (0F:FB72) with the walking speed; it moves and
 * faces the player. Caller: the JMP at 01:AB2B.
 */
void func_AB93_b0(void)
{
    REGS_IN;

    mff_cycles(0xAB93, 6);
    a = RAM(OBJ_AI_TIMER + x);
    if (a != 0) {
        mff_cycles(0xAB98, 10);
        t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
        RAM(OBJ_AI_TIMER + x) = t;
        SET_NZ(t);
        JMP_B01(0xABB0);
    }
    mff_cycles(0xAB96, 1);
    mff_cycles(0xAB9E, 13);
    a = 0x08;
    SET_NZ(a);
    RAM(OBJ_AI_TIMER + x) = a;
    CALL_FIXED(0xABA3, func_FB72);
    mff_cycles(0xABA6, 8);
    y = a;
    SET_NZ(y);
    CALL_B01(0xABA7, 0xA7E1);
    mff_cycles(0xABAA, 4);
    a = prg_read((uint16_t)(DIR_PAD_BITS + y));
    SET_NZ(a);
    mff_cycles(0xABAD, 11);
    RAM(OBJ_DIR + x) = a;
    CALL_B01(0xABB0, 0xA031);
    mff_cycles(0xABB3, 3);
    JMP_B01(0xA090);
}

/* $ABB0: move ($A031) and face the player. Caller: the JMP at 01:AB9B. */
void func_ABB0_b0(void)
{
    REGS_IN;

    mff_cycles(0xABB0, 6);
    CALL_B01(0xABB0, 0xA031);
    mff_cycles(0xABB3, 3);
    JMP_B01(0xA090);
}

static void back_off_step(void);

/*
 * Phase 5, $ABB6: back off. The walk animation; a jumping player in reach
 * ($20 x 9) gets the swing. On the first frame a way away from the player
 * is drawn: direction index 3 (half the time) or 5 at speed 5, or 4 at
 * speed 4 - right-hand indexes, mirrored ((i ^ $0F) + 1) when the player
 * is to the right - for $64 frames (back_off_step).
 */
void func_ABB6_b0(void)
{
    REGS_IN;

    mff_cycles(0xABB6, 6);
    CALL_B01(0xABB6, 0xA882);
    mff_cycles(0xABB9, 7);
    a = RAM(GAME_ACTION);
    CMP_FLAGS(a, ACT_JUMP);
    if (a == ACT_JUMP) {
        mff_cycles(0xABBF, 10);
        a = 0x20;
        y = 0x09;
        SET_NZ(y);
        CALL_FIXED(0xABC3, func_FB3E);
        mff_cycles(0xABC6, 2);
        if (c) {
            mff_cycles(0xABC8, 27);
            a = SR_ATTACK;
            RAM(OBJ_STATE + x) = a;
            a = 0x00;
            RAM(OBJ_TIMER + x) = a;
            a = 0x01;
            SET_NZ(a);
            RAM(BOSS_ATTACK_SET + x) = a;
            RTS_RETURN();
        }
        mff_cycles(0xABC6, 1);
    } else {
        mff_cycles(0xABBD, 1);
    }
    mff_cycles(0xABD8, 6);
    a = RAM(OBJ_TIMER + x);
    if (a != 0) {
        mff_cycles(0xABDB, 2);
        RUN_ON(back_off_step);
    }

    /* the way out: obj_dir holds the direction index for now */
    mff_cycles(0xABDD, 9);
    a = RAM(RNG) & 0x0F;
    CMP_FLAGS(a, 0x08);
    if (!c) {
        mff_cycles(0xABE5, 9);
        a = 0x03;
        RAM(OBJ_DIR + x) = a;
        mff_cycles(0xABEA, 2);
        mff_cycles(0xAC01, 8);
        y = 0x05;
        SET_NZ(y);
        CALL_B01(0xAC03, 0xA7E1);
    } else {
        mff_cycles(0xABE3, 1);
        mff_cycles(0xABEC, 4);
        CMP_FLAGS(a, 0x0D);
        if (!c) {
            mff_cycles(0xABF0, 8);
            y = DIR_INDEX_RIGHT;
            SET_NZ(y);
            CALL_B01(0xABF2, 0xA7E1);
            mff_cycles(0xABF5, 9);
            a = DIR_INDEX_RIGHT;
            SET_NZ(a);
            RAM(OBJ_DIR + x) = a;
            mff_cycles(0xABFA, 2);
        } else {
            mff_cycles(0xABEE, 1);
            mff_cycles(0xABFC, 7);
            a = 0x05;
            RAM(OBJ_DIR + x) = a;
            mff_cycles(0xAC01, 8);
            y = 0x05;
            SET_NZ(y);
            CALL_B01(0xAC03, 0xA7E1);
        }
    }
    mff_cycles(0xAC06, 6);
    CALL_FIXED(0xAC06, func_F374);
    mff_cycles(0xAC09, 2);
    if (c) {
        mff_cycles(0xAC0B, 18);
        a = RAM(OBJ_DIR + x) ^ 0x0F;
        RAM(OBJ_DIR + x) = a;
        RAM(OBJ_DIR + x) = (uint8_t)(RAM(OBJ_DIR + x) + 1);
    } else {
        mff_cycles(0xAC09, 1);
    }
    mff_cycles(0xAC16, 8);
    y = RAM(OBJ_DIR + x);
    a = prg_read((uint16_t)(DIR_PAD_BITS + y));
    mff_cycles(0xAC1C, 25);
    RAM(OBJ_DIR + x) = a;
    a = 0x64;
    RAM(OBJ_AI_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    SET_NZ(t);
    RTS_RETURN();
}

/*
 * $AC28: backing off: moves and faces the player until obj_ai_timer runs
 * out, the move is stopped at the top or bottom line, or x leaves
 * $18-$F0; then phase 6.
 */
static void back_off_step(void)
{
    REGS_IN;

    mff_cycles(0xAC28, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    SET_NZ(t);
    if (!z) {
        mff_cycles(0xAC2D, 6);
        CALL_B01(0xAC2D, 0xA031);
        mff_cycles(0xAC30, 6);
        CALL_B01(0xAC30, 0xA090);
        mff_cycles(0xAC33, 5);
        a = RAM(MOVE_BLOCKED);
        if (a == 0) {
            mff_cycles(0xAC37, 8);
            a = RAM(OBJ_X + x);
            CMP_FLAGS(a, ARENA_RIGHT);
            if (!c) {
                mff_cycles(0xAC3E, 4);
                CMP_FLAGS(a, ARENA_LEFT);
                if (c) {
                    mff_cycles(0xAC42, 6);
                    RTS_RETURN();
                }
                mff_cycles(0xAC40, 1);
            } else {
                mff_cycles(0xAC3C, 1);
            }
        } else {
            mff_cycles(0xAC35, 1);
        }
    } else {
        mff_cycles(0xAC2B, 1);
    }
    mff_cycles(0xAC43, 20);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = SR_WAIT;
    SET_NZ(a);
    RAM(OBJ_STATE + x) = a;
    RTS_RETURN();
}

/* ---- phase 3: the enraged charges ---- */

static void charge_shake_y(void);

/*
 * Phase 3, $AC4E, by obj_timer:
 *  0 aim ($A9CA) into CHARGE_AIM with the charge speed ($A971, 3 px a
 *    frame), the sprite turned that way (0F:F32E), damage $17, animation
 *    $F3 (CHR $2C, chr_banks+5 $2E);
 *  1 charge: obj_attack $90, shaken by CHARGE_SHAKE_X across (the sign
 *    alternating with obj_ai_timer, which steps 0-3) and, unless the aim
 *    is level, CHARGE_SHAKE_Y up or down, while $A031 moves it; at the
 *    arena's side or line ($AD0B) the pause;
 *  2 the pause ($AD50);
 *  3 the show after enraging ($AD5E).
 */
void func_AC4E_b0(void)
{
    REGS_IN;

    mff_cycles(0xAC4E, 6);
    a = RAM(OBJ_TIMER + x);
    SET_NZ(a);
    if (z) {
        mff_cycles(0xAC51, 1);
        mff_cycles(0xAC61, 6);
        CALL_B01(0xAC61, 0xA9CA);
        mff_cycles(0xAC64, 13);
        a = y;
        SET_NZ(a);
        RAM(CHARGE_AIM + x) = a;
        CALL_B01(0xAC68, 0xA971);
        mff_cycles(0xAC6B, 6);
        CALL_FIXED(0xAC6B, func_F32E);
        mff_cycles(0xAC6E, 31);
        a = 0x00;
        RAM(OBJ_AI_TIMER + x) = a;
        RAM(OBJ_TIMER + x) = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        a = 0x17;
        RAM(OBJ_ATTACK_DAMAGE + x) = a;
        a = 0xF3;
        y = 0x2C;
        SET_NZ(y);
        CALL_FIXED(0xAC7F, func_F449);
        mff_cycles(0xAC82, 11);
        a = 0x2E;
        SET_NZ(a);
        RAM(CHR_BANKS + 5) = a;
        RTS_RETURN();
    }
    mff_cycles(0xAC53, 4);
    CMP_FLAGS(a, 0x01);
    if (a != 0x01) {
        mff_cycles(0xAC57, 4);
        CMP_FLAGS(a, 0x02);
        if (z) {
            mff_cycles(0xAC5B, 3);
            JMP_B01(0xAD50);
        }
        mff_cycles(0xAC59, 1);
        mff_cycles(0xAC5E, 3);
        JMP_B01(0xAD5E);
    }
    mff_cycles(0xAC55, 1);

    /* the charge: x shake ($AC87; LDA #$17 there is overwritten) */
    mff_cycles(0xAC87, 25);
    a = 0x90;
    RAM(OBJ_ATTACK + x) = a;
    y = RAM(OBJ_AI_TIMER + x);
    a = RAM(OBJ_DIR + x) ^ RAM(OBJ_AI_TIMER + x);
    c = a & 1;
    a >>= 1;
    if (c) {
        mff_cycles(0xAC9A, 10);
        a = RAM(OBJ_X + x);
        c = 0;
        ADC_A(prg_read((uint16_t)(CHARGE_SHAKE_X + y)));
        mff_cycles(0xACA1, 19);
        RAM(OBJ_X + x) = a;
        a = RAM(OBJ_X_HI + x);
        ADC_A(0x00);
        SET_NZ(a);
        RAM(OBJ_X_HI + x) = a;
        JMP_B01(0xACC1);
    }
    mff_cycles(0xAC98, 1);
    mff_cycles(0xACAF, 10);
    a = RAM(OBJ_X + x);
    c = 1;
    SBC_A(prg_read((uint16_t)(CHARGE_SHAKE_X + y)));
    mff_cycles(0xACB6, 26);                     /* with $ACC1's first 10 */
    RAM(OBJ_X + x) = a;
    a = RAM(OBJ_X_HI + x);
    SBC_A(0x00);
    RAM(OBJ_X_HI + x) = a;
    RUN_ON(charge_shake_y);
}

/*
 * $ACC1: the y shake of the charge. Caller: the JMP at 01:ACAC; $AC4E
 * runs on into it with the first charge paid.
 */
void func_ACC1_b0(void)
{
    mff_cycles(0xACC1, 10);
    charge_shake_y();
}

/* $ACC1 after its first 10 cycles (LDA obj_timer3 / AND #7 / CMP #4 /
 * BEQ): Y = obj_ai_timer from $AC87 */
static void charge_shake_y(void)
{
    REGS_IN;

    a = RAM(CHARGE_AIM + x) & 0x07;
    CMP_FLAGS(a, 0x04);
    if (z) {
        mff_cycles(0xACC8, 2);
        RUN_ON(func_AD0B_b0);
    }
    mff_cycles(0xACCA, 8);
    a = RAM(CHARGE_AIM + x);
    c = a & 1;
    a >>= 1;
    if (!c) {
        mff_cycles(0xACD0, 4);
        a = prg_read((uint16_t)(CHARGE_SHAKE_Y_EVEN + y));
        mff_cycles(0xACD3, 2);
        if (a != 0) {
            mff_cycles(0xACD3, 1);
        } else {
            mff_cycles(0xACD5, 4);
            a = prg_read((uint16_t)(CHARGE_SHAKE_Y_ODD + y));
        }
    } else {
        mff_cycles(0xACCE, 1);
        mff_cycles(0xACD5, 4);
        a = prg_read((uint16_t)(CHARGE_SHAKE_Y_ODD + y));
    }
    mff_cycles(0xACD8, 21);
    RAM(TMP1) = a;
    a = (uint8_t)((RAM(OBJ_DIR + x) >> 3) ^ RAM(OBJ_AI_TIMER + x));
    c = a & 1;
    a >>= 1;
    if (c) {
        mff_cycles(0xACE6, 28);
        a = RAM(OBJ_Y + x);
        c = 1;
        SBC_A(RAM(TMP1));
        RAM(OBJ_Y + x) = a;
        a = RAM(OBJ_Y_HI + x);
        SBC_A(0x00);
        SET_NZ(a);
        RAM(OBJ_Y_HI + x) = a;
        JMP_B01(0xAD0B);
    }
    mff_cycles(0xACE4, 1);
    mff_cycles(0xACFA, 25);
    a = RAM(OBJ_Y + x);
    c = 0;
    ADC_A(RAM(TMP1));
    RAM(OBJ_Y + x) = a;
    a = RAM(OBJ_Y_HI + x);
    ADC_A(0x00);
    RAM(OBJ_Y_HI + x) = a;
    RUN_ON(func_AD0B_b0);
}

/*
 * $AD0B: obj_ai_timer steps (and 3), the move ($A031); the charge ends
 * when the move was stopped at a line, or on the player's screen at x
 * below $18 going left or $F1 and up going right: obj_timer 2, no attack,
 * a pause of $48 frames.
 * Callers: the run-ons from charge_shake_y; the JMP at 01:AD08.
 */
void func_AD0B_b0(void)
{
    REGS_IN;
    int ends;

    mff_cycles(0xAD0B, 24);
    RAM(OBJ_AI_TIMER + x) = (uint8_t)(RAM(OBJ_AI_TIMER + x) + 1);
    a = RAM(OBJ_AI_TIMER + x) & 0x03;
    SET_NZ(a);
    RAM(OBJ_AI_TIMER + x) = a;
    CALL_B01(0xAD16, 0xA031);
    mff_cycles(0xAD19, 5);
    a = RAM(MOVE_BLOCKED);
    if (a != 0) {
        mff_cycles(0xAD1B, 1);
        ends = 1;
    } else {
        mff_cycles(0xAD1D, 10);
        a = RAM(OBJ_X_HI + x);
        CMP_FLAGS(a, RAM(OBJ_X_HI));
        if (!z) {
            mff_cycles(0xAD23, 1);
            ends = 0;
        } else {
            mff_cycles(0xAD25, 8);
            a = RAM(OBJ_DIR + x) & DIR_RIGHT;
            ends = 0;
            if (a == 0) {
                mff_cycles(0xAD2C, 8);
                a = RAM(OBJ_X + x);
                c = a >= ARENA_LEFT;
                if (!c) {
                    mff_cycles(0xAD31, 1);
                    ends = 1;
                }
            } else {
                mff_cycles(0xAD2A, 1);
            }
            if (!ends) {
                mff_cycles(0xAD33, 8);
                a = RAM(OBJ_DIR + x) & DIR_LEFT;
                SET_NZ(a);
                if (!z) {
                    mff_cycles(0xAD38, 1);
                } else {
                    mff_cycles(0xAD3A, 8);
                    a = RAM(OBJ_X + x);
                    CMP_FLAGS(a, ARENA_RIGHT);
                    if (c) {
                        mff_cycles(0xAD3F, 1);
                        ends = 1;
                    }
                }
            }
        }
    }
    if (!ends) {
        mff_cycles(0xAD41, 6);
        RTS_RETURN();
    }
    mff_cycles(0xAD42, 27);
    RAM(OBJ_TIMER + x) = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = 0x48;
    SET_NZ(a);
    RAM(OBJ_AI_TIMER + x) = a;
    RTS_RETURN();
}

/*
 * $AD50: the pause between charges: the stance (the enraged one, $A83A),
 * obj_ai_timer frames, then obj_timer 0 (aim again).
 * Caller: the JMP at 01:AC5B.
 */
void func_AD50_b0(void)
{
    REGS_IN;

    mff_cycles(0xAD50, 6);
    CALL_B01(0xAD50, 0xA83A);
    mff_cycles(0xAD53, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    SET_NZ(t);
    if (z) {
        mff_cycles(0xAD58, 7);
        a = 0x00;
        SET_NZ(a);
        RAM(OBJ_TIMER + x) = a;
    } else {
        mff_cycles(0xAD56, 1);
    }
    mff_cycles(0xAD5D, 6);
    RTS_RETURN();
}

/*
 * $AD5E: the show of enraging ($74 frames from $AAD8): animation $FA (CHR
 * $2E) while obj_ai_timer is $40 or more, then $F1 (CHR $31) facing left;
 * at the end obj_timer 0 (the first charge).
 * Caller: the JMP at 01:AC5E.
 */
void func_AD5E_b0(void)
{
    REGS_IN;

    mff_cycles(0xAD5E, 8);
    a = RAM(OBJ_AI_TIMER + x);
    CMP_FLAGS(a, 0x40);
    if (c) {
        mff_cycles(0xAD65, 6);
        a = 0xFA;
        y = 0x2E;
        SET_NZ(y);
        mff_cycles(0xAD69, 1);
    } else {
        mff_cycles(0xAD63, 1);
        mff_cycles(0xAD6B, 6);
        CALL_FIXED(0xAD6B, func_F099);
        mff_cycles(0xAD6E, 4);
        a = 0xF1;
        y = 0x31;
        SET_NZ(y);
    }
    mff_cycles(0xAD72, 6);
    CALL_FIXED(0xAD72, func_F449);
    mff_cycles(0xAD75, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    SET_NZ(t);
    if (z) {
        mff_cycles(0xAD7A, 7);
        a = 0x00;
        SET_NZ(a);
        RAM(OBJ_TIMER + x) = a;
    } else {
        mff_cycles(0xAD78, 1);
    }
    mff_cycles(0xAD7F, 6);
    RTS_RETURN();
}

/*
 * Phase 7, $AD8C: hurt. Faces the player; when obj_hit_timer runs out:
 * tier 1 rushes back (phase 4), tier 2 decides (phase 0, which enrages
 * it), tier 3 charges (phase 3).
 */
void func_AD8C_b0(void)
{
    REGS_IN;
    int set = 0;

    mff_cycles(0xAD8C, 6);
    CALL_FIXED(0xAD8C, func_F343);
    mff_cycles(0xAD8F, 9);
    t = (uint8_t)(RAM(OBJ_HIT_TIMER + x) - 1);
    RAM(OBJ_HIT_TIMER + x) = t;
    SET_NZ(t);
    if (!z) {
        mff_cycles(0xAD92, 1);
        mff_cycles(0xADB9, 6);
        RTS_RETURN();
    }
    mff_cycles(0xAD94, 8);
    a = RAM(OBJ_TIER + x);
    if (a == SR_TIER_HEALTHY) {
        mff_cycles(0xAD99, 1);
    } else {
        mff_cycles(0xAD9B, 4);
        CMP_FLAGS(a, SR_TIER_HURT);
        if (a == SR_TIER_HURT) {
            mff_cycles(0xAD9D, 1);
            mff_cycles(0xADAB, 4);
            a = SR_DECIDE;
            mff_cycles(0xADAD, 1);
            set = 1;
        } else {
            mff_cycles(0xAD9F, 4);
            CMP_FLAGS(a, SR_TIER_ENRAGED);
            if (a == SR_TIER_ENRAGED) {
                mff_cycles(0xADA1, 1);
                mff_cycles(0xADAF, 2);
                a = SR_RAGE;
                set = 1;
            }
        }
    }
    if (!set) {
        /* $ADA3: tier 1 (0 and 4 on would come here too) */
        mff_cycles(0xADA3, 4);
        CMP_FLAGS(a, SR_TIER_HURT);
        if (c) {
            mff_cycles(0xADA5, 1);
            mff_cycles(0xADAF, 2);
            a = SR_RAGE;
        } else {
            mff_cycles(0xADA7, 4);
            a = SR_DASH;
            mff_cycles(0xADA9, 1);
        }
    }
    mff_cycles(0xADB1, 12);
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    SET_NZ(a);
    RAM(OBJ_TIMER + x) = a;
    mff_cycles(0xADB9, 6);
    RTS_RETURN();
}

/*
 * Phase 6, $ADBA: wait $18 frames in the stance, then rush (phase 4) on
 * rng & $0F below a threshold by the distance across to the player, else
 * approach (phase 1). The ladder meant 0, $0A, $0B, $0C, $0D, $0E for the
 * distances <$28, <$3C, <$50, <$64, <$78 and more, but the BCS at $ADD8
 * jumps over the $3C and $50 rungs: from $28 to $63 the threshold is $0C,
 * and closer than $28 it is 0 - always an approach. ($ADDE-$ADED never
 * execute.)
 */
void func_ADBA_b0(void)
{
    REGS_IN;

    mff_cycles(0xADBA, 6);
    a = RAM(OBJ_TIMER + x);
    if (a == 0) {
        mff_cycles(0xADBF, 17);
        a = 0x18;
        RAM(OBJ_AI_TIMER + x) = a;
        t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        RAM(OBJ_TIMER + x) = t;
        SET_NZ(t);
        JMP_B01(0xA83A);
    }
    mff_cycles(0xADBD, 1);
    mff_cycles(0xADCA, 6);
    a = RAM(OBJ_AI_TIMER + x);
    SET_NZ(a);
    if (!z) {
        mff_cycles(0xADCF, 13);
        t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
        RAM(OBJ_AI_TIMER + x) = t;
        SET_NZ(t);
        RTS_RETURN();
    }
    mff_cycles(0xADCD, 1);
    mff_cycles(0xADD3, 6);
    CALL_FIXED(0xADD3, func_F374);
    mff_cycles(0xADD6, 4);
    CMP_FLAGS(a, 0x28);
    if (!c) {
        mff_cycles(0xADDA, 4);
        a = 0x00;
        mff_cycles(0xADDC, 2);
    } else {
        mff_cycles(0xADD8, 1);
        mff_cycles(0xADEE, 4);
        CMP_FLAGS(a, 0x64);
        if (!c) {
            mff_cycles(0xADF2, 4);
            a = 0x0C;
            mff_cycles(0xADF4, 2);
        } else {
            mff_cycles(0xADF0, 1);
            mff_cycles(0xADF6, 4);
            CMP_FLAGS(a, 0x78);
            if (!c) {
                mff_cycles(0xADFA, 4);
                a = 0x0D;
                mff_cycles(0xADFC, 2);
            } else {
                mff_cycles(0xADF8, 1);
                mff_cycles(0xADFE, 2);
                a = 0x0E;
            }
        }
    }
    mff_cycles(0xAE00, 13);
    RAM(TMP0) = a;
    a = RAM(RNG) & 0x0F;
    CMP_FLAGS(a, RAM(TMP0));
    if (c) {
        mff_cycles(0xAE08, 1);
        mff_cycles(0xAE15, 4);
        a = SR_APPROACH;
        mff_cycles(0xAE0C, 1);
    } else {
        mff_cycles(0xAE0A, 2);
        a = SR_DASH;
    }
    mff_cycles(0xAE0C, 18);
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    SET_NZ(a);
    RAM(OBJ_TIMER + x) = a;
    RTS_RETURN();
}

/*
 * Phase 8, $AE19: the follow-up blow. First frame: the player's
 * invincibility set to $20, damage $27, animation $F6 (CHR $2F, JMP
 * 0F:F44F). Then obj_attack $98 until animation frame 9: phase 1. Only
 * the round 5 path of the swing's recovery (01:A3DB, never executed)
 * sets this phase, yet the coverage sessions ran it (cyc_seeds.txt).
 */
void func_AE19_b0(void)
{
    REGS_IN;

    mff_cycles(0xAE19, 6);
    a = RAM(OBJ_TIMER + x);
    if (a == 0) {
        mff_cycles(0xAE1E, 26);
        a = 0x20;
        RAM(PLAYER_INVULN) = a;
        RAM(OBJ_TIMER + x) = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        a = 0x27;
        RAM(OBJ_ATTACK_DAMAGE + x) = a;
        a = 0xF6;
        y = 0x2F;
        SET_NZ(y);
        JMP_F(0xF44F, func_F44F);
    }
    mff_cycles(0xAE1C, 1);
    mff_cycles(0xAE31, 15);
    a = 0x98;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_ANIM_FRAME + x);
    CMP_FLAGS(a, 0x09);
    if (c) {
        mff_cycles(0xAE3D, 19);
        a = 0x00;
        RAM(OBJ_TIMER + x) = a;
        RAM(OBJ_HIT_PLAYER + x) = a;
        a = SR_APPROACH;
        SET_NZ(a);
        RAM(OBJ_STATE + x) = a;
    } else {
        mff_cycles(0xAE3B, 1);
    }
    mff_cycles(0xAE4A, 6);
    RTS_RETURN();
}
