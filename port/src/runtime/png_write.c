/*
 * png_write.c - screenshots as PNG, standard library only (stored deflate
 * blocks: larger files, no compressor needed)
 */
#include "machine.h"
#include <stdio.h>
#include <stdlib.h>

static uint32_t crc_table[256];

static uint32_t crc32_update(uint32_t c, const uint8_t *p, size_t n) {
    if (!crc_table[1])
        for (uint32_t i = 0; i < 256; i++) {
            uint32_t v = i;
            for (int k = 0; k < 8; k++) v = (v & 1) ? 0xEDB88320u ^ (v >> 1) : v >> 1;
            crc_table[i] = v;
        }
    c = ~c;
    while (n--) c = crc_table[(c ^ *p++) & 0xFF] ^ (c >> 8);
    return ~c;
}

static void put32(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)(v >> 24); p[1] = (uint8_t)(v >> 16); p[2] = (uint8_t)(v >> 8); p[3] = (uint8_t)v;
}

static void chunk(FILE *f, const char *type, const uint8_t *data, uint32_t n) {
    uint8_t hdr[8];
    uint32_t c;
    put32(hdr, n);
    for (int i = 0; i < 4; i++) hdr[4 + i] = (uint8_t)type[i];
    fwrite(hdr, 1, 8, f);
    if (n) fwrite(data, 1, n, f);
    c = crc32_update(crc32_update(0, hdr + 4, 4), data, n);
    put32(hdr, c);
    fwrite(hdr, 1, 4, f);
}

/* ARGB pixels, w x h, to an RGB PNG; 0 on success */
int png_write_argb(const char *path, const uint32_t *px, int w, int h) {
    static const uint8_t sig[8] = { 0x89, 'P', 'N', 'G', '\r', '\n', 0x1A, '\n' };
    size_t raw_n = (size_t)h * (1 + 3 * (size_t)w), blocks = (raw_n + 65534) / 65535;
    size_t z_n = 2 + raw_n + 5 * blocks + 4, at = 0;
    uint8_t ihdr[13], *raw = malloc(raw_n), *z = malloc(z_n);
    uint32_t a = 1, b = 0;
    FILE *f;
    if (!raw || !z) { free(raw); free(z); return -1; }
    for (int y = 0; y < h; y++) {
        uint8_t *row = raw + (size_t)y * (1 + 3 * (size_t)w);
        row[0] = 0;
        for (int x = 0; x < w; x++) {
            uint32_t c = px[(size_t)y * w + x];
            row[1 + 3 * x] = (uint8_t)(c >> 16);
            row[2 + 3 * x] = (uint8_t)(c >> 8);
            row[3 + 3 * x] = (uint8_t)c;
        }
    }
    z[at++] = 0x78;
    z[at++] = 0x01;
    for (size_t done = 0; done < raw_n;) {
        size_t n = raw_n - done > 65535 ? 65535 : raw_n - done;
        z[at++] = (uint8_t)(done + n == raw_n);
        z[at++] = (uint8_t)n; z[at++] = (uint8_t)(n >> 8);
        z[at++] = (uint8_t)~n; z[at++] = (uint8_t)(~n >> 8);
        for (size_t i = 0; i < n; i++) {
            uint8_t v = raw[done + i];
            z[at++] = v;
            a = (a + v) % 65521;
            b = (b + a) % 65521;
        }
        done += n;
    }
    put32(z + at, (b << 16) | a);
    at += 4;
    put32(ihdr, (uint32_t)w);
    put32(ihdr + 4, (uint32_t)h);
    ihdr[8] = 8; ihdr[9] = 2; ihdr[10] = ihdr[11] = ihdr[12] = 0;
    f = fopen(path, "wb");
    if (!f) { free(raw); free(z); return -1; }
    fwrite(sig, 1, 8, f);
    chunk(f, "IHDR", ihdr, 13);
    chunk(f, "IDAT", z, (uint32_t)at);
    chunk(f, "IEND", NULL, 0);
    fclose(f);
    free(raw);
    free(z);
    return 0;
}
