/*
 * enemy_moves.c - the special moves of the common enemies (bank $0D):
 * the jump (Poison's back flip, El Gado's jumping attack), El Gado's knife
 * throw and slide, Andore's charge, and the drop from above. They are
 * phases of the AI in enemy_ai.c, started by enemy_maneuver_pick (the
 * kind's MOVE_PICKS) or, for the charge and the drop, by the ids that
 * enter that way (0C:9D9E-9DAA, 0C:9E20-9E44).
 *
 * Logs (runs/p4/base_*.flog): the jump in rounds 2-5 (Poison) and 2 and 4
 * (El Gado), the knife and the slide in rounds 2 and 4, the charge in
 * rounds 1 and 5, the drop in rounds 3-5. obj_timer is the step of a move.
 *
 * In: X = slot (all of them).
 */
#include "enemy_ai.h"

/* the jump's reach (0F:FB3E): Poison $16 x 8, El Gado $37 x 8 */
#define POISON_JUMP_REACH_X     0x16
#define GADO_JUMP_REACH_X       0x37
#define JUMP_REACH_Y            0x08

/* the knife: by A of bit 6 of the thrower's obj_flags (0 left, 2 right),
 * the 16-bit x offset from the thrower: -$12 or +$12 */
#define KNIFE_DX                0xA64E
#define KNIFE_ANIM              0xBB
#define KNIFE_ATTACK            0x87    /* attack out, reach box 7 */
#define KNIFE_DAMAGE            0x06
#define SFX_KNIFE               0x1B

/* the slide: by obj_ai_count ($0E down to 1), obj_vx_frac / obj_vx; the
 * last entry ($0280) is the start, read as $A707/$A716 */
#define SLIDE_VX_FRAC           0xA6F9
#define SLIDE_VX                0xA708
#define SLIDE_STEPS             0x0E

/* the charge: its steps 1-3 (JMP ($0000)); the direction by the
 * direction index of the player (horizontal or diagonal only); by tier
 * index the stalking time, the obj_ai_timer under which it may stop
 * early, and the mask of those frames (kept in obj_ground_y) */
#define CHARGE_STEPS_LO         0xA878
#define CHARGE_STEPS_HI         0xA87B
#define CHARGE_DIRS             0xA87E
#define CHARGE_TIME             0xA88E
#define CHARGE_STOP_BELOW       0xA891
#define CHARGE_STOP_MASK        0xA894
#define SFX_CHARGE              0x2A    /* every $10 frames of the run */

/* the attack while dropping in, by obj_kind (Poison $94, El Gado $8A;
 * the table has no entry for Andore, who never drops) */
#define DROP_ATTACK             0xA8DF

/*
 * enemy_jump_start, $A41D (name proposed): phase 6. Out of the jump's
 * reach the enemy walks at the player (JMP enemy_walk_to_player). In
 * reach: facing him, obj_ground_y = obj_y, and the jump: Poison $0100
 * across and $0400 up, damage index $36 + tier index, 8 frames crouched;
 * El Gado $0300 across and $0380 up, damage $39 + tier index, $10
 * frames. Phase 9.
 * Caller: 0C:9D38 (phase 6).
 */
void func_A41D_b6(void)
{
    REGS_IN;

    mff_cycles(0xA41D, 6);
    a = RAM(OBJ_KIND + x);
    if (a != KIND_POISON) {
        mff_cycles(0xA420, 1);
        mff_cycles(0xA428, 4);
        a = GADO_JUMP_REACH_X;
        y = JUMP_REACH_Y;
    } else {
        mff_cycles(0xA422, 6);
        a = POISON_JUMP_REACH_X;
        y = JUMP_REACH_Y;
        mff_cycles(0xA426, 1);
    }
    mff_cycles(0xA42C, 6);
    CALL_FIXED(0xA42C, func_FB3E);              /* player within A x Y */
    mff_cycles(0xA42F, 2);
    if (!c) {
        mff_cycles(0xA431, 3);
        GOTO_BANK(0xA085);                      /* enemy_walk_to_player */
    }
    mff_cycles(0xA42F, 1);
    mff_cycles(0xA434, 6);
    CALL_FIXED(0xA434, func_F343);              /* obj_face_player */
    mff_cycles(0xA437, 15);
    RAM(OBJ_GROUND_Y + x) = RAM(OBJ_Y + x);
    a = RAM(OBJ_KIND + x);
    if (a == KIND_POISON) {
        mff_cycles(0xA442, 36);
        RAM(OBJ_VX_FRAC + x) = 0x00;
        RAM(OBJ_VX + x) = 0x01;
        RAM(OBJ_VY_FRAC + x) = 0x00;
        RAM(OBJ_VY + x) = 0x04;
        y = RAM(OBJ_TIER + x);
        a = prg_read_ix(TIER_INDEX, y);
        mff_cycles(0xA45C, 18);
        c = 0;
        ADC_A(0x36);
        RAM(OBJ_ATTACK_DAMAGE + x) = a;
        a = 0x08;
        RAM(OBJ_AI_TIMER + x) = a;
        mff_cycles(0xA467, 1);                  /* BNE, always taken */
    } else {
        mff_cycles(0xA440, 1);
        mff_cycles(0xA469, 36);
        RAM(OBJ_VX_FRAC + x) = 0x00;
        RAM(OBJ_VX + x) = 0x03;
        RAM(OBJ_VY_FRAC + x) = 0x80;
        RAM(OBJ_VY + x) = 0x03;
        y = RAM(OBJ_TIER + x);
        a = prg_read_ix(TIER_INDEX, y);
        mff_cycles(0xA483, 16);
        c = 0;
        ADC_A(0x39);
        RAM(OBJ_ATTACK_DAMAGE + x) = a;
        a = 0x10;
        RAM(OBJ_AI_TIMER + x) = a;
    }
    mff_cycles(0xA48E, 13);
    a = AI_JUMP;
    RAM(OBJ_STATE + x) = a;
    RTS_RETURN();
}

