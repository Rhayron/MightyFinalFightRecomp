/*
 * obj_weapons.c - the weapons of bank $05 (docs/objects.md, "Ids"):
 *   $69  a weapon on the floor (stage list type $0A-$0C, the same entry
 *        showing each character his own: Cody's knife, animation $40;
 *        Guy's, $5E; Haggar's pipe, $5D; 08:81C1 picks it), taken with B
 *   $6A  Guy's weapon put back on the floor when he takes another one; it
 *        carries his throws left in obj_timer
 *   $66  the weapon knocked out of the player's hand by a hit (0F:FAFB);
 *        $67 the same in slot 1 (00:93E6). It flies off, lands and may lie
 *        there as a $69 again, or blinks and goes
 *   $6C  Cody's thrown knife (00:81E6, animation $4D) and the projectile
 *        of 00:8D59 (animation $74, never executed); $6D Guy's (00:8270,
 *        animation $5F). They fly until they leave the screen
 * weapon_held ($A9) is $FF while the player carries one and weapon_ammo
 * ($AD) counts Guy's throws (bank 00 throws and drops the weapon).
 */
#include "obj_props.h"

/* an RTS at `pc` (its 6 cycles). Returns. */
#define RTS_AT(pc) do { mff_cycles(pc, 6); RTS_RETURN(); } while (0)

#define SFX_WEAPON          0x2E    /* taking a weapon (also the item credit's) */
#define GUY_AMMO            0x07    /* Guy's throws with a new weapon */
#define PICKUP_TIMER        0x0C    /* action_timer of the pick-up crouch */

/* ---- ids $69/$6A: a weapon on the floor ---- */

/* game_actions in which nothing is taken: jumping and the pick-up itself
 * ($A825), then hit, falling, grabbing, the throws, Haggar's power, the
 * HeadSmasher and the knee ($A831); Cody's throw ($0B) for Cody only */
static const uint8_t k_pickup_busy_1[] = { 0x01, 0x02 };
static const uint8_t k_pickup_busy_2[] = { 0x06, 0x1F, 0x04, 0x13, 0x15, 0x10, 0x12, 0x21 };
#define ACT_CODY_THROW      0x0B

/* the floor animation of Guy's weapon put back ($A8A5 by player_id & 1:
 * $40, $5E) */
#define DROP_ANIM           0xA8A5

/*
 * weapon_pickup, $A7A5 (name proposed): behaviour of ids $69 and $6A.
 * When the player is free (k_pickup_busy_*), finds himself in the
 * weapon's box (0F:F55A), stands $0A-$0F pixels above it (0F:F365) and
 * presses B alone: sound $2E, the pick-up crouch (player_set_anim
 * $E210[player_id] and its CHR bank, game_action 2, action_timer $0C).
 * With no weapon held: weapon_held $FF and the slot freed ($A82A); Guy
 * also gets his throws, 7 for a new weapon or the $6A's obj_timer. Guy
 * already holding one first puts his back on the floor as a $6A ($A851)
 * and takes the throws of the new one; Cody and Haggar already holding one
 * leave it there.
 * In: X = slot. Out: the RTS of the test that failed, or as $A82A.
 * Callers: $A71B, $A71E (ids $69, $6A).
 */
