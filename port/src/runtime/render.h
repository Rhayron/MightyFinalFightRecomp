/*
 * render.h - one frame's picture as data: what the PPU showed, region by
 * region, and the composer that turns it into 256x240 pixels
 *
 * The runtime fills a FrameRecord at the frame boundary (frame.c): the
 * state at the start of the picture, then one region per raster split. The
 * MMC3 counts scanlines while the background is drawn; when it fires, the
 * game's IRQ handler changes the scroll or the CHR banks and the lines
 * after it belong to a new region. In Mighty Final Fight that is the status
 * bar: the top rows with scroll 0, the playfield with the camera's scroll
 * (two regions in round 1 areas 1 and 2, where the sky stays still), and
 * the bottom rows with scroll 0 and their own CHR banks.
 */
#ifndef MFF_RENDER_H
#define MFF_RENDER_H

#include <stdint.h>

#define FRAME_W 256
#define FRAME_H 240
#define MAX_REGIONS 242                 /* the start, the pre-render line, 240 lines */
#define MAX_NT_COPIES 8

typedef struct {
    int first_line;                     /* the line after the scanline the IRQ came on */
    uint8_t ctrl, mask;                 /* PPUCTRL, PPUMASK */
    uint8_t sx, sy;                     /* the scroll as the scaffold renderer keeps it */
    uint8_t hud_ctrl, hud_x, hud_y;     /* above a sprite-0 split (scaffold renderer) */
    uint16_t t;                         /* the PPU's t register */
    uint8_t fine_x;
    int v_set;                          /* a $2006 pair set v (the VRAM address) here */
    uint16_t v;
    uint8_t pal[32];                    /* palette RAM */
    uint16_t chr[8];                    /* 1 KiB CHR bank behind each $400 of pattern space */
    const uint8_t *nt;                  /* the 4 KiB of nametable RAM */
} PpuRegion;

typedef struct {
    /* the start of the picture */
    uint8_t mask0;                      /* PPUMASK: what is drawn at all */
    uint8_t backdrop;                   /* palette entry 0 */
    uint8_t oam[256];
    uint16_t chr0[8];
    int mirroring;                      /* MIRROR_* (machine.h) */
    int split_y;                        /* sprite-0 split: HUD scroll above it, 240 none */
    /* the background, line by line: region k covers its first_line up to
     * the next one's */
    int nregions;
    PpuRegion region[MAX_REGIONS];
    /* raster IRQs: the last scanline one ran after */
    int irq_fired, last_irq;
    /* after the last IRQ (the scaffold renderer's sprite pass reads these
     * below last_irq) */
    uint8_t ctrl_end, mask_end, pal_end[32];
    uint16_t chr_end[8];
    /* nametable copies the regions point at */
    int ncopies;
    uint8_t nt_copy[MAX_NT_COPIES][0x1000];
} FrameRecord;

enum { PICTURE_SCAFFOLD = 0, PICTURE_CONSOLE = 1 };

/* Draw `f` as 256x240 palette indices (0-63). Returns 1 if sprite 0 hit
 * the background.
 *  PICTURE_SCAFFOLD: as the scaffold's renderer drew it (port/tests compare
 *    against that): every sprite shown, and a split restarts the vertical
 *    scroll from the Y last written to $2005;
 *  PICTURE_CONSOLE: as the PPU draws it: 8 sprites per line, the vertical
 *    position counts on through a split, and the scroll and banks an IRQ
 *    handler sets take effect on the line after the one it runs on. */
int render_frame(const FrameRecord *f, uint8_t *pix, int mode);

/* palette indices to 0xAARRGGBB, with the scaffold renderer's colours */
void picture_rgb(const uint8_t *pix, uint32_t *rgb);

/* the 64 NES colours as the scaffold's renderer shows them */
extern const uint32_t g_nes_rgb[64];

#endif
