/*
 * savestate.c - native save states (P8)
 *
 * A state is everything the runtime and the game keep, written field by
 * field (little endian, doubles as their IEEE bits): the CPU registers and
 * RAM, cartridge RAM and the pads (bus.c), the PPU (ppu_regs.c), the MMC3,
 * the APU and the synthesizer (apu.c), the clock (timing.c), the last
 * pictures (frame.c), the dispatcher's JMP tails and code window
 * (dispatch_rt.c), the coroutines (coroutine.c) and the test hooks.
 *
 * The tasks' C stacks. Each task of the game runs on a C stack of its own
 * (coroutine.c), and the chain of C frames on it cannot be written to a
 * file portably: return addresses, saved registers and locals of compiled
 * code. So a state is taken only where that chain is known and can be
 * built again:
 *   - the frame boundary is at the scheduler's loop top ($FEBD) with no
 *     interrupt running, so the scheduler's own stack is "the scheduler
 *     about to scan", which func_FEBD starts again (its first charge, 2
 *     cycles, is taken back: timing_resume_at_scheduler);
 *   - only task 0 has a stack (the game never starts another), and it waits
 *     in task_wait_frame called from gameplay_frame_end ($C9B0 on its 6502
 *     stack): the end of a gameplay frame, the continue screen, a pause or
 *     a boss's talk (flow_gameplay.c);
 *   - below that, task 0's C stack is a chain of JMP tails, all listed in
 *     dispatch_rt.c with their target and S: the last gameplay loop
 *     ($C8ED) at task_wait_frame's S + 4, and above it only the frame end's
 *     own entries ($C93B, $C987, $C99D, $C9A7) at that S.
 * Loading builds task 0 a new stack: a frame per listed tail that does what
 * call_by_address_tail's loop does (run its target again on a lap back to
 * it), and in the last one the rest of the frame end
 * (gameplay_resume_frame_end): what gameplay_frame_end_work does after
 * task_wait_frame returns, then gameplay's own loop or its JMP back to it.
 * The tails below the last gameplay loop belong to areas played before (each
 * new area JMPs to gameplay from inside a JSR); a return into one of them,
 * which no route makes, stops the game with a message rather than going on
 * differently.
 *
 * A save asked for elsewhere (a cutscene, the title, a round's map) waits
 * for the first frame where it can be made; one that cannot be made within
 * SAVE_PATIENCE frames is dropped. A load happens at the end of the frame it
 * is asked in: the scheduler's stack is left with longjmp back to
 * savestate_run, the old task stack is freed and the state goes on from
 * there. port/tests/savestate_check.sh: save at frame N, load in a new
 * process, the rest of the frame log identical to an uninterrupted run.
 *
 * Window: F5 saves to the slot, F7 loads it, F6 picks the next slot (1-4);
 * files in saves/ of the data directory (setup.c). Tests: MFF_SAVE_AT=F1,F2
 * (and MFF_SAVE_DIR) save at the first possible frame from each Fk on;
 * --load FILE starts from a state; MFF_LOAD_AT=F:FILE loads one at frame F
 * as F7 does (the running game's stacks left behind).
 */
#include "machine.h"
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATE_MAGIC "MFFSTATE"
#define STATE_VERSION 1u
#define SAVE_PATIENCE 600               /* frames a save waits for a place it can be made */
#define SLOTS 4

/* the game's resume point (flow_gameplay.c) */
#define FRAME_END_RET 0xC9B0            /* gameplay_frame_end_work's JSR task_wait_frame */
#define GAMEPLAY 0xC8ED
#define SCHEDULER_TOP 0xFEBD
void func_FEBD(void);
void gameplay_resume_frame_end(int inline_loop);

/* ---- the serializer ---- */

struct StateIO {
    uint8_t *buf;
    size_t len, cap, pos;
    int loading, error;
};

static void io_put(StateIO *io, const void *p, size_t n) {
    if (io->len + n > io->cap) {
        size_t cap = io->cap ? io->cap * 2 : (1u << 20);
        uint8_t *b;
        while (cap < io->len + n) cap *= 2;
        b = (uint8_t *)realloc(io->buf, cap);
        if (!b) {
            io->error = 1;
            return;
        }
        io->buf = b;
        io->cap = cap;
    }
    memcpy(io->buf + io->len, p, n);
    io->len += n;
}

static void io_get(StateIO *io, void *p, size_t n) {
    if (io->pos + n > io->len) {
        io->error = 1;
        memset(p, 0, n);
        return;
    }
    memcpy(p, io->buf + io->pos, n);
    io->pos += n;
}

int st_loading(const StateIO *io) { return io->loading; }

