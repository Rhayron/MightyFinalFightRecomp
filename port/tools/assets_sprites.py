"""Object sprites: animations, frames and layouts as object_draw (0F:E9FE) reads them.

The format (docs/assets.md has the evidence):

- obj_bank table (0F:EB93, 160 entries by obj_id $0320): the PRG bank pair
  object_draw maps, B at $8000 and B+1 at $A000 ($0A/$0B or $02/$03).
- Animation a (RAM $0580 & $7F): pointer lo $8000+a, hi $8080+a in bank B.
  Script: [last, period, frame id x (last+1)]. object_draw counts $0590 up
  to `period`, then advances $0570 (0..last, wrapping); frame id 0 ends the
  object (slot cleared).
- Frame f: pointer lo $8100+f, hi $8200+f in bank B. Frame data:
  [n-1 | $80 if the layout is in bank $03 (else $0B), layout index,
   (tile, attribute) x n].
- Layout l (in the bank the frame names, mapped at $A000): pointer lo
  $B100+l, hi $B200+l; data [(dy, dx) x n], signed, relative to (obj x
  on screen, obj_y). With obj_flags bit 6 the object faces the other way:
  x = screen x + ($F8 - dx) and the attribute's bit 6 is flipped; obj_flags
  bit 5 ORs the priority bit.
- Sprites are 8x8 from pattern table $1000: tile >> 6 picks CHR R2..R5
  (chr_banks $EA-$ED), attribute bits 0-1 the sprite palette.
- objects_draw (0F:E9BC) draws slot 15, then the slot at X-1 where X is
  what object_draw returned (the OAM offset if slot 15 put sprites in OAM),
  then 0..15 or 13..0 by draw_parity. objects_draw() below is a model of it,
  used by assets_verify.py to rebuild the OAM page of every checked frame.
"""
import json
import os

import assets_nes
import assets_png

OBJ_BANK_TABLE = 0xEB93          # bank 0F, 160 entries
OBJ_IDS = 160
ANIM_LO, ANIM_HI = 0x8000, 0x8080
FRAME_LO, FRAME_HI = 0x8100, 0x8200
LAYOUT_LO, LAYOUT_HI = 0xB100, 0xB200
LAYOUT_BANK_DEFAULT, LAYOUT_BANK_ALT = 0x0B, 0x03
PAIRS = (0x0A, 0x02)


def s8(v):
    return v - 256 if v >= 128 else v


class CpuView:
    """$8000-$FFFF as object_draw sees it: bank B at $8000, `a000` at $A000, fixed 0E/0F."""

    def __init__(self, rom, b8000, ba000):
        self.rom, self.b8000, self.ba000 = rom, b8000, ba000

    def __getitem__(self, addr):
        addr &= 0xFFFF
        if addr < 0x8000:
            raise ValueError(f"object_draw read below $8000: ${addr:04X}")
        if addr < 0xA000:
            return self.rom.prg[self.b8000 * 0x2000 + (addr & 0x1FFF)]
        if addr < 0xC000:
            return self.rom.prg[self.ba000 * 0x2000 + (addr & 0x1FFF)]
        return self.rom.prg[(0x0E if addr < 0xE000 else 0x0F) * 0x2000 + (addr & 0x1FFF)]


def obj_banks(rom):
    return list(rom.bytes(0x0F, OBJ_BANK_TABLE, OBJ_IDS))


def anim_ptr(rom, pair, a):
    v = CpuView(rom, pair, pair + 1)
    return v[ANIM_LO + a] | v[ANIM_HI + a] << 8


_cache = {}


def animation(rom, pair, a):
    key = ("a", id(rom), pair, a)
    if key in _cache:
        return _cache[key]
    v = CpuView(rom, pair, pair + 1)
    p = anim_ptr(rom, pair, a)
    last, period = v[p], v[p + 1]
    frames = [v[p + 2 + i] for i in range(last + 1)]
    _cache[key] = r = {"anim": a, "addr": p, "last": last, "period": period, "frames": frames}
    return r


