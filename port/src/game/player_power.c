/*
 * player_power.c - the moves a higher player_level opens (Data Crystal:
 * each character's power), bank $00 actions none of the runs reaches
 * (player_level stays 0 in the bot logs; the oracle's CDL never saw any of
 * this code run). Every path here was compared with the lifted code on
 * random machine states instead.
 *
 * Forward + B (player_attack.c, $D346) starts game_action $FCCE[player_id]:
 * Cody $23, Guy $27, Haggar $10. B in Guy's jump window after a landing
 * (player_states.c, $D2B7) starts $E24C[player_id]: Cody $23, Guy $18.
 * Guy's $18 and $27 go on with $22, a window in which down and then up+B
 * rise into one more attack.
 */
#include "player_defs.h"

#define ACT_POWER_FOLLOW    0x22     /* Guy: the window after $18 and $27 */
#define PAD_UP_B            (PAD_UP | PAD_B)

/* ---- Haggar, game_action $10 ---- */

/* frames of the dash, by player_level */
#define HAGGAR_DASH_FRAMES  0x817A   /* 18 16 1A */
#define ANIM_HAGGAR_WINDUP  0x30
#define ANIM_HAGGAR_DASH    0x3D
/* the dash stops this close to the screen's edges (obj_x - scroll_x) */
#define DASH_RIGHT_EDGE     0xF0
#define DASH_LEFT_EDGE      0x18
/* the dash ends early on slot 15 in this animation */
#define ANIM_DASH_STOPPER   0x7E

/*
 * haggar_power ($80C5 in bank $00, proposed): game_action $10. combo_timer
 * 0. The first step (until the wind-up animation $30 is on): obj_timer[0]
 * $18 and obj_timer[1] = $817A[player_level] (the dash's length), the
 * animation $30 with CHR bank $0B, the next step, obj_vx $0300, and Haggar
 * turned away from player_dir (obj_dir the other way). obj_timer[0]
 * counts down the wind-up; then the dash: animation $3D with CHR bank $0A,
 * sound $2C, attack_id $1A and attack_begin once; each frame a step the
 * other way from obj_dir, i.e. along player_dir ($F000 / $F051). Near the
 * screen's edge, at the end of obj_timer[1], or when slot 15 is live in
 * animation $7E (then obj_timer[1] 0 and attack_end first) it ends:
 * haggar_power_end, a JMP to guy_power_end.
 */
void func_80C5_b0(void) {
    REGS_IN;

    mff_cycles(0x80C5, 2 + 3 + 4 + 2 + 2);
    a = 0;
    RAM(COMBO_TIMER) = a;
    a = RAM(OBJ_STATE) & 0x0F;
    if (a != 0) {
        mff_cycles(0x80CE, 2);                  /* BNE $810E, across a page */
    } else {
        mff_cycles(0x80D0, 2 + 4 + 3 + 4);
        a = 0x18;
        RAM(OBJ_TIMER) = a;
        y = RAM(PLAYER_LEVEL);
        a = prg_read_ix(HAGGAR_DASH_FRAMES, y);
        mff_cycles(0x80DA, 4 + 2 + 4 + 2);
        RAM(OBJ_TIMER + 1) = a;
        a = ANIM_HAGGAR_WINDUP;
        COMPARE(a, RAM(OBJ_ANIM));
        if (z) {
            mff_cycles(0x80E2, 2);              /* BEQ $810E, across a page */
        } else {
            mff_cycles(0x80E4, 6);
            JSR_FN(0x80E4, func_F294);          /* player_set_anim */
            mff_cycles(0x80E7, 2 + 6);
            a = 0x0B;
            SET_NZ(a);
            JSR_FN(0x80E9, func_F440);          /* player_set_chr */
            mff_cycles(0x80EC, 6 + 2 + 4 + 2 + 4 + 3 + 2 + 2);
            RAM(OBJ_STATE)++;
            a = 0;
            RAM(OBJ_VX_FRAC) = a;
            a = 0x03;
            RAM(OBJ_VX) = a;
            a = RAM(PLAYER_DIR) & PAD_RIGHT;
            SET_NZ(a);
            if (!z) {                           /* facing right: turn left */
                mff_cycles(0x80FF, 6);
                JSR_FN(0x80FF, func_F048);      /* player_face_left */
                mff_cycles(0x8102, 2 + 2 + 1);
                a = PAD_LEFT;
                SET_NZ(a);
            } else {
                mff_cycles(0x80FD, 2);          /* BEQ $8106, across a page */
                mff_cycles(0x8106, 6);
                JSR_FN(0x8106, func_F03F);      /* player_face_right */
                mff_cycles(0x8109, 2);
                a = PAD_RIGHT;
            }
            mff_cycles(0x810B, 4);
            RAM(OBJ_DIR) = a;
        }
    }
    mff_cycles(0x810E, 4 + 2);
    a = RAM(OBJ_TIMER);
    if (a != 0) {                               /* the wind-up */
        mff_cycles(0x8113, 6 + 6);
        RAM(OBJ_TIMER)--;
        SET_NZ(RAM(OBJ_TIMER));
        RTS();
    }
    mff_cycles(0x8111, 1 + 2 + 4 + 2);
    a = ANIM_HAGGAR_DASH;
    COMPARE(a, RAM(OBJ_ANIM));
    if (!z) {                                   /* the dash starts */
        mff_cycles(0x811E, 6);
        JSR_FN(0x811E, func_F294);              /* player_set_anim */
        mff_cycles(0x8121, 2 + 6);
        a = 0x0A;
        SET_NZ(a);
        JSR_FN(0x8123, func_F440);              /* player_set_chr */
        mff_cycles(0x8126, 2 + 6);
        a = 0x2C;
        SET_NZ(a);
        JSR_FN(0x8128, func_F34C);              /* sound_queue_push_sfx */
        mff_cycles(0x812B, 2 + 3 + 6);
        a = 0x1A;
        SET_NZ(a);
        RAM(ATTACK_ID) = a;
        JSR_FN(0x812F, func_F43B);              /* attack_begin */
    } else {
        mff_cycles(0x811C, 1);
    }
    mff_cycles(0x8132, 4 + 2 + 2);
    a = RAM(OBJ_DIR) & PAD_RIGHT;
    if (a == 0) {                               /* turned left: dash right */
        mff_cycles(0x8139, 2 + 6);
        y = 0;
        SET_NZ(y);
        JSR_FN(0x813B, func_F000);              /* face right, step right */
        mff_cycles(0x813E, 4 + 2 + 3 + 2 + 2);
        a = RAM(OBJ_X);
        c = 1;
        SBC(RAM(SCROLL_X));
        COMPARE(a, DASH_RIGHT_EDGE);
        if (c) {
            mff_cycles(0x8148, 3);
            JMP_TAIL(0x8177, 0);                /* haggar_power_end */
        }
        mff_cycles(0x8146, 1);
    } else {                                    /* dash left */
        mff_cycles(0x8137, 1 + 2 + 6);
        y = 1;
        SET_NZ(y);
        JSR_FN(0x814D, func_F051);              /* face left, step left */
        mff_cycles(0x8150, 4 + 2 + 3 + 2 + 2);
        a = RAM(OBJ_X);
        c = 1;
        SBC(RAM(SCROLL_X));
        COMPARE(a, DASH_LEFT_EDGE);
        if (!c) {
            mff_cycles(0x8158, 1);
            FALL_INTO(func_8177_b0);
        }
    }
    mff_cycles(0x815A, 4 + 2);
    a = RAM(OBJ_TIMER + 1);
    SET_NZ(a);
    if (z) {
        mff_cycles(0x815D, 1);
        FALL_INTO(func_8177_b0);
    }
    mff_cycles(0x815F, 6 + 4 + 2);
    RAM(OBJ_TIMER + 1)--;
    a = RAM(OBJ_STATE + 15);
    SET_NZ(a);
    if (n) {
        mff_cycles(0x8167, 4 + 2 + 2);
        a = RAM(OBJ_ANIM + 15);
        COMPARE(a, ANIM_DASH_STOPPER);
        if (z) {
            mff_cycles(0x816E, 2 + 4 + 6);
            a = 0;
            SET_NZ(a);
            RAM(OBJ_TIMER + 1) = a;
            JSR_FN(0x8173, func_F436);          /* attack_end */
        } else {
            mff_cycles(0x816C, 1);
        }
    } else {
        mff_cycles(0x8165, 1);
    }
    mff_cycles(0x8176, 6);
    RTS();
}

