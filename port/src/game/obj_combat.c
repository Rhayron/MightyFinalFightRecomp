/*
 * obj_combat.c - the fixed bank's pieces of a hit (docs/objects.md,
 * "Update: objects_update"): the window in which the player's attack can
 * hit (attack_live), the spark where it lands, the hit an object takes
 * (obj_hit_start), the hit the player takes (player_take_hit) and the
 * weapon he drops then (drop_weapon).
 */
#include "obj_fixed.h"

#define ATTACK_LIVE_FRAMES      0x1E    /* attack_live while the attack can hit */
#define SFX_SWING               0x17    /* an attack goes out */
#define HIT_TIMER_START         0x1E    /* obj_hit_timer when a player hit lands */

/* spawn_hit_spark */
#define ANIM_NO_SPARK           0x9D    /* a slot showing it gets no spark */
#define ANIM_SPARK              0x7E
#define ATTACK_SIDE_FIRST       0x0F    /* attacks $0F-$16: the spark on the target's side */
#define ATTACK_SIDE_END         0x17
#define ATTACK_HAGGAR_GRAB_1    0x1B    /* Haggar's grab hits: the spark higher */
#define ATTACK_HAGGAR_GRAB_2    0x1C
#define ATTACK_HAGGAR_GRAB_3    0x1D
/* the spark's x from the player's: +/- $12 (Cody, Guy), +/- $13 (Haggar),
 * +/- $1C toward the target (attacks $0F-$16); 16-bit, indexed by
 * player_dir & 2 or 0/2 */
#define SPARK_DX                0xF54C  /* 12 00 EE FF */
#define SPARK_DX_HAGGAR         0xF550  /* 13 00 ED FF */
#define SPARK_DX_SIDE           0xF554  /* 1C 00 E4 FF */
#define SPARK_DY_HIGH           0x10    /* above the player's y */
#define SPARK_DY                0x04    /* below it */

/* obj_hit_start: obj_hit_cooldown by attack_id */
#define HIT_COOLDOWN            0xFB17

/* player_take_hit */
#define DAMAGE_HEALTH           0xFD16  /* health lost, by obj_attack_damage */
#define DAMAGE_KNOCKS_DOWN      0xFD56  /* nonzero: the hit knocks the player down */
#define PLAYER_ANIM_HURT        0xE207  /* by player_id: 25 26 27 */
#define PLAYER_ANIM_KNOCKBACK   0xE20A  /* 28 29 2A */
#define PLAYER_CHR_HURT         0xE24F  /* 0D 15 11 */
#define PLAYER_CHR_KNOCKBACK    0xE252  /* 0D 15 13 */
#define ANIM_SLOT12_74          0x74    /* a slot 1/2 object a player hit removes */
#define OBJF_PLAYER_LEFT        (OBJF_ON_SCREEN | OBJF_WORLD)  /* the player's obj_flags facing left */
#define NO_HIT_BIT              0x80    /* player_no_hit */

/* drop_weapon */
#define SLOT_WEAPON             0x01
#define DROPPED_WEAPON_ANIM     0xFB06  /* by player_id: 41 41 51 */
#define DROPPED_WEAPON_ATTACK   0x2F

/* ---- the player's attack window ---- */

/*
 * attack_start, $F428: attack_live = $1E, attack_id 0, and the swing
 * sound (the code of attack_swing_sound, in one run).
 * Out: A, C, V as sound_queue_push_sfx leaves them; X, Y kept.
 * Callers: 0E:D3FD, D50B, D537, D550, D589 (player_attack.c).
 */
void func_F428(void)
{
    REGS_IN;

    mff_cycles(0xF428, 18);
    RAM(ATTACK_LIVE) = ATTACK_LIVE_FRAMES;
    RAM(ATTACK_ID) = 0x00;
    a = SFX_SWING;
    CALL_FIXED(0xF432, func_F34C);
    mff_cycles(0xF435, 6);
    RTS_RETURN();
}

/*
 * attack_swing_sound, $F430: sound $17 (sound_queue_push_sfx).
 * Out: C, V as that leaves them; X, Y kept.
 * Callers: 09:AC33, 0E:D421, D438, D5A0, D5CE, D67D, D6B9, D6F3, D721.
 */
void func_F430(void)
{
    REGS_IN;

    mff_cycles(0xF430, 8);
    a = SFX_SWING;
    CALL_FIXED(0xF432, func_F34C);
    mff_cycles(0xF435, 6);
    RTS_RETURN();
}

