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

With --pics it also compares every picture (MFF_PICLOG: frame, picture
hash, CPU cycle count when it was drawn). The scaffold takes the NMI only
at the top of the scheduler's scan ($FEBD, extras.c), so when it lands
follows the exact count of cycles spent: a routine that costs one cycle
more or less shows in the cycle counts even when no RAM byte does (the
game idles in that scan, so RAM cannot tell). The counts must keep the
offset they have at picture 0, which is a constant by design where the
two runners count reset differently (the port on its own runtime charges
reset's 7 cycles, port/src/runtime/main.c); any change later fails.

--nmi-cycles N: the candidate's runtime also charges N cycles per NMI that
the baseline does not (port/src/runtime/frame.c charges the interrupt
sequence's 7; run_routes.sh passes MFF_NMI_CYCLES, 7 for
MightyFinalFight.exe). Each picture then moves the offset by exactly N (N
odd: N - 1 or N + 1, as the OAM DMA's 513/514-cycle stall follows the
parity of the cycle it starts on), less 94 when that pushes the NMI to the
previous scan of the scheduler (94 cycles a scan). Anything else fails. A
cycle error then shows unless it hides in the parity's +-1 that frame; a
recurring one does not hide.
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
    ap.add_argument("--nmi-cycles", type=int, default=0, metavar="N",
                    help="cycles per NMI the candidate charges and the baseline does not (see above)")
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
    cyc_bad = 0
    pics_ok = True
    if a.pics:
        pb, pc = open(a.pics[0], "rb").read(), open(a.pics[1], "rb").read()
        m = min(len(pb), len(pc)) // 12
        nmi = a.nmi_cycles
        steps = {0} if nmi == 0 else {nmi + p - w for p in ((-1, 1) if nmi & 1 else (0,)) for w in (0, 94)}
        offset = prev = None
        for k in range(m):
            rb, rc = pb[k * 12:k * 12 + 12], pc[k * 12:k * 12 + 12]
            if pic_first is None and rb[:8] != rc[:8]:
                pic_first = k
            # the counts are the low 32 bits: wrap the difference
            dc = (int.from_bytes(rc[8:], "little") - int.from_bytes(rb[8:], "little") + 2**31) % 2**32 - 2**31
            if offset is None:
                offset = dc
            elif dc - prev not in steps:
                cyc_bad += 1
                if cyc_first is None:
                    cyc_first, cyc_first_d = k, dc - prev
            prev = dc
        pics_ok = pic_first is None and cyc_bad == 0
        if len(pb) != len(pc):
            print(f"picture logs differ in length: {len(pb) // 12} / {len(pc) // 12} frames")
        print(f"{m} pictures compared: " + ("identical" if pic_first is None
                                            else f"first difference at picture {pic_first}")
              + ((f"; cycle counts identical (offset {offset:+d} from picture 0)" if nmi == 0 else
                  f"; cycle counts as modelled (offset {offset:+d} at picture 0, {nmi} more a frame)")
                 if cyc_bad == 0 else
                 f"; cycle counts off ({offset:+d} at picture 0" + (f", {nmi} more a frame" if nmi else "")
                 + f") at {cyc_bad} pictures, first at picture {cyc_first} (moved {cyc_first_d:+d})"))
    note = f" ({dead_only} frames differ only in dead stack bytes)" if dead_only else ""
    print(f"{n} frames compared: " + ("identical" if first is None else f"first difference at frame {first}") + note)
    sys.exit(0 if first is None and pics_ok and len(base) == len(cand) else 1)


if __name__ == "__main__":
    main()
