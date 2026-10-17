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
    obj_blink(0x8F8D, 0x8FA1);
}

/* ==================== Haggar's back throw, id 8 ==================== */

#define SFX_BACK_THROW      0x33

/*
 * thrown_back, $8FA5 (name proposed): behaviour of id 8, the enemy
 * Haggar's back throw slams behind him. Phases ($8FB8/$8FBF): 0 $8FC6,
 * 1 $9036, 2 $9078, 3 $90BA, 4 $9128, 5 $8F8D (id 4's blink), 6 $917E.
 * In: X = slot. Out: as the phase.
 * Caller: obj_behavior_lo/hi (id 8).
 */
void func_8FA5_b6(void)
{
    obj_phase_jump(0x8FA5, 0x8FB8, 0x8FBF);
}

/*
 * Phase 0, $8FC6: sound $33; obj_timer2 = $10, obj_timer3 = 8, and
 * obj_ground_y = $10, used here as the frames the body lies before the
 * slam hurts (phase 4); 4 px up, 0C:9C55, faces its obj_dir (0F:F090 /
 * F099) and moves beside the player ($8FF9).
 * In: X = slot. Out: as $8FF9.
 */
void func_8FC6_b6(void)
{
    REGS_IN;

    mff_cycles(0x8FC6, 8);
    a = SFX_BACK_THROW;
    SET_NZ(a);
    CALL_FIXED(0x8FC8, func_F34C);
    mff_cycles(0x8FCB, 47);
    RAM(OBJ_TIMER2 + x) = 0x10;
    RAM(OBJ_TIMER3 + x) = 0x08;
    RAM(OBJ_GROUND_Y + x) = 0x10;
    RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    a = RAM(OBJ_Y + x);
    c = 1;
    SBC_A(0x04);
    SET_NZ(a);
    RAM(OBJ_Y + x) = a;
    CALL_BANK(0x8FE6, 0x9C55);
    mff_cycles(0x8FE9, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    SET_NZ(a);
    if (z) {
        mff_cycles(0x8FF0, 6);
        CALL_FIXED(0x8FF0, func_F099);
        JMP_TAIL(0x8FF3, 0x8FF9);
    }
    mff_cycles(0x8FEE, 1);
    mff_cycles(0x8FF6, 6);
    CALL_FIXED(0x8FF6, func_F090);
    SYNC_OUT;
    func_8FF9_b6();
}

/*
 * $8FF9: x += $FCF6[dir], or $FCF2[dir] with obj_throw_flag clear.
 * In: X = slot. Out: A = obj_x, N/Z from it.
 * Callers: JMP at 0C:8FF3; the fall-through from 0C:8FF6.
 */
void func_8FF9_b6(void)
{
    REGS_IN;

    mff_cycles(0x8FF9, 6);
    a = RAM(OBJ_THROW_FLAG + x);
    if (a == 0) {
        SHIFT_X(0x8FFE, 0xFCF2);
        mff_cycles(0x9019, 6);
        RTS_RETURN();
    }
    mff_cycles(0x8FFC, 2);
    SHIFT_X(0x901A, 0xFCF6);
    mff_cycles(0x9035, 6);
    RTS_RETURN();
}

/*
 * Phase 1, $9036: at the player's obj_anim_frame 1, timer 0, the body
 * goes 7 px up; then (or at once when the player is down) the next phase,
 * x += $FCFA[dir].
 * In: X = slot. Out: the RTS with the last test's flags, or A = obj_x.
 */
void func_9036_b6(void)
{
    REGS_IN;
    int i;

    mff_cycles(0x9036, 3);
    a = RAM(GAME_ACTION);
    i = cmp_list(0x9038, 0x9059, a, k_throw_cut, 3);
    if (i == 3) {
        mff_cycles(0x9044, 8);
        a = RAM(OBJ_ANIM_FRAME);
        CMP_FLAGS(a, 0x01);
        if (!z) {
            mff_cycles(0x9049, 7);              /* BNE taken; RTS */
            RTS_RETURN();
        }
        mff_cycles(0x904B, 6);
        a = RAM(OBJ_ANIM_TIMER);
        SET_NZ(a);
        if (!z) {
            mff_cycles(0x904E, 7);
            RTS_RETURN();
        }
        mff_cycles(0x9050, 13);
        a = RAM(OBJ_Y + x);
        c = 1;
        SBC_A(0x07);
        RAM(OBJ_Y + x) = a;
    }
    mff_cycles(0x9059, 7);
    RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    SHIFT_X(0x905C, 0xFCFA);
    mff_cycles(0x9077, 6);
    RTS_RETURN();
}

/*
 * Phase 2, $9078: at the player's frame 2, timer 0 (or the player down):
 * the next phase, 6 px up, x += $FCFE[dir].
 * In: X = slot. Out: the RTS with the last test's flags, or A = obj_x.
 */
void func_9078_b6(void)
{
    REGS_IN;
    int i;

    mff_cycles(0x9078, 3);
    a = RAM(GAME_ACTION);
    i = cmp_list(0x907A, 0x9092, a, k_throw_cut, 3);
    if (i == 3) {
        mff_cycles(0x9086, 8);
        a = RAM(OBJ_ANIM_FRAME);
        CMP_FLAGS(a, 0x02);
        if (!z) {
            mff_cycles(0x908B, 7);
            RTS_RETURN();
        }
        mff_cycles(0x908D, 6);
        a = RAM(OBJ_ANIM_TIMER);
        SET_NZ(a);
        if (!z) {
            mff_cycles(0x9090, 7);
            RTS_RETURN();
        }
    }
    mff_cycles(0x9092, 20);
    RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    a = RAM(OBJ_Y + x);
    c = 1;
    SBC_A(0x06);
    RAM(OBJ_Y + x) = a;
    SHIFT_X(0x909E, 0xFCFE);
    mff_cycles(0x90B9, 6);
    RTS_RETURN();
}

/*
 * Phase 3, $90BA: at the player's frame 3, timer 0, the kind's second
 * thrown animation and its sprite bank; then (or at once when the player
 * is down) the slam: the body on the player's line (obj_y and obj_land_y
 * = the player's obj_y), obj_vy $039E, obj_vx $0099, x += $FD02[dir], and
 * the next phase.
 * In: X = slot. Out: the RTS with the last test's flags, or A = obj_x.
 */
void func_90BA_b6(void)
{
    REGS_IN;
    int i;

    mff_cycles(0x90BA, 3);
    a = RAM(GAME_ACTION);
    i = cmp_list(0x90BC, 0x90EC, a, k_throw_cut, 3);
    if (i == 3) {
        mff_cycles(0x90C8, 8);
        a = RAM(OBJ_ANIM_FRAME);
        CMP_FLAGS(a, 0x03);
        if (!z) {
            mff_cycles(0x90CD, 8);              /* BNE into the next page; RTS */
            RTS_RETURN();
        }
        mff_cycles(0x90CF, 6);
        a = RAM(OBJ_ANIM_TIMER);
        SET_NZ(a);
        if (!z) {
            mff_cycles(0x90D2, 8);
            RTS_RETURN();
        }
        mff_cycles(0x90D4, 10);
        y = RAM(OBJ_KIND + x);
        a = prg_read((uint16_t)(KIND_THROWN_ANIM2 + y));
        SET_NZ(a);
        mff_cycles(0x90DB, 6);
        CALL_FIXED(0x90DB, func_F288);
        mff_cycles(0x90DE, 4);
        a = prg_read((uint16_t)(KIND_CHR_BANK + y));
        mff_cycles(0x90E1, 7);
        RAM(TMP0) = a;
        a = prg_read((uint16_t)(KIND_CHR_SLOT + y));
        mff_cycles(0x90E6, 10);
        y = a;
        a = RAM(TMP0);
        RAM(CHR_BANKS + 3 + y) = a;
    }
    mff_cycles(0x90EC, 49);
    RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + x) = a;
    RAM(OBJ_LAND_Y + x) = a;
    RAM(OBJ_VY_FRAC + x) = 0x9E;
    RAM(OBJ_VY + x) = 0x03;
    RAM(OBJ_VX_FRAC + x) = 0x99;
    RAM(OBJ_VX + x) = 0x00;
    SHIFT_X(0x910C, 0xFD02);
    mff_cycles(0x9127, 6);
    RTS_RETURN();
}

