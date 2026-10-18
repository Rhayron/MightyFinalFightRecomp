/*
 * boss_b01_shared.c - bank $01's entry table and the helpers its bosses
 * share: the move by obj_dir with the arena's top and bottom lines, facing
 * the player, the speed tables of the sixteen directions, the aim of the
 * rush, the animations by kind, and the check that runs before every
 * frame of Thrasher and Sodom (held by the player, hit, the player
 * dropping in). boss_b01.h says who the ids are.
 */
#include "boss_b01.h"

/* $A08E: the top line by obj_tier != 0 (the higher one for id $83) */
#define TOP_LINE            0xA08E

/* the speed tables, by direction index (0 up, 4 right, 8 down, $0C left):
 * obj_vx_frac, obj_vx, obj_vy_frac, obj_vy, unsigned (obj_dir gives the
 * signs). $A7E1: $0133 along an axis (1.2 px a frame), $A918: $024C,
 * $A971: $0300. */
#define SPEED1_VX_FRAC      0xA80A
#define SPEED1_VX           0xA7FA
#define SPEED1_VY_FRAC      0xA82A
#define SPEED1_VY           0xA81A
#define SPEED2_VX_FRAC      0xA941
#define SPEED2_VX           0xA931
#define SPEED2_VY_FRAC      0xA961
#define SPEED2_VY           0xA951
#define SPEED3_VX_FRAC      0xA99A
#define SPEED3_VX           0xA98A
#define SPEED3_VY_FRAC      0xA9BA
#define SPEED3_VY           0xA9AA

/* the animations by obj_kind - 7 (Thrasher, Sodom) and their CHR banks:
 * chr_banks+3+$A914[k] gets the first, chr_banks+3+$A916[$A914[k]] the
 * second - $A916 is indexed with the value read from $A914, which is 1 for
 * both, so the banks always land in chr_banks+4 and chr_banks+5 */
#define ANIM_STANCE         0xA870  /* $E4 $F0, banks $A872/$A874 */
#define ANIM_WALK           0xA8B1  /* $E5 $F2, banks $A8B3/$A8B5 */
#define ANIM_SWING          0xA8E9  /* $E9 $F5, banks $A8EB/$A8ED */
#define ANIM_HURT           0xA910  /* $EA $F7, bank $A912 */
#define CHR_SLOT_A          0xA914
#define CHR_SLOT_B          0xA916

/* the rush's aim: the direction index the steep ones turn into */
#define AIM_CLAMP           0xAA23

/* id $8F's routine is in boss_b01_scene.c; the others jump here */
static void entry_jmp(uint16_t pc, uint16_t target)
{
    mff_jmp_charge(pc, target);
    call_by_address_tail(target, 0);
}

/*
 * The JMP table objects_update enters at $A000 + 3 * (id & $0F)
 * (obj_behavior_lo/hi, 0C:83AF/844F, point every id $80-$8F here).
 */
void func_A000_b0(void) { entry_jmp(0xA000, 0xA0FD); }     /* $80 Thrasher */
void func_A003_b0(void) { entry_jmp(0xA003, 0xA101); }     /* $81 Sodom (round 2) */
void func_A006_b0(void) { entry_jmp(0xA006, 0xA030); }     /* $82 nothing */
void func_A009_b0(void) { entry_jmp(0xA009, 0xAA33); }     /* $83 Sodom (rounds 4-5) */
void func_A00C_b0(void) { entry_jmp(0xA00C, 0xA030); }     /* $84-$8E nothing */
void func_A00F_b0(void) { entry_jmp(0xA00F, 0xA030); }
void func_A012_b0(void) { entry_jmp(0xA012, 0xA030); }
void func_A015_b0(void) { entry_jmp(0xA015, 0xA030); }
void func_A018_b0(void) { entry_jmp(0xA018, 0xA030); }
void func_A01B_b0(void) { entry_jmp(0xA01B, 0xA030); }
void func_A01E_b0(void) { entry_jmp(0xA01E, 0xA030); }
void func_A021_b0(void) { entry_jmp(0xA021, 0xA030); }
void func_A024_b0(void) { entry_jmp(0xA024, 0xA030); }
void func_A027_b0(void) { entry_jmp(0xA027, 0xA030); }
void func_A02A_b0(void) { entry_jmp(0xA02A, 0xA030); }
void func_A02D_b0(void) { entry_jmp(0xA02D, 0xA09B); }     /* $8F the scene prop */

