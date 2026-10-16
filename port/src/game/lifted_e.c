/*
 * lifted_e.c - fixed-bank routines $E000-$EFFF, lifted by
 * port/tools/lift_bank.py from the pure scaffold: exact, still to be
 * tidied and moved into their systems' files.
 */
#include "mff.h"
#include "state.h"
#include "coroutine.h"

void func_E16B(void);
void func_E3A3(void);
void func_E473(void);
void func_E4DF(void);
void func_E515(void);
void func_E53F(void);
void func_E60F(void);
void func_E653(void);
void func_E66E(void);
void func_E689(void);
void func_E6DD(void);
void func_E7F5(void);
void func_E853(void);
void func_E856(void);
void func_E89B(void);
void func_E8BD(void);
void func_E9FE(void);
void func_EA6F(void);
void func_EC33(void);
void func_EC81(void);
void func_EEC4(void);
void func_EEDF(void);
void func_F26E(void);
void func_F34A(void);
void func_F436(void);
void func_FF31(void);
void func_FF4D(void);

/*  $E03B - lifted by port/tools/lift.py */
void func_E03B(void) {
    REGS_IN;
    mff_cycles(0xE03B, 31);
    mff_push(a);
    a &= 0xE0;
    RAM(AREA_FLAGS) = a;
    a = mff_pull();
    a &= 0x1F;
    RAM(AREA_SCREEN_COUNT) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(AREA_PALETTE) = a;
    RAM(AREA_SCREEN) = a;
    SYNC_OUT;
    if (!mff_call_at(0x004D, 0xC3DD, -1)) return;
    SYNC_IN;
    mff_cycles(0xE04E, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PALETTE_REQUEST) = a;
    SYNC_OUT;
    mff_call_fn(0x0054, func_FF31, 0xE000);
    SYNC_IN;
    mff_cycles(0xE055, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0057, 0xC362, -1)) return;
    SYNC_IN;
    mff_cycles(0xE058, 6);
    SYNC_OUT;
    if (!mff_call_at(0x005A, 0xC334, -1)) return;
    SYNC_IN;
    mff_cycles(0xE05B, 37);
    x = 0x00;
    a = 0xD0;
    RAM(OBJ_FLAGS) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0x00;
    RAM(OBJ_Y_HI) = a;
    RAM(OBJ_ID) = a;
    a = RAM(SCROLL_X_HI);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X_HI) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0076, func_F26E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE077, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0079, 0xC8B4, -1)) return;
    SYNC_IN;
    mff_cycles(0xE07A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x007C, func_E16B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE07D, 16);
    x = 0x08;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0086, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE087, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0089, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xE08A, 6);
    SYNC_OUT;
    if (!mff_call_at(0x008C, 0x8038, -1)) return;
    SYNC_IN;
    mff_cycles(0xE08D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x008F, 0x8080, -1)) return;
    SYNC_IN;
    mff_cycles(0xE090, 7);
    a = RAM(SUB_AREA);
    m = 0x02;
    c = a >= m;
    if (a != m) { mff_cycles(0xE094, 1); goto L_E0A3; }
    mff_cycles(0xE096, 5);
    a = RAM(ROUND);
    if (a != 0) {
        mff_cycles(0xE09A, 8);
        a = 0x05;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x009E, func_F34A, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xE098, 1);
    }
    mff_cycles(0xE09F, 4);
    a = 0xFF;
    if (a != 0) { mff_cycles(0xE0A1, 1); goto L_E0A5; }
L_E0A3:;
    mff_cycles(0xE0A3, 2);
    a = 0x00;
L_E0A5:;
    mff_cycles(0xE0A5, 99);
    RAM(BOSS_AREA) = a;
    a = 0x01;
    RAM(PLAYER_DIR) = a;
    RAM(JUMP_DIR) = a;
    a = 0x00;
    RAM(GAME_ACTION) = a;
    RAM(JUMP_MOVING) = a;
    RAM(COMBO_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(UNK_A5) = a;
    RAM(ACTION_TIMER) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(WEAPON_HELD) = a;
    RAM(WEAPON_AMMO) = a;
    RAM(RESPAWN_TIMER2) = a;
    RAM(RESPAWN_TIMER1) = a;
    RAM(CAMERA_LOCK) = a;
    RAM(AREA_CLEAR) = a;
    RAM(AREA_KILLS) = a;
    RAM(SPECIAL_SCREEN_ID) = a;
    RAM(SCREEN_EVENT) = a;
    RAM(UNK_B6) = a;
    RAM(CAMERA_AUTOSCROLL) = a;
    RAM(KEEP_ON_SCREEN) = a;
    RAM(ATTACK_GROUND_Y) = a;
    RAM(ACTION_BEFORE_FALL) = a;
    RAM(GRAB_HIT_COUNT) = a;
    RAM(UNK_B2) = a;
    RAM(SPAWN_GROUPS) = a;
    RAM(SPAWN_GROUPS + 1) = a;
    RAM(SPAWN_GROUPS + 2) = a;
    a = RAM(ROUND);
    if (a != 0) { mff_cycles(0xE0E8, 2); goto L_E10A; }
    mff_cycles(0xE0EA, 7);
    a = RAM(SUB_AREA);
    a &= 0x01;
    if (a == 0) { mff_cycles(0xE0EE, 1); goto L_E0FE; }
    mff_cycles(0xE0F0, 17);
    a = 0x1B;
    RAM(GAME_ACTION) = a;
    a = RAM(OBJ_FLAGS);
    a |= 0x20;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS) = a;
    if (!z) { mff_cycles(0xE0FC, 2); goto L_E12C; }
L_E0FE:;
    mff_cycles(0xE0FE, 7);
    a = RAM(SUB_AREA);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xE102, 1); goto L_E12C; }
    mff_cycles(0xE104, 7);
    a = 0x1C;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    if (!z) { mff_cycles(0xE108, 1); goto L_E12C; }
L_E10A:;
    mff_cycles(0xE10A, 7);
    a = RAM(ROUND);
    if (a != 0x01) { mff_cycles(0xE10E, 1); goto L_E11C; }
    mff_cycles(0xE110, 7);
    a = RAM(SUB_AREA);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xE114, 1); goto L_E12C; }
    mff_cycles(0xE116, 7);
    a = 0x20;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    if (!z) { mff_cycles(0xE11A, 1); goto L_E12C; }
L_E11C:;
    mff_cycles(0xE11C, 7);
    a = RAM(ROUND);
    m = 0x04;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xE122, 7);
        a = RAM(SUB_AREA);
        m = 0x02;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) {
            mff_cycles(0xE128, 5);
            a = 0x20;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(GAME_ACTION) = a;
        } else {
            mff_cycles(0xE126, 1);
        }
    } else {
        mff_cycles(0xE120, 1);
    }
L_E12C:;
    mff_cycles(0xE12C, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC8ED, 2); call_by_address_tail(0xC8ED, -1); return;
}

/*  $E12F - lifted by port/tools/lift.py */
void func_E12F(void) {
    REGS_IN;
    mff_cycles(0xE12F, 18);
    a = 0xFF;
    RAM(PAUSED) = a;
    g_cpu.I = 0;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(IRQ_ENABLE) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $E139 - lifted by port/tools/lift.py */
void func_E139(void) {
    REGS_IN;
    mff_cycles(0xE139, 18);
    g_cpu.I = 0;
    a = 0x01;
    RAM(IRQ_ENABLE) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PAUSED) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $E143 - lifted by port/tools/lift.py */
void func_E143(void) {
    REGS_IN;
    mff_cycles(0xE143, 40);
    a = 0x00;
    RAM(SCROLL_X) = a;
    RAM(SCROLL_Y) = a;
    RAM(SCROLL_NT) = a;
    RAM(SCROLL_Y_HI) = a;
    RAM(SCROLL_X_HI) = a;
    RAM(RENDER_OFF_DEPTH) = a;
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PLAYER_DIR) = a;
    RAM(SCROLL_MODE) = a;
    RAM(JUMP_DIR) = a;
    RAM(CAMERA_DIR) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $E16B - lifted by port/tools/lift.py */
void func_E16B(void) {
    REGS_IN;
    mff_cycles(0xE16B, 7);
    y = RAM(ROUND);
    a = prg_read((uint16_t)(0xE18F + y));
    mff_cycles(0xE170, 7);
    RAM(TMP4) = a;
    a = prg_read((uint16_t)(0xE194 + y));
    mff_cycles(0xE175, 7);
    RAM(TMP5) = a;
    a = prg_read((uint16_t)(0xE199 + y));
    mff_cycles(0xE17A, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0xE19E + y));
    mff_cycles(0xE17F, 13);
    RAM(TMP7) = a;
    a = RAM(SUB_AREA);
    y = a;
    a = mem_read((uint16_t)(RAM16(TMP4) + y));
    mff_cycles(0xE186, 9);
    RAM(OBJ_Y) = a;
    a = mem_read((uint16_t)(RAM16(TMP6) + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xE18B, 10);
    RAM(OBJ_X) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $E2A3 - lifted by port/tools/lift.py */
void func_E2A3(void) {
    REGS_IN;
    mff_cycles(0xE2A3, 7);
    a = RAM(GAME_ACTION);
    m = 0x06;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE2A7, 1); goto L_E2BF; }
    mff_cycles(0xE2A9, 4);
    m = 0x1B;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE2AB, 1); goto L_E2BF; }
    mff_cycles(0xE2AD, 4);
    m = 0x1F;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE2AF, 1); goto L_E2BF; }
    mff_cycles(0xE2B1, 4);
    m = 0x0B;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE2B3, 1); goto L_E2BF; }
    mff_cycles(0xE2B5, 4);
    m = 0x09;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE2B7, 1); goto L_E2BF; }
    mff_cycles(0xE2B9, 4);
    m = 0x0D;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE2BB, 1); goto L_E2BF; }
    mff_cycles(0xE2BD, 2);
    if (!z) { mff_cycles(0xE2BD, 1); goto L_E2C0; }
L_E2BF:;
    mff_cycles(0xE2BF, 6);
    SYNC_OUT;
    mff_rts(); return;
L_E2C0:;
    mff_cycles(0xE2C0, 5);
    a = RAM(CAMERA_LOCK);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0xE2C4, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xE473, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_E473(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xE2C2, 1);
    }
    mff_cycles(0xE2C7, 7);
    a = RAM(AREA_FLAGS);
    a &= 0x20;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0xE2CD, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xE473, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_E473(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xE2CB, 1);
    }
    mff_cycles(0xE2D0, 26);
    a = 0x01;
    RAM(TMP16) = a;
    RAM(CAMERA_DIR) = a;
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    a = (uint8_t)w;
    RAM(TMP0) = a;
    c = 1;
    m = 0x80;
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xE2E3, 4);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xE2E1, 1);
    }
    mff_cycles(0xE2E7, 17);
    RAM(TMP1) = a;
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(PLAYER_X_PREV);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(TMP2) = a;
    if (!n) {
        mff_cycles(0xE2F3, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xE3A3, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_E3A3(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xE2F1, 1);
    }
    mff_cycles(0xE2F6, 16);
    a ^= 0xFF;
    c = 0;
    m = 0x01;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(TMP2) = a;
    a = RAM(TMP1);
    if (a >= 0x19) {
        mff_cycles(0xE303, 6);
        a = RAM(TMP2);
        RAM(TMP1) = a;
    } else {
        mff_cycles(0xE301, 1);
    }
    mff_cycles(0xE307, 7);
    a = 0x18;
    if (a < RAM(TMP1)) {
        mff_cycles(0xE30D, 3);
        RAM(TMP1) = a;
    } else {
        mff_cycles(0xE30B, 1);
    }
    mff_cycles(0xE30F, 15);
    a = 0x02;
    RAM(TMP16) = a;
    RAM(CAMERA_DIR) = a;
    a = RAM(TMP0);
    if (a >= 0x80) { mff_cycles(0xE319, 1); goto L_E353; }
    mff_cycles(0xE31B, 7);
    a = RAM(PLAYER_ID);
    if (a != 0x02) { mff_cycles(0xE31F, 1); goto L_E32F; }
    mff_cycles(0xE321, 7);
    a = RAM(GAME_ACTION);
    if (a != 0x04) { mff_cycles(0xE325, 1); goto L_E32F; }
    mff_cycles(0xE327, 7);
    a = RAM(PLAYER_DIR);
    m = 0x02;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE32B, 1); goto L_E333; }
    mff_cycles(0xE32D, 2);
    if (!z) {
        mff_cycles(0xE32D, 1);
    }
L_E32F:;
    mff_cycles(0xE32F, 4);
    a = 0x18;
    if (a != 0) { mff_cycles(0xE331, 1); goto L_E335; }
L_E333:;
    mff_cycles(0xE333, 2);
    a = 0x24;
L_E335:;
    mff_cycles(0xE335, 20);
    RAM(TMP15) = a;
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(TMP0) = a;
    m = RAM(TMP15);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0xE343, 21);
        a = RAM(SCROLL_X);
        c = 0;
        m = RAM(TMP15);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(OBJ_X) = a;
        a = RAM(SCROLL_X_HI);
        m = 0x00;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_X_HI) = a;
    } else {
        mff_cycles(0xE341, 1);
    }
    mff_cycles(0xE352, 6);
    SYNC_OUT;
    mff_rts(); return;
L_E353:;
    mff_cycles(0xE353, 7);
    a = RAM(GAME_ACTION);
    m = 0x1D;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xE359, 5);
        a = RAM(ROUND);
        if (a == 0) {
            mff_cycles(0xE35D, 7);
            a = RAM(SUB_AREA);
            if (a == 0x01) {
                mff_cycles(0xE363, 7);
                a = RAM(AREA_SCREEN);
                m = 0x02;
                c = a >= m;
                if (a == m) {
                    mff_cycles(0xE369, 17);
                    a = 0x1E;
                    RAM(GAME_ACTION) = a;
                    a = 0x80;
                    n = (a) >> 7;
                    z = (a) == 0;
                    RAM(OBJ_STATE) = a;
                    SYNC_OUT;
                    mff_rts(); return;
                } else {
                    mff_cycles(0xE367, 1);
                }
            } else {
                mff_cycles(0xE361, 1);
            }
        } else {
            mff_cycles(0xE35B, 1);
        }
        mff_cycles(0xE373, 7);
        a = RAM(ROUND);
        m = 0x03;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) {
            mff_cycles(0xE379, 5);
            a = RAM(SUB_AREA);
            n = (a) >> 7;
            z = (a) == 0;
            if (z) {
                mff_cycles(0xE37D, 7);
                a = RAM(AREA_SCREEN);
                m = 0x02;
                c = a >= m;
                n = ((uint8_t)(a - m)) >> 7;
                z = ((uint8_t)(a - m)) == 0;
                if (c) {
                    mff_cycles(0xE383, 17);
                    a = 0x26;
                    RAM(GAME_ACTION) = a;
                    a = 0x80;
                    n = (a) >> 7;
                    z = (a) == 0;
                    RAM(OBJ_STATE) = a;
                    SYNC_OUT;
                    mff_rts(); return;
                } else {
                    mff_cycles(0xE381, 1);
                }
            } else {
                mff_cycles(0xE37B, 1);
            }
        } else {
            mff_cycles(0xE377, 1);
        }
        mff_cycles(0xE38D, 6);
        SYNC_OUT;
        if (!mff_call_at(0x038F, 0xCEAA, -1)) return;
        SYNC_IN;
        mff_cycles(0xE390, 17);
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(GAME_ACTION) = a;
        RAM(CAMERA_AUTOSCROLL) = a;
        RAM(JUMP_MOVING) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x039A, func_F436, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xE39B, 6);
        a = 0x80;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_STATE) = a;
    } else {
        mff_cycles(0xE357, 1);
    }
    mff_cycles(0xE3A0, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xE473, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_E473(); g_code_window_base = swb; } return;
}

/*  $E3A3 - lifted by port/tools/lift.py */
void func_E3A3(void) {
    REGS_IN;
    goto L_E3A3;
L_E353:;
    mff_cycles(0xE353, 7);
    a = RAM(GAME_ACTION);
    m = 0x1D;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xE359, 5);
        a = RAM(ROUND);
        if (a == 0) {
            mff_cycles(0xE35D, 7);
            a = RAM(SUB_AREA);
            if (a == 0x01) {
                mff_cycles(0xE363, 7);
                a = RAM(AREA_SCREEN);
                m = 0x02;
                c = a >= m;
                if (a == m) {
                    mff_cycles(0xE369, 17);
                    a = 0x1E;
                    RAM(GAME_ACTION) = a;
                    a = 0x80;
                    n = (a) >> 7;
                    z = (a) == 0;
                    RAM(OBJ_STATE) = a;
                    SYNC_OUT;
                    mff_rts(); return;
                } else {
                    mff_cycles(0xE367, 1);
                }
            } else {
                mff_cycles(0xE361, 1);
            }
        } else {
            mff_cycles(0xE35B, 1);
        }
        mff_cycles(0xE373, 7);
        a = RAM(ROUND);
        m = 0x03;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) {
            mff_cycles(0xE379, 5);
            a = RAM(SUB_AREA);
            n = (a) >> 7;
            z = (a) == 0;
            if (z) {
                mff_cycles(0xE37D, 7);
                a = RAM(AREA_SCREEN);
                m = 0x02;
                c = a >= m;
                n = ((uint8_t)(a - m)) >> 7;
                z = ((uint8_t)(a - m)) == 0;
                if (c) {
                    mff_cycles(0xE383, 17);
                    a = 0x26;
                    RAM(GAME_ACTION) = a;
                    a = 0x80;
                    n = (a) >> 7;
                    z = (a) == 0;
                    RAM(OBJ_STATE) = a;
                    SYNC_OUT;
                    mff_rts(); return;
                } else {
                    mff_cycles(0xE381, 1);
                }
            } else {
                mff_cycles(0xE37B, 1);
            }
        } else {
            mff_cycles(0xE377, 1);
        }
        mff_cycles(0xE38D, 6);
        SYNC_OUT;
        if (!mff_call_at(0x038F, 0xCEAA, -1)) return;
        SYNC_IN;
        mff_cycles(0xE390, 17);
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(GAME_ACTION) = a;
        RAM(CAMERA_AUTOSCROLL) = a;
        RAM(JUMP_MOVING) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x039A, func_F436, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xE39B, 6);
        a = 0x80;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_STATE) = a;
    } else {
        mff_cycles(0xE357, 1);
    }
    mff_cycles(0xE3A0, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xE473, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_E473(); g_code_window_base = swb; } return;
