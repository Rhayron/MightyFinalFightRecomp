/*
 * player_throws.c - the throws that run as bank-$00 actions: Guy's throw
 * (game_action $13), Haggar's back throw ($15) and the knees all three
 * give a held enemy ($21). Cody's throw is player_state_throw's in the
 * fixed bank (player_grab.c, cody_throw_start).
 *
 * Both throws start from a grab with B (guy_throw_start, $D88C;
 * haggar_back_throw_start, $D8A8), and count their steps in obj_state[0]'s
 * low nibble. Without a direction held B gives the knee instead.
 */
#include "player_defs.h"

#define ANIM_GUY_THROW      0x35
#define ANIM_GUY_THROW_END  0x5C
#define OBJ_ID_GUY_THROWN   0x0F     /* obj_id of the body Guy throws */
#define ANIM_HAGGAR_THROW   0x36
#define ANIM_HAGGAR_THROW_2 0x52
#define ANIM_HAGGAR_THROW_3 0x4C
#define OBJ_ID_HAGGAR_THROWN 0x08    /* obj_id of the body Haggar throws */

static void guy_throw_turn(void);
static void guy_throw_wait(void);
static void haggar_throw_turn(void);
static void haggar_throw_wait(void);

/* knee_start ($8497 in bank $00, proposed): game_action $21, the knee.
 * Branched to from guy_throw (00:84A7). Out: A = $21. */
void func_8497_b0(void) {
    REGS_IN;

    mff_cycles(0x8497, 2 + 3 + 6);
    a = ACT_GRAB_KNEE;
    SET_NZ(a);
    RAM(GAME_ACTION) = a;
    RTS();
}

/*
 * guy_throw ($849C in bank $00, proposed; Data Crystal: Guy's throw):
 * game_action $13. On its first step, with no direction held, the knee
 * (knee_start). Forward: obj_throw_flag of the enemy is set and Guy turns
 * round; backward (never executed in any run) sound $33, player_no_hit $80
 * and the enemy turned to face Guy's way. Then guy_throw_turn and, every
 * step, guy_throw_wait.
 *
 * Forward, Y still holds game_action ($13) from player_action_dispatch:
 * the $FF meant for obj_throw_flag[grab_slot] lands in $0470 + $13, slot
 * 3's obj_spawn_index ($0483). Cody's and Haggar's throws load grab_slot
 * first.
 */