void func_A7A5_b2(void)
{
    REGS_IN;
    int i;

    /* busy? */
    mff_cycles(0xA7A5, 3);
    a = RAM(GAME_ACTION);
    i = cmp_list(0xA7A7, 0xA825, a, k_pickup_busy_1, 2);
    if (i < 2) {
        CMP_LIST_FLAGS(a, k_pickup_busy_1, 2, i);
        RTS_AT(0xA825);
    }
    i = cmp_list(0xA7AF, 0xA831, a, k_pickup_busy_2, 8);
    CMP_LIST_FLAGS(a, k_pickup_busy_2, 8, i);
    if (i < 8)
        RTS_AT(0xA831);
    mff_cycles(0xA7CF, 5);
    a = RAM(PLAYER_ID);
    SET_NZ(a);
    if (z) {
        mff_cycles(0xA7D3, 7);
        a = RAM(GAME_ACTION);
        CMP_FLAGS(a, ACT_CODY_THROW);
        if (z) {
            mff_cycles(0xA7D7, 2);              /* BEQ across a page */
            RTS_AT(0xA831);
        }
    } else {
        mff_cycles(0xA7D1, 1);
    }

    /* close enough, and B */
    mff_cycles(0xA7D9, 6);
    CALL_FIXED(0xA7D9, func_F55A);              /* C set: not in the box */
    mff_cycles(0xA7DC, 2);
    if (c) {
        mff_cycles(0xA7DC, 2);
        RTS_AT(0xA831);
    }
    mff_cycles(0xA7DE, 6);
    CALL_FIXED(0xA7DE, func_F365);              /* A = |dy|, C = the player not above */
    mff_cycles(0xA7E1, 2);
    if (c) {
        mff_cycles(0xA7E1, 2);
        RTS_AT(0xA850);
    }
    mff_cycles(0xA7E3, 4);
    CMP_FLAGS(a, 0x10);
    if (c) {
        mff_cycles(0xA7E5, 2);
        RTS_AT(0xA850);
    }
    mff_cycles(0xA7E7, 4);
    CMP_FLAGS(a, 0x0A);
    if (!c) {
        mff_cycles(0xA7E9, 2);
        RTS_AT(0xA850);
    }
    mff_cycles(0xA7EB, 7);
    a = RAM(JOY_PRESSED);
    CMP_FLAGS(a, 0x40);                         /* B alone */
    if (!z) {
        mff_cycles(0xA7EF, 2);
        RTS_AT(0xA850);
    }

    /* the pick-up */
    mff_cycles(0xA7F1, 8);
    a = SFX_WEAPON;
    SET_NZ(a);
    CALL_FIXED(0xA7F3, func_F34C);
    mff_cycles(0xA7F6, 7);
    y = RAM(PLAYER_ID);
    a = prg_read_ix(PICKUP_ANIM, y);
    SET_NZ(a);
    mff_cycles(0xA7FB, 6);
    CALL_FIXED(0xA7FB, func_F294);              /* player_set_anim */
    mff_cycles(0xA7FE, 4);
    a = prg_read_ix(PICKUP_CHR, y);
    SET_NZ(a);
    mff_cycles(0xA801, 6);
    CALL_FIXED(0xA801, func_F440);              /* chr_banks+2 = A */
    mff_cycles(0xA804, 15);
    RAM(GAME_ACTION) = GA_PICKUP;
    a = PICKUP_TIMER;
    RAM(ACTION_TIMER) = a;
    a = RAM(WEAPON_HELD);
    if (a == 0) {
        mff_cycles(0xA810, 7);
        a = RAM(PLAYER_ID);
        c = a >= PLAYER_GUY;
        if (a != PLAYER_GUY) {
            mff_cycles(0xA814, 1);
            SYNC_OUT;
            func_A82A_b2();                     /* branches into it */
            return;
        }
        mff_cycles(0xA816, 8);
        a = RAM(OBJ_ID + x);
        c = a >= ID_WEAPON_AMMO;
        if (a != ID_WEAPON_AMMO) {
            mff_cycles(0xA81B, 1);
            mff_cycles(0xA826, 5);
            a = GUY_AMMO;
            RAM(WEAPON_AMMO) = a;
            SYNC_OUT;
            func_A82A_b2();                     /* falls into it */
            return;
        }
        mff_cycles(0xA81D, 7);
        a = RAM(OBJ_TIMER + x);
        SET_NZ(a);
        RAM(WEAPON_AMMO) = a;
        JMP_PROP(0xA822, 0xA82A);
    }

    /* already holding one */
    mff_cycles(0xA80E, 1);
    mff_cycles(0xA832, 7);
    a = RAM(PLAYER_ID);
    CMP_FLAGS(a, PLAYER_GUY);
    if (!z) {
        mff_cycles(0xA836, 1);
        RTS_AT(0xA850);
    }
    mff_cycles(0xA838, 6);
    CALL_PROP(0xA838, 0xA851);                  /* his back on the floor */
    mff_cycles(0xA83B, 8);
    a = RAM(OBJ_ID + x);
    c = a >= ID_WEAPON_AMMO;
    if (a != ID_WEAPON_AMMO) {
        mff_cycles(0xA842, 7);
        a = GUY_AMMO;
        SET_NZ(a);
        RAM(WEAPON_AMMO) = a;
        mff_cycles(0xA846, 1);                  /* BNE $A84D, always taken */
    } else {
        mff_cycles(0xA840, 1);
        mff_cycles(0xA848, 7);
        a = RAM(OBJ_TIMER + x);
        SET_NZ(a);
        RAM(WEAPON_AMMO) = a;
    }
    mff_cycles(0xA84D, 6);
    CALL_FIXED(0xA84D, func_F2BE);              /* object_clear */
    RTS_AT(0xA850);
}

