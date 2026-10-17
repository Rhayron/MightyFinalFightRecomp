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

/*  $B300 (bank 5) - lifted by port/tools/lift.py */
void func_B300_b5(void) {
    REGS_IN;
    mff_cycles(0xB300, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB331, 2); call_by_address_tail(0xB331, 5); return;
}

/*  $B303 (bank 5) - lifted by port/tools/lift.py */
void func_B303_b5(void) {
    REGS_IN;
    mff_cycles(0xB303, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB331, 2); call_by_address_tail(0xB331, 5); return;
}

/*  $B306 (bank 5) - lifted by port/tools/lift.py */
void func_B306_b5(void) {
    REGS_IN;
    mff_cycles(0xB306, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB5D7, 2); call_by_address_tail(0xB5D7, 5); return;
}

/*  $B309 (bank 5) - lifted by port/tools/lift.py */
void func_B309_b5(void) {
    REGS_IN;
    mff_cycles(0xB309, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 5); return;
}

/*  $B30C (bank 5) - lifted by port/tools/lift.py */
void func_B30C_b5(void) {
    REGS_IN;
    mff_cycles(0xB30C, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB696, 2); call_by_address_tail(0xB696, 5); return;
}

/*  $B30F (bank 5) - lifted by port/tools/lift.py */
void func_B30F_b5(void) {
    REGS_IN;
    mff_cycles(0xB30F, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 5); return;
}

/*  $B312 (bank 5) - lifted by port/tools/lift.py */
void func_B312_b5(void) {
    REGS_IN;
    mff_cycles(0xB312, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 5); return;
}

/*  $B315 (bank 5) - lifted by port/tools/lift.py */
void func_B315_b5(void) {
    REGS_IN;
    mff_cycles(0xB315, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 5); return;
}

/*  $B318 (bank 5) - lifted by port/tools/lift.py */
void func_B318_b5(void) {
    REGS_IN;
    mff_cycles(0xB318, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 5); return;
}

/*  $B31B (bank 5) - lifted by port/tools/lift.py */
void func_B31B_b5(void) {
    REGS_IN;
    mff_cycles(0xB31B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 5); return;
}

/*  $B31E (bank 5) - lifted by port/tools/lift.py */
void func_B31E_b5(void) {
    REGS_IN;
    mff_cycles(0xB31E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 5); return;
}

/*  $B321 (bank 5) - lifted by port/tools/lift.py */
void func_B321_b5(void) {
    REGS_IN;
    mff_cycles(0xB321, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBD7F, 2); call_by_address_tail(0xBD7F, 5); return;
}

/*  $B324 (bank 5) - lifted by port/tools/lift.py */
void func_B324_b5(void) {
    REGS_IN;
    mff_cycles(0xB324, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 5); return;
}

/*  $B327 (bank 5) - lifted by port/tools/lift.py */
void func_B327_b5(void) {
    REGS_IN;
    mff_cycles(0xB327, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBD7F, 2); call_by_address_tail(0xBD7F, 5); return;
}

/*  $B32A (bank 5) - lifted by port/tools/lift.py */
void func_B32A_b5(void) {
    REGS_IN;
    mff_cycles(0xB32A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 5); return;
}

/*  $B32D (bank 5) - lifted by port/tools/lift.py */
void func_B32D_b5(void) {
    REGS_IN;
    mff_cycles(0xB32D, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 5); return;
}

/*  $B330 (bank 5) - lifted by port/tools/lift.py */
void func_B330_b5(void) {
    REGS_IN;
    mff_cycles(0xB330, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B696 (bank 5) - lifted by port/tools/lift.py */
void func_B696_b5(void) {
    REGS_IN;
    mff_cycles(0xB696, 12);
    RAM(OBJ_SPAWN_GROUP + 15) = x;
    a = RAM(OBJ_STATE + x);
    if (a != 0x8A) {
        mff_cycles(0xB6A0, 9);
        a = RAM(OBJ_Y + x);
        RAM(OBJ_GROUND_Y + x) = a;
    } else {
        mff_cycles(0xB69E, 1);
    }
    mff_cycles(0xB6A6, 6);
    a = RAM(OBJ_INVULN + x);
    if (a != 0) {
        mff_cycles(0xB6AB, 7);
        t = (uint8_t)(RAM(OBJ_INVULN + x) - 1);
        RAM(OBJ_INVULN + x) = t;
    } else {
        mff_cycles(0xB6A9, 1);
    }
    mff_cycles(0xB6AE, 22);
    a = 0x0B;
    RAM(OBJ_KIND + x) = a;
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_ID + 13);
    m = 0x6E;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xB6BF, 9);
        t = (uint8_t)(RAM(OBJ_INVULN + x) + 1);
        RAM(OBJ_INVULN + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
        if (!z) {
            mff_cycles(0xB6C4, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xB743, 2); call_by_address_tail(0xB743, 5); return;
        } else {
            mff_cycles(0xB6C2, 1);
        }
    } else {
        mff_cycles(0xB6BD, 1);
    }
    mff_cycles(0xB6C7, 6);
    a = RAM(OBJ_HIT_TIMER + x);
    if (a != 0) {
        mff_cycles(0xB6CC, 10);
        a = 0xBC;
        y = 0x38;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x16D2, func_F449, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xB6D3, 7);
        a = 0x81;
        RAM(OBJ_STATE + x) = a;
    } else {
        mff_cycles(0xB6CA, 1);
    }
    mff_cycles(0xB6D8, 6);
    a = RAM(OBJ_HELD + x);
    if (a == 0) { mff_cycles(0xB6DB, 2); goto L_B74A; }
    mff_cycles(0xB6DD, 7);
    a = RAM(GAME_ACTION);
    if (a == 0x21) { mff_cycles(0xB6E1, 1); goto L_B6E7; }
    mff_cycles(0xB6E3, 4);
    if (a != 0x04) { mff_cycles(0xB6E5, 2); goto L_B74A; }
