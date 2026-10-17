/*
 * player_actions.c - the player's actions in bank $00: player_action_dispatch
 * and the actions that are neither specials nor throws.
 *
 * gameplay_update calls player_state (fixed bank) and then, with banks
 * $00/$01 mapped, player_action_dispatch ($8000) every frame. The fixed-bank
 * states handle game_action 0-$0E (their bank-$00 entries are a bare RTS)
 * and the bank-$00 actions start at $0F. Some entries have no routine in
 * the port (they run on the interpreter): $10 $80C5, $11 $817E (its three
 * handlers are here), $16 $86FF, $18 $891B, $1A $914D, $1E $8FF8,
 * $22 $8A31, $23 $8CC4, $26 $91BE, $27 $8EEA and $29 $92BA.
 */
#include "player_defs.h"

/* game_action -> action routine, split table in bank $00 */
#define ACTION_LO           0x8011
#define ACTION_HI           0x803B

/*
 * player_action_dispatch ($8000 in bank $00, labels.toml): X = 0, then JMP
 * ($0000) through $8011/$803B[game_action] (42 entries). Called every
 * gameplay frame (JSR at 0E:C948, banks $00/$01 mapped).
 * Out: $00/$01 = the action, A = its high byte, Y = game_action.
 */
void func_8000_b0(void) {
    REGS_IN;

    mff_cycles(0x8000, 2 + 3 + 4);
    x = 0;
    y = RAM(GAME_ACTION);
    a = prg_read((uint16_t)(ACTION_LO + y));
    mff_cycles(0x8007, 3 + 4);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(ACTION_HI + y));
    SET_NZ(a);
    mff_cycles(0x800C, 3 + 5);
    RAM(TMP1) = a;
    JMP_IND(TMP0);
}

/* $8065 (bank $00): the action of game_action 0-$0E and $28, an RTS */
void func_8065_b0(void) {
    REGS_IN;

    mff_cycles(0x8065, 6);
    RTS();
}

/*
 * action_earthquake ($8066 in bank $00, proposed; Data Crystal: HeadSmasher
 * earthquake): game_action $0F, after Haggar's HeadSmasher lands. The screen
 * shakes (scroll_y one up on even frames of obj_timer[0], one down on odd
 * ones) and action_quake_bounce follows.
 */
void func_8066_b0(void) {
    REGS_IN;

    mff_cycles(0x8066, 4 + 2 + 2);
    a = RAM(OBJ_TIMER) & 0x01;
    if (a == 0) {
        mff_cycles(0x806D, 3 + 2 + 2 + 3 + 3);
        a = RAM(SCROLL_Y);
        c = 1;
        SBC(1);
        RAM(SCROLL_Y) = a;
        JMP_TAIL(0x807E, 0);                    /* action_quake_bounce */
    }
    mff_cycles(0x806B, 1 + 3 + 2 + 2 + 3);
    a = RAM(SCROLL_Y);
    c = 0;
    ADC(1);
    RAM(SCROLL_Y) = a;
    FALL_INTO(func_807E_b0);
}

/*
 * action_quake_bounce ($807E in bank $00, proposed): obj_timer[0] counts
 * down and Haggar bounces back, away from his facing side (obj_move_left
 * when facing right, obj_move_right otherwise), then action_quake_fall.
 * Reached by JMP from action_earthquake (00:8074) and run on into from it.
 */
void func_807E_b0(void) {
    REGS_IN;

    mff_cycles(0x807E, 6 + 3 + 2 + 2);
    RAM(OBJ_TIMER)--;
    a = RAM(PLAYER_DIR) & PAD_RIGHT;
    SET_NZ(a);
    if (!z) {
        mff_cycles(0x8087, 6);
        JSR_FN(0x8087, func_F1A1);              /* obj_move_left */
        mff_cycles(0x808A, 3);
        JMP_TAIL(0x8090, 0);                    /* action_quake_fall */
    }
    mff_cycles(0x8085, 1 + 6);
    JSR_FN(0x808D, func_F183);                  /* obj_move_right */
    FALL_INTO(func_8090_b0);
}

