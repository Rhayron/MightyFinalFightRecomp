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

/*  $9B18 (bank 2) - lifted by port/tools/lift.py */
void func_9B18_b2(void) {
    REGS_IN;
    mff_cycles(0x9B18, 17);
    a = RAM(PLAYER_LEVEL);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    RAM(TMP8) = a;
    y = RAM(PLAYER_ID);
    if (y == 0) { mff_cycles(0x9B21, 1); goto L_9B2D; }
    mff_cycles(0x9B23, 6);
    y--;
    y--;
    if (y == 0) { mff_cycles(0x9B25, 1); goto L_9B33; }
    mff_cycles(0x9B27, 6);
    a = 0x30;
    x = 0x06;
    if (x != 0) { mff_cycles(0x9B2B, 1); goto L_9B37; }
L_9B2D:;
    mff_cycles(0x9B2D, 6);
    a = 0x40;
    x = 0x08;
    if (x != 0) { mff_cycles(0x9B31, 1); goto L_9B37; }
L_9B33:;
    mff_cycles(0x9B33, 4);
    a = 0x50;
    x = 0x0A;
L_9B37:;
    mff_cycles(0x9B37, 16);
    c = 0;
    m = RAM(TMP8);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(HEALTH) = a;
    a = 0x2E;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1B40, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9B41, 63);
    a = x;
    c = 0;
    m = RAM(PLAYER_LEVEL);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(TMP14) = a;
    y = a;
    x = RAM(VRAM_BUFFER_LEN);
    RAM(VRAM_BUFFER + x) = a;
    t = (uint8_t)(RAM(VRAM_BUFFER + x) + 1);
    RAM(VRAM_BUFFER + x) = t;
    t = (uint8_t)(RAM(VRAM_BUFFER + x) + 1);
    RAM(VRAM_BUFFER + x) = t;
    x++;
    a = 0x20;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x49;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x19;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    for (;;) {
        mff_cycles(0x9B66, 13);
        a = 0x18;
        RAM(VRAM_BUFFER + x) = a;
        x++;
        y--;
        if (y == 0) break;
        mff_cycles(0x9B66, 1);
    }
    mff_cycles(0x9B6F, 28);
    a = 0x1A;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    RAM(VRAM_BUFFER_LEN) = x;
    a = 0xFF;
    RAM(VRAM_REQUEST) = a;
    t = (uint8_t)(RAM(TEXT_CURSOR) + 1);
    RAM(TEXT_CURSOR) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9B7E (bank 2) - lifted by port/tools/lift.py */
void func_9B7E_b2(void) {
    REGS_IN;
    mff_cycles(0x9B7E, 56);
    t = (uint8_t)(RAM(LIVES) + 1);
    RAM(LIVES) = t;
    x = RAM(VRAM_BUFFER_LEN);
    a = 0x01;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x20;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x7C;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = 0x02;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    RAM(VRAM_BUFFER_LEN) = x;
    a = 0xFF;
    RAM(VRAM_REQUEST) = a;
    a = 0x2D;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0x9BA4, 7);
        t = (uint8_t)(RAM(CREDITS) + 1);
        RAM(CREDITS) = t;
        a = 0x30;
        n = (a) >> 7;
        z = (a) == 0;
    } else {
        mff_cycles(0x9BA2, 1);
    }
    mff_cycles(0x9BA8, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1BAA, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9BAB, 11);
    t = (uint8_t)(RAM(TEXT_CURSOR) + 1);
    RAM(TEXT_CURSOR) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9BA4 (bank 2) - lifted by port/tools/lift.py */
void func_9BA4_b2(void) {
    REGS_IN;
    mff_cycles(0x9BA4, 13);
    t = (uint8_t)(RAM(CREDITS) + 1);
    RAM(CREDITS) = t;
    a = 0x30;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1BAA, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9BAB, 11);
    t = (uint8_t)(RAM(TEXT_CURSOR) + 1);
    RAM(TEXT_CURSOR) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A724 (bank 2) - lifted by port/tools/lift.py */
void func_A724_b2(void) {
    REGS_IN;
    mff_cycles(0xA724, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA8A8, 2); call_by_address_tail(0xA8A8, 2); return;
}

/*  $A82A (bank 2) - lifted by port/tools/lift.py */
void func_A82A_b2(void) {
    REGS_IN;
    mff_cycles(0xA82A, 11);
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(WEAPON_HELD) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0830, func_F2BE, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA831, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A851 (bank 2) - lifted by port/tools/lift.py */
void func_A851_b2(void) {
    REGS_IN;
    mff_cycles(0xA851, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0853, func_F71A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA854, 2);
    if (c) { mff_cycles(0xA854, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0850), 2); return; }
    mff_cycles(0xA856, 58);
    RAM(TMP15) = y;
    a = RAM(OBJ_DIR + x);
    RAM(OBJ_DIR + y) = a;
    a = RAM(SCROLL_X_HI);
    RAM(OBJ_X_HI + y) = a;
    a = RAM(OBJ_Y_HI);
    RAM(OBJ_Y_HI + y) = a;
    a = RAM(OBJ_X + x);
    RAM(OBJ_X + y) = a;
    a = RAM(OBJ_Y + x);
    RAM(OBJ_Y + y) = a;
    a = RAM(PLAYER_ID);
    a &= 0x01;
    y = a;
    a = prg_read((uint16_t)(0xA8A5 + y));
    mff_cycles(0xA87D, 9);
    y = RAM(TMP15);
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0881, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA882, 57);
    a = 0x6A;
    RAM(OBJ_ID + y) = a;
    RAM(OBJ_SAVED_ID + y) = a;
    a = 0x80;
    RAM(OBJ_STATE + y) = a;
    RAM(OBJ_SAVED_STATE + y) = a;
    RAM(OBJ_SPAWN_INDEX + y) = a;
    a = 0x00;
    RAM(OBJ_THROW_FLAG + y) = a;
    a = 0x2F;
    RAM(OBJ_ATTACK + y) = a;
    a = RAM(WEAPON_AMMO);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $80E3 (bank 4) - lifted by port/tools/lift.py */
void func_80E3_b4(void) {
    REGS_IN;
    mff_cycles(0x80E3, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8956 (bank 4) - lifted by port/tools/lift.py */
void func_8956_b4(void) {
    REGS_IN;
    mff_cycles(0x8956, 5);
    a = RAM(AREA_CLEAR);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0x8958, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0955), 4); return; }
    mff_cycles(0x895A, 8);
    a = RAM(AREA_SCREEN);
    m = RAM(AREA_SCREEN_COUNT);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x895E, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x093C), 4); return; }
    mff_cycles(0x8960, 5);
    a = RAM(GAME_ACTION);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x8962, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0955), 4); return; }
    mff_cycles(0x8964, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0966, 0x96FC, 4)) return;
    SYNC_IN;
    mff_cycles(0x8967, 2);
    if (!z) { mff_cycles(0x8967, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0955), 4); return; }
    mff_cycles(0x8969, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x096B, func_F71A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x896C, 2);
    if (c) { mff_cycles(0x896C, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x092C), 4); return; }
    mff_cycles(0x896E, 25);
    a = RAM(SCROLL_X_HI);
    RAM(OBJ_X_HI + y) = a;
    a = RAM(OBJ_Y_HI);
    RAM(OBJ_Y_HI + y) = a;
    a = 0x6D;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x097D, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x897E, 123);
    a = 0x90;
    RAM(OBJ_FLAGS + y) = a;
    a = 0x23;
    RAM(CHR_BANKS + 4) = a;
    a = 0x6F;
    RAM(OBJ_ID + y) = a;
    RAM(OBJ_SAVED_ID + y) = a;
    a = 0x00;
    RAM(OBJ_LIFE + y) = a;
    RAM(OBJ_LIFE_MAX + y) = a;
    RAM(OBJ_THROW_FLAG + y) = a;
    RAM(OBJ_HIT_COOLDOWN + y) = a;
    RAM(OBJ_INVULN + y) = a;
    RAM(OBJ_ATTACK_DAMAGE + y) = a;
    RAM(OBJ_ATTACK + y) = a;
    RAM(OBJ_HIT_PLAYER + y) = a;
    RAM(OBJ_HIT_TIMER + y) = a;
    RAM(OBJ_HELD + y) = a;
    RAM(OBJ_KIND + y) = a;
    RAM(OBJ_TIER + y) = a;
    RAM(OBJ_AI_TIMER + y) = a;
    RAM(OBJ_AI_COUNT + y) = a;
    RAM(OBJ_AI_FLAG + y) = a;
    a = 0x40;
    RAM(OBJ_Y + y) = a;
    a = 0xC0;
    RAM(OBJ_X + y) = a;
    a = 0x06;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x09CC, 0xA013, 4)) return;
    SYNC_IN;
    mff_cycles(0x89CD, 11);
    a = 0x7F;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(AREA_CLEAR) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8BD4 (bank 4) - lifted by port/tools/lift.py */
void func_8BD4_b4(void) {
    REGS_IN;
    mff_cycles(0x8BD4, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DD9 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8BD9, 2);
    if (!c) { mff_cycles(0x8BD9, 1); goto L_8BE4; }
    mff_cycles(0x8BDB, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0BDD, 0x96FC, 4)) return;
    SYNC_IN;
    mff_cycles(0x8BDE, 2);
    if (!z) { mff_cycles(0x8BDE, 1); goto L_8BE7; }
    mff_cycles(0x8BE0, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0BE2, 0x89D2, 4)) return;
    SYNC_IN;
    mff_cycles(0x8BE3, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8BE4:;
    mff_cycles(0x8BE4, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0BE6, 0x9000, 4)) return;
    SYNC_IN;
L_8BE7:;
    mff_cycles(0x8BE7, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9011 (bank 4) - lifted by port/tools/lift.py */
void func_9011_b4(void) {
    REGS_IN;
    mff_cycles(0x9011, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $97AD (bank 4) - lifted by port/tools/lift.py */
void func_97AD_b4(void) {
    REGS_IN;
    mff_cycles(0x97AD, 4);
    a = prg_read((uint16_t)(0x99C6 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x97B0, 6);
    SYNC_OUT;
    if (!mff_call_at(0x17B2, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x97B3, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $97F3 (bank 4) - lifted by port/tools/lift.py */
void func_97F3_b4(void) {
    REGS_IN;
    mff_cycles(0x97F3, 4);
    a = prg_read((uint16_t)(0x99CC + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x97F6, 6);
    SYNC_OUT;
    if (!mff_call_at(0x17F8, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x97F9, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9828 (bank 4) - lifted by port/tools/lift.py */
void func_9828_b4(void) {
    REGS_IN;
    mff_cycles(0x9828, 4);
    a = prg_read((uint16_t)(0x99D0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x982B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x182D, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x982E, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9867 (bank 4) - lifted by port/tools/lift.py */
void func_9867_b4(void) {
    REGS_IN;
    mff_cycles(0x9867, 4);
    a = prg_read((uint16_t)(0x99D7 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x986A, 6);
    SYNC_OUT;
    if (!mff_call_at(0x186C, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x986D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $986E (bank 4) - lifted by port/tools/lift.py */
void func_986E_b4(void) {
    REGS_IN;
    mff_cycles(0x986E, 4);
    a = prg_read((uint16_t)(0x99D7 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9871, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1873, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x9874, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $989A (bank 4) - lifted by port/tools/lift.py */
void func_989A_b4(void) {
    REGS_IN;
    mff_cycles(0x989A, 4);
    a = prg_read((uint16_t)(0x99DB + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x989D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x189F, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x98A0, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $99BB (bank 4) - lifted by port/tools/lift.py */
void func_99BB_b4(void) {
    REGS_IN;
    mff_cycles(0x99BB, 4);
    a = prg_read((uint16_t)(0x99F4 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x99BE, 6);
    SYNC_OUT;
    if (!mff_call_at(0x19C0, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x99C1, 6);
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

/*  $805E (bank 6) - lifted by port/tools/lift.py */
void func_805E_b6(void) {
    REGS_IN;
    mff_cycles(0x805E, 12);
    t = (uint8_t)(RAM(UPDATE_SLOT) + 1);
    RAM(UPDATE_SLOT) = t;
    x = RAM(UPDATE_SLOT);
    m = 0x10;
    c = x >= m;
    n = ((uint8_t)(x - m)) >> 7;
    z = ((uint8_t)(x - m)) == 0;
    if (z) {
        mff_cycles(0x8066, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x8064, 1);
    }
    mff_cycles(0x8067, 6);
    a = RAM(OBJ_HIT_COOLDOWN + x);
    if (a != 0) {
        mff_cycles(0x806C, 10);
        t = (uint8_t)(RAM(OBJ_HIT_COOLDOWN + x) - 1);
        RAM(OBJ_HIT_COOLDOWN + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x81F0, 2); call_by_address_tail(0x81F0, 6); return;
    } else {
        mff_cycles(0x806A, 1);
    }
    mff_cycles(0x8072, 8);
    a = RAM(OBJ_ID + x);
    m = 0x04;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8077, 1); goto L_808D; }
    mff_cycles(0x8079, 4);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x807B, 1); goto L_808D; }
    mff_cycles(0x807D, 4);
    m = 0x0F;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x807F, 1); goto L_808D; }
    mff_cycles(0x8081, 4);
    m = 0x0E;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8083, 1); goto L_8090; }
    mff_cycles(0x8085, 7);
    a = RAM(ATTACK_ID);
    m = 0x09;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8089, 1); goto L_808D; }
    mff_cycles(0x808B, 2);
    if (!z) { mff_cycles(0x808B, 1); goto L_8090; }
L_808D:;
    mff_cycles(0x808D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x008F, 0x8244, 6)) return;
    SYNC_IN;
L_8090:;
    mff_cycles(0x8090, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0092, func_F9E5, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8093, 2);
    if (c) { mff_cycles(0x8093, 1); goto L_80C0; }
    mff_cycles(0x8095, 11);
    y = RAM(TMP15);
    a = RAM(OBJ_ANIM + y);
    m = 0x74;
    c = a >= m;
    if (a == m) { mff_cycles(0x809C, 1); goto L_80AF; }
    mff_cycles(0x809E, 22);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + y) = a;
    RAM(OBJ_SAVED_STATE + y) = a;
    RAM(OBJ_FLAGS + y) = a;
    RAM(OBJ_ID + y) = a;
L_80AC:;
    mff_cycles(0x80AC, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x80D3, 2); call_by_address_tail(0x80D3, 6); return;
L_80AF:;
    mff_cycles(0x80AF, 23);
    a = 0x00;
    RAM(OBJ_ANIM_TIMER + y) = a;
    a = 0x06;
    RAM(OBJ_ANIM_FRAME + y) = a;
    a = 0x09;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ID + y) = a;
    if (!z) { mff_cycles(0x80AC, 1); goto L_80AC; }
L_80C0:;
    mff_cycles(0x80C0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x00C2, func_F738, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x80C3, 2);
    if (c) { mff_cycles(0x80C3, 1); goto L_80CD; }
    mff_cycles(0x80C5, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x00C7, func_F455, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x80C8, 6);
    a = RAM(OBJ_LIFE + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0x80CB, 1); goto L_80D3; }
L_80CD:;
    mff_cycles(0x80CD, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x81F0, 2); call_by_address_tail(0x81F0, 6); return;
L_80D3:;
    mff_cycles(0x80D3, 6);
    SYNC_OUT;
    if (!mff_call_at(0x00D5, 0x84F0, 6)) return;
    SYNC_IN;
    mff_cycles(0x80D6, 15);
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_ANIM + x);
    m = 0x84;
    c = a >= m;
    if (a == m) { mff_cycles(0x80E0, 2); goto L_8108; }
    mff_cycles(0x80E2, 4);
    m = 0xAC;
    c = a >= m;
    if (a == m) { mff_cycles(0x80E4, 2); goto L_8108; }
    mff_cycles(0x80E6, 4);
    m = 0xE7;
    c = a >= m;
    if (a == m) { mff_cycles(0x80E8, 2); goto L_8108; }
    mff_cycles(0x80EA, 4);
    m = 0xBE;
    c = a >= m;
    if (a == m) { mff_cycles(0x80EC, 2); goto L_8108; }
    mff_cycles(0x80EE, 4);
    m = 0xCC;
    c = a >= m;
    if (a == m) { mff_cycles(0x80F0, 2); goto L_8108; }
    mff_cycles(0x80F2, 8);
    a = RAM(OBJ_KIND + x);
    if (a == 0x06) { mff_cycles(0x80F7, 1); goto L_80FD; }
    mff_cycles(0x80F9, 4);
    if (a != 0x0C) { mff_cycles(0x80FB, 2); goto L_8116; }
L_80FD:;
    mff_cycles(0x80FD, 15);
    a = RAM(OBJ_Y + x);
    c = 0;
    m = 0x0A;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_LAND_Y + x) = a;
    if (a != 0) { mff_cycles(0x8106, 1); goto L_8131; }
L_8108:;
    mff_cycles(0x8108, 15);
    a = RAM(OBJ_GROUND_Y + x);
    RAM(OBJ_LAND_Y + x) = a;
    a = RAM(OBJ_LIFE + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) { mff_cycles(0x8111, 2); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x00D0), 6); return; }
    mff_cycles(0x8113, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x819A, 2); call_by_address_tail(0x819A, 6); return;
L_8116:;
    mff_cycles(0x8116, 8);
    a = RAM(OBJ_ID + x);
    m = 0x91;
    c = a >= m;
    if (a == m) { mff_cycles(0x811B, 1); goto L_8131; }
    mff_cycles(0x811D, 7);
    a = RAM(ATTACK_ID);
    m = 0x1D;
    c = a >= m;
    if (a != m) { mff_cycles(0x8121, 1); goto L_812B; }
    mff_cycles(0x8123, 11);
    a = RAM(OBJ_Y);
    RAM(OBJ_LAND_Y + x) = a;
    if (a != 0) { mff_cycles(0x8129, 1); goto L_8131; }
L_812B:;
    mff_cycles(0x812B, 9);
    a = RAM(OBJ_Y + x);
    RAM(OBJ_LAND_Y + x) = a;
L_8131:;
    mff_cycles(0x8131, 6);
    a = RAM(OBJ_LIFE + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) { mff_cycles(0x8134, 2); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x00D0), 6); return; }
    mff_cycles(0x8136, 13);
    a = 0x1E;
    RAM(COMBO_TIMER) = a;
    a = RAM(OBJ_KIND + x);
    m = 0x03;
    c = a >= m;
    if (a != m) { mff_cycles(0x813F, 1); goto L_8146; }
    mff_cycles(0x8141, 6);
    a = RAM(OBJ_HIT_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x8144, 1); goto L_81B5; }
L_8146:;
    mff_cycles(0x8146, 7);
    a = RAM(ATTACK_ID);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0x814A, 1); goto L_81AB; }
    mff_cycles(0x814C, 4);
    m = 0x0D;
    c = a >= m;
    if (a == m) { mff_cycles(0x814E, 1); goto L_81AB; }
    mff_cycles(0x8150, 4);
    m = 0x02;
    c = a >= m;
    if (a == m) { mff_cycles(0x8152, 1); goto L_819A; }
    mff_cycles(0x8154, 4);
    m = 0x03;
    c = a >= m;
    if (a == m) { mff_cycles(0x8156, 1); goto L_819A; }
    mff_cycles(0x8158, 4);
    m = 0x08;
    c = a >= m;
    if (a == m) { mff_cycles(0x815A, 1); goto L_819A; }
    mff_cycles(0x815C, 4);
    m = 0x0E;
    c = a >= m;
    if (a == m) { mff_cycles(0x815E, 1); goto L_819A; }
    mff_cycles(0x8160, 4);
    m = 0x07;
    c = a >= m;
    if (a == m) { mff_cycles(0x8162, 1); goto L_81AB; }
    mff_cycles(0x8164, 4);
    m = 0x0A;
    c = a >= m;
    if (a == m) { mff_cycles(0x8166, 1); goto L_819A; }
    mff_cycles(0x8168, 4);
    m = 0x0F;
    c = a >= m;
    if (a == m) { mff_cycles(0x816A, 1); goto L_819A; }
    mff_cycles(0x816C, 4);
    m = 0x10;
    c = a >= m;
    if (a == m) { mff_cycles(0x816E, 1); goto L_819A; }
    mff_cycles(0x8170, 4);
    m = 0x11;
    c = a >= m;
    if (a == m) { mff_cycles(0x8172, 1); goto L_819A; }
    mff_cycles(0x8174, 4);
    m = 0x12;
    c = a >= m;
    if (a == m) { mff_cycles(0x8176, 1); goto L_819A; }
    mff_cycles(0x8178, 4);
    m = 0x13;
    c = a >= m;
    if (a == m) { mff_cycles(0x817A, 1); goto L_819A; }
    mff_cycles(0x817C, 4);
    m = 0x14;
    c = a >= m;
    if (a == m) { mff_cycles(0x817E, 1); goto L_819A; }
    mff_cycles(0x8180, 4);
    m = 0x15;
    c = a >= m;
    if (a == m) { mff_cycles(0x8182, 1); goto L_819A; }
    mff_cycles(0x8184, 4);
    m = 0x16;
    c = a >= m;
    if (a == m) { mff_cycles(0x8186, 1); goto L_819A; }
    mff_cycles(0x8188, 4);
    m = 0x17;
    c = a >= m;
    if (a == m) { mff_cycles(0x818A, 1); goto L_819A; }
    mff_cycles(0x818C, 4);
    m = 0x18;
    c = a >= m;
    if (a == m) { mff_cycles(0x818E, 1); goto L_81AB; }
    mff_cycles(0x8190, 4);
    m = 0x1D;
    c = a >= m;
    if (a == m) { mff_cycles(0x8192, 1); goto L_819A; }
    mff_cycles(0x8194, 4);
    m = 0x22;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8196, 1); goto L_819A; }
    mff_cycles(0x8198, 2);
    if (!z) { mff_cycles(0x8198, 1); goto L_81B5; }
L_819A:;
    mff_cycles(0x819A, 23);
    a = 0x05;
    RAM(OBJ_ID + x) = a;
    a = 0x8E;
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x03;
    RAM(OBJ_VY + x) = a;
    if (a != 0) { mff_cycles(0x81A9, 1); goto L_81B0; }
L_81AB:;
    mff_cycles(0x81AB, 7);
    a = 0x02;
    RAM(OBJ_ID + x) = a;
L_81B0:;
    mff_cycles(0x81B0, 7);
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
L_81B5:;
    mff_cycles(0x81B5, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x800B, 2); call_by_address_tail(0x800B, 6); return;
}

/*  $80D3 (bank 6) - lifted by port/tools/lift.py */
void func_80D3_b6(void) {
    REGS_IN;
    mff_cycles(0x80D3, 6);
    SYNC_OUT;
    if (!mff_call_at(0x00D5, 0x84F0, 6)) return;
    SYNC_IN;
    mff_cycles(0x80D6, 15);
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_ANIM + x);
    m = 0x84;
    c = a >= m;
    if (a == m) { mff_cycles(0x80E0, 2); goto L_8108; }
    mff_cycles(0x80E2, 4);
    m = 0xAC;
    c = a >= m;
    if (a == m) { mff_cycles(0x80E4, 2); goto L_8108; }
    mff_cycles(0x80E6, 4);
    m = 0xE7;
    c = a >= m;
    if (a == m) { mff_cycles(0x80E8, 2); goto L_8108; }
    mff_cycles(0x80EA, 4);
    m = 0xBE;
    c = a >= m;
    if (a == m) { mff_cycles(0x80EC, 2); goto L_8108; }
    mff_cycles(0x80EE, 4);
    m = 0xCC;
    c = a >= m;
    if (a == m) { mff_cycles(0x80F0, 2); goto L_8108; }
    mff_cycles(0x80F2, 8);
    a = RAM(OBJ_KIND + x);
    if (a == 0x06) { mff_cycles(0x80F7, 1); goto L_80FD; }
    mff_cycles(0x80F9, 4);
    if (a != 0x0C) { mff_cycles(0x80FB, 2); goto L_8116; }
L_80FD:;
    mff_cycles(0x80FD, 15);
    a = RAM(OBJ_Y + x);
    c = 0;
    m = 0x0A;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_LAND_Y + x) = a;
    if (a != 0) { mff_cycles(0x8106, 1); goto L_8131; }
L_8108:;
    mff_cycles(0x8108, 15);
    a = RAM(OBJ_GROUND_Y + x);
    RAM(OBJ_LAND_Y + x) = a;
    a = RAM(OBJ_LIFE + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) { mff_cycles(0x8111, 2); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x00D0), 6); return; }
    mff_cycles(0x8113, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x819A, 2); call_by_address_tail(0x819A, 6); return;
