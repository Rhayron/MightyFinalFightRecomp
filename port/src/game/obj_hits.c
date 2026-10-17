/*
 * obj_hits.c - what a player hit does to an object: AxL's block, the
 * damage per character and level (0C:84F0-8635), and the experience the
 * player earns for a kill (0C:89E3-8ACD). docs/objects.md, "Update" step 4.
 *
 * obj_take_hit runs from objects_update when the player's attack connects
 * and from the thrown-body behaviours when a body lands (they deal the
 * landing's damage as attack 5). The chain of jumps:
 *   obj_take_hit (AxL may block) -> obj_damage ($8593: HUD bar, special
 *   flag, hit cooldown) -> JMP ($87B0/$87B3[player_id]) = the character's
 *   hit sound ($85C8 Cody, $85F0 Guy, $8618 Haggar) -> JMP through the
 *   character's per-level table to the damage lookup at $8638-$8685 (no
 *   function of the scaffold: the interpreter runs it and the subtraction
 *   at $8686).
 * exp_award ($89E3) is called by the dying behaviours: JMP
 * ($8ACE/$8AD1[player_id]) to the character's level check, which loads
 * the next level's threshold and adds the attack's points ($8A31).
 */
#include "obj_defs.h"

/* sound effects (sound_queue_push, 0F:F34C) */
#define SFX_LEVEL_UP        0x14
#define SFX_HIT             0x18    /* the usual hit */
#define SFX_BLOCK           0x26    /* AxL's block (also the item pickup, 0C:8B66) */

/* AxL: phase 3 of his behaviour is the block (obj_state $83) */
#define AXL_BLOCKING        0x83
#define AXL_BLOCK_ODDS      0x8590  /* by tier 0/1/2: blocks when (rng & $0F) is below it */
#define TIER_INDEX          0xA897  /* bank 0D: obj_tier 1/2/4 -> 0/1/2 */
#define ACTION_CODY_POWER   0x23
#define ACTION_SPECIAL      0x17

/* the damage routines: per character, then per level */
#define DAMAGE_BY_PLAYER_LO 0x87B0
#define DAMAGE_BY_PLAYER_HI 0x87B3

/* experience: points per attack (BCD, bank 0F), the level check per
 * character, the health a new level gives (pointers per character) */
#define EXP_POINTS          0xFC10
#define EXP_CHECK_LO        0x8ACE
#define EXP_CHECK_HI        0x8AD1
#define LEVEL_HEALTH_LO     0x8BAA
#define LEVEL_HEALTH_HI     0x8BAD

/* attacks AxL can block ($8534-$855E), and whether the block then lasts
 * 8 frames ($8564) rather than 3 ($8560); Cody's power (game_action $23)
 * gets 8, attack $1A (tested last, with a BNE) 3 */
static const uint8_t k_block_attacks[] = { 0x00, 0x01, 0x03, 0x04, 0x06, 0x07, 0x0B, 0x0C, 0x0D, 0x18 };
static const uint8_t k_block_long[]    = { 0,    1,    1,    0,    0,    1,    0,    0,    1,    1    };

/*
 * obj_take_hit, $84F0: a player hit on slot X. Anyone but AxL (kind 3)
 * takes the damage (JMP $8593). AxL takes it too in phases 5 and 8 and
 * while he is not in his own behaviour (obj_id != obj_saved_id: knocked
 * down, held). Blocking already (phase 3), he takes nothing (RTS) unless
 * his block timer is at 2, when he blocks again; otherwise he blocks when
 * (rng & $0F) is below the odds of his tier ($8590: 8, $0A, $0E), and only
 * the attacks in k_block_attacks, Cody's power and attack $1A can be
 * blocked. A block: obj_ai_timer = 3 or 8 frames, his life bar on the HUD
 * (hud_enemy_slot), sound $26, phase 3 with obj_timer and obj_hit_timer
 * cleared (no knockback), combo_target = X for attacks 6 and $0C, then
 * JMP 0F:F343 (turn to the player).
 * In: X = slot. Out: as obj_damage or 0F:F343, or the RTS (A = obj_timer,
 * flags of CMP #2).
 * Callers: objects_update (JSR at 0C:80D3) and the thrown-body landings
 * (0C:8F68, 9149, 927A, 970E, 9939).
 */
