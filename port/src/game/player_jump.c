/*
 * player_jump.c - the player's jump (0E:CF88-D2B6): player_state_jump,
 * falling into a pit, landing, the attacks and the steering in the air
 *
 * player_state_jump ($CF88) runs each frame of game_action 1. The ROM
 * reaches the routines after it by JMP, which the scaffold made tail calls
 * (2 more cycles), or by branching and running on into them, which costs
 * nothing more; the C calls the routine in both cases and charges the 2
 * cycles only for the tail calls.
 */
#include "player_defs.h"

void func_D00E(void);
void func_D18E(void);

/* ---- per-character tables (3 bytes: Cody, Guy, Haggar) ---- */
#define IS_JUMP_KICK_MOVING2 0xE27C  /* 1F 20 21: the same ids as $E276 */
#define IS_JUMP_BACK        0xE285   /* 3A 3B 3C */
#define ANIM_JUMP_DOWN_ATK  0xE1FB   /* 19 1A 1B: B with Down in a standing jump */
#define ANIM_JUMP_KICK_MOVING 0xE201 /* 1F 20 21 */
#define CHR_JUMP_ATTACK     0xE246   /* 02 06 13 */
#define CHR_JUMP_KICK_MOVING 0xE249  /* 01 05 13 */

/* attack_id of the jump attacks */
#define ATTACK_JUMP_KICK    3
#define ATTACK_JUMP_DOWN    4        /* Haggar's down attack is 3 */

/* the jump attack: animation `anim`, CHR bank `chr`, attack_id 3 (or, with
 * `down`, 3 for Haggar and 4 for the others) and attack_begin ($F43B). The
 * five copies in the ROM (0E:D098, D0BC, D11D, D13F, D16D) differ only in
 * their addresses: `pc` is the copy's first. Each copy then sets
 * attack_ground_y = player_ground_y, which the caller does, with the
 * copy's exit. Registers in and out through g_cpu; 0 if a callee left
 * through the scheduler (the caller must return at once). */
static int jump_attack_start(uint16_t pc, uint16_t anim, uint16_t chr, int down) {
    REGS_IN;

    mff_cycles(pc, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(anim + y));
    SET_NZ(a);
    mff_cycles((uint16_t)(pc + 5), 6);
    SYNC_OUT;
    if (!mff_call_fn(RP(pc + 5), func_F294, 0xE000)) return 0;  /* player_set_anim */
    SYNC_IN;
    mff_cycles((uint16_t)(pc + 8), 4);
    a = prg_read((uint16_t)(chr + y));
    SET_NZ(a);
    mff_cycles((uint16_t)(pc + 11), 6);
    SYNC_OUT;
    if (!mff_call_fn(RP(pc + 11), func_F440, 0xE000)) return 0; /* player_set_chr */
    SYNC_IN;
    if (down) {
        mff_cycles((uint16_t)(pc + 14), 2 + 2);
        COMPARE(y, PLAYER_HAGGAR);
        if (z) {
            mff_cycles((uint16_t)(pc + 18), 2 + 2 + 1);   /* LDA #3, BNE (always) */
            a = ATTACK_JUMP_KICK;
        } else {
            mff_cycles((uint16_t)(pc + 16), 1 + 2);
            a = ATTACK_JUMP_DOWN;
        }
        SET_NZ(a);
        mff_cycles((uint16_t)(pc + 24), 3 + 6);
    } else {
        mff_cycles((uint16_t)(pc + 14), 2 + 3 + 6);
        a = ATTACK_JUMP_KICK;
        SET_NZ(a);
    }
    RAM(ATTACK_ID) = a;
    SYNC_OUT;
    return mff_call_fn(RP(pc + (down ? 26 : 18)), func_F43B, 0xE000);   /* attack_begin */
}

/*
 * player_jump_air ($D04A-$D18D, part of player_state_jump): the frame's B
 * while the jump goes on, then the steering.
 *  - Guy in animation $3B (his jump) pressing Up opens guy_jump_window (5).
 *  - B pressed while still rising, or falling at -2 px a frame or slower:
 *    - a standing jump (jump_moving 0): Down, Down+Right or Down+Left
 *      ($04-$06) is a down attack ($E1FB with CHR bank $E246; not executed
 *      in the CDL runs), anything else the jump kick ($E1FE, $E246);
 *    - a moving jump, unless it already kicks ($E279, $E27C): if it goes
 *      the way the player faces, the same choice ($E201/$E249 for the kick)
 *      and the frame ends there (RTS, no steering); backwards, Down picks
 *      the down attack (with CHR bank $E243; never executed), anything else
 *      goes on to the steering.
 *  - Then player_jump_steer ($D18E): by JMP (tail call), or run on into
 *    after the backward down attack and the backward moving jump.
 */
