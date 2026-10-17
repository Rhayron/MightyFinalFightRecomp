/*
 * enemy_drums.c - the drums. Round 1's second area rolls drums at the
 * player (id $91), sent by an invisible dispenser (id $94); the bonus stage
 * after the bosses of rounds 2 and 4 is a controller in slot $0F (id $70)
 * that rolls twenty drums (id $72) and pays for the ones smashed. A drum
 * whose life runs out becomes id $95 (0C:81C6: any object with animation
 * $7B): two halves that fly off blinking, and, outside the bonus stage, an
 * item left where it stood.
 *
 * Evidence: in base_cody_tour.flog the dispenser lives in slot $0D from
 * frame 6392 in round 1's second area (round 0, sub_area 1) and the drums
 * in slots $0A-$0C from 6593, animation $7B; the screenshot of frame 6680
 * shows them rolling along the street. Both drum ids share the animation
 * ($7B), the attack ($9E) and its damage ($3F). The bonus stage starts in
 * the player's end-of-area step (00:9359 picks a routine per round and
 * area from $9382; 00:94F7, for round index 1 and 3's last area, sets
 * bonus_stage ($B2) and puts id $70 in slot $0F); no route plays it, the
 * bots skip those areas.
 *
 * Bank $03 (ids $91-$95) is the scaffold's bank 1, bank $0B (ids $70-$72)
 * its bank 5.
 */
#include "enemy.h"

/* ---- ids, animations, attacks ---- */
#define OBJ_DRUM            0x91    /* a rolling drum (round 1) */
#define OBJ_BONUS_DRUM      0x72    /* a drum of the bonus stage */
#define OBJ_ITEM_FOOD       0x10    /* the item the first drum always leaves */
#define OBJ_WEAPON          0x69    /* the character's weapon, lying on the ground */
#define DRUM_ANIM           0x7B    /* both drums; 0C:81C6 tests it */
#define DRUM_ATTACK         0x9E    /* out, reach box $1E */
#define DRUM_DAMAGE         0x3F
#define ITEM_ATTACK         0x1F    /* not out (bit 7 clear) */
#define FOOD_ANIM           0x61
#define SFX_DRUM            0x29    /* each drum the dispenser sends */
#define MUSIC_BONUS         0x09

/* ---- the dispenser ($94) ---- */
#define DISPENSER_WAIT      0x3C    /* frames before the first drum, and while three roll */
#define DISPENSER_DRUMS     0x06    /* obj_timer2 reaching it ends the area */
#define AREA_KILLS_DONE     0x08    /* area_kills that ends the area */
#define DRUM_VX_FRAC        0x33    /* obj_vx $0133: 1.2 px a frame */
#define DRUM_VX             0x01
#define DRUM_X_RIGHT_END    0xF8    /* cleared at x >= $F8 going right */
#define DRUM_X_LEFT_END     0x08    /* or below $08 going left */

/* frames until the next drum, by rng & 7 ($BD3E) */
#define DRUM_WAITS          0xBD3E
/* where a drum starts, by round, then rng & 7: y, x and obj_dir */
#define DRUM_Y_ROUND1       0xBCF6  /* $98/$88/$78 */
#define DRUM_Y_ROUND2       0xBCFE
#define DRUM_Y_ROUND3       0xBD06
#define DRUM_X_ROUND1       0xBD0E  /* $FF: from the right edge */
#define DRUM_X_ROUND2       0xBD16  /* 0: from the left */
#define DRUM_X_ROUND3       0xBD1E
#define DRUM_DIR_ROUND1     0xBD26  /* left */
#define DRUM_DIR_ROUND2     0xBD2E  /* right */
#define DRUM_DIR_ROUND3     0xBD36

/* ---- the smashed drum ($95) ---- */
#define DEBRIS_FRAMES       0x10
#define DEBRIS_VX           0x03
#define DEBRIS_VY           0x01
#define ITEMS_ON_SCREEN_MAX 0x02    /* no item while two are lying around */
#define ITEM_DROP_ODDS      0x7F    /* an item when rng >= $7F (about half) */
/* the item a drum leaves, by rng & $0F: id ($BE88) and animation ($BE78);
 * the weapon's animation by player_id ($BE98) */
#define ITEM_IDS            0xBE88
#define ITEM_ANIMS          0xBE78
#define WEAPON_ANIMS        0xBE98

/* the item ids BE9B counts: the four items and the weapon */
static const uint8_t k_item_ids[] = { 0x10, 0x11, 0x12, 0x13, OBJ_WEAPON };

/* ---- the bonus stage ($70, slot $0F) ---- */
/* labels.toml still calls them unk_b2 and unk_b9 (its comments give the
 * evidence; the rename touches other systems' files) */
#define BONUS_STAGE         UNK_B2  /* $B2: $FF while the bonus stage runs */
#define BONUS_DRUM_COUNT    UNK_B9  /* $B9: drums smashed in it */
#define BONUS_SLOT          0x0F
#define BONUS_DRUMS         0x14    /* drums rolled in all */
#define BONUS_FIRST_WAIT    0x78    /* frames before the first drum */
#define BONUS_WAIT          0x40    /* frames between drums */
#define BONUS_LIVE_MAX      0x02    /* no new drum while two roll */
#define BONUS_PRIZE_MIN     0x08    /* drums to smash for a prize */
#define BONUS_PRIZE_SLOT    0x06
#define BONUS_PRIZE_Y       0x8C    /* the prize lands here */
#define BONUS_LIVE_DRUMS    (OBJ_GROUND_Y + BONUS_SLOT)  /* $052F: drums rolling */
#define ACT_BONUS_FAIL      0x29
#define ACT_BONUS_PRIZE     0x1A
#define SCREEN_BONUS_ALL    0xFC    /* all twenty smashed */
#define SCREEN_BONUS_FAIL   0xFB
#define SCREEN_BONUS_PRIZE  0xFE
/* the controller's phases ($B64E/$B652) */
#define BONUS_PHASE_LO      0xB64E
#define BONUS_PHASE_HI      0xB652
/* the prize by drums smashed: animation ($B66C) and id ($B681) */
#define PRIZE_ANIMS         0xB66C
#define PRIZE_IDS           0xB681
/* the player's pose when the prize lands, by player_id (bank $0F) */
#define PLAYER_WIN_ANIM     0xE1E6
#define PLAYER_WIN_CHR      0xE22B
#define PLAYER_WIN_VX_FRAC  0xE1C1
#define PLAYER_WIN_VX       0xE1C4
#define PLAYER_WIN_VY_FRAC  0xE1C7
#define PLAYER_WIN_VY       0xE1CA

/* the bonus drums: obj_kind is how they move, obj_timer2 the line they
 * roll on */
#define BDRUM_ROLLING       0x00
#define BDRUM_DROPPING      0x01    /* falls in from the top, bounces twice */
#define BDRUM_BOUNCING      0x02
#define BDRUM_LINE_NEAR     0x8C
#define BDRUM_LINE_FAR      0xA4
/* tables of the launch: whether a drum is slow (rng >> 1 & 7, $B656), the
 * launch pattern (rng+1 ^ the player's y, & 3: $B666; & 1: $B66A), whether
 * a rolling drum bounces ((rng + spawn_timer) & 7: $B65E) */
#define BDRUM_FAST          0xB656
#define BDRUM_PATTERNS      0xB666
#define BDRUM_PATTERNS_HIGH 0xB66A
#define BDRUM_BOUNCES       0xB65E

void func_BD7C_b1(void);
void func_BDAF_b1(void);
void func_BE72_b1(void);
void func_B543_b5(void);
void func_B625_b5(void);
void func_B512_b5(void);

