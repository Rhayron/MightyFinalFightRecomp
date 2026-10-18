/*
 * ppu.c - the PPU as the game drives it: registers $2000-$2007, nametable
 * and palette RAM, OAM, and the state a picture is drawn from
 *
 * Address and scroll follow the PPU's two 15-bit registers: t, written by
 * $2000/$2005/$2006, and v, the VRAM address $2007 uses (set from t by the
 * second $2006 write), with one write toggle w shared by $2005 and $2006 and
 * cleared by a $2002 read. Bits of t/v: yyy NN YYYYY XXXXX (fine Y,
 * nametable, coarse Y, coarse X); fine X is kept apart.
 *
 * The picture is composed once per frame (frame.c, render_frame.c), so the
 * scroll it uses is kept as plain X/Y bytes: the $2005 writes, zeroed at the
 * frame boundary, taken again from t when the picture starts if a $2005
 * pair was the last thing written, and from v after a raster IRQ that wrote
 * $2006. This is the scaffold renderer's model; the game sets its scroll
 * with $2005 at both places, where it gives the hardware's result.
 *
 * $2002 also carries the scaffold's stand-in for sprite-0 hit polling:
 * three reads in a row with no other PPU write in between set bit 6 (and
 * mark a sprite-0 split if sprite 0 would hit), the next read clears it.
 * The game never waits on sprite 0, but its $2002 reads see these bits.
 */
#include "machine.h"
#include "render.h"
#include <string.h>

static struct {
    uint8_t ctrl, mask, status, oam_addr;
    uint8_t sx, sy;                     /* the scroll the picture takes */
    uint8_t hud_x, hud_y, hud_ctrl;     /* the scroll above a sprite-0 split */
    int split, status_reads;
    uint16_t t, v, v_set;               /* v as the last $2006 pair set it */
    uint8_t fine_x;
    int w;
    uint32_t v_writes;                  /* completed $2006 pairs */
    int scroll_pair_last;               /* a $2005 pair came after the last $2006 pair */
    uint8_t read_buf, io_latch;
    uint32_t vram_writes;               /* nametable bytes written */
    uint8_t oam[256];
    uint8_t pal[32];
    uint8_t nt[0x1000];
} P;

static uint32_t s_copied_at;            /* vram_writes when the frame's NT copy was taken */

/* Power-on. Nametable RAM holds what the oracle's PPU starts with (nesrecomp
 * runner/cyc/hw_ppu.c ppu_power_on: runs of $F0 and $0F; the hardware does
 * not define it). The game never writes the second nametable in some areas,
 * and it shows: in round 4's elevator the screen shakes a line up and down,
 * and on the frames the picture starts a line low the last line wraps into
 * the other nametable row, power-on tiles drawn with the status bar's banks.
 * (The oracle's power-on palette is left out: the game writes the palette
 * before anything is drawn.) */
void ppu_power_on(void) {
    memset(&P, 0, sizeof P);
    for (int i = 0; i < (int)sizeof P.nt; i++) {
        int bit1_clear = (i & 2) == 0, upper = (i & 0x1F) >= 0x10;
        P.nt[i] = bit1_clear != upper ? 0xF0 : 0x0F;
    }
}

/* ---- memory ---- */

/* palette entries $10/$14/$18/$1C are $00/$04/$08/$0C */
static int pal_index(uint16_t a) {
    int i = a & 0x1F;
    if ((i & 0x13) == 0x10) i &= 0x0F;
    return i;
}

static int nt_page(int mirroring, int virt) {
    switch (mirroring) {
    case MIRROR_ONE_LOW: return 0;
    case MIRROR_ONE_HIGH: return 1;
    case MIRROR_HORIZONTAL: return virt >> 1;
    default: return virt & 1;
    }
}

static int nt_index(uint16_t a) {
    int virt = ((a - 0x2000) >> 10) & 3;
    return nt_page(mmc3_mirroring(), virt) * 0x400 + (a & 0x3FF);
}

void ppu_oam_write(int index, uint8_t v) { P.oam[index & 0xFF] = v; }
const uint8_t *ppu_oam(void) { return P.oam; }
const uint8_t *ppu_palette(void) { return P.pal; }
uint8_t ppu_ctrl(void) { return P.ctrl; }
uint8_t ppu_mask(void) { return P.mask; }

void ppu_set_status(uint8_t set, uint8_t clear) {
    P.status = (uint8_t)((P.status & ~clear) | set);
}

/* ---- sprite 0 ---- */

/* the background colour index (0-3) at a screen pixel, scrolled as the
 * picture would be now */
