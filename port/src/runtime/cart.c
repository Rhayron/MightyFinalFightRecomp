/*
 * cart.c - the cartridge's PRG and CHR data, from build/assets
 *
 * The port never reads the ROM. port/tools/extract_assets.py (part
 * "runtime", run by the build with -DMFF_ROM=dump) writes:
 *   chr.bin  the CHR ROM, 128 banks of 1 KiB
 *   prg.bin  the PRG ROM without the bytes the P0 map classes as code
 *            (port/tools/prg_code.txt; format in port/tools/assets_runtime.py)
 *
 * PRG is kept as the 16 banks of 8 KiB the MMC3 maps, with a bit per byte
 * saying whether the data holds it. The routines read PRG only for their
 * tables (prg_read, mem_read through a pointer); reading a byte that is not
 * there stops the run and names it.
 */
#include "machine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Cart g_cart;

/* Headless (the tests) this stops the run. In a window the game goes on
 * reading 0 there, which may make it differ from the console from then on;
 * either way the message names the byte, for port/tools/prg_code_read.txt. */
void cart_missing_prg(int bank8k, uint16_t addr) {
    static int warned;
    const char *what = "the game read PRG byte %02X:%04X (bank $%02X at $%04X; frame %llu, in the "
                       "run of instructions at $%04X), which %s leaves out as code";
    if (!g_window)
        mff_fatal(what, bank8k, addr & 0x1FFF, bank8k, addr, (unsigned long long)g_frame_count,
                  timing_pc(), g_cart.source);
    if (warned++ < 8) {
        fprintf(stderr, "[mff] ");
        fprintf(stderr, what, bank8k, addr & 0x1FFF, bank8k, addr, (unsigned long long)g_frame_count,
                timing_pc(), g_cart.source);
        fprintf(stderr, "; it reads 0 instead\n");
    }
}

static uint8_t *read_file(const char *path, size_t *size) {
    FILE *f = fopen(path, "rb");
    uint8_t *buf;
    long n;
    if (!f) return NULL;
    if (fseek(f, 0, SEEK_END) || (n = ftell(f)) < 0 || fseek(f, 0, SEEK_SET)) {
        fclose(f);
        return NULL;
    }
    buf = (uint8_t *)malloc(n ? (size_t)n : 1);
    if (!buf || fread(buf, 1, (size_t)n, f) != (size_t)n) {
        free(buf);
        fclose(f);
        return NULL;
    }
    fclose(f);
    *size = (size_t)n;
    return buf;
}

static uint32_t le32(const uint8_t *p) {
    return (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24;
}

/* 0 on success, -1 if the files are not there; stops on a damaged file */
int cart_load_assets(const char *dir) {
    char path[1024];
    size_t n = 0, at = 16, held = 0;
    uint8_t *chr, *prg;
    uint32_t ranges;

    snprintf(path, sizeof path, "%s/chr.bin", dir);
    chr = read_file(path, &n);
    if (!chr) return -1;
    if (n != CHR_SIZE) mff_fatal("%s: %zu bytes, expected %d: extract the data again", path, n, CHR_SIZE);
    memcpy(g_cart.chr, chr, CHR_SIZE);
    free(chr);

    snprintf(path, sizeof path, "%s/prg.bin", dir);
    prg = read_file(path, &n);
    if (!prg) return -1;
    if (n < 16 || memcmp(prg, "MFFPRG", 6) != 0)
        mff_fatal("%s is not the port's PRG data: extract the data again", path);
    if (memcmp(prg + 6, "01", 2) != 0)
        mff_fatal("%s is format %.2s, this build reads 01: extract the data again", path, prg + 6);
    g_cart.vertical_mirroring = prg[8] & 1;
    ranges = le32(prg + 12);
    memset(g_cart.prg, 0, sizeof g_cart.prg);
    memset(g_cart.prg_have, 0, sizeof g_cart.prg_have);
    for (uint32_t r = 0; r < ranges; r++) {
        uint32_t off, len;
        if (at + 8 > n) mff_fatal("%s is cut short: extract the data again", path);
        off = le32(prg + at);
        len = le32(prg + at + 4);
        at += 8;
        if (off > PRG_SIZE || len > PRG_SIZE - off || at + len > n)
            mff_fatal("%s: bad range %u+%u: extract the data again", path, off, len);
        memcpy(g_cart.prg + off, prg + at, len);
        for (uint32_t i = off; i < off + len; i++) g_cart.prg_have[i >> 3] |= (uint8_t)(1 << (i & 7));
        at += len;
        held += len;
    }
    free(prg);
    snprintf(g_cart.source, sizeof g_cart.source, "the extracted data in %s (%zu of %d PRG bytes)",
             dir, held, PRG_SIZE);
    return 0;
}
