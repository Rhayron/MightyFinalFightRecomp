# Graphics and stage data (PLAN_DECOMP.md P5)

What `port/tools/extract_assets.py` pulls out of the user's dump, how the
game stores each thing, and the code that reads it. Addresses are CPU
addresses with the 8 KiB bank in front (`BB:AAAA`); see `docs/engine.md` for
the bank windows. Names are the ones in `disasm/labels.toml` or the
generator's `L<bank>_<addr>` labels.

```
py -3 -I port/tools/extract_assets.py ROM [--out runs/assets] [--sprite-contexts FILE ...]
py -3 -I port/tools/assets_verify.py ROM --state STATE --input ROUTE --first A --last B [--contexts FILE]
```

Everything the extractor writes is ROM-derived and stays under `runs/`
(gitignored); only code and this file are tracked. The tools use the Python
standard library only:

| Module | Job |
|---|---|
| `extract_assets.py` | command line, CRC32 check, writes everything below |
| `assets_nes.py` | ROM and bank access, CHR tiles, the NES colours |
| `assets_png.py` | PNG writer/reader (zlib) |
| `assets_stage.py` | round backgrounds, palettes, palette animations, nametable patches |
| `assets_sprites.py` | animations, frames, layouts; a model of `objects_draw` |
| `assets_verify.py` | runs the oracle with `--frame-log` and checks all of the above |
| `assets_shotcheck.py` | compares an area PNG with an oracle screenshot in RGB |

## Files written

| Path | What |
|---|---|
| `manifest.json` | generator version, ROM CRC32, the list of files |
| `chr/chr_XX.png` | 1 KiB CHR bank `XX` (64 tiles, 8x8 grid), grey levels 0-3 |
| `chr/chr_all.png` | all 8,192 tiles, 32 per row (bank = tile / 64) |
| `palettes/nes.png`, `palettes/palettes.json` | the 64 NES colours and every gameplay palette table |
| `stage/roundR_areaA.png` | the whole playfield of round R, area A (R = RAM `$61`, 0-4; A = RAM `$60`, 0-2) |
| `stage/roundR_areaA.json` | the area record and its maps: metatiles, blocks, tiles, attributes |
| `stage/roundR_areaA_blocks.png` | the area's 16x16 blocks at (b mod 16, b div 16) with the area's CHR and palette |
| `stage/roundR_tiles.json` | the round's metatile and block definitions (the ones its screens use) |
| `stage/patches.json` | the four nametable patches of scripted area entrances and exits |
| `sprites/sprites.json` | object bank table, every animation, frame and layout of both sprite bank pairs |
| `sprites/objXX.png`, `sprites/sheets.json` | with `--sprite-contexts`: each object id's animations, one row each |

Round numbers are the RAM's: round 0 is the game's ROUND 1.

The extractor checks the CRC32 of the whole file (`15EB0BEE`, header
included) and the iNES header (mapper 4, 128 KiB PRG, 128 KiB CHR) before
reading anything. Colours come from `assets_nes.nes_rgb`, a port of the
oracle's NTSC model (`nesrecomp/runner/cyc/hw_palette.c`): on frame 4899 of
`inputs/10_cody_poweron.txt` all 61,440 pixels of the oracle's
`--screenshot` have exactly the RGB that function gives their colour index.

## CHR

128 KiB of CHR ROM, 128 banks of 1 KiB (64 tiles). The game runs MMC3 in
CHR mode 0: the bank select shadow `$F0` is only ever `$06` or `$07`
(`bank_switch` 0F:FF4D, `sound_update` 0F:FF72), and PPUCTRL is `$88`
(written to `ppuctrl_shadow` at 0F:FEB6 by reset): background from pattern
table `$0000`, sprites from `$1000`, 8x8 sprites.

