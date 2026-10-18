"""A run loaded from a save state against the run that saved it.

    py -3 -I port/tests/savestate_cmp.py FULL.flog FULL.pic LOADED.flog LOADED.pic

FULL is the uninterrupted run (MFF_SAVE_AT saved the state on the way),
LOADED the one started from the state with --load and the same route
(port/tests/savestate_check.sh). The loaded run's first record is the frame
after the save; from there to its end every frame log record (frame number,
CPU RAM, OAM, palette) and every picture log record (frame number, the
picture's hash and the cycle count) must be the full run's, byte for byte.
Exit 0 only if they are.
"""
import sys

FLOG = 4 + 0x800 + 0x100 + 0x20
PIC = 12


def records(path, size):
    data = open(path, "rb").read()
    return {int.from_bytes(data[k:k + 4], "little"): data[k:k + size]
            for k in range(0, len(data) - size + 1, size)}


def compare(full, loaded, what):
    if not loaded:
        print(f"{what}: the loaded run wrote nothing")
        return False
    first, last = min(loaded), max(loaded)
    for f in range(first, last + 1):
        if f not in loaded or f not in full:
            print(f"{what}: frame {f} missing ({'loaded' if f not in loaded else 'full'} run)")
            return False
        if loaded[f] != full[f]:
            a, b = full[f], loaded[f]
            at = next(i for i in range(len(a)) if a[i] != b[i])
            print(f"{what}: frame {f} differs at byte {at} of its record")
            return False
    print(f"{what}: frames {first}-{last} identical ({last - first + 1})")
    return True


def main():
    full_flog, full_pic, flog, pic = sys.argv[1:5]
    ok = compare(records(full_flog, FLOG), records(flog, FLOG), "frame log")
    ok &= compare(records(full_pic, PIC), records(pic, PIC), "picture log")
    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()