void st_bytes(StateIO *io, void *p, size_t n) {
    if (io->loading) io_get(io, p, n);
    else io_put(io, p, n);
}

static void st_le(StateIO *io, uint64_t *v, int bytes) {
    uint8_t b[8];
    if (io->loading) {
        io_get(io, b, (size_t)bytes);
        *v = 0;
        for (int i = bytes - 1; i >= 0; i--) *v = (*v << 8) | b[i];
    } else {
        for (int i = 0; i < bytes; i++) b[i] = (uint8_t)(*v >> (8 * i));
        io_put(io, b, (size_t)bytes);
    }
}

void st_u8(StateIO *io, uint8_t *v) { st_bytes(io, v, 1); }
void st_u16(StateIO *io, uint16_t *v) { uint64_t x = *v; st_le(io, &x, 2); *v = (uint16_t)x; }
void st_u32(StateIO *io, uint32_t *v) { uint64_t x = *v; st_le(io, &x, 4); *v = (uint32_t)x; }
void st_u64(StateIO *io, uint64_t *v) { st_le(io, v, 8); }
void st_int(StateIO *io, int *v) {
    uint64_t x = (uint32_t)*v;
    st_le(io, &x, 4);
    *v = (int)(int32_t)(uint32_t)x;
}
void st_f64(StateIO *io, double *v) {
    uint64_t x;
    memcpy(&x, v, 8);
    st_le(io, &x, 8);
    memcpy(v, &x, 8);
}
void st_u16s(StateIO *io, uint16_t *v, int n) { for (int i = 0; i < n; i++) st_u16(io, &v[i]); }
void st_ints(StateIO *io, int *v, int n) { for (int i = 0; i < n; i++) st_int(io, &v[i]); }

/* the bot (port/scaffold/bot.c) keeps plain C statics */
void mff_bot_state(void (*field)(void *ctx, void *p, size_t n), void *ctx);
static void bot_field(void *ctx, void *p, size_t n) { st_bytes((StateIO *)ctx, p, n); }

/* every part, always in this order */
static void all_state(StateIO *io) {
    bus_state(io);
    ppu_state(io);
    mmc3_state(io);
    apu_state(io);
    timing_state(io);
    frame_state(io);
    dispatch_state(io);
    coroutine_state(io);
    hooks_state(io);
    mff_bot_state(bot_field, io);
}

static uint64_t fnv64(const uint8_t *p, size_t n) {
    uint64_t h = 1469598103934665603ull;
    for (size_t i = 0; i < n; i++) {
        h ^= p[i];
        h *= 1099511628211ull;
    }
    return h;
}

/* ---- where a state can be taken ---- */

static uint16_t s_boundary_pc;
static int s_boundary_depth;

void savestate_frame_begin(void) {
    s_boundary_pc = timing_pc();
    s_boundary_depth = timing_depth();
}

/* The chain check above (task_mask: which tasks have a C stack); *inner is
 * the last gameplay loop's tail, *inline_loop whether the frame end runs
 * inside it with no tail of its own. NULL if the state can be rebuilt. */
static const char *chain_check(int task_mask, int *inner, int *inline_loop) {
    int n = dispatch_tail_count(), j = -1;
    uint8_t s0 = g_ram[0x82], loop_s = (uint8_t)(s0 + 4);
    if (task_mask != 1) return "not only task 0 has a stack";
    if (g_ram[0x80] != 1 && g_ram[0x80] != 4) return "task 0 is not waiting";
    for (int t = 1; t < 4; t++)
        if (g_ram[0x80 + 4 * t] == 2) return "a task is running";
    if (s0 > 0xFB || g_ram[0x100 + s0 + 3] != (FRAME_END_RET & 0xFF) ||
        g_ram[0x100 + s0 + 4] != (FRAME_END_RET >> 8))
        return "task 0 is not at a gameplay frame end";
    if (dispatch_lap_pending() || timing_in_irq()) return "a jump is being unwound";
    for (int i = 0; i < n; i++)
        if (dispatch_tail_addr(i) == GAMEPLAY) j = i;
    if (j < 0 || dispatch_tail_s(j) != loop_s) return "no gameplay loop at the frame end's stack";
    for (int i = j + 1; i < n; i++) {
        uint16_t a = dispatch_tail_addr(i);
        if ((a != 0xC93B && a != 0xC987 && a != 0xC99D && a != 0xC9A7) || dispatch_tail_s(i) != loop_s)
            return "another routine is between the gameplay loop and its frame end";
    }
    *inner = j;
    *inline_loop = j == n - 1;
    return NULL;
}

