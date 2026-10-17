/*
 * flow_gameplay.c - the gameplay frame (docs/engine.md, "The gameplay
 * frame"), 0E:C8ED-C9B5
 *
 * gameplay ($C8ED) is task 0's loop from a new game on: once a frame it
 * either skips the game (the continue screen, after the last round, the
 * special screens, a pause) or runs gameplay_update ($C93B), then
 * gameplay_draw ($C99D) and gameplay_frame_end ($C9A7), which waits for
 * the next frame and goes round again.
 *
 * The ROM runs these as one stretch of code that its routines JMP and
 * branch into. The scaffold made a function of each entry, and the code
 * that follows an entry runs in that function: gameplay's own loop does
 * gameplay_update, the drawing and the frame end inline and loops with no
 * extra cost, while reaching an entry by JMP is a tail call that costs 2
 * cycles more. So each part is written once as a *_work helper and the
 * entries put them together as their copies did.
 *
 * The lifter put a zero-cycle charge at $C9A7 where gameplay_draw and
 * gameplay_special_screen go on into gameplay_frame_end
 * (mff_cycles(0xC9A7, 0)). The runtime clocks it as one cycle
 * (maybe_trigger_vblank charges at least one); it stays as it is. The
 * same step inside gameplay's loop has no such charge.
 */
#include "flow.h"

void func_C99D(void);
void func_C9A7(void);
void func_E2A3(void);   /* camera_update, stage_camera.c */
void func_E6AB(void);   /* hud_update, stage_hud.c */

#define BANKS_OBJECTS       0x0C    /* 0C/0D: objects_update ($8000) and the behaviours */

/* game_action values the loop tests */
#define ACT_SELECT          0x07    /* back from the continue screen */
#define ACT_CONTINUE        0x09    /* the continue screen */
/* actions during which START does not pause (bank $00's actions) */
static const uint8_t k_no_pause_actions[] = { 0x1B, 0x1C, 0x1E, 0x19, 0x28 };

#define SFX_PAUSE           0x15    /* then $F3 (pause the music) or $F4 (resume) */
#define SOUND_MUSIC_PAUSE   0xF3

/* what gameplay_update_work leaves to its caller */
#define GU_UNWOUND          0       /* a callee left through the scheduler: return at once */
#define GU_DRAW             1       /* game_action 7 or 9: BEQ gameplay_draw (charged) */
#define GU_DRAW_JMP         2       /* after hud_update: JMP gameplay_draw (3 cycles charged) */

/*
 * $C93B-$C984, the body of gameplay_update. The game's work for one frame,
 * in this order:
 *  - player_state ($C9B6): the handler of game_action;
 *  - banks 00/01: player_action_dispatch (00:8000), the actions from $0F up;
 *  - camera_update ($E2A3); then player_x_prev = obj_x[0] and
 *    scroll_x_prev = scroll_x, what the next frame's camera and column
 *    streamer compare with;
 *  - banks 08/09: stage_spawns (08:8080), stage_events (08:8900) and the
 *    background palette animation (09:A300);
 *  - banks 0C/0D: objects_update (0C:8000), every other slot;
 *  - hud_update ($E6AB), except in game_action 7 and 9 (the continue screen
 *    and the way back from it).
 * Registers in and out through g_cpu.
 */
static int gameplay_update_work(void) {
    REGS_IN;

    mff_cycles(0xC93B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x093D, 0xC9B6, -1)) return GU_UNWOUND;       /* player_state */
    SYNC_IN;
    mff_cycles(0xC93E, 2 + 3 + 2 + 3 + 6);
    x = BANKS_PLAYER;
    RAM(BANK_REQUEST) = x;
    x++;
    SET_NZ(x);
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0947, func_FF4D, 0xE000)) return GU_UNWOUND;    /* bank_switch */
    SYNC_IN;
    mff_cycles(0xC948, 6);
    SYNC_OUT;
    if (!mff_call_at(0x094A, 0x8000, -1)) return GU_UNWOUND;       /* 00:8000 player_action_dispatch */
    SYNC_IN;
    mff_cycles(0xC94B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x094D, func_E2A3, 0xE000)) return GU_UNWOUND;    /* camera_update */
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
    if (!mff_call_fn(0x0960, func_FF4D, 0xE000)) return GU_UNWOUND;    /* bank_switch */
    SYNC_IN;
    mff_cycles(0xC961, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0963, 0x8080, -1)) return GU_UNWOUND;       /* 08:8080 stage_spawns */
    SYNC_IN;
    mff_cycles(0xC964, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0966, 0x8900, -1)) return GU_UNWOUND;       /* 08:8900 stage_events */
    SYNC_IN;
    mff_cycles(0xC967, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0969, 0xA300, -1)) return GU_UNWOUND;       /* 09:A300 palette animation */
    SYNC_IN;
    mff_cycles(0xC96A, 2 + 3 + 2 + 3 + 6);
    x = BANKS_OBJECTS;
    RAM(BANK_REQUEST) = x;
    x++;
    SET_NZ(x);
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0973, func_FF4D, 0xE000)) return GU_UNWOUND;    /* bank_switch */
    SYNC_IN;
    mff_cycles(0xC974, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0976, 0x8000, -1)) return GU_UNWOUND;       /* 0C:8000 objects_update */
    SYNC_IN;
    mff_cycles(0xC977, 3 + 2 + 2);
    a = RAM(GAME_ACTION);
    c = a >= ACT_SELECT;
    if (a == ACT_SELECT) {
        mff_cycles(0xC97B, 1);
        SYNC_OUT;
        return GU_DRAW;
    }
    mff_cycles(0xC97D, 2 + 2);
    c = a >= ACT_CONTINUE;
    SET_NZ(a - ACT_CONTINUE);
    if (z) {
        mff_cycles(0xC97F, 1);
        SYNC_OUT;
        return GU_DRAW;
    }
    mff_cycles(0xC981, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0983, func_E6AB, 0xE000)) return GU_UNWOUND;    /* hud_update */
    mff_cycles(0xC984, 3);                                  /* JMP gameplay_draw */
    return GU_DRAW_JMP;
}

