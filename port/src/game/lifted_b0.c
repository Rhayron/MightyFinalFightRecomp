/*
 * lifted_b0.c - bank 0 routines $8000-$BFFF, lifted by
 * port/tools/lift_bank.py from the pure scaffold: exact, still to be
 * tidied and moved into their systems' files.
 */
#include "mff.h"
#include "state.h"
#include "coroutine.h"

void func_F03F(void);
void func_F048(void);
void func_F090(void);
void func_F099(void);
void func_F119(void);
void func_F183(void);
void func_F1A1(void);
void func_F1BF(void);
void func_F1DF(void);
void func_F1FD(void);
void func_F26E(void);
void func_F288(void);
void func_F294(void);
void func_F2A0(void);
void func_F2BE(void);
void func_F32E(void);
void func_F343(void);
void func_F34A(void);
void func_F34C(void);
void func_F374(void);
void func_F406(void);
void func_F436(void);
void func_F43B(void);
void func_F440(void);
void func_F443(void);
void func_F449(void);
void func_F44F(void);
void func_F455(void);
void func_FB3E(void);
void func_FB5B(void);
void func_FB72(void);
void func_FBD9(void);
void func_FF2A(void);
void func_FF31(void);

/*  $9297 (bank 0) - lifted by port/tools/lift.py */
void func_9297_b0(void) {
    REGS_IN;
    mff_cycles(0x9297, 38);
    a = 0x00;
    RAM(OBJ_X + 13) = a;
    a = 0x90;
    RAM(OBJ_Y + 13) = a;
    a = RAM(OBJ_X_HI);
    RAM(OBJ_X_HI + 13) = a;
    a = RAM(OBJ_Y_HI);
    RAM(OBJ_Y_HI + 13) = a;
    y = 0x0D;
    a = 0xB7;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x12B3, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x92B4, 12);
    a = 0x6B;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ID + 13) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $92E1 (bank 0) - lifted by port/tools/lift.py */
void func_92E1_b0(void) {
    REGS_IN;
    mff_cycles(0x92E1, 8);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    if (a != 0) { mff_cycles(0x92E6, 2); goto L_9352; }
    mff_cycles(0x92E8, 5);
    a = RAM(BOSS_AREA);
    if (a == 0) { mff_cycles(0x92EA, 2); goto L_9307; }
    mff_cycles(0x92EC, 7);
    a = RAM(ROUND);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0x92F0, 1); goto L_92F6; }
    mff_cycles(0x92F2, 4);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x92F4, 1); goto L_92F8; }
L_92F6:;
    mff_cycles(0x92F6, 2);
    a = 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
L_92F8:;
    mff_cycles(0x92F8, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x12FA, func_F34A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x92FB, 14);
    a = 0xFF;
    RAM(SPECIAL_SCREEN_ID) = a;
    a = 0x00;
    RAM(BOSS_AREA) = a;
    a = 0xD8;
    if (a != 0) { mff_cycles(0x9305, 1); goto L_9309; }
L_9307:;
    mff_cycles(0x9307, 2);
    a = 0x3C;
L_9309:;
    mff_cycles(0x9309, 8);
    RAM(ACTION_TIMER) = a;
    a = RAM(ROUND);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0x930F, 7);
        a = RAM(SUB_AREA);
        m = 0x01;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) {
            mff_cycles(0x9315, 5);
            a = 0x0F;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(ACTION_TIMER) = a;
        } else {
            mff_cycles(0x9313, 1);
        }
    } else {
        mff_cycles(0x930D, 1);
    }
    mff_cycles(0x9319, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x131B, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x931C, 5);
    a = RAM(WEAPON_HELD);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0x9320, 6);
        SYNC_OUT;
        if (!mff_call_at(0x1322, 0x93AF, 0)) return;
        SYNC_IN;
        mff_cycles(0x9323, 5);
        a = 0x00;
        RAM(WEAPON_HELD) = a;
    } else {
        mff_cycles(0x931E, 1);
    }
    mff_cycles(0x9327, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1E3 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x932C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x132E, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x932F, 4);
    a = prg_read((uint16_t)(0xE22B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9332, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1334, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9335, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1C1 + y));
    mff_cycles(0x933A, 8);
    RAM(OBJ_VX_FRAC) = a;
    a = prg_read((uint16_t)(0xE1C4 + y));
    mff_cycles(0x9340, 8);
    RAM(OBJ_VX) = a;
    a = prg_read((uint16_t)(0xE1C7 + y));
    mff_cycles(0x9346, 8);
    RAM(OBJ_VY_FRAC) = a;
    a = prg_read((uint16_t)(0xE1CA + y));
    mff_cycles(0x934C, 10);
    RAM(OBJ_VY) = a;
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
L_9352:;
    mff_cycles(0x9352, 5);
    a = RAM(ACTION_TIMER);
    if (a != 0) {
        mff_cycles(0x9356, 11);
        t = (uint8_t)(RAM(ACTION_TIMER) - 1);
        RAM(ACTION_TIMER) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9354, 1);
    }
    mff_cycles(0x9359, 7);
    y = RAM(ROUND);
    a = prg_read((uint16_t)(0x9378 + y));
    mff_cycles(0x935E, 7);
    RAM(TMP2) = a;
    a = prg_read((uint16_t)(0x937D + y));
    mff_cycles(0x9363, 13);
    RAM(TMP3) = a;
    a = RAM(SUB_AREA);
    y = a;
    a = mem_read((uint16_t)(RAM16(TMP2) + y));
    mff_cycles(0x936A, 6);
    y = a;
    a = prg_read((uint16_t)(0x9391 + y));
    mff_cycles(0x936E, 7);
    RAM(TMP4) = a;
    a = prg_read((uint16_t)(0x93A0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9373, 8);
    RAM(TMP5) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0004); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $93EF (bank 0) - lifted by port/tools/lift.py */
void func_93EF_b0(void) {
    REGS_IN;
    mff_cycles(0x93EF, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x02;
    c = a >= m;
    if (a != m) {
        mff_cycles(0x93F8, 7);
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(0xE1E6 + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x93FD, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x13FF, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9400, 8);
        a = RAM(OBJ_X);
        m = 0xE0;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!c) {
            mff_cycles(0x9407, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x1409, func_F03F, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x940A, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x9410, 2); call_by_address_tail(0x9410, 0); return;
        } else {
            mff_cycles(0x9405, 1);
        }
        mff_cycles(0x940D, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x140F, func_F048, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9410, 6);
        t = (uint8_t)(RAM(OBJ_STATE) + 1);
        RAM(OBJ_STATE) = t;
    } else {
        mff_cycles(0x93F6, 2);
    }
    mff_cycles(0x9413, 8);
    a = RAM(OBJ_Y);
    m = 0x82;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0x941A, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x141C, func_F1BF, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x941D, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9418, 1);
    }
    mff_cycles(0x941E, 8);
    a = RAM(OBJ_Y);
    m = 0x84;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x9425, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1427, func_F1DF, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9428, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9423, 1);
    }
    mff_cycles(0x9429, 8);
    a = RAM(OBJ_X);
    m = 0xE0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x9430, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1432, func_F1A1, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9433, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1435, func_F048, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9436, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x943F, 2); call_by_address_tail(0x943F, 0); return;
    } else {
        mff_cycles(0x942E, 1);
    }
    mff_cycles(0x9439, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x143B, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x943C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x143E, func_F03F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x943F, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    if (a == 0x03) { mff_cycles(0x9446, 1); goto L_9452; }
    mff_cycles(0x9448, 8);
    a = RAM(OBJ_X);
    m = 0xD0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x944D, 1); goto L_945D; }
    mff_cycles(0x944F, 6);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
L_9452:;
    mff_cycles(0x9452, 8);
    a = RAM(OBJ_X);
    m = 0xE0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x9459, 5);
        a = 0x0E;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(GAME_ACTION) = a;
    } else {
        mff_cycles(0x9457, 1);
    }
L_945D:;
    mff_cycles(0x945D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9410 (bank 0) - lifted by port/tools/lift.py */
void func_9410_b0(void) {
    REGS_IN;
    mff_cycles(0x9410, 14);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    a = RAM(OBJ_Y);
    m = 0x82;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0x941A, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x141C, func_F1BF, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x941D, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9418, 1);
    }
    mff_cycles(0x941E, 8);
    a = RAM(OBJ_Y);
    m = 0x84;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x9425, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1427, func_F1DF, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9428, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9423, 1);
    }
    mff_cycles(0x9429, 8);
    a = RAM(OBJ_X);
    m = 0xE0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x9430, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1432, func_F1A1, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9433, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1435, func_F048, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9436, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x943F, 2); call_by_address_tail(0x943F, 0); return;
    } else {
        mff_cycles(0x942E, 1);
    }
    mff_cycles(0x9439, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x143B, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x943C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x143E, func_F03F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x943F, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    if (a == 0x03) { mff_cycles(0x9446, 1); goto L_9452; }
    mff_cycles(0x9448, 8);
    a = RAM(OBJ_X);
    m = 0xD0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x944D, 1); goto L_945D; }
    mff_cycles(0x944F, 6);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
L_9452:;
    mff_cycles(0x9452, 8);
    a = RAM(OBJ_X);
    m = 0xE0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x9459, 5);
        a = 0x0E;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(GAME_ACTION) = a;
    } else {
        mff_cycles(0x9457, 1);
    }
L_945D:;
    mff_cycles(0x945D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $943F (bank 0) - lifted by port/tools/lift.py */
void func_943F_b0(void) {
    REGS_IN;
    mff_cycles(0x943F, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    if (a == 0x03) { mff_cycles(0x9446, 1); goto L_9452; }
    mff_cycles(0x9448, 8);
    a = RAM(OBJ_X);
    m = 0xD0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x944D, 1); goto L_945D; }
    mff_cycles(0x944F, 6);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
L_9452:;
    mff_cycles(0x9452, 8);
    a = RAM(OBJ_X);
    m = 0xE0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x9459, 5);
        a = 0x0E;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(GAME_ACTION) = a;
    } else {
        mff_cycles(0x9457, 1);
    }
L_945D:;
    mff_cycles(0x945D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $945E (bank 0) - lifted by port/tools/lift.py */
void func_945E_b0(void) {
    REGS_IN;
    mff_cycles(0x945E, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x02;
    c = a >= m;
    if (a == m) { mff_cycles(0x9465, 1); goto L_9496; }
    mff_cycles(0x9467, 4);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9469, 1); goto L_94C5; }
    mff_cycles(0x946B, 11);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    m = prg_read((uint16_t)(0xE273 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x9473, 2);
    if (!z) {
        mff_cycles(0x9475, 4);
        a = prg_read((uint16_t)(0xE1E6 + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x9478, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x147A, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x947B, 4);
        a = prg_read((uint16_t)(0xE22B + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x947E, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1480, func_F440, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x9473, 1);
    }
    mff_cycles(0x9481, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1483, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9484, 4);
    m = 0xB0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x9486, 1); goto L_94C4; }
    mff_cycles(0x9488, 8);
    y = 0x01;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x148C, 0x987A, 0)) return;
    SYNC_IN;
    mff_cycles(0x948D, 18);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER) = a;
    SYNC_OUT;
    mff_rts(); return;
L_9496:;
    mff_cycles(0x9496, 6);
    a = RAM(OBJ_TIMER);
    if (a == 0) {
        mff_cycles(0x949B, 8);
        y = 0x02;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x149F, 0x987A, 0)) return;
        SYNC_IN;
        mff_cycles(0x94A0, 7);
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(0xE219 + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x94A5, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x14A7, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x94A8, 4);
        a = prg_read((uint16_t)(0xE22E + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x94AB, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x14AD, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x94AE, 26);
        t = (uint8_t)(RAM(OBJ_STATE) + 1);
        RAM(OBJ_STATE) = t;
        a = 0x00;
        RAM(OBJ_VY_FRAC) = a;
        a = 0x02;
        RAM(OBJ_VY) = a;
        a = 0x24;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x14BF, func_F34C, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x94C0, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9499, 1);
    }
    mff_cycles(0x94C1, 6);
    t = (uint8_t)(RAM(OBJ_TIMER) - 1);
    RAM(OBJ_TIMER) = t;
    n = (t) >> 7;
    z = (t) == 0;
L_94C4:;
    mff_cycles(0x94C4, 6);
    SYNC_OUT;
    mff_rts(); return;
L_94C5:;
    mff_cycles(0x94C5, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x14C7, func_F1BF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x94C8, 8);
    a = RAM(OBJ_Y);
    if (a >= 0x88) {
        mff_cycles(0x94CF, 10);
        a = RAM(OBJ_FLAGS);
        a |= 0x20;
        RAM(OBJ_FLAGS) = a;
    } else {
        mff_cycles(0x94CD, 1);
    }
    mff_cycles(0x94D7, 8);
    a = RAM(OBJ_Y);
    m = 0xAC;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x94C4, 1); goto L_94C4; }
    mff_cycles(0x94DE, 14);
    a = 0xF4;
    RAM(OBJ_FLAGS) = a;
    a = RAM(OBJ_Y);
    m = 0xE8;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x94C4, 1); goto L_94C4; }
    mff_cycles(0x94EA, 11);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $94F2 (bank 0) - lifted by port/tools/lift.py */
void func_94F2_b0(void) {
    REGS_IN;
    mff_cycles(0x94F2, 11);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $94F7 (bank 0) - lifted by port/tools/lift.py */
void func_94F7_b0(void) {
    REGS_IN;
    mff_cycles(0x94F7, 30);
    a = 0xFF;
    RAM(UNK_B2) = a;
    a = 0x7F;
    RAM(AREA_CLEAR) = a;
    a = 0xFD;
    RAM(SPECIAL_SCREEN_ID) = a;
    a = 0x23;
    RAM(CHR_BANKS + 3) = a;
    RAM(CHR_BANKS + 4) = a;
    a = 0x35;
    RAM(CHR_BANKS + 5) = a;
    x = 0x00;
    for (;;) {
        mff_cycles(0x950F, 4);
        a = prg_read((uint16_t)(0x955D + x));
        mff_cycles(0x9512, 16);
        RAM(PALETTE_BUF + 24 + x) = a;
        RAM(PALETTE_BASE + 24 + x) = a;
        x++;
        m = 0x08;
        c = x >= m;
        if (x == m) break;
        mff_cycles(0x950F, 1);
    }
    mff_cycles(0x951D, 31);
    x = 0xFF;
    RAM(PALETTE_REQUEST) = x;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(JUMP_MOVING) = a;
    RAM(COMBO_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(PLAYER_INVULN) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1531, 0xCEAA, 0)) return;
    SYNC_IN;
    mff_cycles(0x9532, 60);
    a = 0x80;
    RAM(OBJ_STATE + 15) = a;
    RAM(OBJ_FLAGS + 15) = a;
    a = 0x00;
    RAM(OBJ_X + 15) = a;
    RAM(OBJ_Y + 15) = a;
    RAM(OBJ_ANIM + 15) = a;
    RAM(OBJ_ANIM_TIMER + 15) = a;
    RAM(OBJ_ANIM_FRAME + 15) = a;
    a = RAM(OBJ_X_HI);
    RAM(OBJ_X_HI + 15) = a;
    a = RAM(OBJ_Y_HI);
    RAM(OBJ_Y_HI + 15) = a;
    a = 0x70;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ID + 15) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9565 (bank 0) - lifted by port/tools/lift.py */
void func_9565_b0(void) {
    REGS_IN;
    mff_cycles(0x9565, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0x956E, 13);
        t = (uint8_t)(RAM(OBJ_STATE) + 1);
        RAM(OBJ_STATE) = t;
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(0xE1E6 + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x9576, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1578, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9579, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x157B, func_F03F, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x956C, 1);
    }
    mff_cycles(0x957C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x157E, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x957F, 8);
    a = RAM(OBJ_X);
    m = 0xFC;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x9586, 5);
        a = 0x0E;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(GAME_ACTION) = a;
    } else {
        mff_cycles(0x9584, 1);
    }
    mff_cycles(0x958A, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $958B (bank 0) - lifted by port/tools/lift.py */
void func_958B_b0(void) {
    REGS_IN;
    mff_cycles(0x958B, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    if (a == 0x02) { mff_cycles(0x9592, 1); goto L_95BD; }
    mff_cycles(0x9594, 4);
    m = 0x03;
    c = a >= m;
    if (a == m) { mff_cycles(0x9596, 2); goto L_960B; }
    mff_cycles(0x9598, 8);
    y = 0x03;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x159C, 0x987A, 0)) return;
    SYNC_IN;
    mff_cycles(0x959D, 8);
    a = 0x21;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x15A1, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x95A2, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1E6 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x95A7, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x15A9, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x95AA, 8);
    a = RAM(OBJ_X);
    m = 0xE0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x95B1, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x15B3, func_F048, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x95B4, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x95BA, 2); call_by_address_tail(0x95BA, 0); return;
    } else {
        mff_cycles(0x95AF, 1);
    }
    mff_cycles(0x95B7, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x15B9, func_F03F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x95BA, 6);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
L_95BD:;
    mff_cycles(0x95BD, 8);
    a = RAM(OBJ_Y);
    m = 0x9C;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0x95C4, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x15C6, func_F1BF, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x95C7, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x95C2, 1);
    }
    mff_cycles(0x95C8, 8);
    a = RAM(OBJ_Y);
    m = 0xA4;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x95CF, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x15D1, func_F1DF, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x95D2, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x95CD, 1);
    }
    mff_cycles(0x95D3, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x95DA, 2); goto L_960F; }
    mff_cycles(0x95DC, 8);
    a = RAM(OBJ_FLAGS);
    m = 0x90;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x95E1, 1); goto L_95F0; }
    mff_cycles(0x95E3, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x15E5, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x95E6, 8);
    a = RAM(OBJ_X);
    m = 0xC8;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x95EB, 2); goto L_960F; }
    mff_cycles(0x95ED, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x95FA, 2); call_by_address_tail(0x95FA, 0); return;