/*
 * drum_roll, $BB91 (bank $03; name proposed): behaviour of id $91, a drum
 * rolling across round 1's second area. Phase 0 sets obj_vx $0133 once;
 * then each frame it faces and moves the way obj_dir bit 0 says (right,
 * else left), and is cleared past x $F8 going right or below $08 going
 * left. Its attack is out all the time; a hit that empties its life
 * turns it into id $95 (0C:81C6).
 * In: X = slot. Out: the RTS.
 * Caller: the behaviour table (id $91, 03:B303).
 */
void func_BB91_b1(void)
{
    REGS_IN;

    mff_cycles(0xBB91, 8);
    a = RAM(OBJ_STATE + x) & OBJ_PHASE;
    if (a == 0) {
        mff_cycles(0xBB98, 21);
        a = DRUM_VX_FRAC;
        RAM(OBJ_VX_FRAC + x) = a;
        a = DRUM_VX;
        RAM(OBJ_VX + x) = a;
        RAM(OBJ_STATE + x)++;
    } else {
        mff_cycles(0xBB96, 1);
    }
    mff_cycles(0xBBA5, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    if (a != 0) {
        mff_cycles(0xBBAC, 6);
        CALL_FIXED(0xBBAC, func_F090);          /* obj_face_right */
        mff_cycles(0xBBAF, 6);
        CALL_FIXED(0xBBAF, func_F183);          /* obj_move_right */
        mff_cycles(0xBBB2, 8);
        a = RAM(OBJ_X + x);
        c = a >= DRUM_X_RIGHT_END;
        if (!c) {
            mff_cycles(0xBBB9, 6);
            RTS_RETURN();
        }
        mff_cycles(0xBBB7, 1);
    } else {
        mff_cycles(0xBBAA, 1);
        mff_cycles(0xBBBA, 6);
        CALL_FIXED(0xBBBA, func_F099);          /* obj_face_left */
        mff_cycles(0xBBBD, 6);
        CALL_FIXED(0xBBBD, func_F1A1);          /* obj_move_left */
        mff_cycles(0xBBC0, 8);
        a = RAM(OBJ_X + x);
        c = a >= DRUM_X_LEFT_END;
        if (c) {
            mff_cycles(0xBBC5, 1);
            mff_cycles(0xBBCA, 6);
            RTS_RETURN();
        }
    }
    mff_cycles(0xBBC7, 6);
    CALL_FIXED(0xBBC7, func_F2BE);              /* object_clear */
    mff_cycles(0xBBCA, 6);
    RTS_RETURN();
}

/*
 * $BC7A / $BC97 / $BCB4: put the new drum (slot in TMP0) on its line:
 * obj_y, obj_x and obj_dir from the round's tables at TMP1 = rng & 7. The
 * three copies differ only in the tables.
 */
static void drum_place(uint16_t pc, uint16_t ytab, uint16_t xtab, uint16_t dtab)
{
    REGS_IN;

    mff_cycles(pc, 4);
    a = prg_read((uint16_t)(ytab + y));
    mff_cycles((uint16_t)(pc + 0x03), 15);
    y = RAM(TMP0);
    RAM(OBJ_Y + y) = a;
    y = RAM(TMP1);
    a = prg_read((uint16_t)(xtab + y));
    mff_cycles((uint16_t)(pc + 0x0D), 15);
    y = RAM(TMP0);
    RAM(OBJ_X + y) = a;
    y = RAM(TMP1);
    a = prg_read((uint16_t)(dtab + y));
    mff_cycles((uint16_t)(pc + 0x17), 14);
    y = RAM(TMP0);
    RAM(OBJ_DIR + y) = a;
    RTS_RETURN();
}

/*
 * drum_dispenser, $BBCB (bank $03; name proposed): behaviour of id $94,
 * an invisible object of round 1's second area that sends the drums.
 * Phase 0 waits for scroll_x to reach 0 (the camera stopped on a screen),
 * then $3C frames. Each time obj_timer runs out it counts the drums on
 * screen ($BCD1): with three it waits $3C frames more; otherwise it counts
 * one more drum in obj_timer2 and, at the sixth, clears itself and sets
 * area_kills to 8, which ends the area. Else sound $29 and a drum in a
 * free slot ($0D..3; with none the count goes on as if one was sent):
 * id $91, animation $7B, attack $9E, the first one marked to leave an
 * item (obj_land_y $FF), on the round's line and side from rng & 7, and
 * $3C-$5A frames to the next one. Only round 1 has drums in the routes;
 * the tables for rounds 2 and 3 start them from the left, and the test for
 * round index 4 (which would use round 3's) never ran.
 * In: X = slot. Out: the RTS.
 * Caller: the behaviour table (id $94, 03:B30C).
 */
void func_BBCB_b1(void)
{
    REGS_IN;

    mff_cycles(0xBBCB, 8);
    a = RAM(OBJ_STATE + x) & OBJ_PHASE;
    if (a == 0) {
        mff_cycles(0xBBD2, 5);
        a = RAM(SCROLL_X);
        if (a != 0) {
            mff_cycles(0xBBD4, 1);
            mff_cycles(0xBBE6, 6);
            RTS_RETURN();
        }
        mff_cycles(0xBBD6, 14);
        a = DISPENSER_WAIT;
        RAM(OBJ_TIMER + x) = a;
        RAM(OBJ_STATE + x)++;
    } else {
        mff_cycles(0xBBD0, 1);
    }
    mff_cycles(0xBBDE, 6);
    a = RAM(OBJ_TIMER + x);
    if (a != 0) {
        mff_cycles(0xBBE3, 7);
        RAM(OBJ_TIMER + x)--;
        mff_cycles(0xBBE6, 6);
        RTS_RETURN();
    }
    mff_cycles(0xBBE1, 1);
    for (;;) {
        mff_cycles(0xBBEF, 6);
        CALL_IN(0xBBEF, 0xBCD1, BANK_03);      /* Z set: three drums roll */
        mff_cycles(0xBBF2, 2);
        if (z) {
            mff_cycles(0xBBF4, 13);
            a = DISPENSER_WAIT;
            RAM(OBJ_TIMER + x) = a;
            RTS_RETURN();
        }
        mff_cycles(0xBBF2, 1);
        mff_cycles(0xBBFA, 15);
        RAM(OBJ_TIMER2 + x)++;
        a = RAM(OBJ_TIMER2 + x);
        c = a >= DISPENSER_DRUMS;
        if (c) {
            mff_cycles(0xBBE7, 2);
            mff_cycles(0xBBE7, 6);
            CALL_FIXED(0xBBE7, func_F2BE);      /* object_clear */
            mff_cycles(0xBBEA, 11);
            a = AREA_KILLS_DONE;
            RAM(AREA_KILLS) = a;
            RTS_RETURN();
        }
        mff_cycles(0xBC04, 8);
        a = SFX_DRUM;
        CALL_FIXED(0xBC06, func_F34C);          /* sound_queue_push_sfx */
        mff_cycles(0xBC09, 6);
        CALL_FIXED(0xBC09, func_F71A);          /* find_free_slot: Y, carry set if none */
        mff_cycles(0xBC0C, 2);
        if (!c)
            break;
        mff_cycles(0xBBEF, 2);
    }
    mff_cycles(0xBC0E, 97);
    RAM(TMP0) = y;
    a = RAM(OBJ_X_HI + x);
    RAM(OBJ_X_HI + y) = a;
    a = RAM(OBJ_Y_HI + x);
    RAM(OBJ_Y_HI + y) = a;
    a = 0x00;
    RAM(OBJ_LIFE + y) = a;
    RAM(OBJ_LIFE_MAX + y) = a;
    RAM(OBJ_THROW_FLAG + y) = a;
    RAM(OBJ_HIT_COOLDOWN + y) = a;
    RAM(OBJ_INVULN + y) = a;
    RAM(OBJ_HIT_PLAYER + y) = a;
    RAM(OBJ_HIT_TIMER + y) = a;
    RAM(OBJ_HELD + y) = a;
    RAM(OBJ_LAND_Y + y) = a;
    a = DRUM_ATTACK;
    RAM(OBJ_ATTACK + y) = a;
    a = DRUM_DAMAGE;
    RAM(OBJ_ATTACK_DAMAGE + y) = a;
    a = OBJ_DRUM;
    RAM(OBJ_ID + y) = a;
    a = RAM(OBJ_TIMER2 + x);
    c = a >= 0x01;
    if (a == 0x01) {                            /* the first drum leaves an item */
        mff_cycles(0xBC4F, 7);
        a = 0xFF;
        RAM(OBJ_LAND_Y + y) = a;
    } else {
        mff_cycles(0xBC4D, 1);
    }
    mff_cycles(0xBC54, 8);
    a = DRUM_ANIM;
    CALL_FIXED(0xBC56, func_F2A0);              /* obj_spawn: slot Y, animation A */
    mff_cycles(0xBC59, 20);
    a = RAM(RNG);
    ADC_A(RAM(RNG + 1));
    RAM(RNG) = a;
    a &= 0x07;
    y = a;
    RAM(TMP1) = y;
    a = prg_read((uint16_t)(DRUM_WAITS + y));
    mff_cycles(0xBC67, 10);
    RAM(OBJ_TIMER + x) = a;
    a = RAM(ROUND);
    if (a == 0) {
        mff_cycles(0xBC6C, 1);
    } else {
        mff_cycles(0xBC6E, 4);
        if (a == 0x01) {
            mff_cycles(0xBC70, 1);
            SYNC_OUT;
            drum_place(0xBC97, DRUM_Y_ROUND2, DRUM_X_ROUND2, DRUM_DIR_ROUND2);
            return;
        }
        mff_cycles(0xBC72, 4);
        if (a == 0x02) {
            mff_cycles(0xBC74, 1);
            SYNC_OUT;
            drum_place(0xBCB4, DRUM_Y_ROUND3, DRUM_X_ROUND3, DRUM_DIR_ROUND3);
            return;
        }
        mff_cycles(0xBC76, 4);
        if (a == 0x04) {                        /* never executed */
            mff_cycles(0xBC78, 1);
            SYNC_OUT;
            drum_place(0xBCB4, DRUM_Y_ROUND3, DRUM_X_ROUND3, DRUM_DIR_ROUND3);
            return;
        }
    }
    SYNC_OUT;
    drum_place(0xBC7A, DRUM_Y_ROUND1, DRUM_X_ROUND1, DRUM_DIR_ROUND1);
}

/*
 * drums_three_on_screen, $BCD1 (bank $03; name proposed): counts the live
 * slots $0F..3 showing the drum animation ($7B) into TMP0. Out: A = 0 and
 * Z set when there are exactly three, else A = $FF, Z clear.
 * Caller: drum_dispenser (03:BBEF).
 */
void func_BCD1_b1(void)
{
    REGS_IN;

    mff_cycles(0xBCD1, 7);
    a = 0x00;
    RAM(TMP0) = a;
    y = OBJ_SLOT_LAST;
    for (;;) {
        mff_cycles(0xBCD7, 6);
        a = RAM(OBJ_STATE + y);
        if (a & OBJ_LIVE) {
            mff_cycles(0xBCDC, 8);
            a = RAM(OBJ_ANIM + y);
            if (a == DRUM_ANIM) {
                mff_cycles(0xBCE3, 5);
                RAM(TMP0)++;
            } else {
                mff_cycles(0xBCE1, 1);
            }
        } else {
            mff_cycles(0xBCDA, 1);
        }
        mff_cycles(0xBCE5, 6);
        y--;
        if (y == 0x02)
            break;
        mff_cycles(0xBCD7, 1);
    }
    mff_cycles(0xBCEA, 7);
    a = RAM(TMP0);
    if (a != 0x03) {
        mff_cycles(0xBCF0, 8);
        a = 0xFF;
        SET_NZ(a);
        RTS_RETURN();
    }
    mff_cycles(0xBCEE, 1);
    mff_cycles(0xBCF3, 8);
    a = 0x00;
    SET_NZ(a);
    RTS_RETURN();
}

/*
 * $BD8B-$BDAE, the flight of the halves: up by obj_vy (0F:F1DF), across by
 * obj_vx the way obj_dir bit 0 says; past x $F8 going right or below $08
 * going left the slot is cleared (JMP object_clear), else the blink at
 * $BDAF (by JMP going right, by a branch going left).
 */
static void debris_fly(void)
{
    REGS_IN;

    mff_cycles(0xBD8B, 6);
    CALL_FIXED(0xBD8B, func_F1DF);              /* obj_move_up */
    mff_cycles(0xBD8E, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    if (a != 0) {
        mff_cycles(0xBD95, 6);
        CALL_FIXED(0xBD95, func_F183);          /* obj_move_right */
        mff_cycles(0xBD98, 8);
        a = RAM(OBJ_X + x);
        c = a >= DRUM_X_RIGHT_END;
        if (!c) {
            mff_cycles(0xBD9F, 3);
            GO_IN(0xBDAF, BANK_03);
        }
        mff_cycles(0xBD9D, 1);
    } else {
        mff_cycles(0xBD93, 1);
        mff_cycles(0xBDA2, 6);
        CALL_FIXED(0xBDA2, func_F1A1);          /* obj_move_left */
        mff_cycles(0xBDA5, 8);
        a = RAM(OBJ_X + x);
        c = a >= DRUM_X_LEFT_END;
        if (c) {
            mff_cycles(0xBDAA, 1);
            SYNC_OUT;
            func_BDAF_b1();
            return;
        }
    }
    mff_cycles(0xBDAC, 3);
    GO_FIXED(0xF2BE, func_F2BE);                /* object_clear */
}

/*
 * drum_smashed, $BD46 (bank $03; name proposed): behaviour of id $95, a
 * drum (any object with animation $7B, 0C:81C6) whose life ran out.
 * Phase 0: obj_vx 3, obj_vy 1 (fractions 0), no attack, $10 frames. In the
 * bonus stage it only counts: the live drums ($052F) down, the smashed
 * ones (bonus_drum_count, $B9) up, then JMP $BD7C. Elsewhere, unless two
 * items already lie around ($BE9B), it may leave one ($BDCE); then $BD7C
 * turns obj_dir to the player's facing (reversed when they differ, so the
 * halves fly the way the hit went). Every frame after that: debris_fly
 * and the blink ($BDAF).
 * In: X = slot. Out: the RTS, or object_clear's.
 * Caller: the behaviour table (id $95, 03:B30F).
 */
void func_BD46_b1(void)
{
    REGS_IN;

    mff_cycles(0xBD46, 8);
    a = RAM(OBJ_STATE + x) & OBJ_PHASE;
    if (a != 0) {
        mff_cycles(0xBD4B, 1);
        SYNC_OUT;
        debris_fly();
        return;
    }
    mff_cycles(0xBD4D, 48);
    RAM(OBJ_VX_FRAC + x) = a;
    RAM(OBJ_VY_FRAC + x) = a;
    RAM(OBJ_ATTACK + x) = a;
    a = DEBRIS_VX;
    RAM(OBJ_VX + x) = a;
    a = DEBRIS_VY;
    RAM(OBJ_VY + x) = a;
    a = DEBRIS_FRAMES;
    RAM(OBJ_TIMER + x) = a;
    RAM(OBJ_STATE + x)++;
    a = RAM(BONUS_STAGE);
    if (a != 0) {                               /* bonus stage */
        mff_cycles(0xBD6C, 14);
        RAM(BONUS_LIVE_DRUMS)--;
        RAM(BONUS_DRUM_COUNT)++;
        GO_IN(0xBD7C, BANK_03);
    }
    mff_cycles(0xBD6A, 1);
    mff_cycles(0xBD74, 6);
    CALL_IN(0xBD74, 0xBE9B, BANK_03);          /* Z set: fewer than two items */
    mff_cycles(0xBD77, 2);
    if (z) {
        mff_cycles(0xBD79, 6);
        CALL_IN(0xBD79, 0xBDCE, BANK_03);
    } else {
        mff_cycles(0xBD77, 1);
    }
    SYNC_OUT;
    func_BD7C_b1();
}

/*
 * $BD7C (bank $03): the end of drum_smashed's phase 0: obj_dir = the
 * player's facing, reversed (EOR 3) when they differ; then debris_fly.
 * In: X = slot. Out: as debris_fly.
 * Callers: JMP at 03:BD71 (the bonus stage); the fall-through from
 * 03:BD7B.
 */
void func_BD7C_b1(void)
{
    REGS_IN;

    mff_cycles(0xBD7C, 9);
    a = RAM(PLAYER_DIR);
    if (a != RAM(OBJ_DIR + x)) {
        mff_cycles(0xBD83, 6);
        a = RAM(OBJ_DIR + x) ^ 0x03;
    } else {
        mff_cycles(0xBD81, 1);
    }
    mff_cycles(0xBD88, 5);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    debris_fly();
}

/*
 * debris_blink, $BDAF (bank $03; name proposed): obj_timer - 1; at 0
 * nothing more (the halves stay put until cleared off the screen edge);
 * otherwise the slot is hidden on even counts and shown on odd ones
 * (obj_flags bit 2).
 * In: X = slot. Out: the RTS.
 * Callers: JMP at 03:BD9F; branches from 03:BDAA (debris_fly) and from
 * 03:BDDD, BDE2 (drum_drop_item, which leaves through it).
 */
void func_BDAF_b1(void)
{
    REGS_IN;

    mff_cycles(0xBDAF, 13);
    RAM(OBJ_TIMER + x)--;
    a = RAM(OBJ_TIMER + x);
    if (a != 0) {
        mff_cycles(0xBDB7, 4);
        a &= 0x01;
        if (a == 0) {
            mff_cycles(0xBDBB, 13);
            a = RAM(OBJ_FLAGS + x) | OBJF_HIDDEN;
            RAM(OBJ_FLAGS + x) = a;
            mff_cycles(0xBDC3, 1);              /* BNE, always taken */
        } else {
            mff_cycles(0xBDB9, 1);
            mff_cycles(0xBDC5, 11);
            a = RAM(OBJ_FLAGS + x) & (uint8_t)~OBJF_HIDDEN;
            RAM(OBJ_FLAGS + x) = a;
        }
    } else {
        mff_cycles(0xBDB5, 1);
    }
    mff_cycles(0xBDCD, 6);
    RTS_RETURN();
}

/*
 * drum_drop_item, $BDCE (bank $03; name proposed): the item a smashed drum
 * leaves. The first drum of the area (obj_land_y negative) always leaves
 * one; the others when rng + rng+1 (with the carry in) comes out $7F or
 * more. In a free slot ($0D..3), at the drum's x and $0C below its y,
 * cleared fields, attack $1F (not out): the first drum's item is id $10
 * with animation $61, the others id and animation from rng+1 + rng+2,
 * & $0F ($BE88/$BE78), id $69 (the character's weapon) with the animation
 * $BE98[player_id]. Then obj_flags $90 ($BE72). With no item, or no free
 * slot, it goes on into debris_blink.
 * In: X = slot, carry. Out: the RTS (Y = the item's slot).
 * Caller: drum_smashed (03:BD79).
 */
void func_BDCE_b1(void)
{
    REGS_IN;

    mff_cycles(0xBDCE, 6);
    a = RAM(OBJ_LAND_Y + x);
    if (!(a & 0x80)) {
        mff_cycles(0xBDD1, 1);
        mff_cycles(0xBDD5, 13);
        a = RAM(RNG);
        m = RAM(RNG + 1);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(RNG) = a;
        if (a < ITEM_DROP_ODDS) {
            mff_cycles(0xBDAF, 1);
            SYNC_OUT;
            func_BDAF_b1();
            return;
        }
    } else {
        mff_cycles(0xBDD3, 3);                  /* BPL not taken, BMI taken */
    }
    mff_cycles(0xBDDF, 6);
    CALL_FIXED(0xBDDF, func_F71A);              /* find_free_slot */
    mff_cycles(0xBDE2, 2);
    if (c) {
        mff_cycles(0xBDAF, 1);
        SYNC_OUT;
        func_BDAF_b1();
        return;
    }
    mff_cycles(0xBDE4, 128);
    RAM(TMP0) = y;
    a = RAM(OBJ_X_HI + x);
    RAM(OBJ_X_HI + y) = a;
    a = RAM(OBJ_Y_HI + x);
    RAM(OBJ_Y_HI + y) = a;
    a = RAM(OBJ_X + x);
    RAM(OBJ_X + y) = a;
    a = RAM(OBJ_Y + x);
    c = 0;
    ADC_A(0x0C);
    RAM(OBJ_Y + y) = a;
    a = 0x00;
    RAM(OBJ_LIFE + y) = a;
    RAM(OBJ_LIFE_MAX + y) = a;
    RAM(OBJ_THROW_FLAG + y) = a;
    RAM(OBJ_HIT_COOLDOWN + y) = a;
    RAM(OBJ_INVULN + y) = a;
    RAM(OBJ_HIT_PLAYER + y) = a;
    RAM(OBJ_HIT_TIMER + y) = a;
    RAM(OBJ_HELD + y) = a;
    RAM(OBJ_KIND + y) = a;
    RAM(OBJ_TIER + y) = a;
    RAM(OBJ_AI_TIMER + y) = a;
    RAM(OBJ_AI_COUNT + y) = a;
    RAM(OBJ_AI_FLAG + y) = a;
    RAM(OBJ_ATTACK_DAMAGE + y) = a;
    a = ITEM_ATTACK;
    RAM(OBJ_ATTACK + y) = a;
    a = RAM(OBJ_LAND_Y + x);
    if (a & 0x80) {                             /* the first drum: food */
        mff_cycles(0xBE37, 15);
        a = OBJ_ITEM_FOOD;
        RAM(OBJ_ID + y) = a;
        a = FOOD_ANIM;
        CALL_FIXED(0xBE3E, func_F2A0);          /* obj_spawn */
        mff_cycles(0xBE41, 3);
        GO_IN(0xBE72, BANK_03);
    }
    mff_cycles(0xBE35, 1);
    mff_cycles(0xBE44, 20);
    a = RAM(RNG + 1);
    m = RAM(RNG + 2);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(RNG + 1) = a;
    a &= 0x0F;
    y = a;
    RAM(TMP1) = y;
    a = prg_read((uint16_t)(ITEM_IDS + y));
    mff_cycles(0xBE52, 12);
    y = RAM(TMP0);
    RAM(OBJ_ID + y) = a;
    c = a >= OBJ_WEAPON;
    if (a == OBJ_WEAPON) {                      /* never executed */
        mff_cycles(0xBE5B, 7);
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(WEAPON_ANIMS + y));
        mff_cycles(0xBE60, 9);
        y = RAM(TMP0);
        CALL_FIXED(0xBE62, func_F2A0);
        mff_cycles(0xBE65, 3);
        GO_IN(0xBE72, BANK_03);
    }
    mff_cycles(0xBE59, 1);
    mff_cycles(0xBE68, 7);
    y = RAM(TMP1);
    a = prg_read((uint16_t)(ITEM_ANIMS + y));
    mff_cycles(0xBE6D, 9);
    y = RAM(TMP0);
    CALL_FIXED(0xBE6F, func_F2A0);
    SYNC_OUT;
    func_BE72_b1();
}

/*
 * $BE72 (bank $03): the item's obj_flags = $90 (on screen, world
 * coordinates). In: Y = the item's slot. Out: the RTS.
 * Callers: JMPs at 03:BE41, BE65; the fall-through from 03:BE71.
 */
void func_BE72_b1(void)
{
    REGS_IN;

    mff_cycles(0xBE72, 13);
    a = 0x90;
    RAM(OBJ_FLAGS + y) = a;
    RTS_RETURN();
}

/*
 * items_two_on_screen, $BE9B (bank $03; name proposed): counts the live
 * slots $0D..1 holding an item (ids $10-$13) or a weapon ($69) into TMP0.
 * Out: A = $FF and Z clear when there are two or more (never executed),
 * else A = 0, Z set; carry = count >= 2.
 * Caller: drum_smashed (03:BD74).
 */
void func_BE9B_b1(void)
{
    REGS_IN;

    mff_cycles(0xBE9B, 7);
    a = 0x00;
    RAM(TMP0) = a;
    y = OBJ_SLOT_ENEMY_LAST;
    for (;;) {
        mff_cycles(0xBEA1, 6);
        a = RAM(OBJ_STATE + y);
        if (a & OBJ_LIVE) {
            mff_cycles(0xBEA6, 4);
            a = RAM(OBJ_ID + y);
            if (cmp_list(0xBEA9, 0xBEBF, a, k_item_ids, 5) < 5) {
                mff_cycles(0xBEBF, 5);          /* never executed */
                RAM(TMP0)++;
            } else {
                mff_cycles(0xBEBD, 3);          /* BNE, always taken */
            }
        } else {
            mff_cycles(0xBEA4, 1);
        }
        mff_cycles(0xBEC1, 6);
        y--;
        if (y == 0x00)
            break;
        mff_cycles(0xBEA1, 1);
    }
    mff_cycles(0xBEC6, 7);
    a = RAM(TMP0);
    c = a >= ITEMS_ON_SCREEN_MAX;
    if (c) {                                    /* never executed */
        mff_cycles(0xBECC, 8);
        a = 0xFF;
        SET_NZ(a);
        RTS_RETURN();
    }
    mff_cycles(0xBECA, 1);
    mff_cycles(0xBECF, 8);
    a = 0x00;
    SET_NZ(a);
    RTS_RETURN();
}

/* ======================= the bonus stage (bank $0B) ======================= */

/*
 * bonus_stage, $B331 (bank $0B; name proposed): behaviour of id $70 (and
 * $71, which nothing makes), the bonus stage's controller in slot $0F.
 * Nothing runs while a special screen is up; otherwise the phase:
 *   0 ($B349) start: music 9, twenty drums, the first in $78 frames;
 *   1 ($B366) a drum if fewer than two roll; when all are done, the result;
 *   2 ($B403) wait obj_land_y frames ($40 between drums), back to 1;
 *   3 ($B411) the prize falls; landed, the player's win pose.
 * In: X = slot ($0F). Out: as the phase.
 * Caller: the behaviour table (ids $70, $71: 0B:B300, B303).
 */
void func_B331_b5(void)
{
    REGS_IN;

    mff_cycles(0xB331, 5);
    a = RAM(SPECIAL_SCREEN_ID);
    if (a != 0) {
        mff_cycles(0xB335, 6);
        RTS_RETURN();
    }
    mff_cycles(0xB333, 1);
    SYNC_OUT;
    obj_phase_jump(0xB336, BONUS_PHASE_LO, BONUS_PHASE_HI);
}

/*
 * $B349, bonus phase 0: music 9 (0F:F34A), obj_timer = 20 drums to roll,
 * bonus_drum_count ($B9) and the live drums (obj_ground_y) 0, the first
 * wait obj_land_y $78; phase 2.
 */
void func_B349_b5(void)
{
    REGS_IN;

    mff_cycles(0xB349, 8);
    a = MUSIC_BONUS;
    CALL_FIXED(0xB34B, func_F34A);              /* sound_queue_push (music) */
    mff_cycles(0xB34E, 44);
    a = BONUS_DRUMS;
    RAM(OBJ_TIMER + x) = a;
    a = 0x00;
    RAM(BONUS_DRUM_COUNT) = a;
    RAM(OBJ_GROUND_Y + x) = a;
    a = BONUS_FIRST_WAIT;
    RAM(OBJ_LAND_Y + x) = a;
    RAM(OBJ_STATE + x) += 2;                    /* INC, INC: phase 2 */
    RTS_RETURN();
}

/*
 * $B366, bonus phase 1. While drums are left to roll or still rolling:
 * unless two roll, one more ($B46F) and phase 2. When all are gone, once
 * the player stands (game_action 0 or 3): with eight or more smashed the
 * prize appears at the top of slot 6, hidden (animation and id by the
 * count, $B66C/$B681; all twenty also bring special screen $FC), phase 3;
 * with fewer, game_action $29 and action_timer $0C, the controller is
 * cleared and special screen $FB.
 */
static void bonus_result(void);

void func_B366_b5(void)
{
    REGS_IN;

    mff_cycles(0xB366, 6);
    a = RAM(OBJ_TIMER + x);
    if (a == 0) {
        mff_cycles(0xB36B, 6);
        a = RAM(OBJ_GROUND_Y + x);
        if (a == 0) {
            mff_cycles(0xB36E, 1);
            SYNC_OUT;
            bonus_result();
            return;
        }
    } else {
        mff_cycles(0xB369, 1);
    }
    mff_cycles(0xB370, 8);
    a = RAM(OBJ_GROUND_Y + x);
    if (a != BONUS_LIVE_MAX) {
        mff_cycles(0xB377, 6);
        CALL_IN(0xB377, 0xB46F, BANK_0B);
        mff_cycles(0xB37A, 7);
        RAM(OBJ_STATE + x)++;
    } else {
        mff_cycles(0xB375, 1);
    }
    mff_cycles(0xB37D, 6);
    RTS_RETURN();
}

/* $B37E-$B402: the end of bonus phase 1, the result */
static void bonus_result(void)
{
    REGS_IN;

    mff_cycles(0xB37E, 5);
    a = RAM(GAME_ACTION);
    if (a != ACT_IDLE) {
        mff_cycles(0xB382, 4);
        if (a != ACT_PUNCH) {
            mff_cycles(0xB386, 6);
            RTS_RETURN();
        }
        mff_cycles(0xB384, 1);
    } else {
        mff_cycles(0xB380, 1);
    }
    mff_cycles(0xB387, 7);
    a = RAM(BONUS_DRUM_COUNT);
    c = a >= BONUS_PRIZE_MIN;
    if (c) {
        mff_cycles(0xB38D, 75);
        a = 0x80;
        RAM(OBJ_STATE + BONUS_PRIZE_SLOT) = a;
        RAM(OBJ_X + BONUS_PRIZE_SLOT) = a;
        a = 0x90;
        RAM(OBJ_FLAGS + BONUS_PRIZE_SLOT) = a;
        a = RAM(OBJ_X_HI);
        RAM(OBJ_X_HI + BONUS_PRIZE_SLOT) = a;
        a = RAM(OBJ_Y_HI);
        RAM(OBJ_Y_HI + BONUS_PRIZE_SLOT) = a;
        a = 0x00;
        RAM(OBJ_Y + BONUS_PRIZE_SLOT) = a;
        RAM(OBJ_ANIM_TIMER + BONUS_PRIZE_SLOT) = a;
        RAM(OBJ_ANIM_FRAME + BONUS_PRIZE_SLOT) = a;
        RAM(OBJ_VX + BONUS_PRIZE_SLOT) = a;
        RAM(OBJ_VX_FRAC + BONUS_PRIZE_SLOT) = a;
        RAM(OBJ_VY + BONUS_PRIZE_SLOT) = a;
        RAM(OBJ_VY_FRAC + BONUS_PRIZE_SLOT) = a;
        a = ITEM_ATTACK;
        RAM(OBJ_ATTACK + BONUS_PRIZE_SLOT) = a;
        y = RAM(BONUS_DRUM_COUNT);
        a = prg_read((uint16_t)(PRIZE_ANIMS + y));
        mff_cycles(0xB3C7, 11);
        RAM(OBJ_ANIM + BONUS_PRIZE_SLOT) = a;
        a = RAM(BONUS_DRUM_COUNT);
        if (a == BONUS_DRUMS) {
            mff_cycles(0xB3D0, 5);
            a = SCREEN_BONUS_ALL;
            RAM(SPECIAL_SCREEN_ID) = a;
        } else {
            mff_cycles(0xB3CE, 1);
        }
        mff_cycles(0xB3D4, 4);
        a = prg_read((uint16_t)(PRIZE_IDS + y));
        mff_cycles(0xB3D7, 34);
        RAM(OBJ_ID + BONUS_PRIZE_SLOT) = a;
        a = RAM(OBJ_FLAGS + BONUS_PRIZE_SLOT) | OBJF_HIDDEN;
        RAM(OBJ_FLAGS + BONUS_PRIZE_SLOT) = a;
        RAM(OBJ_STATE + x) += 2;                /* INC, INC: phase 3 */
        RTS_RETURN();
    }
    mff_cycles(0xB38B, 1);
    mff_cycles(0xB3E9, 28);
    a = 0x00;
    RAM(OBJ_STATE + BONUS_PRIZE_SLOT) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = ACT_BONUS_FAIL;
    RAM(GAME_ACTION) = a;
    a = 0x0C;
    RAM(ACTION_TIMER) = a;
    CALL_FIXED(0xB3FB, func_F2BE);              /* object_clear */
    mff_cycles(0xB3FE, 11);
    a = SCREEN_BONUS_FAIL;
    RAM(SPECIAL_SCREEN_ID) = a;
    RTS_RETURN();
}

/*
 * $B403, bonus phase 2: obj_land_y - 1; at 0 it is reloaded with $40 and
 * the phase goes back to 1.
 */
void func_B403_b5(void)
{
    REGS_IN;

    mff_cycles(0xB403, 9);
    RAM(OBJ_LAND_Y + x)--;
    if (RAM(OBJ_LAND_Y + x) == 0) {
        mff_cycles(0xB408, 14);
        a = BONUS_WAIT;
        RAM(OBJ_LAND_Y + x) = a;
        RAM(OBJ_STATE + x)--;
    } else {
        mff_cycles(0xB406, 1);
    }
    mff_cycles(0xB410, 6);
    RTS_RETURN();
}

/*
 * $B411, bonus phase 3: the prize in slot 6 falls (shown, 0F:F1FD with
 * X = 6) until y $8C; then, once the player stands (game_action 0 or 3),
 * it rests at $8C and the player takes the win pose: animation $E1E6 and
 * sprite bank $E22B by player_id, speeds from $E1C1-$E1CA, obj_state $80,
 * game_action $1A. The controller is cleared, special screen $FE.
 * Out: X = $0F after the fall.
 */
void func_B411_b5(void)
{
    REGS_IN;

    mff_cycles(0xB411, 8);
    a = RAM(OBJ_Y + BONUS_PRIZE_SLOT);
    c = a >= BONUS_PRIZE_Y;
    if (!c) {
        mff_cycles(0xB416, 1);
        mff_cycles(0xB45F, 18);
        a = RAM(OBJ_FLAGS + BONUS_PRIZE_SLOT) & (uint8_t)~OBJF_HIDDEN;
        RAM(OBJ_FLAGS + BONUS_PRIZE_SLOT) = a;
        x = BONUS_PRIZE_SLOT;
        CALL_FIXED(0xB469, func_F1FD);          /* obj_fall */
        mff_cycles(0xB46C, 8);
        x = BONUS_SLOT;
        RTS_RETURN();
    }
    mff_cycles(0xB418, 5);
    a = RAM(GAME_ACTION);
    if (a != ACT_IDLE) {
        mff_cycles(0xB41C, 4);
        c = a >= ACT_PUNCH;
        if (a != ACT_PUNCH) {
            mff_cycles(0xB420, 6);
            RTS_RETURN();
        }
        mff_cycles(0xB41E, 1);
    } else {
        mff_cycles(0xB41A, 1);
    }
    mff_cycles(0xB421, 13);
    a = BONUS_PRIZE_Y;
    RAM(OBJ_Y + BONUS_PRIZE_SLOT) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(PLAYER_WIN_ANIM + y));
    mff_cycles(0xB42B, 6);
    CALL_FIXED(0xB42B, func_F294);              /* player_set_anim */
    mff_cycles(0xB42E, 4);
    a = prg_read((uint16_t)(PLAYER_WIN_CHR + y));
    SET_NZ(a);
    mff_cycles(0xB431, 6);
    CALL_FIXED(0xB431, func_F440);              /* sprite bank R2 = A */
    mff_cycles(0xB434, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(PLAYER_WIN_VX_FRAC + y));
    mff_cycles(0xB439, 8);
    RAM(OBJ_VX_FRAC) = a;
    a = prg_read((uint16_t)(PLAYER_WIN_VX + y));
    mff_cycles(0xB43F, 8);
    RAM(OBJ_VX) = a;
    a = prg_read((uint16_t)(PLAYER_WIN_VY_FRAC + y));
    mff_cycles(0xB445, 8);
    RAM(OBJ_VY_FRAC) = a;
    a = prg_read((uint16_t)(PLAYER_WIN_VY + y));
    mff_cycles(0xB44B, 21);
    RAM(OBJ_VY) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = ACT_BONUS_PRIZE;
    RAM(GAME_ACTION) = a;
    CALL_FIXED(0xB457, func_F2BE);              /* object_clear */
    mff_cycles(0xB45A, 11);
    a = SCREEN_BONUS_PRIZE;
    RAM(SPECIAL_SCREEN_ID) = a;
    RTS_RETURN();
}

