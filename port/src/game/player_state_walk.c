/*
 * player_state_walk.c - player_state_idle without a button: walking,
 * grabbing an enemy by walking into it, standing (0E:CCB5-CED0)
 *
 * player_idle_buttons (player_state_idle.c) goes on here when no button
 * was pressed:
 *
 *   $CCB5  player_walk: the walking animation unless it already runs
 *   $CCDA  player_walk_move: Left/Right turn and step the player
 *   $CCFA  player_walk_vertical: Down/Up step it
 *   $CD3B  with Left or Right held (alone or on a diagonal) an enemy in
 *          contact is looked for ($CD57); otherwise $CEA4
 *   $CD57  player_grab_search: slots $0E down to 3
 *   $CDFB  player_grab_contact: the last tests on one enemy (Haggar's way
 *          in from the search, by JMP)
 *   $CEA4  player_stand_check: no direction held, the standing pose
 *   $CEAA  player_stand, also called by most other states
 *
 * As in player_state_idle.c, a JMP the generator made a tail call costs 2
 * more cycles and the comments say so; branches, running on and the JMPs
 * it kept inline cost nothing more.
 */
#include "player_defs.h"

void func_CCDA(void);
void func_CCFA(void);
void func_CD3B(void);
void func_CD57(void);
void func_CE11(void);
void func_CEA4(void);
void func_CEAA(void);
void func_F000(void);   /* obj_walk_right (proposed): obj_face_right, then $F003 */
void func_F051(void);   /* obj_walk_left (proposed): obj_face_left, then $F054 */

/* ---- per-character tables (3 bytes: Cody, Guy, Haggar) ---- */
#define ANIM_WALK           0xE1E6   /* 04 05 06 */
#define IS_WALK             0xE273   /* 04 05 06 */
#define ANIM_WEAPON_WALK    0xE204   /* 53 05 54 */
#define IS_WEAPON_WALK      0xE29A   /* 53 05 54 */
#define WALK_UP_VY_FRAC     0xE1DD   /* B3 E6 80: going up, Cody 0.70, Guy 0.90, Haggar 1.50 */
#define WALK_UP_VY          0xE1E0   /* 00 00 01 */
#define GRAB_ANIM           0xE213   /* 31 32 33: holding an enemy */
#define GRAB_CHR            0xE25B   /* 0C 14 08 */
#define CHR_STAND           0xE22B   /* 00 04 08 */
#define ANIM_WEAPON_STAND   0xE21F   /* 42 02 44 */
#define CHR_WEAPON_STAND    0xE267   /* 00 04 08 */
/* the grabbed enemy's place, 15 px in front: + $000F facing right (Y = 0),
 * - $000F facing left (Y = 2) */
#define GRAB_OFFSET_LO      0xE1D3   /* 0F 00 F1 */
#define GRAB_OFFSET_HI      0xE1D4   /* 00 F1 FF */

/* frames an enemy stays held (grab_timer and its obj_held) */
#define GRAB_FRAMES         0x3C
#define GRAB_FRAMES_HAGGAR  0x5A

/*
 * player_walk ($CCB5, proposed): the walking animation ($E1E6, with a
 * weapon $E204) unless obj_anim already is it ($E273, $E29A); then
 * player_walk_move (JMP at 0E:CCC9 is a tail call, the other ways in run
 * on). Reached by JMP from player_idle_buttons (0E:CAEF, tail call).
 */
