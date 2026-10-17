"""Write an NROM program that drives every part of the APU Mighty Final Fight
leaves alone, for synth_replay against the cycle runner (cyc_interp):
envelopes that decay and loop, sweeps that mute both ways, the triangle's
linear counter, noise in both modes, length counters running out, halts,
$4015 enables, the 4-step frame interrupt (polled, interrupts stay masked)
and the 5-step mode, and the DMC: samples with and without loop and IRQ,
every few rates, $4011 writes, and $4015 reads all along.

    py -3 -I port/tests/synth/make_apu_rom.py OUT.nes

The program is a small interpreter: a script of register writes, $4015 reads,
frame waits and short spins (which move later writes to other cycles of the
frame). Writes go through a STA abs in RAM, so no dummy read touches $4015.
The DMC plays bytes from $C000 (a fixed pseudo-random table). The output is
generated, not taken from any game, but keep it with the other test output
under runs/.
"""
import sys

ZP_FRAME, ZP_PTR, ZP_LAST = 0x00, 0x02, 0x07
STUB = 0x0200                      # STA abs / RTS, filled at reset


class Asm:
    def __init__(self, org):
        self.org, self.code, self.labels, self.fix = org, bytearray(), {}, []

    def here(self):
        return self.org + len(self.code)

    def label(self, name):
        self.labels[name] = self.here()

    def b(self, *bs):
        self.code.extend(x & 0xFF for x in bs)

    def abs_(self, op, target):
        self.b(op, 0, 0)
        self.fix.append((len(self.code) - 2, target, "abs"))

    def rel(self, op, target):
        self.b(op, 0)
        self.fix.append((len(self.code) - 1, target, "rel"))

    def resolve(self):
        for at, target, kind in self.fix:
            addr = self.labels[target] if isinstance(target, str) else target
            if kind == "abs":
                self.code[at], self.code[at + 1] = addr & 0xFF, addr >> 8
            else:
                off = addr - (self.org + at + 1)
                assert -128 <= off <= 127, target
                self.code[at] = off & 0xFF


def program():
    a = Asm(0x8000)
    a.label("reset")
    a.b(0x78, 0xD8, 0xA2, 0xFF, 0x9A)                      # SEI CLD LDX #$FF TXS
    a.b(0xA9, 0x00, 0x8D, 0x00, 0x20, 0x8D, 0x01, 0x20)    # LDA #0 STA $2000 STA $2001
    a.b(0x85, ZP_FRAME)                                    # STA frame
    for name in ("vb1", "vb2"):                            # two vblanks: the PPU is up
        a.label(name)
        a.b(0x2C, 0x02, 0x20)                              # BIT $2002
        a.rel(0x10, name)                                  # BPL
    a.b(0xA9, 0x8D, 0x8D, STUB & 0xFF, STUB >> 8)          # STA abs opcode into the stub
    a.b(0xA9, 0x60, 0x8D, (STUB + 3) & 0xFF, (STUB + 3) >> 8)   # RTS after it
    a.b(0xA9, 0)                                           # LDA #<script (patched below)
    lo_at = len(a.code) - 1
    a.b(0x85, ZP_PTR, 0xA9, 0)                             # STA ptr / LDA #>script
    hi_at = len(a.code) - 1
    a.b(0x85, ZP_PTR + 1)
    a.b(0xA9, 0x80, 0x8D, 0x00, 0x20)                      # NMI on

    a.label("next")
    a.b(0xA0, 0x00, 0xB1, ZP_PTR)                          # LDY #0 / LDA (ptr),Y
    a.rel(0xF0, "done")                                    # 0: end
    a.b(0xC9, 1)
    a.rel(0xF0, "write")
    a.b(0xC9, 2)
    a.rel(0xF0, "read")
    a.b(0xC9, 3)
    a.rel(0xF0, "wait")
    # 4: spin n * 5 cycles
    a.b(0xC8, 0xB1, ZP_PTR, 0xAA)                          # INY LDA (ptr),Y TAX
    a.label("spin")
    a.b(0xCA)                                              # DEX
    a.rel(0xD0, "spin")                                    # BNE
    a.b(0xA9, 2)
    a.abs_(0x4C, "advance")

    a.label("write")                                       # 1 lo hi value
    a.b(0xC8, 0xB1, ZP_PTR, 0x8D, (STUB + 1) & 0xFF, (STUB + 1) >> 8)
    a.b(0xC8, 0xB1, ZP_PTR, 0x8D, (STUB + 2) & 0xFF, (STUB + 2) >> 8)
    a.b(0xC8, 0xB1, ZP_PTR)
    a.abs_(0x20, STUB)                                     # JSR stub: STA addr
    a.b(0xA9, 4)
    a.abs_(0x4C, "advance")

    a.label("read")                                        # 2
    a.b(0xAD, 0x15, 0x40, 0x85, ZP_LAST)                   # LDA $4015 / STA last
    a.b(0xA9, 1)
    a.abs_(0x4C, "advance")

    a.label("wait")                                        # 3 n: n frames
    a.b(0xC8, 0xB1, ZP_PTR, 0xAA)
    a.label("wait_frame")
    a.b(0xA5, ZP_FRAME)
    a.label("wait_nmi")
    a.b(0xC5, ZP_FRAME)
    a.rel(0xF0, "wait_nmi")
    a.b(0xCA)
    a.rel(0xD0, "wait_frame")
    a.b(0xA9, 2)

    a.label("advance")                                     # ptr += A
    a.b(0x18, 0x65, ZP_PTR, 0x85, ZP_PTR, 0xA5, ZP_PTR + 1, 0x69, 0x00, 0x85, ZP_PTR + 1)
    a.abs_(0x4C, "next")

    a.label("done")
    a.abs_(0x4C, "done")
    a.label("nmi")
    a.b(0xE6, ZP_FRAME, 0x40)                              # INC frame / RTI
    a.label("irq")
    a.b(0x40)

    a.label("script")
    a.code.extend(script())
    a.resolve()
    a.code[lo_at], a.code[hi_at] = a.labels["script"] & 0xFF, a.labels["script"] >> 8
    return a


