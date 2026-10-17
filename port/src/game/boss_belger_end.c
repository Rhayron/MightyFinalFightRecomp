/*
 * boss_belger_end.c - the scene after Belger falls, before the ending.
 *
 * Belger's life running out makes him id $9F (0C:8211); $9F (03:BED2,
 * boss_belger.c) knocks him down as id 5 with saved id $9B, and the
 * knockdown's recovery hands him back to $9B, which waits obj_ai_timer
 * frames and becomes id $7D: the scene's controller, Belger's own slot.
 * Every frame it sets special_screen_id $F9, which 0F:E6B4 (after
 * objects_update) takes back to 0 instead of updating the HUD, and drives
 * the player through player_state's dispatch (0E:C9B9) with forced pad
 * bits and game_action $28 (player_state_default, an RTS) in between, so
 * the player only moves when the scene says so. Its phases:
 *   0 the player jumps (A), Belger's last animation ($CD), the fall ($9E);
 *   1 the jump goes on, backwards, until the player lands;
 *   2 when the fall is over: the round-clear screen ($FF), music $0F;
 *   6 a wait, then the next phase (3 or 4) from obj_timer;
 *   3 by character: Cody's partner scene ($9C, then id $8F of bank $01),
 *     Guy walks off to the left; either way the player then becomes
 *     Haggar (player_id 2, two levels less) and, as Haggar, walks to the
 *     meeting point ($97-$98, $99-$9A); the controller becomes id $7B;
 *   4 the controller shows a figure entering from the right edge
 *     (animation $23, sprite bank $36: by the story Jessica, proposed);
 *   5 it walks left to x $B0; 7 a last wait, then game_action $0E ends
 *     the round and player_id is the player's again.
 * The ids $7B and $7D share the behaviour; they differ in the sprite data
 * object_draw maps for them ($EB93: $0A for $7B, $02 for $7D), which is
 * why phase 3 switches the id before phase 4's animation.
 *
 * No route plays it: the bots' tours end Belger's fight by skipping the
 * area. The CDL runs reached it, and the old-vs-new differential harness
 * covers every path here but one (the footstep sound, 0B:BE60).
 *
 * Bank $0B ($7B/$7D) is the scaffold's bank 5, bank $03 ($9B, $9C, $9E)
 * its bank 1.
 */
#include "enemy.h"

/* ---- ids ---- */
#define OBJ_SCENE           0x7D    /* the controller, after $9B */
#define OBJ_SCENE_B         0x7B    /* the same, drawn from bank $0A's sprite data */
#define OBJ_CODY_PARTNER    0x9C    /* Cody's part of the scene */
#define OBJ_CODY_STANDIN    0x8F    /* $9C becomes it (01:A09B) */
#define OBJ_BELGER_FALL     0x9E

/* ---- the scene's commands ---- */
#define SCREEN_NO_HUD       0xF9    /* every frame: 0F:E6B4 clears it, no HUD update */
#define SCREEN_ROUND_CLEAR  0xFF    /* 04:984C */
#define MUSIC_SCENE         0x0F
#define ACT_SCENE           0x28    /* player_state_default: the player stands still */
#define ACT_LANDED          0x02
#define PAD_A               0x80
#define SFX_FOOTSTEP        0x13
#define SCENE_SLOT_SAVE     (OBJ_SPAWN_GROUP + 0x0F)   /* $04FF: X across player_state */

/* phase numbers as obj_state values (bit 7 live) */
#define PHASE_MEET          0x83
#define PHASE_ENTER         0x84
#define PHASE_WAIT          0x86
#define PHASE_LAST_WAIT     0x87

/* the controller's phases ($BF9F/$BFA7) */
#define SCENE_PHASE_LO      0xBF9F
#define SCENE_PHASE_HI      0xBFA7

/* Belger's last animation and the frame where the fall takes over */
#define BELGER_LAST_ANIM    0xCD
#define BELGER_LAST_CHR     0x34
#define FALL_ANIM           0xCF
#define FALL_CHR            0x3B
#define FALL_FRAME          0x0B
#define FALL_FRAMES         0xE0
#define FALL_BLINK_FROM     0xA0    /* blinks while obj_ai_timer is $5F-$9F */
#define FALL_BLINK_TO       0x5F