void func_CCB5(void) {
    REGS_IN;

    mff_cycles(0xCCB5, 3 + 3 + 2);
    y = RAM(PLAYER_ID);
    a = RAM(WEAPON_HELD);
    if (a == 0) {
        mff_cycles(0xCCBB, 4 + 4);
        a = RAM(OBJ_ANIM);
        COMPARE(a, prg_read((uint16_t)(IS_WALK + y)));
        mff_cycles(0xCCC1, 2);
        if (z) {
            mff_cycles(0xCCC1, 1);
            FALL_INTO(func_CCDA);
        }
        mff_cycles(0xCCC3, 4);
        a = prg_read((uint16_t)(ANIM_WALK + y));
        SET_NZ(a);
        mff_cycles(0xCCC6, 6);
        JSR_FN(0xCCC6, func_F294);              /* player_set_anim */
        mff_cycles(0xCCC9, 3);
        JMP_TAIL(0xCCDA, -1);                   /* player_walk_move */
    }
    mff_cycles(0xCCB9, 1 + 4 + 4);
    a = RAM(OBJ_ANIM);
    COMPARE(a, prg_read((uint16_t)(IS_WEAPON_WALK + y)));
    mff_cycles(0xCCD2, 2);
    if (!z) {
        mff_cycles(0xCCD4, 4);
        a = prg_read((uint16_t)(ANIM_WEAPON_WALK + y));
        SET_NZ(a);
        mff_cycles(0xCCD7, 6);
        JSR_FN(0xCCD7, func_F294);              /* player_set_anim */
    } else {
        mff_cycles(0xCCD2, 1);
    }
    FALL_INTO(func_CCDA);
}

/*
 * player_walk_move ($CCDA, proposed): player_ground_y = obj_y; with Left or
 * Right held, player_dir takes it and slot 0 (X = 0) walks that way:
 * obj_walk_right ($F000, probe point Y = 0) or obj_walk_left ($F051,
 * Y = 1), which turn the sprite and step by obj_vx unless the background
 * blocks. Then player_walk_vertical (the JMP after the right step is a
 * tail call).
 * Reached from player_walk and by JMP from player_idle_buttons (0E:CAF2,
 * tail call; never executed).
 */
void func_CCDA(void) {
    REGS_IN;

    mff_cycles(0xCCDA, 4 + 3 + 2 + 3 + 2 + 2);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    x = 0;
    a = RAM(JOY_HELD) & (PAD_LEFT | PAD_RIGHT);
    if (a == 0) {
        mff_cycles(0xCCE5, 1);
        FALL_INTO(func_CCFA);
    }
    mff_cycles(0xCCE7, 3 + 2 + 2);
    RAM(PLAYER_DIR) = a;
    a &= PAD_RIGHT;
    if (a != 0) {
        mff_cycles(0xCCED, 2 + 6);
        y = 0;
        SET_NZ(y);
        JSR_FN(0xCCEF, func_F000);              /* obj_walk_right */
        mff_cycles(0xCCF2, 3);
        JMP_TAIL(0xCCFA, -1);                   /* player_walk_vertical */
    }
    mff_cycles(0xCCEB, 1 + 2 + 6);
    y = 1;
    SET_NZ(y);
    JSR_FN(0xCCF7, func_F051);                  /* obj_walk_left */
    FALL_INTO(func_CCFA);
}

/*
 * player_walk_vertical ($CCFA, proposed): Down steps slot 0 down
 * ($F0A2, probe point Y = 0, Haggar 2) at the walking speed; otherwise Up
 * sets vy from $E1DD/$E1E0 and steps up ($F0F7, Y = 1, Haggar 3). Then
 * $CD3B (the JMP after the down step is a tail call).
 * Reached from player_walk_move (0E:CCF2 by JMP, tail call; or running on).
 */
