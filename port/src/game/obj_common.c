/*
 * obj_common.c - code the behaviours of banks $0C/$0D repeat word for word
 * at several addresses: the jump on the behaviour's phase and the
 * clamped horizontal move. Each helper takes the address of the copy it
 * stands for, so the cycles are charged at the instructions of that copy.
 */
#include "obj_defs.h"

/*
 * JMP through a behaviour's phase table: LDA obj_state,X / AND #$0F / TAY /
 * LDA lo,Y / STA $00 / LDA hi,Y / STA $01 / JMP ($0000) at `pc`.
 * Out: TMP0/TMP1 = the phase routine, Y = the phase, A = its high byte.
 * Copies: 0C:8C1C, 8FA5, 9332 (the thrown bodies) and more.
 */
void obj_phase_jump(uint16_t pc, uint16_t lo, uint16_t hi)
{
    REGS_IN;

    mff_cycles(pc, 12);
    a = RAM(OBJ_STATE + x) & OBJ_PHASE;
    y = a;
    a = prg_read_ix(lo, y);
    mff_cycles((uint16_t)(pc + 9), 7);
    RAM(TMP0) = a;
    a = prg_read_ix(hi, y);
    SET_NZ(a);
    mff_cycles((uint16_t)(pc + 14), 8);
    RAM(TMP1) = a;
    JMP_INDIRECT(0x0000);
}

/*
 * The blink of a dead object (the copy at `pc`, $18 bytes): obj_timer2
 * counts down; at 0 the slot is cleared (JMP object_clear at pc + $15);
 * otherwise it is hidden on even counts (0C:9B62) and shown on odd ones
 * (0C:9B6B). After the hide a BNE (always taken: 9B62 leaves Z clear)
 * goes to the RTS at `hide_rts`.
 * Copies: 0C:8F8D, 994A (hide_rts = pc + $14), 9B07 (hide_rts = $9B03).
 */
void obj_blink(uint16_t pc, uint16_t hide_rts)
{
    REGS_IN;

    mff_cycles(pc, 13);
    RAM(OBJ_TIMER2 + x) = (uint8_t)(RAM(OBJ_TIMER2 + x) - 1);
    a = RAM(OBJ_TIMER2 + x);
    SET_NZ(a);
    if (z) {
        mff_cycles((uint16_t)(pc + 0x06), branch_taken((uint16_t)(pc + 0x06), (uint16_t)(pc + 0x15)));
        JMP_FIXED((uint16_t)(pc + 0x15), 0xF2BE, func_F2BE);
    }
    mff_cycles((uint16_t)(pc + 0x08), 4);
    a &= 0x01;
    SET_NZ(a);
    if (z) {
        mff_cycles((uint16_t)(pc + 0x0C), 6);
        CALL_BANK((uint16_t)(pc + 0x0C), 0x9B62);
        mff_cycles((uint16_t)(pc + 0x0F), 2);
        if (!z) {
            mff_cycles((uint16_t)(pc + 0x0F), branch_taken((uint16_t)(pc + 0x0F), hide_rts));
            mff_cycles(hide_rts, 6);
            RTS_RETURN();
        }
    } else {
        mff_cycles((uint16_t)(pc + 0x0A), branch_taken((uint16_t)(pc + 0x0A), (uint16_t)(pc + 0x11)));
    }
    mff_cycles((uint16_t)(pc + 0x11), 6);
    CALL_BANK((uint16_t)(pc + 0x11), 0x9B6B);
    mff_cycles((uint16_t)(pc + 0x14), 6);
    RTS_RETURN();
}

/*
 * The horizontal move by obj_vx with the screen-edge clamp (the copy at
 * `pc`, $50 bytes, followed by `next_fn` at pc + $50). When obj_dir bit 0
 * (right) is set the first side runs: left (0F:F1A1) for the thrown
 * bodies' copies (bit0_right 0), right (0F:F183) for the dying ones
 * (bit0_right 1); otherwise the other side. With keep_on_screen ($BC) set,
 * an object past the edge it moves toward (less than $10 px from the left
 * of the screen, $F1 or more from it on the right) is put on the edge
 * ($10 or $F0 from scroll_x, area_screen as the high byte) instead of
 * moving; that never ran in coverage. The first side and the clamps then
 * JMP to pc + $50; the second side falls into it.
 * Copies: 0C:8E64, 8EF5, 960C (bit0_right 0); 9794, 99AC, 9A88 (1); and
 * 0C:9698, 988B with the sides further apart (obj_move_x_sides).
 */
void obj_move_x_clamped(uint16_t pc, int bit0_right, void (*next_fn)(void))
{
    obj_move_x_sides(pc, (uint16_t)(pc + 0x2D), bit0_right, next_fn);
}

/*
 * The same move where the second side starts at `second` rather than
 * pc + $2D (0C:9698 has a JMP between the sides, second = $96C8); `next`
 * is second + $23.
 */
void obj_move_x_sides(uint16_t pc, uint16_t second, int bit0_right, void (*next_fn)(void))
{
    REGS_IN;
    uint16_t at, next = (uint16_t)(second + 0x23);
    int first, left;

    mff_cycles(pc, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    first = a != 0;
    if (first) {
        at = (uint16_t)(pc + 0x07);
    } else {
        mff_cycles((uint16_t)(pc + 0x05), branch_taken((uint16_t)(pc + 0x05), second));
        at = second;
    }
    left = first ? !bit0_right : bit0_right;

    mff_cycles(at, 5);
    a = RAM(KEEP_ON_SCREEN);
    SET_NZ(a);
    if (z) {
        mff_cycles((uint16_t)(at + 0x02), branch_taken((uint16_t)(at + 0x02), (uint16_t)(at + 0x20)));
    } else {
        mff_cycles((uint16_t)(at + 0x04), 13);
        a = RAM(OBJ_X + x);
        c = 1;
        SBC_A(RAM(SCROLL_X));
        CMP_FLAGS(a, left ? 0x10 : 0xF1);
        if (left ? !c : c) {
            mff_cycles((uint16_t)(at + 0x0E), 22);
            a = RAM(SCROLL_X);
            c = 0;
            ADC_A(left ? 0x10 : 0xF0);
            RAM(OBJ_X + x) = a;
            a = RAM(AREA_SCREEN);
            ADC_A(0x00);
            SET_NZ(a);
            RAM(OBJ_X_HI + x) = a;
            JMP_TAIL((uint16_t)(at + 0x1D), next);
        }
        mff_cycles((uint16_t)(at + 0x0C), branch_taken((uint16_t)(at + 0x0C), (uint16_t)(at + 0x20)));
    }
    mff_cycles((uint16_t)(at + 0x20), 6);
    if (left)
        CALL_FIXED((uint16_t)(at + 0x20), func_F1A1);
    else
        CALL_FIXED((uint16_t)(at + 0x20), func_F183);
    if (first)
        JMP_TAIL((uint16_t)(at + 0x23), next);
    SYNC_OUT;
    next_fn();
}
