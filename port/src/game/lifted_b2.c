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

/*  $A700 (bank 2) - lifted by port/tools/lift.py */
void func_A700_b2(void) {
    REGS_IN;
    mff_cycles(0xA700, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA730, 2); call_by_address_tail(0xA730, 2); return;
}

/*  $A703 (bank 2) - lifted by port/tools/lift.py */
void func_A703_b2(void) {
    REGS_IN;
    mff_cycles(0xA703, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA731, 2); call_by_address_tail(0xA731, 2); return;
}

/*  $A706 (bank 2) - lifted by port/tools/lift.py */
void func_A706_b2(void) {
    REGS_IN;
    mff_cycles(0xA706, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAD58, 2); call_by_address_tail(0xAD58, 2); return;
}

/*  $A709 (bank 2) - lifted by port/tools/lift.py */
void func_A709_b2(void) {
    REGS_IN;
    mff_cycles(0xA709, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAD37, 2); call_by_address_tail(0xAD37, 2); return;
}

/*  $A70C (bank 2) - lifted by port/tools/lift.py */
void func_A70C_b2(void) {
    REGS_IN;
    mff_cycles(0xA70C, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xABE1, 2); call_by_address_tail(0xABE1, 2); return;
}

/*  $A70F (bank 2) - lifted by port/tools/lift.py */
void func_A70F_b2(void) {
    REGS_IN;
    mff_cycles(0xA70F, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xABC4, 2); call_by_address_tail(0xABC4, 2); return;
}

/*  $A712 (bank 2) - lifted by port/tools/lift.py */
void func_A712_b2(void) {
    REGS_IN;
    mff_cycles(0xA712, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA916, 2); call_by_address_tail(0xA916, 2); return;
}

/*  $A715 (bank 2) - lifted by port/tools/lift.py */
void func_A715_b2(void) {
    REGS_IN;
    mff_cycles(0xA715, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA916, 2); call_by_address_tail(0xA916, 2); return;
}

/*  $A718 (bank 2) - lifted by port/tools/lift.py */
void func_A718_b2(void) {
    REGS_IN;
    mff_cycles(0xA718, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAB08, 2); call_by_address_tail(0xAB08, 2); return;
}

/*  $A71B (bank 2) - lifted by port/tools/lift.py */
void func_A71B_b2(void) {
    REGS_IN;
    mff_cycles(0xA71B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA7A5, 2); call_by_address_tail(0xA7A5, 2); return;
}

/*  $A71E (bank 2) - lifted by port/tools/lift.py */
void func_A71E_b2(void) {
    REGS_IN;
    mff_cycles(0xA71E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA7A5, 2); call_by_address_tail(0xA7A5, 2); return;
}

/*  $A721 (bank 2) - lifted by port/tools/lift.py */
void func_A721_b2(void) {
    REGS_IN;
    mff_cycles(0xA721, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAB41, 2); call_by_address_tail(0xAB41, 2); return;
}

/*  $A727 (bank 2) - lifted by port/tools/lift.py */
void func_A727_b2(void) {
    REGS_IN;
    mff_cycles(0xA727, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA8A8, 2); call_by_address_tail(0xA8A8, 2); return;
}

/*  $A72A (bank 2) - lifted by port/tools/lift.py */
void func_A72A_b2(void) {
    REGS_IN;
    mff_cycles(0xA72A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA9E4, 2); call_by_address_tail(0xA9E4, 2); return;
}

/*  $A72D (bank 2) - lifted by port/tools/lift.py */
void func_A72D_b2(void) {
    REGS_IN;
    mff_cycles(0xA72D, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAA82, 2); call_by_address_tail(0xAA82, 2); return;
}