L_B6E7:;
    mff_cycles(0xB6E7, 5);
    if (x != RAM(GRAB_SLOT)) { mff_cycles(0xB6E9, 2); goto L_B74A; }
    mff_cycles(0xB6EB, 8);
    a = RAM(OBJ_LIFE + x);
    m = 0x26;
    c = a >= m;
    if (!c) { mff_cycles(0xB6F0, 2); goto L_B721; }
    mff_cycles(0xB6F2, 7);
    a = RAM(GAME_ACTION);
    m = 0x21;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xB6F8, 10);
        a = 0xC3;
        y = 0x3A;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x16FE, func_F449, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xB6FF, 10);
        a = 0x80;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_STATE + x) = a;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF343, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F343(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xB6F6, 2);
    }
    mff_cycles(0xB707, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(COMBO_TIMER) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x170D, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB70E, 12);
    a = 0x80;
    RAM(OBJ_STATE + x) = a;
    a = RAM(GRAB_TIMER);
    if (a & 0x80) { mff_cycles(0xB715, 1); goto L_B743; }
    mff_cycles(0xB717, 4);
    m = 0x0C;
    c = a >= m;
    if (!c) { mff_cycles(0xB719, 1); goto L_B743; }
    mff_cycles(0xB71B, 7);
    a = 0x0C;
    RAM(GRAB_TIMER) = a;
    if (a != 0) { mff_cycles(0xB71F, 1); goto L_B743; }
L_B721:;
    mff_cycles(0xB721, 30);
    a = 0x00;
    RAM(GAME_ACTION) = a;
    RAM(OBJ_HIT_TIMER + x) = a;
    RAM(OBJ_HELD + x) = a;
    RAM(GRAB_TIMER) = a;
    t = (uint8_t)(RAM(GRAB_TIMER) - 1);
    RAM(GRAB_TIMER) = t;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xBD7C + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB734, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1736, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB737, 16);
    a = 0x20;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x88;
    RAM(OBJ_STATE + x) = a;
    if (a != 0) { mff_cycles(0xB741, 1); goto L_B74A; }
L_B743:;
    mff_cycles(0xB743, 7);
    a = 0xBC;
    y = 0x38;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF449, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F449(); g_code_window_base = swb; } return;
L_B74A:;
    mff_cycles(0xB74A, 8);
    a = RAM(OBJ_LIFE + x);
    m = 0x5A;
    c = a >= m;
    if (!c) {
        mff_cycles(0xB751, 6);
        a = RAM(OBJ_TIMER2 + x);
        if (a == 0) {
            mff_cycles(0xB756, 6);
            a = RAM(OBJ_INVULN + x);
            if (a == 0) {
                mff_cycles(0xB75B, 46);
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
                a = 0xFF;
                RAM(OBJ_INVULN + x) = a;
            } else {
                mff_cycles(0xB759, 1);
            }
            mff_cycles(0xB77B, 20);
            a = 0x80;
            RAM(OBJ_STATE + x) = a;
            RAM(OBJ_SAVED_STATE + x) = a;
            a = 0xF2;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x1787, func_F34A, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xB788, 13);
            t = (uint8_t)(RAM(OBJ_TIMER2 + x) + 1);
            RAM(OBJ_TIMER2 + x) = t;
            n = (t) >> 7;
            z = (t) == 0;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xB754, 1);
        }
    } else {
        mff_cycles(0xB74F, 1);
    }
    mff_cycles(0xB78C, 8);
    a = RAM(OBJ_TIMER2 + x);
    m = 0x01;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xB793, 5);
        a = RAM(GAME_ACTION);
        if (a != 0) {
            mff_cycles(0xB797, 4);
            m = 0x03;
            c = a >= m;
            if (a != m) {
                mff_cycles(0xB79B, 4);
                m = 0x17;
                c = a >= m;
                if (a != m) {
                    mff_cycles(0xB79F, 13);
                    a = 0x01;
                    n = (a) >> 7;
                    z = (a) == 0;
                    RAM(OBJ_INVULN + x) = a;
                    SYNC_OUT;
                    mff_rts(); return;
                } else {
                    mff_cycles(0xB79D, 1);
                }
            } else {
                mff_cycles(0xB799, 1);
            }
        } else {
            mff_cycles(0xB795, 1);
        }
        mff_cycles(0xB7A5, 37);
        a = 0x3C;
        RAM(SPECIAL_SCREEN_ID) = a;
        a = 0x80;
        RAM(OBJ_STATE + x) = a;
        RAM(OBJ_SAVED_STATE + x) = a;
        t = (uint8_t)(RAM(OBJ_TIMER2 + x) + 1);
        RAM(OBJ_TIMER2 + x) = t;
        t = (uint8_t)(RAM(OBJ_TIER + x) + 1);
        RAM(OBJ_TIER + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xB791, 1);
    }
    mff_cycles(0xB7B8, 8);
    a = RAM(OBJ_STATE + x);
    m = 0x8A;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xB7BF, 8);
        a = RAM(OBJ_Y + x);
        if (a < 0x82) {
            mff_cycles(0xB7C6, 7);
            a = 0x82;
            RAM(OBJ_Y + x) = a;
        } else {
            mff_cycles(0xB7C4, 1);
        }
        mff_cycles(0xB7CB, 8);
        a = RAM(OBJ_Y + x);
        m = 0xAE;
        c = a >= m;
        if (c) {
            mff_cycles(0xB7D2, 7);
            a = 0xAE;
            RAM(OBJ_Y + x) = a;
        } else {
            mff_cycles(0xB7D0, 1);
        }
    } else {
        mff_cycles(0xB7BD, 1);
    }
    mff_cycles(0xB7D7, 6);
    a = RAM(OBJ_TIER + x);
    if (a == 0) {
        mff_cycles(0xB7DC, 12);
        a = RAM(OBJ_STATE + x);
        a &= 0x0F;
        y = a;
        a = prg_read((uint16_t)(0xBCE6 + y));
        mff_cycles(0xB7E5, 7);
        RAM(TMP0) = a;
        a = prg_read((uint16_t)(0xBCF7 + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0xB7EA, 8);
        RAM(TMP1) = a;
        SYNC_OUT;
        { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
    } else {
        mff_cycles(0xB7DA, 1);
    }
    mff_cycles(0xB7EF, 7);
    a = 0x06;
    m = RAM(MUSIC_CURRENT);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xB7F5, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x17F7, func_F34A, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xB7F3, 1);
    }
    mff_cycles(0xB7F8, 12);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    y = a;
    a = prg_read((uint16_t)(0xBCEC + y));
    mff_cycles(0xB801, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0xBCFD + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB806, 8);
    RAM(TMP1) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $B743 (bank 5) - lifted by port/tools/lift.py */
