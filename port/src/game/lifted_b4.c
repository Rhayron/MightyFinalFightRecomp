/*
 * lifted_b4.c - bank 4 routines $8000-$BFFF, lifted by
 * port/tools/lift_bank.py from the pure scaffold: exact, still to be
 * tidied and moved into their systems' files.
 */
#include "mff.h"
#include "state.h"
#include "coroutine.h"

void func_F03F(void);
void func_F183(void);
void func_F1A1(void);
void func_F1FD(void);
void func_F26E(void);
void func_F288(void);
void func_F294(void);
void func_F2A0(void);
void func_F314(void);
void func_F32E(void);
void func_F34C(void);
void func_F406(void);
void func_F430(void);
void func_F440(void);
void func_F71A(void);
void func_FF2A(void);
void func_FF31(void);

/*  $8000 (bank 4) - lifted by port/tools/lift.py */
void func_8000_b4(void) {
    REGS_IN;
    mff_cycles(0x8000, 7);
    x = RAM(AREA_INDEX);
    a = prg_read((uint16_t)(0x81FE + x));
    mff_cycles(0x8005, 7);
    RAM(SPAWN_LIST_X_HI) = a;
    a = prg_read((uint16_t)(0x820D + x));
    mff_cycles(0x800A, 7);
    RAM(SPAWN_LIST_X_HI + 1) = a;
    a = prg_read((uint16_t)(0x821C + x));
    mff_cycles(0x800F, 7);
    RAM(SPAWN_LIST_X) = a;
    a = prg_read((uint16_t)(0x822B + x));
    mff_cycles(0x8014, 7);
    RAM(SPAWN_LIST_X + 1) = a;
    a = prg_read((uint16_t)(0x823A + x));
    mff_cycles(0x8019, 7);
    RAM(SPAWN_LIST_Y_HI) = a;
    a = prg_read((uint16_t)(0x8249 + x));
    mff_cycles(0x801E, 7);
    RAM(SPAWN_LIST_Y_HI + 1) = a;
    a = prg_read((uint16_t)(0x8258 + x));
    mff_cycles(0x8023, 7);
    RAM(SPAWN_LIST_Y) = a;
    a = prg_read((uint16_t)(0x8267 + x));
    mff_cycles(0x8028, 7);
    RAM(SPAWN_LIST_Y + 1) = a;
    a = prg_read((uint16_t)(0x8276 + x));
    mff_cycles(0x802D, 7);
    RAM(SPAWN_LIST_TYPE) = a;
    a = prg_read((uint16_t)(0x8285 + x));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8032, 9);
    RAM(SPAWN_LIST_TYPE + 1) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8038 (bank 4) - lifted by port/tools/lift.py */
void func_8038_b4(void) {
    REGS_IN;
    mff_cycles(0x8038, 2);
    y = 0x00;
L_803A:;
    mff_cycles(0x803A, 5);
    a = mem_read((uint16_t)(RAM16(SPAWN_LIST_Y_HI) + y));
    mff_cycles(0x803C, 5);
    m = RAM(SCROLL_Y_HI);
    c = a >= m;
    if (a != m) { mff_cycles(0x803E, 1); goto L_804B; }
    mff_cycles(0x8040, 5);
    a = mem_read((uint16_t)(RAM16(SPAWN_LIST_X) + y));
    mff_cycles(0x8042, 10);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    c = w >= 0;
    a = mem_read((uint16_t)(RAM16(SPAWN_LIST_X_HI) + y));
    mff_cycles(0x8047, 5);
    m = RAM(AREA_SCREEN);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    if (c) { mff_cycles(0x8049, 1); goto L_8073; }
L_804B:;
    mff_cycles(0x804B, 4);
    y++;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0x803A, 1); goto L_803A; }
    mff_cycles(0x804E, 2);
    if (z) { mff_cycles(0x804E, 1); goto L_8073; }
    mff_cycles(0x8050, 20);
    a = RAM(SCROLL_X);
    c = 0;
    m = 0xFF;
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(TMP0) = a;
    a = RAM(AREA_SCREEN);
    m = 0x00;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(TMP1) = a;
    y = 0x00;
L_805F:;
    mff_cycles(0x805F, 5);
    a = mem_read((uint16_t)(RAM16(SPAWN_LIST_Y_HI) + y));
    mff_cycles(0x8061, 5);
    m = RAM(SCROLL_Y_HI);
    c = a >= m;
    if (a != m) { mff_cycles(0x8063, 1); goto L_8070; }
    mff_cycles(0x8065, 5);
    a = mem_read((uint16_t)(RAM16(SPAWN_LIST_X) + y));
    mff_cycles(0x8067, 10);
    c = 1;
    m = RAM(TMP0);
    w = a - m - !c;
    c = w >= 0;
    a = mem_read((uint16_t)(RAM16(SPAWN_LIST_X_HI) + y));
    mff_cycles(0x806C, 5);
    m = RAM(TMP1);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    if (!c) { mff_cycles(0x806E, 1); goto L_8073; }
L_8070:;
    mff_cycles(0x8070, 4);
    y++;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0x805F, 1); goto L_805F; }
L_8073:;
    mff_cycles(0x8073, 12);
    RAM(SPAWN_LIST_NEXT) = y;
    RAM(SPAWN_LIST_PREV) = y;
    SYNC_OUT;
    mff_rts(); return;
}

/* stage_spawns $8080 (bank 4) - lifted by port/tools/lift.py */
void func_8080_b4(void) {
    REGS_IN;
    mff_cycles(0x8080, 7);
    a = RAM(SCROLL_MODE);
    a &= 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x8084, 1); goto L_80A6; }
    mff_cycles(0x8086, 4);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0x8088, 1); goto L_80A7; }
    mff_cycles(0x808A, 4);
    m = 0x02;
    c = a >= m;
    if (a == m) {
        mff_cycles(0x808C, 1);
    }
L_808E:;
    mff_cycles(0x808E, 9);
    y = RAM(SPAWN_LIST_PREV);
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0092, 0x80C2, 4)) return;
    SYNC_IN;
    mff_cycles(0x8093, 2);
    if (!z) { mff_cycles(0x8093, 1); goto L_809C; }
    mff_cycles(0x8095, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0097, 0x80E4, 4)) return;
    SYNC_IN;
    mff_cycles(0x8098, 7);
    t = (uint8_t)(RAM(SPAWN_LIST_PREV) - 1);
    RAM(SPAWN_LIST_PREV) = t;
    if (t != 0) { mff_cycles(0x808E, 1); goto L_808E; }
L_809C:;
    mff_cycles(0x809C, 3);
    y = RAM(SPAWN_LIST_PREV);
    for (;;) {
        mff_cycles(0x809E, 8);
        y++;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x00A1, 0x80C2, 4)) return;
        SYNC_IN;
        mff_cycles(0x80A2, 2);
        if (!z) break;
        mff_cycles(0x809E, 1);
    }
    mff_cycles(0x80A4, 3);
    RAM(SPAWN_LIST_NEXT) = y;
L_80A6:;
    mff_cycles(0x80A6, 6);
    SYNC_OUT;
    mff_rts(); return;
L_80A7:;
    mff_cycles(0x80A7, 9);
    y = RAM(SPAWN_LIST_NEXT);
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x00AB, 0x80C2, 4)) return;
    SYNC_IN;
    mff_cycles(0x80AC, 2);
    if (!z) { mff_cycles(0x80AC, 1); goto L_80B5; }
    mff_cycles(0x80AE, 6);
    SYNC_OUT;
    if (!mff_call_at(0x00B0, 0x80E4, 4)) return;
    SYNC_IN;
    mff_cycles(0x80B1, 7);
    t = (uint8_t)(RAM(SPAWN_LIST_NEXT) + 1);
    RAM(SPAWN_LIST_NEXT) = t;
    if (t != 0) { mff_cycles(0x80A7, 1); goto L_80A7; }
L_80B5:;
    mff_cycles(0x80B5, 5);
    y = RAM(SPAWN_LIST_NEXT);
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) {
        for (;;) {
            mff_cycles(0x80B9, 8);
            y--;
            n = (y) >> 7;
            z = (y) == 0;
            SYNC_OUT;
            if (!mff_call_at(0x00BC, 0x80C2, 4)) return;
            SYNC_IN;
            mff_cycles(0x80BD, 2);
            if (!z) break;
            mff_cycles(0x80B9, 1);
        }
    } else {
        mff_cycles(0x80B7, 1);
    }
    mff_cycles(0x80BF, 9);
    RAM(SPAWN_LIST_PREV) = y;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $80C2 (bank 4) - lifted by port/tools/lift.py */
void func_80C2_b4(void) {
    REGS_IN;
    mff_cycles(0x80C2, 5);
    a = mem_read((uint16_t)(RAM16(SPAWN_LIST_X) + y));
    mff_cycles(0x80C4, 13);
    RAM(TMP0) = a;
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    c = w >= 0;
    a = mem_read((uint16_t)(RAM16(SPAWN_LIST_X_HI) + y));
    mff_cycles(0x80CB, 8);
    RAM(TMP1) = a;
    m = RAM(AREA_SCREEN);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0x80D1, 5);
        a = mem_read((uint16_t)(RAM16(SPAWN_LIST_Y) + y));
        mff_cycles(0x80D3, 10);
        RAM(TMP2) = a;
        c = 1;
        m = RAM(SCROLL_Y);
        w = a - m - !c;
        c = w >= 0;
        if (!c) {
            mff_cycles(0x80DA, 4);
            c = 0;
        } else {
            mff_cycles(0x80D8, 1);
        }
        mff_cycles(0x80DD, 5);
        a = mem_read((uint16_t)(RAM16(SPAWN_LIST_Y_HI) + y));
        mff_cycles(0x80DF, 6);
        RAM(TMP3) = a;
        m = RAM(SCROLL_Y_HI);
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
    } else {
        mff_cycles(0x80CF, 1);
    }
    mff_cycles(0x80E3, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $80E4 (bank 4) - lifted by port/tools/lift.py */
void func_80E4_b4(void) {
    REGS_IN;
    mff_cycles(0x80E4, 4);
    x = 0x0D;
    a = y;
L_80E7:;
    mff_cycles(0x80E7, 6);
    m = RAM(OBJ_SPAWN_INDEX + x);
    c = a >= m;
    if (a == m) { mff_cycles(0x80EA, 1); goto L_80F3; }
    mff_cycles(0x80EC, 6);
    x--;
    m = 0x02;
    c = x >= m;
    z = ((uint8_t)(x - m)) == 0;
    if (!z) { mff_cycles(0x80E7, 1); goto L_80E7; }
    mff_cycles(0x80F1, 2);
    if (z) { mff_cycles(0x80F1, 1); goto L_80F8; }
L_80F3:;
    mff_cycles(0x80F3, 6);
    a = RAM(OBJ_STATE + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) { mff_cycles(0x80F6, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x00E3), 4); return; }
L_80F8:;
    mff_cycles(0x80F8, 2);
    x = 0x0D;
L_80FA:;
    mff_cycles(0x80FA, 6);
    a = RAM(OBJ_STATE + x);
    if (!(a & 0x80)) { mff_cycles(0x80FD, 2); goto L_8105; }
    mff_cycles(0x80FF, 6);
    x--;
    m = 0x02;
    c = x >= m;
    n = ((uint8_t)(x - m)) >> 7;
    z = ((uint8_t)(x - m)) == 0;
    if (!z) { mff_cycles(0x80FA, 2); goto L_80FA; }
    mff_cycles(0x8104, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8105:;
    mff_cycles(0x8105, 54);
    a = y;
    RAM(OBJ_SPAWN_INDEX + x) = a;
    a = RAM(TMP0);
    RAM(OBJ_X + x) = a;
    a = RAM(TMP1);
    RAM(OBJ_X_HI + x) = a;
    a = RAM(TMP2);
    RAM(OBJ_Y + x) = a;
    a = RAM(TMP3);
    RAM(OBJ_Y_HI + x) = a;
    a = 0x42;
    RAM(TMP4) = a;
    a = 0x84;
    RAM(TMP5) = a;
    a = mem_read((uint16_t)(RAM16(SPAWN_LIST_TYPE) + y));
    mff_cycles(0x8127, 22);
    RAM(TMP6) = y;
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    y = a;
    a = RAM(TMP5);
    m = 0x00;
    w = a + m + c;
    a = (uint8_t)w;
    RAM(TMP5) = a;
    a = mem_read((uint16_t)(RAM16(TMP4) + y));
    mff_cycles(0x8134, 12);
    RAM(OBJ_FLAGS + x) = a;
    y++;
    a = mem_read((uint16_t)(RAM16(TMP4) + y));
    mff_cycles(0x813A, 17);
    RAM(OBJ_ID + x) = a;
    RAM(OBJ_SAVED_ID + x) = a;
    y++;
    a = mem_read((uint16_t)(RAM16(TMP4) + y));
    mff_cycles(0x8143, 12);
    RAM(OBJ_ANIM + x) = a;
    y++;
    a = mem_read((uint16_t)(RAM16(TMP4) + y));
    mff_cycles(0x8149, 23);
    RAM(OBJ_ATTACK + x) = a;
    y = RAM(TMP6);
    a = 0xE6;
    RAM(TMP7) = a;
    a = 0x85;
    RAM(TMP8) = a;
    a = mem_read((uint16_t)(RAM16(SPAWN_LIST_TYPE) + y));
    mff_cycles(0x8158, 19);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    y = a;
    a = RAM(TMP8);
    m = 0x00;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(TMP8) = a;
    a = mem_read((uint16_t)(RAM16(TMP7) + y));
    mff_cycles(0x8163, 17);
    RAM(OBJ_LIFE + x) = a;
    RAM(OBJ_LIFE_MAX + x) = a;
    y++;
    a = mem_read((uint16_t)(RAM16(TMP7) + y));
    mff_cycles(0x816C, 12);
    RAM(OBJ_SPAWN_GROUP + x) = a;
    y++;
    a = mem_read((uint16_t)(RAM16(TMP7) + y));
    mff_cycles(0x8172, 23);
    RAM(OBJ_KIND + x) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    RAM(OBJ_SAVED_STATE + x) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x017F, func_F26E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8180, 103);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ANIM_TIMER + x) = a;
    RAM(OBJ_ANIM_FRAME + x) = a;
    RAM(OBJ_THROW_FLAG + x) = a;
    RAM(OBJ_TIMER + x) = a;
    RAM(OBJ_TIMER2 + x) = a;
    RAM(OBJ_GROUND_Y + x) = a;
    RAM(OBJ_LAND_Y + x) = a;
    RAM(OBJ_TIMER3 + x) = a;
    RAM(OBJ_DOWN_TIMER + x) = a;
    RAM(OBJ_HIT_COOLDOWN + x) = a;
    RAM(OBJ_INVULN + x) = a;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    RAM(OBJ_HIT_PLAYER + x) = a;
    RAM(OBJ_HIT_TIMER + x) = a;
    RAM(OBJ_HELD + x) = a;
    RAM(OBJ_TIER + x) = a;
    RAM(OBJ_AI_TIMER + x) = a;
    RAM(OBJ_AI_COUNT + x) = a;
    RAM(OBJ_AI_FLAG + x) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x01BD, func_F314, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x81BE, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x01C0, func_F32E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x81C1, 8);
    a = RAM(OBJ_ID + x);
    if (a != 0x69) {
        mff_cycles(0x81C8, 4);
        m = 0x10;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0x81CC, 4);
            m = 0x11;
            c = a >= m;
            n = ((uint8_t)(a - m)) >> 7;
            z = ((uint8_t)(a - m)) == 0;
            if (!z) {
                mff_cycles(0x81D0, 4);
                m = 0x12;
                c = a >= m;
                n = ((uint8_t)(a - m)) >> 7;
                z = ((uint8_t)(a - m)) == 0;
                if (!z) {
                    mff_cycles(0x81D4, 4);
                    m = 0x13;
                    c = a >= m;
                    n = ((uint8_t)(a - m)) >> 7;
                    z = ((uint8_t)(a - m)) == 0;
                    if (z) {
                        mff_cycles(0x81D6, 1);
                    }
                } else {
                    mff_cycles(0x81D2, 1);
                }
            } else {
                mff_cycles(0x81CE, 1);
            }
        } else {
            mff_cycles(0x81CA, 1);
        }
        mff_cycles(0x81D8, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x81C6, 1);
    }
    mff_cycles(0x81D9, 7);
    a = RAM(PLAYER_ID);
    m = 0x00;
    c = a >= m;
    if (a == m) { mff_cycles(0x81DD, 1); goto L_81E8; }
    mff_cycles(0x81DF, 4);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0x81E1, 1); goto L_81EC; }
    mff_cycles(0x81E3, 4);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x81E5, 1); goto L_81F0; }
    mff_cycles(0x81E7, 6);
    SYNC_OUT;
    mff_rts(); return;