static void jump_air(void) {
    REGS_IN;

    mff_cycles(0xD04A, 3 + 2 + 2);
    a = RAM(PLAYER_ID);
    if (a == PLAYER_GUY) {
        mff_cycles(0xD050, 4 + 2 + 2);
        a = RAM(OBJ_ANIM);
        if (a == 0x3B) {
            mff_cycles(0xD057, 3 + 2 + 2);
            a = RAM(JOY_PRESSED);
            if (a == PAD_UP) {
                mff_cycles(0xD05D, 2 + 3);
                a = 5;
                RAM(GUY_JUMP_WINDOW) = a;
            } else {
                mff_cycles(0xD05B, 1);
            }
        } else {
            mff_cycles(0xD055, 1);
        }
    } else {
        mff_cycles(0xD04E, 1);
    }
    mff_cycles(0xD061, 3 + 2 + 2);
    a = RAM(JOY_PRESSED);
    COMPARE(a, PAD_B);
    if (!z) {
        mff_cycles(0xD067, 3);
        JMP_TAIL(0xD18E, -1);                   /* player_jump_steer */
    }
    mff_cycles(0xD065, 1 + 4 + 2);
    a = RAM(OBJ_VY);
    if (a & 0x80) {
        mff_cycles(0xD06F, 2 + 2);
        COMPARE(a, 0xFE);
        if (!c) {                               /* falling fast: no attack */
            mff_cycles(0xD073, 3);
            JMP_TAIL(0xD18E, -1);               /* player_jump_steer */
        }
        mff_cycles(0xD071, 1);
    } else {
        mff_cycles(0xD06D, 1);
    }
    mff_cycles(0xD076, 3 + 2);
    a = RAM(JUMP_MOVING);
    if (a == 0) {
        /* a standing jump */
        mff_cycles(0xD07A, 3 + 2 + 3 + 2 + 2);
        a = RAM(JOY_HELD) & 0x0F;
        RAM(TMP0) = a;
        COMPARE(a, PAD_RIGHT);
        if (z) {
            mff_cycles(0xD082, 1);
            goto kick;
        }
        mff_cycles(0xD084, 2 + 2);
        COMPARE(a, PAD_LEFT);
        if (z) {
            mff_cycles(0xD086, 1);
            goto kick;
        }
        mff_cycles(0xD088, 2 + 2);
        COMPARE(a, PAD_DOWN);
        if (z) {
            mff_cycles(0xD08A, 1);
            goto down;
        }
        mff_cycles(0xD08C, 2 + 2);
        COMPARE(a, PAD_DOWN | PAD_RIGHT);
        if (z) {
            mff_cycles(0xD08E, 1);
            goto down;
        }
        mff_cycles(0xD090, 3 + 2 + 2);
        a = RAM(TMP0);
        COMPARE(a, PAD_DOWN | PAD_LEFT);
        if (z) {
            mff_cycles(0xD094, 1);
            goto down;
        }
        mff_cycles(0xD096, 2 + 1);              /* BNE (always) */
kick:
        SYNC_OUT;
        if (!jump_attack_start(0xD0BC, ANIM_JUMP_KICK, CHR_JUMP_ATTACK, 0)) return;
        SYNC_IN;
        mff_cycles(0xD0D1, 3 + 3 + 3);
        a = RAM(PLAYER_GROUND_Y);
        SET_NZ(a);
        RAM(ATTACK_GROUND_Y) = a;
        JMP_TAIL(0xD18E, -1);                   /* player_jump_steer */
down:
        SYNC_OUT;
        if (!jump_attack_start(0xD098, ANIM_JUMP_DOWN_ATK, CHR_JUMP_ATTACK, 1)) return;
        SYNC_IN;
        mff_cycles(0xD0B5, 3 + 3 + 3);
        a = RAM(PLAYER_GROUND_Y);
        SET_NZ(a);
        RAM(ATTACK_GROUND_Y) = a;
        JMP_TAIL(0xD18E, -1);                   /* player_jump_steer */
    }

    /* a moving jump */
    mff_cycles(0xD078, 1 + 3 + 2 + 2);
    a = RAM(JOY_PRESSED);
    COMPARE(a, PAD_B);
    if (!z) {                                   /* never: B was tested above */
        mff_cycles(0xD0DE, 3);
        JMP_TAIL(0xD18E, -1);
    }
    mff_cycles(0xD0DC, 1 + 3 + 4 + 4);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    COMPARE(a, prg_read((uint16_t)(IS_JUMP_KICK + y)));
    mff_cycles(0xD0E9, 2);
    if (z) {
        mff_cycles(0xD0EB, 3);
        JMP_TAIL(0xD18E, -1);                   /* player_jump_steer */
    }
    mff_cycles(0xD0E9, 1 + 4 + 4);
    a = RAM(OBJ_ANIM);
    COMPARE(a, prg_read((uint16_t)(IS_JUMP_KICK_MOVING2 + y)));
    mff_cycles(0xD0F4, 2);
    if (z) {                                    /* (never executed) */
        mff_cycles(0xD0F6, 3);
        JMP_TAIL(0xD18E, -1);                   /* player_jump_steer */
    }
    mff_cycles(0xD0F4, 1 + 3 + 3 + 2);
    a = RAM(PLAYER_DIR);
    if (a == RAM(JUMP_DIR)) {
        /* forwards */
        mff_cycles(0xD0FF, 3 + 2 + 3 + 2 + 2);
        a = RAM(JOY_HELD) & 0x0F;
        RAM(TMP0) = a;
        COMPARE(a, PAD_RIGHT);
        if (z) {
            mff_cycles(0xD107, 1);
            goto kick_fwd;
        }
        mff_cycles(0xD109, 2 + 2);
        COMPARE(a, PAD_LEFT);
        if (z) {
            mff_cycles(0xD10B, 1);
            goto kick_fwd;
        }
        mff_cycles(0xD10D, 2 + 2);
        COMPARE(a, PAD_DOWN);
        if (z) {
            mff_cycles(0xD10F, 1);
            goto down_fwd;
        }
        mff_cycles(0xD111, 2 + 2);
        COMPARE(a, PAD_DOWN | PAD_RIGHT);
        if (z) {
            mff_cycles(0xD113, 1);
            goto down_fwd;
        }
        mff_cycles(0xD115, 3 + 2 + 2);
        a = RAM(TMP0);
        COMPARE(a, PAD_DOWN | PAD_LEFT);
        if (z) {
            mff_cycles(0xD119, 1);
            goto down_fwd;
        }
        mff_cycles(0xD11B, 2 + 1);              /* BNE (always) */
kick_fwd:
        SYNC_OUT;
        if (!jump_attack_start(0xD13F, ANIM_JUMP_KICK_MOVING, CHR_JUMP_KICK_MOVING, 0)) return;
        SYNC_IN;
        mff_cycles(0xD154, 3 + 3 + 6);
        a = RAM(PLAYER_GROUND_Y);
        SET_NZ(a);
        RAM(ATTACK_GROUND_Y) = a;
        RTS();
down_fwd:                                       /* never executed */
        SYNC_OUT;
        if (!jump_attack_start(0xD11D, ANIM_JUMP_DOWN_ATK, CHR_JUMP_ATTACK, 1)) return;
        SYNC_IN;
        mff_cycles(0xD13A, 3 + 3 + 6);
        a = RAM(PLAYER_GROUND_Y);
        SET_NZ(a);
        RAM(ATTACK_GROUND_Y) = a;
        RTS();
    }

    /* backwards */
    mff_cycles(0xD0FD, 2 + 3 + 2 + 3 + 2 + 2); /* BNE $D159 crosses a page */
    a = RAM(JOY_HELD) & 0x0F;
    RAM(TMP0) = a;
    COMPARE(a, PAD_DOWN);
    if (z) {
        mff_cycles(0xD161, 1);
        goto down_back;
    }
    mff_cycles(0xD163, 2 + 2);
    COMPARE(a, PAD_DOWN | PAD_RIGHT);
    if (z) {
        mff_cycles(0xD165, 1);
        goto down_back;
    }
    mff_cycles(0xD167, 2 + 2);
    COMPARE(a, PAD_DOWN | PAD_LEFT);
    if (z) {
        mff_cycles(0xD169, 1);
        goto down_back;
    }
    mff_cycles(0xD16B, 2 + 1);                  /* BNE $D18E (always) */
    FALL_INTO(func_D18E);                       /* player_jump_steer */
down_back:                                      /* never executed */
    SYNC_OUT;
    if (!jump_attack_start(0xD16D, ANIM_JUMP_DOWN_ATK, CHR_JUMP_KICK, 1)) return;
    SYNC_IN;
    mff_cycles(0xD18A, 3 + 3);
    a = RAM(PLAYER_GROUND_Y);
    SET_NZ(a);
    RAM(ATTACK_GROUND_Y) = a;
    FALL_INTO(func_D18E);                       /* player_jump_steer */
}

