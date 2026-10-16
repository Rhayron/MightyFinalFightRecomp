/*
 * lifted_d.c - fixed-bank routines $D000-$DFFF, lifted by
 * port/tools/lift_bank.py from the pure scaffold: exact, still to be
 * tidied and moved into their systems' files.
 */
#include "mff.h"
#include "state.h"
#include "coroutine.h"

void func_E03B(void);
void func_E12F(void);
void func_E139(void);
void func_E143(void);
void func_E16B(void);
void func_E521(void);
void func_F003(void);
void func_F03F(void);
void func_F048(void);
void func_F054(void);
void func_F090(void);
void func_F099(void);
void func_F0A2(void);
void func_F0F7(void);
void func_F119(void);
void func_F183(void);
void func_F1A1(void);
void func_F26E(void);
void func_F288(void);
void func_F294(void);
void func_F34A(void);
void func_F34C(void);
void func_F428(void);
void func_F430(void);
void func_F436(void);
void func_F43B(void);
void func_F440(void);
void func_FF2A(void);
void func_FF31(void);
void func_FF4D(void);

/* player_state_grab $D7A3 - lifted by port/tools/lift.py */
void func_D7A3(void) {
    REGS_IN;
    mff_cycles(0xD7A3, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xD7B2 + y));
    mff_cycles(0xD7A8, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0xD7B5 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xD7AD, 8);
    RAM(TMP1) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $D7B8 - lifted by port/tools/lift.py */
void func_D7B8(void) {
    REGS_IN;
    mff_cycles(0xD7B8, 7);
    a = RAM(JOY_PRESSED);
    m = 0x40;
    c = a >= m;
    if (a != m) { mff_cycles(0xD7BC, 2); goto L_D82E; }
    mff_cycles(0xD7BE, 10);
    y = RAM(GRAB_SLOT);
    a = RAM(JOY_HELD);
    a &= 0x0F;
    if (a == 0) { mff_cycles(0xD7C4, 2); goto L_D829; }
    mff_cycles(0xD7C6, 5);
    m = RAM(PLAYER_DIR);
    c = a >= m;
    if (a == m) { mff_cycles(0xD7C8, 1); goto L_D7D4; }
    mff_cycles(0xD7CA, 13);
    a = RAM(OBJ_DIR + y);
    a ^= 0x03;
    RAM(OBJ_DIR + y) = a;
    if (a != 0) { mff_cycles(0xD7D2, 1); goto L_D7EB; }
L_D7D4:;
    mff_cycles(0xD7D4, 14);
    a = 0xFF;
    RAM(OBJ_THROW_FLAG + y) = a;
    a = RAM(PLAYER_DIR);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xD7DD, 1); goto L_D7E6; }
    mff_cycles(0xD7DF, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x17E1, func_F048, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD7E2, 4);
    a = 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xD7E4, 1); goto L_D7EB; }
L_D7E6:;
    mff_cycles(0xD7E6, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x17E8, func_F03F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD7E9, 2);
    a = 0x01;
L_D7EB:;
    mff_cycles(0xD7EB, 11);
    RAM(PLAYER_DIR) = a;
    a = 0x34;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x17F1, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD7F2, 30);
    a = 0x80;
    RAM(PLAYER_NO_HIT) = a;
    y = RAM(GRAB_SLOT);
    a = 0x04;
    RAM(OBJ_ID + y) = a;
    a = 0x80;
    RAM(OBJ_STATE + y) = a;
    a = RAM(OBJ_DIR + y);
    a &= 0x01;
    if (a == 0) { mff_cycles(0xD807, 1); goto L_D813; }
    mff_cycles(0xD809, 13);
    a = RAM(OBJ_FLAGS + y);
    a &= 0xBF;
    RAM(OBJ_FLAGS + y) = a;
    if (a != 0) { mff_cycles(0xD811, 1); goto L_D81B; }
L_D813:;
    mff_cycles(0xD813, 11);
    a = RAM(OBJ_FLAGS + y);
    a |= 0x40;
    RAM(OBJ_FLAGS + y) = a;
L_D81B:;
    mff_cycles(0xD81B, 18);
    a = 0x19;
    RAM(ACTION_TIMER) = a;
    a = 0x0B;
    RAM(GAME_ACTION) = a;
    a = 0x33;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1827, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD828, 6);
    SYNC_OUT;
    mff_rts(); return;
L_D829:;
    mff_cycles(0xD829, 11);
    a = 0x21;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
L_D82E:;
    mff_cycles(0xD82E, 7);
    a = RAM(JOY_PRESSED);
    m = 0x80;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xD834, 5);
        a = RAM(GRAB_TIMER);
        if (!(a & 0x80)) {
            mff_cycles(0xD838, 11);
            t = (uint8_t)(RAM(GRAB_TIMER) - 1);
            RAM(GRAB_TIMER) = t;
            n = (t) >> 7;
            z = (t) == 0;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xD836, 1);
        }
        mff_cycles(0xD83B, 27);
        a = 0x00;
        RAM(GAME_ACTION) = a;
        RAM(GUY_JUMP_WINDOW) = a;
        RAM(GRAB_HIT_COUNT) = a;
        a = 0xFF;
        y = RAM(GRAB_SLOT);
        n = (y) >> 7;
        z = (y) == 0;
        RAM(OBJ_HELD + y) = a;
        SYNC_OUT;
        if (!mff_call_at(0x184C, 0xCEAA, -1)) return;
        SYNC_IN;
        mff_cycles(0xD84D, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xD832, 1);
    }
    mff_cycles(0xD84E, 19);
    a = 0x38;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x04;
    RAM(OBJ_VY) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1E9 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xD85D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x185F, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD860, 4);
    a = prg_read((uint16_t)(0xE22E + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xD863, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1865, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD866, 41);
    a = 0x00;
    RAM(GRAB_TIMER) = a;
    RAM(GRAB_HIT_COUNT) = a;
    a = 0xFF;
    y = RAM(GRAB_SLOT);
    RAM(OBJ_HELD + y) = a;
    a = 0x01;
    RAM(GAME_ACTION) = a;
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    RAM(ATTACK_GROUND_Y) = a;
    a = 0x1B;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1882, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD883, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCF88, 2); call_by_address_tail(0xCF88, -1); return;
}

/*  $D7F2 - lifted by port/tools/lift.py */
void func_D7F2(void) {
    REGS_IN;
    mff_cycles(0xD7F2, 30);
    a = 0x80;
    RAM(PLAYER_NO_HIT) = a;
    y = RAM(GRAB_SLOT);
    a = 0x04;
    RAM(OBJ_ID + y) = a;
    a = 0x80;
    RAM(OBJ_STATE + y) = a;
    a = RAM(OBJ_DIR + y);
    a &= 0x01;
    if (a == 0) { mff_cycles(0xD807, 1); goto L_D813; }
    mff_cycles(0xD809, 13);
    a = RAM(OBJ_FLAGS + y);
    a &= 0xBF;
    RAM(OBJ_FLAGS + y) = a;
    if (a != 0) { mff_cycles(0xD811, 1); goto L_D81B; }
L_D813:;
    mff_cycles(0xD813, 11);
    a = RAM(OBJ_FLAGS + y);
    a |= 0x40;
    RAM(OBJ_FLAGS + y) = a;
L_D81B:;
    mff_cycles(0xD81B, 18);
    a = 0x19;
    RAM(ACTION_TIMER) = a;
    a = 0x0B;
    RAM(GAME_ACTION) = a;
    a = 0x33;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1827, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD828, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $D82E - lifted by port/tools/lift.py */
void func_D82E(void) {
    REGS_IN;
    mff_cycles(0xD82E, 7);
    a = RAM(JOY_PRESSED);
    m = 0x80;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xD834, 5);
        a = RAM(GRAB_TIMER);
        if (!(a & 0x80)) {
            mff_cycles(0xD838, 11);
            t = (uint8_t)(RAM(GRAB_TIMER) - 1);
            RAM(GRAB_TIMER) = t;
            n = (t) >> 7;
            z = (t) == 0;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xD836, 1);
        }
        mff_cycles(0xD83B, 27);
        a = 0x00;
        RAM(GAME_ACTION) = a;
        RAM(GUY_JUMP_WINDOW) = a;
        RAM(GRAB_HIT_COUNT) = a;
        a = 0xFF;
        y = RAM(GRAB_SLOT);
        n = (y) >> 7;
        z = (y) == 0;
        RAM(OBJ_HELD + y) = a;
        SYNC_OUT;
        if (!mff_call_at(0x184C, 0xCEAA, -1)) return;
        SYNC_IN;
        mff_cycles(0xD84D, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xD832, 1);
    }
    mff_cycles(0xD84E, 19);
    a = 0x38;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x04;
    RAM(OBJ_VY) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1E9 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xD85D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x185F, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD860, 4);
    a = prg_read((uint16_t)(0xE22E + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xD863, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1865, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD866, 41);
    a = 0x00;
    RAM(GRAB_TIMER) = a;
    RAM(GRAB_HIT_COUNT) = a;
    a = 0xFF;
    y = RAM(GRAB_SLOT);
    RAM(OBJ_HELD + y) = a;
    a = 0x01;
    RAM(GAME_ACTION) = a;
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    RAM(ATTACK_GROUND_Y) = a;
    a = 0x1B;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1882, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD883, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCF88, 2); call_by_address_tail(0xCF88, -1); return;
}

/*  $D83B - lifted by port/tools/lift.py */
void func_D83B(void) {
    REGS_IN;
    mff_cycles(0xD83B, 27);
    a = 0x00;
    RAM(GAME_ACTION) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(GRAB_HIT_COUNT) = a;
    a = 0xFF;
    y = RAM(GRAB_SLOT);
    n = (y) >> 7;
    z = (y) == 0;
    RAM(OBJ_HELD + y) = a;
    SYNC_OUT;
    if (!mff_call_at(0x184C, 0xCEAA, -1)) return;
    SYNC_IN;
    mff_cycles(0xD84D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $D886 - lifted by port/tools/lift.py */
void func_D886(void) {
    REGS_IN;
    mff_cycles(0xD886, 7);
    a = RAM(JOY_PRESSED);
    m = 0x40;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xD88A, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x182E), -1); return; }
    mff_cycles(0xD88C, 22);
    a = 0x80;
    RAM(PLAYER_NO_HIT) = a;
    a = 0x13;
    RAM(GAME_ACTION) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $D88C - lifted by port/tools/lift.py */