/*
 * attack_end, $F436: attack_live = 0 (the attack no longer hits).
 * Out: A = 0; X, Y, C kept.
 * Callers: about 30 sites in banks 00, 05, 0E, and player_take_hit.
 */
void func_F436(void)
{
    REGS_IN;

    mff_cycles(0xF436, 11);
    a = 0x00;
    RAM(ATTACK_LIVE) = a;
    RTS_RETURN();
}

/*
 * attack_begin, $F43B: attack_live = $1E (the attack can hit).
 * Out: X, Y, C kept.
 * Callers: about 20 sites in banks 00 and 0E.
 */
void func_F43B(void)
{
    REGS_IN;

    mff_cycles(0xF43B, 11);
    a = ATTACK_LIVE_FRAMES;
    RAM(ATTACK_LIVE) = a;
    RTS_RETURN();
}

/* ---- the spark ---- */

/*
 * $F522: the spark's y (slot $0F, Y): the player's obj_y + 4, or - $10
 * for Haggar's grab hits. `charged`: the caller's run already covered
 * this code's first 7 cycles (the fall-through from $F519).
 */
static void spark_y(int charged)
{
    REGS_IN;
    int high = 0;

    if (!charged)
        mff_cycles(0xF522, 7);
    if (RAM(PLAYER_ID) == PLAYER_HAGGAR) {
        mff_cycles(0xF528, 7);
        a = RAM(ATTACK_ID);
        if (a == ATTACK_HAGGAR_GRAB_1) {
            mff_cycles(0xF52C, 1);
            high = 1;
        } else {
            mff_cycles(0xF52E, 4);
            if (a == ATTACK_HAGGAR_GRAB_2) {
                mff_cycles(0xF530, 1);
                high = 1;
            } else {
                mff_cycles(0xF532, 4);
                if (a == ATTACK_HAGGAR_GRAB_3) {
                    mff_cycles(0xF534, 1);
                    high = 1;
                } else {
                    mff_cycles(0xF536, 2);
                    mff_cycles(0xF536, 1);
                }
            }
        }
    } else {
        mff_cycles(0xF526, 1);
    }
    if (high) {
        mff_cycles(0xF538, 19);
        a = RAM(OBJ_Y);
        c = 1;
        SBC_A(SPARK_DY_HIGH);
    } else {
        mff_cycles(0xF542, 19);
        a = RAM(OBJ_Y);
        c = 0;
        ADC_A(SPARK_DY);
    }
    RAM(OBJ_Y + y) = a;
    RTS_RETURN();
}

/* $F522: the spark's y, by JMP from 0F:F4D4 and F503 */
void func_F522(void)
{
    spark_y(0);
}

/*
 * $F4E1: attacks $0F-$16 put the spark on the target's side of the player
 * (obj_dist_x: C set, the player right of slot X, means left), $1C away,
 * then JMP $F522. Reached by the JMP at 0F:F4B5 (Haggar) and the
 * fall-through from 0F:F4DF.
 */
void func_F4E1(void)
{
    REGS_IN;

    mff_cycles(0xF4E1, 6);
    CALL_FIXED(0xF4E1, func_F374);
    mff_cycles(0xF4E4, 2);
    if (c) {
        mff_cycles(0xF4E6, 4);
        y = DIR_LEFT;
        mff_cycles(0xF4E8, 1);
    } else {
        mff_cycles(0xF4E4, 1);
        mff_cycles(0xF4EA, 2);
        y = 0x00;
    }
    mff_cycles(0xF4EC, 10);
    a = RAM(OBJ_X);
    c = 0;
    ADC_A(prg_read((uint16_t)(SPARK_DX_SIDE + y)));
    mff_cycles(0xF4F3, 11);
    mff_push(a);
    a = RAM(OBJ_X_HI);
    ADC_A(prg_read((uint16_t)(SPARK_DX_SIDE + 1 + y)));
    mff_cycles(0xF4FA, 20);
    y = RAM(TMP15);
    RAM(OBJ_X_HI + y) = a;
    RAM(OBJ_X + y) = mff_pull();
    TAIL_FIXED(0xF522, func_F522);
}

