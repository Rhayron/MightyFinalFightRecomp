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

/*  $80AE (bank 0) - lifted by port/tools/lift.py */
void func_80AE_b0(void) {
    REGS_IN;
    mff_cycles(0x80AE, 8);
    a = 0x1C;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x00B2, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x80B3, 24);
    a = 0x00;
    RAM(SCROLL_Y) = a;
    RAM(JUMP_MOVING) = a;
    a = 0x02;
    RAM(GAME_ACTION) = a;
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ACTION_TIMER) = a;
    SYNC_OUT;
    if (!mff_call_at(0x00C3, 0xCEAA, 0)) return;
    SYNC_IN;
    mff_cycles(0x80C4, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $80C5 (bank 0) - lifted by port/tools/lift.py */
void func_80C5_b0(void) {
    REGS_IN;
    mff_cycles(0x80C5, 13);
    a = 0x00;
    RAM(COMBO_TIMER) = a;
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    if (a != 0) { mff_cycles(0x80CE, 2); goto L_810E; }
    mff_cycles(0x80D0, 13);
    a = 0x18;
    RAM(OBJ_TIMER) = a;
    y = RAM(PLAYER_LEVEL);
    a = prg_read((uint16_t)(0x817A + y));
    mff_cycles(0x80DA, 12);
    RAM(OBJ_TIMER + 1) = a;
    a = 0x30;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x80E2, 2); goto L_810E; }
    mff_cycles(0x80E4, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x00E6, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x80E7, 8);
    a = 0x0B;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x00EB, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x80EC, 25);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    a = 0x00;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x03;
    RAM(OBJ_VX) = a;
    a = RAM(PLAYER_DIR);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x80FD, 2); goto L_8106; }
    mff_cycles(0x80FF, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0101, func_F048, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8102, 4);
    a = 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x8104, 1); goto L_810B; }
L_8106:;
    mff_cycles(0x8106, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0108, func_F03F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8109, 2);
    a = 0x01;
L_810B:;
    mff_cycles(0x810B, 4);
    RAM(OBJ_DIR) = a;
L_810E:;
    mff_cycles(0x810E, 6);
    a = RAM(OBJ_TIMER);
    if (a != 0) {
        mff_cycles(0x8113, 12);
        t = (uint8_t)(RAM(OBJ_TIMER) - 1);
        RAM(OBJ_TIMER) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x8111, 1);
    }
    mff_cycles(0x8117, 8);
    a = 0x3D;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0x811E, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0120, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x8121, 8);
        a = 0x0A;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0125, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x8126, 8);
        a = 0x2C;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x012A, func_F34C, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x812B, 11);
        a = 0x1A;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(ATTACK_ID) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x0131, func_F43B, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x811C, 1);
    }
    mff_cycles(0x8132, 8);
    a = RAM(OBJ_DIR);
    a &= 0x01;
    if (a != 0) { mff_cycles(0x8137, 1); goto L_814B; }
    mff_cycles(0x8139, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x013D, func_F000, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x813E, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    m = 0xF0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x8146, 1); goto L_815A; }
    mff_cycles(0x8148, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8177, 2); call_by_address_tail(0x8177, 0); return;
L_814B:;
    mff_cycles(0x814B, 8);
    y = 0x01;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x014F, func_F051, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8150, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    m = 0x18;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x8158, 1); goto L_8177; }
L_815A:;
    mff_cycles(0x815A, 6);
    a = RAM(OBJ_TIMER + 1);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0x815F, 12);
        t = (uint8_t)(RAM(OBJ_TIMER + 1) - 1);
        RAM(OBJ_TIMER + 1) = t;
        a = RAM(OBJ_STATE + 15);
        n = (a) >> 7;
        z = (a) == 0;
        if (n) {
            mff_cycles(0x8167, 8);
            a = RAM(OBJ_ANIM + 15);
            m = 0x7E;
            c = a >= m;
            n = ((uint8_t)(a - m)) >> 7;
            z = ((uint8_t)(a - m)) == 0;
            if (z) {
                mff_cycles(0x816E, 12);
                a = 0x00;
                n = (a) >> 7;
                z = (a) == 0;
                RAM(OBJ_TIMER + 1) = a;
                SYNC_OUT;
                if (!mff_call_fn(0x0175, func_F436, 0xE000)) return;
                SYNC_IN;
            } else {
                mff_cycles(0x816C, 1);
            }
        } else {
            mff_cycles(0x8165, 1);
        }
        mff_cycles(0x8176, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x815D, 1);
    }
L_8177:;
    mff_cycles(0x8177, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8F82, 2); call_by_address_tail(0x8F82, 0); return;
}

/*  $817E (bank 0) - lifted by port/tools/lift.py */
void func_817E_b0(void) {
    REGS_IN;
    mff_cycles(0x817E, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0x818D + y));
    mff_cycles(0x8183, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0x8190 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8188, 8);
    RAM(TMP1) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $86FF (bank 0) - lifted by port/tools/lift.py */
void func_86FF_b0(void) {
    REGS_IN;
    mff_cycles(0x86FF, 8);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    if (a == 0) {
        mff_cycles(0x8706, 16);
        RAM(OBJ_VX_FRAC) = a;
        a = 0x01;
        RAM(OBJ_VX) = a;
        t = (uint8_t)(RAM(OBJ_STATE) + 1);
        RAM(OBJ_STATE) = t;
    } else {
        mff_cycles(0x8704, 1);
    }
    mff_cycles(0x8711, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x06;
    c = a >= m;
    if (c) {
        mff_cycles(0x8718, 7);
        a = RAM(JOY_HELD);
        m = 0x40;
        c = a >= m;
        if (a == m) {
            mff_cycles(0x871E, 12);
            a = 0x33;
            RAM(OBJ_VX_FRAC) = a;
            a = 0x02;
            RAM(OBJ_VX) = a;
        } else {
            mff_cycles(0x871C, 1);
        }
    } else {
        mff_cycles(0x8716, 1);
    }
    mff_cycles(0x8728, 10);
    a = RAM(OBJ_ANIM_FRAME);
    y = a;
    a = prg_read((uint16_t)(0x8903 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x872F, 9);
    RAM(ATTACK_ID) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0733, func_F43B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8734, 7);
    a = RAM(DRAW_PARITY);
    a &= 0x07;
    if (a == 0) {
        mff_cycles(0x873A, 8);
        a = 0x13;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x073E, func_F34C, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x8738, 1);
    }
    mff_cycles(0x873F, 14);
    a = RAM(PLAYER_GROUND_Y);
    RAM(ATTACK_GROUND_Y) = a;
    a = RAM(OBJ_ANIM_TIMER);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x874A, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x074C, func_F436, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x8748, 1);
    }
    mff_cycles(0x874D, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x13;
    c = a >= m;
    if (a == m) {
        mff_cycles(0x8754, 8);
        a = RAM(OBJ_ANIM_TIMER);
        m = 0x04;
        c = a >= m;
        if (a == m) {
            mff_cycles(0x875B, 24);
            a = 0x00;
            RAM(GAME_ACTION) = a;
            RAM(ATTACK_LIVE) = a;
            RAM(COMBO_TIMER) = a;
            a = 0x80;
            RAM(OBJ_STATE) = a;
            y = RAM(PLAYER_ID);
            a = prg_read((uint16_t)(0xE288 + y));
            n = (a) >> 7;
            z = (a) == 0;
            mff_cycles(0x876D, 9);
            RAM(PLAYER_STAND_ANIM) = a;
            SYNC_OUT;
            if (!mff_call_at(0x0771, 0xCEAA, 0)) return;
            SYNC_IN;
            mff_cycles(0x8772, 5);
            a = RAM(SPECIAL_HIT);
            if (a != 0) {
                mff_cycles(0x8776, 12);
                a = RAM(HEALTH);
                c = 1;
                m = 0x06;
                w = a - m - !c;
                v = ((a ^ m) & (a ^ w) & 0x80) != 0;
                c = w >= 0;
                a = (uint8_t)w;
                RAM(HEALTH) = a;
                if (a & 0x80) {
                    mff_cycles(0x877F, 5);
                    a = 0x00;
                    RAM(HEALTH) = a;
                } else {
                    mff_cycles(0x877D, 1);
                }
            } else {
                mff_cycles(0x8774, 1);
            }
            mff_cycles(0x8783, 21);
            a = 0x00;
            RAM(SPECIAL_HIT) = a;
            RAM(OBJ_VX_FRAC) = a;
            a = 0x01;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_VX) = a;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0x8759, 1);
        }
    } else {
        mff_cycles(0x8752, 1);
    }
    mff_cycles(0x8790, 8);
    a = RAM(OBJ_STATE);
    a &= 0x02;
    if (a == 0) {
        mff_cycles(0x8797, 7);
        a = RAM(PLAYER_DIR);
        a &= 0x01;
        if (a != 0) {
            mff_cycles(0x879D, 8);
            y = 0x00;
            n = (y) >> 7;
            z = (y) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x07A1, func_F000, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x87A2, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x87AA, 2); call_by_address_tail(0x87AA, 0); return;
        } else {
            mff_cycles(0x879B, 1);
        }
        mff_cycles(0x87A5, 8);
        y = 0x01;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x07A9, func_F051, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x87AA, 7);
        a = RAM(GAME_ACTION);
        m = 0x16;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) {
            mff_cycles(0x87B0, 5);
            a = RAM(JOY_HELD);
            n = (a) >> 7;
            z = (a) == 0;
            if (!z) {
                mff_cycles(0x87B4, 6);
                t = (uint8_t)(RAM(OBJ_STATE) + 1);
                RAM(OBJ_STATE) = t;
                n = (t) >> 7;
                z = (t) == 0;
            } else {
                mff_cycles(0x87B2, 1);
            }
        } else {
            mff_cycles(0x87AE, 1);
        }
        mff_cycles(0x87B7, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x8795, 1);
    }
    mff_cycles(0x87B8, 7);
    a = RAM(JOY_HELD);
    a &= 0x03;
    if (a != 0) {
        mff_cycles(0x87BE, 7);
        RAM(PLAYER_DIR) = a;
        a &= 0x01;
        if (a != 0) {
            mff_cycles(0x87C4, 8);
            y = 0x00;
            n = (y) >> 7;
            z = (y) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x07C8, func_F000, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x87C9, 6);
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0x87C2, 1);
        }
        mff_cycles(0x87CA, 8);
        y = 0x01;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x07CE, func_F051, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x87CF, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x87BC, 1);
    }
    mff_cycles(0x87D0, 7);
    a = RAM(PLAYER_DIR);
    a &= 0x01;
    if (a != 0) {
        mff_cycles(0x87D6, 8);
        y = 0x00;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x07DA, func_F000, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x87DB, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x87D4, 1);
    }
    mff_cycles(0x87DC, 8);
    y = 0x01;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x07E0, func_F051, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x87E1, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $891B (bank 0) - lifted by port/tools/lift.py */