void func_CCFA(void) {
    REGS_IN;

    mff_cycles(0xCCFA, 3 + 2 + 2);
    a = RAM(JOY_HELD) & (PAD_UP | PAD_DOWN);
    if (a == 0) {
        mff_cycles(0xCCFE, 1);
        FALL_INTO(func_CD3B);
    }
    mff_cycles(0xCD00, 2 + 2);
    a &= PAD_DOWN;
    if (a != 0) {
        mff_cycles(0xCD04, 3 + 2 + 2);
        a = RAM(PLAYER_ID);
        COMPARE(a, PLAYER_CODY);
        if (!z) {
            mff_cycles(0xCD0A, 2 + 2);
            COMPARE(a, PLAYER_GUY);
        }
        if (z) {
            mff_cycles(0xCD08, 1 + 2);          /* BEQ taken, LDY #0 */
            y = 0;
        } else {
            mff_cycles(0xCD0E, 2 + 2 + 1);      /* LDY #2, BNE (always) */
            y = 2;
        }
        SET_NZ(y);
        mff_cycles(0xCD14, 6);
        JSR_FN(0xCD14, func_F0A2);              /* step down */
        mff_cycles(0xCD17, 3);
        JMP_TAIL(0xCD3B, -1);
    }
    mff_cycles(0xCD02, 1 + 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(WALK_UP_VY_FRAC + y));
    mff_cycles(0xCD1F, 4 + 4);
    RAM(OBJ_VY_FRAC) = a;
    a = prg_read((uint16_t)(WALK_UP_VY + y));
    mff_cycles(0xCD25, 4 + 3 + 2 + 2);
    RAM(OBJ_VY) = a;
    a = RAM(PLAYER_ID);
    COMPARE(a, PLAYER_CODY);
    if (!z) {
        mff_cycles(0xCD2E, 2 + 2);
        COMPARE(a, PLAYER_GUY);
    }
    if (z) {
        mff_cycles(0xCD2C, 1 + 2);              /* BEQ taken, LDY #1 */
        y = 1;
    } else {
        mff_cycles(0xCD32, 2 + 2 + 1);          /* LDY #3, BNE (always) */
        y = 3;
    }
    SET_NZ(y);
    mff_cycles(0xCD38, 6);
    JSR_FN(0xCD38, func_F0F7);                  /* step up */
    FALL_INTO(func_CD3B);
}

/*
 * $CD3B: after the steps. With joy_held's direction bits a diagonal, or
 * Left/Right alone, player_grab_search ($CD57) from slot $0F; with none or
 * Up/Down alone, JMP player_stand_check ($CEA4, tail call).
 * Reached from player_walk_vertical (0E:CD17 by JMP, tail call; or
 * running on).
 */
void func_CD3B(void) {
    REGS_IN;
    static const uint8_t k_diagonals[] = { 0x05, 0x06, 0x09, 0x0A };
    int i;

    mff_cycles(0xCD3B, 3 + 2 + 2);
    a = RAM(JOY_HELD) & 0x0F;
    SET_NZ(a);
    if (z) {
        mff_cycles(0xCD3F, 1);
        goto stand;
    }
    for (i = 0; i < 4; i++) {
        mff_cycles((uint16_t)(0xCD41 + 4 * i), 2 + 2);   /* CMP #diagonal, BEQ $CD55 */
        COMPARE(a, k_diagonals[i]);
        if (z) {
            mff_cycles((uint16_t)(0xCD43 + 4 * i), 1);
            goto search;
        }
    }
    mff_cycles(0xCD51, 2 + 2);
    a &= PAD_UP | PAD_DOWN;
    SET_NZ(a);
    if (!z) {
        mff_cycles(0xCD53, 1);
        goto stand;
    }
search:
    mff_cycles(0xCD55, 2);
    y = 0x0F;
    FALL_INTO(func_CD57);                       /* player_grab_search */
stand:
    mff_cycles(0xCD5C, 3);
    JMP_TAIL(0xCEA4, -1);                       /* player_stand_check */
}

/* object ids and animations player_grab_search passes over */
static const uint8_t k_grab_never_ids[] = {
    0x6F, 0x8E, 0x9D, 0x2E, 0x61, 0x69, 0x6A, 0x6C, 0x6D, 0x66, 0x91, 0x72
};
static const uint8_t k_grab_never_anims[] = { 0xA6, 0xAC, 0x84, 0xE7, 0xBE };

