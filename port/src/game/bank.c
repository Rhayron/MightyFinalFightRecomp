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

/* bank_switch, $FF4D. Charged instruction by instruction (a 1 is a taken
 * branch's extra cycle): an interrupt sees $F6 and the shadows as the
 * console's would wherever it lands, and with the console's timing a lag
 * frame's NMI or the raster IRQ can land anywhere in here
 * (docs/timing.md). */
void func_FF4D(void) {
    mff_cycles(0xFF4D, 5);                      /* INC $F6 */
    RAM(BANK_SWITCH_BUSY)++;

    mff_cycles(0xFF4F, 2);                      /* LDA #6 */
    g_cpu.A = 6;
    mff_cycles(0xFF51, 3);                      /* STA $F0 */
    RAM(MMC3_SELECT) = 6;
    mff_cycles(0xFF53, 4);                      /* STA $8000 */
    hw_write(MMC3_BANK_SELECT, 6);
    mff_cycles(0xFF56, 3);                      /* LDA $F4 */
    g_cpu.A = RAM(BANK_REQUEST);
    mff_cycles(0xFF58, 3);                      /* STA $F2 */
    RAM(PRG_BANKS) = g_cpu.A;
    mff_cycles(0xFF5A, 4);                      /* STA $8001 */
    hw_write(MMC3_BANK_DATA, g_cpu.A);
    mff_cycles(0xFF5D, 2);                      /* LDA #7 */
    g_cpu.A = 7;
    mff_cycles(0xFF5F, 3);                      /* STA $F0 */
    RAM(MMC3_SELECT) = 7;
    mff_cycles(0xFF61, 4);                      /* STA $8000 */
    hw_write(MMC3_BANK_SELECT, 7);
    mff_cycles(0xFF64, 3);                      /* LDA $F5 */
    g_cpu.A = RAM(BANK_REQUEST + 1);
    mff_cycles(0xFF66, 3);                      /* STA $F3 */
    RAM(PRG_BANKS + 1) = g_cpu.A;
    mff_cycles(0xFF68, 4);                      /* STA $8001 */
    hw_write(MMC3_BANK_DATA, g_cpu.A);

    mff_cycles(0xFF6B, 5);                      /* DEC $F6 */
    RAM(BANK_SWITCH_BUSY)--;

    mff_cycles(0xFF6D, 3);                      /* LDA $F7 */
    g_cpu.A = RAM(SOUND_DEFERRED);
    mff_nz(g_cpu.A);
    mff_cycles(0xFF6F, 2);                      /* BNE $FF72 */
    if (g_cpu.A) {               /* an NMI deferred its sound update */
        mff_cycles(0xFF6F, 1);
        func_FF72();
        return;
    }
    mff_cycles(0xFF71, 6);                      /* RTS */
    mff_rts();
}
