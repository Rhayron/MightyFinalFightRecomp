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
 * sequence counts here as on the console (7 cycles), and nmi_tail's own
 * 52 + 26 (mff_hooks.c), as the scaffold's extras.c charges them too; with
 * the scaffold's timing (timing.c, --timing scaffold) the 7 are left out,
 * as the scaffold's runner leaves them out, and the routes match its
 * baselines cycle for cycle.
 */
#include "machine.h"
#include "render.h"
#include "render_hd.h"
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
    /* the return address: the run the NMI lands before (the scaffold's
     * runtime had no PC to put here and pushed 0; its baselines keep that) */
    uint16_t ret = g_timing == TIMING_SCAFFOLD ? 0 : timing_pc();
    g_ram[0x100 + g_cpu.S] = (uint8_t)(ret >> 8); g_cpu.S--;
    g_ram[0x100 + g_cpu.S] = (uint8_t)ret;        g_cpu.S--;
    g_ram[0x100 + g_cpu.S] = p;    g_cpu.S--;
    g_cpu.I = 1;
    timing_note_interrupt();
    timing_set_depth(timing_depth() + 1);
    if (g_timing != TIMING_SCAFFOLD)            /* the interrupt sequence's 7 (the scaffold leaves them out) */
        maybe_trigger_vblank(7);
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
    if (hd_active()) hd_picture(f, g_window || hooks_want_picture());     /* P9: render_hd.c */
    if (g_picture == PICTURE_CONSOLE && (g_window || hooks_want_picture())) {
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

static void frame_end(int save) {
    const uint32_t *rgb = s_rgb_show;
    const uint8_t *pix = s_pix_show;
    int w = FRAME_W, view_x = 0;
    if (g_picture == PICTURE_HD) hd_shown(&rgb, &pix, &w, &view_x);
    hooks_picture(s_rgb_test, rgb, pix, w, view_x);
    if (g_window) video_frame(rgb, w);
    g_frame_count++;
    if (save) savestate_frame_end();    /* F5/F7 and MFF_SAVE_AT (savestate.c) */
    if (g_smoke_frames > 0 && (long)g_frame_count >= g_smoke_frames) smoke_exit();
}

/* P9's picture (render_hd.c) reads the game's RAM as it is when the
 * picture starts, and objects_draw's input of the frame before. With the
 * console's timing the picture is drawn at the next vblank, after the
 * frame's own work: it gets the RAM of its start (s_open_ram), and
 * objects_draw's input reaches render_hd only after it (s_draw_snap). */
static uint8_t s_open_ram[0x800], s_draw_snap[0x800];
static int s_draw_pending;

void frame_objects_draw(void) {         /* timing.c: objects_draw ($E9BC) starts */
    if (!hd_active()) return;
    if (g_timing == TIMING_SCAFFOLD) {
        hd_objects_draw_start();
        return;
    }
    memcpy(s_draw_snap, g_ram, sizeof s_draw_snap);
    s_draw_pending = 1;
}

/* run an hd_* call with `ram` in place of the game's RAM */
static void with_ram(const uint8_t *ram, void (*fn)(const FrameRecord *, int), const FrameRecord *f, int shown) {
    static uint8_t live[0x800];
    memcpy(live, g_ram, sizeof live);
    memcpy(g_ram, ram, sizeof live);
    fn(f, shown);
    memcpy(g_ram, live, sizeof live);
}

static void hd_draw_start(const FrameRecord *f, int shown) {
    (void)f; (void)shown;
    hd_objects_draw_start();
}

/* ---- the console's timing: the scanline pass runs with the frame ----
 *
 * The MMC3 counts a scanline at dot 260 of the pre-render line and of
 * lines 0-239 while rendering is on, and its IRQ is taken like the NMI, at
 * the first run boundary after it (I clear, outside the NMI handler: in
 * nmi_tail's sound update too), on whatever code runs then. timing.c calls
 * frame_scanlines as its clock moves (run starts, hardware accesses) and
 * frame_vblank at each vblank. The picture opens after the NMI, takes a
 * region at each IRQ (and where the frame's code turns rendering on or off)
 * and is drawn at the next vblank, when the frame is over. So a frame whose
 * code turns rendering off before the status bar's line has no IRQ, as on
 * the console. (The scaffold's timing runs the whole pass at the NMI.) */
static int s_open;                      /* this frame's picture is being drawn */
static int s_closing;                   /* a frame was delivered: it ends at the next vblank */
static int s_line = FRAME_H;            /* the next line the MMC3 counts (-1 the pre-render line) */
static int s_irq;                       /* the MMC3 raised the IRQ line, not taken yet */
static int s_irq_line;                  /* on this line's count */
static int s_irq_blocked;               /* the I flag or the NMI handler kept it out */
static int32_t s_irq_dot;

#define LINE_CLOCK_DOT(l) (((l) + 21) * 341 + 259)  /* dot 260 of line l, from the vblank's dot */

static int line_of(int32_t dot) {       /* the line a dot past the vblank is on (-1 above line 0) */
    return dot < 21 * 341 ? -1 : (int)((dot - 21 * 341) / 341);
}

static void count_lines(int32_t dot, int flush) {
    while (s_line < FRAME_H && LINE_CLOCK_DOT(s_line) <= dot) {
        /* A run of several instructions can end past the next line: the
         * console would have taken the IRQ and its handler would have set
         * the latch before that line counts (the handlers write it within
         * the line), so the next lines wait for the handler. */
        if (!flush && ((s_irq && !s_irq_blocked) || timing_in_irq())) return;
        if ((ppu_mask() & 0x18) && mmc3_clock_scanline() && !s_irq) {
            s_irq = 1;
            s_irq_line = s_line;
            s_irq_blocked = 0;
            s_irq_dot = LINE_CLOCK_DOT(s_line);
        }
        s_line++;
    }
}

void frame_scanlines(int32_t dot) { count_lines(dot, 0); }

int frame_irq_due(int32_t dot, int32_t need) { return s_irq && dot >= s_irq_dot + need; }

void frame_irq_blocked(void) { s_irq_blocked = 1; }

void frame_irq(int32_t dot) {
    uint32_t vw = ppu_v_writes();
    /* the region starts after the line the handler runs on: the line that
     * raised the IRQ, unless the IRQ waited for the I flag */
    int line = s_irq_blocked ? line_of(dot) : s_irq_line;
    s_irq = 0;
    timing_call_irq_handler();
    count_lines(dot, 0);                /* the lines that waited for the handler */
    if (!s_open) return;
    if (ppu_v_writes() != vw) ppu_sync_from_v();
    s_rec.irq_fired = 1;
    s_rec.last_irq = line;
    if (line + 1 < FRAME_H) ppu_capture_region(&s_rec, line + 1, ppu_v_writes() != vw);
    ppu_capture_end(&s_rec);
}

/* the frame's code wrote PPUMASK while the picture is drawn */
void frame_mask_write(int32_t dot) {
    int line = line_of(dot);
    if (!s_open || line < 0 || line + 1 >= FRAME_H) return;
    ppu_capture_region(&s_rec, line + 1, 0);
    ppu_capture_end(&s_rec);
}

static void picture_open(void) {
    FrameRecord *f = &s_rec;
    uint8_t mask = ppu_mask();
    s_open = 0;
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
    }
    ppu_capture_region(f, 0, 0);
    ppu_capture_end(f);
    if (hd_active()) memcpy(s_open_ram, g_ram, sizeof s_open_ram);
    s_open = 1;
}