/*
 * Phase 4, $9128: on the floor. Over a pit (0D:A9F6 turns it into a pit
 * fall, C set; never executed) it gets obj_vy $FE00. Otherwise
 * obj_ground_y counts down; at 0 the slam hurts: 0C:9C55, attack 5
 * through obj_take_hit, player_no_hit cleared (the throw is over), and
 * phases 5-6 skipped to phase 6 ($917E). A slam that kills (never
 * executed) makes it id $16 at phase 0, turned around.
 * In: X = slot. Out: as phase 6, or the RTS.
 */
void func_9128_b6(void)
{
    REGS_IN;

    mff_cycles(0x9128, 6);
    CALL_BANK(0x9128, 0xA9F6);
    mff_cycles(0x912B, 2);
    if (c) {
        mff_cycles(0x912D, 20);
        RAM(OBJ_VY + x) = 0xFE;
        a = 0x00;
        SET_NZ(a);
        RAM(OBJ_VY_FRAC + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0x912B, 1);
    mff_cycles(0x9138, 6);
    a = RAM(OBJ_GROUND_Y + x);
    SET_NZ(a);
    if (!z) {
        mff_cycles(0x913B, 1);
        mff_cycles(0x916F, 13);
        t = (uint8_t)(RAM(OBJ_GROUND_Y + x) - 1);
        RAM(OBJ_GROUND_Y + x) = t;
        SET_NZ(t);
        RTS_RETURN();
    }
    mff_cycles(0x913D, 6);
    CALL_BANK(0x913D, 0x9C55);
    mff_cycles(0x9140, 18);
    RAM(SAVED_ATTACK_TYPE) = RAM(ATTACK_ID);
    a = ATTACK_LANDING;
    SET_NZ(a);
    RAM(ATTACK_ID) = a;
    CALL_BANK(0x9149, 0x84F0);                  /* obj_take_hit */
    mff_cycles(0x914C, 11);
    RAM(PLAYER_NO_HIT) = 0x00;
    a = RAM(OBJ_LIFE + x);
    if (a & 0x80) {
        mff_cycles(0x9155, 43);
        RAM(AREA_KILLS) = (uint8_t)(RAM(AREA_KILLS) + 1);
        RAM(OBJ_ATTACK + x) = 0x00;
        RAM(OBJ_STATE + x) = OBJ_LIVE;
        RAM(OBJ_ID + x) = 0x16;
        a = RAM(OBJ_DIR + x) ^ 0x03;
        SET_NZ(a);
        RAM(OBJ_DIR + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0x9153, 1);
    mff_cycles(0x9173, 21);
    RAM(ATTACK_ID) = RAM(SAVED_ATTACK_TYPE);
    RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    SYNC_OUT;
    func_917E_b6();
}

/*
 * Phase 6, $917E: unless obj_knockdown_recover has it getting up already
 * (obj_invuln $FE/$FD: JSR $9BFC, RTS), it moves back from its facing
 * (0F:F1A1 / F183) and falls ($9199).
 * In: X = slot. Out: as $9199 or obj_knockdown_recover.
 */
void func_917E_b6(void)
{
    REGS_IN;
    int i;

    mff_cycles(0x917E, 4);
    a = RAM(OBJ_INVULN + x);
    i = cmp_list(0x9181, 0x91B3, a, k_getting_up, 2);
    CMP_LIST_FLAGS(a, k_getting_up, 2, i);
    if (i < 2) {
        mff_cycles(0x91B3, 6);
        CALL_BANK(0x91B3, 0x9BFC);              /* obj_knockdown_recover */
        mff_cycles(0x91B6, 6);
        RTS_RETURN();
    }
    mff_cycles(0x9189, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    SET_NZ(a);
    if (!z) {
        mff_cycles(0x9190, 6);
        CALL_FIXED(0x9190, func_F1A1);
        JMP_TAIL(0x9193, 0x9199);
    }
    mff_cycles(0x918E, 1);
    mff_cycles(0x9196, 6);
    CALL_FIXED(0x9196, func_F183);
    SYNC_OUT;
    func_9199_b6();
}

/*
 * $9199: y -= obj_vy with gravity (0F:F1FD); at obj_land_y it lands:
 * obj_pit_check, obj_invuln = $FF, obj_knockdown_recover.
 * In: X = slot. Out: the RTS, or after obj_knockdown_recover.
 * Callers: JMP at 0C:9193; the fall-through from 0C:9196.
 */
void func_9199_b6(void)
{
    REGS_IN;

    mff_cycles(0x9199, 6);
    CALL_FIXED(0x9199, func_F1FD);
    mff_cycles(0x919C, 10);
    a = RAM(OBJ_Y + x);
    CMP_FLAGS(a, RAM(OBJ_LAND_Y + x));
    if (!c) {
        mff_cycles(0x91A4, 6);
        RTS_RETURN();
    }
    mff_cycles(0x91A2, 1);
    mff_cycles(0x91A5, 15);
    a = RAM(OBJ_LAND_Y + x);
    SET_NZ(a);
    RAM(OBJ_Y + x) = a;
    CALL_BANK(0x91AB, 0x9B1F);                  /* obj_pit_check */
    mff_cycles(0x91AE, 13);
    a = 0xFF;
    SET_NZ(a);
    RAM(OBJ_INVULN + x) = a;
    CALL_BANK(0x91B3, 0x9BFC);                  /* obj_knockdown_recover */
    mff_cycles(0x91B6, 6);
    RTS_RETURN();
}

/* ==================== Haggar's HeadSmasher, id $0E ==================== */

#define ATTACK_SMASH        0x13    /* the HeadSmasher's slam (0C:9276) */
#define PLAYER_ANIM_LIFT    0x48    /* the player's animations of the HeadSmasher: */
#define PLAYER_ANIM_SMASH   0x49    /* lifting the enemy, and slamming it */
#define KIND_HELD_ANIM      0xFC41  /* by kind: the animation held overhead ... */
#define KIND_HELD_CHR       0xFC4E  /* ... and its sprite bank */

/* game_action values while the held body sinks to the floor
 * ($91F4-$9206): none, hit (5, 6), the HeadSmasher itself, $24 */
static const uint8_t k_smash_hold_actions[] = { 0x06, 0x05, 0x12, 0x24 };

static void thrown_smash_fall(void);

/*
 * thrown_smash, $91B7 (name proposed): behaviour of id $0E, the enemy
 * Haggar lifts for the HeadSmasher. Its own animation is frozen
 * (obj_anim_timer = 0). Phase 0, held: when the player's animation is the
 * lift ($48) it gets the kind's held animation and sprite bank (once:
 * obj_invuln $FF, phase 1); while game_action is 0, 6, 5, $12 or $24 it
 * sinks to attack_ground_y (0F:F1FD, by a JMP) and on reaching it
 * obj_pit_check and 0C:9C29 run. Phase 1 (state bit 1 clear): at the
 * player's slam animation ($49), timer 0 or 4 (4 also loads obj_timer2
 * $10 and obj_timer3 8), the kind's thrown animation, facing turned, and
 * the slam ($9276); otherwise it follows thrown_smash_fall. Phases with
 * bit 1 set JMP $92F0.
 * In: X = slot. Out: as the path taken.
 * Caller: obj_behavior_lo/hi (id $0E).
 */
void func_91B7_b6(void)
{
    REGS_IN;
    int i;

    mff_cycles(0x91B7, 15);
    RAM(OBJ_ANIM_TIMER + x) = 0x00;
    a = RAM(OBJ_STATE + x) & OBJ_PHASE;
    if (a == 0) {
        mff_cycles(0x91C3, 8);
        a = RAM(OBJ_ANIM);                      /* slot 0, the player */
        if (a == PLAYER_ANIM_LIFT) {
            mff_cycles(0x91CA, 17);
            RAM(OBJ_TIMER3 + x) = 0x08;
            y = RAM(OBJ_KIND + x);
            a = prg_read((uint16_t)(KIND_HELD_ANIM + y));
            mff_cycles(0x91D6, 6);
            CMP_FLAGS(a, RAM(OBJ_ANIM + x));
            if (!z) {
                mff_cycles(0x91DB, 6);
                CALL_FIXED(0x91DB, func_F288);
                mff_cycles(0x91DE, 4);
                a = prg_read((uint16_t)(KIND_HELD_CHR + y));
                mff_cycles(0x91E1, 7);
                RAM(TMP0) = a;
                a = prg_read((uint16_t)(KIND_CHR_SLOT + y));
                mff_cycles(0x91E6, 24);
                y = a;
                a = RAM(TMP0);
                RAM(CHR_BANKS + 3 + y) = a;
                RAM(OBJ_INVULN + x) = 0xFF;
                RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
            } else {
                mff_cycles(0x91D9, 1);
            }
        } else {
            mff_cycles(0x91C8, 1);
        }

        mff_cycles(0x91F4, 5);
        a = RAM(GAME_ACTION);
        if (a == 0) {
            mff_cycles(0x91F6, 2);
        } else {
            i = cmp_list(0x91F8, 0x9209, a, k_smash_hold_actions, 3);
            if (i == 3) {
                /* $24: a CMP / BEQ the coverage map holds as data */
                mff_cycles(0x9204, 4);
                CMP_FLAGS(a, k_smash_hold_actions[3]);
                if (!z) {
                    mff_cycles(0x9208, 6);
                    RTS_RETURN();
                }
                mff_cycles(0x9206, 1);
            }
        }
        mff_cycles(0x9209, 18);
        a = RAM(ATTACK_GROUND_Y);
        RAM(OBJ_LAND_Y + x) = a;
        a = RAM(OBJ_Y + x);
        CMP_FLAGS(a, RAM(OBJ_LAND_Y + x));
        if (!c)
            JMP_FIXED(0x9216, 0xF1FD, func_F1FD);
        mff_cycles(0x9214, 1);
        mff_cycles(0x9219, 15);
        a = RAM(OBJ_LAND_Y + x);
        SET_NZ(a);
        RAM(OBJ_Y + x) = a;
        CALL_BANK(0x921F, 0x9B1F);              /* obj_pit_check */
        mff_cycles(0x9222, 6);
        CALL_BANK(0x9222, 0x9C29);
        mff_cycles(0x9225, 6);
        RTS_RETURN();
    }
    mff_cycles(0x91C1, 2);

    mff_cycles(0x9226, 8);
    a = RAM(OBJ_STATE + x) & 0x02;
    SET_NZ(a);
    if (!z)
        JMP_TAIL(0x922D, 0x92F0);
    mff_cycles(0x922B, 1);

    mff_cycles(0x9230, 8);
    a = RAM(OBJ_ANIM);
    if (a != PLAYER_ANIM_SMASH) {
        mff_cycles(0x9235, 1);
        SYNC_OUT;
        thrown_smash_fall();
        return;
    }
    mff_cycles(0x9237, 6);
    a = RAM(OBJ_ANIM_TIMER);
    if (a == 0) {
        mff_cycles(0x923A, 1);
    } else {
        mff_cycles(0x923C, 4);
        if (a != 0x04) {
            mff_cycles(0x923E, 1);
            SYNC_OUT;
            thrown_smash_fall();
            return;
        }
        mff_cycles(0x9240, 14);
        RAM(OBJ_TIMER2 + x) = 0x10;
        a = 0x08;
        RAM(OBJ_TIMER3 + x) = a;
    }
    SET_KIND_ANIM(0x924A, KIND_THROWN_ANIM2);
    mff_cycles(0x9266, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    SET_NZ(a);
    if (z) {
        mff_cycles(0x926D, 6);
        CALL_FIXED(0x926D, func_F090);
        JMP_TAIL(0x9270, 0x9276);
    }
    mff_cycles(0x926B, 1);
    mff_cycles(0x9273, 6);
    CALL_FIXED(0x9273, func_F099);
    SYNC_OUT;
    func_9276_b6();
}

/*
 * thrown_smash_slam, $9276 (name proposed): the slam hurts: attack $13
 * through obj_take_hit. Alive, it bounces up (phase + 1, obj_vy $039E,
 * obj_vx $0099, obj_land_y = obj_y) and $92F0 follows. Killed: exp_award,
 * its attack withdrawn, a kill counted, id $16 at phase 0, turned around,
 * obj_land_y = attack_ground_y, then thrown_smash_fall.
 * In: X = slot. Out: as $92F0 or thrown_smash_fall.
 * Callers: JMP at 0C:9270; the fall-through from 0C:9273.
 */
void func_9276_b6(void)
{
    REGS_IN;

    mff_cycles(0x9276, 11);
    a = ATTACK_SMASH;
    SET_NZ(a);
    RAM(ATTACK_ID) = a;
    CALL_BANK(0x927A, 0x84F0);                  /* obj_take_hit */
    mff_cycles(0x927D, 6);
    a = RAM(OBJ_LIFE + x);
    SET_NZ(a);
    if (!n) {
        mff_cycles(0x9280, 1);
        mff_cycles(0x92D3, 44);
        RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        RAM(OBJ_VY_FRAC + x) = 0x9E;
        RAM(OBJ_VY + x) = 0x03;
        RAM(OBJ_VX_FRAC + x) = 0x99;
        RAM(OBJ_VX + x) = 0x00;
        RAM(OBJ_LAND_Y + x) = RAM(OBJ_Y + x);
        SYNC_OUT;
        func_92F0_b6();
        return;
    }
    mff_cycles(0x9282, 6);
    CALL_BANK(0x9282, 0x89E3);                  /* exp_award */
    mff_cycles(0x9285, 45);
    RAM(OBJ_ATTACK + x) = 0x00;
    RAM(AREA_KILLS) = (uint8_t)(RAM(AREA_KILLS) + 1);
    RAM(OBJ_STATE + x) = OBJ_LIVE;
    RAM(OBJ_ID + x) = 0x16;
    RAM(OBJ_DIR + x) = RAM(OBJ_DIR + x) ^ 0x03;
    a = RAM(ATTACK_GROUND_Y);
    RAM(OBJ_LAND_Y + x) = a;
    SYNC_OUT;
    thrown_smash_fall();
}

/*
 * $92A3-$92D2: while the HeadSmasher (game_action $12) is on, wait (RTS);
 * then over a pit (0D:A9F6, never executed) obj_vy $FD00, otherwise fall
 * to attack_ground_y (0F:F1FD) and, reaching it (never executed), sit on
 * it and JMP $932E (the JMP obj_knockdown_recover there, interpreted).
 */
static void thrown_smash_fall(void)
{
    REGS_IN;

    mff_cycles(0x92A3, 7);
    a = RAM(GAME_ACTION);
    CMP_FLAGS(a, ACTION_HEADSMASHER);
    if (z) {
        mff_cycles(0x92A7, 7);                  /* BEQ taken; RTS */
        RTS_RETURN();
    }
    mff_cycles(0x92A9, 6);
    CALL_BANK(0x92A9, 0xA9F6);
    mff_cycles(0x92AC, 2);
    if (c) {
        mff_cycles(0x92AE, 20);
        RAM(OBJ_VY + x) = 0xFD;
        a = 0x00;
        SET_NZ(a);
        RAM(OBJ_VY_FRAC + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0x92AC, 1);
    mff_cycles(0x92B9, 18);
    a = RAM(ATTACK_GROUND_Y);
    RAM(OBJ_LAND_Y + x) = a;
    a = RAM(OBJ_Y + x);
    CMP_FLAGS(a, RAM(OBJ_LAND_Y + x));
    if (c) {
        mff_cycles(0x92C4, 1);
        mff_cycles(0x92CA, 9);
        a = RAM(OBJ_LAND_Y + x);
        SET_NZ(a);
        RAM(OBJ_Y + x) = a;
        JMP_TAIL(0x92D0, 0x932E);
    }
    mff_cycles(0x92C6, 6);
    CALL_FIXED(0x92C6, func_F1FD);
    mff_cycles(0x92C9, 6);
    RTS_RETURN();
}

/*
 * $92F0: the bounce. Unless getting up already (obj_invuln $FE/$FD: JMP
 * obj_knockdown_recover) it moves back from its facing and falls ($930B).
 * In: X = slot. Out: as $930B or obj_knockdown_recover.
 * Callers: JMP at 0C:922D (phases with bit 1); the fall-through from
 * thrown_smash_slam.
 */
void func_92F0_b6(void)
{
    REGS_IN;
    int i;

    mff_cycles(0x92F0, 4);
    a = RAM(OBJ_INVULN + x);
    i = cmp_list(0x92F3, 0x932E, a, k_getting_up, 2);
    CMP_LIST_FLAGS(a, k_getting_up, 2, i);
    if (i < 2)
        JMP_TAIL(0x932E, 0x9BFC);
    mff_cycles(0x92FB, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    SET_NZ(a);
    if (!z) {
        mff_cycles(0x9302, 6);
        CALL_FIXED(0x9302, func_F1A1);
        JMP_TAIL(0x9305, 0x930B);
    }
    mff_cycles(0x9300, 1);
    mff_cycles(0x9308, 6);
    CALL_FIXED(0x9308, func_F183);
    SYNC_OUT;
    func_930B_b6();
}

/*
 * $930B: at obj_anim_timer $14 0C:9C55 (never executed); y -= obj_vy with
 * gravity; at obj_land_y it lands: obj_pit_check, obj_invuln = $FF, JMP
 * obj_knockdown_recover.
 * In: X = slot. Out: the RTS, or as obj_knockdown_recover.
 * Callers: JMP at 0C:9305; the fall-through from 0C:9308.
 */
void func_930B_b6(void)
{
    REGS_IN;

    mff_cycles(0x930B, 8);
    a = RAM(OBJ_ANIM_TIMER + x);
    CMP_FLAGS(a, 0x14);
    if (z) {
        mff_cycles(0x9312, 6);
        CALL_BANK(0x9312, 0x9C55);
    } else {
        mff_cycles(0x9310, 1);
    }
    mff_cycles(0x9315, 6);
    CALL_FIXED(0x9315, func_F1FD);
    mff_cycles(0x9318, 10);
    a = RAM(OBJ_Y + x);
    CMP_FLAGS(a, RAM(OBJ_LAND_Y + x));
    if (!c) {
        mff_cycles(0x931E, 7);                  /* BCC taken; RTS */
        RTS_RETURN();
    }
    mff_cycles(0x9320, 15);
    a = RAM(OBJ_LAND_Y + x);
    SET_NZ(a);
    RAM(OBJ_Y + x) = a;
    CALL_BANK(0x9326, 0x9B1F);                  /* obj_pit_check */
    mff_cycles(0x9329, 7);
    a = 0xFF;
    SET_NZ(a);
    RAM(OBJ_INVULN + x) = a;
    JMP_TAIL(0x932E, 0x9BFC);
}

/* ======================= Guy's throw, id $0F ======================= */

#define SFX_GUY_THROW       0x33    /* the same sound as the back throw */

/*
 * The test each of Guy's steps starts with (the copy at `pc`): go on (BEQs
 * to pc + $16) when the player is down or at obj_anim_frame `val`;
 * otherwise RTS (pc + $15).
 */
#define GUY_STEP(pc, val) do { \
        int i_; \
        mff_cycles(pc, 3); \
        a = RAM(GAME_ACTION); \
        i_ = cmp_list((uint16_t)((pc) + 2), (uint16_t)((pc) + 0x16), a, k_throw_cut, 3); \
        if (i_ < 3) { \
            CMP_FLAGS(a, k_throw_cut[i_]); \
        } else { \
            mff_cycles((uint16_t)((pc) + 14), 8); \
            a = RAM(OBJ_ANIM_FRAME); \
            CMP_FLAGS(a, val); \
            if (!z) { \
                mff_cycles((uint16_t)((pc) + 0x15), 6); \
                RTS_RETURN(); \
            } \
            mff_cycles((uint16_t)((pc) + 0x13), \
                       branch_taken((uint16_t)((pc) + 0x13), (uint16_t)((pc) + 0x16))); \
        } \
    } while (0)

/*
 * The screen-edge clamp between Guy's steps (the copy at `pc`, $48
 * bytes; the facing-left side never ran in coverage). Facing right
 * (obj_dir bit 0) with keep_on_screen set, a body less than $10 px from
 * the screen's left edge is put on it and the step ends (JMP to the RTS
 * at pc + $47); otherwise JMP pc + $48, the step's move. Facing left the
 * same on the right edge ($F1, put at $F0, RTS), else on to pc + $48 by
 * falling into it, or (`via_jmp`, the copy at $94EA, whose BEQ goes back
 * to it) through the JMP at pc + $27.
 */
static void guy_step_clamp(uint16_t pc, int via_jmp, void (*next_fn)(void))
{
    REGS_IN;
    const uint16_t jmp = (uint16_t)(pc + 0x27), second = (uint16_t)(pc + 0x2A);
    const uint16_t rts = (uint16_t)(pc + 0x47), next = (uint16_t)(pc + 0x48);

    mff_cycles(pc, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    if (a != 0) {
        mff_cycles((uint16_t)(pc + 0x07), 5);
        a = RAM(KEEP_ON_SCREEN);
        SET_NZ(a);
        if (!z) {
            mff_cycles((uint16_t)(pc + 0x0B), 13);
            a = RAM(OBJ_X + x);
            c = 1;
            SBC_A(RAM(SCROLL_X));
            CMP_FLAGS(a, 0x10);
            if (!c) {
                mff_cycles((uint16_t)(pc + 0x15), 22);
                a = RAM(SCROLL_X);
                c = 0;
                ADC_A(0x10);
                RAM(OBJ_X + x) = a;
                a = RAM(AREA_SCREEN);
                ADC_A(0x00);
                SET_NZ(a);
                RAM(OBJ_X_HI + x) = a;
                JMP_TAIL((uint16_t)(pc + 0x24), rts);
            }
            mff_cycles((uint16_t)(pc + 0x13), branch_taken((uint16_t)(pc + 0x13), jmp));
        } else {
            mff_cycles((uint16_t)(pc + 0x09), branch_taken((uint16_t)(pc + 0x09), jmp));
        }
        JMP_TAIL(jmp, next);
    }
    mff_cycles((uint16_t)(pc + 0x05), branch_taken((uint16_t)(pc + 0x05), second));
    mff_cycles(second, 5);
    a = RAM(KEEP_ON_SCREEN);
    SET_NZ(a);
    if (z) {
        if (via_jmp) {
            mff_cycles((uint16_t)(second + 0x02), branch_taken((uint16_t)(second + 0x02), jmp));
            JMP_TAIL(jmp, next);
        }
        mff_cycles((uint16_t)(second + 0x02), branch_taken((uint16_t)(second + 0x02), next));
    } else {
        mff_cycles((uint16_t)(second + 0x04), 13);
        a = RAM(OBJ_X + x);
        c = 1;
        SBC_A(RAM(SCROLL_X));
        CMP_FLAGS(a, 0xF1);
        if (c) {
            mff_cycles((uint16_t)(second + 0x0E), 28);
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
        mff_cycles((uint16_t)(second + 0x0C), branch_taken((uint16_t)(second + 0x0C), next));
    }
    SYNC_OUT;
    next_fn();
}

/*
 * thrown_guy, $9332 (name proposed): behaviour of id $0F, the enemy Guy
 * throws. Phases ($9345/$934E): 0 $9357, 1 $93D1, 2 $9457, 3 $94D4,
 * 4 $9551, 5 $95CE, 6 $9602, 7 $968D, 8 $8F8D (id 4's blink).
 * In: X = slot. Out: as the phase.
 * Caller: obj_behavior_lo/hi (id $0F).
 */
void func_9332_b6(void)
{
    obj_phase_jump(0x9332, 0x9345, 0x934E);
}

/*
 * Phase 0, $9357: sound $33; obj_timer2 = $10, obj_timer3 = 8,
 * obj_land_y = obj_y, the next phase, 8 px up, the kind's thrown
 * animation and sprite bank, and x += $FD06[dir] ($FD0A[dir] with
 * obj_throw_flag set, never executed).
 * In: X = slot. Out: A = obj_x, N/Z from it.
 */
void func_9357_b6(void)
{
    REGS_IN;

    mff_cycles(0x9357, 8);
    a = SFX_GUY_THROW;
    SET_NZ(a);
    CALL_FIXED(0x9359, func_F34C);
    mff_cycles(0x935C, 43);
    RAM(OBJ_TIMER2 + x) = 0x10;
    RAM(OBJ_TIMER3 + x) = 0x08;
    RAM(OBJ_LAND_Y + x) = RAM(OBJ_Y + x);
    RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    a = RAM(OBJ_Y + x);
    c = 1;
    SBC_A(0x08);
    RAM(OBJ_Y + x) = a;
    SET_KIND_ANIM(0x9378, KIND_THROWN_ANIM);
    mff_cycles(0x9394, 6);
    a = RAM(OBJ_THROW_FLAG + x);
    if (a == 0) {
        SHIFT_X(0x9399, 0xFD06);
        mff_cycles(0x93B4, 6);
        RTS_RETURN();
    }
    mff_cycles(0x9397, 1);
    SHIFT_X(0x93B5, 0xFD0A);
    mff_cycles(0x93D0, 6);
    RTS_RETURN();
}

/*
 * Phase 1, $93D1: at the player's obj_anim_frame 3 (or the player down),
 * the edge clamp, then $942F.
 * In: X = slot. Out: the RTS, or as $942F.
 */
void func_93D1_b6(void)
{
    REGS_IN;

    GUY_STEP(0x93D1, 0x03);
    SYNC_OUT;
    guy_step_clamp(0x93E7, 0, func_942F_b6);
}

/*
 * $942F: 8 px up, the next phase, x += $FD0E[dir].
 * In: X = slot. Out: A = obj_x, N/Z from it.
 * Callers: JMP at 0C:940E; the fall-through from the clamp.
 */
void func_942F_b6(void)
{
    REGS_IN;

    mff_cycles(0x942F, 20);
    a = RAM(OBJ_Y + x);
    c = 1;
    SBC_A(0x08);
    RAM(OBJ_Y + x) = a;
    RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    SHIFT_X(0x943B, 0xFD0E);
    mff_cycles(0x9456, 6);
    RTS_RETURN();
}

/*
 * Phase 2, $9457: at frame 5 (or the player down), the edge clamp, then
 * $94B5. Phases 3 ($94D4, frame 7) and 4 ($9551, frame 9) do the same.
 * In: X = slot. Out: the RTS, or as the step's move.
 */
void func_9457_b6(void)
{
    REGS_IN;

    GUY_STEP(0x9457, 0x05);
    SYNC_OUT;
    guy_step_clamp(0x946D, 0, func_94B5_b6);
}

/*
 * $94B5, $9532, $95AF: the next phase and x += $FD12[dir] (three copies).
 * In: X = slot. Out: A = obj_x, N/Z from it.
 * Callers: the JMP at 0C:9494 / 9511 / 958E; the fall-through from the
 * clamps.
 */
static void guy_step_move(uint16_t pc)
{
    REGS_IN;

    mff_cycles(pc, 7);
    RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    SHIFT_X((uint16_t)(pc + 3), 0xFD12);
    mff_cycles((uint16_t)(pc + 0x1E), 6);
    RTS_RETURN();
}

void func_94B5_b6(void)
{
    guy_step_move(0x94B5);
}

void func_94D4_b6(void)
{
    REGS_IN;

    GUY_STEP(0x94D4, 0x07);
    SYNC_OUT;
    guy_step_clamp(0x94EA, 1, func_9532_b6);
}

void func_9532_b6(void)
{
    guy_step_move(0x9532);
}

void func_9551_b6(void)
{
    REGS_IN;

    GUY_STEP(0x9551, 0x09);
    SYNC_OUT;
    guy_step_clamp(0x9567, 0, func_95AF_b6);
}

void func_95AF_b6(void)
{
    guy_step_move(0x95AF);
}

/*
 * Phase 5, $95CE: let go: obj_vx = 2, obj_vy = $0101, the kind's second
 * thrown animation and sprite bank, the next phase.
 * In: X = slot. Out: N/Z from obj_state.
 */
void func_95CE_b6(void)
{
    REGS_IN;

    mff_cycles(0x95CE, 28);
    RAM(OBJ_VX_FRAC + x) = 0x00;
    RAM(OBJ_VX + x) = 0x02;
    RAM(OBJ_VY_FRAC + x) = 0x01;
    a = 0x01;
    RAM(OBJ_VY + x) = a;
    SET_KIND_ANIM(0x95E2, KIND_THROWN_ANIM2);
    mff_cycles(0x95FE, 13);
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    RAM(OBJ_STATE + x) = t;
    SET_NZ(t);
    RTS_RETURN();
}

/*
 * Phase 6, $9602: the flight. At obj_anim_timer $10 0C:9C55 runs; the
 * body moves back from its facing and falls ($965C).
 * In: X = slot. Out: as $965C.
 */
void func_9602_b6(void)
{
    REGS_IN;

    mff_cycles(0x9602, 8);
    a = RAM(OBJ_ANIM_TIMER + x);
    CMP_FLAGS(a, 0x10);
    if (z) {
        mff_cycles(0x9609, 6);
        CALL_BANK(0x9609, 0x9C55);
    } else {
        mff_cycles(0x9607, 1);
    }
    SYNC_OUT;
    obj_move_x_clamped(0x960C, 0, func_965C_b6);
}

/*
 * $965C: y -= obj_vy with gravity; falling and down to obj_land_y it
 * lands: obj_pit_check, a bounce (obj_vy $0238, obj_vx $0080) and phase 7
 * at once.
 * Callers: JMP at 0C:9636 (and the clamps); the fall-through from 0C:9659.
 */
void func_965C_b6(void)
{
    REGS_IN;

    mff_cycles(0x965C, 6);
    CALL_FIXED(0x965C, func_F1FD);
    mff_cycles(0x965F, 6);
    a = RAM(OBJ_VY + x);
    SET_NZ(a);
    if (!n) {
        mff_cycles(0x9662, 7);
        RTS_RETURN();
    }
    mff_cycles(0x9664, 10);
    a = RAM(OBJ_Y + x);
    CMP_FLAGS(a, RAM(OBJ_LAND_Y + x));
    if (!c) {
        mff_cycles(0x966C, 6);
        RTS_RETURN();
    }
    mff_cycles(0x966A, 1);
    mff_cycles(0x966D, 15);
    a = RAM(OBJ_LAND_Y + x);
    SET_NZ(a);
    RAM(OBJ_Y + x) = a;
    CALL_BANK(0x9673, 0x9B1F);                  /* obj_pit_check */
    mff_cycles(0x9676, 35);
    RAM(OBJ_VY_FRAC + x) = 0x38;
    RAM(OBJ_VY + x) = 0x02;
    RAM(OBJ_VX_FRAC + x) = 0x80;
    a = 0x00;
    RAM(OBJ_VX + x) = a;
    RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
    SYNC_OUT;
    func_968D_b6();
}

/*
 * Phase 7, $968D: the bounce. Getting up already (obj_invuln $FE/$FD): JMP
 * $96C5 -> JMP $9730 -> obj_knockdown_recover. Otherwise it moves back
 * from its facing and falls ($96EB).
 * In: X = slot. Out: as $96EB or obj_knockdown_recover.
 */
void func_968D_b6(void)
{
    REGS_IN;
    int i;

    mff_cycles(0x968D, 4);
    a = RAM(OBJ_INVULN + x);
    i = cmp_list(0x9690, 0x96C5, a, k_getting_up, 2);
    CMP_LIST_FLAGS(a, k_getting_up, 2, i);
    if (i < 2)
        JMP_TAIL(0x96C5, 0x9730);
    SYNC_OUT;
    obj_move_x_sides(0x9698, 0x96C8, 0, func_96EB_b6);
}

/*
 * $96EB: the second fall. On landing (as $965C) the landing hurts (attack 5
 * through obj_take_hit, the player's attack_id parked meanwhile). Killed:
 * exp_award, its attack withdrawn, the next phase, a kill counted, JMP
 * $9C7A. Alive (never executed): attack_id back, obj_invuln = $FF, JMP
 * obj_knockdown_recover.
 * Callers: JMP at 0C:96C2 (and the clamps); the fall-through from 0C:96E8.
 */
void func_96EB_b6(void)
{
    REGS_IN;

    mff_cycles(0x96EB, 6);
    CALL_FIXED(0x96EB, func_F1FD);
    mff_cycles(0x96EE, 6);
    a = RAM(OBJ_VY + x);
    SET_NZ(a);
    if (!n) {
        mff_cycles(0x96F1, 7);
        RTS_RETURN();
    }
    mff_cycles(0x96F3, 10);
    a = RAM(OBJ_Y + x);
    CMP_FLAGS(a, RAM(OBJ_LAND_Y + x));
    if (!c) {
        mff_cycles(0x96FB, 6);
        RTS_RETURN();
    }
    mff_cycles(0x96F9, 1);
    mff_cycles(0x96FC, 15);
    a = RAM(OBJ_LAND_Y + x);
    SET_NZ(a);
    RAM(OBJ_Y + x) = a;
    CALL_BANK(0x9702, 0x9B1F);                  /* obj_pit_check */
    mff_cycles(0x9705, 18);
    RAM(SAVED_ATTACK_TYPE) = RAM(ATTACK_ID);
    a = ATTACK_LANDING;
    SET_NZ(a);
    RAM(ATTACK_ID) = a;
    CALL_BANK(0x970E, 0x84F0);                  /* obj_take_hit */
    mff_cycles(0x9711, 6);
    a = RAM(OBJ_LIFE + x);
    SET_NZ(a);
    if (n) {
        mff_cycles(0x9716, 6);
        CALL_BANK(0x9716, 0x89E3);              /* exp_award */
        mff_cycles(0x9719, 19);
        a = 0x00;
        RAM(OBJ_ATTACK + x) = a;
        RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
        t = (uint8_t)(RAM(AREA_KILLS) + 1);
        RAM(AREA_KILLS) = t;
        SET_NZ(t);
        JMP_TAIL(0x9723, 0x9C7A);
    }
    mff_cycles(0x9714, 1);
    mff_cycles(0x9726, 14);
    RAM(ATTACK_ID) = RAM(SAVED_ATTACK_TYPE);
    a = 0xFF;
    SET_NZ(a);
    RAM(OBJ_INVULN + x) = a;
    JMP_TAIL(0x9730, 0x9BFC);
}

/*
 * $9730: JMP obj_knockdown_recover (the end of $96EB, and the target of
 * the JMP at 0C:96C5).
 */
void func_9730_b6(void)
{
    mff_jmp_charge(0x9730, 0x9BFC);
    call_by_address_tail(0x9BFC, 6);
}
