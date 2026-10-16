"""P4 regression: a scaffold run against the baseline scaffold, byte for byte.

    py -3 -I port/tests/selfcheck.py BASELINE.framelog CANDIDATE.framelog [--max 10]

Both logs come from port/scaffold's MFF_FRAMELOG with the same MFF_INPUT
route (port/tests/routes/*.keyed). The baseline is the scaffold with no
hand-written replacements; the candidate has some. Both run the same timing
model, so every frame must match exactly: all 2 KiB of CPU RAM (stack page
included), OAM and palette. Exit 0 only if they do.
"""
import argparse
import os
import sys

sys.path.insert(0, os.path.dirname(__file__))
from lockstep import ram_names  # noqa: E402

REC = 4 + 0x800 + 0x100 + 0x20


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("baseline")
    ap.add_argument("candidate")
    ap.add_argument("--max", type=int, default=10)
    a = ap.parse_args()
    base, cand = open(a.baseline, "rb").read(), open(a.candidate, "rb").read()
    names = ram_names()
    n = min(len(base), len(cand)) // REC
    shown = 0
    first = None
    for k in range(n):
        b, c = base[k * REC:(k + 1) * REC], cand[k * REC:(k + 1) * REC]
        if b == c:
            continue
        first = first if first is not None else k
        if shown < a.max:
            shown += 1
            diffs = [i for i in range(4, REC) if b[i] != c[i]]

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
    print(f"{n} frames compared: " + ("identical" if first is None else f"first difference at frame {first}"))
    sys.exit(0 if first is None and len(base) == len(cand) else 1)


if __name__ == "__main__":
    main()
