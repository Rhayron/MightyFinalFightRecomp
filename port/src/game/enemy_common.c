/*
 * enemy_common.c - the common enemies' behaviour, their bank-$0C half
 * (ids $20-$35), the band of the screen they keep to, and El Gado's
 * knife (id $2E). docs/objects.md, "The common enemies".
 *
 * Six enemies share one behaviour: Poison, Two P, Bred, AxL, El Gado and
 * Andore (obj_kind 0-5, the id's low digit through $9D5B), each at three
 * strengths: ids $20-$25 tier 1, $26-$2B tier 2, $30-$35 tier 4
 * (obj_tier; $A897 in bank $0D makes it 0/1/2 for the phases' damage and
 * AxL's block odds). Ids $2C, $2D, $2F (Andore charging in) and $36-$5F
 * (the other entrances) set up and become one of these ids
 * (enemy_entrances.c); $2E is the knife.
 *
 * Every frame the behaviour stores the tier and the kind again, counts
 * down the short immunity left after getting up (obj_invuln), withdraws
 * its attack (the phases put it out again on the frames that hurt), stirs
 * the RNG, lets enemy_react (0D:AB29: held by the player, let go, the
 * flinch of a fresh hit) take the frame, keeps to the band
 * (enemy_keep_on_band) and jumps through its phase table on obj_state &
 * $0F. The phases are the AI of bank $0D (enemy_ai.c, enemy_moves.c,
 * enemy_walk.c; enemy_ai.h names them AI_*), mapped at $A000 for every
 * id below $60 (docs/objects.md, "Update"):
 *    0 $A04C enemy_start       8 $A411 enemy_hit_stun
 *    1 $A060 enemy_approach    9 $A494 enemy_jump (in the air: no band)
 *    2 $A0E8 enemy_maneuver   $A $A54F enemy_knife_throw (El Gado)
 *    3 $A18E enemy_block      $B $A652 enemy_slide
 *    4 $A210 enemy_close_in   $C $A717 enemy_recover
 *    5 $A2C3 enemy_attack     $D $A732 enemy_idle (entrance_stand)
 *    6 $A41D enemy_jump_start $E $A73A enemy_charge (Andore)
 *    7 $A3F5 enemy_retreat    $F $A89C enemy_drop_in (entrance_drop; in
 *                                      the air: no band)
 */
#include "enemy_defs.h"

/* the phase table, by obj_state & $0F (bank-$0D routines) */
#define ENEMY_PHASE_LO      0x9D3B
#define ENEMY_PHASE_HI      0x9D4B

/* the band: obj_y stays in (top, bottom], tables in bank $0D by band
 * (0: $80/$B0, 1: $70/$A0, 2: $90/$B0) */
#define BAND_TOP            0xA046
#define BAND_BOTTOM         0xA049

/* the knife leaves the screen: its screen x (obj_x - scroll_x) at or past
 * $FA going right, under 8 going left */
#define KNIFE_RIGHT_EDGE    0xFA
#define KNIFE_LEFT_EDGE     0x08

/*
 * enemy_common, $9CED/$9CF1/$9CF5 (name proposed): the behaviour of ids
 * $20-$25 (A = tier 1), $26-$2B (tier 2, $9CF1) and $30-$35 (tier 4,
 * $9CF5); `pc`/`cycles` are the entry's LDA #tier (and its BNE $9CF7).
 * The RNG step adds sprite 3's x (oam_buffer+15), rng+1, rng+2 and the
 * carry the behaviour was entered with into rng.
 * In: X = slot, C from objects_update. Out: the RTS at $9D24 when
 * enemy_react took the frame (C set), else as the phase.
 * Callers: obj_behavior_lo/hi.
 */
