/*
 * player_specials.c - the moves that run as bank-$00 actions after an attack
 * or a grab: Haggar's HeadSmasher (game_action $12) and its earthquake
 * ($0F, player_actions.c), Cody's slash ($14), the A+B special ($17) and
 * Haggar's moving lariat ($16).
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

/* ---- Cody's slash, game_action $14 (cody_finisher sets it) ---- */

#define ANIM_SLASH_FALL     0x3E

/* $8540 in bank $00: an RTS that cody_slash branches to */
void func_8540_b0(void) {
    REGS_IN;

    mff_cycles(0x8540, 6);
    RTS();
}

/*
 * cody_slash ($8541 in bank $00, proposed; Data Crystal: Cody's slash): the
 * end of Cody's combo. Until the falling animation $3E: attack_id 7,
 * attack_begin and a dash (obj_vx $0689). Its first step waits for frame 4
 * at timer 1 (then obj_timer/obj_timer2[0] 0 and the next step). Before
 * frame 6 Cody leaps (obj_vy $0438), with the attack on and attack_ground_y
 * = player_ground_y; from frame 6, or in $3E, the dash stops (obj_vx 0).
 * Then cody_slash_move.
 */
void func_8541_b0(void) {
    REGS_IN;

    mff_cycles(0x8541, 4 + 2 + 2);
    a = RAM(OBJ_ANIM);
    COMPARE(a, ANIM_SLASH_FALL);
    if (!z) {
        mff_cycles(0x8548, 2 + 3 + 6);
        a = 0x07;
        SET_NZ(a);
        RAM(ATTACK_ID) = a;
        JSR_FN(0x854C, func_F43B);              /* attack_begin */
        mff_cycles(0x854F, 2 + 4 + 2 + 4);
        a = 0x89;
        RAM(OBJ_VX_FRAC) = a;
        a = 0x06;
        RAM(OBJ_VX) = a;
    } else {
        mff_cycles(0x8546, 1);
    }
    mff_cycles(0x8559, 4 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x0F;
    if (a == 0) {
        mff_cycles(0x8560, 4 + 2 + 2);
        a = RAM(OBJ_ANIM_FRAME);
        COMPARE(a, 4);
        if (!z) {
            mff_cycles(0x8565, 1);
            BRANCH_TAIL(0x8540, 0);             /* RTS */
        }
        mff_cycles(0x8567, 4 + 2 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        COMPARE(a, 1);
        if (!z) {
            mff_cycles(0x856C, 1);
            BRANCH_TAIL(0x8540, 0);             /* RTS */
        }
        mff_cycles(0x856E, 6 + 2 + 4 + 4);
        RAM(OBJ_STATE)++;
        a = 0;
        RAM(OBJ_TIMER) = a;
        RAM(OBJ_TIMER2) = a;
    } else {
        mff_cycles(0x855E, 1);
    }
    mff_cycles(0x8579, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 6);
    if (!c) {
        mff_cycles(0x8580, 4 + 2 + 2);
        a = RAM(OBJ_ANIM);
        COMPARE(a, ANIM_SLASH_FALL);
        if (!z) {
            mff_cycles(0x8587, 2 + 4 + 2 + 4 + 4 + 2 + 2);
            a = 0x38;
            RAM(OBJ_VY_FRAC) = a;
            a = 0x04;
            RAM(OBJ_VY) = a;
            a = RAM(OBJ_VY);
            COMPARE(a, 3);
            if (c) {
                mff_cycles(0x8598, 6);
                JSR_FN(0x8598, func_F43B);      /* attack_begin */
                mff_cycles(0x859B, 3 + 3 + 3);
                a = RAM(PLAYER_GROUND_Y);
                SET_NZ(a);
                RAM(ATTACK_GROUND_Y) = a;
                JMP_TAIL(0x85AA, 0);            /* cody_slash_move */
            }
            mff_cycles(0x8596, 1);
        } else {
            mff_cycles(0x8585, 1);
        }
    } else {
        mff_cycles(0x857E, 1);
    }
    mff_cycles(0x85A2, 2 + 4 + 4);
    a = 0;
    RAM(OBJ_VX_FRAC) = a;
    RAM(OBJ_VX) = a;
    FALL_INTO(func_85AA_b0);
}

/* cody_slash_move ($85AA in bank $00, proposed): Cody faces and moves his
 * way (player_dir), then cody_slash_fall. Reached by JMP from cody_slash
 * (00:859F) and run on into from it. */
void func_85AA_b0(void) {
    REGS_IN;

    mff_cycles(0x85AA, 3 + 2 + 2);
    a = RAM(PLAYER_DIR) & PAD_LEFT;
    SET_NZ(a);
    if (z) {                                    /* right */
        mff_cycles(0x85B0, 6);
        JSR_FN(0x85B0, func_F090);              /* obj_face_right */
        mff_cycles(0x85B3, 6);
        JSR_FN(0x85B3, func_F183);              /* obj_move_right */
        mff_cycles(0x85B6, 3);
        JMP_TAIL(0x85BF, 0);                    /* cody_slash_fall */
    }
    mff_cycles(0x85AE, 1 + 6);
    JSR_FN(0x85B9, func_F099);                  /* obj_face_left */
    mff_cycles(0x85BC, 6);
    JSR_FN(0x85BC, func_F1A1);                  /* obj_move_left */
    FALL_INTO(func_85BF_b0);
}

/*
 * cody_slash_fall ($85BF in bank $00, proposed): obj_fall (Y = 0). On the
 * floor, cody_slash_land. While rising, frame 9 is held. Falling: the
 * attack ends, the falling animation $3E (CHR bank 1) starts once (the
 * step goes on), and past the ground line + 2 Cody lands on it
 * (cody_slash_land_at). Reached by JMP from cody_slash_move (00:85B6) and
 * run on into from it.
 */
void func_85BF_b0(void) {
    REGS_IN;

    mff_cycles(0x85BF, 2 + 6);
    y = 0;
    SET_NZ(y);
    JSR_FN(0x85C1, func_F119);                  /* obj_fall */
    mff_cycles(0x85C4, 2);
    if (c) {
        mff_cycles(0x85C6, 3);
        JMP_TAIL(0x85FE, 0);                    /* cody_slash_land */
    }
    mff_cycles(0x85C4, 1 + 4 + 2);
    a = RAM(OBJ_VY);
    SET_NZ(a);
    if (!n) {                                   /* rising */
        mff_cycles(0x85CC, 2 + 4 + 2 + 2);
        a = RAM(OBJ_ANIM_FRAME);
        COMPARE(a, 9);
        if (z) {
            mff_cycles(0x8633, 2 + 4 + 2 + 4);
            a = 9;
            RAM(OBJ_ANIM_FRAME) = a;
            a = 0;
            SET_NZ(a);
            RAM(OBJ_ANIM_TIMER) = a;
        } else {
            mff_cycles(0x8631, 1);
        }
        mff_cycles(0x863D, 6);
        RTS();
    }
    mff_cycles(0x85CE, 6);
    JSR_FN(0x85CE, func_F436);                  /* attack_end */
    mff_cycles(0x85D1, 4 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x02;
    if (a == 0) {
        mff_cycles(0x85D8, 2 + 4 + 2);
        a = ANIM_SLASH_FALL;
        COMPARE(a, RAM(OBJ_ANIM));
        if (!z) {
            mff_cycles(0x85DF, 6);
            JSR_FN(0x85DF, func_F294);          /* player_set_anim */
            mff_cycles(0x85E2, 2 + 6);
            a = 0x01;
            SET_NZ(a);
            JSR_FN(0x85E4, func_F440);          /* player_set_chr */
            mff_cycles(0x85E7, 6);
            RAM(OBJ_STATE)++;
        } else {
            mff_cycles(0x85DD, 1);
        }
    } else {
        mff_cycles(0x85D6, 1);
    }
    mff_cycles(0x85EA, 3 + 2 + 2 + 4 + 2);
    a = RAM(PLAYER_GROUND_Y);
    c = 0;
    ADC(2);
    COMPARE(a, RAM(OBJ_Y));
    if (c) {                                    /* not past the ground line */
        mff_cycles(0x85F2, 2 + 6);
        RTS();
    }
    mff_cycles(0x85F4, 2 + 3);                  /* BEQ never taken here */
    JMP_TAIL(0x85F9, 0);                        /* cody_slash_land_at */
}

/* cody_slash_land_at ($85F9 in bank $00, proposed): obj_y = player_ground_y,
 * then cody_slash_land. Reached by JMP from cody_slash_fall (00:85F6). */
void func_85F9_b0(void) {
    REGS_IN;

    mff_cycles(0x85F9, 3 + 4);
    a = RAM(PLAYER_GROUND_Y);
    RAM(OBJ_Y) = a;
    FALL_INTO(func_85FE_b0);
}

/*
 * cody_slash_land ($85FE in bank $00, proposed): the slash is over:
 * game_action 2, guy_jump_window, player_no_hit and combo_timer 0, a small
 * rebound (obj_vy -$40/256) and walking speed in x ($0133),
 * obj_timer/obj_timer2[0] 0, obj_state[0] $80. Reached by JMP from
 * cody_slash_fall (00:85C6) and run on into from cody_slash_land_at.
 */
void func_85FE_b0(void) {
    REGS_IN;

    mff_cycles(0x85FE, 2 + 3 + 2 + 3 + 3 + 3 + 2 + 4 + 2 + 4 + 2 + 4 + 2 + 4 + 2 + 4 + 4 + 2 + 4 + 6);
    a = ACT_LAND;
    RAM(GAME_ACTION) = a;
    a = 0;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(PLAYER_NO_HIT) = a;
    RAM(COMBO_TIMER) = a;
    a = 0xC0;
    RAM(OBJ_VY_FRAC) = a;
    a = 0xFF;
    RAM(OBJ_VY) = a;
    a = 0x33;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VX) = a;
    a = 0;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER2) = a;
    a = STATE_LIVE;
    SET_NZ(a);
    RAM(OBJ_STATE) = a;
    RTS();
}

/* ---- Haggar's moving lariat, game_action $16 (lariat_start, 00:88B6) ---- */

/* attack_id by animation frame */
#define LARIAT_ATTACK_ID    0x8903   /* 0F 10 11 12, repeated (Haggar) */
#define LARIAT_SPEED_UP     PAD_B    /* joy_held: B alone, from frame 6 */
#define LARIAT_LAST_FRAME   0x13
#define LARIAT_LAST_TIMER   0x04
#define LARIAT_HIT_COST     6        /* the health a lariat that hit costs */
#define SFX_LARIAT          0x13     /* every 8th frame (draw_parity) */

static void lariat_step(uint16_t pc, int right);
#define LARIAT_STEP(pc, right) do { SYNC_OUT; lariat_step(pc, right); return; } while (0)

/*
 * action_lariat ($86FF in bank $00, proposed; Data Crystal: Haggar's
 * special while moving): game_action $16, A+B with left or right held.
 * The first step sets obj_vx $0100; from frame 6, B held alone speeds it
 * up to $0233. Every frame: attack_id $8903[frame] with attack_begin,
 * sound $13 every 8th frame, attack_ground_y = player_ground_y, attack_end
 * from timer 2 of a frame on. At frame $13 timer 4 it is over: game_action,
 * attack_live and combo_timer 0, obj_state[0] $80, the combo restarted
 * (player_stand_anim), player_stand; a lariat that hit something
 * (special_hit) costs 6 health (below 0: 0, never executed), then
 * special_hit 0 and obj_vx $0100. Otherwise Haggar moves: while obj_state
 * bit 1 is clear the way player_dir says, then lariat_step_end (any button
 * held counts the step up); after that left or right held turns him
 * (player_dir) and he steps that way, or on the way he goes.
 */
void func_86FF_b0(void) {
    REGS_IN;

    mff_cycles(0x86FF, 4 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x0F;
    if (a == 0) {
        mff_cycles(0x8706, 4 + 2 + 4 + 6);
        RAM(OBJ_VX_FRAC) = a;
        a = 0x01;
        RAM(OBJ_VX) = a;
        RAM(OBJ_STATE)++;
    } else {
        mff_cycles(0x8704, 1);
    }
    mff_cycles(0x8711, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    c = a >= 6;
    if (c) {
        mff_cycles(0x8718, 3 + 2 + 2);
        a = RAM(JOY_HELD);
        c = a >= LARIAT_SPEED_UP;
        if (a == LARIAT_SPEED_UP) {
            mff_cycles(0x871E, 2 + 4 + 2 + 4);
            a = 0x33;
            RAM(OBJ_VX_FRAC) = a;
            a = 0x02;
            RAM(OBJ_VX) = a;
        } else {
            mff_cycles(0x871C, 1);
        }
    } else {
        mff_cycles(0x8716, 1);
    }
    mff_cycles(0x8728, 4 + 2 + 4);
    a = RAM(OBJ_ANIM_FRAME);
    y = a;
    a = prg_read_ix(LARIAT_ATTACK_ID, y);
    SET_NZ(a);
    mff_cycles(0x872F, 3 + 6);
    RAM(ATTACK_ID) = a;
    JSR_FN(0x8731, func_F43B);                  /* attack_begin */
    mff_cycles(0x8734, 3 + 2 + 2);
    a = RAM(DRAW_PARITY) & 0x07;
    if (a == 0) {
        mff_cycles(0x873A, 2 + 6);
        a = SFX_LARIAT;
        SET_NZ(a);
        JSR_FN(0x873C, func_F34C);              /* sound_queue_push_sfx */
    } else {
        mff_cycles(0x8738, 1);
    }
    mff_cycles(0x873F, 3 + 3 + 4 + 2 + 2);
    a = RAM(PLAYER_GROUND_Y);
    RAM(ATTACK_GROUND_Y) = a;
    a = RAM(OBJ_ANIM_TIMER);
    COMPARE(a, 2);
    if (c) {
        mff_cycles(0x874A, 6);
        JSR_FN(0x874A, func_F436);              /* attack_end */
    } else {
        mff_cycles(0x8748, 1);
    }
    mff_cycles(0x874D, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    c = a >= LARIAT_LAST_FRAME;
    if (a == LARIAT_LAST_FRAME) {
        mff_cycles(0x8754, 4 + 2 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        c = a >= LARIAT_LAST_TIMER;
        if (a == LARIAT_LAST_TIMER) {           /* the lariat is over */
            mff_cycles(0x875B, 2 + 3 + 3 + 3 + 2 + 4 + 3 + 4);
            a = 0;
            RAM(GAME_ACTION) = a;
            RAM(ATTACK_LIVE) = a;
            RAM(COMBO_TIMER) = a;
            a = STATE_LIVE;
            RAM(OBJ_STATE) = a;
            y = RAM(PLAYER_ID);
            a = prg_read_ix(COMBO_FIRST_ANIM, y);
            SET_NZ(a);
            mff_cycles(0x876D, 3 + 6);
            RAM(PLAYER_STAND_ANIM) = a;
            JSR_AT(0x876F, 0xCEAA, 0);          /* player_stand */
            mff_cycles(0x8772, 3 + 2);
            a = RAM(SPECIAL_HIT);
            if (a != 0) {
                mff_cycles(0x8776, 3 + 2 + 2 + 3 + 2);
                a = RAM(HEALTH);
                c = 1;
                SBC(LARIAT_HIT_COST);
                RAM(HEALTH) = a;
                if (n) {
                    mff_cycles(0x877F, 2 + 3);
                    a = 0;
                    RAM(HEALTH) = a;
                } else {
                    mff_cycles(0x877D, 1);
                }
            } else {
                mff_cycles(0x8774, 1);
            }
            mff_cycles(0x8783, 2 + 3 + 4 + 2 + 4 + 6);
            a = 0;
            RAM(SPECIAL_HIT) = a;
            RAM(OBJ_VX_FRAC) = a;
            a = 0x01;
            SET_NZ(a);
            RAM(OBJ_VX) = a;
            RTS();
        }
        mff_cycles(0x8759, 1);
    } else {
        mff_cycles(0x8752, 1);
    }
    mff_cycles(0x8790, 4 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x02;
    if (a == 0) {                               /* the first steps */
        mff_cycles(0x8797, 3 + 2 + 2);
        a = RAM(PLAYER_DIR) & PAD_RIGHT;
        if (a != 0) {
            mff_cycles(0x879D, 2 + 6);
            y = 0;
            SET_NZ(y);
            JSR_FN(0x879F, func_F000);          /* face right, step right */
            mff_cycles(0x87A2, 3);
            JMP_TAIL(0x87AA, 0);                /* lariat_step_end */
        }
        mff_cycles(0x879B, 1 + 2 + 6);
        y = 1;
        SET_NZ(y);
        JSR_FN(0x87A7, func_F051);              /* face left, step left */
        FALL_INTO(func_87AA_b0);
    }
    mff_cycles(0x8795, 1 + 3 + 2 + 2);          /* steering */
    a = RAM(JOY_HELD) & (PAD_LEFT | PAD_RIGHT);
    if (a != 0) {                               /* turned */
        mff_cycles(0x87BE, 3 + 2 + 2);
        RAM(PLAYER_DIR) = a;
        a &= PAD_RIGHT;
        if (a != 0)
            LARIAT_STEP(0x87C4, 1);
        mff_cycles(0x87C2, 1);
        LARIAT_STEP(0x87CA, 0);
    }
    mff_cycles(0x87BC, 1 + 3 + 2 + 2);
    a = RAM(PLAYER_DIR) & PAD_RIGHT;
    if (a != 0)
        LARIAT_STEP(0x87D6, 1);
    mff_cycles(0x87D4, 1);
    LARIAT_STEP(0x87DC, 0);
}

/* lariat_step (00:87C4, 87CA, 87D6, 87DC, inside action_lariat): at pc,
 * LDY #0 / JSR $F000 (face right, step right) or LDY #1 / JSR $F051 (face
 * left, step left), then RTS. */
static void lariat_step(uint16_t pc, int right) {
    REGS_IN;

    mff_cycles(pc, 2 + 6);
    y = right ? 0 : 1;
    SET_NZ(y);
    JSR_FN(pc + 2, right ? func_F000 : func_F051);
    mff_cycles(pc + 5, 6);
    RTS();
}

/*
 * $87AA in bank $00 (proposed: lariat_step_end): the end of a step of
 * Haggar's moving lariat (action_lariat JMPs here at 00:87A2 and runs on
 * into it): while game_action is still $16 and something is held,
 * obj_state[0] counts up.
 */
void func_87AA_b0(void) {
    REGS_IN;

    mff_cycles(0x87AA, 3 + 2 + 2);
    a = RAM(GAME_ACTION);
    COMPARE(a, ACT_LARIAT);
    if (z) {
        mff_cycles(0x87B0, 3 + 2);
        a = RAM(JOY_HELD);
        SET_NZ(a);
        if (!z) {
            mff_cycles(0x87B4, 6);
            RAM(OBJ_STATE)++;
            SET_NZ(RAM(OBJ_STATE));
        } else {
            mff_cycles(0x87B2, 1);
        }
    } else {
        mff_cycles(0x87AE, 1);
    }
    mff_cycles(0x87B7, 6);
    RTS();
}

/* ---- the A+B special, game_action $17 (player_special_start, $CA3C) ---- */

/* the special's handler of each character */
#define SPECIAL_HANDLER_LO  0x87F1   /* F7 64 AA */
#define SPECIAL_HANDLER_HI  0x87F4   /* 87 88 88: $87F7, $8864, $88AA */
/* attack_id by animation frame */
#define SPECIAL_ATTACK_ID   0x88A6   /* 14 15 16 17 (Cody, Guy); Haggar's: LARIAT_ATTACK_ID */
#define ANIM_LARIAT_MOVING  0x4B

static void lariat_start(void);

/*
 * player_special ($87E2 in bank $00, proposed): JMP ($0000) to the
 * character's special from $87F1/$87F4[player_id]: cody_special $87F7,
 * guy_special $8864, haggar_special $88AA. Reached through
 * player_action_dispatch (entry $17).
 * Out: $00/$01 = the handler, A = its high byte.
 */
void func_87E2_b0(void) {
    REGS_IN;

    mff_cycles(0x87E2, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read_ix(SPECIAL_HANDLER_LO, y);
    mff_cycles(0x87E7, 3 + 4);
    RAM(TMP0) = a;
    a = prg_read_ix(SPECIAL_HANDLER_HI, y);
    SET_NZ(a);
    mff_cycles(0x87EC, 3 + 5);
    RAM(TMP1) = a;
    JMP_IND(TMP0);
}

/*
 * cody_special ($87F7 in bank $00, proposed): the attack is on with
 * attack_id $88A6[frame], sound $25 every 8th frame (draw_parity),
 * attack_ground_y = player_ground_y, and the jump obj_fall (Y = 0) carries;
 * once falling to the ground line (set as the knockdown does) it ends
 * (special_end); every frame special_cost.
 */
void func_87F7_b0(void) {
    REGS_IN;

    mff_cycles(0x87F7, 4 + 2 + 4);
    a = RAM(OBJ_ANIM_FRAME);
    y = a;
    a = prg_read_ix(SPECIAL_ATTACK_ID, y);
    SET_NZ(a);
    mff_cycles(0x87FE, 3 + 6);
    RAM(ATTACK_ID) = a;
    JSR_FN(0x8800, func_F43B);                  /* attack_begin */
    mff_cycles(0x8803, 3 + 2 + 2);
    a = RAM(DRAW_PARITY) & 0x07;
    if (a == 0) {
        mff_cycles(0x8809, 2 + 6);
        a = 0x25;
        SET_NZ(a);
        JSR_FN(0x880B, func_F34C);              /* sound_queue_push_sfx */
    } else {
        mff_cycles(0x8807, 1);
    }
    mff_cycles(0x880E, 3 + 3 + 2 + 6);
    a = RAM(PLAYER_GROUND_Y);
    RAM(ATTACK_GROUND_Y) = a;
    y = 0;
    SET_NZ(y);
    JSR_FN(0x8814, func_F119);                  /* obj_fall */
    mff_cycles(0x8817, 4 + 2);
    a = RAM(OBJ_VY);
    if (a & 0x80) {                             /* falling */
        mff_cycles(0x881C, 4 + 2 + 2 + 3 + 2);
        a = RAM(OBJ_Y);
        c = 0;
        ADC(2);
        COMPARE(a, RAM(PLAYER_GROUND_Y));
        if (c) {                                /* at the ground line: over */
            mff_cycles(0x8826, 2);
            if (z) {
                mff_cycles(0x8826, 1 + 4);
                RAM(OBJ_Y) = a;
            } else {
                mff_cycles(0x8828, 2 + 4 + 2);
                SBC(3);
                RAM(OBJ_Y) = a;
                if (a != 0) {
                    mff_cycles(0x882D, 1);
                } else {
                    mff_cycles(0x882F, 4);
                    RAM(OBJ_Y) = a;
                }
            }
            FALL_INTO(func_8832_b0);
        }
        mff_cycles(0x8824, 1);
    } else {
        mff_cycles(0x881A, 1);
    }
    FALL_INTO(func_884E_b0);
}

/* special_end ($8832 in bank $00, proposed): game_action, attack_live and
 * guy_jump_window 0, a small rebound (obj_vy -$40/256), the combo restarts,
 * player_stand, then special_cost. Run on into from cody_special; reached
 * by JMP through $889F. */
void func_8832_b0(void) {
    REGS_IN;

    mff_cycles(0x8832, 2 + 3 + 3 + 3 + 2 + 4 + 2 + 4 + 3 + 4);
    a = 0;
    RAM(GAME_ACTION) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    a = 0xC0;
    RAM(OBJ_VY_FRAC) = a;
    a = 0xFF;
    RAM(OBJ_VY) = a;
    y = RAM(PLAYER_ID);
    a = prg_read_ix(COMBO_FIRST_ANIM, y);
    SET_NZ(a);
    mff_cycles(0x8849, 3 + 6);
    RAM(PLAYER_STAND_ANIM) = a;
    JSR_AT(0x884B, 0xCEAA, 0);                  /* player_stand */
    FALL_INTO(func_884E_b0);
}

/* special_cost ($884E in bank $00, proposed): when the special connected
 * (special_hit) it costs 6 health, never below 0; special_hit is cleared.
 * Out: A = 0. Run on into from cody_special and special_end; reached by
 * JMP from guy_special (00:88A2). */
void func_884E_b0(void) {
    REGS_IN;

    mff_cycles(0x884E, 3 + 2);
    a = RAM(SPECIAL_HIT);
    if (a != 0) {
        mff_cycles(0x8852, 3 + 2 + 2 + 3 + 2);
        a = RAM(HEALTH);
        c = 1;
        SBC(6);
        RAM(HEALTH) = a;
        if (a & 0x80) {
            mff_cycles(0x885B, 2 + 3);
            a = 0;
            RAM(HEALTH) = a;
        } else {
            mff_cycles(0x8859, 1);
        }
    } else {
        mff_cycles(0x8850, 1);
    }
    mff_cycles(0x885F, 2 + 3 + 6);
    a = 0;
    SET_NZ(a);
    RAM(SPECIAL_HIT) = a;
    RTS();
}

/* guy_special ($8864 in bank $00, proposed): as cody_special, ending by JMP
 * (special_end through $889F, special_cost through $88A2). */
void func_8864_b0(void) {
    REGS_IN;

    mff_cycles(0x8864, 4 + 2 + 4);
    a = RAM(OBJ_ANIM_FRAME);
    y = a;
    a = prg_read_ix(SPECIAL_ATTACK_ID, y);
    SET_NZ(a);
    mff_cycles(0x886B, 3 + 6);
    RAM(ATTACK_ID) = a;
    JSR_FN(0x886D, func_F43B);                  /* attack_begin */
    mff_cycles(0x8870, 3 + 2 + 2);
    a = RAM(DRAW_PARITY) & 0x07;
    if (a == 0) {
        mff_cycles(0x8876, 2 + 6);
        a = 0x25;
        SET_NZ(a);
        JSR_FN(0x8878, func_F34C);              /* sound_queue_push_sfx */
    } else {
        mff_cycles(0x8874, 1);
    }
    mff_cycles(0x887B, 3 + 3 + 2 + 6);
    a = RAM(PLAYER_GROUND_Y);
    RAM(ATTACK_GROUND_Y) = a;
    y = 0;
    SET_NZ(y);
    JSR_FN(0x8881, func_F119);                  /* obj_fall */
    mff_cycles(0x8884, 4 + 2);
    a = RAM(OBJ_VY);
    SET_NZ(a);
    if (n) {                                    /* falling */
        mff_cycles(0x8889, 4 + 2 + 2 + 3 + 2);
        a = RAM(OBJ_Y);
        c = 0;
        ADC(2);
        COMPARE(a, RAM(PLAYER_GROUND_Y));
        if (c) {                                /* at the ground line: over */
            mff_cycles(0x8893, 2);
            if (z) {
                mff_cycles(0x8893, 1 + 4);
                RAM(OBJ_Y) = a;
            } else {
                mff_cycles(0x8895, 2 + 4 + 2);
                SBC(3);
                RAM(OBJ_Y) = a;
                if (!z) {
                    mff_cycles(0x889A, 1);
                } else {
                    mff_cycles(0x889C, 4);
                    RAM(OBJ_Y) = a;
                }
            }
            mff_cycles(0x889F, 3);
            JMP_TAIL(0x8832, 0);                /* special_end */
        }
        mff_cycles(0x8891, 1);
    } else {
        mff_cycles(0x8887, 1);
    }
    mff_cycles(0x88A2, 3);
    JMP_TAIL(0x884E, 0);                        /* special_cost */
}

/* $889F in bank $00: JMP special_end; haggar_special branches here. */
void func_889F_b0(void) {
    REGS_IN;

    mff_cycles(0x889F, 3);
    JMP_TAIL(0x8832, 0);                        /* special_end */
}

/*
 * haggar_special ($88AA in bank $00, proposed): A+B with right or left held
 * (joy_held $C1/$C2) starts the moving lariat: obj_state[0] $80,
 * game_action $16, animation $4B. Otherwise the lariat spins in place: on
 * frame 7 at timer 6 it ends (special_end); before that attack_id
 * $8903[frame] with attack_begin, obj_dir flipped every frame, sound $13
 * every 8th frame, attack_ground_y = player_ground_y and attack_end from
 * timer 3 on.
 */
void func_88AA_b0(void) {
    REGS_IN;

    mff_cycles(0x88AA, 3 + 2 + 2);
    a = RAM(JOY_HELD);
    COMPARE(a, PAD_A | PAD_B | PAD_RIGHT);
    if (z) {
        mff_cycles(0x88AE, 1);
        FALL_INTO(lariat_start);
    }
    mff_cycles(0x88B0, 2 + 2);
    COMPARE(a, PAD_A | PAD_B | PAD_LEFT);
    if (z) {
        mff_cycles(0x88B2, 1);
        FALL_INTO(lariat_start);
    }
    mff_cycles(0x88B4, 2 + 1 + 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 7);
    if (z) {
        mff_cycles(0x88CC, 4 + 2 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        COMPARE(a, 6);
        if (z) {                                /* the spin is over */
            mff_cycles(0x88D3, 2 + 1);
            BRANCH_TAIL(0x889F, 0);             /* special_end */
        }
        mff_cycles(0x88D1, 1);
    } else {
        mff_cycles(0x88CA, 1);
    }
    mff_cycles(0x88D5, 4 + 2 + 4);
    a = RAM(OBJ_ANIM_FRAME);
    y = a;
    a = prg_read_ix(LARIAT_ATTACK_ID, y);
    SET_NZ(a);
    mff_cycles(0x88DC, 3 + 6);
    RAM(ATTACK_ID) = a;
    JSR_FN(0x88DE, func_F43B);                  /* attack_begin */
    mff_cycles(0x88E1, 4 + 2 + 4 + 3 + 2 + 2);
    a = RAM(OBJ_DIR) ^ (PAD_LEFT | PAD_RIGHT);
    RAM(OBJ_DIR) = a;
    a = RAM(DRAW_PARITY) & 0x07;
    if (a == 0) {
        mff_cycles(0x88EF, 2 + 6);
        a = 0x13;
        SET_NZ(a);
        JSR_FN(0x88F1, func_F34C);              /* sound_queue_push_sfx */
    } else {
        mff_cycles(0x88ED, 1);
    }
    mff_cycles(0x88F4, 3 + 3 + 4 + 2 + 2);
    a = RAM(PLAYER_GROUND_Y);
    RAM(ATTACK_GROUND_Y) = a;
    a = RAM(OBJ_ANIM_TIMER);
    COMPARE(a, 3);
    if (c) {
        mff_cycles(0x88FF, 6);
        JSR_FN(0x88FF, func_F436);              /* attack_end */
    } else {
        mff_cycles(0x88FD, 2);
    }
    mff_cycles(0x8902, 6);
    RTS();
}

/* lariat_start (00:88B6, inside haggar_special): obj_state[0] $80,
 * game_action $16, animation $4B. */
static void lariat_start(void) {
    REGS_IN;

    mff_cycles(0x88B6, 2 + 4 + 2 + 3 + 2 + 6);
    a = STATE_LIVE;
    RAM(OBJ_STATE) = a;
    a = ACT_LARIAT;
    RAM(GAME_ACTION) = a;
    a = ANIM_LARIAT_MOVING;
    SET_NZ(a);
    JSR_FN(0x88C1, func_F294);                  /* player_set_anim */
    mff_cycles(0x88C4, 6);
    RTS();
}