/* Haggar's meeting point (slot 0) */
#define MEET_Y_MIN          0x99
#define MEET_Y_MAX          0x9B
#define MEET_X_MIN          0x97
#define MEET_X_MAX          0x99
#define HAGGAR_CHR          0x08    /* player_chr_stand[2] */
#define GUY_EXIT_X          0x1B

/* Haggar's entry of the special move's tables (0E:CA54: $E228/$E270
 * by player_id), read directly */
#define HAGGAR_SPECIAL_ANIM 0xE22A
#define HAGGAR_SPECIAL_CHR  0xE272

void func_BE79_b5(void);

/*
 * belger_end_scene, $BD7F (bank $0B; name proposed): behaviour of ids $7D
 * and $7B. Each frame: special_screen_id $F9 (no HUD update), obj_invuln
 * 1, then the phase ($BF9F/$BFA7).
 * In: X = slot. Out: as the phase.
 * Caller: the behaviour table (ids $7B, $7D: 0B:B321, B327).
 */
void func_BD7F_b5(void)
{
    REGS_IN;

    mff_cycles(0xBD7F, 12);
    a = SCREEN_NO_HUD;
    RAM(SPECIAL_SCREEN_ID) = a;
    a = 0x01;
    RAM(OBJ_INVULN + x) = a;
    SYNC_OUT;
    obj_phase_jump(0xBD88, SCENE_PHASE_LO, SCENE_PHASE_HI);
}

/*
 * $BD9B, scene phase 0 (name proposed: end_scene_start): the player
 * jumps: pad A forced into joy_pressed/joy_held and player_state run once
 * from idle (0E:C9B9, X kept in $04FF); then game_action $28; Belger's
 * last animation $CD (sprite bank R4 $34); phase 1, and JMP $BFAF for the
 * fall.
 */
void func_BD9B_b5(void)
{
    REGS_IN;

    mff_cycles(0xBD9B, 23);
    a = PAD_A;
    RAM(JOY_PRESSED) = a;
    RAM(JOY_HELD) = a;
    a = ACT_IDLE;
    RAM(GAME_ACTION) = a;
    RAM(SCENE_SLOT_SAVE) = x;
    CALL_IN(0xBDA8, 0xC9B9, BANK_0B);          /* player_state, after its respawn step */
    mff_cycles(0xBDAB, 19);
    x = RAM(SCENE_SLOT_SAVE);
    a = ACT_SCENE;
    RAM(GAME_ACTION) = a;
    a = BELGER_LAST_ANIM;
    y = BELGER_LAST_CHR;
    CALL_FIXED(0xBDB6, func_F449);              /* obj_set_anim, sprite bank R4 = Y */
    mff_cycles(0xBDB9, 10);
    RAM(OBJ_STATE + x)++;
    GO_IN(0xBFAF, BANK_0B);
}

/*
 * $BDBF, scene phase 1 (name proposed: end_scene_hop_back): the jump goes
 * on one frame at a time, away from the way the player faces: jump_dir
 * and the pad = ~player_dir & 3, game_action 1, player_state; once it has
 * landed (game_action 2) phase 2. Then game_action $28 again.
 */
void func_BDBF_b5(void)
{
    REGS_IN;

    mff_cycles(0xBDBF, 31);
    a = (uint8_t)(RAM(PLAYER_DIR) ^ 0xFF) & 0x03;
    RAM(JUMP_DIR) = a;
    RAM(JOY_PRESSED) = a;
    RAM(JOY_HELD) = a;
    a = ACT_JUMP;
    RAM(GAME_ACTION) = a;
    RAM(SCENE_SLOT_SAVE) = x;
    CALL_IN(0xBDD2, 0xC9B9, BANK_0B);          /* player_state */
    mff_cycles(0xBDD5, 11);
    x = RAM(SCENE_SLOT_SAVE);
    a = RAM(GAME_ACTION);
    if (a == ACT_LANDED) {
        mff_cycles(0xBDDE, 7);
        RAM(OBJ_STATE + x)++;
    } else {
        mff_cycles(0xBDDC, 1);
    }
    mff_cycles(0xBDE1, 11);
    a = ACT_SCENE;
    RAM(GAME_ACTION) = a;
    RTS_RETURN();
}