void func_D88C(void) {
    REGS_IN;
    mff_cycles(0xD88C, 22);
    a = 0x80;
    RAM(PLAYER_NO_HIT) = a;
    a = 0x13;
    RAM(GAME_ACTION) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $D89A - lifted by port/tools/lift.py */
void func_D89A(void) {
    REGS_IN;
    mff_cycles(0xD89A, 7);
    a = RAM(JOY_PRESSED);
    m = 0x40;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xD8A0, 10);
        a = RAM(OBJ_Y);
        n = (a) >> 7;
        z = (a) == 0;
        RAM(PLAYER_GROUND_Y) = a;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xD8E5, 2); call_by_address_tail(0xD8E5, -1); return;
    } else {
        mff_cycles(0xD89E, 1);
    }
    mff_cycles(0xD8A8, 25);
    y = RAM(GRAB_SLOT);
    a = 0x15;
    RAM(GAME_ACTION) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE) = a;
    RAM(OBJ_STATE + y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $D8A8 - lifted by port/tools/lift.py */
void func_D8A8(void) {
    REGS_IN;
    mff_cycles(0xD8A8, 25);
    y = RAM(GRAB_SLOT);
    a = 0x15;
    RAM(GAME_ACTION) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE) = a;
    RAM(OBJ_STATE + y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $D8B7 - lifted by port/tools/lift.py */
void func_D8B7(void) {
    REGS_IN;
    mff_cycles(0xD8B7, 60);
    y = RAM(GRAB_SLOT);
    a = 0x80;
    RAM(OBJ_STATE + y) = a;
    a = 0x12;
    RAM(GAME_ACTION) = a;
    a = 0x00;
    RAM(GUY_JUMP_WINDOW) = a;
    a = 0x4E;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x05;
    RAM(OBJ_VY) = a;
    a = 0x80;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VX) = a;
    a = RAM(OBJ_Y);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PLAYER_GROUND_Y) = a;
    RAM(ATTACK_GROUND_Y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $D8E5 - lifted by port/tools/lift.py */
void func_D8E5(void) {
    REGS_IN;
    goto L_D8E5;
L_D8E2:;
    mff_cycles(0xD8E2, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xD83B, 2); call_by_address_tail(0xD83B, -1); return;
L_D8E5:;
    mff_cycles(0xD8E5, 7);
    a = RAM(JOY_PRESSED);
    m = 0x80;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xD8E9, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x18B7), -1); return; }
    mff_cycles(0xD8EB, 5);
    a = RAM(GRAB_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) { mff_cycles(0xD8E2, 1); goto L_D8E2; }
    mff_cycles(0xD8EF, 13);
    t = (uint8_t)(RAM(GRAB_TIMER) - 1);
    RAM(GRAB_TIMER) = t;
    a = 0x58;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xD8F8, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x18FA, func_F288, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xD8FB, 8);
        a = 0x08;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x18FF, func_F440, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xD8F6, 2);
    }
    mff_cycles(0xD900, 9);
    x = 0x00;
    a = RAM(JOY_HELD);
    a &= 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xD906, 1); goto L_D92D; }
    mff_cycles(0xD908, 8);
    RAM(OBJ_DIR) = a;
    a &= 0x01;
    if (a == 0) { mff_cycles(0xD90D, 1); goto L_D95C; }
    mff_cycles(0xD90F, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1913, func_F003, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD914, 2);
    if (c) { mff_cycles(0xD914, 1); goto L_D959; }
    mff_cycles(0xD916, 10);
    y = RAM(GRAB_SLOT);
    a = RAM(PLAYER_DIR);
    if (a == 0x01) { mff_cycles(0xD91C, 1); goto L_D930; }
    mff_cycles(0xD91E, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    m = 0xF1;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xD926, 1); goto L_D959; }
    mff_cycles(0xD928, 2);
    if (c) { mff_cycles(0xD928, 1); goto L_D959; }
    mff_cycles(0xD92A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xD93C, 2); call_by_address_tail(0xD93C, -1); return;
L_D92D:;
    mff_cycles(0xD92D, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xD9A3, 2); call_by_address_tail(0xD9A3, -1); return;
L_D930:;
    mff_cycles(0xD930, 13);
    a = RAM(OBJ_X + y);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    m = 0xF9;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xD93A, 2);
        if (!c) {
            mff_cycles(0xD93C, 30);
            a = RAM(OBJ_X_FRAC + y);
            c = 0;
            m = RAM(OBJ_VX_FRAC);
            w = a + m + c;
            c = w > 0xFF;
            a = (uint8_t)w;
            RAM(OBJ_X_FRAC + y) = a;
            a = RAM(OBJ_X + y);
            m = RAM(OBJ_VX);
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_X + y) = a;
            if (c) {
                mff_cycles(0xD951, 11);
                a = RAM(OBJ_X_HI + y);
                m = 0x00;
                w = a + m + c;
                v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
                c = w > 0xFF;
                a = (uint8_t)w;
                n = (a) >> 7;
                z = (a) == 0;
                RAM(OBJ_X_HI + y) = a;
            } else {
                mff_cycles(0xD94F, 1);
            }
        } else {
            mff_cycles(0xD93A, 1);
        }
    } else {
        mff_cycles(0xD938, 1);
    }
L_D959:;
    mff_cycles(0xD959, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xD9A3, 2); call_by_address_tail(0xD9A3, -1); return;
L_D95C:;
    mff_cycles(0xD95C, 8);
    y = 0x01;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1960, func_F054, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD961, 2);
    if (c) { mff_cycles(0xD961, 1); goto L_D9A3; }
    mff_cycles(0xD963, 10);
    y = RAM(GRAB_SLOT);
    a = RAM(PLAYER_DIR);
    if (a == 0x01) { mff_cycles(0xD969, 1); goto L_D97A; }
    mff_cycles(0xD96B, 13);
    a = RAM(OBJ_X + y);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    m = 0x11;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xD973, 1); goto L_D9A3; }
    mff_cycles(0xD975, 2);
    if (!c) { mff_cycles(0xD975, 1); goto L_D9A3; }
    mff_cycles(0xD977, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xD986, 2); call_by_address_tail(0xD986, -1); return;
L_D97A:;
    mff_cycles(0xD97A, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    m = 0x18;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xD984, 2);
        if (c) {
            mff_cycles(0xD986, 30);
            a = RAM(OBJ_X_FRAC + y);
            c = 1;
            m = RAM(OBJ_VX_FRAC);
            w = a - m - !c;
            c = w >= 0;
            a = (uint8_t)w;
            RAM(OBJ_X_FRAC + y) = a;
            a = RAM(OBJ_X + y);
            m = RAM(OBJ_VX);
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            c = w >= 0;
            a = (uint8_t)w;
            RAM(OBJ_X + y) = a;
            if (!c) {
                mff_cycles(0xD99B, 11);
                a = RAM(OBJ_X_HI + y);
                m = 0x00;
                w = a - m - !c;
                v = ((a ^ m) & (a ^ w) & 0x80) != 0;
                c = w >= 0;
                a = (uint8_t)w;
                RAM(OBJ_X_HI + y) = a;
            } else {
                mff_cycles(0xD999, 1);
            }
        } else {
            mff_cycles(0xD984, 1);
        }
    } else {
        mff_cycles(0xD982, 1);
    }
L_D9A3:;
    mff_cycles(0xD9A3, 9);
    x = 0x00;
    a = RAM(JOY_HELD);
    a &= 0x0C;
    if (a == 0) { mff_cycles(0xD9A9, 1); goto L_D9E9; }
    mff_cycles(0xD9AB, 4);
    a &= 0x04;
    if (a == 0) { mff_cycles(0xD9AD, 1); goto L_D9D7; }
    mff_cycles(0xD9AF, 8);
    y = 0x02;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x19B3, func_F0A2, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD9B4, 2);
    if (c) { mff_cycles(0xD9B4, 1); goto L_D9E9; }
    mff_cycles(0xD9B6, 35);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_Y_FRAC + y);
    c = 0;
    m = RAM(OBJ_VY_FRAC);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_Y_FRAC + y) = a;
    a = RAM(OBJ_Y + y);
    m = RAM(OBJ_VY);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_Y + y) = a;
    m = 0xF0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0xD9CF, 7);
        m = 0x0F;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_Y + y) = a;
    } else {
        mff_cycles(0xD9CD, 1);
    }
    mff_cycles(0xD9D4, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xD9E9, 2); call_by_address_tail(0xD9E9, -1); return;
L_D9D7:;
    mff_cycles(0xD9D7, 8);
    y = 0x03;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x19DB, func_F0F7, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD9DC, 2);
    if (!c) {
        mff_cycles(0xD9DE, 16);
        y = RAM(GRAB_SLOT);
        a = RAM(OBJ_Y);
        c = 1;
        m = 0x08;
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        RAM(OBJ_Y + y) = a;
    } else {
        mff_cycles(0xD9DC, 1);
    }
L_D9E9:;
    mff_cycles(0xD9E9, 7);
    a = RAM(JOY_HELD);
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0xD9EF, 8);
        a = 0x33;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x19F3, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xD9F4, 8);
        a = 0x08;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x19F8, func_F440, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xD9ED, 1);
    }
    mff_cycles(0xD9F9, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $D93C - lifted by port/tools/lift.py */
void func_D93C(void) {
    REGS_IN;
    mff_cycles(0xD93C, 30);
    a = RAM(OBJ_X_FRAC + y);
    c = 0;
    m = RAM(OBJ_VX_FRAC);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_X_FRAC + y) = a;
    a = RAM(OBJ_X + y);
    m = RAM(OBJ_VX);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X + y) = a;
    if (c) {
        mff_cycles(0xD951, 11);
        a = RAM(OBJ_X_HI + y);
        m = 0x00;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_X_HI + y) = a;
    } else {
        mff_cycles(0xD94F, 1);
    }
    mff_cycles(0xD959, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xD9A3, 2); call_by_address_tail(0xD9A3, -1); return;
}

