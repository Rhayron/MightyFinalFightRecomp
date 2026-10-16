"""Ground truth for the sound driver, taken from the cycle-accurate oracle.

    py -3 -I port/tools/sound_oracle.py ram    ROM ROUTE FRAMES OUT.ram [STATE]   # per-frame CPU RAM
                                    (ROUTE "-" = no input; with STATE, FRAMES below the
                                    state's frame counts from it)
    py -3 -I port/tools/sound_oracle.py apu    ROM ROUTE A:B OUT.apu [STATE]  # per-frame APU writes
    py -3 -I port/tools/sound_oracle.py show   OUT.apu [A:B]                  # print a trace file
    py -3 -I port/tools/sound_oracle.py inject ROM STATE CMD[,CMD] FRAMES OUT.ram
                                    (queue commands in a copy of STATE, run without input)
    py -3 -I port/tools/sound_oracle.py patch  STATE CMD[,CMD] OUT.cycstate   # just the copy

`ram` runs build_headless/Release/MightyFinalFightRecomp.exe with --frame-log
and keeps only the 2 KiB of CPU RAM of each frame (the full log is ~135 KB a
frame and is deleted at once). Output records: u32 frame + 0x800 RAM.

`apu` gets every write to $4000-$4017 the CPU made in frames A..B, with its
cycle, from the oracle's --trace-frame (one frame per run, so the route is
first run once to save a state every 50 frames, and each traced frame is
run from the nearest state). Output: text lines "frame start CYCLE" (the
frame's first cycle, the NMI), "frame CYCLE ADDR VALUE" per write, "frame
end COUNT". Cycles count from the start of the run that traced the frame.

`inject` puts commands in the sound queue of a copy of a state (as
sound_queue_push would), runs the oracle from it without input and writes
the RAM file, starting with the patched state's own RAM: every sound
command can then be checked through the oracle's driver from its first
frame, wherever the game itself would play it.

The oracle's frame starts at the NMI: the frame-log record of frame N is the
machine at the start of the NMI of frame N+1, and the sound update of frame N
runs early in frame N, after the NMI handler (nmi_tail -> sound_update).

Everything written here is ROM-derived: keep it under runs/sound/.
"""
import os
import re
import struct
import subprocess
import sys
import tempfile

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
ORACLE = os.path.join(ROOT, "build_headless", "Release", "MightyFinalFightRecomp.exe")
RAM_SIZE = 0x800
NO_WINDOW = 0x08000000 if os.name == "nt" else 0          # CREATE_NO_WINDOW


def run_oracle(args, quiet=True):
    out = subprocess.run([ORACLE] + args, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                         creationflags=NO_WINDOW)
    if out.returncode != 0:
        sys.stderr.write(out.stdout.decode("utf-8", "replace"))
        raise SystemExit(f"oracle failed ({out.returncode}): {' '.join(args)}")
    if not quiet:
        sys.stdout.write(out.stdout.decode("utf-8", "replace"))
    return out.stdout.decode("utf-8", "replace")


def framelog_ram(path):
    """Yield (frame, ram) from a version-2 CYCFRAME log."""
    with open(path, "rb") as f:
        magic = f.read(12)
        if magic[:8] != b"CYCFRAME" or struct.unpack_from("<I", magic, 8)[0] != 2:
            raise SystemExit(f"{path}: not a version-2 cycle frame log")
        while True:
            head = f.read(36)
            if len(head) < 36:
                return
            frame, _c0, _c1, ram_len, ciram, cart, chr_len, pic, audio = struct.unpack("<9I", head)
            f.seek(32 + 256, 1)
            ram = f.read(ram_len)
            if len(ram) < ram_len:
                return
            f.seek(ciram + cart + chr_len + pic + audio, 1)
            yield frame, ram


