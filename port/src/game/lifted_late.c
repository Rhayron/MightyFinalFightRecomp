/*
 * lifted_late.c - routines found after the bank lifts (table entries no route
 * used, continuation points), lifted by port/tools/lift_bank.py --into from
 * the pure scaffold: exact, still to be tidied and moved into their
 * systems' files.
 */
#include "mff.h"
#include "state.h"
#include "coroutine.h"

void func_E2A3(void);
void func_E6AB(void);
void func_E9BC(void);
void func_F000(void);
void func_F03F(void);
void func_F048(void);
void func_F051(void);
void func_F119(void);
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
void func_F34C(void);
void func_F374(void);
void func_F430(void);
void func_F436(void);
void func_F43B(void);
void func_F440(void);
void func_F449(void);
void func_F455(void);
void func_F71A(void);
void func_F738(void);
void func_F9E5(void);
void func_FB3E(void);
void func_FF4D(void);

/*  $A54C (bank 0) - lifted by port/tools/lift.py */
void func_A54C_b0(void) {
    REGS_IN;
    mff_cycles(0xA54C, 20);
    a = 0x80;
    RAM(OBJ_STATE + x) = a;
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DOWN_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A6FC (bank 0) - lifted by port/tools/lift.py */
void func_A6FC_b0(void) {
    REGS_IN;
    mff_cycles(0xA6FC, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x06FE, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA6FF, 22);
    a = 0x80;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_HIT_TIMER + x) = a;
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A7BB (bank 0) - lifted by port/tools/lift.py */
void func_A7BB_b0(void) {
    REGS_IN;
    mff_cycles(0xA7BB, 59);
    a = 0x8E;
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x03;
    RAM(OBJ_VY + x) = a;
    a = RAM(OBJ_ID + x);
    RAM(OBJ_SAVED_ID + x) = a;
    a = 0x05;
    RAM(OBJ_ID + x) = a;
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
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

/*  $BED2 (bank 1) - lifted by port/tools/lift.py */
void func_BED2_b1(void) {
    REGS_IN;
    mff_cycles(0xBED2, 12);
    a = 0xFF;
    RAM(OBJ_INVULN + x) = a;
    a = RAM(GAME_ACTION);
    if (a == 0) { mff_cycles(0xBED9, 1); goto L_BEEB; }
    mff_cycles(0xBEDB, 5);
    a = RAM(ATTACK_ID);
    if (a == 0) { mff_cycles(0xBEDD, 1); goto L_BEEB; }
    mff_cycles(0xBEDF, 4);
    m = 0x06;
    c = a >= m;
    if (a == m) { mff_cycles(0xBEE1, 1); goto L_BEEB; }
    mff_cycles(0xBEE3, 4);
    m = 0x0B;
    c = a >= m;
    if (a == m) { mff_cycles(0xBEE5, 1); goto L_BEEB; }
    mff_cycles(0xBEE7, 4);
    m = 0x0C;
    c = a >= m;
    if (a != m) { mff_cycles(0xBEE9, 2); goto L_BF31; }
L_BEEB:;
    mff_cycles(0xBEEB, 97);
    a = 0x0A;
    RAM(GAME_ACTION) = a;
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    RAM(OBJ_VX + x) = a;
    a = 0x99;
    RAM(OBJ_VX_FRAC + x) = a;
    a = RAM(PLAYER_DIR);
    RAM(OBJ_DIR + x) = a;
    a = RAM(OBJ_GROUND_Y + x);
    RAM(OBJ_LAND_Y + x) = a;
    a = 0x8E;
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x03;
    RAM(OBJ_VY + x) = a;
    a = 0x0B;
    RAM(OBJ_KIND + x) = a;
    a = 0x05;
    RAM(OBJ_ID + x) = a;
    a = 0x9B;
    RAM(OBJ_SAVED_ID + x) = a;
    a = 0x81;
    RAM(OBJ_STATE + x) = a;
    a = 0x10;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0xC4;
    y = 0x39;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF449, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F449(); g_code_window_base = swb; } return;
L_BF31:;
    mff_cycles(0xBF31, 7);
    a = 0xC3;
    y = 0x3A;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF449, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F449(); g_code_window_base = swb; } return;
}

