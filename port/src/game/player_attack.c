/*
 * player_attack.c - attacks from the ground: the punch state (game_action
 * 3), each character's combo, and the step into the hitting position.
 *
 * player_state_idle starts an attack with B (0E:CB99, CCA2): game_action 3
 * and, inside a combo (combo_timer running), the animation kept in
 * player_stand_anim, else the first attack $E1EF[player_id]. The punch state
 * then runs the character's handler for obj_anim. A handler makes the attack
 * live on one animation frame (attack_begin $F43B, or attack_start $F428,
 * which also sets attack_id 0; both with the swing sound) and ends it on a
 * later one (attack_end $F436). At the end of a step it saves the combo's
 * next animation in player_stand_anim and enters the hitting position
 * (player_hitpos_start, $D79A: game_action $0C for 8 frames), where B
 * continues the combo (player_state_hitpos, $DD6A).
 *
 * The combos, by obj_anim (attack_id of each hit in brackets; "|" is a stop
 * in the hitting position):
 *   Cody    $0D [0] | $16 [0] | $10 [6] | $13: game_action $14 (bank $00)
 *   Guy     $0E [0, 0] -> $55 -> $56 [0] | $17 [0, 0] -> $02 -> $59 [0] |
 *           $11 [6] | $14 [1] -> $5B: a jump that lands back in the stance
 *   Haggar  $0F [$0B] -> $3F | $18 [$0B] -> $4E | $12 [$0C] | $15 [$0D]: a
 *           jump that lands in game_action 2
 */
#include "player_defs.h"

/* the handler of each character's combo (player_attack_by_character) */
#define ATTACK_HANDLER_LO   0xD3AD   /* B3 7D 41 */
#define ATTACK_HANDLER_HI   0xD3B0   /* D3 D4 D6: $D3B3, $D47D, $D641 */

static void attack_turn(void);
static void cody_jab(void);
static void cody_punch(void);
static void guy_strike_frames(void);
static void haggar_jab(void);
static void haggar_wait(void);
static void haggar_jab2(void);
static void hitpos_wait(int dispatch_end);
static void haggar_hitpos_pose(int dispatch_wait);

/*
 * player_state_punch ($D30F, labels.toml): game_action 3. player_state_idle
 * JMPs here when it starts an attack (0E:CB9D); after that player_state
 * dispatches here through its table (entry 3).
 *  - A+B held, health left and $B2 not negative: the special
 *    (player_special_start, $CA3C), for Guy and Haggar only outside the
 *    animations $E29D/$E2A0[player_id] (Guy $14/$5B, Haggar $15).
 *  - from player_level 3 on for Cody and Guy, 1 on for Haggar: the power
 *    move check (player_attack_power, $D346).
 *  - then attack_turn and the character's handler.
 */
void func_D30F(void) {
    REGS_IN;

    mff_cycles(0xD30F, 3 + 2 + 2);
    a = RAM(JOY_HELD);
    COMPARE(a, PAD_A | PAD_B);
    if (z) {
        mff_cycles(0xD315, 3 + 2);
        a = RAM(HEALTH);
        if (a != 0) {
            mff_cycles(0xD319, 3 + 2);
            a = RAM(BONUS_STAGE);
            if (!(a & 0x80)) {
                mff_cycles(0xD31D, 3 + 2);
                y = RAM(PLAYER_ID);
                SET_NZ(y);
                if (z) {                        /* Cody: always */
                    mff_cycles(0xD31F, 1 + 3);
                    JMP_TAIL(0xCA3C, -1);       /* player_special_start */
                }
                mff_cycles(0xD321, 4 + 4);
                a = RAM(OBJ_ANIM);
                mff_cycles(0xD327, 2);
                if (a != prg_read_ix(NO_SPECIAL_ANIM_1, y)) {
                    mff_cycles(0xD329, 4);
                    COMPARE(a, prg_read_ix(NO_SPECIAL_ANIM_2, y));
                    mff_cycles(0xD32C, 2);
                    if (!z) {
                        mff_cycles(0xD32E, 3);
                        JMP_TAIL(0xCA3C, -1);   /* player_special_start */
                    }
                }
            }
        }
    }

    mff_cycles(0xD313, 1 + 3 + 2 + 2);          /* every way here took a branch */
    a = RAM(PLAYER_ID);
    if (a != PLAYER_HAGGAR) {
        mff_cycles(0xD337, 3 + 2 + 2);
        a = RAM(PLAYER_LEVEL);
        COMPARE(a, 3);
        if (c) {
            mff_cycles(0xD33D, 3);
            JMP_TAIL(0xD346, -1);               /* player_attack_power */
        }
        mff_cycles(0xD33B, 1);
    } else {
        mff_cycles(0xD335, 1 + 3 + 2 + 2);
        a = RAM(PLAYER_LEVEL);
        if (a >= 1)
            FALL_INTO(func_D346);               /* player_attack_power */
        mff_cycles(0xD344, 1);
    }
    FALL_INTO(attack_turn);
}

/*
 * player_attack_power ($D346, proposed): the power move. Forward (the pad
 * direction just pressed equals player_dir) with B and no other button held
 * starts game_action $FCCE[player_id] (Cody $23, Guy $27, Haggar $10; Data
 * Crystal: each one's power) with obj_state[0] $80, and ends the attack
 * (attack_end) - outside the animations that keep out the special. Never
 * executed in any run (player_level stays 0 in the bot logs). Otherwise on to
 * attack_turn. Reached from player_state_punch: JMP for Cody and Guy
 * (0E:D33D), run on into for Haggar.
 */
void func_D346(void) {
    REGS_IN;

    mff_cycles(0xD346, 3 + 2 + 3 + 2);
    a = RAM(JOY_PRESSED) & 0x0F;
    if (a == RAM(PLAYER_DIR)) {
        mff_cycles(0xD34E, 3 + 2 + 2 + 2);
        a = RAM(JOY_HELD) & 0xF0;
        if (a == PAD_B) {
            mff_cycles(0xD356, 3 + 4 + 4);
            y = RAM(PLAYER_ID);
            a = RAM(OBJ_ANIM);
            mff_cycles(0xD35E, 2);
            if (a != prg_read_ix(NO_SPECIAL_ANIM_1, y)) {
                mff_cycles(0xD360, 4);
                COMPARE(a, prg_read_ix(NO_SPECIAL_ANIM_2, y));
                mff_cycles(0xD363, 2);
                if (!z) {
                    mff_cycles(0xD365, 4);
                    a = prg_read_ix(POWER_ACTION, y);
                    mff_cycles(0xD368, 3 + 2 + 4 + 6);
                    RAM(GAME_ACTION) = a;
                    a = STATE_LIVE;
                    SET_NZ(a);
                    RAM(OBJ_STATE) = a;
                    JSR_FN(0xD36F, func_F436);  /* attack_end */
                    mff_cycles(0xD372, 6);
                    RTS();
                }
            }
        }
    }
    mff_cycles(0xD34C, 1);                      /* every way here took a branch */
    FALL_INTO(attack_turn);
}

/*
 * attack_turn (0E:D373, inside player_state_punch and player_attack_power):
 * in the first animation of a combo step ($E288/$E28B/$E28E[player_id]),
 * left or right held turns the player: player_dir takes it, and
 * player_face_right/left flips the sprite. Then
 * player_attack_by_character, by JMP when not in those animations.
 */