/*
 * $BDE6, scene phase 2 (name proposed: end_scene_round_clear): once the
 * fall (the slot in obj_timer2) has ended: special_screen_id $FF (the
 * round-clear screen, 04:984C), music $0F, a wait of $50 frames (phase 6)
 * before phase 3; the player's scene step (slot 0's obj_down_timer),
 * player_no_hit and the drop-in timers cleared, respawn_timer2 1 and
 * 0E:CED1 run once so the drop-in flashing ends; player_id saved in
 * obj_ground_y; for Cody slot 0's obj_flags $D0.
 */
void func_BDE6_b5(void)
{
    REGS_IN;

    mff_cycles(0xBDE6, 10);
    y = RAM(OBJ_TIMER2 + x);
    a = RAM(OBJ_STATE + y);
    if (a & OBJ_LIVE) {
        mff_cycles(0xBDEC, 2);
    } else {
        mff_cycles(0xBDEE, 13);
        a = SCREEN_ROUND_CLEAR;
        RAM(SPECIAL_SCREEN_ID) = a;
        a = MUSIC_SCENE;
        CALL_FIXED(0xBDF4, func_F34A);          /* sound_queue_push (music) */
        mff_cycles(0xBDF7, 44);
        a = 0x50;
        RAM(OBJ_AI_TIMER + x) = a;
        a = PHASE_WAIT;
        RAM(OBJ_STATE + x) = a;
        a = PHASE_MEET;
        RAM(OBJ_TIMER + x) = a;
        a = 0x00;
        RAM(OBJ_DOWN_TIMER) = a;
        RAM(PLAYER_NO_HIT) = a;
        RAM(RESPAWN_TIMER1) = a;
        a = 0x01;
        RAM(RESPAWN_TIMER2) = a;
        CALL_IN(0xBE13, 0xCED1, BANK_0B);      /* the drop-in invincibility step */
        mff_cycles(0xBE16, 10);
        a = RAM(PLAYER_ID);
        RAM(OBJ_GROUND_Y + x) = a;
        if (a == PLAYER_CODY) {
            mff_cycles(0xBE1D, 6);
            a = 0xD0;
            RAM(OBJ_FLAGS) = a;
        } else {
            mff_cycles(0xBE1B, 1);
        }
    }
    mff_cycles(0xBE22, 6);
    RTS_RETURN();
}

/*
 * $BE59-$BE78: Cody's scene steps 1 and 2 (slot 0's obj_down_timer, which
 * $9C and id $8F advance). Step 1: the player, Haggar's special-move
 * sprite since $9C, comes in from the left edge two pixels a frame, a
 * footstep (sound $13) every 8 frames of obj_ai_timer. Step 2: animation
 * 3 and Haggar's sprite bank (8).
 */
static void cody_scene_walk(void)
{
    REGS_IN;

    mff_cycles(0xBE59, 8);
    a = RAM(OBJ_AI_TIMER + x) & 0x07;
    if (a == 0) {
        mff_cycles(0xBE60, 8);
        a = SFX_FOOTSTEP;
        CALL_FIXED(0xBE62, func_F34C);          /* sound_queue_push_sfx */
    } else {
        mff_cycles(0xBE5E, 1);
    }
    mff_cycles(0xBE65, 25);
    RAM(OBJ_AI_TIMER + x)++;
    RAM(OBJ_X) += 2;                            /* INC, INC */
    RTS_RETURN();
}

static void cody_scene_stand(void)
{
    REGS_IN;

    mff_cycles(0xBE6F, 8);
    a = 0x03;
    CALL_FIXED(0xBE71, func_F294);              /* player_set_anim */
    mff_cycles(0xBE74, 5);
    a = HAGGAR_CHR;
    SET_NZ(a);
    GO_FIXED(0xF440, func_F440);                /* set_chr_bank2 */
}

/*
 * $BE8E-$BECD: Guy's scene: he walks left (pad Left, from idle, through
 * player_state); past x $1B he is hidden, becomes Haggar ($BE79) at x 0,
 * y $9A, and the scene waits $80 frames (phase 6) before phase 3 again.
 * game_action $28 after each step.
 */
