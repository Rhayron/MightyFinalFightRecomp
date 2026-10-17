/*
 * lifted_f.c - fixed-bank routines $F000-$FFFF, lifted by
 * port/tools/lift_bank.py from the pure scaffold: exact, still to be
 * tidied and moved into their systems' files.
 */
#include "mff.h"
#include "state.h"
#include "coroutine.h"

void func_E9BC(void);
void func_ECCF(void);
void func_EDBA(void);
void func_EF55(void);
void func_EF6D(void);
void func_EF83(void);
void func_EF9D(void);
void func_F090(void);
void func_F099(void);
void func_F0DC(void);
void func_F16E(void);
void func_F183(void);
void func_F1A1(void);
void func_F1BF(void);
void func_F1DF(void);
void func_F20E(void);
void func_F22E(void);
void func_F24C(void);
void func_F26E(void);
void func_F288(void);
void func_F294(void);
void func_F2A0(void);
void func_F314(void);
void func_F32E(void);
void func_F34C(void);
void func_F365(void);
void func_F374(void);
void func_F38B(void);
void func_F436(void);
void func_F440(void);
void func_F4E1(void);
void func_F522(void);
void func_F63F(void);
void func_F679(void);
void func_F6A9(void);
void func_F72A(void);
void func_F7EA(void);
void func_F7EF(void);
void func_F7FF(void);
void func_F83D(void);
void func_F84C(void);
void func_F9E9(void);
void func_FA84(void);
void func_FA91(void);
void func_FAC4(void);
void func_FBB0(void);
void func_FBD9(void);
void func_FF02(void);
void func_FF31(void);
void func_FF4D(void);

/*  $F000 - lifted by port/tools/lift.py */
void func_F000(void) {
    REGS_IN;
    mff_cycles(0xF000, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1002, func_F090, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF003, 4);
    m = 0x00;
    c = x >= m;
    n = ((uint8_t)(x - m)) >> 7;
    z = ((uint8_t)(x - m)) == 0;
    if (z) {
        mff_cycles(0xF007, 14);
        a = RAM(OBJ_X);
        RAM(TMP2) = a;
        a = RAM(OBJ_X_HI);
        n = (a) >> 7;
        z = (a) == 0;
        RAM(TMP3) = a;
    } else {
        mff_cycles(0xF005, 1);
    }
    mff_cycles(0xF011, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1013, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF014, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1016, func_EDBA, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF017, 9);
    c = 0;
    a = RAM(TMP16);
    a &= 0x40;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xF01C, 1); goto L_F025; }
    mff_cycles(0xF01E, 2);
    if (z) { mff_cycles(0xF01E, 1); goto L_F024; }
L_F020:;
    mff_cycles(0xF020, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1022, func_EF55, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF023, 2);
    c = 1;
L_F024:;
    mff_cycles(0xF024, 6);
    SYNC_OUT;
    mff_rts(); return;
L_F025:;
    mff_cycles(0xF025, 4);
    m = 0x00;
    c = x >= m;
    n = ((uint8_t)(x - m)) >> 7;
    z = ((uint8_t)(x - m)) == 0;
    if (!z) { mff_cycles(0xF020, 1); goto L_F020; }
    mff_cycles(0xF029, 7);
    a = RAM(TMP16);
    m = 0x40;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xF02F, 4);
        m = 0x50;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xF033, 8);
            c = 0;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xF031, 1);
        }
    } else {
        mff_cycles(0xF02D, 1);
    }
    mff_cycles(0xF035, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) { mff_cycles(0xF024, 1); goto L_F024; }
    mff_cycles(0xF03A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x103C, func_F0DC, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF03D, 8);
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F003 - lifted by port/tools/lift.py */
void func_F003(void) {
    REGS_IN;
    mff_cycles(0xF003, 4);
    m = 0x00;
    c = x >= m;
    n = ((uint8_t)(x - m)) >> 7;
    z = ((uint8_t)(x - m)) == 0;
    if (z) {
        mff_cycles(0xF007, 14);
        a = RAM(OBJ_X);
        RAM(TMP2) = a;
        a = RAM(OBJ_X_HI);
        n = (a) >> 7;
        z = (a) == 0;
        RAM(TMP3) = a;
    } else {
        mff_cycles(0xF005, 1);
    }
    mff_cycles(0xF011, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1013, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF014, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1016, func_EDBA, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF017, 9);
    c = 0;
    a = RAM(TMP16);
    a &= 0x40;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xF01C, 1); goto L_F025; }
    mff_cycles(0xF01E, 2);
    if (z) { mff_cycles(0xF01E, 1); goto L_F024; }
L_F020:;
    mff_cycles(0xF020, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1022, func_EF55, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF023, 2);
    c = 1;
L_F024:;
    mff_cycles(0xF024, 6);
    SYNC_OUT;
    mff_rts(); return;
L_F025:;
    mff_cycles(0xF025, 4);
    m = 0x00;
    c = x >= m;
    n = ((uint8_t)(x - m)) >> 7;
    z = ((uint8_t)(x - m)) == 0;
    if (!z) { mff_cycles(0xF020, 1); goto L_F020; }
    mff_cycles(0xF029, 7);
    a = RAM(TMP16);
    m = 0x40;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xF02F, 4);
        m = 0x50;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xF033, 8);
            c = 0;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xF031, 1);
        }
    } else {
        mff_cycles(0xF02D, 1);
    }
    mff_cycles(0xF035, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) { mff_cycles(0xF024, 1); goto L_F024; }
    mff_cycles(0xF03A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x103C, func_F0DC, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF03D, 8);
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F051 - lifted by port/tools/lift.py */
void func_F051(void) {
    REGS_IN;
    mff_cycles(0xF051, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1053, func_F099, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF054, 4);
    m = 0x00;
    c = x >= m;
    n = ((uint8_t)(x - m)) >> 7;
    z = ((uint8_t)(x - m)) == 0;
    if (z) {
        mff_cycles(0xF058, 14);
        a = RAM(OBJ_X);
        RAM(TMP2) = a;
        a = RAM(OBJ_X_HI);
        n = (a) >> 7;
        z = (a) == 0;
        RAM(TMP3) = a;
    } else {
        mff_cycles(0xF056, 1);
    }
    mff_cycles(0xF062, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1064, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF065, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1067, func_EDBA, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF068, 9);
    c = 0;
    a = RAM(TMP16);
    a &= 0x40;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xF06D, 1); goto L_F076; }
    mff_cycles(0xF06F, 2);
    if (z) { mff_cycles(0xF06F, 1); goto L_F075; }
L_F071:;
    mff_cycles(0xF071, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1073, func_EF6D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF074, 2);
    c = 1;
L_F075:;
    mff_cycles(0xF075, 6);
    SYNC_OUT;
    mff_rts(); return;
L_F076:;
    mff_cycles(0xF076, 4);
    m = 0x00;
    c = x >= m;
    n = ((uint8_t)(x - m)) >> 7;
    z = ((uint8_t)(x - m)) == 0;
    if (!z) { mff_cycles(0xF071, 1); goto L_F071; }
    mff_cycles(0xF07A, 7);
    a = RAM(TMP16);
    m = 0x40;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xF080, 4);
        m = 0x50;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xF084, 8);
            c = 0;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xF082, 1);
        }
    } else {
        mff_cycles(0xF07E, 1);
    }
    mff_cycles(0xF086, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) { mff_cycles(0xF075, 1); goto L_F075; }
    mff_cycles(0xF08B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x108D, func_F0DC, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF08E, 8);
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F054 - lifted by port/tools/lift.py */
void func_F054(void) {
    REGS_IN;
    mff_cycles(0xF054, 4);
    m = 0x00;
    c = x >= m;
    n = ((uint8_t)(x - m)) >> 7;
    z = ((uint8_t)(x - m)) == 0;
    if (z) {
        mff_cycles(0xF058, 14);
        a = RAM(OBJ_X);
        RAM(TMP2) = a;
        a = RAM(OBJ_X_HI);
        n = (a) >> 7;
        z = (a) == 0;
        RAM(TMP3) = a;
    } else {
        mff_cycles(0xF056, 1);
    }
    mff_cycles(0xF062, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1064, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF065, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1067, func_EDBA, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF068, 9);
    c = 0;
    a = RAM(TMP16);
    a &= 0x40;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xF06D, 1); goto L_F076; }
    mff_cycles(0xF06F, 2);
    if (z) { mff_cycles(0xF06F, 1); goto L_F075; }
L_F071:;
    mff_cycles(0xF071, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1073, func_EF6D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF074, 2);
    c = 1;
L_F075:;
    mff_cycles(0xF075, 6);
    SYNC_OUT;
    mff_rts(); return;
L_F076:;
    mff_cycles(0xF076, 4);
    m = 0x00;
    c = x >= m;
    n = ((uint8_t)(x - m)) >> 7;
    z = ((uint8_t)(x - m)) == 0;
    if (!z) { mff_cycles(0xF071, 1); goto L_F071; }
    mff_cycles(0xF07A, 7);
    a = RAM(TMP16);
    m = 0x40;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xF080, 4);
        m = 0x50;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xF084, 8);
            c = 0;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xF082, 1);
        }
    } else {
        mff_cycles(0xF07E, 1);
    }
    mff_cycles(0xF086, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) { mff_cycles(0xF075, 1); goto L_F075; }
    mff_cycles(0xF08B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x108D, func_F0DC, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF08E, 8);
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F0A2 - lifted by port/tools/lift.py */
void func_F0A2(void) {
    REGS_IN;
    mff_cycles(0xF0A2, 4);
    m = 0x00;
    c = x >= m;
    n = ((uint8_t)(x - m)) >> 7;
    z = ((uint8_t)(x - m)) == 0;
    if (z) {
        mff_cycles(0xF0A6, 14);
        a = RAM(OBJ_Y);
        RAM(TMP2) = a;
        a = RAM(OBJ_X_HI);
        n = (a) >> 7;
        z = (a) == 0;
        RAM(TMP3) = a;
    } else {
        mff_cycles(0xF0A4, 1);
    }
    mff_cycles(0xF0B0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x10B2, func_F1BF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF0B3, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x10B5, func_ECCF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF0B6, 9);
    a = RAM(TMP16);
    m = 0x10;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xF0BB, 1); goto L_F0C2; }
