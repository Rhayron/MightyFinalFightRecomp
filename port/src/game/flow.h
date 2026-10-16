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
#define BANK_PALETTE_ANIM   0x09    /* at $A000: the area palette scripts ($A000 with A) */

/* ---- the fixed-bank tables flow_*.c reads (bank 0E) ---- */
#define ROUND_BANK_TABLE    0xC1F8  /* round_bank: the $A000 bank of each round (by round_tables) */
#define PLAYER_PAL_TABLE    0xC53E  /* 3 x 8 sprite colors, Cody, Guy, Haggar */
#define PLAYER_PAL_END      0xC8DA  /* per player_id: the last index of its 8 colors (7, $0F, $17) */

/* the NES color a fade puts where darkening goes below 0: black */
#define PAL_BLACK           0x0F

/* an OAM entry's Y that puts the sprite below the screen */
#define OAM_Y_HIDDEN        0xF8

/* ---- routines flow_*.c reach through mff_call_fn or by falling into them ---- */
void func_E9BC(void);   /* objects_draw */
void func_F294(void);   /* player_set_anim */
void func_F34A(void);   /* sound_queue_push (music) */
void func_F34C(void);   /* sound_queue_push_sfx */
void func_F440(void);   /* 0F:F440 */
void func_FF2A(void);   /* task_wait_frames */
void func_FF31(void);   /* task_wait_frame */
void func_FF4D(void);   /* bank_switch */
void func_C85F(void);   /* screen_clear_now */

#endif
