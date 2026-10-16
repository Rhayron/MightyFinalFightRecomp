"""Generate a reassemblable ca65 disassembly of the user's dump (PLAN_DECOMP.md P1).

    py -3 -I disasm/tools/disasm.py ROM --map runs/cdl/prgmap.bin [--labels disasm/labels.toml] \
        [--out build/disasm]

One file per 8 KiB PRG bank (bankNN.s, .org at the bank's MMC3 mode-0 window),
plus an ld65 config. Bytes the map calls code (executed opcodes, and code
reached by control flow) become instructions; everything else is .byte. The
output is generated from the dump and is never committed: the repo keeps only
this generator and disasm/labels.toml.

Byte-exactness rules:
- absolute operands below $0100 are written with ca65's `a:` prefix, so the
  assembler does not shrink them to zero page;
- labels are made only for targets inside the same bank (or the fixed banks
  $0E/$0F, which are always mapped); a reference into a switchable window
  from another bank stays numeric, because which bank is mapped there is a
  run-time fact;
- a label that would fall inside an instruction is not made (the reference
  stays numeric).
"""
import argparse
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, "..", "..", "tools"))
from cdl_report import CLASSES, LENGTH, OFFICIAL, window  # noqa: E402
from dis6502 import MN  # noqa: E402

CODE_CLASSES = {CLASSES.index("opcode"), CLASSES.index("code?")}
TABLE = CLASSES.index("table")
PTR = CLASSES.index("ptr")
FIXED = {0x0E: 0xC000, 0x0F: 0xE000}


HW = {0x2000: "PPUCTRL", 0x2001: "PPUMASK", 0x2002: "PPUSTATUS", 0x2003: "OAMADDR", 0x2004: "OAMDATA",
      0x2005: "PPUSCROLL", 0x2006: "PPUADDR", 0x2007: "PPUDATA",
      0x4000: "SQ1_VOL", 0x4001: "SQ1_SWEEP", 0x4002: "SQ1_LO", 0x4003: "SQ1_HI",
      0x4004: "SQ2_VOL", 0x4005: "SQ2_SWEEP", 0x4006: "SQ2_LO", 0x4007: "SQ2_HI",
      0x4008: "TRI_LINEAR", 0x400A: "TRI_LO", 0x400B: "TRI_HI", 0x400C: "NOISE_VOL", 0x400E: "NOISE_LO",
      0x400F: "NOISE_HI", 0x4010: "DMC_FREQ", 0x4011: "DMC_RAW", 0x4012: "DMC_START", 0x4013: "DMC_LEN",
      0x4014: "OAMDMA", 0x4015: "APU_STATUS", 0x4016: "JOY1", 0x4017: "JOY2_FRAME",
      0x8000: "MMC3_BANK_SELECT", 0x8001: "MMC3_BANK_DATA", 0xA000: "MMC3_MIRRORING", 0xA001: "MMC3_PRG_RAM",
      0xC000: "MMC3_IRQ_LATCH", 0xC001: "MMC3_IRQ_RELOAD", 0xE000: "MMC3_IRQ_DISABLE", 0xE001: "MMC3_IRQ_ENABLE"}
WRITES = {0x8D, 0x8E, 0x8C, 0x9D, 0x99}            # STA/STX/STY abs, STA abs,X / abs,Y
RAM = {}      # address -> (name, offset) from labels.toml [[ram]]
RAM_DEFS = []
CODE_HINTS = set()    # PRG offsets labels.toml marks code = true (code no run executed)


def load_labels(path):
    """labels.toml: [[label]] bank/addr/name/comment for code and tables,
    [[ram]] addr/size/name/comment for RAM operands."""
    out = {}
    if not path or not os.path.exists(path):
        return out
    import tomllib
    with open(path, "rb") as f:
        doc = tomllib.load(f)
    for entry in doc.get("label", []):
        out[(entry["bank"], entry["addr"])] = (entry["name"], entry.get("comment", ""))
        if entry.get("code"):
            CODE_HINTS.add(entry["bank"] * 0x2000 + entry["addr"] - window(entry["bank"]))
    for entry in doc.get("ram", []):
        RAM_DEFS.append((entry["name"], entry["addr"], entry.get("comment", "")))
        for k in range(entry.get("size", 1)):
            RAM[entry["addr"] + k] = (entry["name"], k)
    return out


def ram_operand(v):
    if v not in RAM:
        return None
    name, k = RAM[v]
    return name if not k else f"{name}+{k}"


def owner(bank, addr):
    """Bank whose bytes are at CPU `addr` from code in `bank`, if certain."""
    if addr >= 0xE000:
        return 0x0F
    if addr >= 0xC000:
        return 0x0E
    w = window(bank)
    if bank < 0x0E and w <= addr < w + 0x2000:
        return bank
    return None


