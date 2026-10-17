/*
 * player_state_idle.c - game_action 0: the player standing or walking, and
 * what the buttons start from there (0E:CA20-CCB4)
 *
 * player_state_idle ($CA20) runs every frame the player is free. In order:
 *
 *   $CA20  player_invuln and combo_timer count down; A starts a jump, A+B
 *          the special, anything else goes to the walk speeds
 *   $CA3C  player_special_start: the A+B special (game_action $17)
 *   $CAA2  player_walk_speed: the character's walking speed, slower on a
 *          diagonal ($CAC8 is the last compare of that test)
 *   $CAE5  player_idle_buttons: B starts an attack, nothing walks
 *          (player_walk, $CCB5, in player_state_walk.c)
 *   $CB07  player_attack_start: with a weapon, its swing or throw; without,
 *          the first punch ($CCA2) or the next hit of a combo ($CB22),
 *          which at the third hit may become a grab ($CBA3)
 *
 * The ROM goes from one part to the next by JMP, branch or by running on,
 * and the scaffold made a function of each place a JMP lands. A JMP the
 * generator turned into a tail call costs 2 more cycles; where it kept the
 * code inline (a branch, running on, some JMPs) the C calls the next part
 * with no charge. Which JMP is which follows the lifted code; the comments
 * say "tail call" where the 2 cycles are paid.
 *
 * $CA39, $CAF5 and $CBA0 are JMPs the branches use to reach code beyond
 * their range ("islands").
 */
#include "player_defs.h"

void func_CA3C(void);
void func_CAC8(void);
void func_CAE5(void);
void func_CB07(void);
void func_CB22(void);
void func_CBA0(void);
void func_CAF5(void);
void func_CF33(void);

/* ---- per-character tables (3 bytes: Cody, Guy, Haggar) ---- */
#define ANIM_SPECIAL        0xE228   /* 6C 6A 57: the A+B special */
#define CHR_SPECIAL         0xE270   /* 0E 17 0F */
#define WALK_DIAG_FRAC      0xE1D7   /* D9 0F B5: speed on a diagonal, x and y alike */
#define WALK_DIAG           0xE1DA   /* 00 01 00 */
#define ANIM_PUNCH          0xE1EF   /* 0D 0E 0F: the first punch */
#define CHR_PUNCH           0xE237   /* 02 06 09: the first punch's CHR bank (and a combo
                                        restarting from it) */
#define CHR_COMBO           0xE23A   /* 03 07 09: the other hits of a combo */
#define CHR_COMBO_HOP       0xE23D   /* 03 07 10: the combo animation $15 (Haggar's third,
                                        $E1F5) with its hop */
#define COMBO_GRAB_STAND    0xE291   /* 13 14 15: player_stand_anim at which B with Up or Down
                                        grabs combo_target instead of hitting */
#define ANIM_COMBO_GRAB     0xE216   /* 34 35 36 */
#define CHR_COMBO_GRAB      0xE25E   /* 0C 14 0B */
#define ANIM_WEAPON_SWING   0xE222   /* 45 46 47: Cody swings his weapon */
#define CHR_WEAPON_SWING    0xE26A   /* 03 16 0B */
#define ANIM_WEAPON_USE     0xE225   /* 4A 46 47: game_action $11 */
#define CHR_WEAPON_USE      0xE26D   /* 03 16 11 */
/* the grabbed enemy's place, 15 px in front: + $000F facing right (Y = 0),
 * - $000F facing left (Y = 2) */
#define GRAB_OFFSET_LO      0xE1D3   /* 0F 00 F1 */
#define GRAB_OFFSET_HI      0xE1D4   /* 00 F1 FF */

/* the weapon action, handled in bank $00 (Guy's knives count weapon_ammo
 * down there, 00:8292) */
#define ACT_WEAPON          0x11

/*
 * player_state_idle ($CA20, labels.toml): game_action 0 (player_state's
 * table, entry 0).
 *  - player_invuln and combo_timer count down to 0;
 *  - A ($80 in joy_pressed, nothing else) jumps: vy $04.38, the jump
 *    animation and CHR bank, game_action 1, player_ground_y and
 *    attack_ground_y = obj_y, sound $1B, and player_state_jump ($CF88) runs
 *    at once (tail call). With B held as well and health left it is the
 *    special instead;
 *  - A+B pressed together: player_special_start ($CA3C);
 *  - anything else: player_walk_speed ($CAA2, tail call).
 */