/*
 * enemy_jump, $A494 (name proposed): phase 9. Step 0: crouched for
 * obj_ai_timer frames (Poison animation $E8, El Gado the getting-up one,
 * 0C:9CAD), then the jump's animation (Poison $84, El Gado $AC) and step 1.
 * Step 1: y with gravity (0F:F1FD); down on obj_ground_y it lands: over a
 * pit (enemy_pit_under) obj_pit_flag = obj_kind and no more; otherwise
 * stopped, attack in, obj_ai_timer 8, phase 12 and the getting-up
 * animation. In the air it moves along obj_dir (Poison against it: the back
 * flip) and on into enemy_jump_air.
 * Caller: 0C:9D38 (phase 9).
 */
void func_A494_b6(void)
{
    REGS_IN;

    mff_cycles(0xA494, 6);
    a = RAM(OBJ_TIMER + x);
    if (a == 0) {
        mff_cycles(0xA499, 6);
        a = RAM(OBJ_AI_TIMER + x);
        if (a != 0) {
            mff_cycles(0xA49E, 13);
            RAM(OBJ_AI_TIMER + x) = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
            a = RAM(OBJ_KIND + x);
            if (a == KIND_POISON) {
                mff_cycles(0xA4A6, 7);
                a = 0xE8;
                y = 0x18;
                GOTO_FIXED(0xF449, func_F449);  /* obj_set_anim, chr_banks+4 */
            }
            mff_cycles(0xA4A4, 1);
            mff_cycles(0xA4AD, 3);
            GOTO_BANK(0x9CAD);                  /* obj_anim_getting_up */
        }
        mff_cycles(0xA49C, 1);
        mff_cycles(0xA4B0, 13);
        RAM(OBJ_TIMER + x) = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        a = RAM(OBJ_KIND + x);
        if (a == KIND_POISON) {
            mff_cycles(0xA4B8, 7);
            a = 0x84;
            y = 0x1A;
            GOTO_FIXED(0xF449, func_F449);      /* obj_set_anim, chr_banks+4 */
        }
        mff_cycles(0xA4B6, 1);
        mff_cycles(0xA4BF, 7);
        a = 0xAC;
        y = 0x22;
        GOTO_FIXED(0xF44F, func_F44F);          /* obj_set_anim, chr_banks+5 */
    }
    mff_cycles(0xA497, 1);
    mff_cycles(0xA4C6, 6);
    CALL_FIXED(0xA4C6, func_F1FD);              /* obj_fall */
    mff_cycles(0xA4C9, 10);
    a = RAM(OBJ_Y + x);
    c = a >= RAM(OBJ_GROUND_Y + x);
    if (c) {
        mff_cycles(0xA4D1, 15);
        a = RAM(OBJ_GROUND_Y + x);
        RAM(OBJ_Y + x) = a;
        CALL_BANK(0xA4D7, 0xA9F6);              /* enemy_pit_under */
        mff_cycles(0xA4DA, 2);
        if (c) {
            mff_cycles(0xA4DC, 15);
            a = RAM(OBJ_KIND + x);
            RAM(OBJ_PIT_FLAG + x) = a;
            RTS_RETURN();
        }
        mff_cycles(0xA4DA, 1);
        mff_cycles(0xA4E3, 34);
        a = 0x00;
        RAM(OBJ_VY_FRAC + x) = a;
        RAM(OBJ_VY + x) = a;
        RAM(OBJ_ATTACK + x) = a;
        RAM(OBJ_STATE + x) = AI_RECOVER;
        a = 0x08;
        RAM(OBJ_AI_TIMER + x) = a;
        GOTO_BANK(0x9CAD);                      /* obj_anim_getting_up */
    }
    mff_cycles(0xA4CF, 1);
    mff_cycles(0xA4FB, 10);
    a = RAM(OBJ_DIR + x);
    y = RAM(OBJ_KIND + x);
    if (y == KIND_POISON) {
        mff_cycles(0xA503, 2);
        a ^= 0xFF;
    } else {
        mff_cycles(0xA501, 1);
    }
    mff_cycles(0xA505, 4);
    a &= DIR_RIGHT;
    if (a != 0) {
        mff_cycles(0xA509, 6);
        CALL_FIXED(0xA509, func_F183);          /* obj_move_right */
        mff_cycles(0xA50C, 3);
        GOTO_BANK(0xA512);                      /* enemy_jump_air */
    }
    mff_cycles(0xA507, 1);
    mff_cycles(0xA50F, 6);
    CALL_FIXED(0xA50F, func_F1A1);              /* obj_move_left */
    FALL_INTO(func_A512_b6);
}

