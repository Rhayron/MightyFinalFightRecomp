/*
 * obj_reach.c - the reach tests of objects_update (docs/objects.md,
 * "Update: objects_update"): does slot X's attack reach the player
 * (enemy_attack_test), does the player's attack reach slot X
 * (player_attack_test), does the object in slot 2 or 1 touch slot X
 * (obj_contact_test).
 *
 * Each answers in C: clear = a hit, set = no. A test first rules the slot
 * out (off screen, hidden, ids that cannot hit or be hit, the player
 * immune), then compares the depth (y) lines, then |dx| against a reach
 * in front or behind, then |dy| against a reach in y, from tables indexed
 * by the attack. Distances are the 6502's: SEC/SBC, then EOR #$FF / ADC #1
 * on a borrow.
 *
 * The depth line is where an object stands: obj_y on the ground,
 * obj_ground_y during the airborne animations (airborne_anim),
 * player_ground_y for the player.
 */
#include "obj_fixed.h"

/* enemy_attack_test, by obj_attack & $1F: the reach in y, in x behind
 * and in front of the attacker (or of the player, for k_reach_any_ids) */
#define ENEMY_REACH_Y           0xF6AA
#define ENEMY_REACH_X_BACK      0xF6CA
#define ENEMY_REACH_X_FRONT     0xF6EA
#define OBJ_ATTACK_REACH        0x1F

/* player_attack_test, by attack_id: the reach in y, in x in front of the
 * player and behind him (36 entries each) */
#define PLAYER_REACH_Y          0xF869
#define PLAYER_REACH_X_FRONT    0xF88D
#define PLAYER_REACH_X_BACK     0xF8B1

/* obj_contact_test, by the obj_attack of the slot 1/2 object: the reach
 * in y and in x ($FAC0: 0E 14 14 14, then the code of drop_weapon) */
#define CONTACT_REACH_Y         0xFAC0
#define CONTACT_REACH_X         0xFAC2

#define DEPTH_CLOSE             0x08    /* enemy_attack_test: depth lines within 8 px */
#define DEPTH_CLOSE_STANDING    0x0C    /* ... obj_dist_y within 12 when both stand */
#define ATTACK_DEPTH_CLOSE      0x0C    /* player_attack_test */
#define SPECIAL_DEPTH_CLOSE     0x14    /* player_attack_test, attacks $14-$17 */

/* the airborne animations, tested at obj_ground_y (objects_update's
 * 0C:8108 also has $CC) */
#define ANIM_AIR_84             0x84
#define ANIM_AIR_AC             0xAC
#define ANIM_AIR_E7             0xE7
#define ANIM_AIR_BE             0xBE

/* enemy_attack_test: ids that reach the player whatever he does, the side
 * picked by his facing: items $10-$13 (0C:8B22 asks whether the player
 * is close enough to pick one up) and $69/$6A */
static const uint8_t k_reach_any_ids[] = { 0x69, 0x6A, 0x10, 0x11, 0x12, 0x13 };

/* game_action values in which enemy attacks miss the player: the lariat,
 * the special, knocked down, lying, Cody's throw, the earthquake, the pit
 * fall, dropping in */
static const uint8_t k_player_immune[] = { 0x16, 0x17, 0x06, 0x1F, 0x0B, 0x25, 0x0F, 0x24 };

/* player_attack_test: ids it does not hit (besides those below $1F, and
 * $5F) */
#define OBJ_FIRST_HITTABLE      0x1F
#define OBJ_5F                  0x5F
static const uint8_t k_unhittable_ids[] = { 0x69, 0x6A, 0x6C, 0x95, 0x63, 0x61 };

/* player_attack_test: the depth test of each attack_id listed (the rest
 * as BY_PLAYER_Y_JMP) */
enum {
    BY_PLAYER_Y_JMP,    /* JMP $F7EA: the player's obj_y, attack_ground_y when 0 */
    BY_PLAYER_Y,        /* the same, reached by a branch */
    BY_SPECIAL,         /* attack_ground_y within $14 */
    BY_GROUND_Y_JMP,    /* JMP $F7EF: attack_ground_y */
    BY_GROUND_Y         /* the same, reached by a branch */
};
static const uint8_t k_attack_ids[] = {
    0x0F, 0x10, 0x11, 0x12, 0x14, 0x15, 0x16, 0x17, 0x07, 0x03, 0x04, 0x0D, 0x18
};
static const uint8_t k_attack_depth[] = {
    BY_PLAYER_Y_JMP, BY_PLAYER_Y_JMP, BY_PLAYER_Y, BY_PLAYER_Y,
    BY_SPECIAL, BY_SPECIAL, BY_SPECIAL, BY_SPECIAL,
    BY_GROUND_Y_JMP, BY_GROUND_Y_JMP, BY_GROUND_Y_JMP, BY_GROUND_Y, BY_GROUND_Y
};
/* attacks $1B-$1D (the grab's hits) only reach the held enemy */
#define ATTACK_GRAB_HIT_1       0x1B
#define ATTACK_GRAB_HIT_2       0x1C
#define ATTACK_GRAB_HIT_3       0x1D

