/*
 * player_specials.c - the moves that run as bank-$00 actions after an attack
 * or a grab: Haggar's HeadSmasher (game_action $12) and its earthquake
 * ($0F, player_actions.c), Cody's slash ($14) and the A+B special ($17).
 */
#include "player_defs.h"

#define ANIM_HEADSMASHER    0x48     /* Haggar's smash, B in the air */
#define ANIM_SMASH_LANDED   0x49
#define OBJ_ID_CARRIED      0x0E     /* the held enemy while Haggar jumps */

/*
 * action_headsmasher ($8300 in bank $00, proposed; Data Crystal: Haggar's
 * HeadSmasher): game_action $12, the jump haggar_headsmasher_start began
 * (A while Haggar holds an enemy). The animation is held on its first
 * frame (rising with game_action 0 would also restart the fall, $F26E;
 * never executed). obj_fall (Y = 2):
 *  - in the air (rising, or still above the ground line):
 *    headsmasher_air;
 *  - on the ground (obj_fall's floor, or obj_y + 2 at the ground line, set
 *    as the knockdown does): in the smash (animation $48) the earthquake
 *    starts: CHR bank $0A, animation $49, sound $20, game_action $0F with
 *    obj_vy $0248 and obj_timer[0] $10 (obj_vx_frac is written twice, 0 then
 *    1: the second store was meant for obj_vx). Without the smash Haggar
 *    just lands (sound $1C, game_action 2 for 3 frames, attack_end,
 *    player_stand) and lets go: the enemy gets back its saved id and state,
 *    obj_held $FF, obj_throw_flag 0 and Haggar's obj_y.
 */
