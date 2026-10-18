/*
 * obj_items.c - the items (ids $10-$13): picked up with B when the player
 * stands close (0C:8AEE-8C1B). docs/objects.md, "Ids".
 *
 * The behaviour waits for the pickup; then the player plays the pick-up
 * animation and the item's routine runs through the table at $8B5C/$8B60
 * (by id & $0F): food ($10) gives health, or points when health is full;
 * $11 a credit, $12 two lives, $13 one life, up to 9 each. All of them end
 * in item_taken ($8C00), which frees the slot.
 */
#include "obj_defs.h"

#define SFX_PICKUP          0x26    /* also AxL's block (0C:8571) */
#define SFX_CREDIT          0x2E
#define SFX_LIFE            0x2D

/* the player's pick-up animation and sprite CHR bank, by player_id */
#define PICKUP_ANIM         0xE210
#define PICKUP_CHR          0xE258
/* the items' routines, by id & $0F */
#define ITEM_ROUTINE_LO     0x8B5C
#define ITEM_ROUTINE_HI     0x8B60
/* food: health per kind (obj_anim & $0F) and, at full health, the attack
 * id whose points it gives (obj_anim - $60), both in bank 0F */
#define FOOD_HEALTH         0xFCC0
#define FOOD_POINTS         0xFCD1
/* pointers to each character's full health by level */
#define LEVEL_HEALTH_LO     0x8BAA
#define LEVEL_HEALTH_HI     0x8BAD

#define MAX_COUNT           0x09    /* credits and lives stop at 9 */

/* game_action values in which nothing is picked up ($8AF0-$8B16): jumping
 * (1), 2 (the pick-up itself), falling (6, $25), grabbing (4), the throws
 * and HeadSmasher ($13, $15, $12), Haggar's power ($10), the special ($17).
 * Cody's throw ($0B) is tested apart, for Cody only. */
static const uint8_t k_busy_actions[] = { 0x01, 0x02, 0x06, 0x25, 0x04, 0x13, 0x15, 0x10, 0x12, 0x17 };

/* the RTS at $8B5B, where every "not now" of the pickup test ends */
#define NOT_NOW() do { mff_cycles(0x8B5B, 6); RTS_RETURN(); } while (0)

/*
 * obj_item, $8AEE (name proposed): behaviour of ids $10-$13. The item is
 * taken when the player is not busy (k_busy_actions; Cody not throwing),
 * enemy_attack_test (0F:F55A) finds the player in the item's box, the y
 * distance (0F:F365) is $07-$11 and joy_pressed is B alone. Then
 * player_set_anim($E210[player_id]), chr_banks+2 = $E258[player_id]
 * (0F:F440) and JMP ($8B5C/$8B60[id & $0F]).
 * In: X = slot. Out: the RTS with the flags of the last test, or as the
 * item's routine.
 * Caller: obj_behavior_lo/hi (ids $10-$13).
 */
void func_8AEE_b6(void)
{
    REGS_IN;
    int i;

    mff_cycles(0x8AEE, 3);
    a = RAM(GAME_ACTION);
    i = cmp_list(0x8AF0, 0x8B5B, a, k_busy_actions, 10);
    CMP_LIST_FLAGS(a, k_busy_actions, 10, i);
    if (i < 10)
        NOT_NOW();
    mff_cycles(0x8B18, 5);
    a = RAM(PLAYER_ID);
    SET_NZ(a);
    if (!z) {
        mff_cycles(0x8B1A, 1);
    } else {
        mff_cycles(0x8B1C, 7);
        a = RAM(GAME_ACTION);
        CMP_FLAGS(a, ACTION_CODY_THROW);
        if (z) {
            mff_cycles(0x8B20, 1);
            NOT_NOW();
        }
    }

    mff_cycles(0x8B22, 6);
    CALL_FIXED(0x8B22, func_F55A);
    mff_cycles(0x8B25, 2);
    if (c) {
        mff_cycles(0x8B25, 1);
        NOT_NOW();
    }
    mff_cycles(0x8B27, 6);
    CALL_FIXED(0x8B27, func_F365);
    mff_cycles(0x8B2A, 2);
    if (c) {
        mff_cycles(0x8B2A, 1);
        NOT_NOW();
    }
    mff_cycles(0x8B2C, 4);
    CMP_FLAGS(a, 0x12);
    if (c) {
        mff_cycles(0x8B2E, 1);
        NOT_NOW();
    }
    mff_cycles(0x8B30, 4);
    CMP_FLAGS(a, 0x07);
    if (!c) {
        mff_cycles(0x8B32, 1);
        NOT_NOW();
    }
    mff_cycles(0x8B34, 7);
    a = RAM(JOY_PRESSED);
    CMP_FLAGS(a, 0x40);                         /* B */
    if (!z) {
        mff_cycles(0x8B38, 1);
        NOT_NOW();
    }

    mff_cycles(0x8B3A, 7);
    y = RAM(PLAYER_ID);
    a = prg_read_ix(PICKUP_ANIM, y);
    SET_NZ(a);
    mff_cycles(0x8B3F, 6);
    CALL_FIXED(0x8B3F, func_F294);
    mff_cycles(0x8B42, 4);
    a = prg_read_ix(PICKUP_CHR, y);
    SET_NZ(a);
    mff_cycles(0x8B45, 6);
    CALL_FIXED(0x8B45, func_F440);
    mff_cycles(0x8B48, 12);
    a = RAM(OBJ_ID + x) & 0x0F;
    y = a;
    a = prg_read_ix(ITEM_ROUTINE_LO, y);
    mff_cycles(0x8B51, 7);
    RAM(TMP0) = a;
    a = prg_read_ix(ITEM_ROUTINE_HI, y);
    SET_NZ(a);
    mff_cycles(0x8B56, 8);
    RAM(TMP1) = a;
    JMP_INDIRECT(0x0000);
}

