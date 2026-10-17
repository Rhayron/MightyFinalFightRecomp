/*
 * synth_selftest.c - synth_2a03 checks that need no oracle: the documented
 * timings of the frame counter, the length counters, the DMC and the status
 * register (nesdev wiki, APU pages), and that the output stage settles.
 * The exact comparisons against the cycle runner are synth_replay's.
 */
#include "synth_2a03.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int failures;

#define CHECK(cond, ...)                                    \
    do {                                                    \
        if (!(cond)) {                                      \
            printf("FAIL %s:%d: ", __FILE__, __LINE__);     \
            printf(__VA_ARGS__);                            \
            printf("\n");                                   \
            failures++;                                     \
        }                                                   \
    } while (0)

static Synth2A03 apu;

static void drain(void)
{
    int16_t buf[1024];
    while (synth_take_samples(&apu, buf, 1024) == 1024) {}
}

/* 4-step mode: the flag rises 29828 cycles after the reset, and a read
 * acknowledges it on the next put cycle. */
static void test_frame_irq(void)
{
    synth_init(&apu, 48000, NULL, NULL);
    synth_write(&apu, 0x4017, 0x00, 10);              /* put cycle: reset 3 later, counter 1 on 13 */
    uint64_t one = 13;
    CHECK(!(synth_read_status(&apu, one + 29827 - 1) & 0x40), "flag before 29828");
    CHECK(synth_read_status(&apu, one + 29827 + 1) & 0x40, "flag at 29828");
    CHECK(synth_irq(&apu), "irq line");
    /* Raised again on 29829 and 29830, so only a read after those clears it. */
    CHECK(synth_read_status(&apu, one + 29827 + 4) & 0x40, "flag raised again on 29829-29830");
    uint8_t again = synth_read_status(&apu, one + 29827 + 8);
    CHECK(!(again & 0x40), "acknowledged by the read (got %02X)", again);
    /* Inhibited: visible on 29828-29829 only. */
    synth_init(&apu, 48000, NULL, NULL);
    synth_write(&apu, 0x4017, 0x40, 10);
    CHECK(synth_read_status(&apu, one + 29828) & 0x40, "inhibited flag visible on 29828-29829");
    synth_init(&apu, 48000, NULL, NULL);
    synth_write(&apu, 0x4017, 0x40, 10);
    CHECK(!(synth_read_status(&apu, one + 29830) & 0x40), "inhibited flag gone after 29830");
    CHECK(!synth_irq(&apu), "no irq line when inhibited");
    /* 5-step: no flag at all. */
    synth_init(&apu, 48000, NULL, NULL);
    synth_write(&apu, 0x4017, 0x80, 10);
    CHECK(!(synth_read_status(&apu, one + 40000) & 0x40), "5-step mode has no frame interrupt");
}

/* Length counters: loaded by $4003 when enabled, counted down on half frames
 * (14913 and 29829 in 4-step mode), cleared by $4015. */
static void test_length(void)
{
    synth_init(&apu, 48000, NULL, NULL);
    synth_write(&apu, 0x4017, 0x00, 10);              /* counter 1 on cycle 13 */
    synth_write(&apu, 0x4003, 0x08, 20);              /* not enabled: ignored */
    CHECK(!(synth_read_status(&apu, 30) & 1), "load while disabled is ignored");
    synth_write(&apu, 0x4015, 0x0F, 40);
    synth_write(&apu, 0x4000, 0x10, 41);              /* constant volume, no halt */
    synth_write(&apu, 0x4003, 0x18, 42);              /* index 3: length 2 */
    CHECK(synth_read_status(&apu, 50) & 1, "loaded");
    CHECK(apu.length[0].count == 2, "length 2, got %u", apu.length[0].count);
    synth_run(&apu, 13 + 14912 + 1);                  /* first half frame */
    CHECK(apu.length[0].count == 1, "one half frame: %u", apu.length[0].count);
    synth_run(&apu, 13 + 29828 + 1);
    CHECK(!(synth_read_status(&apu, 13 + 29828 + 2) & 1), "expired after two half frames");
    /* Halt keeps it. */
    synth_write(&apu, 0x4000, 0x20, 13 + 29900);
    synth_write(&apu, 0x4003, 0x18, 13 + 29901);
    synth_run(&apu, 13 + 29830 + 29829 + 10);
    CHECK(apu.length[0].count == 2, "halted counter kept, got %u", apu.length[0].count);
    synth_write(&apu, 0x4015, 0x00, apu.now + 1);
    CHECK(!(synth_read_status(&apu, apu.now + 2) & 0x0F), "cleared by $4015");
}

