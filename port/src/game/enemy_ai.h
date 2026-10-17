/*
 * enemy_ai.h - names shared by the common-enemy AI of bank $0D (enemy_ai.c,
 * enemy_moves.c, enemy_walk.c). enemy_ai.c's header lists the phases;
 * docs/objects.md describes the object system around them.
 *
 * Bank $0D sits at $A000 while objects_update dispatches an id below $60
 * (the quirk in docs/objects.md), and all of its code is this AI: the
 * behaviours of the common enemies (ids $20-$35, 0C:9CED/9CF1/9CF5, which
 * the entrances $2C-$5F become: enemy_entrances.c) run 0C:9D1F, which calls
 * enemy_react ($AB29) and then jumps through the table at 0C:9D3B/9D4B by
 * obj_state & $0F to one of the sixteen phases below.
 *
 * The register model, the flag macros and the call forms are obj_defs.h's.
 * The only flags a caller reads after these routines are C (the helpers
 * that return it) and V (runs/p4/exit_use.json lists it after every one:
 * the ADCs and SBCs keep it); N and Z are left out wherever only an exit
 * or a call would see them.
 */
#ifndef MFF_ENEMY_AI_H
#define MFF_ENEMY_AI_H

#include "obj_defs.h"

/* ---- obj_state of the common enemies: OBJ_LIVE | the AI phase ---- */
#define AI_START        0x80    /* $A04C enemy_start */
#define AI_APPROACH     0x81    /* $A060 enemy_approach */
#define AI_MANEUVER     0x82    /* $A0E8 enemy_maneuver */
#define AI_BLOCK        0x83    /* $A18E enemy_block (AxL, set by obj_take_hit 0C:8574) */
#define AI_CLOSE_IN     0x84    /* $A210 enemy_close_in */
#define AI_ATTACK       0x85    /* $A2C3 enemy_attack */
#define AI_JUMP_START   0x86    /* $A41D enemy_jump_start */
#define AI_RETREAT      0x87    /* $A3F5 enemy_retreat */
#define AI_HIT_STUN     0x88    /* $A411 enemy_hit_stun */
#define AI_JUMP         0x89    /* $A494 enemy_jump */
#define AI_KNIFE        0x8A    /* $A54F enemy_knife_throw */
#define AI_SLIDE        0x8B    /* $A652 enemy_slide */
#define AI_RECOVER      0x8C    /* $A717 enemy_recover */
#define AI_IDLE         0x8D    /* $A732 enemy_idle (set by 0C:9EE9) */
#define AI_CHARGE       0x8E    /* $A73A enemy_charge */
#define AI_DROP_IN      0x8F    /* $A89C enemy_drop_in (set by 0C:9EAB) */

/* obj_kind of the common enemies, KIND_POISON ... KIND_ANDORE and
 * KIND_KNIFE (El Gado's knife, id $2E), are obj_defs.h's */

#define ENEMY_ID_FIRST  0x20    /* tables indexed by obj_id - $20 cover $20-$35 */
#define OBJ_KNIFE       0x2E    /* the knife's behaviour id (0C:9D71) */

/* obj_dir (DIR_RIGHT and DIR_LEFT are obj_defs.h's) and obj_flags */
#define DIR_DOWN        0x04
#define DIR_UP          0x08
#define OBJ_FACING_RIGHT 0x40   /* obj_flags bit 6: the sprites are mirrored */

/* game_action values the AI reacts to (player_defs.h has them all) */
#define ACT_JUMP        0x01
#define ACT_GRAB        0x04
#define ACT_KNOCKDOWN   0x06
#define ACT_HOLD        0x0A    /* a boss in bank $03 holds the player */
#define ACT_THROW       0x0B    /* Cody's throw, the end of the others */
#define ACT_AREA_EXIT   0x19    /* walking out of a cleared area (00:92E1) */
#define ACT_LYING       0x1F
#define ACT_GRAB_KNEE   0x21
#define ACT_PIT_FALL    0x24
#define ACT_DROP_IN     0x25    /* a new life drops in */

/* ---- bank-0F data the AI reads ---- */
#define DIR_FROM_INDEX  0xFBF0  /* direction index 0-15 (0 up, clockwise) -> pad bits */
#define KIND_CHR_SLOT   0xFC33  /* by obj_kind: which chr_banks+3 slot the kind's sprites use */
#define KIND_HURT_ANIM  0xFC5B  /* by obj_kind: the animation of a hurt or held enemy */
#define KIND_HURT_CHR   0xFC75  /* by obj_kind: its sprite bank */

/* ---- bank-0D tables shared by the files ---- */
#define TIER_INDEX      0xA897  /* obj_tier 1/2/4 -> 0/1/2 (also read by 0C:851F, 9E03, 9E84) */

/* the scratch of enemy_walk: what its background probes met. The probes
 * (0F:ECCF/EDBA, and the steps 0F:F000/F051/F0A2/F0F7 that call them and
 * push the object back out, C set) leave the tile attribute in $10 */
