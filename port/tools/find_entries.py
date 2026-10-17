"""Entry points the port is missing: places the game can jump to that have no
routine in port/src/game, so the runtime runs them from the ROM with its
interpreter.

    py -3 -I port/tools/find_entries.py ROM [--toml-out F] [--names-out F] [-v]

Three sources, all static:

- jump tables: the P0 map's (runs/cdl/tables.json), and again from every
  JMP ($zz) of known code whose $zz/$zz+1 come from "LDA t,X|Y / STA zz"
  pairs just before it (split table: length up to where the high half
  starts, the map's count is only what runs used; word table: while the
  targets decode). A target counts when its first
  instructions decode as official opcodes, whatever the map says (enemies
  no run met are "unseen");
- return addresses pushed before a jump ("LDA #hi / PHA / LDA #lo / PHA"
  then JMP): the jumped-to code returns to the pushed address + 1;
- constant targets the port's own code dispatches by address
  (call_by_address_tail, mff_call_at, nes_dispatch_call), such as a branch
  to just before a routine's entry.

Prints the targets with no routine in the port, marking those the pure
scaffold's generator has no function for either (runs/p4/pure/generated):
--toml-out writes those as [[extra_func]] for port/scaffold/game.base.toml,
--names-out writes every missing name for port/tools/lift_bank.py --extra.
"""
import argparse
import glob
import json
import os
import re
import sys

