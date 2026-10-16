/*
 * bot.c - an in-process player for long P4 regression routes
 *
 * MFF_BOT=CHAR[:SEED[:SKIP]]   CHAR 0 Cody, 1 Guy, 2 Haggar
 *
 * The policy of tools/bot.py, run inside the scaffold: every SEG frames it
 * reads RAM and picks the buttons for the next SEG frames, and it plays with
 * that bot's --god aids (health and lives refilled, an enemy alive for
 * LONG_FIGHT frames has its life cut, and with SKIP the game's own skip-stage
 * action, $0030 = $0E, after SKIP frames in an area: a tour of every round).
 * Everything it does is a function of RAM and its own seeded RNG, so two
 * builds that keep RAM identical play identically - which is all the P4
 * selfcheck needs; a divergence ends the comparison anyway.
 *
 * A 50,000-frame tour reaches the final boss (round 4, area 2) at ~25,000.
 * (Until labels.toml named them right, the life cut wrote $04A0, the
 * maximum, which only shortens the HUD bar: no long fight ever ended.)
 *
 * RAM (docs/ram_map.md): slots $0300+i state ($80 live), $0320+i id,
 * $0360+i x, $03C0+i y, slot 0 the player; $30 game action (09 continue),
 * $34 character (the select cursor), $3A health, $3E lives, $AF credits
 * (both 0 until the first game starts), $61 round (0-4), $22/$60 round
 * tables / sub area, $0490+i enemy life ($04A0+i is its maximum).
 */
#include "nes_runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { SEG = 16, STALL = 6, UNSTICK = 4, BAN = 40, REACH = 22, Y_SLACK = 3,
       LONG_FIGHT = 3000, DIALOG_AFTER = 1500 };
enum { BA = 0x80, BB = 0x40, BSTART = 0x10, BU = 0x08, BD = 0x04, BL = 0x02, BR = 0x01 };

typedef struct { int off; uint8_t mask; } Step;

static int s_on = -1, s_char, s_skip;
static uint32_t s_rng = 1;
static Step s_steps[8];
static int s_nsteps;
static uint8_t s_mask;
static long s_tick, s_seg_start;
/* stall detector */
static int s_hist_n, s_unstick, s_walking, s_target = -1;
static struct { uint8_t x, y; int walking, target; } s_hist[STALL];
static int s_banned[16];
/* god aids */
static uint8_t s_full_hp;
static int s_age[16];
static int s_mark[3] = { -1, -1, -1 };
static long s_mark_frame;

static uint32_t rnd(void) {          /* xorshift32 */
    s_rng ^= s_rng << 13; s_rng ^= s_rng >> 17; s_rng ^= s_rng << 5;
    return s_rng;
}

static void steps1(uint8_t m) { s_steps[0].off = 0; s_steps[0].mask = m; s_nsteps = 1; }
static void steps2(uint8_t m, int off2, uint8_t m2) {
    steps1(m); s_steps[1].off = off2; s_steps[1].mask = m2; s_nsteps = 2;
}

static int is_foe(int i) {
    uint8_t id = g_ram[0x320 + i], y = g_ram[0x3C0 + i];
    /* a fighter has a maximum life (obj_life_max $04A0); thrown and flying
     * objects (the final boss's $9D) do not */
    return (g_ram[0x300 + i] & 0x80) && y >= 0x50 && g_ram[0x4A0 + i] &&
           ((id >= 0x20 && id <= 0x35) || (id >= 0x70 && id <= 0x9F));
}

/* Before the first game (credits and lives still 0): the keyed routes'
 * prologue - START at the title, DOWN once per character at the select
 * screen, START - at the same frames, since the bot is idle until then. */