static void guy_scene(void)
{
    REGS_IN;

    mff_cycles(0xBE8E, 23);
    a = DIR_LEFT;
    RAM(JOY_PRESSED) = a;
    RAM(JOY_HELD) = a;
    a = ACT_IDLE;
    RAM(GAME_ACTION) = a;
    RAM(SCENE_SLOT_SAVE) = x;
    CALL_IN(0xBE9B, 0xC9B9, BANK_0B);          /* player_state */
    mff_cycles(0xBE9E, 12);
    x = RAM(SCENE_SLOT_SAVE);
    a = RAM(OBJ_X);
    if (a < GUY_EXIT_X) {
        mff_cycles(0xBEA8, 12);
        a = 0x94;
        RAM(OBJ_FLAGS) = a;
        CALL_IN(0xBEAD, 0xBE79, BANK_0B);      /* the player becomes Haggar */
        mff_cycles(0xBEB0, 33);
        a = 0x9A;
        RAM(OBJ_Y) = a;
        a = 0x00;
        RAM(OBJ_X) = a;
        a = PHASE_MEET;
        RAM(OBJ_TIMER + x) = a;
        a = PHASE_WAIT;
        RAM(OBJ_STATE + x) = a;
        a = 0x80;
        RAM(OBJ_AI_TIMER + x) = a;
    } else {
        mff_cycles(0xBEA6, 1);
    }
    mff_cycles(0xBEC9, 11);
    a = ACT_SCENE;
    RAM(GAME_ACTION) = a;
    RTS_RETURN();
}

/*
 * $BE23, scene phase 3 (name proposed: end_scene_meet): slot 0 shown, then
 * by player_id. Cody (0), by slot 0's obj_down_timer: step 0 puts id $9C
 * in a free slot (03:BFBE: the stand-in for Cody, then id $8F) and
 * obj_ai_timer $80; steps 1 and 2 above; step 3 the player becomes Haggar
 * ($BE79). Guy (1): guy_scene. Haggar (2, which the other two become):
 * $BECE.
 */
void func_BE23_b5(void)
{
    REGS_IN;

    mff_cycles(0xBE23, 15);
    a = RAM(OBJ_FLAGS) & (uint8_t)~OBJF_HIDDEN;
    RAM(OBJ_FLAGS) = a;
    a = RAM(PLAYER_ID);
    if (a != PLAYER_CODY) {
        mff_cycles(0xBE2F, 4);
        c = a >= PLAYER_GUY;
        if (a == PLAYER_GUY) {
            mff_cycles(0xBE31, 1);
            SYNC_OUT;
            guy_scene();
            return;
        }
        mff_cycles(0xBE33, 3);
        GO_IN(0xBECE, BANK_0B);
    }
    mff_cycles(0xBE2D, 1);
    mff_cycles(0xBE36, 8);
    a = RAM(OBJ_DOWN_TIMER);
    c = a >= 0x01;
    if (a == 0x01) {
        mff_cycles(0xBE3B, 1);
        SYNC_OUT;
        cody_scene_walk();
        return;
    }
    mff_cycles(0xBE3D, 4);
    c = a >= 0x02;
    if (a == 0x02) {
        mff_cycles(0xBE3F, 1);
        SYNC_OUT;
        cody_scene_stand();
        return;
    }
    mff_cycles(0xBE41, 4);
    c = a >= 0x03;
    if (a == 0x03) {
        mff_cycles(0xBE43, 1);
        SYNC_OUT;
        func_BE79_b5();
        return;
    }
    mff_cycles(0xBE45, 6);
    CALL_FIXED(0xBE45, func_F71A);              /* find_free_slot (the carry is not tested) */
    mff_cycles(0xBE48, 30);
    a = 0x80;
    RAM(OBJ_STATE + y) = a;
    RAM(OBJ_AI_TIMER + x) = a;
    a = OBJ_CODY_PARTNER;
    RAM(OBJ_ID + y) = a;
    RAM(OBJ_SAVED_ID + y) = a;
    RTS_RETURN();
}

/*
 * end_scene_become_haggar, $BE79 (bank $0B; name proposed): the player
 * becomes Haggar for the rest of the scene: player_level - 2 (Haggar's
 * tables stop at level 3, Cody's and Guy's at 5: 0C:89F4-8A20),
 * player_id 2, palette entries $17 = $19 and $16 = $30, uploaded at the
 * next NMI. phase 7 restores player_id; the level stays lowered.
 * Out: the RTS.
 * Callers: Cody's step 3 (the branch from 0B:BE43), Guy's scene
 * (0B:BEAD).
 */