L_E3A3:;
    mff_cycles(0xE3A3, 7);
    a = RAM(TMP0);
    if (a < 0x81) { mff_cycles(0xE353, 1); goto L_E353; }
    mff_cycles(0xE3A9, 8);
    a = RAM(AREA_SCREEN);
    if (a == RAM(AREA_SCREEN_COUNT)) { mff_cycles(0xE353, 1); goto L_E353; }
    mff_cycles(0xE3AF, 5);
    a = RAM(CAMERA_AUTOSCROLL);
    if (a == 0) { mff_cycles(0xE3B1, 2); goto L_E418; }
    mff_cycles(0xE3B3, 7);
    a = RAM(GAME_ACTION);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE3B7, 1); goto L_E3F7; }
    mff_cycles(0xE3B9, 4);
    m = 0x17;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE3BB, 1); goto L_E3F7; }
    mff_cycles(0xE3BD, 4);
    m = 0x18;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE3BF, 1); goto L_E3F7; }
    mff_cycles(0xE3C1, 4);
    m = 0x23;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE3C3, 1); goto L_E3F7; }
    mff_cycles(0xE3C5, 4);
    m = 0x11;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE3C7, 1); goto L_E3F7; }
    mff_cycles(0xE3C9, 4);
    m = 0x25;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE3CB, 1); goto L_E3F7; }
    mff_cycles(0xE3CD, 4);
    m = 0x24;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE3CF, 1); goto L_E3F7; }
    mff_cycles(0xE3D1, 4);
    m = 0x15;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE3D3, 1); goto L_E3F7; }
    mff_cycles(0xE3D5, 4);
    m = 0x0F;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE3D7, 1); goto L_E3F7; }
    mff_cycles(0xE3D9, 4);
    m = 0x22;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE3DB, 1); goto L_E3F7; }
    mff_cycles(0xE3DD, 4);
    m = 0x24;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE3DF, 1); goto L_E3F7; }
    mff_cycles(0xE3E1, 4);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE3E3, 1); goto L_E3F7; }
    mff_cycles(0xE3E5, 4);
    m = 0x14;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE3E7, 1); goto L_E3F7; }
    mff_cycles(0xE3E9, 4);
    if (a == 0x1D) { mff_cycles(0xE3EB, 2); goto L_E409; }
    mff_cycles(0xE3ED, 7);
    a = RAM(TMP0);
    m = 0x81;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xE3F1, 1); goto L_E3F8; }
    mff_cycles(0xE3F3, 5);
    a = 0x1D;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
L_E3F7:;
    mff_cycles(0xE3F7, 6);
    SYNC_OUT;
    mff_rts(); return;
L_E3F8:;
    mff_cycles(0xE3F8, 6);
    SYNC_OUT;
    if (!mff_call_at(0x03FA, 0xCEAA, -1)) return;
    SYNC_IN;
    mff_cycles(0xE3FB, 23);
    a = 0x00;
    RAM(GAME_ACTION) = a;
    RAM(CAMERA_AUTOSCROLL) = a;
    RAM(JUMP_MOVING) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE) = a;
    SYNC_OUT;
    mff_rts(); return;
L_E409:;
    mff_cycles(0xE409, 12);
    a = RAM(SCROLL_X);
    c = 0;
    m = 0x01;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SCROLL_X) = a;
    if (c) {
        mff_cycles(0xE412, 11);
        t = (uint8_t)(RAM(AREA_SCREEN) + 1);
        RAM(AREA_SCREEN) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xE410, 1);
    }
    mff_cycles(0xE415, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xE4DF, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_E4DF(); g_code_window_base = swb; } return;
L_E418:;
    mff_cycles(0xE418, 7);
    a = RAM(TMP1);
    if (a >= 0x19) {
        mff_cycles(0xE41E, 8);
        a = RAM(TMP2);
        RAM(TMP1) = a;
        if (a == 0) {
            mff_cycles(0xE424, 7);
            a = RAM(GAME_ACTION);
            if (a == 0x14) {
                mff_cycles(0xE42A, 5);
                a = 0x01;
                RAM(TMP1) = a;
            } else {
                mff_cycles(0xE428, 1);
            }
        } else {
            mff_cycles(0xE422, 1);
        }
    } else {
        mff_cycles(0xE41C, 1);
    }
    mff_cycles(0xE42E, 7);
    a = 0x18;
    if (a < RAM(TMP1)) {
        mff_cycles(0xE434, 3);
        RAM(TMP1) = a;
    } else {
        mff_cycles(0xE432, 1);
    }
    mff_cycles(0xE436, 7);
    a = RAM(TMP1);
    m = 0x04;
    c = a >= m;
    if (c) {
        mff_cycles(0xE43C, 5);
        a = 0x01;
        RAM(TMP1) = a;
    } else {
        mff_cycles(0xE43A, 1);
    }
    mff_cycles(0xE440, 5);
    a = RAM(TMP1);
    if (a != 0) {
        mff_cycles(0xE444, 13);
        a = RAM(SCROLL_X);
        c = 0;
        m = RAM(TMP1);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(SCROLL_X) = a;
        if (c) {
            mff_cycles(0xE44D, 13);
            t = (uint8_t)(RAM(AREA_SCREEN) + 1);
            RAM(AREA_SCREEN) = t;
            a = RAM(AREA_SCREEN);
            m = RAM(AREA_SCREEN_COUNT);
            c = a >= m;
            if (a == m) {
                mff_cycles(0xE455, 13);
                a = 0x00;
                RAM(SCROLL_X) = a;
                a = 0xF0;
                m = RAM(OBJ_X);
                c = a >= m;
                if (!c) {
                    mff_cycles(0xE460, 4);
                    RAM(OBJ_X) = a;
                } else {
                    mff_cycles(0xE45E, 1);
                }
            } else {
                mff_cycles(0xE453, 1);
            }
            mff_cycles(0xE463, 18);
            a = 0xFF;
            RAM(CAMERA_LOCK) = a;
            RAM(CAMERA_AUTOSCROLL) = a;
            a = 0x00;
            RAM(SCROLL_X) = a;
            t = (uint8_t)(RAM(SCROLL_X_HI) + 1);
            RAM(SCROLL_X_HI) = t;
            n = (t) >> 7;
            z = (t) == 0;
        } else {
            mff_cycles(0xE44B, 1);
        }
        mff_cycles(0xE46F, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xE4DF, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_E4DF(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xE442, 1);
    }
    mff_cycles(0xE473, 5);
    a = RAM(SCROLL_X);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xE475, 1); goto L_E495; }
    mff_cycles(0xE477, 7);
    a = RAM(PLAYER_ID);
    if (a != 0x02) { mff_cycles(0xE47B, 1); goto L_E489; }
    mff_cycles(0xE47D, 7);
    a = RAM(GAME_ACTION);
    if (a != 0x04) { mff_cycles(0xE481, 1); goto L_E489; }
    mff_cycles(0xE483, 7);
    a = RAM(PLAYER_DIR);
    if (a == 0x02) { mff_cycles(0xE487, 1); goto L_E496; }
L_E489:;
    mff_cycles(0xE489, 8);
    a = RAM(OBJ_X);
    m = 0x18;
    c = a >= m;
    if (c) { mff_cycles(0xE48E, 1); goto L_E4A3; }
    mff_cycles(0xE490, 6);
    a = 0x18;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X) = a;
L_E495:;
    mff_cycles(0xE495, 6);
    SYNC_OUT;
    mff_rts(); return;
L_E496:;
    mff_cycles(0xE496, 8);
    a = RAM(OBJ_X);
    m = 0x20;
    c = a >= m;
    if (!c) {
        mff_cycles(0xE49D, 12);
        a = 0x20;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_X) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xE49B, 1);
    }
L_E4A3:;
    mff_cycles(0xE4A3, 7);
    a = RAM(PLAYER_ID);
    if (a != 0x02) { mff_cycles(0xE4A7, 1); goto L_E4C8; }
    mff_cycles(0xE4A9, 7);
    a = RAM(GAME_ACTION);
    if (a != 0x04) { mff_cycles(0xE4AD, 1); goto L_E4C8; }
    mff_cycles(0xE4AF, 7);
    a = RAM(PLAYER_DIR);
    if (a == 0x02) { mff_cycles(0xE4B3, 1); goto L_E4C8; }
    mff_cycles(0xE4B5, 8);
    a = RAM(OBJ_X);
    m = 0xE8;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xE495, 1); goto L_E495; }
    mff_cycles(0xE4BC, 7);
    a = RAM(GAME_ACTION);
    m = 0x19;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE4C0, 1); goto L_E4DE; }
    mff_cycles(0xE4C2, 12);
    a = 0xE8;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X) = a;
    SYNC_OUT;
    mff_rts(); return;
L_E4C8:;
    mff_cycles(0xE4C8, 8);
    a = RAM(OBJ_X);
    m = 0xF0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xE495, 1); goto L_E495; }
    mff_cycles(0xE4CF, 7);
    a = RAM(GAME_ACTION);
    m = 0x19;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xE4D5, 4);
        m = 0x0E;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xE4D9, 6);
            a = 0xF0;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_X) = a;
        } else {
            mff_cycles(0xE4D7, 1);
        }
    } else {
        mff_cycles(0xE4D3, 1);
    }
L_E4DE:;
    mff_cycles(0xE4DE, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $E473 - lifted by port/tools/lift.py */
void func_E473(void) {
    REGS_IN;
    mff_cycles(0xE473, 5);
    a = RAM(SCROLL_X);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xE475, 1); goto L_E495; }
    mff_cycles(0xE477, 7);
    a = RAM(PLAYER_ID);
    if (a != 0x02) { mff_cycles(0xE47B, 1); goto L_E489; }
    mff_cycles(0xE47D, 7);
    a = RAM(GAME_ACTION);
    if (a != 0x04) { mff_cycles(0xE481, 1); goto L_E489; }
    mff_cycles(0xE483, 7);
    a = RAM(PLAYER_DIR);
    if (a == 0x02) { mff_cycles(0xE487, 1); goto L_E496; }
L_E489:;
    mff_cycles(0xE489, 8);
    a = RAM(OBJ_X);
    m = 0x18;
    c = a >= m;
    if (c) { mff_cycles(0xE48E, 1); goto L_E4A3; }
    mff_cycles(0xE490, 6);
    a = 0x18;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X) = a;
L_E495:;
    mff_cycles(0xE495, 6);
    SYNC_OUT;
    mff_rts(); return;
L_E496:;
    mff_cycles(0xE496, 8);
    a = RAM(OBJ_X);
    m = 0x20;
    c = a >= m;
    if (!c) {
        mff_cycles(0xE49D, 12);
        a = 0x20;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_X) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xE49B, 1);
    }
L_E4A3:;
    mff_cycles(0xE4A3, 7);
    a = RAM(PLAYER_ID);
    if (a != 0x02) { mff_cycles(0xE4A7, 1); goto L_E4C8; }
    mff_cycles(0xE4A9, 7);
    a = RAM(GAME_ACTION);
    if (a != 0x04) { mff_cycles(0xE4AD, 1); goto L_E4C8; }
    mff_cycles(0xE4AF, 7);
    a = RAM(PLAYER_DIR);
    if (a == 0x02) { mff_cycles(0xE4B3, 1); goto L_E4C8; }
    mff_cycles(0xE4B5, 8);
    a = RAM(OBJ_X);
    m = 0xE8;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xE495, 1); goto L_E495; }
    mff_cycles(0xE4BC, 7);
    a = RAM(GAME_ACTION);
    m = 0x19;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE4C0, 1); goto L_E4DE; }
    mff_cycles(0xE4C2, 12);
    a = 0xE8;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X) = a;
    SYNC_OUT;
    mff_rts(); return;
L_E4C8:;
    mff_cycles(0xE4C8, 8);
    a = RAM(OBJ_X);
    m = 0xF0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xE495, 1); goto L_E495; }
    mff_cycles(0xE4CF, 7);
    a = RAM(GAME_ACTION);
    m = 0x19;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xE4D5, 4);
        m = 0x0E;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xE4D9, 6);
            a = 0xF0;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_X) = a;
        } else {
            mff_cycles(0xE4D7, 1);
        }
    } else {
        mff_cycles(0xE4D3, 1);
    }
L_E4DE:;
    mff_cycles(0xE4DE, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $E4DF - lifted by port/tools/lift.py */
void func_E4DF(void) {
    REGS_IN;
    mff_cycles(0xE4DF, 10);
    a = RAM(SCROLL_X);
    c = 1;
    m = RAM(SCROLL_X_PREV);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0xE4E6, 6);
        a ^= 0xFF;
        c = 0;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
    } else {
        mff_cycles(0xE4E4, 1);
    }
    mff_cycles(0xE4EB, 5);
    RAM(TMP3) = a;
    if (z) { mff_cycles(0xE4ED, 2); goto L_E51F; }
    mff_cycles(0xE4EF, 7);
    a = RAM(SCROLL_MODE);
    a &= 0x0C;
    if (a != 0) {
        mff_cycles(0xE4F5, 14);
        a = RAM(TMP16);
        RAM(SCROLL_MODE) = a;
        a &= 0x01;
        x = a;
        a = prg_read((uint16_t)(0xE60B + x));
        mff_cycles(0xE4FF, 7);
        RAM(SCROLL_X_PREV) = a;
        a = prg_read((uint16_t)(0xE60D + x));
        mff_cycles(0xE504, 3);
        RAM(BG_COLUMN) = a;
    } else {
        mff_cycles(0xE4F3, 2);
    }
    mff_cycles(0xE506, 7);
    a = RAM(TMP16);
    a &= 0x01;
    if (a != 0) {
        mff_cycles(0xE50C, 6);
        a = RAM(SCROLL_X_PREV);
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xE515, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_E515(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xE50A, 1);
    }
    mff_cycles(0xE511, 20);
    a = RAM(SCROLL_X_PREV);
    a ^= 0xFF;
    a &= 0x07;
    c = 0;
    m = RAM(TMP3);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) { mff_cycles(0xE51D, 1); goto L_E521; }
