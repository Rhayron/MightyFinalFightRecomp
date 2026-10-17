/*
 * lifted_b2.c - bank 2 routines $8000-$BFFF, lifted by
 * port/tools/lift_bank.py from the pure scaffold: exact, still to be
 * tidied and moved into their systems' files.
 */
#include "mff.h"
#include "state.h"
#include "coroutine.h"

void func_E954(void);
void func_F03F(void);
void func_F090(void);
void func_F099(void);
void func_F183(void);
void func_F1A1(void);
void func_F1DF(void);
void func_F1FD(void);
void func_F288(void);
void func_F294(void);
void func_F2A0(void);
void func_F2BE(void);
void func_F314(void);
void func_F34A(void);
void func_F34C(void);
void func_F365(void);
void func_F374(void);
void func_F406(void);
void func_F436(void);
void func_F440(void);
void func_F443(void);
void func_F44F(void);
void func_F55A(void);
void func_F71A(void);
void func_FF2A(void);
void func_FF31(void);

/*  $9300 (bank 2) - lifted by port/tools/lift.py */
void func_9300_b2(void) {
    REGS_IN;
    mff_cycles(0x9300, 8);
    t = (uint8_t)(RAM(CONTINUE_COUNT) + 1);
    RAM(CONTINUE_COUNT) = t;
    y = 0x0F;
    for (;;) {
        mff_cycles(0x9305, 4);
        a = prg_read((uint16_t)(0x9575 + y));
        mff_cycles(0x9308, 14);
        RAM(PALETTE_BUF + y) = a;
        RAM(PALETTE_BASE + y) = a;
        a = prg_read((uint16_t)(0x9585 + y));
        mff_cycles(0x9311, 14);
        RAM(PALETTE_BUF + 16 + y) = a;
        RAM(PALETTE_BASE + 16 + y) = a;
        y--;
        if (y & 0x80) break;
        mff_cycles(0x9305, 1);
    }
    mff_cycles(0x931A, 26);
    RAM(PALETTE_REQUEST) = y;
    a = 0x78;
    RAM(CHR_BANKS) = a;
    a = 0x7A;
    RAM(CHR_BANKS + 1) = a;
    a = 0x23;
    RAM(CHR_BANKS + 4) = a;
    a = 0x0A;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x132C, func_F34A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x932D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x132F, 0x942A, 2)) return;
    SYNC_IN;
    mff_cycles(0x9330, 8);
    a = 0x72;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1334, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9335, 8);
    a = 0x3C;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1339, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x933A, 88);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    RAM(OBJ_STATE + 1) = a;
    a = 0x90;
    RAM(OBJ_FLAGS) = a;
    RAM(OBJ_FLAGS + 1) = a;
    a = 0x00;
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y_HI) = a;
    RAM(OBJ_X_HI + 1) = a;
    RAM(OBJ_Y_HI + 1) = a;
    RAM(OBJ_TIMER) = a;
    a = 0x3C;
    RAM(OBJ_TIMER2) = a;
    a = 0x01;
    RAM(OBJ_DIR) = a;
    a = 0x00;
    RAM(OBJ_VX_FRAC + 1) = a;
    a = 0x01;
    RAM(OBJ_VX + 1) = a;
    a = 0x68;
    RAM(OBJ_X) = a;
    a = 0x54;
    RAM(OBJ_Y) = a;
    x = 0x01;
    a = 0x67;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x137F, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9380, 19);
    a = 0x84;
    RAM(OBJ_X + 1) = a;
    a = 0xB0;
    RAM(OBJ_Y + 1) = a;
    a = RAM(JOY_PRESSED);
    m = 0x10;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x938E, 1); goto L_93CB; }
    mff_cycles(0x9390, 4);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0x9392, 1); goto L_93CE; }
    mff_cycles(0x9394, 4);
    m = 0x08;
    c = a >= m;
    if (a == m) { mff_cycles(0x9396, 1); goto L_93F0; }
L_9398:;
    mff_cycles(0x9398, 10);
    x = 0x01;
    a = RAM(OBJ_DIR);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0x93A1, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x13A3, func_F090, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x93A4, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x13A6, func_F183, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x93A7, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x93B0, 2); call_by_address_tail(0x93B0, 2); return;
    } else {
        mff_cycles(0x939F, 1);
    }
    mff_cycles(0x93AA, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x13AC, func_F099, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x93AD, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x13AF, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x93B0, 6);
    a = RAM(OBJ_TIMER2);
    if (a == 0) {
        mff_cycles(0x93B5, 16);
        a = 0x79;
        RAM(OBJ_TIMER2) = a;
        a = RAM(OBJ_DIR);
        a ^= 0x03;
        RAM(OBJ_DIR) = a;
    } else {
        mff_cycles(0x93B3, 1);
    }
    mff_cycles(0x93C2, 12);
    t = (uint8_t)(RAM(OBJ_TIMER2) - 1);
    RAM(OBJ_TIMER2) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x13C7, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x93C8, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x938A, 2); call_by_address_tail(0x938A, 2); return;
L_93CB:;
    mff_cycles(0x93CB, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9413, 2); call_by_address_tail(0x9413, 2); return;
