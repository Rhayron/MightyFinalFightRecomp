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

/*  $9733 (bank 6) - lifted by port/tools/lift.py */
void func_9733_b6(void) {
    REGS_IN;
    mff_cycles(0x9733, 8);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    if (a != 0) { mff_cycles(0x9738, 1); goto L_978A; }
    mff_cycles(0x973A, 24);
    RAM(OBJ_TIMER + x) = a;
    RAM(OBJ_VX + x) = a;
    a = 0x99;
    RAM(OBJ_VX_FRAC + x) = a;
    a = RAM(ATTACK_ID);
    m = 0x07;
    c = a >= m;
    if (a != m) { mff_cycles(0x9749, 1); goto L_975C; }
    mff_cycles(0x974B, 23);
    a = 0xCC;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0x98;
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_VY + x) = a;
    if (!z) { mff_cycles(0x975A, 1); goto L_976A; }
L_975C:;
    mff_cycles(0x975C, 7);
    y = RAM(PLAYER_LEVEL);
    a = prg_read((uint16_t)(0x9A33 + y));
    mff_cycles(0x9761, 9);
    RAM(OBJ_VY_FRAC + x) = a;
    a = prg_read((uint16_t)(0x9A39 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9767, 5);
    RAM(OBJ_VY + x) = a;
L_976A:;
    mff_cycles(0x976A, 6);
    SYNC_OUT;
    if (!mff_call_at(0x176C, 0x981B, 6)) return;
    SYNC_IN;
    mff_cycles(0x976D, 13);
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x1772, 0x9C55, 6)) return;
    SYNC_IN;
    mff_cycles(0x9773, 13);
    a = 0x10;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER2 + x) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x177A, func_F314, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x977B, 9);
    a = RAM(PLAYER_DIR);
    if (a != RAM(OBJ_DIR + x)) {
        mff_cycles(0x9782, 6);
        a = RAM(OBJ_DIR + x);
        a ^= 0x03;
    } else {
        mff_cycles(0x9780, 1);
    }
    mff_cycles(0x9787, 5);
    RAM(OBJ_DIR + x) = a;
L_978A:;
    mff_cycles(0x978A, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x9791, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x9818, 2); call_by_address_tail(0x9818, 6); return;
    } else {
        mff_cycles(0x978F, 1);
    }
    mff_cycles(0x9794, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    if (a != 0) {
        mff_cycles(0x979B, 5);
        a = RAM(KEEP_ON_SCREEN);
        n = (a) >> 7;
        z = (a) == 0;
        if (!z) {
            mff_cycles(0x979F, 13);
            a = RAM(OBJ_X + x);
            c = 1;
            m = RAM(SCROLL_X);
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            m = 0xF1;
            c = a >= m;
            n = ((uint8_t)(a - m)) >> 7;
            z = ((uint8_t)(a - m)) == 0;
            if (c) {
                mff_cycles(0x97A9, 25);
                a = RAM(SCROLL_X);
                c = 0;
                m = 0xF0;
                w = a + m + c;
                c = w > 0xFF;
                a = (uint8_t)w;
                RAM(OBJ_X + x) = a;
                a = RAM(AREA_SCREEN);
                m = 0x00;
                w = a + m + c;
                v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
                c = w > 0xFF;
                a = (uint8_t)w;
                n = (a) >> 7;
                z = (a) == 0;
                RAM(OBJ_X_HI + x) = a;
                SYNC_OUT;
                nes_cpu_instruction_boundary(0x97E4, 2); call_by_address_tail(0x97E4, 6); return;
            } else {
                mff_cycles(0x97A7, 1);
            }
        } else {
            mff_cycles(0x979D, 1);
        }
        mff_cycles(0x97BB, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x17BD, func_F183, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x97BE, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x97E4, 2); call_by_address_tail(0x97E4, 6); return;
    } else {
        mff_cycles(0x9799, 1);
    }
    mff_cycles(0x97C1, 5);
    a = RAM(KEEP_ON_SCREEN);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0x97C5, 13);
        a = RAM(OBJ_X + x);
        c = 1;
        m = RAM(SCROLL_X);
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        m = 0x10;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!c) {
            mff_cycles(0x97CF, 25);
            a = RAM(SCROLL_X);
            c = 0;
            m = 0x10;
            w = a + m + c;
            c = w > 0xFF;
            a = (uint8_t)w;
            RAM(OBJ_X + x) = a;
            a = RAM(AREA_SCREEN);
            m = 0x00;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_X_HI + x) = a;
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x97E4, 2); call_by_address_tail(0x97E4, 6); return;
        } else {
            mff_cycles(0x97CD, 1);
        }
    } else {
        mff_cycles(0x97C3, 1);
    }
    mff_cycles(0x97E1, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x17E3, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x97E4, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x17E6, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x97E7, 6);
    a = RAM(OBJ_VY + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0x97EC, 10);
        a = RAM(OBJ_Y + x);
        m = RAM(OBJ_LAND_Y + x);
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0x97F4, 15);
            a = RAM(OBJ_LAND_Y + x);
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_Y + x) = a;
            SYNC_OUT;
            if (!mff_call_at(0x17FC, 0x9B1F, 6)) return;
            SYNC_IN;
            mff_cycles(0x97FD, 8);
            a = 0x20;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x1801, func_F34C, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x9802, 34);
            a = 0x38;
            RAM(OBJ_VY_FRAC + x) = a;
            a = 0x02;
            RAM(OBJ_VY + x) = a;
            a = 0x80;
            RAM(OBJ_VX_FRAC + x) = a;
            t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
            RAM(OBJ_TIMER + x) = t;
            n = (t) >> 7;
            z = (t) == 0;
            SYNC_OUT;
            if (!mff_call_at(0x1816, 0x9C7A, 6)) return;
            SYNC_IN;
        } else {
            mff_cycles(0x97F2, 2);
        }
    } else {
        mff_cycles(0x97EA, 2);
    }
    mff_cycles(0x9817, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $97E4 (bank 6) - lifted by port/tools/lift.py */
void func_97E4_b6(void) {
    REGS_IN;
    mff_cycles(0x97E4, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x17E6, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x97E7, 6);
    a = RAM(OBJ_VY + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0x97EC, 10);
        a = RAM(OBJ_Y + x);
        m = RAM(OBJ_LAND_Y + x);
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0x97F4, 15);
            a = RAM(OBJ_LAND_Y + x);
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_Y + x) = a;
            SYNC_OUT;
            if (!mff_call_at(0x17FC, 0x9B1F, 6)) return;
            SYNC_IN;
            mff_cycles(0x97FD, 8);
            a = 0x20;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x1801, func_F34C, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x9802, 34);
            a = 0x38;
            RAM(OBJ_VY_FRAC + x) = a;
            a = 0x02;
            RAM(OBJ_VY + x) = a;
            a = 0x80;
            RAM(OBJ_VX_FRAC + x) = a;
            t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
            RAM(OBJ_TIMER + x) = t;
            n = (t) >> 7;
            z = (t) == 0;
            SYNC_OUT;
            if (!mff_call_at(0x1816, 0x9C7A, 6)) return;
            SYNC_IN;
        } else {
            mff_cycles(0x97F2, 2);
        }
    } else {
        mff_cycles(0x97EA, 2);
    }
    mff_cycles(0x9817, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9818 (bank 6) - lifted by port/tools/lift.py */
void func_9818_b6(void) {
    REGS_IN;
    mff_cycles(0x9818, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9BFC, 2); call_by_address_tail(0x9BFC, 6); return;
}

/*  $981B (bank 6) - lifted by port/tools/lift.py */
void func_981B_b6(void) {
    REGS_IN;
    mff_cycles(0x981B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x181D, func_F314, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x981E, 9);
    a = RAM(PLAYER_DIR);
    m = RAM(OBJ_DIR + x);
    c = a >= m;
    if (a != m) {
        mff_cycles(0x9825, 17);
        a = RAM(OBJ_DIR + x);
        a ^= 0x03;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_DIR + x) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9823, 1);
    }
    mff_cycles(0x982E, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0x9835, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF099, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F099(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0x9833, 1);
    }
    mff_cycles(0x9838, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF090, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F090(); g_code_window_base = swb; } return;
}

/*  $983B (bank 6) - lifted by port/tools/lift.py */
void func_983B_b6(void) {
    REGS_IN;
    mff_cycles(0x983B, 8);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    if (a == 0) {
        mff_cycles(0x9842, 32);
        RAM(OBJ_TIMER + x) = a;
        RAM(OBJ_VX + x) = a;
        a = 0x99;
        RAM(OBJ_VX_FRAC + x) = a;
        a = 0x10;
        RAM(OBJ_TIMER2 + x) = a;
        a = RAM(OBJ_ID + x);
        m = 0x03;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) {
            mff_cycles(0x9859, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x986E, 2); call_by_address_tail(0x986E, 6); return;
        } else {
            mff_cycles(0x9857, 1);
        }
        mff_cycles(0x985C, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x185E, func_F314, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x985F, 9);
        a = RAM(PLAYER_DIR);
        m = RAM(OBJ_DIR + x);
        c = a >= m;
        if (a == m) {
            mff_cycles(0x9866, 6);
            a = RAM(OBJ_DIR + x);
            a ^= 0x03;
        } else {
            mff_cycles(0x9864, 1);
        }
        mff_cycles(0x986B, 18);
        RAM(OBJ_DIR + x) = a;
        t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_STATE + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x1873, 0x9C55, 6)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x9840, 1);
    }
    mff_cycles(0x9874, 8);
    a = RAM(OBJ_ID + x);
    if (a != 0x03) { mff_cycles(0x9879, 1); goto L_9884; }
    mff_cycles(0x987B, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9880, 1); goto L_98B8; }
    mff_cycles(0x9882, 2);
    if (!z) { mff_cycles(0x9882, 1); goto L_988B; }
L_9884:;
    mff_cycles(0x9884, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9889, 1); goto L_98B8; }
L_988B:;
    mff_cycles(0x988B, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    if (a == 0) { mff_cycles(0x9890, 1); goto L_98BB; }
    mff_cycles(0x9892, 5);
    a = RAM(KEEP_ON_SCREEN);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0x9896, 13);
        a = RAM(OBJ_X + x);
        c = 1;
        m = RAM(SCROLL_X);
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        m = 0xF1;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0x98A0, 25);
            a = RAM(SCROLL_X);
            c = 0;
            m = 0xF0;
            w = a + m + c;
            c = w > 0xFF;
            a = (uint8_t)w;
            RAM(OBJ_X + x) = a;
            a = RAM(AREA_SCREEN);
            m = 0x00;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_X_HI + x) = a;
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x98DE, 2); call_by_address_tail(0x98DE, 6); return;
        } else {
            mff_cycles(0x989E, 1);
        }
    } else {
        mff_cycles(0x9894, 1);
    }
    mff_cycles(0x98B2, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x18B4, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x98B5, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x98DE, 2); call_by_address_tail(0x98DE, 6); return;
L_98B8:;
    mff_cycles(0x98B8, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9912, 2); call_by_address_tail(0x9912, 6); return;
L_98BB:;
    mff_cycles(0x98BB, 5);
    a = RAM(KEEP_ON_SCREEN);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0x98BF, 13);
        a = RAM(OBJ_X + x);
        c = 1;
        m = RAM(SCROLL_X);
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        m = 0x10;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!c) {
            mff_cycles(0x98C9, 25);
            a = RAM(SCROLL_X);
            c = 0;
            m = 0x10;
            w = a + m + c;
            c = w > 0xFF;
            a = (uint8_t)w;
            RAM(OBJ_X + x) = a;
            a = RAM(AREA_SCREEN);
            m = 0x00;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_X_HI + x) = a;
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x98DE, 2); call_by_address_tail(0x98DE, 6); return;
        } else {
            mff_cycles(0x98C7, 1);
        }
    } else {
        mff_cycles(0x98BD, 1);
    }
    mff_cycles(0x98DB, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x18DD, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x98DE, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x18E0, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x98E1, 6);
    a = RAM(OBJ_VY + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0x98E6, 10);
        a = RAM(OBJ_Y + x);
        m = RAM(OBJ_LAND_Y + x);
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0x98EE, 15);
            a = RAM(OBJ_LAND_Y + x);
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_Y + x) = a;
            SYNC_OUT;
            if (!mff_call_at(0x18F6, 0x9B1F, 6)) return;
            SYNC_IN;
            mff_cycles(0x98F7, 8);
            a = 0x20;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x18FB, func_F34C, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x98FC, 31);
            a = 0x38;
            RAM(OBJ_VY_FRAC + x) = a;
            a = 0x02;
            RAM(OBJ_VY + x) = a;
            a = 0x80;
            RAM(OBJ_VX_FRAC + x) = a;
            t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
            RAM(OBJ_TIMER + x) = t;
            n = (t) >> 7;
            z = (t) == 0;
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x9C7A, 2); call_by_address_tail(0x9C7A, 6); return;
        } else {
            mff_cycles(0x98EC, 2);
        }
    } else {
        mff_cycles(0x98E4, 2);
    }
    mff_cycles(0x9911, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $986E (bank 6) - lifted by port/tools/lift.py */
void func_986E_b6(void) {
    REGS_IN;
    mff_cycles(0x986E, 13);
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x1873, 0x9C55, 6)) return;
    SYNC_IN;
    mff_cycles(0x9874, 8);
    a = RAM(OBJ_ID + x);
    if (a != 0x03) { mff_cycles(0x9879, 1); goto L_9884; }
    mff_cycles(0x987B, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9880, 1); goto L_98B8; }
    mff_cycles(0x9882, 2);
    if (!z) { mff_cycles(0x9882, 1); goto L_988B; }
L_9884:;
    mff_cycles(0x9884, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9889, 1); goto L_98B8; }
L_988B:;
    mff_cycles(0x988B, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    if (a == 0) { mff_cycles(0x9890, 1); goto L_98BB; }
    mff_cycles(0x9892, 5);
    a = RAM(KEEP_ON_SCREEN);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0x9896, 13);
        a = RAM(OBJ_X + x);
        c = 1;
        m = RAM(SCROLL_X);
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        m = 0xF1;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0x98A0, 25);
            a = RAM(SCROLL_X);
            c = 0;
            m = 0xF0;
            w = a + m + c;
            c = w > 0xFF;
            a = (uint8_t)w;
            RAM(OBJ_X + x) = a;
            a = RAM(AREA_SCREEN);
            m = 0x00;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_X_HI + x) = a;
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x98DE, 2); call_by_address_tail(0x98DE, 6); return;
        } else {
            mff_cycles(0x989E, 1);
        }
    } else {
        mff_cycles(0x9894, 1);
    }
    mff_cycles(0x98B2, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x18B4, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x98B5, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x98DE, 2); call_by_address_tail(0x98DE, 6); return;
L_98B8:;
    mff_cycles(0x98B8, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9912, 2); call_by_address_tail(0x9912, 6); return;
L_98BB:;
    mff_cycles(0x98BB, 5);
    a = RAM(KEEP_ON_SCREEN);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0x98BF, 13);
        a = RAM(OBJ_X + x);
        c = 1;
        m = RAM(SCROLL_X);
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        m = 0x10;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!c) {
            mff_cycles(0x98C9, 25);
            a = RAM(SCROLL_X);
            c = 0;
            m = 0x10;
            w = a + m + c;
            c = w > 0xFF;
            a = (uint8_t)w;
            RAM(OBJ_X + x) = a;
            a = RAM(AREA_SCREEN);
            m = 0x00;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_X_HI + x) = a;
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x98DE, 2); call_by_address_tail(0x98DE, 6); return;
        } else {
            mff_cycles(0x98C7, 1);
        }
    } else {
        mff_cycles(0x98BD, 1);
    }
    mff_cycles(0x98DB, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x18DD, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x98DE, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x18E0, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x98E1, 6);
    a = RAM(OBJ_VY + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0x98E6, 10);
        a = RAM(OBJ_Y + x);
        m = RAM(OBJ_LAND_Y + x);
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0x98EE, 15);
            a = RAM(OBJ_LAND_Y + x);
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_Y + x) = a;
            SYNC_OUT;
            if (!mff_call_at(0x18F6, 0x9B1F, 6)) return;
            SYNC_IN;
            mff_cycles(0x98F7, 8);
            a = 0x20;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x18FB, func_F34C, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x98FC, 31);
            a = 0x38;
            RAM(OBJ_VY_FRAC + x) = a;
            a = 0x02;
            RAM(OBJ_VY + x) = a;
            a = 0x80;
            RAM(OBJ_VX_FRAC + x) = a;
            t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
            RAM(OBJ_TIMER + x) = t;
            n = (t) >> 7;
            z = (t) == 0;
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x9C7A, 2); call_by_address_tail(0x9C7A, 6); return;
        } else {
            mff_cycles(0x98EC, 2);
        }
    } else {
        mff_cycles(0x98E4, 2);
    }
    mff_cycles(0x9911, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $98DE (bank 6) - lifted by port/tools/lift.py */
void func_98DE_b6(void) {
    REGS_IN;
    mff_cycles(0x98DE, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x18E0, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x98E1, 6);
    a = RAM(OBJ_VY + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0x98E6, 10);
        a = RAM(OBJ_Y + x);
        m = RAM(OBJ_LAND_Y + x);
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0x98EE, 15);
            a = RAM(OBJ_LAND_Y + x);
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_Y + x) = a;
            SYNC_OUT;
            if (!mff_call_at(0x18F6, 0x9B1F, 6)) return;
            SYNC_IN;
            mff_cycles(0x98F7, 8);
            a = 0x20;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x18FB, func_F34C, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x98FC, 31);
            a = 0x38;
            RAM(OBJ_VY_FRAC + x) = a;
            a = 0x02;
            RAM(OBJ_VY + x) = a;
            a = 0x80;
            RAM(OBJ_VX_FRAC + x) = a;
            t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
            RAM(OBJ_TIMER + x) = t;
            n = (t) >> 7;
            z = (t) == 0;
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x9C7A, 2); call_by_address_tail(0x9C7A, 6); return;
        } else {
            mff_cycles(0x98EC, 2);
        }
    } else {
        mff_cycles(0x98E4, 2);
    }
    mff_cycles(0x9911, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9912 (bank 6) - lifted by port/tools/lift.py */
void func_9912_b6(void) {
    REGS_IN;
    mff_cycles(0x9912, 8);
    a = RAM(OBJ_ID + x);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9917, 1); goto L_9923; }
    mff_cycles(0x9919, 2);
    if (!z) { mff_cycles(0x9919, 1); goto L_9920; }