| MMC3 | PPU | Shadow | Written by |
|---|---|---|---|
| R0 (2 KiB, bit 0 ignored) | `$0000-$07FF`, background tiles `$00-$7F` | `$E8` | `nmi_busy` 0E:C0BA, every NMI |
| R1 (2 KiB, bit 0 ignored) | `$0800-$0FFF`, background tiles `$80-$FF` | `$E9` | same |
| R2 | `$1000-$13FF`, sprite tiles `$00-$3F` | `$EA` | `nmi` 0E:C090 (not while `$EE`/`$9A` send the NMI to `nmi_busy`) |
| R3 | `$1400-$17FF`, sprite tiles `$40-$7F` | `$EB` | same |
| R4 | `$1800-$1BFF`, sprite tiles `$80-$BF` | `$EC` | same |
| R5 | `$1C00-$1FFF`, sprite tiles `$C0-$FF` | `$ED` | same |

Background tile `t` is CHR tile `(R0 & $FE) * 64 + t` below `$80` and
`(R1 & $FE) * 64 + t - $80` above; sprite tile `t` is
`R[2 + t / 64] * 64 + t mod 64` (`assets_nes.bg_tile`, `spr_tile`). The
status rows under the playfield switch R0/R1 to `$78`/`$7A` from an IRQ
(0E:C1B1); the status rows above it use the area's own background banks
(the font and bars sit in every background bank, e.g. bank `$40`).

## Palettes (`palettes/palettes.json`)

The game keeps the palette at `$0600-$061F`, uploaded by `nmi` (0E:C061)
when `$18` is set, and a fade target at `$0620-$063F` (`L0E_C362` and
`L0E_C36E` fade toward or away from it in steps of `$10`).

- **Background, per area**: 16 bytes at area record `+$1C` (below), copied
  to `$0600`/`$0620` by `L0E_C494` (`L0E_C4C5`, `L0E_C4F6` for areas 1 and
  2). The palette set byte (`+$11`) would pick another 20-byte set; it is 0
  in all 15 areas.
- **Scripted background palettes**: round 0 area 2 (the first boss room)
  has an all-black palette in its record. Game action `$1C` (00:96EF, set
  at 0F:E104 when that area starts) turns the lights on with 16 bytes from
  00:97F9 (copied at 00:976A and 00:979A) and then 00:9809 (00:97B2), and
  starts palette animation slots itself (`STA $0102` at 00:97CA). Both
  palettes are in `palettes.json` (`assets_stage.SCRIPTED_BG_PALETTES`).
- **Player, sprite palettes 0-1**: 8 bytes per character at 0E:C53E (Cody),
  0E:C546 (Guy), 0E:C54E (Haggar), copied to `$0610`/`$0630` by `L0E_C417`;
  `$0610` is then set to the backdrop `$0600` (`L0E_C527`).
- **Sprite sets: CHR R3-R5 and sprite palettes 2-3**: bank 09 `$A000`,
  called from `L0E_C527` with the area's sprite set number `s` in A
  (record `+$10`): R3 = `09:A035 + 2s`, R4 = `09:A036 + 2s`,
  R5 = `09:A0B5 + 2s`, and 8 bytes from `09:A105 + 8s` to `$0618`/`$0638`.
- **Palette animations**: record `+$2C..+$2F` holds four slots (bit 7 on,
  bits 0-6 the animation number), copied by `L0E_C4A5` to `$0100-$0103`
  (on) and `$0108-$010B` (number), with steps at `$0104` and timers at
  `$010C`; bank 09 `$A300` runs them every gameplay frame (`L09_A321`).
  Animation `n` has its record at `09:A458 + [09:A445 + n]`:
  `[last step, period, palette offset, triplet x (last + 1)]`; triplet `k`
  is 3 colours at `09:A370 + 3k`. The timer counts up to `period` (a step
  lasts `period + 1` frames), then the step advances (0..last, wrapping)
  and its three colours go to `$0600 + offset` and `$0620 + offset`. There
  are 19 animations. Scripts can start slots too (round 0 area 2 above).

Other screens (title, map, select, story, ending) load palettes from bank
`$04` and `$00` code; they are not extracted yet.

## Round backgrounds

### Where a round's data is