/*  $D986 - lifted by port/tools/lift.py */
void func_D986(void) {
    REGS_IN;
    mff_cycles(0xD986, 30);
    a = RAM(OBJ_X_FRAC + y);
    c = 1;
    m = RAM(OBJ_VX_FRAC);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_X_FRAC + y) = a;
    a = RAM(OBJ_X + y);
    m = RAM(OBJ_VX);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_X + y) = a;
    if (!c) {
        mff_cycles(0xD99B, 11);
        a = RAM(OBJ_X_HI + y);
        m = 0x00;
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        RAM(OBJ_X_HI + y) = a;
    } else {
        mff_cycles(0xD999, 1);
    }
    mff_cycles(0xD9A3, 9);
    x = 0x00;
    a = RAM(JOY_HELD);
    a &= 0x0C;
    if (a == 0) { mff_cycles(0xD9A9, 1); goto L_D9E9; }
    mff_cycles(0xD9AB, 4);
    a &= 0x04;
    if (a == 0) { mff_cycles(0xD9AD, 1); goto L_D9D7; }
    mff_cycles(0xD9AF, 8);
    y = 0x02;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x19B3, func_F0A2, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD9B4, 2);
    if (c) { mff_cycles(0xD9B4, 1); goto L_D9E9; }
    mff_cycles(0xD9B6, 35);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_Y_FRAC + y);
    c = 0;
    m = RAM(OBJ_VY_FRAC);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_Y_FRAC + y) = a;
    a = RAM(OBJ_Y + y);
    m = RAM(OBJ_VY);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_Y + y) = a;
    m = 0xF0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0xD9CF, 7);
        m = 0x0F;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_Y + y) = a;
    } else {
        mff_cycles(0xD9CD, 1);
    }
    mff_cycles(0xD9D4, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xD9E9, 2); call_by_address_tail(0xD9E9, -1); return;
L_D9D7:;
    mff_cycles(0xD9D7, 8);
    y = 0x03;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x19DB, func_F0F7, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD9DC, 2);
    if (!c) {
        mff_cycles(0xD9DE, 16);
        y = RAM(GRAB_SLOT);
        a = RAM(OBJ_Y);
        c = 1;
        m = 0x08;
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        RAM(OBJ_Y + y) = a;
    } else {
        mff_cycles(0xD9DC, 1);
    }
L_D9E9:;
    mff_cycles(0xD9E9, 7);
    a = RAM(JOY_HELD);
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0xD9EF, 8);
        a = 0x33;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x19F3, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xD9F4, 8);
        a = 0x08;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x19F8, func_F440, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xD9ED, 1);
    }
    mff_cycles(0xD9F9, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $D9A3 - lifted by port/tools/lift.py */
void func_D9A3(void) {
    REGS_IN;
    mff_cycles(0xD9A3, 9);
    x = 0x00;
    a = RAM(JOY_HELD);
    a &= 0x0C;
    if (a == 0) { mff_cycles(0xD9A9, 1); goto L_D9E9; }
    mff_cycles(0xD9AB, 4);
    a &= 0x04;
    if (a == 0) { mff_cycles(0xD9AD, 1); goto L_D9D7; }
    mff_cycles(0xD9AF, 8);
    y = 0x02;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x19B3, func_F0A2, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD9B4, 2);
    if (c) { mff_cycles(0xD9B4, 1); goto L_D9E9; }
    mff_cycles(0xD9B6, 35);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_Y_FRAC + y);
    c = 0;
    m = RAM(OBJ_VY_FRAC);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_Y_FRAC + y) = a;
    a = RAM(OBJ_Y + y);
    m = RAM(OBJ_VY);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_Y + y) = a;
    m = 0xF0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0xD9CF, 7);
        m = 0x0F;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_Y + y) = a;
    } else {
        mff_cycles(0xD9CD, 1);
    }
    mff_cycles(0xD9D4, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xD9E9, 2); call_by_address_tail(0xD9E9, -1); return;
L_D9D7:;
    mff_cycles(0xD9D7, 8);
    y = 0x03;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x19DB, func_F0F7, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD9DC, 2);
    if (!c) {
        mff_cycles(0xD9DE, 16);
        y = RAM(GRAB_SLOT);
        a = RAM(OBJ_Y);
        c = 1;
        m = 0x08;
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        RAM(OBJ_Y + y) = a;
    } else {
        mff_cycles(0xD9DC, 1);
    }
L_D9E9:;
    mff_cycles(0xD9E9, 7);
    a = RAM(JOY_HELD);
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0xD9EF, 8);
        a = 0x33;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x19F3, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xD9F4, 8);
        a = 0x08;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x19F8, func_F440, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xD9ED, 1);
    }
    mff_cycles(0xD9F9, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $D9E9 - lifted by port/tools/lift.py */
void func_D9E9(void) {
    REGS_IN;
    mff_cycles(0xD9E9, 7);
    a = RAM(JOY_HELD);
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0xD9EF, 8);
        a = 0x33;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x19F3, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xD9F4, 8);
        a = 0x08;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x19F8, func_F440, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xD9ED, 1);
    }
    mff_cycles(0xD9F9, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/* player_state_hit $D9FA - lifted by port/tools/lift.py */
void func_D9FA(void) {
    REGS_IN;
    mff_cycles(0xD9FA, 7);
    a = RAM(JOY_PRESSED);
    m = 0xC0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xDA00, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xCA3C, 2); call_by_address_tail(0xCA3C, -1); return;
    } else {
        mff_cycles(0xD9FE, 1);
    }
    mff_cycles(0xDA03, 7);
    a = RAM(PLAYER_INVULN);
    m = 0x1E;
    c = a >= m;
    if (!c) {
        mff_cycles(0xDA09, 7);
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(0xE1C1 + y));
        mff_cycles(0xDA0E, 8);
        RAM(OBJ_VX_FRAC) = a;
        a = prg_read((uint16_t)(0xE1C4 + y));
        mff_cycles(0xDA14, 8);
        RAM(OBJ_VX) = a;
        a = prg_read((uint16_t)(0xE1C7 + y));
        mff_cycles(0xDA1A, 8);
        RAM(OBJ_VY_FRAC) = a;
        a = prg_read((uint16_t)(0xE1CA + y));
        mff_cycles(0xDA20, 30);
        RAM(OBJ_VY) = a;
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(GRAB_TIMER) = a;
        RAM(GAME_ACTION) = a;
        RAM(COMBO_TIMER) = a;
        RAM(JUMP_MOVING) = a;
        RAM(GRAB_HIT_COUNT) = a;
        RAM(PLAYER_INVULN) = a;
        SYNC_OUT;
        if (!mff_call_at(0x1A33, 0xCEAA, -1)) return;
        SYNC_IN;
        mff_cycles(0xDA34, 7);
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(0xE288 + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0xDA39, 9);
        RAM(PLAYER_STAND_ANIM) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xDA07, 1);
    }
    mff_cycles(0xDA3C, 11);
    t = (uint8_t)(RAM(PLAYER_INVULN) - 1);
    RAM(PLAYER_INVULN) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/* player_state_knockdown $DA3F - lifted by port/tools/lift.py */
void func_DA3F(void) {
    REGS_IN;
    mff_cycles(0xDA3F, 10);
    x = 0x00;
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    if (a == 0) {
        mff_cycles(0xDA48, 23);
        a = RAM(PLAYER_DIR);
        a ^= 0x03;
        RAM(OBJ_DIR) = a;
        t = (uint8_t)(RAM(OBJ_STATE) + 1);
        RAM(OBJ_STATE) = t;
        a = 0x28;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x1A56, func_F34C, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xDA57, 5);
        a = RAM(BOSS_AREA);
        if (!(a & 0x80)) {
            mff_cycles(0xDA5B, 30);
            a = 0xE6;
            RAM(OBJ_VX_FRAC) = a;
            a = 0x00;
            RAM(OBJ_VX) = a;
            a = 0xE8;
            RAM(OBJ_VY_FRAC) = a;
            a = 0x03;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_VY) = a;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xDA59, 1);
        }
        mff_cycles(0xDA70, 24);
        a = 0x80;
        RAM(OBJ_VX_FRAC) = a;
        a = 0x01;
        RAM(OBJ_VX) = a;
        a = 0x6E;
        RAM(OBJ_VY_FRAC) = a;
        a = 0x04;
        RAM(OBJ_VY) = a;
    } else {
        mff_cycles(0xDA46, 1);
    }
    mff_cycles(0xDA84, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xDA8D, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xDB6C, 2); call_by_address_tail(0xDB6C, -1); return;
    } else {
        mff_cycles(0xDA8B, 1);
    }
    mff_cycles(0xDA90, 8);
    a = RAM(OBJ_DIR);
    a &= 0x01;
    if (a == 0) { mff_cycles(0xDA95, 1); goto L_DACE; }
    mff_cycles(0xDA97, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xDA9E, 1); goto L_DAC2; }
    mff_cycles(0xDAA0, 4);
    m = 0x04;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xDAA2, 1); goto L_DACB; }
    mff_cycles(0xDAA4, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    m = 0xE9;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xDAAC, 1); goto L_DABF; }
    mff_cycles(0xDAAE, 2);
    if (!c) { mff_cycles(0xDAAE, 1); goto L_DAC5; }
    mff_cycles(0xDAB0, 20);
    a = RAM(SCROLL_X);
    c = 0;
    m = 0xE8;
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_X) = a;
    a = RAM(AREA_SCREEN);
    m = 0x00;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_X_HI) = a;
L_DABF:;
    mff_cycles(0xDABF, 6);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    n = (t) >> 7;
    z = (t) == 0;
L_DAC2:;
    mff_cycles(0xDAC2, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xDB03, 2); call_by_address_tail(0xDB03, -1); return;
L_DAC5:;
    mff_cycles(0xDAC5, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1AC7, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDAC8, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xDB03, 2); call_by_address_tail(0xDB03, -1); return;
L_DACB:;
    mff_cycles(0xDACB, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xDBD4, 2); call_by_address_tail(0xDBD4, -1); return;
L_DACE:;
    mff_cycles(0xDACE, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x02;
    c = a >= m;
    if (a == m) { mff_cycles(0xDAD5, 2); goto L_DB03; }
    mff_cycles(0xDAD7, 4);
    m = 0x04;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xDACB, 1); goto L_DACB; }
    mff_cycles(0xDADB, 19);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(TMP0) = a;
    a = RAM(TMP0);
    m = 0x18;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xDAE7, 1); goto L_DAFA; }
    mff_cycles(0xDAE9, 2);
    if (c) { mff_cycles(0xDAE9, 2); goto L_DB00; }
    mff_cycles(0xDAEB, 20);
    a = RAM(SCROLL_X);
    c = 0;
    m = 0x19;
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_X) = a;
    a = RAM(AREA_SCREEN);
    m = 0x00;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_X_HI) = a;
L_DAFA:;
    mff_cycles(0xDAFA, 9);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xDB03, 2); call_by_address_tail(0xDB03, -1); return;
