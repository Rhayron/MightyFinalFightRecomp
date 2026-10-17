/*
 * enemy_ai.c - the common-enemy AI of bank $0D: the phases that walk an
 * enemy up to the player and hit him, and enemy_react, which runs before
 * them. The special moves are in enemy_moves.c, walking, pits, speeds and
 * animations in enemy_walk.c; enemy_ai.h has the states.
 *
 * Who runs it: the behaviours of ids $20-$5F (0C:9CED-9F12; the common
 * enemies Poison, Two P, Bred, AxL, El Gado and Andore at tiers 1/2/4, and
 * the ids that make them enter by a charge ($2C-$2F), a drop from above
 * ($36-$3E) or a wait ($3F-$5F)) set obj_tier and obj_kind, call
 * enemy_react ($AB29) at 0C:9D1F and, when it returns C clear, jump through
 * 0C:9D3B/9D4B by obj_state & $0F:
 *
 *   phase  routine  what the enemy does
 *     0    $A04C    start: AI counters cleared, standing
 *     1    $A060    approach: walks at the player until near, or away
 *                   from a pit (obj_pit_flag)
 *     2    $A0E8    maneuver: walks 90 degrees off the player's direction;
 *                   level with him it may pick a move ($ADFC by kind)
 *     3    $A18E    block (AxL only, from obj_take_hit 0C:8574): slides
 *                   back, holds the guard $20 frames
 *     4    $A210    close in: walks to the spot beside the player until he
 *                   is in reach, a number of tries ($A3DF by id)
 *     5    $A2C3    attack: one to three hits (combo $A2AC / $AE2C)
 *     6    $A41D    jump start (Poison's back flip, El Gado's jump)
 *     7    $A3F5    retreat: walks away for obj_ai_timer frames
 *     8    $A411    hit stun: faces the player until obj_hit_timer runs out
 *     9    $A494    jump (enemy_moves.c)
 *    10    $A54F    knife throw (El Gado)
 *    11    $A652    slide (El Gado)
 *    12    $A717    recover after a jump or slide, then retreat
 *    13    $A732    idle: waits obj_ai_timer frames (ids $3F-$5F)
 *    14    $A73A    charge (Andore)
 *    15    $A89C    drop in from above (ids $36-$3E)
 *
 * Phases 1, 2, 4 and 7 end by walking (enemy_walk, $A953), which turns an
 * enemy that meets a pit back to phase 1 with obj_pit_flag set. Nearly
 * every phase ends in enemy_reset ($A8E8: phase 1). The tour logs
 * (runs/p4/base_*_tour.flog, *_alt, *_full) show every phase: Two P and
 * Bred only use 0-2, 4, 5, 7, 8; AxL adds 3, 13 and 15; Poison 6, 9, 12,
 * 15; El Gado 6, 9-13, 15; Andore 13 and 14.
 *
 * In: X = slot (all of them).
 */
#include "enemy_ai.h"

/* phase 2: the move by the direction index of the player (0F:FB5B), one
 * table per obj_ai_flag. Low nibble: the direction index to walk, 90
 * degrees off the player's; high nibble (only when he is level, indexes
 * 3-5 and 11-13): a move may be picked from MOVE_PICKS */
#define MANEUVER_MOVES_A    0xAD66
#define MANEUVER_MOVES_B    0xAD76
/* by obj_kind * 8 + rng & 7: high nibble 0 none, 1 close in and attack,
 * 2-5 the move of MOVE_STATES (2 jump, 3 slide, 4 knife, 5 charge) */
#define MOVE_PICKS          0xADFC
#define MOVE_STATES         0xA133
/* phase 4 begins: by obj_kind, the odds (rng+1 & $0F below it) of a wait
 * first, and that wait; by obj_id - $20, the tries to get in reach */
#define WAIT_ODDS           0xAE32
#define WAIT_TIME           0xAE38
#define CLOSE_IN_TRIES      0xA3DF
/* by obj_kind: the reach of the attack (x, y; 0F:FB3E), the odds of a
 * retreat instead of an attack (rng & $0F below it), the hits of a combo
 * (0 = 1 or 3 at random, odds by tier in COMBO_ODDS + 3), the frame of the
 * attack animation that puts the hit out */
#define REACH_X             0xA3A7
#define REACH_Y             0xA3AD
#define RETREAT_ODDS        0xA2BD
#define COMBO_HITS          0xA2AC
#define COMBO_ODDS          0xAE2C
#define HIT_FRAME           0xA3A1
/* by obj_id - $20: the damage index (obj_attack_damage, 0F:F8D5) of a hit
 * and of the last hit of a combo (AxL: every hit) */
#define HIT_DAMAGE          0xA3B3
#define LAST_HIT_DAMAGE     0xA3C9
/* phase 7: the direction index to walk by the player's */
#define RETREAT_DIRS        0xAE3E

/* the near box of phase 1 ($38 x $18) and the wider one ($18 x $28) */
#define NEAR_X              0x38
#define NEAR_Y              0x18
#define NEAR2_X             0x18
#define NEAR2_Y             0x28

static void approach_avoid_pit(void);
static void attack_next_hit(void);
static void react_knock_down(void);

/*
 * enemy_start, $A04C (name proposed): phase 0. obj_ai_timer, obj_ai_count,
 * obj_ai_flag and obj_timer3 cleared, phase 1, JMP enemy_anim_stand.
 * Caller: 0C:9D38 (phase 0).
 */
void func_A04C_b6(void)
{
    REGS_IN;

    mff_cycles(0xA04C, 32);
    a = 0x00;
    RAM(OBJ_AI_TIMER + x) = a;
    RAM(OBJ_AI_COUNT + x) = a;
    RAM(OBJ_AI_FLAG + x) = a;
    RAM(OBJ_TIMER3 + x) = a;
    RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    GOTO_BANK(0xAAAE);
}

/*
 * enemy_approach, $A060 (name proposed): phase 1. With obj_pit_flag set
 * the enemy first walks away from the pit ($A095). Otherwise it shows the
 * walk and, once the player is within $38 x $18 or $18 x $28 (0F:FB3E),
 * goes on to the maneuver (phase 2) with a coin flip in obj_ai_flag; if
 * not, it walks at him (enemy_walk_to_player, which the code runs on into).
 * Caller: 0C:9D38 (phase 1).
 */
