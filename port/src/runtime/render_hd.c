/*
 * render_hd.c - the picture from the game's state (PLAN_DECOMP.md 2.6,
 * stage 2; render_hd.h has the options)
 *
 * The playfield. The round's stage map (docs/assets.md, "Round
 * backgrounds") is decoded once per area from prg.bin: screens of 8x5
 * metatiles, metatiles of 2x2 blocks, blocks of 2x2 tiles with a palette.
 * Which part of it is on screen comes from the column writer's state: N =
 * 32 * bg_screen ($29) + bg_column ($24) is the newest world column it has
 * sent (one less while vram_column_request $1A still holds it), so the 32
 * nametable columns hold world columns N-31..N, column c the one equal to
 * c mod 32. Through that, every nametable cell of rows 4-24 the picture
 * shows is a cell of the map: row 4 and row 24 are the blank tile 0 (the
 * writer sends them so), and an attribute half is the map's for the world
 * column that wrote it last (docs/assets.md, step 4: the newest column at
 * the right edge keeps the old palette until its odd neighbour arrives).
 * The scripted VRAM writes (the door and trapdoor patches, docs/assets.md
 * "Nametable patches") are cells the game's own requests ($19 lists and
 * $64 strips at $0780, read before the NMI uploads them) put over the map.
 * The camera is 8 * (N - 32) + the playfield's fine x: the frame is a
 * stage frame only while the scroll and N agree. The vertical position of
 * every line (the splits, the shakes) is the PPU's, as render_frame.c
 * works it out from the frame's record.
 *
 * The sprites. When objects_draw ($E9BC) starts, its input (all of RAM) is
 * saved (timing.c calls hd_objects_draw_start); a model of objects_draw,
 * object_draw and their helpers (port/src/game/stage_draw.c, step by step)
 * runs on a copy of it and gives every object it handled: where, which
 * frame of which animation, which sprites, which OAM entries. If the OAM
 * page it builds is the one the frame shows, the sprites come from it;
 * otherwise (screens that write OAM themselves) from OAM.
 *
 * Everything else (title, story, select, round map, continue, ending, the
 * status rows above and under the playfield) is the nametables', drawn as
 * render_frame.c's console picture draws them.
 */
#include "render_hd.h"
#include "machine.h"
#include "state.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HdOptions g_hd;

static int s_check;                     /* MFF_HDCHECK */
static FILE *s_check_log;

/* ---- options ---- */

int hd_option(const char *key, const char *value) {
    if (!strcmp(key, "no_flicker") || !strcmp(key, "no-flicker")) {
        g_hd.no_flicker = value ? atoi(value) != 0 : 1;
        if (g_hd.no_flicker) g_picture = PICTURE_HD;
        return 0;
    }
    if (!strcmp(key, "wide")) {
        int w = value ? atoi(value) : 1;
        if (w == 1) w = HD_WIDE_W;
        if (w != 0 && (w < 256 || w > HD_MAX_W)) return -1;
        g_hd.wide = w == 256 ? 0 : (w & ~1);
        if (g_hd.wide) g_picture = PICTURE_HD;
        return 0;
    }
    if (!strcmp(key, "picture") && value) {
        if (!strcmp(value, "console")) g_picture = PICTURE_CONSOLE;
        else if (!strcmp(value, "scaffold")) g_picture = PICTURE_SCAFFOLD;
        else if (!strcmp(value, "hd")) g_picture = PICTURE_HD;
        else return -1;
        return 0;
    }
    return -1;
}

int hd_width(void) { return (g_picture == PICTURE_HD && g_hd.wide) ? g_hd.wide : FRAME_W; }

int hd_active(void) { return g_picture == PICTURE_HD || s_check; }

/* ---- the data ---- */

static int s_missing;                   /* reads of bytes prg.bin does not hold */

static uint8_t prg(int bank, uint16_t addr) {
    uint32_t off = (uint32_t)bank * 0x2000 + (addr & 0x1FFF);
    if (off >= PRG_SIZE) {
        s_missing++;
        return 0;
    }
    if (!cart_has_prg(off)) s_missing++;
    return g_cart.prg[off];
}

/* a CPU read at $A000-$FFFF with `bank` at $A000 (the column writer's view) */
static uint8_t round_read(int bank, uint16_t addr) {
    if (addr < 0xC000) return prg(bank, addr);
    return prg(addr < 0xE000 ? 0x0E : 0x0F, addr);
}

#define ROUND_BANKS   0xC1F8            /* bank $0E: the round's bank at $A000 */
#define AREA_RECORDS  0xAE60            /* + $30 * area */
#define SCREENS       0xAF00            /* + $40 * screen */
#define METATILES     0xB700            /* + 4 * metatile */
#define BLOCK_TL      0xBB00            /* + block: tiles TL, TR, BL, BR */
#define BLOCK_PAL     0xBF00
#define MAP_ROWS      20                /* tile rows of nametable rows 4..23 */
#define MAX_SCREENS   9                 /* 8 in the list and the one the writer reads past it */
#define MAX_COLS      (MAX_SCREENS * 32)

typedef struct {
    int valid, round, area;
    int bank, last, chr_r0, chr_r1;
    int ncols;                          /* world columns decoded: last + 2 screens */
    uint8_t tile[MAP_ROWS][MAX_COLS];
    uint8_t attr[5][MAX_COLS / 4];      /* the attribute byte of each metatile */
} AreaMap;

static AreaMap s_map;

static void area_load(int rnd, int area) {
    int bank = prg(0x0E, (uint16_t)(ROUND_BANKS + rnd)), rec = AREA_RECORDS + 0x30 * area;
    int hdr, missing = s_missing;
    memset(&s_map, 0, sizeof s_map);
    s_map.round = rnd;
    s_map.area = area;
    s_map.bank = bank;
    hdr = prg(bank, (uint16_t)(rec + 8));
    s_map.last = hdr & 0x1F;
    s_map.chr_r0 = prg(bank, (uint16_t)(rec + 0x18));
    s_map.chr_r1 = prg(bank, (uint16_t)(rec + 0x19));
    if (s_map.last > 7) return;
    s_map.ncols = (s_map.last + 2) * 32;
    for (int si = 0; si <= s_map.last + 1; si++) {
        /* the list entry: entry last + 1 is the one past the list (the
         * header when the list is full), read for the hidden column */
        int screen = prg(bank, (uint16_t)(rec + si));
        uint16_t base = (uint16_t)(SCREENS + 0x40 * screen);
        for (int r = 0; r < 5; r++) {
            for (int mc = 0; mc < 8; mc++) {
                int m = round_read(bank, (uint16_t)(base + 8 * r + mc)), a = 0;
                for (int q = 0; q < 4; q++) {
                    int b = prg(bank, (uint16_t)(METATILES + 4 * m + q));
                    int pal = prg(bank, (uint16_t)(BLOCK_PAL + b)) & 3;
                    int col = si * 32 + mc * 4 + (q & 1) * 2, row = r * 4 + (q >> 1) * 2;
                    for (int k = 0; k < 4; k++)
                        s_map.tile[row + (k >> 1)][col + (k & 1)] =
                            prg(bank, (uint16_t)(BLOCK_TL + 0x100 * k + b));
                    a |= pal << (2 * q);
                }
                s_map.attr[r][si * 8 + mc] = (uint8_t)a;
            }
        }
    }
    s_missing = missing;                /* the screen past the list may read code: never shown */
    s_map.valid = 1;
}