/* A $4017 write with bit 7 clocks a quarter and a half frame at once. */
static void test_five_step_immediate(void)
{
    synth_init(&apu, 48000, NULL, NULL);
    synth_write(&apu, 0x4015, 0x01, 2);
    synth_write(&apu, 0x4000, 0x10, 3);
    synth_write(&apu, 0x4003, 0x18, 4);               /* length 2 */
    synth_run(&apu, 6);
    synth_write(&apu, 0x4017, 0x80, 8);
    synth_run(&apu, 9);
    CHECK(apu.length[0].count == 1, "half frame from the $4017 write: %u", apu.length[0].count);
}

/* The DMC plays a sample through the callback, raises its interrupt at the
 * last byte and reports bytes left in bit 4. */
static unsigned fetches;
static uint16_t last_addr;
static uint8_t dmc_read(void *user, uint16_t addr)
{
    (void)user;
    fetches++;
    last_addr = addr;
    return 0xFF;                                       /* all ones: the level climbs */
}

static void test_dmc(void)
{
    synth_init(&apu, 48000, dmc_read, NULL);
    fetches = 0;
    synth_write(&apu, 0x4010, 0x8F, 2);               /* IRQ, rate 15 (54 cycles per bit) */
    synth_write(&apu, 0x4012, 0x00, 3);               /* $C000 */
    synth_write(&apu, 0x4013, 0x01, 4);               /* 17 bytes */
    synth_write(&apu, 0x4011, 0x00, 5);
    synth_write(&apu, 0x4015, 0x10, 6);
    CHECK(synth_read_status(&apu, 7) & 0x10, "bytes left right after the start");
    synth_run(&apu, 20);
    CHECK(fetches == 1 && last_addr == 0xC000, "first fetch soon after the start (%u, %04X)", fetches, last_addr);
    synth_run(&apu, 20 + 17 * 8 * 54 + 2000);
    CHECK(fetches == 17, "17 fetches, got %u", fetches);
    uint8_t st = synth_read_status(&apu, apu.now);
    CHECK(!(st & 0x10) && (st & 0x80), "done with the interrupt flag (status %02X)", st);
    CHECK(synth_irq(&apu), "DMC irq line");
    CHECK(apu.dmc.level > 100, "level climbed to %u", apu.dmc.level);
    synth_write(&apu, 0x4015, 0x00, apu.now + 1);
    CHECK(!(synth_read_status(&apu, apu.now + 2) & 0x80), "$4015 write clears the DMC interrupt");
    /* Looping keeps fetching from the start. */
    synth_write(&apu, 0x4010, 0x4F, apu.now + 3);
    synth_write(&apu, 0x4015, 0x10, apu.now + 4);
    synth_run(&apu, apu.now + 40 * 8 * 54);
    CHECK(fetches > 17 + 30 && synth_read_status(&apu, apu.now) & 0x10, "looping sample keeps going (%u)", fetches);
}

/* Sweep mute: below period 8, or an add sweep's target above $7FF, even with
 * the sweep disabled. */
static void test_sweep_mute(void)
{
    synth_init(&apu, 48000, NULL, NULL);
    synth_write(&apu, 0x4015, 0x01, 2);
    synth_write(&apu, 0x4000, 0xBF, 3);               /* 50%, constant volume 15 */
    synth_write(&apu, 0x4001, 0x00, 4);               /* disabled, add, shift 0: target 2x period */
    synth_write(&apu, 0x4002, 0x00, 5);
    synth_write(&apu, 0x4003, 0x0C, 6);               /* period $400: target $800 mutes */
    synth_run(&apu, 2000);
    CHECK(!apu.live[0], "target above $7FF mutes");
    synth_write(&apu, 0x4001, 0x08, 2001);            /* negate: never mutes */
    synth_run(&apu, 4000);
    CHECK(apu.live[0], "negated sweep unmutes");
    synth_write(&apu, 0x4002, 0x07, 4001);
    synth_write(&apu, 0x4003, 0x08, 4002);            /* period 7 */
    synth_run(&apu, 6000);
    CHECK(!apu.live[0], "period below 8 mutes");
}

