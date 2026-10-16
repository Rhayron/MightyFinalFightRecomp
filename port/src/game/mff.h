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
 * before it; segments split where an interrupt reads what the routine writes
 * (a guard such as bank_switch_busy). */
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

static inline void mff_nz(uint8_t v) {
    g_cpu.N = (v & 0x80) ? 1 : 0;
    g_cpu.Z = v == 0;
}

#endif