/* the world column nametable column c holds when N is the newest one sent */
static int wcol(int n, int c) { return n - ((n - c) & 31); }

/* ---- the VRAM requests the game makes over the map (patches) ---- */

static uint8_t s_pre_ram[0x800];        /* RAM before this frame's NMI */
static int s_have_pre;
static int16_t s_ov_tile[21][MAX_COLS]; /* nametable rows 4..24, -1 none */
static int16_t s_ov_attr[7][MAX_COLS];  /* attribute rows 0..6 by world column, -1 none */
static int s_ov_round = -1, s_ov_area = -1, s_ov_last_n = -1;
static int s_ov_frame;                  /* playfield cells written this frame (check) */
static long s_ov_writes;                /* and in all */

static void overlay_clear(void) {
    memset(s_ov_tile, 0xFF, sizeof s_ov_tile);
    memset(s_ov_attr, 0xFF, sizeof s_ov_attr);
}

void hd_frame_start(void) {
    if (!hd_active()) return;
    memcpy(s_pre_ram, g_ram, sizeof s_pre_ram);
    s_have_pre = 1;
}

/* one nametable byte the NMI wrote at PPU address `a`, N being the newest
 * column the writer had sent */
static void overlay_put(uint16_t a, uint8_t v, int n) {
    int off, x;
    a &= 0x2FFF;
    if (a < 0x2000 || (a & 0x0800)) return;     /* horizontal mirroring: $2000/$2400 is the playfield's */
    off = a & 0x3FF;
    if (off < 0x3C0) {
        int row = off >> 5, col = off & 31;
        if (row < 4 || row > 24) return;
        x = wcol(n, col);
        if (x < 0) x = col;
        if (x >= MAX_COLS) return;
        s_ov_tile[row - 4][x] = v;
    } else {
        int ar = (off - 0x3C0) >> 3, ac = (off - 0x3C0) & 7;
        if (ar < 1 || ar > 6) return;
        for (int k = 0; k < 4; k++) {
            x = wcol(n, ac * 4 + k);
            if (x < 0) x = ac * 4 + k;
            if (x < MAX_COLS) s_ov_attr[ar][x] = v;
        }
    }
    s_ov_frame++;
    s_ov_writes++;
}

/* the writer sent world column X: its cells are the map's again, and so is
 * the attribute byte it rewrote (the other half from its own copy, $0640) */
static void overlay_column_sent(int x) {
    if (x < 0 || x >= MAX_COLS) return;
    for (int r = 0; r < 21; r++) s_ov_tile[r][x] = -1;
    if (x & 1) {
        int ac = (x & 31) >> 2;
        for (int k = 0; k < 4; k++) {
            int w = wcol(x, ac * 4 + k);
            if (w < 0 || w >= MAX_COLS) continue;
            for (int r = 0; r < 7; r++) s_ov_attr[r][w] = -1;
        }
    }
}

static int writer_n(const uint8_t *ram) {
    if (ram[BG_SCREEN] == 0xFF) return -1;
    return 32 * ram[BG_SCREEN] + ram[BG_COLUMN] - (ram[VRAM_COLUMN_REQUEST] ? 1 : 0);
}

/* after the NMI: what it uploaded of the requests saved before it, in its
 * order (the strips, the lists, the column) */
static void overlay_update(void) {
    const uint8_t *p = s_pre_ram;
    int rnd = g_ram[ROUND], area = g_ram[SUB_AREA], n, n_post = writer_n(g_ram);
    s_ov_frame = 0;
    if (rnd != s_ov_round || area != s_ov_area) {
        overlay_clear();
        s_ov_round = rnd;
        s_ov_area = area;
    }
    if (s_have_pre) {
        n = writer_n(p);
        if (p[VRAM_REQUEST] && !g_ram[VRAM_REQUEST]) {
            /* the strips: [count, hi, lo, bytes...] up to vram_buffer_len */
            int x = 0, end = p[VRAM_BUFFER_LEN], inc = (p[PPUCTRL_SHADOW] & 0x04) ? 32 : 1;
            for (int guard = 0; guard < 128; guard++) {
                int cnt = p[(VRAM_BUFFER + x) & 0x7FF];
                uint16_t a = (uint16_t)((p[(VRAM_BUFFER + ((x + 1) & 0xFF)) & 0x7FF] << 8) |
                                        p[(VRAM_BUFFER + ((x + 2) & 0xFF)) & 0x7FF]);
                x = (x + 3) & 0xFF;
                if (cnt == 0) cnt = 256;
                for (int k = 0; k < cnt; k++, x = (x + 1) & 0xFF)
                    overlay_put((uint16_t)(a + k * inc), p[(VRAM_BUFFER + x) & 0x7FF], n);
                if (x == end) break;
            }
        }
        if (p[VRAM_LIST_REQUEST] && !g_ram[VRAM_LIST_REQUEST]) {
            /* the write lists: [hi, lo, count-1, bytes...] until bit 7 (+1) */
            int x = 0;
            for (int guard = 0; guard < 128; guard++) {
                uint8_t hi = p[(VRAM_BUFFER + x) & 0x7FF], lo, cnt;
                if (hi & 0x80) break;
                lo = p[(VRAM_BUFFER + ((x + 1) & 0xFF)) & 0x7FF];
                cnt = p[(VRAM_BUFFER + ((x + 2) & 0xFF)) & 0x7FF];
                for (int k = 0; k <= cnt; k++)
                    overlay_put((uint16_t)(((hi << 8) | lo) + k), p[(VRAM_BUFFER + ((x + 3 + k) & 0xFF)) & 0x7FF], n);
                x = (x + 4 + cnt) & 0xFF;
                if (!x) break;
            }
        }
    }
    if (n_post < s_ov_last_n) {
        overlay_clear();                /* the writer starts the area over */
    } else {
        for (int x = s_ov_last_n + 1; x <= n_post && x < MAX_COLS; x++) overlay_column_sent(x);
    }
    s_ov_last_n = n_post;
}

/* ---- the frame: a stage frame or not ---- */

typedef struct {
    int ok;
    const char *why;                    /* not a stage frame: why */
    int n;                              /* newest world column sent */
    int cam;                            /* world x at the original screen's x = 0 */
    int cam_region;                     /* the playfield region with the camera's scroll */
    int held;                           /* not a stage frame, but the wide picture keeps the
                                         * last one's camera (an area's exit) */
} Stage;

/* the last stage frame, for the frames of an area's exit (the game puts the
 * scroll back to 0 while the old area fades out) */
static struct {
    int valid, round, area, cam, n;
    uint64_t frame;
} s_last;

static int region_ox(const PpuRegion *g) {
    return (((g->t & 0x1F) << 3) | g->fine_x) + ((g->t & 0x400) ? 256 : 0);
}

static int area_banks(const PpuRegion *g) {
    return g->chr[0] == (s_map.chr_r0 & 0xFE) && g->chr[2] == (s_map.chr_r1 & 0xFE);
}

