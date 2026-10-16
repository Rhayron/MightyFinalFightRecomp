/* state.h - the game's RAM, from disasm/labels.toml (port/tools/state_h.py; do not edit) */
#ifndef MFF_STATE_H
#define MFF_STATE_H

#define JOY_PRESSED              0x0014  /* new presses, pad 1/2 (A $80 B $40 Select $20 Start $10 U D L R); START at the title: AND # ($0014-$0015) */
#define JOY_HELD                 0x0016  /* intro pages end when any button is held ($C5D8) ($0016-$0017) */
#define ROUND_TABLES             0x0022  /* round index for the per-round tables (music $C83C, $A000 bank $C1F8); follows round ($61)  */
#define SCROLL_MODE              0x0023  /* low 2 bits pick stage_scroll's mode; mode 2 never ran in coverage */
#define GAME_ACTION              0x0030  /* docs/ram_map.md; $0E = skip stage (verified) */
#define PLAYER_ID                0x0034  /* Cody 0, Guy 1, Haggar 2 */
#define HEALTH                   0x003A  /* start_health[$34] */
#define LIVES                    0x003E
#define SUB_AREA                 0x0060  /* area within the round: stage_events acts at round 2/sub_area 2 and round 3/sub_area 1 (08: */
#define ROUND                    0x0061  /* 0-based round, docs/ram_map.md (verified) */
#define VRAM_REQUEST             0x0064  /* set to $FF by game code (e.g. $E6D8); nmi_vram_update runs at the next nmi */
#define TASKS                    0x0080  /* 4 x {state, timer, sp_or_entry_lo, entry_hi}; state 0 free, 1 sleeping, 2 running, 4 ready ($0080-$008F) */
#define TASK_CURRENT             0x0091  /* index of the running task (scheduler $FEDB) */
#define FRAME_COUNTER            0x0092  /* INC every nmi ($C0E0) */
#define DRAW_PARITY              0x0095  /* INC per objects_draw; bit 0 picks the draw order */
#define PAUSED                   0x0098
#define IRQ_HANDLER              0x009C  /* IRQ jumps through it (irq, $C147); scenes install their raster split here ($009C-$009D) */
#define NMI_RETURN_PC            0x00A0  /* the PC the NMI interrupted (nmi_hijack_return) ($00A0-$00A1) */
#define CREDITS                  0x00AF  /* 3 at a new game: the continue screen's CREDIT 3 */
#define INTRO_PAGE               0x00B8  /* 0..4, each drawn by $9A00 in bank 00 (main_task) */
#define SOUND_QUEUE_WRITE        0x00DA
#define SOUND_QUEUE_READ         0x00DB
#define SOUND_QUEUE              0x00DC  /* ring of sound commands, $88 = empty (reset fills it) ($00DC-$00E3) */
#define RNG                      0x00E4  /* rng_step state ($00E4-$00E7) */
#define CHR_BANKS                0x00E8  /* MMC3 R0-R5 shadows: nmi writes R2-R5 ($C090) and nmi_busy R0/R1 ($C0BA) from $E8+X ($00E8-$00ED) */
#define RENDER_OFF_DEPTH         0x00EE  /* rendering_off/on count; nmi takes nmi_busy while it (or $9A) is nonzero */
#define BANK_REQUEST             0x00F4  /* banks wanted at $8000/$A000 (bank_switch) ($00F4-$00F5) */
#define BANK_SWITCH_BUSY         0x00F6
#define SOUND_DEFERRED           0x00F7
#define SCROLL_X_HI              0x00F9
#define SCROLL_X                 0x00FC  /* low byte; object_draw subtracts it from obj_x; nmi copies it to $7D */
#define PPUMASK_SHADOW           0x00FE
#define PPUCTRL_SHADOW           0x00FF  /* nmi writes it AND #$7F to $2000 ($C009) */
#define OBJ_STATE                0x0300  /* bit 7 = live; slot 0 = player ($0300-$030F) */
#define OBJ_SAVED_STATE          0x0310  /* obj_state to resume when a temporary behaviour ends: restored with obj_saved_id after a gr ($0310-$031F) */
#define OBJ_ID                   0x0320  /*  ($0320-$032F) */
#define OBJ_SAVED_ID             0x0330  /* obj_id to resume: spawners write it with obj_id (08:813D, 08:91B3, 0F:F46D), the pit fall  ($0330-$033F) */
#define OBJ_X_FRAC               0x0340  /* x sub-pixel: 0F:F183 adds obj_vx_frac/obj_vx into obj_x_frac/obj_x/obj_x_hi, 0F:F1A1 subtr ($0340-$034F) */
#define OBJ_X                    0x0360  /*  ($0360-$036F) */
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
#define OBJ_LIFE_MAX             0x0490  /* set with obj_life at spawn from per-type tables (08:9202, 08:9215, 08:9344) ($0490-$049F) */
#define OBJ_LIFE                 0x04A0  /* the ENEMY bar; Abigail took no damage in 1,200 frames of hits (P0 notes) ($04A0-$04AF) */
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
#define OBJ_FLAGS                0x0560  /* bit 4 = drawn (object_draw) ($0560-$056F) */
#define OBJ_ANIM_FRAME           0x0570  /* frame index in the animation script (bits 0-6): object_draw advances it each time obj_anim ($0570-$057F) */
#define OBJ_ANIM                 0x0580  /* animation id, 0 = nothing drawn: object_draw reads the script pointer at $8000/$8080 + (id ($0580-$058F) */
#define OBJ_ANIM_TIMER           0x0590  /* frames shown of the current animation frame (bits 0-6, INC per draw at 0F:EAA9); bit 7 mak ($0590-$059F) */
#define SPAWN_GROUPS             0x05A0  /* timed-spawner scratch, not a slot table: obj_spawn_group of up to two live enemies (08:911 ($05A0-$05A2) */
#define OBJ_KIND                 0x05B0  /* enemy kind: 0 Poison, 1 Two P, 2 Bred, 3 AxL, 4 El Gado, 5 Andore, 6 id $2E, 7 Thrasher, 8 ($05B0-$05BF) */
#define OBJ_TIER                 0x05C0  /* common-enemy strength tier 1/2/4 by id group ($20-$25, $26-$2B, $30-$35: 0C:9CED-9CF7), tu ($05C0-$05CF) */
#define OBJ_AI_TIMER             0x05D0  /* countdown of the current AI phase: walk and wait times of the common-enemy AI (0D:A0E8, A1 ($05D0-$05DF) */
#define OBJ_AI_COUNT             0x05E0  /* AI step or attack counter: approach tries and combo length of the common-enemy AI (0D:A159 ($05E0-$05EF) */
#define OBJ_AI_FLAG              0x05F0  /* AI coin flip: a random 0/1 drawn when an approach starts (0D:A081) that picks between two  ($05F0-$05FF) */

#endif
