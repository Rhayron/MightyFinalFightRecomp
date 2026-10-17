/*
 * player_states.c - player_state handlers that do not fit the jump, attack,
 * grab or hurt files (player_state dispatches on game_action through
 * 0E:C9CA/C9F5)
 */
#include "player_defs.h"

/* game_action of the move Guy starts with B inside his jump window */
#define WINDOW_ACTION       0xE24C   /* 23 18 00 */
/* player_no_hit for it */
#define WINDOW_NO_HIT       0xFCC5   /* FF FF 00 */

/*
 * player_state_land ($D2B7; labels.toml player_state_02, proposed name):
 * game_action 2, the frames after a jump lands (player_jump_land sets
 * action_timer 3) or Haggar's finisher (8). player_state dispatches here
 * through its table (entry 2).
 *  - action_timer counts down while it is not negative; then
 *  - Haggar, or anyone with guy_jump_window 0: back to idle. game_action,
 *    jump_moving, combo_timer, attack_live, player_invuln, grab_hit_count and
 *    slot 0's obj_timer, obj_timer2 and obj_ground_y go to 0, the player
 *    stands (player_stand, $CEAA) and the combo restarts
 *    (player_stand_anim = $E288[player_id]).
 *  - Guy with his jump window open: it counts down, player_ground_y follows
 *    obj_y, and B starts game_action $E24C[player_id] (Guy $18) with
 *    player_no_hit $FCC5[player_id] and obj_state[0] $80 (0E:D2F8; never
 *    executed in any run).
 * Out: A = player_stand_anim after idle.
 */
void func_D2B7(void) {
    REGS_IN;

    mff_cycles(0xD2B7, 3 + 2);
    a = RAM(ACTION_TIMER);
    if (!(a & 0x80)) {
        mff_cycles(0xD2BB, 5 + 6);
        RAM(ACTION_TIMER)--;
        SET_NZ(RAM(ACTION_TIMER));
        RTS();
    }
    mff_cycles(0xD2B9, 1 + 3 + 2 + 2);
    a = RAM(PLAYER_ID);
    COMPARE(a, PLAYER_HAGGAR);
    if (z) {
        mff_cycles(0xD2C2, 1);
    } else {
        mff_cycles(0xD2C4, 3 + 2);
        a = RAM(GUY_JUMP_WINDOW);
        if (a != 0) {
            mff_cycles(0xD2C6, 1 + 5 + 4 + 3 + 3 + 2 + 2);
            RAM(GUY_JUMP_WINDOW)--;
            a = RAM(OBJ_Y);
            RAM(PLAYER_GROUND_Y) = a;
            a = RAM(JOY_PRESSED);
            COMPARE(a, PAD_B);
            if (!z) {
                mff_cycles(0xD2F7, 6);
                RTS();
            }
            mff_cycles(0xD2F5, 1 + 3 + 4);
            y = RAM(PLAYER_ID);
            a = prg_read((uint16_t)(WINDOW_ACTION + y));
            mff_cycles(0xD2FD, 3 + 4);
            RAM(GAME_ACTION) = a;
            a = prg_read((uint16_t)(WINDOW_NO_HIT + y));
            mff_cycles(0xD302, 3 + 2 + 4 + 4 + 3 + 6);
            RAM(PLAYER_NO_HIT) = a;
            a = STATE_LIVE;
            RAM(OBJ_STATE) = a;
            a = RAM(OBJ_Y);
            SET_NZ(a);
            RAM(PLAYER_GROUND_Y) = a;
            RTS();
        }
    }

    /* back to idle */
    mff_cycles(0xD2C8, 2 + 6 * 3 + 3 * 4 + 6);
    a = ACT_IDLE;
    SET_NZ(a);
    RAM(GAME_ACTION) = a;
    RAM(JUMP_MOVING) = a;
    RAM(COMBO_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(PLAYER_INVULN) = a;
    RAM(GRAB_HIT_COUNT) = a;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER2) = a;
    RAM(OBJ_GROUND_Y) = a;
    JSR_AT(0xD2DF, 0xCEAA, -1);                 /* player_stand */
    mff_cycles(0xD2E2, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(COMBO_FIRST_ANIM + y));
    SET_NZ(a);
    mff_cycles(0xD2E7, 3 + 6);
    RAM(PLAYER_STAND_ANIM) = a;
    RTS();
}