void func_84F0_b6(void)
{
    REGS_IN;
    int i, block_long;

    mff_cycles(0x84F0, 8);
    a = RAM(OBJ_KIND + x);
    CMP_FLAGS(a, KIND_AXL);
    if (!z)
        JMP_TAIL(0x84F7, 0x8593);
    mff_cycles(0x84F5, 1);

    mff_cycles(0x84FA, 8);
    a = RAM(OBJ_STATE + x);
    CMP_FLAGS(a, 0x85);
    if (z) {
        mff_cycles(0x84FF, 2);
        JMP_TAIL(0x84F7, 0x8593);
    }
    mff_cycles(0x8501, 4);
    CMP_FLAGS(a, 0x88);
    if (z) {
        mff_cycles(0x8503, 2);
        JMP_TAIL(0x84F7, 0x8593);
    }
    mff_cycles(0x8505, 10);
    a = RAM(OBJ_ID + x);
    CMP_FLAGS(a, RAM(OBJ_SAVED_ID + x));
    if (!z) {
        mff_cycles(0x850B, 2);
        JMP_TAIL(0x84F7, 0x8593);
    }

    mff_cycles(0x850D, 8);
    a = RAM(OBJ_STATE + x);
    if (a == AXL_BLOCKING) {
        mff_cycles(0x8514, 8);
        a = RAM(OBJ_TIMER + x);
        CMP_FLAGS(a, 0x02);
        if (!z) {
            mff_cycles(0x851B, 6);
            RTS_RETURN();
        }
        mff_cycles(0x8519, 1);
    } else {
        mff_cycles(0x8512, 1);
        mff_cycles(0x851C, 8);
        y = RAM(OBJ_TIER + x);
        a = prg_read((uint16_t)(TIER_INDEX + y));
        mff_cycles(0x8522, 11);
        y = a;
        a = RAM(RNG) & 0x0F;
        m = prg_read((uint16_t)(AXL_BLOCK_ODDS + y));
        c = a >= m;
        mff_cycles(0x852A, 2);
        if (c) {
            mff_cycles(0x852A, 1);
            SYNC_OUT;
            func_8593_b6();
            return;
        }
    }

    /* the attack decides whether he blocks, and for how long */
    mff_cycles(0x852C, 7);
    a = RAM(GAME_ACTION);
    c = a >= ACTION_CODY_POWER;
    if (a == ACTION_CODY_POWER) {
        mff_cycles(0x8530, 1);
        block_long = 1;
    } else {
        mff_cycles(0x8532, 3);
        a = RAM(ATTACK_ID);
        i = cmp_list(0x8534, 0x8560, a, k_block_attacks, 10);
        if (i < 10) {
            c = 1;
            block_long = k_block_long[i];
        } else {
            mff_cycles(0x855C, 4);
            c = a >= 0x1A;
            if (a != 0x1A) {
                mff_cycles(0x855E, 1);
                SYNC_OUT;
                func_8593_b6();
                return;
            }
            block_long = 0;
        }
    }
    if (block_long) {
        mff_cycles(0x8564, 2);
        a = 8;
    } else {
        mff_cycles(0x8560, 5);                  /* LDA #3; BNE taken */
        a = 3;
    }
    mff_cycles(0x8566, 21);
    RAM(OBJ_AI_TIMER + x) = a;
    RAM(HUD_ENEMY_SLOT) = x;
    RAM(HUD_ENEMY_TIMER) = 0;
    a = SFX_BLOCK;
    SET_NZ(a);
    CALL_FIXED(0x8571, func_F34C);
    mff_cycles(0x8574, 26);
    RAM(OBJ_STATE + x) = AXL_BLOCKING;
    RAM(OBJ_TIMER + x) = 0;
    RAM(OBJ_HIT_TIMER + x) = 0;
    /* attacks 6 and $0C make him the combo's target */
    a = RAM(ATTACK_ID);
    CMP_FLAGS(a, 0x06);
    if (z) {
        mff_cycles(0x8585, 1);
    } else {
        mff_cycles(0x8587, 4);
        CMP_FLAGS(a, 0x0C);
        if (!z)
            mff_cycles(0x8589, 1);
    }
    if (z) {
        mff_cycles(0x858B, 3);
        RAM(COMBO_TARGET) = x;
    }
    JMP_FIXED(0x858D, 0xF343, func_F343);
}