def collect_ram(rom, route, frames, out_path, chunk=2000, state=None, first=0):
    """u32 frame + RAM records of frames first..frames-1 into out_path. One
    run saves a state at each chunk boundary, then each chunk runs from its
    state with a frame log (deleted as soon as it is read), so the
    temporary log stays at ~135 KB x chunk. state: start from a .cycstate
    (its frame must be <= first)."""
    tmp = out_path + ".framelog.tmp"
    tmpdir = tempfile.mkdtemp(prefix="ram_", dir=os.path.dirname(os.path.abspath(out_path)))
    common = ["--input", route] if route else []
    if state:
        common += ["--load-state", state]
    bounds = list(range(first + chunk, frames, chunk))
    states = {}
    if bounds:
        args = [rom, "--frames", str(bounds[-1]), *common]
        for b in bounds:
            states[b] = os.path.join(tmpdir, f"s{b}.cycstate")
            args += ["--save-state", f"{b - 1}:{states[b]}"]
        run_oracle(args)
    with open(out_path, "wb") as out:
        start = first
        while start < frames:
            end = min(frames, start + chunk) - 1
            args = [rom, "--frames", str(end + 1), "--frame-log", tmp, "--frame-log-frames", f"{start}:{end}"]
            args += ["--input", route] if route else []
            if start in states:
                args += ["--load-state", states[start]]
            elif state:
                args += ["--load-state", state]
            run_oracle(args)
            for frame, ram in framelog_ram(tmp):
                out.write(struct.pack("<I", frame) + ram)
            os.remove(tmp)
            start = end + 1
    for p in states.values():
        os.remove(p)
    os.rmdir(tmpdir)


def read_ram(path):
    """{frame: ram} from a collect_ram file."""
    data = open(path, "rb").read()
    rec = 4 + RAM_SIZE
    out = {}
    for i in range(0, len(data) - rec + 1, rec):
        out[struct.unpack_from("<I", data, i)[0]] = data[i + 4:i + rec]
    return out


WRITE = re.compile(r"^(\d+) W (40[01][0-9A-F]) ([0-9A-F]{2})")


def state_frame(path):
    """The next frame a .cycstate runs (header field at offset 28)."""
    with open(path, "rb") as f:
        return struct.unpack_from("<I", f.read(32), 28)[0]


