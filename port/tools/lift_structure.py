"""Turn the lifter's label/goto C into if/else and loops (port/tools/lift.py).

The lifter emits one statement per line, labels "L_XXXX:" and two kinds of
jumps: "goto L_X;" and "if (cond) { mff_cycles(P, K); goto L_X; }" (a taken
branch and its extra cycles). This rewrites the shapes that have a plain
structured equivalent, without moving or dropping any statement:

    if (c) { ch; goto X; } S X:            ->  if (!c) { S } else { ch; }
    X: S if (c) { ch; goto X; }            ->  X: for (;;) { S if (!c) break; ch; }
    X: S goto X;                           ->  X: for (;;) { S }
    if (c) { ch; goto A; } S1 goto B; A: S2 B:
                                           ->  if (c) { ch; S2 } else { S1 }

S never contains a label or a jump of its own (structured blocks are fine),
so control can only enter at the top and leave at the bottom or through a
return. Labels nobody jumps to anymore are dropped.
"""
import re

CGOTO = re.compile(r"^if \((.+)\) \{ (mff_cycles\(0x[0-9A-F]{4}, \d+\);) goto L_([0-9A-F]{4}); \}$")
GOTO = re.compile(r"^goto L_([0-9A-F]{4});$")
LABEL = re.compile(r"^L_([0-9A-F]{4}):;?$")


def parse(lines):
    items = []
    for s in lines:
        m = CGOTO.match(s)
        if m:
            items.append(("cgoto", m.group(1), m.group(2), m.group(3)))
            continue
        m = GOTO.match(s)
        if m:
            items.append(("goto", m.group(1)))
            continue
        m = LABEL.match(s)
        if m:
            items.append(("label", m.group(1)))
            continue
        items.append(("stmt", s))
    return items


def neg(c):
    """The opposite condition, kept readable."""
    if re.fullmatch(r"!?[nzcv]", c):
        return c[1:] if c.startswith("!") else "!" + c
    if c.startswith("!(") and c.endswith(")") and balanced(c[2:-1]):
        return c[2:-1]
    m = re.fullmatch(r"([^=!<>()]+|\w+\([^()]*\)) (==|!=|>=|<) (.+)", c)
    if m and balanced(m.group(3)):
        op = {"==": "!=", "!=": "==", ">=": "<", "<": ">="}[m.group(2)]
        return f"{m.group(1)} {op} {m.group(3)}"
    return f"!({c})"


def balanced(e):
    d = 0
    for ch in e:
        d += ch == "("
        d -= ch == ")"
        if d < 0:
            return False
    return d == 0


def plain(items):
    """No labels and no jumps (structured blocks are fine)."""
    return all(it[0] not in ("label", "goto", "cgoto") for it in items)


def refs(items, label):
    n = 0
    for it in items:
        if it[0] == "goto" and it[1] == label:
            n += 1
        elif it[0] == "cgoto" and it[3] == label:
            n += 1
        elif it[0] in ("if", "loop", "forever"):
            for blk in it[1:]:
                if isinstance(blk, list):
                    n += refs(blk, label)
    return n


def step(items):
    """One rewrite; False when nothing matched."""
    n = len(items)
    for i, it in enumerate(items):
        # X: S if (c) { ch; goto X; }
        if it[0] == "label":
            x = it[1]
            for j in range(i + 1, n):
                jt = items[j]
                if jt[0] == "cgoto" and jt[3] == x and plain(items[i + 1:j]):
                    loop = ("loop", items[i + 1:j], jt[1], jt[2])
                    items[i + 1:j + 1] = [loop]
                    return True
                if jt[0] == "goto" and jt[1] == x and plain(items[i + 1:j]):
                    items[i + 1:j + 1] = [("forever", items[i + 1:j])]
                    return True
                if jt[0] in ("label", "goto", "cgoto"):
                    break
        if it[0] == "cgoto":
            c, ch, x = it[1], it[2], it[3]
            # forward skip to X with plain code in between
            for j in range(i + 1, n):
                jt = items[j]
                if jt[0] == "label" and jt[1] == x and plain(items[i + 1:j]):
                    items[i:j] = [("if", neg(c), items[i + 1:j], [("stmt", ch)])]
                    return True
                if jt[0] == "goto" and plain(items[i + 1:j]):
                    # diamond: ... S1 goto B; A: S2 B:
                    b = jt[1]
                    if j + 1 < n and items[j + 1] == ("label", x) and refs(items, x) == 1:
                        for k in range(j + 2, n):
                            kt = items[k]
                            if kt[0] == "label" and kt[1] == b and plain(items[j + 2:k]):
                                then = [("stmt", ch)] + items[j + 2:k]
                                items[i:k] = [("if", c, then, items[i + 1:j])]
                                return True
                            if kt[0] in ("label", "goto", "cgoto"):
                                break
                    break
                if jt[0] in ("label", "goto", "cgoto"):
                    break
    # labels no one jumps to
    for i, it in enumerate(items):
        if it[0] == "label" and refs(items, it[1]) == 0 and not it[1].startswith("keep"):
            del items[i]
            return True
    return False


def structure(lines, keep=()):
    """lines: the statements of one function body (no indentation). keep:
    labels to leave in place even if unreferenced (the entry)."""
    items = parse(lines)
    for i, it in enumerate(items):
        if it[0] == "label" and it[1] in keep:
            items[i] = ("label", "keep" + it[1])
    while step(items):
        pass
    out = []
    emit(items, out, 1)
    return out


def emit(items, out, depth):
    pad = "    " * depth
    for it in items:
        k = it[0]
        if k == "stmt":
            out.append(pad + it[1])
        elif k == "label":
            name = it[1][4:] if it[1].startswith("keep") else it[1]
            out.append(f"L_{name}:;")
        elif k == "goto":
            out.append(f"{pad}goto L_{it[1]};")
        elif k == "cgoto":
            out.append(f"{pad}if ({it[1]}) {{ {it[2]} goto L_{it[3]}; }}")
        elif k == "if":
            cond, then, other = it[1], it[2], it[3]
            if not other:
                out.append(f"{pad}if ({cond}) {{")
                emit(then, out, depth + 1)
                out.append(pad + "}")
            elif not then:
                out.append(f"{pad}if ({neg(cond)}) {{")
                emit(other, out, depth + 1)
                out.append(pad + "}")
            else:
                out.append(f"{pad}if ({cond}) {{")
                emit(then, out, depth + 1)
                out.append(pad + "} else {")
                emit(other, out, depth + 1)
                out.append(pad + "}")
        elif k == "loop":
            body, cond, ch = it[1], it[2], it[3]
            out.append(f"{pad}for (;;) {{")
            emit(body, out, depth + 1)
            out.append(f"{pad}    if ({neg(cond)}) break;")
            out.append(f"{pad}    {ch}")
            out.append(pad + "}")
        elif k == "forever":
            out.append(f"{pad}for (;;) {{")
            emit(it[1], out, depth + 1)
            out.append(pad + "}")