/*
 * bonus_drum_spawn, $B46F (bank $0B; name proposed): one more drum, if a
 * slot is free and obj_timer (drums left) is not 0: obj_timer - 1, the
 * live count (obj_ground_y) + 1, a drum (id $72, animation $7B, attack
 * $9E, damage $3F) at the top of the screen page, obj_vx $0100, or $0140
 * when rng (rotated through the carry) >> 1 & 7 picks it ($B656). The
 * launch pattern comes from rng+1 ^ the player's y: & 3 into $B666
 * (patterns 0, 2, 4, 5), or, when another drum still rolls on the near line
 * ($8C), & 1 into $B66A (patterns 1, 3, the far line). TMP2 = the slot,
 * TMP3 = the pattern; then $B512.
 * In: X = the controller. Out: as $B512.
 * Caller: bonus phase 1 (0B:B377).
 */
void func_B46F_b5(void)
{
    REGS_IN;

    mff_cycles(0xB46F, 6);
    CALL_FIXED(0xB46F, func_F71A);              /* find_free_slot */
    mff_cycles(0xB472, 2);
    if (c) {
        mff_cycles(0xB474, 6);
        RTS_RETURN();
    }
    mff_cycles(0xB472, 1);
    mff_cycles(0xB475, 6);
    a = RAM(OBJ_TIMER + x);
    if (a == 0) {
        mff_cycles(0xB474, 1);
        mff_cycles(0xB474, 6);
        RTS_RETURN();
    }
    mff_cycles(0xB47A, 98);
    RAM(OBJ_TIMER + x)--;
    RAM(OBJ_GROUND_Y + x)++;
    a = DRUM_DAMAGE;
    RAM(OBJ_ATTACK_DAMAGE + y) = a;
    a = DRUM_ATTACK;
    RAM(OBJ_ATTACK + y) = a;
    a = 0x00;
    RAM(OBJ_LIFE + y) = a;
    RAM(OBJ_LIFE_MAX + y) = a;
    RAM(OBJ_KIND + y) = a;
    RAM(OBJ_GROUND_Y + y) = a;
    RAM(OBJ_VY + y) = a;
    RAM(OBJ_VY_FRAC + y) = a;
    a = RAM(OBJ_X_HI);
    RAM(OBJ_X_HI + y) = a;
    a = RAM(OBJ_Y_HI);
    RAM(OBJ_Y_HI + y) = a;
    a = OBJ_BONUS_DRUM;
    RAM(OBJ_ID + y) = a;
    RAM(OBJ_SAVED_ID + y) = a;
    a = DRUM_ANIM;
    CALL_FIXED(0xB4B4, func_F2A0);              /* obj_spawn */
    mff_cycles(0xB4B7, 30);
    a = 0x01;
    RAM(OBJ_VX + y) = a;
    a = 0x00;
    RAM(OBJ_VX_FRAC + y) = a;
    RAM(TMP2) = y;
    a = RAM(RNG);
    a = (uint8_t)(a >> 1 | c << 7);             /* ROR */
    a &= 0x07;
    y = a;
    a = prg_read((uint16_t)(BDRUM_FAST + y));
    mff_cycles(0xB4CC, 2);
    if (a != 0) {
        mff_cycles(0xB4CE, 10);
        y = RAM(TMP2);
        a = 0x40;
        RAM(OBJ_VX_FRAC + y) = a;
    } else {
        mff_cycles(0xB4CC, 1);
    }
    /* another drum still rolling on the near line? */
    mff_cycles(0xB4D5, 2);
    y = 0x02;
    for (;;) {
        mff_cycles(0xB4D7, 6);
        a = RAM(OBJ_STATE + y);
        if (!(a & OBJ_LIVE)) {
            mff_cycles(0xB4DA, 1);
        } else {
            mff_cycles(0xB4DC, 8);
            a = RAM(OBJ_ID + y);
            if (a != OBJ_BONUS_DRUM) {
                mff_cycles(0xB4E1, 1);
            } else {
                mff_cycles(0xB4E3, 5);
                if (y != RAM(TMP2)) {
                    mff_cycles(0xB4E5, 1);
                    mff_cycles(0xB4EE, 8);
                    a = RAM(OBJ_TIMER2 + y);
                    c = a >= BDRUM_LINE_NEAR;
                    if (a == BDRUM_LINE_NEAR) {
                        mff_cycles(0xB4F3, 2);
                        mff_cycles(0xB505, 15);
                        a = (RAM(RNG + 1) ^ RAM(OBJ_Y)) & 0x01;
                        y = a;
                        a = prg_read((uint16_t)(BDRUM_PATTERNS_HIGH + y));
                        mff_cycles(0xB510, 3);
                        RAM(TMP3) = a;
                        SYNC_OUT;
                        func_B512_b5();
                        return;
                    }
                    break;
                }
            }
        }
        mff_cycles(0xB4E7, 6);
        y++;
        c = y >= 0x0F;
        if (y == 0x0F) {
            mff_cycles(0xB4EC, 3);              /* BNE not taken, BEQ taken */
            break;
        }
        mff_cycles(0xB4D7, 1);
    }
    mff_cycles(0xB4F5, 15);
    a = (RAM(RNG + 1) ^ RAM(OBJ_Y)) & 0x03;
    y = a;
    a = prg_read((uint16_t)(BDRUM_PATTERNS + y));
    mff_cycles(0xB500, 6);
    RAM(TMP3) = a;
    GO_IN(0xB512, BANK_0B);
}