void func_A060_b6(void)
{
    REGS_IN;

    mff_cycles(0xA060, 6);
    if (RAM(OBJ_PIT_FLAG + x) != 0) {
        mff_cycles(0xA063, 1);
        SYNC_OUT;
        approach_avoid_pit();
        return;
    }
    mff_cycles(0xA065, 6);
    CALL_BANK(0xA065, 0xAAD7);                  /* enemy_anim_walk */
    mff_cycles(0xA068, 10);
    a = NEAR_X;
    y = NEAR_Y;
    CALL_FIXED(0xA06C, func_FB3E);              /* player within A x Y */
    mff_cycles(0xA06F, 2);
    if (c) {
        mff_cycles(0xA06F, 1);
    } else {
        mff_cycles(0xA071, 10);
        a = NEAR2_X;
        y = NEAR2_Y;
        CALL_FIXED(0xA075, func_FB3E);
        mff_cycles(0xA078, 2);
        if (!c) {
            mff_cycles(0xA078, 1);
            FALL_INTO(func_A085_b6);            /* enemy_walk_to_player */
        }
    }
    mff_cycles(0xA07A, 23);
    RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    a = RAM(RNG) & 0x01;
    RAM(OBJ_AI_FLAG + x) = a;
    RTS_RETURN();
}

/*
 * $A095-$A0E7: phase 1 with obj_pit_flag set (enemy_pit_beside found a pit,
 * or enemy_walk stepped on one). Facing the player; flag 1 (bit 0): walk up
 * (direction 8, speed index 0) with 0F:F0F7, done when obj_y < $88; flag 2,
 * or the step up was pushed back: walk sideways along obj_dir (speed index
 * 4) and probe point 7 (right) or 8 (left) of 0F:ECCF, done when it finds
 * no pit. Done: obj_pit_flag 0, JMP enemy_reset.
 */
static void approach_avoid_pit(void)
{
    REGS_IN;

    mff_cycles(0xA095, 6);
    CALL_FIXED(0xA095, func_F343);              /* obj_face_player */
    mff_cycles(0xA098, 8);
    a = RAM(OBJ_PIT_FLAG + x);
    c = a & 1;                                  /* LSR */
    a >>= 1;
    if (c) {
        mff_cycles(0xA09E, 8);
        y = 0x00;
        CALL_BANK(0xA0A0, 0xAC34);              /* enemy_set_speed: up */
        mff_cycles(0xA0A3, 15);
        a = DIR_UP;
        RAM(OBJ_DIR + x) = a;
        y = 0x01;
        CALL_FIXED(0xA0AA, func_F0F7);          /* step up */
        mff_cycles(0xA0AD, 2);
        if (!c) {
            mff_cycles(0xA0AF, 8);
            a = RAM(OBJ_Y + x);
            c = a >= 0x88;
            if (!c) {
                mff_cycles(0xA0B4, 1);
                goto done;
            }
            mff_cycles(0xA0B6, 6);
            RTS_RETURN();
        }
        mff_cycles(0xA0AD, 1);
    } else {
        mff_cycles(0xA09C, 1);
    }
    mff_cycles(0xA0B7, 8);
    y = 0x04;
    CALL_BANK(0xA0B9, 0xAC34);                  /* enemy_set_speed: sideways */
    mff_cycles(0xA0BC, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    if (a != 0) {
        mff_cycles(0xA0C3, 6);
        CALL_FIXED(0xA0C3, func_F183);          /* obj_move_right */
        mff_cycles(0xA0C6, 8);
        y = 0x07;
        CALL_FIXED(0xA0C8, func_ECCF);
        mff_cycles(0xA0CB, 7);
        a = RAM(WALK_PROBE) & TILE_PIT;
        if (a == 0) {
            mff_cycles(0xA0CF, 1);
            goto done;
        }
        mff_cycles(0xA0D1, 6);
        RTS_RETURN();
    }
    mff_cycles(0xA0C1, 1);
    mff_cycles(0xA0D2, 6);
    CALL_FIXED(0xA0D2, func_F1A1);              /* obj_move_left */
    mff_cycles(0xA0D5, 8);
    y = 0x08;
    CALL_FIXED(0xA0D7, func_ECCF);
    mff_cycles(0xA0DA, 7);
    a = RAM(WALK_PROBE) & TILE_PIT;
    if (a != 0) {
        mff_cycles(0xA0B6, 1);
        mff_cycles(0xA0B6, 6);
        RTS_RETURN();
    }
done:
    mff_cycles(0xA0E0, 10);
    a = 0x00;
    RAM(OBJ_PIT_FLAG + x) = a;
    GOTO_BANK(0xA8E8);                          /* enemy_reset */
}

/*
 * enemy_maneuver, $A0E8 (name proposed): phase 2. While obj_ai_timer
 * counts down it walks (JMP $A18B: enemy_walk); when it runs out, the
 * next move comes from MANEUVER_MOVES_A or _B (by obj_ai_flag), indexed by
 * the direction of the player (0F:FB5B): enemy_maneuver_pick, which the B
 * table's path runs on into.
 * Caller: 0C:9D38 (phase 2).
 */
void func_A0E8_b6(void)
{
    REGS_IN;
    uint8_t timer;

    mff_cycles(0xA0E8, 9);
    timer = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = timer;
    if (!(timer & 0x80)) {
        mff_cycles(0xA0ED, 3);
        GOTO_BANK(0xA18B);
    }
    mff_cycles(0xA0EB, 1);
    mff_cycles(0xA0F0, 6);
    CALL_FIXED(0xA0F0, func_FB5B);              /* direction index to the player */
    mff_cycles(0xA0F3, 8);
    y = a;
    if (RAM(OBJ_AI_FLAG + x) == 0) {
        mff_cycles(0xA0F9, 4);
        a = prg_read((uint16_t)(MANEUVER_MOVES_A + y));
        mff_cycles(0xA0FC, 3);
        GOTO_BANK(0xA102);
    }
    mff_cycles(0xA0F7, 1);
    mff_cycles(0xA0FF, 4);
    a = prg_read((uint16_t)(MANEUVER_MOVES_B + y));
    FALL_INTO(func_A102_b6);
}

/*
 * enemy_maneuver_pick, $A102 (name proposed): A = the move. It goes to
 * obj_timer. With a high nibble, MOVE_PICKS by kind and rng & 7 may start
 * one: 1 = close in for an attack (BEQ enemy_close_in_begin), 2-5 = the
 * state in MOVE_STATES (jump, slide, knife, charge) with obj_timer 0. No
 * pick and the player not within $40 x $F0: back to phase 1. Otherwise it
 * walks: obj_dir from the low nibble, the kind's speed for it
 * (enemy_set_speed), obj_ai_timer from enemy_walk_time, JMP enemy_walk.
 * Callers: JMP at $A0FC; the fall-through from $A0FF.
 */
void func_A102_b6(void)
{
    REGS_IN;

    mff_cycles(0xA102, 9);
    RAM(OBJ_TIMER + x) = a;
    if ((a & 0xF0) == 0) {
        mff_cycles(0xA107, 1);
    } else {
        mff_cycles(0xA109, 29);
        RAM(TMP0) = (uint8_t)(RAM(OBJ_KIND + x) << 3);
        a = RAM(RNG) & 0x07;
        c = 0;
        ADC_A(RAM(TMP0));
        y = a;
        a = prg_read((uint16_t)(MOVE_PICKS + y));
        mff_cycles(0xA11C, 2);
        if (a != 0) {
            mff_cycles(0xA11E, 12);
            a >>= 4;
            c = a >= 0x01;
            if (a == 0x01) {
                mff_cycles(0xA124, 1);
                BRANCH_TAIL(0xA139);            /* enemy_close_in_begin */
            }
            mff_cycles(0xA126, 6);
            y = a;
            a = prg_read((uint16_t)(MOVE_STATES + y));
            mff_cycles(0xA12A, 18);
            RAM(OBJ_STATE + x) = a;
            a = 0x00;
            RAM(OBJ_TIMER + x) = a;
            RTS_RETURN();
        }
        mff_cycles(0xA11C, 1);
        mff_cycles(0xA162, 8);
        if ((RAM(OBJ_TIMER + x) & 0x30) == 0) {
            mff_cycles(0xA167, 1);
        } else {
            mff_cycles(0xA169, 10);
            a = 0x40;
            y = 0xF0;
            CALL_FIXED(0xA16D, func_FB3E);
            mff_cycles(0xA170, 2);
            if (!c) {
                mff_cycles(0xA172, 13);
                RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) - 1);
                RTS_RETURN();
            }
            mff_cycles(0xA170, 1);
        }
    }
    mff_cycles(0xA176, 12);
    y = RAM(OBJ_TIMER + x) & 0x0F;
    a = prg_read((uint16_t)(DIR_FROM_INDEX + y));
    mff_cycles(0xA17F, 11);
    RAM(OBJ_DIR + x) = a;
    CALL_BANK(0xA182, 0xAC34);                  /* enemy_set_speed */
    mff_cycles(0xA185, 6);
    CALL_BANK(0xA185, 0xACB3);                  /* enemy_walk_time */
    mff_cycles(0xA188, 8);
    RAM(OBJ_AI_TIMER + x) = a;
    GOTO_BANK(0xA953);                          /* enemy_walk */
}

