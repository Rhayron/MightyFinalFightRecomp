/*
 * timing.c - the cycle clock: where a frame ends, where the NMI and the
 * raster IRQ land, and how an interrupt enters the game's code
 * (docs/timing.md)
 *
 * Every run of instructions charges its cycles (nes_instruction_boundary,
 * at the run's first PC) before the routine does the run's work; a hardware
 * access ends its run, so it happens on that run's last cycle. A frame is
 * 341 x 262 PPU dots, 89,342, one less when the pre-render line of an odd
 * frame has rendering on; the vblank starts at scanline 241 dot 1.
 *
 * Three clocks (g_timing, set by mff_hooks.c from --slowdown/--timing):
 *
 * TIMING_CONSOLE (the default) is the console's, cycle for cycle with the
 * oracle (the NMI lands at its cycle and PC on every frame of the routes):
 *  - the first vblank is at cycle 27,394 from power-on (dot 82,182; the
 *    reset sequence's 7 cycles count), each next one a frame of dots later,
 *    the odd-frame dot decided when the pre-render line gets there (cycle
 *    ~2,386 of the frame, where a long NMI still has rendering off), the
 *    vblank flag down again at that line's dot 1;
 *  - a hardware access sees the vblank (the flag, the frame's sound) when it
 *    came before the end of the access's cycle;
 *  - the NMI lands at the first run boundary where the 6502 would take it:
 *    the vblank at least a cycle before the instruction ended (a taken
 *    branch that stays in its page polls a cycle earlier: the runtime
 *    knows one by its extra cycle, charged as a run of 1, which is never a
 *    landing place), wherever the game is: in task 0's frame work too,
 *    which is a lag frame. The NMI runs nested on that task's C stack,
 *    and pushes the PC it interrupted;
 *  - the MMC3's raster IRQ is taken the same way, at its scanline
 *    (frame.c), when the I flag is clear;
 *  - cycles are the console's: the interrupt sequences' 7, a page-crossing
 *    indexed read's 1 (nes_page_cross), the OAM DMA's 513/514 by the
 *    write's cycle; not the 2 the scaffold adds to a JMP it made a tail call
 *    (nes_cpu_instruction_boundary from the game) or its minimum charge of 1.
 * With slowdown off (TIMING_NOLAG) the clock is the same but the vblank
 * waits while a task runs, at the threshold and without debt, so a frame
 * that does not fit just takes longer: the game never lags.
 *
 * TIMING_SCAFFOLD is the scaffold runtime's, which the P4 regression routes
 * (port/tests/run_routes.sh) were recorded with: the first frame a full
 * one, the odd-frame dot decided at the boundary, the boundary taken right
 * when a charge reaches the frame's length (before that run's work), the
 * scanline pass all at once after the NMI, charges as the scaffold makes
 * them (no reset or interrupt sequences), and the NMI held where
 * g_nmi_veto says (mff_hooks.c: never in a task, only at the scheduler's
 * $FEBD).
 *
 * Depth: 0 in main code, 1 at the boundary (the frame's callback), 2 inside
 * the NMI handler, POST_NMI_DEPTH while the NMI's hijacked return runs
 * (nmi_tail and the sound update): no NMI can land inside any of them. One
 * running more than half a frame past its end would need a nested NMI,
 * which this game never has; the runtime stops if it happens.
 */
#include "machine.h"
#include "render_hd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t g_nes_cycles;
uint64_t g_frame_count;
int (*g_nmi_veto)(uint16_t pc);
int g_timing = TIMING_CONSOLE;

