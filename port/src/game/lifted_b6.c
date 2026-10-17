/*
 * lifted_b6.c - bank 6 routines $8000-$BFFF, lifted by
 * port/tools/lift_bank.py from the pure scaffold: exact, still to be
 * tidied and moved into their systems' files.
 */
#include "mff.h"
#include "state.h"
#include "coroutine.h"

void func_ECCF(void);
void func_EDBA(void);
void func_F000(void);
void func_F051(void);
void func_F090(void);
void func_F099(void);
void func_F0A2(void);
void func_F0F7(void);
void func_F183(void);
void func_F1A1(void);
void func_F1FD(void);
void func_F288(void);
void func_F294(void);
void func_F2A0(void);
void func_F2BE(void);
void func_F314(void);
void func_F343(void);
void func_F34C(void);
void func_F365(void);
void func_F374(void);
void func_F440(void);
void func_F443(void);
void func_F449(void);
void func_F44F(void);
void func_F455(void);
void func_F55A(void);
void func_F71A(void);
void func_F738(void);
void func_F8D5(void);
void func_F9E5(void);
void func_FB09(void);
void func_FB3E(void);
void func_FB5B(void);
void func_FB72(void);
void func_FBD9(void);
void func_FF4D(void);

/*  $9CED (bank 6) - lifted by port/tools/lift.py */
void func_9CED_b6(void) {
    REGS_IN;
    mff_cycles(0x9CED, 4);
    a = 0x01;
    if (a == 0) {
        mff_cycles(0x9CF1, 4);
        a = 0x02;
        if (a == 0) {
            mff_cycles(0x9CF5, 2);
            a = 0x04;
        } else {
            mff_cycles(0x9CF3, 1);
        }
    } else {
        mff_cycles(0x9CEF, 1);
    }
    mff_cycles(0x9CF7, 11);
    RAM(OBJ_TIER + x) = a;
    a = RAM(OBJ_INVULN + x);
    if (a != 0) {
        mff_cycles(0x9CFF, 7);
        t = (uint8_t)(RAM(OBJ_INVULN + x) - 1);
        RAM(OBJ_INVULN + x) = t;
    } else {
        mff_cycles(0x9CFD, 2);
    }
    mff_cycles(0x9D02, 37);
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OAM_BUFFER + 15);
    m = RAM(RNG);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    m = RAM(RNG + 1);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    m = RAM(RNG + 2);
    w = a + m + c;
    a = (uint8_t)w;
    RAM(RNG) = a;
    a = RAM(OBJ_ID + x);
    c = 1;
    m = 0x20;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    y = a;
    a = prg_read((uint16_t)(0x9D5B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9D1C, 11);
    RAM(OBJ_KIND + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1D21, 0xAB29, 6)) return;
    SYNC_IN;
    mff_cycles(0x9D22, 2);
    if (c) {
        mff_cycles(0x9D24, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9D22, 1);
    }
    mff_cycles(0x9D25, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1D27, 0x9FF6, 6)) return;
    SYNC_IN;
    mff_cycles(0x9D28, 12);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    y = a;
    a = prg_read((uint16_t)(0x9D3B + y));
    mff_cycles(0x9D31, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0x9D4B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9D36, 8);
    RAM(TMP1) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $9CF1 (bank 6) - lifted by port/tools/lift.py */
void func_9CF1_b6(void) {
    REGS_IN;
    mff_cycles(0x9CF1, 4);
    a = 0x02;
    if (a == 0) {
        mff_cycles(0x9CF5, 2);
        a = 0x04;
    } else {
        mff_cycles(0x9CF3, 1);
    }
    mff_cycles(0x9CF7, 11);
    RAM(OBJ_TIER + x) = a;
    a = RAM(OBJ_INVULN + x);
    if (a != 0) {
        mff_cycles(0x9CFF, 7);
        t = (uint8_t)(RAM(OBJ_INVULN + x) - 1);
        RAM(OBJ_INVULN + x) = t;
    } else {
        mff_cycles(0x9CFD, 2);
    }
    mff_cycles(0x9D02, 37);
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OAM_BUFFER + 15);
    m = RAM(RNG);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    m = RAM(RNG + 1);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    m = RAM(RNG + 2);
    w = a + m + c;
    a = (uint8_t)w;
    RAM(RNG) = a;
    a = RAM(OBJ_ID + x);
    c = 1;
    m = 0x20;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    y = a;
    a = prg_read((uint16_t)(0x9D5B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9D1C, 11);
    RAM(OBJ_KIND + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1D21, 0xAB29, 6)) return;
    SYNC_IN;
    mff_cycles(0x9D22, 2);
    if (c) {
        mff_cycles(0x9D24, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9D22, 1);
    }
    mff_cycles(0x9D25, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1D27, 0x9FF6, 6)) return;
    SYNC_IN;
    mff_cycles(0x9D28, 12);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    y = a;
    a = prg_read((uint16_t)(0x9D3B + y));
    mff_cycles(0x9D31, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0x9D4B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9D36, 8);
    RAM(TMP1) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $9CF5 (bank 6) - lifted by port/tools/lift.py */
void func_9CF5_b6(void) {
    REGS_IN;
    mff_cycles(0x9CF5, 13);
    a = 0x04;
    RAM(OBJ_TIER + x) = a;
    a = RAM(OBJ_INVULN + x);
    if (a != 0) {
        mff_cycles(0x9CFF, 7);
        t = (uint8_t)(RAM(OBJ_INVULN + x) - 1);
        RAM(OBJ_INVULN + x) = t;
    } else {
        mff_cycles(0x9CFD, 2);
    }
    mff_cycles(0x9D02, 37);
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OAM_BUFFER + 15);
    m = RAM(RNG);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    m = RAM(RNG + 1);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    m = RAM(RNG + 2);
    w = a + m + c;
    a = (uint8_t)w;
    RAM(RNG) = a;
    a = RAM(OBJ_ID + x);
    c = 1;
    m = 0x20;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    y = a;
    a = prg_read((uint16_t)(0x9D5B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9D1C, 11);
    RAM(OBJ_KIND + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1D21, 0xAB29, 6)) return;
    SYNC_IN;
    mff_cycles(0x9D22, 2);
    if (c) {
        mff_cycles(0x9D24, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9D22, 1);
    }
    mff_cycles(0x9D25, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1D27, 0x9FF6, 6)) return;
    SYNC_IN;
    mff_cycles(0x9D28, 12);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    y = a;
    a = prg_read((uint16_t)(0x9D3B + y));
    mff_cycles(0x9D31, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0x9D4B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9D36, 8);
    RAM(TMP1) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $9D71 (bank 6) - lifted by port/tools/lift.py */