L_F0BD:;
    mff_cycles(0xF0BD, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x10BF, func_EF9D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF0C0, 8);
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
L_F0C2:;
    mff_cycles(0xF0C2, 4);
    m = 0x02;
    if (a == m) { mff_cycles(0xF0C4, 1); goto L_F0CC; }
    mff_cycles(0xF0C6, 4);
    a &= 0x40;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xF0C8, 1); goto L_F0D4; }
    mff_cycles(0xF0CA, 8);
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
L_F0CC:;
    mff_cycles(0xF0CC, 7);
    a = RAM(GAME_ACTION);
    m = 0x0D;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF0BD, 1); goto L_F0BD; }
    mff_cycles(0xF0D2, 8);
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
L_F0D4:;
    mff_cycles(0xF0D4, 4);
    m = 0x00;
    c = x >= m;
    n = ((uint8_t)(x - m)) >> 7;
    z = ((uint8_t)(x - m)) == 0;
    if (!z) { mff_cycles(0xF0BD, 1); goto L_F0BD; }
    mff_cycles(0xF0D8, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x10DA, func_F0DC, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF0DB, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F0DC - lifted by port/tools/lift.py */
void func_F0DC(void) {
    REGS_IN;
    mff_cycles(0xF0DC, 24);
    a = RAM(GAME_ACTION);
    RAM(ACTION_BEFORE_FALL) = a;
    a = 0x24;
    RAM(GAME_ACTION) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = RAM(ACTION_BEFORE_FALL);
    m = 0x04;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xF0EF, 10);
        a = 0xFF;
        y = RAM(GRAB_SLOT);
        n = (y) >> 7;
        z = (y) == 0;
        RAM(OBJ_HELD + y) = a;
    } else {
        mff_cycles(0xF0ED, 1);
    }
    mff_cycles(0xF0F6, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F0F7 - lifted by port/tools/lift.py */
void func_F0F7(void) {
    REGS_IN;
    mff_cycles(0xF0F7, 4);
    m = 0x00;
    c = x >= m;
    n = ((uint8_t)(x - m)) >> 7;
    z = ((uint8_t)(x - m)) == 0;
    if (z) {
        mff_cycles(0xF0FB, 14);
        a = RAM(OBJ_Y);
        RAM(TMP2) = a;
        a = RAM(OBJ_X_HI);
        n = (a) >> 7;
        z = (a) == 0;
        RAM(TMP3) = a;
    } else {
        mff_cycles(0xF0F9, 2);
    }
    mff_cycles(0xF105, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1107, func_F1DF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF108, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x110A, func_ECCF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF10B, 9);
    a = RAM(TMP16);
    m = 0x30;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF110, 1); goto L_F114; }
    mff_cycles(0xF112, 2);
    if (!z) { mff_cycles(0xF112, 1); goto L_F118; }
L_F114:;
    mff_cycles(0xF114, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1116, func_EF83, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF117, 2);
    c = 1;
L_F118:;
    mff_cycles(0xF118, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F119 - lifted by port/tools/lift.py */
void func_F119(void) {
    REGS_IN;
    mff_cycles(0xF119, 4);
    m = 0x00;
    c = x >= m;
    if (x == m) {
        mff_cycles(0xF11D, 14);
        a = RAM(OBJ_Y);
        RAM(TMP2) = a;
        a = RAM(OBJ_X_HI);
        RAM(TMP3) = a;
    } else {
        mff_cycles(0xF11B, 1);
    }
    mff_cycles(0xF127, 6);
    a = RAM(OBJ_VY + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0xF12A, 1); goto L_F14E; }
    mff_cycles(0xF12C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x112E, func_F20E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF12F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1131, func_F24C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF132, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1134, func_ECCF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF135, 4);
    if (x != 0x00) {
        mff_cycles(0xF137, 1);
    }
    mff_cycles(0xF139, 7);
    a = RAM(TMP16);
    m = 0x10;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF13D, 1); goto L_F146; }
    mff_cycles(0xF13F, 4);
    m = 0x40;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xF141, 1); goto L_F170; }
    mff_cycles(0xF143, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF16E, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F16E(); g_code_window_base = swb; } return;
L_F146:;
    mff_cycles(0xF146, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1148, func_EF9D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF149, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x114B, func_F26E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF14C, 8);
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
L_F14E:;
    mff_cycles(0xF14E, 8);
    y++;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1151, func_F22E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF152, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1154, func_F24C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF155, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1157, func_ECCF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF158, 7);
    a = RAM(TMP16);
    m = 0x30;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF15C, 1); goto L_F16E; }
    mff_cycles(0xF15E, 4);
    if (a >= 0x40) { mff_cycles(0xF160, 1); goto L_F170; }
    mff_cycles(0xF162, 4);
    m = 0x10;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF164, 1); goto L_F168; }
    mff_cycles(0xF166, 2);
    if (!z) { mff_cycles(0xF166, 1); goto L_F16E; }
L_F168:;
    mff_cycles(0xF168, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x116A, func_EF83, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF16B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x116D, func_F26E, 0xE000)) return;
    SYNC_IN;
L_F16E:;
    mff_cycles(0xF16E, 8);
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
L_F170:;
    mff_cycles(0xF170, 4);
    m = 0x00;
    n = ((uint8_t)(x - m)) >> 7;
    z = ((uint8_t)(x - m)) == 0;
    if (!z) { mff_cycles(0xF16E, 1); goto L_F16E; }
    mff_cycles(0xF174, 25);
    a = RAM(GAME_ACTION);
    RAM(ACTION_BEFORE_FALL) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0x24;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F16E - lifted by port/tools/lift.py */
void func_F16E(void) {
    REGS_IN;
    mff_cycles(0xF16E, 8);
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
}

/* obj_set_anim $F288 - lifted by port/tools/lift.py */
void func_F288(void) {
    REGS_IN;
    mff_cycles(0xF288, 23);
    RAM(OBJ_ANIM + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ANIM_FRAME + x) = a;
    RAM(OBJ_ANIM_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/* player_set_anim $F294 - lifted by port/tools/lift.py */
void func_F294(void) {
    REGS_IN;
    mff_cycles(0xF294, 20);
    RAM(OBJ_ANIM) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ANIM_FRAME) = a;
    RAM(OBJ_ANIM_TIMER) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/* obj_spawn $F2A0 - lifted by port/tools/lift.py */
void func_F2A0(void) {
    REGS_IN;
    mff_cycles(0xF2A0, 48);
    RAM(OBJ_ANIM + y) = a;
    a = 0x00;
    RAM(OBJ_ANIM_FRAME + y) = a;
    a = RAM(OBJ_ANIM_TIMER + y);
    a &= 0x80;
    RAM(OBJ_ANIM_TIMER + y) = a;
    a = 0x10;
    RAM(OBJ_FLAGS + y) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + y) = a;
    RAM(OBJ_SAVED_STATE + y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F406 - lifted by port/tools/lift.py */
void func_F406(void) {
    REGS_IN;
    mff_cycles(0xF406, 28);
    a = RAM(PRG_BANKS);
    mff_push(a);
    a = RAM(PRG_BANKS + 1);
    mff_push(a);
    t = (uint8_t)(RAM(DRAW_BUSY) + 1);
    RAM(DRAW_BUSY) = t;
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OAM_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1414, 0xC31D, -1)) return;
    SYNC_IN;
    mff_cycles(0xF415, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1417, func_E9BC, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF418, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(DRAW_BUSY) = a;
    SYNC_OUT;
    mff_call_fn(0x141E, func_FF31, 0xE000);
    SYNC_IN;
    mff_cycles(0xF41F, 17);
    a = mff_pull();
    RAM(BANK_REQUEST + 1) = a;
    a = mff_pull();
    n = (a) >> 7;
    z = (a) == 0;
    RAM(BANK_REQUEST) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xFF4D, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_FF4D(); g_code_window_base = swb; } return;
}

/*  $F428 - lifted by port/tools/lift.py */
void func_F428(void) {
    REGS_IN;
    mff_cycles(0xF428, 18);
    a = 0x1E;
    RAM(ATTACK_LIVE) = a;
    a = 0x00;
    RAM(ATTACK_ID) = a;
    a = 0x17;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1434, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF435, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F430 - lifted by port/tools/lift.py */
void func_F430(void) {
    REGS_IN;
    mff_cycles(0xF430, 8);
    a = 0x17;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1434, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF435, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F436 - lifted by port/tools/lift.py */
void func_F436(void) {
    REGS_IN;
    mff_cycles(0xF436, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ATTACK_LIVE) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F43B - lifted by port/tools/lift.py */
void func_F43B(void) {
    REGS_IN;
    mff_cycles(0xF43B, 11);
    a = 0x1E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ATTACK_LIVE) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F440 - lifted by port/tools/lift.py */
void func_F440(void) {
    REGS_IN;
    mff_cycles(0xF440, 9);
    RAM(CHR_BANKS + 2) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F443 - lifted by port/tools/lift.py */
void func_F443(void) {
    REGS_IN;
    mff_cycles(0xF443, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1445, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF446, 9);
    RAM(CHR_BANKS + 3) = y;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F449 - lifted by port/tools/lift.py */
void func_F449(void) {
    REGS_IN;
    mff_cycles(0xF449, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x144B, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF44C, 9);
    RAM(CHR_BANKS + 4) = y;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F44F - lifted by port/tools/lift.py */
void func_F44F(void) {
    REGS_IN;
    mff_cycles(0xF44F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1451, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF452, 9);
    RAM(CHR_BANKS + 5) = y;
    SYNC_OUT;
    mff_rts(); return;
}

/* spawn_hit_spark $F455 - lifted by port/tools/lift.py */
void func_F455(void) {
    REGS_IN;
    goto L_F455;
L_F454:;
    mff_cycles(0xF454, 6);
    SYNC_OUT;
    mff_rts(); return;
L_F455:;
    mff_cycles(0xF455, 8);
    a = RAM(OBJ_ANIM + x);
    m = 0x9D;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF454, 1); goto L_F454; }
    mff_cycles(0xF45C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x145E, func_F72A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF45F, 2);
    if (c) { mff_cycles(0xF454, 1); goto L_F454; }
    mff_cycles(0xF461, 11);
    RAM(TMP15) = y;
    a = 0x7E;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1467, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF468, 105);
    a = 0x01;
    RAM(OBJ_ID + y) = a;
    RAM(OBJ_SAVED_ID + y) = a;
    a = 0x00;
    RAM(OBJ_HIT_COOLDOWN + y) = a;
    RAM(OBJ_ATTACK + y) = a;
    RAM(OBJ_ATTACK_DAMAGE + y) = a;
    RAM(OBJ_LIFE + y) = a;
    RAM(OBJ_LIFE_MAX + y) = a;
    RAM(OBJ_INVULN + y) = a;
    RAM(OBJ_HIT_PLAYER + y) = a;
    RAM(OBJ_HIT_TIMER + y) = a;
    RAM(OBJ_HELD + y) = a;
    RAM(OBJ_KIND + y) = a;
    RAM(OBJ_TIER + y) = a;
    RAM(OBJ_AI_TIMER + y) = a;
    RAM(OBJ_AI_COUNT + y) = a;
    RAM(OBJ_AI_FLAG + y) = a;
    RAM(OBJ_SPAWN_GROUP + y) = a;
    a = RAM(OBJ_Y_HI);
    RAM(OBJ_Y_HI + y) = a;
    a = RAM(PLAYER_ID);
    if (a == 0x02) {
        mff_cycles(0xF4AB, 7);
        a = RAM(ATTACK_ID);
        if (a >= 0x0F) {
            mff_cycles(0xF4B1, 4);
            m = 0x17;
            c = a >= m;
            n = ((uint8_t)(a - m)) >> 7;
            z = ((uint8_t)(a - m)) == 0;
            if (!c) {
                mff_cycles(0xF4B5, 3);
                SYNC_OUT;
                nes_cpu_instruction_boundary(0xF4E1, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F4E1(); g_code_window_base = swb; } return;
            } else {
                mff_cycles(0xF4B3, 1);
            }
        } else {
            mff_cycles(0xF4AF, 1);
        }
        mff_cycles(0xF4B8, 17);
        a = RAM(PLAYER_DIR);
        a &= 0x02;
        y = a;
        a = RAM(OBJ_X);
        c = 0;
        m = prg_read((uint16_t)(0xF550 + y));
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        mff_cycles(0xF4C4, 11);
        mff_push(a);
        a = RAM(OBJ_X_HI);
        m = prg_read((uint16_t)(0xF551 + y));
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        mff_cycles(0xF4CB, 20);
        y = RAM(TMP15);
        RAM(OBJ_X_HI + y) = a;
        a = mff_pull();
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_X + y) = a;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF522, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F522(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xF4A9, 1);
    }
    mff_cycles(0xF4D7, 7);
    a = RAM(ATTACK_ID);
    if (a < 0x0F) { mff_cycles(0xF4DB, 2); goto L_F506; }
    mff_cycles(0xF4DD, 4);
    m = 0x17;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xF4DF, 2); goto L_F506; }
    mff_cycles(0xF4E1, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x14E3, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF4E4, 2);
    if (!c) { mff_cycles(0xF4E4, 1); goto L_F4EA; }
    mff_cycles(0xF4E6, 4);
    y = 0x02;
    if (y != 0) { mff_cycles(0xF4E8, 1); goto L_F4EC; }
L_F4EA:;
    mff_cycles(0xF4EA, 2);
    y = 0x00;
L_F4EC:;
    mff_cycles(0xF4EC, 10);
    a = RAM(OBJ_X);
    c = 0;
    m = prg_read((uint16_t)(0xF554 + y));
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xF4F3, 11);
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = prg_read((uint16_t)(0xF555 + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xF4FA, 20);
    y = RAM(TMP15);
    RAM(OBJ_X_HI + y) = a;
    a = mff_pull();
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X + y) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF522, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F522(); g_code_window_base = swb; } return;
L_F506:;
    mff_cycles(0xF506, 17);
    a = RAM(PLAYER_DIR);
    a &= 0x02;
    y = a;
    a = RAM(OBJ_X);
    c = 0;
    m = prg_read((uint16_t)(0xF54C + y));
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xF512, 11);
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = prg_read((uint16_t)(0xF54D + y));
    w = a + m + c;
    a = (uint8_t)w;
    mff_cycles(0xF519, 24);
    y = RAM(TMP15);
    RAM(OBJ_X_HI + y) = a;
    a = mff_pull();
    RAM(OBJ_X + y) = a;
    a = RAM(PLAYER_ID);
    if (a != 0x02) { mff_cycles(0xF526, 1); goto L_F542; }
    mff_cycles(0xF528, 7);
    a = RAM(ATTACK_ID);
    if (a == 0x1B) { mff_cycles(0xF52C, 1); goto L_F538; }
    mff_cycles(0xF52E, 4);
    if (a == 0x1C) { mff_cycles(0xF530, 1); goto L_F538; }
    mff_cycles(0xF532, 4);
    m = 0x1D;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF534, 1); goto L_F538; }
    mff_cycles(0xF536, 2);
    if (!z) { mff_cycles(0xF536, 1); goto L_F542; }