L_DB00:;
    mff_cycles(0xDB00, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1B02, func_F1A1, 0xE000)) return;
    SYNC_IN;
L_DB03:;
    mff_cycles(0xDB03, 8);
    y = 0x04;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1B07, func_F119, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDB08, 2);
    if (c) {
        mff_cycles(0xDB0A, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xDB2C, 2); call_by_address_tail(0xDB2C, -1); return;
    } else {
        mff_cycles(0xDB08, 1);
    }
    mff_cycles(0xDB0D, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0xDB10, 1); goto L_DB6B; }
    mff_cycles(0xDB12, 13);
    a = RAM(OBJ_Y);
    c = 0;
    m = 0x02;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    m = RAM(PLAYER_GROUND_Y);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xDB1A, 1); goto L_DB6B; }
    mff_cycles(0xDB1C, 2);
    if (!z) {
        mff_cycles(0xDB1E, 12);
        m = 0x03;
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_Y) = a;
        SYNC_OUT;
        if (!mff_call_at(0x1B25, 0xDC29, -1)) return;
        SYNC_IN;
        mff_cycles(0xDB26, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xDB2C, 2); call_by_address_tail(0xDB2C, -1); return;
    } else {
        mff_cycles(0xDB1C, 1);
    }
    mff_cycles(0xDB29, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1B2B, 0xDC29, -1)) return;
    SYNC_IN;
    mff_cycles(0xDB2C, 11);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    a = RAM(BOSS_AREA);
    if (a & 0x80) { mff_cycles(0xDB31, 1); goto L_DB49; }
    mff_cycles(0xDB33, 26);
    a = 0x80;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x00;
    RAM(OBJ_VX) = a;
    a = 0x6E;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VY) = a;
    if (a != 0) { mff_cycles(0xDB47, 1); goto L_DB5D; }
L_DB49:;
    mff_cycles(0xDB49, 24);
    a = 0x00;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VX) = a;
    a = 0x6E;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VY) = a;
L_DB5D:;
    mff_cycles(0xDB5D, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE20D + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xDB62, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1B64, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDB65, 4);
    a = prg_read((uint16_t)(0xE255 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xDB68, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1B6A, func_F440, 0xE000)) return;
    SYNC_IN;
L_DB6B:;
    mff_cycles(0xDB6B, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $DB03 - lifted by port/tools/lift.py */
void func_DB03(void) {
    REGS_IN;
    mff_cycles(0xDB03, 8);
    y = 0x04;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1B07, func_F119, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDB08, 2);
    if (c) {
        mff_cycles(0xDB0A, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xDB2C, 2); call_by_address_tail(0xDB2C, -1); return;
    } else {
        mff_cycles(0xDB08, 1);
    }
    mff_cycles(0xDB0D, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0xDB10, 1); goto L_DB6B; }
    mff_cycles(0xDB12, 13);
    a = RAM(OBJ_Y);
    c = 0;
    m = 0x02;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    m = RAM(PLAYER_GROUND_Y);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xDB1A, 1); goto L_DB6B; }
    mff_cycles(0xDB1C, 2);
    if (!z) {
        mff_cycles(0xDB1E, 12);
        m = 0x03;
        w = a - m - !c;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_Y) = a;
        SYNC_OUT;
        if (!mff_call_at(0x1B25, 0xDC29, -1)) return;
        SYNC_IN;
        mff_cycles(0xDB26, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xDB2C, 2); call_by_address_tail(0xDB2C, -1); return;
    } else {
        mff_cycles(0xDB1C, 1);
    }
    mff_cycles(0xDB29, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1B2B, 0xDC29, -1)) return;
    SYNC_IN;
    mff_cycles(0xDB2C, 11);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    a = RAM(BOSS_AREA);
    if (a & 0x80) { mff_cycles(0xDB31, 1); goto L_DB49; }
    mff_cycles(0xDB33, 26);
    a = 0x80;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x00;
    RAM(OBJ_VX) = a;
    a = 0x6E;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VY) = a;
    if (a != 0) { mff_cycles(0xDB47, 1); goto L_DB5D; }
L_DB49:;
    mff_cycles(0xDB49, 24);
    a = 0x00;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VX) = a;
    a = 0x6E;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VY) = a;
L_DB5D:;
    mff_cycles(0xDB5D, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE20D + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xDB62, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1B64, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDB65, 4);
    a = prg_read((uint16_t)(0xE255 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xDB68, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1B6A, func_F440, 0xE000)) return;
    SYNC_IN;
L_DB6B:;
    mff_cycles(0xDB6B, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $DB2C - lifted by port/tools/lift.py */
void func_DB2C(void) {
    REGS_IN;
    mff_cycles(0xDB2C, 11);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    a = RAM(BOSS_AREA);
    if (a & 0x80) { mff_cycles(0xDB31, 1); goto L_DB49; }
    mff_cycles(0xDB33, 26);
    a = 0x80;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x00;
    RAM(OBJ_VX) = a;
    a = 0x6E;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VY) = a;
    if (a != 0) { mff_cycles(0xDB47, 1); goto L_DB5D; }
L_DB49:;
    mff_cycles(0xDB49, 24);
    a = 0x00;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VX) = a;
    a = 0x6E;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VY) = a;
L_DB5D:;
    mff_cycles(0xDB5D, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE20D + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xDB62, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1B64, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDB65, 4);
    a = prg_read((uint16_t)(0xE255 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xDB68, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1B6A, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDB6B, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $DB6C - lifted by port/tools/lift.py */
void func_DB6C(void) {
    REGS_IN;
    mff_cycles(0xDB6C, 8);
    a = RAM(OBJ_DIR);
    a &= 0x01;
    if (a == 0) { mff_cycles(0xDB71, 1); goto L_DBA3; }
    mff_cycles(0xDB73, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x04;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xDB7A, 1); goto L_DB9A; }
    mff_cycles(0xDB7C, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    m = 0xE8;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xDB84, 1); goto L_DB97; }
    mff_cycles(0xDB86, 2);
    if (!c) { mff_cycles(0xDB86, 1); goto L_DB9D; }
    mff_cycles(0xDB88, 20);
    a = RAM(SCROLL_X);
    c = 0;
    m = 0xE9;
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_X) = a;
    a = RAM(AREA_SCREEN);
    m = 0x00;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_X_HI) = a;
L_DB97:;
    mff_cycles(0xDB97, 6);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    n = (t) >> 7;
    z = (t) == 0;
L_DB9A:;
    mff_cycles(0xDB9A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xDBFD, 2); call_by_address_tail(0xDBFD, -1); return;
L_DB9D:;
    mff_cycles(0xDB9D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1B9F, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDBA0, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xDBD4, 2); call_by_address_tail(0xDBD4, -1); return;
L_DBA3:;
    mff_cycles(0xDBA3, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x04;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xDBAA, 1); goto L_DBCE; }
    mff_cycles(0xDBAC, 19);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(TMP0) = a;
    a = RAM(TMP0);
    m = 0x18;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xDBB8, 1); goto L_DBCB; }
    mff_cycles(0xDBBA, 2);
    if (c) { mff_cycles(0xDBBA, 1); goto L_DBD1; }
    mff_cycles(0xDBBC, 20);
    a = RAM(SCROLL_X);
    c = 0;
    m = 0x19;
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_X) = a;
    a = RAM(AREA_SCREEN);
    m = 0x00;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_X_HI) = a;
L_DBCB:;
    mff_cycles(0xDBCB, 7);
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
L_DBCE:;
    mff_cycles(0xDBCE, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xDBD4, 2); call_by_address_tail(0xDBD4, -1); return;
