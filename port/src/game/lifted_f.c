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