/*
 * enemy_close_in_begin, $A139 (name proposed): to phase 4. obj_ai_timer
 * is a wait first (WAIT_TIME by kind, when rng+1 & $0F is below
 * WAIT_ODDS; every WAIT_TIME is nonzero) or 0; obj_ai_count the tries
 * (CLOSE_IN_TRIES by obj_id - $20).
 * Caller: BEQ at $A124 (enemy_maneuver_pick).
 */
void func_A139_b6(void)
{
    REGS_IN;
    int wait = 0;

    mff_cycles(0xA139, 13);
    y = RAM(OBJ_KIND + x);
    a = RAM(RNG + 1) & 0x0F;
    mff_cycles(0xA143, 2);
    if (a >= prg_read((uint16_t)(WAIT_ODDS + y))) {
        mff_cycles(0xA143, 1);
    } else {
        mff_cycles(0xA145, 4);
        a = prg_read((uint16_t)(WAIT_TIME + y));
        mff_cycles(0xA148, 2);
        wait = a != 0;
        if (wait)
            mff_cycles(0xA148, 1);
    }
    if (!wait) {
        mff_cycles(0xA14A, 2);
        a = 0x00;
    }
    mff_cycles(0xA14C, 19);
    RAM(OBJ_AI_TIMER + x) = a;
    a = RAM(OBJ_ID + x);
    c = 1;
    SBC_A(ENEMY_ID_FIRST);
    y = a;
    a = prg_read((uint16_t)(CLOSE_IN_TRIES + y));
    mff_cycles(0xA159, 18);
    RAM(OBJ_AI_COUNT + x) = a;
    a = AI_CLOSE_IN;
    RAM(OBJ_STATE + x) = a;
    RTS_RETURN();
}

/*
 * $A18B: JMP enemy_walk, the maneuver's walk while obj_ai_timer runs.
 * Caller: JMP at $A0ED.
 */
void func_A18B_b6(void)
{
    REGS_IN;

    mff_cycles(0xA18B, 3);
    GOTO_BANK(0xA953);
}

/*
 * enemy_block, $A18E (name proposed): phase 3, AxL's guard. obj_take_hit
 * (0C:84F0) sets it when AxL blocks: obj_timer 0, obj_ai_timer 3 or 8.
 * Step 0: standing, obj_dir away from the way it faces, speed $02E0.
 * Step 1: the guard (animation $9D, sprite bank $3D); over a pit
 * (enemy_pit_under) obj_pit_flag 0 and that is all; it slides back for
 * obj_ai_timer frames, then obj_ai_timer $20. Step 2: holds the guard
 * $20 frames, then phase 4 or 1 (rng bit 0), stopped, enemy_pit_beside.
 * Logs: steps 1 and 2 in rounds 1, 3 and 4 (AxL's).
 * Caller: 0C:9D38 (phase 3).
 */
void func_A18E_b6(void)
{
    REGS_IN;
    uint8_t timer;

    mff_cycles(0xA18E, 8);
    a = RAM(OBJ_TIMER + x);
    c = a >= 0x01;
    if (a == 0x01) {
        mff_cycles(0xA193, 1);
        mff_cycles(0xA1C2, 10);
        a = 0x9D;
        y = 0x3D;
        CALL_FIXED(0xA1C6, func_F449);          /* obj_set_anim, chr_banks+4 */
        mff_cycles(0xA1C9, 6);
        CALL_BANK(0xA1C9, 0xA9F6);              /* enemy_pit_under */
        mff_cycles(0xA1CC, 2);
        if (c) {
            mff_cycles(0xA1CE, 13);
            a = 0x00;
            RAM(OBJ_PIT_FLAG + x) = a;
            RTS_RETURN();
        }
        mff_cycles(0xA1CC, 1);
        mff_cycles(0xA1D4, 9);
        timer = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
        RAM(OBJ_AI_TIMER + x) = timer;
        if (timer != 0) {
            mff_cycles(0xA1D9, 8);
            a = RAM(OBJ_DIR + x) & DIR_RIGHT;
            if (a != 0) {
                mff_cycles(0xA1E0, 3);
                GOTO_FIXED(0xF183, func_F183);  /* obj_move_right */
            }
            mff_cycles(0xA1DE, 1);
            mff_cycles(0xA1E3, 3);
            GOTO_FIXED(0xF1A1, func_F1A1);      /* obj_move_left */
        }
        mff_cycles(0xA1D7, 1);
        mff_cycles(0xA1E6, 14);
        a = 0x20;
        RAM(OBJ_AI_TIMER + x) = a;
        RAM(OBJ_TIMER + x) = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        mff_cycles(0xA1EE, 6);
        RTS_RETURN();
    }
    mff_cycles(0xA195, 4);
    c = a >= 0x02;
    if (a == 0x02) {
        mff_cycles(0xA197, 1);
        mff_cycles(0xA1EF, 9);
        timer = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
        RAM(OBJ_AI_TIMER + x) = timer;
        if (timer != 0) {
            mff_cycles(0xA1EE, 1);
            mff_cycles(0xA1EE, 6);
            RTS_RETURN();
        }
        mff_cycles(0xA1F4, 7);
        c = RAM(RNG) & 1;                       /* LSR / BCC */
        if (c) {
            mff_cycles(0xA1F9, 4);
            a = AI_CLOSE_IN;
            mff_cycles(0xA1FB, 1);
        } else {
            mff_cycles(0xA1F7, 1);
            mff_cycles(0xA1FD, 2);
            a = AI_APPROACH;
        }
        mff_cycles(0xA1FF, 25);
        RAM(OBJ_STATE + x) = a;
        a = 0x00;
        RAM(OBJ_TIMER + x) = a;
        RAM(OBJ_VX_FRAC + x) = a;
        RAM(OBJ_VX + x) = a;
        GOTO_BANK(0xAA1D);                      /* enemy_pit_beside */
    }
    mff_cycles(0xA199, 6);
    CALL_BANK(0xA199, 0xAAAE);                  /* enemy_anim_stand */
    mff_cycles(0xA19C, 8);
    if (RAM(OBJ_FLAGS + x) & OBJ_FACING_RIGHT) {
        mff_cycles(0xA1A1, 1);
        mff_cycles(0xA1A7, 2);
        a = DIR_LEFT;
    } else {
        mff_cycles(0xA1A3, 4);
        a = DIR_RIGHT;
        mff_cycles(0xA1A5, 1);
    }
    mff_cycles(0xA1A9, 44);
    RAM(OBJ_DIR + x) = a;
    RAM(OBJ_VX + x) = 0x02;
    RAM(OBJ_VX_FRAC + x) = 0xE0;
    a = 0x00;
    RAM(OBJ_VY + x) = a;
    RAM(OBJ_VY_FRAC + x) = a;
    RAM(OBJ_TIMER + x) = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RTS_RETURN();
}