L_81E8:;
    mff_cycles(0x81E8, 4);
    a = 0x40;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x81EA, 1); goto L_81F2; }
L_81EC:;
    mff_cycles(0x81EC, 4);
    a = 0x5E;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x81EE, 1); goto L_81F2; }
L_81F0:;
    mff_cycles(0x81F0, 2);
    a = 0x5D;
    n = (a) >> 7;
    z = (a) == 0;
L_81F2:;
    mff_cycles(0x81F2, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x01F4, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x81F5, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/* stage_events $8900 (bank 4) - lifted by port/tools/lift.py */
void func_8900_b4(void) {
    REGS_IN;
    mff_cycles(0x8900, 5);
    a = RAM(SCROLL_X_HI);
    if (a != 0) { mff_cycles(0x8902, 1); goto L_8924; }
    mff_cycles(0x8904, 7);
    a = RAM(ROUND);
    if (a == 0x02) { mff_cycles(0x8908, 1); goto L_890F; }
    mff_cycles(0x890A, 4);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x890C, 1); goto L_8916; }
    mff_cycles(0x890E, 6);
    SYNC_OUT;
    mff_rts(); return;
L_890F:;
    mff_cycles(0x890F, 7);
    a = RAM(SUB_AREA);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8913, 1); goto L_891D; }
    mff_cycles(0x8915, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8916:;
    mff_cycles(0x8916, 7);
    a = RAM(SUB_AREA);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0x891C, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x891A, 1);
    }
L_891D:;
    mff_cycles(0x891D, 5);
    a = RAM(AREA_CLEAR);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x891F, 1); goto L_8938; }
    mff_cycles(0x8921, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8956, 2); call_by_address_tail(0x8956, 4); return;
L_8924:;
    mff_cycles(0x8924, 5);
    a = RAM(AREA_CLEAR);
    if (a != 0) { mff_cycles(0x8926, 1); goto L_8956; }
    mff_cycles(0x8928, 5);
    a = RAM(CAMERA_LOCK);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x892A, 1); goto L_8938; }
L_892C:;
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
L_8938:;
    mff_cycles(0x8938, 6);
    SYNC_OUT;
    if (!mff_call_at(0x093A, 0x89D7, 4)) return;
    SYNC_IN;
    mff_cycles(0x893B, 6);
    SYNC_OUT;
    mff_rts(); return;
L_893C:;
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
L_8956:;
    mff_cycles(0x8956, 5);
    a = RAM(AREA_CLEAR);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0x8955, 1); goto L_8955; }
    mff_cycles(0x895A, 8);
    a = RAM(AREA_SCREEN);
    m = RAM(AREA_SCREEN_COUNT);
    c = a >= m;
    if (a == m) { mff_cycles(0x893C, 1); goto L_893C; }
    mff_cycles(0x8960, 5);
    a = RAM(GAME_ACTION);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x8955, 1); goto L_8955; }
    mff_cycles(0x8964, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0966, 0x96FC, 4)) return;
    SYNC_IN;
    mff_cycles(0x8967, 2);
    if (!z) { mff_cycles(0x8955, 1); goto L_8955; }
    mff_cycles(0x8969, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x096B, func_F71A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x896C, 2);
    if (c) { mff_cycles(0x892C, 1); goto L_892C; }
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

