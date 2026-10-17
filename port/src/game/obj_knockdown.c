/*
 * obj_knockdown.c - what happens to an enemy after a hit: pushed back
 * (id 2), knocked down (3 by a thrown body, 5 by a heavy hit), killed (6
 * by a light attack, 7 and $16 by others), the fall into a pit (id $1F),
 * and the pieces they share: the pit check, lying and getting up
 * (obj_knockdown_recover), the blink, the knockdown animations.
 * docs/objects.md, "Knockdown, getting up, dying".
 *
 * The hit reactions run on obj_timer: 0 while thrown up, 1 after the
 * first bounce, 2 once landed again; at 2 the knockdown ends in
 * obj_knockdown_recover (pushed back, knocked down) or the blink (killed).
 * obj_state's phase 0 is the setup frame.
 */
#include "obj_defs.h"

#define SFX_LAND            0x20    /* the thud of a body landing */
#define OBJ_HIDDEN          0x04    /* obj_flags bit 2: not drawn, not hit */

/* the launch of an enemy killed or pushed back by a light attack, by
 * player_level: obj_vy_frac / obj_vy (stronger levels throw higher) */
#define LEVEL_VY_FRAC       0x9A33
#define LEVEL_VY            0x9A39

/* by obj_kind: the knocked-down animation and its sprite bank, the lying
 * ones, the getting-up ones, and the immunity after getting up */
#define KIND_DOWN_ANIM      0xFC8F
#define KIND_DOWN_CHR       0xFC9D
#define KIND_LYING_ANIM     0xFCAB
#define KIND_LYING_CHR      0x9C9F
#define KIND_UP_ANIM        0x9CD3
#define KIND_UP_CHR         0x9CE0
#define KIND_CHR_SLOT       0xFC33
#define KIND_UP_INVULN      0xFC82

/* obj_invuln stages of obj_knockdown_recover */
#define KD_LYING            0xFE    /* obj_down_timer $2D frames */
#define KD_GETTING_UP       0xFD    /* obj_down_timer $17 frames */

/* animations that give no experience when killed ($9977, $9A54) */
static const uint8_t k_no_exp_anims[] = { 0xBB, 0xC7, 0xFD };

/* obj_invuln while getting up */
static const uint8_t k_getting_up[] = { KD_LYING, KD_GETTING_UP };

static void knocked_down_step(void);

/*
 * pushed_back, $9733 (name proposed): behaviour of id 2, a light hit.
 * Phase 0: obj_timer 0, slides at obj_vx $0099 and goes up at the
 * level's speed ($9A33/$9A39 by player_level; attack 7: $0498 up and
 * $00CC across); turned away from the player (0C:981B), the knocked-down
 * animation (0C:9C55), obj_timer2 $10, and obj_dir = the way the player
 * faces. Then each frame it moves (forward, obj_move_x_clamped) and falls
 * ($97E4) until obj_timer reaches 2: JMP $9818 (obj_knockdown_recover).
 * In: X = slot. Out: as the path taken.
 * Caller: obj_behavior_lo/hi (id 2).
 */
void func_9733_b6(void)
{
    REGS_IN;

    mff_cycles(0x9733, 8);
    a = RAM(OBJ_STATE + x) & OBJ_PHASE;
    if (a != 0) {
        mff_cycles(0x9738, 1);
    } else {
        mff_cycles(0x973A, 24);
        RAM(OBJ_TIMER + x) = a;
        RAM(OBJ_VX + x) = a;
        RAM(OBJ_VX_FRAC + x) = 0x99;
        a = RAM(ATTACK_ID);
        c = a >= 0x07;
        if (a == 0x07) {
            mff_cycles(0x974B, 24);             /* ... LDA #4; STA; BNE taken */
            RAM(OBJ_VX_FRAC + x) = 0xCC;
            RAM(OBJ_VY_FRAC + x) = 0x98;
            a = 0x04;
            SET_NZ(a);
            RAM(OBJ_VY + x) = a;
        } else {
            mff_cycles(0x9749, 1);
            mff_cycles(0x975C, 7);
            y = RAM(PLAYER_LEVEL);
            a = prg_read((uint16_t)(LEVEL_VY_FRAC + y));
            mff_cycles(0x9761, 9);
            RAM(OBJ_VY_FRAC + x) = a;
            a = prg_read((uint16_t)(LEVEL_VY + y));
            SET_NZ(a);
            mff_cycles(0x9767, 5);
            RAM(OBJ_VY + x) = a;
        }
        mff_cycles(0x976A, 6);
        CALL_BANK(0x976A, 0x981B);
        mff_cycles(0x976D, 13);
        t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_STATE + x) = t;
        SET_NZ(t);
        CALL_BANK(0x9770, 0x9C55);
        mff_cycles(0x9773, 13);
        a = 0x10;
        SET_NZ(a);
        RAM(OBJ_TIMER2 + x) = a;
        CALL_FIXED(0x9778, func_F314);          /* obj_dir toward the player */
        mff_cycles(0x977B, 9);
        a = RAM(PLAYER_DIR);
        if (a != RAM(OBJ_DIR + x)) {
            mff_cycles(0x9782, 6);
            a = RAM(OBJ_DIR + x) ^ 0x03;
        } else {
            mff_cycles(0x9780, 1);
        }
        mff_cycles(0x9787, 5);
        RAM(OBJ_DIR + x) = a;
    }
    mff_cycles(0x978A, 8);
    a = RAM(OBJ_TIMER + x);
    CMP_FLAGS(a, 0x02);
    if (z)
        JMP_TAIL(0x9791, 0x9818);
    mff_cycles(0x978F, 1);
    SYNC_OUT;
    obj_move_x_clamped(0x9794, 1, func_97E4_b6);
}

