"""Check the extracted backgrounds and sprite tables against the oracle.

    py -3 -I port/tools/assets_verify.py ROM [--exe build_headless/Release/MightyFinalFightRecomp.exe] \
        [--state STATE] [--input ROUTE] --first A --last B [--keep LOG] [--report FILE] [--contexts FILE]
    py -3 -I port/tools/assets_verify.py ROM --log EXISTING.flog [...]

Runs the cycle-accurate recomp (the oracle) headless with --frame-log for
frames A..B (from power-on or from a save state), then for every gameplay
frame (docs/assets.md, "Verification"):

- nametable: each nametable column must hold the world column the column
  writer (L0F_E521) last sent there, rows 4..24 (rows 4 and 24 tile 0), and
  each attribute byte the halves of the world columns 4c+1 / 4c+3 last sent;
- picture: the playfield (lines 34..191, x 8..255) of the oracle's own
  picture (colour indices) must equal the extracted world drawn the way that
  nametable holds it, with the frame's x scroll ($7D; 0 on the still lines of
  round 1 areas 1-2), y scroll ($FA as the previous frame left it) and
  palette RAM, except under the 8x8 box of any sprite in OAM; x 0..7 must be
  the backdrop (PPUMASK $18 hides both layers there);
- palette and CHR: palette RAM $3F00-$3F0F against the area's palette (or a
  scripted one), entries an active palette animation slot owns may hold any
  of its steps; frames on other CHR banks than the area's are skipped;
- sprites: the $0200 page objects_draw built must equal
  assets_sprites.simulate_objects_draw run on the frame's RAM with the
  extracted tables (visible entries all four bytes, hidden ones their y).

--contexts merges, per (bank pair, animation), the CHR R2-R5 and sprite
palettes it was drawn with and the object ids that used it into a JSON file
for extract_assets.py --sprite-contexts. Frame logs are big (135 KB per
frame); without --keep the log is deleted.
"""
import argparse
import os
import struct
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import assets_nes  # noqa: E402
import assets_sprites as sp  # noqa: E402
import assets_stage as st  # noqa: E402

# ---- the oracle and its frame log ---------------------------------------------


def run_oracle(exe, rom, log, first, last, state=None, inputs=None):
    cmd = [os.path.abspath(exe), rom, "--frames", str(last + 1), "--frame-log", log,
           "--frame-log-frames", f"{first}:{last}"]
    if state:
        cmd += ["--load-state", state]
    if inputs:
        cmd += ["--input", inputs]
    flags = 0x08000000 if os.name == "nt" else 0          # CREATE_NO_WINDOW
    r = subprocess.run(cmd, capture_output=True, text=True, creationflags=flags)
    if r.returncode != 0:
        raise RuntimeError(f"oracle failed ({r.returncode}): {r.stderr.strip()[-400:]}")
    return r.stderr


def read_frame_log(path):
    """Yields one dict per frame: frame, palette, oam, ram, ciram, picture (u16 LE bytes)."""
    with open(path, "rb") as f:
        if f.read(8) != b"CYCFRAME":
            raise ValueError(f"{path}: not a frame log")
        (version,) = struct.unpack("<I", f.read(4))
        if version != 2:
            raise ValueError(f"{path}: frame log version {version}, expected 2")
        while True:
            head = f.read(36)
            if len(head) < 36:
                return
            frame, clo, chi, n_ram, n_ci, n_cart, n_chr, n_pic, n_fds = struct.unpack("<9I", head)
            pal = f.read(32)
            oam = f.read(256)
            ram = f.read(n_ram)
            ci = f.read(n_ci)
            f.read(n_cart)
            f.read(n_chr)
            pic = f.read(n_pic)
            f.read(n_fds)
            yield {"frame": frame, "cycles": clo | chi << 32, "palette": pal, "oam": oam,
                   "ram": ram, "ciram": ci, "picture": pic}


# ---- the reference: extracted areas ---------------------------------------------