void func_B743_b5(void) {
    REGS_IN;
    mff_cycles(0xB743, 7);
    a = 0xBC;
    y = 0x38;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF449, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F449(); g_code_window_base = swb; } return;
}

/*  $B80B (bank 5) - lifted by port/tools/lift.py */
void func_B80B_b5(void) {
    REGS_IN;
    mff_cycles(0xB80B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x180D, 0xBBE1, 5)) return;
    SYNC_IN;
    mff_cycles(0xB80E, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1810, 0xBCD9, 5)) return;
    SYNC_IN;
    mff_cycles(0xB811, 35);
    a = 0x00;
    RAM(OBJ_AI_COUNT + x) = a;
    RAM(OBJ_AI_TIMER + x) = a;
    RAM(OBJ_AI_FLAG + x) = a;
    RAM(OBJ_TIMER2 + x) = a;
    a = 0x83;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B825 (bank 5) - lifted by port/tools/lift.py */
void func_B825_b5(void) {
    REGS_IN;
    mff_cycles(0xB825, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1827, 0xBCD9, 5)) return;
    SYNC_IN;
    mff_cycles(0xB828, 10);
    a = 0x20;
    y = 0x18;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x182E, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB82F, 2);
    if (c) {
        mff_cycles(0xB831, 20);
        t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_STATE + x) = t;
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_AI_TIMER + x) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xB82F, 1);
    }
    mff_cycles(0xB83A, 13);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    a = RAM(OBJ_AI_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0xB842, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1844, func_FB5B, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xB845, 13);
        y = a;
        a = 0x0A;
        RAM(OBJ_AI_TIMER + x) = a;
        a = prg_read((uint16_t)(0xFBF0 + y));
        mff_cycles(0xB84E, 9);
        RAM(OBJ_DIR + x) = a;
        a = prg_read((uint16_t)(0xBD1C + y));
        mff_cycles(0xB854, 9);
        RAM(OBJ_VX_FRAC + x) = a;
        a = prg_read((uint16_t)(0xBD0C + y));
        mff_cycles(0xB85A, 9);
        RAM(OBJ_VX + x) = a;
        a = prg_read((uint16_t)(0xBD3C + y));
        mff_cycles(0xB860, 9);
        RAM(OBJ_VY_FRAC + x) = a;
        a = prg_read((uint16_t)(0xBD2C + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0xB866, 5);
        RAM(OBJ_VY + x) = a;
    } else {
        mff_cycles(0xB840, 1);
    }
    mff_cycles(0xB869, 6);
    SYNC_OUT;
    if (!mff_call_at(0x186B, 0xBC6B, 5)) return;
    SYNC_IN;
    mff_cycles(0xB86C, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBBE1, 2); call_by_address_tail(0xBBE1, 5); return;
}

/*  $B84B (bank 5) - lifted by port/tools/lift.py */
void func_B84B_b5(void) {
    REGS_IN;
    mff_cycles(0xB84B, 4);
    a = prg_read((uint16_t)(0xFBF0 + y));
    mff_cycles(0xB84E, 9);
    RAM(OBJ_DIR + x) = a;
    a = prg_read((uint16_t)(0xBD1C + y));
    mff_cycles(0xB854, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = prg_read((uint16_t)(0xBD0C + y));
    mff_cycles(0xB85A, 9);
    RAM(OBJ_VX + x) = a;
    a = prg_read((uint16_t)(0xBD3C + y));
    mff_cycles(0xB860, 9);
    RAM(OBJ_VY_FRAC + x) = a;
    a = prg_read((uint16_t)(0xBD2C + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB866, 11);
    RAM(OBJ_VY + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x186B, 0xBC6B, 5)) return;
    SYNC_IN;
    mff_cycles(0xB86C, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBBE1, 2); call_by_address_tail(0xBBE1, 5); return;
}

/*  $B86F (bank 5) - lifted by port/tools/lift.py */
void func_B86F_b5(void) {
    REGS_IN;
    mff_cycles(0xB86F, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1871, 0xBBE1, 5)) return;
    SYNC_IN;
    mff_cycles(0xB872, 13);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    a = RAM(OBJ_AI_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0xB878, 1); goto L_B8B8; }
    mff_cycles(0xB87A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x187C, func_FB5B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB87D, 8);
    y = a;
    a = RAM(OBJ_AI_FLAG + x);
    if (a == 0) {
        mff_cycles(0xB883, 4);
        a = prg_read((uint16_t)(0xBD4C + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0xB886, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xB88C, 2); call_by_address_tail(0xB88C, 5); return;
    } else {
        mff_cycles(0xB881, 1);
    }
    mff_cycles(0xB889, 4);
    a = prg_read((uint16_t)(0xBD5C + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB88C, 7);
    RAM(OBJ_TIMER + x) = a;
    if (!n) { mff_cycles(0xB88F, 1); goto L_B8B8; }
    mff_cycles(0xB891, 10);
    a = 0x1C;
    y = 0x04;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1897, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB898, 2);
    if (c) { mff_cycles(0xB898, 1); goto L_B8C4; }
    mff_cycles(0xB89A, 10);
    a = 0x38;
    y = 0x18;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x18A0, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB8A1, 2);
    if (!c) {
        mff_cycles(0xB8A3, 20);
        a = 0x00;
        RAM(OBJ_AI_TIMER + x) = a;
        t = (uint8_t)(RAM(OBJ_STATE + x) - 1);
        RAM(OBJ_STATE + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xB8A1, 1);
    }
    mff_cycles(0xB8AC, 12);
    a = RAM(OBJ_TIMER + x);
    a &= 0x0F;
    y = a;
    a = prg_read((uint16_t)(0xBD6C + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB8B5, 5);
    RAM(OBJ_AI_TIMER + x) = a;
L_B8B8:;
    mff_cycles(0xB8B8, 6);
    SYNC_OUT;
    if (!mff_call_at(0x18BA, 0xBCD9, 5)) return;
    SYNC_IN;
    mff_cycles(0xB8BB, 11);
    a = RAM(OBJ_TIMER + x);
    a &= 0x0F;
    y = a;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB84B, 2); call_by_address_tail(0xB84B, 5); return;
