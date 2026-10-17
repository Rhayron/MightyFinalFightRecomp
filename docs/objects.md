# The object system

Everything that moves on screen during gameplay, apart from the status bar, is an
object: the player, enemies, bosses, items, thrown bodies, hit sparks, the GO
sign. This page describes the slots, every field, and the life of an object from
spawn to free, with the addresses of the code that does each step. Names are the
ones in `disasm/labels.toml`; `BB:AAAA` is bank and CPU address, `$XXXX,X` is a
table indexed by slot.

How it was worked out: an access list for every RAM byte over all the code the
CDL map knows (`runs/ram/xref.py`), reading the routines in an addressed listing
(`runs/ram/listing.py`), and checking the values in the frame logs of the bot
tours (`runs/p4/base_{cody,guy,haggar}_tour.flog`, 50,000 frames each through
all five rounds, and the round-1 logs).

## Slots and tables

There are 16 slots. Each field is a 16-byte table, so field `F` of slot `X` is
`F + X`; the tables fill `$0300-$05FF` at a stride of `$10`. Code runs with the
slot in X (or in Y when one object works on another).

| Slot | Used for |
|---|---|
| 0 | The player. Driven by `player_state` and bank `$00`, not by `objects_update`. It uses the position, speed, direction, flags and animation fields like any object; the rest of its state is in zero page (`game_action`, `health`, ...) |
| 1, 2 | Player-side objects: the dropped weapon (id `$66`, put in slot 1 by `0F:FAC4`) and still display objects (id 0) shown around round starts. `0F:F9E5` tests slots 2 and 1 against the other objects |
| 3-`$0D` | Enemies, bosses, items, stage objects. Both spawners search from `$0D` down to 3 (`08:80F8`, `0F:F71A`), so the first two enemies land in `$0D` and `$0C`; slots 3 and 4 were never used in the logs |
| `$0E`, `$0F` | Effects (hit sparks, id 1): `0F:F72A` looks for a free slot only there |

A slot is live when bit 7 of `obj_state` is set. Some code also uses fixed
slots directly (`$032D`, slot `$0D`'s id, is read by boss code; bank `$00`
writes slots `$0D` and `$0F` field by field).

### Fields

