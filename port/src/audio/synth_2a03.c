/*
 * synth_2a03.c - the 2A03's sound, cycle by cycle (see synth_2a03.h).
 *
 * synth_run splits time into two kinds of CPU cycles. Most only count the
 * channel timers down, step the sequencers and add the mix to the sample
 * being built (run_fast). A few also have frame counter, length counter or
 * DMC work: a quarter or half frame, a length load, the delayed effect of a
 * $4015 read or a $4017 write, a sample fetch (slow_cycle, which runs before
 * that cycle's timers). slow_at always names the next of those; a register
 * write makes its own cycle one.
 *
 * Inside a cycle the order is the console's (and the cycle runner's): frame
 * counter and what it clocks, length counters, the halt flags latching for
 * the next cycle; then the timers, and the mix of what the channels output.
 */
#include "synth_2a03.h"

#include <string.h>

/* pi to 15 digits, as the cycle runner writes it: the filter coefficients
 * then come out bit for bit the same. */
#define PI_15 3.14159265358979

static const uint8_t length_table[32] = {
    10, 254, 20, 2, 40, 4, 80, 6, 160, 8, 60, 10, 14, 12, 26, 14,
    12, 16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30,
};

/* NTSC periods, in CPU cycles. */
static const uint16_t noise_periods[16] = {
    4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068,
};
static const uint16_t dmc_rates[16] = {
    428, 380, 340, 320, 286, 254, 226, 214, 190, 160, 142, 128, 106, 84, 72, 54,
};

/* The duty cycles as the sequencer plays them from step 0, where a
 * $4003/$4007 write puts it: bit n is the output of step n.
 * 12.5%, 25%, 50% and 25% inverted. */
static const uint8_t duty_waves[4] = { 0x02, 0x06, 0x1E, 0xF9 };

/* The frame counter: what happens when its step counter reaches a value. */
enum {
    FC_QUARTER = 1,   /* envelopes and the triangle's linear counter */
    FC_HALF    = 2,   /* length counters and sweeps */
    FC_IRQ     = 4,   /* raise the frame interrupt flag */
    FC_IRQ_END = 8,   /* ...and drop it again if interrupts are inhibited */
    FC_WRAP    = 16,  /* the counter restarts (reads 1 on the next cycle) */
};

typedef struct {
    uint16_t at;
    uint8_t  what;
} FcStep;

/* The flag is visible on 29828-29829 even with interrupts inhibited. */
static const FcStep fc_four[] = {
    { 7457, FC_QUARTER },          { 14913, FC_QUARTER | FC_HALF },          { 22371, FC_QUARTER },
    { 29828, FC_IRQ },             { 29829, FC_QUARTER | FC_HALF | FC_IRQ }, { 29830, FC_IRQ | FC_IRQ_END | FC_WRAP },
};
static const FcStep fc_five[] = {
    { 7457, FC_QUARTER },          { 14913, FC_QUARTER | FC_HALF },          { 22371, FC_QUARTER },
    { 37281, FC_QUARTER | FC_HALF }, { 37282, FC_WRAP },
};

static const FcStep *fc_steps(const SynthFrameCounter *fc, int *n)
{
    *n = fc->five_step ? (int)(sizeof fc_five / sizeof fc_five[0]) : (int)(sizeof fc_four / sizeof fc_four[0]);
    return fc->five_step ? fc_five : fc_four;
}

/* The step counter on cycle t: 16 bits, 1 on cycle one_at. */
static uint16_t fc_value(const SynthFrameCounter *fc, uint64_t t)
{
    return (uint16_t)(t + 1 - fc->one_at);
}

/* The first cycle at or after `from` on which the counter reaches a step (or
 * wraps to 0, which only a 4-step counter left past 29830 by a mode change
 * could do before the change's own reset). */
