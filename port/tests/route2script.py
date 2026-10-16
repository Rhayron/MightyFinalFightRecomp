"""Turn a cycle-host --input route into a legacy runner --script.

    py -3 -I port/tests/route2script.py ROUTE.txt OUT.txt [--offset 5] [--end FRAME] [--dump DIR]

The cycle host holds the buttons of a `FRAME BUTTONS` line from that frame
on; the legacy runner's script is WAIT/HOLD/RELEASE. The two runners count
frames from different points: --offset is oracle frame minus scaffold frame
for the same NMI (5 for this game: NMI 1 is oracle frame 6, scaffold frame 1;
port/tests/lockstep.py prints the pairs). --end adds a final WAIT and EXIT.
"""
import argparse

NAMES = ["A", "B", "SELECT", "START", "UP", "DOWN", "LEFT", "RIGHT"]


def parse(path):
    steps = []
    for line in open(path):
        line = line.split("#", 1)[0].strip()
        if not line or not line[0].isdigit():
            continue
        frame, _, rest = line.partition(" ")
        rest = rest.strip()
        if rest.startswith(("2:", "3:", "4:")):
            continue                                  # player 1 only (MFF is one player)
        if rest.startswith("1:"):
            rest = rest[2:]
        held = set() if rest in ("", "-") else {b for b in rest.upper().replace(" ", "+").split("+") if b in NAMES}
        steps.append((int(frame), held))
    return steps


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("route")
    ap.add_argument("out")
    ap.add_argument("--offset", type=int, default=6, help="oracle frame minus scaffold frame for presses")
    ap.add_argument("--release-offset", type=int, default=5)
    ap.add_argument("--end", type=int)
    a = ap.parse_args()
    # presses and releases reach the game on different sides of the legacy
    # runner's controller latch: measured with port/tests/lockstep.py on the
    # START presses of inputs/10_cody_poweron.txt (press 6, release 5)
    events, held = [], set()
    for frame, want in parse(a.route):
        for btn in held - want:
            events.append((frame - a.release_offset, 0, btn))
        for btn in want - held:
            events.append((frame - a.offset, 1, btn))
        held = want
    events.sort()
    out, now = [], 0
    for at, press, btn in events:
        if at > now:
            out.append(f"WAIT {at - now}")
            now = at
        out.append(f"{'HOLD' if press else 'RELEASE'} {btn}")
    if a.end is not None:
        if a.end - a.release_offset > now:
            out.append(f"WAIT {a.end - a.release_offset - now}")
        out.append("EXIT 0")
    open(a.out, "w", newline="\n").write("\n".join(out) + "\n")
    print(f"{len(out)} script lines -> {a.out}")


if __name__ == "__main__":
    main()
