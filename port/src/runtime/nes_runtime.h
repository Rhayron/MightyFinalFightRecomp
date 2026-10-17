/*
 * nes_runtime.h - the machine as the game code sees it (PLAN_DECOMP.md P5)
 *
 * port/src/game and port/src/glue/dispatch.c were written against the
 * scaffold's runtime (NESRecomp's legacy runner) and include this header by
 * that name. This is the port's own runtime: the same interface, implemented
 * in port/src/runtime for this one game, so the routines need no change.
 *
 *  - CPU: g_cpu (A/X/Y/S and the flags as separate bytes) and g_ram, the
 *    2 KiB of work RAM. The 6502 stack is page 1 of g_ram.
 *  - Bus: nes_read/nes_write decode the address like the console (RAM, PPU,
 *    APU and pads, cartridge RAM, PRG ROM through the MMC3, mapper writes).
 *  - Clock: a routine charges the cycles of each run of instructions with
 *    nes_instruction_boundary (layout PC, mapped through the code window) or
 *    nes_cpu_instruction_boundary (a CPU PC). The frame ends on a cycle
 *    count; the NMI is delivered there (runtime/timing.c).
 *  - Dispatch: a JSR or JMP to an address known at run time goes through
 *    nes_dispatch_call / call_by_address_tail, which resolve the bank mapped
 *    at that address (dispatch.c's table) and keep a JMP loop from growing
 *    the C stack (runtime/dispatch_rt.c).
 */
#ifndef MFF_NES_RUNTIME_H
#define MFF_NES_RUNTIME_H

#include <stdint.h>

/* ---- CPU ---- */
typedef struct {
    uint8_t A, X, Y, S, P;      /* P is unused: the flags live below */
    uint8_t N, V, D, I, Z, C;
} CPU6502State;

extern CPU6502State g_cpu;
extern uint8_t g_ram[0x0800];

/* ---- bus ---- */
uint8_t nes_read(uint16_t addr);
void nes_write(uint16_t addr, uint8_t val);
uint16_t nes_read16zp(uint8_t zp);          /* a zero-page pointer, wrapping at $FF */

/* ---- clock ---- */
/* Charge `cycles` at layout PC `pc` (mapped through g_code_window_base for
 * $8000+) or at CPU PC `pc`; the frame boundary and the NMI happen here. */
void nes_instruction_boundary(uint16_t pc, int cycles);
void nes_cpu_instruction_boundary(uint16_t pc, int cycles);
/* Charge cycles without moving the PC the NMI rule sees. */
void maybe_trigger_vblank(int cycles);

/* ---- dispatch ---- */
/* CPU window ($8000/$A000/$C000/$E000) the running routine lives in; JSR
 * return addresses are minted from it. */
extern uint16_t g_code_window_base;
/* 8 KiB PRG bank behind each CPU window, $8000/$A000/$C000/$E000 */
extern int g_mmc3_win_bank8k[4];

int nes_dispatch_call(uint16_t addr, int caller_bank);
int call_by_address_tail(uint16_t addr, int caller_bank);
/* in dispatch.c (port/tools/dispatch_gen.py) */
int call_by_address(uint16_t addr);
int call_by_address_cb(uint16_t addr, int caller_bank);
/* dispatch.c's misses: no routine at that address. There is no interpreter,
 * so these stop the run and say where. */
int nes_interp_dispatch(uint16_t addr);
int nes_interp_dispatch_bank(uint16_t cpu_addr, uint16_t gen_addr, int bank);

/* ---- interrupts ---- */
void func_RESET(void);
void func_NMI(void);
void func_IRQ(void);
extern uint16_t g_rti_target;               /* RTI's target (dispatch.c) */
/* around the code an NMI runs after its RTI (the hijacked return) */
void runtime_begin_post_nmi(void);
void runtime_end_post_nmi(void);
/* the game's rule for where the NMI may land (runtime/mff_hooks.c) */
extern int (*g_nmi_veto)(uint16_t pc);

/* ---- pads, frames ---- */
/* bit 7 A, 6 B, 5 Select, 4 Start, 3 Up, 2 Down, 1 Left, 0 Right */
extern uint8_t g_controller1_buttons;
extern uint8_t g_controller2_buttons;
extern uint64_t g_frame_count;              /* frames shown since power-on */
extern uint64_t g_nes_cycles;               /* CPU cycles since power-on */

#endif