/*  $A730 (bank 2) - lifted by port/tools/lift.py */
void func_A730_b2(void) {
    REGS_IN;
    mff_cycles(0xA730, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A731 (bank 2) - lifted by port/tools/lift.py */
void func_A731_b2(void) {
    REGS_IN;
    mff_cycles(0xA731, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0733, func_F314, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA734, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0736, func_F365, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA737, 2);
    if (c) {
        mff_cycles(0xA739, 4);
        m = 0x0C;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!c) {
            mff_cycles(0xA73D, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x073F, func_F374, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xA740, 4);
            m = 0x18;
            c = a >= m;
            n = ((uint8_t)(a - m)) >> 7;
            z = ((uint8_t)(a - m)) == 0;
            if (!c) {
                mff_cycles(0xA744, 4);
                m = 0x11;
                c = a >= m;
                n = ((uint8_t)(a - m)) >> 7;
                z = ((uint8_t)(a - m)) == 0;
                if (c) {
                    mff_cycles(0xA748, 9);
                    a = RAM(OBJ_DIR + x);
                    m = RAM(PLAYER_DIR);
                    c = a >= m;
                    n = ((uint8_t)(a - m)) >> 7;
                    z = ((uint8_t)(a - m)) == 0;
                    if (!z) {
                        mff_cycles(0xA74F, 11);
                        y = RAM(PLAYER_ID);
                        a = RAM(OBJ_ANIM);
                        m = prg_read((uint16_t)(0xA7A2 + y));
                        c = a >= m;
                        n = ((uint8_t)(a - m)) >> 7;
                        z = ((uint8_t)(a - m)) == 0;
                        mff_cycles(0xA757, 2);
                        if (z) {
                            mff_cycles(0xA759, 8);
                            a = 0x22;
                            n = (a) >> 7;
                            z = (a) == 0;
                            SYNC_OUT;
                            if (!mff_call_fn(0x075D, func_F34C, 0xE000)) return;
                            SYNC_IN;
                            mff_cycles(0xA75E, 7);
                            y = RAM(PLAYER_ID);
                            a = prg_read((uint16_t)(0xA78A + y));
                            mff_cycles(0xA763, 7);
                            RAM(TMP2) = a;
                            a = prg_read((uint16_t)(0xA78D + y));
                            mff_cycles(0xA768, 14);
                            RAM(TMP3) = a;
                            y = RAM(PLAYER_LEVEL);
                            a = RAM(HEALTH);
                            m = mem_read((uint16_t)(RAM16(TMP2) + y));
                            c = a >= m;
                            n = ((uint8_t)(a - m)) >> 7;
                            z = ((uint8_t)(a - m)) == 0;
                            mff_cycles(0xA770, 2);
                            if (!z) {
                                mff_cycles(0xA772, 11);
                                y = 0x01;
                                a = RAM(HEALTH);
                                c = 0;
                                m = prg_read((uint16_t)(0xFCC0 + y));
                                w = a + m + c;
                                v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
                                a = (uint8_t)w;
                                mff_cycles(0xA77A, 11);
                                RAM(HEALTH) = a;
                                y = RAM(PLAYER_LEVEL);
                                a = mem_read((uint16_t)(RAM16(TMP2) + y));
                                mff_cycles(0xA780, 5);
                                m = RAM(HEALTH);
                                c = a >= m;
                                n = ((uint8_t)(a - m)) >> 7;
                                z = ((uint8_t)(a - m)) == 0;
                                if (!c) {
                                    mff_cycles(0xA784, 3);
                                    RAM(HEALTH) = a;
                                } else {
                                    mff_cycles(0xA782, 1);
                                }
                            } else {
                                mff_cycles(0xA770, 1);
                            }
                            mff_cycles(0xA786, 6);
                            SYNC_OUT;
                            if (!mff_call_fn(0x0788, func_F2BE, 0xE000)) return;
                            SYNC_IN;
                        } else {
                            mff_cycles(0xA757, 1);
                        }
                    } else {
                        mff_cycles(0xA74D, 1);
                    }
                } else {
                    mff_cycles(0xA746, 1);
                }
            } else {
                mff_cycles(0xA742, 1);
            }
        } else {
            mff_cycles(0xA73B, 1);
        }
    } else {
        mff_cycles(0xA737, 1);
    }
    mff_cycles(0xA789, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A7A5 (bank 2) - lifted by port/tools/lift.py */
void func_A7A5_b2(void) {
    REGS_IN;
    mff_cycles(0xA7A5, 7);
    a = RAM(GAME_ACTION);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA7A9, 2); goto L_A825; }
    mff_cycles(0xA7AB, 4);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA7AD, 2); goto L_A825; }
    mff_cycles(0xA7AF, 4);
    m = 0x06;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA7B1, 2); goto L_A831; }
    mff_cycles(0xA7B3, 4);
    m = 0x1F;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA7B5, 2); goto L_A831; }
    mff_cycles(0xA7B7, 4);
    m = 0x04;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA7B9, 2); goto L_A831; }
    mff_cycles(0xA7BB, 4);
    m = 0x13;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA7BD, 2); goto L_A831; }
    mff_cycles(0xA7BF, 4);
    m = 0x15;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA7C1, 2); goto L_A831; }
    mff_cycles(0xA7C3, 4);
    m = 0x10;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA7C5, 2); goto L_A831; }
    mff_cycles(0xA7C7, 4);
    m = 0x12;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA7C9, 2); goto L_A831; }
    mff_cycles(0xA7CB, 4);
    m = 0x21;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA7CD, 2); goto L_A831; }
    mff_cycles(0xA7CF, 5);
    a = RAM(PLAYER_ID);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA7D1, 1); goto L_A7D9; }
    mff_cycles(0xA7D3, 7);
    a = RAM(GAME_ACTION);
    m = 0x0B;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA7D7, 2); goto L_A831; }
L_A7D9:;
    mff_cycles(0xA7D9, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x07DB, func_F55A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA7DC, 2);
    if (c) { mff_cycles(0xA7DC, 2); goto L_A831; }
    mff_cycles(0xA7DE, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x07E0, func_F365, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA7E1, 2);
    if (c) { mff_cycles(0xA7E1, 2); goto L_A850; }
    mff_cycles(0xA7E3, 4);
    m = 0x10;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xA7E5, 2); goto L_A850; }
    mff_cycles(0xA7E7, 4);
    m = 0x0A;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xA7E9, 2); goto L_A850; }
    mff_cycles(0xA7EB, 7);
    a = RAM(JOY_PRESSED);
    m = 0x40;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA7EF, 2); goto L_A850; }
    mff_cycles(0xA7F1, 8);
    a = 0x2E;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x07F5, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA7F6, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE210 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA7FB, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x07FD, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA7FE, 4);
    a = prg_read((uint16_t)(0xE258 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA801, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0803, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA804, 15);
    a = 0x02;
    RAM(GAME_ACTION) = a;
    a = 0x0C;
    RAM(ACTION_TIMER) = a;
    a = RAM(WEAPON_HELD);
    if (a != 0) { mff_cycles(0xA80E, 1); goto L_A832; }
    mff_cycles(0xA810, 7);
    a = RAM(PLAYER_ID);
    m = 0x01;
    c = a >= m;
    if (a != m) { mff_cycles(0xA814, 1); goto L_A82A; }
    mff_cycles(0xA816, 8);
    a = RAM(OBJ_ID + x);
    m = 0x6A;
    c = a >= m;
    if (a != m) { mff_cycles(0xA81B, 1); goto L_A826; }
    mff_cycles(0xA81D, 10);
    a = RAM(OBJ_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(WEAPON_AMMO) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA82A, 2); call_by_address_tail(0xA82A, 2); return;
L_A825:;
    mff_cycles(0xA825, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A826:;
    mff_cycles(0xA826, 5);
    a = 0x07;
    RAM(WEAPON_AMMO) = a;
L_A82A:;
    mff_cycles(0xA82A, 11);
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(WEAPON_HELD) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0830, func_F2BE, 0xE000)) return;
    SYNC_IN;
L_A831:;
    mff_cycles(0xA831, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A832:;
    mff_cycles(0xA832, 7);
    a = RAM(PLAYER_ID);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA836, 1); goto L_A850; }
    mff_cycles(0xA838, 6);
    SYNC_OUT;
    if (!mff_call_at(0x083A, 0xA851, 2)) return;
    SYNC_IN;
    mff_cycles(0xA83B, 8);
    a = RAM(OBJ_ID + x);
    m = 0x6A;
    c = a >= m;
    if (a == m) { mff_cycles(0xA840, 1); goto L_A848; }
    mff_cycles(0xA842, 7);
    a = 0x07;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(WEAPON_AMMO) = a;
    if (!z) { mff_cycles(0xA846, 1); goto L_A84D; }
L_A848:;
    mff_cycles(0xA848, 7);
    a = RAM(OBJ_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(WEAPON_AMMO) = a;
L_A84D:;
    mff_cycles(0xA84D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x084F, func_F2BE, 0xE000)) return;
    SYNC_IN;