/*
 * item_food, $8B64 (name proposed): sound $26; below the level's full
 * health (TMP2/TMP3 = the character's table) health grows by
 * $FCC0[obj_anim & $0F], at most to full; at full health the food is
 * worth the points of attack $FCD1[obj_anim - $60] instead (exp_award).
 * Then item_taken.
 * In: X = slot. Out: as item_taken.
 * Caller: JMP ($0000) at 0C:8B58 (id $10).
 */
void func_8B64_b6(void)
{
    REGS_IN;

    mff_cycles(0x8B64, 8);
    a = SFX_PICKUP;
    SET_NZ(a);
    CALL_FIXED(0x8B66, func_F34C);
    mff_cycles(0x8B69, 7);
    y = RAM(PLAYER_ID);
    a = prg_read_ix(LEVEL_HEALTH_LO, y);
    mff_cycles(0x8B6E, 7);
    RAM(TMP2) = a;
    a = prg_read_ix(LEVEL_HEALTH_HI, y);
    mff_cycles(0x8B73, 14);
    RAM(TMP3) = a;
    y = RAM(PLAYER_LEVEL);
    a = RAM(HEALTH);
    m = mem_read_iy(TMP2, y);
    mff_cycles(0x8B7B, 2);
    if (a != m) {
        mff_cycles(0x8B7D, 17);
        a = RAM(OBJ_ANIM + x) & 0x0F;
        y = a;
        a = RAM(HEALTH);
        c = 0;
        ADC_A(prg_read_ix(FOOD_HEALTH, y));
        mff_cycles(0x8B89, 11);
        RAM(HEALTH) = a;
        y = RAM(PLAYER_LEVEL);
        a = mem_read_iy(TMP2, y);
        mff_cycles(0x8B8F, 5);
        CMP_FLAGS(a, RAM(HEALTH));
        if (c) {
            mff_cycles(0x8B91, 1);
        } else {
            /* over full: back to full (the BNE after it always branches) */
            mff_cycles(0x8B93, 6);
            RAM(HEALTH) = a;
        }
    } else {
        mff_cycles(0x8B7B, 1);
        mff_cycles(0x8B97, 14);
        a = RAM(OBJ_ANIM + x);
        c = 1;
        SBC_A(0x60);
        y = a;
        a = prg_read_ix(FOOD_POINTS, y);
        SET_NZ(a);
        mff_cycles(0x8BA1, 9);
        RAM(ATTACK_ID) = a;
        CALL_BANK(0x8BA3, 0x89E3);              /* exp_award */
    }
    mff_cycles(0x8BA6, 6);
    CALL_BANK(0x8BA6, 0x8C00);                  /* item_taken */
    mff_cycles(0x8BA9, 6);
    RTS_RETURN();
}

/*
 * item_credit, $8BC2 (name proposed; never executed): sound $2E, credits
 * + 1 up to 9, item_taken.
 * Caller: JMP ($0000) at 0C:8B58 (id $11).
 */