| Table | Name | What it holds |
|---|---|---|
| `$0300` | `obj_state` | Bit 7 live. Bits 0-3: phase of the behaviour's state machine; nearly every behaviour dispatches on `obj_state & $0F` (e.g. `0C:9D28`). Bits 4-6 unused |
| `$0310` | `obj_saved_state` | State to resume after a temporary behaviour (grab, knockdown) |
| `$0320` | `obj_id` | Behaviour id: selects the routine in `obj_behavior_lo/hi`, the sprite banks (`$EB93`), and many id checks. Changes while the object lives (an enemy becomes id 5 when knocked down) |
| `$0330` | `obj_saved_id` | Id to resume. Spawners write both ids |
| `$0340` | `obj_x_frac` | x sub-pixel |
| `$0350` | - | Never accessed, always 0 |
| `$0360` | `obj_x` | World x, low byte |
| `$0370` | - | Never accessed, always 0 |
| `$0380` | `obj_x_hi` | World x, high byte (the stage is several screens wide) |
| `$0390` | - | Never accessed, always 0 |
| `$03A0` | `obj_y_frac` | y sub-pixel |
| `$03B0` | - | Never accessed, always 0 |
| `$03C0` | `obj_y` | y on screen (larger is lower). For a walking object it is the ground line; while it is in the air it includes the height |
| `$03D0` | `obj_pit_flag` | Pit handling of the common-enemy AI (1/2: pit beside it, walk away; nonzero during a pit fall) |
| `$03E0` | `obj_y_hi` | Vertical page of `obj_y` (y runs 0-`$EF` per page). Supported by the code, always 0 in practice |
| `$03F0` | `obj_held` | Nonzero while the player holds the object; `$FF` on release |
| `$0400`/`$0410` | `obj_vx_frac`/`obj_vx` | Horizontal speed, unsigned (direction comes from `obj_dir`) |
| `$0420`/`$0430` | `obj_vy_frac`/`obj_vy` | Vertical speed, signed, positive = up |
| `$0440` | `obj_attack` | Bit 7: an attack is out. Bits 0-4: index of its reach in `$F6AA`/`$F6CA`/`$F6EA` |
| `$0450` | `obj_attack_damage` | Index into the damage tables `$FD16` (health lost) and `$FD56` (knocks down) |
| `$0460` | `obj_dir` | Direction in pad bits: 1 right, 2 left, 4 down, 8 up |
| `$0470` | `obj_throw_flag` | `$FF` when thrown with the pad held forward (a different throw path) |
| `$0480` | `obj_spawn_index` | Entry of the stage object list the slot came from, `$FF` none |
| `$0490` | `obj_life_max` (sic) | **Current** life. Hits subtract from it; negative = dead. See "Wrong labels" |
| `$04A0` | `obj_life` (sic) | **Maximum** life, the length of the HUD bar. See "Wrong labels" |
| `$04B0` | `obj_hit_cooldown` | Frames before the slot can be hit again |
| `$04C0` | `obj_invuln` | Nonzero = cannot be hit; also the stage counter of a knockdown |
| `$04D0` | `obj_hit_player` | `$FF` when this object's attack hit the player |
| `$04E0` | `obj_hit_timer` | `$1E` when a player hit lands, counted down by the behaviour |
| `$04F0` | `obj_spawn_group` | Per-kind code (3/4/5, `$FF` for non-enemies) used to choose the next timed spawn |
| `$0500` | `obj_timer` | General per-behaviour timer or step counter |
| `$0510` | `obj_timer2` | Second counter (death blink, dash target id, boss attack index) |
| `$0520` | `obj_ground_y` | Ground line while airborne (some behaviours use it as a counter) |
| `$0530` | `obj_land_y` | Line a knocked-back object lands on |
| `$0540` | `obj_timer3` | Third counter (thrown-body collision delay, AI attack timer) |
| `$0550` | `obj_down_timer` | Knockdown recovery timer (lying, getting up) |
| `$0560` | `obj_flags` | Display flags, below |
| `$0570` | `obj_anim_frame` | Frame index in the animation script |
| `$0580` | `obj_anim` | Animation id, 0 = nothing drawn |
| `$0590` | `obj_anim_timer` | Frames shown of the current frame; bit 7 blink |
| `$05A0` | `spawn_groups` | Not a slot table: 3 bytes of scratch for the timed spawner |
| `$05B0` | `obj_kind` | Enemy kind (below); 0 for non-enemies |
| `$05C0` | `obj_tier` | Strength tier of common enemies: 1, 2 or 4 |
| `$05D0` | `obj_ai_timer` | Countdown of the current AI phase |
| `$05E0` | `obj_ai_count` | AI step/attack counter |
| `$05F0` | `obj_ai_flag` | AI coin flip (0/1) |

`obj_flags` (`$0560`):

| Bit | Meaning | Evidence |
|---|---|---|
| 7 | On screen this frame. `object_draw` sets it for a visible slot and clears it off screen; hit tests require it | `0F:EA4D`, `0F:EA62`, `0F:F738`, `0F:F55A` |
| 6 | Facing right: the sprites are mirrored (the art faces left). `0F:F090` sets it before moving right (`0C:9D85`), `0F:F099` clears it before moving left (`0C:9D98`), `0F:F32E` copies `obj_dir` bit 0 into it | `0F:EA89`, `0F:EB56`, `0F:F94E-F962` (player: bit 6 set = `$31` 1) |
| 5 | Sprites behind the background (ORed into the OAM attribute) | `0F:EA8E`, `0F:EB58` |
| 4 | World coordinates: the screen position is `obj_x/obj_x_hi - scroll_x/scroll_x_hi`. Clear = `obj_x`/`obj_y` are screen coordinates | `0F:E9FE-EA0B`, `0F:EA58` |
| 2 | Hidden: not drawn and not hit. The death blink toggles it (`0C:9B62`/`9B6B`) | `0F:EA6A`, `0F:F73D`, `0F:F560` |