/*
 * obj_damage, $8593 (name proposed): the hit lands on slot X. Unless the
 * slot has id 0 its life bar goes on the HUD (hud_enemy_slot = X,
 * hud_enemy_timer = 0), and special_hit ($BF) records Cody/Guy's special
 * power (game_action $17) or Haggar's game_action $16. (The BNE after the
 * first STA never branches: Z is still set from the CMP.) 0F:FB09 loads
 * the hit cooldown and obj_hit_timer, then JMP ($87B0/$87B3[player_id]).
 * In: X = slot. Out: TMP2/TMP3 = the routine jumped to; A = its high byte.
 * Callers: JMP at 0C:84F7; the BCS at 0C:852A and BNE at 0C:855E.
 */
void func_8593_b6(void)
{
    REGS_IN;

    mff_cycles(0x8593, 6);
    a = RAM(OBJ_ID + x);
    SET_NZ(a);
    if (!z) {
        mff_cycles(0x8598, 15);
        RAM(HUD_ENEMY_SLOT) = x;
        a = 0;
        RAM(HUD_ENEMY_TIMER) = a;
        a = RAM(GAME_ACTION);
        CMP_FLAGS(a, ACTION_SPECIAL);
        if (z) {
            mff_cycles(0x85A4, 5);
            RAM(SPECIAL_HIT) = a;
        } else {
            mff_cycles(0x85A2, 1);
        }
        mff_cycles(0x85A8, 7);
        a = RAM(PLAYER_ID);
        CMP_FLAGS(a, PLAYER_HAGGAR);
        if (z) {
            mff_cycles(0x85AE, 7);
            a = RAM(GAME_ACTION);
            CMP_FLAGS(a, 0x16);
            if (z) {
                mff_cycles(0x85B4, 3);
                RAM(SPECIAL_HIT) = a;
            } else {
                mff_cycles(0x85B2, 1);
            }
        } else {
            mff_cycles(0x85AC, 1);
        }
    } else {
        mff_cycles(0x8596, 1);
    }
    mff_cycles(0x85B6, 6);
    CALL_FIXED(0x85B6, func_FB09);
    mff_cycles(0x85B9, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(DAMAGE_BY_PLAYER_LO + y));
    mff_cycles(0x85BE, 7);
    RAM(TMP2) = a;
    a = prg_read((uint16_t)(DAMAGE_BY_PLAYER_HI + y));
    SET_NZ(a);
    mff_cycles(0x85C3, 8);
    RAM(TMP3) = a;
    JMP_INDIRECT(0x0002);
}

/*
 * The end the three characters' damage routines share ($85DF, $8607,
 * $8627, at `pc`): TMP4/TMP5 = the damage lookup of the player's level
 * (tables `lo`/`hi` by player_level), Y = attack_id, JMP ($0004).
 */
static void damage_by_level(uint16_t pc, uint16_t lo, uint16_t hi)
{
    REGS_IN;

    mff_cycles(pc, 7);
    y = RAM(PLAYER_LEVEL);
    a = prg_read((uint16_t)(lo + y));
    mff_cycles((uint16_t)(pc + 5), 7);
    RAM(TMP4) = a;
    a = prg_read((uint16_t)(hi + y));
    mff_cycles((uint16_t)(pc + 10), 11);
    RAM(TMP5) = a;
    y = RAM(ATTACK_ID);
    SET_NZ(y);
    JMP_INDIRECT(0x0004);
}

/*
 * damage_cody, $85C8 (name proposed): Cody's hit sound ($19 for attack 7,
 * $1F for attack 8, never executed, else $18), then his level's damage.
 * In: X = slot. Out: as the damage lookup.
 * Caller: JMP ($0002) at 0C:85C5 (table $87B0/$87B3, player_id 0).
 */
