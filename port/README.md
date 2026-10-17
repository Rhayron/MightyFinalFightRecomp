# port/ — the decompiled port (PLAN_DECOMP.md)

Work in progress. This holds the **P3 scaffold**: Mighty Final Fight on
NESRecomp's legacy, function-level backend (one C function per 6502 routine),
the **lockstep** check that compares it frame by frame with the
cycle-accurate build in the repo root (the oracle), **P4**: every routine of
the game in C in `port/src/game/`, building and running without the
scaffold's generated code, and **P5**: the game on its own runtime, with no
code from nesrecomp and no ROM at run time.

## P5: build and run

`port/CMakeLists.txt` builds **MightyFinalFight** from `port/src/game`,
`port/src/glue/dispatch.c`, `port/src/runtime` and P6's 2A03 synthesizer
(`port/src/audio`). Point it at your dump
once; CMake extracts the game's data into `BUILD/assets` while it
configures, and from then on neither the build nor the game reads the ROM.

```bash
cmake -S port -B build_mff -A x64 -DMFF_ROM="rom/Mighty Final Fight (USA).nes" -DMFF_SDL=ON
cmake --build build_mff --config Release
build_mff/Release/MightyFinalFight.exe --window          # play
build_mff/Release/MightyFinalFight.exe --smoke 600       # headless, 600 frames
MFF_EXE=build_mff/Release/MightyFinalFight.exe port/tests/run_routes.sh check
```

- The dump must be Mighty Final Fight (USA), CRC32 `15EB0BEE` with the
  header. Without CMake: `py -3 -I port/tools/extract_assets.py DUMP --only
  runtime --out DIR` and run with `--assets DIR` (or `MFF_ASSETS=DIR`). The
  executable looks in `assets/` next to itself and one level up, and says
  how to make the data when it finds none.
- `-DMFF_SDL=ON` adds the window (SDL2; `MFF_SDL2_DIR` defaults to the copy
  in `nesrecomp/runner/external/SDL2`). Keys: X = A, Z = B, Enter = Start,
  Right Shift = Select, arrows; a game controller works too (B/Y = A,
  A/X = B). Tab runs fast (and mutes), F11 or Alt+Enter is full screen,
  Esc quits. The sound plays on the default audio device and paces the
  game; with no device the clock does ("The sound" below).
- `--picture console` (the default) shows the picture as the PPU draws it;
  `--picture scaffold` as the scaffold's renderer drew it ("The picture"
  below). `MFF_PICTURE` does the same.
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
| `dispatch_rt.c` | dispatch by address, JMP loops flattened, and a clear stop when a target has no routine (there is no interpreter) |
| `coroutine.c` | the four tasks on C stacks of their own (fibers on Windows, ucontext elsewhere) |
| `mff_hooks.c` | where the NMI may land, and `nmi_tail` (`$C119`): game rules the scaffold kept in `extras.c` |
| `cart.c` | loads `chr.bin` and `prg.bin`; a read of a PRG byte the data leaves out stops the run and names it |
| `testhooks.c`, `png_write.c` | the scaffold's frame log, picture log and keyed input; screenshots |
| `main.c`, `video_sdl.c` | command line (and the reset sequence's 7 cycles); the window and the audio device |

What the game can see is reproduced as the scaffold's runtime had it: the
place the NMI lands (only at the scheduler's `$FEBD` while a task waits),
the IRQ handlers running during the frame's scanline pass at the line the
MMC3 counter fires (241 clocks a frame while the background is on),
`$2002` with its sprite-0 bit, the coroutines' switching, the tail table.
Where the scaffold's runtime is not the console's, the oracle decided:

- **Cycles**: the scaffold's plus what it leaves out and the console
  spends: the reset sequence (7), the NMI's interrupt sequence (7) and
  `nmi_tail`'s own instructions around `sound_update` (52 and 26), so a
  picture's cycle stamp is 85 past the scaffold's; the game cannot see it
  (the NMI still lands only at `$FEBD`), and the sound needs it ("The
  sound").
- **`nmi_tail`'s stack**: it makes the console's pushes, the JSR's too.
  The scaffold (`port/scaffold/extras.c`) calls `sound_update` without
  the JSR's push, and its RTS pops two bytes: after every NMI S is `$01`
  instead of `$FF`, and the raster IRQ's interrupt frame lands on `$0101`,
  `$0100` and `$01FF`. `$0100-$0103` are `bg_pal_anim_on`: the background
  palette animations of palettes 0 and 1 stop (round 3 area 2, palette 1,
  is the one the routes reach). The oracle tours found it ("Tours").
- **Nametable RAM at power-on**: the oracle's pattern of `$F0`/`$0F` runs,
  not zeros. Round 4's elevator shakes the screen a line, and on the
  frames the picture starts a line low the last line shows the other
  nametable row, which the game never writes there.

So against the scaffold's baselines the 3 round-1 routes and `cody_full`
are identical in RAM, OAM, palette and picture every frame (the dead part
of the stack apart), and the 3 tours and 2 alt games differ in exactly
`palette_request`, `palette_buf+7`, `palette_base+7` and palette entry 7,
420 to 1,287 frames each from round 3 area 2 on, where the baselines carry
the scaffold's `nmi_tail` (the oracle's tours agree with the runtime).
The scaffold needs the same `nmi_tail` and new baselines.

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

## P4: every routine in port/src/game

Every routine of the game now has a C body in `port/src/game/`, in files by
system: some written by hand (the scheduler and tasks, NMI and IRQ, VRAM
uploads, the RNG, the pads, banking), the rest produced by the lifter below
and then tidied (names, comments citing ROM addresses, structured control
flow), such as the sound driver in `sound_*.c`. With the CMake option
`MFF_EJECTED` the port builds from `port/src/game`, `port/src/glue` and the
runtime alone, no generated code at all, and plays the nine test routes
identically to the original scaffold, without the runtime's interpreter ever
running a ROM instruction.

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
frame, all of CPU RAM but the dead part of the stack page, OAM, palette and
a hash of the rendered picture.

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
  every hardware access (it then happens at the cycle it did) and at every
  call.
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