def W(addr, value):
    return [1, addr & 0xFF, addr >> 8, value]


def R():
    return [2]


def WAIT(frames):
    out = []
    while frames > 0:
        out += [3, min(frames, 255)]
        frames -= 255
    return out


def SPIN(n):
    out = []
    while n > 0:
        out += [4, min(n, 255)]
        n -= 255
    return out


def script():
    s = []
    s += W(0x4017, 0x40) + W(0x4015, 0x0F)
    # Envelopes: pulse 1 decays once (period 15), pulse 2 loops (period 3), constant noise.
    s += W(0x4000, 0x0F) + W(0x4001, 0x08) + W(0x4002, 0xFD) + W(0x4003, 0x00)
    s += W(0x4004, 0x63) + W(0x4005, 0x08) + W(0x4006, 0x7E) + W(0x4007, 0x00)
    s += WAIT(60) + R()
    # Duty cycles and constant volume, a few periods, $4003 rewrites mid-note (phase reset).
    for duty in range(4):
        s += W(0x4000, 0x30 | duty << 6 | 9) + W(0x4002, 0x40 + 16 * duty) + W(0x4003, 0x01) + WAIT(6) + SPIN(37 * duty + 11)
        s += W(0x4003, 0x01) + WAIT(5)
    # Sweeps: pulse 1 adds (falls to the $7FF mute), pulse 2 negates (rises to the period-8 mute).
    s += W(0x4000, 0xBF) + W(0x4002, 0x00) + W(0x4003, 0x02) + W(0x4001, 0xA2)
    s += W(0x4004, 0x7F) + W(0x4006, 0x40) + W(0x4007, 0x01) + W(0x4005, 0x9B)
    s += WAIT(40) + R() + WAIT(40) + R()
    # Pulse 1 negating (ones' complement), shift 1; disabled sweep with shift 0 add: mute above $3FF.
    s += W(0x4001, 0x89) + W(0x4002, 0x80) + W(0x4003, 0x03) + WAIT(40)
    s += W(0x4001, 0x00) + W(0x4002, 0xFF) + W(0x4003, 0x03) + WAIT(20) + W(0x4003, 0x04) + WAIT(20)
    s += W(0x4000, 0x30) + W(0x4004, 0x30) + W(0x4001, 0x08) + W(0x4005, 0x08)
    # Length counters running out (no halt), status reads on the way; halt; $4015 disable.
    s += W(0x4000, 0x1A) + W(0x4002, 0xA0) + W(0x4003, 0x18)            # length 2 half frames
    s += W(0x4004, 0x1A) + W(0x4006, 0xB0) + W(0x4007, 0x58)            # length 4
    s += R() + SPIN(200) + R() + WAIT(1) + R() + WAIT(1) + R() + WAIT(2) + R()
    s += W(0x4000, 0x3A) + W(0x4003, 0x18) + WAIT(20) + R()             # halted: keeps sounding
    s += W(0x4015, 0x0E) + R() + W(0x4003, 0x18) + R() + W(0x4015, 0x0F) + W(0x4000, 0x30)
    # Triangle: linear counter 24 quarter frames, then held (control), then reload rewrites.
    s += W(0x4008, 0x18) + W(0x400A, 0x80) + W(0x400B, 0x09) + WAIT(15) + R()
    s += W(0x4008, 0x9F) + W(0x400B, 0x01) + WAIT(20) + W(0x4008, 0x1F) + WAIT(20) + R()
    s += W(0x4008, 0xFF) + W(0x400B, 0x08) + WAIT(10)
    s += W(0x400A, 0x01) + W(0x400B, 0x00) + WAIT(5)                    # period 1: held output
    s += W(0x400A, 0x00) + WAIT(3) + W(0x400A, 0x55) + W(0x400B, 0x08) + WAIT(10)
    s += W(0x4008, 0x00) + WAIT(5)
    # Noise: both modes, every period, envelope decay and loop.
    for i in range(16):
        s += W(0x400C, 0x30 | (15 - i % 8)) + W(0x400E, (0x80 if i & 1 else 0) | i) + W(0x400F, 0x08) + WAIT(3)
    s += W(0x400C, 0x02) + W(0x400E, 0x84) + W(0x400F, 0xF8) + WAIT(40) + R()
    s += W(0x400C, 0x21) + W(0x400E, 0x0A) + W(0x400F, 0x00) + WAIT(30) + W(0x400C, 0x30)
    # Frame counter: 4-step with the interrupt on (polled), reads around the flag; 5-step.
    s += W(0x4017, 0x00)
    for k in range(12):
        s += WAIT(1) + SPIN(50 * k + 7) + R() + R()
    s += W(0x4017, 0x80) + W(0x4000, 0x16) + W(0x4003, 0x08) + WAIT(30) + R()
    s += W(0x4017, 0xC0) + WAIT(10) + R() + W(0x4017, 0x00) + WAIT(3) + R() + W(0x4017, 0x40) + R()
    s += W(0x4000, 0x30)
    # DMC: one-shot at the fastest rate with $4015 reads; IRQ at the end; loop; rate changes.
    s += W(0x4010, 0x0F) + W(0x4012, 0x00) + W(0x4013, 0x04) + W(0x4011, 0x40) + W(0x4015, 0x1F)
    s += R() + SPIN(250) + R() + WAIT(1) + R() + WAIT(1) + R()
    s += W(0x4010, 0x8E) + W(0x4013, 0x02) + W(0x4015, 0x1F) + R() + WAIT(1) + R() + R()
    s += W(0x4015, 0x1F) + R() + WAIT(2) + R() + W(0x4010, 0x0E) + R()
    s += W(0x4010, 0x4C) + W(0x4012, 0x04) + W(0x4013, 0x01) + W(0x4015, 0x1F) + WAIT(20) + R()
    for rate in (0x0, 0x3, 0x7, 0xA, 0xD):
        s += W(0x4010, 0x40 | rate) + WAIT(8) + R()
    s += W(0x4015, 0x0F) + R() + WAIT(2)
    # Direct $4011 writes (sample playback by the CPU), then a sample started mid-level.
    for i in range(48):
        s += W(0x4011, (i * 37) & 0x7F) + SPIN(20 + (i * 13) % 40)
    s += W(0x4010, 0x09) + W(0x4012, 0x10) + W(0x4013, 0x08) + W(0x4015, 0x1F) + WAIT(10) + R()
    s += W(0x4015, 0x0F) + W(0x4011, 0x00) + WAIT(2)
    # Everything at once for a while.
    s += W(0x4000, 0xB8) + W(0x4002, 0x40) + W(0x4003, 0x01) + W(0x4004, 0x7A) + W(0x4006, 0x60) + W(0x4007, 0x01)
    s += W(0x4008, 0xC0) + W(0x400A, 0x20) + W(0x400B, 0x02) + W(0x400C, 0x34) + W(0x400E, 0x07) + W(0x400F, 0x00)
    s += W(0x4010, 0x4F) + W(0x4012, 0x00) + W(0x4013, 0x20) + W(0x4015, 0x1F) + WAIT(60) + R()
    s += W(0x4015, 0x00) + R() + WAIT(10)
    return bytes(s + [0])


def samples():
    """DMC bytes at $C000: a fixed pseudo-random table."""
    out, x = bytearray(), 0x2F
    for _ in range(0x2000):
        x = (x * 1103515245 + 12345) & 0x7FFFFFFF
        out.append((x >> 16) & 0xFF)
    return out


def main():
    if len(sys.argv) != 2:
        sys.exit(__doc__)
    a = program()
    prg = bytearray(0x8000)
    prg[:len(a.code)] = a.code
    assert len(a.code) < 0x4000, "program overlaps the sample area"
    data = samples()
    prg[0x4000:0x4000 + len(data)] = data
    for vec, name in ((0xFFFA, "nmi"), (0xFFFC, "reset"), (0xFFFE, "irq")):
        prg[vec - 0x8000], prg[vec - 0x8000 + 1] = a.labels[name] & 0xFF, a.labels[name] >> 8
    header = bytes([0x4E, 0x45, 0x53, 0x1A, 2, 1, 0, 0]) + bytes(8)       # NROM-256, 8 KB CHR
    with open(sys.argv[1], "wb") as f:
        f.write(header + prg + bytes(0x2000))
    print(f"{sys.argv[1]}: {len(a.code)} bytes of program and script")


if __name__ == "__main__":
    main()