Spawn types set `$D0` (on screen, facing right, world) or `$D4` (also hidden);
the spawner then turns the object toward the player.

`obj_kind` (`$05B0`): 0 Poison, 1 Two P, 2 Bred, 3 AxL, 4 El Gado, 5 Andore,
6 id `$2E`, 7 Thrasher, 8 Sodom, 9 Abigail, `$0B` Belger, `$0C` id `$9D` (the
objects that fill the screen during the Belger fight). Kinds 7, 8, 9 and `$0B`
get the HUD life bar ahead of anyone else (`0F:E89D`); kind 3 can block
(`0C:84F0`).

## Spawning

Every spawn path finds a slot whose `obj_state` bit 7 is clear and then writes
the fields it cares about. The ones that matter:

- **Stage object list** (`08:80A7`, reached from `stage_spawns` in scroll mode
  1). The round's list gives, per entry, x, x high, y, y page and a type
  (pointers `$72/$74/$76/$78/$7A`). As the scroll reaches an entry (`08:80C2`
  compares it with `scroll_x`), `08:80E4` skips it if a slot already carries
  that `obj_spawn_index`, takes a free slot from `$0D` down to 3, and fills it
  from two per-type tables: `08:8442` (4 bytes: `obj_flags`, `obj_id` and
  `obj_saved_id`, `obj_anim`, `obj_attack`) and `08:85E6` (life, written to both
  `$0490` and `$04A0`, `obj_spawn_group`, `obj_kind`). State and saved state
  become `$80`, `obj_vy` gets a small downward start (`0F:F26E`), the rest is
  zeroed, and `0F:F314`/`0F:F32E` turn it toward the player (`08:81BB`).
- **Timed spawner** (`08:9000`, called from the round event scripts in bank
  `$08` and from `0E:DEBC`). It counts `$63` down; at zero it reloads it from
  `08:9024` and spawns a common enemy at a random spot (`08:9056`). To choose
  the type it adds up the `obj_spawn_group` codes of up to two live enemies in
  `spawn_groups` (`08:910A-912A`) and uses the sum, the round, the area and
  `scroll_x_hi` to index the type tables at `$9456` (id), `$94DD` (animation),
  `$9564` (kind), `$93CF` (group). Life comes from per-round tables
  (`08:91EB-9202`).
- **Effects.** `0F:F455` puts a hit spark (id 1, animation `$7E`) in slot `$0E`
  or `$0F` next to the player's fist whenever an attack connects;
  `0F:FAC4` turns slot 1 into the dropped weapon (id `$66`) when the player is
  hit while holding one.
- **Behaviours** spawn their own projectiles and helpers (Abigail `03:BC10`,
  stage objects `05:AC82`, common enemies `0D:A60F`, Belger `0B:B46F`) with the
  same pattern.

Shared helpers: `0F:F2A0` starts slot Y with animation A (frame and timer 0,
`obj_flags = $10`, state and saved state `$80`); `0F:F288` sets the animation
of slot X; `0F:F294` the player's.

## Update: `objects_update` (`0C:8003`)

