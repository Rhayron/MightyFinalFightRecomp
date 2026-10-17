/*
 * lifted_b5.c - bank 5 routines $8000-$BFFF, lifted by
 * port/tools/lift_bank.py from the pure scaffold: exact, still to be
 * tidied and moved into their systems' files.
 */
#include "mff.h"
#include "state.h"
#include "coroutine.h"

void func_E954(void);
void func_F090(void);
void func_F099(void);
void func_F0A2(void);
void func_F0F7(void);
void func_F183(void);
void func_F1A1(void);
void func_F1BF(void);
void func_F1DF(void);
void func_F1FD(void);
void func_F288(void);
void func_F294(void);
void func_F2A0(void);
void func_F2BE(void);
void func_F343(void);
void func_F34A(void);
void func_F34C(void);
void func_F374(void);
void func_F440(void);
void func_F449(void);
void func_F71A(void);
void func_FB3E(void);
void func_FB5B(void);

/*  $9C00 (bank 5) - lifted by port/tools/lift.py */
void func_9C00_b5(void) {
    REGS_IN;
    mff_cycles(0x9C00, 5);
    x = RAM(VRAM_BUFFER_LEN);
    y = 0x00;
L_9C04:;
    mff_cycles(0x9C04, 4);
    a = prg_read((uint16_t)(0x9C20 + y));
    mff_cycles(0x9C07, 4);
    m = 0xFF;
    c = a >= m;
    if (a == m) { mff_cycles(0x9C09, 1); goto L_9C12; }
    mff_cycles(0x9C0B, 11);
    RAM(VRAM_BUFFER + x) = a;
    y++;
    x++;
    if (x != 0) { mff_cycles(0x9C04, 1); goto L_9C04; }
L_9C12:;
    mff_cycles(0x9C12, 14);
    RAM(VRAM_BUFFER_LEN) = x;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SPECIAL_SCREEN_STEP) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x1C1A, func_E954, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9C1B, 11);
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(VRAM_REQUEST) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BD7F (bank 5) - lifted by port/tools/lift.py */
void func_BD7F_b5(void) {
    REGS_IN;
    mff_cycles(0xBD7F, 24);
    a = 0xF9;
    RAM(SPECIAL_SCREEN_ID) = a;
    a = 0x01;
    RAM(OBJ_INVULN + x) = a;
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    y = a;
    a = prg_read((uint16_t)(0xBF9F + y));
    mff_cycles(0xBD91, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0xBFA7 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xBD96, 8);
    RAM(TMP1) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $BD9B (bank 5) - lifted by port/tools/lift.py */
void func_BD9B_b5(void) {
    REGS_IN;
    mff_cycles(0xBD9B, 23);
    a = 0x80;
    RAM(JOY_PRESSED) = a;
    RAM(JOY_HELD) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(OBJ_SPAWN_GROUP + 15) = x;
    SYNC_OUT;
    if (!mff_call_at(0x1DAA, 0xC9B9, 5)) return;
    SYNC_IN;
    mff_cycles(0xBDAB, 19);
    x = RAM(OBJ_SPAWN_GROUP + 15);
    a = 0x28;
    RAM(GAME_ACTION) = a;
    a = 0xCD;
    y = 0x34;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1DB8, func_F449, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBDB9, 10);
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBFAF, 2); call_by_address_tail(0xBFAF, 5); return;
}

/*  $BDBF (bank 5) - lifted by port/tools/lift.py */
void func_BDBF_b5(void) {
    REGS_IN;
    mff_cycles(0xBDBF, 31);
    a = RAM(PLAYER_DIR);
    a ^= 0xFF;
    a &= 0x03;
    RAM(JUMP_DIR) = a;
    RAM(JOY_PRESSED) = a;
    RAM(JOY_HELD) = a;
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(OBJ_SPAWN_GROUP + 15) = x;
    SYNC_OUT;
    if (!mff_call_at(0x1DD4, 0xC9B9, 5)) return;
    SYNC_IN;
    mff_cycles(0xBDD5, 11);
    x = RAM(OBJ_SPAWN_GROUP + 15);
    a = RAM(GAME_ACTION);
    m = 0x02;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xBDDE, 7);
        t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_STATE + x) = t;
    } else {
        mff_cycles(0xBDDC, 1);
    }
    mff_cycles(0xBDE1, 11);
    a = 0x28;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BDE6 (bank 5) - lifted by port/tools/lift.py */
