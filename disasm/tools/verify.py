"""Reassemble build/disasm with ca65/ld65 and require the PRG to match the dump byte for byte.

    py -3 -I disasm/tools/verify.py ROM [--dir build/disasm] [--cc65 build_cc65]

Exit 0 only when every bank reassembles identical; otherwise lists the first
differing offsets per bank.
"""
import argparse
import os
import subprocess
import sys

NOWIN = getattr(subprocess, "CREATE_NO_WINDOW", 0)


def run(cmd):
    r = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, creationflags=NOWIN)
    if r.returncode:
        sys.exit(f"{' '.join(cmd)} failed:\n{r.stdout[-3000:]}")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("rom")
    ap.add_argument("--dir", default="build/disasm")
    ap.add_argument("--cc65", default="build_cc65")
    a = ap.parse_args()
    rom = open(a.rom, "rb").read()
    prg = rom[16:16 + rom[4] * 0x4000]
    banks = len(prg) // 0x2000
    ca65, ld65 = (os.path.join(a.cc65, x + ".exe") for x in ("ca65", "ld65"))
    objs = []
    for b in range(banks):
        src = os.path.join(a.dir, f"bank{b:02X}.s")
        obj = os.path.join(a.dir, f"bank{b:02X}.o")
        run([ca65, "--cpu", "6502", "-o", obj, src])
        objs.append(obj)
    out = os.path.join(a.dir, "prg.bin")
    run([ld65, "-C", os.path.join(a.dir, "prg.cfg"), "-o", out] + objs)
    built = open(out, "rb").read()
    if len(built) != len(prg):
        sys.exit(f"built {len(built)} bytes, expected {len(prg)}")
    bad = 0
    for b in range(banks):
        lo, hi = b * 0x2000, (b + 1) * 0x2000
        diffs = [i for i in range(lo, hi) if built[i] != prg[i]]
        if diffs:
            bad += 1
            print(f"bank {b:02X}: {len(diffs)} bytes differ, first at offset ${diffs[0] - lo:04X}")
    os.remove(out)
    if bad:
        sys.exit(f"{bad} bank(s) differ")
    print(f"OK: {banks} banks reassemble byte for byte ({len(prg)} bytes)")


if __name__ == "__main__":
    main()
