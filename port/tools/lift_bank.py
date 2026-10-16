"""Lift every routine of some 16 KiB banks into port/src/game/lifted_*.c.

    py -3 -I port/tools/lift_bank.py ROM BANK [BANK ...] [--skip ADDR,...]

For each bank (0-7; 7 is the fixed bank at $C000-$FFFF) it lifts, with
port/tools/lift.py, every entry point of the P0 map (port/scaffold/game.toml)
that has no hand-written replacement, writes them to
port/src/game/lifted_c.c .. lifted_f.c (the fixed bank, by 4 KiB) or
lifted_bN.c, and rewrites the lifted part of port/scaffold/game.base.toml
(the [[replace_func]] entries marked "# lifted") to match. Entries the
lifter refuses, and those in --skip, stay generated. Then run
port/tests/regen.sh and port/tests/run_routes.sh check.
"""
import argparse
import os
import re
import sys
import tomllib

ROOT = os.path.normpath(os.path.join(os.path.dirname(__file__), "..", ".."))
sys.path.insert(0, os.path.dirname(__file__))
import lift  # noqa: E402

BASE = os.path.join(ROOT, "port", "scaffold", "game.base.toml")
GAME = os.path.join(ROOT, "port", "src", "game")
LIFTED = re.compile(r"\[\[replace_func\]\]\n(?:bank = \d+\n)?addr = 0x([0-9A-F]{4})   # lifted[^\n]*\n")


def groups(bank):
    if bank == lift.FIXED:
        return [(f"lifted_{k}.c", lo, lo + 0x1000) for k, lo in
                (("c", 0xC000), ("d", 0xD000), ("e", 0xE000), ("f", 0xF000))]
    return [(f"lifted_b{bank}.c", 0x8000, 0xC000)]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("rom")
    ap.add_argument("banks", nargs="+", type=int)
    ap.add_argument("--skip", default="")
    a = ap.parse_args()
    skip = {int(s, 16) for s in a.skip.split(",") if s}
    rom = open(a.rom, "rb").read()
    game = tomllib.load(open(os.path.join(ROOT, "port", "scaffold", "game.toml"), "rb"))
    base = open(BASE).read()
    # what is hand-written: replace_func entries in game.base.toml not marked lifted
    plain = re.sub(LIFTED, "", base)
    hand = set()
    for m in re.finditer(r"\[\[replace_func\]\]\n(?:bank = (\d+)\n)?addr = 0x([0-9A-F]{4})", plain):
        hand.add((int(m.group(1)) if m.group(1) else lift.FIXED, int(m.group(2), 16)))
    gen = lift.Generated(os.path.join(ROOT, "runs", "p4", "pure", "generated"))
    names = lift.Names()
    keep = [m.group(0) for m in LIFTED.finditer(base)]
    entries = []
    for bank in a.banks:
        # drop the old lifted entries of this bank
        keep = [k for k in keep if not (("bank = %d\n" % bank) in k if bank != lift.FIXED
                                         else "bank = " not in k)]
        starts = sorted({f["addr"] for f in game["extra_func"] if f["bank"] == bank})
        lf = lift.Lifter(rom, bank, gen, names)
        for fname, lo, hi in groups(bank):
            chunks, done = [], []
            for addr in starts:
                if not lo <= addr < hi or (bank, addr) in hand or addr in skip:
                    continue
                try:
                    chunks.append(lf.lift(addr))
                    done.append(addr)
                except lift.Refuse as e:
                    print(f"bank {bank} ${addr:04X}: refused: {e}", file=sys.stderr)
            protos = "".join(f"void {f}(void);\n" for f in sorted(lf.calls))
            lf.calls = set()
            where = "fixed-bank" if bank == lift.FIXED else f"bank {bank}"
            head = (f"/*\n * {fname} - {where} routines ${lo:04X}-${hi - 1:04X}, lifted by\n"
                    f" * port/tools/lift_bank.py from the pure scaffold: exact, still to be\n"
                    f" * tidied and moved into their systems' files.\n */\n"
                    f"#include \"mff.h\"\n#include \"state.h\"\n#include \"coroutine.h\"\n\n")
            path = os.path.join(GAME, fname)
            if done:
                open(path, "w", newline="\n").write(head + protos + "\n" + "\n\n".join(chunks) + "\n")
            elif os.path.exists(path):
                os.remove(path)
            for addr in done:
                entries.append((f"bank = {bank}\n" if bank != lift.FIXED else "") +
                               f"addr = 0x{addr:04X}   # lifted, {fname}\n")
            print(f"bank {bank} {fname}: {len(done)} lifted", file=sys.stderr)
    body = re.sub(LIFTED, "", base).rstrip("\n") + "\n"
    block = "".join(keep) + "".join(f"[[replace_func]]\n{e}" for e in entries)
    open(BASE, "w", newline="\n").write(body + block)


if __name__ == "__main__":
    main()