L_95F0:;
    mff_cycles(0x95F0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x15F2, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x95F3, 8);
    a = RAM(OBJ_X);
    m = 0xC8;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x95F8, 2); goto L_960F; }
    mff_cycles(0x95FA, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE219 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x95FF, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1601, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9602, 4);
    a = prg_read((uint16_t)(0xE22E + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9605, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1607, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9608, 6);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
L_960B:;
    mff_cycles(0x960B, 5);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
L_960F:;
    mff_cycles(0x960F, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $95BA (bank 0) - lifted by port/tools/lift.py */
void func_95BA_b0(void) {
    REGS_IN;
    mff_cycles(0x95BA, 14);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    a = RAM(OBJ_Y);
    m = 0x9C;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0x95C4, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x15C6, func_F1BF, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x95C7, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x95C2, 1);
    }
    mff_cycles(0x95C8, 8);
    a = RAM(OBJ_Y);
    m = 0xA4;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x95CF, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x15D1, func_F1DF, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x95D2, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x95CD, 1);
    }
    mff_cycles(0x95D3, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x95DA, 2); goto L_960F; }
    mff_cycles(0x95DC, 8);
    a = RAM(OBJ_FLAGS);
    m = 0x90;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x95E1, 1); goto L_95F0; }
    mff_cycles(0x95E3, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x15E5, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x95E6, 8);
    a = RAM(OBJ_X);
    m = 0xC8;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x95EB, 2); goto L_960F; }
    mff_cycles(0x95ED, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x95FA, 2); call_by_address_tail(0x95FA, 0); return;
L_95F0:;
    mff_cycles(0x95F0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x15F2, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x95F3, 8);
    a = RAM(OBJ_X);
    m = 0xC8;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x95FA, 7);
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(0xE219 + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x95FF, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1601, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9602, 4);
        a = prg_read((uint16_t)(0xE22E + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x9605, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1607, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9608, 11);
        t = (uint8_t)(RAM(OBJ_STATE) + 1);
        RAM(OBJ_STATE) = t;
        a = 0x0E;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(GAME_ACTION) = a;
    } else {
        mff_cycles(0x95F8, 2);
    }
L_960F:;
    mff_cycles(0x960F, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $95FA (bank 0) - lifted by port/tools/lift.py */
void func_95FA_b0(void) {
    REGS_IN;
    mff_cycles(0x95FA, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE219 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x95FF, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1601, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9602, 4);
    a = prg_read((uint16_t)(0xE22E + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9605, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1607, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9608, 17);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9610 (bank 0) - lifted by port/tools/lift.py */
void func_9610_b0(void) {
    REGS_IN;
    mff_cycles(0x9610, 11);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9615 (bank 0) - lifted by port/tools/lift.py */
void func_9615_b0(void) {
    REGS_IN;
    mff_cycles(0x9615, 11);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $961A (bank 0) - lifted by port/tools/lift.py */
void func_961A_b0(void) {
    REGS_IN;
    mff_cycles(0x961A, 11);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $961F (bank 0) - lifted by port/tools/lift.py */
void func_961F_b0(void) {
    REGS_IN;
    mff_cycles(0x961F, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x02;
    c = a >= m;
    if (a == m) {
        mff_cycles(0x9628, 12);
        a = 0xB4;
        RAM(OBJ_TIMER) = a;
        t = (uint8_t)(RAM(OBJ_STATE) + 1);
        RAM(OBJ_STATE) = t;
    } else {
        mff_cycles(0x9626, 1);
    }
    mff_cycles(0x9630, 12);
    t = (uint8_t)(RAM(OBJ_TIMER) - 1);
    RAM(OBJ_TIMER) = t;
    a = RAM(OBJ_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0x9638, 5);
        a = 0x0E;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(GAME_ACTION) = a;
    } else {
        mff_cycles(0x9636, 1);
    }
    mff_cycles(0x963C, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $963D (bank 0) - lifted by port/tools/lift.py */
void func_963D_b0(void) {
    REGS_IN;
    mff_cycles(0x963D, 8);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    if (a == 0) {
        mff_cycles(0x9644, 29);
        RAM(OBJ_VX_FRAC) = a;
        a = 0x01;
        RAM(OBJ_VX) = a;
        t = (uint8_t)(RAM(OBJ_STATE) + 1);
        RAM(OBJ_STATE) = t;
        a = 0x1E;
        RAM(OBJ_TIMER) = a;
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(0xE1F2 + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x9659, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x165B, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x965C, 4);
        a = prg_read((uint16_t)(0xE23A + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x965F, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1661, func_F440, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x9642, 1);
    }
    mff_cycles(0x9662, 8);
    a = RAM(OBJ_STATE);
    a &= 0x02;
    if (a == 0) {
        mff_cycles(0x9669, 8);
        a = RAM(OBJ_ANIM_FRAME);
        m = 0x03;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) {
            mff_cycles(0x9670, 6);
            a = RAM(OBJ_ANIM_TIMER);
            n = (a) >> 7;
            z = (a) == 0;
            if (z) {
                mff_cycles(0x9675, 8);
                a = 0x21;
                n = (a) >> 7;
                z = (a) == 0;
                SYNC_OUT;
                if (!mff_call_fn(0x1679, func_F34C, 0xE000)) return;
                SYNC_IN;
                mff_cycles(0x967A, 8);
                y = 0x00;
                n = (y) >> 7;
                z = (y) == 0;
                SYNC_OUT;
                if (!mff_call_at(0x167E, 0x987A, 0)) return;
                SYNC_IN;
                mff_cycles(0x967F, 12);
                t = (uint8_t)(RAM(OBJ_STATE) + 1);
                RAM(OBJ_STATE) = t;
                n = (t) >> 7;
                z = (t) == 0;
                SYNC_OUT;
                if (!mff_call_at(0x1684, 0xCEAA, 0)) return;
                SYNC_IN;
            } else {
                mff_cycles(0x9673, 1);
            }
        } else {
            mff_cycles(0x966E, 1);
        }
        mff_cycles(0x9685, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9667, 1);
    }
    mff_cycles(0x9686, 6);
    a = RAM(OBJ_TIMER);
    if (a != 0) {
        mff_cycles(0x968B, 12);
        t = (uint8_t)(RAM(OBJ_TIMER) - 1);
        RAM(OBJ_TIMER) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9689, 1);
    }
    mff_cycles(0x968F, 11);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    m = prg_read((uint16_t)(0xE273 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x9697, 2);
    if (!z) {
        mff_cycles(0x9699, 4);
        a = prg_read((uint16_t)(0xE1E6 + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x969C, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x169E, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x969F, 4);
        a = prg_read((uint16_t)(0xE22B + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x96A2, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x16A4, func_F440, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x9697, 1);
    }
    mff_cycles(0x96A5, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x16A7, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x96A8, 8);
    a = RAM(OBJ_X);
    m = 0x1C;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x96AD, 1); goto L_96DB; }
    mff_cycles(0x96AF, 18);
    a = RAM(OBJ_FLAGS);
    a &= 0xDF;
    RAM(OBJ_FLAGS) = a;
    a = RAM(OBJ_X);
    m = 0x30;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x96BC, 1); goto L_96DB; }
    mff_cycles(0x96BE, 20);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0x00;
    RAM(GAME_ACTION) = a;
    RAM(OBJ_TIMER) = a;
    a = RAM(ROUND);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x96CC, 1); goto L_96DB; }
    mff_cycles(0x96CE, 7);
    a = RAM(SUB_AREA);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0x96D2, 1); goto L_96DC; }
    mff_cycles(0x96D4, 4);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x96D6, 1); goto L_96DC; }
    mff_cycles(0x96D8, 6);
    SYNC_OUT;
    if (!mff_call_at(0x16DA, 0xCEAA, 0)) return;
    SYNC_IN;
L_96DB:;
    mff_cycles(0x96DB, 6);
    SYNC_OUT;
    mff_rts(); return;
L_96DC:;
    mff_cycles(0x96DC, 12);
    a = 0x20;
    RAM(GAME_ACTION) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1C1 + y));
    mff_cycles(0x96E5, 8);
    RAM(OBJ_VX_FRAC) = a;
    a = prg_read((uint16_t)(0xE1C4 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x96EB, 10);
    RAM(OBJ_VX) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $96EF (bank 0) - lifted by port/tools/lift.py */
void func_96EF_b0(void) {
    REGS_IN;
    mff_cycles(0x96EF, 8);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    if (a == 0) {
        mff_cycles(0x96F6, 17);
        RAM(OBJ_VY_FRAC) = a;
        a = 0x05;
        RAM(OBJ_VY) = a;
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(0xE20A + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x9703, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1705, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9706, 4);
        a = prg_read((uint16_t)(0xE252 + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x9709, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x170B, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x970C, 6);
        t = (uint8_t)(RAM(OBJ_STATE) + 1);
        RAM(OBJ_STATE) = t;
    } else {
        mff_cycles(0x96F4, 2);
    }
    mff_cycles(0x970F, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9716, 1); goto L_974A; }
    mff_cycles(0x9718, 4);
    if (a == 0x03) { mff_cycles(0x971A, 1); goto L_9779; }
    mff_cycles(0x971C, 4);
    m = 0x04;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x9720, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x97A9, 2); call_by_address_tail(0x97A9, 0); return;
    } else {
        mff_cycles(0x971E, 1);
    }
    mff_cycles(0x9723, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1725, func_F1BF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9726, 8);
    a = RAM(OBJ_Y);
    m = 0x98;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x972D, 8);
        a = 0x05;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x1731, func_F34A, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9732, 30);
        a = 0xE6;
        RAM(OBJ_VX_FRAC) = a;
        a = 0x00;
        RAM(OBJ_VX) = a;
        a = 0xE8;
        RAM(OBJ_VY_FRAC) = a;
        a = 0x03;
        RAM(OBJ_VY) = a;
        t = (uint8_t)(RAM(OBJ_STATE) + 1);
        RAM(OBJ_STATE) = t;
        n = (t) >> 7;
        z = (t) == 0;
    } else {
        mff_cycles(0x972B, 1);
    }
L_9749:;
    mff_cycles(0x9749, 6);
    SYNC_OUT;
    mff_rts(); return;
L_974A:;
    mff_cycles(0x974A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x174C, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x974D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x174F, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9750, 8);
    a = RAM(OBJ_Y);
    m = 0xA0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x9749, 1); goto L_9749; }
    mff_cycles(0x9757, 13);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE20D + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x975F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1761, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9762, 4);
    a = prg_read((uint16_t)(0xE255 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9765, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1767, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9768, 6);
    y = 0x0F;
    a = prg_read((uint16_t)(0x97F9 + y));
    mff_cycles(0x976D, 14);
    RAM(PALETTE_BUF + y) = a;
    RAM(PALETTE_BASE + y) = a;
    y--;
    n = (y) >> 7;
    z = (y) == 0;
    if (!n) { mff_cycles(0x9774, 1); goto L_979A; }
    mff_cycles(0x9776, 9);
    RAM(PALETTE_REQUEST) = y;
    SYNC_OUT;
    mff_rts(); return;
L_9779:;
    mff_cycles(0x9779, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x06;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x977E, 1); goto L_97A8; }
    mff_cycles(0x9780, 8);
    a = RAM(OBJ_ANIM_TIMER);
    m = 0x0A;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x9785, 1); goto L_97A8; }
    mff_cycles(0x9787, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE210 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x978C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x178E, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x978F, 4);
    a = prg_read((uint16_t)(0xE258 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9792, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1794, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9795, 8);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    y = 0x0F;
L_979A:;
    for (;;) {
        mff_cycles(0x979A, 4);
        a = prg_read((uint16_t)(0x97F9 + y));
        mff_cycles(0x979D, 14);
        RAM(PALETTE_BUF + y) = a;
        RAM(PALETTE_BASE + y) = a;
        y--;
        n = (y) >> 7;
        z = (y) == 0;
        if (n) break;
        mff_cycles(0x979A, 1);
    }
    mff_cycles(0x97A6, 3);
    RAM(PALETTE_REQUEST) = y;
L_97A8:;
    mff_cycles(0x97A8, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $97A9 (bank 0) - lifted by port/tools/lift.py */
void func_97A9_b0(void) {
    REGS_IN;
    goto L_97A9;
L_97A8:;
    mff_cycles(0x97A8, 6);
    SYNC_OUT;
    mff_rts(); return;
L_97A9:;
    mff_cycles(0x97A9, 8);
    a = RAM(OBJ_ANIM_TIMER);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x97A8, 1); goto L_97A8; }
    mff_cycles(0x97B0, 2);
    y = 0x0F;
    for (;;) {
        mff_cycles(0x97B2, 4);
        a = prg_read((uint16_t)(0x9809 + y));
        mff_cycles(0x97B5, 14);
        RAM(PALETTE_BUF + y) = a;
        RAM(PALETTE_BASE + y) = a;
        y--;
        n = (y) >> 7;
        z = (y) == 0;
        if (n) break;
        mff_cycles(0x97B2, 1);
    }
    mff_cycles(0x97BE, 9);
    RAM(PALETTE_REQUEST) = y;
    SYNC_OUT;
    if (!mff_call_at(0x17C2, 0x97C8, 0)) return;
    SYNC_IN;
    mff_cycles(0x97C3, 11);
    a = 0x20;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $97C8 (bank 0) - lifted by port/tools/lift.py */
void func_97C8_b0(void) {
    REGS_IN;
    mff_cycles(0x97C8, 46);
    a = 0x80;
    RAM(BG_PAL_ANIM_ON + 2) = a;
    RAM(BG_PAL_ANIM_ON + 3) = a;
    a = 0x05;
    RAM(BG_PAL_ANIM_SCRIPT + 2) = a;
    a = 0x06;
    RAM(BG_PAL_ANIM_SCRIPT + 3) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(BG_PAL_ANIM_STEP + 2) = a;
    RAM(BG_PAL_ANIM_TIMER + 2) = a;
    RAM(BG_PAL_ANIM_STEP + 3) = a;
    RAM(BG_PAL_ANIM_TIMER + 3) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9819 (bank 0) - lifted by port/tools/lift.py */
void func_9819_b0(void) {
    REGS_IN;
    mff_cycles(0x9819, 11);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    m = prg_read((uint16_t)(0xE273 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x9821, 2);
    if (!z) {
        mff_cycles(0x9823, 4);
        a = prg_read((uint16_t)(0xE1E6 + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x9826, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1828, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9829, 4);
        a = prg_read((uint16_t)(0xE22B + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x982C, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x182E, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x982F, 4);
        a = prg_read((uint16_t)(0xE1C1 + y));
        mff_cycles(0x9832, 8);
        RAM(OBJ_VX_FRAC) = a;
        a = prg_read((uint16_t)(0xE1C4 + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x9838, 4);
        RAM(OBJ_VX) = a;
    } else {
        mff_cycles(0x9821, 1);
    }
    mff_cycles(0x983B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x183D, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x983E, 5);
    a = RAM(ROUND);
    if (a != 0) { mff_cycles(0x9840, 1); goto L_985F; }
    mff_cycles(0x9842, 7);
    a = RAM(SUB_AREA);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x9848, 8);
        a = RAM(OBJ_X);
        m = 0xB0;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0x984F, 21);
            a = 0x80;
            RAM(OBJ_STATE) = a;
            a = 0x00;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(GAME_ACTION) = a;
            RAM(OBJ_TIMER) = a;
            SYNC_OUT;
            if (!mff_call_at(0x185D, 0xCEAA, 0)) return;
            SYNC_IN;
        } else {
            mff_cycles(0x984D, 1);
        }
    } else {
        mff_cycles(0x9846, 1);
    }
L_985E:;
    mff_cycles(0x985E, 6);
    SYNC_OUT;
    mff_rts(); return;
L_985F:;
    mff_cycles(0x985F, 7);
    a = RAM(ROUND);
    m = 0x04;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x985E, 1); goto L_985E; }
    mff_cycles(0x9865, 7);
    a = RAM(SUB_AREA);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x985E, 1); goto L_985E; }
    mff_cycles(0x986B, 6);
    a = RAM(OBJ_X_HI);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x985E, 1); goto L_985E; }
    mff_cycles(0x9870, 8);
    a = RAM(OBJ_X);
    m = 0x80;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x985E, 1); goto L_985E; }
    mff_cycles(0x9877, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x984F, 2); call_by_address_tail(0x984F, 0); return;
}

