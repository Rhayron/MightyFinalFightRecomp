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

/* ---- the knee, game_action $21 (B with no direction while holding) ---- */

/* the knee's handler of each character */
#define KNEE_HANDLER_LO     0x8B0F   /* 15 B1 38 */
#define KNEE_HANDLER_HI     0x8B12   /* 8B 8B 8C: $8B15, $8BB1, $8C38 */
#define KNEE_CHR            0x8CBE   /* 0C 16 13 */
#define KNEE_ATTACK_ID      0x8CC1   /* 1B 1C 1D: the first, second and third knee */
#define GRAB_ANIM           0xE213   /* 31 32 33: holding an enemy */
#define GRAB_CHR            0xE25B   /* 0C 14 08 */

/* the held enemy's ids that end the knees: knocked down, dying, pushed back */
static const uint8_t k_knee_stop_ids[] = { 0x05, 0x06, 0x07, 0x02 };

/*
 * player_knee ($8B00 in bank $00, proposed): game_action $21. JMP ($0000) to
 * the character's knee from $8B0F/$8B12[player_id]: cody_knee $8B15,
 * guy_knee $8BB1, haggar_knee $8C38. Reached through player_action_dispatch
 * (entry $21).
 * Out: $00/$01 = the handler, A = its high byte.
 */
void func_8B00_b0(void) {
    REGS_IN;

    mff_cycles(0x8B00, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(KNEE_HANDLER_LO + y));
    mff_cycles(0x8B05, 3 + 4);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(KNEE_HANDLER_HI + y));
    SET_NZ(a);
    mff_cycles(0x8B0A, 3 + 5);
    RAM(TMP1) = a;
    JMP_IND(TMP0);
}

/*
 * cody_knee ($8B15 in bank $00, proposed): up to three knees into the held
 * enemy, counted in grab_hit_count. Off screen (obj_flags bit 7 clear) the
 * enemy is let go (knee_end). The animation $75 with CHR $8CBE[player_id];
 * at timer 0 a knee goes out: attack_id $8CC1[count], obj_hit_timer of the
 * enemy $1C, attack_begin (after the third, knee_end); attack_end at timer
 * 3; at timer $10 the hold goes on (game_action 4, the holding animation
 * and CHR bank, grab_timer $3C, obj_hit_timer 0, obj_state[0] $80, the
 * combo restarted) unless the enemy is down, dying or pushed back
 * (obj_id 5, 6, 7 or 2: knee_end).
 */
