/*
 * player_grab.c - holding an enemy: player_state_grab (game_action 4) and
 * each character's grab.
 *
 * The grab itself starts in player_state_idle (0E:CE6C-CE92): game_action 4,
 * the enemy's slot in grab_slot, the hold time in grab_timer ($3C frames,
 * Haggar $5A) and in its obj_held. Every frame here:
 *  - A jumps out of the grab and lets go (grab_hold);
 *  - with nothing pressed grab_timer counts down, and when it runs out the
 *    enemy is let go (grab_release);
 *  - B throws: Cody over the shoulder with forward held (player_state_throw)
 *    or knees with no direction (game_action $21), Guy throws (game_action
 *    $13), Haggar throws backwards (game_action $15);
 *  - Haggar can also walk, carrying the enemy (haggar_grab_walk), and A
 *    starts his HeadSmasher (game_action $12).
 */
#include "player_defs.h"

/* the grab handler of each character (player_state_grab) */
#define GRAB_HANDLER_LO     0xD7B2   /* B8 86 9A */
#define GRAB_HANDLER_HI     0xD7B5   /* D7 D8 D8: $D7B8, $D886, $D89A */

#define OBJ_ID_THROWN       0x04     /* obj_id of a body Cody throws */
#define ANIM_CODY_THROW     0x34
#define ANIM_HAGGAR_CARRY   0x58     /* walking with the enemy */
#define ANIM_HAGGAR_HOLD    0x33     /* standing with the enemy */
#define CHR_HAGGAR_HOLD     0x08

static void cody_throw_turn(void);

/*
 * player_state_grab ($D7A3, labels.toml): game_action 4. JMP ($0000) to the
 * character's grab handler from $D7B2/$D7B5[player_id]: cody_grab $D7B8,
 * guy_grab $D886, haggar_grab $D89A. player_state dispatches here through
 * its table (entry 4).
 * Out: $00/$01 = the handler, A = its high byte.
 */
void func_D7A3(void) {
    REGS_IN;

    mff_cycles(0xD7A3, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(GRAB_HANDLER_LO + y));
    mff_cycles(0xD7A8, 3 + 4);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(GRAB_HANDLER_HI + y));
    SET_NZ(a);
    mff_cycles(0xD7AD, 3 + 5);
    RAM(TMP1) = a;
    JMP_IND(TMP0);
}

/*
 * cody_grab ($D7B8, proposed): Cody holding an enemy. Without B, grab_hold.
 * B with no direction held: game_action $21 (Data Crystal: grapple hit, the
 * knee). B with forward held: obj_throw_flag[slot] = $FF and the throw over
 * the shoulder - the player turns round (player_face_left/right, player_dir
 * 2/1) and cody_throw_turn starts it. B held backward (never executed in any
 * run) turns the enemy round instead (obj_dir[slot] ^= 3) and throws.
 */