void func_CA20(void) {
    REGS_IN;

    mff_cycles(0xCA20, 3 + 2);
    a = RAM(PLAYER_INVULN);
    if (a != 0) {
        mff_cycles(0xCA24, 5);
        RAM(PLAYER_INVULN)--;
    } else {
        mff_cycles(0xCA22, 1);
    }
    mff_cycles(0xCA26, 3 + 2);
    a = RAM(COMBO_TIMER);
    if (a != 0) {
        mff_cycles(0xCA2A, 5);
        RAM(COMBO_TIMER)--;
    } else {
        mff_cycles(0xCA28, 1);
    }

    mff_cycles(0xCA2C, 3 + 2 + 2);
    a = RAM(JOY_PRESSED);
    COMPARE(a, PAD_A);
    if (!z) {
        mff_cycles(0xCA32, 2 + 2);
        COMPARE(a, PAD_A | PAD_B);
        if (!z) {
            mff_cycles(0xCA36, 3);
            JMP_TAIL(0xCAA2, -1);               /* player_walk_speed */
        }
        mff_cycles(0xCA34, 1);
        FALL_INTO(func_CA3C);                   /* player_special_start */
    }

    /* A: a jump, or the special with B held */
    mff_cycles(0xCA30, 1 + 3 + 2);
    a = RAM(HEALTH);
    if (a != 0) {
        mff_cycles(0xCA71, 3 + 2 + 2);
        a = RAM(JOY_HELD) & PAD_B;
        if (a != 0) {
            mff_cycles(0xCA75, 1);
            FALL_INTO(func_CA3C);               /* player_special_start */
        }
    } else {
        mff_cycles(0xCA6F, 1);
    }
    mff_cycles(0xCA77, 2 + 4 + 2 + 4 + 3 + 4);
    a = 0x38;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x04;
    RAM(OBJ_VY) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(ANIM_JUMP + y));
    SET_NZ(a);
    mff_cycles(0xCA86, 6);
    JSR_FN(0xCA86, func_F294);                  /* player_set_anim */
    mff_cycles(0xCA89, 4);
    a = prg_read((uint16_t)(CHR_JUMP + y));
    SET_NZ(a);
    mff_cycles(0xCA8C, 6);
    JSR_FN(0xCA8C, func_F440);                  /* player_set_chr */
    mff_cycles(0xCA8F, 2 + 3 + 4 + 3 + 3 + 2 + 6);
    a = ACT_JUMP;
    RAM(GAME_ACTION) = a;
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    RAM(ATTACK_GROUND_Y) = a;
    a = SFX_JUMP;
    SET_NZ(a);
    JSR_FN(0xCA9C, func_F34C);                  /* sound_queue_push_sfx */
    mff_cycles(0xCA9F, 3);
    JMP_TAIL(0xCF88, -1);                       /* player_state_jump */
}

/*
 * $CA39: JMP player_attack_start ($CB07), the target of
 * player_special_start's two branches (tail call).
 */
void func_CA39(void) {
    REGS_IN;

    mff_cycles(0xCA39, 3);
    JMP_TAIL(0xCB07, -1);
}

/*
 * player_special_start ($CA3C, proposed): the A+B special. With no health
 * left, or unk_b2 negative (set to $FF at 00:94F8 with area_clear $7F,
 * cleared at a new game, 0E:C821), it is an attack instead
 * (player_attack_start through $CA39). Otherwise Cody and Guy go up with vy $04.38, the special's
 * animation and CHR bank are set ($E228/$E270), the attack begins
 * (attack_begin, $F43B: attack_live $1E), game_action $17 (bank $00 runs
 * it) and player_ground_y = obj_y.
 * Reached from player_state_idle (BEQ at 0E:CA34, BNE at CA75), $CAF5, and
 * by JMP from player_state_punch and player_state_hit (0E:D32E, DA00).
 * Out: A = player_ground_y.
 */
void func_CA3C(void) {
    REGS_IN;

    mff_cycles(0xCA3C, 3 + 2);
    a = RAM(HEALTH);
    SET_NZ(a);
    if (z) {
        mff_cycles(0xCA3E, 1);
        FALL_INTO(func_CA39);
    }
    mff_cycles(0xCA40, 3 + 2);
    a = RAM(UNK_B2);
    SET_NZ(a);
    if (n) {
        mff_cycles(0xCA42, 1);
        FALL_INTO(func_CA39);
    }
    mff_cycles(0xCA44, 3 + 2 + 2);
    y = RAM(PLAYER_ID);
    COMPARE(y, PLAYER_HAGGAR);
    if (!z) {                                   /* Cody and Guy leave the ground */
        mff_cycles(0xCA4A, 2 + 4 + 2 + 4);
        a = 0x38;
        RAM(OBJ_VY_FRAC) = a;
        a = 0x04;
        RAM(OBJ_VY) = a;
    } else {
        mff_cycles(0xCA48, 1);
    }
    mff_cycles(0xCA54, 4);
    a = prg_read((uint16_t)(ANIM_SPECIAL + y));
    SET_NZ(a);
    mff_cycles(0xCA57, 6);
    JSR_FN(0xCA57, func_F294);                  /* player_set_anim */
    mff_cycles(0xCA5A, 4);
    a = prg_read((uint16_t)(CHR_SPECIAL + y));
    SET_NZ(a);
    mff_cycles(0xCA5D, 6);
    JSR_FN(0xCA5D, func_F440);                  /* player_set_chr */
    mff_cycles(0xCA60, 6);
    JSR_FN(0xCA60, func_F43B);                  /* attack_begin */
    mff_cycles(0xCA63, 2 + 3 + 4 + 3 + 6);
    a = ACT_SPECIAL;
    RAM(GAME_ACTION) = a;
    a = RAM(OBJ_Y);
    SET_NZ(a);
    RAM(PLAYER_GROUND_Y) = a;
    RTS();
}