/* haggar_power_end ($8177 in bank $00, proposed): JMP guy_power_end.
 * Reached by JMP from haggar_power (00:8148) and its branches (00:8158,
 * 815D). */
void func_8177_b0(void) {
    REGS_IN;

    mff_cycles(0x8177, 3);
    JMP_TAIL(0x8F82, 0);                        /* guy_power_end */
}

/* ---- Guy, game_action $18: B in his jump window ---- */

#define ANIM_GUY_HOP        0x1A
#define ANIM_GUY_FLY        0x69
#define ANIM_GUY_FALL       0x38
#define ANIM_GUY_LANDED     0x2F
#define GUY_FLY_FRAMES      7

static void guy_hop_drop(void);

/*
 * guy_hop ($891B in bank $00, proposed): game_action $18. The first step:
 * a hop (obj_vy $0300 up, obj_vx $00E6), animation $1A with CHR bank 6,
 * obj_timer[0] 7 and obj_timer2[0] = obj_y (where he took off). While
 * rising (obj_fall, $F1FD), at the top (obj_vy 0): attack_id $18 with
 * attack_begin, attack_ground_y, sound $34, animation $69 and a dash
 * (obj_vx $0500). Each frame he moves along player_dir (guy_hop_on), and
 * in $69 obj_timer[0] counts down; then the next step: obj_vx $0300,
 * obj_vy -$40/256, and guy_hop_drop.
 */