void func_891B_b0(void) {
    REGS_IN;
    mff_cycles(0x891B, 8);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    if (a == 0) {
        mff_cycles(0x8922, 28);
        RAM(OBJ_VY_FRAC) = a;
        RAM(OBJ_VX) = a;
        a = 0x03;
        RAM(OBJ_VY) = a;
        a = 0xE6;
        RAM(OBJ_VX_FRAC) = a;
        a = 0x1A;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0936, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x8937, 8);
        a = 0x06;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x093B, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x893C, 20);
        a = 0x07;
        RAM(OBJ_TIMER) = a;
        a = RAM(OBJ_Y);
        RAM(OBJ_TIMER2) = a;
        t = (uint8_t)(RAM(OBJ_STATE) + 1);
        RAM(OBJ_STATE) = t;
    } else {
        mff_cycles(0x8920, 1);
    }
    mff_cycles(0x894A, 8);
    a = RAM(OBJ_STATE);
    a &= 0x02;
    if (a != 0) { mff_cycles(0x894F, 1); goto L_89B9; }
    mff_cycles(0x8951, 8);
    a = RAM(OBJ_ANIM);
    m = 0x69;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0x8958, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x095A, func_F1FD, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x895B, 6);
        a = RAM(OBJ_VY);
        if (a == 0) {
            mff_cycles(0x8960, 11);
            a = 0x18;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(ATTACK_ID) = a;
            SYNC_OUT;
            if (!mff_call_fn(0x0966, func_F43B, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x8967, 14);
            a = RAM(PLAYER_GROUND_Y);
            RAM(ATTACK_GROUND_Y) = a;
            a = 0x34;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x096F, func_F34C, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x8970, 8);
            a = 0x69;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0974, func_F294, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x8975, 12);
            a = 0x00;
            RAM(OBJ_VX_FRAC) = a;
            a = 0x05;
            RAM(OBJ_VX) = a;
        } else {
            mff_cycles(0x895E, 1);
        }
    } else {
        mff_cycles(0x8956, 1);
    }
    mff_cycles(0x897F, 7);
    a = RAM(PLAYER_DIR);
    a &= 0x01;
    if (a != 0) {
        mff_cycles(0x8985, 8);
        y = 0x00;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0989, func_F000, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x898A, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x8992, 2); call_by_address_tail(0x8992, 0); return;
    } else {
        mff_cycles(0x8983, 1);
    }
    mff_cycles(0x898D, 8);
    y = 0x01;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0991, func_F051, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8992, 8);
    a = RAM(OBJ_ANIM);
    m = 0x69;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8997, 1); goto L_89F5; }
    mff_cycles(0x8999, 6);
    a = RAM(OBJ_TIMER);
    if (a != 0) {
        mff_cycles(0x899E, 12);
        t = (uint8_t)(RAM(OBJ_TIMER) - 1);
        RAM(OBJ_TIMER) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x899C, 1);
    }
    mff_cycles(0x89A2, 30);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    a = 0x00;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x03;
    RAM(OBJ_VX) = a;
    a = 0xC0;
    RAM(OBJ_VY_FRAC) = a;
    a = 0xFF;
    RAM(OBJ_VY) = a;
L_89B9:;
    mff_cycles(0x89B9, 14);
    a = RAM(OBJ_TIMER2);
    c = 0;
    m = 0x02;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    m = RAM(OBJ_Y);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x89C2, 1); goto L_89C9; }
    mff_cycles(0x89C4, 2);
    if (z) { mff_cycles(0x89C4, 1); goto L_89F6; }
    mff_cycles(0x89C6, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x89F6, 2); call_by_address_tail(0x89F6, 0); return;
L_89C9:;
    mff_cycles(0x89C9, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x09CD, func_F119, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x89CE, 2);
    if (c) {
        mff_cycles(0x89D0, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x89F6, 2); call_by_address_tail(0x89F6, 0); return;
    } else {
        mff_cycles(0x89CE, 1);
    }
    mff_cycles(0x89D3, 8);
    a = RAM(OBJ_VY);
    m = 0xFD;
    c = a >= m;
    if (a == m) {
        mff_cycles(0x89DA, 8);
        a = 0x38;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x09DE, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x89DF, 8);
        a = 0x05;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x09E3, func_F440, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x89D8, 1);
    }
    mff_cycles(0x89E4, 7);
    a = RAM(PLAYER_DIR);
    a &= 0x01;
    if (a != 0) {
        mff_cycles(0x89EA, 8);
        y = 0x00;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x09EE, func_F000, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x89EF, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x89E8, 1);
    }
    mff_cycles(0x89F0, 8);
    y = 0x01;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x09F4, func_F051, 0xE000)) return;
    SYNC_IN;
L_89F5:;
    mff_cycles(0x89F5, 6);
    SYNC_OUT;
    mff_rts(); return;
L_89F6:;
    mff_cycles(0x89F6, 16);
    a = RAM(OBJ_TIMER2);
    RAM(OBJ_Y) = a;
    a = 0x2F;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0A00, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8A01, 8);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0A05, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8A06, 38);
    a = 0xC0;
    RAM(OBJ_VY_FRAC) = a;
    a = 0xFF;
    RAM(OBJ_VY) = a;
    a = 0x33;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VX) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(PLAYER_NO_HIT) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0A22, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8A23, 22);
    a = 0x22;
    RAM(GAME_ACTION) = a;
    a = 0x1E;
    RAM(ACTION_TIMER) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8A31 (bank 0) - lifted by port/tools/lift.py */
void func_8A31_b0(void) {
    REGS_IN;
    mff_cycles(0x8A31, 8);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    if (a != 0) { mff_cycles(0x8A36, 1); goto L_8A71; }
    mff_cycles(0x8A38, 5);
    a = RAM(ACTION_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x8A3A, 1); goto L_8A6E; }
    mff_cycles(0x8A3C, 12);
    t = (uint8_t)(RAM(ACTION_TIMER) - 1);
    RAM(ACTION_TIMER) = t;
    a = RAM(JOY_HELD);
    a &= 0x04;
    if (a != 0) {
        mff_cycles(0x8A44, 5);
        t = (uint8_t)(RAM(GUY_JUMP_WINDOW) + 1);
        RAM(GUY_JUMP_WINDOW) = t;
    } else {
        mff_cycles(0x8A42, 1);
    }
    mff_cycles(0x8A46, 7);
    a = RAM(GUY_JUMP_WINDOW);
    m = 0x10;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x8A4C, 7);
        a = RAM(JOY_HELD);
        m = 0x48;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) {
            mff_cycles(0x8A52, 14);
            t = (uint8_t)(RAM(OBJ_STATE) + 1);
            RAM(OBJ_STATE) = t;
            a = 0x14;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0A59, func_F294, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x8A5A, 8);
            a = 0x07;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0A5E, func_F440, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x8A5F, 17);
            a = 0x33;
            RAM(OBJ_VY_FRAC) = a;
            a = 0x01;
            RAM(OBJ_VY) = a;
            a = 0xFF;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(PLAYER_NO_HIT) = a;
        } else {
            mff_cycles(0x8A50, 1);
        }
    } else {
        mff_cycles(0x8A4A, 1);
    }
