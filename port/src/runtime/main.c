/*
 * main.c - Mighty Final Fight, the port (PLAN_DECOMP.md P5)
 *
 *   MightyFinalFight [--assets DIR] [--smoke N] [--window [--scale N]] [--picture console|scaffold]
 *
 * Runs the game from power-on: the machine (runtime/) starts in its reset
 * state and the game's reset routine takes over; it never returns.
 *
 *   --assets DIR    the data extracted from the dump (default: MFF_ASSETS,
 *                   else assets/ next to the executable or one level up,
 *                   where the CMake build puts it)
 *   --smoke N       headless: run N frames and exit (port/tests/run_routes.sh;
 *                   --smoke-interval and --smoke-output are accepted and
 *                   ignored, the picture log replaces them)
 *   --window        play in a window (keyboard and gamepad; needs MFF_SDL)
 *   --scale N       window size, N times 256x240 (default 3)
 *   --picture P     console (default): the picture as the PPU draws it, 8
 *                   sprites per line; scaffold: as the scaffold's renderer
 *                   drew it (MFF_PICTURE=P too). The picture log
 *                   (MFF_PICLOG) always holds the scaffold's, which is what
 *                   the test baselines have
 * A path given without an option (the test scripts pass the dump's) is
 * ignored: the port reads only the extracted data, never the ROM.
 * Environment for the tests: MFF_FRAMELOG, MFF_PICLOG, MFF_INPUT, MFF_BOT,
 * MFF_SHOTS, MFF_PICDUMP, MFF_PRG_READS (runtime/testhooks.c, mmc3.c),
 * MFF_LAG (runtime/mff_hooks.c).
 */
#include "machine.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif

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
    fprintf(stderr, "usage: MightyFinalFight [--assets DIR] [--smoke N] [--window [--scale N]] "
                    "[--sprite-limit]\n");
    exit(1);
}

/* the directory the executable is in */
static void exe_dir(char *out, size_t cap, const char *argv0) {
    char *slash;
    out[0] = 0;
#ifdef _WIN32
    if (!GetModuleFileNameA(NULL, out, (DWORD)cap)) out[0] = 0;
#else
    {
        ssize_t n = readlink("/proc/self/exe", out, cap - 1);
        out[n > 0 ? n : 0] = 0;
    }
#endif
    if (!out[0]) snprintf(out, cap, "%s", argv0);
    slash = strrchr(out, '/');
#ifdef _WIN32
    {
        char *bs = strrchr(out, '\\');
        if (!slash || (bs && bs > slash)) slash = bs;
    }
#endif
    if (slash) *slash = 0;
    else snprintf(out, cap, ".");
}

static const char *find_assets(const char *given, const char *argv0) {
    static char path[1024];
    char dir[1024];
    const char *env = getenv("MFF_ASSETS");
    if (given) return cart_load_assets(given) ? NULL : given;
    if (env && *env) return cart_load_assets(env) ? NULL : env;
    exe_dir(dir, sizeof dir, argv0);
    snprintf(path, sizeof path, "%s/assets", dir);
    if (!cart_load_assets(path)) return path;
    snprintf(path, sizeof path, "%s/../assets", dir);
    if (!cart_load_assets(path)) return path;
    return NULL;
}

int main(int argc, char **argv) {
    const char *assets = NULL, *ignored = NULL, *picture = getenv("MFF_PICTURE");
    int scale = 3;
    for (int i = 1; i < argc; i++) {
        const char *a = argv[i];
        int more = i + 1 < argc;
        if (!strcmp(a, "--assets") && more) assets = argv[++i];
        else if (!strcmp(a, "--smoke") && more) g_smoke_frames = atol(argv[++i]);
        else if ((!strcmp(a, "--smoke-interval") || !strcmp(a, "--smoke-output")) && more) i++;
        else if (!strcmp(a, "--window")) g_window = 1;
        else if (!strcmp(a, "--scale") && more) scale = atoi(argv[++i]);
        else if (!strcmp(a, "--picture") && more) picture = argv[++i];
        else if (!strcmp(a, "--help") || !strcmp(a, "-h")) usage();
        else if (a[0] != '-' && !ignored) ignored = a;
        else {
            fprintf(stderr, "unknown argument %s\n", a);
            usage();
        }
    }
    if (picture && *picture) {
        if (!strcmp(picture, "console")) g_picture = PICTURE_CONSOLE;
        else if (!strcmp(picture, "scaffold")) g_picture = PICTURE_SCAFFOLD;
        else usage();
    }
    assets = find_assets(assets, argv[0]);
    if (!assets) {
        fprintf(stderr,
                "[mff] The game's data is missing (chr.bin and prg.bin). Make it once from your\n"
                "[mff] Mighty Final Fight (USA) dump (CRC32 15EB0BEE):\n"
                "[mff]   cmake -S port -B build_port -DMFF_ROM=\"path/to/the dump.nes\"\n"
                "[mff]   cmake --build build_port --config Release\n"
                "[mff] or: py -3 port/tools/extract_assets.py DUMP --only runtime --out DIR\n"
                "[mff] and run with --assets DIR (or MFF_ASSETS=DIR).\n");
        return 1;
    }
    if (ignored) printf("[mff] %s ignored: the data comes from %s\n", ignored, assets);

    bus_power_on();
    ppu_power_on();
    apu_power_on();
    mmc3_power_on(g_cart.vertical_mirroring);
    hooks_init();
    mff_hooks_init();
    if (g_window && video_open(scale)) mff_fatal("cannot open the window");
    if (g_smoke_frames > 0) printf("[mff] headless: %ld frames\n", g_smoke_frames);
    fflush(stdout);

    maybe_trigger_vblank(7);            /* the 6502's reset sequence, before the first instruction */
    func_RESET();
    mff_fatal("the reset routine returned");
    return 1;
}