L_B8C4:;
    mff_cycles(0xB8C4, 21);
    a = 0x00;
    RAM(OBJ_AI_COUNT + x) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
    a = 0xC0;
    y = 0x39;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF449, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F449(); g_code_window_base = swb; } return;
}

/*  $B88C (bank 5) - lifted by port/tools/lift.py */
void func_B88C_b5(void) {
    REGS_IN;
    mff_cycles(0xB88C, 7);
    RAM(OBJ_TIMER + x) = a;
    if (!n) { mff_cycles(0xB88F, 1); goto L_B8B8; }
    mff_cycles(0xB891, 10);
    a = 0x1C;
    y = 0x04;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1897, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB898, 2);
    if (c) { mff_cycles(0xB898, 1); goto L_B8C4; }
    mff_cycles(0xB89A, 10);
    a = 0x38;
    y = 0x18;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x18A0, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB8A1, 2);
    if (!c) {
        mff_cycles(0xB8A3, 20);
        a = 0x00;
        RAM(OBJ_AI_TIMER + x) = a;
        t = (uint8_t)(RAM(OBJ_STATE + x) - 1);
        RAM(OBJ_STATE + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xB8A1, 1);
    }
    mff_cycles(0xB8AC, 12);
    a = RAM(OBJ_TIMER + x);
    a &= 0x0F;
    y = a;
    a = prg_read((uint16_t)(0xBD6C + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB8B5, 5);
    RAM(OBJ_AI_TIMER + x) = a;
L_B8B8:;
    mff_cycles(0xB8B8, 6);
    SYNC_OUT;
    if (!mff_call_at(0x18BA, 0xBCD9, 5)) return;
    SYNC_IN;
    mff_cycles(0xB8BB, 11);
    a = RAM(OBJ_TIMER + x);
    a &= 0x0F;
    y = a;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB84B, 2); call_by_address_tail(0xB84B, 5); return;
L_B8C4:;
    mff_cycles(0xB8C4, 21);
    a = 0x00;
    RAM(OBJ_AI_COUNT + x) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
    a = 0xC0;
    y = 0x39;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF449, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F449(); g_code_window_base = swb; } return;
}

/*  $B8D3 (bank 5) - lifted by port/tools/lift.py */
void func_B8D3_b5(void) {
    REGS_IN;
    mff_cycles(0xB8D3, 10);
    a = 0x1C;
    y = 0x04;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x18D9, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB8DA, 2);
    if (!c) {
        mff_cycles(0xB8DC, 6);
        SYNC_OUT;
        if (!mff_call_at(0x18DE, 0xBCD9, 5)) return;
        SYNC_IN;
        mff_cycles(0xB8DF, 13);
        t = (uint8_t)(RAM(OBJ_STATE + x) - 1);
        RAM(OBJ_STATE + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xB8DA, 1);
    }
    mff_cycles(0xB8E3, 6);
    SYNC_OUT;
    if (!mff_call_at(0x18E5, 0xBB0A, 5)) return;
    SYNC_IN;
    mff_cycles(0xB8E6, 6);
    a = RAM(OBJ_HIT_PLAYER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0xB8EB, 8);
        a = 0x18;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x18EF, func_F34C, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xB8F0, 34);
        a = 0x00;
        RAM(OBJ_HIT_PLAYER + x) = a;
        RAM(OBJ_VX_FRAC + x) = a;
        a = 0x04;
        RAM(OBJ_VX + x) = a;
        a = 0x01;
        RAM(OBJ_DIR + x) = a;
        a = RAM(OBJ_FLAGS + x);
        a &= 0x40;
        n = (a) >> 7;
        z = (a) == 0;
        if (!z) {
            mff_cycles(0xB909, 7);
            a = 0x02;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_DIR + x) = a;
        } else {
            mff_cycles(0xB907, 1);
        }
        mff_cycles(0xB90E, 6);
        SYNC_OUT;
        if (!mff_call_at(0x1910, 0xBC6B, 5)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xB8E9, 2);
    }
    mff_cycles(0xB911, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1913, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB914, 17);
    a = 0x00;
    RAM(OBJ_VX_FRAC + x) = a;
    RAM(OBJ_VX + x) = a;
    a = RAM(RNG);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0xB920, 11);
        a = RAM(OBJ_AI_FLAG + x);
        a ^= 0xFF;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_AI_FLAG + x) = a;
    } else {
        mff_cycles(0xB91E, 1);
    }
    mff_cycles(0xB928, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B929 (bank 5) - lifted by port/tools/lift.py */
void func_B929_b5(void) {
    REGS_IN;
    mff_cycles(0xB929, 6);
    SYNC_OUT;
    if (!mff_call_at(0x192B, 0xBBE1, 5)) return;
    SYNC_IN;
    mff_cycles(0xB92C, 6);
    SYNC_OUT;
    if (!mff_call_at(0x192E, 0xBCD9, 5)) return;
    SYNC_IN;
    mff_cycles(0xB92F, 8);
    a = RAM(OBJ_X_HI + x);
    m = 0x02;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xB936, 15);
        a = 0x01;
        RAM(OBJ_DIR + x) = a;
        a = RAM(OBJ_X_HI + x);
        m = 0x01;
        c = a >= m;
        if (a != m) {
            mff_cycles(0xB942, 7);
            a = 0x02;
            RAM(OBJ_DIR + x) = a;
        } else {
            mff_cycles(0xB940, 1);
        }
        mff_cycles(0xB947, 20);
        a = 0x00;
        RAM(OBJ_DOWN_TIMER + x) = a;
        a = 0x82;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_STATE + x) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xB934, 1);
    }
    mff_cycles(0xB952, 13);
    a = 0x83;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B958 (bank 5) - lifted by port/tools/lift.py */