/* $CAD1: a diagonal (Up or Down with Left or Right) walks at $E1D7/$E1DA
 * in x and y alike; then player_idle_buttons, which the ROM runs on into */
static void walk_diagonal_speed(void) {
    REGS_IN;

    mff_cycles(0xCAD1, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(WALK_DIAG_FRAC + y));
    mff_cycles(0xCAD6, 4 + 4 + 4);
    RAM(OBJ_VX_FRAC) = a;
    RAM(OBJ_VY_FRAC) = a;
    a = prg_read((uint16_t)(WALK_DIAG + y));
    mff_cycles(0xCADF, 4 + 4);
    RAM(OBJ_VX) = a;
    RAM(OBJ_VY) = a;
    FALL_INTO(func_CAE5);                       /* player_idle_buttons */
}

/*
 * player_walk_speed ($CAA2, proposed): obj_vx and obj_vy of slot 0 from the
 * character's walking speed (Cody 1.30 and 1.0, Guy 1.50 and 1.20, Haggar
 * 1.10 and 0.80 px a frame), or the diagonal speed when joy_held is exactly
 * a diagonal ($09, $05, $0A, $06), then player_idle_buttons.
 * Reached by JMP from player_state_idle (0E:CA36, tail call). Its last
 * compare, CMP #$0A, runs on into the BEQ at $CAC8.
 */
void func_CAA2(void) {
    REGS_IN;

    mff_cycles(0xCAA2, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(WALK_VX_FRAC + y));
    mff_cycles(0xCAA7, 4 + 4);
    RAM(OBJ_VX_FRAC) = a;
    a = prg_read((uint16_t)(WALK_VX + y));
    mff_cycles(0xCAAD, 4 + 4);
    RAM(OBJ_VX) = a;
    a = prg_read((uint16_t)(WALK_VY_FRAC + y));
    mff_cycles(0xCAB3, 4 + 4);
    RAM(OBJ_VY_FRAC) = a;
    a = prg_read((uint16_t)(WALK_VY + y));
    mff_cycles(0xCAB9, 4 + 3 + 2 + 2);
    RAM(OBJ_VY) = a;
    a = RAM(JOY_HELD);
    COMPARE(a, PAD_UP | PAD_RIGHT);
    if (z) {
        mff_cycles(0xCAC0, 1);
    } else {
        mff_cycles(0xCAC2, 2 + 2);
        COMPARE(a, PAD_DOWN | PAD_RIGHT);
        if (!z) {
            mff_cycles(0xCAC6, 2);
            COMPARE(a, PAD_UP | PAD_LEFT);
            FALL_INTO(func_CAC8);
        }
        mff_cycles(0xCAC4, 1);
    }
    SYNC_OUT;
    walk_diagonal_speed();
}

/*
 * $CAC8: BEQ $CAD1, the end of player_walk_speed's diagonal test (Z from
 * CMP #$0A; then CMP #$06). Nothing jumps here; it is an entry of the P0
 * map.
 */
void func_CAC8(void) {
    REGS_IN;

    mff_cycles(0xCAC8, 2);
    if (!z) {
        mff_cycles(0xCACA, 2 + 2);
        COMPARE(a, PAD_DOWN | PAD_LEFT);
        if (!z) {
            mff_cycles(0xCACE, 3);
            JMP_TAIL(0xCAE5, -1);               /* player_idle_buttons */
        }
        mff_cycles(0xCACC, 1);
    } else {
        mff_cycles(0xCAC8, 1);
    }
    SYNC_OUT;
    walk_diagonal_speed();
}

/*
 * player_idle_buttons ($CAE5, proposed): joy_pressed exactly
 *  - B: player_ground_y = obj_y, then the special if A+B are held (and
 *    health is left; through $CAF5), else player_attack_start ($CB07);
 *  - A: JMP $CCDA, walking with no animation change (never executed:
 *    player_state_idle took A already at 0E:CA2E);
 *  - anything else: player_walk ($CCB5, tail call).
 * Reached from player_walk_speed (JMP at 0E:CACE, tail call, or running on
 * from its diagonal speeds).
 */
