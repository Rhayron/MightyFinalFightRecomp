/*
 * synth_replay.c - plays an oracle capture (capture/oracle_capture.c) through
 * synth_2a03 and compares the result with the oracle's own WAV.
 *
 *   synth_replay CAPTURE OUT.wav [options]
 *     --oracle W.wav     compare with the oracle's --wav-out of the same run
 *     --range A:B        ...over frames A..B only (repeatable; default: all)
 *     --frames-out F     write "frame first_sample" for every frame
 *     --quantize         ignore the captured cycles: put each frame's writes
 *                        where the port will, 1500 cycles after the frame's
 *                        start and 290 apart (sound_model's burst)
 *     --phase SEED       start the oscillators (pulse, triangle, noise
 *                        timers and steps, the noise register) at random
 *                        points, to measure what phase alone changes
 *     --prg ROM.nes      serve DMC fetches from this NROM image's PRG
 *
 * The synthesizer starts from the oracle's APU state in the capture (the
 * runner's HwApu bytes, decoded by load_oracle_state) with an empty output
 * stage, as the oracle's audio does at the start of a run. It checks every
 * $4015 read against the value the CPU got and, when the capture has them
 * (MFF_APU_LEVELS), the five channel outputs on every logged cycle.
 *
 * Exit status: 0 when every compared sample, read and level is identical.
 */
#include "synth_2a03.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char     kind;
    uint64_t t;
    uint16_t addr;
    uint8_t  v[5];
} Event;

typedef struct {
    uint32_t frame;
    uint64_t t;
} FrameStart;

static Event      *events;
static size_t      n_events, cap_events;
static FrameStart *frames;
static size_t      n_frames, cap_frames;
static uint8_t     state[512];
static size_t      state_len;
static uint64_t    state_t, end_t;

static void *grow(void *p, size_t *cap, size_t need, size_t size)
{
    if (need <= *cap) return p;
    size_t n = *cap ? *cap * 2 : 4096;
    while (n < need) n *= 2;
    void *q = realloc(p, n * size);
    if (!q) {
        fprintf(stderr, "out of memory\n");
        exit(2);
    }
    *cap = n;
    return q;
}

static void read_capture(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "cannot read %s\n", path);
        exit(2);
    }
    static char line[4096];
    while (fgets(line, sizeof line, f)) {
        unsigned long long t;
        unsigned a, v, o[5], fr, n;
        int used;
        switch (line[0]) {
        case 'S':
            if (sscanf(line, "S %llu %u%n", &t, &n, &used) != 2 || n > sizeof state) break;
            state_t = t;
            state_len = n;
            for (unsigned i = 0; i < n; i++) {
                int k;
                if (sscanf(line + used, "%x%n", &v, &k) != 1) break;
                state[i] = (uint8_t)v;
                used += k;
            }
            break;
        case 'F':
            if (sscanf(line, "F %u %llu", &fr, &t) == 2) {
                frames = grow(frames, &cap_frames, n_frames + 1, sizeof *frames);
                frames[n_frames].frame = fr;
                frames[n_frames++].t = t;
            }
            break;
        case 'W':
        case 'V':
            if (sscanf(line + 1, " %llu %x %x", &t, &a, &v) == 3) {
                events = grow(events, &cap_events, n_events + 1, sizeof *events);
                events[n_events++] = (Event){ line[0], t, (uint16_t)a, { (uint8_t)v } };
            }
            break;
        case 'R':
            if (sscanf(line, "R %llu %x", &t, &v) == 2) {
                events = grow(events, &cap_events, n_events + 1, sizeof *events);
                events[n_events++] = (Event){ 'R', t, 0x4015, { (uint8_t)v } };
            }
            break;
        case 'L':
            if (sscanf(line, "L %llu %u %u %u %u %u", &t, &o[0], &o[1], &o[2], &o[3], &o[4]) == 6) {
                events = grow(events, &cap_events, n_events + 1, sizeof *events);
                events[n_events++] = (Event){ 'L', t, 0,
                    { (uint8_t)o[0], (uint8_t)o[1], (uint8_t)o[2], (uint8_t)o[3], (uint8_t)o[4] } };
            }
            break;
        case 'E':
            if (sscanf(line, "E %llu", &t) == 1) end_t = t;
            break;
        default:
            break;
        }
    }
    fclose(f);
    if (!state_len) {
        fprintf(stderr, "%s: no S line\n", path);
        exit(2);
    }
}

