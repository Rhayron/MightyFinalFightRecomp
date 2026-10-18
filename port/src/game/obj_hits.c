/*
 * obj_hits.c - what a player hit does to an object: AxL's block, the
 * damage per character and level and the bosses' cut (0C:84F0-87AA), and
 * the experience the player earns for a kill (0C:89E3-8ACD).
 * docs/objects.md, "Update" step 4.
 *
 * obj_take_hit runs from objects_update when the player's attack connects
 * and from the thrown-body behaviours when a body lands (they deal the
 * landing's damage as attack 5). The chain of jumps:
 *   obj_take_hit (AxL may block) -> obj_damage ($8593: HUD bar, special
 *   flag, hit cooldown) -> JMP ($87B0/$87B3[player_id]) = the character's
 *   hit sound ($85C8 Cody, $85F0 Guy, $8618 Haggar) -> JMP through the
 *   character's per-level table to the level's entry of the damage
 *   lookup ($8638-$8683) -> damage_apply ($8686: the bosses' cut and
 *   the subtraction from obj_life).
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
        a = prg_read_ix(TIER_INDEX, y);
        mff_cycles(0x8522, 11);
        y = a;
        a = RAM(RNG) & 0x0F;
        m = prg_read_ix(AXL_BLOCK_ODDS, y);
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
    a = prg_read_ix(DAMAGE_BY_PLAYER_LO, y);
    mff_cycles(0x85BE, 7);
    RAM(TMP2) = a;
    a = prg_read_ix(DAMAGE_BY_PLAYER_HI, y);
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
    a = prg_read_ix(lo, y);
    mff_cycles((uint16_t)(pc + 5), 7);
    RAM(TMP4) = a;
    a = prg_read_ix(hi, y);
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
 * The damage lookup, $8638-$8685: one LDA table,Y / BNE $8686 per
 * character and level (Y = attack_id), entered at the level's own entry
 * by the JMP ($0004) of damage_by_level. Cody's levels 0-5, Guy's 0-5
 * (levels 0 and 1 read the same table, so Guy's first level up adds no
 * damage), Haggar's 0-3. A zero would fall into the next entry's table;
 * no table holds one for attacks 0-$22, so the BNE is always taken and
 * Haggar's level 3 ($8683) is the only entry that falls into $8686.
 */
static const uint16_t k_damage_tables[] = {
    0x87D6, 0x87F9, 0x881C, 0x883F, 0x8862, 0x8885,   /* Cody,   $8638-$8651 */
    0x88A8, 0x88A8, 0x88CB, 0x88EE, 0x8911, 0x8934,   /* Guy,    $8656-$866F */
    0x8957, 0x897A, 0x899D, 0x89C0,                   /* Haggar, $8674-$8683 */
};
#define DAMAGE_LOOKUP       0x8638  /* entry i at $8638 + 5 * i */
#define DAMAGE_ENTRIES      16

/* the bosses' cut, by obj_kind - 7: Thrasher 2, Sodom 4, Abigail 4,
 * Sodom with obj_tier set 8 (index 3), Belger 8 (index 4) */
#define BOSS_DAMAGE_CUT     0x87AB
#define KIND_THRASHER       0x07
#define KIND_SODOM          0x08
#define KIND_ABIGAIL        0x09
#define KIND_BELGER         0x0B
static const uint8_t k_boss_kinds[] = { KIND_THRASHER, KIND_SODOM, KIND_ABIGAIL, KIND_BELGER };

/* Sodom's two pieces ($87A7: x offset, height per piece; slots $0D, $0B) */
#define SODOM_PIECES        0x87A7
#define KIND_SODOM_PIECE    0x0D
#define ANIM_SODOM_PIECE    0xFD    /* no experience when it "dies" (obj_knockdown.c) */

void func_F436(void);   /* attack_end */

static void damage_apply(void);
static void sodom_pieces(void);

/*
 * The lookup from entry `entry` on (the level's table), then the damage.
 * In: Y = attack_id, X = slot. Out: as damage_apply.
 */