static uint64_t fc_next(const SynthFrameCounter *fc, uint64_t from)
{
    int n;
    const FcStep *steps = fc_steps(fc, &n);
    uint16_t v = fc_value(fc, from);
    for (int i = 0; i < n; i++)
        if (steps[i].at >= v) return from + (uint64_t)(steps[i].at - v);
    return from + (uint64_t)(65536u - v);
}

static unsigned fc_what(const SynthFrameCounter *fc, uint16_t v)
{
    int n;
    const FcStep *steps = fc_steps(fc, &n);
    for (int i = 0; i < n; i++)
        if (steps[i].at == v) return steps[i].what;
    return 0;
}

/* ------------------------------------------------------------------------- */
/* Channel parts                                                             */
/* ------------------------------------------------------------------------- */

static uint8_t envelope_volume(const SynthEnvelope *e, uint8_t ctrl)
{
    return (ctrl & 0x10) ? (uint8_t)(ctrl & 15) : e->decay;
}

static void envelope_clock(SynthEnvelope *e, uint8_t ctrl)
{
    if (e->start) {
        e->start = 0;
        e->decay = 15;
        e->divider = ctrl & 15;
    } else if (e->divider) {
        e->divider--;
    } else {
        e->divider = ctrl & 15;
        if (e->decay) e->decay--;
        else if (ctrl & 0x20) e->decay = 15;          /* loop */
    }
}

/* Where the sweep would take the period. Negation is ones' complement on
 * pulse 1 and two's complement on pulse 2; below 0 it is 0, which never mutes. */
static int sweep_target(uint16_t period, uint8_t sweep, int ones_complement)
{
    int change = period >> (sweep & 7);
    if (!(sweep & 0x08)) return period + change;
    int target = period - change - ones_complement;
    return target < 0 ? 0 : target;
}

static uint8_t triangle_shape(uint8_t step)
{
    return (uint8_t)(step < 16 ? 15 - step : step - 16);   /* 15..0, 0..15 */
}

static void quarter_frame(Synth2A03 *s)
{
    envelope_clock(&s->pulse[0].env, s->reg[0x0]);
    envelope_clock(&s->pulse[1].env, s->reg[0x4]);
    envelope_clock(&s->noise.env, s->reg[0xC]);
    SynthTriangle *tr = &s->tri;
    if (tr->linear_reload) tr->linear = s->reg[0x8] & 0x7F;
    else if (tr->linear) tr->linear--;
    if (!(s->reg[0x8] & 0x80)) tr->linear_reload = 0;
}

static void sweep_clock(Synth2A03 *s)
{
    for (int k = 0; k < 2; k++) {
        SynthPulse *p = &s->pulse[k];
        uint8_t sweep = s->reg[4 * k + 1];
        int target = sweep_target(p->period, sweep, k == 0);
        if (!p->sweep_div && (sweep & 0x80) && (sweep & 7) && p->period >= 8 && target <= 0x7FF)
            p->period = (uint16_t)target;
        if (!p->sweep_div || p->sweep_reload) {
            p->sweep_div = (sweep >> 4) & 7;
            p->sweep_reload = 0;
        } else {
            p->sweep_div--;
        }
    }
}

/* A load written since the last cycle lands now, unless this cycle is a half
 * frame and the counter is not empty: then the clock wins and the load is lost. */
static void length_clock(Synth2A03 *s, int half)
{
    for (int ch = 0; ch < 4; ch++) {
        SynthLength *l = &s->length[ch];
        if (half) {
            if (l->pending && !l->count) l->count = l->load;   /* stays pending one more cycle */
            else l->pending = 0;
            if (!l->enabled) l->count = 0;
            if (l->count && !l->halt && !l->pending) l->count--;
        } else if (l->pending) {
            l->count = l->load;
            l->pending = 0;
        }
    }
}

static void length_load(Synth2A03 *s, int ch, uint8_t value)
{
    if (!s->length[ch].enabled) return;
    s->length[ch].load = length_table[value >> 3];
    s->length[ch].pending = 1;
}

