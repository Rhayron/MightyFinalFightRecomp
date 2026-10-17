/*
 * obj_thrown.c - the thrown bodies: the enemy a throw sends flying (ids 4
 * Cody's throw, 8 Haggar's back throw, $0E the HeadSmasher, $0F Guy's
 * throw), from the moment the player lets go to the landing that hurts it.
 * While one flies, objects_update has it knock down what it touches
 * (thrown_body_collide, obj_update.c). docs/objects.md, "Ids".
 *
 * Each body is a state machine on obj_state & $0F, dispatched through its
 * phase table; a phase that is done increments obj_state and falls into
 * the next phase's code in the same frame. The early phases follow the
 * player's throw animation (slot 0's obj_anim_frame/obj_anim_timer) and
 * move the body around the player with fixed offsets; then it flies
 * (obj_vx, obj_vy and gravity, 0F:F1FD), bounces once, lands on
 * obj_land_y taking the landing's damage (attack 5, obj_take_hit), and
 * gets up through obj_knockdown_recover.
 *
 * The scaffold made a function of every phase and every JMP target; a
 * phase that falls into the next one calls that function instead of
 * repeating it, except where the two differ (a branch the scaffold sends
 * through the interpreter in one and not in the other).
 */
#include "obj_defs.h"

/* tables in bank 0F, by obj_kind */
#define KIND_THROWN_ANIM    0xFC5B  /* the kind's animation when thrown */
#define KIND_THROWN_ANIM2   0xFC68  /* the next one, at the throw's second step */
#define KIND_CHR_BANK       0xFC75  /* the kind's sprite bank ... */
#define KIND_CHR_SLOT       0xFC33  /* ... and which of chr_banks+3..5 takes it */

#define ATTACK_LANDING      0x05    /* the damage a landing deals (obj_take_hit) */

/* game_action values that end a throw's steps at once: the player was hit
 * or is down (6, $1F, $0A) */
static const uint8_t k_throw_cut[] = { 0x06, 0x1F, 0x0A };

/* obj_invuln while obj_knockdown_recover gets the body up */
static const uint8_t k_getting_up[] = { 0xFE, 0xFD };

/*
 * x += the 16-bit offset at table[obj_dir & 2] (from the side the body
 * faces), as the copy at `pc` does it: LDA obj_dir,X / AND #2 / TAY /
 * LDA obj_x,X / CLC / ADC table,Y / PHA / LDA obj_x_hi,X / ADC table+1,Y /
 * STA obj_x_hi,X / PLA / STA obj_x,X. Leaves Y = obj_dir & 2, A = obj_x
 * with N/Z, C/V from the high byte.
 */
#define SHIFT_X(pc, table) do { \
        mff_cycles(pc, 18); \
        y = RAM(OBJ_DIR + x) & 0x02; \
        a = RAM(OBJ_X + x); \
        c = 0; \
        ADC_A(prg_read((uint16_t)((table) + y))); \
        mff_cycles((uint16_t)((pc) + 13), 11); \
        mff_push(a); \
        a = RAM(OBJ_X_HI + x); \
        ADC_A(prg_read((uint16_t)((table) + 1 + y))); \
        mff_cycles((uint16_t)((pc) + 20), 14); \
        RAM(OBJ_X_HI + x) = a; \
        a = mff_pull(); \
        SET_NZ(a); \
        RAM(OBJ_X + x) = a; \
    } while (0)

/*
 * The kind's animation from `table` and its sprite bank (the copy at
 * `pc`): obj_set_anim(table[obj_kind]) and chr_banks+3+$FC33[kind] =
 * $FC75[kind], skipped when the animation equals TMP0. TMP0 holds the low
 * byte of the phase routine's address here (the phase jump left it), so
 * the test looks like a slip for the CMP obj_anim,X of 0C:91D6.
 */