/*
 * spawn_hit_spark, $F455: a hit spark (id 1, animation $7E) in a free
 * effect slot ($0F/$0E, find_free_effect_slot), unless slot X shows
 * animation $9D or both are taken. Its fields are cleared, obj_y_hi is the
 * player's, x is the player's + $12 in front of him (Haggar $13; attacks
 * $0F-$16 $1C toward slot X), y the player's + 4 (Haggar's grab hits
 * $1B-$1D: - $10). $0F = the spark's slot.
 * In: X = the slot hit. Out: X kept; Y = the spark's slot; V of the last
 * ADC/SBC.
 * Callers: objects_update (0C:80C5, the player's attack), 01:A0B1.
 */
void func_F455(void)
{
    REGS_IN;

    mff_cycles(0xF455, 8);
    if (RAM(OBJ_ANIM + x) == ANIM_NO_SPARK) {
        mff_cycles(0xF454, 1);
        mff_cycles(0xF454, 6);
        RTS_RETURN();
    }
    mff_cycles(0xF45C, 6);
    CALL_FIXED(0xF45C, func_F72A);
    mff_cycles(0xF45F, 2);
    if (c) {                                    /* no effect slot free */
        mff_cycles(0xF454, 1);
        mff_cycles(0xF454, 6);
        RTS_RETURN();
    }
    mff_cycles(0xF461, 11);
    RAM(TMP15) = y;
    a = ANIM_SPARK;
    CALL_FIXED(0xF465, func_F2A0);              /* obj_spawn */
    mff_cycles(0xF468, 105);
    RAM(OBJ_ID + y) = OBJ_SPARK;
    RAM(OBJ_SAVED_ID + y) = OBJ_SPARK;
    RAM(OBJ_HIT_COOLDOWN + y) = 0x00;
    RAM(OBJ_ATTACK + y) = 0x00;
    RAM(OBJ_ATTACK_DAMAGE + y) = 0x00;
    RAM(OBJ_LIFE + y) = 0x00;
    RAM(OBJ_LIFE_MAX + y) = 0x00;
    RAM(OBJ_INVULN + y) = 0x00;
    RAM(OBJ_HIT_PLAYER + y) = 0x00;
    RAM(OBJ_HIT_TIMER + y) = 0x00;
    RAM(OBJ_HELD + y) = 0x00;
    RAM(OBJ_KIND + y) = 0x00;
    RAM(OBJ_TIER + y) = 0x00;
    RAM(OBJ_AI_TIMER + y) = 0x00;
    RAM(OBJ_AI_COUNT + y) = 0x00;
    RAM(OBJ_AI_FLAG + y) = 0x00;
    RAM(OBJ_SPAWN_GROUP + y) = 0x00;
    RAM(OBJ_Y_HI + y) = RAM(OBJ_Y_HI);
    if (RAM(PLAYER_ID) == PLAYER_HAGGAR) {
        mff_cycles(0xF4AB, 7);
        a = RAM(ATTACK_ID);
        if (a >= ATTACK_SIDE_FIRST) {
            mff_cycles(0xF4B1, 4);
            if (a < ATTACK_SIDE_END) {
                mff_cycles(0xF4B5, 3);
                TAIL_FIXED(0xF4E1, func_F4E1);
            }
            mff_cycles(0xF4B3, 1);
        } else {
            mff_cycles(0xF4AF, 1);
        }
        mff_cycles(0xF4B8, 17);
        y = RAM(PLAYER_DIR) & DIR_LEFT;
        a = RAM(OBJ_X);
        c = 0;
        ADC_A(prg_read((uint16_t)(SPARK_DX_HAGGAR + y)));
        mff_cycles(0xF4C4, 11);
        mff_push(a);
        a = RAM(OBJ_X_HI);
        ADC_A(prg_read((uint16_t)(SPARK_DX_HAGGAR + 1 + y)));
        mff_cycles(0xF4CB, 20);
        y = RAM(TMP15);
        RAM(OBJ_X_HI + y) = a;
        RAM(OBJ_X + y) = mff_pull();
        TAIL_FIXED(0xF522, func_F522);
    }
    mff_cycles(0xF4A9, 1);
    mff_cycles(0xF4D7, 7);
    a = RAM(ATTACK_ID);
    if (a >= ATTACK_SIDE_FIRST) {
        mff_cycles(0xF4DD, 4);
        if (a < ATTACK_SIDE_END)
            FALL_INTO(func_F4E1);
        mff_cycles(0xF4DF, 2);                  /* BCS across a page */
    } else {
        mff_cycles(0xF4DB, 2);                  /* BCC across a page */
    }
    mff_cycles(0xF506, 17);
    y = RAM(PLAYER_DIR) & DIR_LEFT;
    a = RAM(OBJ_X);
    c = 0;
    ADC_A(prg_read((uint16_t)(SPARK_DX + y)));
    mff_cycles(0xF512, 11);
    mff_push(a);
    a = RAM(OBJ_X_HI);
    ADC_A(prg_read((uint16_t)(SPARK_DX + 1 + y)));
    mff_cycles(0xF519, 24);
    y = RAM(TMP15);
    RAM(OBJ_X_HI + y) = a;
    RAM(OBJ_X + y) = mff_pull();
    SYNC_OUT;
    spark_y(1);
}