/* $A030: RTS, the behaviour of the unused ids */
void func_A030_b0(void)
{
    mff_cycles(0xA030, 6);
    mff_rts();
}

/*
 * boss_move, $A031 (name proposed): moves slot X by obj_vx/obj_vy the way
 * obj_dir points. Up (bit 3, 0F:F1DF) stops at the top line ($85, or $82
 * when obj_tier is set), down (bit 2, 0F:F1BF) at $AE; MOVE_BLOCKED ($0F)
 * is 8 or 4 when it stopped there, else 0. Then right (bit 0) or left
 * (bit 1) through a JMP to 0F:F183 / F1A1. Y is kept (in $00).
 * In: X = slot. Out: TMP15; as 0F:F183/F1A1, or A = 0.
 * Callers: the walks and rushes of both behaviours (01:A23D, A45D, A49D,
 * AB74, AB8D, ABB0, AC2D, AD16; the JMP at 01:A526).
 */
void func_A031_b0(void)
{
    REGS_IN;

    mff_cycles(0xA031, 9);
    RAM(TMP0) = y;
    a = RAM(OBJ_TIER + x);
    if (a != 0) {
        mff_cycles(0xA038, 2);
        a = 0x01;
    } else {
        mff_cycles(0xA036, 1);
    }
    mff_cycles(0xA03A, 15);
    y = a;
    a = 0x00;
    RAM(MOVE_BLOCKED) = a;
    a = RAM(OBJ_DIR + x) & DIR_UP;
    SET_NZ(a);
    if (!z) {
        mff_cycles(0xA046, 6);
        CALL_FIXED(0xA046, func_F1DF);
        mff_cycles(0xA049, 8);
        a = RAM(OBJ_Y + x);
        m = prg_read_ix(TOP_LINE, y);
        c = a >= m;
        mff_cycles(0xA04F, 2);
        if (!c) {
            mff_cycles(0xA051, 4);
            a = prg_read_ix(TOP_LINE, y);
            mff_cycles(0xA054, 12);
            RAM(OBJ_Y + x) = a;
            a = DIR_UP;
            RAM(MOVE_BLOCKED) = a;
            mff_cycles(0xA05B, 1);              /* BNE, always taken */
        } else {
            mff_cycles(0xA04F, 1);
        }
    } else {
        mff_cycles(0xA044, 1);
        mff_cycles(0xA05D, 8);
        a = RAM(OBJ_DIR + x) & DIR_DOWN;
        SET_NZ(a);
        if (!z) {
            mff_cycles(0xA064, 6);
            CALL_FIXED(0xA064, func_F1BF);
            mff_cycles(0xA067, 8);
            a = RAM(OBJ_Y + x);
            c = a >= ARENA_BOTTOM + 1;
            if (c) {
                mff_cycles(0xA06E, 12);
                a = ARENA_BOTTOM;
                RAM(OBJ_Y + x) = a;
                a = DIR_DOWN;
                RAM(MOVE_BLOCKED) = a;
            } else {
                mff_cycles(0xA06C, 1);
            }
        } else {
            mff_cycles(0xA062, 1);
        }
    }

    mff_cycles(0xA077, 11);
    y = RAM(TMP0);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    SET_NZ(a);
    if (!z) {
        mff_cycles(0xA080, 3);
        JMP_F(0xF183, func_F183);
    }
    mff_cycles(0xA07E, 1);
    mff_cycles(0xA083, 8);
    a = RAM(OBJ_DIR + x) & DIR_LEFT;
    SET_NZ(a);
    if (!z) {
        mff_cycles(0xA08A, 3);
        JMP_F(0xF1A1, func_F1A1);
    }
    mff_cycles(0xA088, 1);
    mff_cycles(0xA08D, 6);
    RTS_RETURN();
}

/*
 * boss_face_player, $A090 (name proposed): the sprite faces the player:
 * 0F:F374 (C set when the player is to the right), then JMP 0F:F090 (face
 * right) or 0F:F099 (face left). obj_dir is left alone.
 * In: X = slot. Out: as 0F:F090/F099.
 */
