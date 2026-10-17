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
    a = prg_read((uint16_t)(lo + y));
    mff_cycles((uint16_t)(pc + 9), 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(hi + y));
    SET_NZ(a);
    mff_cycles((uint16_t)(pc + 14), 8);
    RAM(TMP1) = a;
    JMP_INDIRECT(0x0000);
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
 * Copies: 0C:8E64, 8EF5, 960C, 9794 (bit0_right 0); 99AC, 9A88 (1).
 */
void obj_move_x_clamped(uint16_t pc, int bit0_right, void (*next_fn)(void))
{
    REGS_IN;
    uint16_t at, next = (uint16_t)(pc + 0x50);
    int first, left;

    mff_cycles(pc, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    first = a != 0;
    if (first) {
        at = (uint16_t)(pc + 0x07);
    } else {
        mff_cycles((uint16_t)(pc + 0x05), branch_taken((uint16_t)(pc + 0x05), (uint16_t)(pc + 0x2D)));
        at = (uint16_t)(pc + 0x2D);
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