L_A850:;
    mff_cycles(0xA850, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A8A8 (bank 2) - lifted by port/tools/lift.py */
void func_A8A8_b2(void) {
    REGS_IN;
    mff_cycles(0xA8A8, 8);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    if (a != 0) { mff_cycles(0xA8AD, 1); goto L_A8CC; }
    mff_cycles(0xA8AF, 17);
    a = 0x19;
    RAM(OBJ_VX_FRAC + x) = a;
    a = RAM(PLAYER_ID);
    RAM(OBJ_VX_FRAC + x) = a;
    if (a != 0) { mff_cycles(0xA8B9, 1); goto L_A8BF; }
    mff_cycles(0xA8BB, 4);
    a = 0x04;
    if (a != 0) { mff_cycles(0xA8BD, 1); goto L_A8C1; }
L_A8BF:;
    mff_cycles(0xA8BF, 2);
    a = 0x05;
L_A8C1:;
    mff_cycles(0xA8C1, 19);
    RAM(OBJ_VX + x) = a;
    a = 0x3C;
    RAM(OBJ_TIMER2 + x) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
L_A8CC:;
    mff_cycles(0xA8CC, 8);
    a = RAM(OBJ_ANIM + x);
    if (a != 0x74) { mff_cycles(0xA8D1, 1); goto L_A8E7; }
    mff_cycles(0xA8D3, 7);
    a = RAM(GAME_ACTION);
    m = 0x05;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA8D7, 1); goto L_A8E4; }
    mff_cycles(0xA8D9, 4);
    m = 0x06;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA8DB, 1); goto L_A8E4; }
    mff_cycles(0xA8DD, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x05;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA8E2, 1); goto L_A8E7; }
L_A8E4:;
    mff_cycles(0xA8E4, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF2BE, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F2BE(); g_code_window_base = swb; } return;
L_A8E7:;
    mff_cycles(0xA8E7, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    if (a != 0) {
        mff_cycles(0xA8EE, 13);
        a = RAM(OBJ_X + x);
        c = 1;
        m = RAM(SCROLL_X);
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        m = 0xFA;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0xA8F8, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xF2BE, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F2BE(); g_code_window_base = swb; } return;
        } else {
            mff_cycles(0xA8F6, 1);
        }
        mff_cycles(0xA8FB, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x08FD, func_F090, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA8FE, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0900, func_F183, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA901, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xA8EC, 2);
    }
    mff_cycles(0xA902, 13);
    a = RAM(OBJ_X + x);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0xA90C, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF2BE, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F2BE(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xA90A, 1);
    }
    mff_cycles(0xA90F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0911, func_F099, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA912, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0914, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA915, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A916 (bank 2) - lifted by port/tools/lift.py */
void func_A916_b2(void) {
    REGS_IN;
    mff_cycles(0xA916, 8);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    if (a == 0) {
        mff_cycles(0xA91D, 53);
        RAM(OBJ_VX + x) = a;
        a = 0xCC;
        RAM(OBJ_VX_FRAC + x) = a;
        a = 0xE8;
        RAM(OBJ_VY_FRAC + x) = a;
        a = 0x03;
        RAM(OBJ_VY + x) = a;
        a = 0x3C;
        RAM(OBJ_TIMER2 + x) = a;
        t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_STATE + x) = t;
        a = RAM(OBJ_Y);
        c = 0;
        m = 0x04;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(OBJ_TIMER + x) = a;
    } else {
        mff_cycles(0xA91B, 1);
    }
    mff_cycles(0xA940, 8);
    a = RAM(OBJ_STATE + x);
    a &= 0x02;
    if (a != 0) { mff_cycles(0xA945, 1); goto L_A9BF; }
    mff_cycles(0xA947, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0xA94E, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0950, func_F183, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA951, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA957, 2); call_by_address_tail(0xA957, 2); return;
    } else {
        mff_cycles(0xA94C, 1);
    }
    mff_cycles(0xA954, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0956, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA957, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0959, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA95A, 10);
    a = RAM(OBJ_Y + x);
    m = RAM(OBJ_TIMER + x);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xA960, 1); goto L_A9BB; }
    mff_cycles(0xA962, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xA9E1 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA967, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0969, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA96A, 8);
    a = RAM(OBJ_ID + x);
    m = 0x67;
    c = a >= m;
    if (a == m) { mff_cycles(0xA96F, 1); goto L_A9BC; }
    mff_cycles(0xA971, 17);
    a = RAM(RNG);
    c = 0;
    m = RAM(RNG + 1);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(RNG) = a;
    a &= 0x0F;
    m = 0x0C;
    c = a >= m;
    if (c) { mff_cycles(0xA97C, 1); goto L_A9BC; }
    mff_cycles(0xA97E, 8);
    a = RAM(OBJ_Y + x);
    m = 0x94;
    c = a >= m;
    if (!c) { mff_cycles(0xA983, 1); goto L_A9BC; }
    mff_cycles(0xA985, 7);
    a = RAM(ROUND);
    if (a == 0x01) { mff_cycles(0xA989, 1); goto L_A991; }
    mff_cycles(0xA98B, 4);
    m = 0x03;
    c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA98D, 1); goto L_A99A; }
    mff_cycles(0xA98F, 2);
    if (!z) { mff_cycles(0xA98F, 1); goto L_A9AB; }
L_A991:;
    mff_cycles(0xA991, 8);
    a = RAM(OBJ_Y + x);
    m = 0xA0;
    c = a >= m;
    if (!c) { mff_cycles(0xA996, 1); goto L_A9BC; }
    mff_cycles(0xA998, 2);
    if (c) { mff_cycles(0xA998, 1); goto L_A9AB; }
L_A99A:;
    mff_cycles(0xA99A, 7);
    a = RAM(SUB_AREA);
    m = 0x01;
    c = a >= m;
    if (a != m) { mff_cycles(0xA99E, 1); goto L_A9AB; }
    mff_cycles(0xA9A0, 8);
    a = RAM(OBJ_X + x);
    m = 0x18;
    c = a >= m;
    if (!c) { mff_cycles(0xA9A5, 1); goto L_A9BC; }
    mff_cycles(0xA9A7, 4);
    m = 0xE8;
    c = a >= m;
    if (c) { mff_cycles(0xA9A9, 1); goto L_A9BC; }
L_A9AB:;
    mff_cycles(0xA9AB, 24);
    a = 0x69;
    RAM(OBJ_ID + x) = a;
    RAM(OBJ_SAVED_ID + x) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    RAM(OBJ_SAVED_STATE + x) = a;
