"""Mighty Final Fight's sound data, read from the user's dump (banks $06/$07).

Shared by port/tools/sound_dump.py, sound_model.py and apu_render.py. Every
address below is a CPU address with bank $06 at $8000 and $07 at $A000 (what
sound_update, 0F:FF72, maps before calling the driver). docs/sound.md
explains the formats; the comments here only say where each table is.
"""
import zlib

ROM_CRC = 0x15EB0BEE                 # with the 16-byte iNES header (CLAUDE.md)

# Tables in bank $06 (addresses of the code that reads them in docs/sound.md)
CMD_COUNT = 0x8A40                   # number of entries in the command table ($36)
INSTR_BASE_HI = 0x8A41               # instrument table pointer, hi byte ...
INSTR_BASE_LO = 0x8A42               # ... and lo byte (8 bytes per instrument)
CMD_TABLE = 0x8A43                   # big-endian pointers, 2 bytes per command
PERIOD_TABLE = 0x8959                # (lo, hi) pairs indexed by note + 1
OCT_THRESHOLDS = 0x8953              # 8 bytes: 0,7,...,49 (the last two are PERIOD_TABLE[0])
RATE_TABLE = 0x8933                  # 32 envelope step sizes
OCTAVE_BASE = 0x8923                 # 16 bytes: octave * 12 (+24 with the $08 flag)
LEN_TRIPLET = 0x8915                 # 7 durations for length codes 1-7, triplet
LEN_NORMAL = 0x891C                  # 7 durations, normal

NOTE_NAMES = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"]
CPU_HZ = 1789773.0                   # NTSC 2A03

CHANNELS = {3: "sq1", 2: "sq2", 1: "tri", 0: "noise"}   # driver index -> name
REG_BASE = {3: 0x4000, 2: 0x4004, 1: 0x4008, 0: 0x400C}

# music stream commands (bytes $00-$1F; jump table at 06:84A7)
MUSIC_CMDS = {
    0x00: ("triplet", 0), 0x01: ("tie", 0), 0x02: ("dot", 0), 0x03: ("oct+2", 0),
    0x04: ("flags", 1), 0x05: ("tempo", 2), 0x06: ("gate", 1), 0x07: ("volume", 1),
    0x08: ("instrument", 1), 0x09: ("octave", 1), 0x0A: ("transpose", 1),
    0x0B: ("ch_transpose", 1), 0x0C: ("detune", 1), 0x0D: ("portamento", 1),
    0x0E: ("loop0", 3), 0x0F: ("loop1", 3), 0x10: ("loop2", 3), 0x11: ("loop3", 3),
    0x12: ("break0", 3), 0x13: ("break1", 3), 0x14: ("break2", 3), 0x15: ("break3", 3),
    0x16: ("jump", 2), 0x17: ("end", 0), 0x18: ("duty", 1),
}

SFX_FIELDS = ["instrument", "duty", "volume", "slide", "detune"]    # 06:8329