/* obj_contact_test: the slot 1/2 objects that touch nothing (the dropped
 * weapon, $69, a spark turned into id 9), the slot X ids they pass
 * through and its animations they pass through */
#define OBJ_69                  0x69
static const uint8_t k_contact_skip_ids[] = {
    0x05, 0x06, 0x07, 0x6C, 0x6D, 0x66, 0x69, 0x6A, 0x05, 0x10, 0x11, 0x12, 0x13, 0x60, 0x95, 0x61
};
#define ANIM_CONTACT_SKIP_1     0x74
#define ANIM_CONTACT_SKIP_2     0x7F
#define ANIM_CONTACT_SKIP_3     0x79

/*
 * CMP #$84 / BEQ at `beq` (the CMP charged by the caller), then CMP #$AC,
 * #$E7 and #$BE, the last a BNE past the code for a match: 1 when `anim`
 * is an airborne animation. The taken branches are charged.
 */
static int airborne_anim(uint16_t beq, uint8_t anim)
{
    if (anim == ANIM_AIR_84) {
        mff_cycles(beq, 1);
        return 1;
    }
    mff_cycles((uint16_t)(beq + 2), 4);
    if (anim == ANIM_AIR_AC) {
        mff_cycles((uint16_t)(beq + 4), 1);
        return 1;
    }
    mff_cycles((uint16_t)(beq + 6), 4);
    if (anim == ANIM_AIR_E7) {
        mff_cycles((uint16_t)(beq + 8), 1);
        return 1;
    }
    mff_cycles((uint16_t)(beq + 10), 4);
    if (anim == ANIM_AIR_BE)
        return 1;
    mff_cycles((uint16_t)(beq + 12), 1);
    return 0;
}

/* ---- enemy_attack_test ---- */

/* $F558: SEC / RTS, the "no" of enemy_attack_test (its branches go here) */
void func_F558(void)
{
    REGS_IN;

    mff_cycles(0xF558, 8);
    c = 1;
    RTS_RETURN();
}

/* $F6A9: RTS, the end of enemy_attack_test (C as the last compare left it) */
void func_F6A9(void)
{
    REGS_IN;

    mff_cycles(0xF6A9, 6);
    RTS_RETURN();
}

/* $F679: JMP $F6A9, a miss in x or in depth (C set) */
void func_F679(void)
{
    REGS_IN;

    mff_cycles(0xF679, 3);
    TAIL_FIXED(0xF6A9, func_F6A9);
}

/*
 * $F63B / $F63F: the reach of slot X's attack (Y = obj_attack & $1F). $03
 * picks whose facing decides front or behind: $FF (from $F63B, after the
 * depth test) the attacker's (obj_flags bit 6), 0 the player's (his
 * obj_dir). |dx| ($00) must be below $F6EA[Y] in front or $F6CA[Y]
 * behind, then |dy| below $F6AA[Y].
 */
