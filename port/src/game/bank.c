/*
 * bank.c - PRG banking (docs/engine.md, "Bank switching")
 *
 * bank_request $F4/$F5 holds the banks wanted at $8000/$A000; the MMC3's
 * R6/R7 get them, with shadows in $F0 (bank select) and $F2/$F3.
 * bank_switch_busy $F6 is held during the switch: the sound update that the
 * NMI runs on its way out (nmi_tail) switches banks itself, so then it only
 * sets sound_deferred $F7, and bank_switch runs it when it finishes.
 */
#include "mff.h"

void func_FF72(void);   /* sound_update, still generated */

/* bank_switch, $FF4D. The cycles are charged per segment of the $F6 guard,
 * so an interrupt sees the same $F6 at any point it could land. */
void func_FF4D(void) {
    mff_cycles(0xFF4D, 5);
    RAM(0xF6)++;

    mff_cycles(0xFF4F, 38);
    RAM(0xF0) = 6;
    nes_write(0x8000, 6);
    RAM(0xF2) = RAM(0xF4);
    nes_write(0x8001, RAM(0xF2));
    RAM(0xF0) = 7;
    nes_write(0x8000, 7);
    RAM(0xF3) = RAM(0xF5);
    nes_write(0x8001, RAM(0xF3));

    mff_cycles(0xFF6B, 5);
    RAM(0xF6)--;

    mff_cycles(0xFF6D, 3);
    g_cpu.A = RAM(0xF7);
    mff_nz(g_cpu.A);
    if (g_cpu.A) {               /* an NMI deferred its sound update */
        mff_cycles(0xFF6F, 3);
        func_FF72();
        return;
    }
    mff_cycles(0xFF6F, 2 + 6);
    mff_rts();
}