/*  $984F (bank 0) - lifted by port/tools/lift.py */
void func_984F_b0(void) {
    REGS_IN;
    mff_cycles(0x984F, 21);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(OBJ_TIMER) = a;
    SYNC_OUT;
    if (!mff_call_at(0x185D, 0xCEAA, 0)) return;
    SYNC_IN;
    mff_cycles(0x985E, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $987A (bank 0) - lifted by port/tools/lift.py */
void func_987A_b0(void) {
    REGS_IN;
    mff_cycles(0x987A, 7);
    RAM(TMP15) = x;
    a = prg_read((uint16_t)(0x989A + y));
    mff_cycles(0x987F, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0x989E + y));
    mff_cycles(0x9884, 9);
    RAM(TMP1) = a;
    a = 0x00;
    x = a;
    y = a;
    for (;;) {
        mff_cycles(0x988A, 5);
        a = mem_read((uint16_t)(RAM16(TMP0) + y));
        mff_cycles(0x988C, 13);
        RAM(VRAM_BUFFER + x) = a;
        x++;
        y++;
        m = 0x80;
        c = y >= m;
        if (y == m) break;
        mff_cycles(0x988A, 1);
    }
    mff_cycles(0x9895, 12);
    RAM(VRAM_LIST_REQUEST) = y;
    x = RAM(TMP15);
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9A00 (bank 0) - lifted by port/tools/lift.py */
void func_9A00_b0(void) {
    REGS_IN;
    mff_cycles(0x9A00, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1A02, 0x9AC0, 0)) return;
    SYNC_IN;
    mff_cycles(0x9A03, 7);
    y = RAM(INTRO_PAGE);
    a = prg_read((uint16_t)(0x9B2C + y));
    mff_cycles(0x9A08, 7);
    RAM(TMP12) = a;
    a = prg_read((uint16_t)(0x9B33 + y));
    mff_cycles(0x9A0D, 16);
    RAM(TMP13) = a;
    a = 0x00;
    RAM(TMP14) = a;
    a = 0x00;
    RAM(TMP16) = a;
    RAM(TMP18) = a;
L_9A19:;
    mff_cycles(0x9A19, 12);
    a = 0x00;
    RAM(TMP17) = a;
    a = RAM(JOY_PRESSED);
    a &= 0xF0;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x9A21, 1); goto L_9A91; }
    mff_cycles(0x9A23, 7);
    x = RAM(TMP16);
    a = prg_read((uint16_t)(0x9A92 + x));
    mff_cycles(0x9A28, 8);
    RAM(VRAM_BUFFER) = a;
    a = prg_read((uint16_t)(0x9A95 + x));
    mff_cycles(0x9A2E, 28);
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
    mff_cycles(0x9A3F, 4);
    m = 0xFF;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9A41, 1); goto L_9A8E; }
    mff_cycles(0x9A43, 4);
    m = 0xFE;
    c = a >= m;
    if (a != m) {
        mff_cycles(0x9A47, 31);
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
        if (!mff_call_at(0x1A59, 0x9A98, 0)) return;
        SYNC_IN;
        mff_cycles(0x9A5A, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1A5C, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9A5D, 6);
        SYNC_OUT;
        mff_call_fn(0x1A5F, func_FF31, 0xE000);
        SYNC_IN;
        mff_cycles(0x9A60, 6);
        SYNC_OUT;
        mff_call_fn(0x1A62, func_FF31, 0xE000);
        SYNC_IN;
        mff_cycles(0x9A63, 26);
        a = mff_pull();
        RAM(TMP17) = a;
        a = mff_pull();
        RAM(TMP16) = a;
        t = (uint8_t)(RAM(TMP17) + 1);
        RAM(TMP17) = t;
        a = RAM(FRAME_COUNTER);
        a &= 0x01;
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0x9A71, 5);
            a = RAM(INTRO_PAGE);
            n = (a) >> 7;
            z = (a) == 0;
            if (z) {
                mff_cycles(0x9A75, 8);
                a = 0x2B;
                n = (a) >> 7;
                z = (a) == 0;
                SYNC_OUT;
                if (!mff_call_fn(0x1A79, func_F34C, 0xE000)) return;
                SYNC_IN;
            } else {
                mff_cycles(0x9A73, 1);
            }
        } else {
            mff_cycles(0x9A6F, 1);
        }
        mff_cycles(0x9A7A, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x9A1D, 2); call_by_address_tail(0x9A1D, 0); return;
    } else {
        mff_cycles(0x9A45, 1);
    }
    mff_cycles(0x9A7D, 12);
    t = (uint8_t)(RAM(TMP16) + 1);
    RAM(TMP16) = t;
    a = RAM(TMP16);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x9A19, 1); goto L_9A19; }
    mff_cycles(0x9A85, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1A87, 0x9B16, 0)) return;
    SYNC_IN;
    mff_cycles(0x9A88, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1A8A, 0x9AC0, 0)) return;
    SYNC_IN;
    mff_cycles(0x9A8B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9A13, 2); call_by_address_tail(0x9A13, 0); return;
L_9A8E:;
    mff_cycles(0x9A8E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9B16, 2); call_by_address_tail(0x9B16, 0); return;
L_9A91:;
    mff_cycles(0x9A91, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9A13 (bank 0) - lifted by port/tools/lift.py */
void func_9A13_b0(void) {
    REGS_IN;
    mff_cycles(0x9A13, 8);
    a = 0x00;
    RAM(TMP16) = a;
    RAM(TMP18) = a;
L_9A19:;
    mff_cycles(0x9A19, 12);
    a = 0x00;
    RAM(TMP17) = a;
    a = RAM(JOY_PRESSED);
    a &= 0xF0;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x9A21, 1); goto L_9A91; }
    mff_cycles(0x9A23, 7);
    x = RAM(TMP16);
    a = prg_read((uint16_t)(0x9A92 + x));
    mff_cycles(0x9A28, 8);
    RAM(VRAM_BUFFER) = a;
    a = prg_read((uint16_t)(0x9A95 + x));
    mff_cycles(0x9A2E, 28);
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
    mff_cycles(0x9A3F, 4);
    m = 0xFF;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9A41, 1); goto L_9A8E; }
    mff_cycles(0x9A43, 4);
    m = 0xFE;
    c = a >= m;
    if (a != m) {
        mff_cycles(0x9A47, 31);
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
        if (!mff_call_at(0x1A59, 0x9A98, 0)) return;
        SYNC_IN;
        mff_cycles(0x9A5A, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1A5C, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9A5D, 6);
        SYNC_OUT;
        mff_call_fn(0x1A5F, func_FF31, 0xE000);
        SYNC_IN;
        mff_cycles(0x9A60, 6);
        SYNC_OUT;
        mff_call_fn(0x1A62, func_FF31, 0xE000);
        SYNC_IN;
        mff_cycles(0x9A63, 26);
        a = mff_pull();
        RAM(TMP17) = a;
        a = mff_pull();
        RAM(TMP16) = a;
        t = (uint8_t)(RAM(TMP17) + 1);
        RAM(TMP17) = t;
        a = RAM(FRAME_COUNTER);
        a &= 0x01;
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0x9A71, 5);
            a = RAM(INTRO_PAGE);
            n = (a) >> 7;
            z = (a) == 0;
            if (z) {
                mff_cycles(0x9A75, 8);
                a = 0x2B;
                n = (a) >> 7;
                z = (a) == 0;
                SYNC_OUT;
                if (!mff_call_fn(0x1A79, func_F34C, 0xE000)) return;
                SYNC_IN;
            } else {
                mff_cycles(0x9A73, 1);
            }
        } else {
            mff_cycles(0x9A6F, 1);
        }
        mff_cycles(0x9A7A, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x9A1D, 2); call_by_address_tail(0x9A1D, 0); return;
    } else {
        mff_cycles(0x9A45, 1);
    }
    mff_cycles(0x9A7D, 12);
    t = (uint8_t)(RAM(TMP16) + 1);
    RAM(TMP16) = t;
    a = RAM(TMP16);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x9A19, 1); goto L_9A19; }
    mff_cycles(0x9A85, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1A87, 0x9B16, 0)) return;
    SYNC_IN;
    mff_cycles(0x9A88, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1A8A, 0x9AC0, 0)) return;
    SYNC_IN;
    mff_cycles(0x9A8B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9A13, 2); call_by_address_tail(0x9A13, 0); return;
L_9A8E:;
    mff_cycles(0x9A8E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9B16, 2); call_by_address_tail(0x9B16, 0); return;
L_9A91:;
    mff_cycles(0x9A91, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9A1D (bank 0) - lifted by port/tools/lift.py */
void func_9A1D_b0(void) {
    REGS_IN;
    goto L_9A1D;
L_9A19:;
    mff_cycles(0x9A19, 5);
    a = 0x00;
    RAM(TMP17) = a;
L_9A1D:;
    mff_cycles(0x9A1D, 7);
    a = RAM(JOY_PRESSED);
    a &= 0xF0;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x9A21, 1); goto L_9A91; }
    mff_cycles(0x9A23, 7);
    x = RAM(TMP16);
    a = prg_read((uint16_t)(0x9A92 + x));
    mff_cycles(0x9A28, 8);
    RAM(VRAM_BUFFER) = a;
    a = prg_read((uint16_t)(0x9A95 + x));
    mff_cycles(0x9A2E, 28);
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
    mff_cycles(0x9A3F, 4);
    m = 0xFF;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9A41, 1); goto L_9A8E; }
    mff_cycles(0x9A43, 4);
    m = 0xFE;
    c = a >= m;
    if (a != m) {
        mff_cycles(0x9A47, 31);
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
        if (!mff_call_at(0x1A59, 0x9A98, 0)) return;
        SYNC_IN;
        mff_cycles(0x9A5A, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1A5C, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9A5D, 6);
        SYNC_OUT;
        mff_call_fn(0x1A5F, func_FF31, 0xE000);
        SYNC_IN;
        mff_cycles(0x9A60, 6);
        SYNC_OUT;
        mff_call_fn(0x1A62, func_FF31, 0xE000);
        SYNC_IN;
        mff_cycles(0x9A63, 26);
        a = mff_pull();
        RAM(TMP17) = a;
        a = mff_pull();
        RAM(TMP16) = a;
        t = (uint8_t)(RAM(TMP17) + 1);
        RAM(TMP17) = t;
        a = RAM(FRAME_COUNTER);
        a &= 0x01;
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0x9A71, 5);
            a = RAM(INTRO_PAGE);
            n = (a) >> 7;
            z = (a) == 0;
            if (z) {
                mff_cycles(0x9A75, 8);
                a = 0x2B;
                n = (a) >> 7;
                z = (a) == 0;
                SYNC_OUT;
                if (!mff_call_fn(0x1A79, func_F34C, 0xE000)) return;
                SYNC_IN;
            } else {
                mff_cycles(0x9A73, 1);
            }
        } else {
            mff_cycles(0x9A6F, 1);
        }
        mff_cycles(0x9A7A, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x9A1D, 2); call_by_address_tail(0x9A1D, 0); return;
    } else {
        mff_cycles(0x9A45, 1);
    }
    mff_cycles(0x9A7D, 12);
    t = (uint8_t)(RAM(TMP16) + 1);
    RAM(TMP16) = t;
    a = RAM(TMP16);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x9A19, 1); goto L_9A19; }
    mff_cycles(0x9A85, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1A87, 0x9B16, 0)) return;
    SYNC_IN;
    mff_cycles(0x9A88, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1A8A, 0x9AC0, 0)) return;
    SYNC_IN;
    mff_cycles(0x9A8B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9A13, 2); call_by_address_tail(0x9A13, 0); return;
L_9A8E:;
    mff_cycles(0x9A8E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9B16, 2); call_by_address_tail(0x9B16, 0); return;
L_9A91:;
    mff_cycles(0x9A91, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9A98 (bank 0) - lifted by port/tools/lift.py */
void func_9A98_b0(void) {
    REGS_IN;
    mff_cycles(0x9A98, 7);
    a = RAM(INTRO_PAGE);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0x9A9C, 1); goto L_9AA7; }
    mff_cycles(0x9A9E, 4);
    m = 0x05;
    c = a >= m;
    if (a == m) { mff_cycles(0x9AA0, 1); goto L_9AAE; }
    mff_cycles(0x9AA2, 4);
    m = 0x06;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9AA4, 1); goto L_9AB5; }
    mff_cycles(0x9AA6, 6);
    SYNC_OUT;
    mff_rts(); return;
L_9AA7:;
    mff_cycles(0x9AA7, 8);
    a = 0x02;
    RAM(OBJ_ANIM_FRAME) = a;
    if (a != 0) { mff_cycles(0x9AAC, 1); goto L_9ABA; }
L_9AAE:;
    mff_cycles(0x9AAE, 8);
    a = 0x01;
    RAM(OBJ_ANIM_FRAME) = a;
    if (a != 0) { mff_cycles(0x9AB3, 1); goto L_9ABA; }
L_9AB5:;
    mff_cycles(0x9AB5, 6);
    a = 0x00;
    RAM(OBJ_ANIM_FRAME) = a;
L_9ABA:;
    mff_cycles(0x9ABA, 12);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ANIM_TIMER) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9AC0 (bank 0) - lifted by port/tools/lift.py */
void func_9AC0_b0(void) {
    REGS_IN;
    mff_cycles(0x9AC0, 76);
    a = 0x23;
    RAM(VRAM_BUFFER) = a;
    a = 0x79;
    RAM(VRAM_BUFFER + 1) = a;
    a = 0x00;
    RAM(VRAM_BUFFER + 2) = a;
    a = 0x00;
    RAM(VRAM_BUFFER + 3) = a;
    a = 0x23;
    RAM(VRAM_BUFFER + 4) = a;
    a = 0x22;
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
        mff_cycles(0x9AFF, 21);
        RAM(VRAM_BUFFER + 7 + y) = a;
        RAM(VRAM_BUFFER + 29 + y) = a;
        RAM(VRAM_BUFFER + 51 + y) = a;
        y++;
        x--;
        if (x & 0x80) break;
        mff_cycles(0x9AFF, 2);
    }
    mff_cycles(0x9B0C, 12);
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(VRAM_BUFFER + 70) = a;
    RAM(VRAM_LIST_REQUEST) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xFF31, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_FF31(); g_code_window_base = swb; } return;
}