void func_B958_b5(void) {
    REGS_IN;
    mff_cycles(0xB958, 6);
    SYNC_OUT;
    if (!mff_call_at(0x195A, 0xBCD9, 5)) return;
    SYNC_IN;
    mff_cycles(0xB95B, 10);
    a = RAM(OBJ_X + x);
    m = RAM(OBJ_X);
    c = a >= m;
    if (!c) { mff_cycles(0xB961, 1); goto L_B967; }
    mff_cycles(0xB963, 4);
    a = 0x01;
    if (a != 0) { mff_cycles(0xB965, 1); goto L_B969; }
L_B967:;
    mff_cycles(0xB967, 2);
    a = 0x02;
L_B969:;
    mff_cycles(0xB969, 18);
    RAM(OBJ_DIR + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DOWN_TIMER + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1973, 0xBBE1, 5)) return;
    SYNC_IN;
    mff_cycles(0xB974, 20);
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B97B (bank 5) - lifted by port/tools/lift.py */
void func_B97B_b5(void) {
    REGS_IN;
    mff_cycles(0xB97B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x197D, 0xBBE1, 5)) return;
    SYNC_IN;
    mff_cycles(0xB97E, 6);
    a = RAM(OBJ_DOWN_TIMER + x);
    if (a != 0) { mff_cycles(0xB981, 1); goto L_B9A7; }
    mff_cycles(0xB983, 10);
    a = 0x40;
    y = 0x10;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1989, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB98A, 2);
    if (!c) { mff_cycles(0xB98A, 1); goto L_B9A7; }
    mff_cycles(0xB98C, 10);
    a = RAM(OBJ_Y + x);
    if (a < RAM(OBJ_Y)) { mff_cycles(0xB992, 1); goto L_B998; }
    mff_cycles(0xB994, 4);
    a = 0x04;
    if (a != 0) { mff_cycles(0xB996, 1); goto L_B99A; }
L_B998:;
    mff_cycles(0xB998, 2);
    a = 0x08;
L_B99A:;
    mff_cycles(0xB99A, 19);
    RAM(OBJ_DIR + x) = a;
    a = 0x01;
    RAM(OBJ_DOWN_TIMER + x) = a;
    a = 0x00;
    RAM(OBJ_AI_COUNT + x) = a;
L_B9A7:;
    mff_cycles(0xB9A7, 30);
    a = 0x03;
    RAM(OBJ_VY + x) = a;
    a = 0x00;
    RAM(OBJ_VY_FRAC + x) = a;
    RAM(OBJ_VX_FRAC + x) = a;
    RAM(OBJ_VX + x) = a;
    a = RAM(OBJ_DOWN_TIMER + x);
    if (a != 0) { mff_cycles(0xB9BA, 2); goto L_BA0A; }
    mff_cycles(0xB9BC, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    if (!(t & 0x80)) { mff_cycles(0xB9BF, 2); goto L_BA0A; }
    mff_cycles(0xB9C1, 8);
    a = RAM(OBJ_AI_COUNT + x);
    m = 0x05;
    c = a >= m;
    if (a != m) { mff_cycles(0xB9C6, 1); goto L_B9F8; }
    mff_cycles(0xB9C8, 10);
    a = 0x68;
    y = 0x0E;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x19CE, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB9CF, 2);
    if (!c) { mff_cycles(0xB9CF, 1); goto L_B9DC; }
    mff_cycles(0xB9D1, 14);
    a = 0x00;
    RAM(OBJ_AI_COUNT + x) = a;
    a = RAM(RNG);
    a &= 0x01;
    if (a != 0) { mff_cycles(0xB9DA, 2); goto L_BA3D; }
L_B9DC:;
    mff_cycles(0xB9DC, 38);
    a = 0x5A;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x00;
    RAM(OBJ_AI_COUNT + x) = a;
    a = 0x84;
    RAM(OBJ_TIMER + x) = a;
    a = 0x87;
    RAM(OBJ_STATE + x) = a;
    a = 0xC2;
    y = 0x33;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x19F6, func_F449, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB9F7, 6);
    SYNC_OUT;
    mff_rts(); return;
L_B9F8:;
    mff_cycles(0xB9F8, 10);
    a = 0xBF;
    y = 0x38;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x19FE, func_F449, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB9FF, 20);
    a = 0x84;
    RAM(OBJ_TIMER + x) = a;
    a = 0x86;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_BA0A:;
    mff_cycles(0xBA0A, 8);
    a = RAM(OBJ_DIR + x);
    m = 0x08;
    c = a >= m;
    if (a != m) { mff_cycles(0xBA0F, 1); goto L_BA27; }
    mff_cycles(0xBA11, 8);
    y = 0x01;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1A15, func_F0F7, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBA16, 7);
    a = RAM(TMP16);
    a &= 0x50;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBA1A, 1); goto L_BA3C; }
    mff_cycles(0xBA1C, 6);
    a = RAM(OBJ_DOWN_TIMER + x);
    if (a != 0) { mff_cycles(0xBA1F, 1); goto L_BA3D; }
    mff_cycles(0xBA21, 13);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_BA27:;
    mff_cycles(0xBA27, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1A2B, func_F0A2, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBA2C, 7);
    a = RAM(TMP16);
    a &= 0x50;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBA30, 1); goto L_BA3C; }
    mff_cycles(0xBA32, 6);
    a = RAM(OBJ_DOWN_TIMER + x);
    if (a != 0) { mff_cycles(0xBA35, 1); goto L_BA3D; }
    mff_cycles(0xBA37, 7);
    a = 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DIR + x) = a;
L_BA3C:;
    mff_cycles(0xBA3C, 6);
    SYNC_OUT;
    mff_rts(); return;
L_BA3D:;
    mff_cycles(0xBA3D, 8);
    a = RAM(OBJ_FLAGS + x);
    a &= 0x40;
    if (a != 0) { mff_cycles(0xBA42, 1); goto L_BA48; }
    mff_cycles(0xBA44, 4);
    a = 0x02;
    if (a != 0) { mff_cycles(0xBA46, 1); goto L_BA4A; }
L_BA48:;
    mff_cycles(0xBA48, 2);
    a = 0x01;
L_BA4A:;
    mff_cycles(0xBA4A, 34);
    RAM(OBJ_DIR + x) = a;
    a = 0x00;
    RAM(OBJ_DOWN_TIMER + x) = a;
    a = 0x24;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x89;
    RAM(OBJ_STATE + x) = a;
    a = 0x12;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1A60, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBA61, 7);
    a = 0xC0;
    y = 0x39;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF449, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F449(); g_code_window_base = swb; } return;
}