static void attack_turn(void) {
    REGS_IN;

    mff_cycles(0xD373, 3 + 4 + 4);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    COMPARE(a, prg_read_ix(COMBO_FIRST_ANIM, y));
    mff_cycles(0xD37B, 2);
    if (!z) {
        mff_cycles(0xD37D, 4);
        COMPARE(a, prg_read_ix(TURN_ANIM_2, y));
        mff_cycles(0xD380, 2);
        if (!z) {
            mff_cycles(0xD382, 4);
            COMPARE(a, prg_read_ix(TURN_ANIM_3, y));
            mff_cycles(0xD385, 2);
            if (!z) {
                mff_cycles(0xD387, 3);
                JMP_TAIL(0xD39E, -1);           /* player_attack_by_character */
            }
        }
    }
    mff_cycles(0xD37B, 1 + 3 + 2 + 2);          /* BEQ taken */
    a = RAM(JOY_HELD) & (PAD_LEFT | PAD_RIGHT);
    if (a != 0) {
        mff_cycles(0xD390, 3 + 2 + 2);
        RAM(PLAYER_DIR) = a;
        a &= PAD_RIGHT;
        SET_NZ(a);
        if (!z) {
            mff_cycles(0xD396, 6);
            JSR_FN(0xD396, func_F03F);          /* player_face_right */
            mff_cycles(0xD399, 2);
            if (!z) {                           /* always: A = obj_flags | $40 */
                mff_cycles(0xD399, 1);
                FALL_INTO(func_D39E);
            }
        } else {
            mff_cycles(0xD394, 1);
        }
        mff_cycles(0xD39B, 6);
        JSR_FN(0xD39B, func_F048);              /* player_face_left */
    } else {
        mff_cycles(0xD38E, 1);
    }
    FALL_INTO(func_D39E);
}

/*
 * player_attack_by_character ($D39E, proposed): JMP ($0000) to the
 * character's attack handler from $D3AD/$D3B0[player_id]: cody_attack $D3B3,
 * guy_attack $D47D, haggar_attack $D641. Reached by JMP from attack_turn
 * (0E:D387) and run on into from it.
 * Out: $00/$01 = the handler, A = its high byte.
 */
