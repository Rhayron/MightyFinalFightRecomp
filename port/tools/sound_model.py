"""An independent model of Mighty Final Fight's sound driver (banks $06/$07).

    from sound_model import Driver
    drv = Driver(sound_rom.Rom(path))          # or Driver(rom, ram=bytearray(...))
    drv.play(0x0C)                              # what sound_play ($8003) does with A = $0C
    writes = drv.tick()                         # what sound_tick ($8000) does: [(addr, value)]
    writes = drv.update([0x17, 0x18])           # sound_update (0F:FF72): play each, then tick

Written from the disassembly as structured Python, one method per driver
routine (named after its address), keeping the driver's RAM layout so the
result can be compared byte for byte with the oracle's frame log:

    $C0-$D8      driver variables (docs/sound.md, "RAM")
    $0700-$0727  effect channels 0-3, ten 4-byte arrays (index x = channel)
    $0728-$077B  music channels: the same arrays at x = $28 + channel, plus
                 the sequencer arrays $0750-$077B (x = $28 + channel again)
    $077C-$077F  last value written to each channel's period-high register

Channel index x: 3 = pulse 1 ($4000), 2 = pulse 2 ($4004), 1 = triangle
($4008), 0 = noise ($400C). Writes to $4000-$4017 come back from tick()/
play() as (address, value) in the order the driver makes them.

Scratch bytes $C1-$C4 are reproduced too (every routine that writes them
does it here as well), with one input the model cannot know: special
commands $F0-$F7 store the caller's Y register in $C3 (06:81AE). play()
takes it as `y` (default 0).
"""
import sound_rom as sr

# zero-page variables
C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, CA, CB, CC, CD, CE, CF = range(0xC0, 0xD0)
D0, D1, D2, D3, D4, D5, D6, D7, D8 = range(0xD0, 0xD9)

# per-channel arrays (base + x)
INSTR = 0x700      # instrument number (1-based; 0 = none)
ENV = 0x704        # bits 0-2 envelope phase, 3 instrument changed, 5 sliding, 6-7 LFO quadrant
LFO = 0x708        # LFO phase
VOL = 0x70C        # duty | $30 | volume (triangle effect: length ratio)
LEVEL = 0x710      # envelope level ($00-$F0); triangle effect: frames left
DETUNE = 0x714     # signed, added to the final period
SLIDE = 0x718      # sign-magnitude slide speed (x2 per frame)
TARGET = 0x71C     # target note index (+1) for slides / portamento
PER_LO = 0x720     # pitch, pseudo-float low byte
PER_HI = 0x724     # pitch, pseudo-float high byte (noise: period index)
# music sequencer arrays (base + x, x = $28 + channel)
PTR_LO = 0x728     # -> $0750+ch
PTR_HI = 0x72C     # -> $0754+ch
FLAGS = 0x730      # -> $0758+ch: bits 0-2 octave, 3 +2 octaves, 4 dotted, 5 triplet, 6 tie, 7 tied
CHTRANS = 0x734    # -> $075C+ch
DUR = 0x738        # -> $0760+ch: ticks left in the current note
GATE_RATIO = 0x73C  # -> $0764+ch
GATE = 0x740       # -> $0768+ch: ticks until release
LOOPS = 0x744      # -> $076C+ch + 4*slot
PHI_CACHE = 0x77C  # + (x & 3)

# jump tables the driver dispatches through (06:8023 leaves the target in $C1/$C2)
T_SPECIAL = 0x81B5
T_FIELDS = 0x8329
T_MUSIC = 0x84A7
T_ENVELOPE = 0x86C7


class DriverError(Exception):
    pass


