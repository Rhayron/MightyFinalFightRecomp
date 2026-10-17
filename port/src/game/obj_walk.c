/*
 * obj_walk.c - the moves that look at the background (docs/objects.md,
 * "Movement helpers (bank $0F)"): a step right, left, down or up, or the
 * vertical step of a jump, then a background probe of kind Y (the caller's
 * Y; stage_probe.c) on the side the object moved to, which puts the object
 * back out of what it walked into, or starts the player's fall into a pit.
 *
 * The probe leaves the attributes of its points ORed in $10. The moves
 * read three kinds: BG_PIT (bit 6), BG_STOP_DOWN ($10), on which a move
 * down stops, and BG_STOP_UP ($30), under which a walk up stops. Walking
 * right or left only reacts to pits: any slot but the player treats the
 * pit's edge as a wall (pushed back out, C set); the player falls in
 * (player_pit_fall_start) unless he is coming down.
 *
 * For the player (X = 0) each move first copies two bytes of his position
 * into $02/$03 (x and x_hi going right or left, y and x_hi otherwise),
 * which the probe overwrites before anything reads them: a leftover.
 */
#include "obj_fixed.h"

/* the walks right and left are the same code at $F003 and $F054 */
#define WALK_RIGHT  0x00
#define WALK_LEFT   0x51
#define AT(pc)      ((uint16_t)((pc) + o))

/*
 * $F003 + o: obj_move_right (o = WALK_RIGHT) or obj_move_left (WALK_LEFT),
 * bg_probe_2 of kind Y, and the pits. Out: C set when pushed out of a pit's
 * edge (not the player), or when the player came down over a pit; clear
 * otherwise. V as the calls leave it.
 */
static void walk_x(uint16_t o)
{
    REGS_IN;

    mff_cycles(AT(0xF003), 4);
    if (x == 0) {                               /* the leftover copy */
        mff_cycles(AT(0xF007), 14);
        RAM(TMP2) = RAM(OBJ_X);
        RAM(TMP3) = RAM(OBJ_X_HI);
    } else {
        mff_cycles(AT(0xF005), 1);
    }
    mff_cycles(AT(0xF011), 6);
    CALL_FIXED(AT(0xF011), o == WALK_RIGHT ? func_F183 : func_F1A1);
    mff_cycles(AT(0xF014), 6);
    CALL_FIXED(AT(0xF014), func_EDBA);          /* bg_probe_2 */
    mff_cycles(AT(0xF017), 9);
    c = 0;
    if (!(RAM(TMP16) & BG_PIT)) {
        mff_cycles(AT(0xF01E), 2);
        mff_cycles(AT(0xF01E), 1);
        mff_cycles(AT(0xF024), 6);
        RTS_RETURN();
    }
    mff_cycles(AT(0xF01C), 1);
    mff_cycles(AT(0xF025), 4);
    if (x != 0) {                               /* the edge of a pit is a wall */
        mff_cycles(AT(0xF020), 1);
        mff_cycles(AT(0xF020), 6);
        CALL_FIXED(AT(0xF020), o == WALK_RIGHT ? func_EF55 : func_EF6D);
        mff_cycles(AT(0xF023), 2);
        c = 1;
        mff_cycles(AT(0xF024), 6);
        RTS_RETURN();
    }
    mff_cycles(AT(0xF029), 7);
    a = RAM(TMP16);
    if (a != BG_PIT) {
        mff_cycles(AT(0xF02F), 4);
        if (a != BG_PIT + BG_STOP_DOWN) {       /* other kinds with bit 6: never executed */
            mff_cycles(AT(0xF033), 8);
            c = 0;
            RTS_RETURN();
        }
        mff_cycles(AT(0xF031), 1);
    } else {
        mff_cycles(AT(0xF02D), 1);
    }
    c = 1;                                      /* the CMP matched */
    mff_cycles(AT(0xF035), 6);
    if (RAM(OBJ_VY) & 0x80) {                   /* coming down: no fall */
        mff_cycles(AT(0xF024), 1);
        mff_cycles(AT(0xF024), 6);
        RTS_RETURN();
    }
    mff_cycles(AT(0xF03A), 6);
    CALL_FIXED(AT(0xF03A), func_F0DC);
    mff_cycles(AT(0xF03D), 8);
    c = 0;
    RTS_RETURN();
}

