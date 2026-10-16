"""Minimal PNG writer and reader (standard library only: zlib + struct).

Used by extract_assets.py and assets_verify.py. Writes 8-bit indexed images
(colour type 3, with PLTE and an optional tRNS entry) and 8-bit RGB/RGBA;
reads the non-interlaced 8-bit RGB/RGBA/grey/indexed images the oracle's
--screenshot writes.
"""
import struct
import zlib

_SIG = b"\x89PNG\r\n\x1a\n"


def _chunk(kind, data):
    body = kind + data
    return struct.pack(">I", len(data)) + body + struct.pack(">I", zlib.crc32(body) & 0xFFFFFFFF)


def write_indexed(path, width, height, rows, palette, transparent=None):
    """rows: `height` sequences of `width` palette indices (bytes, bytearray or lists).
    palette: list of (r, g, b), at most 256. transparent: index drawn fully transparent."""
    if not 0 < len(palette) <= 256:
        raise ValueError("palette must have 1..256 entries")
    raw = bytearray()
    for y in range(height):
        row = rows[y]
        if len(row) != width:
            raise ValueError(f"row {y} has {len(row)} pixels, expected {width}")
        raw.append(0)                       # filter: none
        raw += bytes(row)
    out = bytearray(_SIG)
    out += _chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 3, 0, 0, 0))
    out += _chunk(b"PLTE", b"".join(bytes(c[:3]) for c in palette))
    if transparent is not None:
        trns = bytearray([255] * (transparent + 1))
        trns[transparent] = 0
        out += _chunk(b"tRNS", bytes(trns))
    out += _chunk(b"IDAT", zlib.compress(bytes(raw), 9))
    out += _chunk(b"IEND", b"")
    with open(path, "wb") as f:
        f.write(out)


def write_rgb(path, width, height, rows, alpha=False):
    """rows: `height` byte strings of width*3 (or width*4 with alpha) bytes."""
    n = 4 if alpha else 3
    raw = bytearray()
    for y in range(height):
        row = bytes(rows[y])
        if len(row) != width * n:
            raise ValueError(f"row {y} has {len(row)} bytes, expected {width * n}")
        raw.append(0)
        raw += row
    out = bytearray(_SIG)
    out += _chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 6 if alpha else 2, 0, 0, 0))
    out += _chunk(b"IDAT", zlib.compress(bytes(raw), 9))
    out += _chunk(b"IEND", b"")
    with open(path, "wb") as f:
        f.write(out)


def _paeth(a, b, c):
    p = a + b - c
    pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
    if pa <= pb and pa <= pc:
        return a
    return b if pb <= pc else c


def read_png(path):
    """Returns (width, height, rows) with rows as RGB byte strings (width*3 bytes).
    Handles 8-bit grey (0), RGB (2), indexed (3), grey+alpha (4), RGBA (6), non-interlaced."""
    data = open(path, "rb").read()
    if data[:8] != _SIG:
        raise ValueError(f"{path}: not a PNG")
    at, idat, plte = 8, bytearray(), None
    width = height = depth = ctype = interlace = None
    while at < len(data):
        n, kind = struct.unpack_from(">I4s", data, at)
        body = data[at + 8:at + 8 + n]
        at += 12 + n
        if kind == b"IHDR":
            width, height, depth, ctype, _, _, interlace = struct.unpack(">IIBBBBB", body)
        elif kind == b"PLTE":
            plte = body
        elif kind == b"IDAT":
            idat += body
        elif kind == b"IEND":
            break
    if depth != 8 or interlace:
        raise ValueError(f"{path}: only 8-bit non-interlaced PNGs are supported")
    bpp = {0: 1, 2: 3, 3: 1, 4: 2, 6: 4}[ctype]
    raw = zlib.decompress(bytes(idat))
    stride = width * bpp
    prev = bytearray(stride)
    rows = []
    pos = 0
    for _ in range(height):
        ft = raw[pos]
        line = bytearray(raw[pos + 1:pos + 1 + stride])
        pos += 1 + stride
        if ft == 1:
            for i in range(bpp, stride):
                line[i] = (line[i] + line[i - bpp]) & 0xFF
        elif ft == 2:
            for i in range(stride):
                line[i] = (line[i] + prev[i]) & 0xFF
        elif ft == 3:
            for i in range(stride):
                left = line[i - bpp] if i >= bpp else 0
                line[i] = (line[i] + ((left + prev[i]) >> 1)) & 0xFF
        elif ft == 4:
            for i in range(stride):
                left = line[i - bpp] if i >= bpp else 0
                up_left = prev[i - bpp] if i >= bpp else 0
                line[i] = (line[i] + _paeth(left, prev[i], up_left)) & 0xFF
        prev = line
        if ctype == 2:
            rows.append(bytes(line))
        elif ctype == 6:
            rows.append(bytes(b for i, b in enumerate(line) if i % 4 != 3))
        elif ctype == 0:
            rows.append(bytes(v for g in line for v in (g, g, g)))
        elif ctype == 4:
            rows.append(bytes(v for i, g in enumerate(line) if i % 2 == 0 for v in (g, g, g)))
        else:
            rows.append(b"".join(plte[3 * i:3 * i + 3] for i in line))
    return width, height, rows
