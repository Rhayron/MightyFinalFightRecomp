/*
 * setup.c - where the port finds and keeps its files, and the first run
 *
 * The executable carries no data from the game. On the first run it needs
 * the player's dump of Mighty Final Fight (USA) once: it extracts the
 * game's data from it (extract.c, CRC32 15EB0BEE) into assets/ and never
 * reads the dump again. The dump can be
 *   - given on the command line (a path without an option: dropping the
 *     .nes file on the executable does that) or with --extract DUMP;
 *   - lying next to the executable or in the current directory (any .nes
 *     file there with the right CRC32);
 *   - dropped on the window, which asks for it when there is none.
 *
 * The data directory, where assets/, saves/ and MightyFinalFight.ini go:
 * the executable's own directory when it can be written (a portable
 * install), else the user's (%APPDATA%\MightyFinalFight,
 * ~/Library/Application Support/MightyFinalFight, or
 * $XDG_DATA_HOME/MightyFinalFight, by default ~/.local/share/...).
 * The game data is looked for in --assets DIR, MFF_ASSETS, assets/ next to
 * the executable, ../assets (where the CMake build puts it), then the data
 * directory's assets/.
 */
#include "machine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dirent.h>
#include <unistd.h>
#endif
#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

#define PATH_CAP 1024

static int getcwd_portable(char *out, size_t cap) {
#ifdef _WIN32
    return GetCurrentDirectoryA((DWORD)cap, out) > 0;
#else
    return getcwd(out, cap) != NULL;
#endif
}

static char s_exe_dir[PATH_CAP], s_user_dir[PATH_CAP], s_data_dir[PATH_CAP];
static char s_assets[PATH_CAP + 32];