L_E51F:;
    mff_cycles(0xE51F, 8);
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
L_E521:;
    mff_cycles(0xE521, 22);
    a = RAM(TMP16);
    mff_push(a);
    a &= 0x01;
    x = a;
    a = mff_pull();
    m = RAM(SCROLL_MODE);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    RAM(SCROLL_MODE) = a;
    if (!z) {
        mff_cycles(0xE52E, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xE53F, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_E53F(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xE52C, 1);
    }
    mff_cycles(0xE531, 9);
    a = RAM(BG_COLUMN);
    c = 0;
    m = prg_read((uint16_t)(0xE601 + x));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    mff_cycles(0xE537, 9);
    m = 0x20;
    c = a >= m;
    a &= 0x1F;
    RAM(BG_COLUMN) = a;
    if (c) {
        mff_cycles(0xE53F, 9);
        a = RAM(BG_SCREEN);
        c = 0;
        m = prg_read((uint16_t)(0xE601 + x));
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        mff_cycles(0xE545, 3);
        RAM(BG_SCREEN) = a;
    } else {
        mff_cycles(0xE53D, 1);
    }
    mff_cycles(0xE547, 7);
    y = RAM(ROUND_TABLES);
    a = prg_read((uint16_t)(0xC1F8 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xE54C, 9);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0550, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE551, 19);
    a = RAM(BG_COLUMN);
    t = a;
    t >>= 1;
    a = t;
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    RAM(MAP_CELL) = a;
    y = RAM(BG_SCREEN);
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x055B, func_E66E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE55C, 5);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(TMP3) = a;
    for (;;) {
        mff_cycles(0xE560, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0562, func_E60F, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xE563, 45);
        y = RAM(MAP_CELL);
        a = RAM(ATTR_SHADOW + y);
        RAM(TMP17) = a;
        a = RAM(BG_COLUMN);
        a &= 0x03;
        y = a;
        x = RAM(TMP3);
        a = RAM(TILE_BLOCK + y);
        RAM(VRAM_BUFFER + 3 + x) = a;
        a = RAM(TILE_BLOCK + 4 + y);
        RAM(VRAM_BUFFER + 4 + x) = a;
        a = RAM(MAP_CELL);
        if (a < 0x38) {
            mff_cycles(0xE583, 18);
            a = RAM(TILE_BLOCK + 8 + y);
            RAM(VRAM_BUFFER + 5 + x) = a;
            a = RAM(TILE_BLOCK + 12 + y);
            RAM(VRAM_BUFFER + 6 + x) = a;
        } else {
            mff_cycles(0xE581, 1);
        }
        mff_cycles(0xE58F, 7);
        a = RAM(BG_COLUMN);
        a &= 0x01;
        if (a != 0) {
            mff_cycles(0xE595, 7);
            a = RAM(TMP16);
            a &= prg_read((uint16_t)(0xE603 + y));
            mff_cycles(0xE59A, 10);
            RAM(TMP16) = a;
            a = RAM(TMP17);
            a &= prg_read((uint16_t)(0xE607 + y));
            mff_cycles(0xE5A1, 43);
            a |= RAM(TMP16);
            RAM(VRAM_BUFFER + 27 + x) = a;
            y = RAM(MAP_CELL);
            RAM(ATTR_SHADOW + y) = a;
            a = 0x23;
            RAM(VRAM_BUFFER + 24 + x) = a;
            a = y;
            a |= 0xC0;
            c = 0;
            m = 0x08;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(VRAM_BUFFER + 25 + x) = a;
            a = 0x00;
            RAM(VRAM_BUFFER + 26 + x) = a;
        } else {
            mff_cycles(0xE593, 1);
        }
        mff_cycles(0xE5BE, 34);
        t = (uint8_t)(RAM(TMP3) + 1);
        RAM(TMP3) = t;
        t = (uint8_t)(RAM(TMP3) + 1);
        RAM(TMP3) = t;
        t = (uint8_t)(RAM(TMP3) + 1);
        RAM(TMP3) = t;
        t = (uint8_t)(RAM(TMP3) + 1);
        RAM(TMP3) = t;
        a = RAM(MAP_CELL);
        c = 0;
        m = 0x08;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(MAP_CELL) = a;
        m = 0x28;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0xE5D1, 32);
            a = 0x20;
            RAM(VRAM_BUFFER) = a;
            a = RAM(BG_COLUMN);
            c = 0;
            m = 0x80;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            RAM(VRAM_BUFFER + 1) = a;
            a = 0x14;
            RAM(VRAM_BUFFER + 2) = a;
            y = 0x00;
            a = RAM(BG_COLUMN);
            a &= 0x01;
            if (a != 0) {
                mff_cycles(0xE5EB, 2);
                y = 0x14;
            } else {
                mff_cycles(0xE5E9, 1);
            }
            mff_cycles(0xE5ED, 28);
            a = 0xFF;
            RAM(VRAM_BUFFER + 24 + y) = a;
            RAM(VRAM_COLUMN_REQUEST) = a;
            a = 0x00;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(VRAM_BUFFER + 3) = a;
            RAM(VRAM_BUFFER + 23) = a;
            c = 0;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xE5CF, 1);
        }
        mff_cycles(0xE5FE, 3);
    }
}

/*  $E515 - lifted by port/tools/lift.py */
void func_E515(void) {
    REGS_IN;
    mff_cycles(0xE515, 15);
    a &= 0x07;
    c = 0;
    m = RAM(TMP3);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (z) {
        mff_cycles(0xE51F, 8);
        c = 1;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xE51D, 1);
    }
    mff_cycles(0xE521, 22);
    a = RAM(TMP16);
    mff_push(a);
    a &= 0x01;
    x = a;
    a = mff_pull();
    m = RAM(SCROLL_MODE);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    RAM(SCROLL_MODE) = a;
    if (!z) {
        mff_cycles(0xE52E, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xE53F, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_E53F(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xE52C, 1);
    }
    mff_cycles(0xE531, 9);
    a = RAM(BG_COLUMN);
    c = 0;
    m = prg_read((uint16_t)(0xE601 + x));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    mff_cycles(0xE537, 9);
    m = 0x20;
    c = a >= m;
    a &= 0x1F;
    RAM(BG_COLUMN) = a;
    if (c) {
        mff_cycles(0xE53F, 9);
        a = RAM(BG_SCREEN);
        c = 0;
        m = prg_read((uint16_t)(0xE601 + x));
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        mff_cycles(0xE545, 3);
        RAM(BG_SCREEN) = a;
    } else {
        mff_cycles(0xE53D, 1);
    }
    mff_cycles(0xE547, 7);
    y = RAM(ROUND_TABLES);
    a = prg_read((uint16_t)(0xC1F8 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xE54C, 9);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0550, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE551, 19);
    a = RAM(BG_COLUMN);
    t = a;
    t >>= 1;
    a = t;
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    RAM(MAP_CELL) = a;
    y = RAM(BG_SCREEN);
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x055B, func_E66E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE55C, 5);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(TMP3) = a;
    for (;;) {
        mff_cycles(0xE560, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0562, func_E60F, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xE563, 45);
        y = RAM(MAP_CELL);
        a = RAM(ATTR_SHADOW + y);
        RAM(TMP17) = a;
        a = RAM(BG_COLUMN);
        a &= 0x03;
        y = a;
        x = RAM(TMP3);
        a = RAM(TILE_BLOCK + y);
        RAM(VRAM_BUFFER + 3 + x) = a;
        a = RAM(TILE_BLOCK + 4 + y);
        RAM(VRAM_BUFFER + 4 + x) = a;
        a = RAM(MAP_CELL);
        if (a < 0x38) {
            mff_cycles(0xE583, 18);
            a = RAM(TILE_BLOCK + 8 + y);
            RAM(VRAM_BUFFER + 5 + x) = a;
            a = RAM(TILE_BLOCK + 12 + y);
            RAM(VRAM_BUFFER + 6 + x) = a;
        } else {
            mff_cycles(0xE581, 1);
        }
        mff_cycles(0xE58F, 7);
        a = RAM(BG_COLUMN);
        a &= 0x01;
        if (a != 0) {
            mff_cycles(0xE595, 7);
            a = RAM(TMP16);
            a &= prg_read((uint16_t)(0xE603 + y));
            mff_cycles(0xE59A, 10);
            RAM(TMP16) = a;
            a = RAM(TMP17);
            a &= prg_read((uint16_t)(0xE607 + y));
            mff_cycles(0xE5A1, 43);
            a |= RAM(TMP16);
            RAM(VRAM_BUFFER + 27 + x) = a;
            y = RAM(MAP_CELL);
            RAM(ATTR_SHADOW + y) = a;
            a = 0x23;
            RAM(VRAM_BUFFER + 24 + x) = a;
            a = y;
            a |= 0xC0;
            c = 0;
            m = 0x08;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(VRAM_BUFFER + 25 + x) = a;
            a = 0x00;
            RAM(VRAM_BUFFER + 26 + x) = a;
        } else {
            mff_cycles(0xE593, 1);
        }
        mff_cycles(0xE5BE, 34);
        t = (uint8_t)(RAM(TMP3) + 1);
        RAM(TMP3) = t;
        t = (uint8_t)(RAM(TMP3) + 1);
        RAM(TMP3) = t;
        t = (uint8_t)(RAM(TMP3) + 1);
        RAM(TMP3) = t;
        t = (uint8_t)(RAM(TMP3) + 1);
        RAM(TMP3) = t;
        a = RAM(MAP_CELL);
        c = 0;
        m = 0x08;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(MAP_CELL) = a;
        m = 0x28;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0xE5D1, 32);
            a = 0x20;
            RAM(VRAM_BUFFER) = a;
            a = RAM(BG_COLUMN);
            c = 0;
            m = 0x80;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            RAM(VRAM_BUFFER + 1) = a;
            a = 0x14;
            RAM(VRAM_BUFFER + 2) = a;
            y = 0x00;
            a = RAM(BG_COLUMN);
            a &= 0x01;
            if (a != 0) {
                mff_cycles(0xE5EB, 2);
                y = 0x14;
            } else {
                mff_cycles(0xE5E9, 1);
            }
            mff_cycles(0xE5ED, 28);
            a = 0xFF;
            RAM(VRAM_BUFFER + 24 + y) = a;
            RAM(VRAM_COLUMN_REQUEST) = a;
            a = 0x00;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(VRAM_BUFFER + 3) = a;
            RAM(VRAM_BUFFER + 23) = a;
            c = 0;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xE5CF, 1);
        }
        mff_cycles(0xE5FE, 3);
    }
}

/*  $E521 - lifted by port/tools/lift.py */
void func_E521(void) {
    REGS_IN;
    mff_cycles(0xE521, 22);
    a = RAM(TMP16);
    mff_push(a);
    a &= 0x01;
    x = a;
    a = mff_pull();
    m = RAM(SCROLL_MODE);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    RAM(SCROLL_MODE) = a;
    if (!z) {
        mff_cycles(0xE52E, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xE53F, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_E53F(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xE52C, 1);
    }
    mff_cycles(0xE531, 9);
    a = RAM(BG_COLUMN);
    c = 0;
    m = prg_read((uint16_t)(0xE601 + x));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    mff_cycles(0xE537, 9);
    m = 0x20;
    c = a >= m;
    a &= 0x1F;
    RAM(BG_COLUMN) = a;
    if (c) {
        mff_cycles(0xE53F, 9);
        a = RAM(BG_SCREEN);
        c = 0;
        m = prg_read((uint16_t)(0xE601 + x));
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        mff_cycles(0xE545, 3);
        RAM(BG_SCREEN) = a;
    } else {
        mff_cycles(0xE53D, 1);
    }
    mff_cycles(0xE547, 7);
    y = RAM(ROUND_TABLES);
    a = prg_read((uint16_t)(0xC1F8 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xE54C, 9);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0550, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE551, 19);
    a = RAM(BG_COLUMN);
    t = a;
    t >>= 1;
    a = t;
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    RAM(MAP_CELL) = a;
    y = RAM(BG_SCREEN);
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x055B, func_E66E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE55C, 5);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(TMP3) = a;
    for (;;) {
        mff_cycles(0xE560, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0562, func_E60F, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xE563, 45);
        y = RAM(MAP_CELL);
        a = RAM(ATTR_SHADOW + y);
        RAM(TMP17) = a;
        a = RAM(BG_COLUMN);
        a &= 0x03;
        y = a;
        x = RAM(TMP3);
        a = RAM(TILE_BLOCK + y);
        RAM(VRAM_BUFFER + 3 + x) = a;
        a = RAM(TILE_BLOCK + 4 + y);
        RAM(VRAM_BUFFER + 4 + x) = a;
        a = RAM(MAP_CELL);
        if (a < 0x38) {
            mff_cycles(0xE583, 18);
            a = RAM(TILE_BLOCK + 8 + y);
            RAM(VRAM_BUFFER + 5 + x) = a;
            a = RAM(TILE_BLOCK + 12 + y);
            RAM(VRAM_BUFFER + 6 + x) = a;
        } else {
            mff_cycles(0xE581, 1);
        }
        mff_cycles(0xE58F, 7);
        a = RAM(BG_COLUMN);
        a &= 0x01;
        if (a != 0) {
            mff_cycles(0xE595, 7);
            a = RAM(TMP16);
            a &= prg_read((uint16_t)(0xE603 + y));
            mff_cycles(0xE59A, 10);
            RAM(TMP16) = a;
            a = RAM(TMP17);
            a &= prg_read((uint16_t)(0xE607 + y));
            mff_cycles(0xE5A1, 43);
            a |= RAM(TMP16);
            RAM(VRAM_BUFFER + 27 + x) = a;
            y = RAM(MAP_CELL);
            RAM(ATTR_SHADOW + y) = a;
            a = 0x23;
            RAM(VRAM_BUFFER + 24 + x) = a;
            a = y;
            a |= 0xC0;
            c = 0;
            m = 0x08;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(VRAM_BUFFER + 25 + x) = a;
            a = 0x00;
            RAM(VRAM_BUFFER + 26 + x) = a;
        } else {
            mff_cycles(0xE593, 1);
        }
        mff_cycles(0xE5BE, 34);
        t = (uint8_t)(RAM(TMP3) + 1);
        RAM(TMP3) = t;
        t = (uint8_t)(RAM(TMP3) + 1);
        RAM(TMP3) = t;
        t = (uint8_t)(RAM(TMP3) + 1);
        RAM(TMP3) = t;
        t = (uint8_t)(RAM(TMP3) + 1);
        RAM(TMP3) = t;
        a = RAM(MAP_CELL);
        c = 0;
        m = 0x08;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(MAP_CELL) = a;
        m = 0x28;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0xE5D1, 32);
            a = 0x20;
            RAM(VRAM_BUFFER) = a;
            a = RAM(BG_COLUMN);
            c = 0;
            m = 0x80;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            RAM(VRAM_BUFFER + 1) = a;
            a = 0x14;
            RAM(VRAM_BUFFER + 2) = a;
            y = 0x00;
            a = RAM(BG_COLUMN);
            a &= 0x01;
            if (a != 0) {
                mff_cycles(0xE5EB, 2);
                y = 0x14;
            } else {
                mff_cycles(0xE5E9, 1);
            }
            mff_cycles(0xE5ED, 28);
            a = 0xFF;
            RAM(VRAM_BUFFER + 24 + y) = a;
            RAM(VRAM_COLUMN_REQUEST) = a;
            a = 0x00;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(VRAM_BUFFER + 3) = a;
            RAM(VRAM_BUFFER + 23) = a;
            c = 0;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xE5CF, 1);
        }
        mff_cycles(0xE5FE, 3);
    }
}

/*  $E53F - lifted by port/tools/lift.py */
void func_E53F(void) {
    REGS_IN;
    mff_cycles(0xE53F, 9);
    a = RAM(BG_SCREEN);
    c = 0;
    m = prg_read((uint16_t)(0xE601 + x));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xE545, 10);
    RAM(BG_SCREEN) = a;
    y = RAM(ROUND_TABLES);
    a = prg_read((uint16_t)(0xC1F8 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xE54C, 9);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0550, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE551, 19);
    a = RAM(BG_COLUMN);
    t = a;
    t >>= 1;
    a = t;
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    RAM(MAP_CELL) = a;
    y = RAM(BG_SCREEN);
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x055B, func_E66E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE55C, 5);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(TMP3) = a;
    for (;;) {
        mff_cycles(0xE560, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0562, func_E60F, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xE563, 45);
        y = RAM(MAP_CELL);
        a = RAM(ATTR_SHADOW + y);
        RAM(TMP17) = a;
        a = RAM(BG_COLUMN);
        a &= 0x03;
        y = a;
        x = RAM(TMP3);
        a = RAM(TILE_BLOCK + y);
        RAM(VRAM_BUFFER + 3 + x) = a;
        a = RAM(TILE_BLOCK + 4 + y);
        RAM(VRAM_BUFFER + 4 + x) = a;
        a = RAM(MAP_CELL);
        if (a < 0x38) {
            mff_cycles(0xE583, 18);
            a = RAM(TILE_BLOCK + 8 + y);
            RAM(VRAM_BUFFER + 5 + x) = a;
            a = RAM(TILE_BLOCK + 12 + y);
            RAM(VRAM_BUFFER + 6 + x) = a;
        } else {
            mff_cycles(0xE581, 1);
        }
        mff_cycles(0xE58F, 7);
        a = RAM(BG_COLUMN);
        a &= 0x01;
        if (a != 0) {
            mff_cycles(0xE595, 7);
            a = RAM(TMP16);
            a &= prg_read((uint16_t)(0xE603 + y));
            mff_cycles(0xE59A, 10);
            RAM(TMP16) = a;
            a = RAM(TMP17);
            a &= prg_read((uint16_t)(0xE607 + y));
            mff_cycles(0xE5A1, 43);
            a |= RAM(TMP16);
            RAM(VRAM_BUFFER + 27 + x) = a;
            y = RAM(MAP_CELL);
            RAM(ATTR_SHADOW + y) = a;
            a = 0x23;
            RAM(VRAM_BUFFER + 24 + x) = a;
            a = y;
            a |= 0xC0;
            c = 0;
            m = 0x08;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(VRAM_BUFFER + 25 + x) = a;
            a = 0x00;
            RAM(VRAM_BUFFER + 26 + x) = a;
        } else {
            mff_cycles(0xE593, 1);
        }
        mff_cycles(0xE5BE, 34);
        t = (uint8_t)(RAM(TMP3) + 1);
        RAM(TMP3) = t;
        t = (uint8_t)(RAM(TMP3) + 1);
        RAM(TMP3) = t;
        t = (uint8_t)(RAM(TMP3) + 1);
        RAM(TMP3) = t;
        t = (uint8_t)(RAM(TMP3) + 1);
        RAM(TMP3) = t;
        a = RAM(MAP_CELL);
        c = 0;
        m = 0x08;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(MAP_CELL) = a;
        m = 0x28;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0xE5D1, 32);
            a = 0x20;
            RAM(VRAM_BUFFER) = a;
            a = RAM(BG_COLUMN);
            c = 0;
            m = 0x80;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            RAM(VRAM_BUFFER + 1) = a;
            a = 0x14;
            RAM(VRAM_BUFFER + 2) = a;
            y = 0x00;
            a = RAM(BG_COLUMN);
            a &= 0x01;
            if (a != 0) {
                mff_cycles(0xE5EB, 2);
                y = 0x14;
            } else {
                mff_cycles(0xE5E9, 1);
            }
            mff_cycles(0xE5ED, 28);
            a = 0xFF;
            RAM(VRAM_BUFFER + 24 + y) = a;
            RAM(VRAM_COLUMN_REQUEST) = a;
            a = 0x00;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(VRAM_BUFFER + 3) = a;
            RAM(VRAM_BUFFER + 23) = a;
            c = 0;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xE5CF, 1);
        }
        mff_cycles(0xE5FE, 3);
    }
}

