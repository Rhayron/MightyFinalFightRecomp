/*
 * extras.c — Mighty Final Fight legacy-scaffold hooks (PLAN_DECOMP.md P3)
 * Adapted from YoshisCookieRecomp's extras.c
 * Implements game_extras.h.
 * Features: TCP debug server, verify mode via Nestopia oracle.
 */
#include "game_extras.h"
#include "nes_runtime.h"
#include "debug_server.h"
#include "verify_mode.h"
#ifdef ENABLE_NESTOPIA_ORACLE
#include "nestopia_bridge.h"
#endif
#include <SDL.h>
#include <stdio.h>
#include <string.h>

/* Globals expected by the runner framework */
const char *g_rom_path_for_extras = NULL;
int         g_watchdog_triggered  = 0;
uint32_t    g_watchdog_frame      = 0;
const char *g_watchdog_stack_dump = "";

/* CRC32 of the ROM after its iNES header (launcher.c verify_rom) */
uint32_t game_get_expected_crc32(void) { return 0x3F78037Cu; }

const char *game_get_name(void) { return "Mighty Final Fight (legacy scaffold)"; }

void game_on_init(void) {
    int port = (g_run_mode == RUN_MODE_EMULATED) ? 4371 : 4370;
    debug_server_init(port);

    if (g_run_mode != RUN_MODE_NATIVE && g_rom_path_for_extras) {
        verify_mode_init(g_rom_path_for_extras);
    }

    /* Auto-register followers on stack-page addresses for VRAM buffer
     * investigation — disabled: legacy debug_server_add_follower API
     * removed at nesrecomp commit 69ecd30. Tier 2.5 replacement is
     * rdb_watch_add (TCP-driven). Re-port when the migration is done. */
#if 0
    debug_server_add_follower(0x0100, -1);
    debug_server_add_follower(0x0101, -1);
    debug_server_add_follower(0x0102, -1);
    debug_server_add_follower(0x0103, -1);
    debug_server_add_follower(0x0105, -1);
    debug_server_add_follower(0x0107, -1);
    debug_server_add_follower(0x0108, -1);
#endif
}

#include <stdlib.h>
/* MFF_TRACE=N: print S, the four tasks ($80-$8F), task_current, render_off_depth
 * and bank_switch_busy at the first N frames (P3 scaffold debugging). */
/* MFF_FRAMELOG=FILE: one record per frame for the lockstep check
 * (port/tests/lockstep.py): u32 frame, CPU RAM [0x800], OAM [0x100],
 * palette [0x20]. */
static void frame_log(uint64_t frame_count) {
    static FILE *f = NULL;
    static int state = 0;               /* 0 unknown, 1 on, -1 off */
    if (!state) {
        const char *e = getenv("MFF_FRAMELOG");
        f = (e && *e) ? fopen(e, "wb") : NULL;
        state = f ? 1 : -1;
    }
    if (state < 0) return;
    uint32_t fr = (uint32_t)frame_count;
    fwrite(&fr, 4, 1, f);
    fwrite(g_ram, 1, 0x800, f);
    fwrite(g_ppu_oam, 1, 0x100, f);
    fwrite(g_ppu_pal, 1, 0x20, f);
    fflush(f);
}

/* MFF_INPUT=FILE (port/tests/nmi_input.py): `count rep buttons` lines keyed
 * like port/tests/lockstep.py pairs frames - NMI count since reset ($92
 * unwrapped from its first 0 -> 1 step) and the n-th frame showing it - so
 * the scaffold's pads match the oracle's whatever each runner counts as a
 * frame. Applied here, after the runner set the pads and before the NMI. */
typedef struct { uint32_t count; uint16_t rep; uint8_t mask; } KeyedInput;
static KeyedInput *s_in;
static size_t s_in_n, s_in_at;

static void keyed_input(void) {
    static int state = 0;               /* 0 unknown, 1 on, -1 off */
    static int started = 0, prev = -1;
    static uint32_t count = 0;
    static uint16_t rep = 0;
    if (!state) {
        const char *e = getenv("MFF_INPUT");
        FILE *f = (e && *e) ? fopen(e, "r") : NULL;
        state = -1;
        if (f) {
            size_t cap = 1 << 16;
            s_in = (KeyedInput *)malloc(cap * sizeof(*s_in));
            unsigned c, r, m;
            while (fscanf(f, "%u %u %x", &c, &r, &m) == 3) {
                if (s_in_n == cap) s_in = (KeyedInput *)realloc(s_in, (cap *= 2) * sizeof(*s_in));
                s_in[s_in_n].count = c; s_in[s_in_n].rep = (uint16_t)r; s_in[s_in_n].mask = (uint8_t)m;
                s_in_n++;
            }
            fclose(f);
            state = 1;
            fprintf(stderr, "[mff] MFF_INPUT: %zu keyed frames\n", s_in_n);
        }
    }
    if (state < 0) return;
    int v = g_ram[0x92];
    if (!started) {
        if (prev == 0 && v == 1) { started = 1; count = 1; rep = 0; }
        prev = v;
        if (!started) { g_controller1_buttons = 0; return; }
    } else {
        uint32_t next = count + (uint32_t)((v - prev) & 0xFF);
        rep = (next == count) ? (uint16_t)(rep + 1) : 0;
        count = next;
        prev = v;
    }
    while (s_in_at < s_in_n && (s_in[s_in_at].count < count ||
           (s_in[s_in_at].count == count && s_in[s_in_at].rep < rep)))
        s_in_at++;
    if (s_in_at < s_in_n && s_in[s_in_at].count == count)
        g_controller1_buttons = s_in[s_in_at].mask;
    else if (s_in_at > 0)
        g_controller1_buttons = s_in[s_in_at - 1].mask;   /* more repeats here than in the oracle */
}

