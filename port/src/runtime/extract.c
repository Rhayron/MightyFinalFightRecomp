/*
 * extract.c - the game's data from the player's dump, without Python
 *
 * The same files as port/tools/extract_assets.py DUMP --only runtime
 * (assets_runtime.py, generator "extract_assets.py 2"), byte for byte:
 *   chr.bin        the CHR ROM, 128 KiB
 *   prg.bin        the PRG ROM without the bytes the P0 map classes as code
 *                  (port/tools/prg_code.txt) but the ones the game also
 *                  reads as data (the screens each area's list names past
 *                  its end, worked out from the dump, and
 *                  port/tools/prg_code_read.txt); format in cart.c
 *   manifest.json  what was written, the dump's CRC32, the PRG summary
 * The two code lists are offsets only; CMake compiles them in
 * (mff_prg_code.h, made from the .txt files when it configures).
 * port/tests/extract_check.sh runs both extractors and compares.
 *
 * The dump must be Mighty Final Fight (USA): iNES, mapper 4, 128 KiB PRG
 * and CHR, CRC32 15EB0BEE over the whole file, header included.
 */
#include "machine.h"
#include "mff_prg_code.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#define ROM_CRC32 0x15EB0BEEu
#define GENERATOR "extract_assets.py 2"
/* assets_stage.py: where the area records and screens are */
#define ROUND_BANK_ADDR 0xC1F8          /* bank 0E, 5 bytes */
#define ROUNDS 5
#define AREAS 3
#define AREA_BASE 0xAE60
#define AREA_STRIDE 0x30
#define SCREEN_BASE 0xAF00

static char *s_err;
static size_t s_errcap;

static int fail(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(s_err, s_errcap, fmt, ap);
    va_end(ap);
    return -1;
}

uint32_t mff_crc32(const uint8_t *p, size_t n) {
    static uint32_t table[256];
    uint32_t c = 0xFFFFFFFFu;
    if (!table[1]) {
        for (uint32_t i = 0; i < 256; i++) {
            uint32_t k = i;
            for (int j = 0; j < 8; j++) k = (k & 1) ? 0xEDB88320u ^ (k >> 1) : k >> 1;
            table[i] = k;
        }
    }
    for (size_t i = 0; i < n; i++) c = table[(c ^ p[i]) & 0xFF] ^ (c >> 8);
    return c ^ 0xFFFFFFFFu;
}

