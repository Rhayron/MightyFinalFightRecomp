/*
 * render_frame.c - the 256x240 picture of one frame, from a FrameRecord
 *
 * Background: line by line, each with its region's scroll, PPUCTRL,
 * palette, CHR banks and nametables. Scrolling follows the scaffold
 * renderer's model, which is what the tests compare against: the first
 * line starts at the region's Y scroll (plus 240 for the lower nametables)
 * and each line goes one further; when a split changes the Y scroll or the
 * nametable row the count restarts from the new value. A Y scroll of
 * 240-255 (coarse Y 30 or 31) wraps inside the same nametable as on the
 * PPU. X wraps across the two nametables of a row; mirroring picks the
 * physical ones. PPUMASK bit 1 clear shows the backdrop in the 8 leftmost
 * pixels.
 *
 * Sprites: drawn over it from OAM as it was when the picture started,
 * lowest OAM index in front. A sprite with the priority bit shows only
 * where the background is transparent, and it also hides the sprites with
 * higher indexes there (the PPU's priority quirk). Below the line after the
 * last raster IRQ, sprites take the CHR banks and PPUMASK as the IRQs left
 * them; above it, those of the start. PPUMASK bit 2 clear hides them in
 * the 8 leftmost pixels. Sprite 0 hits where one of its opaque pixels
 * covers an opaque background pixel (not at x = 255, not where either
 * layer is clipped).
 *
 * The console shows at most 8 sprites on a line, the first 8 in OAM order
 * (the game rotates the order so that the ones left out change: the
 * flicker). The scaffold's renderer shows all of them; sprite_limit picks.
 */
#include "render.h"
#include "machine.h"
#include <string.h>

/* the NES colours as 0xAARRGGBB, the scaffold renderer's table */
const uint32_t g_nes_rgb[64] = {
    0xFF545454, 0xFF001E74, 0xFF081090, 0xFF300088, 0xFF440064, 0xFF5C0030, 0xFF540400, 0xFF3C1800,
    0xFF202A00, 0xFF083A00, 0xFF004000, 0xFF003C00, 0xFF00323C, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFF989698, 0xFF084CC4, 0xFF3032EC, 0xFF5C1EE4, 0xFF8814B0, 0xFFA01464, 0xFF982220, 0xFF783C00,
    0xFF545A00, 0xFF287200, 0xFF087C00, 0xFF007628, 0xFF006678, 0xFF000000, 0xFF000000, 0xFF000000,
    0xFFECEEEC, 0xFF4C9AEC, 0xFF787CEC, 0xFFB062EC, 0xFFE454EC, 0xFFEC58B4, 0xFFEC6A64, 0xFFD48820,
    0xFFA0AA00, 0xFF74C400, 0xFF4CD020, 0xFF38CC6C, 0xFF38B4CC, 0xFF3C3C3C, 0xFF000000, 0xFF000000,
    0xFFECEEEC, 0xFFA8CCEC, 0xFFBCBCEC, 0xFFD4B2EC, 0xFFECAEEC, 0xFFECAED4, 0xFFECB4B0, 0xFFE4C490,
    0xFFCCD278, 0xFFB4DE78, 0xFFA8E290, 0xFF98E2B4, 0xFFA0D6E4, 0xFFA0A2A0, 0xFF000000, 0xFF000000,
};

static uint8_t s_opaque[FRAME_H][FRAME_W];      /* background pixel not transparent */
static uint32_t s_under[FRAME_H][FRAME_W];      /* background colour under each pixel */

static uint8_t chr_byte(const uint16_t map[8], int a) {
    a &= 0x1FFF;
    return g_cart.chr[(uint32_t)map[a >> 10] * 0x400 + (a & 0x3FF)];
}

static int nt_page(int mirroring, int virt) {
    switch (mirroring) {
    case MIRROR_ONE_LOW: return 0;
    case MIRROR_ONE_HIGH: return 1;
    case MIRROR_HORIZONTAL: return virt >> 1;
    default: return virt & 1;
    }
}

