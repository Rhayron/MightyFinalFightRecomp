# MightyFinalFightRecomp

Mighty Final Fight (USA, mapper 4/MMC3) recompiled with NESRecomp's
**cycle-accurate backend** (`nesrecomp_add_cycle_game`). Plan: `PLAN_WINDOWS.md`
(Windows) and `PLAN.md` (macOS). Engine rules: `nesrecomp/AGENTS.md`,
`nesrecomp/CLAUDE.md`, `nesrecomp/runner/cyc/README.md`.

ROM: `rom/Mighty Final Fight (USA).nes`, CRC32 `15EB0BEE` (with header). Never
commit it, saves, states or anything built from it.

## Build

```powershell
.\setup.bat                      # submodules + nestopia-core junction
.\build_all.bat -Headless        # build_headless\Release\MightyFinalFightRecomp.exe (no SDL/launcher)
.\build_all.bat                  # build_release\Release\... with window + recomp-ui launcher
.\tools\make_release.ps1         # release\MightyFinalFightRecomp-windows-x64.zip, no ROM
```

`tools/build.ps1` pins CMake's Python to the `py` launcher's newest 3.11+.
Seeds (`cyc_seeds.txt`) are a configure input: rebuilding after they change
regenerates the banks.

## Coverage loop (Phase 2)

```
inputs/*.txt or a bot session -> exe --input X --miss-log runs/<name>.miss
  -> py -3 -I tools/merge_seeds.py cyc_seeds.txt runs/*.miss
  -> build_all.bat -Headless -> run again -> until 0 interpreted ROM cycles
```

- Never point parallel runs at the same `--miss-log` file (the runner
  read-merge-writes it); give each run its own and merge.
- `tools/bot.py` plays from a state in 16-frame segments reading RAM
  (`docs/ram_map.md`); its `route.txt` replays the session byte-identically
  in one run. A state's header frame is the **next** frame to run.
- `tools/state_ram.py` patches RAM in a state (coverage aid only).
- `tools/contact_sheet.py` (run with `uv run --no-project --with pillow`)
  tiles `--shot-every` screenshots into a timeline.
- Key states (not committed, regenerate from the routes): `states/title_4500`
  (title screen, no input from power-on) and `states/r1_<char>_start`
  (round 1 gameplay, frame 4860).

## Status (2026-10-15)

- Coverage converged: `cyc_seeds.txt` (15,548 lines) compiles every ROM
  instruction start seen across boot, attract, all three characters, rounds
  1-5, both phases of the final boss, game over, continue and the credits.
  A sweep from every round start with a fresh bot seed added 0 lines. Left on
  the interpreter by design: `4k:19:9FFF` and `4k:1D:DFFF` (instructions that
  cross a bank boundary).
- Oracles: NESRecomp == TriCNES (`build_cyc/Release/cyc_oracle.exe`) on the
  trace, memory+picture, registers and cycle count of all 5,600 frames of
  `inputs/10_cody_poweron.txt` (`--ram-init zeros`; the `hw=` hash differs
  from frame 0 by layout and is not comparable). Mesen (`build_mesen_shot/`,
  MesenCE `4b8a196`) matches pixel for pixel through frame 5231 and diverges
  from frame 5232 on (an enemy's sprite row, then game state): a Mesen vs
  TriCNES disagreement, not a recompilation fault. Root cause not chased.
- Bot coverage (`--god`) patches health, lives, enemy life (`$04A0+slot`) and
  `$0030 = $0E` (skip stage) into states; those sessions are not replayable
  as inputs and record their misses per segment. Pits (round 3) kill outright.
- `package_cycle_windows.ps1` (nesrecomp `2b4fcdb`) reads an empty
  `NESRECOMP_RECOMP_UI` from a CRLF CMakeCache as `""`; `tools/build.ps1`
  always passes `-RecompUi` to avoid it. Worth an upstream fix.

## Rules

- Run native tools hidden (no console windows, no focus stealing); open the
  game window only when the owner asks.
- Never edit generated code (`build*/cycle-*`). Codegen bugs go to
  `nesrecomp/recompiler/src/cyc_codegen.c`, hardware bugs to
  `nesrecomp/runner/cyc/hw_*.c`, on a branch of the submodule.
- Cite evidence (ROM bytes, Mesen trace, headless RAM dump) before changing
  behavior; small diffs, one system at a time; validate with a headless route
  and screenshot before calling anything done.