`round_bank` (0E:C1F8, 5 bytes) gives the PRG bank mapped at `$A000` while
a round plays: `$01, $05, $07, $09, $0D` for rounds 0-4 (`L0E_C3DD`,
`round_start_wait` and the column writer index it with `$22` or `$61`).
Bank `$09` is round 3's data and also the code of the sprite sets and
palette animations above. Inside the round's bank:

| Address | Size | What |
|---|---|---|
| `$AE60 + $30*A` | `$30` | area record of area A (0-2) |
| `$AF00 + $40*s` | `$40` | screen `s`: 8 metatile columns x 8 rows, row by row; rows 0-4 are drawn |
| `$B700 + 4*m` | 4 | metatile `m` (32x32 px): blocks top-left, top-right, bottom-left, bottom-right |
| `$BB00 + b`, `$BC00 + b`, `$BD00 + b`, `$BE00 + b` | 1 each | block `b` (16x16 px): tiles TL, TR, BL, BR |
| `$BF00 + b` | 1 | block `b`'s palette (bits 0-1) |

### Area record

| Offset | Read by | Meaning |
|---|---|---|
| `+$00..$07` | `L0F_E66E` (`$AE60`/`$AE90`/`$AEC0` + `$29`) | screen list, left to right |
| `+$08` | 0E:C78A (`round_start_wait`), 0F:E02C (next area) | header: bits 0-4 last screen index (`$2C`), bits 5-7 to `$2A`; bit 5 lets the camera scroll (`L0F_E2C7`). All 15 headers are `$2x` |
| `+$09..$0F` | - | never read |
| `+$10..$17` | `L0E_C44F` | four (sprite set, palette set) pairs picked by `$2B`, which is always 0 |
| `+$18/$19` | `L0E_C3F3` (`L0E_C3FF`, `L0E_C40B`) | CHR R0/R1 for the background (`$E8`/`$E9`) |
| `+$1A/$1B` | - | never read |
| `+$1C..$2B` | `L0E_C494` | background palette |
| `+$2C..$2F` | `L0E_C4A5` | palette animation slots |

An area is `last + 1` screens wide: 1 (round 2 area 2, round 3 area 1) to
5 (round 1 area 1). There are three areas per round:
`player_state_transition` (0E:DE5E) wraps `sub_area` after area 2 and moves
to the next round.

### How the game draws it

The column writer (`L0F_E521`, fixed bank) puts one 8-pixel column of the
world into the nametable each time the camera crosses a multiple of 8
(`L0F_E4DF` compares `scroll_x` with last frame's copy in `$25`), and 33
columns, one per frame, when an area starts (the `round_start_wait` loop
and its twin at 0F:E001). `$24` is the column (0-31) and `$29` the screen
index; the last column sent is world column `32*$29 + $24`.

1. `L0F_E66E`: screen number = list[`$29`]; `$20` = `$AF00 + 64*s`.
2. For metatile rows r = 0..4 (`L0F_E560`, `$28` = column/4 + 8r):
   metatile = screen[`$28`]; `L0F_E60F` builds a 4x4-tile block at `$06C0`
   from its four blocks and packs their palette bits into an attribute byte
   (top-left in bits 0-1 ... bottom-right in bits 6-7).
3. The column's tiles go to `$0780` as one vertical run
   `[$20, $80 + column, $14, 21 tiles]` (nametable rows 4..24), `$1A` set;
   `nmi` writes it with `L0E_C201` and PPUCTRL +32. The first byte (row 4)
   and the last (row 24) are forced to tile 0 (0F:E5F4), so **the first tile
   row of every screen's data is never shown**. Tile 0 is blank in every
   background bank, which hides the raster splits on those rows.
4. On odd columns the attribute byte of each metatile row is rewritten at
   `$23C8 + 8r + column/4`: column 4c+1 writes the left half (mask `$33`),
   column 4c+3 the right half (`$CC`), the other half coming from the copy
   at `$0640`. So for one 8-pixel step the newest column at the right edge
   keeps the previous screen's palette in its top or bottom 16x16 block; a
   renderer that rebuilds the nametable the same way reproduces it.
5. Mirroring is horizontal (`MMC3_MIRRORING` = 1 at game start), so the
   nametable is 32 columns wide and wraps; the column being replaced sits
   under the left 8 pixels, which PPUMASK `$18` hides for both layers.

The playfield is nametable rows 4-23 (screen lines 32-191; rows 4 and 24
are blank). MMC3 IRQs split the screen (`nmi_busy` sets latch `$21` and
the handler 0E:C14A): lines 0-33 use x scroll 0 (status rows), the
handler at 0E:C14A writes the camera's x (`$7D`, the copy of `scroll_x`
`nmi` makes) for the playfield with latch `$A1`, and 0E:C1B1 switches to
x 0 and CHR `$78`/`$7A` for the bottom status rows. In **round 1 areas 1
and 2** (RAM `$61` = 1, `$60` = 1 or 2) the first handler writes x = 0
instead with latch `$4A`, and 0E:C194 applies the camera from line 109 on:
lines 34-108 stay still. The data's rows 0-8 (0-9 in area 2) repeat every
32 columns there, so whatever columns the nametable holds show the same
skyline. (Measured: with the switch at line 108 or 109 every frame checked
matches, at line 110 163 of 300 frames differ.)

