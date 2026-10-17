/* state.h - the game's RAM, from disasm/labels.toml (port/tools/state_h.py; do not edit) */
#ifndef MFF_STATE_H
#define MFF_STATE_H

#define TMP0                     0x0000  /* scratch: each routine's own temporaries (docs/ram_map.md) */
#define TMP1                     0x0001  /* scratch: each routine's own temporaries (docs/ram_map.md) */
#define TMP2                     0x0002  /* scratch: each routine's own temporaries (docs/ram_map.md) */
#define TMP3                     0x0003  /* scratch: each routine's own temporaries (docs/ram_map.md) */
#define TMP4                     0x0004  /* scratch: each routine's own temporaries (docs/ram_map.md) */
#define TMP5                     0x0005  /* scratch: each routine's own temporaries (docs/ram_map.md) */
#define TMP6                     0x0006  /* scratch: each routine's own temporaries (docs/ram_map.md) */
#define TMP7                     0x0007  /* scratch: each routine's own temporaries (docs/ram_map.md) */
#define TMP8                     0x0008  /* scratch: each routine's own temporaries (docs/ram_map.md) */
#define TMP9                     0x0009  /* scratch: each routine's own temporaries (docs/ram_map.md) */
#define TMP10                    0x000A  /* scratch: each routine's own temporaries (docs/ram_map.md) */
#define TMP11                    0x000B  /* scratch: each routine's own temporaries (docs/ram_map.md) */
#define TMP12                    0x000C  /* scratch: each routine's own temporaries (docs/ram_map.md) */
#define TMP13                    0x000D  /* scratch: each routine's own temporaries (docs/ram_map.md) */
#define TMP14                    0x000E  /* scratch: each routine's own temporaries (docs/ram_map.md) */
#define TMP15                    0x000F  /* scratch: each routine's own temporaries (docs/ram_map.md) */
#define TMP16                    0x0010  /* scratch: each routine's own temporaries (docs/ram_map.md) */
#define TMP17                    0x0011  /* scratch: each routine's own temporaries (docs/ram_map.md) */
#define TMP18                    0x0012  /* scratch: each routine's own temporaries (docs/ram_map.md) */
#define TMP19                    0x0013  /* scratch: each routine's own temporaries (docs/ram_map.md) */
#define JOY_PRESSED              0x0014  /* new presses, pad 1/2 (A $80 B $40 Select $20 Start $10 U D L R); START at the title: AND # ($0014-$0015) */
#define JOY_HELD                 0x0016  /* intro pages end when any button is held ($C5D8) ($0016-$0017) */
#define PALETTE_REQUEST          0x0018  /* nonzero: the nmi copies palette_buf ($0600-$061F) to $3F00 and clears it (0E:C061-C08B); p */
#define VRAM_LIST_REQUEST        0x0019  /* nonzero: the nmi writes the PPU string list at $0780 ([addr hi, addr lo, count-1, bytes].. */
#define VRAM_COLUMN_REQUEST      0x001A  /* same as vram_list_request but written with the +32 increment (PPUCTRL bit 2, 0E:C046-C057) */
#define UNK_1B                   0x001B  /* purpose unknown: written, never read in coverage; named so no raw address remains (rename  */
#define UNK_1F                   0x001F  /* purpose unknown: accessed only on paths no route runs; named so no raw address remains (re */
#define MAP_SCREEN_PTR           0x0020  /* pointer to the current screen's 64-byte metatile map in the round's $A000 bank: screen num ($0020-$0021) */
#define ROUND_TABLES             0x0022  /* round index for the per-round tables (music $C83C, $A000 bank $C1F8); follows round ($61)  */
#define SCROLL_MODE              0x0023  /* camera direction of the last streamed column, written by the column writer (0F:E521); its  */
#define BG_COLUMN                0x0024  /* nametable column (0-$1F) the column streamer writes next: $1F at round start (0E:C766), +1 */
#define SCROLL_X_PREV            0x0025  /* scroll_x saved after the camera update (0E:C953-C955); the streamer queues a new column wh */
#define PLAYER_X_PREV            0x0027  /* player obj_x saved after the camera update (0E:C94E-C951); the camera follows the player's */
#define MAP_CELL                 0x0028  /* index into the 8x8 metatile map of the current screen (row * 8 + column): the column strea */
#define BG_SCREEN                0x0029  /* index of the screen being streamed in the area's screen list: $FF at round start (0E:C762) */
#define AREA_FLAGS               0x002A  /* top 3 bits of the area header byte ($AE68/$AEC8, 0E:C78A-C790, 0F:E038-E03E); bit 5 = the  */
#define AREA_PALETTE             0x002B  /* palette pair index in the area header ($AE70 + 2n, 0E:C44F); only 0 is ever written (0E:C7 */
#define AREA_SCREEN_COUNT        0x002C  /* screens in the area, low 5 bits of the area header byte (0E:C793, 0F:E041): the camera sto */
#define AREA_SCREEN              0x002D  /* screens the camera has scrolled in the area: 0 at area start (0E:C79B), INC when scroll_x  */
#define CAMERA_DIR               0x002E  /* 1 while the camera follows a move right, 2 left (0F:E2D4, E313); written only, never read */
#define PROBE_SAVED_BANK         0x002F  /* $A000 bank saved by the background probes while they map the round's bank, restored afterw */
#define GAME_ACTION              0x0030  /* docs/ram_map.md; $0E = skip stage (verified) */
#define PLAYER_DIR               0x0031  /* player facing in pad bits, 1 right / 2 left (as obj_dir): 1 at round start (0F:E0A9), set  */
#define JUMP_DIR                 0x0032  /* horizontal direction of the current jump, pad bits 1/2 (joy_held & 3 when it starts, 00:83 */
#define UNK_33                   0x0033  /* purpose unknown: written, never read in coverage; named so no raw address remains (rename  */
#define PLAYER_ID                0x0034  /* Cody 0, Guy 1, Haggar 2 */
#define JUMP_MOVING              0x0035  /* nonzero when the jump started with left or right held (INC at 00:83ED, 0E:D1C7): selects t */
#define PLAYER_GROUND_Y          0x0036  /* player's ground line: obj_y while standing (0E:CEAA-CEAD and the walk code 0E:CA6A, CA96), */
#define ACTION_TIMER             0x0037  /* frames left in the current game_action: 3 for the landing (0E:D01E), 8 or $0C for recoveri */
#define GRAB_TIMER               0x0038  /* frames the player keeps an enemy held: $3C, Haggar $5A (0E:CE81-CE89, also copied into obj */
#define GRAB_SLOT                0x0039  /* slot of the enemy the player holds (0E:CE48; 0E:CC39 for the combo grab); the grab, throw  */
#define HEALTH                   0x003A  /* start_health[$34] */
#define PLAYER_INVULN            0x003B  /* frames the player ignores enemy attacks after a hit: $3C when one connects (0C:8207-8209), */
#define SPAWN_SLOT               0x003C  /* slot picked by the timed spawner (08:905B) and filled later in the same pass (08:91A5) */
#define LIVES                    0x003E
#define AREA_CLEAR               0x003F  /* 0 while the area's enemies are to be beaten; $FF when the area script sees its kill quota  */
#define PROBE_INDEX              0x0040  /* background probes: index into the probe offset tables ($EEEB/$EEF6, $EF23/$EF2D), stepped  */
#define PROBE_MAX                0x0041  /* background probes: highest tile attribute (high nibble of $BF00[tile]) found by the last p */
#define PROBE_HITS               0x0042  /* background probes: tile attribute of each probe point, up to three (STA $0042,Y at 0F:ED6B ($0042-$0044) */
#define CAMERA_LOCK              0x0050  /* $FF when the camera has scrolled onto a new screen and holds it (0F:E463, with camera_auto */
#define LEVEL_SAVED              0x0051  /* player_level at the start of the round (0E:DFA8, select screen 04:91C1), put back when con */
#define AREA_KILLS               0x0054  /* enemies defeated in the current area: INC when one dies (0C:81CE, 0C:8211, 0C:9BD3); the a */
#define UNK_5A                   0x005A  /* purpose unknown: written, never read in coverage; named so no raw address remains (rename  */
#define BOSS_AREA                0x005C  /* $FF in a round's last area (sub_area 2), 0 elsewhere (0F:E090-E0A5, which also queues the  */
#define SUB_AREA                 0x0060  /* area within the round: stage_events acts at round 2/sub_area 2 and round 3/sub_area 1 (08: */
#define ROUND                    0x0061  /* 0-based round, docs/ram_map.md (verified) */
#define SPECIAL_SCREEN_ID        0x0062  /* nonzero replaces gameplay with gameplay_special_screen (0E:C8FD, bank 04 $9800): small val */
#define SPAWN_TIMER              0x0063  /* frames until the timed spawner (08:9000) brings the next enemy: counted down at 08:901B wh */
#define VRAM_REQUEST             0x0064  /* set to $FF by game code (e.g. $E6D8); nmi_vram_update runs at the next nmi */
#define VRAM_BUFFER_LEN          0x0065  /* end of the $0780 buffer in nmi_vram_update's format ([count, addr hi, addr lo, bytes]...): */
#define SPECIAL_SCREEN_STEP      0x0066  /* phase of gameplay_special_screen: bank 04 $9800 jumps through $9D56/$9D59 by it (04:9800-9 */
#define AREA_INDEX               0x0067  /* area number counted over the whole game, round * 3 + sub_area (0 at a new game 0E:C756, IN */
#define RESPAWN_TIMER2           0x0069  /* second part of the drop-in invincibility after a lost life: $B6 frames (00:8EAB), counted  */
#define RESPAWN_TIMER1           0x006A  /* first part of the drop-in invincibility: $1E frames (00:8EAF), counted first with the play */
#define PLAYER_LEVEL             0x006B  /* experience level, 0-based: up when player_exp reaches the per-level threshold, which also  */
#define PLAYER_EXP               0x006C  /* experience, 3 BCD digits: $6C tens/ones, $6D hundreds (HUD 0F:E7CA-E7EB). An enemy kill ad ($006C-$006D) */
#define PLAYER_NO_HIT            0x006E  /* bit 7 set while the player's own grab, throw or special runs (0E:D5B7, D73A, D7F2, D88C; 0 */
#define PLAYER_STAND_ANIM        0x006F  /* animation the player goes back to when idle: $E288[player_id] after most actions (00:82A2, */
#define SPAWN_LIST_X             0x0072  /* pointer to the x low bytes of the area's stage object list (08:8000-8034 from tables index ($0072-$0073) */
#define SPAWN_LIST_X_HI          0x0074  /* pointer to the x high bytes of the stage object list (08:8005; read at 08:80C9) ($0074-$0075) */
#define SPAWN_LIST_Y             0x0076  /* pointer to the y bytes of the stage object list (08:8023; read at 08:80D1) ($0076-$0077) */
#define SPAWN_LIST_Y_HI          0x0078  /* pointer to the y page bytes of the stage object list (08:8019; read at 08:80DD, 08:803A) ($0078-$0079) */
#define SPAWN_LIST_TYPE          0x007A  /* pointer to the type bytes of the stage object list (08:802D); a type indexes the spawn tab ($007A-$007B) */
#define IRQ_SCROLL               0x007C  /* scroll latched by the nmi for the raster split: $F8, scroll_x and $FD copied to $7C/$7D/$7 ($007C-$007E) */
#define TASKS                    0x0080  /* 4 x {state, timer, sp_or_entry_lo, entry_hi}; state 0 free, 1 sleeping, 2 running, 4 ready ($0080-$008F) */
#define SCHED_NMI_FLAG           0x0090  /* the nmi sets it to $FF (0E:C0E1-C0E3); the scheduler clears it before scanning the tasks a */
#define TASK_CURRENT             0x0091  /* index of the running task (scheduler $FEDB) */
#define FRAME_COUNTER            0x0092  /* INC every nmi ($C0E0) */
#define TASK_ENTRY               0x0093  /* task entry pointer: reset puts main_task ($C556) here for task_spawn (0F:FEA9-FEB3, read a ($0093-$0094) */
#define DRAW_PARITY              0x0095  /* INC per objects_draw; bit 0 picks the draw order */
#define DRAW_SLOT                0x0096  /* slot objects_draw is drawing (0F:E9D8-E9F9: 0..15 or 13..0 by draw_parity) */
#define OAM_INDEX                0x0097  /* next free byte in oam_buffer: screens and gameplay_draw set 4 (0E:C99F, 0E:C867, 0F:F410), */
#define PAUSED                   0x0098
#define GRAVITY                  0x0099  /* objects_update sets $38 every frame (0C:8003-8005); 0F:F24C subtracts it from obj_vy_frac/ */
#define DRAW_BUSY                0x009A  /* nonzero while 0F:F406 redraws the objects outside the gameplay loop (scenes in banks 00/04 */
#define IRQ_ENABLE               0x009B  /* 1 = raster IRQ on: the nmi writes $E000+irq_enable ($E000 disables, $E001 enables; 0E:C0DA */
#define IRQ_HANDLER              0x009C  /* IRQ jumps through it (irq, $C147); scenes install their raster split here ($009C-$009D) */
#define SPAWN_LIST_NEXT          0x009E  /* next entry of the area's stage object list to spawn as the screen scrolls right: found at  */
#define SPAWN_LIST_PREV          0x009F  /* first list entry still ahead of the screen, set with spawn_list_next (08:8075) and moved b */
#define NMI_RETURN_PC            0x00A0  /* the PC the NMI interrupted (nmi_hijack_return) ($00A0-$00A1) */
#define COMBO_TIMER              0x00A3  /* frames left to chain the next hit of a combo: $1E when a player hit connects (0C:8136), co */
#define ATTACK_LIVE              0x00A4  /* $1E while the player's current attack can hit (0F:F43B, called when an attack starts), 0 w */
#define UNK_A5                   0x00A5  /* purpose unknown: written, never read in coverage; named so no raw address remains (rename  */
#define ATTACK_GROUND_Y          0x00A6  /* ground line of the player's current attack, copied from player_ground_y when a jump attack */
#define ATTACK_ID                0x00A7  /* id of the player's current attack: indexes hit-stop $FB17 (0F:FB09), reach $F869/$F88D/$F8 */
#define COMBO_TARGET             0x00A8  /* slot of the enemy hit by attack 6 or $0C (0C:858B, 0C:8692); the combo finisher can grab i */
#define WEAPON_HELD              0x00A9  /* $FF while the player carries a picked-up weapon (05:A82A), 0 when it is dropped on a hit ( */
#define TEXT_CURSOR              0x00AA  /* special screen: character position in the current text line (index at 04:9926, PPU column  */
#define TEXT_TIMER               0x00AB  /* special screen: delay counter between text steps ($F0 after a page at 04:98BD, $0A at 04:9 */
#define UNK_AC                   0x00AC  /* purpose unknown: unclear (RAM survey); named so no raw address remains (rename with eviden */
#define WEAPON_AMMO              0x00AD  /* Guy's throws left with a picked-up weapon: 7 at pickup (05:A826), DEC per throw (00:8292); */
#define CREDITS                  0x00AF  /* 3 at a new game: the continue screen's CREDIT 3 */
#define HUD_ENEMY_SLOT           0x00B0  /* slot whose life bar the HUD shows: the enemy just hit (0C:8569, 8598, 9BC8), the held one  */
#define HUD_ENEMY_TIMER          0x00B1  /* frames the enemy bar has been shown: 0 or 1 when hud_enemy_slot changes (0C:856D, 0F:E8C4) */
#define UNK_B2                   0x00B2  /* purpose unknown: unclear (RAM survey); named so no raw address remains (rename with eviden */
#define GUY_JUMP_WINDOW          0x00B3  /* Guy only: 5 when Up is pressed during a jump with animation $3B (0E:D057-D05F); landing th */
#define DIALOGUE_CHOICE          0x00B4  /* special screen: bit 7 toggled by the pad with sound $2F (04:99FE-9A07), drawn as a two-way */
#define SCREEN_EVENT             0x00B5  /* set by the screen-advance object when the camera is $28 pixels into the next screen (05:AA */
#define UNK_B6                   0x00B6  /* purpose unknown: written, never read in coverage; named so no raw address remains (rename  */
#define GAME_FINISHED            0x00B7  /* $FF after the last round (0E:DE99-DEAA, with paused $FF and the ending music $08); gamepla */
#define INTRO_PAGE               0x00B8  /* 0..4, each drawn by $9A00 in bank 00 (main_task) */
#define UNK_B9                   0x00B9  /* purpose unknown: unclear (RAM survey); named so no raw address remains (rename with eviden */
#define GRAB_HIT_COUNT           0x00BA  /* hits delivered during a grab: each hit sets attack_id from $8CC1[n] and INC (00:8B33-8B48, */
#define CAMERA_AUTOSCROLL        0x00BB  /* $FF with camera_lock (0F:E467); once the lock is lifted, walking past the screen centre st */
#define KEEP_ON_SCREEN           0x00BC  /* $FF in areas whose script asks for it (08:8ABF, 8C43, 8CA5, 8CE1, 8D6F, 8DA8): knocked-bac */
#define SELECT_PLAYER            0x00BD  /* character the select screen starts on: 0 after the title (0E:C6F6), the last pick otherwis */
#define ACTION_BEFORE_FALL       0x00BE  /* game_action saved when the player starts falling into a pit (game_action $24: 0E:CFD3, 0F: */
#define SPECIAL_HIT              0x00BF  /* nonzero when the special move (game_action $17, Haggar's $16) hit an enemy (0C:859E-85B4); */
#define SND_FLAGS                0x00C0  /* docs/sound.md (port/tools/sound_model.py matches the oracle on every frame) */
#define SND_TMP                  0x00C1  /* scratch; $C3 also gets the caller's Y in the specials (06:81AE); docs/sound.md (port/tools ($00C1-$00C4) */
#define SND_INSTR_PTR            0x00C5  /* docs/sound.md (port/tools/sound_model.py matches the oracle on every frame) ($00C5-$00C6) */
#define SND_TICKS                0x00C7  /* docs/sound.md (port/tools/sound_model.py matches the oracle on every frame) */
#define SND_TEMPO_ACC            0x00C8  /* docs/sound.md (port/tools/sound_model.py matches the oracle on every frame) */
#define SND_TEMPO                0x00C9  /* docs/sound.md (port/tools/sound_model.py matches the oracle on every frame) ($00C9-$00CA) */
#define SND_TRANSPOSE            0x00CB  /* docs/sound.md (port/tools/sound_model.py matches the oracle on every frame) */
#define SND_FADE_SPEED           0x00CC  /* docs/sound.md (port/tools/sound_model.py matches the oracle on every frame) */
#define SND_FADE_LEVEL           0x00CD  /* docs/sound.md (port/tools/sound_model.py matches the oracle on every frame) */
#define SND_EFFECT_PRIORITY      0x00CE  /* docs/sound.md (port/tools/sound_model.py matches the oracle on every frame) */
#define SND_EFFECT_CHANNELS      0x00CF  /* docs/sound.md (port/tools/sound_model.py matches the oracle on every frame) */
#define SND_EFFECT_PTR           0x00D0  /* docs/sound.md (port/tools/sound_model.py matches the oracle on every frame) ($00D0-$00D1) */
#define SND_EFFECT_TRANSPOSE     0x00D2  /* docs/sound.md (port/tools/sound_model.py matches the oracle on every frame) */
#define SND_EFFECT_ROW_FRAMES    0x00D3  /* docs/sound.md (port/tools/sound_model.py matches the oracle on every frame) */
#define SND_EFFECT_GATE          0x00D4  /* docs/sound.md (port/tools/sound_model.py matches the oracle on every frame) */
#define SND_EFFECT_GATE_LEFT     0x00D5  /* docs/sound.md (port/tools/sound_model.py matches the oracle on every frame) */
#define SND_EFFECT_LOOP          0x00D6  /* docs/sound.md (port/tools/sound_model.py matches the oracle on every frame) */
#define SND_EFFECT_RELAUNCH      0x00D7  /* docs/sound.md (port/tools/sound_model.py matches the oracle on every frame) */
#define SND_EFFECT_PITCH_SCALE   0x00D8  /* docs/sound.md (port/tools/sound_model.py matches the oracle on every frame) */
#define MUSIC_CURRENT            0x00D9  /* last command pushed through the 0F:F34A entry of sound_queue_push (music: round_music, tit */
#define SOUND_QUEUE_WRITE        0x00DA
#define SOUND_QUEUE_READ         0x00DB
#define SOUND_QUEUE              0x00DC  /* ring of sound commands, $88 = empty (reset fills it) ($00DC-$00E3) */
#define RNG                      0x00E4  /* rng_step state ($00E4-$00E7) */
#define CHR_BANKS                0x00E8  /* MMC3 R0-R5 shadows: nmi writes R2-R5 ($C090) and nmi_busy R0/R1 ($C0BA) from $E8+X ($00E8-$00ED) */
#define RENDER_OFF_DEPTH         0x00EE  /* rendering_off/on count; nmi takes nmi_busy while it (or $9A) is nonzero */
#define UPDATE_SLOT              0x00EF  /* slot objects_update is running (0C:8007-8066, from 1 up to $0F) */
#define MMC3_SELECT              0x00F0  /* last value written to MMC3 $8000 (bank_switch 0F:FF51/FF5F, sound_update 0F:FF78/FF84); th */
#define UNK_F1                   0x00F1  /* purpose unknown: accessed only on paths no route runs; named so no raw address remains (re */
#define PRG_BANKS                0x00F2  /* banks bank_switch last mapped at $8000/$A000 (0F:FF58, 0F:FF66); 0F:F406 saves them across ($00F2-$00F3) */
#define BANK_REQUEST             0x00F4  /* banks wanted at $8000/$A000 (bank_switch) ($00F4-$00F5) */
#define BANK_SWITCH_BUSY         0x00F6
#define SOUND_DEFERRED           0x00F7
#define UNK_F8                   0x00F8  /* purpose unknown: the NMI copies it to irq_scroll ($C031 -> $7C) but nothing writes it (alw */
#define SCROLL_X_HI              0x00F9
#define SCROLL_Y                 0x00FA  /* vertical scroll: the nmi writes it as the second $2005 value (0E:C0A5-C0A7). Only screen s */
#define SCROLL_Y_HI              0x00FB  /* vertical page matched against the stage list's y page (08:803C, 08:80E1); screen setups ze */
#define SCROLL_X                 0x00FC  /* low byte; object_draw subtracts it from obj_x; nmi copies it to $7D */
#define SCROLL_NT                0x00FD  /* bit 0 goes into PPUCTRL's nametable select at the nmi (0E:C0AF-C0B5), which also copies it */
#define PPUMASK_SHADOW           0x00FE
#define PPUCTRL_SHADOW           0x00FF  /* nmi writes it AND #$7F to $2000 ($C009) */
#define BG_PAL_ANIM_ON           0x0100  /* per background palette 0-3, bit 7 = color-cycled: loaded from the area header (0E:C4A5-C4A ($0100-$0103) */
#define BG_PAL_ANIM_STEP         0x0104  /* step of each palette's color script, wraps at $A458[script] (09:A338-A345); 0 at load (0E: ($0104-$0107) */
#define BG_PAL_ANIM_SCRIPT       0x0108  /* script of each palette (low 7 bits of the header byte, 0E:C4AF-C4B1), looked up through $A ($0108-$010B) */
#define BG_PAL_ANIM_TIMER        0x010C  /* frames on the current step, compared with $A459[script] (09:A328-A335); 0 at load (0E:C4B9 ($010C-$010F) */
#define CONTINUE_COUNT           0x0150  /* INC each time the continue screen opens with credits left (04:9300, called at 0E:DCB6); cl */
#define SAVED_ATTACK_TYPE        0x0151  /* hit reactions park the player's attack type $A7 here while they deal a landing's damage as */
#define OAM_BUFFER               0x0200  /* sprite buffer, copied by OAM DMA in every nmi that is not busy (0E:C035-C03C); object_draw ($0200-$02FF) */
#define OBJ_STATE                0x0300  /* bit 7 = live; slot 0 = player ($0300-$030F) */
#define OBJ_SAVED_STATE          0x0310  /* obj_state to resume when a temporary behaviour ends: restored with obj_saved_id after a gr ($0310-$031F) */
#define OBJ_ID                   0x0320  /*  ($0320-$032F) */
#define OBJ_SAVED_ID             0x0330  /* obj_id to resume: spawners write it with obj_id (08:813D, 08:91B3, 0F:F46D), the pit fall  ($0330-$033F) */
#define OBJ_X_FRAC               0x0340  /* x sub-pixel: 0F:F183 adds obj_vx_frac/obj_vx into obj_x_frac/obj_x/obj_x_hi, 0F:F1A1 subtr ($0340-$034F) */
#define OBJ_X                    0x0360  /* world x, low byte (obj_x_hi $0380 is the high byte); object_draw subtracts the scroll when ($0360-$036F) */
#define OBJ_X_HI                 0x0380  /* world x high byte (object_draw subtracts $F9) ($0380-$038F) */
#define OBJ_Y_FRAC               0x03A0  /* y sub-pixel: 0F:F1BF adds obj_vy_frac/obj_vy into obj_y_frac/obj_y, 0F:F1DF/F20E/F22E subt ($03A0-$03AF) */
#define OBJ_Y                    0x03C0  /*  ($03C0-$03CF) */
#define OBJ_PIT_FLAG             0x03D0  /* common-enemy AI: 1 or 2 when its background probes (0D:AA1D; tile bit $40 = pit) find a pi ($03D0-$03DF) */
#define OBJ_Y_HI                 0x03E0  /* vertical page of obj_y (0-$EF per page): the stage list gives it with x, x_hi and y (08:81 ($03E0-$03EF) */
#define OBJ_HELD                 0x03F0  /* nonzero while the player holds this object: the grab copies its hold timer here ($3C, Hagg ($03F0-$03FF) */
#define OBJ_VX_FRAC              0x0400  /* x speed, fraction byte (see obj_vx) ($0400-$040F) */
#define OBJ_VX                   0x0410  /* x speed in pixels per frame, unsigned: the caller picks 0F:F183 (right) or 0F:F1A1 (left)  ($0410-$041F) */
#define OBJ_VY_FRAC              0x0420  /* y speed, fraction byte (see obj_vy) ($0420-$042F) */
#define OBJ_VY                   0x0430  /* signed y speed, positive = up: 0F:F1FD subtracts it from obj_y and 0F:F24C applies gravity ($0430-$043F) */
#define OBJ_ATTACK               0x0440  /* bit 7 = an attack is out: objects_update then tests it against the player (0C:81F8 -> 0F:F ($0440-$044F) */
#define OBJ_ATTACK_DAMAGE        0x0450  /* index of the attack that hits the player: health -= $FD16[i], and $FD56[i] != 0 knocks the ($0450-$045F) */
#define OBJ_DIR                  0x0460  /* direction in pad bits: 1 right, 2 left, 4 down, 8 up (0D:A953-A9BE move by them). 0F:F314  ($0460-$046F) */
#define OBJ_THROW_FLAG           0x0470  /* $FF when the player throws this held enemy with the pad held toward the facing side (0E:D7 ($0470-$047F) */
#define OBJ_SPAWN_INDEX          0x0480  /* entry of the stage object list the slot was spawned from, $FF = none: the list spawner ski ($0480-$048F) */
#define OBJ_LIFE                 0x0490  /* current life: damage is subtracted from it (0C:86E4), below 0 is death (0C:80CB, 8111, 813 ($0490-$049F) */
#define OBJ_LIFE_MAX             0x04A0  /* maximum life, written only at spawn; the HUD bar length is obj_life_max/8 (0F:E8E0). (The  ($04A0-$04AF) */
#define OBJ_HIT_COOLDOWN         0x04B0  /* frames before the slot can be hit again: a player hit loads $FB17[attack] (0F:FB0E); while ($04B0-$04BF) */
#define OBJ_INVULN               0x04C0  /* nonzero = cannot be hit: the player hit test (0F:F747) and thrown bodies (0C:8278, 0C:82E8 ($04C0-$04CF) */
#define OBJ_HIT_PLAYER           0x04D0  /* $FF when this object's attack hit the player (0C:8204, with 60 frames of player invincibil ($04D0-$04DF) */
#define OBJ_HIT_TIMER            0x04E0  /* $1E when a player hit connects (0F:FB13, from 0C:85B6), counted down by the behaviours, wh ($04E0-$04EF) */
#define OBJ_SPAWN_GROUP          0x04F0  /* per-kind code from the spawn tables (Poison/AxL 4, Two P/El Gado 5, Bred/Andore 3, bosses  ($04F0-$04FF) */
#define OBJ_TIMER                0x0500  /* general per-behaviour timer or step counter: phases load it and count it down (03:BBD6 $3C ($0500-$050F) */
#define OBJ_TIMER2               0x0510  /* second per-behaviour counter: blink frames before a dead enemy is freed ($10 at 0C:8FCB, c ($0510-$051F) */
#define OBJ_GROUND_Y             0x0520  /* ground line while airborne: saved when a jump starts (0B:BB49, 0D:A437, 01:A55D) and resto ($0520-$052F) */
#define OBJ_LAND_Y               0x0530  /* line a knocked-back object lands on: objects_update sets it when a hit connects (0C:80FD-8 ($0530-$053F) */
#define OBJ_TIMER3               0x0540  /* third per-behaviour counter: frames before a thrown body starts hitting others (8 at 0C:8F ($0540-$054F) */
#define OBJ_DOWN_TIMER           0x0550  /* knockdown recovery: frames lying ($2D) and getting up ($17) before obj_saved_id/obj_saved_ ($0550-$055F) */
#define OBJ_FLAGS                0x0560  /* bit 7 on screen, 6 flip, 5 priority, 4 world coordinates (object_draw subtracts the scroll ($0560-$056F) */
#define OBJ_ANIM_FRAME           0x0570  /* frame index in the animation script (bits 0-6): object_draw advances it each time obj_anim ($0570-$057F) */
#define OBJ_ANIM                 0x0580  /* animation id, 0 = nothing drawn: object_draw reads the script pointer at $8000/$8080 + (id ($0580-$058F) */
#define OBJ_ANIM_TIMER           0x0590  /* frames shown of the current animation frame (bits 0-6, INC per draw at 0F:EAA9); bit 7 mak ($0590-$059F) */
#define SPAWN_GROUPS             0x05A0  /* timed-spawner scratch, not a slot table: obj_spawn_group of up to two live enemies (08:911 ($05A0-$05A2) */
#define OBJ_KIND                 0x05B0  /* enemy kind: 0 Poison, 1 Two P, 2 Bred, 3 AxL, 4 El Gado, 5 Andore, 6 id $2E, 7 Thrasher, 8 ($05B0-$05BF) */
#define OBJ_TIER                 0x05C0  /* common-enemy strength tier 1/2/4 by id group ($20-$25, $26-$2B, $30-$35: 0C:9CED-9CF7), tu ($05C0-$05CF) */
#define OBJ_AI_TIMER             0x05D0  /* countdown of the current AI phase: walk and wait times of the common-enemy AI (0D:A0E8, A1 ($05D0-$05DF) */
#define OBJ_AI_COUNT             0x05E0  /* AI step or attack counter: approach tries and combo length of the common-enemy AI (0D:A159 ($05E0-$05EF) */
#define OBJ_AI_FLAG              0x05F0  /* AI coin flip: a random 0/1 drawn when an approach starts (0D:A081) that picks between two  ($05F0-$05FF) */
#define PALETTE_BUF              0x0600  /* palette the nmi uploads to $3F00-$3F1F when $18 is nonzero (0E:C061-C08B): background pale ($0600-$061F) */
#define PALETTE_BASE             0x0620  /* the screen's palette as loaded (0E:C494-C49A writes it with palette_buf; 04:8D55 the chara ($0620-$063F) */
#define ATTR_SHADOW              0x0640  /* attribute bytes of nametable rows 1-5 ($23C8-$23EF), indexed by $28, so the column rendere ($0640-$0667) */
#define TILE_BLOCK               0x06C0  /* 4x4 tiles of the 32x32-pixel block the column renderer is drawing, expanded from 4 metatil ($06C0-$06CF) */
#define UNK_6DA                  0x06DA  /* written by the round map on every run (04:8D6F) and read by nothing; the pattern of the st */
#define SND_INSTRUMENT           0x0700  /* per effect channel (X 0-3); the music's copy is snd_m_instrument at +$28; docs/sound.md; p ($0700-$0703) */
#define SND_ENV                  0x0704  /* per effect channel (X 0-3); the music's copy is snd_m_env at +$28; docs/sound.md; port/too ($0704-$0707) */
#define SND_LFO                  0x0708  /* per effect channel (X 0-3); the music's copy is snd_m_lfo at +$28; docs/sound.md; port/too ($0708-$070B) */
#define SND_VOLUME               0x070C  /* per effect channel (X 0-3); the music's copy is snd_m_volume at +$28; docs/sound.md; port/ ($070C-$070F) */
#define SND_LEVEL                0x0710  /* per effect channel (X 0-3); the music's copy is snd_m_level at +$28; docs/sound.md; port/t ($0710-$0713) */
#define SND_DETUNE               0x0714  /* per effect channel (X 0-3); the music's copy is snd_m_detune at +$28; docs/sound.md; port/ ($0714-$0717) */
#define SND_SLIDE_AMOUNT         0x0718  /* per effect channel (X 0-3); the music's copy is snd_m_slide at +$28; docs/sound.md ($0718-$071B) */
#define SND_TARGET               0x071C  /* per effect channel (X 0-3); the music's copy is snd_m_target at +$28; docs/sound.md; port/ ($071C-$071F) */
#define SND_PITCH_LO             0x0720  /* per effect channel (X 0-3); the music's copy is snd_m_pitch_lo at +$28; docs/sound.md; por ($0720-$0723) */
#define SND_PITCH_HI             0x0724  /* per effect channel (X 0-3); the music's copy is snd_m_pitch_hi at +$28; docs/sound.md; por ($0724-$0727) */
#define SND_M_INSTRUMENT         0x0728  /* per music channel (snd_instrument + $28); docs/sound.md; port/tools/sound_model.py matches ($0728-$072B) */
#define SND_M_ENV                0x072C  /* per music channel (snd_env + $28); docs/sound.md; port/tools/sound_model.py matches the or ($072C-$072F) */
#define SND_M_LFO                0x0730  /* per music channel (snd_lfo + $28); docs/sound.md; port/tools/sound_model.py matches the or ($0730-$0733) */
#define SND_M_VOLUME             0x0734  /* per music channel (snd_volume + $28); docs/sound.md; port/tools/sound_model.py matches the ($0734-$0737) */
#define SND_M_LEVEL              0x0738  /* per music channel (snd_level + $28); docs/sound.md; port/tools/sound_model.py matches the  ($0738-$073B) */
#define SND_M_DETUNE             0x073C  /* per music channel (snd_detune + $28); docs/sound.md; port/tools/sound_model.py matches the ($073C-$073F) */
#define SND_M_SLIDE              0x0740  /* per music channel (snd_slide + $28); docs/sound.md; port/tools/sound_model.py matches the  ($0740-$0743) */
#define SND_M_TARGET             0x0744  /* per music channel (snd_target + $28); docs/sound.md; port/tools/sound_model.py matches the ($0744-$0747) */
#define SND_M_PITCH_LO           0x0748  /* per music channel (snd_pitch_lo + $28); docs/sound.md; port/tools/sound_model.py matches t ($0748-$074B) */
#define SND_M_PITCH_HI           0x074C  /* per music channel (snd_pitch_hi + $28); docs/sound.md; port/tools/sound_model.py matches t ($074C-$074F) */
#define SND_MUSIC_PTR_LO         0x0750  /* docs/sound.md; port/tools/sound_model.py matches the oracle on every frame ($0750-$0753) */
#define SND_MUSIC_PTR_HI         0x0754  /* docs/sound.md; port/tools/sound_model.py matches the oracle on every frame ($0754-$0757) */
#define SND_MUSIC_FLAGS          0x0758  /* docs/sound.md; port/tools/sound_model.py matches the oracle on every frame ($0758-$075B) */
#define SND_MUSIC_TRANSPOSE      0x075C  /* docs/sound.md; port/tools/sound_model.py matches the oracle on every frame ($075C-$075F) */
#define SND_MUSIC_TICKS          0x0760  /* docs/sound.md; port/tools/sound_model.py matches the oracle on every frame ($0760-$0763) */
#define SND_MUSIC_GATE_RATIO     0x0764  /* docs/sound.md; port/tools/sound_model.py matches the oracle on every frame ($0764-$0767) */
#define SND_MUSIC_GATE           0x0768  /* docs/sound.md; port/tools/sound_model.py matches the oracle on every frame ($0768-$076B) */
#define SND_MUSIC_LOOPS          0x076C  /* docs/sound.md; port/tools/sound_model.py matches the oracle on every frame ($076C-$077B) */
#define SND_PERIOD_HI_CACHE      0x077C  /* docs/sound.md; port/tools/sound_model.py matches the oracle on every frame ($077C-$077F) */
#define VRAM_BUFFER              0x0780  /* NMI upload buffer: count-prefixed strips for nmi_vram_update ($64/$65) or the write lists  ($0780-$07FF) */

#endif