/*
 * player_state_jump ($CF88, labels.toml): game_action 1 (player_state's
 * table, entry 1; JMP from player_state_idle's jump start, 0E:CA9F, and
 * from 0E:D883).
 *  - Slot 0 falls by obj_vy with gravity (obj_fall, $F119). On the floor
 *    (C set): player_jump_land ($D00E, tail call).
 *  - Falling (obj_vy negative), not in a jump kick ($E279, $E27C): a jump
 *    over a pit tile ($10 = $40) in the moving, the backward or (from its
 *    3rd frame) the standing jump animation drops into the pit
 *    (player_pit_fall, $CFD1); otherwise the standing jump turns into the
 *    fall animation ($E219).
 *  - Over a pit ($10 = $40): player_pit_fall, and the frame ends.
 *  - obj_y + 2 at or past player_ground_y: obj_y is put back on the ground
 *    (equal) or 1 px above it, and the jump lands (player_jump_land, run on
 *    into). Above it: the B attacks and the steering (jump_air).
 */
void func_CF88(void) {
    REGS_IN;

    mff_cycles(0xCF88, 2 + 2 + 6);
    y = 0;
    x = 0;
    SET_NZ(x);
    JSR_FN(0xCF8C, func_F119);                  /* obj_fall */
    mff_cycles(0xCF8F, 2);
    if (c) {
        mff_cycles(0xCF91, 3);
        JMP_TAIL(0xD00E, -1);                   /* player_jump_land */
    }
    mff_cycles(0xCF8F, 1 + 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(IS_JUMP_KICK + y));
    mff_cycles(0xCF99, 4 + 2);
    if (a == RAM(OBJ_ANIM)) {
        mff_cycles(0xCF9C, 1);
        goto pit_check;
    }
    mff_cycles(0xCF9E, 4);
    a = prg_read((uint16_t)(IS_JUMP_KICK_MOVING2 + y));
    mff_cycles(0xCFA1, 4 + 2);
    if (a == RAM(OBJ_ANIM)) {
        mff_cycles(0xCFA4, 1);
        goto pit_check;
    }
    mff_cycles(0xCFA6, 4 + 2);
    a = RAM(OBJ_VY);
    if (!(a & 0x80)) {                          /* still rising */
        mff_cycles(0xCFA9, 1);
        goto pit_check;
    }
    mff_cycles(0xCFAB, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(IS_JUMP_MOVING + y));
    mff_cycles(0xCFB0, 4 + 2);
    if (a == RAM(OBJ_ANIM)) {
        mff_cycles(0xCFB3, 1);
        goto pit_drop;
    }
    mff_cycles(0xCFB5, 4);
    a = prg_read((uint16_t)(IS_JUMP_BACK + y));
    mff_cycles(0xCFB8, 4 + 2);
    if (a == RAM(OBJ_ANIM)) {
        mff_cycles(0xCFBB, 1);
        goto pit_drop;
    }
    mff_cycles(0xCFBD, 4);
    a = prg_read((uint16_t)(IS_JUMP + y));
    mff_cycles(0xCFC0, 4 + 2);
    COMPARE(a, RAM(OBJ_ANIM));
    if (z) {
        mff_cycles(0xCFC3, 1);
        goto fall_anim;
    }
    mff_cycles(0xCFC5, 2 + 1);                  /* BNE (always) */
    goto pit_check;

pit_drop:
    mff_cycles(0xCFC7, 3 + 2 + 2);
    a = RAM(TMP16);
    COMPARE(a, PROBE_PIT);
    if (z) {
        mff_cycles(0xCFCD, 6);
        JSR_AT(0xCFCD, 0xCFD1, -1);             /* player_pit_fall */
        mff_cycles(0xCFD0, 6);
        RTS();
    }
    mff_cycles(0xCFCB, 1 + 4 + 2 + 2);
    a = RAM(OBJ_ANIM_FRAME);
    c = a >= 3;
    if (a != 3) {
        mff_cycles(0xCFE4, 1);
        goto pit_check;
    }
fall_anim:
    mff_cycles(0xCFE6, 3 + 4);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(ANIM_FALL + y));
    SET_NZ(a);
    mff_cycles(0xCFEB, 6);
    JSR_FN(0xCFEB, func_F294);                  /* player_set_anim */