Camera: `scroll_x_hi`*256 + `scroll_x` (`$F9`/`$FC`), moved only to the
right by `L0F_E3A3`/`L0F_E440`, at most 3 pixels a frame. At every screen
boundary `L0F_E440` forces `scroll_x` to 0 and sets `$50`/`$BB`, which hold
the camera until they are cleared; it stops for good at `last`*256
(`$2D` = `$2C`, `L0F_E3A3`). `$FA` is a vertical scroll that `nmi_busy`
writes to PPUSCROLL for the whole frame: game action `$0F` (00:8066) shakes
it by one line, and so does an object routine in bank `$05` (05:AC3A) in
round 3 area 1.

### What the extractor writes

`stage/roundR_areaA.png` is `256 * screens` x 160 pixels: nametable rows
4-23 of the whole area as the column writer fills them, screen y 32 at the
top, world x 0 at the left, every block with its own palette (the
right-edge attribute lag of step 4 is not in the image) and the area
record's colours (`image_palette` in the JSON; round 0 area 2 uses its
script's lit palette, since its record is all black). Palette animations
change some entries while the area plays. Row 0 of the data is drawn as
tile 0, like the game. The JSON keeps the raw maps (with that row) so a
port can rebuild the nametable column by column with the same writer.

`port/tools/assets_shotcheck.py AREA.png SHOT.png MEMDUMP` compares such an
image, cropped at the frame's camera, with an oracle `--screenshot` in RGB
(lines 40-191, x 8-255; sprite boxes and the newest column at the right
edge left out). One screenshot per area, 40 frames after a checkpoint of
`runs/cdl/tour_cody`: all 15 areas have 0 different pixels (34,856 to
37,048 compared per frame); in 7 of them some pixels (8 to 4,104) match
only through the frame's palette RAM, i.e. a palette animation step
differs from the record. On frame 14000 of `inputs/10_cody_poweron.txt`
(round 0 area 0, camera 512) all 36,432 pixels are equal in RGB; on frame
4899 (camera 0) 57 pixels show palette 3 colour 3 at animation 3's `$05`
instead of the record's `$15`.

- Screen data has 8 rows; rows 5-7 are never read (they hold one or two
  metatile numbers repeated, the same in most screens of a round).
- When the camera stops, the writer has read one list entry past the last
  screen, for the column under the left clip. Some lists hold garbage there
  (round 0 area 0 has `$5B`, which points at `$C5C0` in the fixed bank);
  the checker reproduces that read (`assets_stage.cpu_read`).
- Screens, metatiles and blocks are shared by a round's three areas; the
  CHR (R0/R1) and the palette are per area.

### Status rows (not extracted)

