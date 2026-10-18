"""The port's pictures against the oracle's, where the two games are in the same state.

    py -3 -I port/tests/oracle_pictures.py ORACLE.flog PORT.flog PORT.pix [--offset K] [--max N]

ORACLE.flog is the cycle-accurate build's --frame-log, PORT.flog and
PORT.pix the port's MFF_FRAMELOG and MFF_PICDUMP of the same route (keyed
input, port/tests/nmi_input.py). Frames pair by the game's NMI count, as
port/tests/lockstep.py pairs RAM, and only pairs whose RAM matches (outside
the stack page and the other bytes lockstep.py ignores) are compared. On
the console's timing (P7, the default) the port lags where the oracle does
and the games stay together; with --slowdown off or --timing scaffold it
never lags, and the games part at the oracle's first lag frame.

The oracle's record holds the picture of the frame that ended with it,
drawn after the previous NMI; the port's picture k is drawn after its NMI
k. So the oracle record paired with the port's record k shows the port's
picture k + OFFSET with OFFSET -1 (the default; --offset changes it).
Pixels are compared as palette indices (the oracle's low 6 bits).
"""
import argparse
import collections
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(__file__))
from lockstep import pair  # noqa: E402

RAM = 0x800
PIX = 256 * 240
# as lockstep.py: the stack page, the tasks' saved stack pointers and
# nmi_return_pc, and the sound driver's RAM (when the post-NMI sound update
# runs against a frame's sound pushes differs; nothing drawn depends on it)
IGNORE = (set(range(0x100, 0x200)) | {0x82, 0x86, 0x8A, 0x8E, 0xA0, 0xA1} |
          set(range(0xC0, 0xD9)) | set(range(0x700, 0x780)))


def read_oracle(path):
    """[(frame, ram, offset of the picture)]"""
    out = []
    with open(path, "rb") as f:
        magic = f.read(12)
        if magic[:8] != b"CYCFRAME" or struct.unpack_from("<I", magic, 8)[0] != 2:
            sys.exit(f"{path}: not a version-2 cycle frame log")
        while True:
            head = f.read(36)
            if len(head) < 36:
                break
            frame, _c0, _c1, n_ram, n_ci, n_cart, n_chr, n_pic, n_extra = struct.unpack("<9I", head)
            f.seek(32 + 256, 1)
            ram = f.read(n_ram)
            if len(ram) < n_ram:
                break
            f.seek(n_ci + n_cart + n_chr, 1)
            out.append((frame, ram, f.tell()))
            f.seek(n_pic + n_extra, 1)
    return out


def read_port(path):
    rec = 4 + RAM + 0x100 + 0x20
    data = open(path, "rb").read()
    return [(struct.unpack_from("<I", data, i)[0], data[i + 4:i + 4 + RAM])
            for i in range(0, len(data) - rec + 1, rec)]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("oracle")
    ap.add_argument("port")
    ap.add_argument("pix")
    ap.add_argument("--offset", type=int, default=-1)
    ap.add_argument("--max", type=int, default=10)
    a = ap.parse_args()
    oracle = read_oracle(a.oracle)
    pic_at = {frame: off for frame, _, off in oracle}
    pairs = pair([(f, r) for f, r, _ in oracle], read_port(a.port))
    pix_size = os.path.getsize(a.pix) // (4 + PIX)
    compared = same = skipped = 0
    diff_lines = collections.Counter()
    shown = 0
    with open(a.oracle, "rb") as fo, open(a.pix, "rb") as fp:
        for (count, rep), (of, oram), (pf, pram) in pairs:
            if any(oram[i] != pram[i] for i in range(RAM) if i not in IGNORE):
                skipped += 1                # not the same game state (a lag frame's record)
                continue
            k = pf + a.offset
            if not 0 <= k < pix_size:
                continue
            fo.seek(pic_at[of])
            o = struct.unpack(f"<{PIX}H", fo.read(2 * PIX))
            fp.seek(k * (4 + PIX) + 4)
            p = fp.read(PIX)
            if not any(p):
                continue                    # nothing drawn yet
            compared += 1
            bad = [i for i in range(PIX) if (o[i] & 0x3F) != p[i]]
            if not bad:
                same += 1
                continue
            lines = sorted({i // 256 for i in bad})
            for y in lines:
                diff_lines[y] += 1
            if shown < a.max:
                shown += 1
                print(f"NMI {count}{'+' + str(rep) if rep else ''}: oracle frame {of}, port picture {k}: "
                      f"{len(bad)} pixels differ on lines {lines[0]}-{lines[-1]} ({len(lines)} lines)")
    print(f"{len(pairs)} frames paired; {compared} pictures compared while RAM matched, {same} identical"
          + (f"; {skipped} frames skipped, their RAM not the oracle's" if skipped else ""))
    if diff_lines:
        top = ", ".join(f"{y}:{n}" for y, n in sorted(diff_lines.items())[:40])
        print(f"lines that differed (line:frames): {top}")
    sys.exit(0 if compared and same == compared else 1)


if __name__ == "__main__":
    main()