#define SET_KIND_ANIM(pc, table) do { \
        mff_cycles(pc, 10); \
        y = RAM(OBJ_KIND + x); \
        a = prg_read((uint16_t)((table) + y)); \
        mff_cycles((uint16_t)((pc) + 7), 5); \
        CMP_FLAGS(a, RAM(TMP0)); \
        if (!z) { \
            mff_cycles((uint16_t)((pc) + 11), 6); \
            CALL_FIXED((uint16_t)((pc) + 11), func_F288); \
            mff_cycles((uint16_t)((pc) + 14), 4); \
            a = prg_read((uint16_t)(KIND_CHR_BANK + y)); \
            mff_cycles((uint16_t)((pc) + 17), 7); \
            RAM(TMP0) = a; \
            a = prg_read((uint16_t)(KIND_CHR_SLOT + y)); \
            mff_cycles((uint16_t)((pc) + 22), 10); \
            y = a; \
            a = RAM(TMP0); \
            RAM(CHR_BANKS + 3 + y) = a; \
        } else { \
            mff_cycles((uint16_t)((pc) + 9), branch_taken((uint16_t)((pc) + 9), (uint16_t)((pc) + 28))); \
        } \
    } while (0)

/*
 * The test that starts a throw step (the copy at `pc`): go on when the
 * player is down (k_throw_cut, the BEQs to `go_pc`) or when slot 0's
 * `field` (obj_anim_timer or obj_anim_frame) equals `val`. Sets `go`; A
 * and the flags are those of the last compare. The caller charges the BNE
 * it takes when it is not time yet.
 */
#define THROW_STEP(pc, go_pc, field, val, go) do { \
        int i_; \
        mff_cycles(pc, 3); \
        a = RAM(GAME_ACTION); \
        i_ = cmp_list((uint16_t)((pc) + 2), go_pc, a, k_throw_cut, 3); \
        if (i_ < 3) { \
            CMP_FLAGS(a, k_throw_cut[i_]); \
            go = 1; \
        } else { \
            mff_cycles((uint16_t)((pc) + 14), 8); \
            a = RAM(field); \
            CMP_FLAGS(a, val); \
            go = z; \
        } \
    } while (0)

/* ======================= Cody's throw, id 4 ======================= */

static void thrown_cody_turn(int native_rts);

/*
 * thrown_cody, $8C1C (name proposed): behaviour of id 4, the enemy Cody
 * throws. JMP through the phase table at $8C2F/$8C37: 0 $8C3F, 1 $8C98,
 * 2 $8D00, 3 $8D88, 4 $8E43, 5 $8E5A, 6 $8EE5, 7 $8F8D.
 * In: X = slot. Out: as the phase.
 * Caller: obj_behavior_lo/hi (id 4).
 */
void func_8C1C_b6(void)
{
    obj_phase_jump(0x8C1C, 0x8C2F, 0x8C37);
}

/*
 * Phase 0, $8C3F: the throw begins. obj_timer2 = $10 (the blink frames if
 * it dies), obj_timer3 = 8 (frames before it hits others), the kind's
 * thrown animation and sprite bank, obj_land_y = the player's obj_y, the
 * body lifted 15 px; with obj_throw_flag set it moves by $FCE2[dir].
 * In: X = slot. Out: A = obj_x after the move, or obj_throw_flag (0).
 */
void func_8C3F_b6(void)
{
    REGS_IN;

    mff_cycles(0x8C3F, 14);
    RAM(OBJ_TIMER2 + x) = 0x10;
    RAM(OBJ_TIMER3 + x) = 0x08;
    SET_KIND_ANIM(0x8C49, KIND_THROWN_ANIM);
    mff_cycles(0x8C65, 35);
    RAM(OBJ_LAND_Y + x) = RAM(OBJ_Y);
    RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    a = RAM(OBJ_Y + x);
    c = 1;
    SBC_A(0x0F);
    RAM(OBJ_Y + x) = a;
    a = RAM(OBJ_THROW_FLAG + x);
    SET_NZ(a);
    if (!z)
        SHIFT_X(0x8C7C, 0xFCE2);
    else
        mff_cycles(0x8C7A, 1);
    mff_cycles(0x8C97, 6);
    RTS_RETURN();
}

/*
 * Phase 1, $8C98: waits for the player's throw animation (obj_anim_timer
 * 4, or the player down), then moves the body by $FCE6[dir], 8 px up, and
 * goes on to phase 2. With obj_throw_flag clear (never seen: every throw
 * in the logs sets it, 0E:D7D4) it first moves by $FCD6[dir], 8 px up, a
 * phase ahead.
 * In: X = slot. Out: the RTS at $8D4B, or as phase 2.
 */