static void enemy_reach(int after_depth)
{
    REGS_IN;
    int right, front;

    if (after_depth) {
        mff_cycles(0xF63B, 40);
        RAM(TMP3) = 0xFF;
    } else {
        mff_cycles(0xF63F, 35);
    }
    y = RAM(OBJ_ATTACK + x) & OBJ_ATTACK_REACH;
    a = RAM(OBJ_X);
    c = 1;
    SBC_A(RAM(OBJ_X + x));
    mff_push(a);
    a = RAM(OBJ_X_HI);
    SBC_A(RAM(OBJ_X_HI + x));
    a = mff_pull();
    if (c) {                                    /* the player is level or to the right */
        mff_cycles(0xF654, 1);
        right = 1;
    } else {
        mff_cycles(0xF656, 6);
        a ^= 0xFF;
        ADC_A(1);
        right = c;                              /* |dx| a multiple of 256 reads as right */
        if (!right)
            mff_cycles(0xF65A, 1);
    }
    if (right) {
        mff_cycles(0xF65C, 8);
        RAM(TMP0) = a;
        if (RAM(TMP3) != 0) {
            mff_cycles(0xF660, 1);
            mff_cycles(0xF66B, 8);
            front = (RAM(OBJ_FLAGS + x) & OBJF_FACE_RIGHT) != 0;
            if (!front)
                mff_cycles(0xF670, 1);
        } else {
            mff_cycles(0xF662, 8);
            front = !(RAM(OBJ_DIR) & DIR_RIGHT);
            if (front) {
                mff_cycles(0xF667, 1);
            } else {
                mff_cycles(0xF669, 2);
                mff_cycles(0xF669, 1);
            }
        }
    } else {
        mff_cycles(0xF67C, 8);
        RAM(TMP0) = a;
        if (RAM(TMP3) != 0) {
            mff_cycles(0xF680, 1);
            mff_cycles(0xF68B, 8);
            front = !(RAM(OBJ_FLAGS + x) & OBJF_FACE_RIGHT);
            if (front)
                mff_cycles(0xF672, 1);
        } else {
            mff_cycles(0xF682, 8);
            front = (RAM(OBJ_DIR) & DIR_RIGHT) != 0;
            if (front) {
                mff_cycles(0xF689, 2);
                mff_cycles(0xF672, 1);
            } else {
                mff_cycles(0xF687, 1);
            }
        }
    }
    if (front) {
        mff_cycles(0xF672, 7);
        a = RAM(TMP0);
        c = a >= prg_read((uint16_t)(ENEMY_REACH_X_FRONT + y));
        mff_cycles(0xF677, 2);
        if (c) {
            mff_cycles(0xF679, 3);
            TAIL_FIXED(0xF6A9, func_F6A9);
        }
        mff_cycles(0xF677, 1);
    } else {
        mff_cycles(0xF692, 7);
        a = RAM(TMP0);
        c = a >= prg_read((uint16_t)(ENEMY_REACH_X_BACK + y));
        mff_cycles(0xF697, 2);
        if (c) {
            mff_cycles(0xF697, 1);
            FALL_INTO(func_F6A9);
        }
    }
    mff_cycles(0xF699, 12);
    a = RAM(OBJ_Y);
    c = 1;
    SBC_A(RAM(OBJ_Y + x));
    if (!c) {
        mff_cycles(0xF6A2, 4);
        a ^= 0xFF;
        ADC_A(1);
    } else {
        mff_cycles(0xF6A0, 1);
    }
    mff_cycles(0xF6A6, 4);
    c = a >= prg_read((uint16_t)(ENEMY_REACH_Y + y));
    FALL_INTO(func_F6A9);
}

/* $F63F: the reach with $03 as the caller left it (the JMP at 0F:F5BC) */
void func_F63F(void)
{
    enemy_reach(0);
}

/*
 * $F601 / $F615 / $F628, three copies: the depth lines p and q within 8
 * px (the SBC at `pc` in a run of `run` cycles, the negation at `neg`):
 * then enemy_reach, else JMP $F679 (C set).
 */
static void enemy_depth(uint16_t pc, int run, uint16_t neg, uint8_t p, uint8_t q)
{
    REGS_IN;
    uint16_t cmp = (uint16_t)(neg + 4);

    mff_cycles(pc, run);
    a = p;
    c = 1;
    SBC_A(q);
    if (!c) {
        mff_cycles(neg, 4);
        a ^= 0xFF;
        ADC_A(1);
    } else {
        mff_cycles((uint16_t)(neg - 2), 1);
    }
    mff_cycles(cmp, 4);
    c = a >= DEPTH_CLOSE;
    if (c) {
        mff_cycles((uint16_t)(cmp + 4), 3);
        TAIL_FIXED(0xF679, func_F679);
    }
    mff_cycles((uint16_t)(cmp + 2), 1);
    SYNC_OUT;
    enemy_reach(1);
}

/*
 * enemy_attack_test, $F55A: does slot X's attack reach the player? Not if
 * the slot is off screen, hidden, not live or dying (ids 6, 7), nor while
 * the player is immune (respawn_timer1/2, k_player_immune); the ids of
 * k_reach_any_ids skip the player checks and the depth test. Depth: $01
 * bit 0 = the player off his ground line, bit 1 = slot X in an airborne
 * animation; both standing: obj_dist_y below $0C, otherwise the two depth
 * lines within 8 px. Then enemy_reach.
 * In: X = slot. Out: C clear = hit; V of the last SBC/ADC; Y = obj_attack
 * & $1F once that far; $00, $01, $03 scratch; X kept.
 * Callers: objects_update (0C:81FD), the items (0C:8B22), 05:A7D9.
 */