def collect_apu(rom, route, first, last, out_path, step=50, state=None):
    """APU register writes of frames first..last, one oracle run per frame
    (from power-on with `route`, or from `state`)."""
    tmpdir = tempfile.mkdtemp(prefix="apu_", dir=os.path.dirname(os.path.abspath(out_path)))
    origin = state_frame(state) if state else 0
    states = {}
    base = max(origin, (first // step) * step)
    points = [p for p in range(base, last + 1, step) if p > origin]
    if points:
        args = [rom, "--frames", str(points[-1] + 1)]
        if route:
            args += ["--input", route]
        if state:
            args += ["--load-state", state]
        for p in points:
            path = os.path.join(tmpdir, f"s{p}.cycstate")
            args += ["--save-state", f"{p - 1}:{path}"]        # saved after frame p-1: resumes at p
            states[p] = path
        run_oracle(args)
    if state:
        states[origin] = state
    with open(out_path, "w") as out:
        for frame in range(first, last + 1):
            start = max([p for p in states if p <= frame], default=None)
            trace = os.path.join(tmpdir, "trace.txt")
            args = [rom, "--frames", str(frame + 1), "--trace-frame", str(frame), "--trace-out", trace]
            if route:
                args += ["--input", route]
            if start is not None:
                args += ["--load-state", states[start]]
            run_oracle(args)
            n = 0
            started = False
            with open(trace) as t:
                for line in t:
                    if not started and line[:1].isdigit():
                        out.write(f"{frame} start {line.split()[0]}\n")    # the frame's first cycle (the NMI)
                        started = True
                    m = WRITE.match(line)
                    if m and int(m.group(2), 16) <= 0x4017:
                        out.write(f"{frame} {m.group(1)} {m.group(2)} {m.group(3)}\n")
                        n += 1
            out.write(f"{frame} end {n}\n")
            os.remove(trace)
    for p, path in states.items():
        if path != state:
            os.remove(path)
    os.rmdir(tmpdir)


def read_apu(path, starts=None):
    """{frame: [(cycle, addr, value), ...]} from a collect_apu file; frames
    that were traced are present even when they wrote nothing. Cycles count
    from the start of the oracle run that traced the frame; pass a dict as
    `starts` to get each frame's first cycle (the NMI) too."""
    out = {}
    for line in open(path):
        parts = line.split()
        frame = int(parts[0])
        if parts[1] == "end":
            out.setdefault(frame, [])
            continue
        if parts[1] == "start":
            if starts is not None:
                starts[frame] = int(parts[2])
            continue
        out.setdefault(frame, []).append((int(parts[1]), int(parts[2], 16), int(parts[3], 16)))
    return out


def patch(state, commands):
    """A copy of `state` with `commands` appended to the game's sound queue
    the way sound_queue_push does (at sound_queue_write, then advance it).
    Returns (state bytes, offset of CPU RAM in them)."""
    sys.path.insert(0, os.path.join(ROOT, "tools"))
    import state_ram
    data = bytearray(open(state, "rb").read())
    base, _ = state_ram.ram_span(bytes(data))
    w = data[base + 0xDA]
    for c in commands:
        if data[base + 0xDC + w] != 0x88:
            raise SystemExit("sound queue full in that state")
        data[base + 0xDC + w] = c
        w = (w + 1) & 7
    data[base + 0xDA] = w
    return data, base


def inject(rom, state, commands, frames, out_path):
    """patch() a state, run the oracle from it without input and keep the
    per-frame RAM. A coverage aid: the commands then go through the
    driver's own code on the oracle at the state's next frame."""
    data, base = patch(state, commands)
    first = struct.unpack_from("<I", data, 28)[0]
    patched = out_path + ".state.tmp"
    open(patched, "wb").write(data)
    try:
        collect_ram(rom, None, first + frames, out_path, state=patched, first=first)
    finally:
        os.remove(patched)
    # The state holds the RAM at the end of frame first-1, with the commands
    # queued: put it in front, so a check starts before the driver sees them.
    body = open(out_path, "rb").read()
    with open(out_path, "wb") as out:
        out.write(struct.pack("<I", first - 1) + bytes(data[base:base + RAM_SIZE]))
        out.write(body)


def main(argv):
    if len(argv) < 2:
        sys.exit(__doc__)
    cmd = argv[1]
    if cmd == "inject" and len(argv) == 7:
        inject(argv[2], argv[3], [int(c, 16) for c in argv[4].split(",")], int(argv[5]), argv[6])
        return
    if cmd == "patch" and len(argv) == 5:
        data, _ = patch(argv[2], [int(c, 16) for c in argv[3].split(",")])
        open(argv[4], "wb").write(data)
        return
    if cmd == "ram" and len(argv) in (6, 7):
        state = argv[6] if len(argv) == 7 else None
        first = 0
        if state:
            with open(state, "rb") as f:
                first = struct.unpack_from("<I", f.read(32), 28)[0]     # the next frame to run
        frames = int(argv[4])
        if frames <= first:
            frames += first                                             # relative to the state
        collect_ram(argv[2], argv[3] if argv[3] != "-" else None, frames, argv[5], state=state, first=first)
    elif cmd == "apu" and len(argv) in (6, 7):
        a, b = (int(x) for x in argv[4].split(":"))
        collect_apu(argv[2], argv[3] if argv[3] != "-" else None, a, b, argv[5],
                    state=argv[6] if len(argv) == 7 else None)
    elif cmd == "show" and len(argv) in (3, 4):
        trace = read_apu(argv[2])
        a, b = (int(x) for x in argv[3].split(":")) if len(argv) == 4 else (min(trace), max(trace))
        for f in range(a, b + 1):
            if f in trace:
                print(f, " ".join(f"{addr:04X}={v:02X}" for _c, addr, v in trace[f]))
    else:
        sys.exit(__doc__)


if __name__ == "__main__":
    main(sys.argv)