void func_9D71_b6(void) {
    REGS_IN;
    mff_cycles(0x9D71, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    if (a != 0) {
        mff_cycles(0x9D78, 13);
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
            mff_cycles(0x9D82, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xF2BE, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F2BE(); g_code_window_base = swb; } return;
        } else {
            mff_cycles(0x9D80, 1);
        }
        mff_cycles(0x9D85, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1D87, func_F090, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9D88, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF183, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F183(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0x9D76, 1);
    }
    mff_cycles(0x9D8B, 13);
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
        mff_cycles(0x9D95, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF2BE, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F2BE(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0x9D93, 1);
    }
    mff_cycles(0x9D98, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1D9A, func_F099, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9D9B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF1A1, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F1A1(); g_code_window_base = swb; } return;
}

/*  $9D9E (bank 6) - lifted by port/tools/lift.py */
void func_9D9E_b6(void) {
    REGS_IN;
    mff_cycles(0x9D9E, 6);
    y = 0x25;
    a = 0x01;
    if (a == 0) {
        mff_cycles(0x9DA4, 6);
        y = 0x2B;
        a = 0x02;
        if (a == 0) {
            mff_cycles(0x9DAA, 4);
            y = 0x35;
            a = 0x04;
        } else {
            mff_cycles(0x9DA8, 1);
        }
    } else {
        mff_cycles(0x9DA2, 1);
    }
    mff_cycles(0x9DAE, 25);
    RAM(OBJ_TIER + x) = a;
    a = y;
    RAM(OBJ_TIMER2 + x) = a;
    a = 0x05;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_KIND + x) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x1DBC, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9DBD, 4);
    m = 0x80;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x9DC1, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9DBF, 1);
    }
    mff_cycles(0x9DC2, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1DC4, func_FB5B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9DC5, 6);
    y = a;
    a = prg_read((uint16_t)(0xA87E + y));
    mff_cycles(0x9DC9, 9);
    RAM(OBJ_DIR + x) = a;
    a &= 0x0C;
    if (a != 0) { mff_cycles(0x9DCE, 1); goto L_9DE4; }
    mff_cycles(0x9DD0, 28);
    a = 0x4C;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0x02;
    RAM(OBJ_VX + x) = a;
    a = 0x00;
    RAM(OBJ_VY_FRAC + x) = a;
    RAM(OBJ_VY + x) = a;
    if (a == 0) { mff_cycles(0x9DE2, 1); goto L_9DF8; }
L_9DE4:;
    mff_cycles(0x9DE4, 28);
    a = 0x1F;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0x02;
    RAM(OBJ_VX + x) = a;
    a = 0xE1;
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x00;
    RAM(OBJ_VY + x) = a;
L_9DF8:;
    mff_cycles(0x9DF8, 10);
    a = 0xD2;
    y = 0x24;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1DFE, func_F443, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9DFF, 10);
    a = RAM(OBJ_TIER + x);
    y = a;
    a = prg_read((uint16_t)(0xA897 + y));
    mff_cycles(0x9E06, 43);
    c = 0;
    m = 0x33;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = 0x8E;
    RAM(OBJ_STATE + x) = a;
    a = 0x03;
    RAM(OBJ_TIMER + x) = a;
    a = RAM(OBJ_TIMER2 + x);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ID + x) = a;
    RAM(OBJ_SAVED_ID + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9DA4 (bank 6) - lifted by port/tools/lift.py */
void func_9DA4_b6(void) {
    REGS_IN;
    mff_cycles(0x9DA4, 6);
    y = 0x2B;
    a = 0x02;
    if (a == 0) {
        mff_cycles(0x9DAA, 4);
        y = 0x35;
        a = 0x04;
    } else {
        mff_cycles(0x9DA8, 1);
    }
    mff_cycles(0x9DAE, 25);
    RAM(OBJ_TIER + x) = a;
    a = y;
    RAM(OBJ_TIMER2 + x) = a;
    a = 0x05;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_KIND + x) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x1DBC, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9DBD, 4);
    m = 0x80;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x9DC1, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9DBF, 1);
    }
    mff_cycles(0x9DC2, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1DC4, func_FB5B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9DC5, 6);
    y = a;
    a = prg_read((uint16_t)(0xA87E + y));
    mff_cycles(0x9DC9, 9);
    RAM(OBJ_DIR + x) = a;
    a &= 0x0C;
    if (a != 0) { mff_cycles(0x9DCE, 1); goto L_9DE4; }
    mff_cycles(0x9DD0, 28);
    a = 0x4C;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0x02;
    RAM(OBJ_VX + x) = a;
    a = 0x00;
    RAM(OBJ_VY_FRAC + x) = a;
    RAM(OBJ_VY + x) = a;
    if (a == 0) { mff_cycles(0x9DE2, 1); goto L_9DF8; }
L_9DE4:;
    mff_cycles(0x9DE4, 28);
    a = 0x1F;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0x02;
    RAM(OBJ_VX + x) = a;
    a = 0xE1;
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x00;
    RAM(OBJ_VY + x) = a;
L_9DF8:;
    mff_cycles(0x9DF8, 10);
    a = 0xD2;
    y = 0x24;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1DFE, func_F443, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9DFF, 10);
    a = RAM(OBJ_TIER + x);
    y = a;
    a = prg_read((uint16_t)(0xA897 + y));
    mff_cycles(0x9E06, 43);
    c = 0;
    m = 0x33;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = 0x8E;
    RAM(OBJ_STATE + x) = a;
    a = 0x03;
    RAM(OBJ_TIMER + x) = a;
    a = RAM(OBJ_TIMER2 + x);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ID + x) = a;
    RAM(OBJ_SAVED_ID + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9DAA (bank 6) - lifted by port/tools/lift.py */
void func_9DAA_b6(void) {
    REGS_IN;
    mff_cycles(0x9DAA, 29);
    y = 0x35;
    a = 0x04;
    RAM(OBJ_TIER + x) = a;
    a = y;
    RAM(OBJ_TIMER2 + x) = a;
    a = 0x05;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_KIND + x) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x1DBC, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9DBD, 4);
    m = 0x80;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x9DC1, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9DBF, 1);
    }
    mff_cycles(0x9DC2, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1DC4, func_FB5B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9DC5, 6);
    y = a;
    a = prg_read((uint16_t)(0xA87E + y));
    mff_cycles(0x9DC9, 9);
    RAM(OBJ_DIR + x) = a;
    a &= 0x0C;
    if (a != 0) { mff_cycles(0x9DCE, 1); goto L_9DE4; }
    mff_cycles(0x9DD0, 28);
    a = 0x4C;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0x02;
    RAM(OBJ_VX + x) = a;
    a = 0x00;
    RAM(OBJ_VY_FRAC + x) = a;
    RAM(OBJ_VY + x) = a;
    if (a == 0) { mff_cycles(0x9DE2, 1); goto L_9DF8; }
L_9DE4:;
    mff_cycles(0x9DE4, 28);
    a = 0x1F;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0x02;
    RAM(OBJ_VX + x) = a;
    a = 0xE1;
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x00;
    RAM(OBJ_VY + x) = a;