void func_BDE6_b5(void) {
    REGS_IN;
    mff_cycles(0xBDE6, 10);
    y = RAM(OBJ_TIMER2 + x);
    a = RAM(OBJ_STATE + y);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) {
        mff_cycles(0xBDEE, 13);
        a = 0xFF;
        RAM(SPECIAL_SCREEN_ID) = a;
        a = 0x0F;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x1DF6, func_F34A, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xBDF7, 44);
        a = 0x50;
        RAM(OBJ_AI_TIMER + x) = a;
        a = 0x86;
        RAM(OBJ_STATE + x) = a;
        a = 0x83;
        RAM(OBJ_TIMER + x) = a;
        a = 0x00;
        RAM(OBJ_DOWN_TIMER) = a;
        RAM(PLAYER_NO_HIT) = a;
        RAM(RESPAWN_TIMER1) = a;
        a = 0x01;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(RESPAWN_TIMER2) = a;
        SYNC_OUT;
        if (!mff_call_at(0x1E15, 0xCED1, 5)) return;
        SYNC_IN;
        mff_cycles(0xBE16, 10);
        a = RAM(PLAYER_ID);
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_GROUND_Y + x) = a;
        if (z) {
            mff_cycles(0xBE1D, 6);
            a = 0xD0;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_FLAGS) = a;
        } else {
            mff_cycles(0xBE1B, 1);
        }
    } else {
        mff_cycles(0xBDEC, 2);
    }
    mff_cycles(0xBE22, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BE23 (bank 5) - lifted by port/tools/lift.py */
void func_BE23_b5(void) {
    REGS_IN;
    mff_cycles(0xBE23, 15);
    a = RAM(OBJ_FLAGS);
    a &= 0xFB;
    RAM(OBJ_FLAGS) = a;
    a = RAM(PLAYER_ID);
    if (a == 0) { mff_cycles(0xBE2D, 1); goto L_BE36; }
    mff_cycles(0xBE2F, 4);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBE31, 1); goto L_BE8E; }
    mff_cycles(0xBE33, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBECE, 2); call_by_address_tail(0xBECE, 5); return;
L_BE36:;
    mff_cycles(0xBE36, 8);
    a = RAM(OBJ_DOWN_TIMER);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0xBE3B, 1); goto L_BE59; }
    mff_cycles(0xBE3D, 4);
    m = 0x02;
    c = a >= m;
    if (a == m) { mff_cycles(0xBE3F, 1); goto L_BE6F; }
    mff_cycles(0xBE41, 4);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBE43, 1); goto L_BE79; }
    mff_cycles(0xBE45, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1E47, func_F71A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBE48, 30);
    a = 0x80;
    RAM(OBJ_STATE + y) = a;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x9C;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ID + y) = a;
    RAM(OBJ_SAVED_ID + y) = a;
    SYNC_OUT;
    mff_rts(); return;
L_BE59:;
    mff_cycles(0xBE59, 8);
    a = RAM(OBJ_AI_TIMER + x);
    a &= 0x07;
    if (a == 0) {
        mff_cycles(0xBE60, 8);
        a = 0x13;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x1E64, func_F34C, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xBE5E, 1);
    }
    mff_cycles(0xBE65, 25);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) + 1);
    RAM(OBJ_AI_TIMER + x) = t;
    t = (uint8_t)(RAM(OBJ_X) + 1);
    RAM(OBJ_X) = t;
    t = (uint8_t)(RAM(OBJ_X) + 1);
    RAM(OBJ_X) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_BE6F:;
    mff_cycles(0xBE6F, 8);
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1E73, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBE74, 5);
    a = 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF440, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F440(); g_code_window_base = swb; } return;
L_BE79:;
    mff_cycles(0xBE79, 36);
    t = (uint8_t)(RAM(PLAYER_LEVEL) - 1);
    RAM(PLAYER_LEVEL) = t;
    t = (uint8_t)(RAM(PLAYER_LEVEL) - 1);
    RAM(PLAYER_LEVEL) = t;
    a = 0x02;
    RAM(PLAYER_ID) = a;
    a = 0x19;
    RAM(PALETTE_BUF + 23) = a;
    a = 0x30;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PALETTE_BUF + 22) = a;
    RAM(PALETTE_REQUEST) = a;
    SYNC_OUT;
    mff_rts(); return;
