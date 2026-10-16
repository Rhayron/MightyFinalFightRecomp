<div align="center">

<img src="docs/img/hero.png" alt="Mighty Final Fight: NES to C to native x64" width="100%">

# Mighty Final Fight Recomp

**Capcom's 1993 NES brawler, translated instruction by instruction into C and built as a native Windows executable. No CPU emulation.**

![platform](https://img.shields.io/badge/platform-Windows%20x64-0078d4?style=for-the-badge&logo=windows)
![backend](https://img.shields.io/badge/backend-cycle--accurate-8a2be2?style=for-the-badge)
![native](https://img.shields.io/badge/native%20code-100%25-2ea44f?style=for-the-badge)
![oracle](https://img.shields.io/badge/TriCNES-bit%20exact-ffb000?style=for-the-badge)
![rom](https://img.shields.io/badge/ROM-not%20included-d73a49?style=for-the-badge)

</div>

---

<div align="center">

| Cody | Guy | Haggar |
|:---:|:---:|:---:|
| <img src="docs/img/gameplay_cody.gif" width="260"> | <img src="docs/img/gameplay_guy.gif" width="260"> | <img src="docs/img/gameplay_haggar.gif" width="260"> |
| *"A tough street fighter who is skilled with knives."* | *"Descended from ninja, he possesses incredible agility."* | *"The hot-tempered wrestler and mayor of Metro City."* |

<sub>Every frame above comes from the recompiled executable running headless. Nobody is holding the controller; the coverage bot is.</sub>

</div>

## What this is

[NESRecomp](https://github.com/mstan/nesrecomp) is a static recompiler. It reads the cartridge's 6502 code and writes C: one function for every 1 KB of every PRG bank. MSVC then builds that C together with a cycle-accurate model of the console (PPU, APU, MMC3, DMA, all stepped cycle by cycle). What comes out is an `.exe` where the game itself runs as x64 machine code instead of being interpreted.

The catch is the MMC3 mapper. The game swaps 4 KB windows of code in and out constantly, so static analysis only sees what the fixed banks lead to. Everything else (every move, every enemy, every boss, the ending) only shows up when the game actually runs. Most of this repo is about making all of it run at least once.

```text
boot → intro → title → select → Round 1 … Round 5 → Belger → ending → credits
 100%   100%    100%     100%        100% native           100%     100%     100%
```

## How it works

```mermaid
flowchart LR
    ROM[("your dump<br/>Mighty Final Fight (USA).nes<br/>CRC32 15EB0BEE")]
    SEEDS["cyc_seeds.txt<br/>15,548 entry points"]
    GEN["NESRecomp<br/>cyc_codegen"]
    C["C per bank<br/>mighty-final-fight_cyc_bNN.c"]
    MSVC["MSVC x64"]
    EXE["MightyFinalFightRecomp.exe"]
    RUN["play it<br/>--miss-log"]
    ROM --> GEN
    SEEDS --> GEN
    GEN --> C --> MSVC --> EXE --> RUN
    RUN -- "instructions that fell<br/>back to the interpreter" --> SEEDS
```

Whenever an instruction runs on the interpreter, the executable writes down its bank and address (`--miss-log`). That list becomes the seed file, the seeds become compiled code on the next build, and the loop repeats until the list comes back empty.

## The bot that played it to the end (with all three)

Hand-writing a frame-by-frame input script for a beat 'em up sounded miserable, so the coverage loop got a player instead: [`tools/bot.py`](tools/bot.py).

<div align="center">
<img src="docs/img/bot_vision.png" width="70%" alt="what the bot sees: object slots in RAM">
</div>

- Every 16 frames it loads a save state, reads the object table out of RAM (`$0300` state, `$0320` id, `$0360` x, `$03C0` y, `$04A0` life), picks the closest enemy, lines up on the Y axis and starts swinging: punch strings, jump kicks, grabs, the special.
- The game is deterministic, and a loaded state carries on exactly like an uninterrupted run. So the segments glue together into a plain `--input` file that replays the whole session byte for byte (see [`inputs/`](inputs)).
- It notices when it's stuck, wanders off in a random direction and ignores that target for a while.
- `--god` is for coverage only. It tops up health and lives, cuts the life of any boss still standing after 3,000 frames, and after 6,000 frames without progress writes `$0030 = $0E`, the game's own "skip stage" action (listed in the RAM map, confirmed here). The code that runs is always the game's.

<div align="center">

| What tripped the bot up | Fix |
|---|---|
| Chasing an "enemy" at the top of the screen | id `$94` at y = 0 is a spawn marker, not a fighter, so targets must stand on the ground band |
| Thrasher and Sodom never dying | enemy life lives at `$04A0+slot`, not `$049B` as Data Crystal says |
| Round 3's pits kill on contact | refill lives; the real game over had already been covered |
| The final boss dialog never advancing | dialog boxes wait for a fresh A press |

</div>

<div align="center">
<img src="docs/img/final_boss.gif" width="420" alt="Belger, second phase">
<br><sub>Belger. "Beat" him once and you get <code>FINAL VICTORY</code>, then he comes back shooting.</sub>
</div>

## The whole trip

<div align="center">
<img src="docs/img/rounds.png" width="85%" alt="Rounds 1 to 5, ending and credits">
</div>

## Hunting down interpreted cycles

```mermaid
xychart-beta
    title "Seeds after each lap of the loop"
    x-axis ["boot", "attract", "mash R1", "bot", "god R1-2", "god R2-3", "god R3", "god R3-5", "god R5", "sweep 1", "sweep 2", "sweep 3"]
    y-axis "entry points" 0 --> 16000
    line [0, 2087, 7261, 8857, 10803, 11433, 11498, 13942, 14798, 15261, 15548, 15548]
```

| Point in time | Native cycles |
|---|---|
| Static discovery only (600 frames) | 91.5% |
| Attract mode, before the first rebuild | 87.1% |
| Attract mode, after it | **100%** (minus the 7 reset cycles) |
| Round 1 gameplay, before seeding | ~82% |
| Rounds 1-5 gameplay, after | **100%** |
| Final sweep: fresh bot seed, from every round start, all three fighters | **0 new seeds** |

Two instructions stay on the interpreter by design: `4k:19:9FFF` and `4k:1D:DFFF` start on the last byte of an 8 KB bank, so the rest of each one comes from whichever bank happens to be mapped next.

## Who's right? Three reference emulators

| Comparison | Result |
|---|---|
| Recompiled vs `--interp-only` (same hardware model, no compiled code) | identical per-frame hashes |
| Recompiled vs **TriCNES** (`cyc_oracle`, the engine's oracle) | bus trace, memory and picture, registers and cycle count **identical for all 5,600 frames** of the Cody route |
| Recompiled vs **Mesen** (MesenCE, `mesen_shot`) | pixel-identical through frame **5231** |

<div align="center">
<img src="docs/img/oracle_5232.png" width="90%" alt="frame 5232: NESRecomp and TriCNES vs Mesen">
</div>

At frame 5232 Mesen draws that thug one step behind, and from there the game's RNG goes its own way. NESRecomp still matches TriCNES bit for bit, so this is the two reference emulators disagreeing, not the recompiler getting it wrong. The engine's README lists similar cases in other games (SMB3's status bar IRQ, for one). Tracking down which emulator is right is left as homework.

## Building

> **Bring your own dump:** `Mighty Final Fight (USA).nes`, CRC32 `15EB0BEE` (with header), mapper 4, 128 KiB PRG, 128 KiB CHR.

**Requirements (Windows x64):** Visual Studio 2022 with *Desktop development with C++* (Build Tools is enough), CMake 3.20+ installed to `C:\Program Files\CMake`, Git, and Python 3.11+ (`py`).

```powershell
git clone --recursive https://github.com/Rhayron/MightyFinalFightRecomp.git
cd MightyFinalFightRecomp
.\setup.bat                          # submodules + nestopia-core junction
mkdir rom; copy "<your dump>.nes" rom\
.\build_all.bat                      # build_release\Release\MightyFinalFightRecomp.exe
```

| Command | What it does |
|---|---|
| `build_all.bat` | windowed build with audio and the [recomp-ui](https://github.com/RetroPortingToolKit/recomp-ui) launcher |
| `build_all.bat -Headless` | no SDL, for coverage runs and tests |
| `tools\make_release.ps1` | ZIP in `release\`, no ROM and no player data |

**Controls:** arrows · `Z` A (jump) · `X` B (punch) · `Enter` Start · `Esc` menu · `Tab` fast-forward · `F8`/`F9` save/load state · `F11` fullscreen. Xbox and DualSense pads work through SDL, and everything can be rebound in the launcher. Optional: `--extra-scanlines 128` gives the MMC3 some extra CPU time and cuts down the slowdown.

## Tools

| File | Purpose |
|---|---|
| [`tools/bot.py`](tools/bot.py) | closed-loop player; replayable routes, or coverage with `--god` |
| [`tools/state_ram.py`](tools/state_ram.py) | read and patch RAM inside a `.cycstate` (`get` / `set` / `verify`) |
| [`tools/merge_seeds.py`](tools/merge_seeds.py) | fold miss logs from parallel runs into `cyc_seeds.txt` |
| [`tools/gen_route.py`](tools/gen_route.py) | deterministic button-mash routes per character |
| [`tools/contact_sheet.py`](tools/contact_sheet.py) | turn `--shot-every` output into a timeline |
| [`tools/readme_assets.py`](tools/readme_assets.py) | rebuild every image in this README from your own dump |
| [`docs/ram_map.md`](docs/ram_map.md) | Data Crystal's RAM map, corrected and checked against the running game |

## Numbers

| | |
|---|---|
| Compiled entry points | **15,548** across 20 of the 32 4 KB banks |
| Frames played by the bots | **1.64 million** (about 7.6 hours of game time) over 25 sessions |
| Uncapped speed (window + audio) | ~157 fps, 2.6x real time |
| Incremental headless build | ~25 s |
| Release ZIP | 4.5 MB, no ROM |

## Credits

- **[NESRecomp](https://github.com/mstan/nesrecomp)** by mstan: the recompiler, the cycle-accurate runner and the TriCNES oracle. Pinned at `2b4fcdb`.
- **[recomp-ui](https://github.com/RetroPortingToolKit/recomp-ui)**: launcher and in-game menu (Dear ImGui).
- **[MesenCE](https://github.com/nesdev-org/MesenCE)**: the third pair of eyes.
- **[Data Crystal](https://datacrystal.tcrf.net/wiki/Mighty_Final_Fight/RAM_map)**: where the RAM map started.
- Project template: [YoshisCookieRecomp](https://github.com/mstan/YoshisCookieRecomp).

## Legal

This repository contains **no ROM** and no code generated from one. The build produces everything locally from **your** dump. *Mighty Final Fight* is © Capcom. This is a preservation and research project with no affiliation with Capcom or Nintendo. The screenshots above were captured from the executable running the author's own dump.

<div align="center"><sub>Working plans (Portuguese): <a href="PLAN_WINDOWS.md">PLAN_WINDOWS.md</a> · <a href="PLAN.md">PLAN.md</a> (macOS)</sub></div>