#define WALK_PROBE      TMP16   /* attribute the last probe found */
#define WALK_ATTRS      TMP14   /* the attributes of this frame's pushed-back steps, ORed */
#define WALK_BLOCKED    TMP15   /* the directions (pad bits) such a step stopped */
#define TILE_PIT        0x40    /* a pit (docs/objects.md) */
#define TILE_STOP       0x50    /* attribute bits that stop a step: $40, $10 */

/* ---- calls and exits whose JMP was charged with the segment before ---- */
/* JMP to bank-6 code the scaffold keeps as a routine of its own: 2 more
 * cycles at the target, then the dispatcher. Returns. */
#define GOTO_BANK(target) do { \
        SYNC_OUT; \
        nes_cpu_instruction_boundary(target, 2); \
        call_by_address_tail(target, 6); \
        return; \
    } while (0)
/* JMP to a bank-0F routine, which returns for this one. Returns. */
#define GOTO_FIXED(target, fn) do { \
        SYNC_OUT; \
        nes_cpu_instruction_boundary(target, 2); \
        { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; fn(); g_code_window_base = swb; } \
        return; \
    } while (0)
/* the code runs on into another routine's entry: the same code, called */
#define FALL_INTO(fn) do { SYNC_OUT; fn(); return; } while (0)

/* ---- bank-0C routines the AI calls ---- */
void func_9BE1_b6(void);    /* obj_pit_zone */
void func_9CAD_b6(void);    /* obj_anim_getting_up */

/* ---- the AI's routines (bank $0D, $A000-$AE4D) ---- */
void func_A04C_b6(void);    /* enemy_start */
void func_A060_b6(void);    /* enemy_approach */
void func_A085_b6(void);    /* enemy_walk_to_player */
void func_A088_b6(void);    /* enemy_walk_index */
void func_A0E8_b6(void);    /* enemy_maneuver */
void func_A102_b6(void);    /* enemy_maneuver_pick */
void func_A139_b6(void);    /* enemy_close_in_begin */
void func_A18B_b6(void);    /* JMP enemy_walk */
void func_A18E_b6(void);    /* enemy_block */
void func_A210_b6(void);    /* enemy_close_in */
void func_A2A6_b6(void);    /* enemy_close_in_wait */
void func_A2C3_b6(void);    /* enemy_attack */
void func_A340_b6(void);    /* enemy_attack_out */
void func_A3F5_b6(void);    /* enemy_retreat */
void func_A411_b6(void);    /* enemy_hit_stun */
void func_A41D_b6(void);    /* enemy_jump_start */
void func_A494_b6(void);    /* enemy_jump */
void func_A512_b6(void);    /* enemy_jump_air */
void func_A54F_b6(void);    /* enemy_knife_throw */
void func_A592_b6(void);    /* its RTS */
void func_A5DA_b6(void);    /* enemy_knife_spawn */
void func_A652_b6(void);    /* enemy_slide */
void func_A717_b6(void);    /* enemy_recover */
void func_A732_b6(void);    /* enemy_idle */
void func_A73A_b6(void);    /* enemy_charge */
void func_A780_b6(void);    /* enemy_charge_stalk */
void func_A7AD_b6(void);    /* enemy_charge_aim */
void func_A82C_b6(void);    /* enemy_charge_run */
void func_A89C_b6(void);    /* enemy_drop_in */
void func_A8E4_b6(void);    /* enemy_reset_wait */
void func_A8E8_b6(void);    /* enemy_reset */
void func_A8F3_b6(void);    /* enemy_walk_blocked */
void func_A953_b6(void);    /* enemy_walk */
void func_A995_b6(void);    /* enemy_walk_x */
void func_A9E3_b6(void);    /* enemy_walk_face */
void func_A9EB_b6(void);    /* enemy_face_player */
void func_A9F6_b6(void);    /* enemy_pit_under */
void func_AA1D_b6(void);    /* enemy_pit_beside */
void func_AA60_b6(void);    /* enemy_anim_hurt */
void func_AAA0_b6(void);    /* enemy_hit_pause */
void func_AAAE_b6(void);    /* enemy_anim_stand */
void func_AAD7_b6(void);    /* enemy_anim_walk */
void func_AB00_b6(void);    /* enemy_anim_attack */
void func_AB29_b6(void);    /* enemy_react */
void func_AB4F_b6(void);    /* enemy_react_held */
void func_AB9F_b6(void);    /* enemy_react_hit */
void func_AC13_b6(void);    /* enemy_react_knockdown */
void func_AC34_b6(void);    /* enemy_set_speed */
void func_AC4B_b6(void);    /* enemy_speed_08 */
void func_AC60_b6(void);    /* enemy_speed_09 */
void func_AC75_b6(void);    /* enemy_speed_12 */
void func_AC8E_b6(void);    /* enemy_speed_14 */
void func_ACB3_b6(void);    /* enemy_walk_time */

#endif
