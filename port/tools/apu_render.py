"""Render 2A03 register writes to a WAV (48 kHz mono, 16-bit), with a simple APU model.

    # the driver model playing commands from silence (music and/or effects):
    py -3 -I port/tools/apu_render.py ROM --play 0C --frames 1800 --out runs/sound/title.wav
    # the driver model fed the commands the game queued in an oracle RAM log:
    py -3 -I port/tools/apu_render.py ROM --ramfile runs/sound/cody.ram --from 4770 --to 6000 --out runs/sound/r1.wav
    # the oracle's own register writes (sound_oracle.py apu):
    py -3 -I port/tools/apu_render.py ROM --apu runs/sound/r1.apu --out runs/sound/r1_oracle_regs.wav

The model follows the nesdev descriptions the cycle runner's hw_apu.c
implements: pulse duty sequencer (reset by $4003/$4007), sweep mute (period
< 8 or target > $7FF), constant volume or envelope, length counters; the
triangle's 32-step sequencer gated by the linear counter (control flag, reload
flag) and length counter, periods 0-1 held; the 15-bit noise LFSR (mode bit
from $400E); the DMC output level from $4011 (the game never plays samples,
so DMA playback is not modelled); the 4-step frame sequencer. Each channel's
output is averaged over the sample interval and mixed with the runner's
lookup-table mixer (95.52 / (8128 / n + 100), 163.67 / (24329 / n + 100)),
then through the NES front-loader stage (90 Hz and 440 Hz high-pass, 14 kHz
low-pass) at 30000 full scale, as hw_apu.c does.

Driver writes from the model have no cycle: the i-th write of a frame is
placed WRITE_AT + i * WRITE_STEP CPU cycles after the frame's start (the
oracle's traces: the driver's first write ~1,500-2,000 cycles after the NMI,
then one every ~290 cycles).
Output files are ROM-derived: write them under runs/sound/.
"""
import argparse
import math
import os
import struct
import sys
import wave

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sound_rom as sr  # noqa: E402

CPU_HZ = 1789773.0
FRAME_CYCLES = 29780.5
WRITE_AT = 1500          # medians in the oracle's traces: first driver write 1,468-1,999 cycles
WRITE_STEP = 290         # after the NMI, then one write every ~290 cycles
LENGTHS = [10, 254, 20, 2, 40, 4, 80, 6, 160, 8, 60, 10, 14, 12, 26, 14,
           12, 16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30]
NOISE_PERIODS = [4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068]
DUTY = [0x40, 0x60, 0x78, 0x9F]          # bit (7 - step) = high, as hw_apu.c
TRI_SEQ = [15 - i for i in range(16)] + list(range(16))
FS_QUARTER = (7457, 14913, 22371, 29829)  # 4-step frame sequencer, CPU cycles
FS_PERIOD = 29830


class Envelope:
    def __init__(self):
        self.start = False
        self.divider = 0
        self.decay = 0

    def clock(self, reg):
        if self.start:
            self.start = False
            self.decay = 15
            self.divider = reg & 15
        elif self.divider == 0:
            self.divider = reg & 15
            if self.decay:
                self.decay -= 1
            elif reg & 0x20:
                self.decay = 15
        else:
            self.divider -= 1

    def volume(self, reg):
        return reg & 15 if reg & 0x10 else self.decay