/* $CD69-$CDBA, slot y: obj_id under $1E (not an enemy) or in
 * k_grab_never_ids, obj_invuln set, or obj_anim in k_grab_never_anims.
 * Returns 1 for those (the taken branch to $CD57 charged); *pa is A as the
 * 6502 leaves it. */
static int grab_kind_rejected(uint8_t y, uint8_t *pa) {
    int i;
    uint16_t pc;
    uint8_t a;

    mff_cycles(0xCD69, 4 + 2 + 2);
    a = RAM(OBJ_ID + y);
    *pa = a;
    if (a < 0x1E) {
        mff_cycles(0xCD6E, 1);
        return 1;
    }
    for (i = 0, pc = 0xCD70; i < (int)sizeof k_grab_never_ids; i++, pc += 4) {
        mff_cycles(pc, 2 + 2);                  /* CMP #id, BEQ $CD57 */
        if (a == k_grab_never_ids[i]) {
            mff_cycles((uint16_t)(pc + 2), 1);
            return 1;
        }
    }
    mff_cycles(0xCDA0, 4 + 2);
    a = RAM(OBJ_INVULN + y);
    *pa = a;
    if (a != 0) {
        mff_cycles(0xCDA3, 1);
        return 1;
    }
    mff_cycles(0xCDA5, 4);
    a = RAM(OBJ_ANIM + y);
    *pa = a;
    for (i = 0, pc = 0xCDA8; i < (int)sizeof k_grab_never_anims; i++, pc += 4) {
        mff_cycles(pc, 2 + 2);                  /* CMP #anim, BEQ $CD57 */
        if (a == k_grab_never_anims[i]) {
            mff_cycles((uint16_t)(pc + 2), 1);
            return 1;
        }
    }
    return 0;
}

/*
 * $CDFB-$CE40, the enemy in slot Y: |dy| 0 or under 8 (it always is: the
 * search tested |dy| < 8 already), and the screen edges: when the camera
 * holds the screen (camera_lock), Haggar cannot grab with obj_x under $28
 * facing left or $E0 and over facing right; otherwise nobody grabs within
 * $24 px of the screen's left edge.
 * 1 = grab (go on at $CE42), 0 = not this one (the code reached $CE11).
 * Registers in and out through g_cpu.
 */
static int grab_contact_test(void) {
    REGS_IN;

    mff_cycles(0xCDFB, 4 + 2 + 4 + 2);
    a = RAM(OBJ_Y);
    c = 1;
    SBC(RAM(OBJ_Y + y));
    if (!c) {
        mff_cycles(0xCE04, 2 + 2 + 2);
        a ^= 0xFF;
        ADC(1);
        c = 0;
    } else {
        mff_cycles(0xCE02, 1);
    }
    mff_cycles(0xCE09, 2 + 2);
    COMPARE(a, 0);
    if (z) {
        mff_cycles(0xCE0B, 1);
    } else {
        mff_cycles(0xCE0D, 2 + 2);
        COMPARE(a, 0x08);
        if (c) {                                /* runs on into $CE11 */
            SYNC_OUT;
            return 0;
        }
        mff_cycles(0xCE0F, 1);
    }

    mff_cycles(0xCE14, 3 + 2);
    a = RAM(CAMERA_LOCK);
    if (a == 0) {
        mff_cycles(0xCE16, 1 + 4 + 2 + 3 + 2 + 2);
        a = RAM(OBJ_X);
        c = 1;
        SBC(RAM(SCROLL_X));
        COMPARE(a, 0x24);
        if (!c) {
            mff_cycles(0xCE40, 1);
            SYNC_OUT;
            return 0;
        }
    } else {
        mff_cycles(0xCE18, 3 + 2 + 2);
        a = RAM(PLAYER_ID);
        COMPARE(a, PLAYER_HAGGAR);
        if (!z) {
            mff_cycles(0xCE1C, 1);
        } else {
            mff_cycles(0xCE1E, 3 + 2 + 2);
            a = RAM(PLAYER_DIR);
            COMPARE(a, PAD_LEFT);
            if (z) {
                mff_cycles(0xCE24, 4 + 2 + 2);
                a = RAM(OBJ_X);
                COMPARE(a, 0x28);
                if (!c) {
                    mff_cycles(0xCE2B, 3);      /* JMP $CE11 */
                    SYNC_OUT;
                    return 0;
                }
                mff_cycles(0xCE29, 1);
            } else {
                mff_cycles(0xCE22, 1 + 4 + 2 + 2);
                a = RAM(OBJ_X);
                COMPARE(a, 0xE0);
                if (c) {
                    mff_cycles(0xCE35, 3);      /* JMP $CE11 */
                    SYNC_OUT;
                    return 0;
                }
                mff_cycles(0xCE33, 1);
            }
        }
    }
    SYNC_OUT;
    return 1;
}