static Stage stage_of(const FrameRecord *f) {
    Stage st;
    int rnd = g_ram[ROUND], area = g_ram[SUB_AREA], ox;
    memset(&st, 0, sizeof st);
    st.why = "";
    if (!(f->mask0 & 0x08)) { st.why = "off"; return st; }
    if (rnd > 4 || area > 2) { st.why = "screen"; return st; }
    if (g_ram[GAME_ACTION] == 0x07 || g_ram[GAME_ACTION] == 0x09 || g_ram[GAME_FINISHED]) {
        st.why = "screen";
        return st;
    }
    if (f->mirroring != MIRROR_HORIZONTAL) { st.why = "screen"; return st; }
    if (!s_map.valid || s_map.round != rnd || s_map.area != area) area_load(rnd, area);
    if (!s_map.valid) { st.why = "screen"; return st; }
    st.cam_region = -1;
    for (int k = 1; k < f->nregions; k++)
        if (area_banks(&f->region[k])) st.cam_region = k;
    if (st.cam_region < 0) { st.why = "banks"; return st; }
    st.n = writer_n(g_ram);
    if (st.n < 32 || st.n >= s_map.ncols) { st.why = "drawing"; return st; }
    ox = region_ox(&f->region[st.cam_region]);
    if (((ox >> 3) & 31) != (st.n & 31) || g_ram[SCROLL_MODE] == 2) {
        st.why = "step";
        if (s_last.valid && s_last.round == rnd && s_last.area == area && g_frame_count - s_last.frame < 120) {
            st.held = 1;
            st.cam = s_last.cam;
            st.n = s_last.n;
        }
        return st;
    }
    st.cam = 8 * (st.n - 32) + (ox & 7);
    st.ok = 1;
    s_last.valid = 1;
    s_last.round = rnd;
    s_last.area = area;
    s_last.cam = st.cam;
    s_last.n = st.n;
    s_last.frame = g_frame_count;
    return st;
}

/* ---- line geometry: the PPU's, as render_frame.c's console picture ---- */

typedef struct {
    const PpuRegion *g;
    int region, ox, row_nt, ty, trow;
} Line;

static Line s_line[FRAME_H];

static void geometry(const FrameRecord *f) {
    const PpuRegion *g = &f->region[0];
    int coarse = (g->t >> 5) & 31, fine = (g->t >> 12) & 7, row_nt = (g->t >> 11) & 1;
    for (int line = 0; line < FRAME_H; line++) {
        const PpuRegion *now = &f->region[0];
        int k;
        for (k = 1; k < f->nregions && f->region[k].first_line + 1 <= line; k++) now = &f->region[k];
        if (now != g && now->v_set) {
            coarse = (now->v >> 5) & 31;
            fine = (now->v >> 12) & 7;
            row_nt = (now->v >> 11) & 1;
        }
        g = now;
        s_line[line].g = g;
        s_line[line].region = (int)(g - f->region);
        s_line[line].ox = region_ox(g);
        s_line[line].row_nt = row_nt;
        s_line[line].ty = coarse;
        s_line[line].trow = fine;
        if (++fine == 8) {
            fine = 0;
            if (coarse == 29) { coarse = 0; row_nt ^= 1; }
            else if (coarse == 31) coarse = 0;
            else coarse++;
        }
    }
}

/* ---- the background ---- */

static uint8_t s_bg[FRAME_H][HD_MAX_W];       /* colour index */
static uint8_t s_opq[FRAME_H][HD_MAX_W];      /* not the backdrop */

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

/* one pixel row of a tile into the line at out_x (clipped to [0, w)) */
static void put_tile_row(int line, int out_x, int from, int to, const PpuRegion *g, int tile, int pal,
                         int trow, int w) {
    int base = (g->ctrl & 0x10) ? 0x1000 : 0, a = base + tile * 16 + trow;
    uint8_t lo = chr_byte(g->chr, a), hi = chr_byte(g->chr, a + 8);
    for (int i = from; i < to; i++) {
        int x = out_x + i - from, c;
        if (x < 0 || x >= w) continue;
        c = ((lo >> (7 - i)) & 1) | (((hi >> (7 - i)) & 1) << 1);
        s_bg[line][x] = g->pal[c ? pal * 4 + c : 0] & 0x3F;
        s_opq[line][x] = c != 0;
    }
}

/* the cell of a map line: tile and palette of world column X, nametable
 * row ty (4..24); with `lag`, the attribute as the writer left it in the
 * nametable (N the newest column) */
static void map_cell(int X, int ty, int n, int lag, int *tile, int *pal) {
    int r, half, shift, src;
    if (X < 0 || X >= s_map.ncols) { *tile = 0; *pal = 0; return; }
    if (s_ov_tile[ty - 4][X] >= 0) *tile = s_ov_tile[ty - 4][X];
    else *tile = (ty == 4 || ty == 24) ? 0 : s_map.tile[ty - 4][X];
    if (ty >= 24) { *pal = 0; return; }
    r = (ty - 4) >> 2;
    half = (X >> 1) & 1;
    shift = ((((ty >> 1) & 1) * 2) + half) * 2;
    if (s_ov_attr[r + 1][X] >= 0) {
        *pal = (s_ov_attr[r + 1][X] >> shift) & 3;
        return;
    }
    src = X;
    if (lag) src = wcol(n, ((X & 31) & ~3) + 1 + 2 * half);
    if (src < 0) src = X;
    *pal = (s_map.attr[r][src >> 2] >> shift) & 3;
}

enum { LINE_NT, LINE_STILL, LINE_CAM };

static int line_kind(const Stage *st, const Line *L, int wide) {
    if (!(st->ok || (wide && st->held)) || L->row_nt != 0 || L->ty < 4 || L->ty > 24 || !area_banks(L->g)) return LINE_NT;
    return L->region == st->cam_region ? LINE_CAM : LINE_STILL;
}

/* a nametable line as the console draws it, its 256 columns at out_x */
static void line_nt(const FrameRecord *f, int line, int out_x, int w) {
    const Line *L = &s_line[line];
    const PpuRegion *g = L->g;
    for (int x = 0; x < FRAME_W;) {
        int nx = (L->ox + x) & 0x1FF, col = nx & 7, tx = nx >> 3, ltx = tx & 31, run = 8 - col;
        int page = nt_page(f->mirroring, L->row_nt * 2 + (tx >> 5)) * 0x400;
        uint8_t tile = g->nt[(page + L->ty * 32 + ltx) & 0xFFF];
        uint8_t attr = g->nt[(page + 0x3C0 + (L->ty >> 2) * 8 + (ltx >> 2)) & 0xFFF];
        int pal = (attr >> ((((L->ty >> 1) & 1) * 2 + ((ltx >> 1) & 1)) * 2)) & 3;
        if (run > FRAME_W - x) run = FRAME_W - x;
        put_tile_row(line, out_x + x, col, col + run, g, tile, pal, L->trow, w);
        x += run;
    }
}

/* a map line through the nametable's window (the console's columns) */
static void line_map_window(int line, const Stage *st, int out_x, int lag, int from_x, int to_x, int w) {
    const Line *L = &s_line[line];
    for (int x = from_x; x < to_x;) {
        int nx = ((L->ox + x) % 512 + 512) % 512, col = nx & 7, run = 8 - col, tile, pal;
        int X = wcol(st->n, (nx >> 3) & 31);
        if (run > to_x - x) run = to_x - x;
        map_cell(X, L->ty, st->n, lag, &tile, &pal);
        put_tile_row(line, out_x + x, col, col + run, L->g, tile, pal, L->trow, w);
        x += run;
    }
}

