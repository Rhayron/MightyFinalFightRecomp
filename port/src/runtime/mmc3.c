/*
 * mmc3.c - the cartridge's MMC3: PRG and CHR banking, mirroring, and the
 * scanline counter behind the status bar's raster IRQ
 *
 * $8000 even  bank select: bits 0-2 the register $8001 writes next, bit 6
 *             PRG mode (0: R6 at $8000, second-to-last bank at $C000),
 *             bit 7 CHR mode (0: the 2 KiB banks R0/R1 at $0000)
 * $8001 odd   bank data
 * $A000 even  mirroring: bit 0 set horizontal, clear vertical
 * $A001 odd   cartridge RAM protect (no effect here)
 * $C000 even  IRQ latch; $C001 odd: reload the counter at the next clock
 * $E000 even  IRQ off (and acknowledged); $E001 odd: IRQ on
 *
 * The game keeps PRG mode 0 and CHR mode 0 all the time: R6 at $8000, R7 at
 * $A000, banks $0E/$0F fixed at $C000/$E000, background tiles from R0/R1 and
 * sprite tiles from R2-R5 (docs/assets.md, "CHR").
 *
 * The counter is clocked once per scanline while the background is on
 * (frame.c, the scanline pass): reload from the latch when it is 0 or a
 * reload is pending, otherwise count down; at 0 with the IRQ on it fires.
 */
#include "machine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRG_BANKS_8K (PRG_SIZE / 0x2000)
#define CHR_BANKS_1K (CHR_SIZE / 0x400)

int g_mmc3_win_bank8k[4];

static uint8_t s_select;
static uint8_t s_reg[8];
static uint8_t s_latch, s_counter;
static int s_reload, s_irq_on;
static int s_mirroring;
static uint16_t s_chr[8];               /* 1 KiB bank behind each $400 of pattern space */

static void map_prg(void) {
    int r6 = s_reg[6] % PRG_BANKS_8K, r7 = s_reg[7] % PRG_BANKS_8K;
    int fixed = PRG_BANKS_8K - 2;
    g_mmc3_win_bank8k[0] = (s_select & 0x40) ? fixed : r6;
    g_mmc3_win_bank8k[1] = r7;
    g_mmc3_win_bank8k[2] = (s_select & 0x40) ? r6 : fixed;
    g_mmc3_win_bank8k[3] = PRG_BANKS_8K - 1;
}

static void map_chr(void) {
    int r0 = (s_reg[0] & 0xFE) % CHR_BANKS_1K, r1 = (s_reg[1] & 0xFE) % CHR_BANKS_1K;
    uint16_t big[4] = { (uint16_t)r0, (uint16_t)(r0 + 1), (uint16_t)r1, (uint16_t)(r1 + 1) };
    uint16_t small[4];
    for (int i = 0; i < 4; i++) small[i] = (uint16_t)(s_reg[2 + i] % CHR_BANKS_1K);
    if (s_select & 0x80) {
        memcpy(s_chr, small, sizeof small);
        memcpy(s_chr + 4, big, sizeof big);
    } else {
        memcpy(s_chr, big, sizeof big);
        memcpy(s_chr + 4, small, sizeof small);
    }
}

void mmc3_power_on(int vertical) {
    s_select = 0;
    memset(s_reg, 0, sizeof s_reg);
    s_latch = s_counter = 0;
    s_reload = s_irq_on = 0;
    s_mirroring = vertical ? MIRROR_VERTICAL : MIRROR_HORIZONTAL;
    map_prg();
    map_chr();
}

void mmc3_write(uint16_t a, uint8_t v) {
    switch (a & 0xE001) {
    case 0x8000: {
        uint8_t changed = (uint8_t)(s_select ^ v);
        s_select = v;
        if (changed & 0x40) map_prg();
        if (changed & 0x80) map_chr();
        break;
    }
    case 0x8001: {
        int r = s_select & 7;
        s_reg[r] = v;
        if (r <= 5) map_chr(); else map_prg();
        break;
    }
    case 0xA000: s_mirroring = (v & 1) ? MIRROR_HORIZONTAL : MIRROR_VERTICAL; break;
    case 0xA001: break;
    case 0xC000: s_latch = v; break;
    case 0xC001: s_reload = 1; s_counter = 0; break;
    case 0xE000: s_irq_on = 0; break;
    case 0xE001: s_irq_on = 1; break;
    }
}

/* MFF_PRG_READS=FILE: every PRG byte the game reads, with where it first
 * read it (port/tools/prg_reads.py compares that with the code list) */
static struct { uint16_t pc; uint32_t frame; } *s_reads;
static int s_reads_on = -1;

static void note_read(uint32_t off) {
    if (s_reads_on < 0) {
        const char *e = getenv("MFF_PRG_READS");
        s_reads_on = e && *e;
        if (s_reads_on) s_reads = calloc(PRG_SIZE, sizeof *s_reads);
        if (!s_reads) s_reads_on = 0;
    }
    if (s_reads_on && !s_reads[off].frame) {
        s_reads[off].pc = timing_pc();
        s_reads[off].frame = (uint32_t)g_frame_count + 1;
    }
}

void mmc3_write_prg_reads(void) {
    const char *path = getenv("MFF_PRG_READS");
    FILE *f;
    if (s_reads_on <= 0 || !path || !(f = fopen(path, "w"))) return;
    fprintf(f, "# PRG bytes the game read: bank (8 KiB), offset, the run of instructions that\n"
               "# first read it, the frame\n");
    for (uint32_t off = 0; off < PRG_SIZE; off++)
        if (s_reads[off].frame)
            fprintf(f, "%02X %04X %04X %u\n", off >> 13, off & 0x1FFF, s_reads[off].pc,
                    s_reads[off].frame - 1);
    fclose(f);
}

uint8_t mmc3_prg_read(uint16_t a) {
    int bank = g_mmc3_win_bank8k[(a >> 13) & 3];
    uint32_t off = (uint32_t)bank * 0x2000 + (a & 0x1FFF);
    if (s_reads_on) note_read(off);
    if (!cart_has_prg(off)) cart_missing_prg(bank, a);
    return g_cart.prg[off];
}

int mmc3_clock_scanline(void) {
    if (s_reload || s_counter == 0) {
        s_counter = s_latch;
        s_reload = 0;
    } else {
        s_counter--;
    }
    return s_counter == 0 && s_irq_on;
}

int mmc3_mirroring(void) { return s_mirroring; }

void mmc3_chr_map(uint16_t banks[8]) { memcpy(banks, s_chr, sizeof s_chr); }

uint8_t mmc3_chr_read(uint16_t a) {
    a &= 0x1FFF;
    return g_cart.chr[(uint32_t)s_chr[a >> 10] * 0x400 + (a & 0x3FF)];
}