void func_CAE5(void) {
    REGS_IN;

    mff_cycles(0xCAE5, 3 + 2 + 2);
    a = RAM(JOY_PRESSED);
    COMPARE(a, PAD_B);
    if (!z) {
        mff_cycles(0xCAEB, 2 + 2);
        COMPARE(a, PAD_A);
        if (!z) {
            mff_cycles(0xCAEF, 3);
            JMP_TAIL(0xCCB5, -1);               /* player_walk */
        }
        mff_cycles(0xCAED, 1 + 3);
        JMP_TAIL(0xCCDA, -1);                   /* player_walk_move */
    }
    mff_cycles(0xCAE9, 1 + 4 + 3 + 3 + 2);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(HEALTH);
    if (a != 0) {
        mff_cycles(0xCB01, 3 + 2 + 2);
        a = RAM(JOY_HELD);
        COMPARE(a, PAD_A | PAD_B);
        if (z) {
            mff_cycles(0xCB05, 2);              /* BEQ $CAF5 crosses a page */
            FALL_INTO(func_CAF5);
        }
    } else {
        mff_cycles(0xCAFF, 1);
    }
    FALL_INTO(func_CB07);                       /* player_attack_start */
}

/* $CAF5: JMP player_special_start ($CA3C), for player_idle_buttons' BEQ
 * at 0E:CB05 (tail call) */
void func_CAF5(void) {
    REGS_IN;

    mff_cycles(0xCAF5, 3);
    JMP_TAIL(0xCA3C, -1);
}

/*
 * player_attack_start ($CB07, proposed): B in player_state_idle.
 *  - With a weapon (weapon_held; never executed in the CDL runs): Cody,
 *    with an enemy near (weapon_target_near, $CF33), swings it ($E222,
 *    game_action 3 through $CB99); otherwise, and always for Guy and
 *    Haggar, game_action $11 with animation $E225 (bank $00 uses the
 *    weapon).
 *  - No combo running (combo_timer 0): the first punch ($CCA2, tail call).
 *  - In a combo: combo_timer = 0 and, at the stance of the third hit
 *    (player_stand_anim = $E291[player_id]), the combo grab ($CBA3, tail
 *    call); otherwise the next hit ($CB22).
 * Reached through $CA39, from player_idle_buttons, and by JMP from the hit
 * position state (0E:DDC1, DE30).
 */
void func_CB07(void) {
    REGS_IN;

    mff_cycles(0xCB07, 3 + 2);
    a = RAM(WEAPON_HELD);
    if (a != 0) {
        mff_cycles(0xCB09, 1 + 3 + 2 + 2);
        a = RAM(PLAYER_ID);
        COMPARE(a, PLAYER_GUY);
        if (z) {
            mff_cycles(0xCB6A, 1);
        } else {
            mff_cycles(0xCB6C, 2 + 2);
            COMPARE(a, PLAYER_HAGGAR);
            if (z) {
                mff_cycles(0xCB6E, 1);
            } else {                            /* Cody */
                mff_cycles(0xCB70, 6);
                JSR_AT(0xCB70, 0xCF33, -1);     /* weapon_target_near */
                mff_cycles(0xCB73, 2);
                if (!z) {                       /* an enemy near: swing */
                    mff_cycles(0xCB75, 3 + 4);
                    y = RAM(PLAYER_ID);
                    a = prg_read((uint16_t)(ANIM_WEAPON_SWING + y));
                    SET_NZ(a);
                    mff_cycles(0xCB7A, 6);
                    JSR_FN(0xCB7A, func_F294);  /* player_set_anim */
                    mff_cycles(0xCB7D, 4);
                    a = prg_read((uint16_t)(CHR_WEAPON_SWING + y));
                    SET_NZ(a);
                    mff_cycles(0xCB80, 6);
                    JSR_FN(0xCB80, func_F440);  /* player_set_chr */
                    mff_cycles(0xCB83, 3);
                    JMP_TAIL(0xCB99, -1);       /* player_attack_go */
                }
                mff_cycles(0xCB73, 1);
            }
        }
        mff_cycles(0xCB86, 3 + 4);
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(ANIM_WEAPON_USE + y));
        SET_NZ(a);
        mff_cycles(0xCB8B, 6);
        JSR_FN(0xCB8B, func_F294);              /* player_set_anim */
        mff_cycles(0xCB8E, 4);
        a = prg_read((uint16_t)(CHR_WEAPON_USE + y));
        SET_NZ(a);
        mff_cycles(0xCB91, 6);
        JSR_FN(0xCB91, func_F440);              /* player_set_chr */
        mff_cycles(0xCB94, 2 + 3 + 6);
        a = ACT_WEAPON;
        SET_NZ(a);
        RAM(GAME_ACTION) = a;
        RTS();
    }

    mff_cycles(0xCB0B, 3 + 2);
    a = RAM(COMBO_TIMER);
    SET_NZ(a);
    if (z) {
        mff_cycles(0xCB0F, 3);
        JMP_TAIL(0xCCA2, -1);                   /* player_punch_start */
    }
    mff_cycles(0xCB0D, 1 + 2 + 3 + 3 + 3 + 4);
    a = 0;
    RAM(COMBO_TIMER) = a;
    y = RAM(PLAYER_ID);
    a = RAM(PLAYER_STAND_ANIM);
    COMPARE(a, prg_read((uint16_t)(COMBO_GRAB_STAND + y)));
    mff_cycles(0xCB1D, 2);
    if (z) {
        mff_cycles(0xCB1F, 3);
        JMP_TAIL(0xCBA3, -1);                   /* player_combo_grab */
    }
    mff_cycles(0xCB1D, 1);
    FALL_INTO(func_CB22);                       /* player_combo_next */
}

