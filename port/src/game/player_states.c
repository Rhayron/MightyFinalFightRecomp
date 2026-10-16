/*
 * player_states.c - player_state handlers that do not fit the jump, attack,
 * grab or hurt files (player_state dispatches on game_action through
 * 0E:C9CA/C9F5)
 */
#include "player_defs.h"

/* game_action of the move Guy starts with B inside his jump window */
#define WINDOW_ACTION       0xE24C   /* 23 18 00 */
/* player_no_hit for it */
#define WINDOW_NO_HIT       0xFCC5   /* FF FF 00 */

/*
 * player_state_land ($D2B7; labels.toml player_state_02, proposed name):
 * game_action 2, the frames after a jump lands (player_jump_land sets
 * action_timer 3) or Haggar's finisher (8). player_state dispatches here
 * through its table (entry 2).
 *  - action_timer counts down while it is not negative; then
 *  - Haggar, or anyone with guy_jump_window 0: back to idle. game_action,
 *    jump_moving, combo_timer, attack_live, player_invuln, grab_hit_count and
 *    slot 0's obj_timer, obj_timer2 and obj_ground_y go to 0, the player
 *    stands (player_stand, $CEAA) and the combo restarts
 *    (player_stand_anim = $E288[player_id]).
 *  - Guy with his jump window open: it counts down, player_ground_y follows
 *    obj_y, and B starts game_action $E24C[player_id] (Guy $18) with
 *    player_no_hit $FCC5[player_id] and obj_state[0] $80 (0E:D2F8; never
 *    executed in any run).
 * Out: A = player_stand_anim after idle.
 */
void func_D2B7(void) {
    REGS_IN;

    mff_cycles(0xD2B7, 3 + 2);
    a = RAM(ACTION_TIMER);
    if (!(a & 0x80)) {
        mff_cycles(0xD2BB, 5 + 6);
        RAM(ACTION_TIMER)--;
        SET_NZ(RAM(ACTION_TIMER));
        RTS();
    }
    mff_cycles(0xD2B9, 1 + 3 + 2 + 2);
    a = RAM(PLAYER_ID);
    COMPARE(a, PLAYER_HAGGAR);
    if (z) {
        mff_cycles(0xD2C2, 1);
    } else {
        mff_cycles(0xD2C4, 3 + 2);
        a = RAM(GUY_JUMP_WINDOW);
        if (a != 0) {
            mff_cycles(0xD2C6, 1 + 5 + 4 + 3 + 3 + 2 + 2);
            RAM(GUY_JUMP_WINDOW)--;
            a = RAM(OBJ_Y);
            RAM(PLAYER_GROUND_Y) = a;
            a = RAM(JOY_PRESSED);
            COMPARE(a, PAD_B);
            if (!z) {
                mff_cycles(0xD2F7, 6);
                RTS();
            }
            mff_cycles(0xD2F5, 1 + 3 + 4);
            y = RAM(PLAYER_ID);
            a = prg_read((uint16_t)(WINDOW_ACTION + y));
            mff_cycles(0xD2FD, 3 + 4);
            RAM(GAME_ACTION) = a;
            a = prg_read((uint16_t)(WINDOW_NO_HIT + y));
            mff_cycles(0xD302, 3 + 2 + 4 + 4 + 3 + 6);
            RAM(PLAYER_NO_HIT) = a;
            a = STATE_LIVE;
            RAM(OBJ_STATE) = a;
            a = RAM(OBJ_Y);
            SET_NZ(a);
            RAM(PLAYER_GROUND_Y) = a;
            RTS();
        }
    }

    /* back to idle */
    mff_cycles(0xD2C8, 2 + 6 * 3 + 3 * 4 + 6);
    a = ACT_IDLE;
    SET_NZ(a);
    RAM(GAME_ACTION) = a;
    RAM(JUMP_MOVING) = a;
    RAM(COMBO_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(PLAYER_INVULN) = a;
    RAM(GRAB_HIT_COUNT) = a;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER2) = a;
    RAM(OBJ_GROUND_Y) = a;
    JSR_AT(0xD2DF, 0xCEAA, -1);                 /* player_stand */
    mff_cycles(0xD2E2, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(COMBO_FIRST_ANIM + y));
    SET_NZ(a);
    mff_cycles(0xD2E7, 3 + 6);
    RAM(PLAYER_STAND_ANIM) = a;
    RTS();
}

/*
 * player_state_0A ($D45A, labels.toml): game_action $0A, set by boss code in
 * bank $03 when it holds the player (03:B912, 03:BA11). The player only
 * stands (player_stand, $CEAA) each frame. ($D450-$D459 before it look like
 * code, attack_live, game_action and combo_timer cleared and a branch here,
 * but nothing reaches them.)
 */
void func_D45A(void) {
    REGS_IN;

    mff_cycles(0xD45A, 6);
    JSR_AT(0xD45A, 0xCEAA, -1);                 /* player_stand */
    mff_cycles(0xD45D, 6);
    RTS();
}
