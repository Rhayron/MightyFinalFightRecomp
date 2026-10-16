"""Mighty Final Fight's sound commands and their data, decoded from your dump.

    py -3 -I port/tools/sound_dump.py ROM --list            # every command the driver accepts
    py -3 -I port/tools/sound_dump.py ROM 0C 1B             # decode commands (hex) into events
    py -3 -I port/tools/sound_dump.py ROM --instruments     # the 8-byte instruments
    py -3 -I port/tools/sound_dump.py ROM --notes           # the period table as pitches
    py -3 -I port/tools/sound_dump.py ROM 0C --frames 300   # what the driver writes, frame by frame
                                                            # (runs port/tools/sound_model.py)

Commands are what game code passes to sound_queue_push (0F:F34A, music; it
also records the command in $D9) or to its entry at 0F:F34C (effects). The
format is described in docs/sound.md. Prints to the terminal only; redirect
under runs/sound/ if you keep it (it is ROM-derived).
"""
import argparse
import math
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sound_rom as sr  # noqa: E402

SPECIAL = {
    0xF0: "stop everything (music and effect)",
    0xF1: "stop the effect",
    0xF2: "stop the music",
    0xF3: "pause the music (channels silenced, effect keeps playing)",
    0xF4: "resume the music",
    0xF5: "fade the music in at speed Y (the Y register; never used: the queue cannot pass Y)",
    0xF6: "fade the music out at speed Y (Y = 0 cancels; never used)",
    0xF7: "pitch scale -Y for a looping effect (never used)",
}


def describe(rom, cmd):
    if cmd >= 0xF0:
        return f"{cmd:02X}  special   {SPECIAL[cmd & 0xF7]}"
    n = rom.command_count()
    eff = cmd % n if cmd >= n else cmd
    kind, ptr, info = sr.command_info(rom, eff)
    alias = f" (= {eff:02X})" if eff != cmd else ""
    if kind == "none":
        return f"{cmd:02X}  none      null pointer: ignored{alias}"
    if kind == "music":
        ch = info["channels"]
        parts = " ".join(f"{sr.CHANNELS[c]}=${ch[c]:04X}" if ch[c] else f"{sr.CHANNELS[c]}=-" for c in (3, 2, 1, 0))
        return f"{cmd:02X}  music     ${ptr:04X}  {parts}{alias}"
    rows = sr.decode_sfx(rom, ptr + 1)
    used = set()
    for _a, text in rows:
        if "channels" in text:
            used.update(text.split("channels ")[1].split("+"))
    used.discard("-")
    loop = " loops forever," if info["loops"] else ""
    chans = "+".join(c for c in ("sq1", "sq2", "tri", "noise") if c in used)
    length = "" if info["loops"] else f", {effect_frames(rom, eff)} frames"
    return f"{cmd:02X}  effect    ${ptr:04X}  priority {info['priority']:2d},{loop} channels {chans or '-'}{length}{alias}"


def effect_frames(rom, cmd, limit=4096):
    """Frames an effect plays, loops included: from the frame it is queued to
    the frame its end marker is read (sound_model, from silence)."""
    import sound_model
    drv = sound_model.Driver(rom)
    drv.update([cmd])
    n = 1
    while drv.m[sound_model.D0] | drv.m[sound_model.D1] and n < limit:
        drv.update([])
        n += 1
    return n - 1


def pitch_name(period, triangle=False):
    hz = sr.period_hz(period, triangle)
    n = round(12 * math.log2(hz / 32.7032))
    return sr.note_name(n) if 0 <= n < 120 else f"p{period}"


def channel_state(regs):
    """What the registers say each channel is doing (the 'channel state' P6 compares)."""
    enabled = regs[0x15]
    out = []
    for base, en in ((0, 1), (4, 2)):
        vol, period = regs[base] & 15, regs[base + 2] | (regs[base + 3] & 7) << 8
        if enabled & en and vol and period >= 8:
            out.append(f"{pitch_name(period):4s} v{vol:<2d} d{regs[base] >> 6} p{period:<4d}")
        else:
            out.append("--".ljust(18))
    period = regs[0x0A] | (regs[0x0B] & 7) << 8
    out.append(f"{pitch_name(period, True):4s} p{period:<5d}" if enabled & 4 and regs[8] & 0x7F and period >= 2
               else "--".ljust(11))
    vol = regs[0x0C] & 15
    out.append(f"#{regs[0x0E] & 15:<2d} v{vol:<2d}{' m' if regs[0x0E] & 0x80 else '  '}  " if enabled & 8 and vol
               else "--".ljust(12))
    return " ".join(out)