L_93CE:;
    mff_cycles(0x93CE, 8);
    a = 0x2F;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x13D2, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x93D3, 6);
    a = RAM(OBJ_TIMER);
    if (a != 0) { mff_cycles(0x93D6, 1); goto L_93E4; }
    mff_cycles(0x93D8, 14);
    a = 0xFF;
    RAM(OBJ_TIMER) = a;
    a = 0x64;
    RAM(OBJ_Y) = a;
    if (a != 0) { mff_cycles(0x9398, 1); goto L_9398; }
L_93E4:;
    mff_cycles(0x93E4, 14);
    a = 0x00;
    RAM(OBJ_TIMER) = a;
    a = 0x54;
    RAM(OBJ_Y) = a;
    if (a != 0) { mff_cycles(0x9398, 1); goto L_9398; }
L_93F0:;
    mff_cycles(0x93F0, 8);
    a = 0x2F;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x13F4, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x93F5, 6);
    a = RAM(OBJ_TIMER);
    if (a != 0) { mff_cycles(0x93F8, 2); goto L_9406; }
    mff_cycles(0x93FA, 14);
    a = 0xFF;
    RAM(OBJ_TIMER) = a;
    a = 0x64;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y) = a;
    if (!z) { mff_cycles(0x9404, 1); goto L_9410; }
L_9406:;
    mff_cycles(0x9406, 12);
    a = 0x00;
    RAM(OBJ_TIMER) = a;
    a = 0x54;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y) = a;
L_9410:;
    mff_cycles(0x9410, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9398, 2); call_by_address_tail(0x9398, 2); return;
}

/*  $938A (bank 2) - lifted by port/tools/lift.py */
void func_938A_b2(void) {
    REGS_IN;
    mff_cycles(0x938A, 7);
    a = RAM(JOY_PRESSED);
    m = 0x10;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x938E, 1); goto L_93CB; }
    mff_cycles(0x9390, 4);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0x9392, 1); goto L_93CE; }
    mff_cycles(0x9394, 4);
    m = 0x08;
    c = a >= m;
    if (a == m) { mff_cycles(0x9396, 1); goto L_93F0; }
L_9398:;
    mff_cycles(0x9398, 10);
    x = 0x01;
    a = RAM(OBJ_DIR);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0x93A1, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x13A3, func_F090, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x93A4, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x13A6, func_F183, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x93A7, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x93B0, 2); call_by_address_tail(0x93B0, 2); return;
    } else {
        mff_cycles(0x939F, 1);
    }
    mff_cycles(0x93AA, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x13AC, func_F099, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x93AD, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x13AF, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x93B0, 6);
    a = RAM(OBJ_TIMER2);
    if (a == 0) {
        mff_cycles(0x93B5, 16);
        a = 0x79;
        RAM(OBJ_TIMER2) = a;
        a = RAM(OBJ_DIR);
        a ^= 0x03;
        RAM(OBJ_DIR) = a;
    } else {
        mff_cycles(0x93B3, 1);
    }
    mff_cycles(0x93C2, 12);
    t = (uint8_t)(RAM(OBJ_TIMER2) - 1);
    RAM(OBJ_TIMER2) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x13C7, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x93C8, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x938A, 2); call_by_address_tail(0x938A, 2); return;
L_93CB:;
    mff_cycles(0x93CB, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9413, 2); call_by_address_tail(0x9413, 2); return;
L_93CE:;
    mff_cycles(0x93CE, 8);
    a = 0x2F;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x13D2, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x93D3, 6);
    a = RAM(OBJ_TIMER);
    if (a != 0) { mff_cycles(0x93D6, 1); goto L_93E4; }
    mff_cycles(0x93D8, 14);
    a = 0xFF;
    RAM(OBJ_TIMER) = a;
    a = 0x64;
    RAM(OBJ_Y) = a;
    if (a != 0) { mff_cycles(0x9398, 1); goto L_9398; }
L_93E4:;
    mff_cycles(0x93E4, 14);
    a = 0x00;
    RAM(OBJ_TIMER) = a;
    a = 0x54;
    RAM(OBJ_Y) = a;
    if (a != 0) { mff_cycles(0x9398, 1); goto L_9398; }
L_93F0:;
    mff_cycles(0x93F0, 8);
    a = 0x2F;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x13F4, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x93F5, 6);
    a = RAM(OBJ_TIMER);
    if (a != 0) { mff_cycles(0x93F8, 2); goto L_9406; }
    mff_cycles(0x93FA, 14);
    a = 0xFF;
    RAM(OBJ_TIMER) = a;
    a = 0x64;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y) = a;
    if (!z) { mff_cycles(0x9404, 1); goto L_9410; }
L_9406:;
    mff_cycles(0x9406, 12);
    a = 0x00;
    RAM(OBJ_TIMER) = a;
    a = 0x54;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y) = a;
L_9410:;
    mff_cycles(0x9410, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9398, 2); call_by_address_tail(0x9398, 2); return;
}

