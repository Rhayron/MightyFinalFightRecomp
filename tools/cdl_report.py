"""Merge code/data logs (--cdl-out) and map what every ROM byte is.

    py -3 -I tools/cdl_report.py ROM OUT.cdl RUN.cdl [RUN.cdl ...] [--seeds cyc_seeds.txt] [--md REPORT.md]

OUT.cdl is the OR of OUT.cdl (if present) and every RUN.cdl. The report then
classifies each PRG byte, per 8 KiB bank:

  opcode   the CPU fetched an opcode there (CODE flag)
  operand  the bytes after an opcode, by the 6502's instruction lengths
  data     read as data and not part of an instruction
  pcm      DMC sample bytes
  fill     unread runs of one repeated byte, 16 or longer (padding)
  unseen   everything else: data or code no run reached, still to explain

and cross-checks the seed file: every seed (an instruction a run executed on
the interpreter) must be an opcode byte in the map. CHR is reported per 1 KiB
page as drawn or not. The ROM is read only locally; the report holds counts
and address ranges, never ROM bytes.
"""
import argparse
import sys
import zlib

CODE, DATA, PCM = 0x01, 0x02, 0x40
CLASSES = ["opcode", "operand", "code?", "operand?", "data", "table", "ptr", "pcm", "fill", "unseen"]
SLICE = [2, 2, 1, 2, 2, 2, 2, 2, 1, 3, 1, 3, 3, 3, 3, 3]          # $x1-$xF rows of odd high nibble
ROWS = {0x0: [1, 2, 1, 2, 2, 2, 2, 2, 1, 2, 1, 2, 3, 3, 3, 3],
        0x2: [3, 2, 1, 2, 2, 2, 2, 2, 1, 2, 1, 2, 3, 3, 3, 3],
        0x4: [1, 2, 1, 2, 2, 2, 2, 2, 1, 2, 1, 2, 3, 3, 3, 3],
        0x6: [1, 2, 1, 2, 2, 2, 2, 2, 1, 2, 1, 2, 3, 3, 3, 3],
        0x8: [2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 3, 3, 3, 3],
        0xA: [2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 3, 3, 3, 3],
        0xC: [2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 3, 3, 3, 3],
        0xE: [2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 3, 3, 3, 3]}
LENGTH = [ROWS[op >> 4][op & 15] if op >> 4 in ROWS else SLICE[op & 15] for op in range(256)]
LENGTH[0x00] = 2   # BRK skips a signature byte


OFFICIAL = set(bytes.fromhex(
    "00010506 08090A0D 0E101115 1618191D 1E202124 2526282A 292C2D2E 30313536 38393D3E 40414546 48494A4C"
    "4D4E5051 55565859 5D5E6061 65666869 6A6C6D6E 70717576 78797D7E 81848586 888A8C8D 8E909194 95969899"
    "9A9DA0A1 A2A4A5A6 A8A9AAAC ADAEB0B1 B4B5B6B8 B9BABCBD BEC0C1C4 C5C6C8C9 CACCCDCE D0D1D5D6 D8D9DDDE"
    "E0E1E4E5 E6E8E9EA ECEDEEF0 F1F5F6F8 F9FDFE".replace(" ", "")))
BRANCH = {0x10, 0x30, 0x50, 0x70, 0x90, 0xB0, 0xD0, 0xF0}
STOP = {0x40, 0x60, 0x4C, 0x6C}                     # RTI RTS JMP JMP()
NOT_READ = {0x20, 0x4C, 0x6C, 0x8C, 0x8D, 0x8E, 0x99, 0x9D,
            0x0E, 0x1E, 0x2E, 0x3E, 0x4E, 0x5E, 0x6E, 0x7E, 0xCE, 0xDE, 0xEE, 0xFE}


def window(bank):
    """CPU address of an 8 KiB bank as the game uses it (MMC3 mode 0): code
    banks by the seeds (even $8000, odd $A000, fixed $0E/$0F), data banks by
    who maps them - $02 is the common enemies' data, put at $A000 by
    objects_update for ids $20-$2F; $0A is mapped at $8000 ($C?? lda #$0A /
    bank_request); $07 is round 3's $A000 bank (round_bank)."""
    return {0x0E: 0xC000, 0x0F: 0xE000, 0x02: 0xA000}.get(bank, 0xA000 if bank & 1 else 0x8000)


def resolve(bank, addr):
    """PRG offset of CPU address `addr` seen from code in `bank` (pairs: 2k at
    $8000 with 2k+1 at $A000), or None."""
    if addr < 0x8000:
        return None
    if addr >= 0xE000:
        return 0x0F * 0x2000 + (addr - 0xE000)
    if addr >= 0xC000:
        return 0x0E * 0x2000 + (addr - 0xC000)
    if bank >= 0x0E:
        return None                                  # fixed code into a switchable window: bank unknown
    base = bank & ~1
    other = base + (1 if addr >= 0xA000 else 0)
    return other * 0x2000 + (addr & 0x1FFF)