void func_8C98_b6(void)
{
    REGS_IN;
    int go;

    mff_cycles(0x8C98, 6);
    a = RAM(OBJ_THROW_FLAG + x);
    if (a == 0) {
        SHIFT_X(0x8C9D, 0xFCD6);
        mff_cycles(0x8CB8, 20);
        RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        a = RAM(OBJ_Y + x);
        c = 1;
        SBC_A(0x08);
        RAM(OBJ_Y + x) = a;
    } else {
        mff_cycles(0x8C9B, 1);
    }
    THROW_STEP(0x8CC4, 0x8CD9, OBJ_ANIM_TIMER, 0x04, go);
    if (!go) {
        mff_cycles(0x8CD7, 2);
        mff_cycles(0x8D4B, 6);
        RTS_RETURN();
    }
    SHIFT_X(0x8CD9, 0xFCE6);
    mff_cycles(0x8CF4, 20);
    RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    a = RAM(OBJ_Y + x);
    c = 1;
    SBC_A(0x08);
    RAM(OBJ_Y + x) = a;
    SYNC_OUT;
    func_8D00_b6();
}

/*
 * Phase 2, $8D00: at the player's obj_anim_frame 1 the body gets the
 * kind's second thrown animation; then at obj_anim_timer 4 it moves by
 * $FCEA[dir], 6 px up, and goes on to phase 3. (obj_throw_flag clear,
 * never seen: $FCDA[dir], 6 px up, and wait.)
 * In: X = slot. Out: the RTS at $8D4B, or as phase 3.
 */
void func_8D00_b6(void)
{
    REGS_IN;
    int go;

    THROW_STEP(0x8D00, 0x8D15, OBJ_ANIM_FRAME, 0x01, go);
    if (!go) {
        mff_cycles(0x8D13, 1);
        mff_cycles(0x8D4B, 6);
        RTS_RETURN();
    }
    mff_cycles(0x8D15, 10);
    y = RAM(OBJ_KIND + x);
    a = prg_read((uint16_t)(KIND_THROWN_ANIM2 + y));
    SET_NZ(a);
    mff_cycles(0x8D1C, 6);
    CALL_FIXED(0x8D1C, func_F288);
    mff_cycles(0x8D1F, 6);
    a = RAM(OBJ_THROW_FLAG + x);
    if (a == 0) {
        SHIFT_X(0x8D24, 0xFCDA);
        mff_cycles(0x8D3F, 20);
        RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        a = RAM(OBJ_Y + x);
        c = 1;
        SBC_A(0x06);
        SET_NZ(a);
        RAM(OBJ_Y + x) = a;
        mff_cycles(0x8D4B, 6);
        RTS_RETURN();
    }
    mff_cycles(0x8D22, 1);
    THROW_STEP(0x8D4C, 0x8D61, OBJ_ANIM_TIMER, 0x04, go);
    if (!go) {
        mff_cycles(0x8D5F, 1);
        mff_cycles(0x8D4B, 6);
        RTS_RETURN();
    }
    SHIFT_X(0x8D61, 0xFCEA);
    mff_cycles(0x8D7C, 20);
    RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    a = RAM(OBJ_Y + x);
    c = 1;
    SBC_A(0x06);
    RAM(OBJ_Y + x) = a;
    SYNC_OUT;
    thrown_cody_turn(1);
}

/*
 * Phase 3, $8D88: entered on its own; see thrown_cody_turn.
 */
void func_8D88_b6(void)
{
    thrown_cody_turn(0);
}

/*
 * $8D88-$8DEE: at the player's obj_anim_frame 2 the body goes over the
 * player's head: facing right (obj_dir bit 0) it is put on the screen's
 * left edge if keep_on_screen asks (also in round 0 at scroll_x_hi 3) and
 * past it, then JMP $8DEF; facing left it is put on the right edge (both
 * never executed) and falls into $8DEF. Before that step it waits on the
 * RTS at $8D4B: natively when phase 2 fell in here, through the
 * interpreter when the phase jump came straight to $8D88 (the scaffold
 * sends that backward branch out of the function).
 */