/*  $89D2 (bank 4) - lifted by port/tools/lift.py */
void func_89D2_b4(void) {
    REGS_IN;
    mff_cycles(0x89D2, 11);
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(AREA_CLEAR) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $89D7 (bank 4) - lifted by port/tools/lift.py */
void func_89D7_b4(void) {
    REGS_IN;
    mff_cycles(0x89D7, 7);
    y = RAM(ROUND);
    a = prg_read((uint16_t)(0x89F5 + y));
    mff_cycles(0x89DC, 7);
    RAM(TMP2) = a;
    a = prg_read((uint16_t)(0x89FA + y));
    mff_cycles(0x89E1, 11);
    RAM(TMP3) = a;
    y = RAM(SUB_AREA);
    a = mem_read((uint16_t)(RAM16(TMP2) + y));
    mff_cycles(0x89E7, 6);
    y = a;
    a = prg_read((uint16_t)(0x8A0E + y));
    mff_cycles(0x89EB, 7);
    RAM(TMP4) = a;
    a = prg_read((uint16_t)(0x8A1D + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x89F0, 8);
    RAM(TMP5) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0004); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $8A2C (bank 4) - lifted by port/tools/lift.py */
void func_8A2C_b4(void) {
    REGS_IN;
    mff_cycles(0x8A2C, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x8A3B + y));
    mff_cycles(0x8A31, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x8A3F + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8A36, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $8A43 (bank 4) - lifted by port/tools/lift.py */
void func_8A43_b4(void) {
    REGS_IN;
    mff_cycles(0x8A43, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8A44 (bank 4) - lifted by port/tools/lift.py */
void func_8A44_b4(void) {
    REGS_IN;
    mff_cycles(0x8A44, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DC4 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8A49, 2);
    if (c) {
        mff_cycles(0x8A4B, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0A4D, 0x89D2, 4)) return;
        SYNC_IN;
        mff_cycles(0x8A4E, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x8A49, 1);
    }
    mff_cycles(0x8A4F, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0A51, 0x9000, 4)) return;
    SYNC_IN;
    mff_cycles(0x8A52, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8A53 (bank 4) - lifted by port/tools/lift.py */
void func_8A53_b4(void) {
    REGS_IN;
    mff_cycles(0x8A53, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DC4 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8A58, 2);
    if (c) {
        mff_cycles(0x8A5A, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0A5C, 0x89D2, 4)) return;
        SYNC_IN;
        mff_cycles(0x8A5D, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x8A58, 1);
    }
    mff_cycles(0x8A5E, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0A60, 0x9000, 4)) return;
    SYNC_IN;
    mff_cycles(0x8A61, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8A62 (bank 4) - lifted by port/tools/lift.py */
void func_8A62_b4(void) {
    REGS_IN;
    mff_cycles(0x8A62, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DC4 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8A67, 2);
    if (!c) { mff_cycles(0x8A67, 1); goto L_8A72; }
    mff_cycles(0x8A69, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0A6B, 0x96FC, 4)) return;
    SYNC_IN;
    mff_cycles(0x8A6C, 2);
    if (!z) { mff_cycles(0x8A6C, 1); goto L_8A75; }
    mff_cycles(0x8A6E, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0A70, 0x89D2, 4)) return;
    SYNC_IN;
    mff_cycles(0x8A71, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8A72:;
    mff_cycles(0x8A72, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0A74, 0x9000, 4)) return;
    SYNC_IN;
L_8A75:;
    mff_cycles(0x8A75, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8A76 (bank 4) - lifted by port/tools/lift.py */
void func_8A76_b4(void) {
    REGS_IN;
    mff_cycles(0x8A76, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x8A85 + y));
    mff_cycles(0x8A7B, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x8A89 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8A80, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $8A8D (bank 4) - lifted by port/tools/lift.py */
void func_8A8D_b4(void) {
    REGS_IN;
    mff_cycles(0x8A8D, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DC8 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8A92, 2);
    if (c) {
        mff_cycles(0x8A94, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0A96, 0x96FC, 4)) return;
        SYNC_IN;
        mff_cycles(0x8A97, 2);
        if (z) {
            mff_cycles(0x8A99, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0A9B, 0x89D2, 4)) return;
            SYNC_IN;
        } else {
            mff_cycles(0x8A97, 1);
        }
    } else {
        mff_cycles(0x8A92, 1);
    }
    mff_cycles(0x8A9C, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8A9D (bank 4) - lifted by port/tools/lift.py */
void func_8A9D_b4(void) {
    REGS_IN;
    mff_cycles(0x8A9D, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DC8 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8AA2, 2);
    if (c) {
        mff_cycles(0x8AA4, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0AA6, 0x89D2, 4)) return;
        SYNC_IN;
        mff_cycles(0x8AA7, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x8AA2, 1);
    }
    mff_cycles(0x8AA8, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0AAA, 0x9000, 4)) return;
    SYNC_IN;
    mff_cycles(0x8AAB, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8AAB (bank 4) - lifted by port/tools/lift.py */
void func_8AAB_b4(void) {
    REGS_IN;
    mff_cycles(0x8AAB, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8AAC (bank 4) - lifted by port/tools/lift.py */
void func_8AAC_b4(void) {
    REGS_IN;
    mff_cycles(0x8AAC, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x8ABB + y));
    mff_cycles(0x8AB1, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x8ABD + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8AB6, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $8ABF (bank 4) - lifted by port/tools/lift.py */
void func_8ABF_b4(void) {
    REGS_IN;
    mff_cycles(0x8ABF, 12);
    a = 0xFF;
    RAM(KEEP_ON_SCREEN) = a;
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DCC + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8AC8, 2);
    if (c) {
        mff_cycles(0x8ACA, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0ACC, 0x96FC, 4)) return;
        SYNC_IN;
        mff_cycles(0x8ACD, 2);
        if (z) {
            mff_cycles(0x8ACF, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0AD1, 0x89D2, 4)) return;
            SYNC_IN;
        } else {
            mff_cycles(0x8ACD, 1);
        }
    } else {
        mff_cycles(0x8AC8, 1);
    }
    mff_cycles(0x8AD2, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8AD3 (bank 4) - lifted by port/tools/lift.py */
void func_8AD3_b4(void) {
    REGS_IN;
    mff_cycles(0x8AD3, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x8AE2 + y));
    mff_cycles(0x8AD8, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x8AE6 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8ADD, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $8AEA (bank 4) - lifted by port/tools/lift.py */
void func_8AEA_b4(void) {
    REGS_IN;
    mff_cycles(0x8AEA, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DCE + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8AEF, 2);
    if (c) {
        mff_cycles(0x8AF1, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0AF3, 0x89D2, 4)) return;
        SYNC_IN;
        mff_cycles(0x8AF4, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x8AEF, 1);
    }
    mff_cycles(0x8AF5, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0AF7, 0x9000, 4)) return;
    SYNC_IN;
    mff_cycles(0x8AF8, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8AF9 (bank 4) - lifted by port/tools/lift.py */
void func_8AF9_b4(void) {
    REGS_IN;
    mff_cycles(0x8AF9, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DCE + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8AFE, 2);
    if (c) {
        mff_cycles(0x8B00, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0B02, 0x89D2, 4)) return;
        SYNC_IN;
        mff_cycles(0x8B03, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x8AFE, 1);
    }
    mff_cycles(0x8B04, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0B06, 0x9000, 4)) return;
    SYNC_IN;
    mff_cycles(0x8B07, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8B08 (bank 4) - lifted by port/tools/lift.py */
void func_8B08_b4(void) {
    REGS_IN;
    mff_cycles(0x8B08, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DCE + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8B0D, 2);
    if (!c) { mff_cycles(0x8B0D, 1); goto L_8B18; }
    mff_cycles(0x8B0F, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0B11, 0x96FC, 4)) return;
    SYNC_IN;
    mff_cycles(0x8B12, 2);
    if (!z) { mff_cycles(0x8B12, 1); goto L_8B1B; }
    mff_cycles(0x8B14, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0B16, 0x89D2, 4)) return;
    SYNC_IN;
    mff_cycles(0x8B17, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8B18:;
    mff_cycles(0x8B18, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0B1A, 0x9000, 4)) return;
    SYNC_IN;
L_8B1B:;
    mff_cycles(0x8B1B, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8B1C (bank 4) - lifted by port/tools/lift.py */
void func_8B1C_b4(void) {
    REGS_IN;
    mff_cycles(0x8B1C, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x8B2B + y));
    mff_cycles(0x8B21, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x8B30 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8B26, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $8B35 (bank 4) - lifted by port/tools/lift.py */
void func_8B35_b4(void) {
    REGS_IN;
    mff_cycles(0x8B35, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DD2 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8B3A, 2);
    if (c) {
        mff_cycles(0x8B3C, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0B3E, 0x89D2, 4)) return;
        SYNC_IN;
        mff_cycles(0x8B3F, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x8B3A, 1);
    }
    mff_cycles(0x8B40, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0B42, 0x9000, 4)) return;
    SYNC_IN;
    mff_cycles(0x8B43, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8B44 (bank 4) - lifted by port/tools/lift.py */
void func_8B44_b4(void) {
    REGS_IN;
    mff_cycles(0x8B44, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DD2 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8B49, 2);
    if (!c) { mff_cycles(0x8B49, 1); goto L_8B54; }
    mff_cycles(0x8B4B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0B4D, 0x96FC, 4)) return;
    SYNC_IN;
    mff_cycles(0x8B4E, 2);
    if (!z) { mff_cycles(0x8B4E, 1); goto L_8B57; }
    mff_cycles(0x8B50, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0B52, 0x89D2, 4)) return;
    SYNC_IN;
    mff_cycles(0x8B53, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8B54:;
    mff_cycles(0x8B54, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0B56, 0x9000, 4)) return;
    SYNC_IN;
L_8B57:;
    mff_cycles(0x8B57, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8B58 (bank 4) - lifted by port/tools/lift.py */
void func_8B58_b4(void) {
    REGS_IN;
    mff_cycles(0x8B58, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DD2 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8B5D, 2);
    if (c) {
        mff_cycles(0x8B5F, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0B61, 0x96FC, 4)) return;
        SYNC_IN;
        mff_cycles(0x8B62, 2);
        if (z) {
            mff_cycles(0x8B64, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0B66, 0x89D2, 4)) return;
            SYNC_IN;
        } else {
            mff_cycles(0x8B62, 1);
        }
    } else {
        mff_cycles(0x8B5D, 1);
    }
    mff_cycles(0x8B67, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8B68 (bank 4) - lifted by port/tools/lift.py */
void func_8B68_b4(void) {
    REGS_IN;
    mff_cycles(0x8B68, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DD2 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8B6D, 2);
    if (!c) { mff_cycles(0x8B6D, 1); goto L_8B78; }
    mff_cycles(0x8B6F, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0B71, 0x96FC, 4)) return;
    SYNC_IN;
    mff_cycles(0x8B72, 2);
    if (!z) { mff_cycles(0x8B72, 1); goto L_8B7B; }
    mff_cycles(0x8B74, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0B76, 0x89D2, 4)) return;
    SYNC_IN;
    mff_cycles(0x8B77, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8B78:;
    mff_cycles(0x8B78, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0B7A, 0x9000, 4)) return;
    SYNC_IN;
L_8B7B:;
    mff_cycles(0x8B7B, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8B7C (bank 4) - lifted by port/tools/lift.py */
void func_8B7C_b4(void) {
    REGS_IN;
    mff_cycles(0x8B7C, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x8B8B + y));
    mff_cycles(0x8B81, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x8B8D + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8B86, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $8B8F (bank 4) - lifted by port/tools/lift.py */
void func_8B8F_b4(void) {
    REGS_IN;
    mff_cycles(0x8B8F, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DD7 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8B94, 2);
    if (c) {
        mff_cycles(0x8B96, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0B98, 0x96FC, 4)) return;
        SYNC_IN;
        mff_cycles(0x8B99, 2);
        if (z) {
            mff_cycles(0x8B9B, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0B9D, 0x89D2, 4)) return;
            SYNC_IN;
        } else {
            mff_cycles(0x8B99, 1);
        }
    } else {
        mff_cycles(0x8B94, 1);
    }
    mff_cycles(0x8B9E, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8B9F (bank 4) - lifted by port/tools/lift.py */
void func_8B9F_b4(void) {
    REGS_IN;
    mff_cycles(0x8B9F, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x8BAE + y));
    mff_cycles(0x8BA4, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x8BB2 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8BA9, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $8BB6 (bank 4) - lifted by port/tools/lift.py */
void func_8BB6_b4(void) {
    REGS_IN;
    mff_cycles(0x8BB6, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DD9 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8BBB, 2);
    if (c) {
        mff_cycles(0x8BBD, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0BBF, 0x89D2, 4)) return;
        SYNC_IN;
        mff_cycles(0x8BC0, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x8BBB, 1);
    }
    mff_cycles(0x8BC1, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0BC3, 0x9000, 4)) return;
    SYNC_IN;
    mff_cycles(0x8BC4, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8BC5 (bank 4) - lifted by port/tools/lift.py */
void func_8BC5_b4(void) {
    REGS_IN;
    mff_cycles(0x8BC5, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DD9 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8BCA, 2);
    if (c) {
        mff_cycles(0x8BCC, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0BCE, 0x89D2, 4)) return;
        SYNC_IN;
        mff_cycles(0x8BCF, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x8BCA, 1);
    }
    mff_cycles(0x8BD0, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0BD2, 0x9000, 4)) return;
    SYNC_IN;
    mff_cycles(0x8BD3, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8BE8 (bank 4) - lifted by port/tools/lift.py */
void func_8BE8_b4(void) {
    REGS_IN;
    mff_cycles(0x8BE8, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x8BF7 + y));
    mff_cycles(0x8BED, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x8BFB + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8BF2, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $8BFF (bank 4) - lifted by port/tools/lift.py */
void func_8BFF_b4(void) {
    REGS_IN;
    mff_cycles(0x8BFF, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DDD + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8C04, 2);
    if (c) {
        mff_cycles(0x8C06, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0C08, 0x89D2, 4)) return;
        SYNC_IN;
        mff_cycles(0x8C09, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x8C04, 1);
    }
    mff_cycles(0x8C0A, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0C0C, 0x9000, 4)) return;
    SYNC_IN;
    mff_cycles(0x8C0D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8C0E (bank 4) - lifted by port/tools/lift.py */
void func_8C0E_b4(void) {
    REGS_IN;
    mff_cycles(0x8C0E, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DDD + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8C13, 2);
    if (c) {
        mff_cycles(0x8C15, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0C17, 0x96FC, 4)) return;
        SYNC_IN;
        mff_cycles(0x8C18, 2);
        if (z) {
            mff_cycles(0x8C1A, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0C1C, 0x89D2, 4)) return;
            SYNC_IN;
        } else {
            mff_cycles(0x8C18, 1);
        }
    } else {
        mff_cycles(0x8C13, 1);
    }
    mff_cycles(0x8C1D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8C1E (bank 4) - lifted by port/tools/lift.py */
void func_8C1E_b4(void) {
    REGS_IN;
    mff_cycles(0x8C1E, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DDD + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8C23, 2);
    if (!c) { mff_cycles(0x8C23, 1); goto L_8C2E; }
    mff_cycles(0x8C25, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0C27, 0x96FC, 4)) return;
    SYNC_IN;
    mff_cycles(0x8C28, 2);
    if (!z) { mff_cycles(0x8C28, 1); goto L_8C31; }
    mff_cycles(0x8C2A, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0C2C, 0x89D2, 4)) return;
    SYNC_IN;
    mff_cycles(0x8C2D, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8C2E:;
    mff_cycles(0x8C2E, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0C30, 0x9000, 4)) return;
    SYNC_IN;
L_8C31:;
    mff_cycles(0x8C31, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8C32 (bank 4) - lifted by port/tools/lift.py */
void func_8C32_b4(void) {
    REGS_IN;
    mff_cycles(0x8C32, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x8C41 + y));
    mff_cycles(0x8C37, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x8C42 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8C3C, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $8C43 (bank 4) - lifted by port/tools/lift.py */
void func_8C43_b4(void) {
    REGS_IN;
    mff_cycles(0x8C43, 12);
    a = 0xFF;
    RAM(KEEP_ON_SCREEN) = a;
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DE1 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8C4C, 2);
    if (c) {
        mff_cycles(0x8C4E, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0C50, 0x96FC, 4)) return;
        SYNC_IN;
        mff_cycles(0x8C51, 2);
        if (z) {
            mff_cycles(0x8C53, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0C55, 0x89D2, 4)) return;
            SYNC_IN;
        } else {
            mff_cycles(0x8C51, 1);
        }
    } else {
        mff_cycles(0x8C4C, 1);
    }
    mff_cycles(0x8C56, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8C57 (bank 4) - lifted by port/tools/lift.py */
void func_8C57_b4(void) {
    REGS_IN;
    mff_cycles(0x8C57, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x8C66 + y));
    mff_cycles(0x8C5C, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x8C6A + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8C61, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $8C6E (bank 4) - lifted by port/tools/lift.py */
void func_8C6E_b4(void) {
    REGS_IN;
    mff_cycles(0x8C6E, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DE2 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8C73, 2);
    if (c) {
        mff_cycles(0x8C75, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0C77, 0x89D2, 4)) return;
        SYNC_IN;
        mff_cycles(0x8C78, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x8C73, 1);
    }
    mff_cycles(0x8C79, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0C7B, 0x9000, 4)) return;
    SYNC_IN;
    mff_cycles(0x8C7C, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8C7D (bank 4) - lifted by port/tools/lift.py */
void func_8C7D_b4(void) {
    REGS_IN;
    mff_cycles(0x8C7D, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DE2 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8C82, 2);
    if (c) {
        mff_cycles(0x8C84, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0C86, 0x89D2, 4)) return;
        SYNC_IN;
        mff_cycles(0x8C87, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x8C82, 1);
    }
    mff_cycles(0x8C88, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0C8A, 0x9000, 4)) return;
    SYNC_IN;
    mff_cycles(0x8C8B, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8C8B (bank 4) - lifted by port/tools/lift.py */
void func_8C8B_b4(void) {
    REGS_IN;
    mff_cycles(0x8C8B, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8C8C (bank 4) - lifted by port/tools/lift.py */
void func_8C8C_b4(void) {
    REGS_IN;
    mff_cycles(0x8C8C, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x8C9B + y));
    mff_cycles(0x8C91, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x8CA0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8C96, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $8CA5 (bank 4) - lifted by port/tools/lift.py */
void func_8CA5_b4(void) {
    REGS_IN;
    mff_cycles(0x8CA5, 12);
    a = 0xFF;
    RAM(KEEP_ON_SCREEN) = a;
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DE6 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8CAE, 2);
    if (!c) { mff_cycles(0x8CAE, 1); goto L_8CB9; }
    mff_cycles(0x8CB0, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0CB2, 0x96FC, 4)) return;
    SYNC_IN;
    mff_cycles(0x8CB3, 2);
    if (!z) { mff_cycles(0x8CB3, 1); goto L_8CBC; }
    mff_cycles(0x8CB5, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0CB7, 0x89D2, 4)) return;
    SYNC_IN;
    mff_cycles(0x8CB8, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8CB9:;
    mff_cycles(0x8CB9, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0CBB, 0x9000, 4)) return;
    SYNC_IN;
L_8CBC:;
    mff_cycles(0x8CBC, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8CBD (bank 4) - lifted by port/tools/lift.py */
void func_8CBD_b4(void) {
    REGS_IN;
    mff_cycles(0x8CBD, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x8CCC + y));
    mff_cycles(0x8CC2, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x8CCF + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8CC7, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $8CD2 (bank 4) - lifted by port/tools/lift.py */
void func_8CD2_b4(void) {
    REGS_IN;
    mff_cycles(0x8CD2, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DEB + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8CD7, 2);
    if (c) {
        mff_cycles(0x8CD9, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0CDB, 0x89D2, 4)) return;
        SYNC_IN;
        mff_cycles(0x8CDC, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x8CD7, 1);
    }
    mff_cycles(0x8CDD, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0CDF, 0x9000, 4)) return;
    SYNC_IN;
    mff_cycles(0x8CE0, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8CE1 (bank 4) - lifted by port/tools/lift.py */
void func_8CE1_b4(void) {
    REGS_IN;
    mff_cycles(0x8CE1, 12);
    a = 0xFF;
    RAM(KEEP_ON_SCREEN) = a;
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DEB + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8CEA, 2);
    if (c) {
        mff_cycles(0x8CEC, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0CEE, 0x96FC, 4)) return;
        SYNC_IN;
        mff_cycles(0x8CEF, 2);
        if (z) {
            mff_cycles(0x8CF1, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0CF3, 0x89D2, 4)) return;
            SYNC_IN;
        } else {
            mff_cycles(0x8CEF, 1);
        }
    } else {
        mff_cycles(0x8CEA, 1);
    }
    mff_cycles(0x8CF4, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8CF5 (bank 4) - lifted by port/tools/lift.py */
void func_8CF5_b4(void) {
    REGS_IN;
    mff_cycles(0x8CF5, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x8D04 + y));
    mff_cycles(0x8CFA, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x8D08 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8CFF, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $8D0C (bank 4) - lifted by port/tools/lift.py */
void func_8D0C_b4(void) {
    REGS_IN;
    mff_cycles(0x8D0C, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DEE + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8D11, 2);
    if (c) {
        mff_cycles(0x8D13, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0D15, 0x89D2, 4)) return;
        SYNC_IN;
        mff_cycles(0x8D16, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x8D11, 1);
    }
    mff_cycles(0x8D17, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0D19, 0x9000, 4)) return;
    SYNC_IN;
    mff_cycles(0x8D1A, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8D1B (bank 4) - lifted by port/tools/lift.py */
void func_8D1B_b4(void) {
    REGS_IN;
    mff_cycles(0x8D1B, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DEE + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8D20, 2);
    if (c) {
        mff_cycles(0x8D22, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0D24, 0x89D2, 4)) return;
        SYNC_IN;
        mff_cycles(0x8D25, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x8D20, 1);
    }
    mff_cycles(0x8D26, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0D28, 0x9000, 4)) return;
    SYNC_IN;
    mff_cycles(0x8D29, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8D2A (bank 4) - lifted by port/tools/lift.py */
void func_8D2A_b4(void) {
    REGS_IN;
    mff_cycles(0x8D2A, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DEE + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8D2F, 2);
    if (c) {
        mff_cycles(0x8D31, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0D33, 0x96FC, 4)) return;
        SYNC_IN;
        mff_cycles(0x8D34, 2);
        if (z) {
            mff_cycles(0x8D36, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0D38, 0x89D2, 4)) return;
            SYNC_IN;
        } else {
            mff_cycles(0x8D34, 1);
        }
    } else {
        mff_cycles(0x8D2F, 1);
    }
    mff_cycles(0x8D39, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8D3A (bank 4) - lifted by port/tools/lift.py */
void func_8D3A_b4(void) {
    REGS_IN;
    mff_cycles(0x8D3A, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x8D49 + y));
    mff_cycles(0x8D3F, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x8D4D + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8D44, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $8D51 (bank 4) - lifted by port/tools/lift.py */
void func_8D51_b4(void) {
    REGS_IN;
    mff_cycles(0x8D51, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DF2 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8D56, 2);
    if (c) {
        mff_cycles(0x8D58, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0D5A, 0x89D2, 4)) return;
        SYNC_IN;
        mff_cycles(0x8D5B, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x8D56, 1);
    }
    mff_cycles(0x8D5C, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0D5E, 0x9000, 4)) return;
    SYNC_IN;
    mff_cycles(0x8D5F, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8D60 (bank 4) - lifted by port/tools/lift.py */
void func_8D60_b4(void) {
    REGS_IN;
    mff_cycles(0x8D60, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DF2 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8D65, 2);
    if (c) {
        mff_cycles(0x8D67, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0D69, 0x89D2, 4)) return;
        SYNC_IN;
        mff_cycles(0x8D6A, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x8D65, 1);
    }
    mff_cycles(0x8D6B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0D6D, 0x9000, 4)) return;
    SYNC_IN;
    mff_cycles(0x8D6E, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8D6F (bank 4) - lifted by port/tools/lift.py */
void func_8D6F_b4(void) {
    REGS_IN;
    mff_cycles(0x8D6F, 12);
    a = 0xFF;
    RAM(KEEP_ON_SCREEN) = a;
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DF2 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8D78, 2);
    if (c) {
        mff_cycles(0x8D7A, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0D7C, 0x96FC, 4)) return;
        SYNC_IN;
        mff_cycles(0x8D7D, 2);
        if (z) {
            mff_cycles(0x8D7F, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0D81, 0x89D2, 4)) return;
            SYNC_IN;
        } else {
            mff_cycles(0x8D7D, 1);
        }
    } else {
        mff_cycles(0x8D78, 1);
    }
    mff_cycles(0x8D82, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8D83 (bank 4) - lifted by port/tools/lift.py */
void func_8D83_b4(void) {
    REGS_IN;
    mff_cycles(0x8D83, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x8D92 + y));
    mff_cycles(0x8D88, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x8D95 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8D8D, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $8D98 (bank 4) - lifted by port/tools/lift.py */
void func_8D98_b4(void) {
    REGS_IN;
    mff_cycles(0x8D98, 7);
    a = RAM(AREA_KILLS);
    m = prg_read((uint16_t)(0x8DF6 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x8D9D, 2);
    if (c) {
        mff_cycles(0x8D9F, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0DA1, 0x96FC, 4)) return;
        SYNC_IN;
        mff_cycles(0x8DA2, 2);
        if (z) {
            mff_cycles(0x8DA4, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0DA6, 0x89D2, 4)) return;
            SYNC_IN;
        } else {
            mff_cycles(0x8DA2, 1);
        }
    } else {
        mff_cycles(0x8D9D, 1);
    }
    mff_cycles(0x8DA7, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8DA8 (bank 4) - lifted by port/tools/lift.py */
void func_8DA8_b4(void) {
    REGS_IN;
    mff_cycles(0x8DA8, 7);
    a = 0xFF;
    RAM(KEEP_ON_SCREEN) = a;
    y = 0x0F;
L_8DAE:;
    mff_cycles(0x8DAE, 8);
    a = RAM(OBJ_ID + y);
    m = 0x74;
    c = a >= m;
    if (a == m) { mff_cycles(0x8DB3, 1); goto L_8DBB; }
    mff_cycles(0x8DB5, 6);
    y--;
    m = 0x03;
    c = y >= m;
    n = ((uint8_t)(y - m)) >> 7;
    z = ((uint8_t)(y - m)) == 0;
    if (!z) { mff_cycles(0x8DAE, 1); goto L_8DAE; }
    mff_cycles(0x8DBA, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8DBB:;
    mff_cycles(0x8DBB, 6);
    a = RAM(OBJ_LIFE + y);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0x8DC0, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0DC2, 0x89D2, 4)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x8DBE, 1);
    }
    mff_cycles(0x8DC3, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/* timed_spawner $9000 (bank 4) - lifted by port/tools/lift.py */
void func_9000_b4(void) {
    REGS_IN;
    mff_cycles(0x9000, 5);
    a = RAM(CAMERA_LOCK);
    if (a != 0) { mff_cycles(0x9002, 1); goto L_9012; }
    mff_cycles(0x9004, 7);
    a = RAM(ROUND);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0x900A, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9008, 1);
    }
    mff_cycles(0x900B, 7);
    a = RAM(SUB_AREA);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x900F, 1); goto L_9012; }
L_9011:;
    mff_cycles(0x9011, 6);
    SYNC_OUT;
    mff_rts(); return;
L_9012:;
    mff_cycles(0x9012, 5);
    a = RAM(AREA_CLEAR);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) { mff_cycles(0x9011, 1); goto L_9011; }
    mff_cycles(0x9016, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1018, 0x96C3, 4)) return;
    SYNC_IN;
    mff_cycles(0x9019, 2);
    if (!z) { mff_cycles(0x9011, 1); goto L_9011; }
    mff_cycles(0x901B, 12);
    t = (uint8_t)(RAM(SPAWN_TIMER) - 1);
    RAM(SPAWN_TIMER) = t;
    a = RAM(SPAWN_TIMER);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x9021, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1038), 4); return; }
    mff_cycles(0x9023, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9038 (bank 4) - lifted by port/tools/lift.py */
void func_9038_b4(void) {
    REGS_IN;
    mff_cycles(0x9038, 19);
    a = RAM(RNG);
    c = 0;
    m = RAM(RNG + 1);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(RNG) = a;
    a &= 0x0F;
    y = a;
    a = prg_read((uint16_t)(0x9024 + y));
    mff_cycles(0x9045, 10);
    RAM(SPAWN_TIMER) = a;
    a = RAM(ROUND);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x904D, 7);
        a = RAM(SUB_AREA);
        m = 0x01;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) {
            mff_cycles(0x9053, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x921C, 2); call_by_address_tail(0x921C, 4); return;
        } else {
            mff_cycles(0x9051, 1);
        }
    } else {
        mff_cycles(0x904B, 1);
    }
    mff_cycles(0x9056, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1058, func_F71A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9059, 2);
    if (c) { mff_cycles(0x9059, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1011), 4); return; }
    mff_cycles(0x905B, 152);
    RAM(SPAWN_SLOT) = y;
    a = RAM(PLAYER_DIR);
    RAM(OBJ_DIR + y) = a;
    a = 0x00;
    RAM(OBJ_THROW_FLAG + y) = a;
    RAM(OBJ_HIT_COOLDOWN + y) = a;
    RAM(OBJ_INVULN + y) = a;
    RAM(OBJ_HIT_PLAYER + y) = a;
    RAM(OBJ_ATTACK_DAMAGE + y) = a;
    RAM(OBJ_ATTACK + y) = a;
    RAM(OBJ_HIT_TIMER + y) = a;
    RAM(OBJ_HELD + y) = a;
    RAM(OBJ_TIER + y) = a;
    RAM(OBJ_AI_TIMER + y) = a;
    RAM(OBJ_AI_COUNT + y) = a;
    RAM(OBJ_AI_FLAG + y) = a;
    RAM(OBJ_TIMER + y) = a;
    RAM(OBJ_TIMER2 + y) = a;
    RAM(OBJ_GROUND_Y + y) = a;
    RAM(OBJ_LAND_Y + y) = a;
    RAM(OBJ_TIMER3 + y) = a;
    RAM(OBJ_DOWN_TIMER + y) = a;
    RAM(OBJ_VX_FRAC + y) = a;
    RAM(OBJ_VX + y) = a;
    a = 0xC0;
    RAM(OBJ_VY_FRAC + y) = a;
    a = 0xFF;
    RAM(OBJ_VY + y) = a;
    a = RAM(RNG);
    m = RAM(RNG + 1);
    w = a + m + c;
    a = (uint8_t)w;
    RAM(RNG) = a;
    c = 1;
    m = RAM(RNG + 2);
    w = a - m - !c;
    a = (uint8_t)w;
    RAM(RNG + 1) = a;
    a &= 0x1F;
    x = a;
    a = prg_read((uint16_t)(0x964B + x));
    mff_cycles(0x90BB, 14);
    RAM(OBJ_X + y) = a;
    a = RAM(SCROLL_X_HI);
    c = 0;
    m = prg_read((uint16_t)(0x966B + x));
    w = a + m + c;
    a = (uint8_t)w;
    mff_cycles(0x90C4, 9);
    RAM(OBJ_X_HI + y) = a;
    a = prg_read((uint16_t)(0x968B + x));
    mff_cycles(0x90CA, 10);
    RAM(OBJ_Y + y) = a;
    a = RAM(ROUND);
    if (a != 0) { mff_cycles(0x90CF, 1); goto L_90EF; }
    mff_cycles(0x90D1, 7);
    a = RAM(SUB_AREA);
    if (a != 0x01) { mff_cycles(0x90D5, 1); goto L_90EF; }
    mff_cycles(0x90D7, 8);
    a = RAM(OBJ_Y + y);
    m = 0x98;
    c = a >= m;
    if (!c) { mff_cycles(0x90DC, 2); goto L_910A; }
    mff_cycles(0x90DE, 17);
    a = RAM(RNG + 2);
    m = RAM(RNG + 1);
    w = a + m + c;
    a = (uint8_t)w;
    RAM(RNG + 2) = a;
    a &= 0x07;
    x = a;
    a = prg_read((uint16_t)(0x96AB + x));
    mff_cycles(0x90EA, 7);
    RAM(OBJ_Y + y) = a;
    if (a != 0) { mff_cycles(0x90ED, 2); goto L_910A; }
L_90EF:;
    mff_cycles(0x90EF, 7);
    a = RAM(ROUND);
    if (a == 0x01) {
        mff_cycles(0x90F5, 7);
        a = RAM(SUB_AREA);
        m = 0x01;
        c = a >= m;
        if (a == m) {
            mff_cycles(0x90FB, 17);
            a = RAM(RNG + 2);
            m = RAM(RNG + 1);
            w = a + m + c;
            a = (uint8_t)w;
            RAM(RNG + 2) = a;
            a &= 0x07;
            x = a;
            a = prg_read((uint16_t)(0x96B3 + x));
            mff_cycles(0x9107, 5);
            RAM(OBJ_Y + y) = a;
        } else {
            mff_cycles(0x90F9, 2);
        }
    } else {
        mff_cycles(0x90F3, 2);
    }
L_910A:;
    mff_cycles(0x910A, 4);
    x = 0x0D;
    y = 0x01;
    for (;;) {
        mff_cycles(0x910E, 6);
        a = RAM(OBJ_STATE + x);
        if (a & 0x80) {
            mff_cycles(0x9113, 6);
            a = RAM(OBJ_SPAWN_GROUP + x);
            if (!(a & 0x80)) {
                mff_cycles(0x9118, 7);
                RAM(SPAWN_GROUPS + y) = a;
                y--;
            } else {
                mff_cycles(0x9116, 1);
            }
        } else {
            mff_cycles(0x9111, 1);
        }
        mff_cycles(0x911C, 6);
        x--;
        if (x == 0x02) break;
        mff_cycles(0x910E, 1);
    }
    mff_cycles(0x9121, 19);
    x = 0x02;
    a = RAM(SPAWN_GROUPS);
    c = 0;
    m = RAM(SPAWN_GROUPS + 1);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(TMP1) = a;
    if (a == 0x07) { mff_cycles(0x912E, 1); goto L_9174; }
    mff_cycles(0x9130, 4);
    if (a == 0x08) { mff_cycles(0x9132, 1); goto L_9174; }
    mff_cycles(0x9134, 4);
    if (a == 0x09) { mff_cycles(0x9136, 1); goto L_9174; }
    mff_cycles(0x9138, 4);
    if (a == 0x00) { mff_cycles(0x913A, 1); goto L_9158; }
    mff_cycles(0x913C, 4);
    if (a == 0x03) { mff_cycles(0x913E, 1); goto L_915E; }
    mff_cycles(0x9140, 4);
    if (a == 0x04) { mff_cycles(0x9142, 1); goto L_9164; }
    mff_cycles(0x9144, 4);
    if (a == 0x05) { mff_cycles(0x9146, 1); goto L_916A; }
    mff_cycles(0x9148, 4);
    m = 0xFF;
    c = a >= m;
    if (a == m) { mff_cycles(0x914A, 1); goto L_9170; }
    mff_cycles(0x914C, 20);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SPAWN_GROUPS) = a;
    RAM(SPAWN_GROUPS + 1) = a;
    RAM(SPAWN_GROUPS + 2) = a;
    SYNC_OUT;
    mff_rts(); return;
L_9158:;
    mff_cycles(0x9158, 7);
    a = 0x09;
    RAM(TMP1) = a;
    if (a != 0) { mff_cycles(0x915C, 1); goto L_9174; }
L_915E:;
    mff_cycles(0x915E, 7);
    a = 0x07;
    RAM(TMP1) = a;
    if (a != 0) { mff_cycles(0x9162, 1); goto L_9174; }
L_9164:;
    mff_cycles(0x9164, 7);
    a = 0x09;
    RAM(TMP1) = a;
    if (a != 0) { mff_cycles(0x9168, 1); goto L_9174; }
L_916A:;
    mff_cycles(0x916A, 7);
    a = 0x08;
    RAM(TMP1) = a;
    if (a != 0) { mff_cycles(0x916E, 1); goto L_9174; }
L_9170:;
    mff_cycles(0x9170, 5);
    a = 0x07;
    RAM(TMP1) = a;
L_9174:;
    mff_cycles(0x9174, 27);
    a = 0x0C;
    c = 1;
    m = RAM(TMP1);
    w = a - m - !c;
    a = (uint8_t)w;
    RAM(TMP1) = a;
    a = 0x05;
    c = 1;
    m = RAM(TMP1);
    w = a - m - !c;
    a = (uint8_t)w;
    RAM(TMP1) = a;
    y = RAM(ROUND);
    a = prg_read((uint16_t)(0x9363 + y));
    mff_cycles(0x9187, 7);
    RAM(TMP2) = a;
    a = prg_read((uint16_t)(0x9368 + y));
    mff_cycles(0x918C, 11);
    RAM(TMP3) = a;
    y = RAM(SUB_AREA);
    a = mem_read((uint16_t)(RAM16(TMP2) + y));
    mff_cycles(0x9192, 6);
    y = a;
    a = prg_read((uint16_t)(0x937C + y));
    mff_cycles(0x9196, 7);
    RAM(TMP4) = a;
    a = prg_read((uint16_t)(0x938B + y));
    mff_cycles(0x919B, 11);
    RAM(TMP5) = a;
    y = RAM(SCROLL_X_HI);
    a = mem_read((uint16_t)(RAM16(TMP4) + y));
    mff_cycles(0x91A1, 14);
    c = 0;
    m = RAM(TMP1);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    x = a;
    y = RAM(SPAWN_SLOT);
    a = prg_read((uint16_t)(0x94DD + x));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x91AA, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x11AC, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x91AD, 4);
    a = prg_read((uint16_t)(0x9456 + x));
    mff_cycles(0x91B0, 14);
    RAM(OBJ_ID + y) = a;
    RAM(OBJ_SAVED_ID + y) = a;
    a = prg_read((uint16_t)(0x9564 + x));
    mff_cycles(0x91B9, 9);
    RAM(OBJ_KIND + y) = a;
    a = prg_read((uint16_t)(0x93CF + x));
    mff_cycles(0x91BF, 31);
    RAM(OBJ_SPAWN_GROUP + y) = a;
    RAM(TMP2) = x;
    a = RAM(RNG);
    c = 0;
    m = RAM(RNG + 1);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(RNG) = a;
    a &= 0x0F;
    x = a;
    a = RAM(OBJ_ANIM + y);
    m = 0xB7;
    c = a >= m;
    if (a == m) { mff_cycles(0x91D3, 2); goto L_9209; }
    mff_cycles(0x91D5, 7);
    a = RAM(ROUND);
    m = 0x00;
    c = a >= m;
    if (a == m) { mff_cycles(0x91D9, 1); goto L_91EB; }
    mff_cycles(0x91DB, 4);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0x91DD, 1); goto L_91F0; }
    mff_cycles(0x91DF, 4);
    m = 0x02;
    c = a >= m;
    if (a == m) { mff_cycles(0x91E1, 1); goto L_91F5; }
    mff_cycles(0x91E3, 4);
    m = 0x03;
    c = a >= m;
    if (a == m) { mff_cycles(0x91E5, 1); goto L_91FA; }
    mff_cycles(0x91E7, 4);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0x91E9, 1); goto L_91FF; }
L_91EB:;
    mff_cycles(0x91EB, 4);
    a = prg_read((uint16_t)(0x95EB + x));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x91EE, 2);
    if (!z) { mff_cycles(0x91EE, 2); goto L_9202; }
L_91F0:;
    mff_cycles(0x91F0, 4);
    a = prg_read((uint16_t)(0x95FB + x));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x91F3, 2);
    if (!z) { mff_cycles(0x91F3, 2); goto L_9202; }
L_91F5:;
    mff_cycles(0x91F5, 4);
    a = prg_read((uint16_t)(0x960B + x));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x91F8, 2);
    if (!z) { mff_cycles(0x91F8, 2); goto L_9202; }
