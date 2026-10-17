/*
 * enemy_entrances.c - the ways a common enemy enters (ids $2C, $2D, $2F
 * and $36-$5F): each id sets the enemy up, waits if it has to, and turns
 * into the plain id of its kind and tier ($20-$35, enemy_common.c), at
 * the phase it starts in. docs/objects.md, "The common enemies".
 *
 *   $2C $2D $2F  Andore charging in (andore_charge_in), tiers 1/2/4;
 *                never executed, and no spawn table lists them
 *   $36-$3E      dropping onto the player (enemy_drop_in): Poison
 *                $36-$38, AxL $39-$3B, El Gado $3C-$3E, tiers 1/2/4
 *   $3F, $4F-$5F standing a moment first (enemy_stand_in), all six kinds
 *   $40-$4B      sitting in wait (enemy_sit_in): Two P $40-$42 and
 *                $46-$48, Bred $43-$45 and $49-$4B, two poses
 *   $4C-$4E      El Gado by $9F91/$9FCC, but their behaviour is the plain
 *                RTS (0C:84EF): an object with them stays as it is
 *
 * The stage object lists make them hidden (types $30-$35 and $5A-$5C:
 * obj_flags $D4, the drop-ins) or not (the others, types $3C-$68 of
 * 08:8442); the timed spawner adds $37, $3A and $3D (08:9456). Waiting at
 * the end of a frame in the routes' logs: $38 (round 2 area 1, round 4
 * area 0), $3E (round 3 area 2), $43 (round 0 area 0, every route), $49
 * (round 0 area 0), $4A (round 1 area 1), $4B (round 3 areas 0 and 2);
 * the standing ones become their plain id in their first frame. Never
 * executed (runs/cdl): $2C, $2D, $2F, $36, $39, $3A, $3C, the tier-2
 * standing entry ($9EC9) and $41/$44. Rounds and areas are the 0-based
 * round/sub_area.
 */
#include "enemy_defs.h"

/* Andore's charge: obj_timer2 holds the plain id meanwhile */
#define ANIM_ANDORE_CHARGE  0xD2
#define CHR_ANDORE          0x24
#define CHARGE_DAMAGE       0x33    /* + the tier index: obj_attack_damage */
#define CHARGE_DIR          0xA87E  /* bank $0D: direction index (0F:FB5B) -> obj_dir */
#define CHARGE_STEP         0x03    /* obj_timer: the rush step of phase $E */

/* the drop-in: by kind (Poison, AxL, El Gado; the others 0) the falling
 * animation, its sprite bank and the attack's damage index (+ tier) */
#define DROP_ANIM           0x9EB3
#define DROP_CHR            0x9EB9
#define DROP_DAMAGE         0x9EBF
#define DROP_POISON_FRAME   0x06    /* Poison's fall starts at frame 6 of $84 */
#define DROP_REACH          0x08    /* falls once the player is less than 8 px away in x */
#define OBJ_HIDDEN          0x04    /* obj_flags bit 2 */

/* standing: obj_ai_timer frames before the first move */
#define STAND_FRAMES        0x78

/* sitting: animation $A2, frame kind - 1 + pose (Two P 0/2, Bred 1/3),
 * the frame's sprite bank; wakes when the player is within $32 px both
 * ways (0F:FB3E) or the enemy is less than $40 px from the screen's left */
#define ANIM_SITTING        0xA2
#define SIT_CHR             0x9F8D
#define SIT_WAKE_REACH      0x32
#define SIT_WAKE_EDGE       0x40

void func_9FBB_b6(void);

/* ======================= Andore charging in, ids $2C $2D $2F ======================= */