L_8116:;
    mff_cycles(0x8116, 8);
    a = RAM(OBJ_ID + x);
    m = 0x91;
    c = a >= m;
    if (a == m) { mff_cycles(0x811B, 1); goto L_8131; }
    mff_cycles(0x811D, 7);
    a = RAM(ATTACK_ID);
    m = 0x1D;
    c = a >= m;
    if (a != m) { mff_cycles(0x8121, 1); goto L_812B; }
    mff_cycles(0x8123, 11);
    a = RAM(OBJ_Y);
    RAM(OBJ_LAND_Y + x) = a;
    if (a != 0) { mff_cycles(0x8129, 1); goto L_8131; }
L_812B:;
    mff_cycles(0x812B, 9);
    a = RAM(OBJ_Y + x);
    RAM(OBJ_LAND_Y + x) = a;
L_8131:;
    mff_cycles(0x8131, 6);
    a = RAM(OBJ_LIFE + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) { mff_cycles(0x8134, 2); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x00D0), 6); return; }
    mff_cycles(0x8136, 13);
    a = 0x1E;
    RAM(COMBO_TIMER) = a;
    a = RAM(OBJ_KIND + x);
    m = 0x03;
    c = a >= m;
    if (a != m) { mff_cycles(0x813F, 1); goto L_8146; }
    mff_cycles(0x8141, 6);
    a = RAM(OBJ_HIT_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x8144, 1); goto L_81B5; }
L_8146:;
    mff_cycles(0x8146, 7);
    a = RAM(ATTACK_ID);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0x814A, 1); goto L_81AB; }
    mff_cycles(0x814C, 4);
    m = 0x0D;
    c = a >= m;
    if (a == m) { mff_cycles(0x814E, 1); goto L_81AB; }
    mff_cycles(0x8150, 4);
    m = 0x02;
    c = a >= m;
    if (a == m) { mff_cycles(0x8152, 1); goto L_819A; }
    mff_cycles(0x8154, 4);
    m = 0x03;
    c = a >= m;
    if (a == m) { mff_cycles(0x8156, 1); goto L_819A; }
    mff_cycles(0x8158, 4);
    m = 0x08;
    c = a >= m;
    if (a == m) { mff_cycles(0x815A, 1); goto L_819A; }
    mff_cycles(0x815C, 4);
    m = 0x0E;
    c = a >= m;
    if (a == m) { mff_cycles(0x815E, 1); goto L_819A; }
    mff_cycles(0x8160, 4);
    m = 0x07;
    c = a >= m;
    if (a == m) { mff_cycles(0x8162, 1); goto L_81AB; }
    mff_cycles(0x8164, 4);
    m = 0x0A;
    c = a >= m;
    if (a == m) { mff_cycles(0x8166, 1); goto L_819A; }
    mff_cycles(0x8168, 4);
    m = 0x0F;
    c = a >= m;
    if (a == m) { mff_cycles(0x816A, 1); goto L_819A; }
    mff_cycles(0x816C, 4);
    m = 0x10;
    c = a >= m;
    if (a == m) { mff_cycles(0x816E, 1); goto L_819A; }
    mff_cycles(0x8170, 4);
    m = 0x11;
    c = a >= m;
    if (a == m) { mff_cycles(0x8172, 1); goto L_819A; }
    mff_cycles(0x8174, 4);
    m = 0x12;
    c = a >= m;
    if (a == m) { mff_cycles(0x8176, 1); goto L_819A; }
    mff_cycles(0x8178, 4);
    m = 0x13;
    c = a >= m;
    if (a == m) { mff_cycles(0x817A, 1); goto L_819A; }
    mff_cycles(0x817C, 4);
    m = 0x14;
    c = a >= m;
    if (a == m) { mff_cycles(0x817E, 1); goto L_819A; }
    mff_cycles(0x8180, 4);
    m = 0x15;
    c = a >= m;
    if (a == m) { mff_cycles(0x8182, 1); goto L_819A; }
    mff_cycles(0x8184, 4);
    m = 0x16;
    c = a >= m;
    if (a == m) { mff_cycles(0x8186, 1); goto L_819A; }
    mff_cycles(0x8188, 4);
    m = 0x17;
    c = a >= m;
    if (a == m) { mff_cycles(0x818A, 1); goto L_819A; }
    mff_cycles(0x818C, 4);
    m = 0x18;
    c = a >= m;
    if (a == m) { mff_cycles(0x818E, 1); goto L_81AB; }
    mff_cycles(0x8190, 4);
    m = 0x1D;
    c = a >= m;
    if (a == m) { mff_cycles(0x8192, 1); goto L_819A; }
    mff_cycles(0x8194, 4);
    m = 0x22;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8196, 1); goto L_819A; }
    mff_cycles(0x8198, 2);
    if (!z) { mff_cycles(0x8198, 1); goto L_81B5; }
L_819A:;
    mff_cycles(0x819A, 23);
    a = 0x05;
    RAM(OBJ_ID + x) = a;
    a = 0x8E;
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x03;
    RAM(OBJ_VY + x) = a;
    if (a != 0) { mff_cycles(0x81A9, 1); goto L_81B0; }
L_81AB:;
    mff_cycles(0x81AB, 7);
    a = 0x02;
    RAM(OBJ_ID + x) = a;
L_81B0:;
    mff_cycles(0x81B0, 7);
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
L_81B5:;
    mff_cycles(0x81B5, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x800B, 2); call_by_address_tail(0x800B, 6); return;
}

/*  $821E (bank 6) - lifted by port/tools/lift.py */
void func_821E_b6(void) {
    REGS_IN;
    mff_cycles(0x821E, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8241 (bank 6) - lifted by port/tools/lift.py */
void func_8241_b6(void) {
    REGS_IN;
    mff_cycles(0x8241, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x83A8, 2); call_by_address_tail(0x83A8, 6); return;
}

/*  $8638 (bank 6) - lifted by port/tools/lift.py */
void func_8638_b6(void) {
    REGS_IN;
    mff_cycles(0x8638, 4);
    a = prg_read((uint16_t)(0x87D6 + y));
    mff_cycles(0x863B, 2);
    if (a == 0) {
        mff_cycles(0x863D, 4);
        a = prg_read((uint16_t)(0x87F9 + y));
        mff_cycles(0x8640, 2);
        if (a == 0) {
            mff_cycles(0x8642, 4);
            a = prg_read((uint16_t)(0x881C + y));
            mff_cycles(0x8645, 2);
            if (a == 0) {
                mff_cycles(0x8647, 4);
                a = prg_read((uint16_t)(0x883F + y));
                mff_cycles(0x864A, 2);
                if (a == 0) {
                    mff_cycles(0x864C, 4);
                    a = prg_read((uint16_t)(0x8862 + y));
                    mff_cycles(0x864F, 2);
                    if (a == 0) {
                        mff_cycles(0x8651, 4);
                        a = prg_read((uint16_t)(0x8885 + y));
                        mff_cycles(0x8654, 2);
                        if (a == 0) {
                            mff_cycles(0x8656, 4);
                            a = prg_read((uint16_t)(0x88A8 + y));
                            mff_cycles(0x8659, 2);
                            if (a == 0) {
                                mff_cycles(0x865B, 4);
                                a = prg_read((uint16_t)(0x88A8 + y));
                                mff_cycles(0x865E, 2);
                                if (a == 0) {
                                    mff_cycles(0x8660, 4);
                                    a = prg_read((uint16_t)(0x88CB + y));
                                    mff_cycles(0x8663, 2);
                                    if (a == 0) {
                                        mff_cycles(0x8665, 4);
                                        a = prg_read((uint16_t)(0x88EE + y));
                                        mff_cycles(0x8668, 2);
                                        if (a == 0) {
                                            mff_cycles(0x866A, 4);
                                            a = prg_read((uint16_t)(0x8911 + y));
                                            mff_cycles(0x866D, 2);
                                            if (a == 0) {
                                                mff_cycles(0x866F, 4);
                                                a = prg_read((uint16_t)(0x8934 + y));
                                                mff_cycles(0x8672, 2);
                                                if (a == 0) {
                                                    mff_cycles(0x8674, 4);
                                                    a = prg_read((uint16_t)(0x8957 + y));
                                                    mff_cycles(0x8677, 2);
                                                    if (a == 0) {
                                                        mff_cycles(0x8679, 4);
                                                        a = prg_read((uint16_t)(0x897A + y));
                                                        mff_cycles(0x867C, 2);
                                                        if (a == 0) {
                                                            mff_cycles(0x867E, 4);
                                                            a = prg_read((uint16_t)(0x899D + y));
                                                            mff_cycles(0x8681, 2);
                                                            if (a == 0) {
                                                                mff_cycles(0x8683, 4);
                                                                a = prg_read((uint16_t)(0x89C0 + y));
                                                            } else {
                                                                mff_cycles(0x8681, 1);
                                                            }
                                                        } else {
                                                            mff_cycles(0x867C, 1);
                                                        }
                                                    } else {
                                                        mff_cycles(0x8677, 1);
                                                    }
                                                } else {
                                                    mff_cycles(0x8672, 1);
                                                }
                                            } else {
                                                mff_cycles(0x866D, 1);
                                            }
                                        } else {
                                            mff_cycles(0x8668, 1);
                                        }
                                    } else {
                                        mff_cycles(0x8663, 1);
                                    }
                                } else {
                                    mff_cycles(0x865E, 1);
                                }
                            } else {
                                mff_cycles(0x8659, 1);
                            }
                        } else {
                            mff_cycles(0x8654, 1);
                        }
                    } else {
                        mff_cycles(0x864F, 1);
                    }
                } else {
                    mff_cycles(0x864A, 1);
                }
            } else {
                mff_cycles(0x8645, 1);
            }
        } else {
            mff_cycles(0x8640, 1);
        }
    } else {
        mff_cycles(0x863B, 1);
    }
    mff_cycles(0x8686, 10);
    RAM(TMP0) = a;
    a = RAM(ATTACK_ID);
    if (a == 0x06) { mff_cycles(0x868C, 1); goto L_8692; }
    mff_cycles(0x868E, 4);
    if (a != 0x0C) { mff_cycles(0x8690, 1); goto L_8694; }
L_8692:;
    mff_cycles(0x8692, 3);
    RAM(COMBO_TARGET) = x;
L_8694:;
    mff_cycles(0x8694, 8);
    a = RAM(OBJ_KIND + x);
    if (a == 0x07) { mff_cycles(0x8699, 1); goto L_86B8; }
    mff_cycles(0x869B, 4);
    if (a == 0x08) { mff_cycles(0x869D, 1); goto L_86A9; }
    mff_cycles(0x869F, 4);
    if (a == 0x09) { mff_cycles(0x86A1, 1); goto L_86B8; }
    mff_cycles(0x86A3, 4);
    m = 0x0B;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x86A5, 1); goto L_86B8; }
    mff_cycles(0x86A7, 2);
    if (!z) { mff_cycles(0x86A7, 1); goto L_86E4; }
L_86A9:;
    mff_cycles(0x86A9, 4);
    if (y == 0x05) { mff_cycles(0x86AB, 1); goto L_86B1; }
    mff_cycles(0x86AD, 4);
    if (y != 0x13) { mff_cycles(0x86AF, 1); goto L_86B8; }
L_86B1:;
    mff_cycles(0x86B1, 10);
    a = RAM(TMP0);
    c = 0;
    m = 0x05;
    w = a + m + c;
    a = (uint8_t)w;
    RAM(TMP0) = a;
L_86B8:;
    mff_cycles(0x86B8, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07;
    w = a - m - !c;
    a = (uint8_t)w;
    y = a;
    if (a == 0x01) {
        mff_cycles(0x86C3, 6);
        a = RAM(OBJ_TIER + x);
        if (a != 0) {
            mff_cycles(0x86C8, 2);
            y = 0x03;
        } else {
            mff_cycles(0x86C6, 1);
        }
    } else {
        mff_cycles(0x86C1, 1);
    }
    mff_cycles(0x86CA, 7);
    a = RAM(ROUND);
    if (a == 0x04) {
        mff_cycles(0x86D0, 5);
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
    } else {
        mff_cycles(0x86CE, 1);
    }
    mff_cycles(0x86D2, 9);
    a = RAM(TMP0);
    c = 1;
    m = prg_read((uint16_t)(0x87AB + y));
    w = a - m - !c;
    a = (uint8_t)w;
    n = (a) >> 7;
    mff_cycles(0x86D8, 5);
    RAM(TMP0) = a;
    if (a == 0) { mff_cycles(0x86DA, 1); goto L_86E0; }
    mff_cycles(0x86DC, 2);
    if (n) { mff_cycles(0x86DC, 1); goto L_86E0; }
    mff_cycles(0x86DE, 2);
    if (!n) { mff_cycles(0x86DE, 1); goto L_86E4; }
L_86E0:;
    mff_cycles(0x86E0, 5);
    a = 0x01;
    RAM(TMP0) = a;
L_86E4:;
    mff_cycles(0x86E4, 22);
    a = RAM(OBJ_LIFE + x);
    c = 1;
    m = RAM(TMP0);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_LIFE + x) = a;
    a = RAM(OBJ_KIND + x);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86F2, 2); goto L_8702; }
    mff_cycles(0x86F4, 8);
    a = RAM(OBJ_LIFE + x);
    m = 0x30;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x86F9, 2); goto L_8702; }
    mff_cycles(0x86FB, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8700, 1); goto L_8703; }
L_8702:;
    mff_cycles(0x8702, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8703:;
    mff_cycles(0x8703, 7);
    a = RAM(GAME_ACTION);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0x8707, 1); goto L_870D; }
    mff_cycles(0x8709, 4);
    m = 0x21;
    c = a >= m;
    if (a != m) { mff_cycles(0x870B, 1); goto L_872D; }
L_870D:;
    mff_cycles(0x870D, 17);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GRAB_TIMER) = a;
    RAM(GAME_ACTION) = a;
    RAM(GRAB_HIT_COUNT) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0717, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8718, 13);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8722, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0726, 0xCEAA, 6)) return;
    SYNC_IN;
    mff_cycles(0x8727, 9);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + x) = a;
L_872D:;
    mff_cycles(0x872D, 7);
    a = 0x0D;
    RAM(TMP0) = a;
    y = 0x00;
    for (;;) {
        mff_cycles(0x8733, 4);
        a = prg_read((uint16_t)(0x87A7 + y));
        mff_cycles(0x8736, 7);
        RAM(TMP1) = a;
        a = prg_read((uint16_t)(0x87A8 + y));
        mff_cycles(0x873B, 55);
        RAM(TMP2) = a;
        y = RAM(TMP0);
        a = RAM(OBJ_DIR + x);
        RAM(OBJ_DIR + y) = a;
        a = RAM(OBJ_Y_HI + x);
        RAM(OBJ_Y_HI + y) = a;
        a = RAM(OBJ_X + x);
        c = 0;
        m = RAM(TMP1);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(OBJ_X + y) = a;
        a = RAM(OBJ_X_HI + x);
        m = 0x00;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(OBJ_X_HI + y) = a;
        if (a == RAM(OBJ_X_HI)) {
            mff_cycles(0x8761, 83);
            a = RAM(OBJ_Y + x);
            c = 0;
            m = 0x0E;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(OBJ_LAND_Y + y) = a;
            a = RAM(OBJ_Y + x);
            c = 1;
            m = RAM(TMP2);
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            RAM(OBJ_Y + y) = a;
            a = 0xFD;
            RAM(OBJ_ANIM + y) = a;
            a = 0xFF;
            RAM(OBJ_LIFE + y) = a;
            a = 0x0D;
            RAM(OBJ_KIND + y) = a;
            a = 0x06;
            RAM(OBJ_ID + y) = a;
            RAM(OBJ_SAVED_ID + y) = a;
            a = 0x80;
            RAM(OBJ_STATE + y) = a;
            a = 0x00;
            RAM(OBJ_ANIM_TIMER + y) = a;
            a = y;
            a &= 0x03;
            RAM(OBJ_ANIM_FRAME + y) = a;
        } else {
            mff_cycles(0x875F, 1);
        }
        mff_cycles(0x879A, 19);
        y = 0x02;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        a = RAM(TMP0);
        m = 0x09;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) break;
        mff_cycles(0x8733, 1);
    }
    mff_cycles(0x87A6, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $863D (bank 6) - lifted by port/tools/lift.py */
void func_863D_b6(void) {
    REGS_IN;
    mff_cycles(0x863D, 4);
    a = prg_read((uint16_t)(0x87F9 + y));
    mff_cycles(0x8640, 2);
    if (a == 0) {
        mff_cycles(0x8642, 4);
        a = prg_read((uint16_t)(0x881C + y));
        mff_cycles(0x8645, 2);
        if (a == 0) {
            mff_cycles(0x8647, 4);
            a = prg_read((uint16_t)(0x883F + y));
            mff_cycles(0x864A, 2);
            if (a == 0) {
                mff_cycles(0x864C, 4);
                a = prg_read((uint16_t)(0x8862 + y));
                mff_cycles(0x864F, 2);
                if (a == 0) {
                    mff_cycles(0x8651, 4);
                    a = prg_read((uint16_t)(0x8885 + y));
                    mff_cycles(0x8654, 2);
                    if (a == 0) {
                        mff_cycles(0x8656, 4);
                        a = prg_read((uint16_t)(0x88A8 + y));
                        mff_cycles(0x8659, 2);
                        if (a == 0) {
                            mff_cycles(0x865B, 4);
                            a = prg_read((uint16_t)(0x88A8 + y));
                            mff_cycles(0x865E, 2);
                            if (a == 0) {
                                mff_cycles(0x8660, 4);
                                a = prg_read((uint16_t)(0x88CB + y));
                                mff_cycles(0x8663, 2);
                                if (a == 0) {
                                    mff_cycles(0x8665, 4);
                                    a = prg_read((uint16_t)(0x88EE + y));
                                    mff_cycles(0x8668, 2);
                                    if (a == 0) {
                                        mff_cycles(0x866A, 4);
                                        a = prg_read((uint16_t)(0x8911 + y));
                                        mff_cycles(0x866D, 2);
                                        if (a == 0) {
                                            mff_cycles(0x866F, 4);
                                            a = prg_read((uint16_t)(0x8934 + y));
                                            mff_cycles(0x8672, 2);
                                            if (a == 0) {
                                                mff_cycles(0x8674, 4);
                                                a = prg_read((uint16_t)(0x8957 + y));
                                                mff_cycles(0x8677, 2);
                                                if (a == 0) {
                                                    mff_cycles(0x8679, 4);
                                                    a = prg_read((uint16_t)(0x897A + y));
                                                    mff_cycles(0x867C, 2);
                                                    if (a == 0) {
                                                        mff_cycles(0x867E, 4);
                                                        a = prg_read((uint16_t)(0x899D + y));
                                                        mff_cycles(0x8681, 2);
                                                        if (a == 0) {
                                                            mff_cycles(0x8683, 4);
                                                            a = prg_read((uint16_t)(0x89C0 + y));
                                                        } else {
                                                            mff_cycles(0x8681, 1);
                                                        }
                                                    } else {
                                                        mff_cycles(0x867C, 1);
                                                    }
                                                } else {
                                                    mff_cycles(0x8677, 1);
                                                }
                                            } else {
                                                mff_cycles(0x8672, 1);
                                            }
                                        } else {
                                            mff_cycles(0x866D, 1);
                                        }
                                    } else {
                                        mff_cycles(0x8668, 1);
                                    }
                                } else {
                                    mff_cycles(0x8663, 1);
                                }
                            } else {
                                mff_cycles(0x865E, 1);
                            }
                        } else {
                            mff_cycles(0x8659, 1);
                        }
                    } else {
                        mff_cycles(0x8654, 1);
                    }
                } else {
                    mff_cycles(0x864F, 1);
                }
            } else {
                mff_cycles(0x864A, 1);
            }
        } else {
            mff_cycles(0x8645, 1);
        }
    } else {
        mff_cycles(0x8640, 1);
    }
    mff_cycles(0x8686, 10);
    RAM(TMP0) = a;
    a = RAM(ATTACK_ID);
    if (a == 0x06) { mff_cycles(0x868C, 1); goto L_8692; }
    mff_cycles(0x868E, 4);
    if (a != 0x0C) { mff_cycles(0x8690, 1); goto L_8694; }
L_8692:;
    mff_cycles(0x8692, 3);
    RAM(COMBO_TARGET) = x;
L_8694:;
    mff_cycles(0x8694, 8);
    a = RAM(OBJ_KIND + x);
    if (a == 0x07) { mff_cycles(0x8699, 1); goto L_86B8; }
    mff_cycles(0x869B, 4);
    if (a == 0x08) { mff_cycles(0x869D, 1); goto L_86A9; }
    mff_cycles(0x869F, 4);
    if (a == 0x09) { mff_cycles(0x86A1, 1); goto L_86B8; }
    mff_cycles(0x86A3, 4);
    m = 0x0B;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x86A5, 1); goto L_86B8; }
    mff_cycles(0x86A7, 2);
    if (!z) { mff_cycles(0x86A7, 1); goto L_86E4; }
