"""synth_2a03 against the oracle, passage by passage.

    py -3 -I port/tests/synth/synth_check.py --oracle-root C:/path/to/mff [--build build_synth/Release]
        [--out runs/synth] [--only NAME,...] [--no-spectral] [--no-inject] [--recapture] [--jobs N]

For each passage a relinked oracle (CMakeLists.txt: mff_apu_capture for the
game, interp_apu_capture for the generated APU test program) plays a route
or a saved state with --wav-out and records every APU write and $4015 read
with its cycle; synth_replay feeds them to synth_2a03, starting from the
oracle's own APU state, and compares the two WAVs sample by sample over the
passage's windows, the $4015 reads, and on the "levels" passages the five
channel outputs on every cycle.

Then, for the case the port is in (writes without exact cycles: each frame's
burst at 1500 + 290 * i cycles, where sound_model.py and apu_render.py put
them), the game windows through sound_wavcmp.py, next to the floor of those
measures: the exact writes from oscillators started at random phases, against
the exact render.

Routes and states come from --oracle-root (rom/, inputs/, states/, runs/cdl/);
the injected passages queue each song and effect into a state the way
sound_oracle.py inject does. Everything written is ROM-derived: --out must
stay under runs/.
"""
import argparse
import concurrent.futures as cf
import os
import re
import struct
import subprocess
import sys
import wave

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.normpath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "port", "tools"))
import sound_oracle  # noqa: E402  (patch(): a command queued into a state)

NO_WINDOW = 0x08000000 if os.name == "nt" else 0
SPECTRAL_CAP = 30 * 48000          # sound_wavcmp is pure Python: at most 30 s per window
FRAME = 29780.5                    # CPU cycles per frame, for the levels windows

# name, input route, state, frames to run (from power-on or the state), windows (label, first, last frame)
GAME = [
    ("poweron", None, None, 7000,
     [("intro story ($07)", 2062, 4428), ("title ($0C)", 4429, 5120)]),
    ("cody", "inputs/10_cody_poweron.txt", None, 20000,
     [("select ($0D)", 4547, 4637), ("round 1 ($00), effects", 4778, 17664), ("continue ($0A)", 17685, 18809)]),
    ("guy", "inputs/10_guy_poweron.txt", None, 20000,
     [("round 1 ($00), effects (Guy)", 4761, 16464)]),
    ("r2", None, "states/rounds/cody_r2.cycstate", 5000, [("round 2 ($01)", 64269, 69268)]),
    ("r3", None, "states/rounds/cody_r3.cycstate", 5000, [("round 3 ($02), effects", 115869, 120868)]),
    ("r4", None, "states/rounds/cody_r4.cycstate", 5000, [("round 4 ($03), effects", 239869, 244868)]),
    ("r5", None, "states/rounds/cody_r5.cycstate", 5000, [("round 5 ($04)", 280160, 285076)]),
    ("boss_r2", None, "states/r2boss_guy_bot.cycstate", 4000, [("round 2 boss ($05)", 144861, 148860)]),
    ("boss_r5", None, "states/god5_cody_from.cycstate", 4000, [("final boss ($05)", 335869, 339868)]),
    ("ending", None, "runs/cdl/tour2_cody_21/f054669.cycstate", 3000,
     [("ending ($08)", 54669, 55588), ("credits ($03, $2B)", 55589, 57668)]),
]
SONGS = list(range(0x00, 0x10))
EFFECTS = list(range(0x12, 0x36))


class Passage:
    def __init__(self, name, frames, windows, route=None, state=None, rom=None, tool="mff", levels=None,
                 prg=False, spectral=False):
        self.name, self.frames, self.windows = name, frames, windows
        self.route, self.state, self.rom, self.tool = route, state, rom, tool
        self.levels, self.prg, self.spectral = levels, prg, spectral


def run(args, env=None, log=None):
    out = subprocess.run(args, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, env=env, creationflags=NO_WINDOW)
    text = out.stdout.decode("utf-8", "replace")
    if log:
        with open(log, "w") as f:
            f.write(text)
    return out.returncode, text