void game_on_frame(uint64_t frame_count) {
    frame_log(frame_count);
    keyed_input();
    static long limit = -2;
    if (limit == -2) { const char *e = getenv("MFF_TRACE"); limit = e ? atol(e) : -1; }
    if (limit < 0 || (long)frame_count >= limit) return;
    fprintf(stderr, "[mff] f=%llu S=%02X tasks", (unsigned long long)frame_count, g_cpu.S);
    for (int t = 0; t < 4; t++)
        fprintf(stderr, " %02X/%02X/%02X%02X", g_ram[0x80 + 4*t], g_ram[0x81 + 4*t], g_ram[0x83 + 4*t], g_ram[0x82 + 4*t]);
    fprintf(stderr, " cur=%02X EE=%02X F6=%02X 92=%02X\n", g_ram[0x91], g_ram[0xEE], g_ram[0xF6], g_ram[0x92]);
}

void game_post_nmi(uint64_t frame_count) { (void)frame_count; }

int game_handle_arg(const char *key, const char *val) {
    if (strcmp(key, "--verify") == 0) {
        g_run_mode = RUN_MODE_VERIFY;
        return 1;
    }
    if (strcmp(key, "--emulated") == 0) {
        g_run_mode = RUN_MODE_EMULATED;
        return 1;
    }
    (void)val;
    return 0;
}

const char *game_arg_usage(void) {
    return "  --verify     Run both native + Nestopia, compare each frame\n"
           "  --emulated   Run Nestopia only (reference mode)\n";
}

void game_run_nmi(void) {
    verify_mode_run_nmi();
}

void game_run_main(void) {
    if (g_run_mode == RUN_MODE_EMULATED) {
#ifdef ENABLE_NESTOPIA_ORACLE
        printf("[Emulated] Nestopia driving main loop\n");
        static uint32_t emu_argb[256 * 240];
        extern void runner_present_framebuf(const uint32_t *argb_buf);

        for (;;) {
            SDL_Event ev;
            while (SDL_PollEvent(&ev)) {
                if (ev.type == SDL_QUIT) exit(0);
                if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE) exit(0);
            }
            const uint8_t *keys = SDL_GetKeyboardState(NULL);
            uint8_t btn = 0;
            if (keys[SDL_SCANCODE_Z])      btn |= 0x80;
            if (keys[SDL_SCANCODE_X])      btn |= 0x40;
            if (keys[SDL_SCANCODE_TAB])    btn |= 0x20;
            if (keys[SDL_SCANCODE_RETURN]) btn |= 0x10;
            if (keys[SDL_SCANCODE_UP])     btn |= 0x08;
            if (keys[SDL_SCANCODE_DOWN])   btn |= 0x04;
            if (keys[SDL_SCANCODE_LEFT])   btn |= 0x02;
            if (keys[SDL_SCANCODE_RIGHT])  btn |= 0x01;
            g_controller1_buttons = btn;

            debug_server_poll();

            int ovr = debug_server_get_input_override();
            if (ovr >= 0) g_controller1_buttons = (uint8_t)ovr;

            nestopia_bridge_run_frame(g_controller1_buttons);
            nestopia_bridge_get_framebuf_argb(emu_argb);
            runner_present_framebuf(emu_argb);

            nestopia_bridge_get_ram(g_ram);
            g_frame_count++;
            debug_server_record_frame();

            SDL_Delay(16);
        }
#else
        fprintf(stderr, "[Error] Nestopia not compiled in\n");
        func_RESET();
#endif
    } else {
        func_RESET();
    }
}

int game_dispatch_override(uint16_t addr) {
    (void)addr;
    return 0;
}

uint8_t game_ram_read_hook(uint16_t pc, uint16_t addr, uint8_t val) {
    (void)pc; (void)addr;
    return val;
}

void game_fill_frame_record(void *record) { (void)record; }

int game_handle_debug_cmd(const char *cmd, int id, const char *json) {
    (void)cmd; (void)id; (void)json;
    return 0;
}

void game_post_render(uint32_t *framebuf) { (void)framebuf; }

/* ---- MFF: nmi_tail (0E:C119), replaced (game.base.toml [[replace_func]]) ----
 * On the console the NMI rewrites its own return address so RTI lands here
 * outside the interrupt; this code pushes P/A/X/Y, builds a return to the
 * interrupted PC ($A0/$A1 - 1) on the stack, calls sound_update ($FF72) and
 * RTSes back into the interrupted code. The legacy runner calls func_NMI
 * between frames, so the "interrupted PC" it finds on the stack is not a
 * place in the program ($FFFF -> RTS to $0000). What the tail does for the
 * game is: run sound_update, leave A/X/Y/P as they were. */
extern void func_FF72(void);
void func_C119(void) {
    uint8_t a = g_cpu.A, x = g_cpu.X, y = g_cpu.Y;
    uint8_t n = g_cpu.N, v = g_cpu.V, d = g_cpu.D, i = g_cpu.I, z = g_cpu.Z, c = g_cpu.C;
    func_FF72();
    g_cpu.A = a; g_cpu.X = x; g_cpu.Y = y;
    g_cpu.N = n; g_cpu.V = v; g_cpu.D = d; g_cpu.I = i; g_cpu.Z = z; g_cpu.C = c;
}
