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
| `$0490` | `obj_life` | Current life. Hits subtract from it; negative = dead. See "Names fixed" |
| `$04A0` | `obj_life_max` | Maximum life, written only at spawn; the HUD bar is `obj_life_max / 8` long |
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
6 El Gado's knife (id `$2E`), 7 Thrasher, 8 Sodom, 9 Abigail, `$0B` Belger,
`$0C` id `$9D` (Belger's shots, fired at `0B:BBEC`), `$0D`
the two pieces Sodom sheds (`0C:877F`, never executed). Kinds 7, 8, 9 and `$0B`
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
- **Behaviours** spawn their own projectiles and helpers (round 1's drum
  dispenser `03:BC0E`, stage objects `05:AC82`, common enemies `0D:A60F`,
  Belger's shot `0B:BBEC`, the bonus stage's drums `0B:B46F`) with the same
  pattern.

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
     indexed by the attack, `0C:8638-8683`; Guy's levels 0 and 1 share one
     table) finds the damage; bosses take `$87AB[kind - 7]` less (Sodom with
     `obj_tier` set `$87AB[3]`; Sodom first takes 5 more from attacks 5 and
     `$13`, never executed), one less again in round 4, never under 1, and
     `0C:86E4` subtracts it from `$0490`. Sodom at `obj_tier` 1 whose life
     falls under `$30` would then shed two pieces into slots `$0D` and `$0B`
     (`0C:8703`, never executed);
   - `obj_attack` is cleared and `obj_land_y` set (`obj_y`, `obj_y+$0A` for
     kinds 6 and `$0C`, `obj_ground_y` for the airborne animations);
   - if `$0490` went negative the object becomes a dying id (6 or 7 by attack,
     `0C:81B8`; Belger becomes `$9F` at `0C:8211`, which no route plays
     (`boss_belger.c`, `boss_belger_end.c`), and a drum, any object with
     animation `$7B`, becomes the smashed drum `$95` at `0C:821F`), `$54` is
     incremented and the spawn index is released
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
| `F38B` | `obj_dir_index`: the direction from slot X to the player as a 16-step compass, 0 up, 4 right, 8 down, `$0C` left, clockwise; `FBF0` turns an index into pad bits for `obj_dir` |
| `F000` / `F051` | face right / left, then `F003` / `F054`: move, `bg_probe_2` of kind Y; a pit's edge is a wall (pushed back out, C set) for any slot but the player, who starts falling in (`F0DC`) unless coming down |
| `F0A2` / `F0F7` | move down / up, `bg_probe` of kind Y; put back on top of a `$10` / below a `$30` (C set) |
| `F119` | a jump's vertical step: `F20E`/`F22E` (`obj_fall`'s halves), gravity, `bg_probe`; landing on `$10` (C set) or the pit fall |
| `ECCF`, `EDBA` | background probes around the object (tile attributes of the round's map; bit `$40` = pit) |
| `F71A`, `F72A` | free slot in `$0D`..3 / `$0F`..`$0E` (carry set if none) |

## Ids

`obj_behavior_lo/hi` (`0C:83AF`/`0C:844F`) has 160 entries, ids `$00`-`$9F`
(`tools/mff_dispatch.py` lists them). `objects_update` maps a bank at `$A000`
by the id first (`0C:8010-803B`, above); from `$60` up each bank opens with
sixteen `JMP`s, one per id, and `obj_behavior` points at them:

| Ids | Bank at `$A000` | `obj_behavior` points at | Ids with no behaviour go to |
|---|---|---|---|
| `$00`-`$5F` | `$0D` (the common-enemy AI) | the routine itself, in bank `$0C` (`$84EF`-`$9F12`) | `0C:84EF` `obj_behavior_none`, an RTS |
| `$60`-`$6F` | `$05` | `05:A700 + 3 * (id & $0F)`, a `JMP` | `05:A730` `prop_behaviors` (RTS) |
| `$70`-`$7F` | `$0B` | `0B:B300 + 3 * (id & $0F)` | `0B:B330` `b0b_no_behavior` (RTS) |
| `$80`-`$8F` | `$01` | `01:A000 + 3 * (id & $0F)` | `01:A030` `b01_no_behavior` (RTS) |
| `$90`-`$9F` | `$03` | `03:B300 + 3 * (id & $0F)` | `03:B330` `b03_no_behavior` (RTS) |

That makes 101 distinct entries and 70 distinct routines behind them, 64
without the RTSs. Banks `$03` and `$0B` hold different code at the same
addresses (`03:B331` and `0B:B331` have nothing in common). The port keeps
the tables as one-line jumps: `obj_props.c` (`$05`), `enemy_tables.c` (`$03`,
`$0B`), `boss_b01_shared.c` (`$01`).

| Id | Routine | What | `port/src/game` |
|---|---|---|---|
| 0 | `0C:84EF` `obj_behavior_none` | Nothing: animation only (display objects in slots 1-2) | `obj_update.c` |
| 1 | `0C:89F8` (an RTS) | Hit spark: it only animates, and its script's end marker frees it | `obj_hits.c` |
| 2 | `0C:9733` `pushed_back` | Pushed back by a light hit | `obj_knockdown.c` |
| 3, 5 | `0C:983B` `knocked_down` | Knocked down by a thrown body (3) or a heavy hit (5) | `obj_knockdown.c` |
| 4 | `0C:8C1C` `thrown_cody` | The enemy Cody throws | `obj_thrown.c` |
| 6 | `0C:9962` `killed_light` | Killed by a light attack | `obj_knockdown.c` |
| 7, `$16` | `0C:9A3F` `killed` | Killed by another attack (`$16`: by a slam) | `obj_knockdown.c` |
| 8 | `0C:8FA5` `thrown_back` | The enemy Haggar's back throw slams behind him | `obj_thrown.c` |
| 9 | `0C:89F8` (an RTS) | A slot-1/2 object with animation `$74` after a contact (`0C:8095-80BE`, never executed) | `obj_hits.c` |
| `$0A`-`$0D`, `$14`, `$15`, `$17`-`$1E` | `0C:84EF` | Nothing | |
| `$0E` | `0C:91B7` `thrown_smash` | The enemy Haggar lifts for the HeadSmasher | `obj_thrown.c` |
| `$0F` | `0C:9332` `thrown_guy` | The enemy Guy throws | `obj_thrown.c` |
| `$10`-`$13` | `0C:8AEE` `obj_item` | Items, taken with B (`0C:8B22-8B3F`) through `$8B5C/$8B60` by id: food (`$10`, health, or points at full health), a credit (`$11`), two lives (`$12`), one life (`$13`), each up to 9 | `obj_items.c` |
| `$1F` | `0C:9B74` `pit_fall` | Falling into a pit | `obj_knockdown.c` |
| `$20`-`$25` | `0C:9CED` `enemy_common` | Poison, Two P, Bred, AxL, El Gado, Andore at tier 1 (below) | `enemy_common.c` |
| `$26`-`$2B` | `0C:9CF1` `enemy_common_tier2` | The same at tier 2 | `enemy_common.c` |
| `$30`-`$35` | `0C:9CF5` `enemy_common_tier4` | The same at tier 4 | `enemy_common.c` |
| `$2C`, `$2D`, `$2F` | `0C:9D9E`, `9DA4`, `9DAA` `entrance_charge` | Andore charging in, tiers 1, 2, 4 (never executed) | `enemy_entrances.c` |
| `$2E` | `0C:9D71` `knife_fly` | El Gado's knife | `enemy_common.c` |
| `$36`-`$3E` | `0C:9E20`-`9E44`, then `entrance_drop` (`$9E48`) | Poison, AxL, El Gado dropping in, tiers 1, 2, 4 each | `enemy_entrances.c` |
| `$3F`, `$4F`-`$5F` | `0C:9EC5`/`9EC9`/`9ECD` (tier 1/2/4), then `entrance_stand` (`$9ECF`) | Standing first | `enemy_entrances.c` |
| `$40`-`$4B` | `0C:9EF4`-`9F12`, then `entrance_sit` (`$9F19`) | Two P and Bred sitting in wait | `enemy_entrances.c` |
| `$4C`-`$4E` | `0C:84EF` | El Gado by the entrance tables, but nothing sets them up | |
| `$60` | `05:A730` | Nothing; no list entry or code makes it | `obj_props.c` |
| `$61` | `05:A731` `hidden_food` | Hidden food (round 5 area 2) | `obj_props.c` |
| `$62` | `05:AD58` `floor_indicator` | The elevator's floor indicator (B1-B4) | `obj_props.c` |
| `$63` | `05:AD37` `shaft_light` | The lights that slide up the elevator shaft | `obj_props.c` |
| `$64` | `05:ABE1` `elevator` | The elevator of round 4 area 2 | `obj_props.c` |
| `$65` | `05:ABC4` `boss_low_palette` | Round 1 area 3: animates background palette 2 once Thrasher is under `$10` of life | `obj_props.c` |
| `$66`, `$67` | `05:A916` `dropped_weapon` | The weapon a hit knocks out of the player's hand (`0F:FAFB`; `$67` in slot 1, `00:93E6`) | `obj_weapons.c` |
| `$68` | `05:AB08` `thrasher_look_in` | Thrasher looking in at the end of round 1 area 2 | `obj_props.c` |
| `$69`, `$6A` | `05:A7A5` `weapon_pickup` | A weapon on the floor, taken with B; `$6A` is Guy's put back, with his throws left | `obj_weapons.c` |
| `$6B` | `05:AB41` `andore_charge` | Andore's charge in the exit scene of round 1 area 2 | `obj_props.c` |
| `$6C`, `$6D` | `05:A8A8` `thrown_weapon` | A thrown weapon: Cody's knife (`$6C`), Guy's (`$6D`) | `obj_weapons.c` |
| `$6E` | `05:A9E4` `boss_talk` | The boss talk (below) | `obj_props.c` |
| `$6F` | `05:AA82` `go_sign` | The GO sign | `obj_props.c` |
| `$70`, `$71` | `0B:B331` `bonus_stage_controller` | The bonus stage's controller, slot `$0F`; nothing makes `$71` | `enemy_drums.c` |
| `$72` | `0B:B5D7` `bonus_drum` | A drum of the bonus stage | `enemy_drums.c` |
| `$74` | `0B:B696` `belger` | Belger, round 5's boss | `boss_belger.c` |
| `$7B`, `$7D` | `0B:BD7F` `belger_end_scene` | The scene after Belger, from his slot | `boss_belger_end.c` |
| `$73`, `$75`-`$7A`, `$7C`, `$7E`, `$7F` | `0B:B330` | Nothing | `enemy_tables.c` |
| `$80` | `01:A0FD` `thrasher_update` | Thrasher, round 1's boss | `boss_thrasher_sodom.c` |
| `$81` | `01:A101` `sodom_update` | Sodom, round 2's boss | `boss_thrasher_sodom.c` |
| `$83` | `01:AA33` `sodom_rematch_update` | Sodom again: round 4's boss area (area 11) and round 5's second area (13) | `boss_sodom_rematch.c` |
| `$8F` | `01:A09B` `scene_prop_update` | Cody's partner in the scene after Belger, once it has stood in for him (never executed) | `boss_b01_scene.c` |
| `$82`, `$84`-`$8E` | `01:A030` | Nothing | `boss_b01_shared.c` |
| `$90` | `03:B3F4` `abigail` | Abigail, round 3's boss | `boss_abigail.c` |
| `$91` | `03:BB91` `drum_roll` | A drum rolling at the player (round 1 area 2) | `enemy_drums.c` |
| `$94` | `03:BBCB` `drum_dispenser` | The drums' invisible dispenser | `enemy_drums.c` |
| `$95` | `03:BD46` `drum_smashed` | A smashed drum: two halves and, outside the bonus stage, an item | `enemy_drums.c` |
| `$9B` | `03:BF38` `belger_down` | Belger down: waits, then becomes the scene's `$7D` | `boss_belger_end.c` |
| `$9C` | `03:BFBE` `cody_scene_partner` | Cody's part of the scene; becomes `$8F` | `boss_belger_end.c` |
| `$9D` | `03:BF95` `belger_shot` | Belger's shot (kind `$0C`) | `boss_belger.c` |
| `$9E` | `03:BF48` `belger_fall` | Belger's fall | `boss_belger_end.c` |
| `$9F` | `03:BED2` `belger_killed` | Belger's life out (`0C:8211`): knocked down as id 5 with saved id `$9B` | `boss_belger.c` |
| `$92`, `$93`, `$96`-`$9A` | `03:B330` | Nothing | `enemy_tables.c` |

The enemy ids from Data Crystal in `docs/ram_map.md` (Poison `20 26 30`, ...) are
these behaviour ids: `$20-$25`, `$26-$2B` and `$30-$35` are the same six
enemies at tiers 1, 2 and 4.

## The common enemies

Poison, Two P, Bred, AxL, El Gado and Andore (`obj_kind` 0-5) share one
behaviour (`port/src/game/enemy_common.c`). Its bank-`$0C` half runs every
frame (`0C:9CED`, `9CF1`, `9CF5` for tiers 1, 2, 4): it stores the tier and
the kind (`$9D5B` by id), counts the immunity after getting up down, clears
`obj_attack`, stirs the RNG (`rng += sprite 3's x + rng+1 + rng+2 + C`),
gives the frame to `0D:AB29` when the player holds it or a fresh hit makes it
flinch (C set), keeps `obj_y` in the area's walkable band (`0C:9FF6`, which
runs on into bank `$0D`: `$81-$B0`, `$71-$A0` in round 0 area 1 and round 3
area 1, `$91-$B0` in round 1 area 1; not in phases 9 and `$F`, which are in
the air) and jumps through its phase table (`$9D3B`/`$9D4B`): the AI of bank
`$0D` (`port/src/game/enemy_ai.c` lists the phases: 0 `enemy_start`, 1
`enemy_approach`, 2 `enemy_maneuver`, 3 `enemy_block` (AxL), 4
`enemy_close_in`, 5 `enemy_attack`, 6 `enemy_jump_start`, 7 `enemy_retreat`,
8 `enemy_hit_stun`, 9 `enemy_jump`, `$A` `enemy_knife_throw` (El Gado), `$B`
`enemy_slide`, `$C` `enemy_recover`, `$D` `enemy_idle`, `$E` `enemy_charge`
(Andore; `obj_timer` 3 is the rush), `$F` `enemy_drop_in`). `obj_tier`
1/2/4 becomes 0/1/2 through `$A897` (bank `$0D`) for the damage indexes and
AxL's block odds.