static uint8_t prologue(long frame) {
    long at = frame - 4520;
    if (at >= 0 && at < 8) return BSTART;
    for (int i = 0; i < s_char; i++)
        if (at >= 80 + 16 * i && at < 86 + 16 * i) return BD;
    if (at >= 80 + 16 * s_char && at < 88 + 16 * s_char) return BSTART;
    if (frame > 6000 && (frame & 63) < 8) return BSTART;      /* got lost: keep tapping */
    return 0;
}

static int in_game(void) { return g_ram[0xAF] || g_ram[0x3E]; }

static void policy(void) {
    uint8_t act = g_ram[0x30];
    if (act == 0x09) { steps2(BSTART, 4, 0); return; }          /* continue screen */
    if (!(g_ram[0x300] & 0x80)) {            /* no live player: cutscene, map, round end */
        steps2(s_tick % 4 == 0 ? BSTART : BA, 4, 0);
        return;
    }
    uint8_t px = g_ram[0x360], py = g_ram[0x3C0];
    /* pause once in a while (gameplay_pause_check, $C904), then tap START
     * until the pause ($98) clears */
    if (g_ram[0x98]) { if (s_tick & 1) steps2(BSTART, 4, 0); else s_nsteps = 0, s_mask = 0; return; }
    if (s_tick % 600 == 300) { steps2(BSTART, 4, 0); return; }
    for (int i = 0; i < 16; i++) if (s_banned[i]) s_banned[i]--;
    if (s_hist_n == STALL) { memmove(s_hist, s_hist + 1, sizeof s_hist[0] * (STALL - 1)); s_hist_n--; }
    s_hist[s_hist_n].x = px; s_hist[s_hist_n].y = py;
    s_hist[s_hist_n].walking = s_walking; s_hist[s_hist_n].target = s_target;
    s_hist_n++;
    if (s_unstick) {
        static const uint8_t moves[] = { BR, BL, BU, BD, BU | BR, BD | BR, BR | BA };
        s_unstick--;
        s_walking = 0;
        steps1(moves[rnd() % 7]);
        return;
    }
    if (s_hist_n == STALL) {
        int stuck = 1;
        for (int i = 0; i < STALL; i++)
            if (!s_hist[i].walking || s_hist[i].x != s_hist[0].x || s_hist[i].y != s_hist[0].y) stuck = 0;
        if (stuck) {
            if (s_hist[STALL - 1].target >= 0) s_banned[s_hist[STALL - 1].target] = BAN;
            s_unstick = UNSTICK;
            s_hist_n = 0;
            steps1(BR | BA);
            return;
        }
    }
    int best = -1, bestd = 1 << 30;
    for (int i = 3; i < 16; i++) {
        if (!is_foe(i) || s_banned[i]) continue;
        int d = abs(g_ram[0x360 + i] - px) + 2 * abs(g_ram[0x3C0 + i] - py);
        if (d < bestd) { bestd = d; best = i; }
    }
    if (best < 0) { s_walking = 1; s_target = -1; steps1(BU | BR); return; }
    s_target = best;
    int ex = g_ram[0x360 + best], ey = g_ram[0x3C0 + best];
    int side = px < ex ? -1 : 1;
    int want = ex + side * REACH;
    uint8_t dirs = 0;
    if (abs(ey - py) > Y_SLACK) dirs |= ey > py ? BD : BU;
    if (abs(want - px) > 6) dirs |= want > px ? BR : BL;
    s_walking = dirs != 0;
    if (dirs) { steps1(dirs); return; }
    uint8_t face = side < 0 ? BR : BL;
    uint32_t r = rnd() % 100;
    if (r < 8) { steps2(BA | BB, 6, 0); return; }               /* special */
    if (r < 18) {                                               /* jump attack */
        steps2(face | BA, 5, face | BA | BB);
        s_steps[2].off = 10; s_steps[2].mask = 0; s_nsteps = 3;
        return;
    }
    if (r < 28) { steps1(face); return; }                       /* walk in: grab */
    static const Step punch[] = { {0, 0}, {2, BB}, {5, 0}, {8, BB}, {11, 0}, {13, BB} };
    memcpy(s_steps, punch, sizeof punch);
    s_steps[0].mask = face;
    s_nsteps = 6;
}