/*  $9398 (bank 2) - lifted by port/tools/lift.py */
void func_9398_b2(void) {
    REGS_IN;
    mff_cycles(0x9398, 10);
    x = 0x01;
    a = RAM(OBJ_DIR);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0x93A1, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x13A3, func_F090, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x93A4, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x13A6, func_F183, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x93A7, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x93B0, 2); call_by_address_tail(0x93B0, 2); return;
    } else {
        mff_cycles(0x939F, 1);
    }
    mff_cycles(0x93AA, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x13AC, func_F099, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x93AD, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x13AF, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x93B0, 6);
    a = RAM(OBJ_TIMER2);
    if (a == 0) {
        mff_cycles(0x93B5, 16);
        a = 0x79;
        RAM(OBJ_TIMER2) = a;
        a = RAM(OBJ_DIR);
        a ^= 0x03;
        RAM(OBJ_DIR) = a;
    } else {
        mff_cycles(0x93B3, 1);
    }
    mff_cycles(0x93C2, 12);
    t = (uint8_t)(RAM(OBJ_TIMER2) - 1);
    RAM(OBJ_TIMER2) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x13C7, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x93C8, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x938A, 2); call_by_address_tail(0x938A, 2); return;
}

/*  $93B0 (bank 2) - lifted by port/tools/lift.py */
void func_93B0_b2(void) {
    REGS_IN;
    mff_cycles(0x93B0, 6);
    a = RAM(OBJ_TIMER2);
    if (a == 0) {
        mff_cycles(0x93B5, 16);
        a = 0x79;
        RAM(OBJ_TIMER2) = a;
        a = RAM(OBJ_DIR);
        a ^= 0x03;
        RAM(OBJ_DIR) = a;
    } else {
        mff_cycles(0x93B3, 1);
    }
    mff_cycles(0x93C2, 12);
    t = (uint8_t)(RAM(OBJ_TIMER2) - 1);
    RAM(OBJ_TIMER2) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x13C7, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x93C8, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x938A, 2); call_by_address_tail(0x938A, 2); return;
}

/*  $9413 (bank 2) - lifted by port/tools/lift.py */
void func_9413_b2(void) {
    REGS_IN;
    mff_cycles(0x9413, 8);
    a = RAM(OBJ_Y);
    m = 0x64;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x941A, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9418, 1);
    }
    mff_cycles(0x941B, 11);
    t = (uint8_t)(RAM(CREDITS) - 1);
    RAM(CREDITS) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x141F, 0x942A, 2)) return;
    SYNC_IN;
    mff_cycles(0x9420, 8);
    a = 0x30;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1424, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9425, 11);
    a = 0x07;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $942A (bank 2) - lifted by port/tools/lift.py */
void func_942A_b2(void) {
    REGS_IN;
    mff_cycles(0x942A, 25);
    a = 0x23;
    RAM(VRAM_BUFFER) = a;
    a = 0x7D;
    RAM(VRAM_BUFFER + 1) = a;
    a = 0x00;
    RAM(VRAM_BUFFER + 2) = a;
    y = RAM(CREDITS);
    a = prg_read((uint16_t)(0x9449 + y));
    mff_cycles(0x943E, 19);
    RAM(VRAM_BUFFER + 3) = a;
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(VRAM_BUFFER + 4) = a;
    RAM(VRAM_LIST_REQUEST) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9600 (bank 2) - lifted by port/tools/lift.py */
void func_9600_b2(void) {
    REGS_IN;
    mff_cycles(0x9600, 2);
    y = 0x0F;
    for (;;) {
        mff_cycles(0x9602, 4);
        a = prg_read((uint16_t)(0x9636 + y));
        mff_cycles(0x9605, 14);
        RAM(PALETTE_BUF + y) = a;
        RAM(PALETTE_BASE + y) = a;
        y--;
        if (y & 0x80) break;
        mff_cycles(0x9602, 1);
    }
    mff_cycles(0x960E, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xC8DA + y));
    mff_cycles(0x9613, 4);
    x = a;
    y = 0x07;
    for (;;) {
        mff_cycles(0x9616, 4);
        a = prg_read((uint16_t)(0x9646 + x));
        mff_cycles(0x9619, 16);
        RAM(PALETTE_BUF + 16 + y) = a;
        RAM(PALETTE_BASE + 16 + y) = a;
        x--;
        y--;
        if (y & 0x80) break;
        mff_cycles(0x9616, 1);
    }
    mff_cycles(0x9623, 20);
    RAM(PALETTE_REQUEST) = y;
    a = 0x78;
    RAM(CHR_BANKS) = a;
    a = 0x7A;
    RAM(CHR_BANKS + 1) = a;
    y = RAM(LIVES);
    a = prg_read((uint16_t)(0x965E + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9632, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1634, func_F34A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9635, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9800 (bank 2) - lifted by port/tools/lift.py */
void func_9800_b2(void) {
    REGS_IN;
    mff_cycles(0x9800, 7);
    y = RAM(SPECIAL_SCREEN_STEP);
    a = prg_read((uint16_t)(0x9D56 + y));
    mff_cycles(0x9805, 7);
    RAM(TMP12) = a;
    a = prg_read((uint16_t)(0x9D59 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x980A, 8);
    RAM(TMP13) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x000C); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $980F (bank 2) - lifted by port/tools/lift.py */
void func_980F_b2(void) {
    REGS_IN;
    mff_cycles(0x980F, 7);
    a = RAM(SPECIAL_SCREEN_ID);
    m = 0xF0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x9815, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x98AF, 2); call_by_address_tail(0x98AF, 2); return;
    } else {
        mff_cycles(0x9813, 1);
    }
    mff_cycles(0x9818, 7);
    a = RAM(SPECIAL_SCREEN_ID);
    m = 0xFE;
    c = a >= m;
    if (a == m) { mff_cycles(0x981C, 1); goto L_9889; }
    mff_cycles(0x981E, 7);
    a = RAM(SPECIAL_SCREEN_ID);
    m = 0xFF;
    c = a >= m;
    if (a == m) { mff_cycles(0x9822, 1); goto L_984C; }
    mff_cycles(0x9824, 7);
    a = RAM(SPECIAL_SCREEN_ID);
    m = 0xFD;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9828, 1); goto L_9879; }
    mff_cycles(0x982A, 7);
    a = RAM(SPECIAL_SCREEN_ID);
    m = 0xFC;
    c = a >= m;
    if (a == m) { mff_cycles(0x982E, 1); goto L_989F; }
    mff_cycles(0x9830, 7);
    a = RAM(SPECIAL_SCREEN_ID);
    m = 0xFB;
    c = a >= m;
    if (a == m) { mff_cycles(0x9834, 1); goto L_98A7; }
    mff_cycles(0x9836, 7);
    a = RAM(SPECIAL_SCREEN_ID);
    m = 0xFA;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x983A, 1); goto L_9881; }
    mff_cycles(0x983C, 6);
    SYNC_OUT;
    if (!mff_call_at(0x183E, 0x9A9E, 2)) return;
    SYNC_IN;
    mff_cycles(0x983F, 25);
    a = 0x00;
    RAM(UNK_AC) = a;
    RAM(TEXT_CURSOR) = a;
    RAM(DIALOGUE_CHOICE) = a;
    RAM(TEXT_TIMER) = a;
    t = (uint8_t)(RAM(SPECIAL_SCREEN_STEP) + 1);
    RAM(SPECIAL_SCREEN_STEP) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_984C:;
    mff_cycles(0x984C, 14);
    a = RAM(PLAYER_LEVEL);
    RAM(LEVEL_SAVED) = a;
    y = 0x03;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x1854, 0x9AA0, 2)) return;
    SYNC_IN;
    mff_cycles(0x9855, 59);
    x = RAM(VRAM_BUFFER_LEN);
    a = 0x01;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x23;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x50;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = RAM(ROUND);
    RAM(VRAM_BUFFER + x) = a;
    t = (uint8_t)(RAM(VRAM_BUFFER + x) + 1);
    RAM(VRAM_BUFFER + x) = t;
    t = (uint8_t)(RAM(VRAM_BUFFER + x) + 1);
    RAM(VRAM_BUFFER + x) = t;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(VRAM_BUFFER_LEN) = x;
    if (!z) { mff_cycles(0x9877, 1); goto L_98BD; }