L_A9BB:;
    mff_cycles(0xA9BB, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A9BC:;
    mff_cycles(0xA9BC, 7);
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
L_A9BF:;
    mff_cycles(0xA9BF, 13);
    t = (uint8_t)(RAM(OBJ_TIMER2 + x) - 1);
    RAM(OBJ_TIMER2 + x) = t;
    a = RAM(OBJ_TIMER2 + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xA9C5, 1); goto L_A9DE; }
    mff_cycles(0xA9C7, 4);
    a &= 0x01;
    if (a != 0) { mff_cycles(0xA9C9, 1); goto L_A9D5; }
    mff_cycles(0xA9CB, 13);
    a = RAM(OBJ_FLAGS + x);
    a |= 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS + x) = a;
    if (!z) { mff_cycles(0xA9D3, 1); goto L_A9DD; }
L_A9D5:;
    mff_cycles(0xA9D5, 11);
    a = RAM(OBJ_FLAGS + x);
    a &= 0xFB;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS + x) = a;
L_A9DD:;
    mff_cycles(0xA9DD, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A9DE:;
    mff_cycles(0xA9DE, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF2BE, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F2BE(); g_code_window_base = swb; } return;
}

/*  $A957 (bank 2) - lifted by port/tools/lift.py */
void func_A957_b2(void) {
    REGS_IN;
    mff_cycles(0xA957, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0959, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA95A, 10);
    a = RAM(OBJ_Y + x);
    m = RAM(OBJ_TIMER + x);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xA960, 1); goto L_A9BB; }
    mff_cycles(0xA962, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xA9E1 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA967, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0969, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA96A, 8);
    a = RAM(OBJ_ID + x);
    m = 0x67;
    c = a >= m;
    if (a == m) { mff_cycles(0xA96F, 1); goto L_A9BC; }
    mff_cycles(0xA971, 17);
    a = RAM(RNG);
    c = 0;
    m = RAM(RNG + 1);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(RNG) = a;
    a &= 0x0F;
    m = 0x0C;
    c = a >= m;
    if (c) { mff_cycles(0xA97C, 1); goto L_A9BC; }
    mff_cycles(0xA97E, 8);
    a = RAM(OBJ_Y + x);
    m = 0x94;
    c = a >= m;
    if (!c) { mff_cycles(0xA983, 1); goto L_A9BC; }
    mff_cycles(0xA985, 7);
    a = RAM(ROUND);
    if (a == 0x01) { mff_cycles(0xA989, 1); goto L_A991; }
    mff_cycles(0xA98B, 4);
    m = 0x03;
    c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA98D, 1); goto L_A99A; }
    mff_cycles(0xA98F, 2);
    if (!z) { mff_cycles(0xA98F, 1); goto L_A9AB; }
L_A991:;
    mff_cycles(0xA991, 8);
    a = RAM(OBJ_Y + x);
    m = 0xA0;
    c = a >= m;
    if (!c) { mff_cycles(0xA996, 1); goto L_A9BC; }
    mff_cycles(0xA998, 2);
    if (c) { mff_cycles(0xA998, 1); goto L_A9AB; }
L_A99A:;
    mff_cycles(0xA99A, 7);
    a = RAM(SUB_AREA);
    m = 0x01;
    c = a >= m;
    if (a != m) { mff_cycles(0xA99E, 1); goto L_A9AB; }
    mff_cycles(0xA9A0, 8);
    a = RAM(OBJ_X + x);
    m = 0x18;
    c = a >= m;
    if (!c) { mff_cycles(0xA9A5, 1); goto L_A9BC; }
    mff_cycles(0xA9A7, 4);
    m = 0xE8;
    c = a >= m;
    if (c) { mff_cycles(0xA9A9, 1); goto L_A9BC; }
L_A9AB:;
    mff_cycles(0xA9AB, 24);
    a = 0x69;
    RAM(OBJ_ID + x) = a;
    RAM(OBJ_SAVED_ID + x) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    RAM(OBJ_SAVED_STATE + x) = a;
L_A9BB:;
    mff_cycles(0xA9BB, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A9BC:;
    mff_cycles(0xA9BC, 20);
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
    t = (uint8_t)(RAM(OBJ_TIMER2 + x) - 1);
    RAM(OBJ_TIMER2 + x) = t;
    a = RAM(OBJ_TIMER2 + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xA9C5, 1); goto L_A9DE; }
    mff_cycles(0xA9C7, 4);
    a &= 0x01;
    if (a != 0) { mff_cycles(0xA9C9, 1); goto L_A9D5; }
    mff_cycles(0xA9CB, 13);
    a = RAM(OBJ_FLAGS + x);
    a |= 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS + x) = a;
    if (!z) { mff_cycles(0xA9D3, 1); goto L_A9DD; }
L_A9D5:;
    mff_cycles(0xA9D5, 11);
    a = RAM(OBJ_FLAGS + x);
    a &= 0xFB;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS + x) = a;
L_A9DD:;
    mff_cycles(0xA9DD, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A9DE:;
    mff_cycles(0xA9DE, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF2BE, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F2BE(); g_code_window_base = swb; } return;
}

/*  $A9E4 (bank 2) - lifted by port/tools/lift.py */
void func_A9E4_b2(void) {
    REGS_IN;
    mff_cycles(0xA9E4, 8);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    if (a != 0) { mff_cycles(0xA9E9, 2); goto L_AA18; }
    mff_cycles(0xA9EB, 5);
    a = RAM(SCROLL_X);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0xA9EF, 7);
        a = RAM(GAME_ACTION);
        m = 0x01;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xA9F5, 4);
            m = 0x14;
            c = a >= m;
            n = ((uint8_t)(a - m)) >> 7;
            z = ((uint8_t)(a - m)) == 0;
            if (!z) {
                mff_cycles(0xA9F9, 4);
                m = 0x16;
                c = a >= m;
                n = ((uint8_t)(a - m)) >> 7;
                z = ((uint8_t)(a - m)) == 0;
                if (!z) {
                    mff_cycles(0xA9FD, 4);
                    m = 0x17;
                    c = a >= m;
                    n = ((uint8_t)(a - m)) >> 7;
                    z = ((uint8_t)(a - m)) == 0;
                    if (!z) {
                        mff_cycles(0xAA01, 4);
                        m = 0x18;
                        c = a >= m;
                        n = ((uint8_t)(a - m)) >> 7;
                        z = ((uint8_t)(a - m)) == 0;
                        if (!z) {
                            mff_cycles(0xAA05, 4);
                            m = 0x10;
                            c = a >= m;
                            n = ((uint8_t)(a - m)) >> 7;
                            z = ((uint8_t)(a - m)) == 0;
                            if (!z) {
                                mff_cycles(0xAA09, 6);
                                SYNC_OUT;
                                if (!mff_call_fn(0x0A0B, func_F03F, 0xE000)) return;
                                SYNC_IN;
                                mff_cycles(0xAA0C, 17);
                                a = 0x0A;
                                RAM(GAME_ACTION) = a;
                                a = 0x00;
                                RAM(JUMP_MOVING) = a;
                                t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
                                RAM(OBJ_STATE + x) = t;
                                n = (t) >> 7;
                                z = (t) == 0;
                            } else {
                                mff_cycles(0xAA07, 1);
                            }
                        } else {
                            mff_cycles(0xAA03, 1);
                        }
                    } else {
                        mff_cycles(0xA9FF, 1);
                    }
                } else {
                    mff_cycles(0xA9FB, 2);
                }
            } else {
                mff_cycles(0xA9F7, 2);
            }
        } else {
            mff_cycles(0xA9F3, 2);
        }
    } else {
        mff_cycles(0xA9ED, 2);
    }