/*
 * enemy_jump_air, $A512 (name proposed): in round 4's second area (round
 * 3, sub_area 1, where x is on the screen) x is kept within $08-$F8. The
 * attack stays out: El Gado $8A; Poison $89, and on frame 7 of its
 * animation it holds that frame (obj_anim_timer 0).
 * Callers: JMP at $A50C; the fall-through from $A50F (enemy_jump).
 */
void func_A512_b6(void)
{
    REGS_IN;

    mff_cycles(0xA512, 7);
    a = RAM(ROUND);
    c = a >= 0x03;
    if (a == 0x03) {
        mff_cycles(0xA518, 7);
        a = RAM(SUB_AREA);
        c = a >= 0x01;
        if (a == 0x01) {
            mff_cycles(0xA51E, 8);
            a = 0x08;
            if (a >= RAM(OBJ_X + x)) {
                mff_cycles(0xA525, 5);
                RAM(OBJ_X + x) = a;
            } else {
                mff_cycles(0xA523, 1);
            }
            mff_cycles(0xA528, 8);
            a = 0xF8;
            c = a >= RAM(OBJ_X + x);
            if (!c) {
                mff_cycles(0xA52F, 5);
                RAM(OBJ_X + x) = a;
            } else {
                mff_cycles(0xA52D, 1);
            }
        } else {
            mff_cycles(0xA51C, 1);
        }
    } else {
        mff_cycles(0xA516, 1);
    }
    mff_cycles(0xA532, 6);
    a = RAM(OBJ_KIND + x);
    if (a != KIND_POISON) {
        mff_cycles(0xA535, 1);
        mff_cycles(0xA542, 2);
        a = 0x8A;
    } else {
        mff_cycles(0xA537, 8);
        a = RAM(OBJ_ANIM_FRAME + x);
        c = a >= 0x07;
        if (a == 0x07) {
            mff_cycles(0xA53C, 1);
            mff_cycles(0xA548, 9);
            a = 0x00;
            RAM(OBJ_ANIM_TIMER + x) = a;
            mff_cycles(0xA544, 1);              /* BEQ, always taken */
        } else {
            mff_cycles(0xA53E, 4);
            a = 0x89;
            mff_cycles(0xA540, 1);
        }
    }
    mff_cycles(0xA544, 11);
    RAM(OBJ_ATTACK + x) = a;
    RTS_RETURN();
}

/*
 * enemy_knife_throw, $A54F (name proposed): phase 10, El Gado. Step 0:
 * backs away (sideways against the way it faces) while lining up with the
 * player's ground line (down or up, speed index 2, enemy_walk) until it is
 * within 8 px of it (0F:FBD9). Step 1: the throw (animation $A7, sprite
 * bank $20); on frame 4 the knife flies (enemy_knife_spawn), after frame 5
 * standing for $30 frames (step 2), then enemy_reset_wait.
 * Caller: 0C:9D38 (phase 10).
 */
void func_A54F_b6(void)
{
    REGS_IN;
    uint8_t timer;

    mff_cycles(0xA54F, 8);
    a = RAM(OBJ_TIMER + x);
    if (a == 0x01) {
        mff_cycles(0xA554, 1);
        mff_cycles(0xA593, 8);
        a = 0xA7;
        c = a >= RAM(OBJ_ANIM + x);
        if (a != RAM(OBJ_ANIM + x)) {
            mff_cycles(0xA59A, 8);
            y = 0x20;
            CALL_FIXED(0xA59C, func_F44F);      /* obj_set_anim, chr_banks+5 */
            mff_cycles(0xA59F, 7);
            RAM(OBJ_ANIM_TIMER + x) = (uint8_t)(RAM(OBJ_ANIM_TIMER + x) + 1);
        } else {
            mff_cycles(0xA598, 1);
        }
        mff_cycles(0xA5A2, 8);
        a = RAM(OBJ_ANIM_FRAME + x);
        c = a >= 0x04;
        if (a == 0x04) {
            mff_cycles(0xA5A9, 6);
            a = RAM(OBJ_ANIM_TIMER + x);
            if (a == 0) {
                mff_cycles(0xA5AE, 10);
                RAM(OBJ_ANIM_TIMER + x) = (uint8_t)(RAM(OBJ_ANIM_TIMER + x) + 1);
                GOTO_BANK(0xA5DA);              /* enemy_knife_spawn */
            }
            mff_cycles(0xA5AC, 1);
        } else {
            mff_cycles(0xA5A7, 1);
        }
        mff_cycles(0xA5B4, 8);
        a = RAM(OBJ_ANIM_FRAME + x);
        c = a >= 0x05;
        if (a == 0x05) {
            mff_cycles(0xA5BB, 8);
            a = RAM(OBJ_ANIM_TIMER + x);
            c = a >= 0x04;
            if (a == 0x04) {
                mff_cycles(0xA5C2, 17);
                RAM(OBJ_TIMER + x) = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
                a = 0x30;
                RAM(OBJ_AI_TIMER + x) = a;
                GOTO_BANK(0xAAAE);              /* enemy_anim_stand */
            }
        }
        mff_cycles(0xA592, 1);
        mff_cycles(0xA592, 6);
        RTS_RETURN();
    }
    mff_cycles(0xA556, 4);
    c = a >= 0x02;
    if (a == 0x02) {
        mff_cycles(0xA558, 1);
        mff_cycles(0xA5CD, 9);
        timer = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
        RAM(OBJ_AI_TIMER + x) = timer;
        if (timer != 0) {
            mff_cycles(0xA592, 1);
            mff_cycles(0xA592, 6);
            RTS_RETURN();
        }
        mff_cycles(0xA5D2, 10);
        a = 0x00;
        RAM(OBJ_TIMER + x) = a;
        GOTO_BANK(0xA8E4);                      /* enemy_reset_wait */
    }
    mff_cycles(0xA55A, 9);
    a = RAM(PLAYER_GROUND_Y);
    c = a >= RAM(OBJ_Y + x);
    if (c) {
        mff_cycles(0xA55F, 1);
        mff_cycles(0xA565, 2);
        a = DIR_DOWN;
    } else {
        mff_cycles(0xA561, 4);
        a = DIR_UP;
        mff_cycles(0xA563, 1);
    }
    mff_cycles(0xA567, 13);
    RAM(OBJ_DIR + x) = a;
    a = RAM(OBJ_FLAGS + x) & OBJ_FACING_RIGHT;
    if (a != 0) {
        mff_cycles(0xA56F, 1);
        mff_cycles(0xA578, 6);
        a = RAM(OBJ_DIR + x) | DIR_LEFT;
    } else {
        mff_cycles(0xA571, 8);
        a = RAM(OBJ_DIR + x) | DIR_RIGHT;
        mff_cycles(0xA576, 1);
    }
    mff_cycles(0xA57D, 13);
    RAM(OBJ_DIR + x) = a;
    y = 0x02;
    CALL_BANK(0xA582, 0xAC34);                  /* enemy_set_speed */
    mff_cycles(0xA585, 6);
    CALL_BANK(0xA585, 0xA953);                  /* enemy_walk */
    mff_cycles(0xA588, 6);
    CALL_FIXED(0xA588, func_FBD9);              /* y distance to the player's ground line */
    mff_cycles(0xA58B, 4);
    c = a >= 0x08;
    if (!c) {
        mff_cycles(0xA58F, 7);
        RAM(OBJ_TIMER + x) = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    } else {
        mff_cycles(0xA58D, 1);
    }
    mff_cycles(0xA592, 6);
    RTS_RETURN();
}