L_DBD1:;
    mff_cycles(0xDBD1, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1BD3, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDBD4, 8);
    y = 0x04;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1BD8, func_F119, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDBD9, 2);
    if (c) {
        mff_cycles(0xDBDB, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xDBFD, 2); call_by_address_tail(0xDBFD, -1); return;
    } else {
        mff_cycles(0xDBD9, 1);
    }
    mff_cycles(0xDBDE, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0xDBE3, 13);
        a = RAM(OBJ_Y);
        c = 0;
        m = 0x02;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        m = RAM(PLAYER_GROUND_Y);
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0xDBED, 2);
            if (!z) {
                mff_cycles(0xDBEF, 12);
                m = 0x03;
                w = a - m - !c;
                v = ((a ^ m) & (a ^ w) & 0x80) != 0;
                c = w >= 0;
                a = (uint8_t)w;
                n = (a) >> 7;
                z = (a) == 0;
                RAM(OBJ_Y) = a;
                SYNC_OUT;
                if (!mff_call_at(0x1BF6, 0xDC29, -1)) return;
                SYNC_IN;
                mff_cycles(0xDBF7, 3);
                SYNC_OUT;
                nes_cpu_instruction_boundary(0xDBFD, 2); call_by_address_tail(0xDBFD, -1); return;
            } else {
                mff_cycles(0xDBED, 1);
            }
            mff_cycles(0xDBFA, 6);
            SYNC_OUT;
            if (!mff_call_at(0x1BFC, 0xDC29, -1)) return;
            SYNC_IN;
            mff_cycles(0xDBFD, 7);
            y = RAM(PLAYER_ID);
            a = prg_read((uint16_t)(0xE20D + y));
            n = (a) >> 7;
            z = (a) == 0;
            mff_cycles(0xDC02, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x1C04, func_F294, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xDC05, 4);
            a = prg_read((uint16_t)(0xE255 + y));
            n = (a) >> 7;
            z = (a) == 0;
            mff_cycles(0xDC08, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x1C0A, func_F440, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xDC0B, 40);
            a = 0x3C;
            RAM(ACTION_TIMER) = a;
            a = 0x1F;
            RAM(GAME_ACTION) = a;
            a = 0x80;
            RAM(OBJ_STATE) = a;
            a = 0x00;
            RAM(OBJ_TIMER) = a;
            RAM(ATTACK_LIVE) = a;
            RAM(ATTACK_GROUND_Y) = a;
            a = RAM(PLAYER_DIR);
            n = (a) >> 7;
            z = (a) == 0;
            RAM(JUMP_DIR) = a;
            SYNC_OUT;
            if (!mff_call_at(0x1C27, 0xDC29, -1)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xDBEB, 2);
        }
    } else {
        mff_cycles(0xDBE1, 2);
    }
    mff_cycles(0xDC28, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $DBD4 - lifted by port/tools/lift.py */
void func_DBD4(void) {
    REGS_IN;
    mff_cycles(0xDBD4, 8);
    y = 0x04;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1BD8, func_F119, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDBD9, 2);
    if (c) {
        mff_cycles(0xDBDB, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xDBFD, 2); call_by_address_tail(0xDBFD, -1); return;
    } else {
        mff_cycles(0xDBD9, 1);
    }
    mff_cycles(0xDBDE, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0xDBE3, 13);
        a = RAM(OBJ_Y);
        c = 0;
        m = 0x02;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        m = RAM(PLAYER_GROUND_Y);
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0xDBED, 2);
            if (!z) {
                mff_cycles(0xDBEF, 12);
                m = 0x03;
                w = a - m - !c;
                v = ((a ^ m) & (a ^ w) & 0x80) != 0;
                c = w >= 0;
                a = (uint8_t)w;
                n = (a) >> 7;
                z = (a) == 0;
                RAM(OBJ_Y) = a;
                SYNC_OUT;
                if (!mff_call_at(0x1BF6, 0xDC29, -1)) return;
                SYNC_IN;
                mff_cycles(0xDBF7, 3);
                SYNC_OUT;
                nes_cpu_instruction_boundary(0xDBFD, 2); call_by_address_tail(0xDBFD, -1); return;
            } else {
                mff_cycles(0xDBED, 1);
            }
            mff_cycles(0xDBFA, 6);
            SYNC_OUT;
            if (!mff_call_at(0x1BFC, 0xDC29, -1)) return;
            SYNC_IN;
            mff_cycles(0xDBFD, 7);
            y = RAM(PLAYER_ID);
            a = prg_read((uint16_t)(0xE20D + y));
            n = (a) >> 7;
            z = (a) == 0;
            mff_cycles(0xDC02, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x1C04, func_F294, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xDC05, 4);
            a = prg_read((uint16_t)(0xE255 + y));
            n = (a) >> 7;
            z = (a) == 0;
            mff_cycles(0xDC08, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x1C0A, func_F440, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xDC0B, 40);
            a = 0x3C;
            RAM(ACTION_TIMER) = a;
            a = 0x1F;
            RAM(GAME_ACTION) = a;
            a = 0x80;
            RAM(OBJ_STATE) = a;
            a = 0x00;
            RAM(OBJ_TIMER) = a;
            RAM(ATTACK_LIVE) = a;
            RAM(ATTACK_GROUND_Y) = a;
            a = RAM(PLAYER_DIR);
            n = (a) >> 7;
            z = (a) == 0;
            RAM(JUMP_DIR) = a;
            SYNC_OUT;
            if (!mff_call_at(0x1C27, 0xDC29, -1)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xDBEB, 2);
        }
    } else {
        mff_cycles(0xDBE1, 2);
    }
    mff_cycles(0xDC28, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $DBFD - lifted by port/tools/lift.py */
void func_DBFD(void) {
    REGS_IN;
    mff_cycles(0xDBFD, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE20D + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xDC02, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1C04, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDC05, 4);
    a = prg_read((uint16_t)(0xE255 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xDC08, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1C0A, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDC0B, 40);
    a = 0x3C;
    RAM(ACTION_TIMER) = a;
    a = 0x1F;
    RAM(GAME_ACTION) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0x00;
    RAM(OBJ_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(ATTACK_GROUND_Y) = a;
    a = RAM(PLAYER_DIR);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(JUMP_DIR) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1C27, 0xDC29, -1)) return;
    SYNC_IN;
    mff_cycles(0xDC28, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $DC29 - lifted by port/tools/lift.py */
void func_DC29(void) {
    REGS_IN;
    mff_cycles(0xDC29, 7);
    y = RAM(ROUND);
    a = prg_read((uint16_t)(0xDC59 + y));
    mff_cycles(0xDC2E, 7);
    RAM(TMP4) = a;
    a = prg_read((uint16_t)(0xDC5E + y));
    mff_cycles(0xDC33, 15);
    RAM(TMP5) = a;
    y = RAM(SUB_AREA);
    a = RAM(OBJ_Y);
    m = mem_read((uint16_t)(RAM16(TMP4) + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xDC3C, 2);
    if (z) { mff_cycles(0xDC3C, 1); goto L_DC40; }
    mff_cycles(0xDC3E, 2);
    if (c) { mff_cycles(0xDC3E, 1); goto L_DC58; }
L_DC40:;
    mff_cycles(0xDC40, 5);
    a = mem_read((uint16_t)(RAM16(TMP4) + y));
    mff_cycles(0xDC42, 14);
    RAM(OBJ_Y) = a;
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(PLAYER_ID);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xDC4D, 15);
        a = RAM(OBJ_Y);
        c = 0;
        m = 0x02;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_Y) = a;
        RAM(PLAYER_GROUND_Y) = a;
    } else {
        mff_cycles(0xDC4B, 1);
    }
L_DC58:;
    mff_cycles(0xDC58, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/* player_state_continue $DC6C - lifted by port/tools/lift.py */
void func_DC6C(void) {
    REGS_IN;
    mff_cycles(0xDC6C, 13);
    a = 0x00;
    RAM(RENDER_OFF_DEPTH) = a;
    a = 0xF2;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1C74, func_F34A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDC75, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1C77, 0xC36E, -1)) return;
    SYNC_IN;
    mff_cycles(0xDC78, 6);
    SYNC_OUT;
    mff_call_fn(0x1C7A, func_FF31, 0xE000);
    SYNC_IN;
    mff_cycles(0xDC7B, 23);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SCROLL_X) = a;
    RAM(SCROLL_Y) = a;
    RAM(SCROLL_NT) = a;
    RAM(SCROLL_Y_HI) = a;
    RAM(SCROLL_X_HI) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1C89, 0xC265, -1)) return;
    SYNC_IN;
    mff_cycles(0xDC8A, 11);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OAM_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1C90, 0xC31D, -1)) return;
    SYNC_IN;
    mff_cycles(0xDC91, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1C93, 0xC334, -1)) return;
    SYNC_IN;
    mff_cycles(0xDC94, 12);
    a = 0x20;
    x = 0x00;
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x1C9C, 0xC2D1, -1)) return;
    SYNC_IN;
    mff_cycles(0xDC9D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1C9F, 0xC31D, -1)) return;
    SYNC_IN;
    mff_cycles(0xDCA0, 16);
    x = 0x04;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x1CA9, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDCAA, 5);
    a = RAM(CREDITS);
    if (a != 0) {
        mff_cycles(0xDCAE, 8);
        x = 0x03;
        n = (x) >> 7;
        z = (x) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x1CB2, 0x8000, -1)) return;
        SYNC_IN;
        mff_cycles(0xDCB3, 6);
        SYNC_OUT;
        if (!mff_call_at(0x1CB5, 0xC26F, -1)) return;
        SYNC_IN;
        mff_cycles(0xDCB6, 6);
        SYNC_OUT;
        if (!mff_call_at(0x1CB8, 0x9300, -1)) return;
        SYNC_IN;
        mff_cycles(0xDCB9, 7);
        a = RAM(GAME_ACTION);
        m = 0x07;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) {
            mff_cycles(0xDCBF, 6);
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xDCBD, 1);
        }
        mff_cycles(0xDCC0, 11);
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(RENDER_OFF_DEPTH) = a;
        SYNC_OUT;
        if (!mff_call_at(0x1CC6, 0xC85C, -1)) return;
        SYNC_IN;
        mff_cycles(0xDCC7, 16);
        x = 0x04;
        RAM(BANK_REQUEST) = x;
        x++;
        n = (x) >> 7;
        z = (x) == 0;
        RAM(BANK_REQUEST + 1) = x;
        SYNC_OUT;
        if (!mff_call_fn(0x1CD0, func_FF4D, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xDCAC, 1);
    }
    mff_cycles(0xDCD1, 8);
    x = 0x05;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x1CD5, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xDCD6, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1CD8, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xDCD9, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1CDB, 0x9500, -1)) return;
    SYNC_IN;
    mff_cycles(0xDCDC, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/* player_state_throw $DCDD - lifted by port/tools/lift.py */
void func_DCDD(void) {
    REGS_IN;
    goto L_DCDD;
L_DC6C:;
    mff_cycles(0xDC6C, 13);
    a = 0x00;
    RAM(RENDER_OFF_DEPTH) = a;
    a = 0xF2;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1C74, func_F34A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDC75, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1C77, 0xC36E, -1)) return;
    SYNC_IN;
    mff_cycles(0xDC78, 6);
    SYNC_OUT;
    mff_call_fn(0x1C7A, func_FF31, 0xE000);
    SYNC_IN;
    mff_cycles(0xDC7B, 23);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SCROLL_X) = a;
    RAM(SCROLL_Y) = a;
    RAM(SCROLL_NT) = a;
    RAM(SCROLL_Y_HI) = a;
    RAM(SCROLL_X_HI) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1C89, 0xC265, -1)) return;
    SYNC_IN;
    mff_cycles(0xDC8A, 11);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OAM_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1C90, 0xC31D, -1)) return;
    SYNC_IN;
    mff_cycles(0xDC91, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1C93, 0xC334, -1)) return;
    SYNC_IN;
    mff_cycles(0xDC94, 12);
    a = 0x20;
    x = 0x00;
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x1C9C, 0xC2D1, -1)) return;
    SYNC_IN;
    mff_cycles(0xDC9D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1C9F, 0xC31D, -1)) return;
    SYNC_IN;
    mff_cycles(0xDCA0, 16);
    x = 0x04;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x1CA9, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDCAA, 5);
    a = RAM(CREDITS);
    if (a != 0) {
        mff_cycles(0xDCAE, 8);
        x = 0x03;
        n = (x) >> 7;
        z = (x) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x1CB2, 0x8000, -1)) return;
        SYNC_IN;
        mff_cycles(0xDCB3, 6);
        SYNC_OUT;
        if (!mff_call_at(0x1CB5, 0xC26F, -1)) return;
        SYNC_IN;
        mff_cycles(0xDCB6, 6);
        SYNC_OUT;
        if (!mff_call_at(0x1CB8, 0x9300, -1)) return;
        SYNC_IN;
        mff_cycles(0xDCB9, 7);
        a = RAM(GAME_ACTION);
        m = 0x07;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) {
            mff_cycles(0xDCBF, 6);
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xDCBD, 1);
        }
        mff_cycles(0xDCC0, 11);
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(RENDER_OFF_DEPTH) = a;
        SYNC_OUT;
        if (!mff_call_at(0x1CC6, 0xC85C, -1)) return;
        SYNC_IN;
        mff_cycles(0xDCC7, 16);
        x = 0x04;
        RAM(BANK_REQUEST) = x;
        x++;
        n = (x) >> 7;
        z = (x) == 0;
        RAM(BANK_REQUEST + 1) = x;
        SYNC_OUT;
        if (!mff_call_fn(0x1CD0, func_FF4D, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xDCAC, 1);
    }
    mff_cycles(0xDCD1, 8);
    x = 0x05;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x1CD5, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xDCD6, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1CD8, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xDCD9, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1CDB, 0x9500, -1)) return;
    SYNC_IN;
    mff_cycles(0xDCDC, 6);
    SYNC_OUT;
    mff_rts(); return;