#define FRAME_DOTS 89342
#define FIRST_VBLANK 27394u             /* (241 * 341 + 1) / 3: scanline 241, dot 1 */
#define VBLANK_DOTS 6820                /* scanline 241 dot 1 to 261 dot 1: the flag goes down */
#define PRERENDER_DOTS 7158             /* scanline 241 dot 1 to 261 dot 339: the odd-frame skip */

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
/* the console clock: s_ops counts from the last vblank */
static int s_frame_dots;                /* this frame's dots */
static uint32_t s_vbl_end;              /* cycles into the frame where the vblank flag goes down */
static uint32_t s_prerender;            /* cycles into the frame of the odd-frame decision */
static int s_prerender_due;             /* 2: the flag still up, 1: the skip not decided, 0: done */
static int s_nmi_due;                   /* raised, not landed yet */
static uint32_t s_held;                 /* cycles the vblank waited (slowdown off) */
static uint64_t s_vblanks;
static uint32_t s_access_dots = 3;      /* dots of its last cycle after which an access sees the vblank */

/* ---- the scaffold's clock ---- */

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

static void scaffold_charge(int cycles) {
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

/* ---- the console's clock ----
 *
 * s_ops counts cycles from the last vblank's cycle; s_dots is that vblank's
 * dot inside its cycle (0-2), so the vblank sits at dot 3 * 0 + s_dots and
 * the next one at s_next_dot, in thirds of a cycle from the same place.
 * Measured against the oracle (docs/timing.md):
 *  - a hardware access happens on its run's last cycle, and sees the vblank
 *    (the flag, the frame's sound) when the vblank's dot came before the
 *    end of that cycle;
 *  - the 6502 takes the NMI after an instruction if the vblank came at
 *    least 3 dots (one cycle) before the instruction ended: before its last
 *    cycle, where it polls. A taken branch that stays in its page polls a
 *    cycle earlier (3 dots more): the runtime knows one by its extra cycle,
 *    charged as a run of 1 (the scheduler's PC-less charge too). */

#define NMI_POLL_DOTS 3                 /* the vblank this far before a boundary: the NMI lands there */
#define BRANCH_POLL_DOTS 3              /* a taken branch polls one cycle earlier */

static int s_branch;                    /* the last charge was a taken branch's extra cycle */

/* the frame from this vblank to the next: its length in cycles from the
 * dots, the vblank's place inside its cycle carried over (s_dots) */
static void console_frame_start(int dots) {
    s_frame_dots = dots;
    s_length = (uint32_t)((s_dots + dots) / 3);
    s_vbl_end = (uint32_t)((s_dots + VBLANK_DOTS) / 3);
    s_prerender = (uint32_t)((s_dots + PRERENDER_DOTS) / 3);
    s_prerender_due = 2;
}

/* the pre-render line's last dot, skipped on odd frames with rendering on */
static void console_prerender(void) {
    s_prerender_due = 0;
    if (s_odd && (ppu_mask() & 0x18)) {
        console_frame_start(FRAME_DOTS - 1);
        s_prerender_due = 0;
        apu_frame_length(s_length);
    }
    s_odd ^= 1;
}

static void console_add(int cycles) {
    uint32_t c = (uint32_t)cycles + (uint32_t)s_dma_stall;
    s_dma_stall = 0;
    s_ops += c;
    g_nes_cycles += c;
    s_branch = cycles == 1;
}

/* MFF_TIMELOG=FILE: "V cycle dot nmi" at each vblank (its CPU cycle, the
 * dot inside it, NMI enabled), "L cycles PC task0" where the NMI lands
 * (cycles past the vblank, the PC, task 0's state: 2 is a lag frame) */
static FILE *s_timelog;
static int s_timelog_open;
static uint8_t s_task0_state;
static uint64_t s_task0_resumed;

static FILE *timelog(void) {
    if (!s_timelog_open) {
        const char *p = getenv("MFF_TIMELOG");
        s_timelog_open = 1;
        if (p && *p && !(s_timelog = fopen(p, "w"))) mff_fatal("MFF_TIMELOG: cannot write %s", p);
    }
    return s_timelog;
}

static int task_running(void) {
    for (int t = 0; t < 4; t++)
        if (g_ram[0x80 + 4 * t] == 2) return 1;
    return 0;
}

/* the vblank: the flag goes up, the frame's sound closes, and if PPUCTRL
 * enables it the NMI is raised, to land at a run boundary */
static void console_vblank(void) {
    int carry = s_dots + s_frame_dots - 3 * (int)s_length;
    frame_vblank();                     /* the frame's last lines, its picture */
    s_ops -= s_length;
    s_dots = carry;
    console_frame_start(FRAME_DOTS);
    ppu_vblank_begin();
    apu_frame_boundary(s_length);
    s_nmi_due = (ppu_ctrl() & 0x80) != 0;
    s_vblanks++;
    if (timelog()) fprintf(s_timelog, "V %llu %d %d\n", (unsigned long long)(g_nes_cycles - s_ops), s_dots, s_nmi_due);
}

/* bring the PPU's side of the clock up to `dot` (thirds of a cycle from
 * the last vblank's cycle): the vblank, the pre-render line. With slowdown
 * off the vblank waits for a running task, at the threshold and without
 * debt. */
static void console_sync(uint32_t dot) {
    uint32_t next = (uint32_t)(s_dots + s_frame_dots);
    if (dot >= next) {
        /* reset ($FE00-$FEB9) is never held: after the title's JMP reset it
         * runs with task 0 still "running" and polls for the vblank */
        if (g_timing == TIMING_NOLAG && task_running() && !(s_pc >= 0xFE00 && s_pc < 0xFEBA)) {
            uint32_t wait = s_length - 1;
            s_held += s_ops - wait;
            s_ops = wait;
            dot = 3 * wait;
        } else {
            uint32_t len = s_length;
            console_vblank();
            dot -= 3 * len;             /* now from the new vblank's cycle */
        }
    }
    frame_scanlines((int32_t)dot - s_dots);
    if (s_prerender_due == 2 && s_ops >= s_vbl_end) {
        s_prerender_due = 1;
        ppu_set_status(0, 0x80);        /* the pre-render line's dot 1 */
    }
    if (s_prerender_due == 1 && s_ops >= s_prerender) console_prerender();
}

/* a hardware access (bus.c), on the last cycle of the run that just charged.
 * The access is the run's last instruction (4 cycles: an absolute LDA or
 * STA): a raster IRQ raised early enough in the instruction before it is
 * taken first, as the console takes it between the two (a write to the
 * sound would otherwise come before the IRQ's 106 cycles instead of after) */
#define ACCESS_CYCLES 4
void timing_sync(void) {
    int32_t before;
    if (g_timing == TIMING_SCAFFOLD || !s_started) return;
    console_sync(3 * s_ops - (3 - s_access_dots));
    before = (int32_t)(3 * (s_ops - ACCESS_CYCLES)) - s_dots;
    if (!s_in_irq && !g_cpu.I && (s_depth == 0 || s_depth == POST_NMI_DEPTH) && s_ops >= ACCESS_CYCLES &&
        frame_irq_due(before, NMI_POLL_DOTS))
        frame_irq((int32_t)(3 * s_ops) - s_dots);
}

/* the frame's code wrote PPUMASK (bus.c): a picture region from the next line */
void timing_mask_write(void) {
    if (g_timing != TIMING_SCAFFOLD && s_started)
        frame_mask_write((int32_t)(3 * s_ops - (3 - s_access_dots)) - s_dots);
}

/* PPUCTRL turned the NMI on while the vblank flag is up: it fires now */
void timing_nmi_enabled(void) {
    if (g_timing != TIMING_SCAFFOLD) s_nmi_due = 1;
}

/* a run of instructions starts at s_pc: the NMI lands here if it was raised
 * early enough in the instruction before (see above) */
static void console_check(void) {
    uint32_t need = (uint32_t)(NMI_POLL_DOTS + (s_branch ? BRANCH_POLL_DOTS : 0));
    console_sync(3 * s_ops);
    if (s_timelog) {
        /* task 0 left state 2: its frame's work is done ("Y cycles past the
         * last vblank, vblanks since it was resumed": 1+ is a lag frame) */
        uint8_t st = g_ram[0x80];
        if (st == 2 && s_task0_state != 2) s_task0_resumed = s_vblanks;
        else if (st != 2 && s_task0_state == 2)
            fprintf(s_timelog, "Y %u %llu\n", s_ops, (unsigned long long)(s_vblanks - s_task0_resumed));
        s_task0_state = st;
    }
    if (s_nmi_due && 3 * s_ops >= (uint32_t)s_dots + need) {
        if (s_depth == 0) {
            s_nmi_due = 0;
            if (timelog()) fprintf(s_timelog, "L %u %04X %u\n", s_ops, s_pc, g_ram[0x80]);
            if (ppu_ctrl() & 0x80) {        /* not turned off again before it landed */
                s_depth++;
                frame_run();
                s_depth--;
            }
        } else if (s_depth < POST_NMI_DEPTH && s_ops > s_length / 2) {
            mff_fatal("an interrupt ran %u cycles into a %u-cycle frame (a nested NMI, which the "
                      "runtime does not model)", s_ops, s_length);
        }
    }
    /* the MMC3's IRQ, the same way, when the I flag lets it in: in main
     * code and in nmi_tail, not in the NMI handler or another IRQ (after an
     * NMI here, the poll is the one after its last instruction, as a state
     * loaded at this point sees it) */
    need = (uint32_t)(NMI_POLL_DOTS + (s_branch ? BRANCH_POLL_DOTS : 0));
    if (!s_in_irq && frame_irq_due((int32_t)(3 * s_ops) - s_dots, (int32_t)need)) {
        if (!g_cpu.I && (s_depth == 0 || s_depth == POST_NMI_DEPTH)) frame_irq((int32_t)(3 * s_ops) - s_dots);
        else frame_irq_blocked();
    }
}

static void console_start(void) {
    s_started = 1;
    s_dots = 0;
    s_odd = 0;
    console_frame_start(FIRST_VBLANK * 3);
    s_prerender_due = 0;                /* the PPU starts at line 0: no pre-render line before it */
}

/* ---- the interface ---- */

void maybe_trigger_vblank(int cycles) {
    if (g_timing == TIMING_SCAFFOLD) {
        scaffold_charge(cycles);
        return;
    }
    /* cycles inside an instruction or an interrupt sequence: no landing place */
    if (!s_started) console_start();
    console_add(cycles);
}

/* MFF_RUNTRACE=FILE:FIRST:LAST: every run of frames FIRST-LAST (g_frame_count)
 * as "cycle PC cycles", to set beside the oracle's --trace-out (docs/timing.md) */
static FILE *s_runtrace;
static long s_rt_first = -1, s_rt_last = -1;
static int s_rt_open;

static void runtrace(uint16_t pc, int cycles) {
    if (!s_rt_open) {
        const char *p = getenv("MFF_RUNTRACE");
        static char path[1024];
        s_rt_open = 1;
        if (p && *p) {
            const char *c1, *c2;
            snprintf(path, sizeof path, "%s", p);
            c2 = strrchr(path, ':');
            if (c2 && c2 > path) {
                *(char *)c2 = 0;
                c1 = strrchr(path, ':');
                if (c1 && c1 > path + 1) {
                    *(char *)c1 = 0;
                    s_rt_first = atol(c1 + 1);
                    s_rt_last = atol(c2 + 1);
                    s_runtrace = fopen(path, "w");
                }
            }
            if (!s_runtrace) mff_fatal("MFF_RUNTRACE=%s: FILE:FIRST:LAST", p);
        }
    }
    if (s_runtrace && (long)g_frame_count >= s_rt_first && (long)g_frame_count <= s_rt_last)
        fprintf(s_runtrace, "%llu %04X %d\n", (unsigned long long)g_nes_cycles, pc, cycles);
}

static void run_start(uint16_t pc, int cycles) {
    s_pc = pc;
    runtrace(pc, cycles);
    if (g_timing == TIMING_SCAFFOLD) {
        scaffold_charge(cycles);
        return;
    }
    if (!s_started) console_start();
    if (cycles != 1) console_check();   /* 1: a taken branch's last cycle, inside the instruction */
    console_add(cycles);
}

/* An indexed read crossed a page (port/src/game/hw.h): the console's 6502
 * spends a cycle more, inside the run that read (no landing place); the
 * scaffold's clock never charged it. */
void nes_page_cross(void) {
    if (g_timing == TIMING_SCAFFOLD) return;
    s_ops++;
    g_nes_cycles++;
}

/* The game calls this only for the 2 cycles the scaffold adds to a JMP it
 * made a tail call (mff.h mff_jmp_charge and the lifted JMP macros): the
 * console has no such cost, so its clock takes only the place. */
void nes_cpu_instruction_boundary(uint16_t pc, int cycles) {
    run_start(pc, g_timing == TIMING_SCAFFOLD ? cycles : cycles - 2);
}

void nes_instruction_boundary(uint16_t pc, int cycles) {
    if (pc >= 0x8000) pc = (uint16_t)(g_code_window_base | (pc & 0x1FFF));
    run_start(pc, cycles);
    if (pc == HD_OBJECTS_DRAW_PC) frame_objects_draw();        /* P9: render_hd.c's sprites (frame.c) */
}

void runtime_begin_post_nmi(void) {
    frame_nmi_done();                   /* the console's timing: the picture starts here */
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
uint32_t timing_held_cycles(void) { return s_held; }

/* 513 cycles, 514 when the write lands on an odd cycle. The $4014 write is
 * the last cycle of its run, so on the console's clock it came at cycle
 * g_nes_cycles - 1 (the oracle: written on an even cycle, the next
 * instruction 513 cycles after the write's); the scaffold's clock took the
 * parity of the cycle after it, and its baselines keep that. */
void timing_add_dma_stall(void) {
    if (g_timing == TIMING_SCAFFOLD) s_dma_stall += 513 + (int)(g_nes_cycles & 1);
    else s_dma_stall += 513 + (int)((g_nes_cycles - 1) & 1);
}

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
    if (g_timing != TIMING_SCAFFOLD) maybe_trigger_vblank(7);  /* the interrupt sequence */
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

/* save states (savestate.c) */
void timing_state(StateIO *io) {
    st_u32(io, &s_ops); st_u32(io, &s_length);
    st_int(io, &s_started); st_int(io, &s_odd); st_int(io, &s_dots);
    st_int(io, &s_depth); st_int(io, &s_saved_depth); st_int(io, &s_pending);
    st_int(io, &s_dma_stall);
    st_u16(io, &s_pc);
    st_u64(io, &s_epoch);
    st_int(io, &s_in_irq);
    st_u64(io, &g_nes_cycles);
    st_u64(io, &g_frame_count);
    /* P7: the console's clock */
    {
        int t = g_timing;
        st_int(io, &t);
        if (st_loading(io) && (t == TIMING_SCAFFOLD) != (g_timing == TIMING_SCAFFOLD))
            mff_fatal("the save state was taken with --timing %s", t == TIMING_SCAFFOLD ? "scaffold" : "console");
    }
    st_int(io, &s_frame_dots);
    st_u32(io, &s_vbl_end); st_u32(io, &s_prerender);
    st_int(io, &s_prerender_due);
    st_int(io, &s_nmi_due);
    st_u32(io, &s_held);
    st_u64(io, &s_vblanks);
    st_u32(io, &s_access_dots);
    st_int(io, &s_branch);
    st_u8(io, &s_task0_state);
    st_u64(io, &s_task0_resumed);
}

/* A loaded state goes on from where it was taken, frame_run's return at the
 * scheduler's $FEBD: frame_boundary's depth back down, and the 2 cycles of
 * $FEBD's charge, which savestate.c makes again by entering func_FEBD. */
void timing_resume_at_scheduler(void) {
    s_depth--;
    if (g_timing == TIMING_SCAFFOLD) {  /* the console's clock charges a run after the NMI */
        s_ops -= 2;
        g_nes_cycles -= 2;
    }
}
