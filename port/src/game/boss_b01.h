/*
 * boss_b01.h - names shared by the object code of 8 KiB bank $01: the
 * behaviours of ids $80-$8F, which objects_update runs with bank $01 mapped
 * at $A000 (0C:8025, docs/objects.md). In the scaffold this is the upper
 * half of 16 KiB bank 0, so the routines are func_XXXX_b0 with addresses
 * $A000-$BFFF.
 *
 * Who the ids are (stage object list types, 08:8442 and 08:85E6, and the
 * frame logs of the P4 routes, runs/p4/base_*.flog):
 *  - $80 Thrasher, the round 1 boss: type $13 (life $48, obj_kind 7),
 *    listed in area 2 (round index 0, sub_area 2); live only there in
 *    every route.
 *  - $81 Sodom, the round 2 boss: type $14 (life $58, kind 8), area 5
 *    (round index 1, sub_area 2).
 *  - $83 Sodom again, in round 4's boss area (area 11) and round 5's second
 *    area (area 13): type $16 (life $68, kind 8, animation $F0 like $81).
 *    No P4 route gets that far into those areas (the bots skip them);
 *    the coverage sessions ran it (cyc_seeds.txt, 4k:02:AA33).
 *  - $8F a prop of a scene no recorded run plays (boss_b01_scene.c).
 *  - $82 and $84-$8E: an RTS; nothing makes them.
 * $80 and $81 share one behaviour (boss_thrasher_sodom.c), $83 has its own
 * (boss_sodom_rematch.c) that reuses two of its phases; the helpers they
 * share are in boss_b01_shared.c.
 *
 * The routines keep the register model of the lifted code (mff.h,
 * obj_defs.h): A/X/Y and the flags live in the locals of REGS_IN and meet
 * g_cpu at every call and exit; each mff_cycles charge is the one of the
 * original instructions, paid before the same calls and ROM reads. Flags
 * are computed as the 6502 sets them wherever a call, an exit or a branch
 * can read them: the callers read A, X, Y, C and V back
 * (runs/p4/exit_use.json), so the SBC #7 of the kind lookups keeps its V.
 *
 * Addresses in comments are BANK:ADDR (8 KiB banks); a bare $XXXX is in
 * bank $01. "Never executed" means no recorded run reached it
 * (cyc_seeds.txt, the P4 routes).
 */
#ifndef MFF_BOSS_B01_H
#define MFF_BOSS_B01_H

#include "obj_defs.h"

/* ---- calls and exits within bank $01 (the scaffold's bank 0) ---- */

/* JSR at `site` to a routine of this bank, through the dispatcher; the
 * JSR's 6 cycles are charged before. Returns if the callee left through
 * the scheduler. */
#define CALL_B01(site, target) do { \
        SYNC_OUT; \
        if (!mff_call_at((uint16_t)(((site) + 2) & 0x1FFF), target, 0)) return; \
        SYNC_IN; \
    } while (0)
/* JMP to a routine of this bank, its 3 cycles charged before: 2 more at
 * the target, then on through the dispatcher. Returns. */
#define JMP_B01(target) do { \
        SYNC_OUT; \
        nes_cpu_instruction_boundary(target, 2); \
        call_by_address_tail(target, 0); \
        return; \
    } while (0)
/* JMP to a bank-0F routine, its 3 cycles charged before. Returns. */
#define JMP_F(target, fn) do { \
        SYNC_OUT; \
        nes_cpu_instruction_boundary(target, 2); \
        { uint16_t swb_ = g_code_window_base; g_code_window_base = 0xE000; fn(); g_code_window_base = swb_; } \
        return; \
    } while (0)
/* The 6502 runs on into code a C function stands for (an entry of its own
 * in the scaffold, or a shared body): registers out, the function, return.
 * Nothing is pushed or charged. */
#define RUN_ON(fn) do { SYNC_OUT; fn(); return; } while (0)

/* ---- kinds and phases ---- */

#define KIND_THRASHER       0x07    /* obj_kind; kind - 7 indexes the two-entry tables */
#define KIND_SODOM          0x08