/*
 * player_combo_next ($CB22, proposed): the next hit of a combo. The
 * animation becomes player_stand_anim (the stance the last hit left). From
 * the first or the second stance ($E288, $E28B) the animation restarts at
 * frame 1 with CHR bank $E237; animation $15 (Haggar's) hops (vy $02.E8,
 * player_ground_y = obj_y) with CHR bank $E23D; the others take $E23A. Then
 * player_attack_go ($CB99, tail call).
 * Reached from player_attack_start, $CBA0 (tail call) and
 * player_combo_grab (JMP at 0E:CC26, CC7F, inline).
 */
void func_CB22(void) {
    REGS_IN;

    mff_cycles(0xCB22, 3 + 6);
    a = RAM(PLAYER_STAND_ANIM);
    SET_NZ(a);
    JSR_FN(0xCB24, func_F294);                  /* player_set_anim */
    mff_cycles(0xCB27, 3 + 4 + 4);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    COMPARE(a, prg_read((uint16_t)(COMBO_FIRST_ANIM + y)));
    mff_cycles(0xCB2F, 2);
    if (z) {
        mff_cycles(0xCB2F, 1);
        goto restart;
    }
    mff_cycles(0xCB31, 4);
    COMPARE(a, prg_read((uint16_t)(TURN_ANIM_2 + y)));
    mff_cycles(0xCB34, 2);
    if (z) {
        mff_cycles(0xCB34, 1);
        goto restart;
    }
    mff_cycles(0xCB36, 2 + 2);
    COMPARE(a, 0x15);
    if (!z) {
        mff_cycles(0xCB3A, 4);
        a = prg_read((uint16_t)(CHR_COMBO + y));
        SET_NZ(a);
        mff_cycles(0xCB3D, 2);
        if (!z) {                               /* always: no 0 in the table */
            mff_cycles(0xCB3D, 1);
            goto set_chr;
        }
    } else {
        mff_cycles(0xCB38, 1);
    }
    mff_cycles(0xCB3F, 2 + 4 + 2 + 4 + 4 + 3 + 4);
    a = 0xE8;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x02;
    RAM(OBJ_VY) = a;
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = prg_read((uint16_t)(CHR_COMBO_HOP + y));
    SET_NZ(a);
    mff_cycles(0xCB51, 2);
    if (!z) {                                   /* always: no 0 in the table */
        mff_cycles(0xCB51, 1);
        goto set_chr;
    }
restart:
    mff_cycles(0xCB53, 2 + 4 + 2 + 4 + 4);
    a = 0x01;
    RAM(OBJ_ANIM_FRAME) = a;
    a = 0x00;
    RAM(OBJ_ANIM_TIMER) = a;
    a = prg_read((uint16_t)(CHR_PUNCH + y));
    SET_NZ(a);
set_chr:
    mff_cycles(0xCB60, 6);
    JSR_FN(0xCB60, func_F440);                  /* player_set_chr */
    mff_cycles(0xCB63, 3);
    JMP_TAIL(0xCB99, -1);                       /* player_attack_go */
}

/*
 * player_attack_go ($CB99, proposed): game_action 3 and JMP
 * player_state_punch ($D30F, tail call), which runs this frame.
 * Reached by JMP from player_combo_next and the weapon swing (0E:CB63,
 * CB83; tail calls).
 */
void func_CB99(void) {
    REGS_IN;

    mff_cycles(0xCB99, 2 + 3 + 3);
    a = ACT_ATTACK;
    SET_NZ(a);
    RAM(GAME_ACTION) = a;
    JMP_TAIL(0xD30F, -1);                       /* player_state_punch */
}

/* $CBA0: JMP player_combo_next ($CB22), the target of player_combo_grab's
 * branches (tail call) */
void func_CBA0(void) {
    REGS_IN;

    mff_cycles(0xCBA0, 3);
    JMP_TAIL(0xCB22, -1);
}

/* combo_target kinds the combo grab never takes (obj_id) */
static const uint8_t k_no_combo_grab[] = { 0x06, 0x03, 0x05, 0x07, 0x16, 0x09, 0x6C, 0x2E, 0x9D };