L_AA17:;
    mff_cycles(0xAA17, 6);
    SYNC_OUT;
    mff_rts(); return;
L_AA18:;
    mff_cycles(0xAA18, 11);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    m = prg_read((uint16_t)(0xAA7C + y));
    c = a >= m;
    mff_cycles(0xAA20, 2);
    if (a != m) {
        mff_cycles(0xAA22, 4);
        m = prg_read((uint16_t)(0xAA7F + y));
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        mff_cycles(0xAA25, 2);
        if (!z) {
            mff_cycles(0xAA27, 6);
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xAA25, 1);
        }
    } else {
        mff_cycles(0xAA20, 1);
    }
    mff_cycles(0xAA28, 8);
    a = RAM(OBJ_STATE + x);
    a &= 0x02;
    if (a != 0) { mff_cycles(0xAA2D, 1); goto L_AA4B; }
    mff_cycles(0xAA2F, 7);
    y = RAM(ROUND);
    if (y != 0x04) { mff_cycles(0xAA33, 1); goto L_AA3C; }
    mff_cycles(0xAA35, 7);
    y = RAM(SUB_AREA);
    a = prg_read((uint16_t)(0xAA64 + y));
    mff_cycles(0xAA3A, 2);
    if (a != 0) { mff_cycles(0xAA3A, 1); goto L_AA3F; }
L_AA3C:;
    mff_cycles(0xAA3C, 4);
    a = prg_read((uint16_t)(0xAA60 + y));
L_AA3F:;
    mff_cycles(0xAA3F, 11);
    c = 0;
    m = RAM(PLAYER_ID);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    y = a;
    a = prg_read((uint16_t)(0xAA67 + y));
    mff_cycles(0xAA46, 10);
    RAM(SPECIAL_SCREEN_ID) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
L_AA4B:;
    mff_cycles(0xAA4B, 5);
    a = RAM(SPECIAL_SCREEN_ID);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xAA17, 1); goto L_AA17; }
    mff_cycles(0xAA4F, 19);
    a = 0xFF;
    RAM(CAMERA_LOCK) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(JUMP_MOVING) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0A5B, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAA5C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0A5E, func_F2BE, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAA5F, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AA82 (bank 2) - lifted by port/tools/lift.py */
void func_AA82_b2(void) {
    REGS_IN;
    mff_cycles(0xAA82, 8);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    if (a == 0) {
        mff_cycles(0xAA89, 40);
        a = 0xC5;
        RAM(OBJ_TIMER + x) = a;
        RAM(OBJ_TIMER2 + x) = a;
        a = 0x20;
        RAM(OBJ_GROUND_Y + x) = a;
        t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_STATE + x) = t;
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(CAMERA_LOCK) = a;
        RAM(AREA_KILLS) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x0AA1, func_F436, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xAA87, 1);
    }
    mff_cycles(0xAAA2, 5);
    a = RAM(SCROLL_X);
    if (a != 0) {
        mff_cycles(0xAAA6, 12);
        a = 0x00;
        RAM(AREA_CLEAR) = a;
        a = RAM(SCROLL_X);
        m = 0x28;
        c = a >= m;
        if (c) {
            mff_cycles(0xAAB0, 21);
            t = (uint8_t)(RAM(SCREEN_EVENT) + 1);
            RAM(SCREEN_EVENT) = t;
            a = 0x00;
            RAM(SPAWN_GROUPS) = a;
            RAM(SPAWN_GROUPS + 1) = a;
            RAM(SPAWN_GROUPS + 2) = a;
            y = 0x0F;
            for (;;) {
                mff_cycles(0xAABF, 8);
                a = RAM(OBJ_ID + y);
                if (a != 0x61) {
                    mff_cycles(0xAAC6, 7);
                    a = 0x00;
                    RAM(OBJ_STATE + y) = a;
                } else {
                    mff_cycles(0xAAC4, 1);
                }
                mff_cycles(0xAACB, 6);
                y--;
                m = 0x02;
                c = y >= m;
                n = ((uint8_t)(y - m)) >> 7;
                z = ((uint8_t)(y - m)) == 0;
                if (z) break;
                mff_cycles(0xAABF, 1);
            }
            mff_cycles(0xAAD0, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xF2BE, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F2BE(); g_code_window_base = swb; } return;
        } else {
            mff_cycles(0xAAAE, 1);
        }
    } else {
        mff_cycles(0xAAA4, 1);
    }
    mff_cycles(0xAAD3, 6);
    a = RAM(OBJ_TIMER + x);
    if (a != 0) {
        mff_cycles(0xAAD8, 13);
        t = (uint8_t)(RAM(OBJ_TIMER + x) - 1);
        RAM(OBJ_TIMER + x) = t;
        a = RAM(OBJ_GROUND_Y + x);
        if (a == 0) {
            mff_cycles(0xAAE0, 8);
            a = 0x31;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0AE4, func_F34C, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAAE5, 7);
            a = 0x21;
            RAM(OBJ_GROUND_Y + x) = a;
        } else {
            mff_cycles(0xAADE, 1);
        }
        mff_cycles(0xAAEA, 13);
        t = (uint8_t)(RAM(OBJ_GROUND_Y + x) - 1);
        RAM(OBJ_GROUND_Y + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xAAD6, 1);
    }
    mff_cycles(0xAAEE, 6);
    a = RAM(OBJ_TIMER2 + x);
    if (a == 0) { mff_cycles(0xAAF1, 1); goto L_AAFA; }
    mff_cycles(0xAAF3, 11);
    t = (uint8_t)(RAM(OBJ_TIMER2 + x) - 1);
    RAM(OBJ_TIMER2 + x) = t;
    a = 0x6B;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xAAF8, 2); goto L_AB04; }