/*  $E560 - lifted by port/tools/lift.py */
void func_E560(void) {
    REGS_IN;
    for (;;) {
        mff_cycles(0xE560, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0562, func_E60F, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xE563, 45);
        y = RAM(MAP_CELL);
        a = RAM(ATTR_SHADOW + y);
        RAM(TMP17) = a;
        a = RAM(BG_COLUMN);
        a &= 0x03;
        y = a;
        x = RAM(TMP3);
        a = RAM(TILE_BLOCK + y);
        RAM(VRAM_BUFFER + 3 + x) = a;
        a = RAM(TILE_BLOCK + 4 + y);
        RAM(VRAM_BUFFER + 4 + x) = a;
        a = RAM(MAP_CELL);
        if (a < 0x38) {
            mff_cycles(0xE583, 18);
            a = RAM(TILE_BLOCK + 8 + y);
            RAM(VRAM_BUFFER + 5 + x) = a;
            a = RAM(TILE_BLOCK + 12 + y);
            RAM(VRAM_BUFFER + 6 + x) = a;
        } else {
            mff_cycles(0xE581, 1);
        }
        mff_cycles(0xE58F, 7);
        a = RAM(BG_COLUMN);
        a &= 0x01;
        if (a != 0) {
            mff_cycles(0xE595, 7);
            a = RAM(TMP16);
            a &= prg_read((uint16_t)(0xE603 + y));
            mff_cycles(0xE59A, 10);
            RAM(TMP16) = a;
            a = RAM(TMP17);
            a &= prg_read((uint16_t)(0xE607 + y));
            mff_cycles(0xE5A1, 43);
            a |= RAM(TMP16);
            RAM(VRAM_BUFFER + 27 + x) = a;
            y = RAM(MAP_CELL);
            RAM(ATTR_SHADOW + y) = a;
            a = 0x23;
            RAM(VRAM_BUFFER + 24 + x) = a;
            a = y;
            a |= 0xC0;
            c = 0;
            m = 0x08;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(VRAM_BUFFER + 25 + x) = a;
            a = 0x00;
            RAM(VRAM_BUFFER + 26 + x) = a;
        } else {
            mff_cycles(0xE593, 1);
        }
        mff_cycles(0xE5BE, 34);
        t = (uint8_t)(RAM(TMP3) + 1);
        RAM(TMP3) = t;
        t = (uint8_t)(RAM(TMP3) + 1);
        RAM(TMP3) = t;
        t = (uint8_t)(RAM(TMP3) + 1);
        RAM(TMP3) = t;
        t = (uint8_t)(RAM(TMP3) + 1);
        RAM(TMP3) = t;
        a = RAM(MAP_CELL);
        c = 0;
        m = 0x08;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(MAP_CELL) = a;
        m = 0x28;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0xE5D1, 32);
            a = 0x20;
            RAM(VRAM_BUFFER) = a;
            a = RAM(BG_COLUMN);
            c = 0;
            m = 0x80;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            RAM(VRAM_BUFFER + 1) = a;
            a = 0x14;
            RAM(VRAM_BUFFER + 2) = a;
            y = 0x00;
            a = RAM(BG_COLUMN);
            a &= 0x01;
            if (a != 0) {
                mff_cycles(0xE5EB, 2);
                y = 0x14;
            } else {
                mff_cycles(0xE5E9, 1);
            }
            mff_cycles(0xE5ED, 28);
            a = 0xFF;
            RAM(VRAM_BUFFER + 24 + y) = a;
            RAM(VRAM_COLUMN_REQUEST) = a;
            a = 0x00;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(VRAM_BUFFER + 3) = a;
            RAM(VRAM_BUFFER + 23) = a;
            c = 0;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xE5CF, 1);
        }
        mff_cycles(0xE5FE, 3);
    }
}

/*  $E60F - lifted by port/tools/lift.py */
void func_E60F(void) {
    REGS_IN;
    mff_cycles(0xE60F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0611, func_E653, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE612, 10);
    y = 0x03;
    RAM(TMP2) = y;
    a = 0x00;
    RAM(TMP16) = a;
L_E61A:;
    mff_cycles(0xE61A, 7);
    y = RAM(TMP2);
    x = prg_read((uint16_t)(0xE64F + y));
    mff_cycles(0xE61F, 5);
    a = mem_read((uint16_t)(RAM16(TMP0) + y));
    mff_cycles(0xE621, 6);
    y = a;
    a = prg_read((uint16_t)(0xBB00 + y));
    mff_cycles(0xE625, 9);
    RAM(TILE_BLOCK + x) = a;
    a = prg_read((uint16_t)(0xBC00 + y));
    mff_cycles(0xE62B, 9);
    RAM(TILE_BLOCK + 1 + x) = a;
    a = prg_read((uint16_t)(0xBD00 + y));
    mff_cycles(0xE631, 9);
    RAM(TILE_BLOCK + 4 + x) = a;
    a = prg_read((uint16_t)(0xBE00 + y));
    mff_cycles(0xE637, 9);
    RAM(TILE_BLOCK + 5 + x) = a;
    a = prg_read((uint16_t)(0xBF00 + y));
    mff_cycles(0xE63D, 15);
    a &= 0x03;
    a |= RAM(TMP16);
    RAM(TMP16) = a;
    t = (uint8_t)(RAM(TMP2) - 1);
    RAM(TMP2) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (n) { mff_cycles(0xE645, 1); goto L_E64E; }
    mff_cycles(0xE647, 13);
    t = RAM(TMP16);
    t = (uint8_t)(t << 1);
    RAM(TMP16) = t;
    t = RAM(TMP16);
    c = t >> 7;
    t = (uint8_t)(t << 1);
    RAM(TMP16) = t;
    goto L_E61A;
L_E64E:;
    mff_cycles(0xE64E, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $E61A - lifted by port/tools/lift.py */
void func_E61A(void) {
    REGS_IN;
L_E61A:;
    mff_cycles(0xE61A, 7);
    y = RAM(TMP2);
    x = prg_read((uint16_t)(0xE64F + y));
    mff_cycles(0xE61F, 5);
    a = mem_read((uint16_t)(RAM16(TMP0) + y));
    mff_cycles(0xE621, 6);
    y = a;
    a = prg_read((uint16_t)(0xBB00 + y));
    mff_cycles(0xE625, 9);
    RAM(TILE_BLOCK + x) = a;
    a = prg_read((uint16_t)(0xBC00 + y));
    mff_cycles(0xE62B, 9);
    RAM(TILE_BLOCK + 1 + x) = a;
    a = prg_read((uint16_t)(0xBD00 + y));
    mff_cycles(0xE631, 9);
    RAM(TILE_BLOCK + 4 + x) = a;
    a = prg_read((uint16_t)(0xBE00 + y));
    mff_cycles(0xE637, 9);
    RAM(TILE_BLOCK + 5 + x) = a;
    a = prg_read((uint16_t)(0xBF00 + y));
    mff_cycles(0xE63D, 15);
    a &= 0x03;
    a |= RAM(TMP16);
    RAM(TMP16) = a;
    t = (uint8_t)(RAM(TMP2) - 1);
    RAM(TMP2) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (n) { mff_cycles(0xE645, 1); goto L_E64E; }
    mff_cycles(0xE647, 13);
    t = RAM(TMP16);
    t = (uint8_t)(t << 1);
    RAM(TMP16) = t;
    t = RAM(TMP16);
    c = t >> 7;
    t = (uint8_t)(t << 1);
    RAM(TMP16) = t;
    goto L_E61A;
L_E64E:;
    mff_cycles(0xE64E, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $E653 - lifted by port/tools/lift.py */
void func_E653(void) {
    REGS_IN;
    mff_cycles(0xE653, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0655, 0xC1DF, -1)) return;
    SYNC_IN;
    mff_cycles(0xE656, 13);
    a = 0x00;
    RAM(TMP1) = a;
    y = RAM(MAP_CELL);
    a = mem_read((uint16_t)(RAM16(MAP_SCREEN_PTR) + y));
    mff_cycles(0xE65E, 33);
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    t = RAM(TMP1);
    t = (uint8_t)(t << 1 | c);
    RAM(TMP1) = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    t = RAM(TMP1);
    t = (uint8_t)(t << 1 | c);
    RAM(TMP1) = t;
    RAM(TMP0) = a;
    a = RAM(TMP1);
    c = 0;
    m = 0xB7;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(TMP1) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $E66E - lifted by port/tools/lift.py */
void func_E66E(void) {
    REGS_IN;
    mff_cycles(0xE66E, 5);
    a = RAM(SUB_AREA);
    if (a == 0) { mff_cycles(0xE670, 1); goto L_E67A; }
    mff_cycles(0xE672, 4);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0xE674, 1); goto L_E680; }
    mff_cycles(0xE676, 4);
    m = 0x02;
    c = a >= m;
    if (a == m) { mff_cycles(0xE678, 1); goto L_E686; }
L_E67A:;
    mff_cycles(0xE67A, 4);
    a = prg_read((uint16_t)(0xAE60 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xE67D, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xE689, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_E689(); g_code_window_base = swb; } return;
L_E680:;
    mff_cycles(0xE680, 4);
    a = prg_read((uint16_t)(0xAE90 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xE683, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xE689, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_E689(); g_code_window_base = swb; } return;
L_E686:;
    mff_cycles(0xE686, 4);
    a = prg_read((uint16_t)(0xAEC0 + y));
    mff_cycles(0xE689, 73);
    mff_push(a);
    a = 0x00;
    RAM(TMP0) = a;
    a = mff_pull();
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    t = RAM(TMP0);
    t = (uint8_t)(t << 1 | c);
    RAM(TMP0) = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    t = RAM(TMP0);
    t = (uint8_t)(t << 1 | c);
    RAM(TMP0) = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    t = RAM(TMP0);
    t = (uint8_t)(t << 1 | c);
    RAM(TMP0) = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    t = RAM(TMP0);
    t = (uint8_t)(t << 1 | c);
    RAM(TMP0) = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    t = RAM(TMP0);
    t = (uint8_t)(t << 1 | c);
    RAM(TMP0) = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    t = RAM(TMP0);
    t = (uint8_t)(t << 1 | c);
    RAM(TMP0) = t;
    RAM(MAP_SCREEN_PTR) = a;
    a = RAM(TMP0);
    c = 0;
    m = 0xAF;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(MAP_SCREEN_PTR + 1) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $E689 - lifted by port/tools/lift.py */
void func_E689(void) {
    REGS_IN;
    mff_cycles(0xE689, 73);
    mff_push(a);
    a = 0x00;
    RAM(TMP0) = a;
    a = mff_pull();
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    t = RAM(TMP0);
    t = (uint8_t)(t << 1 | c);
    RAM(TMP0) = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    t = RAM(TMP0);
    t = (uint8_t)(t << 1 | c);
    RAM(TMP0) = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    t = RAM(TMP0);
    t = (uint8_t)(t << 1 | c);
    RAM(TMP0) = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    t = RAM(TMP0);
    t = (uint8_t)(t << 1 | c);
    RAM(TMP0) = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    t = RAM(TMP0);
    t = (uint8_t)(t << 1 | c);
    RAM(TMP0) = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    t = RAM(TMP0);
    t = (uint8_t)(t << 1 | c);
    RAM(TMP0) = t;
    RAM(MAP_SCREEN_PTR) = a;
    a = RAM(TMP0);
    c = 0;
    m = 0xAF;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(MAP_SCREEN_PTR + 1) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $E6AB - lifted by port/tools/lift.py */
void func_E6AB(void) {
    REGS_IN;
    mff_cycles(0xE6AB, 8);
    a = RAM(VRAM_LIST_REQUEST);
    a |= RAM(VRAM_COLUMN_REQUEST);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xE6AF, 1); goto L_E6DC; }
    mff_cycles(0xE6B1, 5);
    a = RAM(SPECIAL_SCREEN_ID);
    if (a == 0) { mff_cycles(0xE6B3, 1); goto L_E6BE; }
    mff_cycles(0xE6B5, 4);
    m = 0xF9;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xE6B7, 1); goto L_E6DC; }
    mff_cycles(0xE6B9, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SPECIAL_SCREEN_ID) = a;
    SYNC_OUT;
    mff_rts(); return;
L_E6BE:;
    mff_cycles(0xE6BE, 7);
    a = RAM(GAME_ACTION);
    m = 0x0E;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xE6C4, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x06C6, func_E6DD, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xE6C7, 7);
        a = RAM(GAME_ACTION);
        m = 0x04;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) {
            mff_cycles(0xE6CD, 11);
            x = RAM(GRAB_SLOT);
            RAM(HUD_ENEMY_SLOT) = x;
            a = 0x00;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(HUD_ENEMY_TIMER) = a;
        } else {
            mff_cycles(0xE6CB, 1);
        }
        mff_cycles(0xE6D5, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x06D7, func_E89B, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xE6D8, 5);
        a = 0xFF;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(VRAM_REQUEST) = a;
    } else {
        mff_cycles(0xE6C2, 1);
    }
L_E6DC:;
    mff_cycles(0xE6DC, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $E6DD - lifted by port/tools/lift.py */
void func_E6DD(void) {
    REGS_IN;
    mff_cycles(0xE6DD, 10);
    x = RAM(VRAM_BUFFER_LEN);
    a = RAM(GAME_ACTION);
    if (a == 0x27) { mff_cycles(0xE6E3, 2); goto L_E742; }
    mff_cycles(0xE6E5, 8);
    y = RAM(PLAYER_LEVEL);
    a = RAM(PLAYER_ID);
    if (a == 0) { mff_cycles(0xE6E9, 1); goto L_E6F9; }
    mff_cycles(0xE6EB, 4);
    if (a == 0x01) { mff_cycles(0xE6ED, 1); goto L_E6F4; }
    mff_cycles(0xE6EF, 4);
    a = prg_read((uint16_t)(0xE875 + y));
    mff_cycles(0xE6F2, 2);
    if (a != 0) { mff_cycles(0xE6F2, 1); goto L_E6FC; }
L_E6F4:;
    mff_cycles(0xE6F4, 4);
    a = prg_read((uint16_t)(0xE86F + y));
    mff_cycles(0xE6F7, 2);
    if (a != 0) { mff_cycles(0xE6F7, 1); goto L_E6FC; }
L_E6F9:;
    mff_cycles(0xE6F9, 4);
    a = prg_read((uint16_t)(0xE869 + y));
L_E6FC:;
    mff_cycles(0xE6FC, 40);
    RAM(VRAM_BUFFER + x) = a;
    t = (uint8_t)(RAM(VRAM_BUFFER + x) + 1);
    RAM(VRAM_BUFFER + x) = t;
    RAM(TMP0) = a;
    x++;
    a = 0x20;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x4A;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = RAM(HEALTH);
    if (a & 0x80) {
        mff_cycles(0xE715, 2);
        a = 0x00;
    } else {
        mff_cycles(0xE713, 1);
    }
    mff_cycles(0xE717, 3);
    RAM(TMP1) = a;
L_E719:;
    mff_cycles(0xE719, 5);
    a = RAM(TMP1);
    z = (a) == 0;
    if (a & 0x80) { mff_cycles(0xE71B, 1); goto L_E729; }
    mff_cycles(0xE71D, 2);
    if (z) { mff_cycles(0xE71D, 1); goto L_E72B; }
    mff_cycles(0xE71F, 4);
    m = 0x07;
    z = ((uint8_t)(a - m)) == 0;
    if (a >= m) { mff_cycles(0xE721, 1); goto L_E725; }
    mff_cycles(0xE723, 2);
    if (!z) { mff_cycles(0xE723, 1); goto L_E72B; }
L_E725:;
    mff_cycles(0xE725, 4);
    a = 0x18;
    if (a != 0) { mff_cycles(0xE727, 1); goto L_E72B; }
L_E729:;
    mff_cycles(0xE729, 2);
    a = 0x10;
L_E72B:;
    mff_cycles(0xE72B, 26);
    a |= 0x10;
    RAM(VRAM_BUFFER + x) = a;
    a = RAM(TMP1);
    c = 1;
    m = 0x08;
    w = a - m - !c;
    a = (uint8_t)w;
    RAM(TMP1) = a;
    x++;
    t = (uint8_t)(RAM(TMP0) - 1);
    RAM(TMP0) = t;
    if (t != 0) { mff_cycles(0xE719, 1); goto L_E719; }
    mff_cycles(0xE73C, 9);
    a = 0x1A;
    RAM(VRAM_BUFFER + x) = a;
    x++;
L_E742:;
    mff_cycles(0xE742, 134);
    a = 0x01;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x20;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x5C;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = RAM(ROUND);
    RAM(VRAM_BUFFER + x) = a;
    t = (uint8_t)(RAM(VRAM_BUFFER + x) + 1);
    RAM(VRAM_BUFFER + x) = t;
    t = (uint8_t)(RAM(VRAM_BUFFER + x) + 1);
    RAM(VRAM_BUFFER + x) = t;
    x++;
    a = 0x01;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x20;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x7C;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = RAM(LIVES);
    a &= 0x0F;
    RAM(VRAM_BUFFER + x) = a;
    t = (uint8_t)(RAM(VRAM_BUFFER + x) + 1);
    RAM(VRAM_BUFFER + x) = t;
    x++;
    a = 0x01;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x23;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x49;
    c = 0;
    m = RAM(PLAYER_LEVEL);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(VRAM_BUFFER + x) = a;
    a = RAM(PLAYER_ID);
    if (a == 0x02) {
        mff_cycles(0xE797, 14);
        t = (uint8_t)(RAM(VRAM_BUFFER + x) + 1);
        RAM(VRAM_BUFFER + x) = t;
        t = (uint8_t)(RAM(VRAM_BUFFER + x) + 1);
        RAM(VRAM_BUFFER + x) = t;
    } else {
        mff_cycles(0xE795, 1);
    }
    mff_cycles(0xE79D, 45);
    x++;
    a = 0x73;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x03;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x23;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x56;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = RAM(PLAYER_ID);
    if (a == 0x02) { mff_cycles(0xE7BA, 1); goto L_E7C4; }
    mff_cycles(0xE7BC, 7);
    a = RAM(PLAYER_LEVEL);
    m = 0x05;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE7C0, 1); goto L_E7F2; }
    mff_cycles(0xE7C2, 2);
    if (!z) { mff_cycles(0xE7C2, 1); goto L_E7CA; }