L_8A6D:;
    mff_cycles(0x8A6D, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8A6E:;
    mff_cycles(0x8A6E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8AD3, 2); call_by_address_tail(0x8AD3, 0); return;
L_8A71:;
    mff_cycles(0x8A71, 8);
    a = RAM(OBJ_STATE);
    a &= 0x02;
    if (a != 0) { mff_cycles(0x8A76, 1); goto L_8AB3; }
    mff_cycles(0x8A78, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x04;
    c = a >= m;
    if (a == m) {
        mff_cycles(0x8A7F, 6);
        a = RAM(OBJ_ANIM_TIMER);
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0x8A84, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0A86, func_F43B, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x8A87, 11);
            a = 0x01;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(ATTACK_ID) = a;
            SYNC_OUT;
            if (!mff_call_fn(0x0A8D, func_F430, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0x8A82, 1);
        }
    } else {
        mff_cycles(0x8A7D, 1);
    }
    mff_cycles(0x8A8E, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x06;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8A6D, 1); goto L_8A6D; }
    mff_cycles(0x8A95, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0A97, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8A98, 8);
    a = 0x5B;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0A9C, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8A9D, 8);
    a = 0x05;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0AA1, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8AA2, 25);
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
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
L_8AB3:;
    mff_cycles(0x8AB3, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0AB7, func_F119, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8AB8, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0x8ABB, 1); goto L_8AFF; }
    mff_cycles(0x8ABD, 13);
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
    if (!c) { mff_cycles(0x8AC5, 1); goto L_8AFF; }
    mff_cycles(0x8AC7, 2);
    if (z) { mff_cycles(0x8AC7, 1); goto L_8AD0; }
    mff_cycles(0x8AC9, 8);
    m = 0x01;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_Y) = a;
    if (a != 0) { mff_cycles(0x8ACE, 1); goto L_8AD3; }
L_8AD0:;
    mff_cycles(0x8AD0, 4);
    RAM(OBJ_Y) = a;
L_8AD3:;
    mff_cycles(0x8AD3, 50);
    a = 0xC0;
    RAM(OBJ_VY_FRAC) = a;
    a = 0xFF;
    RAM(OBJ_VY) = a;
    a = 0x80;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VX) = a;
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
    if (!mff_call_at(0x0AF7, 0xCEAA, 0)) return;
    SYNC_IN;
    mff_cycles(0x8AF8, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8AFD, 3);
    RAM(PLAYER_STAND_ANIM) = a;
L_8AFF:;
    mff_cycles(0x8AFF, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8CC4 (bank 0) - lifted by port/tools/lift.py */
void func_8CC4_b0(void) {
    REGS_IN;
    mff_cycles(0x8CC4, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(COMBO_TIMER) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0CCA, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8CCB, 8);
    a = 0x73;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0x8CD2, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0CD4, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x8CD5, 8);
        a = 0x3F;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CD9, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x8CDA, 8);
        a = 0x16;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CDE, func_F34C, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x8CD0, 1);
    }
    mff_cycles(0x8CDF, 8);
    a = RAM(OBJ_ANIM_FRAME);
    if (a < 0x05) {
        mff_cycles(0x8CE6, 7);
        a = RAM(JOY_HELD);
        m = 0x48;
        c = a >= m;
        if (a == m) {
            mff_cycles(0x8CEC, 8);
            a = 0x13;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0CF0, func_F294, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x8CF1, 8);
            a = 0x03;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0CF5, func_F440, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x8CF6, 32);
            a = 0x14;
            RAM(GAME_ACTION) = a;
            a = 0x00;
            RAM(ACTION_TIMER) = a;
            a = 0x80;
            RAM(OBJ_STATE) = a;
            RAM(PLAYER_NO_HIT) = a;
            a = RAM(OBJ_Y);
            n = (a) >> 7;
            z = (a) == 0;
            RAM(PLAYER_GROUND_Y) = a;
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0x8CEA, 2);
        }
    } else {
        mff_cycles(0x8CE4, 2);
    }
    mff_cycles(0x8D0B, 8);
    a = RAM(OBJ_ANIM_FRAME);
    if (a != 0x10) { mff_cycles(0x8D10, 1); goto L_8D6F; }
    mff_cycles(0x8D12, 6);
    a = RAM(OBJ_ANIM_TIMER);
    if (a != 0) { mff_cycles(0x8D15, 1); goto L_8D6F; }
    mff_cycles(0x8D17, 2);
    y = 0x02;
L_8D19:;
    mff_cycles(0x8D19, 6);
    a = RAM(OBJ_STATE + y);
    if (!(a & 0x80)) { mff_cycles(0x8D1C, 1); goto L_8D24; }
    mff_cycles(0x8D1E, 6);
    y--;
    m = 0x00;
    c = y >= m;
    n = ((uint8_t)(y - m)) >> 7;
    z = ((uint8_t)(y - m)) == 0;
    if (!z) { mff_cycles(0x8D19, 1); goto L_8D19; }
    mff_cycles(0x8D23, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8D24:;
    mff_cycles(0x8D24, 25);
    RAM(TMP15) = y;
    a = RAM(PLAYER_DIR);
    RAM(OBJ_DIR + y) = a;
    a &= 0x02;
    y = a;
    a = RAM(OBJ_X);
    c = 0;
    m = prg_read((uint16_t)(0x8D89 + y));
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0x8D35, 11);
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = prg_read((uint16_t)(0x8D8A + y));
    w = a + m + c;
    a = (uint8_t)w;
    mff_cycles(0x8D3C, 47);
    y = RAM(TMP15);
    RAM(OBJ_X_HI + y) = a;
    a = mff_pull();
    RAM(OBJ_X + y) = a;
    a = RAM(OBJ_Y);
    c = 0;
    m = 0x04;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_Y + y) = a;
    a = RAM(OBJ_Y_HI);
    RAM(OBJ_Y_HI + y) = a;
    a = 0x74;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0D58, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8D59, 31);
    a = 0x6C;
    RAM(OBJ_ID + y) = a;
    RAM(OBJ_SAVED_ID + y) = a;
    a = 0x22;
    RAM(ATTACK_ID) = a;
    a = 0x01;
    RAM(OBJ_ATTACK + y) = a;
    a = 0x00;
    RAM(OBJ_THROW_FLAG + y) = a;
L_8D6F:;
    mff_cycles(0x8D6F, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x1B;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x8D76, 18);
        a = 0x00;
        RAM(GAME_ACTION) = a;
        RAM(GUY_JUMP_WINDOW) = a;
        RAM(JUMP_MOVING) = a;
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(0xE288 + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x8D83, 9);
        RAM(PLAYER_STAND_ANIM) = a;
        SYNC_OUT;
        if (!mff_call_at(0x0D87, 0xCEAA, 0)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x8D74, 1);
    }
    mff_cycles(0x8D88, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8EEA (bank 0) - lifted by port/tools/lift.py */
void func_8EEA_b0(void) {
    REGS_IN;
    mff_cycles(0x8EEA, 13);
    a = 0x00;
    RAM(COMBO_TIMER) = a;
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    if (a == 0) {
        mff_cycles(0x8EF5, 11);
        RAM(GUY_JUMP_WINDOW) = a;
        a = 0x24;
        m = RAM(OBJ_ANIM);
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0x8EFE, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0F00, func_F294, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x8F01, 8);
            a = 0x32;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0F05, func_F440, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x8F06, 14);
            t = (uint8_t)(RAM(OBJ_STATE) + 1);
            RAM(OBJ_STATE) = t;
            a = 0x1B;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0F0D, func_F34C, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0x8EFC, 2);
        }
    } else {
        mff_cycles(0x8EF3, 2);
    }
    mff_cycles(0x8F0E, 7);
    a = RAM(JOY_HELD);
    if (a == 0x08) {
        mff_cycles(0x8F14, 5);
        t = (uint8_t)(RAM(GUY_JUMP_WINDOW) + 1);
        RAM(GUY_JUMP_WINDOW) = t;
    } else {
        mff_cycles(0x8F12, 1);
    }
    mff_cycles(0x8F16, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x04;
    c = a >= m;
    if (a == m) {
        mff_cycles(0x8F1D, 11);
        a = 0x06;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(ATTACK_ID) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x0F23, func_F43B, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x8F1B, 1);
    }
    mff_cycles(0x8F24, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x06;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x8F2B, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0F2D, func_F436, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x8F29, 1);
    }
    mff_cycles(0x8F2E, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x0B;
    c = a >= m;
    if (a == m) {
        mff_cycles(0x8F35, 11);
        a = 0x03;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(ATTACK_ID) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x0F3B, func_F43B, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x8F3C, 15);
        a = RAM(OBJ_Y);
        RAM(ATTACK_GROUND_Y) = a;
        a = 0x34;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0F45, func_F34C, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x8F33, 1);
    }
    mff_cycles(0x8F46, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x11;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x8F4D, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0F4F, func_F436, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x8F4B, 1);
    }
    mff_cycles(0x8F50, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x13;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8F55, 1); goto L_8FAE; }
    mff_cycles(0x8F57, 7);
    a = RAM(GUY_JUMP_WINDOW);
    m = 0x28;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x8F5B, 1); goto L_8F82; }
    mff_cycles(0x8F5D, 16);
    a = 0x81;
    RAM(OBJ_STATE) = a;
    RAM(PLAYER_NO_HIT) = a;
    a = RAM(PLAYER_DIR);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x8F68, 1); goto L_8F6F; }
    mff_cycles(0x8F6A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0F6C, func_F048, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8F6D, 2);
    if (!z) { mff_cycles(0x8F6D, 1); goto L_8F72; }
