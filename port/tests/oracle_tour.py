"""A tour of the game on the oracle and the port in lockstep, for the pictures of every round.

    py -3 -I port/tests/oracle_tour.py ROM ROUTE.txt OUTDIR [--start 5000] [--seg 300]
        [--skip-every 4] [--frames 30000] [--oracle EXE] [--port EXE]

A power-on route only reaches round 1. This plays a route on the oracle up to
--start, then walks right punching (RIGHT, with A every other 4 frames: it
also answers the boss dialogs, which wait for a fresh A) in segments of
--seg frames, saving a state at the end of each, and patches RAM in it
(tools/state_ram.py) the way tools/bot.py --god does: health and lives
refilled, the game's own skip stage ($0030 = $0E) every --skip-every
segments while the player is alive and not already skipping, and in the
final area (round 4, area 2+, where skipping runs past the end) every
fighter's life set below 0. The same writes go to the port through
MFF_POKE, keyed by the NMI count of the frame the state was saved after,
and the same pad through MFF_INPUT, so the two games stay together: on the
port's default timing (the console's, docs/timing.md) through the lag
frames too, which lag.txt counts.

OUTDIR gets tour_input.txt (the oracle's --input), o.flog (the oracle's
frame log, segments joined), tour.keyed
(MFF_INPUT), tour.poke (MFF_POKE), p.flog and p.pix (the port's MFF_FRAMELOG
and MFF_PICDUMP) and the reports of port/tests/oracle_pictures.py
(pictures.txt), port/tests/lockstep.py (lockstep.txt) and
port/tests/lag_check.py (lag.txt: lag frames and identical-RAM spans, from
o.orec and p.prec, the logs cut to frame, cycles and RAM). The frame logs
take ~135 KB (oracle) and ~63 KB (port) a frame: delete them after.
--lag-only keeps only o.orec and p.prec and writes only lag.txt.
--port-arg passes an argument to the port (e.g. --port-arg=--slowdown
--port-arg=off); the port plays with its default timing, the console's
(docs/timing.md), so it lags where the oracle does.
"""
import argparse
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.normpath(os.path.join(HERE, "..", ".."))
sys.path.insert(0, HERE)
import lag_check  # noqa: E402

STATE_RAM = os.path.join(ROOT, "tools", "state_ram.py")
HIDDEN = 0x08000000 if os.name == "nt" else 0        # CREATE_NO_WINDOW


def run(cmd, **kw):
    env = dict(os.environ, SDL_VIDEODRIVER="dummy", SDL_AUDIODRIVER="dummy")
    r = subprocess.run(cmd, env=env, capture_output=True, text=True, creationflags=HIDDEN, **kw)
    if r.returncode:
        sys.exit(f"{cmd[0]} failed ({r.returncode}):\n{r.stdout[-2000:]}\n{r.stderr[-2000:]}")
    return r.stdout


def state_ram(state):
    """The 2 KiB of CPU RAM in an oracle save state (tools/state_ram.py's layout)."""
    sys.path.insert(0, os.path.join(ROOT, "tools"))
    from state_ram import ram_span
    data = open(state, "rb").read()
    base, _ = ram_span(data)
    return data[base:base + 0x800]


def is_foe(ram, i):
    """port/scaffold/bot.c is_foe: a live fighter (it has a maximum life)."""
    ident, y = ram[0x320 + i], ram[0x3C0 + i]
    return bool(ram[0x300 + i] & 0x80) and y >= 0x50 and ram[0x4A0 + i] != 0 and \
        (0x20 <= ident <= 0x35 or 0x70 <= ident <= 0x9F)