void func_8300_b0(void) {
    REGS_IN;

    mff_cycles(0x8300, 2 + 4 + 4 + 4 + 2);
    a = 0;
    RAM(OBJ_ANIM_TIMER) = a;
    RAM(OBJ_ANIM_FRAME) = a;
    a = RAM(OBJ_VY);
    if (!(a & 0x80)) {
        mff_cycles(0x830D, 3 + 2);
        a = RAM(GAME_ACTION);
        SET_NZ(a);
        if (z) {
            mff_cycles(0x8311, 6);
            JSR_FN(0x8311, func_F26E);          /* the start of a fall */
        } else {
            mff_cycles(0x830F, 1);
        }
    } else {
        mff_cycles(0x830B, 1);
    }
    mff_cycles(0x8314, 2 + 2 + 6);
    x = 0;
    y = 2;
    SET_NZ(y);
    JSR_FN(0x8318, func_F119);                  /* obj_fall */
    mff_cycles(0x831B, 2);
    if (!c) {
        mff_cycles(0x831D, 4 + 2);
        a = RAM(OBJ_VY);
        SET_NZ(a);
        if (!n) {                               /* rising */
            mff_cycles(0x8322, 3);
            JMP_TAIL(0x83A1, 0);                /* headsmasher_air */
        }
        mff_cycles(0x8320, 1 + 4 + 2 + 2 + 3 + 2);
        a = RAM(OBJ_Y);
        c = 0;
        ADC(2);
        COMPARE(a, RAM(PLAYER_GROUND_Y));
        if (!c) {                               /* above the ground line */
            mff_cycles(0x832D, 1);
            FALL_INTO(func_83A1_b0);            /* headsmasher_air */
        }
        mff_cycles(0x832F, 2);
        if (z) {
            mff_cycles(0x832F, 1 + 4);
            RAM(OBJ_Y) = a;
        } else {
            mff_cycles(0x8331, 2 + 4 + 2);
            SBC(3);
            RAM(OBJ_Y) = a;
            if (a != 0) {
                mff_cycles(0x8336, 1);
            } else {
                mff_cycles(0x8338, 4);
                RAM(OBJ_Y) = a;
            }
        }
    } else {
        mff_cycles(0x831B, 1);
    }

    /* on the ground */
    mff_cycles(0x833B, 4 + 2 + 2);
    a = RAM(OBJ_ANIM);
    COMPARE(a, ANIM_HEADSMASHER);
    if (!z) {                                   /* no smash: land and let go */
        mff_cycles(0x8342, 2 + 6);
        a = SFX_LAND;
        SET_NZ(a);
        JSR_FN(0x8344, func_F34C);              /* sound_queue_push_sfx */
        mff_cycles(0x8347, 2 + 3 + 2 + 3 + 6);
        a = ACT_LAND;
        RAM(GAME_ACTION) = a;
        a = 3;
        SET_NZ(a);
        RAM(ACTION_TIMER) = a;
        JSR_FN(0x834F, func_F436);              /* attack_end */
        mff_cycles(0x8352, 6);
        JSR_AT(0x8352, 0xCEAA, 0);              /* player_stand */
        mff_cycles(0x8355, 3 + 4 + 5 + 2 + 5 + 4 + 5 + 2 + 5 + 4 + 5 + 6);
        y = RAM(GRAB_SLOT);
        a = RAM(OBJ_SAVED_ID + y);
        RAM(OBJ_ID + y) = a;
        a = 0xFF;
        RAM(OBJ_HELD + y) = a;
        a = RAM(OBJ_SAVED_STATE + y);
        RAM(OBJ_STATE + y) = a;
        a = 0;
        RAM(OBJ_THROW_FLAG + y) = a;
        a = RAM(OBJ_Y);
        SET_NZ(a);
        RAM(OBJ_Y + y) = a;
        RTS();
    }
    mff_cycles(0x8340, 1 + 2 + 6);              /* the smash lands */
    a = 0x0A;
    SET_NZ(a);
    JSR_FN(0x8376, func_F440);                  /* player_set_chr */
    mff_cycles(0x8379, 2 + 6);
    a = ANIM_SMASH_LANDED;
    SET_NZ(a);
    JSR_FN(0x837B, func_F294);                  /* player_set_anim */
    mff_cycles(0x837E, 2 + 6);
    a = 0x20;
    SET_NZ(a);
    JSR_FN(0x8380, func_F34C);                  /* sound_queue_push_sfx */
    mff_cycles(0x8383, 2 + 3 + 2 + 4 + 2 + 4 + 2 + 4 + 2 + 4 + 2 + 4 + 6);
    a = ACT_EARTHQUAKE;
    RAM(GAME_ACTION) = a;
    a = 0x48;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x02;
    RAM(OBJ_VY) = a;
    a = 0x00;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VX_FRAC) = a;                       /* sic, obj_vx was meant */
    a = 0x10;
    SET_NZ(a);
    RAM(OBJ_TIMER) = a;
    RTS();
}

/*
 * headsmasher_air ($83A1 in bank $00, proposed): while rising, B starts the
 * smash: CHR bank $0A, animation $48, player_no_hit $FF, attack_id 9. The
 * held enemy follows Haggar, 4 px below him in the smash and 12 px above
 * him before it (then headsmasher_carry). Reached by JMP from
 * action_headsmasher (00:8322) and run on into from it.
 */
