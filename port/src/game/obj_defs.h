/*
 * obj_defs.h - names shared by the object code in obj_*.c: objects_update,
 * the hit handling, the behaviours of banks $0C/$0D (16 KiB bank 6 of the
 * scaffold) and the common-enemy AI. docs/objects.md describes the system.
 *
 * The routines in obj_*.c started as port/tools/lift.py output and keep its
 * contract (mff.h): A/X/Y and the flags live in the locals of REGS_IN and
 * meet g_cpu at every call and exit; each mff_cycles charge is paid before
 * the same calls and ROM/hardware reads as the original instructions, with
 * the scaffold's costs (base cycles, +1 for a taken branch, +2 when it
 * crosses a page, +2 more for a JMP into another function). Flags are
 * computed where a call or an exit can see them; elsewhere the next
 * instruction overwrites them and they are left out.
 *
 * Addresses in comments are BANK:ADDR, banks of 8 KiB: 0C = $8000-$9FFF,
 * 0D = $A000-$BFFF (both mapped while objects_update runs), 0E = $C000,
 * 0F = $E000. "Never executed" means no recorded run (docs/objects.md,
 * runs/cdl) reached it.
 */
#ifndef MFF_OBJ_DEFS_H
#define MFF_OBJ_DEFS_H

#include "mff.h"
#include "state.h"

/* ---- flags, as the 6502 leaves them ---- */

/* N and Z after a load, transfer, INC, DEC, AND, ORA, EOR, shift of `val` */
#define SET_NZ(val) (n = (uint8_t)(val) >> 7, z = (uint8_t)(val) == 0)
/* N, Z and C after CMP/CPX/CPY of register `r` with `val` */
#define CMP_FLAGS(r, val) \
    (c = (uint8_t)(r) >= (uint8_t)(val), SET_NZ((uint8_t)(r) - (uint8_t)(val)))
/* ADC / SBC into A, with C and V (N and Z are left to the caller) */
#define ADC_A(val) \
    (m = (uint8_t)(val), w = a + m + c, v = (~(a ^ m) & (a ^ w) & 0x80) != 0, \
     c = w > 0xFF, a = (uint8_t)w)
#define SBC_A(val) \
    (m = (uint8_t)(val), w = a - m - !c, v = ((a ^ m) & (a ^ w) & 0x80) != 0, \
     c = w >= 0, a = (uint8_t)w)

/* ---- calls and exits, exactly as the lifted code makes them ----
 * A JSR at `site` pushes site + 2 (low 13 bits, in the current window) and
 * calls a bank-0F routine directly or bank-6 code through the dispatcher;
 * 0 back from the call means the callee left through the scheduler, and the
 * caller must return at once. The JSR's 6 cycles are charged before. */
#define CALL_FIXED(site, fn) do { \
        SYNC_OUT; \
        if (!mff_call_fn((uint16_t)(((site) + 2) & 0x1FFF), fn, 0xE000)) return; \
        SYNC_IN; \
    } while (0)
#define CALL_BANK(site, target) do { \
        SYNC_OUT; \
        if (!mff_call_at((uint16_t)(((site) + 2) & 0x1FFF), target, 6)) return; \
        SYNC_IN; \
    } while (0)
/* JMP at `pc` to code the scaffold keeps as a function of its own: 3 cycles,
 * 2 more at the target, then on through the dispatcher. Returns. */
#define JMP_TAIL(pc, target) do { \
        mff_cycles(pc, 3); \
        SYNC_OUT; \
        nes_cpu_instruction_boundary(target, 2); \
        call_by_address_tail(target, 6); \
        return; \
    } while (0)
/* JMP at `pc` to a bank-0F routine, which then returns for this one.
 * Returns. */
#define JMP_FIXED(pc, target, fn) do { \
        mff_cycles(pc, 3); \
        SYNC_OUT; \
        nes_cpu_instruction_boundary(target, 2); \
        { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; fn(); g_code_window_base = swb; } \
        return; \
    } while (0)