L_BE8E:;
    mff_cycles(0xBE8E, 23);
    a = 0x02;
    RAM(JOY_PRESSED) = a;
    RAM(JOY_HELD) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(OBJ_SPAWN_GROUP + 15) = x;
    SYNC_OUT;
    if (!mff_call_at(0x1E9D, 0xC9B9, 5)) return;
    SYNC_IN;
    mff_cycles(0xBE9E, 12);
    x = RAM(OBJ_SPAWN_GROUP + 15);
    a = RAM(OBJ_X);
    m = 0x1B;
    c = a >= m;
    if (!c) {
        mff_cycles(0xBEA8, 12);
        a = 0x94;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_FLAGS) = a;
        SYNC_OUT;
        if (!mff_call_at(0x1EAF, 0xBE79, 5)) return;
        SYNC_IN;
        mff_cycles(0xBEB0, 33);
        a = 0x9A;
        RAM(OBJ_Y) = a;
        a = 0x00;
        RAM(OBJ_X) = a;
        a = 0x83;
        RAM(OBJ_TIMER + x) = a;
        a = 0x86;
        RAM(OBJ_STATE + x) = a;
        a = 0x80;
        RAM(OBJ_AI_TIMER + x) = a;
    } else {
        mff_cycles(0xBEA6, 1);
    }
    mff_cycles(0xBEC9, 11);
    a = 0x28;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BE79 (bank 5) - lifted by port/tools/lift.py */
void func_BE79_b5(void) {
    REGS_IN;
    mff_cycles(0xBE79, 36);
    t = (uint8_t)(RAM(PLAYER_LEVEL) - 1);
    RAM(PLAYER_LEVEL) = t;
    t = (uint8_t)(RAM(PLAYER_LEVEL) - 1);
    RAM(PLAYER_LEVEL) = t;
    a = 0x02;
    RAM(PLAYER_ID) = a;
    a = 0x19;
    RAM(PALETTE_BUF + 23) = a;
    a = 0x30;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PALETTE_BUF + 22) = a;
    RAM(PALETTE_REQUEST) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BECE (bank 5) - lifted by port/tools/lift.py */
void func_BECE_b5(void) {
    REGS_IN;
    mff_cycles(0xBECE, 8);
    a = 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1ED2, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBED3, 8);
    a = RAM(OBJ_Y);
    m = 0x99;
    c = a >= m;
    if (c) { mff_cycles(0xBED8, 1); goto L_BEDE; }
    mff_cycles(0xBEDA, 4);
    a = 0x04;
    if (a != 0) { mff_cycles(0xBEDC, 1); goto L_BEF7; }
L_BEDE:;
    mff_cycles(0xBEDE, 4);
    m = 0x9B;
    c = a >= m;
    if (!c) { mff_cycles(0xBEE0, 1); goto L_BEE6; }
    mff_cycles(0xBEE2, 4);
    a = 0x08;
    if (a != 0) { mff_cycles(0xBEE4, 1); goto L_BEF7; }
L_BEE6:;
    mff_cycles(0xBEE6, 8);
    a = RAM(OBJ_X);
    m = 0x97;
    c = a >= m;
    if (c) { mff_cycles(0xBEEB, 1); goto L_BEF1; }
    mff_cycles(0xBEED, 4);
    a = 0x01;
    if (a != 0) { mff_cycles(0xBEEF, 1); goto L_BEF7; }
L_BEF1:;
    mff_cycles(0xBEF1, 4);
    m = 0x99;
    c = a >= m;
    if (!c) { mff_cycles(0xBEF3, 2); goto L_BF0D; }
    mff_cycles(0xBEF5, 2);
    a = 0x02;
L_BEF7:;
    mff_cycles(0xBEF7, 21);
    RAM(JOY_PRESSED) = a;
    RAM(JOY_HELD) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(OBJ_SPAWN_GROUP + 15) = x;
    SYNC_OUT;
    if (!mff_call_at(0x1F04, 0xC9B9, 5)) return;
    SYNC_IN;
    mff_cycles(0xBF05, 15);
    x = RAM(OBJ_SPAWN_GROUP + 15);
    a = 0x28;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