/*
 * action_quake_fall ($8090 in bank $00, proposed): obj_fall (Y = 2); once
 * falling and more than 2 px below player_ground_y, Haggar lands on it:
 * sound $1C, scroll_y and jump_moving 0, game_action 2 for 3 frames,
 * player_stand. (obj_fall reporting the floor would JMP into the middle of
 * this routine, $80AE; never executed in any run.) Reached by JMP from
 * action_quake_bounce (00:808A) and run on into from it.
 */
void func_8090_b0(void) {
    REGS_IN;

    mff_cycles(0x8090, 2 + 6);
    y = 2;
    SET_NZ(y);
    JSR_FN(0x8092, func_F119);                  /* obj_fall */
    mff_cycles(0x8095, 2);
    if (c) {
        mff_cycles(0x8097, 3);
        JMP_TAIL(0x80AE, 0);
    }
    mff_cycles(0x8095, 1 + 4 + 2);
    a = RAM(OBJ_VY);
    SET_NZ(a);
    if (n) {                                    /* falling */
        mff_cycles(0x809F, 3 + 2 + 2 + 4 + 2);
        a = RAM(PLAYER_GROUND_Y);
        c = 0;
        ADC(2);
        COMPARE(a, RAM(OBJ_Y));
        if (!c) {                               /* past the ground line */
            mff_cycles(0x80A9, 3 + 4 + 2 + 6);
            a = RAM(PLAYER_GROUND_Y);
            RAM(OBJ_Y) = a;
            a = SFX_LAND;
            SET_NZ(a);
            JSR_FN(0x80B0, func_F34C);          /* sound_queue_push_sfx */
            mff_cycles(0x80B3, 2 + 3 + 3 + 2 + 3 + 2 + 3 + 6);
            a = 0;
            RAM(SCROLL_Y) = a;
            RAM(JUMP_MOVING) = a;
            a = ACT_LAND;
            RAM(GAME_ACTION) = a;
            a = 3;
            SET_NZ(a);
            RAM(ACTION_TIMER) = a;
            JSR_AT(0x80C1, 0xCEAA, 0);          /* player_stand */
        } else {
            mff_cycles(0x80A7, 1);
        }
    } else {
        mff_cycles(0x809D, 1);
    }
    mff_cycles(0x80C4, 6);
    RTS();
}

/* ---- game_action $11, attacking with a picked-up weapon: $817E (no
 * routine in the port) JMPs ($0000) to these by player_id ---- */

/* the thrown weapon's x offset from the player, by player_dir & 2 */
#define CODY_KNIFE_DX       0x8219   /* $0012 right, $FFEE left */
#define GUY_THROW_DX        0x82AB   /* $0008 right, $FFF8 left */

/*
 * cody_weapon ($8193 in bank $00, proposed; never executed in any run): on
 * frame 2 at timer 0 the weapon is thrown from the first free slot of 2 and
 * 1 (none free: nothing): obj_dir = player_dir, 18 px ahead and 4 px below
 * the player, sound $1B, obj_spawn ($F2A0) with animation $4D, id $6C (also
 * obj_saved_id), attack_id $0A, obj_attack and obj_throw_flag 0. On frame 9
 * at timer 2 the weapon is gone (weapon_held 0) and the player back to idle
 * (game_action 0, the combo restarted, player_stand).
 */