/* ---- the runner's APU state ----------------------------------------------
 * HwApu in nesrecomp runner/cyc/hw_apu.c (2b4fcdb..fb860c7), as MSVC x64 lays
 * it out: 132 bytes. Offsets checked against a power-on capture (the known
 * power-on values below) and by the replay itself. */
static uint16_t u16(const uint8_t *b, int at) { return (uint16_t)(b[at] | b[at + 1] << 8); }

static void load_oracle_state(Synth2A03 *s, const uint8_t *b, size_t n, uint64_t t)
{
    if (n != 132) {
        fprintf(stderr, "APU state is %u bytes, this decoder knows the 132-byte layout\n", (unsigned)n);
        exit(2);
    }
    if (b[0] != ((t & 1) == 0)) {
        fprintf(stderr, "APU put/get phase %u does not match cycle %llu\n", b[0], (unsigned long long)t);
        exit(2);
    }
    s->now = t;
    SynthFrameCounter *fc = &s->fc;
    fc->five_step = b[1];
    fc->inhibit = b[2];
    fc->reset_at = (b[3] & 0x80) ? SYNTH_NEVER : t + b[3];
    fc->one_at = t - u16(b, 4);                    /* the counter read u16(b,4) on cycle t - 1 */
    fc->pending = (uint8_t)((b[6] ? 1 : 0) | (b[7] ? 2 : 0));
    fc->irq = b[8];
    fc->ack_at = b[9] ? t + (t & 1) : SYNTH_NEVER;
    for (int ch = 0; ch < 4; ch++) {
        const uint8_t *l = b + 11 + 5 * ch;
        s->length[ch].count = l[0];
        s->length[ch].halt = l[1];
        s->length[ch].pending = l[2];
        s->length[ch].load = l[3];
        s->length[ch].enabled = l[4];
    }
    memcpy(s->reg, b + 31, 16);
    SynthDmc *d = &s->dmc;
    d->irq_enable = b[47];
    d->loop = b[48];
    d->irq = b[49];
    d->rate = u16(b, 50);
    d->timer = (uint16_t)(u16(b, 52) / 2);
    d->level = b[54];
    d->start = u16(b, 56);
    d->length = u16(b, 58);
    d->remaining = b[71] ? u16(b, 60) : 0;         /* bytes left, if $4015 bit 4 is on */
    d->addr = u16(b, 62);
    d->buffer = b[64];
    d->bits = b[65];
    d->buffer_full = b[67];
    d->shifter = b[68];
    d->silent = b[69];
    d->fetch_at = (b[77] || b[73]) ? t + 4 : SYNTH_NEVER;   /* a DMA under way (never in MFF) */
    for (int k = 0; k < 2; k++) {
        const uint8_t *p = b + 94 + 10 * k;
        s->pulse[k].period = u16(p, 0);
        s->pulse[k].timer = u16(p, 2);
        s->pulse[k].step = p[4];
        s->pulse[k].env.start = p[5];
        s->pulse[k].env.divider = p[6];
        s->pulse[k].env.decay = p[7];
        s->pulse[k].sweep_div = p[8];
        s->pulse[k].sweep_reload = p[9];
    }
    s->tri.period = u16(b, 114);
    s->tri.timer = u16(b, 116);
    s->tri.step = b[118];
    s->tri.linear = b[119];
    s->tri.linear_reload = b[120];
    s->tri.out = b[121];
    s->noise.period = u16(b, 122);
    s->noise.timer = u16(b, 124);
    s->noise.lfsr = u16(b, 126);
    s->noise.env.start = b[128];
    s->noise.env.divider = b[129];
    s->noise.env.decay = b[130];
    /* Resynchronize: the first cycle runs as a slow one, which only does what
     * any cycle would and recomputes the schedule; the mix is redone too. */
    s->slow_at = t;
    s->dirty = 1;
}

