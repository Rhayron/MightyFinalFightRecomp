# port/ — the decompiled port (PLAN_DECOMP.md)

Work in progress. This holds the **P3 scaffold**: Mighty Final Fight on
NESRecomp's legacy, function-level backend (one C function per 6502 routine),
the **lockstep** check that compares it frame by frame with the
cycle-accurate build in the repo root (the oracle), and **P4**: every
routine of the game in C in `port/src/game/`, building and running without
the scaffold's generated code.

## P4: every routine in port/src/game

Every routine of the game now has a C body in `port/src/game/`: some written
by hand (the scheduler and tasks, NMI and IRQ, VRAM uploads, the RNG, the
pads, banking, and the systems tidied so far, such as the sound driver in
`sound_*.c`) and the rest lifted (`lifted_*.c`). With the CMake option
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
entries the port lacks into one file (`lifted_late.c`).

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
