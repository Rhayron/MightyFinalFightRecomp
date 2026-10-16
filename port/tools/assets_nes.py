"""ROM access, CHR tiles and the NES colour palette for the asset extractor.

Everything here reads the user's dump at run time; nothing ROM-derived is
stored in this file.
"""
import math
import zlib

ROM_CRC32 = 0x15EB0BEE          # whole file, iNES header included (CLAUDE.md)
PRG_SIZE = 0x20000              # 16 x 8 KiB
CHR_SIZE = 0x20000              # 128 x 1 KiB


def bank_window(bank):
    """CPU address where the game maps an 8 KiB PRG bank (MMC3 mode 0).

    Even banks at $8000, odd at $A000, $0E fixed at $C000, $0F fixed at
    $E000 (data bank $02 too: object_draw maps it at $8000 with $03 at
    $A000; nothing maps it at $A000)."""
    if bank == 0x0E:
        return 0xC000
    if bank == 0x0F:
        return 0xE000
    return 0x8000 if bank % 2 == 0 else 0xA000


class Rom:
    def __init__(self, path, check_crc=True):
        data = open(path, "rb").read()
        self.path = path
        self.crc32 = zlib.crc32(data) & 0xFFFFFFFF
        if data[:4] != b"NES\x1a":
            raise ValueError(f"{path}: no iNES header")
        prg_16k, chr_8k = data[4], data[5]
        mapper = (data[6] >> 4) | (data[7] & 0xF0)
        if mapper != 4 or prg_16k * 0x4000 != PRG_SIZE or chr_8k * 0x2000 != CHR_SIZE:
            raise ValueError(f"{path}: expected mapper 4, 128 KiB PRG, 128 KiB CHR "
                             f"(got mapper {mapper}, PRG {prg_16k * 16} KiB, CHR {chr_8k * 8} KiB)")
        if check_crc and self.crc32 != ROM_CRC32:
            raise ValueError(f"{path}: CRC32 {self.crc32:08X}, expected {ROM_CRC32:08X} "
                             "(Mighty Final Fight (USA)); --no-crc skips this check")
        skip = 16 + (512 if data[6] & 4 else 0)
        self.prg = data[skip:skip + PRG_SIZE]
        self.chr = data[skip + PRG_SIZE:skip + PRG_SIZE + CHR_SIZE]
        self._tiles = {}

    # ---- PRG -------------------------------------------------------------
    def offset(self, bank, addr):
        """PRG offset of CPU address `addr` with 8 KiB bank `bank` mapped there."""
        if not 0x8000 <= addr <= 0xFFFF:
            raise ValueError(f"${addr:04X} is not a PRG address")
        if (addr & 0xE000) != bank_window(bank):
            raise ValueError(f"bank {bank:02X} is mapped at ${bank_window(bank):04X}, not at ${addr:04X}")
        return bank * 0x2000 + (addr & 0x1FFF)

    def byte(self, bank, addr):
        return self.prg[self.offset(bank, addr)]

    def bytes(self, bank, addr, n):
        off = self.offset(bank, addr)
        if (addr & 0x1FFF) + n > 0x2000:
            raise ValueError(f"{n} bytes at {bank:02X}:{addr:04X} cross the bank end")
        return self.prg[off:off + n]

    def word(self, bank, lo_addr, hi_addr):
        """A pointer split in two tables (lo at lo_addr, hi at hi_addr)."""
        return self.byte(bank, lo_addr) | self.byte(bank, hi_addr) << 8

    # ---- CHR -------------------------------------------------------------
    def tile(self, n):
        """CHR tile n (0..8191) as 8 rows of 8 pixel values 0..3 (bytes)."""
        t = self._tiles.get(n)
        if t is None:
            raw = self.chr[16 * n:16 * n + 16]
            t = []
            for y in range(8):
                lo, hi = raw[y], raw[y + 8]
                t.append(bytes(((lo >> (7 - x)) & 1) | (((hi >> (7 - x)) & 1) << 1) for x in range(8)))
            t = tuple(t)
            self._tiles[n] = t
        return t


def bg_tile(r0, r1, t):
    """CHR tile number of background tile t (pattern table $0000) with MMC3 R0/R1.

    R0/R1 select 2 KiB at $0000/$0800 in 1 KiB units with bit 0 ignored
    (chr_banks $E8/$E9; nmi_busy writes them every frame, $C0BA)."""
    if t < 0x80:
        return (r0 & 0xFE) * 64 + t
    return (r1 & 0xFE) * 64 + (t - 0x80)


def spr_tile(r2345, t):
    """CHR tile number of sprite tile t (pattern table $1000, 8x8 sprites) with R2..R5.

    R2..R5 are 1 KiB each at $1000/$1400/$1800/$1C00 (chr_banks $EA-$ED,
    written by nmi at $C090)."""
    return r2345[t >> 6] * 64 + (t & 0x3F)


# ---- colours ----------------------------------------------------------------
# A Python port of nesrecomp/runner/cyc/hw_palette.c (the oracle's NTSC signal
# model), so PNGs written here use the same RGB as the oracle's --screenshot.
_LEVEL_LOW = (0.350, 0.518, 0.962, 1.550)
_LEVEL_HIGH = (1.094, 1.506, 1.962, 1.962)
_BLACK, _WHITE, _EMPH = 0.518, 1.962, 0.746


def _to_byte(c):
    if c <= 0:
        return 0
    if c >= 1:
        return 255
    return int(c * 255.0 + 0.5)


def nes_rgb(index):
    """RGB of a 9-bit PPU colour index (colour | emphasis << 6)."""
    hue, level, emphasis = index & 0x0F, (index >> 4) & 3, index >> 6
    if hue > 13:
        level = 1
    low, high = _LEVEL_LOW[level], _LEVEL_HIGH[level]
    if hue == 0:
        low = high
    if hue > 12:
        high = low

    def in_phase(h, p):
        return (h + p) % 12 < 6

    y = u = v = 0.0
    for p in range(12):
        s = high if in_phase(hue, p) else low
        if ((emphasis & 1) and in_phase(0, p)) or ((emphasis & 2) and in_phase(4, p)) or \
                ((emphasis & 4) and in_phase(8, p)):
            s *= _EMPH
        s = (s - _BLACK) / (_WHITE - _BLACK)
        angle = math.pi * (p - 0.5) / 6.0
        y += s
        u += s * math.cos(angle)
        v -= s * math.sin(angle)
    y /= 12.0
    u *= 2.0 / 12.0
    v *= 2.0 / 12.0
    r = y + 1.140 * v
    g = y - 0.395 * u - 0.581 * v
    b = y + 2.032 * u
    return _to_byte(r), _to_byte(g), _to_byte(b)


NES_PALETTE = [nes_rgb(i) for i in range(64)]
GREYS = [(0, 0, 0), (85, 85, 85), (170, 170, 170), (255, 255, 255)]