/*
 * $A592: the RTS of enemy_knife_throw, a routine of its own for the BCS
 * of enemy_knife_spawn (no free slot).
 * Caller: the branch at $A5DD.
 */
void func_A592_b6(void)
{
    REGS_IN;

    mff_cycles(0xA592, 6);
    RTS_RETURN();
}

/*
 * enemy_knife_spawn, $A5DA (name proposed): a free slot (0F:F71A) becomes
 * the knife: obj_dir the way the thrower faces, $12 px ahead and 3 lower
 * on its page, no life, $0330 a frame across, animation $BB (0F:F2A0),
 * sound $1B, attack $87 with damage index 6, id and saved id $2E (0C:9D71
 * flies it), kind 6. No free slot: nothing (BCS to the RTS at $A592).
 * Caller: JMP at $A5B1 (enemy_knife_throw).
 */
void func_A5DA_b6(void)
{
    REGS_IN;

    mff_cycles(0xA5DA, 6);
    CALL_FIXED(0xA5DA, func_F71A);              /* find_free_slot: Y */
    mff_cycles(0xA5DD, 2);
    if (c) {
        mff_cycles(0xA5DD, 1);
        BRANCH_TAIL(0xA592);
    }
    mff_cycles(0xA5DF, 11);
    RAM(TMP0) = y;
    a = RAM(OBJ_FLAGS + x) & OBJ_FACING_RIGHT;
    if (a != 0) {
        mff_cycles(0xA5E6, 1);
        mff_cycles(0xA5EC, 2);
        a = DIR_RIGHT;
    } else {
        mff_cycles(0xA5E8, 4);
        a = DIR_LEFT;
        mff_cycles(0xA5EA, 1);
    }
    mff_cycles(0xA5EE, 19);
    RAM(OBJ_DIR + y) = a;
    y = a & DIR_LEFT;                           /* 0 or 2: the offset's index */
    a = RAM(OBJ_X + x);
    w = a + prg_read_ix(KNIFE_DX, y);
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xA5FB, 11);
    mff_push(a);
    a = (uint8_t)(RAM(OBJ_X_HI + x) + prg_read_ix(KNIFE_DX + 1, y) + c);
    mff_cycles(0xA602, 73);
    y = RAM(TMP0);
    RAM(OBJ_X_HI + y) = a;
    a = mff_pull();
    RAM(OBJ_X + y) = a;
    a = RAM(OBJ_Y + x);
    c = 0;
    ADC_A(0x03);
    RAM(OBJ_Y + y) = a;
    RAM(OBJ_Y_HI + y) = RAM(OBJ_Y_HI + x);
    RAM(OBJ_LIFE + y) = 0x00;
    RAM(OBJ_LIFE_MAX + y) = 0x00;
    RAM(OBJ_VX_FRAC + y) = 0x30;
    RAM(OBJ_VX + y) = 0x03;
    a = KNIFE_ANIM;
    CALL_FIXED(0xA62E, func_F2A0);              /* obj_spawn: slot Y, animation A */
    mff_cycles(0xA631, 8);
    a = SFX_KNIFE;
    CALL_FIXED(0xA633, func_F34C);              /* sound_queue_push_sfx */
    mff_cycles(0xA636, 39);
    RAM(OBJ_ATTACK + y) = KNIFE_ATTACK;
    RAM(OBJ_ATTACK_DAMAGE + y) = KNIFE_DAMAGE;
    RAM(OBJ_ID + y) = OBJ_KNIFE;
    RAM(OBJ_SAVED_ID + y) = OBJ_KNIFE;
    a = KIND_KNIFE;
    RAM(OBJ_KIND + y) = a;
    RTS_RETURN();
}

