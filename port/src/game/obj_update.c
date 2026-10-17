/*
 * obj_update.c - objects_update (0C:8003), the per-frame walk over the
 * object slots, and the collision checks it runs between two slots: thrown
 * bodies against the others, the slot-1/2 objects, the player's attack and
 * the object's own attack against the player. docs/objects.md, "Update".
 *
 * The loop in the original: for slot X = 1..15, if it is live, map the
 * id's behaviour bank at $A000, push $805D and JMP through
 * obj_behavior_lo/hi[id]; the behaviour's RTS lands on $805E, which moves
 * to the next slot and runs that slot's checks before dispatching it. So
 * slot 1 is dispatched without checks, and every other slot gets them
 * right before its behaviour runs.
 *
 * In the port the scaffold made a function of every JMP target ($800B,
 * $819A, $81B8, $81F0 ...), and every JMP between them is a tail call
 * through the dispatcher. The places the code reaches otherwise are
 * functions too (port/tools/find_entries.py): $805E (objects_next_slot),
 * where call_by_address_tail($805E) lands after a behaviour returns and a
 * dead slot goes directly; $80D3 (obj_hit_by_player), also the target of
 * the never-executed JMP after a slot-1/2 contact; the RTS at $821E and
 * the JMP $83A8 at $8241, where the thrown-body tests branch.
 */
#include "obj_defs.h"

/* ids that knock down whatever they touch (thrown bodies); the
 * HeadSmasher's body ($0E) only while its attack is on ($8072-$808B) */
static const uint8_t k_thrown_ids[] = { OBJ_THROWN_CODY, OBJ_THROWN_BACK, OBJ_THROWN_GUY };

/* animations of an object in the air: a hit lands it on obj_ground_y
 * ($80DE-$80F0). The thrown-body check ($834F) tests the first four only. */
static const uint8_t k_airborne_anims[] = { 0x84, 0xAC, 0xE7, 0xBE, 0xCC };

/* the knockback a player attack causes ($8146-$8198), in the order tested:
 * light attacks push the enemy back (id 2), the others knock it down (id 5,
 * func_819A_b6); attacks not listed leave its behaviour alone */
static const uint8_t k_knock_attacks[] = {
    0x01, 0x0D, 0x02, 0x03, 0x08, 0x0E, 0x07, 0x0A, 0x0F, 0x10,
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x1D, 0x22,
};
static const uint8_t k_knock_heavy[] = {
    0, 0, 1, 1, 1, 1, 0, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 0, 1, 1,
};

/* the light attacks again, where a kill picks the dying id ($81D3-$81E5) */
static const uint8_t k_light_attacks[] = { 0x01, 0x07, 0x0D, 0x18 };

/* ids that are dying already ($81B8-$81C1) */
static const uint8_t k_dying_ids[] = { OBJ_KILLED_LIGHT, OBJ_KILLED };

/* game_action values during which thrown bodies hit others ($8289-$8299) */
static const uint8_t k_throw_actions[] = {
    ACTION_CODY_THROW, ACTION_GUY_THROW, ACTION_BACK_THROW, ACTION_HEADSMASHER,
};

/* ids a thrown body passes through: those below $16 (tested first), these
 * five through the skip at $8241 ($82A2-$82B4) ... */
static const uint8_t k_body_pass_ids[] = { 0x6F, 0x63, 0x69, 0x6C, 0x6A };
/* ... and these through the JMP at $8333 ($82B6-$82D4). The ones below $16
 * can never match here. */
static const uint8_t k_body_pass_ids2[] = {
    0x6D, OBJ_SPARK, OBJ_THROWN_CODY, OBJ_THROWN_BACK, OBJ_THROWN_SMASH, OBJ_THROWN_GUY, 0x61, 0x62,
};

void func_805E_b6(void);       /* objects_next_slot */
void func_80D3_b6(void);       /* obj_hit_by_player */

/*
 * $8000 (bank $0C entry): JMP objects_update.
 * Caller: gameplay_update, JSR $8000 at 0E:C974 after mapping banks
 * $0C/$0D (0E:C96A-C971). The other JSR $8000 in banks 0E/0F reach other
 * banks' entries.
 */
void func_8000_b6(void)
{
    mff_jmp_charge(0x8000, 0x8003);
    call_by_address_tail(0x8003, 6);
}

/*
 * objects_update, $8003: sets gravity ($99) to $38 and runs slot 1, from
 * which the loop goes on (func_800B_b6).
 * In: nothing. Out: as the loop leaves it (an RTS once update_slot reaches
 * $10, from objects_next_slot after the last behaviour).
 * Caller: JMP at 0C:8000.
 */
void func_8003_b6(void)
{
    REGS_IN;

    mff_cycles(0x8003, 10);
    a = GRAVITY_NORMAL;
    RAM(GRAVITY) = a;
    x = 1;
    RAM(UPDATE_SLOT) = x;
    SYNC_OUT;
    func_800B_b6();
}

