"""Lift a 6502 routine of the P4 scaffold to readable C (port/src/game).

    py -3 -I port/tools/lift.py ROM ADDR [ADDR ...] [--bank B16] [--out FILE.c]

For each entry ADDR (16 KiB bank B16, default the fixed bank 7) this finds the
code the *pure* scaffold runs for it (runs/p4/pure/generated, regen.sh
--pure): the function the dispatcher calls, or the shared body and entry it
wraps. It follows that copy instruction by instruction and writes C that does
the same thing:

- semantics from the ROM (the 6502 instruction set), RAM by name where
  disasm/labels.toml has one (state.h);
- every decision the generator made from its own copy of the line: the
  cycles it charges (taken branches, page crossings, the +2 of a jump it
  turned into a tail call), gotos vs tail calls, how a JSR dispatches and
  whether it checks the stack, coroutine yields;
- one charge per straight run of instructions, cut after every hardware
  access (the access then happens at the cycle it did) and at every call;
- A/X/Y and the flags in locals, synced with g_cpu at calls and exits; a
  flag computation no path reads is left out.

The output is then exact by construction and port/tests/run_routes.sh
proves it; what it is not yet is idiomatic - loops and ifs come out as
gotos, to be tidied by hand. Routines it cannot lift (the scheduler's
coroutine starts, RTS used as a jump) are refused with a reason.
"""
import argparse
import glob
import os
import re
import sys
import tomllib

