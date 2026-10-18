/*
 * player_actions.c - the player's actions in bank $00: player_action_dispatch
 * and the actions that are neither specials nor throws.
 *
 * gameplay_update calls player_state (fixed bank) and then, with banks
 * $00/$01 mapped, player_action_dispatch ($8000) every frame. The fixed-bank
 * states handle game_action 0-$0E (their bank-$00 entries are a bare RTS)
 * and the bank-$00 actions start at $0F. Here: $0F the earthquake, $11 the
 * weapons, $1D/$1E the camera's walks, $1F lying down, $24 the pit fall and
 * $25 the drop-in. player_specials.c has $12, $14, $16 and $17,
 * player_throws.c $13, $15 and $21, player_power.c $10, $18, $22, $23 and
 * $27, player_exits.c $19, $1A, $1B, $1C, $20, $26 and $29.
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
    a = prg_read_ix(ACTION_LO, y);
    mff_cycles(0x8007, 3 + 4);
    RAM(TMP0) = a;
    a = prg_read_ix(ACTION_HI, y);
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
 * falling and more than 2 px below player_ground_y, Haggar is put on it and
 * lands (action_quake_land); obj_fall reporting the floor lands him where
 * he is. Reached by JMP from action_quake_bounce (00:808A) and run on into
 * from it.
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
        JMP_TAIL(0x80AE, 0);                    /* action_quake_land */
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
            mff_cycles(0x80A9, 3 + 4);
            a = RAM(PLAYER_GROUND_Y);
            RAM(OBJ_Y) = a;
            FALL_INTO(func_80AE_b0);            /* action_quake_land */
        }
        mff_cycles(0x80A7, 1);
    } else {
        mff_cycles(0x809D, 1);
    }
    mff_cycles(0x80C4, 6);
    RTS();
}

/*
 * action_quake_land ($80AE in bank $00, proposed): Haggar lands after the
 * earthquake: sound $1C, scroll_y and jump_moving 0, game_action 2 for 3
 * frames, player_stand. Reached by JMP from action_quake_fall when obj_fall
 * reports the floor (00:8097), and run on into from it.
 */
void func_80AE_b0(void) {
    REGS_IN;

    mff_cycles(0x80AE, 2 + 6);
    a = SFX_LAND;
    SET_NZ(a);
    JSR_FN(0x80B0, func_F34C);                  /* sound_queue_push_sfx */
    mff_cycles(0x80B3, 2 + 3 + 3 + 2 + 3 + 2 + 3 + 6);
    a = 0;
    RAM(SCROLL_Y) = a;
    RAM(JUMP_MOVING) = a;
    a = ACT_LAND;
    RAM(GAME_ACTION) = a;
    a = 3;
    SET_NZ(a);
    RAM(ACTION_TIMER) = a;
    JSR_AT(0x80C1, 0xCEAA, 0);                  /* player_stand */
    mff_cycles(0x80C4, 6);
    RTS();
}

/* ---- game_action $11, attacking with a picked-up weapon ---- */

/* the weapon attack of each character */
#define WEAPON_HANDLER_LO   0x818D   /* 93 1D AF */
#define WEAPON_HANDLER_HI   0x8190   /* 81 82 82: $8193, $821D, $82AF */

/*
 * action_weapon ($817E in bank $00, proposed): game_action $11, which
 * player_state_idle sets for an attack with a weapon in hand (0E:CB07,
 * CB94): JMP ($0000) to cody_weapon, guy_weapon or haggar_weapon by
 * player_id.
 * Out: $00/$01 = the handler, A = its high byte.
 */