void func_BE79_b5(void)
{
    REGS_IN;

    mff_cycles(0xBE79, 36);
    RAM(PLAYER_LEVEL) -= 2;                     /* DEC, DEC */
    a = PLAYER_HAGGAR;
    RAM(PLAYER_ID) = a;
    a = 0x19;
    RAM(PALETTE_BUF + 23) = a;
    a = 0x30;
    RAM(PALETTE_BUF + 22) = a;
    RAM(PALETTE_REQUEST) = a;
    RTS_RETURN();
}

/*
 * end_scene_haggar_walk, $BECE (bank $0B; name proposed): Haggar's sprite
 * bank (8), then one step toward the meeting point through player_state
 * with a forced pad: Down while slot 0's y is below $99, Up from $9B,
 * then Right below x $97, Left from $99; game_action $28 after. There
 * (y $99-$9A, x $97-$98): slot 0 shown ($D0), game_action $0A, the
 * controller becomes id $7B and waits $50 frames (phase 6) before
 * phase 4. The carry of the last compare goes into player_state.
 * Caller: JMP at 0B:BE33 (phase 3, player_id 2).
 */
void func_BECE_b5(void)
{
    REGS_IN;

    mff_cycles(0xBECE, 8);
    a = HAGGAR_CHR;
    SET_NZ(a);
    CALL_FIXED(0xBED0, func_F440);              /* set_chr_bank2 */
    mff_cycles(0xBED3, 8);
    a = RAM(OBJ_Y);
    c = a >= MEET_Y_MIN;
    if (!c) {
        mff_cycles(0xBEDA, 4);
        a = DIR_DOWN;
        mff_cycles(0xBEDC, 1);
    } else {
        mff_cycles(0xBED8, 1);
        mff_cycles(0xBEDE, 4);
        c = a >= MEET_Y_MAX;
        if (c) {
            mff_cycles(0xBEE2, 4);
            a = DIR_UP;
            mff_cycles(0xBEE4, 1);
        } else {
            mff_cycles(0xBEE0, 1);
            mff_cycles(0xBEE6, 8);
            a = RAM(OBJ_X);
            c = a >= MEET_X_MIN;
            if (!c) {
                mff_cycles(0xBEED, 4);
                a = DIR_RIGHT;
                mff_cycles(0xBEEF, 1);
            } else {
                mff_cycles(0xBEEB, 1);
                mff_cycles(0xBEF1, 4);
                c = a >= MEET_X_MAX;
                if (!c) {                       /* there */
                    mff_cycles(0xBEF3, 2);
                    mff_cycles(0xBF0D, 45);
                    a = 0xD0;
                    RAM(OBJ_FLAGS) = a;
                    a = ACT_HELD;
                    RAM(GAME_ACTION) = a;
                    a = OBJ_SCENE_B;
                    RAM(OBJ_ID + x) = a;
                    a = 0x50;
                    RAM(OBJ_AI_TIMER + x) = a;
                    a = PHASE_ENTER;
                    RAM(OBJ_TIMER + x) = a;
                    a = PHASE_WAIT;
                    RAM(OBJ_STATE + x) = a;
                    RTS_RETURN();
                }
                mff_cycles(0xBEF5, 2);
                a = DIR_LEFT;
            }
        }
    }
    mff_cycles(0xBEF7, 21);
    RAM(JOY_PRESSED) = a;
    RAM(JOY_HELD) = a;
    a = ACT_IDLE;
    RAM(GAME_ACTION) = a;
    RAM(SCENE_SLOT_SAVE) = x;
    CALL_IN(0xBF02, 0xC9B9, BANK_0B);          /* player_state */
    mff_cycles(0xBF05, 15);
    x = RAM(SCENE_SLOT_SAVE);
    a = ACT_SCENE;
    RAM(GAME_ACTION) = a;
    RTS_RETURN();
}