void func_8B15_b0(void) {
    unsigned i;
    REGS_IN;

    mff_cycles(0x8B15, 3 + 4 + 2);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_FLAGS + y);
    if (!(a & 0x80)) {
        mff_cycles(0x8B1A, 1);
        FALL_INTO(func_8B98_b0);                /* knee_end */
    }
    mff_cycles(0x8B1C, 3 + 2 + 4 + 2);
    y = RAM(PLAYER_ID);
    a = 0x75;
    COMPARE(a, RAM(OBJ_ANIM));
    if (!z) {
        mff_cycles(0x8B25, 6);
        JSR_FN(0x8B25, func_F294);              /* player_set_anim */
        mff_cycles(0x8B28, 4);
        a = prg_read((uint16_t)(KNEE_CHR + y));
        SET_NZ(a);
        mff_cycles(0x8B2B, 6);
        JSR_FN(0x8B2B, func_F440);              /* player_set_chr */
    } else {
        mff_cycles(0x8B23, 1);
    }
    mff_cycles(0x8B2E, 4 + 2);
    a = RAM(OBJ_ANIM_TIMER);
    if (a == 0) {
        mff_cycles(0x8B33, 3 + 2 + 2);
        y = RAM(GRAB_HIT_COUNT);
        COMPARE(y, 3);
        if (z) {
            mff_cycles(0x8B37, 1);
            FALL_INTO(func_8B98_b0);            /* knee_end */
        }
        mff_cycles(0x8B39, 4);
        a = prg_read((uint16_t)(KNEE_ATTACK_ID + y));
        mff_cycles(0x8B3C, 3 + 3 + 2 + 5 + 6);
        RAM(ATTACK_ID) = a;
        y = RAM(GRAB_SLOT);
        a = 0x1C;
        SET_NZ(a);
        RAM(OBJ_HIT_TIMER + y) = a;
        JSR_FN(0x8B45, func_F43B);              /* attack_begin */
        mff_cycles(0x8B48, 5);
        RAM(GRAB_HIT_COUNT)++;
    } else {
        mff_cycles(0x8B31, 1);
    }
    mff_cycles(0x8B4A, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_TIMER);
    COMPARE(a, 3);
    if (z) {
        mff_cycles(0x8B51, 6);
        JSR_FN(0x8B51, func_F436);              /* attack_end */
    } else {
        mff_cycles(0x8B4F, 1);
    }
    mff_cycles(0x8B54, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_TIMER);
    COMPARE(a, 0x10);
    if (!z) {
        mff_cycles(0x8B59, 1 + 6);
        RTS();
    }
    mff_cycles(0x8B5B, 3 + 4);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_ID + y);
    for (i = 0; i < sizeof k_knee_stop_ids; i++) {
        mff_cycles(0x8B60, 2 + 2);              /* CMP #, BEQ */
        COMPARE(a, k_knee_stop_ids[i]);
        if (z) {
            mff_cycles(0x8B60, 1);
            FALL_INTO(func_8B98_b0);            /* knee_end */
        }
    }
    mff_cycles(0x8B70, 2 + 3 + 3 + 4);          /* back to holding */
    a = ACT_GRAB;
    RAM(GAME_ACTION) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(GRAB_ANIM + y));
    SET_NZ(a);
    mff_cycles(0x8B79, 6);
    JSR_FN(0x8B79, func_F294);                  /* player_set_anim */
    mff_cycles(0x8B7C, 4);
    a = prg_read((uint16_t)(GRAB_CHR + y));
    SET_NZ(a);
    mff_cycles(0x8B7F, 6);
    JSR_FN(0x8B7F, func_F440);                  /* player_set_chr */
    mff_cycles(0x8B82, 4);
    a = prg_read((uint16_t)(COMBO_FIRST_ANIM + y));
    mff_cycles(0x8B85, 3 + 2 + 3 + 3 + 2 + 5 + 2 + 4 + 6);
    RAM(PLAYER_STAND_ANIM) = a;
    a = 0x3C;
    RAM(GRAB_TIMER) = a;
    y = RAM(GRAB_SLOT);
    a = 0;
    RAM(OBJ_HIT_TIMER + y) = a;
    a = STATE_LIVE;
    SET_NZ(a);
    RAM(OBJ_STATE) = a;
    RTS();
}

/* knee_end ($8B98 in bank $00, proposed): the knees are over and the enemy
 * let go of: game_action and grab_hit_count 0, attack_end, obj_state[0]
 * $80, the combo restarted, player_stand. Run on into from cody_knee;
 * branched to from guy_knee and reached by JMP from haggar_knee (00:8CB8). */
void func_8B98_b0(void) {
    REGS_IN;

    mff_cycles(0x8B98, 2 + 3 + 3 + 6);
    a = ACT_IDLE;
    SET_NZ(a);
    RAM(GAME_ACTION) = a;
    RAM(GRAB_HIT_COUNT) = a;
    JSR_FN(0x8B9E, func_F436);                  /* attack_end */
    mff_cycles(0x8BA1, 2 + 4 + 3 + 4);
    a = STATE_LIVE;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(COMBO_FIRST_ANIM + y));
    SET_NZ(a);
    mff_cycles(0x8BAB, 3 + 6);
    RAM(PLAYER_STAND_ANIM) = a;
    JSR_AT(0x8BAD, 0xCEAA, 0);                  /* player_stand */
    mff_cycles(0x8BB0, 6);
    RTS();
}

/* guy_knee ($8BB1 in bank $00, proposed): as cody_knee with the animation
 * $76, knee_end by branches, and the next combo step $0E (Guy's jab) once
 * back to holding. */