/*
 * $B54D / $B563: a rolling drum on its line: obj_y and obj_timer2 (the
 * line it comes back to after a bounce) = `line`, maybe a bouncer
 * ($B5AF); RTS.
 */
static void bonus_drum_line(uint16_t pc, uint8_t line)
{
    REGS_IN;

    mff_cycles(pc, 18);
    a = line;
    RAM(OBJ_Y + y) = a;
    RAM(OBJ_TIMER2 + y) = a;
    CALL_IN((uint16_t)(pc + 0x08), 0xB5AF, BANK_0B);
    mff_cycles((uint16_t)(pc + 0x0B), 6);
    RTS_RETURN();
}

/*
 * $B52B / $B537: from the left edge (x 1, moving right), then the line at
 * line_pc (BNE, always taken).
 */
static void bonus_drum_from_left(uint16_t pc, uint16_t line_pc, uint8_t line)
{
    REGS_IN;

    mff_cycles(pc, 16);
    a = 0x01;
    RAM(OBJ_DIR + y) = a;
    RAM(OBJ_X + y) = a;
    mff_cycles((uint16_t)(pc + 0x0A), 1);
    SYNC_OUT;
    bonus_drum_line(line_pc, line);
}

/*
 * $B591: a drum that drops in from the top (y 1, landing on $8C), at rest
 * until it lands (obj_kind 1); RTS.
 */
