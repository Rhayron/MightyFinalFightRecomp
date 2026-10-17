/*
 * player_exits.c - leaving and entering an area. game_action $19, which
 * stage_events sets when the area is cleared (08:8951), walks the player
 * out in the way of each area and then sets game_action $0E
 * (player_state_transition: the next area). Some areas start with an
 * entrance that area_start ($DFE9) sets: $1B, $1C and $20.
 */
#include "player_defs.h"

/* the exit of each area: a list per round ($9382 + 3 * round, the indices
 * 0-14 in order) into the routine table */
#define EXIT_LIST_LO        0x9378   /* 82 85 88 8B 8E */
#define EXIT_LIST_HI        0x937D   /* 93 93 93 93 93 */
#define EXIT_LO             0x9391
#define EXIT_HI             0x93A0   /* $93EF $945E $94F2 | $9565 $9565 $94F7 |
                                        $9565 $958B $94F2 | $9610 $9615 $94F7 |
                                        $961A $961A $961F */
/* the boss's palette for the arrival (8 bytes at $0618 and $0638) */
#define BOSS_ARRIVAL_PALETTE 0x955D

static void exit_r1a1_walk(void);
static void exit_r3a2_walk(void);
static void exit_area_over(uint16_t pc);

/*
 * action_walk_out ($92E1 in bank $00, proposed): game_action $19. The first
 * step sets a wait of $3C frames (round 1's second area: $0F), ends the
 * attack, drops a held weapon ($93AF, never executed), and gives the
 * standing animation, CHR bank and walking speed. In a boss area (never
 * executed) it would queue music $0E ($0F in round 5), special_screen_id
 * $FF, boss_area 0 and a wait of $D8. When the wait is over every frame
 * JMPs ($0004) to the area's exit (EXIT_LIST, EXIT_LO/HI).
 */
void func_92E1_b0(void) {
    REGS_IN;

    mff_cycles(0x92E1, 4 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x0F;
    if (a == 0) {
        mff_cycles(0x92E8, 3 + 2);
        a = RAM(BOSS_AREA);
        if (a != 0) {
            mff_cycles(0x92EC, 3 + 2 + 2);
            a = RAM(ROUND);
            COMPARE(a, 4);
            if (z) {
                mff_cycles(0x92F0, 1 + 2);
                a = 0x0F;
            } else {
                mff_cycles(0x92F2, 2 + 2 + 1);
                a = 0x0E;
            }
            SET_NZ(a);
            mff_cycles(0x92F8, 6);
            JSR_FN(0x92F8, func_F34A);          /* sound_queue_push */
            mff_cycles(0x92FB, 2 + 3 + 2 + 3 + 2 + 2 + 1);
            a = 0xFF;
            RAM(SPECIAL_SCREEN_ID) = a;
            a = 0;
            RAM(BOSS_AREA) = a;
            a = 0xD8;
        } else {
            mff_cycles(0x92EA, 2 + 2);
            a = 0x3C;
        }
        mff_cycles(0x9309, 3 + 3 + 2);
        RAM(ACTION_TIMER) = a;
        a = RAM(ROUND);
        SET_NZ(a);
        if (z) {
            mff_cycles(0x930F, 3 + 2 + 2);
            a = RAM(SUB_AREA);
            COMPARE(a, 1);
            if (z) {
                mff_cycles(0x9315, 2 + 3);
                a = 0x0F;
                SET_NZ(a);
                RAM(ACTION_TIMER) = a;
            } else {
                mff_cycles(0x9313, 1);
            }
        } else {
            mff_cycles(0x930D, 1);
        }
        mff_cycles(0x9319, 6);
        JSR_FN(0x9319, func_F436);              /* attack_end */
        mff_cycles(0x931C, 3 + 2);
        a = RAM(WEAPON_HELD);
        SET_NZ(a);
        if (!z) {
            mff_cycles(0x9320, 6);
            JSR_AT(0x9320, 0x93AF, 0);          /* drop the weapon */
            mff_cycles(0x9323, 2 + 3);
            a = 0;
            RAM(WEAPON_HELD) = a;
        } else {
            mff_cycles(0x931E, 1);
        }
        mff_cycles(0x9327, 3 + 4);
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(ANIM_STAND + y));
        SET_NZ(a);
        mff_cycles(0x932C, 6);
        JSR_FN(0x932C, func_F294);              /* player_set_anim */
        mff_cycles(0x932F, 4);
        a = prg_read((uint16_t)(CHR_STAND + y));
        SET_NZ(a);
        mff_cycles(0x9332, 6);
        JSR_FN(0x9332, func_F440);              /* player_set_chr */
        mff_cycles(0x9335, 3 + 4);
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(WALK_VX_FRAC + y));
        mff_cycles(0x933A, 4 + 4);
        RAM(OBJ_VX_FRAC) = a;
        a = prg_read((uint16_t)(WALK_VX + y));
        mff_cycles(0x9340, 4 + 4);
        RAM(OBJ_VX) = a;
        a = prg_read((uint16_t)(WALK_VY_FRAC + y));
        mff_cycles(0x9346, 4 + 4);
        RAM(OBJ_VY_FRAC) = a;
        a = prg_read((uint16_t)(WALK_VY + y));
        mff_cycles(0x934C, 4 + 6);
        RAM(OBJ_VY) = a;
        RAM(OBJ_STATE)++;
    } else {
        mff_cycles(0x92E6, 2);
    }
    mff_cycles(0x9352, 3 + 2);
    a = RAM(ACTION_TIMER);
    if (a != 0) {
        mff_cycles(0x9356, 5 + 6);
        RAM(ACTION_TIMER)--;
        SET_NZ(RAM(ACTION_TIMER));
        RTS();
    }
    mff_cycles(0x9354, 1 + 3 + 4);
    y = RAM(ROUND);
    a = prg_read((uint16_t)(EXIT_LIST_LO + y));
    mff_cycles(0x935E, 3 + 4);
    RAM(TMP2) = a;
    a = prg_read((uint16_t)(EXIT_LIST_HI + y));
    mff_cycles(0x9363, 3 + 3 + 2 + 5);
    RAM(TMP3) = a;
    a = RAM(SUB_AREA);
    y = a;
    a = mem_read((uint16_t)(RAM16(TMP2) + y));
    mff_cycles(0x936A, 2 + 4);
    y = a;
    a = prg_read((uint16_t)(EXIT_LO + y));
    mff_cycles(0x936E, 3 + 4);
    RAM(TMP4) = a;
    a = prg_read((uint16_t)(EXIT_HI + y));
    SET_NZ(a);
    mff_cycles(0x9373, 3 + 5);
    RAM(TMP5) = a;
    JMP_IND(TMP4);
}