Not every kind uses every phase. In the tour logs Two P and Bred only show
0-2, 4, 5, 7 and 8; AxL adds 3 (his block), `$D` and `$F`; Poison 6, 9, `$C`
and `$F` (her back flip); El Gado 6, 9-`$D` and `$F` (jump, knife, slide);
Andore `$D` and `$E` (the charge). Phases 1, 2, 4 and 7 end in `enemy_walk`
(`0D:A953`), which sends an enemy that meets a pit back to phase 1 with
`obj_pit_flag`; nearly every phase ends in `enemy_reset` (`0D:A8E8`). The
walking speed is the kind's (`enemy_set_speed`, `0D:AC34`, by
`$ACA7/$ACAD`): Bred 0.8 px a frame, AxL and Andore 0.9, Poison 1.2, Two P
and El Gado 1.4, split into x and y by the 16-step direction index. An
attack starts when the player is within the kind's
`enemy_attack_range_x/y` (`0D:A3A7/A3AD`, through `0F:FB3E`); the hit
itself lands by `obj_attack`'s reach box (`0F:F6AA-F6EA`, `obj_reach.c`).

Id `$2E` is El Gado's knife (`0C:9D71`): `0D:A5DA`, from his phase `$A`,
puts it `$12` px ahead of him and 3 px lower with `obj_vx` `$0330`, attack
`$87` and damage index 6, `obj_kind` 6. It flies on `obj_dir` until its
screen x reaches `$FA` (right) or falls under 8 (left), then it is cleared.
The player can knock it out of the air (kind 6 has knocked-down animations
`$BA`/`$B9` and lands `$0A` lower).