void func_D7B8(void) {
    REGS_IN;

    mff_cycles(0xD7B8, 3 + 2 + 2);
    a = RAM(JOY_PRESSED);
    COMPARE(a, PAD_B);
    if (!z) {
        mff_cycles(0xD7BC, 2);
        FALL_INTO(func_D82E);                   /* grab_hold */
    }
    mff_cycles(0xD7BE, 3 + 3 + 2 + 2);
    y = RAM(GRAB_SLOT);
    a = RAM(JOY_HELD) & 0x0F;
    if (a == 0) {                               /* no direction: the knee */
        mff_cycles(0xD7C4, 2 + 2 + 3 + 6);
        a = ACT_GRAB_KNEE;
        SET_NZ(a);
        RAM(GAME_ACTION) = a;
        RTS();
    }
    mff_cycles(0xD7C6, 3 + 2);
    COMPARE(a, RAM(PLAYER_DIR));
    if (!z) {                                   /* not forward */
        mff_cycles(0xD7CA, 4 + 2 + 5 + 2);
        a = RAM(OBJ_DIR + y) ^ (PAD_LEFT | PAD_RIGHT);
        RAM(OBJ_DIR + y) = a;
        if (a != 0) {
            mff_cycles(0xD7D2, 1);
            FALL_INTO(cody_throw_turn);         /* player_dir = the enemy's new dir */
        }
    } else {
        mff_cycles(0xD7C8, 1);
    }

    mff_cycles(0xD7D4, 2 + 5 + 3 + 2 + 2);
    a = 0xFF;
    RAM(OBJ_THROW_FLAG + y) = a;
    a = RAM(PLAYER_DIR) & PAD_RIGHT;
    SET_NZ(a);
    if (!z) {                                   /* facing right: turn left */
        mff_cycles(0xD7DF, 6);
        JSR_FN(0xD7DF, func_F048);              /* player_face_left */
        mff_cycles(0xD7E2, 2 + 2);
        a = PAD_LEFT;
        SET_NZ(a);
        if (!z) {                               /* always */
            mff_cycles(0xD7E4, 1);
            FALL_INTO(cody_throw_turn);
        }
    } else {
        mff_cycles(0xD7DD, 1);
    }
    mff_cycles(0xD7E6, 6);                      /* facing left: turn right */
    JSR_FN(0xD7E6, func_F03F);                  /* player_face_right */
    mff_cycles(0xD7E9, 2);
    a = PAD_RIGHT;
    FALL_INTO(cody_throw_turn);
}

/* cody_throw_turn (0E:D7EB, inside cody_grab): player_dir = A, the throw
 * animation, then cody_throw_start. */
static void cody_throw_turn(void) {
    REGS_IN;

    mff_cycles(0xD7EB, 3 + 2 + 6);
    RAM(PLAYER_DIR) = a;
    a = ANIM_CODY_THROW;
    SET_NZ(a);
    JSR_FN(0xD7EF, func_F294);                  /* player_set_anim */
    FALL_INTO(func_D7F2);
}

/*
 * cody_throw_start ($D7F2, proposed): the held enemy becomes a thrown body
 * (obj_id 4, obj_state $80) drawn facing its obj_dir (flip cleared for
 * right; set for left, never executed in any run); player_no_hit $80,
 * game_action $0B (player_state_throw) for $19 frames, sound $33. Run on
 * into from cody_grab, and reached by JMP from player_state_idle's grab of a
 * combo target (0E:CC62).
 */
void func_D7F2(void) {
    REGS_IN;

    mff_cycles(0xD7F2, 2 + 3 + 3 + 2 + 5 + 2 + 5 + 4 + 2 + 2);
    a = NO_HIT;
    RAM(PLAYER_NO_HIT) = a;
    y = RAM(GRAB_SLOT);
    a = OBJ_ID_THROWN;
    RAM(OBJ_ID + y) = a;
    a = STATE_LIVE;
    RAM(OBJ_STATE + y) = a;
    a = RAM(OBJ_DIR + y) & PAD_RIGHT;
    if (a != 0) {
        mff_cycles(0xD809, 4 + 2 + 5 + 2);
        a = RAM(OBJ_FLAGS + y) & ~FLAGS_FLIP;
        RAM(OBJ_FLAGS + y) = a;
    } else {
        mff_cycles(0xD807, 1);
    }
    /* A is 0 when it went left (or, never, when the flags cleared to 0) */
    if (a == 0) {
        mff_cycles(0xD813, 4 + 2 + 5);
        a = RAM(OBJ_FLAGS + y) | FLAGS_FLIP;
        RAM(OBJ_FLAGS + y) = a;
    } else {
        mff_cycles(0xD811, 1);
    }
    mff_cycles(0xD81B, 2 + 3 + 2 + 3 + 2 + 6);
    a = 0x19;
    RAM(ACTION_TIMER) = a;
    a = ACT_CODY_THROW;
    RAM(GAME_ACTION) = a;
    a = SFX_THROW;
    SET_NZ(a);
    JSR_FN(0xD825, func_F34C);                  /* sound_queue_push_sfx */
    mff_cycles(0xD828, 6);
    RTS();
}