/*  $B9B7 (bank 5) - lifted by port/tools/lift.py */
void func_B9B7_b5(void) {
    REGS_IN;
    mff_cycles(0xB9B7, 6);
    a = RAM(OBJ_DOWN_TIMER + x);
    if (a != 0) { mff_cycles(0xB9BA, 2); goto L_BA0A; }
    mff_cycles(0xB9BC, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    if (!(t & 0x80)) { mff_cycles(0xB9BF, 2); goto L_BA0A; }
    mff_cycles(0xB9C1, 8);
    a = RAM(OBJ_AI_COUNT + x);
    m = 0x05;
    c = a >= m;
    if (a != m) { mff_cycles(0xB9C6, 1); goto L_B9F8; }
    mff_cycles(0xB9C8, 10);
    a = 0x68;
    y = 0x0E;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x19CE, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB9CF, 2);
    if (!c) { mff_cycles(0xB9CF, 1); goto L_B9DC; }
    mff_cycles(0xB9D1, 14);
    a = 0x00;
    RAM(OBJ_AI_COUNT + x) = a;
    a = RAM(RNG);
    a &= 0x01;
    if (a != 0) { mff_cycles(0xB9DA, 2); goto L_BA3D; }
L_B9DC:;
    mff_cycles(0xB9DC, 38);
    a = 0x5A;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x00;
    RAM(OBJ_AI_COUNT + x) = a;
    a = 0x84;
    RAM(OBJ_TIMER + x) = a;
    a = 0x87;
    RAM(OBJ_STATE + x) = a;
    a = 0xC2;
    y = 0x33;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x19F6, func_F449, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB9F7, 6);
    SYNC_OUT;
    mff_rts(); return;
L_B9F8:;
    mff_cycles(0xB9F8, 10);
    a = 0xBF;
    y = 0x38;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x19FE, func_F449, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB9FF, 20);
    a = 0x84;
    RAM(OBJ_TIMER + x) = a;
    a = 0x86;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_BA0A:;
    mff_cycles(0xBA0A, 8);
    a = RAM(OBJ_DIR + x);
    m = 0x08;
    c = a >= m;
    if (a != m) { mff_cycles(0xBA0F, 1); goto L_BA27; }
    mff_cycles(0xBA11, 8);
    y = 0x01;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1A15, func_F0F7, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBA16, 7);
    a = RAM(TMP16);
    a &= 0x50;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBA1A, 1); goto L_BA3C; }
    mff_cycles(0xBA1C, 6);
    a = RAM(OBJ_DOWN_TIMER + x);
    if (a != 0) { mff_cycles(0xBA1F, 1); goto L_BA3D; }
    mff_cycles(0xBA21, 13);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_BA27:;
    mff_cycles(0xBA27, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1A2B, func_F0A2, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBA2C, 7);
    a = RAM(TMP16);
    a &= 0x50;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBA30, 1); goto L_BA3C; }
    mff_cycles(0xBA32, 6);
    a = RAM(OBJ_DOWN_TIMER + x);
    if (a != 0) { mff_cycles(0xBA35, 1); goto L_BA3D; }
    mff_cycles(0xBA37, 7);
    a = 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DIR + x) = a;
L_BA3C:;
    mff_cycles(0xBA3C, 6);
    SYNC_OUT;
    mff_rts(); return;
L_BA3D:;
    mff_cycles(0xBA3D, 8);
    a = RAM(OBJ_FLAGS + x);
    a &= 0x40;
    if (a != 0) { mff_cycles(0xBA42, 1); goto L_BA48; }
    mff_cycles(0xBA44, 4);
    a = 0x02;
    if (a != 0) { mff_cycles(0xBA46, 1); goto L_BA4A; }
L_BA48:;
    mff_cycles(0xBA48, 2);
    a = 0x01;
L_BA4A:;
    mff_cycles(0xBA4A, 34);
    RAM(OBJ_DIR + x) = a;
    a = 0x00;
    RAM(OBJ_DOWN_TIMER + x) = a;
    a = 0x24;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x89;
    RAM(OBJ_STATE + x) = a;
    a = 0x12;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1A60, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBA61, 7);
    a = 0xC0;
    y = 0x39;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF449, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F449(); g_code_window_base = swb; } return;
}

/*  $BA68 (bank 5) - lifted by port/tools/lift.py */
void func_BA68_b5(void) {
    REGS_IN;
    mff_cycles(0xBA68, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1A6A, 0xBCC8, 5)) return;
    SYNC_IN;
    mff_cycles(0xBA6B, 8);
    a = RAM(OBJ_X_HI + x);
    m = 0x02;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xBA72, 13);
        a = 0x80;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_STATE + x) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xBA70, 1);
    }
    mff_cycles(0xBA78, 8);
    a = RAM(OBJ_DIR + x);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xBA7D, 1); goto L_BA8A; }
    mff_cycles(0xBA7F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1A81, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBA82, 8);
    a = RAM(OBJ_X + x);
    m = 0x18;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xBA87, 1); goto L_BA94; }
    mff_cycles(0xBA89, 6);
    SYNC_OUT;
    mff_rts(); return;
L_BA8A:;
    mff_cycles(0xBA8A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1A8C, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBA8D, 8);
    a = RAM(OBJ_X + x);
    m = 0xF0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xBA92, 1); goto L_BA99; }
L_BA94:;
    mff_cycles(0xBA94, 7);
    a = 0x84;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