static void draw_background(const FrameRecord *f, uint32_t *fb, uint32_t back) {
    int r = 0, ready = 0, canon = 0, coarse = 0, fine = 0, canon_nt = 0;
    int last_hud = -1, last_sy = -1, last_nt = -1, y_abs = -1;
    for (int line = 0; line < FRAME_H; line++) {
        const PpuRegion *g;
        uint32_t colors[4][4];
        uint8_t ctrl;
        int hud, sx, sy, base, ox, nt_bit, row_nt, ty, trow;
        while (r + 1 < f->nregions && f->region[r + 1].first_line <= line) r++;
        g = &f->region[r];
        hud = f->split_y < FRAME_H && line < f->split_y;
        ctrl = hud ? g->hud_ctrl : g->ctrl;
        sx = hud ? g->hud_x : g->sx;
        sy = hud ? g->hud_y : g->sy;
        base = (ctrl & 0x10) ? 0x1000 : 0;
        ox = sx + ((ctrl & 1) ? 256 : 0);
        nt_bit = (ctrl & 2) ? 1 : 0;
        if (!ready || hud != last_hud || sy != last_sy || nt_bit != last_nt) {
            canon = sy >= 240;
            if (canon) {
                coarse = (sy >> 3) & 0x1F;
                fine = sy & 7;
                canon_nt = nt_bit;
            }
            ready = 1;
            last_hud = hud;
            if (y_abs < 0 || sy != last_sy || nt_bit != last_nt) y_abs = sy + (nt_bit ? 240 : 0);
            last_sy = sy;
            last_nt = nt_bit;
        }
        if (canon) {
            row_nt = canon_nt;
            ty = coarse;
            trow = fine;
        } else {
            int y = y_abs >= 480 ? y_abs - 480 : y_abs;
            trow = y & 7;
            row_nt = (y >> 3) >= 30;
            ty = (y >> 3) % 30;
        }
        for (int p = 0; p < 4; p++) {
            colors[p][0] = g_nes_rgb[g->pal[0] & 0x3F];
            for (int c = 1; c < 4; c++) colors[p][c] = g_nes_rgb[g->pal[p * 4 + c] & 0x3F];
        }
        for (int x = 0; x < FRAME_W;) {
            int nx = (ox + x) & 0x1FF, col = nx & 7, tx = nx >> 3, ltx = tx & 31;
            int page = nt_page(f->mirroring, row_nt * 2 + (tx >> 5)) * 0x400;
            uint8_t tile = g->nt[(page + ty * 32 + ltx) & 0xFFF];
            uint8_t attr = g->nt[(page + 0x3C0 + (ty >> 2) * 8 + (ltx >> 2)) & 0xFFF];
            int pal = (attr >> ((((ty >> 1) & 1) * 2 + ((ltx >> 1) & 1)) * 2)) & 3;
            int a = base + tile * 16 + trow;
            uint8_t lo = chr_byte(g->chr, a), hi = chr_byte(g->chr, a + 8);
            for (int i = col; i < 8 && x < FRAME_W; i++, x++) {
                int c = ((lo >> (7 - i)) & 1) | (((hi >> (7 - i)) & 1) << 1);
                uint32_t rgb;
                if (x < 8 && !(g->mask & 0x02)) {
                    rgb = back;
                    s_opaque[line][x] = 0;
                } else {
                    rgb = colors[pal][c];
                    s_opaque[line][x] = c != 0;
                }
                fb[line * FRAME_W + x] = rgb;
                s_under[line][x] = rgb;
            }
        }
        if (canon) {
            if (++fine == 8) {
                fine = 0;
                if (coarse == 29) { coarse = 0; canon_nt ^= 1; }
                else if (coarse == 31) coarse = 0;
                else coarse = (coarse + 1) & 0x1F;
            }
        } else {
            y_abs++;
        }
    }
}

static int draw_sprites(const FrameRecord *f, uint32_t *fb, int limit) {
    int tall = (f->ctrl_end & 0x20) != 0, h = tall ? 16 : 8;
    int hit = 0;
    if (!(f->mask0 & 0x10)) return 0;
    for (int line = 1; line < FRAME_H; line++) {
        int list[64], n = 0, late = f->irq_fired && line >= f->last_irq + 1;
        uint8_t mask = late ? f->mask_end : f->mask0;
        const uint16_t *map = late ? f->chr_end : f->chr0;
        if (!(mask & 0x10)) continue;
        for (int s = 0; s < 64 && !(limit && n == 8); s++) {
            int y = f->oam[s * 4];
            if (y < 0xEF && line > y && line <= y + h) list[n++] = s;
        }
        for (int k = n - 1; k >= 0; k--) {
            int s = list[k];
            int y = f->oam[s * 4], tile = f->oam[s * 4 + 1], attr = f->oam[s * 4 + 2], x = f->oam[s * 4 + 3];
            int row = line - y - 1, rr = (attr & 0x80) ? h - 1 - row : row, t, a;
            const uint8_t *pal = f->pal_end;
            uint8_t lo, hi;
            if (tall) {
                a = (tile & 1) ? 0x1000 : 0;
                t = (tile & 0xFE) + (rr >= 8);
            } else {
                a = (f->ctrl_end & 0x08) ? 0x1000 : 0;
                t = tile;
            }
            a += t * 16 + (rr & 7);
            lo = chr_byte(map, a);
            hi = chr_byte(map, a + 8);
            for (int i = 0; i < 8; i++) {
                int px = x + i, bit = (attr & 0x40) ? i : 7 - i;
                int c = ((lo >> bit) & 1) | (((hi >> bit) & 1) << 1);
                int under;
                if (px >= FRAME_W || !c) continue;
                if (px < 8 && !(mask & 0x04)) continue;
                under = s_opaque[line][px];
                if (s == 0 && px < 255 && (mask & 0x18) == 0x18 && under && !(px < 8 && !(mask & 0x02)))
                    hit = 1;
                if ((attr & 0x20) && under)
                    fb[line * FRAME_W + px] = s_under[line][px];
                else
                    fb[line * FRAME_W + px] = g_nes_rgb[pal[(((attr & 3) + 4) * 4 + c) & 0x1F] & 0x3F];
            }
        }
    }
    return hit;
}

int render_frame(const FrameRecord *f, uint32_t *fb, int sprite_limit) {
    uint32_t back = g_nes_rgb[f->backdrop & 0x3F];
    for (int i = 0; i < FRAME_W * FRAME_H; i++) fb[i] = back;
    memset(s_opaque, 0, sizeof s_opaque);
    for (int y = 0; y < FRAME_H; y++)
        for (int x = 0; x < FRAME_W; x++) s_under[y][x] = back;
    if (f->mask0 & 0x08) draw_background(f, fb, back);
    return draw_sprites(f, fb, sprite_limit);
}