L_E7C4:;
    mff_cycles(0xE7C4, 7);
    a = RAM(PLAYER_LEVEL);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE7C8, 1); goto L_E7F2; }
L_E7CA:;
    mff_cycles(0xE7CA, 68);
    a = RAM(PLAYER_EXP + 1);
    a &= 0x0F;
    RAM(VRAM_BUFFER + x) = a;
    t = (uint8_t)(RAM(VRAM_BUFFER + x) + 1);
    RAM(VRAM_BUFFER + x) = t;
    x++;
    a = RAM(PLAYER_EXP);
    a &= 0xF0;
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
    c = t & 1;
    t >>= 1;
    a = t;
    RAM(VRAM_BUFFER + x) = a;
    t = (uint8_t)(RAM(VRAM_BUFFER + x) + 1);
    RAM(VRAM_BUFFER + x) = t;
    x++;
    a = RAM(PLAYER_EXP);
    a &= 0x0F;
    RAM(VRAM_BUFFER + x) = a;
    t = (uint8_t)(RAM(VRAM_BUFFER + x) + 1);
    RAM(VRAM_BUFFER + x) = t;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xE7F5, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_E7F5(); g_code_window_base = swb; } return;
L_E7F2:;
    mff_cycles(0xE7F2, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x07F4, func_E856, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE7F5, 34);
    a = 0x03;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x23;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x5A;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    y = RAM(PLAYER_ID);
    if (y == 0x02) { mff_cycles(0xE80B, 1); goto L_E815; }
    mff_cycles(0xE80D, 7);
    a = RAM(PLAYER_LEVEL);
    m = 0x05;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE811, 1); goto L_E850; }
    mff_cycles(0xE813, 2);
    if (!z) { mff_cycles(0xE813, 1); goto L_E81B; }
L_E815:;
    mff_cycles(0xE815, 7);
    a = RAM(PLAYER_LEVEL);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE819, 1); goto L_E850; }
L_E81B:;
    mff_cycles(0xE81B, 4);
    a = prg_read((uint16_t)(0xE87B + y));
    mff_cycles(0xE81E, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0xE87E + y));
    mff_cycles(0xE823, 15);
    RAM(TMP1) = a;
    a = RAM(PLAYER_LEVEL);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    y = a;
    a = mem_read((uint16_t)(RAM16(TMP0) + y));
    mff_cycles(0xE82B, 21);
    RAM(VRAM_BUFFER + x) = a;
    t = (uint8_t)(RAM(VRAM_BUFFER + x) + 1);
    RAM(VRAM_BUFFER + x) = t;
    x++;
    y++;
    a = mem_read((uint16_t)(RAM16(TMP0) + y));
    mff_cycles(0xE835, 29);
    a &= 0xF0;
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
    c = t & 1;
    t >>= 1;
    a = t;
    RAM(VRAM_BUFFER + x) = a;
    t = (uint8_t)(RAM(VRAM_BUFFER + x) + 1);
    RAM(VRAM_BUFFER + x) = t;
    x++;
    a = mem_read((uint16_t)(RAM16(TMP0) + y));
    mff_cycles(0xE844, 19);
    a &= 0x0F;
    RAM(VRAM_BUFFER + x) = a;
    t = (uint8_t)(RAM(VRAM_BUFFER + x) + 1);
    RAM(VRAM_BUFFER + x) = t;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xE853, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_E853(); g_code_window_base = swb; } return;
L_E850:;
    mff_cycles(0xE850, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0852, func_E856, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE853, 9);
    RAM(VRAM_BUFFER_LEN) = x;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $E7F5 - lifted by port/tools/lift.py */
void func_E7F5(void) {
    REGS_IN;
    mff_cycles(0xE7F5, 34);
    a = 0x03;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x23;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x5A;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    y = RAM(PLAYER_ID);
    if (y == 0x02) { mff_cycles(0xE80B, 1); goto L_E815; }
    mff_cycles(0xE80D, 7);
    a = RAM(PLAYER_LEVEL);
    m = 0x05;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE811, 1); goto L_E850; }
    mff_cycles(0xE813, 2);
    if (!z) { mff_cycles(0xE813, 1); goto L_E81B; }
L_E815:;
    mff_cycles(0xE815, 7);
    a = RAM(PLAYER_LEVEL);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xE819, 1); goto L_E850; }
L_E81B:;
    mff_cycles(0xE81B, 4);
    a = prg_read((uint16_t)(0xE87B + y));
    mff_cycles(0xE81E, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0xE87E + y));
    mff_cycles(0xE823, 15);
    RAM(TMP1) = a;
    a = RAM(PLAYER_LEVEL);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    y = a;
    a = mem_read((uint16_t)(RAM16(TMP0) + y));
    mff_cycles(0xE82B, 21);
    RAM(VRAM_BUFFER + x) = a;
    t = (uint8_t)(RAM(VRAM_BUFFER + x) + 1);
    RAM(VRAM_BUFFER + x) = t;
    x++;
    y++;
    a = mem_read((uint16_t)(RAM16(TMP0) + y));
    mff_cycles(0xE835, 29);
    a &= 0xF0;
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
    c = t & 1;
    t >>= 1;
    a = t;
    RAM(VRAM_BUFFER + x) = a;
    t = (uint8_t)(RAM(VRAM_BUFFER + x) + 1);
    RAM(VRAM_BUFFER + x) = t;
    x++;
    a = mem_read((uint16_t)(RAM16(TMP0) + y));
    mff_cycles(0xE844, 19);
    a &= 0x0F;
    RAM(VRAM_BUFFER + x) = a;
    t = (uint8_t)(RAM(VRAM_BUFFER + x) + 1);
    RAM(VRAM_BUFFER + x) = t;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xE853, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_E853(); g_code_window_base = swb; } return;
L_E850:;
    mff_cycles(0xE850, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0852, func_E856, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE853, 9);
    RAM(VRAM_BUFFER_LEN) = x;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $E853 - lifted by port/tools/lift.py */
void func_E853(void) {
    REGS_IN;
    mff_cycles(0xE853, 9);
    RAM(VRAM_BUFFER_LEN) = x;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $E856 - lifted by port/tools/lift.py */
void func_E856(void) {
    REGS_IN;
    mff_cycles(0xE856, 33);
    a = 0x78;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x78;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x78;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/* hud_enemy_bar $E89B - lifted by port/tools/lift.py */
void func_E89B(void) {
    REGS_IN;
    mff_cycles(0xE89B, 2);
    y = 0x02;
L_E89D:;
    mff_cycles(0xE89D, 8);
    a = RAM(OBJ_KIND + y);
    m = 0x07;
    c = a >= m;
    if (a == m) { mff_cycles(0xE8A2, 1); goto L_E8B8; }
    mff_cycles(0xE8A4, 4);
    m = 0x08;
    c = a >= m;
    if (a == m) { mff_cycles(0xE8A6, 1); goto L_E8B8; }
    mff_cycles(0xE8A8, 4);
    m = 0x09;
    c = a >= m;
    if (a == m) { mff_cycles(0xE8AA, 1); goto L_E8B8; }
    mff_cycles(0xE8AC, 4);
    m = 0x0B;
    c = a >= m;
    if (a == m) { mff_cycles(0xE8AE, 1); goto L_E8B8; }
    mff_cycles(0xE8B0, 6);
    y++;
    m = 0x10;
    c = y >= m;
    n = ((uint8_t)(y - m)) >> 7;
    z = ((uint8_t)(y - m)) == 0;
    if (!z) { mff_cycles(0xE89D, 1); goto L_E89D; }
    mff_cycles(0xE8B5, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xE8BD, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_E8BD(); g_code_window_base = swb; } return;
L_E8B8:;
    mff_cycles(0xE8B8, 6);
    a = RAM(OBJ_LIFE + y);
    if (!(a & 0x80)) { mff_cycles(0xE8BB, 1); goto L_E8C2; }
    mff_cycles(0xE8BD, 5);
    a = RAM(HUD_ENEMY_SLOT);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xE8BF, 1); goto L_E8C9; }
    mff_cycles(0xE8C1, 6);
    SYNC_OUT;
    mff_rts(); return;
L_E8C2:;
    mff_cycles(0xE8C2, 10);
    a = 0x01;
    RAM(HUD_ENEMY_TIMER) = a;
    RAM(HUD_ENEMY_SLOT) = y;
    a = y;
L_E8C9:;
    mff_cycles(0xE8C9, 50);
    y = a;
    x = RAM(VRAM_BUFFER_LEN);
    a = 0x0F;
    RAM(VRAM_BUFFER + x) = a;
    RAM(TMP2) = a;
    x++;
    a = 0x20;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x69;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = RAM(OBJ_LIFE_MAX + y);
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    RAM(TMP0) = a;
    if (t == 0) { mff_cycles(0xE8E8, 2); goto L_E93F; }
    mff_cycles(0xE8EA, 12);
    t = (uint8_t)(RAM(HUD_ENEMY_TIMER) + 1);
    RAM(HUD_ENEMY_TIMER) = t;
    a = RAM(HUD_ENEMY_TIMER);
    m = 0x96;
    c = a >= m;
    if (a == m) { mff_cycles(0xE8F0, 2); goto L_E93F; }
    mff_cycles(0xE8F2, 6);
    a = RAM(OBJ_LIFE + y);
    if (!(a & 0x80)) { mff_cycles(0xE8F5, 2); goto L_E903; }
    mff_cycles(0xE8F7, 7);
    a = RAM(HUD_ENEMY_TIMER);
    m = 0x1E;
    c = a >= m;
    if (a == m) { mff_cycles(0xE8FB, 2); goto L_E93F; }
    mff_cycles(0xE8FD, 4);
    a &= 0x02;
    if (a != 0) { mff_cycles(0xE8FF, 1); goto L_E947; }
    mff_cycles(0xE901, 2);
    a = 0x00;
L_E903:;
    mff_cycles(0xE903, 17);
    RAM(TMP1) = a;
    a = 0x19;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    t = (uint8_t)(RAM(TMP2) - 1);
    RAM(TMP2) = t;
L_E90D:;
    mff_cycles(0xE90D, 5);
    a = RAM(TMP1);
    z = (a) == 0;
    if (a & 0x80) { mff_cycles(0xE90F, 1); goto L_E91D; }
    mff_cycles(0xE911, 2);
    if (z) { mff_cycles(0xE911, 1); goto L_E91F; }
    mff_cycles(0xE913, 4);
    m = 0x07;
    z = ((uint8_t)(a - m)) == 0;
    if (a >= m) { mff_cycles(0xE915, 1); goto L_E919; }
    mff_cycles(0xE917, 2);
    if (!z) { mff_cycles(0xE917, 1); goto L_E91F; }
L_E919:;
    mff_cycles(0xE919, 4);
    a = 0x18;
    if (a != 0) { mff_cycles(0xE91B, 1); goto L_E91F; }
L_E91D:;
    mff_cycles(0xE91D, 2);
    a = 0x10;
L_E91F:;
    mff_cycles(0xE91F, 31);
    a |= 0x10;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    t = (uint8_t)(RAM(TMP2) - 1);
    RAM(TMP2) = t;
    a = RAM(TMP1);
    c = 1;
    m = 0x08;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(TMP1) = a;
    t = (uint8_t)(RAM(TMP0) - 1);
    RAM(TMP0) = t;
    if (t != 0) { mff_cycles(0xE90D, 1); goto L_E90D; }
    mff_cycles(0xE932, 16);
    a = 0x1A;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    t = (uint8_t)(RAM(TMP2) - 1);
    RAM(TMP2) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) { mff_cycles(0xE93A, 1); goto L_E947; }
    mff_cycles(0xE93C, 9);
    RAM(VRAM_BUFFER_LEN) = x;
    SYNC_OUT;
    mff_rts(); return;
L_E93F:;
    mff_cycles(0xE93F, 10);
    a = 0x95;
    RAM(HUD_ENEMY_TIMER) = a;
    a = 0x0F;
    RAM(TMP2) = a;
L_E947:;
    for (;;) {
        mff_cycles(0xE947, 16);
        a = 0x00;
        RAM(VRAM_BUFFER + x) = a;
        x++;
        t = (uint8_t)(RAM(TMP2) - 1);
        RAM(TMP2) = t;
        n = (t) >> 7;
        z = (t) == 0;
        if (z) break;
        mff_cycles(0xE947, 1);
    }
    mff_cycles(0xE951, 9);
    RAM(VRAM_BUFFER_LEN) = x;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $E8BD - lifted by port/tools/lift.py */
void func_E8BD(void) {
    REGS_IN;
    mff_cycles(0xE8BD, 5);
    a = RAM(HUD_ENEMY_SLOT);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0xE8C1, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xE8BF, 1);
    }
    mff_cycles(0xE8C9, 50);
    y = a;
    x = RAM(VRAM_BUFFER_LEN);
    a = 0x0F;
    RAM(VRAM_BUFFER + x) = a;
    RAM(TMP2) = a;
    x++;
    a = 0x20;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x69;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = RAM(OBJ_LIFE_MAX + y);
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    RAM(TMP0) = a;
    if (t == 0) { mff_cycles(0xE8E8, 2); goto L_E93F; }
    mff_cycles(0xE8EA, 12);
    t = (uint8_t)(RAM(HUD_ENEMY_TIMER) + 1);
    RAM(HUD_ENEMY_TIMER) = t;
    a = RAM(HUD_ENEMY_TIMER);
    m = 0x96;
    c = a >= m;
    if (a == m) { mff_cycles(0xE8F0, 2); goto L_E93F; }
    mff_cycles(0xE8F2, 6);
    a = RAM(OBJ_LIFE + y);
    if (!(a & 0x80)) { mff_cycles(0xE8F5, 2); goto L_E903; }
    mff_cycles(0xE8F7, 7);
    a = RAM(HUD_ENEMY_TIMER);
    m = 0x1E;
    c = a >= m;
    if (a == m) { mff_cycles(0xE8FB, 2); goto L_E93F; }
    mff_cycles(0xE8FD, 4);
    a &= 0x02;
    if (a != 0) { mff_cycles(0xE8FF, 1); goto L_E947; }
    mff_cycles(0xE901, 2);
    a = 0x00;
L_E903:;
    mff_cycles(0xE903, 17);
    RAM(TMP1) = a;
    a = 0x19;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    t = (uint8_t)(RAM(TMP2) - 1);
    RAM(TMP2) = t;
L_E90D:;
    mff_cycles(0xE90D, 5);
    a = RAM(TMP1);
    z = (a) == 0;
    if (a & 0x80) { mff_cycles(0xE90F, 1); goto L_E91D; }
    mff_cycles(0xE911, 2);
    if (z) { mff_cycles(0xE911, 1); goto L_E91F; }
    mff_cycles(0xE913, 4);
    m = 0x07;
    z = ((uint8_t)(a - m)) == 0;
    if (a >= m) { mff_cycles(0xE915, 1); goto L_E919; }
    mff_cycles(0xE917, 2);
    if (!z) { mff_cycles(0xE917, 1); goto L_E91F; }
L_E919:;
    mff_cycles(0xE919, 4);
    a = 0x18;
    if (a != 0) { mff_cycles(0xE91B, 1); goto L_E91F; }
L_E91D:;
    mff_cycles(0xE91D, 2);
    a = 0x10;
L_E91F:;
    mff_cycles(0xE91F, 31);
    a |= 0x10;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    t = (uint8_t)(RAM(TMP2) - 1);
    RAM(TMP2) = t;
    a = RAM(TMP1);
    c = 1;
    m = 0x08;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(TMP1) = a;
    t = (uint8_t)(RAM(TMP0) - 1);
    RAM(TMP0) = t;
    if (t != 0) { mff_cycles(0xE90D, 1); goto L_E90D; }
    mff_cycles(0xE932, 16);
    a = 0x1A;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    t = (uint8_t)(RAM(TMP2) - 1);
    RAM(TMP2) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) { mff_cycles(0xE93A, 1); goto L_E947; }
    mff_cycles(0xE93C, 9);
    RAM(VRAM_BUFFER_LEN) = x;
    SYNC_OUT;
    mff_rts(); return;
L_E93F:;
    mff_cycles(0xE93F, 10);
    a = 0x95;
    RAM(HUD_ENEMY_TIMER) = a;
    a = 0x0F;
    RAM(TMP2) = a;
L_E947:;
    for (;;) {
        mff_cycles(0xE947, 16);
        a = 0x00;
        RAM(VRAM_BUFFER + x) = a;
        x++;
        t = (uint8_t)(RAM(TMP2) - 1);
        RAM(TMP2) = t;
        n = (t) >> 7;
        z = (t) == 0;
        if (z) break;
        mff_cycles(0xE947, 1);
    }
    mff_cycles(0xE951, 9);
    RAM(VRAM_BUFFER_LEN) = x;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $E954 - lifted by port/tools/lift.py */