L_86A9:;
    mff_cycles(0x86A9, 4);
    if (y == 0x05) { mff_cycles(0x86AB, 1); goto L_86B1; }
    mff_cycles(0x86AD, 4);
    if (y != 0x13) { mff_cycles(0x86AF, 1); goto L_86B8; }
L_86B1:;
    mff_cycles(0x86B1, 10);
    a = RAM(TMP0);
    c = 0;
    m = 0x05;
    w = a + m + c;
    a = (uint8_t)w;
    RAM(TMP0) = a;
L_86B8:;
    mff_cycles(0x86B8, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07;
    w = a - m - !c;
    a = (uint8_t)w;
    y = a;
    if (a == 0x01) {
        mff_cycles(0x86C3, 6);
        a = RAM(OBJ_TIER + x);
        if (a != 0) {
            mff_cycles(0x86C8, 2);
            y = 0x03;
        } else {
            mff_cycles(0x86C6, 1);
        }
    } else {
        mff_cycles(0x86C1, 1);
    }
    mff_cycles(0x86CA, 7);
    a = RAM(ROUND);
    if (a == 0x04) {
        mff_cycles(0x86D0, 5);
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
    } else {
        mff_cycles(0x86CE, 1);
    }
    mff_cycles(0x86D2, 9);
    a = RAM(TMP0);
    c = 1;
    m = prg_read((uint16_t)(0x87AB + y));
    w = a - m - !c;
    a = (uint8_t)w;
    n = (a) >> 7;
    mff_cycles(0x86D8, 5);
    RAM(TMP0) = a;
    if (a == 0) { mff_cycles(0x86DA, 1); goto L_86E0; }
    mff_cycles(0x86DC, 2);
    if (n) { mff_cycles(0x86DC, 1); goto L_86E0; }
    mff_cycles(0x86DE, 2);
    if (!n) { mff_cycles(0x86DE, 1); goto L_86E4; }
L_86E0:;
    mff_cycles(0x86E0, 5);
    a = 0x01;
    RAM(TMP0) = a;
L_86E4:;
    mff_cycles(0x86E4, 22);
    a = RAM(OBJ_LIFE + x);
    c = 1;
    m = RAM(TMP0);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_LIFE + x) = a;
    a = RAM(OBJ_KIND + x);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86F2, 2); goto L_8702; }
    mff_cycles(0x86F4, 8);
    a = RAM(OBJ_LIFE + x);
    m = 0x30;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x86F9, 2); goto L_8702; }
    mff_cycles(0x86FB, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8700, 1); goto L_8703; }
L_8702:;
    mff_cycles(0x8702, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8703:;
    mff_cycles(0x8703, 7);
    a = RAM(GAME_ACTION);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0x8707, 1); goto L_870D; }
    mff_cycles(0x8709, 4);
    m = 0x21;
    c = a >= m;
    if (a != m) { mff_cycles(0x870B, 1); goto L_872D; }
L_870D:;
    mff_cycles(0x870D, 17);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GRAB_TIMER) = a;
    RAM(GAME_ACTION) = a;
    RAM(GRAB_HIT_COUNT) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0717, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8718, 13);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8722, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0726, 0xCEAA, 6)) return;
    SYNC_IN;
    mff_cycles(0x8727, 9);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + x) = a;
L_872D:;
    mff_cycles(0x872D, 7);
    a = 0x0D;
    RAM(TMP0) = a;
    y = 0x00;
    for (;;) {
        mff_cycles(0x8733, 4);
        a = prg_read((uint16_t)(0x87A7 + y));
        mff_cycles(0x8736, 7);
        RAM(TMP1) = a;
        a = prg_read((uint16_t)(0x87A8 + y));
        mff_cycles(0x873B, 55);
        RAM(TMP2) = a;
        y = RAM(TMP0);
        a = RAM(OBJ_DIR + x);
        RAM(OBJ_DIR + y) = a;
        a = RAM(OBJ_Y_HI + x);
        RAM(OBJ_Y_HI + y) = a;
        a = RAM(OBJ_X + x);
        c = 0;
        m = RAM(TMP1);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(OBJ_X + y) = a;
        a = RAM(OBJ_X_HI + x);
        m = 0x00;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(OBJ_X_HI + y) = a;
        if (a == RAM(OBJ_X_HI)) {
            mff_cycles(0x8761, 83);
            a = RAM(OBJ_Y + x);
            c = 0;
            m = 0x0E;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(OBJ_LAND_Y + y) = a;
            a = RAM(OBJ_Y + x);
            c = 1;
            m = RAM(TMP2);
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            RAM(OBJ_Y + y) = a;
            a = 0xFD;
            RAM(OBJ_ANIM + y) = a;
            a = 0xFF;
            RAM(OBJ_LIFE + y) = a;
            a = 0x0D;
            RAM(OBJ_KIND + y) = a;
            a = 0x06;
            RAM(OBJ_ID + y) = a;
            RAM(OBJ_SAVED_ID + y) = a;
            a = 0x80;
            RAM(OBJ_STATE + y) = a;
            a = 0x00;
            RAM(OBJ_ANIM_TIMER + y) = a;
            a = y;
            a &= 0x03;
            RAM(OBJ_ANIM_FRAME + y) = a;
        } else {
            mff_cycles(0x875F, 1);
        }
        mff_cycles(0x879A, 19);
        y = 0x02;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        a = RAM(TMP0);
        m = 0x09;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) break;
        mff_cycles(0x8733, 1);
    }
    mff_cycles(0x87A6, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8642 (bank 6) - lifted by port/tools/lift.py */
void func_8642_b6(void) {
    REGS_IN;
    mff_cycles(0x8642, 4);
    a = prg_read((uint16_t)(0x881C + y));
    mff_cycles(0x8645, 2);
    if (a == 0) {
        mff_cycles(0x8647, 4);
        a = prg_read((uint16_t)(0x883F + y));
        mff_cycles(0x864A, 2);
        if (a == 0) {
            mff_cycles(0x864C, 4);
            a = prg_read((uint16_t)(0x8862 + y));
            mff_cycles(0x864F, 2);
            if (a == 0) {
                mff_cycles(0x8651, 4);
                a = prg_read((uint16_t)(0x8885 + y));
                mff_cycles(0x8654, 2);
                if (a == 0) {
                    mff_cycles(0x8656, 4);
                    a = prg_read((uint16_t)(0x88A8 + y));
                    mff_cycles(0x8659, 2);
                    if (a == 0) {
                        mff_cycles(0x865B, 4);
                        a = prg_read((uint16_t)(0x88A8 + y));
                        mff_cycles(0x865E, 2);
                        if (a == 0) {
                            mff_cycles(0x8660, 4);
                            a = prg_read((uint16_t)(0x88CB + y));
                            mff_cycles(0x8663, 2);
                            if (a == 0) {
                                mff_cycles(0x8665, 4);
                                a = prg_read((uint16_t)(0x88EE + y));
                                mff_cycles(0x8668, 2);
                                if (a == 0) {
                                    mff_cycles(0x866A, 4);
                                    a = prg_read((uint16_t)(0x8911 + y));
                                    mff_cycles(0x866D, 2);
                                    if (a == 0) {
                                        mff_cycles(0x866F, 4);
                                        a = prg_read((uint16_t)(0x8934 + y));
                                        mff_cycles(0x8672, 2);
                                        if (a == 0) {
                                            mff_cycles(0x8674, 4);
                                            a = prg_read((uint16_t)(0x8957 + y));
                                            mff_cycles(0x8677, 2);
                                            if (a == 0) {
                                                mff_cycles(0x8679, 4);
                                                a = prg_read((uint16_t)(0x897A + y));
                                                mff_cycles(0x867C, 2);
                                                if (a == 0) {
                                                    mff_cycles(0x867E, 4);
                                                    a = prg_read((uint16_t)(0x899D + y));
                                                    mff_cycles(0x8681, 2);
                                                    if (a == 0) {
                                                        mff_cycles(0x8683, 4);
                                                        a = prg_read((uint16_t)(0x89C0 + y));
                                                    } else {
                                                        mff_cycles(0x8681, 1);
                                                    }
                                                } else {
                                                    mff_cycles(0x867C, 1);
                                                }
                                            } else {
                                                mff_cycles(0x8677, 1);
                                            }
                                        } else {
                                            mff_cycles(0x8672, 1);
                                        }
                                    } else {
                                        mff_cycles(0x866D, 1);
                                    }
                                } else {
                                    mff_cycles(0x8668, 1);
                                }
                            } else {
                                mff_cycles(0x8663, 1);
                            }
                        } else {
                            mff_cycles(0x865E, 1);
                        }
                    } else {
                        mff_cycles(0x8659, 1);
                    }
                } else {
                    mff_cycles(0x8654, 1);
                }
            } else {
                mff_cycles(0x864F, 1);
            }
        } else {
            mff_cycles(0x864A, 1);
        }
    } else {
        mff_cycles(0x8645, 1);
    }
    mff_cycles(0x8686, 10);
    RAM(TMP0) = a;
    a = RAM(ATTACK_ID);
    if (a == 0x06) { mff_cycles(0x868C, 1); goto L_8692; }
    mff_cycles(0x868E, 4);
    if (a != 0x0C) { mff_cycles(0x8690, 1); goto L_8694; }
L_8692:;
    mff_cycles(0x8692, 3);
    RAM(COMBO_TARGET) = x;
L_8694:;
    mff_cycles(0x8694, 8);
    a = RAM(OBJ_KIND + x);
    if (a == 0x07) { mff_cycles(0x8699, 1); goto L_86B8; }
    mff_cycles(0x869B, 4);
    if (a == 0x08) { mff_cycles(0x869D, 1); goto L_86A9; }
    mff_cycles(0x869F, 4);
    if (a == 0x09) { mff_cycles(0x86A1, 1); goto L_86B8; }
    mff_cycles(0x86A3, 4);
    m = 0x0B;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x86A5, 1); goto L_86B8; }
    mff_cycles(0x86A7, 2);
    if (!z) { mff_cycles(0x86A7, 1); goto L_86E4; }
L_86A9:;
    mff_cycles(0x86A9, 4);
    if (y == 0x05) { mff_cycles(0x86AB, 1); goto L_86B1; }
    mff_cycles(0x86AD, 4);
    if (y != 0x13) { mff_cycles(0x86AF, 1); goto L_86B8; }
L_86B1:;
    mff_cycles(0x86B1, 10);
    a = RAM(TMP0);
    c = 0;
    m = 0x05;
    w = a + m + c;
    a = (uint8_t)w;
    RAM(TMP0) = a;
L_86B8:;
    mff_cycles(0x86B8, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07;
    w = a - m - !c;
    a = (uint8_t)w;
    y = a;
    if (a == 0x01) {
        mff_cycles(0x86C3, 6);
        a = RAM(OBJ_TIER + x);
        if (a != 0) {
            mff_cycles(0x86C8, 2);
            y = 0x03;
        } else {
            mff_cycles(0x86C6, 1);
        }
    } else {
        mff_cycles(0x86C1, 1);
    }
    mff_cycles(0x86CA, 7);
    a = RAM(ROUND);
    if (a == 0x04) {
        mff_cycles(0x86D0, 5);
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
    } else {
        mff_cycles(0x86CE, 1);
    }
    mff_cycles(0x86D2, 9);
    a = RAM(TMP0);
    c = 1;
    m = prg_read((uint16_t)(0x87AB + y));
    w = a - m - !c;
    a = (uint8_t)w;
    n = (a) >> 7;
    mff_cycles(0x86D8, 5);
    RAM(TMP0) = a;
    if (a == 0) { mff_cycles(0x86DA, 1); goto L_86E0; }
    mff_cycles(0x86DC, 2);
    if (n) { mff_cycles(0x86DC, 1); goto L_86E0; }
    mff_cycles(0x86DE, 2);
    if (!n) { mff_cycles(0x86DE, 1); goto L_86E4; }
L_86E0:;
    mff_cycles(0x86E0, 5);
    a = 0x01;
    RAM(TMP0) = a;
L_86E4:;
    mff_cycles(0x86E4, 22);
    a = RAM(OBJ_LIFE + x);
    c = 1;
    m = RAM(TMP0);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_LIFE + x) = a;
    a = RAM(OBJ_KIND + x);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86F2, 2); goto L_8702; }
    mff_cycles(0x86F4, 8);
    a = RAM(OBJ_LIFE + x);
    m = 0x30;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x86F9, 2); goto L_8702; }
    mff_cycles(0x86FB, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8700, 1); goto L_8703; }
L_8702:;
    mff_cycles(0x8702, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8703:;
    mff_cycles(0x8703, 7);
    a = RAM(GAME_ACTION);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0x8707, 1); goto L_870D; }
    mff_cycles(0x8709, 4);
    m = 0x21;
    c = a >= m;
    if (a != m) { mff_cycles(0x870B, 1); goto L_872D; }
L_870D:;
    mff_cycles(0x870D, 17);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GRAB_TIMER) = a;
    RAM(GAME_ACTION) = a;
    RAM(GRAB_HIT_COUNT) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0717, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8718, 13);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8722, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0726, 0xCEAA, 6)) return;
    SYNC_IN;
    mff_cycles(0x8727, 9);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + x) = a;
L_872D:;
    mff_cycles(0x872D, 7);
    a = 0x0D;
    RAM(TMP0) = a;
    y = 0x00;
    for (;;) {
        mff_cycles(0x8733, 4);
        a = prg_read((uint16_t)(0x87A7 + y));
        mff_cycles(0x8736, 7);
        RAM(TMP1) = a;
        a = prg_read((uint16_t)(0x87A8 + y));
        mff_cycles(0x873B, 55);
        RAM(TMP2) = a;
        y = RAM(TMP0);
        a = RAM(OBJ_DIR + x);
        RAM(OBJ_DIR + y) = a;
        a = RAM(OBJ_Y_HI + x);
        RAM(OBJ_Y_HI + y) = a;
        a = RAM(OBJ_X + x);
        c = 0;
        m = RAM(TMP1);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(OBJ_X + y) = a;
        a = RAM(OBJ_X_HI + x);
        m = 0x00;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(OBJ_X_HI + y) = a;
        if (a == RAM(OBJ_X_HI)) {
            mff_cycles(0x8761, 83);
            a = RAM(OBJ_Y + x);
            c = 0;
            m = 0x0E;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(OBJ_LAND_Y + y) = a;
            a = RAM(OBJ_Y + x);
            c = 1;
            m = RAM(TMP2);
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            RAM(OBJ_Y + y) = a;
            a = 0xFD;
            RAM(OBJ_ANIM + y) = a;
            a = 0xFF;
            RAM(OBJ_LIFE + y) = a;
            a = 0x0D;
            RAM(OBJ_KIND + y) = a;
            a = 0x06;
            RAM(OBJ_ID + y) = a;
            RAM(OBJ_SAVED_ID + y) = a;
            a = 0x80;
            RAM(OBJ_STATE + y) = a;
            a = 0x00;
            RAM(OBJ_ANIM_TIMER + y) = a;
            a = y;
            a &= 0x03;
            RAM(OBJ_ANIM_FRAME + y) = a;
        } else {
            mff_cycles(0x875F, 1);
        }
        mff_cycles(0x879A, 19);
        y = 0x02;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        a = RAM(TMP0);
        m = 0x09;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) break;
        mff_cycles(0x8733, 1);
    }
    mff_cycles(0x87A6, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8647 (bank 6) - lifted by port/tools/lift.py */
void func_8647_b6(void) {
    REGS_IN;
    mff_cycles(0x8647, 4);
    a = prg_read((uint16_t)(0x883F + y));
    mff_cycles(0x864A, 2);
    if (a == 0) {
        mff_cycles(0x864C, 4);
        a = prg_read((uint16_t)(0x8862 + y));
        mff_cycles(0x864F, 2);
        if (a == 0) {
            mff_cycles(0x8651, 4);
            a = prg_read((uint16_t)(0x8885 + y));
            mff_cycles(0x8654, 2);
            if (a == 0) {
                mff_cycles(0x8656, 4);
                a = prg_read((uint16_t)(0x88A8 + y));
                mff_cycles(0x8659, 2);
                if (a == 0) {
                    mff_cycles(0x865B, 4);
                    a = prg_read((uint16_t)(0x88A8 + y));
                    mff_cycles(0x865E, 2);
                    if (a == 0) {
                        mff_cycles(0x8660, 4);
                        a = prg_read((uint16_t)(0x88CB + y));
                        mff_cycles(0x8663, 2);
                        if (a == 0) {
                            mff_cycles(0x8665, 4);
                            a = prg_read((uint16_t)(0x88EE + y));
                            mff_cycles(0x8668, 2);
                            if (a == 0) {
                                mff_cycles(0x866A, 4);
                                a = prg_read((uint16_t)(0x8911 + y));
                                mff_cycles(0x866D, 2);
                                if (a == 0) {
                                    mff_cycles(0x866F, 4);
                                    a = prg_read((uint16_t)(0x8934 + y));
                                    mff_cycles(0x8672, 2);
                                    if (a == 0) {
                                        mff_cycles(0x8674, 4);
                                        a = prg_read((uint16_t)(0x8957 + y));
                                        mff_cycles(0x8677, 2);
                                        if (a == 0) {
                                            mff_cycles(0x8679, 4);
                                            a = prg_read((uint16_t)(0x897A + y));
                                            mff_cycles(0x867C, 2);
                                            if (a == 0) {
                                                mff_cycles(0x867E, 4);
                                                a = prg_read((uint16_t)(0x899D + y));
                                                mff_cycles(0x8681, 2);
                                                if (a == 0) {
                                                    mff_cycles(0x8683, 4);
                                                    a = prg_read((uint16_t)(0x89C0 + y));
                                                } else {
                                                    mff_cycles(0x8681, 1);
                                                }
                                            } else {
                                                mff_cycles(0x867C, 1);
                                            }
                                        } else {
                                            mff_cycles(0x8677, 1);
                                        }
                                    } else {
                                        mff_cycles(0x8672, 1);
                                    }
                                } else {
                                    mff_cycles(0x866D, 1);
                                }
                            } else {
                                mff_cycles(0x8668, 1);
                            }
                        } else {
                            mff_cycles(0x8663, 1);
                        }
                    } else {
                        mff_cycles(0x865E, 1);
                    }
                } else {
                    mff_cycles(0x8659, 1);
                }
            } else {
                mff_cycles(0x8654, 1);
            }
        } else {
            mff_cycles(0x864F, 1);
        }
    } else {
        mff_cycles(0x864A, 1);
    }
    mff_cycles(0x8686, 10);
    RAM(TMP0) = a;
    a = RAM(ATTACK_ID);
    if (a == 0x06) { mff_cycles(0x868C, 1); goto L_8692; }
    mff_cycles(0x868E, 4);
    if (a != 0x0C) { mff_cycles(0x8690, 1); goto L_8694; }
L_8692:;
    mff_cycles(0x8692, 3);
    RAM(COMBO_TARGET) = x;
L_8694:;
    mff_cycles(0x8694, 8);
    a = RAM(OBJ_KIND + x);
    if (a == 0x07) { mff_cycles(0x8699, 1); goto L_86B8; }
    mff_cycles(0x869B, 4);
    if (a == 0x08) { mff_cycles(0x869D, 1); goto L_86A9; }
    mff_cycles(0x869F, 4);
    if (a == 0x09) { mff_cycles(0x86A1, 1); goto L_86B8; }
    mff_cycles(0x86A3, 4);
    m = 0x0B;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x86A5, 1); goto L_86B8; }
    mff_cycles(0x86A7, 2);
    if (!z) { mff_cycles(0x86A7, 1); goto L_86E4; }