/*
 * enemy_close_in, $A210 (name proposed): phase 4. Facing the player; a
 * wait in obj_ai_timer first (JMP enemy_close_in_wait). Then, while the
 * player is out of the kind's reach (REACH_X x REACH_Y, 0F:FB3E), it walks
 * to the spot beside him (0F:FB72, JMP enemy_walk_index) until
 * obj_ai_count tries are spent (JMP enemy_reset_wait). In reach: AxL lets
 * a jumping player be (enemy_reset); the kind's RETREAT_ODDS give a
 * retreat (phase 7, $1E frames); otherwise the attack (phase 5):
 * obj_ai_count hits (COMBO_HITS, or 1 or 3 by COMBO_ODDS), obj_ai_timer 0,
 * facing the player, obj_timer3 and obj_land_y $FF (enemy_attack's flags).
 * Caller: 0C:9D38 (phase 4).
 */
void func_A210_b6(void)
{
    REGS_IN;

    mff_cycles(0xA210, 6);
    CALL_BANK(0xA210, 0xA9EB);                  /* enemy_face_player */
    mff_cycles(0xA213, 6);
    a = RAM(OBJ_AI_TIMER + x);
    if (a != 0) {
        mff_cycles(0xA218, 3);
        GOTO_BANK(0xA2A6);                      /* enemy_close_in_wait */
    }
    mff_cycles(0xA216, 1);
    mff_cycles(0xA21B, 8);
    y = RAM(OBJ_KIND + x);
    a = prg_read((uint16_t)(REACH_X + y));
    mff_cycles(0xA221, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(REACH_Y + y));
    mff_cycles(0xA226, 11);
    y = a;
    a = RAM(TMP0);
    CALL_FIXED(0xA229, func_FB3E);              /* player within A x Y */
    mff_cycles(0xA22C, 2);
    if (!c) {
        uint8_t tries;

        mff_cycles(0xA22E, 9);
        tries = (uint8_t)(RAM(OBJ_AI_COUNT + x) - 1);
        RAM(OBJ_AI_COUNT + x) = tries;
        if (tries != 0) {
            mff_cycles(0xA233, 6);
            CALL_FIXED(0xA233, func_FB72);      /* direction index to the spot beside him */
            mff_cycles(0xA236, 3);
            GOTO_BANK(0xA088);                  /* enemy_walk_index */
        }
        mff_cycles(0xA231, 1);
        mff_cycles(0xA239, 3);
        GOTO_BANK(0xA8E4);                      /* enemy_reset_wait */
    }
    mff_cycles(0xA22C, 1);
    mff_cycles(0xA23C, 8);
    y = RAM(OBJ_KIND + x);
    if (y == KIND_AXL) {
        mff_cycles(0xA243, 7);
        a = RAM(GAME_ACTION);
        c = a >= ACT_JUMP;
        if (a == ACT_JUMP) {
            mff_cycles(0xA249, 3);
            GOTO_BANK(0xA8E8);                  /* enemy_reset */
        }
        mff_cycles(0xA247, 1);
    } else {
        mff_cycles(0xA241, 1);
    }
    mff_cycles(0xA24C, 9);
    a = RAM(RNG) & 0x0F;
    c = a >= prg_read((uint16_t)(RETREAT_ODDS + y));
    mff_cycles(0xA253, 2);
    if (!c) {
        mff_cycles(0xA253, 1);
        mff_cycles(0xA2B2, 20);
        RAM(OBJ_AI_TIMER + x) = 0x1E;
        a = AI_RETREAT;
        RAM(OBJ_STATE + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xA255, 15);
    a = 0x00;
    RAM(OBJ_AI_COUNT + x) = a;
    y = RAM(OBJ_KIND + x);
    a = prg_read((uint16_t)(COMBO_HITS + y));
    mff_cycles(0xA260, 2);
    if (a != 0) {
        mff_cycles(0xA262, 7);
        RAM(OBJ_AI_COUNT + x) = a;
        mff_cycles(0xA265, 1);
    } else {
        /* only Bred and Andore get here: the "kind 0" offset is never used */
        mff_cycles(0xA260, 1);
        mff_cycles(0xA267, 6);
        a = RAM(OBJ_KIND + x);
        if (a != 0) {
            mff_cycles(0xA26C, 2);
            a = 0x03;
        } else {
            mff_cycles(0xA26A, 1);
        }
        mff_cycles(0xA26E, 11);
        RAM(TMP0) = a;
        y = RAM(OBJ_TIER + x);
        a = prg_read((uint16_t)(TIER_INDEX + y));
        mff_cycles(0xA276, 11);
        c = 0;
        ADC_A(RAM(TMP0));
        y = a;
        a = prg_read((uint16_t)(COMBO_ODDS + y));
        mff_cycles(0xA27D, 13);
        RAM(TMP0) = a;
        a = RAM(RNG) & 0x0F;
        c = a >= RAM(TMP0);
        if (c) {
            mff_cycles(0xA287, 14);
            RAM(OBJ_AI_COUNT + x) = (uint8_t)(RAM(OBJ_AI_COUNT + x) + 2);
        } else {
            mff_cycles(0xA285, 1);
        }
        mff_cycles(0xA28D, 7);
        RAM(OBJ_AI_COUNT + x) = (uint8_t)(RAM(OBJ_AI_COUNT + x) + 1);
    }
    mff_cycles(0xA290, 13);
    a = 0x00;
    RAM(OBJ_AI_TIMER + x) = a;
    CALL_FIXED(0xA295, func_F343);              /* obj_face_player */
    mff_cycles(0xA298, 25);
    a = 0xFF;
    RAM(OBJ_TIMER3 + x) = a;
    RAM(OBJ_LAND_Y + x) = a;
    a = AI_ATTACK;
    RAM(OBJ_STATE + x) = a;
    RTS_RETURN();
}

/*
 * enemy_close_in_wait, $A2A6 (name proposed): obj_ai_timer - 1, JMP
 * enemy_anim_stand.
 * Caller: JMP at $A218 (enemy_close_in).
 */
void func_A2A6_b6(void)
{
    REGS_IN;

    mff_cycles(0xA2A6, 10);
    RAM(OBJ_AI_TIMER + x) = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    GOTO_BANK(0xAAAE);
}

/*
 * enemy_attack, $A2C3 (name proposed): phase 5, obj_ai_count hits. A pause
 * in obj_ai_timer shows the stand (enemy_hit_pause loads it between hits).
 * Then the attack animation (enemy_anim_attack): when the animation comes
 * round to its start (Andore: frame 6), obj_timer3 tells the first time
 * ($FF, counted up to 0) from the end of a hit (0: attack_next_hit). On the
 * kind's HIT_FRAME (timer 1) the hit goes out (enemy_attack_out); while it
 * is out, a hit on the player plays sound $18 (El Gado $1F) once.
 * obj_land_y is nonzero until the last hit, which takes LAST_HIT_DAMAGE
 * (AxL: always).
 * Caller: 0C:9D38 (phase 5).
 */
void func_A2C3_b6(void)
{
    REGS_IN;
    int anim_end;

    mff_cycles(0xA2C3, 6);
    a = RAM(OBJ_AI_TIMER + x);
    if (a != 0) {
        mff_cycles(0xA2C8, 10);
        RAM(OBJ_AI_TIMER + x) = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
        GOTO_BANK(0xAAAE);                      /* enemy_anim_stand */
    }
    mff_cycles(0xA2C6, 1);
    mff_cycles(0xA2CE, 6);
    CALL_BANK(0xA2CE, 0xAB00);                  /* enemy_anim_attack */
    mff_cycles(0xA2D1, 8);
    a = RAM(OBJ_KIND + x);
    if (a == KIND_ANDORE) {
        mff_cycles(0xA2D8, 8);
        anim_end = 0;
        a = RAM(OBJ_ANIM_FRAME + x);
        if (a != 0x06) {
            mff_cycles(0xA2DD, 1);
        } else {
            mff_cycles(0xA2DF, 6);
            a = RAM(OBJ_ANIM_TIMER + x);
            if (a != 0) {
                mff_cycles(0xA2E2, 1);
            } else {
                mff_cycles(0xA2E4, 2);
                mff_cycles(0xA2E4, 2);          /* BEQ into the next page */
                anim_end = 1;
            }
        }
        if (anim_end) {
            SYNC_OUT;
            attack_next_hit();
            return;
        }
    } else {
        mff_cycles(0xA2D6, 1);
        mff_cycles(0xA2E6, 10);
        a = RAM(OBJ_ANIM_TIMER + x) | RAM(OBJ_ANIM_FRAME + x);
        if (a != 0) {
            mff_cycles(0xA2EC, 1);
        } else {
            mff_cycles(0xA2EE, 6);
            a = RAM(OBJ_TIMER3 + x);
            if (a == 0) {
                mff_cycles(0xA2F1, 2);
                SYNC_OUT;
                attack_next_hit();
                return;
            }
            mff_cycles(0xA2F3, 13);
            RAM(OBJ_TIMER3 + x) = (uint8_t)(RAM(OBJ_TIMER3 + x) + 1);
            RTS_RETURN();
        }
    }
    mff_cycles(0xA2F7, 8);
    a = RAM(OBJ_ANIM_TIMER + x);
    if (a != 0x01) {
        mff_cycles(0xA2FC, 2);
    } else {
        mff_cycles(0xA2FE, 8);
        y = RAM(OBJ_KIND + x);
        a = prg_read((uint16_t)(HIT_FRAME + y));
        mff_cycles(0xA304, 6);
        if (a == RAM(OBJ_ANIM_FRAME + x)) {
            mff_cycles(0xA307, 1);
            mff_cycles(0xA324, 18);
            a = RAM(OBJ_ID + x);
            c = 1;
            SBC_A(ENEMY_ID_FIRST);
            y = a;
            a = RAM(OBJ_KIND + x);
            c = a >= KIND_AXL;
            if (a != KIND_AXL) {
                mff_cycles(0xA332, 6);
                if (RAM(OBJ_LAND_Y + x) != 0) {
                    mff_cycles(0xA335, 1);
                    mff_cycles(0xA33D, 4);
                    a = prg_read((uint16_t)(HIT_DAMAGE + y));
                    FALL_INTO(func_A340_b6);
                }
            } else {
                mff_cycles(0xA330, 1);
            }
            mff_cycles(0xA337, 4);
            a = prg_read((uint16_t)(LAST_HIT_DAMAGE + y));
            mff_cycles(0xA33A, 3);
            GOTO_BANK(0xA340);
        }
    }
    mff_cycles(0xA309, 6);
    a = RAM(OBJ_HIT_PLAYER + x);
    if (a != 0) {
        mff_cycles(0xA30E, 8);
        a = RAM(OBJ_KIND + x);
        if (a == KIND_EL_GADO) {
            mff_cycles(0xA315, 4);
            a = 0x1F;
            mff_cycles(0xA317, 1);
        } else {
            mff_cycles(0xA313, 1);
            mff_cycles(0xA319, 2);
            a = 0x18;
        }
        mff_cycles(0xA31B, 6);
        CALL_FIXED(0xA31B, func_F34C);          /* sound_queue_push_sfx */
        mff_cycles(0xA31E, 7);
        a = 0x00;
        RAM(OBJ_HIT_PLAYER + x) = a;
    } else {
        mff_cycles(0xA30C, 1);
    }
    mff_cycles(0xA323, 6);
    RTS_RETURN();
}

/*
 * $A34D-$A3A0: a hit of enemy_attack is over. The last one (obj_ai_count
 * down to 0): flags cleared, a retreat of $0F frames (phase 7). Otherwise,
 * the player out of reach, knocked down (game_action 6) or lying ($1F):
 * flags cleared, enemy_reset. Else the next hit: facing him, the pause
 * (enemy_hit_pause), obj_timer3 back to $FF and, before the last hit,
 * obj_land_y + 1 (to 0).
 */
static void attack_next_hit(void)
{
    REGS_IN;
    uint8_t hits;

    mff_cycles(0xA34D, 9);
    hits = (uint8_t)(RAM(OBJ_AI_COUNT + x) - 1);
    RAM(OBJ_AI_COUNT + x) = hits;
    if (hits == 0) {
        mff_cycles(0xA352, 32);
        a = 0x00;
        RAM(OBJ_LAND_Y + x) = a;
        RAM(OBJ_TIMER3 + x) = a;
        RAM(OBJ_AI_TIMER + x) = 0x0F;
        a = AI_RETREAT;
        RAM(OBJ_STATE + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xA350, 1);
    mff_cycles(0xA365, 8);
    y = RAM(OBJ_KIND + x);
    a = prg_read((uint16_t)(REACH_X + y));
    mff_cycles(0xA36B, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(REACH_Y + y));
    mff_cycles(0xA370, 11);
    y = a;
    a = RAM(TMP0);
    CALL_FIXED(0xA373, func_FB3E);              /* player within A x Y */
    mff_cycles(0xA376, 2);
    if (c) {
        mff_cycles(0xA376, 1);
        mff_cycles(0xA383, 7);
        a = RAM(GAME_ACTION);
        c = a >= ACT_KNOCKDOWN;
        if (a == ACT_KNOCKDOWN) {
            mff_cycles(0xA378, 1);
        } else {
            mff_cycles(0xA389, 4);
            c = a >= ACT_LYING;
            if (a == ACT_LYING) {
                mff_cycles(0xA378, 1);
            } else {
                mff_cycles(0xA38D, 6);
                CALL_FIXED(0xA38D, func_F343);  /* obj_face_player */
                mff_cycles(0xA390, 6);
                CALL_BANK(0xA390, 0xAAA0);      /* enemy_hit_pause */
                mff_cycles(0xA393, 15);
                RAM(OBJ_TIMER3 + x) = (uint8_t)(RAM(OBJ_TIMER3 + x) - 1);
                a = RAM(OBJ_AI_COUNT + x);
                c = a >= 0x01;
                if (a == 0x01) {
                    mff_cycles(0xA39D, 7);
                    RAM(OBJ_LAND_Y + x) = (uint8_t)(RAM(OBJ_LAND_Y + x) + 1);
                } else {
                    mff_cycles(0xA39B, 1);
                }
                mff_cycles(0xA3A0, 6);
                RTS_RETURN();
            }
        }
    }
    mff_cycles(0xA378, 15);
    a = 0x00;
    RAM(OBJ_LAND_Y + x) = a;
    RAM(OBJ_TIMER3 + x) = a;
    GOTO_BANK(0xA8E8);                          /* enemy_reset */
}

/*
 * enemy_attack_out, $A340 (name proposed): A = the damage index. The hit
 * goes out: obj_attack_damage = A, obj_attack = $80 + obj_kind (the reach
 * box is the kind).
 * Callers: JMP at $A33A; the fall-through from $A33D (enemy_attack).
 */
void func_A340_b6(void)
{
    REGS_IN;

    mff_cycles(0xA340, 24);
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = RAM(OBJ_KIND + x);
    c = 0;
    ADC_A(0x80);
    RAM(OBJ_ATTACK + x) = a;
    RTS_RETURN();
}

/*
 * enemy_retreat, $A3F5 (name proposed): phase 7. For obj_ai_timer frames
 * it walks the way RETREAT_DIRS gives for the direction of the player
 * (away from him, sideways), then enemy_reset.
 * Caller: 0C:9D38 (phase 7).
 */
void func_A3F5_b6(void)
{
    REGS_IN;
    uint8_t timer;

    mff_cycles(0xA3F5, 9);
    timer = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = timer;
    if (timer & 0x80) {
        mff_cycles(0xA3F8, 2);
        mff_cycles(0xA40E, 3);
        GOTO_BANK(0xA8E8);                      /* enemy_reset */
    }
    mff_cycles(0xA3FA, 6);
    CALL_FIXED(0xA3FA, func_FB5B);              /* direction index to the player */
    mff_cycles(0xA3FD, 6);
    y = a;
    a = prg_read((uint16_t)(RETREAT_DIRS + y));
    mff_cycles(0xA401, 6);
    y = a;
    a = prg_read((uint16_t)(DIR_FROM_INDEX + y));
    mff_cycles(0xA405, 11);
    RAM(OBJ_DIR + x) = a;
    CALL_BANK(0xA408, 0xAC34);                  /* enemy_set_speed */
    mff_cycles(0xA40B, 3);
    GOTO_BANK(0xA953);                          /* enemy_walk */
}

/*
 * enemy_hit_stun, $A411 (name proposed): phase 8 (enemy_react sets it
 * when a hit lands). Faces the player until obj_hit_timer counts out, then
 * enemy_reset.
 * Caller: 0C:9D38 (phase 8).
 */
void func_A411_b6(void)
{
    REGS_IN;
    uint8_t timer;

    mff_cycles(0xA411, 6);
    CALL_FIXED(0xA411, func_F343);              /* obj_face_player */
    mff_cycles(0xA414, 9);
    timer = (uint8_t)(RAM(OBJ_HIT_TIMER + x) - 1);
    RAM(OBJ_HIT_TIMER + x) = timer;
    if (timer == 0) {
        mff_cycles(0xA419, 3);
        GOTO_BANK(0xA8E8);                      /* enemy_reset */
    }
    mff_cycles(0xA417, 1);
    mff_cycles(0xA41C, 6);
    RTS_RETURN();
}

/*
 * enemy_recover, $A717 (name proposed): phase 12, after a jump, a slide or
 * a drop: obj_ai_timer frames, then standing, enemy_pit_beside and, with
 * no pit, a retreat of $1E frames (phase 7).
 * Caller: 0C:9D38 (phase 12).
 */
void func_A717_b6(void)
{
    REGS_IN;
    uint8_t timer;

    mff_cycles(0xA717, 9);
    timer = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = timer;
    if (timer == 0) {
        mff_cycles(0xA71C, 6);
        CALL_BANK(0xA71C, 0xAAAE);              /* enemy_anim_stand */
        mff_cycles(0xA71F, 6);
        CALL_BANK(0xA71F, 0xAA1D);              /* enemy_pit_beside */
        mff_cycles(0xA722, 6);
        a = RAM(OBJ_PIT_FLAG + x);
        if (a == 0) {
            mff_cycles(0xA727, 14);
            RAM(OBJ_AI_TIMER + x) = 0x1E;
            a = AI_RETREAT;
            RAM(OBJ_STATE + x) = a;
        } else {
            mff_cycles(0xA725, 1);
        }
    } else {
        mff_cycles(0xA71A, 1);
    }
    mff_cycles(0xA731, 6);
    RTS_RETURN();
}

/*
 * enemy_idle, $A732 (name proposed): phase 13. Waits obj_ai_timer frames
 * (the waiting ids $3F-$5F load $78 at 0C:9EE4), then enemy_reset.
 * Caller: 0C:9D38 (phase 13).
 */
void func_A732_b6(void)
{
    REGS_IN;
    uint8_t timer;

    mff_cycles(0xA732, 9);
    timer = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = timer;
    if (timer != 0) {
        mff_cycles(0xA731, 1);                  /* BNE back to $A731: RTS */
        mff_cycles(0xA731, 6);
        RTS_RETURN();
    }
    mff_cycles(0xA737, 3);
    GOTO_BANK(0xA8E8);                          /* enemy_reset */
}

/*
 * enemy_reset_wait, $A8E4 (name proposed): back to phase 1 (approach)
 * with obj_ai_timer 8.
 * Callers: JMP at $A239 (enemy_close_in), $A5D7 (enemy_knife_throw).
 */
void func_A8E4_b6(void)
{
    REGS_IN;

    mff_cycles(0xA8E4, 4);
    a = 0x08;
    mff_cycles(0xA8E6, 1);                      /* BNE, always taken */
    mff_cycles(0xA8EA, 18);
    RAM(OBJ_AI_TIMER + x) = a;
    a = AI_APPROACH;
    RAM(OBJ_STATE + x) = a;
    RTS_RETURN();
}

/*
 * enemy_reset, $A8E8 (name proposed): back to phase 1 (approach),
 * obj_ai_timer 0. Out: A = $81.
 * Callers: most phases' ends (JMP), enemy_react (JSR $AB54, $ABE9).
 */
void func_A8E8_b6(void)
{
    REGS_IN;

    mff_cycles(0xA8E8, 20);
    a = 0x00;
    RAM(OBJ_AI_TIMER + x) = a;
    a = AI_APPROACH;
    RAM(OBJ_STATE + x) = a;
    RTS_RETURN();
}

/*
 * enemy_react, $AB29 (name proposed): what happens to the enemy before
 * its phase runs. Out: C set = skip the phase this frame.
 *  - Held (obj_held nonzero) by the player's grab (game_action 4) or knee
 *    ($21) on this slot (grab_slot): the stand or, for the knee, the hurt
 *    animation; obj_hit_timer 0, enemy_reset, facing him, C set
 *    (enemy_react_held).
 *  - Just let go (obj_held $FF): put on the player's ground line; over a
 *    pit (enemy_pit_under turns it into a pit fall) back to its y and
 *    stopped, C set; otherwise obj_held 0 and enemy_pit_beside.
 *  - Hit (obj_hit_timer nonzero): at $1E and $10 the stand, at $1C the
 *    hurt animation; facing him, phase 8, C clear (enemy_react_hit).
 *  - game_action $0A (a boss holds the player) or $19 (walking out of the
 *    area): C set. $0B (a throw): obj_hit_player 0, C clear.
 *  - In phase 1, 2, 4 or 6 while the player is knocked down, falling into
 *    a pit or lying: obj_hit_player 0, standing, enemy_reset, C set.
 *  - A new life dropping in (game_action $25) knocks every enemy not
 *    dropping in itself down (enemy_react_knockdown), C set.
 * Caller: 0C:9D1F (the common enemies' behaviour).
 */
void func_AB29_b6(void)
{
    REGS_IN;

    mff_cycles(0xAB29, 6);
    a = RAM(OBJ_HELD + x);
    if (a == 0) {
        mff_cycles(0xAB2C, 1);
    } else if (a == 0xFF) {
        mff_cycles(0xAB2E, 4);
        mff_cycles(0xAB30, 1);
        mff_cycles(0xAB5C, 21);
        RAM(TMP15) = RAM(OBJ_Y + x);
        a = RAM(PLAYER_GROUND_Y);
        RAM(OBJ_Y + x) = a;
        CALL_BANK(0xAB66, 0xA9F6);              /* enemy_pit_under */
        mff_cycles(0xAB69, 2);
        if (c) {
            mff_cycles(0xAB6B, 28);
            RAM(OBJ_Y + x) = RAM(TMP15);
            a = 0x00;
            RAM(OBJ_VY_FRAC + x) = a;
            RAM(OBJ_VY + x) = a;
            c = 1;
            RTS_RETURN();
        }
        mff_cycles(0xAB69, 1);
        mff_cycles(0xAB7A, 13);
        a = 0x00;
        RAM(OBJ_HELD + x) = a;
        CALL_BANK(0xAB7F, 0xAA1D);              /* enemy_pit_beside */
    } else {
        int held = 0;

        mff_cycles(0xAB2E, 4);
        mff_cycles(0xAB32, 7);
        a = RAM(GAME_ACTION);
        if (a == ACT_GRAB) {
            mff_cycles(0xAB36, 1);
            held = 1;
        } else {
            mff_cycles(0xAB38, 4);
            if (a != ACT_GRAB_KNEE)
                mff_cycles(0xAB3A, 1);
            else
                held = 1;
        }
        if (held) {
            mff_cycles(0xAB3C, 5);
            if (x == RAM(GRAB_SLOT)) {
                mff_cycles(0xAB40, 7);
                a = RAM(GAME_ACTION);
                c = a >= ACT_GRAB_KNEE;
                if (a == ACT_GRAB_KNEE) {
                    mff_cycles(0xAB46, 6);
                    CALL_BANK(0xAB46, 0xAA60);  /* enemy_anim_hurt */
                    mff_cycles(0xAB49, 3);
                    GOTO_BANK(0xAB4F);
                }
                mff_cycles(0xAB44, 1);
                mff_cycles(0xAB4C, 6);
                CALL_BANK(0xAB4C, 0xAAAE);      /* enemy_anim_stand */
                FALL_INTO(func_AB4F_b6);
            }
            mff_cycles(0xAB3E, 1);
        }
    }

    /* $AB82: hit? */
    mff_cycles(0xAB82, 6);
    a = RAM(OBJ_HIT_TIMER + x);
    if (a != 0) {
        mff_cycles(0xAB87, 8);
        c = a >= 0x1E;
        if (a == 0x1E) {
            mff_cycles(0xAB8C, 1);
        } else {
            mff_cycles(0xAB8E, 4);
            c = a >= 0x1C;
            if (a == 0x1C) {
                mff_cycles(0xAB90, 1);
                mff_cycles(0xAB9C, 6);
                CALL_BANK(0xAB9C, 0xAA60);      /* enemy_anim_hurt */
                FALL_INTO(func_AB9F_b6);
            }
            mff_cycles(0xAB92, 4);
            c = a >= 0x10;
            if (a != 0x10) {
                mff_cycles(0xAB94, 1);
                FALL_INTO(func_AB9F_b6);
            }
        }
        mff_cycles(0xAB96, 6);
        CALL_BANK(0xAB96, 0xAAAE);              /* enemy_anim_stand */
        mff_cycles(0xAB99, 3);
        GOTO_BANK(0xAB9F);
    }
    mff_cycles(0xAB85, 1);

    /* $ABA9: the player's action */
    mff_cycles(0xABA9, 7);
    a = RAM(GAME_ACTION);
    if (a == ACT_HOLD) {
        mff_cycles(0xABAD, 1);
        mff_cycles(0xABB7, 8);
        c = 1;
        RTS_RETURN();
    }
    mff_cycles(0xABAF, 4);
    if (a == ACT_THROW) {
        mff_cycles(0xABB1, 1);
        mff_cycles(0xABB9, 15);
        a = 0x00;
        RAM(OBJ_HIT_PLAYER + x) = a;
        c = 0;
        RTS_RETURN();
    }
    mff_cycles(0xABB3, 4);
    if (a == ACT_AREA_EXIT) {
        mff_cycles(0xABB7, 8);
        c = 1;
        RTS_RETURN();
    }
    mff_cycles(0xABB5, 1);

    /* $ABC0: waiting phases while the player is down */
    mff_cycles(0xABC0, 8);
    a = RAM(OBJ_STATE + x);
    {
        int waiting = 1;

        if (a == AI_APPROACH) {
            mff_cycles(0xABC5, 1);
        } else {
            mff_cycles(0xABC7, 4);
            if (a == AI_MANEUVER) {
                mff_cycles(0xABC9, 1);
            } else {
                mff_cycles(0xABCB, 4);
                if (a == AI_CLOSE_IN) {
                    mff_cycles(0xABCD, 1);
                } else {
                    mff_cycles(0xABCF, 4);
                    if (a != AI_JUMP_START) {
                        mff_cycles(0xABD1, 1);
                        waiting = 0;
                    }
                }
            }
        }
        if (waiting) {
            int down = 1;

            mff_cycles(0xABD3, 7);
            a = RAM(GAME_ACTION);
            c = a >= ACT_KNOCKDOWN;
            if (a == ACT_KNOCKDOWN) {
                mff_cycles(0xABD7, 1);
            } else {
                mff_cycles(0xABD9, 4);
                c = a >= ACT_PIT_FALL;
                if (a == ACT_PIT_FALL) {
                    mff_cycles(0xABDB, 1);
                } else {
                    mff_cycles(0xABDD, 4);
                    c = a >= ACT_LYING;
                    if (a != ACT_LYING) {
                        mff_cycles(0xABDF, 1);
                        down = 0;
                    }
                }
            }
            if (down) {
                mff_cycles(0xABE1, 13);
                a = 0x00;
                RAM(OBJ_HIT_PLAYER + x) = a;
                CALL_BANK(0xABE6, 0xAAAE);      /* enemy_anim_stand */
                mff_cycles(0xABE9, 6);
                CALL_BANK(0xABE9, 0xA8E8);      /* enemy_reset */
                mff_cycles(0xABEC, 8);
                c = 1;
                RTS_RETURN();
            }
        }
    }

    /* $ABEE: a new life dropping in */
    mff_cycles(0xABEE, 8);
    a = RAM(OBJ_STATE + x);
    if (a == AI_DROP_IN) {
        mff_cycles(0xABF3, 1);
    } else {
        mff_cycles(0xABF5, 7);
        a = RAM(GAME_ACTION);
        if (a == ACT_DROP_IN) {
            mff_cycles(0xABF9, 1);
            SYNC_OUT;
            react_knock_down();
            return;
        }
    }
    mff_cycles(0xABFB, 8);
    c = 0;
    RTS_RETURN();
}

/*
 * enemy_react_held, $AB4F (name proposed): obj_hit_timer 0, enemy_reset,
 * facing the player, C set.
 * Callers: JMP at $AB49; the fall-through from $AB4C (enemy_react).
 */
void func_AB4F_b6(void)
{
    REGS_IN;

    mff_cycles(0xAB4F, 13);
    a = 0x00;
    RAM(OBJ_HIT_TIMER + x) = a;
    CALL_BANK(0xAB54, 0xA8E8);                  /* enemy_reset */
    mff_cycles(0xAB57, 6);
    CALL_FIXED(0xAB57, func_F343);              /* obj_face_player */
    mff_cycles(0xAB5A, 8);
    c = 1;
    RTS_RETURN();
}

/*
 * enemy_react_hit, $AB9F (name proposed): facing the player, phase 8
 * (enemy_hit_stun), C clear: the phase runs this frame.
 * Callers: JMP at $AB99; the branch at $AB94 and the fall-through from
 * $AB9C (enemy_react).
 */
void func_AB9F_b6(void)
{
    REGS_IN;

    mff_cycles(0xAB9F, 6);
    CALL_FIXED(0xAB9F, func_F343);              /* obj_face_player */
    mff_cycles(0xABA2, 15);
    a = AI_HIT_STUN;
    RAM(OBJ_STATE + x) = a;
    c = 0;
    RTS_RETURN();
}

/* the knockdown of $AC0D/$AC13 after obj_land_y: thrown up at $038E (as a
 * heavy hit), id 5 with the id saved, state $80, obj_invuln $FF */
static void knock_down_fields(uint8_t slot)
{
    RAM(OBJ_VY_FRAC + slot) = KNOCK_VY_FRAC;
    RAM(OBJ_VY + slot) = KNOCK_VY;
    RAM(OBJ_SAVED_ID + slot) = RAM(OBJ_ID + slot);
    RAM(OBJ_ID + slot) = OBJ_KNOCKED_DOWN;
    RAM(OBJ_STATE + slot) = OBJ_LIVE;
}

/*
 * $ABFD-$AC32: knocked down by a new life dropping in. obj_land_y = obj_y
 * (in a jump, phase 9, obj_ground_y and JMP $AC13: never executed), then
 * enemy_react_knockdown's fields; C set.
 */
static void react_knock_down(void)
{
    REGS_IN;

    mff_cycles(0xABFD, 8);
    a = RAM(OBJ_STATE + x);
    c = a >= AI_JUMP;
    if (a == AI_JUMP) {
        mff_cycles(0xAC04, 12);
        a = RAM(OBJ_GROUND_Y + x);
        RAM(OBJ_LAND_Y + x) = a;
        GOTO_BANK(0xAC13);
    }
    mff_cycles(0xAC02, 1);
    mff_cycles(0xAC0D, 61);
    RAM(OBJ_LAND_Y + x) = RAM(OBJ_Y + x);
    knock_down_fields(x);
    a = 0xFF;
    RAM(OBJ_INVULN + x) = a;
    c = 1;
    RTS_RETURN();
}

/*
 * enemy_react_knockdown, $AC13 (name proposed): knocked down, obj_land_y
 * already set: obj_vy $038E, id 5 (obj_saved_id keeps the id), state $80,
 * obj_invuln $FF, C set. Never executed as an entry (JMP at $AC0A, from
 * the jump; the bytes are data to the P0 map).
 */
void func_AC13_b6(void)
{
    REGS_IN;

    mff_cycles(0xAC13, 52);
    knock_down_fields(x);
    a = 0xFF;
    RAM(OBJ_INVULN + x) = a;
    c = 1;
    RTS_RETURN();
}