void func_E954(void) {
    REGS_IN;
    mff_cycles(0xE954, 40);
    x = RAM(VRAM_BUFFER_LEN);
    a = 0x06;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x23;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x49;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    y = RAM(PLAYER_LEVEL);
    a = RAM(PLAYER_ID);
    if (a == 0x02) {
        mff_cycles(0xE970, 4);
        y++;
        y++;
    } else {
        mff_cycles(0xE96E, 1);
    }
    mff_cycles(0xE972, 4);
    a = prg_read((uint16_t)(0xE98C + y));
    mff_cycles(0xE975, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0xE992 + y));
    mff_cycles(0xE97A, 5);
    RAM(TMP1) = a;
    y = 0x00;
    for (;;) {
        mff_cycles(0xE97E, 5);
        a = mem_read((uint16_t)(RAM16(TMP0) + y));
        mff_cycles(0xE980, 13);
        RAM(VRAM_BUFFER + x) = a;
        x++;
        y++;
        m = 0x06;
        c = y >= m;
        n = ((uint8_t)(y - m)) >> 7;
        z = ((uint8_t)(y - m)) == 0;
        if (z) break;
        mff_cycles(0xE97E, 1);
    }
    mff_cycles(0xE989, 9);
    RAM(VRAM_BUFFER_LEN) = x;
    SYNC_OUT;
    mff_rts(); return;
}

/* objects_draw $E9BC - lifted by port/tools/lift.py */
void func_E9BC(void) {
    REGS_IN;
    mff_cycles(0xE9BC, 8);
    x = 0x0F;
    a = RAM(OBJ_STATE + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0xE9C3, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x09C5, func_E9FE, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xE9C1, 1);
    }
    mff_cycles(0xE9C6, 8);
    x--;
    a = RAM(OBJ_STATE + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0xE9CC, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x09CE, func_E9FE, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xE9CA, 1);
    }
    mff_cycles(0xE9CF, 12);
    t = (uint8_t)(RAM(DRAW_PARITY) + 1);
    RAM(DRAW_PARITY) = t;
    a = RAM(DRAW_PARITY);
    t = a;
    c = t & 1;
    t >>= 1;
    if (!c) {
        mff_cycles(0xE9D6, 5);
        x = 0x00;
        RAM(DRAW_SLOT) = x;
        for (;;) {
            mff_cycles(0xE9DA, 6);
            a = RAM(OBJ_STATE + x);
            n = (a) >> 7;
            z = (a) == 0;
            if (n) {
                mff_cycles(0xE9DF, 6);
                SYNC_OUT;
                if (!mff_call_fn(0x09E1, func_E9FE, 0xE000)) return;
                SYNC_IN;
            } else {
                mff_cycles(0xE9DD, 1);
            }
            mff_cycles(0xE9E2, 12);
            t = (uint8_t)(RAM(DRAW_SLOT) + 1);
            RAM(DRAW_SLOT) = t;
            x = RAM(DRAW_SLOT);
            m = 0x10;
            c = x >= m;
            n = ((uint8_t)(x - m)) >> 7;
            z = ((uint8_t)(x - m)) == 0;
            if (z) break;
            mff_cycles(0xE9DA, 1);
        }
        mff_cycles(0xE9EA, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xE9D4, 1);
    }
    mff_cycles(0xE9EB, 5);
    x = 0x0D;
    RAM(DRAW_SLOT) = x;
    for (;;) {
        mff_cycles(0xE9EF, 6);
        a = RAM(OBJ_STATE + x);
        n = (a) >> 7;
        z = (a) == 0;
        if (n) {
            mff_cycles(0xE9F4, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x09F6, func_E9FE, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xE9F2, 1);
        }
        mff_cycles(0xE9F7, 10);
        t = (uint8_t)(RAM(DRAW_SLOT) - 1);
        RAM(DRAW_SLOT) = t;
        x = RAM(DRAW_SLOT);
        n = (x) >> 7;
        z = (x) == 0;
        if (n) break;
        mff_cycles(0xE9EF, 1);
    }
    mff_cycles(0xE9FD, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/* object_draw $E9FE - lifted by port/tools/lift.py */
void func_E9FE(void) {
    REGS_IN;
    mff_cycles(0xE9FE, 8);
    a = RAM(OBJ_FLAGS + x);
    a &= 0x10;
    if (a == 0) { mff_cycles(0xEA03, 1); goto L_EA58; }
    mff_cycles(0xEA05, 21);
    a = RAM(OBJ_X + x);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(TMP19) = a;
    a = RAM(OBJ_X_HI + x);
    m = RAM(SCROLL_X_HI);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (a != 0) { mff_cycles(0xEA12, 1); goto L_EA24; }
    mff_cycles(0xEA14, 6);
    a = RAM(OBJ_Y_HI + x);
    if (a == 0) { mff_cycles(0xEA17, 1); goto L_EA5D; }
    mff_cycles(0xEA19, 4);
    m = 0x00;
    c = x >= m;
    if (x == m) { mff_cycles(0xEA1B, 1); goto L_EA4D; }
    mff_cycles(0xEA1D, 6);
    a = RAM(OBJ_Y_HI + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) { mff_cycles(0xEA20, 1); goto L_EA30; }
    mff_cycles(0xEA22, 2);
    if (!n) { mff_cycles(0xEA22, 1); goto L_EA30; }
L_EA24:;
    mff_cycles(0xEA24, 5);
    a = RAM(TMP19);
    if (!c) {
        mff_cycles(0xEA28, 4);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xEA26, 1);
    }
    mff_cycles(0xEA2C, 4);
    m = 0x68;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xEA2E, 1); goto L_EA34; }
L_EA30:;
    mff_cycles(0xEA30, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0A32, func_EC33, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xEA33, 6);
    SYNC_OUT;
    mff_rts(); return;
L_EA34:;
    mff_cycles(0xEA34, 4);
    m = 0x00;
    c = a >= m;
    if (!c) { mff_cycles(0xEA36, 1); goto L_EA4D; }
    mff_cycles(0xEA38, 8);
    a = RAM(OBJ_ANIM + x);
    m = 0x7E;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xEA30, 1); goto L_EA30; }
    mff_cycles(0xEA3F, 7);
    a = RAM(ROUND);
    m = 0x03;
    c = a >= m;
    if (a != m) { mff_cycles(0xEA43, 1); goto L_EA4D; }
    mff_cycles(0xEA45, 7);
    a = RAM(SUB_AREA);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xEA49, 1); goto L_EA4D; }
    mff_cycles(0xEA4B, 2);
    if (z) { mff_cycles(0xEA30, 1); goto L_EA30; }
L_EA4D:;
    mff_cycles(0xEA4D, 14);
    a = RAM(OBJ_FLAGS + x);
    a &= 0x7F;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xEA6F, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_EA6F(); g_code_window_base = swb; } return;
L_EA58:;
    mff_cycles(0xEA58, 7);
    a = RAM(OBJ_X + x);
    RAM(TMP19) = a;
L_EA5D:;
    mff_cycles(0xEA5D, 22);
    a = RAM(OBJ_Y + x);
    RAM(TMP18) = a;
    a = RAM(OBJ_FLAGS + x);
    a |= 0x80;
    RAM(OBJ_FLAGS + x) = a;
    a &= 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xEA6C, 1); goto L_EA6F; }
L_EA6E:;
    mff_cycles(0xEA6E, 6);
    SYNC_OUT;
    mff_rts(); return;
L_EA6F:;
    mff_cycles(0xEA6F, 8);
    y = RAM(OBJ_ID + x);
    a = prg_read((uint16_t)(0xEB93 + y));
    mff_cycles(0xEA75, 19);
    RAM(BANK_REQUEST) = a;
    c = 0;
    m = 0x01;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(BANK_REQUEST + 1) = a;
    RAM(TMP0) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0A80, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xEA81, 32);
    x = RAM(TMP0);
    y = 0x00;
    a = RAM(OBJ_FLAGS + x);
    mff_push(a);
    a &= 0x40;
    RAM(TMP16) = a;
    a = mff_pull();
    a &= 0x20;
    RAM(TMP17) = a;
    a = RAM(OBJ_ANIM + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xEA6E, 1); goto L_EA6E; }
    mff_cycles(0xEA97, 8);
    a &= 0x7F;
    y = a;
    a = prg_read((uint16_t)(0x8000 + y));
    mff_cycles(0xEA9D, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0x8080 + y));
    mff_cycles(0xEAA2, 23);
    RAM(TMP1) = a;
    a = RAM(OBJ_ANIM_TIMER + x);
    a &= 0x7F;
    t = (uint8_t)(RAM(OBJ_ANIM_TIMER + x) + 1);
    RAM(OBJ_ANIM_TIMER + x) = t;
    y = 0x01;
    m = mem_read((uint16_t)(RAM16(TMP0) + y));
    c = a >= m;
    mff_cycles(0xEAB0, 2);
    if (a == m) {
        mff_cycles(0xEAB2, 31);
        a = RAM(OBJ_ANIM_TIMER + x);
        a &= 0x80;
        RAM(OBJ_ANIM_TIMER + x) = a;
        a = RAM(OBJ_ANIM_FRAME + x);
        a &= 0x7F;
        t = (uint8_t)(RAM(OBJ_ANIM_FRAME + x) + 1);
        RAM(OBJ_ANIM_FRAME + x) = t;
        y--;
        m = mem_read((uint16_t)(RAM16(TMP0) + y));
        c = a >= m;
        mff_cycles(0xEAC5, 2);
        if (a == m) {
            mff_cycles(0xEAC7, 7);
            a = 0x00;
            RAM(OBJ_ANIM_FRAME + x) = a;
        } else {
            mff_cycles(0xEAC5, 1);
        }
    } else {
        mff_cycles(0xEAB0, 1);
    }
    mff_cycles(0xEACC, 6);
    a = RAM(OBJ_FLAGS + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0xEACF, 1); goto L_EAEF; }
    mff_cycles(0xEAD1, 6);
    a = RAM(OBJ_ANIM_TIMER + x);
    if (!(a & 0x80)) { mff_cycles(0xEAD4, 1); goto L_EADC; }
    mff_cycles(0xEAD6, 7);
    a = RAM(FRAME_COUNTER);
    a &= 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xEADA, 1); goto L_EAEF; }
L_EADC:;
    mff_cycles(0xEADC, 17);
    a = RAM(OBJ_ANIM_FRAME + x);
    a &= 0x7F;
    c = 0;
    m = 0x02;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    y = a;
    a = mem_read((uint16_t)(RAM16(TMP0) + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xEAE7, 2);
    if (!z) { mff_cycles(0xEAE7, 1); goto L_EAF0; }
    mff_cycles(0xEAE9, 8);
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xEC81, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_EC81(); g_code_window_base = swb; } return;
L_EAEF:;
    mff_cycles(0xEAEF, 6);
    SYNC_OUT;
    mff_rts(); return;
L_EAF0:;
    mff_cycles(0xEAF0, 6);
    y = a;
    a = prg_read((uint16_t)(0x8100 + y));
    mff_cycles(0xEAF4, 7);
    RAM(TMP2) = a;
    a = prg_read((uint16_t)(0x8200 + y));
    mff_cycles(0xEAF9, 10);
    RAM(TMP3) = a;
    y = 0x00;
    a = mem_read((uint16_t)(RAM16(TMP2) + y));
    mff_cycles(0xEAFF, 11);
    mff_push(a);
    y = 0x0B;
    a = mff_pull();
    if (a & 0x80) {
        mff_cycles(0xEB05, 4);
        a &= 0x7F;
        y = 0x03;
    } else {
        mff_cycles(0xEB03, 1);
    }
    mff_cycles(0xEB09, 8);
    RAM(TMP4) = a;
    m = RAM(BANK_REQUEST + 1);
    c = y >= m;
    n = ((uint8_t)(y - m)) >> 7;
    z = ((uint8_t)(y - m)) == 0;
    if (!z) {
        mff_cycles(0xEB0F, 12);
        RAM(BANK_REQUEST + 1) = y;
        RAM(TMP5) = x;
        SYNC_OUT;
        if (!mff_call_fn(0x0B15, func_FF4D, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xEB16, 3);
    } else {
        mff_cycles(0xEB0D, 1);
    }
    mff_cycles(0xEB18, 7);
    y = 0x01;
    a = mem_read((uint16_t)(RAM16(TMP2) + y));
    mff_cycles(0xEB1C, 6);
    x = a;
    a = prg_read((uint16_t)(0xB100 + x));
    mff_cycles(0xEB20, 11);
    c = 1;
    m = 0x02;
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(TMP5) = a;
    a = prg_read((uint16_t)(0xB200 + x));
    mff_cycles(0xEB28, 10);
    m = 0x00;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(TMP6) = a;
    x = RAM(OAM_INDEX);
    n = (x) >> 7;
    z = (x) == 0;
    if (z) { mff_cycles(0xEB2E, 1); goto L_EB8A; }
L_EB30:;
    mff_cycles(0xEB30, 12);
    a = 0xF0;
    RAM(TMP0) = a;
    y++;
    a = mem_read((uint16_t)(RAM16(TMP2) + y));
    mff_cycles(0xEB37, 15);
    RAM(OAM_BUFFER + 1 + x) = a;
    a = RAM(TMP18);
    c = 0;
    m = mem_read((uint16_t)(RAM16(TMP5) + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xEB3F, 10);
    RAM(OAM_BUFFER + x) = a;
    a = mem_read((uint16_t)(RAM16(TMP5) + y));
    mff_cycles(0xEB44, 2);
    if (a & 0x80) { mff_cycles(0xEB44, 1); goto L_EB4A; }
    mff_cycles(0xEB46, 2);
    if (!c) { mff_cycles(0xEB46, 1); goto L_EB4C; }
    mff_cycles(0xEB48, 2);
    if (c) { mff_cycles(0xEB48, 1); goto L_EB8B; }
L_EB4A:;
    mff_cycles(0xEB4A, 2);
    if (!c) { mff_cycles(0xEB4A, 1); goto L_EB8B; }
L_EB4C:;
    mff_cycles(0xEB4C, 9);
    a = RAM(OAM_BUFFER + x);
    m = RAM(TMP0);
    c = a >= m;
    if (c) { mff_cycles(0xEB51, 1); goto L_EB8B; }
    mff_cycles(0xEB53, 7);
    y++;
    a = mem_read((uint16_t)(RAM16(TMP2) + y));
    mff_cycles(0xEB56, 16);
    a ^= RAM(TMP16);
    a |= RAM(TMP17);
    RAM(OAM_BUFFER + 2 + x) = a;
    a = mem_read((uint16_t)(RAM16(TMP5) + y));
    mff_cycles(0xEB5F, 8);
    RAM(TMP1) = a;
    a = RAM(TMP16);
    if (a != 0) {
        mff_cycles(0xEB65, 10);
        a = 0xF8;
        c = 1;
        m = RAM(TMP1);
        w = a - m - !c;
        a = (uint8_t)w;
        RAM(TMP1) = a;
    } else {
        mff_cycles(0xEB63, 1);
    }
    mff_cycles(0xEB6C, 18);
    a = RAM(TMP19);
    c = 0;
    m = RAM(TMP1);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OAM_BUFFER + 3 + x) = a;
    a = RAM(TMP1);
    if (a & 0x80) { mff_cycles(0xEB76, 1); goto L_EB7C; }
    mff_cycles(0xEB78, 2);
    if (!c) { mff_cycles(0xEB78, 1); goto L_EB7E; }
    mff_cycles(0xEB7A, 2);
    if (c) { mff_cycles(0xEB7A, 1); goto L_EB8C; }
L_EB7C:;
    mff_cycles(0xEB7C, 2);
    if (!c) { mff_cycles(0xEB7C, 1); goto L_EB8C; }
L_EB7E:;
    mff_cycles(0xEB7E, 13);
    x++;
    x++;
    x++;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(OAM_INDEX) = x;
    if (z) { mff_cycles(0xEB84, 1); goto L_EB8A; }
L_EB86:;
    mff_cycles(0xEB86, 7);
    t = (uint8_t)(RAM(TMP4) - 1);
    RAM(TMP4) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!n) { mff_cycles(0xEB30, 1); goto L_EB30; }
L_EB8A:;
    mff_cycles(0xEB8A, 6);
    SYNC_OUT;
    mff_rts(); return;
L_EB8B:;
    mff_cycles(0xEB8B, 2);
    y++;
L_EB8C:;
    mff_cycles(0xEB8C, 9);
    a = 0xF8;
    RAM(OAM_BUFFER + x) = a;
    if (a != 0) { mff_cycles(0xEB86, 1); goto L_EB86; }
    mff_cycles(0xEB93, 4);
    mff_unreachable(0xEB95); return;   /* data: the branch above always goes */
}

