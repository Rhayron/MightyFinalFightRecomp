/*
 * lifted_b5.c - bank 5 routines $8000-$BFFF, lifted by
 * port/tools/lift_bank.py from the pure scaffold: exact, still to be
 * tidied and moved into their systems' files.
 */
#include "mff.h"
#include "state.h"
#include "coroutine.h"

void func_E954(void);
void func_F090(void);
void func_F099(void);
void func_F0A2(void);
void func_F0F7(void);
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
void func_F34A(void);
void func_F34C(void);
void func_F374(void);
void func_F440(void);
void func_F449(void);
void func_F71A(void);
void func_FB3E(void);
void func_FB5B(void);

/*  $9C00 (bank 5) - lifted by port/tools/lift.py */
void func_9C00_b5(void) {
    REGS_IN;
    mff_cycles(0x9C00, 5);
    x = RAM(VRAM_BUFFER_LEN);
    y = 0x00;
L_9C04:;
    mff_cycles(0x9C04, 4);
    a = prg_read((uint16_t)(0x9C20 + y));
    mff_cycles(0x9C07, 4);
    m = 0xFF;
    c = a >= m;
    if (a == m) { mff_cycles(0x9C09, 1); goto L_9C12; }
    mff_cycles(0x9C0B, 11);
    RAM(VRAM_BUFFER + x) = a;
    y++;
    x++;
    if (x != 0) { mff_cycles(0x9C04, 1); goto L_9C04; }
L_9C12:;
    mff_cycles(0x9C12, 14);
    RAM(VRAM_BUFFER_LEN) = x;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SPECIAL_SCREEN_STEP) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x1C1A, func_E954, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9C1B, 11);
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(VRAM_REQUEST) = a;
    SYNC_OUT;
    mff_rts(); return;
}