The ids from `$36` up, and `$2C`, `$2D`, `$2F`, are entrances
(`port/src/game/enemy_entrances.c`): they set the enemy up and turn it into
its plain id (`enemy_become_common`, `0C:9FBB`, `$9FCC` by id) in the phase
it starts in: `entrance_charge`, `entrance_drop`, `entrance_stand`,
`entrance_sit`.

| Ids | Entry | Entrance |
|---|---|---|
| `$2C` `$2D` `$2F` | `0C:9D9E/9DA4/9DAA` | Andore charging in: once the player is within `$80` px he starts at the rush of his charge (phase `$E`, `obj_timer` 3). Never executed; no spawn table lists these ids |
| `$36`-`$38`, `$39`-`$3B`, `$3C`-`$3E` | `0C:9E20`-`9E44` | Poison, AxL, El Gado drop onto the player: spawned hidden (stage types `$30`-`$35`, `$5A`-`$5C`, flags `$D4`), they wait until he is within 8 px in x (at once in round 3 area 1), then fall from above onto his ground line (`obj_vy` `$FF00`) with the kind's attack out (phase `$F`) |
| `$3F`, `$4F`-`$5F` | `0C:9EC5/9EC9/9ECD` | Stand `$78` frames (phase `$D`) before the first move |
| `$40`-`$4B` | `0C:9EF4`-`9F12` | Two P and Bred sit in wait, in one of two poses (animation `$A2`, `obj_timer3` 0/2), unhittable (`obj_invuln` `$FF`) until the player is within `$32` px in x and y or they are less than `$40` px from the screen's left edge |
| `$4C`-`$4E` | `0C:84EF` (RTS) | El Gado by the entrance tables, but nothing sets them up: an object with these ids would stand still |