/* ------------------------------------------------------------------------- */
/* DMC                                                                       */
/* ------------------------------------------------------------------------- */

static void dmc_fetch(Synth2A03 *s)
{
    SynthDmc *d = &s->dmc;
    d->fetch_at = SYNTH_NEVER;
    if (!d->remaining) return;
    d->buffer = s->read ? s->read(s->user, d->addr) : 0;
    d->buffer_full = 1;
    d->addr = d->addr == 0xFFFF ? 0x8000 : (uint16_t)(d->addr + 1);
    if (--d->remaining == 0) {
        if (d->loop) {
            d->addr = d->start;
            d->remaining = d->length;
        } else if (d->irq_enable) {
            d->irq = 1;
        }
    }
}

/* The output unit's clock (every `rate` CPU cycles, on a get cycle). */
static void dmc_bit(Synth2A03 *s, uint64_t t)
{
    SynthDmc *d = &s->dmc;
    if (!d->silent) {
        if ((d->shifter & 1) && d->level <= 125) {
            d->level += 2;
            s->dirty = 1;
        } else if (!(d->shifter & 1) && d->level >= 2) {
            d->level -= 2;
            s->dirty = 1;
        }
    }
    d->shifter >>= 1;
    if (--d->bits) return;
    /* Byte boundary: take the buffer, or play silence, and ask for the next. */
    d->bits = 8;
    d->silent = !d->buffer_full;
    if (d->buffer_full) {
        d->shifter = d->buffer;
        d->buffer_full = 0;
    }
    if (d->remaining && d->fetch_at == SYNTH_NEVER) {
        d->fetch_at = t + 4;
        if (d->fetch_at < s->slow_at) s->slow_at = d->fetch_at;
    }
}

/* ------------------------------------------------------------------------- */
/* Mixer and output stage                                                    */
/* ------------------------------------------------------------------------- */

static void mix(Synth2A03 *s)
{
    for (int k = 0; k < 2; k++) {
        const SynthPulse *p = &s->pulse[k];
        uint8_t ctrl = s->reg[4 * k];
        int on = s->length[k].count && p->period >= 8 && sweep_target(p->period, s->reg[4 * k + 1], k == 0) <= 0x7FF;
        uint8_t vol = on ? envelope_volume(&p->env, ctrl) : 0;
        s->live[k] = vol != 0;
        s->out[k] = (duty_waves[ctrl >> 6] >> p->step & 1) ? vol : 0;
    }
    if (s->tri.period >= 2) s->tri.out = triangle_shape(s->tri.step);
    s->live[2] = s->tri.period >= 2;
    s->out[2] = s->tri.out;
    uint8_t nvol = s->length[3].count ? envelope_volume(&s->noise.env, s->reg[0xC]) : 0;
    s->live[3] = nvol != 0;
    s->out[3] = (s->noise.lfsr & 1) ? 0 : nvol;
    s->out[4] = s->dmc.level;
    /* Single precision, as the tables are: the runner adds them as floats. */
    float level = s->pulse_table[s->out[0] + s->out[1]] + s->tnd_table[3 * s->out[2] + 2 * s->out[3] + s->out[4]];
    s->level = level;
    s->dirty = 0;
}

/* First-order RC high-pass, discretized at the output rate. */
static double highpass(double a, double *x1, double *y1, double x)
{
    double y = a * (*y1 + x - *x1);
    *x1 = x;
    *y1 = y;
    return y;
}

static void emit(Synth2A03 *s, double level)
{
    double v = highpass(s->hp90_a, &s->hp90_x, &s->hp90_y, level);
    v = highpass(s->hp440_a, &s->hp440_x, &s->hp440_y, v);
    s->lp_y += s->lp_a * (v - s->lp_y);
    double pcm = s->lp_y * 30000.0;
    if (pcm > 32767) pcm = 32767;
    if (pcm < -32768) pcm = -32768;
    s->ring[s->head++ & (SYNTH_RING - 1)] = (int16_t)pcm;
    if (s->head - s->tail > SYNTH_RING) {
        s->tail = s->head - SYNTH_RING;
        s->dropped++;
    }
    s->samples++;
}