def static_flow(prg, kind, cdl, extra=None):
    """Grow the map from the evidence: code reachable from executed code,
    tables that code reads, and pointer tables into code. Marks "code?" /
    "operand?" (static, not executed), "table" (the first byte of a table an
    absolute read points at) and "ptr" (words of a code pointer table)."""
    n = len(prg)
    found = {"code": 0, "tables": 0, "ptr_tables": 0, "stops": 0}
    assert len(OFFICIAL) == 151
    visited = set()

    def flood(starts):
        work = list(starts)
        while work:
            off = work.pop()
            while 0 <= off < n and off not in visited:
                visited.add(off)
                op = prg[off]
                length = LENGTH[op]
                bank = off // 0x2000
                if (op not in OFFICIAL or op == 0x00 or kind[off] == "operand" or
                        off // 0x2000 != (off + length - 1) // 0x2000 or
                        any(kind[off + k] in ("data", "pcm", "table", "ptr")
                            and not cdl[off + k] & CODE for k in range(length))):
                    found["stops"] += 1
                    break
                if kind[off] not in ("opcode",):
                    kind[off] = "code?"
                    found["code"] += 1
                for k in range(1, length):
                    if kind[off + k] in ("", "unseen", "data"):
                        kind[off + k] = "operand?" if kind[off] == "code?" else "operand"
                pc = window(bank) + off % 0x2000
                if op in BRANCH:
                    rel = prg[off + 1] - 256 if prg[off + 1] >= 128 else prg[off + 1]
                    t = resolve(bank, (pc + 2 + rel) & 0xFFFF)
                    if t is not None:
                        work.append(t)
                elif length == 3:
                    target = prg[off + 1] | prg[off + 2] << 8
                    t = resolve(bank, target)
                    if op in (0x20, 0x4C) and t is not None:
                        work.append(t)
                    elif op not in NOT_READ and t is not None and kind[t] in ("", "data"):
                        kind[t] = "table"
                        found["tables"] += 1
                if op in STOP:
                    break
                off += length

    flood([i for i in range(n) if kind[i] == "opcode"])
    extra_tables = []
    if extra:
        # game-specific dispatch tables (tools/mff_dispatch.py): their bytes are
        # pointers and their targets are code, wherever the table says
        for t in extra.get("tables", []):
            for k in range(t["count"]):
                for a in (t["lo"] + k, t["hi"] + k):
                    if kind[a] in ("", "data", "table"):
                        kind[a] = "ptr"
            extra_tables.append(t)
        new = [o for o in extra.get("starts", []) if kind[o] not in ("opcode", "code?")]
        found["extra_starts"] = len(new)
        flood(new)

    # Split pointer tables: a dispatch that loads a pointer's low and high
    # bytes from two tables with the same index (LDA lo,X ... LDA hi,X) and
    # then jumps through it (JMP ($zp)) or returns into it (PHA PHA RTS).
    split_seen = {}
    found["split_targets"] = 0
    for _round in range(3):                          # new code can hold new dispatches
        insns = sorted(i for i in range(n) if kind[i] in ("opcode", "code?"))
        new_starts = []
        for idx, off in enumerate(insns):
            if prg[off] not in (0xBD, 0xB9):         # LDA abs,X / LDA abs,Y
                continue
            bank = off // 0x2000
            seq = insns[idx:idx + 5]
            if len(seq) < 5 or any(o // 0x2000 != bank for o in seq):
                continue
            ops = [prg[o] for o in seq]
            t1 = prg[seq[0] + 1] | prg[seq[0] + 2] << 8
            if ops == [ops[0], 0x48, ops[0], 0x48, 0x60]:
                # LDA hi,X / PHA / LDA lo,X / PHA / RTS: returns to word + 1
                t2 = prg[seq[2] + 1] | prg[seq[2] + 2] << 8
                hi_t, lo_t, plus = t1, t2, 1
            elif ops[1] == 0x85 and ops[2] == ops[0] and ops[3] == 0x85 and ops[4] == 0x6C:
                # LDA lo,X / STA zp / LDA hi,X / STA zp+1 / JMP (zp)
                zp = prg[seq[1] + 1]
                if prg[seq[3] + 1] != zp + 1 or (prg[seq[4] + 1] | prg[seq[4] + 2] << 8) != zp:
                    continue
                t2 = prg[seq[2] + 1] | prg[seq[2] + 2] << 8
                lo_t, hi_t, plus = t1, t2, 0
            else:
                continue
            lo, hi = resolve(bank, lo_t), resolve(bank, hi_t)
            if lo is None or hi is None or lo == hi:
                continue
            entries = 0
            for k in range(256):
                a_lo, a_hi = lo + k, hi + k
                if a_lo >= n or a_hi >= n or (k and (a_lo == hi or a_hi == lo)):
                    break
                if kind[a_lo] in ("opcode", "operand", "code?", "operand?", "fill"):
                    break
                t = resolve(a_lo // 0x2000, (prg[a_lo] | prg[a_hi] << 8) + plus)
                if (t is None or prg[t] not in OFFICIAL or prg[t] == 0x00 or
                        (kind[t] in ("data", "pcm", "table", "ptr", "operand") and not cdl[t] & CODE)):
                    break
                for a in (a_lo, a_hi):
                    if kind[a] in ("", "data", "table"):
                        kind[a] = "ptr"
                if kind[t] not in ("opcode", "code?"):
                    new_starts.append(t)
                entries += 1
            if entries >= 2:
                split_seen[(lo, hi, plus)] = max(entries, split_seen.get((lo, hi, plus), 0))
        found["split_targets"] += len(new_starts)
        if not new_starts:
            break
        flood(new_starts)
    found["split_tables"] = len(split_seen)
    found["tables_info"] = extra_tables + [{"type": "split", "lo": lo, "hi": hi, "plus": plus, "count": c}
                                           for (lo, hi, plus), c in sorted(split_seen.items())]

    # Code pointer tables: runs of words (or word+1, the RTS trick) read as
    # data whose targets are mostly known code; their other entries are code.
    code = {i for i in range(n) if kind[i] in ("opcode", "code?")}
    for rts in (0, 1):
        i = 0
        while i + 1 < n:
            if kind[i] not in ("data", "table"):
                i += 1
                continue
            j, hits, words = i, 0, []
            while j + 1 < n and kind[j] in ("data", "table") and kind[j + 1] in ("data", "table"):
                t = resolve(j // 0x2000, (prg[j] | prg[j + 1] << 8) + rts)
                words.append(t)
                hits += t in code
                j += 2
            if len(words) >= 3 and hits >= 0.75 * len(words):
                found["ptr_tables"] += 1
                found["tables_info"].append({"type": "words", "start": i, "plus": rts, "count": len(words)})
                for k in range(i, j):
                    kind[k] = "ptr"
                new = [t for t in words if t is not None and t not in code]
                flood(new)
                code |= {t for t in words if t is not None}
            i = max(j, i + 1)
    return found


def ranges(flags, want, minimum=1):
    out, start = [], None
    for i, f in enumerate(flags + [None]):
        if f == want and start is None:
            start = i
        elif f != want and start is not None:
            if i - start >= minimum:
                out.append((start, i - 1))
            start = None
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("rom")
    ap.add_argument("out")
    ap.add_argument("runs", nargs="*")
    ap.add_argument("--seeds")
    ap.add_argument("--md")
    ap.add_argument("--static", action="store_true", help="grow the map by control flow and table references")
    ap.add_argument("--map-out", help="one class byte per PRG byte, for the disassembler")
    ap.add_argument("--tables-out", help="JSON list of the pointer tables the static pass recognised")
    ap.add_argument("--extra", help="game-specific dispatch tables and code starts (tools/mff_dispatch.py)")
    a = ap.parse_args()

    rom = open(a.rom, "rb").read()
    prg_len, chr_len = rom[4] * 0x4000, rom[5] * 0x2000
    prg = rom[16:16 + prg_len]
    print(f"ROM CRC32 {zlib.crc32(rom):08X}, PRG {prg_len}, CHR {chr_len}")

    cdl = bytearray(prg_len + chr_len)
    for path in [a.out] + a.runs:
        try:
            data = open(path, "rb").read()
        except FileNotFoundError:
            continue
        if len(data) != len(cdl):
            sys.exit(f"{path}: {len(data)} bytes, expected {len(cdl)}")
        for i, b in enumerate(data):
            cdl[i] |= b
    open(a.out, "wb").write(cdl)

    kind = [""] * prg_len
    for i in range(prg_len):
        if cdl[i] & CODE:
            kind[i] = "opcode"
    # Seeds are instruction starts some run executed (--miss-log), possibly in
    # a session these logs did not replay: code by evidence, like CODE flags.
    seed_offsets = []
    if a.seeds:
        for line in open(a.seeds):
            parts = line.split()[0].split(":") if line.startswith("4k:") else []
            if parts:
                seed_offsets.append(int(parts[1], 16) * 0x1000 + (int(parts[2], 16) & 0xFFF))
    seed_only = 0
    for off in seed_offsets:
        if off < prg_len and kind[off] != "opcode":
            kind[off] = "opcode"
            seed_only += 1
    both = sum(1 for i in range(prg_len) if kind[i] == "opcode" and cdl[i] & DATA)
    for i in range(prg_len):
        if kind[i] == "opcode":
            for k in range(1, LENGTH[prg[i]]):
                if i + k < prg_len and kind[i + k] != "opcode":
                    kind[i + k] = "operand"
    for i in range(prg_len):
        if not kind[i]:
            kind[i] = "pcm" if cdl[i] & PCM else "data" if cdl[i] & DATA else ""
    import json
    static = static_flow(prg, kind, cdl, json.load(open(a.extra)) if a.extra else None) if a.static else {}
    i = 0
    while i < prg_len:                         # padding: long runs of one unread byte
        j = i
        while j < prg_len and not kind[j] and prg[j] == prg[i]:
            j += 1
        if j - i >= 16:
            for k in range(i, j):
                kind[k] = "fill"
        i = max(j, i + 1)
    for i in range(prg_len):
        kind[i] = kind[i] or "unseen"

    lines = ["# PRG/CHR map", "",
             f"From {len(a.runs)} run log(s) merged into `{a.out}`. Counts in bytes per 8 KiB PRG bank "
             "(`$8000`-relative offsets inside the bank).", "",
             "| Bank | " + " | ".join(CLASSES) + " |", "|---|" + "---:|" * len(CLASSES)]
    total = {}
    for bank in range(prg_len // 0x2000):
        part = kind[bank * 0x2000:(bank + 1) * 0x2000]
        row = {k: part.count(k) for k in CLASSES}
        for k, v in row.items():
            total[k] = total.get(k, 0) + v
        lines.append(f"| {bank:02X} | " + " | ".join(str(row[k]) for k in row) + " |")
    lines.append("| **all** | " + " | ".join(f"**{total[k]}**" for k in total) + " |")
    pct = 100 * (prg_len - total["unseen"]) / prg_len
    lines += ["", f"Classified: **{pct:.2f}%** of PRG ({total['unseen']} bytes unseen).", ""]
    if static:
        lines += [f"Static pass: {static['code']} instructions reached by control flow but never executed "
                  f"(`code?`), {static['tables']} table starts from absolute reads, {static['ptr_tables']} code "
                  f"pointer tables (`ptr`), {static['split_tables']} split lo/hi dispatch tables leading to "
                  f"{static['split_targets']} new entry points, {static['stops']} paths stopped at an illegal opcode, BRK, a bank "
                  "edge or bytes read as data.", ""]
    if a.map_out:
        open(a.map_out, "wb").write(bytes(CLASSES.index(k) for k in kind))
    if a.tables_out and static:
        json.dump(static["tables_info"], open(a.tables_out, "w"), indent=1)

    if a.seeds:
        lines += [f"Opcode bytes known only from the seed file (executed in a session these logs did not "
                  f"replay): **{seed_only}** of {len(seed_offsets)} seeds.",
                  f"Opcode bytes also read as data: **{both}** (dummy reads, or code that reads itself).", ""]

    lines += ["## Unseen PRG ranges (16 bytes or longer)", "", "| Bank | From | To | Bytes |", "|---|---|---|---:|"]
    for s, e in ranges(kind, "unseen", 16):
        bank = s // 0x2000
        lines.append(f"| {bank:02X} | `{s % 0x2000:04X}` | `{e % 0x2000:04X}` | {e - s + 1} |")
    short = sum(e - s + 1 for s, e in ranges(kind, "unseen") if e - s + 1 < 16)
    lines += ["", f"Plus {short} unseen bytes in runs shorter than 16 (gaps between tables, odd padding).", ""]

    if chr_len:
        lines += ["## CHR (1 KiB pages)", "", "| Page | drawn bytes |", "|---|---:|"]
        drawn_total = 0
        for page in range(chr_len // 0x400):
            n = sum(1 for b in cdl[prg_len + page * 0x400:prg_len + (page + 1) * 0x400] if b)
            drawn_total += n
            lines.append(f"| {page:02X} | {n} |")
        lines += ["", f"CHR drawn: **{drawn_total}** of {chr_len} bytes "
                      f"({100 * drawn_total / chr_len:.1f}%). Undrawn bytes inside a drawn page are usually "
                      "blank tile rows; whole undrawn pages are graphics no run showed.", ""]

    text = "\n".join(lines) + "\n"
    if a.md:
        open(a.md, "w", encoding="utf-8", newline="\n").write(text)
    print("\n".join(lines[:8 + prg_len // 0x2000 + 4]))


if __name__ == "__main__":
    main()