void func_85C8_b6(void)
{
    REGS_IN;

    mff_cycles(0x85C8, 7);
    a = RAM(ATTACK_ID);
    c = a >= 0x07;
    if (a == 0x07) {
        mff_cycles(0x85CE, 5);
        a = 0x19;
    } else {
        mff_cycles(0x85CC, 1);
        mff_cycles(0x85D2, 4);
        c = a >= 0x08;
        if (a == 0x08) {
            mff_cycles(0x85D6, 5);
            a = 0x1F;
        } else {
            mff_cycles(0x85D4, 1);
            mff_cycles(0x85DA, 2);
            a = SFX_HIT;
        }
    }
    SET_NZ(a);
    mff_cycles(0x85DC, 6);
    CALL_FIXED(0x85DC, func_F34C);
    SYNC_OUT;
    damage_by_level(0x85DF, 0x87B6, 0x87BC);
}

/*
 * damage_guy, $85F0 (name proposed): Guy's hit sound ($1A for attack 1,
 * $1F for attack 8, never executed, else $18), then his level's damage.
 * In: X = slot. Out: as the damage lookup.
 * Caller: JMP ($0002) at 0C:85C5 (player_id 1).
 */
void func_85F0_b6(void)
{
    REGS_IN;

    mff_cycles(0x85F0, 7);
    a = RAM(ATTACK_ID);
    c = a >= 0x01;
    if (a == 0x01) {
        mff_cycles(0x85F6, 6);                  /* the BNE crosses into $86 */
        a = 0x1A;
    } else {
        mff_cycles(0x85F4, 1);
        mff_cycles(0x85FA, 4);
        c = a >= 0x08;
        if (a == 0x08) {
            mff_cycles(0x85FE, 5);
            a = 0x1F;
        } else {
            mff_cycles(0x85FC, 2);
            mff_cycles(0x8602, 2);
            a = SFX_HIT;
        }
    }
    SET_NZ(a);
    mff_cycles(0x8604, 6);
    CALL_FIXED(0x8604, func_F34C);
    SYNC_OUT;
    damage_by_level(0x8607, 0x87C2, 0x87C8);
}

/*
 * damage_haggar, $8618 (name proposed): Haggar's hit sound ($27 for attack
 * $0D, else $18), then his level's damage.
 * In: X = slot. Out: as the damage lookup.
 * Caller: JMP ($0002) at 0C:85C5 (player_id 2).
 */
void func_8618_b6(void)
{
    REGS_IN;

    mff_cycles(0x8618, 7);
    a = RAM(ATTACK_ID);
    c = a >= 0x0D;
    if (a == 0x0D) {
        mff_cycles(0x861E, 5);
        a = 0x27;
    } else {
        mff_cycles(0x861C, 1);
        mff_cycles(0x8622, 2);
        a = SFX_HIT;
    }
    SET_NZ(a);
    mff_cycles(0x8624, 6);
    CALL_FIXED(0x8624, func_F34C);
    SYNC_OUT;
    damage_by_level(0x8627, 0x87CE, 0x87D2);
}

/*
 * exp_award, $89E3 (name proposed): the player earns the points of the
 * attack that killed: JMP ($8ACE/$8AD1[player_id]) to the character's
 * level check ($89F2 Cody, $8A08 Guy, $8A1E Haggar).
 * In: nothing. Out: TMP3/TMP4 = the routine; A = its high byte.
 * Callers: the dying and thrown-body behaviours, JSR at 0C:8BA3, 9282,
 * 9716, 9983, 9A5C, 9BC0.
 */
void func_89E3_b6(void)
{
    REGS_IN;

    mff_cycles(0x89E3, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(EXP_CHECK_LO + y));
    mff_cycles(0x89E8, 7);
    RAM(TMP3) = a;
    a = prg_read((uint16_t)(EXP_CHECK_HI + y));
    SET_NZ(a);
    mff_cycles(0x89ED, 8);
    RAM(TMP4) = a;
    JMP_INDIRECT(0x0003);
}