/*
 * objects_update_slot, $800B (name proposed): dispatches slot X.
 * A dead slot goes straight to the next one. A live one gets its
 * behaviour's bank at $A000 (id $60-$6F bank $05, $70-$7F $0B, $80-$8F
 * $01, $90 and up $03, below $60 $0D: the code computes id >> 4 for those
 * and never uses it, see docs/objects.md), mapped through bank_switch when
 * bank_request+1 differs, then a JMP (obj_behavior_lo/hi[id]) with $805D
 * pushed. In: X = slot. Out: the behaviour gets X = slot, Y = id, A = $5D,
 * N/Z from it, C from the CPY $F5 or from bank_switch.
 * Callers: JMP from objects_update's checks at 0C:81B5, 820E and 821B; the
 * fall-through from objects_update (func_8003_b6).
 */
void func_800B_b6(void)
{
    REGS_IN;

    mff_cycles(0x800B, 6);
    a = RAM(OBJ_STATE + x);
    if (!(a & OBJ_LIVE)) {
        mff_cycles(0x800E, 1);
        SYNC_OUT;
        func_805E_b6();                         /* objects_next_slot */
        return;
    }

    /* the bank for $A000, by id */
    mff_cycles(0x8010, 10);
    y = 0x0D;
    a = RAM(OBJ_ID + x);
    if (a < 0x60) {
        mff_cycles(0x8017, 1);
        mff_cycles(0x802F, 12);
        a >>= 4;
        if (a >= 0x0A) {
            /* SEC / SBC #6 / TAY: dead, the id's high digit is 0-5 here */
            mff_cycles(0x8037, 6);
            c = 1;
            SBC_A(0x06);
            y = a;
        } else {
            mff_cycles(0x8035, 1);
        }
    } else {
        mff_cycles(0x8019, 6);
        y = 0x05;
        if (a < 0x70) {
            mff_cycles(0x801D, 1);
        } else {
            mff_cycles(0x801F, 6);
            y = 0x0B;
            if (a < 0x80) {
                mff_cycles(0x8023, 1);
            } else {
                mff_cycles(0x8025, 6);
                y = 0x01;
                if (a < 0x90) {
                    mff_cycles(0x8029, 1);
                } else {
                    mff_cycles(0x802B, 5);      /* LDY #3; BNE taken */
                    y = 0x03;
                }
            }
        }
    }

    mff_cycles(0x803B, 5);
    m = RAM(BANK_REQUEST + 1);
    c = y >= m;
    if (y != m) {
        mff_cycles(0x803F, 14);
        RAM(BANK_REQUEST + 1) = y;
        a = x;
        SET_NZ(a);
        mff_push(a);                            /* the slot, across bank_switch */
        CALL_FIXED(0x8043, func_FF4D);
        mff_cycles(0x8046, 6);
        a = mff_pull();
        x = a;
    } else {
        mff_cycles(0x803D, 1);
    }

    /* JMP (obj_behavior_lo/hi[id]) with $805D pushed: its RTS lands on $805E */
    mff_cycles(0x8048, 8);
    y = RAM(OBJ_ID + x);
    a = prg_read((uint16_t)(OBJ_BEHAVIOR_LO + y));
    mff_cycles(0x804E, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(OBJ_BEHAVIOR_HI + y));
    mff_cycles(0x8053, 18);
    RAM(TMP1) = a;
    mff_push(0x80);
    a = 0x5D;
    SET_NZ(a);
    mff_push(a);
    SYNC_OUT;
    {
        uint16_t jt = nes_read16zp(0x0000);
        nes_cpu_instruction_boundary(jt, 2);
        call_by_address(jt);
        call_by_address_tail(0x805E, 6);
    }
}

/*
 * objects_next_slot, $805E (name proposed): $805E-$81B5 of
 * objects_update, the next slot (an RTS when update_slot reaches $10) and
 * its checks before it is dispatched:
 *  1. obj_hit_cooldown nonzero: count it down, skip to its own attack;
 *  2. a thrown body checks the others (thrown_body_collide);
 *  3. 0F:F9E5 tests it against the objects in slots 2 and 1;
 *  4. the player's attack (0F:F738): on a hit, a spark and, unless it is
 *     dying already, obj_hit_by_player; then
 *  5. its own attack against the player (func_81F0_b6).
 * In: nothing (update_slot). Out: as the path taken.
 * Callers: every behaviour's RTS (to the $805D that func_800B_b6 pushes,
 * through call_by_address_tail); func_800B_b6 for a dead slot (the BPL at
 * 0C:800E).
 */
