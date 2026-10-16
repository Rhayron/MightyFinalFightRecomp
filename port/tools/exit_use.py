"""Which registers and flags really cross each routine's entry and exits.

    py -3 -I port/tools/exit_use.py ROM [--out runs/p4/exit_use.json]

The lifter makes every routine take A, X, Y and N Z C V from g_cpu at entry
and leave all of them in g_cpu at every exit and call, in case someone reads
them. Most of that is never read. This computes, for every routine, two
summaries by an interprocedural liveness fixpoint over all of them (lifted
as lift_bank.py would):

- entry: the registers and flags the routine (or what it calls with them)
  reads before writing them;
- exit:  the ones a caller can read after it returns.

In a routine's own liveness, a sync at an exit reads the routine's exit
summary, a sync before a call or a tail call reads the callee's entry
summary, and after a call the caller's reads make up the callee's exit
summary (a tail call passes the routine's exit summary on to the callee).

Conservative wherever the callers are not visible: routines in a jump
table (port/tools/find_entries.py), the vectors, the IRQ handlers,
nmi_tail and task entry points need everything at entry and exit; an
indirect jump or a yield sends everything; a call to $8000-$BFFF whose bank
is not known counts for that address in every bank; a routine nobody calls
directly needs everything.
"""
import argparse
import glob
import json
import os
import re
import sys

ROOT = os.path.normpath(os.path.join(os.path.dirname(__file__), "..", ".."))
sys.path.insert(0, os.path.dirname(__file__))
import find_entries  # noqa: E402
import lift  # noqa: E402
import lift_opt  # noqa: E402

REGS = ("a", "x", "y", "n", "z", "c", "v")
ALL = frozenset(REGS)
TEMPS = frozenset(("m", "t", "w"))
CALL_AT = re.compile(r"mff_call_at\(0x[0-9A-F]+, 0x([0-9A-F]{4}), (-?\d+)\)")
CALL_FN = re.compile(r"mff_call_fn\(0x[0-9A-F]+, (func_[0-9A-F]{4}(?:_b\d)?),")
TAIL_ADDR = re.compile(r"call_by_address_tail\(0x([0-9A-F]{4}), (-?\d+)\)")
TAIL_WIN = re.compile(r"call_by_address_tail\(\(uint16_t\)\(g_code_window_base \+ 0x([0-9A-F]{4})\), (-?\d+)\)")
TAIL_FN = re.compile(r"\b(func_[0-9A-F]{4}(?:_b\d)?)\(\);")


def name_of(bank, addr):
    return f"func_{addr:04X}" if bank == lift.FIXED else f"func_{addr:04X}_b{bank}"


def by_addr(addr, routines):
    if addr >= 0xC000:
        n = name_of(lift.FIXED, addr)
        return [n] if n in routines else []
    return [n for n in (name_of(b, addr) for b in range(lift.FIXED)) if n in routines]