def state_frame(path):
    with open(path, "rb") as f:
        return struct.unpack_from("<I", f.read(32), 28)[0]


def capture(o, p):
    """The oracle's run: capture file, its WAV and frame hashes."""
    base = os.path.join(o.out, p.name)
    cap = base + ".cap"
    if os.path.exists(cap) and os.path.exists(base + "_oracle.wav") and not o.recapture:
        return
    first = state_frame(p.state) if p.state else 0
    exe = os.path.join(o.build, f"{p.tool}_apu_capture.exe")
    args = [exe, p.rom, "--frames", str(first + p.frames), "--hash-out", base + ".hash",
            "--wav-out", base + "_oracle.wav"]
    if p.route:
        args += ["--input", p.route]
    if p.state:
        args += ["--load-state", p.state]
    env = dict(os.environ, MFF_APU_CAPTURE=cap)
    if p.levels:
        env["MFF_APU_LEVELS"] = p.levels
    code, text = run(args, env=env, log=base + ".oracle.log")
    if code != 0:
        raise SystemExit(f"{p.name}: oracle failed ({code}):\n{text[-2000:]}")


SAMPLE_LINE = re.compile(r"^(all|frames (\d+)-(\d+)): (\d+) samples, (\d+) differ, max \|diff\| (\d+), "
                         r"RMS diff ([\d.]+), RMS ([\d.]+) / ([\d.]+), correlation ([-\d.]+)")


def replay(o, p, extra, tag):
    base = os.path.join(o.out, p.name)
    args = [os.path.join(o.build, "synth_replay.exe"), base + ".cap", f"{base}_{tag}.wav",
            "--frames-out", base + ".frames"]
    if p.prg:
        args += ["--prg", p.rom]
    if tag == "synth":
        args += ["--oracle", base + "_oracle.wav"]
        for _label, a, b in p.windows:
            args += ["--range", f"{a}:{b}"]
    _code, text = run(args + extra, log=f"{base}_{tag}.log")
    rows = {}
    for line in text.splitlines():
        m = SAMPLE_LINE.match(line)
        if m:
            key = (int(m.group(2)), int(m.group(3))) if m.group(2) else None
            rows[key] = dict(samples=int(m.group(4)), differ=int(m.group(5)), max=int(m.group(6)),
                             rms=float(m.group(7)), rms_a=float(m.group(8)), rms_b=float(m.group(9)),
                             r=float(m.group(10)))
    reads = re.search(r"\$4015 reads: (\d+), (\d+) differ", text)
    levels = re.search(r"channel levels: (\d+) cycles, (\d+) differ", text)
    return dict(rows=rows, text=text,
                reads=(int(reads.group(1)), int(reads.group(2))) if reads else None,
                levels=(int(levels.group(1)), int(levels.group(2))) if levels else None)


def frame_samples(path):
    out = {}
    for line in open(path):
        f, s = line.split()
        out[int(f)] = int(s)
    return out