static void enemy_common(uint16_t pc, int cycles, uint8_t tier)
{
    REGS_IN;

    mff_cycles(pc, cycles);
    a = tier;
    mff_cycles(0x9CF7, 11);
    RAM(OBJ_TIER + x) = a;
    a = RAM(OBJ_INVULN + x);
    if (a != 0) {
        mff_cycles(0x9CFF, 7);
        RAM(OBJ_INVULN + x) = (uint8_t)(RAM(OBJ_INVULN + x) - 1);
    } else {
        mff_cycles(0x9CFD, 2);                  /* BEQ $9D02, into the next page */
    }

    mff_cycles(0x9D02, 37);
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OAM_BUFFER + 15);
    ADC_A(RAM(RNG));
    ADC_A(RAM(RNG + 1));
    ADC_A(RAM(RNG + 2));
    RAM(RNG) = a;
    a = RAM(OBJ_ID + x);
    c = 1;
    SBC_A(ENEMY_ID_FIRST);
    y = a;
    a = prg_read((uint16_t)(KIND_BY_ID + y));
    SET_NZ(a);
    mff_cycles(0x9D1C, 11);
    RAM(OBJ_KIND + x) = a;
    CALL_BANK(0x9D1F, ENEMY_REACT);
    mff_cycles(0x9D22, 2);
    if (c) {
        mff_cycles(0x9D24, 6);
        RTS_RETURN();
    }
    mff_cycles(0x9D22, 1);                      /* BCC $9D25 */
    mff_cycles(0x9D25, 6);
    CALL_BANK(0x9D25, 0x9FF6);                  /* enemy_keep_on_band */
    SYNC_OUT;
    obj_phase_jump(0x9D28, ENEMY_PHASE_LO, ENEMY_PHASE_HI);
}

void func_9CED_b6(void)
{
    enemy_common(0x9CED, 5, TIER_1);            /* LDA #1; BNE $9CF7 */
}

void func_9CF1_b6(void)
{
    enemy_common(0x9CF1, 5, TIER_2);            /* LDA #2; BNE $9CF7 */
}

void func_9CF5_b6(void)
{
    enemy_common(0x9CF5, 2, TIER_4);            /* LDA #4 */
}

/*
 * knife_fly, $9D71 (name proposed): the behaviour of id $2E, the knife El
 * Gado throws (0D:A5DA puts it $12 px ahead of him and 3 px lower, with
 * obj_vx $0330, attack $87, damage index 6, obj_kind 6). It flies on its
 * obj_dir, facing its way, until its screen x (obj_x - scroll_x, low
 * bytes only) reaches $FA going right or falls under 8 going left, where
 * it is cleared. The player can hit it out of the air: kind 6 has the
 * knocked-down animations $BA/$B9 ($FC8F/$FCAB) and lands $0A lower.
 * In: X = slot. Out: as object_clear or obj_move_right/left.
 * Caller: obj_behavior_lo/hi (id $2E).
 */