L_991B:;
    mff_cycles(0x991B, 7);
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_INVULN + x) = a;
L_9920:;
    mff_cycles(0x9920, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9BFC, 2); call_by_address_tail(0x9BFC, 6); return;
L_9923:;
    mff_cycles(0x9923, 8);
    a = RAM(OBJ_STATE + x);
    a &= 0x02;
    if (a != 0) { mff_cycles(0x9928, 1); goto L_994A; }
    mff_cycles(0x992A, 8);
    a = RAM(OBJ_INVULN + x);
    m = 0xFE;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9920, 1); goto L_9920; }
    mff_cycles(0x9931, 4);
    m = 0xFD;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9920, 1); goto L_9920; }
    mff_cycles(0x9935, 11);
    a = 0x09;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ATTACK_ID) = a;
    SYNC_OUT;
    if (!mff_call_at(0x193B, 0x84F0, 6)) return;
    SYNC_IN;
    mff_cycles(0x993C, 11);
    a = 0x00;
    RAM(ATTACK_ID) = a;
    a = RAM(OBJ_LIFE + x);
    if (!(a & 0x80)) { mff_cycles(0x991B, 1); goto L_991B; }
    mff_cycles(0x9945, 12);
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
    t = (uint8_t)(RAM(AREA_KILLS) + 1);
    RAM(AREA_KILLS) = t;
L_994A:;
    mff_cycles(0x994A, 13);
    t = (uint8_t)(RAM(OBJ_TIMER2 + x) - 1);
    RAM(OBJ_TIMER2 + x) = t;
    a = RAM(OBJ_TIMER2 + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x9950, 1); goto L_995F; }
    mff_cycles(0x9952, 4);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x9954, 1); goto L_995B; }
    mff_cycles(0x9956, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1958, 0x9B62, 6)) return;
    SYNC_IN;
    mff_cycles(0x9959, 2);
    if (!z) { mff_cycles(0x9959, 1); goto L_995E; }
L_995B:;
    mff_cycles(0x995B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x195D, 0x9B6B, 6)) return;
    SYNC_IN;
L_995E:;
    mff_cycles(0x995E, 6);
    SYNC_OUT;
    mff_rts(); return;
L_995F:;
    mff_cycles(0x995F, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF2BE, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F2BE(); g_code_window_base = swb; } return;
}

/*  $9962 (bank 6) - lifted by port/tools/lift.py */
void func_9962_b6(void) {
    REGS_IN;
    mff_cycles(0x9962, 8);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    if (a == 0) {
        mff_cycles(0x9969, 25);
        RAM(OBJ_TIMER + x) = a;
        RAM(OBJ_VX + x) = a;
        a = 0x99;
        RAM(OBJ_VX_FRAC + x) = a;
        a = RAM(OBJ_ANIM + x);
        m = 0xBB;
        c = a >= m;
        if (a != m) {
            mff_cycles(0x997B, 4);
            m = 0xC7;
            c = a >= m;
            if (a != m) {
                mff_cycles(0x997F, 4);
                m = 0xFD;
                c = a >= m;
                n = ((uint8_t)(a - m)) >> 7;
                z = ((uint8_t)(a - m)) == 0;
                if (!z) {
                    mff_cycles(0x9983, 6);
                    SYNC_OUT;
                    if (!mff_call_at(0x1985, 0x89E3, 6)) return;
                    SYNC_IN;
                } else {
                    mff_cycles(0x9981, 1);
                }
            } else {
                mff_cycles(0x997D, 1);
            }
        } else {
            mff_cycles(0x9979, 1);
        }
        mff_cycles(0x9986, 7);
        y = RAM(PLAYER_LEVEL);
        a = prg_read((uint16_t)(0x9A33 + y));
        mff_cycles(0x998B, 9);
        RAM(OBJ_VY_FRAC + x) = a;
        a = prg_read((uint16_t)(0x9A39 + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x9991, 11);
        RAM(OBJ_VY + x) = a;
        SYNC_OUT;
        if (!mff_call_at(0x1996, 0x981B, 6)) return;
        SYNC_IN;
        mff_cycles(0x9997, 13);
        t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_STATE + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x199C, 0x9C55, 6)) return;
        SYNC_IN;
        mff_cycles(0x999D, 7);
        a = 0x10;
        RAM(OBJ_TIMER2 + x) = a;
    } else {
        mff_cycles(0x9967, 1);
    }
    mff_cycles(0x99A2, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x99A9, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x9A2D, 2); call_by_address_tail(0x9A2D, 6); return;
    } else {
        mff_cycles(0x99A7, 1);
    }
    mff_cycles(0x99AC, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    if (a != 0) {
        mff_cycles(0x99B3, 5);
        a = RAM(KEEP_ON_SCREEN);
        n = (a) >> 7;
        z = (a) == 0;
        if (!z) {
            mff_cycles(0x99B7, 13);
            a = RAM(OBJ_X + x);
            c = 1;
            m = RAM(SCROLL_X);
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            m = 0xF1;
            c = a >= m;
            n = ((uint8_t)(a - m)) >> 7;
            z = ((uint8_t)(a - m)) == 0;
            if (c) {
                mff_cycles(0x99C1, 25);
                a = RAM(SCROLL_X);
                c = 0;
                m = 0xF0;
                w = a + m + c;
                c = w > 0xFF;
                a = (uint8_t)w;
                RAM(OBJ_X + x) = a;
                a = RAM(AREA_SCREEN);
                m = 0x00;
                w = a + m + c;
                v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
                c = w > 0xFF;
                a = (uint8_t)w;
                n = (a) >> 7;
                z = (a) == 0;
                RAM(OBJ_X_HI + x) = a;
                SYNC_OUT;
                nes_cpu_instruction_boundary(0x99FC, 2); call_by_address_tail(0x99FC, 6); return;
            } else {
                mff_cycles(0x99BF, 1);
            }
        } else {
            mff_cycles(0x99B5, 1);
        }
        mff_cycles(0x99D3, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x19D5, func_F183, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x99D6, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x99FC, 2); call_by_address_tail(0x99FC, 6); return;
    } else {
        mff_cycles(0x99B1, 1);
    }
    mff_cycles(0x99D9, 5);
    a = RAM(KEEP_ON_SCREEN);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0x99DD, 13);
        a = RAM(OBJ_X + x);
        c = 1;
        m = RAM(SCROLL_X);
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        m = 0x10;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!c) {
            mff_cycles(0x99E7, 25);
            a = RAM(SCROLL_X);
            c = 0;
            m = 0x10;
            w = a + m + c;
            c = w > 0xFF;
            a = (uint8_t)w;
            RAM(OBJ_X + x) = a;
            a = RAM(AREA_SCREEN);
            m = 0x00;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_X_HI + x) = a;
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x99FC, 2); call_by_address_tail(0x99FC, 6); return;
        } else {
            mff_cycles(0x99E5, 1);
        }
    } else {
        mff_cycles(0x99DB, 1);
    }
    mff_cycles(0x99F9, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x19FB, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x99FC, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x19FE, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x99FF, 6);
    a = RAM(OBJ_VY + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0x9A04, 10);
        a = RAM(OBJ_Y + x);
        m = RAM(OBJ_LAND_Y + x);
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0x9A0C, 15);
            a = RAM(OBJ_LAND_Y + x);
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_Y + x) = a;
            SYNC_OUT;
            if (!mff_call_at(0x1A14, 0x9B1F, 6)) return;
            SYNC_IN;
            mff_cycles(0x9A15, 8);
            a = 0x20;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x1A19, func_F34C, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x9A1A, 28);
            a = 0x38;
            RAM(OBJ_VY_FRAC + x) = a;
            a = 0x02;
            RAM(OBJ_VY + x) = a;
            a = 0x80;
            RAM(OBJ_VX_FRAC + x) = a;
            t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
            RAM(OBJ_TIMER + x) = t;
            n = (t) >> 7;
            z = (t) == 0;
        } else {
            mff_cycles(0x9A0A, 1);
        }
    } else {
        mff_cycles(0x9A02, 1);
    }
    mff_cycles(0x9A2C, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $99FC (bank 6) - lifted by port/tools/lift.py */
void func_99FC_b6(void) {
    REGS_IN;
    mff_cycles(0x99FC, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x19FE, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x99FF, 6);
    a = RAM(OBJ_VY + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0x9A04, 10);
        a = RAM(OBJ_Y + x);
        m = RAM(OBJ_LAND_Y + x);
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0x9A0C, 15);
            a = RAM(OBJ_LAND_Y + x);
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_Y + x) = a;
            SYNC_OUT;
            if (!mff_call_at(0x1A14, 0x9B1F, 6)) return;
            SYNC_IN;
            mff_cycles(0x9A15, 8);
            a = 0x20;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x1A19, func_F34C, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x9A1A, 28);
            a = 0x38;
            RAM(OBJ_VY_FRAC + x) = a;
            a = 0x02;
            RAM(OBJ_VY + x) = a;
            a = 0x80;
            RAM(OBJ_VX_FRAC + x) = a;
            t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
            RAM(OBJ_TIMER + x) = t;
            n = (t) >> 7;
            z = (t) == 0;
        } else {
            mff_cycles(0x9A0A, 1);
        }
    } else {
        mff_cycles(0x9A02, 1);
    }
    mff_cycles(0x9A2C, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9A2D (bank 6) - lifted by port/tools/lift.py */
void func_9A2D_b6(void) {
    REGS_IN;
    mff_cycles(0x9A2D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1A2F, 0x9C7A, 6)) return;
    SYNC_IN;
    mff_cycles(0x9A30, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9B07, 2); call_by_address_tail(0x9B07, 6); return;
}

/*  $9A3F (bank 6) - lifted by port/tools/lift.py */
void func_9A3F_b6(void) {
    REGS_IN;
    mff_cycles(0x9A3F, 8);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    if (a == 0) {
        mff_cycles(0x9A46, 25);
        RAM(OBJ_TIMER + x) = a;
        RAM(OBJ_VX + x) = a;
        a = 0x99;
        RAM(OBJ_VX_FRAC + x) = a;
        a = RAM(OBJ_ANIM + x);
        if (a != 0xBB) {
            mff_cycles(0x9A58, 4);
            m = 0xC7;
            c = a >= m;
            n = ((uint8_t)(a - m)) >> 7;
            z = ((uint8_t)(a - m)) == 0;
            if (!z) {
                mff_cycles(0x9A5C, 6);
                SYNC_OUT;
                if (!mff_call_at(0x1A5E, 0x89E3, 6)) return;
                SYNC_IN;
            } else {
                mff_cycles(0x9A5A, 1);
            }
        } else {
            mff_cycles(0x9A56, 1);
        }
        mff_cycles(0x9A5F, 22);
        a = 0x9E;
        RAM(OBJ_VY_FRAC + x) = a;
        a = 0x03;
        RAM(OBJ_VY + x) = a;
        a = RAM(OBJ_ID + x);
        m = 0x16;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0x9A70, 6);
            SYNC_OUT;
            if (!mff_call_at(0x1A72, 0x981B, 6)) return;
            SYNC_IN;
        } else {
            mff_cycles(0x9A6E, 1);
        }
        mff_cycles(0x9A73, 13);
        t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_STATE + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x1A78, 0x9C55, 6)) return;
        SYNC_IN;
        mff_cycles(0x9A79, 7);
        a = 0x10;
        RAM(OBJ_TIMER2 + x) = a;
    } else {
        mff_cycles(0x9A44, 1);
    }
    mff_cycles(0x9A7E, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x9A85, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x9B04, 2); call_by_address_tail(0x9B04, 6); return;
    } else {
        mff_cycles(0x9A83, 1);
    }
    mff_cycles(0x9A88, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    if (a != 0) {
        mff_cycles(0x9A8F, 5);
        a = RAM(KEEP_ON_SCREEN);
        n = (a) >> 7;
        z = (a) == 0;
        if (!z) {
            mff_cycles(0x9A93, 13);
            a = RAM(OBJ_X + x);
            c = 1;
            m = RAM(SCROLL_X);
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            m = 0xF1;
            c = a >= m;
            n = ((uint8_t)(a - m)) >> 7;
            z = ((uint8_t)(a - m)) == 0;
            if (c) {
                mff_cycles(0x9A9D, 25);
                a = RAM(SCROLL_X);
                c = 0;
                m = 0xF0;
                w = a + m + c;
                c = w > 0xFF;
                a = (uint8_t)w;
                RAM(OBJ_X + x) = a;
                a = RAM(AREA_SCREEN);
                m = 0x00;
                w = a + m + c;
                v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
                c = w > 0xFF;
                a = (uint8_t)w;
                n = (a) >> 7;
                z = (a) == 0;
                RAM(OBJ_X_HI + x) = a;
                SYNC_OUT;
                nes_cpu_instruction_boundary(0x9AD8, 2); call_by_address_tail(0x9AD8, 6); return;
            } else {
                mff_cycles(0x9A9B, 1);
            }
        } else {
            mff_cycles(0x9A91, 1);
        }
        mff_cycles(0x9AAF, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1AB1, func_F183, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9AB2, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x9AD8, 2); call_by_address_tail(0x9AD8, 6); return;
    } else {
        mff_cycles(0x9A8D, 1);
    }
    mff_cycles(0x9AB5, 5);
    a = RAM(KEEP_ON_SCREEN);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0x9AB9, 13);
        a = RAM(OBJ_X + x);
        c = 1;
        m = RAM(SCROLL_X);
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        m = 0x10;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!c) {
            mff_cycles(0x9AC3, 25);
            a = RAM(SCROLL_X);
            c = 0;
            m = 0x10;
            w = a + m + c;
            c = w > 0xFF;
            a = (uint8_t)w;
            RAM(OBJ_X + x) = a;
            a = RAM(AREA_SCREEN);
            m = 0x00;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_X_HI + x) = a;
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x9AD8, 2); call_by_address_tail(0x9AD8, 6); return;
        } else {
            mff_cycles(0x9AC1, 1);
        }
    } else {
        mff_cycles(0x9AB7, 1);
    }
    mff_cycles(0x9AD5, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1AD7, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9AD8, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1ADA, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9ADB, 6);
    a = RAM(OBJ_VY + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0x9AE0, 10);
        a = RAM(OBJ_Y + x);
        m = RAM(OBJ_LAND_Y + x);
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0x9AE8, 15);
            a = RAM(OBJ_LAND_Y + x);
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_Y + x) = a;
            SYNC_OUT;
            if (!mff_call_at(0x1AF0, 0x9B1F, 6)) return;
            SYNC_IN;
            mff_cycles(0x9AF1, 28);
            a = 0x38;
            RAM(OBJ_VY_FRAC + x) = a;
            a = 0x02;
            RAM(OBJ_VY + x) = a;
            a = 0x80;
            RAM(OBJ_VX_FRAC + x) = a;
            t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
            RAM(OBJ_TIMER + x) = t;
            n = (t) >> 7;
            z = (t) == 0;
        } else {
            mff_cycles(0x9AE6, 2);
        }
    } else {
        mff_cycles(0x9ADE, 2);
    }
    mff_cycles(0x9B03, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9AD8 (bank 6) - lifted by port/tools/lift.py */
void func_9AD8_b6(void) {
    REGS_IN;
    mff_cycles(0x9AD8, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1ADA, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9ADB, 6);
    a = RAM(OBJ_VY + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0x9AE0, 10);
        a = RAM(OBJ_Y + x);
        m = RAM(OBJ_LAND_Y + x);
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0x9AE8, 15);
            a = RAM(OBJ_LAND_Y + x);
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_Y + x) = a;
            SYNC_OUT;
            if (!mff_call_at(0x1AF0, 0x9B1F, 6)) return;
            SYNC_IN;
            mff_cycles(0x9AF1, 28);
            a = 0x38;
            RAM(OBJ_VY_FRAC + x) = a;
            a = 0x02;
            RAM(OBJ_VY + x) = a;
            a = 0x80;
            RAM(OBJ_VX_FRAC + x) = a;
            t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
            RAM(OBJ_TIMER + x) = t;
            n = (t) >> 7;
            z = (t) == 0;
        } else {
            mff_cycles(0x9AE6, 2);
        }
    } else {
        mff_cycles(0x9ADE, 2);
    }
    mff_cycles(0x9B03, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9B04 (bank 6) - lifted by port/tools/lift.py */
void func_9B04_b6(void) {
    REGS_IN;
    goto L_9B04;
L_9B03:;
    mff_cycles(0x9B03, 6);
    SYNC_OUT;
    mff_rts(); return;
L_9B04:;
    mff_cycles(0x9B04, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1B06, 0x9C7A, 6)) return;
    SYNC_IN;
    mff_cycles(0x9B07, 13);
    t = (uint8_t)(RAM(OBJ_TIMER2 + x) - 1);
    RAM(OBJ_TIMER2 + x) = t;
    a = RAM(OBJ_TIMER2 + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x9B0D, 1); goto L_9B1C; }
    mff_cycles(0x9B0F, 4);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x9B11, 1); goto L_9B18; }
    mff_cycles(0x9B13, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1B15, 0x9B62, 6)) return;
    SYNC_IN;
    mff_cycles(0x9B16, 2);
    if (!z) { mff_cycles(0x9B03, 1); goto L_9B03; }
L_9B18:;
    mff_cycles(0x9B18, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1B1A, 0x9B6B, 6)) return;
    SYNC_IN;
    mff_cycles(0x9B1B, 6);
    SYNC_OUT;
    mff_rts(); return;
L_9B1C:;
    mff_cycles(0x9B1C, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF2BE, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F2BE(); g_code_window_base = swb; } return;
}

/*  $9B07 (bank 6) - lifted by port/tools/lift.py */
void func_9B07_b6(void) {
    REGS_IN;
    goto L_9B07;
L_9B03:;
    mff_cycles(0x9B03, 6);
    SYNC_OUT;
    mff_rts(); return;
L_9B07:;
    mff_cycles(0x9B07, 13);
    t = (uint8_t)(RAM(OBJ_TIMER2 + x) - 1);
    RAM(OBJ_TIMER2 + x) = t;
    a = RAM(OBJ_TIMER2 + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x9B0D, 1); goto L_9B1C; }
    mff_cycles(0x9B0F, 4);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x9B11, 1); goto L_9B18; }
    mff_cycles(0x9B13, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1B15, 0x9B62, 6)) return;
    SYNC_IN;
    mff_cycles(0x9B16, 2);
    if (!z) { mff_cycles(0x9B03, 1); goto L_9B03; }
