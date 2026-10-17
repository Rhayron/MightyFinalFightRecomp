/*
 * stage.h - what the stage files share (PLAN_DECOMP.md P4): the camera,
 * the background columns, the object drawing, the HUD, the background
 * probes, the area setup and the stage scripts of banks $08/$09.
 *
 * These routines were lifted by port/tools/lift.py and then tidied by hand.
 * They keep the lifter's model (mff.h): A/X/Y and the flags live in the
 * locals a/x/y and n/z/c/v of REGS_IN and meet g_cpu at every call and exit
 * (SYNC_OUT/SYNC_IN), and each path charges the cycles the 6502 spent, with
 * mff_cycles, before every call and hardware access it made (prg_read and
 * mem_read included). Where code falls or branches into another entry point,
 * the C calls that entry's function directly: the original had no JMP
 * there, so no tail-call charge either.
 */
#ifndef MFF_STAGE_H
#define MFF_STAGE_H

#include "mff.h"
#include "state.h"

/* ---- the 6502 flags, on the locals of REGS_IN ---- */

/* N and Z as a load, a transfer, INC/DEC or a logic op leaves them */
#define SET_NZ(val)    (n = ((uint8_t)(val)) >> 7, z = ((uint8_t)(val)) == 0)
/* CMP/CPX/CPY reg, m: C = reg >= m, N and Z of reg - m */
#define SET_CMP(reg, mv) (c = (uint8_t)(reg) >= (uint8_t)(mv), SET_NZ((uint8_t)(reg) - (uint8_t)(mv)))
/* ADC m / SBC m on a with the carry in c, setting N/Z/C/V */
#define ADC_A(mv) do { uint8_t m_ = (uint8_t)(mv); int w_ = a + m_ + c; \
        v = (~(a ^ m_) & (a ^ w_) & 0x80) != 0; c = w_ > 0xFF; a = (uint8_t)w_; SET_NZ(a); } while (0)
#define SBC_A(mv) do { uint8_t m_ = (uint8_t)(mv); int w_ = a - m_ - !c; \
        v = ((a ^ m_) & (a ^ w_) & 0x80) != 0; c = w_ >= 0; a = (uint8_t)w_; SET_NZ(a); } while (0)

/* JMP to a fixed-bank routine the scaffold keeps as a function of its own
 * (a tail call): the generated code charges the target 2 more cycles and
 * runs it in the $E000 window. The caller syncs its registers first and
 * returns right after. */
static inline void stage_jmp(uint16_t target, void (*fn)(void)) {
    uint16_t swb;
    nes_cpu_instruction_boundary(target, 2);
    swb = g_code_window_base;
    g_code_window_base = 0xE000;
    fn();
    g_code_window_base = swb;
}

/* ---- names ---- */

/* player_id ($34) */
#define PLAYER_CODY     0
#define PLAYER_GUY      1
#define PLAYER_HAGGAR   2

/* player_dir ($31) and the direction byte $10 of the camera and the column
 * writer: pad bits */
#define DIR_RIGHT       1
#define DIR_LEFT        2

/* game_action ($30, docs/ram_map.md) values this code tests by name */
#define GA_IDLE         0x00
#define GA_GRAB         0x04    /* the player holds an enemy */
#define GA_SKIP_STAGE   0x0E
#define GA_CODY_SLASH   0x14
#define GA_AREA_EXIT    0x19    /* walking out of a cleared area (00:92E1) */
#define GA_AUTOWALK     0x1D    /* the camera scrolls itself to the next screen */
#define GA_GUY_POWER    0x27

/* area_flags ($2A): the area header's top bits; this one lets the camera scroll */
#define AREA_SCROLLS    0x20

/* obj_flags ($0560) bits (docs/objects.md) */
#define OBJF_ON_SCREEN  0x80    /* object_draw sets it for a slot it places on the screen */
#define OBJF_FACE_RIGHT 0x40    /* the art faces left: mirrored */
#define OBJF_BEHIND     0x20    /* sprite priority: behind the background */
#define OBJF_WORLD      0x10    /* obj_x/obj_x_hi are world coordinates */
#define OBJF_HIDDEN     0x04    /* neither drawn nor hit */

/* ---- routines (fixed bank) ---- */