/* Known power-on values (cyc_power_on runs one APU clock): a check of the layout. */
static int check_power_on(const uint8_t *b)
{
    return b[0] == 0 && b[3] == 0xFF && u16(b, 4) == 1 && u16(b, 50) == 428 && u16(b, 52) == 1022 &&
           u16(b, 56) == 0xC000 && u16(b, 58) == 1 && b[65] == 8 && b[66] == 1 && b[69] == 1 &&
           b[121] == 15 && u16(b, 122) == 4 && u16(b, 126) == 1;
}

/* ---- WAV ---------------------------------------------------------------- */
static void put32(uint8_t *p, uint32_t v) { p[0] = (uint8_t)v, p[1] = (uint8_t)(v >> 8), p[2] = (uint8_t)(v >> 16), p[3] = (uint8_t)(v >> 24); }

static void write_wav(const char *path, const int16_t *pcm, size_t n, int rate)
{
    FILE *f = fopen(path, "wb");
    if (!f) {
        fprintf(stderr, "cannot write %s\n", path);
        exit(2);
    }
    uint8_t h[44] = { 'R', 'I', 'F', 'F', 0, 0, 0, 0, 'W', 'A', 'V', 'E', 'f', 'm', 't', ' ', 16, 0, 0, 0, 1, 0, 1, 0 };
    put32(h + 4, (uint32_t)(36 + 2 * n));
    put32(h + 24, (uint32_t)rate);
    put32(h + 28, (uint32_t)rate * 2);
    h[32] = 2, h[34] = 16;
    memcpy(h + 36, "data", 4);
    put32(h + 40, (uint32_t)(2 * n));
    fwrite(h, 1, 44, f);
    fwrite(pcm, 2, n, f);   /* little-endian hosts */
    fclose(f);
}

static int16_t *read_wav(const char *path, size_t *n)
{
    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "cannot read %s\n", path);
        exit(2);
    }
    uint8_t h[12];
    if (fread(h, 1, 12, f) != 12 || memcmp(h, "RIFF", 4) || memcmp(h + 8, "WAVE", 4)) {
        fprintf(stderr, "%s: not a WAV\n", path);
        exit(2);
    }
    for (;;) {
        uint8_t c[8];
        if (fread(c, 1, 8, f) != 8) {
            fprintf(stderr, "%s: no data chunk\n", path);
            exit(2);
        }
        uint32_t len = (uint32_t)(c[4] | c[5] << 8 | c[6] << 16 | (uint32_t)c[7] << 24);
        if (!memcmp(c, "data", 4)) {
            int16_t *pcm = malloc(len ? len : 2);
            *n = fread(pcm, 2, len / 2, f);
            fclose(f);
            return pcm;
        }
        fseek(f, (long)len, SEEK_CUR);
    }
}

/* ---- DMC fetches from an NROM image ------------------------------------- */
static uint8_t *prg;
static size_t   prg_len;

static void load_prg(const char *path)
{
    FILE *f = fopen(path, "rb");
    uint8_t h[16];
    if (!f || fread(h, 1, 16, f) != 16 || memcmp(h, "NES\x1A", 4) || (h[6] >> 4 | (h[7] & 0xF0)) != 0) {
        fprintf(stderr, "%s: not an NROM iNES image\n", path);
        exit(2);
    }
    prg_len = (size_t)h[4] * 16384;
    prg = malloc(prg_len);
    if (!prg || fread(prg, 1, prg_len, f) != prg_len) {
        fprintf(stderr, "%s: short PRG\n", path);
        exit(2);
    }
    fclose(f);
}

static uint8_t prg_read(void *user, uint16_t addr)
{
    (void)user;
    return prg[(addr - 0x8000u) % prg_len];
}