L_DCDD:;
    mff_cycles(0xDCDD, 5);
    a = RAM(HEALTH);
    if (a & 0x80) { mff_cycles(0xDCDF, 2); goto L_DD2D; }
    mff_cycles(0xDCE1, 15);
    a = 0x00;
    RAM(COMBO_TIMER) = a;
    t = (uint8_t)(RAM(ACTION_TIMER) - 1);
    RAM(ACTION_TIMER) = t;
    a = RAM(ACTION_TIMER);
    if (!(a & 0x80)) {
        mff_cycles(0xDCEB, 4);
        m = 0x01;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0xDCEF, 6);
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0xDCED, 1);
        }
    } else {
        mff_cycles(0xDCE9, 1);
    }
    mff_cycles(0xDCF0, 34);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(JUMP_MOVING) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(COMBO_TIMER) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(GRAB_HIT_COUNT) = a;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER2) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1D06, 0xCEAA, -1)) return;
    SYNC_IN;
    mff_cycles(0xDD07, 7);
    a = RAM(PLAYER_ID);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0xDD0B, 1); goto L_DD25; }
    mff_cycles(0xDD0D, 4);
    m = 0x02;
    c = a >= m;
    if (a == m) { mff_cycles(0xDD0F, 1); goto L_DD25; }
    mff_cycles(0xDD11, 7);
    a = RAM(PLAYER_DIR);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xDD15, 1); goto L_DD1C; }
    mff_cycles(0xDD17, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1D19, func_F048, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDD1A, 2);
    if (!z) { mff_cycles(0xDD1A, 1); goto L_DD1F; }
L_DD1C:;
    mff_cycles(0xDD1C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1D1E, func_F03F, 0xE000)) return;
    SYNC_IN;
L_DD1F:;
    mff_cycles(0xDD1F, 8);
    a = RAM(PLAYER_DIR);
    a ^= 0x03;
    RAM(PLAYER_DIR) = a;
L_DD25:;
    mff_cycles(0xDD25, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xDD2A, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    mff_rts(); return;
L_DD2D:;
    mff_cycles(0xDD2D, 5);
    a = RAM(ACTION_TIMER);
    if (a == 0) { mff_cycles(0xDD2F, 1); goto L_DD4A; }
    mff_cycles(0xDD31, 4);
    a &= 0x01;
    if (a != 0) { mff_cycles(0xDD33, 1); goto L_DD3F; }
    mff_cycles(0xDD35, 12);
    a = RAM(OBJ_FLAGS);
    a |= 0x04;
    RAM(OBJ_FLAGS) = a;
    if (a != 0) { mff_cycles(0xDD3D, 1); goto L_DD47; }
L_DD3F:;
    mff_cycles(0xDD3F, 10);
    a = RAM(OBJ_FLAGS);
    a &= 0xFB;
    RAM(OBJ_FLAGS) = a;
L_DD47:;
    mff_cycles(0xDD47, 11);
    t = (uint8_t)(RAM(ACTION_TIMER) - 1);
    RAM(ACTION_TIMER) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_DD4A:;
    mff_cycles(0xDD4A, 16);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    t = (uint8_t)(RAM(LIVES) - 1);
    RAM(LIVES) = t;
    a = RAM(LIVES);
    if (!(a & 0x80)) { mff_cycles(0xDD53, 1); goto L_DD65; }
    mff_cycles(0xDD55, 13);
    a = 0x00;
    RAM(SPECIAL_SCREEN_ID) = a;
    a = 0x32;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1D5D, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDD5E, 8);
    a = 0x09;
    RAM(GAME_ACTION) = a;
    goto L_DC6C;
L_DD65:;
    mff_cycles(0xDD65, 11);
    a = 0x25;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/* player_state_hitpos $DD6A - lifted by port/tools/lift.py */
void func_DD6A(void) {
    REGS_IN;
    mff_cycles(0xDD6A, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xDD79 + y));
    mff_cycles(0xDD6F, 7);
    RAM(TMP4) = a;
    a = prg_read((uint16_t)(0xDD7C + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xDD74, 8);
    RAM(TMP5) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0004); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $DD7F - lifted by port/tools/lift.py */
void func_DD7F(void) {
    REGS_IN;
    mff_cycles(0xDD7F, 7);
    a = RAM(JOY_PRESSED);
    a &= 0x40;
    if (a == 0) { mff_cycles(0xDD83, 1); goto L_DDA8; }
    mff_cycles(0xDD85, 5);
    a = RAM(COMBO_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xDD87, 1); goto L_DDC1; }
    mff_cycles(0xDD89, 11);
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
L_DD8E:;
    mff_cycles(0xDD8E, 8);
    a = RAM(OBJ_ANIM);
    m = 0x45;
    c = a >= m;
    if (a == m) { mff_cycles(0xDD93, 1); goto L_DD9D; }
    mff_cycles(0xDD95, 7);
    a = RAM(PLAYER_STAND_ANIM);
    m = 0x16;
    c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xDD99, 1); goto L_DD9D; }
    mff_cycles(0xDD9B, 2);
    if (!z) { mff_cycles(0xDD9B, 1); goto L_DDC4; }
L_DD9D:;
    mff_cycles(0xDD9D, 18);
    a = 0x01;
    RAM(OBJ_ANIM_FRAME) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ANIM_TIMER) = a;
    SYNC_OUT;
    mff_rts(); return;
L_DDA8:;
    mff_cycles(0xDDA8, 5);
    a = RAM(ACTION_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xDDAA, 1); goto L_DDB7; }
    mff_cycles(0xDDAC, 10);
    t = (uint8_t)(RAM(ACTION_TIMER) - 1);
    RAM(ACTION_TIMER) = t;
    a = RAM(COMBO_TIMER);
    if (a == 0) { mff_cycles(0xDD8E, 1); goto L_DD8E; }
    mff_cycles(0xDDB2, 8);
    t = (uint8_t)(RAM(COMBO_TIMER) - 1);
    RAM(COMBO_TIMER) = t;
    goto L_DD8E;
L_DDB7:;
    mff_cycles(0xDDB7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1DB9, 0xCEAA, -1)) return;
    SYNC_IN;
    mff_cycles(0xDDBA, 14);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(ATTACK_LIVE) = a;
    SYNC_OUT;
    mff_rts(); return;
L_DDC1:;
    mff_cycles(0xDDC1, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCB07, 2); call_by_address_tail(0xCB07, -1); return;
L_DDC4:;
    mff_cycles(0xDDC4, 5);
    a = RAM(COMBO_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xDDB7, 1); goto L_DDB7; }
    mff_cycles(0xDDC8, 19);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ANIM_FRAME) = a;
    RAM(OBJ_ANIM_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $DD8E - lifted by port/tools/lift.py */
void func_DD8E(void) {
    REGS_IN;
    mff_cycles(0xDD8E, 8);
    a = RAM(OBJ_ANIM);
    m = 0x45;
    c = a >= m;
    if (a == m) { mff_cycles(0xDD93, 1); goto L_DD9D; }
    mff_cycles(0xDD95, 7);
    a = RAM(PLAYER_STAND_ANIM);
    m = 0x16;
    c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xDD99, 1); goto L_DD9D; }
    mff_cycles(0xDD9B, 2);
    if (!z) { mff_cycles(0xDD9B, 1); goto L_DDC4; }
L_DD9D:;
    mff_cycles(0xDD9D, 18);
    a = 0x01;
    RAM(OBJ_ANIM_FRAME) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ANIM_TIMER) = a;
    SYNC_OUT;
    mff_rts(); return;
L_DDB7:;
    mff_cycles(0xDDB7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1DB9, 0xCEAA, -1)) return;
    SYNC_IN;
    mff_cycles(0xDDBA, 14);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(ATTACK_LIVE) = a;
    SYNC_OUT;
    mff_rts(); return;
L_DDC4:;
    mff_cycles(0xDDC4, 5);
    a = RAM(COMBO_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xDDB7, 1); goto L_DDB7; }
    mff_cycles(0xDDC8, 19);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ANIM_FRAME) = a;
    RAM(OBJ_ANIM_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $DDB7 - lifted by port/tools/lift.py */
void func_DDB7(void) {
    REGS_IN;
    mff_cycles(0xDDB7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1DB9, 0xCEAA, -1)) return;
    SYNC_IN;
    mff_cycles(0xDDBA, 14);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(ATTACK_LIVE) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $DDC1 - lifted by port/tools/lift.py */
void func_DDC1(void) {
    REGS_IN;
    mff_cycles(0xDDC1, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCB07, 2); call_by_address_tail(0xCB07, -1); return;
}

/*  $DDC4 - lifted by port/tools/lift.py */
void func_DDC4(void) {
    REGS_IN;
    mff_cycles(0xDDC4, 5);
    a = RAM(COMBO_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xDDC6, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1DB7), -1); return; }
    mff_cycles(0xDDC8, 19);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ANIM_FRAME) = a;
    RAM(OBJ_ANIM_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $DDD3 - lifted by port/tools/lift.py */
void func_DDD3(void) {
    REGS_IN;
    mff_cycles(0xDDD3, 7);
    a = RAM(JOY_PRESSED);
    a &= 0x40;
    if (a == 0) { mff_cycles(0xDDD7, 1); goto L_DDF0; }
    mff_cycles(0xDDD9, 5);
    a = RAM(COMBO_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xDDDB, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1DC1), -1); return; }
    mff_cycles(0xDDDD, 11);
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
L_DDE2:;
    mff_cycles(0xDDE2, 7);
    a = RAM(PLAYER_STAND_ANIM);
    m = 0x17;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xDDE8, 2);
        if (!z) { mff_cycles(0xDDE8, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1DC4), -1); return; }
    } else {
        mff_cycles(0xDDE6, 1);
    }
    mff_cycles(0xDDEA, 8);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1DEE, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDDEF, 6);
    SYNC_OUT;
    mff_rts(); return;