class Driver:
    def __init__(self, rom, ram=None):
        self.rom = rom
        self.m = ram if ram is not None else bytearray(0x800)
        self.out = []
        cpu = rom.cpu
        self.instr_base = cpu(sr.INSTR_BASE_HI) << 8 | cpu(sr.INSTR_BASE_LO)
        self.cmd_count = cpu(sr.CMD_COUNT)

    # ---- entry points ---------------------------------------------------

    def update(self, commands, y=0):
        """sound_update (0F:FF72): each queued command through sound_play, then sound_tick."""
        out = []
        for c in commands:
            out += self.play(c, y)
        return out + self.tick()

    def play(self, a, y=0):
        """sound_play: JMP $80FE. INC $C0 / JSR $8106 / DEC $C0."""
        m = self.m
        self.out = []
        m[C0] = (m[C0] + 1) & 0xFF
        self.L8106(a & 0xFF, y & 0xFF)
        m[C0] = (m[C0] - 1) & 0xFF
        return self.out

    def tick(self):
        """sound_tick: JMP $806C."""
        self.out = []
        self.L806C()
        return self.out

    # ---- helpers ----------------------------------------------------------

    def apu(self, addr, value):
        self.out.append((addr, value & 0xFF))

    def instr(self, i):
        """LDA ($C5),Y: byte i of the current instrument."""
        return self.rom.cpu(((self.m[C6] << 8 | self.m[C5]) + i) & 0xFFFF)

    def L803A(self, lo, hi):
        """Read a data byte at hi:lo; $C1/$C2 = the pointer, Y = 0."""
        m = self.m
        m[C1] = lo
        if hi >= 0xC0:                      # far pointer (06:8047): bank $18 at $A000
            m[C2] = (hi - 0x20) & 0xFF
            v = self.rom.cpu(m[C2] << 8 | lo, a000_bank=0x18)
            m[C2] = (m[C2] + 0x20) & 0xFF
            return v
        m[C2] = hi
        return self.rom.cpu(hi << 8 | lo)

    def L8006(self):
        """$C1:$C2 = $C1 * $C4."""
        m = self.m
        p = m[C1] * m[C4]
        m[C1] = p >> 8
        m[C2] = p & 0xFF

    def dispatch(self, table, index):
        """06:8023: JMP through an inline table; leaves the target in $C1/$C2."""
        lo = self.rom.cpu(table + 2 * index)
        hi = self.rom.cpu(table + 2 * index + 1)
        self.m[C1], self.m[C2] = lo, hi
        return hi << 8 | lo

    def L8386(self):
        """Next byte of the effect stream ($D0/$D1)."""
        m = self.m
        lo, hi = m[D0], m[D1]
        m[D0] = (lo + 1) & 0xFF
        if m[D0] == 0:
            m[D1] = (m[D1] + 1) & 0xFF
        return self.L803A(lo, hi)

    def L8592(self, x):
        """Next byte of music channel x's stream."""
        m = self.m
        lo, hi = m[PTR_LO + x], m[PTR_HI + x]
        m[PTR_LO + x] = (lo + 1) & 0xFF
        if m[PTR_LO + x] == 0:
            m[PTR_HI + x] = (m[PTR_HI + x] + 1) & 0xFF
        return self.L803A(lo, hi)

    def L80D8(self, x):
        """Silence channel x: volume register $30 ($00 for the triangle)."""
        y = ((x & 3) ^ 3) << 2
        self.apu(0x4000 + y, 0x00 if y == 8 else 0x30)

    def L80EC(self, x, y, a):
        """Write A to register Y (0, 2, 3) of channel x; $C4 = Y."""
        self.m[C4] = y
        self.apu(0x4000 + ((((x & 3) ^ 3) << 2) | y), a)

    # ---- sound_play -------------------------------------------------------

    def L8106(self, a, y):
        if a >= 0xF0:
            return self.L81AE(a, y)
        while a >= self.cmd_count:
            a = (a - self.cmd_count) & 0xFF
        self.L8118(a)

    def L8118(self, a):
        m, rom = self.m, self.rom
        x = (a << 1) & 0xFF
        lo = rom.cpu(0x8A44 + x)
        hi = rom.cpu(0x8A43 + x)
        if (lo | hi) == 0:
            return
        v = self.L803A(lo, hi)
        if v == 0:
            return self.L816F()
        x = (x + 1) & 0xFF
        m[C4] = v
        pri = v & 0x7F
        if pri < m[CE]:
            return
        m[CE] = pri
        if pri == 0 and m[D6] & 0x80 and not m[C4] & 0x80:
            m[D7] = 0
        flag = m[C4] >> 7                     # ASL $C4 / ROR $D6 (Y = 0)
        m[C4] = (m[C4] << 1) & 0xFF
        m[D6] = flag << 7
        if flag:
            m[D7] = x
        m[C1] = (m[C1] + 1) & 0xFF
        m[D0] = m[C1]
        if m[C1] == 0:
            m[C2] = (m[C2] + 1) & 0xFF
        m[D1] = m[C2]
        m[D2] = m[D3] = m[D4] = m[D5] = 0
        for i in range(0x28):
            m[0x700 + i] = 0

    def L816F(self):
        """Music start (first data byte 0): tempo 1+$99/256, music RAM cleared,
        four channel pointers (big-endian, pulse 1 first)."""
        m = self.m
        m[C9] = 1
        m[CA] = 0x99
        m[C8] = m[CB] = m[CC] = m[CD] = 0
        for i in range(0x54):
            m[0x728 + i] = 0
        for x in (3, 2, 1, 0):
            for base in (0x754, 0x750):
                m[C1] = (m[C1] + 1) & 0xFF
                if m[C1] == 0:
                    m[C2] = (m[C2] + 1) & 0xFF
                m[base + x] = self.L803A(m[C1], m[C2])
        self.L81F1()

    def L81AE(self, a, y):
        m = self.m
        m[C3] = y
        target = self.dispatch(T_SPECIAL, a & 7)
        if target == 0x81C5:                  # $F0
            self.L81E4()
            self.L81C8()
        elif target == 0x81C8:                # $F1
            self.L81C8()
        elif target == 0x81E4:                # $F2
            self.L81E4()
        elif target == 0x821E:                # $F3
            m[C0] |= 0x02
            self.L81F1()
        elif target == 0x8226:                # $F4
            m[C0] &= 0xFD
        elif target in (0x822D, 0x8234):      # $F5 / $F6 (fade)
            if target == 0x822D:
                m[C3] = (m[C3] << 1) & 0xFF
                if m[C3]:
                    m[C3] = 0x80 | (m[C3] >> 1)
            m[C0] &= 0x0F
            m[CC] = m[C3]
            if m[C3] == 0 or m[CD] == 0xFF:
                m[CD] = 0
        elif target == 0x824A:                # $F7
            m[D8] = (-m[C3]) & 0xFF
        else:
            raise DriverError(f"special command {a:02X}: unknown target {target:04X}")

    def L81C8(self):
        """Stop the effect."""
        m = self.m
        m[CE] = m[D0] = m[D1] = m[D7] = m[D8] = 0
        self.L81D4()

    def L81D4(self):
        m = self.m
        if m[CF]:
            m[CF] ^= 0x0F
            self.L81F1()
            m[CF] = 0

    def L81E4(self):
        """Stop the music: clear the four stream pointers."""
        m = self.m
        for x in (3, 2, 1, 0):
            m[0x754 + x] = 0
            m[0x750 + x] = 0
        self.L81F1()

    def L81F1(self):
        """Silence every channel whose $CF bit is clear (bit 0 = x 3 ... bit 3 =
        x 0); music channels still playing get their period-high rewritten
        next time. Then sweeps off and channels 0-3 enabled."""
        m = self.m
        saved = m[CF]
        for x in (3, 2, 1, 0):
            c = m[CF] & 1
            m[CF] >>= 1
            if not c:
                self.L80D8(x)
                if m[0x754 + x] | m[0x750 + x]:
                    m[PHI_CACHE + x] = 0xFF
        m[CF] = saved
        self.apu(0x4001, 0x08)
        self.apu(0x4005, 0x08)
        self.apu(0x4015, 0x0F)

    # ---- sound_tick -------------------------------------------------------

    def L806C(self):
        m = self.m
        if m[C0] & 1:
            return
        if m[D0] | m[D1]:
            self.L8252()
        s = m[CA] + m[C8]
        m[C8] = s & 0xFF
        m[C7] = (m[C9] + (s >> 8)) & 0xFF
        saved = m[CF]
        for x in (3, 2, 1, 0):
            c = m[CF] & 1
            m[CF] >>= 1
            if c:
                m[CF] |= 0x80
                self.L82DE(x)
            if not m[C0] & 0x02:
                self.L8393(x)
        m[CF] = saved
        m[C0] &= 0xFE
        a = m[CC] & 0x7F
        if a:                                  # fade (06:80B6), unused by the game
            c1 = 0
            for _ in range(4):
                c1 = ((c1 << 1) | (a >> 7)) & 0xFF
                a = (a << 1) & 0xFF
            m[C1] = c1
            s = a + m[C0]
            m[C0] = s & 0xFF
            s2 = m[C1] + m[CD] + (s >> 8)
            if s2 > 0xFF:
                m[CC] &= 0x80
                m[CD] = 0xFF
            else:
                m[CD] = s2

    # ---- effect sequencer ---------------------------------------------------

    def L8252(self):
        m = self.m
        if m[D3]:
            m[D3] -= 1
            m[D5] = (m[D5] - 1) & 0xFF
            return
        self.L825B()

    def L825B(self):
        m = self.m
        while True:
            a = self.L8386()
            m[C4] = a
            if a & 0x80:                       # end of the effect
                m[CE] = 0
                if m[D7] & 1:                  # it loops: start it again
                    self.L8118(m[D7] >> 1)
                    continue
                return self.L81C8()
            c = m[C4] & 1
            m[C4] >>= 1
            if c:                              # loop / jump
                count2 = (self.L8386() << 1) & 0xFF
                jump = True
                if count2:
                    flag = m[D6] >> 7
                    m[D6] = (m[D6] << 1) & 0xFF
                    if count2 == m[D6]:        # 06:8296: done, skip the address
                        m[D6] = flag << 7
                        s = 2 + m[D0]
                        m[D0] = s & 0xFF
                        if s > 0xFF:
                            m[D1] = (m[D1] + 1) & 0xFF
                        jump = False
                    else:
                        m[D6] = (flag << 7) | (m[D6] >> 1)
                        m[D6] = (m[D6] + 1) & 0xFF
                if jump:
                    hi = self.L8386()
                    lo = self.L8386()
                    m[D0], m[D1] = lo, hi
                    if lo == 0:
                        raise DriverError("effect loop target with low byte 0: the driver falls into 06:8296 "
                                          "and pulls a status byte it never pushed")
                    continue
            c = m[C4] & 1
            m[C4] >>= 1
            if c:
                m[D4] = self.L8386()
            c = m[C4] & 1
            m[C4] >>= 1
            if c:
                m[D2] = self.L8386()
            a = self.L8386()
            m[D3] = a
            m[C1] = a
            m[C4] = m[D4]
            self.L8006()
            m[D5] = (m[C1] + 1) & 0xFF
            m[C0] = (m[C0] + 1) & 0xFF
            a = self.L8386()
            if a ^ m[CF]:
                m[CF] = a ^ m[CF]
                self.L81D4()
            m[CF] = a
            return

    def L82DE(self, x):
        """Effect channel x (0-3), once per frame while the effect owns it."""
        m = self.m
        y = m[INSTR + x]
        if y:
            self.L8684(x, y)
        if not m[C0] & 1:
            self.L86BA(x)
            if m[D3] == 0:
                return
            if x == 1:
                m[LEVEL + x] = (m[LEVEL + x] - 1) & 0xFF
                if m[LEVEL + x]:
                    return
            elif m[D5]:
                return
            if m[ENV + x] & 4:
                return
            return self.L85A3(x)
        # 06:830A: a new row: fields, then the note
        m[C4] = 0
        a = self.L8386()
        while True:
            c = a & 1
            a >>= 1
            if c:
                m[C3] = self.L8386()
                target = self.dispatch(T_FIELDS, m[C4])
                self.field(x, target)
            if a == 0:
                break
            m[C4] = (m[C4] + 1) & 0xFF
        y = self.L8386()
        if y == 0:
            m[LEVEL + x] = 0
            m[ENV + x] = (m[ENV + x] & 0xF8) | 4
            return self.L80D8(x)
        m[ENV + x] |= 0x20
        m[TARGET + x] = 0x54 if m[SLIDE + x] & 0x80 else 0x0A
        if y & 0x80:
            if x == 1:
                self.L85AE(x)
            return self.L8644(x)
        self.L85AE(x)
        m[PHI_CACHE + x] = 0xFF
        y = (y - 1) & 0xFF
        if x == 0:
            m[C3] = 0
            return self.L8636(x, y ^ 0x0F)
        return self.L85DE(x, (y + m[D2]) & 0xFF)

    def field(self, x, target):
        m = self.m
        if target == 0x866F:
            self.L866F(x)
        elif target == 0x86AD:
            self.L86AD(x)
        elif target == 0x865A:
            self.L865A(x)
        elif target == 0x86A7:
            m[SLIDE + x] = m[C3]
        elif target == 0x86A1:
            m[DETUNE + x] = m[C3]
        else:
            raise DriverError(f"effect field dispatch to {target:04X}")

    # ---- music channels -------------------------------------------------------

    def L8393(self, ch):
        m = self.m
        x = ch | 0x28
        if (m[PTR_LO + x] | m[PTR_HI + x]) == 0:
            return
        if m[DUR + x]:
            y = m[INSTR + x]
            if y:
                self.L8684(x, y)
                self.L86BA(x)
            s = m[GATE + x] - m[C7]
            m[GATE + x] = s & 0xFF
            if s <= 0:
                self.L85A3(x)
            s = m[DUR + x] - m[C7]
            m[DUR + x] = s & 0xFF
            if s > 0:
                return
        while True:                            # 06:83CD
            a = self.L8592(x)
            if a >= 0x20:
                break
            if self.L8497(x, a):
                return                         # $17 ended the channel
        self.note(x, a)

    def note(self, x, a):
        """06:83DA: a note or rest byte (bits 5-7 length code, bits 0-4 pitch)."""
        m, rom = self.m, self.rom
        y = (a >> 5) - 1
        f = m[FLAGS + x]
        if f & 0x20:
            dur = rom.cpu(sr.LEN_TRIPLET + y)
        else:
            dur = rom.cpu(sr.LEN_NORMAL + y)
            if f & 0x10:
                m[C3] = dur
                m[FLAGS + x] = f & 0xEF
                dur = ((dur >> 1) + m[C3]) & 0xFF
        total = (dur + m[DUR + x]) & 0xFF
        m[DUR + x] = total
        pitch = a & 0x1F
        if pitch == 0:                          # rest
            self.L85A3(x)
            m[GATE + x] = 0xFF
            return
        m[C4] = total
        m[C1] = m[GATE_RATIO + x]
        self.L8006()
        m[GATE + x] = m[C1] or 1
        y = pitch - 1
        if m[FLAGS + x] & 0x80:                 # tied to the previous note
            if m[SLIDE + x] == 0:
                self.L8644(x)
                return self.L847E(x)
        else:
            self.L85AE(x)
            if not m[CF] & 0x80:
                m[C3] = y
                m[PHI_CACHE + (x & 3)] = 0xFF
        if (x & 3) == 0:                        # noise: pitch -> period index
            m[C3] = 0
            self.L8636(x, (y & 0x0F) ^ 0x0F)
        else:
            m[C3] = y
            a = rom.cpu(sr.OCTAVE_BASE + (m[FLAGS + x] & 0x0F))
            a = (a + m[C3]) & 0xFF
            a = (a + m[CB]) & 0xFF
            a = (a + m[CHTRANS + x]) & 0xFF
            self.L85DE(x, a)
        self.L847E(x)

    def L847E(self, x):
        m = self.m
        f = m[FLAGS + x]
        m[C4] = (f & 0x40) << 1
        m[FLAGS + x] = (f & 0x7F) | m[C4]
        if m[FLAGS + x] & 0x80:
            m[GATE + x] = 0xFF

    def L8497(self, x, a):
        """Music command a (< $20). Returns True when the channel ended ($17)."""
        m = self.m
        if a >= 4:
            m[C4] = a
            m[C3] = self.L8592(x)
            a = m[C4]
        target = self.dispatch(T_MUSIC, a)
        if target == 0x84D9:
            m[FLAGS + x] ^= 0x20
        elif target == 0x84DD:
            m[FLAGS + x] ^= 0x40
        elif target == 0x84E1:
            m[FLAGS + x] |= 0x10
        elif target == 0x84E8:
            m[FLAGS + x] ^= 0x08
        elif target == 0x8575:
            self.L8575(x)
        elif target == 0x84F1:                  # tempo
            m[C8] = 0
            lo = self.L8592(x)
            m[CA] = lo
            m[C9] = m[C3]
        elif target == 0x84FF:
            m[GATE_RATIO + x] = m[C3]
        elif target == 0x865A:
            self.L865A(x)
        elif target == 0x866F:
            self.L866F(x)
        elif target == 0x8505:
            m[FLAGS + x] = (m[FLAGS + x] & 0xF8) | m[C3]
        elif target == 0x8510:
            m[CB] = m[C3]
        elif target == 0x8515:
            m[CHTRANS + x] = m[C3]
        elif target == 0x86A1:
            m[DETUNE + x] = m[C3]
        elif target == 0x86A7:
            m[SLIDE + x] = m[C3]
        elif target in (0x851B, 0x851F, 0x8523, 0x8527):
            self.loop(x, (target - 0x851B) // 4)
        elif target == 0x855A:                  # jump
            self.L855A(x)
        elif target == 0x8580:                  # end of the channel
            m[PTR_LO + x] = 0
            m[PTR_HI + x] = 0
            if not m[CF] & 0x80:
                self.L80D8(x)
            return True
        elif target == 0x86AD:
            self.L86AD(x)
        else:
            raise DriverError(f"music command {a:02X}: dispatch to {target:04X}")
        return False

    def L8575(self, x):
        m = self.m
        m[FLAGS + x] = (m[FLAGS + x] & 0x97) | m[C3]

    def loop(self, x, slot):
        """06:8529: $0E-$11 loop (count in $C3), $12-$15 break on the last pass."""
        m = self.m
        m[C2] = slot * 4
        y = (x + m[C2]) & 0xFF
        if m[C4] < 0x12:
            a = m[LOOPS + y] - 1
            if a < 0:
                a = m[C3]
            m[LOOPS + y] = a & 0xFF
            if m[LOOPS + y] == 0:
                return self.L8566(x)
        else:
            if (m[LOOPS + y] - 1) & 0xFF:
                return self.L8566(x)
            m[LOOPS + y] = 0
            self.L8575(x)
        m[C3] = self.L8592(x)                   # 06:8555
        self.L855A(x)

    def L855A(self, x):
        m = self.m
        lo = self.L8592(x)
        m[PTR_LO + x] = lo
        m[PTR_HI + x] = m[C3]

    def L8566(self, x):
        m = self.m
        s = 2 + m[PTR_LO + x]
        m[PTR_LO + x] = s & 0xFF
        if s > 0xFF:
            m[PTR_HI + x] = (m[PTR_HI + x] + 1) & 0xFF

    # ---- shared channel routines ------------------------------------------------

    def L85A3(self, x):
        """Key off: envelope to release (phase 3)."""
        self.m[ENV + x] = (self.m[ENV + x] & 0xF8) | 3

    def L85AE(self, x):
        """Key on: envelope phase 0 at level 0; the triangle starts in sustain
        (music: level 1; effect: level = ($D3 * volume >> 8) + 1 frames)."""
        m = self.m
        y = 0
        m[ENV + x] &= 0xF8
        if x == 0x29 or x == 1:
            if x == 1:
                m[C1] = m[D3]
                m[C4] = m[VOL + x]
                self.L8006()
                y = m[C1]
            y = (y + 1) & 0xFF
            m[ENV + x] = (m[ENV + x] + 2) & 0xFF
        m[LEVEL + x] = y

    def L85DE(self, x, a):
        """Note a -> pitch; for music, portamento toward a new note when $0718 is set."""
        m, rom = self.m, self.rom
        if a >= 0x60:
            a = 0x5F
        m[C3] = (a + 1) & 0xFF
        if x >= 0x28:
            t = m[TARGET + x]
            plain = True
            if t:
                if t == m[C3]:
                    if m[FLAGS + x] & 0x80:
                        return self.L8644(x)
                elif m[SLIDE + x]:
                    if t >= m[C3]:
                        m[SLIDE + x] &= 0x7F
                    else:
                        m[SLIDE + x] |= 0x80
                    m[ENV + x] |= 0x20
                    new = m[C3]
                    m[C3] = t
                    m[TARGET + x] = new
                    plain = False
            if plain:
                m[ENV + x] &= 0xDF
                m[TARGET + x] = m[C3]
        idx = (m[C3] << 1) & 0xFF
        m[C3] = rom.cpu(sr.PERIOD_TABLE + idx)
        self.L8636(x, rom.cpu(sr.PERIOD_TABLE + 1 + idx))

    def L8636(self, x, a):
        m = self.m
        m[PER_HI + x] = a
        m[PER_LO + x] = m[C3]
        if self.instr(4) & 0x80:
            return self.L864C(x)
        self.L8644(x)

    def L8644(self, x):
        if self.m[ENV + x] & 0x08:
            self.L864C(x)

    def L864C(self, x):
        m = self.m
        m[LFO + x] = 0
        m[ENV + x] &= 0x37

    def L865A(self, x):
        m = self.m
        if x == 1 and m[C3]:
            m[VOL + x] = m[C3]
        else:
            m[VOL + x] = (m[VOL + x] & 0xC0) | m[C3] | 0x30

    def L866F(self, x):
        m = self.m
        m[C3] = (m[C3] + 1) & 0xFF
        a = m[C3]
        if a == m[INSTR + x]:
            return
        m[INSTR + x] = a
        m[ENV + x] |= 0x08
        self.L8684(x, a)

    def L8684(self, x, y):
        """$C5/$C6 = instrument table + 8 * (y - 1)."""
        m = self.m
        y = (y - 1) & 0xFF
        m[C3] = y >> 5
        lo = ((y << 3) & 0xFF) + (self.instr_base & 0xFF)
        m[C5] = lo & 0xFF
        m[C6] = (m[C3] + (self.instr_base >> 8) + (lo >> 8)) & 0xFF

    def L86AD(self, x):
        m = self.m
        m[VOL + x] = (m[VOL + x] & 0x0F) | m[C3] | 0x30

    # ---- envelope and output -----------------------------------------------------

    def L86BA(self, x):
        m, rom = self.m, self.rom
        m[C4] = m[LEVEL + x]
        target = self.dispatch(T_ENVELOPE, m[ENV + x] & 7)
        if target == 0x8914:                    # phase 4: off, nothing written
            return
        if target == 0x86D1:                    # attack
            a = m[C4] + rom.cpu(sr.RATE_TABLE + self.instr(0))
            if a > 0xFF or a >= 0xF0:
                a = 0xF0
                m[ENV + x] = (m[ENV + x] + 1) & 0xFF
            m[LEVEL + x] = a
        elif target == 0x86E6:                  # decay
            y = self.instr(1)
            a = None
            if y:
                a = m[C4] - rom.cpu(sr.RATE_TABLE + y)
                if a < 0 or a < self.instr(2):
                    a = None
            if a is None:
                a = self.instr(2)
                m[ENV + x] = (m[ENV + x] + 1) & 0xFF
            m[LEVEL + x] = a
        elif target == 0x8702:                  # release
            if (x & 3) == 1:
                m[ENV + x] = (m[ENV + x] + 1) & 0xFF
                m[LEVEL + x] = 0
            else:
                y = self.instr(3)
                if y:
                    a = m[C4] - rom.cpu(sr.RATE_TABLE + y)
                    if a < 0:
                        a = 0
                        m[ENV + x] = (m[ENV + x] + 1) & 0xFF
                    m[LEVEL + x] = a
        elif target != 0x8720:                  # 0x8720 = sustain
            raise DriverError(f"envelope phase {m[ENV + x] & 7} on channel {x:02X}")
        self.L8720(x)

    def L8720(self, x):
        m = self.m
        if x >= 0x28:
            if m[CF] & 0x80:                    # the effect owns this channel
                return self.L88A0(x)
            a = m[CD]
            if not m[CC] & 0x80:
                a ^= 0xFF
            if a != 0xFF:
                return self.L8740(x, a)
        if (x & 3) == 1:
            return self.L87AA(x, 0xFF if m[LEVEL + x] else 0)
        self.L8763(x, m[LEVEL + x])

    def L8740(self, x, a):
        """Fade (unused by the game)."""
        m = self.m
        if x == 0x29:
            m[C4] = a
            m[C1] = m[GATE + x]
            self.L8006()
            if m[C1] == 0:
                return self.L87AA(x, 0)
            return self.L87AA(x, 0xFF if m[LEVEL + x] else 0)
        if a >= m[LEVEL + x]:
            a = m[LEVEL + x]
        self.L8763(x, a)

    def L8763(self, x, a):
        """Pulse/noise volume: attenuation from the envelope level, deepened by
        the tremolo, subtracted from the channel volume."""
        m = self.m
        m[C3] = (a >> 4) ^ 0x0F
        depth = self.instr(6)
        if depth >= 5:
            m[C4] = depth
            ph = m[LFO + x]
            if m[ENV + x] & 0x40:
                ph ^= 0xFF
            if ph:
                m[C1] = ph
                self.L8006()
                t = m[C1] >> 2
                if t >= 0x10:
                    return self.L87AA(x, m[VOL + x] & 0xF0)
                if t >= m[C3]:
                    m[C3] = t
        m[C4] = 0x10
        a = (m[VOL + x] - m[C3]) & 0xFF
        if a & 0x10:
            return self.L87AA(x, a)
        self.L87AA(x, m[VOL + x] & 0xF0)

    def L87AA(self, x, a):
        """Write the volume register, then the pitch (vibrato applied)."""
        m = self.m
        self.L80EC(x, 0, a)
        base = True
        if not m[PHI_CACHE + (x & 3)] & 0x80:
            depth = self.instr(5)
            if depth:
                m[C4] = depth
                ph = m[LFO + x]
                if m[ENV + x] & 0x40:
                    ph ^= 0xFF
                if ph:
                    m[C1] = ph
                    self.L8006()
                    v = (m[C1] << 8 | m[C2]) >> 4
                    yv = v >> 8
                    m[C2] = v & 0xFF
                    if yv | m[C2]:
                        h = None
                        if not m[ENV + x] & 0x80:
                            s = m[C2] + m[PER_LO + x]
                            m[C2] = s & 0xFF
                            h = (yv + m[PER_HI + x] + (s >> 8)) & 0xFF
                            if h == 0:
                                h = None        # BNE not taken: falls into 06:87FA
                        if h is None:
                            s = m[PER_LO + x] - m[C2]
                            m[C2] = s & 0xFF
                            m[C1] = yv
                            h = (m[PER_HI + x] - m[C1] - (1 if s < 0 else 0)) & 0xFF
                        if h:
                            base = False
                            y = h
        if base:
            m[C2] = m[PER_LO + x]
            y = m[PER_HI + x]
        self.L8814(x, y)

    def L8814(self, x, y):
        m, rom = self.m, self.rom
        if x < 0x28 and m[D6] & 0x80 and m[D8]:     # effect pitch scale ($F7), unused
            m[C4] = m[D8]
            m[C1] = y
            saved = m[C2]
            self.L8006()
            s = saved + m[C2]
            m[C2] = s & 0xFF
            y = (m[C1] + (s >> 8)) & 0xFF
        if (x & 3) == 0:                           # noise
            m[C2] = (y & 0x0F) | self.instr(7)
            m[C1] = 0
        else:                                      # 06:8849: pseudo-float -> period
            a = y
            yy = 8
            while True:
                yy -= 1
                if a >= rom.cpu(sr.OCT_THRESHOLDS + yy):
                    break
            v = (yy + a) & 0xFF
            m[C1] = ((v & 7) + 7) & 0xFF
            sh = (v & 0x38) ^ 0x38
            while sh:
                low = m[C1] & 1
                m[C1] >>= 1
                m[C2] = (m[C2] >> 1) | (low << 7)
                sh -= 8
            d = m[DETUNE + x]
            if d:
                s = d + m[C2]
                m[C2] = s & 0xFF
                m[C1] = ((0xFF if d & 0x80 else 0) + m[C1] + (s >> 8)) & 0xFF
        self.L80EC(x, 2, m[C2])
        if m[C1] != m[PHI_CACHE + (x & 3)]:
            m[PHI_CACHE + (x & 3)] = m[C1]
            self.L80EC(x, 3, m[C1] | 0x08)
        self.L88A0(x)

    def L88A0(self, x):
        """Slide toward $071C, then advance the LFO."""
        m, rom = self.m, self.rom
        if m[ENV + x] & 0x20:
            sp = m[SLIDE + x]
            clear = sp == 0
            if sp:
                neg = sp >> 7
                a = (sp << 1) & 0xFF
                hi = 0
                if neg:
                    a = ((a ^ 0xFF) + 1) & 0xFF
                    hi = 0xFF
                s = a + m[PER_LO + x]
                m[PER_LO + x] = s & 0xFF
                m[PER_HI + x] = (hi + m[PER_HI + x] + (s >> 8)) & 0xFF
                idx = (m[TARGET + x] << 1) & 0xFF
                tlo = rom.cpu(sr.PERIOD_TABLE + idx)
                thi = rom.cpu(sr.PERIOD_TABLE + 1 + idx)
                c = 1 if m[PER_LO + x] >= tlo else 0
                c = 1 if (m[PER_HI + x] & 0x3F) - thi - (1 - c) >= 0 else 0
                r = ((0xFF + c) & 0xFF) + neg
                if (r & 0xFF) == 0 and x != 0:
                    m[PER_LO + x] = tlo
                    m[PER_HI + x] = thi
                    clear = True
            if clear:
                m[ENV + x] &= 0xDF
        sp = self.instr(4) & 0x7F
        if sp:
            s = sp + m[LFO + x]
            m[LFO + x] = s & 0xFF
            if s > 0xFF:
                m[ENV + x] = (m[ENV + x] + 0x40) & 0xFF


# ---- RAM ranges the driver owns ------------------------------------------------

SCRATCH = (C1, C2, C3, C4)
STATE = [a for a in range(0xC0, 0xD9) if a not in SCRATCH] + list(range(0x700, 0x780))
ALL = list(range(0xC0, 0xD9)) + list(range(0x700, 0x780))
