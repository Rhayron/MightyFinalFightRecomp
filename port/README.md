# port/ — the decompiled port (PLAN_DECOMP.md)

Work in progress. This holds the **P3 scaffold**: Mighty Final Fight on
NESRecomp's legacy, function-level backend (one C function per 6502 routine),
the **lockstep** check that compares it frame by frame with the
cycle-accurate build in the repo root (the oracle), and the start of **P4**:
routines rewritten by hand in `port/src/game/`, each replacing its generated
version in the scaffold.

## P4: hand-written routines

The generated code is not ejected wholesale (it is tens of MB with the same
code repeated at every call site); instead one routine at a time gets a
readable C body in `port/src/game/*.c` and a `[[replace_func]]` line in
`scaffold/game.base.toml`. In this phase a replacement must leave the
machine exactly as the original does, so the scaffold with replacements is
compared with the scaffold without them, frame by frame:

```bash
port/tests/run_routes.sh baseline   # once, on the unrefactored scaffold
port/tests/regen.sh                 # after editing game.base.toml or src/game
port/tests/run_routes.sh check      # every route in tests/routes must print "identical"
```

- Exact means all 2 KiB of RAM (temporaries such as `$00` too), OAM and
  palette every frame, and the same registers and flags on return.
- Cycles count too: the scaffold ends a frame on a cycle count, and a
  replacement charging 10 cycles too few fails the check within the first
  gameplay frames. The model (`mff.h`): base cycles per opcode, +1 per taken
  branch, no page-cross penalty. A routine charges a segment, then does its
  work; segments split where an interrupt reads what the routine writes
  (`bank_switch_busy`, the sound queue).
- Under `push_all_jsr` a routine ends with `mff_rts()` (pops the return
  address the caller pushed).

Done so far: `rng_step`, `read_joypads`, `bank_switch`, `rendering_off/on`,
`object_clear`, `objects_clear_all`, `clear_ram_400_5ff`, `sound_queue_push`.

The generator changes this needed are on nesrecomp's `mff-scaffold` branch:
a `replace_func` entry is never folded into another function's body (callers
would bypass it), a replaced function is never an owner of other entries,
and a body that falls into a replaced entry tail-calls the replacement.
The finder's fixed-bank pointer scan still invents entries (`$C842`, inside
the `start_health` table, whose decode runs into `clear_ram_400_5ff`); those
bodies are never called. Turning the heuristics off
(`NESRECOMP_LEGACY_FUNCTION_FINDER=1`) halves the generated code but is not
the certified scaffold: it moves the frame boundary inside the scheduler's
idle loop from frame 2 on and hits a coroutine stack error at frame 7,621 of
the Guy route, so it stays off.

A hand count of cycles is easy to get wrong (`clear_ram_400_5ff` was 510
short at first, caught at frame 19); the check is what makes a routine done.

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