L_DDF0:;
    mff_cycles(0xDDF0, 5);
    a = RAM(ACTION_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xDDF2, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1DB7), -1); return; }
    mff_cycles(0xDDF4, 10);
    t = (uint8_t)(RAM(ACTION_TIMER) - 1);
    RAM(ACTION_TIMER) = t;
    a = RAM(COMBO_TIMER);
    if (a == 0) { mff_cycles(0xDDE2, 1); goto L_DDE2; }
    mff_cycles(0xDDFA, 8);
    t = (uint8_t)(RAM(COMBO_TIMER) - 1);
    RAM(COMBO_TIMER) = t;
    goto L_DDE2;
}

/*  $DDE2 - lifted by port/tools/lift.py */
void func_DDE2(void) {
    REGS_IN;
    mff_cycles(0xDDE2, 7);
    a = RAM(PLAYER_STAND_ANIM);
    m = 0x17;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xDDE8, 2);
        if (!z) { mff_cycles(0xDDE8, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1DC4), -1); return; }
    } else {
        mff_cycles(0xDDE6, 1);
    }
    mff_cycles(0xDDEA, 8);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1DEE, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDDEF, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $DDFF - lifted by port/tools/lift.py */
void func_DDFF(void) {
    REGS_IN;
    goto L_DDFF;
L_DDB7:;
    mff_cycles(0xDDB7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1DB9, 0xCEAA, -1)) return;
    SYNC_IN;
    mff_cycles(0xDDBA, 14);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(ATTACK_LIVE) = a;
    SYNC_OUT;
    mff_rts(); return;
L_DDC4:;
    mff_cycles(0xDDC4, 5);
    a = RAM(COMBO_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xDDB7, 1); goto L_DDB7; }
    mff_cycles(0xDDC8, 19);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ANIM_FRAME) = a;
    RAM(OBJ_ANIM_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    SYNC_OUT;
    mff_rts(); return;
L_DDFF:;
    mff_cycles(0xDDFF, 7);
    a = RAM(JOY_PRESSED);
    a &= 0x40;
    if (a == 0) { mff_cycles(0xDE03, 1); goto L_DE21; }
    mff_cycles(0xDE05, 5);
    a = RAM(COMBO_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xDE07, 1); goto L_DE30; }
    mff_cycles(0xDE09, 11);
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
L_DE0E:;
    mff_cycles(0xDE0E, 7);
    a = RAM(PLAYER_STAND_ANIM);
    m = 0x18;
    c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xDE12, 1); goto L_DE16; }
    mff_cycles(0xDE14, 2);
    if (!z) { mff_cycles(0xDDC4, 2); goto L_DDC4; }
L_DE16:;
    mff_cycles(0xDE16, 8);
    a = 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1E1A, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDE1B, 8);
    a = 0x09;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1E1F, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDE20, 6);
    SYNC_OUT;
    mff_rts(); return;
L_DE21:;
    mff_cycles(0xDE21, 5);
    a = RAM(ACTION_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xDDB7, 2); goto L_DDB7; }
    mff_cycles(0xDE25, 10);
    t = (uint8_t)(RAM(ACTION_TIMER) - 1);
    RAM(ACTION_TIMER) = t;
    a = RAM(COMBO_TIMER);
    if (a == 0) { mff_cycles(0xDE0E, 1); goto L_DE0E; }
    mff_cycles(0xDE2B, 8);
    t = (uint8_t)(RAM(COMBO_TIMER) - 1);
    RAM(COMBO_TIMER) = t;
    goto L_DE0E;
L_DE30:;
    mff_cycles(0xDE30, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCB07, 2); call_by_address_tail(0xCB07, -1); return;
}

/*  $DE0E - lifted by port/tools/lift.py */
void func_DE0E(void) {
    REGS_IN;
    mff_cycles(0xDE0E, 7);
    a = RAM(PLAYER_STAND_ANIM);
    m = 0x18;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xDE14, 2);
        if (!z) { mff_cycles(0xDE14, 2); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1DC4), -1); return; }
    } else {
        mff_cycles(0xDE12, 1);
    }
    mff_cycles(0xDE16, 8);
    a = 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1E1A, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDE1B, 8);
    a = 0x09;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1E1F, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDE20, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $DE33 - lifted by port/tools/lift.py */
void func_DE33(void) {
    REGS_IN;
    mff_cycles(0xDE33, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xDE46 + y));
    mff_cycles(0xDE38, 7);
    RAM(TMP2) = a;
    a = prg_read((uint16_t)(0xDE49 + y));
    mff_cycles(0xDE3D, 11);
    RAM(TMP3) = a;
    y = RAM(PLAYER_LEVEL);
    a = mem_read((uint16_t)(RAM16(TMP2) + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xDE43, 9);
    RAM(HEALTH) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/* player_state_transition $DE5E - lifted by port/tools/lift.py */
void func_DE5E(void) {
    REGS_IN;
    mff_cycles(0xDE5E, 35);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(VRAM_BUFFER_LEN) = a;
    RAM(VRAM_REQUEST) = a;
    RAM(HUD_ENEMY_SLOT) = a;
    RAM(SCROLL_X) = a;
    RAM(SCROLL_Y) = a;
    RAM(SCROLL_NT) = a;
    RAM(SCROLL_Y_HI) = a;
    RAM(SCROLL_X_HI) = a;
    RAM(RENDER_OFF_DEPTH) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1E74, 0xC36E, -1)) return;
    SYNC_IN;
    mff_cycles(0xDE75, 11);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OAM_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1E7B, 0xC31D, -1)) return;
    SYNC_IN;
    mff_cycles(0xDE7C, 6);
    SYNC_OUT;
    mff_call_fn(0x1E7E, func_FF31, 0xE000);
    SYNC_IN;
    mff_cycles(0xDE7F, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1E81, 0xC334, -1)) return;
    SYNC_IN;
    mff_cycles(0xDE82, 7);
    a = RAM(GAME_ACTION);
    m = 0x07;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xDE86, 1); goto L_DE8B; }
L_DE88:;
    mff_cycles(0xDE88, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xDF50, 2); call_by_address_tail(0xDF50, -1); return;
L_DE8B:;
    mff_cycles(0xDE8B, 17);
    t = (uint8_t)(RAM(AREA_INDEX) + 1);
    RAM(AREA_INDEX) = t;
    t = (uint8_t)(RAM(SUB_AREA) + 1);
    RAM(SUB_AREA) = t;
    a = RAM(SUB_AREA);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xDE88, 1); goto L_DE88; }
    mff_cycles(0xDE95, 17);
    a = 0x00;
    RAM(SUB_AREA) = a;
    t = (uint8_t)(RAM(ROUND) + 1);
    RAM(ROUND) = t;
    a = RAM(ROUND);
    m = 0x05;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xDE88, 1); goto L_DE88; }
    mff_cycles(0xDEA1, 8);
    a = 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1EA5, func_F34A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDEA6, 21);
    a = 0xFF;
    RAM(PAUSED) = a;
    RAM(GAME_FINISHED) = a;
    g_cpu.I = 0;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(IRQ_ENABLE) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1EB3, 0xC8A6, -1)) return;
    SYNC_IN;
    mff_cycles(0xDEB4, 8);
    x = 0x06;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x1EB8, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xDEB9, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1EBB, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xDEBC, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1EBE, 0x9000, -1)) return;
    SYNC_IN;
    mff_cycles(0xDEBF, 16);
    x = 0x00;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x1EC8, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDEC9, 11);
    a = 0x05;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(INTRO_PAGE) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1ECF, 0x9A00, -1)) return;
    SYNC_IN;
    mff_cycles(0xDED0, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(RENDER_OFF_DEPTH) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1ED6, 0xC36E, -1)) return;
    SYNC_IN;
    mff_cycles(0xDED7, 11);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OAM_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1EDD, 0xC31D, -1)) return;
    SYNC_IN;
    mff_cycles(0xDEDE, 6);
    SYNC_OUT;
    mff_call_fn(0x1EE0, func_FF31, 0xE000);
    SYNC_IN;
    mff_cycles(0xDEE1, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1EE3, 0xC334, -1)) return;
    SYNC_IN;
    mff_cycles(0xDEE4, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1EE6, 0xC8A6, -1)) return;
    SYNC_IN;
    mff_cycles(0xDEE7, 8);
    x = 0x0D;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x1EEB, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xDEEC, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1EEE, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xDEEF, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1EF1, 0x8C00, -1)) return;
    SYNC_IN;
    mff_cycles(0xDEF2, 16);
    x = 0x00;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x1EFB, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDEFC, 11);
    a = 0x06;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(INTRO_PAGE) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1F02, 0x9A00, -1)) return;
    SYNC_IN;
    mff_cycles(0xDF03, 6);
    a = RAM(CONTINUE_COUNT);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0xDF08, 11);
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(RENDER_OFF_DEPTH) = a;
        SYNC_OUT;
        if (!mff_call_at(0x1F0E, 0xC36E, -1)) return;
        SYNC_IN;
        mff_cycles(0xDF0F, 11);
        a = 0x04;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OAM_INDEX) = a;
        SYNC_OUT;
        if (!mff_call_at(0x1F15, 0xC31D, -1)) return;
        SYNC_IN;
        mff_cycles(0xDF16, 6);
        SYNC_OUT;
        mff_call_fn(0x1F18, func_FF31, 0xE000);
        SYNC_IN;
        mff_cycles(0xDF19, 6);
        SYNC_OUT;
        if (!mff_call_at(0x1F1B, 0xC334, -1)) return;
        SYNC_IN;
        mff_cycles(0xDF1C, 6);
        SYNC_OUT;
        if (!mff_call_at(0x1F1E, 0xC8A6, -1)) return;
        SYNC_IN;
        mff_cycles(0xDF1F, 6);
        SYNC_OUT;
        if (!mff_call_at(0x1F21, 0xC26F, -1)) return;
        SYNC_IN;
        mff_cycles(0xDF22, 6);
        SYNC_OUT;
        if (!mff_call_at(0x1F24, 0x9600, -1)) return;
        SYNC_IN;
        mff_cycles(0xDF25, 16);
        x = 0x08;
        RAM(BANK_REQUEST) = x;
        x++;
        n = (x) >> 7;
        z = (x) == 0;
        RAM(BANK_REQUEST + 1) = x;
        SYNC_OUT;
        if (!mff_call_fn(0x1F2E, func_FF4D, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xDF2F, 11);
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(INTRO_PAGE) = a;
        SYNC_OUT;
        if (!mff_call_at(0x1F35, 0xA700, -1)) return;
        SYNC_IN;
        mff_cycles(0xDF36, 11);
        t = (uint8_t)(RAM(INTRO_PAGE) + 1);
        RAM(INTRO_PAGE) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x1F3A, 0xA700, -1)) return;
        SYNC_IN;
        mff_cycles(0xDF3B, 11);
        t = (uint8_t)(RAM(INTRO_PAGE) + 1);
        RAM(INTRO_PAGE) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x1F3F, 0xA700, -1)) return;
        SYNC_IN;
        mff_cycles(0xDF40, 11);
        t = (uint8_t)(RAM(INTRO_PAGE) + 1);
        RAM(INTRO_PAGE) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x1F44, 0xA700, -1)) return;
        SYNC_IN;
        mff_cycles(0xDF45, 11);
        t = (uint8_t)(RAM(INTRO_PAGE) + 1);
        RAM(INTRO_PAGE) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x1F49, 0xA700, -1)) return;
        SYNC_IN;
        mff_cycles(0xDF4A, 6);
        SYNC_OUT;
        if (!mff_call_at(0x1F4C, 0xAB00, -1)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xDF06, 1);
    }
    mff_cycles(0xDF4D, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC8ED, 2); call_by_address_tail(0xC8ED, -1); return;
}

