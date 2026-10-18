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
#include <stdlib.h>
#include <string.h>

/* Globals expected by the runner framework */
const char *g_rom_path_for_extras = NULL;
int         g_watchdog_triggered  = 0;
uint32_t    g_watchdog_frame      = 0;
const char *g_watchdog_stack_dump = "";

/* CRC32 of the ROM after its iNES header (launcher.c verify_rom) */
uint32_t game_get_expected_crc32(void) { return 0x3F78037Cu; }

const char *game_get_name(void) { return "Mighty Final Fight (legacy scaffold)"; }

/* When the NMI may land (MFF_LAG=1 turns this off).
 *
 * Never while a task is running (state 2 in $80+4n): an NMI the runner fired
 * mid-task (its CPU time is an estimate) cut at $C3BC - inside the RNG, right
 * before the frame-end yield - and corrupted the coroutine stacks (S fell to
 * $01 at NMI 11,338 of inputs/10_cody_poweron.txt). So the scaffold plays as
 * if every frame fit; the lockstep resyncs the oracle's lag frames.
 *
 * And, around the scheduler, only at the top of its scan ($FEBD): not in the
 * rest of the scheduler, task_wait_frame's tail or the IRQ handlers. The
 * registers the NMI interrupts are those of the scheduler's loop, and the
 * sound driver (run by nmi_tail) reads one it never sets, so where in the
 * loop the NMI lands showed in its scratch ($C3). That place followed the
 * frame's exact cycle count, which P4's hand-written routines only keep per
 * block; at $FEBD the registers are always the same. Elsewhere (reset's
 * $2002 polling, which needs the frame to end) the NMI is not held. */
static int mff_nmi_veto(uint16_t pc) {
    for (int t = 0; t < 4; t++)
        if (g_ram[0x80 + 4 * t] == 2) return 1;
    if (pc >= 0xFEBA && pc <= 0xFF01) return pc != 0xFEBD;   /* scheduler */
    if (pc >= 0xFF24 && pc <= 0xFF49) return 1;               /* task_wait_frame */
    if (pc >= 0xC13B && pc <= 0xC1DE) return 1;               /* irq */
    return 0;
}

