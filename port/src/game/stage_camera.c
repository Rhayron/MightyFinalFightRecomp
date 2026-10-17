/*
 * stage_camera.c - the camera (docs/assets.md, "How the game draws it")
 *
 * The camera is scroll_x_hi:scroll_x ($F9/$FC), the world x of the left
 * edge of the screen. It only ever moves right, by at most 3 pixels a
 * frame, following the player once he walks past the middle of the screen.
 * When it crosses onto the next screen it snaps to that screen's left edge
 * and locks there (camera_lock $50, camera_autoscroll $BB) until the
 * screen's enemies are beaten and the lock is lifted (05:AA9B); then, as
 * soon as the player walks past the middle, game_action $1D walks him on
 * while the camera scrolls one pixel a frame by itself up to the next
 * boundary. Whenever the camera rests on a boundary the player is kept on
 * the screen. Every scroll goes on to the column check
 * (stage_columns.c), which streams the background column that came into
 * view.
 *
 * camera_update runs once per gameplay frame, from gameplay_update
 * ($C94B), after the player's action code has moved him; gameplay_update
 * then saves obj_x and scroll_x in player_x_prev ($27) and scroll_x_prev
 * ($25) for the next frame.
 *
 * Scratch: $00 the player's x on the screen, $01 the scroll step, $02 the
 * player's movement this frame, $0F the left margin, $10 the direction of
 * travel (DIR_RIGHT or DIR_LEFT; the column writer reads it).
 */
#include "stage.h"

/* game_action values during which the camera does nothing at all
 * ($E2A5-$E2BB): knocked down, the round 1 entrance, lying down, Cody's
 * throw, the continue screen and $0D */
static const uint8_t k_camera_still[] = { 0x06, 0x1B, 0x1F, 0x0B, 0x09, 0x0D };

/* game_action values that hold back the automatic walk to the next screen
 * ($E3B5-$E3E7): jumps, specials, throws, weapon hits and falls ($24 is in
 * the list twice) */
static const uint8_t k_autowalk_wait[] = {
    0x01, 0x17, 0x18, 0x23, 0x11, 0x25, 0x24, 0x15, 0x0F, 0x22, 0x24, 0x02, 0x14,
};

/*
 * Is the player Haggar holding an enemy (game_action 4), and which way does
 * he face? The same test sits at $E31B, $E477 and $E4A3: LDA/CMP/branch on
 * player_id, game_action and player_dir, 7 cycles each, +1 where the branch
 * is taken (a mismatch, or player_dir left). Returns 0 (not Haggar's grab),
 * DIR_RIGHT or DIR_LEFT. The A and flags it would leave are dead: every
 * place it is used loads and compares again first.
 */
static int haggar_grab_facing(uint16_t pc) {
    mff_cycles(pc, 7);
    if (RAM(PLAYER_ID) != PLAYER_HAGGAR) {
        mff_cycles((uint16_t)(pc + 4), 1);
        return 0;
    }
    mff_cycles((uint16_t)(pc + 6), 7);
    if (RAM(GAME_ACTION) != GA_GRAB) {
        mff_cycles((uint16_t)(pc + 10), 1);
        return 0;
    }
    mff_cycles((uint16_t)(pc + 12), 7);
    if (RAM(PLAYER_DIR) == DIR_LEFT) {
        mff_cycles((uint16_t)(pc + 16), 1);
        return DIR_LEFT;
    }
    return DIR_RIGHT;
}

/*
 * $E353-$E3A2, shared by camera_update ($E319) and camera_scroll_right
 * ($E3A7, $E3AD): the player is in the left half of the screen, or the
 * camera is on the area's last screen. If the camera was walking him to the
 * next screen (game_action $1D), the walk is over: on the third screen of
 * round 1 area 2 (round 0, sub_area 1) the scripted drop into the room
 * below starts (game_action $1E), from the third screen of round 4 area 1
 * (round 3, sub_area 0) game_action $26 would (no route gets there), and
 * otherwise the player stands ($CEAA) and gets the pad back. Then
 * camera_keep_player_on_screen, except after $1E/$26.
 * In: nothing (A and N/Z/C are loaded first; V passes through).
 */