void func_9D71_b6(void)
{
    REGS_IN;

    mff_cycles(0x9D71, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    if (a != 0) {
        mff_cycles(0x9D78, 13);
        a = RAM(OBJ_X + x);
        c = 1;
        SBC_A(RAM(SCROLL_X));
        CMP_FLAGS(a, KNIFE_RIGHT_EDGE);
        if (c)
            JMP_FIXED(0x9D82, 0xF2BE, func_F2BE);       /* object_clear */
        mff_cycles(0x9D80, 1);                  /* BCC $9D85 */
        mff_cycles(0x9D85, 6);
        CALL_FIXED(0x9D85, func_F090);          /* obj_face_right */
        JMP_FIXED(0x9D88, 0xF183, func_F183);   /* obj_move_right */
    }
    mff_cycles(0x9D76, 1);                      /* BEQ $9D8B */
    mff_cycles(0x9D8B, 13);
    a = RAM(OBJ_X + x);
    c = 1;
    SBC_A(RAM(SCROLL_X));
    CMP_FLAGS(a, KNIFE_LEFT_EDGE);
    if (!c)
        JMP_FIXED(0x9D95, 0xF2BE, func_F2BE);   /* object_clear */
    mff_cycles(0x9D93, 1);                      /* BCS $9D98 */
    mff_cycles(0x9D98, 6);
    CALL_FIXED(0x9D98, func_F099);              /* obj_face_left */
    JMP_FIXED(0x9D9B, 0xF1A1, func_F1A1);       /* obj_move_left */
}

/*
 * The band of the area, $A001-$A02C: 1 in round 0 area 1 and round 3
 * area 1, 2 in round 1 area 1, else 0 (round, sub_area).
 */
static uint8_t area_band(void)
{
    uint8_t v;

    mff_cycles(0xA001, 5);
    v = RAM(ROUND);
    if (v == 0) {
        mff_cycles(0xA005, 7);
        v = RAM(SUB_AREA);
        if (v == 0x01) {
            mff_cycles(0xA009, 1);              /* BEQ $A017 */
            mff_cycles(0xA017, 5);              /* LDY #1; BNE $A02D */
            return 1;
        }
    } else {
        mff_cycles(0xA003, 1);                  /* BNE $A00B */
    }
    mff_cycles(0xA00B, 7);
    v = RAM(ROUND);
    if (v == 0x03) {
        mff_cycles(0xA011, 7);
        v = RAM(SUB_AREA);
        if (v == 0x01) {
            mff_cycles(0xA017, 5);              /* LDY #1; BNE $A02D */
            return 1;
        }
        mff_cycles(0xA015, 1);                  /* BNE $A01B */
    } else {
        mff_cycles(0xA00F, 1);                  /* BNE $A01B */
    }
    mff_cycles(0xA01B, 7);
    v = RAM(ROUND);
    if (v == 0x01) {
        mff_cycles(0xA021, 7);
        v = RAM(SUB_AREA);
        if (v == 0x01) {
            mff_cycles(0xA027, 5);              /* LDY #2; BNE $A02D */
            return 2;
        }
        mff_cycles(0xA025, 1);                  /* BNE $A02B */
    } else {
        mff_cycles(0xA01F, 1);                  /* BNE $A02B */
    }
    mff_cycles(0xA02B, 2);
    return 0;
}

/*
 * enemy_keep_on_band, $9FF6 (name proposed): unless in the air (phases 9
 * and $F), obj_y is kept in the walkable band of the area: at most top
 * goes to top + 1, below bottom goes to bottom ($A046/$A049 by
 * area_band: $81-$B0, $71-$A0 or $91-$B0). The routine starts in bank
 * $0C and runs on into bank $0D at $A001; it ends at the RTS at $A045.
 * In: X = slot. Out: A = the bottom or obj_state; C/N/Z of the last
 * compare, V from the ADC.
 * Caller: enemy_common, JSR at 0C:9D25.
 */
void func_9FF6_b6(void)
{
    REGS_IN;

    mff_cycles(0x9FF6, 8);
    a = RAM(OBJ_STATE + x);
    CMP_FLAGS(a, AI_JUMP);
    if (z) {
        mff_cycles(0x9FFB, 2);                  /* BEQ $A045, into the next page */
        mff_cycles(0xA045, 6);
        RTS_RETURN();
    }
    mff_cycles(0x9FFD, 4);
    CMP_FLAGS(a, AI_DROP_IN);
    if (z) {
        mff_cycles(0x9FFF, 1);                  /* BEQ $A045 */
        mff_cycles(0xA045, 6);
        RTS_RETURN();
    }

    y = area_band();
    mff_cycles(0xA02D, 4);
    a = prg_read((uint16_t)(BAND_TOP + y));
    mff_cycles(0xA030, 6);
    c = a >= RAM(OBJ_Y + x);
    if (c) {
        mff_cycles(0xA035, 7);
        ADC_A(0x00);                            /* top + 1 (C set) */
        RAM(OBJ_Y + x) = a;
    } else {
        mff_cycles(0xA033, 1);                  /* BCC $A03A */
    }
    mff_cycles(0xA03A, 4);
    a = prg_read((uint16_t)(BAND_BOTTOM + y));
    mff_cycles(0xA03D, 6);
    CMP_FLAGS(a, RAM(OBJ_Y + x));
    if (!c) {
        mff_cycles(0xA042, 5);
        RAM(OBJ_Y + x) = a;
    } else {
        mff_cycles(0xA040, 1);                  /* BCS $A045 */
    }
    mff_cycles(0xA045, 6);
    RTS_RETURN();
}
