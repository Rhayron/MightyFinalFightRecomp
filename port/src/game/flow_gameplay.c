/*
 * flow_gameplay.c - three steps of the gameplay frame
 *
 * gameplay (0E:C8ED, gameplay.c) runs once a frame and ends at
 * gameplay_frame_end (0E:C9A7, gameplay.c). These steps of it live in this
 * part of the fixed bank: gameplay_update, the game's work for the frame;
 * the special screens, which replace the frame while special_screen_id is
 * set; and the drawing of the objects that ends a normal frame. The last
 * two go on into gameplay_frame_end by calling it, where the 6502 falls
 * through.
 *
 * The lifter put a zero-cycle charge at $C9A7 before that call
 * (mff_cycles(0xC9A7, 0)). The runtime clocks it as one cycle
 * (maybe_trigger_vblank charges at least one); it stays as it is.
 */
#include "flow.h"

void func_C99D(void);
void func_C9A7(void);   /* gameplay_frame_end, gameplay.c */
void func_E2A3(void);   /* camera_update, stage_camera.c */
void func_E6AB(void);   /* hud_update, stage_hud.c */

#define BANKS_OBJECTS       0x0C    /* 0C/0D: objects_update ($8000) and the behaviours */

/*
 * gameplay_update ($C93B, labels.toml)
 * The game's work for one frame, in this order:
 *  - player_state ($C9B6): the handler of game_action;
 *  - banks 00/01: player_action_dispatch (00:8000), the actions from $0F up;
 *  - camera_update ($E2A3); then player_x_prev = obj_x[0] and
 *    scroll_x_prev = scroll_x, what the next frame's camera and column
 *    streamer compare with;
 *  - banks 08/09: stage_spawns (08:8080), stage_events (08:8900) and the
 *    background palette animation (09:A300);
 *  - banks 0C/0D: objects_update (0C:8000), every other slot;
 *  - hud_update ($E6AB, tail call to gameplay_draw after it), except in
 *    game_action 7 and 9 (the continue screen and the way back from it),
 *    which go to gameplay_draw ($C99D) at once.
 * Callers: gameplay by JMP and branches (0E:C8F3, C90E-C91E, C936;
 * gameplay.c keeps its own copy of this code).
 */
void func_C93B(void) {
    REGS_IN;

    mff_cycles(0xC93B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x093D, 0xC9B6, -1)) return;          /* player_state */
    SYNC_IN;
    mff_cycles(0xC93E, 2 + 3 + 2 + 3 + 6);
    x = BANKS_PLAYER;
    RAM(BANK_REQUEST) = x;
    x++;
    SET_NZ(x);
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0947, func_FF4D, 0xE000)) return;   /* bank_switch */
    SYNC_IN;
    mff_cycles(0xC948, 6);
    SYNC_OUT;
    if (!mff_call_at(0x094A, 0x8000, -1)) return;          /* 00:8000 player_action_dispatch */
    SYNC_IN;
    mff_cycles(0xC94B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x094D, func_E2A3, 0xE000)) return;   /* camera_update */
    SYNC_IN;
    mff_cycles(0xC94E, 4 + 3 + 3 + 3 + 2 + 3 + 2 + 3 + 6);
    a = RAM(OBJ_X);
    RAM(PLAYER_X_PREV) = a;
    a = RAM(SCROLL_X);
    RAM(SCROLL_X_PREV) = a;
    x = BANKS_STAGE;
    RAM(BANK_REQUEST) = x;
    x++;
    SET_NZ(x);
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0960, func_FF4D, 0xE000)) return;   /* bank_switch */
    SYNC_IN;
    mff_cycles(0xC961, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0963, 0x8080, -1)) return;          /* 08:8080 stage_spawns */
    SYNC_IN;
    mff_cycles(0xC964, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0966, 0x8900, -1)) return;          /* 08:8900 stage_events */
    SYNC_IN;
    mff_cycles(0xC967, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0969, 0xA300, -1)) return;          /* 09:A300 palette animation */
    SYNC_IN;
    mff_cycles(0xC96A, 2 + 3 + 2 + 3 + 6);
    x = BANKS_OBJECTS;
    RAM(BANK_REQUEST) = x;
    x++;
    SET_NZ(x);
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0973, func_FF4D, 0xE000)) return;   /* bank_switch */
    SYNC_IN;
    mff_cycles(0xC974, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0976, 0x8000, -1)) return;          /* 0C:8000 objects_update */
    SYNC_IN;
    mff_cycles(0xC977, 3 + 2 + 2);
    a = RAM(GAME_ACTION);
    c = a >= 0x07;
    if (a != 0x07) {
        mff_cycles(0xC97D, 2 + 2);
        c = a >= 0x09;
        SET_NZ(a - 0x09);
        if (!z) {
            mff_cycles(0xC981, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0983, func_E6AB, 0xE000)) return;   /* hud_update */
            SYNC_IN;
            mff_cycles(0xC984, 3);                          /* JMP gameplay_draw */
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xC99D, 2);
            call_by_address_tail(0xC99D, -1);
            return;
        }
        mff_cycles(0xC97F, 1);
    } else {
        mff_cycles(0xC97B, 1);
    }
    SYNC_OUT;
    func_C99D();                                            /* gameplay_draw */
}

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