The timed spawner adds `$37`, `$3A` and `$3D` (`08:9456`). In the routes'
logs the waiting ids `$38`, `$3E`, `$43`, `$49`, `$4A`, `$4B` show up (rounds
0-4); `$36`, `$39`, `$3A`, `$3C`, the tier-2 standing entry and `$41`/`$44`
never ran (`runs/cdl`).

## Bosses

While the boss talk sits in slot `$0D` every boss stands still and cannot be
hit. The boss talk, id `$6E` (`05:A9E4`), is an invisible object the stage
list puts at the start of each boss area (type `$18`): once the camera stops
and the player is free it holds him (`game_action` `$0A`) and starts the talk
pages, which `gameplay_special_screen` runs instead of the game
(`flow_talk.c`). Thrasher's first page (3, BOW DOWN BEFORE ME!) is a
question: the right answer, NO, costs him 8 of life (`04:9A74`,
`talk_answer`).

**Thrasher and Sodom** (`$80`, `$81`, bank `$01`, `boss_thrasher_sodom.c`)
share one behaviour; the kind (7 or 8) picks the animations, the swing and a
few decisions. Each frame (`01:A103`): `obj_kind`, no attack out,
`obj_invuln` counted down, the talk wait, `boss_check` (`01:A6D6`: held by
the player, hit, the drop-in), then the phase through `$A6C4/$A6CD`:

| Phase | Routine | What |
|---|---|---|
| 0 | `$A13C` | Enter: circle if on screen 1, else walk in (8) |
| 1 | `$A169` | Circle the arena's edges, turning at the corners; attack once the player is in reach (Thrasher `$1C` x `$0A`, Sodom `$24` x 9); at the corners Thrasher may jump, Sodom rush |
| 2 | `$A2ED` | Swing: one to three (Thrasher), one (Sodom) |
| 3 | `$A417` | Wander around the player for `$78` frames (Thrasher) |
| 4 | `$A473` | Rush: aim, run, slide to a stop (Sodom) |
| 5 | `$A557` | Jump twice on the spot; the landings hit (Thrasher) |
| 6 | `$A5CF` | Leap forward to the arena's side (Thrasher) |
| 7 | `$A633` | Hurt |
| 8 | `$A651` | Walk in |

A swing that hits leaves the player at most `$22` frames of invincibility
instead of `$3C` (`01:A3E1`). A new life dropping in (`game_action` `$25`)
knocks any bank-`$01` boss down (`01:A794`); a player lying on the ground
sends Thrasher and the round 2 Sodom back to phase 0 unless they circle or
jump (`01:A766`).

**Sodom again** (`$83`, `boss_sodom_rematch.c`; type `$16`, life `$68`) is
round 4's boss and stands in round 5's second area. His tier follows his
life: 1 from `$30` up, 2 below, 3 once enraged (phase 0 enrages a tier-2
Sodom: `$7C` frames unhittable and a `$74`-frame show). Phases through
`$AAC6/$AACF`: 0 decide (`$AAD8`), 1 approach (`$AB0A`), 2 the swing
(`$A2ED`), 3 the enraged charges (`$AC4E`), 4 the rush (`$A473`), 5 back off
(`$ABB6`), 6 wait then rush or approach (`$ADBA`), 7 hurt (`$AD8C`), 8 a
follow-up blow (`$AE19`, set only by never-executed code). Phase 6's
distance ladder is broken: the `BCS` at `01:ADD8` jumps over the `$3C` and
`$50` rungs, so from `$28` to `$63` px the rush threshold is `$0C` and
closer than `$28` it is 0 (always an approach); `$ADDE-$ADED` never run. No
P4 route reaches him; the coverage sessions did (`cyc_seeds.txt`
`4k:02:AA33`), while `runs/cdl/map.cdl` has none of his code.