/* a camera line at world x wx0 for the picture's x = 0 */
static void line_map_world(int line, int wx0, int w) {
    const Line *L = &s_line[line];
    int wmax = (s_map.last + 1) * 256;
    for (int x = 0; x < w;) {
        int X = wx0 + x, col, run, tile, pal;
        if (X < 0 || X >= wmax) {
            s_bg[line][x] = L->g->pal[0] & 0x3F;
            s_opq[line][x] = 0;
            x++;
            continue;
        }
        col = X & 7;
        run = 8 - col;
        if (run > w - x) run = w - x;
        map_cell(X >> 3, L->ty, 0, 0, &tile, &pal);
        put_tile_row(line, x, col, col + run, L->g, tile, pal, L->trow, w);
        x += run;
    }
}

/* the backdrop over the PPU's clipped left column (PPUMASK bit 1) */
static void clip_left(int line, int out_x, int w) {
    const PpuRegion *g = s_line[line].g;
    if (g->mask & 0x02) return;
    for (int x = 0; x < 8; x++) {
        if (out_x + x < 0 || out_x + x >= w) continue;
        s_bg[line][out_x + x] = g->pal[0] & 0x3F;
        s_opq[line][out_x + x] = 0;
    }
}

typedef struct {
    int w;                              /* picture width */
    int wide;
    int no_flicker;
    int view_x;                         /* the original screen's x = 0 in the picture */
    int hud_x;                          /* the status rows' and the screens' x = 0 */
    int wx0;                            /* world x at the picture's x = 0 (camera lines) */
} View;

static void background(const FrameRecord *f, const Stage *st, const View *v, int *map_lines) {
    uint8_t back = f->backdrop & 0x3F;
    *map_lines = 0;
    for (int line = 0; line < FRAME_H; line++) {
        memset(s_bg[line], back, (size_t)v->w);
        memset(s_opq[line], 0, (size_t)v->w);
    }
    if (!(f->mask0 & 0x08)) return;
    for (int line = 0; line < FRAME_H; line++) {
        const Line *L = &s_line[line];
        int kind = line_kind(st, L, v->wide);
        uint8_t lb = L->g->pal[0] & 0x3F;
        if (v->wide) {                  /* the margins of a line drawn from 256 columns */
            memset(s_bg[line], lb, (size_t)v->w);
        }
        if (kind != LINE_NT) (*map_lines)++;
        if (!v->wide) {
            if (kind == LINE_NT) line_nt(f, line, 0, v->w);
            else line_map_window(line, st, 0, 1, 0, FRAME_W, v->w);
            clip_left(line, 0, v->w);
        } else if (kind == LINE_CAM) {
            line_map_world(line, v->wx0, v->w);
        } else if (kind == LINE_STILL) {
            line_map_window(line, st, v->hud_x, 0, -v->hud_x, v->w - v->hud_x, v->w);
        } else {
            line_nt(f, line, v->hud_x, v->w);
            clip_left(line, v->hud_x, v->w);
        }
    }
}

/* ---- the sprites: a model of objects_draw ---- */

#define OBJ_SPRITE_BANKS 0xEB93         /* bank $0F: the sprite bank pair of each id */
#define OFFSCREEN_FREE   0xECC3         /* bank $0F: ids freed far off the screen, $FF ends */
#define NEAR_DISTANCE    0x68
#define FRAME_MAX_SPR    32

typedef struct {
    int x, y;                           /* on the original screen: x any int, y as OAM's */
    uint8_t tile, attr;
    int drop_y;                         /* the game drops it whatever x is (wraps, or y >= $F0) */
    int oam;                            /* the OAM entry the game gave it, -1 none */
} Spr;

enum { CALL_DRAWN, CALL_NEAR };

typedef struct {
    int slot;                           /* X: 0-15, or past 15 (objects_draw's second call) */
    int kind;                           /* CALL_DRAWN, CALL_NEAR, -1 nothing to show */
    int world;                          /* obj_flags bit 4 */
    int nspr;
    Spr spr[FRAME_MAX_SPR];
} DrawCall;

#define MAX_CALLS 24

typedef struct {
    int ncalls;
    DrawCall call[MAX_CALLS];
    uint8_t oam[256];
    int bad;                            /* the model read something it cannot */
    int unplaced;                       /* sprites past a full OAM (the 63 entries) */
} Model;

typedef struct {
    uint8_t ram[0x800];
    int b8, ba;                         /* banks at $8000 and $A000 */
} Cpu;

#define MR(a) c->ram[(a) & 0x7FF]

static uint8_t c_read(const Cpu *c, uint16_t a, int *bad) {
    if (a < 0x2000) return c->ram[a & 0x7FF];
    if (a < 0x8000) { *bad = 1; return 0; }
    if (a < 0xA000) return prg(c->b8, a);
    if (a < 0xC000) return prg(c->ba, a);
    return prg(a < 0xE000 ? 0x0E : 0x0F, a);
}

/* object_free, $EC81 */
static void m_free(Cpu *c, int x) {
    static const uint16_t fields[] = {
        OBJ_STATE, OBJ_SAVED_STATE, OBJ_FLAGS, OBJ_THROW_FLAG, OBJ_HIT_COOLDOWN, OBJ_ID, OBJ_SAVED_ID,
        OBJ_ATTACK, OBJ_ATTACK_DAMAGE, OBJ_INVULN, OBJ_HIT_PLAYER, OBJ_HIT_TIMER, OBJ_KIND, OBJ_TIER,
        OBJ_AI_TIMER, OBJ_AI_COUNT, OBJ_AI_FLAG, OBJ_SPAWN_GROUP, OBJ_HELD,
    };
    for (unsigned i = 0; i < sizeof fields / sizeof fields[0]; i++) MR(fields[i] + x) = 0;
    MR(OBJ_SPAWN_INDEX + x) = 0xFF;
}

/* object_offscreen, $EC33: freed, or pulled 16 pixels toward the camera */
static void m_offscreen(Cpu *c, int x) {
    int pos;
    if (MR(ROUND) == 3 && MR(SUB_AREA) == 1) { m_free(c, x); return; }
    for (int y = 0; y < 256; y++) {
        uint8_t id = prg(0x0F, (uint16_t)(OFFSCREEN_FREE + y));
        if (id == MR(OBJ_ID + x)) { m_free(c, x); return; }
        if (id == 0xFF) break;
        if (y == 255) return;
    }
    pos = MR(OBJ_X_HI + x) << 8 | MR(OBJ_X + x);
    if (!((MR(SCROLL_X_HI) - MR(OBJ_X_HI + x)) & 0x80)) pos += 0x10;
    else pos -= 0x10;
    MR(OBJ_X + x) = (uint8_t)pos;
    MR(OBJ_X_HI + x) = (uint8_t)(pos >> 8);
}

/* every sprite of frame `fid` of a bank pair at anchor (sx, sy) as the
 * game would place it, without touching the state: for the objects it
 * animates off its screen and for the sprites past a full OAM */