/*
 * $BF2B, scene phase 4 (name proposed: end_scene_enter): the controller
 * appears at the right edge (x $F8, y $98, obj_flags $90, obj_vx $0101)
 * with animation $23 (0F:F288), sprite bank R3 $36 and palette entries
 * $1B = $15, $1A = $26; phase 5.
 */
void func_BF2B_b5(void)
{
    REGS_IN;

    mff_cycles(0xBF2B, 41);
    a = 0x90;
    RAM(OBJ_FLAGS + x) = a;
    a = 0x01;
    RAM(OBJ_VX + x) = a;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0xF8;
    RAM(OBJ_X + x) = a;
    a = 0x98;
    RAM(OBJ_Y + x) = a;
    a = 0x23;
    CALL_FIXED(0xBF44, func_F288);              /* obj_set_anim */
    mff_cycles(0xBF47, 33);
    a = 0x36;
    RAM(CHR_BANKS + 3) = a;
    a = 0x15;
    RAM(PALETTE_BUF + 27) = a;
    a = 0x26;
    RAM(PALETTE_BUF + 26) = a;
    RAM(PALETTE_REQUEST) = a;
    RAM(OBJ_STATE + x)++;
    RTS_RETURN();
}

/*
 * $BF5B, scene phase 5 (name proposed: end_scene_walk_in): moves left
 * (0F:F1A1) until x is below $B0, then animation $22, obj_ai_timer $FF and
 * phase 7.
 */
void func_BF5B_b5(void)
{
    REGS_IN;

    mff_cycles(0xBF5B, 6);
    CALL_FIXED(0xBF5B, func_F1A1);              /* obj_move_left */
    mff_cycles(0xBF5E, 8);
    a = RAM(OBJ_X + x);
    c = a >= 0xB0;
    if (!c) {
        mff_cycles(0xBF65, 8);
        a = 0x22;
        CALL_FIXED(0xBF67, func_F288);          /* obj_set_anim */
        mff_cycles(0xBF6A, 14);
        a = 0xFF;
        RAM(OBJ_AI_TIMER + x) = a;
        a = PHASE_LAST_WAIT;
        RAM(OBJ_STATE + x) = a;
    } else {
        mff_cycles(0xBF63, 1);
    }
    mff_cycles(0xBF74, 6);
    RTS_RETURN();
}

/*
 * $BF75, scene phase 6 (name proposed: end_scene_wait): while player_id
 * is still the one saved in phase 2, special_screen_id back to 0 (the HUD
 * is updated); game_action $28; after obj_ai_timer frames the phase saved
 * in obj_timer.
 */
void func_BF75_b5(void)
{
    REGS_IN;

    mff_cycles(0xBF75, 9);
    a = RAM(OBJ_GROUND_Y + x);
    if (a == RAM(PLAYER_ID)) {
        mff_cycles(0xBF7C, 5);
        a = 0x00;
        RAM(SPECIAL_SCREEN_ID) = a;
    } else {
        mff_cycles(0xBF7A, 1);
    }
    mff_cycles(0xBF80, 14);
    a = ACT_SCENE;
    RAM(GAME_ACTION) = a;
    RAM(OBJ_AI_TIMER + x)--;
    if (RAM(OBJ_AI_TIMER + x) == 0) {
        mff_cycles(0xBF89, 9);
        a = RAM(OBJ_TIMER + x);
        RAM(OBJ_STATE + x) = a;
    } else {
        mff_cycles(0xBF87, 1);
    }
    mff_cycles(0xBF8F, 6);
    RTS_RETURN();
}

/*
 * $BF90, scene phase 7 (name proposed: end_scene_finish): after
 * obj_ai_timer ($FF) frames, game_action $0E (the round ends; after round
 * 5 the ending follows) and player_id = the one saved in phase 2.
 */
void func_BF90_b5(void)
{
    REGS_IN;

    mff_cycles(0xBF90, 9);
    RAM(OBJ_AI_TIMER + x)--;
    if (RAM(OBJ_AI_TIMER + x) == 0) {
        mff_cycles(0xBF95, 12);
        a = ACT_SKIP_AREA;
        RAM(GAME_ACTION) = a;
        a = RAM(OBJ_GROUND_Y + x);
        RAM(PLAYER_ID) = a;
    } else {
        mff_cycles(0xBF93, 1);
    }
    mff_cycles(0xBF9E, 6);
    RTS_RETURN();
}

