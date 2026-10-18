"""Lag frames of the port against the oracle's (PLAN_DECOMP.md P7).

    py -3 -I port/tests/lag_check.py ROUTE.txt [ROUTE.txt ...] [--out DIR] [--frames N]
        [--port EXE] [--oracle EXE] [--rom ROM] [--env NAME=VALUE ...] [--list N]

For each route (inputs/*.txt, from power-on) the oracle plays it once with
--frame-log, and the log is cut down to what this needs (DIR/NAME.orec:
per frame u32 frame, u64 cycles, the 2 KiB of CPU RAM; the full log is
deleted); its pad goes to the port keyed by NMI count (as nmi_input.py
keys it, DIR/NAME.keyed). Both are kept and reused while the route stays the
same and holds enough frames. The port then plays the keyed route (MFF_INPUT, MFF_FRAMELOG,
the --env settings) and its log is cut down the same way (DIR/NAME.prec).

A frame is a lag frame when its record, taken where the NMI lands, has task
0 still running (tasks $80 = 2): the NMI interrupts the frame's work, which
finishes in the next frame (docs/timing.md). Frames pair by the game's NMI
count (lockstep.py). Reported per route:

  - lag agreement: paired frames whose lag flag is the same on both sides;
  - the first frame whose game RAM differs (--ignore ranges aside), and how
    many frames before it differ in the sound driver's RAM and in
    nmi_return_pc ($A0/$A1: where the NMI landed, the finest check of the
    clock);
  - each oracle lag, whether the port lagged at the same NMI, whether the
    lag frame's own record is identical and for how many frames after it
    the game RAM stays identical;
  - the port's lags the oracle does not have.

oracle_tour.py writes the same report for a tour (lag.txt), with compare().
Exit status 1 if agreement is below --target (default 99%).
"""
import argparse
import os
import struct
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.normpath(os.path.join(HERE, "..", ".."))
sys.path.insert(0, HERE)

RAM = 0x800
FRAME_COUNTER = 0x92
TASK0_STATE = 0x80
OREC = 4 + 8 + RAM
HIDDEN = 0x08000000 if os.name == "nt" else 0        # CREATE_NO_WINDOW
# the stack page and each task's saved stack pointer: where the runtime's
# frames sit differs from the 6502's in dead bytes only (lockstep.py)
DEFAULT_IGNORE = "0100-01FF,0082,0086,008A,008E"
SOUND = "00C0-00D8,0700-077F"
RETURN_PC = (0xA0, 0xA1)                            # nmi_return_pc


def ranges(text):
    out = set()
    for part in filter(None, text.split(",")):
        lo, _, hi = part.partition("-")
        out.update(range(int(lo, 16), int(hi or lo, 16) + 1))
    return out


def run(cmd, env=None):
    e = dict(os.environ, SDL_VIDEODRIVER="dummy", SDL_AUDIODRIVER="dummy")
    e.update(env or {})
    r = subprocess.run(cmd, env=e, capture_output=True, text=True, creationflags=HIDDEN)
    if r.returncode:
        sys.exit(f"{cmd[0]} failed ({r.returncode}):\n{r.stdout[-2000:]}\n{r.stderr[-2000:]}")
    return r.stdout + r.stderr


def reduce_oracle(flog, out, append=False):
    """The oracle's --frame-log (lockstep.read_oracle's format) cut to frame, cycles, RAM."""
    with open(flog, "rb") as f, open(out, "ab" if append else "wb") as g:
        if f.read(12)[:8] != b"CYCFRAME":
            sys.exit(f"{flog}: not a cycle frame log")
        while True:
            head = f.read(36)
            if len(head) < 36:
                break
            n = struct.unpack("<9I", head)
            f.seek(32 + 256, 1)
            ram = f.read(n[3])
            if len(ram) < n[3]:
                break
            f.seek(sum(n[4:9]), 1)
            g.write(struct.pack("<IQ", n[0], n[1] | (n[2] << 32)) + ram)


def reduce_port(flog, out):
    """The port's MFF_FRAMELOG (u32 frame, RAM, OAM, palette) cut to the same records (no cycles)."""
    rec = 4 + RAM + 0x100 + 0x20
    with open(flog, "rb") as f, open(out, "wb") as g:
        while True:
            r = f.read(rec)
            if len(r) < rec:
                break
            g.write(r[:4] + bytes(8) + r[4:4 + RAM])


def load(path):
    data = open(path, "rb").read()
    return [(struct.unpack_from("<I", data, o)[0], struct.unpack_from("<Q", data, o + 4)[0],
             data[o + 12:o + OREC]) for o in range(0, len(data) - OREC + 1, OREC)]