static uint8_t *read_all(const char *path, size_t *size) {
    FILE *f = fopen(path, "rb");
    uint8_t *buf;
    long n;
    if (!f) return NULL;
    if (fseek(f, 0, SEEK_END) || (n = ftell(f)) < 0 || fseek(f, 0, SEEK_SET) || n > (64L << 20)) {
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

/* 1 if `path` is Mighty Final Fight (USA): CRC32 15EB0BEE */
int mff_is_dump(const char *path) {
    size_t size;
    uint8_t *rom = read_all(path, &size);
    int ok = rom && size >= 16 && !memcmp(rom, "NES\x1a", 4) && mff_crc32(rom, size) == ROM_CRC32;
    free(rom);
    return ok;
}

/* the CPU window an 8 KiB bank is mapped at (assets_nes.bank_window) */
static uint16_t bank_window(int bank) {
    if (bank == 0x0E) return 0xC000;
    if (bank == 0x0F) return 0xE000;
    return (bank & 1) ? 0xA000 : 0x8000;
}

/* rom.byte(bank, addr): -1 if the bank is not mapped there */
static int rom_byte(const uint8_t *prg, int bank, uint16_t addr) {
    if (addr < 0x8000 || (addr & 0xE000) != bank_window(bank) || bank >= PRG_SIZE / 0x2000) return -1;
    return prg[bank * 0x2000 + (addr & 0x1FFF)];
}

/* mask[i] = 1 for every byte a code list names; -1 on a bad range */
static int mark_ranges(uint8_t *mask, const uint32_t (*r)[3], uint8_t value, uint32_t *count) {
    for (; r[0][0] != MFF_RANGES_END; r++) {
        uint32_t bank = r[0][0], off = r[0][1], n = r[0][2];
        if (bank >= PRG_SIZE / 0x2000 || off + n > 0x2000)
            return fail("the compiled-in code list has a bad range %02X %04X %04X", bank, off, n);
        for (uint32_t i = 0; i < n; i++) {
            uint32_t at = bank * 0x2000 + off + i;
            if (count && !mask[at]) (*count)++;
            mask[at] = value;
        }
    }
    return 0;
}

/* assets_runtime.past_end_screens: the 64-byte screen each area's list
 * names after its last one, which the column writer reads when the camera
 * stops; in five areas it points into the fixed banks' code */
static int keep_past_end_screens(const uint8_t *prg, uint8_t *mask) {
    for (int rnd = 0; rnd < ROUNDS; rnd++) {
        int bank = rom_byte(prg, 0x0E, (uint16_t)(ROUND_BANK_ADDR + rnd));
        for (int area = 0; area < AREAS; area++) {
            uint16_t base = (uint16_t)(AREA_BASE + AREA_STRIDE * area);
            int hdr = rom_byte(prg, bank, (uint16_t)(base + 8)), last, s;
            uint32_t screen;
            if (hdr < 0) return fail("round %d's bank %02X is not mapped at $A000", rnd, bank);
            last = hdr & 0x1F;
            s = rom_byte(prg, bank, (uint16_t)(base + last + 1));
            screen = SCREEN_BASE + 64u * (uint32_t)s;
            for (uint32_t i = 0; i < 64; i++) {
                uint32_t a = screen + i;
                int b = a < 0xC000 ? bank : (a < 0xE000 ? 0x0E : 0x0F);
                mask[b * 0x2000 + (a & 0x1FFF)] = 0;
            }
        }
    }
    return 0;
}

static void put32(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)v; p[1] = (uint8_t)(v >> 8); p[2] = (uint8_t)(v >> 16); p[3] = (uint8_t)(v >> 24);
}

static int write_file(const char *dir, const char *name, const void *data, size_t n) {
    char path[1200];
    FILE *f;
    snprintf(path, sizeof path, "%s/%s", dir, name);
    f = fopen(path, "wb");
    if (!f) return fail("cannot write %s", path);
    if (fwrite(data, 1, n, f) != n) {
        fclose(f);
        return fail("cannot write %s", path);
    }
    if (fclose(f)) return fail("cannot write %s", path);
    return 0;
}

int mff_make_dirs(const char *path) {
    char tmp[1024];
    size_t n = strlen(path);
    if (n == 0 || n >= sizeof tmp) return -1;
    memcpy(tmp, path, n + 1);
    for (size_t i = 1; i <= n; i++) {
        if (tmp[i] == '/' || tmp[i] == '\\' || tmp[i] == 0) {
            char c = tmp[i];
            if (i > 0 && tmp[i - 1] == ':') continue;          /* C: */
            tmp[i] = 0;
#ifdef _WIN32
            _mkdir(tmp);
#else
            mkdir(tmp, 0755);
#endif
            tmp[i] = c;
        }
    }
    return 0;
}

/* ---- manifest.json, as json.dump(obj, indent=1) writes it ---- */

#define MAX_FILES 4096

static int cmp_str(const void *a, const void *b) {
    return strcmp(*(char *const *)a, *(char *const *)b);
}

static int file_exists(const char *dir, const char *name) {
    char path[1200];
    FILE *f;
    snprintf(path, sizeof path, "%s/%s", dir, name);
    f = fopen(path, "rb");
    if (f) fclose(f);
    return f != NULL;
}

/* A partial run keeps the files an earlier run of the same dump listed
 * (extract_assets.py does the same with --only): the "files" of an old
 * manifest with this CRC that still exist. Plain JSON strings only. */
static int old_files(const char *dir, const char *crc, char **out, int n) {
    char path[1200], *text, *p, *end;
    size_t size;
    snprintf(path, sizeof path, "%s/manifest.json", dir);
    text = (char *)read_all(path, &size);
    if (!text) return n;
    text = (char *)realloc(text, size + 1);
    if (!text) return n;
    text[size] = 0;
    p = strstr(text, "\"rom_crc32\": \"");
    if (!p || strncmp(p + 14, crc, 8) != 0 || p[22] != '"') goto done;
    p = strstr(text, "\"files\": [");
    if (!p) goto done;
    p += 10;
    end = strchr(p, ']');
    while (end && p < end && n < MAX_FILES) {
        char *q = strchr(p, '"'), *e;
        if (!q || q > end) break;
        e = strchr(q + 1, '"');
        if (!e || e > end) break;
        *e = 0;
        if (!strchr(q + 1, '\\') && file_exists(dir, q + 1)) {
            int dup = 0;
            for (int i = 0; i < n; i++) dup |= !strcmp(out[i], q + 1);
            if (!dup) {
                out[n] = (char *)malloc(strlen(q + 1) + 1);
                if (out[n]) strcpy(out[n++], q + 1);
            }
        }
        p = e + 1;
    }
done:
    free(text);
    return n;
}

static int write_manifest(const char *dir, uint32_t crc, uint32_t kept, uint32_t code_bytes) {
    static char *files[MAX_FILES], *old[MAX_FILES];
    static char chr_name[] = "chr.bin", prg_name[] = "prg.bin";
    char crc_hex[9], *json;
    int n = 0, nold, r;
    size_t cap = 4096, len = 0;
    snprintf(crc_hex, sizeof crc_hex, "%08X", crc);
    files[n++] = chr_name;
    files[n++] = prg_name;
    nold = old_files(dir, crc_hex, old, 0);
    for (int i = 0; i < nold; i++)
        if (strcmp(old[i], chr_name) && strcmp(old[i], prg_name) && n < MAX_FILES) files[n++] = old[i];
    qsort(files, (size_t)n, sizeof files[0], cmp_str);
    for (int i = 0; i < n; i++) cap += strlen(files[i]) + 8;
    json = (char *)malloc(cap);
    if (!json) {
        for (int i = 0; i < nold; i++) free(old[i]);
        return fail("no memory");
    }
#define OUT(...) (len += (size_t)snprintf(json + len, cap - len, __VA_ARGS__))
    OUT("{\n \"generator\": \"%s\",\n \"rom_crc32\": \"%s\",\n \"files\": [\n", GENERATOR, crc_hex);
    for (int i = 0; i < n; i++) OUT("  \"%s\"%s\n", files[i], i + 1 < n ? "," : "");
    OUT(" ],\n \"runtime\": {\n  \"format\": 1,\n  \"chr\": \"chr.bin\",\n  \"prg\": \"prg.bin\",\n");
    OUT("  \"prg_bytes\": %u,\n  \"prg_code_bytes\": %u,\n  \"prg_code_bytes_left_out\": %u,\n",
        kept, code_bytes, (unsigned)PRG_SIZE - kept);
    OUT("  \"code_list\": \"port/tools/prg_code.txt\"\n }\n}\n");
#undef OUT
    for (int i = 0; i < nold; i++) free(old[i]);
    r = write_file(dir, "manifest.json", json, len);
    free(json);
    return r;
}

int extract_runtime_assets(const char *rom_path, const char *out_dir, int check_crc, char *err, size_t errcap) {
    uint8_t *rom, *mask = NULL, *prgbin = NULL;
    const uint8_t *prg, *chr;
    size_t size, skip, at;
    uint32_t crc, code_bytes = 0, kept = 0, nranges = 0;
    int mapper, r = -1;

    s_err = err;
    s_errcap = errcap;
    err[0] = 0;
    rom = read_all(rom_path, &size);
    if (!rom) return fail("cannot read %s", rom_path);
    crc = mff_crc32(rom, size);
    if (size < 16 || memcmp(rom, "NES\x1a", 4) != 0) {
        fail("%s: no iNES header (not a .nes dump)", rom_path);
        goto out;
    }
    mapper = (rom[6] >> 4) | (rom[7] & 0xF0);
    if (mapper != 4 || rom[4] * 0x4000 != PRG_SIZE || rom[5] * 0x2000 != CHR_SIZE) {
        fail("%s: expected mapper 4, 128 KiB PRG, 128 KiB CHR (got mapper %d, PRG %d KiB, CHR %d KiB)",
             rom_path, mapper, rom[4] * 16, rom[5] * 8);
        goto out;
    }
    if (check_crc && crc != ROM_CRC32) {
        fail("%s: CRC32 %08X, expected %08X (Mighty Final Fight (USA))", rom_path, crc, ROM_CRC32);
        goto out;
    }
    skip = 16 + ((rom[6] & 4) ? 512 : 0);
    if (size < skip + PRG_SIZE + CHR_SIZE) {
        fail("%s is cut short (%zu bytes)", rom_path, size);
        goto out;
    }
    prg = rom + skip;
    chr = prg + PRG_SIZE;

    /* the bytes prg.bin leaves out: code, but what the game reads as data */
    mask = (uint8_t *)calloc(PRG_SIZE, 1);
    prgbin = (uint8_t *)malloc(16 + PRG_SIZE + 8 * (PRG_SIZE / 2 + 1));
    if (!mask || !prgbin) {
        fail("no memory");
        goto out;
    }
    if (mark_ranges(mask, k_prg_code, 1, &code_bytes)) goto out;
    if (keep_past_end_screens(prg, mask)) goto out;
    if (mark_ranges(mask, k_prg_code_read, 0, NULL)) goto out;

    memcpy(prgbin, "MFFPRG01", 8);
    prgbin[8] = rom[6];
    prgbin[9] = prgbin[10] = prgbin[11] = 0;
    at = 16;
    for (uint32_t i = 0; i < PRG_SIZE;) {
        uint32_t start;
        if (mask[i]) { i++; continue; }
        start = i;
        while (i < PRG_SIZE && !mask[i]) i++;
        put32(prgbin + at, start);
        put32(prgbin + at + 4, i - start);
        memcpy(prgbin + at + 8, prg + start, i - start);
        at += 8 + (i - start);
        kept += i - start;
        nranges++;
    }
    put32(prgbin + 12, nranges);

    mff_make_dirs(out_dir);
    if (write_file(out_dir, "chr.bin", chr, CHR_SIZE)) goto out;
    if (write_file(out_dir, "prg.bin", prgbin, at)) goto out;
    if (write_manifest(out_dir, crc, kept, code_bytes)) goto out;
    r = 0;
out:
    free(prgbin);
    free(mask);
    free(rom);
    return r;
}
