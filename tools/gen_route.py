"""Generate a deterministic --input route for coverage runs.

    py -3 -I tools/gen_route.py --char cody|guy|haggar --seed N --start F --frames N
                                [--select] [--no-pause] > inputs/route.txt

--select prepends the title -> SELECT PLAYER -> pick route used from
states/title_4500.cycstate (START at the title, DOWN per slot, START), so the
mash starts at --start. Without it the route mashes from --start on (for runs
that load a state already in a round).

The mash walks mostly right, mixing punches (B), jumps (A), jump attacks,
grabs (walk into an enemy holding a direction), the A+B special and the odd
pause, in 4-24 frame steps. It does not try to win; it exercises code.
"""
import argparse
import random
import sys

# (buttons, weight). B is attack, A is jump on the NES port.
MOVES = [
    ("RIGHT", 10), ("RIGHT+B", 6), ("B", 8), ("A", 3), ("RIGHT+A", 4),
    ("A+B", 2), ("LEFT", 2), ("LEFT+B", 1), ("UP", 2), ("DOWN", 2),
    ("UP+RIGHT", 2), ("DOWN+RIGHT", 2), ("UP+B", 1), ("DOWN+B", 1),
    ("RIGHT+A+B", 1), ("-", 3),
]
SLOTS = {"cody": 0, "guy": 1, "haggar": 2}


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--char", choices=SLOTS, default="cody")
    p.add_argument("--seed", type=int, default=1)
    p.add_argument("--start", type=int, required=True)
    p.add_argument("--frames", type=int, required=True)
    p.add_argument("--select", action="store_true")
    p.add_argument("--no-pause", action="store_true")
    a = p.parse_args()
    rng = random.Random(a.seed)
    out = [f"# gen_route.py --char {a.char} --seed {a.seed} --start {a.start} --frames {a.frames}"
           + (" --select" if a.select else "")]
    f = a.start
    if a.select:
        f = 4520
        out += [f"{f} START", f"{f + 8} -"]
        f = 4600
        for _ in range(SLOTS[a.char]):
            out += [f"{f} DOWN", f"{f + 6} -"]
            f += 16
        out += [f"{f} START", f"{f + 8} -"]
        f = max(f + 20, a.start)
    end = f + a.frames
    names, weights = zip(*MOVES)
    while f < end:
        if not a.no_pause and rng.random() < 0.004:
            out += [f"{f} START", f"{f + 6} -", f"{f + 40} START", f"{f + 46} -"]
            f += 50
            continue
        move = rng.choices(names, weights)[0]
        hold = rng.randint(4, 24)
        if move in ("B", "RIGHT+B", "LEFT+B") and rng.random() < 0.6:
            # tap combos: the game wants presses, not a held button
            for i in range(rng.randint(2, 5)):
                out += [f"{f} {move}", f"{f + 3} {move.replace('+B', '') if '+' in move else '-'}"]
                f += 7
            continue
        out.append(f"{f} {move}")
        f += hold
    out.append(f"{end} -")
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