/*
 * obj_walk_right, $F000 (name proposed): obj_face_right, then
 * obj_step_right.
 * In: X = slot, Y = probe kind. Out: C as walk_x (set: blocked); X, Y kept
 * (Y = grab_slot when the player let go of an enemy to fall).
 * Callers: 00:879F, 87C6, 87D8, 03:B36C, 0D:A93E, A99E, 0E:CCEF.
 */
void func_F000(void)
{
    REGS_IN;

    mff_cycles(0xF000, 6);
    CALL_FIXED(0xF000, func_F090);
    SYNC_OUT;
    walk_x(WALK_RIGHT);
}

/*
 * obj_step_right, $F003 (name proposed): obj_walk_right without turning.
 * In/Out: as obj_walk_right. Caller: 0E:D911 (the player walking while
 * holding an enemy).
 */
void func_F003(void)
{
    walk_x(WALK_RIGHT);
}

/*
 * obj_walk_left, $F051 (name proposed): obj_face_left, then obj_step_left.
 * In/Out: as obj_walk_right.
 * Callers: 00:87A7, 87CC, 03:B384, 0D:A931, A9BE, 0E:CCF7.
 */
void func_F051(void)
{
    REGS_IN;

    mff_cycles(0xF051, 6);
    CALL_FIXED(0xF051, func_F099);
    SYNC_OUT;
    walk_x(WALK_LEFT);
}

/*
 * obj_step_left, $F054 (name proposed): obj_walk_left without turning.
 * Caller: 0E:D95E (the player walking while holding an enemy).
 */
void func_F054(void)
{
    walk_x(WALK_LEFT);
}

/* $F0BD-$F0C1: back up out of the wall below (push_out_up), C set */
static void walk_down_blocked(void)
{
    REGS_IN;

    mff_cycles(0xF0BD, 6);
    CALL_FIXED(0xF0BD, func_EF9D);
    mff_cycles(0xF0C0, 8);
    c = 1;
    RTS_RETURN();
}

/*
 * obj_walk_down, $F0A2 (name proposed): obj_move_down, bg_probe of kind
 * Y; on BG_STOP_DOWN it is put back up (push_out_up, C set). A pit: an
 * enemy treats it as that wall, the player falls in (player_pit_fall_start,
 * whose C it returns). $02 with game_action $0D also stops it (never
 * executed).
 * In: X = slot, Y = probe kind. Out: C set when stopped; X kept.
 * Callers: 03:B356, 0B:BA29, 0D:A97F, 0E:CD14, D9B1.
 */
void func_F0A2(void)
{
    REGS_IN;

    mff_cycles(0xF0A2, 4);
    if (x == 0) {                               /* the leftover copy */
        mff_cycles(0xF0A6, 14);
        RAM(TMP2) = RAM(OBJ_Y);
        RAM(TMP3) = RAM(OBJ_X_HI);
    } else {
        mff_cycles(0xF0A4, 1);
    }
    mff_cycles(0xF0B0, 6);
    CALL_FIXED(0xF0B0, func_F1BF);
    mff_cycles(0xF0B3, 6);
    CALL_FIXED(0xF0B3, func_ECCF);              /* bg_probe */
    mff_cycles(0xF0B6, 9);
    a = RAM(TMP16);
    if (a == BG_STOP_DOWN)
        FALL_INTO(walk_down_blocked);
    mff_cycles(0xF0BB, 1);
    mff_cycles(0xF0C2, 4);
    if (a == 0x02) {                            /* never executed */
        mff_cycles(0xF0C4, 1);
        mff_cycles(0xF0CC, 7);
        if (RAM(GAME_ACTION) == ACTION_0D) {
            mff_cycles(0xF0BD, 1);
            FALL_INTO(walk_down_blocked);
        }
        mff_cycles(0xF0D2, 8);
        c = 0;
        RTS_RETURN();
    }
    mff_cycles(0xF0C6, 4);
    if (!(a & BG_PIT)) {
        mff_cycles(0xF0CA, 8);
        c = 0;
        RTS_RETURN();
    }
    mff_cycles(0xF0C8, 1);
    mff_cycles(0xF0D4, 4);
    if (x != 0) {
        mff_cycles(0xF0BD, 1);
        FALL_INTO(walk_down_blocked);
    }
    mff_cycles(0xF0D8, 6);
    CALL_FIXED(0xF0D8, func_F0DC);
    mff_cycles(0xF0DB, 6);
    RTS_RETURN();
}