void func_83A1_b0(void) {
    REGS_IN;

    mff_cycles(0x83A1, 4 + 2);
    a = RAM(OBJ_VY);
    if (!(a & 0x80)) {
        mff_cycles(0x83A6, 3 + 2 + 2);
        a = RAM(JOY_PRESSED);
        COMPARE(a, PAD_B);
        if (z) {
            mff_cycles(0x83AC, 2 + 6);
            a = 0x0A;
            SET_NZ(a);
            JSR_FN(0x83AE, func_F440);          /* player_set_chr */
            mff_cycles(0x83B1, 2 + 6);
            a = ANIM_HEADSMASHER;
            SET_NZ(a);
            JSR_FN(0x83B3, func_F294);          /* player_set_anim */
            mff_cycles(0x83B6, 2 + 3 + 2 + 3);
            a = 0xFF;
            RAM(PLAYER_NO_HIT) = a;
            a = 0x09;
            RAM(ATTACK_ID) = a;
        } else {
            mff_cycles(0x83AA, 1);
        }
    } else {
        mff_cycles(0x83A4, 1);
    }
    mff_cycles(0x83BE, 3 + 4 + 2 + 2);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_ANIM);
    if (a == ANIM_HEADSMASHER) {
        mff_cycles(0x83C7, 4 + 2 + 2 + 5 + 3);
        a = RAM(OBJ_Y);
        c = 0;
        ADC(4);
        RAM(OBJ_Y + y) = a;
        JMP_TAIL(0x83DC, 0);                    /* headsmasher_carry */
    }
    mff_cycles(0x83C5, 1 + 4 + 2 + 2 + 5);
    a = RAM(OBJ_Y);
    c = 1;
    SBC(0x0C);
    RAM(OBJ_Y + y) = a;
    FALL_INTO(func_83DC_b0);
}

/*
 * headsmasher_carry ($83DC in bank $00, proposed): the held enemy (slot Y)
 * becomes id $0E. The jump steers as an ordinary one: the first frame with
 * left or right held sets jump_moving and jump_dir; then Haggar moves that
 * way while short of the screen's edge (241 px right, 25 px left) and the
 * enemy moves with him (headsmasher_carry_right/left) while it stays in:
 * in front up to 249 px right or 17 px left, behind while Haggar is short
 * of 241 px or right of 25 px. Reached by JMP from headsmasher_air
 * (00:83D0) and run on into from it.
 */
void func_83DC_b0(void) {
    REGS_IN;

    mff_cycles(0x83DC, 2 + 5 + 3 + 2);
    a = OBJ_ID_CARRIED;
    RAM(OBJ_ID + y) = a;
    a = RAM(JUMP_MOVING);
    if (a == 0) {
        mff_cycles(0x83E5, 2 + 3 + 2 + 2);
        x = 0;
        a = RAM(JOY_HELD) & (PAD_LEFT | PAD_RIGHT);
        SET_NZ(a);
        if (z) {
            mff_cycles(0x83EB, 2 + 6);
            RTS();
        }
        mff_cycles(0x83ED, 5 + 3);
        RAM(JUMP_MOVING)++;
        RAM(JUMP_DIR) = a;
    } else {
        mff_cycles(0x83E3, 1);
    }
    mff_cycles(0x83F1, 3 + 2 + 2);
    a = RAM(JUMP_DIR) & PAD_RIGHT;
    if (a != 0) {                               /* right */
        mff_cycles(0x83F7, 4 + 2 + 3 + 2 + 2);
        a = RAM(OBJ_X);
        c = 1;
        SBC(RAM(SCROLL_X));
        COMPARE(a, SCREEN_RIGHT_LIMIT + 1);
        if (!z) {
            mff_cycles(0x8401, 2);
            if (!c) {
                mff_cycles(0x8403, 6);
                JSR_FN(0x8403, func_F183);      /* obj_move_right */
                mff_cycles(0x8406, 3 + 3 + 2 + 2);
                y = RAM(GRAB_SLOT);
                a = RAM(PLAYER_DIR);
                if (a == PAD_RIGHT) {           /* the enemy in front */
                    mff_cycles(0x840C, 1 + 4 + 2 + 3 + 2 + 2);
                    a = RAM(OBJ_X + y);
                    c = 1;
                    SBC(RAM(SCROLL_X));
                    COMPARE(a, 0xF9);
                    if (!z) {
                        mff_cycles(0x8427, 2);
                        if (!c)
                            FALL_INTO(func_8429_b0);
                    }
                } else {                        /* behind */
                    mff_cycles(0x840E, 4 + 2 + 3 + 2 + 2);
                    a = RAM(OBJ_X);
                    c = 1;
                    SBC(RAM(SCROLL_X));
                    COMPARE(a, SCREEN_RIGHT_LIMIT + 1);
                    if (!z) {
                        mff_cycles(0x8418, 2);
                        if (!c) {
                            mff_cycles(0x841A, 3);
                            JMP_TAIL(0x8429, 0);  /* headsmasher_carry_right */
                        }
                    }
                }
            }
        }
        mff_cycles(0x8446, 1 + 6);              /* a branch taken, RTS */
        RTS();
    }

    mff_cycles(0x83F5, 2 + 4 + 2 + 3 + 2 + 2);  /* left */
    a = RAM(OBJ_X);
    c = 1;
    SBC(RAM(SCROLL_X));
    COMPARE(a, 0x19);
    if (!z) {
        mff_cycles(0x8451, 2);
        if (c) {
            mff_cycles(0x8453, 6);
            JSR_FN(0x8453, func_F1A1);          /* obj_move_left */
            mff_cycles(0x8456, 3 + 3 + 2 + 2);
            y = RAM(GRAB_SLOT);
            a = RAM(PLAYER_DIR);
            if (a == PAD_RIGHT) {               /* the enemy behind */
                mff_cycles(0x845C, 1 + 4 + 2 + 3 + 2 + 2);
                a = RAM(OBJ_X);
                c = 1;
                SBC(RAM(SCROLL_X));
                COMPARE(a, 0x19);
                if (!z) {
                    mff_cycles(0x8477, 2);
                    if (c)
                        FALL_INTO(func_8479_b0);
                }
            } else {                            /* in front */
                mff_cycles(0x845E, 4 + 2 + 3 + 2 + 2);
                a = RAM(OBJ_X + y);
                c = 1;
                SBC(RAM(SCROLL_X));
                COMPARE(a, 0x11);
                if (!z) {
                    mff_cycles(0x8468, 2);
                    if (c) {
                        mff_cycles(0x846A, 3);
                        JMP_TAIL(0x8479, 0);  /* headsmasher_carry_left */
                    }
                }
            }
        }
    }
    mff_cycles(0x8496, 1 + 6);                  /* a branch taken, RTS */
    RTS();
}

