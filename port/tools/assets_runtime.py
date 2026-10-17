"""The data the port's runtime loads (port/src/runtime/cart.c).

    chr.bin   the CHR ROM: 128 banks of 1 KiB, tiles as the PPU reads them
    prg.bin   the PRG ROM without its code. The routines are C now and read
              PRG only for their tables (prg_read, mem_read through a
              pointer); the bytes the P0 map classes as code
              (port/tools/prg_code.txt) are left out, except the few the
              game also reads as data (code_read_as_data below). Layout,
              little endian:
                  "MFFPRG01"   magic and format
                  u8           iNES header flags 6 (bit 0: vertical mirroring
                               at power-on)
                  u8[3]        0
                  u32          number of ranges
                  per range: u32 PRG offset, u32 length, the bytes
              The runtime stops, naming the byte, if the game reads one that
              is not there.

Written by extract_assets.py (part "runtime"); everything here is from the
user's dump and stays under build/ or runs/.
"""
import os
import struct

import assets_stage as st

FORMAT = 1
MAGIC = b"MFFPRG%02d" % FORMAT
HERE = os.path.dirname(os.path.abspath(__file__))
CODE_LIST = os.path.join(HERE, "prg_code.txt")
CODE_READ_LIST = os.path.join(HERE, "prg_code_read.txt")
PRG_SIZE = 0x20000


def read_ranges(path):
    """(bank, offset, length) lines of a list file (hex; '#' starts a comment)."""
    out = []
    for line in open(path):
        line = line.split("#", 1)[0].split()
        if not line:
            continue
        bank, off, n = (int(v, 16) for v in line[:3])
        if bank >= PRG_SIZE // 0x2000 or off + n > 0x2000:
            raise ValueError(f"{path}: bad range {bank:02X} {off:04X} {n:04X}")
        out.append((bank, off, n))
    return out


def code_mask(code_list=CODE_LIST, prg_map=None):
    """bytearray(PRG_SIZE), 1 where a byte is code. From the committed list,
    or straight from a P0 map (runs/cdl/prgmap.bin) when one is given."""
    mask = bytearray(PRG_SIZE)
    if prg_map:
        pmap = open(prg_map, "rb").read()
        if len(pmap) != PRG_SIZE:
            raise ValueError(f"{prg_map}: {len(pmap)} bytes, expected {PRG_SIZE}")
        for i, cls in enumerate(pmap):
            if cls <= 3:
                mask[i] = 1
        return mask
    for bank, off, n in read_ranges(code_list):
        start = bank * 0x2000 + off
        mask[start:start + n] = b"\x01" * n
    return mask


def past_end_screens(rom):
    """PRG offsets of the screen each area's list names past its last one.

    When the camera stops at an area's end the column writer (0F:E656,
    LDA ($20),Y) has already read the list entry after the last screen and
    draws metatiles of that "screen" into the column under the left clip
    (docs/assets.md, "Round backgrounds"). In five areas the garbage number
    points into the fixed banks, at code. The writer's index there runs a
    little past the 8 columns (offsets 40 and 41 were seen, row 4 with a
    column of 8 and 9), so the whole 64-byte screen is kept."""
    out = set()
    banks = st.round_banks(rom)
    for rnd in range(st.ROUNDS):
        bank = banks[rnd]
        for area in range(st.AREAS):
            base = st.AREA_BASE + st.AREA_STRIDE * area
            last = rom.byte(bank, base + 8) & 0x1F
            screen = st.SCREEN_BASE + 64 * rom.byte(bank, base + last + 1)
            for i in range(64):
                a = screen + i
                b = bank if a < 0xC000 else (0x0E if a < 0xE000 else 0x0F)
                out.add(b * 0x2000 + (a & 0x1FFF))
    return out


def code_read_as_data(rom, read_list=CODE_READ_LIST):
    """PRG offsets of bytes the game reads as data although they are code:
    the past-the-end screens, and what port/tools/prg_code_read.txt lists
    (seen with MFF_PRG_READS, port/tools/prg_reads.py)."""
    out = past_end_screens(rom)
    for bank, off, n in read_ranges(read_list):
        out.update(range(bank * 0x2000 + off, bank * 0x2000 + off + n))
    return out


def left_out(rom, prg_map=None):
    """bytearray(PRG_SIZE), 1 where prg.bin leaves the byte out."""
    mask = code_mask(prg_map=prg_map)
    for off in code_read_as_data(rom):
        mask[off] = 0
    return mask


def data_ranges(mask):
    """(offset, length) of each run of bytes the mask keeps."""
    out, start = [], None
    for i in range(PRG_SIZE + 1):
        keep = i < PRG_SIZE and not mask[i]
        if keep and start is None:
            start = i
        elif not keep and start is not None:
            out.append((start, i - start))
            start = None
    return out


def write(rom, out, written, prg_map=None):
    """chr.bin and prg.bin in `out`; returns a summary for the manifest."""
    code = code_mask(prg_map=prg_map)
    mask = left_out(rom, prg_map)
    ranges = data_ranges(mask)
    p = os.path.join(out, "chr.bin")
    with open(p, "wb") as f:
        f.write(rom.chr)
    written.append(p)
    p = os.path.join(out, "prg.bin")
    with open(p, "wb") as f:
        f.write(MAGIC + bytes([rom.header[6], 0, 0, 0]) + struct.pack("<I", len(ranges)))
        for start, n in ranges:
            f.write(struct.pack("<II", start, n))
            f.write(rom.prg[start:start + n])
    written.append(p)
    kept = sum(n for _, n in ranges)
    return {
        "format": FORMAT,
        "chr": "chr.bin",
        "prg": "prg.bin",
        "prg_bytes": kept,
        "prg_code_bytes": sum(code),
        "prg_code_bytes_left_out": PRG_SIZE - kept,
        "code_list": os.path.basename(prg_map) if prg_map else "port/tools/prg_code.txt",
    }