/*
 * player_combo_grab ($CBA3, proposed): B at the third hit's stance. The
 * enemy the last hit struck (combo_target) is grabbed if
 *  - it is in slots 3-$0D, live, and not one of the kinds above;
 *  - $04FF (a boss's slot pointer, bank $0B) is 0, or that slot's obj_life
 *    is $26 or more;
 *  - |dx| < $20 and |dy| < $14 from the player;
 *  - joy_held is exactly B+Down or B+Up.
 * Otherwise it is the next hit (player_combo_next, through $CBA0 or JMP
 * $CB22 inline). The grab: animation $E216, CHR bank $E25E, grab_slot =
 * the enemy, which takes the player's obj_y;
 *  - Cody: an enemy facing the player's way is put 15 px in front
 *    (player_grab_place, $CC85), the other is turned to face that way
 *    (obj_dir); then Cody's grab, $D7F2 (tail call);
 *  - Guy: an enemy facing his way is put in front; then $D88C (tail call);
 *  - Haggar: an enemy facing his way is not grabbed after all (JMP $CB22,
 *    after the animation and grab_slot were set); else $D8A8 (tail call).
 * Reached by JMP from player_attack_start (0E:CB1F, tail call).
 */
void func_CBA3(void) {
    REGS_IN;
    int i;
    uint16_t pc;

    mff_cycles(0xCBA3, 3 + 2 + 2);
    y = RAM(COMBO_TARGET);
    COMPARE(y, 0);
    if (z) {
        mff_cycles(0xCBA7, 1);
        FALL_INTO(func_CBA0);
    }
    mff_cycles(0xCBA9, 2 + 2);
    COMPARE(y, 1);
    if (z) {
        mff_cycles(0xCBAB, 1);
        FALL_INTO(func_CBA0);
    }
    mff_cycles(0xCBAD, 2 + 2);
    COMPARE(y, 2);
    if (z) {
        mff_cycles(0xCBAF, 1);
        FALL_INTO(func_CBA0);
    }
    mff_cycles(0xCBB1, 2 + 2);
    COMPARE(y, 0x0E);
    if (c) {
        mff_cycles(0xCBB3, 1);
        FALL_INTO(func_CBA0);
    }
    mff_cycles(0xCBB5, 4 + 2);
    a = RAM(OBJ_STATE + y);
    SET_NZ(a);
    if (!n) {
        mff_cycles(0xCBB8, 1);
        FALL_INTO(func_CBA0);
    }
    mff_cycles(0xCBBA, 4);
    a = RAM(OBJ_ID + y);
    for (i = 0, pc = 0xCBBD; i < (int)sizeof k_no_combo_grab; i++, pc += 4) {
        mff_cycles(pc, 2 + 2);                  /* CMP #id, BEQ $CBA0 */
        COMPARE(a, k_no_combo_grab[i]);
        if (z) {
            mff_cycles(pc + 2, 1);
            FALL_INTO(func_CBA0);
        }
    }
    mff_cycles(0xCBE1, 4 + 2);
    a = RAM(OBJ_SPAWN_GROUP + 15);
    if (a != 0) {
        mff_cycles(0xCBE6, 2 + 4 + 2 + 2);
        y = a;
        a = RAM(OBJ_LIFE + y);
        COMPARE(a, 0x26);
        if (!c) {
            mff_cycles(0xCBEC, 1);
            FALL_INTO(func_CBA0);
        }
    } else {
        mff_cycles(0xCBE4, 1);
    }

    /* A = |obj_x - obj_x[target]|, the low byte of the 16-bit difference */
    mff_cycles(0xCBEE, 3 + 4 + 2 + 4 + 3 + 4 + 4 + 4 + 2);
    y = RAM(COMBO_TARGET);
    a = RAM(OBJ_X);
    c = 1;
    SBC(RAM(OBJ_X + y));
    mff_push(a);
    a = RAM(OBJ_X_HI);
    SBC(RAM(OBJ_X_HI + y));
    a = mff_pull();
    if (!c) {
        mff_cycles(0xCC01, 2 + 2 + 2);
        a ^= 0xFF;
        ADC(1);
        c = 0;
    } else {
        mff_cycles(0xCBFF, 1);
    }
    mff_cycles(0xCC06, 2 + 2);
    COMPARE(a, 0x20);
    if (c) {
        mff_cycles(0xCC08, 2);                  /* BCS $CBA0 crosses a page */
        FALL_INTO(func_CBA0);
    }
    /* A = |obj_y - obj_y[target]| */
    mff_cycles(0xCC0A, 4 + 2 + 4 + 2);
    a = RAM(OBJ_Y);
    c = 1;
    SBC(RAM(OBJ_Y + y));
    if (!c) {
        mff_cycles(0xCC13, 2 + 2 + 2);
        a ^= 0xFF;
        ADC(1);
        c = 0;
    } else {
        mff_cycles(0xCC11, 1);
    }
    mff_cycles(0xCC18, 2 + 2);
    COMPARE(a, 0x14);
    if (c) {
        mff_cycles(0xCC1A, 2);                  /* BCS $CBA0 crosses a page */
        FALL_INTO(func_CBA0);
    }
    mff_cycles(0xCC1C, 3 + 2 + 2);
    a = RAM(JOY_HELD);
    COMPARE(a, PAD_B | PAD_DOWN);
    if (!z) {
        mff_cycles(0xCC22, 2 + 2);
        COMPARE(a, PAD_B | PAD_UP);
        if (!z) {
            mff_cycles(0xCC26, 3);
            FALL_INTO(func_CB22);               /* player_combo_next */
        }
        mff_cycles(0xCC24, 1);
    } else {
        mff_cycles(0xCC20, 1);
    }

    /* the grab */
    mff_cycles(0xCC29, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(ANIM_COMBO_GRAB + y));
    SET_NZ(a);
    mff_cycles(0xCC2E, 6);
    JSR_FN(0xCC2E, func_F294);                  /* player_set_anim */
    mff_cycles(0xCC31, 4);
    a = prg_read((uint16_t)(CHR_COMBO_GRAB + y));
    SET_NZ(a);
    mff_cycles(0xCC34, 6);
    JSR_FN(0xCC34, func_F440);                  /* player_set_chr */
    mff_cycles(0xCC37, 3 + 3 + 3 + 2);
    y = RAM(COMBO_TARGET);
    RAM(GRAB_SLOT) = y;
    a = RAM(PLAYER_ID);
    if (a == PLAYER_CODY) {
        int place;

        mff_cycles(0xCC3F, 4 + 2 + 3 + 2);
        a = RAM(OBJ_DIR + y) & (PAD_LEFT | PAD_RIGHT);
        COMPARE(a, RAM(PLAYER_DIR));
        if (z) {                                /* facing Cody's way: in front */
            mff_cycles(0xCC46, 1);
            place = 1;
        } else {                                /* turned to face Cody's way */
            mff_cycles(0xCC48, 3 + 5 + 2);
            a = RAM(PLAYER_DIR);
            SET_NZ(a);
            RAM(OBJ_DIR + y) = a;
            place = z;                          /* player_dir 0 runs on: never */
            if (!z)
                mff_cycles(0xCC4D, 1);
        }
        if (place) {
            mff_cycles(0xCC4F, 6);
            JSR_AT(0xCC4F, 0xCC85, -1);         /* player_grab_place */
        }
    } else {
        mff_cycles(0xCC3D, 1);
    }
    mff_cycles(0xCC52, 4 + 5 + 3 + 2 + 2);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + y) = a;
    a = RAM(PLAYER_ID);
    COMPARE(a, PLAYER_GUY);
    if (z) {
        mff_cycles(0xCC5C, 1 + 4 + 2 + 3 + 2);
        a = RAM(OBJ_DIR + y) & (PAD_LEFT | PAD_RIGHT);
        COMPARE(a, RAM(PLAYER_DIR));
        if (z) {
            mff_cycles(0xCC6C, 1 + 6);
            JSR_AT(0xCC70, 0xCC85, -1);         /* player_grab_place */
        } else {
            mff_cycles(0xCC6E, 2 + 1);
        }
        mff_cycles(0xCC73, 3);
        JMP_TAIL(0xD88C, -1);                   /* Guy's grab */
    }
    mff_cycles(0xCC5E, 2 + 2);
    COMPARE(a, PLAYER_HAGGAR);
    if (!z) {
        mff_cycles(0xCC62, 3);
        JMP_TAIL(0xD7F2, -1);                   /* Cody's grab */
    }
    mff_cycles(0xCC60, 1 + 4 + 2 + 3 + 2);
    a = RAM(OBJ_DIR + y) & (PAD_LEFT | PAD_RIGHT);
    COMPARE(a, RAM(PLAYER_DIR));
    if (z) {
        mff_cycles(0xCC7F, 3);
        FALL_INTO(func_CB22);                   /* player_combo_next */
    }
    mff_cycles(0xCC7D, 1 + 3);
    JMP_TAIL(0xD8A8, -1);                       /* Haggar's grab */
}