/*
 * grab_hold ($D82E, proposed): A jumps out of the grab: the enemy is let go
 * (obj_held[slot] = $FF, grab_timer and grab_hit_count 0) and the player
 * jumps (obj_vy $0438, the jump animation and CHR bank, game_action 1,
 * player_ground_y and attack_ground_y = obj_y, sound $1B, then
 * player_state_jump at once). Otherwise grab_timer counts down while not
 * negative, after which grab_release. Run on into from cody_grab; reached
 * by a branch from guy_grab (0E:D88A).
 */
void func_D82E(void) {
    REGS_IN;

    mff_cycles(0xD82E, 3 + 2 + 2);
    a = RAM(JOY_PRESSED);
    COMPARE(a, PAD_A);
    if (!z) {
        mff_cycles(0xD834, 3 + 2);
        a = RAM(GRAB_TIMER);
        if (!(a & 0x80)) {
            mff_cycles(0xD838, 5 + 6);
            RAM(GRAB_TIMER)--;
            SET_NZ(RAM(GRAB_TIMER));
            RTS();
        }
        mff_cycles(0xD836, 1);
        FALL_INTO(func_D83B);                   /* grab_release */
    }

    mff_cycles(0xD832, 1 + 2 + 4 + 2 + 4 + 3 + 4);
    a = 0x38;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x04;
    RAM(OBJ_VY) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(ANIM_JUMP + y));
    SET_NZ(a);
    mff_cycles(0xD85D, 6);
    JSR_FN(0xD85D, func_F294);                  /* player_set_anim */
    mff_cycles(0xD860, 4);
    a = prg_read((uint16_t)(CHR_JUMP + y));
    SET_NZ(a);
    mff_cycles(0xD863, 6);
    JSR_FN(0xD863, func_F440);                  /* player_set_chr */
    mff_cycles(0xD866, 2 + 3 + 3 + 2 + 3 + 5 + 2 + 3 + 4 + 3 + 3 + 2 + 6);
    a = 0;
    RAM(GRAB_TIMER) = a;
    RAM(GRAB_HIT_COUNT) = a;
    a = 0xFF;
    y = RAM(GRAB_SLOT);
    RAM(OBJ_HELD + y) = a;
    a = ACT_JUMP;
    RAM(GAME_ACTION) = a;
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    RAM(ATTACK_GROUND_Y) = a;
    a = SFX_JUMP;
    SET_NZ(a);
    JSR_FN(0xD880, func_F34C);                  /* sound_queue_push_sfx */
    mff_cycles(0xD883, 3);
    JMP_TAIL(0xCF88, -1);                       /* player_state_jump */
}

/*
 * grab_release ($D83B, proposed): the hold is over. game_action,
 * guy_jump_window and grab_hit_count 0, obj_held[slot] = $FF (the enemy's
 * behaviour then recovers), player_stand ($CEAA). Run on into from
 * grab_hold; reached by JMP from haggar_grab_walk (0E:D8E2).
 */
void func_D83B(void) {
    REGS_IN;

    mff_cycles(0xD83B, 2 + 3 + 3 + 3 + 2 + 3 + 5 + 6);
    a = ACT_IDLE;
    RAM(GAME_ACTION) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(GRAB_HIT_COUNT) = a;
    a = 0xFF;
    y = RAM(GRAB_SLOT);
    SET_NZ(y);
    RAM(OBJ_HELD + y) = a;
    JSR_AT(0xD84A, 0xCEAA, -1);                 /* player_stand */
    mff_cycles(0xD84D, 6);
    RTS();
}

/* guy_grab ($D886, proposed): B throws (guy_throw_start); otherwise
 * grab_hold. */