void game_on_init(void) {
    { const char *lag = getenv("MFF_LAG"); if (!lag || *lag != '1') g_nmi_veto = mff_nmi_veto; }
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

/* MFF_RESYNC=ORACLE.framelog (the cycle build's --frame-log of the same
 * route): after each frame is logged, copy the oracle's CPU RAM for the same
 * NMI key over the scaffold's, minus the bytes each runner owns (stack page,
 * the tasks' saved stack pointers, nmi_return_pc). A divergence then lasts one
 * frame instead of carrying on - expected on lag frames, where the scaffold
 * cuts the frame's work at a different instruction - so the lockstep keeps
 * checking the whole route. Keys as in keyed_input/lockstep.py. */
static void resync(void) {
    static int state = 0, started = 0, prev = -1;
    static FILE *f;
    static uint32_t count, o_count, o_last;
    static uint16_t rep, o_rep;
    static int o_started, o_have, o_prev = -1;
    static uint8_t o_ram[0x800];
    static unsigned long resyncs;
    if (!state) {
        const char *e = getenv("MFF_RESYNC");
        f = (e && *e) ? fopen(e, "rb") : NULL;
        char magic[12];
        state = (f && fread(magic, 1, 12, f) == 12 && !memcmp(magic, "CYCFRAME", 8)) ? 1 : -1;
        if (state > 0) fprintf(stderr, "[mff] MFF_RESYNC: %s\n", e);
    }
    if (state < 0) return;
    /* this frame's key */
    int v = g_ram[0x92];
    if (!started) {
        if (prev == 0 && v == 1) { started = 1; count = 1; rep = 0; }
        prev = v;
        if (!started) return;
    } else {
        uint32_t next = count + (uint32_t)((v - prev) & 0xFF);
        rep = (next == count) ? (uint16_t)(rep + 1) : 0;
        count = next;
        prev = v;
    }
    /* advance the oracle log to the same key */
    for (;;) {
        if (o_have && (o_count > count || (o_count == count && o_rep >= rep))) break;
        uint32_t head[9];
        if (fread(head, 4, 9, f) != 9) { state = -1; return; }
        fseek(f, 32 + 256, SEEK_CUR);
        if (fread(o_ram, 1, head[3], f) != head[3]) { state = -1; return; }
        fseek(f, (long)(head[4] + head[5] + head[6] + head[7] + head[8]), SEEK_CUR);
        int ov = o_ram[0x92];
        if (!o_started) {
            if (o_prev == 0 && ov == 1) { o_started = 1; o_count = 1; o_rep = 0; o_have = 1; }
            o_prev = ov;
            continue;
        }
        uint32_t next = o_count + (uint32_t)((ov - o_prev) & 0xFF);
        o_rep = (next == o_count) ? (uint16_t)(o_rep + 1) : 0;
        o_count = next;
        o_prev = ov;
        o_last = head[0];
    }
    if (o_count != count || o_rep != rep) return;      /* no oracle frame with this key */
    /* Only between frames on both sides: while a task is still running (a lag
     * frame on either side) the scaffold's C control flow is mid-task, and RAM
     * saying otherwise would desynchronise code and data. */
    if (g_ram[0x80] == 2 || o_ram[0x80] == 2) return;
    int changed = 0;
    for (int i = 0; i < 0x800; i++) {
        /* never copied: the stack page, the scheduler's task table and current
         * task ($80-$91, control flow, not data) and nmi_return_pc */
        if ((i >= 0x100 && i < 0x200) || (i >= 0x80 && i <= 0x91) || i == 0xA0 || i == 0xA1)
            continue;
        if (g_ram[i] != o_ram[i]) { g_ram[i] = o_ram[i]; changed = 1; }
    }
    if (changed && ++resyncs <= 50)
        fprintf(stderr, "[mff] resync at NMI %u (oracle frame %u)\n", (unsigned)count, (unsigned)o_last);
}

void mff_bot_frame(uint64_t frame_count);   /* bot.c, MFF_BOT */

void game_on_frame(uint64_t frame_count) {
    frame_log(frame_count);
    resync();
    keyed_input();
    mff_bot_frame(frame_count);
    /* MFF_TRACE=N (first N frames) or START:N */
    static long first = -2, limit = 0;
    if (first == -2) {
        const char *e = getenv("MFF_TRACE");
        first = -1;
        if (e) {
            const char *c = strchr(e, ':');
            if (c) { first = atol(e); limit = first + atol(c + 1); }
            else { first = 0; limit = atol(e); }
        }
    }
    if (first < 0 || (long)frame_count < first || (long)frame_count >= limit) return;
    extern const char *g_last_recomp_func;
    fprintf(stderr, "[mff] f=%llu S=%02X tasks", (unsigned long long)frame_count, g_cpu.S);
    for (int t = 0; t < 4; t++)
        fprintf(stderr, " %02X/%02X/%02X%02X", g_ram[0x80 + 4*t], g_ram[0x81 + 4*t], g_ram[0x83 + 4*t], g_ram[0x82 + 4*t]);
    fprintf(stderr, " cur=%02X EE=%02X F6=%02X 92=%02X act=%02X fn=%s\n", g_ram[0x91], g_ram[0xEE], g_ram[0xF6],
            g_ram[0x92], g_ram[0x30], g_last_recomp_func ? g_last_recomp_func : "?");
}

void game_post_nmi(uint64_t frame_count) { (void)frame_count; }

/* port/src/game/hw.h: an indexed read crossed a page. The scaffold's cycle
 * model has no such cycle (the port's runtime charges it on the console's
 * clock, port/src/runtime/timing.c). */
void nes_page_cross(void) {}

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

/* MFF_PICLOG=FILE: per rendered frame, u32 frame, u32 FNV-1a hash of the
 * picture and the low 32 bits of the CPU cycle count, so
 * port/tests/selfcheck.py also compares what the PPU code drew (VRAM
 * uploads, palette, the IRQ's mid-frame scroll split) and when. */
void game_post_render(uint32_t *framebuf) {
    static FILE *f = NULL;
    static int state = 0;
    if (!state) {
        const char *e = getenv("MFF_PICLOG");
        f = (e && *e) ? fopen(e, "wb") : NULL;
        state = f ? 1 : -1;
    }
    if (state < 0 || !framebuf) return;
    uint32_t h = 2166136261u, rec[3];
    size_t n = (size_t)g_render_width * 240;
    for (size_t i = 0; i < n; i++) { h ^= framebuf[i]; h *= 16777619u; }
    rec[0] = (uint32_t)g_frame_count;
    rec[1] = h;
    rec[2] = (uint32_t)g_nes_cycles;     /* CPU cycles so far: timing drift shows here first */
    fwrite(rec, 4, 3, f);
    fflush(f);
}

/* ---- MFF: nmi_tail (0E:C119), replaced (game.base.toml [[replace_func]]) ----
 * On the console the NMI rewrites its own return address so RTI lands here
 * outside the interrupt; this code pushes P/A/X/Y, builds a return to the
 * interrupted PC ($A0/$A1 - 1) on the stack, calls sound_update ($FF72) and
 * RTSes back into the interrupted code. The legacy runner calls func_NMI
 * between frames, so the "interrupted PC" it finds on the stack is not a
 * place in the program ($FFFF -> RTS to $0000). The tail does all but the
 * jump, as port/src/runtime/mff_hooks.c does: the same stack writes
 * (sound_update's RTS pops the JSR's return address, so S comes back where
 * it was), sound_update, A/X/Y/P as they were, and the console's cycles.
 * Until 2026-10-17 it called sound_update without the JSR's push, which
 * left S two bytes up after every NMI; the raster IRQ's interrupt frame then
 * landed on $0101/$0100 and zeroed bg_pal_anim_on, so round 3 area 2's
 * palette animation stopped (found by the P5 tours against the oracle). */
extern void func_FF72(void);
static void tail_push(uint8_t v) {
    g_ram[0x100 + g_cpu.S] = v;
    g_cpu.S--;
}
void func_C119(void) {
    uint8_t a = g_cpu.A, x = g_cpu.X, y = g_cpu.Y;
    { /* MFF_TAILTRACE=FIRST:N - registers and sound scratch at nmi_tail */
        static long first = -2, last = 0;
        if (first == -2) {
            const char *e = getenv("MFF_TAILTRACE");
            first = -1;
            if (e) { first = atol(e); const char *c = strchr(e, ':'); last = first + (c ? atol(c + 1) : 10); }
        }
        if (first >= 0 && (long)g_frame_count >= first && (long)g_frame_count < last)
            fprintf(stderr, "[tail] f=%llu A=%02X X=%02X Y=%02X P=%02X C0=%02X%02X%02X%02X cyc=%llu\n",
                    (unsigned long long)g_frame_count, a, x, y,
                    (g_cpu.N << 7) | (g_cpu.V << 6) | (g_cpu.Z << 1) | g_cpu.C,
                    g_ram[0xC0], g_ram[0xC1], g_ram[0xC2], g_ram[0xC3], (unsigned long long)g_nes_cycles);
    }
    uint8_t n = g_cpu.N, v = g_cpu.V, d = g_cpu.D, i = g_cpu.I, z = g_cpu.Z, c = g_cpu.C;
    uint8_t p = (uint8_t)((n << 7) | (v << 6) | 0x30 | (d << 3) | (i << 2) | (z << 1) | c);  /* as PHP pushes it */
    uint16_t ret = (uint16_t)(((g_ram[0xA1] << 8) | g_ram[0xA0]) - 1);
    uint8_t s;
    tail_push(p);                               /* PHP x3: room for the return address, and P */
    tail_push(p);
    tail_push(p);
    tail_push(a);                               /* PHA, TXA/PHA, TYA/PHA */
    tail_push(x);
    tail_push(y);
    s = g_cpu.S;                                /* TSX; STA $0105,X / $0106,X */
    g_ram[(0x105 + s) & 0x7FF] = (uint8_t)ret;
    g_ram[(0x106 + s) & 0x7FF] = (uint8_t)(ret >> 8);
    tail_push(0xC1);                            /* JSR $FF72: returns to $C134 */
    tail_push(0x33);
    /* the console's cycles: PHP x3, PHA/TXA/PHA/TYA/PHA, TSX, SEC, the fake
     * return (LDA zp, SBC #, STA abs,X twice) and the JSR; after it
     * PLA/TAY/PLA/TAX/PLA, PLP and RTS ($C119-$C13A) */
    maybe_trigger_vblank(52);
    func_FF72();                                /* its RTS pops $C133 */
    maybe_trigger_vblank(26);
    g_cpu.S = (uint8_t)(g_cpu.S + 6);           /* PLA/TAY, PLA/TAX, PLA, PLP, RTS */
    g_cpu.A = a; g_cpu.X = x; g_cpu.Y = y;
    g_cpu.N = n; g_cpu.V = v; g_cpu.D = d; g_cpu.I = i; g_cpu.Z = z; g_cpu.C = c;
}