void func_F55A(void)
{
    REGS_IN;

    mff_cycles(0xF55A, 8);
    c = 1;
    a = RAM(OBJ_FLAGS + x);
    if (!(a & OBJF_ON_SCREEN)) {
        mff_cycles(0xF55E, 1);
        FALL_INTO(func_F558);
    }
    mff_cycles(0xF560, 4);
    if (a & OBJF_HIDDEN) {
        mff_cycles(0xF562, 1);
        FALL_INTO(func_F558);
    }
    mff_cycles(0xF564, 6);
    if (!(RAM(OBJ_STATE + x) & OBJ_LIVE)) {
        mff_cycles(0xF567, 1);
        FALL_INTO(func_F558);
    }
    mff_cycles(0xF569, 8);
    a = RAM(OBJ_ID + x);
    if (a == OBJ_KILLED_LIGHT) {
        mff_cycles(0xF56E, 1);
        FALL_INTO(func_F558);
    }
    mff_cycles(0xF570, 4);
    if (a == OBJ_KILLED) {
        mff_cycles(0xF572, 1);
        FALL_INTO(func_F558);
    }
    if (cmp_list(0xF574, 0xF5B8, a, k_reach_any_ids, 6) < 6) {
        mff_cycles(0xF5B8, 8);
        RAM(TMP3) = 0x00;
        TAIL_FIXED(0xF63F, func_F63F);
    }
    mff_cycles(0xF58C, 5);
    if (RAM(RESPAWN_TIMER1) != 0) {
        mff_cycles(0xF58E, 1);
        FALL_INTO(func_F558);
    }
    mff_cycles(0xF590, 5);
    if (RAM(RESPAWN_TIMER2) != 0) {
        mff_cycles(0xF592, 1);
        FALL_INTO(func_F558);
    }
    mff_cycles(0xF594, 7);
    a = RAM(GAME_ACTION);
    if (a == k_player_immune[0]) {
        mff_cycles(0xF598, 1);
        FALL_INTO(func_F558);
    }
    if (cmp_list(0xF59A, 0xF558, a, k_player_immune + 1, 7) < 7)
        FALL_INTO(func_F558);
    mff_cycles(0xF5B6, 2);
    mff_cycles(0xF5B6, 1);

    /* $01: who is off the ground */
    mff_cycles(0xF5BF, 9);
    if (RAM(OBJ_Y) != RAM(PLAYER_GROUND_Y)) {
        mff_cycles(0xF5C6, 7);
        RAM(TMP1) = 0x01;
        mff_cycles(0xF5CA, 1);
    } else {
        mff_cycles(0xF5C4, 1);
        mff_cycles(0xF5CC, 5);
        RAM(TMP1) = 0x00;
    }
    mff_cycles(0xF5D0, 8);
    if (airborne_anim(0xF5D5, RAM(OBJ_ANIM + x))) {
        mff_cycles(0xF5E3, 8);
        RAM(TMP1) |= 0x02;
    }

    mff_cycles(0xF5E9, 7);
    a = RAM(TMP1);
    if (a == 0x01) {                            /* the player's ground line, slot X's obj_y */
        mff_cycles(0xF5ED, 2);
        SYNC_OUT;
        enemy_depth(0xF615, 11, 0xF61D, RAM(PLAYER_GROUND_Y), RAM(OBJ_Y + x));
        return;
    }
    mff_cycles(0xF5EF, 4);
    if (a == 0x02) {                            /* the player's obj_y, slot X's ground line */
        mff_cycles(0xF5F1, 2);
        SYNC_OUT;
        enemy_depth(0xF601, 12, 0xF60A, RAM(OBJ_Y), RAM(OBJ_GROUND_Y + x));
        return;
    }
    mff_cycles(0xF5F3, 4);
    if (a == 0x03) {                            /* both ground lines */
        mff_cycles(0xF5F5, 2);
        SYNC_OUT;
        enemy_depth(0xF628, 11, 0xF630, RAM(PLAYER_GROUND_Y), RAM(OBJ_GROUND_Y + x));
        return;
    }
    mff_cycles(0xF5F7, 6);                      /* both standing */
    CALL_FIXED(0xF5F7, func_F365);
    mff_cycles(0xF5FA, 4);
    c = a >= DEPTH_CLOSE_STANDING;
    if (c) {
        mff_cycles(0xF5FE, 3);
        TAIL_FIXED(0xF679, func_F679);
    }
    mff_cycles(0xF5FC, 2);                      /* BCC across a page */
    SYNC_OUT;
    enemy_reach(1);
}

/* ---- player_attack_test ---- */

/* $F745: SEC / RTS, a miss before anything was changed */
static void attack_no(void)
{
    REGS_IN;

    mff_cycles(0xF745, 8);
    c = 1;
    RTS_RETURN();
}

/* $F862: a miss: obj_y back from $04, SEC / RTS */
static void attack_miss(void)
{
    REGS_IN;

    mff_cycles(0xF862, 16);
    RAM(OBJ_Y + x) = RAM(TMP4);
    c = 1;
    RTS_RETURN();
}

/*
 * $F84C: obj_y back from $04, then C = |dy| >= $F869[attack] (clear: a
 * hit). The last step of player_attack_test (its JMP at 0F:F83A, or
 * reached from $F845).
 */