/* $C99D-$C9A6, the body of gameplay_draw: oam_index = 4 and
 * sprites_hide_rest ($C31D) hide every sprite but sprite 0, then
 * objects_draw ($E9BC) fills the buffer again from entry 1. 0 if a callee
 * left through the scheduler. Registers in and out through g_cpu. */
static int gameplay_draw_work(void) {
    REGS_IN;

    mff_cycles(0xC99D, 2 + 3 + 6);
    a = 0x04;
    SET_NZ(a);
    RAM(OAM_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_at(0x09A3, 0xC31D, -1)) return 0;        /* sprites_hide_rest */
    mff_cycles(0xC9A4, 6);
    return mff_call_fn(0x09A6, func_E9BC, 0xE000);         /* objects_draw */
}

/* $C9A7-$C9B3, the body of gameplay_frame_end: rng_step ($C3B4), then
 * render_off_depth 0 while task_wait_frame ($FF31) waits for the next
 * frame, back to 1 after it; the JMP gameplay that follows is charged
 * here. 0 if rng_step left through the scheduler. Registers in and out
 * through g_cpu. */
static int gameplay_frame_end_work(void) {
    REGS_IN;

    mff_cycles(0xC9A7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x09A9, 0xC3B4, -1)) return 0;        /* rng_step */
    SYNC_IN;
    mff_cycles(0xC9AA, 2 + 3 + 6);
    a = 0;
    SET_NZ(a);
    RAM(RENDER_OFF_DEPTH) = a;
    SYNC_OUT;
    mff_call_fn(0x09B0, func_FF31, 0xE000);                 /* task_wait_frame */
    SYNC_IN;
    mff_cycles(0xC9B1, 5 + 3);
    RAM(RENDER_OFF_DEPTH)++;
    SET_NZ(RAM(RENDER_OFF_DEPTH));
    SYNC_OUT;
    return 1;
}

/*
 * gameplay ($C8ED, labels.toml): task 0's loop from a new game on (JMP from
 * the new game, 0E:C8EA, and from the round starts in player_round.c), once
 * a frame:
 *  - game_action 9, the continue screen: gameplay_update only (JMP $C93B,
 *    tail call), which runs player_state_continue;
 *  - after the last round (game_finished): only the frame end (JMP $C9A7,
 *    tail call);
 *  - special_screen_id set: gameplay_special_screen (JMP $C987, tail call);
 *  - START, except during the actions of k_no_pause_actions: sound $15,
 *    paused toggles, and sound $F3 (pause) or $F4 (resume) for the music;
 *  - paused: only the frame end (JMP $C9A7, tail call);
 *  - otherwise gameplay_update, gameplay_draw (JMP after hud_update, tail
 *    call; inline for game_action 7) and the frame end, and round again.
 */
