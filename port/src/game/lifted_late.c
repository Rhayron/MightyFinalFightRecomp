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