pit_check:
    mff_cycles(0xCFEE, 3 + 2 + 2);
    a = RAM(TMP16);
    COMPARE(a, PROBE_PIT);
    if (z) {
        mff_cycles(0xCFF4, 6);
        JSR_AT(0xCFF4, 0xCFD1, -1);             /* player_pit_fall */
        mff_cycles(0xCFF7, 6);
        RTS();
    }

    /* obj_y + 2 against the ground line */
    mff_cycles(0xCFF2, 1 + 4 + 2 + 2 + 3 + 2);
    a = RAM(OBJ_Y);
    c = 0;
    ADC(2);
    COMPARE(a, RAM(PLAYER_GROUND_Y));
    if (!c) {
        mff_cycles(0xD000, 1);
        SYNC_OUT;
        jump_air();
        return;
    }
    mff_cycles(0xD002, 2);
    if (!z) {                                   /* past it: 1 px above */
        mff_cycles(0xD004, 2 + 4 + 2);
        SBC(3);
        RAM(OBJ_Y) = a;
        if (a != 0) {
            mff_cycles(0xD009, 1);
            FALL_INTO(func_D00E);               /* player_jump_land */
        }
    } else {
        mff_cycles(0xD002, 1);
    }
    mff_cycles(0xD00B, 4);
    RAM(OBJ_Y) = a;
    FALL_INTO(func_D00E);                       /* player_jump_land */
}