/*
 * $9297 in bank $00 (proposed name spawn_slot13_b7): an object in slot 13
 * at the screen's left (x 0 on the player's screen, y $90), animation $B7,
 * id $6B (obj_spawn). Called by game_action $26's routine ($91BE: JSR at
 * 00:9264).
 */
void func_9297_b0(void) {
    REGS_IN;

    mff_cycles(0x9297, 2 + 4 + 2 + 4 + 4 + 4 + 4 + 4 + 2 + 2 + 6);
    a = 0;
    RAM(OBJ_X + 13) = a;
    a = 0x90;
    RAM(OBJ_Y + 13) = a;
    a = RAM(OBJ_X_HI);
    RAM(OBJ_X_HI + 13) = a;
    a = RAM(OBJ_Y_HI);
    RAM(OBJ_Y_HI + 13) = a;
    y = 13;
    a = 0xB7;
    SET_NZ(a);
    JSR_FN(0x92B1, func_F2A0);                  /* obj_spawn */
    mff_cycles(0x92B4, 2 + 4 + 6);
    a = 0x6B;
    SET_NZ(a);
    RAM(OBJ_ID + 13) = a;
    RTS();
}

/*
 * exit_r1a1 ($93EF in bank $00, proposed): round 1's first area. The first
 * step: the walking animation, facing right (left from x $E0 on), then
 * exit_r1a1_turned.
 */
