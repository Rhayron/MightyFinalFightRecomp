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
