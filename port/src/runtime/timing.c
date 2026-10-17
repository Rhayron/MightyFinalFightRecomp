/*
 * timing.c - the cycle clock: where a frame ends, and how an interrupt
 * enters the game's code
 *
 * Every run of instructions charges its cycles (nes_instruction_boundary).
 * A frame is 341 x 262 PPU dots, 89,342, or one less on odd frames with
 * rendering on, which is 29,780 or 29,781 CPU cycles with the remainder
 * dots carried over. When a charge reaches the frame's length the frame
 * boundary happens right there, between two runs of instructions: the
 * vblank flag goes up and, if PPUCTRL enables it, frame.c delivers the NMI
 * and draws the picture before the interrupted code continues.
 *
 * Every boundary also closes the frame's sound (apu.c), NMI or not.
 *
 * The overshoot of the charge that crossed the boundary counts toward the
 * next frame. The game may hold the NMI back (g_nmi_veto: mff_hooks.c says
 * where it may land); then the boundary waits at the threshold for the
 * first instruction the game allows.
 *
 * Depth: 0 in main code, 1 at the boundary (the frame's callback and its
 * raster IRQs), 2 inside the NMI handler, POST_NMI_DEPTH while the NMI's
 * hijacked return runs (nmi_tail and the sound update): no frame boundary
 * can start inside any of them. One running more than half a frame past
 * its end would need a nested NMI, which this game never has; the runtime
 * stops if it happens.
 */
#include "machine.h"
#include <stdio.h>

uint64_t g_nes_cycles;
uint64_t g_frame_count;
int (*g_nmi_veto)(uint16_t pc);

#define FRAME_DOTS 89342

static uint32_t s_ops;                  /* cycles into this frame */
static uint32_t s_length;               /* this frame's length in cycles */
static int s_started;
static int s_odd;                       /* this is an odd frame */
static int s_dots;                      /* dots not yet turned into cycles (0-2) */
static int s_depth, s_saved_depth;
static int s_pending;                   /* a boundary passed inside an interrupt */
static int s_dma_stall;                 /* OAM DMA cycles, charged with the next run */
static uint16_t s_pc;                   /* the CPU address about to run */
static uint64_t s_epoch;                /* interrupts entered so far */
static int s_in_irq;

static uint32_t next_frame_length(void) {
    int dots = FRAME_DOTS - ((s_odd && (ppu_mask() & 0x18)) ? 1 : 0);
    uint32_t cycles;
    s_odd ^= 1;
    s_dots += dots;
    cycles = (uint32_t)(s_dots / 3);
    s_dots -= (int)cycles * 3;
    return cycles;
}

static void frame_boundary(void) {
    s_ops -= s_length;
    s_length = next_frame_length();
    s_depth++;
    ppu_vblank_begin();
    apu_frame_boundary(s_length);
    if (ppu_ctrl() & 0x80) frame_run();
    s_depth--;
}

void maybe_trigger_vblank(int cycles) {
    uint32_t c;
    if (!s_started) {
        s_started = 1;
        s_length = next_frame_length();
    }
    c = (uint32_t)(cycles > 0 ? cycles : 1) + (uint32_t)s_dma_stall;
    s_dma_stall = 0;
    s_ops += c;
    g_nes_cycles += c;
    if (s_ops < s_length) return;
    if (s_depth >= POST_NMI_DEPTH) return;
    if (s_depth > 0) {
        s_pending = 1;
        if (s_ops > s_length + s_length / 2)
            mff_fatal("an interrupt ran %u cycles into a %u-cycle frame (a nested NMI, which the "
                      "runtime does not model)", s_ops, s_length);
        return;
    }
    if (g_nmi_veto && g_nmi_veto(s_pc)) {
        s_ops = s_length;               /* wait at the threshold, no debt */
        return;
    }
    frame_boundary();
}

void nes_cpu_instruction_boundary(uint16_t pc, int cycles) {
    s_pc = pc;
    maybe_trigger_vblank(cycles);
}

void nes_instruction_boundary(uint16_t pc, int cycles) {
    if (pc >= 0x8000) pc = (uint16_t)(g_code_window_base | (pc & 0x1FFF));
    nes_cpu_instruction_boundary(pc, cycles);
}

void runtime_begin_post_nmi(void) {
    s_pending = 0;
    s_saved_depth = s_depth;
    s_depth = POST_NMI_DEPTH;
}

void runtime_end_post_nmi(void) { s_depth = s_saved_depth; }

int timing_depth(void) { return s_depth; }
void timing_set_depth(int d) { s_depth = d; }
void timing_reset_depth(void) { s_depth = 0; s_pending = 0; }
uint64_t timing_interrupt_epoch(void) { return s_epoch; }
void timing_note_interrupt(void) { s_epoch++; }
uint32_t timing_frame_cycles(void) { return s_ops; }
uint16_t timing_pc(void) { return s_pc; }
int timing_in_irq(void) { return s_in_irq; }

/* 513 cycles, 514 when the write lands on an odd cycle */
void timing_add_dma_stall(void) { s_dma_stall += 513 + (int)(g_nes_cycles & 1); }

/* The MMC3 raised the IRQ line (frame.c, the scanline pass): push the
 * interrupt frame on the stack of whatever code was running, enter the
 * handler, and give that code its registers back afterwards. The pushed
 * return address is 0: the handler leaves with RTI, which only pops it. */
void timing_call_irq_handler(void) {
    uint8_t s = g_cpu.S, a = g_cpu.A, x = g_cpu.X, y = g_cpu.Y;
    uint8_t n = g_cpu.N, v = g_cpu.V, d = g_cpu.D, i = g_cpu.I, z = g_cpu.Z, c = g_cpu.C;
    uint8_t p;
    if (s_in_irq) return;
    s_epoch++;
    p = (uint8_t)((n << 7) | (v << 6) | 0x20 | (d << 3) | (i << 2) | (z << 1) | c);
    g_ram[0x100 + g_cpu.S] = 0x00; g_cpu.S--;
    g_ram[0x100 + g_cpu.S] = 0x00; g_cpu.S--;
    g_ram[0x100 + g_cpu.S] = p;    g_cpu.S--;
    g_cpu.I = 1;
    s_in_irq = 1;
    func_IRQ();
    /* a handler that left without its RTI: run on from the return address
     * it consumed last, as the scaffold does (never seen in this game) */
    for (int k = 0; k < 8 && g_cpu.S < s; k++) {
        uint16_t ret = (uint16_t)(((g_ram[0x100 + g_cpu.S] << 8) |
                                   g_ram[0x100 + ((g_cpu.S - 1) & 0xFF)]) + 1);
        if (ret < 0x8000 || !call_by_address(ret)) break;
    }
    s_in_irq = 0;
    g_cpu.S = s; g_cpu.A = a; g_cpu.X = x; g_cpu.Y = y;
    g_cpu.N = n; g_cpu.V = v; g_cpu.D = d; g_cpu.I = i; g_cpu.Z = z; g_cpu.C = c;
}