Rows 0-3 and 25-29 are the status bar. At every area start bank 0A `$9C00`
appends the fixed text list at 0A:9C20 (`[count, PPU hi, PPU lo, tiles...]`
until `$FF`, the `nmi_vram_update` format) to the VRAM buffer: PLAYER at
row 2 column 3, ENEMY at row 3 column 3, RD at row 2 column 24, PL x at row
3 column 24, and LEVEL, EXP and "/" on row 26 (columns 3, 18, 25). Every
gameplay frame `L0F_E6DD` (through 0F:E6AB, unless a column or patch is
waiting) refreshes the health bar from `$204A`, the round digit at `$205C`,
the lives at `$207C` and the level and experience on row 26. The bottom
rows use CHR `$78`/`$7A`, the top rows the area's background banks.

### Nametable patches (`stage/patches.json`)

Four scripted sequences overwrite parts of the playfield. `L00_987A`
copies 128 bytes from patch Y (pointer lo `00:989A + Y`, hi `00:989E + Y`)
to `$0780` and sets `$19`; `nmi` writes them with `L0E_C201`
(`[PPU hi, PPU lo, count-1, bytes...]` until a byte with bit 7 set, +1
increment). The addresses are fixed nametable positions: the entrance
patch runs with the camera at 0 (nametable column j = world column j), the
exit patches with the camera stopped on the last screen (nametable column
j = world column 32 * last + j).

| Patch | Data | Called from | When |
|---|---|---|---|
| 0 | `00:98A2` | 00:967C | game action `$1B` (00:963D): the entrance at the start of round 0 area 1 |
| 1 | `00:98DA` | 00:948A | area exit handler 1 (00:945E): round 0 area 1 |
| 2 | `00:9918` | 00:949D | the same handler, next step |
| 3 | `00:9927` | 00:959A | area exit handler 7 (00:958B): round 2 area 1 |

Exit handlers are picked from `00:9391`/`00:93A0` by `3*round + area`
(through `00:9378`/`00:937D`) by game action `$19` (00:92E1).

The patches are decoded but not checked against the oracle: none of the
replayable runs reaches those scripted exits (the bot tours leave areas
through the skip-stage action, game action `$0E`, which bypasses them),
and a short bot session from the start of round 0 area 1 did not beat it.
Drawn over the area maps they make sense: patch 0 redraws the door on the
left wall where round 0 area 1 begins, patches 1 and 2 break a hole in the
street on its last screen (the next area is the room below), and patch 3
opens a trapdoor in the floor of round 2 area 1's last screen.

## Sprites (`sprites/sprites.json`)

`objects_draw` (0F:E9BC) builds the OAM page `$0200` once per gameplay
frame from the 16 object slots; `object_draw` (0F:E9FE) does one slot.

**Bank pair.** `obj_bank` at 0F:EB93 (160 bytes, by `obj_id` `$0320`)
holds `$0A` (41 ids) or `$02` (119 ids). object_draw maps that bank at
`$8000` and the next one at `$A000` (so bank `$02`, which `objects_update`
maps at `$A000`, is read at `$8000` here).

**Animation.** `$0580` (bits 0-6) picks the script: pointer lo
`$8000 + a`, hi `$8080 + a` (128 per pair). Script:
`[last, period, frame id x (last + 1)]`. Every call of object_draw steps
it (0F:EAA4-EACB): `$0590` bits 0-6 count 0..period, and the call that
finds `period` resets them and advances `$0570` (0..last, wrapping). Frame
id 0 ends the object: `L0F_EC81` (0F:EAE7) clears its state, flags, id and
other tables, but not its position or `$0570`-`$0590`. `$0590` bit 7 makes
the object blink (not drawn while `frame_counter` bit 2 is set; that branch,
0F:EAD6, never ran in any coverage run).

**Frame.** Pointer lo `$8100 + f`, hi `$8200 + f`. Data:
`[n-1 | $80 if the layout is in bank $03 (else $0B), layout index,
(tile, attribute) x n]`. object_draw maps the layout's bank at `$A000`
before reading on (0F:EB00-EB16).