static void camera_walk_end(void) {
    REGS_IN;

    mff_cycles(0xE353, 7);
    a = RAM(GAME_ACTION);
    SET_CMP(a, GA_AUTOWALK);
    if (a == GA_AUTOWALK) {
        mff_cycles(0xE359, 5);
        if (RAM(ROUND) == 0) {
            mff_cycles(0xE35D, 7);
            if (RAM(SUB_AREA) == 1) {
                mff_cycles(0xE363, 7);
                a = RAM(AREA_SCREEN);
                c = a >= 2;
                if (a == 2) {
                    mff_cycles(0xE369, 17);
                    RAM(GAME_ACTION) = 0x1E;
                    a = 0x80;
                    SET_NZ(a);
                    RAM(OBJ_STATE) = a;
                    SYNC_OUT;
                    mff_rts();
                    return;
                }
                mff_cycles(0xE367, 1);
            } else {
                mff_cycles(0xE361, 1);
            }
        } else {
            mff_cycles(0xE35B, 1);
        }
        mff_cycles(0xE373, 7);
        a = RAM(ROUND);
        SET_CMP(a, 3);
        if (z) {
            mff_cycles(0xE379, 5);
            a = RAM(SUB_AREA);
            SET_NZ(a);
            if (z) {
                mff_cycles(0xE37D, 7);
                a = RAM(AREA_SCREEN);
                SET_CMP(a, 2);
                if (c) {
                    mff_cycles(0xE383, 17);
                    RAM(GAME_ACTION) = 0x26;
                    a = 0x80;
                    SET_NZ(a);
                    RAM(OBJ_STATE) = a;
                    SYNC_OUT;
                    mff_rts();
                    return;
                }
                mff_cycles(0xE381, 1);
            } else {
                mff_cycles(0xE37B, 1);
            }
        } else {
            mff_cycles(0xE377, 1);
        }

        /* stand still and take the pad back */
        mff_cycles(0xE38D, 6);
        SYNC_OUT;
        if (!mff_call_at(0x038F, 0xCEAA, -1)) return;
        SYNC_IN;
        mff_cycles(0xE390, 17);
        a = GA_IDLE;
        SET_NZ(a);
        RAM(GAME_ACTION) = a;
        RAM(CAMERA_AUTOSCROLL) = a;
        RAM(JUMP_MOVING) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x039A, func_F436, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xE39B, 6 + 3);
        a = 0x80;
        SET_NZ(a);
        RAM(OBJ_STATE) = a;
    } else {
        mff_cycles(0xE357, 1 + 3);
    }
    SYNC_OUT;
    stage_jmp(0xE473, func_E473);
}

/*
 * camera_update, $E2A3 (proposed name): the camera's work for the frame.
 * Nothing during the actions of k_camera_still; on a locked screen only
 * camera_keep_player_on_screen. Otherwise it measures the player against
 * the middle of the screen: moving right or standing, camera_scroll_right
 * decides the scroll; moving left, the camera stays and the player is held
 * $18 pixels from the left edge ($24 when Haggar carries an enemy facing
 * left). Scratch as at the top of the file.
 * In: nothing. Out: A and the flags of the path's last instruction (the
 * caller reloads A and does not read them).
 * Called by: gameplay_update, JSR at $C94B (gameplay.c).
 */