/* ------------------------------------------------------------------------- */
/* Running                                                                   */
/* ------------------------------------------------------------------------- */

static void schedule(Synth2A03 *s, uint64_t from)
{
    uint64_t at = fc_next(&s->fc, from);
    if (s->fc.reset_at < at) at = s->fc.reset_at;
    if (s->fc.ack_at < at) at = s->fc.ack_at;
    if (s->dmc.fetch_at < at) at = s->dmc.fetch_at;
    if (s->fc.pending) at = from;
    for (int ch = 0; ch < 4; ch++)
        if (s->length[ch].pending) at = from;
    s->slow_at = at < from ? from : at;
}

static void slow_cycle(Synth2A03 *s, uint64_t t)
{
    SynthFrameCounter *fc = &s->fc;
    unsigned what = fc->pending;
    fc->pending = 0;
    if (!(t & 1) && t >= fc->ack_at) {          /* a $4015 read acknowledges on a put cycle */
        fc->irq = 0;
        fc->ack_at = SYNTH_NEVER;
    }
    if (t >= s->dmc.fetch_at) dmc_fetch(s);
    if (t >= fc->reset_at) {                     /* a $4017 write's reset */
        fc->one_at = t;
        fc->reset_at = SYNTH_NEVER;
    }
    uint16_t v = fc_value(fc, t);
    if (!v) fc->one_at = t + 1;
    else what |= fc_what(fc, v);
    if (what & FC_IRQ) fc->irq = 1;
    if ((what & FC_IRQ_END) && fc->inhibit) fc->irq = 0;
    if (what & FC_WRAP) fc->one_at = t + 1;
    if (what & FC_QUARTER) quarter_frame(s);
    if (what & FC_HALF) sweep_clock(s);
    length_clock(s, (what & FC_HALF) != 0);
    /* The halt flags take effect on the length counters from the next cycle. */
    s->length[0].halt = (s->reg[0x0] >> 5) & 1;
    s->length[1].halt = (s->reg[0x4] >> 5) & 1;
    s->length[2].halt = (s->reg[0x8] >> 7) & 1;
    s->length[3].halt = (s->reg[0xC] >> 5) & 1;
    s->dirty = 1;
    schedule(s, t + 1);
}

/* Cycles from t up to (not including) end that have no slow work; returns
 * where it stopped (earlier when the DMC asks for a fetch on the way). */
static uint64_t run_fast(Synth2A03 *s, uint64_t t, uint64_t end)
{
    for (; t < end; t++) {
        if (t & 1) {                               /* get cycle: pulse timers, DMC */
            for (int k = 0; k < 2; k++) {
                SynthPulse *p = &s->pulse[k];
                if (p->timer) {
                    p->timer--;
                } else {
                    p->timer = p->period;
                    p->step = (p->step + 1) & 7;
                    if (s->live[k]) s->dirty = 1;
                }
            }
            if (--s->dmc.timer == 0) {
                s->dmc.timer = s->dmc.rate >> 1;
                dmc_bit(s, t);
                if (s->slow_at < end) end = s->slow_at;
            }
        }
        SynthTriangle *tr = &s->tri;
        if (tr->timer) {
            tr->timer--;
        } else {
            tr->timer = tr->period;
            if (s->length[2].count && tr->linear) {
                tr->step = (tr->step + 1) & 31;
                if (s->live[2]) s->dirty = 1;
            }
        }
        SynthNoise *nz = &s->noise;
        if (nz->timer) {
            nz->timer--;
        } else {
            unsigned tap = (s->reg[0xE] & 0x80) ? 6 : 1;
            nz->timer = (uint16_t)(nz->period - 1);
            nz->lfsr = (uint16_t)((nz->lfsr >> 1) | (((nz->lfsr ^ (nz->lfsr >> tap)) & 1) << 14));
            if (s->live[3]) s->dirty = 1;
        }
        if (s->dirty) mix(s);
        /* The output sample is the mean of its cycles' levels. */
        s->acc += s->level;
        s->acc_cycles++;
        s->phase += 1.0;
        if (s->phase >= s->cycles_per_sample) {
            s->phase -= s->cycles_per_sample;
            emit(s, s->acc / s->acc_cycles);
            s->acc = 0;
            s->acc_cycles = 0;
        }
    }
    return t;
}