/*
 * belger_fall_spawn, $BFAF (bank $0B; name proposed): id $9E in a free
 * slot at Belger's x and y (x high byte 2, the boss room's screen),
 * hidden (obj_flags $94), invulnerable, obj_ai_timer $E0, obj_timer =
 * Belger's slot; Belger's obj_timer2 = its slot. unk_1b ($1B) = $FF: the
 * only access to $1B in the code the P0 map knows, nothing reads it. With
 * no free slot nothing happens (phase 2 then goes on at once: obj_timer2
 * keeps whatever it held).
 * In: X = Belger's slot. Out: the RTS.
 * Caller: JMP at 0B:BDBC (scene phase 0).
 */
void func_BFAF_b5(void)
{
    REGS_IN;

    mff_cycles(0xBFAF, 6);
    CALL_FIXED(0xBFAF, func_F71A);              /* find_free_slot */
    mff_cycles(0xBFB2, 2);
    if (!c) {
        mff_cycles(0xBFB4, 84);
        a = RAM(OBJ_X + x);
        RAM(OBJ_X + y) = a;
        a = RAM(OBJ_Y + x);
        RAM(OBJ_Y + y) = a;
        a = 0x02;
        RAM(OBJ_X_HI + y) = a;
        a = 0xFF;
        RAM(UNK_1B) = a;
        a = OBJ_BELGER_FALL;
        RAM(OBJ_ID + y) = a;
        RAM(OBJ_SAVED_ID + y) = a;
        a = 0x80;
        RAM(OBJ_STATE + y) = a;
        a = 0xFF;
        RAM(OBJ_INVULN + y) = a;
        a = FALL_FRAMES;
        RAM(OBJ_AI_TIMER + y) = a;
        a = x;
        RAM(OBJ_TIMER + y) = a;
        a = 0x94;
        RAM(OBJ_FLAGS + y) = a;
        a = y;
        RAM(OBJ_TIMER2 + x) = a;
    } else {
        mff_cycles(0xBFB2, 1);
    }
    mff_cycles(0xBFED, 6);
    RTS_RETURN();
}

/* ======================= bank $03 ======================= */

/*
 * belger_down, $BF38 (bank $03; name proposed): behaviour of id $9B,
 * Belger knocked down for good (the saved id $9F leaves for the knockdown,
 * 03:BED2): after obj_ai_timer frames, obj_state $80 and id $7D, the
 * scene's controller.
 * In: X = slot. Out: the RTS.
 * Caller: the behaviour table (id $9B, 03:B321).
 */
void func_BF38_b1(void)
{
    REGS_IN;

    mff_cycles(0xBF38, 9);
    RAM(OBJ_AI_TIMER + x)--;
    if (RAM(OBJ_AI_TIMER + x) == 0) {
        mff_cycles(0xBF3D, 14);
        a = 0x80;
        RAM(OBJ_STATE + x) = a;
        a = OBJ_SCENE;
        RAM(OBJ_ID + x) = a;
    } else {
        mff_cycles(0xBF3B, 1);
    }
    mff_cycles(0xBF47, 6);
    RTS_RETURN();
}

/*
 * belger_fall, $BF48 (bank $03; name proposed): behaviour of id $9E,
 * Belger's last fall (by the story, out of the window). Always
 * invulnerable. Phase 0 waits for Belger's slot (obj_timer) to reach frame
 * $0B of his last animation; then it shows itself (animation $CF, sprite
 * bank R4 $3B, obj_flags $90), hides Belger (his obj_flags = $94) and
 * goes to phase 1. Every frame from then on obj_ai_timer - 1 (from $E0):
 * the sprite blinks while it is $5F-$9F, and at 0 the slot is cleared.
 * In: X = slot. Out: the RTS, or object_clear's.
 * Caller: the behaviour table (id $9E, 03:B32A).
 */