L_F538:;
    mff_cycles(0xF538, 19);
    a = RAM(OBJ_Y);
    c = 1;
    m = 0x10;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + y) = a;
    SYNC_OUT;
    mff_rts(); return;
L_F542:;
    mff_cycles(0xF542, 19);
    a = RAM(OBJ_Y);
    c = 0;
    m = 0x04;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F4E1 - lifted by port/tools/lift.py */
void func_F4E1(void) {
    REGS_IN;
    mff_cycles(0xF4E1, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x14E3, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF4E4, 2);
    if (!c) { mff_cycles(0xF4E4, 1); goto L_F4EA; }
    mff_cycles(0xF4E6, 4);
    y = 0x02;
    if (y != 0) { mff_cycles(0xF4E8, 1); goto L_F4EC; }
L_F4EA:;
    mff_cycles(0xF4EA, 2);
    y = 0x00;
L_F4EC:;
    mff_cycles(0xF4EC, 10);
    a = RAM(OBJ_X);
    c = 0;
    m = prg_read((uint16_t)(0xF554 + y));
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xF4F3, 11);
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = prg_read((uint16_t)(0xF555 + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xF4FA, 20);
    y = RAM(TMP15);
    RAM(OBJ_X_HI + y) = a;
    a = mff_pull();
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X + y) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF522, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F522(); g_code_window_base = swb; } return;
}

/*  $F522 - lifted by port/tools/lift.py */
void func_F522(void) {
    REGS_IN;
    mff_cycles(0xF522, 7);
    a = RAM(PLAYER_ID);
    if (a != 0x02) { mff_cycles(0xF526, 1); goto L_F542; }
    mff_cycles(0xF528, 7);
    a = RAM(ATTACK_ID);
    if (a == 0x1B) { mff_cycles(0xF52C, 1); goto L_F538; }
    mff_cycles(0xF52E, 4);
    if (a == 0x1C) { mff_cycles(0xF530, 1); goto L_F538; }
    mff_cycles(0xF532, 4);
    m = 0x1D;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF534, 1); goto L_F538; }
    mff_cycles(0xF536, 2);
    if (!z) { mff_cycles(0xF536, 1); goto L_F542; }
L_F538:;
    mff_cycles(0xF538, 19);
    a = RAM(OBJ_Y);
    c = 1;
    m = 0x10;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + y) = a;
    SYNC_OUT;
    mff_rts(); return;
L_F542:;
    mff_cycles(0xF542, 19);
    a = RAM(OBJ_Y);
    c = 0;
    m = 0x04;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F558 - lifted by port/tools/lift.py */
void func_F558(void) {
    REGS_IN;
    mff_cycles(0xF558, 8);
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
}

/* enemy_attack_test $F55A - lifted by port/tools/lift.py */
void func_F55A(void) {
    REGS_IN;
    mff_cycles(0xF55A, 8);
    c = 1;
    a = RAM(OBJ_FLAGS + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0xF55E, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1558), -1); return; }
    mff_cycles(0xF560, 4);
    a &= 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xF562, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1558), -1); return; }
    mff_cycles(0xF564, 6);
    a = RAM(OBJ_STATE + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0xF567, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1558), -1); return; }
    mff_cycles(0xF569, 8);
    a = RAM(OBJ_ID + x);
    m = 0x06;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF56E, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1558), -1); return; }
    mff_cycles(0xF570, 4);
    m = 0x07;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF572, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1558), -1); return; }
    mff_cycles(0xF574, 4);
    m = 0x69;
    c = a >= m;
    if (a == m) { mff_cycles(0xF576, 1); goto L_F5B8; }
    mff_cycles(0xF578, 4);
    m = 0x6A;
    c = a >= m;
    if (a == m) { mff_cycles(0xF57A, 1); goto L_F5B8; }
    mff_cycles(0xF57C, 4);
    m = 0x10;
    c = a >= m;
    if (a == m) { mff_cycles(0xF57E, 1); goto L_F5B8; }
    mff_cycles(0xF580, 4);
    m = 0x11;
    c = a >= m;
    if (a == m) { mff_cycles(0xF582, 1); goto L_F5B8; }
    mff_cycles(0xF584, 4);
    m = 0x12;
    c = a >= m;
    if (a == m) { mff_cycles(0xF586, 1); goto L_F5B8; }
    mff_cycles(0xF588, 4);
    m = 0x13;
    c = a >= m;
    if (a == m) { mff_cycles(0xF58A, 1); goto L_F5B8; }
    mff_cycles(0xF58C, 5);
    a = RAM(RESPAWN_TIMER1);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xF58E, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1558), -1); return; }
    mff_cycles(0xF590, 5);
    a = RAM(RESPAWN_TIMER2);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xF592, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1558), -1); return; }
    mff_cycles(0xF594, 7);
    a = RAM(GAME_ACTION);
    m = 0x16;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF598, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1558), -1); return; }
    mff_cycles(0xF59A, 4);
    m = 0x17;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF59C, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1558), -1); return; }
    mff_cycles(0xF59E, 4);
    m = 0x06;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF5A0, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1558), -1); return; }
    mff_cycles(0xF5A2, 4);
    m = 0x1F;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF5A4, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1558), -1); return; }
    mff_cycles(0xF5A6, 4);
    m = 0x0B;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF5A8, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1558), -1); return; }
    mff_cycles(0xF5AA, 4);
    m = 0x25;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF5AC, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1558), -1); return; }
    mff_cycles(0xF5AE, 4);
    m = 0x0F;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF5B0, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1558), -1); return; }
    mff_cycles(0xF5B2, 4);
    m = 0x24;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF5B4, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1558), -1); return; }
    mff_cycles(0xF5B6, 2);
    if (!z) { mff_cycles(0xF5B6, 1); goto L_F5BF; }
L_F5B8:;
    mff_cycles(0xF5B8, 8);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(TMP3) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF63F, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F63F(); g_code_window_base = swb; } return;
L_F5BF:;
    mff_cycles(0xF5BF, 9);
    a = RAM(OBJ_Y);
    if (a == RAM(PLAYER_GROUND_Y)) { mff_cycles(0xF5C4, 1); goto L_F5CC; }
    mff_cycles(0xF5C6, 7);
    a = 0x01;
    RAM(TMP1) = a;
    if (a != 0) { mff_cycles(0xF5CA, 1); goto L_F5D0; }
L_F5CC:;
    mff_cycles(0xF5CC, 5);
    a = 0x00;
    RAM(TMP1) = a;
L_F5D0:;
    mff_cycles(0xF5D0, 8);
    a = RAM(OBJ_ANIM + x);
    if (a == 0x84) { mff_cycles(0xF5D5, 1); goto L_F5E3; }
    mff_cycles(0xF5D7, 4);
    if (a == 0xAC) { mff_cycles(0xF5D9, 1); goto L_F5E3; }
    mff_cycles(0xF5DB, 4);
    if (a == 0xE7) { mff_cycles(0xF5DD, 1); goto L_F5E3; }
    mff_cycles(0xF5DF, 4);
    if (a != 0xBE) { mff_cycles(0xF5E1, 1); goto L_F5E9; }
L_F5E3:;
    mff_cycles(0xF5E3, 8);
    a = RAM(TMP1);
    a |= 0x02;
    RAM(TMP1) = a;
L_F5E9:;
    mff_cycles(0xF5E9, 7);
    a = RAM(TMP1);
    if (a == 0x01) { mff_cycles(0xF5ED, 2); goto L_F615; }
    mff_cycles(0xF5EF, 4);
    if (a == 0x02) { mff_cycles(0xF5F1, 2); goto L_F601; }
    mff_cycles(0xF5F3, 4);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF5F5, 2); goto L_F628; }
    mff_cycles(0xF5F7, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x15F9, func_F365, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF5FA, 4);
    m = 0x0C;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xF5FC, 2); goto L_F63B; }
    mff_cycles(0xF5FE, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF679, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F679(); g_code_window_base = swb; } return;