void func_E2A3(void) {
    REGS_IN;
    unsigned i;
    uint8_t margin;

    mff_cycles(0xE2A3, 3);
    a = RAM(GAME_ACTION);
    for (i = 0; i < sizeof k_camera_still; i++) {
        mff_cycles((uint16_t)(0xE2A5 + 4 * i), 4);         /* CMP #, BEQ */
        SET_CMP(a, k_camera_still[i]);
        if (z) {
            mff_cycles((uint16_t)(0xE2A7 + 4 * i), 1 + 6);  /* taken, RTS */
            SYNC_OUT;
            mff_rts();
            return;
        }
    }
    mff_cycles(0xE2BD, 3);                                  /* BNE, always taken */

    /* locked on a screen, or an area that does not scroll (none does not) */
    mff_cycles(0xE2C0, 5);
    a = RAM(CAMERA_LOCK);
    SET_NZ(a);
    if (a != 0) {
        mff_cycles(0xE2C4, 3);
        SYNC_OUT;
        stage_jmp(0xE473, func_E473);
        return;
    }
    mff_cycles(0xE2C2, 1 + 7);
    a = RAM(AREA_FLAGS) & AREA_SCROLLS;
    SET_NZ(a);
    if (a == 0) {
        mff_cycles(0xE2CD, 3);
        SYNC_OUT;
        stage_jmp(0xE473, func_E473);
        return;
    }

    /* $00 = the player's screen x, $01 = his distance from the middle */
    mff_cycles(0xE2CB, 1 + 26);
    RAM(TMP16) = DIR_RIGHT;
    RAM(CAMERA_DIR) = DIR_RIGHT;
    a = (uint8_t)(RAM(OBJ_X) - RAM(SCROLL_X));
    RAM(TMP0) = a;
    if (a < 0x80) {
        mff_cycles(0xE2E3, 4);
        a = (uint8_t)(0x80 - a);
    } else {
        mff_cycles(0xE2E1, 1);
        a = (uint8_t)(a - 0x80);
    }

    /* $02 = his movement since last frame */
    mff_cycles(0xE2E7, 17);
    RAM(TMP1) = a;
    a = RAM(OBJ_X);
    c = 1;
    SBC_A(RAM(PLAYER_X_PREV));
    RAM(TMP2) = a;
    if (!n) {
        mff_cycles(0xE2F3, 3);
        SYNC_OUT;
        stage_jmp(0xE3A3, func_E3A3);
        return;
    }

    /* moving left: $02 = the speed; $01 = the speed once he is $19 or more
     * from the middle, at most $18 (not used on this path) */
    mff_cycles(0xE2F1, 1 + 16);
    a ^= 0xFF;
    c = 0;
    ADC_A(1);
    RAM(TMP2) = a;
    if (RAM(TMP1) >= 0x19) {
        mff_cycles(0xE303, 6);
        RAM(TMP1) = RAM(TMP2);
    } else {
        mff_cycles(0xE301, 1);
    }
    mff_cycles(0xE307, 7);
    if (RAM(TMP1) > 0x18) {
        mff_cycles(0xE30D, 3);
        RAM(TMP1) = 0x18;
    } else {
        mff_cycles(0xE30B, 1);
    }
    mff_cycles(0xE30F, 15);
    RAM(TMP16) = DIR_LEFT;
    RAM(CAMERA_DIR) = DIR_LEFT;
    if (RAM(TMP0) >= 0x80) {                                /* right half */
        mff_cycles(0xE319, 1);
        SYNC_OUT;
        camera_walk_end();
        return;
    }

    /* left half: hold him `margin` pixels from the left edge */
    switch (haggar_grab_facing(0xE31B)) {
    case DIR_LEFT:
        mff_cycles(0xE333, 2);
        margin = 0x24;
        break;
    case DIR_RIGHT:
        mff_cycles(0xE32D, 3);                              /* BNE, always taken */
        /* fall through */
    default:
        mff_cycles(0xE32F, 5);                              /* LDA #, BNE taken */
        margin = 0x18;
        break;
    }
    mff_cycles(0xE335, 20);
    RAM(TMP15) = margin;
    a = RAM(OBJ_X);
    c = 1;
    SBC_A(RAM(SCROLL_X));
    RAM(TMP0) = a;
    SET_CMP(a, margin);
    if (!c) {
        mff_cycles(0xE343, 21);
        a = RAM(SCROLL_X);
        c = 0;
        ADC_A(margin);
        RAM(OBJ_X) = a;
        a = RAM(SCROLL_X_HI);
        ADC_A(0);
        RAM(OBJ_X_HI) = a;
    } else {
        mff_cycles(0xE341, 1);
    }
    mff_cycles(0xE352, 6);
    SYNC_OUT;
    mff_rts();
}

/*
 * camera_scroll_right, $E3A3 (proposed name): the player moves right or
 * stands, $00/$01/$02 as camera_update left them. In the left half, or on
 * the area's last screen, camera_walk_end. When the lock has just been
 * lifted (camera_autoscroll) the automatic walk starts as soon as the
 * player is not busy (k_autowalk_wait), and then scrolls one pixel a frame.
 * Otherwise the camera follows: the step is the player's movement once he
 * is $19 or more past the middle (1 for Cody's slash standing there), at
 * most $18, and any step of 4 or more becomes 1. A scroll that crosses
 * onto the next screen snaps to it and locks the camera. Every scroll ends
 * in bg_column_check; no step ends in camera_keep_player_on_screen.
 * Called by: camera_update, JMP at $E2F3.
 */
