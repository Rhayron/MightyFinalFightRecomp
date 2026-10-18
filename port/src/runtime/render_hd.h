/*
 * render_hd.h - stage 2's picture (PLAN_DECOMP.md 2.6, P9): drawn from the
 * game's state instead of the PPU's memories, with the options of the
 * high-level renderer
 *
 * render_hd.c draws the playfield from the round's stage map (docs/
 * assets.md, "Round backgrounds") and the sprites from the object slots
 * and their metasprites (docs/objects.md, "Drawing"), through a model of
 * the game's own column writer and objects_draw. Where the state does not
 * describe the picture (title, story, select, map, continue, ending, the
 * status rows) it takes the PPU's nametables and OAM, as stage 1 does.
 *
 * With every option off the picture is stage 1's console picture
 * (render_frame.c), pixel for pixel: MFF_HDCHECK compares the two on every
 * frame (port/tests/hd_check.py). The options:
 *   no_flicker  every sprite of every object, every frame, in a fixed
 *               order (effects, the player, slots 1-13): no 8-per-line
 *               limit and no 63-entry OAM, so the order the game rotates
 *               to spread the limit no longer shows;
 *   wide        16:9 (426 x 240 by default): the camera shows more of the
 *               stage on both sides, from the full stage map. Nothing in
 *               the game changes, so its spawn points stay at the original
 *               screen's edges: an object is drawn outside the original
 *               256 columns only once it has been wholly inside them
 *               (revealed), and until then only its part inside them, so
 *               nothing pops into view in the margins; objects the game
 *               treats as far ($68 px or more off its screen) are not drawn.
 *               The wide camera is the game's camera held inside the
 *               area (its first and last screens), the status rows stay in
 *               the middle, and screens that are not gameplay are shown in
 *               the middle with the backdrop around them.
 * The picture log (MFF_PICLOG) and the game never see any of this.
 */
#ifndef MFF_RENDER_HD_H
#define MFF_RENDER_HD_H

#include "render.h"
#include <stdint.h>

#define HD_MAX_W 512
#define HD_WIDE_W 426                   /* 240 * 16 / 9, rounded to even */

typedef struct {
    int no_flicker;                     /* every sprite, fixed order */
    int wide;                           /* 0, or the picture's width (HD_WIDE_W for --wide) */
} HdOptions;

extern HdOptions g_hd;

/* An option by name, for the command line and the config file:
 * "no_flicker" (0/1), "wide" (0/1, or a width 256-512), "picture"
 * (console, scaffold, hd). Returns 0, or -1 for an unknown key or value. */
int hd_option(const char *key, const char *value);

/* the shown picture's width (256 unless wide) */
int hd_width(void);

void hd_init(void);                     /* main.c: MFF_HDCHECK, MFF_WIDE, MFF_NO_FLICKER */
/* frame.c, before and after the NMI: the VRAM requests it uploads */
void hd_frame_start(void);
void hd_after_nmi(void);
/* timing.c: objects_draw ($E9BC) is about to run; its input is saved */
void hd_objects_draw_start(void);
#define HD_OBJECTS_DRAW_PC 0xE9BC

/* frame.c, after the frame's record: draw the HD picture (when it is shown,
 * `shown`) and, with MFF_HDCHECK, compare it (options off) with the console
 * picture */
void hd_picture(const FrameRecord *f, int shown);
int hd_active(void);                    /* the HD picture is drawn (PICTURE_HD or MFF_HDCHECK) */
/* the last HD picture: ARGB and palette indices, its width, and where the
 * original 256 columns are in it */
void hd_shown(const uint32_t **rgb, const uint8_t **pix, int *w, int *view_x);
void hd_exit(void);                     /* MFF_HDCHECK's summary */

#endif
