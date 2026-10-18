/*
 * main.c - Mighty Final Fight, the port (PLAN_DECOMP.md P5, P8)
 *
 *   MightyFinalFight [DUMP] [--assets DIR] [--window | --headless | --smoke N] [--scale N]
 *                    [--picture console|scaffold] [--extract DUMP] [--load STATE] [--config FILE]
 *
 * Runs the game from power-on: the machine (runtime/) starts in its reset
 * state and the game's reset routine takes over; it never returns.
 *
 *   DUMP            a path without an option: the player's dump of Mighty
 *                   Final Fight (USA), used once when the game's data is
 *                   missing (setup.c) and ignored when it is there (the
 *                   test scripts pass it)
 *   --extract DUMP  extract the game's data from the dump (extract.c, the
 *                   same files as extract_assets.py --only runtime) into
 *                   --assets DIR or the data directory's assets/, and exit
 *   --assets DIR    the data extracted from the dump (default: MFF_ASSETS,
 *                   else assets/ next to the executable or one level up,
 *                   where the CMake build puts it, else the user's data
 *                   directory: setup.c)
 *   --smoke N       headless: run N frames and exit (port/tests/run_routes.sh;
 *                   --smoke-interval and --smoke-output are accepted and
 *                   ignored, the picture log replaces them)
 *   --window        play in a window (keyboard and gamepad; needs MFF_SDL).
 *                   The default when the build has one and neither --smoke
 *                   nor --headless is given
 *   --headless      no window
 *   --scale N       window size, N times 256x240 (default 3, or the config's)
 *   --load STATE    start from a save state (savestate.c) instead of power-on
 *   --config FILE   the settings (config.c; default MightyFinalFight.ini in
 *                   the data directory, read only with a window)
 *   --picture P     console (default): the picture as the PPU draws it, 8
 *                   sprites per line; scaffold: as the scaffold's renderer
 *                   drew it (MFF_PICTURE=P too). The picture log
 *                   (MFF_PICLOG) always holds the scaffold's, which is what
 *                   the test baselines have
 * The port reads only the extracted data, never the ROM.
 * Environment for the tests: MFF_FRAMELOG, MFF_PICLOG, MFF_INPUT, MFF_BOT,
 * MFF_SHOTS, MFF_PICDUMP, MFF_PRG_READS (runtime/testhooks.c, mmc3.c),
 * MFF_LAG (runtime/mff_hooks.c), MFF_SAVE_AT and MFF_SAVE_DIR (savestate.c).
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

static void usage(FILE *f) {
    fprintf(f,
            "usage: MightyFinalFight [DUMP] [--assets DIR] [--window | --headless | --smoke N]\n"
            "                        [--scale N] [--picture console|scaffold] [--extract DUMP]\n"
            "  DUMP            your Mighty Final Fight (USA) .nes (CRC32 15EB0BEE), needed once:\n"
            "                  the game's data is extracted from it into assets/\n"
            "  --extract DUMP  only extract the data (into --assets DIR or assets/) and exit\n"
            "  --assets DIR    where the extracted data is\n"
            "  --window        play in a window (the default when this build has one)\n"
            "  --headless      no window; --smoke N: run N frames headless and exit\n"
            "  --scale N       window size, N x 256x240\n"
            "  --picture P     console (default) or scaffold\n"
            "  --load STATE    start from a save state (F5 saves, F7 loads in the window)\n"
            "  --config FILE   settings (default MightyFinalFight.ini next to the game)\n");
}

int main(int argc, char **argv) {
    const char *assets = NULL, *dump = NULL, *extract = NULL, *picture = getenv("MFF_PICTURE");
    const char *load = NULL, *config = NULL;
    int scale = 0, window = -1;
    for (int i = 1; i < argc; i++) {
        const char *a = argv[i];
        int more = i + 1 < argc;
        if (!strcmp(a, "--assets") && more) assets = argv[++i];
        else if (!strcmp(a, "--smoke") && more) g_smoke_frames = atol(argv[++i]);
        else if ((!strcmp(a, "--smoke-interval") || !strcmp(a, "--smoke-output")) && more) i++;
        else if (!strcmp(a, "--window")) window = 1;
        else if (!strcmp(a, "--headless")) window = 0;
        else if (!strcmp(a, "--scale") && more) scale = atoi(argv[++i]);
        else if (!strcmp(a, "--picture") && more) picture = argv[++i];
        else if (!strcmp(a, "--extract") && more) extract = argv[++i];
        else if (!strcmp(a, "--load") && more) load = argv[++i];
        else if (!strcmp(a, "--config") && more) config = argv[++i];
        else if (!strcmp(a, "--help") || !strcmp(a, "-h")) {
            usage(stdout);
            return 0;
        }
        else if (a[0] != '-' && !dump) dump = a;
        else {
            fprintf(stderr, "unknown argument %s\n", a);
            usage(stderr);
            return 1;
        }
    }
    if (picture && *picture) {
        if (!strcmp(picture, "console")) g_picture = PICTURE_CONSOLE;
        else if (!strcmp(picture, "scaffold")) g_picture = PICTURE_SCAFFOLD;
        else {
            usage(stderr);
            return 1;
        }
    }
    setup_init(argv[0]);
    if (extract) return setup_extract_cli(extract, assets);
    if (window < 0) window = video_available() && g_smoke_frames <= 0;
    g_window = window;
    if (g_window || config) config_load(config, g_window && !config);
    if (scale <= 0) scale = config_int("video.scale", 3);

    if (assets) {
        if (!setup_find_assets(assets)) {
            fprintf(stderr, "[mff] no game data (chr.bin and prg.bin) in %s\n", assets);
            return 1;
        }
    } else if ((assets = setup_find_assets(NULL))) {
        if (dump) printf("[mff] %s ignored: the data comes from %s\n", dump, assets);
    } else if (!(assets = setup_first_run(dump, g_window))) {
        return 1;
    }

    bus_power_on();
    ppu_power_on();
    apu_power_on();
    mmc3_power_on(g_cart.vertical_mirroring);
    hooks_init();
    mff_hooks_init();
    savestate_init();
    if (g_window && video_open(scale)) mff_fatal("cannot open the window");
    if (g_smoke_frames > 0) printf("[mff] %s%ld frames\n", g_window ? "" : "headless: ", g_smoke_frames);
    fflush(stdout);

    savestate_run(load);                /* reset (or the state), and the game; never returns */
    return 1;
}