L_9879:;
    mff_cycles(0x9879, 6);
    SYNC_OUT;
    if (!mff_call_at(0x187B, 0x9B18, 2)) return;
    SYNC_IN;
    mff_cycles(0x987C, 11);
    a = 0xFA;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SPECIAL_SCREEN_ID) = a;
    SYNC_OUT;
    mff_rts(); return;
L_9881:;
    mff_cycles(0x9881, 8);
    y = 0x04;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x1885, 0x9AA0, 2)) return;
    SYNC_IN;
    mff_cycles(0x9886, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x98BD, 2); call_by_address_tail(0x98BD, 2); return;
L_9889:;
    mff_cycles(0x9889, 8);
    y = 0x07;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x188D, 0x9AA0, 2)) return;
    SYNC_IN;
    mff_cycles(0x988E, 9);
    a = RAM(UNK_B9);
    y = a;
    a = prg_read((uint16_t)(0x98C8 + y));
    mff_cycles(0x9894, 8);
    y = a;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x1897, 0x9AA0, 2)) return;
    SYNC_IN;
    mff_cycles(0x9898, 8);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(UNK_B9) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x98BD, 2); call_by_address_tail(0x98BD, 2); return;
L_989F:;
    mff_cycles(0x989F, 8);
    y = 0x05;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x18A3, 0x9AA0, 2)) return;
    SYNC_IN;
    mff_cycles(0x98A4, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x98BD, 2); call_by_address_tail(0x98BD, 2); return;
L_98A7:;
    mff_cycles(0x98A7, 8);
    y = 0x06;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x18AB, 0x9AA0, 2)) return;
    SYNC_IN;
    mff_cycles(0x98AC, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x98BD, 2); call_by_address_tail(0x98BD, 2); return;
L_98BD:;
    mff_cycles(0x98BD, 19);
    a = 0xF0;
    RAM(TEXT_TIMER) = a;
    a = 0xF0;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SPECIAL_SCREEN_ID) = a;
    RAM(VRAM_REQUEST) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $98AF (bank 2) - lifted by port/tools/lift.py */
