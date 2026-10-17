/*
 * frame.c - one frame at the frame boundary, with the NMI enabled
 *
 * timing.c calls frame_run between two runs of instructions when a frame's
 * cycles are up. In order:
 *   1. the pads for this frame (window, keyed route, bot) and the frame log
 *      (testhooks.c), before the game sees anything new;
 *   2. the NMI: its interrupt frame goes on the stack of the code it
 *      interrupts and the handler runs (func_NMI in dispatch.c, which also
 *      runs the hijacked return, nmi_tail, after the handler's RTI);
 *   3. the picture: if rendering is on, the scanline pass clocks the MMC3
 *      once per line (the pre-render line and lines 0-239) while the
 *      background is on, running the game's IRQ handler whenever the
 *      counter fires; each handler starts a new region of the picture with
 *      the scroll and banks it left (render.h). render_frame.c then draws
 *      the record; a sprite-0 hit shows in $2002 afterwards;
 *   4. the picture log, the window, and the frame count.
 * With rendering off the picture is not redrawn (the screen keeps the last
 * one) and the MMC3 is not clocked.
 *
 * This is when the scaffold's runtime ran these things too, so the game's
 * raster IRQs run at the same point of its code. The NMI's interrupt
 * sequence counts here as on the console (7 cycles), which the scaffold's
 * runner leaves out; nmi_tail's own 52 + 26 (mff_hooks.c) both charge, the
 * scaffold's extras.c too since 2026-10-17. port/tests/selfcheck.py's
 * --nmi-cycles (MFF_NMI_CYCLES) accounts for the 7.
 */
#include "machine.h"
#include "render.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long g_smoke_frames;
int g_window;
int g_picture = PICTURE_CONSOLE;

/* Two pictures of each frame: the scaffold renderer's, which the picture
 * log hashes and the sprite-0 hit comes from (so the game sees the same
 * thing whatever is shown), and the one shown (g_picture: the window,
 * screenshots). Both keep the last picture while rendering is off, and
 * start out all zero, as the scaffold's framebuffer did. */
static uint8_t s_pix_test[FRAME_W * FRAME_H], s_pix_show[FRAME_W * FRAME_H];
static uint32_t s_rgb_test[FRAME_W * FRAME_H], s_rgb_show[FRAME_W * FRAME_H];
static FrameRecord s_rec;
static int s_split_last = 240, s_split_hold;    /* the sprite-0 split's line, held 6 frames */

static void deliver_nmi(void) {
    CPU6502State saved = g_cpu;
    uint8_t p = (uint8_t)((g_cpu.N << 7) | (g_cpu.V << 6) | 0x20 | (g_cpu.D << 3) |
                          (g_cpu.I << 2) | (g_cpu.Z << 1) | g_cpu.C);
    g_ram[0x100 + g_cpu.S] = 0x00; g_cpu.S--;   /* the return address: the runtime */
    g_ram[0x100 + g_cpu.S] = 0x00; g_cpu.S--;   /* has no PC to put here */
    g_ram[0x100 + g_cpu.S] = p;    g_cpu.S--;
    g_cpu.I = 1;
    timing_note_interrupt();
    timing_set_depth(timing_depth() + 1);
    maybe_trigger_vblank(7);                    /* the interrupt sequence's 7 cycles */
    func_NMI();
    if (timing_depth() > 0) timing_set_depth(timing_depth() - 1);
    if (g_rti_target == 0) {
        /* the handler left without its RTI: back to where it interrupted.
         * (After a hijacked return the CPU is where nmi_tail left it.) */
        g_cpu.S = saved.S;
        g_cpu.A = saved.A; g_cpu.X = saved.X; g_cpu.Y = saved.Y;
        g_cpu.N = saved.N; g_cpu.V = saved.V; g_cpu.D = saved.D;
        g_cpu.I = saved.I; g_cpu.Z = saved.Z; g_cpu.C = saved.C;
    }
}

/* where a sprite-0 split would put the bottom of the status bar: under
 * sprite 0, on a tile row, kept for 6 frames after sprite 0 goes away */
static int split_line(void) {
    if (ppu_split_active()) {
        int h = (ppu_ctrl() & 0x20) ? 16 : 8;
        int y = (ppu_oam()[0] + h + 7) & ~7;
        if (y > 240) y = 240;
        s_split_last = y;
        s_split_hold = 6;
        return y;
    }
    if (s_split_hold > 0) {
        s_split_hold--;
        return s_split_last;
    }
    return 240;
}

static void scanline(int line) {
    uint32_t vw;
    if (!mmc3_clock_scanline()) return;
    vw = ppu_v_writes();
    timing_call_irq_handler();
    if (ppu_v_writes() != vw) ppu_sync_from_v();
    s_rec.irq_fired = 1;
    s_rec.last_irq = line;
    ppu_capture_region(&s_rec, line + 1, ppu_v_writes() != vw);
}

static void picture(void) {
    FrameRecord *f = &s_rec;
    uint8_t mask = ppu_mask();
    if (!(mask & 0x18)) return;
    ppu_capture_start(f);
    f->nregions = 0;
    f->irq_fired = 0;
    f->last_irq = -2;
    f->split_y = 240;
    f->mirroring = mmc3_mirroring();
    if (mask & 0x08) {
        ppu_render_sync();
        f->mirroring = mmc3_mirroring();
        f->split_y = split_line();
        ppu_capture_region(f, 0, 0);
        scanline(-1);
        for (int line = 0; line < FRAME_H; line++) scanline(line);
    } else {
        ppu_capture_region(f, 0, 0);            /* the sprites' state; no scanline pass */
    }
    ppu_capture_end(f);
    if (render_frame(f, s_pix_test, PICTURE_SCAFFOLD)) ppu_set_status(0x40, 0);
    picture_rgb(s_pix_test, s_rgb_test);
    if (g_picture != PICTURE_SCAFFOLD && (g_window || hooks_want_picture())) {
        render_frame(f, s_pix_show, g_picture);
        picture_rgb(s_pix_show, s_rgb_show);
    } else {
        memcpy(s_pix_show, s_pix_test, sizeof s_pix_show);
        memcpy(s_rgb_show, s_rgb_test, sizeof s_rgb_show);
    }
}

static void smoke_exit(void) {
    printf("{\n  \"frames_run\": %llu,\n  \"dispatch_miss_count\": 0\n}\n",
           (unsigned long long)g_frame_count);
    fflush(stdout);
    hooks_exit_log();
    exit(0);
}

void frame_run(void) {
    ppu_frame_status();
    if (g_window) video_poll_input();
    g_controller2_buttons = 0;
    hooks_frame_start(g_frame_count);
    deliver_nmi();
    picture();
    hooks_picture(s_rgb_test, s_rgb_show, s_pix_show);
    if (g_window) video_frame(s_rgb_show);
    g_frame_count++;
    if (g_smoke_frames > 0 && (long)g_frame_count >= g_smoke_frames) smoke_exit();
}