/*
 * enemy_slide, $A652 (name proposed): phase 11, El Gado. Step 0: out of
 * reach ($37 x 6) it walks at the player (JMP enemy_walk_to_player); in
 * reach it crouches (0C:9CAD) for $18 frames facing him. Step 1: the slide
 * starts: animation $A6 (sprite bank $21), $0280 across, damage $30 +
 * tier index. Step 2: attack $86 out; every 3 frames the speed drops a
 * step (SLIDE_VX by obj_ai_count) unless a pit takes it (enemy_pit_under);
 * after $0E steps, or in the pit, the attack goes in, obj_ai_timer 8,
 * phase 12 and the getting-up animation.
 * Caller: 0C:9D38 (phase 11).
 */
void func_A652_b6(void)
{
    REGS_IN;
    uint8_t timer;

    mff_cycles(0xA652, 8);
    a = RAM(OBJ_TIMER + x);
    c = a >= 0x01;
    if (a == 0x01) {
        mff_cycles(0xA657, 1);
        mff_cycles(0xA677, 9);
        timer = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
        RAM(OBJ_AI_TIMER + x) = timer;
        if (timer == 0) {
            mff_cycles(0xA67C, 36);
            RAM(OBJ_AI_TIMER + x) = 0x03;
            RAM(OBJ_AI_COUNT + x) = SLIDE_STEPS;
            RAM(OBJ_VY_FRAC + x) = 0x00;
            RAM(OBJ_VY + x) = 0x00;
            a = 0xA6;
            y = 0x21;
            CALL_FIXED(0xA692, func_F44F);      /* obj_set_anim, chr_banks+5 */
            mff_cycles(0xA695, 4);
            a = prg_read(SLIDE_VX_FRAC + SLIDE_STEPS);
            mff_cycles(0xA698, 9);
            RAM(OBJ_VX_FRAC + x) = a;
            a = prg_read(SLIDE_VX + SLIDE_STEPS);
            mff_cycles(0xA69E, 13);
            RAM(OBJ_VX + x) = a;
            y = RAM(OBJ_TIER + x);
            a = prg_read_ix(TIER_INDEX, y);
            mff_cycles(0xA6A7, 16);
            c = 0;
            ADC_A(0x30);
            RAM(OBJ_ATTACK_DAMAGE + x) = a;
            RAM(OBJ_TIMER + x) = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
        } else {
            mff_cycles(0xA67A, 1);
        }
        mff_cycles(0xA6B0, 6);
        RTS_RETURN();
    }
    mff_cycles(0xA659, 4);
    c = a >= 0x02;
    if (a == 0x02) {
        mff_cycles(0xA65B, 1);
        mff_cycles(0xA6B1, 16);
        a = 0x86;
        RAM(OBJ_ATTACK + x) = a;
        timer = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
        RAM(OBJ_AI_TIMER + x) = timer;
        if (timer != 0) {
            mff_cycles(0xA6B9, 1);
        } else {
            uint8_t steps;
            int stop;

            mff_cycles(0xA6BB, 16);
            RAM(OBJ_AI_TIMER + x) = 0x03;
            steps = (uint8_t)(RAM(OBJ_AI_COUNT + x) - 1);
            RAM(OBJ_AI_COUNT + x) = steps;
            if (steps == 0) {
                mff_cycles(0xA6C5, 14);
                a = 0x08;
                RAM(OBJ_AI_TIMER + x) = a;
                RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
                stop = 1;
            } else {
                mff_cycles(0xA6C3, 1);
                mff_cycles(0xA6D8, 6);
                CALL_BANK(0xA6D8, 0xA9F6);      /* enemy_pit_under */
                mff_cycles(0xA6DB, 2);
                stop = c;
                if (stop)
                    mff_cycles(0xA6CD, 1);
            }
            if (stop) {
                mff_cycles(0xA6CD, 15);
                a = 0x00;
                RAM(OBJ_ATTACK + x) = a;
                RAM(OBJ_TIMER + x) = a;
                GOTO_BANK(0x9CAD);              /* obj_anim_getting_up */
            }
            mff_cycles(0xA6DD, 8);
            y = RAM(OBJ_AI_COUNT + x);
            a = prg_read_ix(SLIDE_VX_FRAC, y);
            mff_cycles(0xA6E3, 9);
            RAM(OBJ_VX_FRAC + x) = a;
            a = prg_read_ix(SLIDE_VX, y);
            mff_cycles(0xA6E9, 5);
            RAM(OBJ_VX + x) = a;
        }
        mff_cycles(0xA6EC, 8);
        a = RAM(OBJ_DIR + x) & DIR_RIGHT;
        if (a != 0) {
            mff_cycles(0xA6F3, 3);
            GOTO_FIXED(0xF183, func_F183);      /* obj_move_right */
        }
        mff_cycles(0xA6F1, 1);
        mff_cycles(0xA6F6, 3);
        GOTO_FIXED(0xF1A1, func_F1A1);          /* obj_move_left */
    }
    mff_cycles(0xA65D, 10);
    a = GADO_JUMP_REACH_X;
    y = 0x06;
    CALL_FIXED(0xA661, func_FB3E);              /* player within A x Y */
    mff_cycles(0xA664, 2);
    if (!c) {
        mff_cycles(0xA666, 3);
        GOTO_BANK(0xA085);                      /* enemy_walk_to_player */
    }
    mff_cycles(0xA664, 1);
    mff_cycles(0xA669, 6);
    CALL_BANK(0xA669, 0x9CAD);                  /* obj_anim_getting_up */
    mff_cycles(0xA66C, 17);
    a = 0x18;
    RAM(OBJ_AI_TIMER + x) = a;
    RAM(OBJ_TIMER + x) = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    GOTO_FIXED(0xF343, func_F343);              /* obj_face_player */
}