void func_98AF_b2(void) {
    REGS_IN;
    mff_cycles(0x98AF, 10);
    t = (uint8_t)(RAM(TEXT_TIMER) - 1);
    RAM(TEXT_TIMER) = t;
    a = RAM(TEXT_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0x98B5, 16);
        t = (uint8_t)(RAM(SPECIAL_SCREEN_STEP) + 1);
        RAM(SPECIAL_SCREEN_STEP) = t;
        t = (uint8_t)(RAM(SPECIAL_SCREEN_STEP) + 1);
        RAM(SPECIAL_SCREEN_STEP) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x18BB, 0x9A9E, 2)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x98B3, 1);
    }
    mff_cycles(0x98BC, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $98BD (bank 2) - lifted by port/tools/lift.py */
void func_98BD_b2(void) {
    REGS_IN;
    mff_cycles(0x98BD, 19);
    a = 0xF0;
    RAM(TEXT_TIMER) = a;
    a = 0xF0;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SPECIAL_SCREEN_ID) = a;
    RAM(VRAM_REQUEST) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $98DD (bank 2) - lifted by port/tools/lift.py */
void func_98DD_b2(void) {
    REGS_IN;
    mff_cycles(0x98DD, 7);
    a = RAM(JOY_PRESSED);
    m = 0x10;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x98E3, 6);
        SYNC_OUT;
        if (!mff_call_at(0x18E5, 0x9A9E, 2)) return;
        SYNC_IN;
        mff_cycles(0x98E6, 11);
        t = (uint8_t)(RAM(SPECIAL_SCREEN_STEP) + 1);
        RAM(SPECIAL_SCREEN_STEP) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x98E1, 1);
    }
    mff_cycles(0x98E9, 10);
    t = (uint8_t)(RAM(TEXT_TIMER) - 1);
    RAM(TEXT_TIMER) = t;
    a = RAM(TEXT_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) {
        mff_cycles(0x98EF, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x98ED, 1);
    }
    mff_cycles(0x98F0, 7);
    a = RAM(SPECIAL_SCREEN_ID);
    if (a != 0x33) { mff_cycles(0x98F4, 2); goto L_9918; }
    mff_cycles(0x98F6, 8);
    y = RAM(PLAYER_LEVEL);
    a = RAM(PLAYER_ID);
    if (a == 0) { mff_cycles(0x98FA, 2); goto L_9905; }
    mff_cycles(0x98FC, 4);
    if (a == 0x01) { mff_cycles(0x98FE, 1); goto L_990A; }
    mff_cycles(0x9900, 4);
    a = prg_read((uint16_t)(0x9D6B + y));
    mff_cycles(0x9903, 2);
    if (a != 0) { mff_cycles(0x9903, 1); goto L_990D; }
L_9905:;
    mff_cycles(0x9905, 4);
    a = prg_read((uint16_t)(0x9D5F + y));
    mff_cycles(0x9908, 2);
    if (a != 0) { mff_cycles(0x9908, 1); goto L_990D; }
L_990A:;
    mff_cycles(0x990A, 4);
    a = prg_read((uint16_t)(0x9D65 + y));
L_990D:;
    mff_cycles(0x990D, 5);
    if (a == RAM(HEALTH)) {
        mff_cycles(0x9911, 7);
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(0x9D5C + y));
        mff_cycles(0x9916, 3);
        RAM(SPECIAL_SCREEN_ID) = a;
    } else {
        mff_cycles(0x990F, 1);
    }