void func_F84C(void)
{
    REGS_IN;

    mff_cycles(0xF84C, 20);
    RAM(OBJ_Y + x) = RAM(TMP4);
    a = RAM(OBJ_Y);
    c = 1;
    SBC_A(RAM(OBJ_Y + x));
    if (!c) {
        mff_cycles(0xF85A, 4);
        a ^= 0xFF;
        ADC_A(1);
    } else {
        mff_cycles(0xF858, 1);
    }
    mff_cycles(0xF85E, 4);
    c = a >= prg_read((uint16_t)(PLAYER_REACH_Y + y));
    mff_cycles(0xF861, 6);
    RTS_RETURN();
}

/*
 * $F833: |dx| ($00) against the reach behind the player, $F8B1[attack]:
 * then $F84C, or a miss.
 */
void func_F833(void)
{
    REGS_IN;

    mff_cycles(0xF833, 7);
    a = RAM(TMP0);
    c = a >= prg_read((uint16_t)(PLAYER_REACH_X_BACK + y));
    mff_cycles(0xF838, 2);
    if (!c) {
        mff_cycles(0xF83A, 3);
        TAIL_FIXED(0xF84C, func_F84C);
    }
    mff_cycles(0xF838, 1);
    FALL_INTO(attack_miss);
}

/* $F845: |dx| ($00) against the reach in front, $F88D[attack] */
static void attack_reach_front(void)
{
    REGS_IN;

    mff_cycles(0xF845, 7);
    a = RAM(TMP0);
    m = prg_read((uint16_t)(PLAYER_REACH_X_FRONT + y));
    mff_cycles(0xF84A, 2);
    if (a < m)
        FALL_INTO(func_F84C);
    mff_cycles(0xF84A, 1);
    FALL_INTO(attack_miss);
}

/*
 * $F83D: the player is to the left of slot X ($00 = |dx|): facing right is
 * facing it (the reach in front), else the reach behind ($F833).
 * Reached by the JMP at 0F:F828.
 */
void func_F83D(void)
{
    REGS_IN;

    mff_cycles(0xF83D, 10);
    RAM(TMP0) = a;
    if (!(RAM(PLAYER_DIR) & DIR_RIGHT)) {
        mff_cycles(0xF843, 1);
        FALL_INTO(func_F833);
    }
    FALL_INTO(attack_reach_front);
}

/*
 * $F7FF: the grab's hits ($1B-$1D) only reach the held enemy (grab_slot);
 * then Y = attack_id, |dx| between the player and slot X, and the reach
 * on the side the player faces. Reached by the JMP at 0F:F7E0 and from the
 * depth test.
 */
void func_F7FF(void)
{
    REGS_IN;
    int grab_hit = 1;

    mff_cycles(0xF7FF, 7);
    a = RAM(ATTACK_ID);
    if (a == ATTACK_GRAB_HIT_1) {
        mff_cycles(0xF803, 1);
    } else {
        mff_cycles(0xF805, 4);
        if (a == ATTACK_GRAB_HIT_2) {
            mff_cycles(0xF807, 1);
        } else {
            mff_cycles(0xF809, 4);
            if (a != ATTACK_GRAB_HIT_3) {
                mff_cycles(0xF80B, 1);
                grab_hit = 0;
            }
        }
    }
    if (grab_hit) {
        mff_cycles(0xF80D, 5);
        if (x != RAM(GRAB_SLOT)) {
            mff_cycles(0xF80F, 1);
            FALL_INTO(attack_miss);
        }
    }
    mff_cycles(0xF811, 30);
    y = RAM(ATTACK_ID);
    a = RAM(OBJ_X);
    c = 1;
    SBC_A(RAM(OBJ_X + x));
    mff_push(a);
    a = RAM(OBJ_X_HI);
    SBC_A(RAM(OBJ_X_HI + x));
    a = mff_pull();
    if (!c) {                                   /* the player is to the left */
        mff_cycles(0xF824, 7);
        a ^= 0xFF;
        ADC_A(1);
        TAIL_FIXED(0xF83D, func_F83D);
    }
    mff_cycles(0xF822, 1);
    mff_cycles(0xF82B, 10);
    RAM(TMP0) = a;
    if (!(RAM(PLAYER_DIR) & DIR_RIGHT)) {       /* facing left is facing slot X */
        mff_cycles(0xF831, 1);
        FALL_INTO(attack_reach_front);
    }
    FALL_INTO(func_F833);
}

/*
 * $F7EA-$F7FE: the depth test of most attacks: |a - obj_y| below $0C,
 * with a = the player's obj_y ($F7EA; attack_ground_y when that is 0) or
 * attack_ground_y ($F7EF). The JMP into $F7EF charges $F7EF-$F7F4 in one
 * run, a branch into it two.
 */