static void thrown_cody_turn(int native_rts)
{
    REGS_IN;
    int go, clamp;

    THROW_STEP(0x8D88, 0x8D9D, OBJ_ANIM_FRAME, 0x02, go);
    if (!go) {
        mff_cycles(0x8D9B, 1);
        if (native_rts) {
            mff_cycles(0x8D4B, 6);
            RTS_RETURN();
        }
        BRANCH_TAIL(0x8D4B);
    }
    mff_cycles(0x8D9D, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    if (a != 0) {
        mff_cycles(0x8DA4, 5);
        a = RAM(ROUND);
        clamp = 0;
        if (a != 0) {
            mff_cycles(0x8DA6, 1);
        } else {
            mff_cycles(0x8DA8, 7);
            a = RAM(SCROLL_X_HI);
            c = a >= 0x03;
            clamp = a == 0x03;
            if (clamp)
                mff_cycles(0x8DAC, 1);
        }
        if (!clamp) {
            mff_cycles(0x8DAE, 5);
            a = RAM(KEEP_ON_SCREEN);
            SET_NZ(a);
            clamp = !z;
            if (!clamp)
                mff_cycles(0x8DB0, 1);
        }
        if (clamp) {
            mff_cycles(0x8DB2, 13);
            a = RAM(OBJ_X + x);
            c = 1;
            SBC_A(RAM(SCROLL_X));
            CMP_FLAGS(a, 0x10);
            if (!c) {
                mff_cycles(0x8DBC, 22);
                a = RAM(SCROLL_X);
                c = 0;
                ADC_A(0x10);
                RAM(OBJ_X + x) = a;
                a = RAM(AREA_SCREEN);
                ADC_A(0x00);
                SET_NZ(a);
                RAM(OBJ_X_HI + x) = a;
                JMP_TAIL(0x8DCB, 0x8DEE);
            }
            mff_cycles(0x8DBA, 1);
        }
        JMP_TAIL(0x8DCE, 0x8DEF);
    }
    mff_cycles(0x8DA2, 1);
    mff_cycles(0x8DD1, 5);
    a = RAM(KEEP_ON_SCREEN);
    if (a != 0) {
        mff_cycles(0x8DD5, 13);
        a = RAM(OBJ_X + x);
        c = 1;
        SBC_A(RAM(SCROLL_X));
        if (a >= 0xF1) {
            mff_cycles(0x8DDF, 28);
            a = RAM(SCROLL_X);
            c = 0;
            ADC_A(0xF0);
            RAM(OBJ_X + x) = a;
            a = RAM(AREA_SCREEN);
            ADC_A(0x00);
            SET_NZ(a);
            RAM(OBJ_X_HI + x) = a;
            RTS_RETURN();
        }
        mff_cycles(0x8DDD, 1);
    } else {
        mff_cycles(0x8DD3, 1);
    }
    SYNC_OUT;
    func_8DEF_b6();
}

/*
 * $8DEF: the last step around the player: moves by $FCEE[dir], 6 px up,
 * and phase 4 follows at once. (obj_throw_flag clear, never seen:
 * $FCDE[dir], 6 px up, and wait.)
 * In: X = slot. Out: as phase 4, or N/Z from obj_state.
 * Callers: JMP at 0C:8DCE; the fall-through at 0C:8DD3/8DDD.
 */
void func_8DEF_b6(void)
{
    REGS_IN;

    mff_cycles(0x8DEF, 6);
    a = RAM(OBJ_THROW_FLAG + x);
    if (a == 0) {
        SHIFT_X(0x8DF4, 0xFCDE);
        mff_cycles(0x8E0F, 26);
        a = RAM(OBJ_Y + x);
        c = 1;
        SBC_A(0x06);
        RAM(OBJ_Y + x) = a;
        t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_STATE + x) = t;
        SET_NZ(t);
        RTS_RETURN();
    }
    mff_cycles(0x8DF2, 2);
    SHIFT_X(0x8E1C, 0xFCEE);
    mff_cycles(0x8E37, 20);
    a = RAM(OBJ_Y + x);
    c = 1;
    SBC_A(0x06);
    RAM(OBJ_Y + x) = a;
    RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    SYNC_OUT;
    func_8E43_b6();
}