/*
 * player_pit_fall ($CFD1, proposed): the player drops into a pit:
 * action_before_fall = game_action, game_action $24 (bank $00's pit fall,
 * 00:8D8D), obj_state[0] = $80. Called by player_state_jump and
 * player_jump_land (0E:CFCD, CFF4, D016).
 * Out: A = $80.
 */
void func_CFD1(void) {
    REGS_IN;

    mff_cycles(0xCFD1, 3 + 3 + 2 + 3 + 2 + 4 + 6);
    a = RAM(GAME_ACTION);
    RAM(ACTION_BEFORE_FALL) = a;
    a = ACT_PIT_FALL;
    RAM(GAME_ACTION) = a;
    a = STATE_LIVE;
    SET_NZ(a);
    RAM(OBJ_STATE) = a;
    RTS();
}

/*
 * player_jump_land ($D00E, proposed name): the end of a jump.
 * player_state_jump comes here when obj_fall ($F119) reports the floor (JMP
 * at 0E:CF91) or when obj_y reaches player_ground_y (BNE at 0E:D009, and on
 * from 0E:D00B).
 *
 * In: $10 = attribute of the tile the floor probe found.
 *  - $40 and up, a pit: player_pit_fall ($CFD1) takes over.
 *  - otherwise game_action = ACT_LAND with action_timer 3, the attack ends
 *    (attack_end, $F436) and the player stands (player_stand, $CEAA). Guy,
 *    landing while guy_jump_window runs (Up during his jump), takes animation
 *    $68 and CHR bank 6 instead. Both then play the landing sound
 *    (player_land_finish, $D03F).
 * Out: A = 0 after a landing; X and Y as the callees leave them.
 */
void func_D00E(void) {
    REGS_IN;

    mff_cycles(0xD00E, 3 + 2 + 2);
    a = RAM(TMP16);
    COMPARE(a, PROBE_PIT);
    if (c) {                                    /* a pit */
        mff_cycles(0xD012, 1 + 6);
        JSR_AT(0xD016, 0xCFD1, -1);             /* player_pit_fall */
        mff_cycles(0xD019, 6);
        RTS();
    }
    /* BCC (always taken here), then the two stores and the JSR */
    mff_cycles(0xD014, 2 + 1 + 2 + 3 + 2 + 3 + 6);
    a = ACT_LAND;
    RAM(GAME_ACTION) = a;
    a = 3;
    SET_NZ(a);
    RAM(ACTION_TIMER) = a;
    JSR_FN(0xD022, func_F436);                  /* attack_end */

    mff_cycles(0xD025, 3 + 2 + 2);
    a = RAM(PLAYER_ID);
    COMPARE(a, PLAYER_GUY);
    if (z) {
        mff_cycles(0xD02B, 3 + 2);
        a = RAM(GUY_JUMP_WINDOW);
        SET_NZ(a);
        if (!z) {                               /* Guy inside his jump window */
            mff_cycles(0xD02F, 2 + 6);
            a = 0x68;
            SET_NZ(a);
            JSR_FN(0xD031, func_F294);          /* player_set_anim */
            mff_cycles(0xD034, 2 + 6);
            a = 0x06;
            SET_NZ(a);
            JSR_FN(0xD036, func_F440);          /* player_set_chr */
            mff_cycles(0xD039, 3);
            JMP_TAIL(0xD03F, -1);               /* player_land_finish */
        }
    }
    mff_cycles(0xD029, 1 + 6);                  /* BNE or BEQ taken, JSR */
    JSR_AT(0xD03C, 0xCEAA, -1);                 /* player_stand */
    FALL_INTO(func_D03F);
}