/*  $9B16 (bank 0) - lifted by port/tools/lift.py */
void func_9B16_b0(void) {
    REGS_IN;
    mff_cycles(0x9B16, 7);
    a = RAM(INTRO_PAGE);
    m = 0x05;
    c = a >= m;
    if (a != m) {
        mff_cycles(0x9B1C, 4);
        m = 0x06;
        c = a >= m;
        if (a != m) {
            mff_cycles(0x9B20, 8);
            x = 0xB7;
            n = (x) >> 7;
            z = (x) == 0;
            SYNC_OUT;
            mff_call_fn(0x1B24, func_FF2A, 0xE000);
            SYNC_IN;
            mff_cycles(0x9B25, 6);
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0x9B1E, 1);
        }
    } else {
        mff_cycles(0x9B1A, 1);
    }
    mff_cycles(0x9B26, 8);
    x = 0x60;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    mff_call_fn(0x1B2A, func_FF2A, 0xE000);
    SYNC_IN;
    mff_cycles(0x9B2B, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A000 (bank 0) - lifted by port/tools/lift.py */
void func_A000_b0(void) {
    REGS_IN;
    mff_cycles(0xA000, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA0FD, 2); call_by_address_tail(0xA0FD, 0); return;
}

/*  $A003 (bank 0) - lifted by port/tools/lift.py */
void func_A003_b0(void) {
    REGS_IN;
    mff_cycles(0xA003, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA101, 2); call_by_address_tail(0xA101, 0); return;
}

/*  $A006 (bank 0) - lifted by port/tools/lift.py */
void func_A006_b0(void) {
    REGS_IN;
    mff_cycles(0xA006, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A009 (bank 0) - lifted by port/tools/lift.py */
void func_A009_b0(void) {
    REGS_IN;
    mff_cycles(0xA009, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAA33, 2); call_by_address_tail(0xAA33, 0); return;
}

/*  $A00C (bank 0) - lifted by port/tools/lift.py */
void func_A00C_b0(void) {
    REGS_IN;
    mff_cycles(0xA00C, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A00F (bank 0) - lifted by port/tools/lift.py */
void func_A00F_b0(void) {
    REGS_IN;
    mff_cycles(0xA00F, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A012 (bank 0) - lifted by port/tools/lift.py */
void func_A012_b0(void) {
    REGS_IN;
    mff_cycles(0xA012, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A015 (bank 0) - lifted by port/tools/lift.py */
void func_A015_b0(void) {
    REGS_IN;
    mff_cycles(0xA015, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A018 (bank 0) - lifted by port/tools/lift.py */
void func_A018_b0(void) {
    REGS_IN;
    mff_cycles(0xA018, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A01B (bank 0) - lifted by port/tools/lift.py */
void func_A01B_b0(void) {
    REGS_IN;
    mff_cycles(0xA01B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A01E (bank 0) - lifted by port/tools/lift.py */
void func_A01E_b0(void) {
    REGS_IN;
    mff_cycles(0xA01E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A021 (bank 0) - lifted by port/tools/lift.py */
void func_A021_b0(void) {
    REGS_IN;
    mff_cycles(0xA021, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A024 (bank 0) - lifted by port/tools/lift.py */
void func_A024_b0(void) {
    REGS_IN;
    mff_cycles(0xA024, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A027 (bank 0) - lifted by port/tools/lift.py */
void func_A027_b0(void) {
    REGS_IN;
    mff_cycles(0xA027, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A02A (bank 0) - lifted by port/tools/lift.py */
void func_A02A_b0(void) {
    REGS_IN;
    mff_cycles(0xA02A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A02D (bank 0) - lifted by port/tools/lift.py */
void func_A02D_b0(void) {
    REGS_IN;
    mff_cycles(0xA02D, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA09B, 2); call_by_address_tail(0xA09B, 0); return;
}

/*  $A030 (bank 0) - lifted by port/tools/lift.py */
void func_A030_b0(void) {
    REGS_IN;
    mff_cycles(0xA030, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A031 (bank 0) - lifted by port/tools/lift.py */
void func_A031_b0(void) {
    REGS_IN;
    mff_cycles(0xA031, 9);
    RAM(TMP0) = y;
    a = RAM(OBJ_TIER + x);
    if (a != 0) {
        mff_cycles(0xA038, 2);
        a = 0x01;
    } else {
        mff_cycles(0xA036, 1);
    }
    mff_cycles(0xA03A, 15);
    y = a;
    a = 0x00;
    RAM(TMP15) = a;
    a = RAM(OBJ_DIR + x);
    a &= 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xA044, 1); goto L_A05D; }
    mff_cycles(0xA046, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0048, func_F1DF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA049, 8);
    a = RAM(OBJ_Y + x);
    m = prg_read((uint16_t)(0xA08E + y));
    c = a >= m;
    mff_cycles(0xA04F, 2);
    if (c) { mff_cycles(0xA04F, 1); goto L_A077; }
    mff_cycles(0xA051, 4);
    a = prg_read((uint16_t)(0xA08E + y));
    mff_cycles(0xA054, 12);
    RAM(OBJ_Y + x) = a;
    a = 0x08;
    RAM(TMP15) = a;
    if (a != 0) { mff_cycles(0xA05B, 1); goto L_A077; }
L_A05D:;
    mff_cycles(0xA05D, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0xA064, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0066, func_F1BF, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA067, 8);
        a = RAM(OBJ_Y + x);
        m = 0xAF;
        c = a >= m;
        if (c) {
            mff_cycles(0xA06E, 12);
            a = 0xAE;
            RAM(OBJ_Y + x) = a;
            a = 0x04;
            RAM(TMP15) = a;
        } else {
            mff_cycles(0xA06C, 1);
        }
    } else {
        mff_cycles(0xA062, 1);
    }
L_A077:;
    mff_cycles(0xA077, 11);
    y = RAM(TMP0);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0xA080, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF183, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F183(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xA07E, 1);
    }
    mff_cycles(0xA083, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0xA08A, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF1A1, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F1A1(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xA088, 1);
    }
    mff_cycles(0xA08D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A090 (bank 0) - lifted by port/tools/lift.py */
void func_A090_b0(void) {
    REGS_IN;
    mff_cycles(0xA090, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0092, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA093, 2);
    if (!c) {
        mff_cycles(0xA095, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF099, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F099(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xA093, 1);
    }
    mff_cycles(0xA098, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF090, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F090(); g_code_window_base = swb; } return;
}

/*  $A09B (bank 0) - lifted by port/tools/lift.py */
void func_A09B_b0(void) {
    REGS_IN;
    mff_cycles(0xA09B, 8);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0xA0A2, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x00A4, func_F374, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA0A5, 4);
        m = 0x10;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!c) {
            mff_cycles(0xA0A9, 16);
            a = 0x01;
            RAM(PLAYER_DIR) = a;
            a = 0x0D;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(ATTACK_ID) = a;
            SYNC_OUT;
            if (!mff_call_fn(0x00B3, func_F455, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xA0B4, 8);
            a = 0x18;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x00B8, func_F34C, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xA0B9, 15);
            t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
            RAM(OBJ_STATE + x) = t;
            a = RAM(OBJ_X);
            m = 0x70;
            c = a >= m;
            n = ((uint8_t)(a - m)) >> 7;
            z = ((uint8_t)(a - m)) == 0;
            if (c) {
                mff_cycles(0xA0C3, 6);
                t = (uint8_t)(RAM(OBJ_DOWN_TIMER) + 1);
                RAM(OBJ_DOWN_TIMER) = t;
                n = (t) >> 7;
                z = (t) == 0;
            } else {
                mff_cycles(0xA0C1, 1);
            }
        } else {
            mff_cycles(0xA0A7, 1);
        }
        mff_cycles(0xA0C6, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xA0A0, 1);
    }
    mff_cycles(0xA0C7, 8);
    a = RAM(OBJ_X);
    m = 0x70;
    c = a >= m;
    if (c) {
        mff_cycles(0xA0CE, 6);
        a = 0x02;
        RAM(OBJ_DOWN_TIMER) = a;
    } else {
        mff_cycles(0xA0CC, 1);
    }
    mff_cycles(0xA0D3, 10);
    a = 0xFF;
    y = 0x31;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x00D9, func_F443, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA0DA, 22);
    a = 0x90;
    RAM(OBJ_FLAGS + x) = a;
    a = RAM(OBJ_X + x);
    c = 0;
    m = 0x03;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_X + x) = a;
    if (!c) {
        mff_cycles(0xA0EA, 13);
        a = RAM(OBJ_Y + x);
        m = 0x02;
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_Y + x) = a;
        if (c) {
            mff_cycles(0xA0F4, 6);
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xA0F2, 1);
        }
    } else {
        mff_cycles(0xA0E8, 1);
    }
    mff_cycles(0xA0F5, 9);
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DOWN_TIMER) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF2BE, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F2BE(); g_code_window_base = swb; } return;
}

/*  $A0FD (bank 0) - lifted by port/tools/lift.py */
void func_A0FD_b0(void) {
    REGS_IN;
    mff_cycles(0xA0FD, 4);
    a = 0x07;
    if (a == 0) {
        mff_cycles(0xA101, 2);
        a = 0x08;
    } else {
        mff_cycles(0xA0FF, 1);
    }
    mff_cycles(0xA103, 23);
    RAM(OBJ_KIND + x) = a;
    a = 0x00;
    RAM(OBJ_TIER + x) = a;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_INVULN + x);
    if (a != 0) {
        mff_cycles(0xA113, 7);
        t = (uint8_t)(RAM(OBJ_INVULN + x) - 1);
        RAM(OBJ_INVULN + x) = t;
    } else {
        mff_cycles(0xA111, 1);
    }
    mff_cycles(0xA116, 8);
    a = RAM(OBJ_ID + 13);
    m = 0x6E;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA11B, 1); goto L_A123; }
    mff_cycles(0xA11D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x011F, 0xA6D6, 0)) return;
    SYNC_IN;
    mff_cycles(0xA120, 2);
    if (!c) { mff_cycles(0xA120, 1); goto L_A129; }
    mff_cycles(0xA122, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A123:;
    mff_cycles(0xA123, 13);
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_INVULN + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_A129:;
    mff_cycles(0xA129, 12);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    y = a;
    a = prg_read((uint16_t)(0xA6C4 + y));
    mff_cycles(0xA132, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0xA6CD + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA137, 8);
    RAM(TMP1) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $A101 (bank 0) - lifted by port/tools/lift.py */
void func_A101_b0(void) {
    REGS_IN;
    mff_cycles(0xA101, 25);
    a = 0x08;
    RAM(OBJ_KIND + x) = a;
    a = 0x00;
    RAM(OBJ_TIER + x) = a;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_INVULN + x);
    if (a != 0) {
        mff_cycles(0xA113, 7);
        t = (uint8_t)(RAM(OBJ_INVULN + x) - 1);
        RAM(OBJ_INVULN + x) = t;
    } else {
        mff_cycles(0xA111, 1);
    }
    mff_cycles(0xA116, 8);
    a = RAM(OBJ_ID + 13);
    m = 0x6E;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA11B, 1); goto L_A123; }
    mff_cycles(0xA11D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x011F, 0xA6D6, 0)) return;
    SYNC_IN;
    mff_cycles(0xA120, 2);
    if (!c) { mff_cycles(0xA120, 1); goto L_A129; }
    mff_cycles(0xA122, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A123:;
    mff_cycles(0xA123, 13);
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_INVULN + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_A129:;
    mff_cycles(0xA129, 12);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    y = a;
    a = prg_read((uint16_t)(0xA6C4 + y));
    mff_cycles(0xA132, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0xA6CD + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA137, 8);
    RAM(TMP1) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $A13C (bank 0) - lifted by port/tools/lift.py */
void func_A13C_b0(void) {
    REGS_IN;
    mff_cycles(0xA13C, 6);
    SYNC_OUT;
    if (!mff_call_at(0x013E, 0xA090, 0)) return;
    SYNC_IN;
    mff_cycles(0xA13F, 8);
    a = RAM(OBJ_X_HI + x);
    m = 0x01;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xA146, 13);
        a = 0x01;
        RAM(OBJ_DIR + x) = a;
        a = RAM(OBJ_X_HI + x);
        if (a != 0) {
            mff_cycles(0xA150, 7);
            a = 0x02;
            RAM(OBJ_DIR + x) = a;
        } else {
            mff_cycles(0xA14E, 1);
        }
        mff_cycles(0xA155, 13);
        a = 0x88;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_STATE + x) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xA144, 1);
    }
    mff_cycles(0xA15B, 27);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x05;
    RAM(OBJ_DOWN_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A169 (bank 0) - lifted by port/tools/lift.py */
void func_A169_b0(void) {
    REGS_IN;
    mff_cycles(0xA169, 7);
    a = RAM(GAME_ACTION);
    m = 0x1F;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA16D, 1); goto L_A19A; }
    mff_cycles(0xA16F, 4);
    m = 0x06;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA171, 1); goto L_A19A; }
    mff_cycles(0xA173, 8);
    a = RAM(OBJ_KIND + x);
    m = 0x07;
    c = a >= m;
    if (a != m) { mff_cycles(0xA178, 1); goto L_A180; }
    mff_cycles(0xA17A, 6);
    a = 0x1C;
    y = 0x0A;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0xA17E, 1); goto L_A184; }
L_A180:;
    mff_cycles(0xA180, 4);
    a = 0x24;
    y = 0x09;
    n = (y) >> 7;
    z = (y) == 0;
L_A184:;
    mff_cycles(0xA184, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0186, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA187, 2);
    if (c) {
        mff_cycles(0xA189, 31);
        a = RAM(OBJ_KIND + x);
        m = 0x07;
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        RAM(OBJ_TIMER2 + x) = a;
        a = 0x00;
        RAM(OBJ_TIMER + x) = a;
        t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_STATE + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xA187, 1);
    }
L_A19A:;
    mff_cycles(0xA19A, 6);
    SYNC_OUT;
    if (!mff_call_at(0x019C, 0xA882, 0)) return;
    SYNC_IN;
    mff_cycles(0xA19D, 6);
    a = RAM(OBJ_TIMER + x);
    if (a == 0) {
        mff_cycles(0xA1A2, 23);
        a = RAM(RNG);
        a &= 0x01;
        RAM(OBJ_AI_FLAG + x) = a;
        a = 0x04;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_LAND_Y + x) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x01B0, func_F374, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA1B1, 2);
        if (c) {
            mff_cycles(0xA1B3, 7);
            a = 0x0C;
            RAM(OBJ_LAND_Y + x) = a;
        } else {
            mff_cycles(0xA1B1, 1);
        }
        mff_cycles(0xA1B8, 13);
        t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        RAM(OBJ_TIMER + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xA1A0, 1);
    }
    mff_cycles(0xA1BC, 8);
    a = RAM(OBJ_X_HI + x);
    if (a == 0x01) { mff_cycles(0xA1C1, 1); goto L_A1D5; }
    mff_cycles(0xA1C3, 4);
    if (a == 0x02) { mff_cycles(0xA1C5, 1); goto L_A1CB; }
    mff_cycles(0xA1C7, 4);
    a = 0x01;
    if (a != 0) { mff_cycles(0xA1C9, 1); goto L_A1CD; }
L_A1CB:;
    mff_cycles(0xA1CB, 2);
    a = 0xFE;
L_A1CD:;
    mff_cycles(0xA1CD, 12);
    RAM(OBJ_X + x) = a;
    a = 0x01;
    RAM(OBJ_X_HI + x) = a;
L_A1D5:;
    mff_cycles(0xA1D5, 8);
    a = RAM(OBJ_LAND_Y + x);
    if (a != 0x04) { mff_cycles(0xA1DA, 1); goto L_A1E5; }
    mff_cycles(0xA1DC, 8);
    a = RAM(OBJ_X + x);
    m = 0xF1;
    c = a >= m;
    if (c) { mff_cycles(0xA1E1, 2); goto L_A20C; }
    mff_cycles(0xA1E3, 2);
    if (!c) { mff_cycles(0xA1E3, 2); goto L_A231; }
L_A1E5:;
    mff_cycles(0xA1E5, 4);
    if (a != 0x0C) { mff_cycles(0xA1E7, 1); goto L_A1F2; }
    mff_cycles(0xA1E9, 8);
    a = RAM(OBJ_X + x);
    m = 0x18;
    c = a >= m;
    if (!c) { mff_cycles(0xA1EE, 2); goto L_A20C; }
    mff_cycles(0xA1F0, 2);
    if (c) { mff_cycles(0xA1F0, 2); goto L_A231; }
L_A1F2:;
    mff_cycles(0xA1F2, 4);
    if (a != 0x00) { mff_cycles(0xA1F4, 1); goto L_A1FF; }
    mff_cycles(0xA1F6, 8);
    a = RAM(OBJ_Y + x);
    m = 0x86;
    c = a >= m;
    if (!c) { mff_cycles(0xA1FB, 2); goto L_A20C; }
    mff_cycles(0xA1FD, 2);
    if (c) { mff_cycles(0xA1FD, 2); goto L_A231; }
L_A1FF:;
    mff_cycles(0xA1FF, 4);
    m = 0x08;
    c = a >= m;
    if (a != m) { mff_cycles(0xA201, 1); goto L_A231; }
    mff_cycles(0xA203, 8);
    a = RAM(OBJ_Y + x);
    m = 0xAE;
    c = a >= m;
    if (c) { mff_cycles(0xA208, 1); goto L_A20C; }
    mff_cycles(0xA20A, 2);
    if (!c) { mff_cycles(0xA20A, 1); goto L_A231; }
L_A20C:;
    mff_cycles(0xA20C, 6);
    a = RAM(OBJ_AI_FLAG + x);
    if (a == 0) {
        mff_cycles(0xA211, 18);
        a = RAM(OBJ_LAND_Y + x);
        c = 0;
        m = 0x04;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        a &= 0x0F;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_LAND_Y + x) = a;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA22A, 2); call_by_address_tail(0xA22A, 0); return;
    } else {
        mff_cycles(0xA20F, 1);
    }
    mff_cycles(0xA21F, 23);
    a = RAM(OBJ_LAND_Y + x);
    c = 1;
    m = 0x04;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    a &= 0x0F;
    RAM(OBJ_LAND_Y + x) = a;
    y = a;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x022D, 0xA7E1, 0)) return;
    SYNC_IN;
    mff_cycles(0xA22E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA28B, 2); call_by_address_tail(0xA28B, 0); return;
L_A231:;
    mff_cycles(0xA231, 10);
    y = RAM(OBJ_LAND_Y + x);
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0236, 0xA7E1, 0)) return;
    SYNC_IN;
    mff_cycles(0xA237, 4);
    a = prg_read((uint16_t)(0xFBF0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA23A, 11);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x023F, 0xA031, 0)) return;
    SYNC_IN;
    mff_cycles(0xA240, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0242, 0xA090, 0)) return;
    SYNC_IN;
    mff_cycles(0xA243, 8);
    a = RAM(OBJ_KIND + x);
    m = 0x07;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA248, 1); goto L_A26E; }
    mff_cycles(0xA24A, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x0C;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xA24F, 1); goto L_A258; }
    mff_cycles(0xA251, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0253, func_FBD9, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA254, 4);
    m = 0x02;
    c = a >= m;
    if (!c) { mff_cycles(0xA256, 1); goto L_A26F; }
L_A258:;
    mff_cycles(0xA258, 8);
    a = RAM(OBJ_Y + x);
    if (a < 0x90) { mff_cycles(0xA25D, 1); goto L_A263; }
    mff_cycles(0xA25F, 4);
    m = 0x98;
    c = a >= m;
    if (!c) { mff_cycles(0xA261, 1); goto L_A2CB; }
L_A263:;
    mff_cycles(0xA263, 8);
    a = RAM(OBJ_X + x);
    m = 0x7C;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xA268, 1); goto L_A26E; }
    mff_cycles(0xA26A, 4);
    m = 0x84;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xA26C, 1); goto L_A2CB; }
L_A26E:;
    mff_cycles(0xA26E, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A26F:;
    mff_cycles(0xA26F, 8);
    a = RAM(OBJ_DOWN_TIMER + x);
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA274, 1); goto L_A28A; }
    mff_cycles(0xA276, 16);
    a = 0x03;
    RAM(OBJ_DOWN_TIMER + x) = a;
    a = RAM(RNG);
    a &= 0x0F;
    m = 0x0C;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xA281, 1); goto L_A28A; }
    mff_cycles(0xA283, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0285, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA286, 4);
    a = 0x86;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA288, 1); goto L_A2E4; }
L_A28A:;
    mff_cycles(0xA28A, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A2CB:;
    mff_cycles(0xA2CB, 8);
    a = RAM(OBJ_DOWN_TIMER + x);
    a &= 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA2D0, 1); goto L_A2E3; }
    mff_cycles(0xA2D2, 16);
    a = 0x05;
    RAM(OBJ_DOWN_TIMER + x) = a;
    a = RAM(RNG);
    a &= 0x0F;
    m = 0x0A;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xA2DD, 1); goto L_A2E3; }
    mff_cycles(0xA2DF, 4);
    a = 0x83;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA2E1, 1); goto L_A2E4; }
L_A2E3:;
    mff_cycles(0xA2E3, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A2E4:;
    mff_cycles(0xA2E4, 18);
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A22A (bank 0) - lifted by port/tools/lift.py */
void func_A22A_b0(void) {
    REGS_IN;
    mff_cycles(0xA22A, 8);
    y = a;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x022D, 0xA7E1, 0)) return;
    SYNC_IN;
    mff_cycles(0xA22E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA28B, 2); call_by_address_tail(0xA28B, 0); return;
}

/*  $A28B (bank 0) - lifted by port/tools/lift.py */
void func_A28B_b0(void) {
    REGS_IN;
    mff_cycles(0xA28B, 8);
    a = RAM(OBJ_DOWN_TIMER + x);
    t = a;
    c = t & 1;
    t >>= 1;
    if (c) { mff_cycles(0xA28F, 1); goto L_A2AA; }
    mff_cycles(0xA291, 8);
    a = RAM(OBJ_KIND + x);
    if (a == 0x08) { mff_cycles(0xA296, 1); goto L_A2B3; }
    mff_cycles(0xA298, 16);
    a = 0x00;
    RAM(OBJ_DOWN_TIMER + x) = a;
    a = RAM(RNG);
    a &= 0x0F;
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xA2A3, 1); goto L_A2A9; }
    mff_cycles(0xA2A5, 4);
    a = 0x85;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA2A7, 1); goto L_A2E4; }
L_A2A9:;
    mff_cycles(0xA2A9, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A2AA:;
    mff_cycles(0xA2AA, 17);
    a = RAM(OBJ_DOWN_TIMER + x);
    a &= 0x06;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DOWN_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_A2B3:;
    mff_cycles(0xA2B3, 16);
    a = 0x00;
    RAM(OBJ_DOWN_TIMER + x) = a;
    a = RAM(RNG);
    a &= 0x0F;
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xA2BE, 1); goto L_A2CA; }
    mff_cycles(0xA2C0, 7);
    a = RAM(GAME_ACTION);
    m = 0x06;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA2C4, 1); goto L_A2CA; }
    mff_cycles(0xA2C6, 4);
    a = 0x84;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA2C8, 1); goto L_A2E4; }
L_A2CA:;
    mff_cycles(0xA2CA, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A2E4:;
    mff_cycles(0xA2E4, 18);
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A2ED (bank 0) - lifted by port/tools/lift.py */
void func_A2ED_b0(void) {
    REGS_IN;
    mff_cycles(0xA2ED, 6);
    a = RAM(OBJ_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA2F0, 2); goto L_A31F; }
    mff_cycles(0xA2F2, 11);
    RAM(OBJ_TIMER3 + x) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x02F7, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA2F8, 6);
    a = RAM(OBJ_TIMER2 + x);
    if (a != 0) { mff_cycles(0xA2FB, 2); goto L_A303; }
    mff_cycles(0xA2FD, 6);
    a = 0x03;
    y = 0x02;
    if (y != 0) { mff_cycles(0xA301, 1); goto L_A307; }