class Rom:
    def __init__(self, path):
        data = open(path, "rb").read()
        if data[:4] != b"NES\x1a":
            raise SystemExit(f"{path}: not an iNES file")
        crc = zlib.crc32(data) & 0xFFFFFFFF
        if crc != ROM_CRC:
            raise SystemExit(f"{path}: CRC32 {crc:08X}, expected {ROM_CRC:08X} (Mighty Final Fight (USA))")
        prg_banks = data[4] * 2                       # 16 KiB units -> 8 KiB banks
        self.prg = data[16:16 + prg_banks * 0x2000]
        self.banks = [self.prg[i * 0x2000:(i + 1) * 0x2000] for i in range(prg_banks)]

    # CPU view while the driver runs: $06 at $8000, $07 at $A000, $0E/$0F fixed
    def cpu(self, addr, a000_bank=7):
        addr &= 0xFFFF
        if 0x8000 <= addr < 0xA000:
            return self.banks[6][addr - 0x8000]
        if 0xA000 <= addr < 0xC000:
            return self.banks[a000_bank % len(self.banks)][addr - 0xA000]
        if 0xC000 <= addr < 0xE000:
            return self.banks[0x0E][addr - 0xC000]
        if addr >= 0xE000:
            return self.banks[0x0F][addr - 0xE000]
        raise ValueError(f"not ROM: ${addr:04X}")

    def data(self, addr):
        """A byte of sound data as 06:803A reads it: pointers below $C000 read
        banks 06/07; $C000-$FFFF is a 'far' pointer that the driver reads at
        addr - $2000 with bank $18 (= $08 on this 128 KiB cart) at $A000.
        No sound in this ROM uses a far pointer."""
        if (addr >> 8) >= 0xC0:
            return self.cpu(addr - 0x2000, a000_bank=0x18)
        return self.cpu(addr)

    def word_be(self, addr):
        return self.data(addr) << 8 | self.data(addr + 1)

    # ---- tables ----
    def command_count(self):
        return self.cpu(CMD_COUNT)

    def command_ptr(self, cmd):
        return self.cpu(CMD_TABLE + 2 * cmd) << 8 | self.cpu(CMD_TABLE + 2 * cmd + 1)

    def instr_base(self):
        return self.cpu(INSTR_BASE_HI) << 8 | self.cpu(INSTR_BASE_LO)

    def instrument(self, n):
        """Instrument n (1-based, as the streams number them): 8 bytes."""
        base = self.instr_base() + 8 * ((n - 1) & 0xFF)
        return [self.cpu(base + i) for i in range(8)]

    def period_entry(self, index):
        """(lo, hi) of PERIOD_TABLE[index], index = note + 1 (0..96)."""
        return self.cpu(PERIOD_TABLE + 2 * index), self.cpu(PERIOD_TABLE + 2 * index + 1)

    def rate(self, i):
        return self.cpu(RATE_TABLE + i)


def float_period(hi, lo):
    """The driver keeps pitch as a pseudo-float (hi = 7 * octave + r, lo):
    period = ((r + 7) * 256 + lo) >> (7 - octave). Returns the 16-bit value
    before the hardware's 11-bit truncation (06:8849)."""
    y = 7
    while y > 0 and hi < 7 * y:
        y -= 1
    v = hi + y
    mant = ((v & 7) + 7) << 8 | lo
    return mant >> (7 - ((v >> 3) & 7))


def note_name(note):
    """Driver note number (0 = C of octave 1 in the period table) -> 'C1'..."""
    return f"{NOTE_NAMES[note % 12]}{note // 12 + 1}"


def period_hz(period, triangle=False):
    return CPU_HZ / ((32 if triangle else 16) * (period + 1))


# ---- stream decoding ---------------------------------------------------------

LEN_NORMAL_TICKS = [3, 6, 12, 24, 48, 96, 192]       # 06:891C (length codes 1-7)
LEN_TRIPLET_TICKS = [2, 4, 8, 16, 32, 64, 128]      # 06:8915


def signed(v):
    return v - 256 if v >= 128 else v


