/*
 * mff_hooks.c - two game rules the scaffold kept in port/scaffold/extras.c
 *
 * Both belong with the game's code; they are here until port/src/game takes
 * them (port/src/game is being tidied on other branches).
 *
 * 1. Where the NMI may land (MFF_LAG=1 turns the rule off). Never while a
 *    task runs (state 2 in $80+4n): the tasks keep return addresses on
 *    their own parts of the stack, and an NMI in the middle of one (a lag
 *    frame) cannot be modelled on C stacks. Around the scheduler only at
 *    the top of its scan ($FEBD): the sound driver reads a register that
 *    nmi_tail never sets, so the place the NMI interrupts leaks into the
 *    driver's scratch; at $FEBD the registers are always the same. Not in
 *    task_wait_frame's tail or the IRQ handlers either. Elsewhere (reset's
 *    $2002 polling) it is not held.
 *
 * 2. nmi_tail ($C119). The NMI rewrites its own return address so that RTI
 *    lands here outside the interrupt; on the console this pushes P/A/X/Y,
 *    fakes a return to the interrupted PC ($A0/$A1 - 1), calls
 *    sound_update ($FF72) and RTSes back into the interrupted code. The
 *    runtime takes the NMI between instructions, not at a PC, so the tail
 *    does it all but the jump: the same stack writes (sound_update's RTS
 *    pops the JSR's return address, so the stack pointer comes back where
 *    it was), sound_update, A/X/Y/P as they were, and the cycles. Calling
 *    sound_update without the JSR's push, as the scaffold's extras.c does,
 *    left S two bytes up after every NMI ($01 instead of $FF at the
 *    scheduler), and the raster IRQ's interrupt frame then went to $0101,
 *    $0100 and $01FF: it zeroed bg_pal_anim_on for background palettes 0
 *    and 1, and round 3 area 2's palette animation (palette 1) stopped
 *    where the oracle's runs.
 */
#include "machine.h"
#include <stdlib.h>

static int nmi_held(uint16_t pc) {
    /* reset ($FE00-$FEB9) is never held: its vblank polling waits for the
     * frame boundary, and after the title's JMP reset (attract mode) it runs
     * on task 0's stack with task 0 still "running" until RAM is cleared */
    if (pc >= 0xFE00 && pc < 0xFEBA) return 0;
    for (int t = 0; t < 4; t++)
        if (g_ram[0x80 + 4 * t] == 2) return 1;
    if (pc >= 0xFEBA && pc <= 0xFF01) return pc != 0xFEBD;     /* scheduler */
    if (pc >= 0xFF24 && pc <= 0xFF49) return 1;                /* task_wait_frame */
    if (pc >= 0xC13B && pc <= 0xC1DE) return 1;                /* irq */
    return 0;
}

void mff_hooks_init(void) {
    const char *lag = getenv("MFF_LAG");
    g_nmi_veto = (lag && *lag == '1') ? NULL : nmi_held;
}

void func_FF72(void);                   /* sound_update */

static void push(uint8_t v) {
    g_ram[0x100 + g_cpu.S] = v;
    g_cpu.S--;
}

static uint8_t pull(void) {
    g_cpu.S++;
    return g_ram[0x100 + g_cpu.S];
}

void func_C119(void) {
    CPU6502State saved = g_cpu;
    uint8_t p = (uint8_t)((g_cpu.N << 7) | (g_cpu.V << 6) | 0x30 | (g_cpu.D << 3) |
                          (g_cpu.I << 2) | (g_cpu.Z << 1) | g_cpu.C);   /* as PHP pushes it */
    uint16_t ret = (uint16_t)(((g_ram[0xA1] << 8) | g_ram[0xA0]) - 1);
    uint8_t x;
    push(p);                                    /* PHP x3: room for the return address, and P */
    push(p);
    push(p);
    push(g_cpu.A);                              /* PHA, TXA/PHA, TYA/PHA */
    push(g_cpu.X);
    push(g_cpu.Y);
    x = g_cpu.S;                                /* TSX; STA $0105,X / $0106,X */
    g_ram[(0x105 + x) & 0x7FF] = (uint8_t)ret;
    g_ram[(0x106 + x) & 0x7FF] = (uint8_t)(ret >> 8);
    push(0xC1);                                 /* JSR $FF72: returns to $C134 */
    push(0x33);
    /* the console's cycles: PHP x3, PHA/TXA/PHA/TYA/PHA, TSX, SEC, the
     * fake return (LDA zp, SBC #, STA abs,X twice) and the JSR; after it
     * PLA/TAY/PLA/TAX/PLA, PLP and RTS ($C119-$C13A) */
    maybe_trigger_vblank(52);
    func_FF72();                                /* its RTS pops $C133 */
    maybe_trigger_vblank(26);
    (void)pull();                               /* PLA/TAY, PLA/TAX, PLA, PLP */
    (void)pull();
    (void)pull();
    (void)pull();
    (void)pull();                               /* RTS: the faked return address */
    (void)pull();
    g_cpu.A = saved.A; g_cpu.X = saved.X; g_cpu.Y = saved.Y;
    g_cpu.N = saved.N; g_cpu.V = saved.V; g_cpu.D = saved.D;
    g_cpu.I = saved.I; g_cpu.Z = saved.Z; g_cpu.C = saved.C;
}