/*
 * player_land_finish ($D03F, proposed): the landing sound and
 * obj_timer2[0] = 0. Reached by JMP from player_jump_land (0E:D039) and run
 * on into after its player_stand (0E:D03C).
 * Out: A = 0 (N clear, Z set).
 */
void func_D03F(void) {
    REGS_IN;

    mff_cycles(0xD03F, 2 + 6);
    a = SFX_LAND;
    SET_NZ(a);
    JSR_FN(0xD041, func_F34C);                  /* sound_queue_push_sfx */
    mff_cycles(0xD044, 2 + 4 + 6);
    a = 0;
    SET_NZ(a);
    RAM(OBJ_TIMER2) = a;
    RTS();
}

/*
 * player_jump_steer ($D18E, proposed): left or right during a jump. Reached
 * by JMP from player_state_jump's air control (0E:D067, D073, D0B9, D0D5,
 * D0EB).
 *
 * Once the jump moves (jump_moving set) player_jump_drift ($D242) carries
 * it on. Otherwise, while the jump still rises (obj_vy[0] not negative:
 * obj_y goes up by it), not in a jump kick, with left or right held:
 *  - the animation becomes the moving jump (with its CHR bank) unless B was
 *    just pressed or it already is;
 *  - jump_moving is counted up and jump_dir takes the direction (pad bits,
 *    through $00);
 *  - the sprite faces that way (obj_face_right/left on slot X) and moves a
 *    step (obj_move_right/left by vx);
 *  - if that is not the way the player faces (player_dir), the jump goes on
 *    backwards: the back-jump animation, vx from $E1CD/$E1D0 and obj_flags
 *    turned back to the facing side.
 * Going right with the camera scrolling by itself (camera_autoscroll
 * negative) and not held on a screen (camera_lock positive), a player at 241
 * px or more from the screen's left edge is put back at 240 and does not
 * move (0E:D1D9; never executed in any run).
 */