/* ---- the player hit ---- */

/*
 * The player turns toward the attacker: obj_flags bit 6 = bit 0 of
 * (obj_dir of slot X ^ 3), by three RORs through $00 (the runs are the
 * callers'). Returns 1 when the result is $90 (facing left).
 */
#define FACE_ATTACKER() do { \
        a = (uint8_t)(RAM(OBJ_DIR + x) ^ 0x03); \
        c = (a >> 2) & 1; \
        RAM(TMP0) = (uint8_t)((a & 0x01) << 6); \
        a = (uint8_t)((RAM(OBJ_FLAGS) & ~OBJF_FACE_RIGHT) | RAM(TMP0)); \
        RAM(OBJ_FLAGS) = a; \
    } while (0)

/*
 * player_take_hit, $F8D5: slot X's attack hit the player (objects_update
 * set player_invuln = $3C just before). Nothing while player_no_hit bit 7
 * is set or player_invuln is not exactly $3C. A slot 1/2 object showing
 * animation $74 is removed (slot 1's never executed). A held enemy is let
 * go; a weapon dropped (drop_weapon; Guy keeps his). health -=
 * $FD16[obj_attack_damage]; if that goes negative or $FD56[...] is set,
 * the player is knocked back (game_action 6), else hurt (5); both turn him
 * toward the attacker and stop his attack and combo. Cody and Haggar lose
 * the weapon (weapon_held, and weapon_ammo when knocked back).
 * A hit while jumping (game_action 1) or during Cody's slash ($14) is a
 * knockback whatever the damage, and its BEQ lands on the knockback's
 * LDA #2 ($F9B7, not $F9BB past the STA): player_dir = 2 (left) even when
 * he faces right, likely a slip (base_guy_alt.flog frame 11807,
 * base_guy_tour.flog 27411: game_action 1 -> 6, player_dir 1 -> 2,
 * obj_flags $D0).
 * In: X = the attacker's slot. Out: X kept; V of the health SBC.
 * Callers: objects_update (0C:820B), 03:B95C, B96B (a boss).
 */
