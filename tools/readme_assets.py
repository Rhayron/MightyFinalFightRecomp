"""Regenerate the README images in docs/img/ from your own ROM.

    uv run --no-project --with pillow tools/readme_assets.py

Needs build_headless/ (tools/build.ps1 -Headless), the states the coverage work
leaves in states/ (title_4500, r1_<char>_start, rounds/) and, for the oracle
panel, build_mesen_shot/ and build_cyc/. Nothing here is committed with the
ROM: the images are screenshots of runs of the user's own dump.
"""
import os
import struct
import subprocess
import sys
import tempfile

from PIL import Image, ImageDraw, ImageFont

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
EXE = os.path.join(ROOT, "build_headless", "Release", "MightyFinalFightRecomp.exe")
ROM = os.path.join(ROOT, "rom", "Mighty Final Fight (USA).nes")
OUT = os.path.join(ROOT, "docs", "img")
TMP = tempfile.mkdtemp(prefix="mff_readme_")
NOWIN = getattr(subprocess, "CREATE_NO_WINDOW", 0)

YELLOW, INK, BG = (255, 214, 64), (20, 18, 28), (16, 14, 24)


def run(*args):
    subprocess.run([EXE, ROM, *args], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
                   check=True, creationflags=NOWIN)


def state_frame(path):
    return struct.unpack_from("<I", open(path, "rb").read(), 28)[0]


def frames(tag, start_args, first, last, every, inp=None):
    """Run to `last` and return the PNG paths of frames first..last step every."""
    d = os.path.join(TMP, tag)
    os.makedirs(d, exist_ok=True)
    args = [*start_args, "--frames", str(last + 1), "--screenshot", os.path.join(d, "s.png"),
            "--shot-every", str(every)]
    if inp:
        args += ["--input", inp]
    run(*args)
    shots = sorted(f for f in os.listdir(d) if f.startswith("s_"))
    return [os.path.join(d, f) for f in shots if first <= int(f[2:-4]) <= last]


def shot(tag, start_args, frame, inp=None):
    """The picture at the end of `frame` (the run's last frame)."""
    out = os.path.join(TMP, f"{tag}.png")
    args = [*start_args, "--frames", str(frame + 1), "--screenshot", out]
    if inp:
        args += ["--input", inp]
    run(*args)
    return out


def font(size):
    for name in ("consolab.ttf", "consola.ttf", "arialbd.ttf"):
        try:
            return ImageFont.truetype(name, size)
        except OSError:
            pass
    return ImageFont.load_default()


def gif(paths, out, scale=2, fps=30, crop=None):
    ims = []
    for p in paths:
        im = Image.open(p).convert("RGB")
        if crop:
            im = im.crop(crop)
        ims.append(im.resize((im.width * scale, im.height * scale), Image.NEAREST)
                   .quantize(colors=64, method=Image.MEDIANCUT, dither=Image.NONE))
    ims[0].save(out, save_all=True, append_images=ims[1:], duration=int(1000 / fps), loop=0,
                optimize=True, disposal=1)
    print(f"{os.path.relpath(out, ROOT)}: {len(ims)} frames, {os.path.getsize(out) // 1024} KiB")


def big(path, scale=2):
    im = Image.open(path).convert("RGB")
    return im.resize((im.width * scale, im.height * scale), Image.NEAREST)


def label(draw, xy, text, size=18, fill=YELLOW):
    f = font(size)
    x, y = xy
    draw.text((x + 2, y + 2), text, font=f, fill=(0, 0, 0))
    draw.text((x, y), text, font=f, fill=fill)


