"""Round backgrounds: area records, screens, metatiles, blocks (docs/assets.md).

How the game reads them (fixed banks, addresses are CPU addresses):

- round_bank (0E:C1F8, 5 bytes) gives the PRG bank mapped at $A000 for a
  round: L0E_C3DD and the column writer L0F_E547 index it with $22/$61.
- Area record k (sub_area $60 = 0..2) at $AE60 + $30*k in that bank:
    +$00..$07 screen list (screen numbers), read at L0F_E66E ($AE60/$AE90/$AEC0 + $29)
    +$08      header: bit 5 = the camera may scroll (L0F_E2C7), bits 0-4 = last
              screen index ($2C, the number of screen widths the camera can move)
    +$10..$17 four (sprite set, palette set) pairs indexed by $2B (always 0)
              (L0E_C44F; the sprite set goes to bank 09 $A000, the palette set
              picks 20 bytes at +$1C + 20*n)
    +$18/$19  CHR R0/R1 for the background ($E8/$E9, L0E_C3F3)
    +$1C..$2B background palette, 16 bytes -> $0600/$0620 (L0E_C494)
    +$2C..$2F four palette animation slots: bit 7 = on, bits 0-6 = animation
              number (L0E_C4A5 -> $0100/$0108, run by bank 09 $A300)
- Screen s: 64 bytes at $AF00 + 64*s, 8 metatile columns x 8 rows; the
  column writer draws rows 0-4 only (L0F_E653 / L0F_E5BE).
- Metatile m (32x32 px): 4 block numbers at $B700 + 4*m (TL, TR, BL, BR; L0F_E60F).
- Block b (16x16 px): tiles $BB00+b (TL), $BC00+b (TR), $BD00+b (BL),
  $BE00+b (BR); palette $BF00+b & 3 (L0F_E61A).
- The column writer (L0F_E521) sends 21 tiles to nametable rows 4..24 with
  the first (row 4) and the last (row 24) replaced by tile 0, and one
  attribute byte per metatile at $23C8 + 8*row + col (odd columns only, half
  a byte at a time).
"""
from assets_nes import bg_tile

ROUND_BANK_ADDR = 0xC1F8         # bank 0E
ROUNDS = 5
AREAS = 3
AREA_BASE, AREA_STRIDE = 0xAE60, 0x30
SCREEN_BASE = 0xAF00
METATILE_BASE = 0xB700
BLOCK_TILES = (0xBB00, 0xBC00, 0xBD00, 0xBE00)   # TL, TR, BL, BR
BLOCK_ATTR = 0xBF00
SCREEN_ROWS_DRAWN = 5            # metatile rows the column writer sends
NT_FIRST_ROW = 4                 # nametable row of metatile row 0
BLANK_TILE = 0

# Sprite sets (bank 09, $A000 with A = set; called from L0E_C527)
SPRITE_SET_BANK = 0x09
SPRITE_SET_R3 = 0xA035           # + 2*set
SPRITE_SET_R4 = 0xA036           # + 2*set
SPRITE_SET_R5 = 0xA0B5           # + 2*set
SPRITE_SET_PAL = 0xA105          # + 8*set -> $0618..$061F (sprite palettes 2, 3)

# Palette animation (bank 09 $A300, slots set up by L0E_C4A5)
PAL_ANIM_BANK = 0x09
PAL_ANIM_INDEX = 0xA445          # animation n -> offset of its record from $A458
PAL_ANIM_RECORDS = 0xA458        # [last step, period, palette offset, triplet per step...]
PAL_ANIM_TRIPLETS = 0xA370       # 3 colours each

# Player sprite palettes ($0610-$0617), bank 0E: Cody, Guy, Haggar (L0E_C417)
PLAYER_PALETTES = (0xC53E, 0xC546, 0xC54E)

# Background palettes loaded by scripts instead of the area record. Round 0
# area 2 (the first boss room) has an all-black record palette; game action
# $1C (00:96EF, set by 0F:E104 when that area starts) turns the lights on with
# 16 bytes from 00:97F9 (copied at 00:976A and 00:979A) and then 00:9809
# (00:97B2), and starts palette animation slots itself (00:97C8, STA $0102 at
# 00:97CA).
SCRIPTED_BG_PALETTES = {(0, 2): [(0x00, 0x97F9), (0x00, 0x9809)]}


def scripted_palettes(rom, rnd, area):
    return [list(rom.bytes(b, a, 16)) for b, a in SCRIPTED_BG_PALETTES.get((rnd, area), [])]


def round_banks(rom):
    return list(rom.bytes(0x0E, ROUND_BANK_ADDR, ROUNDS))