static int bg_pixel(int x, int y) {
    int base = (P.ctrl & 0x10) ? 0x1000 : 0;
    int nx = (P.sx + ((P.ctrl & 1) ? 256 : 0) + x) & 0x1FF;
    int ny = P.sy + ((P.ctrl & 2) ? 240 : 0) + y;
    int tx, ty, page;
    uint8_t tile;
    uint16_t a;
    if (x < 0 || x >= 256 || y < 0 || y >= 240) return 0;
    if (ny >= 480) ny -= 480;
    tx = nx >> 3;
    ty = ny >> 3;
    page = nt_page(mmc3_mirroring(), (ty >= 30 ? 2 : 0) + (tx >= 32 ? 1 : 0));
    tile = P.nt[(page * 0x400 + (ty % 30) * 32 + (tx & 31)) & 0xFFF];
    a = (uint16_t)(base + tile * 16 + (ny & 7));
    return ((mmc3_chr_read(a) >> (7 - (nx & 7))) & 1) |
           (((mmc3_chr_read((uint16_t)(a + 8)) >> (7 - (nx & 7))) & 1) << 1);
}

/* the first scanline where sprite 0 would hit the background now, 240 if
 * it would not */
static int spr0_hit_line(void) {
    int y = P.oam[0], tile = P.oam[1], attr = P.oam[2], x = P.oam[3];
    int tall = (P.ctrl & 0x20) != 0, h = tall ? 16 : 8;
    int base = tall ? ((tile & 1) ? 0x1000 : 0) : ((P.ctrl & 0x08) ? 0x1000 : 0);
    int first = tall ? (tile & 0xFE) : tile;
    if ((P.mask & 0x18) != 0x18 || y >= 0xEF) return 240;
    for (int row = 0; row < h; row++) {
        int r = (attr & 0x80) ? h - 1 - row : row, t = first, py = y + 1 + row;
        uint8_t lo, hi;
        if (py >= 240) continue;
        if (r >= 8) { t++; r -= 8; }
        lo = mmc3_chr_read((uint16_t)(base + t * 16 + r));
        hi = mmc3_chr_read((uint16_t)(base + t * 16 + r + 8));
        for (int i = 0; i < 8; i++) {
            int bit = (attr & 0x40) ? i : 7 - i, px = x + i;
            if (px >= 255) continue;            /* off screen, or no hit at x = 255 */
            if (!(((lo >> bit) & 1) | (((hi >> bit) & 1) << 1))) continue;
            if (px < 8 && (!(P.mask & 0x04) || !(P.mask & 0x02))) continue;
            if (bg_pixel(px, py)) return py;
        }
    }
    return 240;
}

/* ---- registers ---- */

uint8_t ppu_read(uint16_t reg) {
    switch (reg) {
    case 0x2002: {
        uint8_t s;
        if (P.status & 0x40) {
            P.status &= (uint8_t)~0x40;
            P.status_reads = 0;
            P.split = 1;
        } else if (++P.status_reads >= 3) {
            P.hud_x = P.sx;
            P.hud_y = P.sy;
            P.hud_ctrl = P.ctrl & 0x38;
            P.status |= 0x40;
            P.status_reads = 0;
            if (spr0_hit_line() < 240) P.split = 1;
        }
        s = (uint8_t)((P.status & 0xE0) | (P.io_latch & 0x1F));
        P.status &= (uint8_t)~0x80;
        P.w = 0;
        return s;
    }
    case 0x2004:
        return P.io_latch = P.oam[P.oam_addr];
    case 0x2007: {
        uint16_t a = P.v & 0x3FFF;
        uint8_t r;
        P.v = (uint16_t)(P.v + ((P.ctrl & 0x04) ? 32 : 1));
        if (a >= 0x3F00) {                  /* palette: no delay, the buffer gets the NT under it */
            P.read_buf = P.nt[nt_index(a)];
            return P.io_latch = P.pal[pal_index(a)];
        }
        r = P.read_buf;
        P.read_buf = a >= 0x2000 ? P.nt[nt_index(a)] : mmc3_chr_read(a);
        return P.io_latch = r;
    }
    }
    return 0;
}

void ppu_write(uint16_t reg, uint8_t v) {
    P.io_latch = v;
    P.status_reads = 0;
    switch (reg) {
    case 0x2000:
        P.ctrl = v;
        P.t = (uint16_t)((P.t & 0xF3FF) | ((v & 3) << 10));
        break;
    case 0x2001:
        P.mask = v;
        break;
    case 0x2003:
        P.oam_addr = v;
        break;
    case 0x2004:
        P.oam[P.oam_addr++] = v;
        break;
    case 0x2005:
        if (!P.w) {
            P.t = (uint16_t)((P.t & 0xFFE0) | (v >> 3));
            P.fine_x = v & 7;
            P.sx = v;
        } else {
            P.t = (uint16_t)((P.t & 0x0C1F) | ((v & 7) << 12) | ((v >> 3) << 5));
            P.sy = v;
            P.scroll_pair_last = 1;
        }
        P.w ^= 1;
        break;
    case 0x2006:
        if (!P.w) {
            P.t = (uint16_t)((P.t & 0x00FF) | ((v & 0x3F) << 8));
            P.v = (uint16_t)(v << 8);
        } else {
            P.t = (uint16_t)((P.t & 0xFF00) | v);
            P.v = P.t & 0x3FFF;
            P.v_set = P.v;
            P.v_writes++;
            P.scroll_pair_last = 0;
        }
        P.w ^= 1;
        P.read_buf = 0;
        break;
    case 0x2007: {
        uint16_t a = P.v & 0x3FFF;
        if (a >= 0x3F00) {
            P.pal[pal_index(a)] = v;
        } else if (a >= 0x2000) {
            P.nt[nt_index(a)] = v;
            P.vram_writes++;
        }                                   /* pattern tables are ROM */
        P.v = (uint16_t)(P.v + ((P.ctrl & 0x04) ? 32 : 1));
        break;
    }
    }
}