void func_A090_b0(void)
{
    REGS_IN;

    mff_cycles(0xA090, 6);
    CALL_FIXED(0xA090, func_F374);
    mff_cycles(0xA093, 2);
    if (!c) {
        mff_cycles(0xA095, 3);
        JMP_F(0xF099, func_F099);
    }
    mff_cycles(0xA093, 1);
    mff_cycles(0xA098, 3);
    JMP_F(0xF090, func_F090);
}

/* ---- before every frame: held, hit, the drop-in ---- */

static void boss_hit_reaction(void);
static void boss_action_check(void);
static void boss_down_check(void);
static void boss_knocked_down(void);

/*
 * boss_check, $A6D6 (name proposed): the part of a boss's frame that comes
 * before its phase; C set means it took the frame over. In order:
 *  - held by the player (obj_held, 0E:CE8D): while the grab of this slot
 *    goes on (game_action 4, or $21 - that test never executed) the stance
 *    animation and phase 0 (the knee, $21, would show the hurt one:
 *    never executed); C set.
 *  - just let go (obj_held $FF): cleared; after Haggar's hold the boss is
 *    put on the player's line.
 *  - obj_hit_timer, $1E when a player hit lands (0F:FB13): the hit
 *    reaction ($A71C).
 *  - otherwise the player's action ($A74C).
 * In: X = slot. Out: C, A.
 * Callers: 01:A11D (ids $80/$81), 01:AA63 (id $83).
 */
void func_A6D6_b0(void)
{
    REGS_IN;

    mff_cycles(0xA6D6, 6);
    a = RAM(OBJ_HELD + x);
    if (a == 0) {
        mff_cycles(0xA6D9, 2);
        RUN_ON(boss_hit_reaction);
    }
    mff_cycles(0xA6DB, 4);
    if (a == 0xFF) {
        mff_cycles(0xA6DD, 2);
        mff_cycles(0xA70B, 14);
        a = 0x00;
        RAM(OBJ_HELD + x) = a;
        a = RAM(PLAYER_ID);
        if (a == PLAYER_HAGGAR) {
            mff_cycles(0xA716, 9);
            a = RAM(OBJ_Y);
            RAM(OBJ_Y + x) = a;
        } else {
            mff_cycles(0xA714, 1);
        }
        RUN_ON(boss_hit_reaction);
    }

    mff_cycles(0xA6DF, 7);
    a = RAM(GAME_ACTION);
    if (a == ACT_GRAB) {
        mff_cycles(0xA6E3, 1);
    } else {
        mff_cycles(0xA6E5, 4);
        if (a != ACT_GRAB_KNEE) {
            mff_cycles(0xA6E7, 2);
            RUN_ON(boss_hit_reaction);
        }
    }
    mff_cycles(0xA6E9, 5);
    if (x != RAM(GRAB_SLOT)) {
        mff_cycles(0xA6EB, 2);
        RUN_ON(boss_hit_reaction);
    }
    mff_cycles(0xA6ED, 7);
    a = RAM(GAME_ACTION);
    CMP_FLAGS(a, ACT_GRAB_KNEE);
    if (z) {
        mff_cycles(0xA6F3, 6);
        CALL_B01(0xA6F3, 0xA8EF);
        mff_cycles(0xA6F6, 3);
        JMP_B01(0xA6FC);
    }
    mff_cycles(0xA6F1, 1);
    mff_cycles(0xA6F9, 6);
    CALL_B01(0xA6F9, 0xA83A);
    RUN_ON(func_A6FC_b0);
}

/*
 * $A6FC: held: faces the player (0F:F343), phase 0 (obj_state $80),
 * obj_hit_timer cleared, C set.
 * Callers: the run-on from boss_check's grab ($A6F9); the JMP at 01:A6F6.
 */
void func_A6FC_b0(void)
{
    REGS_IN;

    mff_cycles(0xA6FC, 6);
    CALL_FIXED(0xA6FC, func_F343);
    mff_cycles(0xA6FF, 22);
    a = OBJ_LIVE;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    SET_NZ(a);
    RAM(OBJ_HIT_TIMER + x) = a;
    c = 1;
    RTS_RETURN();
}

