# port/ — the decompiled port (PLAN_DECOMP.md)

Work in progress. This holds the **P3 scaffold**: Mighty Final Fight on
NESRecomp's legacy, function-level backend (one C function per 6502 routine),
the **lockstep** check that compares it frame by frame with the
cycle-accurate build in the repo root (the oracle), **P4**: every routine of
the game in C in `port/src/game/`, building and running without the
scaffold's generated code, **P5**: the game on its own runtime, with no
code from nesrecomp and no ROM at run time, **P8**: Windows, Linux and
macOS, a first run that needs only the player's dump, settings, save states
and a package for players, and **P9** (begun): the picture drawn from the
game's state, with 16:9 and no sprite flicker as options.

## P8: three systems, the first run, settings, save states, the package

**Building.** The same `port/CMakeLists.txt` on Windows (MSVC; SDL2 from
nesrecomp's bundled copy) and on Linux and macOS (gcc or clang; SDL2 from
the system: its CMake package, else pkg-config), `-Wall -Wextra` with no
warnings (`-Wno-type-limits`: the lifted `CMP #$00` keeps its `A >= 0`).
CMake no longer needs Python: with `-DMFF_ROM` the build runs the
executable's own extractor into `BUILD/assets`.

```bash
cmake -S port -B build -DMFF_SDL=ON -DMFF_ROM="rom/Mighty Final Fight (USA).nes"   # -A x64 on Windows
cmake --build build --config Release
build/MightyFinalFight                       # build/Release/MightyFinalFight.exe on Windows: the window
```

Checked on Windows (MSVC 19.44) and Linux (Ubuntu 24.04 in Docker: gcc 13.3
and clang 18, SDL2 2.30 from libsdl2-dev): the 9 routes give byte-identical
frame logs and picture logs on all three compilers, identical to the
baselines (`run_routes.sh check`, which now runs under plain bash too:
`pwd -W` only on Git Bash, `python3` without the `py` launcher). macOS is
written for (`_NSGetExecutablePath`, ucontext behind `_XOPEN_SOURCE`) but
was not built here; the CI workflow below builds it.

**The first run, without Python** (`runtime/extract.c`, `runtime/setup.c`).
The executable carries nothing from the cartridge. When it finds no game
data it takes the player's dump once (CRC32 `15EB0BEE` with the header)
and writes `chr.bin`, `prg.bin` and `manifest.json`, byte for byte what
`extract_assets.py --only runtime` writes (the code lists are compiled in
from `port/tools/prg_code*.txt`, offsets only; `port/tests/extract_check.sh`
compares the two extractors, into an empty directory and over a Python
extraction of other parts). The dump comes from the command line (a path
without an option: dropping the file on the executable), from a `.nes`
with the right CRC next to the executable or in the current directory, or,
in the window build, from a window asking for it (a message, then drop the
file on it: SDL2 has no file dialog). `--extract DUMP [--assets DIR]` only
extracts. Everything goes to the data directory: next to the executable
when it can be written, else `%APPDATA%\MightyFinalFight`,
`~/Library/Application Support/MightyFinalFight` or
`~/.local/share/MightyFinalFight`. The window opens by default when the
build has one; `--headless` or `--smoke N` do not.

**Settings** (`runtime/config.c`): `MightyFinalFight.ini` in the data
directory, written by the window's first run: `[video]` scale, full
screen and P9's picture options (`picture`, `no_flicker`, `wide`, through
`hd_option`), `[audio]` on/off, `[keys]` every NES button and the hotkeys (SDL key
names, several each), `[pad]` the controller buttons, and `[game] slowdown`,
a placeholder handed to the game as `MFF_SLOWDOWN` (P7's option) unless the
command line or the environment sets it. Headless runs read no settings
unless `--config FILE`, so a player's file never reaches a route check.

**Save states** (`runtime/savestate.c`): F5 saves to the slot, F7 loads it,
F6 picks the next of four (`saves/slotN.mffstate`; the window's title says
what happened); `--load STATE` starts from one. A state is every part of
the runtime, field by field (each runtime file with state has a
`*_state(StateIO *)` function: a new static must go there too): CPU, RAM,
cartridge RAM and pads, PPU, MMC3, APU and the synthesizer (doubles as
their bits), the clock, the pictures kept while rendering is off, the JMP
tails and code window, the coroutines' flags, the test hooks and the bot,
and P9's HD picture (its overlay of the game's VRAM patches, the reveal
state of `--wide`, the last objects_draw input and the last HD picture).
About 330 KB.

The tasks' C stacks cannot be written to a file portably, so a state is
taken only at a frame whose stacks are known, and loading builds them
again:

- the frame boundary is at the scheduler's loop top (`$FEBD`) with no
  interrupt running: the scheduler's stack is "about to scan", which
  `func_FEBD` starts again, with the 2 cycles of `$FEBD`'s own charge taken
  back (`timing_resume_at_scheduler`);
- only task 0 has a stack (the game never starts another), waiting in
  `gameplay_frame_end`'s `task_wait_frame` (`$C9B0` on its 6502 stack): the
  end of a gameplay frame, which includes a pause, the continue screen and
  the bosses' talk;
- its C stack is then a chain of JMP tails, all listed in `dispatch_rt.c`
  with target and S: the last gameplay loop (`$C8ED`) at
  `task_wait_frame`'s S + 4, and above it only the frame end's own entries
  (`$C93B`, `$C987`, `$C99D`, `$C9A7`) at that S. Below it are the loops of
  the areas played before: each new area JMPs to gameplay from inside a
  JSR, so the 6502 stack keeps 2 bytes and the C stack a few frames per
  area (48 tails by the end of a tour), and nothing ever returns there.

