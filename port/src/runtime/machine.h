/*
 * machine.h - what the parts of the port's runtime share
 *
 *   cart.c        PRG and CHR data, from build/assets (never the ROM)
 *   bus.c         CPU registers and RAM, the address decoder, the pads
 *   ppu_regs.c    PPU registers, nametables, palette, OAM
 *   mmc3.c        bank registers and the scanline IRQ counter
 *   apu.c         APU writes to the 2A03 synthesizer (port/src/audio), the sound out
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
uint8_t mmc3_prg_peek(uint16_t addr);      /* no side effects (DMC fetches) */
int mmc3_clock_scanline(void);      /* 1 when the counter fires an IRQ */
int mmc3_mirroring(void);
void mmc3_chr_map(uint16_t banks[8]);   /* 1 KiB CHR bank behind each $400 of pattern space */
uint8_t mmc3_chr_read(uint16_t a);      /* a pattern byte, $0000-$1FFF */
void mmc3_write_prg_reads(void);        /* MFF_PRG_READS, at exit */

/* ---- apu.c ---- */
void apu_power_on(void);
void apu_write(uint16_t addr, uint8_t v);
uint8_t apu_read_status(void);
/* a frame boundary (timing.c, NMI on or off): the sound up to it to the
 * window and MFF_WAV; next_len is the new frame's length in cycles */
void apu_frame_boundary(uint32_t next_len);

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

/* ---- dispatch_rt.c ---- */
/* the scheduler restarted (coroutine.c): forget the abandoned tasks' JMP tails */
void dispatch_reset_tails(void);

/* ---- video_sdl.c (with MFF_SDL) ---- */
int video_open(int scale);
void video_frame(const uint32_t *fb);       /* present and pace */
void video_poll_input(void);                /* pad 1 from keyboard/gamepad */
void video_audio(const int16_t *pcm, int n);    /* 48 kHz mono samples to play */
int video_available(void);                  /* 1 if this build has the window */
/* no game data yet: a window asking for the dump (`why`: the last attempt's
 * error, or NULL); 0 with the path of a file dropped on it, -1 if closed */
int video_ask_for_dump(char *path, size_t cap, const char *why);

/* ---- extract.c, setup.c (P8: the first run, without Python) ---- */
uint32_t mff_crc32(const uint8_t *p, size_t n);
int mff_is_dump(const char *path);          /* Mighty Final Fight (USA), CRC32 15EB0BEE */
int mff_make_dirs(const char *path);
/* extract_assets.py --only runtime: chr.bin, prg.bin, manifest.json in
 * out_dir; 0, or -1 with the reason in err */
int extract_runtime_assets(const char *rom, const char *out_dir, int check_crc, char *err, size_t errcap);
void setup_init(const char *argv0);
const char *setup_exe_dir(void);
const char *setup_data_dir(void);           /* assets/, saves/, the config: writable */
const char *setup_find_assets(const char *given);   /* loaded (cart.c): the dir, or NULL */
const char *setup_first_run(const char *dump, int window);
int setup_extract_cli(const char *dump, const char *out);

/* ---- savestate.c (P8): save states ----
 * Every part of the runtime with state writes and reads it in its
 * *_state(StateIO *) function, field by field; a new static there must be
 * added to it (port/tests/savestate_check.sh finds one that is not). */
typedef struct StateIO StateIO;
int st_loading(const StateIO *io);
void st_bytes(StateIO *io, void *p, size_t n);
void st_u8(StateIO *io, uint8_t *v);
void st_u16(StateIO *io, uint16_t *v);
void st_u32(StateIO *io, uint32_t *v);
void st_u64(StateIO *io, uint64_t *v);
void st_int(StateIO *io, int *v);           /* as 32 bits */
void st_f64(StateIO *io, double *v);
void st_u16s(StateIO *io, uint16_t *v, int n);
void st_ints(StateIO *io, int *v, int n);
void bus_state(StateIO *io);
void ppu_state(StateIO *io);
void mmc3_state(StateIO *io);
void apu_state(StateIO *io);
void timing_state(StateIO *io);
void frame_state(StateIO *io);
void dispatch_state(StateIO *io);
void coroutine_state(StateIO *io);
void hooks_state(StateIO *io);
void savestate_init(void);
void savestate_frame_begin(void);           /* frame.c, at the frame's start and end */
void savestate_frame_end(void);
void savestate_request_save(void);          /* the window's keys: the current slot */
void savestate_request_load(void);
void savestate_next_slot(void);
const char *savestate_status(void);         /* "slot 2 saved" and such, for the title */
/* main: the game from power-on, or from the state in load_path; never returns */
void savestate_run(const char *load_path);
/* what loading needs from the other parts */
void timing_resume_at_scheduler(void);
int dispatch_tail_count(void);
uint16_t dispatch_tail_addr(int i);
uint8_t dispatch_tail_s(int i);
int dispatch_lap_pending(void);
void dispatch_rebuild_tail(int inner, void (*resume)(void));
int coroutine_on_scheduler(void);
int coroutine_task_mask(void);              /* bit per task with a C stack */
int coroutine_saved_task_mask(void);        /* the same, as the loaded state had it */
void coroutine_drop_all(void);
void coroutine_rebuild(int ch, void (*body)(void));
void video_after_load(void);

/* ---- config.c (P8): MightyFinalFight.ini ---- */
void config_load(const char *path, int write_default);
const char *config_get(const char *key);    /* "section.key", NULL if not set */
int config_int(const char *key, int def);
const char *config_path(void);

/* ---- errors ---- */
void mff_fatal(const char *fmt, ...);

#endif
