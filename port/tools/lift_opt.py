"""Clean up the lifter's flat C before structuring (port/tools/lift.py).

The lifter writes every instruction's effect in full: each one sets the
flags it would set, compares go through a temporary m, and so on. On the
flat list of statements (labels, gotos and one-line branches) this:

1. splits compound statements into single assignments;
2. computes, over the control-flow graph, where each local (a x y, the flags
   n z c v, the temporaries m t w) is live;
3. drops assignments to a local nobody reads afterwards, unless the right
   side has a side effect (a pull, a hardware or run-time memory read, a
   call);
4. folds a flag into the branch that tests it when that branch is its only
   reader: "z = (a) == 0; if (z) ..." becomes "if (a == 0) ...", and a
   compare's "m = 0x06" goes into the condition too.

Registers and flags still reach g_cpu exactly at calls and exits (SYNC_OUT
reads them all), so the routine's behaviour does not change; the routes
check it.
"""
import re

LOCALS = ("a", "x", "y", "n", "z", "c", "v", "m", "t", "w")
FLAGSET = {"n", "z", "c", "v"}
ALL = set(LOCALS)
WORD = re.compile(r"\b([axynzcvmtw])\b")
ASSIGN = re.compile(r"^([axynzcvmtw])\s*(=|\+=|-=|&=|\|=|\^=)\s*(.*);$")
INCDEC = re.compile(r"^([axyt])(\+\+|--);$")
CBRANCH = re.compile(r"^if \((.+?)\) \{ (mff_cycles\(0x[0-9A-F]{4}, \d+\);) goto L_([0-9A-F]{4}); \}$")
SIDE = re.compile(r"\b(hw_read|mem_read|mff_pull|mff_call|nes_|call_by|func_|coroutine)")


def atomize(s):
    """Split 'p = q; r = s;' sequences (no braces) into single statements."""
    if "{" in s or s.startswith(("if ", "for ", "goto ", "L_")) or "return" in s:
        return [s]
    parts = [p.strip() for p in s.split(";") if p.strip()]
    if len(parts) <= 1:
        return [s]
    return [p + ";" for p in parts]


def info(s):
    """(defs, uses, kind) of one flat statement."""
    if re.match(r"^L_[0-9A-F]{4}:", s):
        return set(), set(), "label"
    if s.startswith("goto L_"):
        return set(), set(), "goto"
    m = CBRANCH.match(s)
    if m:
        return set(), set(WORD.findall(m.group(1))), "branch"
    if s.startswith("SYNC_OUT"):
        return set(), set(ALL), "sync_out"
    if s.startswith("SYNC_IN"):
        return set(ALL), set(), "sync_in"
    if s.startswith("FLAGS_OUT"):
        return set(), set(FLAGSET), "other"
    if "FLAGS_IN" in s:
        return set(FLAGSET), set(), "other"
    if s.startswith("if ("):
        # a branch that leaves (tail call): SYNC_OUT inside reads everything
        return set(), set(ALL), "exitbranch" if "return;" in s else "other"
    m = ASSIGN.match(s)
    if m and "{" not in s:
        var, op, rhs = m.groups()
        uses = set(WORD.findall(rhs))
        if op != "=":
            uses.add(var)
        return {var}, uses, "assign"
    m = INCDEC.match(s)
    if m:
        return {m.group(1)}, {m.group(1)}, "assign"
    uses = set(WORD.findall(s))
    kind = "exit" if re.search(r"\breturn;", s) and not s.startswith("if (") else "other"
    return set(), uses, kind


def successors(lines):
    idx = {}
    for i, s in enumerate(lines):
        m = re.match(r"^L_([0-9A-F]{4}):", s)
        if m:
            idx[m.group(1)] = i
    succ = []
    n = len(lines)
    for i, s in enumerate(lines):
        d, u, kind = info(s)
        nxt = [i + 1] if i + 1 < n else []
        if kind == "goto":
            nxt = [idx[s[7:11]]]
        elif kind == "branch":
            nxt = nxt + [idx[CBRANCH.match(s).group(3)]]
        elif kind == "exit":
            nxt = []
        succ.append(nxt)
    return succ


def liveness(lines, succ):
    n = len(lines)
    infos = [info(s) for s in lines]
    live_in = [set() for _ in range(n)]
    changed = True
    while changed:
        changed = False
        for i in range(n - 1, -1, -1):
            out = set()
            for j in succ[i]:
                out |= live_in[j]
            d, u, _ = infos[i]
            li = (out - d) | u
            if li != live_in[i]:
                live_in[i] = li
                changed = True
    live_out = []
    for i in range(n):
        out = set()
        for j in succ[i]:
            out |= live_in[j]
        live_out.append(out)
    return live_out