L_F601:;
    mff_cycles(0xF601, 12);
    a = RAM(OBJ_Y);
    c = 1;
    m = RAM(OBJ_GROUND_Y + x);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xF60A, 4);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xF608, 1);
    }
    mff_cycles(0xF60E, 4);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xF610, 1); goto L_F63B; }
    mff_cycles(0xF612, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF679, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F679(); g_code_window_base = swb; } return;
L_F615:;
    mff_cycles(0xF615, 11);
    a = RAM(PLAYER_GROUND_Y);
    c = 1;
    m = RAM(OBJ_Y + x);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xF61D, 4);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xF61B, 1);
    }
    mff_cycles(0xF621, 4);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xF623, 1); goto L_F63B; }
    mff_cycles(0xF625, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF679, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F679(); g_code_window_base = swb; } return;
L_F628:;
    mff_cycles(0xF628, 11);
    a = RAM(PLAYER_GROUND_Y);
    c = 1;
    m = RAM(OBJ_GROUND_Y + x);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xF630, 4);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xF62E, 1);
    }
    mff_cycles(0xF634, 4);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0xF638, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF679, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F679(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xF636, 1);
    }
L_F63B:;
    mff_cycles(0xF63B, 40);
    a = 0xFF;
    RAM(TMP3) = a;
    a = RAM(OBJ_ATTACK + x);
    a &= 0x1F;
    y = a;
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(OBJ_X + x);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = RAM(OBJ_X_HI + x);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = mff_pull();
    if (c) { mff_cycles(0xF654, 1); goto L_F65C; }
    mff_cycles(0xF656, 6);
    a ^= 0xFF;
    m = 0x01;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    if (!c) { mff_cycles(0xF65A, 1); goto L_F67C; }
L_F65C:;
    mff_cycles(0xF65C, 8);
    RAM(TMP0) = a;
    a = RAM(TMP3);
    if (a != 0) { mff_cycles(0xF660, 1); goto L_F66B; }
    mff_cycles(0xF662, 8);
    a = RAM(OBJ_DIR);
    a &= 0x01;
    z = (a) == 0;
    if (z) { mff_cycles(0xF667, 1); goto L_F672; }
    mff_cycles(0xF669, 2);
    if (!z) { mff_cycles(0xF669, 1); goto L_F692; }
L_F66B:;
    mff_cycles(0xF66B, 8);
    a = RAM(OBJ_FLAGS + x);
    a &= 0x40;
    if (a == 0) { mff_cycles(0xF670, 1); goto L_F692; }
L_F672:;
    mff_cycles(0xF672, 7);
    a = RAM(TMP0);
    m = prg_read((uint16_t)(0xF6EA + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xF677, 2);
    if (!c) { mff_cycles(0xF677, 1); goto L_F699; }
    mff_cycles(0xF679, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF6A9, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F6A9(); g_code_window_base = swb; } return;
L_F67C:;
    mff_cycles(0xF67C, 8);
    RAM(TMP0) = a;
    a = RAM(TMP3);
    if (a != 0) { mff_cycles(0xF680, 1); goto L_F68B; }
    mff_cycles(0xF682, 8);
    a = RAM(OBJ_DIR);
    a &= 0x01;
    z = (a) == 0;
    if (z) { mff_cycles(0xF687, 1); goto L_F692; }
    mff_cycles(0xF689, 2);
    if (!z) { mff_cycles(0xF672, 1); goto L_F672; }
L_F68B:;
    mff_cycles(0xF68B, 8);
    a = RAM(OBJ_FLAGS + x);
    a &= 0x40;
    if (a == 0) { mff_cycles(0xF672, 1); goto L_F672; }
L_F692:;
    mff_cycles(0xF692, 7);
    a = RAM(TMP0);
    m = prg_read((uint16_t)(0xF6CA + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xF697, 2);
    if (c) { mff_cycles(0xF697, 1); goto L_F6A9; }
L_F699:;
    mff_cycles(0xF699, 12);
    a = RAM(OBJ_Y);
    c = 1;
    m = RAM(OBJ_Y + x);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xF6A2, 4);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xF6A0, 1);
    }
    mff_cycles(0xF6A6, 4);
    m = prg_read((uint16_t)(0xF6AA + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
L_F6A9:;
    mff_cycles(0xF6A9, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F63F - lifted by port/tools/lift.py */
void func_F63F(void) {
    REGS_IN;
    mff_cycles(0xF63F, 35);
    a = RAM(OBJ_ATTACK + x);
    a &= 0x1F;
    y = a;
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(OBJ_X + x);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = RAM(OBJ_X_HI + x);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = mff_pull();
    if (c) { mff_cycles(0xF654, 1); goto L_F65C; }
    mff_cycles(0xF656, 6);
    a ^= 0xFF;
    m = 0x01;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    if (!c) { mff_cycles(0xF65A, 1); goto L_F67C; }
L_F65C:;
    mff_cycles(0xF65C, 8);
    RAM(TMP0) = a;
    a = RAM(TMP3);
    if (a != 0) { mff_cycles(0xF660, 1); goto L_F66B; }
    mff_cycles(0xF662, 8);
    a = RAM(OBJ_DIR);
    a &= 0x01;
    z = (a) == 0;
    if (z) { mff_cycles(0xF667, 1); goto L_F672; }
    mff_cycles(0xF669, 2);
    if (!z) { mff_cycles(0xF669, 1); goto L_F692; }
L_F66B:;
    mff_cycles(0xF66B, 8);
    a = RAM(OBJ_FLAGS + x);
    a &= 0x40;
    if (a == 0) { mff_cycles(0xF670, 1); goto L_F692; }
L_F672:;
    mff_cycles(0xF672, 7);
    a = RAM(TMP0);
    m = prg_read((uint16_t)(0xF6EA + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xF677, 2);
    if (!c) { mff_cycles(0xF677, 1); goto L_F699; }
    mff_cycles(0xF679, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF6A9, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F6A9(); g_code_window_base = swb; } return;
L_F67C:;
    mff_cycles(0xF67C, 8);
    RAM(TMP0) = a;
    a = RAM(TMP3);
    if (a != 0) { mff_cycles(0xF680, 1); goto L_F68B; }
    mff_cycles(0xF682, 8);
    a = RAM(OBJ_DIR);
    a &= 0x01;
    z = (a) == 0;
    if (z) { mff_cycles(0xF687, 1); goto L_F692; }
    mff_cycles(0xF689, 2);
    if (!z) { mff_cycles(0xF672, 1); goto L_F672; }
L_F68B:;
    mff_cycles(0xF68B, 8);
    a = RAM(OBJ_FLAGS + x);
    a &= 0x40;
    if (a == 0) { mff_cycles(0xF672, 1); goto L_F672; }
L_F692:;
    mff_cycles(0xF692, 7);
    a = RAM(TMP0);
    m = prg_read((uint16_t)(0xF6CA + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xF697, 2);
    if (c) { mff_cycles(0xF697, 1); goto L_F6A9; }
L_F699:;
    mff_cycles(0xF699, 12);
    a = RAM(OBJ_Y);
    c = 1;
    m = RAM(OBJ_Y + x);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xF6A2, 4);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xF6A0, 1);
    }
    mff_cycles(0xF6A6, 4);
    m = prg_read((uint16_t)(0xF6AA + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
L_F6A9:;
    mff_cycles(0xF6A9, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F679 - lifted by port/tools/lift.py */
void func_F679(void) {
    REGS_IN;
    mff_cycles(0xF679, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF6A9, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F6A9(); g_code_window_base = swb; } return;
}

/*  $F6A9 - lifted by port/tools/lift.py */
void func_F6A9(void) {
    REGS_IN;
    mff_cycles(0xF6A9, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/* find_free_slot $F71A - lifted by port/tools/lift.py */
void func_F71A(void) {
    REGS_IN;
    mff_cycles(0xF71A, 2);
    y = 0x0D;
L_F71C:;
    mff_cycles(0xF71C, 6);
    a = RAM(OBJ_STATE + y);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0xF71F, 1); goto L_F728; }
    mff_cycles(0xF721, 6);
    y--;
    m = 0x02;
    n = ((uint8_t)(y - m)) >> 7;
    z = ((uint8_t)(y - m)) == 0;
    if (!z) { mff_cycles(0xF71C, 1); goto L_F71C; }
    mff_cycles(0xF726, 8);
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
L_F728:;
    mff_cycles(0xF728, 8);
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
}

/* find_free_effect_slot $F72A - lifted by port/tools/lift.py */
void func_F72A(void) {
    REGS_IN;
    goto L_F72A;
L_F728:;
    mff_cycles(0xF728, 8);
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
L_F72A:;
    mff_cycles(0xF72A, 2);
    y = 0x0F;
L_F72C:;
    mff_cycles(0xF72C, 6);
    a = RAM(OBJ_STATE + y);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0xF728, 1); goto L_F728; }
    mff_cycles(0xF731, 6);
    y--;
    m = 0x0D;
    n = ((uint8_t)(y - m)) >> 7;
    z = ((uint8_t)(y - m)) == 0;
    if (!z) { mff_cycles(0xF72C, 1); goto L_F72C; }
    mff_cycles(0xF736, 8);
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
}

/* player_attack_test $F738 - lifted by port/tools/lift.py */
void func_F738(void) {
    REGS_IN;
    mff_cycles(0xF738, 6);
    a = RAM(OBJ_FLAGS + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0xF73B, 1); goto L_F745; }
    mff_cycles(0xF73D, 4);
    a &= 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xF73F, 1); goto L_F745; }
    mff_cycles(0xF741, 5);
    a = RAM(GAME_ACTION);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xF743, 1); goto L_F747; }
L_F745:;
    mff_cycles(0xF745, 8);
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
L_F747:;
    mff_cycles(0xF747, 6);
    a = RAM(OBJ_INVULN + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xF745, 1); goto L_F745; }
    mff_cycles(0xF74C, 8);
    a = RAM(OBJ_ID + x);
    m = 0x1F;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (a < m) { mff_cycles(0xF745, 1); goto L_F745; }
    mff_cycles(0xF753, 4);
    m = 0x69;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF745, 1); goto L_F745; }
    mff_cycles(0xF757, 4);
    m = 0x6A;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF745, 1); goto L_F745; }
    mff_cycles(0xF75B, 4);
    m = 0x6C;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF745, 1); goto L_F745; }
    mff_cycles(0xF75F, 4);
    m = 0x95;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF745, 1); goto L_F745; }
    mff_cycles(0xF763, 4);
    m = 0x63;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF745, 1); goto L_F745; }
    mff_cycles(0xF767, 4);
    m = 0x61;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF745, 1); goto L_F745; }
    mff_cycles(0xF76B, 5);
    a = RAM(ATTACK_LIVE);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xF745, 1); goto L_F745; }
    mff_cycles(0xF76F, 8);
    a = RAM(OBJ_ID + x);
    m = 0x5F;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF745, 1); goto L_F745; }
    mff_cycles(0xF776, 15);
    a = RAM(OBJ_Y + x);
    RAM(TMP4) = a;
    a = RAM(OBJ_ANIM + x);
    if (a == 0x84) { mff_cycles(0xF780, 1); goto L_F78E; }
    mff_cycles(0xF782, 4);
    if (a == 0xAC) { mff_cycles(0xF784, 1); goto L_F78E; }
    mff_cycles(0xF786, 4);
    if (a == 0xE7) { mff_cycles(0xF788, 1); goto L_F78E; }
    mff_cycles(0xF78A, 4);
    if (a != 0xBE) { mff_cycles(0xF78C, 1); goto L_F794; }
