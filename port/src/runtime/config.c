/*
 * config.c - MightyFinalFight.ini, the player's settings (P8)
 *
 * A small INI file in the data directory (setup.c: next to the executable
 * when it can be written). The window writes the default one on its first
 * run; edit it and start the game again. Lines are "key = value" under a
 * [section]; a line starting with # or ; is a comment. Headless runs (the
 * tests) read no file unless given --config FILE, so a player's settings
 * never reach a route check.
 *
 *   [video]  scale (window size, N x 256x240), fullscreen (0/1), and P9's
 *            picture, no_flicker and wide (render_hd.h's hd_option)
 *   [audio]  enabled (0/1)
 *   [game]   slowdown: empty keeps the game's own setting; anything else is
 *            given to the game as MFF_SLOWDOWN (the --slowdown option) when
 *            neither the command line nor the environment sets it
 *   [keys]   each NES button and each hotkey: SDL key names, several
 *            separated by commas (video_sdl.c)
 *   [pad]    each NES button: SDL game controller button names
 */
#include "machine.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRIES 128

static struct {
    char key[128];
    char value[192];
} s_ent[MAX_ENTRIES];
static int s_n;
static char s_path[1100];

static const char k_default[] =
    "# Mighty Final Fight - settings. Written by the game on its first run in a\n"
    "# window; edit, then start the game again.\n"
    "#\n"
    "# [keys] take SDL key names (https://wiki.libsdl.org/SDL2/SDL_Keycode, the\n"
    "# \"Key Name\" column: X, Return, Right Shift, Left Ctrl, Space, F5 ...), several\n"
    "# separated by commas. [pad] takes SDL game controller buttons: a b x y back\n"
    "# guide start leftstick rightstick leftshoulder rightshoulder dpup dpdown\n"
    "# dpleft dpright (the left stick moves too).\n"
    "\n"
    "[video]\n"
    "scale = 3\n"
    "fullscreen = 0\n"
    "# picture: console (as the NES draws it), hd (drawn from the game's state;\n"
    "# the two options below need it and turn it on) or scaffold\n"
    "picture = console\n"
    "# no_flicker: 1 draws every sprite every frame (the NES shows 8 per line)\n"
    "no_flicker = 0\n"
    "# wide: 0, 1 for 16:9 (426 x 240), or a width from 256 to 512\n"
    "wide = 0\n"
    "\n"
    "[audio]\n"
    "enabled = 1\n"
    "\n"
    "[game]\n"
    "# slowdown: empty keeps the game's own setting (the console's slowdown);\n"
    "# otherwise the value of the --slowdown option. The command line and the\n"
    "# MFF_SLOWDOWN environment variable win over this.\n"
    "slowdown =\n"
    "\n"
    "[keys]\n"
    "a = X\n"
    "b = Z\n"
    "start = Return\n"
    "select = Right Shift\n"
    "up = Up\n"
    "down = Down\n"
    "left = Left\n"
    "right = Right\n"
    "# hold to run fast (no sound)\n"
    "fast = Tab\n"
    "fullscreen = F11\n"
    "# save states: save to the slot, load it, next slot (1-4); files in saves/\n"
    "save_state = F5\n"
    "next_slot = F6\n"
    "load_state = F7\n"
    "quit = Escape\n"
    "\n"
    "[pad]\n"
    "a = b, y\n"
    "b = a, x\n"
    "start = start\n"
    "select = back\n"
    "up = dpup\n"
    "down = dpdown\n"
    "left = dpleft\n"
    "right = dpright\n";

static void set(const char *key, const char *value) {
    int i;
    for (i = 0; i < s_n; i++)
        if (!strcmp(s_ent[i].key, key)) break;
    if (i == s_n) {
        if (s_n == MAX_ENTRIES) return;
        s_n++;
        snprintf(s_ent[i].key, sizeof s_ent[i].key, "%s", key);
    }
    snprintf(s_ent[i].value, sizeof s_ent[i].value, "%s", value);
}

static char *trim(char *p) {
    char *e;
    while (*p && isspace((unsigned char)*p)) p++;
    e = p + strlen(p);
    while (e > p && isspace((unsigned char)e[-1])) *--e = 0;
    return p;
}

static void parse(const char *text) {
    char section[64] = "", line[512];
    const char *p = text;
    while (*p) {
        size_t n = strcspn(p, "\n");
        char *l, *eq;
        if (n >= sizeof line) n = sizeof line - 1;
        memcpy(line, p, n);
        line[n] = 0;
        p += strcspn(p, "\n");
        if (*p) p++;
        l = trim(line);
        if (!*l || *l == '#' || *l == ';') continue;
        if (*l == '[') {
            char *e = strchr(l, ']');
            if (e) *e = 0;
            snprintf(section, sizeof section, "%s", trim(l + 1));
            for (char *c = section; *c; c++) *c = (char)tolower((unsigned char)*c);
            continue;
        }
        eq = strchr(l, '=');
        if (!eq) continue;
        *eq = 0;
        {
            char key[64 + 64];
            char *k = trim(l);
            for (char *c = k; *c; c++) *c = (char)tolower((unsigned char)*c);
            snprintf(key, sizeof key, "%s.%s", section, k);
            set(key, trim(eq + 1));
        }
    }
}

void config_load(const char *path, int write_default) {
    FILE *f;
    s_n = 0;
    parse(k_default);
    if (path) snprintf(s_path, sizeof s_path, "%s", path);
    else snprintf(s_path, sizeof s_path, "%s/MightyFinalFight.ini", setup_data_dir());
    f = fopen(s_path, "rb");
    if (f) {
        static char text[64 * 1024];
        size_t n = fread(text, 1, sizeof text - 1, f);
        fclose(f);
        text[n] = 0;
        for (size_t i = 0; i < n; i++)
            if (text[i] == '\r') text[i] = ' ';
        parse(text);
    } else if (write_default) {
        f = fopen(s_path, "wb");
        if (f) {
            fputs(k_default, f);
            fclose(f);
            fprintf(stderr, "[mff] settings written to %s\n", s_path);
        }
    }
    /* the slowdown option goes to the game as its environment variable */
    {
        const char *slow = config_get("game.slowdown"), *env = getenv("MFF_SLOWDOWN");
        if (slow && *slow && !(env && *env)) {
#ifdef _WIN32
            _putenv_s("MFF_SLOWDOWN", slow);
#else
            setenv("MFF_SLOWDOWN", slow, 0);
#endif
        }
    }
}

const char *config_get(const char *key) {
    for (int i = 0; i < s_n; i++)
        if (!strcmp(s_ent[i].key, key)) return s_ent[i].value;
    return NULL;
}

int config_int(const char *key, int def) {
    const char *v = config_get(key);
    char *end;
    long n;
    if (!v || !*v) return def;
    n = strtol(v, &end, 10);
    return end == v ? def : (int)n;
}

const char *config_path(void) { return s_path; }