static void damage_lookup(int entry)
{
    REGS_IN;
    int i;

    for (i = entry; ; i++) {
        uint16_t pc = (uint16_t)(DAMAGE_LOOKUP + 5 * i);

        mff_cycles(pc, 4);
        a = prg_read_ix(k_damage_tables[i], y);
        if (i == DAMAGE_ENTRIES - 1)
            break;                              /* $8683 falls into $8686 */
        mff_cycles((uint16_t)(pc + 3), 2);
        if (a != 0) {
            mff_cycles((uint16_t)(pc + 3), 1);  /* BNE $8686 */
            break;
        }
        /* never executed: a zero, on to the next table */
    }
    SYNC_OUT;
    damage_apply();
}

/*
 * damage_apply, $8686 (name proposed): A = the table's damage. Attacks 6
 * and $0C make slot X the combo's target. A boss takes less: Sodom 5 more
 * first for attacks 5 and $13 (never executed), then $87AB[kind - 7] off
 * (Sodom with obj_tier set: index 3), one more off in round 4 (the
 * fifth), never under 1. obj_life -= the damage. Sodom at obj_tier 1
 * whose life falls under $30 then sheds his pieces (sodom_pieces, never
 * executed).
 * In: A = damage, X = slot, Y = attack_id. Out: the RTS at $8702: A =
 * obj_kind or obj_life or obj_tier, flags of the CMP that ended it, V
 * from the subtraction; Y = attack_id, or the boss index.
 */
static void damage_apply(void)
{
    REGS_IN;
    int i, combo;

    mff_cycles(0x8686, 10);
    RAM(TMP0) = a;
    a = RAM(ATTACK_ID);
    combo = a == 0x06;
    if (combo) {
        mff_cycles(0x868C, 1);
    } else {
        mff_cycles(0x868E, 4);
        combo = a == 0x0C;
        if (!combo)
            mff_cycles(0x8690, 1);
    }
    if (combo) {
        mff_cycles(0x8692, 3);
        RAM(COMBO_TARGET) = x;
    }

    /* the bosses: the BEQs all stay in page $86 */
    mff_cycles(0x8694, 4);
    a = RAM(OBJ_KIND + x);
    i = cmp_list(0x8697, 0x86B8, a, k_boss_kinds, 4);
    if (i == 4) {
        mff_cycles(0x86A7, 3);                  /* BNE $86E4 */
    } else {
        if (a == KIND_SODOM) {
            int more;

            /* never executed */
            mff_cycles(0x86A9, 4);
            more = y == 0x05;
            if (more) {
                mff_cycles(0x86AB, 1);
            } else {
                mff_cycles(0x86AD, 4);
                more = y == 0x13;
                if (!more)
                    mff_cycles(0x86AF, 1);
            }
            if (more) {
                mff_cycles(0x86B1, 10);
                RAM(TMP0) = (uint8_t)(RAM(TMP0) + 0x05);
            }
        }
        mff_cycles(0x86B8, 14);
        a = RAM(OBJ_KIND + x);
        c = 1;
        SBC_A(0x07);
        y = a;
        if (a == 0x01) {
            mff_cycles(0x86C3, 6);
            a = RAM(OBJ_TIER + x);
            if (a != 0) {
                mff_cycles(0x86C8, 2);
                y = 0x03;
            } else {
                mff_cycles(0x86C6, 1);
            }
        } else {
            mff_cycles(0x86C1, 1);
        }
        mff_cycles(0x86CA, 7);
        a = RAM(ROUND);
        if (a == 0x04) {
            mff_cycles(0x86D0, 5);
            RAM(TMP0) = (uint8_t)(RAM(TMP0) - 1);
        } else {
            mff_cycles(0x86CE, 1);
        }
        mff_cycles(0x86D2, 9);
        a = RAM(TMP0);
        c = 1;
        SBC_A(prg_read_ix(BOSS_DAMAGE_CUT, y));
        SET_NZ(a);
        mff_cycles(0x86D8, 5);
        RAM(TMP0) = a;
        if (z) {
            mff_cycles(0x86DA, 1);
        } else {
            mff_cycles(0x86DC, 2);
            if (n)
                mff_cycles(0x86DC, 1);
            else
                mff_cycles(0x86DE, 3);          /* BPL $86E4 */
        }
        if (z || n) {
            mff_cycles(0x86E0, 5);
            a = 0x01;
            RAM(TMP0) = a;
        }
    }

    /* the hit */
    mff_cycles(0x86E4, 22);
    a = RAM(OBJ_LIFE + x);
    c = 1;
    SBC_A(RAM(TMP0));
    RAM(OBJ_LIFE + x) = a;
    a = RAM(OBJ_KIND + x);
    CMP_FLAGS(a, KIND_SODOM);
    if (!z) {
        mff_cycles(0x86F2, 2);
    } else {
        mff_cycles(0x86F4, 8);
        a = RAM(OBJ_LIFE + x);
        CMP_FLAGS(a, 0x30);
        if (c) {
            mff_cycles(0x86F9, 2);
        } else {
            mff_cycles(0x86FB, 8);
            a = RAM(OBJ_TIER + x);
            CMP_FLAGS(a, 0x01);
            if (z) {
                mff_cycles(0x8700, 1);
                SYNC_OUT;
                sodom_pieces();
                return;
            }
        }
    }
    mff_cycles(0x8702, 6);
    RTS_RETURN();
}

