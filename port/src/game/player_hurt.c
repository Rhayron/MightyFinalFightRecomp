/*
 * player_hurt.c - the player hit: the hit state (game_action 5) and the
 * knockdown (game_action 6) until the player lies down (game_action $1F,
 * bank $00).
 *
 * An enemy attack that reaches the player (objects_update, 0F:F8D5) sets
 * player_invuln to $3C and game_action 5, or 6 for a knockdown. The
 * knockdown counts its steps in obj_state[0]'s low nibble:
 *   0  the start: knocked away from the facing side (obj_dir = player_dir
 *      ^ 3), sound $28, the flight speed;
 *   1  flying back and falling (knockdown_fall);
 *   2  the same, after reaching the screen's edge;
 *   3  the bounce after landing (knockdown_bounce);
 *   4  the bounce after the edge; landing again lays the player down
 *      (knockdown_lie).
 * The screen's edges are 24 px and 232 px from its left (233 for the
 * bounce).
 */
#include "player_defs.h"

/* the top of the walking band, per round, by sub_area (player_clamp_top) */
#define BAND_TOP_LO         0xDC59   /* 63 66 69 63 69 */
#define BAND_TOP_HI         0xDC5E   /* DC DC DC DC DC: $DC63, $DC66, $DC69 */

/*
 * player_state_hit ($D9FA, labels.toml): game_action 5, reeling from a hit.
 * player_state dispatches here through its table (entry 5).
 *  - A+B pressed: the special (player_special_start, $CA3C).
 *  - while player_invuln is $1E or more it counts down;
 *  - then back to idle: the walking speed from $E1C1-$E1CA[player_id];
 *    grab_timer, game_action, combo_timer, jump_moving, grab_hit_count and
 *    player_invuln 0; player_stand ($CEAA); the combo restarts
 *    (player_stand_anim = $E288[player_id]).
 */
void func_D9FA(void) {
    REGS_IN;

    mff_cycles(0xD9FA, 3 + 2 + 2);
    a = RAM(JOY_PRESSED);
    COMPARE(a, PAD_A | PAD_B);
    if (z) {
        mff_cycles(0xDA00, 3);
        JMP_TAIL(0xCA3C, -1);                   /* player_special_start */
    }
    mff_cycles(0xD9FE, 1 + 3 + 2 + 2);
    a = RAM(PLAYER_INVULN);
    COMPARE(a, 0x1E);
    if (c) {
        mff_cycles(0xDA07, 1 + 5 + 6);
        RAM(PLAYER_INVULN)--;
        SET_NZ(RAM(PLAYER_INVULN));
        RTS();
    }
    mff_cycles(0xDA09, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(WALK_VX_FRAC + y));
    mff_cycles(0xDA0E, 4 + 4);
    RAM(OBJ_VX_FRAC) = a;
    a = prg_read((uint16_t)(WALK_VX + y));
    mff_cycles(0xDA14, 4 + 4);
    RAM(OBJ_VX) = a;
    a = prg_read((uint16_t)(WALK_VY_FRAC + y));
    mff_cycles(0xDA1A, 4 + 4);
    RAM(OBJ_VY_FRAC) = a;
    a = prg_read((uint16_t)(WALK_VY + y));
    mff_cycles(0xDA20, 4 + 2 + 6 * 3 + 6);
    RAM(OBJ_VY) = a;
    a = 0;
    SET_NZ(a);
    RAM(GRAB_TIMER) = a;
    RAM(GAME_ACTION) = a;
    RAM(COMBO_TIMER) = a;
    RAM(JUMP_MOVING) = a;
    RAM(GRAB_HIT_COUNT) = a;
    RAM(PLAYER_INVULN) = a;
    JSR_AT(0xDA31, 0xCEAA, -1);                 /* player_stand */
    mff_cycles(0xDA34, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(COMBO_FIRST_ANIM + y));
    SET_NZ(a);
    mff_cycles(0xDA39, 3 + 6);
    RAM(PLAYER_STAND_ANIM) = a;
    RTS();
}

