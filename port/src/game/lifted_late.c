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