static void bonus_drum_drop(void)
{
    REGS_IN;

    mff_cycles(0xB591, 49);
    a = 0x01;
    RAM(OBJ_Y + y) = a;
    a = BDRUM_LINE_NEAR;
    RAM(OBJ_TIMER2 + y) = a;
    a = 0x00;
    RAM(OBJ_VX + y) = a;
    RAM(OBJ_VX_FRAC + y) = a;
    RAM(OBJ_VY + y) = a;
    RAM(OBJ_VY_FRAC + y) = a;
    a = BDRUM_DROPPING;
    RAM(OBJ_KIND + y) = a;
    RTS_RETURN();
}

/*
 * bonus_drum_launch, $B512 (bank $0B; name proposed): where the new drum
 * (slot TMP2) starts, by the pattern in TMP3:
 *   0 from the left on the near line ($8C), 1 from the left on the far line
 *   ($A4), 2 and 3 the same from the right (x $FF); 4 drops in at x $50
 *   moving right and 5+ at x $B0 moving left ($B581), but only from round
 *   index 2 on: before that they are patterns 0 and 2.
 * The carry the compares leave goes on into $B5AF's ADC.
 * Callers: JMP at 0B:B502; the fall-through from 0B:B510.
 */
void func_B512_b5(void)
{
    REGS_IN;

    mff_cycles(0xB512, 8);
    y = RAM(TMP2);
    a = RAM(TMP3);
    if (a == 0) {
        mff_cycles(0xB516, 1);
        SYNC_OUT;
        bonus_drum_from_left(0xB52B, 0xB54D, BDRUM_LINE_NEAR);
        return;
    }
    mff_cycles(0xB518, 4);
    c = a >= 0x01;
    if (a == 0x01) {
        mff_cycles(0xB51A, 1);
        SYNC_OUT;
        bonus_drum_from_left(0xB537, 0xB563, BDRUM_LINE_FAR);
        return;
    }
    mff_cycles(0xB51C, 4);
    c = a >= 0x02;
    if (a == 0x02) {
        mff_cycles(0xB51E, 1);
        SYNC_OUT;
        func_B543_b5();
        return;
    }
    mff_cycles(0xB520, 4);
    c = a >= 0x03;
    if (a == 0x03) {
        mff_cycles(0xB522, 1);
        mff_cycles(0xB559, 14);
        a = DIR_LEFT;
        RAM(OBJ_DIR + y) = a;
        a = 0xFF;
        RAM(OBJ_X + y) = a;
        SYNC_OUT;
        bonus_drum_line(0xB563, BDRUM_LINE_FAR);
        return;
    }
    mff_cycles(0xB524, 4);
    c = a >= 0x04;
    if (a != 0x04) {
        mff_cycles(0xB528, 3);
        GO_IN(0xB581, BANK_0B);
    }
    mff_cycles(0xB526, 1);
    mff_cycles(0xB56F, 7);
    a = RAM(ROUND);
    c = a >= 0x02;
    if (!c) {
        mff_cycles(0xB52B, 1);
        SYNC_OUT;
        bonus_drum_from_left(0xB52B, 0xB54D, BDRUM_LINE_NEAR);
        return;
    }
    mff_cycles(0xB575, 16);
    a = DIR_RIGHT;
    RAM(OBJ_DIR + y) = a;
    a = 0x50;
    RAM(OBJ_X + y) = a;
    mff_cycles(0xB57F, 1);
    SYNC_OUT;
    bonus_drum_drop();
}