L_91FA:;
    mff_cycles(0x91FA, 4);
    a = prg_read((uint16_t)(0x961B + x));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x91FD, 2);
    if (!z) { mff_cycles(0x91FD, 2); goto L_9202; }
L_91FF:;
    mff_cycles(0x91FF, 4);
    a = prg_read((uint16_t)(0x962B + x));
    n = (a) >> 7;
    z = (a) == 0;
L_9202:;
    mff_cycles(0x9202, 16);
    RAM(OBJ_LIFE + y) = a;
    RAM(OBJ_LIFE_MAX + y) = a;
    SYNC_OUT;
    mff_rts(); return;
L_9209:;
    mff_cycles(0x9209, 17);
    a = RAM(RNG + 2);
    m = RAM(RNG + 1);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(RNG + 2) = a;
    a &= 0x0F;
    x = a;
    a = prg_read((uint16_t)(0x963B + x));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9215, 16);
    RAM(OBJ_LIFE + y) = a;
    RAM(OBJ_LIFE_MAX + y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $921C (bank 4) - lifted by port/tools/lift.py */
void func_921C_b4(void) {
    REGS_IN;
    goto L_921C;
L_921B:;
    mff_cycles(0x921B, 6);
    SYNC_OUT;
    mff_rts(); return;
L_921C:;
    mff_cycles(0x921C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x121E, func_F71A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x921F, 2);
    if (c) { mff_cycles(0x921B, 1); goto L_921B; }
    mff_cycles(0x9221, 149);
    RAM(TMP0) = y;
    t = (uint8_t)(RAM(AREA_KILLS) + 1);
    RAM(AREA_KILLS) = t;
    a = RAM(PLAYER_DIR);
    RAM(OBJ_DIR + y) = a;
    a = 0x00;
    RAM(OBJ_THROW_FLAG + y) = a;
    RAM(OBJ_HIT_COOLDOWN + y) = a;
    RAM(OBJ_INVULN + y) = a;
    RAM(OBJ_HIT_PLAYER + y) = a;
    RAM(OBJ_ATTACK_DAMAGE + y) = a;
    RAM(OBJ_ATTACK + y) = a;
    RAM(OBJ_HIT_TIMER + y) = a;
    RAM(OBJ_HELD + y) = a;
    RAM(OBJ_TIER + y) = a;
    RAM(OBJ_AI_TIMER + y) = a;
    RAM(OBJ_AI_COUNT + y) = a;
    RAM(OBJ_AI_FLAG + y) = a;
    RAM(OBJ_TIMER + y) = a;
    RAM(OBJ_TIMER2 + y) = a;
    RAM(OBJ_GROUND_Y + y) = a;
    RAM(OBJ_LAND_Y + y) = a;
    RAM(OBJ_TIMER3 + y) = a;
    RAM(OBJ_DOWN_TIMER + y) = a;
    RAM(OBJ_VX_FRAC + y) = a;
    RAM(OBJ_VX + y) = a;
    a = 0xC0;
    RAM(OBJ_VY_FRAC + y) = a;
    a = 0xFF;
    RAM(OBJ_VY + y) = a;
    a = RAM(RNG);
    m = RAM(RNG + 1);
    w = a + m + c;
    a = (uint8_t)w;
    RAM(RNG) = a;
    a &= 0x07;
    x = a;
    a = prg_read((uint16_t)(0x96BB + x));
    mff_cycles(0x927E, 23);
    RAM(OBJ_X + y) = a;
    a = 0x30;
    RAM(OBJ_Y + y) = a;
    a = 0x00;
    RAM(OBJ_X_HI + y) = a;
    x = 0x0D;
    y = 0x01;
    for (;;) {
        mff_cycles(0x928F, 6);
        a = RAM(OBJ_STATE + x);
        if (a & 0x80) {
            mff_cycles(0x9294, 6);
            a = RAM(OBJ_SPAWN_GROUP + x);
            if (!(a & 0x80)) {
                mff_cycles(0x9299, 7);
                RAM(SPAWN_GROUPS + y) = a;
                y--;
            } else {
                mff_cycles(0x9297, 1);
            }
        } else {
            mff_cycles(0x9292, 1);
        }
        mff_cycles(0x929D, 6);
        x--;
        if (x == 0x02) break;
        mff_cycles(0x928F, 1);
    }
    mff_cycles(0x92A2, 19);
    x = 0x02;
    a = RAM(SPAWN_GROUPS);
    c = 0;
    m = RAM(SPAWN_GROUPS + 1);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(TMP1) = a;
    if (a == 0x07) { mff_cycles(0x92AF, 1); goto L_92E8; }
    mff_cycles(0x92B1, 4);
    if (a == 0x08) { mff_cycles(0x92B3, 1); goto L_92E8; }
    mff_cycles(0x92B5, 4);
    if (a == 0x09) { mff_cycles(0x92B7, 1); goto L_92E8; }
    mff_cycles(0x92B9, 4);
    if (a == 0x00) { mff_cycles(0x92BB, 1); goto L_92D2; }
    mff_cycles(0x92BD, 4);
    if (a == 0x04) { mff_cycles(0x92BF, 1); goto L_92D8; }
    mff_cycles(0x92C1, 4);
    if (a == 0x05) { mff_cycles(0x92C3, 1); goto L_92DE; }
    mff_cycles(0x92C5, 4);
    m = 0xFF;
    c = a >= m;
    if (a == m) { mff_cycles(0x92C7, 1); goto L_92E4; }
    mff_cycles(0x92C9, 16);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SPAWN_GROUPS) = a;
    RAM(SPAWN_GROUPS + 1) = a;
    SYNC_OUT;
    mff_rts(); return;
L_92D2:;
    mff_cycles(0x92D2, 7);
    a = 0x07;
    RAM(TMP1) = a;
    if (a != 0) { mff_cycles(0x92D6, 1); goto L_92E8; }
L_92D8:;
    mff_cycles(0x92D8, 7);
    a = 0x07;
    RAM(TMP1) = a;
    if (a != 0) { mff_cycles(0x92DC, 1); goto L_92E8; }
L_92DE:;
    mff_cycles(0x92DE, 7);
    a = 0x08;
    RAM(TMP1) = a;
    if (a != 0) { mff_cycles(0x92E2, 1); goto L_92E8; }
L_92E4:;
    mff_cycles(0x92E4, 5);
    a = 0x07;
    RAM(TMP1) = a;
L_92E8:;
    mff_cycles(0x92E8, 24);
    a = 0x0C;
    c = 1;
    m = RAM(TMP1);
    w = a - m - !c;
    a = (uint8_t)w;
    RAM(TMP1) = a;
    a = 0x05;
    c = 1;
    m = RAM(TMP1);
    w = a - m - !c;
    a = (uint8_t)w;
    RAM(TMP1) = a;
    if (a == 0x00) { mff_cycles(0x92F8, 1); goto L_92FE; }
    mff_cycles(0x92FA, 4);
    if (a == 0x01) { mff_cycles(0x92FC, 2); goto L_930F; }
L_92FE:;
    mff_cycles(0x92FE, 19);
    a = RAM(RNG);
    c = 0;
    m = RAM(RNG + 1);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(RNG) = a;
    a &= 0x01;
    x = a;
    a = prg_read((uint16_t)(0x934D + x));
    mff_cycles(0x930B, 5);
    x = a;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x931D, 2); call_by_address_tail(0x931D, 4); return;