class Pulse:
    def __init__(self, ones_complement):
        self.ones = ones_complement
        self.reg = [0, 0, 0, 0]
        self.period = 0
        self.step = 0
        self.left = 2.0               # CPU cycles until the next sequencer step
        self.length = 0
        self.enabled = False
        self.env = Envelope()
        self.sweep_div = 0
        self.sweep_reload = False

    def write(self, r, v):
        self.reg[r] = v
        if r == 1:
            self.sweep_reload = True
        elif r == 2:
            self.period = (self.period & 0x700) | v
        elif r == 3:
            self.period = (self.period & 0xFF) | (v & 7) << 8
            if self.enabled:
                self.length = LENGTHS[v >> 3]
            self.step = 0
            self.env.start = True

    def target(self):
        r = self.reg[1]
        change = self.period >> (r & 7)
        if r & 0x08:
            return self.period - change - (1 if self.ones else 0)
        return self.period + change

    def muted(self):
        return self.length == 0 or self.period < 8 or self.target() > 0x7FF

    def half_frame(self):
        if self.length and not self.reg[0] & 0x20:
            self.length -= 1
        r = self.reg[1]
        if self.sweep_div == 0 and r & 0x80 and r & 7 and not self.muted():
            t = self.target()
            if t >= 0:
                self.period = t
        if self.sweep_div == 0 or self.sweep_reload:
            self.sweep_div = (r >> 4) & 7
            self.sweep_reload = False
        else:
            self.sweep_div -= 1

    def run(self, cycles):
        """Average output (0-15) over the next `cycles` CPU cycles."""
        step_len = 2.0 * (self.period + 1)
        vol = 0 if self.muted() else self.env.volume(self.reg[0])
        duty = DUTY[self.reg[0] >> 6]
        if vol == 0:                   # keep the sequencer moving, cheaply
            t = cycles - self.left
            if t >= 0:
                n = int(t // step_len) + 1
                self.step = (self.step + n) & 7
                self.left = step_len * n - t
            else:
                self.left = -t
            return 0.0
        acc = 0.0
        t = cycles
        while t > 0:
            seg = self.left if self.left < t else t
            if (duty >> (7 - self.step)) & 1:
                acc += seg
            t -= seg
            self.left -= seg
            if self.left <= 1e-9:
                self.left += step_len
                self.step = (self.step + 1) & 7
        return acc * vol / cycles


class Triangle:
    def __init__(self):
        self.reg = [0, 0, 0, 0]
        self.period = 0
        self.step = 0
        self.left = 1.0
        self.length = 0
        self.enabled = False
        self.linear = 0
        self.reload = False
        self.out = 0

    def write(self, r, v):
        self.reg[r] = v
        if r == 2:
            self.period = (self.period & 0x700) | v
        elif r == 3:
            self.period = (self.period & 0xFF) | (v & 7) << 8
            if self.enabled:
                self.length = LENGTHS[v >> 3]
            self.reload = True

    def quarter_frame(self):
        if self.reload:
            self.linear = self.reg[0] & 0x7F
        elif self.linear:
            self.linear -= 1
        if not self.reg[0] & 0x80:
            self.reload = False

    def half_frame(self):
        if self.length and not self.reg[0] & 0x80:
            self.length -= 1

    def run(self, cycles):
        if self.period < 2 or not (self.length and self.linear):
            return float(self.out)    # holds its level (hw_apu.c)
        step_len = float(self.period + 1)
        acc = 0.0
        t = cycles
        while t > 0:
            seg = self.left if self.left < t else t
            acc += seg * TRI_SEQ[self.step]
            t -= seg
            self.left -= seg
            if self.left <= 1e-9:
                self.left += step_len
                self.step = (self.step + 1) & 31
        self.out = TRI_SEQ[self.step]
        return acc / cycles


class Noise:
    def __init__(self):
        self.reg = [0, 0, 0, 0]
        self.period = NOISE_PERIODS[0]
        self.lfsr = 1
        self.left = 4.0
        self.length = 0
        self.enabled = False
        self.env = Envelope()

    def write(self, r, v):
        self.reg[r] = v
        if r == 2:
            self.period = NOISE_PERIODS[v & 15]
        elif r == 3:
            if self.enabled:
                self.length = LENGTHS[v >> 3]
            self.env.start = True

    def half_frame(self):
        if self.length and not self.reg[0] & 0x20:
            self.length -= 1

    def run(self, cycles):
        vol = self.env.volume(self.reg[0]) if self.length else 0
        tap = 6 if self.reg[2] & 0x80 else 1
        acc = 0.0
        t = cycles
        while t > 0:
            seg = self.left if self.left < t else t
            if vol and not self.lfsr & 1:
                acc += seg
            t -= seg
            self.left -= seg
            if self.left <= 1e-9:
                self.left += self.period
                fb = (self.lfsr ^ (self.lfsr >> tap)) & 1
                self.lfsr = (self.lfsr >> 1) | (fb << 14)
        return acc * vol / cycles


class Apu:
    def __init__(self, rate=48000):
        self.p1, self.p2 = Pulse(True), Pulse(False)
        self.tri, self.noise = Triangle(), Noise()
        self.dmc_level = 0
        self.fs_cycle = 0.0
        self.rate = rate
        dt = 1.0 / rate

        def hp(f):
            rc = 1.0 / (2 * math.pi * f)
            return rc / (rc + dt)
        rc14k = 1.0 / (2 * math.pi * 14000)
        self.hp90_a, self.hp440_a, self.lp_a = hp(90), hp(440), dt / (rc14k + dt)
        self.h90_in = self.h90_out = self.h440_in = self.h440_out = self.lp = 0.0

    def write(self, addr, v):
        if 0x4000 <= addr <= 0x4003:
            self.p1.write(addr - 0x4000, v)
        elif 0x4004 <= addr <= 0x4007:
            self.p2.write(addr - 0x4004, v)
        elif 0x4008 <= addr <= 0x400B:
            self.tri.write(addr - 0x4008, v)
        elif 0x400C <= addr <= 0x400F:
            self.noise.write(addr - 0x400C, v)
        elif addr == 0x4011:
            self.dmc_level = v & 0x7F
        elif addr == 0x4015:
            for bit, ch in ((1, self.p1), (2, self.p2), (4, self.tri), (8, self.noise)):
                ch.enabled = bool(v & bit)
                if not ch.enabled:
                    ch.length = 0

    def frame_sequencer(self, cycles):
        start = self.fs_cycle
        end = start + cycles
        for q in FS_QUARTER:
            for base in (0.0, FS_PERIOD):
                at = q + base
                if start <= at < end:
                    for env_ch in (self.p1, self.p2, self.noise):
                        env_ch.env.clock(env_ch.reg[0])
                    self.tri.quarter_frame()
                    if q in (14913, 29829):
                        for ch in (self.p1, self.p2, self.tri, self.noise):
                            ch.half_frame()
        self.fs_cycle = end % FS_PERIOD

    def sample(self, cycles):
        self.frame_sequencer(cycles)
        p = self.p1.run(cycles) + self.p2.run(cycles)
        tnd = 3 * self.tri.run(cycles) + 2 * self.noise.run(cycles) + self.dmc_level
        level = (95.52 / (8128.0 / p + 100) if p > 0 else 0.0) + (163.67 / (24329.0 / tnd + 100) if tnd > 0 else 0.0)
        hp = self.hp90_a * (self.h90_out + level - self.h90_in)
        self.h90_in, self.h90_out = level, hp
        hp2 = self.hp440_a * (self.h440_out + hp - self.h440_in)
        self.h440_in, self.h440_out = hp, hp2
        self.lp += self.lp_a * (hp2 - self.lp)
        s = int(self.lp * 30000.0)
        return max(-32768, min(32767, s))


def render(writes, frames, rate=48000):
    """writes: [(cycle, addr, value)] sorted by cycle, cycle 0 = start of the
    first frame. Returns 16-bit samples."""
    apu = Apu(rate)
    per = CPU_HZ / rate
    total = frames * FRAME_CYCLES
    n = int(total / per)
    out = []
    wi = 0
    for i in range(n):
        t1 = (i + 1) * per
        while wi < len(writes) and writes[wi][0] < t1:
            apu.write(writes[wi][1], writes[wi][2])
            wi += 1
        out.append(apu.sample(per))
    return out


def write_wav(path, samples, rate=48000):
    if "runs" + os.sep + "sound" not in os.path.abspath(path) and "runs/sound" not in os.path.abspath(path).replace("\\", "/"):
        print(f"warning: {path} is outside runs/sound/ (WAVs are ROM-derived)", file=sys.stderr)
    with wave.open(path, "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(rate)
        w.writeframes(struct.pack(f"<{len(samples)}h", *samples))


def model_writes(rom, frame_cmds, frames, ram=None):
    """Run sound_model for `frames` frames; frame_cmds: {frame index: [commands]}."""
    import sound_model
    drv = sound_model.Driver(rom, ram)
    out = []
    for f in range(frames):
        for i, (addr, v) in enumerate(drv.update(frame_cmds.get(f, []))):
            out.append((f * FRAME_CYCLES + WRITE_AT + i * WRITE_STEP, addr, v))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("rom")
    ap.add_argument("--play", nargs="*", default=[], help="commands (hex) played at frame 0")
    ap.add_argument("--frames", type=int, default=600)
    ap.add_argument("--ramfile", help="oracle RAM log: replay the commands the game queued")
    ap.add_argument("--from", dest="first", type=int)
    ap.add_argument("--to", dest="last", type=int)
    ap.add_argument("--apu", help="oracle APU write log (sound_oracle.py apu)")
    ap.add_argument("--out", required=True)
    ap.add_argument("--rate", type=int, default=48000)
    a = ap.parse_args()
    rom = sr.Rom(a.rom)
    if a.apu:
        import sound_oracle as so
        starts = {}
        trace = so.read_apu(a.apu, starts)
        f0, f1 = min(trace), max(trace)
        writes = []
        for f in range(f0, f1 + 1):
            for cyc, addr, v in trace.get(f, []):
                # each frame was traced by its own run: place it by its own NMI cycle
                rel = cyc - starts[f] if f in starts else WRITE_AT
                writes.append(((f - f0) * FRAME_CYCLES + rel, addr, v))
        writes.sort()
        frames = f1 - f0 + 1
    elif a.ramfile:
        import sound_oracle as so
        import sound_check
        import sound_model
        ram = so.read_ram(a.ramfile)
        first = a.first if a.first is not None else min(ram)
        last = a.last if a.last is not None else max(ram)
        start = bytearray(0x800)
        for addr in sound_model.ALL:
            start[addr] = ram[first][addr]
        cmds = {f - first - 1: sound_check.pending(ram[f - 1]) for f in range(first + 1, last + 1) if f - 1 in ram}
        frames = last - first
        writes = model_writes(rom, cmds, frames, start)
    else:
        frames = a.frames
        writes = model_writes(rom, {0: [int(c, 16) for c in a.play]}, frames)
    samples = render(writes, frames, a.rate)
    write_wav(a.out, samples, a.rate)
    print(f"{a.out}: {frames} frames, {len(writes)} register writes, {len(samples) / a.rate:.2f} s")


if __name__ == "__main__":
    main()