L_9DF8:;
    mff_cycles(0x9DF8, 10);
    a = 0xD2;
    y = 0x24;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1DFE, func_F443, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9DFF, 10);
    a = RAM(OBJ_TIER + x);
    y = a;
    a = prg_read((uint16_t)(0xA897 + y));
    mff_cycles(0x9E06, 43);
    c = 0;
    m = 0x33;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = 0x8E;
    RAM(OBJ_STATE + x) = a;
    a = 0x03;
    RAM(OBJ_TIMER + x) = a;
    a = RAM(OBJ_TIMER2 + x);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ID + x) = a;
    RAM(OBJ_SAVED_ID + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9E20 (bank 6) - lifted by port/tools/lift.py */
void func_9E20_b6(void) {
    REGS_IN;
    mff_cycles(0x9E20, 4);
    a = 0x01;
    if (a == 0) {
        mff_cycles(0x9E24, 4);
        a = 0x02;
        if (a == 0) {
            mff_cycles(0x9E28, 2);
            a = 0x04;
        } else {
            mff_cycles(0x9E26, 1);
        }
    } else {
        mff_cycles(0x9E22, 1);
    }
    mff_cycles(0x9E2A, 4);
    y = 0x00;
    if (y != 0) {
        mff_cycles(0x9E2E, 4);
        a = 0x01;
        if (a == 0) {
            mff_cycles(0x9E32, 4);
            a = 0x02;
            if (a == 0) {
                mff_cycles(0x9E36, 2);
                a = 0x04;
            } else {
                mff_cycles(0x9E34, 1);
            }
        } else {
            mff_cycles(0x9E30, 1);
        }
        mff_cycles(0x9E38, 4);
        y = 0x04;
        if (y == 0) {
            mff_cycles(0x9E3C, 4);
            a = 0x01;
            if (a == 0) {
                mff_cycles(0x9E40, 4);
                a = 0x02;
                if (a == 0) {
                    mff_cycles(0x9E44, 2);
                    a = 0x04;
                } else {
                    mff_cycles(0x9E42, 1);
                }
            } else {
                mff_cycles(0x9E3E, 1);
            }
            mff_cycles(0x9E46, 2);
            y = 0x03;
        } else {
            mff_cycles(0x9E3A, 1);
        }
    } else {
        mff_cycles(0x9E2C, 1);
    }
    mff_cycles(0x9E48, 19);
    RAM(OBJ_TIER + x) = a;
    a = y;
    RAM(OBJ_KIND + x) = a;
    a = RAM(ROUND);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x9E53, 1); goto L_9E5B; }
    mff_cycles(0x9E55, 7);
    a = RAM(SUB_AREA);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9E59, 1); goto L_9E63; }
L_9E5B:;
    mff_cycles(0x9E5B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1E5D, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9E5E, 4);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x9E62, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9E60, 1);
    }
L_9E63:;
    mff_cycles(0x9E63, 53);
    a = RAM(OBJ_FLAGS + x);
    a &= 0xFB;
    RAM(OBJ_FLAGS + x) = a;
    a = RAM(PLAYER_GROUND_Y);
    RAM(OBJ_GROUND_Y + x) = a;
    a = 0xFF;
    RAM(OBJ_VY + x) = a;
    a = 0x00;
    RAM(OBJ_VY_FRAC + x) = a;
    RAM(OBJ_VX_FRAC + x) = a;
    RAM(OBJ_VX + x) = a;
    a = RAM(OBJ_TIER + x);
    y = a;
    a = prg_read((uint16_t)(0xA897 + y));
    mff_cycles(0x9E87, 10);
    c = 0;
    y = RAM(OBJ_KIND + x);
    m = prg_read((uint16_t)(0x9EBF + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0x9E8E, 9);
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = prg_read((uint16_t)(0x9EB3 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9E94, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1E96, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9E97, 4);
    a = y;
    if (a == 0) {
        mff_cycles(0x9E9A, 7);
        a = 0x06;
        RAM(OBJ_ANIM_FRAME + x) = a;
    } else {
        mff_cycles(0x9E98, 1);
    }
    mff_cycles(0x9E9F, 4);
    a = prg_read((uint16_t)(0x9EB9 + y));
    mff_cycles(0x9EA2, 7);
    mff_push(a);
    a = prg_read((uint16_t)(0xFC33 + y));
    mff_cycles(0x9EA6, 21);
    y = a;
    a = mff_pull();
    RAM(CHR_BANKS + 3 + y) = a;
    a = 0x8F;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9FBB, 2); call_by_address_tail(0x9FBB, 6); return;
}

/*  $9E24 (bank 6) - lifted by port/tools/lift.py */
void func_9E24_b6(void) {
    REGS_IN;
    mff_cycles(0x9E24, 4);
    a = 0x02;
    if (a == 0) {
        mff_cycles(0x9E28, 2);
        a = 0x04;
    } else {
        mff_cycles(0x9E26, 1);
    }
    mff_cycles(0x9E2A, 4);
    y = 0x00;
    if (y != 0) {
        mff_cycles(0x9E2E, 4);
        a = 0x01;
        if (a == 0) {
            mff_cycles(0x9E32, 4);
            a = 0x02;
            if (a == 0) {
                mff_cycles(0x9E36, 2);
                a = 0x04;
            } else {
                mff_cycles(0x9E34, 1);
            }
        } else {
            mff_cycles(0x9E30, 1);
        }
        mff_cycles(0x9E38, 4);
        y = 0x04;
        if (y == 0) {
            mff_cycles(0x9E3C, 4);
            a = 0x01;
            if (a == 0) {
                mff_cycles(0x9E40, 4);
                a = 0x02;
                if (a == 0) {
                    mff_cycles(0x9E44, 2);
                    a = 0x04;
                } else {
                    mff_cycles(0x9E42, 1);
                }
            } else {
                mff_cycles(0x9E3E, 1);
            }
            mff_cycles(0x9E46, 2);
            y = 0x03;
        } else {
            mff_cycles(0x9E3A, 1);
        }
    } else {
        mff_cycles(0x9E2C, 1);
    }
    mff_cycles(0x9E48, 19);
    RAM(OBJ_TIER + x) = a;
    a = y;
    RAM(OBJ_KIND + x) = a;
    a = RAM(ROUND);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x9E53, 1); goto L_9E5B; }
    mff_cycles(0x9E55, 7);
    a = RAM(SUB_AREA);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9E59, 1); goto L_9E63; }
L_9E5B:;
    mff_cycles(0x9E5B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1E5D, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9E5E, 4);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x9E62, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9E60, 1);
    }
L_9E63:;
    mff_cycles(0x9E63, 53);
    a = RAM(OBJ_FLAGS + x);
    a &= 0xFB;
    RAM(OBJ_FLAGS + x) = a;
    a = RAM(PLAYER_GROUND_Y);
    RAM(OBJ_GROUND_Y + x) = a;
    a = 0xFF;
    RAM(OBJ_VY + x) = a;
    a = 0x00;
    RAM(OBJ_VY_FRAC + x) = a;
    RAM(OBJ_VX_FRAC + x) = a;
    RAM(OBJ_VX + x) = a;
    a = RAM(OBJ_TIER + x);
    y = a;
    a = prg_read((uint16_t)(0xA897 + y));
    mff_cycles(0x9E87, 10);
    c = 0;
    y = RAM(OBJ_KIND + x);
    m = prg_read((uint16_t)(0x9EBF + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0x9E8E, 9);
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = prg_read((uint16_t)(0x9EB3 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9E94, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1E96, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9E97, 4);
    a = y;
    if (a == 0) {
        mff_cycles(0x9E9A, 7);
        a = 0x06;
        RAM(OBJ_ANIM_FRAME + x) = a;
    } else {
        mff_cycles(0x9E98, 1);
    }
    mff_cycles(0x9E9F, 4);
    a = prg_read((uint16_t)(0x9EB9 + y));
    mff_cycles(0x9EA2, 7);
    mff_push(a);
    a = prg_read((uint16_t)(0xFC33 + y));
    mff_cycles(0x9EA6, 21);
    y = a;
    a = mff_pull();
    RAM(CHR_BANKS + 3 + y) = a;
    a = 0x8F;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9FBB, 2); call_by_address_tail(0x9FBB, 6); return;
}

