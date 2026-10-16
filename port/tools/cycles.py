"""A routine's code with the scaffold's cycle counts, for writing P4 replacements.

    py -3 -I port/tools/cycles.py ROM ADDR [--bank BB]

Walks the routine at ADDR (8 KiB bank BB; $C000+ defaults to the fixed $0E/$0F)
along branches and fall-through, not into JSRs, up to RTS/RTI/JMP, and prints
it in address order with each instruction's cycles as NESRecomp's generator
charges them (its opcode table: base count, no page-cross penalty; a taken
branch adds 1). Basic blocks are summed so a replacement can charge one
mff_cycles() per block. Names come from disasm/labels.toml. Prints only to
the terminal.
"""
import argparse
import os
import re
import sys
import tomllib

ROOT = os.path.join(os.path.dirname(__file__), "..", "..")
sys.path.insert(0, os.path.join(ROOT, "tools"))
from cdl_report import window  # noqa: E402
from dis6502 import MN  # noqa: E402

TABLE = os.path.join(ROOT, "nesrecomp", "recompiler", "src", "cpu6502_decoder.c")
SIZE = {"imp": 1, "acc": 1, "imm": 2, "zp": 2, "zpx": 2, "zpy": 2, "izx": 2, "izy": 2, "rel": 2,
        "abs": 3, "abx": 3, "aby": 3, "ind": 3}


def cycle_table():
    cyc = {}
    for m in re.finditer(r"/\* 0x([0-9A-F]{2}) \*/ \{\w+\s*,\s*\w+\s*,\s*(\d)\s*,\s*(\d+)\}",
                         open(TABLE).read()):
        cyc[int(m.group(1), 16)] = int(m.group(3))
    return cyc


def names():
    lab = tomllib.load(open(os.path.join(ROOT, "disasm", "labels.toml"), "rb"))
    ram, code = {}, {}
    for r in lab.get("ram", []):
        for i in range(r.get("size", 1)):
            ram[r["addr"] + i] = r["name"] + (f"+{i}" if i else "")
    for l in lab.get("label", []):
        code[l["addr"]] = l["name"]
    return ram, code


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("rom")
    ap.add_argument("addr")
    ap.add_argument("--bank")
    a = ap.parse_args()
    addr = int(a.addr, 16)
    bank = int(a.bank, 16) if a.bank else (0x0E if addr < 0xE000 else 0x0F) if addr >= 0xC000 else None
    if bank is None:
        sys.exit("--bank is needed below $C000")
    rom = open(a.rom, "rb").read()
    prg = rom[16:16 + rom[4] * 0x4000]
    base = bank * 0x2000 - window(bank)
    cyc = cycle_table()
    ram, code = names()

    def byte(pc):
        return prg[base + pc]

    seen, todo, targets = {}, [addr], set()
    while todo:
        pc = todo.pop()
        while pc not in seen and window(bank) <= pc < window(bank) + 0x2000:
            op = byte(pc)
            if op not in MN:
                seen[pc] = (op, "???", None)
                break
            mn, mode = MN[op]
            n = SIZE[mode]
            arg = byte(pc + 1) if n == 2 else (byte(pc + 1) | byte(pc + 2) << 8) if n == 3 else None
            seen[pc] = (op, mn, mode, arg, n)
            if mode == "rel":
                t = (pc + 2 + (arg - 256 if arg >= 128 else arg)) & 0xFFFF
                targets.add(t)
                todo.append(t)
            if mn in ("RTS", "RTI", "JMP"):
                break
            pc += n

    total_block, block_start = 0, None
    for pc in sorted(seen):
        e = seen[pc]
        if len(e) == 3:
            print(f"  ${pc:04X}  .byte ${e[0]:02X}   (not an official opcode: data?)")
            continue
        op, mn, mode, arg, n = e
        if pc in targets or block_start is None:
            if block_start is not None and total_block:
                print(f"        ---- block ${block_start:04X}: {total_block} cycles (falls through)")
            block_start, total_block = pc, 0
            label = code.get(pc, "")
            print(f"${pc:04X}:" + (f"  {label}" if label else ""))
        if mode == "rel":
            text = f"{mn} ${(pc + 2 + (arg - 256 if arg >= 128 else arg)) & 0xFFFF:04X}"
        elif mode in ("imp",):
            text = mn
        elif mode == "acc":
            text = f"{mn} A"
        elif mode == "imm":
            text = f"{mn} #${arg:02X}"
        else:
            fmt = {"zp": "${:02X}", "zpx": "${:02X},X", "zpy": "${:02X},Y", "izx": "(${:02X},X)",
                   "izy": "(${:02X}),Y", "abs": "${:04X}", "abx": "${:04X},X", "aby": "${:04X},Y",
                   "ind": "(${:04X})"}[mode]
            text = f"{mn} " + fmt.format(arg)
            nm = code.get(arg) if mn in ("JSR", "JMP") else ram.get(arg)
            if nm:
                text += f"  ; {nm}"
        c = cyc.get(op, 0)
        total_block += c
        extra = "  (+1 taken)" if mode == "rel" else ""
        raw = " ".join(f"{byte(pc + i):02X}" for i in range(n))
        print(f"  ${pc:04X}  {raw:<9} {text:<34} {c}{extra}")
        if mode == "rel" or mn in ("RTS", "RTI", "JMP"):
            end = {"RTS": "returns", "RTI": "returns", "JMP": "jumps"}.get(mn, "branches")
            print(f"        ---- block ${block_start:04X}: {total_block} cycles ({end})")
            block_start, total_block = pc + n, 0
            if pc + n in seen and pc + n not in targets:
                print(f"${pc + n:04X}:")


if __name__ == "__main__":
    main()