**Layout.** In that bank, pointer lo `$B100 + l`, hi `$B200 + l`; data
`[(dy, dx) x n]`, signed. Pair `$0A` uses 228 frames and 155 layouts, all in
bank `$0B`; pair `$02` uses 225 frames and 180 layouts (177 frames in bank
`$03`, 48 in `$0B`). A frame has 1 to 30 sprites.

**Placing.** Objects with `obj_flags` bit 4 live in the world: screen x
= `obj_x - scroll_x`, and they are drawn only when the subtraction leaves
`obj_x_hi` equal to `scroll_x_hi` (and `$03E0` is 0). Off that screen an
object less than `$68` pixels away is animated but not drawn (bit 7
cleared); one further away, or with animation `$7E`, or anywhere in round 3
area 1, goes to `L0F_EC33`, which deletes it in round 3 area 1 or when its
id is in the `$FF`-terminated list at 0F:ECC3, and otherwise pulls it 16
pixels toward the camera. Without bit 4, x is `obj_x` on screen. y is
`obj_y`. Bit 2 skips the object altogether (no drawing, no animation). Each sprite goes at y = obj y + dy, x = screen x + dx; with
`obj_flags` bit 6 the object faces the other way: x = screen x + (`$F8` -
dx) and the attribute's bit 6 is flipped; bit 5 is ORed in (priority). A
sprite whose y or x wraps past the screen, or whose y is `$F0` or more, is
dropped: its OAM entry gets y = `$F8` and is reused by the next sprite
(0F:EB8B/EB8C).

**Order.** `gameplay_draw` starts at OAM entry 1 (`$97` = 4) after hiding
every entry (`L0E_C31D`); entry 0 stays hidden and drawing stops when the
other 63 are used. objects_draw draws slot 15, then **the slot at X - 1,
where X is what object_draw returned for slot 15**: the slot number when
slot 15 drew nothing (so slot 14), but the next OAM offset when it did, in
which case the second object drawn is slot 3, 7, 11 or 15 (`$97` - 1), or
a "slot" past 15 whose fields are other tables' bytes. Then it increments
`draw_parity` (`$95`) and draws 0..15 when it is even, 13..0 when odd. A
slot drawn twice in a frame is stepped twice. `assets_sprites.objects_draw`
models all of this, including the X quirk.

**CHR and palettes.** Frames hold tile numbers in pattern table `$1000`,
so what they show depends on R2-R5 at the time. The player's code sets R2
together with the animation (`LDA anim,Y / JSR $F294 / LDA bank,Y /
JSR $F440`, 51 places, per-character tables at 0F:E1C1-E2A2), enemies and
bosses call `$F443`/`$F449`/`$F44F` (animation in A, R3/R4/R5 in Y), the
rest relies on the area's sprite set. The ROM has no table from animation
to banks, so `sprites.json` keeps tile numbers. To draw frames,
`assets_verify.py --contexts FILE` records, per (pair, animation), the
R2-R5 and sprite palettes it was drawn with in oracle runs and on which
object ids; `extract_assets.py --sprite-contexts FILE` draws one sheet per
object id with the most frequent context of each animation.

## Verification

`port/tools/assets_verify.py` runs the oracle
(`build_headless/Release/MightyFinalFightRecomp.exe`) headless with
`--frame-log` over a frame range (from power-on or a save state, with an
input route), or reads an existing log, and checks every gameplay frame:

1. **Nametable**: each of the 32 columns of rows 4-24 must hold the world
   column the writer last sent there (last column `W` = 32*`$29` + `$24`,
   minus one while `$1A` still holds an unsent column), and each attribute
   byte of rows 1-5 the two halves of step 4.