void func_D886(void) {
    REGS_IN;

    mff_cycles(0xD886, 3 + 2 + 2);
    a = RAM(JOY_PRESSED);
    COMPARE(a, PAD_B);
    if (!z) {
        mff_cycles(0xD88A, 1);
        BRANCH_TAIL(0xD82E, -1);                    /* grab_hold */
    }
    FALL_INTO(func_D88C);
}

/*
 * guy_throw_start ($D88C, proposed): player_no_hit $80, game_action $13
 * (Guy's throw, bank $00), obj_state[0] $80. Run on into from guy_grab;
 * reached by JMP from player_state_idle's grab of a combo target (0E:CC73)
 * and from guy_attack (0E:D4CC, never executed).
 */
void func_D88C(void) {
    REGS_IN;

    mff_cycles(0xD88C, 2 + 3 + 2 + 3 + 2 + 4 + 6);
    a = NO_HIT;
    RAM(PLAYER_NO_HIT) = a;
    a = ACT_GUY_THROW;
    RAM(GAME_ACTION) = a;
    a = STATE_LIVE;
    SET_NZ(a);
    RAM(OBJ_STATE) = a;
    RTS();
}

/* haggar_grab ($D89A, proposed): B throws backwards
 * (haggar_back_throw_start); otherwise player_ground_y = obj_y and
 * haggar_grab_walk. */
void func_D89A(void) {
    REGS_IN;

    mff_cycles(0xD89A, 3 + 2 + 2);
    a = RAM(JOY_PRESSED);
    COMPARE(a, PAD_B);
    if (!z) {
        mff_cycles(0xD8A0, 4 + 3 + 3);
        a = RAM(OBJ_Y);
        SET_NZ(a);
        RAM(PLAYER_GROUND_Y) = a;
        JMP_TAIL(0xD8E5, -1);                   /* haggar_grab_walk */
    }
    mff_cycles(0xD89E, 1);
    FALL_INTO(func_D8A8);
}

/*
 * haggar_back_throw_start ($D8A8, proposed): game_action $15 (Haggar's back
 * throw, bank $00), obj_state $80 for the player and the held enemy. Run on
 * into from haggar_grab; reached by JMP from player_state_idle's grab of a
 * combo target (0E:CC82).
 */
void func_D8A8(void) {
    REGS_IN;

    mff_cycles(0xD8A8, 3 + 2 + 3 + 2 + 4 + 5 + 6);
    y = RAM(GRAB_SLOT);
    a = ACT_HAGGAR_THROW;
    RAM(GAME_ACTION) = a;
    a = STATE_LIVE;
    SET_NZ(a);
    RAM(OBJ_STATE) = a;
    RAM(OBJ_STATE + y) = a;
    RTS();
}

/*
 * haggar_headsmasher_start ($D8B7, proposed): A while Haggar holds an
 * enemy. The enemy's obj_state $80, game_action $12 (the HeadSmasher, bank
 * $00), guy_jump_window 0, the jump speed (obj_vy $054E, obj_vx $0180),
 * player_ground_y and attack_ground_y = obj_y. Reached by a branch from
 * haggar_grab_walk (0E:D8E9).
 */
void func_D8B7(void) {
    REGS_IN;

    mff_cycles(0xD8B7, 3 + 2 + 5 + 2 + 3 + 2 + 3 + 2 + 4 + 2 + 4 + 2 + 4 + 2 + 4 + 4 + 3 + 3 + 6);
    y = RAM(GRAB_SLOT);
    a = STATE_LIVE;
    RAM(OBJ_STATE + y) = a;
    a = ACT_HEADSMASHER;
    RAM(GAME_ACTION) = a;
    a = 0;
    RAM(GUY_JUMP_WINDOW) = a;
    a = 0x4E;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x05;
    RAM(OBJ_VY) = a;
    a = 0x80;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VX) = a;
    a = RAM(OBJ_Y);
    SET_NZ(a);
    RAM(PLAYER_GROUND_Y) = a;
    RAM(ATTACK_GROUND_Y) = a;
    RTS();
}