/*
 * andore_charge_in, $9D9E/$9DA4/$9DAA (name proposed; never executed):
 * the behaviour of ids $2C, $2D, $2F (A = tier, Y = Andore's plain id
 * $25/$2B/$35; `pc`/`cycles` are the entry's LDY/LDA and its BNE $9DAE).
 * Kind 5 (Andore); it waits until the player is less than $80 px away in
 * x, then charges at him: obj_dir from the direction index toward the
 * player on his line (0F:FB5B, $A87E), obj_vx $024C level or $021F with
 * obj_vy $00E1 on a diagonal, animation $D2 with sprite bank $24 (0F:F443),
 * damage index $33 + the tier's, and he becomes Andore at phase $E,
 * obj_timer 3: the rush step of his charge (0D:A7AD-A814 is the same
 * code).
 * In: X = slot. Out: the RTS at $9DC1 (waiting) or $9E1F, A = the id.
 * Callers: obj_behavior_lo/hi (ids $2C, $2D, $2F).
 */
static void andore_charge_in(uint16_t pc, int cycles, uint8_t tier, uint8_t id)
{
    REGS_IN;

    mff_cycles(pc, cycles);
    y = id;
    a = tier;
    mff_cycles(0x9DAE, 25);
    RAM(OBJ_TIER + x) = a;
    a = y;
    RAM(OBJ_TIMER2 + x) = a;
    a = KIND_ANDORE;
    SET_NZ(a);
    RAM(OBJ_KIND + x) = a;
    CALL_FIXED(0x9DBA, func_F374);              /* obj_dist_x */
    mff_cycles(0x9DBD, 4);
    CMP_FLAGS(a, 0x80);
    if (c) {
        mff_cycles(0x9DC1, 6);
        RTS_RETURN();
    }
    mff_cycles(0x9DBF, 1);                      /* BCC $9DC2 */

    mff_cycles(0x9DC2, 6);
    CALL_FIXED(0x9DC2, func_FB5B);              /* direction index to the player */
    mff_cycles(0x9DC5, 6);
    y = a;
    a = prg_read((uint16_t)(CHARGE_DIR + y));
    mff_cycles(0x9DC9, 9);
    RAM(OBJ_DIR + x) = a;
    a &= 0x0C;
    if (a == 0) {
        mff_cycles(0x9DD0, 28);
        RAM(OBJ_VX_FRAC + x) = 0x4C;
        RAM(OBJ_VX + x) = 0x02;
        a = 0x00;
        RAM(OBJ_VY_FRAC + x) = a;
        RAM(OBJ_VY + x) = a;
        mff_cycles(0x9DE2, 1);                  /* BEQ $9DF8 */
    } else {
        mff_cycles(0x9DCE, 1);                  /* BNE $9DE4 */
        mff_cycles(0x9DE4, 28);
        RAM(OBJ_VX_FRAC + x) = 0x1F;
        RAM(OBJ_VX + x) = 0x02;
        RAM(OBJ_VY_FRAC + x) = 0xE1;
        a = 0x00;
        RAM(OBJ_VY + x) = a;
    }
    mff_cycles(0x9DF8, 10);
    a = ANIM_ANDORE_CHARGE;
    y = CHR_ANDORE;
    SET_NZ(y);
    CALL_FIXED(0x9DFC, func_F443);              /* obj_set_anim, chr_banks+3 = Y */
    mff_cycles(0x9DFF, 10);
    y = RAM(OBJ_TIER + x);
    a = prg_read((uint16_t)(TIER_INDEX + y));
    mff_cycles(0x9E06, 43);
    c = 0;
    ADC_A(CHARGE_DAMAGE);
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    RAM(OBJ_STATE + x) = PHASE_CHARGE;
    RAM(OBJ_TIMER + x) = CHARGE_STEP;
    a = RAM(OBJ_TIMER2 + x);
    SET_NZ(a);
    RAM(OBJ_ID + x) = a;
    RAM(OBJ_SAVED_ID + x) = a;
    RTS_RETURN();
}

void func_9D9E_b6(void)
{
    andore_charge_in(0x9D9E, 7, TIER_1, 0x25);  /* LDY #$25; LDA #1; BNE $9DAE */
}

