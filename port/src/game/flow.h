/*
 * flow.h - names shared by the game flow and the screens (flow_*.c)
 *
 * The routines in flow_*.c started as port/tools/lift.py output and keep
 * its contract (mff.h): A/X/Y and the flags live in the locals of REGS_IN,
 * meet g_cpu at every call and exit, and each mff_cycles charge stays where
 * the original instructions are. Addresses in comments are CPU addresses in
 * the 8 KiB bank given as BANK:ADDR (0E = $C000-$DFFF, 0F = $E000-$FFFF,
 * 04 = $8000-$9FFF and 05 = $A000-$BFFF when mapped for the screens).
 */
#ifndef MFF_FLOW_H
#define MFF_FLOW_H

#include "mff.h"
#include "state.h"

/* N and Z as a load, a transfer, INC or DEC of `v` leaves them (the lifted
 * code keeps the flags in the locals n and z) */
#define SET_NZ(v) (n = (uint8_t)(v) >> 7, z = (uint8_t)(v) == 0)

/* ---- pads: joy_pressed $14 (new presses) and joy_held $16 ---- */
#define PAD_A       0x80
#define PAD_B       0x40
#define PAD_SELECT  0x20
#define PAD_START   0x10
#define PAD_UP      0x08
#define PAD_DOWN    0x04
#define PAD_LEFT    0x02
#define PAD_RIGHT   0x01

/* ---- sound commands (sound_queue_push $F34A for music, $F34C for
 * effects; docs/sound.md lists them with where each is pushed) ---- */
#define MUSIC_STORY         0x07    /* the intro story (0E:C5DE) */
#define MUSIC_TITLE         0x0C    /* 0E:C659 */
#define SFX_START           0x30    /* START on the title, picks on the select screen */
#define SFX_TITLE_FLASH     0x32    /* the title's flash, $10 frames after it appears */
#define SOUND_STOP_ALL      0xF0    /* stop the music and the effect */

/* ---- PRG banks: bank_switch maps bank_request ($F4) at $8000 and
 * bank_request+1 ($F5) at $A000. The game maps them in pairs, N and N+1. */
#define BANKS_PLAYER        0x00    /* 00/01: player_action_dispatch, the story pages ($9A00) */
#define BANKS_SCREENS       0x04    /* 04/05: screen_draw ($8000, X = screen) and the screens */
#define BANKS_STAGE         0x08    /* 08/09: stage_spawns, stage_events, palette cycling */
#define BANK_SPRITE_SETS    0x09    /* at $A000: the sprite sets ($A000, A = set) and the palette
                                     * animations ($A300); also round 3's data */
#define BANK_HUD_TEXT       0x0A    /* at $8000: objects' animations (0F:EB93), and at 0A:9C00
                                     * the code that queues the HUD's fixed text */

/* ---- bank 04's screens: screen_draw (04:8000) draws screen X from the
 * pointer tables at 04:8100/810E; each screen's setup routine in bank 04
 * then loads its palette, CHR banks and sprites ---- */
#define SCREEN_CAPCOM       0x00    /* CAPCOM PRESENTS (setup 04:8900) */
#define SCREEN_TITLE        0x01    /* setup 04:8940 */
#define SCREEN_SELECT       0x02    /* run by 04:9100 */
#define SCREEN_LICENSE      0x04    /* LICENSED BY NINTENDO (setup 04:89C0) */
#define SCREEN_STORY_1      0x06    /* the picture of story page 1 (setup 04:8B40) */
#define SCREEN_STORY_2      0x07    /* the picture of story page 3 (setup 04:8AC0) */
#define SCREEN_ROUND_MAP    0x08    /* run by 04:8D00 */

/* ---- main_task (flow_main_task.c) ---- */
#define RNG_SEED            0x88    /* rng at power-on */
#define BOOT_SCREEN_FRAMES  0xB4    /* 180: the licence screen, and the CAPCOM one at most */
#define ROUND_MAP_FRAMES    0x2D    /* 45 */
#define TITLE_FLASH_AT      0xF0    /* the title flashes when its first timer gets here */
#define TITLE_FLASH_COLOR   0x12    /* put in palette_buf +3/+7/+$0F/+$1F by the flash */
#define START_LIVES         5
#define START_CREDITS       3
/* the title's three countdowns, kept in slot 0's object fields: the first
 * counts $FF frames (flashing the title at $F0), then the second $FF and
 * the third $8B; when all are 0 the game restarts */
#define TITLE_TIMER_1       OBJ_TIMER
#define TITLE_TIMER_2       OBJ_TIMER2
#define TITLE_TIMER_3       OBJ_GROUND_Y

/* ---- the fixed-bank tables flow_*.c reads (banks 0E and 0F) ---- */
#define ROUND_BANK_TABLE    0xC1F8  /* round_bank: the $A000 bank of each round (by round_tables) */
#define PLAYER_PAL_TABLE    0xC53E  /* 3 x 8 sprite colors, Cody, Guy, Haggar */
#define PLAYER_PAL_END      0xC8DA  /* per player_id: the last index of its 8 colors (7, $0F, $17) */
#define PLAYER_ANIM_STAND   0xE1E3  /* per player_id: the standing animation (1, 2, 3); 0E:CEB3 too */
#define PLAYER_CHR_STAND    0xE22B  /* per player_id: that pose's sprite CHR bank for chr_banks+2
                                     * (0, 4, 8) */

#define ROUND_MUSIC_TABLE   0xC83C  /* round_music: the music command of each round (by round_tables) */
#define START_HEALTH_TABLE  0xC841  /* start_health: per player_id, $40, $30, $50 */

/* ---- the round's data bank at $A000 (round_bank[round_tables]) starts with
 * three area records of $30 bytes, one per sub_area (docs/assets.md, "Area
 * record") ---- */
#define AREA_RECORD(k)      (0xAE60 + 0x30 * (k))
#define AREA_REC_HEADER     0x08    /* bits 0-4 the last screen (area_screen_count), bits 5-7
                                     * area_flags */
#define AREA_REC_SETS       0x10    /* 4 (sprite set, palette set) pairs; area_palette picks one */
#define AREA_REC_BG_CHR     0x18    /* CHR R0/R1: the background's banks */
#define AREA_REC_BG_PALETTE 0x1C    /* palette set n at +20n: 16 colors, then 4 palette animation
                                     * slots (bit 7 on, bits 0-6 the animation) */

/* the NES color a fade puts where darkening goes below 0: black */
#define PAL_BLACK           0x0F

/* an OAM entry's Y that puts the sprite below the screen */
#define OAM_Y_HIDDEN        0xF8

/* ---- routines flow_*.c reach through mff_call_fn or by falling into them ---- */
void func_E521(void);   /* 0F:E521: the column writer, one nametable column toward $10 */
void func_E9BC(void);   /* objects_draw */
void func_F26E(void);   /* 0F:F26E: slot X's starting fall speed */
void func_F294(void);   /* player_set_anim */
void func_F34A(void);   /* sound_queue_push (music) */
void func_F34C(void);   /* sound_queue_push_sfx */
void func_F440(void);   /* 0F:F440: chr_banks+2 = A */
void func_FE00(void);   /* reset */
void func_FF2A(void);   /* task_wait_frames */
void func_FF31(void);   /* task_wait_frame */
void func_FF4C(void);   /* 0F:FF4C: an RTS that code reaches by JMP */
void func_FF4D(void);   /* bank_switch */
void func_C85F(void);   /* screen_clear_now */

#endif
