/*
 * oracle_capture.c - ground truth for synth_2a03: linked into a copy of a
 * finished cycle-runner build (the oracle's own object files, unchanged; see
 * port/tests/synth/CMakeLists.txt), it records what the APU saw, cycle by
 * cycle, while the program runs as usual (--wav-out and all).
 *
 *   MFF_APU_CAPTURE=FILE  <exe> ROM --frames N --hash-out H [--wav-out W] [...]
 *
 * --hash-out is needed: it turns the runner's trace on, and the hook below is
 * the trace's per-cycle observer. Lines of FILE (cycles are APU clocks:
 * cyc_cycle_count() after the cycle, so 1 is the first CPU cycle after
 * power-on, and a write stamped T is seen first by APU clock T):
 *
 *   S T N b0 b1 ...    the runner's APU state (N bytes) before clock T, the
 *                      first clock of the run (after power-on or a state load)
 *   F f T              frame f begins with clock T
 *   W T ADDR VV        CPU write to $4000-$4017
 *   V T ADDR VV        the same, for a write already inside the S state
 *                      (the run's very first cycle)
 *   R T VV             CPU read of $4015 and the value it got
 *   L T o0 o1 o2 o3 o4 the five channel outputs after clock T (only for
 *                      MFF_APU_LEVELS=A:B, clocks A..B, and not on cycles
 *                      that write an APU register). The tone generators only
 *                      run while the runner's audio is on: use --wav-out.
 *   E T                the run ended after clock T
 *
 * Builds with MSVC only (the hook is installed before main through the CRT's
 * initializer table).
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cyc_trace.h"

/* From the runner (cyc_core.h, hw_internal.h, cyc_ring.c). */
uint64_t cyc_cycle_count(void);
void    *apu_state_ptr(size_t *size);
void     apu_channel_levels(uint8_t out[5]);
extern uint32_t cyc_ring_frame;

static FILE    *capture;
static bool     started;
static uint32_t frame_seen = UINT32_MAX;
static uint64_t levels_from = 1, levels_to = 0;

static void on_cycle(const CycTraceCycle *c)
{
    /* This cycle's APU clock normally comes after the hook (cyc_cycle_count()
     * is the clocks run so far). A PPU register access runs the first master
     * ticks of its cycle during the access, the APU clock (tick 0) included:
     * then it has already run. The APU's put/get flag (HwApu byte 0, 1 on even
     * clocks, toggled by each) tells which. APU register accesses never do it,
     * so W, V and R stamps are the clock right after the access either way. */
    size_t n;
    const uint8_t *apu = (const uint8_t *)apu_state_ptr(&n);
    uint64_t done = cyc_cycle_count();
    if (apu[0] != (((done + 1) & 1) == 0)) done++;
    uint64_t t = done + 1;
    bool apu_write = !c->dma && c->kind == 'W' && c->addr >= 0x4000 && c->addr <= 0x4017;
    char write_tag = 'W';
    if (!started) {
        started = true;
        fprintf(capture, "S %llu %u", (unsigned long long)t, (unsigned)n);
        for (size_t i = 0; i < n; i++) fprintf(capture, " %02X", apu[i]);
        fputc('\n', capture);
        write_tag = 'V';
    }
    if (cyc_ring_frame != frame_seen) {
        frame_seen = cyc_ring_frame;
        fprintf(capture, "F %u %llu\n", frame_seen, (unsigned long long)t);
    }
    /* The hook runs after the cycle's bus access: on a cycle that wrote an APU
     * register the runner's state already holds the write, so its levels are
     * not those of clock `done`. */
    if (done >= levels_from && done <= levels_to && !apu_write) {
        uint8_t o[5];
        apu_channel_levels(o);
        fprintf(capture, "L %llu %u %u %u %u %u\n", (unsigned long long)done, o[0], o[1], o[2], o[3], o[4]);
    }
    if (apu_write)
        fprintf(capture, "%c %llu %04X %02X\n", write_tag, (unsigned long long)t, c->addr, c->value);
    else if (!c->dma && c->kind == 'R' && c->addr == 0x4015)
        fprintf(capture, "R %llu %02X\n", (unsigned long long)t, c->value);
}

static void capture_end(void)
{
    if (!capture) return;
    fprintf(capture, "E %llu\n", (unsigned long long)cyc_cycle_count());   /* the last APU clock run */
    fclose(capture);
    capture = NULL;
}

static void __cdecl capture_start(void)
{
    const char *path = getenv("MFF_APU_CAPTURE");
    if (!path || !*path) return;
    capture = fopen(path, "w");
    if (!capture) {
        fprintf(stderr, "MFF_APU_CAPTURE: cannot write %s\n", path);
        return;
    }
    setvbuf(capture, NULL, _IOFBF, 1 << 20);
    const char *levels = getenv("MFF_APU_LEVELS");
    unsigned long long a, b;
    if (levels && sscanf(levels, "%llu:%llu", &a, &b) == 2) levels_from = a, levels_to = b;
    cyc_trace_cycle_hook = on_cycle;
    atexit(capture_end);
}

#pragma section(".CRT$XCU", read)
__declspec(allocate(".CRT$XCU")) void(__cdecl *mff_apu_capture_init)(void) = capture_start;
#pragma comment(linker, "/include:mff_apu_capture_init")