def frame(rom, pair, f):
    """Frame f of a bank pair: sprites as (dx, dy, tile, attr), plus where it came from."""
    key = ("f", id(rom), pair, f)
    if key in _cache:
        return _cache[key]
    v = CpuView(rom, pair, pair + 1)
    p = v[FRAME_LO + f] | v[FRAME_HI + f] << 8
    head = v[p]
    lbank = LAYOUT_BANK_ALT if head & 0x80 else LAYOUT_BANK_DEFAULT
    n = (head & 0x7F) + 1
    v2 = CpuView(rom, pair, lbank)        # object_draw maps the layout bank before reading on
    li = v2[p + 1]
    lp = v2[LAYOUT_LO + li] | v2[LAYOUT_HI + li] << 8
    sprites = []
    for i in range(n):
        tile, attr = v2[p + 2 + 2 * i], v2[p + 3 + 2 * i]
        dy, dx = s8(v2[lp + 2 * i]), s8(v2[lp + 2 * i + 1])
        sprites.append((dx, dy, tile, attr))
    _cache[key] = r = {"frame": f, "addr": p, "layout_bank": lbank, "layout": li, "layout_addr": lp,
                       "sprites": sprites}
    return r


# ---- objects_draw, simulated (for checking the format against the oracle) ----

FAR_DELETE_LIST = 0xECC3         # bank 0F: ids L0F_EC33 deletes when far off screen, $FF-terminated
OBJ_TABLES_CLEARED = (0x300, 0x310, 0x560, 0x470, 0x4B0, 0x320, 0x330, 0x440, 0x450, 0x4C0, 0x4D0, 0x4E0,
                      0x5B0, 0x5C0, 0x5D0, 0x5E0, 0x5F0, 0x4F0, 0x3F0)   # L0F_EC81, then $0480 = $FF


def _pair_of(rom, oid):
    return rom.prg[0x0F * 0x2000 + (OBJ_BANK_TABLE & 0x1FFF) + oid]


def _anim_of(rom, m, x):
    return animation(rom, _pair_of(rom, m[0x320 + x]), m[0x580 + x] & 0x7F)


def _update(m, x, an):
    """object_draw's animation step (0F:EAA4-EACB)."""
    t = m[0x590 + x] & 0x7F
    m[0x590 + x] = (m[0x590 + x] + 1) & 0xFF
    if t == an["period"]:
        m[0x590 + x] &= 0x80
        f = m[0x570 + x] & 0x7F
        m[0x570 + x] = (m[0x570 + x] + 1) & 0xFF
        if f == an["last"]:
            m[0x570 + x] = 0


def _rewind(m, x, an, k):
    """Undo k animation steps (the timer runs 0..period, a reset advances the frame)."""
    for _ in range(k):
        t = m[0x590 + x] & 0x7F
        if t:
            m[0x590 + x] = (m[0x590 + x] & 0x80) | (t - 1)
        else:
            m[0x590 + x] = (m[0x590 + x] & 0x80) | an["period"]
            f = m[0x570 + x] & 0x7F
            m[0x570 + x] = (m[0x570 + x] & 0x80) | (f - 1 if f else an["last"])


def _clear(m, x):
    for base in OBJ_TABLES_CLEARED:
        m[(base + x) & 0x7FF] = 0
    m[0x480 + x] = 0xFF


def _far(rom, m, x):
    """L0F_EC33: delete (round 3 area 1, or ids in the list) or pull 16 px toward the camera."""
    if m[0x61] == 3 and m[0x60] == 1:
        _clear(m, x)
        return
    i = 0
    while True:
        v = rom.prg[0x0F * 0x2000 + (FAR_DELETE_LIST & 0x1FFF) + i]
        if v == m[0x320 + x]:
            _clear(m, x)
            return
        if v == 0xFF:
            break
        i += 1
    pos = m[0x380 + x] << 8 | m[0x360 + x]
    pos += 16 if (m[0xF9] - m[0x380 + x]) & 0x80 == 0 else -16
    m[0x360 + x], m[0x380 + x] = pos & 0xFF, (pos >> 8) & 0xFF