L_BA99:;
    mff_cycles(0xBA99, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BA9A (bank 5) - lifted by port/tools/lift.py */
void func_BA9A_b5(void) {
    REGS_IN;
    mff_cycles(0xBA9A, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    if (a == 0x01) {
        mff_cycles(0xBAA1, 8);
        a = RAM(OBJ_ANIM_TIMER + x);
        m = 0x01;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) {
            mff_cycles(0xBAA8, 6);
            SYNC_OUT;
            if (!mff_call_at(0x1AAA, 0xBBEC, 5)) return;
            SYNC_IN;
            mff_cycles(0xBAAB, 8);
            a = 0x2C;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x1AAF, func_F34C, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xBAA6, 1);
        }
    } else {
        mff_cycles(0xBA9F, 1);
    }
    mff_cycles(0xBAB0, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xBAB7, 8);
        a = RAM(OBJ_ANIM_TIMER + x);
        m = 0x01;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) {
            mff_cycles(0xBABE, 20);
            a = 0x08;
            RAM(OBJ_AI_TIMER + x) = a;
            t = (uint8_t)(RAM(OBJ_AI_COUNT + x) + 1);
            RAM(OBJ_AI_COUNT + x) = t;
            n = (t) >> 7;
            z = (t) == 0;
            SYNC_OUT;
            if (!mff_call_at(0x1AC8, 0xBCD9, 5)) return;
            SYNC_IN;
            mff_cycles(0xBAC9, 9);
            a = RAM(OBJ_TIMER + x);
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_STATE + x) = a;
        } else {
            mff_cycles(0xBABC, 1);
        }
    } else {
        mff_cycles(0xBAB5, 1);
    }
    mff_cycles(0xBACF, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BAD0 (bank 5) - lifted by port/tools/lift.py */
void func_BAD0_b5(void) {
    REGS_IN;
    mff_cycles(0xBAD0, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1AD2, 0xBBE1, 5)) return;
    SYNC_IN;
    mff_cycles(0xBAD3, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (z) {
        mff_cycles(0xBAD8, 13);
        a = 0x08;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_AI_TIMER + x) = a;
        SYNC_OUT;
        if (!mff_call_at(0x1ADF, 0xBCD9, 5)) return;
        SYNC_IN;
        mff_cycles(0xBAE0, 15);
        a = RAM(OBJ_TIMER + x);
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_STATE + x) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xBAD6, 1);
    }
    mff_cycles(0xBAE7, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BB05 (bank 5) - lifted by port/tools/lift.py */
void func_BB05_b5(void) {
    REGS_IN;
    mff_cycles(0xBB05, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    if (t == 0) { mff_cycles(0xBB08, 1); goto L_BB28; }
    mff_cycles(0xBB0A, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x03;
    c = a >= m;
    if (a != m) { mff_cycles(0xBB0F, 1); goto L_BB22; }
    mff_cycles(0xBB11, 6);
    a = RAM(OBJ_ANIM_TIMER + x);
    if (a != 0) { mff_cycles(0xBB14, 1); goto L_BB22; }
    mff_cycles(0xBB16, 16);
    a = 0x85;
    RAM(OBJ_ATTACK + x) = a;
    a = 0x11;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    if (!z) { mff_cycles(0xBB20, 1); goto L_BB27; }
L_BB22:;
    mff_cycles(0xBB22, 7);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ATTACK + x) = a;
L_BB27:;
    mff_cycles(0xBB27, 6);
    SYNC_OUT;
    mff_rts(); return;
L_BB28:;
    mff_cycles(0xBB28, 48);
    RAM(OBJ_VX_FRAC + x) = a;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x05;
    RAM(OBJ_VX + x) = a;
    a = 0x80;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0x01;
    RAM(OBJ_VY + x) = a;
    a = 0xA0;
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0xCC;
    y = 0x39;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1B48, func_F449, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBB49, 22);
    a = RAM(OBJ_Y + x);
    RAM(OBJ_GROUND_Y + x) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BB0A (bank 5) - lifted by port/tools/lift.py */
void func_BB0A_b5(void) {
    REGS_IN;
    mff_cycles(0xBB0A, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x03;
    c = a >= m;
    if (a != m) { mff_cycles(0xBB0F, 1); goto L_BB22; }
    mff_cycles(0xBB11, 6);
    a = RAM(OBJ_ANIM_TIMER + x);
    if (a != 0) { mff_cycles(0xBB14, 1); goto L_BB22; }
    mff_cycles(0xBB16, 16);
    a = 0x85;
    RAM(OBJ_ATTACK + x) = a;
    a = 0x11;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    if (!z) { mff_cycles(0xBB20, 1); goto L_BB27; }
L_BB22:;
    mff_cycles(0xBB22, 7);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ATTACK + x) = a;
L_BB27:;
    mff_cycles(0xBB27, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BB53 (bank 5) - lifted by port/tools/lift.py */
void func_BB53_b5(void) {
    REGS_IN;
    mff_cycles(0xBB53, 20);
    a = 0x90;
    RAM(OBJ_ATTACK + x) = a;
    a = 0x34;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x1B5F, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBB60, 10);
    a = RAM(OBJ_Y + x);
    m = RAM(OBJ_GROUND_Y + x);
    c = a >= m;
    if (c) {
        mff_cycles(0xBB68, 15);
        a = RAM(OBJ_GROUND_Y + x);
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_Y + x) = a;
        SYNC_OUT;
        if (!mff_call_at(0x1B70, 0xBCD9, 5)) return;
        SYNC_IN;
        mff_cycles(0xBB71, 25);
        a = 0x00;
        RAM(OBJ_ATTACK + x) = a;
        RAM(OBJ_ATTACK_DAMAGE + x) = a;
        a = 0x85;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_STATE + x) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xBB66, 1);
    }
    mff_cycles(0xBB7F, 8);
    a = RAM(OBJ_DIR + x);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xBB86, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF1A1, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F1A1(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xBB84, 1);
    }
    mff_cycles(0xBB89, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF183, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F183(); g_code_window_base = swb; } return;
}