/*
 * Phase 4, $8E43: let go: obj_vx = 2, obj_vy = $0101, and phase 5 at once.
 * In: X = slot. Out: as phase 5.
 */
void func_8E43_b6(void)
{
    REGS_IN;

    mff_cycles(0x8E43, 35);
    RAM(OBJ_VX_FRAC + x) = 0x00;
    RAM(OBJ_VX + x) = 0x02;
    RAM(OBJ_VY_FRAC + x) = 0x01;
    a = 0x01;
    RAM(OBJ_VY + x) = a;
    RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    SYNC_OUT;
    func_8E5A_b6();
}

/*
 * Phase 5, $8E5A: the flight. At obj_anim_timer $0C 0C:9C55 runs; the body
 * moves back from its facing (obj_move_x_clamped) and falls ($8EB4).
 * In: X = slot. Out: as $8EB4.
 */
void func_8E5A_b6(void)
{
    REGS_IN;

    mff_cycles(0x8E5A, 8);
    a = RAM(OBJ_ANIM_TIMER + x);
    CMP_FLAGS(a, 0x0C);
    if (z) {
        mff_cycles(0x8E61, 6);
        CALL_BANK(0x8E61, 0x9C55);
    } else {
        mff_cycles(0x8E5F, 1);
    }
    SYNC_OUT;
    obj_move_x_clamped(0x8E64, 0, func_8EB4_b6);
}

/*
 * $8EB4: y -= obj_vy with gravity (0F:F1FD); falling (obj_vy negative) and
 * down to obj_land_y it lands there: obj_pit_check, a bounce (obj_vy
 * $0238, obj_vx $0080), and phase 6 at once.
 * In: X = slot. Out: the RTS (still in the air), or as phase 6.
 * Callers: JMP at 0C:8E8E (and the clamps); the fall-through from 0C:8EB1.
 */
void func_8EB4_b6(void)
{
    REGS_IN;

    mff_cycles(0x8EB4, 6);
    CALL_FIXED(0x8EB4, func_F1FD);
    mff_cycles(0x8EB7, 6);
    a = RAM(OBJ_VY + x);
    SET_NZ(a);
    if (!n) {
        mff_cycles(0x8EBA, 7);
        RTS_RETURN();
    }
    mff_cycles(0x8EBC, 10);
    a = RAM(OBJ_Y + x);
    CMP_FLAGS(a, RAM(OBJ_LAND_Y + x));
    if (!c) {
        mff_cycles(0x8EC4, 6);
        RTS_RETURN();
    }
    mff_cycles(0x8EC2, 1);
    mff_cycles(0x8EC5, 15);
    a = RAM(OBJ_LAND_Y + x);
    SET_NZ(a);
    RAM(OBJ_Y + x) = a;
    CALL_BANK(0x8ECB, 0x9B1F);                  /* obj_pit_check */
    mff_cycles(0x8ECE, 35);
    RAM(OBJ_VY_FRAC + x) = 0x38;
    RAM(OBJ_VY + x) = 0x02;
    RAM(OBJ_VX_FRAC + x) = 0x80;
    a = 0x00;
    RAM(OBJ_VX + x) = a;
    RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    SYNC_OUT;
    func_8EE5_b6();
}

/*
 * Phase 6, $8EE5: the bounce. Once obj_knockdown_recover has it getting up
 * (obj_invuln $FE/$FD) that runs it (JMP $9BFC); otherwise it moves back
 * and falls ($8F45).
 * In: X = slot. Out: as obj_knockdown_recover or $8F45.
 */
void func_8EE5_b6(void)
{
    REGS_IN;
    int i;

    mff_cycles(0x8EE5, 4);
    a = RAM(OBJ_INVULN + x);
    i = cmp_list(0x8EE8, 0x8EF2, a, k_getting_up, 2);
    CMP_LIST_FLAGS(a, k_getting_up, 2, i);
    if (i < 2)
        JMP_TAIL(0x8EF2, 0x9BFC);
    mff_cycles(0x8EF0, 3);                      /* BNE $8EF5 */
    SYNC_OUT;
    obj_move_x_clamped(0x8EF5, 0, func_8F45_b6);
}