def object_draw(rom, m, x, oam, st):
    """One call of object_draw (0F:E9FE) on RAM copy m; returns the X it leaves.
    st["idx"] is the OAM offset ($97). Updates m like the game does."""
    flags = m[0x560 + x]
    if flags & 0x10:
        lo = m[0x360 + x] - m[0xFC]
        sx = lo & 0xFF
        hi = m[0x380 + x] - m[0xF9] - (1 if lo < 0 else 0)
        if hi & 0xFF:
            v = sx if hi >= 0 else (-sx) & 0xFF
            if v >= 0x68 or m[0x580 + x] == 0x7E or (m[0x61] == 3 and m[0x60] == 1):
                _far(rom, m, x)
                return x
            m[0x560 + x] &= 0x7F                       # off screen: animate, do not draw
            visible = False
        elif m[0x3E0 + x]:
            # 0F:EA1B-EA23: bytes no coverage run executed; treated as not drawn
            st["unmodelled"] = st.get("unmodelled", 0) + 1
            return x
        else:
            visible = True
    else:
        sx = m[0x360 + x]
        visible = True
    if visible:
        m[0x560 + x] |= 0x80
        if m[0x560 + x] & 0x04:
            return x
    if not m[0x580 + x]:
        return x
    an = _anim_of(rom, m, x)
    _update(m, x, an)
    st["updated"].append(x)
    if not m[0x560 + x] & 0x80:
        return x
    if m[0x590 + x] & 0x80 and m[0x92] & 0x04:
        return x                                      # blinking
    fi = m[0x570 + x] & 0x7F
    pair = _pair_of(rom, m[0x320 + x])
    fid = CpuView(rom, pair, pair + 1)[an["addr"] + 2 + fi]     # LDA ($00),Y even past the script
    if fid == 0:
        _clear(m, x)                                   # the script ended: slot cleared
        return x
    if st["idx"] == 0:
        return 0
    obj = {"id": m[0x320 + x], "flags": m[0x560 + x], "x": m[0x360 + x], "y": m[0x3C0 + x]}
    st["idx"] = _place(rom, m, oam, st["idx"], obj, fid)
    return st["idx"]


def _place(rom, ram, oam, idx, obj, fid):
    """object_draw's sprite loop for frame fid of one object; returns the new OAM index (0 = full)."""
    pair = _pair_of(rom, obj["id"])
    flags = obj["flags"]
    flip, prio = flags & 0x40, flags & 0x20
    fr = frame(rom, pair, fid)
    sx = (obj["x"] - ram[0xFC]) & 0xFF if flags & 0x10 else obj["x"]
    sy = obj["y"]
    for dx, dy, tile, attr in fr["sprites"]:
        oam[idx + 1] = tile
        y = sy + (dy & 0xFF)
        oam[idx] = y & 0xFF
        carry = y > 0xFF
        if (dy >= 0 and carry) or (dy < 0 and not carry) or (y & 0xFF) >= 0xF0:
            oam[idx] = 0xF8
            continue
        oam[idx + 2] = (attr ^ flip) | prio
        d = dx & 0xFF
        if flip:
            d = (0xF8 - d) & 0xFF
        x = sx + d
        oam[idx + 3] = x & 0xFF
        carry = x > 0xFF
        if (s8(d) >= 0 and carry) or (s8(d) < 0 and not carry):
            oam[idx] = 0xF8
            continue
        idx = (idx + 4) & 0xFF
        if idx == 0:
            return 0
    return idx


def objects_draw(rom, m, oam):
    """objects_draw (0F:E9BC) on RAM copy m. Returns the X values whose animation
    object_draw stepped, in order. Quirk: the second call uses X - 1 where X is what the first
    call (slot 15) returned, which is the OAM offset when slot 15 drew sprites."""
    st = {"idx": 4, "updated": []}
    calls = []
    x = 15
    if m[0x300 + x] & 0x80:
        calls.append(x)
        x = object_draw(rom, m, x, oam, st)
    x = (x - 1) & 0xFF
    if m[(0x300 + x) & 0x7FF] & 0x80:
        calls.append(x)
        object_draw(rom, m, x, oam, st)
    m[0x95] = (m[0x95] + 1) & 0xFF
    order = range(13, -1, -1) if m[0x95] & 1 else range(16)
    for s in order:
        if m[0x300 + s] & 0x80:
            calls.append(s)
            object_draw(rom, m, s, oam, st)
    m[0x97] = st["idx"]
    return st["updated"]


