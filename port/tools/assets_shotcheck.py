"""Compare an extracted area image (PNG) with an oracle screenshot (PNG).

    py -3 -I port/tools/assets_shotcheck.py AREA.png SHOT.png MEMDUMP [--camera X] [--yscroll N]
        [--palette C0,C1,...,C15]

MEMDUMP is the oracle's --mem-out of the same frame (--mem-frame N with
--screenshot at --frames N+1). The camera comes from its RAM ($7D, $F9/$FC)
unless given; --yscroll is the vertical scroll the frame was drawn with
(RAM $FA as the previous frame left it, signed). Pixels of lines 40-191,
x 8-255 are compared in RGB, except under any sprite's 8x8 box (OAM from the
dump) and in the newest column at the right edge (x >= 256 - camera mod 8),
whose attribute half the column writer has not rewritten yet. Lines 32-39
show nametable row 4, which the column writer blanks, and are left out with
the status rows. In round 1 areas 1-2 lines 40-103 are compared at camera 0
(they do not scroll and repeat every 256 pixels) and lines 104-108 are left
out.

The image has the colours of --palette (the area's, from its JSON); a pixel
also counts as equal when the frame's palette RAM has the screen's colour in
a slot whose area colour is the image's (palette animations and fades change
palette RAM, not the image). Without --palette only exact RGB counts.
"""
import argparse
import collections
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import assets_nes  # noqa: E402
import assets_png  # noqa: E402


def load_mem(path):
    out = {}
    for line in open(path):
        parts = line.split()
        if parts and parts[0] in ("ram", "oam", "palette", "ciram"):
            out.setdefault(parts[0], bytearray()).extend(int(x, 16) for x in parts[2:])
    return out


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("area_png")
    p.add_argument("shot_png")
    p.add_argument("memdump")
    p.add_argument("--camera", type=int)
    p.add_argument("--yscroll", type=int, default=0)
    p.add_argument("--palette", help="the area palette: 16 colour indices, comma separated (hex)")
    a = p.parse_args()
    aw, ah, area = assets_png.read_png(a.area_png)
    sw, sh, shot = assets_png.read_png(a.shot_png)
    mem = load_mem(a.memdump)
    ram, oam, pal_ram = mem["ram"], mem["oam"], mem["palette"]
    if a.camera is None:
        lo = ram[0x7D]
        cx = (ram[0xF9] - (1 if ram[0xFC] < lo else 0)) * 256 + lo
    else:
        cx = a.camera
    still = (ram[0x61], ram[0x60]) in ((1, 1), (1, 2))
    lag_from = 256 - (cx & 7) if cx & 7 else 256
    # area colour -> screen colours that palette RAM may show for it
    allowed = {}
    if a.palette:
        area_pal = [int(v, 16) for v in a.palette.replace("$", "").split(",")]
        for slot in range(16):
            if slot % 4 == 0 and slot:
                continue
            c = bytes(assets_nes.NES_PALETTE[area_pal[slot]])
            allowed.setdefault(c, set()).add(bytes(assets_nes.NES_PALETTE[pal_ram[slot if slot % 4 else 0]]))
    covered = set()
    for i in range(64):
        y, x = oam[4 * i], oam[4 * i + 3]
        if y < 0xEF:
            for yy in range(y + 1, y + 9):
                for xx in range(x, min(x + 8, 256)):
                    covered.add((xx, yy))
    index = {}
    for i, c in enumerate(assets_nes.NES_PALETTE):
        index.setdefault(bytes(c), i)
    same = via_palette = diff = 0
    pairs = collections.Counter()
    for y in range(40, 192):
        if still and 104 <= y < 109:
            continue
        ry = y - 32 + a.yscroll
        if not 8 <= ry < 160:
            continue
        cam = 0 if still and y < 104 else cx
        arow, srow = area[ry], shot[y]
        for x in range(8, lag_from):
            if (x, y) in covered:
                continue
            ea, es = bytes(arow[3 * (cam + x):3 * (cam + x) + 3]), bytes(srow[3 * x:3 * x + 3])
            if ea == es:
                same += 1
            elif es in allowed.get(ea, ()):
                via_palette += 1
            else:
                diff += 1
                pairs[(index.get(ea), index.get(es))] += 1
    print(f"round {ram[0x61]} area {ram[0x60]}, camera {cx}, y scroll {a.yscroll}: {same} pixels equal, "
          f"{via_palette} equal through the frame's palette RAM, {diff} different")
    for (ca, cs), n in pairs.most_common():
        print(f"    image ${ca:02X} vs screen ${cs:02X}: {n} pixels")
    sys.exit(1 if diff else 0)


if __name__ == "__main__":
    main()