void func_849C_b0(void) {
    REGS_IN;

    mff_cycles(0x849C, 4 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x0F;
    if (a != 0) {
        mff_cycles(0x84A1, 2);
        FALL_INTO(guy_throw_wait);
    }
    mff_cycles(0x84A3, 3 + 2 + 2);
    a = RAM(JOY_HELD) & 0x0F;
    SET_NZ(a);
    if (z) {
        mff_cycles(0x84A7, 1);
        BRANCH_TAIL(0x8497, 0);                 /* knee_start */
    }
    mff_cycles(0x84A9, 3 + 2);
    COMPARE(a, RAM(PLAYER_DIR));
    if (!z) {                                   /* backward */
        mff_cycles(0x84AD, 2 + 6);
        a = SFX_THROW;
        SET_NZ(a);
        JSR_FN(0x84AF, func_F34C);              /* sound_queue_push_sfx */
        mff_cycles(0x84B2, 2 + 3 + 3 + 4 + 2 + 3 + 2);
        a = NO_HIT;
        RAM(PLAYER_NO_HIT) = a;
        y = RAM(GRAB_SLOT);
        a = RAM(OBJ_DIR + y) & (PAD_LEFT | PAD_RIGHT);
        COMPARE(a, RAM(PLAYER_DIR));
        if (z) {
            mff_cycles(0x84BF, 1);
            FALL_INTO(guy_throw_turn);
        }
        mff_cycles(0x84C1, 4 + 2 + 5 + 2);
        a = RAM(OBJ_DIR + y) ^ (PAD_LEFT | PAD_RIGHT);
        RAM(OBJ_DIR + y) = a;
        if (a != 0) {
            mff_cycles(0x84C9, 1);
            FALL_INTO(guy_throw_turn);
        }
    } else {
        mff_cycles(0x84AB, 1);
    }
    mff_cycles(0x84CB, 2 + 5 + 3 + 2 + 2);      /* forward */
    a = 0xFF;
    RAM(OBJ_THROW_FLAG + y) = a;
    a = RAM(PLAYER_DIR) & PAD_RIGHT;
    SET_NZ(a);
    if (!z) {                                   /* facing right (never executed) */
        mff_cycles(0x84D6, 6);
        JSR_FN(0x84D6, func_F048);              /* player_face_left */
        mff_cycles(0x84D9, 2 + 2 + 1);
        a = PAD_LEFT;
        SET_NZ(a);
        FALL_INTO(guy_throw_turn);
    }
    mff_cycles(0x84D4, 1 + 6);
    JSR_FN(0x84DD, func_F03F);                  /* player_face_right */
    mff_cycles(0x84E0, 2);
    a = PAD_RIGHT;
    FALL_INTO(guy_throw_turn);
}

/* guy_throw_turn (00:84E2, inside guy_throw): player_dir = A, the throw
 * animation $35; the enemy becomes a thrown body (id $0F, obj_state $80)
 * drawn facing its obj_dir (flip set for left: never executed), and the
 * step counts up. */
static void guy_throw_turn(void) {
    REGS_IN;

    mff_cycles(0x84E2, 3 + 2 + 6);
    RAM(PLAYER_DIR) = a;
    a = ANIM_GUY_THROW;
    SET_NZ(a);
    JSR_FN(0x84E6, func_F294);                  /* player_set_anim */
    mff_cycles(0x84E9, 3 + 2 + 5 + 2 + 5 + 4 + 2 + 2);
    y = RAM(GRAB_SLOT);
    a = OBJ_ID_GUY_THROWN;
    RAM(OBJ_ID + y) = a;
    a = STATE_LIVE;
    RAM(OBJ_STATE + y) = a;
    a = RAM(OBJ_DIR + y) & PAD_RIGHT;
    if (a != 0) {
        mff_cycles(0x84FC, 4 + 2 + 5 + 2);
        a = RAM(OBJ_FLAGS + y) & ~FLAGS_FLIP;
        RAM(OBJ_FLAGS + y) = a;
    } else {
        mff_cycles(0x84FA, 2);
    }
    /* A is 0 when it went left (or, never, when the flags cleared to 0) */
    if (a == 0) {
        mff_cycles(0x8506, 4 + 2 + 5);
        a = RAM(OBJ_FLAGS + y) | FLAGS_FLIP;
        RAM(OBJ_FLAGS + y) = a;
    } else {
        mff_cycles(0x8504, 1);
    }
    mff_cycles(0x850E, 6);
    RAM(OBJ_STATE)++;
    FALL_INTO(guy_throw_wait);
}

/* guy_throw_wait (00:8511, inside guy_throw): on frame $0C of the throw,
 * animation $5C with CHR bank 5 and obj_timer/obj_timer2[0] 0; once in $5C
 * the throw ends: attack_live and combo_timer 0, game_action $0B for 9
 * frames (player_state_throw). */
static void guy_throw_wait(void) {
    REGS_IN;

    mff_cycles(0x8511, 4 + 2 + 2);
    a = RAM(OBJ_ANIM);
    COMPARE(a, ANIM_GUY_THROW_END);
    if (!z) {
        mff_cycles(0x8518, 4 + 2 + 2);
        a = RAM(OBJ_ANIM_FRAME);
        COMPARE(a, 0x0C);
        if (z) {
            mff_cycles(0x851F, 2 + 6);
            a = ANIM_GUY_THROW_END;
            SET_NZ(a);
            JSR_FN(0x8521, func_F294);          /* player_set_anim */
            mff_cycles(0x8524, 2 + 6);
            a = 0x05;
            SET_NZ(a);
            JSR_FN(0x8526, func_F440);          /* player_set_chr */
            mff_cycles(0x8529, 2 + 4 + 4);
            a = 0;
            SET_NZ(a);
            RAM(OBJ_TIMER) = a;
            RAM(OBJ_TIMER2) = a;
        } else {
            mff_cycles(0x851D, 1);
        }
        mff_cycles(0x8531, 6);
        RTS();
    }
    mff_cycles(0x8516, 1 + 2 + 3 + 3 + 2 + 3 + 2 + 3 + 6);
    a = 0;
    RAM(ATTACK_LIVE) = a;
    RAM(COMBO_TIMER) = a;
    a = 9;
    RAM(ACTION_TIMER) = a;
    a = ACT_THROW;
    SET_NZ(a);
    RAM(GAME_ACTION) = a;
    RTS();
}

/* knee_start_haggar ($863E in bank $00, proposed): game_action $21, the
 * knee. Branched to from haggar_back_throw (00:8654). Out: A = $21. */
void func_863E_b0(void) {
    REGS_IN;

    mff_cycles(0x863E, 2 + 3 + 6);
    a = ACT_GRAB_KNEE;
    SET_NZ(a);
    RAM(GAME_ACTION) = a;
    RTS();
}

/*
 * haggar_back_throw ($8643 in bank $00, proposed; Data Crystal: Haggar's
 * back throw): game_action $15. On its first step player_no_hit $80; with
 * no direction held the knee (knee_start_haggar). Backward: the enemy is
 * turned to face Haggar's way unless it does (if its obj_dir turns to 0 the
 * forward throw follows). Forward: obj_throw_flag[grab_slot] $FF and Haggar
 * turns round (player_face_left and player_dir 2; the other way never
 * executed in any run). Then haggar_throw_turn and, every step,
 * haggar_throw_wait.
 */
void func_8643_b0(void) {
    REGS_IN;

    mff_cycles(0x8643, 4 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x0F;
    if (a != 0) {
        mff_cycles(0x8648, 1);
        FALL_INTO(haggar_throw_wait);
    }
    mff_cycles(0x864A, 2 + 3 + 3 + 3 + 2 + 2);
    a = NO_HIT;
    RAM(PLAYER_NO_HIT) = a;
    y = RAM(GRAB_SLOT);
    a = RAM(JOY_HELD) & 0x0F;
    SET_NZ(a);
    if (z) {
        mff_cycles(0x8654, 1);
        BRANCH_TAIL(0x863E, 0);                 /* knee_start_haggar */
    }
    mff_cycles(0x8656, 3 + 2);
    COMPARE(a, RAM(PLAYER_DIR));
    if (!z) {                                   /* backward */
        mff_cycles(0x865A, 4 + 2 + 3 + 2);
        a = RAM(OBJ_DIR + y) & (PAD_LEFT | PAD_RIGHT);
        COMPARE(a, RAM(PLAYER_DIR));
        if (z) {
            mff_cycles(0x8661, 1);
            FALL_INTO(haggar_throw_turn);
        }
        mff_cycles(0x8663, 4 + 2 + 5 + 2);
        a = RAM(OBJ_DIR + y) ^ (PAD_LEFT | PAD_RIGHT);
        RAM(OBJ_DIR + y) = a;
        if (a != 0) {
            mff_cycles(0x866B, 1);
            FALL_INTO(haggar_throw_turn);
        }
    } else {
        mff_cycles(0x8658, 1);
    }
    mff_cycles(0x866D, 2 + 5 + 3 + 2 + 2);      /* forward */
    a = 0xFF;
    RAM(OBJ_THROW_FLAG + y) = a;
    a = RAM(PLAYER_DIR) & PAD_RIGHT;
    SET_NZ(a);
    if (!z) {
        mff_cycles(0x8678, 6);
        JSR_FN(0x8678, func_F048);              /* player_face_left */
        mff_cycles(0x867B, 2 + 2 + 1);
        a = PAD_LEFT;
        SET_NZ(a);
        FALL_INTO(haggar_throw_turn);
    }
    mff_cycles(0x8676, 1 + 6);
    JSR_FN(0x867F, func_F03F);                  /* player_face_right */
    mff_cycles(0x8682, 2);
    a = PAD_RIGHT;
    FALL_INTO(haggar_throw_turn);
}

/* haggar_throw_turn (00:8684, inside haggar_back_throw): player_dir = A,
 * animation $36 with CHR bank $0A, the enemy a thrown body (id 8,
 * obj_state $80) drawn facing its obj_dir, and the step counts up. */
static void haggar_throw_turn(void) {
    REGS_IN;

    mff_cycles(0x8684, 3 + 2 + 6);
    RAM(PLAYER_DIR) = a;
    a = ANIM_HAGGAR_THROW;
    SET_NZ(a);
    JSR_FN(0x8688, func_F294);                  /* player_set_anim */
    mff_cycles(0x868B, 2 + 6);
    a = 0x0A;
    SET_NZ(a);
    JSR_FN(0x868D, func_F440);                  /* player_set_chr */
    mff_cycles(0x8690, 3 + 2 + 5 + 2 + 5 + 4 + 2 + 2);
    y = RAM(GRAB_SLOT);
    a = OBJ_ID_HAGGAR_THROWN;
    RAM(OBJ_ID + y) = a;
    a = STATE_LIVE;
    RAM(OBJ_STATE + y) = a;
    a = RAM(OBJ_DIR + y) & PAD_RIGHT;
    if (a != 0) {
        mff_cycles(0x86A3, 4 + 2 + 5 + 2);
        a = RAM(OBJ_FLAGS + y) & ~FLAGS_FLIP;
        RAM(OBJ_FLAGS + y) = a;
    } else {
        mff_cycles(0x86A1, 1);
    }
    /* A is 0 when it went left (or, never, when the flags cleared to 0) */
    if (a == 0) {
        mff_cycles(0x86AD, 4 + 2 + 5);
        a = RAM(OBJ_FLAGS + y) | FLAGS_FLIP;
        RAM(OBJ_FLAGS + y) = a;
    } else {
        mff_cycles(0x86AB, 1);
    }
    mff_cycles(0x86B5, 6);
    RAM(OBJ_STATE)++;
    FALL_INTO(haggar_throw_wait);
}

/* haggar_throw_wait (00:86B8, inside haggar_back_throw): animation $36
 * turns into $52 (CHR bank $0B) at timer 4, $52 into $4C (CHR $0A) on frame
 * $12 at timer 2, and $4C ends the throw: game_action $0B for 2 frames. */
static void haggar_throw_wait(void) {
    REGS_IN;

    mff_cycles(0x86B8, 4 + 2 + 2);
    a = RAM(OBJ_ANIM);
    if (a == ANIM_HAGGAR_THROW) {
        mff_cycles(0x86BF, 4 + 2 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        COMPARE(a, 4);
        if (z) {
            mff_cycles(0x86C6, 2 + 6);
            a = ANIM_HAGGAR_THROW_2;
            SET_NZ(a);
            JSR_FN(0x86C8, func_F294);          /* player_set_anim */
            mff_cycles(0x86CB, 2 + 6);
            a = 0x0B;
            SET_NZ(a);
            JSR_FN(0x86CD, func_F440);          /* player_set_chr */
        } else {
            mff_cycles(0x86C4, 1);
        }
    } else {
        mff_cycles(0x86BD, 1);
    }
    mff_cycles(0x86D0, 4 + 2 + 2);
    a = RAM(OBJ_ANIM);
    if (a == ANIM_HAGGAR_THROW_2) {
        mff_cycles(0x86D7, 4 + 2 + 2);
        a = RAM(OBJ_ANIM_FRAME);
        if (a == 0x12) {
            mff_cycles(0x86DE, 4 + 2 + 2);
            a = RAM(OBJ_ANIM_TIMER);
            COMPARE(a, 2);
            if (z) {
                mff_cycles(0x86E5, 2 + 6);
                a = ANIM_HAGGAR_THROW_3;
                SET_NZ(a);
                JSR_FN(0x86E7, func_F294);      /* player_set_anim */
                mff_cycles(0x86EA, 2 + 6);
                a = 0x0A;
                SET_NZ(a);
                JSR_FN(0x86EC, func_F440);      /* player_set_chr */
            } else {
                mff_cycles(0x86E3, 1);
            }
        } else {
            mff_cycles(0x86DC, 1);
        }
    } else {
        mff_cycles(0x86D5, 1);
    }
    mff_cycles(0x86EF, 4 + 2 + 2);
    a = RAM(OBJ_ANIM);
    COMPARE(a, ANIM_HAGGAR_THROW_3);
    if (z) {
        mff_cycles(0x86F6, 2 + 3 + 2 + 3);
        a = 2;
        RAM(ACTION_TIMER) = a;
        a = ACT_THROW;
        SET_NZ(a);
        RAM(GAME_ACTION) = a;
    } else {
        mff_cycles(0x86F4, 1);
    }
    mff_cycles(0x86FE, 6);
    RTS();
}