L_9B18:;
    mff_cycles(0x9B18, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1B1A, 0x9B6B, 6)) return;
    SYNC_IN;
    mff_cycles(0x9B1B, 6);
    SYNC_OUT;
    mff_rts(); return;
L_9B1C:;
    mff_cycles(0x9B1C, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF2BE, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F2BE(); g_code_window_base = swb; } return;
}

/* obj_pit_check $9B1F (bank 6) - lifted by port/tools/lift.py */
void func_9B1F_b6(void) {
    REGS_IN;
    mff_cycles(0x9B1F, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1B21, 0x9BE1, 6)) return;
    SYNC_IN;
    mff_cycles(0x9B22, 15);
    a = RAM(OBJ_Y + x);
    RAM(TMP15) = a;
    a = RAM(OBJ_KIND + x);
    m = 0x06;
    c = a >= m;
    if (a == m) {
        mff_cycles(0x9B2E, 13);
        a = RAM(OBJ_Y + x);
        c = 1;
        m = 0x0C;
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        RAM(OBJ_Y + x) = a;
    } else {
        mff_cycles(0x9B2C, 1);
    }
    mff_cycles(0x9B37, 8);
    y = 0x06;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1B3B, func_ECCF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9B3C, 15);
    a = RAM(TMP15);
    RAM(OBJ_Y + x) = a;
    a = RAM(TMP16);
    a &= 0x40;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0x9B47, 43);
        a = RAM(OBJ_ID + x);
        RAM(OBJ_SAVED_ID + x) = a;
        a = 0x1F;
        RAM(OBJ_ID + x) = a;
        RAM(OBJ_PIT_FLAG + x) = a;
        a = 0x00;
        RAM(OBJ_TIMER + x) = a;
        a = 0xFF;
        RAM(OBJ_INVULN + x) = a;
        a = mff_pull();
        a = mff_pull();
        n = (a) >> 7;
        z = (a) == 0;
    } else {
        mff_cycles(0x9B45, 1);
    }
    mff_cycles(0x9B61, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9B62 (bank 6) - lifted by port/tools/lift.py */
void func_9B62_b6(void) {
    REGS_IN;
    mff_cycles(0x9B62, 17);
    a = RAM(OBJ_FLAGS + x);
    a |= 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9B6B (bank 6) - lifted by port/tools/lift.py */
void func_9B6B_b6(void) {
    REGS_IN;
    mff_cycles(0x9B6B, 17);
    a = RAM(OBJ_FLAGS + x);
    a &= 0xFB;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9B74 (bank 6) - lifted by port/tools/lift.py */
void func_9B74_b6(void) {
    REGS_IN;
    mff_cycles(0x9B74, 13);
    a = 0xFF;
    RAM(OBJ_INVULN + x) = a;
    a = RAM(OBJ_TIMER + x);
    if (a != 0) { mff_cycles(0x9B7C, 1); goto L_9BD9; }
    mff_cycles(0x9B7E, 6);
    a = RAM(OBJ_PIT_FLAG + x);
    if (a == 0) {
        mff_cycles(0x9B83, 5);
        RAM(OBJ_ANIM_TIMER + x) = a;
    } else {
        mff_cycles(0x9B81, 1);
    }
    mff_cycles(0x9B86, 18);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_VX + x) = a;
    RAM(OBJ_VX_FRAC + x) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x1B90, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9B91, 8);
    a = RAM(OBJ_Y + x);
    m = 0xB0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x9B96, 1); goto L_9BD8; }
    mff_cycles(0x9B98, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1B9A, 0x9B62, 6)) return;
    SYNC_IN;
    mff_cycles(0x9B9B, 8);
    a = RAM(OBJ_KIND + x);
    m = 0x06;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9BA0, 1); goto L_9BDE; }
    mff_cycles(0x9BA2, 6);
    a = RAM(OBJ_LIFE + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) { mff_cycles(0x9BA5, 1); goto L_9BDE; }
    mff_cycles(0x9BA7, 8);
    a = RAM(OBJ_SAVED_ID + x);
    m = 0x20;
    c = a >= m;
    if (!c) { mff_cycles(0x9BAC, 1); goto L_9BBC; }
    mff_cycles(0x9BAE, 8);
    a = RAM(OBJ_HELD + x);
    m = 0xFF;
    c = a >= m;
    if (a == m) { mff_cycles(0x9BB3, 1); goto L_9BBC; }
    mff_cycles(0x9BB5, 8);
    a = RAM(OBJ_ANIM + x);
    m = 0x9D;
    c = a >= m;
    if (a != m) { mff_cycles(0x9BBA, 1); goto L_9BC3; }
L_9BBC:;
    mff_cycles(0x9BBC, 11);
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ATTACK_ID) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1BC2, 0x89E3, 6)) return;
    SYNC_IN;
L_9BC3:;
    mff_cycles(0x9BC3, 34);
    a = 0xFF;
    RAM(OBJ_LIFE + x) = a;
    RAM(HUD_ENEMY_SLOT) = x;
    a = 0x00;
    RAM(HUD_ENEMY_TIMER) = a;
    a = 0x10;
    RAM(OBJ_AI_TIMER + x) = a;
    t = (uint8_t)(RAM(AREA_KILLS) + 1);
    RAM(AREA_KILLS) = t;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
L_9BD8:;
    mff_cycles(0x9BD8, 6);
    SYNC_OUT;
    mff_rts(); return;
L_9BD9:;
    mff_cycles(0x9BD9, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) { mff_cycles(0x9BD8, 1); goto L_9BD8; }
L_9BDE:;
    mff_cycles(0x9BDE, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF2BE, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F2BE(); g_code_window_base = swb; } return;
}

/*  $9BE1 (bank 6) - lifted by port/tools/lift.py */
void func_9BE1_b6(void) {
    REGS_IN;
    mff_cycles(0x9BE1, 8);
    a = RAM(OBJ_KIND + x);
    m = 0x07;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x9BE6, 1); goto L_9BFB; }
    mff_cycles(0x9BE8, 8);
    a = RAM(OBJ_X_HI + x);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x9BED, 1); goto L_9BFB; }
    mff_cycles(0x9BEF, 2);
    if (!z) { mff_cycles(0x9BEF, 1); goto L_9BF8; }
    mff_cycles(0x9BF1, 8);
    a = RAM(OBJ_X + x);
    m = 0xF0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x9BF6, 1); goto L_9BFB; }
L_9BF8:;
    mff_cycles(0x9BF8, 10);
    a = mff_pull();
    a = mff_pull();
    n = (a) >> 7;
    z = (a) == 0;
    c = 0;
L_9BFB:;
    mff_cycles(0x9BFB, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/* obj_knockdown_recover $9BFC (bank 6) - lifted by port/tools/lift.py */
void func_9BFC_b6(void) {
    REGS_IN;
    mff_cycles(0x9BFC, 8);
    a = RAM(OBJ_INVULN + x);
    m = 0xFE;
    c = a >= m;
    if (a == m) { mff_cycles(0x9C01, 1); goto L_9C13; }
    mff_cycles(0x9C03, 4);
    m = 0xFD;
    c = a >= m;
    if (a == m) { mff_cycles(0x9C05, 1); goto L_9C24; }
    mff_cycles(0x9C07, 13);
    a = 0x2D;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DOWN_TIMER + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1C0E, 0x9C7A, 6)) return;
    SYNC_IN;
    mff_cycles(0x9C0F, 13);
    t = (uint8_t)(RAM(OBJ_INVULN + x) - 1);
    RAM(OBJ_INVULN + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_9C13:;
    mff_cycles(0x9C13, 9);
    t = (uint8_t)(RAM(OBJ_DOWN_TIMER + x) - 1);
    RAM(OBJ_DOWN_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) { mff_cycles(0x9C16, 1); goto L_9C54; }
    mff_cycles(0x9C18, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1C1A, 0x9CAD, 6)) return;
    SYNC_IN;
    mff_cycles(0x9C1B, 20);
    a = 0x17;
    RAM(OBJ_DOWN_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_INVULN + x) - 1);
    RAM(OBJ_INVULN + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_9C24:;
    mff_cycles(0x9C24, 9);
    t = (uint8_t)(RAM(OBJ_DOWN_TIMER + x) - 1);
    RAM(OBJ_DOWN_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (z) {
        mff_cycles(0x9C29, 24);
        a = RAM(OBJ_SAVED_ID + x);
        RAM(OBJ_ID + x) = a;
        a = RAM(OBJ_SAVED_STATE + x);
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_STATE + x) = a;
        SYNC_OUT;
        if (!mff_call_at(0x1C37, 0xAA1D, 6)) return;
        SYNC_IN;
        mff_cycles(0x9C38, 36);
        a = 0x00;
        RAM(OBJ_THROW_FLAG + x) = a;
        RAM(OBJ_HIT_TIMER + x) = a;
        RAM(OBJ_HELD + x) = a;
        a = RAM(OBJ_FLAGS + x);
        a |= 0x80;
        RAM(OBJ_FLAGS + x) = a;
        y = RAM(OBJ_KIND + x);
        a = prg_read((uint16_t)(0xFC82 + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x9C51, 5);
        RAM(OBJ_INVULN + x) = a;
    } else {
        mff_cycles(0x9C27, 1);
    }
L_9C54:;
    mff_cycles(0x9C54, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9C29 (bank 6) - lifted by port/tools/lift.py */
void func_9C29_b6(void) {
    REGS_IN;
    mff_cycles(0x9C29, 24);
    a = RAM(OBJ_SAVED_ID + x);
    RAM(OBJ_ID + x) = a;
    a = RAM(OBJ_SAVED_STATE + x);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1C37, 0xAA1D, 6)) return;
    SYNC_IN;
    mff_cycles(0x9C38, 36);
    a = 0x00;
    RAM(OBJ_THROW_FLAG + x) = a;
    RAM(OBJ_HIT_TIMER + x) = a;
    RAM(OBJ_HELD + x) = a;
    a = RAM(OBJ_FLAGS + x);
    a |= 0x80;
    RAM(OBJ_FLAGS + x) = a;
    y = RAM(OBJ_KIND + x);
    a = prg_read((uint16_t)(0xFC82 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9C51, 11);
    RAM(OBJ_INVULN + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9C55 (bank 6) - lifted by port/tools/lift.py */
void func_9C55_b6(void) {
    REGS_IN;
    mff_cycles(0x9C55, 10);
    a = RAM(OBJ_KIND + x);
    y = a;
    a = prg_read((uint16_t)(0xFC8F + y));
    mff_cycles(0x9C5C, 6);
    m = RAM(OBJ_ANIM + x);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0x9C61, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1C63, func_F288, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9C64, 8);
        a = RAM(OBJ_KIND + x);
        m = 0x06;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0x9C6B, 4);
            a = prg_read((uint16_t)(0xFC9D + y));
            mff_cycles(0x9C6E, 7);
            RAM(TMP0) = a;
            a = prg_read((uint16_t)(0xFC33 + y));
            mff_cycles(0x9C73, 10);
            y = a;
            a = RAM(TMP0);
            n = (a) >> 7;
            z = (a) == 0;
            RAM(CHR_BANKS + 3 + y) = a;
        } else {
            mff_cycles(0x9C69, 1);
        }
    } else {
        mff_cycles(0x9C5F, 1);
    }
    mff_cycles(0x9C79, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9C7A (bank 6) - lifted by port/tools/lift.py */
void func_9C7A_b6(void) {
    REGS_IN;
    mff_cycles(0x9C7A, 10);
    a = RAM(OBJ_KIND + x);
    y = a;
    a = prg_read((uint16_t)(0xFCAB + y));
    mff_cycles(0x9C81, 6);
    m = RAM(OBJ_ANIM + x);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0x9C86, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1C88, func_F288, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9C89, 8);
        a = RAM(OBJ_KIND + x);
        m = 0x06;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0x9C90, 4);
            a = prg_read((uint16_t)(0x9C9F + y));
            mff_cycles(0x9C93, 7);
            RAM(TMP0) = a;
            a = prg_read((uint16_t)(0xFC33 + y));
            mff_cycles(0x9C98, 10);
            y = a;
            a = RAM(TMP0);
            n = (a) >> 7;
            z = (a) == 0;
            RAM(CHR_BANKS + 3 + y) = a;
        } else {
            mff_cycles(0x9C8E, 1);
        }
    } else {
        mff_cycles(0x9C84, 1);
    }
    mff_cycles(0x9C9E, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9CAD (bank 6) - lifted by port/tools/lift.py */
void func_9CAD_b6(void) {
    REGS_IN;
    mff_cycles(0x9CAD, 10);
    a = RAM(OBJ_KIND + x);
    y = a;
    a = prg_read((uint16_t)(0x9CD3 + y));
    mff_cycles(0x9CB4, 6);
    m = RAM(OBJ_ANIM + x);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0x9CB9, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1CBB, func_F288, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9CBC, 4);
        a = prg_read((uint16_t)(0x9CE0 + y));
        mff_cycles(0x9CBF, 7);
        RAM(TMP0) = a;
        a = prg_read((uint16_t)(0xFC33 + y));
        mff_cycles(0x9CC4, 10);
        y = a;
        a = RAM(OBJ_KIND + x);
        m = 0x08;
        c = a >= m;
        if (a == m) {
            mff_cycles(0x9CCC, 2);
            y++;
        } else {
            mff_cycles(0x9CCA, 1);
        }
        mff_cycles(0x9CCD, 8);
        a = RAM(TMP0);
        n = (a) >> 7;
        z = (a) == 0;
        RAM(CHR_BANKS + 3 + y) = a;
    } else {
        mff_cycles(0x9CB7, 1);
    }
    mff_cycles(0x9CD2, 6);
    SYNC_OUT;
    mff_rts(); return;
}

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

/*  $A04C (bank 6) - lifted by port/tools/lift.py */
void func_A04C_b6(void) {
    REGS_IN;
    mff_cycles(0xA04C, 32);
    a = 0x00;
    RAM(OBJ_AI_TIMER + x) = a;
    RAM(OBJ_AI_COUNT + x) = a;
    RAM(OBJ_AI_FLAG + x) = a;
    RAM(OBJ_TIMER3 + x) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAAAE, 2); call_by_address_tail(0xAAAE, 6); return;
}

/*  $A060 (bank 6) - lifted by port/tools/lift.py */
void func_A060_b6(void) {
    REGS_IN;
    mff_cycles(0xA060, 6);
    a = RAM(OBJ_PIT_FLAG + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA063, 1); goto L_A095; }
    mff_cycles(0xA065, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0067, 0xAAD7, 6)) return;
    SYNC_IN;
    mff_cycles(0xA068, 10);
    a = 0x38;
    y = 0x18;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x006E, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA06F, 2);
    if (c) { mff_cycles(0xA06F, 1); goto L_A07A; }
    mff_cycles(0xA071, 10);
    a = 0x18;
    y = 0x28;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0077, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA078, 2);
    if (!c) { mff_cycles(0xA078, 1); goto L_A085; }
L_A07A:;
    mff_cycles(0xA07A, 23);
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
    a = RAM(RNG);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_FLAG + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_A085:;
    mff_cycles(0xA085, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0087, func_FB5B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA088, 6);
    y = a;
    a = prg_read((uint16_t)(0xFBF0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA08C, 11);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0091, 0xAC34, 6)) return;
    SYNC_IN;
    mff_cycles(0xA092, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA953, 2); call_by_address_tail(0xA953, 6); return;
L_A095:;
    mff_cycles(0xA095, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0097, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA098, 8);
    a = RAM(OBJ_PIT_FLAG + x);
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    if (!c) { mff_cycles(0xA09C, 1); goto L_A0B7; }
    mff_cycles(0xA09E, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x00A2, 0xAC34, 6)) return;
    SYNC_IN;
    mff_cycles(0xA0A3, 15);
    a = 0x08;
    RAM(OBJ_DIR + x) = a;
    y = 0x01;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x00AC, func_F0F7, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA0AD, 2);
    if (c) { mff_cycles(0xA0AD, 1); goto L_A0B7; }
    mff_cycles(0xA0AF, 8);
    a = RAM(OBJ_Y + x);
    m = 0x88;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xA0B4, 1); goto L_A0E0; }
L_A0B6:;
    mff_cycles(0xA0B6, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A0B7:;
    mff_cycles(0xA0B7, 8);
    y = 0x04;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x00BB, 0xAC34, 6)) return;
    SYNC_IN;
    mff_cycles(0xA0BC, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xA0C1, 1); goto L_A0D2; }
    mff_cycles(0xA0C3, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x00C5, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA0C6, 8);
    y = 0x07;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x00CA, func_ECCF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA0CB, 7);
    a = RAM(TMP16);
    a &= 0x40;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xA0CF, 1); goto L_A0E0; }
    mff_cycles(0xA0D1, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A0D2:;
    mff_cycles(0xA0D2, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x00D4, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA0D5, 8);
    y = 0x08;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x00D9, func_ECCF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA0DA, 7);
    a = RAM(TMP16);
    a &= 0x40;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA0B6, 1); goto L_A0B6; }
L_A0E0:;
    mff_cycles(0xA0E0, 10);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_PIT_FLAG + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA8E8, 2); call_by_address_tail(0xA8E8, 6); return;
}

/*  $A085 (bank 6) - lifted by port/tools/lift.py */
void func_A085_b6(void) {
    REGS_IN;
    mff_cycles(0xA085, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0087, func_FB5B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA088, 6);
    y = a;
    a = prg_read((uint16_t)(0xFBF0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA08C, 11);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0091, 0xAC34, 6)) return;
    SYNC_IN;
    mff_cycles(0xA092, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA953, 2); call_by_address_tail(0xA953, 6); return;
}

/*  $A088 (bank 6) - lifted by port/tools/lift.py */
void func_A088_b6(void) {
    REGS_IN;
    mff_cycles(0xA088, 6);
    y = a;
    a = prg_read((uint16_t)(0xFBF0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA08C, 11);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0091, 0xAC34, 6)) return;
    SYNC_IN;
    mff_cycles(0xA092, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA953, 2); call_by_address_tail(0xA953, 6); return;
}