/*
 * player_pit_fall_start, $F0DC (name proposed): game_action_before_fall =
 * game_action, game_action = $24 (the pit fall), the player's obj_state
 * $80 (phase 0); a player who was holding an enemy (game_action 4) lets it
 * go (obj_held = $FF).
 * Out: C set when the saved game_action is 4 or more (the CMP #4); Y =
 * grab_slot when letting go; X kept.
 * Callers: the walks right/left (0F:F03A, F08B) and down (0F:F0D8).
 */
void func_F0DC(void)
{
    REGS_IN;

    mff_cycles(0xF0DC, 24);
    RAM(ACTION_BEFORE_FALL) = RAM(GAME_ACTION);
    RAM(GAME_ACTION) = ACTION_PIT_FALL;
    RAM(OBJ_STATE) = OBJ_LIVE;
    a = RAM(ACTION_BEFORE_FALL);
    c = a >= ACTION_GRAB;
    if (a == ACTION_GRAB) {
        mff_cycles(0xF0EF, 10);
        y = RAM(GRAB_SLOT);
        RAM(OBJ_HELD + y) = 0xFF;
    } else {
        mff_cycles(0xF0ED, 1);
    }
    mff_cycles(0xF0F6, 6);
    RTS_RETURN();
}

/*
 * obj_walk_up, $F0F7 (name proposed): obj_move_up, bg_probe of kind Y; on
 * BG_STOP_UP it is put back down (push_out_down, C set). Pits do not
 * matter going up.
 * In: X = slot, Y = probe kind. Out: C set when stopped, and whenever $10
 * is $30 or more (the CMP #$30); X kept.
 * Callers: 03:B33E, 0B:BA13, 0D:A0AA, A962, 0E:CD38, D9D9.
 */
void func_F0F7(void)
{
    REGS_IN;

    mff_cycles(0xF0F7, 4);
    if (x == 0) {                               /* the leftover copy */
        mff_cycles(0xF0FB, 14);
        RAM(TMP2) = RAM(OBJ_Y);
        RAM(TMP3) = RAM(OBJ_X_HI);
    } else {
        mff_cycles(0xF0F9, 2);                  /* BNE across a page */
    }
    mff_cycles(0xF105, 6);
    CALL_FIXED(0xF105, func_F1DF);
    mff_cycles(0xF108, 6);
    CALL_FIXED(0xF108, func_ECCF);              /* bg_probe */
    mff_cycles(0xF10B, 9);
    a = RAM(TMP16);
    c = a >= BG_STOP_UP;
    if (a == BG_STOP_UP) {
        mff_cycles(0xF110, 1);
        mff_cycles(0xF114, 6);
        CALL_FIXED(0xF114, func_EF83);
        mff_cycles(0xF117, 2);
        c = 1;
    } else {
        mff_cycles(0xF112, 2);
        mff_cycles(0xF112, 1);
    }
    mff_cycles(0xF118, 6);
    RTS_RETURN();
}

/*
 * $F170: over a pit, the player starts falling into it (as
 * player_pit_fall_start, but holding nothing); any other slot ignores it.
 * C clear.
 */
static void jump_over_pit(void)
{
    REGS_IN;

    mff_cycles(0xF170, 4);
    if (x != 0) {
        mff_cycles(0xF16E, 1);
        FALL_INTO(func_F16E);
    }
    mff_cycles(0xF174, 25);
    RAM(ACTION_BEFORE_FALL) = RAM(GAME_ACTION);
    RAM(OBJ_STATE) = OBJ_LIVE;
    RAM(GAME_ACTION) = ACTION_PIT_FALL;
    c = 0;
    RTS_RETURN();
}