static void frame_sprites(int pair, int fid, int sx, int sy, uint8_t flags, DrawCall *dc, int *bad) {
    Cpu tmp;
    const Cpu *c = &tmp;
    uint16_t fp, lp;
    uint8_t head, li, flip = flags & 0x40, prio = flags & 0x20;
    int n, y = 1;
    tmp.b8 = pair;
    tmp.ba = pair + 1;
    fp = (uint16_t)(c_read(c, (uint16_t)(0x8100 + fid), bad) | c_read(c, (uint16_t)(0x8200 + fid), bad) << 8);
    head = c_read(c, fp, bad);
    tmp.ba = (head & 0x80) ? 0x03 : 0x0B;
    n = (head & 0x7F) + 1;
    li = c_read(c, (uint16_t)(fp + 1), bad);
    lp = (uint16_t)((c_read(c, (uint16_t)(0xB100 + li), bad) | c_read(c, (uint16_t)(0xB200 + li), bad) << 8) - 2);
    dc->nspr = 0;
    for (int i = 0; i < n && i < FRAME_MAX_SPR; i++) {
        Spr *s = &dc->spr[dc->nspr++];
        uint8_t dyb, dxb;
        int yy;
        y = (y + 1) & 0xFF;
        s->tile = c_read(c, (uint16_t)(fp + y), bad);
        dyb = c_read(c, (uint16_t)(lp + y), bad);
        y = (y + 1) & 0xFF;
        s->attr = (uint8_t)((c_read(c, (uint16_t)(fp + y), bad) ^ flip) | prio);
        dxb = c_read(c, (uint16_t)(lp + y), bad);
        if (flip) dxb = (uint8_t)(0xF8 - dxb);
        yy = sy + (int8_t)dyb;
        s->drop_y = yy < 0 || yy >= 0xF0;
        s->y = yy & 0xFF;
        s->x = sx + (int8_t)dxb;
        s->oam = -1;
    }
}

/* object_draw_sprites, $EA6F; returns object_draw's X */
static int m_draw_sprites(Cpu *c, int x, int sx, Model *out, DrawCall *dc) {
    int pair = prg(0x0F, (uint16_t)(OBJ_SPRITE_BANKS + MR(OBJ_ID + x)));
    uint8_t flags, anim, t, period, last, f, fi, fid, head, lb, li;
    uint16_t ptr, fp, lp;
    int xo, y, i, bad = 0;
    MR(BANK_REQUEST) = (uint8_t)pair;
    MR(BANK_REQUEST + 1) = (uint8_t)(pair + 1);
    c->b8 = pair;
    c->ba = pair + 1;
    MR(TMP0) = (uint8_t)x;
    flags = MR(OBJ_FLAGS + x);
    MR(TMP16) = flags & 0x40;
    MR(TMP17) = flags & 0x20;
    anim = MR(OBJ_ANIM + x);
    if (!anim) return x;
    y = anim & 0x7F;
    MR(TMP0) = c_read(c, (uint16_t)(0x8000 + y), &bad);
    MR(TMP1) = c_read(c, (uint16_t)(0x8080 + y), &bad);
    ptr = (uint16_t)(MR(TMP0) | MR(TMP1) << 8);
    t = MR(OBJ_ANIM_TIMER + x) & 0x7F;
    MR(OBJ_ANIM_TIMER + x)++;
    period = c_read(c, (uint16_t)(ptr + 1), &bad);
    if (t == period) {
        MR(OBJ_ANIM_TIMER + x) &= 0x80;
        f = MR(OBJ_ANIM_FRAME + x) & 0x7F;
        MR(OBJ_ANIM_FRAME + x)++;
        last = c_read(c, ptr, &bad);
        if (f == last) MR(OBJ_ANIM_FRAME + x) = 0;
    }
    fi = (uint8_t)((MR(OBJ_ANIM_FRAME + x) & 0x7F) + 2);
    if (!(MR(OBJ_FLAGS + x) & 0x80)) {          /* near: animated, not drawn */
        fid = c_read(c, (uint16_t)(ptr + fi), &bad);
        /* shown in a wide picture as the game would show it on its screen:
         * not while hidden (bit 2) or blinking out */
        if (fid && dc && !(MR(OBJ_FLAGS + x) & 0x04) &&
            !((MR(OBJ_ANIM_TIMER + x) & 0x80) && (MR(FRAME_COUNTER) & 0x04))) {
            dc->kind = CALL_NEAR;
            frame_sprites(pair, fid, sx, MR(OBJ_Y + x), MR(OBJ_FLAGS + x), dc, &bad);
        }
        out->bad |= bad;
        return x;
    }
    if ((MR(OBJ_ANIM_TIMER + x) & 0x80) && (MR(FRAME_COUNTER) & 0x04)) { out->bad |= bad; return x; }
    fid = c_read(c, (uint16_t)(ptr + fi), &bad);
    if (!fid) {
        MR(OBJ_STATE + x) = 0;
        m_free(c, x);
        out->bad |= bad;
        return x;
    }
    if (dc) {
        dc->kind = CALL_DRAWN;
        frame_sprites(pair, fid, sx, MR(TMP18), MR(OBJ_FLAGS + x), dc, &bad);
    }
    MR(TMP2) = c_read(c, (uint16_t)(0x8100 + fid), &bad);
    MR(TMP3) = c_read(c, (uint16_t)(0x8200 + fid), &bad);
    fp = (uint16_t)(MR(TMP2) | MR(TMP3) << 8);
    head = c_read(c, fp, &bad);
    lb = 0x0B;
    if (head & 0x80) { head &= 0x7F; lb = 0x03; }
    MR(TMP4) = head;
    if (lb != MR(BANK_REQUEST + 1)) {
        MR(BANK_REQUEST + 1) = lb;
        MR(TMP5) = (uint8_t)x;
        c->b8 = MR(BANK_REQUEST);
        c->ba = lb;
    }
    li = c_read(c, (uint16_t)(fp + 1), &bad);
    lp = (uint16_t)((c_read(c, (uint16_t)(0xB100 + li), &bad) | c_read(c, (uint16_t)(0xB200 + li), &bad) << 8) - 2);
    MR(TMP5) = (uint8_t)lp;
    MR(TMP6) = (uint8_t)(lp >> 8);
    xo = MR(OAM_INDEX);
    if (!xo) { out->bad |= bad; out->unplaced += head + 1; return 0; }
    y = 1;
    for (i = 0;; i++) {
        uint8_t tile, dyb, yb, dxb, attr;
        int sum, vis;
        MR(TMP0) = 0xF0;
        y = (y + 1) & 0xFF;
        tile = c_read(c, (uint16_t)(fp + y), &bad);
        MR(OAM_BUFFER + 1 + xo) = tile;
        dyb = c_read(c, (uint16_t)(lp + y), &bad);
        sum = MR(TMP18) + dyb;
        yb = (uint8_t)sum;
        MR(OAM_BUFFER + xo) = yb;
        vis = (dyb & 0x80) ? sum > 0xFF : sum <= 0xFF;
        if (vis && yb >= 0xF0) vis = 0;
        y = (y + 1) & 0xFF;
        if (vis) {
            attr = (uint8_t)((c_read(c, (uint16_t)(fp + y), &bad) ^ MR(TMP16)) | MR(TMP17));
            MR(OAM_BUFFER + 2 + xo) = attr;
            dxb = c_read(c, (uint16_t)(lp + y), &bad);
            MR(TMP1) = dxb;
            if (MR(TMP16)) MR(TMP1) = (uint8_t)(0xF8 - MR(TMP1));
            sum = MR(TMP19) + MR(TMP1);
            MR(OAM_BUFFER + 3 + xo) = (uint8_t)sum;
            vis = (MR(TMP1) & 0x80) ? sum > 0xFF : sum <= 0xFF;
            if (vis) {
                if (dc && i < dc->nspr) dc->spr[i].oam = xo >> 2;
                xo = (xo + 4) & 0xFF;
                MR(OAM_INDEX) = (uint8_t)xo;
                if (!xo) { out->bad |= bad; out->unplaced += MR(TMP4); return 0; }
            }
        }
        if (!vis) MR(OAM_BUFFER + xo) = 0xF8;
        MR(TMP4) = (uint8_t)(MR(TMP4) - 1);
        if (MR(TMP4) & 0x80) break;
    }
    out->bad |= bad;
    return xo;
}