void func_805E_b6(void)
{
    REGS_IN;
    int i, thrown;

    mff_cycles(0x805E, 12);
    t = (uint8_t)(RAM(UPDATE_SLOT) + 1);
    RAM(UPDATE_SLOT) = t;
    x = RAM(UPDATE_SLOT);
    CMP_FLAGS(x, 0x10);
    if (z) {
        mff_cycles(0x8066, 6);
        RTS_RETURN();
    }
    mff_cycles(0x8064, 1);

    /* 1. hit a moment ago: no being-hit checks while the cooldown runs */
    mff_cycles(0x8067, 6);
    a = RAM(OBJ_HIT_COOLDOWN + x);
    if (a != 0) {
        mff_cycles(0x806C, 7);
        t = (uint8_t)(RAM(OBJ_HIT_COOLDOWN + x) - 1);
        RAM(OBJ_HIT_COOLDOWN + x) = t;
        SET_NZ(t);
        JMP_TAIL(0x806F, 0x81F0);
    }
    mff_cycles(0x806A, 1);

    /* 2. thrown bodies */
    mff_cycles(0x8072, 4);
    a = RAM(OBJ_ID + x);
    i = cmp_list(0x8075, 0x808D, a, k_thrown_ids, 3);
    thrown = i < 3;
    if (thrown) {
        CMP_FLAGS(a, k_thrown_ids[i]);
    } else {
        mff_cycles(0x8081, 4);
        CMP_FLAGS(a, OBJ_THROWN_SMASH);
        if (a != OBJ_THROWN_SMASH) {
            mff_cycles(0x8083, 1);
        } else {
            mff_cycles(0x8085, 7);
            a = RAM(ATTACK_ID);
            CMP_FLAGS(a, ATTACK_HEADSMASHER);
            thrown = z;
            if (thrown)
                mff_cycles(0x8089, 1);
            else
                mff_cycles(0x808B, 3);          /* BNE $8090 */
        }
    }
    if (thrown) {
        mff_cycles(0x808D, 6);
        CALL_BANK(0x808D, 0x8244);              /* thrown_body_collide */
    }

    /* 3. the objects in slots 2 and 1: C clear on contact */
    mff_cycles(0x8090, 6);
    CALL_FIXED(0x8090, func_F9E5);
    mff_cycles(0x8093, 2);
    if (!c) {
        /* never executed: the slot-1/2 object (TMP15) goes, or with
         * animation $74 turns into id 9; this slot takes the hit (JMP
         * $80D3, obj_hit_by_player) */
        mff_cycles(0x8095, 11);
        y = RAM(TMP15);
        a = RAM(OBJ_ANIM + y);
        c = a >= 0x74;
        if (a == 0x74) {
            mff_cycles(0x809C, 1);
            mff_cycles(0x80AF, 23);
            RAM(OBJ_ANIM_TIMER + y) = 0;
            RAM(OBJ_ANIM_FRAME + y) = 6;
            a = OBJ_SPARK_9;
            SET_NZ(a);
            RAM(OBJ_ID + y) = a;
            mff_cycles(0x80BE, 1);              /* BNE $80AC */
        } else {
            mff_cycles(0x809E, 22);
            a = 0;
            SET_NZ(a);
            RAM(OBJ_STATE + y) = a;
            RAM(OBJ_SAVED_STATE + y) = a;
            RAM(OBJ_FLAGS + y) = a;
            RAM(OBJ_ID + y) = a;
        }
        JMP_TAIL(0x80AC, 0x80D3);
    }
    mff_cycles(0x8093, 1);

    /* 4. the player's attack: C clear when it hits this slot */
    mff_cycles(0x80C0, 6);
    CALL_FIXED(0x80C0, func_F738);              /* player_attack_test */
    mff_cycles(0x80C3, 2);
    if (!c) {
        mff_cycles(0x80C5, 6);
        CALL_FIXED(0x80C5, func_F455);          /* spawn_hit_spark */
        mff_cycles(0x80C8, 6);
        a = RAM(OBJ_LIFE + x);
        SET_NZ(a);
        if (!n) {
            mff_cycles(0x80CB, 1);
            SYNC_OUT;
            func_80D3_b6();                     /* obj_hit_by_player */
            return;
        }
        /* dying already: the spark is all */
    } else {
        mff_cycles(0x80C3, 1);
    }
    /* 5. its own attack */
    JMP_TAIL(0x80CD, 0x81F0);
}

/*
 * obj_hit_by_player, $80D3 (name proposed): $80D3-$81B5 of
 * objects_update, the player's attack hit slot X, which was
 * alive. obj_take_hit deals the damage (AxL may block); the attack the
 * object had out is cancelled; obj_land_y is set to the line it will land
 * on: obj_ground_y if it was in the air (or if obj_y + $0A came out 0),
 * obj_y + $0A for kinds 6 (the knife) and $0C, unchanged for id $91, the
 * player's obj_y for attack $1D (obj_y if that is 0), else its own obj_y. Then, with life
 * gone negative, it dies (JMP $80D0 -> func_81B8_b6); otherwise the combo
 * window opens and the attack's knockback is applied: pushed back (id 2) or
 * knocked down (id 5), both from phase 0, or nothing. AxL takes no
 * knockback when he blocked (obj_hit_timer cleared). On to the slot's
 * behaviour (JMP $800B).
 * In: X = slot. Out: as the path taken.
 * Callers: objects_next_slot (the BPL at 0C:80CB; the JMP at 0C:80AC,
 * never executed).
 */