def cut(src, dst, first, last):
    with wave.open(src) as w:
        w.setpos(first)
        data = w.readframes(last - first)
        rate = w.getframerate()
    with wave.open(dst, "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(rate)
        w.writeframes(data)


WAVCMP = re.compile(r"RMS: ([\d.]+) vs ([\d.]+)|correlation \(per 800 samples\): ([-\d.na]+)|"
                    r"cosine ([\d.]+), mean \|dB difference\| ([\d.]+)")


def wavcmp(a, b):
    _code, text = run([sys.executable, "-I", os.path.join(ROOT, "port", "tools", "sound_wavcmp.py"), a, b])
    r = {}
    for m in WAVCMP.finditer(text):
        if m.group(1):
            r["rms"] = (float(m.group(1)), float(m.group(2)))
        elif m.group(3):
            r["env"] = float(m.group(3))
        else:
            r["cos"], r["db"] = float(m.group(4)), float(m.group(5))
    return r


def spectral(o, p, a, b):
    base = os.path.join(o.out, p.name)
    fs = frame_samples(base + ".frames")
    first, last = fs[a], fs.get(b + 1)
    if last is None:                                   # the window runs to the end of the capture
        with wave.open(base + "_oracle.wav") as w:
            last = w.getnframes()
    last = min(last, first + SPECTRAL_CAP)
    pieces = {}
    for kind in ("oracle", "quant", "synth", "phase"):
        pieces[kind] = os.path.join(o.out, "cut", f"{p.name}_{a}_{b}_{kind}.wav")
        cut(f"{base}_{kind}.wav", pieces[kind], first, last)
    return dict(quant=wavcmp(pieces["quant"], pieces["oracle"]), floor=wavcmp(pieces["phase"], pieces["synth"]),
                seconds=(last - first) / 48000)


def passages(o, root):
    rom = os.path.join(root, "rom", "Mighty Final Fight (USA).nes")
    out = [Passage(n, f, w, route=r and os.path.join(root, r), state=s and os.path.join(root, s), rom=rom,
                   spectral=True) for n, r, s, f, w in GAME]
    # Every cycle's channel outputs during the title music's first minute.
    out.append(Passage("title_levels", 4490, [("title ($0C), channel outputs per cycle", 4429, 4489)], rom=rom,
                       levels=f"{int(4429 * FRAME)}:{int(4490 * FRAME)}"))
    if not o.no_inject:
        # Every song from its first note on a silent screen (frame 300 of the idle route), every
        # effect over the title music (frame 4510), queued as sound_queue_push would.
        oracle_exe = os.path.join(root, "build_headless", "Release", "MightyFinalFightRecomp.exe")
        bases = {300: os.path.join(o.out, "base300.cycstate"), 4510: os.path.join(o.out, "base4510.cycstate")}
        if not all(os.path.exists(b) for b in bases.values()):
            run([oracle_exe, rom, "--frames", "4510", "--save-state", f"299:{bases[300]}",
                 "--save-state", f"4509:{bases[4510]}"])
        for cmds, base, frames, name in ((SONGS, 300, 1700, "song"), (EFFECTS, 4510, 250, "sfx")):
            for c in cmds:
                path = os.path.join(o.out, f"{name}_{c:02X}.cycstate")
                if not os.path.exists(path):
                    data, _ = sound_oracle.patch(bases[base], [c])
                    open(path, "wb").write(data)
                out.append(Passage(f"{name}_{c:02X}", frames, [(f"{name} ${c:02X}", base, base + frames - 1)],
                                   state=path, rom=rom))
    # What the game never does: the generated test program on cyc_interp, with DMC fetches.
    test_rom = os.path.join(o.out, "apu_test.nes")
    run([sys.executable, "-I", os.path.join(HERE, "make_apu_rom.py"), test_rom])
    out.append(Passage("apu_test", 760, [("APU test program", 0, 759)], rom=test_rom, tool="interp", prg=True))
    out.append(Passage("apu_test_levels", 640, [("APU test, frame IRQ and DMC, outputs per cycle", 490, 639)],
                       rom=test_rom, tool="interp", prg=True, levels=f"{int(490 * FRAME)}:{int(640 * FRAME)}"))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--oracle-root", required=True, help="checkout with rom/, inputs/, states/ and the oracle builds")
    ap.add_argument("--build", default=os.path.join(ROOT, "build_synth", "Release"))
    ap.add_argument("--out", default=os.path.join(ROOT, "runs", "synth"))
    ap.add_argument("--only", help="comma-separated passage names")
    ap.add_argument("--no-spectral", action="store_true")
    ap.add_argument("--no-inject", action="store_true")
    ap.add_argument("--recapture", action="store_true")
    ap.add_argument("--jobs", type=int, default=max(1, (os.cpu_count() or 4) // 2))
    o = ap.parse_args()
    if "runs" not in os.path.abspath(o.out).replace("\\", "/").split("/"):
        raise SystemExit("--out must be under runs/ (everything here is ROM-derived)")
    os.makedirs(os.path.join(o.out, "cut"), exist_ok=True)
    todo = passages(o, os.path.abspath(o.oracle_root))
    if o.only:
        keep = set(o.only.split(","))
        todo = [p for p in todo if p.name in keep]

    with cf.ThreadPoolExecutor(o.jobs) as pool:
        list(pool.map(lambda p: capture(o, p), todo))
    results = {}

    def one(p):
        r = replay(o, p, [], "synth")
        if p.spectral and not o.no_spectral:
            replay(o, p, ["--quantize"], "quant")
            replay(o, p, ["--phase", "1"], "phase")
        if p.levels:     # the per-cycle logs are large; the numbers are in the report
            os.remove(os.path.join(o.out, p.name + ".cap"))
        return p, r
    with cf.ThreadPoolExecutor(o.jobs) as pool:
        for p, r in pool.map(one, todo):
            results[p.name] = (p, r)
    spec = {}
    if not o.no_spectral:
        with cf.ThreadPoolExecutor(o.jobs) as pool:
            jobs = {pool.submit(spectral, o, p, a, b): (p.name, label)
                    for p, _r in results.values() if p.spectral for label, a, b in p.windows}
            for fut, key in jobs.items():
                spec[key] = fut.result()

    lines = ["| Passage | Frames | Samples | Differ | Max abs diff | RMS diff | Correlation |",
             "|---|---|---|---|---|---|---|"]
    total = dict(samples=0, differ=0, max=0, reads=0, reads_bad=0, levels=0, levels_bad=0)
    notes = []
    for name, (p, r) in results.items():
        for label, a, b in p.windows:
            row = r["rows"].get((a, b))
            if not row:
                lines.append(f"| {label} | {a}-{b} | - | replay failed | | | |")
                total["differ"] += 1
                continue
            total["samples"] += row["samples"]
            total["differ"] += row["differ"]
            total["max"] = max(total["max"], row["max"])
            lines.append(f"| {label} | {a}-{b} | {row['samples']:,} | {row['differ']} | {row['max']} | "
                         f"{row['rms']:.4f} | {row['r']:.9f} |")
        if r["reads"]:
            total["reads"] += r["reads"][0]
            total["reads_bad"] += r["reads"][1]
        if r["levels"]:
            total["levels"] += r["levels"][0]
            total["levels_bad"] += r["levels"][1]
            notes.append(f"{name}: channel outputs on {r['levels'][0]:,} cycles, {r['levels'][1]} differ")
    lines.append(f"\nTotal: {total['samples']:,} samples compared, {total['differ']} differ, "
                 f"max |diff| {total['max']}; $4015 reads {total['reads']}, {total['reads_bad']} differ; "
                 f"channel outputs {total['levels']:,} cycles, {total['levels_bad']} differ.")
    lines += [f"- {n}" for n in notes]
    if spec:
        lines += ["", "Writes without exact cycles (each frame's burst from cycle 1500, 290 apart) against the "
                      "oracle, and the floor: exact writes from random oscillator phases against the exact render.",
                  "", "| Window | Seconds | RMS synth / oracle | Env r | Cosine | dB | Floor env r | Floor cosine "
                      "| Floor dB |",
                  "|---|---|---|---|---|---|---|---|---|"]
        for (name, label), s in spec.items():
            q, f = s["quant"], s["floor"]
            lines.append(f"| {label} | {s['seconds']:.1f} | {q['rms'][0]:.1f} / {q['rms'][1]:.1f} | {q['env']:.3f} | "
                         f"{q['cos']:.3f} | {q['db']:.2f} | {f['env']:.3f} | {f['cos']:.3f} | {f['db']:.2f} |")
    report = "\n".join(lines)
    with open(os.path.join(o.out, "report.md"), "w") as f:
        f.write(report + "\n")
    print(report)
    bad = total["differ"] or total["reads_bad"] or total["levels_bad"]
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