L_8F6F:;
    mff_cycles(0x8F6F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0F71, func_F03F, 0xE000)) return;
    SYNC_IN;
L_8F72:;
    mff_cycles(0x8F72, 14);
    a = RAM(PLAYER_DIR);
    a ^= 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PLAYER_DIR) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0F7A, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8F7B, 8);
    a = 0x22;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8A55, 2); call_by_address_tail(0x8A55, 0); return;
L_8F82:;
    mff_cycles(0x8F82, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0F84, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8F85, 32);
    a = 0x00;
    RAM(GAME_ACTION) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(JUMP_MOVING) = a;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER + 1) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE288 + y));
    mff_cycles(0x8F9D, 7);
    RAM(PLAYER_STAND_ANIM) = a;
    a = prg_read((uint16_t)(0xE1C1 + y));
    mff_cycles(0x8FA2, 8);
    RAM(OBJ_VX_FRAC) = a;
    a = prg_read((uint16_t)(0xE1C4 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8FA8, 10);
    RAM(OBJ_VX) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0FAD, 0xCEAA, 0)) return;
    SYNC_IN;
L_8FAE:;
    mff_cycles(0x8FAE, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8FF8 (bank 0) - lifted by port/tools/lift.py */
void func_8FF8_b0(void) {
    REGS_IN;
    mff_cycles(0x8FF8, 8);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    if (a != 0) { mff_cycles(0x8FFD, 2); goto L_9073; }
    mff_cycles(0x8FFF, 12);
    a = 0x00;
    RAM(CAMERA_AUTOSCROLL) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1C1 + y));
    mff_cycles(0x9008, 8);
    RAM(OBJ_VX_FRAC) = a;
    a = prg_read((uint16_t)(0xE1C4 + y));
    mff_cycles(0x900E, 8);
    RAM(OBJ_VX) = a;
    a = prg_read((uint16_t)(0xE1C7 + y));
    mff_cycles(0x9014, 8);
    RAM(OBJ_VY_FRAC) = a;
    a = prg_read((uint16_t)(0xE1CA + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x901A, 10);
    RAM(OBJ_VY) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x101F, func_F03F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9020, 5);
    a = RAM(WEAPON_HELD);
    if (a != 0) { mff_cycles(0x9022, 1); goto L_9035; }
    mff_cycles(0x9024, 8);
    a = RAM(OBJ_ANIM);
    m = prg_read((uint16_t)(0xE273 + y));
    c = a >= m;
    mff_cycles(0x902A, 2);
    if (a == m) { mff_cycles(0x902A, 1); goto L_9049; }
    mff_cycles(0x902C, 4);
    a = prg_read((uint16_t)(0xE1E6 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x902F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1031, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9032, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9043, 2); call_by_address_tail(0x9043, 0); return;
L_9035:;
    mff_cycles(0x9035, 8);
    a = RAM(OBJ_ANIM);
    m = prg_read((uint16_t)(0xE29A + y));
    c = a >= m;
    mff_cycles(0x903B, 2);
    if (a != m) {
        mff_cycles(0x903D, 4);
        a = prg_read((uint16_t)(0xE204 + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x9040, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1042, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9043, 4);
        a = prg_read((uint16_t)(0xE22B + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x9046, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1048, func_F440, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x903B, 1);
    }
L_9049:;
    mff_cycles(0x9049, 8);
    a = RAM(OBJ_Y);
    m = 0x7C;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0x9050, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1052, func_F1BF, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9053, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x904E, 1);
    }
    mff_cycles(0x9054, 8);
    a = RAM(OBJ_Y);
    m = 0x80;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x905B, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x105D, func_F1DF, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x905E, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9059, 1);
    }
    mff_cycles(0x905F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1061, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9062, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1064, func_F03F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9065, 18);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    a = 0x38;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_VY) = a;
L_9072:;
    mff_cycles(0x9072, 6);
    SYNC_OUT;
    mff_rts(); return;
L_9073:;
    mff_cycles(0x9073, 8);
    a = RAM(OBJ_STATE);
    a &= 0x02;
    if (a != 0) { mff_cycles(0x9078, 1); goto L_90C1; }
    mff_cycles(0x907A, 8);
    a = RAM(OBJ_X_HI);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x907F, 1); goto L_90BD; }
    mff_cycles(0x9081, 8);
    a = RAM(OBJ_X);
    m = 0x40;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x9086, 1); goto L_90BD; }
    mff_cycles(0x9088, 6);
    a = RAM(OBJ_VY);
    if (!(a & 0x80)) {
        mff_cycles(0x908D, 7);
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(0xE1E9 + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x9092, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1094, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9095, 4);
        a = prg_read((uint16_t)(0xE22E + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x9098, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x109A, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x909B, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x90AC, 2); call_by_address_tail(0x90AC, 0); return;
    } else {
        mff_cycles(0x908B, 1);
    }
    mff_cycles(0x909E, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE219 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x90A3, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x10A5, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x90A6, 4);
    a = prg_read((uint16_t)(0xE22E + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x90A9, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x10AB, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x90AC, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x10AE, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x90AF, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x10B1, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x90B2, 8);
    a = RAM(OBJ_Y);
    m = 0x7C;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x9072, 1); goto L_9072; }
    mff_cycles(0x90B9, 12);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_90BD:;
    mff_cycles(0x90BD, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x10BF, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x90C0, 6);
    SYNC_OUT;
    mff_rts(); return;
L_90C1:;
    mff_cycles(0x90C1, 17);
    a = 0x19;
    RAM(GAME_ACTION) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9072 (bank 0) - lifted by port/tools/lift.py */
void func_9072_b0(void) {
    REGS_IN;
    mff_cycles(0x9072, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $914D (bank 0) - lifted by port/tools/lift.py */
void func_914D_b0(void) {
    REGS_IN;
    mff_cycles(0x914D, 8);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    if (a == 0) {
        mff_cycles(0x9154, 8);
        a = RAM(OBJ_X);
        m = 0x80;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!c) {
            mff_cycles(0x915B, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x115D, func_F03F, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x915E, 9);
            a = 0x01;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(OBJ_DIR) = a;
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x916E, 2); call_by_address_tail(0x916E, 0); return;
        } else {
            mff_cycles(0x9159, 1);
        }
        mff_cycles(0x9166, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1168, func_F048, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9169, 12);
        a = 0x02;
        RAM(OBJ_DIR) = a;
        t = (uint8_t)(RAM(OBJ_STATE) + 1);
        RAM(OBJ_STATE) = t;
    } else {
        mff_cycles(0x9152, 1);
    }
    mff_cycles(0x9171, 8);
    a = RAM(OBJ_STATE);
    a &= 0x02;
    if (a != 0) { mff_cycles(0x9176, 1); goto L_91B3; }
    mff_cycles(0x9178, 8);
    a = RAM(OBJ_Y);
    m = 0x82;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0x917F, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1181, func_F1BF, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9182, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x917D, 1);
    }
    mff_cycles(0x9183, 8);
    a = RAM(OBJ_Y);
    m = 0x84;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x918A, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x118C, func_F1DF, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x918D, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9188, 1);
    }
    mff_cycles(0x918E, 8);
    a = RAM(OBJ_DIR);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x9193, 1); goto L_91A5; }
    mff_cycles(0x9195, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1197, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9198, 8);
    a = RAM(OBJ_X);
    m = 0x80;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x919D, 1); goto L_91B2; }
    mff_cycles(0x919F, 9);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x91B2, 2); call_by_address_tail(0x91B2, 0); return;
L_91A5:;
    mff_cycles(0x91A5, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x11A7, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x91A8, 8);
    a = RAM(OBJ_X);
    m = 0x80;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0x91AF, 6);
        t = (uint8_t)(RAM(OBJ_STATE) + 1);
        RAM(OBJ_STATE) = t;
        n = (t) >> 7;
        z = (t) == 0;
    } else {
        mff_cycles(0x91AD, 1);
    }
L_91B2:;
    mff_cycles(0x91B2, 6);
    SYNC_OUT;
    mff_rts(); return;