/*
 * $B543 (bank $0B): launch pattern 2, from the right edge (x $FF, moving
 * left) on the near line.
 * Callers: the branches from 0B:B51E and 0B:B585.
 */
void func_B543_b5(void)
{
    REGS_IN;

    mff_cycles(0xB543, 14);
    a = DIR_LEFT;
    RAM(OBJ_DIR + y) = a;
    a = 0xFF;
    RAM(OBJ_X + y) = a;
    SYNC_OUT;
    bonus_drum_line(0xB54D, BDRUM_LINE_NEAR);
}

/*
 * $B581 (bank $0B): launch patterns 5 and up: from round index 2 on a drum
 * dropping in at x $B0, moving left; before that pattern 2.
 * Caller: JMP at 0B:B528.
 */
void func_B581_b5(void)
{
    REGS_IN;

    mff_cycles(0xB581, 7);
    a = RAM(ROUND);
    c = a >= 0x02;
    if (!c) {
        mff_cycles(0xB585, 1);
        SYNC_OUT;
        func_B543_b5();
        return;
    }
    mff_cycles(0xB587, 14);
    a = DIR_LEFT;
    RAM(OBJ_DIR + y) = a;
    a = 0xB0;
    RAM(OBJ_X + y) = a;
    SYNC_OUT;
    bonus_drum_drop();
}

