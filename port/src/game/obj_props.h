/*
 * obj_props.h - what the behaviours of bank $05 share (obj_props.c,
 * obj_weapons.c): the objects of ids $60-$6F (docs/objects.md, "Ids").
 *
 * objects_update maps bank $05 at $A000 for ids $60-$6F (0C:8010-803B) and
 * dispatches id $60 + k to $A700 + 3k, a JMP to the behaviour. The port
 * names these routines func_XXXX_b2 (16 KiB bank 2 of the scaffold = 8 KiB
 * banks $04 at $8000 and $05 at $A000). They keep the lifter's contract
 * (obj_defs.h); the macros below are obj_defs.h's CALL_BANK and JMP_TAIL
 * for code of this bank.
 */
#ifndef MFF_OBJ_PROPS_H
#define MFF_OBJ_PROPS_H

#include "obj_defs.h"

/* JSR at `site` to bank-$05 code, through the dispatcher */
#define CALL_PROP(site, target) do { \
        SYNC_OUT; \
        if (!mff_call_at((uint16_t)(((site) + 2) & 0x1FFF), target, 2)) return; \
        SYNC_IN; \
    } while (0)
/* JMP at `pc` to bank-$05 code the scaffold keeps as a function of its own:
 * 3 cycles, 2 more at the target. Returns. */
#define JMP_PROP(pc, target) do { \
        mff_cycles(pc, 3); \
        SYNC_OUT; \
        nes_cpu_instruction_boundary(target, 2); \
        call_by_address_tail(target, 2); \
        return; \
    } while (0)

/* ---- ids $60-$6F ---- */
#define ID_DROPPED_WEAPON   0x66    /* the weapon a hit knocks out of the player's hand (0F:FAFB) */
#define ID_DROPPED_WEAPON_2 0x67    /* the same, put in slot 1 by bank 00 (00:93E6) */
#define ID_WEAPON           0x69    /* a weapon on the floor, picked up with B */
#define ID_WEAPON_AMMO      0x6A    /* Guy's weapon on the floor, carrying its throws in obj_timer */
#define ID_HIDDEN_FOOD      0x61    /* spared when the GO sign clears the slots (05:AAC2) */

/* ---- the player (slot 0) ---- */
#define PLAYER_GUY          0x01    /* player_id: 0 Cody, 1 Guy, 2 Haggar */
#define FACE_RIGHT_FLAGS    0xD0    /* obj_flags: on screen, facing right, world coordinates */

/* game_action values */
#define GA_IDLE             0x00
#define GA_JUMP             0x01
#define GA_PICKUP           0x02    /* picking something up (also the items, 0C:8B3F) */
#define GA_CUTSCENE         0x0A    /* standing still while a stage object runs a scene */
#define GA_SKIP_STAGE       0x0E

/* the start-of-combo punch, by player_id (COMBO_FIRST_ANIM, $E288: $0D $0E $0F) */
#define ANIM_FIRST_PUNCH    0xA7A2

/* the pick-up animation and its sprite bank, by player_id (as obj_items.c) */
#define PICKUP_ANIM         0xE210
#define PICKUP_CHR          0xE258

/* ---- routines of this bank that others reach directly ---- */
void func_A82A_b2(void);    /* weapon_take */
void func_A850_b2(void);    /* an RTS */
void func_A957_b2(void);    /* dropped_weapon_fall */
void func_AC58_b2(void);    /* elevator_step */

/* ---- bank-0F routines used here and not declared in obj_defs.h ---- */
void func_F03F(void);   /* player_face_right */
void func_F1DF(void);   /* y -= obj_vy */
void func_F436(void);   /* attack_end: attack_live = 0 */

#endif
