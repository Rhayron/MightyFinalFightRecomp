"""Tile headless screenshots into one labeled contact sheet.

    uv run --with pillow tools/contact_sheet.py OUT.png SHOT [SHOT ...] [--cols N]

Each tile is labeled with the frame number taken from the file name
(shot_00120.png -> 120), so a --shot-every run reads as a timeline.
"""
import re
import sys

from PIL import Image, ImageDraw


def main(argv):
    cols = 6
    if "--cols" in argv:
        i = argv.index("--cols")
        cols = int(argv[i + 1])
        del argv[i:i + 2]
    out, shots = argv[0], argv[1:]
    w, h, label = 256, 240, 12
    rows = (len(shots) + cols - 1) // cols
    sheet = Image.new("RGB", (cols * w, rows * (h + label)), (40, 40, 40))
    draw = ImageDraw.Draw(sheet)
    for n, path in enumerate(shots):
        x, y = (n % cols) * w, (n // cols) * (h + label)
        sheet.paste(Image.open(path).convert("RGB").resize((w, h)), (x, y + label))
        m = re.search(r"_(\d+)\.png$", path)
        draw.text((x + 2, y), m.group(1).lstrip("0") or "0" if m else path, fill=(255, 255, 0))
    sheet.save(out)


if __name__ == "__main__":
    main(sys.argv[1:])
