"""Minimal 6502 disassembler over the user's dump, for reading code while mapping.

    py -3 -I tools/dis6502.py ROM BANK ADDR [COUNT]        # COUNT instructions from $ADDR in 8 KiB bank BANK
    py -3 -I tools/dis6502.py ROM --find "9D A0 04"         # opcode-aligned hits in code (needs runs/cdl/prgmap.bin)

Addresses use the MMC3 mode-0 windows the seeds show (even banks $8000, odd
$A000, $0E at $C000, $0F at $E000). Prints only to the terminal; nothing of
the ROM is written to disk.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(__file__))
from cdl_report import CLASSES, LENGTH, OFFICIAL, window  # noqa: E402

MN = {}
for spec in """
00 BRK imp|01 ORA izx|05 ORA zp|06 ASL zp|08 PHP imp|09 ORA imm|0A ASL acc|0D ORA abs|0E ASL abs|10 BPL rel|11 ORA izy|15 ORA zpx|16 ASL zpx|18 CLC imp|19 ORA aby|1D ORA abx|1E ASL abx
20 JSR abs|21 AND izx|24 BIT zp|25 AND zp|26 ROL zp|28 PLP imp|29 AND imm|2A ROL acc|2C BIT abs|2D AND abs|2E ROL abs|30 BMI rel|31 AND izy|35 AND zpx|36 ROL zpx|38 SEC imp|39 AND aby|3D AND abx|3E ROL abx
40 RTI imp|41 EOR izx|45 EOR zp|46 LSR zp|48 PHA imp|49 EOR imm|4A LSR acc|4C JMP abs|4D EOR abs|4E LSR abs|50 BVC rel|51 EOR izy|55 EOR zpx|56 LSR zpx|58 CLI imp|59 EOR aby|5D EOR abx|5E LSR abx
60 RTS imp|61 ADC izx|65 ADC zp|66 ROR zp|68 PLA imp|69 ADC imm|6A ROR acc|6C JMP ind|6D ADC abs|6E ROR abs|70 BVS rel|71 ADC izy|75 ADC zpx|76 ROR zpx|78 SEI imp|79 ADC aby|7D ADC abx|7E ROR abx
81 STA izx|84 STY zp|85 STA zp|86 STX zp|88 DEY imp|8A TXA imp|8C STY abs|8D STA abs|8E STX abs|90 BCC rel|91 STA izy|94 STY zpx|95 STA zpx|96 STX zpy|98 TYA imp|99 STA aby|9A TXS imp|9D STA abx
A0 LDY imm|A1 LDA izx|A2 LDX imm|A4 LDY zp|A5 LDA zp|A6 LDX zp|A8 TAY imp|A9 LDA imm|AA TAX imp|AC LDY abs|AD LDA abs|AE LDX abs|B0 BCS rel|B1 LDA izy|B4 LDY zpx|B5 LDA zpx|B6 LDX zpy|B8 CLV imp|B9 LDA aby|BA TSX imp|BC LDY abx|BD LDA abx|BE LDX aby
C0 CPY imm|C1 CMP izx|C4 CPY zp|C5 CMP zp|C6 DEC zp|C8 INY imp|C9 CMP imm|CA DEX imp|CC CPY abs|CD CMP abs|CE DEC abs|D0 BNE rel|D1 CMP izy|D5 CMP zpx|D6 DEC zpx|D8 CLD imp|D9 CMP aby|DD CMP abx|DE DEC abx
E0 CPX imm|E1 SBC izx|E4 CPX zp|E5 SBC zp|E6 INC zp|E8 INX imp|E9 SBC imm|EA NOP imp|EC CPX abs|ED SBC abs|EE INC abs|F0 BEQ rel|F1 SBC izy|F5 SBC zpx|F6 INC zpx|F8 SED imp|F9 SBC aby|FD SBC abx|FE INC abx
""".replace("\n", "|").split("|"):
    if spec.strip():
        op, mn, mode = spec.split()
        MN[int(op, 16)] = (mn, mode)

FMT = {"imp": "", "acc": "A", "imm": "#${:02X}", "zp": "${:02X}", "zpx": "${:02X},X", "zpy": "${:02X},Y",
       "izx": "(${:02X},X)", "izy": "(${:02X}),Y", "abs": "${:04X}", "abx": "${:04X},X", "aby": "${:04X},Y",
       "ind": "(${:04X})", "rel": "${:04X}"}


def insn(prg, off):
    op = prg[off]
    if op not in MN:
        return f".byte ${op:02X}", 1
    mn, mode = MN[op]
    n = LENGTH[op]
    pc = window(off // 0x2000) + off % 0x2000
    if mode == "rel":
        rel = prg[off + 1] - 256 if prg[off + 1] >= 128 else prg[off + 1]
        arg = FMT[mode].format((pc + 2 + rel) & 0xFFFF)
    elif n == 2:
        arg = FMT[mode].format(prg[off + 1])
    elif n == 3:
        arg = FMT[mode].format(prg[off + 1] | prg[off + 2] << 8)
    else:
        arg = FMT[mode]
    return f"{mn} {arg}".strip(), n


def show(prg, kind, off, count):
    for _ in range(count):
        text, n = insn(prg, off)
        k = CLASSES[kind[off]] if kind else ""
        pc = window(off // 0x2000) + off % 0x2000
        raw = " ".join(f"{b:02X}" for b in prg[off:off + n])
        print(f"{off // 0x2000:02X}:{pc:04X}  {raw:<9} {text:<18} {k}")
        off += n


def main(argv):
    rom = open(argv[0], "rb").read()
    prg = rom[16:16 + rom[4] * 0x4000]
    mp = os.path.join("runs", "cdl", "prgmap.bin")
    kind = open(mp, "rb").read() if os.path.exists(mp) else None
    if argv[1] == "--find":
        pat = bytes.fromhex(argv[2])
        code = (CLASSES.index("opcode"), CLASSES.index("code?"))
        start = 0
        while (i := prg.find(pat, start)) >= 0:
            if kind and kind[i] in code:
                show(prg, kind, i, 1)
            start = i + 1
        return
    bank, addr = int(argv[1], 16), int(argv[2], 16)
    off = bank * 0x2000 + (addr - window(bank))
    show(prg, kind, off, int(argv[3]) if len(argv) > 3 else 24)


if __name__ == "__main__":
    main(sys.argv[1:])