2. **Picture**: lines 34-191, x 8-255 of the oracle's own picture (colour
   indices) must equal the extracted world drawn the way that nametable
   holds it (tile pixels from the CHR banks the area names, palettes from
   the attribute model, colours from the frame's palette RAM), shifted by
   the frame's x scroll (`$7D`; 0 on the still lines of round 1 areas 1-2)
   and y scroll (`$FA` as the previous frame left it), except under the 8x8
   box of any sprite in OAM; x 0-7 must be the backdrop colour.
3. **CHR and palette**: frames on other R0/R1 than the area's (the round
   map) are skipped; palette RAM `$3F00-$3F0F` must equal the area palette
   or a scripted one, where entries an active animation slot (`$0100`
   as RAM has it) owns may hold any of its steps. A palette that matches
   after the fade arithmetic of `L0E_C362`/`L0E_C36E` (each colour minus
   `$10*k`, `$0F` when it goes negative) counts as a fade step; anything
   else is counted as unexplained (the picture check uses the frame's own
   palette RAM either way).
4. **Sprites**: `assets_sprites.simulate_objects_draw` rebuilds the `$0200`
   page from the frame's RAM and the extracted tables, by winding each
   slot's animation back to where the previous frame left it (or by the
   steps the draw gave it, when the behaviour code restarted it) and
   running the objects_draw model forward; every visible entry must match
   all four bytes and every hidden entry its y. Only frames where
   objects_draw ran to the end are compared (`draw_parity` moved and task
   0 is asleep).