/*
 * sodom_pieces, $8703 (name proposed; never executed): a player holding
 * the boss (game_action 4 or $21) lets go (grab state cleared, attack_end,
 * the player's obj_state $80 and standing animation $E288[player_id],
 * player_stand at 0E:CEAA) and the boss comes down to the player's obj_y.
 * Then slots $0D and $0B become two dying objects (id 6, kind $0D,
 * animation $FD, obj_life $FF) beside him: x + $0C and $0C up, x and 4 up
 * ($87A7), obj_land_y = his obj_y + $0E, obj_anim_frame = slot & 3; a
 * piece whose x high byte differs from the player's is not made (only
 * obj_dir, obj_y_hi and the position are written).
 * In: X = slot. Out: the RTS at $87A6, A = 9, Y = 2, flags of CMP #9.
 */
static void sodom_pieces(void)
{
    REGS_IN;
    int release;

    mff_cycles(0x8703, 7);
    a = RAM(GAME_ACTION);
    c = a >= 0x04;                              /* attack_end and player_stand pass C on */
    release = a == 0x04;
    if (release) {
        mff_cycles(0x8707, 1);
    } else {
        mff_cycles(0x8709, 4);
        c = a >= 0x21;
        release = a == 0x21;
        if (!release)
            mff_cycles(0x870B, 1);
    }
    if (release) {
        mff_cycles(0x870D, 17);
        a = 0x00;
        SET_NZ(a);
        RAM(GRAB_TIMER) = a;
        RAM(GAME_ACTION) = a;
        RAM(GRAB_HIT_COUNT) = a;
        CALL_FIXED(0x8715, func_F436);          /* attack_end */
        mff_cycles(0x8718, 13);
        RAM(OBJ_STATE) = OBJ_LIVE;              /* slot 0, the player */
        y = RAM(PLAYER_ID);
        a = prg_read_ix(0xE288, y);
        SET_NZ(a);
        mff_cycles(0x8722, 9);
        RAM(PLAYER_STAND_ANIM) = a;
        CALL_BANK(0x8724, 0xCEAA);              /* player_stand */
        mff_cycles(0x8727, 9);
        a = RAM(OBJ_Y);
        RAM(OBJ_Y + x) = a;
    }

    mff_cycles(0x872D, 7);
    RAM(TMP0) = 0x0D;                           /* the slot */
    y = 0x00;                                   /* the piece's offsets */
    for (;;) {
        mff_cycles(0x8733, 4);
        a = prg_read_ix(SODOM_PIECES, y);
        mff_cycles(0x8736, 7);
        RAM(TMP1) = a;
        a = prg_read_ix(SODOM_PIECES + 1, y);
        mff_cycles(0x873B, 55);
        RAM(TMP2) = a;
        y = RAM(TMP0);
        RAM(OBJ_DIR + y) = RAM(OBJ_DIR + x);
        RAM(OBJ_Y_HI + y) = RAM(OBJ_Y_HI + x);
        a = RAM(OBJ_X + x);
        c = 0;
        ADC_A(RAM(TMP1));
        RAM(OBJ_X + y) = a;
        a = RAM(OBJ_X_HI + x);
        ADC_A(0x00);
        RAM(OBJ_X_HI + y) = a;
        if (a == RAM(OBJ_X_HI)) {
            mff_cycles(0x8761, 83);
            a = RAM(OBJ_Y + x);
            c = 0;
            ADC_A(0x0E);
            RAM(OBJ_LAND_Y + y) = a;
            a = RAM(OBJ_Y + x);
            c = 1;
            SBC_A(RAM(TMP2));
            RAM(OBJ_Y + y) = a;
            RAM(OBJ_ANIM + y) = ANIM_SODOM_PIECE;
            RAM(OBJ_LIFE + y) = 0xFF;
            RAM(OBJ_KIND + y) = KIND_SODOM_PIECE;
            RAM(OBJ_ID + y) = OBJ_KILLED_LIGHT;
            RAM(OBJ_SAVED_ID + y) = OBJ_KILLED_LIGHT;
            RAM(OBJ_STATE + y) = OBJ_LIVE;
            RAM(OBJ_ANIM_TIMER + y) = 0x00;
            a = y & 0x03;
            RAM(OBJ_ANIM_FRAME + y) = a;
        } else {
            mff_cycles(0x875F, 1);              /* BNE $879A */
        }
        mff_cycles(0x879A, 19);
        y = 0x02;
        RAM(TMP0) = (uint8_t)(RAM(TMP0) - 2);
        a = RAM(TMP0);
        CMP_FLAGS(a, 0x09);
        if (z)
            break;
        mff_cycles(0x87A4, 1);                  /* BNE $8733 */
    }
    mff_cycles(0x87A6, 6);
    RTS_RETURN();
}