Loading frees the old stacks (for F7, `longjmp` back to `savestate_run`),
gives task 0 a new stack that runs `gameplay_resume_frame_end`
(`flow_gameplay.c`: what `gameplay_frame_end_work` does after
`task_wait_frame` returns, then gameplay's own loop or its JMP back to it)
inside a rebuilt frame for the last loop's tail, which then does what
`call_by_address_tail`'s loop does (on each lap back, run gameplay again),
and enters the scheduler. The older tails stay listed, without frames: a
return into one, which no route makes, stops the game with a message
instead of going on differently. A save asked for elsewhere (title, map,
cutscenes, round changes, the ending) waits up to 600 frames for such a
frame, then is dropped (the title says so).

`port/tests/savestate_check.sh` plays each route once, saving at 10, 30,
50, 70 and 90% (`MFF_SAVE_AT`, `MFF_SAVE_DIR`), loads every state in a new
process and compares the rest of the frame log and the picture log (cycle
counts included) with the uninterrupted run (`savestate_cmp.py`). On the 9
routes, 41 states (the tours' requests that fall in the ending have none to
take), Windows and Linux: all identical to the end of the route.
`MFF_LOAD_AT=F:STATE` loads in the running process as F7 does; on
`cody_round1` (back from 15,000 to 6,058) and `cody_tour` (from 45,000 to
37,981, 48 tails deep) the rest is identical too.

**Attract mode** (fixed here): left at the title, the game restarts by
JMP reset and froze at frame 5,112: reset's vblank polling waited for frame
boundaries the NMI rule held back (task 0 counts as running until reset
clears RAM; reset's code is never held now), and the abandoned task's JMP
tails stayed listed, so the new `main_task`'s first JMP matched an old one
as a lap and ended the task (the scheduler's restart now clears them).
20,000 idle frames go round the boot screens three times; against the
oracle's frame log the RAM is identical in all 10,984 paired frames, 5,872
after the first restart.

**The package** (`tools/make_port_release.ps1`): builds with the window in
a clean directory configured without a dump, runs the executable from the
package folder (`--help`; a start without data must ask for the dump and
leave nothing), and zips `release/MightyFinalFight-port-windows-x64.zip`:

| File | |
|---|---|
| `MightyFinalFight/MightyFinalFight.exe` | the game, no data |
| `MightyFinalFight/SDL2.dll` | SDL2 2.30 |
| `MightyFinalFight/README.txt` | `port/dist/README.txt`: the dump, the files, controls, settings, saves |
| `MightyFinalFight/SDL2-LICENSE.txt` | SDL2's zlib license |

`port/tools/release_check.py ZIP [DUMP]` lists it, refuses any other name or
data file, and with the dump searches every file for any 32-byte run of
the dump's PRG and CHR (210,262 runs; counting runs, common to any program,
skipped): none. **CI** (`port/ci/github-actions.yml`, files only): Windows,
Linux and macOS, the window and headless builds,
`port/tests/smoke_no_rom.sh` (help, asks for the dump, refuses a random
file, another game and a wrong `.nes` next to it) and a package each. It
sits outside `.github/workflows`, so GitHub does not run it, and `port/`
is not pushed until the owner decides (PLAN_DECOMP.md section 6).

## P5: build and run

`port/CMakeLists.txt` builds **MightyFinalFight** from `port/src/game`,
`port/src/glue/dispatch.c`, `port/src/runtime` and P6's 2A03 synthesizer
(`port/src/audio`). Point it at your dump
once; the build extracts the game's data into `BUILD/assets` (with the
executable's own extractor since P8; CMake configured with Python's
before), and from then on neither the build nor the game reads the ROM.

```bash
cmake -S port -B build_mff -A x64 -DMFF_ROM="rom/Mighty Final Fight (USA).nes" -DMFF_SDL=ON
cmake --build build_mff --config Release
build_mff/Release/MightyFinalFight.exe --window          # play
build_mff/Release/MightyFinalFight.exe --smoke 600       # headless, 600 frames
MFF_EXE=build_mff/Release/MightyFinalFight.exe port/tests/run_routes.sh check
```

- The dump must be Mighty Final Fight (USA), CRC32 `15EB0BEE` with the
  header. Without CMake: `MightyFinalFight --extract DUMP --assets DIR` or
  `py -3 -I port/tools/extract_assets.py DUMP --only runtime --out DIR`, and
  run with `--assets DIR` (or `MFF_ASSETS=DIR`). The executable looks in
  `assets/` next to itself, one level up and in the data directory, and
  takes the dump when it finds none (P8).
- `-DMFF_SDL=ON` adds the window (SDL2; `MFF_SDL2_DIR` defaults to the copy
  in `nesrecomp/runner/external/SDL2`). Keys: X = A, Z = B, Enter = Start,
  Right Shift = Select, arrows; a game controller works too (B/Y = A,
  A/X = B). Tab runs fast (and mutes), F11 or Alt+Enter is full screen,
  Esc quits. The sound plays on the default audio device and paces the
  game; with no device the clock does ("The sound" below).
- `--picture console` (the default) shows the picture as the PPU draws it;
  `--picture scaffold` as the scaffold's renderer drew it ("The picture"
  below); `--picture hd` from the game's state, with `--wide` and
  `--no-flicker` ("P9" below). `MFF_PICTURE` does the same.
- The test hooks are the scaffold's: `MFF_FRAMELOG`, `MFF_PICLOG`,
  `MFF_INPUT`, `MFF_BOT`, plus `MFF_SHOTS=F1,F2,...` (PNG pictures of those
  frames, in `MFF_SHOT_DIR`), `MFF_PICDUMP=FILE` (the shown pictures as
  palette indices), `MFF_PRG_READS=FILE` (every PRG byte read),
  `MFF_WAV=FILE` (the sound, 16-bit mono at 48 kHz, as the oracle's
  `--wav-out`), `MFF_APU_LOG=FILE` (the APU writes, in the lines of the
  oracle's capture) and `MFF_POKE=FILE` (RAM writes keyed like
  `MFF_INPUT`, made where the oracle's save state of that frame is taken:
  "Tours" below).

### The runtime (port/src/runtime)

The routines were written against the scaffold's runtime and include its
interface by name (`nes_runtime.h`, `coroutine.h`); `port/src/runtime`
implements that interface for this one game, so no routine changed:

| File | What |
|---|---|
| `bus.c` | CPU registers and RAM, the address decoder, open bus, the pads |
| `ppu_regs.c` | `$2000-$2007` (t/v/w and the `$2007` read buffer), nametables, palette, OAM |
| `mmc3.c` | bank registers, PRG windows, CHR banks, mirroring, the scanline counter |
| `apu.c` | the game's sound: APU writes to the 2A03 synthesizer (`port/src/audio`) on the console's frame grid; each frame's samples to the window and `MFF_WAV` |
| `timing.c` | the cycle clock, the frame boundary (89,342 dots, one less on odd frames with rendering on), OAM DMA's 513/514 cycles, interrupt entry; each boundary closes the frame's sound |
| `frame.c` | one frame: pads and test hooks, the NMI, the scanline pass with the raster IRQs, the picture |
| `render_frame.c` | the 256x240 picture from the frame's record (`render.h`) |
| `render_hd.c` | P9: the picture from the game's state, `--wide`, `--no-flicker` (`render_hd.h`) |
| `dispatch_rt.c` | dispatch by address, JMP loops flattened, and a clear stop when a target has no routine (there is no interpreter) |
| `coroutine.c` | the four tasks on C stacks of their own (fibers on Windows, ucontext elsewhere) |
| `mff_hooks.c` | where the NMI may land, and `nmi_tail` (`$C119`): game rules the scaffold kept in `extras.c` |
| `cart.c` | loads `chr.bin` and `prg.bin`; a read of a PRG byte the data leaves out stops the run and names it |
| `testhooks.c`, `png_write.c` | the scaffold's frame log, picture log and keyed input; screenshots |
| `main.c`, `video_sdl.c` | command line (and the reset sequence's 7 cycles); the window and the audio device |
| `setup.c`, `extract.c` | (P8) the data directory and the first run: the game's data from the dump, without Python |
| `config.c`, `savestate.c` | (P8) `MightyFinalFight.ini`; save states |

What the game can see is reproduced as the scaffold's runtime had it: the
place the NMI lands (only at the scheduler's `$FEBD` while a task waits),
the IRQ handlers running during the frame's scanline pass at the line the
MMC3 counter fires (241 clocks a frame while the background is on),
`$2002` with its sprite-0 bit, the coroutines' switching, the tail table.
Where the scaffold's runtime is not the console's, the oracle decided:

- **Cycles**: the scaffold's plus what its runner leaves out and the
  console spends: the reset sequence (7) and the NMI's interrupt sequence
  (7 a frame); `nmi_tail`'s own instructions around `sound_update` (52 and
  26) charge in both. The game cannot see it (the NMI still lands only at
  `$FEBD`), and the sound needs it ("The sound"). `selfcheck.py
  --nmi-cycles` (`MFF_NMI_CYCLES`, 7 by default for this executable in
  `run_routes.sh`) takes it into account, so any other cycle difference
  fails the check.
- **`nmi_tail`'s stack**: it makes the console's pushes, the JSR's too.
  Until 2026-10-17 the scaffold (`port/scaffold/extras.c`) called
  `sound_update` without the JSR's push, and its RTS popped two bytes:
  after every NMI S was `$01` instead of `$FF`, and the raster IRQ's
  interrupt frame landed on `$0101`, `$0100` and `$01FF`. `$0100-$0103` are
  `bg_pal_anim_on`: the background palette animations of palettes 0 and 1
  stopped (round 3 area 2, palette 1, is the one the routes reach). The
  oracle tours found it ("Tours"); `extras.c` now has the runtime's
  `nmi_tail` and the baselines were recorded again.
- **Nametable RAM at power-on**: the oracle's pattern of `$F0`/`$0F` runs,
  not zeros. Round 4's elevator shakes the screen a line, and on the
  frames the picture starts a line low the last line shows the other
  nametable row, which the game never writes there.

So against the scaffold's baselines all 9 routes are identical in RAM,
OAM, palette and picture every frame (the dead part of the stack apart),
and in cycles up to the NMI entries above. (Against the baselines recorded
before the `nmi_tail` fix, the 3 tours and 2 alt games differed in exactly
`palette_request`, `palette_buf+7`, `palette_base+7` and palette entry 7,
from round 3 area 2 on; the oracle's tours agree with the fixed version.)

### The picture

`frame.c` fills a record at the frame boundary: the PPU state when the
picture starts, then a region per raster split, opened by the game's IRQ
handler after the scanline the MMC3 fired on (in Mighty Final Fight: the
status rows on top, the playfield with the camera's scroll, in round 1
areas 1 and 2 a still sky first, and the bottom rows with their own CHR
banks). `render_frame.c` draws it in one of two ways (`render.h`):

- **console**: as the PPU draws it. 8 sprites per line (the first 8 in OAM
  order: the game's flicker), the vertical position counting on through
  the splits (a `$2005` Y write waits for the next frame), and a split's
  scroll and banks reaching the line after the one its handler runs on.
  Checked against the cycle-accurate build in the repo root on every
  route of `inputs/` that starts at power-on, from power-on to the
  oracle's first lag frame (where the two games part; a lag the oracle has
  and the runtime does not is P7's), pixel for pixel in palette indices
  (`port/tests/oracle_pictures.py`): all 57,175 pictures are identical.

  | Route | Pictures compared | Identical | The games part at (port frame) |
  |---|---|---|---|
  | `10_cody_poweron` | 7,635 | 7,635 | 7,639 |
  | `10_guy_poweron` | 10,437 | 10,437 | 10,441 |
  | `10_haggar_poweron` | 7,667 | 7,667 | 7,671 |
  | `01_cody_s1` | 8,493 | 8,493 | not before 8,500 frames |
  | `01_guy_s1` | 6,954 | 6,954 | 6,958 |
  | `01_haggar_s1` | 7,003 | 7,003 | 7,007 |
  | `probe` (START through the intro) | 2,993 | 2,993 | not before 3,000 frames |
  | `probe2` (game start, pause) | 5,993 | 5,993 | not before 6,000 frames |

  The `bot1_*` routes start from a saved state, which the port cannot
  load; they are the second half of the `10_*` routes. The tours below go
  further, to the final boss and the ending.
- **scaffold**: as the scaffold's renderer drew it, which is what the test
  baselines hold: every sprite drawn, and a split restarting the vertical
  scroll from the Y the handler writes to `$2005` (so the playfield sits
  one line high and the bottom status rows show the top rows' tiles in the
  other CHR banks). On the same stretch 2,829 of the 7,635 pictures differ
  from the oracle's.

The window and the screenshots show the console's picture unless told
otherwise; the picture log (`MFF_PICLOG`) always hashes the scaffold's, and
the sprite-0 hit the game can see in `$2002` comes from it too, so what is
shown never changes the game.

```bash
oracle=build_headless/Release/MightyFinalFightRecomp.exe
$oracle ROM --frames 7700 --input inputs/10_cody_poweron.txt --frame-log o.flog
py -3 -I port/tests/nmi_input.py inputs/10_cody_poweron.txt o.flog cody.keyed  # or port/tests/routes'
MFF_INPUT=cody.keyed MFF_FRAMELOG=p.flog MFF_PICDUMP=p.pix \
    build_mff/Release/MightyFinalFight.exe --smoke 7800
py -3 -I port/tests/oracle_pictures.py o.flog p.flog p.pix
```

### Tours with the oracle (port/tests/oracle_tour.py)

A power-on route reaches only round 1 before the oracle's first lag frame.
`oracle_tour.py ROM ROUTE OUTDIR` plays the route on the oracle to
`--start`, then walks right and punches (RIGHT, A every other 4 frames,
which also answers the boss dialogs) in segments of `--seg` frames, saving
a state after each and patching its RAM as `tools/bot.py --god` does
(health and lives, the game's skip stage `$0030 = $0E` every
`--skip-every` segments, the final area's fighters' life below 0). The
port gets the same pad (`MFF_INPUT`) and the same writes (`MFF_POKE`, at
the NMI count of the frame each state was saved after), so the two games
stay together until the oracle lags; then the script compares the pictures
and the RAM. With the runtime as it is now (the three power-on routes, from
frame 5,000, 300-frame segments):

| Tour | Skip every | Pictures identical | The games part (the oracle lags) |
|---|---|---|---|
| Guy | 4 segments | 24,171 of 24,171 | NMI 24,173, in the ending, after round 5's final boss |
| Cody | 4 | 16,860 of 16,860 | NMI 16,862, loading round 4's second area |
| Haggar | 4 | 16,860 of 16,860 | NMI 16,862, the same load |
| Haggar | 5 | 15,011 of 15,011 | NMI 15,013, round 3 |
| Guy | 3 | 13,787 of 13,787 | NMI 13,789, round 4 |
| Cody | 3 | 8,653 of 8,653 | NMI 8,655, round 2 |

(Rounds and areas counted from 1, as the game shows them; RAM `$61`/`$60`
count from 0.)

95,342 pictures, all identical; the RAM is the oracle's every frame up to
the lag but the sound's `$C3` (the caller's Y). The tours found the
`nmi_tail` stack bug (Cody's first tour parted at NMI 16,776 in round 3
area 2: `palette_buf+7` `$35` on the oracle, `$15` on the port) and the
power-on nametables (Guy's: 37-45 pixels on line 239 of every other
picture in the elevator).

```bash
py -3 -I port/tests/oracle_tour.py ROM inputs/10_guy_poweron.txt runs/tour_guy \
    --start 5000 --seg 300 --frames 45000 --skip-every 4 --port build_port/Release/MightyFinalFight.exe
```

### The sound

`apu.c` hands every APU write to `synth_2a03` (P6, `docs/sound.md`) and, at
every frame boundary (NMI or not), the frame's 48 kHz samples to the
window's audio queue and to `MFF_WAV`. Three things make it the oracle's
sound:

- **The console's frame grid.** The synthesizer's clock is not the
  runtime's cycle count: frame k starts where the console's k-th vblank is
  (the first at cycle 27,394, scanline 241 dot 1), each one a frame length
  later, and a write is stamped with its place in its frame. The runtime's
  count runs longer, because a boundary the NMI rule holds back waits
  without carrying the wait over (29,822.8 cycles a frame on Cody's route,
  the oracle 29,780.5): stamped with it, the sound fell 170 ms behind the
  oracle's in two minutes.
- **The console's cycles** for the reset, the NMI's entry and `nmi_tail`
  (above): without them each frame's sound writes came 53 cycles early.
- **The oracle's power-on.** Its runner clocks the APU once before the
  CPU's first cycle with the sound output still off: the frame counter
  counts that clock, the tone generators do not. The synthesizer starts at
  cycle 1 the same way (otherwise the noise register runs a cycle ahead
  and its sequence parts at the first period change).

Against the oracle, on the three power-on routes up to the oracle's first
lag frame:

- The driver's RAM (`$C0-$D8`, `$0700-$077F`) is the oracle's every frame
  (Cody's route, 7,636 frames) but `$C3` for NMI 1-436: the caller's Y
  after `$F0`, which no RAM shows (`docs/sound.md`).
- `port/tests/apu_writes.py`: every frame holds the same writes in the same
  order, and they land a few cycles after the oracle's:

| Route | Frames | Writes | Port after the oracle (cycles): median, mean | Within 16 cycles |
|---|---|---|---|---|
| Cody | 7,641 | 44,390 | 6, 6.7 | 97.2% |
| Guy | 10,437 | 67,397 | 7, 7.0 | 96.1% |
| Haggar | 7,673 | 44,696 | 6, 6.7 | 97.2% |

  Frame starts agree to 1-6 cycles (the oracle's frame ends on the
  instruction after the vblank). The rest is the lifted routines' cycle
  model (a JMP turned into a tail call costs 2 more, loads cross pages for
  free): P7's subject.
- The write cycles are the whole difference. With the synthesizer started
  as above, the oracle's own writes (the capture tool of `port/tests/synth`)
  give the oracle's WAV sample for sample, and the same writes moved to the
  port's cycles give the port's WAV sample for sample (Cody's route, to the
  first lag). A few cycles are not audible: each new note restarts a pulse
  waveform, and the pulses keep the oracle's (r 0.99 or more in most
  one-second windows); the triangle never restarts, so a `$4008`/`$400A`
  write that falls on the other side of a quarter-frame clock leaves it at
  another phase from then on, and the noise register at another point of
  its sequence. `port/tools/sound_wavcmp.py` (loudness envelope and spectra,
  both cut to the same samples) puts the passages at the floor P6 measured
  for oscillator phase alone:

| Passage (oracle frames) | Envelope r | Spectral cosine | Mean dB difference | P6's floor (r, cosine, dB) |
|---|---|---|---|---|
| Cody's route: boot, logo and intro text (6-2061) | 1.000 | 1.000 | 0.06 | |
| intro story `$07` (2062-3862) | 0.945 | 0.979 | 1.56 | 0.969, 0.964, 2.02 |
| title `$0C` (4429-4546) | 0.973 | 0.926 | 2.90 | 0.985, 0.919, 3.37 |
| character select `$0D` (4547-4637) | 0.993 | 0.961 | 2.13 | 0.985, 0.957, 2.06 |
| round 1 and its effects (4778-6578) | 0.956 | 0.938 | 2.75 | 0.961, 0.941, 2.66 |
| round 1 (6578-7636) | 0.965 | 0.950 | 2.52 | |
| Guy's route: character select (4547-4637) | 0.987 | 0.964 | 2.03 | |
| round 1 (4761-6561) | 0.943 | 0.931 | 2.83 | 0.953, 0.934, 2.74 |
| round 1 (8550-10430) | 0.945 | 0.934 | 2.86 | |
| Haggar's route: round 1 (4800-6600) | 0.951 | 0.937 | 2.75 | |
| round 1 (6600-7665) | 0.944 | 0.951 | 2.50 | |

```bash
# the oracle's writes: port/tests/synth's capture tool
cmake -S port/tests/synth -B build_synth -A x64 -DMFF_ORACLE_ROOT=.
cmake --build build_synth --config Release
MFF_APU_CAPTURE=o.cap build_synth/Release/mff_apu_capture.exe ROM --frames 7700 \
    --input inputs/10_cody_poweron.txt --hash-out o.hash --wav-out o.wav
MFF_INPUT=port/tests/routes/cody_round1.keyed MFF_APU_LOG=p.cap MFF_WAV=p.wav \
    build_mff/Release/MightyFinalFight.exe --smoke 7700
py -3 -I port/tests/apu_writes.py o.cap p.cap --last 7630
```

### The data (BUILD/assets)

`port/tools/extract_assets.py DUMP --only runtime` (`assets_runtime.py`)
checks the CRC32 and writes `chr.bin` (the CHR ROM), `prg.bin` (the PRG ROM
without its code) and `manifest.json`. The routines read PRG only for their
tables, so `prg.bin` leaves out the 46,368 bytes the P0 map classes as code
(`port/tools/prg_code.txt`, offsets only, made by `prg_code.py` from
`runs/cdl/prgmap.bin`) except the 297 the game also reads as data: the
screens each area's list names past its end, read when the camera stops
(five of them point into the fixed banks' code; worked out from the dump),
and the bytes in `port/tools/prg_code_read.txt`. `MFF_PRG_READS` on data
with every byte, and `port/tools/prg_reads.py`, find such bytes: over the 9
routes and 36 bot sessions of 100,000 frames, 45,973 PRG bytes were read and
all are in `prg.bin`. A byte the data leaves out stops a headless run with
its address (in a window the game reads 0 there and goes on).

## P9: stage 2's picture (port/src/runtime/render_hd.c)

`--picture hd` draws the picture from the game's state instead of the
PPU's nametables and OAM, and adds the options of PLAN_DECOMP.md 2.6:

```bash
build_mff/Release/MightyFinalFight.exe --window --wide --no-flicker   # 16:9, no sprite limit
build_mff/Release/MightyFinalFight.exe --window --picture hd          # options off: stage 1's picture
py -3 -I port/tests/hd_check.py build_mff/Release/MightyFinalFight.exe OUTDIR [ROUTE...] [--wide [--no-flicker]]
```

| Option | Command line | Environment | `hd_option()` key |
|---|---|---|---|
| the HD picture | `--picture hd` | `MFF_PICTURE=hd` | `picture` = `hd` |
| no sprite limit | `--no-flicker` | `MFF_NO_FLICKER=1` | `no_flicker` = 1 |
| 16:9 | `--wide` (426 x 240), `--wide W` (256-512 columns) | `MFF_WIDE=1` or `W` | `wide` = 1 or W |

Either option implies `--picture hd`. `hd_option(key, value)`
(`render_hd.h`) is the hook for the config file P8 adds. The window
(`video_sdl.c`) opens `hd_width()` columns wide and takes the picture's
width with every frame; `MFF_SHOTS` writes PNGs that wide; `MFF_PICDUMP`
keeps its 256 x 240 records (with `--wide`, the original screen's 256
columns). The picture log, the sprite-0 hit and everything the game sees
stay stage 1's: `run_routes.sh check` is identical on the 9 routes with
the options off, with `--wide` and with `--wide --no-flicker` (pictures
included: the log hashes the scaffold renderer's picture whatever is
shown).

### What it draws from

- **The playfield** from the round's stage map (screens, metatiles,
  blocks, palettes: docs/assets.md "Round backgrounds"), decoded once per
  area from `prg.bin` (nothing new is extracted: the game reads those
  bytes itself, so the runtime data already holds them). Which world
  columns are on screen comes from the column writer's state: `N =
  32 * bg_screen + bg_column` (one less while `vram_column_request` still
  holds the column) is the newest world column sent, nametable column c
  holds the world column equal to c mod 32 in N-31..N, rows 4 and 24 are
  tile 0, and an attribute half belongs to the world column that last
  wrote it (the newest column's lag, docs/assets.md step 4). The game's own
  VRAM requests into the playfield (the `$19` write lists and `$64` strips
  at `$0780`, saved before the NMI and applied when it uploads them: the
  door at the start of round 1 area 2 is one) go over the map until the
  writer sends that column again. The camera is `8 * (N - 32)` plus the
  playfield's fine x; a frame is a stage frame only while the scroll and N
  agree. Each line's vertical position (the splits, round 4's shakes) is
  the PPU's, from the frame's record, as `render_frame.c` works it out.
- **The sprites** from the object slots: when objects_draw (`$E9BC`)
  starts, `timing.c` calls `hd_objects_draw_start`, which saves RAM; a
  model of objects_draw, object_draw, object_draw_sprites, object_offscreen
  and object_free (`port/src/game/stage_draw.c`, step by step, on a copy)
  gives every call: slot, place (on the game's screen or off it but near),
  frame, sprites, the OAM entries they got. When the OAM page the model
  builds is the one the frame shows, byte for byte, the sprites come from
  it; otherwise from OAM.
- **From the PPU, as stage 1**: the status rows above and under the
  playfield, and every screen that is not gameplay (title, story,
  character select, round map, continue, the ending and credits).

### The check (M1)

`MFF_HDCHECK=FILE` draws, every frame, the console picture
(`render_frame.c`) and the HD picture with every option off, compares them
pixel for pixel and logs where each came from; `port/tests/hd_check.py`
runs routes that way (in parallel) and lists every picture that differs by
class. On the 9 routes and the 5 power-on routes of `inputs/` not among
them (`01_*_s1`, `probe`, `probe2`, keyed with `nmi_input.py` from the
oracle's frame log):

| | Pictures |
|---|---:|
| compared | 427,520 |
| identical to the console picture | 427,520 |
| playfield from the stage map | 308,156 |
| playfield from the PPU: not gameplay or between areas (CHR banks not the area's) | 118,653 |
| playfield from the PPU: an area's first columns being drawn (black) | 320 |
| playfield from the PPU: an area's exit (the scroll goes to 0 before the next area) | 391 |
| sprites from the objects (the model's OAM is the frame's) | 417,089 |
| sprites from OAM (screens that write OAM themselves, frames before the first objects_draw) | 10,431 |

So there is no difference class to explain: where the state describes the
picture, the picture drawn from it is stage 1's; where it does not (the
classes above), stage 1's is used. A check that it is the map that draws:
one map column changed made 3,027 of 7,970 pictures differ.

### --no-flicker (M2)

Each object once per frame (its last object_draw call: the effect slots
14-15 that objects_draw visits twice show their final frame), every sprite
of its frame, in a fixed order: the effects (slots 15, 14) in front, then
the player and slots 1-13; no 8 per line, no 63-entry OAM. On the routes
the 8 per line leave sprites out in 103,255 of the 427,520 pictures;
objects_draw never filled OAM. Screens whose OAM is not the model's draw
all 64 entries without the limit.

### --wide (M3)

The camera is the game's, held inside the area: the wide picture shows
`cam - 85 .. cam + 341` of the stage map, moved right at an area's first
screen and left at its last so the stage fills it; the one-screen rooms
(round 3 area 3 and round 4 area 2, RAM rounds 2/3) sit in the middle with
the backdrop around them (15,566 of the 308,156 stage pictures). Round 2
areas 2-3's still sky stays still. The status rows and the screens are the
original 256 columns in the middle with the backdrop around them. An
area's exit keeps the last stage frame's camera through the fade.

The game is unchanged, so its spawn points stay at the original screen's
edges: the stage list puts enemies at x 255, the timed spawner at -16, -4
and 276 (screen x). Drawn in a wide picture as they are they would pop
into view in the margins. So an object is drawn outside the original 256
columns only once it has been wholly inside them (or if it was first seen
beyond the wide picture's edge), and until then only its part inside
them: a new enemy comes in through the original screen's edge as on the
console, and one that leaves it stays visible in the margin. Objects the
game treats as far ($68 px or more off its screen, pulled back or freed by
object_offscreen) are not drawn, nor hidden or blinking ones off it.
`hd_check.py --wide` counts objects that appear in the margins without
crossing the picture's edge: 0 on all 14 routes, with and without
`--no-flicker`; with every object drawn as it is (the rule off), 10 on
`guy_round1` and 23 on `cody_tour`, the spawns above.

### What P9 still has to do

- The status bar and the screens (title, story, select, round map,
  continue, ending, credits) from the state: they are the nametables'
  (centred in `--wide`). The message box under the playfield too.
- The one-screen rooms have nothing to show beside them in `--wide`.
- Lag frames (P7): if the NMI lands inside objects_draw, the frame's OAM is
  not the model's and that picture's sprites come from OAM (the
  wide picture then lacks the off-screen ones for a frame).
- The scripted exits' patches (docs/assets.md, patches 1-3) go through the
  same VRAM requests as the door, but no route reaches them.
- The state free of the 2 KB overlay, texture packs, filters and integer
  scaling (PLAN_DECOMP.md 2.6, P9).

## P4: every routine in port/src/game

Every routine of the game has a C body in `port/src/game/`, in files by
system (the map below): some written by hand (the scheduler and tasks, NMI
and IRQ, VRAM uploads, the RNG, the pads, banking), the rest produced by the
lifter below and then tidied: names from `disasm/labels.toml`, comments that
say what the code does and cite ROM addresses as evidence, named tables,
structured control flow. No lifted file is left. With the CMake option
`MFF_EJECTED` the port builds from `port/src/game`, `port/src/glue` and the
runtime alone, no generated code at all, and plays the nine test routes
identically to the original scaffold, cycle counts included, without the
runtime's interpreter ever running a ROM instruction.

```bash
port/tests/run_routes.sh baseline   # once, on the pure scaffold (regen.sh --pure)
port/tests/regen.sh                 # the scaffold with port/src/game (replace_func)
port/tests/run_routes.sh check      # every route must print "identical"
port/tests/port_check.sh build_port # the port alone: dispatch.c and state.h, the
                                    # entry check, -DMFF_EJECTED=ON build, all routes
```

The check fails a route where the runtime's interpreter ran anything
(`port/tests/interp_report.py` reads `NESRECOMP_FALLBACK_LOG`): a jump to an
address with no routine still works on the runtime, from the ROM, and the
routes would stay identical. `port/tools/find_entries.py ROM` finds those
places statically (jump tables longer than the runs used, tables inline
after a JSR, return addresses pushed before a jump, constant targets in the
port's code), and `port_check.sh` stops if it reports any.

The routes (`port/tests/routes`): round 1 with each character (inputs keyed
by NMI count), three 50,000-frame tours by the in-process bot
(`port/scaffold/bot.c`, `MFF_BOT=char:seed:skip`) that go through every
round, beat the final boss and run into the ending, and three 60,000-frame
games with other bot seeds. The check compares, every
frame, all of CPU RAM but the dead part of the stack page, OAM, palette, a
hash of the rendered picture and the CPU cycle count when it was drawn. The
NMI lands only at the top of the scheduler's idle scan, so a routine that
costs a cycle more or less changes no RAM byte, only that count:
`selfcheck.py` fails as soon as the count moves off the offset it has at
picture 0 (0 for the scaffold; the port on its own runtime charges the 6502's
reset and NMI sequences, 7 cycles each, which `--nmi-cycles 7` models).

### Map of port/src/game

| System | Files |
|---|---|
| Machine services | `boot.c` (reset), `tasks.c` (the four cooperative tasks and the scheduler), `interrupts.c` (NMI, IRQ, VRAM uploads), `bank.c`, `ppu.c`, `input.c`, `rng.c`, `objects.c` (clearing the slots) |
| Game flow and screens | `flow_main_task.c` (power-on to the first round), `flow_gameplay.c` (the gameplay frame), `flow_screen.c` (fades, clears), `flow_screens.c` (bank `$04`'s screens), `flow_select.c`, `flow_round_map.c`, `flow_continue.c`, `flow_area.c` (an area's graphics), `flow_story_text.c`, `flow_talk.c` (the message box: boss talks, round messages); `flow.h` |
| Stage | `stage_area.c`, `stage_camera.c`, `stage_columns.c` (background streaming), `stage_draw.c` (objects into OAM), `stage_hud.c`, `stage_probe.c` (background probes), all fixed bank; `stage_spawn.c`, `stage_events.c` (bank `$08`); `stage_palette.c`, `stage_ending.c` (bank `$09`); `stage.h` |
| Player | `player_states.c` (`player_state` and its dispatch), `player_state_idle.c`, `player_state_walk.c`, `player_jump.c`, `player_attack.c`, `player_grab.c`, `player_hurt.c`, `player_round.c` (fixed bank); `player_actions.c`, `player_specials.c`, `player_throws.c`, `player_power.c`, `player_exits.c` (bank `$00`'s actions); `player_defs.h` |
| Objects | `obj_update.c` (`objects_update` and its collision checks), `obj_hits.c`, `obj_knockdown.c`, `obj_thrown.c`, `obj_items.c`, `obj_common.c` (bank `$0C`); `obj_move.c`, `obj_walk.c`, `obj_anim.c`, `obj_reach.c`, `obj_combat.c` (fixed-bank helpers); `obj_props.c`, `obj_weapons.c` (bank `$05`, ids `$60`-`$6F`); `obj_defs.h`, `obj_fixed.h`, `obj_props.h` |
| Common enemies | `enemy_common.c`, `enemy_entrances.c` (bank `$0C`, ids `$20`-`$5F`); `enemy_ai.c`, `enemy_walk.c`, `enemy_moves.c` (the AI, bank `$0D`); `enemy_defs.h`, `enemy_ai.h` |
| Bosses and set pieces | `boss_thrasher_sodom.c`, `boss_sodom_rematch.c`, `boss_b01_shared.c`, `boss_b01_scene.c` (bank `$01`, ids `$80`-`$8F`); `boss_abigail.c`, `boss_belger.c`, `boss_belger_end.c`, `enemy_drums.c`, `enemy_tables.c` (banks `$03`/`$0B`, ids `$90`-`$9F`, `$70`-`$7F`); `boss_b01.h`, `enemy.h` |
| Sound | `sound.c` (the game's side), `sound_driver.c`, `sound_music.c`, `sound_effect.c`, `sound_voice.c`, `sound_output.c` (the driver, banks `$06`/`$07`); `sound_driver.h` |
| Shared | `mff.h` (the exactness rules, charges, calls and exits), `hw.h` (the hardware registers by name), `state.h` (RAM names, generated from `labels.toml`) |

`docs/engine.md` and `docs/objects.md` describe the systems with the same
names; `docs/objects.md`, "Ids", maps every object id to its routine and file.
What P4 leaves (PLAN_DECOMP.md): the lifter's register model (A/X/Y and the
flags in locals that meet `g_cpu` at every call), 55 `goto`s that jump to a
tail several 6502 branches share, INC/DEC of memory through the temporary
`t`, and names still marked "(proposed)".

### The lifter

`port/tools/lift.py` turns a routine into C that is exact by construction:
the semantics come from the ROM, and every decision the generator made comes
from the pure scaffold's own copy of the code (which jumps are tail calls
and cost 2 more cycles, page-crossing branches, yields, how a JSR
dispatches). A/X/Y and the flags are locals synced with `g_cpu` at calls and
exits, a flag no path reads is not computed, RAM is named from
`disasm/labels.toml` (`port/src/game/state.h`, by `port/tools/state_h.py`),
and `lift_structure.py` turns the simple goto shapes into if/else and loops.
`port/tools/lift_bank.py ROM BANK...` lifts every routine of some banks that
has no hand-written replacement; `--extra` adds entries the P0 map does not
list but runs reach (find them with a stack-tracking build and
`NESRECOMP_FUNC_COUNTS=FILE`), and `--into FILE` lifts just the listed
entries the port lacks into one file. Every lifted file has since been
emptied into the system files; the lifter stays as the way that code was
produced and the way to bring in a routine the port lacks.

Two passes clean the output (`lift_opt.py`):

- **Partial syncs.** `port/tools/exit_use.py ROM` works out, for every routine,
  which registers and flags it reads at entry and which ones its callers read
  after it returns (`runs/p4/exit_use.json`). With that file, a sync carries
  only those, as plain stores and loads, so the flag computations nobody reads
  drop out.
- **Branch folding.** A branch on a flag known to be constant (the 6502's
  `LDA #0 / BEQ`) becomes a goto, and register round trips through `g_cpu`
  between two calls go away.

### Rules a replacement keeps (port/src/game/mff.h)

- Exact means RAM (temporaries such as `$00` too), registers and flags on
  return, and cycles: the scaffold ends a frame on a cycle count. The model:
  base cycles per opcode, +1 for a taken branch (+2 across a page), no
  page-cross penalty on loads, +2 when a JMP becomes a tail call.
- A routine charges a run of instructions, then does its work. Runs end at
  every hardware access (it then happens at the cycle it did: the OAM DMA's
  stall, 513 or 514 cycles, follows the parity of the cycle of the
  `STA $4014`) and at every call. A charge of 0 cycles is not free: the
  runtime counts at least one.
- The scaffold takes the NMI only between tasks, and around the scheduler
  only at the top of its scan (`$FEBD`): the sound driver reads a register
  nmi_tail never sets, so the place the NMI interrupted leaked into its
  scratch. The scheduler is written instruction by instruction for that.

## Scaffold

```powershell
# entry points and data regions from the P0 map (runs/cdl/*, see CLAUDE.md)
py -3 -I port/tools/legacy_config.py "rom\Mighty Final Fight (USA).nes" --map runs/cdl/prgmap.bin `
    --tables runs/cdl/tables.json --extra runs/cdl/extra.json `
    --base port/scaffold/game.base.toml --out port/scaffold/game.toml
cd port/scaffold
..\..\build_headless\nesrecomp-host\Release\NESRecomp.exe "..\..\rom\Mighty Final Fight (USA).nes" --legacy --game game.toml
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

`generated/` is made from your dump and never committed. What the scaffold
needed (details in `game.base.toml` and `extras.c`):

| Problem | Fix |
|---|---|
| The finder decodes data as code (9,728 "functions") | the map's entry points and data regions; `disable_ptr_scan`, `disable_secondary` |
| Each task resume leaked two stack bytes | `push_all_jsr`: the scheduler resumes a task by popping the yield's return address |
| The NMI's hijacked return (`nmi_tail`, `$C119`) RTSed to `$0000` | `replace_func`: `func_C119` runs `sound_update` and keeps A/X/Y/P |
| An NMI fired mid-task (the runner's estimated-time lag) corrupted the coroutine stacks | `g_nmi_veto` (nesrecomp branch `mff-scaffold`): no NMI while a task runs; `MFF_LAG=1` brings the lag back |

Run headless: `MffScaffold.exe ROM --smoke FRAMES --smoke-interval N --smoke-output DIR`
(`SDL_VIDEODRIVER=dummy`), or `--script FILE` for the legacy runner's
WAIT/HOLD/SCREENSHOT scripts. `MFF_TRACE=N` prints the task table for N frames.

## Lockstep

```bash
oracle=build_headless/Release/MightyFinalFightRecomp.exe
$oracle ROM --frames 7000 --input inputs/10_cody_poweron.txt --frame-log o.flog
py -3 -I port/tests/nmi_input.py inputs/10_cody_poweron.txt o.flog cody.keyed
MFF_INPUT=cody.keyed MFF_FRAMELOG=s.flog port/scaffold/build/Release/MffScaffold.exe ROM --smoke 7200 ...
py -3 -I port/tests/lockstep.py o.flog s.flog
```

- Frames pair by the game's own NMI counter (`frame_counter`, `$92`, unwrapped
  from reset) and the n-th frame showing it, so the two runners line up
  whatever each calls a frame.
- Input is keyed the same way (`nmi_input.py` -> `MFF_INPUT`): a fixed frame
  offset drifts, because the runners count NMI-off frames differently. Each key
  carries the next oracle frame's buttons (the oracle's frame RAM already holds
  that frame's pad read; the scaffold reads on its next pass).
- Ignored by default, each for a stated reason: the stack page, the tasks'
  saved stack pointers (`$82/$86/$8A/$8E`) and `nmi_return_pc` (`$A0/$A1`, the
  PC the NMI interrupted, which the scaffold never has).
- `--frame-log` costs ~135 KB per frame on the oracle side; delete it after
  the check.
- `MFF_RESYNC=o.flog` copies the oracle's RAM for the same key between
  frames (never the stack page, the task table `$80-$91` or `nmi_return_pc`).
  It realigns data, not control flow.

### What the lockstep shows (power-on through round 1, ~19,900 frames)

Identical until the first frame the oracle lags (its NMI lands while task 0 is
still running): Cody to frame 7,637, Guy to 10,433, Haggar to 7,669 with the
veto on. The scaffold never lags, and after a lag only the oracle had, a task
is at another point of its own code (a wait counter on its stack), which no
RAM copy fixes; lags also move the RNG (`rng_step` mixes in `frame_counter`).
So:

- **against the oracle**: boot, intro, title, select and the gameplay up to
  the first lag are certified bit for bit;
- **for P4**: the refactored scaffold is checked against the unrefactored one
  (same timing model, deterministic), which is exact over the whole game;
- **faithful lag** (the NMI at the oracle's interrupted PC, `nmi_return_pc`)
  is P7's timing model.