L_9918:;
    mff_cycles(0x9918, 10);
    y = RAM(SPECIAL_SCREEN_ID);
    x = RAM(VRAM_BUFFER_LEN);
    a = prg_read((uint16_t)(0xA54C + y));
    mff_cycles(0x991F, 7);
    RAM(TMP12) = a;
    a = prg_read((uint16_t)(0xA4FD + y));
    mff_cycles(0x9924, 21);
    RAM(TMP13) = a;
    y = RAM(TEXT_CURSOR);
    y++;
    y++;
    y++;
    y++;
    y++;
    a = mem_read((uint16_t)(RAM16(TMP12) + y));
    mff_cycles(0x992F, 4);
    m = 0xF9;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x9933, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x99D6, 2); call_by_address_tail(0x99D6, 2); return;
    } else {
        mff_cycles(0x9931, 1);
    }
    mff_cycles(0x9936, 4);
    m = 0xFA;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x993A, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x9AD1, 2); call_by_address_tail(0x9AD1, 2); return;
    } else {
        mff_cycles(0x9938, 1);
    }
    mff_cycles(0x993D, 4);
    m = 0xFB;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x9941, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x9AD9, 2); call_by_address_tail(0x9AD9, 2); return;
    } else {
        mff_cycles(0x993F, 1);
    }
    mff_cycles(0x9944, 4);
    m = 0xFC;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x9948, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x9B18, 2); call_by_address_tail(0x9B18, 2); return;
    } else {
        mff_cycles(0x9946, 1);
    }
    mff_cycles(0x994B, 4);
    m = 0xFD;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x994F, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x9BA4, 2); call_by_address_tail(0x9BA4, 2); return;
    } else {
        mff_cycles(0x994D, 1);
    }
    mff_cycles(0x9952, 4);
    m = 0xFE;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x9956, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x9B7E, 2); call_by_address_tail(0x9B7E, 2); return;
    } else {
        mff_cycles(0x9954, 1);
    }
    mff_cycles(0x9959, 4);
    m = 0xFF;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x995D, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x999B, 2); call_by_address_tail(0x999B, 2); return;
    } else {
        mff_cycles(0x995B, 1);
    }
    mff_cycles(0x9960, 9);
    RAM(VRAM_BUFFER + 3 + x) = a;
    m = 0x00;
    c = a >= m;
    if (a != m) {
        mff_cycles(0x9967, 7);
        y = 0x00;
        a = mem_read((uint16_t)(RAM16(TMP12) + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x996B, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x196D, func_F34C, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x9965, 1);
    }
    mff_cycles(0x996E, 49);
    a = 0x01;
    RAM(VRAM_BUFFER + x) = a;
    a = 0x23;
    RAM(VRAM_BUFFER + 1 + x) = a;
    a = 0x22;
    c = 0;
    m = RAM(TEXT_CURSOR);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(VRAM_BUFFER + 2 + x) = a;
    t = (uint8_t)(RAM(TEXT_CURSOR) + 1);
    RAM(TEXT_CURSOR) = t;
    x++;
    x++;
    x++;
    x++;
    RAM(VRAM_BUFFER_LEN) = x;
    a = RAM(JOY_HELD);
    a &= 0xC0;
    if (a == 0) { mff_cycles(0x998C, 1); goto L_9992; }
    mff_cycles(0x998E, 4);
    a = 0x00;
    if (a == 0) { mff_cycles(0x9990, 1); goto L_9994; }
L_9992:;
    mff_cycles(0x9992, 2);
    a = 0x0A;
L_9994:;
    mff_cycles(0x9994, 14);
    RAM(TEXT_TIMER) = a;
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(VRAM_REQUEST) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $999B (bank 2) - lifted by port/tools/lift.py */
void func_999B_b2(void) {
    REGS_IN;
    mff_cycles(0x999B, 7);
    y = RAM(SPECIAL_SCREEN_ID);
    a = prg_read((uint16_t)(0xA54C + y));
    mff_cycles(0x99A0, 7);
    RAM(TMP12) = a;
    a = prg_read((uint16_t)(0xA4FD + y));
    mff_cycles(0x99A5, 10);
    RAM(TMP13) = a;
    y = 0x01;
    a = mem_read((uint16_t)(RAM16(TMP12) + y));
    mff_cycles(0x99AB, 2);
    if (a == 0) {
        mff_cycles(0x99AD, 7);
        a = RAM(JOY_PRESSED);
        a &= 0xC0;
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0x99B3, 56);
            RAM(TEXT_TIMER) = a;
            x = RAM(VRAM_BUFFER_LEN);
            a = 0x01;
            RAM(VRAM_BUFFER + x) = a;
            x++;
            a = 0x23;
            RAM(VRAM_BUFFER + x) = a;
            x++;
            a = 0x6F;
            RAM(VRAM_BUFFER + x) = a;
            x++;
            a = 0x7A;
            RAM(VRAM_BUFFER + x) = a;
            x++;
            RAM(VRAM_BUFFER_LEN) = x;
            a = 0xFF;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(VRAM_REQUEST) = a;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0x99B1, 1);
        }
        mff_cycles(0x99D6, 6);
        SYNC_OUT;
        if (!mff_call_at(0x19D8, 0x9A9E, 2)) return;
        SYNC_IN;
        mff_cycles(0x99D9, 12);
        y = RAM(PLAYER_ID);
        y++;
        y++;
        a = mem_read((uint16_t)(RAM16(TMP12) + y));
        mff_cycles(0x99DF, 2);
        if (a != 0) {
            mff_cycles(0x99E1, 17);
            RAM(SPECIAL_SCREEN_ID) = a;
            a = 0x00;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(TEXT_CURSOR) = a;
            RAM(TEXT_TIMER) = a;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0x99DF, 1);
        }
        mff_cycles(0x99EA, 11);
        t = (uint8_t)(RAM(SPECIAL_SCREEN_STEP) + 1);
        RAM(SPECIAL_SCREEN_STEP) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x99AB, 1);
    }
    mff_cycles(0x99ED, 8);
    y = 0x02;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x19F1, 0x9AA0, 2)) return;
    SYNC_IN;
    mff_cycles(0x99F2, 7);
    a = RAM(JOY_PRESSED);
    a &= 0xC0;
    if (a != 0) { mff_cycles(0x99F6, 2); goto L_9A56; }
    mff_cycles(0x99F8, 7);
    a = RAM(JOY_PRESSED);
    a &= 0x03;
    if (a != 0) {
        mff_cycles(0x99FE, 8);
        a = 0x2F;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x1A02, func_F34C, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9A03, 8);
        a = RAM(DIALOGUE_CHOICE);
        a ^= 0x80;
        RAM(DIALOGUE_CHOICE) = a;
    } else {
        mff_cycles(0x99FC, 2);
    }
    mff_cycles(0x9A09, 50);
    x = RAM(VRAM_BUFFER_LEN);
    a = 0x01;
    RAM(VRAM_BUFFER + x) = a;
    a = 0x01;
    RAM(VRAM_BUFFER + 4 + x) = a;
    a = 0x23;
    RAM(VRAM_BUFFER + 1 + x) = a;
    a = 0x23;
    RAM(VRAM_BUFFER + 5 + x) = a;
    a = 0x67;
    RAM(VRAM_BUFFER + 2 + x) = a;
    a = 0x73;
    RAM(VRAM_BUFFER + 6 + x) = a;
    a = RAM(DIALOGUE_CHOICE);
    if (a & 0x80) { mff_cycles(0x9A2B, 1); goto L_9A39; }
    mff_cycles(0x9A2D, 16);
    a = 0x79;
    RAM(VRAM_BUFFER + 3 + x) = a;
    a = 0x00;
    RAM(VRAM_BUFFER + 7 + x) = a;
    if (a == 0) { mff_cycles(0x9A37, 1); goto L_9A43; }