void func_D39E(void) {
    REGS_IN;

    mff_cycles(0xD39E, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read_ix(ATTACK_HANDLER_LO, y);
    mff_cycles(0xD3A3, 3 + 4);
    RAM(TMP0) = a;
    a = prg_read_ix(ATTACK_HANDLER_HI, y);
    SET_NZ(a);
    mff_cycles(0xD3A8, 3 + 5);
    RAM(TMP1) = a;
    JMP_IND(TMP0);
}

/*
 * cody_attack ($D3B3, proposed): Cody's handler, by obj_anim. The jabs $0D
 * and $16 (cody_jab) and the punch $10 (cody_punch); $13 ends the combo
 * (cody_finisher, $D438). Any other animation (never executed in any run):
 * attack_begin with attack_id 8 on frame 1, and on frame 2 at timer 4
 * attack_end, sound $1E and the hitting position.
 */
void func_D3B3(void) {
    REGS_IN;

    mff_cycles(0xD3B3, 4 + 2 + 2);
    a = RAM(OBJ_ANIM);
    if (a == 0x0D) {
        mff_cycles(0xD3B8, 1);
        FALL_INTO(cody_jab);
    }
    mff_cycles(0xD3BA, 2 + 2);
    if (a == 0x16) {
        mff_cycles(0xD3BC, 1);
        FALL_INTO(cody_jab);
    }
    mff_cycles(0xD3BE, 2 + 2);
    if (a == 0x10) {
        mff_cycles(0xD3C0, 2);
        FALL_INTO(cody_punch);
    }
    mff_cycles(0xD3C2, 2 + 2);
    COMPARE(a, 0x13);
    if (z) {
        mff_cycles(0xD3C6, 3);
        JMP_TAIL(0xD438, -1);                   /* cody_finisher */
    }

    mff_cycles(0xD3C4, 1 + 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 1);
    if (z) {
        mff_cycles(0xD3D0, 6);
        JSR_FN(0xD3D0, func_F43B);              /* attack_begin */
        mff_cycles(0xD3D3, 2 + 3);
        a = 0x08;
        RAM(ATTACK_ID) = a;
    } else {
        mff_cycles(0xD3CE, 1);
    }
    mff_cycles(0xD3D7, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 2);
    if (z) {
        mff_cycles(0xD3DE, 4 + 2 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        COMPARE(a, 4);
        if (z) {
            mff_cycles(0xD3E5, 6);
            JSR_FN(0xD3E5, func_F436);          /* attack_end */
            mff_cycles(0xD3E8, 2 + 6);
            a = 0x1E;
            SET_NZ(a);
            JSR_FN(0xD3EA, func_F34C);          /* sound_queue_push_sfx */
            mff_cycles(0xD3ED, 3);
            JMP_TAIL(0xD477, -1);               /* player_attack_to_hitpos */
        }
    }
    mff_cycles(0xD3DC, 1 + 6);                  /* BNE taken, RTS */
    RTS();
}

/* cody_jab (0E:D3F1), animations $0D and $16: attack_start on frame 2 at
 * timer 0, attack_end on frame 3 and the next step (cody_combo_next). */
static void cody_jab(void) {
    REGS_IN;

    mff_cycles(0xD3F1, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 2);
    if (z) {
        mff_cycles(0xD3F8, 4 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        SET_NZ(a);
        if (z) {
            mff_cycles(0xD3FD, 6);
            JSR_FN(0xD3FD, func_F428);          /* attack_start */
        } else {
            mff_cycles(0xD3FB, 2);
        }
    } else {
        mff_cycles(0xD3F6, 2);
    }
    mff_cycles(0xD400, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 3);
    if (z) {
        mff_cycles(0xD407, 6);
        JSR_FN(0xD407, func_F436);              /* attack_end */
        mff_cycles(0xD40A, 3);
        JMP_TAIL(0xD45E, -1);                   /* cody_combo_next */
    }
    mff_cycles(0xD405, 1 + 6);
    RTS();
}

/* cody_punch (0E:D40E), animation $10: attack_begin with attack_id 6 and the
 * swing sound on frame 2 at timer 0, attack_end on frame 4, the next step
 * on frame 5. */
static void cody_punch(void) {
    REGS_IN;

    mff_cycles(0xD40E, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 2);
    if (z) {
        mff_cycles(0xD415, 4 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        SET_NZ(a);
        if (z) {
            mff_cycles(0xD41A, 6);
            JSR_FN(0xD41A, func_F43B);          /* attack_begin */
            mff_cycles(0xD41D, 2 + 3 + 6);
            a = 0x06;
            SET_NZ(a);
            RAM(ATTACK_ID) = a;
            JSR_FN(0xD421, func_F430);          /* attack_swing_sound */
        } else {
            mff_cycles(0xD418, 1);
        }
    } else {
        mff_cycles(0xD413, 1);
    }
    mff_cycles(0xD424, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 4);
    if (z) {
        mff_cycles(0xD42B, 6);
        JSR_FN(0xD42B, func_F436);              /* attack_end */
    } else {
        mff_cycles(0xD429, 1);
    }
    mff_cycles(0xD42E, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 5);
    if (!z) {
        mff_cycles(0xD433, 1 + 6);
        RTS();
    }
    mff_cycles(0xD435, 3);
    JMP_TAIL(0xD45E, -1);                       /* cody_combo_next */
}

/*
 * cody_finisher ($D438, proposed): animation $13, the end of Cody's combo.
 * The swing sound, then game_action $14 (run in bank $00; Data Crystal:
 * Cody's slash) with action_timer 0, obj_state[0] $80, player_no_hit $80 and
 * player_ground_y = obj_y. Reached by JMP from cody_attack (0E:D3C6).
 */
void func_D438(void) {
    REGS_IN;

    mff_cycles(0xD438, 6);
    JSR_FN(0xD438, func_F430);                  /* attack_swing_sound */
    mff_cycles(0xD43B, 2 + 3 + 2 + 3 + 2 + 4 + 3 + 4 + 3 + 6);
    a = ACT_CODY_SLASH;
    RAM(GAME_ACTION) = a;
    a = 0;
    RAM(ACTION_TIMER) = a;
    a = STATE_LIVE;
    RAM(OBJ_STATE) = a;
    RAM(PLAYER_NO_HIT) = a;
    a = RAM(OBJ_Y);
    SET_NZ(a);
    RAM(PLAYER_GROUND_Y) = a;
    RTS();
}

/*
 * cody_combo_next ($D45E, proposed): the animation after this step goes to
 * player_stand_anim ($0D -> $16, $16 -> $10, else $13), then the hitting
 * position (player_attack_to_hitpos). Reached by JMP from cody_jab and
 * cody_punch (0E:D40A, D435).
 */
void func_D45E(void) {
    REGS_IN;

    mff_cycles(0xD45E, 4 + 2 + 2);
    a = RAM(OBJ_ANIM);
    COMPARE(a, 0x0D);
    if (z) {
        mff_cycles(0xD463, 1 + 2 + 2 + 1);      /* BEQ, LDA, BNE taken */
        a = 0x16;
    } else {
        mff_cycles(0xD465, 2 + 2);
        COMPARE(a, 0x16);
        if (z) {
            mff_cycles(0xD467, 1 + 2 + 2 + 1);
            a = 0x10;
        } else {
            mff_cycles(0xD469, 2 + 1 + 2);
            a = 0x13;
        }
    }
    SET_NZ(a);
    mff_cycles(0xD475, 3);
    RAM(PLAYER_STAND_ANIM) = a;
    FALL_INTO(func_D477);
}

/*
 * player_attack_to_hitpos ($D477, proposed): player_hitpos_start ($D79A),
 * then JMP player_state_hitpos ($DD6A) for this frame. Reached by JMP from
 * cody_attack (0E:D3ED) and run on into from cody_combo_next.
 */
void func_D477(void) {
    REGS_IN;

    mff_cycles(0xD477, 6);
    JSR_AT(0xD477, 0xD79A, -1);                 /* player_hitpos_start */
    mff_cycles(0xD47A, 3);
    JMP_TAIL(0xDD6A, -1);                       /* player_state_hitpos */
}

/* Guy's handlers, by obj_anim, in the order guy_attack tests them */
static const struct {
    uint8_t anim;
    uint16_t handler;
} k_guy_steps[] = {
    { 0x0E, 0xD4CF },
    { 0x55, 0xD4E6 },
    { 0x56, 0xD4F2 },
    { 0x17, 0xD50F },
    { 0x02, 0xD55E },
    { 0x59, 0xD570 },
    { 0x11, 0xD58D },
    { 0x14, 0xD5B7 },
    { 0x5B, 0xD5F3 },
};

/*
 * guy_attack ($D47D, proposed): Guy's handler, a JMP by obj_anim through
 * k_guy_steps. Animation $35 (never executed in any run) puts the held enemy
 * (grab_slot) at the player's obj_y and JMPs to Guy's grab ($D88C); any
 * other returns.
 */
void func_D47D(void) {
    unsigned i;
    REGS_IN;

    mff_cycles(0xD47D, 4);
    a = RAM(OBJ_ANIM);
    for (i = 0; i < sizeof k_guy_steps / sizeof k_guy_steps[0]; i++) {
        mff_cycles(0xD47D, 2 + 2);              /* CMP #, BEQ */
        COMPARE(a, k_guy_steps[i].anim);
        if (z) {
            mff_cycles(0xD47D, 1 + 3);          /* BEQ taken, JMP */
            JMP_TAIL(k_guy_steps[i].handler, -1);
        }
    }
    mff_cycles(0xD4A4, 2 + 2);
    COMPARE(a, 0x35);
    if (z) {
        mff_cycles(0xD4A6, 1 + 3 + 4 + 5 + 3);
        y = RAM(GRAB_SLOT);
        a = RAM(OBJ_Y);
        SET_NZ(a);
        RAM(OBJ_Y + y) = a;
        JMP_TAIL(0xD88C, -1);                   /* guy_grab */
    }
    mff_cycles(0xD4A8, 6);
    RTS();
}

/*
 * guy_jab ($D4CF, proposed): animation $0E. On frame 4 at timer 2 it turns
 * into $55 (with CHR bank 4 and attack_end, guy_step_end $D522); before that
 * guy_strike_frames.
 */
void func_D4CF(void) {
    REGS_IN;

    mff_cycles(0xD4CF, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    if (a == 4) {
        mff_cycles(0xD4D6, 4 + 2 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        COMPARE(a, 2);
        if (z) {
            mff_cycles(0xD4DD, 2 + 6);
            a = 0x55;
            SET_NZ(a);
            JSR_FN(0xD4DF, func_F294);          /* player_set_anim */
            mff_cycles(0xD4E2, 3);
            JMP_TAIL(0xD522, -1);               /* guy_step_end */
        }
    }
    mff_cycles(0xD4D4, 2);                      /* BNE taken, to the next page */
    FALL_INTO(guy_strike_frames);
}

/* $D4E5: an RTS that guy_attack's handlers branch to */
void func_D4E5(void) {
    REGS_IN;

    mff_cycles(0xD4E5, 6);
    RTS();
}

/* guy_attack_55 ($D4E6, proposed): animation $55 turns into $56 (CHR bank 6,
 * guy_set_anim_chr6 $D567) at timer 4. */
void func_D4E6(void) {
    REGS_IN;

    mff_cycles(0xD4E6, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_TIMER);
    COMPARE(a, 4);
    if (!z) {
        mff_cycles(0xD4EB, 1);
        BRANCH_TAIL(0xD4E5, -1);                    /* RTS */
    }
    mff_cycles(0xD4ED, 2 + 3);
    a = 0x56;
    SET_NZ(a);
    JMP_TAIL(0xD567, -1);                       /* guy_set_anim_chr6 */
}

/* guy_attack_56 ($D4F2, proposed): animation $56. attack_start on frame 1 at
 * timer 0; on frame 2 attack_end and the next step (guy_combo_next). */
void func_D4F2(void) {
    REGS_IN;

    mff_cycles(0xD4F2, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 2);
    if (z) {
        mff_cycles(0xD4F9, 6);
        JSR_FN(0xD4F9, func_F436);              /* attack_end */
        mff_cycles(0xD4FC, 3);
        JMP_TAIL(0xD61F, -1);                   /* guy_combo_next */
    }
    mff_cycles(0xD4F7, 1 + 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 1);
    if (!z) {
        mff_cycles(0xD504, 2);
        BRANCH_TAIL(0xD4E5, -1);                    /* RTS */
    }
    mff_cycles(0xD506, 4 + 2);
    a = RAM(OBJ_ANIM_TIMER);
    SET_NZ(a);
    if (!z) {
        mff_cycles(0xD509, 2);
        BRANCH_TAIL(0xD4E5, -1);                    /* RTS */
    }
    mff_cycles(0xD50B, 6);
    JSR_FN(0xD50B, func_F428);                  /* attack_start */
    mff_cycles(0xD50E, 6);
    RTS();
}

/* guy_attack_17 ($D50F, proposed): animation $17. On frame 4 at timer 2 it
 * turns into $02 (CHR bank 4, attack_end); before that guy_strike_frames. */
void func_D50F(void) {
    REGS_IN;

    mff_cycles(0xD50F, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    if (a == 4) {
        mff_cycles(0xD516, 4 + 2 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        COMPARE(a, 2);
        if (z) {
            mff_cycles(0xD51D, 2 + 6);
            a = 0x02;
            SET_NZ(a);
            JSR_FN(0xD51F, func_F294);          /* player_set_anim */
            FALL_INTO(func_D522);
        }
    }
    mff_cycles(0xD514, 1);                      /* BNE taken */
    FALL_INTO(guy_strike_frames);
}

/* guy_step_end ($D522, proposed): CHR bank 4 and attack_end. Reached by JMP
 * from guy_jab (0E:D4E2) and run on into from guy_attack_17. */
void func_D522(void) {
    REGS_IN;

    mff_cycles(0xD522, 2 + 6);
    a = 0x04;
    SET_NZ(a);
    JSR_FN(0xD524, func_F440);                  /* player_set_chr */
    mff_cycles(0xD527, 6);
    JSR_FN(0xD527, func_F436);                  /* attack_end */
    mff_cycles(0xD52A, 6);
    RTS();
}

/* guy_strike_frames (0E:D52B, inside guy_jab and guy_attack_17): attack_start
 * on frames 1 and 4 at timer 0, attack_end on frame 2 (and on frame 5, which
 * never comes: both animations change on frame 4). */
static void guy_strike_frames(void) {
    REGS_IN;

    mff_cycles(0xD52B, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 1);
    if (z) {
        mff_cycles(0xD532, 4 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        SET_NZ(a);
        if (z) {
            mff_cycles(0xD537, 6);
            JSR_FN(0xD537, func_F428);          /* attack_start */
        } else {
            mff_cycles(0xD535, 1);
        }
    } else {
        mff_cycles(0xD530, 1);
    }
    mff_cycles(0xD53A, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 2);
    if (z) {
        mff_cycles(0xD541, 6);
        JSR_FN(0xD541, func_F436);              /* attack_end */
    } else {
        mff_cycles(0xD53F, 1);
    }
    mff_cycles(0xD544, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 4);
    if (z) {
        mff_cycles(0xD54B, 4 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        SET_NZ(a);
        if (z) {
            mff_cycles(0xD550, 6);
            JSR_FN(0xD550, func_F428);          /* attack_start */
        } else {
            mff_cycles(0xD54E, 1);
        }
    } else {
        mff_cycles(0xD549, 1);
    }
    mff_cycles(0xD553, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 5);
    if (z) {
        mff_cycles(0xD55A, 6);
        JSR_FN(0xD55A, func_F436);              /* attack_end */
    } else {
        mff_cycles(0xD558, 1);
    }
    mff_cycles(0xD55D, 6);
    RTS();
}

/* $D55D: an RTS that guy_attack's handlers branch to */
void func_D55D(void) {
    REGS_IN;

    mff_cycles(0xD55D, 6);
    RTS();
}

/* guy_attack_02 ($D55E, proposed): animation $02 (inside the combo) turns
 * into $59 with CHR bank 6 at timer 4. */
void func_D55E(void) {
    REGS_IN;

    mff_cycles(0xD55E, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_TIMER);
    COMPARE(a, 4);
    if (!z) {
        mff_cycles(0xD563, 1 + 6);
        RTS();
    }
    mff_cycles(0xD565, 2);
    a = 0x59;
    SET_NZ(a);
    FALL_INTO(func_D567);
}

/* guy_set_anim_chr6 ($D567, proposed): animation A, CHR bank 6. Reached by
 * JMP from guy_attack_55 (0E:D4EF) and run on into from guy_attack_02. */
void func_D567(void) {
    REGS_IN;

    mff_cycles(0xD567, 6);
    JSR_FN(0xD567, func_F294);                  /* player_set_anim */
    mff_cycles(0xD56A, 2 + 6);
    a = 0x06;
    SET_NZ(a);
    JSR_FN(0xD56C, func_F440);                  /* player_set_chr */
    mff_cycles(0xD56F, 6);
    RTS();
}

/* guy_attack_59 ($D570, proposed): animation $59, like $56: attack_start on
 * frame 1 at timer 0; on frame 2 attack_end and the next step. */
void func_D570(void) {
    REGS_IN;

    mff_cycles(0xD570, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 2);
    if (z) {
        mff_cycles(0xD577, 6);
        JSR_FN(0xD577, func_F436);              /* attack_end */
        mff_cycles(0xD57A, 3);
        JMP_TAIL(0xD61F, -1);                   /* guy_combo_next */
    }
    mff_cycles(0xD575, 1 + 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 1);
    if (!z) {
        mff_cycles(0xD582, 1);
        BRANCH_TAIL(0xD55D, -1);                    /* RTS */
    }
    mff_cycles(0xD584, 4 + 2);
    a = RAM(OBJ_ANIM_TIMER);
    SET_NZ(a);
    if (!z) {
        mff_cycles(0xD587, 1);
        BRANCH_TAIL(0xD55D, -1);                    /* RTS */
    }
    mff_cycles(0xD589, 6);
    JSR_FN(0xD589, func_F428);                  /* attack_start */
    mff_cycles(0xD58C, 6);
    RTS();
}

/* guy_attack_11 ($D58D, proposed): animation $11. attack_begin with
 * attack_id 6 and the swing sound on frame 2 at timer 0, attack_end on
 * frame 4, the next step on frame 6. */
void func_D58D(void) {
    REGS_IN;

    mff_cycles(0xD58D, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 2);
    if (z) {
        mff_cycles(0xD594, 4 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        SET_NZ(a);
        if (z) {
            mff_cycles(0xD599, 6);
            JSR_FN(0xD599, func_F43B);          /* attack_begin */
            mff_cycles(0xD59C, 2 + 3 + 6);
            a = 0x06;
            SET_NZ(a);
            RAM(ATTACK_ID) = a;
            JSR_FN(0xD5A0, func_F430);          /* attack_swing_sound */
        } else {
            mff_cycles(0xD597, 1);
        }
    } else {
        mff_cycles(0xD592, 1);
    }
    mff_cycles(0xD5A3, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 4);
    if (z) {
        mff_cycles(0xD5AA, 6);
        JSR_FN(0xD5AA, func_F436);              /* attack_end */
    } else {
        mff_cycles(0xD5A8, 1);
    }
    mff_cycles(0xD5AD, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 6);
    if (!z) {
        mff_cycles(0xD5B2, 1);
        BRANCH_TAIL(0xD55D, -1);                    /* RTS */
    }
    mff_cycles(0xD5B4, 3);
    JMP_TAIL(0xD61F, -1);                       /* guy_combo_next */
}

/*
 * guy_finisher ($D5B7, proposed): animation $14, the end of Guy's combo.
 * player_no_hit $80 throughout; attack_begin with attack_id 1 and the swing
 * sound on frame 4 at timer 0. On frame 6 attack_end, then the jump:
 * animation $5B with CHR bank 5, player_ground_y = obj_y, obj_y 32 px up,
 * and guy_finisher_fall ($D5F3) at once.
 */
void func_D5B7(void) {
    REGS_IN;

    mff_cycles(0xD5B7, 2 + 3 + 4 + 2 + 2);
    a = NO_HIT;
    RAM(PLAYER_NO_HIT) = a;
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 4);
    if (z) {
        mff_cycles(0xD5C2, 4 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        SET_NZ(a);
        if (z) {
            mff_cycles(0xD5C7, 6);
            JSR_FN(0xD5C7, func_F43B);          /* attack_begin */
            mff_cycles(0xD5CA, 2 + 3 + 6);
            a = 0x01;
            SET_NZ(a);
            RAM(ATTACK_ID) = a;
            JSR_FN(0xD5CE, func_F430);          /* attack_swing_sound */
        } else {
            mff_cycles(0xD5C5, 1);
        }
    } else {
        mff_cycles(0xD5C0, 1);
    }
    mff_cycles(0xD5D1, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 6);
    if (!z) {
        mff_cycles(0xD5D6, 2 + 6);
        RTS();
    }
    mff_cycles(0xD5D8, 6);
    JSR_FN(0xD5D8, func_F436);                  /* attack_end */
    mff_cycles(0xD5DB, 2 + 6);
    a = 0x5B;
    SET_NZ(a);
    JSR_FN(0xD5DD, func_F294);                  /* player_set_anim */
    mff_cycles(0xD5E0, 2 + 6);
    a = 0x05;
    SET_NZ(a);
    JSR_FN(0xD5E2, func_F440);                  /* player_set_chr */
    mff_cycles(0xD5E5, 4 + 3 + 4 + 2 + 2 + 4);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(OBJ_Y);
    c = 1;
    SBC(0x20);
    RAM(OBJ_Y) = a;
    FALL_INTO(func_D5F3);
}

/*
 * guy_finisher_fall ($D5F3, proposed): animation $5B. obj_fall (Y = 0);
 * once falling, when obj_y + 2 reaches player_ground_y Guy lands: obj_y
 * becomes obj_y + 1 (the ground line itself when obj_y + 2 equals it; never
 * executed), attack_live, game_action and combo_timer go to 0 and
 * player_stand ($CEAA) runs. Reached by JMP from guy_attack and run on into
 * from guy_finisher.
 */
void func_D5F3(void) {
    REGS_IN;

    mff_cycles(0xD5F3, 2 + 6);
    y = 0;
    SET_NZ(y);
    JSR_FN(0xD5F5, func_F119);                  /* obj_fall */
    mff_cycles(0xD5F8, 4 + 2);
    a = RAM(OBJ_VY);
    SET_NZ(a);
    if (!n) {                                   /* still rising */
        mff_cycles(0xD5FB, 2 + 6);
        RTS();
    }
    mff_cycles(0xD5FD, 4 + 2 + 2 + 3 + 2);
    a = RAM(OBJ_Y);
    c = 0;
    ADC(2);
    COMPARE(a, RAM(PLAYER_GROUND_Y));
    if (!c) {                                   /* above the ground line */
        mff_cycles(0xD605, 1 + 6);
        RTS();
    }
    mff_cycles(0xD607, 2);
    if (z) {
        mff_cycles(0xD607, 1 + 4);
        RAM(OBJ_Y) = a;
    } else {
        mff_cycles(0xD609, 2 + 4 + 2);
        SBC(1);
        RAM(OBJ_Y) = a;
        if (a != 0) {
            mff_cycles(0xD60E, 1);
        } else {
            mff_cycles(0xD610, 4);
            RAM(OBJ_Y) = a;
        }
    }
    mff_cycles(0xD613, 2 + 3 + 3 + 3 + 6);
    a = 0;
    SET_NZ(a);
    RAM(ATTACK_LIVE) = a;
    RAM(GAME_ACTION) = a;
    RAM(COMBO_TIMER) = a;
    JSR_AT(0xD61B, 0xCEAA, -1);                 /* player_stand */
    mff_cycles(0xD61E, 6);
    RTS();
}

/*
 * guy_combo_next ($D61F, proposed): the animation after this step goes to
 * player_stand_anim ($56 -> $17, $59 -> $11, $11 -> $14; any other returns,
 * never executed), then player_hitpos_start and player_state_hitpos.
 * Reached by JMP from guy_attack_56, _59 and _11 (0E:D4FC, D57A, D5B4).
 */
void func_D61F(void) {
    REGS_IN;

    mff_cycles(0xD61F, 4 + 2 + 2);
    a = RAM(OBJ_ANIM);
    COMPARE(a, 0x56);
    if (z) {
        mff_cycles(0xD624, 1 + 2 + 2 + 1);      /* BEQ, LDA, BNE taken */
        a = 0x17;
    } else {
        mff_cycles(0xD626, 2 + 2);
        COMPARE(a, 0x59);
        if (z) {
            mff_cycles(0xD628, 1 + 2 + 2 + 1);
            a = 0x11;
        } else {
            mff_cycles(0xD62A, 2 + 2);
            COMPARE(a, 0x11);
            if (!z) {
                mff_cycles(0xD62E, 6);
                RTS();
            }
            mff_cycles(0xD62C, 1 + 2);
            a = 0x14;
        }
    }
    SET_NZ(a);
    mff_cycles(0xD639, 3 + 6);
    RAM(PLAYER_STAND_ANIM) = a;
    JSR_AT(0xD63B, 0xD79A, -1);                 /* player_hitpos_start */
    mff_cycles(0xD63E, 3);
    JMP_TAIL(0xDD6A, -1);                       /* player_state_hitpos */
}

/*
 * haggar_attack ($D641, proposed): Haggar's handler, by obj_anim: $0F
 * (haggar_jab), $18 (haggar_jab2), $3F and $4E (haggar_wait), $12
 * (haggar_attack_12, $D6E0), $15 (haggar_finisher, $D70A); $47 goes
 * straight to the hitting position (never executed); any other returns.
 */
void func_D641(void) {
    REGS_IN;

    mff_cycles(0xD641, 4 + 2 + 2);
    a = RAM(OBJ_ANIM);
    if (a == 0x0F) {
        mff_cycles(0xD646, 1);
        FALL_INTO(haggar_jab);
    }
    mff_cycles(0xD648, 2 + 2);
    if (a == 0x3F) {
        mff_cycles(0xD64A, 1);
        FALL_INTO(haggar_wait);
    }
    mff_cycles(0xD64C, 2 + 2);
    if (a == 0x18) {
        mff_cycles(0xD64E, 1);
        FALL_INTO(haggar_jab2);
    }
    mff_cycles(0xD650, 2 + 2);
    if (a == 0x4E) {
        mff_cycles(0xD652, 1);
        FALL_INTO(haggar_wait);
    }
    mff_cycles(0xD654, 2 + 2);
    COMPARE(a, 0x12);
    if (z) {
        mff_cycles(0xD656, 1 + 3);
        JMP_TAIL(0xD6E0, -1);                   /* haggar_attack_12 */
    }
    mff_cycles(0xD658, 2 + 2);
    COMPARE(a, 0x15);
    if (z) {
        mff_cycles(0xD65A, 1 + 3);
        JMP_TAIL(0xD70A, -1);                   /* haggar_finisher */
    }
    mff_cycles(0xD65C, 2 + 2);
    COMPARE(a, 0x47);
    if (z) {
        mff_cycles(0xD65E, 1 + 3);
        JMP_TAIL(0xD79A, -1);                   /* player_hitpos_start */
    }
    mff_cycles(0xD660, 6);
    RTS();
}

/* haggar_jab (0E:D66A), animation $0F: attack_begin with attack_id $0B and
 * the swing sound on frame 2 at timer 0, attack_end on frame 3 at timer 0;
 * on frame 5 animation $3F with CHR bank 8 (haggar_set_anim_chr8). */
static void haggar_jab(void) {
    REGS_IN;

    mff_cycles(0xD66A, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 2);
    if (z) {
        mff_cycles(0xD671, 4 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        SET_NZ(a);
        if (z) {
            mff_cycles(0xD676, 6);
            JSR_FN(0xD676, func_F43B);          /* attack_begin */
            mff_cycles(0xD679, 2 + 3 + 6);
            a = 0x0B;
            SET_NZ(a);
            RAM(ATTACK_ID) = a;
            JSR_FN(0xD67D, func_F430);          /* attack_swing_sound */
        } else {
            mff_cycles(0xD674, 1);
        }
    } else {
        mff_cycles(0xD66F, 1);
    }
    mff_cycles(0xD680, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 3);
    if (z) {
        mff_cycles(0xD687, 4 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        SET_NZ(a);
        if (z) {
            mff_cycles(0xD68C, 6);
            JSR_FN(0xD68C, func_F436);          /* attack_end */
        } else {
            mff_cycles(0xD68A, 1);
        }
    } else {
        mff_cycles(0xD685, 1);
    }
    mff_cycles(0xD68F, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 5);
    if (z) {
        mff_cycles(0xD696, 2 + 3);
        a = 0x3F;
        SET_NZ(a);
        JMP_TAIL(0xD6D4, -1);                   /* haggar_set_anim_chr8 */
    }
    mff_cycles(0xD694, 1 + 6);
    RTS();
}

/* haggar_wait (0E:D69C), animations $3F and $4E: at timer 4 the next step
 * (haggar_combo_next, $D777). */
static void haggar_wait(void) {
    REGS_IN;

    mff_cycles(0xD69C, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_TIMER);
    COMPARE(a, 4);
    if (!z) {
        mff_cycles(0xD6A1, 1 + 6);
        RTS();
    }
    mff_cycles(0xD6A3, 3);
    JMP_TAIL(0xD777, -1);                       /* haggar_combo_next */
}

/* haggar_jab2 (0E:D6A6), animation $18: as haggar_jab, then animation $4E
 * on frame 5. */
static void haggar_jab2(void) {
    REGS_IN;

    mff_cycles(0xD6A6, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 2);
    if (z) {
        mff_cycles(0xD6AD, 4 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        SET_NZ(a);
        if (z) {
            mff_cycles(0xD6B2, 6);
            JSR_FN(0xD6B2, func_F43B);          /* attack_begin */
            mff_cycles(0xD6B5, 2 + 3 + 6);
            a = 0x0B;
            SET_NZ(a);
            RAM(ATTACK_ID) = a;
            JSR_FN(0xD6B9, func_F430);          /* attack_swing_sound */
        } else {
            mff_cycles(0xD6B0, 1);
        }
    } else {
        mff_cycles(0xD6AB, 1);
    }
    mff_cycles(0xD6BC, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 3);
    if (z) {
        mff_cycles(0xD6C3, 4 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        SET_NZ(a);
        if (z) {
            mff_cycles(0xD6C8, 6);
            JSR_FN(0xD6C8, func_F436);          /* attack_end */
        } else {
            mff_cycles(0xD6C6, 1);
        }
    } else {
        mff_cycles(0xD6C1, 1);
    }
    mff_cycles(0xD6CB, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 5);
    if (!z) {
        mff_cycles(0xD6D0, 1 + 6);
        RTS();
    }
    mff_cycles(0xD6D2, 2);
    a = 0x4E;
    SET_NZ(a);
    FALL_INTO(func_D6D4);
}

/* haggar_set_anim_chr8 ($D6D4, proposed): animation A with CHR bank 8, then
 * haggar_combo_next. Reached by JMP from haggar_jab (0E:D698) and run on
 * into from haggar_jab2. */
void func_D6D4(void) {
    REGS_IN;

    mff_cycles(0xD6D4, 6);
    JSR_FN(0xD6D4, func_F294);                  /* player_set_anim */
    mff_cycles(0xD6D7, 2 + 6);
    a = 0x08;
    SET_NZ(a);
    JSR_FN(0xD6D9, func_F440);                  /* player_set_chr */
    mff_cycles(0xD6DC, 3);
    JMP_TAIL(0xD777, -1);                       /* haggar_combo_next */
}

/* $D6DF: an RTS that haggar_attack_12 branches to */
void func_D6DF(void) {
    REGS_IN;

    mff_cycles(0xD6DF, 6);
    RTS();
}

/* haggar_attack_12 ($D6E0, proposed): animation $12. attack_begin with
 * attack_id $0C and the swing sound on frame 4 at timer 0, attack_end on
 * frame 5, the next step on frame 7. */
void func_D6E0(void) {
    REGS_IN;

    mff_cycles(0xD6E0, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 4);
    if (z) {
        mff_cycles(0xD6E7, 4 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        SET_NZ(a);
        if (z) {
            mff_cycles(0xD6EC, 6);
            JSR_FN(0xD6EC, func_F43B);          /* attack_begin */
            mff_cycles(0xD6EF, 2 + 3 + 6);
            a = 0x0C;
            SET_NZ(a);
            RAM(ATTACK_ID) = a;
            JSR_FN(0xD6F3, func_F430);          /* attack_swing_sound */
        } else {
            mff_cycles(0xD6EA, 1);
        }
    } else {
        mff_cycles(0xD6E5, 1);
    }
    mff_cycles(0xD6F6, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 5);
    if (z) {
        mff_cycles(0xD6FD, 6);
        JSR_FN(0xD6FD, func_F436);              /* attack_end */
    } else {
        mff_cycles(0xD6FB, 2);
    }
    mff_cycles(0xD700, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 7);
    if (!z) {
        mff_cycles(0xD705, 2);
        BRANCH_TAIL(0xD6DF, -1);                    /* RTS */
    }
    mff_cycles(0xD707, 3);
    JMP_TAIL(0xD777, -1);                       /* haggar_combo_next */
}

/*
 * haggar_finisher ($D70A, proposed): animation $15, the end of Haggar's
 * combo: attack_begin with attack_id $0D, attack_ground_y = player_ground_y
 * and the swing sound on frame 4 at timer 0; attack_end on frame 7 (frame
 * $0F would reset player_ground_y; never executed). player_no_hit $80 and
 * obj_fall (Y = 2) every frame; once falling, when obj_y + 2 reaches
 * player_ground_y obj_y is set as in guy_finisher_fall and Haggar lands:
 * animation $50, CHR bank $0B, attack_live and combo_timer 0, game_action 2
 * for 8 frames.
 */
void func_D70A(void) {
    REGS_IN;

    mff_cycles(0xD70A, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 4);
    if (z) {
        mff_cycles(0xD711, 4 + 2);
        a = RAM(OBJ_ANIM_TIMER);
        SET_NZ(a);
        if (z) {
            mff_cycles(0xD716, 6);
            JSR_FN(0xD716, func_F43B);          /* attack_begin */
            mff_cycles(0xD719, 2 + 3 + 3 + 3 + 6);
            a = 0x0D;
            RAM(ATTACK_ID) = a;
            a = RAM(PLAYER_GROUND_Y);
            SET_NZ(a);
            RAM(ATTACK_GROUND_Y) = a;
            JSR_FN(0xD721, func_F430);          /* attack_swing_sound */
        } else {
            mff_cycles(0xD714, 1);
        }
    } else {
        mff_cycles(0xD70F, 1);
    }
    mff_cycles(0xD724, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 7);
    if (z) {
        mff_cycles(0xD72B, 6);
        JSR_FN(0xD72B, func_F436);              /* attack_end */
    } else {
        mff_cycles(0xD729, 1);
    }
    mff_cycles(0xD72E, 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    COMPARE(a, 0x0F);
    if (z) {
        mff_cycles(0xD735, 4 + 3);
        a = RAM(OBJ_Y);
        RAM(PLAYER_GROUND_Y) = a;
    } else {
        mff_cycles(0xD733, 1);
    }

    mff_cycles(0xD73A, 2 + 3 + 2 + 6);
    a = NO_HIT;
    RAM(PLAYER_NO_HIT) = a;
    y = 2;
    SET_NZ(y);
    JSR_FN(0xD740, func_F119);                  /* obj_fall */
    mff_cycles(0xD743, 4 + 2);
    a = RAM(OBJ_VY);
    SET_NZ(a);
    if (!n) {                                   /* still rising */
        mff_cycles(0xD746, 1 + 6);
        RTS();
    }
    mff_cycles(0xD748, 4 + 2 + 2 + 3 + 2);
    a = RAM(OBJ_Y);
    c = 0;
    ADC(2);
    COMPARE(a, RAM(PLAYER_GROUND_Y));
    if (!c) {                                   /* above the ground line */
        mff_cycles(0xD750, 1 + 6);
        RTS();
    }
    mff_cycles(0xD752, 2);
    if (z) {
        mff_cycles(0xD752, 1 + 4);
        RAM(OBJ_Y) = a;
    } else {
        mff_cycles(0xD754, 2 + 4 + 2);
        SBC(1);
        RAM(OBJ_Y) = a;
        if (a != 0) {
            mff_cycles(0xD759, 1);
        } else {
            mff_cycles(0xD75B, 4);
            RAM(OBJ_Y) = a;
        }
    }
    mff_cycles(0xD75E, 2 + 6);
    a = 0x50;
    SET_NZ(a);
    JSR_FN(0xD760, func_F294);                  /* player_set_anim */
    mff_cycles(0xD763, 2 + 6);
    a = 0x0B;
    SET_NZ(a);
    JSR_FN(0xD765, func_F440);                  /* player_set_chr */
    mff_cycles(0xD768, 2 + 3 + 3 + 2 + 3 + 2 + 3 + 6);
    a = 0;
    RAM(ATTACK_LIVE) = a;
    RAM(COMBO_TIMER) = a;
    a = ACT_LAND;
    RAM(GAME_ACTION) = a;
    a = 8;
    SET_NZ(a);
    RAM(ACTION_TIMER) = a;
    RTS();
}

/*
 * haggar_combo_next ($D777, proposed): the animation after this step goes to
 * player_stand_anim ($3F -> $18, $4E -> $12, $12 -> $15), then
 * player_hitpos_start and player_state_hitpos. Any other animation only gets
 * player_hitpos_start (run on into; never executed). Reached by JMP from
 * haggar_wait, haggar_set_anim_chr8 and haggar_attack_12 (0E:D6A3, D6DC,
 * D707).
 */
void func_D777(void) {
    REGS_IN;

    mff_cycles(0xD777, 4 + 2 + 2);
    a = RAM(OBJ_ANIM);
    COMPARE(a, 0x3F);
    if (z) {
        mff_cycles(0xD77C, 1 + 2 + 2 + 1);      /* BEQ, LDA, BNE taken */
        a = 0x18;
    } else {
        mff_cycles(0xD77E, 2 + 2);
        COMPARE(a, 0x4E);
        if (z) {
            mff_cycles(0xD780, 1 + 2 + 2 + 1);
            a = 0x12;
        } else {
            mff_cycles(0xD782, 2 + 2);
            COMPARE(a, 0x12);
            if (!z) {
                mff_cycles(0xD786, 2 + 1);
                FALL_INTO(func_D79A);
            }
            mff_cycles(0xD784, 1 + 2);
            a = 0x15;
        }
    }
    SET_NZ(a);
    mff_cycles(0xD792, 3 + 6);
    RAM(PLAYER_STAND_ANIM) = a;
    JSR_AT(0xD794, 0xD79A, -1);                 /* player_hitpos_start */
    mff_cycles(0xD797, 3);
    JMP_TAIL(0xDD6A, -1);                       /* player_state_hitpos */
}

/*
 * player_hitpos_start ($D79A, proposed): game_action $0C (the hitting
 * position) for 8 frames (action_timer). Called by player_attack_to_hitpos,
 * guy_combo_next and haggar_combo_next (JSR at 0E:D477, D63B, D794), and
 * reached by JMP from haggar_attack (0E:D667) and a branch in
 * haggar_combo_next.
 * Out: A = 8.
 */
void func_D79A(void) {
    REGS_IN;

    mff_cycles(0xD79A, 2 + 3 + 2 + 3 + 6);
    a = ACT_HITPOS;
    RAM(GAME_ACTION) = a;
    a = 8;
    SET_NZ(a);
    RAM(ACTION_TIMER) = a;
    RTS();
}

/* ---- the hitting position (game_action $0C) ---- */

/* the hitpos handler of each character (player_state_hitpos) */
#define HITPOS_HANDLER_LO   0xDD79   /* 7F D3 FF */
#define HITPOS_HANDLER_HI   0xDD7C   /* DD DD DD: $DD7F, $DDD3, $DDFF */

/*
 * player_state_hitpos ($DD6A, labels.toml): game_action $0C, the pause
 * after a combo step (player_hitpos_start: 8 frames). JMP ($0004) to the
 * character's handler from $DD79/$DD7C[player_id]: cody_hitpos $DD7F,
 * guy_hitpos $DDD3, haggar_hitpos $DDFF. Reached by JMP right after
 * player_hitpos_start (0E:D47A, D63E, D797) and through player_state's table
 * (entry $0C).
 * Out: $04/$05 = the handler, A = its high byte.
 */
void func_DD6A(void) {
    REGS_IN;

    mff_cycles(0xDD6A, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read_ix(HITPOS_HANDLER_LO, y);
    mff_cycles(0xDD6F, 3 + 4);
    RAM(TMP4) = a;
    a = prg_read_ix(HITPOS_HANDLER_HI, y);
    SET_NZ(a);
    mff_cycles(0xDD74, 3 + 5);
    RAM(TMP5) = a;
    JMP_IND(TMP4);
}

/*
 * cody_hitpos ($DD7F, proposed). The three hitpos handlers do the same:
 *  - B: with combo_timer running (a hit landed: objects_update sets it to
 *    $1E) the combo goes on (hitpos_combo_next: player_state_idle's
 *    $CB07, the next animation from player_stand_anim); without, the punch
 *    state again (game_action 3).
 *  - otherwise, at the end of action_timer, hitpos_end; before it
 *    action_timer and combo_timer count down and the pose holds: here
 *    animation $45 or a next step of $16 keep frame 1; any other goes to
 *    hitpos_wait.
 */
void func_DD7F(void) {
    REGS_IN;

    mff_cycles(0xDD7F, 3 + 2 + 2);
    a = RAM(JOY_PRESSED) & PAD_B;
    if (a != 0) {
        mff_cycles(0xDD85, 3 + 2);
        a = RAM(COMBO_TIMER);
        SET_NZ(a);
        if (!z) {
            mff_cycles(0xDD87, 1);
            FALL_INTO(func_DDC1);               /* hitpos_combo_next */
        }
        mff_cycles(0xDD89, 2 + 3 + 6);
        a = ACT_ATTACK;
        SET_NZ(a);
        RAM(GAME_ACTION) = a;
        RTS();
    }
    mff_cycles(0xDD83, 1 + 3 + 2);
    a = RAM(ACTION_TIMER);
    SET_NZ(a);
    if (z) {
        mff_cycles(0xDDAA, 1);
        FALL_INTO(func_DDB7);                   /* hitpos_end */
    }
    mff_cycles(0xDDAC, 5 + 3 + 2);
    RAM(ACTION_TIMER)--;
    a = RAM(COMBO_TIMER);
    if (a == 0) {
        mff_cycles(0xDDB0, 1);
    } else {
        mff_cycles(0xDDB2, 5 + 3);
        RAM(COMBO_TIMER)--;
    }
    FALL_INTO(func_DD8E);
}

/* cody_hitpos_pose ($DD8E, proposed): animation $45, or player_stand_anim
 * $16: frame 1 at timer 0; otherwise hitpos_wait. Reached by JMP from
 * cody_hitpos (0E:DDB4) and run on into from it. */
void func_DD8E(void) {
    REGS_IN;

    mff_cycles(0xDD8E, 4 + 2 + 2);
    a = RAM(OBJ_ANIM);
    COMPARE(a, 0x45);
    if (!z) {
        mff_cycles(0xDD95, 3 + 2 + 2);
        a = RAM(PLAYER_STAND_ANIM);
        COMPARE(a, 0x16);
        if (!z) {
            mff_cycles(0xDD9B, 2 + 1);
            SYNC_OUT;
            hitpos_wait(0);
            return;
        }
        mff_cycles(0xDD99, 1);
    } else {
        mff_cycles(0xDD93, 1);
    }
    mff_cycles(0xDD9D, 2 + 4 + 2 + 4 + 6);
    a = 1;
    RAM(OBJ_ANIM_FRAME) = a;
    a = 0;
    SET_NZ(a);
    RAM(OBJ_ANIM_TIMER) = a;
    RTS();
}

/* hitpos_end ($DDB7, proposed): the hitting position is over: player_stand
 * ($CEAA), game_action and attack_live 0. Branched to from the hitpos
 * handlers. Out: A = 0. */
void func_DDB7(void) {
    REGS_IN;

    mff_cycles(0xDDB7, 6);
    JSR_AT(0xDDB7, 0xCEAA, -1);                 /* player_stand */
    mff_cycles(0xDDBA, 2 + 3 + 3 + 6);
    a = ACT_IDLE;
    SET_NZ(a);
    RAM(GAME_ACTION) = a;
    RAM(ATTACK_LIVE) = a;
    RTS();
}

/* hitpos_combo_next ($DDC1, proposed): JMP player_combo_continue ($CB07,
 * inside player_state_idle). Branched to from cody_hitpos and guy_hitpos. */
void func_DDC1(void) {
    REGS_IN;

    mff_cycles(0xDDC1, 3);
    JMP_TAIL(0xCB07, -1);                       /* player_combo_continue */
}

/*
 * hitpos_wait ($DDC4, proposed): out of the pose. With combo_timer running
 * the animation is held at frame 0 and attack_live cleared; with it at 0,
 * hitpos_end. As its own routine (branched to from guy_hitpos_pose and
 * haggar_hitpos_pose) the lifter dispatches the branch to hitpos_end; run
 * inline inside cody_hitpos_pose and haggar_hitpos it runs hitpos_end
 * inline. dispatch_end keeps the two forms.
 */
static void hitpos_wait(int dispatch_end) {
    REGS_IN;

    mff_cycles(0xDDC4, 3 + 2);
    a = RAM(COMBO_TIMER);
    SET_NZ(a);
    if (z) {
        mff_cycles(0xDDC6, 1);
        if (dispatch_end)
            BRANCH_TAIL(0xDDB7, -1);                /* hitpos_end */
        FALL_INTO(func_DDB7);
    }
    mff_cycles(0xDDC8, 2 + 4 + 4 + 3 + 6);
    a = 0;
    SET_NZ(a);
    RAM(OBJ_ANIM_FRAME) = a;
    RAM(OBJ_ANIM_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    RTS();
}

void func_DDC4(void) {
    hitpos_wait(1);
}

/* guy_hitpos ($DDD3, proposed): as cody_hitpos, with guy_hitpos_pose. */
void func_DDD3(void) {
    REGS_IN;

    mff_cycles(0xDDD3, 3 + 2 + 2);
    a = RAM(JOY_PRESSED) & PAD_B;
    if (a != 0) {
        mff_cycles(0xDDD9, 3 + 2);
        a = RAM(COMBO_TIMER);
        SET_NZ(a);
        if (!z) {
            mff_cycles(0xDDDB, 1);
            BRANCH_TAIL(0xDDC1, -1);                /* hitpos_combo_next */
        }
        mff_cycles(0xDDDD, 2 + 3 + 6);
        a = ACT_ATTACK;
        SET_NZ(a);
        RAM(GAME_ACTION) = a;
        RTS();
    }
    mff_cycles(0xDDD7, 1 + 3 + 2);
    a = RAM(ACTION_TIMER);
    SET_NZ(a);
    if (z) {
        mff_cycles(0xDDF2, 1);
        BRANCH_TAIL(0xDDB7, -1);                    /* hitpos_end */
    }
    mff_cycles(0xDDF4, 5 + 3 + 2);
    RAM(ACTION_TIMER)--;
    a = RAM(COMBO_TIMER);
    if (a == 0) {
        mff_cycles(0xDDF8, 1);
    } else {
        mff_cycles(0xDDFA, 5 + 3);
        RAM(COMBO_TIMER)--;
    }
    FALL_INTO(func_DDE2);
}

/* guy_hitpos_pose ($DDE2, proposed): before the step $17 Guy's pose
 * restarts animation $0E; otherwise hitpos_wait. Reached by JMP from
 * guy_hitpos (0E:DDFC) and run on into from it. */
void func_DDE2(void) {
    REGS_IN;

    mff_cycles(0xDDE2, 3 + 2 + 2);
    a = RAM(PLAYER_STAND_ANIM);
    COMPARE(a, 0x17);
    if (!z) {
        mff_cycles(0xDDE8, 2 + 1);
        BRANCH_TAIL(0xDDC4, -1);                    /* hitpos_wait */
    }
    mff_cycles(0xDDE6, 1 + 2 + 6);
    a = 0x0E;
    SET_NZ(a);
    JSR_FN(0xDDEC, func_F294);                  /* player_set_anim */
    mff_cycles(0xDDEF, 6);
    RTS();
}

/* haggar_hitpos ($DDFF, proposed): as cody_hitpos, with
 * haggar_hitpos_pose. */
void func_DDFF(void) {
    REGS_IN;

    mff_cycles(0xDDFF, 3 + 2 + 2);
    a = RAM(JOY_PRESSED) & PAD_B;
    if (a != 0) {
        mff_cycles(0xDE05, 3 + 2);
        a = RAM(COMBO_TIMER);
        SET_NZ(a);
        if (!z) {
            mff_cycles(0xDE07, 1 + 3);
            JMP_TAIL(0xCB07, -1);               /* player_combo_continue */
        }
        mff_cycles(0xDE09, 2 + 3 + 6);
        a = ACT_ATTACK;
        SET_NZ(a);
        RAM(GAME_ACTION) = a;
        RTS();
    }
    mff_cycles(0xDE03, 1 + 3 + 2);
    a = RAM(ACTION_TIMER);
    SET_NZ(a);
    if (z) {
        mff_cycles(0xDE23, 2);
        FALL_INTO(func_DDB7);                   /* hitpos_end */
    }
    mff_cycles(0xDE25, 5 + 3 + 2);
    RAM(ACTION_TIMER)--;
    a = RAM(COMBO_TIMER);
    if (a == 0) {
        mff_cycles(0xDE29, 1);
    } else {
        mff_cycles(0xDE2B, 5 + 3);
        RAM(COMBO_TIMER)--;
    }
    SYNC_OUT;
    haggar_hitpos_pose(0);
}

/*
 * haggar_hitpos_pose ($DE0E, proposed): before the step $18 Haggar's pose
 * is animation $0F with CHR bank 9; otherwise hitpos_wait - branched to
 * when this runs as its own routine (JMP from haggar_hitpos, 0E:DE2D), run
 * inline when haggar_hitpos runs on into it.
 */
static void haggar_hitpos_pose(int dispatch_wait) {
    REGS_IN;

    mff_cycles(0xDE0E, 3 + 2 + 2);
    a = RAM(PLAYER_STAND_ANIM);
    COMPARE(a, 0x18);
    if (!z) {
        mff_cycles(0xDE14, 2 + 2);
        if (dispatch_wait)
            BRANCH_TAIL(0xDDC4, -1);                /* hitpos_wait */
        SYNC_OUT;
        hitpos_wait(0);
        return;
    }
    mff_cycles(0xDE12, 1 + 2 + 6);
    a = 0x0F;
    SET_NZ(a);
    JSR_FN(0xDE18, func_F294);                  /* player_set_anim */
    mff_cycles(0xDE1B, 2 + 6);
    a = 0x09;
    SET_NZ(a);
    JSR_FN(0xDE1D, func_F440);                  /* player_set_chr */
    mff_cycles(0xDE20, 6);
    RTS();
}

void func_DE0E(void) {
    haggar_hitpos_pose(1);
}