class Routine:
    def __init__(self, name, bank, addr, body, routines):
        self.name, self.bank, self.addr = name, bank, addr
        lines = []
        for s in body:
            lines.extend(lift_opt.atomize(s))
        self.lines = lines
        self.succ = lift_opt.successors(lines)
        window = addr & 0xE000
        # what each sync-out feeds: ("exit",), ("call", [callees]), ("tail", [targets]), ("all",)
        self.sync = {}
        self.calls = []                 # (index of the SYNC_IN after the call, [callees])
        self.tails = []
        n = len(lines)
        for i, s in enumerate(lines):
            tails = [t for m in TAIL_ADDR.finditer(s) for t in by_addr(int(m.group(1), 16), routines)]
            tails += [name_of(bank, window | int(m.group(1), 16)) for m in TAIL_WIN.finditer(s)
                      if name_of(bank, window | int(m.group(1), 16)) in routines]
            if s.startswith("if (") and "SYNC_OUT" in s:          # a branch that leaves
                self.sync[i] = ("tail", tails) if tails else ("all",)
                self.tails += tails
                continue
            if not s.startswith("SYNC_OUT"):
                continue
            j = i + 1
            nxt = lines[j] if j < n else ""
            m1, m2 = CALL_AT.search(nxt), CALL_FN.search(nxt)
            if m1 or m2:
                callees = by_addr(int(m1.group(1), 16), routines) if m1 else \
                    ([m2.group(1)] if m2.group(1) in routines else [])
                follow = j + 1 < n and lines[j + 1].startswith("SYNC_IN")
                self.sync[i] = ("call", callees) if callees and follow else ("all",)
                if follow:
                    self.calls.append((j + 1, callees))
                continue
            if nxt.startswith("mff_rts"):
                self.sync[i] = ("exit",)
                continue
            if nxt.startswith("mff_rti"):
                self.sync[i] = ("rti",)        # the interrupted code gets its own registers back
                continue
            t2 = [t for m in TAIL_ADDR.finditer(nxt) for t in by_addr(int(m.group(1), 16), routines)]
            t2 += [m.group(1) for m in TAIL_FN.finditer(nxt) if m.group(1) in routines and nxt.rstrip().endswith("return;")]
            if t2 and "jt" not in nxt:
                self.sync[i] = ("tail", t2)
                self.tails += t2
                continue
            if t2 and "call_by_address(jt)" in nxt:
                self.tails += t2        # the continuation the jumped-to code returns to
            # (whatever the jump goes to gets everything)
            self.sync[i] = ("all",)                               # JMP (), yield, unknown

    def liveness(self, entry, exit_):
        """live_in per line, with syncs reading the summaries."""
        lines, n = self.lines, len(self.lines)
        infos = []
        for i, s in enumerate(lines):
            d, u, kind = lift_opt.info(s)
            if CALL_AT.search(s) or CALL_FN.search(s):
                u = set()                   # the registers went out in the sync before it
            if i in self.sync:
                k = self.sync[i]
                if k[0] == "exit":
                    use = exit_[self.name]
                elif k[0] == "rti":
                    use = frozenset()
                elif k[0] == "call" or k[0] == "tail":
                    use = frozenset().union(*(entry[c] for c in k[1])) if k[1] else ALL
                    if k[0] == "tail":
                        use = use | frozenset()     # the target returns for us: its exits are ours
                else:
                    use = ALL
                if s.startswith("if ("):
                    use = use | (u - ALL) | (u & frozenset(lift_opt.WORD.findall(s.split("{")[0])))
                u = set(use) | (u & TEMPS)
            infos.append((d, u))
        live_in = [set() for _ in range(n)]
        changed = True
        while changed:
            changed = False
            for i in range(n - 1, -1, -1):
                out = set()
                for j in self.succ[i]:
                    out |= live_in[j]
                d, u = infos[i]
                li = (out - d) | u
                if li != live_in[i]:
                    live_in[i] = li
                    changed = True
        return live_in


class Result:
    pass