void func_F8D5(void)
{
    REGS_IN;
    int knockback;

    mff_cycles(0xF8D5, 5);
    if (RAM(PLAYER_NO_HIT) & NO_HIT_BIT) {
        mff_cycles(0xF8D9, 6);
        RTS_RETURN();
    }
    mff_cycles(0xF8D7, 1);
    mff_cycles(0xF8DA, 6);
    a = RAM(OBJ_STATE + 1);
    if (a & OBJ_LIVE) {
        mff_cycles(0xF8DF, 8);
        if (RAM(OBJ_ANIM + 1) == ANIM_SLOT12_74) {      /* never executed */
            mff_cycles(0xF8E6, 8);
            RAM(OBJ_STATE + 1) = 0x00;
            mff_cycles(0xF8EB, 1);
            goto slots_done;
        }
        mff_cycles(0xF8E4, 1);
    } else {
        mff_cycles(0xF8DD, 1);
    }
    mff_cycles(0xF8ED, 6);
    if (RAM(OBJ_STATE + 2) & OBJ_LIVE) {
        mff_cycles(0xF8F2, 8);                          /* never executed from here */
        if (RAM(OBJ_ANIM + 2) == ANIM_SLOT12_74) {
            mff_cycles(0xF8F9, 6);
            RAM(OBJ_STATE + 2) = 0x00;
        } else {
            mff_cycles(0xF8F7, 1);
        }
    } else {
        mff_cycles(0xF8F0, 1);
    }
slots_done:
    mff_cycles(0xF8FE, 5);
    a = RAM(PLAYER_INVULN);
    if (a == 0) {
        mff_cycles(0xF8D9, 2);                          /* BEQ across a page */
        mff_cycles(0xF8D9, 6);
        RTS_RETURN();
    }
    mff_cycles(0xF902, 4);
    if (a != PLAYER_HIT_INVULN) {
        mff_cycles(0xF8D9, 2);
        mff_cycles(0xF8D9, 6);
        RTS_RETURN();
    }

    /* let go of the held enemy, drop the weapon */
    mff_cycles(0xF906, 7);
    if (RAM(GAME_ACTION) == ACTION_GRAB) {
        mff_cycles(0xF90A, 1);
        mff_cycles(0xF90E, 10);
        y = RAM(GRAB_SLOT);
        RAM(OBJ_HELD + y) = 0xFF;
    } else {
        mff_cycles(0xF90C, 2);
        mff_cycles(0xF90C, 1);
    }
    mff_cycles(0xF915, 5);
    if (RAM(WEAPON_HELD) != 0) {
        mff_cycles(0xF919, 7);
        if (RAM(PLAYER_ID) != PLAYER_GUY) {
            mff_cycles(0xF91F, 6);
            CALL_FIXED(0xF91F, func_FAC4);
        } else {
            mff_cycles(0xF91D, 1);
        }
    } else {
        mff_cycles(0xF917, 1);
    }

    /* the damage */
    mff_cycles(0xF922, 13);
    y = RAM(OBJ_ATTACK_DAMAGE + x);
    a = RAM(HEALTH);
    c = 1;
    SBC_A(prg_read((uint16_t)(DAMAGE_HEALTH + y)));
    mff_cycles(0xF92B, 5);
    RAM(HEALTH) = a;
    if (a & 0x80) {
        mff_cycles(0xF92D, 1);
        knockback = 1;
    } else {
        mff_cycles(0xF92F, 4);
        knockback = prg_read((uint16_t)(DAMAGE_KNOCKS_DOWN + y)) != 0;
        mff_cycles(0xF932, 2);
        if (knockback)
            mff_cycles(0xF932, 1);
    }

    if (!knockback) {                           /* hurt */
        mff_cycles(0xF934, 7);
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(PLAYER_ANIM_HURT + y));
        mff_cycles(0xF939, 6);
        CALL_FIXED(0xF939, func_F294);          /* player_set_anim */
        mff_cycles(0xF93C, 4);
        a = prg_read((uint16_t)(PLAYER_CHR_HURT + y));
        mff_cycles(0xF93F, 6);
        CALL_FIXED(0xF93F, func_F440);          /* set_chr_bank2 */
        mff_cycles(0xF942, 34);
        FACE_ATTACKER();
        if (a == OBJF_PLAYER_LEFT) {
            mff_cycles(0xF95A, 1);
            mff_cycles(0xF960, 2);
            a = DIR_LEFT;
        } else {
            mff_cycles(0xF95C, 4);
            a = DIR_RIGHT;
            mff_cycles(0xF95E, 1);
        }
        mff_cycles(0xF962, 10);
        RAM(PLAYER_DIR) = a;
        a = RAM(GAME_ACTION);
        if (a == ACTION_JUMP) {
            mff_cycles(0xF968, 1);
            goto knocked_back_left;
        }
        mff_cycles(0xF96A, 4);
        if (a == ACTION_CODY_SLASH) {
            mff_cycles(0xF96C, 1);
            goto knocked_back_left;
        }
        mff_cycles(0xF96E, 12);
        RAM(GAME_ACTION) = ACTION_HIT;
        if (RAM(PLAYER_ID) != PLAYER_GUY) {
            mff_cycles(0xF978, 17);
            a = 0x00;
            RAM(ATTACK_LIVE) = a;
            RAM(WEAPON_HELD) = a;
            RAM(COMBO_TIMER) = a;
            RTS_RETURN();
        }
        mff_cycles(0xF976, 1);
        mff_cycles(0xF981, 5);
        if (RAM(WEAPON_HELD) != 0) {            /* never executed */
            mff_cycles(0xF985, 6);
            CALL_FIXED(0xF985, func_F436);      /* attack_end */
            mff_cycles(0xF988, 16);
            RAM(COMBO_TIMER) = 0x00;
            RAM(WEAPON_HELD) = 0xFF;
            RTS_RETURN();
        }
        mff_cycles(0xF983, 1);
        mff_cycles(0xF991, 6);
        CALL_FIXED(0xF991, func_F436);          /* attack_end */
        mff_cycles(0xF994, 11);
        RAM(COMBO_TIMER) = 0x00;
        RTS_RETURN();
    }

    /* knocked back */
    mff_cycles(0xF999, 34);
    FACE_ATTACKER();
    if (a == OBJF_PLAYER_LEFT) {
        mff_cycles(0xF9B1, 1);
        goto knocked_back_left;
    }
    mff_cycles(0xF9B3, 4);
    a = DIR_RIGHT;
    mff_cycles(0xF9B5, 1);
    goto knocked_back;