L_A303:;
    mff_cycles(0xA303, 4);
    a = 0x01;
    y = 0x00;
L_A307:;
    mff_cycles(0xA307, 12);
    RAM(OBJ_AI_COUNT + x) = a;
    a = RAM(GAME_ACTION);
    m = 0x01;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xA310, 8);
        a = y;
        c = 0;
        m = 0x0A;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        y = a;
    } else {
        mff_cycles(0xA30E, 1);
    }
    mff_cycles(0xA315, 17);
    a = y;
    RAM(OBJ_AI_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA83A, 2); call_by_address_tail(0xA83A, 0); return;
L_A31F:;
    mff_cycles(0xA31F, 6);
    a = RAM(OBJ_AI_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0xA324, 25);
        t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
        RAM(OBJ_AI_TIMER + x) = t;
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_ANIM_TIMER + x) = a;
        RAM(OBJ_ANIM_FRAME + x) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xA322, 1);
    }
    mff_cycles(0xA330, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0332, 0xA8B7, 0)) return;
    SYNC_IN;
    mff_cycles(0xA333, 6);
    a = RAM(OBJ_TIMER2 + x);
    if (a == 0) { mff_cycles(0xA336, 1); goto L_A350; }
    mff_cycles(0xA338, 10);
    a = RAM(OBJ_ANIM_TIMER + x);
    a |= RAM(OBJ_ANIM_FRAME + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0xA340, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA3EA, 2); call_by_address_tail(0xA3EA, 0); return;
    } else {
        mff_cycles(0xA33E, 1);
    }
    mff_cycles(0xA343, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    if (a < 0x04) { mff_cycles(0xA348, 1); goto L_A379; }
    mff_cycles(0xA34A, 4);
    m = 0x09;
    c = a >= m;
    if (!c) { mff_cycles(0xA34C, 1); goto L_A36E; }
    mff_cycles(0xA34E, 2);
    if (c) { mff_cycles(0xA34E, 1); goto L_A3C5; }
L_A350:;
    mff_cycles(0xA350, 6);
    a = RAM(OBJ_ANIM_TIMER + x);
    if (a == 0) {
        mff_cycles(0xA355, 8);
        a = RAM(OBJ_ANIM_FRAME + x);
        m = 0x09;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) {
            mff_cycles(0xA35C, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xA3EA, 2); call_by_address_tail(0xA3EA, 0); return;
        } else {
            mff_cycles(0xA35A, 1);
        }
    } else {
        mff_cycles(0xA353, 1);
    }
    mff_cycles(0xA35F, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    if (a < 0x02) { mff_cycles(0xA364, 1); goto L_A379; }
    mff_cycles(0xA366, 4);
    m = 0x05;
    c = a >= m;
    if (!c) { mff_cycles(0xA368, 1); goto L_A3C5; }
    mff_cycles(0xA36A, 4);
    m = 0x07;
    c = a >= m;
    if (c) { mff_cycles(0xA36C, 1); goto L_A3C5; }
L_A36E:;
    mff_cycles(0xA36E, 8);
    y = RAM(OBJ_TIMER2 + x);
    a = prg_read((uint16_t)(0xA413 + y));
    mff_cycles(0xA374, 7);
    RAM(OBJ_ATTACK + x) = a;
    if (a != 0) { mff_cycles(0xA377, 1); goto L_A382; }
L_A379:;
    mff_cycles(0xA379, 8);
    y = RAM(OBJ_TIMER2 + x);
    a = prg_read((uint16_t)(0xA411 + y));
    mff_cycles(0xA37F, 5);
    RAM(OBJ_ATTACK + x) = a;
L_A382:;
    mff_cycles(0xA382, 6);
    a = RAM(OBJ_TIMER2 + x);
    if (a != 0) { mff_cycles(0xA385, 1); goto L_A399; }
    mff_cycles(0xA387, 8);
    a = RAM(OBJ_AI_COUNT + x);
    m = 0x01;
    c = a >= m;
    if (a != m) { mff_cycles(0xA38C, 1); goto L_A395; }
    mff_cycles(0xA38E, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x05;
    c = a >= m;
    if (c) { mff_cycles(0xA393, 1); goto L_A399; }
L_A395:;
    mff_cycles(0xA395, 4);
    a = 0x1E;
    if (a != 0) { mff_cycles(0xA397, 1); goto L_A3AF; }
L_A399:;
    mff_cycles(0xA399, 7);
    a = RAM(GAME_ACTION);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0xA39D, 1); goto L_A3A9; }
    mff_cycles(0xA39F, 7);
    a = RAM(ROUND);
    m = 0x04;
    c = a >= m;
    if (a != m) { mff_cycles(0xA3A3, 1); goto L_A3A9; }
    mff_cycles(0xA3A5, 4);
    a = 0x05;
    if (a != 0) { mff_cycles(0xA3A7, 1); goto L_A3AF; }
L_A3A9:;
    mff_cycles(0xA3A9, 8);
    y = RAM(OBJ_TIMER2 + x);
    a = prg_read((uint16_t)(0xA415 + y));
L_A3AF:;
    mff_cycles(0xA3AF, 11);
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = RAM(OBJ_HIT_PLAYER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0xA3B7, 13);
        a = 0x18;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_TIMER3 + x) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x03BE, func_F34C, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA3BF, 7);
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_HIT_PLAYER + x) = a;
    } else {
        mff_cycles(0xA3B5, 1);
    }
    mff_cycles(0xA3C4, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A3C5:;
    mff_cycles(0xA3C5, 6);
    a = RAM(OBJ_TIMER3 + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xA3C8, 1); goto L_A3E9; }
    mff_cycles(0xA3CA, 7);
    a = RAM(ROUND);
    if (a != 0x04) { mff_cycles(0xA3CE, 1); goto L_A3E1; }
    mff_cycles(0xA3D0, 7);
    a = RAM(GAME_ACTION);
    m = 0x06;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA3D4, 1); goto L_A3E9; }
    mff_cycles(0xA3D6, 20);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x88;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_A3E1:;
    mff_cycles(0xA3E1, 7);
    a = 0x22;
    m = RAM(PLAYER_INVULN);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0xA3E7, 3);
        RAM(PLAYER_INVULN) = a;
    } else {
        mff_cycles(0xA3E5, 1);
    }
L_A3E9:;
    mff_cycles(0xA3E9, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A3EA (bank 0) - lifted by port/tools/lift.py */
void func_A3EA_b0(void) {
    REGS_IN;
    mff_cycles(0xA3EA, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x03EC, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA3ED, 9);
    t = (uint8_t)(RAM(OBJ_AI_COUNT + x) - 1);
    RAM(OBJ_AI_COUNT + x) = t;
    if (t != 0) {
        mff_cycles(0xA3F2, 10);
        a = 0x20;
        y = 0x0A;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x03F8, func_FB3E, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA3F9, 2);
        if (c) {
            mff_cycles(0xA3FB, 13);
            a = 0x03;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_AI_TIMER + x) = a;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xA3F9, 2);
        }
    } else {
        mff_cycles(0xA3F0, 2);
    }
    mff_cycles(0xA401, 27);
    a = 0x80;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x07;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DOWN_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A417 (bank 0) - lifted by port/tools/lift.py */
void func_A417_b0(void) {
    REGS_IN;
    mff_cycles(0xA417, 6);
    a = RAM(OBJ_TIMER + x);
    if (a == 0) {
        mff_cycles(0xA41C, 27);
        a = 0x78;
        RAM(OBJ_AI_TIMER + x) = a;
        a = 0x01;
        RAM(OBJ_AI_COUNT + x) = a;
        t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        RAM(OBJ_TIMER + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xA41A, 1);
    }
    mff_cycles(0xA42A, 10);
    a = 0x20;
    y = 0x0A;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0430, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA431, 2);
    if (c) {
        mff_cycles(0xA433, 25);
        a = 0x82;
        RAM(OBJ_STATE + x) = a;
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_TIMER + x) = a;
        RAM(OBJ_TIMER2 + x) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xA431, 1);
    }
    mff_cycles(0xA441, 9);
    t = (uint8_t)(RAM(OBJ_AI_COUNT + x) - 1);
    RAM(OBJ_AI_COUNT + x) = t;
    if (t == 0) {
        mff_cycles(0xA446, 13);
        a = 0x08;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_AI_COUNT + x) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x044D, func_FB72, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA44E, 8);
        y = a;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x0451, 0xA7E1, 0)) return;
        SYNC_IN;
        mff_cycles(0xA452, 4);
        a = prg_read((uint16_t)(0xFBF0 + y));
        mff_cycles(0xA455, 5);
        RAM(OBJ_DIR + x) = a;
    } else {
        mff_cycles(0xA444, 1);
    }
    mff_cycles(0xA458, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) {
        mff_cycles(0xA45D, 6);
        SYNC_OUT;
        if (!mff_call_at(0x045F, 0xA031, 0)) return;
        SYNC_IN;
        mff_cycles(0xA460, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA090, 2); call_by_address_tail(0xA090, 0); return;
    } else {
        mff_cycles(0xA45B, 1);
    }
    mff_cycles(0xA463, 27);
    a = 0x81;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x07;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DOWN_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A473 (bank 0) - lifted by port/tools/lift.py */
void func_A473_b0(void) {
    REGS_IN;
    mff_cycles(0xA473, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA478, 1); goto L_A49D; }
    mff_cycles(0xA47A, 4);
    if (a == 0x02) { mff_cycles(0xA47C, 1); goto L_A4E2; }
    mff_cycles(0xA47E, 4);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xA482, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA53B, 2); call_by_address_tail(0xA53B, 0); return;
    } else {
        mff_cycles(0xA480, 1);
    }
    mff_cycles(0xA485, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0487, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA488, 6);
    SYNC_OUT;
    if (!mff_call_at(0x048A, 0xA9CA, 0)) return;
    SYNC_IN;
    mff_cycles(0xA48B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x048D, 0xA9F5, 0)) return;
    SYNC_IN;
    mff_cycles(0xA48E, 17);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    a = 0xF4;
    y = 0x2C;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0497, func_F449, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA498, 11);
    a = 0x2E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(CHR_BANKS + 5) = a;
    SYNC_OUT;
    mff_rts(); return;
L_A49D:;
    mff_cycles(0xA49D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x049F, 0xA031, 0)) return;
    SYNC_IN;
    mff_cycles(0xA4A0, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    if (a == 0) {
        mff_cycles(0xA4A7, 8);
        a = RAM(OBJ_X + x);
        m = 0xF1;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0xA4AE, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xA54C, 2); call_by_address_tail(0xA54C, 0); return;
        } else {
            mff_cycles(0xA4AC, 1);
        }
    } else {
        mff_cycles(0xA4A5, 1);
    }
    mff_cycles(0xA4B1, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0xA4B8, 8);
        a = RAM(OBJ_X + x);
        m = 0x18;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!c) {
            mff_cycles(0xA4BF, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xA54C, 2); call_by_address_tail(0xA54C, 0); return;
        } else {
            mff_cycles(0xA4BD, 1);
        }
    } else {
        mff_cycles(0xA4B6, 1);
    }
    mff_cycles(0xA4C2, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x04C4, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA4C5, 4);
    m = 0x30;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xA4C7, 1); goto L_A4E1; }
    mff_cycles(0xA4C9, 10);
    a = 0xF6;
    y = 0x2F;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x04CF, func_F44F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA4D0, 6);
    a = RAM(OBJ_TIER + x);
    if (a == 0) { mff_cycles(0xA4D3, 1); goto L_A4D9; }
    mff_cycles(0xA4D5, 4);
    a = 0x2E;
    if (a != 0) { mff_cycles(0xA4D7, 1); goto L_A4DB; }
L_A4D9:;
    mff_cycles(0xA4D9, 2);
    a = 0x17;
L_A4DB:;
    mff_cycles(0xA4DB, 12);
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
L_A4E1:;
    mff_cycles(0xA4E1, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A4E2:;
    mff_cycles(0xA4E2, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    if (a == 0x01) {
        mff_cycles(0xA4E9, 7);
        a = 0x00;
        RAM(OBJ_ANIM_TIMER + x) = a;
    } else {
        mff_cycles(0xA4E7, 1);
    }
    mff_cycles(0xA4EE, 6);
    a = RAM(OBJ_VX + x);
    if (a != 0) {
        mff_cycles(0xA4F3, 7);
        a = 0x8B;
        RAM(OBJ_ATTACK + x) = a;
    } else {
        mff_cycles(0xA4F1, 1);
    }
    mff_cycles(0xA4F8, 58);
    a = RAM(OBJ_VX_FRAC + x);
    c = 1;
    m = RAM(OBJ_TIMER2 + x);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_VX_FRAC + x) = a;
    a = RAM(OBJ_VX + x);
    m = 0x00;
    w = a - m - !c;
    a = (uint8_t)w;
    RAM(OBJ_VX + x) = a;
    a = RAM(OBJ_VY_FRAC + x);
    c = 1;
    m = RAM(OBJ_GROUND_Y + x);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_VY_FRAC + x) = a;
    a = RAM(OBJ_VY + x);
    m = 0x00;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_VY + x) = a;
    a = RAM(OBJ_VX + x);
    if (!(a & 0x80)) {
        mff_cycles(0xA521, 6);
        a = RAM(OBJ_VY + x);
        n = (a) >> 7;
        z = (a) == 0;
        if (!n) {
            mff_cycles(0xA526, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xA031, 2); call_by_address_tail(0xA031, 0); return;
        } else {
            mff_cycles(0xA524, 1);
        }
    } else {
        mff_cycles(0xA51F, 1);
    }
    mff_cycles(0xA529, 13);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    a = RAM(OBJ_TIER + x);
    if (a == 0) { mff_cycles(0xA52F, 1); goto L_A535; }
    mff_cycles(0xA531, 4);
    a = 0x06;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA533, 1); goto L_A537; }
L_A535:;
    mff_cycles(0xA535, 2);
    a = 0x0C;
    n = (a) >> 7;
    z = (a) == 0;
L_A537:;
    mff_cycles(0xA537, 11);
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A53B (bank 0) - lifted by port/tools/lift.py */
void func_A53B_b0(void) {
    REGS_IN;
    mff_cycles(0xA53B, 6);
    a = RAM(OBJ_AI_TIMER + x);
    if (a != 0) {
        mff_cycles(0xA540, 25);
        a = 0x00;
        RAM(OBJ_ATTACK + x) = a;
        RAM(OBJ_ANIM_TIMER + x) = a;
        t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
        RAM(OBJ_AI_TIMER + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xA53E, 1);
    }
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

/*  $A557 (bank 0) - lifted by port/tools/lift.py */
void func_A557_b0(void) {
    REGS_IN;
    mff_cycles(0xA557, 8);
    a = RAM(OBJ_TIMER + x);
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (c) { mff_cycles(0xA55B, 1); goto L_A58A; }
    mff_cycles(0xA55D, 17);
    a = RAM(OBJ_Y + x);
    RAM(OBJ_GROUND_Y + x) = a;
    a = RAM(OBJ_STATE + x);
    m = 0x86;
    c = a >= m;
    if (a == m) { mff_cycles(0xA568, 1); goto L_A576; }
    mff_cycles(0xA56A, 16);
    a = 0xE0;
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x04;
    RAM(OBJ_VY + x) = a;
    if (a != 0) { mff_cycles(0xA574, 1); goto L_A580; }
L_A576:;
    mff_cycles(0xA576, 14);
    a = 0x00;
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x04;
    RAM(OBJ_VY + x) = a;
L_A580:;
    mff_cycles(0xA580, 14);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    a = 0xE7;
    y = 0x2A;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF449, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F449(); g_code_window_base = swb; } return;
L_A58A:;
    mff_cycles(0xA58A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x058C, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA58D, 6);
    a = RAM(OBJ_VY + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0xA592, 31);
        a = 0x01;
        RAM(OBJ_ANIM_FRAME + x) = a;
        a = 0x8F;
        RAM(OBJ_ATTACK + x) = a;
        a = 0x16;
        RAM(OBJ_ATTACK_DAMAGE + x) = a;
        a = RAM(OBJ_Y + x);
        m = RAM(OBJ_GROUND_Y + x);
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0xA5A9, 24);
            a = RAM(OBJ_GROUND_Y + x);
            RAM(OBJ_Y + x) = a;
            t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
            RAM(OBJ_TIMER + x) = t;
            a = RAM(OBJ_TIMER + x);
            m = 0x04;
            c = a >= m;
            n = ((uint8_t)(a - m)) >> 7;
            z = ((uint8_t)(a - m)) == 0;
            if (z) {
                mff_cycles(0xA5B9, 31);
                a = 0x00;
                RAM(OBJ_VY_FRAC + x) = a;
                RAM(OBJ_VY + x) = a;
                RAM(OBJ_ATTACK + x) = a;
                a = 0x01;
                RAM(OBJ_TIMER + x) = a;
                a = 0x81;
                n = (a) >> 7;
                z = (a) == 0;
                RAM(OBJ_STATE + x) = a;
            } else {
                mff_cycles(0xA5B7, 1);
            }
        } else {
            mff_cycles(0xA5A7, 1);
        }
    } else {
        mff_cycles(0xA590, 1);
    }
    mff_cycles(0xA5CE, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A5CF (bank 0) - lifted by port/tools/lift.py */
void func_A5CF_b0(void) {
    REGS_IN;
    mff_cycles(0xA5CF, 20);
    a = 0x00;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_VX + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x05DB, 0xA557, 0)) return;
    SYNC_IN;
    mff_cycles(0xA5DC, 8);
    a = RAM(OBJ_STATE + x);
    m = 0x81;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xA5E3, 13);
        a = 0x80;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_STATE + x) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xA5E1, 1);
    }
    mff_cycles(0xA5E9, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x03;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xA5F0, 7);
        a = 0x01;
        RAM(OBJ_TIMER + x) = a;
    } else {
        mff_cycles(0xA5EE, 1);
    }
    mff_cycles(0xA5F5, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0xA5FC, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x05FE, func_F1A1, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xA5FA, 1);
    }
    mff_cycles(0xA5FF, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0xA606, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0608, func_F183, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xA604, 1);
    }
    mff_cycles(0xA609, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    if (a != 0) { mff_cycles(0xA60E, 1); goto L_A61C; }
    mff_cycles(0xA610, 8);
    a = 0x18;
    m = RAM(OBJ_X + x);
    c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xA615, 1); goto L_A61C; }
    mff_cycles(0xA617, 7);
    RAM(OBJ_X + x) = a;
    if (!z) { mff_cycles(0xA61A, 1); goto L_A62D; }
