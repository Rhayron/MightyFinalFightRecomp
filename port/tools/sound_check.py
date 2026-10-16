"""Check port/tools/sound_model.py against the oracle, frame by frame.

    py -3 -I port/tools/sound_check.py ROM RAMFILE [--apu APUFILE] [--from F] [--to F]
                                       [--closed | --resync] [--show N]

RAMFILE: per-frame CPU RAM from `sound_oracle.py ram` or `inject`. APUFILE:
per-frame APU register writes from `sound_oracle.py apu` (optional).

Open loop (default): the model's RAM is copied from the oracle once, at
--from (default: the first frame after reset cleared RAM, else the file's
first frame), and from then on only the sound commands reach it: for frame
f it plays the commands that sit in the oracle's queue at the end of frame
f-1 (what sound_update drains at the start of frame f: from
sound_queue_read up to the first $88) and ticks once. Its RAM is then
compared with the oracle's frame f. A game reset (RAM freshly cleared, the
attract mode's JMP reset) is applied to the model as a reset.
--closed restarts every frame from the oracle's RAM of frame f-1 (localizes
a single bad frame); --resync copies the oracle's RAM after a bad frame.

Compared: driver state ($C0, $C5-$D8, $0700-$077F) and the scratch bytes
$C1-$C4 separately. Special commands store the caller's Y register in $C3,
which no RAM shows: a twin model plays them with another Y, and a $C3 that
differs between the twins is reported as Y-dependent. With --apu, the
model's register writes of frame f are compared with the oracle's writes to
$4000-$4013/$4015 in frame f (same values in the same order).
"""
import argparse
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sound_model as sm  # noqa: E402
import sound_oracle as so  # noqa: E402
import sound_rom as sr  # noqa: E402

APU_REGS = set(range(0x4000, 0x4014)) | {0x4015}


def pending(ram):
    """Commands sound_update would drain from this RAM image (0F:FF8E)."""
    out = []
    x = ram[0xDB] & 7
    for _ in range(8):
        c = ram[0xDC + x]
        if c == 0x88:
            break
        out.append(c)
        x = (x + 1) & 7
    return out


def just_reset(prev, cur):
    """reset (0F:FE00) cleared RAM during this frame: the attract mode restarts
    the game with JMP reset (docs/engine.md). Not a sound command, so the
    harness applies it to the model as an external event."""
    fresh = (all(cur[a] == 0 for a in sm.ALL) and cur[0xDA] == 0 and cur[0xDB] == 0
             and all(cur[0xDC + i] == 0x88 for i in range(8)))
    return fresh and not all(prev[a] == 0 for a in sm.ALL)


def first_cleared(frames, ram):
    """First frame whose sound RAM is all zero and whose queue is all $88
    (reset has run)."""
    for f in frames:
        r = ram[f]
        if all(r[a] == 0 for a in sm.ALL) and all(r[0xDC + i] == 0x88 for i in range(8)):
            return f
    return frames[0]


