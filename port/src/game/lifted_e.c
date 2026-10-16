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