/*  $EA6F - lifted by port/tools/lift.py */
void func_EA6F(void) {
    REGS_IN;
    goto L_EA6F;
L_EA6E:;
    mff_cycles(0xEA6E, 6);
    SYNC_OUT;
    mff_rts(); return;
L_EA6F:;
    mff_cycles(0xEA6F, 8);
    y = RAM(OBJ_ID + x);
    a = prg_read((uint16_t)(0xEB93 + y));
    mff_cycles(0xEA75, 19);
    RAM(BANK_REQUEST) = a;
    c = 0;
    m = 0x01;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(BANK_REQUEST + 1) = a;
    RAM(TMP0) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0A80, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xEA81, 32);
    x = RAM(TMP0);
    y = 0x00;
    a = RAM(OBJ_FLAGS + x);
    mff_push(a);
    a &= 0x40;
    RAM(TMP16) = a;
    a = mff_pull();
    a &= 0x20;
    RAM(TMP17) = a;
    a = RAM(OBJ_ANIM + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xEA6E, 1); goto L_EA6E; }
    mff_cycles(0xEA97, 8);
    a &= 0x7F;
    y = a;
    a = prg_read((uint16_t)(0x8000 + y));
    mff_cycles(0xEA9D, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0x8080 + y));
    mff_cycles(0xEAA2, 23);
    RAM(TMP1) = a;
    a = RAM(OBJ_ANIM_TIMER + x);
    a &= 0x7F;
    t = (uint8_t)(RAM(OBJ_ANIM_TIMER + x) + 1);
    RAM(OBJ_ANIM_TIMER + x) = t;
    y = 0x01;
    m = mem_read((uint16_t)(RAM16(TMP0) + y));
    c = a >= m;
    mff_cycles(0xEAB0, 2);
    if (a == m) {
        mff_cycles(0xEAB2, 31);
        a = RAM(OBJ_ANIM_TIMER + x);
        a &= 0x80;
        RAM(OBJ_ANIM_TIMER + x) = a;
        a = RAM(OBJ_ANIM_FRAME + x);
        a &= 0x7F;
        t = (uint8_t)(RAM(OBJ_ANIM_FRAME + x) + 1);
        RAM(OBJ_ANIM_FRAME + x) = t;
        y--;
        m = mem_read((uint16_t)(RAM16(TMP0) + y));
        c = a >= m;
        mff_cycles(0xEAC5, 2);
        if (a == m) {
            mff_cycles(0xEAC7, 7);
            a = 0x00;
            RAM(OBJ_ANIM_FRAME + x) = a;
        } else {
            mff_cycles(0xEAC5, 1);
        }
    } else {
        mff_cycles(0xEAB0, 1);
    }
    mff_cycles(0xEACC, 6);
    a = RAM(OBJ_FLAGS + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0xEACF, 1); goto L_EAEF; }
    mff_cycles(0xEAD1, 6);
    a = RAM(OBJ_ANIM_TIMER + x);
    if (!(a & 0x80)) { mff_cycles(0xEAD4, 1); goto L_EADC; }
    mff_cycles(0xEAD6, 7);
    a = RAM(FRAME_COUNTER);
    a &= 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xEADA, 1); goto L_EAEF; }
L_EADC:;
    mff_cycles(0xEADC, 17);
    a = RAM(OBJ_ANIM_FRAME + x);
    a &= 0x7F;
    c = 0;
    m = 0x02;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    y = a;
    a = mem_read((uint16_t)(RAM16(TMP0) + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xEAE7, 2);
    if (!z) { mff_cycles(0xEAE7, 1); goto L_EAF0; }
    mff_cycles(0xEAE9, 8);
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xEC81, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_EC81(); g_code_window_base = swb; } return;
L_EAEF:;
    mff_cycles(0xEAEF, 6);
    SYNC_OUT;
    mff_rts(); return;
L_EAF0:;
    mff_cycles(0xEAF0, 6);
    y = a;
    a = prg_read((uint16_t)(0x8100 + y));
    mff_cycles(0xEAF4, 7);
    RAM(TMP2) = a;
    a = prg_read((uint16_t)(0x8200 + y));
    mff_cycles(0xEAF9, 10);
    RAM(TMP3) = a;
    y = 0x00;
    a = mem_read((uint16_t)(RAM16(TMP2) + y));
    mff_cycles(0xEAFF, 11);
    mff_push(a);
    y = 0x0B;
    a = mff_pull();
    if (a & 0x80) {
        mff_cycles(0xEB05, 4);
        a &= 0x7F;
        y = 0x03;
    } else {
        mff_cycles(0xEB03, 1);
    }
    mff_cycles(0xEB09, 8);
    RAM(TMP4) = a;
    m = RAM(BANK_REQUEST + 1);
    c = y >= m;
    n = ((uint8_t)(y - m)) >> 7;
    z = ((uint8_t)(y - m)) == 0;
    if (!z) {
        mff_cycles(0xEB0F, 12);
        RAM(BANK_REQUEST + 1) = y;
        RAM(TMP5) = x;
        SYNC_OUT;
        if (!mff_call_fn(0x0B15, func_FF4D, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xEB16, 3);
    } else {
        mff_cycles(0xEB0D, 1);
    }
    mff_cycles(0xEB18, 7);
    y = 0x01;
    a = mem_read((uint16_t)(RAM16(TMP2) + y));
    mff_cycles(0xEB1C, 6);
    x = a;
    a = prg_read((uint16_t)(0xB100 + x));
    mff_cycles(0xEB20, 11);
    c = 1;
    m = 0x02;
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(TMP5) = a;
    a = prg_read((uint16_t)(0xB200 + x));
    mff_cycles(0xEB28, 10);
    m = 0x00;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(TMP6) = a;
    x = RAM(OAM_INDEX);
    n = (x) >> 7;
    z = (x) == 0;
    if (z) { mff_cycles(0xEB2E, 1); goto L_EB8A; }
L_EB30:;
    mff_cycles(0xEB30, 12);
    a = 0xF0;
    RAM(TMP0) = a;
    y++;
    a = mem_read((uint16_t)(RAM16(TMP2) + y));
    mff_cycles(0xEB37, 15);
    RAM(OAM_BUFFER + 1 + x) = a;
    a = RAM(TMP18);
    c = 0;
    m = mem_read((uint16_t)(RAM16(TMP5) + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xEB3F, 10);
    RAM(OAM_BUFFER + x) = a;
    a = mem_read((uint16_t)(RAM16(TMP5) + y));
    mff_cycles(0xEB44, 2);
    if (a & 0x80) { mff_cycles(0xEB44, 1); goto L_EB4A; }
    mff_cycles(0xEB46, 2);
    if (!c) { mff_cycles(0xEB46, 1); goto L_EB4C; }
    mff_cycles(0xEB48, 2);
    if (c) { mff_cycles(0xEB48, 1); goto L_EB8B; }
L_EB4A:;
    mff_cycles(0xEB4A, 2);
    if (!c) { mff_cycles(0xEB4A, 1); goto L_EB8B; }
L_EB4C:;
    mff_cycles(0xEB4C, 9);
    a = RAM(OAM_BUFFER + x);
    m = RAM(TMP0);
    c = a >= m;
    if (c) { mff_cycles(0xEB51, 1); goto L_EB8B; }
    mff_cycles(0xEB53, 7);
    y++;
    a = mem_read((uint16_t)(RAM16(TMP2) + y));
    mff_cycles(0xEB56, 16);
    a ^= RAM(TMP16);
    a |= RAM(TMP17);
    RAM(OAM_BUFFER + 2 + x) = a;
    a = mem_read((uint16_t)(RAM16(TMP5) + y));
    mff_cycles(0xEB5F, 8);
    RAM(TMP1) = a;
    a = RAM(TMP16);
    if (a != 0) {
        mff_cycles(0xEB65, 10);
        a = 0xF8;
        c = 1;
        m = RAM(TMP1);
        w = a - m - !c;
        a = (uint8_t)w;
        RAM(TMP1) = a;
    } else {
        mff_cycles(0xEB63, 1);
    }
    mff_cycles(0xEB6C, 18);
    a = RAM(TMP19);
    c = 0;
    m = RAM(TMP1);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OAM_BUFFER + 3 + x) = a;
    a = RAM(TMP1);
    if (a & 0x80) { mff_cycles(0xEB76, 1); goto L_EB7C; }
    mff_cycles(0xEB78, 2);
    if (!c) { mff_cycles(0xEB78, 1); goto L_EB7E; }
    mff_cycles(0xEB7A, 2);
    if (c) { mff_cycles(0xEB7A, 1); goto L_EB8C; }
L_EB7C:;
    mff_cycles(0xEB7C, 2);
    if (!c) { mff_cycles(0xEB7C, 1); goto L_EB8C; }
L_EB7E:;
    mff_cycles(0xEB7E, 13);
    x++;
    x++;
    x++;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(OAM_INDEX) = x;
    if (z) { mff_cycles(0xEB84, 1); goto L_EB8A; }
L_EB86:;
    mff_cycles(0xEB86, 7);
    t = (uint8_t)(RAM(TMP4) - 1);
    RAM(TMP4) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!n) { mff_cycles(0xEB30, 1); goto L_EB30; }
L_EB8A:;
    mff_cycles(0xEB8A, 6);
    SYNC_OUT;
    mff_rts(); return;
L_EB8B:;
    mff_cycles(0xEB8B, 2);
    y++;
L_EB8C:;
    mff_cycles(0xEB8C, 9);
    a = 0xF8;
    RAM(OAM_BUFFER + x) = a;
    if (a != 0) { mff_cycles(0xEB86, 1); goto L_EB86; }
    mff_cycles(0xEB93, 4);
    mff_unreachable(0xEB95); return;   /* data: the branch above always goes */
}

/* object_offscreen $EC33 - lifted by port/tools/lift.py */
void func_EC33(void) {
    REGS_IN;
    mff_cycles(0xEC33, 7);
    a = RAM(ROUND);
    if (a != 0x03) { mff_cycles(0xEC37, 1); goto L_EC41; }
    mff_cycles(0xEC39, 7);
    a = RAM(SUB_AREA);
    m = 0x01;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xEC3D, 1); goto L_EC41; }
    mff_cycles(0xEC3F, 2);
    if (z) { mff_cycles(0xEC3F, 1); goto L_EC81; }
L_EC41:;
    mff_cycles(0xEC41, 2);
    y = 0x00;
L_EC43:;
    mff_cycles(0xEC43, 4);
    a = prg_read((uint16_t)(0xECC3 + y));
    mff_cycles(0xEC46, 6);
    m = RAM(OBJ_ID + x);
    if (a == m) { mff_cycles(0xEC49, 1); goto L_EC81; }
    mff_cycles(0xEC4B, 4);
    m = 0xFF;
    c = a >= m;
    if (a == m) { mff_cycles(0xEC4D, 1); goto L_EC53; }
    mff_cycles(0xEC4F, 4);
    y++;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0xEC43, 1); goto L_EC43; }
    mff_cycles(0xEC52, 6);
    SYNC_OUT;
    mff_rts(); return;
L_EC53:;
    mff_cycles(0xEC53, 11);
    a = RAM(SCROLL_X_HI);
    c = 1;
    m = RAM(OBJ_X_HI + x);
    w = a - m - !c;
    a = (uint8_t)w;
    if (!(a & 0x80)) {
        mff_cycles(0xEC5B, 32);
        a = RAM(OBJ_X + x);
        c = 0;
        m = 0x10;
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(OBJ_X + x) = a;
        a = RAM(OBJ_X_HI + x);
        m = 0x00;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_X_HI + x) = a;
        c = 1;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xEC59, 1);
    }
    mff_cycles(0xEC6E, 32);
    a = RAM(OBJ_X + x);
    c = 1;
    m = 0x10;
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_X + x) = a;
    a = RAM(OBJ_X_HI + x);
    m = 0x00;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X_HI + x) = a;
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
L_EC81:;
    mff_cycles(0xEC81, 112);
    a = 0x00;
    RAM(OBJ_STATE + x) = a;
    RAM(OBJ_SAVED_STATE + x) = a;
    RAM(OBJ_FLAGS + x) = a;
    RAM(OBJ_THROW_FLAG + x) = a;
    RAM(OBJ_HIT_COOLDOWN + x) = a;
    RAM(OBJ_ID + x) = a;
    RAM(OBJ_SAVED_ID + x) = a;
    RAM(OBJ_ATTACK + x) = a;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    RAM(OBJ_INVULN + x) = a;
    RAM(OBJ_HIT_PLAYER + x) = a;
    RAM(OBJ_HIT_TIMER + x) = a;
    RAM(OBJ_KIND + x) = a;
    RAM(OBJ_TIER + x) = a;
    RAM(OBJ_AI_TIMER + x) = a;
    RAM(OBJ_AI_COUNT + x) = a;
    RAM(OBJ_AI_FLAG + x) = a;
    RAM(OBJ_SPAWN_GROUP + x) = a;
    RAM(OBJ_HELD + x) = a;
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_SPAWN_INDEX + x) = a;
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
}

/* object_free $EC81 - lifted by port/tools/lift.py */
void func_EC81(void) {
    REGS_IN;
    mff_cycles(0xEC81, 112);
    a = 0x00;
    RAM(OBJ_STATE + x) = a;
    RAM(OBJ_SAVED_STATE + x) = a;
    RAM(OBJ_FLAGS + x) = a;
    RAM(OBJ_THROW_FLAG + x) = a;
    RAM(OBJ_HIT_COOLDOWN + x) = a;
    RAM(OBJ_ID + x) = a;
    RAM(OBJ_SAVED_ID + x) = a;
    RAM(OBJ_ATTACK + x) = a;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    RAM(OBJ_INVULN + x) = a;
    RAM(OBJ_HIT_PLAYER + x) = a;
    RAM(OBJ_HIT_TIMER + x) = a;
    RAM(OBJ_KIND + x) = a;
    RAM(OBJ_TIER + x) = a;
    RAM(OBJ_AI_TIMER + x) = a;
    RAM(OBJ_AI_COUNT + x) = a;
    RAM(OBJ_AI_FLAG + x) = a;
    RAM(OBJ_SPAWN_GROUP + x) = a;
    RAM(OBJ_HELD + x) = a;
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_SPAWN_INDEX + x) = a;
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
}

/* bg_probe $ECCF - lifted by port/tools/lift.py */
void func_ECCF(void) {
    REGS_IN;
    mff_cycles(0xECCF, 4);
    a = prg_read((uint16_t)(0xEEEB + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xECD2, 9);
    RAM(PROBE_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0CD6, func_EEC4, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xECD7, 6);
    y = a;
    a = prg_read((uint16_t)(0xEEF6 + y));
    mff_cycles(0xECDB, 14);
    RAM(TMP2) = a;
    a = RAM(OBJ_Y_HI + x);
    RAM(TMP3) = a;
    a = prg_read((uint16_t)(0xEEF7 + y));
    mff_cycles(0xECE5, 18);
    mff_push(a);
    c = 0;
    m = RAM(OBJ_Y + x);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(TMP17) = a;
    a = mff_pull();
    if (a & 0x80) { mff_cycles(0xECED, 1); goto L_ECF9; }
    mff_cycles(0xECEF, 2);
    if (c) { mff_cycles(0xECEF, 1); goto L_ECFB; }
    mff_cycles(0xECF1, 7);
    a = RAM(TMP17);
    m = 0xF0;
    c = a >= m;
    if (c) { mff_cycles(0xECF5, 1); goto L_ECFB; }
    mff_cycles(0xECF7, 2);
    if (!c) { mff_cycles(0xECF7, 2); goto L_ED08; }
L_ECF9:;
    mff_cycles(0xECF9, 2);
    if (c) { mff_cycles(0xECF9, 2); goto L_ED08; }
L_ECFB:;
    mff_cycles(0xECFB, 5);
    a = 0x00;
    y = RAM(TMP2);
    for (;;) {
        mff_cycles(0xECFF, 9);
        RAM(PROBE_HITS + y) = a;
        y--;
        n = (y) >> 7;
        z = (y) == 0;
        if (n) break;
        mff_cycles(0xECFF, 2);
    }
    mff_cycles(0xED05, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xEEDF, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_EEDF(); g_code_window_base = swb; } return;
L_ED08:;
    mff_cycles(0xED08, 5);
    a = RAM(TMP3);
    if (a != 0) { mff_cycles(0xECFB, 2); goto L_ECFB; }
    mff_cycles(0xED0C, 39);
    a = RAM(TMP17);
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    mff_push(a);
    a &= 0x38;
    c = 1;
    m = 0x08;
    w = a - m - !c;
    a = (uint8_t)w;
    RAM(MAP_CELL) = a;
    a = mff_pull();
    t = a;
    t >>= 1;
    a = t;
    a &= 0x02;
    RAM(TMP3) = a;
    a = 0x00;
    RAM(TMP4) = a;
    a = prg_read((uint16_t)(0xEEF8 + y));
    mff_cycles(0xED25, 2);
    if (a & 0x80) {
        mff_cycles(0xED27, 5);
        t = (uint8_t)(RAM(TMP4) - 1);
        RAM(TMP4) = t;
    } else {
        mff_cycles(0xED25, 1);
    }
    mff_cycles(0xED29, 53);
    c = 0;
    m = RAM(OBJ_X + x);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(TMP18) = a;
    a = RAM(OBJ_X_HI + x);
    m = RAM(TMP4);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(TMP19) = a;
    a = RAM(TMP18);
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
    mff_push(a);
    a &= 0x01;
    a |= RAM(TMP3);
    RAM(TMP3) = a;
    a = mff_pull();
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    a |= RAM(MAP_CELL);
    RAM(MAP_CELL) = a;
L_ED49:;
    mff_cycles(0xED49, 10);
    RAM(TMP4) = x;
    y = RAM(ROUND_TABLES);
    a = prg_read((uint16_t)(0xC1F8 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xED50, 9);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0D54, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xED55, 12);
    x = RAM(TMP4);
    y = RAM(TMP19);
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0D5B, func_E66E, 0xE000)) return;
    SYNC_IN;
L_ED5C:;
    mff_cycles(0xED5C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D5E, func_E653, 0xE000)) return;
    SYNC_IN;