L_91B3:;
    mff_cycles(0x91B3, 19);
    a = 0x40;
    RAM(JOY_PRESSED) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(JUMP_MOVING) = a;
    RAM(COMBO_TIMER) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $91BE (bank 0) - lifted by port/tools/lift.py */
void func_91BE_b0(void) {
    REGS_IN;
    mff_cycles(0x91BE, 8);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    if (a == 0) {
        mff_cycles(0x91C5, 11);
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(CAMERA_AUTOSCROLL) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x11CB, func_F436, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x91CC, 11);
        y = RAM(PLAYER_ID);
        a = RAM(OBJ_ANIM);
        m = prg_read((uint16_t)(0xE273 + y));
        c = a >= m;
        mff_cycles(0x91D4, 2);
        if (a != m) {
            mff_cycles(0x91D6, 4);
            a = prg_read((uint16_t)(0xE1E6 + y));
            n = (a) >> 7;
            z = (a) == 0;
            mff_cycles(0x91D9, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x11DB, func_F294, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x91DC, 4);
            a = prg_read((uint16_t)(0xE22B + y));
            n = (a) >> 7;
            z = (a) == 0;
            mff_cycles(0x91DF, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x11E1, func_F440, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x91E2, 4);
            a = prg_read((uint16_t)(0xE1C1 + y));
            mff_cycles(0x91E5, 8);
            RAM(OBJ_VX_FRAC) = a;
            a = prg_read((uint16_t)(0xE1C4 + y));
            mff_cycles(0x91EB, 8);
            RAM(OBJ_VX) = a;
            a = prg_read((uint16_t)(0xE1C7 + y));
            mff_cycles(0x91F1, 8);
            RAM(OBJ_VY_FRAC) = a;
            a = prg_read((uint16_t)(0xE1CA + y));
            n = (a) >> 7;
            z = (a) == 0;
            mff_cycles(0x91F7, 10);
            RAM(OBJ_VY) = a;
            SYNC_OUT;
            if (!mff_call_fn(0x11FC, func_F03F, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0x91D4, 1);
        }
        mff_cycles(0x91FD, 8);
        a = RAM(OBJ_Y);
        m = 0x90;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!c) {
            mff_cycles(0x9204, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x1206, func_F1BF, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x9207, 6);
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0x9202, 1);
        }
        mff_cycles(0x9208, 8);
        a = RAM(OBJ_Y);
        m = 0x9C;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0x920F, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x1211, func_F1DF, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0x9212, 6);
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0x920D, 1);
        }
        mff_cycles(0x9213, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1215, func_F183, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9216, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1218, func_F03F, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9219, 24);
        t = (uint8_t)(RAM(OBJ_STATE) + 1);
        RAM(OBJ_STATE) = t;
        a = 0x38;
        RAM(OBJ_VY_FRAC) = a;
        a = 0x04;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_VY) = a;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x91C3, 2);
    }
    mff_cycles(0x9227, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x02;
    c = a >= m;
    if (a == m) { mff_cycles(0x922E, 1); goto L_9249; }
    mff_cycles(0x9230, 4);
    if (a == 0x03) { mff_cycles(0x9232, 1); goto L_9268; }
    mff_cycles(0x9234, 8);
    a = RAM(OBJ_X_HI);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0x923B, 8);
        a = RAM(OBJ_X);
        m = 0x88;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0x9242, 6);
            t = (uint8_t)(RAM(OBJ_STATE) + 1);
            RAM(OBJ_STATE) = t;
            n = (t) >> 7;
            z = (t) == 0;
        } else {
            mff_cycles(0x9240, 1);
        }
    } else {
        mff_cycles(0x9239, 1);
    }
    mff_cycles(0x9245, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1247, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9248, 6);
    SYNC_OUT;
    mff_rts(); return;
L_9249:;
    mff_cycles(0x9249, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE210 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x924E, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1250, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9251, 4);
    a = prg_read((uint16_t)(0xE258 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9254, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1256, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9257, 24);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    a = 0x6E;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_VY) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1266, 0x9297, 0)) return;
    SYNC_IN;
    mff_cycles(0x9267, 6);
    SYNC_OUT;
    mff_rts(); return;
L_9268:;
    mff_cycles(0x9268, 11);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    m = prg_read((uint16_t)(0x9294 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x9270, 2);
    if (!z) {
        mff_cycles(0x9272, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9270, 1);
    }
    mff_cycles(0x9273, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1275, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9276, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1278, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9279, 8);
    a = RAM(OBJ_Y);
    m = 0xA8;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x9280, 18);
        a = RAM(OBJ_FLAGS);
        a |= 0x20;
        RAM(OBJ_FLAGS) = a;
        a = RAM(OBJ_Y);
        m = 0xB0;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0x928F, 5);
            a = 0x0E;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(GAME_ACTION) = a;
        } else {
            mff_cycles(0x928D, 1);
        }
    } else {
        mff_cycles(0x927E, 1);
    }
    mff_cycles(0x9293, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $92BA (bank 0) - lifted by port/tools/lift.py */
void func_92BA_b0(void) {
    REGS_IN;
    mff_cycles(0x92BA, 5);
    a = RAM(ACTION_TIMER);
    if (!(a & 0x80)) {
        mff_cycles(0x92BE, 11);
        t = (uint8_t)(RAM(ACTION_TIMER) - 1);
        RAM(ACTION_TIMER) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x92BC, 1);
    }
    mff_cycles(0x92C1, 8);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0x92C8, 6);
        SYNC_OUT;
        if (!mff_call_at(0x12CA, 0xCEAA, 0)) return;
        SYNC_IN;
        mff_cycles(0x92CB, 12);
        a = 0x10;
        RAM(OBJ_TIMER) = a;
        t = (uint8_t)(RAM(OBJ_STATE) + 1);
        RAM(OBJ_STATE) = t;
    } else {
        mff_cycles(0x92C6, 1);
    }
    mff_cycles(0x92D3, 6);
    a = RAM(OBJ_TIMER);
    if (!(a & 0x80)) {
        mff_cycles(0x92D8, 12);
        t = (uint8_t)(RAM(OBJ_TIMER) - 1);
        RAM(OBJ_TIMER) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x92D6, 1);
    }
    mff_cycles(0x92DC, 11);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $93AF (bank 0) - lifted by port/tools/lift.py */
void func_93AF_b0(void) {
    REGS_IN;
    mff_cycles(0x93AF, 51);
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
    a = prg_read((uint16_t)(0x93EC + y));
    mff_cycles(0x93D6, 33);
    RAM(OBJ_ANIM + 1) = a;
    a = 0x00;
    RAM(OBJ_ANIM_TIMER + 1) = a;
    RAM(OBJ_ANIM_FRAME + 1) = a;
    a = RAM(PLAYER_DIR);
    RAM(OBJ_DIR + 1) = a;
    a = 0x67;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ID + 1) = a;
    SYNC_OUT;
    mff_rts(); return;
}

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

/* snd_start $8118 (bank 3) - lifted by port/tools/lift.py */
void func_8118_b3(void) {
    REGS_IN;
    mff_cycles(0x8118, 8);
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    x = a;
    y = prg_read((uint16_t)(0x8A44 + x));
    mff_cycles(0x811D, 6);
    a = y;
    a |= prg_read((uint16_t)(0x8A43 + x));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8121, 2);
    if (z) { mff_cycles(0x8121, 1); goto L_816E; }
    mff_cycles(0x8123, 4);
    a = prg_read((uint16_t)(0x8A43 + x));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8126, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0128, 0x803A, 3)) return;
    SYNC_IN;
    mff_cycles(0x8129, 4);
    y = a;
    if (y == 0) { mff_cycles(0x812A, 1); goto L_816F; }
    mff_cycles(0x812C, 14);
    y = 0x00;
    x++;
    RAM(SND_TMP + 3) = a;
    a &= 0x7F;
    m = RAM(SND_EFFECT_PRIORITY);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x8137, 5);
        RAM(SND_EFFECT_PRIORITY) = a;
        if (z) {
            mff_cycles(0x813B, 5);
            a = RAM(SND_EFFECT_LOOP);
            if (a & 0x80) {
                mff_cycles(0x813F, 5);
                a = RAM(SND_TMP + 3);
                if (!(a & 0x80)) {
                    mff_cycles(0x8143, 3);
                    RAM(SND_EFFECT_RELAUNCH) = y;
                } else {
                    mff_cycles(0x8141, 1);
                }
            } else {
                mff_cycles(0x813D, 1);
            }
        } else {
            mff_cycles(0x8139, 1);
        }
        mff_cycles(0x8145, 15);
        RAM(SND_EFFECT_LOOP) = y;
        t = RAM(SND_TMP + 3);
        c = t >> 7;
        t = (uint8_t)(t << 1);
        RAM(SND_TMP + 3) = t;
        t = RAM(SND_EFFECT_LOOP);
        m = t & 1;
        t = (uint8_t)(t >> 1 | c << 7);
        c = m;
        RAM(SND_EFFECT_LOOP) = t;
        if (t & 0x80) {
            mff_cycles(0x814D, 3);
            RAM(SND_EFFECT_RELAUNCH) = x;
        } else {
            mff_cycles(0x814B, 1);
        }
        mff_cycles(0x814F, 13);
        t = (uint8_t)(RAM(SND_TMP) + 1);
        RAM(SND_TMP) = t;
        a = RAM(SND_TMP);
        RAM(SND_EFFECT_PTR) = a;
        if (a == 0) {
            mff_cycles(0x8157, 5);
            t = (uint8_t)(RAM(SND_TMP + 1) + 1);
            RAM(SND_TMP + 1) = t;
        } else {
            mff_cycles(0x8155, 1);
        }
        mff_cycles(0x8159, 22);
        a = RAM(SND_TMP + 1);
        RAM(SND_EFFECT_PTR + 1) = a;
        a = y;
        RAM(SND_EFFECT_TRANSPOSE) = a;
        RAM(SND_EFFECT_ROW_FRAMES) = a;
        RAM(SND_EFFECT_GATE) = a;
        RAM(SND_EFFECT_GATE_LEFT) = a;
        y = 0x27;
        for (;;) {
            mff_cycles(0x8168, 9);
            RAM(SND_INSTRUMENT + y) = a;
            y--;
            n = (y) >> 7;
            z = (y) == 0;
            if (n) break;
            mff_cycles(0x8168, 1);
        }
    } else {
        mff_cycles(0x8135, 1);
    }