void func_9DA4_b6(void)
{
    andore_charge_in(0x9DA4, 7, TIER_2, 0x2B);  /* LDY #$2B; LDA #2; BNE $9DAE */
}

void func_9DAA_b6(void)
{
    andore_charge_in(0x9DAA, 4, TIER_4, 0x35);  /* LDY #$35; LDA #4 */
}

/* ======================= dropping in, ids $36-$3E ======================= */

/*
 * enemy_drop_in, $9E48 (name proposed): the drop-in entrances, A = tier,
 * Y = kind (0 Poison, 3 AxL, 4 El Gado). The enemy waits hidden (the stage
 * list spawns it with obj_flags $D4) until the player is less than 8 px
 * away in x, or at once in round 3 area 1; then it shows, takes the
 * player's ground line as its obj_ground_y, starts falling (obj_vy $FF00,
 * no x speed) with the kind's fall animation ($84 from frame 6 for
 * Poison, $BE, $AC) and sprite bank ($1A, $35, $22), damage index $36,
 * $36 or $39 + the tier's, and becomes the plain id at phase $F (the
 * fall, attack out).
 * In: X = slot. Out: the RTS at $9E62 (waiting), or as $9FBB.
 */
static void enemy_drop_in(void)
{
    REGS_IN;
    int at_once = 0;

    mff_cycles(0x9E48, 19);
    RAM(OBJ_TIER + x) = a;
    a = y;
    RAM(OBJ_KIND + x) = a;
    a = RAM(ROUND);
    if (a != 0x03) {
        mff_cycles(0x9E53, 1);                  /* BNE $9E5B */
    } else {
        mff_cycles(0x9E55, 7);
        a = RAM(SUB_AREA);
        at_once = a == 0x01;
        if (at_once)
            mff_cycles(0x9E59, 1);              /* BEQ $9E63 */
    }
    if (!at_once) {
        mff_cycles(0x9E5B, 6);
        CALL_FIXED(0x9E5B, func_F374);          /* obj_dist_x */
        mff_cycles(0x9E5E, 4);
        CMP_FLAGS(a, DROP_REACH);
        if (c) {
            mff_cycles(0x9E62, 6);
            RTS_RETURN();
        }
        mff_cycles(0x9E60, 1);                  /* BCC $9E63 */
    }

    mff_cycles(0x9E63, 53);
    RAM(OBJ_FLAGS + x) = (uint8_t)(RAM(OBJ_FLAGS + x) & ~OBJ_HIDDEN);
    RAM(OBJ_GROUND_Y + x) = RAM(PLAYER_GROUND_Y);
    RAM(OBJ_VY + x) = 0xFF;
    a = 0x00;
    RAM(OBJ_VY_FRAC + x) = a;
    RAM(OBJ_VX_FRAC + x) = a;
    RAM(OBJ_VX + x) = a;
    y = RAM(OBJ_TIER + x);
    a = prg_read((uint16_t)(TIER_INDEX + y));
    mff_cycles(0x9E87, 10);
    c = 0;
    y = RAM(OBJ_KIND + x);
    ADC_A(prg_read((uint16_t)(DROP_DAMAGE + y)));
    mff_cycles(0x9E8E, 9);
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = prg_read((uint16_t)(DROP_ANIM + y));
    SET_NZ(a);
    mff_cycles(0x9E94, 6);
    CALL_FIXED(0x9E94, func_F288);              /* obj_set_anim */
    mff_cycles(0x9E97, 4);
    a = y;
    if (a == KIND_POISON) {
        mff_cycles(0x9E9A, 7);
        RAM(OBJ_ANIM_FRAME + x) = DROP_POISON_FRAME;
    } else {
        mff_cycles(0x9E98, 1);                  /* BNE $9E9F */
    }
    mff_cycles(0x9E9F, 4);
    a = prg_read((uint16_t)(DROP_CHR + y));
    mff_cycles(0x9EA2, 7);
    mff_push(a);
    a = prg_read((uint16_t)(KIND_CHR_SLOT + y));
    mff_cycles(0x9EA6, 18);
    y = a;
    a = mff_pull();
    RAM(CHR_BANKS + 3 + y) = a;
    a = PHASE_DROP;
    SET_NZ(a);
    RAM(OBJ_STATE + x) = a;
    JMP_TAIL(0x9EB0, 0x9FBB);                   /* enemy_become_common */
}