void synth_run(Synth2A03 *s, uint64_t cycle)
{
    while (s->now < cycle) {
        uint64_t t = s->now;
        if (t >= s->slow_at) slow_cycle(s, t);
        s->now = run_fast(s, t, s->slow_at < cycle ? s->slow_at : cycle);
    }
}

/* ------------------------------------------------------------------------- */
/* Interface                                                                 */
/* ------------------------------------------------------------------------- */

void synth_init(Synth2A03 *s, int sample_rate, SynthDmcRead read, void *user)
{
    memset(s, 0, sizeof *s);
    if (sample_rate <= 0) sample_rate = 48000;
    s->sample_rate = sample_rate;
    s->read = read;
    s->user = user;

    /* The nonlinear mixer (nesdev wiki, APU Mixer), as lookup tables. */
    for (int i = 0; i < 31; i++) s->pulse_table[i] = i ? (float)(95.52 / (8128.0 / i + 100)) : 0.0f;
    for (int i = 0; i < 203; i++) s->tnd_table[i] = i ? (float)(163.67 / (24329.0 / i + 100)) : 0.0f;

    /* The front-loader's output: 90 Hz and 440 Hz high-pass, 14 kHz low-pass. */
    double dt = 1.0 / sample_rate;
    double rc90 = 1.0 / (2 * PI_15 * 90), rc440 = 1.0 / (2 * PI_15 * 440), rc14k = 1.0 / (2 * PI_15 * 14000);
    s->hp90_a = rc90 / (rc90 + dt);
    s->hp440_a = rc440 / (rc440 + dt);
    s->lp_a = dt / (rc14k + dt);
    s->cycles_per_sample = SYNTH_CPU_HZ / sample_rate;

    /* Power-on: what the reference console measurements give (the DMC's first
     * output bit 1022 cycles in), the noise register at 1, the triangle at
     * the top of its ramp. */
    s->noise.period = noise_periods[0];
    s->noise.lfsr = 1;
    s->tri.out = triangle_shape(0);
    s->dmc.rate = dmc_rates[0];
    s->dmc.timer = 1022 / 2;
    s->dmc.bits = 8;
    s->dmc.silent = 1;
    s->dmc.start = s->dmc.addr = 0xC000;
    s->dmc.length = 1;
    s->dmc.fetch_at = SYNTH_NEVER;
    s->fc.one_at = 0;
    s->fc.reset_at = s->fc.ack_at = SYNTH_NEVER;
    s->dirty = 1;
    schedule(s, 0);
}

static void status_write(Synth2A03 *s, uint8_t v, uint64_t t)
{
    for (int ch = 0; ch < 4; ch++) {
        s->length[ch].enabled = (v >> ch) & 1;
        if (!s->length[ch].enabled) s->length[ch].count = 0;
    }
    SynthDmc *d = &s->dmc;
    d->irq = 0;
    if (!(v & 0x10)) {
        d->remaining = 0;
        d->fetch_at = SYNTH_NEVER;
    } else if (!d->remaining) {
        d->addr = d->start;
        d->remaining = d->length;
        /* An empty buffer is filled on the first get cycle 5 or more later. */
        if (!d->buffer_full && d->fetch_at == SYNTH_NEVER) d->fetch_at = (t + 5) | 1;
    }
}

