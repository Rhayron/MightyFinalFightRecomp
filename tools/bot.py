"""Closed-loop coverage bot: plays from a save state in short segments.

    py -3 -I tools/bot.py --exe build_headless/Release/MightyFinalFightRecomp.exe \
        --rom "rom/Mighty Final Fight (USA).nes" --state states/X.cycstate \
        --out runs/bot_cody --frames 20000 [--seg 16] [--seed 1]

Each segment loads the last state, runs --seg frames with the input the policy
chose from the state's RAM, and saves a new state. The game is deterministic
and a loaded state continues exactly as the uninterrupted run (cyc_state.h),
so the segments concatenate into <out>/route.txt, an ordinary --input file
that replays the whole session from --state in one run (use that run, with
--miss-log, for coverage). Checkpoint states land in <out>/ every 600 frames.

RAM used (docs/ram_map.md): object slots at $0300+i (state, $80 = live),
$0320+i (id), $0360+i (x), $03C0+i (y); the player is slot 0, enemies use
slots $0C-$0F. $0030 game action ($09 continue screen), $003A health,
$003E lives.
"""
import argparse
import os
import random
import struct
import subprocess
import sys

sys.path.insert(0, os.path.dirname(__file__))
import state_ram  # noqa: E402

ENEMY_SLOTS = range(0x03, 0x10)


def read_state(path):
    data = open(path, "rb").read()
    frame = struct.unpack_from("<I", data, 28)[0]
    base, _ = state_ram.ram_span(data)
    return frame, data[base:base + 0x800]


def enemies(ram, banned=()):
    out = []
    for i in ENEMY_SLOTS:
        ident, y = ram[0x320 + i], ram[0x3C0 + i]
        # y 0 marks spawners/markers (id $94 seen at y 0 in round 1); fighters
        # stand on the ground band
        if (ram[0x300 + i] & 0x80 and y >= 0x50 and i not in banned
                and (0x20 <= ident <= 0x35 or 0x70 <= ident <= 0x9F)):
            out.append((ram[0x360 + i], y, ident, i))
    return out


STALL = 6        # segments with the player not moving while the bot walks
UNSTICK = 4      # segments of random walking after a stall
BAN = 40         # segments a target that caused a stall is ignored
LONG_FIGHT = 3000  # --god: frames an enemy may stay alive before its life is cut
DIALOG_AFTER = 1500  # frames without progress before the bot starts tapping A
SKIP_AFTER = 6000  # --god: frames without progress (round, area, exp) before $0030 = $0E,
                   # the game's own "skip stage" action (moves on to the next area)


