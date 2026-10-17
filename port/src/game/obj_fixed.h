/*
 * obj_fixed.h - what the fixed-bank object helpers share (bank $0F,
 * $F000-$FBEF): the walks with background probes (obj_walk.c), the moves,
 * facing and distances (obj_move.c), animations, sprite banks and free
 * slots (obj_anim.c), the reach tests (obj_reach.c) and the hits
 * (obj_combat.c). docs/objects.md, "Movement helpers (bank $0F)" and
 * "Update: objects_update".
 *
 * These are called from every bank, many with registers and flags as their
 * interface: each routine's comment gives In/Out, and Out lists what the
 * callers really read (port/tools/exit_use.py, runs/p4/exit_use.json); the
 * code keeps those exact, and the cycles, as obj_defs.h describes.
 */
#ifndef MFF_OBJ_FIXED_H
#define MFF_OBJ_FIXED_H

#include "obj_defs.h"

/* JMP to a bank-0F routine whose 3 cycles the caller already charged (in a
 * longer run): 2 more at the target, which then returns for this one.
 * Returns. */
#define TAIL_FIXED(target, fn) do { \
        SYNC_OUT; \
        nes_cpu_instruction_boundary(target, 2); \
        { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; fn(); g_code_window_base = swb; } \
        return; \
    } while (0)

/* A helper of this file that ends the routine (its RTS or tail): the
 * registers go through g_cpu. Returns. */
#define FALL_INTO(fn) do { SYNC_OUT; fn(); return; } while (0)

/* obj_flags bits (docs/objects.md) */
#ifndef OBJF_ON_SCREEN
#define OBJF_ON_SCREEN  0x80    /* object_draw placed it on the screen */
#define OBJF_FACE_RIGHT 0x40    /* the art faces left: mirrored */
#define OBJF_BEHIND     0x20    /* sprite priority: behind the background */
#define OBJF_WORLD      0x10    /* obj_x/obj_x_hi are world coordinates */
#define OBJF_HIDDEN     0x04    /* neither drawn nor hit */
#endif

/* what the background probes leave in $10 (all the points' attributes
 * ORed, stage_probe.c) as the movement helpers read it */
#define BG_PIT          0x40    /* bit 6: a pit */
#define BG_STOP_DOWN    0x10    /* a move down is put back on top of it (proposed reading) */
#define BG_STOP_UP      0x30    /* a walk up is put back below it (proposed reading) */

/* game_action values used here (docs/ram_map.md) */
#define ACTION_JUMP         0x01
#define ACTION_GRAB         0x04
#define ACTION_HIT          0x05
#define ACTION_KNOCKDOWN    0x06
#define ACTION_HITPOS       0x0C
#define ACTION_0D           0x0D
#define ACTION_EARTHQUAKE   0x0F
#define ACTION_CODY_SLASH   0x14
#define ACTION_LARIAT       0x16
#define ACTION_SPECIAL      0x17
#define ACTION_LYING        0x1F
#define ACTION_PIT_FALL     0x24
#define ACTION_DROP_IN      0x25

#define PLAYER_CODY         0x00
#define PLAYER_GUY          0x01

/* ---- the routines of these files that obj_defs.h does not declare ---- */
void func_F003(void);   /* walk right, already facing */
void func_F03F(void);   /* player_face_right */
void func_F048(void);   /* player_face_left */
void func_F054(void);   /* walk left, already facing */
void func_F0DC(void);   /* player_pit_fall_start */
void func_F119(void);   /* obj_jump_move */
void func_F16E(void);
void func_F1BF(void);
void func_F1DF(void);
void func_F20E(void);
void func_F22E(void);
void func_F24C(void);   /* obj_gravity */
void func_F26E(void);   /* obj_init_vy */
void func_F32E(void);   /* obj_flip_from_dir */
void func_F38B(void);   /* obj_dir_index */
void func_F406(void);   /* scene_frame */
void func_F428(void);
void func_F430(void);
void func_F436(void);   /* attack_end */
void func_F43B(void);   /* attack_start */
void func_F4E1(void);
void func_F522(void);
void func_F558(void);
void func_F63F(void);
void func_F679(void);
void func_F6A9(void);
void func_F72A(void);   /* find_free_effect_slot */
void func_F7EA(void);
void func_F7EF(void);
void func_F7FF(void);
void func_F833(void);
void func_F83D(void);
void func_F84C(void);
void func_F9E9(void);
void func_FA84(void);
void func_FA91(void);
void func_FAC4(void);   /* drop_weapon */
void func_FBB0(void);

/* ---- other bank-0F routines called here ---- */
void func_E9BC(void);   /* objects_draw */
void func_EF55(void);   /* push_out_left */
void func_EF6D(void);   /* push_out_right */
void func_EF83(void);   /* push_out_down */
void func_EF9D(void);   /* push_out_up */
void func_FF31(void);   /* task_wait_frame */

#endif