L_F78E:;
    mff_cycles(0xF78E, 9);
    a = RAM(OBJ_GROUND_Y + x);
    RAM(OBJ_Y + x) = a;
L_F794:;
    mff_cycles(0xF794, 7);
    a = RAM(ATTACK_ID);
    m = 0x0F;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF798, 1); goto L_F7CA; }
    mff_cycles(0xF79A, 4);
    m = 0x10;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF79C, 1); goto L_F7CA; }
    mff_cycles(0xF79E, 4);
    if (a == 0x11) { mff_cycles(0xF7A0, 1); goto L_F7EA; }
    mff_cycles(0xF7A2, 4);
    if (a == 0x12) { mff_cycles(0xF7A4, 1); goto L_F7EA; }
    mff_cycles(0xF7A6, 4);
    if (a == 0x14) { mff_cycles(0xF7A8, 1); goto L_F7D0; }
    mff_cycles(0xF7AA, 4);
    if (a == 0x15) { mff_cycles(0xF7AC, 1); goto L_F7D0; }
    mff_cycles(0xF7AE, 4);
    if (a == 0x16) { mff_cycles(0xF7B0, 1); goto L_F7D0; }
    mff_cycles(0xF7B2, 4);
    if (a == 0x17) { mff_cycles(0xF7B4, 1); goto L_F7D0; }
    mff_cycles(0xF7B6, 4);
    m = 0x07;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF7B8, 1); goto L_F7CD; }
    mff_cycles(0xF7BA, 4);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF7BC, 1); goto L_F7CD; }
    mff_cycles(0xF7BE, 4);
    m = 0x04;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF7C0, 1); goto L_F7CD; }
    mff_cycles(0xF7C2, 4);
    if (a == 0x0D) { mff_cycles(0xF7C4, 1); goto L_F7EF; }
    mff_cycles(0xF7C6, 4);
    m = 0x18;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF7C8, 1); goto L_F7EF; }
L_F7CA:;
    mff_cycles(0xF7CA, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF7EA, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F7EA(); g_code_window_base = swb; } return;
L_F7CD:;
    mff_cycles(0xF7CD, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF7EF, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F7EF(); g_code_window_base = swb; } return;
L_F7D0:;
    mff_cycles(0xF7D0, 11);
    a = RAM(ATTACK_GROUND_Y);
    c = 1;
    m = RAM(OBJ_Y + x);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xF7D8, 4);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xF7D6, 1);
    }
    mff_cycles(0xF7DC, 4);
    m = 0x14;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0xF7E0, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF7FF, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F7FF(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xF7DE, 1);
    }
    mff_cycles(0xF7E3, 16);
    a = RAM(TMP4);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + x) = a;
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
L_F7EA:;
    mff_cycles(0xF7EA, 6);
    a = RAM(OBJ_Y);
    if (a != 0) { mff_cycles(0xF7ED, 1); goto L_F7F1; }
L_F7EF:;
    mff_cycles(0xF7EF, 3);
    a = RAM(ATTACK_GROUND_Y);
L_F7F1:;
    mff_cycles(0xF7F1, 8);
    c = 1;
    m = RAM(OBJ_Y + x);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xF7F7, 4);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xF7F5, 1);
    }
    mff_cycles(0xF7FB, 4);
    m = 0x0C;
    if (a >= m) { mff_cycles(0xF7FD, 2); goto L_F862; }
    mff_cycles(0xF7FF, 7);
    a = RAM(ATTACK_ID);
    if (a == 0x1B) { mff_cycles(0xF803, 1); goto L_F80D; }
    mff_cycles(0xF805, 4);
    if (a == 0x1C) { mff_cycles(0xF807, 1); goto L_F80D; }
    mff_cycles(0xF809, 4);
    if (a != 0x1D) { mff_cycles(0xF80B, 1); goto L_F811; }
L_F80D:;
    mff_cycles(0xF80D, 5);
    m = RAM(GRAB_SLOT);
    if (x != m) { mff_cycles(0xF80F, 1); goto L_F862; }
L_F811:;
    mff_cycles(0xF811, 30);
    y = RAM(ATTACK_ID);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(OBJ_X + x);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = RAM(OBJ_X_HI + x);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = mff_pull();
    if (!c) {
        mff_cycles(0xF824, 7);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF83D, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F83D(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xF822, 1);
    }
    mff_cycles(0xF82B, 10);
    RAM(TMP0) = a;
    a = RAM(PLAYER_DIR);
    a &= 0x01;
    if (a == 0) { mff_cycles(0xF831, 1); goto L_F845; }
    mff_cycles(0xF833, 7);
    a = RAM(TMP0);
    m = prg_read((uint16_t)(0xF8B1 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xF838, 2);
    if (c) { mff_cycles(0xF838, 1); goto L_F862; }
    mff_cycles(0xF83A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF84C, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F84C(); g_code_window_base = swb; } return;
L_F845:;
    mff_cycles(0xF845, 7);
    a = RAM(TMP0);
    m = prg_read((uint16_t)(0xF88D + y));
    mff_cycles(0xF84A, 2);
    if (a < m) {
        mff_cycles(0xF84C, 20);
        a = RAM(TMP4);
        RAM(OBJ_Y + x) = a;
        a = RAM(OBJ_Y);
        c = 1;
        m = RAM(OBJ_Y + x);
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        if (!c) {
            mff_cycles(0xF85A, 4);
            a ^= 0xFF;
            m = 0x01;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
        } else {
            mff_cycles(0xF858, 1);
        }
        mff_cycles(0xF85E, 4);
        m = prg_read((uint16_t)(0xF869 + y));
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        mff_cycles(0xF861, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xF84A, 1);
    }
L_F862:;
    mff_cycles(0xF862, 16);
    a = RAM(TMP4);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + x) = a;
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F7EA - lifted by port/tools/lift.py */
void func_F7EA(void) {
    REGS_IN;
    mff_cycles(0xF7EA, 6);
    a = RAM(OBJ_Y);
    if (a == 0) {
        mff_cycles(0xF7EF, 3);
        a = RAM(ATTACK_GROUND_Y);
    } else {
        mff_cycles(0xF7ED, 1);
    }
    mff_cycles(0xF7F1, 8);
    c = 1;
    m = RAM(OBJ_Y + x);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xF7F7, 4);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xF7F5, 1);
    }
    mff_cycles(0xF7FB, 4);
    m = 0x0C;
    if (a >= m) { mff_cycles(0xF7FD, 2); goto L_F862; }
    mff_cycles(0xF7FF, 7);
    a = RAM(ATTACK_ID);
    if (a == 0x1B) { mff_cycles(0xF803, 1); goto L_F80D; }
    mff_cycles(0xF805, 4);
    if (a == 0x1C) { mff_cycles(0xF807, 1); goto L_F80D; }
    mff_cycles(0xF809, 4);
    if (a != 0x1D) { mff_cycles(0xF80B, 1); goto L_F811; }
L_F80D:;
    mff_cycles(0xF80D, 5);
    m = RAM(GRAB_SLOT);
    if (x != m) { mff_cycles(0xF80F, 1); goto L_F862; }
L_F811:;
    mff_cycles(0xF811, 30);
    y = RAM(ATTACK_ID);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(OBJ_X + x);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = RAM(OBJ_X_HI + x);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = mff_pull();
    if (!c) {
        mff_cycles(0xF824, 7);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF83D, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F83D(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xF822, 1);
    }
    mff_cycles(0xF82B, 10);
    RAM(TMP0) = a;
    a = RAM(PLAYER_DIR);
    a &= 0x01;
    if (a == 0) { mff_cycles(0xF831, 1); goto L_F845; }
    mff_cycles(0xF833, 7);
    a = RAM(TMP0);
    m = prg_read((uint16_t)(0xF8B1 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xF838, 2);
    if (c) { mff_cycles(0xF838, 1); goto L_F862; }
    mff_cycles(0xF83A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF84C, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F84C(); g_code_window_base = swb; } return;
L_F845:;
    mff_cycles(0xF845, 7);
    a = RAM(TMP0);
    m = prg_read((uint16_t)(0xF88D + y));
    mff_cycles(0xF84A, 2);
    if (a < m) {
        mff_cycles(0xF84C, 20);
        a = RAM(TMP4);
        RAM(OBJ_Y + x) = a;
        a = RAM(OBJ_Y);
        c = 1;
        m = RAM(OBJ_Y + x);
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        if (!c) {
            mff_cycles(0xF85A, 4);
            a ^= 0xFF;
            m = 0x01;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
        } else {
            mff_cycles(0xF858, 1);
        }
        mff_cycles(0xF85E, 4);
        m = prg_read((uint16_t)(0xF869 + y));
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        mff_cycles(0xF861, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xF84A, 1);
    }
L_F862:;
    mff_cycles(0xF862, 16);
    a = RAM(TMP4);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + x) = a;
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F7EF - lifted by port/tools/lift.py */
void func_F7EF(void) {
    REGS_IN;
    mff_cycles(0xF7EF, 11);
    a = RAM(ATTACK_GROUND_Y);
    c = 1;
    m = RAM(OBJ_Y + x);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xF7F7, 4);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xF7F5, 1);
    }
    mff_cycles(0xF7FB, 4);
    m = 0x0C;
    if (a >= m) { mff_cycles(0xF7FD, 2); goto L_F862; }
    mff_cycles(0xF7FF, 7);
    a = RAM(ATTACK_ID);
    if (a == 0x1B) { mff_cycles(0xF803, 1); goto L_F80D; }
    mff_cycles(0xF805, 4);
    if (a == 0x1C) { mff_cycles(0xF807, 1); goto L_F80D; }
    mff_cycles(0xF809, 4);
    if (a != 0x1D) { mff_cycles(0xF80B, 1); goto L_F811; }
L_F80D:;
    mff_cycles(0xF80D, 5);
    m = RAM(GRAB_SLOT);
    if (x != m) { mff_cycles(0xF80F, 1); goto L_F862; }