/*  $A0E8 (bank 6) - lifted by port/tools/lift.py */
void func_A0E8_b6(void) {
    REGS_IN;
    mff_cycles(0xA0E8, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!n) {
        mff_cycles(0xA0ED, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA18B, 2); call_by_address_tail(0xA18B, 6); return;
    } else {
        mff_cycles(0xA0EB, 1);
    }
    mff_cycles(0xA0F0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x00F2, func_FB5B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA0F3, 8);
    y = a;
    a = RAM(OBJ_AI_FLAG + x);
    if (a == 0) {
        mff_cycles(0xA0F9, 4);
        a = prg_read((uint16_t)(0xAD66 + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0xA0FC, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA102, 2); call_by_address_tail(0xA102, 6); return;
    } else {
        mff_cycles(0xA0F7, 1);
    }
    mff_cycles(0xA0FF, 4);
    a = prg_read((uint16_t)(0xAD76 + y));
    mff_cycles(0xA102, 9);
    RAM(OBJ_TIMER + x) = a;
    a &= 0xF0;
    if (a != 0) {
        mff_cycles(0xA109, 29);
        a = RAM(OBJ_KIND + x);
        t = a;
        t = (uint8_t)(t << 1);
        a = t;
        t = a;
        t = (uint8_t)(t << 1);
        a = t;
        t = a;
        t = (uint8_t)(t << 1);
        a = t;
        RAM(TMP0) = a;
        a = RAM(RNG);
        a &= 0x07;
        c = 0;
        m = RAM(TMP0);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        y = a;
        a = prg_read((uint16_t)(0xADFC + y));
        mff_cycles(0xA11C, 2);
        if (a != 0) {
            mff_cycles(0xA11E, 12);
            t = a;
            t >>= 1;
            a = t;
            t = a;
            t >>= 1;
            a = t;
            t = a;
            t >>= 1;
            a = t;
            t = a;
            t >>= 1;
            a = t;
            m = 0x01;
            c = a >= m;
            n = ((uint8_t)(a - m)) >> 7;
            z = ((uint8_t)(a - m)) == 0;
            if (z) { mff_cycles(0xA124, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0139), 6); return; }
            mff_cycles(0xA126, 6);
            y = a;
            a = prg_read((uint16_t)(0xA133 + y));
            mff_cycles(0xA12A, 18);
            RAM(OBJ_STATE + x) = a;
            a = 0x00;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_TIMER + x) = a;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xA11C, 1);
        }
        mff_cycles(0xA162, 8);
        a = RAM(OBJ_TIMER + x);
        a &= 0x30;
        if (a != 0) {
            mff_cycles(0xA169, 10);
            a = 0x40;
            y = 0xF0;
            n = (y) >> 7;
            z = (y) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x016F, func_FB3E, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xA170, 2);
            if (!c) {
                mff_cycles(0xA172, 13);
                t = (uint8_t)(RAM(OBJ_STATE + x) - 1);
                RAM(OBJ_STATE + x) = t;
                n = (t) >> 7;
                z = (t) == 0;
                SYNC_OUT;
                mff_rts(); return;
            } else {
                mff_cycles(0xA170, 1);
            }
        } else {
            mff_cycles(0xA167, 1);
        }
    } else {
        mff_cycles(0xA107, 1);
    }
    mff_cycles(0xA176, 12);
    a = RAM(OBJ_TIMER + x);
    a &= 0x0F;
    y = a;
    a = prg_read((uint16_t)(0xFBF0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA17F, 11);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0184, 0xAC34, 6)) return;
    SYNC_IN;
    mff_cycles(0xA185, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0187, 0xACB3, 6)) return;
    SYNC_IN;
    mff_cycles(0xA188, 8);
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA953, 2); call_by_address_tail(0xA953, 6); return;
}

/*  $A102 (bank 6) - lifted by port/tools/lift.py */
void func_A102_b6(void) {
    REGS_IN;
    mff_cycles(0xA102, 9);
    RAM(OBJ_TIMER + x) = a;
    a &= 0xF0;
    if (a != 0) {
        mff_cycles(0xA109, 29);
        a = RAM(OBJ_KIND + x);
        t = a;
        t = (uint8_t)(t << 1);
        a = t;
        t = a;
        t = (uint8_t)(t << 1);
        a = t;
        t = a;
        t = (uint8_t)(t << 1);
        a = t;
        RAM(TMP0) = a;
        a = RAM(RNG);
        a &= 0x07;
        c = 0;
        m = RAM(TMP0);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        y = a;
        a = prg_read((uint16_t)(0xADFC + y));
        mff_cycles(0xA11C, 2);
        if (a != 0) {
            mff_cycles(0xA11E, 12);
            t = a;
            t >>= 1;
            a = t;
            t = a;
            t >>= 1;
            a = t;
            t = a;
            t >>= 1;
            a = t;
            t = a;
            t >>= 1;
            a = t;
            m = 0x01;
            c = a >= m;
            n = ((uint8_t)(a - m)) >> 7;
            z = ((uint8_t)(a - m)) == 0;
            if (z) { mff_cycles(0xA124, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0139), 6); return; }
            mff_cycles(0xA126, 6);
            y = a;
            a = prg_read((uint16_t)(0xA133 + y));
            mff_cycles(0xA12A, 18);
            RAM(OBJ_STATE + x) = a;
            a = 0x00;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_TIMER + x) = a;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xA11C, 1);
        }
        mff_cycles(0xA162, 8);
        a = RAM(OBJ_TIMER + x);
        a &= 0x30;
        if (a != 0) {
            mff_cycles(0xA169, 10);
            a = 0x40;
            y = 0xF0;
            n = (y) >> 7;
            z = (y) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x016F, func_FB3E, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xA170, 2);
            if (!c) {
                mff_cycles(0xA172, 13);
                t = (uint8_t)(RAM(OBJ_STATE + x) - 1);
                RAM(OBJ_STATE + x) = t;
                n = (t) >> 7;
                z = (t) == 0;
                SYNC_OUT;
                mff_rts(); return;
            } else {
                mff_cycles(0xA170, 1);
            }
        } else {
            mff_cycles(0xA167, 1);
        }
    } else {
        mff_cycles(0xA107, 1);
    }
    mff_cycles(0xA176, 12);
    a = RAM(OBJ_TIMER + x);
    a &= 0x0F;
    y = a;
    a = prg_read((uint16_t)(0xFBF0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA17F, 11);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0184, 0xAC34, 6)) return;
    SYNC_IN;
    mff_cycles(0xA185, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0187, 0xACB3, 6)) return;
    SYNC_IN;
    mff_cycles(0xA188, 8);
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA953, 2); call_by_address_tail(0xA953, 6); return;
}

/*  $A139 (bank 6) - lifted by port/tools/lift.py */
void func_A139_b6(void) {
    REGS_IN;
    mff_cycles(0xA139, 13);
    y = RAM(OBJ_KIND + x);
    a = RAM(RNG + 1);
    a &= 0x0F;
    mff_cycles(0xA143, 2);
    if (a >= prg_read((uint16_t)(0xAE32 + y))) { mff_cycles(0xA143, 1); goto L_A14A; }
    mff_cycles(0xA145, 4);
    a = prg_read((uint16_t)(0xAE38 + y));
    mff_cycles(0xA148, 2);
    if (a != 0) { mff_cycles(0xA148, 1); goto L_A14C; }
L_A14A:;
    mff_cycles(0xA14A, 2);
    a = 0x00;
L_A14C:;
    mff_cycles(0xA14C, 19);
    RAM(OBJ_AI_TIMER + x) = a;
    a = RAM(OBJ_ID + x);
    c = 1;
    m = 0x20;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    y = a;
    a = prg_read((uint16_t)(0xA3DF + y));
    mff_cycles(0xA159, 18);
    RAM(OBJ_AI_COUNT + x) = a;
    a = 0x84;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A18B (bank 6) - lifted by port/tools/lift.py */
void func_A18B_b6(void) {
    REGS_IN;
    mff_cycles(0xA18B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA953, 2); call_by_address_tail(0xA953, 6); return;
}

/*  $A18E (bank 6) - lifted by port/tools/lift.py */
void func_A18E_b6(void) {
    REGS_IN;
    mff_cycles(0xA18E, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0xA193, 1); goto L_A1C2; }
    mff_cycles(0xA195, 4);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA197, 1); goto L_A1EF; }
    mff_cycles(0xA199, 6);
    SYNC_OUT;
    if (!mff_call_at(0x019B, 0xAAAE, 6)) return;
    SYNC_IN;
    mff_cycles(0xA19C, 8);
    a = RAM(OBJ_FLAGS + x);
    a &= 0x40;
    if (a != 0) { mff_cycles(0xA1A1, 1); goto L_A1A7; }
    mff_cycles(0xA1A3, 4);
    a = 0x01;
    if (a != 0) { mff_cycles(0xA1A5, 1); goto L_A1A9; }
L_A1A7:;
    mff_cycles(0xA1A7, 2);
    a = 0x02;
L_A1A9:;
    mff_cycles(0xA1A9, 44);
    RAM(OBJ_DIR + x) = a;
    a = 0x02;
    RAM(OBJ_VX + x) = a;
    a = 0xE0;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0x00;
    RAM(OBJ_VY + x) = a;
    RAM(OBJ_VY_FRAC + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_A1C2:;
    mff_cycles(0xA1C2, 10);
    a = 0x9D;
    y = 0x3D;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x01C8, func_F449, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA1C9, 6);
    SYNC_OUT;
    if (!mff_call_at(0x01CB, 0xA9F6, 6)) return;
    SYNC_IN;
    mff_cycles(0xA1CC, 2);
    if (c) {
        mff_cycles(0xA1CE, 13);
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_PIT_FLAG + x) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xA1CC, 1);
    }
    mff_cycles(0xA1D4, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    if (t != 0) {
        mff_cycles(0xA1D9, 8);
        a = RAM(OBJ_DIR + x);
        a &= 0x01;
        n = (a) >> 7;
        z = (a) == 0;
        if (!z) {
            mff_cycles(0xA1E0, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xF183, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F183(); g_code_window_base = swb; } return;
        } else {
            mff_cycles(0xA1DE, 1);
        }
        mff_cycles(0xA1E3, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF1A1, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F1A1(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xA1D7, 1);
    }
    mff_cycles(0xA1E6, 14);
    a = 0x20;
    RAM(OBJ_AI_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
L_A1EE:;
    mff_cycles(0xA1EE, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A1EF:;
    mff_cycles(0xA1EF, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) { mff_cycles(0xA1EE, 1); goto L_A1EE; }
    mff_cycles(0xA1F4, 7);
    a = RAM(RNG);
    t = a;
    c = t & 1;
    t >>= 1;
    if (!c) { mff_cycles(0xA1F7, 1); goto L_A1FD; }
    mff_cycles(0xA1F9, 4);
    a = 0x84;
    if (a != 0) { mff_cycles(0xA1FB, 1); goto L_A1FF; }
L_A1FD:;
    mff_cycles(0xA1FD, 2);
    a = 0x81;
L_A1FF:;
    mff_cycles(0xA1FF, 25);
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    RAM(OBJ_VX_FRAC + x) = a;
    RAM(OBJ_VX + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAA1D, 2); call_by_address_tail(0xAA1D, 6); return;
}

/*  $A210 (bank 6) - lifted by port/tools/lift.py */
void func_A210_b6(void) {
    REGS_IN;
    mff_cycles(0xA210, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0212, 0xA9EB, 6)) return;
    SYNC_IN;
    mff_cycles(0xA213, 6);
    a = RAM(OBJ_AI_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0xA218, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA2A6, 2); call_by_address_tail(0xA2A6, 6); return;
    } else {
        mff_cycles(0xA216, 1);
    }
    mff_cycles(0xA21B, 8);
    y = RAM(OBJ_KIND + x);
    a = prg_read((uint16_t)(0xA3A7 + y));
    mff_cycles(0xA221, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0xA3AD + y));
    mff_cycles(0xA226, 11);
    y = a;
    a = RAM(TMP0);
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x022B, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA22C, 2);
    if (!c) {
        mff_cycles(0xA22E, 9);
        t = (uint8_t)(RAM(OBJ_AI_COUNT + x) - 1);
        RAM(OBJ_AI_COUNT + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
        if (!z) {
            mff_cycles(0xA233, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0235, func_FB72, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xA236, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xA088, 2); call_by_address_tail(0xA088, 6); return;
        } else {
            mff_cycles(0xA231, 1);
        }
        mff_cycles(0xA239, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA8E4, 2); call_by_address_tail(0xA8E4, 6); return;
    } else {
        mff_cycles(0xA22C, 1);
    }
    mff_cycles(0xA23C, 8);
    y = RAM(OBJ_KIND + x);
    if (y == 0x03) {
        mff_cycles(0xA243, 7);
        a = RAM(GAME_ACTION);
        m = 0x01;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) {
            mff_cycles(0xA249, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xA8E8, 2); call_by_address_tail(0xA8E8, 6); return;
        } else {
            mff_cycles(0xA247, 1);
        }
    } else {
        mff_cycles(0xA241, 1);
    }
    mff_cycles(0xA24C, 9);
    a = RAM(RNG);
    a &= 0x0F;
    m = prg_read((uint16_t)(0xA2BD + y));
    c = a >= m;
    mff_cycles(0xA253, 2);
    if (!c) { mff_cycles(0xA253, 1); goto L_A2B2; }
    mff_cycles(0xA255, 15);
    a = 0x00;
    RAM(OBJ_AI_COUNT + x) = a;
    y = RAM(OBJ_KIND + x);
    a = prg_read((uint16_t)(0xA2AC + y));
    z = (a) == 0;
    mff_cycles(0xA260, 2);
    if (z) { mff_cycles(0xA260, 1); goto L_A267; }
    mff_cycles(0xA262, 7);
    RAM(OBJ_AI_COUNT + x) = a;
    if (!z) { mff_cycles(0xA265, 1); goto L_A290; }
L_A267:;
    mff_cycles(0xA267, 6);
    a = RAM(OBJ_KIND + x);
    if (a != 0) {
        mff_cycles(0xA26C, 2);
        a = 0x03;
    } else {
        mff_cycles(0xA26A, 1);
    }
    mff_cycles(0xA26E, 11);
    RAM(TMP0) = a;
    y = RAM(OBJ_TIER + x);
    a = prg_read((uint16_t)(0xA897 + y));
    mff_cycles(0xA276, 11);
    c = 0;
    m = RAM(TMP0);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    y = a;
    a = prg_read((uint16_t)(0xAE2C + y));
    mff_cycles(0xA27D, 13);
    RAM(TMP0) = a;
    a = RAM(RNG);
    a &= 0x0F;
    m = RAM(TMP0);
    c = a >= m;
    if (c) {
        mff_cycles(0xA287, 14);
        t = (uint8_t)(RAM(OBJ_AI_COUNT + x) + 1);
        RAM(OBJ_AI_COUNT + x) = t;
        t = (uint8_t)(RAM(OBJ_AI_COUNT + x) + 1);
        RAM(OBJ_AI_COUNT + x) = t;
    } else {
        mff_cycles(0xA285, 1);
    }
    mff_cycles(0xA28D, 7);
    t = (uint8_t)(RAM(OBJ_AI_COUNT + x) + 1);
    RAM(OBJ_AI_COUNT + x) = t;
L_A290:;
    mff_cycles(0xA290, 13);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0297, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA298, 25);
    a = 0xFF;
    RAM(OBJ_TIMER3 + x) = a;
    RAM(OBJ_LAND_Y + x) = a;
    a = 0x85;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_A2B2:;
    mff_cycles(0xA2B2, 20);
    a = 0x1E;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x87;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A2A6 (bank 6) - lifted by port/tools/lift.py */
void func_A2A6_b6(void) {
    REGS_IN;
    mff_cycles(0xA2A6, 10);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAAAE, 2); call_by_address_tail(0xAAAE, 6); return;
}

/*  $A2C3 (bank 6) - lifted by port/tools/lift.py */
void func_A2C3_b6(void) {
    REGS_IN;
    mff_cycles(0xA2C3, 6);
    a = RAM(OBJ_AI_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0xA2C8, 10);
        t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
        RAM(OBJ_AI_TIMER + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xAAAE, 2); call_by_address_tail(0xAAAE, 6); return;
    } else {
        mff_cycles(0xA2C6, 1);
    }
    mff_cycles(0xA2CE, 6);
    SYNC_OUT;
    if (!mff_call_at(0x02D0, 0xAB00, 6)) return;
    SYNC_IN;
    mff_cycles(0xA2D1, 8);
    a = RAM(OBJ_KIND + x);
    m = 0x05;
    c = a >= m;
    if (a != m) { mff_cycles(0xA2D6, 1); goto L_A2E6; }
    mff_cycles(0xA2D8, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x06;
    c = a >= m;
    if (a != m) { mff_cycles(0xA2DD, 1); goto L_A2F7; }
    mff_cycles(0xA2DF, 6);
    a = RAM(OBJ_ANIM_TIMER + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xA2E2, 1); goto L_A2F7; }
    mff_cycles(0xA2E4, 2);
    if (z) { mff_cycles(0xA2E4, 2); goto L_A34D; }
L_A2E6:;
    mff_cycles(0xA2E6, 10);
    a = RAM(OBJ_ANIM_TIMER + x);
    a |= RAM(OBJ_ANIM_FRAME + x);
    if (a != 0) { mff_cycles(0xA2EC, 1); goto L_A2F7; }
    mff_cycles(0xA2EE, 6);
    a = RAM(OBJ_TIMER3 + x);
    if (a == 0) { mff_cycles(0xA2F1, 2); goto L_A34D; }
    mff_cycles(0xA2F3, 13);
    t = (uint8_t)(RAM(OBJ_TIMER3 + x) + 1);
    RAM(OBJ_TIMER3 + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_A2F7:;
    mff_cycles(0xA2F7, 8);
    a = RAM(OBJ_ANIM_TIMER + x);
    m = 0x01;
    c = a >= m;
    if (a != m) { mff_cycles(0xA2FC, 2); goto L_A309; }
    mff_cycles(0xA2FE, 8);
    y = RAM(OBJ_KIND + x);
    a = prg_read((uint16_t)(0xA3A1 + y));
    mff_cycles(0xA304, 6);
    m = RAM(OBJ_ANIM_FRAME + x);
    c = a >= m;
    if (a == m) { mff_cycles(0xA307, 1); goto L_A324; }
L_A309:;
    mff_cycles(0xA309, 6);
    a = RAM(OBJ_HIT_PLAYER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xA30C, 1); goto L_A323; }
    mff_cycles(0xA30E, 8);
    a = RAM(OBJ_KIND + x);
    m = 0x04;
    c = a >= m;
    if (a != m) { mff_cycles(0xA313, 1); goto L_A319; }
    mff_cycles(0xA315, 4);
    a = 0x1F;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA317, 1); goto L_A31B; }