/* at the end of a frame: can a state be taken here? */
static const char *resumable(int boundary_ok, int *inner, int *inline_loop) {
    if (!boundary_ok) return "the frame did not end at the scheduler";
    if (!coroutine_on_scheduler()) return "the frame ended on a task's stack";
    return chain_check(coroutine_task_mask(), inner, inline_loop);
}

/* ---- saving ---- */

static int s_slot = 1;
static int s_save_slot, s_save_wait, s_load_slot;
static long s_load_at = -1;             /* MFF_LOAD_AT: a load as F7 makes it, for the tests */
static char s_save_path[1200], s_load_path[1200];
static long s_test_at[64];
static int s_ntest, s_test_next;
static const char *s_test_dir;
static char s_status[256];

static void slot_path(char *out, size_t cap, int slot) {
    char dir[1100];
    snprintf(dir, sizeof dir, "%s/saves", setup_data_dir());
    mff_make_dirs(dir);
    snprintf(out, cap, "%s/slot%d.mffstate", dir, slot);
}

const char *savestate_status(void) { return s_status; }
void savestate_next_slot(void) {
    s_slot = s_slot % SLOTS + 1;
    snprintf(s_status, sizeof s_status, "slot %d", s_slot);
}

void savestate_request_save(void) {
    s_save_slot = s_slot;
    s_save_wait = SAVE_PATIENCE;
    slot_path(s_save_path, sizeof s_save_path, s_slot);
    snprintf(s_status, sizeof s_status, "slot %d: saving...", s_slot);
}

void savestate_request_load(void) {
    s_load_slot = s_slot;
    slot_path(s_load_path, sizeof s_load_path, s_slot);
}

static int write_state(const char *path) {
    StateIO io;
    uint8_t head[32];
    uint64_t h;
    FILE *f;
    int ok;
    memset(&io, 0, sizeof io);
    all_state(&io);
    if (io.error) {
        free(io.buf);
        return -1;
    }
    h = fnv64(io.buf, io.len);
    memcpy(head, STATE_MAGIC, 8);
    for (int i = 0; i < 4; i++) head[8 + i] = (uint8_t)(STATE_VERSION >> (8 * i));
    for (int i = 0; i < 4; i++) head[12 + i] = (uint8_t)((uint32_t)io.len >> (8 * i));
    for (int i = 0; i < 8; i++) head[16 + i] = (uint8_t)(g_frame_count >> (8 * i));
    for (int i = 0; i < 8; i++) head[24 + i] = (uint8_t)(h >> (8 * i));
    f = fopen(path, "wb");
    ok = f && fwrite(head, 1, sizeof head, f) == sizeof head && fwrite(io.buf, 1, io.len, f) == io.len;
    if (f && fclose(f)) ok = 0;
    free(io.buf);
    return ok ? 0 : -1;
}

/* ---- loading ---- */

static jmp_buf s_main_jmp;
static int s_main_jmp_set;
static uint8_t *s_loaded;
static size_t s_loaded_len;
static int s_inner, s_inline;

static uint64_t le(const uint8_t *p, int n) {
    uint64_t v = 0;
    for (int i = n - 1; i >= 0; i--) v = (v << 8) | p[i];
    return v;
}

/* the file, checked; 0 and s_loaded set, or -1 with the reason */
static int read_state(const char *path, char *why, size_t cap) {
    FILE *f = fopen(path, "rb");
    uint8_t head[32];
    uint32_t len;
    if (!f) {
        snprintf(why, cap, "no state in %s", path);
        return -1;
    }
    if (fread(head, 1, 32, f) != 32 || memcmp(head, STATE_MAGIC, 8) != 0) {
        fclose(f);
        snprintf(why, cap, "%s is not a save state", path);
        return -1;
    }
    if (le(head + 8, 4) != STATE_VERSION) {
        fclose(f);
        snprintf(why, cap, "%s is a state of format %u, this build reads %u", path,
                 (unsigned)le(head + 8, 4), STATE_VERSION);
        return -1;
    }
    len = (uint32_t)le(head + 12, 4);
    free(s_loaded);
    s_loaded = (uint8_t *)malloc(len ? len : 1);
    if (!s_loaded || fread(s_loaded, 1, len, f) != len || fnv64(s_loaded, len) != le(head + 24, 8)) {
        fclose(f);
        free(s_loaded);
        s_loaded = NULL;
        snprintf(why, cap, "%s is damaged", path);
        return -1;
    }
    fclose(f);
    s_loaded_len = len;
    return 0;
}

/* task 0's new stack: the tails, and the frame end in the last loop */
static void resume_frame_end(void) { gameplay_resume_frame_end(s_inline); }
static void task0_rebuilt(void) {
    dispatch_rebuild_tail(s_inner, resume_frame_end);
}