/*
 * weapon_take, $A82A (name proposed): weapon_held = $FF and the slot is
 * freed (object_clear).
 * In: X = slot. Out: the RTS.
 * Callers: weapon_pickup (JMP at $A822, and its branch and fall-through).
 */
void func_A82A_b2(void)
{
    REGS_IN;

    mff_cycles(0xA82A, 11);
    a = 0xFF;
    SET_NZ(a);
    RAM(WEAPON_HELD) = a;
    CALL_FIXED(0xA82E, func_F2BE);
    RTS_AT(0xA831);
}

/*
 * $A850: an RTS (weapon_pickup's, and weapon_drop's when no slot is free).
 * Caller: the BCS at $A854.
 */
void func_A850_b2(void)
{
    mff_cycles(0xA850, 6);
    mff_rts();
}

/*
 * weapon_drop, $A851 (name proposed): Guy's weapon goes back on the floor:
 * a free slot (0F:F71A; none: return) at the weapon's place and direction
 * (x high from scroll_x_hi, page from the player's), animation $5E, id $6A
 * live, obj_spawn_index $80, obj_attack $2F, obj_timer = weapon_ammo.
 * In: X = the slot being picked up. Out: the RTS; Y = the new slot.
 * Caller: weapon_pickup ($A838).
 */
void func_A851_b2(void)
{
    REGS_IN;

    mff_cycles(0xA851, 6);
    CALL_FIXED(0xA851, func_F71A);              /* Y = a free slot, C set if none */
    mff_cycles(0xA854, 2);
    if (c) {
        mff_cycles(0xA854, 1);
        SYNC_OUT;
        func_A850_b2();                         /* BCS to the RTS before */
        return;
    }
    mff_cycles(0xA856, 58);
    RAM(TMP15) = y;
    RAM(OBJ_DIR + y) = RAM(OBJ_DIR + x);
    RAM(OBJ_X_HI + y) = RAM(SCROLL_X_HI);
    RAM(OBJ_Y_HI + y) = RAM(OBJ_Y_HI);
    RAM(OBJ_X + y) = RAM(OBJ_X + x);
    RAM(OBJ_Y + y) = RAM(OBJ_Y + x);
    y = RAM(PLAYER_ID) & 0x01;
    a = prg_read_ix(DROP_ANIM, y);
    mff_cycles(0xA87D, 9);
    y = RAM(TMP15);
    SET_NZ(y);
    CALL_FIXED(0xA87F, func_F2A0);              /* obj_spawn: slot Y, animation A */
    mff_cycles(0xA882, 51);
    RAM(OBJ_ID + y) = ID_WEAPON_AMMO;
    RAM(OBJ_SAVED_ID + y) = ID_WEAPON_AMMO;
    RAM(OBJ_STATE + y) = OBJ_LIVE;
    RAM(OBJ_SAVED_STATE + y) = OBJ_LIVE;
    RAM(OBJ_SPAWN_INDEX + y) = OBJ_LIVE;        /* $80, where others put $FF */
    RAM(OBJ_THROW_FLAG + y) = 0x00;
    RAM(OBJ_ATTACK + y) = 0x2F;
    a = RAM(WEAPON_AMMO);
    SET_NZ(a);
    RAM(OBJ_TIMER + y) = a;
    RTS_AT(0xA8A4);
}

/* ---- ids $6C/$6D: a thrown weapon ---- */

#define THROW_VX_FRAC       0x19    /* written, then overwritten (see below) */
#define ANIM_THROW_74       0x74    /* 00:8D54's projectile */