void func_8BB1_b0(void) {
    unsigned i;
    REGS_IN;

    mff_cycles(0x8BB1, 3 + 4 + 2);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_FLAGS + y);
    SET_NZ(a);
    if (!n) {
        mff_cycles(0x8BB6, 1);
        BRANCH_TAIL(0x8B98, 0);                 /* knee_end */
    }
    mff_cycles(0x8BB8, 3 + 2 + 4 + 2);
    y = RAM(PLAYER_ID);
    a = 0x76;
    COMPARE(a, RAM(OBJ_ANIM));
    if (!z) {
        mff_cycles(0x8BC1, 6);
        JSR_FN(0x8BC1, func_F294);              /* player_set_anim */
        mff_cycles(0x8BC4, 4);
        a = prg_read((uint16_t)(KNEE_CHR + y));
        SET_NZ(a);
        mff_cycles(0x8BC7, 6);
        JSR_FN(0x8BC7, func_F440);              /* player_set_chr */
    } else {
        mff_cycles(0x8BBF, 1);
    }
    mff_cycles(0x8BCA, 4 + 2);
    a = RAM(OBJ_ANIM_TIMER);
    if (a == 0) {
        mff_cycles(0x8BCF, 3 + 2 + 2);
        y = RAM(GRAB_HIT_COUNT);
        COMPARE(y, 3);
        if (z) {
            mff_cycles(0x8BD3, 1);
            BRANCH_TAIL(0x8B98, 0);             /* knee_end */
        }
        mff_cycles(0x8BD5, 4);
        a = prg_read((uint16_t)(KNEE_ATTACK_ID + y));
        mff_cycles(0x8BD8, 3 + 3 + 2 + 5 + 6);
        RAM(ATTACK_ID) = a;
        y = RAM(GRAB_SLOT);
        a = 0x1C;
        SET_NZ(a);
        RAM(OBJ_HIT_TIMER + y) = a;
        JSR_FN(0x8BE1, func_F43B);              /* attack_begin */
        mff_cycles(0x8BE4, 5);
        RAM(GRAB_HIT_COUNT)++;
    } else {
        mff_cycles(0x8BCD, 1);
    }
    mff_cycles(0x8BE6, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_TIMER);
    COMPARE(a, 3);
    if (z) {
        mff_cycles(0x8BED, 6);
        JSR_FN(0x8BED, func_F436);              /* attack_end */
    } else {
        mff_cycles(0x8BEB, 1);
    }
    mff_cycles(0x8BF0, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_TIMER);
    COMPARE(a, 0x10);
    if (!z) {
        mff_cycles(0x8BF5, 2 + 6);
        RTS();
    }
    mff_cycles(0x8BF7, 3 + 4);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_ID + y);
    for (i = 0; i < sizeof k_knee_stop_ids; i++) {
        mff_cycles(0x8BFC, 2 + 2);              /* CMP #, BEQ */
        COMPARE(a, k_knee_stop_ids[i]);
        if (z) {
            mff_cycles(0x8BFC, 2);              /* BEQ taken, to the page before */
            BRANCH_TAIL(0x8B98, 0);             /* knee_end */
        }
    }
    mff_cycles(0x8C0C, 2 + 3 + 3 + 4);          /* back to holding */
    a = ACT_GRAB;
    RAM(GAME_ACTION) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(GRAB_ANIM + y));
    SET_NZ(a);
    mff_cycles(0x8C15, 6);
    JSR_FN(0x8C15, func_F294);                  /* player_set_anim */
    mff_cycles(0x8C18, 4);
    a = prg_read((uint16_t)(GRAB_CHR + y));
    SET_NZ(a);
    mff_cycles(0x8C1B, 6);
    JSR_FN(0x8C1B, func_F440);                  /* player_set_chr */
    mff_cycles(0x8C1E, 4);
    a = prg_read((uint16_t)(COMBO_FIRST_ANIM + y));
    mff_cycles(0x8C21, 3 + 2 + 3 + 3 + 2 + 5 + 2 + 4 + 2 + 3 + 6);
    RAM(PLAYER_STAND_ANIM) = a;
    a = 0x3C;
    RAM(GRAB_TIMER) = a;
    y = RAM(GRAB_SLOT);
    a = 0;
    RAM(OBJ_HIT_TIMER + y) = a;
    a = STATE_LIVE;
    RAM(OBJ_STATE) = a;
    a = 0x0E;
    SET_NZ(a);
    RAM(PLAYER_STAND_ANIM) = a;
    RTS();
}

/* haggar_knee ($8C38 in bank $00, proposed): as cody_knee with the
 * animation $77, no on-screen test, knee_end by JMP (00:8CB8) and the next
 * combo step $0F (Haggar's jab) once back to holding. */