class AreaRef:
    """Index images (4*palette + pixel, 0..15) and world maps of one area."""

    def __init__(self, rom, rnd, area):
        self.rec = rec = st.area_record(rom, rnd, area)
        # one screen more than the camera can show: its column 0 sits in the
        # nametable (hidden by the left clip) once the camera stops
        ext = dict(rec)
        extra = rec["screen_list"][rec["last_screen"] + 1] if rec["last_screen"] < 7 else rec["screens"][-1]
        ext["screens"] = rec["screens"] + [extra]
        self.maps = st.area_maps(rom, ext)
        self.cols = len(self.maps["tiles"][0])
        tiles = st.shown_tiles(self.maps)
        ident = list(range(16))               # palette slot -> itself: pixel values 0..3
        rows = st.render(rom, tiles, lambda ty, tx: 0, rec["chr_r0"], rec["chr_r1"], ident)
        # value_cols[ry][w]: the 8 pixel values (0..3) of world column w on row ry
        self.value_cols = [[bytes(r[8 * w:8 * w + 8]) for w in range(self.cols)] for r in rows]
        self.tiles = tiles

    def nt_attrs(self, W):
        """The attribute bytes (5 rows x 8) the column writer has left once world
        column W is the last one sent: each byte's left half comes from the last
        world column = 4c+1 (mod 32) and its right half from the last = 4c+3."""
        out = []
        for r in range(5):
            line = []
            for ac in range(8):
                wl = W - ((W - (4 * ac + 1)) % 32)
                wr = W - ((W - (4 * ac + 3)) % 32)
                line.append((self.maps["attrs"][r][wl // 4] & 0x33) | (self.maps["attrs"][r][wr // 4] & 0xCC))
            out.append(line)
        return out

    def column(self, w):
        """The 21 tiles the column writer sends for world column w (nametable rows 4..24)."""
        return [self.tiles[y][w] for y in range(20)] + [st.BLANK_TILE]


# Lines drawn with x scroll 0 instead of the camera: round 1 areas 1-2 (RAM
# $61 = 1, $60 = 1/2), where the IRQ handler ($C14A -> $C15A) writes
# PPUSCROLL x = 0 at the first split and the camera's x at the second
# ($C194, latch $4A = 74 lines later).
STATIC_TOP = {(1, 1): (34, 109), (1, 2): (34, 109)}


ADD = [bytes((v + 4 * p) & 0xFF for v in range(256)) for p in range(4)]   # pixel value -> 4*p + value


def palette_table(pal_ram):
    """bytes.translate table: index value (4*p + v) -> colour; v == 0 -> backdrop."""
    t = bytearray(256)
    for i in range(16):
        t[i] = pal_ram[0] if i % 4 == 0 else pal_ram[i]
    return bytes(t)


# ---- one frame ----------------------------------------------------------------

def sprite_boxes(oam):
    """Per screen line, the x ranges covered by a sprite's 8x8 box."""
    lines = {}
    for i in range(64):
        y, t, a, x = oam[4 * i:4 * i + 4]
        if y >= 0xEF:
            continue
        for ly in range(y + 1, min(y + 9, 240)):
            lines.setdefault(ly, []).append((x, min(x + 8, 256)))
    return lines


def check_sprites(rom, fr, prev_ram, contexts):
    """The $0200 page objects_draw built this frame against the extracted tables.
    Hidden entries (y >= $EF) are compared on y only. Also records, per (pair,
    animation), the CHR R2-R5 and sprite palettes in RAM it was drawn with.

    Only frames where objects_draw ran to the end are compared: draw_parity
    ($95, incremented by every objects_draw) moved since the previous frame,
    and task 0 is asleep in task_wait_frame (state 1 at $80) rather than still
    running when the frame ended (a lag frame)."""
    ram = fr["ram"]
    if prev_ram is None or prev_ram[0x95] == ram[0x95]:
        return {"oam_skip": "objects_draw did not run"}
    if ram[0x80] != 1:
        return {"oam_skip": "lag frame (task 0 still running)"}
    page = ram[0x200:0x300]
    sim = sp.simulate_objects_draw(rom, ram, prev_ram)
    bad = 0
    for i in range(64):
        a, b = page[4 * i:4 * i + 4], sim[4 * i:4 * i + 4]
        vis = a[0] < 0xEF or b[0] < 0xEF
        if (vis and a != b) or (not vis and a[0] != b[0]):
            bad += 1
    visible = sum(1 for i in range(64) if page[4 * i] < 0xEF)
    if contexts is not None:
        chr4 = tuple(ram[0xEA:0xEE])
        pals = tuple(tuple([ram[0x600]] + list(ram[0x611 + 4 * p:0x614 + 4 * p])) for p in range(4))
        for slot in range(16):
            if ram[0x300 + slot] & 0x80 and ram[0x560 + slot] & 0x80 and ram[0x580 + slot]:
                oid = ram[0x320 + slot]
                key = (sp.obj_banks(rom)[oid], ram[0x580 + slot] & 0x7F)
                c = contexts.setdefault(key, {"contexts": {}, "objects": {}})
                k2 = (chr4, pals)
                c["contexts"][k2] = c["contexts"].get(k2, 0) + 1
                c["objects"][oid] = c["objects"].get(oid, 0) + 1
    return {"oam_entries_bad": bad, "oam_visible": visible}


def check_frame(refs, fr, stats, rom=None, prev_ram=None, contexts=None):
    ram, ci, pal = fr["ram"], fr["ciram"], fr["palette"]
    rnd, area = ram[0x61], ram[0x60]
    res = {"frame": fr["frame"], "round": rnd, "area": area}
    if rnd > 4 or area > 2 or ram[0xFE] != 0x18 or ram[0xEE] != 0:
        res["skip"] = "not gameplay or rendering off"
        return res
    if ram[0x30] in (0x07, 0x09) or ram[0xB7]:
        # select / continue screens (game_action 7, 9) and the ending ($B7);
        # boss dialogs ($62, gameplay_special_screen) keep the playfield
        res["skip"] = f"screen, game_action ${ram[0x30]:02X}"
        return res
    if rom is not None:
        res.update(check_sprites(rom, fr, prev_ram, contexts))
        if "oam_skip" in res:
            stats["oam_skipped"] = stats.get("oam_skipped", 0) + 1
        else:
            stats["oam_frames"] = stats.get("oam_frames", 0) + 1
            if res["oam_entries_bad"]:
                stats["oam_frames_bad"] = stats.get("oam_frames_bad", 0) + 1
    ref = refs.get((rnd, area))
    if ref is None:
        res["skip"] = "no reference"
        return res
    W = 32 * ram[0x29] + ram[0x24]
    if ram[0x29] == 0xFF or W < 32:
        res["skip"] = "area still being drawn"
        return res
    if ram[0x1A]:
        W -= 1                          # column buffered, not yet uploaded
    rec = ref.rec
    # camera: $7D as copied by this frame's NMI; the high byte may already be newer
    lo = ram[0x7D]
    hi = ram[0xF9] - (1 if ram[0xFC] < lo else 0)
    cx = hi * 256 + lo
    res["camera"] = cx
    res["last_column"] = W
    # what this frame's NMI wrote comes from RAM as the previous frame left it
    if prev_ram is None:
        res["skip"] = "first frame of the log"
        return res
    before = prev_ram
    fy = before[0xFA]
    fy = fy - 256 if fy >= 0x80 else fy           # PPUSCROLL y from nmi_busy (the earthquake shakes it)
    if abs(fy) > 16:
        res["skip"] = f"vertical scroll ${before[0xFA]:02X}"
        return res
    res["y_scroll"] = fy
    if before[0xE8] != rec["chr_r0"] or before[0xE9] != rec["chr_r1"]:
        # another screen's graphics (the round map between rounds)
        res["skip"] = f"CHR R0/R1 {before[0xE8]:02X}/{before[0xE9]:02X}, not the area's"
        return res
    if W != (cx >> 3) + 32:
        # area exit: the camera is reset to 0 before the next area's columns
        res["skip"] = f"columns ({W}) and camera ({cx}) out of step"
        return res
    errs = []
    # palette: the area record's (or a scripted one), entries an active
    # animation slot ($0100-$010F, as RAM has them) owns may hold any step
    bases = [rec["palette"]] + refs["scripted"].get((rnd, area), [])
    anim_owned = {}
    for slot in range(4):
        if before[0x100 + slot] & 0x80:
            a = refs["anims"][before[0x108 + slot] & 0x7F]
            for k in range(3):
                anim_owned[a["palette_offset"] + k] = {s["colours"][k] for s in a["steps"]}
    def faded(v, k):                    # L0E_C362/L0E_C36E: colour - $10*k, $0F when it goes negative
        return v - 16 * k if v - 16 * k >= 0 else 0x0F

    best = None
    for k in range(0, 5):               # 0 = no fade; fades go up to $40 below the target
        for base in bases:
            bad = [i for i in range(16) if (i % 4 or not i) and pal[i] != faded(base[i], k)
                   and pal[i] not in {faded(c, k) for c in anim_owned.get(i, ())}]
            if best is None or len(bad) < len(best[1]):
                best = (k, bad)
        if not best[1]:
            break
    fading = bool(best[1])
    if best[0] and not best[1]:
        res["palette_fade"] = best[0]
    if best[1]:
        res["palette_differs"] = best[1]
    # -- nametable
    nt_bad = 0
    for j in range(32):
        w = W - ((W - j) % 32)
        if w < 0 or w >= ref.cols:
            continue
        exp = ref.column(w)
        for k in range(21):
            if ci[(4 + k) * 32 + j] != exp[k]:
                nt_bad += 1
    at_bad = 0
    nt_at = ref.nt_attrs(W)
    for r in range(5):
        for ac in range(8):
            if ci[0x3C0 + 8 * (r + 1) + ac] != nt_at[r][ac]:
                at_bad += 1
    res["nt_tiles_bad"] = nt_bad
    res["nt_attrs_bad"] = at_bad
    # -- picture: the nametable as the column writer filled it (world column
    # wcol[j] in nametable column j), shown with the IRQ split's x scroll
    table = palette_table(pal)
    pic = fr["picture"]
    lowbytes = pic[0::2]
    if any(pic[1::2]):
        errs.append("emphasis bits set")
    boxes = sprite_boxes(fr["oam"])
    px_bad = px_cmp = 0
    left_bad = 0
    first_bad = None
    backdrop = pal[0]
    wcol = [W - ((W - j) % 32) for j in range(32)]
    static_lines = STATIC_TOP.get((rnd, area), (0, 0))
    for y in range(32, 192):
        line = lowbytes[256 * y:256 * y + 256]
        ry = y - 32 + fy
        if y >= 34 and 0 <= ry < 160:
            src = ref.value_cols[ry]
            arow = nt_at[ry >> 5]
            sh = ((ry >> 4) & 1) * 4
            nt = b"".join(src[wcol[j]].translate(ADD[(arow[j >> 2] >> (sh + (j & 2))) & 3]) for j in range(32))
            sx = 0 if static_lines[0] <= y < static_lines[1] else lo
            nt = nt[sx:] + nt[:sx]
            exp = nt[8:256].translate(table)
            got = line[8:256]
            spans = boxes.get(y)
            if not spans:
                px_cmp += 248
                if got != exp:
                    n = sum(1 for a, b in zip(got, exp) if a != b)
                    px_bad += n
                    if first_bad is None:
                        first_bad = (y, next(i for i in range(248) if got[i] != exp[i]) + 8)
            else:
                mask = bytearray(256)
                for x0, x1 in spans:
                    for x in range(x0, x1):
                        mask[x] = 1
                for i in range(248):
                    if mask[i + 8]:
                        continue
                    px_cmp += 1
                    if got[i] != exp[i]:
                        px_bad += 1
                        if first_bad is None:
                            first_bad = (y, i + 8)
        for x in range(8):
            if line[x] != backdrop:
                left_bad += 1
    res["pixels_compared"] = px_cmp
    res["pixels_bad"] = px_bad
    res["left_clip_bad"] = left_bad
    if first_bad:
        res["first_bad_pixel"] = first_bad
    res["fading"] = fading
    res["errors"] = errs
    key = "ok" if not (nt_bad or at_bad or px_bad or left_bad or errs) else "bad"
    stats[key] = stats.get(key, 0) + 1
    return res


def load_contexts(path):
    """{(pair, anim): {"contexts": {(chr4, palettes): frames}, "objects": {obj_id: frames}}}"""
    out = {}
    if path and os.path.exists(path):
        import json
        for k, v in json.load(open(path)).items():
            pair, anim = (int(x, 16) for x in k.split(":"))
            out[(pair, anim)] = {
                "contexts": {(tuple(c["chr"]), tuple(tuple(p) for p in c["palettes"])): c["frames"]
                             for c in v["contexts"]},
                "objects": {int(o, 16): n for o, n in v["objects"].items()},
            }
    return out


def merge_contexts(into, more):
    for key, info in more.items():
        c = into.setdefault(key, {"contexts": {}, "objects": {}})
        for k, n in info["contexts"].items():
            c["contexts"][k] = c["contexts"].get(k, 0) + n
        for k, n in info["objects"].items():
            c["objects"][k] = c["objects"].get(k, 0) + n
    return into


def save_contexts(path, contexts):
    import json
    obj = {}
    for (pair, anim), seen in sorted(contexts.items()):
        obj[f"{pair:02X}:{anim:02X}"] = {
            "objects": {f"{o:02X}": n for o, n in sorted(seen["objects"].items())},
            "contexts": [{"chr": list(c), "palettes": [list(p) for p in pals], "frames": n}
                         for (c, pals), n in sorted(seen["contexts"].items(), key=lambda kv: -kv[1])],
        }
    nl = chr(10)
    with open(path, "w") as f:
        f.write("{" + nl + ("," + nl).join(f"{json.dumps(k)}: {json.dumps(v)}" for k, v in obj.items())
                + nl + "}" + nl)


def build_refs(rom):
    refs = {(r, a): AreaRef(rom, r, a) for r in range(st.ROUNDS) for a in range(st.AREAS)}
    refs["anims"] = [st.palette_animation(rom, n) for n in range(st.palette_animation_count(rom))]
    refs["scripted"] = {k: st.scripted_palettes(rom, *k) for k in st.SCRIPTED_BG_PALETTES}
    return refs


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("rom")
    p.add_argument("--exe", default="build_headless/Release/MightyFinalFightRecomp.exe")
    p.add_argument("--state")
    p.add_argument("--input")
    p.add_argument("--first", type=int)
    p.add_argument("--last", type=int)
    p.add_argument("--log", help="use an existing frame log instead of running the oracle")
    p.add_argument("--keep", help="where to write (and keep) the frame log")
    p.add_argument("--report", help="append one line per frame here")
    p.add_argument("--quiet", action="store_true")
    p.add_argument("--contexts", help="merge the sprite contexts seen (CHR R2-R5, palettes per animation) "
                                      "into this JSON file, for extract_assets.py --sprite-contexts")
    a = p.parse_args()

    rom = assets_nes.Rom(a.rom)
    refs = build_refs(rom)
    log = a.log
    temp = None
    if not log:
        if a.first is None or a.last is None:
            sys.exit("--first and --last are needed to run the oracle")
        log = a.keep or os.path.join("runs", "assets", "verify", f"tmp_{os.getpid()}.flog")
        os.makedirs(os.path.dirname(log) or ".", exist_ok=True)
        temp = None if a.keep else log
        run_oracle(a.exe, a.rom, log, a.first, a.last, a.state, a.input)
    stats = {}
    per_area = {}
    contexts = load_contexts(a.contexts) if a.contexts else None
    rep = open(a.report, "a") if a.report else None
    prev_ram = None
    try:
        for fr in read_frame_log(log):
            res = check_frame(refs, fr, stats, rom, prev_ram, contexts)
            prev_ram = fr["ram"]
            key = (res["round"], res["area"])
            pa = per_area.setdefault(key, {"frames": 0, "ok": 0, "skipped": 0, "pixels": 0, "pixels_bad": 0,
                                           "nt_bad": 0, "attr_bad": 0, "palette_frames": 0, "fade_frames": 0,
                                           "oam": 0, "oam_bad": 0, "oam_sprites": 0, "oam_skipped": 0})
            if "oam_skip" in res:
                pa["oam_skipped"] += 1
            if "oam_entries_bad" in res:
                pa["oam"] += 1
                pa["oam_sprites"] += res["oam_visible"]
                if res["oam_entries_bad"]:
                    pa["oam_bad"] += 1
                    if not a.quiet:
                        print("frame", res["frame"], "OAM entries differ:", res["oam_entries_bad"])
            if "skip" in res:
                pa["skipped"] += 1
                stats["skipped"] = stats.get("skipped", 0) + 1
                why = res["skip"].split(",")[0].split("(")[0].strip()
                rng = pa.setdefault("skips", {}).setdefault(why, [])
                if rng and rng[-1][1] == res["frame"] - 1:
                    rng[-1][1] = res["frame"]
                else:
                    rng.append([res["frame"], res["frame"]])
            else:
                pa["frames"] += 1
                pa["pixels"] += res["pixels_compared"]
                pa["pixels_bad"] += res["pixels_bad"]
                pa["nt_bad"] += res["nt_tiles_bad"]
                pa["attr_bad"] += res["nt_attrs_bad"]
                pa["palette_frames"] += 1 if res.get("fading") else 0
                pa["fade_frames"] += 1 if res.get("palette_fade") else 0
                if not (res["pixels_bad"] or res["nt_tiles_bad"] or res["nt_attrs_bad"] or res["left_clip_bad"]
                        or res["errors"]):
                    pa["ok"] += 1
            if rep:
                rep.write(repr(res) + "\n")
            if not a.quiet and "skip" not in res and (res["pixels_bad"] or res["nt_tiles_bad"] or
                                                      res["nt_attrs_bad"] or res["errors"] or res["left_clip_bad"]):
                print("frame", res["frame"], {k: v for k, v in res.items() if k != "frame"})
    finally:
        if rep:
            rep.close()
        if temp and os.path.exists(temp):
            os.remove(temp)
    if contexts is not None:
        save_contexts(a.contexts, contexts)
    for (r, ar), pa in sorted(per_area.items()):
        print(f"round {r} area {ar}: {pa['frames']} frames compared ({pa['ok']} exact), {pa['skipped']} skipped; "
              f"pixels {pa['pixels'] - pa['pixels_bad']}/{pa['pixels']} equal; nametable tile diffs {pa['nt_bad']}, "
              f"attribute diffs {pa['attr_bad']}; palette: {pa['fade_frames']} frames in a fade step, "
              f"{pa['palette_frames']} not explained by the tables")
        print(f"    sprites: {pa['oam'] - pa['oam_bad']}/{pa['oam']} frames with the OAM page rebuilt exactly "
              f"({pa['oam_sprites']} visible sprites); {pa['oam_skipped']} frames without a full objects_draw")
        for why, rng in pa.get("skips", {}).items():
            print(f"    skipped ({why}): " + ", ".join(f"{a}-{b}" if a != b else f"{a}" for a, b in rng[:12])
                  + (" ..." if len(rng) > 12 else ""))
    print("totals:", stats)


if __name__ == "__main__":
    main()
