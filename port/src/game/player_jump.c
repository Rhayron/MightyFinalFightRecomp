/*
 * player_jump.c - the player's jump: landing and steering in the air
 * (player_state_jump itself, 0E:CF88-D18D, is in the $C000 file and reaches
 * these by JMP)
 */
#include "player_defs.h"

/*
 * player_jump_land ($D00E, proposed name): the end of a jump.
 * player_state_jump comes here when obj_fall ($F119) reports the floor (JMP
 * at 0E:CF91) or when obj_y reaches player_ground_y (BNE at 0E:D009, and on
 * from 0E:D00B).
 *
 * In: $10 = attribute of the tile the floor probe found.
 *  - $40 and up, a pit: player_pit_fall ($CFD1) takes over.
 *  - otherwise game_action = ACT_LAND with action_timer 3, the attack ends
 *    (attack_end, $F436) and the player stands (player_stand, $CEAA). Guy,
 *    landing while guy_jump_window runs (Up during his jump), takes animation
 *    $68 and CHR bank 6 instead. Both then play the landing sound
 *    (player_land_finish, $D03F).
 * Out: A = 0 after a landing; X and Y as the callees leave them.
 */
void func_D00E(void) {
    REGS_IN;

    mff_cycles(0xD00E, 3 + 2 + 2);
    a = RAM(TMP16);
    COMPARE(a, PROBE_PIT);
    if (c) {                                    /* a pit */
        mff_cycles(0xD012, 1 + 6);
        JSR_AT(0xD016, 0xCFD1, -1);             /* player_pit_fall */
        mff_cycles(0xD019, 6);
        RTS();
    }
    /* BCC (always taken here), then the two stores and the JSR */
    mff_cycles(0xD014, 2 + 1 + 2 + 3 + 2 + 3 + 6);
    a = ACT_LAND;
    RAM(GAME_ACTION) = a;
    a = 3;
    SET_NZ(a);
    RAM(ACTION_TIMER) = a;
    JSR_FN(0xD022, func_F436);                  /* attack_end */

    mff_cycles(0xD025, 3 + 2 + 2);
    a = RAM(PLAYER_ID);
    COMPARE(a, PLAYER_GUY);
    if (z) {
        mff_cycles(0xD02B, 3 + 2);
        a = RAM(GUY_JUMP_WINDOW);
        SET_NZ(a);
        if (!z) {                               /* Guy inside his jump window */
            mff_cycles(0xD02F, 2 + 6);
            a = 0x68;
            SET_NZ(a);
            JSR_FN(0xD031, func_F294);          /* player_set_anim */
            mff_cycles(0xD034, 2 + 6);
            a = 0x06;
            SET_NZ(a);
            JSR_FN(0xD036, func_F440);          /* player_set_chr */
            mff_cycles(0xD039, 3);
            JMP_TAIL(0xD03F, -1);               /* player_land_finish */
        }
    }
    mff_cycles(0xD029, 1 + 6);                  /* BNE or BEQ taken, JSR */
    JSR_AT(0xD03C, 0xCEAA, -1);                 /* player_stand */
    FALL_INTO(func_D03F);
}

/*
 * player_land_finish ($D03F, proposed): the landing sound and
 * obj_timer2[0] = 0. Reached by JMP from player_jump_land (0E:D039) and run
 * on into after its player_stand (0E:D03C).
 * Out: A = 0 (N clear, Z set).
 */
void func_D03F(void) {
    REGS_IN;

    mff_cycles(0xD03F, 2 + 6);
    a = SFX_LAND;
    SET_NZ(a);
    JSR_FN(0xD041, func_F34C);                  /* sound_queue_push_sfx */
    mff_cycles(0xD044, 2 + 4 + 6);
    a = 0;
    SET_NZ(a);
    RAM(OBJ_TIMER2) = a;
    RTS();
}