L_86A9:;
    mff_cycles(0x86A9, 4);
    if (y == 0x05) { mff_cycles(0x86AB, 1); goto L_86B1; }
    mff_cycles(0x86AD, 4);
    if (y != 0x13) { mff_cycles(0x86AF, 1); goto L_86B8; }
L_86B1:;
    mff_cycles(0x86B1, 10);
    a = RAM(TMP0);
    c = 0;
    m = 0x05;
    w = a + m + c;
    a = (uint8_t)w;
    RAM(TMP0) = a;
L_86B8:;
    mff_cycles(0x86B8, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07;
    w = a - m - !c;
    a = (uint8_t)w;
    y = a;
    if (a == 0x01) {
        mff_cycles(0x86C3, 6);
        a = RAM(OBJ_TIER + x);
        if (a != 0) {
            mff_cycles(0x86C8, 2);
            y = 0x03;
        } else {
            mff_cycles(0x86C6, 1);
        }
    } else {
        mff_cycles(0x86C1, 1);
    }
    mff_cycles(0x86CA, 7);
    a = RAM(ROUND);
    if (a == 0x04) {
        mff_cycles(0x86D0, 5);
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
    } else {
        mff_cycles(0x86CE, 1);
    }
    mff_cycles(0x86D2, 9);
    a = RAM(TMP0);
    c = 1;
    m = prg_read((uint16_t)(0x87AB + y));
    w = a - m - !c;
    a = (uint8_t)w;
    n = (a) >> 7;
    mff_cycles(0x86D8, 5);
    RAM(TMP0) = a;
    if (a == 0) { mff_cycles(0x86DA, 1); goto L_86E0; }
    mff_cycles(0x86DC, 2);
    if (n) { mff_cycles(0x86DC, 1); goto L_86E0; }
    mff_cycles(0x86DE, 2);
    if (!n) { mff_cycles(0x86DE, 1); goto L_86E4; }
L_86E0:;
    mff_cycles(0x86E0, 5);
    a = 0x01;
    RAM(TMP0) = a;
L_86E4:;
    mff_cycles(0x86E4, 22);
    a = RAM(OBJ_LIFE + x);
    c = 1;
    m = RAM(TMP0);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_LIFE + x) = a;
    a = RAM(OBJ_KIND + x);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86F2, 2); goto L_8702; }
    mff_cycles(0x86F4, 8);
    a = RAM(OBJ_LIFE + x);
    m = 0x30;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x86F9, 2); goto L_8702; }
    mff_cycles(0x86FB, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8700, 1); goto L_8703; }
L_8702:;
    mff_cycles(0x8702, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8703:;
    mff_cycles(0x8703, 7);
    a = RAM(GAME_ACTION);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0x8707, 1); goto L_870D; }
    mff_cycles(0x8709, 4);
    m = 0x21;
    c = a >= m;
    if (a != m) { mff_cycles(0x870B, 1); goto L_872D; }
L_870D:;
    mff_cycles(0x870D, 17);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GRAB_TIMER) = a;
    RAM(GAME_ACTION) = a;
    RAM(GRAB_HIT_COUNT) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0717, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8718, 13);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8722, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0726, 0xCEAA, 6)) return;
    SYNC_IN;
    mff_cycles(0x8727, 9);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + x) = a;
L_872D:;
    mff_cycles(0x872D, 7);
    a = 0x0D;
    RAM(TMP0) = a;
    y = 0x00;
    for (;;) {
        mff_cycles(0x8733, 4);
        a = prg_read((uint16_t)(0x87A7 + y));
        mff_cycles(0x8736, 7);
        RAM(TMP1) = a;
        a = prg_read((uint16_t)(0x87A8 + y));
        mff_cycles(0x873B, 55);
        RAM(TMP2) = a;
        y = RAM(TMP0);
        a = RAM(OBJ_DIR + x);
        RAM(OBJ_DIR + y) = a;
        a = RAM(OBJ_Y_HI + x);
        RAM(OBJ_Y_HI + y) = a;
        a = RAM(OBJ_X + x);
        c = 0;
        m = RAM(TMP1);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(OBJ_X + y) = a;
        a = RAM(OBJ_X_HI + x);
        m = 0x00;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(OBJ_X_HI + y) = a;
        if (a == RAM(OBJ_X_HI)) {
            mff_cycles(0x8761, 83);
            a = RAM(OBJ_Y + x);
            c = 0;
            m = 0x0E;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(OBJ_LAND_Y + y) = a;
            a = RAM(OBJ_Y + x);
            c = 1;
            m = RAM(TMP2);
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            RAM(OBJ_Y + y) = a;
            a = 0xFD;
            RAM(OBJ_ANIM + y) = a;
            a = 0xFF;
            RAM(OBJ_LIFE + y) = a;
            a = 0x0D;
            RAM(OBJ_KIND + y) = a;
            a = 0x06;
            RAM(OBJ_ID + y) = a;
            RAM(OBJ_SAVED_ID + y) = a;
            a = 0x80;
            RAM(OBJ_STATE + y) = a;
            a = 0x00;
            RAM(OBJ_ANIM_TIMER + y) = a;
            a = y;
            a &= 0x03;
            RAM(OBJ_ANIM_FRAME + y) = a;
        } else {
            mff_cycles(0x875F, 1);
        }
        mff_cycles(0x879A, 19);
        y = 0x02;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        a = RAM(TMP0);
        m = 0x09;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) break;
        mff_cycles(0x8733, 1);
    }
    mff_cycles(0x87A6, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $864C (bank 6) - lifted by port/tools/lift.py */
void func_864C_b6(void) {
    REGS_IN;
    mff_cycles(0x864C, 4);
    a = prg_read((uint16_t)(0x8862 + y));
    mff_cycles(0x864F, 2);
    if (a == 0) {
        mff_cycles(0x8651, 4);
        a = prg_read((uint16_t)(0x8885 + y));
        mff_cycles(0x8654, 2);
        if (a == 0) {
            mff_cycles(0x8656, 4);
            a = prg_read((uint16_t)(0x88A8 + y));
            mff_cycles(0x8659, 2);
            if (a == 0) {
                mff_cycles(0x865B, 4);
                a = prg_read((uint16_t)(0x88A8 + y));
                mff_cycles(0x865E, 2);
                if (a == 0) {
                    mff_cycles(0x8660, 4);
                    a = prg_read((uint16_t)(0x88CB + y));
                    mff_cycles(0x8663, 2);
                    if (a == 0) {
                        mff_cycles(0x8665, 4);
                        a = prg_read((uint16_t)(0x88EE + y));
                        mff_cycles(0x8668, 2);
                        if (a == 0) {
                            mff_cycles(0x866A, 4);
                            a = prg_read((uint16_t)(0x8911 + y));
                            mff_cycles(0x866D, 2);
                            if (a == 0) {
                                mff_cycles(0x866F, 4);
                                a = prg_read((uint16_t)(0x8934 + y));
                                mff_cycles(0x8672, 2);
                                if (a == 0) {
                                    mff_cycles(0x8674, 4);
                                    a = prg_read((uint16_t)(0x8957 + y));
                                    mff_cycles(0x8677, 2);
                                    if (a == 0) {
                                        mff_cycles(0x8679, 4);
                                        a = prg_read((uint16_t)(0x897A + y));
                                        mff_cycles(0x867C, 2);
                                        if (a == 0) {
                                            mff_cycles(0x867E, 4);
                                            a = prg_read((uint16_t)(0x899D + y));
                                            mff_cycles(0x8681, 2);
                                            if (a == 0) {
                                                mff_cycles(0x8683, 4);
                                                a = prg_read((uint16_t)(0x89C0 + y));
                                            } else {
                                                mff_cycles(0x8681, 1);
                                            }
                                        } else {
                                            mff_cycles(0x867C, 1);
                                        }
                                    } else {
                                        mff_cycles(0x8677, 1);
                                    }
                                } else {
                                    mff_cycles(0x8672, 1);
                                }
                            } else {
                                mff_cycles(0x866D, 1);
                            }
                        } else {
                            mff_cycles(0x8668, 1);
                        }
                    } else {
                        mff_cycles(0x8663, 1);
                    }
                } else {
                    mff_cycles(0x865E, 1);
                }
            } else {
                mff_cycles(0x8659, 1);
            }
        } else {
            mff_cycles(0x8654, 1);
        }
    } else {
        mff_cycles(0x864F, 1);
    }
    mff_cycles(0x8686, 10);
    RAM(TMP0) = a;
    a = RAM(ATTACK_ID);
    if (a == 0x06) { mff_cycles(0x868C, 1); goto L_8692; }
    mff_cycles(0x868E, 4);
    if (a != 0x0C) { mff_cycles(0x8690, 1); goto L_8694; }
L_8692:;
    mff_cycles(0x8692, 3);
    RAM(COMBO_TARGET) = x;
L_8694:;
    mff_cycles(0x8694, 8);
    a = RAM(OBJ_KIND + x);
    if (a == 0x07) { mff_cycles(0x8699, 1); goto L_86B8; }
    mff_cycles(0x869B, 4);
    if (a == 0x08) { mff_cycles(0x869D, 1); goto L_86A9; }
    mff_cycles(0x869F, 4);
    if (a == 0x09) { mff_cycles(0x86A1, 1); goto L_86B8; }
    mff_cycles(0x86A3, 4);
    m = 0x0B;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x86A5, 1); goto L_86B8; }
    mff_cycles(0x86A7, 2);
    if (!z) { mff_cycles(0x86A7, 1); goto L_86E4; }
L_86A9:;
    mff_cycles(0x86A9, 4);
    if (y == 0x05) { mff_cycles(0x86AB, 1); goto L_86B1; }
    mff_cycles(0x86AD, 4);
    if (y != 0x13) { mff_cycles(0x86AF, 1); goto L_86B8; }
L_86B1:;
    mff_cycles(0x86B1, 10);
    a = RAM(TMP0);
    c = 0;
    m = 0x05;
    w = a + m + c;
    a = (uint8_t)w;
    RAM(TMP0) = a;
L_86B8:;
    mff_cycles(0x86B8, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07;
    w = a - m - !c;
    a = (uint8_t)w;
    y = a;
    if (a == 0x01) {
        mff_cycles(0x86C3, 6);
        a = RAM(OBJ_TIER + x);
        if (a != 0) {
            mff_cycles(0x86C8, 2);
            y = 0x03;
        } else {
            mff_cycles(0x86C6, 1);
        }
    } else {
        mff_cycles(0x86C1, 1);
    }
    mff_cycles(0x86CA, 7);
    a = RAM(ROUND);
    if (a == 0x04) {
        mff_cycles(0x86D0, 5);
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
    } else {
        mff_cycles(0x86CE, 1);
    }
    mff_cycles(0x86D2, 9);
    a = RAM(TMP0);
    c = 1;
    m = prg_read((uint16_t)(0x87AB + y));
    w = a - m - !c;
    a = (uint8_t)w;
    n = (a) >> 7;
    mff_cycles(0x86D8, 5);
    RAM(TMP0) = a;
    if (a == 0) { mff_cycles(0x86DA, 1); goto L_86E0; }
    mff_cycles(0x86DC, 2);
    if (n) { mff_cycles(0x86DC, 1); goto L_86E0; }
    mff_cycles(0x86DE, 2);
    if (!n) { mff_cycles(0x86DE, 1); goto L_86E4; }
L_86E0:;
    mff_cycles(0x86E0, 5);
    a = 0x01;
    RAM(TMP0) = a;
L_86E4:;
    mff_cycles(0x86E4, 22);
    a = RAM(OBJ_LIFE + x);
    c = 1;
    m = RAM(TMP0);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_LIFE + x) = a;
    a = RAM(OBJ_KIND + x);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86F2, 2); goto L_8702; }
    mff_cycles(0x86F4, 8);
    a = RAM(OBJ_LIFE + x);
    m = 0x30;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x86F9, 2); goto L_8702; }
    mff_cycles(0x86FB, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8700, 1); goto L_8703; }
L_8702:;
    mff_cycles(0x8702, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8703:;
    mff_cycles(0x8703, 7);
    a = RAM(GAME_ACTION);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0x8707, 1); goto L_870D; }
    mff_cycles(0x8709, 4);
    m = 0x21;
    c = a >= m;
    if (a != m) { mff_cycles(0x870B, 1); goto L_872D; }
L_870D:;
    mff_cycles(0x870D, 17);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GRAB_TIMER) = a;
    RAM(GAME_ACTION) = a;
    RAM(GRAB_HIT_COUNT) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0717, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8718, 13);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8722, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0726, 0xCEAA, 6)) return;
    SYNC_IN;
    mff_cycles(0x8727, 9);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + x) = a;
L_872D:;
    mff_cycles(0x872D, 7);
    a = 0x0D;
    RAM(TMP0) = a;
    y = 0x00;
    for (;;) {
        mff_cycles(0x8733, 4);
        a = prg_read((uint16_t)(0x87A7 + y));
        mff_cycles(0x8736, 7);
        RAM(TMP1) = a;
        a = prg_read((uint16_t)(0x87A8 + y));
        mff_cycles(0x873B, 55);
        RAM(TMP2) = a;
        y = RAM(TMP0);
        a = RAM(OBJ_DIR + x);
        RAM(OBJ_DIR + y) = a;
        a = RAM(OBJ_Y_HI + x);
        RAM(OBJ_Y_HI + y) = a;
        a = RAM(OBJ_X + x);
        c = 0;
        m = RAM(TMP1);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(OBJ_X + y) = a;
        a = RAM(OBJ_X_HI + x);
        m = 0x00;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(OBJ_X_HI + y) = a;
        if (a == RAM(OBJ_X_HI)) {
            mff_cycles(0x8761, 83);
            a = RAM(OBJ_Y + x);
            c = 0;
            m = 0x0E;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(OBJ_LAND_Y + y) = a;
            a = RAM(OBJ_Y + x);
            c = 1;
            m = RAM(TMP2);
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            RAM(OBJ_Y + y) = a;
            a = 0xFD;
            RAM(OBJ_ANIM + y) = a;
            a = 0xFF;
            RAM(OBJ_LIFE + y) = a;
            a = 0x0D;
            RAM(OBJ_KIND + y) = a;
            a = 0x06;
            RAM(OBJ_ID + y) = a;
            RAM(OBJ_SAVED_ID + y) = a;
            a = 0x80;
            RAM(OBJ_STATE + y) = a;
            a = 0x00;
            RAM(OBJ_ANIM_TIMER + y) = a;
            a = y;
            a &= 0x03;
            RAM(OBJ_ANIM_FRAME + y) = a;
        } else {
            mff_cycles(0x875F, 1);
        }
        mff_cycles(0x879A, 19);
        y = 0x02;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        a = RAM(TMP0);
        m = 0x09;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) break;
        mff_cycles(0x8733, 1);
    }
    mff_cycles(0x87A6, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8651 (bank 6) - lifted by port/tools/lift.py */
void func_8651_b6(void) {
    REGS_IN;
    mff_cycles(0x8651, 4);
    a = prg_read((uint16_t)(0x8885 + y));
    mff_cycles(0x8654, 2);
    if (a == 0) {
        mff_cycles(0x8656, 4);
        a = prg_read((uint16_t)(0x88A8 + y));
        mff_cycles(0x8659, 2);
        if (a == 0) {
            mff_cycles(0x865B, 4);
            a = prg_read((uint16_t)(0x88A8 + y));
            mff_cycles(0x865E, 2);
            if (a == 0) {
                mff_cycles(0x8660, 4);
                a = prg_read((uint16_t)(0x88CB + y));
                mff_cycles(0x8663, 2);
                if (a == 0) {
                    mff_cycles(0x8665, 4);
                    a = prg_read((uint16_t)(0x88EE + y));
                    mff_cycles(0x8668, 2);
                    if (a == 0) {
                        mff_cycles(0x866A, 4);
                        a = prg_read((uint16_t)(0x8911 + y));
                        mff_cycles(0x866D, 2);
                        if (a == 0) {
                            mff_cycles(0x866F, 4);
                            a = prg_read((uint16_t)(0x8934 + y));
                            mff_cycles(0x8672, 2);
                            if (a == 0) {
                                mff_cycles(0x8674, 4);
                                a = prg_read((uint16_t)(0x8957 + y));
                                mff_cycles(0x8677, 2);
                                if (a == 0) {
                                    mff_cycles(0x8679, 4);
                                    a = prg_read((uint16_t)(0x897A + y));
                                    mff_cycles(0x867C, 2);
                                    if (a == 0) {
                                        mff_cycles(0x867E, 4);
                                        a = prg_read((uint16_t)(0x899D + y));
                                        mff_cycles(0x8681, 2);
                                        if (a == 0) {
                                            mff_cycles(0x8683, 4);
                                            a = prg_read((uint16_t)(0x89C0 + y));
                                        } else {
                                            mff_cycles(0x8681, 1);
                                        }
                                    } else {
                                        mff_cycles(0x867C, 1);
                                    }
                                } else {
                                    mff_cycles(0x8677, 1);
                                }
                            } else {
                                mff_cycles(0x8672, 1);
                            }
                        } else {
                            mff_cycles(0x866D, 1);
                        }
                    } else {
                        mff_cycles(0x8668, 1);
                    }
                } else {
                    mff_cycles(0x8663, 1);
                }
            } else {
                mff_cycles(0x865E, 1);
            }
        } else {
            mff_cycles(0x8659, 1);
        }
    } else {
        mff_cycles(0x8654, 1);
    }
    mff_cycles(0x8686, 10);
    RAM(TMP0) = a;
    a = RAM(ATTACK_ID);
    if (a == 0x06) { mff_cycles(0x868C, 1); goto L_8692; }
    mff_cycles(0x868E, 4);
    if (a != 0x0C) { mff_cycles(0x8690, 1); goto L_8694; }
L_8692:;
    mff_cycles(0x8692, 3);
    RAM(COMBO_TARGET) = x;
L_8694:;
    mff_cycles(0x8694, 8);
    a = RAM(OBJ_KIND + x);
    if (a == 0x07) { mff_cycles(0x8699, 1); goto L_86B8; }
    mff_cycles(0x869B, 4);
    if (a == 0x08) { mff_cycles(0x869D, 1); goto L_86A9; }
    mff_cycles(0x869F, 4);
    if (a == 0x09) { mff_cycles(0x86A1, 1); goto L_86B8; }
    mff_cycles(0x86A3, 4);
    m = 0x0B;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x86A5, 1); goto L_86B8; }
    mff_cycles(0x86A7, 2);
    if (!z) { mff_cycles(0x86A7, 1); goto L_86E4; }
L_86A9:;
    mff_cycles(0x86A9, 4);
    if (y == 0x05) { mff_cycles(0x86AB, 1); goto L_86B1; }
    mff_cycles(0x86AD, 4);
    if (y != 0x13) { mff_cycles(0x86AF, 1); goto L_86B8; }
L_86B1:;
    mff_cycles(0x86B1, 10);
    a = RAM(TMP0);
    c = 0;
    m = 0x05;
    w = a + m + c;
    a = (uint8_t)w;
    RAM(TMP0) = a;
