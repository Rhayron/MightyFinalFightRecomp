/*
 * apu.c - the game's sound: its APU register writes go to the 2A03
 * synthesizer (port/src/audio/synth_2a03.c, P6), and at every frame boundary
 * the samples of the frame that ended go to the window's audio queue and to
 * MFF_WAV.
 *
 * The sound's clock. The synthesizer runs on the console's frame grid, not
 * on the runtime's cycle count: frame k (the k-th vblank) starts at the
 * cycle the console reaches it, the first at scanline 241 dot 1 (dot
 * 82,182, cycle 27,394; the oracle's first frame ends at 27,396, on the
 * instruction boundary after it) and each next one a frame length later
 * (timing.c: 89,342 dots, one less on odd frames with rendering on), and a
 * write is stamped with its place in its frame, the cycles charged since the
 * boundary (timing_frame_cycles). The runtime's own count (g_nes_cycles)
 * runs longer than the console's: its first frame is a full one, and a
 * boundary the game holds back (mff_hooks.c, where the NMI may land) waits
 * without carrying the wait over, on average 42 cycles a frame on Cody's
 * route (29,822.8 against 29,780.5). Stamped with it, the sound fell behind
 * the oracle's by a frame every ~700 (170 ms after two minutes). A frame
 * held back past its length (a lag frame on the console, which the runtime
 * does not have) keeps its writes on its last cycle.
 *
 * The synthesizer runs from power-on in every mode, so that a $4015 read
 * would see the same thing headless or in a window (the game never reads it,
 * and nothing the APU does reaches the game):
 *  - reset writes $40 to $4017 before any frame-counter interrupt can be
 *    raised (16 cycles in, the first would come at 29,830), so the frame
 *    interrupt is inhibited for good;
 *  - the sound driver only enables the pulse, triangle and noise channels
 *    ($4015 = $0F), never the DMC: no sample DMA steals cycles, no DMC
 *    interrupt.
 * The runtime says so if that ever stops being true.
 *
 * MFF_WAV=FILE writes the output, 16-bit mono at 48 kHz, to compare with the
 * oracle's --wav-out for the same route (port/tools/sound_wavcmp.py).
 * MFF_APU_LOG=FILE writes what the synthesizer was given, in the lines of
 * the oracle's capture (port/tests/synth/capture/oracle_capture.c): "F n T"
 * frame boundary n at cycle T, "W T ADDR VV" a write, "R T VV" a $4015 read,
 * T on the sound's clock (port/tests/apu_writes.py compares the two).
 */
#include "machine.h"
#include "synth_2a03.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define APU_RATE 48000
#define FIRST_VBLANK 27394u             /* (241 * 341 + 1) / 3: scanline 241, dot 1 */

static Synth2A03 s_apu;
static uint64_t s_frame_at;             /* the synthesizer's cycle where this frame began */
static uint32_t s_frame_len = FIRST_VBLANK;
static uint32_t s_boundaries;
static FILE *s_wav, *s_log;
static uint32_t s_wav_samples;

/* DMC sample fetches read PRG through the MMC3 (the game never plays one) */
static uint8_t dmc_read(void *user, uint16_t addr) {
    (void)user;
    return mmc3_prg_peek(addr);
}

static void put16(uint8_t *p, uint32_t v) { p[0] = (uint8_t)v; p[1] = (uint8_t)(v >> 8); }
static void put32(uint8_t *p, uint32_t v) { put16(p, v); put16(p + 2, v >> 16); }

static void wav_header(uint32_t samples) {
    uint8_t h[44];
    memcpy(h, "RIFF", 4);
    put32(h + 4, 36 + samples * 2);
    memcpy(h + 8, "WAVEfmt ", 8);
    put32(h + 16, 16);
    put16(h + 20, 1);                   /* PCM */
    put16(h + 22, 1);                   /* mono */
    put32(h + 24, APU_RATE);
    put32(h + 28, APU_RATE * 2);
    put16(h + 32, 2);
    put16(h + 34, 16);
    memcpy(h + 36, "data", 4);
    put32(h + 40, samples * 2);
    fseek(s_wav, 0, SEEK_SET);
    fwrite(h, 1, sizeof h, s_wav);
    fseek(s_wav, 0, SEEK_END);
}

static void sound_files_close(void) {
    if (s_log) fclose(s_log);
    s_log = NULL;
    if (!s_wav) return;
    wav_header(s_wav_samples);
    fclose(s_wav);
    s_wav = NULL;
}

/* now on the sound's clock */
static uint64_t apu_now(void) {
    uint32_t into = timing_frame_cycles();
    return s_frame_at + (into < s_frame_len ? into : s_frame_len - 1);
}

void apu_power_on(void) {
    const char *wav = getenv("MFF_WAV"), *log = getenv("MFF_APU_LOG");
    synth_init(&s_apu, APU_RATE, dmc_read, NULL);
    s_frame_at = 0;
    s_frame_len = FIRST_VBLANK;
    s_boundaries = 0;
    if (wav && *wav) {
        s_wav = fopen(wav, "wb");
        if (!s_wav) mff_fatal("MFF_WAV: cannot write %s", wav);
        wav_header(0);
    }
    if (log && *log) {
        s_log = fopen(log, "w");
        if (!s_log) mff_fatal("MFF_APU_LOG: cannot write %s", log);
    }
    if (s_wav || s_log) atexit(sound_files_close);
}

void apu_write(uint16_t a, uint8_t v) {
    static int warned;
    uint64_t t = apu_now();
    if (a > 0x4017) return;
    synth_write(&s_apu, a, v, t);
    if (s_log) fprintf(s_log, "W %llu %04X %02X\n", (unsigned long long)t, a, v);
    if (!warned && ((a == 0x4015 && (v & 0x10)) || (a == 0x4017 && !(v & 0x40)) ||
                    (a == 0x4010 && (v & 0x80)))) {
        warned = 1;
        fprintf(stderr, "[mff] APU: $%04X = $%02X (DMC or frame interrupt on): the runtime does not "
                "give the APU's interrupt or DMA cycles to the CPU, so timing may leave the scaffold's\n",
                a, v);
    }
}

uint8_t apu_read_status(void) {
    uint64_t t = apu_now();
    uint8_t v = synth_read_status(&s_apu, t);
    if (s_log) fprintf(s_log, "R %llu %02X\n", (unsigned long long)t, v);
    return v;
}

void apu_frame_boundary(uint32_t next_len) {
    int16_t pcm[4096];
    int n;
    s_frame_at += s_frame_len;
    s_frame_len = next_len;
    synth_run(&s_apu, s_frame_at);
    if (s_log) fprintf(s_log, "F %u %llu\n", s_boundaries, (unsigned long long)s_frame_at);
    s_boundaries++;
    while ((n = synth_take_samples(&s_apu, pcm, (int)(sizeof pcm / sizeof pcm[0]))) > 0) {
        if (g_window) video_audio(pcm, n);
        if (s_wav) {
            fwrite(pcm, sizeof pcm[0], (size_t)n, s_wav);
            s_wav_samples += (uint32_t)n;
        }
    }
}