void func_891B_b0(void) {
    REGS_IN;

    mff_cycles(0x891B, 4 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x0F;
    if (a == 0) {
        mff_cycles(0x8922, 4 + 4 + 2 + 4 + 2 + 4 + 2 + 6);
        RAM(OBJ_VY_FRAC) = a;
        RAM(OBJ_VX) = a;
        a = 0x03;
        RAM(OBJ_VY) = a;
        a = 0xE6;
        RAM(OBJ_VX_FRAC) = a;
        a = ANIM_GUY_HOP;
        SET_NZ(a);
        JSR_FN(0x8934, func_F294);              /* player_set_anim */
        mff_cycles(0x8937, 2 + 6);
        a = 0x06;
        SET_NZ(a);
        JSR_FN(0x8939, func_F440);              /* player_set_chr */
        mff_cycles(0x893C, 2 + 4 + 4 + 4 + 6);
        a = GUY_FLY_FRAMES;
        RAM(OBJ_TIMER) = a;
        a = RAM(OBJ_Y);
        RAM(OBJ_TIMER2) = a;
        RAM(OBJ_STATE)++;
    } else {
        mff_cycles(0x8920, 1);
    }
    mff_cycles(0x894A, 4 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x02;
    if (a != 0) {
        mff_cycles(0x894F, 1);
        FALL_INTO(guy_hop_drop);
    }
    mff_cycles(0x8951, 4 + 2 + 2);
    a = RAM(OBJ_ANIM);
    COMPARE(a, ANIM_GUY_FLY);
    if (!z) {                                   /* rising */
        mff_cycles(0x8958, 6);
        JSR_FN(0x8958, func_F1FD);              /* obj_fall, no probe */
        mff_cycles(0x895B, 4 + 2);
        a = RAM(OBJ_VY);
        if (a == 0) {                           /* the top: the dash */
            mff_cycles(0x8960, 2 + 3 + 6);
            a = 0x18;
            SET_NZ(a);
            RAM(ATTACK_ID) = a;
            JSR_FN(0x8964, func_F43B);          /* attack_begin */
            mff_cycles(0x8967, 3 + 3 + 2 + 6);
            a = RAM(PLAYER_GROUND_Y);
            RAM(ATTACK_GROUND_Y) = a;
            a = 0x34;
            SET_NZ(a);
            JSR_FN(0x896D, func_F34C);          /* sound_queue_push_sfx */
            mff_cycles(0x8970, 2 + 6);
            a = ANIM_GUY_FLY;
            SET_NZ(a);
            JSR_FN(0x8972, func_F294);          /* player_set_anim */
            mff_cycles(0x8975, 2 + 4 + 2 + 4);
            a = 0;
            RAM(OBJ_VX_FRAC) = a;
            a = 0x05;
            RAM(OBJ_VX) = a;
        } else {
            mff_cycles(0x895E, 1);
        }
    } else {
        mff_cycles(0x8956, 1);
    }
    mff_cycles(0x897F, 3 + 2 + 2);
    a = RAM(PLAYER_DIR) & PAD_RIGHT;
    if (a != 0) {
        mff_cycles(0x8985, 2 + 6);
        y = 0;
        SET_NZ(y);
        JSR_FN(0x8987, func_F000);              /* face right, step right */
        mff_cycles(0x898A, 3);
        JMP_TAIL(0x8992, 0);                    /* guy_hop_fly */
    }
    mff_cycles(0x8983, 1 + 2 + 6);
    y = 1;
    SET_NZ(y);
    JSR_FN(0x898F, func_F051);                  /* face left, step left */
    FALL_INTO(func_8992_b0);
}

/* guy_hop_fly ($8992 in bank $00, proposed): in animation $69, obj_timer[0]
 * counts down; at 0 the next step with obj_vx $0300 and obj_vy -$40/256,
 * then guy_hop_drop. Reached by JMP from guy_hop (00:898A) and run on into
 * from it. */
void func_8992_b0(void) {
    REGS_IN;

    mff_cycles(0x8992, 4 + 2 + 2);
    a = RAM(OBJ_ANIM);
    COMPARE(a, ANIM_GUY_FLY);
    if (!z) {
        mff_cycles(0x8997, 1 + 6);
        RTS();
    }
    mff_cycles(0x8999, 4 + 2);
    a = RAM(OBJ_TIMER);
    if (a != 0) {
        mff_cycles(0x899E, 6 + 6);
        RAM(OBJ_TIMER)--;
        SET_NZ(RAM(OBJ_TIMER));
        RTS();
    }
    mff_cycles(0x899C, 1 + 6 + 2 + 4 + 2 + 4 + 2 + 4 + 2 + 4);
    RAM(OBJ_STATE)++;
    a = 0;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x03;
    RAM(OBJ_VX) = a;
    a = 0xC0;
    RAM(OBJ_VY_FRAC) = a;
    a = 0xFF;
    RAM(OBJ_VY) = a;
    FALL_INTO(guy_hop_drop);
}

/* guy_hop_drop (00:89B9, inside guy_hop): below the take-off y + 2 he lands
 * (guy_hop_land). Above it he falls (obj_fall, Y = 0; its floor lands him
 * too), the falling animation $38 with CHR bank 5 at obj_vy $FD, and moves
 * along player_dir. */
static void guy_hop_drop(void) {
    REGS_IN;

    mff_cycles(0x89B9, 4 + 2 + 2 + 4 + 2);
    a = RAM(OBJ_TIMER2);
    c = 0;
    m = 0x02;                                   /* ADC: N, Z and C are recomputed */
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    COMPARE(a, RAM(OBJ_Y));
    if (!c) {
        mff_cycles(0x89C4, 2);                  /* BEQ never taken (C clear) */
        if (z) {
            mff_cycles(0x89C4, 1);
            FALL_INTO(func_89F6_b0);
        }
        mff_cycles(0x89C6, 3);
        JMP_TAIL(0x89F6, 0);                    /* guy_hop_land */
    }
    mff_cycles(0x89C2, 1 + 2 + 6);
    y = 0;
    SET_NZ(y);
    JSR_FN(0x89CB, func_F119);                  /* obj_fall */
    mff_cycles(0x89CE, 2);
    if (c) {                                    /* the floor */
        mff_cycles(0x89D0, 3);
        JMP_TAIL(0x89F6, 0);                    /* guy_hop_land */
    }
    mff_cycles(0x89CE, 1 + 4 + 2 + 2);
    a = RAM(OBJ_VY);
    c = a >= 0xFD;
    if (a == 0xFD) {
        mff_cycles(0x89DA, 2 + 6);
        a = ANIM_GUY_FALL;
        SET_NZ(a);
        JSR_FN(0x89DC, func_F294);              /* player_set_anim */
        mff_cycles(0x89DF, 2 + 6);
        a = 0x05;
        SET_NZ(a);
        JSR_FN(0x89E1, func_F440);              /* player_set_chr */
    } else {
        mff_cycles(0x89D8, 1);
    }
    mff_cycles(0x89E4, 3 + 2 + 2);
    a = RAM(PLAYER_DIR) & PAD_RIGHT;
    if (a != 0) {
        mff_cycles(0x89EA, 2 + 6);
        y = 0;
        SET_NZ(y);
        JSR_FN(0x89EC, func_F000);              /* face right, step right */
        mff_cycles(0x89EF, 6);
        RTS();
    }
    mff_cycles(0x89E8, 1 + 2 + 6);
    y = 1;
    SET_NZ(y);
    JSR_FN(0x89F2, func_F051);                  /* face left, step left */
    mff_cycles(0x89F5, 6);
    RTS();
}

/*
 * guy_hop_land ($89F6 in bank $00, proposed): back at the take-off y, the
 * landing animation $2F with CHR bank 4, a small rebound (obj_vy -$40/256,
 * obj_vx $0133), guy_jump_window and player_no_hit 0, attack_end, and
 * game_action $22 for $1E frames with obj_state[0] $80. Reached by JMP
 * from guy_hop_drop (00:89C6, 89D0).
 */
void func_89F6_b0(void) {
    REGS_IN;

    mff_cycles(0x89F6, 4 + 4 + 2 + 6);
    a = RAM(OBJ_TIMER2);
    RAM(OBJ_Y) = a;
    a = ANIM_GUY_LANDED;
    SET_NZ(a);
    JSR_FN(0x89FE, func_F294);                  /* player_set_anim */
    mff_cycles(0x8A01, 2 + 6);
    a = 0x04;
    SET_NZ(a);
    JSR_FN(0x8A03, func_F440);                  /* player_set_chr */
    mff_cycles(0x8A06, 2 + 4 + 2 + 4 + 2 + 4 + 2 + 4 + 2 + 3 + 3 + 6);
    a = 0xC0;
    RAM(OBJ_VY_FRAC) = a;
    a = 0xFF;
    RAM(OBJ_VY) = a;
    a = 0x33;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VX) = a;
    a = 0;
    SET_NZ(a);
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(PLAYER_NO_HIT) = a;
    JSR_FN(0x8A20, func_F436);                  /* attack_end */
    mff_cycles(0x8A23, 2 + 3 + 2 + 3 + 2 + 4 + 6);
    a = ACT_POWER_FOLLOW;
    RAM(GAME_ACTION) = a;
    a = 0x1E;
    RAM(ACTION_TIMER) = a;
    a = STATE_LIVE;
    SET_NZ(a);
    RAM(OBJ_STATE) = a;
    RTS();
}

/* ---- Guy, game_action $22: after $18 and $27 ---- */

#define ANIM_GUY_RISE       0x14
#define ANIM_GUY_RISE_TOP   0x5B
#define FOLLOW_DOWN_COUNT   0x10     /* frames of down held that open up+B */
#define RISE_HEIGHT         0x20

static void guy_follow_rise(void);

/*
 * guy_follow ($8A31 in bank $00, proposed): game_action $22. While
 * action_timer counts down, every frame with down held counts in
 * guy_jump_window; from $10 of them up+B rises (the next step,
 * guy_follow_up). When action_timer is out: guy_follow_end. The steps
 * after: guy_follow_rise.
 */
void func_8A31_b0(void) {
    REGS_IN;

    mff_cycles(0x8A31, 4 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x0F;
    if (a != 0) {
        mff_cycles(0x8A36, 1);
        FALL_INTO(guy_follow_rise);
    }
    mff_cycles(0x8A38, 3 + 2);
    a = RAM(ACTION_TIMER);
    SET_NZ(a);
    if (z) {
        mff_cycles(0x8A3A, 1 + 3);
        JMP_TAIL(0x8AD3, 0);                    /* guy_follow_end */
    }
    mff_cycles(0x8A3C, 5 + 3 + 2 + 2);
    RAM(ACTION_TIMER)--;
    a = RAM(JOY_HELD) & PAD_DOWN;
    if (a != 0) {
        mff_cycles(0x8A44, 5);
        RAM(GUY_JUMP_WINDOW)++;
    } else {
        mff_cycles(0x8A42, 1);
    }
    mff_cycles(0x8A46, 3 + 2 + 2);
    a = RAM(GUY_JUMP_WINDOW);
    COMPARE(a, FOLLOW_DOWN_COUNT);
    if (c) {
        mff_cycles(0x8A4C, 3 + 2 + 2);
        a = RAM(JOY_HELD);
        COMPARE(a, PAD_UP_B);
        if (z) {
            mff_cycles(0x8A52, 6);
            RAM(OBJ_STATE)++;
            FALL_INTO(func_8A55_b0);
        }
        mff_cycles(0x8A50, 1);
    } else {
        mff_cycles(0x8A4A, 1);
    }
    mff_cycles(0x8A6D, 6);
    RTS();
}

/* guy_follow_up ($8A55 in bank $00, proposed): animation $14 with CHR bank
 * 7, rising at obj_vy $0133, player_no_hit $FF. Run on into from
 * guy_follow, reached by JMP from guy_power (00:8F7F). */
void func_8A55_b0(void) {
    REGS_IN;

    mff_cycles(0x8A55, 2 + 6);
    a = ANIM_GUY_RISE;
    SET_NZ(a);
    JSR_FN(0x8A57, func_F294);                  /* player_set_anim */
    mff_cycles(0x8A5A, 2 + 6);
    a = 0x07;
    SET_NZ(a);
    JSR_FN(0x8A5C, func_F440);                  /* player_set_chr */
    mff_cycles(0x8A5F, 2 + 4 + 2 + 4 + 2 + 3 + 6);
    a = 0x33;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VY) = a;
    a = 0xFF;
    SET_NZ(a);
    RAM(PLAYER_NO_HIT) = a;
    RTS();
}

/* guy_follow_rise (00:8A71, inside guy_follow): the first of these steps
 * swings on frame 4 at timer 0 (attack_begin, attack_id 1, the swing
 * sound) and on frame 6 ends the attack: animation $5B with CHR bank 5,
 * player_ground_y = obj_y, obj_y 32 px up, the next step. Then obj_fall
 * (Y = 0) until, falling, obj_y + 2 reaches player_ground_y: put on it
 * (obj_y + 2 when equal, + 1 when past) and guy_follow_end. */
static void guy_follow_rise(void) {
    REGS_IN;

    mff_cycles(0x8A71, 4 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x02;
    if (a == 0) {
        mff_cycles(0x8A78, 4 + 2 + 2);
        a = RAM(OBJ_ANIM_FRAME);
        c = a >= 4;
        if (a == 4) {
            mff_cycles(0x8A7F, 4 + 2);
            a = RAM(OBJ_ANIM_TIMER);
            SET_NZ(a);
            if (z) {
                mff_cycles(0x8A84, 6);
                JSR_FN(0x8A84, func_F43B);      /* attack_begin */
                mff_cycles(0x8A87, 2 + 3 + 6);
                a = 0x01;
                SET_NZ(a);
                RAM(ATTACK_ID) = a;
                JSR_FN(0x8A8B, func_F430);      /* attack_swing_sound */
            } else {
                mff_cycles(0x8A82, 1);
            }
        } else {
            mff_cycles(0x8A7D, 1);
        }
        mff_cycles(0x8A8E, 4 + 2 + 2);
        a = RAM(OBJ_ANIM_FRAME);
        COMPARE(a, 6);
        if (!z) {
            mff_cycles(0x8A93, 1 + 6);          /* BNE $8A6D: guy_follow's RTS */
            RTS();
        }
        mff_cycles(0x8A95, 6);
        JSR_FN(0x8A95, func_F436);              /* attack_end */
        mff_cycles(0x8A98, 2 + 6);
        a = ANIM_GUY_RISE_TOP;
        SET_NZ(a);
        JSR_FN(0x8A9A, func_F294);              /* player_set_anim */
        mff_cycles(0x8A9D, 2 + 6);
        a = 0x05;
        SET_NZ(a);
        JSR_FN(0x8A9F, func_F440);              /* player_set_chr */
        mff_cycles(0x8AA2, 4 + 3 + 4 + 2 + 2 + 4 + 6);
        a = RAM(OBJ_Y);
        RAM(PLAYER_GROUND_Y) = a;
        a = RAM(OBJ_Y);
        c = 1;
        SBC(RISE_HEIGHT);
        RAM(OBJ_Y) = a;
        RAM(OBJ_STATE)++;
    } else {
        mff_cycles(0x8A76, 1);
    }
    mff_cycles(0x8AB3, 2 + 6);
    y = 0;
    SET_NZ(y);
    JSR_FN(0x8AB5, func_F119);                  /* obj_fall */
    mff_cycles(0x8AB8, 4 + 2);
    a = RAM(OBJ_VY);
    SET_NZ(a);
    if (n) {                                    /* falling */
        mff_cycles(0x8ABD, 4 + 2 + 2 + 3 + 2);
        a = RAM(OBJ_Y);
        c = 0;
        m = 0x02;                               /* ADC: N, Z and C are recomputed */
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        COMPARE(a, RAM(PLAYER_GROUND_Y));
        if (c) {                                /* down to the ground line */
            mff_cycles(0x8AC7, 2);
            if (z) {
                mff_cycles(0x8AC7, 1 + 4);
                RAM(OBJ_Y) = a;
            } else {
                mff_cycles(0x8AC9, 2 + 4 + 2);
                SBC(1);
                RAM(OBJ_Y) = a;
                if (a == 0) {                   /* never: BNE taken */
                    mff_cycles(0x8AD0, 4);
                    RAM(OBJ_Y) = a;
                } else {
                    mff_cycles(0x8ACE, 1);
                }
            }
            FALL_INTO(func_8AD3_b0);
        }
        mff_cycles(0x8AC5, 1);
    } else {
        mff_cycles(0x8ABB, 1);
    }
    mff_cycles(0x8AFF, 6);
    RTS();
}

/*
 * guy_follow_end ($8AD3 in bank $00, proposed): a small rebound (obj_vy
 * -$40/256, obj_vx $0180), back to idle (game_action, jump_moving,
 * combo_timer, attack_live, player_invuln and guy_jump_window 0),
 * player_stand and the combo restarted. Reached by JMP from guy_follow
 * (00:8A6E) and run on into from guy_follow_rise.
 */
void func_8AD3_b0(void) {
    REGS_IN;

    mff_cycles(0x8AD3, 2 + 4 + 2 + 4 + 2 + 4 + 2 + 4 + 2 + 3 * 6 + 6);
    a = 0xC0;
    RAM(OBJ_VY_FRAC) = a;
    a = 0xFF;
    RAM(OBJ_VY) = a;
    a = 0x80;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VX) = a;
    a = 0;
    SET_NZ(a);
    RAM(GAME_ACTION) = a;
    RAM(JUMP_MOVING) = a;
    RAM(COMBO_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(PLAYER_INVULN) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    JSR_AT(0x8AF5, 0xCEAA, 0);                  /* player_stand */
    mff_cycles(0x8AF8, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read_ix(COMBO_FIRST_ANIM, y);
    SET_NZ(a);
    mff_cycles(0x8AFD, 3 + 6);
    RAM(PLAYER_STAND_ANIM) = a;
    RTS();
}

/* ---- Cody, game_action $23 ---- */

#define ANIM_CODY_POWER     0x73
#define ANIM_CODY_SLASH     0x13
#define CODY_POWER_THROW_AT 0x10     /* the frame the blade goes */
#define CODY_POWER_LAST     0x1B
/* the blade's x offset from Cody, by player_dir & 2 */
#define CODY_BLADE_DX       0x8D89   /* $000C right, $FFF4 left */
#define ANIM_CODY_BLADE     0x74
#define OBJ_ID_THROWN       0x6C     /* as cody_weapon's knife */

/*
 * cody_power ($8CC4 in bank $00, proposed): game_action $23. combo_timer
 * 0, attack_end, the animation $73 (CHR bank $3F, sound $16) once. Before
 * its frame 5, up+B turns it into the slash: animation $13 with CHR bank
 * 3, game_action $14 (cody_slash, player_specials.c) with action_timer 0,
 * obj_state[0] $80, player_no_hit $80, player_ground_y = obj_y. On frame
 * $10 at timer 0 a blade flies from the first free slot of 2 and 1 (none:
 * nothing): obj_dir = player_dir, 12 px ahead and 4 px below Cody,
 * obj_spawn ($F2A0) with animation $74, id $6C (also obj_saved_id),
 * attack_id $22, obj_attack 1, obj_throw_flag 0. On frame $1B: back to idle
 * (game_action, guy_jump_window, jump_moving 0, the combo restarted,
 * player_stand).
 */
void func_8CC4_b0(void) {
    REGS_IN;

    mff_cycles(0x8CC4, 2 + 3 + 6);
    a = 0;
    SET_NZ(a);
    RAM(COMBO_TIMER) = a;
    JSR_FN(0x8CC8, func_F436);                  /* attack_end */
    mff_cycles(0x8CCB, 2 + 4 + 2);
    a = ANIM_CODY_POWER;
    COMPARE(a, RAM(OBJ_ANIM));
    if (!z) {
        mff_cycles(0x8CD2, 6);
        JSR_FN(0x8CD2, func_F294);              /* player_set_anim */
        mff_cycles(0x8CD5, 2 + 6);
        a = 0x3F;
        SET_NZ(a);
        JSR_FN(0x8CD7, func_F440);              /* player_set_chr */
        mff_cycles(0x8CDA, 2 + 6);
        a = 0x16;
        SET_NZ(a);
        JSR_FN(0x8CDC, func_F34C);              /* sound_queue_push_sfx */
    } else {
        mff_cycles(0x8CD0, 1);
    }
    mff_cycles(0x8CDF, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    if (a < 5) {
        mff_cycles(0x8CE6, 3 + 2 + 2);
        a = RAM(JOY_HELD);
        c = a >= PAD_UP_B;
        if (a == PAD_UP_B) {                    /* the slash */
            mff_cycles(0x8CEC, 2 + 6);
            a = ANIM_CODY_SLASH;
            SET_NZ(a);
            JSR_FN(0x8CEE, func_F294);          /* player_set_anim */
            mff_cycles(0x8CF1, 2 + 6);
            a = 0x03;
            SET_NZ(a);
            JSR_FN(0x8CF3, func_F440);          /* player_set_chr */
            mff_cycles(0x8CF6, 2 + 3 + 2 + 3 + 2 + 4 + 3 + 4 + 3 + 6);
            a = ACT_CODY_SLASH;
            RAM(GAME_ACTION) = a;
            a = 0;
            RAM(ACTION_TIMER) = a;
            a = STATE_LIVE;
            RAM(OBJ_STATE) = a;
            RAM(PLAYER_NO_HIT) = a;
            a = RAM(OBJ_Y);
            SET_NZ(a);
            RAM(PLAYER_GROUND_Y) = a;
            RTS();
        }
        mff_cycles(0x8CEA, 2);                  /* BNE $8D0B, across a page */
    } else {
        mff_cycles(0x8CE4, 2);                  /* BCS $8D0B, across a page */
    }
    mff_cycles(0x8D0B, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    if (a == CODY_POWER_THROW_AT) {
        mff_cycles(0x8D12, 4 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        if (a == 0) {
            mff_cycles(0x8D17, 2);
            y = 2;
            for (;;) {                          /* a free slot, 2 then 1 */
                mff_cycles(0x8D19, 4 + 2);
                a = RAM(OBJ_STATE + y);
                if (!(a & 0x80))
                    break;
                mff_cycles(0x8D1E, 2 + 2 + 2);
                y--;
                COMPARE(y, 0);
                if (z) {
                    mff_cycles(0x8D23, 6);
                    RTS();
                }
                mff_cycles(0x8D21, 1);
            }
            mff_cycles(0x8D1C, 1 + 3 + 3 + 5 + 2 + 2 + 4 + 2 + 4);
            RAM(TMP15) = y;
            a = RAM(PLAYER_DIR);
            RAM(OBJ_DIR + y) = a;
            a &= PAD_LEFT;
            y = a;
            a = RAM(OBJ_X);
            c = 0;
            m = prg_read_ix(CODY_BLADE_DX, y);
            w = a + m + c;                      /* ADC: only C is read */
            c = w > 0xFF;
            a = (uint8_t)w;
            mff_cycles(0x8D35, 3 + 4 + 4);
            mff_push(a);
            a = RAM(OBJ_X_HI);
            m = prg_read_ix(CODY_BLADE_DX + 1, y);
            w = a + m + c;
            a = (uint8_t)w;
            mff_cycles(0x8D3C, 3 + 5 + 4 + 5 + 4 + 2 + 2 + 5 + 4 + 5 + 2 + 6);
            y = RAM(TMP15);
            RAM(OBJ_X_HI + y) = a;
            a = mff_pull();
            RAM(OBJ_X + y) = a;
            a = RAM(OBJ_Y);
            c = 0;
            m = 0x04;                           /* ADC: N and Z are recomputed */
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            RAM(OBJ_Y + y) = a;
            a = RAM(OBJ_Y_HI);
            RAM(OBJ_Y_HI + y) = a;
            a = ANIM_CODY_BLADE;
            SET_NZ(a);
            JSR_FN(0x8D56, func_F2A0);          /* obj_spawn: slot Y, animation A */
            mff_cycles(0x8D59, 2 + 5 + 5 + 2 + 3 + 2 + 5 + 2 + 5);
            a = OBJ_ID_THROWN;
            RAM(OBJ_ID + y) = a;
            RAM(OBJ_SAVED_ID + y) = a;
            a = 0x22;
            RAM(ATTACK_ID) = a;
            a = 0x01;
            RAM(OBJ_ATTACK + y) = a;
            a = 0;
            RAM(OBJ_THROW_FLAG + y) = a;
        } else {
            mff_cycles(0x8D15, 1);
        }
    } else {
        mff_cycles(0x8D10, 1);
    }
    mff_cycles(0x8D6F, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, CODY_POWER_LAST);
    if (z) {
        mff_cycles(0x8D76, 2 + 3 + 3 + 3 + 3 + 4);
        a = 0;
        RAM(GAME_ACTION) = a;
        RAM(GUY_JUMP_WINDOW) = a;
        RAM(JUMP_MOVING) = a;
        y = RAM(PLAYER_ID);
        a = prg_read_ix(COMBO_FIRST_ANIM, y);
        SET_NZ(a);
        mff_cycles(0x8D83, 3 + 6);
        RAM(PLAYER_STAND_ANIM) = a;
        JSR_AT(0x8D85, 0xCEAA, 0);              /* player_stand */
    } else {
        mff_cycles(0x8D74, 1);
    }
    mff_cycles(0x8D88, 6);
    RTS();
}

/* ---- Guy, game_action $27 ---- */

#define ANIM_GUY_POWER      0x24
#define GUY_POWER_UP_COUNT  0x28     /* frames of up held that let it rise on */

/*
 * guy_power ($8EEA in bank $00, proposed): game_action $27. combo_timer 0.
 * The first step: guy_jump_window 0, the animation $24 (CHR bank $32, the
 * next step, sound $1B) once. Each frame up held alone counts in
 * guy_jump_window. Frame 4: attack_id 6 and attack_begin; frame 6:
 * attack_end; frame $0B: attack_id 3, attack_begin, attack_ground_y =
 * obj_y, sound $34; frame $11: attack_end. Frame $13 ends it: with up held
 * $28 frames or more, obj_state[0] and player_no_hit $81, Guy turned
 * around (player_dir too), attack_end and game_action $22, rising at once
 * (JMP guy_follow_up); otherwise guy_power_end.
 */
void func_8EEA_b0(void) {
    REGS_IN;

    mff_cycles(0x8EEA, 2 + 3 + 4 + 2 + 2);
    a = 0;
    RAM(COMBO_TIMER) = a;
    a = RAM(OBJ_STATE) & 0x0F;
    if (a == 0) {
        mff_cycles(0x8EF5, 3 + 2 + 4 + 2);
        RAM(GUY_JUMP_WINDOW) = a;
        a = ANIM_GUY_POWER;
        COMPARE(a, RAM(OBJ_ANIM));
        if (!z) {
            mff_cycles(0x8EFE, 6);
            JSR_FN(0x8EFE, func_F294);          /* player_set_anim */
            mff_cycles(0x8F01, 2 + 6);
            a = 0x32;
            SET_NZ(a);
            JSR_FN(0x8F03, func_F440);          /* player_set_chr */
            mff_cycles(0x8F06, 6 + 2 + 6);
            RAM(OBJ_STATE)++;
            a = 0x1B;
            SET_NZ(a);
            JSR_FN(0x8F0B, func_F34C);          /* sound_queue_push_sfx */
        } else {
            mff_cycles(0x8EFC, 2);              /* BEQ $8F0E, across a page */
        }
    } else {
        mff_cycles(0x8EF3, 2);                  /* BNE $8F0E, across a page */
    }
    mff_cycles(0x8F0E, 3 + 2 + 2);
    a = RAM(JOY_HELD);
    if (a == PAD_UP) {
        mff_cycles(0x8F14, 5);
        RAM(GUY_JUMP_WINDOW)++;
    } else {
        mff_cycles(0x8F12, 1);
    }
    mff_cycles(0x8F16, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    c = a >= 4;
    if (a == 4) {
        mff_cycles(0x8F1D, 2 + 3 + 6);
        a = 0x06;
        SET_NZ(a);
        RAM(ATTACK_ID) = a;
        JSR_FN(0x8F21, func_F43B);              /* attack_begin */
    } else {
        mff_cycles(0x8F1B, 1);
    }
    mff_cycles(0x8F24, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 6);
    if (z) {
        mff_cycles(0x8F2B, 6);
        JSR_FN(0x8F2B, func_F436);              /* attack_end */
    } else {
        mff_cycles(0x8F29, 1);
    }
    mff_cycles(0x8F2E, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    c = a >= 0x0B;
    if (a == 0x0B) {
        mff_cycles(0x8F35, 2 + 3 + 6);
        a = 0x03;
        SET_NZ(a);
        RAM(ATTACK_ID) = a;
        JSR_FN(0x8F39, func_F43B);              /* attack_begin */
        mff_cycles(0x8F3C, 4 + 3 + 2 + 6);
        a = RAM(OBJ_Y);
        RAM(ATTACK_GROUND_Y) = a;
        a = 0x34;
        SET_NZ(a);
        JSR_FN(0x8F43, func_F34C);              /* sound_queue_push_sfx */
    } else {
        mff_cycles(0x8F33, 1);
    }
    mff_cycles(0x8F46, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 0x11);
    if (z) {
        mff_cycles(0x8F4D, 6);
        JSR_FN(0x8F4D, func_F436);              /* attack_end */
    } else {
        mff_cycles(0x8F4B, 1);
    }
    mff_cycles(0x8F50, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 0x13);
    if (!z) {
        mff_cycles(0x8F55, 1 + 6);              /* BNE $8FAE: the RTS */
        RTS();
    }
    mff_cycles(0x8F57, 3 + 2 + 2);
    a = RAM(GUY_JUMP_WINDOW);
    COMPARE(a, GUY_POWER_UP_COUNT);
    if (!c) {
        mff_cycles(0x8F5B, 1);
        FALL_INTO(func_8F82_b0);                /* guy_power_end */
    }
    mff_cycles(0x8F5D, 2 + 4 + 3 + 3 + 2 + 2);
    a = 0x81;
    RAM(OBJ_STATE) = a;
    RAM(PLAYER_NO_HIT) = a;
    a = RAM(PLAYER_DIR) & PAD_RIGHT;
    SET_NZ(a);
    if (!z) {
        mff_cycles(0x8F6A, 6);
        JSR_FN(0x8F6A, func_F048);              /* player_face_left */
        mff_cycles(0x8F6D, 2);
        if (!z) {                               /* BNE on $F048's flags */
            mff_cycles(0x8F6D, 1);
        } else {
            mff_cycles(0x8F6F, 6);
            JSR_FN(0x8F6F, func_F03F);          /* player_face_right */
        }
    } else {
        mff_cycles(0x8F68, 1 + 6);
        JSR_FN(0x8F6F, func_F03F);              /* player_face_right */
    }
    mff_cycles(0x8F72, 3 + 2 + 3 + 6);
    a = RAM(PLAYER_DIR) ^ (PAD_LEFT | PAD_RIGHT);
    SET_NZ(a);
    RAM(PLAYER_DIR) = a;
    JSR_FN(0x8F78, func_F436);                  /* attack_end */
    mff_cycles(0x8F7B, 2 + 3 + 3);
    a = ACT_POWER_FOLLOW;
    SET_NZ(a);
    RAM(GAME_ACTION) = a;
    JMP_TAIL(0x8A55, 0);                        /* guy_follow_up */
}

/*
 * guy_power_end ($8F82 in bank $00, proposed): attack_end, back to idle
 * (game_action, guy_jump_window, jump_moving, obj_timer[0] and [1] 0,
 * obj_state[0] $80), the combo restarted, the walking speed in x,
 * player_stand. Run on into from guy_power; reached by JMP from
 * haggar_power_end (00:8177).
 */
void func_8F82_b0(void) {
    REGS_IN;

    mff_cycles(0x8F82, 6);
    JSR_FN(0x8F82, func_F436);                  /* attack_end */
    mff_cycles(0x8F85, 2 + 3 + 3 + 3 + 4 + 4 + 2 + 4 + 3 + 4);
    a = 0;
    RAM(GAME_ACTION) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(JUMP_MOVING) = a;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER + 1) = a;
    a = STATE_LIVE;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = prg_read_ix(COMBO_FIRST_ANIM, y);
    mff_cycles(0x8F9D, 3 + 4);
    RAM(PLAYER_STAND_ANIM) = a;
    a = prg_read_ix(WALK_VX_FRAC, y);
    mff_cycles(0x8FA2, 4 + 4);
    RAM(OBJ_VX_FRAC) = a;
    a = prg_read_ix(WALK_VX, y);
    SET_NZ(a);
    mff_cycles(0x8FA8, 4 + 6);
    RAM(OBJ_VX) = a;
    JSR_AT(0x8FAB, 0xCEAA, 0);                  /* player_stand */
    mff_cycles(0x8FAE, 6);
    RTS();
}