void func_93EF_b0(void) {
    REGS_IN;

    mff_cycles(0x93EF, 4 + 2 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x0F;
    COMPARE(a, 2);
    if (z) {
        mff_cycles(0x93F6, 2);
        FALL_INTO(exit_r1a1_walk);
    }
    mff_cycles(0x93F8, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(ANIM_WALK + y));
    SET_NZ(a);
    mff_cycles(0x93FD, 6);
    JSR_FN(0x93FD, func_F294);                  /* player_set_anim */
    mff_cycles(0x9400, 4 + 2 + 2);
    a = RAM(OBJ_X);
    COMPARE(a, 0xE0);
    if (!c) {
        mff_cycles(0x9407, 6);
        JSR_FN(0x9407, func_F03F);              /* player_face_right */
        mff_cycles(0x940A, 3);
        JMP_TAIL(0x9410, 0);                    /* exit_r1a1_turned */
    }
    mff_cycles(0x9405, 1 + 6);
    JSR_FN(0x940D, func_F048);                  /* player_face_left */
    FALL_INTO(func_9410_b0);
}

/* exit_r1a1_turned ($9410 in bank $00, proposed): the next step, then
 * exit_r1a1_walk. Reached by JMP from exit_r1a1 (00:940A) and run on into
 * from it. */
void func_9410_b0(void) {
    REGS_IN;

    mff_cycles(0x9410, 6);
    RAM(OBJ_STATE)++;
    FALL_INTO(exit_r1a1_walk);
}

/* exit_r1a1_walk (00:9413): into the band between y $82 and $84 first
 * (down, or up), then towards x $E0 facing that way; then
 * exit_r1a1_out. */
static void exit_r1a1_walk(void) {
    REGS_IN;

    mff_cycles(0x9413, 4 + 2 + 2);
    a = RAM(OBJ_Y);
    COMPARE(a, 0x82);
    if (!c) {
        mff_cycles(0x941A, 6);
        JSR_FN(0x941A, func_F1BF);              /* obj_move_down */
        mff_cycles(0x941D, 6);
        RTS();
    }
    mff_cycles(0x9418, 1 + 4 + 2 + 2);
    a = RAM(OBJ_Y);
    COMPARE(a, 0x84);
    if (c) {
        mff_cycles(0x9425, 6);
        JSR_FN(0x9425, func_F1DF);              /* obj_move_up */
        mff_cycles(0x9428, 6);
        RTS();
    }
    mff_cycles(0x9423, 1 + 4 + 2 + 2);
    a = RAM(OBJ_X);
    COMPARE(a, 0xE0);
    if (c) {
        mff_cycles(0x9430, 6);
        JSR_FN(0x9430, func_F1A1);              /* obj_move_left */
        mff_cycles(0x9433, 6);
        JSR_FN(0x9433, func_F048);              /* player_face_left */
        mff_cycles(0x9436, 3);
        JMP_TAIL(0x943F, 0);                    /* exit_r1a1_out */
    }
    mff_cycles(0x942E, 1 + 6);
    JSR_FN(0x9439, func_F183);                  /* obj_move_right */
    mff_cycles(0x943C, 6);
    JSR_FN(0x943C, func_F03F);                  /* player_face_right */
    FALL_INTO(func_943F_b0);
}

/* exit_r1a1_out ($943F in bank $00, proposed): from x $D0 the next step;
 * in it, from x $E0 the area is over (game_action $0E). Reached by JMP
 * (00:9436) and run on into from exit_r1a1_walk. */
void func_943F_b0(void) {
    REGS_IN;

    mff_cycles(0x943F, 4 + 2 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x0F;
    if (a != 3) {
        mff_cycles(0x9448, 4 + 2 + 2);
        a = RAM(OBJ_X);
        COMPARE(a, 0xD0);
        if (!c) {
            mff_cycles(0x944D, 1 + 6);
            RTS();
        }
        mff_cycles(0x944F, 6);
        RAM(OBJ_STATE)++;
    } else {
        mff_cycles(0x9446, 1);
    }
    mff_cycles(0x9452, 4 + 2 + 2);
    a = RAM(OBJ_X);
    COMPARE(a, 0xE0);
    if (c) {
        mff_cycles(0x9459, 2 + 3);
        a = ACT_NEXT_AREA;
        SET_NZ(a);
        RAM(GAME_ACTION) = a;
    } else {
        mff_cycles(0x9457, 1);
    }
    mff_cycles(0x945D, 6);
    RTS();
}

/*
 * exit_r1a2 ($945E in bank $00, proposed): round 1's second area. The
 * player walks right (the walking animation unless already walking) until x
 * $B0, when $987A (Y = 1) runs and the next step starts with obj_timer[0]
 * 1; after that frame $987A (Y = 2), the falling animation, obj_vy $0200
 * and sound $24; then he goes down (obj_move_down), behind the scenery
 * from y $88 (obj_flags bit 5), hidden at y $AC (obj_flags $F4), and at
 * y $E8 the area is over.
 */
void func_945E_b0(void) {
    REGS_IN;

    mff_cycles(0x945E, 4 + 2 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x0F;
    COMPARE(a, 2);
    if (z) {                                    /* the jump */
        mff_cycles(0x9465, 1 + 4 + 2);
        a = RAM(OBJ_TIMER);
        if (a == 0) {
            mff_cycles(0x949B, 2 + 6);
            y = 2;
            SET_NZ(y);
            JSR_AT(0x949D, 0x987A, 0);
            mff_cycles(0x94A0, 3 + 4);
            y = RAM(PLAYER_ID);
            a = prg_read((uint16_t)(ANIM_FALL + y));
            SET_NZ(a);
            mff_cycles(0x94A5, 6);
            JSR_FN(0x94A5, func_F294);          /* player_set_anim */
            mff_cycles(0x94A8, 4);
            a = prg_read((uint16_t)(CHR_JUMP + y));
            SET_NZ(a);
            mff_cycles(0x94AB, 6);
            JSR_FN(0x94AB, func_F440);          /* player_set_chr */
            mff_cycles(0x94AE, 6 + 2 + 4 + 2 + 4 + 2 + 6);
            RAM(OBJ_STATE)++;
            a = 0;
            RAM(OBJ_VY_FRAC) = a;
            a = 0x02;
            RAM(OBJ_VY) = a;
            a = 0x24;
            SET_NZ(a);
            JSR_FN(0x94BD, func_F34C);          /* sound_queue_push_sfx */
            mff_cycles(0x94C0, 6);
            RTS();
        }
        mff_cycles(0x9499, 1 + 6);
        RAM(OBJ_TIMER)--;
        SET_NZ(RAM(OBJ_TIMER));
        mff_cycles(0x94C4, 6);
        RTS();
    }
    mff_cycles(0x9467, 2 + 2);
    COMPARE(a, 3);
    if (z) {                                    /* going down */
        mff_cycles(0x9469, 1 + 6);
        JSR_FN(0x94C5, func_F1BF);              /* obj_move_down */
        mff_cycles(0x94C8, 4 + 2 + 2);
        a = RAM(OBJ_Y);
        if (a >= 0x88) {
            mff_cycles(0x94CF, 4 + 2 + 4);
            a = RAM(OBJ_FLAGS) | 0x20;
            RAM(OBJ_FLAGS) = a;
        } else {
            mff_cycles(0x94CD, 1);
        }
        mff_cycles(0x94D7, 4 + 2 + 2);
        a = RAM(OBJ_Y);
        COMPARE(a, 0xAC);
        if (!c) {
            mff_cycles(0x94DC, 1 + 6);
            RTS();
        }
        mff_cycles(0x94DE, 2 + 4 + 4 + 2 + 2);
        a = 0xF4;
        RAM(OBJ_FLAGS) = a;
        a = RAM(OBJ_Y);
        COMPARE(a, 0xE8);
        if (!c) {
            mff_cycles(0x94E8, 1 + 6);
            RTS();
        }
        mff_cycles(0x94EA, 2 + 3 + 6);
        a = ACT_NEXT_AREA;
        SET_NZ(a);
        RAM(GAME_ACTION) = a;
        RTS();
    }
    mff_cycles(0x946B, 3 + 4 + 4);              /* walking right */
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    COMPARE(a, prg_read((uint16_t)(IS_WALKING + y)));
    mff_cycles(0x9473, 2);
    if (!z) {
        mff_cycles(0x9475, 4);
        a = prg_read((uint16_t)(ANIM_WALK + y));
        SET_NZ(a);
        mff_cycles(0x9478, 6);
        JSR_FN(0x9478, func_F294);              /* player_set_anim */
        mff_cycles(0x947B, 4);
        a = prg_read((uint16_t)(CHR_STAND + y));
        SET_NZ(a);
        mff_cycles(0x947E, 6);
        JSR_FN(0x947E, func_F440);              /* player_set_chr */
    } else {
        mff_cycles(0x9473, 1);
    }
    mff_cycles(0x9481, 6);
    JSR_FN(0x9481, func_F183);                  /* obj_move_right */
    mff_cycles(0x9484, 2 + 2);
    COMPARE(a, 0xB0);                           /* A as obj_move_right leaves it */
    if (!c) {
        mff_cycles(0x9486, 1 + 6);
        RTS();
    }
    mff_cycles(0x9488, 2 + 6);
    y = 1;
    SET_NZ(y);
    JSR_AT(0x948A, 0x987A, 0);
    mff_cycles(0x948D, 6 + 2 + 4 + 6);
    RAM(OBJ_STATE)++;
    a = 1;
    SET_NZ(a);
    RAM(OBJ_TIMER) = a;
    RTS();
}

/* exit_area_over: LDA #$0E / STA game_action / RTS, the area is over. The
 * ROM has five copies: the exits $94F2, $9610, $9615 and $961A, and the end
 * of exit_r3a2 (00:960B). Out: A = $0E. */
static void exit_area_over(uint16_t pc) {
    REGS_IN;

    mff_cycles(pc, 2 + 3 + 6);
    a = ACT_NEXT_AREA;
    SET_NZ(a);
    RAM(GAME_ACTION) = a;
    RTS();
}

/* exit_now ($94F2 in bank $00, proposed): the area is over at once (round
 * 1's third area, round 3's third). */
void func_94F2_b0(void) {
    exit_area_over(0x94F2);
}

/*
 * exit_boss_arrival ($94F7 in bank $00, proposed; round 2's and round 4's
 * third areas): no walk out - the boss comes. $B2 $FF, area_clear $7F,
 * special_screen_id $FD, CHR banks 3-5 $23 $23 $35, the boss's palette
 * ($955D, 8 bytes into $0618 and $0638, palette_request $FF), the
 * player's state cleared and player_stand, and slot 15 set up for id $70
 * (obj_state and obj_flags $80, at x 0 / y 0 on the player's screen, its
 * animation cleared).
 */
void func_94F7_b0(void) {
    REGS_IN;

    mff_cycles(0x94F7, 2 + 3 + 2 + 3 + 2 + 3 + 2 + 3 + 3 + 2 + 3 + 2);
    a = 0xFF;
    RAM(UNK_B2) = a;
    a = 0x7F;
    RAM(AREA_CLEAR) = a;
    a = 0xFD;
    RAM(SPECIAL_SCREEN_ID) = a;
    a = 0x23;
    RAM(CHR_BANKS + 3) = a;
    RAM(CHR_BANKS + 4) = a;
    a = 0x35;
    RAM(CHR_BANKS + 5) = a;
    x = 0;
    for (;;) {
        mff_cycles(0x950F, 4);
        a = prg_read((uint16_t)(BOSS_ARRIVAL_PALETTE + x));
        mff_cycles(0x9512, 5 + 5 + 2 + 2 + 2);
        RAM(PALETTE_BUF + 24 + x) = a;
        RAM(PALETTE_BASE + 24 + x) = a;
        x++;
        COMPARE(x, 8);
        if (z)
            break;
        mff_cycles(0x951B, 1);
    }
    mff_cycles(0x951D, 2 + 3 + 2 + 3 * 6 + 6);
    x = 0xFF;
    RAM(PALETTE_REQUEST) = x;
    a = 0;
    SET_NZ(a);
    RAM(GAME_ACTION) = a;
    RAM(JUMP_MOVING) = a;
    RAM(COMBO_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(PLAYER_INVULN) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    JSR_AT(0x952F, 0xCEAA, 0);                  /* player_stand */
    mff_cycles(0x9532, 2 + 4 + 4 + 2 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 2 + 4 + 6);
    a = STATE_LIVE;
    RAM(OBJ_STATE + 15) = a;
    RAM(OBJ_FLAGS + 15) = a;
    a = 0;
    RAM(OBJ_X + 15) = a;
    RAM(OBJ_Y + 15) = a;
    RAM(OBJ_ANIM + 15) = a;
    RAM(OBJ_ANIM_TIMER + 15) = a;
    RAM(OBJ_ANIM_FRAME + 15) = a;
    a = RAM(OBJ_X_HI);
    RAM(OBJ_X_HI + 15) = a;
    a = RAM(OBJ_Y_HI);
    RAM(OBJ_Y_HI + 15) = a;
    a = 0x70;
    SET_NZ(a);
    RAM(OBJ_ID + 15) = a;
    RTS();
}

/* exit_walk_right ($9565 in bank $00, proposed; round 2's first and second
 * areas, round 3's first): the walking animation facing right on the first
 * step, then right until x $FC, where the area is over. */
void func_9565_b0(void) {
    REGS_IN;

    mff_cycles(0x9565, 4 + 2 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x0F;
    COMPARE(a, 2);
    if (!z) {
        mff_cycles(0x956E, 6 + 3 + 4);
        RAM(OBJ_STATE)++;
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(ANIM_WALK + y));
        SET_NZ(a);
        mff_cycles(0x9576, 6);
        JSR_FN(0x9576, func_F294);              /* player_set_anim */
        mff_cycles(0x9579, 6);
        JSR_FN(0x9579, func_F03F);              /* player_face_right */
    } else {
        mff_cycles(0x956C, 1);
    }
    mff_cycles(0x957C, 6);
    JSR_FN(0x957C, func_F183);                  /* obj_move_right */
    mff_cycles(0x957F, 4 + 2 + 2);
    a = RAM(OBJ_X);
    COMPARE(a, 0xFC);
    if (c) {
        mff_cycles(0x9586, 2 + 3);
        a = ACT_NEXT_AREA;
        SET_NZ(a);
        RAM(GAME_ACTION) = a;
    } else {
        mff_cycles(0x9584, 1);
    }
    mff_cycles(0x958A, 6);
    RTS();
}

/* exit_r3a2 ($958B in bank $00, proposed): round 3's second area. The first
 * step runs $987A (Y = 3), sound $21, the walking animation facing right
 * (left from x $E0 on), then exit_r3a2_turned. */
void func_958B_b0(void) {
    REGS_IN;

    mff_cycles(0x958B, 4 + 2 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x0F;
    if (a == 2) {
        mff_cycles(0x9592, 1);
        FALL_INTO(exit_r3a2_walk);
    }
    mff_cycles(0x9594, 2 + 2);
    COMPARE(a, 3);
    if (z) {                                    /* BEQ $960B, across a page */
        mff_cycles(0x9596, 2);
        SYNC_OUT;
        exit_area_over(0x960B);
        return;
    }
    mff_cycles(0x9598, 2 + 6);
    y = 3;
    SET_NZ(y);
    JSR_AT(0x959A, 0x987A, 0);
    mff_cycles(0x959D, 2 + 6);
    a = 0x21;
    SET_NZ(a);
    JSR_FN(0x959F, func_F34C);                  /* sound_queue_push_sfx */
    mff_cycles(0x95A2, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(ANIM_WALK + y));
    SET_NZ(a);
    mff_cycles(0x95A7, 6);
    JSR_FN(0x95A7, func_F294);                  /* player_set_anim */
    mff_cycles(0x95AA, 4 + 2 + 2);
    a = RAM(OBJ_X);
    COMPARE(a, 0xE0);
    if (c) {
        mff_cycles(0x95B1, 6);
        JSR_FN(0x95B1, func_F048);              /* player_face_left */
        mff_cycles(0x95B4, 3);
        JMP_TAIL(0x95BA, 0);                    /* exit_r3a2_turned */
    }
    mff_cycles(0x95AF, 1 + 6);
    JSR_FN(0x95B7, func_F03F);                  /* player_face_right */
    FALL_INTO(func_95BA_b0);
}

/* exit_r3a2_turned ($95BA in bank $00, proposed): the next step, then
 * exit_r3a2_walk. Reached by JMP from exit_r3a2 (00:95B4) and run on into
 * from it. */
void func_95BA_b0(void) {
    REGS_IN;

    mff_cycles(0x95BA, 6);
    RAM(OBJ_STATE)++;
    FALL_INTO(exit_r3a2_walk);
}

/* exit_r3a2_walk (00:95BD): into the band between y $9C and $A4 first
 * (down, or up: never executed), then the way he faces until x $C8, where
 * exit_r3a2_jump. */
static void exit_r3a2_walk(void) {
    REGS_IN;

    mff_cycles(0x95BD, 4 + 2 + 2);
    a = RAM(OBJ_Y);
    COMPARE(a, 0x9C);
    if (!c) {
        mff_cycles(0x95C4, 6);
        JSR_FN(0x95C4, func_F1BF);              /* obj_move_down */
        mff_cycles(0x95C7, 6);
        RTS();
    }
    mff_cycles(0x95C2, 1 + 4 + 2 + 2);
    a = RAM(OBJ_Y);
    COMPARE(a, 0xA4);
    if (c) {
        mff_cycles(0x95CF, 6);
        JSR_FN(0x95CF, func_F1DF);              /* obj_move_up */
        mff_cycles(0x95D2, 6);
        RTS();
    }
    mff_cycles(0x95CD, 1 + 4 + 2 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x0F;
    COMPARE(a, 3);
    if (z) {
        mff_cycles(0x95DA, 2 + 6);
        RTS();
    }
    mff_cycles(0x95DC, 4 + 2 + 2);
    a = RAM(OBJ_FLAGS);
    COMPARE(a, FLAGS_FACING_LEFT);
    if (z) {                                    /* facing left */
        mff_cycles(0x95E3, 6);
        JSR_FN(0x95E3, func_F1A1);              /* obj_move_left */
        mff_cycles(0x95E6, 4 + 2 + 2);
        a = RAM(OBJ_X);
        COMPARE(a, 0xC8);
        if (c) {
            mff_cycles(0x95EB, 2 + 6);
            RTS();
        }
        mff_cycles(0x95ED, 3);
        JMP_TAIL(0x95FA, 0);                    /* exit_r3a2_jump */
    }
    mff_cycles(0x95E1, 1 + 6);
    JSR_FN(0x95F0, func_F183);                  /* obj_move_right */
    mff_cycles(0x95F3, 4 + 2 + 2);
    a = RAM(OBJ_X);
    COMPARE(a, 0xC8);
    if (!c) {
        mff_cycles(0x95F8, 2 + 6);
        RTS();
    }
    FALL_INTO(func_95FA_b0);
}

/* exit_r3a2_jump ($95FA in bank $00, proposed): the falling animation and
 * the jump's CHR bank, the next step, and the area is over. Reached by JMP
 * (00:95ED) and run on into from exit_r3a2_walk. */
void func_95FA_b0(void) {
    REGS_IN;

    mff_cycles(0x95FA, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(ANIM_FALL + y));
    SET_NZ(a);
    mff_cycles(0x95FF, 6);
    JSR_FN(0x95FF, func_F294);                  /* player_set_anim */
    mff_cycles(0x9602, 4);
    a = prg_read((uint16_t)(CHR_JUMP + y));
    SET_NZ(a);
    mff_cycles(0x9605, 6);
    JSR_FN(0x9605, func_F440);                  /* player_set_chr */
    mff_cycles(0x9608, 6);
    RAM(OBJ_STATE)++;
    SYNC_OUT;
    exit_area_over(0x960B);
}

/* exit_now ($9610, $9615 and $961A in bank $00, proposed): the area is over
 * at once. $9610 is round 4's first area, $9615 its second, $961A round 5's
 * first and second; the CDL of the oracle runs never saw $9610 or $961A
 * run. */
void func_9610_b0(void) {
    exit_area_over(0x9610);
}

void func_9615_b0(void) {
    exit_area_over(0x9615);
}

void func_961A_b0(void) {
    exit_area_over(0x961A);
}

/* exit_wait ($961F in bank $00, proposed; round 5's third area, never
 * executed in any run): $B4 frames (obj_timer[0]), then the area is
 * over. */
void func_961F_b0(void) {
    REGS_IN;

    mff_cycles(0x961F, 4 + 2 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x0F;
    COMPARE(a, 2);
    if (z) {
        mff_cycles(0x9628, 2 + 4 + 6);
        a = 0xB4;
        RAM(OBJ_TIMER) = a;
        RAM(OBJ_STATE)++;
    } else {
        mff_cycles(0x9626, 1);
    }
    mff_cycles(0x9630, 6 + 4 + 2);
    RAM(OBJ_TIMER)--;
    a = RAM(OBJ_TIMER);
    SET_NZ(a);
    if (z) {
        mff_cycles(0x9638, 2 + 3);
        a = ACT_NEXT_AREA;
        SET_NZ(a);
        RAM(GAME_ACTION) = a;
    } else {
        mff_cycles(0x9636, 1);
    }
    mff_cycles(0x963C, 6);
    RTS();
}

/* ---- entering an area: the actions area_start sets ($1B round 1's
 * second area, $1C its third, $20 the third areas of rounds 2 and 5) ---- */

#define ANIM_ENTER_DOOR     0xE1F2   /* 10 11 12 */
#define CHR_ENTER_DOOR      0xE23A   /* 03 07 09 */
#define ANIM_DROP_DOWN      0xE20A   /* 28 29 2A */
#define CHR_DROP_DOWN       0xE252   /* 0D 15 13 */
/* the palettes of round 1's third area as the player comes in (16 bytes
 * into $0600 and $0620) */
#define ENTRANCE_PALETTE    0x97F9
#define ENTRANCE_PALETTE_2  0x9809
/* the scenery changes $987A copies into vram_buffer, by Y */
#define SCENERY_LIST_LO     0x989A   /* A2 DA 18 27 */
#define SCENERY_LIST_HI     0x989E   /* 98 98 99 99 */

static void entrance_palette(void);

/*
 * enter_r1a2 ($963D in bank $00, proposed): game_action $1B, round 1's
 * second area. The first step: obj_vx $0100, obj_timer[0] $1E, animation
 * $E1F2 with CHR $E23A. On frame 3 at timer 0 of it sound $21, the scenery
 * change 0 ($987A), the next step and player_stand. Then after obj_timer
 * frames the player walks right (the walking animation unless already
 * walking): in front of the scenery from x $1C (obj_flags bit 5 cleared),
 * and at x $30 he is in: obj_state[0] $80, game_action and obj_timer 0.
 * In round 1's second area (area_start sets $1B only there) game_action
 * $20 follows with the walking speed in x; another round, or another area
 * than the second and fourth, would end with player_stand (never
 * executed).
 */
void func_963D_b0(void) {
    REGS_IN;

    mff_cycles(0x963D, 4 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x0F;
    if (a == 0) {
        mff_cycles(0x9644, 4 + 2 + 4 + 6 + 2 + 4 + 3 + 4);
        RAM(OBJ_VX_FRAC) = a;
        a = 0x01;
        RAM(OBJ_VX) = a;
        RAM(OBJ_STATE)++;
        a = 0x1E;
        RAM(OBJ_TIMER) = a;
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(ANIM_ENTER_DOOR + y));
        SET_NZ(a);
        mff_cycles(0x9659, 6);
        JSR_FN(0x9659, func_F294);              /* player_set_anim */
        mff_cycles(0x965C, 4);
        a = prg_read((uint16_t)(CHR_ENTER_DOOR + y));
        SET_NZ(a);
        mff_cycles(0x965F, 6);
        JSR_FN(0x965F, func_F440);              /* player_set_chr */
    } else {
        mff_cycles(0x9642, 1);
    }
    mff_cycles(0x9662, 4 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x02;
    if (a == 0) {
        mff_cycles(0x9669, 4 + 2 + 2);
        a = RAM(OBJ_ANIM_FRAME);
        COMPARE(a, 3);
        if (z) {
            mff_cycles(0x9670, 4 + 2);
            a = RAM(OBJ_ANIM_TIMER);
            SET_NZ(a);
            if (z) {
                mff_cycles(0x9675, 2 + 6);
                a = 0x21;
                SET_NZ(a);
                JSR_FN(0x9677, func_F34C);      /* sound_queue_push_sfx */
                mff_cycles(0x967A, 2 + 6);
                y = 0;
                SET_NZ(y);
                JSR_AT(0x967C, 0x987A, 0);      /* scenery_change */
                mff_cycles(0x967F, 6 + 6);
                RAM(OBJ_STATE)++;
                SET_NZ(RAM(OBJ_STATE));
                JSR_AT(0x9682, 0xCEAA, 0);      /* player_stand */
            } else {
                mff_cycles(0x9673, 1);
            }
        } else {
            mff_cycles(0x966E, 1);
        }
        mff_cycles(0x9685, 6);
        RTS();
    }
    mff_cycles(0x9667, 1 + 4 + 2);
    a = RAM(OBJ_TIMER);
    if (a != 0) {
        mff_cycles(0x968B, 6 + 6);
        RAM(OBJ_TIMER)--;
        SET_NZ(RAM(OBJ_TIMER));
        RTS();
    }
    mff_cycles(0x9689, 1 + 3 + 4 + 4);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    COMPARE(a, prg_read((uint16_t)(IS_WALKING + y)));
    mff_cycles(0x9697, 2);
    if (!z) {
        mff_cycles(0x9699, 4);
        a = prg_read((uint16_t)(ANIM_WALK + y));
        SET_NZ(a);
        mff_cycles(0x969C, 6);
        JSR_FN(0x969C, func_F294);              /* player_set_anim */
        mff_cycles(0x969F, 4);
        a = prg_read((uint16_t)(CHR_STAND + y));
        SET_NZ(a);
        mff_cycles(0x96A2, 6);
        JSR_FN(0x96A2, func_F440);              /* player_set_chr */
    } else {
        mff_cycles(0x9697, 1);
    }
    mff_cycles(0x96A5, 6);
    JSR_FN(0x96A5, func_F183);                  /* obj_move_right */
    mff_cycles(0x96A8, 4 + 2 + 2);
    a = RAM(OBJ_X);
    COMPARE(a, 0x1C);
    if (!c) {
        mff_cycles(0x96AD, 1 + 6);
        RTS();
    }
    mff_cycles(0x96AF, 4 + 2 + 4 + 4 + 2 + 2);
    a = RAM(OBJ_FLAGS) & ~0x20;
    RAM(OBJ_FLAGS) = a;
    a = RAM(OBJ_X);
    COMPARE(a, 0x30);
    if (!c) {
        mff_cycles(0x96BC, 1 + 6);
        RTS();
    }
    mff_cycles(0x96BE, 2 + 4 + 2 + 3 + 4 + 3 + 2);
    a = STATE_LIVE;
    RAM(OBJ_STATE) = a;
    a = 0;
    RAM(GAME_ACTION) = a;
    RAM(OBJ_TIMER) = a;
    a = RAM(ROUND);
    SET_NZ(a);
    if (!z) {
        mff_cycles(0x96CC, 1 + 6);
        RTS();
    }
    mff_cycles(0x96CE, 3 + 2 + 2);
    a = RAM(SUB_AREA);
    COMPARE(a, 1);
    if (!z) {
        mff_cycles(0x96D4, 2 + 2);
        COMPARE(a, 3);
        if (!z) {
            mff_cycles(0x96D8, 6);
            JSR_AT(0x96D8, 0xCEAA, 0);          /* player_stand */
            mff_cycles(0x96DB, 6);
            RTS();
        }
        mff_cycles(0x96D6, 1);
    } else {
        mff_cycles(0x96D2, 1);
    }
    mff_cycles(0x96DC, 2 + 3 + 3 + 4);
    a = 0x20;
    RAM(GAME_ACTION) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(WALK_VX_FRAC + y));
    mff_cycles(0x96E5, 4 + 4);
    RAM(OBJ_VX_FRAC) = a;
    a = prg_read((uint16_t)(WALK_VX + y));
    SET_NZ(a);
    mff_cycles(0x96EB, 4 + 6);
    RAM(OBJ_VX) = a;
    RTS();
}

/*
 * enter_r1a3 ($96EF in bank $00, proposed): game_action $1C, round 1's
 * third area: the player drops in. The first step: obj_vy $0500 (down),
 * animation $E20A with CHR $E252. Step 1: down (obj_move_down) to y $98,
 * then the boss music ($05) and a knocked-down speed (obj_vx $00E6, obj_vy
 * $03E8). Step 2: falling and drifting right to y $A0, then lying (the
 * knocked-down animation and CHR bank) with the palette $97F9. Step 3: on
 * frame 6 at timer $0A he gets up (the getting-up animation and CHR) with
 * the palette again. Step 4: enter_r1a3_done.
 */
void func_96EF_b0(void) {
    REGS_IN;

    mff_cycles(0x96EF, 4 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x0F;
    if (a == 0) {
        mff_cycles(0x96F6, 4 + 2 + 4 + 3 + 4);
        RAM(OBJ_VY_FRAC) = a;
        a = 0x05;
        RAM(OBJ_VY) = a;
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(ANIM_DROP_DOWN + y));
        SET_NZ(a);
        mff_cycles(0x9703, 6);
        JSR_FN(0x9703, func_F294);              /* player_set_anim */
        mff_cycles(0x9706, 4);
        a = prg_read((uint16_t)(CHR_DROP_DOWN + y));
        SET_NZ(a);
        mff_cycles(0x9709, 6);
        JSR_FN(0x9709, func_F440);              /* player_set_chr */
        mff_cycles(0x970C, 6);
        RAM(OBJ_STATE)++;
    } else {
        mff_cycles(0x96F4, 2);
    }
    mff_cycles(0x970F, 4 + 2 + 2 + 2);
    a = RAM(OBJ_STATE) & 0x0F;
    COMPARE(a, 2);
    if (z) {                                    /* falling */
        mff_cycles(0x9716, 1 + 6);
        JSR_FN(0x974A, func_F1FD);              /* obj_fall, no probe */
        mff_cycles(0x974D, 6);
        JSR_FN(0x974D, func_F183);              /* obj_move_right */
        mff_cycles(0x9750, 4 + 2 + 2);
        a = RAM(OBJ_Y);
        COMPARE(a, 0xA0);
        if (!c) {
            mff_cycles(0x9755, 1 + 6);
            RTS();
        }
        mff_cycles(0x9757, 6 + 3 + 4);
        RAM(OBJ_STATE)++;
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(ANIM_KNOCKED_DOWN + y));
        SET_NZ(a);
        mff_cycles(0x975F, 6);
        JSR_FN(0x975F, func_F294);              /* player_set_anim */
        mff_cycles(0x9762, 4);
        a = prg_read((uint16_t)(CHR_KNOCKED_DOWN + y));
        SET_NZ(a);
        mff_cycles(0x9765, 6);
        JSR_FN(0x9765, func_F440);              /* player_set_chr */
        mff_cycles(0x9768, 2 + 4);
        y = 0x0F;
        a = prg_read((uint16_t)(ENTRANCE_PALETTE + y));
        mff_cycles(0x976D, 5 + 5 + 2 + 2);
        RAM(PALETTE_BUF + y) = a;
        RAM(PALETTE_BASE + y) = a;
        y--;
        SET_NZ(y);                              /* never negative: Y is now $0E */
        mff_cycles(0x9774, 1);
        FALL_INTO(entrance_palette);
    }
    mff_cycles(0x9718, 2 + 2);
    if (a == 3) {                               /* lying */
        mff_cycles(0x971A, 1 + 4 + 2 + 2);
        a = RAM(OBJ_ANIM_FRAME);
        COMPARE(a, 6);
        if (!z) {
            mff_cycles(0x977E, 1 + 6);
            RTS();
        }
        mff_cycles(0x9780, 4 + 2 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        COMPARE(a, 0x0A);
        if (!z) {
            mff_cycles(0x9785, 1 + 6);
            RTS();
        }
        mff_cycles(0x9787, 3 + 4);
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(ANIM_GET_UP + y));
        SET_NZ(a);
        mff_cycles(0x978C, 6);
        JSR_FN(0x978C, func_F294);              /* player_set_anim */
        mff_cycles(0x978F, 4);
        a = prg_read((uint16_t)(CHR_GET_UP + y));
        SET_NZ(a);
        mff_cycles(0x9792, 6);
        JSR_FN(0x9792, func_F440);              /* player_set_chr */
        mff_cycles(0x9795, 6 + 2);
        RAM(OBJ_STATE)++;
        y = 0x0F;
        FALL_INTO(entrance_palette);
    }
    mff_cycles(0x971C, 2 + 2);
    COMPARE(a, 4);
    if (z) {
        mff_cycles(0x9720, 3);
        JMP_TAIL(0x97A9, 0);                    /* enter_r1a3_done */
    }
    mff_cycles(0x971E, 1 + 6);                  /* coming down */
    JSR_FN(0x9723, func_F1BF);                  /* obj_move_down */
    mff_cycles(0x9726, 4 + 2 + 2);
    a = RAM(OBJ_Y);
    COMPARE(a, 0x98);
    if (c) {
        mff_cycles(0x972D, 2 + 6);
        a = 0x05;
        SET_NZ(a);
        JSR_FN(0x972F, func_F34A);              /* sound_queue_push: the boss music */
        mff_cycles(0x9732, 2 + 4 + 2 + 4 + 2 + 4 + 2 + 4 + 6);
        a = 0xE6;
        RAM(OBJ_VX_FRAC) = a;
        a = 0x00;
        RAM(OBJ_VX) = a;
        a = 0xE8;
        RAM(OBJ_VY_FRAC) = a;
        a = 0x03;
        RAM(OBJ_VY) = a;
        RAM(OBJ_STATE)++;
        SET_NZ(RAM(OBJ_STATE));
    } else {
        mff_cycles(0x972B, 1);
    }
    mff_cycles(0x9749, 6);
    RTS();
}

/* entrance_palette (00:979A, inside enter_r1a3): the palette $97F9 from Y
 * down to 0 into $0600 and $0620, palette_request = Y ($FF). */
static void entrance_palette(void) {
    REGS_IN;

    for (;;) {
        mff_cycles(0x979A, 4);
        a = prg_read((uint16_t)(ENTRANCE_PALETTE + y));
        mff_cycles(0x979D, 5 + 5 + 2 + 2);
        RAM(PALETTE_BUF + y) = a;
        RAM(PALETTE_BASE + y) = a;
        y--;
        SET_NZ(y);
        if (n)
            break;
        mff_cycles(0x97A4, 1);
    }
    mff_cycles(0x97A6, 3 + 6);
    RAM(PALETTE_REQUEST) = y;
    RTS();
}

/* enter_r1a3_done ($97A9 in bank $00, proposed): at timer 8 the palette
 * $9809 and the palette animations ($97C8), then game_action $20. Reached
 * by JMP from enter_r1a3 (00:9720). */
void func_97A9_b0(void) {
    REGS_IN;

    mff_cycles(0x97A9, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_TIMER);
    COMPARE(a, 8);
    if (!z) {
        mff_cycles(0x97AE, 1 + 6);
        RTS();
    }
    mff_cycles(0x97B0, 2);
    y = 0x0F;
    for (;;) {
        mff_cycles(0x97B2, 4);
        a = prg_read((uint16_t)(ENTRANCE_PALETTE_2 + y));
        mff_cycles(0x97B5, 5 + 5 + 2 + 2);
        RAM(PALETTE_BUF + y) = a;
        RAM(PALETTE_BASE + y) = a;
        y--;
        SET_NZ(y);
        if (n)
            break;
        mff_cycles(0x97BC, 1);
    }
    mff_cycles(0x97BE, 3 + 6);
    RAM(PALETTE_REQUEST) = y;
    JSR_AT(0x97C0, 0x97C8, 0);                  /* entrance_palette_anims */
    mff_cycles(0x97C3, 2 + 3 + 6);
    a = 0x20;
    SET_NZ(a);
    RAM(GAME_ACTION) = a;
    RTS();
}

/* entrance_palette_anims ($97C8 in bank $00, proposed): background palette
 * animation slots 2 and 3 on, with scripts 5 and 6 from their first step.
 * Called by enter_r1a3_done (JSR at 00:97C0). Out: A = 0. */
void func_97C8_b0(void) {
    REGS_IN;

    mff_cycles(0x97C8, 2 + 4 + 4 + 2 + 4 + 2 + 4 + 2 + 4 + 4 + 4 + 4 + 6);
    a = 0x80;
    RAM(BG_PAL_ANIM_ON + 2) = a;
    RAM(BG_PAL_ANIM_ON + 3) = a;
    a = 0x05;
    RAM(BG_PAL_ANIM_SCRIPT + 2) = a;
    a = 0x06;
    RAM(BG_PAL_ANIM_SCRIPT + 3) = a;
    a = 0;
    SET_NZ(a);
    RAM(BG_PAL_ANIM_STEP + 2) = a;
    RAM(BG_PAL_ANIM_TIMER + 2) = a;
    RAM(BG_PAL_ANIM_STEP + 3) = a;
    RAM(BG_PAL_ANIM_TIMER + 3) = a;
    RTS();
}

/*
 * enter_walk ($9819 in bank $00, proposed): game_action $20, walking in
 * from the left (the walking animation and speed unless already walking).
 * In round 1's second area he is in at x $B0, in round 5's
 * third area on its second screen from x $80 (enter_walk_done); elsewhere
 * he just walks.
 */
void func_9819_b0(void) {
    REGS_IN;

    mff_cycles(0x9819, 3 + 4 + 4);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    COMPARE(a, prg_read((uint16_t)(IS_WALKING + y)));
    mff_cycles(0x9821, 2);
    if (!z) {
        mff_cycles(0x9823, 4);
        a = prg_read((uint16_t)(ANIM_WALK + y));
        SET_NZ(a);
        mff_cycles(0x9826, 6);
        JSR_FN(0x9826, func_F294);              /* player_set_anim */
        mff_cycles(0x9829, 4);
        a = prg_read((uint16_t)(CHR_STAND + y));
        SET_NZ(a);
        mff_cycles(0x982C, 6);
        JSR_FN(0x982C, func_F440);              /* player_set_chr */
        mff_cycles(0x982F, 4);
        a = prg_read((uint16_t)(WALK_VX_FRAC + y));
        mff_cycles(0x9832, 4 + 4);
        RAM(OBJ_VX_FRAC) = a;
        a = prg_read((uint16_t)(WALK_VX + y));
        SET_NZ(a);
        mff_cycles(0x9838, 4);
        RAM(OBJ_VX) = a;
    } else {
        mff_cycles(0x9821, 1);
    }
    mff_cycles(0x983B, 6);
    JSR_FN(0x983B, func_F183);                  /* obj_move_right */
    mff_cycles(0x983E, 3 + 2);
    a = RAM(ROUND);
    if (a == 0) {
        mff_cycles(0x9842, 3 + 2 + 2);
        a = RAM(SUB_AREA);
        COMPARE(a, 1);
        if (z) {
            mff_cycles(0x9848, 4 + 2 + 2);
            a = RAM(OBJ_X);
            COMPARE(a, 0xB0);
            if (c)
                FALL_INTO(func_984F_b0);        /* enter_walk_done */
            mff_cycles(0x984D, 1);
        } else {
            mff_cycles(0x9846, 1);
        }
        mff_cycles(0x985E, 6);
        RTS();
    }
    mff_cycles(0x9840, 1 + 3 + 2 + 2);
    a = RAM(ROUND);
    COMPARE(a, 4);
    if (z) {
        mff_cycles(0x9865, 3 + 2 + 2);
        a = RAM(SUB_AREA);
        COMPARE(a, 2);
        if (z) {
            mff_cycles(0x986B, 4 + 2);
            a = RAM(OBJ_X_HI);
            SET_NZ(a);
            if (!z) {
                mff_cycles(0x9870, 4 + 2 + 2);
                a = RAM(OBJ_X);
                COMPARE(a, 0x80);
                if (c) {
                    mff_cycles(0x9877, 3);
                    JMP_TAIL(0x984F, 0);        /* enter_walk_done */
                }
            }
        }
    }
    mff_cycles(0x985E, 1 + 6);                  /* a branch taken, RTS */
    RTS();
}

/* enter_walk_done ($984F in bank $00, proposed): the player is in:
 * obj_state[0] $80, game_action and obj_timer[0] 0, player_stand. Reached
 * by JMP (00:9877) and run on into from enter_walk. Out: A = 0. */
void func_984F_b0(void) {
    REGS_IN;

    mff_cycles(0x984F, 2 + 4 + 2 + 3 + 4 + 6);
    a = STATE_LIVE;
    RAM(OBJ_STATE) = a;
    a = 0;
    SET_NZ(a);
    RAM(GAME_ACTION) = a;
    RAM(OBJ_TIMER) = a;
    JSR_AT(0x985B, 0xCEAA, 0);                  /* player_stand */
    mff_cycles(0x985E, 6);
    RTS();
}

/*
 * scenery_change ($987A in bank $00, proposed): copies the VRAM list Y
 * ($989A/$989E: $98A2, $98DA, $9918, $9927), 128 bytes, into vram_buffer
 * ($0780) and requests it (vram_list_request = $80). X is kept (through
 * $0F). Called by the exits and entrances (JSR at 00:948A, 949D, 959A,
 * 967C).
 */
void func_987A_b0(void) {
    REGS_IN;

    mff_cycles(0x987A, 3 + 4);
    RAM(TMP15) = x;
    a = prg_read((uint16_t)(SCENERY_LIST_LO + y));
    mff_cycles(0x987F, 3 + 4);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(SCENERY_LIST_HI + y));
    mff_cycles(0x9884, 3 + 2 + 2 + 2);
    RAM(TMP1) = a;
    a = 0;
    x = a;
    y = a;
    for (;;) {
        mff_cycles(0x988A, 5);
        a = mem_read((uint16_t)(RAM16(TMP0) + y));
        mff_cycles(0x988C, 5 + 2 + 2 + 2 + 2);
        RAM(VRAM_BUFFER + x) = a;
        x++;
        y++;
        COMPARE(y, 0x80);
        if (z)
            break;
        mff_cycles(0x9893, 1);
    }
    mff_cycles(0x9895, 3 + 3 + 6);
    RAM(VRAM_LIST_REQUEST) = y;
    x = RAM(TMP15);
    SET_NZ(x);
    RTS();
}