L_AAFA:;
    mff_cycles(0xAAFA, 14);
    a = 0xC5;
    RAM(OBJ_TIMER + x) = a;
    RAM(OBJ_TIMER2 + x) = a;
    a = 0x6D;
    n = (a) >> 7;
    z = (a) == 0;
L_AB04:;
    mff_cycles(0xAB04, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B06, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAB07, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AB08 (bank 2) - lifted by port/tools/lift.py */
void func_AB08_b2(void) {
    REGS_IN;
    mff_cycles(0xAB08, 8);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    if (a == 0) {
        mff_cycles(0xAB0F, 36);
        RAM(OBJ_VX_FRAC + x) = a;
        RAM(OBJ_VY_FRAC + x) = a;
        a = 0x01;
        RAM(OBJ_VX + x) = a;
        RAM(OBJ_VY + x) = a;
        t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_STATE + x) = t;
        a = 0x3C;
        RAM(OBJ_TIMER + x) = a;
    } else {
        mff_cycles(0xAB0D, 1);
    }
    mff_cycles(0xAB25, 6);
    a = RAM(OBJ_TIMER + x);
    if (a != 0) { mff_cycles(0xAB28, 1); goto L_AB37; }
    mff_cycles(0xAB2A, 8);
    a = 0xE5;
    m = RAM(OBJ_ANIM + x);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xAB2F, 1); goto L_AB3B; }
    mff_cycles(0xAB31, 8);
    y = 0x28;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0B35, func_F44F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAB36, 6);
    SYNC_OUT;
    mff_rts(); return;
L_AB37:;
    mff_cycles(0xAB37, 13);
    t = (uint8_t)(RAM(OBJ_TIMER + x) - 1);
    RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_AB3B:;
    mff_cycles(0xAB3B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B3D, func_F090, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAB3E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF183, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F183(); g_code_window_base = swb; } return;
}

/*  $AB41 (bank 2) - lifted by port/tools/lift.py */
void func_AB41_b2(void) {
    REGS_IN;
    mff_cycles(0xAB41, 8);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    if (a == 0) {
        mff_cycles(0xAB48, 32);
        RAM(OBJ_VX_FRAC + x) = a;
        a = 0x02;
        RAM(OBJ_VX + x) = a;
        t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_STATE + x) = t;
        a = 0x01;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_DIR + x) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x0B5A, func_F090, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAB5B, 10);
        a = 0xD2;
        y = 0x24;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0B61, func_F443, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xAB46, 1);
    }
    mff_cycles(0xAB62, 10);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xAB69, 1); goto L_AB93; }
    mff_cycles(0xAB6B, 4);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xAB6F, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0B71, func_F183, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAB72, 8);
        a = RAM(OBJ_X + x);
        m = 0x68;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0xAB79, 8);
            a = 0x9A;
            m = RAM(OBJ_ANIM + x);
            c = a >= m;
            n = ((uint8_t)(a - m)) >> 7;
            z = ((uint8_t)(a - m)) == 0;
            if (!z) {
                mff_cycles(0xAB80, 8);
                y = 0x3E;
                n = (y) >> 7;
                z = (y) == 0;
                SYNC_OUT;
                if (!mff_call_fn(0x0B84, func_F443, 0xE000)) return;
                SYNC_IN;
                mff_cycles(0xAB85, 22);
                a = 0x03;
                RAM(OBJ_VX + x) = a;
                t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
                RAM(OBJ_STATE + x) = t;
                a = 0x2A;
                n = (a) >> 7;
                z = (a) == 0;
                SYNC_OUT;
                if (!mff_call_fn(0x0B91, func_F34C, 0xE000)) return;
                SYNC_IN;
            } else {
                mff_cycles(0xAB7E, 1);
            }
        } else {
            mff_cycles(0xAB77, 1);
        }
    } else {
        mff_cycles(0xAB6D, 1);
    }
L_AB92:;
    mff_cycles(0xAB92, 6);
    SYNC_OUT;
    mff_rts(); return;