void func_817E_b0(void) {
    REGS_IN;

    mff_cycles(0x817E, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read_ix(WEAPON_HANDLER_LO, y);
    mff_cycles(0x8183, 3 + 4);
    RAM(TMP0) = a;
    a = prg_read_ix(WEAPON_HANDLER_HI, y);
    SET_NZ(a);
    mff_cycles(0x8188, 3 + 5);
    RAM(TMP1) = a;
    JMP_IND(TMP0);
}

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
            ADC(prg_read_ix(CODY_KNIFE_DX, y));
            mff_cycles(0x81BD, 3 + 4 + 4);
            mff_push(a);
            a = RAM(OBJ_X_HI);
            ADC(prg_read_ix(CODY_KNIFE_DX + 1, y));
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
            a = prg_read_ix(COMBO_FIRST_ANIM, y);
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
            ADC(prg_read_ix(GUY_THROW_DX, y));
            mff_cycles(0x8247, 3 + 4 + 4);
            mff_push(a);
            a = RAM(OBJ_X_HI);
            ADC(prg_read_ix(GUY_THROW_DX + 1, y));
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
            a = prg_read_ix(COMBO_FIRST_ANIM, y);
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
        a = prg_read_ix(COMBO_FIRST_ANIM, y);
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

/* the dropped weapon's animation, by player_id */
#define ANIM_WEAPON_DROPPED 0x93EC   /* 41 5F 51 */
#define OBJ_ID_WEAPON       0x67

/*
 * weapon_drop_on_exit ($93AF in bank $00, proposed; never executed in any
 * run): the weapon in hand falls where the player is: slot 1 live (obj_state
 * $80, obj_flags $90) at the player's position, animation
 * $93EC[player_id] from its start, obj_dir = player_dir, id $67.
 * Called by action_walk_out (JSR at 00:9320) when an area ends with a
 * weapon held. Out: A = $67.
 */
void func_93AF_b0(void) {
    REGS_IN;

    mff_cycles(0x93AF, 2 + 4 + 2 + 4 + 4 * (4 + 4) + 3 + 4);
    a = STATE_LIVE;
    RAM(OBJ_STATE + 1) = a;
    a = FLAGS_FACING_LEFT;
    RAM(OBJ_FLAGS + 1) = a;
    a = RAM(OBJ_X);
    RAM(OBJ_X + 1) = a;
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + 1) = a;
    a = RAM(OBJ_X_HI);
    RAM(OBJ_X_HI + 1) = a;
    a = RAM(OBJ_Y_HI);
    RAM(OBJ_Y_HI + 1) = a;
    y = RAM(PLAYER_ID);
    a = prg_read_ix(ANIM_WEAPON_DROPPED, y);
    mff_cycles(0x93D6, 4 + 2 + 4 + 4 + 3 + 4 + 2 + 4 + 6);
    RAM(OBJ_ANIM + 1) = a;
    a = 0;
    RAM(OBJ_ANIM_TIMER + 1) = a;
    RAM(OBJ_ANIM_FRAME + 1) = a;
    a = RAM(PLAYER_DIR);
    RAM(OBJ_DIR + 1) = a;
    a = OBJ_ID_WEAPON;
    SET_NZ(a);
    RAM(OBJ_ID + 1) = a;
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
        a = prg_read_ix(WALK_VX_FRAC, y);
        mff_cycles(0x8DB5, 4 + 4);
        RAM(OBJ_VX_FRAC) = a;
        a = prg_read_ix(WALK_VX, y);
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
    a = prg_read_ix(ANIM_FALL, y);
    SET_NZ(a);
    mff_cycles(0x8DE5, 6);
    JSR_FN(0x8DE5, func_F294);                  /* player_set_anim */
    mff_cycles(0x8DE8, 4);
    a = prg_read_ix(CHR_JUMP, y);
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
        a = prg_read_ix(ANIM_FALL, y);
        SET_NZ(a);
        mff_cycles(0x8E74, 6);
        JSR_FN(0x8E74, func_F294);              /* player_set_anim */
        mff_cycles(0x8E77, 4);
        a = prg_read_ix(CHR_JUMP, y);
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
        a = prg_read_ix(COMBO_FIRST_ANIM, y);
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

/* ---- the camera's scroll, down and getting up ---- */

/*
 * action_camera_wait ($8FAF in bank $00, proposed): game_action $1D, which
 * the camera sets in an area that scrolls by itself when the player pushes
 * ahead (0F:E3F3). Once scroll_x has gone $28 into the screen - except on
 * the third screen of round 1's second area and of round 4's first area,
 * where the camera code goes on with game_action $1E (0F:E369) or $26
 * (0F:E383, player_exits.c) - the player stands again facing right (left
 * when obj_flags is $90: never executed), game_action, camera_autoscroll
 * and jump_moving 0, obj_state[0] $80.
 */
void func_8FAF_b0(void) {
    REGS_IN;

    mff_cycles(0x8FAF, 3 + 2 + 2);
    a = RAM(SCROLL_X);
    COMPARE(a, 0x28);
    if (!c) {
        mff_cycles(0x8FB5, 6);
        RTS();
    }
    mff_cycles(0x8FB3, 1 + 3 + 2);
    a = RAM(ROUND);
    if (a == 0) {
        mff_cycles(0x8FBA, 3 + 2 + 2);
        a = RAM(SUB_AREA);
        if (a == 1) {
            mff_cycles(0x8FC0, 3 + 2 + 2);
            a = RAM(AREA_SCREEN);
            COMPARE(a, 2);
            if (z) {
                mff_cycles(0x8FC6, 6);
                RTS();
            }
            mff_cycles(0x8FC4, 1);
        } else {
            mff_cycles(0x8FBE, 1);
        }
    } else {
        mff_cycles(0x8FB8, 1);
    }
    mff_cycles(0x8FC7, 3 + 2 + 2);
    a = RAM(ROUND);
    if (a == 3) {
        mff_cycles(0x8FCD, 3 + 2);
        a = RAM(SUB_AREA);
        if (a == 0) {
            mff_cycles(0x8FD1, 3 + 2 + 2);
            a = RAM(AREA_SCREEN);
            COMPARE(a, 2);
            if (z) {
                mff_cycles(0x8FD7, 6);
                RTS();
            }
            mff_cycles(0x8FD5, 1);
        } else {
            mff_cycles(0x8FCF, 1);
        }
    } else {
        mff_cycles(0x8FCB, 1);
    }
    mff_cycles(0x8FD8, 4 + 2 + 2);
    a = RAM(OBJ_FLAGS);
    COMPARE(a, FLAGS_FACING_LEFT);
    if (z) {
        mff_cycles(0x8FDF, 2 + 2 + 1);
        a = PAD_LEFT;
    } else {
        mff_cycles(0x8FDD, 1 + 2);
        a = PAD_RIGHT;
    }
    SET_NZ(a);
    mff_cycles(0x8FE5, 3 + 6);
    RAM(PLAYER_DIR) = a;
    JSR_AT(0x8FE7, 0xCEAA, 0);                  /* player_stand */
    mff_cycles(0x8FEA, 2 + 3 + 3 + 3 + 2 + 4 + 6);
    a = 0;
    RAM(GAME_ACTION) = a;
    RAM(CAMERA_AUTOSCROLL) = a;
    RAM(JUMP_MOVING) = a;
    a = STATE_LIVE;
    SET_NZ(a);
    RAM(OBJ_STATE) = a;
    RTS();
}

/* walking with a weapon in hand: the animation, and the ids to compare */
#define ANIM_WEAPON_WALK    0xE204   /* 53 05 54 */
#define IS_WEAPON_WALK      0xE29A   /* 53 05 54 */
/* round 1's second area: where the jump starts (x $0340) and the band
 * the player walks into first (y $7C-$7F, also the landing) */
#define CAMERA_JUMP_X_HI    0x03
#define CAMERA_JUMP_X       0x40
#define CAMERA_WALK_TOP     0x7C
#define CAMERA_WALK_BOTTOM  0x80

static void camera_walk_band(void);
static void camera_walk_on(void);

/*
 * action_camera_walk ($8FF8 in bank $00, proposed): game_action $1E, which
 * the camera code sets on the third screen of round 1's second area instead
 * of letting action_camera_wait end (0F:E369). The first step, every frame
 * until it is done: camera_autoscroll 0, the walking speed in x and y,
 * facing right, the walking animation ($E204's with a weapon) unless
 * already in it, with CHR bank $E22B (camera_walk_chr); then into the band
 * y $7C-$7F (camera_walk_band), and in it a step right and the next step
 * with obj_vy $0438. Then (camera_walk_on) he walks right to x $0340, jumps
 * from there (camera_walk_step) and, landed, game_action $19 (the area's
 * walk out, player_exits.c) with obj_state[0] $80.
 */
void func_8FF8_b0(void) {
    REGS_IN;

    mff_cycles(0x8FF8, 4 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x0F;
    if (a != 0) {
        mff_cycles(0x8FFD, 2);                  /* BNE $9073, across a page */
        FALL_INTO(camera_walk_on);
    }
    mff_cycles(0x8FFF, 2 + 3 + 3 + 4);
    a = 0;
    RAM(CAMERA_AUTOSCROLL) = a;
    y = RAM(PLAYER_ID);
    a = prg_read_ix(WALK_VX_FRAC, y);
    mff_cycles(0x9008, 4 + 4);
    RAM(OBJ_VX_FRAC) = a;
    a = prg_read_ix(WALK_VX, y);
    mff_cycles(0x900E, 4 + 4);
    RAM(OBJ_VX) = a;
    a = prg_read_ix(WALK_VY_FRAC, y);
    mff_cycles(0x9014, 4 + 4);
    RAM(OBJ_VY_FRAC) = a;
    a = prg_read_ix(WALK_VY, y);
    SET_NZ(a);
    mff_cycles(0x901A, 4 + 6);
    RAM(OBJ_VY) = a;
    JSR_FN(0x901D, func_F03F);                  /* player_face_right */
    mff_cycles(0x9020, 3 + 2);
    a = RAM(WEAPON_HELD);
    if (a == 0) {
        mff_cycles(0x9024, 4 + 4);
        a = RAM(OBJ_ANIM);
        m = prg_read_ix(IS_WALKING, y);
        c = a >= m;
        mff_cycles(0x902A, 2);
        if (a == m) {
            mff_cycles(0x902A, 1);
            FALL_INTO(camera_walk_band);
        }
        mff_cycles(0x902C, 4);
        a = prg_read_ix(ANIM_WALK, y);
        SET_NZ(a);
        mff_cycles(0x902F, 6);
        JSR_FN(0x902F, func_F294);              /* player_set_anim */
        mff_cycles(0x9032, 3);
        JMP_TAIL(0x9043, 0);                    /* camera_walk_chr */
    }
    mff_cycles(0x9022, 1 + 4 + 4);
    a = RAM(OBJ_ANIM);
    m = prg_read_ix(IS_WEAPON_WALK, y);
    c = a >= m;
    mff_cycles(0x903B, 2);
    if (a == m) {
        mff_cycles(0x903B, 1);
        FALL_INTO(camera_walk_band);
    }
    mff_cycles(0x903D, 4);
    a = prg_read_ix(ANIM_WEAPON_WALK, y);
    SET_NZ(a);
    mff_cycles(0x9040, 6);
    JSR_FN(0x9040, func_F294);                  /* player_set_anim */
    FALL_INTO(func_9043_b0);
}

/* camera_walk_chr ($9043 in bank $00, proposed): the walking CHR bank
 * ($E22B[Y]), then camera_walk_band. Reached by JMP from
 * action_camera_walk (00:9032) and run on into from it. */
void func_9043_b0(void) {
    REGS_IN;

    mff_cycles(0x9043, 4);
    a = prg_read_ix(CHR_STAND, y);
    SET_NZ(a);
    mff_cycles(0x9046, 6);
    JSR_FN(0x9046, func_F440);                  /* player_set_chr */
    FALL_INTO(camera_walk_band);
}

/* camera_walk_band (00:9049, inside action_camera_walk): down to y $7C or
 * up from y $80; in the band a step right facing right, the next step and
 * obj_vy $0438 (the jump's speed). */
static void camera_walk_band(void) {
    REGS_IN;

    mff_cycles(0x9049, 4 + 2 + 2);
    a = RAM(OBJ_Y);
    COMPARE(a, CAMERA_WALK_TOP);
    if (!c) {
        mff_cycles(0x9050, 6);
        JSR_FN(0x9050, func_F1BF);              /* obj_move_down */
        mff_cycles(0x9053, 6);
        RTS();
    }
    mff_cycles(0x904E, 1 + 4 + 2 + 2);
    a = RAM(OBJ_Y);
    COMPARE(a, CAMERA_WALK_BOTTOM);
    if (c) {
        mff_cycles(0x905B, 6);
        JSR_FN(0x905B, func_F1DF);              /* obj_move_up */
        mff_cycles(0x905E, 6);
        RTS();
    }
    mff_cycles(0x9059, 1 + 6);
    JSR_FN(0x905F, func_F183);                  /* obj_move_right */
    mff_cycles(0x9062, 6);
    JSR_FN(0x9062, func_F03F);                  /* player_face_right */
    mff_cycles(0x9065, 6 + 2 + 4 + 2 + 4);
    RAM(OBJ_STATE)++;
    a = 0x38;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x04;
    SET_NZ(a);
    RAM(OBJ_VY) = a;
    FALL_INTO(func_9072_b0);
}

/* $9072 in bank $00: the RTS of action_camera_walk, which camera_walk_step
 * branches to (00:90B7). */
void func_9072_b0(void) {
    REGS_IN;

    mff_cycles(0x9072, 6);
    RTS();
}

/* camera_walk_on (00:9073, inside action_camera_walk): the second step
 * walks right (obj_move_right) to x $0340, then jumps: the jump's animation
 * while rising, the falling one after, each with the jump's CHR bank, and
 * camera_walk_step. The third: game_action $19 with obj_state[0] $80. */
static void camera_walk_on(void) {
    REGS_IN;

    mff_cycles(0x9073, 4 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x02;
    if (a != 0) {                               /* landed: the walk out */
        mff_cycles(0x9078, 1 + 2 + 3 + 2 + 4 + 6);
        a = ACT_WALK_OUT;
        RAM(GAME_ACTION) = a;
        a = STATE_LIVE;
        SET_NZ(a);
        RAM(OBJ_STATE) = a;
        RTS();
    }
    mff_cycles(0x907A, 4 + 2 + 2);
    a = RAM(OBJ_X_HI);
    COMPARE(a, CAMERA_JUMP_X_HI);
    if (z) {
        mff_cycles(0x9081, 4 + 2 + 2);
        a = RAM(OBJ_X);
        COMPARE(a, CAMERA_JUMP_X);
        if (c) {                                /* the jump */
            mff_cycles(0x9088, 4 + 2);
            a = RAM(OBJ_VY);
            if (!(a & 0x80)) {                  /* rising */
                mff_cycles(0x908D, 3 + 4);
                y = RAM(PLAYER_ID);
                a = prg_read_ix(ANIM_JUMP, y);
                SET_NZ(a);
                mff_cycles(0x9092, 6);
                JSR_FN(0x9092, func_F294);      /* player_set_anim */
                mff_cycles(0x9095, 4);
                a = prg_read_ix(CHR_JUMP, y);
                SET_NZ(a);
                mff_cycles(0x9098, 6);
                JSR_FN(0x9098, func_F440);      /* player_set_chr */
                mff_cycles(0x909B, 3);
                JMP_TAIL(0x90AC, 0);            /* camera_walk_step */
            }
            mff_cycles(0x908B, 1 + 3 + 4);
            y = RAM(PLAYER_ID);
            a = prg_read_ix(ANIM_FALL, y);
            SET_NZ(a);
            mff_cycles(0x90A3, 6);
            JSR_FN(0x90A3, func_F294);          /* player_set_anim */
            mff_cycles(0x90A6, 4);
            a = prg_read_ix(CHR_JUMP, y);
            SET_NZ(a);
            mff_cycles(0x90A9, 6);
            JSR_FN(0x90A9, func_F440);          /* player_set_chr */
            FALL_INTO(func_90AC_b0);
        }
        mff_cycles(0x9086, 1 + 6);
    } else {
        mff_cycles(0x907F, 1 + 6);
    }
    JSR_FN(0x90BD, func_F183);                  /* obj_move_right */
    mff_cycles(0x90C0, 6);
    RTS();
}

/*
 * camera_walk_step ($90AC in bank $00, proposed): the jump of
 * action_camera_walk, every frame from x $0340 (JMP at 00:909B, run on
 * into from camera_walk_on): the player moves right and falls ($F1FD);
 * landed at y $7C the step counts up, above it the branch goes to the RTS
 * of action_camera_walk (00:9072).
 */
void func_90AC_b0(void) {
    REGS_IN;

    mff_cycles(0x90AC, 6);
    JSR_FN(0x90AC, func_F183);                  /* obj_move_right */
    mff_cycles(0x90AF, 6);
    JSR_FN(0x90AF, func_F1FD);                  /* obj_fall, no probe */
    mff_cycles(0x90B2, 4 + 2 + 2);
    a = RAM(OBJ_Y);
    COMPARE(a, 0x7C);
    if (!c) {
        mff_cycles(0x90B7, 1);
        BRANCH_TAIL(0x9072, 0);
    }
    mff_cycles(0x90B9, 6 + 6);
    RAM(OBJ_STATE)++;
    SET_NZ(RAM(OBJ_STATE));
    RTS();
}

/* direction presses that make the player get up sooner, by player_level */
#define GET_UP_PRESSES      0x9147   /* 14 12 10 0E 0C 0A */

/* the presses that count: a direction, alone or diagonal */
static const uint8_t k_get_up_pads[] = {
    PAD_RIGHT, PAD_LEFT, PAD_UP, PAD_DOWN,
    PAD_UP | PAD_RIGHT, PAD_UP | PAD_LEFT, PAD_DOWN | PAD_RIGHT, PAD_DOWN | PAD_LEFT,
};

static void get_up(void);

/*
 * action_lying ($90CB in bank $00, proposed; Data Crystal: lying down):
 * game_action $1F, down after a knockdown (knockdown_lie set action_timer
 * $3C). With health gone the death starts: player_no_hit $80, game_action
 * $0B for $78 frames (player_state_throw's blink). Otherwise action_timer
 * counts down to get_up, and every direction press counts in obj_timer[0]:
 * $9147[player_level] presses (20 at the first level, 10 at the sixth) get
 * the player up at once.
 */
void func_90CB_b0(void) {
    unsigned i;
    REGS_IN;

    mff_cycles(0x90CB, 3 + 2);
    a = RAM(ACTION_TIMER);
    if (a == 0)
        FALL_INTO(get_up);
    mff_cycles(0x90CD, 2 + 3 + 2);
    a = RAM(HEALTH);
    if (a & 0x80) {
        mff_cycles(0x9107, 2 + 3 + 2 + 3 + 2 + 3 + 6);
        a = NO_HIT;
        RAM(PLAYER_NO_HIT) = a;
        a = ACT_THROW;
        RAM(GAME_ACTION) = a;
        a = 0x78;
        SET_NZ(a);
        RAM(ACTION_TIMER) = a;
        RTS();
    }
    mff_cycles(0x9105, 1 + 3);
    a = RAM(JOY_PRESSED);
    for (i = 0; i < sizeof k_get_up_pads; i++) {
        mff_cycles(0x9116, 2 + 2);              /* CMP #, BEQ */
        COMPARE(a, k_get_up_pads[i]);
        if (z)
            break;
    }
    if (i == sizeof k_get_up_pads) {            /* no press */
        mff_cycles(0x9136, 5 + 6);
        RAM(ACTION_TIMER)--;
        SET_NZ(RAM(ACTION_TIMER));
        RTS();
    }
    mff_cycles(0x9118, 1 + 6 + 4 + 3 + 4);
    RAM(OBJ_TIMER)++;
    a = RAM(OBJ_TIMER);
    y = RAM(PLAYER_LEVEL);
    COMPARE(a, prg_read_ix(GET_UP_PRESSES, y));
    mff_cycles(0x9144, 2);
    if (z) {
        mff_cycles(0x9144, 2);                  /* BEQ taken, to the page before */
        FALL_INTO(get_up);
    }
    mff_cycles(0x9146, 6);
    RTS();
}

/* get_up (00:90CF, inside action_lying): the getting-up animation and CHR
 * bank, player_dir from the facing (obj_flags $D0: right), game_action 2
 * for 8 frames, guy_jump_window, attack_ground_y, jump_moving and slot 0's
 * obj_timer and obj_timer2 0. */
static void get_up(void) {
    REGS_IN;

    mff_cycles(0x90CF, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read_ix(ANIM_GET_UP, y);
    SET_NZ(a);
    mff_cycles(0x90D4, 6);
    JSR_FN(0x90D4, func_F294);                  /* player_set_anim */
    mff_cycles(0x90D7, 4);
    a = prg_read_ix(CHR_GET_UP, y);
    SET_NZ(a);
    mff_cycles(0x90DA, 6);
    JSR_FN(0x90DA, func_F440);                  /* player_set_chr */
    mff_cycles(0x90DD, 4 + 2 + 2);
    a = RAM(OBJ_FLAGS);
    COMPARE(a, FLAGS_FACING_RIGHT);
    if (z) {
        mff_cycles(0x90E4, 2 + 2 + 1);
        a = PAD_RIGHT;
    } else {
        mff_cycles(0x90E2, 1 + 2);
        a = PAD_LEFT;
    }
    mff_cycles(0x90EA, 3 + 2 + 3 + 2 + 3 + 2 + 3 + 3 + 3 + 4 + 4 + 6);
    RAM(PLAYER_DIR) = a;
    a = ACT_LAND;
    RAM(GAME_ACTION) = a;
    a = 8;
    RAM(ACTION_TIMER) = a;
    a = 0;
    SET_NZ(a);
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(ATTACK_GROUND_Y) = a;
    RAM(JUMP_MOVING) = a;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER2) = a;
    RTS();
}