/*  $9E28 (bank 6) - lifted by port/tools/lift.py */
void func_9E28_b6(void) {
    REGS_IN;
    mff_cycles(0x9E28, 6);
    a = 0x04;
    y = 0x00;
    if (y != 0) {
        mff_cycles(0x9E2E, 4);
        a = 0x01;
        if (a == 0) {
            mff_cycles(0x9E32, 4);
            a = 0x02;
            if (a == 0) {
                mff_cycles(0x9E36, 2);
                a = 0x04;
            } else {
                mff_cycles(0x9E34, 1);
            }
        } else {
            mff_cycles(0x9E30, 1);
        }
        mff_cycles(0x9E38, 4);
        y = 0x04;
        if (y == 0) {
            mff_cycles(0x9E3C, 4);
            a = 0x01;
            if (a == 0) {
                mff_cycles(0x9E40, 4);
                a = 0x02;
                if (a == 0) {
                    mff_cycles(0x9E44, 2);
                    a = 0x04;
                } else {
                    mff_cycles(0x9E42, 1);
                }
            } else {
                mff_cycles(0x9E3E, 1);
            }
            mff_cycles(0x9E46, 2);
            y = 0x03;
        } else {
            mff_cycles(0x9E3A, 1);
        }
    } else {
        mff_cycles(0x9E2C, 1);
    }
    mff_cycles(0x9E48, 19);
    RAM(OBJ_TIER + x) = a;
    a = y;
    RAM(OBJ_KIND + x) = a;
    a = RAM(ROUND);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x9E53, 1); goto L_9E5B; }
    mff_cycles(0x9E55, 7);
    a = RAM(SUB_AREA);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9E59, 1); goto L_9E63; }
L_9E5B:;
    mff_cycles(0x9E5B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1E5D, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9E5E, 4);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x9E62, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9E60, 1);
    }
L_9E63:;
    mff_cycles(0x9E63, 53);
    a = RAM(OBJ_FLAGS + x);
    a &= 0xFB;
    RAM(OBJ_FLAGS + x) = a;
    a = RAM(PLAYER_GROUND_Y);
    RAM(OBJ_GROUND_Y + x) = a;
    a = 0xFF;
    RAM(OBJ_VY + x) = a;
    a = 0x00;
    RAM(OBJ_VY_FRAC + x) = a;
    RAM(OBJ_VX_FRAC + x) = a;
    RAM(OBJ_VX + x) = a;
    a = RAM(OBJ_TIER + x);
    y = a;
    a = prg_read((uint16_t)(0xA897 + y));
    mff_cycles(0x9E87, 10);
    c = 0;
    y = RAM(OBJ_KIND + x);
    m = prg_read((uint16_t)(0x9EBF + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0x9E8E, 9);
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = prg_read((uint16_t)(0x9EB3 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9E94, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1E96, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9E97, 4);
    a = y;
    if (a == 0) {
        mff_cycles(0x9E9A, 7);
        a = 0x06;
        RAM(OBJ_ANIM_FRAME + x) = a;
    } else {
        mff_cycles(0x9E98, 1);
    }
    mff_cycles(0x9E9F, 4);
    a = prg_read((uint16_t)(0x9EB9 + y));
    mff_cycles(0x9EA2, 7);
    mff_push(a);
    a = prg_read((uint16_t)(0xFC33 + y));
    mff_cycles(0x9EA6, 21);
    y = a;
    a = mff_pull();
    RAM(CHR_BANKS + 3 + y) = a;
    a = 0x8F;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9FBB, 2); call_by_address_tail(0x9FBB, 6); return;
}

/*  $9E32 (bank 6) - lifted by port/tools/lift.py */
void func_9E32_b6(void) {
    REGS_IN;
    mff_cycles(0x9E32, 4);
    a = 0x02;
    if (a == 0) {
        mff_cycles(0x9E36, 2);
        a = 0x04;
    } else {
        mff_cycles(0x9E34, 1);
    }
    mff_cycles(0x9E38, 4);
    y = 0x04;
    if (y == 0) {
        mff_cycles(0x9E3C, 4);
        a = 0x01;
        if (a == 0) {
            mff_cycles(0x9E40, 4);
            a = 0x02;
            if (a == 0) {
                mff_cycles(0x9E44, 2);
                a = 0x04;
            } else {
                mff_cycles(0x9E42, 1);
            }
        } else {
            mff_cycles(0x9E3E, 1);
        }
        mff_cycles(0x9E46, 2);
        y = 0x03;
    } else {
        mff_cycles(0x9E3A, 1);
    }
    mff_cycles(0x9E48, 19);
    RAM(OBJ_TIER + x) = a;
    a = y;
    RAM(OBJ_KIND + x) = a;
    a = RAM(ROUND);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x9E53, 1); goto L_9E5B; }
    mff_cycles(0x9E55, 7);
    a = RAM(SUB_AREA);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9E59, 1); goto L_9E63; }
L_9E5B:;
    mff_cycles(0x9E5B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1E5D, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9E5E, 4);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x9E62, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9E60, 1);
    }
L_9E63:;
    mff_cycles(0x9E63, 53);
    a = RAM(OBJ_FLAGS + x);
    a &= 0xFB;
    RAM(OBJ_FLAGS + x) = a;
    a = RAM(PLAYER_GROUND_Y);
    RAM(OBJ_GROUND_Y + x) = a;
    a = 0xFF;
    RAM(OBJ_VY + x) = a;
    a = 0x00;
    RAM(OBJ_VY_FRAC + x) = a;
    RAM(OBJ_VX_FRAC + x) = a;
    RAM(OBJ_VX + x) = a;
    a = RAM(OBJ_TIER + x);
    y = a;
    a = prg_read((uint16_t)(0xA897 + y));
    mff_cycles(0x9E87, 10);
    c = 0;
    y = RAM(OBJ_KIND + x);
    m = prg_read((uint16_t)(0x9EBF + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0x9E8E, 9);
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = prg_read((uint16_t)(0x9EB3 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9E94, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1E96, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9E97, 4);
    a = y;
    if (a == 0) {
        mff_cycles(0x9E9A, 7);
        a = 0x06;
        RAM(OBJ_ANIM_FRAME + x) = a;
    } else {
        mff_cycles(0x9E98, 1);
    }
    mff_cycles(0x9E9F, 4);
    a = prg_read((uint16_t)(0x9EB9 + y));
    mff_cycles(0x9EA2, 7);
    mff_push(a);
    a = prg_read((uint16_t)(0xFC33 + y));
    mff_cycles(0x9EA6, 21);
    y = a;
    a = mff_pull();
    RAM(CHR_BANKS + 3 + y) = a;
    a = 0x8F;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9FBB, 2); call_by_address_tail(0x9FBB, 6); return;
}

