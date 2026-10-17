/*
 * enemy_walk.c - how the common enemies of bank $0D move: walking a
 * direction with the background checks (enemy_walk), the speed of each
 * kind for the sixteen directions (enemy_set_speed), the pit checks, the
 * facing, and the stand, walk, attack and hurt animations. enemy_ai.c
 * has the phases that use them.
 *
 * Directions come as an index 0-15, 0 = up and clockwise in steps of 22.5
 * degrees (0F:FB5B gives the one toward the player); 0F:FBF0 turns it into
 * pad bits for obj_dir, and the speed tables below into obj_vx/obj_vy.
 *
 * In: X = slot (all of them).
 */
#include "enemy_ai.h"

/* by obj_kind: the animation and its sprite bank (for the chr_banks+3
 * slot of KIND_CHR_SLOT) of standing, walking and attacking */
#define STAND_ANIMS     0xAACB
#define STAND_CHR       0xAAD1
#define WALK_ANIMS      0xAAF4
#define WALK_CHR        0xAAFA
#define ATTACK_ANIMS    0xAB1D
#define ATTACK_CHR      0xAB23

/* by obj_kind: enemy_set_speed's setter (Poison 1.2 px a frame, Two P
 * and El Gado 1.4, Bred 0.8, AxL and Andore 0.9) */
#define SPEED_FN_LO     0xACA7
#define SPEED_FN_HI     0xACAD
/* by direction index: x and y speeds, fraction bytes (a sine and a
 * cosine), and the integer bytes the two faster setters share */
#define SPEED08_X       0xACC6
#define SPEED08_Y       0xACD6
#define SPEED09_X       0xACE6
#define SPEED09_Y       0xACF6
#define SPEED_HI_X      0xAD06
#define SPEED_HI_Y      0xAD16
#define SPEED12_X       0xAD26
#define SPEED12_Y       0xAD36
#define SPEED14_X       0xAD46
#define SPEED14_Y       0xAD56

/* by obj_kind * 16 + direction index: the frames of a maneuver's walk */
#define WALK_TIMES      0xAD9C
/* by obj_id - $20: the pause before each further hit of a combo */
#define HIT_PAUSES      0xAD86

/* the player's animations (slot 0) after which a held Bred shows the
 * other hurt pose: $94 with sprite bank $1D */
#define BRED_LOW_HURT_ANIM  0x94
#define BRED_LOW_HURT_CHR   0x1D

/*
 * enemy_walk_to_player, $A085 (name proposed): the direction index toward
 * the player (0F:FB5B), then enemy_walk_index, which the code runs on into.
 * Callers: the branch at $A078 (enemy_approach); JMP at $A431
 * (enemy_jump_start) and $A666 (enemy_slide) when the player is out of
 * their reach.
 */
void func_A085_b6(void)
{
    REGS_IN;

    mff_cycles(0xA085, 6);
    CALL_FIXED(0xA085, func_FB5B);              /* direction index to the player */
    FALL_INTO(func_A088_b6);
}

/*
 * enemy_walk_index, $A088 (name proposed): A = a direction index. obj_dir
 * from it (0F:FBF0), the kind's speed for it (enemy_set_speed), JMP
 * enemy_walk.
 * Callers: JMP at $A236 (enemy_close_in); the fall-through from $A085.
 */
void func_A088_b6(void)
{
    REGS_IN;

    mff_cycles(0xA088, 6);
    y = a;
    a = prg_read((uint16_t)(DIR_FROM_INDEX + y));
    mff_cycles(0xA08C, 11);
    RAM(OBJ_DIR + x) = a;
    CALL_BANK(0xA08F, 0xAC34);                  /* enemy_set_speed */
    mff_cycles(0xA092, 3);
    GOTO_BANK(0xA953);                          /* enemy_walk */
}

/*
 * enemy_walk_blocked, $A8F3 (name proposed): enemy_walk's step up or down
 * was stopped. By a pit (bit $40): back to phase 1 unless maneuvering,
 * obj_ai_flag flipped, the speed of index $0C and a step sideways toward
 * the player (0F:F051 left when he is left of it, 0F:F000 right),
 * JMP enemy_walk_face. By a wall while going straight up or down:
 * obj_ai_flag flipped, obj_ai_timer 0 and the sideways part of the walk
 * (BEQ enemy_walk_x). Diagonally: speed index $0C, JMP enemy_walk_x.
 * Callers: JMP at $A973 and $A992 (enemy_walk).
 */