void func_E3A3(void) {
    REGS_IN;
    unsigned i;
    uint8_t step;

    mff_cycles(0xE3A3, 7);
    if (RAM(TMP0) < 0x81) {
        mff_cycles(0xE3A7, 1);
        SYNC_OUT;
        camera_walk_end();
        return;
    }
    mff_cycles(0xE3A9, 8);
    if (RAM(AREA_SCREEN) == RAM(AREA_SCREEN_COUNT)) {
        mff_cycles(0xE3AD, 1);
        SYNC_OUT;
        camera_walk_end();
        return;
    }

    mff_cycles(0xE3AF, 5);
    if (RAM(CAMERA_AUTOSCROLL) != 0) {
        /* the walk to the next screen: wait while the player is busy */
        mff_cycles(0xE3B3, 3);
        a = RAM(GAME_ACTION);
        for (i = 0; i < sizeof k_autowalk_wait; i++) {
            mff_cycles((uint16_t)(0xE3B5 + 4 * i), 4);      /* CMP #, BEQ */
            SET_CMP(a, k_autowalk_wait[i]);
            if (z) {
                mff_cycles((uint16_t)(0xE3B7 + 4 * i), 1 + 6);
                SYNC_OUT;
                mff_rts();
                return;
            }
        }
        mff_cycles(0xE3E9, 4);
        if (a == GA_AUTOWALK) {
            /* walking: one pixel a frame */
            mff_cycles(0xE3EB, 2 + 12);
            a = RAM(SCROLL_X);
            c = 0;
            ADC_A(1);
            RAM(SCROLL_X) = a;
            if (c) {                                        /* no route */
                mff_cycles(0xE412, 11);
                t = (uint8_t)(RAM(AREA_SCREEN) + 1);
                RAM(AREA_SCREEN) = t;
                SET_NZ(t);
                SYNC_OUT;
                mff_rts();
                return;
            }
            mff_cycles(0xE410, 1 + 3);
            SYNC_OUT;
            stage_jmp(0xE4DF, func_E4DF);
            return;
        }
        mff_cycles(0xE3ED, 7);
        a = RAM(TMP0);
        SET_CMP(a, 0x81);
        if (c) {                                            /* always: checked above */
            mff_cycles(0xE3F3, 5 + 6);
            a = GA_AUTOWALK;
            SET_NZ(a);
            RAM(GAME_ACTION) = a;
            SYNC_OUT;
            mff_rts();
            return;
        }
        mff_cycles(0xE3F1, 1 + 6);
        SYNC_OUT;
        if (!mff_call_at(0x03FA, 0xCEAA, -1)) return;
        SYNC_IN;
        mff_cycles(0xE3FB, 23);
        a = GA_IDLE;
        RAM(GAME_ACTION) = a;
        RAM(CAMERA_AUTOSCROLL) = a;
        RAM(JUMP_MOVING) = a;
        a = 0x80;
        SET_NZ(a);
        RAM(OBJ_STATE) = a;
        SYNC_OUT;
        mff_rts();
        return;
    }

    /* follow the player */
    mff_cycles(0xE3B1, 2 + 7);                              /* BEQ across a page */
    if (RAM(TMP1) >= 0x19) {
        mff_cycles(0xE41E, 8);
        RAM(TMP1) = RAM(TMP2);
        if (RAM(TMP1) == 0) {
            mff_cycles(0xE424, 7);
            if (RAM(GAME_ACTION) == GA_CODY_SLASH) {
                mff_cycles(0xE42A, 5);
                RAM(TMP1) = 1;
            } else {
                mff_cycles(0xE428, 1);
            }
        } else {
            mff_cycles(0xE422, 1);
        }
    } else {
        mff_cycles(0xE41C, 1);
    }
    mff_cycles(0xE42E, 7);
    if (RAM(TMP1) > 0x18) {
        mff_cycles(0xE434, 3);
        RAM(TMP1) = 0x18;
    } else {
        mff_cycles(0xE432, 1);
    }
    mff_cycles(0xE436, 7);
    c = RAM(TMP1) >= 4;
    if (c) {
        mff_cycles(0xE43C, 5);
        RAM(TMP1) = 1;
    } else {
        mff_cycles(0xE43A, 1);
    }
    mff_cycles(0xE440, 5);
    step = RAM(TMP1);
    if (step == 0) {
        mff_cycles(0xE442, 1);
        SYNC_OUT;
        func_E473();
        return;
    }

    mff_cycles(0xE444, 13);
    a = RAM(SCROLL_X);
    c = 0;
    ADC_A(step);
    RAM(SCROLL_X) = a;
    if (c) {
        /* onto the next screen: snap to its edge and lock */
        mff_cycles(0xE44D, 13);
        RAM(AREA_SCREEN)++;
        a = RAM(AREA_SCREEN);
        SET_CMP(a, RAM(AREA_SCREEN_COUNT));
        if (z) {                                            /* the area's last screen */
            mff_cycles(0xE455, 13);
            RAM(SCROLL_X) = 0;
            a = 0xF0;
            SET_CMP(a, RAM(OBJ_X));
            if (!c) {
                mff_cycles(0xE460, 4);
                RAM(OBJ_X) = a;
            } else {
                mff_cycles(0xE45E, 1);
            }
        } else {
            mff_cycles(0xE453, 1);
        }
        mff_cycles(0xE463, 18);
        RAM(CAMERA_LOCK) = 0xFF;
        RAM(CAMERA_AUTOSCROLL) = 0xFF;
        a = 0;
        RAM(SCROLL_X) = a;
        t = (uint8_t)(RAM(SCROLL_X_HI) + 1);
        RAM(SCROLL_X_HI) = t;
        SET_NZ(t);
    } else {
        mff_cycles(0xE44B, 1);
    }
    mff_cycles(0xE46F, 3);
    SYNC_OUT;
    stage_jmp(0xE4DF, func_E4DF);
}