/*
 * player_state_0A ($D45A, labels.toml): game_action $0A, set by boss code in
 * bank $03 when it holds the player (03:B912, 03:BA11). The player only
 * stands (player_stand, $CEAA) each frame. ($D450-$D459 before it look like
 * code, attack_live, game_action and combo_timer cleared and a branch here,
 * but nothing reaches them.)
 */
void func_D45A(void) {
    REGS_IN;

    mff_cycles(0xD45A, 6);
    JSR_AT(0xD45A, 0xCEAA, -1);                 /* player_stand */
    mff_cycles(0xD45D, 6);
    RTS();
}

/*
 * player_state_continue ($DC6C, labels.toml): game_action 9, the continue
 * and game over screens. Sound command $F2, a palette fade ($C36E), one
 * frame, scroll to 0, rendering off, the sprites hidden ($C31D), every
 * object cleared, the nametable filled ($C2D1, A = $20, X = Y = 0), then
 * banks $04/$05:
 *  - with credits left, bank $04's function 3 ($8000, X = 3) and the
 *    continue screen ($9300); it returns with game_action 7 when the player
 *    continues (player_state_transition takes over). Otherwise (never
 *    executed in any run) the screen is cleared and banks $04/$05 mapped
 *    again for the game over;
 *  - the game over: function 5 and $9500.
 * Reached through player_state's table (entry 9), by JMP from bank $00
 * (00:8E36) and run on into by player_state_throw when the lives run out.
 */
void func_DC6C(void) {
    REGS_IN;

    mff_cycles(0xDC6C, 2 + 3 + 2 + 6);
    a = 0;
    RAM(RENDER_OFF_DEPTH) = a;
    a = 0xF2;
    SET_NZ(a);
    JSR_FN(0xDC72, func_F34A);                  /* sound_queue_push */
    mff_cycles(0xDC75, 6);
    JSR_AT(0xDC75, 0xC36E, -1);                 /* palette fade */
    mff_cycles(0xDC78, 6);
    JSR_YIELD(0xDC78, func_FF31);               /* task_wait_frame */
    mff_cycles(0xDC7B, 2 + 3 * 5 + 6);
    a = 0;
    SET_NZ(a);
    RAM(SCROLL_X) = a;
    RAM(SCROLL_Y) = a;
    RAM(SCROLL_NT) = a;
    RAM(SCROLL_Y_HI) = a;
    RAM(SCROLL_X_HI) = a;
    JSR_AT(0xDC87, 0xC265, -1);                 /* rendering_off */
    mff_cycles(0xDC8A, 2 + 3 + 6);
    a = 4;
    SET_NZ(a);
    RAM(OAM_INDEX) = a;
    JSR_AT(0xDC8E, 0xC31D, -1);                 /* hide the sprites */
    mff_cycles(0xDC91, 6);
    JSR_AT(0xDC91, 0xC334, -1);                 /* objects_clear_all */
    mff_cycles(0xDC94, 2 + 2 + 2 + 6);
    a = 0x20;
    x = 0;
    y = 0;
    SET_NZ(y);
    JSR_AT(0xDC9A, 0xC2D1, -1);                 /* fill the nametable */
    mff_cycles(0xDC9D, 6);
    JSR_AT(0xDC9D, 0xC31D, -1);                 /* hide the sprites */
    mff_cycles(0xDCA0, 2 + 3 + 2 + 3 + 6);
    x = 0x04;
    RAM(BANK_REQUEST) = x;
    x++;
    SET_NZ(x);
    RAM(BANK_REQUEST + 1) = x;
    JSR_FN(0xDCA7, func_FF4D);                  /* bank_switch */
    mff_cycles(0xDCAA, 3 + 2);
    a = RAM(CREDITS);
    if (a != 0) {
        mff_cycles(0xDCAE, 2 + 6);
        x = 3;
        SET_NZ(x);
        JSR_AT(0xDCB0, 0x8000, -1);             /* bank $04, function 3 */
        mff_cycles(0xDCB3, 6);
        JSR_AT(0xDCB3, 0xC26F, -1);             /* rendering_on */
        mff_cycles(0xDCB6, 6);
        JSR_AT(0xDCB6, 0x9300, -1);             /* the continue screen */
        mff_cycles(0xDCB9, 3 + 2 + 2);
        a = RAM(GAME_ACTION);
        COMPARE(a, ACT_SELECT);
        if (z) {                                /* continued */
            mff_cycles(0xDCBF, 6);
            RTS();
        }
        mff_cycles(0xDCBD, 1 + 2 + 3 + 6);
        a = 0;
        SET_NZ(a);
        RAM(RENDER_OFF_DEPTH) = a;
        JSR_AT(0xDCC4, 0xC85C, -1);             /* screen_clear */
        mff_cycles(0xDCC7, 2 + 3 + 2 + 3 + 6);
        x = 0x04;
        RAM(BANK_REQUEST) = x;
        x++;
        SET_NZ(x);
        RAM(BANK_REQUEST + 1) = x;
        JSR_FN(0xDCCE, func_FF4D);              /* bank_switch */
    } else {
        mff_cycles(0xDCAC, 1);
    }
    mff_cycles(0xDCD1, 2 + 6);                  /* game over */
    x = 5;
    SET_NZ(x);
    JSR_AT(0xDCD3, 0x8000, -1);                 /* bank $04, function 5 */
    mff_cycles(0xDCD6, 6);
    JSR_AT(0xDCD6, 0xC26F, -1);                 /* rendering_on */
    mff_cycles(0xDCD9, 6);
    JSR_AT(0xDCD9, 0x9500, -1);                 /* the game over screen */
    mff_cycles(0xDCDC, 6);
    RTS();
}