L_A319:;
    mff_cycles(0xA319, 2);
    a = 0x18;
    n = (a) >> 7;
    z = (a) == 0;
L_A31B:;
    mff_cycles(0xA31B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x031D, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA31E, 7);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_HIT_PLAYER + x) = a;
L_A323:;
    mff_cycles(0xA323, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A324:;
    mff_cycles(0xA324, 18);
    a = RAM(OBJ_ID + x);
    c = 1;
    m = 0x20;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    y = a;
    a = RAM(OBJ_KIND + x);
    m = 0x03;
    c = a >= m;
    if (a == m) { mff_cycles(0xA330, 1); goto L_A337; }
    mff_cycles(0xA332, 6);
    a = RAM(OBJ_LAND_Y + x);
    if (a != 0) { mff_cycles(0xA335, 1); goto L_A33D; }
L_A337:;
    mff_cycles(0xA337, 4);
    a = prg_read((uint16_t)(0xA3C9 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA33A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA340, 2); call_by_address_tail(0xA340, 6); return;
L_A33D:;
    mff_cycles(0xA33D, 4);
    a = prg_read((uint16_t)(0xA3B3 + y));
    mff_cycles(0xA340, 24);
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = RAM(OBJ_KIND + x);
    c = 0;
    m = 0x80;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ATTACK + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_A34D:;
    mff_cycles(0xA34D, 9);
    t = (uint8_t)(RAM(OBJ_AI_COUNT + x) - 1);
    RAM(OBJ_AI_COUNT + x) = t;
    if (t == 0) {
        mff_cycles(0xA352, 32);
        a = 0x00;
        RAM(OBJ_LAND_Y + x) = a;
        RAM(OBJ_TIMER3 + x) = a;
        a = 0x0F;
        RAM(OBJ_AI_TIMER + x) = a;
        a = 0x87;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_STATE + x) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xA350, 1);
    }
    mff_cycles(0xA365, 8);
    y = RAM(OBJ_KIND + x);
    a = prg_read((uint16_t)(0xA3A7 + y));
    mff_cycles(0xA36B, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0xA3AD + y));
    mff_cycles(0xA370, 11);
    y = a;
    a = RAM(TMP0);
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0375, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA376, 2);
    if (c) { mff_cycles(0xA376, 1); goto L_A383; }
L_A378:;
    mff_cycles(0xA378, 15);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_LAND_Y + x) = a;
    RAM(OBJ_TIMER3 + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA8E8, 2); call_by_address_tail(0xA8E8, 6); return;
L_A383:;
    mff_cycles(0xA383, 7);
    a = RAM(GAME_ACTION);
    m = 0x06;
    c = a >= m;
    if (a == m) { mff_cycles(0xA378, 1); goto L_A378; }
    mff_cycles(0xA389, 4);
    m = 0x1F;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA378, 1); goto L_A378; }
    mff_cycles(0xA38D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x038F, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA390, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0392, 0xAAA0, 6)) return;
    SYNC_IN;
    mff_cycles(0xA393, 15);
    t = (uint8_t)(RAM(OBJ_TIMER3 + x) - 1);
    RAM(OBJ_TIMER3 + x) = t;
    a = RAM(OBJ_AI_COUNT + x);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xA39D, 7);
        t = (uint8_t)(RAM(OBJ_LAND_Y + x) + 1);
        RAM(OBJ_LAND_Y + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
    } else {
        mff_cycles(0xA39B, 1);
    }
    mff_cycles(0xA3A0, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A340 (bank 6) - lifted by port/tools/lift.py */
void func_A340_b6(void) {
    REGS_IN;
    mff_cycles(0xA340, 24);
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = RAM(OBJ_KIND + x);
    c = 0;
    m = 0x80;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ATTACK + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A3F5 (bank 6) - lifted by port/tools/lift.py */
void func_A3F5_b6(void) {
    REGS_IN;
    mff_cycles(0xA3F5, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!n) {
        mff_cycles(0xA3FA, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x03FC, func_FB5B, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA3FD, 6);
        y = a;
        a = prg_read((uint16_t)(0xAE3E + y));
        mff_cycles(0xA401, 6);
        y = a;
        a = prg_read((uint16_t)(0xFBF0 + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0xA405, 11);
        RAM(OBJ_DIR + x) = a;
        SYNC_OUT;
        if (!mff_call_at(0x040A, 0xAC34, 6)) return;
        SYNC_IN;
        mff_cycles(0xA40B, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA953, 2); call_by_address_tail(0xA953, 6); return;
    } else {
        mff_cycles(0xA3F8, 2);
    }
    mff_cycles(0xA40E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA8E8, 2); call_by_address_tail(0xA8E8, 6); return;
}

/*  $A411 (bank 6) - lifted by port/tools/lift.py */
void func_A411_b6(void) {
    REGS_IN;
    mff_cycles(0xA411, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0413, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA414, 9);
    t = (uint8_t)(RAM(OBJ_HIT_TIMER + x) - 1);
    RAM(OBJ_HIT_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (z) {
        mff_cycles(0xA419, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA8E8, 2); call_by_address_tail(0xA8E8, 6); return;
    } else {
        mff_cycles(0xA417, 1);
    }
    mff_cycles(0xA41C, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A41D (bank 6) - lifted by port/tools/lift.py */
void func_A41D_b6(void) {
    REGS_IN;
    mff_cycles(0xA41D, 6);
    a = RAM(OBJ_KIND + x);
    if (a != 0) { mff_cycles(0xA420, 1); goto L_A428; }
    mff_cycles(0xA422, 6);
    a = 0x16;
    y = 0x08;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0xA426, 1); goto L_A42C; }
L_A428:;
    mff_cycles(0xA428, 4);
    a = 0x37;
    y = 0x08;
    n = (y) >> 7;
    z = (y) == 0;
L_A42C:;
    mff_cycles(0xA42C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x042E, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA42F, 2);
    if (!c) {
        mff_cycles(0xA431, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA085, 2); call_by_address_tail(0xA085, 6); return;
    } else {
        mff_cycles(0xA42F, 1);
    }
    mff_cycles(0xA434, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0436, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA437, 15);
    a = RAM(OBJ_Y + x);
    RAM(OBJ_GROUND_Y + x) = a;
    a = RAM(OBJ_KIND + x);
    if (a != 0) { mff_cycles(0xA440, 1); goto L_A469; }
    mff_cycles(0xA442, 36);
    a = 0x00;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0x01;
    RAM(OBJ_VX + x) = a;
    a = 0x00;
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x04;
    RAM(OBJ_VY + x) = a;
    y = RAM(OBJ_TIER + x);
    a = prg_read((uint16_t)(0xA897 + y));
    mff_cycles(0xA45C, 18);
    c = 0;
    m = 0x36;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = 0x08;
    RAM(OBJ_AI_TIMER + x) = a;
    if (a != 0) { mff_cycles(0xA467, 1); goto L_A48E; }
L_A469:;
    mff_cycles(0xA469, 36);
    a = 0x00;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0x03;
    RAM(OBJ_VX + x) = a;
    a = 0x80;
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x03;
    RAM(OBJ_VY + x) = a;
    y = RAM(OBJ_TIER + x);
    a = prg_read((uint16_t)(0xA897 + y));
    mff_cycles(0xA483, 16);
    c = 0;
    m = 0x39;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = 0x10;
    RAM(OBJ_AI_TIMER + x) = a;
L_A48E:;
    mff_cycles(0xA48E, 13);
    a = 0x89;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A494 (bank 6) - lifted by port/tools/lift.py */
void func_A494_b6(void) {
    REGS_IN;
    mff_cycles(0xA494, 6);
    a = RAM(OBJ_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0xA499, 6);
        a = RAM(OBJ_AI_TIMER + x);
        if (a != 0) {
            mff_cycles(0xA49E, 13);
            t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
            RAM(OBJ_AI_TIMER + x) = t;
            a = RAM(OBJ_KIND + x);
            n = (a) >> 7;
            z = (a) == 0;
            if (z) {
                mff_cycles(0xA4A6, 7);
                a = 0xE8;
                y = 0x18;
                n = (y) >> 7;
                z = (y) == 0;
                SYNC_OUT;
                nes_cpu_instruction_boundary(0xF449, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F449(); g_code_window_base = swb; } return;
            } else {
                mff_cycles(0xA4A4, 1);
            }
            mff_cycles(0xA4AD, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x9CAD, 2); call_by_address_tail(0x9CAD, 6); return;
        } else {
            mff_cycles(0xA49C, 1);
        }
        mff_cycles(0xA4B0, 13);
        t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        RAM(OBJ_TIMER + x) = t;
        a = RAM(OBJ_KIND + x);
        if (a == 0) {
            mff_cycles(0xA4B8, 7);
            a = 0x84;
            y = 0x1A;
            n = (y) >> 7;
            z = (y) == 0;
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xF449, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F449(); g_code_window_base = swb; } return;
        } else {
            mff_cycles(0xA4B6, 1);
        }
        mff_cycles(0xA4BF, 7);
        a = 0xAC;
        y = 0x22;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF44F, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F44F(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xA497, 1);
    }
    mff_cycles(0xA4C6, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x04C8, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA4C9, 10);
    a = RAM(OBJ_Y + x);
    m = RAM(OBJ_GROUND_Y + x);
    c = a >= m;
    if (c) {
        mff_cycles(0xA4D1, 15);
        a = RAM(OBJ_GROUND_Y + x);
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_Y + x) = a;
        SYNC_OUT;
        if (!mff_call_at(0x04D9, 0xA9F6, 6)) return;
        SYNC_IN;
        mff_cycles(0xA4DA, 2);
        if (c) {
            mff_cycles(0xA4DC, 15);
            a = RAM(OBJ_KIND + x);
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_PIT_FLAG + x) = a;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xA4DA, 1);
        }
        mff_cycles(0xA4E3, 34);
        a = 0x00;
        RAM(OBJ_VY_FRAC + x) = a;
        RAM(OBJ_VY + x) = a;
        RAM(OBJ_ATTACK + x) = a;
        a = 0x8C;
        RAM(OBJ_STATE + x) = a;
        a = 0x08;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_AI_TIMER + x) = a;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x9CAD, 2); call_by_address_tail(0x9CAD, 6); return;
    } else {
        mff_cycles(0xA4CF, 1);
    }
    mff_cycles(0xA4FB, 10);
    a = RAM(OBJ_DIR + x);
    y = RAM(OBJ_KIND + x);
    if (y == 0) {
        mff_cycles(0xA503, 2);
        a ^= 0xFF;
    } else {
        mff_cycles(0xA501, 1);
    }
    mff_cycles(0xA505, 4);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0xA509, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x050B, func_F183, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA50C, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA512, 2); call_by_address_tail(0xA512, 6); return;
    } else {
        mff_cycles(0xA507, 1);
    }
    mff_cycles(0xA50F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0511, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA512, 7);
    a = RAM(ROUND);
    m = 0x03;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xA518, 7);
        a = RAM(SUB_AREA);
        m = 0x01;
        c = a >= m;
        if (a == m) {
            mff_cycles(0xA51E, 8);
            a = 0x08;
            if (a >= RAM(OBJ_X + x)) {
                mff_cycles(0xA525, 5);
                RAM(OBJ_X + x) = a;
            } else {
                mff_cycles(0xA523, 1);
            }
            mff_cycles(0xA528, 8);
            a = 0xF8;
            m = RAM(OBJ_X + x);
            c = a >= m;
            if (!c) {
                mff_cycles(0xA52F, 5);
                RAM(OBJ_X + x) = a;
            } else {
                mff_cycles(0xA52D, 1);
            }
        } else {
            mff_cycles(0xA51C, 1);
        }
    } else {
        mff_cycles(0xA516, 1);
    }
    mff_cycles(0xA532, 6);
    a = RAM(OBJ_KIND + x);
    if (a != 0) { mff_cycles(0xA535, 1); goto L_A542; }
    mff_cycles(0xA537, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x07;
    c = a >= m;
    if (a == m) { mff_cycles(0xA53C, 1); goto L_A548; }
    mff_cycles(0xA53E, 4);
    a = 0x89;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA540, 1); goto L_A544; }
L_A542:;
    mff_cycles(0xA542, 2);
    a = 0x8A;
    n = (a) >> 7;
    z = (a) == 0;
L_A544:;
    mff_cycles(0xA544, 11);
    RAM(OBJ_ATTACK + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_A548:;
    mff_cycles(0xA548, 9);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ANIM_TIMER + x) = a;
    if (z) { mff_cycles(0xA544, 1); goto L_A544; }
    mff_cycles(0xA54F, 8);
    a = RAM(OBJ_TIMER + x);
    if (a == 0x01) { mff_cycles(0xA554, 1); goto L_A593; }
    mff_cycles(0xA556, 4);
    m = 0x02;
    c = a >= m;
    if (a == m) { mff_cycles(0xA558, 1); goto L_A5CD; }
    mff_cycles(0xA55A, 9);
    a = RAM(PLAYER_GROUND_Y);
    m = RAM(OBJ_Y + x);
    c = a >= m;
    if (c) { mff_cycles(0xA55F, 1); goto L_A565; }
    mff_cycles(0xA561, 4);
    a = 0x08;
    if (a != 0) { mff_cycles(0xA563, 1); goto L_A567; }
L_A565:;
    mff_cycles(0xA565, 2);
    a = 0x04;
L_A567:;
    mff_cycles(0xA567, 13);
    RAM(OBJ_DIR + x) = a;
    a = RAM(OBJ_FLAGS + x);
    a &= 0x40;
    if (a != 0) { mff_cycles(0xA56F, 1); goto L_A578; }
    mff_cycles(0xA571, 8);
    a = RAM(OBJ_DIR + x);
    a |= 0x01;
    if (a != 0) { mff_cycles(0xA576, 1); goto L_A57D; }
L_A578:;
    mff_cycles(0xA578, 6);
    a = RAM(OBJ_DIR + x);
    a |= 0x02;
L_A57D:;
    mff_cycles(0xA57D, 13);
    RAM(OBJ_DIR + x) = a;
    y = 0x02;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0584, 0xAC34, 6)) return;
    SYNC_IN;
    mff_cycles(0xA585, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0587, 0xA953, 6)) return;
    SYNC_IN;
    mff_cycles(0xA588, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x058A, func_FBD9, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA58B, 4);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0xA58F, 7);
        t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        RAM(OBJ_TIMER + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
    } else {
        mff_cycles(0xA58D, 1);
    }
L_A592:;
    mff_cycles(0xA592, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A593:;
    mff_cycles(0xA593, 8);
    a = 0xA7;
    m = RAM(OBJ_ANIM + x);
    c = a >= m;
    if (a != m) {
        mff_cycles(0xA59A, 8);
        y = 0x20;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x059E, func_F44F, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA59F, 7);
        t = (uint8_t)(RAM(OBJ_ANIM_TIMER + x) + 1);
        RAM(OBJ_ANIM_TIMER + x) = t;
    } else {
        mff_cycles(0xA598, 1);
    }
    mff_cycles(0xA5A2, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x04;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xA5A9, 6);
        a = RAM(OBJ_ANIM_TIMER + x);
        if (a == 0) {
            mff_cycles(0xA5AE, 10);
            t = (uint8_t)(RAM(OBJ_ANIM_TIMER + x) + 1);
            RAM(OBJ_ANIM_TIMER + x) = t;
            n = (t) >> 7;
            z = (t) == 0;
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xA5DA, 2); call_by_address_tail(0xA5DA, 6); return;
        } else {
            mff_cycles(0xA5AC, 1);
        }
    } else {
        mff_cycles(0xA5A7, 1);
    }
    mff_cycles(0xA5B4, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x05;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA592, 1); goto L_A592; }
    mff_cycles(0xA5BB, 8);
    a = RAM(OBJ_ANIM_TIMER + x);
    m = 0x04;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA592, 1); goto L_A592; }
    mff_cycles(0xA5C2, 17);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    a = 0x30;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAAAE, 2); call_by_address_tail(0xAAAE, 6); return;
L_A5CD:;
    mff_cycles(0xA5CD, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) { mff_cycles(0xA592, 1); goto L_A592; }
    mff_cycles(0xA5D2, 10);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA8E4, 2); call_by_address_tail(0xA8E4, 6); return;
}

/*  $A512 (bank 6) - lifted by port/tools/lift.py */
void func_A512_b6(void) {
    REGS_IN;
    mff_cycles(0xA512, 7);
    a = RAM(ROUND);
    m = 0x03;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xA518, 7);
        a = RAM(SUB_AREA);
        m = 0x01;
        c = a >= m;
        if (a == m) {
            mff_cycles(0xA51E, 8);
            a = 0x08;
            if (a >= RAM(OBJ_X + x)) {
                mff_cycles(0xA525, 5);
                RAM(OBJ_X + x) = a;
            } else {
                mff_cycles(0xA523, 1);
            }
            mff_cycles(0xA528, 8);
            a = 0xF8;
            m = RAM(OBJ_X + x);
            c = a >= m;
            if (!c) {
                mff_cycles(0xA52F, 5);
                RAM(OBJ_X + x) = a;
            } else {
                mff_cycles(0xA52D, 1);
            }
        } else {
            mff_cycles(0xA51C, 1);
        }
    } else {
        mff_cycles(0xA516, 1);
    }
    mff_cycles(0xA532, 6);
    a = RAM(OBJ_KIND + x);
    if (a != 0) { mff_cycles(0xA535, 1); goto L_A542; }
    mff_cycles(0xA537, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x07;
    c = a >= m;
    if (a == m) { mff_cycles(0xA53C, 1); goto L_A548; }
    mff_cycles(0xA53E, 4);
    a = 0x89;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA540, 1); goto L_A544; }
L_A542:;
    mff_cycles(0xA542, 2);
    a = 0x8A;
    n = (a) >> 7;
    z = (a) == 0;