static void exe_dir(char *out, size_t cap, const char *argv0) {
    char *slash;
    out[0] = 0;
#if defined(_WIN32)
    if (!GetModuleFileNameA(NULL, out, (DWORD)cap)) out[0] = 0;
#elif defined(__APPLE__)
    {
        uint32_t size = (uint32_t)cap;
        if (_NSGetExecutablePath(out, &size) != 0) out[0] = 0;
    }
#else
    {
        ssize_t n = readlink("/proc/self/exe", out, cap - 1);
        out[n > 0 ? n : 0] = 0;
    }
#endif
    if (!out[0]) snprintf(out, cap, "%s", argv0 ? argv0 : ".");
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

static void user_dir(char *out, size_t cap) {
    const char *home = getenv("HOME");
    out[0] = 0;
#if defined(_WIN32)
    {
        const char *app = getenv("APPDATA");
        if (app && *app) snprintf(out, cap, "%s\\MightyFinalFight", app);
    }
#elif defined(__APPLE__)
    if (home && *home) snprintf(out, cap, "%s/Library/Application Support/MightyFinalFight", home);
#else
    {
        const char *xdg = getenv("XDG_DATA_HOME");
        if (xdg && *xdg) snprintf(out, cap, "%s/MightyFinalFight", xdg);
        else if (home && *home) snprintf(out, cap, "%s/.local/share/MightyFinalFight", home);
    }
#endif
    (void)home;
}

/* the directory exists (made if need be) and a file can be written there */
static int writable(const char *dir) {
    char path[PATH_CAP + 32];
    FILE *f;
    if (!dir[0]) return 0;
    mff_make_dirs(dir);
    snprintf(path, sizeof path, "%s/.mff_write_test", dir);
    f = fopen(path, "wb");
    if (!f) return 0;
    fclose(f);
    remove(path);
    return 1;
}

void setup_init(const char *argv0) {
    exe_dir(s_exe_dir, sizeof s_exe_dir, argv0);
    user_dir(s_user_dir, sizeof s_user_dir);
}

const char *setup_exe_dir(void) { return s_exe_dir; }

const char *setup_data_dir(void) {
    if (!s_data_dir[0]) {
        if (writable(s_exe_dir) || !s_user_dir[0] || !writable(s_user_dir))
            snprintf(s_data_dir, sizeof s_data_dir, "%s", s_exe_dir);
        else
            snprintf(s_data_dir, sizeof s_data_dir, "%s", s_user_dir);
    }
    return s_data_dir;
}

static const char *try_assets(const char *dir) {
    if (cart_load_assets(dir)) return NULL;
    snprintf(s_assets, sizeof s_assets, "%s", dir);
    return s_assets;
}

const char *setup_find_assets(const char *given) {
    char path[PATH_CAP + 16];
    const char *env = getenv("MFF_ASSETS"), *r;
    if (given) return try_assets(given);
    if (env && *env) return try_assets(env);
    snprintf(path, sizeof path, "%s/assets", s_exe_dir);
    if ((r = try_assets(path))) return r;
    snprintf(path, sizeof path, "%s/../assets", s_exe_dir);
    if ((r = try_assets(path))) return r;
    if (s_user_dir[0]) {
        snprintf(path, sizeof path, "%s/assets", s_user_dir);
        if ((r = try_assets(path))) return r;
    }
    return NULL;
}

/* extract `dump` into DIR/assets of the data directory and load it */
static const char *extract_and_load(const char *dump, char *err, size_t errcap) {
    char out[PATH_CAP + 16];
    snprintf(out, sizeof out, "%s/assets", setup_data_dir());
    if (extract_runtime_assets(dump, out, 1, err, errcap)) return NULL;
    printf("[mff] the game's data was extracted from %s into %s\n", dump, out);
    fflush(stdout);
    return try_assets(out);
}

int setup_extract_cli(const char *dump, const char *out) {
    char err[1024], dir[PATH_CAP + 16];
    if (!out) {
        snprintf(dir, sizeof dir, "%s/assets", setup_data_dir());
        out = dir;
    }
    if (extract_runtime_assets(dump, out, 1, err, sizeof err)) {
        fprintf(stderr, "[mff] %s\n", err);
        return 1;
    }
    printf("[mff] chr.bin, prg.bin and manifest.json in %s\n", out);
    return 0;
}

/* a .nes file in `dir` with Mighty Final Fight (USA)'s CRC32 */
static int find_dump_in(const char *dir, char *out, size_t cap) {
    int found = 0;
#ifdef _WIN32
    char pattern[PATH_CAP + 8];
    WIN32_FIND_DATAA fd;
    HANDLE h;
    snprintf(pattern, sizeof pattern, "%s\\*.nes", dir);
    h = FindFirstFileA(pattern, &fd);
    if (h == INVALID_HANDLE_VALUE) return 0;
    do {
        snprintf(out, cap, "%s\\%s", dir, fd.cFileName);
        found = mff_is_dump(out);
    } while (!found && FindNextFileA(h, &fd));
    FindClose(h);
#else
    DIR *d = opendir(dir);
    struct dirent *e;
    if (!d) return 0;
    while (!found && (e = readdir(d))) {
        size_t n = strlen(e->d_name);
        if (n < 4 || (strcmp(e->d_name + n - 4, ".nes") && strcmp(e->d_name + n - 4, ".NES"))) continue;
        snprintf(out, cap, "%s/%s", dir, e->d_name);
        found = mff_is_dump(out);
    }
    closedir(d);
#endif
    return found;
}

const char *setup_first_run(const char *dump, int window) {
    char err[1024], found[PATH_CAP + 260], dir[PATH_CAP];
    const char *r;
    err[0] = 0;
    if (dump) {
        if ((r = extract_and_load(dump, err, sizeof err))) return r;
        fprintf(stderr, "[mff] %s\n", err);
    } else if (find_dump_in(s_exe_dir, found, sizeof found) ||
               (getcwd_portable(dir, sizeof dir) && find_dump_in(dir, found, sizeof found))) {
        if ((r = extract_and_load(found, err, sizeof err))) return r;
        fprintf(stderr, "[mff] %s\n", err);
    }
    if (window) {
        char dropped[PATH_CAP];
        const char *why = err[0] ? err : NULL;
        while (video_ask_for_dump(dropped, sizeof dropped, why) == 0) {
            if ((r = extract_and_load(dropped, err, sizeof err))) return r;
            why = err;
        }
        return NULL;
    }
    fprintf(stderr,
            "[mff] The game's data is missing. It comes from your dump of Mighty Final Fight (USA)\n"
            "[mff] (iNES, CRC32 15EB0BEE), once:\n"
            "[mff]   MightyFinalFight \"path/to/Mighty Final Fight (USA).nes\"\n"
            "[mff] or put the .nes file next to the executable, or run with --extract DUMP.\n"
            "[mff] The data goes to %s/assets; nothing else is read from the dump.\n",
            setup_data_dir());
    return NULL;
}