void func_8BC2_b6(void)
{
    REGS_IN;

    mff_cycles(0x8BC2, 8);
    a = SFX_CREDIT;
    SET_NZ(a);
    CALL_FIXED(0x8BC4, func_F34C);
    mff_cycles(0x8BC7, 7);
    a = RAM(CREDITS);
    CMP_FLAGS(a, MAX_COUNT);
    if (!z) {
        mff_cycles(0x8BCD, 7);
        c = 0;
        ADC_A(0x01);
        SET_NZ(a);
        RAM(CREDITS) = a;
    } else {
        mff_cycles(0x8BCB, 1);
    }
    mff_cycles(0x8BD2, 6);
    CALL_BANK(0x8BD2, 0x8C00);                  /* item_taken */
    mff_cycles(0x8BD5, 6);
    RTS_RETURN();
}

/*
 * item_two_lives, $8BD6 (name proposed; never executed): sound $2D, lives
 * + 2, at most 9, item_taken.
 * Caller: JMP ($0000) at 0C:8B58 (id $12).
 */
void func_8BD6_b6(void)
{
    REGS_IN;

    mff_cycles(0x8BD6, 8);
    a = SFX_LIFE;
    SET_NZ(a);
    CALL_FIXED(0x8BD8, func_F34C);
    mff_cycles(0x8BDB, 17);
    RAM(LIVES) = (uint8_t)(RAM(LIVES) + 1);
    RAM(LIVES) = (uint8_t)(RAM(LIVES) + 1);
    a = RAM(LIVES);
    CMP_FLAGS(a, MAX_COUNT);
    if (c) {
        mff_cycles(0x8BE5, 5);
        a = MAX_COUNT;
        SET_NZ(a);
        RAM(LIVES) = a;
    } else {
        mff_cycles(0x8BE3, 1);
    }
    mff_cycles(0x8BE9, 6);
    CALL_BANK(0x8BE9, 0x8C00);                  /* item_taken */
    mff_cycles(0x8BEC, 6);
    RTS_RETURN();
}

/*
 * item_life, $8BED (name proposed): sound $2D, lives + 1 unless at 9
 * already, then falls into item_taken.
 * Caller: JMP ($0000) at 0C:8B58 (id $13).
 */
void func_8BED_b6(void)
{
    REGS_IN;

    mff_cycles(0x8BED, 8);
    a = SFX_LIFE;
    SET_NZ(a);
    CALL_FIXED(0x8BEF, func_F34C);
    mff_cycles(0x8BF2, 7);
    a = RAM(LIVES);
    c = a >= MAX_COUNT;
    if (c) {
        /* never executed */
        mff_cycles(0x8BF8, 7);
        a = MAX_COUNT;
        RAM(LIVES) = a;
        mff_cycles(0x8BFC, 2);                  /* BNE $8C00, into the next page */
    } else {
        mff_cycles(0x8BF6, 1);
        mff_cycles(0x8BFE, 5);
        RAM(LIVES) = (uint8_t)(RAM(LIVES) + 1);
    }
    SYNC_OUT;
    func_8C00_b6();
}

/*
 * item_taken, $8C00 (name proposed): the end of every pickup: game_action
 * 2 for $0C frames (action_timer) and object_clear on the item's slot.
 * With $B2 bit 7 set (never executed) game_action becomes $29 and the
 * player's obj_state $80 first (then JMP $8C08).
 * In: X = slot. Out: as object_clear (A = $FF).
 * Callers: JSR at 0C:8BA6, 8BD2, 8BE9; the fall-through from item_life.
 */
void func_8C00_b6(void)
{
    REGS_IN;

    mff_cycles(0x8C00, 5);
    a = RAM(BONUS_STAGE);
    if (!(a & 0x80)) {
        mff_cycles(0x8C04, 16);
        RAM(GAME_ACTION) = 0x02;
        a = 0x0C;
        SET_NZ(a);
        RAM(ACTION_TIMER) = a;
        CALL_FIXED(0x8C0C, func_F2BE);          /* object_clear */
        mff_cycles(0x8C0F, 6);
        RTS_RETURN();
    }
    mff_cycles(0x8C02, 1);
    mff_cycles(0x8C10, 11);
    RAM(GAME_ACTION) = 0x29;
    a = OBJ_LIVE;
    SET_NZ(a);
    RAM(OBJ_STATE) = a;
    JMP_TAIL(0x8C19, 0x8C08);
}

/*
 * $8C08: item_taken's end: action_timer = $0C and object_clear on slot X.
 * In: X = slot. Out: as object_clear.
 * Caller: JMP at 0C:8C19 (the $B2 path, never executed).
 */
void func_8C08_b6(void)
{
    REGS_IN;

    mff_cycles(0x8C08, 11);
    a = 0x0C;
    SET_NZ(a);
    RAM(ACTION_TIMER) = a;
    CALL_FIXED(0x8C0C, func_F2BE);              /* object_clear */
    mff_cycles(0x8C0F, 6);
    RTS_RETURN();
}