/* object_draw, $E9FE; returns its X */
static int m_object_draw(Cpu *c, int x, Model *out) {
    DrawCall *dc = out->ncalls < MAX_CALLS ? &out->call[out->ncalls] : NULL;
    int sx, r;
    if (dc) {
        memset(dc, 0, sizeof *dc);
        dc->slot = x;
        dc->kind = -1;
        dc->world = (MR(OBJ_FLAGS + x) & 0x10) != 0;
    }
    if (MR(OBJ_FLAGS + x) & 0x10) {
        int lo = MR(OBJ_X + x) - MR(SCROLL_X), borrow = lo < 0, hi, carry;
        hi = MR(OBJ_X_HI + x) - MR(SCROLL_X_HI) - borrow;
        carry = hi >= 0;
        MR(TMP19) = (uint8_t)lo;
        sx = (int16_t)(uint16_t)(((hi & 0xFF) << 8) | (lo & 0xFF));
        if ((hi & 0xFF) == 0) {
            if (MR(OBJ_Y_HI + x) != 0) {        /* no route: obj_y_hi is always 0 */
                if (x == 0) goto near;
                m_offscreen(c, x);
                return x;
            }
        } else {
            uint8_t v = carry ? (uint8_t)lo : (uint8_t)(-lo);
            if (v >= NEAR_DISTANCE || MR(OBJ_ANIM + x) == 0x7E || (MR(ROUND) == 3 && MR(SUB_AREA) == 1)) {
                m_offscreen(c, x);
                return x;
            }
            goto near;
        }
    } else {
        sx = MR(OBJ_X + x);
        MR(TMP19) = (uint8_t)sx;
    }
    MR(TMP18) = MR(OBJ_Y + x);
    MR(OBJ_FLAGS + x) |= 0x80;
    if (MR(OBJ_FLAGS + x) & 0x04) return x;
    r = m_draw_sprites(c, x, sx, out, dc);
    if (dc && dc->kind >= 0) out->ncalls++;
    return r;
near:
    MR(OBJ_FLAGS + x) &= 0x7F;
    r = m_draw_sprites(c, x, sx, out, dc);
    if (dc && dc->kind >= 0) out->ncalls++;
    return r;
}

/* objects_draw, $E9BC, on a copy of the saved RAM */
static void m_objects_draw(const uint8_t *ram, Model *out) {
    static Cpu cpu;
    Cpu *c = &cpu;
    int x;
    memcpy(c->ram, ram, sizeof c->ram);
    c->b8 = c->ba = 0;
    memset(out, 0, sizeof *out);
    x = 0x0F;
    if (MR(OBJ_STATE + x) & 0x80) x = m_object_draw(c, x, out);
    x = (x - 1) & 0xFF;
    if (MR(OBJ_STATE + x) & 0x80) m_object_draw(c, x, out);
    MR(DRAW_PARITY)++;
    if (!(MR(DRAW_PARITY) & 1)) {
        for (x = 0; x < 16; x++) {
            MR(DRAW_SLOT) = (uint8_t)x;
            if (MR(OBJ_STATE + x) & 0x80) m_object_draw(c, x, out);
        }
    } else {
        for (x = 13; x >= 0; x--) {
            MR(DRAW_SLOT) = (uint8_t)x;
            if (MR(OBJ_STATE + x) & 0x80) m_object_draw(c, x, out);
        }
    }
    memcpy(out->oam, &c->ram[OAM_BUFFER], 256);
}

#undef MR

static uint8_t s_draw_ram[0x800];       /* RAM when objects_draw last started */
static int s_draw_seq, s_model_seq = -1;
static Model s_model;

void hd_objects_draw_start(void) {
    if (!hd_active()) return;
    memcpy(s_draw_ram, g_ram, sizeof s_draw_ram);
    s_draw_seq++;
}

/* ---- the view: where the original screen and the status rows go ---- */


static void view_of(const Stage *st, int wide, int no_flicker, View *v) {
    memset(v, 0, sizeof *v);
    v->w = wide ? wide : FRAME_W;
    v->wide = wide != 0;
    v->no_flicker = no_flicker;
    if (!wide) return;
    v->hud_x = v->view_x = (v->w - FRAME_W) / 2;
    if (st->ok || st->held) {
        /* the game's camera, held inside the area: its first and last
         * screens fill the picture instead of the backdrop around them */
        int wmax = (s_map.last + 1) * 256, cw = st->cam - v->hud_x;
        if (wmax >= v->w) {
            if (cw < 0) cw = 0;
            if (cw > wmax - v->w) cw = wmax - v->w;
        } else {
            cw = (wmax - v->w) / 2;
        }
        v->wx0 = cw;
        v->view_x = st->cam - cw;
    }
}

/* ---- the sprites: what to draw ---- */

typedef struct {
    int x, y;                           /* picture x, OAM y */
    uint8_t tile, attr;
    int limited;                        /* counts toward the 8 per line */
    int clip_lo, clip_hi;               /* drawn only inside [clip_lo, clip_hi) */
    int unclipped;                      /* PPUMASK's left column does not apply */
    int slot;                           /* the object it belongs to, -1 not known */
} HdSprite;

#define MAX_LIST (MAX_CALLS * FRAME_MAX_SPR + 64)
static HdSprite s_list[MAX_LIST];
static int s_nlist;

static void list_add(int x, int y, uint8_t tile, uint8_t attr, int limited, int lo, int hi, int unclipped,
                     int slot) {
    HdSprite *s;
    if (s_nlist >= MAX_LIST) return;
    s = &s_list[s_nlist++];
    s->x = x;
    s->y = y;
    s->tile = tile;
    s->attr = attr;
    s->limited = limited;
    s->clip_lo = lo;
    s->clip_hi = hi;
    s->unclipped = unclipped;
    s->slot = slot;
}

/* reveal: an object is drawn outside the original 256 columns only once
 * it has been wholly inside them (render_hd.h). Picture state only. */
static struct {
    int live, spawn, x, revealed;       /* revealed: 1, 0, -1 not decided yet */
} s_rev[16];

static int revealed(int slot) { return slot <= 15 && s_rev[slot].revealed > 0; }