L_F811:;
    mff_cycles(0xF811, 30);
    y = RAM(ATTACK_ID);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(OBJ_X + x);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = RAM(OBJ_X_HI + x);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = mff_pull();
    if (!c) {
        mff_cycles(0xF824, 7);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF83D, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F83D(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xF822, 1);
    }
    mff_cycles(0xF82B, 10);
    RAM(TMP0) = a;
    a = RAM(PLAYER_DIR);
    a &= 0x01;
    if (a == 0) { mff_cycles(0xF831, 1); goto L_F845; }
    mff_cycles(0xF833, 7);
    a = RAM(TMP0);
    m = prg_read((uint16_t)(0xF8B1 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xF838, 2);
    if (c) { mff_cycles(0xF838, 1); goto L_F862; }
    mff_cycles(0xF83A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF84C, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F84C(); g_code_window_base = swb; } return;
L_F845:;
    mff_cycles(0xF845, 7);
    a = RAM(TMP0);
    m = prg_read((uint16_t)(0xF88D + y));
    mff_cycles(0xF84A, 2);
    if (a < m) {
        mff_cycles(0xF84C, 20);
        a = RAM(TMP4);
        RAM(OBJ_Y + x) = a;
        a = RAM(OBJ_Y);
        c = 1;
        m = RAM(OBJ_Y + x);
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        if (!c) {
            mff_cycles(0xF85A, 4);
            a ^= 0xFF;
            m = 0x01;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
        } else {
            mff_cycles(0xF858, 1);
        }
        mff_cycles(0xF85E, 4);
        m = prg_read((uint16_t)(0xF869 + y));
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        mff_cycles(0xF861, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xF84A, 1);
    }
L_F862:;
    mff_cycles(0xF862, 16);
    a = RAM(TMP4);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + x) = a;
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F7FF - lifted by port/tools/lift.py */
void func_F7FF(void) {
    REGS_IN;
    mff_cycles(0xF7FF, 7);
    a = RAM(ATTACK_ID);
    if (a == 0x1B) { mff_cycles(0xF803, 1); goto L_F80D; }
    mff_cycles(0xF805, 4);
    if (a == 0x1C) { mff_cycles(0xF807, 1); goto L_F80D; }
    mff_cycles(0xF809, 4);
    if (a != 0x1D) { mff_cycles(0xF80B, 1); goto L_F811; }
L_F80D:;
    mff_cycles(0xF80D, 5);
    m = RAM(GRAB_SLOT);
    if (x != m) { mff_cycles(0xF80F, 1); goto L_F862; }
L_F811:;
    mff_cycles(0xF811, 30);
    y = RAM(ATTACK_ID);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(OBJ_X + x);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = RAM(OBJ_X_HI + x);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = mff_pull();
    if (!c) {
        mff_cycles(0xF824, 7);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF83D, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F83D(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xF822, 1);
    }
    mff_cycles(0xF82B, 10);
    RAM(TMP0) = a;
    a = RAM(PLAYER_DIR);
    a &= 0x01;
    if (a == 0) { mff_cycles(0xF831, 1); goto L_F845; }
    mff_cycles(0xF833, 7);
    a = RAM(TMP0);
    m = prg_read((uint16_t)(0xF8B1 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xF838, 2);
    if (c) { mff_cycles(0xF838, 1); goto L_F862; }
    mff_cycles(0xF83A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF84C, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F84C(); g_code_window_base = swb; } return;
L_F845:;
    mff_cycles(0xF845, 7);
    a = RAM(TMP0);
    m = prg_read((uint16_t)(0xF88D + y));
    mff_cycles(0xF84A, 2);
    if (a < m) {
        mff_cycles(0xF84C, 20);
        a = RAM(TMP4);
        RAM(OBJ_Y + x) = a;
        a = RAM(OBJ_Y);
        c = 1;
        m = RAM(OBJ_Y + x);
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        if (!c) {
            mff_cycles(0xF85A, 4);
            a ^= 0xFF;
            m = 0x01;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
        } else {
            mff_cycles(0xF858, 1);
        }
        mff_cycles(0xF85E, 4);
        m = prg_read((uint16_t)(0xF869 + y));
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        mff_cycles(0xF861, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xF84A, 1);
    }
L_F862:;
    mff_cycles(0xF862, 16);
    a = RAM(TMP4);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + x) = a;
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F833 - lifted by port/tools/lift.py */
void func_F833(void) {
    REGS_IN;
    mff_cycles(0xF833, 7);
    a = RAM(TMP0);
    m = prg_read((uint16_t)(0xF8B1 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xF838, 2);
    if (!c) {
        mff_cycles(0xF83A, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF84C, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F84C(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xF838, 1);
    }
    mff_cycles(0xF862, 16);
    a = RAM(TMP4);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + x) = a;
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F83D - lifted by port/tools/lift.py */
void func_F83D(void) {
    REGS_IN;
    mff_cycles(0xF83D, 10);
    RAM(TMP0) = a;
    a = RAM(PLAYER_DIR);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xF843, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1833), -1); return; }
    mff_cycles(0xF845, 7);
    a = RAM(TMP0);
    m = prg_read((uint16_t)(0xF88D + y));
    mff_cycles(0xF84A, 2);
    if (a < m) {
        mff_cycles(0xF84C, 20);
        a = RAM(TMP4);
        RAM(OBJ_Y + x) = a;
        a = RAM(OBJ_Y);
        c = 1;
        m = RAM(OBJ_Y + x);
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        if (!c) {
            mff_cycles(0xF85A, 4);
            a ^= 0xFF;
            m = 0x01;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
        } else {
            mff_cycles(0xF858, 1);
        }
        mff_cycles(0xF85E, 4);
        m = prg_read((uint16_t)(0xF869 + y));
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        mff_cycles(0xF861, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xF84A, 1);
    }
    mff_cycles(0xF862, 16);
    a = RAM(TMP4);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + x) = a;
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F84C - lifted by port/tools/lift.py */
void func_F84C(void) {
    REGS_IN;
    mff_cycles(0xF84C, 20);
    a = RAM(TMP4);
    RAM(OBJ_Y + x) = a;
    a = RAM(OBJ_Y);
    c = 1;
    m = RAM(OBJ_Y + x);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xF85A, 4);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xF858, 1);
    }
    mff_cycles(0xF85E, 4);
    m = prg_read((uint16_t)(0xF869 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xF861, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/* player_take_hit $F8D5 - lifted by port/tools/lift.py */
void func_F8D5(void) {
    REGS_IN;
    mff_cycles(0xF8D5, 5);
    a = RAM(PLAYER_NO_HIT);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0xF8D7, 1); goto L_F8DA; }
L_F8D9:;
    mff_cycles(0xF8D9, 6);
    SYNC_OUT;
    mff_rts(); return;
L_F8DA:;
    mff_cycles(0xF8DA, 6);
    a = RAM(OBJ_STATE + 1);
    if (!(a & 0x80)) { mff_cycles(0xF8DD, 1); goto L_F8ED; }
    mff_cycles(0xF8DF, 8);
    a = RAM(OBJ_ANIM + 1);
    m = 0x74;
    c = a >= m;
    if (a != m) { mff_cycles(0xF8E4, 1); goto L_F8ED; }
    mff_cycles(0xF8E6, 8);
    a = 0x00;
    RAM(OBJ_STATE + 1) = a;
    if (a == 0) { mff_cycles(0xF8EB, 1); goto L_F8FE; }
L_F8ED:;
    mff_cycles(0xF8ED, 6);
    a = RAM(OBJ_STATE + 2);
    if (a & 0x80) {
        mff_cycles(0xF8F2, 8);
        a = RAM(OBJ_ANIM + 2);
        m = 0x74;
        c = a >= m;
        if (a == m) {
            mff_cycles(0xF8F9, 6);
            a = 0x00;
            RAM(OBJ_STATE + 2) = a;
        } else {
            mff_cycles(0xF8F7, 1);
        }
    } else {
        mff_cycles(0xF8F0, 1);
    }
L_F8FE:;
    mff_cycles(0xF8FE, 5);
    a = RAM(PLAYER_INVULN);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xF8D9, 2); goto L_F8D9; }
    mff_cycles(0xF902, 4);
    m = 0x3C;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xF8D9, 2); goto L_F8D9; }
    mff_cycles(0xF906, 7);
    a = RAM(GAME_ACTION);
    m = 0x04;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF90A, 1); goto L_F90E; }
    mff_cycles(0xF90C, 2);
    if (!z) { mff_cycles(0xF90C, 1); goto L_F915; }
L_F90E:;
    mff_cycles(0xF90E, 10);
    a = 0xFF;
    y = RAM(GRAB_SLOT);
    RAM(OBJ_HELD + y) = a;