/*
 * bonus_drum_bounce_pick, $B5AF (bank $0B; name proposed): a rolling drum
 * becomes a bouncer (obj_kind 2: obj_vx $0140, obj_vy $0280) when
 * (rng + spawn_timer + carry) & 7 picks it ($B65E: two in eight).
 * In: TMP2 = the drum's slot, carry. Out: the RTS.
 * Callers: 0B:B555, B56B.
 */
void func_B5AF_b5(void)
{
    REGS_IN;

    mff_cycles(0xB5AF, 14);
    a = RAM(RNG);
    ADC_A(RAM(SPAWN_TIMER));
    a &= 0x07;
    y = a;
    a = prg_read((uint16_t)(BDRUM_BOUNCES + y));
    mff_cycles(0xB5B9, 2);
    if (a != 0) {
        mff_cycles(0xB5BB, 38);
        y = RAM(TMP2);
        a = 0x01;
        RAM(OBJ_VX + y) = a;
        a = 0x02;
        RAM(OBJ_VY + y) = a;
        a = 0x40;
        RAM(OBJ_VX_FRAC + y) = a;
        a = 0x80;
        RAM(OBJ_VY_FRAC + y) = a;
        a = BDRUM_BOUNCING;
        RAM(OBJ_KIND + y) = a;
    } else {
        mff_cycles(0xB5B9, 1);
    }
    mff_cycles(0xB5D6, 6);
    RTS_RETURN();
}

