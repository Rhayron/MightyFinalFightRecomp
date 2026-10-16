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