L_86B8:;
    mff_cycles(0x86B8, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07;
    w = a - m - !c;
    a = (uint8_t)w;
    y = a;
    if (a == 0x01) {
        mff_cycles(0x86C3, 6);
        a = RAM(OBJ_TIER + x);
        if (a != 0) {
            mff_cycles(0x86C8, 2);
            y = 0x03;
        } else {
            mff_cycles(0x86C6, 1);
        }
    } else {
        mff_cycles(0x86C1, 1);
    }
    mff_cycles(0x86CA, 7);
    a = RAM(ROUND);
    if (a == 0x04) {
        mff_cycles(0x86D0, 5);
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
    } else {
        mff_cycles(0x86CE, 1);
    }
    mff_cycles(0x86D2, 9);
    a = RAM(TMP0);
    c = 1;
    m = prg_read((uint16_t)(0x87AB + y));
    w = a - m - !c;
    a = (uint8_t)w;
    n = (a) >> 7;
    mff_cycles(0x86D8, 5);
    RAM(TMP0) = a;
    if (a == 0) { mff_cycles(0x86DA, 1); goto L_86E0; }
    mff_cycles(0x86DC, 2);
    if (n) { mff_cycles(0x86DC, 1); goto L_86E0; }
    mff_cycles(0x86DE, 2);
    if (!n) { mff_cycles(0x86DE, 1); goto L_86E4; }
L_86E0:;
    mff_cycles(0x86E0, 5);
    a = 0x01;
    RAM(TMP0) = a;
L_86E4:;
    mff_cycles(0x86E4, 22);
    a = RAM(OBJ_LIFE + x);
    c = 1;
    m = RAM(TMP0);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_LIFE + x) = a;
    a = RAM(OBJ_KIND + x);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86F2, 2); goto L_8702; }
    mff_cycles(0x86F4, 8);
    a = RAM(OBJ_LIFE + x);
    m = 0x30;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x86F9, 2); goto L_8702; }
    mff_cycles(0x86FB, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8700, 1); goto L_8703; }
L_8702:;
    mff_cycles(0x8702, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8703:;
    mff_cycles(0x8703, 7);
    a = RAM(GAME_ACTION);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0x8707, 1); goto L_870D; }
    mff_cycles(0x8709, 4);
    m = 0x21;
    c = a >= m;
    if (a != m) { mff_cycles(0x870B, 1); goto L_872D; }
L_870D:;
    mff_cycles(0x870D, 17);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GRAB_TIMER) = a;
    RAM(GAME_ACTION) = a;
    RAM(GRAB_HIT_COUNT) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0717, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8718, 13);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8722, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0726, 0xCEAA, 6)) return;
    SYNC_IN;
    mff_cycles(0x8727, 9);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + x) = a;
L_872D:;
    mff_cycles(0x872D, 7);
    a = 0x0D;
    RAM(TMP0) = a;
    y = 0x00;
    for (;;) {
        mff_cycles(0x8733, 4);
        a = prg_read((uint16_t)(0x87A7 + y));
        mff_cycles(0x8736, 7);
        RAM(TMP1) = a;
        a = prg_read((uint16_t)(0x87A8 + y));
        mff_cycles(0x873B, 55);
        RAM(TMP2) = a;
        y = RAM(TMP0);
        a = RAM(OBJ_DIR + x);
        RAM(OBJ_DIR + y) = a;
        a = RAM(OBJ_Y_HI + x);
        RAM(OBJ_Y_HI + y) = a;
        a = RAM(OBJ_X + x);
        c = 0;
        m = RAM(TMP1);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(OBJ_X + y) = a;
        a = RAM(OBJ_X_HI + x);
        m = 0x00;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(OBJ_X_HI + y) = a;
        if (a == RAM(OBJ_X_HI)) {
            mff_cycles(0x8761, 83);
            a = RAM(OBJ_Y + x);
            c = 0;
            m = 0x0E;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(OBJ_LAND_Y + y) = a;
            a = RAM(OBJ_Y + x);
            c = 1;
            m = RAM(TMP2);
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            RAM(OBJ_Y + y) = a;
            a = 0xFD;
            RAM(OBJ_ANIM + y) = a;
            a = 0xFF;
            RAM(OBJ_LIFE + y) = a;
            a = 0x0D;
            RAM(OBJ_KIND + y) = a;
            a = 0x06;
            RAM(OBJ_ID + y) = a;
            RAM(OBJ_SAVED_ID + y) = a;
            a = 0x80;
            RAM(OBJ_STATE + y) = a;
            a = 0x00;
            RAM(OBJ_ANIM_TIMER + y) = a;
            a = y;
            a &= 0x03;
            RAM(OBJ_ANIM_FRAME + y) = a;
        } else {
            mff_cycles(0x875F, 1);
        }
        mff_cycles(0x879A, 19);
        y = 0x02;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        a = RAM(TMP0);
        m = 0x09;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) break;
        mff_cycles(0x8733, 1);
    }
    mff_cycles(0x87A6, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8656 (bank 6) - lifted by port/tools/lift.py */
void func_8656_b6(void) {
    REGS_IN;
    mff_cycles(0x8656, 4);
    a = prg_read((uint16_t)(0x88A8 + y));
    mff_cycles(0x8659, 2);
    if (a == 0) {
        mff_cycles(0x865B, 4);
        a = prg_read((uint16_t)(0x88A8 + y));
        mff_cycles(0x865E, 2);
        if (a == 0) {
            mff_cycles(0x8660, 4);
            a = prg_read((uint16_t)(0x88CB + y));
            mff_cycles(0x8663, 2);
            if (a == 0) {
                mff_cycles(0x8665, 4);
                a = prg_read((uint16_t)(0x88EE + y));
                mff_cycles(0x8668, 2);
                if (a == 0) {
                    mff_cycles(0x866A, 4);
                    a = prg_read((uint16_t)(0x8911 + y));
                    mff_cycles(0x866D, 2);
                    if (a == 0) {
                        mff_cycles(0x866F, 4);
                        a = prg_read((uint16_t)(0x8934 + y));
                        mff_cycles(0x8672, 2);
                        if (a == 0) {
                            mff_cycles(0x8674, 4);
                            a = prg_read((uint16_t)(0x8957 + y));
                            mff_cycles(0x8677, 2);
                            if (a == 0) {
                                mff_cycles(0x8679, 4);
                                a = prg_read((uint16_t)(0x897A + y));
                                mff_cycles(0x867C, 2);
                                if (a == 0) {
                                    mff_cycles(0x867E, 4);
                                    a = prg_read((uint16_t)(0x899D + y));
                                    mff_cycles(0x8681, 2);
                                    if (a == 0) {
                                        mff_cycles(0x8683, 4);
                                        a = prg_read((uint16_t)(0x89C0 + y));
                                    } else {
                                        mff_cycles(0x8681, 1);
                                    }
                                } else {
                                    mff_cycles(0x867C, 1);
                                }
                            } else {
                                mff_cycles(0x8677, 1);
                            }
                        } else {
                            mff_cycles(0x8672, 1);
                        }
                    } else {
                        mff_cycles(0x866D, 1);
                    }
                } else {
                    mff_cycles(0x8668, 1);
                }
            } else {
                mff_cycles(0x8663, 1);
            }
        } else {
            mff_cycles(0x865E, 1);
        }
    } else {
        mff_cycles(0x8659, 1);
    }
    mff_cycles(0x8686, 10);
    RAM(TMP0) = a;
    a = RAM(ATTACK_ID);
    if (a == 0x06) { mff_cycles(0x868C, 1); goto L_8692; }
    mff_cycles(0x868E, 4);
    if (a != 0x0C) { mff_cycles(0x8690, 1); goto L_8694; }
L_8692:;
    mff_cycles(0x8692, 3);
    RAM(COMBO_TARGET) = x;
L_8694:;
    mff_cycles(0x8694, 8);
    a = RAM(OBJ_KIND + x);
    if (a == 0x07) { mff_cycles(0x8699, 1); goto L_86B8; }
    mff_cycles(0x869B, 4);
    if (a == 0x08) { mff_cycles(0x869D, 1); goto L_86A9; }
    mff_cycles(0x869F, 4);
    if (a == 0x09) { mff_cycles(0x86A1, 1); goto L_86B8; }
    mff_cycles(0x86A3, 4);
    m = 0x0B;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x86A5, 1); goto L_86B8; }
    mff_cycles(0x86A7, 2);
    if (!z) { mff_cycles(0x86A7, 1); goto L_86E4; }
L_86A9:;
    mff_cycles(0x86A9, 4);
    if (y == 0x05) { mff_cycles(0x86AB, 1); goto L_86B1; }
    mff_cycles(0x86AD, 4);
    if (y != 0x13) { mff_cycles(0x86AF, 1); goto L_86B8; }
L_86B1:;
    mff_cycles(0x86B1, 10);
    a = RAM(TMP0);
    c = 0;
    m = 0x05;
    w = a + m + c;
    a = (uint8_t)w;
    RAM(TMP0) = a;
L_86B8:;
    mff_cycles(0x86B8, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07;
    w = a - m - !c;
    a = (uint8_t)w;
    y = a;
    if (a == 0x01) {
        mff_cycles(0x86C3, 6);
        a = RAM(OBJ_TIER + x);
        if (a != 0) {
            mff_cycles(0x86C8, 2);
            y = 0x03;
        } else {
            mff_cycles(0x86C6, 1);
        }
    } else {
        mff_cycles(0x86C1, 1);
    }
    mff_cycles(0x86CA, 7);
    a = RAM(ROUND);
    if (a == 0x04) {
        mff_cycles(0x86D0, 5);
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
    } else {
        mff_cycles(0x86CE, 1);
    }
    mff_cycles(0x86D2, 9);
    a = RAM(TMP0);
    c = 1;
    m = prg_read((uint16_t)(0x87AB + y));
    w = a - m - !c;
    a = (uint8_t)w;
    n = (a) >> 7;
    mff_cycles(0x86D8, 5);
    RAM(TMP0) = a;
    if (a == 0) { mff_cycles(0x86DA, 1); goto L_86E0; }
    mff_cycles(0x86DC, 2);
    if (n) { mff_cycles(0x86DC, 1); goto L_86E0; }
    mff_cycles(0x86DE, 2);
    if (!n) { mff_cycles(0x86DE, 1); goto L_86E4; }
L_86E0:;
    mff_cycles(0x86E0, 5);
    a = 0x01;
    RAM(TMP0) = a;
L_86E4:;
    mff_cycles(0x86E4, 22);
    a = RAM(OBJ_LIFE + x);
    c = 1;
    m = RAM(TMP0);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_LIFE + x) = a;
    a = RAM(OBJ_KIND + x);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86F2, 2); goto L_8702; }
    mff_cycles(0x86F4, 8);
    a = RAM(OBJ_LIFE + x);
    m = 0x30;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x86F9, 2); goto L_8702; }
    mff_cycles(0x86FB, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8700, 1); goto L_8703; }
L_8702:;
    mff_cycles(0x8702, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8703:;
    mff_cycles(0x8703, 7);
    a = RAM(GAME_ACTION);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0x8707, 1); goto L_870D; }
    mff_cycles(0x8709, 4);
    m = 0x21;
    c = a >= m;
    if (a != m) { mff_cycles(0x870B, 1); goto L_872D; }
L_870D:;
    mff_cycles(0x870D, 17);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GRAB_TIMER) = a;
    RAM(GAME_ACTION) = a;
    RAM(GRAB_HIT_COUNT) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0717, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8718, 13);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8722, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0726, 0xCEAA, 6)) return;
    SYNC_IN;
    mff_cycles(0x8727, 9);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + x) = a;
L_872D:;
    mff_cycles(0x872D, 7);
    a = 0x0D;
    RAM(TMP0) = a;
    y = 0x00;
    for (;;) {
        mff_cycles(0x8733, 4);
        a = prg_read((uint16_t)(0x87A7 + y));
        mff_cycles(0x8736, 7);
        RAM(TMP1) = a;
        a = prg_read((uint16_t)(0x87A8 + y));
        mff_cycles(0x873B, 55);
        RAM(TMP2) = a;
        y = RAM(TMP0);
        a = RAM(OBJ_DIR + x);
        RAM(OBJ_DIR + y) = a;
        a = RAM(OBJ_Y_HI + x);
        RAM(OBJ_Y_HI + y) = a;
        a = RAM(OBJ_X + x);
        c = 0;
        m = RAM(TMP1);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(OBJ_X + y) = a;
        a = RAM(OBJ_X_HI + x);
        m = 0x00;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(OBJ_X_HI + y) = a;
        if (a == RAM(OBJ_X_HI)) {
            mff_cycles(0x8761, 83);
            a = RAM(OBJ_Y + x);
            c = 0;
            m = 0x0E;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(OBJ_LAND_Y + y) = a;
            a = RAM(OBJ_Y + x);
            c = 1;
            m = RAM(TMP2);
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            RAM(OBJ_Y + y) = a;
            a = 0xFD;
            RAM(OBJ_ANIM + y) = a;
            a = 0xFF;
            RAM(OBJ_LIFE + y) = a;
            a = 0x0D;
            RAM(OBJ_KIND + y) = a;
            a = 0x06;
            RAM(OBJ_ID + y) = a;
            RAM(OBJ_SAVED_ID + y) = a;
            a = 0x80;
            RAM(OBJ_STATE + y) = a;
            a = 0x00;
            RAM(OBJ_ANIM_TIMER + y) = a;
            a = y;
            a &= 0x03;
            RAM(OBJ_ANIM_FRAME + y) = a;
        } else {
            mff_cycles(0x875F, 1);
        }
        mff_cycles(0x879A, 19);
        y = 0x02;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        a = RAM(TMP0);
        m = 0x09;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) break;
        mff_cycles(0x8733, 1);
    }
    mff_cycles(0x87A6, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $865B (bank 6) - lifted by port/tools/lift.py */
void func_865B_b6(void) {
    REGS_IN;
    mff_cycles(0x865B, 4);
    a = prg_read((uint16_t)(0x88A8 + y));
    mff_cycles(0x865E, 2);
    if (a == 0) {
        mff_cycles(0x8660, 4);
        a = prg_read((uint16_t)(0x88CB + y));
        mff_cycles(0x8663, 2);
        if (a == 0) {
            mff_cycles(0x8665, 4);
            a = prg_read((uint16_t)(0x88EE + y));
            mff_cycles(0x8668, 2);
            if (a == 0) {
                mff_cycles(0x866A, 4);
                a = prg_read((uint16_t)(0x8911 + y));
                mff_cycles(0x866D, 2);
                if (a == 0) {
                    mff_cycles(0x866F, 4);
                    a = prg_read((uint16_t)(0x8934 + y));
                    mff_cycles(0x8672, 2);
                    if (a == 0) {
                        mff_cycles(0x8674, 4);
                        a = prg_read((uint16_t)(0x8957 + y));
                        mff_cycles(0x8677, 2);
                        if (a == 0) {
                            mff_cycles(0x8679, 4);
                            a = prg_read((uint16_t)(0x897A + y));
                            mff_cycles(0x867C, 2);
                            if (a == 0) {
                                mff_cycles(0x867E, 4);
                                a = prg_read((uint16_t)(0x899D + y));
                                mff_cycles(0x8681, 2);
                                if (a == 0) {
                                    mff_cycles(0x8683, 4);
                                    a = prg_read((uint16_t)(0x89C0 + y));
                                } else {
                                    mff_cycles(0x8681, 1);
                                }
                            } else {
                                mff_cycles(0x867C, 1);
                            }
                        } else {
                            mff_cycles(0x8677, 1);
                        }
                    } else {
                        mff_cycles(0x8672, 1);
                    }
                } else {
                    mff_cycles(0x866D, 1);
                }
            } else {
                mff_cycles(0x8668, 1);
            }
        } else {
            mff_cycles(0x8663, 1);
        }
    } else {
        mff_cycles(0x865E, 1);
    }
    mff_cycles(0x8686, 10);
    RAM(TMP0) = a;
    a = RAM(ATTACK_ID);
    if (a == 0x06) { mff_cycles(0x868C, 1); goto L_8692; }
    mff_cycles(0x868E, 4);
    if (a != 0x0C) { mff_cycles(0x8690, 1); goto L_8694; }
L_8692:;
    mff_cycles(0x8692, 3);
    RAM(COMBO_TARGET) = x;
L_8694:;
    mff_cycles(0x8694, 8);
    a = RAM(OBJ_KIND + x);
    if (a == 0x07) { mff_cycles(0x8699, 1); goto L_86B8; }
    mff_cycles(0x869B, 4);
    if (a == 0x08) { mff_cycles(0x869D, 1); goto L_86A9; }
    mff_cycles(0x869F, 4);
    if (a == 0x09) { mff_cycles(0x86A1, 1); goto L_86B8; }
    mff_cycles(0x86A3, 4);
    m = 0x0B;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x86A5, 1); goto L_86B8; }
    mff_cycles(0x86A7, 2);
    if (!z) { mff_cycles(0x86A7, 1); goto L_86E4; }
L_86A9:;
    mff_cycles(0x86A9, 4);
    if (y == 0x05) { mff_cycles(0x86AB, 1); goto L_86B1; }
    mff_cycles(0x86AD, 4);
    if (y != 0x13) { mff_cycles(0x86AF, 1); goto L_86B8; }
L_86B1:;
    mff_cycles(0x86B1, 10);
    a = RAM(TMP0);
    c = 0;
    m = 0x05;
    w = a + m + c;
    a = (uint8_t)w;
    RAM(TMP0) = a;
L_86B8:;
    mff_cycles(0x86B8, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07;
    w = a - m - !c;
    a = (uint8_t)w;
    y = a;
    if (a == 0x01) {
        mff_cycles(0x86C3, 6);
        a = RAM(OBJ_TIER + x);
        if (a != 0) {
            mff_cycles(0x86C8, 2);
            y = 0x03;
        } else {
            mff_cycles(0x86C6, 1);
        }
    } else {
        mff_cycles(0x86C1, 1);
    }
    mff_cycles(0x86CA, 7);
    a = RAM(ROUND);
    if (a == 0x04) {
        mff_cycles(0x86D0, 5);
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
    } else {
        mff_cycles(0x86CE, 1);
    }
    mff_cycles(0x86D2, 9);
    a = RAM(TMP0);
    c = 1;
    m = prg_read((uint16_t)(0x87AB + y));
    w = a - m - !c;
    a = (uint8_t)w;
    n = (a) >> 7;
    mff_cycles(0x86D8, 5);
    RAM(TMP0) = a;
    if (a == 0) { mff_cycles(0x86DA, 1); goto L_86E0; }
    mff_cycles(0x86DC, 2);
    if (n) { mff_cycles(0x86DC, 1); goto L_86E0; }
    mff_cycles(0x86DE, 2);
    if (!n) { mff_cycles(0x86DE, 1); goto L_86E4; }
L_86E0:;
    mff_cycles(0x86E0, 5);
    a = 0x01;
    RAM(TMP0) = a;
L_86E4:;
    mff_cycles(0x86E4, 22);
    a = RAM(OBJ_LIFE + x);
    c = 1;
    m = RAM(TMP0);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_LIFE + x) = a;
    a = RAM(OBJ_KIND + x);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86F2, 2); goto L_8702; }
    mff_cycles(0x86F4, 8);
    a = RAM(OBJ_LIFE + x);
    m = 0x30;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x86F9, 2); goto L_8702; }
    mff_cycles(0x86FB, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8700, 1); goto L_8703; }
L_8702:;
    mff_cycles(0x8702, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8703:;
    mff_cycles(0x8703, 7);
    a = RAM(GAME_ACTION);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0x8707, 1); goto L_870D; }
    mff_cycles(0x8709, 4);
    m = 0x21;
    c = a >= m;
    if (a != m) { mff_cycles(0x870B, 1); goto L_872D; }
L_870D:;
    mff_cycles(0x870D, 17);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GRAB_TIMER) = a;
    RAM(GAME_ACTION) = a;
    RAM(GRAB_HIT_COUNT) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0717, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8718, 13);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8722, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0726, 0xCEAA, 6)) return;
    SYNC_IN;
    mff_cycles(0x8727, 9);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + x) = a;