def keyed(rows):
    """[(count, rep), frame, cycles, ram]: frames keyed by NMI count since reset
    (lockstep.pair), each key's first frame only."""
    start = next((k for k in range(len(rows) - 1)
                  if rows[k][2][FRAME_COUNTER] == 0 and rows[k + 1][2][FRAME_COUNTER] == 1), None)
    if start is None:
        return []
    out, count, last, seen = [], 0, 0, {}
    for frame, cyc, ram in rows[start:]:
        v = ram[FRAME_COUNTER]
        count += (v - last) & 0xFF
        last = v
        rep = seen.get(count, 0)
        seen[count] = rep + 1
        out.append(((count, rep), frame, cyc, ram))
    return out


def is_lag(ram):
    return ram[TASK0_STATE] == 2


def keyed_input(route, rows, out):
    """nmi_input.py from the reduced records: `count rep buttons` per oracle
    frame, each key carrying the next oracle frame's buttons."""
    from route2script import NAMES, parse
    bits = {nm: 0x80 >> i for i, nm in enumerate(NAMES)}
    steps = parse(route)
    keys, masks, si, held = [], [], 0, set()
    for (count, rep), frame, _cyc, _ram in keyed(rows):
        while si < len(steps) and steps[si][0] <= frame:
            held = steps[si][1]
            si += 1
        keys.append(f"{count} {rep}")
        masks.append(f"{sum(bits[b] for b in held):02X}")
    lines = [f"{k} {m}" for k, m in zip(keys, masks[1:] + masks[-1:])]
    open(out, "w", newline="\n").write("\n".join(lines) + "\n")


def kept_slices(ign):
    out, lo = [], None
    for a in range(RAM + 1):
        keep = a < RAM and a not in ign
        if keep and lo is None:
            lo = a
        elif not keep and lo is not None:
            out.append((lo, a))
            lo = None
    return out


def compare(orows, prows, ignore=DEFAULT_IGNORE, sound=SOUND, listing=20, name=""):
    """Lag agreement and identical-RAM spans; returns (lines, stats)."""
    ign, snd = ranges(ignore), ranges(sound)
    slices = kept_slices(ign)
    ok = {k: (f, c, r) for k, f, c, r in keyed(orows)}
    pk = [(k, f, c, r) for k, f, c, r in keyed(prows) if k in ok]
    lines = []
    if not pk:
        return [f"{name}: no frames paired"], dict(paired=0, agree=0, olag=0, plag=0)
    # first difference from each frame on: walk backwards
    n = len(pk)
    game_diff = [None] * n                  # bytes that differ at pair i
    for i, (k, f, c, r) in enumerate(pk):
        orr = ok[k][2]
        d = []
        for lo, hi in slices:
            if orr[lo:hi] != r[lo:hi]:
                d.extend(a for a in range(lo, hi) if orr[a] != r[a])
        game_diff[i] = d
    def next_where(pred):
        nxt = [n] * (n + 1)
        for i in range(n - 1, -1, -1):
            nxt[i] = i if pred(game_diff[i]) else nxt[i + 1]
        return nxt
    # classes: game bytes; the sound driver's; nmi_return_pc (where the NMI
    # landed, which says how close the two clocks are)
    next_diff = next_where(bool)
    next_game = next_where(lambda d: any(a not in snd and a not in RETURN_PC for a in d))
    next_sound = next_where(lambda d: any(a in snd for a in d))
    next_ret = next_where(lambda d: any(a in RETURN_PC for a in d))
    agree = olag = plag = both = 0
    olist, ponly = [], []
    for i, (k, f, c, r) in enumerate(pk):
        lo, lp = is_lag(ok[k][2]), is_lag(r)
        agree += lo == lp
        olag += lo
        plag += lp
        both += lo and lp
        if lo:
            olist.append((i, lp))
        elif lp:
            ponly.append(i)
    first_game = next_game[0]
    differ = [i for i in range(n) if game_diff[i]]

    def at(i):
        if i >= n:
            return "the end"
        k = pk[i][0]
        return f"NMI {k[0]}{'+' + str(k[1]) if k[1] else ''} (oracle frame {ok[k][0]}, port frame {pk[i][1]})"

    def bytes_at(i, pick):
        k = pk[i][0]
        orr, prr = ok[k][2], pk[i][3]
        d = [a for a in game_diff[i] if pick(a)]
        return ", ".join(f"${a:04X} {orr[a]:02X}/{prr[a]:02X}" for a in d[:8]) + (" ..." if len(d) > 8 else "")

    is_game = lambda a: a not in snd and a not in RETURN_PC          # noqa: E731
    lines.append(f"{name}: {n} frames paired, oracle lags {olag}, port lags {plag}, both {both}; "
                 f"lag agreement {agree}/{n} = {100.0 * agree / n:.3f}%")
    lines.append(f"  game RAM identical until {at(first_game)}"
                 + (f": {bytes_at(first_game, is_game)}" if first_game < n else ""))
    for label, nxt, pick in (("sound driver RAM", next_sound, lambda a: a in snd),
                             ("nmi_return_pc", next_ret, lambda a: a in RETURN_PC)):
        cnt = sum(1 for i in differ if any(pick(a) for a in game_diff[i]) and i < first_game)
        lines.append(f"  {label}: {cnt} of the {first_game} frames before that differ"
                     + (f", the first at {at(nxt[0])}: {bytes_at(nxt[0], pick)}" if cnt else ""))
    spans = []
    for i, lp in olist:
        # the lag frame's own record is a snapshot of the work the NMI
        # interrupted: what counts is the frames after it
        span = next_game[i + 1] - (i + 1) if i + 1 < n else 0
        spans.append(span)
        if len(spans) <= listing:
            here = "identical" if not any(is_game(a) for a in game_diff[i]) else \
                f"differs ({bytes_at(i, is_game)})"
            lines.append(f"  oracle lag at {at(i)}: port {'lags too' if lp else 'does NOT lag'}; at the lag "
                         f"game RAM {here}; then identical for {span} frames"
                         + (" (to the end)" if i + 1 + span >= n else f", to {at(i + 1 + span)}"))
    if len(olist) > listing:
        lines.append(f"  ... {len(olist) - listing} more oracle lags")
    for i in ponly[:listing]:
        lines.append(f"  port-only lag at {at(i)}")
    if len(ponly) > listing:
        lines.append(f"  ... {len(ponly) - listing} more port-only lags")
    return lines, dict(paired=n, agree=agree, olag=olag, plag=plag, both=both,
                       first_diff=next_diff[0], first_game=first_game, spans=spans)