/* ---- comparison --------------------------------------------------------- */
static int compare(const int16_t *a, const int16_t *b, size_t from, size_t to, const char *label)
{
    double sum_d2 = 0, sa = 0, sb = 0, saa = 0, sbb = 0, sab = 0;
    int max_d = 0;
    size_t differ = 0, first = (size_t)-1;
    for (size_t i = from; i < to; i++) {
        int d = a[i] - b[i];
        if (d) {
            differ++;
            if (first == (size_t)-1) first = i;
        }
        if (abs(d) > max_d) max_d = abs(d);
        sum_d2 += (double)d * d;
        sa += a[i], sb += b[i], saa += (double)a[i] * a[i], sbb += (double)b[i] * b[i], sab += (double)a[i] * b[i];
    }
    double n = (double)(to - from);
    double cov = sab - sa * sb / n, va = saa - sa * sa / n, vb = sbb - sb * sb / n;
    double r = va > 0 && vb > 0 ? cov / sqrt(va * vb) : (va == vb ? 1.0 : 0.0);
    printf("%s: %zu samples, %zu differ, max |diff| %d, RMS diff %.4f, RMS %.1f / %.1f, correlation %.9f",
           label, to - from, differ, max_d, sqrt(sum_d2 / n), sqrt(saa / n), sqrt(sbb / n), r);
    if (differ) printf(", first at sample %zu", first);
    printf("\n");
    return differ != 0;
}

/* ---- main --------------------------------------------------------------- */
static uint64_t rng_state;
static uint32_t rng(void)
{
    rng_state = rng_state * 6364136223846793005ull + 1442695040888963407ull;
    return (uint32_t)(rng_state >> 33);
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "usage: synth_replay CAPTURE OUT.wav [--oracle W.wav] [--range A:B]... "
                        "[--frames-out F] [--quantize] [--phase SEED] [--prg ROM.nes]\n");
        return 2;
    }
    const char *oracle = NULL, *frames_out = NULL;
    unsigned ranges[64][2];
    int n_ranges = 0, quantize = 0;
    long long phase_seed = -1;
    for (int i = 3; i < argc; i++) {
        if (!strcmp(argv[i], "--oracle") && i + 1 < argc) oracle = argv[++i];
        else if (!strcmp(argv[i], "--frames-out") && i + 1 < argc) frames_out = argv[++i];
        else if (!strcmp(argv[i], "--quantize")) quantize = 1;
        else if (!strcmp(argv[i], "--phase") && i + 1 < argc) phase_seed = atoll(argv[++i]);
        else if (!strcmp(argv[i], "--prg") && i + 1 < argc) load_prg(argv[++i]);
        else if (!strcmp(argv[i], "--range") && i + 1 < argc && n_ranges < 64 &&
                 sscanf(argv[++i], "%u:%u", &ranges[n_ranges][0], &ranges[n_ranges][1]) == 2) n_ranges++;
        else {
            fprintf(stderr, "bad argument %s\n", argv[i]);
            return 2;
        }
    }
    read_capture(argv[1]);
    if (!n_frames) {
        fprintf(stderr, "no frames in the capture\n");
        return 2;
    }
    if (state_t == 1 && !check_power_on(state)) {
        fprintf(stderr, "a power-on capture, but the APU bytes do not decode to the power-on state: layout changed?\n");
        return 2;
    }

    static Synth2A03 synth;
    Synth2A03 *s = &synth;
    synth_init(s, 48000, prg ? prg_read : NULL, NULL);
    load_oracle_state(s, state, state_len, state_t);
    if (phase_seed >= 0) {
        rng_state = (uint64_t)phase_seed * 2654435761u + 1;
        for (int k = 0; k < 2; k++) {
            s->pulse[k].timer = (uint16_t)(rng() % (s->pulse[k].period + 1u));
            s->pulse[k].step = (uint8_t)(rng() & 7);
        }
        s->tri.timer = (uint16_t)(rng() % (s->tri.period + 1u));
        s->tri.step = (uint8_t)(rng() & 31);
        s->noise.timer = (uint16_t)(rng() % s->noise.period);
        s->noise.lfsr = (uint16_t)(rng() & 0x7FFF) | 1;
    }

    size_t cap_pcm = 1 << 20, n_pcm = 0;
    int16_t *pcm = malloc(cap_pcm * sizeof *pcm);
    uint64_t *frame_sample = malloc(n_frames * sizeof *frame_sample);
    size_t fi = 0, reads = 0, read_bad = 0, levels = 0, level_bad = 0;
    unsigned burst = 0;
    uint64_t frame_t = frames[0].t;