L_A544:;
    mff_cycles(0xA544, 11);
    RAM(OBJ_ATTACK + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_A548:;
    mff_cycles(0xA548, 9);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ANIM_TIMER + x) = a;
    if (z) { mff_cycles(0xA544, 1); goto L_A544; }
    mff_cycles(0xA54F, 8);
    a = RAM(OBJ_TIMER + x);
    if (a == 0x01) { mff_cycles(0xA554, 1); goto L_A593; }
    mff_cycles(0xA556, 4);
    m = 0x02;
    c = a >= m;
    if (a == m) { mff_cycles(0xA558, 1); goto L_A5CD; }
    mff_cycles(0xA55A, 9);
    a = RAM(PLAYER_GROUND_Y);
    m = RAM(OBJ_Y + x);
    c = a >= m;
    if (c) { mff_cycles(0xA55F, 1); goto L_A565; }
    mff_cycles(0xA561, 4);
    a = 0x08;
    if (a != 0) { mff_cycles(0xA563, 1); goto L_A567; }
L_A565:;
    mff_cycles(0xA565, 2);
    a = 0x04;
L_A567:;
    mff_cycles(0xA567, 13);
    RAM(OBJ_DIR + x) = a;
    a = RAM(OBJ_FLAGS + x);
    a &= 0x40;
    if (a != 0) { mff_cycles(0xA56F, 1); goto L_A578; }
    mff_cycles(0xA571, 8);
    a = RAM(OBJ_DIR + x);
    a |= 0x01;
    if (a != 0) { mff_cycles(0xA576, 1); goto L_A57D; }
L_A578:;
    mff_cycles(0xA578, 6);
    a = RAM(OBJ_DIR + x);
    a |= 0x02;
L_A57D:;
    mff_cycles(0xA57D, 13);
    RAM(OBJ_DIR + x) = a;
    y = 0x02;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0584, 0xAC34, 6)) return;
    SYNC_IN;
    mff_cycles(0xA585, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0587, 0xA953, 6)) return;
    SYNC_IN;
    mff_cycles(0xA588, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x058A, func_FBD9, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA58B, 4);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0xA58F, 7);
        t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        RAM(OBJ_TIMER + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
    } else {
        mff_cycles(0xA58D, 1);
    }
L_A592:;
    mff_cycles(0xA592, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A593:;
    mff_cycles(0xA593, 8);
    a = 0xA7;
    m = RAM(OBJ_ANIM + x);
    c = a >= m;
    if (a != m) {
        mff_cycles(0xA59A, 8);
        y = 0x20;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x059E, func_F44F, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA59F, 7);
        t = (uint8_t)(RAM(OBJ_ANIM_TIMER + x) + 1);
        RAM(OBJ_ANIM_TIMER + x) = t;
    } else {
        mff_cycles(0xA598, 1);
    }
    mff_cycles(0xA5A2, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x04;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xA5A9, 6);
        a = RAM(OBJ_ANIM_TIMER + x);
        if (a == 0) {
            mff_cycles(0xA5AE, 10);
            t = (uint8_t)(RAM(OBJ_ANIM_TIMER + x) + 1);
            RAM(OBJ_ANIM_TIMER + x) = t;
            n = (t) >> 7;
            z = (t) == 0;
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xA5DA, 2); call_by_address_tail(0xA5DA, 6); return;
        } else {
            mff_cycles(0xA5AC, 1);
        }
    } else {
        mff_cycles(0xA5A7, 1);
    }
    mff_cycles(0xA5B4, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x05;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA592, 1); goto L_A592; }
    mff_cycles(0xA5BB, 8);
    a = RAM(OBJ_ANIM_TIMER + x);
    m = 0x04;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA592, 1); goto L_A592; }
    mff_cycles(0xA5C2, 17);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    a = 0x30;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAAAE, 2); call_by_address_tail(0xAAAE, 6); return;
L_A5CD:;
    mff_cycles(0xA5CD, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) { mff_cycles(0xA592, 1); goto L_A592; }
    mff_cycles(0xA5D2, 10);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA8E4, 2); call_by_address_tail(0xA8E4, 6); return;
}

/*  $A54F (bank 6) - lifted by port/tools/lift.py */
void func_A54F_b6(void) {
    REGS_IN;
    mff_cycles(0xA54F, 8);
    a = RAM(OBJ_TIMER + x);
    if (a == 0x01) { mff_cycles(0xA554, 1); goto L_A593; }
    mff_cycles(0xA556, 4);
    m = 0x02;
    c = a >= m;
    if (a == m) { mff_cycles(0xA558, 1); goto L_A5CD; }
    mff_cycles(0xA55A, 9);
    a = RAM(PLAYER_GROUND_Y);
    m = RAM(OBJ_Y + x);
    c = a >= m;
    if (c) { mff_cycles(0xA55F, 1); goto L_A565; }
    mff_cycles(0xA561, 4);
    a = 0x08;
    if (a != 0) { mff_cycles(0xA563, 1); goto L_A567; }
L_A565:;
    mff_cycles(0xA565, 2);
    a = 0x04;
L_A567:;
    mff_cycles(0xA567, 13);
    RAM(OBJ_DIR + x) = a;
    a = RAM(OBJ_FLAGS + x);
    a &= 0x40;
    if (a != 0) { mff_cycles(0xA56F, 1); goto L_A578; }
    mff_cycles(0xA571, 8);
    a = RAM(OBJ_DIR + x);
    a |= 0x01;
    if (a != 0) { mff_cycles(0xA576, 1); goto L_A57D; }
L_A578:;
    mff_cycles(0xA578, 6);
    a = RAM(OBJ_DIR + x);
    a |= 0x02;
L_A57D:;
    mff_cycles(0xA57D, 13);
    RAM(OBJ_DIR + x) = a;
    y = 0x02;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0584, 0xAC34, 6)) return;
    SYNC_IN;
    mff_cycles(0xA585, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0587, 0xA953, 6)) return;
    SYNC_IN;
    mff_cycles(0xA588, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x058A, func_FBD9, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA58B, 4);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0xA58F, 7);
        t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        RAM(OBJ_TIMER + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
    } else {
        mff_cycles(0xA58D, 1);
    }
L_A592:;
    mff_cycles(0xA592, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A593:;
    mff_cycles(0xA593, 8);
    a = 0xA7;
    m = RAM(OBJ_ANIM + x);
    c = a >= m;
    if (a != m) {
        mff_cycles(0xA59A, 8);
        y = 0x20;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x059E, func_F44F, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA59F, 7);
        t = (uint8_t)(RAM(OBJ_ANIM_TIMER + x) + 1);
        RAM(OBJ_ANIM_TIMER + x) = t;
    } else {
        mff_cycles(0xA598, 1);
    }
    mff_cycles(0xA5A2, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x04;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xA5A9, 6);
        a = RAM(OBJ_ANIM_TIMER + x);
        if (a == 0) {
            mff_cycles(0xA5AE, 10);
            t = (uint8_t)(RAM(OBJ_ANIM_TIMER + x) + 1);
            RAM(OBJ_ANIM_TIMER + x) = t;
            n = (t) >> 7;
            z = (t) == 0;
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xA5DA, 2); call_by_address_tail(0xA5DA, 6); return;
        } else {
            mff_cycles(0xA5AC, 1);
        }
    } else {
        mff_cycles(0xA5A7, 1);
    }
    mff_cycles(0xA5B4, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x05;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA592, 1); goto L_A592; }
    mff_cycles(0xA5BB, 8);
    a = RAM(OBJ_ANIM_TIMER + x);
    m = 0x04;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA592, 1); goto L_A592; }
    mff_cycles(0xA5C2, 17);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    a = 0x30;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAAAE, 2); call_by_address_tail(0xAAAE, 6); return;
L_A5CD:;
    mff_cycles(0xA5CD, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) { mff_cycles(0xA592, 1); goto L_A592; }
    mff_cycles(0xA5D2, 10);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA8E4, 2); call_by_address_tail(0xA8E4, 6); return;
}

/*  $A5DA (bank 6) - lifted by port/tools/lift.py */
void func_A5DA_b6(void) {
    REGS_IN;
    mff_cycles(0xA5DA, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x05DC, func_F71A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA5DD, 2);
    if (c) { mff_cycles(0xA5DD, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0592), 6); return; }
    mff_cycles(0xA5DF, 11);
    RAM(TMP0) = y;
    a = RAM(OBJ_FLAGS + x);
    a &= 0x40;
    if (a != 0) { mff_cycles(0xA5E6, 1); goto L_A5EC; }
    mff_cycles(0xA5E8, 4);
    a = 0x02;
    if (a != 0) { mff_cycles(0xA5EA, 1); goto L_A5EE; }
L_A5EC:;
    mff_cycles(0xA5EC, 2);
    a = 0x01;
L_A5EE:;
    mff_cycles(0xA5EE, 19);
    RAM(OBJ_DIR + y) = a;
    a &= 0x02;
    y = a;
    a = RAM(OBJ_X + x);
    c = 0;
    m = prg_read((uint16_t)(0xA64E + y));
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xA5FB, 11);
    mff_push(a);
    a = RAM(OBJ_X_HI + x);
    m = prg_read((uint16_t)(0xA64F + y));
    w = a + m + c;
    a = (uint8_t)w;
    mff_cycles(0xA602, 73);
    y = RAM(TMP0);
    RAM(OBJ_X_HI + y) = a;
    a = mff_pull();
    RAM(OBJ_X + y) = a;
    a = RAM(OBJ_Y + x);
    c = 0;
    m = 0x03;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_Y + y) = a;
    a = RAM(OBJ_Y_HI + x);
    RAM(OBJ_Y_HI + y) = a;
    a = 0x00;
    RAM(OBJ_LIFE + y) = a;
    RAM(OBJ_LIFE_MAX + y) = a;
    a = 0x30;
    RAM(OBJ_VX_FRAC + y) = a;
    a = 0x03;
    RAM(OBJ_VX + y) = a;
    a = 0xBB;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0630, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA631, 8);
    a = 0x1B;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0635, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA636, 39);
    a = 0x87;
    RAM(OBJ_ATTACK + y) = a;
    a = 0x06;
    RAM(OBJ_ATTACK_DAMAGE + y) = a;
    a = 0x2E;
    RAM(OBJ_ID + y) = a;
    RAM(OBJ_SAVED_ID + y) = a;
    a = 0x06;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_KIND + y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A652 (bank 6) - lifted by port/tools/lift.py */
void func_A652_b6(void) {
    REGS_IN;
    mff_cycles(0xA652, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0xA657, 1); goto L_A677; }
    mff_cycles(0xA659, 4);
    m = 0x02;
    c = a >= m;
    if (a == m) { mff_cycles(0xA65B, 1); goto L_A6B1; }
    mff_cycles(0xA65D, 10);
    a = 0x37;
    y = 0x06;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0663, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA664, 2);
    if (!c) {
        mff_cycles(0xA666, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA085, 2); call_by_address_tail(0xA085, 6); return;
    } else {
        mff_cycles(0xA664, 1);
    }
    mff_cycles(0xA669, 6);
    SYNC_OUT;
    if (!mff_call_at(0x066B, 0x9CAD, 6)) return;
    SYNC_IN;
    mff_cycles(0xA66C, 17);
    a = 0x18;
    RAM(OBJ_AI_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF343, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F343(); g_code_window_base = swb; } return;
L_A677:;
    mff_cycles(0xA677, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (z) {
        mff_cycles(0xA67C, 36);
        a = 0x03;
        RAM(OBJ_AI_TIMER + x) = a;
        a = 0x0E;
        RAM(OBJ_AI_COUNT + x) = a;
        a = 0x00;
        RAM(OBJ_VY_FRAC + x) = a;
        RAM(OBJ_VY + x) = a;
        a = 0xA6;
        y = 0x21;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0694, func_F44F, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA695, 4);
        a = prg_read(0xA707);
        mff_cycles(0xA698, 9);
        RAM(OBJ_VX_FRAC + x) = a;
        a = prg_read(0xA716);
        mff_cycles(0xA69E, 13);
        RAM(OBJ_VX + x) = a;
        y = RAM(OBJ_TIER + x);
        a = prg_read((uint16_t)(0xA897 + y));
        mff_cycles(0xA6A7, 16);
        c = 0;
        m = 0x30;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(OBJ_ATTACK_DAMAGE + x) = a;
        t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        RAM(OBJ_TIMER + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
    } else {
        mff_cycles(0xA67A, 1);
    }
    mff_cycles(0xA6B0, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A6B1:;
    mff_cycles(0xA6B1, 16);
    a = 0x86;
    RAM(OBJ_ATTACK + x) = a;
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    if (t != 0) { mff_cycles(0xA6B9, 1); goto L_A6EC; }
    mff_cycles(0xA6BB, 16);
    a = 0x03;
    RAM(OBJ_AI_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_AI_COUNT + x) - 1);
    RAM(OBJ_AI_COUNT + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) { mff_cycles(0xA6C3, 1); goto L_A6D8; }
    mff_cycles(0xA6C5, 14);
    a = 0x08;
    RAM(OBJ_AI_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
L_A6CD:;
    mff_cycles(0xA6CD, 15);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ATTACK + x) = a;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9CAD, 2); call_by_address_tail(0x9CAD, 6); return;
L_A6D8:;
    mff_cycles(0xA6D8, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06DA, 0xA9F6, 6)) return;
    SYNC_IN;
    mff_cycles(0xA6DB, 2);
    if (c) { mff_cycles(0xA6CD, 1); goto L_A6CD; }
    mff_cycles(0xA6DD, 8);
    y = RAM(OBJ_AI_COUNT + x);
    a = prg_read((uint16_t)(0xA6F9 + y));
    mff_cycles(0xA6E3, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = prg_read((uint16_t)(0xA708 + y));
    mff_cycles(0xA6E9, 5);
    RAM(OBJ_VX + x) = a;
L_A6EC:;
    mff_cycles(0xA6EC, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0xA6F3, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF183, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F183(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xA6F1, 1);
    }
    mff_cycles(0xA6F6, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF1A1, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F1A1(); g_code_window_base = swb; } return;
}

/*  $A717 (bank 6) - lifted by port/tools/lift.py */
void func_A717_b6(void) {
    REGS_IN;
    mff_cycles(0xA717, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (z) {
        mff_cycles(0xA71C, 6);
        SYNC_OUT;
        if (!mff_call_at(0x071E, 0xAAAE, 6)) return;
        SYNC_IN;
        mff_cycles(0xA71F, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0721, 0xAA1D, 6)) return;
        SYNC_IN;
        mff_cycles(0xA722, 6);
        a = RAM(OBJ_PIT_FLAG + x);
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0xA727, 14);
            a = 0x1E;
            RAM(OBJ_AI_TIMER + x) = a;
            a = 0x87;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_STATE + x) = a;
        } else {
            mff_cycles(0xA725, 1);
        }
    } else {
        mff_cycles(0xA71A, 1);
    }
    mff_cycles(0xA731, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A732 (bank 6) - lifted by port/tools/lift.py */
void func_A732_b6(void) {
    REGS_IN;
    goto L_A732;
L_A731:;
    mff_cycles(0xA731, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A732:;
    mff_cycles(0xA732, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) { mff_cycles(0xA731, 1); goto L_A731; }
    mff_cycles(0xA737, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA8E8, 2); call_by_address_tail(0xA8E8, 6); return;
}

/*  $A73A (bank 6) - lifted by port/tools/lift.py */
void func_A73A_b6(void) {
    REGS_IN;
    mff_cycles(0xA73A, 6);
    a = RAM(OBJ_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA73D, 1); goto L_A76F; }
    mff_cycles(0xA73F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0741, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA742, 2);
    if (c) { mff_cycles(0xA742, 1); goto L_A748; }
    mff_cycles(0xA744, 4);
    a = 0x01;
    if (a != 0) { mff_cycles(0xA746, 1); goto L_A74A; }
L_A748:;
    mff_cycles(0xA748, 2);
    a = 0x02;
L_A74A:;
    mff_cycles(0xA74A, 13);
    y = 0x0C;
    n = (y) >> 7;
    z = (y) == 0;
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0751, 0xAC34, 6)) return;
    SYNC_IN;
    mff_cycles(0xA752, 8);
    y = RAM(OBJ_TIER + x);
    a = prg_read((uint16_t)(0xA897 + y));
    mff_cycles(0xA758, 6);
    y = a;
    a = prg_read((uint16_t)(0xA88E + y));
    mff_cycles(0xA75C, 9);
    RAM(OBJ_AI_TIMER + x) = a;
    a = prg_read((uint16_t)(0xA894 + y));
    mff_cycles(0xA762, 9);
    RAM(OBJ_GROUND_Y + x) = a;
    a = prg_read((uint16_t)(0xA891 + y));
    mff_cycles(0xA768, 18);
    RAM(OBJ_AI_COUNT + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_A76F:;
    mff_cycles(0xA76F, 10);
    y = RAM(OBJ_TIMER + x);
    y--;
    a = prg_read((uint16_t)(0xA878 + y));
    mff_cycles(0xA776, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0xA87B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA77B, 8);
    RAM(TMP1) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $A780 (bank 6) - lifted by port/tools/lift.py */
void func_A780_b6(void) {
    REGS_IN;
    mff_cycles(0xA780, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (z) { mff_cycles(0xA783, 1); goto L_A7A9; }
    mff_cycles(0xA785, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0787, 0xA953, 6)) return;
    SYNC_IN;
    mff_cycles(0xA788, 6);
    SYNC_OUT;
    if (!mff_call_at(0x078A, 0xA9EB, 6)) return;
    SYNC_IN;
    mff_cycles(0xA78B, 10);
    a = RAM(OBJ_AI_TIMER + x);
    m = RAM(OBJ_AI_COUNT + x);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xA791, 1); goto L_A7AC; }
    mff_cycles(0xA793, 14);
    a = RAM(OBJ_AI_TIMER + x);
    a &= RAM(OBJ_GROUND_Y + x);
    a ^= RAM(OBJ_GROUND_Y + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA79C, 1); goto L_A7AC; }
    mff_cycles(0xA79E, 7);
    a = RAM(RNG);
    a &= 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xA7A2, 1); goto L_A7AC; }
    mff_cycles(0xA7A4, 7);
    a = 0x00;
    RAM(OBJ_AI_TIMER + x) = a;
L_A7A9:;
    mff_cycles(0xA7A9, 7);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