void func_80D3_b6(void)
{
    REGS_IN;
    int i, airborne;

    mff_cycles(0x80D3, 6);
    CALL_BANK(0x80D3, 0x84F0);                  /* obj_take_hit */

    mff_cycles(0x80D6, 11);
    a = 0;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_ANIM + x);
    i = cmp_list(0x80DE, 0x8108, a, k_airborne_anims, 5);
    CMP_LIST_FLAGS(a, k_airborne_anims, 5, i);
    airborne = i < 5;
    if (!airborne) {
        int lower;

        mff_cycles(0x80F2, 8);
        a = RAM(OBJ_KIND + x);
        lower = a == KIND_KNIFE;
        if (lower) {
            mff_cycles(0x80F7, 1);
        } else {
            mff_cycles(0x80F9, 4);
            lower = a == KIND_9D;
            if (!lower)
                mff_cycles(0x80FB, 2);
        }
        if (lower) {
            mff_cycles(0x80FD, 15);
            a = RAM(OBJ_Y + x);
            c = 0;
            ADC_A(0x0A);
            RAM(OBJ_LAND_Y + x) = a;
            if (a != 0)
                mff_cycles(0x8106, 1);
            else
                airborne = 1;                   /* the BNE falls into $8108 */
        } else {
            mff_cycles(0x8116, 8);
            a = RAM(OBJ_ID + x);
            c = a >= 0x91;
            if (a == 0x91) {
                mff_cycles(0x811B, 1);
            } else {
                int own_y = 1;

                mff_cycles(0x811D, 7);
                a = RAM(ATTACK_ID);
                c = a >= ATTACK_1D;
                if (a == ATTACK_1D) {
                    mff_cycles(0x8123, 11);
                    a = RAM(OBJ_Y);             /* slot 0, the player */
                    RAM(OBJ_LAND_Y + x) = a;
                    own_y = a == 0;
                    if (!own_y)
                        mff_cycles(0x8129, 1);
                } else {
                    mff_cycles(0x8121, 1);
                }
                if (own_y) {
                    mff_cycles(0x812B, 9);
                    a = RAM(OBJ_Y + x);
                    RAM(OBJ_LAND_Y + x) = a;
                }
            }
        }
    }
    if (airborne) {
        mff_cycles(0x8108, 15);
        a = RAM(OBJ_GROUND_Y + x);
        RAM(OBJ_LAND_Y + x) = a;
        a = RAM(OBJ_LIFE + x);
        SET_NZ(a);
        if (n) {
            mff_cycles(0x8111, 2);
            BRANCH_TAIL(0x80D0);
        }
        JMP_TAIL(0x8113, 0x819A);
    }

    mff_cycles(0x8131, 6);
    a = RAM(OBJ_LIFE + x);
    SET_NZ(a);
    if (n) {
        mff_cycles(0x8134, 2);
        BRANCH_TAIL(0x80D0);
    }
    mff_cycles(0x8136, 13);
    a = COMBO_WINDOW;
    RAM(COMBO_TIMER) = a;
    a = RAM(OBJ_KIND + x);
    c = a >= KIND_AXL;
    if (a == KIND_AXL) {
        mff_cycles(0x8141, 6);
        a = RAM(OBJ_HIT_TIMER + x);
        SET_NZ(a);
        if (z) {                                /* he blocked */
            mff_cycles(0x8144, 1);
            JMP_TAIL(0x81B5, 0x800B);
        }
    } else {
        mff_cycles(0x813F, 1);
    }

    /* the knockback (the BEQs all land in page $81) */
    mff_cycles(0x8146, 3);
    a = RAM(ATTACK_ID);
    i = cmp_list(0x8148, 0x819A, a, k_knock_attacks, 20);
    CMP_LIST_FLAGS(a, k_knock_attacks, 20, i);
    if (i == 20) {
        mff_cycles(0x8198, 3);                  /* BNE $81B5 */
        JMP_TAIL(0x81B5, 0x800B);
    }
    if (k_knock_heavy[i]) {
        SYNC_OUT;
        func_819A_b6();
        return;
    }
    mff_cycles(0x81AB, 14);
    RAM(OBJ_ID + x) = OBJ_PUSHED_BACK;
    a = OBJ_LIVE;
    SET_NZ(a);
    RAM(OBJ_STATE + x) = a;
    JMP_TAIL(0x81B5, 0x800B);
}

/*
 * obj_knock_down, $819A (name proposed): slot X becomes id 5 (knocked
 * down) at phase 0, thrown up at obj_vy $038E, and is dispatched.
 * In: X = slot. Out: A = $80, N/Z from it; C and V kept.
 * Callers: JMP at 0C:8113 (a hit in the air); the knockback table of
 * obj_hit_by_player (BEQs at 0C:8152-8196).
 */
void func_819A_b6(void)
{
    REGS_IN;

    mff_cycles(0x819A, 24);                     /* ... LDA #3; STA; BNE taken */
    RAM(OBJ_ID + x) = OBJ_KNOCKED_DOWN;
    RAM(OBJ_VY_FRAC + x) = KNOCK_VY_FRAC;
    a = KNOCK_VY;
    RAM(OBJ_VY + x) = a;
    mff_cycles(0x81B0, 7);
    a = OBJ_LIVE;
    SET_NZ(a);
    RAM(OBJ_STATE + x) = a;
    JMP_TAIL(0x81B5, 0x800B);
}

/*
 * $80D0: JMP $81B8, where the "life went negative" BMIs at 0C:8111 and
 * 0C:8134 go.
 */
void func_80D0_b6(void)
{
    mff_jmp_charge(0x80D0, 0x81B8);
    call_by_address_tail(0x81B8, 6);
}

/*
 * obj_hit_dies, $81B8 (name proposed): the player's hit took slot X's last
 * life. An object already dying (ids 6, 7) is left alone; Belger (id $74)
 * is killed and turns into id $9F (never executed); an object showing
 * animation $7B becomes id $95 with animation $7F and objects_update ends
 * for this frame (its RTS returns to gameplay_update; never executed).
 * Anything else: area_kills + 1, obj_kill, and id 6 (killed by a light
 * attack: 1, 7, $0D, $18) or id 7, then its own attack is still tested
 * (falls into func_81F0_b6).
 * In: X = slot. Out: as func_81F0_b6, or A = $9F / N/Z from it, or the
 * returns of obj_set_anim.
 * Caller: JMP at 0C:80D0.
 */