void func_A8F3_b6(void)
{
    REGS_IN;

    mff_cycles(0xA8F3, 7);
    a = RAM(WALK_ATTRS) & TILE_PIT;
    if (a == 0) {
        mff_cycles(0xA8F9, 8);
        a = RAM(OBJ_DIR + x);
        c = a >= DIR_UP;
        if (a == DIR_UP) {
            mff_cycles(0xA8FE, 1);
        } else {
            mff_cycles(0xA900, 4);
            c = a >= DIR_DOWN;
            if (a != DIR_DOWN) {
                mff_cycles(0xA904, 8);
                y = 0x0C;
                CALL_BANK(0xA906, 0xAC34);      /* enemy_set_speed */
                mff_cycles(0xA909, 3);
                GOTO_BANK(0xA995);              /* enemy_walk_x */
            }
            mff_cycles(0xA902, 1);
        }
        mff_cycles(0xA944, 20);
        a = RAM(OBJ_AI_FLAG + x) ^ 0x01;
        RAM(OBJ_AI_FLAG + x) = a;
        a = 0x00;
        RAM(OBJ_AI_TIMER + x) = a;
        mff_cycles(0xA951, 1);                  /* BEQ, always taken */
        FALL_INTO(func_A995_b6);
    }
    mff_cycles(0xA8F7, 2);
    mff_cycles(0xA90C, 8);
    a = RAM(OBJ_STATE + x);
    c = a >= AI_MANEUVER;
    if (a != AI_MANEUVER) {
        mff_cycles(0xA913, 7);
        a = AI_APPROACH;
        RAM(OBJ_STATE + x) = a;
    } else {
        mff_cycles(0xA911, 1);
    }
    mff_cycles(0xA918, 19);
    a = RAM(OBJ_AI_FLAG + x) ^ 0x01;
    RAM(OBJ_AI_FLAG + x) = a;
    y = 0x0C;
    CALL_BANK(0xA922, 0xAC34);                  /* enemy_set_speed */
    mff_cycles(0xA925, 6);
    CALL_FIXED(0xA925, func_F374);              /* obj_dist_x: C = player to the right */
    mff_cycles(0xA928, 2);
    if (!c) {
        mff_cycles(0xA92A, 15);
        a = DIR_LEFT;
        RAM(OBJ_DIR + x) = a;
        y = 0x00;
        CALL_FIXED(0xA931, func_F051);          /* step left */
        mff_cycles(0xA934, 3);
        GOTO_BANK(0xA9E3);                      /* enemy_walk_face */
    }
    mff_cycles(0xA928, 1);
    mff_cycles(0xA937, 15);
    a = DIR_RIGHT;
    RAM(OBJ_DIR + x) = a;
    y = 0x01;
    CALL_FIXED(0xA93E, func_F000);              /* step right */
    mff_cycles(0xA941, 3);
    GOTO_BANK(0xA9E3);                          /* enemy_walk_face */
}

/*
 * enemy_walk, $A953 (name proposed): one frame's walk along obj_dir at
 * obj_vx/obj_vy, up or down first (0F:F0F7 / F0A2), then right or left
 * (enemy_walk_x, which the code runs on into). A step the background
 * pushed back (C set) ORs its tile attribute into WALK_ATTRS; a stopping
 * one (bit $40 or $10) up or down goes to enemy_walk_blocked with the
 * direction in WALK_BLOCKED.
 * Callers: the phases' walks (JMP at $A092, $A18B, $A40B; JSR at $A585,
 * $A785, $A83F).
 */