/* JMP ($zp), charged before: 2 more cycles at the target, which the
 * dispatcher runs. Returns. */
#define JMP_INDIRECT(zp) do { \
        SYNC_OUT; \
        { uint16_t jt = nes_read16zp(zp); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); } \
        return; \
    } while (0)
/* A branch into code that is no function of the scaffold (an RTS or JMP in
 * the middle of another routine): the dispatcher hands it to the
 * interpreter. The branch is charged before. Returns. */
#define BRANCH_TAIL(target) do { \
        SYNC_OUT; \
        call_by_address_tail((uint16_t)(g_code_window_base + ((target) & 0x1FFF)), 6); \
        return; \
    } while (0)
/* RTS (its 6 cycles charged before). Returns. */
#define RTS_RETURN() do { SYNC_OUT; mff_rts(); return; } while (0)

/*
 * The game's way of asking "is A one of these": CMP #list[0] / BEQ target,
 * CMP #list[1] / BEQ target, ... from `pc` on, 4 bytes and 4 cycles per
 * test, the taken BEQ 1 more (2 into another page). Returns the index of the
 * first match, or `count`; CMP_LIST_FLAGS then sets N/Z/C as the last CMP
 * made left them.
 */
static inline int cmp_list(uint16_t pc, uint16_t target, uint8_t val,
                           const uint8_t *list, int count)
{
    int i;
    for (i = 0; i < count; i++) {
        uint16_t beq = (uint16_t)(pc + 4 * i + 2);
        mff_cycles((uint16_t)(pc + 4 * i), 4);
        if (val == list[i]) {
            mff_cycles(beq, (((beq + 2) ^ target) & 0xFF00) ? 2 : 1);
            return i;
        }
    }
    return count;
}
#define CMP_LIST_FLAGS(r, list, count, i) CMP_FLAGS(r, (list)[(i) < (count) ? (i) : (count) - 1])

/* ---- object slots ---- */
#define OBJ_LIVE            0x80    /* obj_state bit 7; bits 0-3 are the behaviour's phase */
#define OBJ_PHASE           0x0F
#define OBJ_ON_SCREEN       0x80    /* obj_flags bit 7, set by object_draw */
#define OBJ_SLOT_LAST       0x0F
#define OBJ_SLOT_ENEMY_LAST 0x0D    /* thrown bodies test slots $0D down to 1 */

/* the dispatch tables: obj_behavior_lo/hi[obj_id], 160 entries each */
#define OBJ_BEHAVIOR_LO     0x83AF
#define OBJ_BEHAVIOR_HI     0x844F

/* ---- object ids (obj_id, $0320) used by the engine ---- */
#define OBJ_NONE            0x00    /* animation only (behaviour = RTS) */
#define OBJ_SPARK           0x01    /* hit spark (0F:F455) */
#define OBJ_PUSHED_BACK     0x02    /* a light hit: pushed back */
#define OBJ_KNOCKED_BY_BODY 0x03    /* knocked down by a thrown body */
#define OBJ_THROWN_CODY     0x04    /* thrown by Cody (0E:D7F8, game_action $0B) */
#define OBJ_KNOCKED_DOWN    0x05    /* a heavy hit: knocked down */
#define OBJ_KILLED_LIGHT    0x06    /* killed by a light attack: thrown up by player_level */
#define OBJ_KILLED          0x07    /* killed by any other attack */
#define OBJ_THROWN_BACK     0x08    /* Haggar's back throw (00:8692, game_action $15) */
#define OBJ_SPARK_9         0x09    /* hit-spark behaviour; a slot-1/2 object with animation $74 becomes it */
#define OBJ_THROWN_SMASH    0x0E    /* Haggar's HeadSmasher (00:83DC, game_action $12) */
#define OBJ_THROWN_GUY      0x0F    /* Guy's throw (00:84EB, game_action $13) */
#define OBJ_PIT_FALL        0x1F    /* falling into a pit */
#define OBJ_BELGER          0x74    /* Cyborg Belger, the last boss */

