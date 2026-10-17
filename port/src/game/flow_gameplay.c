/*
 * flow_gameplay.c - two steps of the gameplay frame
 *
 * gameplay (0E:C8ED, gameplay.c) runs once a frame and ends at
 * gameplay_frame_end (0E:C9A7, gameplay.c). These two of its steps live in
 * this part of the fixed bank: the special screens, which replace the
 * frame while special_screen_id is set, and the drawing of the objects
 * that ends a normal frame. Both go on into gameplay_frame_end by calling
 * it, where the 6502 falls through.
 *
 * The lifter put a zero-cycle charge at $C9A7 before that call
 * (mff_cycles(0xC9A7, 0)). The runtime clocks it as one cycle
 * (maybe_trigger_vblank charges at least one); it stays as it is.
 */
#include "flow.h"

void func_C99D(void);
void func_C9A7(void);   /* gameplay_frame_end, gameplay.c */

/*
 * gameplay_special_screen ($C987, labels.toml)
 * While special_screen_id is not 0 gameplay JMPs here instead of updating
 * the game (0E:C901): banks 04/05, then 04:9800 runs the special screen
 * (the text pages of the boss talk; $F0-$FF are commands). The objects
 * are drawn only while slot 12's obj_kind is 7; either way the frame ends
 * at gameplay_frame_end.
 * Caller: gameplay by JMP (0E:C901; gameplay.c tail-calls it).
 */
void func_C987(void) {
    REGS_IN;

    mff_cycles(0xC987, 16);
    x = BANKS_SCREENS;
    RAM(BANK_REQUEST) = x;
    x++;
    SET_NZ(x);
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0990, func_FF4D, 0xE000)) return;   /* bank_switch */
    SYNC_IN;
    mff_cycles(0xC991, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0993, 0x9800, -1)) return;          /* 04:9800 the special screen */
    SYNC_IN;
    mff_cycles(0xC994, 8);
    a = RAM(OBJ_KIND + 12);
    c = a >= 7;
    SET_NZ(a - 7);
    if (z) {
        mff_cycles(0xC999, 1);                              /* BEQ gameplay_draw */
        SYNC_OUT;
        func_C99D();
        return;
    }
    mff_cycles(0xC99B, 2);
    mff_cycles(0xC99B, 1);                                  /* BNE $C9A7, always taken */
    mff_cycles(0xC9A7, 0);
    SYNC_OUT;
    func_C9A7();
}

/*
 * gameplay_draw ($C99D, labels.toml)
 * oam_index = 4 and sprites_hide_rest ($C31D) hide every sprite but
 * sprite 0, then objects_draw fills the buffer again from entry 1; then
 * gameplay_frame_end.
 * Callers: gameplay_update by BEQ and JMP (0E:C97D, C981, C984; gameplay.c
 * tail-calls it) and gameplay_special_screen.
 */
void func_C99D(void) {
    REGS_IN;

    mff_cycles(0xC99D, 11);
    a = 0x04;
    SET_NZ(a);
    RAM(OAM_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_at(0x09A3, 0xC31D, -1)) return;          /* sprites_hide_rest */
    SYNC_IN;
    mff_cycles(0xC9A4, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x09A6, func_E9BC, 0xE000)) return;   /* objects_draw */
    SYNC_IN;
    mff_cycles(0xC9A7, 0);
    SYNC_OUT;
    func_C9A7();
}