static void picture_close(void) {
    FrameRecord *f = &s_rec;
    if (s_open) {
        if (render_frame(f, s_pix_test, PICTURE_SCAFFOLD)) ppu_set_status(0x40, 0);
        picture_rgb(s_pix_test, s_rgb_test);
        if (hd_active()) with_ram(s_open_ram, hd_picture, f, g_window || hooks_want_picture());
        if (g_picture == PICTURE_CONSOLE && (g_window || hooks_want_picture())) {
            render_frame(f, s_pix_show, g_picture);
            picture_rgb(s_pix_show, s_rgb_show);
        } else {
            memcpy(s_pix_show, s_pix_test, sizeof s_pix_show);
            memcpy(s_rgb_show, s_rgb_test, sizeof s_rgb_show);
        }
    }
    s_open = 0;
    if (s_draw_pending) {               /* this frame's objects_draw, for the next picture */
        s_draw_pending = 0;
        with_ram(s_draw_snap, hd_draw_start, f, 0);
    }
}

void frame_vblank(void) {
    count_lines(INT32_MAX, 1);          /* the lines left: every one is before the vblank */
    s_line = -1;
    if (s_closing) {
        s_closing = 0;
        picture_close();
        frame_end(0);
    }
}

/* the NMI handler's RTI (timing.c, before the hijacked return): the
 * picture starts from the state the handler set; the raster IRQ can come
 * as early as nmi_tail's sound update */