/*
 * The drop-in entries: LDA #tier and LDY #kind, with the BNE/BEQ that
 * reach $9E48 (`cycles` in all). $9E2E and $9E3C start inside the
 * .byte runs after the BEQ at $9E2C and the BNE at $9E3A.
 * Callers: obj_behavior_lo/hi.
 */
static void drop_in_entry(uint16_t pc, int cycles, uint8_t tier, uint8_t kind)
{
    mff_cycles(pc, cycles);
    g_cpu.A = tier;
    g_cpu.Y = kind;
    enemy_drop_in();
}

void func_9E20_b6(void) { drop_in_entry(0x9E20, 10, TIER_1, KIND_POISON); }   /* id $36, never executed */
void func_9E24_b6(void) { drop_in_entry(0x9E24, 10, TIER_2, KIND_POISON); }   /* id $37 */
void func_9E28_b6(void) { drop_in_entry(0x9E28, 7, TIER_4, KIND_POISON); }    /* id $38 */
void func_9E3C_b6(void) { drop_in_entry(0x9E3C, 7, TIER_1, KIND_AXL); }       /* id $39, never executed */
void func_9E40_b6(void) { drop_in_entry(0x9E40, 7, TIER_2, KIND_AXL); }       /* id $3A, never executed */
void func_9E44_b6(void) { drop_in_entry(0x9E44, 4, TIER_4, KIND_AXL); }       /* id $3B */
void func_9E2E_b6(void) { drop_in_entry(0x9E2E, 10, TIER_1, KIND_EL_GADO); }  /* id $3C, never executed */
void func_9E32_b6(void) { drop_in_entry(0x9E32, 10, TIER_2, KIND_EL_GADO); }  /* id $3D */
void func_9E36_b6(void) { drop_in_entry(0x9E36, 7, TIER_4, KIND_EL_GADO); }   /* id $3E */

/* ======================= standing first, ids $3F, $4F-$5F ======================= */

/*
 * enemy_stand_in, $9ECF (name proposed): the standing entrances, A =
 * tier. The kind comes from the id ($9F91), the attack is withdrawn, and
 * the enemy stands for $78 frames (phase $D, obj_ai_timer) with its
 * kind's standing animation (0D:AAAE) before it chooses a move, as the
 * plain id.
 * In: X = slot. Out: as $9FBB.
 */
static void enemy_stand_in(void)
{
    REGS_IN;

    mff_cycles(0x9ECF, 26);
    RAM(OBJ_TIER + x) = a;
    RAM(OBJ_ATTACK + x) = 0;
    a = RAM(OBJ_ID + x);
    c = 1;
    SBC_A(ENTRANCE_ID_FIRST);
    y = a;
    a = prg_read((uint16_t)(ENTRANCE_KIND + y));
    mff_cycles(0x9EE1, 25);
    RAM(OBJ_KIND + x) = a;
    RAM(OBJ_AI_TIMER + x) = STAND_FRAMES;
    a = PHASE_STAND;
    SET_NZ(a);
    RAM(OBJ_STATE + x) = a;
    CALL_BANK(0x9EEE, ENEMY_STAND_ANIM);
    JMP_TAIL(0x9EF1, 0x9FBB);                   /* enemy_become_common */
}