void func_81B8_b6(void)
{
    REGS_IN;
    int i;

    mff_cycles(0x81B8, 4);
    a = RAM(OBJ_ID + x);
    i = cmp_list(0x81BB, 0x820E, a, k_dying_ids, 2);
    CMP_LIST_FLAGS(a, k_dying_ids, 2, i);
    if (i < 2)
        JMP_TAIL(0x820E, 0x800B);

    mff_cycles(0x81C3, 4);
    c = a >= OBJ_BELGER;
    if (a == OBJ_BELGER) {
        mff_cycles(0x81C5, 2);
        mff_cycles(0x8211, 11);
        t = (uint8_t)(RAM(AREA_KILLS) + 1);
        RAM(AREA_KILLS) = t;
        SET_NZ(t);
        CALL_BANK(0x8213, 0x822F);              /* obj_kill */
        mff_cycles(0x8216, 7);
        a = 0x9F;
        SET_NZ(a);
        RAM(OBJ_ID + x) = a;
        JMP_TAIL(0x821B, 0x800B);
    }

    mff_cycles(0x81C7, 8);
    a = RAM(OBJ_ANIM + x);
    c = a >= 0x7B;
    if (a == 0x7B) {
        mff_cycles(0x81CC, 2);
        mff_cycles(0x821F, 22);
        RAM(OBJ_STATE + x) = OBJ_LIVE;
        RAM(OBJ_ID + x) = 0x95;
        a = 0x7F;
        SET_NZ(a);
        CALL_FIXED(0x822B, func_F288);          /* obj_set_anim */
        mff_cycles(0x822E, 6);
        RTS_RETURN();
    }

    mff_cycles(0x81CE, 11);
    t = (uint8_t)(RAM(AREA_KILLS) + 1);
    RAM(AREA_KILLS) = t;
    SET_NZ(t);
    CALL_BANK(0x81D0, 0x822F);                  /* obj_kill */
    mff_cycles(0x81D3, 3);
    a = RAM(ATTACK_ID);
    i = cmp_list(0x81D5, 0x81E7, a, k_light_attacks, 4);
    CMP_LIST_FLAGS(a, k_light_attacks, 4, i);
    if (i < 4) {
        mff_cycles(0x81E7, 5);                  /* LDA #6; BNE taken */
        a = OBJ_KILLED_LIGHT;
    } else {
        mff_cycles(0x81E5, 5);                  /* BNE taken; LDA #7 */
        a = OBJ_KILLED;
    }
    mff_cycles(0x81ED, 5);
    RAM(OBJ_ID + x) = a;
    SYNC_OUT;
    func_81F0_b6();
}

/*
 * obj_attack_player, $81F0 (name proposed): step 5 of the checks. If the
 * player is not in a grab, throw or special (player_no_hit bit 7) nor
 * invincible (player_invuln), and slot X's attack is out (obj_attack
 * bit 7), enemy_attack_test (0F:F55A) looks for the player in its reach;
 * on a hit obj_hit_player = $FF, player_invuln = $3C and player_take_hit
 * (0F:F8D5) runs. Then the slot is dispatched (JMP $800B).
 * In: X = slot. Out: what the tests and calls leave.
 * Callers: JMP at 0C:806F (hit cooldown) and 0C:80CD; the fall-through
 * from obj_hit_dies (0C:81ED).
 */
void func_81F0_b6(void)
{
    REGS_IN;

    mff_cycles(0x81F0, 5);
    a = RAM(PLAYER_NO_HIT);
    SET_NZ(a);
    if (n) {
        mff_cycles(0x81F2, 2);
        JMP_TAIL(0x820E, 0x800B);
    }
    mff_cycles(0x81F4, 5);
    a = RAM(PLAYER_INVULN);
    SET_NZ(a);
    if (!z) {
        mff_cycles(0x81F6, 2);
        JMP_TAIL(0x820E, 0x800B);
    }
    mff_cycles(0x81F8, 6);
    a = RAM(OBJ_ATTACK + x);
    SET_NZ(a);
    if (!n) {
        mff_cycles(0x81FB, 2);
        JMP_TAIL(0x820E, 0x800B);
    }
    mff_cycles(0x81FD, 6);
    CALL_FIXED(0x81FD, func_F55A);              /* enemy_attack_test: C clear = in reach */
    mff_cycles(0x8200, 2);
    if (c) {
        mff_cycles(0x8200, 1);
        JMP_TAIL(0x820E, 0x800B);
    }
    mff_cycles(0x8202, 18);
    RAM(OBJ_HIT_PLAYER + x) = 0xFF;
    a = PLAYER_HIT_INVULN;
    SET_NZ(a);
    RAM(PLAYER_INVULN) = a;
    CALL_FIXED(0x820B, func_F8D5);              /* player_take_hit */
    JMP_TAIL(0x820E, 0x800B);
}

/*
 * $821E: the RTS between obj_hit_dies' Belger path and its $7B path,
 * where thrown_body_collide ends for a body still in phase 0.
 * Caller: the BEQ at 0C:8249.
 */
void func_821E_b6(void)
{
    mff_cycles(0x821E, 6);
    mff_rts();
}