/*
 * $A71C: obj_hit_timer counts the frames of a hit (the behaviour's own
 * phase counts it down, $A633/$AD8C): $1E (the hit lands) and $10 show
 * the stance, $1A the hurt animation; then the hurt phase ($A736). 0 goes
 * on to the player's action.
 */
static void boss_hit_reaction(void)
{
    REGS_IN;

    mff_cycles(0xA71C, 6);
    a = RAM(OBJ_HIT_TIMER + x);
    if (a == 0) {
        mff_cycles(0xA71F, 1);
        RUN_ON(boss_action_check);
    }
    mff_cycles(0xA721, 4);
    CMP_FLAGS(a, 0x1E);
    if (!z) {
        mff_cycles(0xA725, 4);
        CMP_FLAGS(a, 0x1A);
        if (z) {
            mff_cycles(0xA727, 1);
            mff_cycles(0xA733, 6);
            CALL_B01(0xA733, 0xA8EF);
            RUN_ON(func_A736_b0);
        }
        mff_cycles(0xA729, 4);
        CMP_FLAGS(a, 0x10);
        if (!z) {
            mff_cycles(0xA72B, 1);
            RUN_ON(func_A736_b0);
        }
    } else {
        mff_cycles(0xA723, 1);
    }
    mff_cycles(0xA72D, 6);
    CALL_B01(0xA72D, 0xA83A);
    mff_cycles(0xA730, 3);
    JMP_B01(0xA736);
}

/*
 * $A736: into the hurt phase: faces the player ($A090), obj_state $87;
 * at obj_tier 2 (id $83 below $30 life) obj_invuln $20. C clear.
 * Callers: boss_hit_reaction's run-ons and the JMP at 01:A730.
 */
void func_A736_b0(void)
{
    REGS_IN;

    mff_cycles(0xA736, 6);
    CALL_B01(0xA736, 0xA090);
    mff_cycles(0xA739, 15);
    a = TS_HURT;
    RAM(OBJ_STATE + x) = a;
    a = RAM(OBJ_TIER + x);
    CMP_FLAGS(a, SR_TIER_HURT);
    if (z) {
        mff_cycles(0xA745, 7);
        a = 0x20;
        SET_NZ(a);
        RAM(OBJ_INVULN + x) = a;
    } else {
        mff_cycles(0xA743, 1);
    }
    mff_cycles(0xA74A, 8);
    c = 0;
    RTS_RETURN();
}

/*
 * $A74C: game_action $0A (a bank-$03 boss holds the player) or $19: the
 * stance, C set (never executed). Cody's throw ($0B): obj_hit_player cleared, C clear.
 * Else $A766.
 */
static void boss_action_check(void)
{
    REGS_IN;

    mff_cycles(0xA74C, 7);
    a = RAM(GAME_ACTION);
    CMP_FLAGS(a, ACT_HELD_A);
    if (!z) {
        mff_cycles(0xA752, 4);
        CMP_FLAGS(a, ACT_CODY_THROW_B);
        if (z) {
            mff_cycles(0xA754, 1);
            mff_cycles(0xA75F, 15);
            a = 0x00;
            SET_NZ(a);
            RAM(OBJ_HIT_PLAYER + x) = a;
            c = 0;
            RTS_RETURN();
        }
        mff_cycles(0xA756, 4);
        CMP_FLAGS(a, ACT_HELD_19);
        if (!z) {
            mff_cycles(0xA758, 1);
            RUN_ON(boss_down_check);
        }
    } else {
        mff_cycles(0xA750, 1);
    }
    /* never executed */
    mff_cycles(0xA75A, 6);
    CALL_B01(0xA75A, 0xA83A);
    mff_cycles(0xA75D, 8);
    c = 1;
    RTS_RETURN();
}

/*
 * $A766: Thrasher and Sodom of round 2 (obj_tier 0), when the player lies
 * on the ground ($1F) and they are not circling (phase 1) or jumping
 * (phases 5, 6), go back to phase 0 with BOSS_TRIED 7; C clear. A new
 * life dropping in (game_action $25) knocks any of them down: C set.
 */