/*
 * enemy_charge, $A73A (name proposed): phase 14, Andore. Step 0: obj_dir
 * away from the player (the speed of index $0C), and by tier index
 * obj_ai_timer CHARGE_TIME, obj_ground_y CHARGE_STOP_MASK, obj_ai_count
 * CHARGE_STOP_BELOW. Steps 1-3 go through CHARGE_STEPS (JMP ($0000)):
 * enemy_charge_stalk, enemy_charge_aim, enemy_charge_run. The ids that
 * enter charging (0C:9D9E-9DAA) start at step 3.
 * Caller: 0C:9D38 (phase 14).
 */
void func_A73A_b6(void)
{
    REGS_IN;

    mff_cycles(0xA73A, 6);
    a = RAM(OBJ_TIMER + x);
    if (a != 0) {
        mff_cycles(0xA73D, 1);
        mff_cycles(0xA76F, 10);
        y = (uint8_t)(RAM(OBJ_TIMER + x) - 1);
        a = prg_read_ix(CHARGE_STEPS_LO, y);
        mff_cycles(0xA776, 7);
        RAM(TMP0) = a;
        a = prg_read_ix(CHARGE_STEPS_HI, y);
        mff_cycles(0xA77B, 8);
        RAM(TMP1) = a;
        JMP_INDIRECT(0x0000);
    }
    mff_cycles(0xA73F, 6);
    CALL_FIXED(0xA73F, func_F374);              /* obj_dist_x: C = player to the right */
    mff_cycles(0xA742, 2);
    if (c) {
        mff_cycles(0xA742, 1);
        mff_cycles(0xA748, 2);
        a = DIR_LEFT;
    } else {
        mff_cycles(0xA744, 4);
        a = DIR_RIGHT;
        mff_cycles(0xA746, 1);
    }
    mff_cycles(0xA74A, 13);
    y = 0x0C;
    RAM(OBJ_DIR + x) = a;
    CALL_BANK(0xA74F, 0xAC34);                  /* enemy_set_speed */
    mff_cycles(0xA752, 8);
    y = RAM(OBJ_TIER + x);
    a = prg_read_ix(TIER_INDEX, y);
    mff_cycles(0xA758, 6);
    y = a;
    a = prg_read_ix(CHARGE_TIME, y);
    mff_cycles(0xA75C, 9);
    RAM(OBJ_AI_TIMER + x) = a;
    a = prg_read_ix(CHARGE_STOP_MASK, y);
    mff_cycles(0xA762, 9);
    RAM(OBJ_GROUND_Y + x) = a;
    a = prg_read_ix(CHARGE_STOP_BELOW, y);
    mff_cycles(0xA768, 18);
    RAM(OBJ_AI_COUNT + x) = a;
    RAM(OBJ_TIMER + x) = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    RTS_RETURN();
}

/*
 * enemy_charge_stalk, $A780 (name proposed): charge step 1. Walks (its
 * obj_dir, enemy_walk) facing the player for obj_ai_timer frames; once
 * obj_ai_timer is below obj_ai_count, on the frames where it has all the
 * bits of the mask (obj_ground_y) set, it stops early 3 times in 4. Then
 * step 2.
 * Caller: JMP ($0000) at $A77D (enemy_charge, step 1).
 */
void func_A780_b6(void)
{
    REGS_IN;
    uint8_t timer;

    mff_cycles(0xA780, 9);
    timer = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = timer;
    if (timer == 0) {
        mff_cycles(0xA783, 1);
        goto next_step;
    }
    mff_cycles(0xA785, 6);
    CALL_BANK(0xA785, 0xA953);                  /* enemy_walk */
    mff_cycles(0xA788, 6);
    CALL_BANK(0xA788, 0xA9EB);                  /* enemy_face_player */
    mff_cycles(0xA78B, 10);
    a = RAM(OBJ_AI_TIMER + x);
    c = a >= RAM(OBJ_AI_COUNT + x);
    if (c) {
        mff_cycles(0xA791, 1);
        goto done;
    }
    mff_cycles(0xA793, 14);
    a = (uint8_t)((RAM(OBJ_AI_TIMER + x) & RAM(OBJ_GROUND_Y + x)) ^ RAM(OBJ_GROUND_Y + x));
    if (a != 0) {
        mff_cycles(0xA79C, 1);
        goto done;
    }
    mff_cycles(0xA79E, 7);
    a = RAM(RNG) & 0x03;
    if (a == 0) {
        mff_cycles(0xA7A2, 1);
        goto done;
    }
    mff_cycles(0xA7A4, 7);
    a = 0x00;
    RAM(OBJ_AI_TIMER + x) = a;
next_step:
    mff_cycles(0xA7A9, 7);
    RAM(OBJ_TIMER + x) = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
done:
    mff_cycles(0xA7AC, 6);
    RTS_RETURN();
}