def area_record(rom, rnd, area):
    """The area record of round `rnd` (0-4, RAM $61) and area `area` (0-2, RAM $60)."""
    bank = round_banks(rom)[rnd]
    base = AREA_BASE + AREA_STRIDE * area
    hdr = rom.byte(bank, base + 8)
    pairs = [tuple(rom.bytes(bank, base + 0x10 + 2 * i, 2)) for i in range(4)]
    sprite_set, palette_set = pairs[0]
    pal_addr = base + 0x1C + 20 * palette_set
    last = hdr & 0x1F
    screens = list(rom.bytes(bank, base, 8))
    return {
        "round": rnd,
        "area": area,
        "bank": bank,
        "record_addr": base,
        "header": hdr,
        "scrolls": bool(hdr & 0x20),
        "last_screen": last,
        "screens": screens[:last + 1],
        "screen_list": screens,
        "pairs": pairs,
        "sprite_set": sprite_set,
        "palette_set": palette_set,
        "palette_addr": pal_addr,
        "chr_r0": rom.byte(bank, base + 0x18),
        "chr_r1": rom.byte(bank, base + 0x19),
        "palette": list(rom.bytes(bank, pal_addr, 16)),
        "palette_anim_slots": list(rom.bytes(bank, pal_addr + 16, 4)),
    }


def cpu_read(rom, bank_a000, addr):
    """A CPU read at $A000-$FFFF while the column writer runs (round bank at $A000,
    banks 0E/0F fixed above it)."""
    if addr < 0xC000:
        return rom.byte(bank_a000, addr)
    return rom.byte(0x0E if addr < 0xE000 else 0x0F, addr)


def screen_metatiles(rom, bank, screen, rows=SCREEN_ROWS_DRAWN):
    """rows x 8 metatile numbers of one screen.

    Real screens sit below $B700; a garbage screen number past the list's
    end (read only for the hidden column right of the last screen) can point
    past $BFFF into the fixed banks, which is what the CPU reads then."""
    base = SCREEN_BASE + 64 * screen
    raw = [cpu_read(rom, bank, base + i) for i in range(8 * rows)]
    return [raw[8 * r:8 * r + 8] for r in range(rows)]


def metatile_blocks(rom, bank, m):
    return list(rom.bytes(bank, METATILE_BASE + 4 * m, 4))


def block_tiles(rom, bank, b):
    """(TL, TR, BL, BR) tile numbers and the palette (0-3) of block b."""
    return [rom.byte(bank, a + b) for a in BLOCK_TILES], rom.byte(bank, BLOCK_ATTR + b) & 3


def area_maps(rom, rec, rows=SCREEN_ROWS_DRAWN):
    """Metatile, block, tile and attribute maps of a whole area, screens left to right.

    tiles[y][x]: rows*4 tile rows (nametable rows 4.. as stored, before the
    column writer blanks row 4); attrs[r][c]: one NES attribute byte per metatile."""
    bank = rec["bank"]
    n = len(rec["screens"])
    meta = [[0] * (8 * n) for _ in range(rows)]
    blocks = [[0] * (16 * n) for _ in range(rows * 2)]
    tiles = [[0] * (32 * n) for _ in range(rows * 4)]
    pals = [[0] * (16 * n) for _ in range(rows * 2)]
    attrs = [[0] * (8 * n) for _ in range(rows)]
    for si, s in enumerate(rec["screens"]):
        for r, line in enumerate(screen_metatiles(rom, bank, s, rows)):
            for c, m in enumerate(line):
                x = 8 * si + c
                meta[r][x] = m
                a = 0
                for q, b in enumerate(metatile_blocks(rom, bank, m)):
                    bx, by = 2 * x + (q & 1), 2 * r + (q >> 1)
                    blocks[by][bx] = b
                    (tl, tr, bl, br), p = block_tiles(rom, bank, b)
                    pals[by][bx] = p
                    tiles[2 * by][2 * bx], tiles[2 * by][2 * bx + 1] = tl, tr
                    tiles[2 * by + 1][2 * bx], tiles[2 * by + 1][2 * bx + 1] = bl, br
                    a |= p << (2 * q)
                attrs[r][x] = a
    return {"metatiles": meta, "blocks": blocks, "block_palettes": pals, "tiles": tiles, "attrs": attrs}


def shown_tiles(maps):
    """The tile map as the column writer leaves it in nametable rows 4..23: row 4 is tile 0."""
    t = [list(r) for r in maps["tiles"]]
    t[0] = [BLANK_TILE] * len(t[0])
    return t


def render(rom, tiles, pals_of_tile, r0, r1, palette):
    """Colour indices (0..63) of a tile map. pals_of_tile(y, x) -> palette 0-3."""
    h, w = len(tiles), len(tiles[0])
    cache = {}
    out = []
    for ty in range(h):
        lines = [bytearray() for _ in range(8)]
        for tx in range(w):
            p = pals_of_tile(ty, tx)
            key = (tiles[ty][tx], p)
            rows = cache.get(key)
            if rows is None:
                t = rom.tile(bg_tile(r0, r1, key[0]))
                colours = [palette[0], palette[4 * p + 1], palette[4 * p + 2], palette[4 * p + 3]]
                rows = [bytes(colours[v] for v in line) for line in t]
                cache[key] = rows
            for y in range(8):
                lines[y] += rows[y]
        out.extend(bytes(l) for l in lines)
    return out