/*  $9E36 (bank 6) - lifted by port/tools/lift.py */
void func_9E36_b6(void) {
    REGS_IN;
    mff_cycles(0x9E36, 6);
    a = 0x04;
    y = 0x04;
    if (y == 0) {
        mff_cycles(0x9E3C, 4);
        a = 0x01;
        if (a == 0) {
            mff_cycles(0x9E40, 4);
            a = 0x02;
            if (a == 0) {
                mff_cycles(0x9E44, 2);
                a = 0x04;
            } else {
                mff_cycles(0x9E42, 1);
            }
        } else {
            mff_cycles(0x9E3E, 1);
        }
        mff_cycles(0x9E46, 2);
        y = 0x03;
    } else {
        mff_cycles(0x9E3A, 1);
    }
    mff_cycles(0x9E48, 19);
    RAM(OBJ_TIER + x) = a;
    a = y;
    RAM(OBJ_KIND + x) = a;
    a = RAM(ROUND);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x9E53, 1); goto L_9E5B; }
    mff_cycles(0x9E55, 7);
    a = RAM(SUB_AREA);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9E59, 1); goto L_9E63; }
L_9E5B:;
    mff_cycles(0x9E5B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1E5D, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9E5E, 4);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x9E62, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9E60, 1);
    }
L_9E63:;
    mff_cycles(0x9E63, 53);
    a = RAM(OBJ_FLAGS + x);
    a &= 0xFB;
    RAM(OBJ_FLAGS + x) = a;
    a = RAM(PLAYER_GROUND_Y);
    RAM(OBJ_GROUND_Y + x) = a;
    a = 0xFF;
    RAM(OBJ_VY + x) = a;
    a = 0x00;
    RAM(OBJ_VY_FRAC + x) = a;
    RAM(OBJ_VX_FRAC + x) = a;
    RAM(OBJ_VX + x) = a;
    a = RAM(OBJ_TIER + x);
    y = a;
    a = prg_read((uint16_t)(0xA897 + y));
    mff_cycles(0x9E87, 10);
    c = 0;
    y = RAM(OBJ_KIND + x);
    m = prg_read((uint16_t)(0x9EBF + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0x9E8E, 9);
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = prg_read((uint16_t)(0x9EB3 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9E94, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1E96, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9E97, 4);
    a = y;
    if (a == 0) {
        mff_cycles(0x9E9A, 7);
        a = 0x06;
        RAM(OBJ_ANIM_FRAME + x) = a;
    } else {
        mff_cycles(0x9E98, 1);
    }
    mff_cycles(0x9E9F, 4);
    a = prg_read((uint16_t)(0x9EB9 + y));
    mff_cycles(0x9EA2, 7);
    mff_push(a);
    a = prg_read((uint16_t)(0xFC33 + y));
    mff_cycles(0x9EA6, 21);
    y = a;
    a = mff_pull();
    RAM(CHR_BANKS + 3 + y) = a;
    a = 0x8F;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9FBB, 2); call_by_address_tail(0x9FBB, 6); return;
}

/*  $9E40 (bank 6) - lifted by port/tools/lift.py */
void func_9E40_b6(void) {
    REGS_IN;
    mff_cycles(0x9E40, 4);
    a = 0x02;
    if (a == 0) {
        mff_cycles(0x9E44, 2);
        a = 0x04;
    } else {
        mff_cycles(0x9E42, 1);
    }
    mff_cycles(0x9E46, 21);
    y = 0x03;
    RAM(OBJ_TIER + x) = a;
    a = y;
    RAM(OBJ_KIND + x) = a;
    a = RAM(ROUND);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x9E53, 1); goto L_9E5B; }
    mff_cycles(0x9E55, 7);
    a = RAM(SUB_AREA);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9E59, 1); goto L_9E63; }
L_9E5B:;
    mff_cycles(0x9E5B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1E5D, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9E5E, 4);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x9E62, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9E60, 1);
    }
L_9E63:;
    mff_cycles(0x9E63, 53);
    a = RAM(OBJ_FLAGS + x);
    a &= 0xFB;
    RAM(OBJ_FLAGS + x) = a;
    a = RAM(PLAYER_GROUND_Y);
    RAM(OBJ_GROUND_Y + x) = a;
    a = 0xFF;
    RAM(OBJ_VY + x) = a;
    a = 0x00;
    RAM(OBJ_VY_FRAC + x) = a;
    RAM(OBJ_VX_FRAC + x) = a;
    RAM(OBJ_VX + x) = a;
    a = RAM(OBJ_TIER + x);
    y = a;
    a = prg_read((uint16_t)(0xA897 + y));
    mff_cycles(0x9E87, 10);
    c = 0;
    y = RAM(OBJ_KIND + x);
    m = prg_read((uint16_t)(0x9EBF + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0x9E8E, 9);
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = prg_read((uint16_t)(0x9EB3 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9E94, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1E96, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9E97, 4);
    a = y;
    if (a == 0) {
        mff_cycles(0x9E9A, 7);
        a = 0x06;
        RAM(OBJ_ANIM_FRAME + x) = a;
    } else {
        mff_cycles(0x9E98, 1);
    }
    mff_cycles(0x9E9F, 4);
    a = prg_read((uint16_t)(0x9EB9 + y));
    mff_cycles(0x9EA2, 7);
    mff_push(a);
    a = prg_read((uint16_t)(0xFC33 + y));
    mff_cycles(0x9EA6, 21);
    y = a;
    a = mff_pull();
    RAM(CHR_BANKS + 3 + y) = a;
    a = 0x8F;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9FBB, 2); call_by_address_tail(0x9FBB, 6); return;
}

/*  $9E44 (bank 6) - lifted by port/tools/lift.py */
void func_9E44_b6(void) {
    REGS_IN;
    mff_cycles(0x9E44, 23);
    a = 0x04;
    y = 0x03;
    RAM(OBJ_TIER + x) = a;
    a = y;
    RAM(OBJ_KIND + x) = a;
    a = RAM(ROUND);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x9E53, 1); goto L_9E5B; }
    mff_cycles(0x9E55, 7);
    a = RAM(SUB_AREA);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9E59, 1); goto L_9E63; }
L_9E5B:;
    mff_cycles(0x9E5B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1E5D, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9E5E, 4);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x9E62, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9E60, 1);
    }