/*
 * The level check the characters share ($89F2, $8A08, $8A1E, at `pc`):
 * at the top level (`top`) nothing more (RTS); otherwise TMP8/TMP9 = the
 * next level's threshold from the character's tables `lo`/`hi` (BCD:
 * tens/ones, hundreds) and the points are added (exp_add, $8A31): by a JMP
 * for Cody and Guy, by falling into it for Haggar.
 */
static void exp_check_level(uint16_t pc, uint8_t top, uint16_t lo, uint16_t hi, int falls)
{
    REGS_IN;

    mff_cycles(pc, 7);
    a = RAM(PLAYER_LEVEL);
    CMP_FLAGS(a, top);
    if (z) {
        mff_cycles((uint16_t)(pc + 6), 6);
        RTS_RETURN();
    }
    mff_cycles((uint16_t)(pc + 4), 1);
    mff_cycles((uint16_t)(pc + 7), 7);
    y = RAM(PLAYER_LEVEL);
    a = prg_read((uint16_t)(lo + y));
    mff_cycles((uint16_t)(pc + 12), 7);
    RAM(TMP8) = a;
    a = prg_read((uint16_t)(hi + y));
    SET_NZ(a);
    mff_cycles((uint16_t)(pc + 17), 3);
    RAM(TMP9) = a;
    if (!falls)
        JMP_TAIL((uint16_t)(pc + 19), 0x8A31);
    SYNC_OUT;
    func_8A31_b6();
}

/*
 * exp_check_cody, $89F2 (name proposed): Cody's levels end at 5.
 * Caller: JMP ($0003) at 0C:89EF (player_id 0).
 */
void func_89F2_b6(void)
{
    exp_check_level(0x89F2, 5, 0x8AD4, 0x8AD9, 0);
}

/*
 * $89F8: an RTS: the end of Cody's level check at the top level, and the
 * behaviour of ids 1 and 9 (the hit spark only animates).
 * Callers: obj_behavior_lo/hi (ids 1, 9); the fall-through at 0C:89F6.
 */
void func_89F8_b6(void)
{
    mff_cycles(0x89F8, 6);
    mff_rts();
}

/*
 * exp_check_guy, $8A08 (name proposed): Guy's levels end at 5.
 * Caller: JMP ($0003) at 0C:89EF (player_id 1).
 */
void func_8A08_b6(void)
{
    exp_check_level(0x8A08, 5, 0x8ADE, 0x8AE3, 0);
}

/*
 * exp_check_haggar, $8A1E (name proposed): Haggar's levels end at 3.
 * Caller: JMP ($0003) at 0C:89EF (player_id 2).
 */
void func_8A1E_b6(void)
{
    exp_check_level(0x8A1E, 3, 0x8AE8, 0x8AEB, 1);
}

/*
 * exp_add, $8A31 (name proposed): adds $FC10[attack_id] (BCD, two digits;
 * 5 for attack 3 while the player shows animation $24, never executed) to
 * player_exp (3 BCD digits, $6C/$6D), digit by digit in TMP5 (ones), TMP6
 * (tens, high nibble) and TMP7 (hundreds) with TMP10/TMP11 as the points.
 * Reaching TMP9:TMP8 (the threshold) is a level up: player_level + 1,
 * health refilled from the character's table for the new level, sound $14,
 * player_exp = 0. Otherwise player_exp takes the new digits.
 * In: TMP8/TMP9. Out: A = 0 after a level up, else the hundreds digit;
 * N/Z from A.
 * Callers: JMP at 0C:8A05 and 0C:8A1B; the fall-through from 0C:8A2F.
 */