L_930F:;
    mff_cycles(0x930F, 19);
    a = RAM(RNG + 1);
    c = 0;
    m = RAM(RNG + 2);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(RNG + 1) = a;
    a &= 0x01;
    x = a;
    a = prg_read((uint16_t)(0x934B + x));
    mff_cycles(0x931C, 9);
    x = a;
    y = RAM(TMP0);
    a = prg_read((uint16_t)(0x934F + x));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9322, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1324, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9325, 11);
    a = 0xD4;
    RAM(OBJ_FLAGS + y) = a;
    a = prg_read((uint16_t)(0x9353 + x));
    mff_cycles(0x932D, 14);
    RAM(OBJ_ID + y) = a;
    RAM(OBJ_SAVED_ID + y) = a;
    a = prg_read((uint16_t)(0x9357 + x));
    mff_cycles(0x9336, 9);
    RAM(OBJ_KIND + y) = a;
    a = prg_read((uint16_t)(0x935B + x));
    mff_cycles(0x933C, 12);
    RAM(OBJ_SPAWN_GROUP + y) = a;
    RAM(TMP2) = x;
    a = prg_read((uint16_t)(0x935F + x));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9344, 16);
    RAM(OBJ_LIFE + y) = a;
    RAM(OBJ_LIFE_MAX + y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $931D (bank 4) - lifted by port/tools/lift.py */
void func_931D_b4(void) {
    REGS_IN;
    mff_cycles(0x931D, 7);
    y = RAM(TMP0);
    a = prg_read((uint16_t)(0x934F + x));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9322, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1324, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9325, 11);
    a = 0xD4;
    RAM(OBJ_FLAGS + y) = a;
    a = prg_read((uint16_t)(0x9353 + x));
    mff_cycles(0x932D, 14);
    RAM(OBJ_ID + y) = a;
    RAM(OBJ_SAVED_ID + y) = a;
    a = prg_read((uint16_t)(0x9357 + x));
    mff_cycles(0x9336, 9);
    RAM(OBJ_KIND + y) = a;
    a = prg_read((uint16_t)(0x935B + x));
    mff_cycles(0x933C, 12);
    RAM(OBJ_SPAWN_GROUP + y) = a;
    RAM(TMP2) = x;
    a = prg_read((uint16_t)(0x935F + x));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9344, 16);
    RAM(OBJ_LIFE + y) = a;
    RAM(OBJ_LIFE_MAX + y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $96C3 (bank 4) - lifted by port/tools/lift.py */
void func_96C3_b4(void) {
    REGS_IN;
    mff_cycles(0x96C3, 7);
    a = 0x00;
    RAM(TMP0) = a;
    y = 0x0D;
    for (;;) {
        mff_cycles(0x96C9, 6);
        a = RAM(OBJ_STATE + y);
        if (a & 0x80) {
            mff_cycles(0x96CE, 8);
            a = RAM(OBJ_ANIM + y);
            if (a >= 0x80) {
                mff_cycles(0x96D5, 4);
                if (a != 0xBB) {
                    mff_cycles(0x96D9, 4);
                    if (a != 0xB9) {
                        mff_cycles(0x96DD, 4);
                        if (a != 0xBA) {
                            mff_cycles(0x96E1, 4);
                            if (a != 0xC7) {
                                mff_cycles(0x96E5, 4);
                                if (a != 0xC8) {
                                    mff_cycles(0x96E9, 5);
                                    t = (uint8_t)(RAM(TMP0) + 1);
                                    RAM(TMP0) = t;
                                } else {
                                    mff_cycles(0x96E7, 1);
                                }
                            } else {
                                mff_cycles(0x96E3, 1);
                            }
                        } else {
                            mff_cycles(0x96DF, 1);
                        }
                    } else {
                        mff_cycles(0x96DB, 1);
                    }
                } else {
                    mff_cycles(0x96D7, 1);
                }
            } else {
                mff_cycles(0x96D3, 1);
            }
        } else {
            mff_cycles(0x96CC, 1);
        }
        mff_cycles(0x96EB, 6);
        y--;
        if (y == 0x02) break;
        mff_cycles(0x96C9, 1);
    }
    mff_cycles(0x96F0, 7);
    a = RAM(TMP0);
    m = 0x02;
    c = a >= m;
    if (!c) {
        mff_cycles(0x96F6, 8);
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x96F4, 1);
    }
    mff_cycles(0x96F9, 8);
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $96FC (bank 4) - lifted by port/tools/lift.py */
void func_96FC_b4(void) {
    REGS_IN;
    mff_cycles(0x96FC, 7);
    a = 0x00;
    RAM(TMP0) = a;
    y = 0x0D;
L_9702:;
    mff_cycles(0x9702, 6);
    a = RAM(OBJ_STATE + y);
    if (!(a & 0x80)) { mff_cycles(0x9705, 1); goto L_971B; }
    mff_cycles(0x9707, 8);
    a = RAM(OBJ_ID + y);
    if (a == 0x91) { mff_cycles(0x970C, 1); goto L_9719; }
    mff_cycles(0x970E, 4);
    if (a == 0x95) { mff_cycles(0x9710, 1); goto L_9719; }
    mff_cycles(0x9712, 8);
    a = RAM(OBJ_ANIM + y);
    if (a < 0x80) { mff_cycles(0x9717, 1); goto L_971B; }
L_9719:;
    mff_cycles(0x9719, 5);
    t = (uint8_t)(RAM(TMP0) + 1);
    RAM(TMP0) = t;
L_971B:;
    mff_cycles(0x971B, 6);
    y--;
    m = 0x02;
    c = y >= m;
    if (y != m) { mff_cycles(0x9702, 1); goto L_9702; }
    mff_cycles(0x9720, 5);
    a = RAM(TMP0);
    if (a == 0) {
        mff_cycles(0x9724, 8);
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9722, 1);
    }
    mff_cycles(0x9727, 8);
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $972A (bank 4) - lifted by port/tools/lift.py */
void func_972A_b4(void) {
    REGS_IN;
    mff_cycles(0x972A, 7);
    y = RAM(ROUND);
    a = prg_read((uint16_t)(0x9748 + y));
    mff_cycles(0x972F, 7);
    RAM(TMP2) = a;
    a = prg_read((uint16_t)(0x974D + y));
    mff_cycles(0x9734, 11);
    RAM(TMP3) = a;
    y = RAM(SUB_AREA);
    a = mem_read((uint16_t)(RAM16(TMP2) + y));
    mff_cycles(0x973A, 6);
    y = a;
    a = prg_read((uint16_t)(0x9761 + y));
    mff_cycles(0x973E, 7);
    RAM(TMP4) = a;
    a = prg_read((uint16_t)(0x9770 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9743, 8);
    RAM(TMP5) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0004); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $977F (bank 4) - lifted by port/tools/lift.py */
void func_977F_b4(void) {
    REGS_IN;
    mff_cycles(0x977F, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x99C2 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9784, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1786, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x9787, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9787 (bank 4) - lifted by port/tools/lift.py */
void func_9787_b4(void) {
    REGS_IN;
    mff_cycles(0x9787, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9788 (bank 4) - lifted by port/tools/lift.py */
void func_9788_b4(void) {
    REGS_IN;
    mff_cycles(0x9788, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x9797 + y));
    mff_cycles(0x978D, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x979B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9792, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $979F (bank 4) - lifted by port/tools/lift.py */
void func_979F_b4(void) {
    REGS_IN;
    mff_cycles(0x979F, 4);
    a = prg_read((uint16_t)(0x99C6 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x97A2, 6);
    SYNC_OUT;
    if (!mff_call_at(0x17A4, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x97A5, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $97A6 (bank 4) - lifted by port/tools/lift.py */
void func_97A6_b4(void) {
    REGS_IN;
    mff_cycles(0x97A6, 4);
    a = prg_read((uint16_t)(0x99C6 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x97A9, 6);
    SYNC_OUT;
    if (!mff_call_at(0x17AB, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x97AC, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $97B4 (bank 4) - lifted by port/tools/lift.py */
void func_97B4_b4(void) {
    REGS_IN;
    mff_cycles(0x97B4, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x97C3 + y));
    mff_cycles(0x97B9, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x97C5 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x97BE, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $97C7 (bank 4) - lifted by port/tools/lift.py */
void func_97C7_b4(void) {
    REGS_IN;
    mff_cycles(0x97C7, 4);
    a = prg_read((uint16_t)(0x99CA + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x97CA, 6);
    SYNC_OUT;
    if (!mff_call_at(0x17CC, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x97CD, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $97CE (bank 4) - lifted by port/tools/lift.py */
void func_97CE_b4(void) {
    REGS_IN;
    mff_cycles(0x97CE, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x97DD + y));
    mff_cycles(0x97D3, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x97E1 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x97D8, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $97E5 (bank 4) - lifted by port/tools/lift.py */
void func_97E5_b4(void) {
    REGS_IN;
    mff_cycles(0x97E5, 4);
    a = prg_read((uint16_t)(0x99CC + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x97E8, 6);
    SYNC_OUT;
    if (!mff_call_at(0x17EA, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x97EB, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $97EC (bank 4) - lifted by port/tools/lift.py */
void func_97EC_b4(void) {
    REGS_IN;
    mff_cycles(0x97EC, 4);
    a = prg_read((uint16_t)(0x99CC + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x97EF, 6);
    SYNC_OUT;
    if (!mff_call_at(0x17F1, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x97F2, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $97FA (bank 4) - lifted by port/tools/lift.py */
void func_97FA_b4(void) {
    REGS_IN;
    mff_cycles(0x97FA, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x9809 + y));
    mff_cycles(0x97FF, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x980E + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9804, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $9813 (bank 4) - lifted by port/tools/lift.py */
void func_9813_b4(void) {
    REGS_IN;
    mff_cycles(0x9813, 4);
    a = prg_read((uint16_t)(0x99D0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9816, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1818, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x9819, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $981A (bank 4) - lifted by port/tools/lift.py */
void func_981A_b4(void) {
    REGS_IN;
    mff_cycles(0x981A, 4);
    a = prg_read((uint16_t)(0x99D0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x981D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x181F, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x9820, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9821 (bank 4) - lifted by port/tools/lift.py */
void func_9821_b4(void) {
    REGS_IN;
    mff_cycles(0x9821, 4);
    a = prg_read((uint16_t)(0x99D0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9824, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1826, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x9827, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $982F (bank 4) - lifted by port/tools/lift.py */
void func_982F_b4(void) {
    REGS_IN;
    mff_cycles(0x982F, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x983E + y));
    mff_cycles(0x9834, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x9840 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9839, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $9842 (bank 4) - lifted by port/tools/lift.py */
void func_9842_b4(void) {
    REGS_IN;
    mff_cycles(0x9842, 4);
    a = prg_read((uint16_t)(0x99D5 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9845, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1847, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x9848, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9849 (bank 4) - lifted by port/tools/lift.py */
void func_9849_b4(void) {
    REGS_IN;
    mff_cycles(0x9849, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x9858 + y));
    mff_cycles(0x984E, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x985C + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9853, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $9860 (bank 4) - lifted by port/tools/lift.py */
void func_9860_b4(void) {
    REGS_IN;
    mff_cycles(0x9860, 4);
    a = prg_read((uint16_t)(0x99D7 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9863, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1865, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x9866, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9875 (bank 4) - lifted by port/tools/lift.py */
void func_9875_b4(void) {
    REGS_IN;
    mff_cycles(0x9875, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x9884 + y));
    mff_cycles(0x987A, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x9888 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x987F, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $988C (bank 4) - lifted by port/tools/lift.py */
void func_988C_b4(void) {
    REGS_IN;
    mff_cycles(0x988C, 4);
    a = prg_read((uint16_t)(0x99DB + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x988F, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1891, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x9892, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9893 (bank 4) - lifted by port/tools/lift.py */
void func_9893_b4(void) {
    REGS_IN;
    mff_cycles(0x9893, 4);
    a = prg_read((uint16_t)(0x99DB + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9896, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1898, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x9899, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $98A1 (bank 4) - lifted by port/tools/lift.py */
void func_98A1_b4(void) {
    REGS_IN;
    mff_cycles(0x98A1, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x98B0 + y));
    mff_cycles(0x98A6, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x98B2 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x98AB, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $98B4 (bank 4) - lifted by port/tools/lift.py */
void func_98B4_b4(void) {
    REGS_IN;
    mff_cycles(0x98B4, 4);
    a = prg_read((uint16_t)(0x99E4 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x98B7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x18B9, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x98BA, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $98BB (bank 4) - lifted by port/tools/lift.py */
void func_98BB_b4(void) {
    REGS_IN;
    mff_cycles(0x98BB, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x98CA + y));
    mff_cycles(0x98C0, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x98CE + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x98C5, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $98D2 (bank 4) - lifted by port/tools/lift.py */
void func_98D2_b4(void) {
    REGS_IN;
    mff_cycles(0x98D2, 4);
    a = prg_read((uint16_t)(0x99E0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x98D5, 6);
    SYNC_OUT;
    if (!mff_call_at(0x18D7, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x98D8, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $98D9 (bank 4) - lifted by port/tools/lift.py */
void func_98D9_b4(void) {
    REGS_IN;
    mff_cycles(0x98D9, 4);
    a = prg_read((uint16_t)(0x99E0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x98DC, 6);
    SYNC_OUT;
    if (!mff_call_at(0x18DE, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x98DF, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $98E0 (bank 4) - lifted by port/tools/lift.py */
void func_98E0_b4(void) {
    REGS_IN;
    mff_cycles(0x98E0, 4);
    a = prg_read((uint16_t)(0x99E0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x98E3, 6);
    SYNC_OUT;
    if (!mff_call_at(0x18E5, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x98E6, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $98E7 (bank 4) - lifted by port/tools/lift.py */
void func_98E7_b4(void) {
    REGS_IN;
    mff_cycles(0x98E7, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x98F6 + y));
    mff_cycles(0x98EC, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x98F8 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x98F1, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $98FA (bank 4) - lifted by port/tools/lift.py */
void func_98FA_b4(void) {
    REGS_IN;
    mff_cycles(0x98FA, 4);
    a = prg_read((uint16_t)(0x99ED + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x98FD, 6);
    SYNC_OUT;
    if (!mff_call_at(0x18FF, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x9900, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9901 (bank 4) - lifted by port/tools/lift.py */
void func_9901_b4(void) {
    REGS_IN;
    mff_cycles(0x9901, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x9910 + y));
    mff_cycles(0x9906, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x9914 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x990B, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $9918 (bank 4) - lifted by port/tools/lift.py */
void func_9918_b4(void) {
    REGS_IN;
    mff_cycles(0x9918, 4);
    a = prg_read((uint16_t)(0x99EA + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x991B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x191D, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x991E, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $991F (bank 4) - lifted by port/tools/lift.py */
void func_991F_b4(void) {
    REGS_IN;
    mff_cycles(0x991F, 4);
    a = prg_read((uint16_t)(0x99EB + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9922, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1924, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x9925, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9926 (bank 4) - lifted by port/tools/lift.py */
void func_9926_b4(void) {
    REGS_IN;
    mff_cycles(0x9926, 4);
    a = prg_read((uint16_t)(0x99F2 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9929, 6);
    SYNC_OUT;
    if (!mff_call_at(0x192B, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x992C, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9947 (bank 4) - lifted by port/tools/lift.py */
void func_9947_b4(void) {
    REGS_IN;
    mff_cycles(0x9947, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x9956 + y));
    mff_cycles(0x994C, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x995A + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9951, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $995E (bank 4) - lifted by port/tools/lift.py */
void func_995E_b4(void) {
    REGS_IN;
    mff_cycles(0x995E, 4);
    a = prg_read((uint16_t)(0x99EC + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9961, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1963, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x9964, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9965 (bank 4) - lifted by port/tools/lift.py */
void func_9965_b4(void) {
    REGS_IN;
    mff_cycles(0x9965, 4);
    a = prg_read((uint16_t)(0x99EC + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9968, 6);
    SYNC_OUT;
    if (!mff_call_at(0x196A, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x996B, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $996C (bank 4) - lifted by port/tools/lift.py */
void func_996C_b4(void) {
    REGS_IN;
    mff_cycles(0x996C, 4);
    a = prg_read((uint16_t)(0x99EC + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x996F, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1971, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x9972, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9973 (bank 4) - lifted by port/tools/lift.py */
void func_9973_b4(void) {
    REGS_IN;
    mff_cycles(0x9973, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x9982 + y));
    mff_cycles(0x9978, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x9986 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x997D, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $998A (bank 4) - lifted by port/tools/lift.py */
void func_998A_b4(void) {
    REGS_IN;
    mff_cycles(0x998A, 4);
    a = prg_read((uint16_t)(0x99F0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x998D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x198F, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x9990, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9991 (bank 4) - lifted by port/tools/lift.py */
void func_9991_b4(void) {
    REGS_IN;
    mff_cycles(0x9991, 4);
    a = prg_read((uint16_t)(0x99F0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9994, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1996, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x9997, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9998 (bank 4) - lifted by port/tools/lift.py */
void func_9998_b4(void) {
    REGS_IN;
    mff_cycles(0x9998, 4);
    a = prg_read((uint16_t)(0x99F0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x999B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x199D, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x999E, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $999F (bank 4) - lifted by port/tools/lift.py */
void func_999F_b4(void) {
    REGS_IN;
    mff_cycles(0x999F, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read((uint16_t)(0x99AE + y));
    mff_cycles(0x99A4, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(0x99B1 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x99A9, 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0006); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $99B4 (bank 4) - lifted by port/tools/lift.py */
void func_99B4_b4(void) {
    REGS_IN;
    mff_cycles(0x99B4, 4);
    a = prg_read((uint16_t)(0x99F4 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x99B7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x19B9, 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles(0x99BA, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A000 (bank 4) - lifted by port/tools/lift.py */
void func_A000_b4(void) {
    REGS_IN;
    mff_cycles(0xA000, 11);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    mff_push(a);
    x = a;
    a = prg_read((uint16_t)(0xA035 + x));
    mff_cycles(0xA006, 7);
    RAM(CHR_BANKS + 3) = a;
    a = prg_read((uint16_t)(0xA036 + x));
    mff_cycles(0xA00B, 7);
    RAM(CHR_BANKS + 4) = a;
    a = prg_read((uint16_t)(0xA0B5 + x));
    mff_cycles(0xA010, 27);
    RAM(CHR_BANKS + 5) = a;
    a = mff_pull();
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    y = a;
    a = 0x00;
    m = 0xA1;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(TMP1) = a;
    a = 0x05;
    RAM(TMP0) = a;
    x = 0x00;
    for (;;) {
        mff_cycles(0xA022, 5);
        a = mem_read((uint16_t)(RAM16(TMP0) + y));
        mff_cycles(0xA024, 18);
        RAM(PALETTE_BUF + 24 + x) = a;
        RAM(PALETTE_BASE + 24 + x) = a;
        y++;
        x++;
        m = 0x08;
        c = x >= m;
        if (x == m) break;
        mff_cycles(0xA022, 1);
    }
    mff_cycles(0xA030, 11);
    x = 0xFF;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(PALETTE_REQUEST) = x;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A013 (bank 4) - lifted by port/tools/lift.py */
void func_A013_b4(void) {
    REGS_IN;
    mff_cycles(0xA013, 20);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    y = a;
    a = 0x00;
    m = 0xA1;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(TMP1) = a;
    a = 0x05;
    RAM(TMP0) = a;
    x = 0x00;
    for (;;) {
        mff_cycles(0xA022, 5);
        a = mem_read((uint16_t)(RAM16(TMP0) + y));
        mff_cycles(0xA024, 18);
        RAM(PALETTE_BUF + 24 + x) = a;
        RAM(PALETTE_BASE + 24 + x) = a;
        y++;
        x++;
        m = 0x08;
        c = x >= m;
        if (x == m) break;
        mff_cycles(0xA022, 1);
    }
    mff_cycles(0xA030, 11);
    x = 0xFF;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(PALETTE_REQUEST) = x;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A300 (bank 4) - lifted by port/tools/lift.py */
void func_A300_b4(void) {
    REGS_IN;
    mff_cycles(0xA300, 18);
    a = 0x03;
    RAM(TMP16) = a;
    a = RAM(PALETTE_REQUEST);
    a &= 0x01;
    RAM(TMP17) = a;
    a = 0x00;
    RAM(PALETTE_REQUEST) = a;
    for (;;) {
        mff_cycles(0xA30E, 9);
        x = RAM(TMP16);
        a = RAM(BG_PAL_ANIM_ON + x);
        n = (a) >> 7;
        z = (a) == 0;
        if (n) {
            mff_cycles(0xA315, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0317, 0xA321, 4)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xA313, 1);
        }
        mff_cycles(0xA318, 7);
        t = (uint8_t)(RAM(TMP16) - 1);
        RAM(TMP16) = t;
        if (t & 0x80) break;
        mff_cycles(0xA30E, 1);
    }
    mff_cycles(0xA31C, 12);
    a = RAM(TMP17);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PALETTE_REQUEST) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A321 (bank 4) - lifted by port/tools/lift.py */
void func_A321_b4(void) {
    REGS_IN;
    mff_cycles(0xA321, 8);
    y = RAM(BG_PAL_ANIM_SCRIPT + x);
    a = prg_read((uint16_t)(0xA445 + y));
    mff_cycles(0xA327, 17);
    y = a;
    a = RAM(BG_PAL_ANIM_TIMER + x);
    t = (uint8_t)(RAM(BG_PAL_ANIM_TIMER + x) + 1);
    RAM(BG_PAL_ANIM_TIMER + x) = t;
    m = prg_read((uint16_t)(0xA459 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xA331, 2);
    if (z) {
        mff_cycles(0xA333, 22);
        a = 0x00;
        RAM(BG_PAL_ANIM_TIMER + x) = a;
        a = RAM(BG_PAL_ANIM_STEP + x);
        t = (uint8_t)(RAM(BG_PAL_ANIM_STEP + x) + 1);
        RAM(BG_PAL_ANIM_STEP + x) = t;
        mff_cycles(0xA341, 2);
        if (a == prg_read((uint16_t)(0xA458 + y))) {
            mff_cycles(0xA343, 7);
            a = 0x00;
            RAM(BG_PAL_ANIM_STEP + x) = a;
        } else {
            mff_cycles(0xA341, 1);
        }
        mff_cycles(0xA348, 14);
        a = y;
        c = 0;
        m = RAM(BG_PAL_ANIM_STEP + x);
        w = a + m + c;
        a = (uint8_t)w;
        x = a;
        a = prg_read((uint16_t)(0xA45B + x));
        mff_cycles(0xA351, 6);
        t = a;
        c = t >> 7;
        t = (uint8_t)(t << 1);
        a = t;
        m = prg_read((uint16_t)(0xA45B + x));
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        mff_cycles(0xA355, 6);
        x = a;
        a = prg_read((uint16_t)(0xA45A + y));
        mff_cycles(0xA359, 7);
        y = a;
        a = 0x03;
        RAM(TMP0) = a;
        for (;;) {
            mff_cycles(0xA35E, 4);
            a = prg_read((uint16_t)(0xA370 + x));
            mff_cycles(0xA361, 21);
            RAM(PALETTE_BUF + y) = a;
            RAM(PALETTE_BASE + y) = a;
            y++;
            x++;
            t = (uint8_t)(RAM(TMP0) - 1);
            RAM(TMP0) = t;
            if (t == 0) break;
            mff_cycles(0xA35E, 1);
        }
        mff_cycles(0xA36D, 5);
        t = (uint8_t)(RAM(TMP17) + 1);
        RAM(TMP17) = t;
        n = (t) >> 7;
        z = (t) == 0;
    } else {
        mff_cycles(0xA331, 1);
    }
    mff_cycles(0xA36F, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A700 (bank 4) - lifted by port/tools/lift.py */
void func_A700_b4(void) {
    REGS_IN;
    mff_cycles(0xA700, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0702, 0xA78E, 4)) return;
    SYNC_IN;
    mff_cycles(0xA703, 7);
    y = RAM(INTRO_PAGE);
    a = prg_read((uint16_t)(0xA7D6 + y));
    mff_cycles(0xA708, 7);
    RAM(TMP12) = a;
    a = prg_read((uint16_t)(0xA7DB + y));
    mff_cycles(0xA70D, 16);
    RAM(TMP13) = a;
    a = 0x00;
    RAM(TMP14) = a;
    a = 0x00;
    RAM(TMP16) = a;
    RAM(TMP18) = a;
L_A719:;
    mff_cycles(0xA719, 12);
    a = 0x00;
    RAM(TMP17) = a;
    x = RAM(TMP16);
    a = prg_read((uint16_t)(0xA782 + x));
    mff_cycles(0xA722, 8);
    RAM(VRAM_BUFFER) = a;
    a = prg_read((uint16_t)(0xA785 + x));
    mff_cycles(0xA728, 28);
    c = 0;
    m = RAM(TMP17);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(VRAM_BUFFER + 1) = a;
    a = 0x00;
    RAM(VRAM_BUFFER + 2) = a;
    y = RAM(TMP14);
    t = (uint8_t)(RAM(TMP14) + 1);
    RAM(TMP14) = t;
    a = mem_read((uint16_t)(RAM16(TMP12) + y));
    mff_cycles(0xA739, 4);
    m = 0xFF;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA73B, 1); goto L_A77E; }
    mff_cycles(0xA73D, 4);
    m = 0xFE;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xA741, 31);
        RAM(VRAM_BUFFER + 3) = a;
        a = 0xFF;
        RAM(VRAM_BUFFER + 4) = a;
        RAM(VRAM_LIST_REQUEST) = a;
        a = RAM(TMP16);
        mff_push(a);
        a = RAM(TMP17);
        n = (a) >> 7;
        z = (a) == 0;
        mff_push(a);
        SYNC_OUT;
        if (!mff_call_at(0x0753, 0xA788, 4)) return;
        SYNC_IN;
        mff_cycles(0xA754, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0756, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA757, 26);
        a = mff_pull();
        RAM(TMP17) = a;
        a = mff_pull();
        RAM(TMP16) = a;
        t = (uint8_t)(RAM(TMP17) + 1);
        RAM(TMP17) = t;
        a = RAM(FRAME_COUNTER);
        a &= 0x03;
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0xA765, 8);
            a = 0x2B;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0769, func_F34C, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xA763, 1);
        }
        mff_cycles(0xA76A, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA71D, 2); call_by_address_tail(0xA71D, 4); return;
    } else {
        mff_cycles(0xA73F, 1);
    }
    mff_cycles(0xA76D, 12);
    t = (uint8_t)(RAM(TMP16) + 1);
    RAM(TMP16) = t;
    a = RAM(TMP16);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA719, 1); goto L_A719; }
    mff_cycles(0xA775, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0777, 0xA7D0, 4)) return;
    SYNC_IN;
    mff_cycles(0xA778, 6);
    SYNC_OUT;
    if (!mff_call_at(0x077A, 0xA78E, 4)) return;
    SYNC_IN;
    mff_cycles(0xA77B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA713, 2); call_by_address_tail(0xA713, 4); return;
L_A77E:;
    mff_cycles(0xA77E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA7D0, 2); call_by_address_tail(0xA7D0, 4); return;
}

/*  $A713 (bank 4) - lifted by port/tools/lift.py */
void func_A713_b4(void) {
    REGS_IN;
    mff_cycles(0xA713, 8);
    a = 0x00;
    RAM(TMP16) = a;
    RAM(TMP18) = a;
L_A719:;
    mff_cycles(0xA719, 12);
    a = 0x00;
    RAM(TMP17) = a;
    x = RAM(TMP16);
    a = prg_read((uint16_t)(0xA782 + x));
    mff_cycles(0xA722, 8);
    RAM(VRAM_BUFFER) = a;
    a = prg_read((uint16_t)(0xA785 + x));
    mff_cycles(0xA728, 28);
    c = 0;
    m = RAM(TMP17);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(VRAM_BUFFER + 1) = a;
    a = 0x00;
    RAM(VRAM_BUFFER + 2) = a;
    y = RAM(TMP14);
    t = (uint8_t)(RAM(TMP14) + 1);
    RAM(TMP14) = t;
    a = mem_read((uint16_t)(RAM16(TMP12) + y));
    mff_cycles(0xA739, 4);
    m = 0xFF;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA73B, 1); goto L_A77E; }
    mff_cycles(0xA73D, 4);
    m = 0xFE;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xA741, 31);
        RAM(VRAM_BUFFER + 3) = a;
        a = 0xFF;
        RAM(VRAM_BUFFER + 4) = a;
        RAM(VRAM_LIST_REQUEST) = a;
        a = RAM(TMP16);
        mff_push(a);
        a = RAM(TMP17);
        n = (a) >> 7;
        z = (a) == 0;
        mff_push(a);
        SYNC_OUT;
        if (!mff_call_at(0x0753, 0xA788, 4)) return;
        SYNC_IN;
        mff_cycles(0xA754, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0756, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA757, 26);
        a = mff_pull();
        RAM(TMP17) = a;
        a = mff_pull();
        RAM(TMP16) = a;
        t = (uint8_t)(RAM(TMP17) + 1);
        RAM(TMP17) = t;
        a = RAM(FRAME_COUNTER);
        a &= 0x03;
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0xA765, 8);
            a = 0x2B;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0769, func_F34C, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xA763, 1);
        }
        mff_cycles(0xA76A, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA71D, 2); call_by_address_tail(0xA71D, 4); return;
    } else {
        mff_cycles(0xA73F, 1);
    }
    mff_cycles(0xA76D, 12);
    t = (uint8_t)(RAM(TMP16) + 1);
    RAM(TMP16) = t;
    a = RAM(TMP16);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA719, 1); goto L_A719; }
    mff_cycles(0xA775, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0777, 0xA7D0, 4)) return;
    SYNC_IN;
    mff_cycles(0xA778, 6);
    SYNC_OUT;
    if (!mff_call_at(0x077A, 0xA78E, 4)) return;
    SYNC_IN;
    mff_cycles(0xA77B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA713, 2); call_by_address_tail(0xA713, 4); return;
L_A77E:;
    mff_cycles(0xA77E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA7D0, 2); call_by_address_tail(0xA7D0, 4); return;
}

/*  $A71D (bank 4) - lifted by port/tools/lift.py */
void func_A71D_b4(void) {
    REGS_IN;
    goto L_A71D;
L_A719:;
    mff_cycles(0xA719, 5);
    a = 0x00;
    RAM(TMP17) = a;
L_A71D:;
    mff_cycles(0xA71D, 7);
    x = RAM(TMP16);
    a = prg_read((uint16_t)(0xA782 + x));
    mff_cycles(0xA722, 8);
    RAM(VRAM_BUFFER) = a;
    a = prg_read((uint16_t)(0xA785 + x));
    mff_cycles(0xA728, 28);
    c = 0;
    m = RAM(TMP17);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(VRAM_BUFFER + 1) = a;
    a = 0x00;
    RAM(VRAM_BUFFER + 2) = a;
    y = RAM(TMP14);
    t = (uint8_t)(RAM(TMP14) + 1);
    RAM(TMP14) = t;
    a = mem_read((uint16_t)(RAM16(TMP12) + y));
    mff_cycles(0xA739, 4);
    m = 0xFF;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA73B, 1); goto L_A77E; }
    mff_cycles(0xA73D, 4);
    m = 0xFE;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xA741, 31);
        RAM(VRAM_BUFFER + 3) = a;
        a = 0xFF;
        RAM(VRAM_BUFFER + 4) = a;
        RAM(VRAM_LIST_REQUEST) = a;
        a = RAM(TMP16);
        mff_push(a);
        a = RAM(TMP17);
        n = (a) >> 7;
        z = (a) == 0;
        mff_push(a);
        SYNC_OUT;
        if (!mff_call_at(0x0753, 0xA788, 4)) return;
        SYNC_IN;
        mff_cycles(0xA754, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0756, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA757, 26);
        a = mff_pull();
        RAM(TMP17) = a;
        a = mff_pull();
        RAM(TMP16) = a;
        t = (uint8_t)(RAM(TMP17) + 1);
        RAM(TMP17) = t;
        a = RAM(FRAME_COUNTER);
        a &= 0x03;
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0xA765, 8);
            a = 0x2B;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0769, func_F34C, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xA763, 1);
        }
        mff_cycles(0xA76A, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA71D, 2); call_by_address_tail(0xA71D, 4); return;
    } else {
        mff_cycles(0xA73F, 1);
    }
    mff_cycles(0xA76D, 12);
    t = (uint8_t)(RAM(TMP16) + 1);
    RAM(TMP16) = t;
    a = RAM(TMP16);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA719, 1); goto L_A719; }
    mff_cycles(0xA775, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0777, 0xA7D0, 4)) return;
    SYNC_IN;
    mff_cycles(0xA778, 6);
    SYNC_OUT;
    if (!mff_call_at(0x077A, 0xA78E, 4)) return;
    SYNC_IN;
    mff_cycles(0xA77B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA713, 2); call_by_address_tail(0xA713, 4); return;
L_A77E:;
    mff_cycles(0xA77E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA7D0, 2); call_by_address_tail(0xA7D0, 4); return;
}

/*  $A788 (bank 4) - lifted by port/tools/lift.py */
void func_A788_b4(void) {
    REGS_IN;
    mff_cycles(0xA788, 8);
    x = 0x00;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x078C, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA78D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A78E (bank 4) - lifted by port/tools/lift.py */
void func_A78E_b4(void) {
    REGS_IN;
    mff_cycles(0xA78E, 52);
    a = 0x22;
    RAM(VRAM_BUFFER + 4) = a;
    a = 0x21;
    RAM(VRAM_BUFFER + 48) = a;
    RAM(VRAM_BUFFER + 26) = a;
    a = 0x27;
    RAM(VRAM_BUFFER + 5) = a;
    a = 0xE7;
    RAM(VRAM_BUFFER + 27) = a;
    a = 0xA7;
    RAM(VRAM_BUFFER + 49) = a;
    x = 0x12;
    RAM(VRAM_BUFFER + 6) = x;
    RAM(VRAM_BUFFER + 28) = x;
    RAM(VRAM_BUFFER + 50) = x;
    y = 0x00;
    a = 0x00;
    for (;;) {
        mff_cycles(0xA7B9, 21);
        RAM(VRAM_BUFFER + 7 + y) = a;
        RAM(VRAM_BUFFER + 29 + y) = a;
        RAM(VRAM_BUFFER + 51 + y) = a;
        y++;
        x--;
        if (x & 0x80) break;
        mff_cycles(0xA7B9, 1);
    }
    mff_cycles(0xA7C6, 12);
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(VRAM_BUFFER + 70) = a;
    RAM(VRAM_LIST_REQUEST) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xFF31, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_FF31(); g_code_window_base = swb; } return;
}

/*  $A7D0 (bank 4) - lifted by port/tools/lift.py */
void func_A7D0_b4(void) {
    REGS_IN;
    mff_cycles(0xA7D0, 8);
    x = 0xB4;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    mff_call_fn(0x07D4, func_FF2A, 0xE000);
    SYNC_IN;
    mff_cycles(0xA7D5, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AB00 (bank 4) - lifted by port/tools/lift.py */
void func_AB00_b4(void) {
    REGS_IN;
    mff_cycles(0xAB00, 45);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0xD0;
    RAM(OBJ_FLAGS) = a;
    a = 0x00;
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y_HI) = a;
    RAM(OBJ_ID) = a;
    a = 0x58;
    RAM(OBJ_X) = a;
    a = 0xA8;
    RAM(OBJ_Y) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xAB2E + y));
    mff_cycles(0xAB24, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0xAB31 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xAB29, 8);
    RAM(TMP1) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $AB34 (bank 4) - lifted by port/tools/lift.py */
void func_AB34_b4(void) {
    REGS_IN;
    mff_cycles(0xAB34, 8);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    if (a == 0) {
        mff_cycles(0xAB3B, 8);
        a = 0x73;
        m = RAM(OBJ_ANIM);
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xAB42, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0B44, func_F294, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAB45, 8);
            a = 0x3F;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0B49, func_F440, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xAB40, 1);
        }
        mff_cycles(0xAB4A, 6);
        a = RAM(OBJ_ANIM_FRAME);
        if (a == 0) {
            mff_cycles(0xAB4F, 6);
            a = RAM(OBJ_ANIM_TIMER);
            if (a == 0) {
                mff_cycles(0xAB54, 8);
                a = 0x16;
                n = (a) >> 7;
                z = (a) == 0;
                SYNC_OUT;
                if (!mff_call_fn(0x0B58, func_F34C, 0xE000)) return;
                SYNC_IN;
            } else {
                mff_cycles(0xAB52, 1);
            }
        } else {
            mff_cycles(0xAB4D, 1);
        }
        mff_cycles(0xAB59, 8);
        a = RAM(OBJ_ANIM_FRAME);
        m = 0x06;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) {
            mff_cycles(0xAB60, 8);
            a = 0x13;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0B64, func_F294, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAB65, 8);
            a = 0x03;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0B69, func_F440, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAB6A, 18);
            a = 0x38;
            RAM(OBJ_VY_FRAC) = a;
            a = 0x04;
            RAM(OBJ_VY) = a;
            t = (uint8_t)(RAM(OBJ_STATE) + 1);
            RAM(OBJ_STATE) = t;
            n = (t) >> 7;
            z = (t) == 0;
        } else {
            mff_cycles(0xAB5E, 1);
        }
        mff_cycles(0xAB77, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0B79, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAB7A, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xAB34, 2); call_by_address_tail(0xAB34, 4); return;
    } else {
        mff_cycles(0xAB39, 1);
    }
    mff_cycles(0xAB7D, 8);
    a = RAM(OBJ_STATE);
    a &= 0x02;
    if (a != 0) { mff_cycles(0xAB82, 1); goto L_AB95; }
    mff_cycles(0xAB84, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x05;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xAB89, 1); goto L_AB9A; }
    mff_cycles(0xAB8B, 12);
    a = 0x05;
    RAM(OBJ_ANIM_FRAME) = a;
    a = 0x00;
    RAM(OBJ_ANIM_TIMER) = a;
L_AB95:;
    mff_cycles(0xAB95, 8);
    x = 0x00;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0B99, func_F1FD, 0xE000)) return;
    SYNC_IN;
L_AB9A:;
    mff_cycles(0xAB9A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B9C, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAB9D, 6);
    a = RAM(OBJ_VY);
    if (a & 0x80) {
        mff_cycles(0xABA2, 8);
        a = 0x3E;
        m = RAM(OBJ_ANIM);
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xABA9, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0BAB, func_F294, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xABAC, 8);
            a = 0x01;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0BB0, func_F440, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xABB1, 6);
            t = (uint8_t)(RAM(OBJ_STATE) + 1);
            RAM(OBJ_STATE) = t;
        } else {
            mff_cycles(0xABA7, 1);
        }
    } else {
        mff_cycles(0xABA0, 1);
    }
    mff_cycles(0xABB4, 8);
    a = RAM(OBJ_Y);
    m = 0xA9;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0xABBB, 14);
        a = 0xA8;
        RAM(OBJ_Y) = a;
        a = 0x01;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0BC4, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xABC5, 8);
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0BC9, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xABCA, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF406, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F406(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xABB9, 1);
    }
    mff_cycles(0xABCD, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAB77, 2); call_by_address_tail(0xAB77, 4); return;
}

/*  $AB77 (bank 4) - lifted by port/tools/lift.py */
void func_AB77_b4(void) {
    REGS_IN;
    mff_cycles(0xAB77, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B79, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAB7A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAB34, 2); call_by_address_tail(0xAB34, 4); return;
}

/*  $ABD0 (bank 4) - lifted by port/tools/lift.py */
void func_ABD0_b4(void) {
    REGS_IN;
    mff_cycles(0xABD0, 8);
    a = 0x24;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xABD7, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0BD9, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xABDA, 8);
        a = 0x32;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0BDE, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xABDF, 6);
        a = RAM(OBJ_ANIM_FRAME);
        if (a == 0) {
            mff_cycles(0xABE4, 6);
            a = RAM(OBJ_ANIM_TIMER);
            if (a == 0) {
                mff_cycles(0xABE9, 8);
                a = 0x1B;
                n = (a) >> 7;
                z = (a) == 0;
                SYNC_OUT;
                if (!mff_call_fn(0x0BED, func_F34C, 0xE000)) return;
                SYNC_IN;
            } else {
                mff_cycles(0xABE7, 1);
            }
        } else {
            mff_cycles(0xABE2, 1);
        }
    } else {
        mff_cycles(0xABD5, 1);
    }
    mff_cycles(0xABEE, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x0B;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xABF5, 8);
        a = 0x34;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0BF9, func_F34C, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xABF3, 1);
    }
    mff_cycles(0xABFA, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x13;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xAC01, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0C03, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAC04, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xABDF, 2); call_by_address_tail(0xABDF, 4); return;
    } else {
        mff_cycles(0xABFF, 1);
    }
    mff_cycles(0xAC07, 8);
    a = 0x14;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xAC0E, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0C10, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAC11, 8);
        a = 0x07;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0C15, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAC16, 12);
        a = 0x33;
        RAM(OBJ_VY_FRAC) = a;
        a = 0x01;
        RAM(OBJ_VY) = a;
    } else {
        mff_cycles(0xAC0C, 1);
    }
    mff_cycles(0xAC20, 8);
    a = RAM(OBJ_ANIM);
    m = 0x5B;
    c = a >= m;
    if (a == m) { mff_cycles(0xAC25, 1); goto L_AC55; }
    mff_cycles(0xAC27, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x04;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xAC2E, 6);
        a = RAM(OBJ_ANIM_TIMER);
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0xAC33, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0C35, func_F430, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xAC31, 1);
        }
    } else {
        mff_cycles(0xAC2C, 1);
    }
    mff_cycles(0xAC36, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x06;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xAC3B, 1); goto L_AC7B; }
    mff_cycles(0xAC3D, 8);
    a = 0x5B;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0C41, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC42, 8);
    a = 0x05;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0C46, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC47, 19);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(OBJ_Y);
    c = 1;
    m = 0x20;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_Y) = a;
L_AC55:;
    mff_cycles(0xAC55, 8);
    x = 0x00;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0C59, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC5A, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0xAC5F, 8);
        a = RAM(OBJ_Y);
        m = 0xA8;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0xAC66, 14);
            a = 0xA8;
            RAM(OBJ_Y) = a;
            a = 0x02;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0C6F, func_F294, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAC70, 8);
            a = 0x04;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0C74, func_F440, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAC75, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0C77, func_F406, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAC78, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xAC75, 2); call_by_address_tail(0xAC75, 4); return;
        } else {
            mff_cycles(0xAC64, 1);
        }
    } else {
        mff_cycles(0xAC5D, 1);
    }
L_AC7B:;
    mff_cycles(0xAC7B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C7D, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC7E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAC20, 2); call_by_address_tail(0xAC20, 4); return;
}

/*  $ABDF (bank 4) - lifted by port/tools/lift.py */
void func_ABDF_b4(void) {
    REGS_IN;
    mff_cycles(0xABDF, 6);
    a = RAM(OBJ_ANIM_FRAME);
    if (a == 0) {
        mff_cycles(0xABE4, 6);
        a = RAM(OBJ_ANIM_TIMER);
        if (a == 0) {
            mff_cycles(0xABE9, 8);
            a = 0x1B;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0BED, func_F34C, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xABE7, 1);
        }
    } else {
        mff_cycles(0xABE2, 1);
    }
    mff_cycles(0xABEE, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x0B;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xABF5, 8);
        a = 0x34;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0BF9, func_F34C, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xABF3, 1);
    }
    mff_cycles(0xABFA, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x13;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xAC01, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0C03, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAC04, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xABDF, 2); call_by_address_tail(0xABDF, 4); return;
    } else {
        mff_cycles(0xABFF, 1);
    }
    mff_cycles(0xAC07, 8);
    a = 0x14;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xAC0E, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0C10, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAC11, 8);
        a = 0x07;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0C15, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAC16, 12);
        a = 0x33;
        RAM(OBJ_VY_FRAC) = a;
        a = 0x01;
        RAM(OBJ_VY) = a;
    } else {
        mff_cycles(0xAC0C, 1);
    }
    mff_cycles(0xAC20, 8);
    a = RAM(OBJ_ANIM);
    m = 0x5B;
    c = a >= m;
    if (a == m) { mff_cycles(0xAC25, 1); goto L_AC55; }
    mff_cycles(0xAC27, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x04;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xAC2E, 6);
        a = RAM(OBJ_ANIM_TIMER);
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0xAC33, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0C35, func_F430, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xAC31, 1);
        }
    } else {
        mff_cycles(0xAC2C, 1);
    }
    mff_cycles(0xAC36, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x06;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xAC3B, 1); goto L_AC7B; }
    mff_cycles(0xAC3D, 8);
    a = 0x5B;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0C41, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC42, 8);
    a = 0x05;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0C46, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC47, 19);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(OBJ_Y);
    c = 1;
    m = 0x20;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_Y) = a;
L_AC55:;
    mff_cycles(0xAC55, 8);
    x = 0x00;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0C59, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC5A, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0xAC5F, 8);
        a = RAM(OBJ_Y);
        m = 0xA8;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0xAC66, 14);
            a = 0xA8;
            RAM(OBJ_Y) = a;
            a = 0x02;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0C6F, func_F294, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAC70, 8);
            a = 0x04;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0C74, func_F440, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAC75, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0C77, func_F406, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAC78, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xAC75, 2); call_by_address_tail(0xAC75, 4); return;
        } else {
            mff_cycles(0xAC64, 1);
        }
    } else {
        mff_cycles(0xAC5D, 1);
    }
L_AC7B:;
    mff_cycles(0xAC7B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C7D, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC7E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAC20, 2); call_by_address_tail(0xAC20, 4); return;
}

/*  $AC20 (bank 4) - lifted by port/tools/lift.py */
void func_AC20_b4(void) {
    REGS_IN;
    mff_cycles(0xAC20, 8);
    a = RAM(OBJ_ANIM);
    m = 0x5B;
    c = a >= m;
    if (a == m) { mff_cycles(0xAC25, 1); goto L_AC55; }
    mff_cycles(0xAC27, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x04;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xAC2E, 6);
        a = RAM(OBJ_ANIM_TIMER);
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0xAC33, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0C35, func_F430, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xAC31, 1);
        }
    } else {
        mff_cycles(0xAC2C, 1);
    }
    mff_cycles(0xAC36, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x06;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xAC3B, 1); goto L_AC7B; }
    mff_cycles(0xAC3D, 8);
    a = 0x5B;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0C41, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC42, 8);
    a = 0x05;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0C46, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC47, 19);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(OBJ_Y);
    c = 1;
    m = 0x20;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_Y) = a;
L_AC55:;
    mff_cycles(0xAC55, 8);
    x = 0x00;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0C59, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC5A, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0xAC5F, 8);
        a = RAM(OBJ_Y);
        m = 0xA8;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0xAC66, 14);
            a = 0xA8;
            RAM(OBJ_Y) = a;
            a = 0x02;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0C6F, func_F294, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAC70, 8);
            a = 0x04;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0C74, func_F440, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAC75, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0C77, func_F406, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAC78, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xAC75, 2); call_by_address_tail(0xAC75, 4); return;
        } else {
            mff_cycles(0xAC64, 1);
        }
    } else {
        mff_cycles(0xAC5D, 1);
    }
L_AC7B:;
    mff_cycles(0xAC7B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C7D, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC7E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAC20, 2); call_by_address_tail(0xAC20, 4); return;
}

/*  $AC75 (bank 4) - lifted by port/tools/lift.py */
void func_AC75_b4(void) {
    REGS_IN;
    mff_cycles(0xAC75, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C77, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC78, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAC75, 2); call_by_address_tail(0xAC75, 4); return;
}

/*  $AC81 (bank 4) - lifted by port/tools/lift.py */
void func_AC81_b4(void) {
    REGS_IN;
    mff_cycles(0xAC81, 8);
    a = 0x30;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xAC88, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0C8A, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAC8B, 8);
        a = 0x0B;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0C8F, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAC90, 30);
        a = 0x08;
        RAM(OBJ_TIMER) = a;
        a = 0x1E;
        RAM(OBJ_TIMER + 1) = a;
        a = 0x00;
        RAM(OBJ_VX_FRAC) = a;
        a = 0x03;
        RAM(OBJ_VX) = a;
        a = 0x90;
        RAM(OBJ_FLAGS) = a;
    } else {
        mff_cycles(0xAC86, 1);
    }
    mff_cycles(0xACA9, 6);
    a = RAM(OBJ_TIMER);
    if (a != 0) {
        mff_cycles(0xACAE, 12);
        t = (uint8_t)(RAM(OBJ_TIMER) - 1);
        RAM(OBJ_TIMER) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CB3, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xACB4, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xAC81, 2); call_by_address_tail(0xAC81, 4); return;
    } else {
        mff_cycles(0xACAC, 1);
    }
    mff_cycles(0xACB7, 8);
    a = 0x3D;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xACBE, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0CC0, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xACC1, 8);
        a = 0x0A;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CC5, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xACC6, 8);
        a = 0x2C;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CCA, func_F34C, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xACBC, 1);
    }
    mff_cycles(0xACCB, 8);
    x = 0x00;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0CCF, func_F03F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xACD0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0CD2, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xACD3, 6);
    a = RAM(OBJ_TIMER + 1);
    if (a != 0) {
        mff_cycles(0xACD8, 12);
        t = (uint8_t)(RAM(OBJ_TIMER + 1) - 1);
        RAM(OBJ_TIMER + 1) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CDD, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xACDE, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xACB7, 2); call_by_address_tail(0xACB7, 4); return;
    } else {
        mff_cycles(0xACD6, 1);
    }
    mff_cycles(0xACE1, 8);
    a = 0x47;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xACE8, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0CEA, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xACEB, 8);
        a = 0x11;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CEF, func_F440, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xACE6, 1);
    }
    mff_cycles(0xACF0, 6);
    a = RAM(OBJ_ANIM_FRAME);
    if (a == 0) {
        mff_cycles(0xACF5, 8);
        a = 0x1D;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CF9, func_F34C, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xACF3, 1);
    }
    mff_cycles(0xACFA, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x0C;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xAD01, 8);
        a = 0x03;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0D05, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAD06, 8);
        a = 0x08;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0D0A, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAD0B, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0D0D, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAD0E, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xAD0B, 2); call_by_address_tail(0xAD0B, 4); return;
    } else {
        mff_cycles(0xACFF, 1);
    }
    mff_cycles(0xAD11, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D13, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAD14, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xACE1, 2); call_by_address_tail(0xACE1, 4); return;
}

/*  $ACB7 (bank 4) - lifted by port/tools/lift.py */
void func_ACB7_b4(void) {
    REGS_IN;
    mff_cycles(0xACB7, 8);
    a = 0x3D;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xACBE, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0CC0, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xACC1, 8);
        a = 0x0A;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CC5, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xACC6, 8);
        a = 0x2C;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CCA, func_F34C, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xACBC, 1);
    }
    mff_cycles(0xACCB, 8);
    x = 0x00;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0CCF, func_F03F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xACD0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0CD2, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xACD3, 6);
    a = RAM(OBJ_TIMER + 1);
    if (a != 0) {
        mff_cycles(0xACD8, 12);
        t = (uint8_t)(RAM(OBJ_TIMER + 1) - 1);
        RAM(OBJ_TIMER + 1) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CDD, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xACDE, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xACB7, 2); call_by_address_tail(0xACB7, 4); return;
    } else {
        mff_cycles(0xACD6, 1);
    }
    mff_cycles(0xACE1, 8);
    a = 0x47;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xACE8, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0CEA, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xACEB, 8);
        a = 0x11;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CEF, func_F440, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xACE6, 1);
    }
    mff_cycles(0xACF0, 6);
    a = RAM(OBJ_ANIM_FRAME);
    if (a == 0) {
        mff_cycles(0xACF5, 8);
        a = 0x1D;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CF9, func_F34C, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xACF3, 1);
    }
    mff_cycles(0xACFA, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x0C;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xAD01, 8);
        a = 0x03;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0D05, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAD06, 8);
        a = 0x08;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0D0A, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAD0B, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0D0D, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAD0E, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xAD0B, 2); call_by_address_tail(0xAD0B, 4); return;
    } else {
        mff_cycles(0xACFF, 1);
    }
    mff_cycles(0xAD11, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D13, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAD14, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xACE1, 2); call_by_address_tail(0xACE1, 4); return;
}