void func_8C38_b0(void) {
    unsigned i;
    REGS_IN;

    mff_cycles(0x8C38, 3 + 2 + 4 + 2);
    y = RAM(PLAYER_ID);
    a = 0x77;
    COMPARE(a, RAM(OBJ_ANIM));
    if (!z) {
        mff_cycles(0x8C41, 6);
        JSR_FN(0x8C41, func_F294);              /* player_set_anim */
        mff_cycles(0x8C44, 4);
        a = prg_read((uint16_t)(KNEE_CHR + y));
        SET_NZ(a);
        mff_cycles(0x8C47, 6);
        JSR_FN(0x8C47, func_F440);              /* player_set_chr */
    } else {
        mff_cycles(0x8C3F, 1);
    }
    mff_cycles(0x8C4A, 4 + 2);
    a = RAM(OBJ_ANIM_TIMER);
    if (a == 0) {
        mff_cycles(0x8C4F, 3 + 2 + 2);
        y = RAM(GRAB_HIT_COUNT);
        COMPARE(y, 3);
        if (z) {
            mff_cycles(0x8C53, 1 + 3);
            JMP_TAIL(0x8B98, 0);                /* knee_end */
        }
        mff_cycles(0x8C55, 4);
        a = prg_read((uint16_t)(KNEE_ATTACK_ID + y));
        mff_cycles(0x8C58, 3 + 3 + 2 + 5 + 6);
        RAM(ATTACK_ID) = a;
        y = RAM(GRAB_SLOT);
        a = 0x1C;
        SET_NZ(a);
        RAM(OBJ_HIT_TIMER + y) = a;
        JSR_FN(0x8C61, func_F43B);              /* attack_begin */
        mff_cycles(0x8C64, 5);
        RAM(GRAB_HIT_COUNT)++;
    } else {
        mff_cycles(0x8C4D, 1);
    }
    mff_cycles(0x8C66, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_TIMER);
    COMPARE(a, 3);
    if (z) {
        mff_cycles(0x8C6D, 6);
        JSR_FN(0x8C6D, func_F436);              /* attack_end */
    } else {
        mff_cycles(0x8C6B, 1);
    }
    mff_cycles(0x8C70, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_TIMER);
    COMPARE(a, 0x10);
    if (!z) {
        mff_cycles(0x8C75, 1 + 6);
        RTS();
    }
    mff_cycles(0x8C77, 3 + 4);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_ID + y);
    for (i = 0; i < sizeof k_knee_stop_ids; i++) {
        mff_cycles(0x8C7C, 2 + 2);              /* CMP #, BEQ */
        COMPARE(a, k_knee_stop_ids[i]);
        if (z) {
            mff_cycles(0x8C7C, 1 + 3);
            JMP_TAIL(0x8B98, 0);                /* knee_end */
        }
    }
    mff_cycles(0x8C8C, 2 + 3 + 3 + 4);          /* back to holding */
    a = ACT_GRAB;
    RAM(GAME_ACTION) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(GRAB_ANIM + y));
    SET_NZ(a);
    mff_cycles(0x8C95, 6);
    JSR_FN(0x8C95, func_F294);                  /* player_set_anim */
    mff_cycles(0x8C98, 4);
    a = prg_read((uint16_t)(GRAB_CHR + y));
    SET_NZ(a);
    mff_cycles(0x8C9B, 6);
    JSR_FN(0x8C9B, func_F440);                  /* player_set_chr */
    mff_cycles(0x8C9E, 4);
    a = prg_read((uint16_t)(COMBO_FIRST_ANIM + y));
    mff_cycles(0x8CA1, 3 + 2 + 3 + 3 + 2 + 5 + 2 + 4 + 2 + 3 + 6);
    RAM(PLAYER_STAND_ANIM) = a;
    a = 0x3C;
    RAM(GRAB_TIMER) = a;
    y = RAM(GRAB_SLOT);
    a = 0;
    RAM(OBJ_HIT_TIMER + y) = a;
    a = STATE_LIVE;
    RAM(OBJ_STATE) = a;
    a = 0x0F;
    SET_NZ(a);
    RAM(PLAYER_STAND_ANIM) = a;
    RTS();
}