def decide(ram, seg_index, a, full_hp):
    """The RAM writes for the state after a segment: {addr: value}."""
    w = {}
    alive = bool(ram[0x300] & 0x80)
    in_game = ram[0xAF] or ram[0x3E]
    if not in_game:
        return w
    if alive and 0 < ram[0x3A] < full_hp:
        w[0x3A] = full_hp
    if ram[0x3E] < 3 and ram[0x30] != 0x09:
        w[0x3E] = 3
    final_area = ram[0x61] == 4 and ram[0x60] >= 2
    if final_area:
        for i in range(3, 16):
            if is_foe(ram, i) and 0 < ram[0x490 + i] < 0x80:
                w[0x490 + i] = 0xFF             # below 0: the game's own death test (bot.c)
    elif alive and ram[0x30] not in (0x0E, 0x09) and a.skip_every and \
            seg_index % a.skip_every == a.skip_every - 1:
        w[0x30] = 0x0E
    return w


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("rom")
    ap.add_argument("route")
    ap.add_argument("out")
    ap.add_argument("--start", type=int, default=5000, help="oracle frame of the first patch")
    ap.add_argument("--seg", type=int, default=300)
    ap.add_argument("--skip-every", type=int, default=4, help="segments per area (0: never skip)")
    ap.add_argument("--frames", type=int, default=30000)
    ap.add_argument("--oracle", default=os.path.join(ROOT, "build_headless", "Release", "MightyFinalFightRecomp.exe"))
    ap.add_argument("--port", default=os.path.join(ROOT, "build_mff", "Release", "MightyFinalFight.exe"))
    ap.add_argument("--port-arg", action="append", default=[], help="an argument for the port (repeatable)")
    ap.add_argument("--lag-only", action="store_true", help="no pictures: keep only the RAM records")
    a = ap.parse_args()
    a.oracle, a.port, a.out = os.path.abspath(a.oracle), os.path.abspath(a.port), os.path.abspath(a.out)
    os.makedirs(a.out, exist_ok=True)
    o = lambda name: os.path.join(a.out, name)
    sys.path.insert(0, HERE)
    from route2script import parse
    with open(o("tour_input.txt"), "w") as f:
        for frame, held in parse(a.route):
            if frame < a.start:
                f.write(f"{frame} {'+'.join(sorted(held)) or '-'}\n")
        for frame in range(a.start, a.frames + 8, 8):
            f.write(f"{frame} RIGHT\n{frame + 4} RIGHT+A\n")
    a.route = o("tour_input.txt")
    joined = None if a.lag_only else open(o("o.flog"), "wb")
    open(o("o.orec"), "wb").close()
    patches = []                                    # (oracle frame, {addr: value})
    full_hp = 0

    def segment(load, first, last, part):
        cmd = [a.oracle, a.rom, "--frames", str(last + 1), "--input", a.route,
               "--frame-log", o(part), "--save-state", f"{last}:{o('seg.state')}"]
        if load:
            cmd += ["--load-state", load]
        run(cmd)
        lag_check.reduce_oracle(o(part), o("o.orec"), append=True)
        if joined:
            data = open(o(part), "rb").read()
            joined.write(data if first == 0 else data[12:])
        os.remove(o(part))

    segment(None, 0, a.start, "part.flog")
    frame, k = a.start, 0
    while True:
        ram = state_ram(o("seg.state"))
        if ram[0x300] & 0x80 and ram[0x3A] < 0x80:
            full_hp = max(full_hp, ram[0x3A])
        w = decide(ram, k, a, full_hp)
        if w:
            args = [f"{addr:04X}={v:02X}" for addr, v in sorted(w.items())]
            run([sys.executable, "-I", STATE_RAM, o("seg.state"), "set", *args, "-o", o("seg.state")])
            patches.append((frame, w))
        print(f"frame {frame}: action {ram[0x30]:02X} round {ram[0x61]} area {ram[0x60]} hp {ram[0x3A]} "
              f"lives {ram[0x3E]} writes {' '.join(f'{x:04X}={v:02X}' for x, v in sorted(w.items())) or '-'}",
              flush=True)
        if frame + a.seg >= a.frames:
            break
        os.replace(o("seg.state"), o("load.state"))
        segment(o("load.state"), frame + 1, frame + a.seg, "part.flog")
        frame += a.seg
        k += 1
    if joined:
        joined.close()
    for f in ("seg.state", "load.state"):
        if os.path.exists(o(f)):
            os.remove(o(f))

    orows = lag_check.load(o("o.orec"))
    keys = {frame: key for key, frame, _c, _r in lag_check.keyed(orows)}
    with open(o("tour.poke"), "w") as f:
        for fr, w in patches:
            count, rep = keys[fr]
            for addr, v in sorted(w.items()):
                f.write(f"{count} {rep} {addr:04X} {v:02X}\n")
    lag_check.keyed_input(a.route, orows, o("tour.keyed"))
    env = dict(MFF_INPUT=o("tour.keyed"), MFF_POKE=o("tour.poke"), MFF_FRAMELOG=o("p.flog"))
    if not a.lag_only:
        env["MFF_PICDUMP"] = o("p.pix")
    os.environ.update(env)
    run([a.port, "--smoke", str(a.frames + 100), *a.port_arg])
    lag_check.reduce_port(o("p.flog"), o("p.prec"))
    lines, _ = lag_check.compare(orows, lag_check.load(o("p.prec")), name=os.path.basename(a.out), listing=40)
    open(o("lag.txt"), "w").write("\n".join(lines) + "\n")
    print("== lag.txt\n" + "\n".join(lines[:4]))
    if a.lag_only:
        os.remove(o("p.flog"))
        return
    for name, cmd in (("pictures.txt", [os.path.join(HERE, "oracle_pictures.py"), o("o.flog"), o("p.flog"),
                                        o("p.pix"), "--max", "40"]),
                      ("lockstep.txt", [os.path.join(HERE, "lockstep.py"), o("o.flog"), o("p.flog"), "--max", "5"])):
        r = subprocess.run([sys.executable, "-I", *cmd], capture_output=True, text=True, creationflags=HIDDEN)
        open(o(name), "w").write(r.stdout + r.stderr)
        print(f"== {name}\n" + "\n".join((r.stdout + r.stderr).splitlines()[-6:]))


if __name__ == "__main__":
    main()