/*
 * player_jump_steer ($D18E, proposed): left or right during a jump. Reached
 * by JMP from player_state_jump's air control (0E:D067, D073, D0B9, D0D5,
 * D0EB).
 *
 * Once the jump moves (jump_moving set) player_jump_drift ($D242) carries
 * it on. Otherwise, while the jump still rises (obj_vy[0] not negative:
 * obj_y goes up by it), not in a jump kick, with left or right held:
 *  - the animation becomes the moving jump (with its CHR bank) unless B was
 *    just pressed or it already is;
 *  - jump_moving is counted up and jump_dir takes the direction (pad bits,
 *    through $00);
 *  - the sprite faces that way (obj_face_right/left on slot X) and moves a
 *    step (obj_move_right/left by vx);
 *  - if that is not the way the player faces (player_dir), the jump goes on
 *    backwards: the back-jump animation, vx from $E1CD/$E1D0 and obj_flags
 *    turned back to the facing side.
 * Going right with the camera scrolling by itself (camera_autoscroll
 * negative) and not held on a screen (camera_lock positive), a player at 241
 * px or more from the screen's left edge is put back at 240 and does not
 * move (0E:D1D9; never executed in any run).
 */
void func_D18E(void) {
    REGS_IN;

    mff_cycles(0xD18E, 3 + 2);
    a = RAM(JUMP_MOVING);
    SET_NZ(a);
    if (!z) {                                   /* already moving */
        mff_cycles(0xD190, 1 + 3);
        JMP_TAIL(0xD242, -1);                   /* player_jump_drift */
    }
    mff_cycles(0xD192, 4 + 2);
    a = RAM(OBJ_VY);
    SET_NZ(a);
    if (n) {                                    /* falling already */
        mff_cycles(0xD195, 1 + 6);
        RTS();
    }
    mff_cycles(0xD197, 3 + 4 + 4);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    COMPARE(a, prg_read((uint16_t)(IS_JUMP_KICK + y)));
    mff_cycles(0xD19F, 2);
    if (z) {                                    /* kicking */
        mff_cycles(0xD19F, 1 + 6);
        RTS();
    }
    mff_cycles(0xD1A1, 3 + 2 + 2);
    a = RAM(JOY_HELD) & (PAD_LEFT | PAD_RIGHT);
    SET_NZ(a);
    if (z) {                                    /* neither held */
        mff_cycles(0xD1A5, 1 + 6);
        RTS();
    }

    mff_cycles(0xD1A7, 3 + 3 + 2 + 2);
    RAM(TMP0) = a;                              /* the direction */
    a = RAM(JOY_PRESSED);
    COMPARE(a, PAD_B);
    if (!z) {                                   /* B not just pressed */
        mff_cycles(0xD1AF, 3 + 4 + 4);
        y = RAM(PLAYER_ID);
        a = RAM(OBJ_ANIM);
        COMPARE(a, prg_read((uint16_t)(IS_JUMP_MOVING + y)));
        mff_cycles(0xD1B7, 2);
        if (!z) {                               /* not the moving jump yet */
            mff_cycles(0xD1B9, 3 + 4);
            y = RAM(PLAYER_ID);
            a = prg_read((uint16_t)(ANIM_JUMP_MOVING + y));
            SET_NZ(a);
            mff_cycles(0xD1BE, 6);
            JSR_FN(0xD1BE, func_F294);          /* player_set_anim */
            mff_cycles(0xD1C1, 4);
            a = prg_read((uint16_t)(CHR_JUMP + y));
            SET_NZ(a);
            mff_cycles(0xD1C4, 6);
            JSR_FN(0xD1C4, func_F440);          /* player_set_chr */
        } else {
            mff_cycles(0xD1B7, 1);
        }
    } else {
        mff_cycles(0xD1AD, 1);
    }

    mff_cycles(0xD1C7, 5 + 3 + 3 + 2 + 2);
    RAM(JUMP_MOVING)++;
    a = RAM(TMP0);
    RAM(JUMP_DIR) = a;
    a &= PAD_RIGHT;
    SET_NZ(a);
    if (z) {                                    /* left */
        mff_cycles(0xD1CF, 2 + 6);
        JSR_FN(0xD21C, func_F099);              /* obj_face_left */
        mff_cycles(0xD21F, 6);
        JSR_FN(0xD21F, func_F1A1);              /* obj_move_left */
        mff_cycles(0xD222, 3 + 3 + 2);
        a = RAM(PLAYER_DIR);
        COMPARE(a, RAM(JUMP_DIR));
        if (!z) {                               /* facing right: backwards */
            mff_cycles(0xD228, 3 + 4);
            y = RAM(PLAYER_ID);
            a = prg_read((uint16_t)(ANIM_JUMP_BACK + y));
            SET_NZ(a);
            mff_cycles(0xD22D, 6);
            JSR_FN(0xD22D, func_F294);          /* player_set_anim */
            mff_cycles(0xD230, 4);
            a = prg_read((uint16_t)(JUMP_BACK_VX_FRAC + y));
            mff_cycles(0xD233, 4 + 4);
            RAM(OBJ_VX_FRAC) = a;
            a = prg_read((uint16_t)(JUMP_BACK_VX + y));
            mff_cycles(0xD239, 4 + 2 + 4);
            RAM(OBJ_VX) = a;
            a = FLAGS_FACING_RIGHT;
            SET_NZ(a);
            RAM(OBJ_FLAGS) = a;
        } else {
            mff_cycles(0xD226, 1);
        }
        mff_cycles(0xD241, 6);
        RTS();
    }

    /* right */
    mff_cycles(0xD1D1, 3 + 2);
    a = RAM(CAMERA_AUTOSCROLL);
    SET_NZ(a);
    if (n) {
        mff_cycles(0xD1D5, 3 + 2);
        a = RAM(CAMERA_LOCK);
        SET_NZ(a);
        if (!n) {
            mff_cycles(0xD1D9, 4 + 2 + 3 + 2 + 2);
            a = RAM(OBJ_X);
            c = 1;
            SBC(RAM(SCROLL_X));
            COMPARE(a, SCREEN_RIGHT_LIMIT + 1);
            if (c) {                            /* at the edge: stay there */
                mff_cycles(0xD1E3, 3 + 2 + 2 + 4 + 3 + 2 + 4 + 6);
                a = RAM(SCROLL_X);
                c = 0;
                ADC(SCREEN_RIGHT_LIMIT);
                RAM(OBJ_X) = a;
                a = RAM(AREA_SCREEN);
                ADC(0);
                RAM(OBJ_X_HI) = a;
                RTS();
            }
        }
    }
    mff_cycles(0xD1F6, 1 + 6);                  /* BPL, BMI or BCC taken, JSR */
    JSR_FN(0xD1F6, func_F090);                  /* obj_face_right */
    mff_cycles(0xD1F9, 6);
    JSR_FN(0xD1F9, func_F183);                  /* obj_move_right */
    mff_cycles(0xD1FC, 3 + 3 + 2);
    a = RAM(PLAYER_DIR);
    COMPARE(a, RAM(JUMP_DIR));
    if (z) {                                    /* facing right too */
        mff_cycles(0xD200, 1 + 6);
        RTS();
    }
    mff_cycles(0xD202, 3 + 4);                  /* facing left: backwards */
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(ANIM_JUMP_BACK + y));
    SET_NZ(a);
    mff_cycles(0xD207, 6);
    JSR_FN(0xD207, func_F294);                  /* player_set_anim */
    mff_cycles(0xD20A, 4);
    a = prg_read((uint16_t)(JUMP_BACK_VX_FRAC + y));
    mff_cycles(0xD20D, 4 + 4);
    RAM(OBJ_VX_FRAC) = a;
    a = prg_read((uint16_t)(JUMP_BACK_VX + y));
    mff_cycles(0xD213, 4 + 2 + 4 + 6);
    RAM(OBJ_VX) = a;
    a = FLAGS_FACING_LEFT;
    SET_NZ(a);
    RAM(OBJ_FLAGS) = a;
    RTS();
}