def oracle_side(a, route, name):
    orec, keyed_in = os.path.join(a.out, name + ".orec"), os.path.join(a.out, name + ".keyed")
    stamp = os.path.join(a.out, name + ".ostamp")
    src = f"{os.path.abspath(route)} {os.path.getmtime(route)}"
    if os.path.exists(orec) and os.path.exists(keyed_in) and os.path.exists(stamp):
        have = open(stamp).read().rsplit(" ", 1)          # "route mtime frames"
        if have[0] == src and int(have[1]) >= a.frames:
            return orec, keyed_in
    want = f"{src} {a.frames}"
    flog = os.path.join(a.out, name + ".o.flog")
    run([a.oracle, a.rom, "--frames", str(a.frames), "--input", route, "--frame-log", flog])
    reduce_oracle(flog, orec)
    os.remove(flog)
    keyed_input(route, load(orec), keyed_in)
    open(stamp, "w").write(want)
    return orec, keyed_in


def port_side(a, keyed_in, name):
    prec, flog = os.path.join(a.out, name + ".prec"), os.path.join(a.out, name + ".p.flog")
    env = dict(MFF_INPUT=keyed_in, MFF_FRAMELOG=flog)
    for kv in a.env:
        k, _, v = kv.partition("=")
        env[k] = v
    run([a.port, "--smoke", str(a.frames + 100)], env)
    reduce_port(flog, prec)
    os.remove(flog)
    return prec


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("routes", nargs="+")
    ap.add_argument("--out", default=os.path.join(ROOT, "runs", "p7"))
    ap.add_argument("--frames", type=int, default=0, help="frames to play (default: the route's last + 30)")
    ap.add_argument("--rom", default=os.path.join(ROOT, "rom", "Mighty Final Fight (USA).nes"))
    ap.add_argument("--oracle", default=os.path.join(ROOT, "build_headless", "Release", "MightyFinalFightRecomp.exe"))
    ap.add_argument("--port", default=os.path.join(ROOT, "build_mff", "Release", "MightyFinalFight.exe"))
    ap.add_argument("--env", action="append", default=[], help="NAME=VALUE for the port (repeatable)")
    ap.add_argument("--ignore", default=DEFAULT_IGNORE)
    ap.add_argument("--list", type=int, default=20)
    ap.add_argument("--target", type=float, default=99.0)
    a = ap.parse_args()
    a.out = os.path.abspath(a.out)
    os.makedirs(a.out, exist_ok=True)
    from route2script import parse
    total = dict(paired=0, agree=0, olag=0, plag=0, both=0)
    status = 0
    for route in a.routes:
        name = os.path.splitext(os.path.basename(route))[0]
        frames = a.frames or (max((f for f, _ in parse(route)), default=0) + 30)
        b = argparse.Namespace(**vars(a))
        b.frames = frames
        orec, keyed_in = oracle_side(b, route, name)
        prec = port_side(b, keyed_in, name)
        lines, st = compare(load(orec), load(prec), a.ignore, listing=a.list, name=name)
        print("\n".join(lines), flush=True)
        for k in total:
            total[k] += st.get(k, 0)
        if st["paired"] and 100.0 * st["agree"] / st["paired"] < a.target:
            status = 1
    if len(a.routes) > 1 and total["paired"]:
        print(f"all: {total['paired']} frames paired, oracle lags {total['olag']}, port lags {total['plag']}, "
              f"both {total['both']}; lag agreement {100.0 * total['agree'] / total['paired']:.3f}%")
    sys.exit(status)


if __name__ == "__main__":
    main()