L_9E63:;
    mff_cycles(0x9E63, 53);
    a = RAM(OBJ_FLAGS + x);
    a &= 0xFB;
    RAM(OBJ_FLAGS + x) = a;
    a = RAM(PLAYER_GROUND_Y);
    RAM(OBJ_GROUND_Y + x) = a;
    a = 0xFF;
    RAM(OBJ_VY + x) = a;
    a = 0x00;
    RAM(OBJ_VY_FRAC + x) = a;
    RAM(OBJ_VX_FRAC + x) = a;
    RAM(OBJ_VX + x) = a;
    a = RAM(OBJ_TIER + x);
    y = a;
    a = prg_read((uint16_t)(0xA897 + y));
    mff_cycles(0x9E87, 10);
    c = 0;
    y = RAM(OBJ_KIND + x);
    m = prg_read((uint16_t)(0x9EBF + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0x9E8E, 9);
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = prg_read((uint16_t)(0x9EB3 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9E94, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1E96, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9E97, 4);
    a = y;
    if (a == 0) {
        mff_cycles(0x9E9A, 7);
        a = 0x06;
        RAM(OBJ_ANIM_FRAME + x) = a;
    } else {
        mff_cycles(0x9E98, 1);
    }
    mff_cycles(0x9E9F, 4);
    a = prg_read((uint16_t)(0x9EB9 + y));
    mff_cycles(0x9EA2, 7);
    mff_push(a);
    a = prg_read((uint16_t)(0xFC33 + y));
    mff_cycles(0x9EA6, 21);
    y = a;
    a = mff_pull();
    RAM(CHR_BANKS + 3 + y) = a;
    a = 0x8F;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9FBB, 2); call_by_address_tail(0x9FBB, 6); return;
}

/*  $9EC5 (bank 6) - lifted by port/tools/lift.py */
void func_9EC5_b6(void) {
    REGS_IN;
    mff_cycles(0x9EC5, 4);
    a = 0x01;
    if (a == 0) {
        mff_cycles(0x9EC9, 4);
        a = 0x02;
        if (a == 0) {
            mff_cycles(0x9ECD, 2);
            a = 0x04;
        } else {
            mff_cycles(0x9ECB, 1);
        }
    } else {
        mff_cycles(0x9EC7, 1);
    }
    mff_cycles(0x9ECF, 26);
    RAM(OBJ_TIER + x) = a;
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_ID + x);
    c = 1;
    m = 0x36;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    y = a;
    a = prg_read((uint16_t)(0x9F91 + y));
    mff_cycles(0x9EE1, 25);
    RAM(OBJ_KIND + x) = a;
    a = 0x78;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x8D;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1EF0, 0xAAAE, 6)) return;
    SYNC_IN;
    mff_cycles(0x9EF1, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9FBB, 2); call_by_address_tail(0x9FBB, 6); return;
}

/*  $9ECD (bank 6) - lifted by port/tools/lift.py */
void func_9ECD_b6(void) {
    REGS_IN;
    mff_cycles(0x9ECD, 28);
    a = 0x04;
    RAM(OBJ_TIER + x) = a;
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_ID + x);
    c = 1;
    m = 0x36;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    y = a;
    a = prg_read((uint16_t)(0x9F91 + y));
    mff_cycles(0x9EE1, 25);
    RAM(OBJ_KIND + x) = a;
    a = 0x78;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x8D;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1EF0, 0xAAAE, 6)) return;
    SYNC_IN;
    mff_cycles(0x9EF1, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9FBB, 2); call_by_address_tail(0x9FBB, 6); return;
}

/*  $9EF4 (bank 6) - lifted by port/tools/lift.py */
void func_9EF4_b6(void) {
    REGS_IN;
    mff_cycles(0x9EF4, 4);
    a = 0x00;
    if (a != 0) {
        mff_cycles(0x9EF8, 2);
        a = 0x02;
    } else {
        mff_cycles(0x9EF6, 1);
    }
    mff_cycles(0x9EFA, 9);
    RAM(OBJ_TIMER3 + x) = a;
    a = 0x01;
    if (a == 0) {
        mff_cycles(0x9F01, 4);
        a = 0x00;
        if (a != 0) {
            mff_cycles(0x9F05, 2);
            a = 0x02;
        } else {
            mff_cycles(0x9F03, 1);
        }
        mff_cycles(0x9F07, 9);
        RAM(OBJ_TIMER3 + x) = a;
        a = 0x02;
        if (a == 0) {
            mff_cycles(0x9F0E, 4);
            a = 0x00;
            if (a != 0) {
                mff_cycles(0x9F12, 2);
                a = 0x02;
            } else {
                mff_cycles(0x9F10, 1);
            }
            mff_cycles(0x9F14, 7);
            RAM(OBJ_TIMER3 + x) = a;
            a = 0x04;
        } else {
            mff_cycles(0x9F0C, 1);
        }
    } else {
        mff_cycles(0x9EFF, 1);
    }
    mff_cycles(0x9F19, 19);
    RAM(OBJ_TIER + x) = a;
    a = RAM(OBJ_ID + x);
    c = 1;
    m = 0x36;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    y = a;
    a = prg_read((uint16_t)(0x9F91 + y));
    mff_cycles(0x9F26, 20);
    RAM(OBJ_KIND + x) = a;
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_STATE + x);
    t = a;
    c = t & 1;
    t >>= 1;
    if (!c) {
        mff_cycles(0x9F34, 44);
        t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_STATE + x) = t;
        y = RAM(OBJ_KIND + x);
        y--;
        a = y;
        m = RAM(OBJ_TIMER3 + x);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        y = a;
        RAM(OBJ_ANIM_FRAME + x) = a;
        a = 0xA2;
        RAM(OBJ_ANIM + x) = a;
        a = 0x00;
        RAM(OBJ_ANIM_TIMER + x) = a;
        a = prg_read((uint16_t)(0x9F8D + y));
        mff_cycles(0x9F50, 11);
        mff_push(a);
        y = RAM(OBJ_KIND + x);
        a = prg_read((uint16_t)(0xFC33 + y));
        mff_cycles(0x9F57, 17);
        y = a;
        a = mff_pull();
        n = (a) >> 7;
        z = (a) == 0;
        RAM(CHR_BANKS + 3 + y) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9F32, 1);
    }
    mff_cycles(0x9F5D, 17);
    a = 0x00;
    RAM(OBJ_ANIM_TIMER + x) = a;
    a = 0x32;
    y = 0x32;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1F68, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9F69, 2);
    if (c) { mff_cycles(0x9F69, 1); goto L_9F7F; }
    mff_cycles(0x9F6B, 27);
    a = 0xFF;
    RAM(OBJ_INVULN + x) = a;
    a = RAM(OBJ_X_HI + x);
    c = 1;
    m = RAM(SCROLL_X_HI);
    w = a - m - !c;
    c = w >= 0;
    a = RAM(OBJ_X + x);
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    m = 0x40;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x9F7D, 1); goto L_9F8C; }
L_9F7F:;
    mff_cycles(0x9F7F, 17);
    a = 0x00;
    RAM(OBJ_INVULN + x) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9FBB, 2); call_by_address_tail(0x9FBB, 6); return;