/* ---- enemy kinds (obj_kind, $05B0) ---- */
#define KIND_AXL            0x03    /* can block (0C:84F0) */
#define KIND_6              0x06    /* id $2E */
#define KIND_9D             0x0C    /* id $9D: the objects that fill the screen in the Belger fight */

/* ---- the player ---- */
#define PLAYER_HAGGAR       0x02    /* player_id: 0 Cody, 1 Guy, 2 Haggar */
#define PLAYER_HIT_INVULN   0x3C    /* frames player_invuln is set to when an enemy hits */
#define COMBO_WINDOW        0x1E    /* combo_timer after a player hit connects */

/* game_action values of the throws (docs/ram_map.md) */
#define ACTION_CODY_THROW   0x0B
#define ACTION_HEADSMASHER  0x12
#define ACTION_GUY_THROW    0x13
#define ACTION_BACK_THROW   0x15

/* attack_id values with a meaning here */
#define ATTACK_HEADSMASHER  0x09    /* set by the HeadSmasher (00:83BA) */
#define ATTACK_1D           0x1D    /* the enemy lands on the player's line (0C:811D) */

/* obj_dir, in pad bits */
#define DIR_RIGHT           0x01
#define DIR_LEFT            0x02

/* knockback speed: obj_vy/obj_vy_frac = $038E, up */
#define KNOCK_VY            0x03
#define KNOCK_VY_FRAC       0x8E

/* objects_update's gravity, obj_vy_frac per frame */
#define GRAVITY_NORMAL      0x38

/* ---- bank-0F routines the object code calls ---- */
void func_ECCF(void);   /* background probe */
void func_EDBA(void);   /* background probe */
void func_F000(void);
void func_F051(void);
void func_F090(void);   /* face right */
void func_F099(void);   /* face left */
void func_F0A2(void);
void func_F0F7(void);
void func_F183(void);   /* x += obj_vx */
void func_F1A1(void);   /* x -= obj_vx */
void func_F1FD(void);   /* y -= obj_vy, then gravity */
void func_F288(void);   /* obj_set_anim */
void func_F294(void);   /* player_set_anim */
void func_F2A0(void);   /* start slot Y with animation A */
void func_F2BE(void);   /* object_clear */
void func_F314(void);   /* obj_dir toward the player */
void func_F343(void);   /* obj_dir toward the player, and the flags */
void func_F34C(void);   /* sound_queue_push (effects) */
void func_F365(void);   /* y distance to the player */
void func_F374(void);   /* x distance to the player */
void func_F440(void);   /* chr_banks+2 = A (the player's sprite bank) */
void func_F443(void);   /* obj_set_anim, chr_banks+3 = Y */
void func_F449(void);   /* obj_set_anim, chr_banks+4 = Y */
void func_F44F(void);   /* obj_set_anim, chr_banks+5 = Y */
void func_F455(void);   /* spawn_hit_spark */
void func_F55A(void);   /* enemy_attack_test */
void func_F71A(void);   /* free slot in $0D..3 */
void func_F738(void);   /* player_attack_test */
void func_F8D5(void);   /* player_take_hit */
void func_F9E5(void);   /* test against slots 2 and 1 */
void func_FB09(void);
void func_FB3E(void);
void func_FB5B(void);
void func_FB72(void);
void func_FBD9(void);
void func_FF4D(void);   /* bank_switch */

/* ---- bank-6 routines other code falls into (called directly) ---- */
void func_800B_b6(void);
void func_81F0_b6(void);
void func_819A_b6(void);
void func_8256_b6(void);
void func_827D_b6(void);
void func_8347_b6(void);
void func_83A8_b6(void);
void func_8593_b6(void);
void func_8A31_b6(void);
void func_8C00_b6(void);

#endif