L_A61C:;
    mff_cycles(0xA61C, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA621, 1); goto L_A632; }
    mff_cycles(0xA623, 8);
    a = 0xF1;
    m = RAM(OBJ_X + x);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xA628, 1); goto L_A632; }
    mff_cycles(0xA62A, 5);
    RAM(OBJ_X + x) = a;
L_A62D:;
    mff_cycles(0xA62D, 7);
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
L_A632:;
    mff_cycles(0xA632, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A633 (bank 0) - lifted by port/tools/lift.py */
void func_A633_b0(void) {
    REGS_IN;
    mff_cycles(0xA633, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0635, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA636, 9);
    t = (uint8_t)(RAM(OBJ_HIT_TIMER + x) - 1);
    RAM(OBJ_HIT_TIMER + x) = t;
    if (t != 0) { mff_cycles(0xA639, 1); goto L_A64B; }
    mff_cycles(0xA63B, 8);
    a = RAM(OBJ_KIND + x);
    a &= 0x08;
    if (a != 0) { mff_cycles(0xA640, 1); goto L_A646; }
    mff_cycles(0xA642, 4);
    a = 0x80;
    if (a != 0) { mff_cycles(0xA644, 1); goto L_A648; }
L_A646:;
    mff_cycles(0xA646, 2);
    a = 0x84;
L_A648:;
    mff_cycles(0xA648, 5);
    RAM(OBJ_STATE + x) = a;
L_A64B:;
    mff_cycles(0xA64B, 13);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A651 (bank 0) - lifted by port/tools/lift.py */
void func_A651_b0(void) {
    REGS_IN;
    mff_cycles(0xA651, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0653, 0xA882, 0)) return;
    SYNC_IN;
    mff_cycles(0xA654, 8);
    a = RAM(OBJ_X_HI + x);
    m = 0x01;
    c = a >= m;
    if (a != m) { mff_cycles(0xA659, 1); goto L_A684; }
    mff_cycles(0xA65B, 8);
    a = RAM(OBJ_KIND + x);
    m = 0x07;
    c = a >= m;
    if (a != m) { mff_cycles(0xA660, 1); goto L_A668; }
    mff_cycles(0xA662, 6);
    a = 0x20;
    y = 0x0A;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0xA666, 1); goto L_A66C; }
L_A668:;
    mff_cycles(0xA668, 4);
    a = 0x24;
    y = 0x09;
    n = (y) >> 7;
    z = (y) == 0;
L_A66C:;
    mff_cycles(0xA66C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x066E, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA66F, 2);
    if (c) {
        mff_cycles(0xA671, 31);
        a = RAM(OBJ_KIND + x);
        m = 0x07;
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        RAM(OBJ_TIMER2 + x) = a;
        a = 0x82;
        RAM(OBJ_STATE + x) = a;
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_TIMER + x) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xA66F, 1);
    }
L_A684:;
    mff_cycles(0xA684, 8);
    y = 0x04;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0688, 0xA7E1, 0)) return;
    SYNC_IN;
    mff_cycles(0xA689, 8);
    a = RAM(OBJ_DIR + x);
    if (a != 0x01) { mff_cycles(0xA68E, 1); goto L_A6A1; }
    mff_cycles(0xA690, 8);
    a = RAM(OBJ_X_HI + x);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA695, 1); goto L_A69E; }
    mff_cycles(0xA697, 8);
    a = RAM(OBJ_X + x);
    m = 0x18;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xA69C, 1); goto L_A6B2; }
L_A69E:;
    mff_cycles(0xA69E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF183, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F183(); g_code_window_base = swb; } return;
L_A6A1:;
    mff_cycles(0xA6A1, 8);
    a = RAM(OBJ_X_HI + x);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA6A6, 1); goto L_A6AF; }
    mff_cycles(0xA6A8, 8);
    a = RAM(OBJ_X + x);
    m = 0xF2;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xA6AD, 1); goto L_A6B2; }
L_A6AF:;
    mff_cycles(0xA6AF, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF1A1, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F1A1(); g_code_window_base = swb; } return;
L_A6B2:;
    mff_cycles(0xA6B2, 24);
    a = 0x81;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x07;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DOWN_TIMER + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF343, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F343(); g_code_window_base = swb; } return;
}

/*  $A6D6 (bank 0) - lifted by port/tools/lift.py */
void func_A6D6_b0(void) {
    REGS_IN;
    mff_cycles(0xA6D6, 6);
    a = RAM(OBJ_HELD + x);
    if (a == 0) { mff_cycles(0xA6D9, 2); goto L_A71C; }
    mff_cycles(0xA6DB, 4);
    if (a == 0xFF) { mff_cycles(0xA6DD, 2); goto L_A70B; }
    mff_cycles(0xA6DF, 7);
    a = RAM(GAME_ACTION);
    if (a == 0x04) { mff_cycles(0xA6E3, 1); goto L_A6E9; }
    mff_cycles(0xA6E5, 4);
    if (a != 0x21) { mff_cycles(0xA6E7, 2); goto L_A71C; }
L_A6E9:;
    mff_cycles(0xA6E9, 5);
    if (x != RAM(GRAB_SLOT)) { mff_cycles(0xA6EB, 2); goto L_A71C; }
    mff_cycles(0xA6ED, 7);
    a = RAM(GAME_ACTION);
    m = 0x21;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xA6F3, 6);
        SYNC_OUT;
        if (!mff_call_at(0x06F5, 0xA8EF, 0)) return;
        SYNC_IN;
        mff_cycles(0xA6F6, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA6FC, 2); call_by_address_tail(0xA6FC, 0); return;
    } else {
        mff_cycles(0xA6F1, 1);
    }
    mff_cycles(0xA6F9, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06FB, 0xA83A, 0)) return;
    SYNC_IN;
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
L_A70B:;
    mff_cycles(0xA70B, 14);
    a = 0x00;
    RAM(OBJ_HELD + x) = a;
    a = RAM(PLAYER_ID);
    if (a == 0x02) {
        mff_cycles(0xA716, 9);
        a = RAM(OBJ_Y);
        RAM(OBJ_Y + x) = a;
    } else {
        mff_cycles(0xA714, 1);
    }
L_A71C:;
    mff_cycles(0xA71C, 6);
    a = RAM(OBJ_HIT_TIMER + x);
    if (a == 0) { mff_cycles(0xA71F, 1); goto L_A74C; }
    mff_cycles(0xA721, 4);
    m = 0x1E;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA723, 1); goto L_A72D; }
    mff_cycles(0xA725, 4);
    m = 0x1A;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA727, 1); goto L_A733; }
    mff_cycles(0xA729, 4);
    m = 0x10;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA72B, 1); goto L_A736; }
L_A72D:;
    mff_cycles(0xA72D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x072F, 0xA83A, 0)) return;
    SYNC_IN;
    mff_cycles(0xA730, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA736, 2); call_by_address_tail(0xA736, 0); return;
L_A733:;
    mff_cycles(0xA733, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0735, 0xA8EF, 0)) return;
    SYNC_IN;
L_A736:;
    mff_cycles(0xA736, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0738, 0xA090, 0)) return;
    SYNC_IN;
    mff_cycles(0xA739, 15);
    a = 0x87;
    RAM(OBJ_STATE + x) = a;
    a = RAM(OBJ_TIER + x);
    m = 0x02;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xA745, 7);
        a = 0x20;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_INVULN + x) = a;
    } else {
        mff_cycles(0xA743, 1);
    }
    mff_cycles(0xA74A, 8);
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
L_A74C:;
    mff_cycles(0xA74C, 7);
    a = RAM(GAME_ACTION);
    m = 0x0A;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA750, 1); goto L_A75A; }
    mff_cycles(0xA752, 4);
    m = 0x0B;
    if (a == m) { mff_cycles(0xA754, 1); goto L_A75F; }
    mff_cycles(0xA756, 4);
    m = 0x19;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA758, 1); goto L_A766; }
L_A75A:;
    mff_cycles(0xA75A, 6);
    SYNC_OUT;
    if (!mff_call_at(0x075C, 0xA83A, 0)) return;
    SYNC_IN;
    mff_cycles(0xA75D, 8);
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
L_A75F:;
    mff_cycles(0xA75F, 15);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_HIT_PLAYER + x) = a;
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
L_A766:;
    mff_cycles(0xA766, 6);
    a = RAM(OBJ_TIER + x);
    if (a == 0) {
        mff_cycles(0xA76B, 8);
        a = RAM(OBJ_STATE + x);
        if (a != 0x81) {
            mff_cycles(0xA772, 7);
            a = RAM(GAME_ACTION);
            if (a == 0x1F) {
                mff_cycles(0xA778, 8);
                a = RAM(OBJ_STATE + x);
                m = 0x85;
                n = ((uint8_t)(a - m)) >> 7;
                z = ((uint8_t)(a - m)) == 0;
                if (!z) {
                    mff_cycles(0xA77F, 4);
                    m = 0x86;
                    n = ((uint8_t)(a - m)) >> 7;
                    z = ((uint8_t)(a - m)) == 0;
                    if (!z) {
                        mff_cycles(0xA783, 21);
                        a = 0x07;
                        RAM(OBJ_DOWN_TIMER + x) = a;
                        a = 0x80;
                        RAM(OBJ_STATE + x) = a;
                        a = 0x00;
                        n = (a) >> 7;
                        z = (a) == 0;
                        RAM(OBJ_TIMER + x) = a;
                    } else {
                        mff_cycles(0xA781, 1);
                    }
                } else {
                    mff_cycles(0xA77D, 1);
                }
                mff_cycles(0xA792, 8);
                c = 0;
                SYNC_OUT;
                mff_rts(); return;
            } else {
                mff_cycles(0xA776, 1);
            }
        } else {
            mff_cycles(0xA770, 1);
        }
    } else {
        mff_cycles(0xA769, 1);
    }
    mff_cycles(0xA794, 7);
    a = RAM(GAME_ACTION);
    m = 0x25;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xA79A, 8);
        c = 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xA798, 1);
    }
    mff_cycles(0xA79C, 6);
    a = RAM(OBJ_TIER + x);
    if (a != 0) { mff_cycles(0xA79F, 1); goto L_A7B5; }
    mff_cycles(0xA7A1, 8);
    a = RAM(OBJ_STATE + x);
    m = 0x85;
    c = a >= m;
    if (a == m) { mff_cycles(0xA7A6, 1); goto L_A7AC; }
    mff_cycles(0xA7A8, 4);
    m = 0x86;
    c = a >= m;
    if (a != m) { mff_cycles(0xA7AA, 1); goto L_A7B5; }
L_A7AC:;
    mff_cycles(0xA7AC, 12);
    a = RAM(OBJ_GROUND_Y + x);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_LAND_Y + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA7BB, 2); call_by_address_tail(0xA7BB, 0); return;
L_A7B5:;
    mff_cycles(0xA7B5, 68);
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

