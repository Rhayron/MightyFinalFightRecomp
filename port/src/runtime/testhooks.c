/*
 * testhooks.c - what port/tests reads and drives, in the scaffold's formats
 * (port/scaffold/extras.c), so run_routes.sh checks this executable as it
 * checks the scaffold
 *
 * MFF_FRAMELOG=FILE  per frame, before the NMI: u32 frame, CPU RAM [$800],
 *                    OAM [$100], palette RAM [$20]
 * MFF_PICLOG=FILE    per frame, after the picture: u32 frame, u32 FNV-1a of
 *                    the 256x240 ARGB pixels, u32 low CPU cycle count
 * MFF_INPUT=FILE     pad 1 keyed by the game's NMI count (port/tests/
 *                    nmi_input.py): lines "count repeat buttons(hex)"
 * MFF_BOT=C:SEED:SKIP  the in-process player (port/scaffold/bot.c)
 * NESRECOMP_FALLBACK_LOG=FILE  run_routes.sh asks every runtime how much ROM
 *                    code it interpreted; this one has no interpreter, so
 *                    the answer is always none
 * MFF_SHOTS=F1,F2,.. the pictures of those frames as PNG, in MFF_SHOT_DIR
 *                    (default the current directory): frame_NNNNNN.png
 */
#include "machine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mff_bot_frame(uint64_t frame);     /* port/scaffold/bot.c */

static FILE *s_framelog, *s_piclog;
static const char *s_interp_path;
static long s_shots[64];
static int s_nshots;
static const char *s_shot_dir = ".";

/* ---- keyed input ---- */

typedef struct { uint32_t count; uint16_t rep; uint8_t mask; } Key;
static Key *s_keys;
static size_t s_nkeys, s_at;
static int s_keyed, s_started, s_prev = -1;
static uint32_t s_count;
static uint16_t s_rep;

static void keys_load(const char *path) {
    FILE *f = fopen(path, "r");
    size_t cap = 1 << 16;
    unsigned c, r, m;
    if (!f) mff_fatal("MFF_INPUT: cannot open %s", path);
    s_keys = (Key *)malloc(cap * sizeof *s_keys);
    if (!s_keys) mff_fatal("no memory for MFF_INPUT");
    while (fscanf(f, "%u %u %x", &c, &r, &m) == 3) {
        if (s_nkeys == cap) {
            s_keys = (Key *)realloc(s_keys, (cap *= 2) * sizeof *s_keys);
            if (!s_keys) mff_fatal("no memory for MFF_INPUT");
        }
        s_keys[s_nkeys].count = c;
        s_keys[s_nkeys].rep = (uint16_t)r;
        s_keys[s_nkeys].mask = (uint8_t)m;
        s_nkeys++;
    }
    fclose(f);
    s_keyed = 1;
    fprintf(stderr, "[mff] MFF_INPUT: %zu keyed frames\n", s_nkeys);
}

/* The key of this frame: NMIs since reset (frame_counter $92, unwrapped
 * from its first 0 -> 1 step) and how many frames already showed that
 * count. Before the first step the pad is idle. */
static void keyed_input(void) {
    int v = g_ram[0x92];
    if (!s_started) {
        if (s_prev == 0 && v == 1) { s_started = 1; s_count = 1; s_rep = 0; }
        s_prev = v;
        if (!s_started) { g_controller1_buttons = 0; return; }
    } else {
        uint32_t next = s_count + (uint32_t)((v - s_prev) & 0xFF);
        s_rep = (next == s_count) ? (uint16_t)(s_rep + 1) : 0;
        s_count = next;
        s_prev = v;
    }
    while (s_at < s_nkeys && (s_keys[s_at].count < s_count ||
                              (s_keys[s_at].count == s_count && s_keys[s_at].rep < s_rep)))
        s_at++;
    if (s_at < s_nkeys && s_keys[s_at].count == s_count)
        g_controller1_buttons = s_keys[s_at].mask;
    else if (s_at > 0)
        g_controller1_buttons = s_keys[s_at - 1].mask;   /* more repeats than the route had */
}

/* ---- logs ---- */

static FILE *open_log(const char *var) {
    const char *p = getenv(var);
    FILE *f;
    if (!p || !*p) return NULL;
    f = fopen(p, "wb");
    if (!f) mff_fatal("%s: cannot write %s", var, p);
    return f;
}

void hooks_init(void) {
    const char *in = getenv("MFF_INPUT"), *shots = getenv("MFF_SHOTS"), *dir = getenv("MFF_SHOT_DIR");
    s_framelog = open_log("MFF_FRAMELOG");
    s_piclog = open_log("MFF_PICLOG");
    if (in && *in) keys_load(in);
    for (const char *p = shots; p && *p && s_nshots < 64;) {
        char *end;
        long v = strtol(p, &end, 10);
        if (end == p) break;
        s_shots[s_nshots++] = v;
        p = *end ? end + 1 : end;
    }
    if (dir && *dir) s_shot_dir = dir;
    s_interp_path = getenv("NESRECOMP_FALLBACK_LOG");
    if (s_interp_path && *s_interp_path) {
        FILE *f = fopen(s_interp_path, "w");
        if (f) {
            fprintf(f, "{\"event\":\"session_start\",\"version\":1,\"interpreter\":false,\"frame\":0}\n");
            fclose(f);
        }
    } else {
        s_interp_path = NULL;
    }
}

void hooks_frame_start(uint64_t frame) {
    if (s_framelog) {
        uint32_t fr = (uint32_t)frame;
        fwrite(&fr, 4, 1, s_framelog);
        fwrite(g_ram, 1, 0x800, s_framelog);
        fwrite(ppu_oam(), 1, 0x100, s_framelog);
        fwrite(ppu_palette(), 1, 0x20, s_framelog);
        fflush(s_framelog);
    }
    if (s_keyed) keyed_input();
    mff_bot_frame(frame);
}

void hooks_picture(const uint32_t *fb) {
    uint32_t rec[3], h = 2166136261u;
    for (int i = 0; i < s_nshots; i++) {
        if (s_shots[i] == (long)g_frame_count) {
            char path[1024];
            snprintf(path, sizeof path, "%s/frame_%06ld.png", s_shot_dir, s_shots[i]);
            if (png_write_argb(path, fb, 256, 240)) fprintf(stderr, "[mff] cannot write %s\n", path);
        }
    }
    if (!s_piclog) return;
    for (int i = 0; i < 256 * 240; i++) {
        h ^= fb[i];
        h *= 16777619u;
    }
    rec[0] = (uint32_t)g_frame_count;
    rec[1] = h;
    rec[2] = (uint32_t)g_nes_cycles;
    fwrite(rec, 4, 3, s_piclog);
    fflush(s_piclog);
}

void hooks_exit_log(void) {
    mmc3_write_prg_reads();
    if (s_interp_path) {
        FILE *f = fopen(s_interp_path, "a");
        if (f) {
            fprintf(f, "{\"event\":\"session_end\",\"frame\":%llu,\"interpreter\":false,"
                    "\"total_instrs\":0,\"total_dispatch_misses\":0,\"entries\":[]}\n",
                    (unsigned long long)g_frame_count);
            fclose(f);
        }
        s_interp_path = NULL;
    }
    if (s_framelog) fflush(s_framelog);
    if (s_piclog) fflush(s_piclog);
}