/* ---- frame boundary ---- */

void ppu_vblank_begin(void) {
    P.status = (uint8_t)((P.status & ~0x40) | 0x80);
    P.split = 0;
    P.status_reads = 0;
    P.sx = P.sy = 0;
    P.hud_x = P.hud_y = 0;
    P.hud_ctrl = P.ctrl & 0x38;
}

void ppu_frame_status(void) {
    P.status &= (uint8_t)~0x60;
    P.status |= 0x80;
}

/* ---- the picture ---- */

int ppu_split_active(void) { return P.split; }
uint32_t ppu_v_writes(void) { return P.v_writes; }

/* the start of the background: v from t, and the scroll from t when a
 * $2005 pair was written last */
void ppu_render_sync(void) {
    P.v = P.t & 0x3FFF;
    if (P.scroll_pair_last) {
        P.sx = (uint8_t)(((P.t & 0x1F) << 3) | (P.fine_x & 7));
        P.sy = (uint8_t)((((P.t >> 5) & 0x1F) << 3) | ((P.t >> 12) & 7));
    }
}

/* after a raster IRQ that wrote a $2006 pair: the scroll and nametable it
 * set */
void ppu_sync_from_v(void) {
    uint16_t v = P.v_set;
    P.sx = (uint8_t)(((v & 0x1F) << 3) | (P.fine_x & 7));
    P.sy = (uint8_t)((((v >> 5) & 0x1F) << 3) | ((v >> 12) & 7));
    P.ctrl = (uint8_t)((P.ctrl & 0xFC) | ((v >> 10) & 3));
}

void ppu_capture_start(FrameRecord *f) {
    f->mask0 = P.mask;
    f->backdrop = P.pal[0];
    memcpy(f->oam, P.oam, sizeof f->oam);
    mmc3_chr_map(f->chr0);
    memcpy(f->nt_copy[0], P.nt, sizeof P.nt);
    f->ncopies = 1;
    s_copied_at = P.vram_writes;
}

void ppu_capture_region(FrameRecord *f, int first_line, int v_set) {
    PpuRegion *r;
    if (P.vram_writes != s_copied_at) {     /* a raster IRQ wrote VRAM: a copy of its own */
        if (f->ncopies == MAX_NT_COPIES)
            mff_fatal("more than %d VRAM changes inside one picture", MAX_NT_COPIES - 1);
        memcpy(f->nt_copy[f->ncopies++], P.nt, sizeof P.nt);
        s_copied_at = P.vram_writes;
    }
    if (f->nregions == MAX_REGIONS) mff_fatal("too many raster splits in one picture");
    r = &f->region[f->nregions++];
    r->first_line = first_line;
    r->ctrl = P.ctrl;
    r->mask = P.mask;
    r->sx = P.sx;
    r->sy = P.sy;
    r->hud_ctrl = P.hud_ctrl;
    r->hud_x = P.hud_x;
    r->hud_y = P.hud_y;
    r->t = P.t;
    r->fine_x = P.fine_x;
    r->v_set = v_set;
    r->v = P.v_set;
    memcpy(r->pal, P.pal, sizeof r->pal);
    mmc3_chr_map(r->chr);
    r->nt = f->nt_copy[f->ncopies - 1];
}

void ppu_capture_end(FrameRecord *f) {
    f->ctrl_end = P.ctrl;
    f->mask_end = P.mask;
    memcpy(f->pal_end, P.pal, sizeof f->pal_end);
    mmc3_chr_map(f->chr_end);
}

/* save states (savestate.c) */
void ppu_state(StateIO *io) {
    st_u8(io, &P.ctrl); st_u8(io, &P.mask); st_u8(io, &P.status); st_u8(io, &P.oam_addr);
    st_u8(io, &P.sx); st_u8(io, &P.sy);
    st_u8(io, &P.hud_x); st_u8(io, &P.hud_y); st_u8(io, &P.hud_ctrl);
    st_int(io, &P.split); st_int(io, &P.status_reads);
    st_u16(io, &P.t); st_u16(io, &P.v); st_u16(io, &P.v_set);
    st_u8(io, &P.fine_x);
    st_int(io, &P.w);
    st_u32(io, &P.v_writes);
    st_int(io, &P.scroll_pair_last);
    st_u8(io, &P.read_buf); st_u8(io, &P.io_latch);
    st_u32(io, &P.vram_writes);
    st_bytes(io, P.oam, sizeof P.oam);
    st_bytes(io, P.pal, sizeof P.pal);
    st_bytes(io, P.nt, sizeof P.nt);
    st_u32(io, &s_copied_at);
}