def decode_music_channel(rom, start, channel=3, transpose=0):
    """Reachable events of one music channel stream, in address order:
    [(addr, length, text, kind, target)], following loops, breaks and jumps.
    Note names come from the octave, flags and transposes in effect the
    first time the walk reaches each byte (the driver's own state can
    differ on later passes)."""
    seen = {}
    work = [(start, (0, transpose, 0))]    # (pc, (flags $0730, global transpose, channel transpose))
    while work:
        pc, (flags, gtr, ctr) = work.pop()
        while pc not in seen:
            b = rom.data(pc)
            if b < 0x20:
                name, n = MUSIC_CMDS.get(b, (f"cmd{b:02X}?", 0))
                args = [rom.data(pc + 1 + i) for i in range(n)]
                target = None
                state_at_target = None
                if b == 0x00:
                    flags ^= 0x20
                    text = f"triplet {'on' if flags & 0x20 else 'off'}"
                elif b == 0x01:
                    flags ^= 0x40
                    text = f"tie {'on' if flags & 0x40 else 'off'}"
                elif b == 0x02:
                    flags |= 0x10
                    text = "dot (next note x1.5)"
                elif b == 0x03:
                    flags ^= 0x08
                    text = f"octave +2 {'on' if flags & 0x08 else 'off'}"
                elif b == 0x04:
                    flags = (flags & 0x97) | args[0]
                    text = f"flags ${args[0]:02X} (octave+2 {'on' if args[0] & 8 else 'off'}, triplet " \
                           f"{'on' if args[0] & 0x20 else 'off'}, tie {'on' if args[0] & 0x40 else 'off'})"
                elif b == 0x05:
                    tempo = args[0] + args[1] / 256
                    text = f"tempo {args[0]}+${args[1]:02X}/256 ticks/frame (quarter = {75 * tempo:.1f} BPM)"
                elif b == 0x06:
                    text = f"gate {args[0]}/256 of each note"
                elif b == 0x07:
                    text = f"volume {args[0] & 15}"
                elif b == 0x08:
                    text = f"instrument {(args[0] + 1) & 0xFF}"
                elif b == 0x09:
                    flags = (flags & 0xF8) | args[0]
                    text = f"octave {args[0]}"
                elif b == 0x0A:
                    gtr = args[0]
                    text = f"transpose {signed(args[0]):+d} (all channels)"
                elif b == 0x0B:
                    ctr = args[0]
                    text = f"channel transpose {signed(args[0]):+d}"
                elif b == 0x0C:
                    text = f"detune {signed(args[0]):+d} (period units)"
                elif b == 0x0D:
                    text = f"portamento speed {args[0] & 0x7F}" if args[0] else "portamento off"
                elif 0x0E <= b <= 0x11:
                    target = args[1] << 8 | args[2]
                    text = f"loop{b - 0x0E}: play {args[0] + 1} times from ${target:04X}"
                elif 0x12 <= b <= 0x15:
                    target = args[1] << 8 | args[2]
                    state_at_target = ((flags & 0x97) | args[0], gtr, ctr)
                    text = f"break{b - 0x12}: on loop{b - 0x12}'s last pass, flags ${args[0]:02X} and go to ${target:04X}"
                elif b == 0x16:
                    target = args[0] << 8 | args[1]
                    text = f"jump ${target:04X}"
                elif b == 0x17:
                    text = "end of channel"
                elif b == 0x18:
                    text = f"duty {args[0] >> 6} (${args[0]:02X})"
                else:
                    text = name
                seen[pc] = (pc, 1 + n, text, "cmd", target)
                if target is not None:
                    work.append((target, state_at_target or (flags, gtr, ctr)))
                if b in (0x16, 0x17) or b not in MUSIC_CMDS:
                    break
                pc += 1 + n
            else:
                code, pitch = b >> 5, b & 0x1F
                ticks = (LEN_TRIPLET_TICKS if flags & 0x20 else LEN_NORMAL_TICKS)[code - 1]
                if flags & 0x10 and not flags & 0x20:
                    ticks = ticks + ticks // 2
                    flags &= 0xEF
                if pitch == 0:
                    text = f"rest      {ticks:3d} ticks"
                elif channel == 0:
                    text = f"noise {((pitch - 1) & 15) ^ 15:2d}  {ticks:3d} ticks"
                else:
                    note = rom.cpu(OCTAVE_BASE + (flags & 0x0F)) + pitch - 1 + gtr + ctr
                    note &= 0xFF
                    text = f"{note_name(min(note, 0x5F)):4s}      {ticks:3d} ticks"
                if flags & 0x40:
                    text += "  (tie on)"
                seen[pc] = (pc, 1, text, "note", None)
                pc += 1
    return [seen[k] for k in sorted(seen)]


