"""Check a player package of the port for anything from the cartridge.

    py -3 -I port/tools/release_check.py PACKAGE.zip [DUMP] [--allow NAME ...]

Lists the package and fails if it holds a name outside the allowed ones
(the executable, SDL2's library, the README and SDL2's license) or anything
that looks like game data (assets/, *.bin, *.nes, *.mffstate, *.ini).
With the dump it also looks inside every file for any run of 32 bytes of
the dump's PRG or CHR ROM (runs of fewer than 8 different byte values,
counting runs and interleaved counting runs are skipped: fills, padding and
ascending tables match any program): none may be there. Python
standard library only.
"""
import argparse
import sys
import zipfile

WINDOW = 32
MIN_DISTINCT = 8
ALLOWED = {"MightyFinalFight.exe", "MightyFinalFight", "SDL2.dll", "libSDL2-2.0.so.0",
           "libSDL2-2.0.0.dylib", "README.txt", "SDL2-LICENSE.txt"}
FORBIDDEN = (".bin", ".nes", ".mffstate", ".ini", ".png", ".json", ".flog", ".pic")


def plain(w):
    """A counting run (00 01 02 ...) or two interleaved ones (UTF-16 text,
    ascending tables): common in any program, not the game's."""
    for step in (1, 2):
        if len({(w[i + step] - w[i]) & 0xFF for i in range(len(w) - step)}) <= 2:
            return True
    return False


def rom_windows(path):
    data = open(path, "rb").read()
    skip = 16 + (512 if data[6] & 4 else 0)
    body = data[skip:skip + 0x40000]              # PRG then CHR, 128 KiB each
    seen = set()
    for i in range(len(body) - WINDOW + 1):
        w = body[i:i + WINDOW]
        if len(set(w)) >= MIN_DISTINCT and not plain(w):
            seen.add(w)
    return seen


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("package")
    ap.add_argument("dump", nargs="?")
    ap.add_argument("--allow", action="append", default=[])
    a = ap.parse_args()
    allowed = ALLOWED | set(a.allow)
    ok = True
    windows = rom_windows(a.dump) if a.dump else None
    with zipfile.ZipFile(a.package) as z:
        for info in z.infolist():
            name = info.filename
            base = name.rstrip("/").split("/")[-1]
            print(f"{info.file_size:>10}  {name}")
            if name.endswith("/"):
                if base.lower() in ("assets", "saves"):
                    print(f"  ** a {base}/ folder")
                    ok = False
                continue
            if base not in allowed or base.lower().endswith(FORBIDDEN):
                print("  ** not an allowed file")
                ok = False
            if windows is not None:
                data = z.read(info)
                hits = [i for i in range(len(data) - WINDOW + 1) if data[i:i + WINDOW] in windows]
                if hits:
                    print(f"  ** {len(hits)} runs of {WINDOW} bytes from the dump, the first at offset {hits[0]}")
                    ok = False
    if windows is not None:
        print(f"searched for {len(windows)} distinct {WINDOW}-byte runs of the dump's PRG and CHR")
    print("OK: nothing from the cartridge" if ok else "FAILED")
    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()