/*
 * thrown_weapon, $A8A8 (name proposed): behaviour of ids $6C and $6D.
 * Phase 0: obj_vx 4 for Cody, 5 for the others, obj_timer2 $3C. The
 * fraction is meant to be $19 but the next store writes player_id over it
 * ($A8AF-$A8B6), so it is 0, 1 or 2. Then it flies the way obj_dir says
 * (facing that way) and is freed past the screen's edge (x - scroll_x
 * $FA or more going right, under 8 going left). With animation $74 it is
 * also freed when the player is hit (game_action 5 or 6) or at its
 * animation frame 5.
 * In: X = slot. Out: the RTS, or as object_clear.
 * Callers: $A724, $A727 (ids $6C, $6D).
 */
void func_A8A8_b2(void)
{
    REGS_IN;

    mff_cycles(0xA8A8, 8);
    a = RAM(OBJ_STATE + x) & OBJ_PHASE;
    if (a == 0) {
        mff_cycles(0xA8AF, 17);
        RAM(OBJ_VX_FRAC + x) = THROW_VX_FRAC;
        a = RAM(PLAYER_ID);
        RAM(OBJ_VX_FRAC + x) = a;               /* the game's slip */
        if (a == 0) {                           /* Cody: never executed */
            mff_cycles(0xA8BB, 4);
            a = 0x04;
            mff_cycles(0xA8BD, 1);              /* BNE $A8C1, always taken */
        } else {
            mff_cycles(0xA8B9, 1);
            mff_cycles(0xA8BF, 2);
            a = 0x05;
        }
        mff_cycles(0xA8C1, 19);
        RAM(OBJ_VX + x) = a;
        a = 0x3C;
        RAM(OBJ_TIMER2 + x) = a;
        RAM(OBJ_STATE + x)++;
    } else {
        mff_cycles(0xA8AD, 1);
    }

    /* animation $74's end */
    mff_cycles(0xA8CC, 8);
    a = RAM(OBJ_ANIM + x);
    if (a == ANIM_THROW_74) {
        int gone;

        mff_cycles(0xA8D3, 7);
        a = RAM(GAME_ACTION);
        CMP_FLAGS(a, 0x05);
        gone = z;
        if (gone) {
            mff_cycles(0xA8D7, 1);
        } else {
            mff_cycles(0xA8D9, 4);
            CMP_FLAGS(a, 0x06);
            gone = z;
            if (gone) {
                mff_cycles(0xA8DB, 1);
            } else {
                mff_cycles(0xA8DD, 8);
                a = RAM(OBJ_ANIM_FRAME + x);
                CMP_FLAGS(a, 0x05);
                gone = z;
                if (!gone)
                    mff_cycles(0xA8E2, 1);
            }
        }
        if (gone)
            JMP_FIXED(0xA8E4, 0xF2BE, func_F2BE);
    } else {
        mff_cycles(0xA8D1, 1);
    }

    /* fly */
    mff_cycles(0xA8E7, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    if (a != 0) {
        mff_cycles(0xA8EE, 9);
        a = RAM(OBJ_X + x);
        c = 1;
        SBC_A(RAM(SCROLL_X));
        mff_cycles(0xA8F4, 4);
        CMP_FLAGS(a, 0xFA);
        if (c)
            JMP_FIXED(0xA8F8, 0xF2BE, func_F2BE);
        mff_cycles(0xA8F6, 1);
        mff_cycles(0xA8FB, 6);
        CALL_FIXED(0xA8FB, func_F090);          /* face right */
        mff_cycles(0xA8FE, 6);
        CALL_FIXED(0xA8FE, func_F183);          /* x += obj_vx */
        RTS_AT(0xA901);
    }
    mff_cycles(0xA8EC, 2);                      /* BEQ across a page */
    mff_cycles(0xA902, 9);
    a = RAM(OBJ_X + x);
    c = 1;
    SBC_A(RAM(SCROLL_X));
    mff_cycles(0xA908, 4);
    CMP_FLAGS(a, 0x08);
    if (!c)
        JMP_FIXED(0xA90C, 0xF2BE, func_F2BE);
    mff_cycles(0xA90A, 1);
    mff_cycles(0xA90F, 6);
    CALL_FIXED(0xA90F, func_F099);              /* face left */
    mff_cycles(0xA912, 6);
    CALL_FIXED(0xA912, func_F1A1);              /* x -= obj_vx */
    RTS_AT(0xA915);
}

/* ---- ids $66/$67: the weapon knocked out of the player's hand ---- */

#define DROP_VX_FRAC        0xCC    /* obj_vx $00CC */
#define DROP_VY_FRAC        0xE8    /* obj_vy $03E8, up */
#define DROP_VY             0x03
#define DROP_BLINK          0x3C
#define WEAPON_FLOOR_ANIM   0xA9E1  /* by player_id: $40 $5E $5D */
#define OBJ_HIDDEN          0x04    /* obj_flags bit 2 */

static void dropped_weapon_blink(void);

/*
 * dropped_weapon, $A916 (name proposed): behaviour of ids $66 and $67.
 * Phase 0: obj_vx $00CC, obj_vy $03E8 (up), obj_timer2 $3C, and the line
 * it lands on, obj_timer = the player's y + 4. Phase 1 flies against
 * obj_dir (0F:F183 when it is left, 0F:F1A1 when right) and falls
 * ($A957); phase 2 blinks.
 * In: X = slot. Out: as $A957 or the blink.
 * Callers: $A712, $A715 (ids $66, $67).
 */
void func_A916_b2(void)
{
    REGS_IN;

    mff_cycles(0xA916, 8);
    a = RAM(OBJ_STATE + x) & OBJ_PHASE;
    if (a == 0) {
        mff_cycles(0xA91D, 53);
        RAM(OBJ_VX + x) = a;
        RAM(OBJ_VX_FRAC + x) = DROP_VX_FRAC;
        RAM(OBJ_VY_FRAC + x) = DROP_VY_FRAC;
        RAM(OBJ_VY + x) = DROP_VY;
        RAM(OBJ_TIMER2 + x) = DROP_BLINK;
        RAM(OBJ_STATE + x)++;
        a = RAM(OBJ_Y);
        c = 0;
        ADC_A(0x04);
        RAM(OBJ_TIMER + x) = a;
    } else {
        mff_cycles(0xA91B, 1);
    }
    mff_cycles(0xA940, 8);
    a = RAM(OBJ_STATE + x) & 0x02;
    if (a != 0) {
        mff_cycles(0xA945, 1);
        SYNC_OUT;
        dropped_weapon_blink();
        return;
    }
    mff_cycles(0xA947, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    SET_NZ(a);
    if (z) {
        mff_cycles(0xA94E, 6);
        CALL_FIXED(0xA94E, func_F183);          /* x += obj_vx */
        JMP_PROP(0xA951, 0xA957);
    }
    mff_cycles(0xA94C, 1);
    mff_cycles(0xA954, 6);
    CALL_FIXED(0xA954, func_F1A1);              /* x -= obj_vx */
    SYNC_OUT;
    func_A957_b2();                             /* falls into it */
}

/*
 * dropped_weapon_fall, $A957 (name proposed): y -= obj_vy with gravity
 * (0F:F1FD); on reaching its line (obj_timer) it lies down with its floor
 * animation ($A9E1[player_id]). A $66 (not $67) then becomes a weapon on
 * the floor ($69) if the RNG allows (its low digit under $0C, 3 times in 4)
 * and it lies where one can be taken: y $94 or more, in round 2 $A0 or
 * more, in round 4 area 2 x within $18-$E7. Otherwise phase 2: the blink.
 * In: X = slot. Out: the RTS, or as the blink.
 * Callers: dropped_weapon (its JMP at $A951 and its fall-through).
 */
void func_A957_b2(void)
{
    REGS_IN;
    int lies;

    mff_cycles(0xA957, 6);
    CALL_FIXED(0xA957, func_F1FD);              /* fall */
    mff_cycles(0xA95A, 10);
    a = RAM(OBJ_Y + x);
    CMP_FLAGS(a, RAM(OBJ_TIMER + x));
    if (!c) {                                   /* still in the air */
        mff_cycles(0xA960, 1);
        RTS_AT(0xA9BB);
    }
    mff_cycles(0xA962, 7);
    y = RAM(PLAYER_ID);
    a = prg_read_ix(WEAPON_FLOOR_ANIM, y);
    SET_NZ(a);
    mff_cycles(0xA967, 6);
    CALL_FIXED(0xA967, func_F288);              /* obj_set_anim */

    /* a weapon to take again? */
    mff_cycles(0xA96A, 8);
    a = RAM(OBJ_ID + x);
    c = a >= ID_DROPPED_WEAPON_2;
    lies = 0;
    if (a == ID_DROPPED_WEAPON_2) {
        mff_cycles(0xA96F, 1);
    } else {
        mff_cycles(0xA971, 17);
        a = RAM(RNG);
        c = 0;
        ADC_A(RAM(RNG + 1));
        RAM(RNG) = a;
        a &= 0x0F;
        c = a >= 0x0C;
        if (c) {
            mff_cycles(0xA97C, 1);
        } else {
            mff_cycles(0xA97E, 8);
            a = RAM(OBJ_Y + x);
            c = a >= 0x94;
            if (!c) {
                mff_cycles(0xA983, 1);
            } else {
                mff_cycles(0xA985, 7);
                a = RAM(ROUND);
                if (a == 0x01) {
                    /* round 2 */
                    mff_cycles(0xA989, 1);
                    mff_cycles(0xA991, 8);
                    a = RAM(OBJ_Y + x);
                    c = a >= 0xA0;
                    if (!c) {
                        mff_cycles(0xA996, 1);
                    } else {
                        mff_cycles(0xA998, 2 + 1);  /* BCS $A9AB, always taken */
                        lies = 1;
                    }
                } else {
                    mff_cycles(0xA98B, 4);
                    c = a >= 0x03;
                    z = a == 0x03;
                    if (!z) {
                        mff_cycles(0xA98F, 2 + 1);  /* BNE $A9AB, always taken */
                        lies = 1;
                    } else {
                        /* round 4: area 2 between x $18 and $E7 */
                        mff_cycles(0xA98D, 1);
                        mff_cycles(0xA99A, 7);
                        a = RAM(SUB_AREA);
                        c = a >= 0x01;
                        if (a != 0x01) {
                            mff_cycles(0xA99E, 1);
                            lies = 1;
                        } else {
                            mff_cycles(0xA9A0, 8);
                            a = RAM(OBJ_X + x);
                            c = a >= 0x18;
                            if (!c) {
                                mff_cycles(0xA9A5, 1);
                            } else {
                                mff_cycles(0xA9A7, 4);
                                c = a >= 0xE8;
                                if (c)
                                    mff_cycles(0xA9A9, 1);
                                else
                                    lies = 1;
                            }
                        }
                    }
                }
            }
        }
    }
    if (lies) {
        mff_cycles(0xA9AB, 24);
        RAM(OBJ_ID + x) = ID_WEAPON;
        RAM(OBJ_SAVED_ID + x) = ID_WEAPON;
        a = OBJ_LIVE;
        SET_NZ(a);
        RAM(OBJ_STATE + x) = a;
        RAM(OBJ_SAVED_STATE + x) = a;
        RTS_AT(0xA9BB);
    }

    /* phase 2 */
    mff_cycles(0xA9BC, 7);
    RAM(OBJ_STATE + x)++;
    SYNC_OUT;
    dropped_weapon_blink();
}

/*
 * $A9BF: obj_timer2 - 1; at 0 the slot is freed (JMP object_clear); else
 * the weapon is hidden on even counts and shown on odd ones.
 * In: X = slot. Out: the RTS, or as object_clear.
 */
static void dropped_weapon_blink(void)
{
    REGS_IN;

    mff_cycles(0xA9BF, 13);
    RAM(OBJ_TIMER2 + x)--;
    a = RAM(OBJ_TIMER2 + x);
    SET_NZ(a);
    if (z) {
        mff_cycles(0xA9C5, 1);
        JMP_FIXED(0xA9DE, 0xF2BE, func_F2BE);
    }
    mff_cycles(0xA9C7, 4);
    a &= 0x01;
    if (a == 0) {
        mff_cycles(0xA9CB, 13);
        a = RAM(OBJ_FLAGS + x) | OBJ_HIDDEN;
        SET_NZ(a);
        RAM(OBJ_FLAGS + x) = a;
        mff_cycles(0xA9D3, 1);                  /* BNE $A9DD, always taken */
    } else {
        mff_cycles(0xA9C9, 1);
        mff_cycles(0xA9D5, 11);
        a = RAM(OBJ_FLAGS + x) & (uint8_t)~OBJ_HIDDEN;
        SET_NZ(a);
        RAM(OBJ_FLAGS + x) = a;
    }
    RTS_AT(0xA9DD);
}