/* camera (stage_camera.c) */
void func_E2A3(void);   /* camera_update */
void func_E3A3(void);   /* camera_scroll_right */
void func_E473(void);   /* camera_keep_player_on_screen */

/* background columns (stage_columns.c) */
void func_E4DF(void);   /* bg_column_check */
void func_E515(void);   /* bg_column_check_from */
void func_E521(void);   /* bg_column_write */
void func_E53F(void);   /* bg_column_turn */
void func_E560(void);   /* bg_column_rows */
void func_E60F(void);   /* metatile_tiles */
void func_E61A(void);   /* metatile_tiles_loop */
void func_E653(void);   /* metatile_ptr */
void func_E66E(void);   /* screen_map_ptr */
void func_E689(void);   /* screen_map_ptr_of */

/* background probes (stage_probe.c) */
void func_ECCF(void);   /* bg_probe */
void func_ED49(void);   /* bg_probe_walk */
void func_EDBA(void);   /* bg_probe_2 */
void func_EEC4(void);   /* probe_begin */
void func_EEDF(void);   /* probe_end */
void func_EF55(void);   /* push_out_left */
void func_EF6D(void);   /* push_out_right */
void func_EF83(void);   /* push_out_down */
void func_EF9D(void);   /* push_out_up */

/* status bar (stage_hud.c) */
void func_E6AB(void);   /* hud_update */
void func_E6DD(void);   /* hud_player */
void func_E7F5(void);   /* hud_next_level */
void func_E853(void);   /* hud_close */
void func_E856(void);   /* hud_dashes */
void func_E89B(void);   /* hud_enemy_bar */
void func_E8BD(void);   /* hud_enemy_bar_shown */
void func_E954(void);   /* hud_level_gauge */

/* object drawing (stage_draw.c) */
void func_E9BC(void);   /* objects_draw */
void func_E9FE(void);   /* object_draw */
void func_EA6F(void);   /* object_draw_sprites */
void func_EC33(void);   /* object_offscreen */
void func_EC81(void);   /* object_free */

/* ---- routines of 16 KiB bank 4 (8 KiB banks $08 at $8000, $09 at $A000) ---- */

/* the stage object list and the timed spawner (stage_spawn.c) */
void func_8000_b4(void);    /* stage_list_setup */
void func_8038_b4(void);    /* stage_list_seek */
void func_8080_b4(void);    /* stage_spawns */
void func_80C2_b4(void);    /* stage_list_in_view */
void func_80E4_b4(void);    /* stage_list_spawn */
void func_9000_b4(void);    /* timed_spawner */
void func_9038_b4(void);    /* timed_spawn */
void func_921C_b4(void);    /* timed_spawn_r3a1 */
void func_931D_b4(void);    /* timed_spawn_r3a1_set */

/* sprite sets and background palette animation (stage_palette.c) */
void func_A000_b4(void);    /* sprite_set_load */
void func_A013_b4(void);    /* sprite_set_palette */
void func_A300_b4(void);    /* bg_palette_animate */
void func_A321_b4(void);    /* bg_palette_step */

/* the full ending (stage_ending.c) */
void func_A700_b4(void);    /* ending_text */
void func_A713_b4(void);    /* ending_text_screen */
void func_A71D_b4(void);    /* ending_text_type */
void func_A788_b4(void);    /* ending_step_left */
void func_A78E_b4(void);    /* ending_text_clear */
void func_A7D0_b4(void);    /* ending_hold */
void func_AB00_b4(void);    /* ending_pose */
void func_AB34_b4(void);    /* cody_pose */
void func_AB77_b4(void);    /* cody_pose_next */
void func_ABD0_b4(void);    /* guy_pose */
void func_ABDF_b4(void);    /* guy_pose_spin */
void func_AC20_b4(void);    /* guy_pose_leap */
void func_AC75_b4(void);    /* guy_pose_hold */
void func_AC81_b4(void);    /* haggar_pose */
void func_ACB7_b4(void);    /* haggar_pose_run */
void func_ACE1_b4(void);    /* haggar_pose_stop */
void func_AD0B_b4(void);    /* haggar_pose_hold */

/* elsewhere */
void func_F436(void);   /* attack_live = 0 */
void func_FF4D(void);   /* bank_switch */

#endif