/*
 * player_grab_place ($CC85, proposed): the held enemy (grab_slot) 15 px in
 * front of the player: obj_x/obj_x_hi = the player's + $E1D3/$E1D4 by
 * player_dir & 2. Called by player_combo_grab (0E:CC4F, CC70).
 * Out: A = the enemy's obj_x, Y = grab_slot.
 */
void func_CC85(void) {
    REGS_IN;

    mff_cycles(0xCC85, 3 + 2 + 2 + 4 + 2 + 4);
    a = RAM(PLAYER_DIR) & PAD_LEFT;
    y = a;
    a = RAM(OBJ_X);
    c = 0;
    ADC(prg_read((uint16_t)(GRAB_OFFSET_LO + y)));
    mff_cycles(0xCC91, 3 + 4 + 4);
    mff_push(a);
    a = RAM(OBJ_X_HI);
    ADC(prg_read((uint16_t)(GRAB_OFFSET_HI + y)));
    mff_cycles(0xCC98, 3 + 5 + 4 + 5 + 6);
    y = RAM(GRAB_SLOT);
    RAM(OBJ_X_HI + y) = a;
    a = mff_pull();
    SET_NZ(a);
    RAM(OBJ_X + y) = a;
    RTS();
}

/*
 * player_punch_start ($CCA2, proposed): the first punch, game_action 3 with
 * animation $E1EF and CHR bank $E237; runs on into player_state_default's
 * RTS. Reached by JMP from player_attack_start (0E:CB0F, tail call).
 */