static int s_unseen[16];                /* pictures since the object was last drawn (check) */

static void reveal_update(const Model *m, const View *v) {
    int pic_lo = -v->view_x, pic_hi = v->w - v->view_x;     /* the picture in screen x */
    for (int s = 0; s < 16; s++) {
        int live = (g_ram[OBJ_STATE + s] & 0x80) != 0;
        int x = g_ram[OBJ_X_HI + s] << 8 | g_ram[OBJ_X + s];
        int spawn = g_ram[OBJ_SPAWN_INDEX + s];
        if (live && (!s_rev[s].live || spawn != s_rev[s].spawn || abs(x - s_rev[s].x) > 64)) {
            s_rev[s].revealed = -1;
            s_unseen[s] = 1000;
        }
        if (!live) s_rev[s].revealed = 0;
        s_rev[s].live = live;
        s_rev[s].spawn = spawn;
        s_rev[s].x = x;
    }
    for (int k = 0; k < m->ncalls; k++) {
        const DrawCall *dc = &m->call[k];
        int lo = 1 << 20, hi = -(1 << 20), s = dc->slot, n = 0;
        if (s > 15) continue;
        if (!dc->world) { s_rev[s].revealed = 1; continue; }
        for (int i = 0; i < dc->nspr; i++) {
            if (dc->spr[i].drop_y) continue;
            if (dc->spr[i].x < lo) lo = dc->spr[i].x;
            if (dc->spr[i].x + 8 > hi) hi = dc->spr[i].x + 8;
            n++;
        }
        if (!n) continue;
        if (s_rev[s].revealed < 0)      /* first seen beyond the picture: it walks in */
            s_rev[s].revealed = (hi <= pic_lo || lo >= pic_hi) ? 1 : 0;
        if (lo >= 0 && hi <= FRAME_W) s_rev[s].revealed = 1;
    }
}

static int slot_rank(int s) {
    if (s == 15) return 0;
    if (s == 14) return 1;
    if (s < 14) return 2 + s;
    return 16 + s;                      /* objects_draw's second call past slot 15 */
}

static void sprites_list(const FrameRecord *f, const View *v, int model_ok, const Model *m) {
    int lo0 = v->view_x, hi0 = v->view_x + FRAME_W;
    s_nlist = 0;
    if (!model_ok) {
        /* OAM as the PPU has it, in the original screen */
        for (int s = 0; s < 64; s++)
            list_add(v->view_x + f->oam[s * 4 + 3], f->oam[s * 4], f->oam[s * 4 + 1], f->oam[s * 4 + 2],
                     !v->no_flicker, lo0, hi0, 0, -1);
        return;
    }
    if (!v->no_flicker) {
        /* the game's order: OAM as objects_draw built it, 8 per line */
        int slot_of[64];
        for (int s = 0; s < 64; s++) slot_of[s] = -1;
        for (int k = 0; k < m->ncalls; k++)
            for (int i = 0; i < m->call[k].nspr; i++)
                if (m->call[k].spr[i].oam >= 0) slot_of[m->call[k].spr[i].oam] = m->call[k].slot;
        for (int s = 0; s < 64; s++) {
            int open = v->wide && slot_of[s] >= 0 && revealed(slot_of[s]);
            list_add(v->view_x + m->oam[s * 4 + 3], m->oam[s * 4], m->oam[s * 4 + 1], m->oam[s * 4 + 2], 1,
                     open ? 0 : (v->wide ? lo0 : 0), open ? v->w : (v->wide ? hi0 : v->w), v->wide, slot_of[s]);
        }
        if (!v->wide) return;
        /* and, wide, what it left out of OAM: objects off its screen,
         * sprites past its edges or past a full OAM */
        for (int k = 0; k < m->ncalls; k++) {
            const DrawCall *dc = &m->call[k];
            int open = revealed(dc->slot);
            for (int i = 0; i < dc->nspr; i++) {
                const Spr *p = &dc->spr[i];
                if (p->oam >= 0 || p->drop_y) continue;
                list_add(v->view_x + p->x, p->y, p->tile, p->attr, 0, open ? 0 : lo0, open ? v->w : hi0, 1,
                         dc->slot);
            }
        }
        return;
    }
    /* no flicker: each object once (its last call this frame), effects
     * first, then the player and slots 1-13, every sprite */
    {
        int order[MAX_CALLS], n = 0;
        for (int k = 0; k < m->ncalls; k++) {
            int later = 0;
            for (int j = k + 1; j < m->ncalls; j++)
                if (m->call[j].slot == m->call[k].slot) later = 1;
            if (!later) order[n++] = k;
        }
        for (int i = 1; i < n; i++)
            for (int j = i; j > 0 && slot_rank(m->call[order[j - 1]].slot) > slot_rank(m->call[order[j]].slot); j--) {
                int t = order[j];
                order[j] = order[j - 1];
                order[j - 1] = t;
            }
        for (int i = 0; i < n; i++) {
            const DrawCall *dc = &m->call[order[i]];
            int open = v->wide && revealed(dc->slot);
            if (!v->wide && dc->kind != CALL_DRAWN) continue;
            for (int k = 0; k < dc->nspr; k++) {
                const Spr *p = &dc->spr[k];
                if (p->drop_y) continue;
                list_add(v->view_x + p->x, p->y, p->tile, p->attr, 0, open ? 0 : lo0, open ? v->w : hi0,
                         v->wide, dc->slot);
            }
        }
    }
}

static void sprites_draw(const FrameRecord *f, const View *v, uint8_t *pix) {
    static int list[MAX_LIST];
    if (!(f->mask0 & 0x10)) return;
    for (int line = 1; line < FRAME_H; line++) {
        const PpuRegion *g = s_line[line].g;
        int h = (g->ctrl & 0x20) ? 16 : 8, tall = h == 16, n = 0, lim = 0;
        if (!(g->mask & 0x10)) continue;
        for (int i = 0; i < s_nlist; i++) {
            const HdSprite *s = &s_list[i];
            if (s->y >= 0xEF || line <= s->y || line > s->y + h) continue;
            if (s->limited) {
                if (lim == 8) continue;
                lim++;
            }
            list[n++] = i;
        }
        for (int k = n - 1; k >= 0; k--) {
            const HdSprite *s = &s_list[list[k]];
            int row = line - s->y - 1, r = (s->attr & 0x80) ? h - 1 - row : row, a;
            uint8_t lo, hi;
            if (tall) a = ((s->tile & 1) ? 0x1000 : 0) + ((s->tile & 0xFE) + (r >= 8)) * 16;
            else a = ((g->ctrl & 0x08) ? 0x1000 : 0) + s->tile * 16;
            a += r & 7;
            lo = chr_byte(g->chr, a);
            hi = chr_byte(g->chr, a + 8);
            for (int i = 0; i < 8; i++) {
                int px = s->x + i, bit = (s->attr & 0x40) ? i : 7 - i;
                int c = ((lo >> bit) & 1) | (((hi >> bit) & 1) << 1);
                if (!c || px < 0 || px >= v->w || px < s->clip_lo || px >= s->clip_hi) continue;
                if (!s->unclipped && px >= v->view_x && px - v->view_x < 8 && !(g->mask & 0x04)) continue;
                if ((s->attr & 0x20) && s_opq[line][px])
                    pix[line * v->w + px] = s_bg[line][px];
                else
                    pix[line * v->w + px] = g->pal[(((s->attr & 3) + 4) * 4 + c) & 0x1F] & 0x3F;
            }
        }
    }
}