static void boss_down_check(void)
{
    REGS_IN;

    mff_cycles(0xA766, 6);
    a = RAM(OBJ_TIER + x);
    if (a == 0) {
        mff_cycles(0xA76B, 8);
        a = RAM(OBJ_STATE + x);
        if (a != TS_CIRCLE) {
            mff_cycles(0xA772, 7);
            a = RAM(GAME_ACTION);
            if (a == ACT_LYING) {
                mff_cycles(0xA778, 8);
                a = RAM(OBJ_STATE + x);
                CMP_FLAGS(a, TS_JUMP);
                if (!z) {
                    mff_cycles(0xA77F, 4);
                    CMP_FLAGS(a, TS_LEAP);
                    if (!z) {
                        mff_cycles(0xA783, 21);
                        a = 0x07;
                        RAM(BOSS_TRIED + x) = a;
                        a = TS_ENTER;
                        RAM(OBJ_STATE + x) = a;
                        a = 0x00;
                        SET_NZ(a);
                        RAM(OBJ_TIMER + x) = a;
                    } else {
                        mff_cycles(0xA781, 1);
                    }
                } else {
                    mff_cycles(0xA77D, 1);
                }
                mff_cycles(0xA792, 8);
                c = 0;
                RTS_RETURN();
            }
            mff_cycles(0xA776, 1);
        } else {
            mff_cycles(0xA770, 1);
        }
    } else {
        mff_cycles(0xA769, 1);
    }

    mff_cycles(0xA794, 7);
    a = RAM(GAME_ACTION);
    CMP_FLAGS(a, ACT_DROP_IN);
    if (!z) {
        mff_cycles(0xA79A, 8);
        c = 0;
        RTS_RETURN();
    }
    mff_cycles(0xA798, 1);
    mff_cycles(0xA79C, 6);
    a = RAM(OBJ_TIER + x);
    if (a == 0) {
        mff_cycles(0xA7A1, 8);
        a = RAM(OBJ_STATE + x);
        CMP_FLAGS(a, TS_JUMP);
        if (a != TS_JUMP) {
            mff_cycles(0xA7A8, 4);
            CMP_FLAGS(a, TS_LEAP);
        } else {
            mff_cycles(0xA7A6, 1);
        }
        if (a == TS_JUMP || a == TS_LEAP) {
            /* in the air: lands on the ground line (never executed) */
            mff_cycles(0xA7AC, 12);
            a = RAM(OBJ_GROUND_Y + x);
            SET_NZ(a);
            RAM(OBJ_LAND_Y + x) = a;
            JMP_B01(0xA7BB);
        }
        mff_cycles(0xA7AA, 1);
    } else {
        mff_cycles(0xA79F, 1);
    }
    mff_cycles(0xA7B5, 68);
    a = RAM(OBJ_Y + x);
    RAM(OBJ_LAND_Y + x) = a;
    RUN_ON(boss_knocked_down);
}

/* $A7BB's work, its cycles charged by the caller: knocked down like a
 * common enemy - id 5 (0C:983B) with the boss's id saved to come back to,
 * obj_vy $038E, phase 0, obj_invuln $FF; C set */
static void boss_knocked_down(void)
{
    REGS_IN;

    a = KNOCK_VY_FRAC;
    RAM(OBJ_VY_FRAC + x) = a;
    a = KNOCK_VY;
    RAM(OBJ_VY + x) = a;
    a = RAM(OBJ_ID + x);
    RAM(OBJ_SAVED_ID + x) = a;
    a = OBJ_KNOCKED_DOWN;
    RAM(OBJ_ID + x) = a;
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = OBJ_LIVE;
    RAM(OBJ_STATE + x) = a;
    a = 0xFF;
    SET_NZ(a);
    RAM(OBJ_INVULN + x) = a;
    c = 1;
    RTS_RETURN();
}

/*
 * $A7BB: the knockdown of the drop-in, landing on obj_land_y.
 * Caller: the JMP at 01:A7B2 (never executed); boss_down_check runs the
 * same code on from $A7B5.
 */
void func_A7BB_b0(void)
{
    mff_cycles(0xA7BB, 59);
    boss_knocked_down();
}

/* ---- speeds ---- */

