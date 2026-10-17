/*
 * machine.h - what the parts of the port's runtime share
 *
 *   cart.c        PRG and CHR data, from build/assets (never the ROM)
 *   bus.c         CPU registers and RAM, the address decoder, the pads
 *   ppu_regs.c    PPU registers, nametables, palette, OAM
 *   mmc3.c        bank registers and the scanline IRQ counter
 *   apu.c         APU registers (kept, not played: P6 adds the synthesizer)
 *   timing.c      the cycle clock, the frame boundary, NMI and IRQ entry
 *   frame.c       one frame: hooks, NMI, the scanline pass, the picture
 *   render_frame.c  the picture from a FrameRecord
 *   dispatch_rt.c dispatch by address, JMP-loop flattening, misses
 *   coroutine.c   the tasks' C stacks
 *   testhooks.c   frame log, picture log, keyed input (port/tests)
 *   mff_hooks.c   game rules that live outside port/src/game for now
 *   main.c        command line
 */
#ifndef MFF_MACHINE_H
#define MFF_MACHINE_H

#include "nes_runtime.h"
#include "render.h"
#include <stddef.h>
#include <stdint.h>

/* ---- cart.c ---- */
#define PRG_SIZE 0x20000            /* 16 banks of 8 KiB */
#define CHR_SIZE 0x20000            /* 128 banks of 1 KiB */

typedef struct {
    uint8_t prg[PRG_SIZE];
    uint8_t prg_have[PRG_SIZE / 8]; /* bit per PRG byte: present in the data */
    uint8_t chr[CHR_SIZE];
    int vertical_mirroring;         /* iNES header flags 6 bit 0 */
    char source[512];               /* where it came from, for messages */
} Cart;

extern Cart g_cart;

/* chr.bin and prg.bin from the extracted data (build/assets): 0 on
 * success, -1 if they are not there */
int cart_load_assets(const char *dir);
/* a PRG byte the data does not hold was read: stop and say where */
void cart_missing_prg(int bank8k, uint16_t addr);

static inline int cart_has_prg(uint32_t off) {
    return (g_cart.prg_have[off >> 3] >> (off & 7)) & 1;
}

/* ---- bus.c ---- */
extern uint8_t g_sram[0x2000];      /* $6000-$7FFF */
void bus_power_on(void);
/* a pad read: bit 0 of the shift register; strobe high reads A */
uint8_t pad_read(int port);
void pad_strobe(uint8_t v);

/* ---- mmc3.c ---- */
enum { MIRROR_ONE_LOW = 0, MIRROR_ONE_HIGH = 1, MIRROR_VERTICAL = 2, MIRROR_HORIZONTAL = 3 };
void mmc3_power_on(int vertical);
void mmc3_write(uint16_t addr, uint8_t v);
uint8_t mmc3_prg_read(uint16_t addr);
int mmc3_clock_scanline(void);      /* 1 when the counter fires an IRQ */
int mmc3_mirroring(void);
void mmc3_chr_map(uint16_t banks[8]);   /* 1 KiB CHR bank behind each $400 of pattern space */
uint8_t mmc3_chr_read(uint16_t a);      /* a pattern byte, $0000-$1FFF */
void mmc3_write_prg_reads(void);        /* MFF_PRG_READS, at exit */

/* ---- apu.c ---- */
void apu_power_on(void);
void apu_write(uint16_t addr, uint8_t v);
uint8_t apu_read_status(void);

/* ---- ppu_regs.c ---- */
void ppu_power_on(void);
uint8_t ppu_read(uint16_t reg);
void ppu_write(uint16_t reg, uint8_t v);
void ppu_oam_write(int index, uint8_t v);
uint8_t ppu_ctrl(void);
uint8_t ppu_mask(void);
void ppu_set_status(uint8_t set, uint8_t clear);
const uint8_t *ppu_oam(void);       /* 256 bytes */
const uint8_t *ppu_palette(void);   /* 32 bytes as the PPU stores them */
/* frame boundary: vblank flag on, sprite-0 hit off, scroll and splits reset */
void ppu_vblank_begin(void);
/* the frame callback's start: hit and overflow off, vblank on */
void ppu_frame_status(void);
/* the picture (frame.c): its start, the regions the raster IRQs open */
void ppu_render_sync(void);
int ppu_split_active(void);
uint32_t ppu_v_writes(void);
void ppu_sync_from_v(void);
void ppu_capture_start(FrameRecord *f);
void ppu_capture_region(FrameRecord *f, int first_line, int v_set);
void ppu_capture_end(FrameRecord *f);

/* ---- timing.c ---- */
/* Interrupt-depth values: 0 main code, 1 inside the frame boundary (the
 * callback, the scanline pass), 2 inside the NMI handler, POST_NMI_DEPTH
 * while the hijacked return runs (no frame boundary may start there). */
#define POST_NMI_DEPTH 8
int timing_depth(void);
void timing_set_depth(int d);
void timing_reset_depth(void);      /* a scheduler restart (coroutine.c) */
uint64_t timing_interrupt_epoch(void);
void timing_note_interrupt(void);
void timing_add_dma_stall(void);    /* OAM DMA: 513 or 514 cycles */
uint32_t timing_frame_cycles(void); /* cycles into the current frame */
uint16_t timing_pc(void);           /* where the last run of instructions began */
int timing_in_irq(void);
/* run the game's IRQ handler as an interrupt of the code that was running */
void timing_call_irq_handler(void);

/* ---- frame.c ---- */
void frame_run(void);               /* the frame boundary with NMI enabled */
extern long g_smoke_frames;         /* --smoke N: exit after N frames (0: run on) */
extern int g_window;                /* a window is open (video_sdl.c) */
extern int g_picture;               /* PICTURE_CONSOLE or PICTURE_SCAFFOLD (render.h): what is shown */

/* ---- png_write.c ---- */
int png_write_argb(const char *path, const uint32_t *px, int w, int h);

/* ---- testhooks.c ---- */
void hooks_init(void);
void hooks_frame_start(uint64_t frame);     /* before the NMI */
/* after the picture: the scaffold renderer's (the picture log), the one
 * shown, and the shown one as palette indices */
void hooks_picture(const uint32_t *test, const uint32_t *shown, const uint8_t *shown_pix);
int hooks_want_picture(void);                /* screenshots or a picture dump asked for */
void hooks_exit_log(void);

/* ---- mff_hooks.c ---- */
void mff_hooks_init(void);

/* ---- coroutine.c ---- */
void coroutine_power_on(void);

/* ---- video_sdl.c (with MFF_SDL) ---- */
int video_open(int scale);
void video_frame(const uint32_t *fb);       /* present and pace */
void video_poll_input(void);                /* pad 1 from keyboard/gamepad */

/* ---- errors ---- */
void mff_fatal(const char *fmt, ...);

#endif
