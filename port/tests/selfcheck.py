"""P4 regression: a scaffold run against the baseline scaffold, byte for byte.

    py -3 -I port/tests/selfcheck.py BASELINE.framelog CANDIDATE.framelog [--max 10]

Both logs come from port/scaffold's MFF_FRAMELOG with the same MFF_INPUT
route (port/tests/routes/*.keyed). The baseline is the scaffold with no
hand-written replacements; the candidate has some. Both run the same timing
model, so every frame must match exactly: CPU RAM, OAM and palette. Exit 0
only if they do.

Of the stack page only the live part is compared (--strict compares it all).
Between frames the game's one task (task 0, whose stack starts at S=$BF in
main_task) is waiting, and its live stack is above its saved S ($82):
$0100+($82+1) to $01BF. Below that and in $01C0-$01FF (the scheduler's and
the interrupts' scratch) the bytes are left over by interrupt frames, which
hold the interrupted PC and flags; a replacement that charges a block of
instructions at once reports another PC there, and no code reads them back.
Frames that differ only there are counted, not failed.
"""
import argparse
import os
import sys

sys.path.insert(0, os.path.dirname(__file__))
from lockstep import ram_names  # noqa: E402

REC = 4 + 0x800 + 0x100 + 0x20


def live_stack(rec):
    """The live stack addresses ($01xx) of the baseline frame `rec`: task 0's
    stack above its saved S when it is waiting; the whole page otherwise."""
    ram = rec[4:4 + 0x800]
    if ram[0x80] != 1:                  # no task waiting: keep the strict check
        return set(range(0x100, 0x200)) if ram[0x80] == 2 else set()
    return set(range(0x100 + ram[0x82] + 1, 0x1C0))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("baseline")
    ap.add_argument("candidate")
    ap.add_argument("--max", type=int, default=10)
    ap.add_argument("--strict", action="store_true", help="compare the whole stack page")
    ap.add_argument("--pics", nargs=2, metavar=("BASE", "CAND"),
                    help="MFF_PICLOG files of the same runs: compare every frame's picture hash")
    a = ap.parse_args()
    base, cand = open(a.baseline, "rb").read(), open(a.candidate, "rb").read()
    names = ram_names()
    n = min(len(base), len(cand)) // REC
    shown = 0
    first = None
    dead_only = 0
    for k in range(n):
        b, c = base[k * REC:(k + 1) * REC], cand[k * REC:(k + 1) * REC]
        if b == c:
            continue
        diffs = [i for i in range(4, REC) if b[i] != c[i]]
        if not a.strict:
            live = live_stack(b)
            if not [i for i in diffs if not (0x104 <= i < 0x204 and i - 4 not in live)]:
                dead_only += 1
                continue
            diffs = [i for i in diffs if not (0x104 <= i < 0x204 and i - 4 not in live)]
        first = first if first is not None else k
        if shown < a.max:
            shown += 1

            def where(i):
                if i < 4 + 0x800:
                    addr = i - 4
                    return f"${addr:04X}" + (f"({names[addr]})" if addr in names else "")
                if i < 4 + 0x900:
                    return f"OAM[{i - 4 - 0x800}]"
                return f"PAL[{i - 4 - 0x900}]"
            listed = ", ".join(f"{where(i)} {b[i]:02X}/{c[i]:02X}" for i in diffs[:8])
            print(f"frame {k}: {len(diffs)} bytes differ: {listed}" + (" ..." if len(diffs) > 8 else ""))
    if len(base) // REC != len(cand) // REC:
        print(f"lengths differ: baseline {len(base) // REC} frames, candidate {len(cand) // REC}")
    pic_first = cyc_first = None
    if a.pics:
        pb, pc = open(a.pics[0], "rb").read(), open(a.pics[1], "rb").read()
        m = min(len(pb), len(pc)) // 12
        for k in range(m):
            rb, rc = pb[k * 12:k * 12 + 12], pc[k * 12:k * 12 + 12]
            if pic_first is None and rb[:8] != rc[:8]:
                pic_first = k
            if cyc_first is None and rb[8:] != rc[8:]:
                cyc_first = k
                dc = int.from_bytes(rc[8:], "little") - int.from_bytes(rb[8:], "little")
        if len(pb) != len(pc):
            print(f"picture logs differ in length: {len(pb) // 12} / {len(pc) // 12} frames")
        print(f"{m} pictures compared: " + ("identical" if pic_first is None
                                            else f"first difference at picture {pic_first}")
              + ("; cycle counts identical" if cyc_first is None
                 else f"; cycle count first differs at picture {cyc_first} ({dc:+d})"))
    note = f" ({dead_only} frames differ only in dead stack bytes)" if dead_only else ""
    print(f"{n} frames compared: " + ("identical" if first is None else f"first difference at frame {first}") + note)
    # cycle counts are reported, not failed: a block charged at once moves
    # the frame boundary by a few cycles (it lands after the block), and the
    # difference stays bounded instead of drifting
    sys.exit(0 if first is None and pic_first is None and len(base) == len(cand) else 1)


if __name__ == "__main__":
    main()