L_872D:;
    mff_cycles(0x872D, 7);
    a = 0x0D;
    RAM(TMP0) = a;
    y = 0x00;
    for (;;) {
        mff_cycles(0x8733, 4);
        a = prg_read((uint16_t)(0x87A7 + y));
        mff_cycles(0x8736, 7);
        RAM(TMP1) = a;
        a = prg_read((uint16_t)(0x87A8 + y));
        mff_cycles(0x873B, 55);
        RAM(TMP2) = a;
        y = RAM(TMP0);
        a = RAM(OBJ_DIR + x);
        RAM(OBJ_DIR + y) = a;
        a = RAM(OBJ_Y_HI + x);
        RAM(OBJ_Y_HI + y) = a;
        a = RAM(OBJ_X + x);
        c = 0;
        m = RAM(TMP1);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(OBJ_X + y) = a;
        a = RAM(OBJ_X_HI + x);
        m = 0x00;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(OBJ_X_HI + y) = a;
        if (a == RAM(OBJ_X_HI)) {
            mff_cycles(0x8761, 83);
            a = RAM(OBJ_Y + x);
            c = 0;
            m = 0x0E;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(OBJ_LAND_Y + y) = a;
            a = RAM(OBJ_Y + x);
            c = 1;
            m = RAM(TMP2);
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            RAM(OBJ_Y + y) = a;
            a = 0xFD;
            RAM(OBJ_ANIM + y) = a;
            a = 0xFF;
            RAM(OBJ_LIFE + y) = a;
            a = 0x0D;
            RAM(OBJ_KIND + y) = a;
            a = 0x06;
            RAM(OBJ_ID + y) = a;
            RAM(OBJ_SAVED_ID + y) = a;
            a = 0x80;
            RAM(OBJ_STATE + y) = a;
            a = 0x00;
            RAM(OBJ_ANIM_TIMER + y) = a;
            a = y;
            a &= 0x03;
            RAM(OBJ_ANIM_FRAME + y) = a;
        } else {
            mff_cycles(0x875F, 1);
        }
        mff_cycles(0x879A, 19);
        y = 0x02;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        a = RAM(TMP0);
        m = 0x09;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) break;
        mff_cycles(0x8733, 1);
    }
    mff_cycles(0x87A6, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8660 (bank 6) - lifted by port/tools/lift.py */
void func_8660_b6(void) {
    REGS_IN;
    mff_cycles(0x8660, 4);
    a = prg_read((uint16_t)(0x88CB + y));
    mff_cycles(0x8663, 2);
    if (a == 0) {
        mff_cycles(0x8665, 4);
        a = prg_read((uint16_t)(0x88EE + y));
        mff_cycles(0x8668, 2);
        if (a == 0) {
            mff_cycles(0x866A, 4);
            a = prg_read((uint16_t)(0x8911 + y));
            mff_cycles(0x866D, 2);
            if (a == 0) {
                mff_cycles(0x866F, 4);
                a = prg_read((uint16_t)(0x8934 + y));
                mff_cycles(0x8672, 2);
                if (a == 0) {
                    mff_cycles(0x8674, 4);
                    a = prg_read((uint16_t)(0x8957 + y));
                    mff_cycles(0x8677, 2);
                    if (a == 0) {
                        mff_cycles(0x8679, 4);
                        a = prg_read((uint16_t)(0x897A + y));
                        mff_cycles(0x867C, 2);
                        if (a == 0) {
                            mff_cycles(0x867E, 4);
                            a = prg_read((uint16_t)(0x899D + y));
                            mff_cycles(0x8681, 2);
                            if (a == 0) {
                                mff_cycles(0x8683, 4);
                                a = prg_read((uint16_t)(0x89C0 + y));
                            } else {
                                mff_cycles(0x8681, 1);
                            }
                        } else {
                            mff_cycles(0x867C, 1);
                        }
                    } else {
                        mff_cycles(0x8677, 1);
                    }
                } else {
                    mff_cycles(0x8672, 1);
                }
            } else {
                mff_cycles(0x866D, 1);
            }
        } else {
            mff_cycles(0x8668, 1);
        }
    } else {
        mff_cycles(0x8663, 1);
    }
    mff_cycles(0x8686, 10);
    RAM(TMP0) = a;
    a = RAM(ATTACK_ID);
    if (a == 0x06) { mff_cycles(0x868C, 1); goto L_8692; }
    mff_cycles(0x868E, 4);
    if (a != 0x0C) { mff_cycles(0x8690, 1); goto L_8694; }
L_8692:;
    mff_cycles(0x8692, 3);
    RAM(COMBO_TARGET) = x;
L_8694:;
    mff_cycles(0x8694, 8);
    a = RAM(OBJ_KIND + x);
    if (a == 0x07) { mff_cycles(0x8699, 1); goto L_86B8; }
    mff_cycles(0x869B, 4);
    if (a == 0x08) { mff_cycles(0x869D, 1); goto L_86A9; }
    mff_cycles(0x869F, 4);
    if (a == 0x09) { mff_cycles(0x86A1, 1); goto L_86B8; }
    mff_cycles(0x86A3, 4);
    m = 0x0B;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x86A5, 1); goto L_86B8; }
    mff_cycles(0x86A7, 2);
    if (!z) { mff_cycles(0x86A7, 1); goto L_86E4; }
L_86A9:;
    mff_cycles(0x86A9, 4);
    if (y == 0x05) { mff_cycles(0x86AB, 1); goto L_86B1; }
    mff_cycles(0x86AD, 4);
    if (y != 0x13) { mff_cycles(0x86AF, 1); goto L_86B8; }
L_86B1:;
    mff_cycles(0x86B1, 10);
    a = RAM(TMP0);
    c = 0;
    m = 0x05;
    w = a + m + c;
    a = (uint8_t)w;
    RAM(TMP0) = a;
L_86B8:;
    mff_cycles(0x86B8, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07;
    w = a - m - !c;
    a = (uint8_t)w;
    y = a;
    if (a == 0x01) {
        mff_cycles(0x86C3, 6);
        a = RAM(OBJ_TIER + x);
        if (a != 0) {
            mff_cycles(0x86C8, 2);
            y = 0x03;
        } else {
            mff_cycles(0x86C6, 1);
        }
    } else {
        mff_cycles(0x86C1, 1);
    }
    mff_cycles(0x86CA, 7);
    a = RAM(ROUND);
    if (a == 0x04) {
        mff_cycles(0x86D0, 5);
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
    } else {
        mff_cycles(0x86CE, 1);
    }
    mff_cycles(0x86D2, 9);
    a = RAM(TMP0);
    c = 1;
    m = prg_read((uint16_t)(0x87AB + y));
    w = a - m - !c;
    a = (uint8_t)w;
    n = (a) >> 7;
    mff_cycles(0x86D8, 5);
    RAM(TMP0) = a;
    if (a == 0) { mff_cycles(0x86DA, 1); goto L_86E0; }
    mff_cycles(0x86DC, 2);
    if (n) { mff_cycles(0x86DC, 1); goto L_86E0; }
    mff_cycles(0x86DE, 2);
    if (!n) { mff_cycles(0x86DE, 1); goto L_86E4; }
L_86E0:;
    mff_cycles(0x86E0, 5);
    a = 0x01;
    RAM(TMP0) = a;
L_86E4:;
    mff_cycles(0x86E4, 22);
    a = RAM(OBJ_LIFE + x);
    c = 1;
    m = RAM(TMP0);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_LIFE + x) = a;
    a = RAM(OBJ_KIND + x);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86F2, 2); goto L_8702; }
    mff_cycles(0x86F4, 8);
    a = RAM(OBJ_LIFE + x);
    m = 0x30;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x86F9, 2); goto L_8702; }
    mff_cycles(0x86FB, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8700, 1); goto L_8703; }
L_8702:;
    mff_cycles(0x8702, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8703:;
    mff_cycles(0x8703, 7);
    a = RAM(GAME_ACTION);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0x8707, 1); goto L_870D; }
    mff_cycles(0x8709, 4);
    m = 0x21;
    c = a >= m;
    if (a != m) { mff_cycles(0x870B, 1); goto L_872D; }
L_870D:;
    mff_cycles(0x870D, 17);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GRAB_TIMER) = a;
    RAM(GAME_ACTION) = a;
    RAM(GRAB_HIT_COUNT) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0717, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8718, 13);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8722, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0726, 0xCEAA, 6)) return;
    SYNC_IN;
    mff_cycles(0x8727, 9);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + x) = a;
L_872D:;
    mff_cycles(0x872D, 7);
    a = 0x0D;
    RAM(TMP0) = a;
    y = 0x00;
    for (;;) {
        mff_cycles(0x8733, 4);
        a = prg_read((uint16_t)(0x87A7 + y));
        mff_cycles(0x8736, 7);
        RAM(TMP1) = a;
        a = prg_read((uint16_t)(0x87A8 + y));
        mff_cycles(0x873B, 55);
        RAM(TMP2) = a;
        y = RAM(TMP0);
        a = RAM(OBJ_DIR + x);
        RAM(OBJ_DIR + y) = a;
        a = RAM(OBJ_Y_HI + x);
        RAM(OBJ_Y_HI + y) = a;
        a = RAM(OBJ_X + x);
        c = 0;
        m = RAM(TMP1);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(OBJ_X + y) = a;
        a = RAM(OBJ_X_HI + x);
        m = 0x00;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(OBJ_X_HI + y) = a;
        if (a == RAM(OBJ_X_HI)) {
            mff_cycles(0x8761, 83);
            a = RAM(OBJ_Y + x);
            c = 0;
            m = 0x0E;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(OBJ_LAND_Y + y) = a;
            a = RAM(OBJ_Y + x);
            c = 1;
            m = RAM(TMP2);
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            RAM(OBJ_Y + y) = a;
            a = 0xFD;
            RAM(OBJ_ANIM + y) = a;
            a = 0xFF;
            RAM(OBJ_LIFE + y) = a;
            a = 0x0D;
            RAM(OBJ_KIND + y) = a;
            a = 0x06;
            RAM(OBJ_ID + y) = a;
            RAM(OBJ_SAVED_ID + y) = a;
            a = 0x80;
            RAM(OBJ_STATE + y) = a;
            a = 0x00;
            RAM(OBJ_ANIM_TIMER + y) = a;
            a = y;
            a &= 0x03;
            RAM(OBJ_ANIM_FRAME + y) = a;
        } else {
            mff_cycles(0x875F, 1);
        }
        mff_cycles(0x879A, 19);
        y = 0x02;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        a = RAM(TMP0);
        m = 0x09;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) break;
        mff_cycles(0x8733, 1);
    }
    mff_cycles(0x87A6, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8665 (bank 6) - lifted by port/tools/lift.py */
void func_8665_b6(void) {
    REGS_IN;
    mff_cycles(0x8665, 4);
    a = prg_read((uint16_t)(0x88EE + y));
    mff_cycles(0x8668, 2);
    if (a == 0) {
        mff_cycles(0x866A, 4);
        a = prg_read((uint16_t)(0x8911 + y));
        mff_cycles(0x866D, 2);
        if (a == 0) {
            mff_cycles(0x866F, 4);
            a = prg_read((uint16_t)(0x8934 + y));
            mff_cycles(0x8672, 2);
            if (a == 0) {
                mff_cycles(0x8674, 4);
                a = prg_read((uint16_t)(0x8957 + y));
                mff_cycles(0x8677, 2);
                if (a == 0) {
                    mff_cycles(0x8679, 4);
                    a = prg_read((uint16_t)(0x897A + y));
                    mff_cycles(0x867C, 2);
                    if (a == 0) {
                        mff_cycles(0x867E, 4);
                        a = prg_read((uint16_t)(0x899D + y));
                        mff_cycles(0x8681, 2);
                        if (a == 0) {
                            mff_cycles(0x8683, 4);
                            a = prg_read((uint16_t)(0x89C0 + y));
                        } else {
                            mff_cycles(0x8681, 1);
                        }
                    } else {
                        mff_cycles(0x867C, 1);
                    }
                } else {
                    mff_cycles(0x8677, 1);
                }
            } else {
                mff_cycles(0x8672, 1);
            }
        } else {
            mff_cycles(0x866D, 1);
        }
    } else {
        mff_cycles(0x8668, 1);
    }
    mff_cycles(0x8686, 10);
    RAM(TMP0) = a;
    a = RAM(ATTACK_ID);
    if (a == 0x06) { mff_cycles(0x868C, 1); goto L_8692; }
    mff_cycles(0x868E, 4);
    if (a != 0x0C) { mff_cycles(0x8690, 1); goto L_8694; }
L_8692:;
    mff_cycles(0x8692, 3);
    RAM(COMBO_TARGET) = x;
L_8694:;
    mff_cycles(0x8694, 8);
    a = RAM(OBJ_KIND + x);
    if (a == 0x07) { mff_cycles(0x8699, 1); goto L_86B8; }
    mff_cycles(0x869B, 4);
    if (a == 0x08) { mff_cycles(0x869D, 1); goto L_86A9; }
    mff_cycles(0x869F, 4);
    if (a == 0x09) { mff_cycles(0x86A1, 1); goto L_86B8; }
    mff_cycles(0x86A3, 4);
    m = 0x0B;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x86A5, 1); goto L_86B8; }
    mff_cycles(0x86A7, 2);
    if (!z) { mff_cycles(0x86A7, 1); goto L_86E4; }
L_86A9:;
    mff_cycles(0x86A9, 4);
    if (y == 0x05) { mff_cycles(0x86AB, 1); goto L_86B1; }
    mff_cycles(0x86AD, 4);
    if (y != 0x13) { mff_cycles(0x86AF, 1); goto L_86B8; }
L_86B1:;
    mff_cycles(0x86B1, 10);
    a = RAM(TMP0);
    c = 0;
    m = 0x05;
    w = a + m + c;
    a = (uint8_t)w;
    RAM(TMP0) = a;
L_86B8:;
    mff_cycles(0x86B8, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07;
    w = a - m - !c;
    a = (uint8_t)w;
    y = a;
    if (a == 0x01) {
        mff_cycles(0x86C3, 6);
        a = RAM(OBJ_TIER + x);
        if (a != 0) {
            mff_cycles(0x86C8, 2);
            y = 0x03;
        } else {
            mff_cycles(0x86C6, 1);
        }
    } else {
        mff_cycles(0x86C1, 1);
    }
    mff_cycles(0x86CA, 7);
    a = RAM(ROUND);
    if (a == 0x04) {
        mff_cycles(0x86D0, 5);
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
    } else {
        mff_cycles(0x86CE, 1);
    }
    mff_cycles(0x86D2, 9);
    a = RAM(TMP0);
    c = 1;
    m = prg_read((uint16_t)(0x87AB + y));
    w = a - m - !c;
    a = (uint8_t)w;
    n = (a) >> 7;
    mff_cycles(0x86D8, 5);
    RAM(TMP0) = a;
    if (a == 0) { mff_cycles(0x86DA, 1); goto L_86E0; }
    mff_cycles(0x86DC, 2);
    if (n) { mff_cycles(0x86DC, 1); goto L_86E0; }
    mff_cycles(0x86DE, 2);
    if (!n) { mff_cycles(0x86DE, 1); goto L_86E4; }
L_86E0:;
    mff_cycles(0x86E0, 5);
    a = 0x01;
    RAM(TMP0) = a;
L_86E4:;
    mff_cycles(0x86E4, 22);
    a = RAM(OBJ_LIFE + x);
    c = 1;
    m = RAM(TMP0);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_LIFE + x) = a;
    a = RAM(OBJ_KIND + x);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86F2, 2); goto L_8702; }
    mff_cycles(0x86F4, 8);
    a = RAM(OBJ_LIFE + x);
    m = 0x30;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x86F9, 2); goto L_8702; }
    mff_cycles(0x86FB, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8700, 1); goto L_8703; }
L_8702:;
    mff_cycles(0x8702, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8703:;
    mff_cycles(0x8703, 7);
    a = RAM(GAME_ACTION);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0x8707, 1); goto L_870D; }
    mff_cycles(0x8709, 4);
    m = 0x21;
    c = a >= m;
    if (a != m) { mff_cycles(0x870B, 1); goto L_872D; }
L_870D:;
    mff_cycles(0x870D, 17);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GRAB_TIMER) = a;
    RAM(GAME_ACTION) = a;
    RAM(GRAB_HIT_COUNT) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0717, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8718, 13);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8722, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0726, 0xCEAA, 6)) return;
    SYNC_IN;
    mff_cycles(0x8727, 9);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + x) = a;
L_872D:;
    mff_cycles(0x872D, 7);
    a = 0x0D;
    RAM(TMP0) = a;
    y = 0x00;
    for (;;) {
        mff_cycles(0x8733, 4);
        a = prg_read((uint16_t)(0x87A7 + y));
        mff_cycles(0x8736, 7);
        RAM(TMP1) = a;
        a = prg_read((uint16_t)(0x87A8 + y));
        mff_cycles(0x873B, 55);
        RAM(TMP2) = a;
        y = RAM(TMP0);
        a = RAM(OBJ_DIR + x);
        RAM(OBJ_DIR + y) = a;
        a = RAM(OBJ_Y_HI + x);
        RAM(OBJ_Y_HI + y) = a;
        a = RAM(OBJ_X + x);
        c = 0;
        m = RAM(TMP1);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(OBJ_X + y) = a;
        a = RAM(OBJ_X_HI + x);
        m = 0x00;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(OBJ_X_HI + y) = a;
        if (a == RAM(OBJ_X_HI)) {
            mff_cycles(0x8761, 83);
            a = RAM(OBJ_Y + x);
            c = 0;
            m = 0x0E;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(OBJ_LAND_Y + y) = a;
            a = RAM(OBJ_Y + x);
            c = 1;
            m = RAM(TMP2);
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            RAM(OBJ_Y + y) = a;
            a = 0xFD;
            RAM(OBJ_ANIM + y) = a;
            a = 0xFF;
            RAM(OBJ_LIFE + y) = a;
            a = 0x0D;
            RAM(OBJ_KIND + y) = a;
            a = 0x06;
            RAM(OBJ_ID + y) = a;
            RAM(OBJ_SAVED_ID + y) = a;
            a = 0x80;
            RAM(OBJ_STATE + y) = a;
            a = 0x00;
            RAM(OBJ_ANIM_TIMER + y) = a;
            a = y;
            a &= 0x03;
            RAM(OBJ_ANIM_FRAME + y) = a;
        } else {
            mff_cycles(0x875F, 1);
        }
        mff_cycles(0x879A, 19);
        y = 0x02;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        a = RAM(TMP0);
        m = 0x09;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) break;
        mff_cycles(0x8733, 1);
    }
    mff_cycles(0x87A6, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $866A (bank 6) - lifted by port/tools/lift.py */
void func_866A_b6(void) {
    REGS_IN;
    mff_cycles(0x866A, 4);
    a = prg_read((uint16_t)(0x8911 + y));
    mff_cycles(0x866D, 2);
    if (a == 0) {
        mff_cycles(0x866F, 4);
        a = prg_read((uint16_t)(0x8934 + y));
        mff_cycles(0x8672, 2);
        if (a == 0) {
            mff_cycles(0x8674, 4);
            a = prg_read((uint16_t)(0x8957 + y));
            mff_cycles(0x8677, 2);
            if (a == 0) {
                mff_cycles(0x8679, 4);
                a = prg_read((uint16_t)(0x897A + y));
                mff_cycles(0x867C, 2);
                if (a == 0) {
                    mff_cycles(0x867E, 4);
                    a = prg_read((uint16_t)(0x899D + y));
                    mff_cycles(0x8681, 2);
                    if (a == 0) {
                        mff_cycles(0x8683, 4);
                        a = prg_read((uint16_t)(0x89C0 + y));
                    } else {
                        mff_cycles(0x8681, 1);
                    }
                } else {
                    mff_cycles(0x867C, 1);
                }
            } else {
                mff_cycles(0x8677, 1);
            }
        } else {
            mff_cycles(0x8672, 1);
        }
    } else {
        mff_cycles(0x866D, 1);
    }
    mff_cycles(0x8686, 10);
    RAM(TMP0) = a;
    a = RAM(ATTACK_ID);
    if (a == 0x06) { mff_cycles(0x868C, 1); goto L_8692; }
    mff_cycles(0x868E, 4);
    if (a != 0x0C) { mff_cycles(0x8690, 1); goto L_8694; }
L_8692:;
    mff_cycles(0x8692, 3);
    RAM(COMBO_TARGET) = x;
L_8694:;
    mff_cycles(0x8694, 8);
    a = RAM(OBJ_KIND + x);
    if (a == 0x07) { mff_cycles(0x8699, 1); goto L_86B8; }
    mff_cycles(0x869B, 4);
    if (a == 0x08) { mff_cycles(0x869D, 1); goto L_86A9; }
    mff_cycles(0x869F, 4);
    if (a == 0x09) { mff_cycles(0x86A1, 1); goto L_86B8; }
    mff_cycles(0x86A3, 4);
    m = 0x0B;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x86A5, 1); goto L_86B8; }
    mff_cycles(0x86A7, 2);
    if (!z) { mff_cycles(0x86A7, 1); goto L_86E4; }
L_86A9:;
    mff_cycles(0x86A9, 4);
    if (y == 0x05) { mff_cycles(0x86AB, 1); goto L_86B1; }
    mff_cycles(0x86AD, 4);
    if (y != 0x13) { mff_cycles(0x86AF, 1); goto L_86B8; }
L_86B1:;
    mff_cycles(0x86B1, 10);
    a = RAM(TMP0);
    c = 0;
    m = 0x05;
    w = a + m + c;
    a = (uint8_t)w;
    RAM(TMP0) = a;
L_86B8:;
    mff_cycles(0x86B8, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07;
    w = a - m - !c;
    a = (uint8_t)w;
    y = a;
    if (a == 0x01) {
        mff_cycles(0x86C3, 6);
        a = RAM(OBJ_TIER + x);
        if (a != 0) {
            mff_cycles(0x86C8, 2);
            y = 0x03;
        } else {
            mff_cycles(0x86C6, 1);
        }
    } else {
        mff_cycles(0x86C1, 1);
    }
    mff_cycles(0x86CA, 7);
    a = RAM(ROUND);
    if (a == 0x04) {
        mff_cycles(0x86D0, 5);
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
    } else {
        mff_cycles(0x86CE, 1);
    }
    mff_cycles(0x86D2, 9);
    a = RAM(TMP0);
    c = 1;
    m = prg_read((uint16_t)(0x87AB + y));
    w = a - m - !c;
    a = (uint8_t)w;
    n = (a) >> 7;
    mff_cycles(0x86D8, 5);
    RAM(TMP0) = a;
    if (a == 0) { mff_cycles(0x86DA, 1); goto L_86E0; }
    mff_cycles(0x86DC, 2);
    if (n) { mff_cycles(0x86DC, 1); goto L_86E0; }
    mff_cycles(0x86DE, 2);
    if (!n) { mff_cycles(0x86DE, 1); goto L_86E4; }
L_86E0:;
    mff_cycles(0x86E0, 5);
    a = 0x01;
    RAM(TMP0) = a;
L_86E4:;
    mff_cycles(0x86E4, 22);
    a = RAM(OBJ_LIFE + x);
    c = 1;
    m = RAM(TMP0);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_LIFE + x) = a;
    a = RAM(OBJ_KIND + x);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86F2, 2); goto L_8702; }
    mff_cycles(0x86F4, 8);
    a = RAM(OBJ_LIFE + x);
    m = 0x30;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x86F9, 2); goto L_8702; }
    mff_cycles(0x86FB, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8700, 1); goto L_8703; }