def decode_insns(prg, kind, bank):
    """Instruction starts in this bank: code bytes whose whole instruction fits."""
    base = bank * 0x2000
    starts = {}
    hinted = set()
    for h in sorted(o for o in CODE_HINTS if base <= o < base + 0x2000):
        o = h                                         # linear decode up to RTS/RTI/JMP
        while o < base + 0x2000 and prg[o] in OFFICIAL and prg[o] != 0x00:
            hinted.add(o)
            if prg[o] in (0x60, 0x40, 0x4C, 0x6C):
                break
            o += LENGTH[prg[o]]
    off = base
    while off < base + 0x2000:
        op = prg[off]
        n = LENGTH[op]
        if (kind[off] in CODE_CLASSES or off in hinted) and op in MN and op in OFFICIAL and off + n <= base + 0x2000:
            starts[off] = n
            off += n
        else:
            off += 1
    return starts


def operand_target(prg, off):
    op = prg[off]
    mode = MN[op][1]
    pc = window(off // 0x2000) + off % 0x2000
    if mode == "rel":
        rel = prg[off + 1] - 256 if prg[off + 1] >= 128 else prg[off + 1]
        return (pc + 2 + rel) & 0xFFFF
    if LENGTH[op] == 3:
        return prg[off + 1] | prg[off + 2] << 8
    return None


def table_items(prg, tables, insns_by_bank):
    """Per-offset directives for recognised pointer tables: off -> (size,
    kind, target cpu addr, plus, context bank). Split tables give one byte per
    entry in each half; word tables give two bytes per entry."""
    items = {}
    for t in tables:
        if t["type"] == "split":
            for k in range(t["count"]):
                lo, hi = t["lo"] + k, t["hi"] + k
                word = prg[lo] | prg[hi] << 8
                ctx = t["target_banks"][k] if "target_banks" in t else lo // 0x2000
                items[lo] = (1, "lobytes", (word + t["plus"]) & 0xFFFF, t["plus"], ctx)
                items[hi] = (1, "hibytes", (word + t["plus"]) & 0xFFFF, t["plus"], ctx)
        else:
            for k in range(t["count"]):
                off = t["start"] + 2 * k
                word = prg[off] | prg[off + 1] << 8
                items[off] = (2, "word", (word + t["plus"]) & 0xFFFF, t["plus"], off // 0x2000)
    return items


def emit_bank(prg, kind, bank, insns_by_bank, user_labels, items=None):
    items = items or {}
    base, org = bank * 0x2000, window(bank)
    starts = insns_by_bank[bank]
    # label targets: branches/jumps/reads that land on an instruction start or a table start here
    names = {}

    def name_for(b, addr):
        if (b, addr) in user_labels:
            return user_labels[(b, addr)][0]
        return f"L{b:02X}_{addr:04X}" if b == bank else f"F{b:02X}_{addr:04X}"

    refs = {}
    for off in starts:
        t = operand_target(prg, off)
        if t is None or t < 0x8000 or (prg[off] in WRITES and t in HW):
            continue
        b = owner(bank, t)
        if b is None:
            continue
        toff = b * 0x2000 + (t - window(b))
        boundary = toff in insns_by_bank[b] or kind[toff] in (TABLE, PTR) or (
            toff not in _inside(insns_by_bank[b]))
        if boundary:
            refs[off] = (b, t)
            if b == bank:
                names[t] = name_for(b, t)
    item_ref = {}
    for off, (size, how, target, plus, ctx) in items.items():
        if off // 0x2000 != bank:
            continue
        b = owner(ctx, target)
        if b is None:
            continue
        toff = b * 0x2000 + (target - window(b))
        if toff in insns_by_bank[b]:
            item_ref[off] = (b, target)
            refs.setdefault(("item", off), (b, target))
            if b == bank:
                names[target] = name_for(b, target)
    for (b, addr), (nm, _) in user_labels.items():
        if b == bank:
            names[addr] = nm
    for off in range(base, base + 0x2000):
        if kind[off] in (TABLE, PTR) and (off == base or kind[off - 1] not in (TABLE, PTR)):
            addr = org + off - base
            names.setdefault(addr, name_for(bank, addr))

    out = [f"; bank ${bank:02X} at ${org:04X} - generated by disasm/tools/disasm.py from your dump; do not edit",
           f'.segment "BANK{bank:02X}"', f".org ${org:04X}", ""]
    out += [f"{name} = ${addr:04X}" for addr, name in sorted(HW.items())]
    out += [f"{name} = ${addr:04X}" + (f"  ; {c}" if c else "") for name, addr, c in RAM_DEFS]
    out.append("")
    # other banks' labels this bank uses (fixed banks) are plain equates
    used_far = sorted({(b, t) for b, t in refs.values() if b != bank})
    refs = {k: v for k, v in refs.items() if not isinstance(k, tuple)}
    for b, t in used_far:
        out.append(f"{name_for(b, t)} = ${t:04X}")
    if used_far:
        out.append("")
    off = base
    pending = []
    while off < base + 0x2000:
        addr = org + off - base
        if addr in names:
            if pending:
                out.append("    .byte " + ",".join(pending))
                pending = []
            comment = user_labels.get((bank, addr), ("", ""))[1]
            out.append(f"{names[addr]}:" + (f"  ; {comment}" if comment else ""))
        size_item = items.get(off) if off // 0x2000 == bank else None
        if size_item and off not in starts and all(org + off - base + k not in names for k in range(1, size_item[0])):
            if pending:
                out.append("    .byte " + ",".join(pending))
                pending = []
            size, how, target, plus, ctx = size_item
            if off in item_ref:
                expr = name_for(*item_ref[off]) + (f"-{plus}" if plus else "")
            else:
                expr = f"${(target - plus) & 0xFFFF:04X}"
            out.append(f"    .{how} {expr}")
            off += size
            continue
        if off in starts:
            if pending:
                out.append("    .byte " + ",".join(pending))
                pending = []
            out.append("    " + format_insn(prg, off, refs.get(off), name_for))
            off += starts[off]
            continue
        pending.append(f"${prg[off]:02X}")
        if len(pending) == 16:
            out.append("    .byte " + ",".join(pending))
            pending = []
        off += 1
    if pending:
        out.append("    .byte " + ",".join(pending))
    return "\n".join(out) + "\n"


def _inside(starts):
    inside = set()
    for off, n in starts.items():
        inside.update(range(off + 1, off + n))
    return inside


def format_insn(prg, off, ref, name_for):
    op = prg[off]
    mn, mode = MN[op]
    mn = mn.lower()
    if mode == "imp":
        return mn
    if mode == "acc":
        return f"{mn} a"
    if mode == "rel":
        t = operand_target(prg, off)
        arg = name_for(*ref) if ref else f"${t:04X}"
        return f"{mn} {arg}"
    if LENGTH[op] == 2:
        v = prg[off + 1]
        if mode == "imm":
            return f"{mn} #${v:02X}"
        z = ram_operand(v) or f"${v:02X}"
        return {"zp": f"{mn} {z}", "zpx": f"{mn} {z},x", "zpy": f"{mn} {z},y",
                "izx": f"{mn} ({z},x)", "izy": f"{mn} ({z}),y"}[mode]
    v = prg[off + 1] | prg[off + 2] << 8
    if mode == "ind":                                # JMP (abs) has no zero-page form
        return f"{mn} ({name_for(*ref) if ref else ram_operand(v) or f'${v:04X}'})"
    if ref:
        arg = name_for(*ref)
    else:
        hw = HW.get(v) if (v < 0x8000 or op in WRITES) else None
        arg = hw or ram_operand(v) or (f"${v:02X}" if v < 0x100 else f"${v:04X}")
        if v < 0x100:
            arg = "a:" + arg
    return {"abs": f"{mn} {arg}", "abx": f"{mn} {arg},x", "aby": f"{mn} {arg},y"}[mode]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("rom")
    ap.add_argument("--map", required=True)
    ap.add_argument("--labels", default=os.path.join(HERE, "..", "labels.toml"))
    ap.add_argument("--out", default="build/disasm")
    ap.add_argument("--tables", help="pointer tables JSON from tools/cdl_report.py --tables-out")
    a = ap.parse_args()
    rom = open(a.rom, "rb").read()
    prg = rom[16:16 + rom[4] * 0x4000]
    kind = open(a.map, "rb").read()
    if len(kind) != len(prg):
        sys.exit("map size does not match PRG")
    labels = load_labels(a.labels)
    os.makedirs(a.out, exist_ok=True)
    banks = len(prg) // 0x2000
    insns = {b: decode_insns(prg, kind, b) for b in range(banks)}
    import json
    items = table_items(prg, json.load(open(a.tables)) if a.tables else [], insns)
    cfg = ["MEMORY {"]
    segs = ["SEGMENTS {"]
    for b in range(banks):
        open(os.path.join(a.out, f"bank{b:02X}.s"), "w", newline="\n").write(
            emit_bank(prg, kind, b, insns, labels, items))
        cfg.append(f"    B{b:02X}: start = ${window(b):04X}, size = $2000, fill = yes, file = %O;")
        segs.append(f"    BANK{b:02X}: load = B{b:02X}, type = ro;")
    open(os.path.join(a.out, "prg.cfg"), "w", newline="\n").write(
        "\n".join(cfg + ["}"] + segs + ["}"]) + "\n")
    n_insn = sum(len(v) for v in insns.values())
    print(f"{banks} banks, {n_insn} instructions, {len(labels)} user labels -> {a.out}")


if __name__ == "__main__":
    main()