void func_BF48_b1(void)
{
    REGS_IN;

    mff_cycles(0xBF48, 15);
    a = 0x01;
    RAM(OBJ_INVULN + x) = a;
    a = RAM(OBJ_STATE + x) & OBJ_PHASE;
    if (a == 0) {
        mff_cycles(0xBF54, 12);
        y = RAM(OBJ_TIMER + x);
        a = RAM(OBJ_ANIM_FRAME + y);
        c = a >= FALL_FRAME;
        if (a != FALL_FRAME) {
            mff_cycles(0xBF5C, 1);
            mff_cycles(0xBF94, 6);
            RTS_RETURN();
        }
        mff_cycles(0xBF5E, 45);
        a = FALL_ANIM;
        RAM(OBJ_ANIM + x) = a;
        a = FALL_CHR;
        RAM(CHR_BANK4) = a;
        a = 0x00;
        RAM(OBJ_ANIM_FRAME + x) = a;
        RAM(OBJ_ANIM_TIMER + x) = a;
        a = 0x90;
        RAM(OBJ_FLAGS + x) = a;
        a |= OBJF_HIDDEN;
        RAM(OBJ_FLAGS + y) = a;
        RAM(OBJ_STATE + x)++;
    } else {
        mff_cycles(0xBF52, 1);
    }
    mff_cycles(0xBF7C, 9);
    RAM(OBJ_AI_TIMER + x)--;
    if (RAM(OBJ_AI_TIMER + x) == 0) {
        mff_cycles(0xBF7F, 1);
        mff_cycles(0xBFBB, 3);
        GO_FIXED(0xF2BE, func_F2BE);            /* object_clear */
    }
    mff_cycles(0xBF81, 8);
    a = RAM(OBJ_AI_TIMER + x);
    if (a >= FALL_BLINK_TO) {
        mff_cycles(0xBF88, 4);
        if (a < FALL_BLINK_FROM) {
            mff_cycles(0xBF8C, 11);
            a = RAM(OBJ_FLAGS + x) ^ OBJF_HIDDEN;
            RAM(OBJ_FLAGS + x) = a;
        } else {
            mff_cycles(0xBF8A, 1);
        }
    } else {
        mff_cycles(0xBF86, 1);
    }
    mff_cycles(0xBF94, 6);
    RTS_RETURN();
}

/*
 * cody_scene_partner, $BFBE (bank $03; name proposed): behaviour of id
 * $9C, put in Cody's scene step 0. It stands in for Cody where the player
 * stood (obj_flags $D0, animation $FE, sprite bank R4 $31) and becomes id
 * $8F (01:A09B, bank $01), and the player turns into Haggar's
 * special-move sprite ($E22A/$E272, Haggar's entries of the tables the
 * A+B special uses at 0E:CA54) at the left edge (x 0), facing right;
 * slot 0's obj_down_timer + 1 (step 1); palette entries $17 = $19 and
 * $1B = $21.
 * In: X = slot. Out: as 0F:F449.
 * Caller: the behaviour table (id $9C, 03:B324).
 */
void func_BFBE_b1(void)
{
    REGS_IN;

    mff_cycles(0xBFBE, 41);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + x) = a;
    a = RAM(OBJ_X);
    RAM(OBJ_X + x) = a;
    a = 0xD0;
    RAM(OBJ_FLAGS + x) = a;
    RAM(OBJ_DOWN_TIMER)++;
    a = 0x00;
    RAM(OBJ_X) = a;
    a = prg_read(HAGGAR_SPECIAL_ANIM);
    mff_cycles(0xBFDA, 6);
    CALL_FIXED(0xBFDA, func_F294);              /* player_set_anim */
    mff_cycles(0xBFDD, 4);
    a = prg_read(HAGGAR_SPECIAL_CHR);
    SET_NZ(a);
    mff_cycles(0xBFE0, 6);
    CALL_FIXED(0xBFE0, func_F440);              /* set_chr_bank2 */
    mff_cycles(0xBFE3, 6);
    CALL_FIXED(0xBFE3, func_F03F);              /* player_face_right */
    mff_cycles(0xBFE6, 29);
    a = OBJ_CODY_STANDIN;
    RAM(OBJ_ID + x) = a;
    a = 0x19;
    RAM(PALETTE_BUF + 23) = a;
    a = 0x21;
    RAM(PALETTE_BUF + 27) = a;
    RAM(PALETTE_REQUEST) = a;
    a = 0xFE;
    y = 0x31;
    GO_FIXED(0xF449, func_F449);                /* obj_set_anim, sprite bank R4 = Y */
}