def policy(ram, rng, tick, mem):
    """Return a list of (offset, buttons) for one segment.

    mem carries the stall detector between segments: the player's recent
    positions, a countdown of random moves and banned target slots."""
    if ram[0x30] in (0x07, 0x09):          # select / continue screen
        return [(0, "START"), (4, "-")]
    if not ram[0x300] & 0x80:              # no live player: cutscene, map, round end
        return [(0, "START" if tick % 4 == 0 else "A"), (4, "-")]
    px, py = ram[0x360], ram[0x3C0]
    mem["banned"] = {s: n - 1 for s, n in mem.get("banned", {}).items() if n > 1}
    hist = mem.setdefault("hist", [])
    hist.append((px, py, mem.get("walking", False), mem.get("target")))
    del hist[:-STALL]
    if mem.get("unstick", 0):
        mem["unstick"] -= 1
        mem["walking"] = False
        return [(0, rng.choice(["RIGHT", "LEFT", "UP", "DOWN", "UP+RIGHT", "DOWN+RIGHT", "RIGHT+A"]))]
    if len(hist) == STALL and all(h[2] for h in hist) and len({h[:2] for h in hist}) == 1:
        if hist[-1][3] is not None:
            mem["banned"][hist[-1][3]] = BAN
        mem["unstick"] = UNSTICK
        hist.clear()
        return [(0, "RIGHT+A")]
    foes = enemies(ram, mem["banned"])
    if not foes:
        vert = "UP+" if py > 0xB0 else "DOWN+" if py < 0x98 else ""
        mem["walking"], mem["target"] = True, None
        return [(0, vert + "RIGHT")]
    ex, ey, _, slot = min(foes, key=lambda e: abs(e[0] - px) + 2 * abs(e[1] - py))
    mem["target"] = slot
    side = -1 if px < ex else 1             # stand on our current side of it
    want = ex + side * 22
    dirs = []
    if abs(ey - py) > 3:
        dirs.append("DOWN" if ey > py else "UP")
    if abs(want - px) > 6:
        dirs.append("RIGHT" if want > px else "LEFT")
    mem["walking"] = bool(dirs)
    if dirs:
        return [(0, "+".join(dirs))]
    face = "RIGHT" if side < 0 else "LEFT"
    r = rng.random()
    if r < 0.08:
        return [(0, "A+B"), (6, "-")]       # special (costs health)
    if r < 0.18:
        return [(0, face + "+A"), (5, face + "+A+B"), (10, "-")]  # jump attack
    if r < 0.28:
        return [(0, face)]                  # walk in: grab / throw
    return [(0, face), (2, "B"), (5, "-"), (8, "B"), (11, "-"), (13, "B")]


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--exe", required=True)
    p.add_argument("--rom", required=True)
    p.add_argument("--state", required=True)
    p.add_argument("--out", required=True)
    p.add_argument("--frames", type=int, required=True)
    p.add_argument("--seg", type=int, default=16)
    p.add_argument("--seed", type=int, default=1)
    p.add_argument("--god", action="store_true",
                   help="refill health ($3A) and lives ($3E), and cut the life of enemies alive for "
                        "LONG_FIGHT frames, in the state before every segment; the route no "
                        "longer replays on its own, so segments write --miss-log themselves")
    a = p.parse_args()
    a.exe = os.path.abspath(a.exe)
    os.makedirs(a.out, exist_ok=True)
    rng = random.Random(a.seed)
    route = open(os.path.join(a.out, "route.txt"), "w", newline="\n")
    log = open(os.path.join(a.out, "progress.log"), "w", newline="\n")
    route.write(f"# tools/bot.py from {a.state}, seg {a.seg}, seed {a.seed}"
                + (" (--god: not replayable)" if a.god else "") + "\n")
    state = a.state
    start, ram = read_state(state)
    seg_in = os.path.join(a.out, "seg.txt")
    cur = os.path.join(a.out, "cur.cycstate")
    miss = os.path.join(a.out, "segments.miss")
    tick = 0
    mem = {}
    full_hp = ram[0x3A]
    frame = start
    while frame < start + a.frames:
        steps = policy(ram, rng, tick, mem)
        if frame - mem.get("mark_frame", frame) > DIALOG_AFTER and tick % 3 == 0:
            # boss dialogs ("FINAL FIGHT!") wait for a fresh A press
            steps = [(0, "A"), (4, "-")]
        if a.god:
            patch = {}
            if ram[0x300] & 0x80 and 0 < ram[0x3A] < full_hp:
                # coverage aid: only a live player's health, never a death in progress
                patch[0x3A] = full_hp
            if ram[0x300] & 0x80 and ram[0x3E] < 3:
                patch[0x3E] = 3     # pits kill outright; never reach game over
            # an enemy (a boss, mostly) still standing after LONG_FIGHT frames
            # has run its code plenty: drop its life ($04A0+slot) to finish it
            ages = mem.setdefault("ages", {})
            live = {(e[3], e[2]) for e in enemies(ram)}
            for key in list(ages):
                if key not in live:
                    del ages[key]
            for key in live:
                ages[key] = ages.get(key, 0) + a.seg
                if ages[key] > LONG_FIGHT and ram[0x4A0 + key[0]] > 1:
                    patch[0x4A0 + key[0]] = 1
            mark = (ram[0x61], ram[0x22], ram[0x6C], ram[0x6D])
            if mark != mem.get("mark"):
                mem["mark"], mem["mark_frame"] = mark, frame
            elif ram[0x300] & 0x80 and frame - mem["mark_frame"] > SKIP_AFTER:
                patch[0x30] = 0x0E
                mem["mark_frame"] = frame
                print(f"frame {frame}: no progress for {SKIP_AFTER} frames, skip stage", flush=True)
            if patch:
                data = bytearray(open(state, "rb").read())
                base, _ = state_ram.ram_span(data)
                for addr, val in patch.items():
                    data[base + addr] = val
                state = os.path.join(a.out, "god.cycstate")
                open(state, "wb").write(data)
        full_hp = max(full_hp, ram[0x3A] if ram[0x3A] < 0x80 else 0)
        # A state's header frame is the next frame to run (saved after F -> F+1).
        lines = [f"{frame + off} {btn}" for off, btn in steps]
        with open(seg_in, "w", newline="\n") as f:
            f.write("\n".join(lines) + "\n")
        end = frame + a.seg - 1
        nxt = os.path.join(a.out, "next.cycstate")
        cmd = [a.exe, a.rom, "--load-state", state, "--frames", str(end + 1),
               "--input", seg_in, "--save-state", f"{end}:{nxt}"]
        if a.god:
            cmd += ["--miss-log", miss]
        if (end + 1 - start) % 600 < a.seg:
            cmd += ["--screenshot", os.path.join(a.out, f"f{end + 1:06d}.png")]
        r = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True,
                           creationflags=getattr(subprocess, "CREATE_NO_WINDOW", 0))
        if r.returncode or not os.path.exists(nxt):
            sys.exit(f"segment at {frame} failed:\n{r.stdout[-2000:]}")
        os.replace(nxt, cur)
        state = cur
        route.write("\n".join(lines) + "\n")
        frame, ram = read_state(state)
        tick += 1
        if (frame - start) % 600 < a.seg:
            ck = os.path.join(a.out, f"f{frame:06d}.cycstate")
            with open(state, "rb") as s, open(ck, "wb") as d:
                d.write(s.read())
            info = (f"frame {frame} action {ram[0x30]:02X} hp {ram[0x3A]} lives {ram[0x3E]} "
                    f"61={ram[0x61]:02X} 22={ram[0x22]:02X} exp {ram[0x6C] | ram[0x6D] << 8} "
                    f"lvl {ram[0x6B]} foes {len(enemies(ram))} px {ram[0x360]} py {ram[0x3C0]}")
            print(info, flush=True)
            log.write(info + "\n")
            log.flush()
            route.flush()
    route.write(f"{frame} -\n")


if __name__ == "__main__":
    main()
