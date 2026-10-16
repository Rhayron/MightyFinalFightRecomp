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
#include <stdio.h>

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
static inline uint8_t mff_pull(void) { g_cpu.S++; return g_ram[0x100 + g_cpu.S]; }

/* the flags as PHP pushes them (B and bit 5 set) and as PLP/RTI take them */
static inline uint8_t mff_p(void) {
    return (uint8_t)((g_cpu.N << 7) | (g_cpu.V << 6) | 0x30 | (g_cpu.D << 3) |
                     (g_cpu.I << 2) | (g_cpu.Z << 1) | g_cpu.C);
}
static inline void mff_set_p(uint8_t p) {
    g_cpu.N = (p >> 7) & 1; g_cpu.V = (p >> 6) & 1; g_cpu.D = (p >> 3) & 1;
    g_cpu.I = (p >> 2) & 1; g_cpu.Z = (p >> 1) & 1; g_cpu.C = p & 1;
}

/* RTI as the generated code does it: flags and PC off the stack; the PC
 * goes to g_rti_target (func_NMI runs it when an NMI hijacked its return). */
extern uint16_t g_rti_target, g_rti_source;
extern int g_rti_bank;
static inline void mff_rti(uint16_t pc) {
    uint8_t lo, hi;
    g_rti_source = pc;
    g_rti_bank = 7;
    mff_set_p(mff_pull());
    lo = mff_pull();
    hi = mff_pull();
    g_rti_target = (uint16_t)((hi << 8) | lo);
}

/* JMP to code the scaffold keeps as a function of its own: the generator
 * charges the target 2 more cycles before its tail call. */
static inline void mff_jmp_charge(uint16_t pc, uint16_t target) {
    mff_cycles(pc, 3);
    nes_cpu_instruction_boundary(target, 2);
}

/* ---- for routines lifted by port/tools/lift.py ----
 * A/X/Y and the flags live in locals; they meet g_cpu at calls and exits. */
#define REGS_IN \
    uint8_t a = g_cpu.A, x = g_cpu.X, y = g_cpu.Y, t = 0, m = 0; \
    int w = 0, n = g_cpu.N, z = g_cpu.Z, c = g_cpu.C, v = g_cpu.V; \
    (void)t; (void)m; (void)w; (void)n; (void)z; (void)c; (void)v
#define FLAGS_OUT (g_cpu.N = (uint8_t)n, g_cpu.Z = (uint8_t)z, g_cpu.C = (uint8_t)c, g_cpu.V = (uint8_t)v)
#define FLAGS_IN (n = g_cpu.N, z = g_cpu.Z, c = g_cpu.C, v = g_cpu.V)
#define SYNC_OUT (g_cpu.A = a, g_cpu.X = x, g_cpu.Y = y, FLAGS_OUT)
#define SYNC_IN (a = g_cpu.A, x = g_cpu.X, y = g_cpu.Y, FLAGS_IN)

/* a pointer in zero page (wraps at $FF like the 6502) */
#define RAM16(zp) ((uint16_t)(g_ram[(zp) & 0xFF] | g_ram[((zp) + 1) & 0xFF] << 8))

#define mff_rti_bank(pc, b) (mff_rti(pc), g_rti_bank = (b))

/* JSR as the generator emits it: push (window | rp_low), then dispatch with
 * its bank argument, or call a function directly in a given window (0 =
 * keep). 1 if the stack came back balanced. */
static inline int mff_call_at(uint16_t rp_low, uint16_t target, int bank) {
    uint8_t s = g_cpu.S;
    uint16_t ret = (uint16_t)(g_code_window_base | rp_low);
    g_ram[0x100 + g_cpu.S] = (uint8_t)(ret >> 8); g_cpu.S--;
    g_ram[0x100 + g_cpu.S] = (uint8_t)ret; g_cpu.S--;
    if (!nes_dispatch_call(target, bank)) g_cpu.S += 2;
    return g_cpu.S == s;
}
static inline int mff_call_fn(uint16_t rp_low, void (*fn)(void), uint16_t window) {
    uint8_t s = g_cpu.S;
    uint16_t ret = (uint16_t)(g_code_window_base | rp_low);
    g_ram[0x100 + g_cpu.S] = (uint8_t)(ret >> 8); g_cpu.S--;
    g_ram[0x100 + g_cpu.S] = (uint8_t)ret; g_cpu.S--;
    if (window) {
        uint16_t swb = g_code_window_base;
        g_code_window_base = window;
        fn();
        g_code_window_base = swb;
    } else {
        fn();
    }
    return g_cpu.S == s;
}

/* A lifted path that runs into data (a branch the game always takes, decoded
 * on by the generator). Never reached in any route; say so if it is. */
static inline void mff_unreachable(uint16_t pc) {
    fprintf(stderr, "[mff] lifted code reached data at $%04X\n", pc);
}

static inline void mff_nz(uint8_t v) {
    g_cpu.N = (v & 0x80) ? 1 : 0;
    g_cpu.Z = v == 0;
}

#endif