/*
 * $8F45: the second fall. On landing (as $8EB4) the landing hurts: the
 * player's attack_id is parked in saved_attack_type while obj_take_hit
 * deals attack 5; then obj_invuln = $FF and obj_knockdown_recover. If that
 * killed it (never executed): its attack is withdrawn, phase 7, a kill
 * counted, exp_award, JMP $9C7A.
 * In: X = slot. Out: the RTS (in the air), or as obj_knockdown_recover.
 * Callers: JMP at 0C:8F1F (and the clamps); the fall-through from 0C:8F42.
 */
void func_8F45_b6(void)
{
    REGS_IN;

    mff_cycles(0x8F45, 6);
    CALL_FIXED(0x8F45, func_F1FD);
    mff_cycles(0x8F48, 6);
    a = RAM(OBJ_VY + x);
    SET_NZ(a);
    if (!n) {
        mff_cycles(0x8F4B, 7);
        RTS_RETURN();
    }
    mff_cycles(0x8F4D, 10);
    a = RAM(OBJ_Y + x);
    CMP_FLAGS(a, RAM(OBJ_LAND_Y + x));
    if (!c) {
        mff_cycles(0x8F55, 6);
        RTS_RETURN();
    }
    mff_cycles(0x8F53, 1);
    mff_cycles(0x8F56, 15);
    a = RAM(OBJ_LAND_Y + x);
    SET_NZ(a);
    RAM(OBJ_Y + x) = a;
    CALL_BANK(0x8F5C, 0x9B1F);                  /* obj_pit_check */
    mff_cycles(0x8F5F, 18);
    RAM(SAVED_ATTACK_TYPE) = RAM(ATTACK_ID);
    a = ATTACK_LANDING;
    SET_NZ(a);
    RAM(ATTACK_ID) = a;
    CALL_BANK(0x8F68, 0x84F0);                  /* obj_take_hit */
    mff_cycles(0x8F6B, 6);
    a = RAM(OBJ_LIFE + x);
    if (a & 0x80) {
        mff_cycles(0x8F70, 25);
        a = 0x00;
        RAM(OBJ_ATTACK + x) = a;
        RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        t = (uint8_t)(RAM(AREA_KILLS) + 1);
        RAM(AREA_KILLS) = t;
        SET_NZ(t);
        CALL_BANK(0x8F7A, 0x89E3);              /* exp_award */
        JMP_TAIL(0x8F7D, 0x9C7A);
    }
    mff_cycles(0x8F6E, 1);
    mff_cycles(0x8F80, 14);
    RAM(ATTACK_ID) = RAM(SAVED_ATTACK_TYPE);
    a = 0xFF;
    SET_NZ(a);
    RAM(OBJ_INVULN + x) = a;
    JMP_TAIL(0x8F8A, 0x9BFC);
}

/*
 * Phase 7, $8F8D: a dead body blinks: obj_timer2 counts down, hidden on
 * even counts (0C:9B62) and shown on odd ones (0C:9B6B), then the slot is
 * cleared (JMP object_clear).
 * In: X = slot. Out: as 0C:9B62/9B6B or object_clear.
 */
void func_8F8D_b6(void)
{
    REGS_IN;

    mff_cycles(0x8F8D, 13);
    RAM(OBJ_TIMER2 + x) = (uint8_t)(RAM(OBJ_TIMER2 + x) - 1);
    a = RAM(OBJ_TIMER2 + x);
    SET_NZ(a);
    if (z) {
        mff_cycles(0x8F93, 1);
        JMP_FIXED(0x8FA2, 0xF2BE, func_F2BE);
    }
    mff_cycles(0x8F95, 4);
    a &= 0x01;
    SET_NZ(a);
    if (z) {
        mff_cycles(0x8F99, 6);
        CALL_BANK(0x8F99, 0x9B62);
        mff_cycles(0x8F9C, 2);
        if (!z) {
            mff_cycles(0x8F9C, 7);              /* BNE taken; RTS */
            RTS_RETURN();
        }
    } else {
        mff_cycles(0x8F97, 1);
    }
    mff_cycles(0x8F9E, 6);
    CALL_BANK(0x8F9E, 0x9B6B);
    mff_cycles(0x8FA1, 6);
    RTS_RETURN();
}
