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
 *    does what matters to the game: sound_update, with A/X/Y/P kept.
 */
#include "machine.h"
#include <stdlib.h>

static int nmi_held(uint16_t pc) {
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

void func_C119(void) {
    CPU6502State saved = g_cpu;
    func_FF72();
    g_cpu.A = saved.A; g_cpu.X = saved.X; g_cpu.Y = saved.Y;
    g_cpu.N = saved.N; g_cpu.V = saved.V; g_cpu.D = saved.D;
    g_cpu.I = saved.I; g_cpu.Z = saved.Z; g_cpu.C = saved.C;
}