static void god(long frame) {
    if ((g_ram[0x300] & 0x80) && g_ram[0x3A] > 0 && g_ram[0x3A] < s_full_hp) g_ram[0x3A] = s_full_hp;
    if (g_ram[0x3E] < 3 && g_ram[0x30] != 0x09) g_ram[0x3E] = 3;
    for (int i = 3; i < 16; i++) {
        /* age by slot while it is live: a boss's id changes while it is hit
         * ($74 -> $05/$02 and back), which used to restart the count */
        if (!(g_ram[0x300 + i] & 0x80)) { s_age[i] = 0; continue; }
        s_age[i] += SEG;
        if (!is_foe(i)) continue;
        /* obj_life $0490 (signed: a dying enemy goes below 0) */
        if (s_age[i] > LONG_FIGHT && (int8_t)g_ram[0x490 + i] > 1) g_ram[0x490 + i] = 1;
        /* still standing long after: below 0 is the game's own death test */
        if (s_age[i] > 2 * LONG_FIGHT && g_ram[0x490 + i] == 1) g_ram[0x490 + i] = 0xFF;
    }
    int mark[3] = { g_ram[0x61], g_ram[0x22], g_ram[0x60] };
    if (memcmp(mark, s_mark, sizeof mark)) { memcpy(s_mark, mark, sizeof mark); s_mark_frame = frame; }
    /* never in the final area (round 4, area 2): skipping it runs past the
     * game's end (round index 5, stuck in action $0E); the boss's life is
     * cut instead */
    else if (s_skip && !(g_ram[0x61] == 4 && g_ram[0x60] >= 2) && (g_ram[0x300] & 0x80) &&
             frame - s_mark_frame > s_skip) {
        g_ram[0x30] = 0x0E;
        s_mark_frame = frame;
    }
    if (g_ram[0x3A] < 0x80 && g_ram[0x3A] > s_full_hp) s_full_hp = g_ram[0x3A];
}

/* Called between frames, before the NMI reads the pads. */
void mff_bot_frame(uint64_t frame_count) {
    if (s_on < 0) {
        const char *e = getenv("MFF_BOT");
        s_on = 0;
        if (e && *e) {
            s_on = 1;
            s_char = atoi(e);
            const char *c = strchr(e, ':');
            if (c) { s_rng = (uint32_t)strtoul(c + 1, NULL, 10) | 1; c = strchr(c + 1, ':'); }
            if (c) s_skip = atoi(c + 1);
            fprintf(stderr, "[mff] MFF_BOT: character %d, seed %u, skip %d\n", s_char, s_rng, s_skip);
        }
    }
    if (!s_on) return;
    long frame = (long)frame_count;
    if (!in_game()) {
        g_controller1_buttons = s_mask = prologue(frame);
        s_seg_start = frame + 1;
        s_mark_frame = frame;
        s_nsteps = 0;
        return;
    }
    int off = (int)((frame - s_seg_start) % SEG);
    if (off == 0) {
        policy();
        if (frame - s_mark_frame > DIALOG_AFTER && s_tick % 3 == 0 && (g_ram[0x300] & 0x80))
            steps2(BA, 4, 0);                /* boss dialogs wait for a fresh A */
        god(frame);
        s_tick++;
    }
    for (int i = 0; i < s_nsteps; i++)
        if (s_steps[i].off == off) s_mask = s_steps[i].mask;
    g_controller1_buttons = s_mask;
    if (frame % 3000 == 0)
        fprintf(stderr, "[mff] bot f=%ld act=%02X char=%d round=%d area=%d hp=%d lives=%d\n", frame,
                g_ram[0x30], g_ram[0x34], g_ram[0x61], g_ram[0x60], g_ram[0x3A], g_ram[0x3E]);
}