L_8702:;
    mff_cycles(0x8702, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8703:;
    mff_cycles(0x8703, 7);
    a = RAM(GAME_ACTION);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0x8707, 1); goto L_870D; }
    mff_cycles(0x8709, 4);
    m = 0x21;
    c = a >= m;
    if (a != m) { mff_cycles(0x870B, 1); goto L_872D; }
L_870D:;
    mff_cycles(0x870D, 17);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GRAB_TIMER) = a;
    RAM(GAME_ACTION) = a;
    RAM(GRAB_HIT_COUNT) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0717, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8718, 13);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8722, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0726, 0xCEAA, 6)) return;
    SYNC_IN;
    mff_cycles(0x8727, 9);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + x) = a;
L_872D:;
    mff_cycles(0x872D, 7);
    a = 0x0D;
    RAM(TMP0) = a;
    y = 0x00;
    for (;;) {
        mff_cycles(0x8733, 4);
        a = prg_read((uint16_t)(0x87A7 + y));
        mff_cycles(0x8736, 7);
        RAM(TMP1) = a;
        a = prg_read((uint16_t)(0x87A8 + y));
        mff_cycles(0x873B, 55);
        RAM(TMP2) = a;
        y = RAM(TMP0);
        a = RAM(OBJ_DIR + x);
        RAM(OBJ_DIR + y) = a;
        a = RAM(OBJ_Y_HI + x);
        RAM(OBJ_Y_HI + y) = a;
        a = RAM(OBJ_X + x);
        c = 0;
        m = RAM(TMP1);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(OBJ_X + y) = a;
        a = RAM(OBJ_X_HI + x);
        m = 0x00;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(OBJ_X_HI + y) = a;
        if (a == RAM(OBJ_X_HI)) {
            mff_cycles(0x8761, 83);
            a = RAM(OBJ_Y + x);
            c = 0;
            m = 0x0E;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(OBJ_LAND_Y + y) = a;
            a = RAM(OBJ_Y + x);
            c = 1;
            m = RAM(TMP2);
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            RAM(OBJ_Y + y) = a;
            a = 0xFD;
            RAM(OBJ_ANIM + y) = a;
            a = 0xFF;
            RAM(OBJ_LIFE + y) = a;
            a = 0x0D;
            RAM(OBJ_KIND + y) = a;
            a = 0x06;
            RAM(OBJ_ID + y) = a;
            RAM(OBJ_SAVED_ID + y) = a;
            a = 0x80;
            RAM(OBJ_STATE + y) = a;
            a = 0x00;
            RAM(OBJ_ANIM_TIMER + y) = a;
            a = y;
            a &= 0x03;
            RAM(OBJ_ANIM_FRAME + y) = a;
        } else {
            mff_cycles(0x875F, 1);
        }
        mff_cycles(0x879A, 19);
        y = 0x02;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        a = RAM(TMP0);
        m = 0x09;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) break;
        mff_cycles(0x8733, 1);
    }
    mff_cycles(0x87A6, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $866F (bank 6) - lifted by port/tools/lift.py */
void func_866F_b6(void) {
    REGS_IN;
    mff_cycles(0x866F, 4);
    a = prg_read((uint16_t)(0x8934 + y));
    mff_cycles(0x8672, 2);
    if (a == 0) {
        mff_cycles(0x8674, 4);
        a = prg_read((uint16_t)(0x8957 + y));
        mff_cycles(0x8677, 2);
        if (a == 0) {
            mff_cycles(0x8679, 4);
            a = prg_read((uint16_t)(0x897A + y));
            mff_cycles(0x867C, 2);
            if (a == 0) {
                mff_cycles(0x867E, 4);
                a = prg_read((uint16_t)(0x899D + y));
                mff_cycles(0x8681, 2);
                if (a == 0) {
                    mff_cycles(0x8683, 4);
                    a = prg_read((uint16_t)(0x89C0 + y));
                } else {
                    mff_cycles(0x8681, 1);
                }
            } else {
                mff_cycles(0x867C, 1);
            }
        } else {
            mff_cycles(0x8677, 1);
        }
    } else {
        mff_cycles(0x8672, 1);
    }
    mff_cycles(0x8686, 10);
    RAM(TMP0) = a;
    a = RAM(ATTACK_ID);
    if (a == 0x06) { mff_cycles(0x868C, 1); goto L_8692; }
    mff_cycles(0x868E, 4);
    if (a != 0x0C) { mff_cycles(0x8690, 1); goto L_8694; }
L_8692:;
    mff_cycles(0x8692, 3);
    RAM(COMBO_TARGET) = x;
L_8694:;
    mff_cycles(0x8694, 8);
    a = RAM(OBJ_KIND + x);
    if (a == 0x07) { mff_cycles(0x8699, 1); goto L_86B8; }
    mff_cycles(0x869B, 4);
    if (a == 0x08) { mff_cycles(0x869D, 1); goto L_86A9; }
    mff_cycles(0x869F, 4);
    if (a == 0x09) { mff_cycles(0x86A1, 1); goto L_86B8; }
    mff_cycles(0x86A3, 4);
    m = 0x0B;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x86A5, 1); goto L_86B8; }
    mff_cycles(0x86A7, 2);
    if (!z) { mff_cycles(0x86A7, 1); goto L_86E4; }
L_86A9:;
    mff_cycles(0x86A9, 4);
    if (y == 0x05) { mff_cycles(0x86AB, 1); goto L_86B1; }
    mff_cycles(0x86AD, 4);
    if (y != 0x13) { mff_cycles(0x86AF, 1); goto L_86B8; }
L_86B1:;
    mff_cycles(0x86B1, 10);
    a = RAM(TMP0);
    c = 0;
    m = 0x05;
    w = a + m + c;
    a = (uint8_t)w;
    RAM(TMP0) = a;
L_86B8:;
    mff_cycles(0x86B8, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07;
    w = a - m - !c;
    a = (uint8_t)w;
    y = a;
    if (a == 0x01) {
        mff_cycles(0x86C3, 6);
        a = RAM(OBJ_TIER + x);
        if (a != 0) {
            mff_cycles(0x86C8, 2);
            y = 0x03;
        } else {
            mff_cycles(0x86C6, 1);
        }
    } else {
        mff_cycles(0x86C1, 1);
    }
    mff_cycles(0x86CA, 7);
    a = RAM(ROUND);
    if (a == 0x04) {
        mff_cycles(0x86D0, 5);
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
    } else {
        mff_cycles(0x86CE, 1);
    }
    mff_cycles(0x86D2, 9);
    a = RAM(TMP0);
    c = 1;
    m = prg_read((uint16_t)(0x87AB + y));
    w = a - m - !c;
    a = (uint8_t)w;
    n = (a) >> 7;
    mff_cycles(0x86D8, 5);
    RAM(TMP0) = a;
    if (a == 0) { mff_cycles(0x86DA, 1); goto L_86E0; }
    mff_cycles(0x86DC, 2);
    if (n) { mff_cycles(0x86DC, 1); goto L_86E0; }
    mff_cycles(0x86DE, 2);
    if (!n) { mff_cycles(0x86DE, 1); goto L_86E4; }
L_86E0:;
    mff_cycles(0x86E0, 5);
    a = 0x01;
    RAM(TMP0) = a;
L_86E4:;
    mff_cycles(0x86E4, 22);
    a = RAM(OBJ_LIFE + x);
    c = 1;
    m = RAM(TMP0);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_LIFE + x) = a;
    a = RAM(OBJ_KIND + x);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86F2, 2); goto L_8702; }
    mff_cycles(0x86F4, 8);
    a = RAM(OBJ_LIFE + x);
    m = 0x30;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x86F9, 2); goto L_8702; }
    mff_cycles(0x86FB, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8700, 1); goto L_8703; }
L_8702:;
    mff_cycles(0x8702, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8703:;
    mff_cycles(0x8703, 7);
    a = RAM(GAME_ACTION);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0x8707, 1); goto L_870D; }
    mff_cycles(0x8709, 4);
    m = 0x21;
    c = a >= m;
    if (a != m) { mff_cycles(0x870B, 1); goto L_872D; }
L_870D:;
    mff_cycles(0x870D, 17);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GRAB_TIMER) = a;
    RAM(GAME_ACTION) = a;
    RAM(GRAB_HIT_COUNT) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0717, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8718, 13);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8722, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0726, 0xCEAA, 6)) return;
    SYNC_IN;
    mff_cycles(0x8727, 9);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + x) = a;
L_872D:;
    mff_cycles(0x872D, 7);
    a = 0x0D;
    RAM(TMP0) = a;
    y = 0x00;
    for (;;) {
        mff_cycles(0x8733, 4);
        a = prg_read((uint16_t)(0x87A7 + y));
        mff_cycles(0x8736, 7);
        RAM(TMP1) = a;
        a = prg_read((uint16_t)(0x87A8 + y));
        mff_cycles(0x873B, 55);
        RAM(TMP2) = a;
        y = RAM(TMP0);
        a = RAM(OBJ_DIR + x);
        RAM(OBJ_DIR + y) = a;
        a = RAM(OBJ_Y_HI + x);
        RAM(OBJ_Y_HI + y) = a;
        a = RAM(OBJ_X + x);
        c = 0;
        m = RAM(TMP1);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(OBJ_X + y) = a;
        a = RAM(OBJ_X_HI + x);
        m = 0x00;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(OBJ_X_HI + y) = a;
        if (a == RAM(OBJ_X_HI)) {
            mff_cycles(0x8761, 83);
            a = RAM(OBJ_Y + x);
            c = 0;
            m = 0x0E;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(OBJ_LAND_Y + y) = a;
            a = RAM(OBJ_Y + x);
            c = 1;
            m = RAM(TMP2);
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            RAM(OBJ_Y + y) = a;
            a = 0xFD;
            RAM(OBJ_ANIM + y) = a;
            a = 0xFF;
            RAM(OBJ_LIFE + y) = a;
            a = 0x0D;
            RAM(OBJ_KIND + y) = a;
            a = 0x06;
            RAM(OBJ_ID + y) = a;
            RAM(OBJ_SAVED_ID + y) = a;
            a = 0x80;
            RAM(OBJ_STATE + y) = a;
            a = 0x00;
            RAM(OBJ_ANIM_TIMER + y) = a;
            a = y;
            a &= 0x03;
            RAM(OBJ_ANIM_FRAME + y) = a;
        } else {
            mff_cycles(0x875F, 1);
        }
        mff_cycles(0x879A, 19);
        y = 0x02;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        a = RAM(TMP0);
        m = 0x09;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) break;
        mff_cycles(0x8733, 1);
    }
    mff_cycles(0x87A6, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8674 (bank 6) - lifted by port/tools/lift.py */
void func_8674_b6(void) {
    REGS_IN;
    mff_cycles(0x8674, 4);
    a = prg_read((uint16_t)(0x8957 + y));
    mff_cycles(0x8677, 2);
    if (a == 0) {
        mff_cycles(0x8679, 4);
        a = prg_read((uint16_t)(0x897A + y));
        mff_cycles(0x867C, 2);
        if (a == 0) {
            mff_cycles(0x867E, 4);
            a = prg_read((uint16_t)(0x899D + y));
            mff_cycles(0x8681, 2);
            if (a == 0) {
                mff_cycles(0x8683, 4);
                a = prg_read((uint16_t)(0x89C0 + y));
            } else {
                mff_cycles(0x8681, 1);
            }
        } else {
            mff_cycles(0x867C, 1);
        }
    } else {
        mff_cycles(0x8677, 1);
    }
    mff_cycles(0x8686, 10);
    RAM(TMP0) = a;
    a = RAM(ATTACK_ID);
    if (a == 0x06) { mff_cycles(0x868C, 1); goto L_8692; }
    mff_cycles(0x868E, 4);
    if (a != 0x0C) { mff_cycles(0x8690, 1); goto L_8694; }
L_8692:;
    mff_cycles(0x8692, 3);
    RAM(COMBO_TARGET) = x;
L_8694:;
    mff_cycles(0x8694, 8);
    a = RAM(OBJ_KIND + x);
    if (a == 0x07) { mff_cycles(0x8699, 1); goto L_86B8; }
    mff_cycles(0x869B, 4);
    if (a == 0x08) { mff_cycles(0x869D, 1); goto L_86A9; }
    mff_cycles(0x869F, 4);
    if (a == 0x09) { mff_cycles(0x86A1, 1); goto L_86B8; }
    mff_cycles(0x86A3, 4);
    m = 0x0B;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x86A5, 1); goto L_86B8; }
    mff_cycles(0x86A7, 2);
    if (!z) { mff_cycles(0x86A7, 1); goto L_86E4; }
L_86A9:;
    mff_cycles(0x86A9, 4);
    if (y == 0x05) { mff_cycles(0x86AB, 1); goto L_86B1; }
    mff_cycles(0x86AD, 4);
    if (y != 0x13) { mff_cycles(0x86AF, 1); goto L_86B8; }
L_86B1:;
    mff_cycles(0x86B1, 10);
    a = RAM(TMP0);
    c = 0;
    m = 0x05;
    w = a + m + c;
    a = (uint8_t)w;
    RAM(TMP0) = a;
L_86B8:;
    mff_cycles(0x86B8, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07;
    w = a - m - !c;
    a = (uint8_t)w;
    y = a;
    if (a == 0x01) {
        mff_cycles(0x86C3, 6);
        a = RAM(OBJ_TIER + x);
        if (a != 0) {
            mff_cycles(0x86C8, 2);
            y = 0x03;
        } else {
            mff_cycles(0x86C6, 1);
        }
    } else {
        mff_cycles(0x86C1, 1);
    }
    mff_cycles(0x86CA, 7);
    a = RAM(ROUND);
    if (a == 0x04) {
        mff_cycles(0x86D0, 5);
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
    } else {
        mff_cycles(0x86CE, 1);
    }
    mff_cycles(0x86D2, 9);
    a = RAM(TMP0);
    c = 1;
    m = prg_read((uint16_t)(0x87AB + y));
    w = a - m - !c;
    a = (uint8_t)w;
    n = (a) >> 7;
    mff_cycles(0x86D8, 5);
    RAM(TMP0) = a;
    if (a == 0) { mff_cycles(0x86DA, 1); goto L_86E0; }
    mff_cycles(0x86DC, 2);
    if (n) { mff_cycles(0x86DC, 1); goto L_86E0; }
    mff_cycles(0x86DE, 2);
    if (!n) { mff_cycles(0x86DE, 1); goto L_86E4; }
L_86E0:;
    mff_cycles(0x86E0, 5);
    a = 0x01;
    RAM(TMP0) = a;
L_86E4:;
    mff_cycles(0x86E4, 22);
    a = RAM(OBJ_LIFE + x);
    c = 1;
    m = RAM(TMP0);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_LIFE + x) = a;
    a = RAM(OBJ_KIND + x);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86F2, 2); goto L_8702; }
    mff_cycles(0x86F4, 8);
    a = RAM(OBJ_LIFE + x);
    m = 0x30;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x86F9, 2); goto L_8702; }
    mff_cycles(0x86FB, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8700, 1); goto L_8703; }
L_8702:;
    mff_cycles(0x8702, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8703:;
    mff_cycles(0x8703, 7);
    a = RAM(GAME_ACTION);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0x8707, 1); goto L_870D; }
    mff_cycles(0x8709, 4);
    m = 0x21;
    c = a >= m;
    if (a != m) { mff_cycles(0x870B, 1); goto L_872D; }
L_870D:;
    mff_cycles(0x870D, 17);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GRAB_TIMER) = a;
    RAM(GAME_ACTION) = a;
    RAM(GRAB_HIT_COUNT) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0717, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8718, 13);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8722, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0726, 0xCEAA, 6)) return;
    SYNC_IN;
    mff_cycles(0x8727, 9);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + x) = a;
L_872D:;
    mff_cycles(0x872D, 7);
    a = 0x0D;
    RAM(TMP0) = a;
    y = 0x00;
    for (;;) {
        mff_cycles(0x8733, 4);
        a = prg_read((uint16_t)(0x87A7 + y));
        mff_cycles(0x8736, 7);
        RAM(TMP1) = a;
        a = prg_read((uint16_t)(0x87A8 + y));
        mff_cycles(0x873B, 55);
        RAM(TMP2) = a;
        y = RAM(TMP0);
        a = RAM(OBJ_DIR + x);
        RAM(OBJ_DIR + y) = a;
        a = RAM(OBJ_Y_HI + x);
        RAM(OBJ_Y_HI + y) = a;
        a = RAM(OBJ_X + x);
        c = 0;
        m = RAM(TMP1);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(OBJ_X + y) = a;
        a = RAM(OBJ_X_HI + x);
        m = 0x00;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(OBJ_X_HI + y) = a;
        if (a == RAM(OBJ_X_HI)) {
            mff_cycles(0x8761, 83);
            a = RAM(OBJ_Y + x);
            c = 0;
            m = 0x0E;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(OBJ_LAND_Y + y) = a;
            a = RAM(OBJ_Y + x);
            c = 1;
            m = RAM(TMP2);
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            RAM(OBJ_Y + y) = a;
            a = 0xFD;
            RAM(OBJ_ANIM + y) = a;
            a = 0xFF;
            RAM(OBJ_LIFE + y) = a;
            a = 0x0D;
            RAM(OBJ_KIND + y) = a;
            a = 0x06;
            RAM(OBJ_ID + y) = a;
            RAM(OBJ_SAVED_ID + y) = a;
            a = 0x80;
            RAM(OBJ_STATE + y) = a;
            a = 0x00;
            RAM(OBJ_ANIM_TIMER + y) = a;
            a = y;
            a &= 0x03;
            RAM(OBJ_ANIM_FRAME + y) = a;
        } else {
            mff_cycles(0x875F, 1);
        }
        mff_cycles(0x879A, 19);
        y = 0x02;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        a = RAM(TMP0);
        m = 0x09;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) break;
        mff_cycles(0x8733, 1);
    }
    mff_cycles(0x87A6, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8679 (bank 6) - lifted by port/tools/lift.py */
void func_8679_b6(void) {
    REGS_IN;
    mff_cycles(0x8679, 4);
    a = prg_read((uint16_t)(0x897A + y));
    mff_cycles(0x867C, 2);
    if (a == 0) {
        mff_cycles(0x867E, 4);
        a = prg_read((uint16_t)(0x899D + y));
        mff_cycles(0x8681, 2);
        if (a == 0) {
            mff_cycles(0x8683, 4);
            a = prg_read((uint16_t)(0x89C0 + y));
        } else {
            mff_cycles(0x8681, 1);
        }
    } else {
        mff_cycles(0x867C, 1);
    }
    mff_cycles(0x8686, 10);
    RAM(TMP0) = a;
    a = RAM(ATTACK_ID);
    if (a == 0x06) { mff_cycles(0x868C, 1); goto L_8692; }
    mff_cycles(0x868E, 4);
    if (a != 0x0C) { mff_cycles(0x8690, 1); goto L_8694; }
L_8692:;
    mff_cycles(0x8692, 3);
    RAM(COMBO_TARGET) = x;
L_8694:;
    mff_cycles(0x8694, 8);
    a = RAM(OBJ_KIND + x);
    if (a == 0x07) { mff_cycles(0x8699, 1); goto L_86B8; }
    mff_cycles(0x869B, 4);
    if (a == 0x08) { mff_cycles(0x869D, 1); goto L_86A9; }
    mff_cycles(0x869F, 4);
    if (a == 0x09) { mff_cycles(0x86A1, 1); goto L_86B8; }
    mff_cycles(0x86A3, 4);
    m = 0x0B;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x86A5, 1); goto L_86B8; }
    mff_cycles(0x86A7, 2);
    if (!z) { mff_cycles(0x86A7, 1); goto L_86E4; }
L_86A9:;
    mff_cycles(0x86A9, 4);
    if (y == 0x05) { mff_cycles(0x86AB, 1); goto L_86B1; }
    mff_cycles(0x86AD, 4);
    if (y != 0x13) { mff_cycles(0x86AF, 1); goto L_86B8; }
L_86B1:;
    mff_cycles(0x86B1, 10);
    a = RAM(TMP0);
    c = 0;
    m = 0x05;
    w = a + m + c;
    a = (uint8_t)w;
    RAM(TMP0) = a;
