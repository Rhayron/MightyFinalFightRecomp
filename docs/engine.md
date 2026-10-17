# How Mighty Final Fight's engine runs (PLAN_DECOMP.md P2)

Read from the generated disassembly (`disasm/tools/disasm.py`) and checked
against runs. Names are the ones in `disasm/labels.toml`; addresses are CPU
addresses in the bank given (MMC3 mode 0: even banks at `$8000`, odd at
`$A000`, `$0E` fixed at `$C000`, `$0F` fixed at `$E000`).

## Boot

`reset` (`0F:FE00`) does the usual NES start-up: interrupts off, PPU and APU
silenced, four vblank waits, all RAM cleared, the sound queue filled with the
empty marker `$88`, MMC3 banks set (CHR `$40/$42`, PRG through
`bank_switch`), both nametables cleared. Then it spawns **task 0 at
`main_task` (`0E:C556`)** and falls into the scheduler. It never returns.

## Tasks: cooperative threads with their own stacks

The main loop is a scheduler for **four tasks** (`tasks`, `$80`-`$8F`, four
bytes each):

| byte | meaning |
|---|---|
| `+0` state | 0 free, 1 sleeping, 2 running, 4 ready, 8 new |
| `+1` timer | frames left to sleep |
| `+2/+3` | entry point while new; afterwards `+2` is the task's saved stack pointer |

- `scheduler` (`0F:FEBA`) resets S to `$FF`, takes the first task whose state
  is at least 4, marks it running and either jumps to its entry point (new) or
  **switches the 6502 stack to the task's saved S** (`TXS`) and returns into it.
  Each task therefore owns a slice of page 1 (task 0 starts at `S=$BF`).
- `task_wait_frame` (`0F:FF31`) is how a task yields: timer 1, state 1, save
  S, back to the scheduler. `task_wait_frames` (`0F:FF2A`) repeats it X times.
- `nmi_task_timers` (`0E:C0E8`) counts every sleeping task's timer down once a
  frame and makes it ready at 0.
- `task_spawn` (`0F:FF02`) is called once in the whole game (reset, for
  `main_task`). `task_kill` and `task_exit` exist but no coverage run ever
  executed them.

**Port consequence (P4):** game code is written as straight-line sequences
that wait for frames in the middle (`LDX #$B4 / JSR task_wait_frames` shows
LICENSED BY NINTENDO for 180 frames). In C that is a coroutine per task, or
each task rewritten as a state machine. The lockstep harness must compare at
frame boundaries, where every task is parked in a known place.

## NMI and the hijacked return

`nmi` (`0E:C000`) saves the registers, rewrites PPUCTRL from
`ppuctrl_shadow` (`$FF`), turns rendering off, and skips straight to
`nmi_busy` while `$EE` or `$9A` is set. Otherwise it does the frame's VRAM
work, scroll (`$FC/$FD/$F8` copied to `$7C`-`$7E`), installs the default IRQ
handler (`irq_handler = $C14A`), counts `frame_counter` (`$92`) and runs
`nmi_task_timers`.

Then it **hijacks its own return** (`nmi_hijack_return`, `0E:C0FD`): it moves
the interrupted PC from the stack to `nmi_return_pc` (`$A0/$A1`) and puts
`nmi_tail` (`$C119`) there instead. RTI lands in `nmi_tail` outside the
interrupt, which pushes the registers, fakes a return address of
`nmi_return_pc - 1`, calls `sound_update` and returns into the interrupted code
with RTS. Heavy per-frame work runs with interrupts enabled without the
interrupted code noticing.

## IRQ (raster splits)

`irq` (`0E:C13B`) acknowledges the MMC3 IRQ and jumps through `irq_handler`
(`$9C/$9D`). Screens install their own handler there, which is how the
status bar split is done; the NMI resets it to `$C14A` every frame.

## Bank switching

`bank_switch` (`0F:FF4D`) maps `bank_request` (`$F4` -> `$8000`, `$F5` ->
`$A000`) through MMC3 R6/R7, keeping shadows in `$F0/$F2/$F3`.
`bank_switch_busy` (`$F6`) is held during the switch; if the post-NMI sound
update arrives then, it only sets `sound_deferred` (`$F7`) and `bank_switch`
runs it when it finishes (that path was never hit in coverage).

Banks start with an entry point at `$8000` that dispatches on a register or
RAM: bank `$00` is `player_action_dispatch` (on `game_action`, `$30`, through
a 42-entry split pointer table); bank `$04` takes X as a function index; bank
`$06` starts with a jump table (`sound_tick`, `sound_play`).

## Sound

`sound_queue_push` (`0F:F34A`) appends a command to an 8-entry ring
(`sound_queue`, `$DC`, write index `$DA`, `$88` = empty). After every NMI,
`sound_update` (`0F:FF72`) maps banks `$06/$07`, feeds each queued command to
`sound_play` (`06:8003`), calls `sound_tick` (`06:8000`) once, and restores the
game's banks. The sound driver lives entirely in banks `$06/$07`.

## Objects