L_A7AC:;
    mff_cycles(0xA7AC, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A7AD (bank 6) - lifted by port/tools/lift.py */
void func_A7AD_b6(void) {
    REGS_IN;
    mff_cycles(0xA7AD, 6);
    a = RAM(OBJ_AI_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0xA7B2, 7);
        a = 0x04;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_AI_TIMER + x) = a;
    } else {
        mff_cycles(0xA7B0, 1);
    }
    mff_cycles(0xA7B7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07B9, 0xAAAE, 6)) return;
    SYNC_IN;
    mff_cycles(0xA7BA, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) { mff_cycles(0xA7BD, 2); goto L_A828; }
    mff_cycles(0xA7BF, 18);
    a = RAM(RNG);
    a &= 0x0F;
    RAM(TMP0) = a;
    a = RAM(OBJ_FLAGS);
    a ^= RAM(OBJ_FLAGS + x);
    if (a == 0) { mff_cycles(0xA7CB, 1); goto L_A7D1; }
    mff_cycles(0xA7CD, 4);
    a = 0x0C;
    if (a != 0) { mff_cycles(0xA7CF, 1); goto L_A7D3; }
L_A7D1:;
    mff_cycles(0xA7D1, 2);
    a = 0x0F;
L_A7D3:;
    mff_cycles(0xA7D3, 5);
    m = RAM(TMP0);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xA7D5, 2); goto L_A829; }
    mff_cycles(0xA7D7, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x07D9, func_FB5B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA7DA, 6);
    y = a;
    a = prg_read((uint16_t)(0xA87E + y));
    mff_cycles(0xA7DE, 9);
    RAM(OBJ_DIR + x) = a;
    a &= 0x0C;
    if (a != 0) { mff_cycles(0xA7E3, 1); goto L_A7F9; }
    mff_cycles(0xA7E5, 28);
    a = 0x4C;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0x02;
    RAM(OBJ_VX + x) = a;
    a = 0x00;
    RAM(OBJ_VY_FRAC + x) = a;
    RAM(OBJ_VY + x) = a;
    if (a == 0) { mff_cycles(0xA7F7, 2); goto L_A80D; }
L_A7F9:;
    mff_cycles(0xA7F9, 28);
    a = 0x1F;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0x02;
    RAM(OBJ_VX + x) = a;
    a = 0xE1;
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x00;
    RAM(OBJ_VY + x) = a;
L_A80D:;
    mff_cycles(0xA80D, 10);
    a = 0xD2;
    y = 0x24;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0813, func_F443, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA814, 8);
    y = RAM(OBJ_TIER + x);
    a = prg_read((uint16_t)(0xA897 + y));
    mff_cycles(0xA81A, 23);
    c = 0;
    m = 0x33;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = 0x01;
    RAM(OBJ_AI_COUNT + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
L_A828:;
    mff_cycles(0xA828, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A829:;
    mff_cycles(0xA829, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA8E8, 2); call_by_address_tail(0xA8E8, 6); return;
}

/*  $A82C (bank 6) - lifted by port/tools/lift.py */
void func_A82C_b6(void) {
    REGS_IN;
    mff_cycles(0xA82C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x082E, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA82F, 4);
    m = 0x30;
    c = a >= m;
    if (!c) {
        mff_cycles(0xA833, 10);
        a = 0x9A;
        y = 0x3E;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0839, func_F443, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xA831, 1);
    }
    mff_cycles(0xA83A, 13);
    a = 0x88;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ATTACK + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0841, 0xA953, 6)) return;
    SYNC_IN;
    mff_cycles(0xA842, 5);
    a = RAM(TMP15);
    if (a != 0) { mff_cycles(0xA844, 1); goto L_A86C; }
    mff_cycles(0xA846, 9);
    t = (uint8_t)(RAM(OBJ_AI_COUNT + x) - 1);
    RAM(OBJ_AI_COUNT + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (z) {
        mff_cycles(0xA84B, 8);
        a = 0x2A;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x084F, func_F34C, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA850, 7);
        a = 0x10;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_AI_COUNT + x) = a;
    } else {
        mff_cycles(0xA849, 1);
    }
    mff_cycles(0xA855, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0857, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA858, 8);
    a = RAM(OBJ_FLAGS + x);
    a &= 0x40;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA85D, 1); goto L_A863; }
    mff_cycles(0xA85F, 2);
    if (!c) { mff_cycles(0xA85F, 1); goto L_A877; }
    mff_cycles(0xA861, 2);
    if (c) { mff_cycles(0xA861, 1); goto L_A865; }
L_A863:;
    mff_cycles(0xA863, 2);
    if (c) { mff_cycles(0xA863, 1); goto L_A877; }
L_A865:;
    mff_cycles(0xA865, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0867, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA868, 4);
    m = 0x30;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xA86A, 1); goto L_A877; }
L_A86C:;
    mff_cycles(0xA86C, 15);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ATTACK + x) = a;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA8E8, 2); call_by_address_tail(0xA8E8, 6); return;
L_A877:;
    mff_cycles(0xA877, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A89C (bank 6) - lifted by port/tools/lift.py */
void func_A89C_b6(void) {
    REGS_IN;
    mff_cycles(0xA89C, 8);
    y = RAM(OBJ_KIND + x);
    a = prg_read((uint16_t)(0xA8DF + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA8A2, 11);
    RAM(OBJ_ATTACK + x) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x08A7, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA8A8, 10);
    a = RAM(OBJ_Y + x);
    m = RAM(OBJ_GROUND_Y + x);
    c = a >= m;
    if (c) {
        mff_cycles(0xA8B0, 55);
        a = RAM(OBJ_GROUND_Y + x);
        RAM(OBJ_Y + x) = a;
        a = 0x00;
        RAM(OBJ_VY_FRAC + x) = a;
        RAM(OBJ_VY + x) = a;
        RAM(OBJ_ATTACK + x) = a;
        RAM(OBJ_TIMER3 + x) = a;
        a = 0x08;
        RAM(OBJ_AI_TIMER + x) = a;
        a = 0x00;
        RAM(OBJ_TIMER + x) = a;
        a = 0x8C;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_STATE + x) = a;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x9CAD, 2); call_by_address_tail(0x9CAD, 6); return;
    } else {
        mff_cycles(0xA8AE, 1);
    }
    mff_cycles(0xA8D6, 6);
    a = RAM(OBJ_KIND + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0xA8DB, 5);
        RAM(OBJ_ANIM_TIMER + x) = a;
    } else {
        mff_cycles(0xA8D9, 1);
    }
    mff_cycles(0xA8DE, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A8E4 (bank 6) - lifted by port/tools/lift.py */
void func_A8E4_b6(void) {
    REGS_IN;
    mff_cycles(0xA8E4, 4);
    a = 0x08;
    if (a == 0) {
        mff_cycles(0xA8E8, 2);
        a = 0x00;
    } else {
        mff_cycles(0xA8E6, 1);
    }
    mff_cycles(0xA8EA, 18);
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x81;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A8E8 (bank 6) - lifted by port/tools/lift.py */
void func_A8E8_b6(void) {
    REGS_IN;
    mff_cycles(0xA8E8, 20);
    a = 0x00;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x81;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A8F3 (bank 6) - lifted by port/tools/lift.py */
void func_A8F3_b6(void) {
    REGS_IN;
    mff_cycles(0xA8F3, 7);
    a = RAM(TMP14);
    a &= 0x40;
    if (a != 0) { mff_cycles(0xA8F7, 2); goto L_A90C; }
    mff_cycles(0xA8F9, 8);
    a = RAM(OBJ_DIR + x);
    m = 0x08;
    c = a >= m;
    if (a == m) { mff_cycles(0xA8FE, 1); goto L_A944; }
    mff_cycles(0xA900, 4);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0xA902, 1); goto L_A944; }
    mff_cycles(0xA904, 8);
    y = 0x0C;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0908, 0xAC34, 6)) return;
    SYNC_IN;
    mff_cycles(0xA909, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA995, 2); call_by_address_tail(0xA995, 6); return;
L_A90C:;
    mff_cycles(0xA90C, 8);
    a = RAM(OBJ_STATE + x);
    m = 0x82;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xA913, 7);
        a = 0x81;
        RAM(OBJ_STATE + x) = a;
    } else {
        mff_cycles(0xA911, 1);
    }
    mff_cycles(0xA918, 19);
    a = RAM(OBJ_AI_FLAG + x);
    a ^= 0x01;
    RAM(OBJ_AI_FLAG + x) = a;
    y = 0x0C;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0924, 0xAC34, 6)) return;
    SYNC_IN;
    mff_cycles(0xA925, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0927, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA928, 2);
    if (!c) {
        mff_cycles(0xA92A, 15);
        a = 0x02;
        RAM(OBJ_DIR + x) = a;
        y = 0x00;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0933, func_F051, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA934, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA9E3, 2); call_by_address_tail(0xA9E3, 6); return;
    } else {
        mff_cycles(0xA928, 1);
    }
    mff_cycles(0xA937, 15);
    a = 0x01;
    RAM(OBJ_DIR + x) = a;
    y = 0x01;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0940, func_F000, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA941, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA9E3, 2); call_by_address_tail(0xA9E3, 6); return;
L_A944:;
    mff_cycles(0xA944, 20);
    a = RAM(OBJ_AI_FLAG + x);
    a ^= 0x01;
    RAM(OBJ_AI_FLAG + x) = a;
    a = 0x00;
    RAM(OBJ_AI_TIMER + x) = a;
    if (a == 0) { mff_cycles(0xA951, 1); goto L_A995; }
    mff_cycles(0xA953, 16);
    a = 0x00;
    RAM(TMP14) = a;
    RAM(TMP15) = a;
    a = RAM(OBJ_DIR + x);
    a &= 0x08;
    if (a == 0) { mff_cycles(0xA95E, 1); goto L_A976; }
    mff_cycles(0xA960, 8);
    y = 0x0A;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0964, func_F0F7, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA965, 2);
    if (!c) { mff_cycles(0xA965, 1); goto L_A995; }
    mff_cycles(0xA967, 10);
    a = RAM(TMP16);
    RAM(TMP14) = a;
    a &= 0x50;
    if (a == 0) { mff_cycles(0xA96D, 1); goto L_A995; }
    mff_cycles(0xA96F, 8);
    a = 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(TMP15) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA8F3, 2); call_by_address_tail(0xA8F3, 6); return;
L_A976:;
    mff_cycles(0xA976, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x04;
    if (a != 0) {
        mff_cycles(0xA97D, 8);
        y = 0x09;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0981, func_F0A2, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA982, 2);
        if (c) {
            mff_cycles(0xA984, 13);
            a = RAM(TMP16);
            a |= RAM(TMP14);
            RAM(TMP14) = a;
            a &= 0x50;
            if (a != 0) {
                mff_cycles(0xA98E, 8);
                a = 0x04;
                n = (a) >> 7;
                z = (a) == 0;
                RAM(TMP15) = a;
                SYNC_OUT;
                nes_cpu_instruction_boundary(0xA8F3, 2); call_by_address_tail(0xA8F3, 6); return;
            } else {
                mff_cycles(0xA98C, 1);
            }
        } else {
            mff_cycles(0xA982, 1);
        }
    } else {
        mff_cycles(0xA97B, 1);
    }
L_A995:;
    mff_cycles(0xA995, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    if (a == 0) { mff_cycles(0xA99A, 1); goto L_A9B5; }
    mff_cycles(0xA99C, 8);
    y = 0x08;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x09A0, func_F000, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA9A1, 2);
    if (!c) { mff_cycles(0xA9A1, 1); goto L_A9B5; }
    mff_cycles(0xA9A3, 13);
    a = RAM(TMP16);
    a |= RAM(TMP14);
    RAM(TMP14) = a;
    a &= 0x50;
    if (a == 0) { mff_cycles(0xA9AB, 1); goto L_A9D3; }
    mff_cycles(0xA9AD, 10);
    a = 0x01;
    a |= RAM(TMP15);
    RAM(TMP15) = a;
    if (a != 0) { mff_cycles(0xA9B3, 1); goto L_A9D3; }
L_A9B5:;
    mff_cycles(0xA9B5, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    if (a != 0) {
        mff_cycles(0xA9BC, 8);
        y = 0x09;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x09C0, func_F051, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA9C1, 2);
        if (c) {
            mff_cycles(0xA9C3, 13);
            a = RAM(TMP16);
            a |= RAM(TMP14);
            RAM(TMP14) = a;
            a &= 0x50;
            if (a != 0) {
                mff_cycles(0xA9CD, 8);
                a = 0x02;
                a |= RAM(TMP15);
                RAM(TMP15) = a;
            } else {
                mff_cycles(0xA9CB, 1);
            }
        } else {
            mff_cycles(0xA9C1, 1);
        }
    } else {
        mff_cycles(0xA9BA, 1);
    }
L_A9D3:;
    mff_cycles(0xA9D3, 7);
    a = RAM(TMP14);
    a &= 0x40;
    if (a != 0) {
        mff_cycles(0xA9D9, 14);
        a = 0x01;
        RAM(OBJ_PIT_FLAG + x) = a;
        a = 0x81;
        RAM(OBJ_STATE + x) = a;
    } else {
        mff_cycles(0xA9D7, 1);
    }
    mff_cycles(0xA9E3, 8);
    a = RAM(OBJ_STATE + x);
    m = 0x8E;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xA9EA, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xA9E8, 1);
    }
    mff_cycles(0xA9EB, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x09ED, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA9EE, 2);
    if (!c) {
        mff_cycles(0xA9F0, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF099, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F099(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xA9EE, 1);
    }
    mff_cycles(0xA9F3, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF090, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F090(); g_code_window_base = swb; } return;
}

/*  $A953 (bank 6) - lifted by port/tools/lift.py */
void func_A953_b6(void) {
    REGS_IN;
    mff_cycles(0xA953, 16);
    a = 0x00;
    RAM(TMP14) = a;
    RAM(TMP15) = a;
    a = RAM(OBJ_DIR + x);
    a &= 0x08;
    if (a == 0) { mff_cycles(0xA95E, 1); goto L_A976; }
    mff_cycles(0xA960, 8);
    y = 0x0A;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0964, func_F0F7, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA965, 2);
    if (!c) { mff_cycles(0xA965, 1); goto L_A995; }
    mff_cycles(0xA967, 10);
    a = RAM(TMP16);
    RAM(TMP14) = a;
    a &= 0x50;
    if (a == 0) { mff_cycles(0xA96D, 1); goto L_A995; }
    mff_cycles(0xA96F, 8);
    a = 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(TMP15) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA8F3, 2); call_by_address_tail(0xA8F3, 6); return;
L_A976:;
    mff_cycles(0xA976, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x04;
    if (a != 0) {
        mff_cycles(0xA97D, 8);
        y = 0x09;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0981, func_F0A2, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA982, 2);
        if (c) {
            mff_cycles(0xA984, 13);
            a = RAM(TMP16);
            a |= RAM(TMP14);
            RAM(TMP14) = a;
            a &= 0x50;
            if (a != 0) {
                mff_cycles(0xA98E, 8);
                a = 0x04;
                n = (a) >> 7;
                z = (a) == 0;
                RAM(TMP15) = a;
                SYNC_OUT;
                nes_cpu_instruction_boundary(0xA8F3, 2); call_by_address_tail(0xA8F3, 6); return;
            } else {
                mff_cycles(0xA98C, 1);
            }
        } else {
            mff_cycles(0xA982, 1);
        }
    } else {
        mff_cycles(0xA97B, 1);
    }
L_A995:;
    mff_cycles(0xA995, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    if (a == 0) { mff_cycles(0xA99A, 1); goto L_A9B5; }
    mff_cycles(0xA99C, 8);
    y = 0x08;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x09A0, func_F000, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA9A1, 2);
    if (!c) { mff_cycles(0xA9A1, 1); goto L_A9B5; }
    mff_cycles(0xA9A3, 13);
    a = RAM(TMP16);
    a |= RAM(TMP14);
    RAM(TMP14) = a;
    a &= 0x50;
    if (a == 0) { mff_cycles(0xA9AB, 1); goto L_A9D3; }
    mff_cycles(0xA9AD, 10);
    a = 0x01;
    a |= RAM(TMP15);
    RAM(TMP15) = a;
    if (a != 0) { mff_cycles(0xA9B3, 1); goto L_A9D3; }
L_A9B5:;
    mff_cycles(0xA9B5, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    if (a != 0) {
        mff_cycles(0xA9BC, 8);
        y = 0x09;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x09C0, func_F051, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA9C1, 2);
        if (c) {
            mff_cycles(0xA9C3, 13);
            a = RAM(TMP16);
            a |= RAM(TMP14);
            RAM(TMP14) = a;
            a &= 0x50;
            if (a != 0) {
                mff_cycles(0xA9CD, 8);
                a = 0x02;
                a |= RAM(TMP15);
                RAM(TMP15) = a;
            } else {
                mff_cycles(0xA9CB, 1);
            }
        } else {
            mff_cycles(0xA9C1, 1);
        }
    } else {
        mff_cycles(0xA9BA, 1);
    }
L_A9D3:;
    mff_cycles(0xA9D3, 7);
    a = RAM(TMP14);
    a &= 0x40;
    if (a != 0) {
        mff_cycles(0xA9D9, 14);
        a = 0x01;
        RAM(OBJ_PIT_FLAG + x) = a;
        a = 0x81;
        RAM(OBJ_STATE + x) = a;
    } else {
        mff_cycles(0xA9D7, 1);
    }
    mff_cycles(0xA9E3, 8);
    a = RAM(OBJ_STATE + x);
    m = 0x8E;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xA9EA, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xA9E8, 1);
    }
    mff_cycles(0xA9EB, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x09ED, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA9EE, 2);
    if (!c) {
        mff_cycles(0xA9F0, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF099, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F099(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xA9EE, 1);
    }
    mff_cycles(0xA9F3, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF090, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F090(); g_code_window_base = swb; } return;
}