void func_CCA2(void) {
    REGS_IN;

    mff_cycles(0xCCA2, 2 + 3 + 3 + 4);
    a = ACT_ATTACK;
    RAM(GAME_ACTION) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(ANIM_PUNCH + y));
    SET_NZ(a);
    mff_cycles(0xCCAB, 6);
    JSR_FN(0xCCAB, func_F294);                  /* player_set_anim */
    mff_cycles(0xCCAE, 4);
    a = prg_read((uint16_t)(CHR_PUNCH + y));
    SET_NZ(a);
    mff_cycles(0xCCB1, 6);
    JSR_FN(0xCCB1, func_F440);                  /* player_set_chr */
    mff_cycles(0xCCB4, 6);
    RTS();
}

/* object ids weapon_target_near ignores */
static const uint8_t k_weapon_ignores[] = { 0x69, 0x10, 0x11, 0x12, 0x13 };

/*
 * weapon_target_near ($CF33, proposed): is there something for Cody's
 * weapon to hit? Slots $0F down to 4: a live one whose obj_id is not $69 or
 * $10-$13, with |dy| < $10 and |dx| < $60 from the player, returns A = $FF
 * (Z clear); none returns A = 0 (Z set). Called by player_attack_start
 * (0E:CB70) only; never executed in the CDL runs.
 */
void func_CF33(void) {
    REGS_IN;
    int i;
    uint16_t pc;

    mff_cycles(0xCF33, 2);
    y = 0x0F;
    for (;;) {
        mff_cycles(0xCF35, 4 + 2);
        a = RAM(OBJ_STATE + y);
        if (!(a & 0x80)) {
            mff_cycles(0xCF38, 1);
            goto next;
        }
        mff_cycles(0xCF3A, 4);
        a = RAM(OBJ_ID + y);
        for (i = 0, pc = 0xCF3D; i < (int)sizeof k_weapon_ignores; i++, pc += 4) {
            mff_cycles(pc, 2 + 2);              /* CMP #id, BEQ next */
            if (a == k_weapon_ignores[i]) {
                mff_cycles(pc + 2, 1);
                goto next;
            }
        }
        /* |obj_y - obj_y[Y]| */
        mff_cycles(0xCF51, 4 + 2 + 4 + 2);
        a = RAM(OBJ_Y);
        c = 1;
        SBC(RAM(OBJ_Y + y));
        if (!c) {
            mff_cycles(0xCF5A, 2 + 2 + 2);
            a ^= 0xFF;
            ADC(1);
        } else {
            mff_cycles(0xCF58, 1);
        }
        mff_cycles(0xCF5F, 2 + 2);
        if (a >= 0x10) {
            mff_cycles(0xCF61, 1);
            goto next;
        }
        /* |obj_x - obj_x[Y]|, low byte of the 16-bit difference */
        mff_cycles(0xCF63, 4 + 2 + 4 + 3 + 4 + 4 + 4 + 2);
        a = RAM(OBJ_X);
        c = 1;
        SBC(RAM(OBJ_X + y));
        mff_push(a);
        a = RAM(OBJ_X_HI);
        SBC(RAM(OBJ_X_HI + y));
        a = mff_pull();
        if (!c) {
            mff_cycles(0xCF74, 2 + 2 + 2);
            a ^= 0xFF;
            ADC(1);
        } else {
            mff_cycles(0xCF72, 1);
        }
        mff_cycles(0xCF79, 2 + 2);
        c = a >= 0x60;
        if (!c) {
            mff_cycles(0xCF7D, 2 + 6);
            a = 0xFF;
            SET_NZ(a);
            RTS();
        }
        mff_cycles(0xCF7B, 1);
next:
        mff_cycles(0xCF80, 2 + 2 + 2);
        y--;
        c = y >= 3;
        if (y == 3) break;
        mff_cycles(0xCF83, 1);
    }
    mff_cycles(0xCF85, 2 + 6);
    a = 0;
    SET_NZ(a);
    RTS();
}