def analyze(rom):
    # every routine the port has (port/src/game and the scaffold's extras.c),
    # lifted again from the pure scaffold
    entries = set()
    paths = glob.glob(os.path.join(ROOT, "port", "src", "game", "*.c"))
    for path in paths + [os.path.join(ROOT, "port", "scaffold", "extras.c")]:
        for m in re.finditer(r"^void func_([0-9A-F]{4})(?:_b(\d))?\(void\)\s*\{", open(path).read(), re.M):
            entries.add((int(m.group(2)) if m.group(2) else lift.FIXED, int(m.group(1), 16)))
    gen = lift.Generated(os.path.join(ROOT, "runs", "p4", "pure", "generated"))
    names = lift.Names()
    bodies = {}
    for bank in range(lift.FIXED + 1):
        lf = lift.Lifter(rom, bank, gen, names)
        lf.flat = True
        for b, addr in sorted(entries):
            if b == bank:
                try:
                    lf.lift(addr)
                    bodies[name_of(bank, addr)] = (bank, addr, lf.last_body)
                except lift.Refuse:
                    pass
    routines = set(bodies)
    rs = {r: Routine(r, b, ad, body, routines) for r, (b, ad, body) in bodies.items()}
    direct = {r: 0 for r in routines}
    for r in rs.values():
        for _, cs in r.calls:
            for c in cs:
                direct[c] += 1
        for t in r.tails:
            direct[t] += 1
    # the jump tables (port/tools/find_entries.py): what each is read at, its targets
    prg = rom[16:16 + rom[4] * 0x4000]
    kind = open(os.path.join(ROOT, "runs", "cdl", "prgmap.bin"), "rb").read()
    tbls = [(j.reads, {find_entries.name(o) for o in j.targets})
            for j in find_entries.jumps(prg, kind, os.path.join(ROOT, "runs", "cdl", "tables.json")) if j.reads]
    roots = {"func_C000", "func_C13B", "func_FE00", "func_C119", "func_C14A", "func_C194",
             "func_C1B1", "func_C1D8", "func_C556"}
    # a table target returns to whoever called the routine that jumped to it
    # through the table: its exits are those of the indirect dispatchers
    dispatchers = [r.name for r in rs.values()
                   if any(k == ("all",) and "jt" in "".join(r.lines[i:i + 2]) for i, k in r.sync.items())]
    exit_force = set()
    tabled_r = {n for _, names in tbls for n in names if n in routines and n not in roots}
    # which dispatcher reads which table: its targets return to that
    # dispatcher's callers (tables no dispatcher visibly reads: all of them)
    READ = re.compile(r"prg_read\(\(uint16_t\)\(0x([0-9A-F]{4}) \+ [xy]\)\)|prg_read\(0x([0-9A-F]{4})\)")
    owners, conts = {}, {}
    for d in dispatchers:
        r = rs[d]
        reads = {int(m.group(1) or m.group(2), 16) for line in r.lines for m in READ.finditer(line)}
        # "call_by_address(jt); call_by_address_tail(CONT, b)": the target
        # returns to the pushed continuation, whose entry is what it must leave
        cont = None
        for i, k in r.sync.items():
            text = "".join(r.lines[i:i + 2])
            if k == ("all",) and "call_by_address(jt)" in text:
                m = TAIL_ADDR.search(text)
                cont = by_addr(int(m.group(1), 16), routines) if m else []
                cont = [c for c in cont if rs[c].bank == r.bank or rs[c].bank == lift.FIXED] or None
        for treads, names in tbls:
            if treads & reads:
                for t in names & tabled_r:
                    if cont:
                        conts.setdefault(t, set()).update(cont)
                    else:
                        owners.setdefault(t, set()).add(d if cont is None else None)
    for t in list(owners):
        if None in owners[t]:           # returns into a continuation it cannot name: be safe
            owners[t] = set()
            exit_force.add(t)
    # roots never return to a caller (reset, the task entry, the interrupt
    # handlers end in RTI): everything may come in, nothing goes back
    nodirect = {r for r in routines if r not in roots and direct[r] == 0 and r not in tabled_r}
    pinned = roots | nodirect
    entry = {r: (ALL if r in pinned or r in tabled_r else frozenset()) for r in routines}
    exit_ = {r: (ALL if r in nodirect or r in exit_force else frozenset()) for r in routines}
    for it in range(1000):
        changed = False
        new_exit = {r: set(exit_[r]) for r in routines}
        for r in rs.values():
            live = r.liveness(entry, exit_)
            e = frozenset(live[0] & ALL) if live else frozenset()
            if not e <= entry[r.name]:
                entry[r.name] = entry[r.name] | e
                changed = True
            for idx, cs in r.calls:
                after = set()
                for j in r.succ[idx]:
                    after |= live[j]
                for c in cs:
                    new_exit[c] |= after & ALL
            for t in r.tails:
                new_exit[t] |= exit_[r.name]
        anyd = frozenset().union(*(exit_[d] for d in dispatchers)) if dispatchers else ALL
        for t in tabled_r:
            ds, cs = owners.get(t), conts.get(t)
            if cs:
                new_exit[t] |= frozenset().union(*(entry[c] for c in cs))
            if ds:
                new_exit[t] |= frozenset().union(*(exit_[d] for d in ds))
            elif not cs:
                new_exit[t] |= anyd
        for r in routines:
            ne = frozenset(new_exit[r])
            if r not in nodirect and r not in exit_force and not ne <= exit_[r]:
                exit_[r] = exit_[r] | ne
                changed = True
        if not changed:
            break
    res = Result()
    res.rs, res.routines, res.entry, res.exit = rs, routines, entry, exit_
    res.roots, res.nodirect, res.pinned, res.tabled, res.owners = roots, nodirect, pinned, tabled_r, owners
    res.dispatchers, res.exit_force, res.rounds = dispatchers, exit_force, it + 1
    return res


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("rom")
    ap.add_argument("--out", default=os.path.join(ROOT, "runs", "p4", "exit_use.json"))
    a = ap.parse_args()
    r = analyze(open(a.rom, "rb").read())
    order = "axynzcv"
    res = {n: {"entry": sorted(r.entry[n], key=order.index), "exit": sorted(r.exit[n], key=order.index)}
           for n in sorted(r.routines)}
    os.makedirs(os.path.dirname(a.out), exist_ok=True)
    json.dump(res, open(a.out, "w"), indent=0)
    unpinned = [n for n in r.routines if n not in r.nodirect]
    full = sum(1 for n in unpinned if r.exit[n] == ALL)
    per = {k: sum(1 for n in unpinned if k in r.exit[n]) for k in REGS}
    print(f"{len(r.routines)} routines, {len(r.pinned)} pinned to everything; of the {len(unpinned)} others "
          f"{full} leave everything to their callers; callers read " +
          ", ".join(f"{k.upper()} after {per[k]}" for k in REGS) + f"; {r.rounds} rounds; {a.out}")


if __name__ == "__main__":
    main()