**Abigail** (`$90`, `boss_abigail.c`): each frame (`03:B3F4`) counts
`obj_invuln` and `obj_tier` (a grab cooldown) down; then, by priority, stands
during the talk, breaks a grab (he slips out of the player's at once),
reacts to a hit (half the time a counter-grab, half a charge), backs off
while the player lies down, walks back when off the player's screen, else the
phase (`$B536/$B53E`): 0 start (`$B546`), 1 approach at 1 px a frame
(`$B57B`), 2 back off (`$B666`), 3 one or three punches (`$B6F5`, the last of
three strong), 4 the charge (`$B7C9`: wind-up, a 2.3 px a frame rush that
slows down), 5 the grab (`$B879`: three blows and a throw, unbreakable), 6
decide (`$BA1C`), 7 the uppercut against a jumping player (`$BA88`).

**Belger** (`$74`, `boss_belger.c`) fights in two forms, `obj_tier` 0 and 1,
each with its phase table:

| Phase | First form (`$BCE6/$BCF7`) | Second form (`$BCEC/$BCFD`) |
|---|---|---|
| 0 | `$B80B` start | `$B929` start: to his page |
| 1 | `$BB8C` hurt | `$BB8C` hurt |
| 2 | `$BBAC` back to his page (never executed) | the same |
| 3 | `$B825` approach | `$B958` turn away |
| 4 | `$B86F` circle the player | `$B97B` patrol up and down, shoot |
| 5 | `$B8D3` punch | `$BA68` walk to the screen's edge |
| 6 | | `$BA9A` fire a shot (id `$9D`, `0B:BBEC`) |
| 7 | | `$BAD0` rest `$5A` frames |
| 8 | | `$BAE8` dizzy (never executed) |
| 9 | | `$BB05` punch, then leap |
| 10 | | `$BB53` the leap |

The change comes when his life drops under `$5A`: music `$F2`, then, once
the player stands, talk page `$3C` and `obj_tier` 1 (music 6 from then on).
The tours only get there through the bots' life patch (`cody_tour` frame
28298). His life running out makes him `$9F` (`0C:8211`), knocked down as id
5 with saved id `$9B`.

**The scene after Belger** (`boss_belger_end.c`, never played by a route):
`$9B` waits and becomes `$7D`, the scene's controller in Belger's slot. Every
frame it sets `special_screen_id` `$F9`, which `hud_update` (`0F:E6B4`) takes
back to 0 instead of drawing the status bar, and drives the player through
`player_state` with forced pad bits. Its phases: 0 the player jumps, Belger's
fall (`$9E`); 1 the jump goes on until the player lands; 2 the round-clear
screen, music `$0F`; 6 a wait; 3 by character: Cody's partner scene (`$9C`,
then `$8F`), Guy walks off; either way the player then becomes Haggar
(`player_id` 2, `player_level` two less: `DEC DEC` at `0B:BE79`) and walks to
the meeting point, and the controller becomes `$7B`; 4 a figure enters from
the right (animation `$23`, sprite bank `$36`); 5 it walks left to x `$B0`; 7
a last wait, then `game_action` `$0E` ends the round with `player_id` back.
`$7B` and `$7D` differ only in the sprite data `object_draw` maps
(`$EB93`: `$0A` and `$02`).

## Stage objects (ids `$60`-`$6F`, bank `$05`)