void synth_write(Synth2A03 *s, uint16_t addr, uint8_t v, uint64_t cycle)
{
    if (addr < 0x4000 || addr > 0x4017 || addr == 0x4014 || addr == 0x4016) return;
    synth_run(s, cycle);
    uint64_t t = s->now;
    if (addr <= 0x400F) s->reg[addr & 15] = v;
    switch (addr) {
    case 0x4001:
    case 0x4005:
        s->pulse[(addr >> 2) & 1].sweep_reload = 1;
        break;
    case 0x4002:
    case 0x4006: {
        SynthPulse *p = &s->pulse[(addr >> 2) & 1];
        p->period = (uint16_t)((p->period & 0x700) | v);
        break;
    }
    case 0x4003:
    case 0x4007: {
        int k = (addr >> 2) & 1;
        SynthPulse *p = &s->pulse[k];
        length_load(s, k, v);
        p->period = (uint16_t)((p->period & 0xFF) | (v & 7) << 8);
        p->step = 0;
        p->env.start = 1;
        break;
    }
    case 0x400A:
        s->tri.period = (uint16_t)((s->tri.period & 0x700) | v);
        break;
    case 0x400B:
        length_load(s, 2, v);
        s->tri.period = (uint16_t)((s->tri.period & 0xFF) | (v & 7) << 8);
        s->tri.linear_reload = 1;
        break;
    case 0x400E:
        s->noise.period = noise_periods[v & 15];
        break;
    case 0x400F:
        length_load(s, 3, v);
        s->noise.env.start = 1;
        break;
    case 0x4010:
        s->dmc.irq_enable = v >> 7;
        s->dmc.loop = (v >> 6) & 1;
        s->dmc.rate = dmc_rates[v & 15];
        if (!s->dmc.irq_enable) s->dmc.irq = 0;
        break;
    case 0x4011:
        s->dmc.level = v & 0x7F;
        break;
    case 0x4012:
        s->dmc.start = (uint16_t)(0xC000 | v << 6);
        break;
    case 0x4013:
        s->dmc.length = (uint16_t)(v << 4 | 1);
        break;
    case 0x4015:
        status_write(s, v, t);
        break;
    case 0x4017:
        s->fc.five_step = v >> 7;
        s->fc.inhibit = (v >> 6) & 1;
        if (s->fc.five_step) s->fc.pending = FC_QUARTER | FC_HALF;   /* clocked at once */
        if (s->fc.inhibit) s->fc.irq = 0;
        s->fc.reset_at = t + ((t & 1) ? 4 : 3);
        break;
    default:   /* $4000/$4004/$4008/$400C and the unused $4009/$400D: the register is all */
        break;
    }
    s->dirty = 1;
    if (s->slow_at > t) s->slow_at = t;   /* this cycle lands the write's effects on the counters */
}

uint8_t synth_read_status(Synth2A03 *s, uint64_t cycle)
{
    synth_run(s, cycle);
    uint8_t v = 0;
    for (int ch = 0; ch < 4; ch++)
        if (s->length[ch].count) v |= (uint8_t)(1 << ch);
    if (s->dmc.remaining) v |= 0x10;
    if (s->fc.irq) v |= 0x40;
    if (s->dmc.irq) v |= 0x80;
    s->fc.ack_at = s->now + (s->now & 1);
    if (s->fc.ack_at < s->slow_at) s->slow_at = s->fc.ack_at;
    return v;
}

int synth_take_samples(Synth2A03 *s, int16_t *out, int max)
{
    int n = 0;
    while (n < max && s->tail != s->head) out[n++] = s->ring[s->tail++ & (SYNTH_RING - 1)];
    return n;
}

bool synth_irq(const Synth2A03 *s)
{
    return (s->fc.irq && !s->fc.inhibit) || s->dmc.irq;
}