/*  $B543 (bank 5) - lifted by port/tools/lift.py */
void func_B543_b5(void) {
    REGS_IN;
    mff_cycles(0xB543, 32);
    a = 0x02;
    RAM(OBJ_DIR + y) = a;
    a = 0xFF;
    RAM(OBJ_X + y) = a;
    a = 0x8C;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + y) = a;
    RAM(OBJ_TIMER2 + y) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1557, 0xB5AF, 5)) return;
    SYNC_IN;
    mff_cycles(0xB558, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BAE8 (bank 5) - lifted by port/tools/lift.py */
void func_BAE8_b5(void) {
    REGS_IN;
    mff_cycles(0xBAE8, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (z) {
        mff_cycles(0xBAED, 6);
        SYNC_OUT;
        if (!mff_call_at(0x1AEF, 0xBCD9, 5)) return;
        SYNC_IN;
        mff_cycles(0xBAF0, 20);
        a = 0x80;
        RAM(OBJ_STATE + x) = a;
        a = 0x0F;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_INVULN + x) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xBAEB, 1);
    }
    mff_cycles(0xBAFB, 14);
    t = (uint8_t)(RAM(OBJ_INVULN + x) + 1);
    RAM(OBJ_INVULN + x) = t;
    a = 0xC6;
    y = 0x3A;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF449, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F449(); g_code_window_base = swb; } return;
}

/*  $BBAC (bank 5) - lifted by port/tools/lift.py */
void func_BBAC_b5(void) {
    REGS_IN;
    mff_cycles(0xBBAC, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1BAE, 0xBCD9, 5)) return;
    SYNC_IN;
    mff_cycles(0xBBAF, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1BB1, 0xBCC8, 5)) return;
    SYNC_IN;
    mff_cycles(0xBBB2, 8);
    a = RAM(OBJ_DIR + x);
    if (a != 0x01) { mff_cycles(0xBBB7, 1); goto L_BBCA; }
    mff_cycles(0xBBB9, 8);
    a = RAM(OBJ_X_HI + x);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xBBBE, 1); goto L_BBC7; }
    mff_cycles(0xBBC0, 8);
    a = RAM(OBJ_X + x);
    m = 0x17;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xBBC5, 1); goto L_BBDB; }
L_BBC7:;
    mff_cycles(0xBBC7, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF183, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F183(); g_code_window_base = swb; } return;
L_BBCA:;
    mff_cycles(0xBBCA, 8);
    a = RAM(OBJ_X_HI + x);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xBBCF, 1); goto L_BBD8; }
    mff_cycles(0xBBD1, 8);
    a = RAM(OBJ_X + x);
    m = 0xF1;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xBBD6, 1); goto L_BBDB; }
L_BBD8:;
    mff_cycles(0xBBD8, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF1A1, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F1A1(); g_code_window_base = swb; } return;
L_BBDB:;
    mff_cycles(0xBBDB, 13);
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}


/*  $C4A5 - lifted by port/tools/lift.py */
void func_C4A5(void) {
    REGS_IN;
    for (;;) {
        mff_cycles(0xC4A5, 4);
        a = prg_read((uint16_t)(0xAE7C + y));
        mff_cycles(0xC4A8, 41);
        mff_push(a);
        a &= 0x80;
        RAM(BG_PAL_ANIM_ON + x) = a;
        a = mff_pull();
        a &= 0x7F;
        RAM(BG_PAL_ANIM_SCRIPT + x) = a;
        a = 0x00;
        RAM(BG_PAL_ANIM_STEP + x) = a;
        RAM(BG_PAL_ANIM_TIMER + x) = a;
        y++;
        x++;
        m = 0x04;
        c = x >= m;
        n = ((uint8_t)(x - m)) >> 7;
        z = ((uint8_t)(x - m)) == 0;
        if (z) break;
        mff_cycles(0xC4A5, 1);
    }
    mff_cycles(0xC4C2, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC527, 2); call_by_address_tail(0xC527, -1); return;
}

/* gameplay_update $C93B - lifted by port/tools/lift.py */
void func_C93B(void) {
    REGS_IN;
    mff_cycles(0xC93B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x093D, 0xC9B6, -1)) return;
    SYNC_IN;
    mff_cycles(0xC93E, 16);
    x = 0x00;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0947, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC948, 6);
    SYNC_OUT;
    if (!mff_call_at(0x094A, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC94B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x094D, func_E2A3, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC94E, 29);
    a = RAM(OBJ_X);
    RAM(PLAYER_X_PREV) = a;
    a = RAM(SCROLL_X);
    RAM(SCROLL_X_PREV) = a;
    x = 0x08;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0960, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC961, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0963, 0x8080, -1)) return;
    SYNC_IN;
    mff_cycles(0xC964, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0966, 0x8900, -1)) return;
    SYNC_IN;
    mff_cycles(0xC967, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0969, 0xA300, -1)) return;
    SYNC_IN;
    mff_cycles(0xC96A, 16);
    x = 0x0C;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0973, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC974, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0976, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC977, 7);
    a = RAM(GAME_ACTION);
    m = 0x07;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xC97D, 4);
        m = 0x09;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xC981, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0983, func_E6AB, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xC984, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xC99D, 2); call_by_address_tail(0xC99D, -1); return;
        } else {
            mff_cycles(0xC97F, 1);
        }
    } else {
        mff_cycles(0xC97B, 1);
    }
    mff_cycles(0xC99D, 11);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OAM_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_at(0x09A3, 0xC31D, -1)) return;
    SYNC_IN;
    mff_cycles(0xC9A4, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x09A6, func_E9BC, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC9A7, 0);
    SYNC_OUT;
    { void func_C9A7(void); func_C9A7(); } return;   /* written by hand */
}

