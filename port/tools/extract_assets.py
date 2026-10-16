"""Extract Mighty Final Fight's graphics and stage data from the user's dump.

    py -3 -I port/tools/extract_assets.py ROM [--out DIR] [--no-crc] [--only PART ...]
                                              [--sprite-contexts FILE ...]

Default DIR is runs/assets (gitignored). Everything written there comes
from the ROM and must never be committed. Formats and the code that reads
each table: docs/assets.md. Python standard library only.

Output:
    manifest.json                 what was written, ROM CRC, generator version
    chr/chr_XX.png                each 1 KiB CHR bank (64 tiles, 8x8 grid, grey)
    chr/chr_all.png               all 8192 tiles, 32 per row
    palettes/nes.png, palettes.json
                                  the NES colours (the oracle's NTSC model) and
                                  every gameplay palette table
    stage/roundR_areaA.png        the whole playfield of an area as the game
                                  draws it (nametable rows 4..23, screens left
                                  to right; R = RAM $61, A = RAM $60)
    stage/roundR_areaA.json       the area record and its maps (metatiles,
                                  blocks, tiles, attributes)
    stage/roundR_areaA_blocks.png the area's 16x16 blocks, 16 per row
    stage/roundR_tiles.json       the round's metatile and block definitions
    stage/patches.json            nametable patches of scripted area entrances
                                  and exits
    sprites/sprites.json          object bank table, animations, frames, layouts
    sprites/objXX.png, sheets.json
                                  with --sprite-contexts: each object id's
                                  animations drawn with the CHR banks and
                                  palettes the oracle showed them with
"""
import argparse
import json
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import assets_nes  # noqa: E402
import assets_png  # noqa: E402
import assets_stage as st  # noqa: E402

GENERATOR = "extract_assets.py 1"


def write_json(path, obj):
    with open(path, "w", newline="\n") as f:
        json.dump(obj, f, indent=1, sort_keys=False)
        f.write("\n")


def hexs(v):
    return f"${v:04X}" if v > 0xFF else f"${v:02X}"


# ---- CHR ----------------------------------------------------------------------