void func_8193_b0(void) {
    REGS_IN;

    mff_cycles(0x8193, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    if (a == 2) {
        mff_cycles(0x819A, 4 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        if (a == 0) {
            mff_cycles(0x819F, 2);
            y = 2;
            for (;;) {                          /* a free slot, 2 then 1 */
                mff_cycles(0x81A1, 4 + 2);
                a = RAM(OBJ_STATE + y);
                if (!(a & 0x80))
                    break;
                mff_cycles(0x81A6, 2 + 2 + 2);
                y--;
                COMPARE(y, 0);
                if (z) {
                    mff_cycles(0x81AB, 6);
                    RTS();
                }
                mff_cycles(0x81A9, 1);
            }
            mff_cycles(0x81A4, 1 + 3 + 3 + 5 + 2 + 2 + 4 + 2 + 4);
            RAM(TMP15) = y;
            a = RAM(PLAYER_DIR);
            RAM(OBJ_DIR + y) = a;
            a &= PAD_LEFT;
            y = a;
            a = RAM(OBJ_X);
            c = 0;
            ADC(prg_read((uint16_t)(CODY_KNIFE_DX + y)));
            mff_cycles(0x81BD, 3 + 4 + 4);
            mff_push(a);
            a = RAM(OBJ_X_HI);
            ADC(prg_read((uint16_t)(CODY_KNIFE_DX + 1 + y)));
            mff_cycles(0x81C4, 3 + 5 + 4 + 5 + 4 + 2 + 2 + 5 + 4 + 5 + 2 + 6);
            y = RAM(TMP15);
            RAM(OBJ_X_HI + y) = a;
            a = mff_pull();
            RAM(OBJ_X + y) = a;
            a = RAM(OBJ_Y);
            c = 0;
            ADC(4);
            RAM(OBJ_Y + y) = a;
            a = RAM(OBJ_Y_HI);
            RAM(OBJ_Y_HI + y) = a;
            a = 0x1B;
            SET_NZ(a);
            JSR_FN(0x81DE, func_F34C);          /* sound_queue_push_sfx */
            mff_cycles(0x81E1, 2 + 6);
            a = 0x4D;
            SET_NZ(a);
            JSR_FN(0x81E3, func_F2A0);          /* obj_spawn: slot Y, animation A */
            mff_cycles(0x81E6, 2 + 5 + 5 + 2 + 3 + 2 + 5 + 5);
            a = 0x6C;
            RAM(OBJ_ID + y) = a;
            RAM(OBJ_SAVED_ID + y) = a;
            a = 0x0A;
            RAM(ATTACK_ID) = a;
            a = 0;
            RAM(OBJ_ATTACK + y) = a;
            RAM(OBJ_THROW_FLAG + y) = a;
        } else {
            mff_cycles(0x819D, 1);
        }
    } else {
        mff_cycles(0x8198, 1);
    }
    mff_cycles(0x81FA, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 9);
    if (z) {
        mff_cycles(0x8201, 4 + 2 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        COMPARE(a, 2);
        if (z) {
            mff_cycles(0x8208, 2 + 3 + 3 + 3 + 4);
            a = 0;
            RAM(WEAPON_HELD) = a;
            RAM(GAME_ACTION) = a;
            y = RAM(PLAYER_ID);
            a = prg_read((uint16_t)(COMBO_FIRST_ANIM + y));
            SET_NZ(a);
            mff_cycles(0x8213, 3 + 6);
            RAM(PLAYER_STAND_ANIM) = a;
            JSR_AT(0x8215, 0xCEAA, 0);          /* player_stand */
        } else {
            mff_cycles(0x8206, 1);
        }
    } else {
        mff_cycles(0x81FF, 1);
    }
    mff_cycles(0x8218, 6);
    RTS();
}

/*
 * guy_weapon ($821D in bank $00, proposed): as cody_weapon with 8 px ahead,
 * animation $5F and id $6D; on frame 9 at timer 2 weapon_ammo counts down
 * and only when it goes negative is the weapon gone (never executed).
 */
void func_821D_b0(void) {
    REGS_IN;

    mff_cycles(0x821D, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    if (a == 2) {
        mff_cycles(0x8224, 4 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        if (a == 0) {
            mff_cycles(0x8229, 2);
            y = 2;
            for (;;) {                          /* a free slot, 2 then 1 */
                mff_cycles(0x822B, 4 + 2);
                a = RAM(OBJ_STATE + y);
                if (!(a & 0x80))
                    break;
                mff_cycles(0x8230, 2 + 2 + 2);
                y--;
                COMPARE(y, 0);
                if (z) {
                    mff_cycles(0x8235, 6);
                    RTS();
                }
                mff_cycles(0x8233, 1);
            }
            mff_cycles(0x822E, 1 + 3 + 3 + 5 + 2 + 2 + 4 + 2 + 4);
            RAM(TMP15) = y;
            a = RAM(PLAYER_DIR);
            RAM(OBJ_DIR + y) = a;
            a &= PAD_LEFT;
            y = a;
            a = RAM(OBJ_X);
            c = 0;
            ADC(prg_read((uint16_t)(GUY_THROW_DX + y)));
            mff_cycles(0x8247, 3 + 4 + 4);
            mff_push(a);
            a = RAM(OBJ_X_HI);
            ADC(prg_read((uint16_t)(GUY_THROW_DX + 1 + y)));
            mff_cycles(0x824E, 3 + 5 + 4 + 5 + 4 + 2 + 2 + 5 + 4 + 5 + 2 + 6);
            y = RAM(TMP15);
            RAM(OBJ_X_HI + y) = a;
            a = mff_pull();
            RAM(OBJ_X + y) = a;
            a = RAM(OBJ_Y);
            c = 0;
            ADC(4);
            RAM(OBJ_Y + y) = a;
            a = RAM(OBJ_Y_HI);
            RAM(OBJ_Y_HI + y) = a;
            a = 0x1B;
            SET_NZ(a);
            JSR_FN(0x8268, func_F34C);          /* sound_queue_push_sfx */
            mff_cycles(0x826B, 2 + 6);
            a = 0x5F;
            SET_NZ(a);
            JSR_FN(0x826D, func_F2A0);          /* obj_spawn */
            mff_cycles(0x8270, 2 + 5 + 5 + 2 + 3 + 2 + 5 + 5);
            a = 0x6D;
            RAM(OBJ_ID + y) = a;
            RAM(OBJ_SAVED_ID + y) = a;
            a = 0x0A;
            RAM(ATTACK_ID) = a;
            a = 0;
            RAM(OBJ_ATTACK + y) = a;
            RAM(OBJ_THROW_FLAG + y) = a;
        } else {
            mff_cycles(0x8227, 1);
        }
    } else {
        mff_cycles(0x8222, 1);
    }
    mff_cycles(0x8284, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 9);
    if (z) {
        mff_cycles(0x828B, 4 + 2 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        COMPARE(a, 2);
        if (z) {
            mff_cycles(0x8292, 5 + 3 + 2);
            RAM(WEAPON_AMMO)--;
            a = RAM(WEAPON_AMMO);
            if (a & 0x80) {                     /* out of throws */
                mff_cycles(0x8298, 2 + 3);
                a = 0;
                RAM(WEAPON_HELD) = a;
            } else {
                mff_cycles(0x8296, 1);
            }
            mff_cycles(0x829C, 2 + 3 + 3 + 4);
            a = 0;
            RAM(GAME_ACTION) = a;
            y = RAM(PLAYER_ID);
            a = prg_read((uint16_t)(COMBO_FIRST_ANIM + y));
            SET_NZ(a);
            mff_cycles(0x82A5, 3 + 6);
            RAM(PLAYER_STAND_ANIM) = a;
            JSR_AT(0x82A7, 0xCEAA, 0);          /* player_stand */
        } else {
            mff_cycles(0x8290, 1);
        }
    } else {
        mff_cycles(0x8289, 1);
    }
    mff_cycles(0x82AA, 6);
    RTS();
}

/*
 * haggar_weapon ($82AF in bank $00, proposed; never executed in any run): a
 * swing. Sound $1D on frame 0, attack_id $0E and attack_begin on frame 6 at
 * timer 0, attack_end on frame 9 at timer 0; on frame $0C back to idle
 * (game_action 0, the combo restarted) with the standing animation, $44
 * with the weapon or $03 without, and CHR bank 8.
 */
void func_82AF_b0(void) {
    REGS_IN;

    mff_cycles(0x82AF, 4 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    if (a == 0) {
        mff_cycles(0x82B4, 2 + 6);
        a = 0x1D;
        SET_NZ(a);
        JSR_FN(0x82B6, func_F34C);              /* sound_queue_push_sfx */
    } else {
        mff_cycles(0x82B2, 1);
    }
    mff_cycles(0x82B9, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 6);
    if (z) {
        mff_cycles(0x82C0, 4 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        if (a == 0) {
            mff_cycles(0x82C5, 2 + 3 + 6);
            a = 0x0E;
            SET_NZ(a);
            RAM(ATTACK_ID) = a;
            JSR_FN(0x82C9, func_F43B);          /* attack_begin */
        } else {
            mff_cycles(0x82C3, 1);
        }
    } else {
        mff_cycles(0x82BE, 1);
    }
    mff_cycles(0x82CC, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 9);
    if (z) {
        mff_cycles(0x82D3, 4 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        SET_NZ(a);
        if (z) {
            mff_cycles(0x82D8, 6);
            JSR_FN(0x82D8, func_F436);          /* attack_end */
        } else {
            mff_cycles(0x82D6, 1);
        }
    } else {
        mff_cycles(0x82D1, 1);
    }
    mff_cycles(0x82DB, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 0x0C);
    if (z) {
        mff_cycles(0x82E2, 2 + 3 + 3 + 4);
        a = 0;
        RAM(GAME_ACTION) = a;
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(COMBO_FIRST_ANIM + y));
        mff_cycles(0x82EB, 3 + 3 + 2);
        RAM(PLAYER_STAND_ANIM) = a;
        a = RAM(WEAPON_HELD);
        if (a == 0) {
            mff_cycles(0x82F1, 2 + 2 + 1);
            a = 0x03;
        } else {
            mff_cycles(0x82EF, 1 + 2);
            a = 0x44;
        }
        SET_NZ(a);
        mff_cycles(0x82F7, 6);
        JSR_FN(0x82F7, func_F294);              /* player_set_anim */
        mff_cycles(0x82FA, 2 + 6);
        a = 0x08;
        SET_NZ(a);
        JSR_FN(0x82FC, func_F440);              /* player_set_chr */
    } else {
        mff_cycles(0x82E0, 1);
    }
    mff_cycles(0x82FF, 6);
    RTS();
}

/* ---- losing a life and the next one ---- */

/*
 * action_pit_fall ($8D8D in bank $00, proposed): game_action $24, falling
 * into a pit (player_pit_fall $CFD1 and obj_fall set it). In round 4's
 * second area the first step sends the player away from the screen's
 * middle (obj_dir 2 left of x $80, 1 from it) at walking speed, and every
 * step after he drifts that way. Then pit_fall_down.
 */
void func_8D8D_b0(void) {
    REGS_IN;

    mff_cycles(0x8D8D, 4 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x0F;
    if (a == 0) {
        mff_cycles(0x8D94, 3 + 2 + 2);
        a = RAM(ROUND);
        COMPARE(a, 3);
        if (!z) {
            mff_cycles(0x8D98, 1);
            FALL_INTO(func_8DD1_b0);            /* pit_fall_down */
        }
        mff_cycles(0x8D9A, 3 + 2 + 2);
        a = RAM(SUB_AREA);
        COMPARE(a, 1);
        if (!z) {
            mff_cycles(0x8D9E, 1);
            FALL_INTO(func_8DD1_b0);            /* pit_fall_down */
        }
        mff_cycles(0x8DA0, 4 + 2 + 2);
        a = RAM(OBJ_X);
        COMPARE(a, 0x80);
        if (c) {
            mff_cycles(0x8DA5, 1 + 2);
            a = PAD_RIGHT;
        } else {
            mff_cycles(0x8DA7, 2 + 2 + 1);
            a = PAD_LEFT;
        }
        mff_cycles(0x8DAD, 4 + 3 + 4);
        RAM(OBJ_DIR) = a;
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(WALK_VX_FRAC + y));
        mff_cycles(0x8DB5, 4 + 4);
        RAM(OBJ_VX_FRAC) = a;
        a = prg_read((uint16_t)(WALK_VX + y));
        mff_cycles(0x8DBB, 4 + 6);
        RAM(OBJ_VX) = a;
        RAM(OBJ_STATE)++;
    } else {
        mff_cycles(0x8D92, 1);
    }
    mff_cycles(0x8DC1, 4 + 2 + 2);
    a = RAM(OBJ_DIR) & PAD_RIGHT;
    SET_NZ(a);
    if (!z) {
        mff_cycles(0x8DC8, 6);
        JSR_FN(0x8DC8, func_F183);              /* obj_move_right */
        mff_cycles(0x8DCB, 3);
        JMP_TAIL(0x8DD1, 0);                    /* pit_fall_down */
    }
    mff_cycles(0x8DC6, 1 + 6);
    JSR_FN(0x8DCE, func_F1A1);                  /* obj_move_left */
    FALL_INTO(func_8DD1_b0);
}

/*
 * pit_fall_down ($8DD1 in bank $00, proposed): the player falls ($F1FD,
 * without the floor probe) in the falling animation and the jump's CHR
 * bank; from y $A8 down the sound $28 plays and the sprite is hidden
 * (obj_flags bit 2). guy_jump_window, scroll_y, jump_moving, combo_timer,
 * attack_live, player_invuln and slot 0's obj_timer, obj_timer2 and
 * obj_ground_y go to 0, attack_end. From y $E4 down a life is lost
 * (obj_state[0] $80): with lives left a new one drops in (game_action
 * $25), without the continue screen comes at once (special_screen_id 0,
 * game_action 9, JMP player_state_continue). Reached by JMP from
 * action_pit_fall (00:8DCB) and run on into from it.
 */
void func_8DD1_b0(void) {
    REGS_IN;

    mff_cycles(0x8DD1, 6);
    JSR_FN(0x8DD1, func_F1FD);                  /* obj_fall, no probe */
    /* a test with no effect: both ways go on at $8DE0 (Haggar letting go of
     * an enemy, action_before_fall 4, may have been meant) */
    mff_cycles(0x8DD4, 3 + 2 + 2);
    a = RAM(ACTION_BEFORE_FALL);
    COMPARE(a, ACT_GRAB);
    if (z) {
        mff_cycles(0x8DDA, 3 + 2 + 2);
        a = RAM(PLAYER_ID);
        COMPARE(a, PLAYER_HAGGAR);
        if (!z)
            mff_cycles(0x8DDE, 1);
    } else {
        mff_cycles(0x8DD8, 1);
    }
    mff_cycles(0x8DE0, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(ANIM_FALL + y));
    SET_NZ(a);
    mff_cycles(0x8DE5, 6);
    JSR_FN(0x8DE5, func_F294);                  /* player_set_anim */
    mff_cycles(0x8DE8, 4);
    a = prg_read((uint16_t)(CHR_JUMP + y));
    SET_NZ(a);
    mff_cycles(0x8DEB, 6);
    JSR_FN(0x8DEB, func_F440);                  /* player_set_chr */
    mff_cycles(0x8DEE, 4 + 2 + 2);
    a = RAM(OBJ_Y);
    COMPARE(a, 0xA8);
    if (c) {
        mff_cycles(0x8DF5, 2 + 6);
        a = SFX_KNOCKDOWN;
        SET_NZ(a);
        JSR_FN(0x8DF7, func_F34C);              /* sound_queue_push_sfx */
        mff_cycles(0x8DFA, 4 + 2 + 4);
        a = RAM(OBJ_FLAGS) | 0x04;              /* hidden */
        RAM(OBJ_FLAGS) = a;
    } else {
        mff_cycles(0x8DF3, 2);
    }
    mff_cycles(0x8E02, 2 + 6 * 3 + 3 * 4 + 6);
    a = 0;
    SET_NZ(a);
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(SCROLL_Y) = a;
    RAM(JUMP_MOVING) = a;
    RAM(COMBO_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(PLAYER_INVULN) = a;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER2) = a;
    RAM(OBJ_GROUND_Y) = a;
    JSR_FN(0x8E19, func_F436);                  /* attack_end */
    mff_cycles(0x8E1C, 4 + 2 + 2);
    a = RAM(OBJ_Y);
    COMPARE(a, 0xE4);
    if (c) {
        mff_cycles(0x8E23, 2 + 4 + 5 + 3 + 2);
        a = STATE_LIVE;
        RAM(OBJ_STATE) = a;
        RAM(LIVES)--;
        a = RAM(LIVES);
        if (a & 0x80) {
            mff_cycles(0x8E2E, 2 + 3 + 2 + 3 + 3);
            a = 0;
            RAM(SPECIAL_SCREEN_ID) = a;
            a = ACT_CONTINUE;
            SET_NZ(a);
            RAM(GAME_ACTION) = a;
            JMP_TAIL(0xDC6C, -1);               /* player_state_continue */
        }
        mff_cycles(0x8E2C, 1 + 2 + 3);
        a = ACT_DROP_IN;
        SET_NZ(a);
        RAM(GAME_ACTION) = a;
    } else {
        mff_cycles(0x8E21, 1);
    }
    mff_cycles(0x8E3D, 6);
    RTS();
}

/*
 * action_drop_in ($8E3E in bank $00, proposed; Data Crystal: falling,
 * invincible): game_action $25, a new life. The first step puts the player
 * high on the screen (y $10, x $38 from the camera's left edge; when
 * scroll_x is 0 only obj_x is set), refills health (player_health_reset),
 * the falling animation and CHR bank, obj_vy $0580, obj_flags $D0. Every
 * step he comes down (obj_move_down); at y $88 (round 2: $98, never
 * executed) he lands: facing right, respawn_timer2 $B6 and respawn_timer1
 * $1E (the blinking invincibility), obj_state[0] and player_no_hit $80, the
 * player's state cleared (game_action, jump_moving, scroll_y, obj_timer,
 * obj_timer2, grab_hit_count, attack_live, weapon_held, combo_timer,
 * weapon_ammo, guy_jump_window), player_stand, a fall started ($F26E) and
 * the combo restarted (special_screen_id $B5 would count up; never
 * executed).
 */
void func_8E3E_b0(void) {
    REGS_IN;

    mff_cycles(0x8E3E, 4 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x0F;
    SET_NZ(a);
    if (z) {
        mff_cycles(0x8E45, 3 + 2);
        a = RAM(SCROLL_X);
        if (a == 0) {
            mff_cycles(0x8E49, 2 + 4 + 2 + 1);
            a = 0x38;
            RAM(OBJ_X) = a;
        } else {
            mff_cycles(0x8E47, 1 + 3 + 2 + 2 + 4 + 3 + 2 + 4);
            a = RAM(SCROLL_X);
            c = 0;
            ADC(0x38);
            RAM(OBJ_X) = a;
            a = RAM(SCROLL_X_HI);
            ADC(0);
            RAM(OBJ_X_HI) = a;
        }
        mff_cycles(0x8E5F, 2 + 4 + 2 + 4 + 4 + 6);
        a = 0x10;
        RAM(OBJ_Y) = a;
        a = 0;
        SET_NZ(a);
        RAM(OBJ_Y_HI) = a;
        RAM(OBJ_ID) = a;
        JSR_AT(0x8E6C, 0xDE33, 0);              /* player_health_reset */
        mff_cycles(0x8E6F, 3 + 4);
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(ANIM_FALL + y));
        SET_NZ(a);
        mff_cycles(0x8E74, 6);
        JSR_FN(0x8E74, func_F294);              /* player_set_anim */
        mff_cycles(0x8E77, 4);
        a = prg_read((uint16_t)(CHR_JUMP + y));
        SET_NZ(a);
        mff_cycles(0x8E7A, 6);
        JSR_FN(0x8E7A, func_F440);              /* player_set_chr */
        mff_cycles(0x8E7D, 6 + 2 + 4 + 2 + 4 + 2 + 4);
        RAM(OBJ_STATE)++;
        a = 0x80;
        RAM(OBJ_VY_FRAC) = a;
        a = 0x05;
        RAM(OBJ_VY) = a;
        a = FLAGS_FACING_RIGHT;
        SET_NZ(a);
        RAM(OBJ_FLAGS) = a;
    } else {
        mff_cycles(0x8E43, 1);
    }
    mff_cycles(0x8E8F, 6);
    JSR_FN(0x8E8F, func_F1BF);                  /* obj_move_down */
    mff_cycles(0x8E92, 3 + 2 + 2);
    a = RAM(ROUND);
    if (a == 1) {
        mff_cycles(0x8E98, 2 + 2 + 1);
        a = 0x98;
    } else {
        mff_cycles(0x8E96, 1 + 2);
        a = 0x88;
    }
    mff_cycles(0x8E9E, 3 + 4 + 3 + 2);
    RAM(TMP0) = a;
    a = RAM(OBJ_Y);
    COMPARE(a, RAM(TMP0));
    if (c) {                                    /* landed */
        mff_cycles(0x8EA7, 2 + 3 + 2 + 3 + 2 + 3 + 2 + 4 + 3 + 2 + 9 * 3 + 2 * 4 + 6);
        a = PAD_RIGHT;
        RAM(PLAYER_DIR) = a;
        a = 0xB6;
        RAM(RESPAWN_TIMER2) = a;
        a = 0x1E;
        RAM(RESPAWN_TIMER1) = a;
        a = STATE_LIVE;
        RAM(OBJ_STATE) = a;
        RAM(PLAYER_NO_HIT) = a;
        a = 0;
        SET_NZ(a);
        RAM(GAME_ACTION) = a;
        RAM(JUMP_MOVING) = a;
        RAM(SCROLL_Y) = a;
        RAM(OBJ_TIMER) = a;
        RAM(OBJ_TIMER2) = a;
        RAM(GRAB_HIT_COUNT) = a;
        RAM(ATTACK_LIVE) = a;
        RAM(WEAPON_HELD) = a;
        RAM(COMBO_TIMER) = a;
        RAM(WEAPON_AMMO) = a;
        RAM(GUY_JUMP_WINDOW) = a;
        JSR_AT(0x8ED4, 0xCEAA, 0);              /* player_stand */
        mff_cycles(0x8ED7, 6);
        JSR_FN(0x8ED7, func_F26E);              /* the start of a fall */
        mff_cycles(0x8EDA, 3 + 4);
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(COMBO_FIRST_ANIM + y));
        mff_cycles(0x8EDF, 3 + 3 + 2 + 2);
        RAM(PLAYER_STAND_ANIM) = a;
        a = RAM(SPECIAL_SCREEN_ID);
        COMPARE(a, 0xB5);
        if (z) {
            mff_cycles(0x8EE7, 5);
            RAM(SPECIAL_SCREEN_ID)++;
            SET_NZ(RAM(SPECIAL_SCREEN_ID));
        } else {
            mff_cycles(0x8EE5, 1);
        }
    } else {
        mff_cycles(0x8EA5, 1);
    }
    mff_cycles(0x8EE9, 6);
    RTS();
}
