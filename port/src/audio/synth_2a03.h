/*
 * synth_2a03.h - the NES APU (the 2A03's sound) as a small library, for the
 * port (PLAN_DECOMP.md 2.7, phase P6).
 *
 * Two pulse channels (duty, envelope, sweep and its mute, length counter),
 * the triangle (linear counter), noise (both modes), the DMC (sample fetches
 * through a callback, IRQ), the frame counter (4- and 5-step, IRQ), the
 * nonlinear mixer and the NES front-loader's output stage (90 Hz and 440 Hz
 * high-pass, 14 kHz low-pass), averaged down to the output rate.
 *
 * No globals, no dependencies beyond the C11 library; several instances can
 * run side by side. NTSC timing only (Mighty Final Fight is an NTSC game).
 *
 * Time. Everything is stamped with CPU cycles (1,789,772.67 Hz), counted by
 * the caller from synth_init (cycle 0). A write stamped `cycle` lands between
 * cycles `cycle - 1` and `cycle`, and `cycle` is the first one to see it;
 * a status read stamped `cycle` sees everything before it. Even cycles are
 * the APU's "put" half-cycles and odd ones its "get" half-cycles: the pulse
 * timers and the DMC count on get cycles, and a $4017 write resets the frame
 * counter 3 cycles later from a put cycle, 4 from a get cycle. A stamp older
 * than what has already run is treated as "now".
 *
 * Usage, once per video frame:
 *
 *     synth_write(&apu, 0x4000, 0xBF, frame_start + 1500);   (each register write)
 *     ...
 *     synth_run(&apu, frame_end);
 *     n = synth_take_samples(&apu, pcm, cap);                  (to the audio device)
 *
 * The output buffer holds SYNTH_RING samples (about 0.34 s at 48 kHz); take
 * them at least that often, or the oldest are dropped (counted in `dropped`).
 *
 * Exactness. Given the same writes on the same cycles, from the same state,
 * the samples equal those of NESRecomp's cycle-accurate runner (the oracle's
 * --wav-out): the channels, the frame counter and the length counters follow
 * the same rules cycle by cycle, the mixer uses the same single-precision
 * tables, and the output stage the same double-precision filters. Builds must
 * not reassociate or contract floating point (no -ffast-math, and
 * -ffp-contract=off on GCC/Clang); port/tests/synth checks it. What the
 * library cannot know is when the CPU lets a DMC sample fetch through: it
 * fetches 4 cycles after the output unit asks for a byte and 5-6 cycles after
 * $4015 starts a sample, which is the console's timing when the CPU is
 * reading on those cycles. Bit 5 of the status ($4015 read) is open bus on
 * the console; it reads 0 here.
 *
 * The state is public so a test can start from another emulator's snapshot
 * (port/tests/synth does, from the oracle's); a program only needs the
 * functions.
 */
#ifndef SYNTH_2A03_H
#define SYNTH_2A03_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SYNTH_RING  16384u            /* output samples buffered (a power of two) */
#define SYNTH_NEVER UINT64_MAX        /* "nothing scheduled" in the *_at fields */

/* The NTSC CPU clock: the 21.477272 MHz master clock divided by 12. */
#define SYNTH_CPU_HZ (21477272.0 / 12.0)

/* Reads one byte of the CPU address space for the DMC ($8000-$FFFF). */
typedef uint8_t (*SynthDmcRead)(void *user, uint16_t addr);

typedef struct {
    uint8_t start;      /* restart at the next quarter frame ($4003/$4007/$400F write) */
    uint8_t divider;
    uint8_t decay;      /* 15 down to 0: the volume when the constant flag is off */
} SynthEnvelope;

typedef struct {
    uint8_t count;      /* length counter; 0 silences the channel */
    uint8_t enabled;    /* $4015 bit */
    uint8_t halt;       /* halt flag as it was at the end of the previous cycle */
    uint8_t pending;    /* a load written since the last cycle ran */
    uint8_t load;       /* the value it loads */
} SynthLength;

typedef struct {
    uint16_t period;    /* 11-bit timer period */
    uint16_t timer;     /* counts down on get cycles; at 0 reloads and steps */
    uint8_t  step;      /* duty sequencer, 0-7; a $4003/$4007 write resets it */
    uint8_t  sweep_div, sweep_reload;
    SynthEnvelope env;
} SynthPulse;