/*
 * player_state_throw ($DCDD, labels.toml): game_action $0B, two uses.
 *  - Cody's throw (cody_throw_start sets it for $19 frames): combo_timer 0,
 *    action_timer counts down; when it goes under 1, back to idle
 *    (game_action, jump_moving, attack_live, combo_timer, guy_jump_window,
 *    grab_hit_count, obj_timer[0] and obj_timer2[0] cleared, player_stand)
 *    and Cody turns back round (player_face_left and player_dir ^= 3; the
 *    other way never executed in any run); the combo restarts.
 *  - dying, with health negative (bank $00 sets game_action $0B with
 *    action_timer $78 at 00:910B): the player blinks (obj_flags bit 2 on
 *    even frames of action_timer, off on odd ones) while it counts down;
 *    then obj_state[0] $80 and a life goes: with lives left a new one drops
 *    in (game_action $25); with none, special_screen_id 0, sound $32 and
 *    the continue screen at once (game_action 9, player_state_continue).
 * player_state dispatches here through its table (entry $0B).
 */
void func_DCDD(void) {
    REGS_IN;

    mff_cycles(0xDCDD, 3 + 2);
    a = RAM(HEALTH);
    if (!(a & 0x80)) {
        mff_cycles(0xDCE1, 2 + 3 + 5 + 3 + 2);
        a = 0;
        RAM(COMBO_TIMER) = a;
        RAM(ACTION_TIMER)--;
        a = RAM(ACTION_TIMER);
        if (!(a & 0x80)) {
            mff_cycles(0xDCEB, 2 + 2);
            COMPARE(a, 1);
            if (c) {                            /* still throwing */
                mff_cycles(0xDCEF, 6);
                RTS();
            }
            mff_cycles(0xDCED, 1);
        } else {
            mff_cycles(0xDCE9, 1);
        }
        mff_cycles(0xDCF0, 2 + 6 * 3 + 2 * 4 + 6);
        a = ACT_IDLE;
        SET_NZ(a);
        RAM(GAME_ACTION) = a;
        RAM(JUMP_MOVING) = a;
        RAM(ATTACK_LIVE) = a;
        RAM(COMBO_TIMER) = a;
        RAM(GUY_JUMP_WINDOW) = a;
        RAM(GRAB_HIT_COUNT) = a;
        RAM(OBJ_TIMER) = a;
        RAM(OBJ_TIMER2) = a;
        JSR_AT(0xDD04, 0xCEAA, -1);             /* player_stand */
        mff_cycles(0xDD07, 3 + 2 + 2);
        a = RAM(PLAYER_ID);
        COMPARE(a, PLAYER_GUY);
        if (z) {
            mff_cycles(0xDD0B, 1);
        } else {
            mff_cycles(0xDD0D, 2 + 2);
            COMPARE(a, PLAYER_HAGGAR);
            if (z) {
                mff_cycles(0xDD0F, 1);
            } else {                            /* Cody turns back round */
                mff_cycles(0xDD11, 3 + 2 + 2);
                a = RAM(PLAYER_DIR) & PAD_RIGHT;
                SET_NZ(a);
                if (!z) {
                    mff_cycles(0xDD17, 6);
                    JSR_FN(0xDD17, func_F048);  /* player_face_left */
                    mff_cycles(0xDD1A, 2);
                    if (z) {                    /* never: A = obj_flags & $BF */
                        mff_cycles(0xDD1C, 6);
                        JSR_FN(0xDD1C, func_F03F);
                    } else {
                        mff_cycles(0xDD1A, 1);
                    }
                } else {
                    mff_cycles(0xDD15, 1 + 6);
                    JSR_FN(0xDD1C, func_F03F);  /* player_face_right */
                }
                mff_cycles(0xDD1F, 3 + 2 + 3);
                a = RAM(PLAYER_DIR) ^ (PAD_LEFT | PAD_RIGHT);
                RAM(PLAYER_DIR) = a;
            }
        }
        mff_cycles(0xDD25, 3 + 4);
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(COMBO_FIRST_ANIM + y));
        SET_NZ(a);
        mff_cycles(0xDD2A, 3 + 6);
        RAM(PLAYER_STAND_ANIM) = a;
        RTS();
    }

    mff_cycles(0xDCDF, 2 + 3 + 2);              /* dying */
    a = RAM(ACTION_TIMER);
    if (a != 0) {
        mff_cycles(0xDD31, 2 + 2);
        a &= 0x01;
        if (a == 0) {
            mff_cycles(0xDD35, 4 + 2 + 4 + 2 + 1);
            a = RAM(OBJ_FLAGS) | 0x04;          /* hidden */
            RAM(OBJ_FLAGS) = a;
        } else {
            mff_cycles(0xDD33, 1 + 4 + 2 + 4);
            a = RAM(OBJ_FLAGS) & ~0x04;         /* shown */
            RAM(OBJ_FLAGS) = a;
        }
        mff_cycles(0xDD47, 5 + 6);
        RAM(ACTION_TIMER)--;
        SET_NZ(RAM(ACTION_TIMER));
        RTS();
    }
    mff_cycles(0xDD2F, 1 + 2 + 4 + 5 + 3 + 2);
    a = STATE_LIVE;
    RAM(OBJ_STATE) = a;
    RAM(LIVES)--;
    a = RAM(LIVES);
    if (!(a & 0x80)) {
        mff_cycles(0xDD53, 1 + 2 + 3 + 6);
        a = ACT_DROP_IN;
        SET_NZ(a);
        RAM(GAME_ACTION) = a;
        RTS();
    }
    mff_cycles(0xDD55, 2 + 3 + 2 + 6);          /* no lives left */
    a = 0;
    RAM(SPECIAL_SCREEN_ID) = a;
    a = 0x32;
    SET_NZ(a);
    JSR_FN(0xDD5B, func_F34C);                  /* sound_queue_push_sfx */
    mff_cycles(0xDD5E, 2 + 3 + 3);
    a = ACT_CONTINUE;
    RAM(GAME_ACTION) = a;
    FALL_INTO(func_DC6C);                       /* player_state_continue */
}