void func_8A31_b6(void)
{
    REGS_IN;
    int special, level_up;

    mff_cycles(0x8A31, 31);
    a = RAM(PLAYER_EXP) & 0x0F;
    RAM(TMP5) = a;
    a = RAM(PLAYER_EXP) & 0xF0;
    RAM(TMP6) = a;
    a = RAM(PLAYER_EXP + 1) & 0x0F;
    RAM(TMP7) = a;
    y = RAM(ATTACK_ID);
    special = 0;
    if (y == 0x03) {
        mff_cycles(0x8A49, 8);
        a = RAM(OBJ_ANIM);                      /* slot 0, the player */
        special = a == 0x24;
        if (special) {
            mff_cycles(0x8A50, 8);
            a = 0x05;
            RAM(TMP10) = a;
        } else {
            mff_cycles(0x8A4E, 1);
        }
    } else {
        mff_cycles(0x8A47, 1);
    }
    if (!special) {
        mff_cycles(0x8A56, 4);
        a = prg_read((uint16_t)(EXP_POINTS + y)) & 0x0F;
        mff_cycles(0x8A59, 5);
        RAM(TMP10) = a;
    }
    mff_cycles(0x8A5D, 4);
    a = prg_read((uint16_t)(EXP_POINTS + y)) & 0xF0;
    mff_cycles(0x8A60, 20);
    RAM(TMP11) = a;

    /* ones, carrying into the points' tens */
    a = RAM(TMP5);
    c = 0;
    ADC_A(RAM(TMP10));
    RAM(TMP5) = a;
    if (a >= 0x0A) {
        mff_cycles(0x8A6F, 17);
        c = 1;
        SBC_A(0x0A);
        RAM(TMP5) = a;
        a = RAM(TMP11);
        c = 0;
        ADC_A(0x10);
        RAM(TMP11) = a;
    } else {
        mff_cycles(0x8A6D, 1);
    }
    /* tens, carrying into the hundreds */
    mff_cycles(0x8A7B, 15);
    a = RAM(TMP6);
    c = 0;
    ADC_A(RAM(TMP11));
    RAM(TMP6) = a;
    if (a >= 0xA0) {
        mff_cycles(0x8A86, 12);
        c = 1;
        SBC_A(0xA0);
        RAM(TMP6) = a;
        RAM(TMP7) = (uint8_t)(RAM(TMP7) + 1);
    } else {
        mff_cycles(0x8A84, 1);
    }

    /* against the threshold, hundreds first */
    mff_cycles(0x8A8D, 8);
    a = RAM(TMP7);
    if (a < RAM(TMP9)) {
        mff_cycles(0x8A91, 1);
        level_up = 0;
    } else {
        mff_cycles(0x8A93, 8);
        a = RAM(TMP7);
        m = RAM(TMP9);
        c = a >= m;
        level_up = a != m;
        if (level_up) {
            mff_cycles(0x8A97, 1);
        } else {
            mff_cycles(0x8A99, 13);
            a = RAM(TMP6);
            c = 0;
            ADC_A(RAM(TMP5));
            c = a >= RAM(TMP8);
            level_up = c;
            if (!level_up)
                mff_cycles(0x8AA0, 1);
        }
    }

    if (level_up) {
        mff_cycles(0x8AA2, 12);
        RAM(PLAYER_LEVEL) = (uint8_t)(RAM(PLAYER_LEVEL) + 1);
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(LEVEL_HEALTH_LO + y));
        mff_cycles(0x8AA9, 7);
        RAM(TMP12) = a;
        a = prg_read((uint16_t)(LEVEL_HEALTH_HI + y));
        mff_cycles(0x8AAE, 11);
        RAM(TMP13) = a;
        y = RAM(PLAYER_LEVEL);
        a = mem_read((uint16_t)(RAM16(TMP12) + y));
        mff_cycles(0x8AB4, 11);
        RAM(HEALTH) = a;
        a = SFX_LEVEL_UP;
        SET_NZ(a);
        CALL_FIXED(0x8AB8, func_F34C);
        mff_cycles(0x8ABB, 14);
        a = 0;
        SET_NZ(a);
        RAM(PLAYER_EXP) = a;
        RAM(PLAYER_EXP + 1) = a;
        RTS_RETURN();
    }
    mff_cycles(0x8AC2, 23);
    a = RAM(TMP5);
    c = 0;
    ADC_A(RAM(TMP6));
    RAM(PLAYER_EXP) = a;
    a = RAM(TMP7);
    SET_NZ(a);
    RAM(PLAYER_EXP + 1) = a;
    RTS_RETURN();
}