/*
 * haggar_grab_walk ($D8E5, proposed): Haggar holding an enemy, every frame
 * after haggar_grab (JMP at 0E:D8A5).
 *  - A: the HeadSmasher (haggar_headsmasher_start).
 *  - grab_timer negative: grab_release; otherwise it counts down.
 *  - the carrying animation $58 (obj_set_anim, X = 0) with CHR bank 8.
 *  - left or right: obj_dir[0] takes it and Haggar steps ($F003/$F054 with
 *    probe points 0/1; nothing more if blocked). The enemy moves with him
 *    (haggar_carry_right/left) while it stays on screen: in front, until it
 *    is 249 px (right) or 17 px (left) from the screen's left edge; behind,
 *    until Haggar himself is at 241 or 24 px.
 *  - then haggar_grab_walk_y.
 */
void func_D8E5(void) {
    REGS_IN;

    mff_cycles(0xD8E5, 3 + 2 + 2);
    a = RAM(JOY_PRESSED);
    COMPARE(a, PAD_A);
    if (z) {
        mff_cycles(0xD8E9, 1);
        BRANCH_TAIL(0xD8B7, -1);                    /* haggar_headsmasher_start */
    }
    mff_cycles(0xD8EB, 3 + 2);
    a = RAM(GRAB_TIMER);
    SET_NZ(a);
    if (n) {
        mff_cycles(0xD8E2, 1 + 3);
        JMP_TAIL(0xD83B, -1);                   /* grab_release */
    }
    mff_cycles(0xD8EF, 5 + 2 + 4 + 2);
    RAM(GRAB_TIMER)--;
    a = ANIM_HAGGAR_CARRY;
    COMPARE(a, RAM(OBJ_ANIM));
    if (!z) {
        mff_cycles(0xD8F8, 6);
        JSR_FN(0xD8F8, func_F288);              /* obj_set_anim */
        mff_cycles(0xD8FB, 2 + 6);
        a = CHR_HAGGAR_HOLD;
        SET_NZ(a);
        JSR_FN(0xD8FD, func_F440);              /* player_set_chr */
    } else {
        mff_cycles(0xD8F6, 2);
    }

    mff_cycles(0xD900, 2 + 3 + 2 + 2);
    x = 0;
    a = RAM(JOY_HELD) & (PAD_LEFT | PAD_RIGHT);
    SET_NZ(a);
    if (z) {
        mff_cycles(0xD906, 1 + 3);
        JMP_TAIL(0xD9A3, -1);                   /* haggar_grab_walk_y */
    }
    mff_cycles(0xD908, 4 + 2 + 2);
    RAM(OBJ_DIR) = a;
    a &= PAD_RIGHT;
    if (a != 0) {                               /* right */
        mff_cycles(0xD90F, 2 + 6);
        y = 0;
        SET_NZ(y);
        JSR_FN(0xD911, func_F003);              /* step right */
        mff_cycles(0xD914, 2);
        if (!c) {
            mff_cycles(0xD916, 3 + 3 + 2 + 2);
            y = RAM(GRAB_SLOT);
            a = RAM(PLAYER_DIR);
            if (a == PAD_RIGHT) {               /* the enemy in front */
                mff_cycles(0xD91C, 1 + 4 + 2 + 3 + 2 + 2);
                a = RAM(OBJ_X + y);
                c = 1;
                SBC(RAM(SCROLL_X));
                COMPARE(a, 0xF9);
                if (!z) {
                    mff_cycles(0xD93A, 2);
                    if (!c)
                        FALL_INTO(func_D93C);   /* haggar_carry_right */
                }
            } else {                            /* the enemy behind */
                mff_cycles(0xD91E, 4 + 2 + 3 + 2 + 2);
                a = RAM(OBJ_X);
                c = 1;
                SBC(RAM(SCROLL_X));
                COMPARE(a, SCREEN_RIGHT_LIMIT + 1);
                if (!z) {
                    mff_cycles(0xD928, 2);
                    if (!c) {
                        mff_cycles(0xD92A, 3);
                        JMP_TAIL(0xD93C, -1);   /* haggar_carry_right */
                    }
                }
            }
        }
        mff_cycles(0xD959, 1 + 3);              /* a branch taken, JMP */
        JMP_TAIL(0xD9A3, -1);                   /* haggar_grab_walk_y */
    }

    mff_cycles(0xD90D, 1 + 2 + 6);              /* left */
    y = 1;
    SET_NZ(y);
    JSR_FN(0xD95E, func_F054);                  /* step left */
    mff_cycles(0xD961, 2);
    if (!c) {
        mff_cycles(0xD963, 3 + 3 + 2 + 2);
        y = RAM(GRAB_SLOT);
        a = RAM(PLAYER_DIR);
        if (a == PAD_RIGHT) {                   /* the enemy behind */
            mff_cycles(0xD969, 1 + 4 + 2 + 3 + 2 + 2);
            a = RAM(OBJ_X);
            c = 1;
            SBC(RAM(SCROLL_X));
            COMPARE(a, 0x18);
            if (!z) {
                mff_cycles(0xD984, 2);
                if (c)
                    FALL_INTO(func_D986);       /* haggar_carry_left */
            }
        } else {                                /* the enemy in front */
            mff_cycles(0xD96B, 4 + 2 + 3 + 2 + 2);
            a = RAM(OBJ_X + y);
            c = 1;
            SBC(RAM(SCROLL_X));
            COMPARE(a, 0x11);
            if (!z) {
                mff_cycles(0xD975, 2);
                if (c) {
                    mff_cycles(0xD977, 3);
                    JMP_TAIL(0xD986, -1);       /* haggar_carry_left */
                }
            }
        }
    }
    mff_cycles(0xD961, 1);                      /* a branch taken */
    FALL_INTO(func_D9A3);
}