L_AB93:;
    mff_cycles(0xAB93, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B95, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAB96, 8);
    a = RAM(OBJ_X + x);
    m = 0x78;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xAB92, 1); goto L_AB92; }
    mff_cycles(0xAB9D, 15);
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
    a = 0xB7;
    m = RAM(OBJ_ANIM + x);
    c = a >= m;
    if (a != m) {
        mff_cycles(0xABA7, 8);
        y = 0x24;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0BAB, func_F443, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xABA5, 1);
    }
    mff_cycles(0xABAC, 18);
    a = 0x01;
    RAM(PLAYER_DIR) = a;
    a = 0xD0;
    RAM(OBJ_FLAGS) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE20A + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xABBA, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0BBC, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xABBD, 4);
    a = prg_read((uint16_t)(0xE252 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xABC0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0BC2, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xABC3, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $ABC4 (bank 2) - lifted by port/tools/lift.py */
void func_ABC4_b2(void) {
    REGS_IN;
    mff_cycles(0xABC4, 8);
    a = RAM(OBJ_LIFE + 12);
    m = 0x10;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0xABCB, 28);
        a = 0x80;
        RAM(BG_PAL_ANIM_ON + 2) = a;
        a = 0x07;
        RAM(BG_PAL_ANIM_SCRIPT + 2) = a;
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(BG_PAL_ANIM_STEP + 2) = a;
        RAM(BG_PAL_ANIM_TIMER + 2) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x0BDF, func_F2BE, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xABC9, 1);
    }
    mff_cycles(0xABE0, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $ABE1 (bank 2) - lifted by port/tools/lift.py */
void func_ABE1_b2(void) {
    REGS_IN;
    mff_cycles(0xABE1, 8);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    if (a == 0) {
        mff_cycles(0xABE8, 26);
        RAM(OBJ_LAND_Y + x) = a;
        a = 0x2D;
        RAM(OBJ_TIMER + x) = a;
        a = 0x87;
        RAM(OBJ_GROUND_Y + x) = a;
        t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_STATE + x) = t;
    } else {
        mff_cycles(0xABE6, 1);
    }
    mff_cycles(0xABF8, 8);
    a = RAM(OBJ_STATE + x);
    a &= 0x02;
    if (a == 0) {
        mff_cycles(0xABFF, 6);
        a = RAM(OBJ_TIMER + x);
        if (a == 0) {
            mff_cycles(0xAC04, 61);
            a = 0x80;
            RAM(BG_PAL_ANIM_ON + 2) = a;
            RAM(BG_PAL_ANIM_ON + 3) = a;
            a = 0x0C;
            RAM(BG_PAL_ANIM_SCRIPT + 2) = a;
            a = 0x0D;
            RAM(BG_PAL_ANIM_SCRIPT + 3) = a;
            a = 0x00;
            RAM(BG_PAL_ANIM_STEP + 2) = a;
            RAM(BG_PAL_ANIM_TIMER + 2) = a;
            RAM(BG_PAL_ANIM_STEP + 3) = a;
            RAM(BG_PAL_ANIM_TIMER + 3) = a;
            t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
            RAM(OBJ_STATE + x) = t;
            a = 0x01;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(TMP0) = a;
            RAM(TMP1) = x;
            SYNC_OUT;
            if (!mff_call_at(0x0C2F, 0xAC7D, 2)) return;
            SYNC_IN;
            mff_cycles(0xAC30, 9);
            x = RAM(TMP1);
            n = (x) >> 7;
            z = (x) == 0;
            SYNC_OUT;
            if (!mff_call_at(0x0C34, 0xACE1, 2)) return;
            SYNC_IN;
            mff_cycles(0xAC35, 6);
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xAC02, 1);
        }
        mff_cycles(0xAC36, 13);
        t = (uint8_t)(RAM(OBJ_TIMER + x) - 1);
        RAM(OBJ_TIMER + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xABFD, 2);
    }
    mff_cycles(0xAC3A, 7);
    a = RAM(GAME_ACTION);
    m = 0x0E;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xAC40, 8);
        a = RAM(OBJ_TIMER2 + x);
        a &= 0x01;
        if (a == 0) {
            mff_cycles(0xAC47, 13);
            a = RAM(SCROLL_Y);
            c = 1;
            m = 0x01;
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            c = w >= 0;
            a = (uint8_t)w;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(SCROLL_Y) = a;
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xAC58, 2); call_by_address_tail(0xAC58, 2); return;
        } else {
            mff_cycles(0xAC45, 1);
        }
        mff_cycles(0xAC51, 30);
        a = RAM(SCROLL_Y);
        c = 0;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(SCROLL_Y) = a;
        t = (uint8_t)(RAM(OBJ_TIMER2 + x) - 1);
        RAM(OBJ_TIMER2 + x) = t;
        t = (uint8_t)(RAM(OBJ_GROUND_Y + x) - 1);
        RAM(OBJ_GROUND_Y + x) = t;
        a = RAM(OBJ_GROUND_Y + x);
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0xAC63, 21);
            a = 0x87;
            RAM(OBJ_GROUND_Y + x) = a;
            a = 0x01;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(TMP0) = a;
            RAM(TMP1) = x;
            SYNC_OUT;
            if (!mff_call_at(0x0C70, 0xAC7D, 2)) return;
            SYNC_IN;
            mff_cycles(0xAC71, 3);
            x = RAM(TMP1);
            n = (x) >> 7;
            z = (x) == 0;
        } else {
            mff_cycles(0xAC61, 1);
        }
        mff_cycles(0xAC73, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xAC3E, 1);
    }
    mff_cycles(0xAC74, 16);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(BG_PAL_ANIM_ON + 2) = a;
    RAM(BG_PAL_ANIM_ON + 3) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AC58 (bank 2) - lifted by port/tools/lift.py */