/*
 * obj_kill, $822F: slot X stays live at phase 0 ($80), its attack is
 * withdrawn, the combo window closed, and its stage-list entry released
 * (obj_spawn_index = $FF) so the list can spawn it again.
 * In: X = slot. Out: A = $FF, N set, Z clear; X, Y, C, V kept.
 * Callers: obj_hit_dies, JSR at 0C:81D0 and 0C:8213.
 */
void func_822F_b6(void)
{
    uint8_t x = g_cpu.X;

    mff_cycles(0x822F, 30);
    RAM(OBJ_STATE + x) = OBJ_LIVE;
    RAM(OBJ_ATTACK + x) = 0;
    RAM(COMBO_TIMER) = 0;
    RAM(OBJ_SPAWN_INDEX + x) = 0xFF;
    g_cpu.A = 0xFF;
    mff_nz(0xFF);
    mff_rts();
}

/*
 * $8241: JMP thrown_body_next ($83A8), after obj_kill's RTS: where the
 * thrown-body tests skip a slot.
 * Callers: the branches of thrown_body_test (0C:825C, 8273, 827B, 8280,
 * 8285, 8299, 82A0 and the k_body_pass_ids tests).
 */
void func_8241_b6(void)
{
    mff_jmp_charge(0x8241, 0x83A8);
    call_by_address_tail(0x83A8, 6);
}

/*
 * thrown_body_collide, $8244: step 2 of the checks, for a thrown body in
 * slot X. Nothing in its behaviour's phase 0 (the BEQ goes to the RTS at
 * $821E) nor while obj_timer3 counts its first frames down; then slots $0D
 * down to 1 are tested (func_8256_b6).
 * In: X = slot. Out: as the loop leaves it.
 * Caller: objects_next_slot, JSR at 0C:808D.
 */
void func_8244_b6(void)
{
    REGS_IN;

    mff_cycles(0x8244, 8);
    a = RAM(OBJ_STATE + x) & OBJ_PHASE;
    SET_NZ(a);
    if (z) {
        mff_cycles(0x8249, 1);
        BRANCH_TAIL(0x821E);
    }
    mff_cycles(0x824B, 6);
    a = RAM(OBJ_TIMER3 + x);
    if (a != 0) {
        mff_cycles(0x8250, 13);
        t = (uint8_t)(RAM(OBJ_TIMER3 + x) - 1);
        RAM(OBJ_TIMER3 + x) = t;
        SET_NZ(t);
        RTS_RETURN();
    }
    mff_cycles(0x824E, 1);
    mff_cycles(0x8254, 2);
    y = OBJ_SLOT_ENEMY_LAST;
    SYNC_OUT;
    func_8256_b6();
}

/*
 * thrown_body_test, $8256 (name proposed): the body in slot X against slot
 * Y, first part. Slots with the body's own id are skipped; so is the
 * slot when the invulnerability that counts is set: the target's
 * (obj_invuln,Y) when Haggar throws with attack 9 or $13, otherwise the
 * body's own (obj_invuln,X). Skips go to the JMP $83A8 at $8241.
 * In: X = body, Y = slot to test. Out: as the rest of the test.
 * Callers: JMP at 0C:83AB (the next slot); the fall-through from
 * thrown_body_collide (0C:8254).
 */
void func_8256_b6(void)
{
    REGS_IN;
    int targets;

    mff_cycles(0x8256, 10);
    a = RAM(OBJ_ID + x);
    CMP_FLAGS(a, RAM(OBJ_ID + y));
    if (z) {
        mff_cycles(0x825C, 1);
        BRANCH_TAIL(0x8241);
    }

    mff_cycles(0x825E, 7);
    a = RAM(PLAYER_ID);
    c = a >= PLAYER_HAGGAR;
    targets = 0;
    if (!c) {
        mff_cycles(0x8262, 1);
    } else {
        mff_cycles(0x8264, 7);
        a = RAM(ATTACK_ID);
        CMP_FLAGS(a, ATTACK_HEADSMASHER);
        targets = z;
        if (targets) {
            mff_cycles(0x8268, 1);
        } else {
            mff_cycles(0x826A, 4);
            CMP_FLAGS(a, 0x13);
            targets = z;
            if (targets)
                mff_cycles(0x826C, 1);
            else
                mff_cycles(0x826E, 3);          /* BNE $8278 */
        }
    }
    if (targets) {
        mff_cycles(0x8270, 6);
        a = RAM(OBJ_INVULN + y);
        SET_NZ(a);
        if (!z) {
            mff_cycles(0x8273, 1);
            BRANCH_TAIL(0x8241);
        }
        JMP_TAIL(0x8275, 0x827D);
    }
    mff_cycles(0x8278, 6);
    a = RAM(OBJ_INVULN + x);
    SET_NZ(a);
    if (!z) {
        mff_cycles(0x827B, 1);
        BRANCH_TAIL(0x8241);
    }
    SYNC_OUT;
    func_827D_b6();
}