/*
 * bonus_drum, $B5D7 (bank $0B; name proposed): behaviour of id $72. A
 * dropping or bouncing drum (obj_kind not 0) falls (0F:F1FD) and lands on
 * its line (obj_timer2): a bouncer jumps again (obj_vy $0280); a dropping
 * one bounces (obj_vy $0205) and at its second landing rolls on (obj_vx
 * $0100, obj_kind 0; never executed). Then it rolls ($B625).
 * In: X = slot. Out: the RTS, or object_clear's.
 * Caller: the behaviour table (id $72, 0B:B306).
 */
void func_B5D7_b5(void)
{
    REGS_IN;

    mff_cycles(0xB5D7, 6);
    a = RAM(OBJ_KIND + x);
    if (a == BDRUM_ROLLING) {
        mff_cycles(0xB5DA, 2);
        SYNC_OUT;
        func_B625_b5();
        return;
    }
    mff_cycles(0xB5DC, 6);
    CALL_FIXED(0xB5DC, func_F1FD);              /* obj_fall */
    mff_cycles(0xB5DF, 10);
    a = RAM(OBJ_Y + x);
    c = a >= RAM(OBJ_TIMER2 + x);
    if (!c) {
        mff_cycles(0xB5E5, 2);
        SYNC_OUT;
        func_B625_b5();
        return;
    }
    mff_cycles(0xB5E7, 17);
    a = RAM(OBJ_TIMER2 + x);
    RAM(OBJ_Y + x) = a;
    a = RAM(OBJ_KIND + x);
    c = a >= BDRUM_DROPPING;
    if (a != BDRUM_DROPPING) {
        mff_cycles(0xB5F4, 17);
        a = 0x02;
        RAM(OBJ_VY + x) = a;
        a = 0x80;
        RAM(OBJ_VY_FRAC + x) = a;
        GO_IN(0xB625, BANK_0B);
    }
    mff_cycles(0xB5F2, 2);
    mff_cycles(0xB601, 15);
    RAM(OBJ_GROUND_Y + x)++;
    a = RAM(OBJ_GROUND_Y + x);
    c = a >= 0x02;
    if (a != 0x02) {
        mff_cycles(0xB60B, 20);
        a = 0x02;
        RAM(OBJ_VY + x) = a;
        a = 0x05;
        RAM(OBJ_VY_FRAC + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xB609, 1);
    mff_cycles(0xB616, 21);
    a = 0x01;
    RAM(OBJ_VX + x) = a;
    a = 0x00;
    RAM(OBJ_VX_FRAC + x) = a;
    RAM(OBJ_KIND + x) = a;
    SYNC_OUT;
    func_B625_b5();
}

/*
 * $B625 (bank $0B): a bonus drum rolls: faces and moves right if obj_dir
 * bit 0, left if bit 1; once its x high byte differs from the player's it
 * has left the screen: the live count ($052F) - 1, JMP object_clear.
 * Callers: JMP at 0B:B5FE; the branches and fall-through of bonus_drum.
 */
void func_B625_b5(void)
{
    REGS_IN;

    mff_cycles(0xB625, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    if (a != 0) {
        mff_cycles(0xB62C, 6);
        CALL_FIXED(0xB62C, func_F090);          /* obj_face_right */
        mff_cycles(0xB62F, 6);
        CALL_FIXED(0xB62F, func_F183);          /* obj_move_right */
    } else {
        mff_cycles(0xB62A, 1);
    }
    mff_cycles(0xB632, 8);
    a = RAM(OBJ_DIR + x) & DIR_LEFT;
    if (a != 0) {
        mff_cycles(0xB639, 6);
        CALL_FIXED(0xB639, func_F099);          /* obj_face_left */
        mff_cycles(0xB63C, 6);
        CALL_FIXED(0xB63C, func_F1A1);          /* obj_move_left */
    } else {
        mff_cycles(0xB637, 1);
    }
    mff_cycles(0xB63F, 10);
    a = RAM(OBJ_X_HI + x);
    c = a >= RAM(OBJ_X_HI);
    if (a != RAM(OBJ_X_HI)) {
        mff_cycles(0xB647, 9);
        RAM(BONUS_LIVE_DRUMS)--;
        GO_FIXED(0xF2BE, func_F2BE);            /* object_clear */
    }
    mff_cycles(0xB645, 1);
    mff_cycles(0xB64D, 6);
    RTS_RETURN();
}