ROOT = os.path.normpath(os.path.join(os.path.dirname(__file__), "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from dis6502 import MN  # noqa: E402
sys.path.insert(0, os.path.dirname(__file__))
import lift_structure  # noqa: E402

SIZE = {"imp": 1, "acc": 1, "imm": 2, "zp": 2, "zpx": 2, "zpy": 2, "izx": 2, "izy": 2, "rel": 2,
        "abs": 3, "abx": 3, "aby": 3, "ind": 3}
FIXED = 7
LINE = re.compile(r"^    /\* \$([0-9A-F]{4}): ([0-9A-F]{2}) \*/ (.*)$")
FUNC = re.compile(r"^void (func_[0-9A-F]{4}(?:_b\d+)?(?:_body)?)\((void|int _entry)\) \{")
BRANCH = {"BPL": ("n", False), "BMI": ("n", True), "BVC": ("v", False), "BVS": ("v", True),
          "BCC": ("c", False), "BCS": ("c", True), "BNE": ("z", False), "BEQ": ("z", True)}


FLAGS = {"n", "z", "c", "v"}


class Refuse(Exception):
    pass


# ---------------------------------------------------------------- generated C

class Generated:
    """The pure scaffold's generated functions, by name: [(pc, op, stmt)]."""

    def __init__(self, gen_dir):
        self.funcs = {}
        self.cases = {}
        for path in sorted(glob.glob(os.path.join(gen_dir, "mff_full*.c"))):
            self._parse(open(path).read().split("\n"))

    def _parse(self, lines):
        cur = None
        i = 0
        while i < len(lines):
            line = lines[i]
            m = FUNC.match(line)
            if m:
                cur = m.group(1)
                self.funcs[cur] = []
                self.cases[cur] = {}
                i += 1
                continue
            if cur is None:
                i += 1
                continue
            if line == "}" and (i + 1 >= len(lines) or lines[i + 1] == ""):
                cur = None
                i += 1
                continue
            mc = re.match(r"\s+case (\d+): goto label_([0-9A-F]{4});", line)
            if mc:
                self.cases[cur][int(mc.group(1))] = int(mc.group(2), 16)
            m = LINE.match(line)
            if m:
                body = [m.group(3)]
                j = i + 1
                while (j < len(lines) and not LINE.match(lines[j]) and not lines[j].startswith("label_")
                       and not (lines[j] == "}" and (j + 1 >= len(lines) or lines[j + 1] == ""))):
                    body.append(lines[j].strip())
                    j += 1
                stmt = " ".join(body)
                stmt = re.sub(r"#ifdef RECOMP_STACK_TRACKING.*?#endif", "", stmt)
                stmt = re.sub(r"#ifdef WATCHDOG_ENABLED.*?#endif", "", stmt)
                self.funcs[cur].append((int(m.group(1), 16), int(m.group(2), 16), re.sub(r"\s+", " ", stmt)))
                i = j
                continue
            # a wrapper: func_X() { ... func_O_body(k); ... }
            mw = re.match(r"\s+(func_[0-9A-F]{4}(?:_b\d+)?)_body\((\d+)\);", line)
            if mw and not self.funcs[cur]:
                self.funcs[cur] = ("wrap", mw.group(1) + "_body", int(mw.group(2)))
            i += 1

    def resolve(self, name):
        """(body name, entry pc or None for the start) of the code func `name` runs."""
        f = self.funcs.get(name)
        if f is None:
            raise Refuse(f"{name} is not in the pure scaffold's generated code")
        if isinstance(f, tuple):
            body, k = f[1], f[2]
            if k == 0:
                return body, self.funcs[body][0][0]
            return body, self.cases[body][k]
        if not f:
            raise Refuse(f"{name} has no body (interpreter stub?)")
        return name, f[0][0]


# ---------------------------------------------------------------- names

class Names:
    def __init__(self):
        lab = tomllib.load(open(os.path.join(ROOT, "disasm", "labels.toml"), "rb"))
        self.ram = {}
        for r in lab.get("ram", []):
            for i in range(r.get("size", 1)):
                self.ram[r["addr"] + i] = (r["name"].upper(), i)
        self.code = {(l["bank"], l["addr"]): l["name"] for l in lab.get("label", [])}

    def addr(self, a):
        if a in self.ram:
            n, i = self.ram[a]
            return n + (f" + {i}" if i else "")
        return f"0x{a:02X}" if a < 0x100 else f"0x{a:04X}"

    def label(self, addr, bank16):
        for b8 in ((0x0E, 0x0F) if bank16 == FIXED else (bank16 * 2, bank16 * 2 + 1)):
            if (b8, addr) in self.code:
                return self.code[(b8, addr)]
        return None


# ---------------------------------------------------------------- the lifter

REGS = {0x2000: "PPU_CTRL", 0x2001: "PPU_MASK", 0x2002: "PPU_STATUS", 0x2003: "OAM_ADDR",
        0x2004: "OAM_DATA", 0x2005: "PPU_SCROLL", 0x2006: "PPU_ADDR", 0x2007: "PPU_DATA",
        0x4000: "APU_PULSE1", 0x4004: "APU_PULSE2", 0x4008: "APU_TRIANGLE", 0x400C: "APU_NOISE",
        0x4010: "APU_DMC", 0x4014: "OAM_DMA", 0x4015: "APU_STATUS", 0x4016: "JOY1", 0x4017: "JOY2",
        0x8000: "MMC3_BANK_SELECT", 0x8001: "MMC3_BANK_DATA", 0xA000: "MMC3_MIRRORING",
        0xA001: "MMC3_PRG_RAM", 0xC000: "MMC3_IRQ_LATCH", 0xC001: "MMC3_IRQ_RELOAD",
        0xE000: "MMC3_IRQ_DISABLE", 0xE001: "MMC3_IRQ_ENABLE"}


def reg_name(a):
    """hw.h's name for a register (APU channels by their first register +
    offset); a PRG address stays a number - a table, or an MMC3 register
    when written."""
    if a in REGS and a < 0x8000:
        return REGS[a]
    if 0x4000 <= a < 0x4014:
        base = a & ~3
        return f"{REGS[base]} + {a - base}"
    if 0x2008 <= a < 0x4000:
        return f"PPU_CTRL + 0x{a - 0x2000:X}"   # a mirror
    return f"0x{a:04X}"


def is_hw(a):
    return 0x2000 <= a < 0x6000 or a >= 0x8000


class Lifter:
    def __init__(self, rom, bank16, gen, names):
        self.prg = rom[16:16 + rom[4] * 0x4000]
        self.bank = bank16
        self.gen = gen
        self.nm = names
        self.calls = set()          # prototypes needed
        self.stops = set()          # entries written by hand: reaching one is a tail call
        self.flat = False           # True: labels and gotos, no if/loop structuring

    def byte(self, pc):
        if pc >= 0xC000:
            return self.prg[FIXED * 0x4000 + pc - 0xC000]
        return self.prg[self.bank * 0x4000 + (pc - 0x8000) % 0x4000]

    def decode(self, pc):
        op = self.byte(pc)
        if op not in MN:
            raise Refuse(f"${pc:04X}: opcode ${op:02X} not official")
        mn, mode = MN[op]
        n = SIZE[mode]
        arg = self.byte(pc + 1) if n == 2 else (self.byte(pc + 1) | self.byte(pc + 2) << 8) if n == 3 else None
        return mn, mode, arg, n

    # ---- one entry

    def lift(self, entry):
        fname = f"func_{entry:04X}" + ("" if self.bank == FIXED else f"_b{self.bank}")
        body, start = self.gen.resolve(fname)
        code = {pc: (op, stmt) for pc, op, stmt in self.gen.funcs[body]}
        if start not in code:
            raise Refuse(f"entry ${start:04X} not found in {body}")
        # walk the copy the scaffold runs
        insns, todo, targets = {}, [start], {start}
        while todo:
            pc = todo.pop()
            while pc not in insns:
                if pc != start and pc in self.stops:
                    insns[pc] = {"pc": pc, "mn": "STOP", "n": 0, "cyc": 0, "goto": [], "end": True}
                    break
                if pc not in code:
                    raise Refuse(f"${pc:04X} reached but not in {body}")
                op, stmt = code[pc]
                if op != self.byte(pc):
                    raise Refuse(f"${pc:04X}: generated opcode differs from the ROM")
                if op not in MN:
                    # a branch that never falls through, decoded on into data
                    insns[pc] = {"pc": pc, "mn": "DATA", "n": 1, "cyc": 0, "goto": [], "end": True}
                    break
                ins = self.control(pc, stmt)
                insns[pc] = ins
                for t in ins["goto"]:
                    if t not in targets:
                        targets.add(t)
                        todo.append(t)
                if ins["end"]:
                    break
                pc += ins["n"]
        self.insns, self.targets = insns, targets
        return self.emit(entry, fname, start)

    # ---- what the generator decided for one instruction

    def control(self, pc, stmt):
        mn, mode, arg, n = self.decode(pc)
        m = re.match(r"nes_instruction_boundary\(0x([0-9A-F]{4}), (\d+)\); ?(.*)$", stmt)
        if not m:
            raise Refuse(f"${pc:04X}: unexpected statement {stmt[:80]}")
        ins = {"pc": pc, "mn": mn, "mode": mode, "arg": arg, "n": n, "cyc": int(m.group(2)),
               "goto": [], "end": False}
        rest = m.group(3)
        if mn in BRANCH:
            t = (pc + 2 + (arg - 256 if arg >= 128 else arg)) & 0xFFFF
            ins["target"] = t
            mg = re.search(r"\{ (?:maybe_trigger_vblank\((\d+)\)|nes_instruction_boundary\(0x([0-9A-F]{4}), (\d+)\)); goto label_([0-9A-F]{4}); \}", rest)
            mt = re.search(r"\{ maybe_trigger_vblank\((\d+)\); call_by_address_tail\(\(uint16_t\)\(g_code_window_base \+ 0x([0-9A-F]{4})\), (-?\d+)\); return; \}", rest)
            if mg:
                ins["taken"] = int(mg.group(1) or mg.group(3))
                ins["taken_pc"] = pc if mg.group(1) else int(mg.group(2), 16)
                ins["kind"] = "goto"
                ins["goto"] = [int(mg.group(4), 16)]
            elif mt:
                ins["taken"] = int(mt.group(1))
                ins["taken_pc"] = pc
                ins["kind"] = "tail"
                ins["tail"] = (int(mt.group(2), 16), int(mt.group(3)))
            else:
                raise Refuse(f"${pc:04X}: branch form {rest[:90]}")
        elif mn == "JMP" and mode == "abs":
            ins["end"] = True
            if rest.startswith("coroutine_yield();"):
                ins["kind"] = "yield"
            elif re.match(r"goto label_([0-9A-F]{4});", rest):
                ins["kind"] = "goto"
                ins["goto"] = [arg]
            elif rest.startswith("while(1)"):
                raise Refuse(f"${pc:04X}: self loop")
            else:
                me = re.match(r"(?:nes_cpu_instruction_boundary\(0x([0-9A-F]{4}), (\d+)\); )?(.*)$", rest)
                ins["kind"] = "tail"
                ins["extra"] = int(me.group(2)) if me.group(2) else 0
                tail = me.group(3)
                md = re.match(r"\{ uint16_t _swb = g_code_window_base; g_code_window_base = 0x([0-9A-F]{4}); (func_[0-9A-F_b]+)\(\); g_code_window_base = _swb; \} return;", tail)
                mc = re.match(r"call_by_address_tail\(0x([0-9A-F]{4}), (-?\d+)\); return;", tail)
                mf = re.match(r"(func_[0-9A-F_b]+)\(\); return;", tail)
                if md:
                    ins["call"] = ("direct", md.group(2), int(md.group(1), 16))
                elif mc:
                    ins["call"] = ("dispatch_tail", int(mc.group(1), 16), int(mc.group(2)))
                elif mf:
                    ins["call"] = ("direct", mf.group(1), None)
                else:
                    raise Refuse(f"${pc:04X}: JMP form {tail[:90]}")
        elif mn == "JMP":
            ins["end"] = True
            mi = re.search(r"_jt = (nes_read16_jmpbug|nes_read16zp)\(0x([0-9A-F]{2,4})\); nes_cpu_instruction_boundary\(_jt, (\d+)\); call_by_address_tail\(_jt, (-?\d+)\); return;", rest)
            # a jump whose target returns to an address the code pushed itself:
            # call it, then continue there
            mk = re.search(r"_jt = (nes_read16_jmpbug|nes_read16zp)\(0x([0-9A-F]{2,4})\); nes_cpu_instruction_boundary\(_jt, (\d+)\); call_by_address\(_jt\); call_by_address_tail\(0x([0-9A-F]{4}), (-?\d+)\); return;", rest)
            if mi:
                ins["kind"] = "ind"
                ins["read"], ins["extra"], ins["bankarg"] = mi.group(1), int(mi.group(3)), int(mi.group(4))
            elif mk:
                ins["kind"] = "ind_cont"
                ins["read"], ins["extra"] = mk.group(1), int(mk.group(3))
                ins["cont"], ins["bankarg"] = int(mk.group(4), 16), int(mk.group(5))
            else:
                raise Refuse(f"${pc:04X}: JMP () form {rest[:90]}")
        elif mn == "JSR":
            md = re.search(r"g_ram\[0x100 \+ g_cpu.S\] = \(uint8_t\)_rp; g_cpu.S--; (.*)$", rest)
            mr = re.search(r"g_code_window_base \| 0x([0-9A-F]{4})", rest)
            tail = md.group(1)
            ins["ret"] = int(mr.group(1), 16)
            ins["check"] = "if (g_cpu.S != _cbs)" in tail
            m1 = re.match(r"if \(!nes_dispatch_call\(0x([0-9A-F]{4}), (-?\d+)\)\) g_cpu.S \+= 2;", tail)
            m2 = re.match(r"\{ uint16_t _swb = g_code_window_base; g_code_window_base = 0x([0-9A-F]{4}); (func_[0-9A-F_b]+)\(\); g_code_window_base = _swb; \}", tail)
            m3 = re.match(r"(func_[0-9A-F_b]+)\(\);", tail)
            if m1:
                ins["call"] = ("dispatch", int(m1.group(1), 16), int(m1.group(2)))
            elif m2:
                ins["call"] = ("direct", m2.group(2), int(m2.group(1), 16))
            elif m3:
                ins["call"] = ("direct", m3.group(1), None)
            else:
                raise Refuse(f"${pc:04X}: JSR form {tail[:100]}")
        elif mn == "RTS":
            if "coroutine" in rest or "g_rts_target" not in rest:
                raise Refuse(f"${pc:04X}: RTS form {rest[:80]}")
            ins["end"] = True
        elif mn == "RTI":
            ins["end"] = True
            mb = re.search(r"g_rti_bank = (-?\d+);", rest)
            ins["rti_bank"] = int(mb.group(1))
        elif mn == "BRK":
            raise Refuse(f"${pc:04X}: BRK")
        elif re.search(r"\b(goto|return|call_by_address|coroutine)", rest):
            raise Refuse(f"${pc:04X}: {mn} with control flow {rest[:80]}")
        return ins

    # ---- C for one instruction

    def ea(self, ins):
        """(C address expression, kind) of a memory operand; kind is "ram"
        (CPU RAM, known statically), "hw" (a register, hw.h), "prg" (a table
        in PRG ROM) or "mem" (an address known only at run time)."""
        mode, arg = ins["mode"], ins["arg"]
        nm = self.nm.addr
        if mode == "zp":
            return nm(arg), "ram"
        if mode == "zpx":
            return (f"({nm(arg)} + x) & 0xFF" if arg else "x"), "ram"
        if mode == "zpy":
            return f"({nm(arg)} + y) & 0xFF", "ram"
        if mode == "abs":
            if arg < 0x2000:
                return nm(arg), "ram"
            if arg < 0x6000 or arg >= 0x8000:
                return reg_name(arg), ("prg" if arg >= 0x8000 else "hw")
            return f"0x{arg:04X}", "mem"
        if mode in ("abx", "aby"):
            r = "x" if mode == "abx" else "y"
            if arg + 0xFF < 0x2000:
                return f"{nm(arg)} + {r}", "ram"
            if 0x2000 <= arg and arg + 0xFF < 0x6000:
                return f"(uint16_t)({reg_name(arg)} + {r})", "hw"
            if arg >= 0x8000:
                return f"(uint16_t)({reg_name(arg)} + {r})", "prg"
            return f"(uint16_t)(0x{arg:04X} + {r})", "mem"
        if mode == "izy":
            return f"(uint16_t)(RAM16({nm(arg)}) + y)", "mem"
        if mode == "izx":
            return f"RAM16((uint8_t)({nm(arg)} + x))", "mem"
        raise Refuse(f"${ins['pc']:04X}: address mode {mode}")

    def load(self, ins):
        if ins["mode"] == "imm":
            return f"0x{ins['arg']:02X}"
        e, kind = self.ea(ins)
        return {"ram": f"RAM({e})", "hw": f"hw_read({e})", "prg": f"prg_read({e})",
                "mem": f"mem_read({e})"}[kind]

    def store(self, ins, val):
        e, kind = self.ea(ins)
        # a write to $8000-$FFFF is an MMC3 register write
        if kind == "prg" and ins["mode"] == "abs" and ins["arg"] in REGS:
            e = REGS[ins["arg"]]
        return {"ram": f"RAM({e}) = {val};", "hw": f"hw_write({e}, {val});",
                "prg": f"hw_write({e}, {val});", "mem": f"mem_write({e}, {val});"}[kind]

    def hw_access(self, ins):
        if ins["mode"] in ("imm", "imp", "acc", "rel", "zp", "zpx", "zpy") or ins["mn"] in ("JMP", "JSR"):
            return False
        e, kind = self.ea(ins)
        return kind != "ram"

    def sem(self, ins):
        """[(stmt, defs, uses)] for the instruction's data effect (no control)."""
        mn = ins["mn"]
        L = self.load
        out = []

        def nz(v):
            out.append((f"n = ({v}) >> 7;", {"n"}, set()))
            out.append((f"z = ({v}) == 0;", {"z"}, set()))

        if mn in ("LDA", "LDX", "LDY"):
            r = mn[2].lower()
            out.append((f"{r} = {L(ins)};", {r}, set()))
            nz(r)
        elif mn in ("STA", "STX", "STY"):
            out.append((self.store(ins, mn[2].lower()), set(), set()))
        elif mn in ("TAX", "TAY", "TXA", "TYA"):
            s, d = mn[1].lower(), mn[2].lower()
            out.append((f"{d} = {s};", {d}, set()))
            nz(d)
        elif mn == "TSX":
            out.append(("x = g_cpu.S;", {"x"}, set()))
            nz("x")
        elif mn == "TXS":
            out.append(("g_cpu.S = x;", set(), set()))
        elif mn in ("INX", "INY", "DEX", "DEY"):
            r = mn[2].lower()
            out.append((f"{r}{'++' if mn[:2] == 'IN' else '--'};", {r}, set()))
            nz(r)
        elif mn in ("INC", "DEC"):
            e, kind = self.ea(ins)
            op = "+ 1" if mn == "INC" else "- 1"
            if kind == "ram":
                out.append((f"t = (uint8_t)(RAM({e}) {op}); RAM({e}) = t;", {"t"}, set()))
            else:
                out.append((f"t = (uint8_t)(mem_read({e}) {op}); mem_write({e}, t);", {"t"}, set()))
            nz("t")
        elif mn in ("AND", "ORA", "EOR"):
            o = {"AND": "&", "ORA": "|", "EOR": "^"}[mn]
            out.append((f"a {o}= {L(ins)};", {"a"}, set()))
            nz("a")
        elif mn == "ADC":
            out.append((f"m = {L(ins)}; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; "
                        f"c = w > 0xFF; a = (uint8_t)w;", {"a", "c", "v", "m", "w"}, {"c"}))
            nz("a")
        elif mn == "SBC":
            out.append((f"m = {L(ins)}; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; "
                        f"c = w >= 0; a = (uint8_t)w;", {"a", "c", "v", "m", "w"}, {"c"}))
            nz("a")
        elif mn in ("CMP", "CPX", "CPY"):
            r = {"CMP": "a", "CPX": "x", "CPY": "y"}[mn]
            out.append((f"m = {L(ins)}; c = {r} >= m;", {"c", "m"}, set()))
            nz(f"(uint8_t)({r} - m)")
        elif mn == "BIT":
            out.append((f"m = {L(ins)};", {"m"}, set()))
            out.append(("n = m >> 7;", {"n"}, set()))
            out.append(("v = (m >> 6) & 1;", {"v"}, set()))
            out.append(("z = (a & m) == 0;", {"z"}, set()))
        elif mn in ("ASL", "LSR", "ROL", "ROR"):
            if ins["mode"] == "acc":
                src, put = "a", "a = t;"
            else:
                e, kind = self.ea(ins)
                src = f"RAM({e})" if kind == "ram" else f"mem_read({e})"
                put = f"RAM({e}) = t;" if kind == "ram" else f"mem_write({e}, t);"
            if mn == "ASL":
                s = f"t = {src}; c = t >> 7; t = (uint8_t)(t << 1); {put}"
                uses = set()
            elif mn == "LSR":
                s = f"t = {src}; c = t & 1; t >>= 1; {put}"
                uses = set()
            elif mn == "ROL":
                s = f"t = {src}; m = t >> 7; t = (uint8_t)(t << 1 | c); c = m; {put}"
                uses = {"c"}
            else:
                s = f"t = {src}; m = t & 1; t = (uint8_t)(t >> 1 | c << 7); c = m; {put}"
                uses = {"c"}
            out.append((s, {"t", "c", "m"} | ({"a"} if ins["mode"] == "acc" else set()), uses))
            nz("t")
        elif mn == "PHA":
            out.append(("mff_push(a);", set(), set()))
        elif mn == "PHP":
            out.append(("FLAGS_OUT; mff_push(mff_p());", set(), {"n", "z", "c", "v"}))
        elif mn == "PLA":
            out.append(("a = mff_pull();", {"a"}, set()))
            nz("a")
        elif mn == "PLP":
            out.append(("mff_set_p(mff_pull()); FLAGS_IN;", {"n", "z", "c", "v"}, set()))
        elif mn == "CLC":
            out.append(("c = 0;", {"c"}, set()))
        elif mn == "SEC":
            out.append(("c = 1;", {"c"}, set()))
        elif mn == "CLV":
            out.append(("v = 0;", {"v"}, set()))
        elif mn in ("SEI", "CLI", "SED", "CLD"):
            f = "I" if mn[2] == "I" else "D"
            out.append((f"g_cpu.{f} = {1 if mn[0] == 'S' else 0};", set(), set()))
        elif mn == "NOP":
            pass
        else:
            raise Refuse(f"${ins['pc']:04X}: {mn} not lifted")
        return out

    # ---- the whole routine

    def emit(self, entry, fname, start):
        insns = self.insns
        order = sorted(insns)
        lines = []          # (stmt, defs, uses, is_label)
        # blocks: a label wherever something jumps in, or after a terminator
        labels = set(self.targets)

        def add(s, defs=set(), uses=set()):
            lines.append([s, set(defs), set(uses), None])

        def sync_all():
            add("SYNC_OUT", set(), FLAGS)

        def reload_all():
            add("SYNC_IN", FLAGS, set())

        seg, seg_at = 0, None

        def flush():
            nonlocal seg, seg_at
            if seg_at is not None:
                lines[seg_at[0]][0] = f"mff_cycles(0x{seg_at[1]:04X}, {seg});"
            seg, seg_at = 0, None

        prev_end = True
        # fall-through that is not the next address emitted (a branch into the
        # middle of an instruction, the BIT-skip trick): an explicit goto
        for i, pc in enumerate(order):
            ins = insns[pc]
            nxt = order[i + 1] if i + 1 < len(order) else None
            if not ins["end"] and nxt != pc + ins["n"]:
                labels.add(pc + ins["n"])
        # the walk can reach code below the entry (a branch back into the body
        # it shares): the function must still start at the entry
        if order[0] != start:
            add(f"goto L_{start:04X};")
        fall = None
        for pc in order:
            ins = insns[pc]
            if fall is not None and fall != pc:
                flush()
                add(f"goto L_{fall:04X};")
                prev_end = True
            fall = None if ins["end"] else pc + ins["n"]
            if pc in labels or prev_end:
                flush()
                lines.append([f"L_{pc:04X}:", set(), set(), "label"])
            if seg_at is None:
                seg_at = (len(lines), pc)
                add("CYC")
            seg += ins["cyc"]
            mn = ins["mn"]
            if mn in BRANCH:
                flush()
                flag, val = BRANCH[mn]
                cond = flag if val else f"!{flag}"
                use = flag
                if ins["kind"] == "goto":
                    add(f"if ({cond}) {{ mff_cycles(0x{ins['taken_pc']:04X}, {ins['taken']}); goto L_{ins['goto'][0]:04X}; }}",
                        set(), {use})
                else:
                    t, b = ins["tail"]
                    add(f"if ({cond}) {{ mff_cycles(0x{pc:04X}, {ins['taken']}); SYNC_OUT_INLINE "
                        f"call_by_address_tail((uint16_t)(g_code_window_base + 0x{t:04X}), {b}); return; }}",
                        set(), {use} | FLAGS)
                prev_end = False
                continue
            if mn == "JSR":
                flush()
                sync_all()
                kind = ins["call"]
                site = pc
                if kind[0] == "dispatch":
                    call = f"mff_call_at(0x{ins['ret']:04X}, 0x{kind[1]:04X}, {kind[2]})"
                else:
                    self.calls.add(kind[1])
                    call = f"mff_call_fn(0x{ins['ret']:04X}, {kind[1]}, 0x{kind[2]:04X})" if kind[2] is not None \
                        else f"mff_call_fn(0x{ins['ret']:04X}, {kind[1]}, 0)"
                if ins["check"]:
                    add(f"if (!{call}) return;")
                else:
                    add(f"{call};")
                reload_all()
                prev_end = False
                continue
            if mn == "STOP":
                flush()
                sync_all()
                fn = f"func_{pc:04X}" + ("" if self.bank == FIXED else f"_b{self.bank}")
                add(f"{{ void {fn}(void); {fn}(); }} return;   /* written by hand */")
                prev_end = True
                continue
            if mn == "DATA":
                flush()
                add(f"mff_unreachable(0x{pc:04X}); return;   /* data: the branch above always goes */")
                prev_end = True
                continue
            if mn == "RTS":
                flush()
                sync_all()
                add("mff_rts(); return;")
                prev_end = True
                continue
            if mn == "RTI":
                flush()
                sync_all()
                add(f"mff_rti_bank(0x{pc:04X}, {ins['rti_bank']}); return;")
                prev_end = True
                continue
            if mn == "JMP":
                k = ins["kind"]
                if k == "goto":
                    flush()
                    add(f"goto L_{ins['goto'][0]:04X};")
                elif k == "yield":
                    flush()
                    sync_all()
                    add("coroutine_yield(); return;")
                elif k == "ind":
                    flush()
                    sync_all()
                    p = ins["arg"]
                    add(f"{{ uint16_t jt = {ins['read']}(0x{p:04X}); nes_cpu_instruction_boundary(jt, {ins['extra']}); "
                        f"call_by_address_tail(jt, {ins['bankarg']}); return; }}")
                elif k == "ind_cont":
                    flush()
                    sync_all()
                    p = ins["arg"]
                    add(f"{{ uint16_t jt = {ins['read']}(0x{p:04X}); nes_cpu_instruction_boundary(jt, {ins['extra']}); "
                        f"call_by_address(jt); call_by_address_tail(0x{ins['cont']:04X}, {ins['bankarg']}); return; }}")
                else:
                    flush()
                    sync_all()
                    call = ins["call"]
                    pre = f"nes_cpu_instruction_boundary(0x{ins['arg']:04X}, {ins['extra']}); " if ins["extra"] else ""
                    if call[0] == "direct":
                        self.calls.add(call[1])
                        if call[2] is not None:
                            add(f"{pre}{{ uint16_t swb = g_code_window_base; g_code_window_base = 0x{call[2]:04X}; "
                                f"{call[1]}(); g_code_window_base = swb; }} return;")
                        else:
                            add(f"{pre}{call[1]}(); return;")
                    else:
                        add(f"{pre}call_by_address_tail(0x{call[1]:04X}, {call[2]}); return;")
                prev_end = True
                continue
            for s, d, u in self.sem(ins):
                add(s, d, u)
            if self.hw_access(ins):
                flush()
            prev_end = False
        if fall is not None:
            flush()
            add(f"goto L_{fall:04X};")
        flush()
        self.lines = lines
        return self.finish(entry, fname, start)

    def finish(self, entry, fname, start):
        lines = self.lines
        # liveness of the flag locals over the straight-line list with labels/gotos
        idx = {l[0][2:6]: i for i, l in enumerate(lines) if l[3] == "label"}
        n = len(lines)
        succ = []
        for i, l in enumerate(lines):
            s = l[0]
            nxt = [i + 1] if i + 1 < n else []
            gs = re.findall(r"goto L_([0-9A-F]{4});", s)
            if s.startswith("goto L_"):
                nxt = []
            if re.search(r"\breturn;", s) and not s.startswith("if ("):
                nxt = []
            if s.startswith("if (!") and s.endswith("return;"):     # bail after a call
                pass
            succ.append(nxt + [idx[g] for g in gs])
        flags = FLAGS
        live_in = [set() for _ in range(n)]
        changed = True
        while changed:
            changed = False
            for i in range(n - 1, -1, -1):
                out = set()
                for j in succ[i]:
                    out |= live_in[j]
                s, d, u = lines[i][0], lines[i][1], lines[i][2]
                li = (out - (d & flags)) | (u & flags)
                if li != live_in[i]:
                    live_in[i] = li
                    changed = True
        body = []
        for i, l in enumerate(lines):
            s, d, u = l[0], l[1], l[2]
            out = set()
            for j in succ[i]:
                out |= live_in[j]
            if re.match(r"[nzcv] = ", s) and s[0] not in out:
                continue
            if s == "SYNC_OUT":
                s = "SYNC_OUT;"
            elif s == "SYNC_IN":
                s = "SYNC_IN;"
            s = s.replace("SYNC_OUT_INLINE ", "SYNC_OUT; ")
            body.append(s)
        label = self.nm.label(entry, self.bank)
        out = [f"/* {label or ''} ${entry:04X}{'' if self.bank == FIXED else f' (bank {self.bank})'}"
               f" - lifted by port/tools/lift.py */",
               f"void {fname}(void) {{",
               "    REGS_IN;"]
        if start != entry:
            raise Refuse("entry inside a shared body")
        if self.flat:
            for s in body:
                out.append(s + ";" if s.startswith("L_") else "    " + s)
        else:
            out += lift_structure.structure(body)
        out.append("}")
        return "\n".join(out)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("rom")
    ap.add_argument("addr", nargs="+")
    ap.add_argument("--bank", type=int, default=FIXED)
    ap.add_argument("--gen", default=os.path.join(ROOT, "runs", "p4", "pure", "generated"))
    ap.add_argument("--out")
    a = ap.parse_args()
    rom = open(a.rom, "rb").read()
    gen = Generated(a.gen)
    names = Names()
    lf = Lifter(rom, a.bank, gen, names)
    chunks, ok = [], []
    for s in a.addr:
        addr = int(s, 16)
        try:
            chunks.append(lf.lift(addr))
            ok.append(addr)
        except Refuse as e:
            print(f"${addr:04X}: refused: {e}", file=sys.stderr)
    protos = "".join(f"void {f}(void);\n" for f in sorted(lf.calls))
    text = (protos + "\n" if protos else "") + "\n\n".join(chunks) + "\n"
    if a.out:
        open(a.out, "w").write(text)
    else:
        print(text)
    print(f"lifted {len(ok)} of {len(a.addr)}", file=sys.stderr)


if __name__ == "__main__":
    main()
