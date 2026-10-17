/*
 * enemy_defs.h - names shared by the bank-$0C side of the common enemies
 * (enemy_common.c) and their entrances (enemy_entrances.c).
 * docs/objects.md, "Ids" and "The common enemies".
 */
#ifndef MFF_ENEMY_DEFS_H
#define MFF_ENEMY_DEFS_H

#include "obj_defs.h"

/* ---- ids (obj_id) ---- */
#define ENEMY_ID_FIRST      0x20    /* $20-$25 tier 1, $26-$2B tier 2, $30-$35 tier 4 */
#define ENTRANCE_ID_FIRST   0x36    /* $36-$5F: the entrances, turned into $20-$35 */

/* ---- obj_tier: the three strengths of a common enemy ---- */
#define TIER_1              0x01
#define TIER_2              0x02
#define TIER_4              0x04

/* ---- tables ---- */
#define KIND_BY_ID          0x9D5B  /* bank $0C: id - $20 -> obj_kind (22 bytes) */
#define ENTRANCE_KIND       0x9F91  /* bank $0C: id - $36 -> obj_kind (42 bytes) */
#define ENTRANCE_BECOMES    0x9FCC  /* bank $0C: id - $36 -> the common id (42 bytes) */
#define TIER_INDEX          0xA897  /* bank $0D: obj_tier 1/2/4 -> 0/1/2 */
#define KIND_CHR_SLOT       0xFC33  /* bank $0F, by obj_kind: which of chr_banks+3..5 takes its sprites */

/* ---- obj_state of the phases the entrances start in (bank $0D) ---- */
#define PHASE_STAND         0x8D    /* $A732: stand obj_ai_timer frames, then choose a move */
#define PHASE_CHARGE        0x8E    /* $A73A: Andore's charge */
#define PHASE_DROP          0x8F    /* $A89C: fall to obj_ground_y, attack out */

/* ---- bank-$0D routines ---- */
#define ENEMY_CHECKS        0xAB29  /* held, let go, the flinch of a fresh hit: C set = no phase this frame */
#define ENEMY_STAND_ANIM    0xAAAE  /* the kind's standing animation and sprite bank */

#endif