# ---- hero: title screen flanked by the three fighters from the select screen ----
def hero():
    title = shot("title", ["--load-state", "states/title_4500.cycstate"], 4510)
    sel_in = os.path.join(TMP, "sel.txt")
    open(sel_in, "w").write("4520 START\n4528 -\n")
    select = shot("select", ["--load-state", "states/title_4500.cycstate"], 4580, sel_in)
    t, s = Image.open(title).convert("RGB"), Image.open(select).convert("RGB")
    W, H = 1280, 440
    im = Image.new("RGB", (W, H), BG)
    logo = t.crop((16, 30, 240, 130)).resize((224 * 3, 100 * 3), Image.NEAREST)
    im.paste(logo, ((W - logo.width) // 2, 8))
    d = ImageDraw.Draw(im)
    boxes = {"CODY": (24, 56, 72, 104), "GUY": (24, 112, 72, 160), "HAGGAR": (24, 168, 72, 216)}
    xs = {"CODY": 36, "GUY": W - 36 - 2 * 144 - 16, "HAGGAR": W - 36 - 144}
    for name, box in boxes.items():
        face = s.crop(box).resize((144, 144), Image.NEAREST)
        x, y = xs[name], H - 144 - 44
        d.rectangle([x - 4, y - 4, x + 147, y + 147], outline=YELLOW, width=3)
        im.paste(face, (x, y))
        label(d, (x + (144 - len(name) * 14) // 2, y + 152), name, 24)
    label(d, (310, H - 120), "NES  →  C  →  native x64", 40)
    label(d, (270, H - 62), "cycle-accurate static recompilation · NESRecomp", 24, (230, 230, 240))
    im.save(os.path.join(OUT, "hero.png"))
    print("docs/img/hero.png")


# ---- gameplay GIFs: replayable bot sessions, one per fighter ----
def gameplay():
    for c, (a, b) in {"cody": (5150, 5510), "guy": (5880, 6240), "haggar": (5300, 5660)}.items():
        st = f"states/r1_{c}_start.cycstate"
        shots = frames(f"g_{c}", ["--load-state", st], a, b, 2, f"inputs/bot1_{c}.txt")
        gif(shots, os.path.join(OUT, f"gameplay_{c}.gif"), scale=2, fps=30, crop=(0, 8, 256, 232))


def boss():
    st = "runs/god6_cody/f340669.cycstate"
    if not os.path.exists(st):
        return
    f0 = state_frame(st)
    taps = os.path.join(TMP, "taps.txt")
    open(taps, "w").write("\n".join(f"{f0 + i} B\n{f0 + i + 3} -" for i in range(0, 1500, 8)))
    data = bytearray(open(st, "rb").read())
    sys.path.insert(0, os.path.join(ROOT, "tools"))
    import state_ram
    base, _ = state_ram.ram_span(data)
    data[base + 0x4AC] = 1                      # Belger, one hit from his second phase
    st2 = os.path.join(TMP, "belger.cycstate")
    open(st2, "wb").write(data)
    shots = frames("boss", ["--load-state", st2], f0 + 900, f0 + 1500, 3, taps)
    gif(shots, os.path.join(OUT, "final_boss.gif"), scale=2, fps=20, crop=(0, 8, 256, 232))


# ---- round tour: one frame per round + the ending ----
def rounds():
    names = ["SLUM", "RIVERSIDE", "OLD TOWN", "FACTORY", "BAY AREA"]
    picks = [("cody", 1, 1500), ("guy", 2, 900), ("haggar", 3, 1200), ("cody", 4, 2400), ("guy", 5, 600)]
    tiles = []
    for (c, r, dt), name in zip(picks, names):
        st = f"states/rounds/{c}_r{r}.cycstate"
        f0 = state_frame(st)
        tiles.append((shot(f"r{r}", ["--load-state", st], f0 + dt), f"ROUND {r} · {name}"))
    for p, cap in (("runs/god6_cody/f342477.png", "ENDING"), ("runs/god5_cody/f350269.png", "CREDITS")):
        if os.path.exists(p):
            tiles.append((p, cap))
    w, h, pad = 384, 360, 12
    cols = 3
    rows = (len(tiles) + cols - 1) // cols
    im = Image.new("RGB", (cols * (w + pad) + pad, rows * (h + 40 + pad) + pad), BG)
    d = ImageDraw.Draw(im)
    for i, (p, cap) in enumerate(tiles):
        x, y = pad + (i % cols) * (w + pad), pad + (i // cols) * (h + 40 + pad)
        im.paste(Image.open(p).convert("RGB").resize((w, h), Image.NEAREST), (x, y + 40))
        label(d, (x + 4, y + 8), cap, 22)
    im.save(os.path.join(OUT, "rounds.png"))
    print("docs/img/rounds.png")


# ---- what the bot sees: RAM object slots drawn over the frame ----
def bot_vision():
    st = "states/r1_cody_start.cycstate"
    f = 6600
    pic = shot("vision", ["--load-state", st], f, "inputs/01_cody_s1.txt")
    mem = os.path.join(TMP, "vision_mem.txt")
    run("--load-state", st, "--frames", str(f + 1), "--input", "inputs/01_cody_s1.txt",
        "--mem-frame", str(f), "--mem-out", mem)
    ram = bytearray()
    for line in open(mem):
        if line.startswith("ram "):
            ram += bytes.fromhex(line.split(":", 1)[1])
    s = 3
    im = big(pic, s)
    d = ImageDraw.Draw(im)
    names = {0x20: "POISON", 0x21: "TWO P", 0x22: "BRED", 0x23: "AXL", 0x24: "EL GADO", 0x25: "ANDORE"}
    live = [i for i in [0] + list(range(3, 0x10)) if ram[0x300 + i] & 0x80]
    for row, i in enumerate(live):
        x, y = ram[0x360 + i], ram[0x3C0 + i]
        color = (80, 220, 255) if i == 0 else (255, 80, 110)
        d.rectangle([(x - 13) * s, (y - 44) * s, (x + 13) * s, (y + 2) * s], outline=color, width=3)
        d.ellipse([(x - 2) * s, (y - 2) * s, (x + 2) * s, (y + 2) * s], fill=color)
        if i == 0:
            txt = f"slot 00 · PLAYER · x={x:02X} y={y:02X} · hp={ram[0x3A]}"
        else:
            txt = f"slot {i:02X} · {names.get(ram[0x320 + i], hex(ram[0x320 + i]))} · x={x:02X} y={y:02X} · life={ram[0x4A0 + i]}"
        ly = 150 + row * 30
        tw = int(d.textlength(txt, font=font(17)))
        d.line([(30 + tw, ly + 12), (x * s, (y - 44) * s)], fill=color, width=2)
        d.rectangle([20, ly - 2, 34 + tw, ly + 26], fill=(0, 0, 0))
        label(d, (26, ly), txt, 17, color)
    label(d, (24, im.height - 44), "the bot reads the object slots out of RAM every 16 frames", 22)
    im.save(os.path.join(OUT, "bot_vision.png"))
    print("docs/img/bot_vision.png")


# ---- oracles: NESRecomp vs TriCNES vs Mesen at the first frame Mesen disagrees ----
def oracles():
    mesen = os.path.join(ROOT, "build_mesen_shot", "mesen_shot.exe")
    if not os.path.exists(mesen):
        return
    f = 5232
    route = "inputs/10_cody_poweron.txt"
    ours = shot("oracle", ["--ram-init", "zeros"], f, route)
    subprocess.run([mesen, ROM, os.path.join(TMP, "mesen"), "--input", route, str(f)],
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=True, creationflags=NOWIN)
    mem = os.path.join(TMP, "oracle_mem.txt")
    run("--ram-init", "zeros", "--frames", str(f + 1), "--input", route, "--mem-frame", str(f), "--mem-out", mem)
    sys.path.insert(0, os.path.join(ROOT, "nesrecomp", "tools", "cyc"))
    import cmp_picture
    ia = cmp_picture.read_mem_dump(mem)
    ib = cmp_picture.read_idx(os.path.join(TMP, "mesen", f"{f}.idx"))
    rgb = Image.open(ours).convert("RGB")
    pal = {}
    for n, c in enumerate(rgb.get_flattened_data()):
        pal.setdefault(ia[n] & 0x3F, c)          # our PNG gives each index its colour
    W0, H0 = 256, 240

    def paint(idx, mark=None):
        im = Image.new("RGB", (W0, H0))
        px = im.load()
        for n, v in enumerate(idx):
            c = pal.get(v & 0x3F, (128, 128, 128))
            if mark is not None:
                c = (255, 40, 90) if (v & 0x3F) != (mark[n] & 0x3F) else tuple(q // 4 for q in c)
            px[n % W0, n // W0] = c
        return im

    crop = (140, 100, 210, 160)
    s = 5
    za, zb, zd = (paint(ia).crop(crop), paint(ib).crop(crop), paint(ia, ib).crop(crop))
    ndiff = sum((x & 0x3F) != (y & 0x3F) for x, y in zip(ia, ib))
    W = za.width * s
    im = Image.new("RGB", (3 * W + 4 * 16, za.height * s + 110), BG)
    d = ImageDraw.Draw(im)
    for i, (p, cap) in enumerate(((za, "NESRecomp  (= TriCNES)"), (zb, "Mesen"), (zd, "differing pixels"))):
        x = 16 + i * (W + 16)
        im.paste(p.resize((W, za.height * s), Image.NEAREST), (x, 54))
        label(d, (x, 16), cap, 24)
    label(d, (16, za.height * s + 66), f"frame {f}: {ndiff} of 61,440 pixels differ (first Mesen vs TriCNES split)", 20, (230, 230, 240))
    im.save(os.path.join(OUT, "oracle_5232.png"))
    print("docs/img/oracle_5232.png")


def intro_gif():
    shots = frames("intro", [], 4440, 4700, 2, "inputs/probe2.txt")
    gif(shots, os.path.join(OUT, "title.gif"), scale=2, fps=30)


if __name__ == "__main__":
    os.chdir(ROOT)
    os.makedirs(OUT, exist_ok=True)
    for step in (sys.argv[1:] or ["hero", "intro_gif", "gameplay", "boss", "rounds", "bot_vision", "oracles"]):
        globals()[step]()
