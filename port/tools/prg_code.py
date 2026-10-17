"""List the PRG bytes the P0 map classes as code, for the asset extractor.

    py -3 -I port/tools/prg_code.py runs/cdl/prgmap.bin > port/tools/prg_code.txt

The map (tools/cdl_report.py --map-out, one class byte per PRG byte) comes
from the coverage runs and stays under runs/. The list it gives is offsets
only, no ROM bytes, like docs/prg_map.md: port/tools/assets_runtime.py reads
it to leave code out of build/assets/prg.bin, so a dump is all a user needs.
Code is classes 0-3 (opcode, operand, and code?/operand? found statically).
"""
import sys

CLASSES = ["opcode", "operand", "code?", "operand?", "data", "table", "ptr", "pcm", "fill", "unseen"]
CODE = {0, 1, 2, 3}


def code_ranges(pmap):
    """(bank, offset, length) of each run of code bytes, split at the 8 KiB banks."""
    out = []
    for bank in range(len(pmap) // 0x2000):
        start = None
        for off in range(0x2001):
            is_code = off < 0x2000 and pmap[bank * 0x2000 + off] in CODE
            if is_code and start is None:
                start = off
            elif not is_code and start is not None:
                out.append((bank, start, off - start))
                start = None
    return out


def main():
    if len(sys.argv) != 2:
        sys.exit(__doc__)
    pmap = open(sys.argv[1], "rb").read()
    if len(pmap) != 0x20000:
        sys.exit(f"{sys.argv[1]}: {len(pmap)} bytes, expected one class byte per PRG byte (131072)")
    ranges = code_ranges(pmap)
    print("# PRG bytes the P0 map (runs/cdl/prgmap.bin) classes as code: opcode, operand, and")
    print("# code?/operand? (reached statically, never executed). Offsets only, no ROM bytes.")
    print("# Written by port/tools/prg_code.py; port/tools/assets_runtime.py leaves these bytes")
    print("# out of build/assets/prg.bin.")
    print(f"# {len(ranges)} ranges, {sum(r[2] for r in ranges)} bytes.")
    print("# bank (8 KiB), offset in the bank, length; hex")
    for bank, off, n in ranges:
        print(f"{bank:02X} {off:04X} {n:04X}")


if __name__ == "__main__":
    main()