L_9A39:;
    mff_cycles(0x9A39, 14);
    a = 0x00;
    RAM(VRAM_BUFFER + 3 + x) = a;
    a = 0x79;
    RAM(VRAM_BUFFER + 7 + x) = a;
L_9A43:;
    mff_cycles(0x9A43, 35);
    x++;
    x++;
    x++;
    x++;
    x++;
    x++;
    x++;
    x++;
    RAM(VRAM_BUFFER_LEN) = x;
    a = 0x00;
    RAM(TEXT_TIMER) = a;
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(VRAM_REQUEST) = a;
    SYNC_OUT;
    mff_rts(); return;
L_9A56:;
    mff_cycles(0x9A56, 7);
    y = 0x01;
    a = mem_read((uint16_t)(RAM16(TMP12) + y));
    mff_cycles(0x9A5A, 4);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x9A5E, 6);
        SYNC_OUT;
        if (!mff_call_at(0x1A60, 0x9A74, 2)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x9A5C, 1);
    }
    mff_cycles(0x9A61, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1A63, 0x9A9E, 2)) return;
    SYNC_IN;
    mff_cycles(0x9A64, 7);
    y = 0x02;
    a = RAM(DIALOGUE_CHOICE);
    if (a & 0x80) {
        mff_cycles(0x9A6A, 2);
        y++;
    } else {
        mff_cycles(0x9A68, 1);
    }
    mff_cycles(0x9A6B, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(TEXT_CURSOR) = a;
    RAM(DIALOGUE_CHOICE) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x99DD, 2); call_by_address_tail(0x99DD, 2); return;
}

/*  $99D6 (bank 2) - lifted by port/tools/lift.py */
void func_99D6_b2(void) {
    REGS_IN;
    mff_cycles(0x99D6, 6);
    SYNC_OUT;
    if (!mff_call_at(0x19D8, 0x9A9E, 2)) return;
    SYNC_IN;
    mff_cycles(0x99D9, 12);
    y = RAM(PLAYER_ID);
    y++;
    y++;
    a = mem_read((uint16_t)(RAM16(TMP12) + y));
    mff_cycles(0x99DF, 2);
    if (a != 0) {
        mff_cycles(0x99E1, 17);
        RAM(SPECIAL_SCREEN_ID) = a;
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(TEXT_CURSOR) = a;
        RAM(TEXT_TIMER) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x99DF, 1);
    }
    mff_cycles(0x99EA, 11);
    t = (uint8_t)(RAM(SPECIAL_SCREEN_STEP) + 1);
    RAM(SPECIAL_SCREEN_STEP) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $99DD (bank 2) - lifted by port/tools/lift.py */
void func_99DD_b2(void) {
    REGS_IN;
    mff_cycles(0x99DD, 5);
    a = mem_read((uint16_t)(RAM16(TMP12) + y));
    mff_cycles(0x99DF, 2);
    if (a != 0) {
        mff_cycles(0x99E1, 17);
        RAM(SPECIAL_SCREEN_ID) = a;
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(TEXT_CURSOR) = a;
        RAM(TEXT_TIMER) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x99DF, 1);
    }
    mff_cycles(0x99EA, 11);
    t = (uint8_t)(RAM(SPECIAL_SCREEN_STEP) + 1);
    RAM(SPECIAL_SCREEN_STEP) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9A74 (bank 2) - lifted by port/tools/lift.py */
