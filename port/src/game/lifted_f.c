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