def dce(lines):
    while True:
        succ = successors(lines)
        out = liveness(lines, succ)
        keep = []
        removed = False
        for i, s in enumerate(lines):
            d, u, kind = info(s)
            if kind == "assign" and d and not (d & out[i]) and not SIDE.search(s):
                removed = True
                continue
            keep.append(s)
        lines = keep
        if not removed:
            return lines


def simplify(cond):
    cond = re.sub(r"\(\(uint8_t\)\((\w+) - (\w+)\)\) == 0", r"\1 == \2", cond)
    cond = re.sub(r"\(\(uint8_t\)\((\w+) - (\w+)\)\) != 0", r"\1 != \2", cond)
    cond = re.sub(r"\((\w+)\) == 0", r"\1 == 0", cond)
    cond = re.sub(r"\((\w+)\) != 0", r"\1 != 0", cond)
    cond = re.sub(r"\((\w+)\) & 0x80", r"\1 & 0x80", cond)
    return cond


def fold_branch(var, positive, expr):
    """The condition 'var' or '!var' written with the expression that set var."""
    if var == "z":
        m = re.match(r"^(.*) == 0$", expr)
        if not m:
            return None
        return simplify(f"{m.group(1)} == 0" if positive else f"{m.group(1)} != 0")
    if var == "n":
        m = re.match(r"^(.*) >> 7$", expr)
        if not m:
            return None
        inner = simplify(f"{m.group(1)} & 0x80")
        return inner if positive else f"!({inner})"
    if var == "c":
        m = re.match(r"^(\w+) >= (\w+)$", expr)
        if m:
            return f"{m.group(1)} >= {m.group(2)}" if positive else f"{m.group(1)} < {m.group(2)}"
        return None
    return None


def fold(lines):
    """Fold flags (and a compare's m) into the branches that are their only readers."""
    changed = True
    while changed:
        changed = False
        succ = successors(lines)
        out = liveness(lines, succ)
        for j, s in enumerate(lines):
            mb = CBRANCH.match(s)
            if not mb:
                continue
            cond = mb.group(1)
            mc = re.match(r"^(!?)([nzc])$", cond)
            if not mc:
                continue
            positive, var = mc.group(1) == "", mc.group(2)
            if var in out[j]:
                continue
            # the definition, in the same straight run
            i = j - 1
            while i >= 0:
                d, u, kind = info(lines[i])
                if kind in ("label", "goto", "branch", "exit", "sync_in", "exitbranch") or (kind == "other" and SIDE.search(lines[i])):
                    i = -1
                    break
                if var in u:
                    i = -1
                    break
                if var in d:
                    break
                i -= 1
            if i < 0:
                continue
            md = ASSIGN.match(lines[i])
            if not md or md.group(2) != "=":
                continue
            expr = md.group(3)
            used = set(WORD.findall(expr))
            if any(used & info(lines[k])[0] for k in range(i + 1, j)):
                continue
            new = fold_branch(var, positive, expr)
            if new is None:
                continue
            # a compare: put a simple m into the condition as well
            if "m" in WORD.findall(new) and "m" not in out[j]:
                k = i - 1
                while k >= 0:
                    d, u, kind = info(lines[k])
                    if kind != "assign" or SIDE.search(lines[k]):
                        k = -1
                        break
                    if "m" in d:
                        break
                    k -= 1
                if k >= 0:
                    mm = ASSIGN.match(lines[k])
                    if mm and mm.group(2) == "=" and re.match(r"^(0x[0-9A-F]+|RAM\([^;]*\)|prg_read\([^;]*\)|[axy])$", mm.group(3)):
                        val = mm.group(3)
                        if not any(set(WORD.findall(val)) & info(lines[q])[0] for q in range(k + 1, j)) and \
                                not any("m" in info(lines[q])[1] for q in range(k + 1, j) if q != i):
                            new = re.sub(r"\bm\b", val, new)
            lines[j] = f"if ({new}) {{ {mb.group(2)} goto L_{mb.group(3)}; }}"
            del lines[i]
            changed = True
            break
    return lines


def optimize(lines):
    flat = []
    for s in lines:
        flat.extend(atomize(s))
    flat = dce(flat)
    flat = fold(flat)
    return dce(flat)