/*
 * The standing entries: tier 1 ($9EC5: ids $3F, $50, $53, $56, $59,
 * $5C), tier 2 ($9EC9, inside the .byte run after the BNE at $9EC7: $4F,
 * $51, $54, $57, $5A, $5D), tier 4 ($9ECD: $52, $55, $58, $5B, $5E, $5F).
 * Callers: obj_behavior_lo/hi.
 */
static void stand_in_entry(uint16_t pc, int cycles, uint8_t tier)
{
    mff_cycles(pc, cycles);
    g_cpu.A = tier;
    enemy_stand_in();
}

void func_9EC5_b6(void) { stand_in_entry(0x9EC5, 5, TIER_1); }   /* LDA #1; BNE $9ECF */
void func_9EC9_b6(void) { stand_in_entry(0x9EC9, 5, TIER_2); }   /* LDA #2; BNE $9ECF; never executed */
void func_9ECD_b6(void) { stand_in_entry(0x9ECD, 2, TIER_4); }   /* LDA #4 */

/* ======================= sitting in wait, ids $40-$4B ======================= */

/*
 * enemy_sit_in, $9F19 (name proposed): the sitting entrances, A = tier,
 * obj_timer3 = the pose (0 or 2). The kind comes from the id ($9F91),
 * the attack is withdrawn. Phase 0 sits it down: animation $A2 held at
 * frame kind - 1 + pose and that frame's sprite bank ($9F8D), phase 1.
 * Phase 1 keeps the animation still (obj_anim_timer 0) and the enemy
 * unhittable (obj_invuln $FF) until the player comes within $32 px in x
 * and y (0F:FB3E) or its screen x falls under $40; then it gets up as
 * the plain id at phase 0.
 * In: X = slot. Out: the RTS at $9F5C or $9F8C, or as $9FBB.
 */
static void enemy_sit_in(void)
{
    REGS_IN;

    mff_cycles(0x9F19, 19);
    RAM(OBJ_TIER + x) = a;
    a = RAM(OBJ_ID + x);
    c = 1;
    SBC_A(ENTRANCE_ID_FIRST);
    y = a;
    a = prg_read((uint16_t)(ENTRANCE_KIND + y));
    mff_cycles(0x9F26, 20);
    RAM(OBJ_KIND + x) = a;
    RAM(OBJ_ATTACK + x) = 0;
    a = RAM(OBJ_STATE + x);
    c = a & 0x01;
    a >>= 1;
    if (!c) {
        /* phase 0: sit down */
        mff_cycles(0x9F34, 44);
        RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        y = (uint8_t)(RAM(OBJ_KIND + x) - 1);
        a = y;
        ADC_A(RAM(OBJ_TIMER3 + x));             /* C clear from the LSR */
        y = a;
        RAM(OBJ_ANIM_FRAME + x) = a;
        RAM(OBJ_ANIM + x) = ANIM_SITTING;
        RAM(OBJ_ANIM_TIMER + x) = 0;
        a = prg_read((uint16_t)(SIT_CHR + y));
        mff_cycles(0x9F50, 11);
        mff_push(a);
        y = RAM(OBJ_KIND + x);
        a = prg_read((uint16_t)(KIND_CHR_SLOT + y));
        mff_cycles(0x9F57, 17);
        y = a;
        a = mff_pull();
        SET_NZ(a);
        RAM(CHR_BANKS + 3 + y) = a;
        RTS_RETURN();
    }

    /* phase 1: wait */
    mff_cycles(0x9F32, 1);                      /* BCS $9F5D */
    mff_cycles(0x9F5D, 17);
    RAM(OBJ_ANIM_TIMER + x) = 0;
    a = SIT_WAKE_REACH;
    y = SIT_WAKE_REACH;
    SET_NZ(y);
    CALL_FIXED(0x9F66, func_FB3E);              /* the player within A px in x, Y in y: C */
    mff_cycles(0x9F69, 2);
    if (!c) {
        mff_cycles(0x9F6B, 27);
        RAM(OBJ_INVULN + x) = 0xFF;
        /* the screen x: the high bytes' SBC only passes its borrow on */
        a = RAM(OBJ_X_HI + x);
        c = 1;
        SBC_A(RAM(SCROLL_X_HI));
        a = RAM(OBJ_X + x);
        SBC_A(RAM(SCROLL_X));
        CMP_FLAGS(a, SIT_WAKE_EDGE);
        if (c) {
            mff_cycles(0x9F7D, 1);              /* BCS $9F8C */
            mff_cycles(0x9F8C, 6);
            RTS_RETURN();
        }
    } else {
        mff_cycles(0x9F69, 1);                  /* BCS $9F7F */
    }
    /* get up */
    mff_cycles(0x9F7F, 14);
    RAM(OBJ_INVULN + x) = 0;
    a = OBJ_LIVE;
    SET_NZ(a);
    RAM(OBJ_STATE + x) = a;
    JMP_TAIL(0x9F89, 0x9FBB);                   /* enemy_become_common */
}