L_86B8:;
    mff_cycles(0x86B8, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07;
    w = a - m - !c;
    a = (uint8_t)w;
    y = a;
    if (a == 0x01) {
        mff_cycles(0x86C3, 6);
        a = RAM(OBJ_TIER + x);
        if (a != 0) {
            mff_cycles(0x86C8, 2);
            y = 0x03;
        } else {
            mff_cycles(0x86C6, 1);
        }
    } else {
        mff_cycles(0x86C1, 1);
    }
    mff_cycles(0x86CA, 7);
    a = RAM(ROUND);
    if (a == 0x04) {
        mff_cycles(0x86D0, 5);
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
    } else {
        mff_cycles(0x86CE, 1);
    }
    mff_cycles(0x86D2, 9);
    a = RAM(TMP0);
    c = 1;
    m = prg_read((uint16_t)(0x87AB + y));
    w = a - m - !c;
    a = (uint8_t)w;
    n = (a) >> 7;
    mff_cycles(0x86D8, 5);
    RAM(TMP0) = a;
    if (a == 0) { mff_cycles(0x86DA, 1); goto L_86E0; }
    mff_cycles(0x86DC, 2);
    if (n) { mff_cycles(0x86DC, 1); goto L_86E0; }
    mff_cycles(0x86DE, 2);
    if (!n) { mff_cycles(0x86DE, 1); goto L_86E4; }
L_86E0:;
    mff_cycles(0x86E0, 5);
    a = 0x01;
    RAM(TMP0) = a;
L_86E4:;
    mff_cycles(0x86E4, 22);
    a = RAM(OBJ_LIFE + x);
    c = 1;
    m = RAM(TMP0);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_LIFE + x) = a;
    a = RAM(OBJ_KIND + x);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86F2, 2); goto L_8702; }
    mff_cycles(0x86F4, 8);
    a = RAM(OBJ_LIFE + x);
    m = 0x30;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x86F9, 2); goto L_8702; }
    mff_cycles(0x86FB, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8700, 1); goto L_8703; }
L_8702:;
    mff_cycles(0x8702, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8703:;
    mff_cycles(0x8703, 7);
    a = RAM(GAME_ACTION);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0x8707, 1); goto L_870D; }
    mff_cycles(0x8709, 4);
    m = 0x21;
    c = a >= m;
    if (a != m) { mff_cycles(0x870B, 1); goto L_872D; }
L_870D:;
    mff_cycles(0x870D, 17);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GRAB_TIMER) = a;
    RAM(GAME_ACTION) = a;
    RAM(GRAB_HIT_COUNT) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0717, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8718, 13);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8722, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0726, 0xCEAA, 6)) return;
    SYNC_IN;
    mff_cycles(0x8727, 9);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + x) = a;
L_872D:;
    mff_cycles(0x872D, 7);
    a = 0x0D;
    RAM(TMP0) = a;
    y = 0x00;
    for (;;) {
        mff_cycles(0x8733, 4);
        a = prg_read((uint16_t)(0x87A7 + y));
        mff_cycles(0x8736, 7);
        RAM(TMP1) = a;
        a = prg_read((uint16_t)(0x87A8 + y));
        mff_cycles(0x873B, 55);
        RAM(TMP2) = a;
        y = RAM(TMP0);
        a = RAM(OBJ_DIR + x);
        RAM(OBJ_DIR + y) = a;
        a = RAM(OBJ_Y_HI + x);
        RAM(OBJ_Y_HI + y) = a;
        a = RAM(OBJ_X + x);
        c = 0;
        m = RAM(TMP1);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(OBJ_X + y) = a;
        a = RAM(OBJ_X_HI + x);
        m = 0x00;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(OBJ_X_HI + y) = a;
        if (a == RAM(OBJ_X_HI)) {
            mff_cycles(0x8761, 83);
            a = RAM(OBJ_Y + x);
            c = 0;
            m = 0x0E;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(OBJ_LAND_Y + y) = a;
            a = RAM(OBJ_Y + x);
            c = 1;
            m = RAM(TMP2);
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            RAM(OBJ_Y + y) = a;
            a = 0xFD;
            RAM(OBJ_ANIM + y) = a;
            a = 0xFF;
            RAM(OBJ_LIFE + y) = a;
            a = 0x0D;
            RAM(OBJ_KIND + y) = a;
            a = 0x06;
            RAM(OBJ_ID + y) = a;
            RAM(OBJ_SAVED_ID + y) = a;
            a = 0x80;
            RAM(OBJ_STATE + y) = a;
            a = 0x00;
            RAM(OBJ_ANIM_TIMER + y) = a;
            a = y;
            a &= 0x03;
            RAM(OBJ_ANIM_FRAME + y) = a;
        } else {
            mff_cycles(0x875F, 1);
        }
        mff_cycles(0x879A, 19);
        y = 0x02;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        a = RAM(TMP0);
        m = 0x09;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) break;
        mff_cycles(0x8733, 1);
    }
    mff_cycles(0x87A6, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $867E (bank 6) - lifted by port/tools/lift.py */
void func_867E_b6(void) {
    REGS_IN;
    mff_cycles(0x867E, 4);
    a = prg_read((uint16_t)(0x899D + y));
    mff_cycles(0x8681, 2);
    if (a == 0) {
        mff_cycles(0x8683, 4);
        a = prg_read((uint16_t)(0x89C0 + y));
    } else {
        mff_cycles(0x8681, 1);
    }
    mff_cycles(0x8686, 10);
    RAM(TMP0) = a;
    a = RAM(ATTACK_ID);
    if (a == 0x06) { mff_cycles(0x868C, 1); goto L_8692; }
    mff_cycles(0x868E, 4);
    if (a != 0x0C) { mff_cycles(0x8690, 1); goto L_8694; }
L_8692:;
    mff_cycles(0x8692, 3);
    RAM(COMBO_TARGET) = x;
L_8694:;
    mff_cycles(0x8694, 8);
    a = RAM(OBJ_KIND + x);
    if (a == 0x07) { mff_cycles(0x8699, 1); goto L_86B8; }
    mff_cycles(0x869B, 4);
    if (a == 0x08) { mff_cycles(0x869D, 1); goto L_86A9; }
    mff_cycles(0x869F, 4);
    if (a == 0x09) { mff_cycles(0x86A1, 1); goto L_86B8; }
    mff_cycles(0x86A3, 4);
    m = 0x0B;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x86A5, 1); goto L_86B8; }
    mff_cycles(0x86A7, 2);
    if (!z) { mff_cycles(0x86A7, 1); goto L_86E4; }
L_86A9:;
    mff_cycles(0x86A9, 4);
    if (y == 0x05) { mff_cycles(0x86AB, 1); goto L_86B1; }
    mff_cycles(0x86AD, 4);
    if (y != 0x13) { mff_cycles(0x86AF, 1); goto L_86B8; }
L_86B1:;
    mff_cycles(0x86B1, 10);
    a = RAM(TMP0);
    c = 0;
    m = 0x05;
    w = a + m + c;
    a = (uint8_t)w;
    RAM(TMP0) = a;
L_86B8:;
    mff_cycles(0x86B8, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07;
    w = a - m - !c;
    a = (uint8_t)w;
    y = a;
    if (a == 0x01) {
        mff_cycles(0x86C3, 6);
        a = RAM(OBJ_TIER + x);
        if (a != 0) {
            mff_cycles(0x86C8, 2);
            y = 0x03;
        } else {
            mff_cycles(0x86C6, 1);
        }
    } else {
        mff_cycles(0x86C1, 1);
    }
    mff_cycles(0x86CA, 7);
    a = RAM(ROUND);
    if (a == 0x04) {
        mff_cycles(0x86D0, 5);
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
    } else {
        mff_cycles(0x86CE, 1);
    }
    mff_cycles(0x86D2, 9);
    a = RAM(TMP0);
    c = 1;
    m = prg_read((uint16_t)(0x87AB + y));
    w = a - m - !c;
    a = (uint8_t)w;
    n = (a) >> 7;
    mff_cycles(0x86D8, 5);
    RAM(TMP0) = a;
    if (a == 0) { mff_cycles(0x86DA, 1); goto L_86E0; }
    mff_cycles(0x86DC, 2);
    if (n) { mff_cycles(0x86DC, 1); goto L_86E0; }
    mff_cycles(0x86DE, 2);
    if (!n) { mff_cycles(0x86DE, 1); goto L_86E4; }
L_86E0:;
    mff_cycles(0x86E0, 5);
    a = 0x01;
    RAM(TMP0) = a;
L_86E4:;
    mff_cycles(0x86E4, 22);
    a = RAM(OBJ_LIFE + x);
    c = 1;
    m = RAM(TMP0);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_LIFE + x) = a;
    a = RAM(OBJ_KIND + x);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86F2, 2); goto L_8702; }
    mff_cycles(0x86F4, 8);
    a = RAM(OBJ_LIFE + x);
    m = 0x30;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x86F9, 2); goto L_8702; }
    mff_cycles(0x86FB, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8700, 1); goto L_8703; }
L_8702:;
    mff_cycles(0x8702, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8703:;
    mff_cycles(0x8703, 7);
    a = RAM(GAME_ACTION);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0x8707, 1); goto L_870D; }
    mff_cycles(0x8709, 4);
    m = 0x21;
    c = a >= m;
    if (a != m) { mff_cycles(0x870B, 1); goto L_872D; }
L_870D:;
    mff_cycles(0x870D, 17);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GRAB_TIMER) = a;
    RAM(GAME_ACTION) = a;
    RAM(GRAB_HIT_COUNT) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0717, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8718, 13);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8722, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0726, 0xCEAA, 6)) return;
    SYNC_IN;
    mff_cycles(0x8727, 9);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + x) = a;
L_872D:;
    mff_cycles(0x872D, 7);
    a = 0x0D;
    RAM(TMP0) = a;
    y = 0x00;
    for (;;) {
        mff_cycles(0x8733, 4);
        a = prg_read((uint16_t)(0x87A7 + y));
        mff_cycles(0x8736, 7);
        RAM(TMP1) = a;
        a = prg_read((uint16_t)(0x87A8 + y));
        mff_cycles(0x873B, 55);
        RAM(TMP2) = a;
        y = RAM(TMP0);
        a = RAM(OBJ_DIR + x);
        RAM(OBJ_DIR + y) = a;
        a = RAM(OBJ_Y_HI + x);
        RAM(OBJ_Y_HI + y) = a;
        a = RAM(OBJ_X + x);
        c = 0;
        m = RAM(TMP1);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(OBJ_X + y) = a;
        a = RAM(OBJ_X_HI + x);
        m = 0x00;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(OBJ_X_HI + y) = a;
        if (a == RAM(OBJ_X_HI)) {
            mff_cycles(0x8761, 83);
            a = RAM(OBJ_Y + x);
            c = 0;
            m = 0x0E;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(OBJ_LAND_Y + y) = a;
            a = RAM(OBJ_Y + x);
            c = 1;
            m = RAM(TMP2);
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            RAM(OBJ_Y + y) = a;
            a = 0xFD;
            RAM(OBJ_ANIM + y) = a;
            a = 0xFF;
            RAM(OBJ_LIFE + y) = a;
            a = 0x0D;
            RAM(OBJ_KIND + y) = a;
            a = 0x06;
            RAM(OBJ_ID + y) = a;
            RAM(OBJ_SAVED_ID + y) = a;
            a = 0x80;
            RAM(OBJ_STATE + y) = a;
            a = 0x00;
            RAM(OBJ_ANIM_TIMER + y) = a;
            a = y;
            a &= 0x03;
            RAM(OBJ_ANIM_FRAME + y) = a;
        } else {
            mff_cycles(0x875F, 1);
        }
        mff_cycles(0x879A, 19);
        y = 0x02;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        a = RAM(TMP0);
        m = 0x09;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) break;
        mff_cycles(0x8733, 1);
    }
    mff_cycles(0x87A6, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8683 (bank 6) - lifted by port/tools/lift.py */
void func_8683_b6(void) {
    REGS_IN;
    mff_cycles(0x8683, 4);
    a = prg_read((uint16_t)(0x89C0 + y));
    mff_cycles(0x8686, 10);
    RAM(TMP0) = a;
    a = RAM(ATTACK_ID);
    if (a == 0x06) { mff_cycles(0x868C, 1); goto L_8692; }
    mff_cycles(0x868E, 4);
    if (a != 0x0C) { mff_cycles(0x8690, 1); goto L_8694; }
L_8692:;
    mff_cycles(0x8692, 3);
    RAM(COMBO_TARGET) = x;
L_8694:;
    mff_cycles(0x8694, 8);
    a = RAM(OBJ_KIND + x);
    if (a == 0x07) { mff_cycles(0x8699, 1); goto L_86B8; }
    mff_cycles(0x869B, 4);
    if (a == 0x08) { mff_cycles(0x869D, 1); goto L_86A9; }
    mff_cycles(0x869F, 4);
    if (a == 0x09) { mff_cycles(0x86A1, 1); goto L_86B8; }
    mff_cycles(0x86A3, 4);
    m = 0x0B;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x86A5, 1); goto L_86B8; }
    mff_cycles(0x86A7, 2);
    if (!z) { mff_cycles(0x86A7, 1); goto L_86E4; }
L_86A9:;
    mff_cycles(0x86A9, 4);
    if (y == 0x05) { mff_cycles(0x86AB, 1); goto L_86B1; }
    mff_cycles(0x86AD, 4);
    if (y != 0x13) { mff_cycles(0x86AF, 1); goto L_86B8; }
L_86B1:;
    mff_cycles(0x86B1, 10);
    a = RAM(TMP0);
    c = 0;
    m = 0x05;
    w = a + m + c;
    a = (uint8_t)w;
    RAM(TMP0) = a;
L_86B8:;
    mff_cycles(0x86B8, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07;
    w = a - m - !c;
    a = (uint8_t)w;
    y = a;
    if (a == 0x01) {
        mff_cycles(0x86C3, 6);
        a = RAM(OBJ_TIER + x);
        if (a != 0) {
            mff_cycles(0x86C8, 2);
            y = 0x03;
        } else {
            mff_cycles(0x86C6, 1);
        }
    } else {
        mff_cycles(0x86C1, 1);
    }
    mff_cycles(0x86CA, 7);
    a = RAM(ROUND);
    if (a == 0x04) {
        mff_cycles(0x86D0, 5);
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
    } else {
        mff_cycles(0x86CE, 1);
    }
    mff_cycles(0x86D2, 9);
    a = RAM(TMP0);
    c = 1;
    m = prg_read((uint16_t)(0x87AB + y));
    w = a - m - !c;
    a = (uint8_t)w;
    n = (a) >> 7;
    mff_cycles(0x86D8, 5);
    RAM(TMP0) = a;
    if (a == 0) { mff_cycles(0x86DA, 1); goto L_86E0; }
    mff_cycles(0x86DC, 2);
    if (n) { mff_cycles(0x86DC, 1); goto L_86E0; }
    mff_cycles(0x86DE, 2);
    if (!n) { mff_cycles(0x86DE, 1); goto L_86E4; }
L_86E0:;
    mff_cycles(0x86E0, 5);
    a = 0x01;
    RAM(TMP0) = a;
L_86E4:;
    mff_cycles(0x86E4, 22);
    a = RAM(OBJ_LIFE + x);
    c = 1;
    m = RAM(TMP0);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_LIFE + x) = a;
    a = RAM(OBJ_KIND + x);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86F2, 2); goto L_8702; }
    mff_cycles(0x86F4, 8);
    a = RAM(OBJ_LIFE + x);
    m = 0x30;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x86F9, 2); goto L_8702; }
    mff_cycles(0x86FB, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8700, 1); goto L_8703; }
L_8702:;
    mff_cycles(0x8702, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8703:;
    mff_cycles(0x8703, 7);
    a = RAM(GAME_ACTION);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0x8707, 1); goto L_870D; }
    mff_cycles(0x8709, 4);
    m = 0x21;
    c = a >= m;
    if (a != m) { mff_cycles(0x870B, 1); goto L_872D; }
L_870D:;
    mff_cycles(0x870D, 17);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GRAB_TIMER) = a;
    RAM(GAME_ACTION) = a;
    RAM(GRAB_HIT_COUNT) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0717, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8718, 13);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8722, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0726, 0xCEAA, 6)) return;
    SYNC_IN;
    mff_cycles(0x8727, 9);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + x) = a;
L_872D:;
    mff_cycles(0x872D, 7);
    a = 0x0D;
    RAM(TMP0) = a;
    y = 0x00;
    for (;;) {
        mff_cycles(0x8733, 4);
        a = prg_read((uint16_t)(0x87A7 + y));
        mff_cycles(0x8736, 7);
        RAM(TMP1) = a;
        a = prg_read((uint16_t)(0x87A8 + y));
        mff_cycles(0x873B, 55);
        RAM(TMP2) = a;
        y = RAM(TMP0);
        a = RAM(OBJ_DIR + x);
        RAM(OBJ_DIR + y) = a;
        a = RAM(OBJ_Y_HI + x);
        RAM(OBJ_Y_HI + y) = a;
        a = RAM(OBJ_X + x);
        c = 0;
        m = RAM(TMP1);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(OBJ_X + y) = a;
        a = RAM(OBJ_X_HI + x);
        m = 0x00;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(OBJ_X_HI + y) = a;
        if (a == RAM(OBJ_X_HI)) {
            mff_cycles(0x8761, 83);
            a = RAM(OBJ_Y + x);
            c = 0;
            m = 0x0E;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(OBJ_LAND_Y + y) = a;
            a = RAM(OBJ_Y + x);
            c = 1;
            m = RAM(TMP2);
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            a = (uint8_t)w;
            RAM(OBJ_Y + y) = a;
            a = 0xFD;
            RAM(OBJ_ANIM + y) = a;
            a = 0xFF;
            RAM(OBJ_LIFE + y) = a;
            a = 0x0D;
            RAM(OBJ_KIND + y) = a;
            a = 0x06;
            RAM(OBJ_ID + y) = a;
            RAM(OBJ_SAVED_ID + y) = a;
            a = 0x80;
            RAM(OBJ_STATE + y) = a;
            a = 0x00;
            RAM(OBJ_ANIM_TIMER + y) = a;
            a = y;
            a &= 0x03;
            RAM(OBJ_ANIM_FRAME + y) = a;
        } else {
            mff_cycles(0x875F, 1);
        }
        mff_cycles(0x879A, 19);
        y = 0x02;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        a = RAM(TMP0);
        m = 0x09;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) break;
        mff_cycles(0x8733, 1);
    }
    mff_cycles(0x87A6, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8C08 (bank 6) - lifted by port/tools/lift.py */
void func_8C08_b6(void) {
    REGS_IN;
    mff_cycles(0x8C08, 11);
    a = 0x0C;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ACTION_TIMER) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0C0E, func_F2BE, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8C0F, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8D4B (bank 6) - lifted by port/tools/lift.py */
void func_8D4B_b6(void) {
    REGS_IN;
    mff_cycles(0x8D4B, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8DEE (bank 6) - lifted by port/tools/lift.py */
void func_8DEE_b6(void) {
    REGS_IN;
    mff_cycles(0x8DEE, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $932E (bank 6) - lifted by port/tools/lift.py */
void func_932E_b6(void) {
    REGS_IN;
    mff_cycles(0x932E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9BFC, 2); call_by_address_tail(0x9BFC, 6); return;
}

/*  $942E (bank 6) - lifted by port/tools/lift.py */
void func_942E_b6(void) {
    REGS_IN;
    mff_cycles(0x942E, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $94B4 (bank 6) - lifted by port/tools/lift.py */
void func_94B4_b6(void) {
    REGS_IN;
    mff_cycles(0x94B4, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9531 (bank 6) - lifted by port/tools/lift.py */
void func_9531_b6(void) {
    REGS_IN;
    mff_cycles(0x9531, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $95AE (bank 6) - lifted by port/tools/lift.py */
void func_95AE_b6(void) {
    REGS_IN;
    mff_cycles(0x95AE, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9E2E (bank 6) - lifted by port/tools/lift.py */
void func_9E2E_b6(void) {
    REGS_IN;
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

/*  $9E3C (bank 6) - lifted by port/tools/lift.py */
void func_9E3C_b6(void) {
    REGS_IN;
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

/*  $9EC9 (bank 6) - lifted by port/tools/lift.py */
void func_9EC9_b6(void) {
    REGS_IN;
    mff_cycles(0x9EC9, 4);
    a = 0x02;
    if (a == 0) {
        mff_cycles(0x9ECD, 2);
        a = 0x04;
    } else {
        mff_cycles(0x9ECB, 1);
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

/*  $9F01 (bank 6) - lifted by port/tools/lift.py */
void func_9F01_b6(void) {
    REGS_IN;
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

/*  $A850 (bank 2) - lifted by port/tools/lift.py */
void func_A850_b2(void) {
    REGS_IN;
    mff_cycles(0xA850, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $892C (bank 4) - lifted by port/tools/lift.py */
void func_892C_b4(void) {
    REGS_IN;
    mff_cycles(0x892C, 5);
    a = RAM(SCREEN_EVENT);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0x8930, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0932, 0x972A, 4)) return;
        SYNC_IN;
        mff_cycles(0x8933, 5);
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(SCREEN_EVENT) = a;
    } else {
        mff_cycles(0x892E, 1);
    }
    mff_cycles(0x8937, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $893C (bank 4) - lifted by port/tools/lift.py */
void func_893C_b4(void) {
    REGS_IN;
    mff_cycles(0x893C, 7);
    a = RAM(GAME_ACTION);
    m = 0x19;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8940, 1); goto L_8955; }
    mff_cycles(0x8942, 5);
    a = RAM(GAME_ACTION);
    if (a == 0) { mff_cycles(0x8944, 1); goto L_894C; }
    mff_cycles(0x8946, 4);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8948, 1); goto L_894C; }
    mff_cycles(0x894A, 2);
    if (!z) { mff_cycles(0x894A, 1); goto L_8955; }
L_894C:;
    mff_cycles(0x894C, 11);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0x19;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
L_8955:;
    mff_cycles(0x8955, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8955 (bank 4) - lifted by port/tools/lift.py */
void func_8955_b4(void) {
    REGS_IN;
    mff_cycles(0x8955, 6);
    SYNC_OUT;
    mff_rts(); return;
}