void func_D18E(void) {
    REGS_IN;

    mff_cycles(0xD18E, 3 + 2);
    a = RAM(JUMP_MOVING);
    SET_NZ(a);
    if (!z) {                                   /* already moving */
        mff_cycles(0xD190, 1 + 3);
        JMP_TAIL(0xD242, -1);                   /* player_jump_drift */
    }
    mff_cycles(0xD192, 4 + 2);
    a = RAM(OBJ_VY);
    SET_NZ(a);
    if (n) {                                    /* falling already */
        mff_cycles(0xD195, 1 + 6);
        RTS();
    }
    mff_cycles(0xD197, 3 + 4 + 4);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    COMPARE(a, prg_read((uint16_t)(IS_JUMP_KICK + y)));
    mff_cycles(0xD19F, 2);
    if (z) {                                    /* kicking */
        mff_cycles(0xD19F, 1 + 6);
        RTS();
    }
    mff_cycles(0xD1A1, 3 + 2 + 2);
    a = RAM(JOY_HELD) & (PAD_LEFT | PAD_RIGHT);
    SET_NZ(a);
    if (z) {                                    /* neither held */
        mff_cycles(0xD1A5, 1 + 6);
        RTS();
    }

    mff_cycles(0xD1A7, 3 + 3 + 2 + 2);
    RAM(TMP0) = a;                              /* the direction */
    a = RAM(JOY_PRESSED);
    COMPARE(a, PAD_B);
    if (!z) {                                   /* B not just pressed */
        mff_cycles(0xD1AF, 3 + 4 + 4);
        y = RAM(PLAYER_ID);
        a = RAM(OBJ_ANIM);
        COMPARE(a, prg_read((uint16_t)(IS_JUMP_MOVING + y)));
        mff_cycles(0xD1B7, 2);
        if (!z) {                               /* not the moving jump yet */
            mff_cycles(0xD1B9, 3 + 4);
            y = RAM(PLAYER_ID);
            a = prg_read((uint16_t)(ANIM_JUMP_MOVING + y));
            SET_NZ(a);
            mff_cycles(0xD1BE, 6);
            JSR_FN(0xD1BE, func_F294);          /* player_set_anim */
            mff_cycles(0xD1C1, 4);
            a = prg_read((uint16_t)(CHR_JUMP + y));
            SET_NZ(a);
            mff_cycles(0xD1C4, 6);
            JSR_FN(0xD1C4, func_F440);          /* player_set_chr */
        } else {
            mff_cycles(0xD1B7, 1);
        }
    } else {
        mff_cycles(0xD1AD, 1);
    }

    mff_cycles(0xD1C7, 5 + 3 + 3 + 2 + 2);
    RAM(JUMP_MOVING)++;
    a = RAM(TMP0);
    RAM(JUMP_DIR) = a;
    a &= PAD_RIGHT;
    SET_NZ(a);
    if (z) {                                    /* left */
        mff_cycles(0xD1CF, 2 + 6);
        JSR_FN(0xD21C, func_F099);              /* obj_face_left */
        mff_cycles(0xD21F, 6);
        JSR_FN(0xD21F, func_F1A1);              /* obj_move_left */
        mff_cycles(0xD222, 3 + 3 + 2);
        a = RAM(PLAYER_DIR);
        COMPARE(a, RAM(JUMP_DIR));
        if (!z) {                               /* facing right: backwards */
            mff_cycles(0xD228, 3 + 4);
            y = RAM(PLAYER_ID);
            a = prg_read((uint16_t)(ANIM_JUMP_BACK + y));
            SET_NZ(a);
            mff_cycles(0xD22D, 6);
            JSR_FN(0xD22D, func_F294);          /* player_set_anim */
            mff_cycles(0xD230, 4);
            a = prg_read((uint16_t)(JUMP_BACK_VX_FRAC + y));
            mff_cycles(0xD233, 4 + 4);
            RAM(OBJ_VX_FRAC) = a;
            a = prg_read((uint16_t)(JUMP_BACK_VX + y));
            mff_cycles(0xD239, 4 + 2 + 4);
            RAM(OBJ_VX) = a;
            a = FLAGS_FACING_RIGHT;
            SET_NZ(a);
            RAM(OBJ_FLAGS) = a;
        } else {
            mff_cycles(0xD226, 1);
        }
        mff_cycles(0xD241, 6);
        RTS();
    }

    /* right */
    mff_cycles(0xD1D1, 3 + 2);
    a = RAM(CAMERA_AUTOSCROLL);
    SET_NZ(a);
    if (n) {
        mff_cycles(0xD1D5, 3 + 2);
        a = RAM(CAMERA_LOCK);
        SET_NZ(a);
        if (!n) {
            mff_cycles(0xD1D9, 4 + 2 + 3 + 2 + 2);
            a = RAM(OBJ_X);
            c = 1;
            SBC(RAM(SCROLL_X));
            COMPARE(a, SCREEN_RIGHT_LIMIT + 1);
            if (c) {                            /* at the edge: stay there */
                mff_cycles(0xD1E3, 3 + 2 + 2 + 4 + 3 + 2 + 4 + 6);
                a = RAM(SCROLL_X);
                c = 0;
                ADC(SCREEN_RIGHT_LIMIT);
                RAM(OBJ_X) = a;
                a = RAM(AREA_SCREEN);
                ADC(0);
                RAM(OBJ_X_HI) = a;
                RTS();
            }
        }
    }
    mff_cycles(0xD1F6, 1 + 6);                  /* BPL, BMI or BCC taken, JSR */
    JSR_FN(0xD1F6, func_F090);                  /* obj_face_right */
    mff_cycles(0xD1F9, 6);
    JSR_FN(0xD1F9, func_F183);                  /* obj_move_right */
    mff_cycles(0xD1FC, 3 + 3 + 2);
    a = RAM(PLAYER_DIR);
    COMPARE(a, RAM(JUMP_DIR));
    if (z) {                                    /* facing right too */
        mff_cycles(0xD200, 1 + 6);
        RTS();
    }
    mff_cycles(0xD202, 3 + 4);                  /* facing left: backwards */
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(ANIM_JUMP_BACK + y));
    SET_NZ(a);
    mff_cycles(0xD207, 6);
    JSR_FN(0xD207, func_F294);                  /* player_set_anim */
    mff_cycles(0xD20A, 4);
    a = prg_read((uint16_t)(JUMP_BACK_VX_FRAC + y));
    mff_cycles(0xD20D, 4 + 4);
    RAM(OBJ_VX_FRAC) = a;
    a = prg_read((uint16_t)(JUMP_BACK_VX + y));
    mff_cycles(0xD213, 4 + 2 + 4 + 6);
    RAM(OBJ_VX) = a;
    a = FLAGS_FACING_LEFT;
    SET_NZ(a);
    RAM(OBJ_FLAGS) = a;
    RTS();
}

/*
 * player_jump_drift ($D242, proposed): a moving jump, every frame after
 * player_jump_steer started it (JMP at 0E:D1F3).
 *  - the standing jump's animation turns into the moving jump on its fourth
 *    frame (obj_anim_timer 4);
 *  - the moving jump kick turns into the jump kick, at once for Guy and on
 *    the fourth frame for the others;
 *  - then the player moves by jump_dir: obj_move_right or obj_move_left,
 *    reached by JMP (with the camera clamp of player_jump_steer first when
 *    going right; never executed in any run).
 */