Called once per gameplay frame from `gameplay_update`. It sets the gravity
`$99 = $38` and walks slots 1-15 (slot 0 is the player's business). For each
live slot it maps the id's behaviour bank into `$A000`, pushes a return address
and jumps through `obj_behavior_lo/hi` (`0C:83AF`/`0C:844F`, 160 entries).

The bank depends on the id (`0C:8010-803B`): `$60-$6F` bank `$05`, `$70-$7F`
bank `$0B`, `$80-$8F` bank `$01`, `$90` and up bank `$03`, and **every id below
`$60` gets bank `$0D`**. The code shifts the id right four times but never moves
the result into Y, which still holds the `$0D` loaded at `0C:8010` (the
`SBC #$06 / TAY` that would use it is dead code). All ids below `$60` dispatch
into bank `$0C` itself, and their calls into `$A000` land in bank `$0D`: the
common enemy routine at `0C:9D1F` calls `$AB29`, which is code in bank `$0D`.
`tools/mff_dispatch.py` and `docs/engine.md` say "bank id >> 4"; that only
happens to be harmless for the dispatch table because none of those targets is
in `$A000-$BFFF`.

When a behaviour returns (`0C:805E`), the loop moves to the next slot and, before
dispatching it, runs the collision checks for it (`0C:8067-820E`). Slot 1 is
dispatched first without them.

1. If `obj_hit_cooldown` is nonzero it is counted down and the being-hit checks
   are skipped (straight to step 5).
2. Thrown bodies (ids 4, 8, `$0F`, and `$0E` during attack 9) test every other
   slot (`0C:8244`): after `obj_timer3` frames, while the player is in a throw
   (`game_action` `$0B`, `$12`, `$13`, `$15`), any live slot `$0D`..1 with a
   different id that is on screen, not held, not invulnerable and within `$1A`
   px in x and `$0C` in y is knocked down (id 3, `obj_vy = $038E`, flying away
   from the player).
3. `0F:F9E5` tests the object against whatever is in slots 2 and 1 (live, on
   screen, not ids `$66`, `$69`, 9). On contact the slot-1/2 object is removed
   (or, with animation `$74`, turned into id 9) and the object takes a hit
   (`0C:8095-80BE`). Only the first checks ever ran in coverage; the position
   test (`0F:FA09-FAC3`) and the contact path are unseen.
4. The player's attack (`0F:F738`): the slot must be on screen, not hidden, the
   player attacking (`game_action != 0`, `$A4` set), `obj_invuln` 0, and the
   id one that can be hit (from `$1F` up, with exceptions). The reach depends on
   the attack (`$A7`, tables at `$F869`/`$F88D`/`$F8B1`). On a hit:
   - `0F:F455` spawns the spark;
   - `0C:84F0` applies it: AxL may block (random, odds by tier), otherwise
     `$B0`/`$B1` select this slot's HUD bar, `0F:FB09` loads
     `obj_hit_cooldown` and sets `obj_hit_timer = $1E`, and per-character
     code (`$87B0`/`$87B3` by `player_id`, then a table per level `$6B`
     indexed by the attack) finds the damage; bosses take `$87AB[kind - 7]`
     less, one less again in round 4, never under 1, and `0C:86E4` subtracts
     it from `$0490`;
   - `obj_attack` is cleared and `obj_land_y` set (`obj_y`, `obj_y+$0A` for
     kinds 6 and `$0C`, `obj_ground_y` for the airborne animations);
   - if `$0490` went negative the object becomes a dying id (6 or 7 by attack,
     `0C:81B8`; Belger would become `$9F` at `0C:8211`, a path no run
     executed), `$54` is incremented and the spawn index is released
     (`0C:822F`);
   - otherwise attacks `$01/$07/$0D/$18` turn it into id 2 (pushed back a
     little) and the heavier ones into id 5 (knocked down, `obj_vy = $038E`),
     both with `obj_state = $80` (`0C:8136-81B0`); other attacks leave the
     behaviour alone. AxL only reacts if `obj_hit_timer` survived his block
     check.
5. The object's own attack (`0C:81F0`): if `obj_attack` bit 7 is set, the player
   is not invincible (`$3B`) and `0F:F55A` finds the player in reach (box
   `obj_attack & $1F`, the player's depth `$36`), then `obj_hit_player = $FF`,
   `$3B = $3C` (60 frames of invincibility) and `0F:F8D5` hurts the player:
   `health -= $FD16[obj_attack_damage]`, knockdown if `$FD56[...]` or if health
   runs out, facing turned toward the attacker, a held enemy released and a held
   weapon dropped (Guy keeps his). A hit while jumping or during Cody's slash
   always knocks down, and leaves `player_dir` 2 whichever way he faces
   (`BEQ $F9B7` lands on the knockdown's `LDA #2`; `base_guy_alt.flog` frame
   11807).

Then the slot is dispatched (`JMP $800B`).

### Knockdown, getting up, dying

The hit reactions in bank `$0C` (ids 2, 3, 5, 6, 7 and the thrown bodies) move
the object with `obj_vx`/`obj_vy`, land it on `obj_land_y` (e.g. `0C:8EBF`),
check for a pit under it (`0C:9B1F`: background tile bit `$40` turns it into
id `$1F`, which falls until `obj_y >= $B0` and dies), and run the recovery in
`0C:9BFC`: `obj_invuln` goes `$FF` (lying), `$FE` (`obj_down_timer` `$2D`
frames, then the get-up animation), `$FD` (`$17` frames), after which
`obj_saved_id`/`obj_saved_state` are restored, `obj_hit_timer` and `obj_held`
cleared, and `obj_invuln` reloaded from `$FC82[obj_kind]` as a short immunity
counted down by the behaviour.

A dead enemy blinks: `obj_timer2` counts `$10` frames while `obj_flags` bit 2
flips every frame (`0C:8F8D`, `0C:9B07`), then the slot is cleared. An enemy
that fell into a pit is hidden, gets `$0490 = $FF`, and is cleared after `$10`
frames of `obj_ai_timer` (`0C:9B98-9BDE`).

### Being held

The grab (`0E:CE6C-CE92`, which sets `game_action` 4) keeps the held slot in
`$39`, the hold time in `$38` (`$3C` frames, Haggar `$5A`) and a copy of it in
`obj_held`, and zeroes the held object's `obj_attack`.
Throwing turns the object into a thrown-body id (4 for Cody at `0E:D7F8`),
optionally with `obj_throw_flag`. Releasing it writes `$FF` to `obj_held`; the
behaviour clears that and recovers (`0D:AB29-AB7F`), or `00:8355-836A` restores
`obj_saved_id`/`obj_saved_state` directly.

## Clearing

- `object_clear` (`0F:F2BE`): zeroes slot X in `obj_state`, `obj_saved_state`,
  `obj_flags`, `obj_hit_cooldown`, both ids, `obj_attack`,
  `obj_attack_damage`, both life bytes, `$04C0-$04F0`, `$03F0`, `$05B0-$05F0`,
  `$0500-$0550`; `obj_spawn_index = $FF`. Position, speed and animation are
  left as they were.
- `0F:EC81`: a shorter clear (state, saved state, flags, `obj_throw_flag`,
  `obj_hit_cooldown`, both ids, `obj_attack`, `obj_attack_damage`,
  `$04C0-$04F0`, `$05B0-$05F0`, `obj_held`; `obj_spawn_index = $FF`) that
  leaves the life bytes and `$0500-$0550` alone and returns with carry clear.
  `object_draw` jumps here when an animation reaches its end marker.
  (`object_clear`, for its part, does not clear `obj_throw_flag`.)
- `objects_clear_all` (`0E:C334`): all 16 slots, a shorter list (state, saved
  state, flags, `obj_throw_flag`, `obj_hit_cooldown`, both ids, `obj_attack`,
  `obj_attack_damage`, both life bytes), `obj_spawn_index = $FF`. Called at
  game and round start (`0E:C6FE`, `C744`, `C7A7`) and on area changes
  (`0E:DE7F`, `DEE1`, `DF19`, `DFAF`, ...).

## Drawing

`gameplay_draw` sets the OAM write index `$97 = 4` and calls `objects_draw`
(`0F:E9BC`): slots `$0F` and `$0E` first, then 0-`$0F` on even passes of
`draw_parity` and `$0D`-0 on odd ones. Changing the order every frame makes
sprites past the 8-per-line limit flicker instead of vanishing. On even passes
the effect slots are visited twice, so their animations advance two steps on
those frames: in the tour logs slot `$0F`'s `obj_anim_timer` grew by 2 after
every even pass and by 1 after odd ones.

`object_draw` (`0F:E9FE`), for slot X:

1. Position: with `obj_flags` bit 4, screen x = `obj_x/obj_x_hi -
   scroll_x/scroll_x_hi`. If that is more than `$68` px off the screen
   (`0F:EC33`), objects with ids in the list at `$ECC3` (`$2E $9D $7F $91 $95
   $01 $10-$13 $61`) are freed and everything else is moved 16 px back toward
   the screen, so enemies cannot be lost off screen. A slot that is off screen
   but closer gets bit 7 of `obj_flags` cleared: its animation still runs but
   nothing is drawn and it cannot be hit. With bit 4 clear, `obj_x`/`obj_y`
   are screen coordinates.
2. Hidden (`obj_flags` bit 2): return.
3. Banks: `$8000 = $EB93[obj_id]`, `$A000` = that + 1 (`$0A/$0B` or
   `$02/$03`, by id), so data bank `$02` is mapped at `$8000` here
   (`0F:EA6F-EA7E`). The tools used to place bank `$02` at `$A000`
   (`tools/cdl_report.py` `window()`, "put at `$A000` by objects_update"; fixed,
   and `tools/mff_dispatch.py` now has bank `$0D` for ids below `$60`), but
   nothing ever maps it there: every write of the `$A000` request (`$F5`)
   comes from `round_bank` (`$01 $05 $07 $09 $0D`), `objects_update` (`$0D`,
   `$05`, `$0B`, `$01`, `$03`), `object_draw` (`$03`/`$0B`), an `INX` after an
   even `$8000` bank, `$09` at `0E:C532`, or a saved copy (`runs/ram/bankreq.py`).
4. Animation: the script pointer is `$8000/$8080 + (obj_anim & $7F)`. Byte 0 is
   the last frame index, byte 1 the step length (each frame lasts byte 1 + 1
   draws), bytes 2.. the frame ids. `obj_anim_timer` is incremented; when it
   had reached byte 1 it restarts and `obj_anim_frame` advances, wrapping to 0
   after byte 0. A frame id of 0 ends the object (`obj_state = 0`,
   `0F:EC81`). If `obj_anim_timer` bit 7 is set the sprite blinks on
   `frame_counter` bit 2.
5. Sprites: the frame id indexes `$8100/$8200` for the frame's data (sprite
   count, bit 7 selecting the layout bank `$03` instead of `$0B`; layout index;
   then tile and attribute per sprite) and the layout's offsets come from
   `$B100/$B200`. Each sprite goes to OAM at `$0200 + $97`, mirrored when
   `obj_flags` bit 6 is set, with bit 5 as the priority bit, and is dropped if
   it falls off the screen edge. Drawing stops when the OAM buffer is full.

## Movement helpers (bank `$0F`)

| Routine | Does |
|---|---|
| `F183` / `F1A1` | x += / -= `obj_vx` (with `obj_x_frac`, carry into `obj_x_hi`) |
| `F1BF` / `F1DF` | y += / -= `obj_vy` (with `obj_y_frac`; `obj_y_hi` on page wrap) |
| `F1FD` | y -= `obj_vy`, then gravity (`F24C`: `obj_vy -= $99`, not below -7) |
| `F26E` | `obj_vy = -$40/256` for the player, `-$55/256` for others |
| `F090` / `F099` | face right / left (`obj_flags` bit 6 set / clear) |
| `F314`, `F32E`, `F343` | point `obj_dir` at the player, copy it into the flags, both |
| `F365`, `F374`, `F38B` | distance to the player in y, in x, and the direction index |
| `FB3E`, `FB5B`, `FB72`, `FBD9` | the AI's tests around the player's ground line: within A px in x and Y in y (C set); `F38B` to his feet; to the spot `$FBCC[obj_kind]` px beside him on the object's side; `F365` from his ground line |
| `F000` / `F051` | face right / left, then `F003` / `F054`: move, `bg_probe_2` of kind Y; a pit's edge is a wall (pushed back out, C set) for any slot but the player, who starts falling in (`F0DC`) unless coming down |
| `F0A2` / `F0F7` | move down / up, `bg_probe` of kind Y; put back on top of a `$10` / below a `$30` (C set) |
| `F119` | a jump's vertical step: `F20E`/`F22E` (`obj_fall`'s halves), gravity, `bg_probe`; landing on `$10` (C set) or the pit fall |
| `ECCF`, `EDBA` | background probes around the object (tile attributes of the round's map; bit `$40` = pit) |
| `F71A`, `F72A` | free slot in `$0D`..3 / `$0F`..`$0E` (carry set if none) |

## Ids

`obj_behavior` has 160 entries (`tools/mff_dispatch.py` lists them, with the
bank caveat above). The ones the engine itself relies on:

| Id | Routine | What |
|---|---|---|
| 0 | `0C:84EF` (RTS) | No behaviour: animation only (display objects in slots 1-2) |
| 1 (9 shares it) | `0C:89F8` | Hit spark |
| 2 | `0C:9733` | Pushed back by a light hit |
| 3, 5 | `0C:983B` | Knocked down (3 by a thrown body, 5 by a heavy hit) |
| 4, 8, `$0E`, `$0F` | `0C:8C1C`, `8FA5`, `91B7`, `9332` | Thrown bodies |
| 6, 7, `$16` | `0C:9962`, `9A3F` | Dying |
| `$10`-`$13` | `0C:8AEE` | Items: picked up with B when the player is close (`0C:8B22-8B3F`) |
| `$1F` | `0C:9B74` | Falling into a pit |
| `$20`-`$35` | `0C:9CED/9CF1/9CF5` | Common enemies: low digit = kind (via `0C:9D5B`), group = tier |
| `$66` | `05:A712` | Dropped weapon |
| `$74` | `0B:B30C` | Belger |
| `$80`, `$81`/`$83`, `$90` | banks `$01`, `$03` | Thrasher, Sodom, Abigail |

The enemy ids from Data Crystal in `docs/ram_map.md` (Poison `20 26 30`, ...) are
these behaviour ids: `$20-$25`, `$26-$2B` and `$30-$35` are the same six
enemies at tiers 1, 2 and 4.

## Wrong labels

- `$0490` is named `obj_life_max` and `$04A0` `obj_life`, but it is the other
  way round. Damage is subtracted from `$0490` (`0C:86E4`), `$0490` going
  negative is the death test (`0C:80CB`, `8111`, `8134`, `9BA5`), the HUD fills
  the bar from `$0490` and sizes it from `$04A0 / 8` (`0F:E8E0`, `E8F2`), and
  in `base_cody_round1.flog` slot `$0C`'s `$0490` goes `28 26 1E 16 0E 06 FE`
  over five hits while `$04A0` stays `28`. `$04A0` is only written at spawn,
  with the same value. Data Crystal's "enemy life `$049B/$049E`" was right.
  Consequences: the bots' "finish a long fight" patch (`port/scaffold/bot.c`
  line 157, `tools/bot.py` line 181) writes 1 to `$04A0`, which only shortens
  the HUD bar (Belger in `base_cody_tour.flog`: `$04AC` goes from `$68` to 1 at
  frame 40414 while `$049C` stays `$59`); and the note that Abigail "took no
  damage, `obj_life` stayed put" was watching the maximum.
- The comments on `obj_flags` ("bit 4 = drawn") and `object_draw` ("if obj_flags
  bit 4") read bit 4 as visibility. It selects world versus screen
  coordinates; slots with bit 4 clear are drawn too.