/*  $A736 (bank 0) - lifted by port/tools/lift.py */
void func_A736_b0(void) {
    REGS_IN;
    mff_cycles(0xA736, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0738, 0xA090, 0)) return;
    SYNC_IN;
    mff_cycles(0xA739, 15);
    a = 0x87;
    RAM(OBJ_STATE + x) = a;
    a = RAM(OBJ_TIER + x);
    m = 0x02;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xA745, 7);
        a = 0x20;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_INVULN + x) = a;
    } else {
        mff_cycles(0xA743, 1);
    }
    mff_cycles(0xA74A, 8);
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A7E1 (bank 0) - lifted by port/tools/lift.py */
void func_A7E1_b0(void) {
    REGS_IN;
    mff_cycles(0xA7E1, 4);
    a = prg_read((uint16_t)(0xA80A + y));
    mff_cycles(0xA7E4, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = prg_read((uint16_t)(0xA7FA + y));
    mff_cycles(0xA7EA, 9);
    RAM(OBJ_VX + x) = a;
    a = prg_read((uint16_t)(0xA82A + y));
    mff_cycles(0xA7F0, 9);
    RAM(OBJ_VY_FRAC + x) = a;
    a = prg_read((uint16_t)(0xA81A + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA7F6, 11);
    RAM(OBJ_VY + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A83A (bank 0) - lifted by port/tools/lift.py */
void func_A83A_b0(void) {
    REGS_IN;
    mff_cycles(0xA83A, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x02;
    c = a >= m;
    if (!c) {
        mff_cycles(0xA841, 14);
        a = RAM(OBJ_KIND + x);
        c = 1;
        m = 0x07;
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        y = a;
        a = prg_read((uint16_t)(0xA870 + y));
        mff_cycles(0xA84B, 6);
        m = RAM(OBJ_ANIM + x);
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xA850, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0852, func_F288, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xA853, 4);
            a = prg_read((uint16_t)(0xA872 + y));
            mff_cycles(0xA856, 7);
            RAM(TMP0) = a;
            a = prg_read((uint16_t)(0xA874 + y));
            mff_cycles(0xA85B, 7);
            RAM(TMP1) = a;
            a = prg_read((uint16_t)(0xA914 + y));
            mff_cycles(0xA860, 14);
            y = a;
            a = RAM(TMP0);
            RAM(CHR_BANKS + 3 + y) = a;
            a = prg_read((uint16_t)(0xA916 + y));
            mff_cycles(0xA869, 10);
            y = a;
            a = RAM(TMP1);
            n = (a) >> 7;
            z = (a) == 0;
            RAM(CHR_BANKS + 3 + y) = a;
        } else {
            mff_cycles(0xA84E, 1);
        }
        mff_cycles(0xA86F, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xA83F, 1);
    }
    mff_cycles(0xA876, 10);
    a = 0xEF;
    y = 0x2C;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x087C, func_F449, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA87D, 11);
    a = 0x2E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(CHR_BANKS + 5) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A882 (bank 0) - lifted by port/tools/lift.py */
void func_A882_b0(void) {
    REGS_IN;
    mff_cycles(0xA882, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    y = a;
    a = prg_read((uint16_t)(0xA8B1 + y));
    mff_cycles(0xA88C, 6);
    m = RAM(OBJ_ANIM + x);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xA891, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0893, func_F288, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA894, 4);
        a = prg_read((uint16_t)(0xA8B3 + y));
        mff_cycles(0xA897, 7);
        RAM(TMP0) = a;
        a = prg_read((uint16_t)(0xA8B5 + y));
        mff_cycles(0xA89C, 7);
        RAM(TMP1) = a;
        a = prg_read((uint16_t)(0xA914 + y));
        mff_cycles(0xA8A1, 14);
        y = a;
        a = RAM(TMP0);
        RAM(CHR_BANKS + 3 + y) = a;
        a = prg_read((uint16_t)(0xA916 + y));
        mff_cycles(0xA8AA, 10);
        y = a;
        a = RAM(TMP1);
        n = (a) >> 7;
        z = (a) == 0;
        RAM(CHR_BANKS + 3 + y) = a;
    } else {
        mff_cycles(0xA88F, 1);
    }
    mff_cycles(0xA8B0, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A8B7 (bank 0) - lifted by port/tools/lift.py */
void func_A8B7_b0(void) {
    REGS_IN;
    mff_cycles(0xA8B7, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    y = a;
    a = prg_read((uint16_t)(0xA8E9 + y));
    mff_cycles(0xA8C1, 6);
    m = RAM(OBJ_ANIM + x);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xA8C6, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x08C8, func_F288, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA8C9, 4);
        a = prg_read((uint16_t)(0xA8EB + y));
        mff_cycles(0xA8CC, 7);
        RAM(TMP0) = a;
        a = prg_read((uint16_t)(0xA8ED + y));
        mff_cycles(0xA8D1, 7);
        RAM(TMP1) = a;
        a = prg_read((uint16_t)(0xA914 + y));
        mff_cycles(0xA8D6, 14);
        y = a;
        a = RAM(TMP0);
        RAM(CHR_BANKS + 3 + y) = a;
        a = prg_read((uint16_t)(0xA916 + y));
        mff_cycles(0xA8DF, 17);
        y = a;
        a = RAM(TMP1);
        RAM(CHR_BANKS + 3 + y) = a;
        t = (uint8_t)(RAM(OBJ_ANIM_TIMER + x) + 1);
        RAM(OBJ_ANIM_TIMER + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
    } else {
        mff_cycles(0xA8C4, 1);
    }
    mff_cycles(0xA8E8, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A8EF (bank 0) - lifted by port/tools/lift.py */
void func_A8EF_b0(void) {
    REGS_IN;
    mff_cycles(0xA8EF, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    y = a;
    a = prg_read((uint16_t)(0xA910 + y));
    mff_cycles(0xA8F9, 6);
    m = RAM(OBJ_ANIM + x);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xA8FE, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0900, func_F288, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA901, 4);
        a = prg_read((uint16_t)(0xA912 + y));
        mff_cycles(0xA904, 7);
        RAM(TMP0) = a;
        a = prg_read((uint16_t)(0xA914 + y));
        mff_cycles(0xA909, 10);
        y = a;
        a = RAM(TMP0);
        n = (a) >> 7;
        z = (a) == 0;
        RAM(CHR_BANKS + 3 + y) = a;
    } else {
        mff_cycles(0xA8FC, 2);
    }
    mff_cycles(0xA90F, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A918 (bank 0) - lifted by port/tools/lift.py */
void func_A918_b0(void) {
    REGS_IN;
    mff_cycles(0xA918, 4);
    a = prg_read((uint16_t)(0xA941 + y));
    mff_cycles(0xA91B, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = prg_read((uint16_t)(0xA931 + y));
    mff_cycles(0xA921, 9);
    RAM(OBJ_VX + x) = a;
    a = prg_read((uint16_t)(0xA961 + y));
    mff_cycles(0xA927, 9);
    RAM(OBJ_VY_FRAC + x) = a;
    a = prg_read((uint16_t)(0xA951 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA92D, 11);
    RAM(OBJ_VY + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A971 (bank 0) - lifted by port/tools/lift.py */
void func_A971_b0(void) {
    REGS_IN;
    mff_cycles(0xA971, 4);
    a = prg_read((uint16_t)(0xA99A + y));
    mff_cycles(0xA974, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = prg_read((uint16_t)(0xA98A + y));
    mff_cycles(0xA97A, 9);
    RAM(OBJ_VX + x) = a;
    a = prg_read((uint16_t)(0xA9BA + y));
    mff_cycles(0xA980, 9);
    RAM(OBJ_VY_FRAC + x) = a;
    a = prg_read((uint16_t)(0xA9AA + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA986, 11);
    RAM(OBJ_VY + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A9CA (bank 0) - lifted by port/tools/lift.py */
void func_A9CA_b0(void) {
    REGS_IN;
    mff_cycles(0xA9CA, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x09CC, func_FB5B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA9CD, 6);
    y = a;
    a &= 0x07;
    if (a != 0) { mff_cycles(0xA9D0, 1); goto L_A9EA; }
    mff_cycles(0xA9D2, 4);
    a = y;
    if (a != 0) { mff_cycles(0xA9D3, 1); goto L_A9D9; }
    mff_cycles(0xA9D5, 6);
    y++;
    y++;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0xA9D7, 1); goto L_A9DB; }
L_A9D9:;
    mff_cycles(0xA9D9, 4);
    y--;
    y--;
    n = (y) >> 7;
    z = (y) == 0;
L_A9DB:;
    mff_cycles(0xA9DB, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x09DD, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA9DE, 2);
    if (c) { mff_cycles(0xA9DE, 1); goto L_A9EE; }
    mff_cycles(0xA9E0, 12);
    a = y;
    a ^= 0xFF;
    m = 0x01;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    a &= 0x0F;
    y = a;
    if (y != 0) { mff_cycles(0xA9E8, 1); goto L_A9EE; }
L_A9EA:;
    mff_cycles(0xA9EA, 4);
    a = prg_read((uint16_t)(0xAA23 + y));
    mff_cycles(0xA9ED, 2);
    y = a;
L_A9EE:;
    mff_cycles(0xA9EE, 4);
    a = prg_read((uint16_t)(0xFBF0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA9F1, 11);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A9F5 (bank 0) - lifted by port/tools/lift.py */
void func_A9F5_b0(void) {
    REGS_IN;
    mff_cycles(0xA9F5, 6);
    SYNC_OUT;
    if (!mff_call_at(0x09F7, 0xA918, 0)) return;
    SYNC_IN;
    mff_cycles(0xA9F8, 6);
    a = y;
    a &= 0x01;
    if (a == 0) { mff_cycles(0xA9FB, 2); goto L_AA09; }
    mff_cycles(0xA9FD, 16);
    a = 0x12;
    RAM(OBJ_TIMER2 + x) = a;
    a = 0x07;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_GROUND_Y + x) = a;
    if (!z) { mff_cycles(0xAA07, 1); goto L_AA22; }
L_AA09:;
    mff_cycles(0xAA09, 8);
    a = y;
    t = a;
    t >>= 1;
    a = t;
    t = a;
    c = t & 1;
    t >>= 1;
    if (!c) { mff_cycles(0xAA0C, 1); goto L_AA18; }
    mff_cycles(0xAA0E, 14);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER2 + x) = a;
    RAM(OBJ_GROUND_Y + x) = a;
    if (!z) { mff_cycles(0xAA16, 1); goto L_AA22; }
L_AA18:;
    mff_cycles(0xAA18, 14);
    a = 0x14;
    RAM(OBJ_TIMER2 + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_GROUND_Y + x) = a;
L_AA22:;
    mff_cycles(0xAA22, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AA33 (bank 0) - lifted by port/tools/lift.py */
void func_AA33_b0(void) {
    REGS_IN;
    mff_cycles(0xAA33, 8);
    a = RAM(OBJ_LIFE + x);
    if (a >= 0x30) { mff_cycles(0xAA38, 1); goto L_AA45; }
    mff_cycles(0xAA3A, 8);
    a = RAM(OBJ_TIER + x);
    if (a != 0x01) { mff_cycles(0xAA3F, 1); goto L_AA4A; }
    mff_cycles(0xAA41, 4);
    a = 0x02;
    if (a != 0) { mff_cycles(0xAA43, 1); goto L_AA47; }
L_AA45:;
    mff_cycles(0xAA45, 2);
    a = 0x01;
L_AA47:;
    mff_cycles(0xAA47, 5);
    RAM(OBJ_TIER + x) = a;
L_AA4A:;
    mff_cycles(0xAA4A, 20);
    a = 0x08;
    RAM(OBJ_KIND + x) = a;
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_INVULN + x);
    if (a != 0) {
        mff_cycles(0xAA59, 7);
        t = (uint8_t)(RAM(OBJ_INVULN + x) - 1);
        RAM(OBJ_INVULN + x) = t;
    } else {
        mff_cycles(0xAA57, 1);
    }
    mff_cycles(0xAA5C, 8);
    a = RAM(OBJ_ID + 13);
    m = 0x6E;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xAA61, 1); goto L_AA69; }
    mff_cycles(0xAA63, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0A65, 0xA6D6, 0)) return;
    SYNC_IN;
    mff_cycles(0xAA66, 2);
    if (!c) { mff_cycles(0xAA66, 1); goto L_AA6F; }
    mff_cycles(0xAA68, 6);
    SYNC_OUT;
    mff_rts(); return;
L_AA69:;
    mff_cycles(0xAA69, 13);
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_INVULN + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_AA6F:;
    mff_cycles(0xAA6F, 7);
    a = RAM(GAME_ACTION);
    m = 0x1F;
    c = a >= m;
    if (a != m) { mff_cycles(0xAA73, 1); goto L_AAB3; }
    mff_cycles(0xAA75, 8);
    a = RAM(OBJ_TIER + x);
    if (a != 0x03) { mff_cycles(0xAA7A, 1); goto L_AA9B; }
    mff_cycles(0xAA7C, 8);
    a = RAM(OBJ_STATE + x);
    m = 0x83;
    c = a >= m;
    if (a != m) { mff_cycles(0xAA81, 1); goto L_AA8A; }
    mff_cycles(0xAA83, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0xAA88, 1); goto L_AAB3; }
L_AA8A:;
    mff_cycles(0xAA8A, 23);
    a = 0x83;
    RAM(OBJ_STATE + x) = a;
    a = 0x02;
    RAM(OBJ_TIMER + x) = a;
    a = 0x20;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    if (!z) { mff_cycles(0xAA99, 1); goto L_AAB0; }
L_AA9B:;
    mff_cycles(0xAA9B, 8);
    a = RAM(OBJ_STATE + x);
    m = 0x86;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xAAA0, 1); goto L_AAB0; }
    mff_cycles(0xAAA2, 4);
    m = 0x85;
    c = a >= m;
    if (a == m) { mff_cycles(0xAAA4, 1); goto L_AAB3; }
    mff_cycles(0xAAA6, 14);
    a = 0x85;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
L_AAB0:;
    mff_cycles(0xAAB0, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA83A, 2); call_by_address_tail(0xA83A, 0); return;
L_AAB3:;
    mff_cycles(0xAAB3, 12);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    y = a;
    a = prg_read((uint16_t)(0xAAC6 + y));
    mff_cycles(0xAABC, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0xAACF + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xAAC1, 8);
    RAM(TMP1) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $AAD8 (bank 0) - lifted by port/tools/lift.py */
void func_AAD8_b0(void) {
    REGS_IN;
    mff_cycles(0xAAD8, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x03;
    c = a >= m;
    if (a == m) { mff_cycles(0xAADD, 1); goto L_AAF4; }
    mff_cycles(0xAADF, 4);
    m = 0x02;
    c = a >= m;
    if (a != m) { mff_cycles(0xAAE1, 1); goto L_AAFF; }
    mff_cycles(0xAAE3, 25);
    a = 0x7C;
    RAM(OBJ_INVULN + x) = a;
    a = 0x74;
    RAM(OBJ_AI_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_TIER + x) + 1);
    RAM(OBJ_TIER + x) = t;
    a = 0x03;
    if (a != 0) { mff_cycles(0xAAF2, 1); goto L_AAF6; }
L_AAF4:;
    mff_cycles(0xAAF4, 2);
    a = 0x00;
L_AAF6:;
    mff_cycles(0xAAF6, 18);
    RAM(OBJ_TIMER + x) = a;
    a = 0x83;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_AAFF:;
    mff_cycles(0xAAFF, 17);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA83A, 2); call_by_address_tail(0xA83A, 0); return;
}

/*  $AB0A (bank 0) - lifted by port/tools/lift.py */
void func_AB0A_b0(void) {
    REGS_IN;
    mff_cycles(0xAB0A, 10);
    a = 0x24;
    y = 0x09;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0B10, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAB11, 2);
    if (c) {
        mff_cycles(0xAB13, 27);
        a = 0x82;
        RAM(OBJ_STATE + x) = a;
        a = 0x00;
        RAM(OBJ_TIMER + x) = a;
        a = 0x01;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_TIMER2 + x) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xAB11, 1);
    }
    mff_cycles(0xAB23, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0B25, 0xA882, 0)) return;
    SYNC_IN;
    mff_cycles(0xAB26, 6);
    a = RAM(OBJ_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0xAB2B, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xAB93, 2); call_by_address_tail(0xAB93, 0); return;
    } else {
        mff_cycles(0xAB29, 1);
    }
    mff_cycles(0xAB2E, 10);
    a = 0x30;
    y = 0x10;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0B34, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAB35, 2);
    if (!c) { mff_cycles(0xAB35, 1); goto L_AB5A; }
    mff_cycles(0xAB37, 7);
    a = RAM(GAME_ACTION);
    m = 0x06;
    c = a >= m;
    if (a == m) { mff_cycles(0xAB3B, 1); goto L_AB46; }
    mff_cycles(0xAB3D, 4);
    m = 0x1F;
    c = a >= m;
    if (a == m) { mff_cycles(0xAB3F, 1); goto L_AB46; }
    mff_cycles(0xAB41, 7);
    a = RAM(RNG);
    t = a;
    c = t & 1;
    t >>= 1;
    if (c) { mff_cycles(0xAB44, 1); goto L_AB51; }
L_AB46:;
    mff_cycles(0xAB46, 20);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x85;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_AB51:;
    mff_cycles(0xAB51, 20);
    a = 0x00;
    RAM(OBJ_AI_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_AB5A:;
    mff_cycles(0xAB5A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B5C, func_FBD9, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAB5D, 4);
    m = 0x08;
    c = a >= m;
    if (!c) { mff_cycles(0xAB5F, 1); goto L_AB7A; }
    mff_cycles(0xAB61, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0B65, 0xA7E1, 0)) return;
    SYNC_IN;
    mff_cycles(0xAB66, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B68, func_FBD9, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAB69, 2);
    if (c) { mff_cycles(0xAB69, 1); goto L_AB6F; }
    mff_cycles(0xAB6B, 4);
    a = 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xAB6D, 1); goto L_AB71; }
L_AB6F:;
    mff_cycles(0xAB6F, 2);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
L_AB71:;
    mff_cycles(0xAB71, 11);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0B76, 0xA031, 0)) return;
    SYNC_IN;
    mff_cycles(0xAB77, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA090, 2); call_by_address_tail(0xA090, 0); return;
L_AB7A:;
    mff_cycles(0xAB7A, 8);
    y = 0x04;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0B7E, 0xA7E1, 0)) return;
    SYNC_IN;
    mff_cycles(0xAB7F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B81, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAB82, 2);
    if (c) { mff_cycles(0xAB82, 1); goto L_AB88; }
    mff_cycles(0xAB84, 4);
    a = 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xAB86, 1); goto L_AB8A; }
L_AB88:;
    mff_cycles(0xAB88, 2);
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
L_AB8A:;
    mff_cycles(0xAB8A, 11);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0B8F, 0xA031, 0)) return;
    SYNC_IN;
    mff_cycles(0xAB90, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA090, 2); call_by_address_tail(0xA090, 0); return;
}

/*  $AB93 (bank 0) - lifted by port/tools/lift.py */
void func_AB93_b0(void) {
    REGS_IN;
    mff_cycles(0xAB93, 6);
    a = RAM(OBJ_AI_TIMER + x);
    if (a != 0) {
        mff_cycles(0xAB98, 10);
        t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
        RAM(OBJ_AI_TIMER + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xABB0, 2); call_by_address_tail(0xABB0, 0); return;
    } else {
        mff_cycles(0xAB96, 1);
    }
    mff_cycles(0xAB9E, 13);
    a = 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0BA5, func_FB72, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xABA6, 8);
    y = a;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0BA9, 0xA7E1, 0)) return;
    SYNC_IN;
    mff_cycles(0xABAA, 4);
    a = prg_read((uint16_t)(0xFBF0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xABAD, 11);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0BB2, 0xA031, 0)) return;
    SYNC_IN;
    mff_cycles(0xABB3, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA090, 2); call_by_address_tail(0xA090, 0); return;
}

/*  $ABB0 (bank 0) - lifted by port/tools/lift.py */
void func_ABB0_b0(void) {
    REGS_IN;
    mff_cycles(0xABB0, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0BB2, 0xA031, 0)) return;
    SYNC_IN;
    mff_cycles(0xABB3, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA090, 2); call_by_address_tail(0xA090, 0); return;
}

/*  $ABB6 (bank 0) - lifted by port/tools/lift.py */
void func_ABB6_b0(void) {
    REGS_IN;
    mff_cycles(0xABB6, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0BB8, 0xA882, 0)) return;
    SYNC_IN;
    mff_cycles(0xABB9, 7);
    a = RAM(GAME_ACTION);
    m = 0x01;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xABBF, 10);
        a = 0x20;
        y = 0x09;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0BC5, func_FB3E, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xABC6, 2);
        if (c) {
            mff_cycles(0xABC8, 27);
            a = 0x82;
            RAM(OBJ_STATE + x) = a;
            a = 0x00;
            RAM(OBJ_TIMER + x) = a;
            a = 0x01;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_TIMER2 + x) = a;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xABC6, 1);
        }
    } else {
        mff_cycles(0xABBD, 1);
    }
    mff_cycles(0xABD8, 6);
    a = RAM(OBJ_TIMER + x);
    if (a != 0) { mff_cycles(0xABDB, 2); goto L_AC28; }
    mff_cycles(0xABDD, 9);
    a = RAM(RNG);
    a &= 0x0F;
    m = 0x08;
    c = a >= m;
    if (c) { mff_cycles(0xABE3, 1); goto L_ABEC; }
    mff_cycles(0xABE5, 9);
    a = 0x03;
    RAM(OBJ_DIR + x) = a;
    if (a != 0) { mff_cycles(0xABEA, 2); goto L_AC01; }
L_ABEC:;
    mff_cycles(0xABEC, 4);
    m = 0x0D;
    c = a >= m;
    if (c) { mff_cycles(0xABEE, 1); goto L_ABFC; }
    mff_cycles(0xABF0, 8);
    y = 0x04;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0BF4, 0xA7E1, 0)) return;
    SYNC_IN;
    mff_cycles(0xABF5, 9);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DIR + x) = a;
    if (!z) { mff_cycles(0xABFA, 2); goto L_AC06; }
L_ABFC:;
    mff_cycles(0xABFC, 7);
    a = 0x05;
    RAM(OBJ_DIR + x) = a;
L_AC01:;
    mff_cycles(0xAC01, 8);
    y = 0x05;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0C05, 0xA7E1, 0)) return;
    SYNC_IN;
L_AC06:;
    mff_cycles(0xAC06, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C08, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC09, 2);
    if (c) {
        mff_cycles(0xAC0B, 18);
        a = RAM(OBJ_DIR + x);
        a ^= 0x0F;
        RAM(OBJ_DIR + x) = a;
        t = (uint8_t)(RAM(OBJ_DIR + x) + 1);
        RAM(OBJ_DIR + x) = t;
    } else {
        mff_cycles(0xAC09, 1);
    }
    mff_cycles(0xAC16, 8);
    y = RAM(OBJ_DIR + x);
    a = prg_read((uint16_t)(0xFBF0 + y));
    mff_cycles(0xAC1C, 25);
    RAM(OBJ_DIR + x) = a;
    a = 0x64;
    RAM(OBJ_AI_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_AC28:;
    mff_cycles(0xAC28, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) {
        mff_cycles(0xAC2D, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0C2F, 0xA031, 0)) return;
        SYNC_IN;
        mff_cycles(0xAC30, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0C32, 0xA090, 0)) return;
        SYNC_IN;
        mff_cycles(0xAC33, 5);
        a = RAM(TMP15);
        if (a == 0) {
            mff_cycles(0xAC37, 8);
            a = RAM(OBJ_X + x);
            m = 0xF1;
            c = a >= m;
            if (!c) {
                mff_cycles(0xAC3E, 4);
                m = 0x18;
                c = a >= m;
                n = ((uint8_t)(a - m)) >> 7;
                z = ((uint8_t)(a - m)) == 0;
                if (c) {
                    mff_cycles(0xAC42, 6);
                    SYNC_OUT;
                    mff_rts(); return;
                } else {
                    mff_cycles(0xAC40, 1);
                }
            } else {
                mff_cycles(0xAC3C, 1);
            }
        } else {
            mff_cycles(0xAC35, 1);
        }
    } else {
        mff_cycles(0xAC2B, 1);
    }
    mff_cycles(0xAC43, 20);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x86;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AC4E (bank 0) - lifted by port/tools/lift.py */
void func_AC4E_b0(void) {
    REGS_IN;
    mff_cycles(0xAC4E, 6);
    a = RAM(OBJ_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xAC51, 1); goto L_AC61; }
    mff_cycles(0xAC53, 4);
    if (a == 0x01) { mff_cycles(0xAC55, 1); goto L_AC87; }
    mff_cycles(0xAC57, 4);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xAC5B, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xAD50, 2); call_by_address_tail(0xAD50, 0); return;
    } else {
        mff_cycles(0xAC59, 1);
    }
    mff_cycles(0xAC5E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAD5E, 2); call_by_address_tail(0xAD5E, 0); return;