/*
 * $CE42: the enemy in slot Y is grabbed. It takes the player's obj_y and
 * goes 15 px in front ($E1D3/$E1D4 by player_dir), grab_slot = Y. With
 * Left or Right held (always, from the search) game_action 4: grab_timer
 * and obj_held = $3C frames (Haggar $5A, who also lifts the enemy 6 px),
 * obj_attack = 0 (its attack is cancelled), the holding animation and CHR
 * bank. Otherwise player_stand_check ($CEA4).
 */
static void grab_take(void) {
    REGS_IN;

    mff_cycles(0xCE42, 4 + 5 + 3 + 3 + 2 + 2 + 4 + 2 + 4);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + y) = a;
    RAM(GRAB_SLOT) = y;
    a = RAM(PLAYER_DIR) & PAD_LEFT;
    y = a;
    a = RAM(OBJ_X);
    c = 0;
    ADC(prg_read((uint16_t)(GRAB_OFFSET_LO + y)));
    mff_cycles(0xCE56, 3 + 4 + 4);
    mff_push(a);
    a = RAM(OBJ_X_HI);
    ADC(prg_read((uint16_t)(GRAB_OFFSET_HI + y)));
    mff_cycles(0xCE5D, 3 + 5 + 4 + 5 + 3 + 2 + 2);
    y = RAM(GRAB_SLOT);
    RAM(OBJ_X_HI + y) = a;
    a = mff_pull();
    RAM(OBJ_X + y) = a;
    a = RAM(JOY_HELD) & (PAD_LEFT | PAD_RIGHT);
    if (a == 0) {
        mff_cycles(0xCE6A, 1);
        FALL_INTO(func_CEA4);                   /* player_stand_check */
    }
    mff_cycles(0xCE6C, 2 + 3 + 3 + 2 + 2);
    a = ACT_GRAB;
    RAM(GAME_ACTION) = a;
    y = RAM(PLAYER_ID);
    COMPARE(y, PLAYER_HAGGAR);
    if (z) {
        mff_cycles(0xCE76, 3 + 4 + 2 + 2 + 5 + 2 + 2 + 1);
        y = RAM(GRAB_SLOT);
        a = RAM(OBJ_Y + y);
        c = 1;
        SBC(6);
        RAM(OBJ_Y + y) = a;
        a = GRAB_FRAMES_HAGGAR;
    } else {
        mff_cycles(0xCE74, 1 + 2 + 2 + 1);
        a = GRAB_FRAMES;
    }
    mff_cycles(0xCE89, 3 + 3 + 5 + 2 + 5 + 3 + 4);
    RAM(GRAB_TIMER) = a;
    y = RAM(GRAB_SLOT);
    RAM(OBJ_HELD + y) = a;
    a = 0;
    RAM(OBJ_ATTACK + y) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(GRAB_ANIM + y));
    SET_NZ(a);
    mff_cycles(0xCE9A, 6);
    JSR_FN(0xCE9A, func_F294);                  /* player_set_anim */
    mff_cycles(0xCE9D, 4);
    a = prg_read((uint16_t)(GRAB_CHR + y));
    SET_NZ(a);
    mff_cycles(0xCEA0, 6);
    JSR_FN(0xCEA0, func_F440);                  /* player_set_chr */
    mff_cycles(0xCEA3, 6);
    RTS();
}