static void resume_loaded(void) {
    StateIO io;
    const char *why;
    memset(&io, 0, sizeof io);
    io.buf = s_loaded;
    io.len = s_loaded_len;
    io.loading = 1;
    coroutine_drop_all();
    all_state(&io);
    if (io.error || io.pos != io.len) mff_fatal("the save state does not fit this build (%zu of %zu bytes read)",
                                                io.pos, io.len);
    free(s_loaded);
    s_loaded = NULL;
    /* checked when it was saved; again, for what the new stacks are made of */
    why = chain_check(coroutine_saved_task_mask(), &s_inner, &s_inline);
    if (why) mff_fatal("the save state cannot be resumed: %s", why);
    coroutine_rebuild(0, task0_rebuilt);
    timing_resume_at_scheduler();
    video_after_load();
    printf("[mff] state loaded: frame %llu\n", (unsigned long long)g_frame_count);
    fflush(stdout);
    func_FEBD();                        /* the scheduler goes on; never returns */
    mff_fatal("the scheduler returned");
}

void savestate_frame_end(void) {
    int inner, inl, boundary_ok;
    const char *why;
    if (!s_save_slot && !s_load_slot && s_test_next >= s_ntest && s_load_at < 0) return;
    if (s_load_at >= 0 && (long)g_frame_count >= s_load_at) {
        s_load_at = -1;
        s_load_slot = -1;               /* s_load_path holds the file */
    }
    boundary_ok = s_boundary_pc == SCHEDULER_TOP && s_boundary_depth == 1;
    why = resumable(boundary_ok, &inner, &inl);
    if (s_test_next < s_ntest && (long)g_frame_count >= s_test_at[s_test_next] && !why) {
        char path[1200];
        snprintf(path, sizeof path, "%s/state_%llu.mffstate", s_test_dir,
                 (unsigned long long)g_frame_count);
        if (write_state(path)) mff_fatal("cannot write %s", path);
        printf("[mff] state saved: %s (asked for at frame %ld)\n", path, s_test_at[s_test_next]);
        fflush(stdout);
        while (s_test_next < s_ntest && s_test_at[s_test_next] <= (long)g_frame_count) s_test_next++;
    }
    if (s_save_slot) {
        if (!why) {
            if (write_state(s_save_path)) snprintf(s_status, sizeof s_status, "slot %d: cannot write", s_save_slot);
            else snprintf(s_status, sizeof s_status, "slot %d saved", s_save_slot);
            fprintf(stderr, "[mff] %s (%s)\n", s_status, s_save_path);
            s_save_slot = 0;
        } else if (--s_save_wait <= 0) {
            snprintf(s_status, sizeof s_status, "slot %d: not saved (save during play)", s_save_slot);
            fprintf(stderr, "[mff] %s: %s\n", s_status, why);
            s_save_slot = 0;
        }
    }
    if (s_load_slot && coroutine_on_scheduler() && boundary_ok) {
        char err[1300];
        int slot = s_load_slot;
        s_load_slot = 0;
        if (read_state(s_load_path, err, sizeof err)) {
            snprintf(s_status, sizeof s_status, "slot %d: nothing to load", slot < 0 ? 0 : slot);
            fprintf(stderr, "[mff] %s\n", err);
            return;
        }
        if (slot > 0) snprintf(s_status, sizeof s_status, "slot %d loaded", slot);
        if (!s_main_jmp_set) mff_fatal("no place to load a state from");
        longjmp(s_main_jmp, 1);
    }
}

void savestate_init(void) {
    const char *at = getenv("MFF_SAVE_AT"), *dir = getenv("MFF_SAVE_DIR");
    s_test_dir = dir && *dir ? dir : ".";
    for (const char *p = at; p && *p && s_ntest < 64;) {
        char *end;
        long v = strtol(p, &end, 10);
        if (end == p) break;
        s_test_at[s_ntest++] = v;
        p = *end ? end + 1 : end;
    }
    snprintf(s_status, sizeof s_status, "slot %d", s_slot);
    at = getenv("MFF_LOAD_AT");         /* FRAME:FILE */
    if (at && *at && strchr(at, ':')) {
        s_load_at = strtol(at, NULL, 10);
        snprintf(s_load_path, sizeof s_load_path, "%s", strchr(at, ':') + 1);
    }
}

void savestate_run(const char *load_path) {
    if (load_path) {
        char err[1300];
        if (read_state(load_path, err, sizeof err)) mff_fatal("%s", err);
    }
    s_main_jmp_set = 1;
    if (setjmp(s_main_jmp) == 0 && !s_loaded) {
        maybe_trigger_vblank(7);        /* the 6502's reset sequence, before the first instruction */
        func_RESET();
        mff_fatal("the reset routine returned");
    }
    resume_loaded();
}
