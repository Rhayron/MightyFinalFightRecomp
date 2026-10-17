/*
 * enemy_defs.h - names shared by the bank-$0C side of the common enemies
 * (enemy_common.c) and their entrances (enemy_entrances.c), on top of the
 * AI's (enemy_ai.h: the phases AI_*, TIER_INDEX, KIND_CHR_SLOT,
 * ENEMY_ID_FIRST). docs/objects.md, "The common enemies".
 */
#ifndef MFF_ENEMY_DEFS_H
#define MFF_ENEMY_DEFS_H

#include "enemy_ai.h"

/* ---- ids (obj_id) ---- */
#define ENTRANCE_ID_FIRST   0x36    /* $36-$5F: the entrances, turned into $20-$35 */

/* ---- obj_tier: the three strengths of a common enemy ---- */
#define TIER_1              0x01
#define TIER_2              0x02
#define TIER_4              0x04

/* ---- bank-$0C tables ---- */
#define KIND_BY_ID          0x9D5B  /* id - $20 -> obj_kind (22 bytes) */
#define ENTRANCE_KIND       0x9F91  /* id - $36 -> obj_kind (42 bytes) */
#define ENTRANCE_BECOMES    0x9FCC  /* id - $36 -> the plain id (42 bytes) */

/* ---- bank-$0D routines, called through the dispatcher as the original ---- */
#define ENEMY_REACT         0xAB29  /* held, let go, the flinch of a fresh hit: C set = no phase this frame */
#define ENEMY_ANIM_STAND    0xAAAE  /* the kind's standing animation and sprite bank */

#endif