def simulate_objects_draw(rom, ram, prev_ram=None):
    """The OAM page ($0200) objects_draw built this frame, from RAM at the frame's
    end (and the previous frame's RAM): objects the draw itself removed (frame
    id 0; L0F_EC81 clears state, flags and id but not position or animation)
    get their state, id and flags back from the previous frame, each slot's
    animation state is put back to what the previous frame left (or, when the
    behaviour code restarted it, wound back by the steps the draw gave it),
    then the objects_draw model runs forward."""
    base = bytearray(ram)
    base[0x95] = (ram[0x95] - 1) & 0xFF
    if prev_ram is not None:
        for s in range(16):
            if not ram[0x300 + s] & 0x80 and prev_ram[0x300 + s] & 0x80 and ram[0x580 + s] \
                    and not ram[0x320 + s] and not ram[0x590 + s] & 0x7F:
                pair = _pair_of(rom, prev_ram[0x320 + s])
                an = animation(rom, pair, ram[0x580 + s] & 0x7F)
                if CpuView(rom, pair, pair + 1)[an["addr"] + 2 + (ram[0x570 + s] & 0x7F)] == 0:
                    for t in (0x300, 0x320, 0x560):
                        base[t + s] = prev_ram[t + s]
    # The animation state before the draw is the previous frame's when the
    # behaviour code left the animation alone; slots where running the draw from
    # there does not end in this frame's state are wound back from this frame's
    # state instead, by the number of steps the draw gave them.
    from_prev = set()
    if prev_ram is not None:
        for s in range(16):
            if base[0x300 + s] & 0x80 and prev_ram[0x300 + s] & 0x80 and prev_ram[0x580 + s] == ram[0x580 + s]:
                base[0x570 + s], base[0x590 + s] = prev_ram[0x570 + s], prev_ram[0x590 + s]
                from_prev.add(s)
    counts = {s: (2 if not ram[0x95] & 1 and s >= 14 else 1) for s in range(16) if base[0x300 + s] & 0x80}
    oam = None
    for _ in range(4):
        m = bytearray(base)
        for x, k in counts.items():
            if x not in from_prev and m[0x580 + x]:
                m[0x570 + x], m[0x590 + x] = ram[0x570 + x], ram[0x590 + x]
                _rewind(m, x, _anim_of(rom, m, x), k)
        oam = bytearray(prev_ram[0x200:0x300] if prev_ram is not None else bytes(256))
        oam[0] = 0xF8
        for i in range(4, 256, 4):
            oam[i] = 0xF8
        stepped = objects_draw(rom, m, oam)
        new_counts = {}
        for x in stepped:
            new_counts[x] = new_counts.get(x, 0) + 1
        wrong = {s for s in from_prev if (m[0x570 + s], m[0x590 + s]) != (ram[0x570 + s], ram[0x590 + s])}
        keys = set(counts) | set(new_counts)
        if not wrong and all(new_counts.get(x, 0) == counts.get(x, 0) for x in keys if x not in from_prev):
            break
        from_prev -= wrong
        counts = {x: new_counts.get(x, 0) for x in keys}
    return bytes(oam)


# ---- extraction -------------------------------------------------------------

def tables(rom):
    """Every animation of both bank pairs and every frame they use."""
    out = {}
    for pair in PAIRS:
        anims = [animation(rom, pair, a) for a in range(128)]
        used = sorted({f for an in anims for f in an["frames"] if f})
        frames = {f: frame(rom, pair, f) for f in used}
        out[pair] = {"animations": anims, "frames": frames}
    return out


def frame_box(fr):
    xs = [s[0] for s in fr["sprites"]] or [0]
    ys = [s[1] for s in fr["sprites"]] or [0]
    return min(xs), min(ys), max(xs) + 8, max(ys) + 8


