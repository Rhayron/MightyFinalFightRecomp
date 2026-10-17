# Mighty Final Fight's sound driver (PLAN_DECOMP.md P6)

The driver is a Capcom engine written for bigger cartridges than this one:
its far-pointer code maps banks `$18` and `$17` (`06:8047`), which wrap to
`$08` and `$07` on this 128 KiB board. (Its command set looks like the one
Capcom's later Mega Man games use; that comparison was not checked here.)
Code sits at the start of PRG bank `$06`, data in the rest of bank `$06` and
in bank `$07`. Everything here was read from the
disassembly (`disasm/tools/disasm.py`, plus `tools/dis6502.py` for the parts
no run executed) and checked against the cycle-accurate oracle; the
"Verification" section at the end gives the numbers. Addresses are CPU
addresses with bank `$06` at `$8000` and `$07` at `$A000`, which is how
`sound_update` maps them; `06:xxxx` is code or data in bank `$06`.

The tools that go with this page are in `port/tools/`:

| Tool | What it does |
|---|---|
| `sound_dump.py ROM --list` | every command the driver accepts and what it starts |
| `sound_dump.py ROM 0C 2B` | decodes songs and effects into events (notes with names and lengths, commands, rows) |
| `sound_dump.py ROM --instruments / --notes` | the instrument table and the period table |
| `sound_dump.py ROM 0C --frames 300` | runs the model and prints each frame's channel state and register writes |
| `sound_model.py` | the driver re-implemented in Python, routine by routine, same RAM layout |
| `sound_check.py ROM RAMFILE [--apu APUFILE]` | the model against the oracle, frame by frame |
| `sound_oracle.py` | ground truth from the oracle: per-frame RAM (`ram`), per-frame APU writes (`apu`), a command queued into a saved state (`patch`, `inject`) |
| `sound_rom.py` | the tables' addresses and the stream decoders the others share |
| `apu_render.py` | register writes to a WAV through a 2A03 model |
| `sound_wavcmp.py` | compares two WAVs of the same passage |

Their outputs are ROM-derived and belong under `runs/sound/`.

## How the game talks to it

- `sound_queue_push` (`0F:F34A`) puts the command in A into an 8-entry ring
  (`sound_queue`, `$DC-$E3`, write index `$DA`, `$88` = empty slot) and
  records it in `$D9`. A full ring (the slot at the write index is not
  `$88`) drops the command.
- `0F:F34C` is the same routine entered after the `STA $D9`: effects go
  through it, so `$D9` keeps the last *music* command. Two places read `$D9`
  to avoid restarting a song that is already playing: the round music after
  an area change (`0E:DFEE`) and the boss music `$06` (`0B:B7F1`).
- Once a frame, after the NMI (`nmi_tail`, `0E:C119`), `sound_update`
  (`0F:FF72`) maps banks `$06`/`$07`, takes commands from the read index
  `$DB` until it meets a `$88` (setting each slot back to `$88`), calls
  `sound_play` (`06:8003`) with each one, calls `sound_tick` (`06:8000`)
  once, and restores the game's banks with `bank_switch`. If the NMI landed
  inside `bank_switch` (`$F6` set) it only increments `$F7`, and
  `bank_switch` runs the update when it finishes.
- Both NMI paths reach `nmi_tail`, so the sound runs every frame, lag frames
  included. Reset (`0F:FE00`) clears all RAM and fills the queue with `$88`;
  the attract mode's `JMP reset` does the same to the driver mid-song.

The driver touches nothing else: zero page `$C0-$D8` and `$0700-$077F`
(no instruction in the other banks' disassembled code addresses them,
directly, indexed or through a pointer), APU registers `$4000-$400F`, `$4001`/
`$4005` (always `$08`) and `$4015` (always `$0F`). It never writes the DMC
registers `$4010-$4013`; the only DMC writes in the game are reset's
`$4010 = 0` and `$4015 = 0`. `$0780-$0783` belong to game code (banks `$00`
and `$04`), not to the driver.

## Commands

`sound_play` (`06:80FE`) increments `$C0`, runs `06:8106` and decrements
`$C0` again. `06:8106`:

- `$F0-$FF`: special commands, `A & 7` through the table at `06:81B5`
  (`06:81AE`; so `$F8-$FF` repeat `$F0-$F7`).
- below `$F0`: the command is reduced modulo the table size (`06:810D`; the
  size is the byte at `06:8A40`, `$36`), then `06:8118` reads the pointer at
  `06:8A43 + 2 * cmd` (big-endian: high byte first). A null pointer is
  ignored. The first data byte decides the kind: `$00` is music, anything
  else is an effect and is its priority.

The table holds 16 songs, two null entries and 36 effects:

| Commands | Kind |
|---|---|
| `$00-$0F` | music (`$8B57-$AA81`) |
| `$10`, `$11` | null pointers: ignored |
| `$12-$35` | effects (`$AA82-$AE58`) |
| `$F0` | stop everything (`06:81C5`: stop music, then stop the effect) |
| `$F1` | stop the effect (`06:81C8`) |
| `$F2` | stop the music (`06:81E4`) |
| `$F3` | pause the music: `$C0` bit 1, its channels silenced (`06:821E`) |
| `$F4` | resume the music (`06:8226`) |
| `$F5`, `$F6`, `$F7` | fade in, fade out, effect pitch scale, with the Y register as the argument (`06:822D`, `06:8234`, `06:824A`); never used |

Every special command stores the caller's Y register in `$C3`
(`STY $C3`, `06:81AE`). Through the queue that Y is whatever the
interrupted game code had in Y, which no RAM image shows; `$C3` is scratch
and is overwritten by the next sound activity, but until then it is the
only byte of driver RAM that cannot be predicted from the commands.

What the game plays (`LDA #cmd` before a push, found statically, plus what
the coverage runs saw):

| Command | Used for | Pushed at |
|---|---|---|
| `$00-$04` | rounds 1-5 | `round_music` (`0E:C83C`) by `$22` at the game start (`0E:C75A`), by `$61` after areas (`0E:DFEB`); `04:962F` from `04:965E` |
| `$05` | special area / boss (seen in the round 2 boss state) | `00:972F`, `0F:E09C` |
| `$06` | boss, unless already playing | `0B:B7EF` |
| `$07` | intro story | `0E:C5DE` |
| `$08` | after round 5 is cleared (ending) | `0E:DEA1` |
| `$09` | | `0B:B349` |
| `$0A` | continue screen (seen at game action 9) | `04:9328` |
| `$0B` | a one-shot jingle (229 frames) | `04:951E` |
| `$0C` | title | `0E:C659` |
| `$0D` | character select | `04:917E` |
| `$0E`, `$0F` | one-shot jingles (203 and 384 frames): `$0F` in round 5, `$0E` otherwise (a code path no run reached) | `00:92F2`; `$0F` also `0B:BDF2` |
| `$15` | pause, followed by `$F3` or `$F4` (`LDY #$F3` / `INY` / `TYA`, `0E:C925-0E:C930`) | `0E:C922` |
| `$F0` | at boot (`main_task`) and in bank `$04` | `0E:C560`, `04:8D02` |
| `$F2` | before the continue screen (seen with game action 9) | `0E:DC72`, `0B:B785` |

The effects. Frames: how long each one runs in the oracle when queued
alone over the title music (from the start until its end marker); pushed
at: `LDA #cmd` right before `JSR $F34A/$F34C`; first seen: the coverage
run, frame and game action (`$30`) where the game first queued it. Effects
with no static caller are pushed from a register or a table.

| Cmd | Priority | Channels | Frames | Pushed at | First seen |
|---|---|---|---|---|---|
| `$12` | 9 | sq2+noise | 70 | 0B:BA5E | - |
| `$13` | 9 | sq2+noise | 11 | 00:873C, 00:88F1, 04:8EF5, 0B:BE62 | haggar frame 4670, game action $00 |
| `$14` | 18 | sq2+tri | 60 | 0C:8AB8 | - |
| `$15` | 18 | sq2 | 23 | 0E:C922 | cody frame 18909, game action $00 (pause) |
| `$16` | 9 | sq2+tri+noise | 70 | 00:8CDC, 04:8DE9, 09:AB56 | cody frame 4638, game action $00 |
| `$17` | 7 | noise | 8 | 0F:F432 | cody frame 5285, game action $03 |
| `$18` | 9 | sq2+noise | 8 | 01:A0B6, 03:B780, 0B:B8ED, 0C:85DC, 0C:8604, 0C:8624, 0D:A31B | cody frame 5333, game action $03 |
| `$19` | 9 | tri+noise | 23 | - | - |
| `$1A` | 9 | tri+noise | 17 | - | guy frame 5406, game action $03 |
| `$1B` | 7 | noise | 9 | 00:81DE, 00:8268, 00:8F0B, 04:8E91, 09:ABEB, 0D:A633, 0E:CA9C, 0E:D880 | cody frame 5149, game action $01 |
| `$1C` | 7 | tri+noise | 6 | 00:80B0, 00:8344, 0E:D041 | cody frame 5187, game action $02 |
| `$1D` | 7 | sq2+noise | 10 | 00:82B6, 09:ACF7 | - |
| `$1E` | 7 | noise | 7 | 0E:D3EA | - |
| `$1F` | 9 | tri+noise | 10 | 03:B94B | - |
| `$20` | 8 | sq2+noise | 22 | 00:8380, 0C:97FF, 0C:98F9, 0C:9A17 | cody frame 6110, game action $17 |
| `$21` | 7 | sq2+noise | 14 | 00:959F, 00:9677 | haggar frame 14177, game action $1B |
| `$22` | 17 | sq1+sq2 | 43 | 05:A75B | - |
| `$23` | 7 | tri+noise | 29 | - | - |
| `$24` | 7 | sq1+sq2+tri+noise | 167 | 00:94BD | - |
| `$25` | 9 | sq2+noise | 7 | 00:880B, 00:8878 | cody frame 6084, game action $17 |
| `$26` | 7 | sq2+tri+noise | 6 | 0C:8571, 0C:8B66 | guy frame 8546, game action $03 |
| `$27` | 9 | tri+noise | 11 | - | haggar frame 6061, game action $03 |
| `$28` | 9 | noise | 52 | 00:8DF7, 0E:DA54 | cody frame 6734, game action $06 |
| `$29` | 8 | sq2+tri+noise | 19 | 03:BC06 | - |
| `$2A` | 8 | tri+noise | 14 | 05:AB8F, 0D:A84D | - |
| `$2B` | 7 | sq2 | 7 | 00:9A77, 09:A767 | cody frame 441, game action $00 (intro text) |
| `$2C` | 8 | sq2+noise | 37 | 00:8128, 09:ACC8, 0B:BAAD | - |
| `$2D` | 18 | sq2 | 51 | 0C:8BD8, 0C:8BEF | - |
| `$2E` | 18 | sq2 | 12 | 04:9B3E, 05:A7F3, 0C:8BC4 | - |
| `$2F` | 17 | sq2 | 10 | 04:91E7, 04:93D0, 04:93F2, 04:9A00 | cody frame 17693, game action $09 |
| `$30` | 17 | sq1+sq2 | 21 | 04:9191, 04:9422, 04:9BA8, 05:AD6C, 05:AD8A, 05:ADA6, 05:ADC5, 0E:C6DA | cody frame 4520, game action $00 (START on the title) |
| `$31` | 17 | sq2 | 11 | 05:AAE2 | cody frame 17045, game action $1D |
| `$32` | 17 | sq1+sq2+noise | 29 | 0E:C6B3, 0E:DD5B | cody frame 4466, game action $00 |
| `$33` | 8 | sq2 | 12 | 00:84AF, 0C:8FC8, 0C:9359, 0E:D825 | - |
| `$34` | 16 | sq2+noise | 17 | 00:896D, 00:8F43, 09:ABF7 | - |
| `$35` | 7 | sq2 | 7 | - | - |

`$28` is the only effect with a loop: `01 02 AC F8` jumps back twice, so
its first row (13 frames) plays three times before the last one (13
frames), 52 frames in all.

## Each frame (`sound_tick`, `06:806C`)

1. If `$C0` bit 0 is set, do nothing (a `sound_play` in progress; never the
   case here, since both run from `sound_update` one after the other).
2. If an effect is playing (`$D0/$D1` not 0), step its sequencer
   (`06:8252`).
3. Tempo: `$C8 += $CA`, and `$C7 = $C9 + carry`, the number of ticks the
   music advances this frame (`06:807A`).
4. For each channel, pulse 1, pulse 2, triangle, noise (`X` = 3, 2, 1, 0):
   if the effect owns it (`LSR $CF`, carry), set `$CF` bit 7 and run the
   effect channel (`06:82DE`); then, unless the music is paused (`$C0` bit
   1), run the music channel (`06:8393`).
5. Clear `$C0` bit 0; if a fade is on (`$CC & $7F`), advance it (`06:80B6`).

The channel index `X` used everywhere: 3 = pulse 1 (`$4000`), 2 = pulse 2
(`$4004`), 1 = triangle (`$4008`), 0 = noise (`$400C`); the register
offset is `(3 - X) * 4` (`06:80D8`, `06:80EC`).

## Music and effects sharing the channels

There is one music track (four channel streams) and one effect at a time.

- **Priority.** An effect's first byte is its priority (bits 0-6) and a
  loop flag (bit 7). A new effect starts only if its priority is at least
  the current one in `$CE` (`06:8133`); `$CE` drops to 0 when the effect
  ends. The game uses priorities 7, 8, 9, 16, 17 and 18, and no effect sets
  the loop flag. Starting an effect clears its four channels' state
  (`$0700-$0727`) and its sequencer variables (`$D2-$D5`) but keeps `$CF`.
- **Ownership.** Each effect row names the channels it uses (`$CF`, bit 0 =
  pulse 1 ... bit 3 = noise). The music keeps running on owned channels
  (notes, envelopes and slides advance, so it stays in time) but writes no
  registers there (`06:8724`: `$CF` bit 7 jumps to the slide/LFO update).
- **Handing channels back.** When a row's mask differs from the previous
  one, `06:81D4` silences every channel whose ownership changed (both ways)
  and marks the music channels still playing for a full period rewrite
  (`$077C+ch = $FF`, `06:81F1`). The same happens when the effect ends
  (`06:81C8`). The music is audible again from the next frame, in the
  middle of its note.
- **Looping effects** (unused here): `$D6` bit 7 and `$D7 = 2 * cmd + 1`
  (`06:8145`); at the end marker `06:825B` starts the command again.

## RAM

Zero page (all written only by bank `$06`):

| Address | Meaning |
|---|---|
| `$C0` | bit 0: inside `sound_play` (`06:80FE`), and "a new effect row was read this frame" (`INC $C0`, `06:82CB`; cleared at `06:80AC`); bit 1: music paused (`$F3`/`$F4`); bits 4-7: fade fraction |
| `$C1/$C2` | scratch: data pointer (`06:803A`), product high/low (`06:8006`), jump-table target (`06:8023`) |
| `$C3`, `$C4` | scratch: command argument, multiplier, register offset of the last write (`06:80EC`); `$C3` = caller's Y after `$F0-$F7` |
| `$C5/$C6` | pointer to the instrument of the channel being processed (`06:8684`) |
| `$C7` | ticks this frame |
| `$C8` | tempo accumulator (fraction) |
| `$C9/$CA` | tempo in ticks per frame, integer and fraction; music start sets 1 and `$99` (1.6, 120 BPM) |
| `$CB` | global transpose (music command `$0A`) |
| `$CC/$CD` | fade speed (bit 7 = fade in) and fade level |
| `$CE` | priority of the current effect |
| `$CF` | channels the effect owns (bit 0 pulse 1, 1 pulse 2, 2 triangle, 3 noise); bit 7 during the channel loop = the current channel is owned |
| `$D0/$D1` | effect stream pointer (0 = no effect) |
| `$D2` | effect transpose (row header bit 2; unused) |
| `$D3` | frames left in the current effect row |
| `$D4` | effect gate ratio (row header bit 1) |
| `$D5` | frames until the effect's notes release: `($D3 * $D4 >> 8) + 1` at the row start |
| `$D6` | effect loop counter (bits 0-6), loop flag (bit 7) |
| `$D7` | `2 * cmd + 1` of a looping effect, else 0 |
| `$D8` | effect pitch scale (`$F7`; unused) |

Channel arrays, four bytes each, indexed by `X`. The effect uses `X` = 0-3,
the music `X` = `$28` + channel, so the same code serves both (`06:8393`
starts with `ORA #$28`). In every range below the lowest address is the
noise channel and the highest pulse 1 (`X` = 3 or `$2B`).

| Array (`+X`) | Effect | Music | Meaning |
|---|---|---|---|
| `$0700` | `$0700-$0703` | `$0728-$072B` | instrument number (1-based; 0 = none: the music channel then writes nothing) |
| `$0704` | `$0704-$0707` | `$072C-$072F` | bits 0-2 envelope phase (0 attack, 1 decay, 2 sustain, 3 release, 4 off); bit 3 instrument just changed; bit 5 sliding; bits 6-7 LFO quarter |
| `$0708` | `$0708-$070B` | `$0730-$0733` | LFO phase |
| `$070C` | `$070C-$070F` | `$0734-$0737` | duty (bits 6-7), `$30`, volume (bits 0-3); for the effect triangle a length ratio |
| `$0710` | `$0710-$0713` | `$0738-$073B` | envelope level `$00-$F0`; for the effect triangle, frames left |
| `$0714` | `$0714-$0717` | `$073C-$073F` | detune, signed, added to the final period |
| `$0718` | `$0718-$071B` | `$0740-$0743` | slide speed: bit 7 sign, bits 0-6 half the step |
| `$071C` | `$071C-$071F` | `$0744-$0747` | target note (note + 1) for slides and portamento |
| `$0720`, `$0724` | `$0720-$0727` | `$0748-$074F` | pitch, low and high byte of the pseudo-float (noise: `$0724` = period index) |
| `$0728`, `$072C` | - | `$0750-$0757` | stream pointer, low and high |
| `$0730` | - | `$0758-$075B` | bits 0-2 octave; 3 octave + 2; 4 dotted; 5 triplet; 6 tie; 7 tied to the previous note |
| `$0734` | - | `$075C-$075F` | channel transpose |
| `$0738` | - | `$0760-$0763` | ticks left in the note |
| `$073C` | - | `$0764-$0767` | gate ratio (`/256` of each note) |
| `$0740` | - | `$0768-$076B` | ticks left until release |
| `$0744` + 4 * slot | - | `$076C-$077B` | loop counters 0-3 |
| `$077C` + (`X` & 3) | `$077C-$077F` | (shared) | last value written to the period-high register; `$FF` forces a rewrite |

Music start (`06:816F`) clears `$0728-$077B`; an effect start clears
`$0700-$0727` (`06:8159`).

## Music data

A song is `$00` followed by four big-endian stream pointers: pulse 1,
pulse 2, triangle, noise (`06:8189`, stored X = 3 down to 0). A null pointer
leaves the channel silent. Each channel stream is a byte code read by
`06:83CD` until it reaches a note:

**Notes** (`$20-$FF`, `06:83DA`): bits 5-7 are the length code 1-7, bits
0-4 the pitch 1-31 (0 = rest).

| Length code | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
|---|---|---|---|---|---|---|---|
| ticks (`06:891C`) | 3 | 6 | 12 | 24 | 48 | 96 | 192 |
| triplet (`06:8915`) | 2 | 4 | 8 | 16 | 32 | 64 | 128 |

192 ticks is a whole note. The dot flag multiplies the next note by 1.5
and clears itself (`06:83F6`); it is ignored while triplets are on. The
length is added to what is left of the previous note (`06:8406`), so a
note that overran by a fraction of a tick is shortened by the same amount
and the tempo stays exact. With `T` ticks per frame, a quarter note lasts
`48 / T` frames, `75 T` beats per minute.

The note number is `octave_base[flags & $0F] + pitch - 1 + $CB + channel
transpose` (`06:8466`), with `octave_base` (`06:8923`) = 0, 12, ..., 84 for
octaves 0-7 and 24 more when flag `$08` is set. Note 0 is C1 (32.7 Hz),
note 9 is A1 (55 Hz on a pulse, half that on the triangle); the table goes
up to 95 (B8) and clamps there (`06:85DE`). Notes 0-8 have periods beyond
11 bits and come out wrong on the hardware; no song goes below A2. On the noise channel the pitch is a period index instead:
`((pitch - 1) & 15) ^ 15` (`06:8454`), so pitch 1 is the lowest noise.

A rest starts the release (`06:85A3`) and sets the gate counter to `$FF`.
A note sets the gate to `max(1, ticks * gate_ratio >> 8)` (`06:8419`),
starts the envelope (`06:85AE`) and the pitch (`06:85DE`). When the
previous note had the tie flag, the note is tied: no new attack, the gate
stays at `$FF`, and the pitch does not change unless a portamento speed is
set (`06:8430`).

**Commands** (`$00-$18`, jump table `06:84A7`; commands from `$04` up take
an argument, read at `06:849B`):

| Byte | Arguments | Effect | Code |
|---|---|---|---|
| `$00` | | toggle triplet (flag `$20`) | `06:84D9` |
| `$01` | | toggle tie (flag `$40`) | `06:84DD` |
| `$02` | | dot the next note (flag `$10`) | `06:84E1` |
| `$03` | | toggle octave + 2 (flag `$08`) | `06:84E8` |
| `$04` | `ff` | flags AND `$97`, OR `ff`: sets octave+2, triplet and tie at once | `06:8575` |
| `$05` | `ii ff` | tempo: `$C9 = ii`, `$CA = ff`, `$C8 = 0` (affects all channels) | `06:84F1` |
| `$06` | `gg` | gate ratio: release after `gg/256` of each note | `06:84FF` |
| `$07` | `vv` | volume (low nibble of `$070C`) | `06:865A` |
| `$08` | `nn` | instrument `nn + 1`; a change resets the LFO at the next note | `06:866F` |
| `$09` | `oo` | octave 0-7 | `06:8505` |
| `$0A` | `tt` | global transpose `$CB` (signed) | `06:8510` |
| `$0B` | `tt` | channel transpose (signed) | `06:8515` |
| `$0C` | `dd` | detune: signed, added to the period | `06:86A1` |
| `$0D` | `ss` | portamento speed (0 = off) | `06:86A7` |
| `$0E-$11` | `cc hh ll` | loop 0-3: jump to `hhll` `cc` times (the section plays `cc + 1` times) | `06:8529` |
| `$12-$15` | `ff hh ll` | break out of loop 0-3: on that loop's last pass, set flags as `$04 ff` and jump to `hhll` | `06:8547` |
| `$16` | `hh ll` | jump | `06:855A` |
| `$17` | | end of the channel: pointer 0, channel silenced (unless the effect owns it) | `06:8580` |
| `$18` | `dd` | duty (bits 6-7 of `$070C`) | `06:86AD` |

Each channel has its own four loop counters (`$076C-$077B`), so loops of
different slots nest. A loop counter is 0 when idle; the loop command loads
it with `cc` on the first pass and counts it down (`06:8529`); a break
fires when the counter is 1, that is during the last pass. The data uses
every command; most songs end in a `$16` jump back, and the title (`$0C`)
and a few jingles end with `$17`.

Timing inside a frame: `06:8393` first outputs the current note (envelope
and registers, `06:86BA`), then counts the gate and the duration down by
`$C7`. A note read in frame `f` is first heard in frame `f + 1`.

## Effect data

After the priority byte, an effect is a list of rows (`06:825B`):

| Byte | Present when | Meaning |
|---|---|---|
| header | always | bit 7: end of the effect; bit 0: loop; bit 1: gate byte follows; bit 2: transpose byte follows |
| `cc hh ll` | header bit 0 | loop: jump to `hhll` `cc` times (one global counter, `$D6`); `cc = 0` jumps always |
| `gg` | header bit 1 | gate ratio `$D4` |
| `tt` | header bit 2 | transpose `$D2` (no effect uses it) |
| `nn` | always | the row lasts `nn + 1` frames |
| `mm` | always | channel mask (bit 0 pulse 1 ... bit 3 noise) |
| per channel | for each bit of `mm`, pulse 1 first | a field mask, the fields, a note |

A channel's entry (`06:830A`) is a mask byte whose bits select the fields
that follow, in this order: bit 0 instrument (`nn + 1`), bit 1 duty, bit 2
volume, bit 3 slide speed, bit 4 detune (table `06:8329`). Then the note:

- `0`: off: level 0, phase 4, channel silenced at once (`06:8339`);
- bit 7 set: hold: keep the pitch, no new attack (except on the triangle,
  which restarts its timer);
- otherwise: note number `note - 1 + $D2`, absolute (no octave); on the
  noise channel `(note - 1) ^ $0F` is the period index (`06:8374`).

Every note also sets the slide target `$071C` (`06:8349`): `$54`, that is
note 83 (B7), when the slide speed is negative (rising), `$0A`, note 9
(A1), when it is positive (falling), so an effect slide runs to the end of
that range. The noise channel (`X` = 0) never stops sliding (`06:88E3`),
which gives the long noise sweeps.

Rows are read at the start of a frame (`06:8252` counts `$D3` down, then
reads the next row); the channel entries are parsed in the same frame, but
the first register writes come in the next one (`06:82E6`: only the
parse, no output, while `$C0` bit 0 is set). After that, each frame runs the
envelope and output; a note releases when `$D5` reaches 0 (gate) or, on
the triangle, when its frame counter `$0710` does (`06:82FB`). `$D4` is 0
when an effect starts, so without a gate byte notes release on their first
frame and are heard through the instrument's release.

The sound data fills `$8B57-$AE58` with no gaps: `sound_dump.py`'s decoders
reach every byte except 41 `$17` end markers that follow channels ending in
a jump. No pointer is a "far" one (`$C000` and up, which `06:8047` would
read from bank `$18`, that is `$08` on this 128 KiB board).

## Instruments

Eight bytes each, from `06:8AAF` (the pointer is at `06:8A41`/`06:8A42`;
21 fit before the first song). Numbered from 1 by the streams (`$08 nn` and
the effects' field 0 store `nn + 1`).

| Byte | Meaning | Code |
|---|---|---|
| 0 | attack step (index into the rate table) | `06:86D1` |
| 1 | decay step (0 = straight to sustain) | `06:86E6` |
| 2 | sustain level (`$00-$F0`) | `06:86F5` |
| 3 | release step (0 = hold the level) | `06:8702` |
| 4 | LFO speed (bits 0-6, per frame); bit 7: reset the LFO at every note | `06:88FA`, `06:863E` |
| 5 | vibrato depth | `06:87B8` |
| 6 | tremolo depth (only from 5 up) | `06:876B` |
| 7 | ORed into `$400E`: `$80` = short (metallic) noise | `06:883D` |

The rate table (`06:8933`, 32 bytes) maps the step indexes to 0, 1, 2, 3,
4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 16, 18, 19, 20, 22, 24, 27, 30, 35,
40, 48, 60, 80, 126, 127, 254, 255 per frame.

**Envelope** (`06:86BA`, phase in `$0704` bits 0-2, level in `$0710`): key
on sets phase 0 and level 0 (`06:85AE`). Attack adds its step until the
level reaches `$F0`; decay subtracts its step down to the sustain level;
sustain holds; release (from the gate, a rest or a row's gate) subtracts
its step down to 0 and then phase 4 (off), after which the channel writes
nothing at all. The music triangle skips all that: key on gives level 1 in
sustain, release gives 0 at once (`06:8702`).

**Volume** (`06:8760`): attenuation = `15 - (level >> 4)`; the tremolo
can deepen it to `(LFO * depth) >> 10` (16 or more mutes); the register gets
`$070C - attenuation`, or the duty bits alone when that underflows into
bit 4 (`BIT $C4` with `$C4 = $10`, `06:87A1`). The triangle has no volume: `$4008` = `$FF`
while the level is not 0, `$00` otherwise (`06:8752`).

**LFO** (`06:88FA`): `$0708` grows by the speed every frame; each
overflow advances the quarter in `$0704` bits 6-7. The vibrato and the
tremolo read it as a triangle wave: phase, or `255 - phase` in odd
quarters (`06:87C0`, `06:8775`); the vibrato adds its offset in quarters
0-1 and subtracts it in 2-3 (`06:87E7`). The LFO
restarts at a note when instrument byte 4 has bit 7 or the instrument just
changed (`06:8636`). The vibrato skips the first frame of a note (`$077C`
is `$FF`, `06:87B3`).

## Pitch

Pitches are kept as a pseudo-float (`$0724` high, `$0720` low): the high
byte is `7 * octave + r` with `r` in 0-6, and the period is
`((r + 7) * 256 + low) >> (7 - octave)` (`06:8849`, octave found against
the thresholds 0, 7, ..., 49 at `06:8953`). The period table (`06:8959`,
`(low, high)` pairs indexed by note + 1; entry 0 doubles as the last two
thresholds) is equal-tempered from C1 up: note 9 (A1) is period 2033, 55.0
Hz. Because the representation is logarithmic, slides and vibrato move by
musical intervals at the same rate in every octave.

- **Detune** is added to the 11-bit period after the conversion
  (`06:8870`), a fixed offset in period units.
- **Slides** (`06:88A0`): while `$0704` bit 5 is set, the pitch moves by
  twice the speed every frame, in the pseudo-float; when it reaches or
  passes the target note it snaps to it and stops (never on the effect noise
  channel).
- **Portamento** (music, `06:85FC`): with a speed set by `$0D`, a new note
  slides from the previous target note to the new one; the speed's sign is
  set from the direction.
- **Writes** (`06:8884`): the low byte every frame; the high byte (`| $08`:
  length counter index 1) only when it changed, kept in `$077C+ch`, because
  writing `$4003/$4007` restarts the pulse waveform and would click. Key on
  (when the channel is not taken by the effect) and every change of
  ownership set `$077C` to `$FF` to force it.
- **Noise**: `$400E = (pitch high & 15) | instrument byte 7` every frame,
  `$400F = $08` only after a key on (the cached high byte is then `$FF`,
  `06:8835`).
- `06:81F1` writes `$4001 = $4005 = $08` (sweep off with the negate bit,
  which keeps the sweep unit from muting low notes) and `$4015 = $0F` at
  every start, stop and change of ownership.

## Quirks a reimplementation must keep

These matter for byte-exact RAM, which is what the lockstep tests compare:

- `$C3` = caller's Y after a special command (above).
- The vibrato's add path (`06:87F8`): when the sum's high byte is 0 the
  code falls into the subtract path with the sum already in `$C2`.
- `06:8289`: an effect loop whose target address has a low byte of 0 would
  fall into `06:8296` and pull a status byte that was never pushed. No
  effect does it; `sound_model.py` raises an error if one ever does.
- The music gate counter keeps counting on tied notes and rests (`$FF`
  ticks) and releases again when it wraps.
- Music channels with instrument 0 run no envelope and write nothing.
- The music start does not touch the effect, and an effect start does not
  touch the music.
- Commands `$36-$EF` wrap modulo `$36` (`06:810D`, never executed).

## Verification

**Model against the oracle, RAM.** `sound_check.py` copies the driver's RAM
from the oracle once, at the first frame after reset (or the first frame of
a run that starts from a saved state), and from then on gives
the model only the commands: for frame `f`, the ones in the oracle's queue
at the end of frame `f - 1` (what `sound_update` drains). The model's
`$C0-$D8` and `$0700-$077F` are compared with the oracle's frame log
every frame. The oracle's frame starts at the NMI and its frame-log record
is taken at the next NMI, so frame `f`'s record already includes frame
`f`'s sound update. A twin model run with another Y tells which `$C3`
differences come from the caller's Y. Game resets (attract mode) are
applied to the model as resets.

| Run | Frames | Frames with commands | Driver state identical | All bytes identical | All but Y-dependent `$C3` |
|---|---|---|---|---|---|
| power-on, no input (intro, title, attract restart) | 6,994 | 241 | 100% | 87.53% | 100% |
| `inputs/10_cody_poweron.txt` | 19,995 | 464 | 100% | 97.82% | 100% |
| `inputs/10_guy_poweron.txt` | 19,995 | 616 | 100% | 97.79% | 100% |
| `inputs/10_haggar_poweron.txt` | 19,995 | 339 | 100% | 97.82% | 100% |
| `states/rounds/cody_r2` / `_r3` / `_r4` / `_r5`, no input | 4 x 4,999 | 0 / 40 / 57 / 1 | 100% | 100% | 100% |
| `states/r2boss_guy_bot`, `god4_guy_from`, `god5_cody_from`, `map_cody_r3`, no input | 4 x 3,999 | 12 / 47 / 0 / 1 | 100% | 100% | 100% |
| each song `$00-$0F` queued at the end of frame 299, no input | 16 x 1,700 | 119 (the song, then the game's `$2B`) | 100% | 100% | 100% |
| each effect `$12-$35` queued at the end of frame 4509 (title music) | 36 x 250 | 1 | 100% | 100% | 100% |
| song `$06` queued in `states/rounds/cody_r2` (no other command for 5,000 frames), no input | 2,500 | 1 | 100% | 100% | 100% |

The open loop never resynchronizes: one wrong byte would propagate. The
game runs cover songs `$00-$05`, `$07`, `$0A`, `$0C`, `$0D`, the pause
(`$F3`, `$F4`), `$F0`, `$F2` and 20 effects in context. The last two rows
cover every command the table holds: `sound_oracle.py inject` writes the
command into the queue of a saved state the way `sound_queue_push` would,
the oracle's own driver plays it, and the model starts from the patched
state's RAM, so it handles the command itself (each song from its first
byte; each effect to its end marker, with the title music underneath).
Songs `$0B`, `$0C`, `$0E` and `$0F` end with `$17` after 229, 683, 203 and
384 frames, the others loop. The effect lengths the oracle shows are the
ones `sound_dump.py --list` computes with the model.

Paths no run exercised (the bank `$06` code the project's CDL marks as
never executed, confirmed for these runs by a line tracer over
`sound_model.py` and, for the one-line branches, by probes): far pointers
(`06:8047`), the command wrap (`06:8112`), null commands `$10`/`$11`, `$F1`
on its own, the fade (`$F5`/`$F6`, `06:80B6`, `06:8740`, `06:875B`), the
pitch scale (`$F7`, `06:881C`), looping effects and a priority-0 effect
cutting one (`06:813F`, `06:814D`, `06:826A`), the effect transpose byte
(`06:82B3`), a hold note on the effect triangle (`06:8365`), a gate that
rounds to 0 ticks (`06:8428`), the clamp of notes above 95 (`06:85E2`), the
vibrato's fall-through (`06:87F8` not taken), and page crossings of an
effect pointer or a song header (`06:8157`, `06:818D`, `06:819D`,
`06:82A4`). `sound_model.py` implements them from the disassembly,
unverified. A negative detune (`06:8879`) was on that list too: the only
one in the data (song `$06`, `$9E1A`) comes 1,824 frames into the song, so
the last RAM row and the last register row below play the song long enough
to reach it.

**Model against the oracle, registers.** `sound_oracle.py apu` takes every
write to `$4000-$4017` of each frame from the oracle's `--trace-frame`
(one run per frame, from a state saved at most 50 frames before). The
model's writes, in order, equal the oracle's (`$4000-$4013`, `$4015`) in
**1,700 of 1,700** traced frames, the model running open loop (from
power-on, or from the patched state for the last row):

| Passage | Frames | Identical |
|---|---|---|
| title music, no input, frames 4425-4774 | 350 | 350 |
| round 1, Cody's route, frames 4775-5474 (music and effects `$1B $1C $17 $18`) | 700 | 700 |
| Guy's route, its four busiest effect windows (frames 8550-8699, 9750-9899, 15150-15299, 16000-16149) | 600 | 600 |
| song `$06` in the round 2 state, frames 66080-66129 (its negative detune on pulse 2) | 50 | 50 |

**Rendering.** `apu_render.py` against the oracle's `--wav-out` of the same
frames (`sound_wavcmp.py`; oscillator phases differ, so it compares
loudness envelopes and spectra; two renders of the same writes with random
starting phases give envelope r = 0.983, cosine 0.924, 3.27 dB, the floor of
these measures):

| Passage | Render of | RMS (render / oracle) | Envelope r | Spectral cosine | Mean dB difference |
|---|---|---|---|---|---|
| title, 350 frames | the oracle's own writes | 1123.9 / 1120.8 | 0.977 | 0.915 | 3.42 |
| title, 350 frames | the model's writes | 1117.5 / 1120.8 | 0.979 | 0.911 | 3.48 |
| round 1, 700 frames | the model's writes | 1373.3 / 1373.1 | 0.967 | 0.936 | 2.71 |

The port's synthesizer, below, reproduces the oracle's WAV sample for sample.

## The synthesizer (synth_2a03)

`port/src/audio/synth_2a03.c` and `.h`: the 2A03's sound as a small C11
library, with no globals and nothing outside the C library. Two pulse
channels (duty, envelope, sweep and its mute, length counter), the triangle
(linear counter), noise (both modes), the DMC (sample fetches through a
callback, IRQ), the frame counter (4- and 5-step, IRQ), the nonlinear mixer
and the NES front-loader's output stage (90 Hz and 440 Hz high-pass, 14 kHz
low-pass), averaged down to the output rate.

```c
Synth2A03 apu;                                   /* about 34 KB, most of it the output buffer */
synth_init(&apu, 48000, dmc_read, user);         /* power-on, CPU cycle 0 */
synth_write(&apu, 0x4003, 0x08, cycle);          /* $4000-$4017, stamped with a CPU cycle */
uint8_t status = synth_read_status(&apu, cycle); /* $4015 */
synth_run(&apu, frame_end);                      /* everything before frame_end */
int n = synth_take_samples(&apu, pcm, max);      /* 16-bit mono at the init rate */
```

The functions are `synth_*`, not `apu_*`: the runtimes the scaffold links
(`runner/src/apu.c`, `runner/cyc/hw_apu.c`) already define `apu_write` and
`apu_read_status`. `synth_irq` gives the IRQ line. The header documents the
time base: a write stamped `cycle` is first seen by that cycle, even cycles
are the APU's put half-cycles.

Inside, each CPU cycle follows the rules of the cycle runner's APU (frame
counter steps and their interrupt flag, the length-load race against half
frames, halt flags taking effect a cycle late, timers on get cycles), so the
levels match cycle for cycle; the mixer adds the same single-precision tables
and the output stage runs the same double-precision filters on the mean
level of each sample's cycles. Most cycles only count timers and add the mix
(`run_fast`); frame counter, length and DMC work is scheduled on the cycles
that have it, and the mix is recomputed only when a channel output changes.
Worst case (every channel busy, noise at its shortest period, `synth_selftest`)
it makes 60 s of audio in 0.5-0.65 s, under 0.2 ms per frame; Cody's whole
20,000-frame route replays in 2.7 s with the file reading and writing. What
it cannot know is when the CPU lets a DMC fetch through: it fetches 4 cycles
after the output unit asks and 5-6 after `$4015` starts a sample, the
console's timing when the CPU is reading on those cycles. Bit 5 of `$4015`
(open bus) reads 0.

**How it is checked** (`port/tests/synth`, its own CMake project):

- `capture/oracle_capture.c` is linked into copies of the oracle made from
  their own object files, unchanged: `mff_apu_capture` from
  `build_headless` (the game) and `interp_apu_capture` from `build_cyc`
  (`cyc_interp`, any ROM). It installs the runner's per-cycle trace observer
  (`cyc_trace_cycle_hook`; `--hash-out` turns the trace on) and writes the
  runner's APU state at the start of the run, every write to `$4000-$4017`
  and every `$4015` read with the APU clock it lands on, the frame starts,
  and with `MFF_APU_LEVELS=A:B` the five channel outputs after each clock.
  The copy's `--wav-out` is byte for byte the oracle's.
- `synth_replay` starts the synthesizer from that state (the runner's
  `HwApu`, decoded), plays the writes and reads, and compares the WAVs over
  frame windows, each read, and each logged cycle's outputs. `--quantize`
  replaces the cycles with the burst the port will use, `--phase` starts the
  oscillators at random points, `--prg` serves DMC fetches from an NROM image.
- `synth_check.py --oracle-root <checkout>` runs every passage below in
  parallel; `make_apu_rom.py` writes the NROM program for what the game never
  does (envelopes that decay and loop, sweeps muting both ways, linear counter
  values, both noise modes, lengths running out, halts and `$4015` disables,
  the 4-step frame interrupt polled through `$4015`, the 5-step mode, the DMC
  one-shot with its IRQ, looping, at several rates, `$4011` writes).
- `synth_selftest` checks the documented timings without the oracle.

Results (2026-10-17, every window from its first frame to its last):

| Passage | Frames | Samples | Differ |
|---|---|---|---|
| intro story (`$07`), power-on, no input | 2062-4428 | 1,890,487 | 0 |
| title (`$0C`) | 4429-5120 | 552,690 | 0 |
| character select (`$0D`), Cody's route | 4547-4637 | 72,680 | 0 |
| round 1 (`$00`) and its effects, Cody's route | 4778-17664 | 10,292,649 | 0 |
| continue screen (`$0A`) | 17685-18809 | 898,521 | 0 |
| round 1 and its effects, Guy's route | 4761-16464 | 9,347,806 | 0 |
| round 2 (`$01`), `states/rounds/cody_r2` | 64269-69268 | 3,993,423 | 0 |
| round 3 (`$02`) and effects, `cody_r3` | 115869-120868 | 3,993,423 | 0 |
| round 4 (`$03`) and effects, `cody_r4` | 239869-244868 | 3,993,423 | 0 |
| round 5 (`$04`) from its first note, `cody_r5` | 280160-285076 | 3,927,133 | 0 |
| round 2 boss (`$05`), `r2boss_guy_bot` | 144861-148860 | 3,194,738 | 0 |
| final boss (`$05`), `god5_cody_from` | 335869-339868 | 3,194,738 | 0 |
| ending (`$08`), tour state `runs/cdl/tour2_cody_21/f054669` | 54669-55588 | 734,789 | 0 |
| credits (`$03`, typing `$2B`), same run | 55589-57668 | 1,661,265 | 0 |
| each song `$00-$0F` from its first note (16 runs) | 1,700 each | 21,724,209 | 0 |
| each effect `$12-$35` over the title music (36 runs) | 250 each | 7,188,156 | 0 |
| the APU test program on `cyc_interp` | 0-759 | 606,939 | 0 |

77.3 million samples, all identical (maximum difference 0, correlation 1);
the 122 `$4015` reads (all from the test program: the game never reads it)
give the same values; and the channel outputs match on all 6,278,293 logged
cycles (the title music's first 60 frames, and the test program's frame
interrupt and DMC part).

With writes that do not carry the oracle's cycles (each frame's writes at
1,500 + 290 i cycles after its start, as the port will place them), sample
equality is out of reach, so `sound_wavcmp.py` measures the same windows
(first 30 s). The floor is what oscillator phase alone does to those
measures: the exact writes, oscillators started at random points, against the
exact render. The burst placement sits at that floor:

| Window | Env r | Cosine | dB | Floor env r | Floor cosine | Floor dB |
|---|---|---|---|---|---|---|
| intro story | 0.970 | 0.964 | 2.06 | 0.969 | 0.964 | 2.02 |
| title | 0.975 | 0.916 | 3.46 | 0.985 | 0.919 | 3.37 |
| select | 0.990 | 0.958 | 2.17 | 0.985 | 0.957 | 2.06 |
| round 1 (Cody) | 0.959 | 0.937 | 2.78 | 0.961 | 0.941 | 2.66 |
| continue | 0.920 | 0.946 | 2.65 | 0.933 | 0.948 | 2.58 |
| round 1 (Guy) | 0.925 | 0.929 | 2.91 | 0.953 | 0.934 | 2.74 |
| round 2 | 0.960 | 0.967 | 1.96 | 0.936 | 0.957 | 2.42 |
| round 3 | 0.959 | 0.948 | 2.48 | 0.950 | 0.942 | 2.69 |
| round 4 | 0.957 | 0.932 | 2.87 | 0.961 | 0.936 | 2.72 |
| round 5 | 0.976 | 0.968 | 1.74 | 0.956 | 0.953 | 2.14 |
| round 2 boss | 0.964 | 0.948 | 2.48 | 0.962 | 0.942 | 2.66 |
| final boss | 0.966 | 0.966 | 1.91 | 0.966 | 0.952 | 2.45 |
| ending | 0.987 | 0.960 | 2.12 | 0.981 | 0.960 | 2.09 |
| credits | 0.977 | 0.944 | 2.58 | 0.981 | 0.952 | 2.35 |