void func_9A74_b2(void) {
    REGS_IN;
    mff_cycles(0x9A74, 7);
    y = 0x04;
    a = mem_read((uint16_t)(RAM16(TMP12) + y));
    mff_cycles(0x9A78, 5);
    m = RAM(DIALOGUE_CHOICE);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x9A7C, 6);
        SYNC_OUT;
        if (!mff_call_at(0x1A7E, 0x9AC2, 2)) return;
        SYNC_IN;
        mff_cycles(0x9A7F, 19);
        a = RAM(OBJ_LIFE + x);
        c = 1;
        m = 0x08;
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_LIFE + x) = a;
        SYNC_OUT;
        if (!mff_call_at(0x1A8A, 0x9AF4, 2)) return;
        SYNC_IN;
        mff_cycles(0x9A8B, 11);
        t = (uint8_t)(RAM(UNK_AC) + 1);
        RAM(UNK_AC) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9A7A, 1);
    }
    mff_cycles(0x9A8E, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9A8F (bank 2) - lifted by port/tools/lift.py */
void func_9A8F_b2(void) {
    REGS_IN;
    mff_cycles(0x9A8F, 16);
    y = 0x00;
    RAM(SPECIAL_SCREEN_STEP) = y;
    RAM(SPECIAL_SCREEN_ID) = y;
    y = 0x01;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x1A99, 0x9AA0, 2)) return;
    SYNC_IN;
    mff_cycles(0x9A9A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1A9C, func_E954, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9A9D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9A9E (bank 2) - lifted by port/tools/lift.py */
void func_9A9E_b2(void) {
    REGS_IN;
    mff_cycles(0x9A9E, 6);
    y = 0x00;
    a = prg_read((uint16_t)(0x9BAE + y));
    mff_cycles(0x9AA3, 7);
    RAM(TMP10) = a;
    a = prg_read((uint16_t)(0x9BBD + y));
    mff_cycles(0x9AA8, 8);
    RAM(TMP11) = a;
    x = RAM(VRAM_BUFFER_LEN);
    y = 0x00;
L_9AAE:;
    mff_cycles(0x9AAE, 5);
    a = mem_read((uint16_t)(RAM16(TMP10) + y));
    mff_cycles(0x9AB0, 4);
    m = 0xFF;
    c = a >= m;
    if (a == m) { mff_cycles(0x9AB2, 1); goto L_9ABB; }
    mff_cycles(0x9AB4, 11);
    RAM(VRAM_BUFFER + x) = a;
    y++;
    x++;
    if (x != 0) { mff_cycles(0x9AAE, 1); goto L_9AAE; }
L_9ABB:;
    mff_cycles(0x9ABB, 14);
    RAM(VRAM_BUFFER_LEN) = x;
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(VRAM_REQUEST) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9AA0 (bank 2) - lifted by port/tools/lift.py */
void func_9AA0_b2(void) {
    REGS_IN;
    mff_cycles(0x9AA0, 4);
    a = prg_read((uint16_t)(0x9BAE + y));
    mff_cycles(0x9AA3, 7);
    RAM(TMP10) = a;
    a = prg_read((uint16_t)(0x9BBD + y));
    mff_cycles(0x9AA8, 8);
    RAM(TMP11) = a;
    x = RAM(VRAM_BUFFER_LEN);
    y = 0x00;
L_9AAE:;
    mff_cycles(0x9AAE, 5);
    a = mem_read((uint16_t)(RAM16(TMP10) + y));
    mff_cycles(0x9AB0, 4);
    m = 0xFF;
    c = a >= m;
    if (a == m) { mff_cycles(0x9AB2, 1); goto L_9ABB; }
    mff_cycles(0x9AB4, 11);
    RAM(VRAM_BUFFER + x) = a;
    y++;
    x++;
    if (x != 0) { mff_cycles(0x9AAE, 1); goto L_9AAE; }
L_9ABB:;
    mff_cycles(0x9ABB, 14);
    RAM(VRAM_BUFFER_LEN) = x;
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(VRAM_REQUEST) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9AC2 (bank 2) - lifted by port/tools/lift.py */
void func_9AC2_b2(void) {
    REGS_IN;
    mff_cycles(0x9AC2, 2);
    x = 0x02;
L_9AC4:;
    mff_cycles(0x9AC4, 8);
    a = RAM(OBJ_KIND + x);
    m = 0x07;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9AC9, 1); goto L_9AD0; }
    mff_cycles(0x9ACB, 6);
    x++;
    m = 0x10;
    c = x >= m;
    n = ((uint8_t)(x - m)) >> 7;
    z = ((uint8_t)(x - m)) == 0;
    if (!z) { mff_cycles(0x9AC4, 1); goto L_9AC4; }
L_9AD0:;
    mff_cycles(0x9AD0, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9AD1 (bank 2) - lifted by port/tools/lift.py */
void func_9AD1_b2(void) {
    REGS_IN;
    mff_cycles(0x9AD1, 9);
    a = 0xE4;
    RAM(TMP7) = a;
    a = 0x28;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0x9AD9, 7);
        a = 0xEA;
        RAM(TMP7) = a;
        a = 0x2B;
        n = (a) >> 7;
        z = (a) == 0;
    } else {
        mff_cycles(0x9AD7, 1);
    }
    mff_cycles(0x9ADF, 9);
    RAM(CHR_BANKS + 4) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1AE3, 0x9AC2, 2)) return;
    SYNC_IN;
    mff_cycles(0x9AE4, 31);
    a = RAM(TMP7);
    RAM(OBJ_ANIM + x) = a;
    a = 0x00;
    RAM(OBJ_ANIM_TIMER + x) = a;
    RAM(OBJ_ANIM_FRAME + x) = a;
    t = (uint8_t)(RAM(TEXT_CURSOR) + 1);
    RAM(TEXT_CURSOR) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9AD9 (bank 2) - lifted by port/tools/lift.py */
void func_9AD9_b2(void) {
    REGS_IN;
    mff_cycles(0x9AD9, 16);
    a = 0xEA;
    RAM(TMP7) = a;
    a = 0x2B;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(CHR_BANKS + 4) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1AE3, 0x9AC2, 2)) return;
    SYNC_IN;
    mff_cycles(0x9AE4, 31);
    a = RAM(TMP7);
    RAM(OBJ_ANIM + x) = a;
    a = 0x00;
    RAM(OBJ_ANIM_TIMER + x) = a;
    RAM(OBJ_ANIM_FRAME + x) = a;
    t = (uint8_t)(RAM(TEXT_CURSOR) + 1);
    RAM(TEXT_CURSOR) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9AF4 (bank 2) - lifted by port/tools/lift.py */
void func_9AF4_b2(void) {
    REGS_IN;
    mff_cycles(0x9AF4, 58);
    x = RAM(VRAM_BUFFER_LEN);
    a = 0x01;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x20;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x72;
    c = 1;
    m = RAM(UNK_AC);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x10;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    RAM(VRAM_BUFFER_LEN) = x;
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(VRAM_REQUEST) = a;
    SYNC_OUT;
    mff_rts(); return;
}