typedef struct {
    uint16_t period, timer;  /* timer counts every cycle */
    uint8_t  step;           /* 0-31 */
    uint8_t  linear, linear_reload;
    uint8_t  out;            /* 0-15; holds while the period is below 2 */
} SynthTriangle;

typedef struct {
    uint16_t period;    /* in CPU cycles */
    uint16_t timer;     /* counts every cycle; at 0 reloads with period - 1 */
    uint16_t lfsr;      /* 15-bit shift register; bit 0 set = silent */
    SynthEnvelope env;
} SynthNoise;

typedef struct {
    uint16_t rate;           /* CPU cycles per output bit */
    uint16_t timer;          /* get cycles left until the next output bit */
    uint8_t  level;          /* 7-bit output ($4011) */
    uint8_t  shifter, bits;  /* the byte being played and its bits left */
    uint8_t  silent;         /* the output unit had no byte at the last boundary */
    uint8_t  buffer, buffer_full;
    uint8_t  loop, irq_enable, irq;
    uint16_t start, length;  /* $4012/$4013, decoded */
    uint16_t addr, remaining;
    uint64_t fetch_at;       /* cycle of the pending sample fetch */
} SynthDmc;

typedef struct {
    uint8_t  five_step, inhibit;
    uint8_t  irq;            /* $4015 bit 6 */
    uint8_t  pending;        /* quarter/half frame from a $4017 write, run on its cycle */
    uint64_t one_at;         /* cycle on which the step counter reads 1 */
    uint64_t reset_at;       /* a $4017 write's delayed reset */
    uint64_t ack_at;         /* a $4015 read's delayed acknowledge of bit 6 */
} SynthFrameCounter;

typedef struct Synth2A03 {
    uint64_t now;            /* the next cycle to run */
    uint64_t slow_at;        /* next cycle with frame counter, length or DMC work */
    uint8_t  reg[16];        /* $4000-$400F as last written */
    SynthPulse    pulse[2];
    SynthTriangle tri;
    SynthNoise    noise;
    SynthDmc      dmc;
    SynthLength   length[4]; /* pulse 1, pulse 2, triangle, noise */
    SynthFrameCounter fc;

    /* Mixer: the channel outputs and their mix, redone only when one changes. */
    uint8_t  out[5];         /* pulse 1, pulse 2, triangle, noise, DMC */
    uint8_t  live[4];        /* a timer step there can change the mix */
    uint8_t  dirty;
    double   level;
    float    pulse_table[31], tnd_table[203];

    /* Output stage: per-cycle levels averaged over each output sample, then
     * the console's analog filters. */
    int      sample_rate;
    double   cycles_per_sample, phase, acc;
    uint32_t acc_cycles;
    double   hp90_a, hp440_a, lp_a;
    double   hp90_x, hp90_y, hp440_x, hp440_y, lp_y;
    int16_t  ring[SYNTH_RING];
    uint32_t head, tail, dropped;
    uint64_t samples;        /* samples produced since synth_init */

    SynthDmcRead read;
    void        *user;
} Synth2A03;

/* Power-on state at cycle 0. sample_rate is the output rate (48000 for the
 * port); read may be NULL if the DMC is never used (fetches then read 0). */
void    synth_init(Synth2A03 *s, int sample_rate, SynthDmcRead read, void *user);

/* A write to $4000-$4017 at `cycle` ($4014 and $4016 are not sound and are
 * ignored). Runs the synthesizer up to `cycle` first. */
void    synth_write(Synth2A03 *s, uint16_t addr, uint8_t value, uint64_t cycle);

/* $4015 read at `cycle`: bits 0-3 length counters running, 4 DMC bytes left,
 * 6 frame interrupt (acknowledged by the read), 7 DMC interrupt. */
uint8_t synth_read_status(Synth2A03 *s, uint64_t cycle);

/* Runs every cycle before `cycle`, producing samples. */
void    synth_run(Synth2A03 *s, uint64_t cycle);

/* Moves up to `max` produced samples to `out`; returns how many. */
int     synth_take_samples(Synth2A03 *s, int16_t *out, int max);

/* The IRQ line: the frame interrupt (unless inhibited) or the DMC's. */
bool    synth_irq(const Synth2A03 *s);

#ifdef __cplusplus
}
#endif

#endif