/*
 * $97E4: y -= obj_vy with gravity; falling and down to obj_land_y it
 * lands: obj_pit_check, sound $20, a bounce (obj_vy $0238, obj_vx_frac
 * $80), obj_timer + 1, the lying animation (0C:9C7A).
 * In: X = slot. Out: the RTS.
 * Callers: JMP at 0C:97BE (and the clamps); the fall-through from 0C:97E1.
 */
void func_97E4_b6(void)
{
    REGS_IN;

    mff_cycles(0x97E4, 6);
    CALL_FIXED(0x97E4, func_F1FD);
    mff_cycles(0x97E7, 6);
    a = RAM(OBJ_VY + x);
    SET_NZ(a);
    if (n) {
        mff_cycles(0x97EC, 10);
        a = RAM(OBJ_Y + x);
        CMP_FLAGS(a, RAM(OBJ_LAND_Y + x));
        if (c) {
            mff_cycles(0x97F4, 15);
            a = RAM(OBJ_LAND_Y + x);
            SET_NZ(a);
            RAM(OBJ_Y + x) = a;
            CALL_BANK(0x97FA, 0x9B1F);          /* obj_pit_check */
            mff_cycles(0x97FD, 8);
            a = SFX_LAND;
            SET_NZ(a);
            CALL_FIXED(0x97FF, func_F34C);
            mff_cycles(0x9802, 34);
            RAM(OBJ_VY_FRAC + x) = 0x38;
            RAM(OBJ_VY + x) = 0x02;
            a = 0x80;
            RAM(OBJ_VX_FRAC + x) = a;
            t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
            RAM(OBJ_TIMER + x) = t;
            SET_NZ(t);
            CALL_BANK(0x9814, 0x9C7A);
        } else {
            mff_cycles(0x97F2, 2);
        }
    } else {
        mff_cycles(0x97EA, 2);
    }
    mff_cycles(0x9817, 6);
    RTS_RETURN();
}

/*
 * $9818: JMP obj_knockdown_recover (pushed_back's end).
 * Caller: JMP at 0C:9791.
 */
void func_9818_b6(void)
{
    mff_jmp_charge(0x9818, 0x9BFC);
    call_by_address_tail(0x9BFC, 6);
}

/*
 * obj_turn_from_player, $981B (name proposed): obj_dir toward the player
 * (0F:F314); if player_dir differs, obj_dir is reversed (it is then the
 * way the player faces) and that is all (RTS); if they agree, the sprite
 * faces against obj_dir (JMP 0F:F099 when it is right, 0F:F090 when left).
 * In: X = slot. Out: A = obj_dir, or as 0F:F090/F099.
 * Callers: pushed_back (0C:976A), the killed behaviours (0C:9994, 9A70).
 */
