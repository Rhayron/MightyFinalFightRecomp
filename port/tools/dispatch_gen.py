"""Write port/src/glue/dispatch.c: the port's address -> function table.

    py -3 -I port/tools/dispatch_gen.py

The scaffold's generated code brings its own dispatcher (call_by_address_cb,
which the runtime calls for every JSR/JMP it cannot resolve statically) and
the NMI/IRQ/RESET entry points. Once every routine lives in port/src/game,
that is all the generated code still provides; this writes the same glue
from the functions defined in port/src/game, so the port builds without
generated/ (CMake option MFF_EJECTED). Rerun it after adding a routine.

The dispatcher resolves an address the way the generated one does: through
the live MMC3 8 KiB bank of its CPU window, rebased to the layout the
routines are named after (fixed bank $C000-$FFFF as func_XXXX, switchable
16 KiB bank N at $8000-$BFFF as func_XXXX_bN). An address with no routine
goes to the interpreter (counted by the runtime as a dispatch miss).
"""
import glob
import os
import re
from collections import defaultdict

ROOT = os.path.normpath(os.path.join(os.path.dirname(__file__), "..", ".."))
FUNC = re.compile(r"^void func_([0-9A-F]{4})(?:_b(\d))?\(void\)\s*\{", re.M)


def main():
    fixed, banked = set(), defaultdict(dict)
    # nmi_tail ($C119) stays in the scaffold's extras.c, which the pure build also needs
    paths = sorted(glob.glob(os.path.join(ROOT, "port", "src", "game", "*.c")))
    paths.append(os.path.join(ROOT, "port", "scaffold", "extras.c"))
    for path in paths:
        for m in FUNC.finditer(open(path).read()):
            addr = int(m.group(1), 16)
            if m.group(2) is None:
                fixed.add(addr)
            else:
                banked[addr][int(m.group(2))] = f"func_{addr:04X}_b{m.group(2)}"
    out = ["/* dispatch.c - the port's address -> routine table and the CPU entry points.",
           " * Written by port/tools/dispatch_gen.py from port/src/game; do not edit. */",
           '#include "nes_runtime.h"', "",
           "/* the routines push their JSR return addresses (interp.c checks this) */",
           "int g_recomp_push_all_jsr = 1;", "",
           "uint16_t g_rti_target = 0;   /* RTI's target, func_NMI runs it (the hijacked return) */",
           "uint16_t g_rti_source = 0;", "int g_rti_bank = -1;", "uint16_t g_rts_target = 0;", ""]
    names = sorted(f"func_{a:04X}" for a in fixed) + sorted(n for d in banked.values() for n in d.values())
    out += [f"void {n}(void);" for n in names] + [""]
    out += [
        "int call_by_address_cb(uint16_t addr, int caller_bank) {",
        "    extern int g_mmc3_win_bank8k[4];",
        "    uint16_t cpu_addr = addr;",
        "    int b8, bank;",
        "    if (addr < 0x8000) return nes_interp_dispatch(addr);",
        "    b8 = g_mmc3_win_bank8k[(addr >> 13) & 3];",
        "    bank = b8 >> 1;",
        "    (void)caller_bank;",
        "    addr = (uint16_t)((bank == 7 ? 0xC000 : 0x8000) + ((b8 & 1) ? 0x2000 : 0) + (addr & 0x1FFF));",
        "    switch (addr) {",
    ]
    for a in sorted(fixed):
        out.append(f"    case 0x{a:04X}: func_{a:04X}(); break;")
    for a in sorted(banked):
        out.append(f"    case 0x{a:04X}:")
        out.append("        switch (bank) {")
        for b, n in sorted(banked[a].items()):
            out.append(f"        case {b}: {n}(); break;")
        out.append("        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);")
        out.append("        }")
        out.append("        break;")
    out += [
        "    default:",
        "        return nes_interp_dispatch_bank(cpu_addr, addr, bank);",
        "    }",
        "    return 1;",
        "}", "",
        "int call_by_address(uint16_t addr) { return nes_dispatch_call(addr, -1); }", "",
        "void func_RESET(void) {",
        "    uint16_t wb = g_code_window_base;",
        "    g_code_window_base = 0xE000;",
        "    func_FE00();",
        "    g_code_window_base = wb;",
        "}", "",
        "/* The runner pushes the NMI's hardware frame; the handler's RTI leaves its",
        " * target in g_rti_target, and a hijacked return (nmi_tail) runs after it,",
        " * outside the NMI, as on the 6502. */",
        "void func_NMI(void) {",
        "    uint16_t wb = g_code_window_base;",
        "    g_rti_target = 0;",
        "    g_code_window_base = 0xC000;",
        "    func_C000();",
        "    g_code_window_base = wb;",
        "    if (g_rti_target != 0) {",
        "        runtime_begin_post_nmi();",
        "        call_by_address(g_rti_target);",
        "        runtime_end_post_nmi();",
        "    }",
        "}", "",
        "void func_IRQ(void) {",
        "    uint16_t wb = g_code_window_base;",
        "    g_code_window_base = 0xC000;",
        "    func_C13B();",
        "    g_code_window_base = wb;",
        "}", "",
    ]
    os.makedirs(os.path.join(ROOT, "port", "src", "glue"), exist_ok=True)
    path = os.path.join(ROOT, "port", "src", "glue", "dispatch.c")
    open(path, "w", newline="\n").write("\n".join(out))
    print(f"{path}: {len(fixed)} fixed-bank and {sum(len(d) for d in banked.values())} banked routines")


if __name__ == "__main__":
    main()