def extract_chr(rom, out, written):
    d = os.path.join(out, "chr")
    os.makedirs(d, exist_ok=True)
    for bank in range(128):
        rows = [bytearray(64) for _ in range(64)]
        for t in range(64):
            tile = rom.tile(bank * 64 + t)
            ox, oy = 8 * (t % 8), 8 * (t // 8)
            for y in range(8):
                rows[oy + y][ox:ox + 8] = tile[y]
        p = os.path.join(d, f"chr_{bank:02X}.png")
        assets_png.write_indexed(p, 64, 64, rows, assets_nes.GREYS)
        written.append(p)
    per_row = 32
    h = 8192 // per_row * 8
    rows = [bytearray(per_row * 8) for _ in range(h)]
    for n in range(8192):
        tile = rom.tile(n)
        ox, oy = 8 * (n % per_row), 8 * (n // per_row)
        for y in range(8):
            rows[oy + y][ox:ox + 8] = tile[y]
    p = os.path.join(d, "chr_all.png")
    assets_png.write_indexed(p, per_row * 8, h, rows, assets_nes.GREYS)
    written.append(p)


# ---- palettes -------------------------------------------------------------------

def extract_palettes(rom, out, written):
    d = os.path.join(out, "palettes")
    os.makedirs(d, exist_ok=True)
    # NES colour swatch: 16 x 4 cells of 16x16
    rows = []
    for cy in range(4):
        line = bytearray()
        for cx in range(16):
            line += bytes([cy * 16 + cx]) * 16
        rows += [bytes(line)] * 16
    p = os.path.join(d, "nes.png")
    assets_png.write_indexed(p, 256, 64, rows, assets_nes.NES_PALETTE)
    written.append(p)

    banks = st.round_banks(rom)
    areas = []
    sets = set()
    for r in range(st.ROUNDS):
        for a in range(st.AREAS):
            rec = st.area_record(rom, r, a)
            sets.add(rec["sprite_set"])
            areas.append({
                "round": r, "area": a, "bank": hexs(rec["bank"]),
                "palette_addr": hexs(rec["palette_addr"]), "palette": [hexs(v) for v in rec["palette"]],
                "palette_anim_slots": [hexs(v) for v in rec["palette_anim_slots"]],
                "sprite_set": rec["sprite_set"],
            })
    sprite_sets = []
    for s in sorted(sets):
        ss = st.sprite_set(rom, s)
        sprite_sets.append({"set": s, "chr_r3": hexs(ss["r3"]), "chr_r4": hexs(ss["r4"]), "chr_r5": hexs(ss["r5"]),
                            "sprite_palettes_2_3": [hexs(v) for v in ss["palettes_2_3"]],
                            "addr": {k: "09:" + hexs(v) for k, v in ss["addr"].items()}})
    anims = []
    for n in range(st.palette_animation_count(rom)):
        an = st.palette_animation(rom, n)
        anims.append({"number": n, "record": "09:" + hexs(an["record_addr"]), "last_step": an["last_step"],
                      "period": an["period"], "palette_offset": an["palette_offset"],
                      "steps": [[hexs(c) for c in s["colours"]] for s in an["steps"]]})
    players = st.player_palettes(rom)
    scripted = [{"round": r, "area": a, "palettes": [
        {"addr": f"{b:02X}:{hexs(addr)}", "colours": [hexs(v) for v in rom.bytes(b, addr, 16)]}
        for b, addr in st.SCRIPTED_BG_PALETTES[(r, a)]]} for r, a in sorted(st.SCRIPTED_BG_PALETTES)]
    obj = {
        "nes_rgb": [list(c) for c in assets_nes.NES_PALETTE],
        "note": "palette RAM layout: $3F00-$3F0F background (4 x 4), $3F10-$3F1F sprites; colour 0 of every "
                "palette is the backdrop $3F00. The game keeps a copy at $0600 (uploaded by nmi when $18 is set) "
                "and a fade target at $0620.",
        "round_banks": [hexs(b) for b in banks],
        "areas": areas,
        "scripted_background_palettes": scripted,
        "player_sprite_palettes_0_1": [
            {"character": name, "addr": "0E:" + hexs(addr), "colours": [hexs(v) for v in pal]}
            for name, addr, pal in zip(("Cody", "Guy", "Haggar"), st.PLAYER_PALETTES, players)],
        "sprite_sets": sprite_sets,
        "palette_animations": anims,
    }
    p = os.path.join(d, "palettes.json")
    write_json(p, obj)
    written.append(p)


# ---- stage ------------------------------------------------------------------------

def extract_stage(rom, out, written):
    d = os.path.join(out, "stage")
    os.makedirs(d, exist_ok=True)
    banks = st.round_banks(rom)
    for r in range(st.ROUNDS):
        bank = banks[r]
        used_meta, used_blocks = set(), set()
        for a in range(st.AREAS):
            rec = st.area_record(rom, r, a)
            maps = st.area_maps(rom, rec)
            tiles = st.shown_tiles(maps)
            pal_of = st.tile_palette_from_attrs(maps["attrs"])
            # an area whose script replaces the record's palette (round 0 area 2
            # starts black) is drawn with the script's last palette
            scripted = st.scripted_palettes(rom, r, a)
            image_pal = scripted[-1] if scripted else rec["palette"]
            rows = st.render(rom, tiles, pal_of, rec["chr_r0"], rec["chr_r1"], image_pal)
            w = 256 * len(rec["screens"])
            name = f"round{r}_area{a}"
            p = os.path.join(d, name + ".png")
            assets_png.write_indexed(p, w, len(rows), rows, assets_nes.NES_PALETTE)
            written.append(p)
            for line in maps["metatiles"]:
                used_meta.update(line)
            for line in maps["blocks"]:
                used_blocks.update(line)
            area_blocks = sorted({b for line in maps["blocks"] for b in line})
            # block sheet: block b at (b % 16, b // 16), unused blocks left at colour 0
            sheet_tiles = [[0] * 32 for _ in range(32)]
            sheet_pal = [[0] * 16 for _ in range(16)]
            for b in area_blocks:
                (tl, tr, bl, br), pb = st.block_tiles(rom, bank, b)
                x, y = 2 * (b % 16), 2 * (b // 16)
                sheet_tiles[y][x], sheet_tiles[y][x + 1], sheet_tiles[y + 1][x], sheet_tiles[y + 1][x + 1] = \
                    tl, tr, bl, br
                sheet_pal[b // 16][b % 16] = pb
            used_mask = {(b % 16, b // 16) for b in area_blocks}
            srows = st.render(rom, sheet_tiles, lambda ty, tx: sheet_pal[ty // 2][tx // 2],
                              rec["chr_r0"], rec["chr_r1"], image_pal)
            # blank the unused cells with the backdrop
            bd = image_pal[0]
            srows = [bytearray(x) for x in srows]
            for cy in range(16):
                for cx in range(16):
                    if (cx, cy) not in used_mask:
                        for yy in range(16):
                            srows[16 * cy + yy][16 * cx:16 * cx + 16] = bytes([bd]) * 16
            p = os.path.join(d, name + "_blocks.png")
            assets_png.write_indexed(p, 256, 256, srows, assets_nes.NES_PALETTE)
            written.append(p)
            obj = {
                "round": r, "area": a, "bank": hexs(bank),
                "record": f"{bank:02X}:{hexs(rec['record_addr'])}",
                "header": hexs(rec["header"]), "camera_scrolls": rec["scrolls"],
                "last_screen": rec["last_screen"], "screens": rec["screens"],
                "screen_list_bytes": [hexs(v) for v in rec["screen_list"]],
                "sprite_set": rec["sprite_set"], "palette_set": rec["palette_set"],
                "chr_r0": hexs(rec["chr_r0"]), "chr_r1": hexs(rec["chr_r1"]),
                "palette": [hexs(v) for v in rec["palette"]],
                "palette_anim_slots": [hexs(v) for v in rec["palette_anim_slots"]],
                "image": name + ".png",
                "image_palette": [hexs(v) for v in image_pal],
                "image_origin": "pixel (0,0) = world x 0, screen y 32 (nametable row 4); "
                                "camera x = scroll_x_hi*256 + scroll_x",
                "screen_y_of_row0": 32,
                "note_hidden_row": "tiles[0] holds the data's first tile row; the column writer replaces it "
                                   "with tile 0 (nametable row 4) and the image shows tile 0 there",
                "metatiles": maps["metatiles"],
                "blocks": maps["blocks"],
                "tiles": maps["tiles"],
                "attributes": maps["attrs"],
            }
            p = os.path.join(d, name + ".json")
            write_json(p, obj)
            written.append(p)
        tdef = {
            "round": r, "bank": hexs(bank),
            "metatiles_addr": hexs(st.METATILE_BASE), "block_tile_addrs": [hexs(a) for a in st.BLOCK_TILES],
            "block_palette_addr": hexs(st.BLOCK_ATTR),
            "metatiles": {str(m): st.metatile_blocks(rom, bank, m) for m in sorted(used_meta)},
            "blocks": {str(b): dict(zip(("tiles", "palette"), st.block_tiles(rom, bank, b)))
                       for b in sorted(used_blocks)},
        }
        p = os.path.join(d, f"round{r}_tiles.json")
        write_json(p, tdef)
        written.append(p)
    p = os.path.join(d, "patches.json")
    write_json(p, st.nametable_patches(rom))
    written.append(p)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("rom")
    ap.add_argument("--out", default=os.path.join("runs", "assets"))
    ap.add_argument("--no-crc", action="store_true", help="skip the CRC32 check (other dumps are untested)")
    ap.add_argument("--only", choices=["chr", "palettes", "stage", "sprites"], action="append")
    ap.add_argument("--sprite-contexts", action="append",
                    help="contexts JSON from assets_verify.py --contexts (repeatable, merged): also draw one "
                         "sheet per object id (sprites/objXX.png)")
    a = ap.parse_args()
    rom = assets_nes.Rom(a.rom, check_crc=not a.no_crc)
    os.makedirs(a.out, exist_ok=True)
    written = []
    parts = a.only or ["chr", "palettes", "stage", "sprites"]
    if "chr" in parts:
        extract_chr(rom, a.out, written)
    if "palettes" in parts:
        extract_palettes(rom, a.out, written)
    if "stage" in parts:
        extract_stage(rom, a.out, written)
    if "sprites" in parts:
        import assets_sprites
        contexts = None
        if a.sprite_contexts:
            import assets_verify
            contexts = {}
            for path in a.sprite_contexts:
                assets_verify.merge_contexts(contexts, assets_verify.load_contexts(path))
        assets_sprites.extract(rom, a.out, written, contexts)
    files = {os.path.relpath(p, a.out).replace(os.sep, "/") for p in written}
    man_path = os.path.join(a.out, "manifest.json")
    if a.only and os.path.exists(man_path):
        # a partial run keeps what an earlier run of the same dump listed
        old = json.load(open(man_path))
        if old.get("rom_crc32") == f"{rom.crc32:08X}":
            files |= {f for f in old.get("files", []) if os.path.exists(os.path.join(a.out, f))}
    man = {
        "generator": GENERATOR,
        "rom_crc32": f"{rom.crc32:08X}",
        "files": sorted(files),
    }
    write_json(man_path, man)
    print(f"{len(written)} files in {a.out}")


if __name__ == "__main__":
    main()