/*  $A995 (bank 6) - lifted by port/tools/lift.py */
void func_A995_b6(void) {
    REGS_IN;
    mff_cycles(0xA995, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    if (a == 0) { mff_cycles(0xA99A, 1); goto L_A9B5; }
    mff_cycles(0xA99C, 8);
    y = 0x08;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x09A0, func_F000, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA9A1, 2);
    if (!c) { mff_cycles(0xA9A1, 1); goto L_A9B5; }
    mff_cycles(0xA9A3, 13);
    a = RAM(TMP16);
    a |= RAM(TMP14);
    RAM(TMP14) = a;
    a &= 0x50;
    if (a == 0) { mff_cycles(0xA9AB, 1); goto L_A9D3; }
    mff_cycles(0xA9AD, 10);
    a = 0x01;
    a |= RAM(TMP15);
    RAM(TMP15) = a;
    if (a != 0) { mff_cycles(0xA9B3, 1); goto L_A9D3; }
L_A9B5:;
    mff_cycles(0xA9B5, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    if (a != 0) {
        mff_cycles(0xA9BC, 8);
        y = 0x09;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x09C0, func_F051, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA9C1, 2);
        if (c) {
            mff_cycles(0xA9C3, 13);
            a = RAM(TMP16);
            a |= RAM(TMP14);
            RAM(TMP14) = a;
            a &= 0x50;
            if (a != 0) {
                mff_cycles(0xA9CD, 8);
                a = 0x02;
                a |= RAM(TMP15);
                RAM(TMP15) = a;
            } else {
                mff_cycles(0xA9CB, 1);
            }
        } else {
            mff_cycles(0xA9C1, 1);
        }
    } else {
        mff_cycles(0xA9BA, 1);
    }
L_A9D3:;
    mff_cycles(0xA9D3, 7);
    a = RAM(TMP14);
    a &= 0x40;
    if (a != 0) {
        mff_cycles(0xA9D9, 14);
        a = 0x01;
        RAM(OBJ_PIT_FLAG + x) = a;
        a = 0x81;
        RAM(OBJ_STATE + x) = a;
    } else {
        mff_cycles(0xA9D7, 1);
    }
    mff_cycles(0xA9E3, 8);
    a = RAM(OBJ_STATE + x);
    m = 0x8E;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xA9EA, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xA9E8, 1);
    }
    mff_cycles(0xA9EB, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x09ED, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA9EE, 2);
    if (!c) {
        mff_cycles(0xA9F0, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF099, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F099(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xA9EE, 1);
    }
    mff_cycles(0xA9F3, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF090, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F090(); g_code_window_base = swb; } return;
}

/*  $A9E3 (bank 6) - lifted by port/tools/lift.py */
void func_A9E3_b6(void) {
    REGS_IN;
    mff_cycles(0xA9E3, 8);
    a = RAM(OBJ_STATE + x);
    m = 0x8E;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xA9EA, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xA9E8, 1);
    }
    mff_cycles(0xA9EB, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x09ED, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA9EE, 2);
    if (!c) {
        mff_cycles(0xA9F0, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF099, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F099(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xA9EE, 1);
    }
    mff_cycles(0xA9F3, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF090, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F090(); g_code_window_base = swb; } return;
}

/*  $A9EB (bank 6) - lifted by port/tools/lift.py */
void func_A9EB_b6(void) {
    REGS_IN;
    mff_cycles(0xA9EB, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x09ED, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA9EE, 2);
    if (!c) {
        mff_cycles(0xA9F0, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF099, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F099(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xA9EE, 1);
    }
    mff_cycles(0xA9F3, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF090, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F090(); g_code_window_base = swb; } return;
}

/*  $A9F6 (bank 6) - lifted by port/tools/lift.py */
void func_A9F6_b6(void) {
    REGS_IN;
    mff_cycles(0xA9F6, 6);
    SYNC_OUT;
    if (!mff_call_at(0x09F8, 0x9BE1, 6)) return;
    SYNC_IN;
    mff_cycles(0xA9F9, 8);
    y = 0x06;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x09FD, func_ECCF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA9FE, 7);
    a = RAM(TMP16);
    a &= 0x40;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0xAA04, 38);
        a = RAM(OBJ_ID + x);
        RAM(OBJ_SAVED_ID + x) = a;
        a = 0x1F;
        RAM(OBJ_ID + x) = a;
        a = 0x00;
        RAM(OBJ_TIMER + x) = a;
        a = 0xFF;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_INVULN + x) = a;
        c = 1;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xAA02, 1);
    }
    mff_cycles(0xAA1B, 8);
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AA1D (bank 6) - lifted by port/tools/lift.py */
void func_AA1D_b6(void) {
    REGS_IN;
    mff_cycles(0xAA1D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0A1F, 0x9BE1, 6)) return;
    SYNC_IN;
    mff_cycles(0xAA20, 8);
    y = 0x07;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0A24, func_ECCF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAA25, 7);
    a = RAM(TMP16);
    a &= 0x40;
    if (a != 0) { mff_cycles(0xAA29, 1); goto L_AA49; }
    mff_cycles(0xAA2B, 8);
    y = 0x08;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0A2F, func_EDBA, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAA30, 7);
    a = RAM(TMP16);
    a &= 0x40;
    if (a != 0) { mff_cycles(0xAA34, 1); goto L_AA45; }
    mff_cycles(0xAA36, 8);
    y = 0x09;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0A3A, func_ECCF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAA3B, 7);
    a = RAM(TMP16);
    a &= 0x40;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xAA3F, 1); goto L_AA55; }
    mff_cycles(0xAA41, 11);
    RAM(OBJ_PIT_FLAG + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_AA45:;
    mff_cycles(0xAA45, 4);
    a = 0x02;
    if (a != 0) { mff_cycles(0xAA47, 1); goto L_AA4B; }
L_AA49:;
    mff_cycles(0xAA49, 2);
    a = 0x01;
L_AA4B:;
    mff_cycles(0xAA4B, 14);
    RAM(OBJ_DIR + x) = a;
    a = 0x02;
    RAM(OBJ_PIT_FLAG + x) = a;
    if (a != 0) { mff_cycles(0xAA53, 1); goto L_AA5A; }
L_AA55:;
    mff_cycles(0xAA55, 7);
    a = 0x01;
    RAM(OBJ_PIT_FLAG + x) = a;
L_AA5A:;
    mff_cycles(0xAA5A, 13);
    a = 0x81;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AA60 (bank 6) - lifted by port/tools/lift.py */
void func_AA60_b6(void) {
    REGS_IN;
    mff_cycles(0xAA60, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0A62, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAA63, 8);
    y = RAM(OBJ_KIND + x);
    m = 0x02;
    c = y >= m;
    if (y == m) { mff_cycles(0xAA68, 1); goto L_AA7F; }
L_AA6A:;
    mff_cycles(0xAA6A, 4);
    a = prg_read((uint16_t)(0xFC5B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xAA6D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0A6F, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAA70, 4);
    a = prg_read((uint16_t)(0xFC75 + y));
    mff_cycles(0xAA73, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0xFC33 + y));
    mff_cycles(0xAA78, 16);
    y = a;
    a = RAM(TMP0);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(CHR_BANKS + 3 + y) = a;
    SYNC_OUT;
    mff_rts(); return;
L_AA7F:;
    mff_cycles(0xAA7F, 8);
    a = RAM(OBJ_ANIM);
    m = 0x0D;
    c = a >= m;
    if (a == m) { mff_cycles(0xAA84, 1); goto L_AA96; }
    mff_cycles(0xAA86, 4);
    m = 0x16;
    c = a >= m;
    if (a == m) { mff_cycles(0xAA88, 1); goto L_AA96; }
    mff_cycles(0xAA8A, 4);
    m = 0x0E;
    c = a >= m;
    if (a == m) { mff_cycles(0xAA8C, 1); goto L_AA96; }
    mff_cycles(0xAA8E, 4);
    m = 0x17;
    c = a >= m;
    if (a == m) { mff_cycles(0xAA90, 1); goto L_AA96; }
    mff_cycles(0xAA92, 4);
    m = 0x12;
    c = a >= m;
    if (a != m) { mff_cycles(0xAA6A, 1); goto L_AA6A; }
L_AA96:;
    mff_cycles(0xAA96, 8);
    a = 0x94;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0A9A, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAA9B, 11);
    a = 0x1D;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(CHR_BANKS + 3) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AAA0 (bank 6) - lifted by port/tools/lift.py */
void func_AAA0_b6(void) {
    REGS_IN;
    mff_cycles(0xAAA0, 14);
    a = RAM(OBJ_ID + x);
    c = 1;
    m = 0x20;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    y = a;
    a = prg_read((uint16_t)(0xAD86 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xAAAA, 11);
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AAAE (bank 6) - lifted by port/tools/lift.py */
void func_AAAE_b6(void) {
    REGS_IN;
    mff_cycles(0xAAAE, 8);
    y = RAM(OBJ_KIND + x);
    a = prg_read((uint16_t)(0xAACB + y));
    mff_cycles(0xAAB4, 6);
    m = RAM(OBJ_ANIM + x);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xAAB9, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0ABB, func_F288, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xAAB7, 1);
    }
    mff_cycles(0xAABC, 4);
    a = prg_read((uint16_t)(0xAAD1 + y));
    mff_cycles(0xAABF, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0xFC33 + y));
    mff_cycles(0xAAC4, 16);
    y = a;
    a = RAM(TMP0);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(CHR_BANKS + 3 + y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AAD7 (bank 6) - lifted by port/tools/lift.py */
void func_AAD7_b6(void) {
    REGS_IN;
    mff_cycles(0xAAD7, 8);
    y = RAM(OBJ_KIND + x);
    a = prg_read((uint16_t)(0xAAF4 + y));
    mff_cycles(0xAADD, 6);
    m = RAM(OBJ_ANIM + x);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xAAE2, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0AE4, func_F288, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xAAE0, 1);
    }
    mff_cycles(0xAAE5, 4);
    a = prg_read((uint16_t)(0xAAFA + y));
    mff_cycles(0xAAE8, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0xFC33 + y));
    mff_cycles(0xAAED, 16);
    y = a;
    a = RAM(TMP0);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(CHR_BANKS + 3 + y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AB00 (bank 6) - lifted by port/tools/lift.py */
void func_AB00_b6(void) {
    REGS_IN;
    mff_cycles(0xAB00, 8);
    y = RAM(OBJ_KIND + x);
    a = prg_read((uint16_t)(0xAB1D + y));
    mff_cycles(0xAB06, 6);
    m = RAM(OBJ_ANIM + x);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xAB0B, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0B0D, func_F288, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xAB09, 1);
    }
    mff_cycles(0xAB0E, 4);
    a = prg_read((uint16_t)(0xAB23 + y));
    mff_cycles(0xAB11, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0xFC33 + y));
    mff_cycles(0xAB16, 16);
    y = a;
    a = RAM(TMP0);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(CHR_BANKS + 3 + y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AB29 (bank 6) - lifted by port/tools/lift.py */
void func_AB29_b6(void) {
    REGS_IN;
    mff_cycles(0xAB29, 6);
    a = RAM(OBJ_HELD + x);
    if (a == 0) { mff_cycles(0xAB2C, 1); goto L_AB82; }
    mff_cycles(0xAB2E, 4);
    m = 0xFF;
    c = a >= m;
    if (a == m) { mff_cycles(0xAB30, 1); goto L_AB5C; }
    mff_cycles(0xAB32, 7);
    a = RAM(GAME_ACTION);
    if (a == 0x04) { mff_cycles(0xAB36, 1); goto L_AB3C; }
    mff_cycles(0xAB38, 4);
    if (a != 0x21) { mff_cycles(0xAB3A, 1); goto L_AB82; }
L_AB3C:;
    mff_cycles(0xAB3C, 5);
    if (x != RAM(GRAB_SLOT)) { mff_cycles(0xAB3E, 1); goto L_AB82; }
    mff_cycles(0xAB40, 7);
    a = RAM(GAME_ACTION);
    m = 0x21;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xAB46, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0B48, 0xAA60, 6)) return;
        SYNC_IN;
        mff_cycles(0xAB49, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xAB4F, 2); call_by_address_tail(0xAB4F, 6); return;
    } else {
        mff_cycles(0xAB44, 1);
    }
    mff_cycles(0xAB4C, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0B4E, 0xAAAE, 6)) return;
    SYNC_IN;
    mff_cycles(0xAB4F, 13);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_HIT_TIMER + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0B56, 0xA8E8, 6)) return;
    SYNC_IN;
    mff_cycles(0xAB57, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B59, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAB5A, 8);
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
L_AB5C:;
    mff_cycles(0xAB5C, 21);
    a = RAM(OBJ_Y + x);
    RAM(TMP15) = a;
    a = RAM(PLAYER_GROUND_Y);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0B68, 0xA9F6, 6)) return;
    SYNC_IN;
    mff_cycles(0xAB69, 2);
    if (c) {
        mff_cycles(0xAB6B, 28);
        a = RAM(TMP15);
        RAM(OBJ_Y + x) = a;
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_VY_FRAC + x) = a;
        RAM(OBJ_VY + x) = a;
        c = 1;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xAB69, 1);
    }
    mff_cycles(0xAB7A, 13);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_HELD + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0B81, 0xAA1D, 6)) return;
    SYNC_IN;
L_AB82:;
    mff_cycles(0xAB82, 6);
    a = RAM(OBJ_HIT_TIMER + x);
    if (a == 0) { mff_cycles(0xAB85, 1); goto L_ABA9; }
    mff_cycles(0xAB87, 8);
    a = RAM(OBJ_HIT_TIMER + x);
    m = 0x1E;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xAB8C, 1); goto L_AB96; }
    mff_cycles(0xAB8E, 4);
    m = 0x1C;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xAB90, 1); goto L_AB9C; }
    mff_cycles(0xAB92, 4);
    m = 0x10;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xAB94, 1); goto L_AB9F; }
L_AB96:;
    mff_cycles(0xAB96, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0B98, 0xAAAE, 6)) return;
    SYNC_IN;
    mff_cycles(0xAB99, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAB9F, 2); call_by_address_tail(0xAB9F, 6); return;
L_AB9C:;
    mff_cycles(0xAB9C, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0B9E, 0xAA60, 6)) return;
    SYNC_IN;
L_AB9F:;
    mff_cycles(0xAB9F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0BA1, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xABA2, 15);
    a = 0x88;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
L_ABA9:;
    mff_cycles(0xABA9, 7);
    a = RAM(GAME_ACTION);
    m = 0x0A;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xABAD, 1); goto L_ABB7; }
    mff_cycles(0xABAF, 4);
    m = 0x0B;
    if (a == m) { mff_cycles(0xABB1, 1); goto L_ABB9; }
    mff_cycles(0xABB3, 4);
    m = 0x19;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xABB5, 1); goto L_ABC0; }
L_ABB7:;
    mff_cycles(0xABB7, 8);
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
L_ABB9:;
    mff_cycles(0xABB9, 15);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_HIT_PLAYER + x) = a;
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
L_ABC0:;
    mff_cycles(0xABC0, 8);
    a = RAM(OBJ_STATE + x);
    if (a == 0x81) { mff_cycles(0xABC5, 1); goto L_ABD3; }
    mff_cycles(0xABC7, 4);
    if (a == 0x82) { mff_cycles(0xABC9, 1); goto L_ABD3; }
    mff_cycles(0xABCB, 4);
    if (a == 0x84) { mff_cycles(0xABCD, 1); goto L_ABD3; }
    mff_cycles(0xABCF, 4);
    if (a != 0x86) { mff_cycles(0xABD1, 1); goto L_ABEE; }
L_ABD3:;
    mff_cycles(0xABD3, 7);
    a = RAM(GAME_ACTION);
    m = 0x06;
    c = a >= m;
    if (a == m) { mff_cycles(0xABD7, 1); goto L_ABE1; }
    mff_cycles(0xABD9, 4);
    m = 0x24;
    c = a >= m;
    if (a == m) { mff_cycles(0xABDB, 1); goto L_ABE1; }
    mff_cycles(0xABDD, 4);
    m = 0x1F;
    c = a >= m;
    if (a != m) { mff_cycles(0xABDF, 1); goto L_ABEE; }
L_ABE1:;
    mff_cycles(0xABE1, 13);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_HIT_PLAYER + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0BE8, 0xAAAE, 6)) return;
    SYNC_IN;
    mff_cycles(0xABE9, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0BEB, 0xA8E8, 6)) return;
    SYNC_IN;
    mff_cycles(0xABEC, 8);
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
L_ABEE:;
    mff_cycles(0xABEE, 8);
    a = RAM(OBJ_STATE + x);
    m = 0x8F;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xABF3, 1); goto L_ABFB; }
    mff_cycles(0xABF5, 7);
    a = RAM(GAME_ACTION);
    m = 0x25;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xABF9, 1); goto L_ABFD; }
L_ABFB:;
    mff_cycles(0xABFB, 8);
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
L_ABFD:;
    mff_cycles(0xABFD, 8);
    a = RAM(OBJ_STATE + x);
    m = 0x89;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xAC04, 12);
        a = RAM(OBJ_GROUND_Y + x);
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_LAND_Y + x) = a;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xAC13, 2); call_by_address_tail(0xAC13, 6); return;
    } else {
        mff_cycles(0xAC02, 1);
    }
    mff_cycles(0xAC0D, 61);
    a = RAM(OBJ_Y + x);
    RAM(OBJ_LAND_Y + x) = a;
    a = 0x8E;
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x03;
    RAM(OBJ_VY + x) = a;
    a = RAM(OBJ_ID + x);
    RAM(OBJ_SAVED_ID + x) = a;
    a = 0x05;
    RAM(OBJ_ID + x) = a;
    a = 0x80;
    RAM(OBJ_STATE + x) = a;
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_INVULN + x) = a;
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AB4F (bank 6) - lifted by port/tools/lift.py */
void func_AB4F_b6(void) {
    REGS_IN;
    mff_cycles(0xAB4F, 13);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_HIT_TIMER + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0B56, 0xA8E8, 6)) return;
    SYNC_IN;
    mff_cycles(0xAB57, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B59, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAB5A, 8);
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AB9F (bank 6) - lifted by port/tools/lift.py */
void func_AB9F_b6(void) {
    REGS_IN;
    mff_cycles(0xAB9F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0BA1, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xABA2, 15);
    a = 0x88;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AC34 (bank 6) - lifted by port/tools/lift.py */
void func_AC34_b6(void) {
    REGS_IN;
    mff_cycles(0xAC34, 9);
    RAM(TMP2) = y;
    SYNC_OUT;
    if (!mff_call_at(0x0C38, 0xAAD7, 6)) return;
    SYNC_IN;
    mff_cycles(0xAC39, 8);
    y = RAM(OBJ_KIND + x);
    a = prg_read((uint16_t)(0xACA7 + y));
    mff_cycles(0xAC3F, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0xACAD + y));
    mff_cycles(0xAC44, 11);
    RAM(TMP1) = a;
    y = RAM(TMP2);
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $ACB3 (bank 6) - lifted by port/tools/lift.py */
void func_ACB3_b6(void) {
    REGS_IN;
    mff_cycles(0xACB3, 26);
    RAM(TMP15) = y;
    a = RAM(OBJ_KIND + x);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    c = 0;
    m = RAM(TMP15);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    y = a;
    a = prg_read((uint16_t)(0xAD9C + y));
    mff_cycles(0xACC3, 9);
    y = RAM(TMP15);
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    mff_rts(); return;
}