knocked_back_left:
    mff_cycles(0xF9B7, 2);
    a = DIR_LEFT;
knocked_back:
    mff_cycles(0xF9B9, 10);
    RAM(PLAYER_DIR) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(PLAYER_ANIM_KNOCKBACK + y));
    mff_cycles(0xF9C0, 6);
    CALL_FIXED(0xF9C0, func_F294);              /* player_set_anim */
    mff_cycles(0xF9C3, 4);
    a = prg_read((uint16_t)(PLAYER_CHR_KNOCKBACK + y));
    mff_cycles(0xF9C6, 6);
    CALL_FIXED(0xF9C6, func_F440);              /* set_chr_bank2 */
    mff_cycles(0xF9C9, 15);
    RAM(ATTACK_LIVE) = 0x00;
    RAM(COMBO_TIMER) = 0x00;
    if (RAM(PLAYER_ID) != PLAYER_GUY) {
        mff_cycles(0xF9D5, 8);
        RAM(WEAPON_HELD) = 0x00;
        RAM(WEAPON_AMMO) = 0x00;
    } else {
        mff_cycles(0xF9D3, 1);
    }
    mff_cycles(0xF9DB, 17);
    RAM(GAME_ACTION) = ACTION_KNOCKDOWN;
    a = OBJ_LIVE;
    RAM(OBJ_STATE) = a;
    RTS_RETURN();
}

/*
 * drop_weapon, $FAC4: slot 1 becomes the dropped weapon (id $66, its
 * behaviour 05:A712): live, on screen in world coordinates ($90), at the
 * player's position, facing his way, animation $FB06[player_id] from its
 * first frame, obj_attack $2F (no attack out). Nothing else of the slot is
 * cleared.
 * Out: X kept (callers read nothing else).
 * Caller: player_take_hit (0F:F91F).
 */
void func_FAC4(void)
{
    REGS_IN;

    mff_cycles(0xFAC4, 51);
    RAM(OBJ_STATE + SLOT_WEAPON) = OBJ_LIVE;
    RAM(OBJ_FLAGS + SLOT_WEAPON) = OBJF_ON_SCREEN | OBJF_WORLD;
    RAM(OBJ_X + SLOT_WEAPON) = RAM(OBJ_X);
    RAM(OBJ_Y + SLOT_WEAPON) = RAM(OBJ_Y);
    RAM(OBJ_X_HI + SLOT_WEAPON) = RAM(OBJ_X_HI);
    RAM(OBJ_Y_HI + SLOT_WEAPON) = RAM(OBJ_Y_HI);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(DROPPED_WEAPON_ANIM + y));
    mff_cycles(0xFAEB, 39);
    RAM(OBJ_ANIM + SLOT_WEAPON) = a;
    RAM(OBJ_ANIM_TIMER + SLOT_WEAPON) = 0x00;
    RAM(OBJ_ANIM_FRAME + SLOT_WEAPON) = 0x00;
    RAM(OBJ_DIR + SLOT_WEAPON) = RAM(PLAYER_DIR);
    RAM(OBJ_ID + SLOT_WEAPON) = OBJ_WEAPON;
    a = DROPPED_WEAPON_ATTACK;
    RAM(OBJ_ATTACK + SLOT_WEAPON) = a;
    RTS_RETURN();
}

/*
 * obj_hit_start, $FB09: the player's hit landed on slot X:
 * obj_hit_cooldown = $FB17[attack_id] (frames before it can be hit
 * again), obj_hit_timer = $1E.
 * In: X = slot. Out: X, C kept (Y = attack_id).
 * Caller: the hit handling of objects_update (0C:85B6).
 */
void func_FB09(void)
{
    REGS_IN;

    mff_cycles(0xFB09, 7);
    y = RAM(ATTACK_ID);
    a = prg_read((uint16_t)(HIT_COOLDOWN + y));
    mff_cycles(0xFB0E, 18);
    RAM(OBJ_HIT_COOLDOWN + x) = a;
    a = HIT_TIMER_START;
    RAM(OBJ_HIT_TIMER + x) = a;
    RTS_RETURN();
}
