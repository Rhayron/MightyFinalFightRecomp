"""Lockstep check: the P3 scaffold (or, later, the port) against the cycle-accurate oracle.

    py -3 -I port/tests/lockstep.py ORACLE.framelog SCAFFOLD.framelog [--ignore 0100-01FF,00A0-00A1] [--max 20]

ORACLE.framelog: the cycle build's --frame-log (runner/cyc/cyc_host.c
write_frame_log). SCAFFOLD.framelog: port/scaffold's MFF_FRAMELOG.

Frames are paired by the game's own NMI counter (frame_counter, $92), which
the NMI bumps once per frame even on lag frames, so the two runs line up
whatever each counts as "frame 0". For each pair the CPU RAM is compared
(minus --ignore ranges); the report gives the first diverging frames and
bytes, with names from disasm/labels.toml.
"""
import argparse
import os
import struct
import sys
import tomllib

RAM = 0x800
FRAME_COUNTER = 0x92


def read_oracle(path):
    out = []
    with open(path, "rb") as f:
        magic = f.read(12)                           # "CYCFRAME" + u32 version
        if magic[:8] != b"CYCFRAME" or struct.unpack_from("<I", magic, 8)[0] != 2:
            sys.exit(f"{path}: not a version-2 cycle frame log")
        while True:
            head = f.read(36)
            if len(head) < 36:
                break
            frame, _c0, _c1, ram_len, ciram, cart, chr_len, pic, audio = struct.unpack("<9I", head)
            f.read(32 + 256)                         # palette, OAM
            ram = f.read(ram_len)
            if len(ram) < ram_len:
                break
            f.seek(ciram + cart + chr_len + pic + audio, 1)
            out.append((frame, ram))
    return out


def read_scaffold(path):
    out = []
    rec = 4 + RAM + 0x100 + 0x20
    data = open(path, "rb").read()
    for i in range(0, len(data) - rec + 1, rec):
        frame = struct.unpack_from("<I", data, i)[0]
        out.append((frame, data[i + 4:i + 4 + RAM]))
    return out


def ram_names():
    path = os.path.join(os.path.dirname(__file__), "..", "..", "disasm", "labels.toml")
    names = {}
    try:
        doc = tomllib.load(open(path, "rb"))
    except FileNotFoundError:
        return names
    for e in doc.get("ram", []):
        for k in range(e.get("size", 1)):
            names[e["addr"] + k] = e["name"] + (f"+{k}" if k else "")
    return names


def pair(oracle, scaffold):
    """Pair frames by the NMI count since reset cleared RAM: each side starts
    counting at its first frame with $92 = 0 that the next frame increments
    (power-on RAM differs: the oracle's reference pattern, zeros on the
    scaffold), and $92 is unwrapped from there. Key = (NMI count, n-th frame
    showing it), so a lag frame pairs with a lag frame."""
    def keyed(rows):
        out, start = [], None
        for k in range(len(rows) - 1):
            if rows[k][1][FRAME_COUNTER] == 0 and rows[k + 1][1][FRAME_COUNTER] == 1:
                start = k
                break
        if start is None:
            return out
        count, last, seen = 0, 0, {}
        for frame, ram in rows[start:]:
            v = ram[FRAME_COUNTER]
            count += (v - last) & 0xFF
            last = v
            n = seen.get(count, 0)
            seen[count] = n + 1
            out.append(((count, n), frame, ram))
        return out
    o = {}
    for key, frame, ram in keyed(oracle):
        o.setdefault(key, (frame, ram))
    pairs = []
    for key, frame, ram in keyed(scaffold):
        if key in o:
            pairs.append((key, o[key], (frame, ram)))
    return pairs


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("oracle")
    ap.add_argument("scaffold")
    # stack page, each task's saved stack pointer (tasks+2/+6/+10/+14) and
    # nmi_return_pc (the PC the NMI interrupted; the scaffold calls the NMI between frames): the
    # scaffold's JSR/NMI frames are not byte-for-byte the 6502's
    ap.add_argument("--ignore", default="0100-01FF,0082,0086,008A,008E,00A0-00A1", help="comma-separated hex ranges")
    # RAM the sound driver (banks $06/$07) owns, $C0-$D8 and $0700-$077F
    # (docs/sound.md): told apart, since when the post-NMI sound update runs
    # relative to a frame's sound pushes differs
    ap.add_argument("--sound", default="00C0-00D8,0700-077F")
    ap.add_argument("--max", type=int, default=12)
    a = ap.parse_args()

    def ranges(text):
        out = set()
        for part in filter(None, text.split(",")):
            lo, _, hi = part.partition("-")
            out.update(range(int(lo, 16), int(hi or lo, 16) + 1))
        return out
    ignore, sound = ranges(a.ignore), ranges(a.sound)
    names = ram_names()
    pairs = pair(read_oracle(a.oracle), read_scaffold(a.scaffold))
    if not pairs:
        sys.exit("no frames paired (no common $92 values)")
    # Classes: "sound" = only sound driver bytes differ; "lag" = the oracle's
    # snapshot caught task 0 still running (state 2: the frame's work spilled
    # past the NMI) or this is the frame after one - the scaffold estimates
    # CPU time, so where the NMI cuts a long frame differs; "game" = the rest.
    counts = {"identical": 0, "sound": 0, "lag": 0, "game": 0}
    shown, first_game, prev_lag = 0, None, False
    for (count, rep), (of, oram), (sf, sram) in pairs:
        lag = oram[0x80] == 2 or sram[0x80] == 2
        diffs = [i for i in range(RAM) if i not in ignore and oram[i] != sram[i]]
        cls = ("identical" if not diffs else "sound" if all(i in sound for i in diffs)
               else "lag" if lag or prev_lag else "game")
        prev_lag = lag
        counts[cls] += 1
        if cls != "game":
            continue
        if first_game is None:
            first_game = (of, sf)
        if shown < a.max:
            shown += 1
            game = [i for i in diffs if i not in sound]
            listed = ", ".join(f"${i:04X}{'(' + names[i] + ')' if i in names else ''} "
                               f"{oram[i]:02X}/{sram[i]:02X}" for i in game[:8])
            print(f"NMI {count}{'+' + str(rep) if rep else ''}: oracle frame {of}, scaffold frame {sf}: "
                  f"{len(game)} game bytes differ: {listed}" + (" ..." if len(game) > 8 else ""))
    print(f"{len(pairs)} frames paired: " + ", ".join(f"{v} {k}" for k, v in counts.items())
          + f" (ignoring {len(ignore)} bytes)"
          + (f"; first game divergence at oracle frame {first_game[0]} / scaffold frame {first_game[1]}"
             if first_game else ""))
    sys.exit(1 if first_game else 0)


if __name__ == "__main__":
    main()