/*  $DF50 - lifted by port/tools/lift.py */
void func_DF50(void) {
    REGS_IN;
    mff_cycles(0xDF50, 7);
    a = RAM(GAME_ACTION);
    m = 0x07;
    c = a >= m;
    if (a != m) { mff_cycles(0xDF54, 1); goto L_DFA4; }
    mff_cycles(0xDF56, 27);
    a = 0x05;
    RAM(LIVES) = a;
    a = RAM(LEVEL_SAVED);
    RAM(PLAYER_LEVEL) = a;
    a = 0x00;
    RAM(SUB_AREA) = a;
    RAM(PLAYER_EXP) = a;
    RAM(PLAYER_EXP + 1) = a;
    a = RAM(ROUND);
    if (a != 0) { mff_cycles(0xDF68, 1); goto L_DF70; }
    mff_cycles(0xDF6A, 7);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(AREA_INDEX) = a;
    if (z) { mff_cycles(0xDF6E, 1); goto L_DF77; }
L_DF70:;
    mff_cycles(0xDF70, 7);
    y = RAM(ROUND);
    a = prg_read((uint16_t)(0xE161 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xDF75, 3);
    RAM(AREA_INDEX) = a;
L_DF77:;
    mff_cycles(0xDF77, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1F79, func_E12F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDF7A, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1F7C, 0xC85F, -1)) return;
    SYNC_IN;
    mff_cycles(0xDF7D, 16);
    x = 0x04;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x1F86, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDF87, 8);
    x = 0x02;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x1F8B, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xDF8C, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1F8E, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xDF8F, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1F91, 0x9100, -1)) return;
    SYNC_IN;
    mff_cycles(0xDF92, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1F94, 0xC85C, -1)) return;
    SYNC_IN;
    mff_cycles(0xDF95, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1F97, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xDF98, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1F9A, func_E143, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDF9B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1F9D, func_E139, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xDF9E, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1FA0, 0xDE33, -1)) return;
    SYNC_IN;
    mff_cycles(0xDFA1, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xDFE9, 2); call_by_address_tail(0xDFE9, -1); return;
L_DFA4:;
    mff_cycles(0xDFA4, 5);
    a = RAM(SUB_AREA);
    if (a == 0) {
        mff_cycles(0xDFA8, 12);
        a = RAM(PLAYER_LEVEL);
        n = (a) >> 7;
        z = (a) == 0;
        RAM(LEVEL_SAVED) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x1FAE, func_E12F, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xDFAF, 6);
        SYNC_OUT;
        if (!mff_call_at(0x1FB1, 0xC334, -1)) return;
        SYNC_IN;
        mff_cycles(0xDFB2, 6);
        SYNC_OUT;
        if (!mff_call_at(0x1FB4, 0xC85F, -1)) return;
        SYNC_IN;
        mff_cycles(0xDFB5, 16);
        x = 0x04;
        RAM(BANK_REQUEST) = x;
        x++;
        n = (x) >> 7;
        z = (x) == 0;
        RAM(BANK_REQUEST + 1) = x;
        SYNC_OUT;
        if (!mff_call_fn(0x1FBE, func_FF4D, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xDFBF, 7);
        y = RAM(ROUND);
        a = prg_read((uint16_t)(0xE15C + y));
        mff_cycles(0xDFC4, 8);
        x = a;
        n = (x) >> 7;
        z = (x) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x1FC7, 0x8000, -1)) return;
        SYNC_IN;
        mff_cycles(0xDFC8, 6);
        SYNC_OUT;
        if (!mff_call_at(0x1FCA, 0xC26F, -1)) return;
        SYNC_IN;
        mff_cycles(0xDFCB, 6);
        SYNC_OUT;
        if (!mff_call_at(0x1FCD, 0x8D00, -1)) return;
        SYNC_IN;
        mff_cycles(0xDFCE, 8);
        x = 0x2D;
        n = (x) >> 7;
        z = (x) == 0;
        SYNC_OUT;
        mff_call_fn(0x1FD2, func_FF2A, 0xE000);
        SYNC_IN;
        mff_cycles(0xDFD3, 6);
        SYNC_OUT;
        if (!mff_call_at(0x1FD5, 0xC85C, -1)) return;
        SYNC_IN;
        mff_cycles(0xDFD6, 6);
        SYNC_OUT;
        if (!mff_call_at(0x1FD8, 0xC26F, -1)) return;
        SYNC_IN;
        mff_cycles(0xDFD9, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1FDB, func_E143, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xDFDC, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1FDE, func_E139, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xDFDF, 6);
        SYNC_OUT;
        if (!mff_call_at(0x1FE1, 0xDE33, -1)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xDFA6, 1);
    }
    mff_cycles(0xDFE2, 7);
    y = RAM(ROUND);
    a = prg_read((uint16_t)(0xE166 + y));
    mff_cycles(0xDFE7, 10);
    RAM(ROUND_TABLES) = a;
    y = RAM(ROUND);
    a = prg_read((uint16_t)(0xC83C + y));
    mff_cycles(0xDFEE, 5);
    m = RAM(MUSIC_CURRENT);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xDFF2, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1FF4, func_F34A, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xDFF0, 1);
    }
    mff_cycles(0xDFF5, 10);
    a = 0xFF;
    RAM(BG_SCREEN) = a;
    a = 0x1F;
    RAM(BG_COLUMN) = a;
    for (;;) {
        mff_cycles(0xDFFD, 11);
        a = 0x01;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(TMP16) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x0003, func_E521, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xE004, 6);
        SYNC_OUT;
        mff_call_fn(0x0006, func_FF31, 0xE000);
        SYNC_IN;
        mff_cycles(0xE007, 5);
        a = RAM(BG_SCREEN);
        if (a != 0) break;
        mff_cycles(0xDFFD, 2);
    }
    mff_cycles(0xE00B, 16);
    x = 0x0A;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0014, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE015, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0017, 0x9C00, -1)) return;
    SYNC_IN;
    mff_cycles(0xE018, 7);
    y = RAM(ROUND);
    a = prg_read((uint16_t)(0xC1F8 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xE01D, 9);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0021, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE022, 7);
    a = RAM(SUB_AREA);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0xE026, 1); goto L_E032; }
    mff_cycles(0xE028, 4);
    m = 0x02;
    c = a >= m;
    if (a == m) { mff_cycles(0xE02A, 1); goto L_E038; }
    mff_cycles(0xE02C, 4);
    a = prg_read(0xAE68);
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xE02F, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xE03B, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_E03B(); g_code_window_base = swb; } return;
L_E032:;
    mff_cycles(0xE032, 4);
    a = prg_read(0xAE98);
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xE035, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xE03B, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_E03B(); g_code_window_base = swb; } return;
L_E038:;
    mff_cycles(0xE038, 4);
    a = prg_read(0xAEC8);
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

/*  $DFE9 - lifted by port/tools/lift.py */
void func_DFE9(void) {
    REGS_IN;
    mff_cycles(0xDFE9, 7);
    y = RAM(ROUND);
    a = prg_read((uint16_t)(0xC83C + y));
    mff_cycles(0xDFEE, 5);
    m = RAM(MUSIC_CURRENT);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xDFF2, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1FF4, func_F34A, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xDFF0, 1);
    }
    mff_cycles(0xDFF5, 10);
    a = 0xFF;
    RAM(BG_SCREEN) = a;
    a = 0x1F;
    RAM(BG_COLUMN) = a;
    for (;;) {
        mff_cycles(0xDFFD, 11);
        a = 0x01;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(TMP16) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x0003, func_E521, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xE004, 6);
        SYNC_OUT;
        mff_call_fn(0x0006, func_FF31, 0xE000);
        SYNC_IN;
        mff_cycles(0xE007, 5);
        a = RAM(BG_SCREEN);
        if (a != 0) break;
        mff_cycles(0xDFFD, 2);
    }
    mff_cycles(0xE00B, 16);
    x = 0x0A;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0014, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE015, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0017, 0x9C00, -1)) return;
    SYNC_IN;
    mff_cycles(0xE018, 7);
    y = RAM(ROUND);
    a = prg_read((uint16_t)(0xC1F8 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xE01D, 9);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0021, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE022, 7);
    a = RAM(SUB_AREA);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0xE026, 1); goto L_E032; }
    mff_cycles(0xE028, 4);
    m = 0x02;
    c = a >= m;
    if (a == m) { mff_cycles(0xE02A, 1); goto L_E038; }
    mff_cycles(0xE02C, 4);
    a = prg_read(0xAE68);
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xE02F, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xE03B, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_E03B(); g_code_window_base = swb; } return;
L_E032:;
    mff_cycles(0xE032, 4);
    a = prg_read(0xAE98);
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xE035, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xE03B, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_E03B(); g_code_window_base = swb; } return;
L_E038:;
    mff_cycles(0xE038, 4);
    a = prg_read(0xAEC8);
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