/*
 * player_state_knockdown ($DA3F, labels.toml): game_action 6, steps 0-2
 * (step 3 on: knockdown_bounce). player_state dispatches here through its
 * table (entry 6). X = 0.
 *  - step 0: obj_dir[0] = player_dir ^ 3, the step counts up, sound $28 and
 *    the speed: obj_vx $00E6, obj_vy $03E8; in a boss area (boss_area
 *    negative) $0180 and $046E, and step 1 runs in the same frame.
 *  - steps 1 and 2: in the knockdown's direction, the player moves by vx
 *    up to the screen's edge (put back on it, and the step counts up, when
 *    past it); then knockdown_fall.
 */
void func_DA3F(void) {
    REGS_IN;

    mff_cycles(0xDA3F, 2 + 4 + 2 + 2);
    x = 0;
    a = RAM(OBJ_STATE) & 0x0F;
    if (a == 0) {
        mff_cycles(0xDA48, 3 + 2 + 4 + 6 + 2 + 6);
        a = RAM(PLAYER_DIR) ^ (PAD_LEFT | PAD_RIGHT);
        RAM(OBJ_DIR) = a;
        RAM(OBJ_STATE)++;
        a = SFX_KNOCKDOWN;
        SET_NZ(a);
        JSR_FN(0xDA54, func_F34C);              /* sound_queue_push_sfx */
        mff_cycles(0xDA57, 3 + 2);
        a = RAM(BOSS_AREA);
        if (!(a & 0x80)) {
            mff_cycles(0xDA5B, 2 + 4 + 2 + 4 + 2 + 4 + 2 + 4 + 6);
            a = 0xE6;
            RAM(OBJ_VX_FRAC) = a;
            a = 0x00;
            RAM(OBJ_VX) = a;
            a = 0xE8;
            RAM(OBJ_VY_FRAC) = a;
            a = 0x03;
            SET_NZ(a);
            RAM(OBJ_VY) = a;
            RTS();
        }
        mff_cycles(0xDA59, 1 + 2 + 4 + 2 + 4 + 2 + 4 + 2 + 4);
        a = 0x80;
        RAM(OBJ_VX_FRAC) = a;
        a = 0x01;
        RAM(OBJ_VX) = a;
        a = 0x6E;
        RAM(OBJ_VY_FRAC) = a;
        a = 0x04;
        RAM(OBJ_VY) = a;
    } else {
        mff_cycles(0xDA46, 1);
    }

    mff_cycles(0xDA84, 4 + 2 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x0F;
    COMPARE(a, 3);
    if (z) {
        mff_cycles(0xDA8D, 3);
        JMP_TAIL(0xDB6C, -1);                   /* knockdown_bounce */
    }
    mff_cycles(0xDA8B, 1 + 4 + 2 + 2);
    a = RAM(OBJ_DIR) & PAD_RIGHT;
    if (a != 0) {                               /* flying right */
        mff_cycles(0xDA97, 4 + 2 + 2 + 2);
        a = RAM(OBJ_STATE) & 0x0F;
        COMPARE(a, 2);
        if (z) {
            mff_cycles(0xDA9E, 1 + 3);
            JMP_TAIL(0xDB03, -1);               /* knockdown_fall */
        }
        mff_cycles(0xDAA0, 2 + 2);
        COMPARE(a, 4);
        if (z) {
            mff_cycles(0xDAA2, 1 + 3);
            JMP_TAIL(0xDBD4, -1);               /* knockdown_fall_again */
        }
        mff_cycles(0xDAA4, 4 + 2 + 3 + 2 + 2);
        a = RAM(OBJ_X);
        c = 1;
        SBC(RAM(SCROLL_X));
        COMPARE(a, 0xE9);
        if (!z) {
            mff_cycles(0xDAAE, 2);
            if (!c) {                           /* short of the edge */
                mff_cycles(0xDAAE, 1 + 6);
                JSR_FN(0xDAC5, func_F183);      /* obj_move_right */
                mff_cycles(0xDAC8, 3);
                JMP_TAIL(0xDB03, -1);           /* knockdown_fall */
            }
            mff_cycles(0xDAB0, 3 + 2 + 2 + 4 + 3 + 2 + 4);
            a = RAM(SCROLL_X);                  /* past it: back on it */
            c = 0;
            ADC(0xE8);
            RAM(OBJ_X) = a;
            a = RAM(AREA_SCREEN);
            ADC(0);
            RAM(OBJ_X_HI) = a;
        } else {
            mff_cycles(0xDAAC, 1);
        }
        mff_cycles(0xDABF, 6 + 3);
        RAM(OBJ_STATE)++;
        SET_NZ(RAM(OBJ_STATE));
        JMP_TAIL(0xDB03, -1);                   /* knockdown_fall */
    }

    mff_cycles(0xDA95, 1 + 4 + 2 + 2 + 2);      /* flying left */
    a = RAM(OBJ_STATE) & 0x0F;
    COMPARE(a, 2);
    if (z) {
        mff_cycles(0xDAD5, 2);
        FALL_INTO(func_DB03);                   /* knockdown_fall */
    }
    mff_cycles(0xDAD7, 2 + 2);
    COMPARE(a, 4);
    if (z) {
        mff_cycles(0xDAD9, 1 + 3);
        JMP_TAIL(0xDBD4, -1);                   /* knockdown_fall_again */
    }
    mff_cycles(0xDADB, 4 + 2 + 3 + 3 + 3 + 2 + 2);
    a = RAM(OBJ_X);
    c = 1;
    SBC(RAM(SCROLL_X));
    RAM(TMP0) = a;
    a = RAM(TMP0);
    COMPARE(a, 0x18);
    if (!z) {
        mff_cycles(0xDAE9, 2);
        if (c) {                                /* short of the edge */
            mff_cycles(0xDAE9, 2 + 6);
            JSR_FN(0xDB00, func_F1A1);          /* obj_move_left */
            FALL_INTO(func_DB03);               /* knockdown_fall */
        }
        mff_cycles(0xDAEB, 3 + 2 + 2 + 4 + 3 + 2 + 4);
        a = RAM(SCROLL_X);                      /* past it: back on it */
        c = 0;
        ADC(0x19);
        RAM(OBJ_X) = a;
        a = RAM(AREA_SCREEN);
        ADC(0);
        RAM(OBJ_X_HI) = a;
    } else {
        mff_cycles(0xDAE7, 1);
    }
    mff_cycles(0xDAFA, 6 + 3);
    RAM(OBJ_STATE)++;
    SET_NZ(RAM(OBJ_STATE));
    JMP_TAIL(0xDB03, -1);                       /* knockdown_fall */
}

/*
 * knockdown_fall ($DB03, proposed): obj_fall (Y = 4). On the floor (C set),
 * knockdown_land. Otherwise, once falling, when obj_y + 2 reaches
 * player_ground_y obj_y becomes obj_y - 1 (unless exactly on the line),
 * player_clamp_top runs and knockdown_land follows. Reached by JMP from
 * player_state_knockdown (0E:DAC2, DAC8, DAFD) and run on into from it.
 */
void func_DB03(void) {
    REGS_IN;

    mff_cycles(0xDB03, 2 + 6);
    y = 4;
    SET_NZ(y);
    JSR_FN(0xDB05, func_F119);                  /* obj_fall */
    mff_cycles(0xDB08, 2);
    if (c) {
        mff_cycles(0xDB0A, 3);
        JMP_TAIL(0xDB2C, -1);                   /* knockdown_land */
    }
    mff_cycles(0xDB08, 1 + 4 + 2);
    a = RAM(OBJ_VY);
    SET_NZ(a);
    if (!n) {                                   /* still rising */
        mff_cycles(0xDB10, 1 + 6);
        RTS();
    }
    mff_cycles(0xDB12, 4 + 2 + 2 + 3 + 2);
    a = RAM(OBJ_Y);
    c = 0;
    ADC(2);
    COMPARE(a, RAM(PLAYER_GROUND_Y));
    if (!c) {                                   /* above the ground line */
        mff_cycles(0xDB1A, 1 + 6);
        RTS();
    }
    mff_cycles(0xDB1C, 2);
    if (!z) {
        mff_cycles(0xDB1E, 2 + 4 + 6);
        SBC(3);
        RAM(OBJ_Y) = a;
        JSR_AT(0xDB23, 0xDC29, -1);             /* player_clamp_top */
        mff_cycles(0xDB26, 3);
        JMP_TAIL(0xDB2C, -1);                   /* knockdown_land */
    }
    mff_cycles(0xDB1C, 1 + 6);
    JSR_AT(0xDB29, 0xDC29, -1);                 /* player_clamp_top */
    FALL_INTO(func_DB2C);
}

/*
 * knockdown_land ($DB2C, proposed): the first landing. The step counts up
 * (to 3) and the bounce gets its speed: obj_vx $0080, obj_vy $016E ($0100
 * and $016E in a boss area); the knocked-down animation and CHR bank
 * ($E20D/$E255[player_id]). Reached by JMP from knockdown_fall (0E:DB0A,
 * DB26) and run on into from it.
 */
void func_DB2C(void) {
    REGS_IN;

    mff_cycles(0xDB2C, 6 + 3 + 2);
    RAM(OBJ_STATE)++;
    a = RAM(BOSS_AREA);
    if (!(a & 0x80)) {
        mff_cycles(0xDB33, 2 + 4 + 2 + 4 + 2 + 4 + 2 + 4 + 2 + 1);
        a = 0x80;
        RAM(OBJ_VX_FRAC) = a;
        a = 0x00;
        RAM(OBJ_VX) = a;
        a = 0x6E;
        RAM(OBJ_VY_FRAC) = a;
        a = 0x01;
        RAM(OBJ_VY) = a;
    } else {
        mff_cycles(0xDB31, 1 + 2 + 4 + 2 + 4 + 2 + 4 + 2 + 4);
        a = 0x00;
        RAM(OBJ_VX_FRAC) = a;
        a = 0x01;
        RAM(OBJ_VX) = a;
        a = 0x6E;
        RAM(OBJ_VY_FRAC) = a;
        a = 0x01;
        RAM(OBJ_VY) = a;
    }
    mff_cycles(0xDB5D, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(ANIM_KNOCKED_DOWN + y));
    SET_NZ(a);
    mff_cycles(0xDB62, 6);
    JSR_FN(0xDB62, func_F294);                  /* player_set_anim */
    mff_cycles(0xDB65, 4);
    a = prg_read((uint16_t)(CHR_KNOCKED_DOWN + y));
    SET_NZ(a);
    mff_cycles(0xDB68, 6);
    JSR_FN(0xDB68, func_F440);                  /* player_set_chr */
    mff_cycles(0xDB6B, 6);
    RTS();
}

/*
 * knockdown_bounce ($DB6C, proposed): steps 3 and 4. As steps 1 and 2 of
 * player_state_knockdown (the edges at 24 and 233 px), then
 * knockdown_fall_again; at the edge in step 4 straight to knockdown_lie
 * (right) or knockdown_fall_again (left). Reached by JMP from
 * player_state_knockdown (0E:DA8D). The left edge put-back is never
 * executed in any run.
 */
void func_DB6C(void) {
    REGS_IN;

    mff_cycles(0xDB6C, 4 + 2 + 2);
    a = RAM(OBJ_DIR) & PAD_RIGHT;
    if (a != 0) {                               /* flying right */
        mff_cycles(0xDB73, 4 + 2 + 2 + 2);
        a = RAM(OBJ_STATE) & 0x0F;
        COMPARE(a, 4);
        if (z) {
            mff_cycles(0xDB7A, 1 + 3);
            JMP_TAIL(0xDBFD, -1);               /* knockdown_lie */
        }
        mff_cycles(0xDB7C, 4 + 2 + 3 + 2 + 2);
        a = RAM(OBJ_X);
        c = 1;
        SBC(RAM(SCROLL_X));
        COMPARE(a, 0xE8);
        if (!z) {
            mff_cycles(0xDB86, 2);
            if (!c) {                           /* short of the edge */
                mff_cycles(0xDB86, 1 + 6);
                JSR_FN(0xDB9D, func_F183);      /* obj_move_right */
                mff_cycles(0xDBA0, 3);
                JMP_TAIL(0xDBD4, -1);           /* knockdown_fall_again */
            }
            mff_cycles(0xDB88, 3 + 2 + 2 + 4 + 3 + 2 + 4);
            a = RAM(SCROLL_X);                  /* past it: back on it */
            c = 0;
            ADC(0xE9);
            RAM(OBJ_X) = a;
            a = RAM(AREA_SCREEN);
            ADC(0);
            RAM(OBJ_X_HI) = a;
        } else {
            mff_cycles(0xDB84, 1);
        }
        mff_cycles(0xDB97, 6 + 3);
        RAM(OBJ_STATE)++;
        SET_NZ(RAM(OBJ_STATE));
        JMP_TAIL(0xDBFD, -1);                   /* knockdown_lie */
    }

    mff_cycles(0xDB71, 1 + 4 + 2 + 2 + 2);      /* flying left */
    a = RAM(OBJ_STATE) & 0x0F;
    COMPARE(a, 4);
    if (z) {
        mff_cycles(0xDBAA, 1 + 3);
        JMP_TAIL(0xDBD4, -1);                   /* knockdown_fall_again */
    }
    mff_cycles(0xDBAC, 4 + 2 + 3 + 3 + 3 + 2 + 2);
    a = RAM(OBJ_X);
    c = 1;
    SBC(RAM(SCROLL_X));
    RAM(TMP0) = a;
    a = RAM(TMP0);
    COMPARE(a, 0x18);
    if (!z) {
        mff_cycles(0xDBBA, 2);
        if (c) {                                /* short of the edge */
            mff_cycles(0xDBBA, 1 + 6);
            JSR_FN(0xDBD1, func_F1A1);          /* obj_move_left */
            FALL_INTO(func_DBD4);               /* knockdown_fall_again */
        }
        mff_cycles(0xDBBC, 3 + 2 + 2 + 4 + 3 + 2 + 4);
        a = RAM(SCROLL_X);                      /* past it: back on it */
        c = 0;
        ADC(0x19);
        RAM(OBJ_X) = a;
        a = RAM(AREA_SCREEN);
        ADC(0);
        RAM(OBJ_X_HI) = a;
    } else {
        mff_cycles(0xDBB8, 1);
    }
    mff_cycles(0xDBCB, 7 + 3);
    RAM(OBJ_STATE + x)++;
    SET_NZ(RAM(OBJ_STATE + x));
    JMP_TAIL(0xDBD4, -1);                       /* knockdown_fall_again */
}

/*
 * knockdown_fall_again ($DBD4, proposed): the bounce's fall, as
 * knockdown_fall, ending in knockdown_lie. Reached by JMP from
 * player_state_knockdown and knockdown_bounce (0E:DACB, DBA0, DBCE) and run
 * on into from knockdown_bounce.
 */
void func_DBD4(void) {
    REGS_IN;

    mff_cycles(0xDBD4, 2 + 6);
    y = 4;
    SET_NZ(y);
    JSR_FN(0xDBD6, func_F119);                  /* obj_fall */
    mff_cycles(0xDBD9, 2);
    if (c) {
        mff_cycles(0xDBDB, 3);
        JMP_TAIL(0xDBFD, -1);                   /* knockdown_lie */
    }
    mff_cycles(0xDBD9, 1 + 4 + 2);
    a = RAM(OBJ_VY);
    SET_NZ(a);
    if (n) {                                    /* falling */
        mff_cycles(0xDBE3, 4 + 2 + 2 + 3 + 2);
        a = RAM(OBJ_Y);
        c = 0;
        ADC(2);
        COMPARE(a, RAM(PLAYER_GROUND_Y));
        if (c) {                                /* at the ground line */
            mff_cycles(0xDBED, 2);
            if (!z) {
                mff_cycles(0xDBEF, 2 + 4 + 6);
                SBC(3);
                RAM(OBJ_Y) = a;
                JSR_AT(0xDBF4, 0xDC29, -1);     /* player_clamp_top */
                mff_cycles(0xDBF7, 3);
                JMP_TAIL(0xDBFD, -1);           /* knockdown_lie */
            }
            mff_cycles(0xDBED, 1 + 6);
            JSR_AT(0xDBFA, 0xDC29, -1);         /* player_clamp_top */
            FALL_INTO(func_DBFD);
        }
    }
    mff_cycles(0xDBE1, 2 + 6);                  /* BPL or BCC taken (to the next page), RTS */
    RTS();
}

/*
 * knockdown_lie ($DBFD, proposed): the player lies down. The knocked-down
 * animation and CHR bank, action_timer $3C, game_action $1F (bank $00),
 * obj_state[0] $80, obj_timer[0], attack_live and attack_ground_y 0,
 * jump_dir = player_dir, player_clamp_top. Reached by JMP from
 * knockdown_bounce and knockdown_fall_again (0E:DB9A, DBDB, DBF7) and run on
 * into from the latter.
 */
void func_DBFD(void) {
    REGS_IN;

    mff_cycles(0xDBFD, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(ANIM_KNOCKED_DOWN + y));
    SET_NZ(a);
    mff_cycles(0xDC02, 6);
    JSR_FN(0xDC02, func_F294);                  /* player_set_anim */
    mff_cycles(0xDC05, 4);
    a = prg_read((uint16_t)(CHR_KNOCKED_DOWN + y));
    SET_NZ(a);
    mff_cycles(0xDC08, 6);
    JSR_FN(0xDC08, func_F440);                  /* player_set_chr */
    mff_cycles(0xDC0B, 2 + 3 + 2 + 3 + 2 + 4 + 2 + 4 + 3 + 3 + 3 + 3 + 6);
    a = 0x3C;
    RAM(ACTION_TIMER) = a;
    a = ACT_LYING;
    RAM(GAME_ACTION) = a;
    a = STATE_LIVE;
    RAM(OBJ_STATE) = a;
    a = 0;
    RAM(OBJ_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(ATTACK_GROUND_Y) = a;
    a = RAM(PLAYER_DIR);
    SET_NZ(a);
    RAM(JUMP_DIR) = a;
    JSR_AT(0xDC25, 0xDC29, -1);                 /* player_clamp_top */
    mff_cycles(0xDC28, 6);
    RTS();
}

/*
 * player_clamp_top ($DC29, proposed): keeps the player inside the walking
 * band: an obj_y at or above the band's top for this round and sub_area
 * ($DC63.. through $DC59/$DC5E[round]) is put on it, 2 px lower for Haggar,
 * and player_ground_y follows. Called by the knockdown (JSR at 0E:DB23,
 * DB29, DBF4, DBFA, DC25).
 * Out: $04/$05 = the round's band table.
 */
void func_DC29(void) {
    REGS_IN;

    mff_cycles(0xDC29, 3 + 4);
    y = RAM(ROUND);
    a = prg_read((uint16_t)(BAND_TOP_LO + y));
    mff_cycles(0xDC2E, 3 + 4);
    RAM(TMP4) = a;
    a = prg_read((uint16_t)(BAND_TOP_HI + y));
    mff_cycles(0xDC33, 3 + 3 + 4 + 5);
    RAM(TMP5) = a;
    y = RAM(SUB_AREA);
    a = RAM(OBJ_Y);
    COMPARE(a, mem_read((uint16_t)(RAM16(TMP4) + y)));
    mff_cycles(0xDC3C, 2);
    if (z) {
        mff_cycles(0xDC3C, 1);
    } else {
        mff_cycles(0xDC3E, 2);
        if (c) {                                /* below the top: nothing to do */
            mff_cycles(0xDC3E, 1 + 6);
            RTS();
        }
    }
    mff_cycles(0xDC40, 5);
    a = mem_read((uint16_t)(RAM16(TMP4) + y));
    mff_cycles(0xDC42, 4 + 3 + 3 + 2 + 2);
    RAM(OBJ_Y) = a;
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(PLAYER_ID);
    COMPARE(a, PLAYER_HAGGAR);
    if (z) {
        mff_cycles(0xDC4D, 4 + 2 + 2 + 4 + 3);
        a = RAM(OBJ_Y);
        c = 0;
        ADC(2);
        RAM(OBJ_Y) = a;
        RAM(PLAYER_GROUND_Y) = a;
    } else {
        mff_cycles(0xDC4B, 1);
    }
    mff_cycles(0xDC58, 6);
    RTS();
}