/*
 * player_grab_search ($CD57, proposed): walking into an enemy grabs it.
 * Y counts down from the slot before the one given (the caller sets $0F)
 * to 3; slot 2 or below ends the search: player_stand_check ($CEA4, tail
 * call). A slot is a candidate when it is live (obj_state bit 7), on
 * screen (obj_flags bit 7) and not passed over (grab_kind_rejected), at
 * |dy| < 8 and |dx| < 7 (Haggar < 8) from the player. Haggar's candidates
 * go to player_grab_contact by JMP (tail call); the others run the same
 * tests here (grab_contact_test). A grab ends the frame (grab_take);
 * a candidate that fails goes on with the next slot through $CE11's JMP,
 * inline here.
 * In: Y = the slot after the first one tested. Reached from $CD3B (running
 * on), $CE11 and player_grab_contact (JMP, tail call).
 */
void func_CD57(void) {
    REGS_IN;

    for (;;) {
        mff_cycles(0xCD57, 2 + 2 + 2);
        y--;
        COMPARE(y, 2);
        if (z) {
            mff_cycles(0xCD5C, 3);
            JMP_TAIL(0xCEA4, -1);               /* player_stand_check */
        }
        mff_cycles(0xCD5A, 1 + 4 + 2);
        a = RAM(OBJ_STATE + y);
        if (!(a & 0x80)) {
            mff_cycles(0xCD62, 1);
            continue;
        }
        mff_cycles(0xCD64, 4 + 2);
        a = RAM(OBJ_FLAGS + y);
        if (!(a & 0x80)) {
            mff_cycles(0xCD67, 1);
            continue;
        }
        if (grab_kind_rejected(y, &a))
            continue;

        /* |obj_y - obj_y[Y]| < 8 */
        mff_cycles(0xCDBC, 4 + 2 + 4 + 2);
        a = RAM(OBJ_Y);
        c = 1;
        SBC(RAM(OBJ_Y + y));
        if (!c) {
            mff_cycles(0xCDC5, 2 + 2 + 2);
            a ^= 0xFF;
            ADC(1);
            c = 0;
        } else {
            mff_cycles(0xCDC3, 1);
        }
        mff_cycles(0xCDCA, 2 + 2);
        COMPARE(a, 0x08);
        if (c) {
            mff_cycles(0xCDCC, 2);              /* BCS $CE11 crosses a page */
            goto next;
        }

        /* $00 = |obj_x - obj_x[Y]|, the low byte of the 16-bit difference */
        mff_cycles(0xCDCE, 4 + 2 + 4 + 3 + 4 + 4 + 4 + 2);
        a = RAM(OBJ_X);
        c = 1;
        SBC(RAM(OBJ_X + y));
        mff_push(a);
        a = RAM(OBJ_X_HI);
        SBC(RAM(OBJ_X_HI + y));
        a = mff_pull();
        if (!c) {
            mff_cycles(0xCDDF, 2 + 2 + 2);
            a ^= 0xFF;
            ADC(1);
            c = 0;
        } else {
            mff_cycles(0xCDDD, 1);
        }
        mff_cycles(0xCDE4, 3 + 3 + 2 + 2);
        RAM(TMP0) = a;
        a = RAM(PLAYER_ID);
        COMPARE(a, PLAYER_HAGGAR);
        if (z) {
            mff_cycles(0xCDEC, 3 + 2 + 2);
            a = RAM(TMP0);
            COMPARE(a, 0x08);
            if (c) {
                mff_cycles(0xCDF0, 2);          /* BCS $CE11 crosses a page */
                goto next;
            }
            mff_cycles(0xCDF2, 3);
            JMP_TAIL(0xCDFB, -1);               /* player_grab_contact */
        }
        mff_cycles(0xCDEA, 1 + 3 + 2 + 2);
        a = RAM(TMP0);
        COMPARE(a, 0x07);
        if (c) {
            mff_cycles(0xCDF9, 2);              /* BCS $CE11 crosses a page */
            goto next;
        }
        SYNC_OUT;
        if (grab_contact_test()) {
            grab_take();
            return;
        }
        SYNC_IN;
next:
        mff_cycles(0xCE11, 3);                  /* JMP $CD57 */
    }
}