void func_981B_b6(void)
{
    REGS_IN;

    mff_cycles(0x981B, 6);
    CALL_FIXED(0x981B, func_F314);
    mff_cycles(0x981E, 9);
    a = RAM(PLAYER_DIR);
    c = a >= RAM(OBJ_DIR + x);
    if (a != RAM(OBJ_DIR + x)) {
        mff_cycles(0x9825, 17);
        a = RAM(OBJ_DIR + x) ^ 0x03;
        SET_NZ(a);
        RAM(OBJ_DIR + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0x9823, 1);
    mff_cycles(0x982E, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    SET_NZ(a);
    if (!z)
        JMP_FIXED(0x9835, 0xF099, func_F099);
    mff_cycles(0x9833, 1);
    JMP_FIXED(0x9838, 0xF090, func_F090);
}

/*
 * knocked_down, $983B (name proposed): behaviour of ids 3 (knocked down by
 * a thrown body) and 5 (by a heavy hit). Phase 0: obj_timer 0, slides at
 * obj_vx $0099, obj_timer2 $10; id 5 also turns: obj_dir toward the player,
 * reversed when the player faces the same way. Then $986E.
 * In: X = slot. Out: as the path taken.
 * Caller: obj_behavior_lo/hi (ids 3, 5).
 */
void func_983B_b6(void)
{
    REGS_IN;

    mff_cycles(0x983B, 8);
    a = RAM(OBJ_STATE + x) & OBJ_PHASE;
    if (a != 0) {
        mff_cycles(0x9840, 1);
        SYNC_OUT;
        knocked_down_step();
        return;
    }
    mff_cycles(0x9842, 32);
    RAM(OBJ_TIMER + x) = a;
    RAM(OBJ_VX + x) = a;
    RAM(OBJ_VX_FRAC + x) = 0x99;
    RAM(OBJ_TIMER2 + x) = 0x10;
    a = RAM(OBJ_ID + x);
    CMP_FLAGS(a, OBJ_KNOCKED_BY_BODY);
    if (z)
        JMP_TAIL(0x9859, 0x986E);
    mff_cycles(0x9857, 1);
    mff_cycles(0x985C, 6);
    CALL_FIXED(0x985C, func_F314);
    mff_cycles(0x985F, 9);
    a = RAM(PLAYER_DIR);
    c = a >= RAM(OBJ_DIR + x);
    if (a == RAM(OBJ_DIR + x)) {
        mff_cycles(0x9866, 6);
        a = RAM(OBJ_DIR + x) ^ 0x03;
    } else {
        mff_cycles(0x9864, 1);
    }
    mff_cycles(0x986B, 5);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    func_986E_b6();
}

/*
 * $986E: the next phase and the knocked-down animation (0C:9C55), then
 * the frame's step.
 * Callers: JMP at 0C:9859 (id 3); the fall-through from 0C:986B.
 */
void func_986E_b6(void)
{
    REGS_IN;

    mff_cycles(0x986E, 13);
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
    SET_NZ(t);
    CALL_BANK(0x9871, 0x9C55);
    SYNC_OUT;
    knocked_down_step();
}

/*
 * $9874-$98BA: once obj_timer is 2 (landed twice) JMP $9912; otherwise it
 * moves forward (obj_move_x_sides, the JMP $9912 sits between the sides)
 * and falls ($98DE).
 */
static void knocked_down_step(void)
{
    REGS_IN;

    mff_cycles(0x9874, 8);
    a = RAM(OBJ_ID + x);
    if (a == OBJ_KNOCKED_BY_BODY) {
        mff_cycles(0x987B, 8);
        a = RAM(OBJ_TIMER + x);
        CMP_FLAGS(a, 0x02);
        if (z) {
            mff_cycles(0x9880, 1);
            JMP_TAIL(0x98B8, 0x9912);
        }
        mff_cycles(0x9882, 3);                  /* BNE $988B */
    } else {
        mff_cycles(0x9879, 1);
        mff_cycles(0x9884, 8);
        a = RAM(OBJ_TIMER + x);
        CMP_FLAGS(a, 0x02);
        if (z) {
            mff_cycles(0x9889, 1);
            JMP_TAIL(0x98B8, 0x9912);
        }
    }
    SYNC_OUT;
    obj_move_x_sides(0x988B, 0x98BB, 1, func_98DE_b6);
}

/*
 * $98DE: y -= obj_vy with gravity; falling and down to obj_land_y it
 * lands: obj_pit_check, sound $20, a bounce (obj_vy $0238, obj_vx_frac
 * $80), obj_timer + 1, JMP 0C:9C7A (the lying animation).
 * Callers: JMP at 0C:98B5 (and the clamps); the fall-through from 0C:98DB.
 */
void func_98DE_b6(void)
{
    REGS_IN;

    mff_cycles(0x98DE, 6);
    CALL_FIXED(0x98DE, func_F1FD);
    mff_cycles(0x98E1, 6);
    a = RAM(OBJ_VY + x);
    SET_NZ(a);
    if (!n) {
        mff_cycles(0x98E4, 8);                  /* BPL into the next page; RTS */
        RTS_RETURN();
    }
    mff_cycles(0x98E6, 10);
    a = RAM(OBJ_Y + x);
    CMP_FLAGS(a, RAM(OBJ_LAND_Y + x));
    if (!c) {
        mff_cycles(0x98EC, 8);
        RTS_RETURN();
    }
    mff_cycles(0x98EE, 15);
    a = RAM(OBJ_LAND_Y + x);
    SET_NZ(a);
    RAM(OBJ_Y + x) = a;
    CALL_BANK(0x98F4, 0x9B1F);                  /* obj_pit_check */
    mff_cycles(0x98F7, 8);
    a = SFX_LAND;
    SET_NZ(a);
    CALL_FIXED(0x98F9, func_F34C);
    mff_cycles(0x98FC, 28);
    RAM(OBJ_VY_FRAC + x) = 0x38;
    RAM(OBJ_VY + x) = 0x02;
    a = 0x80;
    RAM(OBJ_VX_FRAC + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    SET_NZ(t);
    JMP_TAIL(0x990E, 0x9C7A);
}

/*
 * $9912: the end of a knockdown. Id 5 goes to obj_knockdown_recover.
 * Id 3 (a thrown body's victim) blinks out if its phase has bit 1 set;
 * getting up already, the recovery; otherwise it takes the body's
 * damage now (attack 9 through obj_take_hit, attack_id then 0): alive,
 * obj_invuln $FF and the recovery; killed (never executed), the next
 * phase, a kill counted, and the blink.
 * In: X = slot. Out: as obj_knockdown_recover or the blink.
 * Caller: JMP at 0C:98B8.
 */
void func_9912_b6(void)
{
    REGS_IN;
    int i;

    mff_cycles(0x9912, 8);
    a = RAM(OBJ_ID + x);
    CMP_FLAGS(a, OBJ_KNOCKED_BY_BODY);
    if (!z) {
        mff_cycles(0x9919, 3);                  /* BNE $9920 */
        JMP_TAIL(0x9920, 0x9BFC);
    }
    mff_cycles(0x9917, 1);
    mff_cycles(0x9923, 8);
    a = RAM(OBJ_STATE + x) & 0x02;
    if (a != 0) {
        mff_cycles(0x9928, 1);
        SYNC_OUT;
        obj_blink(0x994A, 0x995E);
        return;
    }
    mff_cycles(0x992A, 4);
    a = RAM(OBJ_INVULN + x);
    i = cmp_list(0x992D, 0x9920, a, k_getting_up, 2);
    CMP_LIST_FLAGS(a, k_getting_up, 2, i);
    if (i < 2)
        JMP_TAIL(0x9920, 0x9BFC);
    mff_cycles(0x9935, 11);
    a = ATTACK_HEADSMASHER;
    SET_NZ(a);
    RAM(ATTACK_ID) = a;
    CALL_BANK(0x9939, 0x84F0);                  /* obj_take_hit */
    mff_cycles(0x993C, 11);
    RAM(ATTACK_ID) = 0x00;
    a = RAM(OBJ_LIFE + x);
    if (!(a & 0x80)) {
        mff_cycles(0x9943, 1);
        mff_cycles(0x991B, 7);
        a = 0xFF;
        SET_NZ(a);
        RAM(OBJ_INVULN + x) = a;
        JMP_TAIL(0x9920, 0x9BFC);
    }
    mff_cycles(0x9945, 12);
    RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(AREA_KILLS) = (uint8_t)(RAM(AREA_KILLS) + 1);
    SYNC_OUT;
    obj_blink(0x994A, 0x995E);
}

/*
 * killed_light, $9962 (name proposed): behaviour of id 6, killed by a
 * light attack. Phase 0: obj_timer 0, slides at obj_vx $0099, experience
 * for the kill (exp_award, unless its animation is $BB, $C7 or $FD),
 * thrown up at the level's speed ($9A33/$9A39), turned (0C:981B), the
 * knocked-down animation, obj_timer2 $10. Then it moves forward and falls
 * ($99FC) until obj_timer is 2: the lying animation and the blink ($9A2D).
 * In: X = slot. Out: as the path taken.
 * Caller: obj_behavior_lo/hi (id 6).
 */
void func_9962_b6(void)
{
    REGS_IN;
    int i;

    mff_cycles(0x9962, 8);
    a = RAM(OBJ_STATE + x) & OBJ_PHASE;
    if (a == 0) {
        mff_cycles(0x9969, 21);
        RAM(OBJ_TIMER + x) = a;
        RAM(OBJ_VX + x) = a;
        RAM(OBJ_VX_FRAC + x) = 0x99;
        a = RAM(OBJ_ANIM + x);
        i = cmp_list(0x9977, 0x9986, a, k_no_exp_anims, 3);
        CMP_LIST_FLAGS(a, k_no_exp_anims, 3, i);
        if (i == 3) {
            mff_cycles(0x9983, 6);
            CALL_BANK(0x9983, 0x89E3);          /* exp_award */
        }
        mff_cycles(0x9986, 7);
        y = RAM(PLAYER_LEVEL);
        a = prg_read((uint16_t)(LEVEL_VY_FRAC + y));
        mff_cycles(0x998B, 9);
        RAM(OBJ_VY_FRAC + x) = a;
        a = prg_read((uint16_t)(LEVEL_VY + y));
        SET_NZ(a);
        mff_cycles(0x9991, 11);
        RAM(OBJ_VY + x) = a;
        CALL_BANK(0x9994, 0x981B);
        mff_cycles(0x9997, 13);
        t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_STATE + x) = t;
        SET_NZ(t);
        CALL_BANK(0x999A, 0x9C55);
        mff_cycles(0x999D, 7);
        a = 0x10;
        RAM(OBJ_TIMER2 + x) = a;
    } else {
        mff_cycles(0x9967, 1);
    }
    mff_cycles(0x99A2, 8);
    a = RAM(OBJ_TIMER + x);
    CMP_FLAGS(a, 0x02);
    if (z)
        JMP_TAIL(0x99A9, 0x9A2D);
    mff_cycles(0x99A7, 1);
    SYNC_OUT;
    obj_move_x_clamped(0x99AC, 1, func_99FC_b6);
}

/*
 * $99FC: y -= obj_vy with gravity; falling and down to obj_land_y it
 * lands: obj_pit_check, sound $20, a bounce (obj_vy $0238, obj_vx_frac
 * $80), obj_timer + 1.
 * Callers: JMP at 0C:99D6 (and the clamps); the fall-through from 0C:99F9.
 */
void func_99FC_b6(void)
{
    REGS_IN;

    mff_cycles(0x99FC, 6);
    CALL_FIXED(0x99FC, func_F1FD);
    mff_cycles(0x99FF, 6);
    a = RAM(OBJ_VY + x);
    SET_NZ(a);
    if (n) {
        mff_cycles(0x9A04, 10);
        a = RAM(OBJ_Y + x);
        CMP_FLAGS(a, RAM(OBJ_LAND_Y + x));
        if (c) {
            mff_cycles(0x9A0C, 15);
            a = RAM(OBJ_LAND_Y + x);
            SET_NZ(a);
            RAM(OBJ_Y + x) = a;
            CALL_BANK(0x9A12, 0x9B1F);          /* obj_pit_check */
            mff_cycles(0x9A15, 8);
            a = SFX_LAND;
            SET_NZ(a);
            CALL_FIXED(0x9A17, func_F34C);
            mff_cycles(0x9A1A, 28);
            RAM(OBJ_VY_FRAC + x) = 0x38;
            RAM(OBJ_VY + x) = 0x02;
            a = 0x80;
            RAM(OBJ_VX_FRAC + x) = a;
            t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
            RAM(OBJ_TIMER + x) = t;
            SET_NZ(t);
        } else {
            mff_cycles(0x9A0A, 1);
        }
    } else {
        mff_cycles(0x9A02, 1);
    }
    mff_cycles(0x9A2C, 6);
    RTS_RETURN();
}

/*
 * $9A2D: the lying animation (0C:9C7A), then the blink (JMP $9B07).
 * Caller: JMP at 0C:99A9.
 */
void func_9A2D_b6(void)
{
    REGS_IN;

    mff_cycles(0x9A2D, 6);
    CALL_BANK(0x9A2D, 0x9C7A);
    JMP_TAIL(0x9A30, 0x9B07);
}

/*
 * killed, $9A3F (name proposed): behaviour of ids 7 and $16, killed by
 * another attack (or by a slam, $16). As killed_light, with experience
 * unless the animation is $BB or $C7, a fixed launch (obj_vy $039E), no
 * turn for id $16, no landing sound, and the blink through $9B04.
 * In: X = slot. Out: as the path taken.
 * Caller: obj_behavior_lo/hi (ids 7, $16).
 */
void func_9A3F_b6(void)
{
    REGS_IN;
    int i;

    mff_cycles(0x9A3F, 8);
    a = RAM(OBJ_STATE + x) & OBJ_PHASE;
    if (a == 0) {
        mff_cycles(0x9A46, 21);
        RAM(OBJ_TIMER + x) = a;
        RAM(OBJ_VX + x) = a;
        RAM(OBJ_VX_FRAC + x) = 0x99;
        a = RAM(OBJ_ANIM + x);
        i = cmp_list(0x9A54, 0x9A5F, a, k_no_exp_anims, 2);
        if (i == 2) {
            CMP_FLAGS(a, k_no_exp_anims[1]);
            mff_cycles(0x9A5C, 6);
            CALL_BANK(0x9A5C, 0x89E3);          /* exp_award */
        }
        mff_cycles(0x9A5F, 22);
        RAM(OBJ_VY_FRAC + x) = 0x9E;
        RAM(OBJ_VY + x) = 0x03;
        a = RAM(OBJ_ID + x);
        CMP_FLAGS(a, 0x16);
        if (!z) {
            mff_cycles(0x9A70, 6);
            CALL_BANK(0x9A70, 0x981B);
        } else {
            mff_cycles(0x9A6E, 1);
        }
        mff_cycles(0x9A73, 13);
        t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_STATE + x) = t;
        SET_NZ(t);
        CALL_BANK(0x9A76, 0x9C55);
        mff_cycles(0x9A79, 7);
        a = 0x10;
        RAM(OBJ_TIMER2 + x) = a;
    } else {
        mff_cycles(0x9A44, 1);
    }
    mff_cycles(0x9A7E, 8);
    a = RAM(OBJ_TIMER + x);
    CMP_FLAGS(a, 0x02);
    if (z)
        JMP_TAIL(0x9A85, 0x9B04);
    mff_cycles(0x9A83, 1);
    SYNC_OUT;
    obj_move_x_clamped(0x9A88, 1, func_9AD8_b6);
}

/*
 * $9AD8: y -= obj_vy with gravity; falling and down to obj_land_y it
 * lands: obj_pit_check, a bounce (obj_vy $0238, obj_vx_frac $80),
 * obj_timer + 1.
 * Callers: JMP at 0C:9AB2 (and the clamps); the fall-through from 0C:9AD5.
 */
void func_9AD8_b6(void)
{
    REGS_IN;

    mff_cycles(0x9AD8, 6);
    CALL_FIXED(0x9AD8, func_F1FD);
    mff_cycles(0x9ADB, 6);
    a = RAM(OBJ_VY + x);
    SET_NZ(a);
    if (n) {
        mff_cycles(0x9AE0, 10);
        a = RAM(OBJ_Y + x);
        CMP_FLAGS(a, RAM(OBJ_LAND_Y + x));
        if (c) {
            mff_cycles(0x9AE8, 15);
            a = RAM(OBJ_LAND_Y + x);
            SET_NZ(a);
            RAM(OBJ_Y + x) = a;
            CALL_BANK(0x9AEE, 0x9B1F);          /* obj_pit_check */
            mff_cycles(0x9AF1, 28);
            RAM(OBJ_VY_FRAC + x) = 0x38;
            RAM(OBJ_VY + x) = 0x02;
            a = 0x80;
            RAM(OBJ_VX_FRAC + x) = a;
            t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
            RAM(OBJ_TIMER + x) = t;
            SET_NZ(t);
        } else {
            mff_cycles(0x9AE6, 2);
        }
    } else {
        mff_cycles(0x9ADE, 2);
    }
    mff_cycles(0x9B03, 6);
    RTS_RETURN();
}

/*
 * $9B04: the lying animation (0C:9C7A), then the blink ($9B07).
 * Caller: JMP at 0C:9A85.
 */
void func_9B04_b6(void)
{
    REGS_IN;

    mff_cycles(0x9B04, 6);
    CALL_BANK(0x9B04, 0x9C7A);
    SYNC_OUT;
    func_9B07_b6();
}

/*
 * $9B07: the blink of a killed enemy (obj_blink), then object_clear.
 * Callers: JMP at 0C:9A30; the fall-through from $9B04.
 */
void func_9B07_b6(void)
{
    obj_blink(0x9B07, 0x9B03);
}

/*
 * obj_pit_check, $9B1F: where the zone allows pits (0C:9BE1, which
 * otherwise returns from this routine too), probes the background under
 * the object (0F:ECCF with Y = 6; kind 6 probes 12 px higher, obj_y
 * restored from TMP15 after). On a pit tile (TMP16 bit 6) it becomes a pit
 * fall: obj_saved_id = id, id $1F, obj_pit_flag $1F, obj_timer 0,
 * obj_invuln $FF, and the caller's return address is dropped (PLA PLA),
 * so the RTS leaves the behaviour that called it.
 * In: X = slot. Out: A = high byte pulled, or TMP16 & $40 (0); N/Z from A.
 * Callers: the landings of the hit reactions and thrown bodies (0C:8ECB,
 * 8F5C, 91AB, 921F, 9326, 9673, 9702, 97FA, 98F4, 9A12, 9AEE).
 */
void func_9B1F_b6(void)
{
    REGS_IN;

    mff_cycles(0x9B1F, 6);
    CALL_BANK(0x9B1F, 0x9BE1);
    mff_cycles(0x9B22, 15);
    a = RAM(OBJ_Y + x);
    RAM(TMP15) = a;
    a = RAM(OBJ_KIND + x);
    c = a >= KIND_6;
    if (a == KIND_6) {
        mff_cycles(0x9B2E, 13);
        a = RAM(OBJ_Y + x);
        c = 1;
        SBC_A(0x0C);
        RAM(OBJ_Y + x) = a;
    } else {
        mff_cycles(0x9B2C, 1);
    }
    mff_cycles(0x9B37, 8);
    y = 0x06;
    SET_NZ(y);
    CALL_FIXED(0x9B39, func_ECCF);
    mff_cycles(0x9B3C, 15);
    a = RAM(TMP15);
    RAM(OBJ_Y + x) = a;
    a = RAM(TMP16) & 0x40;
    SET_NZ(a);
    if (!z) {
        mff_cycles(0x9B47, 43);
        RAM(OBJ_SAVED_ID + x) = RAM(OBJ_ID + x);
        a = OBJ_PIT_FALL;
        RAM(OBJ_ID + x) = a;
        RAM(OBJ_PIT_FLAG + x) = a;
        RAM(OBJ_TIMER + x) = 0x00;
        RAM(OBJ_INVULN + x) = 0xFF;
        a = mff_pull();
        a = mff_pull();
        SET_NZ(a);
    } else {
        mff_cycles(0x9B45, 1);
    }
    mff_cycles(0x9B61, 6);
    RTS_RETURN();
}

/*
 * obj_hide, $9B62 (name proposed): obj_flags bit 2 (hidden) set.
 * In: X = slot. Out: A = obj_flags, N/Z from it (Z clear).
 * Callers: the blinks (0C:8F99, 9956, 9B13) and the pit fall (0C:9B98).
 */
void func_9B62_b6(void)
{
    REGS_IN;

    mff_cycles(0x9B62, 17);
    a = RAM(OBJ_FLAGS + x) | OBJ_HIDDEN;
    SET_NZ(a);
    RAM(OBJ_FLAGS + x) = a;
    RTS_RETURN();
}

/*
 * obj_show, $9B6B (name proposed): obj_flags bit 2 (hidden) cleared.
 * In: X = slot. Out: A = obj_flags, N/Z from it.
 * Callers: the blinks (0C:8F9E, 995B, 9B18).
 */
void func_9B6B_b6(void)
{
    REGS_IN;

    mff_cycles(0x9B6B, 17);
    a = RAM(OBJ_FLAGS + x) & (uint8_t)~OBJ_HIDDEN;
    SET_NZ(a);
    RAM(OBJ_FLAGS + x) = a;
    RTS_RETURN();
}

/*
 * pit_fall, $9B74 (name proposed): behaviour of id $1F, an object falling
 * into a pit. obj_invuln stays $FF. While obj_timer is 0 it falls straight
 * down (obj_vx 0, 0F:F1FD; its animation frozen unless obj_pit_flag is
 * set) until obj_y reaches $B0: then hidden; kind 6, or an object dead
 * already, is cleared at once; otherwise the player gets experience (as
 * attack 3) when its saved id is below $20, when the player had just let
 * it go (obj_held $FF) or when its animation is $9D; then obj_life $FF,
 * its HUD bar, obj_ai_timer $10, a kill counted, obj_timer 1. With
 * obj_timer set it waits obj_ai_timer frames and is cleared.
 * In: X = slot. Out: the RTS, or as object_clear.
 * Caller: obj_behavior_lo/hi (id $1F).
 */
void func_9B74_b6(void)
{
    REGS_IN;
    int exp;

    mff_cycles(0x9B74, 13);
    RAM(OBJ_INVULN + x) = 0xFF;
    a = RAM(OBJ_TIMER + x);
    if (a != 0) {
        mff_cycles(0x9B7C, 1);
        mff_cycles(0x9BD9, 9);
        t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
        RAM(OBJ_AI_TIMER + x) = t;
        SET_NZ(t);
        if (!z) {
            mff_cycles(0x9BDC, 7);              /* BNE taken; RTS */
            RTS_RETURN();
        }
        JMP_FIXED(0x9BDE, 0xF2BE, func_F2BE);
    }
    mff_cycles(0x9B7E, 6);
    a = RAM(OBJ_PIT_FLAG + x);
    if (a == 0) {
        mff_cycles(0x9B83, 5);
        RAM(OBJ_ANIM_TIMER + x) = a;
    } else {
        mff_cycles(0x9B81, 1);
    }
    mff_cycles(0x9B86, 18);
    a = 0x00;
    SET_NZ(a);
    RAM(OBJ_VX + x) = a;
    RAM(OBJ_VX_FRAC + x) = a;
    CALL_FIXED(0x9B8E, func_F1FD);
    mff_cycles(0x9B91, 8);
    a = RAM(OBJ_Y + x);
    CMP_FLAGS(a, 0xB0);
    if (!c) {
        mff_cycles(0x9B96, 7);                  /* BCC taken; RTS */
        RTS_RETURN();
    }
    mff_cycles(0x9B98, 6);
    CALL_BANK(0x9B98, 0x9B62);                  /* obj_hide */
    mff_cycles(0x9B9B, 8);
    a = RAM(OBJ_KIND + x);
    CMP_FLAGS(a, KIND_6);
    if (z) {
        mff_cycles(0x9BA0, 1);
        JMP_FIXED(0x9BDE, 0xF2BE, func_F2BE);
    }
    mff_cycles(0x9BA2, 6);
    a = RAM(OBJ_LIFE + x);
    SET_NZ(a);
    if (n) {
        mff_cycles(0x9BA5, 1);
        JMP_FIXED(0x9BDE, 0xF2BE, func_F2BE);
    }

    mff_cycles(0x9BA7, 8);
    a = RAM(OBJ_SAVED_ID + x);
    c = a >= 0x20;
    exp = !c;
    if (exp) {
        mff_cycles(0x9BAC, 1);
    } else {
        mff_cycles(0x9BAE, 8);
        a = RAM(OBJ_HELD + x);
        c = a >= 0xFF;
        exp = a == 0xFF;
        if (exp) {
            mff_cycles(0x9BB3, 1);
        } else {
            mff_cycles(0x9BB5, 8);
            a = RAM(OBJ_ANIM + x);
            c = a >= 0x9D;
            exp = a == 0x9D;
            if (!exp)
                mff_cycles(0x9BBA, 1);
        }
    }
    if (exp) {
        mff_cycles(0x9BBC, 11);
        a = 0x03;
        SET_NZ(a);
        RAM(ATTACK_ID) = a;
        CALL_BANK(0x9BC0, 0x89E3);              /* exp_award */
    }
    mff_cycles(0x9BC3, 34);
    RAM(OBJ_LIFE + x) = 0xFF;
    RAM(HUD_ENEMY_SLOT) = x;
    RAM(HUD_ENEMY_TIMER) = 0x00;
    a = 0x10;
    RAM(OBJ_AI_TIMER + x) = a;
    RAM(AREA_KILLS) = (uint8_t)(RAM(AREA_KILLS) + 1);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RAM(OBJ_TIMER + x) = t;
    SET_NZ(t);
    mff_cycles(0x9BD8, 6);
    RTS_RETURN();
}

/*
 * obj_pit_zone, $9BE1 (name proposed): lets its caller probe for a pit
 * only where there can be one: kinds 7 and up (the bosses) and objects
 * left of x $03F0 return normally (C as the compare left it); anything
 * further right returns from the caller as well (PLA PLA), C clear.
 * In: X = slot. Out: C; A and N/Z as described.
 * Callers: obj_pit_check (0C:9B1F), 0D:A9F6, 0D:AA1D.
 */
void func_9BE1_b6(void)
{
    REGS_IN;
    int leave;

    mff_cycles(0x9BE1, 8);
    a = RAM(OBJ_KIND + x);
    CMP_FLAGS(a, 0x07);
    if (c) {
        mff_cycles(0x9BE6, 7);                  /* BCS taken; RTS */
        RTS_RETURN();
    }
    mff_cycles(0x9BE8, 8);
    a = RAM(OBJ_X_HI + x);
    CMP_FLAGS(a, 0x03);
    if (!c) {
        mff_cycles(0x9BED, 7);
        RTS_RETURN();
    }
    mff_cycles(0x9BEF, 2);
    leave = 1;
    if (!z) {
        mff_cycles(0x9BEF, 1);
    } else {
        mff_cycles(0x9BF1, 8);
        a = RAM(OBJ_X + x);
        CMP_FLAGS(a, 0xF0);
        if (!c) {
            mff_cycles(0x9BF6, 1);
            leave = 0;
        }
    }
    if (leave) {
        mff_cycles(0x9BF8, 10);
        a = mff_pull();
        a = mff_pull();
        SET_NZ(a);
        c = 0;
    }
    mff_cycles(0x9BFB, 6);
    RTS_RETURN();
}

/*
 * obj_knockdown_recover, $9BFC: lying and getting up, one step a frame,
 * staged on obj_invuln. Any value but $FE/$FD (the callers set $FF):
 * obj_down_timer $2D, the lying animation (0C:9C7A), obj_invuln - 1 ($FE).
 * $FE: when obj_down_timer runs out, the getting-up animation (0C:9CAD),
 * obj_down_timer $17, obj_invuln $FD. $FD: when it runs out, the object
 * gets up ($9C29).
 * In: X = slot. Out: the RTS.
 * Callers: the hit reactions' and thrown bodies' ends (JMP at 0C:8EF2,
 * 8F8A, 9818, 9920, 932E, 9730, ...; JSR at 0C:91B3).
 */
void func_9BFC_b6(void)
{
    REGS_IN;

    mff_cycles(0x9BFC, 8);
    a = RAM(OBJ_INVULN + x);
    c = a >= KD_LYING;
    if (a == KD_LYING) {
        mff_cycles(0x9C01, 1);
        mff_cycles(0x9C13, 9);
        t = (uint8_t)(RAM(OBJ_DOWN_TIMER + x) - 1);
        RAM(OBJ_DOWN_TIMER + x) = t;
        SET_NZ(t);
        if (!z) {
            mff_cycles(0x9C16, 1);
            mff_cycles(0x9C54, 6);
            RTS_RETURN();
        }
        mff_cycles(0x9C18, 6);
        CALL_BANK(0x9C18, 0x9CAD);
        mff_cycles(0x9C1B, 20);
        a = 0x17;
        RAM(OBJ_DOWN_TIMER + x) = a;
        t = (uint8_t)(RAM(OBJ_INVULN + x) - 1);
        RAM(OBJ_INVULN + x) = t;
        SET_NZ(t);
        RTS_RETURN();
    }
    mff_cycles(0x9C03, 4);
    c = a >= KD_GETTING_UP;
    if (a == KD_GETTING_UP) {
        mff_cycles(0x9C05, 1);
        mff_cycles(0x9C24, 9);
        t = (uint8_t)(RAM(OBJ_DOWN_TIMER + x) - 1);
        RAM(OBJ_DOWN_TIMER + x) = t;
        SET_NZ(t);
        if (!z) {
            mff_cycles(0x9C27, 1);
            mff_cycles(0x9C54, 6);
            RTS_RETURN();
        }
        SYNC_OUT;
        func_9C29_b6();
        return;
    }
    mff_cycles(0x9C07, 13);
    a = 0x2D;
    SET_NZ(a);
    RAM(OBJ_DOWN_TIMER + x) = a;
    CALL_BANK(0x9C0C, 0x9C7A);
    mff_cycles(0x9C0F, 13);
    t = (uint8_t)(RAM(OBJ_INVULN + x) - 1);
    RAM(OBJ_INVULN + x) = t;
    SET_NZ(t);
    RTS_RETURN();
}

/*
 * obj_get_up, $9C29 (name proposed): the object resumes what it was doing:
 * obj_id and obj_state from the saved ones, the pit look-around of the
 * common enemies (0D:AA1D), obj_throw_flag, obj_hit_timer and obj_held
 * cleared, on screen again (obj_flags bit 7), and a short immunity
 * (obj_invuln = $FC82[kind]).
 * In: X = slot. Out: A = the immunity, N/Z from it.
 * Callers: obj_knockdown_recover (falls in at 0C:9C27); the HeadSmasher's
 * body (JSR at 0C:9222).
 */
void func_9C29_b6(void)
{
    REGS_IN;

    mff_cycles(0x9C29, 24);
    RAM(OBJ_ID + x) = RAM(OBJ_SAVED_ID + x);
    a = RAM(OBJ_SAVED_STATE + x);
    SET_NZ(a);
    RAM(OBJ_STATE + x) = a;
    CALL_BANK(0x9C35, 0xAA1D);
    mff_cycles(0x9C38, 36);
    a = 0x00;
    RAM(OBJ_THROW_FLAG + x) = a;
    RAM(OBJ_HIT_TIMER + x) = a;
    RAM(OBJ_HELD + x) = a;
    RAM(OBJ_FLAGS + x) = RAM(OBJ_FLAGS + x) | OBJ_ON_SCREEN;
    y = RAM(OBJ_KIND + x);
    a = prg_read((uint16_t)(KIND_UP_INVULN + y));
    SET_NZ(a);
    mff_cycles(0x9C51, 11);
    RAM(OBJ_INVULN + x) = a;
    RTS_RETURN();
}

/*
 * The knockdown animations of $9C55 and $9C7A (the copy at `pc`): the
 * kind's animation from `anim` unless it shows it already, and, except
 * for kind 6, its sprite bank from `chr` into chr_banks+3+$FC33[kind].
 */
static void kind_anim_bank(uint16_t pc, uint16_t anim, uint16_t chr)
{
    REGS_IN;

    mff_cycles(pc, 10);
    y = RAM(OBJ_KIND + x);
    a = prg_read((uint16_t)(anim + y));
    mff_cycles((uint16_t)(pc + 0x07), 6);
    CMP_FLAGS(a, RAM(OBJ_ANIM + x));
    if (z) {
        mff_cycles((uint16_t)(pc + 0x0A), branch_taken((uint16_t)(pc + 0x0A), (uint16_t)(pc + 0x24)));
    } else {
        mff_cycles((uint16_t)(pc + 0x0C), 6);
        CALL_FIXED((uint16_t)(pc + 0x0C), func_F288);
        mff_cycles((uint16_t)(pc + 0x0F), 8);
        a = RAM(OBJ_KIND + x);
        CMP_FLAGS(a, KIND_6);
        if (z) {
            mff_cycles((uint16_t)(pc + 0x14), branch_taken((uint16_t)(pc + 0x14), (uint16_t)(pc + 0x24)));
        } else {
            mff_cycles((uint16_t)(pc + 0x16), 4);
            a = prg_read((uint16_t)(chr + y));
            mff_cycles((uint16_t)(pc + 0x19), 7);
            RAM(TMP0) = a;
            a = prg_read((uint16_t)(KIND_CHR_SLOT + y));
            mff_cycles((uint16_t)(pc + 0x1E), 10);
            y = a;
            a = RAM(TMP0);
            SET_NZ(a);
            RAM(CHR_BANKS + 3 + y) = a;
        }
    }
    mff_cycles((uint16_t)(pc + 0x24), 6);
    RTS_RETURN();
}

/*
 * obj_anim_knocked, $9C55 (name proposed): the kind's knocked-down
 * animation ($FC8F) and sprite bank ($FC9D).
 * In: X = slot. Out: as kind_anim_bank.
 * Callers: the hit reactions (0C:8E61, 8FE6, 913D, 9609, 9770, 9871,
 * 999A, 9A76).
 */
void func_9C55_b6(void)
{
    kind_anim_bank(0x9C55, KIND_DOWN_ANIM, KIND_DOWN_CHR);
}

/*
 * obj_anim_lying, $9C7A (name proposed): the kind's lying animation
 * ($FCAB) and sprite bank ($9C9F).
 * Callers: JMP at 0C:8F7D, 9723, 990E; JSR at 0C:9814, 9A2D, 9B04, 9C0C.
 */
void func_9C7A_b6(void)
{
    kind_anim_bank(0x9C7A, KIND_LYING_ANIM, KIND_LYING_CHR);
}

/*
 * obj_anim_getting_up, $9CAD (name proposed): the kind's getting-up
 * animation ($9CD3) unless it shows it already, and its sprite bank
 * ($9CE0; kind 8 one slot further).
 * In: X = slot. Out: the RTS.
 * Callers: obj_knockdown_recover (0C:9C18) and the common-enemy AI
 * (0D:A4AD, A4F8, A669, A6D5, A8D3).
 */
void func_9CAD_b6(void)
{
    REGS_IN;

    mff_cycles(0x9CAD, 10);
    y = RAM(OBJ_KIND + x);
    a = prg_read((uint16_t)(KIND_UP_ANIM + y));
    mff_cycles(0x9CB4, 6);
    CMP_FLAGS(a, RAM(OBJ_ANIM + x));
    if (z) {
        mff_cycles(0x9CB7, 1);
    } else {
        mff_cycles(0x9CB9, 6);
        CALL_FIXED(0x9CB9, func_F288);
        mff_cycles(0x9CBC, 4);
        a = prg_read((uint16_t)(KIND_UP_CHR + y));
        mff_cycles(0x9CBF, 7);
        RAM(TMP0) = a;
        a = prg_read((uint16_t)(KIND_CHR_SLOT + y));
        mff_cycles(0x9CC4, 10);
        y = a;
        a = RAM(OBJ_KIND + x);
        c = a >= 0x08;
        if (a == 0x08) {
            mff_cycles(0x9CCC, 2);
            y++;
        } else {
            mff_cycles(0x9CCA, 1);
        }
        mff_cycles(0x9CCD, 8);
        a = RAM(TMP0);
        SET_NZ(a);
        RAM(CHR_BANKS + 3 + y) = a;
    }
    mff_cycles(0x9CD2, 6);
    RTS_RETURN();
}