Sixteen slots, one byte per slot in tables `$10` apart (`obj_state` `$0300`,
`$0310`, `obj_id` `$0320`, `$0330`, ..., `obj_x` `$0360`, `obj_y` `$03C0`,
`obj_life` `$0490`, `obj_life_max` `$04A0`, up to `$05E0`; every field in
`docs/objects.md`). Slot 0 is the
player. `object_clear` (`0F:F2BE`) zeroes one slot, `objects_clear_all`
(`0E:C334`) all of them; spawners copy a type's life into `$0490/$04A0` from
per-type tables (bank `$08`).

## Game flow (main_task)

1. LICENSED BY NINTENDO (bank `$04` function 4, 180 frames), CAPCOM PRESENTS
   (function 0, 180 frames, skippable), `rom_integrity_check`.
2. The story: `intro_page` (`$B8`) 0-4, each drawn by `$9A00` in bank `$00`,
   with the illustrated screens (bank `$04` functions 6 and 7) in between.
   Holding any button (`joy_held`) jumps to the title.
3. `title_screen` (`0E:C657`): music `$0C`, three timers at `$0500/$0510/$0520`.
   When all run out it does **`JMP reset`**: attract mode is the whole game
   restarting from power-on, LICENSED BY NINTENDO included.
4. `title_wait_start`: `joy_pressed & $10` (START) -> character select (bank
   `$04` function 2, `$9100`), round map (function 8, `$8D00`, 45 frames),
   then the new game: IRQ on, scroll and objects cleared, `$22` 0 (the round
   index for per-round tables), the round's music (`round_music`) and
   `$A000` bank (`round_bank`), its header from `$AE68`. `sub_area` (`$60`)
   is the area inside a round.
5. New game: the player in slot 0 at `$30,$90`, `health = start_health[player_id]`
   (Cody `$40`, Guy `$30`, Haggar `$50`), `lives` 5, `credits` 3, `round` 0,
   then `gameplay`.

## The gameplay frame (`gameplay`, `0E:C8ED`)

Once a frame, in task 0:

1. Special states: `game_action` 9 (continue), `game_finished` (`$B7`),
   `special_screen_id` (`$62`: `gameplay_special_screen` runs the message
   box, bank `$04` `$9800`, instead of the game; `flow_talk.c`).
2. `gameplay_pause_check`: START toggles `paused` (`$98`) with sound `$15`
   (`inputs/10_cody_poweron.txt` pauses at frames 18909 and 18925).
3. `gameplay_update`: `player_state` (fixed bank, dispatched on `game_action`
   through a 43-entry table: idle, jump, punch, grab, hit, knockdown, continue,
   throw...; every special move from `$0F` up goes to `player_state_default`
   and is handled per character in bank `$00`), then
   `player_action_dispatch` (bank `$00`), `$E2A3`, banks `$08/$09`
   (`stage_spawns` `$8080`, `stage_events` `$8900`, `$A300`), bank `$0C`
   `objects_update` (every live slot: map the id's behaviour bank into `$A000`,
   dispatch through `obj_behavior_lo/hi`: 160 ids, 101 distinct entries, of
   which 64 are the `JMP` tables of banks `$05`, `$0B`, `$01` and `$03`, and 70
   routines behind them; `docs/objects.md`, "Ids"), `hud_update` (`$E6AB`;
   skipped while `special_screen_id` is set, and `$F9` there is a token: the
   scene after Belger sets it every frame and `0F:E6B4` takes it back to 0
   instead of drawing the status bar).
4. `gameplay_draw` (`$C31D`, `$E9BC`), `gameplay_frame_end`
   (`task_wait_frame`), and around again.

## Odds and ends

- The player's states have a few paths no run takes: `player_idle_buttons`
  tests A at `0E:CAEB`, but `player_state_idle` has taken A already
  (`0E:CA2E`); the grab's `BEQ` at `0E:CE6A` (no Left or Right held) never
  branches, because the grab search runs only with one held; and
  `player_respawn_blink` reads its white `$FCCB,Y` without loading Y (it is
  0 from `rng_step`, and all three bytes are `$30`).
- Leaving an area: `game_action` `$19` walks the player out the way of each
  area (`00:9359`, by round and area from `$9382`). Round 4's first area
  ends otherwise: from its third screen the camera sets `game_action` `$26`
  (`0F:E383`), a scripted drop (`00:91BE`). The CDL runs never saw that, nor
  the exits `00:9610`, `961A` and `961F`.
- The credits' music is a round's tune by `lives` (`04:962D`, table
  `04:965E`: 0-4, lives mod 5).
- `rom_integrity_check` (`0E:C5B3`): after CAPCOM PRESENTS the game compares
  16 bytes at `$FFE0` with a copy at `$C8DD` and requires `$FFF8 = 8`;
  otherwise it hangs. Irrelevant to the port.
- Abigail (round 3 boss) seemed to take no damage in 1,200 frames of the bot's
  attacks: that watch was on `$04A0`, which is the maximum life; the current
  life is `$0490` (docs/objects.md).
