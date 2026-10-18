/*
 * boss_thrasher_sodom.c - ids $80 and $81: Thrasher, the round 1 boss, and
 * Sodom as the round 2 boss share one behaviour; the kind (7 or 8) picks
 * the animations, the swing and a few decisions. boss_b01.h says how they
 * were identified.
 *
 * Every frame ($A103): obj_kind is set, obj_attack cleared, obj_invuln
 * counted down; while the talk object ($6E) sits in slot $0D the boss
 * waits there, unhittable; then boss_check (01:A6D6: held, hit, the
 * drop-in) and the phase, obj_state & $0F, through $A6C4/$A6CD:
 *   0 $A13C enter: on screen 1 go circling, else walk in (phase 8)
 *   1 $A169 circle: walk the arena's edges, turning at the corners;
 *     attack when the player is in reach; Thrasher may leap or wander
 *     near the middle, at the corners Thrasher may jump, Sodom rush
 *   2 $A2ED attack: one to three swings (Thrasher), one (Sodom)
 *   3 $A417 wander: drift around the player for $78 frames (Thrasher)
 *   4 $A473 rush: aim, run at the player, slide to a stop (Sodom)
 *   5 $A557 jump twice on the spot, the landings hit (Thrasher)
 *   6 $A5CF leap forward until landing against the arena's side (Thrasher)
 *   7 $A633 hurt
 *   8 $A651 walk in
 * Phases 2 and 4 are id $83's too (boss_sodom_rematch.c).
 *
 * At frame ends in the logs (runs/p4/base_guy_alt.flog and four 160,000-
 * frame bot runs) Thrasher is seen in phases 0-3, 5, 6 and 8 (8 twice),
 * Sodom in 0-2 and 4; phase 7 never lasts to a frame end there.
 */
#include "boss_b01.h"

/* the phase table */
#define TS_PHASE_LO         0xA6C4
#define TS_PHASE_HI         0xA6CD

/* the swing, by BOSS_ATTACK_SET (0 Thrasher's, 1 Sodom's): obj_attack
 * near the start, at full reach, and the damage after the first frames */
#define SWING_REACH_NEAR    0xA411  /* $91 $92 */
#define SWING_REACH_FAR     0xA413  /* $8D $8E */
#define SWING_DAMAGE        0xA415  /* $1F $27 */
#define SWING_DAMAGE_EARLY  0x1E    /* Thrasher's but the end of his last swing */
#define SWING_HIT_SOUND     0x18
#define SWING_HIT_PAUSE     0x18    /* BOSS_HIT_PAUSE when a swing hit the player */
#define PLAYER_INVULN_CAP   0x22    /* a swing that hit leaves the player no more than this */

/* player in reach, 0F:FB3E (A px across, Y px deep) */
#define REACH_X_THRASHER    0x1C
#define REACH_Y_THRASHER    0x0A
#define REACH_X_SODOM       0x24
#define REACH_Y_SODOM       0x09
#define REACH_X_AGAIN       0x20    /* another swing, the walk-in, the wander */
#define REACH_Y_AGAIN       0x0A

static void circle_walk(void);
static void set_state(void);
static void walk_in_arrived(void);

/*
 * The frame of ids $80/$81 from $A103 on: `pc`/`cycles` is its first
 * charge, $A103's 23 cycles for Thrasher (LDA #7 / BNE paid before) and 25
 * at $A101 for Sodom (its LDA #8 with them).
 */
static void ts_update(uint16_t pc, int cycles, uint8_t kind)
{
    REGS_IN;

    mff_cycles(pc, cycles);
    a = kind;
    RAM(OBJ_KIND + x) = a;
    a = 0x00;
    RAM(OBJ_TIER + x) = a;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_INVULN + x);
    if (a != 0) {
        mff_cycles(0xA113, 7);
        RAM(OBJ_INVULN + x) = (uint8_t)(RAM(OBJ_INVULN + x) - 1);
    } else {
        mff_cycles(0xA111, 1);
    }
    mff_cycles(0xA116, 8);
    a = RAM(OBJ_ID + TALK_SLOT);
    CMP_FLAGS(a, OBJ_TALK);
    if (z) {
        /* the talk is on: wait */
        mff_cycles(0xA11B, 1);
        mff_cycles(0xA123, 13);
        a = 0x01;
        SET_NZ(a);
        RAM(OBJ_INVULN + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xA11D, 6);
    CALL_B01(0xA11D, 0xA6D6);
    mff_cycles(0xA120, 2);
    if (c) {
        mff_cycles(0xA122, 6);
        RTS_RETURN();
    }
    mff_cycles(0xA120, 1);
    SYNC_OUT;
    obj_phase_jump(0xA129, TS_PHASE_LO, TS_PHASE_HI);
}

/*
 * thrasher_update, $A0FD (name proposed): the behaviour of id $80.
 * In: X = slot. Caller: the JMP at 01:A000.
 */
void func_A0FD_b0(void)
{
    mff_cycles(0xA0FD, 4);
    mff_cycles(0xA0FF, 1);
    ts_update(0xA103, 23, KIND_THRASHER);
}

/*
 * sodom_update, $A101 (name proposed): the behaviour of id $81.
 * In: X = slot. Caller: the JMP at 01:A003.
 */
void func_A101_b0(void)
{
    ts_update(0xA101, 25, KIND_SODOM);
}

/*
 * Phase 0, $A13C: faces the player ($A090). On screen 1 (obj_x_hi 1): to
 * phase 1 with obj_timer 0 and BOSS_TRIED 5. Elsewhere: obj_dir toward
 * screen 1 and phase 8 (walk in). The stage lists of areas 2 and 5 put
 * both bosses on screen 1 (x_hi 1), so phase 8 only follows something
 * that moved them off it (Thrasher, twice in the logs).
 */