def dump_music(rom, cmd, ptr, info):
    print(f"command {cmd:02X}: music at ${ptr:04X} (byte 0 = $00 marks music)")
    gtr = sr.initial_transpose(rom, info["channels"])
    if gtr:
        print(f"  (the song sets transpose {sr.signed(gtr):+d} before its first notes: note names include it on every channel)")
    for ch in (3, 2, 1, 0):
        start = info["channels"][ch]
        note = "  (sounds an octave below the note names)" if ch == 1 and start else ""
        print(f"\n  {sr.CHANNELS[ch]} (${sr.REG_BASE[ch]:04X}): " + (f"${start:04X}" if start else "not used") + note)
        if not start:
            continue
        events = sr.decode_music_channel(rom, start, ch, gtr)
        targets = {e[4] for e in events if e[4] is not None}
        for addr, n, text, kind, target in events:
            label = f"${addr:04X}{'*' if addr in targets else ' '}"
            raw = " ".join(f"{rom.data(addr + i):02X}" for i in range(n))
            print(f"    {label} {raw:12s} {text}")
    print("\n  (* = loop, break or jump target)")


def dump_sfx(rom, cmd, ptr, info):
    loop = ", restarts itself when it ends" if info["loops"] else ""
    print(f"command {cmd:02X}: effect at ${ptr:04X}, priority {info['priority']}{loop}")
    for addr, text in sr.decode_sfx(rom, ptr + 1):
        print(f"  ${addr:04X}  {text}")


def dump_instruments(rom):
    base = rom.instr_base()
    first_song = min(p for p in (rom.command_ptr(c) for c in range(rom.command_count())) if p)
    count = (first_song - base) // 8
    print(f"instrument table at ${base:04X} (8 bytes each; {count} fit before the first sound at ${first_song:04X})")
    print("  n   attack decay sustain release  lfo_speed vib_depth trem_depth noise_mode  raw")
    for n in range(1, count + 1):
        b = rom.instrument(n)
        reset = " reset" if b[4] & 0x80 else ""
        print(f"  {n:2d}  {b[0]:4d}({rom.rate(b[0] & 31):3d}) {b[1]:2d}({rom.rate(b[1] & 31):3d})  ${b[2]:02X}"
              f"   {b[3]:2d}({rom.rate(b[3] & 31):3d})   {b[4] & 0x7F:3d}{reset:6s}  {b[5]:3d}       {b[6]:3d}"
              f"       ${b[7]:02X}     " + " ".join(f"{x:02X}" for x in b))


def dump_notes(rom):
    print("note  name  table(lo,hi)  period  hardware  pulse Hz   (index = note + 1; entry 0 doubles as thresholds)")
    for note in range(96):
        lo, hi = rom.period_entry(note + 1)
        p = sr.float_period(hi, lo)
        hw = p & 0x7FF
        flag = "" if p == hw else "  (does not fit 11 bits)"
        print(f"  {note:2d}  {sr.note_name(note):4s}  ${lo:02X},${hi:02X}      {p:5d}   {hw:5d}   {sr.period_hz(hw):9.2f}{flag}")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("rom")
    ap.add_argument("cmds", nargs="*", help="commands in hex")
    ap.add_argument("--list", action="store_true")
    ap.add_argument("--instruments", action="store_true")
    ap.add_argument("--notes", action="store_true")
    ap.add_argument("--frames", type=int, help="run the model for N frames after the commands and print the APU writes")
    a = ap.parse_args()
    rom = sr.Rom(a.rom)
    if a.list:
        n = rom.command_count()
        print(f"{n} table entries at ${sr.CMD_TABLE:04X} (commands $00-${n - 1:02X}); "
              f"${n:02X}-$EF wrap modulo ${n:02X}; $F0-$F7 special ($F8-$FF = $F0-$F7)")
        for cmd in range(n):
            print(describe(rom, cmd))
        for cmd in range(0xF0, 0xF8):
            print(describe(rom, cmd))
    if a.instruments:
        dump_instruments(rom)
    if a.notes:
        dump_notes(rom)
    cmds = [int(c, 16) for c in a.cmds]
    if a.frames is not None:
        import sound_model
        drv = sound_model.Driver(rom)
        regs = [0] * 0x18
        print("frame  sq1                sq2                tri         noise        | writes")
        for f in range(a.frames):
            writes = drv.update(cmds if f == 0 else [])
            for addr, v in writes:
                regs[addr - 0x4000] = v
            print(f"{f:5d}  {channel_state(regs)} | " + " ".join(f"{addr:04X}={v:02X}" for addr, v in writes))
        return
    for c in cmds:
        if c >= 0xF0:
            print(describe(rom, c))
            continue
        eff = c % rom.command_count()
        kind, ptr, info = sr.command_info(rom, eff)
        if kind == "music":
            dump_music(rom, c, ptr, info)
        elif kind == "sfx":
            dump_sfx(rom, c, ptr, info)
        else:
            print(f"command {c:02X}: null pointer, ignored")
        print()


if __name__ == "__main__":
    main()