/* obj_vx_frac/vx/vy_frac/vy from four tables at index Y (the copy at `pc`) */
static void set_speed(uint16_t pc, uint16_t vx_frac, uint16_t vx, uint16_t vy_frac, uint16_t vy)
{
    REGS_IN;

    mff_cycles(pc, 4);
    a = prg_read_ix(vx_frac, y);
    mff_cycles((uint16_t)(pc + 3), 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = prg_read_ix(vx, y);
    mff_cycles((uint16_t)(pc + 9), 9);
    RAM(OBJ_VX + x) = a;
    a = prg_read_ix(vy_frac, y);
    mff_cycles((uint16_t)(pc + 15), 9);
    RAM(OBJ_VY_FRAC + x) = a;
    a = prg_read_ix(vy, y);
    SET_NZ(a);
    mff_cycles((uint16_t)(pc + 21), 11);
    RAM(OBJ_VY + x) = a;
    RTS_RETURN();
}

/*
 * boss_speed_walk, $A7E1 / boss_speed_rush, $A918 / boss_speed_charge,
 * $A971 (names proposed): the speed for direction index Y, 1.2, 2.3 or 3
 * px a frame along an axis ($0133, $024C, $0300), the components of a
 * circle in between. In: X = slot, Y = index. Out: A = obj_vy.
 */
void func_A7E1_b0(void) { set_speed(0xA7E1, SPEED1_VX_FRAC, SPEED1_VX, SPEED1_VY_FRAC, SPEED1_VY); }
void func_A918_b0(void) { set_speed(0xA918, SPEED2_VX_FRAC, SPEED2_VX, SPEED2_VY_FRAC, SPEED2_VY); }
void func_A971_b0(void) { set_speed(0xA971, SPEED3_VX_FRAC, SPEED3_VX, SPEED3_VY_FRAC, SPEED3_VY); }

/* ---- animations ---- */

/*
 * The animation `anims`[k] (k = obj_kind - 7) unless it is on already:
 * 0F:F288 starts it, and chr_banks+4/+5 get `chr_a`[k] and `chr_b`[k]
 * (through CHR_SLOT_A/B). The copy at `pc` (01:A841, A882).
 */
static void set_anim(uint16_t pc, uint16_t anims, uint16_t chr_a, uint16_t chr_b)
{
    REGS_IN;

    mff_cycles(pc, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    SBC_A(KIND_THRASHER);
    y = a;
    a = prg_read_ix(anims, y);
    mff_cycles((uint16_t)(pc + 0x0A), 6);
    CMP_FLAGS(a, RAM(OBJ_ANIM + x));
    if (!z) {
        mff_cycles((uint16_t)(pc + 0x0F), 6);
        CALL_FIXED((uint16_t)(pc + 0x0F), func_F288);
        mff_cycles((uint16_t)(pc + 0x12), 4);
        a = prg_read_ix(chr_a, y);
        mff_cycles((uint16_t)(pc + 0x15), 7);
        RAM(TMP0) = a;
        a = prg_read_ix(chr_b, y);
        mff_cycles((uint16_t)(pc + 0x1A), 7);
        RAM(TMP1) = a;
        a = prg_read_ix(CHR_SLOT_A, y);
        mff_cycles((uint16_t)(pc + 0x1F), 14);
        y = a;
        a = RAM(TMP0);
        RAM(CHR_BANKS + 3 + y) = a;
        a = prg_read_ix(CHR_SLOT_B, y);
        mff_cycles((uint16_t)(pc + 0x28), 10);
        y = a;
        a = RAM(TMP1);
        SET_NZ(a);
        RAM(CHR_BANKS + 3 + y) = a;
    } else {
        mff_cycles((uint16_t)(pc + 0x0D), 1);
    }
    mff_cycles((uint16_t)(pc + 0x2E), 6);
    RTS_RETURN();
}

/*
 * boss_anim_stance, $A83A (name proposed): the standing animation, $E4
 * (Thrasher) or $F0 (Sodom). From obj_tier 2 on (id $83 hurt or enraged)
 * it is $EF with CHR bank $2C (0F:F449) and chr_banks+5 = $2E.
 * In: X = slot.
 */
void func_A83A_b0(void)
{
    REGS_IN;

    mff_cycles(0xA83A, 8);
    a = RAM(OBJ_TIER + x);
    c = a >= SR_TIER_HURT;
    if (!c) {
        SYNC_OUT;
        set_anim(0xA841, ANIM_STANCE, 0xA872, 0xA874);
        return;
    }
    mff_cycles(0xA83F, 1);
    mff_cycles(0xA876, 10);
    a = 0xEF;
    y = 0x2C;
    SET_NZ(y);
    CALL_FIXED(0xA87A, func_F449);
    mff_cycles(0xA87D, 11);
    a = 0x2E;
    SET_NZ(a);
    RAM(CHR_BANKS + 5) = a;
    RTS_RETURN();
}

/* boss_anim_walk, $A882 (name proposed): $E5 / $F2. In: X = slot. */
void func_A882_b0(void)
{
    set_anim(0xA882, ANIM_WALK, 0xA8B3, 0xA8B5);
}

/*
 * boss_anim_swing, $A8B7 (name proposed): $E9 / $F5, the swing of phase 2;
 * when it starts, obj_anim_timer is set ahead by one.
 * In: X = slot.
 */
void func_A8B7_b0(void)
{
    REGS_IN;

    mff_cycles(0xA8B7, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    SBC_A(KIND_THRASHER);
    y = a;
    a = prg_read_ix(ANIM_SWING, y);
    mff_cycles(0xA8C1, 6);
    CMP_FLAGS(a, RAM(OBJ_ANIM + x));
    if (!z) {
        mff_cycles(0xA8C6, 6);
        CALL_FIXED(0xA8C6, func_F288);
        mff_cycles(0xA8C9, 4);
        a = prg_read_ix(0xA8EB, y);
        mff_cycles(0xA8CC, 7);
        RAM(TMP0) = a;
        a = prg_read_ix(0xA8ED, y);
        mff_cycles(0xA8D1, 7);
        RAM(TMP1) = a;
        a = prg_read_ix(CHR_SLOT_A, y);
        mff_cycles(0xA8D6, 14);
        y = a;
        a = RAM(TMP0);
        RAM(CHR_BANKS + 3 + y) = a;
        a = prg_read_ix(CHR_SLOT_B, y);
        mff_cycles(0xA8DF, 17);
        y = a;
        a = RAM(TMP1);
        RAM(CHR_BANKS + 3 + y) = a;
        t = (uint8_t)(RAM(OBJ_ANIM_TIMER + x) + 1);
        RAM(OBJ_ANIM_TIMER + x) = t;
        SET_NZ(t);
    } else {
        mff_cycles(0xA8C4, 1);
    }
    mff_cycles(0xA8E8, 6);
    RTS_RETURN();
}

/* boss_anim_hurt, $A8EF (name proposed): $EA / $F7, one CHR bank ($2B /
 * $2D into chr_banks+4). In: X = slot. */
void func_A8EF_b0(void)
{
    REGS_IN;

    mff_cycles(0xA8EF, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    SBC_A(KIND_THRASHER);
    y = a;
    a = prg_read_ix(ANIM_HURT, y);
    mff_cycles(0xA8F9, 6);
    CMP_FLAGS(a, RAM(OBJ_ANIM + x));
    if (!z) {
        mff_cycles(0xA8FE, 6);
        CALL_FIXED(0xA8FE, func_F288);
        mff_cycles(0xA901, 4);
        a = prg_read_ix(0xA912, y);
        mff_cycles(0xA904, 7);
        RAM(TMP0) = a;
        a = prg_read_ix(CHR_SLOT_A, y);
        mff_cycles(0xA909, 10);
        y = a;
        a = RAM(TMP0);
        SET_NZ(a);
        RAM(CHR_BANKS + 3 + y) = a;
    } else {
        mff_cycles(0xA8FC, 2);
    }
    mff_cycles(0xA90F, 6);
    RTS_RETURN();
}

/* ---- the rush ---- */

/*
 * boss_aim, $A9CA (name proposed): obj_dir toward the player's ground
 * point, never steeper than the diagonals. 0F:FB5B gives the direction
 * index; straight up (0) becomes 2, straight down (8) 6 (the DEY DEY of
 * the other case never executed), then mirrored ((-i) & $0F) when the
 * player is to the left; any other index goes through AIM_CLAMP (1-3 to
 * 2-3, 5-7 to 5-6, the left half alike). obj_dir = 0F:FBF0[index].
 * In: X = slot. Out: Y = the index, A = obj_dir.
 * Callers: 01:A488 (the rush), 01:AC61 (the enraged charge).
 */
void func_A9CA_b0(void)
{
    REGS_IN;

    mff_cycles(0xA9CA, 6);
    CALL_FIXED(0xA9CA, func_FB5B);
    mff_cycles(0xA9CD, 6);
    y = a;
    a &= 0x07;
    if (a != 0) {
        mff_cycles(0xA9D0, 1);
        mff_cycles(0xA9EA, 4);
        a = prg_read_ix(AIM_CLAMP, y);
        mff_cycles(0xA9ED, 2);
        y = a;
    } else {
        mff_cycles(0xA9D2, 4);
        a = y;
        if (a == 0) {
            mff_cycles(0xA9D5, 6);
            y = (uint8_t)(y + 2);
            SET_NZ(y);
            mff_cycles(0xA9D7, 1);
        } else {
            mff_cycles(0xA9D3, 1);
            mff_cycles(0xA9D9, 4);
            y = (uint8_t)(y - 2);
            SET_NZ(y);
        }
        mff_cycles(0xA9DB, 6);
        CALL_FIXED(0xA9DB, func_F374);
        mff_cycles(0xA9DE, 2);
        if (c) {
            mff_cycles(0xA9DE, 1);
        } else {
            /* the player is to the left: mirror the index */
            mff_cycles(0xA9E0, 12);
            a = y ^ 0xFF;
            ADC_A(0x01);
            a &= 0x0F;
            y = a;
            if (y != 0) {
                mff_cycles(0xA9E8, 1);
            } else {
                mff_cycles(0xA9EA, 4);
                a = prg_read_ix(AIM_CLAMP, y);
                mff_cycles(0xA9ED, 2);
                y = a;
            }
        }
    }
    mff_cycles(0xA9EE, 4);
    a = prg_read_ix(DIR_PAD_BITS, y);
    SET_NZ(a);
    mff_cycles(0xA9F1, 11);
    RAM(OBJ_DIR + x) = a;
    RTS_RETURN();
}

/*
 * boss_rush_speed, $A9F5 (name proposed): the rush's speed for direction
 * index Y ($A918) and how fast it dies away, obj_timer2 from obj_vx_frac
 * and obj_ground_y from obj_vy_frac each frame ($A4F8): $12/7 for the odd
 * indexes, $0E/$0E for 2, 6, 10 and 14, $14/0 for 4 and 12.
 * In: X = slot, Y = index. Out: Y kept; C = bit 1 of an even index.
 */
void func_A9F5_b0(void)
{
    REGS_IN;

    mff_cycles(0xA9F5, 6);
    CALL_B01(0xA9F5, 0xA918);
    mff_cycles(0xA9F8, 6);
    a = y & 0x01;
    if (a != 0) {
        mff_cycles(0xA9FD, 16);
        a = 0x12;
        RAM(OBJ_TIMER2 + x) = a;
        a = 0x07;
        SET_NZ(a);
        RAM(OBJ_GROUND_Y + x) = a;
        mff_cycles(0xAA07, 1);
    } else {
        mff_cycles(0xA9FB, 2);
        mff_cycles(0xAA09, 8);
        a = (uint8_t)(y >> 1);
        c = a & 1;
        a >>= 1;
        if (c) {
            mff_cycles(0xAA0E, 14);
            a = 0x0E;
            SET_NZ(a);
            RAM(OBJ_TIMER2 + x) = a;
            RAM(OBJ_GROUND_Y + x) = a;
            mff_cycles(0xAA16, 1);
        } else {
            mff_cycles(0xAA0C, 1);
            mff_cycles(0xAA18, 14);
            a = 0x14;
            RAM(OBJ_TIMER2 + x) = a;
            a = 0x00;
            SET_NZ(a);
            RAM(OBJ_GROUND_Y + x) = a;
        }
    }
    mff_cycles(0xAA22, 6);
    RTS_RETURN();
}