/*
 * camera_keep_player_on_screen, $E473 (proposed name): when the camera
 * rests on a screen boundary (scroll_x 0: locked, or at the area's start)
 * the player stays on the screen: obj_x at least $18 and at most $F0.
 * Haggar carrying an enemy needs room for it on the side he faces: at
 * least $20 facing left (no route got there), at most $E8 facing right.
 * The area-exit walk (game_action $19), and the skip-stage action ($0E)
 * except for Haggar's grab, may cross the right edge.
 * In: nothing (V and, when scroll_x is not 0, C pass through). Out: A and
 * N/Z/C of the last load or compare.
 * Called by: camera_update (JMP at $E2C4, $E2CD, and $E3A0 in
 * camera_walk_end) and camera_scroll_right (BEQ at $E440).
 */
void func_E473(void) {
    REGS_IN;

    mff_cycles(0xE473, 5);
    a = RAM(SCROLL_X);
    SET_NZ(a);
    if (a != 0) {
        mff_cycles(0xE475, 1 + 6);
        SYNC_OUT;
        mff_rts();
        return;
    }

    /* the left edge */
    if (haggar_grab_facing(0xE477) == DIR_LEFT) {
        mff_cycles(0xE496, 8);
        a = RAM(OBJ_X);
        c = a >= 0x20;
        if (!c) {
            mff_cycles(0xE49D, 12);
            a = 0x20;
            SET_NZ(a);
            RAM(OBJ_X) = a;
            SYNC_OUT;
            mff_rts();
            return;
        }
        mff_cycles(0xE49B, 1);
    } else {
        mff_cycles(0xE489, 8);
        a = RAM(OBJ_X);
        c = a >= 0x18;
        if (!c) {
            mff_cycles(0xE490, 6 + 6);
            a = 0x18;
            SET_NZ(a);
            RAM(OBJ_X) = a;
            SYNC_OUT;
            mff_rts();
            return;
        }
        mff_cycles(0xE48E, 1);
    }

    /* the right edge */
    if (haggar_grab_facing(0xE4A3) == DIR_RIGHT) {
        mff_cycles(0xE4B5, 8);
        a = RAM(OBJ_X);
        SET_CMP(a, 0xE8);
        if (!c) {
            mff_cycles(0xE4BA, 1 + 6);
            SYNC_OUT;
            mff_rts();
            return;
        }
        mff_cycles(0xE4BC, 7);
        a = RAM(GAME_ACTION);
        SET_CMP(a, GA_AREA_EXIT);
        if (z) {
            mff_cycles(0xE4C0, 1 + 6);
            SYNC_OUT;
            mff_rts();
            return;
        }
        mff_cycles(0xE4C2, 12);
        a = 0xE8;
        SET_NZ(a);
        RAM(OBJ_X) = a;
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0xE4C8, 8);
    a = RAM(OBJ_X);
    SET_CMP(a, 0xF0);
    if (!c) {
        mff_cycles(0xE4CD, 1 + 6);
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0xE4CF, 7);
    a = RAM(GAME_ACTION);
    SET_CMP(a, GA_AREA_EXIT);
    if (!z) {
        mff_cycles(0xE4D5, 4);
        SET_CMP(a, GA_SKIP_STAGE);
        if (!z) {
            mff_cycles(0xE4D9, 6);
            a = 0xF0;
            SET_NZ(a);
            RAM(OBJ_X) = a;
        } else {
            mff_cycles(0xE4D7, 1);
        }
    } else {
        mff_cycles(0xE4D3, 1);
    }
    mff_cycles(0xE4DE, 6);
    SYNC_OUT;
    mff_rts();
}