/*  $CA39 - lifted by port/tools/lift.py */
void func_CA39(void) {
    REGS_IN;
    mff_cycles(0xCA39, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCB07, 2); call_by_address_tail(0xCB07, -1); return;
}

/*  $CAF5 - lifted by port/tools/lift.py */
void func_CAF5(void) {
    REGS_IN;
    mff_cycles(0xCAF5, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCA3C, 2); call_by_address_tail(0xCA3C, -1); return;
}

/*  $CBA0 - lifted by port/tools/lift.py */
void func_CBA0(void) {
    REGS_IN;
    mff_cycles(0xCBA0, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCB22, 2); call_by_address_tail(0xCB22, -1); return;
}

/*  $CF33 - lifted by port/tools/lift.py */
void func_CF33(void) {
    REGS_IN;
    mff_cycles(0xCF33, 2);
    y = 0x0F;
    for (;;) {
        mff_cycles(0xCF35, 6);
        a = RAM(OBJ_STATE + y);
        if (a & 0x80) {
            mff_cycles(0xCF3A, 8);
            a = RAM(OBJ_ID + y);
            if (a != 0x69) {
                mff_cycles(0xCF41, 4);
                if (a != 0x10) {
                    mff_cycles(0xCF45, 4);
                    if (a != 0x11) {
                        mff_cycles(0xCF49, 4);
                        if (a != 0x12) {
                            mff_cycles(0xCF4D, 4);
                            if (a != 0x13) {
                                mff_cycles(0xCF51, 12);
                                a = RAM(OBJ_Y);
                                c = 1;
                                m = RAM(OBJ_Y + y);
                                w = a - m - !c;
                                v = ((a ^ m) & (a ^ w) & 0x80) != 0;
                                c = w >= 0;
                                a = (uint8_t)w;
                                if (!c) {
                                    mff_cycles(0xCF5A, 6);
                                    a ^= 0xFF;
                                    m = 0x01;
                                    w = a + m + c;
                                    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
                                    a = (uint8_t)w;
                                } else {
                                    mff_cycles(0xCF58, 1);
                                }
                                mff_cycles(0xCF5F, 4);
                                if (a < 0x10) {
                                    mff_cycles(0xCF63, 27);
                                    a = RAM(OBJ_X);
                                    c = 1;
                                    m = RAM(OBJ_X + y);
                                    w = a - m - !c;
                                    c = w >= 0;
                                    a = (uint8_t)w;
                                    mff_push(a);
                                    a = RAM(OBJ_X_HI);
                                    m = RAM(OBJ_X_HI + y);
                                    w = a - m - !c;
                                    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
                                    c = w >= 0;
                                    a = mff_pull();
                                    if (!c) {
                                        mff_cycles(0xCF74, 6);
                                        a ^= 0xFF;
                                        m = 0x01;
                                        w = a + m + c;
                                        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
                                        a = (uint8_t)w;
                                    } else {
                                        mff_cycles(0xCF72, 1);
                                    }
                                    mff_cycles(0xCF79, 4);
                                    m = 0x60;
                                    c = a >= m;
                                    if (!c) {
                                        mff_cycles(0xCF7D, 8);
                                        a = 0xFF;
                                        n = (a) >> 7;
                                        z = (a) == 0;
                                        SYNC_OUT;
                                        mff_rts(); return;
                                    } else {
                                        mff_cycles(0xCF7B, 1);
                                    }
                                } else {
                                    mff_cycles(0xCF61, 1);
                                }
                            } else {
                                mff_cycles(0xCF4F, 1);
                            }
                        } else {
                            mff_cycles(0xCF4B, 1);
                        }
                    } else {
                        mff_cycles(0xCF47, 1);
                    }
                } else {
                    mff_cycles(0xCF43, 1);
                }
            } else {
                mff_cycles(0xCF3F, 1);
            }
        } else {
            mff_cycles(0xCF38, 1);
        }
        mff_cycles(0xCF80, 6);
        y--;
        m = 0x03;
        c = y >= m;
        if (y == m) break;
        mff_cycles(0xCF35, 1);
    }
    mff_cycles(0xCF85, 8);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    mff_rts(); return;
}