void frame_nmi_done(void) {
    if (g_timing == TIMING_SCAFFOLD || s_closing) return;
    picture_open();
    s_closing = 1;
}

void frame_run(void) {
    savestate_frame_begin();
    ppu_frame_status();
    if (g_window) video_poll_input();
    g_controller2_buttons = 0;
    hooks_frame_start(g_frame_count);
    hd_frame_start();
    deliver_nmi();
    hd_after_nmi();
    if (g_timing != TIMING_SCAFFOLD) {
        frame_nmi_done();               /* a handler that did not hijack its return */
        /* a state is taken where the NMI landed at the scheduler's $FEBD
         * (savestate.c): the frame's picture is still open, its end at the
         * next vblank */
        savestate_frame_end();
        return;
    }
    picture();
    frame_end(1);
}

/* the frame's record, field by field (its regions point into its own
 * nametable copies: kept as the copy's index) */
static void record_state(StateIO *io, FrameRecord *f) {
    st_u8(io, &f->mask0); st_u8(io, &f->backdrop);
    st_bytes(io, f->oam, sizeof f->oam);
    st_u16s(io, f->chr0, 8);
    st_int(io, &f->mirroring); st_int(io, &f->split_y);
    st_int(io, &f->nregions);
    if (f->nregions < 0 || f->nregions > MAX_REGIONS) f->nregions = 0;
    for (int i = 0; i < f->nregions; i++) {
        PpuRegion *r = &f->region[i];
        int nt = r->nt ? (int)((r->nt - &f->nt_copy[0][0]) / 0x1000) : 0;
        st_int(io, &r->first_line);
        st_u8(io, &r->ctrl); st_u8(io, &r->mask); st_u8(io, &r->sx); st_u8(io, &r->sy);
        st_u8(io, &r->hud_ctrl); st_u8(io, &r->hud_x); st_u8(io, &r->hud_y);
        st_u16(io, &r->t); st_u8(io, &r->fine_x);
        st_int(io, &r->v_set); st_u16(io, &r->v);
        st_bytes(io, r->pal, sizeof r->pal);
        st_u16s(io, r->chr, 8);
        st_int(io, &nt);
        if (nt < 0 || nt >= MAX_NT_COPIES) nt = 0;
        r->nt = f->nt_copy[nt];
    }
    st_int(io, &f->irq_fired); st_int(io, &f->last_irq);
    st_u8(io, &f->ctrl_end); st_u8(io, &f->mask_end);
    st_bytes(io, f->pal_end, sizeof f->pal_end);
    st_u16s(io, f->chr_end, 8);
    st_int(io, &f->ncopies);
    if (f->ncopies < 0 || f->ncopies > MAX_NT_COPIES) f->ncopies = 0;
    for (int i = 0; i < f->ncopies; i++) st_bytes(io, f->nt_copy[i], sizeof f->nt_copy[i]);
}

/* save states (savestate.c): the pictures kept while rendering is off; the
 * colours follow from the palette indices */
void frame_state(StateIO *io) {
    st_bytes(io, s_pix_test, sizeof s_pix_test);
    st_bytes(io, s_pix_show, sizeof s_pix_show);
    st_int(io, &s_split_last);
    st_int(io, &s_split_hold);
    /* P7: the console's timing draws a picture across its frame */
    st_int(io, &s_open);
    st_int(io, &s_closing);
    st_int(io, &s_line);
    st_int(io, &s_irq);
    st_int(io, &s_irq_line);
    st_int(io, &s_irq_blocked);
    {
        uint32_t d = (uint32_t)s_irq_dot;
        st_u32(io, &d);
        s_irq_dot = (int32_t)d;
    }
    st_bytes(io, s_open_ram, sizeof s_open_ram);
    st_bytes(io, s_draw_snap, sizeof s_draw_snap);
    st_int(io, &s_draw_pending);
    record_state(io, &s_rec);
    if (st_loading(io)) {
        picture_rgb(s_pix_test, s_rgb_test);
        picture_rgb(s_pix_show, s_rgb_show);
    }
}