/*
 * damage_cody_0 ... damage_haggar_3, $8638-$8683 (names proposed): the
 * damage lookup entered at a level's table.
 * Callers: JMP ($0004) at 0C:85ED (Cody, $87B6/$87BC by player_level),
 * 0C:8615 (Guy, $87C2/$87C8), 0C:8635 (Haggar, $87CE/$87D2).
 */
void func_8638_b6(void) { damage_lookup(0); }
void func_863D_b6(void) { damage_lookup(1); }
void func_8642_b6(void) { damage_lookup(2); }
void func_8647_b6(void) { damage_lookup(3); }
void func_864C_b6(void) { damage_lookup(4); }   /* Cody level 4: never executed */
void func_8651_b6(void) { damage_lookup(5); }   /* Cody level 5: never executed */
void func_8656_b6(void) { damage_lookup(6); }
void func_865B_b6(void) { damage_lookup(7); }
void func_8660_b6(void) { damage_lookup(8); }
void func_8665_b6(void) { damage_lookup(9); }
void func_866A_b6(void) { damage_lookup(10); }
void func_866F_b6(void) { damage_lookup(11); }  /* Guy level 5: never executed */
void func_8674_b6(void) { damage_lookup(12); }
void func_8679_b6(void) { damage_lookup(13); }
void func_867E_b6(void) { damage_lookup(14); }  /* Haggar level 2: never executed */
void func_8683_b6(void) { damage_lookup(15); }  /* Haggar level 3: never executed */

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
    a = prg_read_ix(EXP_CHECK_LO, y);
    mff_cycles(0x89E8, 7);
    RAM(TMP3) = a;
    a = prg_read_ix(EXP_CHECK_HI, y);
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
    a = prg_read_ix(lo, y);
    mff_cycles((uint16_t)(pc + 12), 7);
    RAM(TMP8) = a;
    a = prg_read_ix(hi, y);
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
        a = prg_read_ix(EXP_POINTS, y) & 0x0F;
        mff_cycles(0x8A59, 5);
        RAM(TMP10) = a;
    }
    mff_cycles(0x8A5D, 4);
    a = prg_read_ix(EXP_POINTS, y) & 0xF0;
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
        a = prg_read_ix(LEVEL_HEALTH_LO, y);
        mff_cycles(0x8AA9, 7);
        RAM(TMP12) = a;
        a = prg_read_ix(LEVEL_HEALTH_HI, y);
        mff_cycles(0x8AAE, 11);
        RAM(TMP13) = a;
        y = RAM(PLAYER_LEVEL);
        a = mem_read_iy(TMP12, y);
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