`obj_props.c` and `obj_weapons.c`; the stage list's per-type tables
(`08:8442`, `08:85E6`) place most of them.

- **Hidden food** (`$61`): list type `$1C`, round 5 area 2, hidden (flags
  `$D4`), animation `$61`; the player's first punch thrown at it from
  `$11`-`$17` px to its right (facing it) gives sound `$22` and health
  `$FCC0[1]`, up to the level's full health, and frees it. Alive in every
  tour through that area, never punched.
- **The elevator** (`$64`, round 4 area 2, list type `$1F`): shakes the
  screen, cycles background palettes 2-3 and makes the floor indicator
  (`$62`, B1-B4, chime `$30`) and the lights that slide up the shaft
  (`$63`).
- **Thrasher's low-life palette** (`$65`, round 1 area 3, type `$1B`): once
  slot `$0C`'s Thrasher is under `$10` of life, background palette 2 runs
  color script 7. No route gets him that low.
- **Thrasher looking in** (`$68`, end of round 1 area 2, type `$19`) and
  **Andore's charge** (`$6B`, made in slot `$0D` by `00:9297` in the exit
  scene of round 1 area 2): no route plays either.
- **The boss talk** (`$6E`, above) and **the GO sign** (`$6F`, put up by
  `stage_events` at `08:8987`, chime `$31`).
- **Weapons**: `$69` lies on the floor (list types `$0A`-`$0C`, the same
  entry showing each character his own: Cody's knife, Guy's, Haggar's pipe)
  and is taken with B (`weapon_held` `$FF`; Guy gets 7 throws in
  `weapon_ammo`). Guy taking another puts his back down as a `$6A` carrying
  his throws in `obj_timer`. `$66`/`$67` is the weapon a hit knocks out of
  the player's hand (Guy keeps his): it flies, lands, and may lie there as a
  `$69` again, or blinks and goes. `$6C`/`$6D` are thrown weapons, flying
  until they leave the screen; their speed fraction is meant to be `$19` but
  the next store writes `player_id` over it (`05:A8AF-A8B6`), so it is 0, 1
  or 2.

## Drums and the bonus stage

Round 1's second area rolls drums at the player (`$91`, 1.2 px a frame,
attack `$9E`, damage `$3F`), sent by an invisible dispenser (`$94`). The
bonus stage after the bosses of rounds 2 and 4 starts in the player's
end-of-area step (`00:94F7`): `bonus_stage` (`$B2`) `$FF`, special screen
`$FD` (BONUS STAGE) and the controller `$70` in slot `$0F`, which rolls twenty
drums (`$72`) and pays for the ones smashed (`bonus_drum_count`, `$B9`: 8 or
more win a prize, all `$14` add PERFECT). Any object with animation `$7B`
whose life runs out becomes `$95` (`0C:821F`): two halves flying off and,
outside the bonus stage, an item. While `bonus_stage` is set the A+B special
is off (`0E:CA40`, `0E:D305`). No route plays the bonus stage (the bots skip
those areas).

## Names fixed

- `$0490` and `$04A0` were named the other way round until 2026-10-16.
  Damage is subtracted from `$0490` (`0C:86E4`), its going negative is the
  death test (`0C:80CB`, `8111`, `8134`, `9BA5`), the HUD fills the bar from
  it and sizes it from `$04A0 / 8` (`0F:E8E0`, `E8F2`), and in
  `base_cody_round1.flog` slot `$0C`'s `$0490` goes `28 26 1E 16 0E 06 FE`
  over five hits while `$04A0` stays `28`. So `$0490` is `obj_life` (Data
  Crystal's "enemy life `$049B/$049E`") and `$04A0` `obj_life_max`. Until
  then the bots' "finish a long fight" patch wrote the maximum, which only
  shortened the HUD bar (Belger in `base_cody_tour.flog`: `$04AC` from `$68`
  to 1 at frame 40414 while `$049C` stayed `$59`); it now writes `$0490`
  (`port/scaffold/bot.c`, `tools/bot.py`). The early note that Abigail "took
  no damage" was watching the maximum too.
- The comments on `obj_flags` ("bit 4 = drawn") and `object_draw` ("if obj_flags
  bit 4") read bit 4 as visibility. It selects world versus screen
  coordinates; slots with bit 4 clear are drawn too.
