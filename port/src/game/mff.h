/*
 * mff.h - shared helpers for the hand-written routines (PLAN_DECOMP.md P4)
 *
 * Each routine here replaces one 6502 routine of the scaffold through
 * [[replace_func]] in port/scaffold/game.base.toml. In P4 a replacement must
 * leave the machine exactly as the original does: RAM (temporaries such as
 * $00 included), A/X/Y, the flags, S, and the cycles it costs, because the
 * scaffold ends a frame on a cycle count. port/tests/run_routes.sh check
 * compares every frame against the unrefactored scaffold.
 */
#ifndef MFF_H
#define MFF_H

#include "nes_runtime.h"
#include <stdint.h>

extern uint16_t g_rts_target;   /* the runtime reads it after a call returns */

/* CPU RAM, $0000-$07FF (the routines here only touch internal RAM directly) */
#define RAM(a) g_ram[(a) & 0x7FF]

/* Charge `cycles` CPU cycles at `pc`, as the generated code charges each
 * instruction before running it (frame end, MMC3 and APU clocks run from
 * this). The scaffold's model: each opcode's base count, +1 for a taken
 * branch, no page-cross penalties. A routine charges a segment and then does
 * that segment's work, so an interrupt landing in the segment sees the state
 * before it. Segments split
 *  - where an interrupt reads what the routine writes (a guard such as
 *    bank_switch_busy);
 *  - at every push, pull and call: an IRQ (the MMC3 split, never held)
 *    pushes its frame at S, so it must see S where the original had it;
 *  - instruction by instruction wherever the NMI is not held: outside a
 *    running task, e.g. task_wait_frame after it marks its task waiting. */
static inline void mff_cycles(uint16_t pc, int cycles) {
    nes_instruction_boundary(pc, cycles);
}

/* RTS under push_all_jsr: the caller pushed the return address. */
static inline void mff_rts(void) {
    uint8_t lo, hi;
    g_cpu.S++; lo = g_ram[0x100 + g_cpu.S];
    g_cpu.S++; hi = g_ram[0x100 + g_cpu.S];
    g_rts_target = (uint16_t)((hi << 8) | lo);
}

/* JSR under push_all_jsr, as the generated code does it: push the return
 * address (the JSR at `site`, so site + 2, in the current code window),
 * dispatch to `target` (generated or hand-written), and report whether the
 * stack came back balanced. 0 means the callee left through the scheduler
 * (a task switch unwinding C frames): the caller must return at once.
 * The JSR's 6 cycles belong to the caller's segment. */
static inline int mff_call(uint16_t site, uint16_t target) {
    uint8_t s = g_cpu.S;
    uint16_t ret = (uint16_t)(g_code_window_base | ((site + 2) & 0x1FFF));
    g_ram[0x100 + g_cpu.S] = (uint8_t)(ret >> 8); g_cpu.S--;
    g_ram[0x100 + g_cpu.S] = (uint8_t)ret; g_cpu.S--;
    if (!nes_dispatch_call(target, -1)) g_cpu.S += 2;
    return g_cpu.S == s;
}

/* A call into the task yield routines (task_wait_frame and its entries):
 * the scheduler's resume path changes S across the yield, so there is no
 * balance check - the generated callers skip it too. */
static inline void mff_call_yield(uint16_t site, uint16_t target) {
    uint16_t ret = (uint16_t)(g_code_window_base | ((site + 2) & 0x1FFF));
    g_ram[0x100 + g_cpu.S] = (uint8_t)(ret >> 8); g_cpu.S--;
    g_ram[0x100 + g_cpu.S] = (uint8_t)ret; g_cpu.S--;
    if (!nes_dispatch_call(target, -1)) g_cpu.S += 2;
}

static inline void mff_push(uint8_t v) { g_ram[0x100 + g_cpu.S] = v; g_cpu.S--; }

static inline void mff_nz(uint8_t v) {
    g_cpu.N = (v & 0x80) ? 1 : 0;
    g_cpu.Z = v == 0;
}

#endif
