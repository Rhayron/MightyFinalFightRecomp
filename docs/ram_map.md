# Mighty Final Fight (USA) — RAM map

Source: [Data Crystal](https://datacrystal.tcrf.net/wiki/Mighty_Final_Fight/RAM_map)
(stub page, transcribed 2026-10-15), plus what this project verified against
the running game. **Verified** = checked in a headless run (frame and route
given); everything else is Data Crystal's word only.

## Player and game

| Address | Meaning | Status |
|---|---|---|
| `$0030` | Game action (table below) | Data Crystal |
| `$0034` | Player id: Cody `00`, Guy `01`, Haggar `02` | **Verified** (each pick from `states/title_4500`) |
| `$003A` | Health (max 104, depends on experience) | **Verified** — `$40` at round 1 start, drops when hit |
| `$003B` | Invincible | Data Crystal |
| `$003E` | Lives | **Verified** — `05` at start, HUD "PL x5" |
| `$0022` | Stage data; follows `$0061` (changes a few hundred frames after it at round ends) | **Verified** (bot runs) |
| `$0061` | Round, 0-based (`00` Slum … `04` Bay Area, `05` ending). Advances with the HUD's RD in every bot run. Patching it does not select a round (on the select screen it only moves the map marker; round 1 still loads) | **Verified** |
| `$0063` | Enemy timer (next enemy load) | Data Crystal |
| `$006B` | Level (0–2 per Data Crystal) | Data Crystal |
| `$006C/$006D` | Experience, low/high | Data Crystal |
| `$009E` | Total enemies | Data Crystal |
| `$00A9` | Weapon: `00` dropped, `FF` held | Data Crystal |

## Object slots

Data Crystal lists single addresses (`$033C` enemy id, `$036C/D` enemy x, ...);
they are entries of per-slot tables, indexed by slot `i`:

| Table | Meaning | Status |
|---|---|---|
| `$0300+i` | State; bit 7 = live (`$80`/`$84` seen) | **Verified** |
| `$0320+i` | Object id (enemy ids below) | **Verified** — `$21` Two P, `$22` Bred |
| `$0360+i` | X (screen) | **Verified** |
| `$03C0+i` | Y (ground line, larger = lower) | **Verified** |
| `$04A0+i` | Enemy life. Data Crystal says `$049B/E`; the life is one table later | **Verified** — `$28`/`$30` on two thugs (frame 6600), `$58` on Sodom (round 2) with its bar full |

Slot 0 is the player (`$0360` x, `$03C0` y, as Data Crystal says). Enemies
were seen in slots `$0C`–`$0F` (frame 6600 of `inputs/01_cody_s1.txt` from
`states/title_4500`). Slots `$01`/`$02` carry x/y with state 0 (not
characters); `$1C`/`$1D` mirror the enemy ids with x = 0.

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