/*  $ACE1 (bank 4) - lifted by port/tools/lift.py */
void func_ACE1_b4(void) {
    REGS_IN;
    mff_cycles(0xACE1, 8);
    a = 0x47;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xACE8, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0CEA, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xACEB, 8);
        a = 0x11;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CEF, func_F440, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xACE6, 1);
    }
    mff_cycles(0xACF0, 6);
    a = RAM(OBJ_ANIM_FRAME);
    if (a == 0) {
        mff_cycles(0xACF5, 8);
        a = 0x1D;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CF9, func_F34C, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xACF3, 1);
    }
    mff_cycles(0xACFA, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x0C;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xAD01, 8);
        a = 0x03;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0D05, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAD06, 8);
        a = 0x08;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0D0A, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAD0B, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0D0D, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAD0E, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xAD0B, 2); call_by_address_tail(0xAD0B, 4); return;
    } else {
        mff_cycles(0xACFF, 1);
    }
    mff_cycles(0xAD11, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D13, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAD14, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xACE1, 2); call_by_address_tail(0xACE1, 4); return;
}

/*  $AD0B (bank 4) - lifted by port/tools/lift.py */
void func_AD0B_b4(void) {
    REGS_IN;
    mff_cycles(0xAD0B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D0D, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAD0E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAD0B, 2); call_by_address_tail(0xAD0B, 4); return;
}
