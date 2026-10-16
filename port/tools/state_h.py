"""Write port/src/game/state.h: the game's RAM variables, from disasm/labels.toml.

    py -3 -I port/tools/state_h.py

One #define per [[ram]] entry (NAME = address; a table's other bytes are
NAME + i), with its comment, so lifted and hand-written routines say
RAM(DRAW_PARITY) and the names stay those of the disassembly. Run it after
editing labels.toml (regen.sh does).
"""
import os
import tomllib

ROOT = os.path.normpath(os.path.join(os.path.dirname(__file__), "..", ".."))


def main():
    lab = tomllib.load(open(os.path.join(ROOT, "disasm", "labels.toml"), "rb"))
    out = ["/* state.h - the game's RAM, from disasm/labels.toml (port/tools/state_h.py; do not edit) */",
           "#ifndef MFF_STATE_H", "#define MFF_STATE_H", ""]
    for r in sorted(lab.get("ram", []), key=lambda r: r["addr"]):
        size = r.get("size", 1)
        note = r.get("comment", "")
        span = f" (${r['addr']:04X}-${r['addr'] + size - 1:04X})" if size > 1 else ""
        out.append(f"#define {r['name'].upper():<24} 0x{r['addr']:04X}" +
                   (f"  /* {note[:90]}{span} */" if note or span else ""))
    out += ["", "#endif", ""]
    path = os.path.join(ROOT, "port", "src", "game", "state.h")
    open(path, "w", newline="\n").write("\n".join(out))
    print(f"{path}: {len(lab.get('ram', []))} variables")


if __name__ == "__main__":
    main()