/* haggar_carry_right ($D93C, proposed): the held enemy (slot Y) moves right
 * by the player's vx, then haggar_grab_walk_y. */
void func_D93C(void) {
    REGS_IN;

    mff_cycles(0xD93C, 4 + 2 + 4 + 5 + 4 + 4 + 5 + 2);
    a = RAM(OBJ_X_FRAC + y);
    c = 0;
    ADC(RAM(OBJ_VX_FRAC));
    RAM(OBJ_X_FRAC + y) = a;
    a = RAM(OBJ_X + y);
    ADC(RAM(OBJ_VX));
    RAM(OBJ_X + y) = a;
    if (c) {
        mff_cycles(0xD951, 4 + 2 + 5);
        a = RAM(OBJ_X_HI + y);
        ADC(0);
        RAM(OBJ_X_HI + y) = a;
    } else {
        mff_cycles(0xD94F, 1);
    }
    mff_cycles(0xD959, 3);
    JMP_TAIL(0xD9A3, -1);                       /* haggar_grab_walk_y */
}

/* haggar_carry_left ($D986, proposed): the held enemy (slot Y) moves left
 * by the player's vx, then haggar_grab_walk_y (run on into). */
void func_D986(void) {
    REGS_IN;

    mff_cycles(0xD986, 4 + 2 + 4 + 5 + 4 + 4 + 5 + 2);
    a = RAM(OBJ_X_FRAC + y);
    c = 1;
    SBC(RAM(OBJ_VX_FRAC));
    RAM(OBJ_X_FRAC + y) = a;
    a = RAM(OBJ_X + y);
    SBC(RAM(OBJ_VX));
    RAM(OBJ_X + y) = a;
    if (!c) {
        mff_cycles(0xD99B, 4 + 2 + 5);
        a = RAM(OBJ_X_HI + y);
        SBC(0);
        RAM(OBJ_X_HI + y) = a;
    } else {
        mff_cycles(0xD999, 1);
    }
    FALL_INTO(func_D9A3);
}