void func_A953_b6(void)
{
    REGS_IN;

    mff_cycles(0xA953, 16);
    a = 0x00;
    RAM(WALK_ATTRS) = a;
    RAM(WALK_BLOCKED) = a;
    a = RAM(OBJ_DIR + x) & DIR_UP;
    if (a != 0) {
        mff_cycles(0xA960, 8);
        y = 0x0A;
        CALL_FIXED(0xA962, func_F0F7);          /* step up */
        mff_cycles(0xA965, 2);
        if (!c) {
            mff_cycles(0xA965, 1);
            FALL_INTO(func_A995_b6);
        }
        mff_cycles(0xA967, 10);
        a = RAM(WALK_PROBE);
        RAM(WALK_ATTRS) = a;
        a &= TILE_STOP;
        if (a == 0) {
            mff_cycles(0xA96D, 1);
            FALL_INTO(func_A995_b6);
        }
        mff_cycles(0xA96F, 8);
        a = DIR_UP;
        RAM(WALK_BLOCKED) = a;
        GOTO_BANK(0xA8F3);                      /* enemy_walk_blocked */
    }
    mff_cycles(0xA95E, 1);
    mff_cycles(0xA976, 8);
    a = RAM(OBJ_DIR + x) & DIR_DOWN;
    if (a == 0) {
        mff_cycles(0xA97B, 1);
        FALL_INTO(func_A995_b6);
    }
    mff_cycles(0xA97D, 8);
    y = 0x09;
    CALL_FIXED(0xA97F, func_F0A2);              /* step down */
    mff_cycles(0xA982, 2);
    if (!c) {
        mff_cycles(0xA982, 1);
        FALL_INTO(func_A995_b6);
    }
    mff_cycles(0xA984, 13);
    a = RAM(WALK_PROBE) | RAM(WALK_ATTRS);
    RAM(WALK_ATTRS) = a;
    a &= TILE_STOP;
    if (a == 0) {
        mff_cycles(0xA98C, 1);
        FALL_INTO(func_A995_b6);
    }
    mff_cycles(0xA98E, 8);
    a = DIR_DOWN;
    RAM(WALK_BLOCKED) = a;
    GOTO_BANK(0xA8F3);                          /* enemy_walk_blocked */
}

/*
 * enemy_walk_x, $A995 (name proposed): the sideways part of enemy_walk,
 * right (0F:F000) or left (0F:F051) by obj_dir, a stopping step adding its
 * direction to WALK_BLOCKED. If any step met a pit (WALK_ATTRS bit $40):
 * obj_pit_flag 1 and phase 1. Then on into enemy_walk_face.
 * Callers: JMP at $A909 and BEQ at $A951 (enemy_walk_blocked); the
 * fall-through and branches of enemy_walk.
 */