/*  $BB8C (bank 5) - lifted by port/tools/lift.py */
void func_BB8C_b5(void) {
    REGS_IN;
    mff_cycles(0xBB8C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1B8E, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBB8F, 13);
    t = (uint8_t)(RAM(OBJ_HIT_TIMER + x) - 1);
    RAM(OBJ_HIT_TIMER + x) = t;
    a = RAM(OBJ_HIT_TIMER + x);
    if (a == 0) {
        mff_cycles(0xBB97, 7);
        a = 0x80;
        RAM(OBJ_STATE + x) = a;
    } else {
        mff_cycles(0xBB95, 1);
    }
    mff_cycles(0xBB9C, 4);
    m = 0x1C;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0xBBA0, 4);
        m = 0x10;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0xBBA4, 10);
            a = 0xC3;
            y = 0x3A;
            n = (y) >> 7;
            z = (y) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x1BAA, func_F449, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xBBA2, 1);
        }
    } else {
        mff_cycles(0xBB9E, 1);
    }
    mff_cycles(0xBBAB, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BBE1 (bank 5) - lifted by port/tools/lift.py */
void func_BBE1_b5(void) {
    REGS_IN;
    mff_cycles(0xBBE1, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1BE3, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBBE4, 2);
    if (!c) {
        mff_cycles(0xBBE6, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF099, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F099(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xBBE4, 1);
    }
    mff_cycles(0xBBE9, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF090, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F090(); g_code_window_base = swb; } return;
}

/*  $BBEC (bank 5) - lifted by port/tools/lift.py */
void func_BBEC_b5(void) {
    REGS_IN;
    mff_cycles(0xBBEC, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1BEE, func_F71A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBBEF, 2);
    if (c) { mff_cycles(0xBBEF, 2); goto L_BC6A; }
    mff_cycles(0xBBF1, 11);
    RAM(TMP0) = y;
    a = 0xC7;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1BF7, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBBF8, 8);
    a = RAM(OBJ_FLAGS + x);
    a &= 0x40;
    if (a != 0) { mff_cycles(0xBBFD, 2); goto L_BC08; }
    mff_cycles(0xBBFF, 11);
    a = 0x90;
    RAM(OBJ_FLAGS + y) = a;
    a = 0x02;
    if (a != 0) { mff_cycles(0xBC06, 1); goto L_BC0F; }
L_BC08:;
    mff_cycles(0xBC08, 9);
    a = 0xD0;
    RAM(OBJ_FLAGS + y) = a;
    a = 0x01;
L_BC0F:;
    mff_cycles(0xBC0F, 19);
    RAM(OBJ_DIR + y) = a;
    a &= 0x02;
    y = a;
    a = RAM(OBJ_X + x);
    c = 0;
    m = prg_read((uint16_t)(0xBD08 + y));
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xBC1C, 11);
    mff_push(a);
    a = RAM(OBJ_X_HI + x);
    m = prg_read((uint16_t)(0xBD09 + y));
    w = a + m + c;
    a = (uint8_t)w;
    mff_cycles(0xBC23, 108);
    y = RAM(TMP0);
    RAM(OBJ_X_HI + y) = a;
    a = mff_pull();
    RAM(OBJ_X + y) = a;
    a = RAM(OBJ_Y + x);
    c = 1;
    m = 0x03;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_Y + y) = a;
    a = RAM(OBJ_Y_HI + x);
    RAM(OBJ_Y_HI + y) = a;
    a = 0x9D;
    RAM(OBJ_ID + y) = a;
    RAM(OBJ_SAVED_ID + y) = a;
    a = 0x03;
    RAM(OBJ_VX + y) = a;
    a = 0x80;
    RAM(OBJ_VX_FRAC + y) = a;
    a = 0x00;
    RAM(OBJ_LIFE + y) = a;
    RAM(OBJ_LIFE_MAX + y) = a;
    RAM(OBJ_VY + y) = a;
    RAM(OBJ_VY_FRAC + y) = a;
    a = 0x0C;
    RAM(OBJ_KIND + y) = a;
    a = 0x87;
    RAM(OBJ_ATTACK + y) = a;
    a = 0x34;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ATTACK_DAMAGE + y) = a;
L_BC6A:;
    mff_cycles(0xBC6A, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BC6B (bank 5) - lifted by port/tools/lift.py */
void func_BC6B_b5(void) {
    REGS_IN;
    mff_cycles(0xBC6B, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBC70, 1); goto L_BC90; }
    mff_cycles(0xBC72, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1C74, func_F1DF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBC75, 8);
    a = RAM(OBJ_Y + x);
    m = 0x82;
    c = a >= m;
    if (c) { mff_cycles(0xBC7A, 1); goto L_BC90; }
    mff_cycles(0xBC7C, 13);
    a = 0x82;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1C83, 0xBCC8, 5)) return;
    SYNC_IN;
    mff_cycles(0xBC84, 6);
    a = RAM(OBJ_AI_FLAG + x);
    if (a != 0) { mff_cycles(0xBC87, 1); goto L_BCAE; }
L_BC89:;
    mff_cycles(0xBC89, 9);
    a = 0x02;
    RAM(OBJ_DIR + x) = a;
    if (a != 0) { mff_cycles(0xBC8E, 1); goto L_BCB3; }
L_BC90:;
    mff_cycles(0xBC90, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBC95, 1); goto L_BCB3; }
    mff_cycles(0xBC97, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1C99, func_F1BF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBC9A, 8);
    a = RAM(OBJ_Y + x);
    m = 0xAE;
    c = a >= m;
    if (!c) { mff_cycles(0xBC9F, 1); goto L_BCB3; }
    mff_cycles(0xBCA1, 13);
    a = 0xAE;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1CA8, 0xBCC8, 5)) return;
    SYNC_IN;
    mff_cycles(0xBCA9, 6);
    a = RAM(OBJ_AI_FLAG + x);
    if (a != 0) { mff_cycles(0xBC89, 1); goto L_BC89; }
L_BCAE:;
    mff_cycles(0xBCAE, 7);
    a = 0x01;
    RAM(OBJ_DIR + x) = a;
L_BCB3:;
    mff_cycles(0xBCB3, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0xBCBA, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1CBC, func_F183, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xBCB8, 1);
    }
    mff_cycles(0xBCBD, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0xBCC4, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1CC6, func_F1A1, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xBCC2, 1);
    }
    mff_cycles(0xBCC7, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BCC8 (bank 5) - lifted by port/tools/lift.py */
void func_BCC8_b5(void) {
    REGS_IN;
    mff_cycles(0xBCC8, 30);
    a = 0x03;
    RAM(OBJ_VX + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_VX_FRAC + x) = a;
    RAM(OBJ_VY_FRAC + x) = a;
    RAM(OBJ_VY + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BCD9 (bank 5) - lifted by port/tools/lift.py */
void func_BCD9_b5(void) {
    REGS_IN;
    mff_cycles(0xBCD9, 8);
    a = 0xBD;
    m = RAM(OBJ_ANIM + x);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xBCE0, 8);
        y = 0x38;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x1CE4, func_F449, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xBCDE, 1);
    }
    mff_cycles(0xBCE5, 6);
    SYNC_OUT;
    mff_rts(); return;
}