L_816E:;
    mff_cycles(0x816E, 6);
    SYNC_OUT;
    mff_rts(); return;
L_816F:;
    mff_cycles(0x816F, 24);
    x = 0x01;
    RAM(SND_TEMPO) = x;
    x = 0x99;
    RAM(SND_TEMPO + 1) = x;
    RAM(SND_TEMPO_ACC) = a;
    RAM(SND_TRANSPOSE) = a;
    RAM(SND_FADE_SPEED) = a;
    RAM(SND_FADE_LEVEL) = a;
    x = 0x53;
    for (;;) {
        mff_cycles(0x8181, 9);
        RAM(SND_M_INSTRUMENT + x) = a;
        x--;
        if (x & 0x80) break;
        mff_cycles(0x8181, 1);
    }
    mff_cycles(0x8187, 2);
    x = 0x03;
    for (;;) {
        mff_cycles(0x8189, 7);
        t = (uint8_t)(RAM(SND_TMP) + 1);
        RAM(SND_TMP) = t;
        if (t == 0) {
            mff_cycles(0x818D, 5);
            t = (uint8_t)(RAM(SND_TMP + 1) + 1);
            RAM(SND_TMP + 1) = t;
        } else {
            mff_cycles(0x818B, 1);
        }
        mff_cycles(0x818F, 12);
        y = RAM(SND_TMP);
        a = RAM(SND_TMP + 1);
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x0195, 0x803A, 3)) return;
        SYNC_IN;
        mff_cycles(0x8196, 12);
        RAM(SND_MUSIC_PTR_HI + x) = a;
        t = (uint8_t)(RAM(SND_TMP) + 1);
        RAM(SND_TMP) = t;
        if (t == 0) {
            mff_cycles(0x819D, 5);
            t = (uint8_t)(RAM(SND_TMP + 1) + 1);
            RAM(SND_TMP + 1) = t;
        } else {
            mff_cycles(0x819B, 1);
        }
        mff_cycles(0x819F, 12);
        y = RAM(SND_TMP);
        a = RAM(SND_TMP + 1);
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x01A5, 0x803A, 3)) return;
        SYNC_IN;
        mff_cycles(0x81A6, 9);
        RAM(SND_MUSIC_PTR_LO + x) = a;
        x--;
        n = (x) >> 7;
        if (n) break;
        mff_cycles(0x8189, 1);
    }
    mff_cycles(0x81AC, 2);
    if (!n) {
        mff_cycles(0x81AE, 11);
        RAM(SND_TMP + 2) = y;
        a &= 0x07;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x01B4, 0x8023, 3)) return;
        SYNC_IN;
        mff_cycles(0x81B5, 11);
        mem_write(RAM16((uint8_t)(RNG + x)), a);
        mff_cycles(0x81BA, 6);
        mem_write(RAM16((uint8_t)(0x1E + x)), a);
        mff_cycles(0x81BC, 0);
        mff_unreachable(0x81BC); return;   /* data: the branch above always goes */
    } else {
        mff_cycles(0x81AC, 1);
    }
    mff_cycles(0x81F1, 8);
    a = RAM(SND_EFFECT_CHANNELS);
    mff_push(a);
    x = 0x03;
    for (;;) {
        mff_cycles(0x81F6, 7);
        t = RAM(SND_EFFECT_CHANNELS);
        c = t & 1;
        t >>= 1;
        RAM(SND_EFFECT_CHANNELS) = t;
        n = (t) >> 7;
        z = (t) == 0;
        if (!c) {
            mff_cycles(0x81FA, 6);
            SYNC_OUT;
            if (!mff_call_at(0x01FC, 0x80D8, 3)) return;
            SYNC_IN;
            mff_cycles(0x81FD, 10);
            a = RAM(SND_MUSIC_PTR_HI + x);
            a |= RAM(SND_MUSIC_PTR_LO + x);
            if (a != 0) {
                mff_cycles(0x8205, 7);
                a = 0xFF;
                RAM(SND_PERIOD_HI_CACHE + x) = a;
            } else {
                mff_cycles(0x8203, 1);
            }
        } else {
            mff_cycles(0x81F8, 2);
        }
        mff_cycles(0x820A, 4);
        x--;
        if (x & 0x80) break;
        mff_cycles(0x81F6, 2);
    }
    mff_cycles(0x820D, 13);
    a = mff_pull();
    RAM(SND_EFFECT_CHANNELS) = a;
    a = 0x08;
    hw_write(APU_PULSE1 + 1, a);
    mff_cycles(0x8215, 4);
    hw_write(APU_PULSE2 + 1, a);
    mff_cycles(0x8218, 6);
    a = 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    hw_write(APU_STATUS, a);
    mff_cycles(0x821D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/* snd_effect_row $825B (bank 3) - lifted by port/tools/lift.py */
void func_825B_b3(void) {
    REGS_IN;
L_825B:;
    mff_cycles(0x825B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x025D, 0x8386, 3)) return;
    SYNC_IN;
    mff_cycles(0x825E, 7);
    RAM(SND_TMP + 3) = a;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    if (c) {
        mff_cycles(0x8263, 10);
        RAM(SND_EFFECT_PRIORITY) = y;
        a = RAM(SND_EFFECT_RELAUNCH);
        t = a;
        c = t & 1;
        t >>= 1;
        a = t;
        n = (t) >> 7;
        z = (t) == 0;
        if (c) {
            mff_cycles(0x826A, 6);
            SYNC_OUT;
            if (!mff_call_at(0x026C, 0x8118, 3)) return;
            SYNC_IN;
            mff_cycles(0x826D, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x825B, 2); call_by_address_tail(0x825B, 3); return;
        } else {
            mff_cycles(0x8268, 1);
        }
        mff_cycles(0x8270, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x81C8, 2); call_by_address_tail(0x81C8, 3); return;
    } else {
        mff_cycles(0x8261, 1);
    }
    mff_cycles(0x8273, 7);
    t = RAM(SND_TMP + 3);
    c = t & 1;
    t >>= 1;
    RAM(SND_TMP + 3) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!c) { mff_cycles(0x8275, 1); goto L_82A6; }
    mff_cycles(0x8277, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0279, 0x8386, 3)) return;
    SYNC_IN;
    mff_cycles(0x827A, 4);
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (z) { mff_cycles(0x827B, 1); goto L_8289; }
    mff_cycles(0x827D, 13);
    t = RAM(SND_EFFECT_LOOP);
    c = t >> 7;
    t = (uint8_t)(t << 1);
    RAM(SND_EFFECT_LOOP) = t;
    n = (t) >> 7;
    z = (t) == 0;
    FLAGS_OUT;
    mff_push(mff_p());
    if (a == RAM(SND_EFFECT_LOOP)) { mff_cycles(0x8282, 1); goto L_8296; }
    mff_cycles(0x8284, 14);
    mff_set_p(mff_pull());
    FLAGS_IN;
    t = RAM(SND_EFFECT_LOOP);
    m = t & 1;
    t = (uint8_t)(t >> 1 | c << 7);
    c = m;
    RAM(SND_EFFECT_LOOP) = t;
    t = (uint8_t)(RAM(SND_EFFECT_LOOP) + 1);
    RAM(SND_EFFECT_LOOP) = t;
    n = (t) >> 7;
    z = (t) == 0;
