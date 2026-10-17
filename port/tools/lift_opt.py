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

Registers and flags still reach g_cpu at calls and exits: all of them
(SYNC_OUT) or the ones the lifter's summaries say cross (g_cpu.X = x; ...),
which are joined back on one line at the end; the routes check it.
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
# the syncs the lifter writes when it knows what crosses (lift.py OUT/IN)
STORE = re.compile(r"^g_cpu\.[AXYNZCV] = (?:\(uint8_t\))?[axynzcv];$")
LOAD = re.compile(r"^[axynzcv] = g_cpu\.[AXYNZCV];$")
STORE_IN = re.compile(r"g_cpu\.[AXYNZCV] = (?:\(uint8_t\))?([axynzcv]);")


def split_if(s):
    """'if (COND) REST' -> (COND, REST), parentheses balanced."""
    d = 0
    for i in range(3, len(s)):
        d += s[i] == "("
        d -= s[i] == ")"
        if d == 0:
            return s[4:i], s[i + 1:]
    return s[4:], ""


def join_syncs(lines):
    """Put the register stores (or loads) of one sync back on one line."""
    out, kinds = [], []
    for s in lines:
        k = "store" if STORE.match(s) else "load" if LOAD.match(s) else None
        if k and kinds and kinds[-1] == k:
            out[-1] += " " + s
        else:
            out.append(s)
            kinds.append(k)
    return out


U8 = {"a", "x", "y", "t", "m"}


def c_to_py(e):
    """A lifter expression in Python (casts and ! made explicit), or None."""
    out, i = [], 0
    while i < len(e):
        prefix = "(uint8_t)" if e.startswith("(uint8_t)", i) else "!" if e[i] == "!" and e[i + 1:i + 2] != "=" else None
        if prefix:
            j = i + len(prefix)
            while j < len(e) and e[j] == " ":
                j += 1
            if j < len(e) and e[j] == "(":
                d, k = 0, j
                while k < len(e):
                    d += e[k] == "("
                    d -= e[k] == ")"
                    if d == 0:
                        break
                    k += 1
                operand, i = e[j + 1:k], k + 1
            else:
                m = re.match(r"\w+", e[j:])
                if not m:
                    return None
                operand, i = m.group(0), j + len(m.group(0))
            inner = c_to_py(operand)
            if inner is None:
                return None
            out.append(f"(0xFF & ({inner}))" if prefix != "!" else f"(0 if ({inner}) else 1)")
            continue
        if e.startswith(("&&", "||", "?"), i):
            return None
        out.append(e[i])
        i += 1
    return "".join(out)


def cval(expr, known):
    """The value of an expression whose every local is a known constant, or None."""
    names = set(re.findall(r"[A-Za-z_]\w*", re.sub(r"\b0[xX][0-9A-Fa-f]+|\b\d+", " ", expr))) - {"uint8_t"}
    if not names <= set(known):
        return None
    return _eval(expr, known)


def _eval(expr, known):
    py = c_to_py(expr)
    if py is None:
        return None
    try:
        return int(eval(py, {"__builtins__": {}}, dict(known)))
    except Exception:
        return None


def constfold(lines):
    """Branches whose flag is a known constant: the 6502's "branch always"
    (LDA #0 / BEQ) becomes a goto, a branch never taken goes away (it would
    have charged nothing). Constants come from immediates, transfers,
    INC/DEC and the flag forms the lifter writes, within a straight run."""
    out, known, dead = [], {}, False
    for s in lines:
        if re.match(r"^L_[0-9A-F]{4}:", s):
            known, dead = {}, False
            out.append(s)
            continue
        if dead:                         # after an unconditional jump: only a label reaches on
            continue
        mb = CBRANCH.match(s)
        if mb:
            v = cval(mb.group(1), known)
            if v is None:
                out.append(s)
            elif v:
                out += [mb.group(2), f"goto L_{mb.group(3)};"]
                dead = True
            continue
        if s.startswith("goto L_") or (re.search(r"\breturn;", s) and not s.startswith("if (")):
            out.append(s)
            dead = True
            continue
        d, u, kind = info(s)
        target, val = None, None
        ma, mi = ASSIGN.match(s), INCDEC.match(s)
        if kind == "assign" and ma:
            target, op, rhs = ma.groups()
            val = cval(rhs if op == "=" else f"{target} {op[0]} ({rhs})", known)
        elif kind == "assign" and mi:
            target = mi.group(1)
            val = cval(f"{target} {'+' if mi.group(2) == '++' else '-'} 1", known)
        for k in d:
            known.pop(k, None)
        if val is not None:
            known[target] = val & 0xFF if target in U8 else val
        out.append(s)
    return out


REG_OF = {"A": "a", "X": "x", "Y": "y", "N": "n", "Z": "z", "C": "c", "V": "v"}
CHANGES_CPU = re.compile(r"\b(mff_call|call_by|nes_dispatch|func_[0-9A-F]{4}\w*\(\)|coroutine|mff_rti)")


def drop_round_trips(lines):
    """A store of a register g_cpu already holds goes: after "x = g_cpu.X;",
    "g_cpu.X = x;" changes nothing until x or g_cpu changes (a call; an
    interrupt saves and restores g_cpu). The loads then left unread go too."""
    out, same = [], set(REG_OF.values())          # REGS_IN: the locals come from g_cpu
    targets = {t for s in lines for t in re.findall(r"goto L_([0-9A-F]{4});", s)}
    for i, s in enumerate(lines):
        ml = re.match(r"^L_([0-9A-F]{4}):", s)
        if ml:
            if i > 0 or ml.group(1) in targets:  # a join: what g_cpu holds is not known
                same = set()
            out.append(s)
            continue
        if STORE.match(s) and s.endswith(f" {REG_OF[s[6]]};"):
            r = REG_OF[s[6]]
            if r in same:
                continue
            same.add(r)
            out.append(s)
            continue
        if LOAD.match(s):
            same.add(s[0])
            out.append(s)
            continue
        if s.startswith("SYNC_OUT") or s.startswith("SYNC_IN"):
            same = set(REG_OF.values())
            out.append(s)
            continue
        if CHANGES_CPU.search(s):
            same = set()
        elif "mff_set_p" in s:
            same -= FLAGSET
        if "FLAGS_IN" in s or "FLAGS_OUT" in s:
            same |= FLAGSET
        d, u, kind = info(s)
        same -= d
        out.append(s)
    return out


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
        # a branch that leaves (a tail call, a bail-out after a call): it reads
        # its condition and what it hands over in g_cpu (SYNC_OUT: everything)
        cond, rest = split_if(s)
        uses = set(WORD.findall(cond))
        uses |= set(ALL) if "SYNC_OUT" in rest else set(STORE_IN.findall(rest))
        return set(), uses, "exitbranch" if "return;" in s else "other"
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
    flat = dce(constfold(flat))
    flat = fold(flat)
    return join_syncs(dce(drop_round_trips(dce(flat))))