def extract(rom, out, written, contexts=None):
    """sprites.json with every table; with contexts (assets_verify.load_contexts:
    the CHR R2-R5 and sprite palettes each (pair, animation) was seen drawn
    with, and on which object ids), one sheet per object id."""
    d = os.path.join(out, "sprites")
    os.makedirs(d, exist_ok=True)
    tb = tables(rom)
    banks = obj_banks(rom)
    obj = {
        "obj_bank_table": "0F:$EB93",
        "obj_bank": {str(i): f"${b:02X}" for i, b in enumerate(banks)},
        "pairs": {},
    }
    for pair, t in tb.items():
        obj["pairs"][f"{pair:02X}"] = {
            "anim_table": f"{pair:02X}:$8000 (lo) / $8080 (hi)",
            "frame_table": f"{pair:02X}:$8100 (lo) / $8200 (hi)",
            "animations": [{"anim": an["anim"], "addr": f"${an['addr']:04X}", "last": an["last"],
                            "period": an["period"], "frames": an["frames"]} for an in t["animations"]],
            "frames": {str(f): {"addr": f"${fr['addr']:04X}", "layout_bank": f"${fr['layout_bank']:02X}",
                                "layout": fr["layout"], "layout_addr": f"${fr['layout_addr']:04X}",
                                "sprites": [{"dx": dx, "dy": dy, "tile": f"${tl:02X}", "attr": f"${at:02X}"}
                                            for dx, dy, tl, at in fr["sprites"]]}
                       for f, fr in t["frames"].items()},
        }
    p = os.path.join(d, "sprites.json")
    with open(p, "w", newline="\n") as f:
        json.dump(obj, f, indent=1)
        f.write("\n")
    written.append(p)
    if not contexts:
        return
    sheets(rom, tb, banks, contexts, d, written)


def draw_frame(canvas, ox, oy, rom, fr, chr4, palettes):
    """Draw a frame into an RGBA canvas (rows of bytearrays) with the object at (ox, oy).
    Earlier sprites of a frame are in front, as lower OAM entries are on the NES."""
    for dx, dy, tile, attr in reversed(fr["sprites"]):
        t = rom.tile(assets_nes.spr_tile(chr4, tile))
        pal = palettes[attr & 3]
        for yy in range(8):
            row = t[7 - yy if attr & 0x80 else yy]
            line = canvas[oy + dy + yy]
            for xx in range(8):
                v = row[7 - xx if attr & 0x40 else xx]
                if v:
                    x = 4 * (ox + dx + xx)
                    line[x:x + 4] = bytes(assets_nes.NES_PALETTE[pal[v]]) + b"\xff"


def sheets(rom, tb, banks, contexts, d, written):
    """One sheet per object id seen in the contexts: a row per animation, its
    distinct frames left to right in script order, each cell anchored at the
    object's position, drawn with the animation's most frequent context."""
    by_obj = {}
    for (pair, a), info in contexts.items():
        for oid in info["objects"]:
            by_obj.setdefault(oid, []).append((pair, a))
    index = {}
    for oid in sorted(by_obj):
        rows = []
        for pair, a in sorted(by_obj[oid]):
            an = tb[pair]["animations"][a]
            order = []
            for f in an["frames"]:
                if f and f not in order:
                    order.append(f)
            if not order:
                continue
            ctx = max(contexts[(pair, a)]["contexts"].items(), key=lambda kv: kv[1])[0]
            rows.append((pair, a, order, ctx))
        if not rows:
            continue
        boxes = [frame_box(tb[p]["frames"][f]) for p, _, order, _ in rows for f in order]
        x0, y0 = min(b[0] for b in boxes), min(b[1] for b in boxes)
        x1, y1 = max(b[2] for b in boxes), max(b[3] for b in boxes)
        cw, ch = x1 - x0 + 2, y1 - y0 + 2
        ncols = max(len(r[2]) for r in rows)
        w, h = cw * ncols, ch * len(rows)
        canvas = [bytearray(4 * w) for _ in range(h)]
        entry = []
        for ri, (pair, a, order, (chr4, pals)) in enumerate(rows):
            for ci, f in enumerate(order):
                draw_frame(canvas, ci * cw + 1 - x0, ri * ch + 1 - y0, rom, tb[pair]["frames"][f], chr4, pals)
            entry.append({"row": ri, "pair": f"{pair:02X}", "anim": f"{a:02X}", "frames": order,
                          "chr_r2_r5": [f"{c:02X}" for c in chr4], "sprite_palettes": [list(p) for p in pals]})
        p = os.path.join(d, f"obj{oid:02X}.png")
        assets_png.write_rgb(p, w, h, canvas, alpha=True)
        written.append(p)
        index[f"{oid:02X}"] = {"image": os.path.basename(p), "cell": [cw, ch],
                               "anchor_in_cell": [1 - x0, 1 - y0], "rows": entry}
    p = os.path.join(d, "sheets.json")
    with open(p, "w", newline="\n") as f:
        json.dump(index, f, indent=1)
        f.write("\n")
    written.append(p)