def tile_palette_from_attrs(attrs):
    """pals_of_tile for a map whose attribute grid starts at tile row 0 of `tiles`."""
    def f(ty, tx):
        a = attrs[ty // 4][tx // 4]
        return (a >> (((ty >> 1) & 1) * 4 + ((tx >> 1) & 1) * 2)) & 3
    return f


def sprite_set(rom, s):
    """CHR R3, R4, R5 and sprite palettes 2-3 (8 bytes) of a sprite set (bank 09 $A000)."""
    r3 = rom.byte(SPRITE_SET_BANK, SPRITE_SET_R3 + 2 * s)
    r4 = rom.byte(SPRITE_SET_BANK, SPRITE_SET_R4 + 2 * s)
    r5 = rom.byte(SPRITE_SET_BANK, SPRITE_SET_R5 + 2 * s)
    pal = list(rom.bytes(SPRITE_SET_BANK, SPRITE_SET_PAL + 8 * s, 8))
    return {"r3": r3, "r4": r4, "r5": r5, "palettes_2_3": pal,
            "addr": {"r3": SPRITE_SET_R3 + 2 * s, "r4": SPRITE_SET_R4 + 2 * s,
                     "r5": SPRITE_SET_R5 + 2 * s, "palettes": SPRITE_SET_PAL + 8 * s}}


def palette_animation(rom, n):
    """Palette animation n as bank 09 $A321 runs it.

    Each slot counts a timer up to `period`, then moves to the next step
    (0..last, wrapping) and writes the step's 3 colours at palette RAM
    offset `offset`..`offset`+2 (and to $0620+). The first write happens
    after `period`+1 frames and shows step 1."""
    base = rom.byte(PAL_ANIM_BANK, PAL_ANIM_INDEX + n)
    rec = PAL_ANIM_RECORDS + base
    last, period, offset = rom.bytes(PAL_ANIM_BANK, rec, 3)
    steps = []
    for i in range(last + 1):
        k = rom.byte(PAL_ANIM_BANK, rec + 3 + i)
        steps.append({"triplet": k, "colours": list(rom.bytes(PAL_ANIM_BANK, PAL_ANIM_TRIPLETS + 3 * k, 3))})
    return {"number": n, "record_addr": rec, "last_step": last, "period": period,
            "palette_offset": offset, "steps": steps}


def palette_animation_count(rom):
    """Entries of the $A445 index: it ends where the $A458 records begin."""
    return PAL_ANIM_RECORDS - PAL_ANIM_INDEX


def player_palettes(rom):
    return [list(rom.bytes(0x0E, a, 8)) for a in PLAYER_PALETTES]


# Nametable patches of scripted area exits: bank 00 L00_987A copies 128 bytes
# from patch Y (pointer lo $989A+Y, hi $989E+Y) to $0780 and sets $19; nmi
# writes them with L0E_C201 ([hi, lo, count-1, bytes...] until a byte >= $80,
# PPUCTRL increment +1).
PATCH_BANK = 0x00
PATCH_PTR_LO, PATCH_PTR_HI, PATCH_COUNT = 0x989A, 0x989E, 4
PATCH_CALLERS = {0: ["00:967C"], 1: ["00:948A"], 2: ["00:949D"], 3: ["00:959A"]}   # JSR L00_987A


def nametable_patches(rom):
    out = []
    for i in range(PATCH_COUNT):
        addr = rom.word(PATCH_BANK, PATCH_PTR_LO + i, PATCH_PTR_HI + i)
        raw = rom.bytes(PATCH_BANK, addr, 128) if (addr & 0x1FFF) <= 0x2000 - 128 else \
            rom.bytes(PATCH_BANK, addr, 0x2000 - (addr & 0x1FFF))
        runs, at = [], 0
        while at < len(raw) and raw[at] < 0x80:
            hi, lo, n = raw[at], raw[at + 1], raw[at + 2]
            data = list(raw[at + 3:at + 4 + n])
            ppu = hi << 8 | lo
            runs.append({"ppu_addr": f"${ppu:04X}", "row": (ppu - 0x2000) // 32 if ppu < 0x23C0 else None,
                         "col": (ppu - 0x2000) % 32 if ppu < 0x23C0 else None,
                         "bytes": [f"${v:02X}" for v in data]})
            at += 4 + n
        out.append({"patch": i, "addr": f"00:${addr:04X}", "called_from": PATCH_CALLERS.get(i, []),
                    "length": at + 1, "runs": runs})
    return out