L_ED5F:;
    mff_cycles(0xED5F, 8);
    y = RAM(TMP3);
    a = mem_read((uint16_t)(RAM16(TMP0) + y));
    mff_cycles(0xED63, 6);
    y = a;
    a = prg_read((uint16_t)(0xBF00 + y));
    mff_cycles(0xED67, 15);
    a &= 0xF0;
    y = RAM(TMP2);
    RAM(PROBE_HITS + y) = a;
    if (a >= RAM(PROBE_MAX)) {
        mff_cycles(0xED72, 3);
        RAM(PROBE_MAX) = a;
    } else {
        mff_cycles(0xED70, 1);
    }
    mff_cycles(0xED74, 13);
    a |= RAM(TMP16);
    RAM(TMP16) = a;
    t = (uint8_t)(RAM(TMP2) - 1);
    RAM(TMP2) = t;
    if (t & 0x80) { mff_cycles(0xED7A, 1); goto L_EDB3; }
    mff_cycles(0xED7C, 29);
    t = (uint8_t)(RAM(PROBE_INDEX) + 1);
    RAM(PROBE_INDEX) = t;
    y = RAM(PROBE_INDEX);
    a = RAM(TMP18);
    mff_push(a);
    a &= 0x10;
    RAM(TMP4) = a;
    a = mff_pull();
    c = 0;
    m = prg_read((uint16_t)(0xEEF8 + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    mff_cycles(0xED8C, 10);
    RAM(TMP18) = a;
    a &= 0x10;
    m = RAM(TMP4);
    c = a >= m;
    if (a == m) { mff_cycles(0xED5F, 1); goto L_ED5F; }
    mff_cycles(0xED94, 12);
    a = RAM(TMP3);
    a ^= 0x01;
    RAM(TMP3) = a;
    a &= 0x01;
    if (a != 0) { mff_cycles(0xED5F, 1); goto L_ED5F; }
    mff_cycles(0xED9E, 12);
    t = (uint8_t)(RAM(MAP_CELL) + 1);
    RAM(MAP_CELL) = t;
    a = RAM(MAP_CELL);
    a &= 0x07;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xED5C, 1); goto L_ED5C; }
    mff_cycles(0xEDA6, 21);
    t = (uint8_t)(RAM(TMP19) + 1);
    RAM(TMP19) = t;
    t = (uint8_t)(RAM(MAP_CELL) - 1);
    RAM(MAP_CELL) = t;
    a = RAM(MAP_CELL);
    a &= 0x38;
    RAM(MAP_CELL) = a;
    goto L_ED49;
L_EDB3:;
    mff_cycles(0xEDB3, 4);
    m = 0x00;
    c = x >= m;
    n = ((uint8_t)(x - m)) >> 7;
    z = ((uint8_t)(x - m)) == 0;
    if (!z) {
        mff_cycles(0xEDB5, 1);
    }
    mff_cycles(0xEDB7, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xEEDF, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_EEDF(); g_code_window_base = swb; } return;
}

/*  $ED49 - lifted by port/tools/lift.py */
void func_ED49(void) {
    REGS_IN;
L_ED49:;
    mff_cycles(0xED49, 10);
    RAM(TMP4) = x;
    y = RAM(ROUND_TABLES);
    a = prg_read((uint16_t)(0xC1F8 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xED50, 9);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0D54, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xED55, 12);
    x = RAM(TMP4);
    y = RAM(TMP19);
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0D5B, func_E66E, 0xE000)) return;
    SYNC_IN;
L_ED5C:;
    mff_cycles(0xED5C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D5E, func_E653, 0xE000)) return;
    SYNC_IN;
L_ED5F:;
    mff_cycles(0xED5F, 8);
    y = RAM(TMP3);
    a = mem_read((uint16_t)(RAM16(TMP0) + y));
    mff_cycles(0xED63, 6);
    y = a;
    a = prg_read((uint16_t)(0xBF00 + y));
    mff_cycles(0xED67, 15);
    a &= 0xF0;
    y = RAM(TMP2);
    RAM(PROBE_HITS + y) = a;
    if (a >= RAM(PROBE_MAX)) {
        mff_cycles(0xED72, 3);
        RAM(PROBE_MAX) = a;
    } else {
        mff_cycles(0xED70, 1);
    }
    mff_cycles(0xED74, 13);
    a |= RAM(TMP16);
    RAM(TMP16) = a;
    t = (uint8_t)(RAM(TMP2) - 1);
    RAM(TMP2) = t;
    if (t & 0x80) { mff_cycles(0xED7A, 1); goto L_EDB3; }
    mff_cycles(0xED7C, 29);
    t = (uint8_t)(RAM(PROBE_INDEX) + 1);
    RAM(PROBE_INDEX) = t;
    y = RAM(PROBE_INDEX);
    a = RAM(TMP18);
    mff_push(a);
    a &= 0x10;
    RAM(TMP4) = a;
    a = mff_pull();
    c = 0;
    m = prg_read((uint16_t)(0xEEF8 + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    mff_cycles(0xED8C, 10);
    RAM(TMP18) = a;
    a &= 0x10;
    m = RAM(TMP4);
    c = a >= m;
    if (a == m) { mff_cycles(0xED5F, 1); goto L_ED5F; }
    mff_cycles(0xED94, 12);
    a = RAM(TMP3);
    a ^= 0x01;
    RAM(TMP3) = a;
    a &= 0x01;
    if (a != 0) { mff_cycles(0xED5F, 1); goto L_ED5F; }
    mff_cycles(0xED9E, 12);
    t = (uint8_t)(RAM(MAP_CELL) + 1);
    RAM(MAP_CELL) = t;
    a = RAM(MAP_CELL);
    a &= 0x07;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xED5C, 1); goto L_ED5C; }
    mff_cycles(0xEDA6, 21);
    t = (uint8_t)(RAM(TMP19) + 1);
    RAM(TMP19) = t;
    t = (uint8_t)(RAM(MAP_CELL) - 1);
    RAM(MAP_CELL) = t;
    a = RAM(MAP_CELL);
    a &= 0x38;
    RAM(MAP_CELL) = a;
    goto L_ED49;
L_EDB3:;
    mff_cycles(0xEDB3, 4);
    m = 0x00;
    c = x >= m;
    n = ((uint8_t)(x - m)) >> 7;
    z = ((uint8_t)(x - m)) == 0;
    if (!z) {
        mff_cycles(0xEDB5, 1);
    }
    mff_cycles(0xEDB7, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xEEDF, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_EEDF(); g_code_window_base = swb; } return;
}

/* bg_probe_2 $EDBA - lifted by port/tools/lift.py */
void func_EDBA(void) {
    REGS_IN;
    mff_cycles(0xEDBA, 4);
    a = prg_read((uint16_t)(0xEF23 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xEDBD, 9);
    RAM(PROBE_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0DC1, func_EEC4, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xEDC2, 6);
    y = a;
    a = prg_read((uint16_t)(0xEF2D + y));
    mff_cycles(0xEDC6, 12);
    RAM(TMP2) = a;
    a = 0x00;
    RAM(TMP4) = a;
    a = prg_read((uint16_t)(0xEF2E + y));
    mff_cycles(0xEDCF, 2);
    if (a & 0x80) {
        mff_cycles(0xEDD1, 5);
        t = (uint8_t)(RAM(TMP4) - 1);
        RAM(TMP4) = t;
    } else {
        mff_cycles(0xEDCF, 1);
    }
    mff_cycles(0xEDD3, 53);
    c = 0;
    m = RAM(OBJ_X + x);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(TMP18) = a;
    a = RAM(OBJ_X_HI + x);
    m = RAM(TMP4);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(TMP19) = a;
    a = RAM(TMP18);
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
    mff_push(a);
    a &= 0x01;
    RAM(TMP3) = a;
    a = mff_pull();
    t = a;
    t >>= 1;
    a = t;
    RAM(MAP_CELL) = a;
    a = RAM(OBJ_Y_HI + x);
    z = (a) == 0;
    if (a & 0x80) { mff_cycles(0xEDF2, 2); goto L_EE18; }
    mff_cycles(0xEDF4, 2);
    if (!z) { mff_cycles(0xEDF4, 2); goto L_EE12; }
    mff_cycles(0xEDF6, 10);
    a = RAM(OBJ_Y + x);
    c = 0;
    m = prg_read((uint16_t)(0xEF2F + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xEDFD, 7);
    RAM(TMP17) = a;
    a = prg_read((uint16_t)(0xEF2F + y));
    mff_cycles(0xEE02, 2);
    if (!(a & 0x80)) { mff_cycles(0xEE02, 1); goto L_EE08; }
    mff_cycles(0xEE04, 2);
    if (!c) { mff_cycles(0xEE04, 1); goto L_EE18; }
    mff_cycles(0xEE06, 2);
    if (c) { mff_cycles(0xEE06, 1); goto L_EE0A; }
L_EE08:;
    mff_cycles(0xEE08, 2);
    if (c) { mff_cycles(0xEE08, 1); goto L_EE12; }
L_EE0A:;
    mff_cycles(0xEE0A, 7);
    a = RAM(TMP17);
    m = 0xF0;
    c = a >= m;
    if (!c) { mff_cycles(0xEE0E, 1); goto L_EE43; }
    mff_cycles(0xEE10, 2);
    if (c) { mff_cycles(0xEE08, 1); goto L_EE08; }
L_EE12:;
    mff_cycles(0xEE12, 7);
    a = 0xEF;
    RAM(TMP17) = a;
    if (a != 0) { mff_cycles(0xEE16, 1); goto L_EE43; }
L_EE18:;
    mff_cycles(0xEE18, 7);
    a = 0x00;
    RAM(TMP17) = a;
    if (a == 0) { mff_cycles(0xEE1C, 1); goto L_EE43; }
L_EE1E:;
    mff_cycles(0xEE1E, 9);
    a = RAM(TMP17);
    c = 0;
    m = prg_read((uint16_t)(0xEF2F + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    mff_cycles(0xEE24, 7);
    RAM(TMP17) = a;
    m = 0xF0;
    c = a >= m;
    if (!c) { mff_cycles(0xEE28, 1); goto L_EE32; }
    mff_cycles(0xEE2A, 12);
    m = 0x10;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(TMP17) = a;
    t = (uint8_t)(RAM(TMP4) + 1);
    RAM(TMP4) = t;
    if (t == 0) { mff_cycles(0xEE30, 1); goto L_EE43; }
L_EE32:;
    mff_cycles(0xEE32, 22);
    y++;
    RAM(PROBE_INDEX) = y;
    y = RAM(TMP2);
    a = 0x00;
    RAM(PROBE_HITS + y) = a;
    t = (uint8_t)(RAM(TMP2) - 1);
    RAM(TMP2) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!n) { mff_cycles(0xEE1E, 1); goto L_EE1E; }
    mff_cycles(0xEE40, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xEEDF, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_EEDF(); g_code_window_base = swb; } return;
L_EE43:;
    mff_cycles(0xEE43, 42);
    a = RAM(TMP17);
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    mff_push(a);
    a &= 0x38;
    a |= RAM(MAP_CELL);
    RAM(MAP_CELL) = a;
    a = mff_pull();
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    a &= 0x02;
    a |= RAM(TMP3);
    RAM(TMP3) = a;
    RAM(TMP4) = x;
    y = RAM(ROUND_TABLES);
    a = prg_read((uint16_t)(0xC1F8 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xEE5D, 9);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0E61, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xEE62, 12);
    x = RAM(TMP4);
    y = RAM(TMP19);
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0E68, func_E66E, 0xE000)) return;
    SYNC_IN;
L_EE69:;
    mff_cycles(0xEE69, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0E6B, func_E653, 0xE000)) return;
    SYNC_IN;
L_EE6C:;
    mff_cycles(0xEE6C, 8);
    y = RAM(TMP3);
    a = mem_read((uint16_t)(RAM16(TMP0) + y));
    mff_cycles(0xEE70, 6);
    y = a;
    a = prg_read((uint16_t)(0xBF00 + y));
    mff_cycles(0xEE74, 15);
    a &= 0xF0;
    y = RAM(TMP2);
    RAM(PROBE_HITS + y) = a;
    if (a >= RAM(PROBE_MAX)) {
        mff_cycles(0xEE7F, 3);
        RAM(PROBE_MAX) = a;
    } else {
        mff_cycles(0xEE7D, 1);
    }
    mff_cycles(0xEE81, 13);
    a |= RAM(TMP16);
    RAM(TMP16) = a;
    t = (uint8_t)(RAM(TMP2) - 1);
    RAM(TMP2) = t;
    if (t & 0x80) { mff_cycles(0xEE87, 1); goto L_EEBD; }
    mff_cycles(0xEE89, 29);
    t = (uint8_t)(RAM(PROBE_INDEX) + 1);
    RAM(PROBE_INDEX) = t;
    y = RAM(PROBE_INDEX);
    a = RAM(TMP17);
    mff_push(a);
    a &= 0x10;
    RAM(TMP4) = a;
    a = mff_pull();
    c = 0;
    m = prg_read((uint16_t)(0xEF2F + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    mff_cycles(0xEE99, 10);
    RAM(TMP17) = a;
    a &= 0x10;
    if (a == RAM(TMP4)) { mff_cycles(0xEE6C, 1); goto L_EE6C; }
    mff_cycles(0xEEA1, 12);
    a = RAM(TMP3);
    a ^= 0x02;
    RAM(TMP3) = a;
    a &= 0x02;
    if (a != 0) { mff_cycles(0xEE6C, 1); goto L_EE6C; }
    mff_cycles(0xEEAB, 21);
    a = RAM(MAP_CELL);
    mff_push(a);
    c = 0;
    m = 0x08;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(MAP_CELL) = a;
    m = 0x40;
    c = a >= m;
    a = mff_pull();
    n = (a) >> 7;
    z = (a) == 0;
    if (!c) { mff_cycles(0xEE69, 1); goto L_EE69; }
    mff_cycles(0xEEB8, 6);
    RAM(MAP_CELL) = a;
    goto L_EE6C;
L_EEBD:;
    mff_cycles(0xEEBD, 4);
    m = 0x00;
    c = x >= m;
    n = ((uint8_t)(x - m)) >> 7;
    z = ((uint8_t)(x - m)) == 0;
    if (!z) {
        mff_cycles(0xEEBF, 1);
    }
    mff_cycles(0xEEC1, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xEEDF, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_EEDF(); g_code_window_base = swb; } return;
}

/*  $EEC4 - lifted by port/tools/lift.py */
void func_EEC4(void) {
    REGS_IN;
    mff_cycles(0xEEC4, 29);
    mff_push(a);
    a = x;
    mff_push(a);
    a = 0x00;
    RAM(TMP16) = a;
    RAM(PROBE_MAX) = a;
    a = RAM(BANK_REQUEST + 1);
    RAM(PROBE_SAVED_BANK) = a;
    y = RAM(ROUND_TABLES);
    a = prg_read((uint16_t)(0xC1F8 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xEED6, 9);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0EDA, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xEEDB, 16);
    a = mff_pull();
    x = a;
    a = mff_pull();
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $EEDF - lifted by port/tools/lift.py */
void func_EEDF(void) {
    REGS_IN;
    mff_cycles(0xEEDF, 17);
    a = x;
    mff_push(a);
    a = RAM(PROBE_SAVED_BANK);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0EE7, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xEEE8, 12);
    a = mff_pull();
    x = a;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $EF55 - lifted by port/tools/lift.py */
void func_EF55(void) {
    REGS_IN;
    mff_cycles(0xEF55, 39);
    a = RAM(TMP18);
    a &= 0x0F;
    RAM(TMP18) = a;
    a = RAM(OBJ_X + x);
    c = 1;
    m = RAM(TMP18);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_X + x) = a;
    a = RAM(OBJ_X_HI + x);
    m = 0x00;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X_HI + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $EF6D - lifted by port/tools/lift.py */
void func_EF6D(void) {
    REGS_IN;
    mff_cycles(0xEF6D, 35);
    a = RAM(TMP18);
    a &= 0x0F;
    a ^= 0x0F;
    c = 1;
    m = RAM(OBJ_X + x);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_X + x) = a;
    a = RAM(OBJ_X_HI + x);
    m = 0x00;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X_HI + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $EF83 - lifted by port/tools/lift.py */
void func_EF83(void) {
    REGS_IN;
    mff_cycles(0xEF83, 22);
    a = RAM(TMP17);
    a &= 0x0F;
    a ^= 0x0F;
    c = 1;
    m = RAM(OBJ_Y + x);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_Y + x) = a;
    m = 0xF0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0xEF94, 14);
        m = 0x0F;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(OBJ_Y + x) = a;
        t = (uint8_t)(RAM(OBJ_Y_HI + x) + 1);
        RAM(OBJ_Y_HI + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
    } else {
        mff_cycles(0xEF92, 1);
    }
    mff_cycles(0xEF9C, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $EF9D - lifted by port/tools/lift.py */
void func_EF9D(void) {
    REGS_IN;
    mff_cycles(0xEF9D, 27);
    a = RAM(TMP17);
    mff_push(a);
    a &= 0x0F;
    RAM(TMP17) = a;
    a = RAM(OBJ_Y + x);
    c = 1;
    m = RAM(TMP17);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_Y + x) = a;
    if (!c) {
        mff_cycles(0xEFAF, 14);
        m = 0x0F;
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        RAM(OBJ_Y + x) = a;
        t = (uint8_t)(RAM(OBJ_Y_HI + x) - 1);
        RAM(OBJ_Y_HI + x) = t;
    } else {
        mff_cycles(0xEFAD, 1);
    }
    mff_cycles(0xEFB7, 13);
    a = mff_pull();
    n = (a) >> 7;
    z = (a) == 0;
    RAM(TMP17) = a;
    SYNC_OUT;
    mff_rts(); return;
}