void func_A13C_b0(void)
{
    REGS_IN;

    mff_cycles(0xA13C, 6);
    CALL_B01(0xA13C, 0xA090);
    mff_cycles(0xA13F, 8);
    a = RAM(OBJ_X_HI + x);
    CMP_FLAGS(a, 0x01);
    if (a != 0x01) {
        mff_cycles(0xA146, 13);
        a = DIR_RIGHT;
        RAM(OBJ_DIR + x) = a;
        a = RAM(OBJ_X_HI + x);
        if (a != 0) {
            mff_cycles(0xA150, 7);
            a = DIR_LEFT;
            RAM(OBJ_DIR + x) = a;
        } else {
            mff_cycles(0xA14E, 1);
        }
        mff_cycles(0xA155, 13);
        a = TS_WALK_IN;
        SET_NZ(a);
        RAM(OBJ_STATE + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xA144, 1);
    mff_cycles(0xA15B, 27);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x05;
    RAM(BOSS_TRIED + x) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
    SET_NZ(t);
    RTS_RETURN();
}

/*
 * Phase 1, $A169: unless the player is down (game_action $1F or 6), a
 * player in reach (0F:FB3E: $1C x $0A for Thrasher, $24 x 9 for Sodom)
 * starts the attack: BOSS_ATTACK_SET = kind - 7, phase 2. Otherwise the
 * walk ($A882): on the first frame a coin flip (obj_ai_flag) picks the way
 * round and BOSS_DIR_INDEX the side away from the player (right, 4, when
 * the player is to the left); then circle_walk.
 */
void func_A169_b0(void)
{
    REGS_IN;

    mff_cycles(0xA169, 7);
    a = RAM(GAME_ACTION);
    CMP_FLAGS(a, ACT_LYING);
    if (!z) {
        mff_cycles(0xA16F, 4);
        CMP_FLAGS(a, ACT_KNOCKDOWN);
        if (!z) {
            mff_cycles(0xA173, 8);
            a = RAM(OBJ_KIND + x);
            CMP_FLAGS(a, KIND_THRASHER);
            if (a == KIND_THRASHER) {
                mff_cycles(0xA17A, 6);
                a = REACH_X_THRASHER;
                y = REACH_Y_THRASHER;
                SET_NZ(y);
                mff_cycles(0xA17E, 1);
            } else {
                mff_cycles(0xA178, 1);
                mff_cycles(0xA180, 4);
                a = REACH_X_SODOM;
                y = REACH_Y_SODOM;
                SET_NZ(y);
            }
            mff_cycles(0xA184, 6);
            CALL_FIXED(0xA184, func_FB3E);
            mff_cycles(0xA187, 2);
            if (c) {
                mff_cycles(0xA189, 31);
                a = RAM(OBJ_KIND + x);
                SBC_A(KIND_THRASHER);           /* C is set */
                RAM(BOSS_ATTACK_SET + x) = a;
                a = 0x00;
                RAM(OBJ_TIMER + x) = a;
                t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
                RAM(OBJ_STATE + x) = t;
                SET_NZ(t);
                RTS_RETURN();
            }
            mff_cycles(0xA187, 1);
        } else {
            mff_cycles(0xA171, 1);
        }
    } else {
        mff_cycles(0xA16D, 1);
    }

    mff_cycles(0xA19A, 6);
    CALL_B01(0xA19A, 0xA882);
    mff_cycles(0xA19D, 6);
    a = RAM(OBJ_TIMER + x);
    if (a != 0) {
        mff_cycles(0xA1A0, 1);
        RUN_ON(circle_walk);
    }
    mff_cycles(0xA1A2, 23);
    a = RAM(RNG) & 0x01;
    RAM(OBJ_AI_FLAG + x) = a;
    a = DIR_INDEX_RIGHT;
    SET_NZ(a);
    RAM(BOSS_DIR_INDEX + x) = a;
    CALL_FIXED(0xA1AE, func_F374);
    mff_cycles(0xA1B1, 2);
    if (c) {
        mff_cycles(0xA1B3, 7);
        a = DIR_INDEX_LEFT;
        RAM(BOSS_DIR_INDEX + x) = a;
    } else {
        mff_cycles(0xA1B1, 1);
    }
    mff_cycles(0xA1B8, 13);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    SET_NZ(t);
    RTS_RETURN();
}

static void maybe_leap(void);
static void maybe_wander(void);

/*
 * $A1BC: a step of the circling walk. (Off screen 1 the boss would be put
 * back on its edge, x $01 or $FE: never executed.) At the edge it walks
 * toward - right past x $F1, left below $18, up above y $86, down from
 * $AE - BOSS_DIR_INDEX turns a quarter, +4 with obj_ai_flag 0 or -4, and
 * $A28B may start a jump or a rush. Otherwise it moves at walking speed
 * ($A7E1, $A031), faces the player, and Thrasher may leap when on the
 * player's line in a vertical walk ($A26F) or wander when crossing the
 * middle (y $90-$97 or x $7C-$83, $A2CB).
 */
static void circle_walk(void)
{
    REGS_IN;
    int at_edge;

    mff_cycles(0xA1BC, 8);
    a = RAM(OBJ_X_HI + x);
    if (a != 0x01) {
        mff_cycles(0xA1C3, 4);
        if (a == 0x02) {
            mff_cycles(0xA1C5, 1);
            mff_cycles(0xA1CB, 2);
            a = 0xFE;
        } else {
            mff_cycles(0xA1C7, 4);
            a = 0x01;
            mff_cycles(0xA1C9, 1);
        }
        mff_cycles(0xA1CD, 12);
        RAM(OBJ_X + x) = a;
        a = 0x01;
        RAM(OBJ_X_HI + x) = a;
    } else {
        mff_cycles(0xA1C1, 1);
    }

    mff_cycles(0xA1D5, 8);
    a = RAM(BOSS_DIR_INDEX + x);
    CMP_FLAGS(a, DIR_INDEX_RIGHT);
    if (a == DIR_INDEX_RIGHT) {
        mff_cycles(0xA1DC, 8);
        a = RAM(OBJ_X + x);
        CMP_FLAGS(a, ARENA_RIGHT);
        at_edge = c;
        if (at_edge) {
            mff_cycles(0xA1E1, 2);
        } else {
            mff_cycles(0xA1E3, 2);
            mff_cycles(0xA1E3, 2);
        }
    } else {
        mff_cycles(0xA1DA, 1);
        mff_cycles(0xA1E5, 4);
        CMP_FLAGS(a, DIR_INDEX_LEFT);
        if (a == DIR_INDEX_LEFT) {
            mff_cycles(0xA1E9, 8);
            a = RAM(OBJ_X + x);
            CMP_FLAGS(a, ARENA_LEFT);
            at_edge = !c;
            if (at_edge) {
                mff_cycles(0xA1EE, 2);
            } else {
                mff_cycles(0xA1F0, 2);
                mff_cycles(0xA1F0, 2);
            }
        } else {
            mff_cycles(0xA1E7, 1);
            mff_cycles(0xA1F2, 4);
            CMP_FLAGS(a, DIR_INDEX_UP);
            if (a == DIR_INDEX_UP) {
                mff_cycles(0xA1F6, 8);
                a = RAM(OBJ_Y + x);
                CMP_FLAGS(a, 0x86);
                at_edge = !c;
                if (at_edge) {
                    mff_cycles(0xA1FB, 2);
                } else {
                    mff_cycles(0xA1FD, 2);
                    mff_cycles(0xA1FD, 2);
                }
            } else {
                mff_cycles(0xA1F4, 1);
                mff_cycles(0xA1FF, 4);
                CMP_FLAGS(a, DIR_INDEX_DOWN);
                if (a == DIR_INDEX_DOWN) {
                    mff_cycles(0xA203, 8);
                    a = RAM(OBJ_Y + x);
                    CMP_FLAGS(a, ARENA_BOTTOM);
                    at_edge = c;
                    if (at_edge) {
                        mff_cycles(0xA208, 1);
                    } else {
                        mff_cycles(0xA20A, 2);
                        mff_cycles(0xA20A, 1);
                    }
                } else {
                    mff_cycles(0xA201, 1);
                    at_edge = 0;
                }
            }
        }
    }

    if (at_edge) {
        mff_cycles(0xA20C, 6);
        a = RAM(OBJ_AI_FLAG + x);
        if (a == 0) {
            mff_cycles(0xA211, 18);
            a = RAM(BOSS_DIR_INDEX + x);
            c = 0;
            ADC_A(0x04);
            a &= 0x0F;
            SET_NZ(a);
            RAM(BOSS_DIR_INDEX + x) = a;
            JMP_B01(0xA22A);
        }
        mff_cycles(0xA20F, 1);
        mff_cycles(0xA21F, 23);
        a = RAM(BOSS_DIR_INDEX + x);
        c = 1;
        SBC_A(0x04);
        a &= 0x0F;
        RAM(BOSS_DIR_INDEX + x) = a;
        y = a;
        SET_NZ(y);
        CALL_B01(0xA22B, 0xA7E1);
        mff_cycles(0xA22E, 3);
        JMP_B01(0xA28B);
    }

    mff_cycles(0xA231, 10);
    y = RAM(BOSS_DIR_INDEX + x);
    SET_NZ(y);
    CALL_B01(0xA234, 0xA7E1);
    mff_cycles(0xA237, 4);
    a = prg_read_ix(DIR_PAD_BITS, y);
    SET_NZ(a);
    mff_cycles(0xA23A, 11);
    RAM(OBJ_DIR + x) = a;
    CALL_B01(0xA23D, 0xA031);
    mff_cycles(0xA240, 6);
    CALL_B01(0xA240, 0xA090);
    mff_cycles(0xA243, 8);
    a = RAM(OBJ_KIND + x);
    CMP_FLAGS(a, KIND_THRASHER);
    if (!z) {
        mff_cycles(0xA248, 1);
        mff_cycles(0xA26E, 6);
        RTS_RETURN();
    }
    mff_cycles(0xA24A, 8);
    a = RAM(OBJ_DIR + x) & (DIR_UP | DIR_DOWN);
    SET_NZ(a);
    if (!z) {
        mff_cycles(0xA251, 6);
        CALL_FIXED(0xA251, func_FBD9);
        mff_cycles(0xA254, 4);
        CMP_FLAGS(a, 0x02);
        if (!c) {
            mff_cycles(0xA256, 1);
            RUN_ON(maybe_leap);
        }
    } else {
        mff_cycles(0xA24F, 1);
    }
    mff_cycles(0xA258, 8);
    a = RAM(OBJ_Y + x);
    CMP_FLAGS(a, 0x90);
    if (c) {
        mff_cycles(0xA25F, 4);
        CMP_FLAGS(a, 0x98);
        if (!c) {
            mff_cycles(0xA261, 1);
            RUN_ON(maybe_wander);
        }
    } else {
        mff_cycles(0xA25D, 1);
    }
    mff_cycles(0xA263, 8);
    a = RAM(OBJ_X + x);
    CMP_FLAGS(a, 0x7C);
    if (c) {
        mff_cycles(0xA26A, 4);
        CMP_FLAGS(a, 0x84);
        if (!c) {
            mff_cycles(0xA26C, 1);
            RUN_ON(maybe_wander);
        }
    } else {
        mff_cycles(0xA268, 1);
    }
    mff_cycles(0xA26E, 6);
    RTS_RETURN();
}

/*
 * $A26F (Thrasher): unless BOSS_TRIED bit 1 is set, BOSS_TRIED = 3 and
 * with 12 chances in 16 a leap at the player: faces it (0F:F343), phase 6.
 */
static void maybe_leap(void)
{
    REGS_IN;

    mff_cycles(0xA26F, 8);
    a = RAM(BOSS_TRIED + x) & 0x02;
    SET_NZ(a);
    if (!z) {
        mff_cycles(0xA274, 1);
        mff_cycles(0xA28A, 6);
        RTS_RETURN();
    }
    mff_cycles(0xA276, 16);
    a = 0x03;
    RAM(BOSS_TRIED + x) = a;
    a = RAM(RNG) & 0x0F;
    CMP_FLAGS(a, 0x0C);
    if (c) {
        mff_cycles(0xA281, 1);
        mff_cycles(0xA28A, 6);
        RTS_RETURN();
    }
    mff_cycles(0xA283, 6);
    CALL_FIXED(0xA283, func_F343);
    mff_cycles(0xA286, 4);
    a = TS_LEAP;
    SET_NZ(a);
    mff_cycles(0xA288, 1);
    RUN_ON(set_state);
}

/*
 * $A2CB (Thrasher): unless BOSS_TRIED bit 2 is set, BOSS_TRIED = 5 and
 * with 10 chances in 16 phase 3 (wander).
 */
static void maybe_wander(void)
{
    REGS_IN;

    mff_cycles(0xA2CB, 8);
    a = RAM(BOSS_TRIED + x) & 0x04;
    SET_NZ(a);
    if (!z) {
        mff_cycles(0xA2D0, 1);
        mff_cycles(0xA2E3, 6);
        RTS_RETURN();
    }
    mff_cycles(0xA2D2, 16);
    a = 0x05;
    RAM(BOSS_TRIED + x) = a;
    a = RAM(RNG) & 0x0F;
    CMP_FLAGS(a, 0x0A);
    if (c) {
        mff_cycles(0xA2DD, 1);
        mff_cycles(0xA2E3, 6);
        RTS_RETURN();
    }
    mff_cycles(0xA2DF, 4);
    a = TS_WANDER;
    SET_NZ(a);
    mff_cycles(0xA2E1, 1);
    RUN_ON(set_state);
}

/* $A2E4: obj_state = A, obj_timer = 0 */
static void set_state(void)
{
    REGS_IN;

    mff_cycles(0xA2E4, 18);
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    SET_NZ(a);
    RAM(OBJ_TIMER + x) = a;
    RTS_RETURN();
}

/*
 * $A22A: the turn with obj_ai_flag 0 - the speed for the new direction
 * index (in A), then $A28B. Caller: the JMP at 01:A21C.
 */
void func_A22A_b0(void)
{
    REGS_IN;

    mff_cycles(0xA22A, 8);
    y = a;
    SET_NZ(y);
    CALL_B01(0xA22B, 0xA7E1);
    mff_cycles(0xA22E, 3);
    JMP_B01(0xA28B);
}

/*
 * $A28B: a corner of the circling walk. With BOSS_TRIED bit 0 set it is
 * only cleared. Otherwise BOSS_TRIED = 0 and with 8 chances in 16 Thrasher
 * jumps (phase 5) and Sodom rushes (phase 4, not while the player is
 * knocked down).
 * Callers: the JMPs at 01:A22E (both turns).
 */
void func_A28B_b0(void)
{
    REGS_IN;

    mff_cycles(0xA28B, 8);
    a = RAM(BOSS_TRIED + x);
    c = a & 1;
    a >>= 1;
    if (c) {
        mff_cycles(0xA28F, 1);
        mff_cycles(0xA2AA, 17);
        a = RAM(BOSS_TRIED + x) & 0x06;
        SET_NZ(a);
        RAM(BOSS_TRIED + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xA291, 8);
    a = RAM(OBJ_KIND + x);
    if (a != KIND_SODOM) {
        mff_cycles(0xA298, 16);
        a = 0x00;
        RAM(BOSS_TRIED + x) = a;
        a = RAM(RNG) & 0x0F;
        CMP_FLAGS(a, 0x08);
        if (c) {
            mff_cycles(0xA2A3, 1);
            mff_cycles(0xA2A9, 6);
            RTS_RETURN();
        }
        mff_cycles(0xA2A5, 4);
        a = TS_JUMP;
        SET_NZ(a);
        mff_cycles(0xA2A7, 1);
        RUN_ON(set_state);
    }
    mff_cycles(0xA296, 1);
    mff_cycles(0xA2B3, 16);
    a = 0x00;
    RAM(BOSS_TRIED + x) = a;
    a = RAM(RNG) & 0x0F;
    CMP_FLAGS(a, 0x08);
    if (!c) {
        mff_cycles(0xA2C0, 7);
        a = RAM(GAME_ACTION);
        CMP_FLAGS(a, ACT_KNOCKDOWN);
        if (!z) {
            mff_cycles(0xA2C6, 4);
            a = TS_DASH;
            SET_NZ(a);
            mff_cycles(0xA2C8, 1);
            RUN_ON(set_state);
        }
        mff_cycles(0xA2C4, 1);
    } else {
        mff_cycles(0xA2BE, 1);
    }
    mff_cycles(0xA2CA, 6);
    RTS_RETURN();
}

/* ---- phase 2: the attack (ids $80, $81 and $83) ---- */

static void swing_reach_far(void);
static void swing_reach_near(void);
static void swing_damage(void);
static void swing_recover(void);

/*
 * Phase 2, $A2ED: the attack, BOSS_ATTACK_SET choosing the swing. First
 * frame: BOSS_HIT_PAUSE cleared, faces the player, obj_ai_count swings (3
 * for Thrasher's set, 1 for Sodom's), a wind-up of obj_ai_timer frames (2
 * or 0, $0A more against a jumping player), the stance ($A83A). Then the
 * wind-up holds the first frame; then the swing animation ($A8B7), whose
 * frames put the reach out (Thrasher: frames 0-1 near, 5-6 far; Sodom:
 * 0-3 near, 4-8 far) or count as the recovery; at its end $A3EA.
 */
void func_A2ED_b0(void)
{
    REGS_IN;

    mff_cycles(0xA2ED, 6);
    a = RAM(OBJ_TIMER + x);
    SET_NZ(a);
    if (z) {
        mff_cycles(0xA2F2, 11);
        RAM(BOSS_HIT_PAUSE + x) = a;
        CALL_FIXED(0xA2F5, func_F343);
        mff_cycles(0xA2F8, 6);
        a = RAM(BOSS_ATTACK_SET + x);
        if (a == 0) {
            mff_cycles(0xA2FD, 6);
            a = 0x03;
            y = 0x02;
            mff_cycles(0xA301, 1);
        } else {
            mff_cycles(0xA2FB, 2);
            mff_cycles(0xA303, 4);
            a = 0x01;
            y = 0x00;
        }
        mff_cycles(0xA307, 12);
        RAM(OBJ_AI_COUNT + x) = a;
        a = RAM(GAME_ACTION);
        CMP_FLAGS(a, ACT_JUMP);
        if (a == ACT_JUMP) {
            mff_cycles(0xA310, 8);
            a = y;
            c = 0;
            ADC_A(0x0A);
            y = a;
        } else {
            mff_cycles(0xA30E, 1);
        }
        mff_cycles(0xA315, 17);
        a = y;
        RAM(OBJ_AI_TIMER + x) = a;
        t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        RAM(OBJ_TIMER + x) = t;
        SET_NZ(t);
        JMP_B01(0xA83A);
    }
    mff_cycles(0xA2F0, 2);

    mff_cycles(0xA31F, 6);
    a = RAM(OBJ_AI_TIMER + x);
    SET_NZ(a);
    if (!z) {
        mff_cycles(0xA324, 25);
        RAM(OBJ_AI_TIMER + x) = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
        a = 0x00;
        SET_NZ(a);
        RAM(OBJ_ANIM_TIMER + x) = a;
        RAM(OBJ_ANIM_FRAME + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xA322, 1);
    mff_cycles(0xA330, 6);
    CALL_B01(0xA330, 0xA8B7);
    mff_cycles(0xA333, 6);
    a = RAM(BOSS_ATTACK_SET + x);
    if (a != 0) {
        /* Sodom's swing: 9 frames */
        mff_cycles(0xA338, 10);
        a = RAM(OBJ_ANIM_TIMER + x) | RAM(OBJ_ANIM_FRAME + x);
        SET_NZ(a);
        if (z) {
            mff_cycles(0xA340, 3);
            JMP_B01(0xA3EA);
        }
        mff_cycles(0xA33E, 1);
        mff_cycles(0xA343, 8);
        a = RAM(OBJ_ANIM_FRAME + x);
        CMP_FLAGS(a, 0x04);
        if (!c) {
            mff_cycles(0xA348, 1);
            RUN_ON(swing_reach_near);
        }
        mff_cycles(0xA34A, 4);
        CMP_FLAGS(a, 0x09);
        if (!c) {
            mff_cycles(0xA34C, 1);
            RUN_ON(swing_reach_far);
        }
        mff_cycles(0xA34E, 2);
        mff_cycles(0xA34E, 1);
        RUN_ON(swing_recover);
    }
    /* Thrasher's swing: ends on frame 9 */
    mff_cycles(0xA336, 1);
    mff_cycles(0xA350, 6);
    a = RAM(OBJ_ANIM_TIMER + x);
    if (a == 0) {
        mff_cycles(0xA355, 8);
        a = RAM(OBJ_ANIM_FRAME + x);
        CMP_FLAGS(a, 0x09);
        if (z) {
            mff_cycles(0xA35C, 3);
            JMP_B01(0xA3EA);
        }
        mff_cycles(0xA35A, 1);
    } else {
        mff_cycles(0xA353, 1);
    }
    mff_cycles(0xA35F, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    CMP_FLAGS(a, 0x02);
    if (!c) {
        mff_cycles(0xA364, 1);
        RUN_ON(swing_reach_near);
    }
    mff_cycles(0xA366, 4);
    CMP_FLAGS(a, 0x05);
    if (!c) {
        mff_cycles(0xA368, 1);
        RUN_ON(swing_recover);
    }
    mff_cycles(0xA36A, 4);
    CMP_FLAGS(a, 0x07);
    if (c) {
        mff_cycles(0xA36C, 1);
        RUN_ON(swing_recover);
    }
    RUN_ON(swing_reach_far);
}

/* $A36E: obj_attack = SWING_REACH_FAR[set] ($A379 if it were 0) */
static void swing_reach_far(void)
{
    REGS_IN;

    mff_cycles(0xA36E, 8);
    y = RAM(BOSS_ATTACK_SET + x);
    a = prg_read_ix(SWING_REACH_FAR, y);
    mff_cycles(0xA374, 7);
    RAM(OBJ_ATTACK + x) = a;
    if (a == 0)
        RUN_ON(swing_reach_near);
    mff_cycles(0xA377, 1);
    RUN_ON(swing_damage);
}

/* $A379: obj_attack = SWING_REACH_NEAR[set] */
static void swing_reach_near(void)
{
    REGS_IN;

    mff_cycles(0xA379, 8);
    y = RAM(BOSS_ATTACK_SET + x);
    a = prg_read_ix(SWING_REACH_NEAR, y);
    mff_cycles(0xA37F, 5);
    RAM(OBJ_ATTACK + x) = a;
    RUN_ON(swing_damage);
}

/*
 * $A382: the damage: $1E for Thrasher's swings but the late frames (5 on)
 * of his last one; else SWING_DAMAGE[set] - in round 5 against a player
 * on the ground it would be 5 (never executed). If the swing hit the
 * player (obj_hit_player, 0C:8204): sound $18, BOSS_HIT_PAUSE $18, the
 * flag cleared.
 */
static void swing_damage(void)
{
    REGS_IN;
    int early = 0;

    mff_cycles(0xA382, 6);
    a = RAM(BOSS_ATTACK_SET + x);
    if (a == 0) {
        mff_cycles(0xA387, 8);
        a = RAM(OBJ_AI_COUNT + x);
        CMP_FLAGS(a, 0x01);
        if (a == 0x01) {
            mff_cycles(0xA38E, 8);
            a = RAM(OBJ_ANIM_FRAME + x);
            CMP_FLAGS(a, 0x05);
            if (c)
                mff_cycles(0xA393, 1);
            else
                early = 1;
        } else {
            mff_cycles(0xA38C, 1);
            early = 1;
        }
    } else {
        mff_cycles(0xA385, 1);
    }
    if (early) {
        mff_cycles(0xA395, 4);
        a = SWING_DAMAGE_EARLY;
        mff_cycles(0xA397, 1);
    } else {
        int round5 = 0;
        mff_cycles(0xA399, 7);
        a = RAM(GAME_ACTION);
        CMP_FLAGS(a, ACT_JUMP);
        if (a != ACT_JUMP) {
            mff_cycles(0xA39F, 7);
            a = RAM(ROUND);
            CMP_FLAGS(a, 0x04);
            if (a == 0x04)
                round5 = 1;
            else
                mff_cycles(0xA3A3, 1);
        } else {
            mff_cycles(0xA39D, 1);
        }
        if (round5) {
            mff_cycles(0xA3A5, 4);
            a = 0x05;
            mff_cycles(0xA3A7, 1);
        } else {
            mff_cycles(0xA3A9, 8);
            y = RAM(BOSS_ATTACK_SET + x);
            a = prg_read_ix(SWING_DAMAGE, y);
        }
    }
    mff_cycles(0xA3AF, 11);
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = RAM(OBJ_HIT_PLAYER + x);
    SET_NZ(a);
    if (!z) {
        mff_cycles(0xA3B7, 13);
        a = SWING_HIT_PAUSE;
        SET_NZ(a);
        RAM(BOSS_HIT_PAUSE + x) = a;
        CALL_FIXED(0xA3BC, func_F34C);          /* sound $18 */
        mff_cycles(0xA3BF, 7);
        a = 0x00;
        SET_NZ(a);
        RAM(OBJ_HIT_PLAYER + x) = a;
    } else {
        mff_cycles(0xA3B5, 1);
    }
    mff_cycles(0xA3C4, 6);
    RTS_RETURN();
}

/*
 * $A3C5: the recovery frames. After a swing that hit (BOSS_HIT_PAUSE) the
 * player's invincibility is cut to $22 frames, so the next swing can hit
 * again; in round 5 the boss would go on to phase 8 instead, unless the
 * player is knocked down (never executed).
 */
static void swing_recover(void)
{
    REGS_IN;

    mff_cycles(0xA3C5, 6);
    a = RAM(BOSS_HIT_PAUSE + x);
    SET_NZ(a);
    if (z) {
        mff_cycles(0xA3C8, 1);
        mff_cycles(0xA3E9, 6);
        RTS_RETURN();
    }
    mff_cycles(0xA3CA, 7);
    a = RAM(ROUND);
    CMP_FLAGS(a, 0x04);
    if (a == 0x04) {
        mff_cycles(0xA3D0, 7);
        a = RAM(GAME_ACTION);
        CMP_FLAGS(a, ACT_KNOCKDOWN);
        if (z) {
            mff_cycles(0xA3D4, 1);
            mff_cycles(0xA3E9, 6);
            RTS_RETURN();
        }
        mff_cycles(0xA3D6, 20);
        a = 0x00;
        RAM(OBJ_TIMER + x) = a;
        a = SR_FOLLOW_UP;
        SET_NZ(a);
        RAM(OBJ_STATE + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xA3CE, 1);
    mff_cycles(0xA3E1, 7);
    a = PLAYER_INVULN_CAP;
    CMP_FLAGS(a, RAM(PLAYER_INVULN));
    if (!c) {
        mff_cycles(0xA3E7, 3);
        RAM(PLAYER_INVULN) = a;
    } else {
        mff_cycles(0xA3E5, 1);
    }
    mff_cycles(0xA3E9, 6);
    RTS_RETURN();
}

/*
 * $A3EA: the end of a swing: faces the player; with swings left and the
 * player still in reach ($20 x $0A) a pause of 3 frames and the next one;
 * else phase 0 with BOSS_TRIED 7.
 * Callers: the JMPs at 01:A340, A35C.
 */
void func_A3EA_b0(void)
{
    REGS_IN;

    mff_cycles(0xA3EA, 6);
    CALL_FIXED(0xA3EA, func_F343);
    mff_cycles(0xA3ED, 9);
    t = (uint8_t)(RAM(OBJ_AI_COUNT + x) - 1);
    RAM(OBJ_AI_COUNT + x) = t;
    if (t != 0) {
        mff_cycles(0xA3F2, 10);
        a = REACH_X_AGAIN;
        y = REACH_Y_AGAIN;
        SET_NZ(y);
        CALL_FIXED(0xA3F6, func_FB3E);
        mff_cycles(0xA3F9, 2);
        if (c) {
            mff_cycles(0xA3FB, 13);
            a = 0x03;
            SET_NZ(a);
            RAM(OBJ_AI_TIMER + x) = a;
            RTS_RETURN();
        }
        mff_cycles(0xA3F9, 2);
    } else {
        mff_cycles(0xA3F0, 2);
    }
    mff_cycles(0xA401, 27);
    a = TS_ENTER;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x07;
    SET_NZ(a);
    RAM(BOSS_TRIED + x) = a;
    RTS_RETURN();
}

/*
 * Phase 3, $A417 (Thrasher): obj_ai_timer $78 frames of drifting. A
 * player in reach ($20 x $0A) starts Thrasher's swing (phase 2, set 0);
 * every 8 frames the aim is taken again (0F:FB72: a point beside the
 * player) with the walking speed; it moves and faces the player. At the
 * end: phase 1, BOSS_TRIED 7.
 */
void func_A417_b0(void)
{
    REGS_IN;

    mff_cycles(0xA417, 6);
    a = RAM(OBJ_TIMER + x);
    if (a == 0) {
        mff_cycles(0xA41C, 27);
        a = 0x78;
        RAM(OBJ_AI_TIMER + x) = a;
        a = 0x01;
        RAM(OBJ_AI_COUNT + x) = a;
        t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        RAM(OBJ_TIMER + x) = t;
        SET_NZ(t);
        RTS_RETURN();
    }
    mff_cycles(0xA41A, 1);
    mff_cycles(0xA42A, 10);
    a = REACH_X_AGAIN;
    y = REACH_Y_AGAIN;
    SET_NZ(y);
    CALL_FIXED(0xA42E, func_FB3E);
    mff_cycles(0xA431, 2);
    if (c) {
        mff_cycles(0xA433, 25);
        a = TS_ATTACK;
        RAM(OBJ_STATE + x) = a;
        a = 0x00;
        SET_NZ(a);
        RAM(OBJ_TIMER + x) = a;
        RAM(BOSS_ATTACK_SET + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xA431, 1);
    mff_cycles(0xA441, 9);
    t = (uint8_t)(RAM(OBJ_AI_COUNT + x) - 1);
    RAM(OBJ_AI_COUNT + x) = t;
    if (t == 0) {
        mff_cycles(0xA446, 13);
        a = 0x08;
        SET_NZ(a);
        RAM(OBJ_AI_COUNT + x) = a;
        CALL_FIXED(0xA44B, func_FB72);
        mff_cycles(0xA44E, 8);
        y = a;
        SET_NZ(y);
        CALL_B01(0xA44F, 0xA7E1);
        mff_cycles(0xA452, 4);
        a = prg_read_ix(DIR_PAD_BITS, y);
        mff_cycles(0xA455, 5);
        RAM(OBJ_DIR + x) = a;
    } else {
        mff_cycles(0xA444, 1);
    }
    mff_cycles(0xA458, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    SET_NZ(t);
    if (!z) {
        mff_cycles(0xA45D, 6);
        CALL_B01(0xA45D, 0xA031);
        mff_cycles(0xA460, 3);
        JMP_B01(0xA090);
    }
    mff_cycles(0xA45B, 1);
    mff_cycles(0xA463, 27);
    a = TS_CIRCLE;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x07;
    SET_NZ(a);
    RAM(BOSS_TRIED + x) = a;
    RTS_RETURN();
}

/* ---- phase 4: the rush (ids $81 and $83) ---- */

/*
 * Phase 4, $A473, by obj_timer:
 *  0 aim ($A9CA, at most diagonal) with the rush speed and its slowing
 *    ($A9F5), animation $F4 (CHR $2C, chr_banks+5 $2E);
 *  1 run ($A031) until within $30 px in x of the player, then the
 *    strike, animation $F6 (CHR $2F) with damage $17 ($2E at a nonzero
 *    obj_tier: never executed); at the arena's side it would stop
 *    ($A54C: never executed);
 *  2 slide: obj_vx_frac loses obj_timer2 and obj_vy_frac obj_ground_y
 *    each frame (set by $A9F5), obj_attack $8B while there is speed across;
 *    when either component goes negative, a pause of $0C frames (6 at a
 *    nonzero obj_tier: never executed);
 *  3 the pause ($A53B), then phase 0.
 */
void func_A473_b0(void)
{
    REGS_IN;

    mff_cycles(0xA473, 8);
    a = RAM(OBJ_TIMER + x);
    CMP_FLAGS(a, 0x01);
    if (z) {
        mff_cycles(0xA478, 1);
        mff_cycles(0xA49D, 6);
        CALL_B01(0xA49D, 0xA031);
        mff_cycles(0xA4A0, 8);
        a = RAM(OBJ_DIR + x) & DIR_LEFT;
        if (a == 0) {
            mff_cycles(0xA4A7, 8);
            a = RAM(OBJ_X + x);
            CMP_FLAGS(a, ARENA_RIGHT);
            if (c) {
                mff_cycles(0xA4AE, 3);
                JMP_B01(0xA54C);
            }
            mff_cycles(0xA4AC, 1);
        } else {
            mff_cycles(0xA4A5, 1);
        }
        mff_cycles(0xA4B1, 8);
        a = RAM(OBJ_DIR + x) & DIR_RIGHT;
        SET_NZ(a);
        if (z) {
            mff_cycles(0xA4B8, 8);
            a = RAM(OBJ_X + x);
            CMP_FLAGS(a, ARENA_LEFT);
            if (!c) {
                mff_cycles(0xA4BF, 3);
                JMP_B01(0xA54C);
            }
            mff_cycles(0xA4BD, 1);
        } else {
            mff_cycles(0xA4B6, 1);
        }
        mff_cycles(0xA4C2, 6);
        CALL_FIXED(0xA4C2, func_F374);
        mff_cycles(0xA4C5, 4);
        CMP_FLAGS(a, 0x30);
        if (c) {
            mff_cycles(0xA4C7, 1);
            mff_cycles(0xA4E1, 6);
            RTS_RETURN();
        }
        mff_cycles(0xA4C9, 10);
        a = 0xF6;
        y = 0x2F;
        SET_NZ(y);
        CALL_FIXED(0xA4CD, func_F44F);
        mff_cycles(0xA4D0, 6);
        a = RAM(OBJ_TIER + x);
        if (a == 0) {
            mff_cycles(0xA4D3, 1);
            mff_cycles(0xA4D9, 2);
            a = 0x17;
        } else {
            mff_cycles(0xA4D5, 4);
            a = 0x2E;
            mff_cycles(0xA4D7, 1);
        }
        mff_cycles(0xA4DB, 12);
        RAM(OBJ_ATTACK_DAMAGE + x) = a;
        t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        RAM(OBJ_TIMER + x) = t;
        SET_NZ(t);
        mff_cycles(0xA4E1, 6);
        RTS_RETURN();
    }
    mff_cycles(0xA47A, 4);
    CMP_FLAGS(a, 0x02);
    if (a == 0x02) {
        mff_cycles(0xA47C, 1);
        mff_cycles(0xA4E2, 8);
        a = RAM(OBJ_ANIM_FRAME + x);
        CMP_FLAGS(a, 0x01);
        if (a == 0x01) {
            mff_cycles(0xA4E9, 7);
            a = 0x00;
            RAM(OBJ_ANIM_TIMER + x) = a;
        } else {
            mff_cycles(0xA4E7, 1);
        }
        mff_cycles(0xA4EE, 6);
        a = RAM(OBJ_VX + x);
        if (a != 0) {
            mff_cycles(0xA4F3, 7);
            a = 0x8B;
            RAM(OBJ_ATTACK + x) = a;
        } else {
            mff_cycles(0xA4F1, 1);
        }
        mff_cycles(0xA4F8, 58);
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
            mff_cycles(0xA521, 6);
            a = RAM(OBJ_VY + x);
            SET_NZ(a);
            if (!n) {
                mff_cycles(0xA526, 3);
                JMP_B01(0xA031);
            }
            mff_cycles(0xA524, 1);
        } else {
            mff_cycles(0xA51F, 1);
        }
        mff_cycles(0xA529, 13);
        t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        RAM(OBJ_TIMER + x) = t;
        a = RAM(OBJ_TIER + x);
        if (a == 0) {
            mff_cycles(0xA52F, 1);
            mff_cycles(0xA535, 2);
            a = 0x0C;
            SET_NZ(a);
        } else {
            mff_cycles(0xA531, 4);
            a = 0x06;
            SET_NZ(a);
            mff_cycles(0xA533, 1);
        }
        mff_cycles(0xA537, 11);
        RAM(OBJ_AI_TIMER + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xA47E, 4);
    CMP_FLAGS(a, 0x03);
    if (z) {
        mff_cycles(0xA482, 3);
        JMP_B01(0xA53B);
    }
    mff_cycles(0xA480, 1);
    mff_cycles(0xA485, 6);
    CALL_FIXED(0xA485, func_F343);
    mff_cycles(0xA488, 6);
    CALL_B01(0xA488, 0xA9CA);
    mff_cycles(0xA48B, 6);
    CALL_B01(0xA48B, 0xA9F5);
    mff_cycles(0xA48E, 17);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    a = 0xF4;
    y = 0x2C;
    SET_NZ(y);
    CALL_FIXED(0xA495, func_F449);
    mff_cycles(0xA498, 11);
    a = 0x2E;
    SET_NZ(a);
    RAM(CHR_BANKS + 5) = a;
    RTS_RETURN();
}

/*
 * $A53B: the rush's pause: obj_ai_timer frames with no attack and the
 * animation held, then $A54C. Caller: the JMP at 01:A482.
 */
void func_A53B_b0(void)
{
    REGS_IN;

    mff_cycles(0xA53B, 6);
    a = RAM(OBJ_AI_TIMER + x);
    if (a != 0) {
        mff_cycles(0xA540, 25);
        a = 0x00;
        RAM(OBJ_ATTACK + x) = a;
        RAM(OBJ_ANIM_TIMER + x) = a;
        t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
        RAM(OBJ_AI_TIMER + x) = t;
        SET_NZ(t);
        RTS_RETURN();
    }
    mff_cycles(0xA53E, 1);
    RUN_ON(func_A54C_b0);
}

/*
 * $A54C: the rush is over: phase 0 with BOSS_TRIED 1.
 * Callers: the run-on from $A53E; the JMPs at 01:A4AE, A4BF (never
 * executed).
 */
void func_A54C_b0(void)
{
    REGS_IN;

    mff_cycles(0xA54C, 20);
    a = OBJ_LIVE;
    RAM(OBJ_STATE + x) = a;
    a = 0x01;
    SET_NZ(a);
    RAM(BOSS_TRIED + x) = a;
    RTS_RETURN();
}

/* ---- phases 5 and 6: Thrasher's jumps ---- */

/*
 * Phase 5, $A557: jump on the spot, twice. Even obj_timer: take off from
 * the ground line (kept in obj_ground_y) at obj_vy $04E0 ($0400 from
 * phase 6), animation $E7 (CHR $2A); odd: in the air (0F:F1FD); falling,
 * frame 1 with the stomp out (obj_attack $8F, damage $16); on landing
 * obj_timer + 1, and at 4 the jumps are over: phase 1, obj_timer 1.
 * Callers: obj_behavior's phase table; 01:A5D9 (phase 6).
 */
void func_A557_b0(void)
{
    REGS_IN;

    mff_cycles(0xA557, 8);
    a = RAM(OBJ_TIMER + x);
    c = a & 1;
    a >>= 1;
    SET_NZ(a);
    if (!c) {
        mff_cycles(0xA55D, 17);
        a = RAM(OBJ_Y + x);
        RAM(OBJ_GROUND_Y + x) = a;
        a = RAM(OBJ_STATE + x);
        CMP_FLAGS(a, TS_LEAP);
        if (a == TS_LEAP) {
            mff_cycles(0xA568, 1);
            mff_cycles(0xA576, 14);
            a = 0x00;
            RAM(OBJ_VY_FRAC + x) = a;
            a = 0x04;
            RAM(OBJ_VY + x) = a;
        } else {
            mff_cycles(0xA56A, 16);
            a = 0xE0;
            RAM(OBJ_VY_FRAC + x) = a;
            a = 0x04;
            RAM(OBJ_VY + x) = a;
            mff_cycles(0xA574, 1);
        }
        mff_cycles(0xA580, 14);
        t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        RAM(OBJ_TIMER + x) = t;
        a = 0xE7;
        y = 0x2A;
        SET_NZ(y);
        JMP_F(0xF449, func_F449);
    }
    mff_cycles(0xA55B, 1);
    mff_cycles(0xA58A, 6);
    CALL_FIXED(0xA58A, func_F1FD);
    mff_cycles(0xA58D, 6);
    a = RAM(OBJ_VY + x);
    SET_NZ(a);
    if (n) {
        mff_cycles(0xA592, 31);
        a = 0x01;
        RAM(OBJ_ANIM_FRAME + x) = a;
        a = 0x8F;
        RAM(OBJ_ATTACK + x) = a;
        a = 0x16;
        RAM(OBJ_ATTACK_DAMAGE + x) = a;
        a = RAM(OBJ_Y + x);
        CMP_FLAGS(a, RAM(OBJ_GROUND_Y + x));
        if (c) {
            mff_cycles(0xA5A9, 24);
            a = RAM(OBJ_GROUND_Y + x);
            RAM(OBJ_Y + x) = a;
            RAM(OBJ_TIMER + x) = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
            a = RAM(OBJ_TIMER + x);
            CMP_FLAGS(a, 0x04);
            if (z) {
                mff_cycles(0xA5B9, 31);
                a = 0x00;
                RAM(OBJ_VY_FRAC + x) = a;
                RAM(OBJ_VY + x) = a;
                RAM(OBJ_ATTACK + x) = a;
                a = 0x01;
                RAM(OBJ_TIMER + x) = a;
                a = TS_CIRCLE;
                SET_NZ(a);
                RAM(OBJ_STATE + x) = a;
            } else {
                mff_cycles(0xA5B7, 1);
            }
        } else {
            mff_cycles(0xA5A7, 1);
        }
    } else {
        mff_cycles(0xA590, 1);
    }
    mff_cycles(0xA5CE, 6);
    RTS_RETURN();
}

/*
 * Phase 6, $A5CF: the jumps of $A557 at obj_vx 2 the way obj_dir points,
 * held inside x $18-$F1; obj_timer 3 marks a jump pressed against the
 * side (back to 1 on the next frame), so only a landing there takes it
 * to 4 and ends the leaps: phase 0.
 */
void func_A5CF_b0(void)
{
    REGS_IN;

    mff_cycles(0xA5CF, 20);
    a = 0x00;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0x02;
    SET_NZ(a);
    RAM(OBJ_VX + x) = a;
    CALL_B01(0xA5D9, 0xA557);
    mff_cycles(0xA5DC, 8);
    a = RAM(OBJ_STATE + x);
    CMP_FLAGS(a, TS_CIRCLE);
    if (a == TS_CIRCLE) {
        mff_cycles(0xA5E3, 13);
        a = TS_ENTER;
        SET_NZ(a);
        RAM(OBJ_STATE + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xA5E1, 1);
    mff_cycles(0xA5E9, 8);
    a = RAM(OBJ_TIMER + x);
    CMP_FLAGS(a, 0x03);
    if (a == 0x03) {
        mff_cycles(0xA5F0, 7);
        a = 0x01;
        RAM(OBJ_TIMER + x) = a;
    } else {
        mff_cycles(0xA5EE, 1);
    }
    mff_cycles(0xA5F5, 8);
    a = RAM(OBJ_DIR + x) & DIR_LEFT;
    SET_NZ(a);
    if (!z) {
        mff_cycles(0xA5FC, 6);
        CALL_FIXED(0xA5FC, func_F1A1);
    } else {
        mff_cycles(0xA5FA, 1);
    }
    mff_cycles(0xA5FF, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    SET_NZ(a);
    if (!z) {
        mff_cycles(0xA606, 6);
        CALL_FIXED(0xA606, func_F183);
    } else {
        mff_cycles(0xA604, 1);
    }

    mff_cycles(0xA609, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    if (a == 0) {
        mff_cycles(0xA610, 8);
        a = ARENA_LEFT;
        CMP_FLAGS(a, RAM(OBJ_X + x));
        if (c) {
            /* at or past the left side: put on it */
            mff_cycles(0xA617, 7);
            RAM(OBJ_X + x) = a;
            if (!z) {
                mff_cycles(0xA61A, 1);
                mff_cycles(0xA62D, 7);
                a = 0x03;
                SET_NZ(a);
                RAM(OBJ_TIMER + x) = a;
                mff_cycles(0xA632, 6);
                RTS_RETURN();
            }
        } else {
            mff_cycles(0xA615, 1);
        }
    } else {
        mff_cycles(0xA60E, 1);
    }
    mff_cycles(0xA61C, 8);
    a = RAM(OBJ_DIR + x) & DIR_LEFT;
    SET_NZ(a);
    if (z) {
        mff_cycles(0xA623, 8);
        a = ARENA_RIGHT;
        CMP_FLAGS(a, RAM(OBJ_X + x));
        if (!c) {
            /* past the right side: put on it */
            mff_cycles(0xA62A, 5);
            RAM(OBJ_X + x) = a;
            mff_cycles(0xA62D, 7);
            a = 0x03;
            SET_NZ(a);
            RAM(OBJ_TIMER + x) = a;
        } else {
            mff_cycles(0xA628, 1);
        }
    } else {
        mff_cycles(0xA621, 1);
    }
    mff_cycles(0xA632, 6);
    RTS_RETURN();
}

/*
 * Phase 7, $A633: hurt. Faces the player; obj_hit_timer counts down and
 * at 0 Thrasher goes to phase 0, Sodom (kind bit 3) rushes (phase 4).
 */
void func_A633_b0(void)
{
    REGS_IN;

    mff_cycles(0xA633, 6);
    CALL_FIXED(0xA633, func_F343);
    mff_cycles(0xA636, 9);
    t = (uint8_t)(RAM(OBJ_HIT_TIMER + x) - 1);
    RAM(OBJ_HIT_TIMER + x) = t;
    if (t == 0) {
        mff_cycles(0xA63B, 8);
        a = RAM(OBJ_KIND + x) & KIND_SODOM;
        if (a != 0) {
            mff_cycles(0xA640, 1);
            mff_cycles(0xA646, 2);
            a = TS_DASH;
        } else {
            mff_cycles(0xA642, 4);
            a = TS_ENTER;
            mff_cycles(0xA644, 1);
        }
        mff_cycles(0xA648, 5);
        RAM(OBJ_STATE + x) = a;
    } else {
        mff_cycles(0xA639, 1);
    }
    mff_cycles(0xA64B, 13);
    a = 0x00;
    SET_NZ(a);
    RAM(OBJ_TIMER + x) = a;
    RTS_RETURN();
}

/*
 * Phase 8, $A651: walk in. The walk animation; once on screen 1 a player
 * in reach ($20 x $0A Thrasher, $24 x 9 Sodom) starts the attack with
 * the kind's swing. It walks at index 4's speed the way obj_dir points
 * (right only when it is exactly 1) until x $18 from the left or $F1 from
 * the right on screen 1: then phase 1, BOSS_TRIED 7, facing the player
 * (JMP 0F:F343).
 */
void func_A651_b0(void)
{
    REGS_IN;

    mff_cycles(0xA651, 6);
    CALL_B01(0xA651, 0xA882);
    mff_cycles(0xA654, 8);
    a = RAM(OBJ_X_HI + x);
    CMP_FLAGS(a, 0x01);
    if (a == 0x01) {
        mff_cycles(0xA65B, 8);
        a = RAM(OBJ_KIND + x);
        CMP_FLAGS(a, KIND_THRASHER);
        if (a == KIND_THRASHER) {
            mff_cycles(0xA662, 6);
            a = REACH_X_AGAIN;
            y = REACH_Y_AGAIN;
            SET_NZ(y);
            mff_cycles(0xA666, 1);
        } else {
            mff_cycles(0xA660, 1);
            mff_cycles(0xA668, 4);
            a = REACH_X_SODOM;
            y = REACH_Y_SODOM;
            SET_NZ(y);
        }
        mff_cycles(0xA66C, 6);
        CALL_FIXED(0xA66C, func_FB3E);
        mff_cycles(0xA66F, 2);
        if (c) {
            mff_cycles(0xA671, 31);
            a = RAM(OBJ_KIND + x);
            SBC_A(KIND_THRASHER);               /* C is set */
            RAM(BOSS_ATTACK_SET + x) = a;
            a = TS_ATTACK;
            RAM(OBJ_STATE + x) = a;
            a = 0x00;
            SET_NZ(a);
            RAM(OBJ_TIMER + x) = a;
            RTS_RETURN();
        }
        mff_cycles(0xA66F, 1);
    } else {
        mff_cycles(0xA659, 1);
    }

    mff_cycles(0xA684, 8);
    y = DIR_INDEX_RIGHT;
    SET_NZ(y);
    CALL_B01(0xA686, 0xA7E1);
    mff_cycles(0xA689, 8);
    a = RAM(OBJ_DIR + x);
    if (a == DIR_RIGHT) {
        mff_cycles(0xA690, 8);
        a = RAM(OBJ_X_HI + x);
        CMP_FLAGS(a, 0x01);
        if (z) {
            mff_cycles(0xA697, 8);
            a = RAM(OBJ_X + x);
            CMP_FLAGS(a, ARENA_LEFT);
            if (c) {
                mff_cycles(0xA69C, 1);
                RUN_ON(walk_in_arrived);
            }
        } else {
            mff_cycles(0xA695, 1);
        }
        mff_cycles(0xA69E, 3);
        JMP_F(0xF183, func_F183);
    }
    mff_cycles(0xA68E, 1);
    mff_cycles(0xA6A1, 8);
    a = RAM(OBJ_X_HI + x);
    CMP_FLAGS(a, 0x01);
    if (z) {
        mff_cycles(0xA6A8, 8);
        a = RAM(OBJ_X + x);
        CMP_FLAGS(a, ARENA_RIGHT + 1);
        if (!c) {
            mff_cycles(0xA6AD, 1);
            RUN_ON(walk_in_arrived);
        }
    } else {
        mff_cycles(0xA6A6, 1);
    }
    mff_cycles(0xA6AF, 3);
    JMP_F(0xF1A1, func_F1A1);
}

/* $A6B2: on screen: phase 1, BOSS_TRIED 7, facing the player */
static void walk_in_arrived(void)
{
    REGS_IN;

    mff_cycles(0xA6B2, 24);
    a = TS_CIRCLE;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x07;
    SET_NZ(a);
    RAM(BOSS_TRIED + x) = a;
    JMP_F(0xF343, func_F343);
}