L_9F8C:;
    mff_cycles(0x9F8C, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9EF8 (bank 6) - lifted by port/tools/lift.py */
void func_9EF8_b6(void) {
    REGS_IN;
    mff_cycles(0x9EF8, 11);
    a = 0x02;
    RAM(OBJ_TIMER3 + x) = a;
    a = 0x01;
    if (a == 0) {
        mff_cycles(0x9F01, 4);
        a = 0x00;
        if (a != 0) {
            mff_cycles(0x9F05, 2);
            a = 0x02;
        } else {
            mff_cycles(0x9F03, 1);
        }
        mff_cycles(0x9F07, 9);
        RAM(OBJ_TIMER3 + x) = a;
        a = 0x02;
        if (a == 0) {
            mff_cycles(0x9F0E, 4);
            a = 0x00;
            if (a != 0) {
                mff_cycles(0x9F12, 2);
                a = 0x02;
            } else {
                mff_cycles(0x9F10, 1);
            }
            mff_cycles(0x9F14, 7);
            RAM(OBJ_TIMER3 + x) = a;
            a = 0x04;
        } else {
            mff_cycles(0x9F0C, 1);
        }
    } else {
        mff_cycles(0x9EFF, 1);
    }
    mff_cycles(0x9F19, 19);
    RAM(OBJ_TIER + x) = a;
    a = RAM(OBJ_ID + x);
    c = 1;
    m = 0x36;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    y = a;
    a = prg_read((uint16_t)(0x9F91 + y));
    mff_cycles(0x9F26, 20);
    RAM(OBJ_KIND + x) = a;
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_STATE + x);
    t = a;
    c = t & 1;
    t >>= 1;
    if (!c) {
        mff_cycles(0x9F34, 44);
        t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_STATE + x) = t;
        y = RAM(OBJ_KIND + x);
        y--;
        a = y;
        m = RAM(OBJ_TIMER3 + x);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        y = a;
        RAM(OBJ_ANIM_FRAME + x) = a;
        a = 0xA2;
        RAM(OBJ_ANIM + x) = a;
        a = 0x00;
        RAM(OBJ_ANIM_TIMER + x) = a;
        a = prg_read((uint16_t)(0x9F8D + y));
        mff_cycles(0x9F50, 11);
        mff_push(a);
        y = RAM(OBJ_KIND + x);
        a = prg_read((uint16_t)(0xFC33 + y));
        mff_cycles(0x9F57, 17);
        y = a;
        a = mff_pull();
        n = (a) >> 7;
        z = (a) == 0;
        RAM(CHR_BANKS + 3 + y) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9F32, 1);
    }
    mff_cycles(0x9F5D, 17);
    a = 0x00;
    RAM(OBJ_ANIM_TIMER + x) = a;
    a = 0x32;
    y = 0x32;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1F68, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9F69, 2);
    if (c) { mff_cycles(0x9F69, 1); goto L_9F7F; }
    mff_cycles(0x9F6B, 27);
    a = 0xFF;
    RAM(OBJ_INVULN + x) = a;
    a = RAM(OBJ_X_HI + x);
    c = 1;
    m = RAM(SCROLL_X_HI);
    w = a - m - !c;
    c = w >= 0;
    a = RAM(OBJ_X + x);
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    m = 0x40;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x9F7D, 1); goto L_9F8C; }
L_9F7F:;
    mff_cycles(0x9F7F, 17);
    a = 0x00;
    RAM(OBJ_INVULN + x) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9FBB, 2); call_by_address_tail(0x9FBB, 6); return;
L_9F8C:;
    mff_cycles(0x9F8C, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9F05 (bank 6) - lifted by port/tools/lift.py */
void func_9F05_b6(void) {
    REGS_IN;
    mff_cycles(0x9F05, 11);
    a = 0x02;
    RAM(OBJ_TIMER3 + x) = a;
    a = 0x02;
    if (a == 0) {
        mff_cycles(0x9F0E, 4);
        a = 0x00;
        if (a != 0) {
            mff_cycles(0x9F12, 2);
            a = 0x02;
        } else {
            mff_cycles(0x9F10, 1);
        }
        mff_cycles(0x9F14, 7);
        RAM(OBJ_TIMER3 + x) = a;
        a = 0x04;
    } else {
        mff_cycles(0x9F0C, 1);
    }
    mff_cycles(0x9F19, 19);
    RAM(OBJ_TIER + x) = a;
    a = RAM(OBJ_ID + x);
    c = 1;
    m = 0x36;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    y = a;
    a = prg_read((uint16_t)(0x9F91 + y));
    mff_cycles(0x9F26, 20);
    RAM(OBJ_KIND + x) = a;
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_STATE + x);
    t = a;
    c = t & 1;
    t >>= 1;
    if (!c) {
        mff_cycles(0x9F34, 44);
        t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_STATE + x) = t;
        y = RAM(OBJ_KIND + x);
        y--;
        a = y;
        m = RAM(OBJ_TIMER3 + x);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        y = a;
        RAM(OBJ_ANIM_FRAME + x) = a;
        a = 0xA2;
        RAM(OBJ_ANIM + x) = a;
        a = 0x00;
        RAM(OBJ_ANIM_TIMER + x) = a;
        a = prg_read((uint16_t)(0x9F8D + y));
        mff_cycles(0x9F50, 11);
        mff_push(a);
        y = RAM(OBJ_KIND + x);
        a = prg_read((uint16_t)(0xFC33 + y));
        mff_cycles(0x9F57, 17);
        y = a;
        a = mff_pull();
        n = (a) >> 7;
        z = (a) == 0;
        RAM(CHR_BANKS + 3 + y) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9F32, 1);
    }
    mff_cycles(0x9F5D, 17);
    a = 0x00;
    RAM(OBJ_ANIM_TIMER + x) = a;
    a = 0x32;
    y = 0x32;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1F68, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9F69, 2);
    if (c) { mff_cycles(0x9F69, 1); goto L_9F7F; }
    mff_cycles(0x9F6B, 27);
    a = 0xFF;
    RAM(OBJ_INVULN + x) = a;
    a = RAM(OBJ_X_HI + x);
    c = 1;
    m = RAM(SCROLL_X_HI);
    w = a - m - !c;
    c = w >= 0;
    a = RAM(OBJ_X + x);
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    m = 0x40;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x9F7D, 1); goto L_9F8C; }
L_9F7F:;
    mff_cycles(0x9F7F, 17);
    a = 0x00;
    RAM(OBJ_INVULN + x) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9FBB, 2); call_by_address_tail(0x9FBB, 6); return;
L_9F8C:;
    mff_cycles(0x9F8C, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9F0E (bank 6) - lifted by port/tools/lift.py */
void func_9F0E_b6(void) {
    REGS_IN;
    mff_cycles(0x9F0E, 4);
    a = 0x00;
    if (a != 0) {
        mff_cycles(0x9F12, 2);
        a = 0x02;
    } else {
        mff_cycles(0x9F10, 1);
    }
    mff_cycles(0x9F14, 26);
    RAM(OBJ_TIMER3 + x) = a;
    a = 0x04;
    RAM(OBJ_TIER + x) = a;
    a = RAM(OBJ_ID + x);
    c = 1;
    m = 0x36;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    y = a;
    a = prg_read((uint16_t)(0x9F91 + y));
    mff_cycles(0x9F26, 20);
    RAM(OBJ_KIND + x) = a;
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_STATE + x);
    t = a;
    c = t & 1;
    t >>= 1;
    if (!c) {
        mff_cycles(0x9F34, 44);
        t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_STATE + x) = t;
        y = RAM(OBJ_KIND + x);
        y--;
        a = y;
        m = RAM(OBJ_TIMER3 + x);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        y = a;
        RAM(OBJ_ANIM_FRAME + x) = a;
        a = 0xA2;
        RAM(OBJ_ANIM + x) = a;
        a = 0x00;
        RAM(OBJ_ANIM_TIMER + x) = a;
        a = prg_read((uint16_t)(0x9F8D + y));
        mff_cycles(0x9F50, 11);
        mff_push(a);
        y = RAM(OBJ_KIND + x);
        a = prg_read((uint16_t)(0xFC33 + y));
        mff_cycles(0x9F57, 17);
        y = a;
        a = mff_pull();
        n = (a) >> 7;
        z = (a) == 0;
        RAM(CHR_BANKS + 3 + y) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9F32, 1);
    }
    mff_cycles(0x9F5D, 17);
    a = 0x00;
    RAM(OBJ_ANIM_TIMER + x) = a;
    a = 0x32;
    y = 0x32;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1F68, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9F69, 2);
    if (c) { mff_cycles(0x9F69, 1); goto L_9F7F; }
    mff_cycles(0x9F6B, 27);
    a = 0xFF;
    RAM(OBJ_INVULN + x) = a;
    a = RAM(OBJ_X_HI + x);
    c = 1;
    m = RAM(SCROLL_X_HI);
    w = a - m - !c;
    c = w >= 0;
    a = RAM(OBJ_X + x);
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    m = 0x40;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x9F7D, 1); goto L_9F8C; }
L_9F7F:;
    mff_cycles(0x9F7F, 17);
    a = 0x00;
    RAM(OBJ_INVULN + x) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9FBB, 2); call_by_address_tail(0x9FBB, 6); return;