void func_AC58_b2(void) {
    REGS_IN;
    mff_cycles(0xAC58, 20);
    t = (uint8_t)(RAM(OBJ_TIMER2 + x) - 1);
    RAM(OBJ_TIMER2 + x) = t;
    t = (uint8_t)(RAM(OBJ_GROUND_Y + x) - 1);
    RAM(OBJ_GROUND_Y + x) = t;
    a = RAM(OBJ_GROUND_Y + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0xAC63, 21);
        a = 0x87;
        RAM(OBJ_GROUND_Y + x) = a;
        a = 0x01;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(TMP0) = a;
        RAM(TMP1) = x;
        SYNC_OUT;
        if (!mff_call_at(0x0C70, 0xAC7D, 2)) return;
        SYNC_IN;
        mff_cycles(0xAC71, 3);
        x = RAM(TMP1);
        n = (x) >> 7;
        z = (x) == 0;
    } else {
        mff_cycles(0xAC61, 1);
    }
    mff_cycles(0xAC73, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AC7D (bank 2) - lifted by port/tools/lift.py */
void func_AC7D_b2(void) {
    REGS_IN;
    goto L_AC7D;
L_AC73:;
    mff_cycles(0xAC73, 6);
    SYNC_OUT;
    mff_rts(); return;
L_AC7D:;
    mff_cycles(0xAC7D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C7F, func_F71A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC80, 2);
    if (c) { mff_cycles(0xAC73, 1); goto L_AC73; }
    mff_cycles(0xAC82, 104);
    a = RAM(OBJ_X_HI);
    RAM(OBJ_X_HI + y) = a;
    a = RAM(OBJ_Y_HI);
    RAM(OBJ_Y_HI + y) = a;
    a = 0x00;
    RAM(OBJ_LIFE + y) = a;
    RAM(OBJ_LIFE_MAX + y) = a;
    RAM(OBJ_THROW_FLAG + y) = a;
    RAM(OBJ_HIT_COOLDOWN + y) = a;
    RAM(OBJ_INVULN + y) = a;
    RAM(OBJ_HIT_PLAYER + y) = a;
    RAM(OBJ_HIT_TIMER + y) = a;
    RAM(OBJ_HELD + y) = a;
    RAM(OBJ_ATTACK + y) = a;
    RAM(OBJ_ATTACK_DAMAGE + y) = a;
    RAM(OBJ_VY_FRAC + y) = a;
    a = 0x01;
    RAM(OBJ_VY + y) = a;
    a = 0xFF;
    RAM(OBJ_SPAWN_GROUP + y) = a;
    a = 0x63;
    RAM(OBJ_ID + y) = a;
    a = 0x79;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0CC4, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xACC5, 21);
    a = 0xD0;
    RAM(OBJ_FLAGS + y) = a;
    a = 0xC4;
    RAM(OBJ_Y + y) = a;
    x = RAM(TMP0);
    a = prg_read((uint16_t)(0xACDF + x));
    mff_cycles(0xACD4, 15);
    RAM(OBJ_X + y) = a;
    x--;
    RAM(TMP0) = x;
    a = RAM(TMP0);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0xAC7D, 1); goto L_AC7D; }
    mff_cycles(0xACDE, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $ACE1 (bank 2) - lifted by port/tools/lift.py */
void func_ACE1_b2(void) {
    REGS_IN;
    goto L_ACE1;
L_AC73:;
    mff_cycles(0xAC73, 6);
    SYNC_OUT;
    mff_rts(); return;
L_ACE1:;
    mff_cycles(0xACE1, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0CE3, func_F71A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xACE4, 2);
    if (c) { mff_cycles(0xAC73, 1); goto L_AC73; }
    mff_cycles(0xACE6, 102);
    a = RAM(OBJ_X_HI);
    RAM(OBJ_X_HI + y) = a;
    a = RAM(OBJ_Y_HI);
    RAM(OBJ_Y_HI + y) = a;
    a = 0x00;
    RAM(OBJ_LIFE + y) = a;
    RAM(OBJ_LIFE_MAX + y) = a;
    RAM(OBJ_THROW_FLAG + y) = a;
    RAM(OBJ_HIT_COOLDOWN + y) = a;
    RAM(OBJ_INVULN + y) = a;
    RAM(OBJ_HIT_PLAYER + y) = a;
    RAM(OBJ_HIT_TIMER + y) = a;
    RAM(OBJ_HELD + y) = a;
    RAM(OBJ_ATTACK + y) = a;
    RAM(OBJ_ATTACK_DAMAGE + y) = a;
    RAM(OBJ_VY_FRAC + y) = a;
    RAM(OBJ_VY + y) = a;
    a = 0xFF;
    RAM(OBJ_SPAWN_GROUP + y) = a;
    a = 0x62;
    RAM(OBJ_ID + y) = a;
    a = 0x78;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0D26, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAD27, 27);
    a = 0x90;
    RAM(OBJ_FLAGS + y) = a;
    a = 0xAC;
    RAM(OBJ_Y + y) = a;
    a = 0x81;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X + y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AD37 (bank 2) - lifted by port/tools/lift.py */
void func_AD37_b2(void) {
    REGS_IN;
    mff_cycles(0xAD37, 8);
    a = RAM(OBJ_Y + x);
    m = 0x2F;
    c = a >= m;
    if (c) {
        mff_cycles(0xAD3E, 17);
        a = 0x01;
        RAM(OBJ_ANIM_FRAME + x) = a;
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_ANIM_TIMER + x) = a;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF1DF, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F1DF(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xAD3C, 1);
    }
    mff_cycles(0xAD4B, 17);
    a = 0x02;
    RAM(OBJ_ANIM_FRAME + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ANIM_TIMER + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF2BE, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F2BE(); g_code_window_base = swb; } return;
}

/*  $AD58 (bank 2) - lifted by port/tools/lift.py */
void func_AD58_b2(void) {
    REGS_IN;
    mff_cycles(0xAD58, 8);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    if (a == 0) {
        mff_cycles(0xAD5F, 12);
        RAM(OBJ_TIMER + x) = a;
        t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_STATE + x) = t;
    } else {
        mff_cycles(0xAD5D, 1);
    }
    mff_cycles(0xAD65, 6);
    a = RAM(OBJ_TIMER + x);
    if (a == 0) {
        mff_cycles(0xAD6A, 8);
        a = 0x30;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0D6E, func_F34C, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAD6F, 7);
        t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        RAM(OBJ_TIMER + x) = t;
    } else {
        mff_cycles(0xAD68, 1);
    }
    mff_cycles(0xAD72, 7);
    a = RAM(AREA_KILLS);
    m = 0x05;
    c = a >= m;
    if (!c) {
        mff_cycles(0xAD78, 18);
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_ANIM_FRAME + x) = a;
        RAM(OBJ_ANIM_TIMER + x) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xAD76, 1);
    }
    mff_cycles(0xAD81, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x02;
    c = a >= m;
    if (!c) {
        mff_cycles(0xAD88, 8);
        a = 0x30;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0D8C, func_F34C, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAD8D, 7);
        t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        RAM(OBJ_TIMER + x) = t;
    } else {
        mff_cycles(0xAD86, 1);
    }
    mff_cycles(0xAD90, 7);
    a = RAM(AREA_KILLS);
    m = 0x0A;
    c = a >= m;
    if (c) { mff_cycles(0xAD94, 1); goto L_AD9D; }
    mff_cycles(0xAD96, 9);
    a = 0x01;
    RAM(OBJ_ANIM_FRAME + x) = a;
    if (a != 0) { mff_cycles(0xAD9B, 1); goto L_ADCD; }
L_AD9D:;
    mff_cycles(0xAD9D, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x03;
    c = a >= m;
    if (!c) {
        mff_cycles(0xADA4, 8);
        a = 0x30;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0DA8, func_F34C, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xADA9, 7);
        t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        RAM(OBJ_TIMER + x) = t;
    } else {
        mff_cycles(0xADA2, 1);
    }
    mff_cycles(0xADAC, 7);
    a = RAM(AREA_KILLS);
    m = 0x0F;
    c = a >= m;
    if (c) { mff_cycles(0xADB0, 1); goto L_ADB9; }
    mff_cycles(0xADB2, 9);
    a = 0x02;
    RAM(OBJ_ANIM_FRAME + x) = a;
    if (a != 0) { mff_cycles(0xADB7, 1); goto L_ADCD; }
L_ADB9:;
    mff_cycles(0xADB9, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x04;
    c = a >= m;
    if (!c) {
        mff_cycles(0xADC0, 15);
        t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        RAM(OBJ_TIMER + x) = t;
        a = 0x30;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0DC7, func_F34C, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xADBE, 1);
    }
    mff_cycles(0xADC8, 7);
    a = 0x03;
    RAM(OBJ_ANIM_FRAME + x) = a;
L_ADCD:;
    mff_cycles(0xADCD, 13);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ANIM_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}