void func_C8ED(void) {
    REGS_IN;
    int i, r;

    for (;;) {
        mff_cycles(0xC8ED, 3 + 2 + 2);
        a = RAM(GAME_ACTION);
        c = a >= ACT_CONTINUE;
        SET_NZ(a - ACT_CONTINUE);
        if (z) {
            mff_cycles(0xC8F3, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xC93B, 2); call_by_address_tail(0xC93B, -1); return;
        }
        mff_cycles(0xC8F1, 1 + 3 + 2);
        a = RAM(GAME_FINISHED);
        SET_NZ(a);
        if (!z) {
            mff_cycles(0xC8FA, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xC9A7, 2); call_by_address_tail(0xC9A7, -1); return;
        }
        mff_cycles(0xC8F8, 1 + 3 + 2);
        a = RAM(SPECIAL_SCREEN_ID);
        SET_NZ(a);
        if (!z) {
            mff_cycles(0xC901, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xC987, 2); call_by_address_tail(0xC987, -1); return;
        }

        /* gameplay_pause_check ($C904) */
        mff_cycles(0xC8FF, 1 + 3 + 2 + 2);
        a = RAM(JOY_PRESSED) & PAD_START;
        z = a == 0;
        if (z) {
            mff_cycles(0xC908, 1);
        } else {
            mff_cycles(0xC90A, 3 + 2 + 2);
            a = RAM(GAME_ACTION);
            for (i = 0; i < (int)sizeof k_no_pause_actions; i++) {
                if (i > 0)
                    mff_cycles((uint16_t)(0xC90C + 4 * i), 2 + 2);   /* CMP #action, BEQ */
                c = a >= k_no_pause_actions[i];
                SET_NZ(a - k_no_pause_actions[i]);
                if (z) {
                    mff_cycles((uint16_t)(0xC90E + 4 * i), 1);
                    goto update;
                }
            }
            mff_cycles(0xC920, 2 + 6);
            a = SFX_PAUSE;
            SET_NZ(a);
            SYNC_OUT;
            if (!mff_call_fn(0x0924, func_F34C, 0xE000)) return;    /* sound_queue_push_sfx */
            SYNC_IN;
            mff_cycles(0xC925, 2 + 3 + 2 + 3 + 2);
            y = SOUND_MUSIC_PAUSE;
            a = RAM(PAUSED) ^ 0x01;
            z = a == 0;
            RAM(PAUSED) = a;
            if (!z) {
                mff_cycles(0xC92D, 1);
            } else {                            /* resumed: $F4 */
                mff_cycles(0xC92F, 2);
                y++;
            }
            mff_cycles(0xC930, 2 + 6);
            a = y;
            SET_NZ(a);
            SYNC_OUT;
            if (!mff_call_fn(0x0933, func_F34C, 0xE000)) return;    /* sound_queue_push_sfx */
            SYNC_IN;
        }
        mff_cycles(0xC934, 3 + 2);
        a = RAM(PAUSED);
        SET_NZ(a);
        if (!z) {
            mff_cycles(0xC938, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xC9A7, 2); call_by_address_tail(0xC9A7, -1); return;
        }
        mff_cycles(0xC936, 1);
update:
        SYNC_OUT;
        r = gameplay_update_work();
        if (r == GU_UNWOUND)
            return;
        if (r == GU_DRAW_JMP) {
            nes_cpu_instruction_boundary(0xC99D, 2); call_by_address_tail(0xC99D, -1); return;
        }
        if (!gameplay_draw_work())
            return;
        if (!gameplay_frame_end_work())
            return;
        SYNC_IN;
    }
}

/*
 * gameplay_update ($C93B, labels.toml): gameplay_update_work, then
 * gameplay_draw ($C99D; by JMP after hud_update, a tail call).
 * Reached by JMP from gameplay in game_action 9 (0E:C8F3); gameplay's own
 * frames run the same code inline.
 */
void func_C93B(void) {
    int r = gameplay_update_work();

    if (r == GU_UNWOUND)
        return;
    if (r == GU_DRAW_JMP) {
        nes_cpu_instruction_boundary(0xC99D, 2); call_by_address_tail(0xC99D, -1); return;
    }
    func_C99D();                                            /* gameplay_draw */
}

/*
 * gameplay_special_screen ($C987, labels.toml)
 * While special_screen_id is not 0 gameplay JMPs here instead of updating
 * the game (0E:C901): banks 04/05, then 04:9800 runs the special screen
 * (the text pages of the boss talk; $F0-$FF are commands). The objects
 * are drawn only while slot 12's obj_kind is 7; either way the frame ends
 * at gameplay_frame_end.
 * Caller: gameplay by JMP (0E:C901, tail call).
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
 * gameplay_draw ($C99D, labels.toml): gameplay_draw_work, then
 * gameplay_frame_end.
 * Callers: gameplay_update (BEQ at 0E:C97B, C97F; JMP at C984, tail call)
 * and gameplay_special_screen.
 */
void func_C99D(void) {
    if (!gameplay_draw_work())
        return;
    mff_cycles(0xC9A7, 0);
    func_C9A7();
}

/*
 * gameplay_frame_end ($C9A7, labels.toml): gameplay_frame_end_work, then
 * JMP gameplay (tail call).
 * Callers: gameplay by JMP when the game is over or paused (0E:C8FA,
 * C938; tail calls), gameplay_draw and gameplay_special_screen.
 */
void func_C9A7(void) {
    if (!gameplay_frame_end_work())
        return;
    nes_cpu_instruction_boundary(0xC8ED, 2); call_by_address_tail(0xC8ED, -1);
}