L_9F8C:;
    mff_cycles(0x9F8C, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9F12 (bank 6) - lifted by port/tools/lift.py */
void func_9F12_b6(void) {
    REGS_IN;
    mff_cycles(0x9F12, 28);
    a = 0x02;
    RAM(OBJ_TIMER3 + x) = a;
    a = 0x04;
    RAM(OBJ_TIER + x) = a;
    a = RAM(OBJ_ID + x);
    c = 1;
    m = 0x36;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    y = a;
    a = prg_read((uint16_t)(0x9F91 + y));
    mff_cycles(0x9F26, 20);
    RAM(OBJ_KIND + x) = a;
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_STATE + x);
    t = a;
    c = t & 1;
    t >>= 1;
    if (!c) {
        mff_cycles(0x9F34, 44);
        t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_STATE + x) = t;
        y = RAM(OBJ_KIND + x);
        y--;
        a = y;
        m = RAM(OBJ_TIMER3 + x);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        y = a;
        RAM(OBJ_ANIM_FRAME + x) = a;
        a = 0xA2;
        RAM(OBJ_ANIM + x) = a;
        a = 0x00;
        RAM(OBJ_ANIM_TIMER + x) = a;
        a = prg_read((uint16_t)(0x9F8D + y));
        mff_cycles(0x9F50, 11);
        mff_push(a);
        y = RAM(OBJ_KIND + x);
        a = prg_read((uint16_t)(0xFC33 + y));
        mff_cycles(0x9F57, 17);
        y = a;
        a = mff_pull();
        n = (a) >> 7;
        z = (a) == 0;
        RAM(CHR_BANKS + 3 + y) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9F32, 1);
    }
    mff_cycles(0x9F5D, 17);
    a = 0x00;
    RAM(OBJ_ANIM_TIMER + x) = a;
    a = 0x32;
    y = 0x32;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1F68, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9F69, 2);
    if (c) { mff_cycles(0x9F69, 1); goto L_9F7F; }
    mff_cycles(0x9F6B, 27);
    a = 0xFF;
    RAM(OBJ_INVULN + x) = a;
    a = RAM(OBJ_X_HI + x);
    c = 1;
    m = RAM(SCROLL_X_HI);
    w = a - m - !c;
    c = w >= 0;
    a = RAM(OBJ_X + x);
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    m = 0x40;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x9F7D, 1); goto L_9F8C; }
L_9F7F:;
    mff_cycles(0x9F7F, 17);
    a = 0x00;
    RAM(OBJ_INVULN + x) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9FBB, 2); call_by_address_tail(0x9FBB, 6); return;
L_9F8C:;
    mff_cycles(0x9F8C, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9FBB (bank 6) - lifted by port/tools/lift.py */
void func_9FBB_b6(void) {
    REGS_IN;
    mff_cycles(0x9FBB, 14);
    a = RAM(OBJ_ID + x);
    c = 1;
    m = 0x36;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    y = a;
    a = prg_read((uint16_t)(0x9FCC + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9FC5, 16);
    RAM(OBJ_ID + x) = a;
    RAM(OBJ_SAVED_ID + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9FF6 (bank 6) - lifted by port/tools/lift.py */
void func_9FF6_b6(void) {
    REGS_IN;
    mff_cycles(0x9FF6, 8);
    a = RAM(OBJ_STATE + x);
    m = 0x89;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9FFB, 2); goto L_A045; }
    mff_cycles(0x9FFD, 4);
    m = 0x8F;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9FFF, 1); goto L_A045; }
    mff_cycles(0xA001, 5);
    a = RAM(ROUND);
    if (a != 0) { mff_cycles(0xA003, 1); goto L_A00B; }
    mff_cycles(0xA005, 7);
    a = RAM(SUB_AREA);
    if (a == 0x01) { mff_cycles(0xA009, 1); goto L_A017; }
L_A00B:;
    mff_cycles(0xA00B, 7);
    a = RAM(ROUND);
    if (a != 0x03) { mff_cycles(0xA00F, 1); goto L_A01B; }
    mff_cycles(0xA011, 7);
    a = RAM(SUB_AREA);
    if (a != 0x01) { mff_cycles(0xA015, 1); goto L_A01B; }
L_A017:;
    mff_cycles(0xA017, 4);
    y = 0x01;
    if (y != 0) { mff_cycles(0xA019, 1); goto L_A02D; }
L_A01B:;
    mff_cycles(0xA01B, 7);
    a = RAM(ROUND);
    if (a != 0x01) { mff_cycles(0xA01F, 1); goto L_A02B; }
    mff_cycles(0xA021, 7);
    a = RAM(SUB_AREA);
    if (a != 0x01) { mff_cycles(0xA025, 1); goto L_A02B; }
    mff_cycles(0xA027, 4);
    y = 0x02;
    if (y != 0) { mff_cycles(0xA029, 1); goto L_A02D; }
L_A02B:;
    mff_cycles(0xA02B, 2);
    y = 0x00;
L_A02D:;
    mff_cycles(0xA02D, 4);
    a = prg_read((uint16_t)(0xA046 + y));
    mff_cycles(0xA030, 6);
    m = RAM(OBJ_Y + x);
    c = a >= m;
    if (c) {
        mff_cycles(0xA035, 7);
        m = 0x00;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(OBJ_Y + x) = a;
    } else {
        mff_cycles(0xA033, 1);
    }
    mff_cycles(0xA03A, 4);
    a = prg_read((uint16_t)(0xA049 + y));
    mff_cycles(0xA03D, 6);
    m = RAM(OBJ_Y + x);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0xA042, 5);
        RAM(OBJ_Y + x) = a;
    } else {
        mff_cycles(0xA040, 1);
    }
L_A045:;
    mff_cycles(0xA045, 6);
    SYNC_OUT;
    mff_rts(); return;
}
