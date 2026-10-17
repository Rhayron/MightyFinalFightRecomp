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
`port/src/glue/dispatch.c` and `port/src/runtime`. Point it at your dump
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
  A/X = B). Tab runs fast, F11 or Alt+Enter is full screen, Esc quits. No
  sound yet (P6).
- `--sprite-limit` (or `MFF_SPRITE_LIMIT=1`) shows at most 8 sprites per
  line, as the console does; without it every sprite is drawn, as the
  scaffold's renderer does and the test baselines expect.
- The test hooks are the scaffold's: `MFF_FRAMELOG`, `MFF_PICLOG`,
  `MFF_INPUT`, `MFF_BOT`, plus `MFF_SHOTS=F1,F2,...` (PNG pictures of those
  frames, in `MFF_SHOT_DIR`) and `MFF_PRG_READS=FILE` (every PRG byte read).

### The runtime (port/src/runtime)

The routines were written against the scaffold's runtime and include its
interface by name (`nes_runtime.h`, `coroutine.h`); `port/src/runtime`
implements that interface for this one game, so no routine changed:

| File | What |
|---|---|
| `bus.c` | CPU registers and RAM, the address decoder, open bus, the pads |
| `ppu_regs.c` | `$2000-$2007` (t/v/w and the `$2007` read buffer), nametables, palette, OAM |
| `mmc3.c` | bank registers, PRG windows, CHR banks, mirroring, the scanline counter |
| `apu.c` | the APU registers, kept for P6; nothing the APU does reaches the game |
| `timing.c` | the cycle clock, the frame boundary (89,342 dots, one less on odd frames with rendering on), OAM DMA's 513/514 cycles, interrupt entry |
| `frame.c` | one frame: pads and test hooks, the NMI, the scanline pass with the raster IRQs, the picture |
| `render_frame.c` | the 256x240 picture from the frame's record (`render.h`) |
| `dispatch_rt.c` | dispatch by address, JMP loops flattened, and a clear stop when a target has no routine (there is no interpreter) |
| `coroutine.c` | the four tasks on C stacks of their own (fibers on Windows, ucontext elsewhere) |
| `mff_hooks.c` | where the NMI may land, and `nmi_tail` (`$C119`): game rules the scaffold kept in `extras.c` |
| `cart.c` | loads `chr.bin` and `prg.bin`; a read of a PRG byte the data leaves out stops the run and names it |
| `testhooks.c`, `png_write.c` | the scaffold's frame log, picture log and keyed input; screenshots |
| `main.c`, `video_sdl.c` | command line; the window |

What the game can see is reproduced exactly as the scaffold's runtime had
it: the cycle count and the place the NMI lands (only at the scheduler's
`$FEBD` while a task waits), the IRQ handlers running during the frame's
scanline pass at the line the MMC3 counter fires (241 clocks a frame while
the background is on), `$2002` with its sprite-0 bit, the coroutines'
switching, the tail table. All 9 routes are identical to the baselines in
RAM, OAM, palette and picture every frame, and against the scaffold built
from the same routines the cycle stamps of every picture match too.

### The picture

`frame.c` fills a record at the frame boundary: the PPU state when the
picture starts, then a region per raster split, opened by the game's IRQ
handler after the scanline the MMC3 fired on (in Mighty Final Fight: the
status rows on top, the playfield with the camera's scroll, in round 1
areas 1 and 2 a still sky first, and the bottom rows with their own CHR
banks). `render_frame.c` draws the background line by line from the
regions and the sprites over it with the PPU's priority rules, with the
scaffold renderer's scroll model, which is what the baselines hold. Two
places where that model is not the console's: every sprite is drawn
(`--sprite-limit` gives 8 per line), and a split restarts the vertical
scroll from the Y the handler writes to `$2005` (the console keeps counting
lines), which is why the bottom status rows show the top rows' tiles in the
other CHR banks.

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

Every routine of the game now has a C body in `port/src/game/`: about thirty
written by hand (the scheduler and tasks, NMI and IRQ, VRAM uploads, the
RNG, the pads, banking) and the rest lifted. With the CMake option
`MFF_EJECTED` the port builds from `port/src/game`, `port/src/glue` and the
runtime alone, no generated code at all, and plays the six test routes
identically to the original scaffold.

```bash
port/tests/run_routes.sh baseline   # once, on the pure scaffold (regen.sh --pure)
port/tests/regen.sh                 # the scaffold with port/src/game (replace_func)
port/tests/run_routes.sh check      # every route must print "identical"
py -3 -I port/tools/dispatch_gen.py # then build the port alone:
cmake -S port/scaffold -B build_port -A x64 -DMFF_EJECTED=ON && cmake --build build_port --config Release
MFF_EXE=build_port/Release/MffScaffold.exe port/tests/run_routes.sh check
```

The routes (`port/tests/routes`): round 1 with each character (inputs keyed
by NMI count) and three 50,000-frame tours by the in-process bot
(`port/scaffold/bot.c`, `MFF_BOT=char:seed:skip`) that go through every
round, beat the final boss and run into the ending. The check compares, every
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
`NESRECOMP_FUNC_COUNTS=FILE`).

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
