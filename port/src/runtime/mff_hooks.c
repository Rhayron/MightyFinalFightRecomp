/*
 * mff_hooks.c - two game rules the scaffold kept in port/scaffold/extras.c,
 * and the choice of clock (--slowdown, --timing)
 *
 * The rules belong with the game's code; they are here until port/src/game
 * takes them.
 *
 * 1. Where the NMI may land, with the scaffold's timing only (--timing
 *    scaffold; MFF_LAG=1 turns the rule off): never while a task runs
 *    (state 2 in $80+4n), so no lag frame; around the scheduler only at the
 *    top of its scan ($FEBD), where the registers are always the same (the
 *    sound driver keeps the interrupted Y in $C3, and the place in the
 *    scan followed cycle counts the scaffold only kept per block); not in
 *    task_wait_frame's tail or the IRQ handlers either; elsewhere (reset's
 *    $2002 polling) it is not held. The console's timing (timing.c) has no
 *    such rule: the NMI lands where the vblank finds the game, in a task's
 *    frame work too (a lag frame), and runs nested on that task's C stack,
 *    as on the 6502 its frame goes on the task's part of the stack page.
 *    The scaffold held it mid-task because "an NMI in the middle of a task
 *    cannot be modelled on C stacks": what broke was the scaffold's
 *    nmi_tail (2. below), whose RTS popped two bytes it never pushed. At
 *    $FEBD the scheduler resets S anyway; inside a task the task then
 *    returned through the wrong bytes (inputs/10_cody_poweron.txt, NMI
 *    11,338: S = $01; this runtime with that nmi_tail and the console's
 *    timing stops at the same frame with S = $01, docs/timing.md).
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
#include <string.h>

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

/* The clock (timing.c, docs/timing.md). The command line wins over the
 * environment:
 *   --slowdown on   (MFF_SLOWDOWN=1, the default) the console's timing: the
 *                   NMI lands where the console's does, and a frame whose
 *                   work does not fit lags as on the console
 *   --slowdown off  (MFF_SLOWDOWN=0) the same clock, but the NMI waits for a
 *                   running task: no lag frames, the game always at full speed
 *   --timing scaffold  (MFF_TIMING=scaffold) the scaffold runtime's timing,
 *                   which the P4 regression baselines were recorded with
 *                   (port/tests/run_routes.sh): no lag, the NMI only at the
 *                   scheduler's $FEBD (rule 1 above); MFF_LAG=1 drops the rule
 *   --timing console  the console's (as --slowdown on) */
static int s_timing_set;

static int timing_choice(const char *name, const char *value) {
    int on = !strcmp(value, "on") || !strcmp(value, "1");
    int off = !strcmp(value, "off") || !strcmp(value, "0");
    if (!strcmp(name, "slowdown") && (on || off)) {
        g_timing = on ? TIMING_CONSOLE : TIMING_NOLAG;
        return 1;
    }
    if (!strcmp(name, "timing") && (!strcmp(value, "console") || !strcmp(value, "scaffold"))) {
        g_timing = !strcmp(value, "console") ? TIMING_CONSOLE : TIMING_SCAFFOLD;
        return 1;
    }
    return 0;
}

int mff_timing_option(const char *name, const char *value) {
    if (!timing_choice(name, value)) return 0;
    s_timing_set = 1;
    return 1;
}

void mff_hooks_init(void) {
    const char *lag = getenv("MFF_LAG"), *slowdown = getenv("MFF_SLOWDOWN"), *timing = getenv("MFF_TIMING");
    if (!s_timing_set) {
        if (timing && *timing && !timing_choice("timing", timing))
            mff_fatal("MFF_TIMING=%s: console or scaffold", timing);
        if (slowdown && *slowdown && !timing_choice("slowdown", slowdown))
            mff_fatal("MFF_SLOWDOWN=%s: 1 (on) or 0 (off)", slowdown);
    }
    g_nmi_veto = (g_timing != TIMING_SCAFFOLD || (lag && *lag == '1')) ? NULL : nmi_held;
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