/*
 * enemy_charge_aim, $A7AD (name proposed): charge step 2. Stands 4 frames;
 * then it charges, except when its obj_flags and the player's differ (they
 * face different ways) and rng & $0F is above $0C, 3 times in 16, which
 * gives enemy_reset. It aims (CHARGE_DIRS by the player's direction index):
 * $024C across, or $021F across and $00E1 up or down on a diagonal; the
 * run's animation ($D2, sprite bank $24), damage $33 + tier index,
 * obj_ai_count 1 (the run's sound comes at once), step 3.
 * Caller: JMP ($0000) at $A77D (enemy_charge, step 2).
 */
void func_A7AD_b6(void)
{
    REGS_IN;
    uint8_t timer;

    mff_cycles(0xA7AD, 6);
    a = RAM(OBJ_AI_TIMER + x);
    if (a == 0) {
        mff_cycles(0xA7B2, 7);
        a = 0x04;
        RAM(OBJ_AI_TIMER + x) = a;
    } else {
        mff_cycles(0xA7B0, 1);
    }
    mff_cycles(0xA7B7, 6);
    CALL_BANK(0xA7B7, 0xAAAE);                  /* enemy_anim_stand */
    mff_cycles(0xA7BA, 9);
    timer = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1);
    RAM(OBJ_AI_TIMER + x) = timer;
    if (timer != 0) {
        mff_cycles(0xA7BD, 2);
        mff_cycles(0xA828, 6);
        RTS_RETURN();
    }
    mff_cycles(0xA7BF, 18);
    a = RAM(RNG) & 0x0F;
    RAM(TMP0) = a;
    a = RAM(OBJ_FLAGS) ^ RAM(OBJ_FLAGS + x);    /* the player's and its own */
    if (a == 0) {
        mff_cycles(0xA7CB, 1);
        mff_cycles(0xA7D1, 2);
        a = 0x0F;
    } else {
        mff_cycles(0xA7CD, 4);
        a = 0x0C;
        mff_cycles(0xA7CF, 1);
    }
    mff_cycles(0xA7D3, 5);
    c = a >= RAM(TMP0);
    if (!c) {
        mff_cycles(0xA7D5, 2);
        mff_cycles(0xA829, 3);
        GOTO_BANK(0xA8E8);                      /* enemy_reset */
    }
    mff_cycles(0xA7D7, 6);
    CALL_FIXED(0xA7D7, func_FB5B);              /* direction index to the player */
    mff_cycles(0xA7DA, 6);
    y = a;
    a = prg_read_ix(CHARGE_DIRS, y);
    mff_cycles(0xA7DE, 9);
    RAM(OBJ_DIR + x) = a;
    a &= DIR_UP | DIR_DOWN;
    if (a == 0) {
        mff_cycles(0xA7E5, 28);
        RAM(OBJ_VX_FRAC + x) = 0x4C;
        RAM(OBJ_VX + x) = 0x02;
        a = 0x00;
        RAM(OBJ_VY_FRAC + x) = a;
        RAM(OBJ_VY + x) = a;
        mff_cycles(0xA7F7, 2);                  /* BEQ into the next page */
    } else {
        mff_cycles(0xA7E3, 1);
        mff_cycles(0xA7F9, 28);
        RAM(OBJ_VX_FRAC + x) = 0x1F;
        RAM(OBJ_VX + x) = 0x02;
        RAM(OBJ_VY_FRAC + x) = 0xE1;
        a = 0x00;
        RAM(OBJ_VY + x) = a;
    }
    mff_cycles(0xA80D, 10);
    a = 0xD2;
    y = 0x24;
    CALL_FIXED(0xA811, func_F443);              /* obj_set_anim, chr_banks+3 */
    mff_cycles(0xA814, 8);
    y = RAM(OBJ_TIER + x);
    a = prg_read_ix(TIER_INDEX, y);
    mff_cycles(0xA81A, 23);
    c = 0;
    ADC_A(0x33);
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = 0x01;
    RAM(OBJ_AI_COUNT + x) = a;
    RAM(OBJ_TIMER + x) = (uint8_t)(RAM(OBJ_TIMER + x) + 1);
    mff_cycles(0xA828, 6);
    RTS_RETURN();
}

/*
 * enemy_charge_run, $A82C (name proposed): charge step 3. Within $30 px of
 * the player the animation is $9A (sprite bank $3E); attack $88 out, the
 * walk (enemy_walk, which keeps its way while charging), sound $2A every
 * $10 frames. It stops (attack in, obj_timer 0, enemy_reset) when a wall or
 * pit blocked the walk (WALK_BLOCKED), or once it has run past the player
 * by $30 px.
 * Caller: JMP ($0000) at $A77D (enemy_charge, step 3).
 */