def initial_transpose(rom, channels):
    """The global transpose ($0A, in $CB) a song sets before its first notes.
    $CB is shared: the channel that sets it on the first tick (pulse 1 is
    processed first) transposes the others too. Returns 0 if none."""
    value = 0
    for ch in (3, 2, 1, 0):
        pc = channels.get(ch, 0)
        while pc:
            b = rom.data(pc)
            if b >= 0x20 or b in (0x16, 0x17):
                break
            n = MUSIC_CMDS.get(b, ("", 0))[1]
            if b == 0x0A:
                value = rom.data(pc + 1)
            pc += 1 + n
    return value


def decode_sfx(rom, start, max_rows=4096):
    """Rows of a sound effect stream (start = the byte after the priority):
    [(addr, text)], stopping at the end marker. Loops are listed, not
    followed (the stream is linear between them)."""
    out = []
    pc = start
    mask = 0
    for _ in range(max_rows):
        head = rom.data(pc)
        row_at = pc
        pc += 1
        if head & 0x80:
            out.append((row_at, f"end (${head:02X})"))
            return out
        parts = []
        if head & 1:
            count = rom.data(pc)
            target = rom.data(pc + 1) << 8 | rom.data(pc + 2)
            pc += 3
            parts.append(f"loop x{count & 0x7F} -> ${target:04X}" if count & 0x7F else f"jump -> ${target:04X}")
            if (target & 0xFF) == 0:
                parts.append("[WARNING: target lo byte 0 trips the driver's flag test at 06:8293]")
            if (count & 0x7F) == 0:
                out.append((row_at, " ".join(parts)))
                return out
        if head & 2:
            parts.append(f"gate ${rom.data(pc):02X}")
            pc += 1
        if head & 4:
            parts.append(f"transpose {rom.data(pc)}")
            pc += 1
        frames = rom.data(pc)
        mask = rom.data(pc + 1)
        pc += 2
        chans = [CHANNELS[3 - b] for b in range(4) if mask >> b & 1]
        parts.append(f"{frames + 1} frames, channels {'+'.join(chans) or '-'}")
        out.append((row_at, " ".join(parts)))
        for b in range(4):
            if not mask >> b & 1:
                continue
            ch_at = pc
            fields = rom.data(pc)
            pc += 1
            items = []
            ch = 3 - b
            for i in range(8):
                if fields >> i & 1:
                    v = rom.data(pc)
                    pc += 1
                    if i == 0:
                        items.append(f"instrument {(v + 1) & 0xFF}")
                    elif i == 1:
                        items.append(f"duty {v >> 6}")
                    elif i == 2:
                        items.append(f"length {v}/256 of the row" if ch == 1 and v else f"volume {v & 15}")
                    elif i == 3:
                        items.append(f"slide {'-' if v & 0x80 else '+'}{v & 0x7F}" if v else "slide off")
                    elif i == 4:
                        items.append(f"detune {signed(v):+d}")
                    else:
                        items.append(f"field{i}?=${v:02X}")
            note = rom.data(pc)
            pc += 1
            if note == 0:
                items.append("note off")
            elif note & 0x80:
                items.append(f"hold the note (${note:02X})")
            elif ch == 0:
                items.append(f"noise {(note - 1) & 15 ^ 15}")
            else:
                items.append(f"note {note_name(min(note - 1, 0x5F))}")
            out.append((ch_at, f"    {CHANNELS[3 - b]:5s} " + " ".join(items)))
    out.append((pc, "... (stopped)"))
    return out


def command_info(rom, cmd):
    """(kind, pointer, details) for a command number below CMD_COUNT."""
    ptr = rom.command_ptr(cmd)
    if ptr == 0:
        return "none", 0, {}
    first = rom.data(ptr)
    if first == 0:
        chans = {}
        for i, ch in enumerate((3, 2, 1, 0)):
            chans[ch] = rom.word_be(ptr + 1 + 2 * i)
        return "music", ptr, {"channels": chans}
    return "sfx", ptr, {"priority": first & 0x7F, "loops": bool(first & 0x80)}