L_BF0D:;
    mff_cycles(0xBF0D, 45);
    a = 0xD0;
    RAM(OBJ_FLAGS) = a;
    a = 0x0A;
    RAM(GAME_ACTION) = a;
    a = 0x7B;
    RAM(OBJ_ID + x) = a;
    a = 0x50;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x84;
    RAM(OBJ_TIMER + x) = a;
    a = 0x86;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BF2B (bank 5) - lifted by port/tools/lift.py */
void func_BF2B_b5(void) {
    REGS_IN;
    mff_cycles(0xBF2B, 41);
    a = 0x90;
    RAM(OBJ_FLAGS + x) = a;
    a = 0x01;
    RAM(OBJ_VX + x) = a;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0xF8;
    RAM(OBJ_X + x) = a;
    a = 0x98;
    RAM(OBJ_Y + x) = a;
    a = 0x23;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1F46, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBF47, 33);
    a = 0x36;
    RAM(CHR_BANKS + 3) = a;
    a = 0x15;
    RAM(PALETTE_BUF + 27) = a;
    a = 0x26;
    RAM(PALETTE_BUF + 26) = a;
    RAM(PALETTE_REQUEST) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BF5B (bank 5) - lifted by port/tools/lift.py */
void func_BF5B_b5(void) {
    REGS_IN;
    mff_cycles(0xBF5B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1F5D, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBF5E, 8);
    a = RAM(OBJ_X + x);
    m = 0xB0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0xBF65, 8);
        a = 0x22;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x1F69, func_F288, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xBF6A, 14);
        a = 0xFF;
        RAM(OBJ_AI_TIMER + x) = a;
        a = 0x87;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_STATE + x) = a;
    } else {
        mff_cycles(0xBF63, 1);
    }
    mff_cycles(0xBF74, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BF75 (bank 5) - lifted by port/tools/lift.py */
void func_BF75_b5(void) {
    REGS_IN;
    mff_cycles(0xBF75, 9);
    a = RAM(OBJ_GROUND_Y + x);
    m = RAM(PLAYER_ID);
    c = a >= m;
    if (a == m) {
        mff_cycles(0xBF7C, 5);
        a = 0x00;
        RAM(SPECIAL_SCREEN_ID) = a;
    } else {
        mff_cycles(0xBF7A, 1);
    }
    mff_cycles(0xBF80, 14);
    a = 0x28;
    RAM(GAME_ACTION) = a;
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (z) {
        mff_cycles(0xBF89, 9);
        a = RAM(OBJ_TIMER + x);
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_STATE + x) = a;
    } else {
        mff_cycles(0xBF87, 1);
    }
    mff_cycles(0xBF8F, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BF90 (bank 5) - lifted by port/tools/lift.py */
void func_BF90_b5(void) {
    REGS_IN;
    mff_cycles(0xBF90, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (z) {
        mff_cycles(0xBF95, 12);
        a = 0x0E;
        RAM(GAME_ACTION) = a;
        a = RAM(OBJ_GROUND_Y + x);
        n = (a) >> 7;
        z = (a) == 0;
        RAM(PLAYER_ID) = a;
    } else {
        mff_cycles(0xBF93, 1);
    }
    mff_cycles(0xBF9E, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BFAF (bank 5) - lifted by port/tools/lift.py */
void func_BFAF_b5(void) {
    REGS_IN;
    mff_cycles(0xBFAF, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1FB1, func_F71A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBFB2, 2);
    if (!c) {
        mff_cycles(0xBFB4, 84);
        a = RAM(OBJ_X + x);
        RAM(OBJ_X + y) = a;
        a = RAM(OBJ_Y + x);
        RAM(OBJ_Y + y) = a;
        a = 0x02;
        RAM(OBJ_X_HI + y) = a;
        a = 0xFF;
        RAM(UNK_1B) = a;
        a = 0x9E;
        RAM(OBJ_ID + y) = a;
        RAM(OBJ_SAVED_ID + y) = a;
        a = 0x80;
        RAM(OBJ_STATE + y) = a;
        a = 0xFF;
        RAM(OBJ_INVULN + y) = a;
        a = 0xE0;
        RAM(OBJ_AI_TIMER + y) = a;
        a = x;
        RAM(OBJ_TIMER + y) = a;
        a = 0x94;
        RAM(OBJ_FLAGS + y) = a;
        a = y;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_TIMER2 + x) = a;
    } else {
        mff_cycles(0xBFB2, 1);
    }
    mff_cycles(0xBFED, 6);
    SYNC_OUT;
    mff_rts(); return;
}