L_F915:;
    mff_cycles(0xF915, 5);
    a = RAM(WEAPON_HELD);
    if (a != 0) {
        mff_cycles(0xF919, 7);
        a = RAM(PLAYER_ID);
        m = 0x01;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xF91F, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x1921, func_FAC4, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xF91D, 1);
        }
    } else {
        mff_cycles(0xF917, 1);
    }
    mff_cycles(0xF922, 13);
    y = RAM(OBJ_ATTACK_DAMAGE + x);
    a = RAM(HEALTH);
    c = 1;
    m = prg_read((uint16_t)(0xFD16 + y));
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    mff_cycles(0xF92B, 5);
    RAM(HEALTH) = a;
    if (a & 0x80) { mff_cycles(0xF92D, 1); goto L_F999; }
    mff_cycles(0xF92F, 4);
    a = prg_read((uint16_t)(0xFD56 + y));
    mff_cycles(0xF932, 2);
    if (a != 0) { mff_cycles(0xF932, 1); goto L_F999; }
    mff_cycles(0xF934, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE207 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xF939, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x193B, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF93C, 4);
    a = prg_read((uint16_t)(0xE24F + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xF93F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1941, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF942, 34);
    a = RAM(OBJ_DIR + x);
    a ^= 0x03;
    t = a;
    m = t & 1;
    t = (uint8_t)(t >> 1 | c << 7);
    c = m;
    a = t;
    t = a;
    m = t & 1;
    t = (uint8_t)(t >> 1 | c << 7);
    c = m;
    a = t;
    t = a;
    t = (uint8_t)(t >> 1 | c << 7);
    a = t;
    a &= 0x40;
    RAM(TMP0) = a;
    a = RAM(OBJ_FLAGS);
    a &= 0xBF;
    a |= RAM(TMP0);
    RAM(OBJ_FLAGS) = a;
    if (a == 0x90) { mff_cycles(0xF95A, 1); goto L_F960; }
    mff_cycles(0xF95C, 4);
    a = 0x01;
    if (a != 0) { mff_cycles(0xF95E, 1); goto L_F962; }
L_F960:;
    mff_cycles(0xF960, 2);
    a = 0x02;
L_F962:;
    mff_cycles(0xF962, 10);
    RAM(PLAYER_DIR) = a;
    a = RAM(GAME_ACTION);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0xF968, 1); goto L_F9B7; }
    mff_cycles(0xF96A, 4);
    m = 0x14;
    c = a >= m;
    if (a == m) { mff_cycles(0xF96C, 1); goto L_F9B7; }
    mff_cycles(0xF96E, 12);
    a = 0x05;
    RAM(GAME_ACTION) = a;
    a = RAM(PLAYER_ID);
    m = 0x01;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xF978, 17);
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(ATTACK_LIVE) = a;
        RAM(WEAPON_HELD) = a;
        RAM(COMBO_TIMER) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xF976, 1);
    }
    mff_cycles(0xF981, 5);
    a = RAM(WEAPON_HELD);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0xF985, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1987, func_F436, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xF988, 16);
        a = 0x00;
        RAM(COMBO_TIMER) = a;
        a = 0xFF;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(WEAPON_HELD) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xF983, 1);
    }
    mff_cycles(0xF991, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1993, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF994, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(COMBO_TIMER) = a;
    SYNC_OUT;
    mff_rts(); return;
L_F999:;
    mff_cycles(0xF999, 34);
    a = RAM(OBJ_DIR + x);
    a ^= 0x03;
    t = a;
    m = t & 1;
    t = (uint8_t)(t >> 1 | c << 7);
    c = m;
    a = t;
    t = a;
    m = t & 1;
    t = (uint8_t)(t >> 1 | c << 7);
    c = m;
    a = t;
    t = a;
    t = (uint8_t)(t >> 1 | c << 7);
    a = t;
    a &= 0x40;
    RAM(TMP0) = a;
    a = RAM(OBJ_FLAGS);
    a &= 0xBF;
    a |= RAM(TMP0);
    RAM(OBJ_FLAGS) = a;
    m = 0x90;
    c = a >= m;
    if (a == m) { mff_cycles(0xF9B1, 1); goto L_F9B7; }
    mff_cycles(0xF9B3, 4);
    a = 0x01;
    if (a != 0) { mff_cycles(0xF9B5, 1); goto L_F9B9; }
L_F9B7:;
    mff_cycles(0xF9B7, 2);
    a = 0x02;
L_F9B9:;
    mff_cycles(0xF9B9, 10);
    RAM(PLAYER_DIR) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE20A + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xF9C0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x19C2, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF9C3, 4);
    a = prg_read((uint16_t)(0xE252 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xF9C6, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x19C8, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xF9C9, 15);
    a = 0x00;
    RAM(ATTACK_LIVE) = a;
    RAM(COMBO_TIMER) = a;
    a = RAM(PLAYER_ID);
    m = 0x01;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xF9D5, 8);
        a = 0x00;
        RAM(WEAPON_HELD) = a;
        RAM(WEAPON_AMMO) = a;
    } else {
        mff_cycles(0xF9D3, 1);
    }
    mff_cycles(0xF9DB, 17);
    a = 0x06;
    RAM(GAME_ACTION) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F9E5 - lifted by port/tools/lift.py */
void func_F9E5(void) {
    REGS_IN;
    mff_cycles(0xF9E5, 5);
    a = 0x02;
    RAM(TMP16) = a;
L_F9E9:;
    mff_cycles(0xF9E9, 9);
    y = RAM(TMP16);
    a = RAM(OBJ_STATE + y);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0xF9EE, 2); goto L_FA5D; }
    mff_cycles(0xF9F0, 6);
    a = RAM(OBJ_FLAGS + y);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0xF9F3, 2); goto L_FA5D; }
    mff_cycles(0xF9F5, 8);
    a = RAM(OBJ_ID + y);
    m = 0x66;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF9FA, 2); goto L_FA5D; }
    mff_cycles(0xF9FC, 4);
    m = 0x69;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF9FE, 1); goto L_FA5D; }
    mff_cycles(0xFA00, 4);
    m = 0x09;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xFA02, 1); goto L_FA5D; }
    mff_cycles(0xFA04, 6);
    a = RAM(OBJ_STATE + x);
    if (!(a & 0x80)) { mff_cycles(0xFA07, 1); goto L_FA84; }
    mff_cycles(0xFA09, 6);
    a = RAM(OBJ_ID + x);
    if (a == 0) { mff_cycles(0xFA0C, 1); goto L_FA84; }
    mff_cycles(0xFA0E, 4);
    if (a == 0x05) { mff_cycles(0xFA10, 1); goto L_FA84; }
    mff_cycles(0xFA12, 4);
    if (a == 0x06) { mff_cycles(0xFA14, 1); goto L_FA84; }
    mff_cycles(0xFA16, 4);
    if (a == 0x07) { mff_cycles(0xFA18, 1); goto L_FA84; }
    mff_cycles(0xFA1A, 4);
    if (a == 0x6C) { mff_cycles(0xFA1C, 1); goto L_FA84; }
    mff_cycles(0xFA1E, 4);
    if (a == 0x6D) { mff_cycles(0xFA20, 1); goto L_FA84; }
    mff_cycles(0xFA22, 4);
    if (a == 0x66) { mff_cycles(0xFA24, 1); goto L_FA84; }
    mff_cycles(0xFA26, 4);
    if (a == 0x69) { mff_cycles(0xFA28, 1); goto L_FA84; }
    mff_cycles(0xFA2A, 4);
    if (a == 0x6A) { mff_cycles(0xFA2C, 1); goto L_FA84; }
    mff_cycles(0xFA2E, 4);
    if (a == 0x05) { mff_cycles(0xFA30, 1); goto L_FA84; }
    mff_cycles(0xFA32, 4);
    if (a == 0x10) { mff_cycles(0xFA34, 1); goto L_FA84; }
    mff_cycles(0xFA36, 4);
    if (a == 0x11) { mff_cycles(0xFA38, 1); goto L_FA84; }
    mff_cycles(0xFA3A, 4);
    if (a == 0x12) { mff_cycles(0xFA3C, 1); goto L_FA84; }
    mff_cycles(0xFA3E, 4);
    if (a == 0x13) { mff_cycles(0xFA40, 1); goto L_FA84; }
    mff_cycles(0xFA42, 4);
    if (a == 0x60) { mff_cycles(0xFA44, 1); goto L_FA84; }
    mff_cycles(0xFA46, 4);
    if (a == 0x95) { mff_cycles(0xFA48, 1); goto L_FA84; }
    mff_cycles(0xFA4A, 4);
    if (a == 0x61) { mff_cycles(0xFA4C, 1); goto L_FA84; }
    mff_cycles(0xFA4E, 8);
    a = RAM(OBJ_ANIM + x);
    if (a == 0x74) { mff_cycles(0xFA53, 1); goto L_FA84; }
    mff_cycles(0xFA55, 4);
    if (a == 0x7F) { mff_cycles(0xFA57, 1); goto L_FA84; }
    mff_cycles(0xFA59, 4);
    m = 0x79;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xFA5B, 1); goto L_FA60; }
L_FA5D:;
    mff_cycles(0xFA5D, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xFA84, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_FA84(); g_code_window_base = swb; } return;
L_FA60:;
    mff_cycles(0xFA60, 6);
    a = RAM(OBJ_INVULN + x);
    if (a != 0) { mff_cycles(0xFA63, 1); goto L_FA84; }
    mff_cycles(0xFA65, 6);
    a = RAM(OBJ_FLAGS + x);
    if (!(a & 0x80)) { mff_cycles(0xFA68, 1); goto L_FA84; }
    mff_cycles(0xFA6A, 4);
    a &= 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xFA6C, 1); goto L_FA8F; }
    mff_cycles(0xFA6E, 30);
    a = RAM(OBJ_X + y);
    RAM(TMP0) = a;
    a = RAM(OBJ_X_HI + y);
    RAM(TMP1) = a;
    a = RAM(OBJ_Y + y);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(TMP2) = a;
    RAM(TMP15) = y;
    SYNC_OUT;
    if (!mff_call_fn(0x1A81, func_FA91, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xFA82, 2);
    if (!c) { mff_cycles(0xFA82, 1); goto L_FA90; }
L_FA84:;
    mff_cycles(0xFA84, 12);
    t = (uint8_t)(RAM(TMP16) - 1);
    RAM(TMP16) = t;
    a = RAM(TMP16);
    m = 0x00;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xFA8A, 1); goto L_FA8F; }
    mff_cycles(0xFA8C, 3);
    goto L_F9E9;
L_FA8F:;
    mff_cycles(0xFA8F, 2);
    c = 1;
L_FA90:;
    mff_cycles(0xFA90, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $F9E9 - lifted by port/tools/lift.py */
void func_F9E9(void) {
    REGS_IN;
L_F9E9:;
    mff_cycles(0xF9E9, 9);
    y = RAM(TMP16);
    a = RAM(OBJ_STATE + y);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0xF9EE, 2); goto L_FA5D; }
    mff_cycles(0xF9F0, 6);
    a = RAM(OBJ_FLAGS + y);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0xF9F3, 2); goto L_FA5D; }
    mff_cycles(0xF9F5, 8);
    a = RAM(OBJ_ID + y);
    m = 0x66;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF9FA, 2); goto L_FA5D; }
    mff_cycles(0xF9FC, 4);
    m = 0x69;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xF9FE, 1); goto L_FA5D; }
    mff_cycles(0xFA00, 4);
    m = 0x09;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xFA02, 1); goto L_FA5D; }
    mff_cycles(0xFA04, 6);
    a = RAM(OBJ_STATE + x);
    if (!(a & 0x80)) { mff_cycles(0xFA07, 1); goto L_FA84; }
    mff_cycles(0xFA09, 6);
    a = RAM(OBJ_ID + x);
    if (a == 0) { mff_cycles(0xFA0C, 1); goto L_FA84; }
    mff_cycles(0xFA0E, 4);
    if (a == 0x05) { mff_cycles(0xFA10, 1); goto L_FA84; }
    mff_cycles(0xFA12, 4);
    if (a == 0x06) { mff_cycles(0xFA14, 1); goto L_FA84; }
    mff_cycles(0xFA16, 4);
    if (a == 0x07) { mff_cycles(0xFA18, 1); goto L_FA84; }
    mff_cycles(0xFA1A, 4);
    if (a == 0x6C) { mff_cycles(0xFA1C, 1); goto L_FA84; }
    mff_cycles(0xFA1E, 4);
    if (a == 0x6D) { mff_cycles(0xFA20, 1); goto L_FA84; }
    mff_cycles(0xFA22, 4);
    if (a == 0x66) { mff_cycles(0xFA24, 1); goto L_FA84; }
    mff_cycles(0xFA26, 4);
    if (a == 0x69) { mff_cycles(0xFA28, 1); goto L_FA84; }
    mff_cycles(0xFA2A, 4);
    if (a == 0x6A) { mff_cycles(0xFA2C, 1); goto L_FA84; }
    mff_cycles(0xFA2E, 4);
    if (a == 0x05) { mff_cycles(0xFA30, 1); goto L_FA84; }
    mff_cycles(0xFA32, 4);
    if (a == 0x10) { mff_cycles(0xFA34, 1); goto L_FA84; }
    mff_cycles(0xFA36, 4);
    if (a == 0x11) { mff_cycles(0xFA38, 1); goto L_FA84; }
    mff_cycles(0xFA3A, 4);
    if (a == 0x12) { mff_cycles(0xFA3C, 1); goto L_FA84; }
    mff_cycles(0xFA3E, 4);
    if (a == 0x13) { mff_cycles(0xFA40, 1); goto L_FA84; }
    mff_cycles(0xFA42, 4);
    if (a == 0x60) { mff_cycles(0xFA44, 1); goto L_FA84; }
    mff_cycles(0xFA46, 4);
    if (a == 0x95) { mff_cycles(0xFA48, 1); goto L_FA84; }
    mff_cycles(0xFA4A, 4);
    if (a == 0x61) { mff_cycles(0xFA4C, 1); goto L_FA84; }
    mff_cycles(0xFA4E, 8);
    a = RAM(OBJ_ANIM + x);
    if (a == 0x74) { mff_cycles(0xFA53, 1); goto L_FA84; }
    mff_cycles(0xFA55, 4);
    if (a == 0x7F) { mff_cycles(0xFA57, 1); goto L_FA84; }
    mff_cycles(0xFA59, 4);
    m = 0x79;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xFA5B, 1); goto L_FA60; }