enum { DEPTH_PLAYER_Y, DEPTH_GROUND_Y, DEPTH_GROUND_Y_JMP };
static void attack_depth(int from)
{
    REGS_IN;

    if (from == DEPTH_GROUND_Y_JMP) {
        mff_cycles(0xF7EF, 11);
        a = RAM(ATTACK_GROUND_Y);
    } else {
        if (from == DEPTH_PLAYER_Y) {
            mff_cycles(0xF7EA, 6);
            a = RAM(OBJ_Y);
            if (a != 0)
                mff_cycles(0xF7ED, 1);
            else
                from = DEPTH_GROUND_Y;
        }
        if (from == DEPTH_GROUND_Y) {
            mff_cycles(0xF7EF, 3);
            a = RAM(ATTACK_GROUND_Y);
        }
        mff_cycles(0xF7F1, 8);
    }
    c = 1;
    SBC_A(RAM(OBJ_Y + x));
    if (!c) {
        mff_cycles(0xF7F7, 4);
        a ^= 0xFF;
        ADC_A(1);
    } else {
        mff_cycles(0xF7F5, 1);
    }
    mff_cycles(0xF7FB, 4);
    if (a >= ATTACK_DEPTH_CLOSE) {
        mff_cycles(0xF7FD, 2);                  /* BCS across a page */
        FALL_INTO(attack_miss);
    }
    FALL_INTO(func_F7FF);
}

/* $F7EA: the depth test from the player's obj_y (the JMP at 0F:F7CA) */
void func_F7EA(void)
{
    attack_depth(DEPTH_PLAYER_Y);
}

/* $F7EF: the depth test from attack_ground_y (the JMP at 0F:F7CD) */
void func_F7EF(void)
{
    attack_depth(DEPTH_GROUND_Y_JMP);
}

/*
 * $F7D0: attacks $14-$17 (Cody's slash, Haggar's back throw, the lariat,
 * the special): attack_ground_y within $14 of obj_y, then JMP $F7FF.
 */
static void attack_special_depth(void)
{
    REGS_IN;

    mff_cycles(0xF7D0, 11);
    a = RAM(ATTACK_GROUND_Y);
    c = 1;
    SBC_A(RAM(OBJ_Y + x));
    if (!c) {
        mff_cycles(0xF7D8, 4);
        a ^= 0xFF;
        ADC_A(1);
    } else {
        mff_cycles(0xF7D6, 1);
    }
    mff_cycles(0xF7DC, 4);
    c = a >= SPECIAL_DEPTH_CLOSE;
    if (!c) {
        mff_cycles(0xF7E0, 3);
        TAIL_FIXED(0xF7FF, func_F7FF);
    }
    mff_cycles(0xF7DE, 1);
    mff_cycles(0xF7E3, 16);
    RAM(OBJ_Y + x) = RAM(TMP4);
    c = 1;
    RTS_RETURN();
}

/*
 * player_attack_test, $F738: does the player's attack reach slot X? Not
 * if the slot is off screen or hidden, the player idle (game_action 0),
 * the slot invulnerable, its id below $1F, $5F or in k_unhittable_ids, or
 * no attack live (attack_live). Slot X in an airborne animation is tested
 * at its ground line (obj_y meanwhile, $04 keeping it). Then the depth
 * test of the attack (k_attack_depth), the held-enemy rule of the grab's
 * hits, and the reach in x on the side the player faces ($F88D in front,
 * $F8B1 behind) and in y ($F869), by attack_id.
 * In: X = slot. Out: C clear = hit; V of the last SBC/ADC; Y = attack_id
 * once that far; obj_y as it was; $00, $04 scratch; X kept.
 * Caller: objects_update (0C:80C0).
 */