void func_A82C_b6(void)
{
    REGS_IN;
    uint8_t count;

    mff_cycles(0xA82C, 6);
    CALL_FIXED(0xA82C, func_F374);              /* obj_dist_x */
    mff_cycles(0xA82F, 4);
    c = a >= 0x30;
    if (!c) {
        mff_cycles(0xA833, 10);
        a = 0x9A;
        y = 0x3E;
        CALL_FIXED(0xA837, func_F443);          /* obj_set_anim, chr_banks+3 */
    } else {
        mff_cycles(0xA831, 1);
    }
    mff_cycles(0xA83A, 13);
    a = 0x88;
    RAM(OBJ_ATTACK + x) = a;
    CALL_BANK(0xA83F, 0xA953);                  /* enemy_walk */
    mff_cycles(0xA842, 5);
    a = RAM(WALK_BLOCKED);
    if (a != 0) {
        mff_cycles(0xA844, 1);
        goto stop;
    }
    mff_cycles(0xA846, 9);
    count = (uint8_t)(RAM(OBJ_AI_COUNT + x) - 1);
    RAM(OBJ_AI_COUNT + x) = count;
    if (count == 0) {
        mff_cycles(0xA84B, 8);
        a = SFX_CHARGE;
        CALL_FIXED(0xA84D, func_F34C);          /* sound_queue_push_sfx */
        mff_cycles(0xA850, 7);
        a = 0x10;
        RAM(OBJ_AI_COUNT + x) = a;
    } else {
        mff_cycles(0xA849, 1);
    }
    mff_cycles(0xA855, 6);
    CALL_FIXED(0xA855, func_F374);              /* obj_dist_x: C = player to the right */
    mff_cycles(0xA858, 8);
    a = RAM(OBJ_FLAGS + x) & OBJ_FACING_RIGHT;
    if (a != 0) {
        mff_cycles(0xA85D, 1);
        mff_cycles(0xA863, 2);
        if (c) {                                /* still ahead */
            mff_cycles(0xA863, 1);
            goto done;
        }
    } else {
        mff_cycles(0xA85F, 2);
        if (!c) {                               /* still ahead */
            mff_cycles(0xA85F, 1);
            goto done;
        }
        mff_cycles(0xA861, 2);
        mff_cycles(0xA861, 1);                  /* BCS, always taken */
    }
    mff_cycles(0xA865, 6);
    CALL_FIXED(0xA865, func_F374);              /* obj_dist_x */
    mff_cycles(0xA868, 4);
    c = a >= 0x30;
    if (!c) {
        mff_cycles(0xA86A, 1);
        goto done;
    }
stop:
    mff_cycles(0xA86C, 15);
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    RAM(OBJ_TIMER + x) = a;
    GOTO_BANK(0xA8E8);                          /* enemy_reset */
done:
    mff_cycles(0xA877, 6);
    RTS_RETURN();
}

/*
 * enemy_drop_in, $A89C (name proposed): phase 15, the ids that drop from
 * above (0C:9E20-9E44 set obj_vy $FF00 and obj_ground_y). The kind's
 * attack is out (DROP_ATTACK); it falls (0F:F1FD) to obj_ground_y, then
 * lands: stopped, attack in, obj_timer3 0, obj_ai_timer 8, obj_timer 0,
 * phase 12 and the getting-up animation. Poison holds its first frame on
 * the way down (obj_anim_timer 0).
 * Caller: 0C:9D38 (phase 15).
 */
void func_A89C_b6(void)
{
    REGS_IN;

    mff_cycles(0xA89C, 8);
    y = RAM(OBJ_KIND + x);
    a = prg_read_ix(DROP_ATTACK, y);
    mff_cycles(0xA8A2, 11);
    RAM(OBJ_ATTACK + x) = a;
    CALL_FIXED(0xA8A5, func_F1FD);              /* obj_fall */
    mff_cycles(0xA8A8, 10);
    a = RAM(OBJ_Y + x);
    c = a >= RAM(OBJ_GROUND_Y + x);
    if (c) {
        mff_cycles(0xA8B0, 55);
        RAM(OBJ_Y + x) = RAM(OBJ_GROUND_Y + x);
        a = 0x00;
        RAM(OBJ_VY_FRAC + x) = a;
        RAM(OBJ_VY + x) = a;
        RAM(OBJ_ATTACK + x) = a;
        RAM(OBJ_TIMER3 + x) = a;
        RAM(OBJ_AI_TIMER + x) = 0x08;
        RAM(OBJ_TIMER + x) = 0x00;
        a = AI_RECOVER;
        RAM(OBJ_STATE + x) = a;
        GOTO_BANK(0x9CAD);                      /* obj_anim_getting_up */
    }
    mff_cycles(0xA8AE, 1);
    mff_cycles(0xA8D6, 6);
    a = RAM(OBJ_KIND + x);
    if (a == KIND_POISON) {
        mff_cycles(0xA8DB, 5);
        RAM(OBJ_ANIM_TIMER + x) = a;
    } else {
        mff_cycles(0xA8D9, 1);
    }
    mff_cycles(0xA8DE, 6);
    RTS_RETURN();
}