/*
 * haggar_grab_walk_y ($D9A3, proposed): up or down while Haggar holds an
 * enemy. Down: Haggar steps ($F0A2, probe point 2) and, unless blocked, the
 * enemy moves down by his vy (wrapping past $F0, never executed). Up:
 * Haggar steps ($F0F7, probe point 3) and the enemy goes to 8 px above
 * him. Then haggar_grab_hold_anim. X = 0 throughout.
 */
void func_D9A3(void) {
    REGS_IN;

    mff_cycles(0xD9A3, 2 + 3 + 2 + 2);
    x = 0;
    a = RAM(JOY_HELD) & (PAD_UP | PAD_DOWN);
    if (a == 0) {
        mff_cycles(0xD9A9, 1);
        FALL_INTO(func_D9E9);
    }
    mff_cycles(0xD9AB, 2 + 2);
    a &= PAD_DOWN;
    if (a != 0) {                               /* down */
        mff_cycles(0xD9AF, 2 + 6);
        y = 2;
        SET_NZ(y);
        JSR_FN(0xD9B1, func_F0A2);              /* step down */
        mff_cycles(0xD9B4, 2);
        if (c) {
            mff_cycles(0xD9B4, 1);
            FALL_INTO(func_D9E9);
        }
        mff_cycles(0xD9B6, 3 + 4 + 2 + 4 + 5 + 4 + 4 + 5 + 2 + 2);
        y = RAM(GRAB_SLOT);
        a = RAM(OBJ_Y_FRAC + y);
        c = 0;
        ADC(RAM(OBJ_VY_FRAC));
        RAM(OBJ_Y_FRAC + y) = a;
        a = RAM(OBJ_Y + y);
        ADC(RAM(OBJ_VY));
        RAM(OBJ_Y + y) = a;
        COMPARE(a, 0xF0);
        if (c) {
            mff_cycles(0xD9CF, 2 + 5);
            ADC(0x0F);
            RAM(OBJ_Y + y) = a;
        } else {
            mff_cycles(0xD9CD, 1);
        }
        mff_cycles(0xD9D4, 3);
        JMP_TAIL(0xD9E9, -1);                   /* haggar_grab_hold_anim */
    }
    mff_cycles(0xD9AD, 1 + 2 + 6);              /* up */
    y = 3;
    SET_NZ(y);
    JSR_FN(0xD9D9, func_F0F7);                  /* step up */
    mff_cycles(0xD9DC, 2);
    if (!c) {
        mff_cycles(0xD9DE, 3 + 4 + 2 + 2 + 5);
        y = RAM(GRAB_SLOT);
        a = RAM(OBJ_Y);
        c = 1;
        SBC(8);
        RAM(OBJ_Y + y) = a;
    } else {
        mff_cycles(0xD9DC, 1);
    }
    FALL_INTO(func_D9E9);
}

/* haggar_grab_hold_anim ($D9E9, proposed): with no direction held, the
 * standing hold: animation $33, CHR bank 8. */
void func_D9E9(void) {
    REGS_IN;

    mff_cycles(0xD9E9, 3 + 2 + 2);
    a = RAM(JOY_HELD) & 0x0F;
    SET_NZ(a);
    if (z) {
        mff_cycles(0xD9EF, 2 + 6);
        a = ANIM_HAGGAR_HOLD;
        SET_NZ(a);
        JSR_FN(0xD9F1, func_F294);              /* player_set_anim */
        mff_cycles(0xD9F4, 2 + 6);
        a = CHR_HAGGAR_HOLD;
        SET_NZ(a);
        JSR_FN(0xD9F6, func_F440);              /* player_set_chr */
    } else {
        mff_cycles(0xD9ED, 1);
    }
    mff_cycles(0xD9F9, 6);
    RTS();
}