void func_F738(void)
{
    REGS_IN;
    int i;

    mff_cycles(0xF738, 6);
    a = RAM(OBJ_FLAGS + x);
    if (!(a & OBJF_ON_SCREEN)) {
        mff_cycles(0xF73B, 1);
        FALL_INTO(attack_no);
    }
    mff_cycles(0xF73D, 4);
    if (a & OBJF_HIDDEN) {
        mff_cycles(0xF73F, 1);
        FALL_INTO(attack_no);
    }
    mff_cycles(0xF741, 5);
    if (RAM(GAME_ACTION) == 0)
        FALL_INTO(attack_no);
    mff_cycles(0xF743, 1);
    mff_cycles(0xF747, 6);
    if (RAM(OBJ_INVULN + x) != 0) {
        mff_cycles(0xF745, 1);
        FALL_INTO(attack_no);
    }
    mff_cycles(0xF74C, 8);
    a = RAM(OBJ_ID + x);
    if (a < OBJ_FIRST_HITTABLE) {
        mff_cycles(0xF745, 1);
        FALL_INTO(attack_no);
    }
    for (i = 0; i < 6; i++) {
        mff_cycles((uint16_t)(0xF753 + 4 * i), 4);
        if (a == k_unhittable_ids[i]) {
            mff_cycles(0xF745, 1);
            FALL_INTO(attack_no);
        }
    }
    mff_cycles(0xF76B, 5);
    if (RAM(ATTACK_LIVE) == 0) {
        mff_cycles(0xF745, 1);
        FALL_INTO(attack_no);
    }
    mff_cycles(0xF76F, 8);
    if (RAM(OBJ_ID + x) == OBJ_5F) {
        mff_cycles(0xF745, 1);
        FALL_INTO(attack_no);
    }
    mff_cycles(0xF776, 15);
    RAM(TMP4) = RAM(OBJ_Y + x);
    if (airborne_anim(0xF780, RAM(OBJ_ANIM + x))) {
        mff_cycles(0xF78E, 9);
        RAM(OBJ_Y + x) = RAM(OBJ_GROUND_Y + x);
    }

    mff_cycles(0xF794, 7);
    a = RAM(ATTACK_ID);
    for (i = 0; i < (int)sizeof k_attack_ids; i++) {
        if (i > 0)
            mff_cycles((uint16_t)(0xF796 + 4 * i), 4);
        if (a == k_attack_ids[i]) {
            mff_cycles((uint16_t)(0xF798 + 4 * i), 1);
            break;
        }
    }
    switch (i < (int)sizeof k_attack_ids ? k_attack_depth[i] : BY_PLAYER_Y_JMP) {
    case BY_PLAYER_Y_JMP:
        mff_cycles(0xF7CA, 3);
        TAIL_FIXED(0xF7EA, func_F7EA);
    case BY_PLAYER_Y:
        SYNC_OUT;
        attack_depth(DEPTH_PLAYER_Y);
        return;
    case BY_SPECIAL:
        FALL_INTO(attack_special_depth);
    case BY_GROUND_Y_JMP:
        mff_cycles(0xF7CD, 3);
        TAIL_FIXED(0xF7EF, func_F7EF);
    default:
        SYNC_OUT;
        attack_depth(DEPTH_GROUND_Y);
        return;
    }
}

/* ---- obj_contact_test ---- */

/*
 * $FA91: contact between the slot 1/2 object ($00/$01 = its x, $02 its y,
 * Y its slot) and slot X: |dx| below $FAC2[its obj_attack], |dy| below
 * $FAC0[...]. C clear = contact. Never executed.
 */
void func_FA91(void)
{
    REGS_IN;

    mff_cycles(0xFA91, 33);
    y = RAM(OBJ_ATTACK + y);
    a = RAM(TMP0);
    c = 1;
    SBC_A(RAM(OBJ_X + x));
    mff_push(a);
    a = RAM(TMP1);
    SBC_A(RAM(OBJ_X_HI + x));
    a = mff_pull();
    if (!c) {
        mff_cycles(0xFAA5, 4);
        a ^= 0xFF;
        ADC_A(1);
    } else {
        mff_cycles(0xFAA3, 1);
    }
    mff_cycles(0xFAA9, 4);
    c = a >= prg_read((uint16_t)(CONTACT_REACH_X + y));
    mff_cycles(0xFAAC, 2);
    if (!c) {
        mff_cycles(0xFAAE, 11);
        a = RAM(TMP2);
        c = 1;
        SBC_A(RAM(OBJ_Y + x));
        if (!c) {
            mff_cycles(0xFAB6, 4);
            a ^= 0xFF;
            ADC_A(1);
        } else {
            mff_cycles(0xFAB4, 1);
        }
        mff_cycles(0xFABA, 4);
        c = a >= prg_read((uint16_t)(CONTACT_REACH_Y + y));
        mff_cycles(0xFABD, 2);
        if (!c)
            mff_cycles(0xFABD, 1);              /* BCC to the next instruction */
    } else {
        mff_cycles(0xFAAC, 1);
    }
    mff_cycles(0xFABF, 6);
    RTS_RETURN();
}

/*
 * $F9E9: the loop of obj_contact_test over $10 = 2, then 1: whether slot
 * $10 (live, on screen, not the weapon, $69 or 9) touches slot X (live,
 * not in k_contact_skip_ids nor the animations $74/$7F/$79, not
 * invulnerable, on screen). Slot X hidden ends the test (C set). A slot
 * that does not count goes on through JMP $FA84 (tail calls each way, the
 * scaffold charges both); the other steps loop here.
 * Out: C clear = contact (Y = the slot 1/2 object's obj_attack, $0F its
 * slot); C set = none.
 * Reached from obj_contact_test and by the JMP at 0F:FA8C.
 */