/* Silence settles to 0 through the high-pass filters; a full-volume square
 * wave comes out with the expected amplitude and no clipping. */
static void test_output(void)
{
    synth_init(&apu, 48000, NULL, NULL);
    synth_run(&apu, 1789773);                          /* one second */
    int16_t buf[2048];
    int n = 0, got;
    drain();
    synth_run(&apu, 1789773 + 89489);                  /* 50 ms more */
    got = synth_take_samples(&apu, buf, 2048);
    for (int i = 0; i < got; i++) n += buf[i] != 0;
    CHECK(got > 2000 && n == 0, "silence settles (%d samples, %d nonzero)", got, n);
    CHECK(apu.samples == (uint64_t)((1789773.0 + 89489.0) / (SYNTH_CPU_HZ / 48000)), "sample count %llu",
          (unsigned long long)apu.samples);

    synth_write(&apu, 0x4015, 0x01, apu.now);
    synth_write(&apu, 0x4000, 0xBF, apu.now + 1);
    synth_write(&apu, 0x4001, 0x08, apu.now + 2);
    synth_write(&apu, 0x4002, 0xFD, apu.now + 3);
    synth_write(&apu, 0x4003, 0x00, apu.now + 4);     /* 440 Hz */
    synth_run(&apu, apu.now + 179000);
    drain();
    synth_run(&apu, apu.now + 89489);
    got = synth_take_samples(&apu, buf, 2048);
    int lo = 0, hi = 0;
    for (int i = 0; i < got; i++) {
        if (buf[i] < lo) lo = buf[i];
        if (buf[i] > hi) hi = buf[i];
    }
    CHECK(hi > 1500 && lo < -1500 && hi < 32767 && lo > -32768, "square wave range %d..%d", lo, hi);
}

/* Throughput, worst case: every channel sounding, the noise at its shortest
 * period (a timer step every 4 cycles), the DMC looping, a write burst each
 * frame. Prints, never fails. */
static void bench(void)
{
    synth_init(&apu, 48000, dmc_read, NULL);
    static const uint8_t setup[][2] = {
        { 0x15, 0x1F }, { 0x00, 0xBF }, { 0x02, 0x40 }, { 0x03, 0x01 }, { 0x04, 0x7A }, { 0x06, 0x60 },
        { 0x07, 0x01 }, { 0x08, 0xC0 }, { 0x0A, 0x20 }, { 0x0B, 0x02 }, { 0x0C, 0x34 }, { 0x0E, 0x00 },
        { 0x0F, 0x00 }, { 0x10, 0x4F }, { 0x13, 0x20 }, { 0x15, 0x1F },
    };
    uint64_t t = 0;
    for (size_t i = 0; i < sizeof setup / sizeof setup[0]; i++) synth_write(&apu, 0x4000 | setup[i][0], setup[i][1], t += 10);
    /* A busy driver's burst: periods and volumes of the tone channels. */
    static const uint16_t burst[] = { 0x4000, 0x4002, 0x4003, 0x4004, 0x4006, 0x4007, 0x400A, 0x400B, 0x400C };
    const int frames = 60 * 60;
    clock_t start = clock();
    for (int f = 0; f < frames; f++) {
        uint64_t frame = (uint64_t)(f * 29780.5);
        for (unsigned w = 0; w < sizeof burst / sizeof burst[0]; w++) {
            uint8_t v = (uint8_t)(w * 37 + (unsigned)f);
            if (burst[w] == 0x4000 || burst[w] == 0x4004 || burst[w] == 0x400C) v |= 0x3C;   /* loud */
            if (burst[w] == 0x4003 || burst[w] == 0x4007 || burst[w] == 0x400B) v &= 0x07;   /* long notes */
            synth_write(&apu, burst[w], v, frame + 1500 + 290 * (uint64_t)w);
        }
        synth_run(&apu, (uint64_t)((f + 1) * 29780.5));
        drain();
    }
    double ms = 1000.0 * (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("throughput: 60 s of audio (every channel busy, noise period 4) in %.0f ms, %.2f ms per frame; "
           "state %u bytes\n", ms, ms / frames, (unsigned)sizeof(Synth2A03));
}

int main(void)
{
    test_frame_irq();
    test_length();
    test_five_step_immediate();
    test_dmc();
    test_sweep_mute();
    test_output();
    bench();
    printf(failures ? "%d check(s) failed\n" : "all checks passed\n", failures);
    return failures ? 1 : 0;
}