ROOT = os.path.normpath(os.path.join(os.path.dirname(__file__), "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from cdl_report import CLASSES, LENGTH, OFFICIAL, resolve, window  # noqa: E402

CODE = {CLASSES.index("opcode"), CLASSES.index("code?")}
FUNC = re.compile(r"^void (func_[0-9A-F]{4}(?:_b\d)?)\(void\)\s*\{", re.M)
CONST = re.compile(r"(?:call_by_address_tail|nes_dispatch_call)\(0x([0-9A-F]{4}), (-?\d+)\)"
                   r"|mff_call_at\(0x[0-9A-F]+, 0x([0-9A-F]{4}), (-?\d+)\)"
                   r"|call_by_address_tail\(\(uint16_t\)\(g_code_window_base \+ 0x([0-9A-F]{4})\), (-?\d+)\)")


def legacy(off):
    """PRG offset -> (legacy 16 KiB bank, CPU address), as legacy_config.py."""
    b8 = off // 0x2000
    if b8 >= 0x0E:
        return 7, 0xC000 + (b8 - 0x0E) * 0x2000 + off % 0x2000
    return b8 >> 1, 0x8000 + (b8 & 1) * 0x2000 + off % 0x2000


def name(off):
    b, a = legacy(off)
    return f"func_{a:04X}" if b == 7 else f"func_{a:04X}_b{b}"


def unlegacy(bank, addr):
    """(legacy bank, CPU address) -> PRG offset."""
    if bank == 7:
        return 0x0E * 0x2000 + (addr - 0xC000)
    return (2 * bank + (1 if addr >= 0xA000 else 0)) * 0x2000 + (addr & 0x1FFF)


def decodes(prg, off):
    """The first instructions at off are official opcodes inside one 8 KiB bank."""
    end = (off // 0x2000 + 1) * 0x2000
    for _ in range(4):
        if off >= end or prg[off] not in OFFICIAL or prg[off] == 0x00:
            return False
        if prg[off] in (0x40, 0x60, 0x4C, 0x6C):
            return True
        off += LENGTH[prg[off]]
    return True


def functions(paths):
    out = set()
    for p in paths:
        out |= set(FUNC.findall(open(p).read()))
    return out


class Jump:
    """One way the game jumps somewhere no JSR names: where (BB:AAAA), what,
    the CPU addresses the code reads the table at (empty for a push), and the
    PRG offsets of the targets."""
    def __init__(self, where, what, reads, targets):
        self.where, self.what, self.reads, self.targets = where, what, reads, sorted(set(targets))


def jumps(prg, kind, tables_path):
    """Every jump table (the P0 map's, then again from each JMP ($zz) site) and
    every return address pushed before a jump."""
    def cpu(off):
        return window(off // 0x2000) + off % 0x2000

    out = []
    banked = set()              # tables whose targets' banks depend on the entry
    for t in json.load(open(tables_path)):
        n, plus, offs = t.get("count", 0), t.get("plus", 0), []
        if t.get("type") == "split":
            if "target_banks" in t:
                banked.add(t["lo"])
            for k in range(n):
                w = ((prg[t["lo"] + k] | prg[t["hi"] + k] << 8) + plus) & 0xFFFF
                ctx = t["target_banks"][k] if "target_banks" in t else t["lo"] // 0x2000
                o = ctx * 0x2000 + (w - 0xA000) if 0xA000 <= w < 0xC000 and "target_banks" in t else resolve(ctx, w)
                if o is not None and decodes(prg, o):
                    offs.append(o)
            where, reads = f"{t['lo'] // 0x2000:02X}:{cpu(t['lo']):04X}", {cpu(t["lo"]), cpu(t["hi"])}
        elif "start" in t:
            for k in range(n):
                w = ((prg[t["start"] + 2 * k] | prg[t["start"] + 2 * k + 1] << 8) + plus) & 0xFFFF
                o = resolve(t["start"] // 0x2000, w)
                if o is not None and decodes(prg, o):
                    offs.append(o)
            where, reads = f"{t['start'] // 0x2000:02X}:{cpu(t['start']):04X}", {cpu(t["start"]), cpu(t["start"]) + 1}
        else:
            continue
        out.append(Jump(where, f"P0 table {t.get('name', '')} x{n}", reads, offs))

    def pops_return(t):
        """Does the routine at t take its return address off the stack into a
        zero-page pointer (PLA / STA zz / PLA / STA zz+1) and jump through it:
        a JSR to it is followed by an inline jump table (snd_jump_inline)."""
        seen, o, zz = [], t, None
        for _ in range(24):
            if o >= len(prg) or prg[o] not in OFFICIAL:
                return False
            seen.append(o)
            if prg[o] == 0x6C:
                break
            o += LENGTH[prg[o]]
        ops = [prg[s] for s in seen]
        for k in range(len(ops) - 3):
            if ops[k] == 0x68 and ops[k + 1] == 0x85 and ops[k + 2] == 0x68 and ops[k + 3] == 0x85:
                zz = prg[seen[k + 1] + 1]
                if prg[seen[k + 3] + 1] == zz + 1:
                    return ops[-1] == 0x6C
        return False

    inline = {}
    for b8 in range(len(prg) // 0x2000):
        for o in range(b8 * 0x2000, (b8 + 1) * 0x2000 - 2):
            if kind[o] in CODE and prg[o] == 0x20:
                t = resolve(b8, prg[o + 1] | prg[o + 2] << 8)
                if t is not None and t not in inline:
                    inline[t] = pops_return(t)
    for b8 in range(len(prg) // 0x2000):
        for o in range(b8 * 0x2000, (b8 + 1) * 0x2000 - 2):
            if not (kind[o] in CODE and prg[o] == 0x20):
                continue
            t = resolve(b8, prg[o + 1] | prg[o + 2] << 8)
            if not inline.get(t):
                continue
            # the table right after the JSR; its length from an AND #2^k-1 just before, else while it decodes
            count = 64
            if o >= 2 and prg[o - 2] == 0x29 and (prg[o - 1] + 1) & prg[o - 1] == 0:
                count = prg[o - 1] + 1
            offs, tab = [], o + 3
            for k in range(count):
                if tab + 2 * k + 1 >= (b8 + 1) * 0x2000 or kind[tab + 2 * k] in CODE:
                    break                       # the table ends where code starts
                tt = resolve(b8, prg[tab + 2 * k] | prg[tab + 2 * k + 1] << 8)
                if tt is None or not decodes(prg, tt):
                    if count == 64:
                        break
                    continue
                offs.append(tt)
            out.append(Jump(f"{b8:02X}:{cpu(o):04X}", f"inline table after JSR ${cpu(t):04X} x{len(offs)}",
                            set(), offs))

    for b8 in range(len(prg) // 0x2000):
        starts = [o for o in range(b8 * 0x2000, (b8 + 1) * 0x2000) if kind[o] in CODE]
        for i, o in enumerate(starts):
            op = prg[o]
            where = f"{b8:02X}:{cpu(o):04X}"
            if op == 0x6C:                                   # JMP (ind)
                zz = prg[o + 1] | prg[o + 2] << 8
                src = {}
                for k in range(i - 1, max(i - 14, 0), -1):
                    s = starts[k]
                    if prg[s] in (0x85, 0x8D):
                        dst = prg[s + 1] | (prg[s + 2] << 8 if prg[s] == 0x8D else 0)
                        p = starts[k - 1]
                        if dst in (zz, zz + 1) and dst not in src and prg[p] in (0xB9, 0xBD) and p + 3 == s:
                            src[dst] = prg[p + 1] | prg[p + 2] << 8
                if len(src) < 2:
                    continue
                lo, hi = src[zz], src[zz + 1]
                lo_o, hi_o = resolve(b8, lo), resolve(b8, hi)
                if lo_o is None or hi_o is None or lo_o in banked:
                    continue
                offs = []
                word = hi == lo + 1
                count = 64 if word or not 0 < hi - lo <= 128 else hi - lo
                for k in range(count):
                    w = prg[lo_o + 2 * k] | prg[lo_o + 2 * k + 1] << 8 if word else prg[lo_o + k] | prg[hi_o + k] << 8
                    t = resolve(b8, w)
                    if t is None or not decodes(prg, t):
                        break
                    offs.append(t)
                what = f"JMP (${zz:04X}) " + (f"word table ${lo:04X}" if word else f"split table ${lo:04X}/${hi:04X}")
                out.append(Jump(where, f"{what} x{len(offs)}", {lo, hi}, offs))
            if op == 0x48 and i >= 3:                        # ... PHA: a pushed return address
                p = starts[i - 3:i + 1]
                if [prg[x] for x in p] == [0xA9, 0x48, 0xA9, 0x48] and i + 1 < len(starts) \
                        and prg[starts[i + 1]] in (0x4C, 0x6C):
                    t = resolve(b8, (prg[p[0] + 1] << 8 | prg[p[2] + 1]) + 1)
                    if t is not None:
                        out.append(Jump(where, "pushed return", set(), [t]))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("rom")
    ap.add_argument("--map", default=os.path.join(ROOT, "runs", "cdl", "prgmap.bin"))
    ap.add_argument("--tables", default=os.path.join(ROOT, "runs", "cdl", "tables.json"))
    ap.add_argument("--generated", default=os.path.join(ROOT, "runs", "p4", "pure", "generated"))
    ap.add_argument("--toml-out")
    ap.add_argument("--names-out")
    ap.add_argument("-v", action="store_true", help="print every table and push, not just the gaps")
    a = ap.parse_args()
    rom = open(a.rom, "rb").read()
    prg = rom[16:16 + rom[4] * 0x4000]
    kind = open(a.map, "rb").read()
    gen = functions(glob.glob(os.path.join(a.generated, "*.c")))
    port_files = sorted(glob.glob(os.path.join(ROOT, "port", "src", "game", "*.c")))
    port = functions(port_files + [os.path.join(ROOT, "port", "scaffold", "extras.c")])

    found = {}                                   # name -> where it comes from

    def add(where, what, offs):
        names = []
        for o in offs:
            n = name(o)
            found.setdefault(n, where)
            names.append(n + ("" if n in port else " (not in port)" if n in gen else " (no function)"))
        if a.v or any(n not in port for n in (name(o) for o in offs)):
            print(f"{where}  {what}: {', '.join(names)}")

    for j in jumps(prg, kind, a.tables):
        add(j.where, j.what, j.targets)

    # constant targets in the port's code
    for path in port_files:
        current = None
        for line in open(path):
            m = FUNC.match(line)
            if m:
                current = m.group(1)
            for m in CONST.finditer(line):
                if m.group(5):                   # relative to the routine's own 8 KiB window
                    addr = (int(current[5:9], 16) & 0xE000) | int(m.group(5), 16)
                    bank = int(m.group(6))
                else:
                    addr = int(m.group(1) or m.group(3), 16)
                    bank = int(m.group(2) or m.group(4))
                if addr < 0x8000:
                    continue
                if addr >= 0xC000:
                    bank = 7
                elif bank < 0:
                    mb = re.search(r"_b(\d)$", current or "")
                    if not mb:
                        continue
                    bank = int(mb.group(1))
                add(f"{os.path.basename(path)}:{current}", f"dispatch to ${addr:04X}", [unlegacy(bank, addr)])

    missing = sorted(n for n in found if n not in port)
    nogen = [n for n in missing if n not in gen]
    print(f"{len(found)} entries; {len(missing)} not in the port, {len(nogen)} of them with no generated function")
    if a.names_out:
        open(a.names_out, "w", newline="\n").write("\n".join(missing) + "\n")
    if a.toml_out:
        out = []
        for n in nogen:
            m = re.match(r"func_([0-9A-F]{4})(?:_b(\d))?$", n)
            out += [f"[[extra_func]]   # {found[n]}", f"bank = {m.group(2) or 7}", f"addr = 0x{m.group(1)}", ""]
        open(a.toml_out, "w", encoding="utf-8", newline="\n").write("\n".join(out))


if __name__ == "__main__":
    main()
