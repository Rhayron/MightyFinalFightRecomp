"""Game-specific dispatch tables the generic static pass cannot resolve.

    py -3 -I tools/mff_dispatch.py ROM > runs/cdl/extra.json

The object loop (`0C:8003`, docs/engine.md) maps the behaviour code of each
object id into `$A000` before dispatching through a split table indexed by the
id (lo `0C:83AF`, hi `0C:844F`, 160 entries), so each entry's bank depends on
the id:

    id < $60  -> bank $0D          $60-$6F -> bank $05
    $70-$7F   -> bank $0B          $80-$8F -> bank $01      $90+ -> bank $03

(`0C:8010`-`803F`: Y starts at $0D, and the id >> 4 test at `0C:802F` always
branches past the code that would change it). Targets in `$8000-$9FFF` are
in bank `$0C` itself.

Output: {"tables": [...], "starts": [prg offsets]} for tools/cdl_report.py
--extra and disasm/tools/disasm.py --tables.
"""
import json
import sys

LO, HI, COUNT = 0x0C * 0x2000 + 0x03AF, 0x0C * 0x2000 + 0x044F, 160


def object_bank(obj_id):
    if obj_id < 0x60:
        return 0x0D
    if obj_id < 0x70:
        return 0x05
    if obj_id < 0x80:
        return 0x0B
    if obj_id < 0x90:
        return 0x01
    return 0x03


def main():
    rom = open(sys.argv[1], "rb").read()
    prg = rom[16:16 + rom[4] * 0x4000]
    banks, starts = [], []
    for i in range(COUNT):
        target = prg[LO + i] | prg[HI + i] << 8
        if 0xA000 <= target < 0xC000:
            bank = object_bank(i)
            off = bank * 0x2000 + (target - 0xA000)
        elif 0x8000 <= target < 0xA000:
            bank = 0x0C
            off = bank * 0x2000 + (target - 0x8000)
        else:
            bank = 0x0E if target < 0xE000 else 0x0F
            off = bank * 0x2000 + (target & 0x1FFF)
        banks.append(bank)
        starts.append(off)
    json.dump({"tables": [{"type": "split", "lo": LO, "hi": HI, "plus": 0, "count": COUNT,
                           "target_banks": banks, "name": "obj_behavior"}],
               "starts": sorted(set(starts))}, sys.stdout, indent=1)


if __name__ == "__main__":
    main()
