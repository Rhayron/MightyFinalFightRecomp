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
#include "state.h"

void func_FF72(void);   /* sound_update, still generated */

/* bank_switch, $FF4D. The cycles are charged per segment of the $F6 guard,
 * so an interrupt sees the same $F6 at any point it could land. */
void func_FF4D(void) {
    mff_cycles(0xFF4D, 5);
    RAM(BANK_SWITCH_BUSY)++;

    mff_cycles(0xFF4F, 38);
    RAM(MMC3_SELECT) = 6;
    hw_write(MMC3_BANK_SELECT, 6);
    RAM(PRG_BANKS) = RAM(BANK_REQUEST);
    hw_write(MMC3_BANK_DATA, RAM(PRG_BANKS));
    RAM(MMC3_SELECT) = 7;
    hw_write(MMC3_BANK_SELECT, 7);
    RAM(PRG_BANKS + 1) = RAM(BANK_REQUEST + 1);
    hw_write(MMC3_BANK_DATA, RAM(PRG_BANKS + 1));

    mff_cycles(0xFF6B, 5);
    RAM(BANK_SWITCH_BUSY)--;

    mff_cycles(0xFF6D, 3);
    g_cpu.A = RAM(SOUND_DEFERRED);
    mff_nz(g_cpu.A);
    if (g_cpu.A) {               /* an NMI deferred its sound update */
        mff_cycles(0xFF6F, 3);
        func_FF72();
        return;
    }
    mff_cycles(0xFF6F, 2 + 6);
    mff_rts();
}