L_8289:;
    mff_cycles(0x8289, 6);
    SYNC_OUT;
    if (!mff_call_at(0x028B, 0x8386, 3)) return;
    SYNC_IN;
    mff_cycles(0x828C, 8);
    x = a;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x028F, 0x8386, 3)) return;
    SYNC_IN;
    mff_cycles(0x8290, 8);
    RAM(SND_EFFECT_PTR) = a;
    RAM(SND_EFFECT_PTR + 1) = x;
    if (!z) { mff_cycles(0x825B, 1); goto L_825B; }
L_8296:;
    mff_cycles(0x8296, 23);
    a = y;
    mff_set_p(mff_pull());
    FLAGS_IN;
    t = a;
    t = (uint8_t)(t >> 1 | c << 7);
    a = t;
    RAM(SND_EFFECT_LOOP) = a;
    c = 0;
    a = 0x02;
    m = RAM(SND_EFFECT_PTR);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(SND_EFFECT_PTR) = a;
    if (c) {
        mff_cycles(0x82A4, 5);
        t = (uint8_t)(RAM(SND_EFFECT_PTR + 1) + 1);
        RAM(SND_EFFECT_PTR + 1) = t;
    } else {
        mff_cycles(0x82A2, 1);
    }
L_82A6:;
    mff_cycles(0x82A6, 7);
    t = RAM(SND_TMP + 3);
    c = t & 1;
    t >>= 1;
    RAM(SND_TMP + 3) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (c) {
        mff_cycles(0x82AA, 6);
        SYNC_OUT;
        if (!mff_call_at(0x02AC, 0x8386, 3)) return;
        SYNC_IN;
        mff_cycles(0x82AD, 3);
        RAM(SND_EFFECT_GATE) = a;
    } else {
        mff_cycles(0x82A8, 1);
    }
    mff_cycles(0x82AF, 7);
    t = RAM(SND_TMP + 3);
    c = t & 1;
    t >>= 1;
    RAM(SND_TMP + 3) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (c) {
        mff_cycles(0x82B3, 6);
        SYNC_OUT;
        if (!mff_call_at(0x02B5, 0x8386, 3)) return;
        SYNC_IN;
        mff_cycles(0x82B6, 3);
        RAM(SND_EFFECT_TRANSPOSE) = a;
    } else {
        mff_cycles(0x82B1, 1);
    }
    mff_cycles(0x82B8, 6);
    SYNC_OUT;
    if (!mff_call_at(0x02BA, 0x8386, 3)) return;
    SYNC_IN;
    mff_cycles(0x82BB, 18);
    RAM(SND_EFFECT_ROW_FRAMES) = a;
    RAM(SND_TMP) = a;
    a = RAM(SND_EFFECT_GATE);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_TMP + 3) = a;
    SYNC_OUT;
    if (!mff_call_at(0x02C5, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x82C6, 19);
    y = RAM(SND_TMP);
    y++;
    RAM(SND_EFFECT_GATE_LEFT) = y;
    t = (uint8_t)(RAM(SND_FLAGS) + 1);
    RAM(SND_FLAGS) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x02CF, 0x8386, 3)) return;
    SYNC_IN;
    mff_cycles(0x82D0, 8);
    mff_push(a);
    a ^= RAM(SND_EFFECT_CHANNELS);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0x82D5, 9);
        RAM(SND_EFFECT_CHANNELS) = a;
        SYNC_OUT;
        if (!mff_call_at(0x02D9, 0x81D4, 3)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x82D3, 1);
    }
    mff_cycles(0x82DA, 13);
    a = mff_pull();
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_EFFECT_CHANNELS) = a;
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

/*  $A592 (bank 6) - lifted by port/tools/lift.py */
void func_A592_b6(void) {
    REGS_IN;
    mff_cycles(0xA592, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AC13 (bank 6) - lifted by port/tools/lift.py */
void func_AC13_b6(void) {
    REGS_IN;
    mff_cycles(0xAC13, 52);
    a = 0x8E;
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x03;
    RAM(OBJ_VY + x) = a;
    a = RAM(OBJ_ID + x);
    RAM(OBJ_SAVED_ID + x) = a;
    a = 0x05;
    RAM(OBJ_ID + x) = a;
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

/*  $AC4B (bank 6) - lifted by port/tools/lift.py */
void func_AC4B_b6(void) {
    REGS_IN;
    mff_cycles(0xAC4B, 4);
    a = prg_read((uint16_t)(0xACC6 + y));
    mff_cycles(0xAC4E, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = prg_read((uint16_t)(0xACD6 + y));
    mff_cycles(0xAC54, 23);
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_VX + x) = a;
    RAM(OBJ_VY + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AC60 (bank 6) - lifted by port/tools/lift.py */
void func_AC60_b6(void) {
    REGS_IN;
    mff_cycles(0xAC60, 4);
    a = prg_read((uint16_t)(0xACE6 + y));
    mff_cycles(0xAC63, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = prg_read((uint16_t)(0xACF6 + y));
    mff_cycles(0xAC69, 23);
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_VX + x) = a;
    RAM(OBJ_VY + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AC75 (bank 6) - lifted by port/tools/lift.py */
void func_AC75_b6(void) {
    REGS_IN;
    mff_cycles(0xAC75, 4);
    a = prg_read((uint16_t)(0xAD26 + y));
    mff_cycles(0xAC78, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = prg_read((uint16_t)(0xAD06 + y));
    mff_cycles(0xAC7E, 9);
    RAM(OBJ_VX + x) = a;
    a = prg_read((uint16_t)(0xAD36 + y));
    mff_cycles(0xAC84, 9);
    RAM(OBJ_VY_FRAC + x) = a;
    a = prg_read((uint16_t)(0xAD16 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xAC8A, 11);
    RAM(OBJ_VY + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AC8E (bank 6) - lifted by port/tools/lift.py */
void func_AC8E_b6(void) {
    REGS_IN;
    mff_cycles(0xAC8E, 4);
    a = prg_read((uint16_t)(0xAD46 + y));
    mff_cycles(0xAC91, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = prg_read((uint16_t)(0xAD06 + y));
    mff_cycles(0xAC97, 9);
    RAM(OBJ_VX + x) = a;
    a = prg_read((uint16_t)(0xAD56 + y));
    mff_cycles(0xAC9D, 9);
    RAM(OBJ_VY_FRAC + x) = a;
    a = prg_read((uint16_t)(0xAD16 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xACA3, 11);
    RAM(OBJ_VY + x) = a;
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

/*  $8177 (bank 0) - lifted by port/tools/lift.py */
void func_8177_b0(void) {
    REGS_IN;
    mff_cycles(0x8177, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8F82, 2); call_by_address_tail(0x8F82, 0); return;
}

/*  $8992 (bank 0) - lifted by port/tools/lift.py */
void func_8992_b0(void) {
    REGS_IN;
    mff_cycles(0x8992, 8);
    a = RAM(OBJ_ANIM);
    m = 0x69;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8997, 1); goto L_89F5; }
    mff_cycles(0x8999, 6);
    a = RAM(OBJ_TIMER);
    if (a != 0) {
        mff_cycles(0x899E, 12);
        t = (uint8_t)(RAM(OBJ_TIMER) - 1);
        RAM(OBJ_TIMER) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x899C, 1);
    }
    mff_cycles(0x89A2, 44);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    a = 0x00;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x03;
    RAM(OBJ_VX) = a;
    a = 0xC0;
    RAM(OBJ_VY_FRAC) = a;
    a = 0xFF;
    RAM(OBJ_VY) = a;
    a = RAM(OBJ_TIMER2);
    c = 0;
    m = 0x02;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    m = RAM(OBJ_Y);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x89C2, 1); goto L_89C9; }
    mff_cycles(0x89C4, 2);
    if (z) { mff_cycles(0x89C4, 1); goto L_89F6; }
    mff_cycles(0x89C6, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x89F6, 2); call_by_address_tail(0x89F6, 0); return;
L_89C9:;
    mff_cycles(0x89C9, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x09CD, func_F119, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x89CE, 2);
    if (c) {
        mff_cycles(0x89D0, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x89F6, 2); call_by_address_tail(0x89F6, 0); return;
    } else {
        mff_cycles(0x89CE, 1);
    }
    mff_cycles(0x89D3, 8);
    a = RAM(OBJ_VY);
    m = 0xFD;
    c = a >= m;
    if (a == m) {
        mff_cycles(0x89DA, 8);
        a = 0x38;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x09DE, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x89DF, 8);
        a = 0x05;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x09E3, func_F440, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x89D8, 1);
    }
    mff_cycles(0x89E4, 7);
    a = RAM(PLAYER_DIR);
    a &= 0x01;
    if (a != 0) {
        mff_cycles(0x89EA, 8);
        y = 0x00;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x09EE, func_F000, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x89EF, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x89E8, 1);
    }
    mff_cycles(0x89F0, 8);
    y = 0x01;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x09F4, func_F051, 0xE000)) return;
    SYNC_IN;
L_89F5:;
    mff_cycles(0x89F5, 6);
    SYNC_OUT;
    mff_rts(); return;
L_89F6:;
    mff_cycles(0x89F6, 16);
    a = RAM(OBJ_TIMER2);
    RAM(OBJ_Y) = a;
    a = 0x2F;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0A00, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8A01, 8);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0A05, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8A06, 38);
    a = 0xC0;
    RAM(OBJ_VY_FRAC) = a;
    a = 0xFF;
    RAM(OBJ_VY) = a;
    a = 0x33;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VX) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(PLAYER_NO_HIT) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0A22, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8A23, 22);
    a = 0x22;
    RAM(GAME_ACTION) = a;
    a = 0x1E;
    RAM(ACTION_TIMER) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $89F6 (bank 0) - lifted by port/tools/lift.py */
void func_89F6_b0(void) {
    REGS_IN;
    mff_cycles(0x89F6, 16);
    a = RAM(OBJ_TIMER2);
    RAM(OBJ_Y) = a;
    a = 0x2F;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0A00, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8A01, 8);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0A05, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8A06, 38);
    a = 0xC0;
    RAM(OBJ_VY_FRAC) = a;
    a = 0xFF;
    RAM(OBJ_VY) = a;
    a = 0x33;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VX) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(PLAYER_NO_HIT) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0A22, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8A23, 22);
    a = 0x22;
    RAM(GAME_ACTION) = a;
    a = 0x1E;
    RAM(ACTION_TIMER) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8A55 (bank 0) - lifted by port/tools/lift.py */
