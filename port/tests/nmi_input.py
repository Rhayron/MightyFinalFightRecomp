"""Key a route's input by the game's NMI count, for the scaffold (MFF_INPUT).

    py -3 -I port/tests/nmi_input.py ROUTE.txt ORACLE.framelog OUT.txt

The cycle host applies a route by frame; the legacy runner counts frames
differently (frames with the NMI off, nested vblanks), so a fixed frame
offset drifts. This reads the oracle's frame log of the same route, gives each
oracle frame the key port/tests/lockstep.py pairs on - (NMI count since reset,
n-th frame showing it) - and writes `count rep buttons` lines with the buttons
the route held on that frame. port/scaffold/extras.c keeps the same key from
$92 and applies the buttons, so each side's joypad reads see the same pads.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(__file__))
from lockstep import FRAME_COUNTER, read_oracle  # noqa: E402
from route2script import NAMES, parse  # noqa: E402

BITS = {name: 0x80 >> i for i, name in enumerate(NAMES)}


def main():
    route, flog, out = sys.argv[1:4]
    steps = parse(route)
    rows = read_oracle(flog)
    start = next(k for k in range(len(rows) - 1)
                 if rows[k][1][FRAME_COUNTER] == 0 and rows[k + 1][1][FRAME_COUNTER] == 1)
    lines, count, last, seen, si, held = [], 0, 0, {}, 0, set()
    for frame, ram in rows[start:]:
        v = ram[FRAME_COUNTER]
        count += (v - last) & 0xFF
        last = v
        rep = seen.get(count, 0)
        seen[count] = rep + 1
        while si < len(steps) and steps[si][0] <= frame:
            held = steps[si][1]
            si += 1
        mask = sum(BITS[b] for b in held)
        lines.append(f"{count} {rep} {mask:02X}")
    # The oracle's frame-f RAM already holds the pads it read during f; the
    # scaffold reads what extras.c sets on its next pass. Each key therefore
    # carries the next oracle frame's buttons.
    keys = [line.rsplit(" ", 1)[0] for line in lines]
    masks = [line.rsplit(" ", 1)[1] for line in lines]
    lines = [f"{k} {m}" for k, m in zip(keys, masks[1:] + masks[-1:])]
    open(out, "w", newline="\n").write("\n".join(lines) + "\n")
    print(f"{len(lines)} keyed frames (oracle frames {rows[start][0]}-{rows[-1][0]}) -> {out}")


if __name__ == "__main__":
    main()