Skipped (and counted): frames that are not gameplay (select and continue
screens, the ending, rendering off), lag frames (the main loop was still
running when the frame ended, so `render_off_depth` is still 1), the first
33 frames of an area while its columns are drawn, area exits (the camera
resets before the next area's columns arrive), the first frame of each log
(the checks need the previous frame), and y scrolls larger than 16.

Runs used (all states and routes are local, under `states/` and `runs/`):

- **Round 0 area 0 (the game's ROUND 1), the whole route**:
  `inputs/10_cody_poweron.txt` from `states/r1_cody_start.cycstate` (frame
  4861) to its last frame, 19869, in 2,000-frame pieces (states saved on the
  way with `--save-state F:FILE` from the same route).
- **Every area**: the checkpoints of the three bot tours
  (`runs/cdl/tour_{cody,guy,haggar}/fNNNNNN.cycstate`, one every ~600
  frames through all 15 areas), each run for 600 frames with the tour's
  `route.txt` as input. The tours were god-mode sessions, so the replays
  leave the bot's path after a while (they even pause the game, which the
  picture check covers and the sprite check skips); they are ordinary oracle
  runs all the same.

```
py -3 -I port/tools/assets_verify.py ROM --state runs/cdl/tour_cody/f010269.cycstate \
    --input runs/cdl/tour_cody/route.txt --first 10269 --last 10868 --contexts runs/assets/verify/ctx.json
```

Results (oracle at nesrecomp `2b4fcdb`, this repo's `build_headless`,
2026-10-16). "Exact" means nametable, attributes and every compared pixel
equal; "OAM" counts frames whose `$0200` page was rebuilt exactly.

| Run | Frames compared | Exact | Pixels compared | Pixels different | Nametable diffs | OAM frames exact | Visible sprites checked |
|---|---:|---:|---:|---:|---:|---:|---:|
| `inputs/10_cody_poweron.txt`, frames 4861-19869 | 14,850 | 14,850 | 560,278,857 | 0 | 0 | 14,847 of 14,847 | 387,095 |
| `runs/cdl/tour_cody` checkpoints | 25,087 | 25,087 | 942,972,362 | 0 | 0 | 13,693 of 13,693 | 359,667 |
| `runs/cdl/tour_guy` checkpoints | 25,177 | 25,177 | 945,666,256 | 0 | 0 | 13,752 of 13,752 | 376,786 |
| `runs/cdl/tour_haggar` checkpoints | 25,376 | 25,376 | 950,440,208 | 0 | 0 | 14,067 of 14,067 | 388,869 |
| all | 90,490 | 90,490 | 3,399,357,683 | 0 | 0 | 56,359 of 56,359 | 1,512,417 |

Per area, all runs together:

| Round | Area | Frames compared (all exact) | With a y scroll | In a fade step | Palette unexplained | OAM frames (all exact) |
|---|---|---:|---:|---:|---:|---:|
| 0 | 0 | 18,444 | 0 | 0 | 0 | 16,929 |
| 0 | 1 | 3,594 | 0 | 0 | 0 | 2,082 |
| 0 | 2 | 4,541 | 0 | 0 | 0 | 4,541 |
| 1 | 0 | 4,478 | 0 | 42 | 0 | 3,825 |
| 1 | 1 | 3,594 | 0 | 0 | 0 | 3,050 |
| 1 | 2 | 11,380 | 0 | 0 | 0 | 514 |
| 2 | 0 | 3,636 | 0 | 24 | 0 | 3,807 |
| 2 | 1 | 5,321 | 0 | 42 | 0 | 4,238 |
| 2 | 2 | 9,609 | 0 | 48 | 0 | 42 |
| 3 | 0 | 4,260 | 0 | 42 | 0 | 4,213 |
| 3 | 1 | 3,566 | 1,736 | 0 | 0 | 3,298 |
| 3 | 2 | 3,594 | 0 | 0 | 0 | 3,546 |
| 4 | 0 | 5,204 | 0 | 42 | 0 | 3,627 |
| 4 | 1 | 3,594 | 0 | 0 | 0 | 2,237 |
| 4 | 2 | 5,675 | 0 | 0 | 0 | 410 |

14,519 frames were skipped: 12,350 not gameplay or lag (most of them the
ending and the screens between rounds), 1,283 on the round map's CHR and
24 on the previous area's, 435 while an area's first columns were drawn,
153 at area exits, 138 on the continue screen, 136 first frames of a log.
The sprite check also skips the 36,161 frames where objects_draw did not
run and one lag frame. Those are mostly stretches where the replayed routes
paused the game (a START press meant for something else): in round 1 area
2, round 2 area 2 and round 4 area 2 the replays spent most of the area
paused, hence their few OAM frames.

The reports (one line per frame), summaries and sprite contexts of these
runs are in `runs/assets/verify/<run>/`.

## Corrections to other files

Found while writing the extractor; all of them have since been made in the
files named.

- `disasm/labels.toml`, `stage_scroll` (08:8080) and `docs/engine.md`:
  the routine does not scroll. It spawns objects from the area's list
  (pointers `$72`-`$7B`, set by 08:8000 from tables indexed by `$67`) when
  the camera (`scroll_x`, `$2D`) reaches their x, scanning in the camera's
  direction. Scrolling is `L0F_E2A3`-`L0F_E4DF` in the fixed bank and the
  background columns are `L0F_E521`.
- `scroll_mode` (`$23`): the column writer stores its direction there
  (`$10`: 1 right, 2 left; `L0F_E521`); the spawner reads it.
- `player_action_dispatch` (00:8000) and `docs/engine.md` ("39-entry
  table"): the split table has 42 entries (lo `$8011-$803A`, hi
  `$803B-$8064`; the first handler is at `$8065`).
- `obj_flags` (`$0560`) "bit 4 = drawn" and the `object_draw` comment: bit 4
  means world coordinates (object_draw subtracts the camera); objects
  without it are drawn at `obj_x` on screen. Bit 7 = on the camera's screen
  (object_draw sets or clears it), bit 6 = facing (horizontal flip), bit 5 =
  priority (behind the background), bit 2 = neither drawn nor animated.
- `objects_draw` comment ("slots 15, 14, then ..."): the second object is
  slot X-1 with X as object_draw returned it for slot 15, which is slot 14
  only when slot 15 put no sprite in OAM (see Sprites, Order).
- `docs/ram_map.md`, `$0360+i` "X (screen)": it is the low byte of the
  world x for objects with `obj_flags` bit 4 (the player included), with
  `$0380+i` as high byte; only objects without bit 4 use screen x.
- `round_start_wait` comment ("loops until `$29` is set"): it draws one
  background column per frame (`L0F_E521`) while `$29` is 0, 33 columns in
  all (screen 0 and the first column of screen 1), then reads the header.
- `CLAUDE.md`, bank windows: bank `$02` is also mapped at `$8000`, with
  `$03` at `$A000`, by object_draw (sprite frames); `$0A`/`$0B` likewise.