L_FA5D:;
    mff_cycles(0xFA5D, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xFA84, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_FA84(); g_code_window_base = swb; } return;
L_FA60:;
    mff_cycles(0xFA60, 6);
    a = RAM(OBJ_INVULN + x);
    if (a != 0) { mff_cycles(0xFA63, 1); goto L_FA84; }
    mff_cycles(0xFA65, 6);
    a = RAM(OBJ_FLAGS + x);
    if (!(a & 0x80)) { mff_cycles(0xFA68, 1); goto L_FA84; }
    mff_cycles(0xFA6A, 4);
    a &= 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xFA6C, 1); goto L_FA8F; }
    mff_cycles(0xFA6E, 30);
    a = RAM(OBJ_X + y);
    RAM(TMP0) = a;
    a = RAM(OBJ_X_HI + y);
    RAM(TMP1) = a;
    a = RAM(OBJ_Y + y);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(TMP2) = a;
    RAM(TMP15) = y;
    SYNC_OUT;
    if (!mff_call_fn(0x1A81, func_FA91, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xFA82, 2);
    if (!c) { mff_cycles(0xFA82, 1); goto L_FA90; }
L_FA84:;
    mff_cycles(0xFA84, 12);
    t = (uint8_t)(RAM(TMP16) - 1);
    RAM(TMP16) = t;
    a = RAM(TMP16);
    m = 0x00;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xFA8A, 1); goto L_FA8F; }
    mff_cycles(0xFA8C, 3);
    goto L_F9E9;
L_FA8F:;
    mff_cycles(0xFA8F, 2);
    c = 1;
L_FA90:;
    mff_cycles(0xFA90, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $FA84 - lifted by port/tools/lift.py */
void func_FA84(void) {
    REGS_IN;
    mff_cycles(0xFA84, 12);
    t = (uint8_t)(RAM(TMP16) - 1);
    RAM(TMP16) = t;
    a = RAM(TMP16);
    m = 0x00;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xFA8C, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF9E9, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F9E9(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xFA8A, 1);
    }
    mff_cycles(0xFA8F, 8);
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $FA91 - lifted by port/tools/lift.py */
void func_FA91(void) {
    REGS_IN;
    mff_cycles(0xFA91, 33);
    a = RAM(OBJ_ATTACK + y);
    y = a;
    a = RAM(TMP0);
    c = 1;
    m = RAM(OBJ_X + x);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    mff_push(a);
    a = RAM(TMP1);
    m = RAM(OBJ_X_HI + x);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = mff_pull();
    if (!c) {
        mff_cycles(0xFAA5, 4);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xFAA3, 1);
    }
    mff_cycles(0xFAA9, 4);
    m = prg_read((uint16_t)(0xFAC2 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xFAAC, 2);
    if (!c) {
        mff_cycles(0xFAAE, 11);
        a = RAM(TMP2);
        c = 1;
        m = RAM(OBJ_Y + x);
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        if (!c) {
            mff_cycles(0xFAB6, 4);
            a ^= 0xFF;
            m = 0x01;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
        } else {
            mff_cycles(0xFAB4, 1);
        }
        mff_cycles(0xFABA, 4);
        m = prg_read((uint16_t)(0xFAC0 + y));
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        mff_cycles(0xFABD, 2);
        if (!c) {
            mff_cycles(0xFABD, 1);
        }
    } else {
        mff_cycles(0xFAAC, 1);
    }
    mff_cycles(0xFABF, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/* drop_weapon $FAC4 - lifted by port/tools/lift.py */
void func_FAC4(void) {
    REGS_IN;
    mff_cycles(0xFAC4, 51);
    a = 0x80;
    RAM(OBJ_STATE + 1) = a;
    a = 0x90;
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
    a = prg_read((uint16_t)(0xFB06 + y));
    mff_cycles(0xFAEB, 39);
    RAM(OBJ_ANIM + 1) = a;
    a = 0x00;
    RAM(OBJ_ANIM_TIMER + 1) = a;
    RAM(OBJ_ANIM_FRAME + 1) = a;
    a = RAM(PLAYER_DIR);
    RAM(OBJ_DIR + 1) = a;
    a = 0x66;
    RAM(OBJ_ID + 1) = a;
    a = 0x2F;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ATTACK + 1) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/* obj_hit_start $FB09 - lifted by port/tools/lift.py */
void func_FB09(void) {
    REGS_IN;
    mff_cycles(0xFB09, 7);
    y = RAM(ATTACK_ID);
    a = prg_read((uint16_t)(0xFB17 + y));
    mff_cycles(0xFB0E, 18);
    RAM(OBJ_HIT_COOLDOWN + x) = a;
    a = 0x1E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_HIT_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/* reset $FE00 - lifted by port/tools/lift.py */
void func_FE00(void) {
    REGS_IN;
    mff_cycles(0xFE00, 10);
    g_cpu.I = 1;
    g_cpu.D = 0;
    a = 0x08;
    hw_write(PPU_CTRL, a);
    mff_cycles(0xFE07, 6);
    a = 0x40;
    hw_write(JOY2, a);
    mff_cycles(0xFE0C, 6);
    x = 0x00;
    hw_write(PPU_MASK, x);
    mff_cycles(0xFE11, 4);
    hw_write(APU_DMC, x);
    mff_cycles(0xFE14, 4);
    hw_write(APU_STATUS, x);
    mff_cycles(0xFE17, 6);
    x--;
    g_cpu.S = x;
    x = 0x04;
    for (;;) {
        for (;;) {
            mff_cycles(0xFE1B, 4);
            a = hw_read(PPU_STATUS);
            mff_cycles(0xFE1E, 2);
            if (a & 0x80) break;
            mff_cycles(0xFE1B, 1);
        }
        for (;;) {
            mff_cycles(0xFE20, 4);
            a = hw_read(PPU_STATUS);
            mff_cycles(0xFE23, 2);
            if (!(a & 0x80)) break;
            mff_cycles(0xFE20, 1);
        }
        mff_cycles(0xFE25, 4);
        x--;
        if (x == 0) break;
        mff_cycles(0xFE1B, 1);
    }
    mff_cycles(0xFE28, 4);
    a = hw_read(PPU_STATUS);
    mff_cycles(0xFE2B, 4);
    a = 0x10;
    y = a;
    for (;;) {
        mff_cycles(0xFE2E, 4);
        hw_write(PPU_ADDR, a);
        mff_cycles(0xFE31, 4);
        hw_write(PPU_ADDR, a);
        mff_cycles(0xFE34, 6);
        a ^= 0x10;
        y--;
        if (y == 0) break;
        mff_cycles(0xFE2E, 1);
    }
    mff_cycles(0xFE39, 2);
    a = y;
    for (;;) {
        mff_cycles(0xFE3A, 9);
        RAM(TMP0 + y) = a;
        y--;
        if (y == 0) break;
        mff_cycles(0xFE3A, 1);
    }
    for (;;) {
        mff_cycles(0xFE40, 5);
        t = (uint8_t)(RAM(TMP1) + 1);
        RAM(TMP1) = t;
        for (;;) {
            mff_cycles(0xFE42, 6);
            mem_write((uint16_t)(RAM16(TMP0) + y), a);
            mff_cycles(0xFE44, 4);
            y++;
            if (y == 0) break;
            mff_cycles(0xFE42, 1);
        }
        mff_cycles(0xFE47, 7);
        x = RAM(TMP1);
        m = 0x07;
        c = x >= m;
        if (x == m) break;
        mff_cycles(0xFE40, 1);
    }
    mff_cycles(0xFE4D, 4);
    y = 0x07;
    a = 0x88;
    for (;;) {
        mff_cycles(0xFE51, 8);
        RAM((SOUND_QUEUE + x) & 0xFF) = a;
        x--;
        if (x & 0x80) break;
        mff_cycles(0xFE51, 1);
    }
    mff_cycles(0xFE56, 11);
    a = 0x18;
    RAM(PPUMASK_SHADOW) = a;
    a = 0x01;
    hw_write(MMC3_MIRRORING, a);
    mff_cycles(0xFE5F, 16);
    x = 0x0C;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x1E68, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xFE69, 6);
    x = 0x00;
    hw_write(MMC3_BANK_SELECT, x);
    mff_cycles(0xFE6E, 6);
    a = 0x40;
    hw_write(MMC3_BANK_DATA, a);
    mff_cycles(0xFE73, 6);
    x++;
    hw_write(MMC3_BANK_SELECT, x);
    mff_cycles(0xFE77, 6);
    a = 0x42;
    hw_write(MMC3_BANK_DATA, a);
    mff_cycles(0xFE7C, 36);
    a = 0x40;
    RAM(CHR_BANKS) = a;
    a = 0x42;
    RAM(CHR_BANKS + 1) = a;
    a = 0x00;
    RAM(CHR_BANKS + 2) = a;
    a = 0x01;
    RAM(CHR_BANKS + 3) = a;
    a = 0x02;
    RAM(CHR_BANKS + 4) = a;
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(CHR_BANKS + 5) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1E96, 0xC31D, -1)) return;
    SYNC_IN;
    mff_cycles(0xFE97, 12);
    a = 0x20;
    x = 0x00;
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x1E9F, 0xC2D1, -1)) return;
    SYNC_IN;
    mff_cycles(0xFEA0, 12);
    a = 0x24;
    x = 0x00;
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x1EA8, 0xC2D1, -1)) return;
    SYNC_IN;
    mff_cycles(0xFEA9, 18);
    a = 0xC5;
    RAM(TASK_ENTRY + 1) = a;
    a = 0x56;
    RAM(TASK_ENTRY) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1EB5, func_FF02, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xFEB6, 5);
    a = 0x88;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PPUCTRL_SHADOW) = a;
    SYNC_OUT;
    { void func_FEBA(void); func_FEBA(); } return;   /* written by hand */
}

/*  $FF4C - lifted by port/tools/lift.py */
void func_FF4C(void) {
    REGS_IN;
    mff_cycles(0xFF4C, 6);
    SYNC_OUT;
    mff_rts(); return;
}