void func_D242(void) {
    REGS_IN;

    mff_cycles(0xD242, 3 + 4 + 4);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    mff_cycles(0xD24A, 2);
    if (a != prg_read((uint16_t)(IS_JUMP_MOVING + y))) {
        mff_cycles(0xD24C, 4 + 2);
        if (a == prg_read((uint16_t)(IS_JUMP + y))) {
            mff_cycles(0xD251, 4 + 2 + 2);
            a = RAM(OBJ_ANIM_TIMER);
            COMPARE(a, 4);
            if (z) {                            /* the standing jump's 4th frame */
                mff_cycles(0xD258, 4);
                a = prg_read((uint16_t)(ANIM_JUMP_MOVING + y));
                SET_NZ(a);
                mff_cycles(0xD25B, 6);
                JSR_FN(0xD25B, func_F294);      /* player_set_anim */
                mff_cycles(0xD25E, 4);
                a = prg_read((uint16_t)(CHR_JUMP + y));
                SET_NZ(a);
                mff_cycles(0xD261, 6);
                JSR_FN(0xD261, func_F440);      /* player_set_chr */
            } else {
                mff_cycles(0xD256, 1);
            }
        } else {
            mff_cycles(0xD24F, 1);
        }
    } else {
        mff_cycles(0xD24A, 1);
    }

    mff_cycles(0xD264, 3 + 4 + 4);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    COMPARE(a, prg_read((uint16_t)(IS_JUMP_KICK_MOVING + y)));
    mff_cycles(0xD26C, 2);
    if (z) {                                    /* the moving jump kick */
        mff_cycles(0xD26E, 3 + 2 + 2);
        a = RAM(PLAYER_ID);
        COMPARE(a, PLAYER_GUY);
        if (z) {
            mff_cycles(0xD272, 1);              /* Guy: now */
        } else {
            mff_cycles(0xD274, 4 + 2 + 2);      /* the others: on its 4th frame */
            a = RAM(OBJ_ANIM_TIMER);
            COMPARE(a, 4);
        }
        if (z) {
            mff_cycles(0xD27B, 3 + 4);
            y = RAM(PLAYER_ID);
            a = prg_read((uint16_t)(ANIM_JUMP_KICK + y));
            SET_NZ(a);
            mff_cycles(0xD280, 6);
            JSR_FN(0xD280, func_F294);          /* player_set_anim */
            mff_cycles(0xD283, 4);
            a = prg_read((uint16_t)(CHR_JUMP_KICK + y));
            SET_NZ(a);
            mff_cycles(0xD286, 6);
            JSR_FN(0xD286, func_F440);          /* player_set_chr */
        } else {
            mff_cycles(0xD279, 1);
        }
    } else {
        mff_cycles(0xD26C, 1);
    }

    mff_cycles(0xD289, 3 + 2 + 2);
    a = RAM(JUMP_DIR) & PAD_RIGHT;
    SET_NZ(a);
    if (z) {                                    /* left */
        mff_cycles(0xD28D, 1 + 3);
        JMP_FN(0xF1A1, func_F1A1);              /* obj_move_left */
    }
    mff_cycles(0xD28F, 3 + 2);
    a = RAM(CAMERA_AUTOSCROLL);
    SET_NZ(a);
    if (n) {
        mff_cycles(0xD293, 3 + 2);
        a = RAM(CAMERA_LOCK);
        SET_NZ(a);
        if (!n) {
            mff_cycles(0xD297, 4 + 2 + 3 + 2 + 2);
            a = RAM(OBJ_X);
            c = 1;
            SBC(RAM(SCROLL_X));
            COMPARE(a, SCREEN_RIGHT_LIMIT + 1);
            if (c) {                            /* at the edge: stay there */
                mff_cycles(0xD2A1, 3 + 2 + 2 + 4 + 3 + 2 + 4 + 6);
                a = RAM(SCROLL_X);
                c = 0;
                ADC(SCREEN_RIGHT_LIMIT);
                RAM(OBJ_X) = a;
                a = RAM(AREA_SCREEN);
                ADC(0);
                RAM(OBJ_X_HI) = a;
                RTS();
            }
        }
    }
    mff_cycles(0xD2B1, 1 + 3);                  /* BPL, BMI or BCC taken, JMP */
    JMP_FN(0xF183, func_F183);                  /* obj_move_right */
}