/*
 * player_grab_contact ($CDFB, proposed): grab_contact_test on slot Y, then
 * the grab, or the search goes on through $CE11 (whose JMP is a tail call
 * here). Reached by JMP from player_grab_search for Haggar (0E:CDF2, tail
 * call).
 */
void func_CDFB(void) {
    if (grab_contact_test()) {
        grab_take();
        return;
    }
    func_CE11();
}

/* $CE11: JMP player_grab_search ($CD57) for the next slot (tail call) */
void func_CE11(void) {
    REGS_IN;

    mff_cycles(0xCE11, 3);
    JMP_TAIL(0xCD57, -1);
}

/*
 * player_stand_check ($CEA4, proposed): with a direction still held the
 * frame is over (RTS); with none, player_stand.
 * Reached by JMP from $CD3B and player_grab_search (0E:CD5C, tail call)
 * and from grab_take (BEQ at 0E:CE6A).
 */
void func_CEA4(void) {
    REGS_IN;

    mff_cycles(0xCEA4, 3 + 2 + 2);
    a = RAM(JOY_HELD) & 0x0F;
    SET_NZ(a);
    if (!z) {
        mff_cycles(0xCEA8, 1 + 6);
        RTS();
    }
    FALL_INTO(func_CEAA);                       /* player_stand */
}

/*
 * player_stand ($CEAA, proposed): player_ground_y = obj_y and the standing
 * animation and CHR bank ($E1E3/$E22B; with a weapon $E21F/$E267).
 * Called by most player states when they end (0E:D03C, D2DF, D45A, D61B,
 * D84A, DA31, DD04, DDB7), by bank $00 (00:80C1 ... 00:985B), 0C:8724 and
 * 0F:E38D, E3F8; run on into from player_stand_check.
 * Out: A = the CHR bank, Y = player_id.
 */
void func_CEAA(void) {
    REGS_IN;

    mff_cycles(0xCEAA, 4 + 3 + 3 + 2);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(WEAPON_HELD);
    if (a == 0) {
        mff_cycles(0xCEB3, 3 + 4);
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(ANIM_STAND + y));
        SET_NZ(a);
        mff_cycles(0xCEB8, 6);
        JSR_FN(0xCEB8, func_F294);              /* player_set_anim */
        mff_cycles(0xCEBB, 4);
        a = prg_read((uint16_t)(CHR_STAND + y));
        SET_NZ(a);
        mff_cycles(0xCEBE, 6);
        JSR_FN(0xCEBE, func_F440);              /* player_set_chr */
        mff_cycles(0xCEC1, 6);
        RTS();
    }
    mff_cycles(0xCEB1, 1 + 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(ANIM_WEAPON_STAND + y));
    SET_NZ(a);
    mff_cycles(0xCEC7, 6);
    JSR_FN(0xCEC7, func_F294);                  /* player_set_anim */
    mff_cycles(0xCECA, 4);
    a = prg_read((uint16_t)(CHR_WEAPON_STAND + y));
    SET_NZ(a);
    mff_cycles(0xCECD, 6);
    JSR_FN(0xCECD, func_F440);                  /* player_set_chr */
    mff_cycles(0xCED0, 6);
    RTS();
}