L_AC61:;
    mff_cycles(0xAC61, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0C63, 0xA9CA, 0)) return;
    SYNC_IN;
    mff_cycles(0xAC64, 13);
    a = y;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER3 + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0C6A, 0xA971, 0)) return;
    SYNC_IN;
    mff_cycles(0xAC6B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C6D, func_F32E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC6E, 31);
    a = 0x00;
    RAM(OBJ_AI_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    a = 0x17;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = 0xF3;
    y = 0x2C;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0C81, func_F449, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC82, 11);
    a = 0x2E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(CHR_BANKS + 5) = a;
    SYNC_OUT;
    mff_rts(); return;
L_AC87:;
    mff_cycles(0xAC87, 25);
    a = 0x90;
    RAM(OBJ_ATTACK + x) = a;
    y = RAM(OBJ_AI_TIMER + x);
    a = RAM(OBJ_DIR + x);
    a ^= RAM(OBJ_AI_TIMER + x);
    t = a;
    c = t & 1;
    t >>= 1;
    if (c) {
        mff_cycles(0xAC9A, 10);
        a = RAM(OBJ_X + x);
        c = 0;
        m = prg_read((uint16_t)(0xAD80 + y));
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        mff_cycles(0xACA1, 19);
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
        nes_cpu_instruction_boundary(0xACC1, 2); call_by_address_tail(0xACC1, 0); return;
    } else {
        mff_cycles(0xAC98, 1);
    }
    mff_cycles(0xACAF, 10);
    a = RAM(OBJ_X + x);
    c = 1;
    m = prg_read((uint16_t)(0xAD80 + y));
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    mff_cycles(0xACB6, 26);
    RAM(OBJ_X + x) = a;
    a = RAM(OBJ_X_HI + x);
    m = 0x00;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_X_HI + x) = a;
    a = RAM(OBJ_TIMER3 + x);
    a &= 0x07;
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0xACC8, 2); goto L_AD0B; }
    mff_cycles(0xACCA, 8);
    a = RAM(OBJ_TIMER3 + x);
    t = a;
    c = t & 1;
    t >>= 1;
    if (c) { mff_cycles(0xACCE, 1); goto L_ACD5; }
    mff_cycles(0xACD0, 4);
    a = prg_read((uint16_t)(0xAD84 + y));
    mff_cycles(0xACD3, 2);
    if (a != 0) { mff_cycles(0xACD3, 1); goto L_ACD8; }
L_ACD5:;
    mff_cycles(0xACD5, 4);
    a = prg_read((uint16_t)(0xAD88 + y));
L_ACD8:;
    mff_cycles(0xACD8, 21);
    RAM(TMP1) = a;
    a = RAM(OBJ_DIR + x);
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    a ^= RAM(OBJ_AI_TIMER + x);
    t = a;
    c = t & 1;
    t >>= 1;
    if (c) {
        mff_cycles(0xACE6, 28);
        a = RAM(OBJ_Y + x);
        c = 1;
        m = RAM(TMP1);
        w = a - m - !c;
        c = w >= 0;
        a = (uint8_t)w;
        RAM(OBJ_Y + x) = a;
        a = RAM(OBJ_Y_HI + x);
        m = 0x00;
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_Y_HI + x) = a;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xAD0B, 2); call_by_address_tail(0xAD0B, 0); return;
    } else {
        mff_cycles(0xACE4, 1);
    }
    mff_cycles(0xACFA, 25);
    a = RAM(OBJ_Y + x);
    c = 0;
    m = RAM(TMP1);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_Y + x) = a;
    a = RAM(OBJ_Y_HI + x);
    m = 0x00;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_Y_HI + x) = a;
L_AD0B:;
    mff_cycles(0xAD0B, 24);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) + 1);
    RAM(OBJ_AI_TIMER + x) = t;
    a = RAM(OBJ_AI_TIMER + x);
    a &= 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0D18, 0xA031, 0)) return;
    SYNC_IN;
    mff_cycles(0xAD19, 5);
    a = RAM(TMP15);
    if (a != 0) { mff_cycles(0xAD1B, 1); goto L_AD42; }
    mff_cycles(0xAD1D, 10);
    a = RAM(OBJ_X_HI + x);
    m = RAM(OBJ_X_HI);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xAD23, 1); goto L_AD41; }
    mff_cycles(0xAD25, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    if (a != 0) { mff_cycles(0xAD2A, 1); goto L_AD33; }
    mff_cycles(0xAD2C, 8);
    a = RAM(OBJ_X + x);
    m = 0x18;
    c = a >= m;
    if (!c) { mff_cycles(0xAD31, 1); goto L_AD42; }
L_AD33:;
    mff_cycles(0xAD33, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xAD38, 1); goto L_AD41; }
    mff_cycles(0xAD3A, 8);
    a = RAM(OBJ_X + x);
    m = 0xF1;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xAD3F, 1); goto L_AD42; }
L_AD41:;
    mff_cycles(0xAD41, 6);
    SYNC_OUT;
    mff_rts(); return;
L_AD42:;
    mff_cycles(0xAD42, 27);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = 0x48;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $ACC1 (bank 0) - lifted by port/tools/lift.py */
void func_ACC1_b0(void) {
    REGS_IN;
    mff_cycles(0xACC1, 10);
    a = RAM(OBJ_TIMER3 + x);
    a &= 0x07;
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0xACC8, 2); goto L_AD0B; }
    mff_cycles(0xACCA, 8);
    a = RAM(OBJ_TIMER3 + x);
    t = a;
    c = t & 1;
    t >>= 1;
    if (c) { mff_cycles(0xACCE, 1); goto L_ACD5; }
    mff_cycles(0xACD0, 4);
    a = prg_read((uint16_t)(0xAD84 + y));
    mff_cycles(0xACD3, 2);
    if (a != 0) { mff_cycles(0xACD3, 1); goto L_ACD8; }
L_ACD5:;
    mff_cycles(0xACD5, 4);
    a = prg_read((uint16_t)(0xAD88 + y));
L_ACD8:;
    mff_cycles(0xACD8, 21);
    RAM(TMP1) = a;
    a = RAM(OBJ_DIR + x);
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    a ^= RAM(OBJ_AI_TIMER + x);
    t = a;
    c = t & 1;
    t >>= 1;
    if (c) {
        mff_cycles(0xACE6, 28);
        a = RAM(OBJ_Y + x);
        c = 1;
        m = RAM(TMP1);
        w = a - m - !c;
        c = w >= 0;
        a = (uint8_t)w;
        RAM(OBJ_Y + x) = a;
        a = RAM(OBJ_Y_HI + x);
        m = 0x00;
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_Y_HI + x) = a;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xAD0B, 2); call_by_address_tail(0xAD0B, 0); return;
    } else {
        mff_cycles(0xACE4, 1);
    }
    mff_cycles(0xACFA, 25);
    a = RAM(OBJ_Y + x);
    c = 0;
    m = RAM(TMP1);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_Y + x) = a;
    a = RAM(OBJ_Y_HI + x);
    m = 0x00;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_Y_HI + x) = a;
L_AD0B:;
    mff_cycles(0xAD0B, 24);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) + 1);
    RAM(OBJ_AI_TIMER + x) = t;
    a = RAM(OBJ_AI_TIMER + x);
    a &= 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0D18, 0xA031, 0)) return;
    SYNC_IN;
    mff_cycles(0xAD19, 5);
    a = RAM(TMP15);
    if (a != 0) { mff_cycles(0xAD1B, 1); goto L_AD42; }
    mff_cycles(0xAD1D, 10);
    a = RAM(OBJ_X_HI + x);
    m = RAM(OBJ_X_HI);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xAD23, 1); goto L_AD41; }
    mff_cycles(0xAD25, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    if (a != 0) { mff_cycles(0xAD2A, 1); goto L_AD33; }
    mff_cycles(0xAD2C, 8);
    a = RAM(OBJ_X + x);
    m = 0x18;
    c = a >= m;
    if (!c) { mff_cycles(0xAD31, 1); goto L_AD42; }
L_AD33:;
    mff_cycles(0xAD33, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xAD38, 1); goto L_AD41; }
    mff_cycles(0xAD3A, 8);
    a = RAM(OBJ_X + x);
    m = 0xF1;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xAD3F, 1); goto L_AD42; }
L_AD41:;
    mff_cycles(0xAD41, 6);
    SYNC_OUT;
    mff_rts(); return;
L_AD42:;
    mff_cycles(0xAD42, 27);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = 0x48;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AD0B (bank 0) - lifted by port/tools/lift.py */
void func_AD0B_b0(void) {
    REGS_IN;
    mff_cycles(0xAD0B, 24);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) + 1);
    RAM(OBJ_AI_TIMER + x) = t;
    a = RAM(OBJ_AI_TIMER + x);
    a &= 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0D18, 0xA031, 0)) return;
    SYNC_IN;
    mff_cycles(0xAD19, 5);
    a = RAM(TMP15);
    if (a != 0) { mff_cycles(0xAD1B, 1); goto L_AD42; }
    mff_cycles(0xAD1D, 10);
    a = RAM(OBJ_X_HI + x);
    m = RAM(OBJ_X_HI);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xAD23, 1); goto L_AD41; }
    mff_cycles(0xAD25, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    if (a != 0) { mff_cycles(0xAD2A, 1); goto L_AD33; }
    mff_cycles(0xAD2C, 8);
    a = RAM(OBJ_X + x);
    m = 0x18;
    c = a >= m;
    if (!c) { mff_cycles(0xAD31, 1); goto L_AD42; }
L_AD33:;
    mff_cycles(0xAD33, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xAD38, 1); goto L_AD41; }
    mff_cycles(0xAD3A, 8);
    a = RAM(OBJ_X + x);
    m = 0xF1;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xAD3F, 1); goto L_AD42; }
L_AD41:;
    mff_cycles(0xAD41, 6);
    SYNC_OUT;
    mff_rts(); return;
L_AD42:;
    mff_cycles(0xAD42, 27);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = 0x48;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AD50 (bank 0) - lifted by port/tools/lift.py */
void func_AD50_b0(void) {
    REGS_IN;
    mff_cycles(0xAD50, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0D52, 0xA83A, 0)) return;
    SYNC_IN;
    mff_cycles(0xAD53, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (z) {
        mff_cycles(0xAD58, 7);
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_TIMER + x) = a;
    } else {
        mff_cycles(0xAD56, 1);
    }
    mff_cycles(0xAD5D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AD5E (bank 0) - lifted by port/tools/lift.py */
void func_AD5E_b0(void) {
    REGS_IN;
    mff_cycles(0xAD5E, 8);
    a = RAM(OBJ_AI_TIMER + x);
    m = 0x40;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xAD63, 1); goto L_AD6B; }
    mff_cycles(0xAD65, 6);
    a = 0xFA;
    y = 0x2E;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0xAD69, 1); goto L_AD72; }
L_AD6B:;
    mff_cycles(0xAD6B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D6D, func_F099, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAD6E, 4);
    a = 0xF1;
    y = 0x31;
    n = (y) >> 7;
    z = (y) == 0;
L_AD72:;
    mff_cycles(0xAD72, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D74, func_F449, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAD75, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (z) {
        mff_cycles(0xAD7A, 7);
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_TIMER + x) = a;
    } else {
        mff_cycles(0xAD78, 1);
    }
    mff_cycles(0xAD7F, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AD8C (bank 0) - lifted by port/tools/lift.py */
void func_AD8C_b0(void) {
    REGS_IN;
    mff_cycles(0xAD8C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D8E, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAD8F, 9);
    t = (uint8_t)(RAM(OBJ_HIT_TIMER + x) - 1);
    RAM(OBJ_HIT_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) { mff_cycles(0xAD92, 1); goto L_ADB9; }
    mff_cycles(0xAD94, 8);
    a = RAM(OBJ_TIER + x);
    if (a == 0x01) { mff_cycles(0xAD99, 1); goto L_ADA3; }
    mff_cycles(0xAD9B, 4);
    m = 0x02;
    c = a >= m;
    if (a == m) { mff_cycles(0xAD9D, 1); goto L_ADAB; }
    mff_cycles(0xAD9F, 4);
    m = 0x03;
    c = a >= m;
    if (a == m) { mff_cycles(0xADA1, 1); goto L_ADAF; }
L_ADA3:;
    mff_cycles(0xADA3, 4);
    m = 0x02;
    c = a >= m;
    if (c) { mff_cycles(0xADA5, 1); goto L_ADAF; }
    mff_cycles(0xADA7, 4);
    a = 0x84;
    if (a != 0) { mff_cycles(0xADA9, 1); goto L_ADB1; }
L_ADAB:;
    mff_cycles(0xADAB, 4);
    a = 0x80;
    if (a != 0) { mff_cycles(0xADAD, 1); goto L_ADB1; }
L_ADAF:;
    mff_cycles(0xADAF, 2);
    a = 0x83;
L_ADB1:;
    mff_cycles(0xADB1, 12);
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
L_ADB9:;
    mff_cycles(0xADB9, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $ADBA (bank 0) - lifted by port/tools/lift.py */
void func_ADBA_b0(void) {
    REGS_IN;
    mff_cycles(0xADBA, 6);
    a = RAM(OBJ_TIMER + x);
    if (a == 0) {
        mff_cycles(0xADBF, 17);
        a = 0x18;
        RAM(OBJ_AI_TIMER + x) = a;
        t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        RAM(OBJ_TIMER + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA83A, 2); call_by_address_tail(0xA83A, 0); return;
    } else {
        mff_cycles(0xADBD, 1);
    }
    mff_cycles(0xADCA, 6);
    a = RAM(OBJ_AI_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0xADCF, 13);
        t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
        RAM(OBJ_AI_TIMER + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xADCD, 1);
    }
    mff_cycles(0xADD3, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0DD5, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xADD6, 4);
    if (a >= 0x28) { mff_cycles(0xADD8, 1); goto L_ADEE; }
    mff_cycles(0xADDA, 4);
    a = 0x00;
    if (a == 0) { mff_cycles(0xADDC, 2); goto L_AE00; }
    mff_cycles(0xADDE, 4);
    if (a >= 0x3C) { mff_cycles(0xADE0, 1); goto L_ADE6; }
    mff_cycles(0xADE2, 4);
    a = 0x0A;
    if (a != 0) { mff_cycles(0xADE4, 2); goto L_AE00; }
L_ADE6:;
    mff_cycles(0xADE6, 4);
    if (a >= 0x50) { mff_cycles(0xADE8, 1); goto L_ADEE; }
    mff_cycles(0xADEA, 4);
    a = 0x0B;
    if (a != 0) { mff_cycles(0xADEC, 2); goto L_AE00; }
L_ADEE:;
    mff_cycles(0xADEE, 4);
    if (a >= 0x64) { mff_cycles(0xADF0, 1); goto L_ADF6; }
    mff_cycles(0xADF2, 4);
    a = 0x0C;
    if (a != 0) { mff_cycles(0xADF4, 2); goto L_AE00; }
L_ADF6:;
    mff_cycles(0xADF6, 4);
    if (a >= 0x78) { mff_cycles(0xADF8, 1); goto L_ADFE; }
    mff_cycles(0xADFA, 4);
    a = 0x0D;
    if (a != 0) { mff_cycles(0xADFC, 2); goto L_AE00; }
L_ADFE:;
    mff_cycles(0xADFE, 2);
    a = 0x0E;
L_AE00:;
    mff_cycles(0xAE00, 13);
    RAM(TMP0) = a;
    a = RAM(RNG);
    a &= 0x0F;
    m = RAM(TMP0);
    c = a >= m;
    if (c) { mff_cycles(0xAE08, 1); goto L_AE15; }
    mff_cycles(0xAE0A, 2);
    a = 0x84;
L_AE0C:;
    mff_cycles(0xAE0C, 18);
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_AE15:;
    mff_cycles(0xAE15, 4);
    a = 0x81;
    if (a != 0) { mff_cycles(0xAE0C, 1); goto L_AE0C; }
    mff_cycles(0xAE19, 6);
    a = RAM(OBJ_TIMER + x);
    if (a == 0) {
        mff_cycles(0xAE1E, 26);
        a = 0x20;
        RAM(PLAYER_INVULN) = a;
        t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        RAM(OBJ_TIMER + x) = t;
        a = 0x27;
        RAM(OBJ_ATTACK_DAMAGE + x) = a;
        a = 0xF6;
        y = 0x2F;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF44F, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F44F(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xAE1C, 1);
    }
    mff_cycles(0xAE31, 15);
    a = 0x98;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x09;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0xAE3D, 19);
        a = 0x00;
        RAM(OBJ_TIMER + x) = a;
        RAM(OBJ_HIT_PLAYER + x) = a;
        a = 0x81;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_STATE + x) = a;
    } else {
        mff_cycles(0xAE3B, 1);
    }
    mff_cycles(0xAE4A, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AE19 (bank 0) - lifted by port/tools/lift.py */
void func_AE19_b0(void) {
    REGS_IN;
    mff_cycles(0xAE19, 6);
    a = RAM(OBJ_TIMER + x);
    if (a == 0) {
        mff_cycles(0xAE1E, 26);
        a = 0x20;
        RAM(PLAYER_INVULN) = a;
        t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        RAM(OBJ_TIMER + x) = t;
        a = 0x27;
        RAM(OBJ_ATTACK_DAMAGE + x) = a;
        a = 0xF6;
        y = 0x2F;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF44F, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F44F(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xAE1C, 1);
    }
    mff_cycles(0xAE31, 15);
    a = 0x98;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x09;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0xAE3D, 19);
        a = 0x00;
        RAM(OBJ_TIMER + x) = a;
        RAM(OBJ_HIT_PLAYER + x) = a;
        a = 0x81;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_STATE + x) = a;
    } else {
        mff_cycles(0xAE3B, 1);
    }
    mff_cycles(0xAE4A, 6);
    SYNC_OUT;
    mff_rts(); return;
}