void func_A995_b6(void)
{
    REGS_IN;

    mff_cycles(0xA995, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    if (a != 0) {
        mff_cycles(0xA99C, 8);
        y = 0x08;
        CALL_FIXED(0xA99E, func_F000);          /* step right */
        mff_cycles(0xA9A1, 2);
        if (c) {
            mff_cycles(0xA9A3, 13);
            a = RAM(WALK_PROBE) | RAM(WALK_ATTRS);
            RAM(WALK_ATTRS) = a;
            a &= TILE_STOP;
            if (a == 0) {
                mff_cycles(0xA9AB, 1);
            } else {
                mff_cycles(0xA9AD, 10);
                a = DIR_RIGHT | RAM(WALK_BLOCKED);
                RAM(WALK_BLOCKED) = a;
                mff_cycles(0xA9B3, 1);          /* BNE, always taken */
            }
            goto pit;
        }
        mff_cycles(0xA9A1, 1);
    } else {
        mff_cycles(0xA99A, 1);
    }
    mff_cycles(0xA9B5, 8);
    a = RAM(OBJ_DIR + x) & DIR_LEFT;
    if (a != 0) {
        mff_cycles(0xA9BC, 8);
        y = 0x09;
        CALL_FIXED(0xA9BE, func_F051);          /* step left */
        mff_cycles(0xA9C1, 2);
        if (c) {
            mff_cycles(0xA9C3, 13);
            a = RAM(WALK_PROBE) | RAM(WALK_ATTRS);
            RAM(WALK_ATTRS) = a;
            a &= TILE_STOP;
            if (a != 0) {
                mff_cycles(0xA9CD, 8);
                a = DIR_LEFT | RAM(WALK_BLOCKED);
                RAM(WALK_BLOCKED) = a;
            } else {
                mff_cycles(0xA9CB, 1);
            }
        } else {
            mff_cycles(0xA9C1, 1);
        }
    } else {
        mff_cycles(0xA9BA, 1);
    }
pit:
    mff_cycles(0xA9D3, 7);
    a = RAM(WALK_ATTRS) & TILE_PIT;
    if (a != 0) {
        mff_cycles(0xA9D9, 14);
        RAM(OBJ_PIT_FLAG + x) = 0x01;
        a = AI_APPROACH;
        RAM(OBJ_STATE + x) = a;
    } else {
        mff_cycles(0xA9D7, 1);
    }
    FALL_INTO(func_A9E3_b6);
}

/*
 * enemy_walk_face, $A9E3 (name proposed): the end of a walk: facing the
 * player (enemy_face_player, run on into), except while charging
 * (phase 14), which keeps its way.
 * Callers: JMP at $A934 and $A941 (enemy_walk_blocked); the fall-through
 * from enemy_walk_x.
 */
void func_A9E3_b6(void)
{
    REGS_IN;

    mff_cycles(0xA9E3, 8);
    a = RAM(OBJ_STATE + x);
    c = a >= AI_CHARGE;
    if (a == AI_CHARGE) {
        mff_cycles(0xA9EA, 6);
        RTS_RETURN();
    }
    mff_cycles(0xA9E8, 1);
    FALL_INTO(func_A9EB_b6);
}

/*
 * enemy_face_player, $A9EB (name proposed): the sprite turned toward the
 * player: JMP 0F:F090 (face right) when he is to the right (0F:F374, C
 * set), 0F:F099 (left) otherwise. obj_dir is left alone.
 * Callers: enemy_close_in (JSR $A210), enemy_charge_stalk (JSR $A788);
 * the fall-through from enemy_walk_face.
 */
void func_A9EB_b6(void)
{
    REGS_IN;

    mff_cycles(0xA9EB, 6);
    CALL_FIXED(0xA9EB, func_F374);              /* obj_dist_x: C = player to the right */
    mff_cycles(0xA9EE, 2);
    if (!c) {
        mff_cycles(0xA9F0, 3);
        GOTO_FIXED(0xF099, func_F099);          /* obj_face_left */
    }
    mff_cycles(0xA9EE, 1);
    mff_cycles(0xA9F3, 3);
    GOTO_FIXED(0xF090, func_F090);              /* obj_face_right */
}

/*
 * enemy_pit_under, $A9F6 (name proposed): a pit under the object (probe
 * point 6 of 0F:ECCF, bit $40) turns it into the pit fall: obj_saved_id
 * keeps the id, id $1F, obj_timer 0, obj_invuln $FF; C set. Otherwise C
 * clear. Right of x $03F0 (obj_pit_zone, 0C:9BE1) it returns from its
 * caller as well.
 * Callers: enemy_block, enemy_jump, enemy_slide, enemy_react (JSR $A1C9,
 * $A4D7, $A6D8, $AB66); the thrown bodies (0C:9128, 0C:92A9).
 */
void func_A9F6_b6(void)
{
    REGS_IN;

    mff_cycles(0xA9F6, 6);
    CALL_BANK(0xA9F6, 0x9BE1);                  /* obj_pit_zone */
    mff_cycles(0xA9F9, 8);
    y = 0x06;
    CALL_FIXED(0xA9FB, func_ECCF);              /* background probe */
    mff_cycles(0xA9FE, 7);
    a = RAM(WALK_PROBE) & TILE_PIT;
    if (a != 0) {
        mff_cycles(0xAA04, 38);
        RAM(OBJ_SAVED_ID + x) = RAM(OBJ_ID + x);
        RAM(OBJ_ID + x) = OBJ_PIT_FALL;
        RAM(OBJ_TIMER + x) = 0x00;
        a = 0xFF;
        RAM(OBJ_INVULN + x) = a;
        c = 1;
        RTS_RETURN();
    }
    mff_cycles(0xAA02, 1);
    mff_cycles(0xAA1B, 8);
    c = 0;
    RTS_RETURN();
}

/*
 * enemy_pit_beside, $AA1D (name proposed): looks for a pit around the
 * object at probe points 7 (0F:ECCF), 8 (0F:EDBA) and 9 (0F:ECCF). At 7:
 * obj_dir right, obj_pit_flag 2; at 8: left, 2; at 9: obj_pit_flag 1; each
 * then phase 1 (enemy_approach walks away from it). None: obj_pit_flag 0.
 * Right of x $03F0 (obj_pit_zone) it returns from its caller as well.
 * Callers: enemy_block (JMP $A20D), enemy_recover (JSR $A71F), enemy_react
 * (JSR $AB7F); obj_knockdown_recover (0C:9C35).
 */
void func_AA1D_b6(void)
{
    REGS_IN;

    mff_cycles(0xAA1D, 6);
    CALL_BANK(0xAA1D, 0x9BE1);                  /* obj_pit_zone */
    mff_cycles(0xAA20, 8);
    y = 0x07;
    CALL_FIXED(0xAA22, func_ECCF);
    mff_cycles(0xAA25, 7);
    a = RAM(WALK_PROBE) & TILE_PIT;
    if (a != 0) {
        mff_cycles(0xAA29, 1);
        mff_cycles(0xAA49, 2);
        a = DIR_RIGHT;
    } else {
        mff_cycles(0xAA2B, 8);
        y = 0x08;
        CALL_FIXED(0xAA2D, func_EDBA);
        mff_cycles(0xAA30, 7);
        a = RAM(WALK_PROBE) & TILE_PIT;
        if (a == 0) {
            mff_cycles(0xAA36, 8);
            y = 0x09;
            CALL_FIXED(0xAA38, func_ECCF);
            mff_cycles(0xAA3B, 7);
            a = RAM(WALK_PROBE) & TILE_PIT;
            if (a == 0) {
                mff_cycles(0xAA41, 11);
                RAM(OBJ_PIT_FLAG + x) = a;
                RTS_RETURN();
            }
            mff_cycles(0xAA3F, 1);
            mff_cycles(0xAA55, 7);
            RAM(OBJ_PIT_FLAG + x) = 0x01;
            goto approach;
        }
        mff_cycles(0xAA34, 1);
        mff_cycles(0xAA45, 4);
        a = DIR_LEFT;
        mff_cycles(0xAA47, 1);
    }
    mff_cycles(0xAA4B, 14);
    RAM(OBJ_DIR + x) = a;
    RAM(OBJ_PIT_FLAG + x) = 0x02;
    mff_cycles(0xAA53, 1);                      /* BNE, always taken */
approach:
    mff_cycles(0xAA5A, 13);
    a = AI_APPROACH;
    RAM(OBJ_STATE + x) = a;
    RTS_RETURN();
}

/*
 * enemy_anim_hurt, $AA60 (name proposed): facing the player, the kind's
 * hurt animation (0F:FC5B, sprite bank 0F:FC75). Bred has a second pose
 * ($94, bank $1D) for the player's animations $0D, $16, $0E, $17 and $12.
 * Callers: enemy_react (JSR $AB46 held by the knee, $AB9C hit).
 */
void func_AA60_b6(void)
{
    REGS_IN;

    mff_cycles(0xAA60, 6);
    CALL_FIXED(0xAA60, func_F343);              /* obj_face_player */
    mff_cycles(0xAA63, 8);
    y = RAM(OBJ_KIND + x);
    c = y >= KIND_BRED;
    if (y == KIND_BRED) {
        int low = 1;

        mff_cycles(0xAA68, 1);
        mff_cycles(0xAA7F, 8);
        a = RAM(OBJ_ANIM);                      /* the player's */
        c = a >= 0x0D;
        if (a == 0x0D) {
            mff_cycles(0xAA84, 1);
        } else {
            mff_cycles(0xAA86, 4);
            c = a >= 0x16;
            if (a == 0x16) {
                mff_cycles(0xAA88, 1);
            } else {
                mff_cycles(0xAA8A, 4);
                c = a >= 0x0E;
                if (a == 0x0E) {
                    mff_cycles(0xAA8C, 1);
                } else {
                    mff_cycles(0xAA8E, 4);
                    c = a >= 0x17;
                    if (a == 0x17) {
                        mff_cycles(0xAA90, 1);
                    } else {
                        mff_cycles(0xAA92, 4);
                        c = a >= 0x12;
                        if (a != 0x12) {
                            mff_cycles(0xAA6A, 1);
                            low = 0;
                        }
                    }
                }
            }
        }
        if (low) {
            mff_cycles(0xAA96, 8);
            a = BRED_LOW_HURT_ANIM;
            CALL_FIXED(0xAA98, func_F288);      /* obj_set_anim */
            mff_cycles(0xAA9B, 11);
            a = BRED_LOW_HURT_CHR;
            RAM(CHR_BANKS + 3) = a;
            RTS_RETURN();
        }
    }
    mff_cycles(0xAA6A, 4);
    a = prg_read((uint16_t)(KIND_HURT_ANIM + y));
    mff_cycles(0xAA6D, 6);
    CALL_FIXED(0xAA6D, func_F288);              /* obj_set_anim */
    mff_cycles(0xAA70, 4);
    a = prg_read((uint16_t)(KIND_HURT_CHR + y));
    mff_cycles(0xAA73, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(KIND_CHR_SLOT + y));
    mff_cycles(0xAA78, 16);
    y = a;
    a = RAM(TMP0);
    RAM(CHR_BANKS + 3 + y) = a;
    RTS_RETURN();
}

/*
 * enemy_hit_pause, $AAA0 (name proposed): obj_ai_timer = HIT_PAUSES[obj_id
 * - $20], the stand before the next hit of a combo.
 * Caller: JSR at $A390 (enemy_attack).
 */
void func_AAA0_b6(void)
{
    REGS_IN;

    mff_cycles(0xAAA0, 14);
    a = RAM(OBJ_ID + x);
    c = 1;
    SBC_A(ENEMY_ID_FIRST);
    y = a;
    a = prg_read((uint16_t)(HIT_PAUSES + y));
    mff_cycles(0xAAAA, 11);
    RAM(OBJ_AI_TIMER + x) = a;
    RTS_RETURN();
}

/*
 * $AAAE/$AAD7/$AB00: the kind's animation from `anims` unless it shows it
 * already (0F:F288; C as the CMP left it, or as obj_set_anim returns), and
 * its sprite bank from `chrs` into the kind's chr_banks+3 slot.
 */
static void set_kind_anim(uint16_t pc, uint16_t anims, uint16_t chrs)
{
    REGS_IN;

    mff_cycles(pc, 8);
    y = RAM(OBJ_KIND + x);
    a = prg_read((uint16_t)(anims + y));
    mff_cycles((uint16_t)(pc + 0x06), 6);
    c = a >= RAM(OBJ_ANIM + x);
    if (a != RAM(OBJ_ANIM + x)) {
        mff_cycles((uint16_t)(pc + 0x0B), 6);
        CALL_FIXED(pc + 0x0B, func_F288);       /* obj_set_anim */
    } else {
        mff_cycles((uint16_t)(pc + 0x09), 1);
    }
    mff_cycles((uint16_t)(pc + 0x0E), 4);
    a = prg_read((uint16_t)(chrs + y));
    mff_cycles((uint16_t)(pc + 0x11), 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(KIND_CHR_SLOT + y));
    mff_cycles((uint16_t)(pc + 0x16), 16);
    y = a;
    a = RAM(TMP0);
    RAM(CHR_BANKS + 3 + y) = a;
    RTS_RETURN();
}

/*
 * enemy_anim_stand, $AAAE (name proposed): the kind's standing animation.
 * Callers: most phases (enemy_start, enemy_close_in_wait, enemy_attack's
 * pause, ...), enemy_react; 0C:9EEE (the waiting ids).
 */
void func_AAAE_b6(void)
{
    set_kind_anim(0xAAAE, STAND_ANIMS, STAND_CHR);
}

/*
 * enemy_anim_walk, $AAD7 (name proposed): the kind's walking animation.
 * Out: C as described (its callers' callers read it).
 * Callers: enemy_approach (JSR $A065), enemy_set_speed (JSR $AC36).
 */
void func_AAD7_b6(void)
{
    set_kind_anim(0xAAD7, WALK_ANIMS, WALK_CHR);
}

/*
 * enemy_anim_attack, $AB00 (name proposed): the kind's attack animation.
 * Caller: JSR at $A2CE (enemy_attack).
 */
void func_AB00_b6(void)
{
    set_kind_anim(0xAB00, ATTACK_ANIMS, ATTACK_CHR);
}

/*
 * enemy_set_speed, $AC34 (name proposed): Y = a direction index. The walk
 * animation (enemy_anim_walk), then obj_vx/obj_vy for that direction at
 * the kind's speed: JMP ($0000) to SPEED_FN_LO/HI[kind], with Y back.
 * Callers: the phases before walking (enemy_walk_index, enemy_maneuver_pick,
 * enemy_retreat, enemy_walk_blocked, enemy_knife_throw, enemy_charge,
 * approach_avoid_pit).
 */
void func_AC34_b6(void)
{
    REGS_IN;

    mff_cycles(0xAC34, 9);
    RAM(TMP2) = y;
    CALL_BANK(0xAC36, 0xAAD7);                  /* enemy_anim_walk */
    mff_cycles(0xAC39, 8);
    y = RAM(OBJ_KIND + x);
    a = prg_read((uint16_t)(SPEED_FN_LO + y));
    mff_cycles(0xAC3F, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(SPEED_FN_HI + y));
    mff_cycles(0xAC44, 11);
    RAM(TMP1) = a;
    y = RAM(TMP2);
    JMP_INDIRECT(0x0000);
}

/*
 * The four setters of enemy_set_speed, Y = the direction index. The two
 * slow ones have speeds under a pixel (only fraction bytes); the fast ones
 * share the integer bytes of SPEED_HI_X/Y.
 */

/* enemy_speed_08, $AC4B (name proposed): 0.8 px a frame (Bred). */
void func_AC4B_b6(void)
{
    REGS_IN;

    mff_cycles(0xAC4B, 4);
    a = prg_read((uint16_t)(SPEED08_X + y));
    mff_cycles(0xAC4E, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = prg_read((uint16_t)(SPEED08_Y + y));
    mff_cycles(0xAC54, 23);
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x00;
    RAM(OBJ_VX + x) = a;
    RAM(OBJ_VY + x) = a;
    RTS_RETURN();
}

/* enemy_speed_09, $AC60 (name proposed): 0.9 px a frame (AxL, Andore). */
void func_AC60_b6(void)
{
    REGS_IN;

    mff_cycles(0xAC60, 4);
    a = prg_read((uint16_t)(SPEED09_X + y));
    mff_cycles(0xAC63, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = prg_read((uint16_t)(SPEED09_Y + y));
    mff_cycles(0xAC69, 23);
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x00;
    RAM(OBJ_VX + x) = a;
    RAM(OBJ_VY + x) = a;
    RTS_RETURN();
}

/* enemy_speed_12, $AC75 (name proposed): 1.2 px a frame (Poison). */
void func_AC75_b6(void)
{
    REGS_IN;

    mff_cycles(0xAC75, 4);
    a = prg_read((uint16_t)(SPEED12_X + y));
    mff_cycles(0xAC78, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = prg_read((uint16_t)(SPEED_HI_X + y));
    mff_cycles(0xAC7E, 9);
    RAM(OBJ_VX + x) = a;
    a = prg_read((uint16_t)(SPEED12_Y + y));
    mff_cycles(0xAC84, 9);
    RAM(OBJ_VY_FRAC + x) = a;
    a = prg_read((uint16_t)(SPEED_HI_Y + y));
    mff_cycles(0xAC8A, 11);
    RAM(OBJ_VY + x) = a;
    RTS_RETURN();
}

/* enemy_speed_14, $AC8E (name proposed): 1.4 px a frame (Two P, El Gado). */
void func_AC8E_b6(void)
{
    REGS_IN;

    mff_cycles(0xAC8E, 4);
    a = prg_read((uint16_t)(SPEED14_X + y));
    mff_cycles(0xAC91, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = prg_read((uint16_t)(SPEED_HI_X + y));
    mff_cycles(0xAC97, 9);
    RAM(OBJ_VX + x) = a;
    a = prg_read((uint16_t)(SPEED14_Y + y));
    mff_cycles(0xAC9D, 9);
    RAM(OBJ_VY_FRAC + x) = a;
    a = prg_read((uint16_t)(SPEED_HI_Y + y));
    mff_cycles(0xACA3, 11);
    RAM(OBJ_VY + x) = a;
    RTS_RETURN();
}

/*
 * enemy_walk_time, $ACB3 (name proposed): Y = a direction index. Out: A =
 * WALK_TIMES[kind * 16 + Y] (C and V of the ADC), Y kept.
 * Caller: JSR at $A185 (enemy_maneuver_pick).
 */
void func_ACB3_b6(void)
{
    REGS_IN;

    mff_cycles(0xACB3, 26);
    RAM(TMP15) = y;
    a = (uint8_t)(RAM(OBJ_KIND + x) << 4);
    c = 0;
    ADC_A(RAM(TMP15));
    y = a;
    a = prg_read((uint16_t)(WALK_TIMES + y));
    mff_cycles(0xACC3, 9);
    y = RAM(TMP15);
    RTS_RETURN();
}
