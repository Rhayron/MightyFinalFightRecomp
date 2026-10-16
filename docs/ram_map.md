# Mighty Final Fight (USA) — RAM map

Source: [Data Crystal](https://datacrystal.tcrf.net/wiki/Mighty_Final_Fight/RAM_map)
(stub page, transcribed 2026-10-15), plus what this project read in the code
and checked against the running game. Every named address is a `[[ram]]` entry
in `disasm/labels.toml`, whose comment cites the code. **Verified** = checked
in a headless run or in the bot frame logs (`runs/p4/base_*_tour.flog`,
`base_*_round1.flog`); **Code** = read from the code only; **Guess** = said
so; Data Crystal = their word only.

## Player and game

| Address | Name | Meaning | Status |
|---|---|---|---|
| `$0030` | `game_action` | Game action (table below) | Data Crystal; `$0E` verified |
| `$0031` | `player_dir` | Player facing, 1 right / 2 left | **Verified** — 1 while walking right, 2 left |
| `$0032` | `jump_dir` | Horizontal direction of the current jump (pad bits) | Code |
| `$0034` | `player_id` | Player id: Cody `00`, Guy `01`, Haggar `02` | **Verified** (each pick from `states/title_4500`) |
| `$0035` | `jump_moving` | The jump started with left or right held | Code |
| `$0036` | `player_ground_y` | Player's ground line (landing line, depth for hit tests) | Code |
| `$0037` | `action_timer` | Frames left in the current game action; also the CAPCOM PRESENTS wait | **Verified** |
| `$0038` | `grab_timer` | Hold time of a grab (`$3C`, Haggar `$5A`) | Code |
| `$0039` | `grab_slot` | Slot of the enemy the player holds | Code |
| `$003A` | `health` | Health (max 104, depends on experience) | **Verified** — `$40` at round 1 start, drops when hit |
| `$003B` | `player_invuln` | Frames the player ignores enemy attacks after a hit (60) | **Verified** — `$3C` on every one of 607 hits. Data Crystal's "Invincible" |
| `$003E` | `lives` | Lives | **Verified** — `05` at start, HUD "PL x5" |
| `$0051` | `level_saved` | Level at round start, restored on continue | Code |
| `$0069` / `$006A` | `respawn_timer2` / `respawn_timer1` | Drop-in invincibility after a lost life (`$1E` then `$B6` frames) | **Verified** |
| `$006B` | `player_level` | Level, 0-based, capped at 5 (Cody, Guy) or 3 (Haggar). Data Crystal's "0-2" is wrong | Code — always 0 in the bot logs |
| `$006C/$006D` | `player_exp` | Experience, 3 BCD digits (`$6D` hundreds, `$6C` tens/ones), not low/high | **Verified** |
| `$006E` | `player_no_hit` | Bit 7 while the player's own grab, throw or special runs: enemy attacks are not tested | Code |
| `$006F` | `player_stand_anim` | Animation the player returns to when idle | Code |
| `$00A3` | `combo_timer` | `$1E` when a hit lands; B inside the window continues the combo | **Verified** |
| `$00A4` | `attack_live` | `$1E` while the player's attack can hit | Code |
| `$00A6` | `attack_ground_y` | Ground line of a jump attack or special | Code |
| `$00A7` | `attack_id` | Current attack: indexes hit cooldown, reach, damage, exp and knockback tables | Code |
| `$00A8` | `combo_target` | Enemy hit by attack 6 or `$0C`, which the combo finisher can grab | **Verified** — slots `$0C`/`$0D` |
| `$00A9` | `weapon_held` | Weapon: `00` dropped, `FF` held | Code (agrees with Data Crystal) |
| `$00AD` | `weapon_ammo` | Guy's throws left with a picked-up weapon (7) | Code |
| `$00AF` | `credits` | 3 at a new game | **Verified** |
| `$00B3` | `guy_jump_window` | Guy only: 5-frame window after Up in a jump | Guess |
| `$00BA` | `grab_hit_count` | Hits delivered during a grab (ends at 3) | Code |
| `$00BD` | `select_player` | Character the select screen starts on | Code |
| `$00BE` | `action_before_fall` | `game_action` saved when falling into a pit | Code |
| `$00BF` | `special_hit` | The special move connected, so it costs health | Code |
| `$0150` | `continue_count` | Continue screens seen; the full ending needs 0 | **Verified** — 1 after the continue screen |
| `$0151` | `saved_attack_type` | `$A7` parked by the hit reactions | Code |

## Rounds, areas, camera and spawning

| Address | Name | Meaning | Status |
|---|---|---|---|
| `$0022` | `round_tables` | Round index for the per-round tables; follows `$0061` (changes a few hundred frames after it at round ends) | **Verified** (bot runs) |
| `$0023` | `scroll_mode` | Direction of the last streamed column (1 right, 2 left); picks `stage_spawns`'s mode | Code |
| `$0024` | `bg_column` | Next nametable column to stream (0-`$1F`) | Code |
| `$0025` | `scroll_x_prev` | `scroll_x` after the camera update (column streaming) | Code |
| `$0027` | `player_x_prev` | Player x after the camera update (the camera follows the difference) | Code |
| `$0028` | `map_cell` | Index into the 8x8 metatile map (row * 8 + column) | Code |
| `$0029` | `bg_screen` | Screen being streamed | Code |
| `$002A` | `area_flags` | Top 3 bits of the area header; bit 5 = the area scrolls | **Verified** |
| `$002B` | `area_palette` | Palette pair of the area header (always 0) | Code |
| `$002C` | `area_screen_count` | Screens in the area; the camera stops there | Code |
| `$002D` | `area_screen` | Screens scrolled in the area (x high byte of the camera) | **Verified** — equals `scroll_x_hi` in 98.6% of frames |
| `$002E` | `camera_dir` | 1/2 while the camera follows right/left; never read | Code |
| `$003C` | `spawn_slot` | Slot picked by the timed spawner | Code |
| `$003F` | `area_clear` | 0 fighting, `$FF` kill quota met, `$7F` GO sign up; negative stops the timed spawner | Code |
| `$0050` | `camera_lock` | `$FF` while the camera holds a new screen; the timed spawner runs only then | **Verified** |
| `$0054` | `area_kills` | Enemies defeated in the area, against the area script's quota | **Verified** — counts up and resets per area |
| `$005C` | `boss_area` | `$FF` in a round's last area (sub_area 2) | **Verified** |
| `$0060` | `sub_area` | Area within the round | **Verified** |
| `$0061` | `round` | Round, 0-based (`00` Slum … `04` Bay Area, `05` ending). Advances with the HUD's RD in every bot run. Patching it does not select a round (on the select screen it only moves the map marker; round 1 still loads) | **Verified** |
| `$0062` | `special_screen_id` | Nonzero: the boss talk/cutscene screen runs instead of gameplay (text page or `$F0`-`$FF` command) | **Verified** |
| `$0063` | `spawn_timer` | Frames to the next timed spawn. Data Crystal's "enemy timer" | Code |
| `$0066` | `special_screen_step` | Phase of the special screen | Code |
| `$0067` | `area_index` | `round * 3 + sub_area`; picks the stage object list | **Verified** |
| `$0072`-`$007B` | `spawn_list_x`, `_x_hi`, `_y`, `_y_hi`, `_type` | Pointers into the area's stage object list | Code |
| `$009E` | `spawn_list_next` | Next stage-list entry to spawn. Data Crystal's "total enemies" | Code |
| `$009F` | `spawn_list_prev` | First stage-list entry still ahead of the screen | Code |
| `$00AA` / `$00AB` | `text_cursor` / `text_timer` | Special screen text position and delay | Code |
| `$00B0` | `hud_enemy_slot` | Slot whose life bar the HUD shows (bosses first) | **Verified** — follows the enemy being hit |
| `$00B1` | `hud_enemy_timer` | Frames that bar has been shown (hidden at `$96`) | Code |
| `$00B4` | `dialogue_choice` | Special-screen two-way cursor (bit 7) | Code |
| `$00B5` | `screen_event` | Triggers the per-screen setup in `stage_events` | Code |
| `$00B7` | `game_finished` | `$FF` after the last round | Code |
| `$00B8` | `intro_page` | Story page 0-4 | Code |
| `$00BB` | `camera_autoscroll` | After the lock lifts, the camera scrolls itself to the next screen | **Verified** |
| `$00BC` | `keep_on_screen` | Knocked-back enemies held `$10` px inside the screen | **Verified** |
| `$05A0`-`$05A2` | `spawn_groups` | Timed-spawner scratch | Code |

## Frame services, banks, buffers

| Address | Name | Meaning | Status |
|---|---|---|---|
| `$0018` | `palette_request` | Nmi uploads `palette_buf` | Code |
| `$0019` | `vram_list_request` | Nmi writes the strip list at `$0780` in rows | Code |
| `$001A` | `vram_column_request` | Same list in columns (+32): stage column streaming | Code |
| `$0020/$0021` | `map_screen_ptr` | Current screen's 64-byte metatile map | Code |
| `$002F` | `probe_saved_bank` | `$A000` bank saved by the background probes | Code |
| `$0040`-`$0044` | `probe_index`, `probe_max`, `probe_hits` | Background probe state and results (tile bit `$40` = pit) | Code |
| `$0064` | `vram_request` | `nmi_vram_update` runs at the next nmi | Code |
| `$0065` | `vram_buffer_len` | End of the count-prefixed list at `$0780` | Code |
| `$007C`-`$007E` | `irq_scroll` | Scroll latched by the nmi for the status-bar split (only `$7D` is read) | Code |
| `$0080`-`$008F` | `tasks` | Four cooperative tasks (`docs/engine.md`) | Code |
| `$0090` | `sched_nmi_flag` | Set by the nmi; the scheduler scans again if it was set during a scan | Code |
| `$0091` / `$0092` | `task_current` / `frame_counter` | Running task / nmi count | Code |
| `$0093/$0094` | `task_entry` | Entry pointer for `task_spawn` and the scheduler | Code |
| `$0095` / `$0096` | `draw_parity` / `draw_slot` | `objects_draw` order bit / loop slot | Code |
| `$0097` | `oam_index` | Next free OAM byte (4 = empty, 0 = full) | **Verified** |
| `$0098` | `paused` | START toggles it 0/1; `$FF` while a scene freezes play | Code |
| `$0099` | `gravity` | `$38`, set by `objects_update` every frame | **Verified** |
| `$009A` | `draw_busy` | Holds the nmi while `0F:F406` redraws the objects | Code |
| `$009B` | `irq_enable` | 1 = MMC3 IRQ on; 0 during each round map | **Verified** |
| `$009C/$009D` | `irq_handler` | Raster IRQ handler | Code |
| `$00A0/$00A1` | `nmi_return_pc` | PC the nmi interrupted | Code |
| `$00D9` | `music_current` | Last music command (`sound_queue_push` at `$F34A`) | **Verified** |
| `$00DA`-`$00E3` | `sound_queue_write`, `sound_queue_read`, `sound_queue` | Sound command ring | Code |
| `$00E4`-`$00E7` | `rng` | Random state | Code |
| `$00E8`-`$00ED` | `chr_banks` | MMC3 CHR shadows | Code |
| `$00EE` | `render_off_depth` | `rendering_off/on` count | Code |
| `$00EF` | `update_slot` | `objects_update` loop slot | Code |
| `$00F0` | `mmc3_select` | Last `$8000` write, restored by nmi and IRQ | **Verified** |
| `$00F2/$00F3` | `prg_banks` | Banks mapped at `$8000`/`$A000` | **Verified** |
| `$00F4/$00F5` | `bank_request` | Banks wanted at `$8000`/`$A000` | Code |
| `$00F6` / `$00F7` | `bank_switch_busy` / `sound_deferred` | `bank_switch` guard / deferred sound update | Code |
| `$00F9` / `$00FC` | `scroll_x_hi` / `scroll_x` | Horizontal scroll | **Verified** |
| `$00FA` / `$00FB` | `scroll_y` / `scroll_y_hi` | Vertical scroll (screen shake only) / page (always 0) | **Verified** |
| `$00FD` | `scroll_nt` | Nametable bit for PPUCTRL (always 0) | Code |
| `$00FE` / `$00FF` | `ppumask_shadow` / `ppuctrl_shadow` | PPU register shadows | Code |
| `$0100`-`$010F` | `bg_pal_anim_on`, `_step`, `_script`, `_timer` | Background palette cycling, 4 palettes (the stack never reaches below `$0194`) | **Verified** |
| `$0200`-`$02FF` | `oam_buffer` | Sprite buffer for OAM DMA | Code |
| `$0600`-`$061F` | `palette_buf` | Palette uploaded at `$3F00` | Code |
| `$0620`-`$063F` | `palette_base` | Palette as loaded; the fades darken `palette_buf` from it | Code |
| `$0640`-`$0667` | `attr_shadow` | Attribute rows 1-5 of the nametable | **Verified** extent |
| `$06C0`-`$06CF` | `tile_block` | 4x4 tiles of the block being streamed | **Verified** extent |
| `$0780`-`$07FF` | (unnamed) | VRAM update buffer: count-prefixed entries for `nmi_vram_update` (`$64`/`$65`), or the strip list for `$19`/`$1A`. `$0780`-`$0783` overlap the range reserved for the sound RAM; the sound code only touches `$077C`-`$077F` | Code |

Never accessed and always 0 in the logs: `$1C`-`$1F`, `$26`, `$3D`, `$45`-`$4F`,
`$52`, `$53`, `$55`-`$59`, `$5B`, `$5D`-`$5F`, `$68`, `$70`, `$71`, `$7F`,
`$A2`, `$AE`, `$F1`, `$0152`-`$0193`, `$0668`-`$06BF`, `$06D0`-`$06FF`, and the
object tables `$0350`, `$0370`, `$0390`, `$03B0`. Written but never read: `$1B`,
`$33`, `$5A`, `$A5`, `$B6`. `$F8` is never written; the nmi copies its 0 to
`$7C`. `$00`-`$13` are scratch (pointers and temporaries of single routines).
Unclear, left unnamed: `$AC` and `$B9` (special-screen and boss counters, never
nonzero in the logs), `$B2` (set only at `00:94F7`, never in the logs).

## Object slots

Sixteen slots, every field a 16-byte table indexed by slot `i`; the whole
system (spawn, update, hits, draw) is in `docs/objects.md`. Data Crystal lists
single addresses (`$033C` enemy id, `$036C/D` enemy x, `$049B/E` life); they
are entries of these tables.

| Table | Name | Meaning | Status |
|---|---|---|---|
| `$0300+i` | `obj_state` | Bit 7 = live; bits 0-3 = phase of the behaviour's state machine (`$80`-`$8E` seen) | **Verified** |
| `$0310+i` | `obj_saved_state` | State to resume after a grab or a knockdown | Code |
| `$0320+i` | `obj_id` | Behaviour id (ids below); changes while the object lives (5 when knocked down) | **Verified** — `$21` Two P, `$22` Bred |
| `$0330+i` | `obj_saved_id` | Id to resume (Data Crystal's `$033C` enemy id is slot `$0C`) | **Verified** — an enemy knocked down as id 5 comes back as its saved id |
| `$0340+i` | `obj_x_frac` | x sub-pixel | Code |
| `$0350+i` | - | Never accessed; always 0 | **Verified** |
| `$0360+i` | `obj_x` | World x, low byte | **Verified** |
| `$0370+i` | - | Never accessed; always 0 | **Verified** |
| `$0380+i` | `obj_x_hi` | World x, high byte | **Verified** |
| `$0390+i` | - | Never accessed; always 0 | **Verified** |
| `$03A0+i` | `obj_y_frac` | y sub-pixel | Code |
| `$03B0+i` | - | Never accessed; always 0 | **Verified** |
| `$03C0+i` | `obj_y` | y (larger = lower); includes the height while in the air | **Verified** |
| `$03D0+i` | `obj_pit_flag` | Common-enemy pit avoidance (1/2), nonzero during a pit fall | **Verified** — nonzero only in rounds 2-3 (0-based) |
| `$03E0+i` | `obj_y_hi` | Vertical page of `obj_y` | Code — always 0 in the logs |
| `$03F0+i` | `obj_held` | Nonzero while the player holds it (`$3C`/`$5A`), `$FF` on release | **Verified** |
| `$0400+i` / `$0410+i` | `obj_vx_frac` / `obj_vx` | x speed, unsigned; direction from `obj_dir` | Code |
| `$0420+i` / `$0430+i` | `obj_vy_frac` / `obj_vy` | y speed, signed, positive = up; gravity `$99` | Code |
| `$0440+i` | `obj_attack` | Bit 7 = attack out; bits 0-4 = reach box | Code |
| `$0450+i` | `obj_attack_damage` | Index into the player damage tables `$FD16`/`$FD56` | Code |
| `$0460+i` | `obj_dir` | Pad bits: 1 right, 2 left, 4 down, 8 up | **Verified** — 1, 2, 4, 8, 5, 6, 9, `$0A` seen |
| `$0470+i` | `obj_throw_flag` | `$FF` when thrown with the pad held forward | Code — never set in the logs |
| `$0480+i` | `obj_spawn_index` | Stage-list entry the slot came from, `$FF` none | **Verified** |
| `$0490+i` | `obj_life_max` (wrong name) | **Current** life; negative = dead. Data Crystal's `$049B/E` | **Verified** — slot `$0C` in `base_cody_round1.flog`: `28 26 1E 16 0E 06 FE` over the hits |
| `$04A0+i` | `obj_life` (wrong name) | **Maximum** life: written once at spawn, sizes the HUD bar | **Verified** — stays `$28` while `$0490` falls. The bots' patch of this byte does not end fights |
| `$04B0+i` | `obj_hit_cooldown` | Frames before the slot can be hit again (`$FB17[attack]`) | **Verified** — 2 or `$10` after a hit, -1 per frame |
| `$04C0+i` | `obj_invuln` | Nonzero = cannot be hit; `$FF`/`$FE`/`$FD` = knockdown stages | **Verified** |
| `$04D0+i` | `obj_hit_player` | `$FF` when its attack hit the player | Code |
| `$04E0+i` | `obj_hit_timer` | `$1E` when a player hit lands, counted down | **Verified** |
| `$04F0+i` | `obj_spawn_group` | Per-kind code 3/4/5 (`$FF` non-enemies) for the timed spawner | **Verified** |
| `$0500+i` | `obj_timer` | General per-behaviour timer/step counter | Code |
| `$0510+i` | `obj_timer2` | Second per-behaviour counter | Code |
| `$0520+i` | `obj_ground_y` | Ground line while airborne | Code |
| `$0530+i` | `obj_land_y` | Where a knocked-back object lands | **Verified** |
| `$0540+i` | `obj_timer3` | Third per-behaviour counter | Code |
| `$0550+i` | `obj_down_timer` | Knockdown: `$2D` frames lying, `$17` getting up | **Verified** |
| `$0560+i` | `obj_flags` | 7 on screen, 6 facing right (sprites mirrored), 5 behind background, 4 world coordinates, 2 hidden | **Verified** — the player has bit 6 set with `$31` = 1 (right) in 113,782 of 115,225 frames |
| `$0570+i` | `obj_anim_frame` | Frame index in the animation script | Code |
| `$0580+i` | `obj_anim` | Animation id, 0 = not drawn | Code |
| `$0590+i` | `obj_anim_timer` | Frames shown of the current frame; bit 7 blink | **Verified** |
| `$05A0`-`$05A2` | `spawn_groups` | Timed-spawner scratch (not a slot table) | Code |
| `$05B0+i` | `obj_kind` | Enemy kind (below) | **Verified** |
| `$05C0+i` | `obj_tier` | Common-enemy tier 1/2/4 | **Verified** — 1 for ids `$20-$25`, 2 for `$26-$2B`, 4 for `$30-$35` |
| `$05D0+i` | `obj_ai_timer` | Countdown of the current AI phase | Code |
| `$05E0+i` | `obj_ai_count` | AI step/attack counter | Code |
| `$05F0+i` | `obj_ai_flag` | AI coin flip | Code |

`obj_kind`: 0 Poison, 1 Two P, 2 Bred, 3 AxL, 4 El Gado, 5 Andore, 6 id `$2E`,
7 Thrasher, 8 Sodom, 9 Abigail, `$0B` Belger, `$0C` id `$9D`.

Slot 0 is the player (`$0360` x, `$03C0` y, as Data Crystal says). Enemies
take slots `$0D` and `$0C` first (both spawners search down from `$0D`), up to
3; `$0E`/`$0F` hold hit sparks; slots `$01`/`$02` hold the dropped weapon and
still display objects around round starts (state 0 most of the time).
`$1C`/`$1D` past `$0320` "mirror the enemy ids with x = 0": those are
`obj_saved_id` of slots `$0C`/`$0D` and the unused `$0370` table.

## Game action values (`$0030`)

| Action | Value |
|---|---|
| Jumping | `01` |
| Punching | `03` |
| Grabbing enemy | `04` |
| Got hit | `05` |
| Falling down | `06` |
| Player selection screen | `07` |
| Continue screen | `09` |
| Cody's throw | `0B` |
| Hitting position | `0C` |
| Skip stage | `0E` — **verified**: writing it into a state ends the current area through the game's own code (round 3 pits → mansion; Sodom → round 3 map) |
| HeadSmasher earthquake | `0F` |
| Haggar's power | `10` |
| Hitting with weapon | `11` |
| Haggar's HeadSmasher | `12` |
| Guy's throw | `13` |
| Cody's slash | `14` |
| Haggar's back throw | `15` |
| Special power | `17` |
| Lying down | `1F` |
| Grapple hit | `21` |
| Cody's power | `23` |
| Falling (invincible) | `25` |
| Guy's power | `27` |
| Picking up weapon | `92` |

## Object ids (`$0320+i`)

These are behaviour ids (`obj_behavior_lo/hi`, `docs/objects.md`). The three
ids of each common enemy are the same enemy at tier 1, 2 and 4 (`obj_tier`);
the low digit gives the kind. Engine ids: 0 animation only, 1 hit spark, 2
pushed back, 3/5 knocked down, 4/8/`$0E`/`$0F` thrown bodies, 6/7/`$16` dying,
`$10`-`$13` items, `$1F` falling into a pit, `$66` dropped weapon.

| Enemy | Ids |
|---|---|
| Poison | `20` `26` `30` |
| Two P | `21` `27` `31` |
| Bred | `22` `28` `32` |
| AxL | `23` `29` `33` |
| El Gado | `24` `2A` `34` |
| Andore | `25` `2B` `35` |
| Thrasher (boss) | `80` |
| Sodom easy / hard | `81` / `83` |
| Abigail | `90` |
| Cyborg Belger | `74` |
| Drums | `72` |
| GO sign | `6F` |
| Enemy killed | `16` |
| Cyborg killed | `9B` |

## Save-state access

`tools/state_ram.py` reads and patches CPU RAM inside a `.cycstate`
(`get`/`set`/`verify`). The RAM offset was measured against `--mem-out` at
nesrecomp `2b4fcdb`; re-run `verify` after bumping the submodule.