/*
 * thrown_body_test_hit, $827D (name proposed): the rest of the test of
 * slot Y. It must be live and not held, the player in a throw
 * (game_action $0B, $13, $15, $12), its id $16 or more and none of the
 * pass-through ids, both objects on screen (obj_flags bit 7), Y not
 * invulnerable, |x distance| < $1A (16 bits, through TMP0/TMP1), and
 * |y distance| < $0C: from attack_ground_y for the HeadSmasher (attack 9),
 * from the player's obj_y otherwise. Then func_8347_b6 knocks it down.
 * In: X = body, Y = slot. Out: as func_8347_b6 / func_83A8_b6, or the
 * skips (through $8241 or the JMP $83A8 at $8333).
 * Callers: JMP at 0C:8275; the fall-through from 0C:827B.
 */
void func_827D_b6(void)
{
    REGS_IN;
    int i;

    mff_cycles(0x827D, 6);
    a = RAM(OBJ_STATE + y);
    SET_NZ(a);
    if (!n) {
        mff_cycles(0x8280, 1);
        BRANCH_TAIL(0x8241);
    }
    mff_cycles(0x8282, 6);
    a = RAM(OBJ_HELD + y);
    SET_NZ(a);
    if (!z) {
        mff_cycles(0x8285, 1);
        BRANCH_TAIL(0x8241);
    }

    mff_cycles(0x8287, 3);
    a = RAM(GAME_ACTION);
    i = cmp_list(0x8289, 0x829B, a, k_throw_actions, 4);
    CMP_LIST_FLAGS(a, k_throw_actions, 4, i);
    if (i == 4) {
        mff_cycles(0x8299, 3);                  /* BNE $8241 */
        BRANCH_TAIL(0x8241);
    }

    mff_cycles(0x829B, 8);
    a = RAM(OBJ_ID + y);
    CMP_FLAGS(a, 0x16);
    if (!c) {
        mff_cycles(0x82A0, 1);
        BRANCH_TAIL(0x8241);
    }
    i = cmp_list(0x82A2, 0x8241, a, k_body_pass_ids, 5);
    if (i < 5) {
        CMP_LIST_FLAGS(a, k_body_pass_ids, 5, i);
        BRANCH_TAIL(0x8241);
    }
    i = cmp_list(0x82B6, 0x8333, a, k_body_pass_ids2, 8);
    CMP_LIST_FLAGS(a, k_body_pass_ids2, 8, i);
    if (i < 8)
        JMP_TAIL(0x8333, 0x83A8);

    /* both on screen; the CMP #$80 / BCC after each BPL never branches */
    mff_cycles(0x82D6, 6);
    a = RAM(OBJ_FLAGS + x);
    SET_NZ(a);
    if (!n) {
        mff_cycles(0x82D9, 2);
        JMP_TAIL(0x8333, 0x83A8);
    }
    mff_cycles(0x82DB, 4);
    CMP_FLAGS(a, 0x80);
    if (!c) {
        mff_cycles(0x82DD, 2);
        JMP_TAIL(0x8333, 0x83A8);
    }
    mff_cycles(0x82DF, 6);
    a = RAM(OBJ_FLAGS + y);
    SET_NZ(a);
    if (!n) {
        mff_cycles(0x82E2, 2);
        JMP_TAIL(0x8333, 0x83A8);
    }
    mff_cycles(0x82E4, 4);
    CMP_FLAGS(a, 0x80);
    if (!c) {
        mff_cycles(0x82E6, 2);
        JMP_TAIL(0x8333, 0x83A8);
    }
    mff_cycles(0x82E8, 6);
    a = RAM(OBJ_INVULN + y);
    SET_NZ(a);
    if (!z) {
        mff_cycles(0x82EB, 2);
        JMP_TAIL(0x8333, 0x83A8);
    }

    /* |obj_x/obj_x_hi of X - those of Y| in TMP0/TMP1 */
    mff_cycles(0x82ED, 26);
    a = RAM(OBJ_X + x);
    c = 1;
    SBC_A(RAM(OBJ_X + y));
    RAM(TMP0) = a;
    a = RAM(OBJ_X_HI + x);
    SBC_A(RAM(OBJ_X_HI + y));
    SET_NZ(a);
    RAM(TMP1) = a;
    if (n) {
        mff_cycles(0x8300, 19);
        c = 0;
        a = RAM(TMP0) ^ 0xFF;
        ADC_A(0x01);
        RAM(TMP0) = a;
        a = RAM(TMP1) ^ 0xFF;
        ADC_A(0x00);
        SET_NZ(a);
    } else {
        mff_cycles(0x82FE, 1);
    }
    mff_cycles(0x830F, 2);
    if (!z) {
        mff_cycles(0x830F, 1);
        JMP_TAIL(0x8333, 0x83A8);
    }
    mff_cycles(0x8311, 7);
    a = RAM(TMP0);
    CMP_FLAGS(a, 0x1A);
    if (c) {
        mff_cycles(0x8315, 1);
        JMP_TAIL(0x8333, 0x83A8);
    }

    /* |y distance| < $0C */
    mff_cycles(0x8317, 7);
    a = RAM(ATTACK_ID);
    CMP_FLAGS(a, ATTACK_HEADSMASHER);
    if (z) {
        mff_cycles(0x831B, 1);
        mff_cycles(0x831F, 11);
        a = RAM(ATTACK_GROUND_Y);
        c = 1;
        SBC_A(RAM(OBJ_Y + y));
        if (!c) {                               /* never executed */
            mff_cycles(0x8327, 6);
            a ^= 0xFF;
            ADC_A(0x01);
            c = 0;
        } else {
            mff_cycles(0x8325, 1);
        }
        mff_cycles(0x832C, 4);
        CMP_FLAGS(a, 0x0C);
        if (c) {
            mff_cycles(0x832E, 1);
            SYNC_OUT;
            func_83A8_b6();
            return;
        }
        JMP_TAIL(0x8330, 0x8347);
    }
    mff_cycles(0x831D, 3);                      /* BNE $8336 */
    mff_cycles(0x8336, 12);
    a = RAM(OBJ_Y);                             /* slot 0, the player */
    c = 1;
    SBC_A(RAM(OBJ_Y + y));
    if (!c) {                                   /* never executed */
        mff_cycles(0x833F, 4);
        a ^= 0xFF;
        ADC_A(0x01);
    } else {
        mff_cycles(0x833D, 1);
    }
    mff_cycles(0x8343, 4);
    CMP_FLAGS(a, 0x0C);
    if (c) {
        mff_cycles(0x8345, 1);
        SYNC_OUT;
        func_83A8_b6();
        return;
    }
    SYNC_OUT;
    func_8347_b6();
}