def name(addr):
    names = {sm.C0: "C0 flags", sm.C5: "C5 instr lo", sm.C6: "C6 instr hi", sm.C7: "C7 ticks", sm.C8: "C8 tempo acc",
             sm.C9: "C9 tempo", sm.CA: "CA tempo frac", sm.CB: "CB transpose", sm.CC: "CC fade", sm.CD: "CD fade lvl",
             sm.CE: "CE priority", sm.CF: "CF sfx mask", sm.D0: "D0 sfx ptr", sm.D1: "D1 sfx ptr", sm.D2: "D2",
             sm.D3: "D3 row frames", sm.D4: "D4 gate", sm.D5: "D5 gate left", sm.D6: "D6 loop", sm.D7: "D7 relaunch",
             sm.D8: "D8", sm.C1: "C1", sm.C2: "C2", sm.C3: "C3", sm.C4: "C4"}
    if addr in names:
        return f"${addr:02X} ({names[addr]})"
    arrays = ["instr", "env", "lfo", "vol", "level", "detune", "slide", "target", "per_lo", "per_hi",
              "ptr_lo", "ptr_hi", "flags", "chtrans", "dur", "gate_ratio", "gate", "loop0", "loop1", "loop2",
              "loop3", "phi_cache"]
    if 0x700 <= addr < 0x728:
        arr, x = divmod(addr - 0x700, 4)
        return f"${addr:04X} (sfx {arrays[arr]}[{sr.CHANNELS[x]}])"
    if 0x728 <= addr < 0x750:
        return f"${addr:04X} (music {arrays[(addr - 0x728) // 4]}[{sr.CHANNELS[(addr - 0x728) % 4]}])"
    if 0x750 <= addr < 0x77C:
        return f"${addr:04X} (music {arrays[10 + (addr - 0x750) // 4]}[{sr.CHANNELS[(addr - 0x750) % 4]}])"
    if 0x77C <= addr < 0x780:
        return f"${addr:04X} (phi_cache[{sr.CHANNELS[addr - 0x77C]}])"
    return f"${addr:04X}"


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("rom")
    ap.add_argument("ramfile")
    ap.add_argument("--apu")
    ap.add_argument("--from", dest="first", type=int)
    ap.add_argument("--to", dest="last", type=int)
    ap.add_argument("--closed", action="store_true")
    ap.add_argument("--resync", action="store_true", help="open loop, but copy the oracle's RAM after a bad frame")
    ap.add_argument("--show", type=int, default=8, help="mismatching frames to print")
    ap.add_argument("--quiet", action="store_true")
    a = ap.parse_args()

    rom = sr.Rom(a.rom)
    ram = so.read_ram(a.ramfile)
    frames = sorted(ram)
    f0 = a.first if a.first is not None else first_cleared(frames, ram)
    last = a.last if a.last is not None else frames[-1]
    apu = so.read_apu(a.apu) if a.apu else None

    # Two models: special commands store the caller's Y in $C3 (06:81AE), which
    # no RAM shows; the twin plays them with another Y, and a $C3 that differs
    # between the two at a frame depends on that unknown register.
    model, twin = bytearray(0x800), bytearray(0x800)
    for addr in sm.ALL:
        model[addr] = twin[addr] = ram[f0][addr]
    drv, drv2 = sm.Driver(rom, model), sm.Driver(rom, twin)

    n = state_ok = all_ok = all_but_y = 0
    apu_n = apu_ok = 0
    shown = 0
    first_bad = None
    cmd_frames = 0
    resets = 0
    for f in range(f0 + 1, last + 1):
        if f not in ram or f - 1 not in ram:
            continue
        prev, cur = ram[f - 1], ram[f]
        if a.closed:
            for addr in sm.ALL:
                model[addr] = twin[addr] = prev[addr]
        if just_reset(prev, cur):
            resets += 1
            for addr in sm.ALL:
                model[addr] = twin[addr] = 0
            continue
        cmds = pending(prev)
        if cmds:
            cmd_frames += 1
        try:
            writes = drv.update(cmds, y=0x00)
            drv2.update(cmds, y=0xA5)
        except sm.DriverError as e:
            print(f"frame {f}: model stopped: {e}")
            break
        n += 1
        bad_state = [x for x in sm.STATE if model[x] != cur[x]]
        bad_scratch = [x for x in sm.SCRATCH if model[x] != cur[x]]
        y_dependent = model[sm.C3] != twin[sm.C3]
        if not bad_state:
            state_ok += 1
            if not bad_scratch:
                all_ok += 1
            if not bad_scratch or (bad_scratch == [sm.C3] and y_dependent):
                all_but_y += 1
        elif first_bad is None:
            first_bad = f
        apu_bad = False
        if apu is not None and f in apu:
            apu_n += 1
            want = [(addr, v) for _c, addr, v in apu[f] if addr in APU_REGS]
            if want == writes:
                apu_ok += 1
            else:
                apu_bad = True
        if (bad_state or bad_scratch or apu_bad) and shown < a.show and not a.quiet:
            shown += 1
            print(f"frame {f}: commands {' '.join(f'{c:02X}' for c in cmds) or '-'}")
            for x in bad_state + bad_scratch:
                print(f"    {name(x):34s} model {model[x]:02X}  oracle {cur[x]:02X}")
            if apu_bad:
                want = [(addr, v) for _c, addr, v in apu[f] if addr in APU_REGS]
                print("    APU model:  " + " ".join(f"{ad:04X}={v:02X}" for ad, v in writes))
                print("    APU oracle: " + " ".join(f"{ad:04X}={v:02X}" for ad, v in want))
        if (bad_state or bad_scratch) and a.resync:
            for addr in sm.ALL:
                model[addr] = twin[addr] = cur[addr]

    mode = "closed loop" if a.closed else "open loop, resynced after each bad frame" if a.resync else "open loop"
    print(f"{mode}, frames {f0 + 1}-{last}: {n} frames compared, {cmd_frames} with commands"
          + (f", {resets} game resets applied (not compared)" if resets else ""))
    print(f"  driver state ($C0,$C5-$D8,$0700-$077F) identical: {state_ok}/{n}"
          f" ({100.0 * state_ok / max(n, 1):.2f}%)" + (f", first difference at frame {first_bad}" if first_bad else ""))
    print(f"  state and scratch $C1-$C4 identical:            {all_ok}/{n} ({100.0 * all_ok / max(n, 1):.2f}%)")
    print(f"  the same, $C3 = caller's Y after $Fx excepted:  {all_but_y}/{n} ({100.0 * all_but_y / max(n, 1):.2f}%)")
    if apu is not None:
        print(f"  APU writes identical (value and order):        {apu_ok}/{apu_n} traced frames"
              f" ({100.0 * apu_ok / max(apu_n, 1):.2f}%)")


if __name__ == "__main__":
    main()
