"""The port's APU writes against the oracle's, frame by frame.

    py -3 -I port/tests/apu_writes.py ORACLE.cap PORT.log [--first F] [--last F] [--max N]

ORACLE.cap: the oracle's capture (port/tests/synth: mff_apu_capture with
MFF_APU_CAPTURE, --hash-out and --wav-out). PORT.log: the port's MFF_APU_LOG
of the same route (keyed input, port/tests/nmi_input.py). Both have lines
"F n T" (a frame starts at cycle T), "W T ADDR VV" and "R T VV".

The oracle's frame n + 1 starts at its n-th vblank, as the port's frame
boundary n does (the oracle counts the stretch from power-on as frame 0),
so frames pair by that index; a route that lags on the oracle parts there.
For each pair the writes must be the same registers and values in the same
order; then the report gives where they land: the offset of the frame's
start (oracle - port, cycles) and of each write within its frame.
"""
import argparse
import collections
import sys


def load(path, oracle):
    frames, writes = [], collections.defaultdict(list)
    cur = -1
    with open(path) as f:
        for line in f:
            p = line.split()
            if not p:
                continue
            if p[0] == "F":
                n, t = int(p[1]), int(p[2])
                if oracle:
                    n -= 1                      # the oracle's frame 0 is power-on to the first vblank
                frames.append((n, t))
                cur = n
            elif p[0] in ("W", "V") and int(p[2], 16) not in (0x4014, 0x4016):   # OAM DMA, pads
                writes[cur].append((int(p[1]), int(p[2], 16), int(p[3], 16)))
    return dict(frames), writes


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("oracle")
    ap.add_argument("port")
    ap.add_argument("--first", type=int, default=0, help="first frame (port boundary index)")
    ap.add_argument("--last", type=int, default=1 << 30)
    ap.add_argument("--max", type=int, default=10)
    a = ap.parse_args()
    of, ow = load(a.oracle, True)
    pf, pw = load(a.port, False)
    grid = collections.Counter()
    offs = collections.Counter()
    compared = same = writes = 0
    shown = 0
    first_bad = None
    for n in sorted(pf):
        if n < a.first or n > a.last or n not in of:
            continue
        grid[of[n] - pf[n]] += 1
        o, p = ow.get(n, []), pw.get(n, [])
        compared += 1
        if [(x[1], x[2]) for x in o] != [(x[1], x[2]) for x in p]:
            if first_bad is None:
                first_bad = n
            if shown < a.max:
                shown += 1
                print(f"frame {n}: writes differ: oracle {[(f'{x[1]:04X}', f'{x[2]:02X}') for x in o][:8]}"
                      f" port {[(f'{x[1]:04X}', f'{x[2]:02X}') for x in p][:8]}")
            continue
        same += 1
        for (to, _, _), (tp, _, _) in zip(o, p):
            offs[(to - of[n]) - (tp - pf[n])] += 1
            writes += 1
    print(f"{compared} frames paired, {same} with the same writes ({writes} writes)"
          + (f"; first difference at frame {first_bad}" if first_bad is not None else ""))
    if grid:
        print("frame start, oracle - port (cycles: frames):",
              ", ".join(f"{k:+d}:{v}" for k, v in sorted(grid.items(), key=lambda kv: -kv[1])[:12]))
    if offs:
        total = sum(offs.values())
        mean = sum(k * v for k, v in offs.items()) / total
        ordered = sorted(offs.elements())
        print(f"write within its frame, oracle - port (cycles): mean {mean:+.1f}, median {ordered[total // 2]:+d}, "
              f"min {ordered[0]:+d}, max {ordered[-1]:+d}")
        print("  most common:", ", ".join(f"{k:+d}:{v}" for k, v in sorted(offs.items(), key=lambda kv: -kv[1])[:12]))
    sys.exit(0 if compared and same == compared else 1)


if __name__ == "__main__":
    main()