/* A pop-in (the check, wide): an object drawn outside the original 256
 * columns that was not drawn in the last 4 pictures (or is a new object),
 * without touching the picture's edges: it appeared in the margin. */
static int popins(const View *v) {
    int vis[16] = {0}, margin[16] = {0}, edge[16] = {0}, n = 0;
    for (int i = 0; i < s_nlist; i++) {
        const HdSprite *p = &s_list[i];
        int lo = p->x, hi = p->x + 8;
        if (p->slot < 0 || p->slot > 15 || p->y >= 0xEF) continue;
        if (lo < p->clip_lo) lo = p->clip_lo;
        if (hi > p->clip_hi) hi = p->clip_hi;
        if (lo < 0) lo = 0;
        if (hi > v->w) hi = v->w;
        if (lo >= hi) continue;
        vis[p->slot] = 1;
        if (lo < v->view_x || hi > v->view_x + FRAME_W) margin[p->slot] = 1;
        if (lo <= 0 || hi >= v->w) edge[p->slot] = 1;
    }
    for (int s = 0; s < 16; s++) {
        if (vis[s] && margin[s] && !edge[s] && s_unseen[s] > 4) n++;
        s_unseen[s] = vis[s] ? 0 : (s_unseen[s] < 1000 ? s_unseen[s] + 1 : 1000);
    }
    return n;
}

/* ---- one picture ---- */

static uint8_t s_pix[FRAME_H * HD_MAX_W];
static uint32_t s_rgb[FRAME_H * HD_MAX_W];
static int s_w = FRAME_W, s_view_x;
static uint8_t s_tmp[FRAME_H * HD_MAX_W], s_con[FRAME_W * FRAME_H];

static void draw(const FrameRecord *f, const Stage *st, const View *v, int model_ok, uint8_t *pix,
                 int *map_lines) {
    background(f, st, v, map_lines);
    for (int line = 0; line < FRAME_H; line++) memcpy(pix + line * v->w, s_bg[line], (size_t)v->w);
    sprites_list(f, v, model_ok, &s_model);
    sprites_draw(f, v, pix);
}

static struct {
    long frames, identical, map_frames, model_frames, differ;
} s_sum;

void hd_after_nmi(void) {
    if (!hd_active()) return;
    overlay_update();
    s_have_pre = 0;
}

void hd_picture(const FrameRecord *f, int shown) {
    Stage st;
    View v;
    int model_seen, model_ok, map_lines = 0, wide_lines = 0;
    int diff = 0, fx = -1, fy = -1, over = 0, pop = 0;
    if (s_model_seq != s_draw_seq) {
        m_objects_draw(s_draw_ram, &s_model);
        s_model_seq = s_draw_seq;
    }
    model_seen = s_draw_seq > 0;
    model_ok = model_seen && !s_model.bad && !memcmp(s_model.oam, f->oam, 256);
    geometry(f);
    st = stage_of(f);
    if (s_check) {
        /* every option off, against the console picture */
        render_frame(f, s_con, PICTURE_CONSOLE);
        if (f->mask0 & 0x10)                    /* sprites the 8 per line leave out */
            for (int line = 1; line < FRAME_H; line++) {
                int k = 0;
                for (int i = 0; i < 64; i++)
                    if (f->oam[i * 4] < 0xEF && line > f->oam[i * 4] && line <= f->oam[i * 4] + 8) k++;
                if (k > 8) over += k - 8;
            }
        view_of(&st, 0, 0, &v);
        draw(f, &st, &v, model_ok, s_tmp, &map_lines);
        for (int i = 0; i < FRAME_W * FRAME_H; i++)
            if (s_tmp[i] != s_con[i]) {
                if (!diff) { fx = i % FRAME_W; fy = i / FRAME_W; }
                diff++;
            }
        s_sum.frames++;
        if (st.ok) s_sum.map_frames++;
        if (model_ok) s_sum.model_frames++;
        if (diff) s_sum.differ++;
        else s_sum.identical++;
    }
    if (g_picture == PICTURE_HD) {
        view_of(&st, g_hd.wide, g_hd.no_flicker, &v);
        if (v.wide && model_ok) reveal_update(&s_model, &v);
        if (shown || (s_check && v.wide)) {
            draw(f, &st, &v, model_ok, s_pix, &wide_lines);
            if (s_check && v.wide) pop = popins(&v);
            s_w = v.w;
            s_view_x = v.view_x;
            for (int i = 0; i < s_w * FRAME_H; i++) s_rgb[i] = g_nes_rgb[s_pix[i] & 0x3F];
        }
    }
    if (s_check && s_check_log)
        fprintf(s_check_log, "%llu %s %s %d %d %d %d %d %d %d %d %d %d\n", (unsigned long long)g_frame_count,
                st.ok ? "map" : (st.held ? "held" : st.why),
                model_ok ? "model" : (model_seen ? (s_model.bad ? "bad" : "oam") : "none"),
                map_lines, diff, fx, fy, s_ov_frame, g_ram[ROUND], g_ram[SUB_AREA], over,
                model_ok ? s_model.unplaced : 0, pop);
}

void hd_shown(const uint32_t **rgb, const uint8_t **pix, int *w, int *view_x) {
    *rgb = s_rgb;
    *pix = s_pix;
    *w = s_w;
    *view_x = s_view_x;
}

void hd_exit(void) {
    if (!s_check) return;
    fprintf(stderr, "[mff] HD check: %ld pictures, %ld identical to the console picture, %ld differ; "
                    "playfield from the stage map in %ld, sprites from the objects in %ld; "
                    "%ld playfield cells written by the game's VRAM requests\n",
            s_sum.frames, s_sum.identical, s_sum.differ, s_sum.map_frames, s_sum.model_frames, s_ov_writes);
    if (s_check_log) {
        fflush(s_check_log);
        fclose(s_check_log);
        s_check_log = NULL;
    }
    s_check = 0;
}

/* MFF_HDCHECK=FILE: the check log, one line per picture: frame, playfield
 * source ("map" or why not), sprite source ("model", "oam" when the
 * model's OAM is not the frame's, "none" before the first objects_draw),
 * lines drawn from the map, pixels differing, the first one's x y, the
 * overlay cells, round, area, the (line, sprite) pairs the 8 per line
 * leave out, the sprites past a full OAM, and with --wide the objects that
 * appeared in the margins (popins). MFF_WIDE, MFF_NO_FLICKER: the
 * options. */
void hd_init(void) {
    const char *p = getenv("MFF_HDCHECK");
    const char *wide = getenv("MFF_WIDE"), *nf = getenv("MFF_NO_FLICKER");
    if (wide && *wide && hd_option("wide", wide)) mff_fatal("MFF_WIDE: 0, 1 or a width 256-%d", HD_MAX_W);
    if (nf && *nf) hd_option("no_flicker", nf);
    if (p && *p) {
        s_check = 1;
        s_check_log = fopen(p, "w");
        if (!s_check_log) mff_fatal("MFF_HDCHECK: cannot write %s", p);
    }
    overlay_clear();
}