void func_8A55_b0(void) {
    REGS_IN;
    mff_cycles(0x8A55, 8);
    a = 0x14;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0A59, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8A5A, 8);
    a = 0x07;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0A5E, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8A5F, 23);
    a = 0x33;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VY) = a;
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PLAYER_NO_HIT) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8AD3 (bank 0) - lifted by port/tools/lift.py */
void func_8AD3_b0(void) {
    REGS_IN;
    mff_cycles(0x8AD3, 50);
    a = 0xC0;
    RAM(OBJ_VY_FRAC) = a;
    a = 0xFF;
    RAM(OBJ_VY) = a;
    a = 0x80;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VX) = a;
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
    if (!mff_call_at(0x0AF7, 0xCEAA, 0)) return;
    SYNC_IN;
    mff_cycles(0x8AF8, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8AFD, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8F82 (bank 0) - lifted by port/tools/lift.py */
void func_8F82_b0(void) {
    REGS_IN;
    mff_cycles(0x8F82, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0F84, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8F85, 32);
    a = 0x00;
    RAM(GAME_ACTION) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(JUMP_MOVING) = a;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER + 1) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE288 + y));
    mff_cycles(0x8F9D, 7);
    RAM(PLAYER_STAND_ANIM) = a;
    a = prg_read((uint16_t)(0xE1C1 + y));
    mff_cycles(0x8FA2, 8);
    RAM(OBJ_VX_FRAC) = a;
    a = prg_read((uint16_t)(0xE1C4 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8FA8, 10);
    RAM(OBJ_VX) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0FAD, 0xCEAA, 0)) return;
    SYNC_IN;
    mff_cycles(0x8FAE, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9043 (bank 0) - lifted by port/tools/lift.py */
void func_9043_b0(void) {
    REGS_IN;
    mff_cycles(0x9043, 4);
    a = prg_read((uint16_t)(0xE22B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9046, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1048, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9049, 8);
    a = RAM(OBJ_Y);
    m = 0x7C;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0x9050, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1052, func_F1BF, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9053, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x904E, 1);
    }
    mff_cycles(0x9054, 8);
    a = RAM(OBJ_Y);
    m = 0x80;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x905B, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x105D, func_F1DF, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x905E, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9059, 1);
    }
    mff_cycles(0x905F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1061, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9062, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1064, func_F03F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9065, 24);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    a = 0x38;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_VY) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $916E (bank 0) - lifted by port/tools/lift.py */
void func_916E_b0(void) {
    REGS_IN;
    mff_cycles(0x916E, 14);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    a = RAM(OBJ_STATE);
    a &= 0x02;
    if (a != 0) { mff_cycles(0x9176, 1); goto L_91B3; }
    mff_cycles(0x9178, 8);
    a = RAM(OBJ_Y);
    m = 0x82;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0x917F, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x1181, func_F1BF, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x9182, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x917D, 1);
    }
    mff_cycles(0x9183, 8);
    a = RAM(OBJ_Y);
    m = 0x84;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x918A, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x118C, func_F1DF, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0x918D, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x9188, 1);
    }
    mff_cycles(0x918E, 8);
    a = RAM(OBJ_DIR);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x9193, 1); goto L_91A5; }
    mff_cycles(0x9195, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1197, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9198, 8);
    a = RAM(OBJ_X);
    m = 0x80;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x919D, 1); goto L_91B2; }
    mff_cycles(0x919F, 9);
    t = (uint8_t)(RAM(OBJ_STATE) + 1);
    RAM(OBJ_STATE) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x91B2, 2); call_by_address_tail(0x91B2, 0); return;
L_91A5:;
    mff_cycles(0x91A5, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x11A7, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x91A8, 8);
    a = RAM(OBJ_X);
    m = 0x80;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0x91AF, 6);
        t = (uint8_t)(RAM(OBJ_STATE) + 1);
        RAM(OBJ_STATE) = t;
        n = (t) >> 7;
        z = (t) == 0;
    } else {
        mff_cycles(0x91AD, 1);
    }
L_91B2:;
    mff_cycles(0x91B2, 6);
    SYNC_OUT;
    mff_rts(); return;
L_91B3:;
    mff_cycles(0x91B3, 19);
    a = 0x40;
    RAM(JOY_PRESSED) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(JUMP_MOVING) = a;
    RAM(COMBO_TIMER) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $91B2 (bank 0) - lifted by port/tools/lift.py */
void func_91B2_b0(void) {
    REGS_IN;
    mff_cycles(0x91B2, 6);
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

/*  $822D (bank 3) - lifted by port/tools/lift.py */
void func_822D_b3(void) {
    REGS_IN;
    mff_cycles(0x822D, 7);
    t = RAM(SND_TMP + 2);
    c = t >> 7;
    t = (uint8_t)(t << 1);
    RAM(SND_TMP + 2) = t;
    if (t != 0) {
        mff_cycles(0x8231, 7);
        c = 1;
        t = RAM(SND_TMP + 2);
        m = t & 1;
        t = (uint8_t)(t >> 1 | c << 7);
        c = m;
        RAM(SND_TMP + 2) = t;
    } else {
        mff_cycles(0x822F, 1);
    }
    mff_cycles(0x8234, 16);
    a = RAM(SND_FLAGS);
    a &= 0x0F;
    RAM(SND_FLAGS) = a;
    y = RAM(SND_TMP + 2);
    n = (y) >> 7;
    z = (y) == 0;
    RAM(SND_FADE_SPEED) = y;
    if (z) { mff_cycles(0x823E, 1); goto L_8247; }
    mff_cycles(0x8240, 7);
    y = 0xFF;
    m = RAM(SND_FADE_LEVEL);
    c = y >= m;
    n = ((uint8_t)(y - m)) >> 7;
    z = ((uint8_t)(y - m)) == 0;
    if (!z) { mff_cycles(0x8244, 1); goto L_8249; }
    mff_cycles(0x8246, 2);
    y++;
    n = (y) >> 7;
    z = (y) == 0;
L_8247:;
    mff_cycles(0x8247, 3);
    RAM(SND_FADE_LEVEL) = y;
L_8249:;
    mff_cycles(0x8249, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8234 (bank 3) - lifted by port/tools/lift.py */
void func_8234_b3(void) {
    REGS_IN;
    mff_cycles(0x8234, 16);
    a = RAM(SND_FLAGS);
    a &= 0x0F;
    RAM(SND_FLAGS) = a;
    y = RAM(SND_TMP + 2);
    n = (y) >> 7;
    z = (y) == 0;
    RAM(SND_FADE_SPEED) = y;
    if (z) { mff_cycles(0x823E, 1); goto L_8247; }
    mff_cycles(0x8240, 7);
    y = 0xFF;
    m = RAM(SND_FADE_LEVEL);
    c = y >= m;
    n = ((uint8_t)(y - m)) >> 7;
    z = ((uint8_t)(y - m)) == 0;
    if (!z) { mff_cycles(0x8244, 1); goto L_8249; }
    mff_cycles(0x8246, 2);
    y++;
    n = (y) >> 7;
    z = (y) == 0;
L_8247:;
    mff_cycles(0x8247, 3);
    RAM(SND_FADE_LEVEL) = y;
L_8249:;
    mff_cycles(0x8249, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $824A (bank 3) - lifted by port/tools/lift.py */
void func_824A_b3(void) {
    REGS_IN;
    mff_cycles(0x824A, 16);
    a = 0x00;
    c = 1;
    m = RAM(SND_TMP + 2);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_EFFECT_PITCH_SCALE) = a;
    SYNC_OUT;
    mff_rts(); return;
}
