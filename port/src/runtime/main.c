/*
 * main.c - Mighty Final Fight, the port (PLAN_DECOMP.md P5)
 *
 *   MightyFinalFight ROM [--smoke N] [--window [--scale N]] [--sprite-limit]
 *
 * Runs the game from power-on: the machine (runtime/) starts in its reset
 * state and the game's reset routine takes over; it never returns.
 *
 *   --smoke N       headless: run N frames and exit (port/tests/run_routes.sh;
 *                   --smoke-interval and --smoke-output are accepted and
 *                   ignored, the picture log replaces them)
 *   --window        play in a window (keyboard and gamepad; needs MFF_SDL)
 *   --scale N       window size, N times 256x240 (default 3)
 *   --sprite-limit  show at most 8 sprites per line, as the console does
 *                   (MFF_SPRITE_LIMIT=1 too)
 * Environment for the tests: MFF_FRAMELOG, MFF_PICLOG, MFF_INPUT, MFF_BOT
 * (runtime/testhooks.c), MFF_LAG (runtime/mff_hooks.c).
 */
#include "machine.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mff_fatal(const char *fmt, ...) {
    va_list ap;
    fflush(stdout);
    fprintf(stderr, "[mff] ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, "\n");
    fflush(stderr);
    hooks_exit_log();
    exit(2);
}

static void usage(void) {
    fprintf(stderr,
            "usage: MightyFinalFight ROM [--smoke N] [--window [--scale N]] [--sprite-limit]\n");
    exit(1);
}

int main(int argc, char **argv) {
    const char *rom = NULL, *e;
    int scale = 3;
    for (int i = 1; i < argc; i++) {
        const char *a = argv[i];
        int more = i + 1 < argc;
        if (!strcmp(a, "--smoke") && more) g_smoke_frames = atol(argv[++i]);
        else if ((!strcmp(a, "--smoke-interval") || !strcmp(a, "--smoke-output")) && more) i++;
        else if (!strcmp(a, "--window")) g_window = 1;
        else if (!strcmp(a, "--scale") && more) scale = atoi(argv[++i]);
        else if (!strcmp(a, "--sprite-limit")) g_sprite_limit = 1;
        else if (!strcmp(a, "--help") || !strcmp(a, "-h")) usage();
        else if (a[0] != '-' && !rom) rom = a;
        else {
            fprintf(stderr, "unknown argument %s\n", a);
            usage();
        }
    }
    e = getenv("MFF_SPRITE_LIMIT");
    if (e && *e == '1') g_sprite_limit = 1;
    if (!rom) usage();
    if (cart_load_dump(rom)) mff_fatal("cannot open %s", rom);

    bus_power_on();
    ppu_power_on();
    apu_power_on();
    mmc3_power_on(g_cart.vertical_mirroring);
    hooks_init();
    mff_hooks_init();
    if (g_window && video_open(scale)) mff_fatal("cannot open the window");
    if (g_smoke_frames > 0) {
        printf("[mff] headless: %ld frames\n", g_smoke_frames);
        fflush(stdout);
    }

    func_RESET();
    mff_fatal("the reset routine returned");
    return 1;
}