/* obj_state = OBJ_LIVE | phase. Thrasher and Sodom ($80/$81, table $A6C4) */
#define TS_ENTER            0x80    /* $A13C: on screen 1, or walk in */
#define TS_CIRCLE           0x81    /* $A169: walk the arena's edges */
#define TS_ATTACK           0x82    /* $A2ED: the swing, combos */
#define TS_WANDER           0x83    /* $A417: drift toward the player */
#define TS_DASH             0x84    /* $A473: the rush */
#define TS_JUMP             0x85    /* $A557: jump on the spot (Thrasher) */
#define TS_LEAP             0x86    /* $A5CF: jump forward (Thrasher) */
#define TS_HURT             0x87    /* $A633: hit reaction */
#define TS_WALK_IN          0x88    /* $A651: walk in from the next screen */
/* Sodom of rounds 4-5 ($83, table $AAC6); phases 2 and 4 are $A2ED, $A473 */
#define SR_DECIDE           0x80    /* $AAD8 */
#define SR_APPROACH         0x81    /* $AB0A */
#define SR_ATTACK           0x82    /* $A2ED */
#define SR_RAGE             0x83    /* $AC4E: the enraged charges */
#define SR_DASH             0x84    /* $A473 */
#define SR_BACK_OFF         0x85    /* $ABB6 */
#define SR_WAIT             0x86    /* $ADBA */
#define SR_HURT             0x87    /* $AD8C */
#define SR_FOLLOW_UP        0x88    /* $AE19 */

/* obj_tier of id $83: 1 while obj_life >= $30, 2 below, 3 once enraged
 * ($AA33, $AAD8). Thrasher and Sodom of round 2 keep it 0 ($A106). */
#define SR_TIER_HEALTHY     0x01
#define SR_TIER_HURT        0x02
#define SR_TIER_ENRAGED     0x03
#define SR_LIFE_RAGE        0x30

/* The boss fields these behaviours give their own meaning */
#define BOSS_DIR_INDEX      OBJ_LAND_Y      /* direction index 0-$0F of the circling walk ($A1A9) */
#define BOSS_TRIED          OBJ_DOWN_TIMER  /* bits 0-2: decisions taken since the last reset ($A26F-$A2D4) */
#define BOSS_ATTACK_SET     OBJ_TIMER2      /* 0 Thrasher's swing, 1 Sodom's (kind - 7, $A18E) */
#define BOSS_HIT_PAUSE      OBJ_TIMER3      /* $18 when a swing hit the player ($A3B9) */
#define MOVE_BLOCKED        TMP15           /* boss_move's result: 8 stopped at the top line, 4 at the bottom */

/* Direction indexes: 0 up, 4 right, 8 down, $0C left, sixteen in all;
 * 0F:FBF0 turns one into obj_dir pad bits */
#define DIR_INDEX_UP        0x00
#define DIR_INDEX_RIGHT     0x04
#define DIR_INDEX_DOWN      0x08
#define DIR_INDEX_LEFT      0x0C
#define DIR_PAD_BITS        0xFBF0
#define DIR_UP              0x08    /* obj_dir pad bits */
#define DIR_DOWN            0x04

/* the arena: obj_x limits on screen 1 (obj_x_hi 1) and obj_y lines */
#define ARENA_LEFT          0x18
#define ARENA_RIGHT         0xF1
#define ARENA_BOTTOM        0xAE

/* game_action values the bosses test (docs/ram_map.md) */
#define ACT_JUMP            0x01
#define ACT_GRAB            0x04
#define ACT_KNOCKDOWN       0x06
#define ACT_HELD_A          0x0A    /* a boss holds the player (bank $03) */
#define ACT_CODY_THROW_B    0x0B
#define ACT_HELD_19         0x19
#define ACT_LYING           0x1F
#define ACT_GRAB_KNEE       0x21
#define ACT_DROP_IN         0x25

#define OBJ_TALK            0x6E    /* the boss talk object: while it sits in slot $0D, the boss waits */
#define TALK_SLOT           0x0D

/* ---- routines (bank $01 here; bank 0F ones the object code lacks) ---- */
void func_F1BF(void);   /* obj_move_down: y += obj_vy */
void func_F1DF(void);   /* obj_move_up: y -= obj_vy */
void func_F32E(void);   /* obj_flip_from_dir */

void func_A031_b0(void);
void func_A090_b0(void);
void func_A54C_b0(void);
void func_A6FC_b0(void);
void func_A736_b0(void);
void func_A7BB_b0(void);
void func_ACC1_b0(void);
void func_AD0B_b0(void);

#endif