#define DRAIN()                                                                                   \
    do {                                                                                          \
        for (;;) {                                                                                \
            pcm = grow(pcm, &cap_pcm, n_pcm + 4096, sizeof *pcm);                                 \
            int got = synth_take_samples(s, pcm + n_pcm, 4096);                                   \
            n_pcm += (size_t)got;                                                                 \
            if (got < 4096) break;                                                                \
        }                                                                                         \
    } while (0)

    for (size_t e = 0; e <= n_events; e++) {
        uint64_t t = e < n_events ? events[e].t : (end_t ? end_t + 1 : frames[n_frames - 1].t);
        /* Frame starts up to this event: note the sample index there. */
        while (fi < n_frames && frames[fi].t <= t) {
            synth_run(s, frames[fi].t);
            DRAIN();
            frame_sample[fi] = s->samples;
            frame_t = frames[fi].t;
            burst = 0;
            fi++;
        }
        if (e == n_events) {
            synth_run(s, t);
            DRAIN();
            break;
        }
        const Event *ev = &events[e];
        switch (ev->kind) {
        case 'W':
            if (quantize) synth_write(s, ev->addr, ev->v[0], frame_t + 1500 + 290 * burst++);
            else synth_write(s, ev->addr, ev->v[0], ev->t);
            break;
        case 'R': {
            if (quantize) break;
            uint8_t got = synth_read_status(s, ev->t);
            reads++;
            if ((got & 0xDF) != (ev->v[0] & 0xDF)) {
                if (read_bad++ < 5)
                    printf("$4015 read at cycle %llu: synth %02X, oracle %02X\n", (unsigned long long)ev->t, got,
                           ev->v[0]);
            }
            break;
        }
        case 'L':
            if (quantize || phase_seed >= 0) break;
            synth_run(s, ev->t + 1);
            levels++;
            if (memcmp(s->out, ev->v, 5)) {
                if (level_bad++ < 5)
                    printf("levels after cycle %llu: synth %u %u %u %u %u, oracle %u %u %u %u %u\n",
                           (unsigned long long)ev->t, s->out[0], s->out[1], s->out[2], s->out[3], s->out[4],
                           ev->v[0], ev->v[1], ev->v[2], ev->v[3], ev->v[4]);
            }
            break;
        default:
            break;
        }
        DRAIN();
    }
    write_wav(argv[2], pcm, n_pcm, 48000);
    printf("%s: %zu samples, %zu frames (%u-%u), %zu events", argv[2], n_pcm, n_frames, frames[0].frame,
           frames[n_frames - 1].frame, n_events);
    if (s->dropped) printf(", %u dropped", s->dropped);
    printf("\n");
    if (reads) printf("$4015 reads: %zu, %zu differ\n", reads, read_bad);
    if (levels) printf("channel levels: %zu cycles, %zu differ\n", levels, level_bad);
    int status = (read_bad || level_bad) ? 1 : 0;

    if (frames_out) {
        FILE *f = fopen(frames_out, "w");
        if (f) {
            for (size_t i = 0; i < n_frames; i++)
                fprintf(f, "%u %llu\n", frames[i].frame, (unsigned long long)frame_sample[i]);
            fclose(f);
        }
    }
    if (oracle) {
        size_t n_or;
        int16_t *ref = read_wav(oracle, &n_or);
        if (n_or != n_pcm) printf("length: synth %zu samples, oracle %zu\n", n_pcm, n_or);
        size_t n = n_or < n_pcm ? n_or : n_pcm;
        if (!n_ranges) {
            status |= compare(pcm, ref, 0, n, "all");
        }
        for (int r = 0; r < n_ranges; r++) {
            size_t from = (size_t)-1, to = (size_t)-1;
            for (size_t i = 0; i < n_frames; i++) {
                if (frames[i].frame == ranges[r][0]) from = (size_t)frame_sample[i];
                if (frames[i].frame == ranges[r][1] + 1) to = (size_t)frame_sample[i];
            }
            if (to == (size_t)-1 && ranges[r][1] + 1 > frames[n_frames - 1].frame) to = n;
            if (from == (size_t)-1 || to == (size_t)-1 || to > n || from >= to) {
                printf("frames %u-%u: not in this capture\n", ranges[r][0], ranges[r][1]);
                status = 1;
                continue;
            }
            char label[64];
            snprintf(label, sizeof label, "frames %u-%u", ranges[r][0], ranges[r][1]);
            status |= compare(pcm, ref, from, to, label);
        }
        free(ref);
    }
    free(pcm);
    free(frame_sample);
    return status;
}