/*
 * obj_jump_move, $F119 (name proposed): the vertical step of a jump or a
 * fall. Coming down (vy negative): $F20E, obj_gravity, bg_probe of kind Y;
 * landing on BG_STOP_DOWN puts it on top (push_out_up), restarts the fall
 * speed (obj_init_vy) and sets C. Going up: the same with $F22E and probe
 * kind Y + 1; BG_STOP_UP is ignored and BG_STOP_DOWN would push it down
 * (never executed). A pit ($40 and up) either way: the player starts the
 * pit fall.
 * In: X = slot, Y = probe kind. Out: C set when it landed; X kept, Y + 1
 * going up.
 * Callers: 00:8092, 8318, 85C1, 8814, 8881, 0E:CF8C, D5F5, D740, DB05,
 * DBD6 (the player's jumps and falls).
 */
void func_F119(void)
{
    REGS_IN;

    mff_cycles(0xF119, 4);
    if (x == 0) {                               /* the leftover copy */
        mff_cycles(0xF11D, 14);
        RAM(TMP2) = RAM(OBJ_Y);
        RAM(TMP3) = RAM(OBJ_X_HI);
    } else {
        mff_cycles(0xF11B, 1);
    }
    mff_cycles(0xF127, 6);
    if (RAM(OBJ_VY + x) & 0x80) {
        mff_cycles(0xF12C, 6);
        CALL_FIXED(0xF12C, func_F20E);
        mff_cycles(0xF12F, 6);
        CALL_FIXED(0xF12F, func_F24C);
        mff_cycles(0xF132, 6);
        CALL_FIXED(0xF132, func_ECCF);
        mff_cycles(0xF135, 4);
        if (x != 0)
            mff_cycles(0xF137, 1);              /* BNE to the next instruction */
        mff_cycles(0xF139, 7);
        a = RAM(TMP16);
        if (a == BG_STOP_DOWN) {
            mff_cycles(0xF13D, 1);
            mff_cycles(0xF146, 6);
            CALL_FIXED(0xF146, func_EF9D);
            mff_cycles(0xF149, 6);
            CALL_FIXED(0xF149, func_F26E);
            mff_cycles(0xF14C, 8);
            c = 1;
            RTS_RETURN();
        }
        mff_cycles(0xF13F, 4);
        if (a >= BG_PIT) {
            mff_cycles(0xF141, 1);
            FALL_INTO(jump_over_pit);
        }
        JMP_FIXED(0xF143, 0xF16E, func_F16E);
    }
    mff_cycles(0xF12A, 1);
    mff_cycles(0xF14E, 8);
    y++;
    CALL_FIXED(0xF14F, func_F22E);
    mff_cycles(0xF152, 6);
    CALL_FIXED(0xF152, func_F24C);
    mff_cycles(0xF155, 6);
    CALL_FIXED(0xF155, func_ECCF);
    mff_cycles(0xF158, 7);
    a = RAM(TMP16);
    if (a == BG_STOP_UP) {
        mff_cycles(0xF15C, 1);
        FALL_INTO(func_F16E);
    }
    mff_cycles(0xF15E, 4);
    if (a >= BG_PIT) {
        mff_cycles(0xF160, 1);
        FALL_INTO(jump_over_pit);
    }
    mff_cycles(0xF162, 4);
    if (a == BG_STOP_DOWN) {                    /* never executed */
        mff_cycles(0xF164, 1);
        mff_cycles(0xF168, 6);
        CALL_FIXED(0xF168, func_EF83);
        mff_cycles(0xF16B, 6);
        CALL_FIXED(0xF16B, func_F26E);
    } else {
        mff_cycles(0xF166, 2);
        mff_cycles(0xF166, 1);
    }
    FALL_INTO(func_F16E);
}

/*
 * $F16E: CLC / RTS, obj_jump_move's "not landed" exit (its JMP at
 * 0F:F143; the other paths fall into it).
 */
void func_F16E(void)
{
    REGS_IN;

    mff_cycles(0xF16E, 8);
    c = 0;
    RTS_RETURN();
}