Two things about the oracle a comparison has to know (both affect what is
observed, not the sound):

- The runner clocks its tone generators (timers, sequencers, envelopes,
  sweeps, the linear counter) only while its audio output is on. A run
  without `--wav-out` leaves them at power-on, and so does a state saved by
  such a run: loading it with audio on continues from that stale tone state,
  which is why the replay starts from the oracle's own `HwApu` and not from
  a state of its own making.
- The APU clocks at tick 0 of each CPU cycle, normally after the CPU's
  access; a PPU register access (`$2002` polling, `$2007`) runs the first
  ticks of its cycle during the access, the APU clock included. The capture
  tells the two apart by the APU's put/get flag.

## For the port (P6)

- `sound_model.py` is a template for `sound_driver.c`: one function per
  driver routine, the same RAM, the same order of writes. Keep the scratch
  bytes in the same places if the lockstep should see no sound differences
  at all; `$C3` after `$Fx` needs the caller's Y, or an exception in the
  comparison as `sound_check.py` makes.
- The synthesizer takes the writes themselves, in order, not just the
  register image after each frame (`sound_dump.py --frames` prints both): a
  `$4003`/`$4007`/`$400B`/`$400F` write restarts a waveform, an envelope or
  a length counter. The driver writes each frame's registers in one burst
  ~1,500-6,500 cycles after the NMI.
- The synthesizer is done: `synth_2a03` (above). The game uses pulse (duty,
  constant volume, sweep mute), triangle (linear counter on/off at
  `$4008 = $FF/$00`, reload on `$400B` writes) and noise (both modes), and
  never plays the DMC; the library does all of it. Give it the driver's
  writes stamped where the port puts them (the burst at 1,500 + 290 i cycles
  measures the same as the exact cycles) and run it to each frame's end.
- What is left of P6 is `sound_driver.c`: `sound_model.py` in C, writing
  through `synth_write` instead of registers.