/* the health of each level, per character */
#define HEALTH_TABLE_LO     0xDE46   /* 4C 52 58 */
#define HEALTH_TABLE_HI     0xDE49   /* DE DE DE: Cody $DE4C, Guy $DE52, Haggar $DE58 */

/*
 * player_health_reset ($DE33, proposed): health = the health of
 * player_level for this character (Cody 40 48 50 58 60 68, Guy 30 38 40 48
 * 50 58, Haggar 50 58 60 68 68 68). Called at a round's start (JSR at
 * 0E:DF9E, DFDF) and from bank $00 (00:8E6C).
 * Out: A = health; $02/$03 = the character's table.
 */
void func_DE33(void) {
    REGS_IN;

    mff_cycles(0xDE33, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(HEALTH_TABLE_LO + y));
    mff_cycles(0xDE38, 3 + 4);
    RAM(TMP2) = a;
    a = prg_read((uint16_t)(HEALTH_TABLE_HI + y));
    mff_cycles(0xDE3D, 3 + 3 + 5);
    RAM(TMP3) = a;
    y = RAM(PLAYER_LEVEL);
    a = mem_read((uint16_t)(RAM16(TMP2) + y));
    SET_NZ(a);
    mff_cycles(0xDE43, 3 + 6);
    RAM(HEALTH) = a;
    RTS();
}
