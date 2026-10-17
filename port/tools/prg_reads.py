"""Which PRG bytes the port read, against what build/assets/prg.bin leaves out.

    py -3 -I port/tools/prg_reads.py READS.txt [READS.txt ...] [--rom DUMP]

READS files come from the port run with MFF_PRG_READS=FILE on data that
holds every PRG byte (extract_assets.py --prg-map with a map of no code):
every byte the game read through prg_read or mem_read, with the run of
instructions that first read it. Lists the ones prg.bin would leave out,
which the port would stop on, and exits 1 if there is any. With --rom the
check is exactly the extractor's (code list, minus the past-the-end screens
it works out from the dump and port/tools/prg_code_read.txt); without, the
past-the-end screens are not known and show up too. Add a byte that turns
up to prg_code_read.txt with where it was read.
"""
import argparse
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import assets_runtime  # noqa: E402


def load_reads(paths):
    reads = {}
    for p in paths:
        for line in open(p):
            line = line.split("#", 1)[0].split()
            if not line:
                continue
            bank, off, pc, frame = int(line[0], 16), int(line[1], 16), int(line[2], 16), int(line[3])
            key = bank * 0x2000 + off
            if key not in reads or frame < reads[key][1]:
                reads[key] = (pc, frame, os.path.basename(p))
    return reads


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("reads", nargs="+")
    ap.add_argument("--rom", help="the dump: check against exactly what the extractor leaves out")
    a = ap.parse_args()
    if a.rom:
        import assets_nes
        mask = assets_runtime.left_out(assets_nes.Rom(a.rom))
    else:
        mask = assets_runtime.code_mask()
        for bank, off, n in assets_runtime.read_ranges(assets_runtime.CODE_READ_LIST):
            for i in range(n):
                mask[bank * 0x2000 + off + i] = 0
    reads = load_reads(a.reads)
    bad = sorted(k for k in reads if mask[k])
    print(f"{len(reads)} PRG bytes read in {len(a.reads)} runs; {len(bad)} of them left out of prg.bin")
    for k in bad:
        pc, frame, src = reads[k]
        print(f"  bank {k >> 13:02X} offset {k & 0x1FFF:04X}: first read in the run at ${pc:04X}, "
              f"frame {frame} ({src})")
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