/*
 * The sitting entries: LDA #pose / STA obj_timer3 / LDA #tier and the
 * branches between (`pre` cycles to the STA, `post` after it).
 * Callers: obj_behavior_lo/hi.
 */
static void sit_in_entry(uint16_t pc, int pre, uint8_t pose, uint16_t lda_pc, int post, uint8_t tier)
{
    uint8_t x = g_cpu.X;

    mff_cycles(pc, pre);
    RAM(OBJ_TIMER3 + x) = pose;
    mff_cycles(lda_pc, post);
    g_cpu.A = tier;
    enemy_sit_in();
}

/* tier 1: LDA #pose (BEQ $9EFA), STA obj_timer3, LDA #1, BNE $9F19 */
void func_9EF4_b6(void) { sit_in_entry(0x9EF4, 10, 0, 0x9EFD, 5, TIER_1); }   /* ids $40, $43 */
void func_9EF8_b6(void) { sit_in_entry(0x9EF8, 7, 2, 0x9EFD, 5, TIER_1); }    /* ids $46, $49 */
/* tier 2: from $9F01, inside the .byte run after the BNE at $9EFF */
void func_9F01_b6(void) { sit_in_entry(0x9F01, 10, 0, 0x9F0A, 5, TIER_2); }   /* ids $41, $44; never executed */
void func_9F05_b6(void) { sit_in_entry(0x9F05, 7, 2, 0x9F0A, 5, TIER_2); }    /* ids $47, $4A */
/* tier 4: LDA #4 falls into $9F19 */
void func_9F0E_b6(void) { sit_in_entry(0x9F0E, 10, 0, 0x9F17, 2, TIER_4); }   /* ids $42, $45 */
void func_9F12_b6(void) { sit_in_entry(0x9F12, 7, 2, 0x9F17, 2, TIER_4); }    /* ids $48, $4B */

/* ======================= the plain id ======================= */

/*
 * enemy_become_common, $9FBB (name proposed): obj_id and obj_saved_id =
 * the plain id of the entrance ($9FCC[id - $36]).
 * In: X = slot. Out: A = the id, N/Z from it; C/V from the SBC.
 * Callers: JMP at 0C:9EB0, 9EF1, 9F89.
 */
void func_9FBB_b6(void)
{
    REGS_IN;

    mff_cycles(0x9FBB, 14);
    a = RAM(OBJ_ID + x);
    c = 1;
    SBC_A(ENTRANCE_ID_FIRST);
    y = a;
    a = prg_read((uint16_t)(ENTRANCE_BECOMES + y));
    SET_NZ(a);
    mff_cycles(0x9FC5, 16);
    RAM(OBJ_ID + x) = a;
    RAM(OBJ_SAVED_ID + x) = a;
    RTS_RETURN();
}
