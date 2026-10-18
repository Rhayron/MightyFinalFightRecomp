"""Stage 2's picture against stage 1's (PLAN_DECOMP.md P9, port/src/runtime/render_hd.h).

    py -3 -I port/tests/hd_check.py EXE OUTDIR [ROUTE ...] [--jobs N] [--max-frames N]

Plays each route (port/tests/routes/NAME.keyed or NAME.bot, or a path to a
.keyed file, e.g. one made by nmi_input.py from a power-on route of inputs/)
headless with MFF_HDCHECK: every picture is drawn twice, by render_frame.c
(the console picture) and by render_hd.c with every option off, and
compared pixel for pixel. Prints, per route, how many pictures were
compared and identical, where the playfield came from (the stage map, or
why not) and the sprites (the objects through the objects_draw model, or
OAM), and for every picture that differs the class it falls in; exits 1 if
any differs. With no ROUTE, all of port/tests/routes.

Sources in the log (render_hd.c, hd_init):
  playfield  map      the stage map through the column writer's state
             screen   not gameplay (title, story, select, map, continue, ending)
             banks    the background banks are not the area's (between areas)
             drawing  the area's first columns are being drawn
             step     the scroll and the writer's columns disagree (area exits)
             off      background off
  sprites    model    objects_draw's model built the very OAM page shown
             oam      the page shown is not the model's (a screen wrote OAM itself)
             none     objects_draw has not run yet
The log also counts, per picture, the (line, sprite) pairs the console's 8
sprites per line leave out and the sprites objects_draw could not place in a
full OAM: what --no-flicker shows.
"""
import argparse
import collections
import os
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

HERE = os.path.dirname(os.path.abspath(__file__))
ROUTES = os.path.join(HERE, "routes")


def route_spec(name):
    if os.path.isfile(name):
        path = name
    else:
        path = next((os.path.join(ROUTES, name + e) for e in (".keyed", ".bot")
                     if os.path.isfile(os.path.join(ROUTES, name + e))), None)
        if not path:
            sys.exit(f"no route {name}")
    base = os.path.splitext(os.path.basename(path))[0]
    if path.endswith(".bot"):
        bot, frames = open(path).read().split()[:2]
        return base, {"MFF_BOT": bot}, int(frames)
    with open(path) as f:
        frames = sum(1 for _ in f) + 300
    return base, {"MFF_INPUT": os.path.abspath(path)}, frames


def run(exe, outdir, spec, max_frames):
    name, env_add, frames = spec
    if max_frames:
        frames = min(frames, max_frames)
    log = os.path.join(outdir, name + ".hd.txt")
    env = dict(os.environ, SDL_VIDEODRIVER="dummy", SDL_AUDIODRIVER="dummy", MFF_HDCHECK=log, **env_add)
    flags = 0x08000000 if os.name == "nt" else 0          # CREATE_NO_WINDOW
    r = subprocess.run([os.path.abspath(exe), "--smoke", str(frames)], env=env, capture_output=True, text=True,
                       creationflags=flags)
    return name, log, r.returncode, r.stderr


def summarize(name, log):
    rows = []
    with open(log) as f:
        for line in f:
            p = line.split()
            rows.append((int(p[0]), p[1], p[2], int(p[3]), int(p[4]), int(p[5]), int(p[6]), int(p[7]),
                         int(p[8]), int(p[9]), int(p[10]), int(p[11])))
    bg = collections.Counter(r[1] for r in rows)
    spr = collections.Counter(r[2] for r in rows)
    diff = [r for r in rows if r[4]]
    overlay = sum(r[7] for r in rows)
    print(f"== {name}: {len(rows)} pictures, {len(rows) - len(diff)} identical, {len(diff)} differ")
    print("   playfield: " + ", ".join(f"{k} {v}" for k, v in bg.most_common()))
    print("   sprites:   " + ", ".join(f"{k} {v}" for k, v in spr.most_common()))
    if overlay:
        print(f"   playfield cells written by the game's VRAM requests: {overlay}")
    over = [r for r in rows if r[10]]
    full = [r for r in rows if r[11]]
    print(f"   the 8 per line leave out {sum(r[10] for r in over)} (line, sprite) pairs in {len(over)} pictures; "
          f"{sum(r[11] for r in full)} sprites past a full OAM in {len(full)} (--no-flicker draws them)")
    classes = collections.defaultdict(list)
    for r in diff:
        classes[(r[1], r[2], r[8], r[9])].append(r)
    for (b, s, rnd, area), rs in sorted(classes.items(), key=lambda kv: -len(kv[1])):
        ex = ", ".join(f"{r[0]} ({r[4]} px from {r[5]},{r[6]})" for r in rs[:4])
        print(f"   DIFFER playfield {b}, sprites {s}, round {rnd} area {area}: {len(rs)} pictures, e.g. {ex}")
    return len(rows), len(diff), bg, spr, len(over), len(full)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("exe")
    ap.add_argument("outdir")
    ap.add_argument("routes", nargs="*")
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 4)
    ap.add_argument("--max-frames", type=int, default=0)
    a = ap.parse_args()
    os.makedirs(a.outdir, exist_ok=True)
    names = a.routes or sorted(os.path.splitext(n)[0] for n in os.listdir(ROUTES)
                               if n.endswith((".keyed", ".bot")))
    specs = [route_spec(n) for n in names]
    with ThreadPoolExecutor(a.jobs) as pool:
        results = list(pool.map(lambda s: run(a.exe, a.outdir, s, a.max_frames), specs))
    total = differ = over_all = full_all = 0
    bg_all, spr_all = collections.Counter(), collections.Counter()
    status = 0
    for name, log, code, err in results:
        if code != 0 or not os.path.isfile(log):
            print(f"== {name}: exit {code}\n{err[-2000:]}")
            status = 1
            continue
        n, d, bg, spr, o, fu = summarize(name, log)
        over_all += o
        full_all += fu
        total += n
        differ += d
        bg_all.update(bg)
        spr_all.update(spr)
    print(f"== all: {total} pictures, {total - differ} identical, {differ} differ; playfield from the map "
          f"{bg_all['map']}, sprites from the objects {spr_all['model']}; pictures where the console leaves "
          f"sprites out: {over_all} (8 per line), {full_all} (full OAM)")
    if differ:
        status = 1
    sys.exit(status)


if __name__ == "__main__":
    main()