/*
 * thrown_body_knock, $8347 (name proposed): the thrown body knocks slot Y
 * down: its attack is cancelled, obj_land_y set (obj_y + $0A for kinds 6
 * and $0C, never executed; obj_ground_y if in the air; obj_y otherwise;
 * each one falling through to the next when it comes out 0), and it
 * becomes id 3 at phase 0, thrown up at $038E, moving away from the
 * player. Then the next slot (func_83A8_b6).
 * In: X = body, Y = slot. Out: as func_83A8_b6, with C = (player's
 * obj_x >= obj_x,Y).
 * Callers: JMP at 0C:8330 (HeadSmasher); the fall-through from 0C:8345.
 */
void func_8347_b6(void)
{
    REGS_IN;
    int i, lower, ground, own_y;

    mff_cycles(0x8347, 11);
    a = 0;
    RAM(OBJ_ATTACK + y) = a;
    a = RAM(OBJ_ANIM + y);
    i = cmp_list(0x834F, 0x8375, a, k_airborne_anims, 4);
    ground = i < 4;
    lower = 0;
    own_y = 0;
    if (!ground) {
        mff_cycles(0x835F, 8);
        a = RAM(OBJ_KIND + y);
        lower = a == KIND_KNIFE;
        if (lower) {
            mff_cycles(0x8364, 1);
        } else {
            mff_cycles(0x8366, 4);
            lower = a == KIND_9D;
            own_y = !lower;
            if (own_y)
                mff_cycles(0x8368, 1);
        }
    }
    if (lower) {
        mff_cycles(0x836A, 15);
        a = RAM(OBJ_Y + y);
        c = 0;
        ADC_A(0x0A);
        RAM(OBJ_LAND_Y + y) = a;
        if (a != 0)
            mff_cycles(0x8373, 1);
        else
            ground = 1;
    }
    if (ground) {
        mff_cycles(0x8375, 11);
        a = RAM(OBJ_GROUND_Y + y);
        RAM(OBJ_LAND_Y + y) = a;
        if (a != 0)
            mff_cycles(0x837B, 1);
        else
            own_y = 1;
    }
    if (own_y) {
        mff_cycles(0x837D, 9);
        a = RAM(OBJ_Y + y);
        RAM(OBJ_LAND_Y + y) = a;
    }

    mff_cycles(0x8383, 38);
    RAM(OBJ_STATE + y) = OBJ_LIVE;
    RAM(OBJ_ID + y) = OBJ_KNOCKED_BY_BODY;
    RAM(OBJ_VY_FRAC + y) = KNOCK_VY_FRAC;
    a = KNOCK_VY;
    RAM(OBJ_VY + y) = a;
    a = RAM(OBJ_X);
    c = a >= RAM(OBJ_X + y);
    if (c) {
        mff_cycles(0x839F, 5);                  /* LDA #2; BNE taken */
        a = DIR_LEFT;
    } else {
        mff_cycles(0x839D, 3);                  /* BCC taken; LDA #1 */
        a = DIR_RIGHT;
    }
    mff_cycles(0x83A5, 5);
    RAM(OBJ_DIR + y) = a;
    SYNC_OUT;
    func_83A8_b6();
}

/*
 * thrown_body_next, $83A8 (name proposed): DEY, and the next slot down
 * (JMP $8256) until slot 0, where the test ends (RTS).
 * In: Y = slot just tested. Out: Y - 1, N/Z from it.
 * Callers: JMP at 0C:8241 (the skips) and 0C:8333; the BCS
 * at 0C:832E and 0C:8345; the fall-through from thrown_body_knock.
 */
void func_83A8_b6(void)
{
    REGS_IN;

    mff_cycles(0x83A8, 4);
    y--;
    SET_NZ(y);
    if (!z)
        JMP_TAIL(0x83AB, 0x8256);
    mff_cycles(0x83A9, 7);                      /* BEQ taken; RTS */
    RTS_RETURN();
}

/*
 * obj_behavior_none, $84EF (name proposed): an RTS, the behaviour of ids
 * 0, $0A-$0D, $14, $15, $17-$1E and $4C-$4E: objects that are only drawn.
 * Reached through obj_behavior_lo/hi (objects_update, 0C:805B).
 */
void func_84EF_b6(void)
{
    mff_cycles(0x84EF, 6);
    mff_rts();
}