/* headsmasher_carry_right ($8429 in bank $00, proposed): the held enemy
 * (slot Y) moves right by Haggar's vx. */
void func_8429_b0(void) {
    REGS_IN;

    mff_cycles(0x8429, 4 + 2 + 4 + 5 + 4 + 4 + 5 + 2);
    a = RAM(OBJ_X_FRAC + y);
    c = 0;
    ADC(RAM(OBJ_VX_FRAC));
    RAM(OBJ_X_FRAC + y) = a;
    a = RAM(OBJ_X + y);
    ADC(RAM(OBJ_VX));
    RAM(OBJ_X + y) = a;
    if (c) {
        mff_cycles(0x843E, 4 + 2 + 5);
        a = RAM(OBJ_X_HI + y);
        ADC(0);
        RAM(OBJ_X_HI + y) = a;
    } else {
        mff_cycles(0x843C, 1);
    }
    mff_cycles(0x8446, 6);
    RTS();
}

/* headsmasher_carry_left ($8479 in bank $00, proposed): the held enemy
 * (slot Y) moves left by Haggar's vx (the high byte's borrow never
 * executed in any run). */
void func_8479_b0(void) {
    REGS_IN;

    mff_cycles(0x8479, 4 + 2 + 4 + 5 + 4 + 4 + 5 + 2);
    a = RAM(OBJ_X_FRAC + y);
    c = 1;
    SBC(RAM(OBJ_VX_FRAC));
    RAM(OBJ_X_FRAC + y) = a;
    a = RAM(OBJ_X + y);
    SBC(RAM(OBJ_VX));
    RAM(OBJ_X + y) = a;
    if (!c) {
        mff_cycles(0x848E, 4 + 2 + 5);
        a = RAM(OBJ_X_HI + y);
        SBC(0);
        RAM(OBJ_X_HI + y) = a;
    } else {
        mff_cycles(0x848C, 1);
    }
    mff_cycles(0x8496, 6);
    RTS();
}
