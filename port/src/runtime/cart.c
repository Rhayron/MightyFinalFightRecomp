/*
 * cart.c - the cartridge's PRG and CHR data
 *
 * PRG is kept as the 16 banks of 8 KiB the MMC3 maps, with a bit per byte
 * saying whether the data holds it. The code reads PRG only for tables
 * (prg_read, mem_read through a pointer), so data that leaves out the code
 * still runs the game; reading a byte that is not there stops the run and
 * names it.
 */
#include "machine.h"
#include <stdio.h>
#include <string.h>

Cart g_cart;

void cart_missing_prg(int bank8k, uint16_t addr) {
    mff_fatal("the game read PRG byte %02X:%04X (bank $%02X at $%04X), which the extracted data "
              "leaves out as code; frame %llu. %s", bank8k, addr & 0x1FFF, bank8k, addr,
              (unsigned long long)g_frame_count, g_cart.source);
}

int cart_load_dump(const char *path) {
    uint8_t h[16];
    size_t skip;
    FILE *f = fopen(path, "rb");
    if (!f) return -1;
    if (fread(h, 1, 16, f) != 16 || memcmp(h, "NES\x1A", 4) != 0) {
        fclose(f);
        mff_fatal("%s is not an iNES file", path);
    }
    if (h[4] * 0x4000 != PRG_SIZE || h[5] * 0x2000 != CHR_SIZE || ((h[6] >> 4) | (h[7] & 0xF0)) != 4) {
        fclose(f);
        mff_fatal("%s: not Mighty Final Fight (USA): expected mapper 4, 128 KiB PRG, 128 KiB CHR", path);
    }
    skip = (h[6] & 4) ? 512 : 0;
    if (skip) fseek(f, (long)skip, SEEK_CUR);
    if (fread(g_cart.prg, 1, PRG_SIZE, f) != PRG_SIZE || fread(g_cart.chr, 1, CHR_SIZE, f) != CHR_SIZE) {
        fclose(f);
        mff_fatal("%s: file too short", path);
    }
    fclose(f);
    memset(g_cart.prg_have, 0xFF, sizeof g_cart.prg_have);
    g_cart.vertical_mirroring = h[6] & 1;
    snprintf(g_cart.source, sizeof g_cart.source, "Read from the dump %s.", path);
    return 0;
}