/*
 * player_jump_drift ($D242, proposed): a moving jump, every frame after
 * player_jump_steer started it (JMP at 0E:D1F3).
 *  - the standing jump's animation turns into the moving jump on its fourth
 *    frame (obj_anim_timer 4);
 *  - the moving jump kick turns into the jump kick, at once for Guy and on
 *    the fourth frame for the others;
 *  - then the player moves by jump_dir: obj_move_right or obj_move_left,
 *    reached by JMP (with the camera clamp of player_jump_steer first when
 *    going right; never executed in any run).
 */
void func_D242(void) {
    REGS_IN;

    mff_cycles(0xD242, 3 + 4 + 4);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    mff_cycles(0xD24A, 2);
    if (a != prg_read((uint16_t)(IS_JUMP_MOVING + y))) {
        mff_cycles(0xD24C, 4 + 2);
        if (a == prg_read((uint16_t)(IS_JUMP + y))) {
            mff_cycles(0xD251, 4 + 2 + 2);
            a = RAM(OBJ_ANIM_TIMER);
            COMPARE(a, 4);
            if (z) {                            /* the standing jump's 4th frame */
                mff_cycles(0xD258, 4);
                a = prg_read((uint16_t)(ANIM_JUMP_MOVING + y));
                SET_NZ(a);
                mff_cycles(0xD25B, 6);
                JSR_FN(0xD25B, func_F294);      /* player_set_anim */
                mff_cycles(0xD25E, 4);
                a = prg_read((uint16_t)(CHR_JUMP + y));
                SET_NZ(a);
                mff_cycles(0xD261, 6);
                JSR_FN(0xD261, func_F440);      /* player_set_chr */
            } else {
                mff_cycles(0xD256, 1);
            }
        } else {
            mff_cycles(0xD24F, 1);
        }
    } else {
        mff_cycles(0xD24A, 1);
    }

    mff_cycles(0xD264, 3 + 4 + 4);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    COMPARE(a, prg_read((uint16_t)(IS_JUMP_KICK_MOVING + y)));
    mff_cycles(0xD26C, 2);
    if (z) {                                    /* the moving jump kick */
        mff_cycles(0xD26E, 3 + 2 + 2);
        a = RAM(PLAYER_ID);
        COMPARE(a, PLAYER_GUY);
        if (z) {
            mff_cycles(0xD272, 1);              /* Guy: now */
        } else {
            mff_cycles(0xD274, 4 + 2 + 2);      /* the others: on its 4th frame */
            a = RAM(OBJ_ANIM_TIMER);
            COMPARE(a, 4);
        }
        if (z) {
            mff_cycles(0xD27B, 3 + 4);
            y = RAM(PLAYER_ID);
            a = prg_read((uint16_t)(ANIM_JUMP_KICK + y));
            SET_NZ(a);
            mff_cycles(0xD280, 6);
            JSR_FN(0xD280, func_F294);          /* player_set_anim */
            mff_cycles(0xD283, 4);
            a = prg_read((uint16_t)(CHR_JUMP_KICK + y));
            SET_NZ(a);
            mff_cycles(0xD286, 6);
            JSR_FN(0xD286, func_F440);          /* player_set_chr */
        } else {
            mff_cycles(0xD279, 1);
        }
    } else {
        mff_cycles(0xD26C, 1);
    }

    mff_cycles(0xD289, 3 + 2 + 2);
    a = RAM(JUMP_DIR) & PAD_RIGHT;
    SET_NZ(a);
    if (z) {                                    /* left */
        mff_cycles(0xD28D, 1 + 3);
        JMP_FN(0xF1A1, func_F1A1);              /* obj_move_left */
    }
    mff_cycles(0xD28F, 3 + 2);
    a = RAM(CAMERA_AUTOSCROLL);
    SET_NZ(a);
    if (n) {
        mff_cycles(0xD293, 3 + 2);
        a = RAM(CAMERA_LOCK);
        SET_NZ(a);
        if (!n) {
            mff_cycles(0xD297, 4 + 2 + 3 + 2 + 2);
            a = RAM(OBJ_X);
            c = 1;
            SBC(RAM(SCROLL_X));
            COMPARE(a, SCREEN_RIGHT_LIMIT + 1);
            if (c) {                            /* at the edge: stay there */
                mff_cycles(0xD2A1, 3 + 2 + 2 + 4 + 3 + 2 + 4 + 6);
                a = RAM(SCROLL_X);
                c = 0;
                ADC(SCREEN_RIGHT_LIMIT);
                RAM(OBJ_X) = a;
                a = RAM(AREA_SCREEN);
                ADC(0);
                RAM(OBJ_X_HI) = a;
                RTS();
            }
        }
    }
    mff_cycles(0xD2B1, 1 + 3);                  /* BPL, BMI or BCC taken, JMP */
    JMP_FN(0xF183, func_F183);                  /* obj_move_right */
}