void func_F9E9(void)
{
    REGS_IN;

    for (;;) {
        mff_cycles(0xF9E9, 9);
        y = RAM(TMP16);
        if (!(RAM(OBJ_STATE + y) & OBJ_LIVE)) {
            mff_cycles(0xF9EE, 2);
            break;
        }
        mff_cycles(0xF9F0, 6);
        if (!(RAM(OBJ_FLAGS + y) & OBJF_ON_SCREEN)) {
            mff_cycles(0xF9F3, 2);
            break;
        }
        mff_cycles(0xF9F5, 8);
        a = RAM(OBJ_ID + y);
        if (a == OBJ_WEAPON) {
            mff_cycles(0xF9FA, 2);
            break;
        }
        mff_cycles(0xF9FC, 4);
        if (a == OBJ_69) {
            mff_cycles(0xF9FE, 1);
            break;
        }
        mff_cycles(0xFA00, 4);
        if (a == OBJ_SPARK_9) {
            mff_cycles(0xFA02, 1);
            break;
        }
        mff_cycles(0xFA04, 6);
        if (!(RAM(OBJ_STATE + x) & OBJ_LIVE)) {
            mff_cycles(0xFA07, 1);
            goto next;
        }
        /* from here on never executed */
        mff_cycles(0xFA09, 6);
        a = RAM(OBJ_ID + x);
        if (a == OBJ_NONE) {
            mff_cycles(0xFA0C, 1);
            goto next;
        }
        if (cmp_list(0xFA0E, 0xFA84, a, k_contact_skip_ids, 16) < 16)
            goto next;
        mff_cycles(0xFA4E, 8);
        a = RAM(OBJ_ANIM + x);
        if (a == ANIM_CONTACT_SKIP_1) {
            mff_cycles(0xFA53, 1);
            goto next;
        }
        mff_cycles(0xFA55, 4);
        if (a == ANIM_CONTACT_SKIP_2) {
            mff_cycles(0xFA57, 1);
            goto next;
        }
        mff_cycles(0xFA59, 4);
        if (a == ANIM_CONTACT_SKIP_3)
            break;
        mff_cycles(0xFA5B, 1);
        mff_cycles(0xFA60, 6);
        if (RAM(OBJ_INVULN + x) != 0) {
            mff_cycles(0xFA63, 1);
            goto next;
        }
        mff_cycles(0xFA65, 6);
        a = RAM(OBJ_FLAGS + x);
        if (!(a & OBJF_ON_SCREEN)) {
            mff_cycles(0xFA68, 1);
            goto next;
        }
        mff_cycles(0xFA6A, 4);
        if (a & OBJF_HIDDEN) {
            mff_cycles(0xFA6C, 1);
            mff_cycles(0xFA8F, 2);
            c = 1;
            mff_cycles(0xFA90, 6);
            RTS_RETURN();
        }
        mff_cycles(0xFA6E, 30);
        RAM(TMP0) = RAM(OBJ_X + y);
        RAM(TMP1) = RAM(OBJ_X_HI + y);
        RAM(TMP2) = RAM(OBJ_Y + y);
        RAM(TMP15) = y;
        CALL_FIXED(0xFA7F, func_FA91);
        mff_cycles(0xFA82, 2);
        if (!c) {                               /* contact */
            mff_cycles(0xFA82, 1);
            mff_cycles(0xFA90, 6);
            RTS_RETURN();
        }
    next:
        mff_cycles(0xFA84, 12);
        RAM(TMP16)--;
        c = 1;                                  /* CMP #0 */
        if (RAM(TMP16) == 0) {
            mff_cycles(0xFA8A, 1);
            mff_cycles(0xFA8F, 2);
            mff_cycles(0xFA90, 6);
            RTS_RETURN();
        }
        mff_cycles(0xFA8C, 3);
    }
    /* $FA5D: JMP $FA84 */
    mff_cycles(0xFA5D, 3);
    TAIL_FIXED(0xFA84, func_FA84);
}

/*
 * $FA84: the next slot ($10 - 1): JMP $F9E9 while it is not 0, else C
 * set. Reached by the JMP at 0F:FA5D.
 */
void func_FA84(void)
{
    REGS_IN;

    mff_cycles(0xFA84, 12);
    RAM(TMP16)--;
    a = RAM(TMP16);
    c = 1;
    if (a != 0) {
        mff_cycles(0xFA8C, 3);
        TAIL_FIXED(0xF9E9, func_F9E9);
    }
    mff_cycles(0xFA8A, 1);
    mff_cycles(0xFA8F, 8);
    RTS_RETURN();
}

/*
 * obj_contact_test, $F9E5 (name proposed): does the object in slot 2 or
 * slot 1 (the player's side: dropped weapon, display objects) touch slot
 * X? $10 = 2, then $F9E9. Only its first checks ever ran: the slot 1/2
 * objects of the routes are never candidates, or slot X is not live.
 * In: X = slot. Out: C clear = contact; X kept.
 * Caller: objects_update (0C:8090).
 */
void func_F9E5(void)
{
    REGS_IN;

    mff_cycles(0xF9E5, 5);
    RAM(TMP16) = 0x02;
    FALL_INTO(func_F9E9);
}
