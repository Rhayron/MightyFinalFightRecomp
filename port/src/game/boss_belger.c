/*
 * boss_belger.c - Belger, round 5's boss (id $74, bank $0B; the scaffold's
 * bank 5), his shot (id $9D) and his death (id $9F), both in bank $03
 * (bank 1).
 *
 * Evidence: id $74 lives only in round 5's last area (round 4, sub_area 2),
 * slot $0C, obj_kind $0B, in the five tours (base_cody_tour.flog from frame
 * 25123); Data Crystal lists $74 as Cyborg Belger. The screenshots of
 * cody_tour show him (the white-suited old man) punching at frame 26720
 * and, after the change, firing from the screen's left edge at 28530.
 *
 * He fights in two forms; obj_tier says which, and each has its phase
 * table (obj_state & $0F):
 *
 *   first form ($BCE6/$BCF7)          second form ($BCEC/$BCFD)
 *   0 $B80B start                     0 $B929 start: to his page
 *   1 $BB8C hurt                      1 $BB8C hurt
 *   2 $BBAC back to his page (n.e.)   2 $BBAC back to his page (n.e.)
 *   3 $B825 approach                  3 $B958 turn away
 *   4 $B86F circle the player         4 $B97B patrol up and down, shoot
 *   5 $B8D3 punch                     5 $BA68 walk to the screen edge
 *                                     6 $BA9A fire a shot
 *                                     7 $BAD0 rest ($5A frames)
 *                                     8 $BAE8 dizzy (never executed)
 *                                     9 $BB05 punch, then leap
 *                                    10 $BB53 the leap
 *
 * The change comes when his life drops under $5A: music $F2, then, once
 * the player stands, the talk page $3C and obj_tier 1 (music 6 from then
 * on). In the tours it only happens after the bots' patch of his life to
 * 1 (frame 28298 in cody_tour), so the first form never reaches it there
 * by damage. While the boss talk runs (id $6E in slot $0D, present at the
 * start of every boss area in the tours, with game_action $0A) he only
 * stands, invulnerable.
 *
 * Frames in the five tours (form, phase): first form 0: 7504, 1: 165,
 * 3: 223, 4: 2257, 5: 4586; second form 0: 898, 3: 5, 4: 18491, 5: 1693,
 * 6: 17604, 7: 29798, 9: 864, 10: 384 (belger_survey.py over
 * runs/p4/base_*.flog).
 */
#include "enemy.h"

/* ---- ids, kinds, slots ---- */
#define OBJ_BELGER_SHOT     0x9D
#define OBJ_BELGER_DOWN     0x9B    /* 03:BF38: lies, then the closing scene */
#define KIND_BELGER_SHOT    0x0C
#define BELGER_SLOT         (OBJ_SPAWN_GROUP + 0x0F)    /* $04FF: Belger's slot (0E:CBE1 reads it) */

/* ---- obj_state values ---- */
#define BELGER_RESTART      0x80    /* phase 0 */
#define BELGER_HURT         0x81
#define BELGER_TO_PAGE      0x82
#define BELGER_PHASE3       0x83
#define BELGER_PHASE4       0x84
#define BELGER_PHASE5       0x85
#define BELGER_SHOOT        0x86
#define BELGER_REST         0x87
#define BELGER_DIZZY        0x88
#define BELGER_PUNCH2       0x89
#define BELGER_LEAPING      0x8A    /* obj_ground_y and the y clamp stay put */

/* ---- animations (obj_set_anim with sprite bank R4, 0F:F449) ---- */
#define ANIM_WAIT           0xBC    /* standing: the talk, held */
#define ANIM_MOVE           0xBD
#define ANIM_SHOOT          0xBF
#define ANIM_PUNCH          0xC0
#define ANIM_REST           0xC2
#define ANIM_HURT           0xC3
#define ANIM_KILLED         0xC4
#define ANIM_DIZZY          0xC6
#define ANIM_LEAP           0xCC
#define ANIM_SHOT           0xC7    /* the shot (0F:F2A0) */
#define CHR_BELGER_33       0x33
#define CHR_BELGER_38       0x38
#define CHR_BELGER_39       0x39
#define CHR_BELGER_3A       0x3A

/* ---- attacks (obj_attack: out, reach box; obj_attack_damage) ---- */
#define PUNCH_ATTACK        0x85
#define PUNCH_DAMAGE        0x11
#define LEAP_ATTACK         0x90
#define LEAP_DAMAGE         0x34
#define SHOT_ATTACK         0x87
#define SHOT_DAMAGE         0x34

/* ---- sound ---- */
#define MUSIC_CHANGE        0xF2    /* pushed when the change starts */
#define MUSIC_SECOND_FORM   0x06
#define SFX_PUNCH_HIT       0x18
#define SFX_LEAP            0x12
#define SFX_SHOT            0x2C

/* ---- life, lines, limits ---- */
#define LIFE_SECOND_FORM    0x5A    /* the change below this */
#define LIFE_HOLD_MIN       0x26    /* held under this he breaks free (never executed) */
#define TALK_PAGE_CHANGE    0x3C    /* special_screen_id: his talk before the second form */
#define GRAB_HOLD_MAX       0x0C    /* the player holds him at most this long */
#define BELGER_Y_TOP        0x82
#define BELGER_Y_BOTTOM     0xAE
#define BELGER_PAGE         0x02    /* obj_x_hi of the fight */
#define BELGER_SHOTS        0x05    /* shots before a rest */
#define PROBE_BLOCKED       0x50    /* TMP16 bits of the background probes */

/* ---- tables ---- */
#define PHASES1_LO          0xBCE6
#define PHASES1_HI          0xBCF7
#define PHASES2_LO          0xBCEC
#define PHASES2_HI          0xBCFD
#define SPEED_VX            0xBD0C  /* by the 16-way index */
#define SPEED_VX_FRAC       0xBD1C
#define SPEED_VY            0xBD2C
#define SPEED_VY_FRAC       0xBD3C
#define CIRCLE_ONE_WAY      0xBD4C  /* index a quarter turn on; bit 7: lined up, try a punch */
#define CIRCLE_OTHER_WAY    0xBD5C
#define CIRCLE_FRAMES       0xBD6C  /* frames to the next turn */
#define SHOT_DX             0xBD08  /* $001A ahead facing right, $FFE6 facing left */
#define SHOT_DX_HI          0xBD09
#define ESCAPE_PLAYER_CHR   0xBD7C  /* the player's sprite bank by player_id */

void func_B743_b5(void);
void func_B84B_b5(void);
void func_B88C_b5(void);
void func_B9B7_b5(void);
void func_BB0A_b5(void);

/*
 * belger, $B696 (bank $0B): behaviour of id $74. Every frame: his slot in
 * $04FF, obj_ground_y = obj_y (not while leaping), obj_invuln counted
 * down, obj_kind $0B, no attack out. Then, in order:
 * - while the boss talk runs (slot $0D id $6E) he stands (anim $BC),
 *   invulnerable (obj_invuln + 1 each frame);
 * - hit (obj_hit_timer set): the hurt phase ($81, anim $BC);
 * - held by the player (grab or grapple hit, his slot in grab_slot): back
 *   to phase 0 facing the player, combo_timer 0 and the hold cut to
 *   $0C frames; with life under $26 he would break free instead (player
 *   released, dizzy phase $88) and a grapple hit would show anim $C3;
 *   neither ever ran;
 * - life under $5A: the change. First frame: music $F2, obj_timer2 1 (and,
 *   if not invulnerable, knocked down: never executed); then, once the
 *   player stands, punches or uses the special: talk page $3C, obj_timer2 2,
 *   obj_tier 1;
 * - y kept in $82-$AE (not while leaping), and the phase of his form, the
 *   second one with music 6.
 * In: X = slot. Out: as the phase.
 * Caller: the behaviour table (id $74, 0B:B30C).
 */
void func_B696_b5(void)
{
    int held;

    REGS_IN;

    mff_cycles(0xB696, 12);
    RAM(BELGER_SLOT) = x;
    a = RAM(OBJ_STATE + x);
    if (a != BELGER_LEAPING) {
        mff_cycles(0xB6A0, 9);
        a = RAM(OBJ_Y + x);
        RAM(OBJ_GROUND_Y + x) = a;
    } else {
        mff_cycles(0xB69E, 1);
    }
    mff_cycles(0xB6A6, 6);
    a = RAM(OBJ_INVULN + x);
    if (a != 0) {
        mff_cycles(0xB6AB, 7);
        RAM(OBJ_INVULN + x)--;
    } else {
        mff_cycles(0xB6A9, 1);
    }
    mff_cycles(0xB6AE, 22);
    a = KIND_BELGER;
    RAM(OBJ_KIND + x) = a;
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(BOSS_TALK_ID);
    c = a >= OBJ_BOSS_TALK;
    if (a == OBJ_BOSS_TALK) {
        mff_cycles(0xB6BF, 9);
        RAM(OBJ_INVULN + x)++;
        if (RAM(OBJ_INVULN + x) != 0) {
            mff_cycles(0xB6C4, 3);
            GO_IN(0xB743, BANK_0B);
        }
        mff_cycles(0xB6C2, 1);
    } else {
        mff_cycles(0xB6BD, 1);
    }

    /* hit: the hurt phase */
    mff_cycles(0xB6C7, 6);
    a = RAM(OBJ_HIT_TIMER + x);
    if (a != 0) {
        mff_cycles(0xB6CC, 10);
        a = ANIM_WAIT;
        y = CHR_BELGER_38;
        CALL_FIXED(0xB6D0, func_F449);          /* obj_set_anim, R4 = Y */
        mff_cycles(0xB6D3, 7);
        a = BELGER_HURT;
        RAM(OBJ_STATE + x) = a;
    } else {
        mff_cycles(0xB6CA, 1);
    }

    /* held by the player */
    mff_cycles(0xB6D8, 6);
    a = RAM(OBJ_HELD + x);
    if (a != 0) {
        mff_cycles(0xB6DD, 7);
        a = RAM(GAME_ACTION);
        held = 0;
        if (a == ACT_GRAPPLE_HIT) {
            mff_cycles(0xB6E1, 1);
            held = 1;
        } else {
            mff_cycles(0xB6E3, 4);
            if (a == ACT_GRAB)
                held = 1;
            else
                mff_cycles(0xB6E5, 2);
        }
        if (held) {
            mff_cycles(0xB6E7, 5);
            if (x == RAM(GRAB_SLOT)) {
                mff_cycles(0xB6EB, 8);
                a = RAM(OBJ_LIFE + x);
                c = a >= LIFE_HOLD_MIN;
                if (c) {
                    mff_cycles(0xB6F2, 7);
                    a = RAM(GAME_ACTION);
                    c = a >= ACT_GRAPPLE_HIT;
                    if (a == ACT_GRAPPLE_HIT) {         /* never executed */
                        mff_cycles(0xB6F8, 10);
                        a = ANIM_HURT;
                        y = CHR_BELGER_3A;
                        CALL_FIXED(0xB6FC, func_F449);
                        mff_cycles(0xB6FF, 10);
                        a = BELGER_RESTART;
                        RAM(OBJ_STATE + x) = a;
                        GO_FIXED(0xF343, func_F343);    /* obj_face_player */
                    }
                    mff_cycles(0xB6F6, 2);
                    mff_cycles(0xB707, 11);
                    a = 0x00;
                    RAM(COMBO_TIMER) = a;
                    CALL_FIXED(0xB70B, func_F343);      /* obj_face_player */
                    mff_cycles(0xB70E, 12);
                    a = BELGER_RESTART;
                    RAM(OBJ_STATE + x) = a;
                    a = RAM(GRAB_TIMER);
                    if (a & 0x80) {
                        mff_cycles(0xB715, 1);
                    } else {
                        mff_cycles(0xB717, 4);
                        c = a >= GRAB_HOLD_MAX;
                        if (!c) {
                            mff_cycles(0xB719, 1);
                        } else {
                            mff_cycles(0xB71B, 7);
                            a = GRAB_HOLD_MAX;
                            RAM(GRAB_TIMER) = a;
                            mff_cycles(0xB71F, 1);
                        }
                    }
                    SYNC_OUT;
                    func_B743_b5();
                    return;
                }
                /* weak: he breaks free (never executed) */
                mff_cycles(0xB6F0, 2);
                mff_cycles(0xB721, 30);
                a = 0x00;
                RAM(GAME_ACTION) = a;
                RAM(OBJ_HIT_TIMER + x) = a;
                RAM(OBJ_HELD + x) = a;
                RAM(GRAB_TIMER) = a;
                RAM(GRAB_TIMER)--;
                y = RAM(PLAYER_ID);
                a = prg_read((uint16_t)(ESCAPE_PLAYER_CHR + y));
                SET_NZ(a);
                mff_cycles(0xB734, 6);
                CALL_FIXED(0xB734, func_F440);          /* sprite bank R2 = A */
                mff_cycles(0xB737, 16);
                a = 0x20;
                RAM(OBJ_AI_TIMER + x) = a;
                a = BELGER_DIZZY;
                RAM(OBJ_STATE + x) = a;
                mff_cycles(0xB741, 1);
            } else {
                mff_cycles(0xB6E9, 2);
            }
        }
    } else {
        mff_cycles(0xB6DB, 2);
    }

    /* the change to the second form */
    mff_cycles(0xB74A, 8);
    a = RAM(OBJ_LIFE + x);
    c = a >= LIFE_SECOND_FORM;
    if (!c) {
        mff_cycles(0xB751, 6);
        a = RAM(OBJ_TIMER2 + x);
        if (a == 0) {
            mff_cycles(0xB756, 6);
            a = RAM(OBJ_INVULN + x);
            if (a == 0) {                               /* never executed */
                mff_cycles(0xB75B, 46);
                a = RAM(OBJ_Y + x);
                RAM(OBJ_LAND_Y + x) = a;
                a = KNOCK_VY_FRAC;
                RAM(OBJ_VY_FRAC + x) = a;
                a = KNOCK_VY;
                RAM(OBJ_VY + x) = a;
                a = RAM(OBJ_ID + x);
                RAM(OBJ_SAVED_ID + x) = a;
                a = OBJ_KNOCKED_DOWN;
                RAM(OBJ_ID + x) = a;
                a = 0xFF;
                RAM(OBJ_INVULN + x) = a;
            } else {
                mff_cycles(0xB759, 1);
            }
            mff_cycles(0xB77B, 20);
            a = BELGER_RESTART;
            RAM(OBJ_STATE + x) = a;
            RAM(OBJ_SAVED_STATE + x) = a;
            a = MUSIC_CHANGE;
            CALL_FIXED(0xB785, func_F34A);              /* sound_queue_push (music) */
            mff_cycles(0xB788, 13);
            RAM(OBJ_TIMER2 + x)++;
            RTS_RETURN();
        }
        mff_cycles(0xB754, 1);
    } else {
        mff_cycles(0xB74F, 1);
    }
    mff_cycles(0xB78C, 8);
    a = RAM(OBJ_TIMER2 + x);
    if (a == 0x01) {
        /* wait for the player to stand, punch or use the special */
        mff_cycles(0xB793, 5);
        a = RAM(GAME_ACTION);
        if (a != ACT_IDLE) {
            mff_cycles(0xB797, 4);
            if (a != ACT_PUNCH) {
                mff_cycles(0xB79B, 4);
                if (a != ACT_SPECIAL) {
                    mff_cycles(0xB79F, 13);
                    a = 0x01;
                    RAM(OBJ_INVULN + x) = a;
                    RTS_RETURN();
                }
                mff_cycles(0xB79D, 1);
            } else {
                mff_cycles(0xB799, 1);
            }
        } else {
            mff_cycles(0xB795, 1);
        }
        mff_cycles(0xB7A5, 37);
        a = TALK_PAGE_CHANGE;
        RAM(SPECIAL_SCREEN_ID) = a;
        a = BELGER_RESTART;
        RAM(OBJ_STATE + x) = a;
        RAM(OBJ_SAVED_STATE + x) = a;
        RAM(OBJ_TIMER2 + x)++;
        RAM(OBJ_TIER + x)++;
        RTS_RETURN();
    }
    mff_cycles(0xB791, 1);

    /* y kept in $82-$AE */
    mff_cycles(0xB7B8, 8);
    a = RAM(OBJ_STATE + x);
    c = a >= BELGER_LEAPING;
    if (a != BELGER_LEAPING) {
        mff_cycles(0xB7BF, 8);
        a = RAM(OBJ_Y + x);
        if (a < BELGER_Y_TOP) {                         /* never executed */
            mff_cycles(0xB7C6, 7);
            a = BELGER_Y_TOP;
            RAM(OBJ_Y + x) = a;
        } else {
            mff_cycles(0xB7C4, 1);
        }
        mff_cycles(0xB7CB, 8);
        a = RAM(OBJ_Y + x);
        c = a >= BELGER_Y_BOTTOM;
        if (c) {
            mff_cycles(0xB7D2, 7);
            a = BELGER_Y_BOTTOM;
            RAM(OBJ_Y + x) = a;
        } else {
            mff_cycles(0xB7D0, 1);
        }
    } else {
        mff_cycles(0xB7BD, 1);
    }

    /* the phase */
    mff_cycles(0xB7D7, 6);
    a = RAM(OBJ_TIER + x);
    if (a == 0) {
        SYNC_OUT;
        obj_phase_jump(0xB7DC, PHASES1_LO, PHASES1_HI);
        return;
    }
    mff_cycles(0xB7DA, 1);
    mff_cycles(0xB7EF, 7);
    a = MUSIC_SECOND_FORM;
    c = a >= RAM(MUSIC_CURRENT);
    if (a != RAM(MUSIC_CURRENT)) {
        mff_cycles(0xB7F5, 6);
        CALL_FIXED(0xB7F5, func_F34A);                  /* sound_queue_push (music) */
    } else {
        mff_cycles(0xB7F3, 1);
    }
    SYNC_OUT;
    obj_phase_jump(0xB7F8, PHASES2_LO, PHASES2_HI);
}

/*
 * $B743 (bank $0B): standing, animation $BC (sprite bank $38); JMP
 * obj_set_anim.
 * Callers: JMP at 0B:B6C4 (the boss talk); the branches of the grab from
 * 0B:B715, B719, B71F.
 */
void func_B743_b5(void)
{
    REGS_IN;

    mff_cycles(0xB743, 7);
    a = ANIM_WAIT;
    y = CHR_BELGER_38;
    GO_FIXED(0xF449, func_F449);
}

/*
 * belger_start, $B80B (bank $0B; name proposed): first form, phase 0:
 * faces the player, walking animation, the AI counters and obj_timer2
 * cleared, phase 3.
 */
void func_B80B_b5(void)
{
    REGS_IN;

    mff_cycles(0xB80B, 6);
    CALL_IN(0xB80B, 0xBBE1, BANK_0B);                  /* face the player */
    mff_cycles(0xB80E, 6);
    CALL_IN(0xB80E, 0xBCD9, BANK_0B);                  /* walking animation */
    mff_cycles(0xB811, 35);
    a = 0x00;
    RAM(OBJ_AI_COUNT + x) = a;
    RAM(OBJ_AI_TIMER + x) = a;
    RAM(OBJ_AI_FLAG + x) = a;
    RAM(OBJ_TIMER2 + x) = a;
    a = BELGER_PHASE3;
    RAM(OBJ_STATE + x) = a;
    RTS_RETURN();
}

/*
 * belger_approach, $B825 (bank $0B; name proposed): first form, phase 3.
 * Within $20 px across and $18 down of the player's line: phase 4 (circle).
 * Otherwise every $0A frames (obj_ai_timer) he heads straight at the
 * player (0F:FB5B's 16-way index into the speed tables, $B84B), and walks.
 */
void func_B825_b5(void)
{
    REGS_IN;

    mff_cycles(0xB825, 6);
    CALL_IN(0xB825, 0xBCD9, BANK_0B);
    mff_cycles(0xB828, 10);
    a = 0x20;
    y = 0x18;
    CALL_FIXED(0xB82C, func_FB3E);                     /* player_near */
    mff_cycles(0xB82F, 2);
    if (c) {
        mff_cycles(0xB831, 20);
        RAM(OBJ_STATE + x)++;
        a = 0x00;
        RAM(OBJ_AI_TIMER + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xB82F, 1);
    mff_cycles(0xB83A, 13);
    RAM(OBJ_AI_TIMER + x)--;
    a = RAM(OBJ_AI_TIMER + x);
    if (a & 0x80) {
        mff_cycles(0xB842, 6);
        CALL_FIXED(0xB842, func_FB5B);                 /* obj_dir_index */
        mff_cycles(0xB845, 9);
        y = a;
        a = 0x0A;
        RAM(OBJ_AI_TIMER + x) = a;
        SYNC_OUT;
        func_B84B_b5();
        return;
    }
    mff_cycles(0xB840, 1);
    mff_cycles(0xB869, 6);
    CALL_IN(0xB869, 0xBC6B, BANK_0B);
    mff_cycles(0xB86C, 3);
    GO_IN(0xBBE1, BANK_0B);
}

/*
 * belger_steer, $B84B (bank $0B; name proposed): obj_dir and the speeds
 * for the 16-way index Y ($FBF0, $BD0C-$BD3C), the walk ($BC6B), then JMP
 * $BBE1 (face the player).
 * Callers: JMP at 0B:B8C1 (belger_circle); the fall-through from 0B:B84A.
 */
void func_B84B_b5(void)
{
    REGS_IN;

    mff_cycles(0xB84B, 4);
    a = prg_read((uint16_t)(DIR_BY_INDEX + y));
    mff_cycles(0xB84E, 9);
    RAM(OBJ_DIR + x) = a;
    a = prg_read((uint16_t)(SPEED_VX_FRAC + y));
    mff_cycles(0xB854, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = prg_read((uint16_t)(SPEED_VX + y));
    mff_cycles(0xB85A, 9);
    RAM(OBJ_VX + x) = a;
    a = prg_read((uint16_t)(SPEED_VY_FRAC + y));
    mff_cycles(0xB860, 9);
    RAM(OBJ_VY_FRAC + x) = a;
    a = prg_read((uint16_t)(SPEED_VY + y));
    mff_cycles(0xB866, 11);
    RAM(OBJ_VY + x) = a;
    CALL_IN(0xB869, 0xBC6B, BANK_0B);
    mff_cycles(0xB86C, 3);
    GO_IN(0xBBE1, BANK_0B);
}

/*
 * $B8B8: the walking animation and a step the way obj_timer & $0F says
 * (JMP belger_steer).
 */
static void belger_circle_step(void)
{
    REGS_IN;

    mff_cycles(0xB8B8, 6);
    CALL_IN(0xB8B8, 0xBCD9, BANK_0B);
    mff_cycles(0xB8BB, 11);
    a = RAM(OBJ_TIMER + x) & 0x0F;
    y = a;
    GO_IN(0xB84B, BANK_0B);
}

/*
 * belger_circle, $B86F (bank $0B; name proposed): first form, phase 4. He
 * faces the player and walks around him: each time obj_ai_timer runs out
 * the 16-way index toward the player, a quarter turn on ($BD4C, or the
 * other way round $BD5C by obj_ai_flag), goes to obj_timer ($B88C).
 */
void func_B86F_b5(void)
{
    REGS_IN;

    mff_cycles(0xB86F, 6);
    CALL_IN(0xB86F, 0xBBE1, BANK_0B);
    mff_cycles(0xB872, 13);
    RAM(OBJ_AI_TIMER + x)--;
    a = RAM(OBJ_AI_TIMER + x);
    if (!(a & 0x80)) {
        mff_cycles(0xB878, 1);
        SYNC_OUT;
        belger_circle_step();
        return;
    }
    mff_cycles(0xB87A, 6);
    CALL_FIXED(0xB87A, func_FB5B);                     /* obj_dir_index */
    mff_cycles(0xB87D, 8);
    y = a;
    a = RAM(OBJ_AI_FLAG + x);
    if (a == 0) {
        mff_cycles(0xB883, 4);
        a = prg_read((uint16_t)(CIRCLE_ONE_WAY + y));
        SET_NZ(a);
        mff_cycles(0xB886, 3);
        GO_IN(0xB88C, BANK_0B);
    }
    mff_cycles(0xB881, 1);
    mff_cycles(0xB889, 4);
    a = prg_read((uint16_t)(CIRCLE_OTHER_WAY + y));
    SET_NZ(a);
    SYNC_OUT;
    func_B88C_b5();
}

/*
 * $B88C (bank $0B): obj_timer = A, the new circling index. With bit 7
 * set (he is lined up with the player across): within $1C px and 4 lines,
 * phase 5 (punch: anim $C0, obj_ai_count 0); beyond $38 px or $18 lines,
 * back to phase 4's approach (phase 3); otherwise obj_ai_timer from
 * $BD6C. Then the step ($B8B8).
 * In: A and N = the index. Callers: JMP at 0B:B886; the fall-through from
 * 0B:B88B.
 */
void func_B88C_b5(void)
{
    REGS_IN;

    mff_cycles(0xB88C, 7);
    RAM(OBJ_TIMER + x) = a;
    if (!n) {
        mff_cycles(0xB88F, 1);
        SYNC_OUT;
        belger_circle_step();
        return;
    }
    mff_cycles(0xB891, 10);
    a = 0x1C;
    y = 0x04;
    CALL_FIXED(0xB895, func_FB3E);                     /* player_near */
    mff_cycles(0xB898, 2);
    if (c) {
        mff_cycles(0xB898, 1);
        mff_cycles(0xB8C4, 21);
        a = 0x00;
        RAM(OBJ_AI_COUNT + x) = a;
        RAM(OBJ_STATE + x)++;
        a = ANIM_PUNCH;
        y = CHR_BELGER_39;
        GO_FIXED(0xF449, func_F449);
    }
    mff_cycles(0xB89A, 10);
    a = 0x38;
    y = 0x18;
    CALL_FIXED(0xB89E, func_FB3E);
    mff_cycles(0xB8A1, 2);
    if (!c) {
        mff_cycles(0xB8A3, 20);
        a = 0x00;
        RAM(OBJ_AI_TIMER + x) = a;
        RAM(OBJ_STATE + x)--;
        RTS_RETURN();
    }
    mff_cycles(0xB8A1, 1);
    mff_cycles(0xB8AC, 12);
    a = RAM(OBJ_TIMER + x) & 0x0F;
    y = a;
    a = prg_read((uint16_t)(CIRCLE_FRAMES + y));
    mff_cycles(0xB8B5, 5);
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    belger_circle_step();
}

/*
 * belger_punch, $B8D3 (bank $0B; name proposed): first form, phase 5.
 * Out of reach ($1C px, 4 lines): walking animation, phase 4. Otherwise the
 * punch ($BB0A); when it lands, sound $18 and he steps back ($0400 the way
 * he faces away from). He faces the player, stops, and unless rng is 0 the
 * circling turns the other way (obj_ai_flag ^ $FF).
 */
void func_B8D3_b5(void)
{
    REGS_IN;

    mff_cycles(0xB8D3, 10);
    a = 0x1C;
    y = 0x04;
    CALL_FIXED(0xB8D7, func_FB3E);                     /* player_near */
    mff_cycles(0xB8DA, 2);
    if (!c) {
        mff_cycles(0xB8DC, 6);
        CALL_IN(0xB8DC, 0xBCD9, BANK_0B);
        mff_cycles(0xB8DF, 13);
        RAM(OBJ_STATE + x)--;
        RTS_RETURN();
    }
    mff_cycles(0xB8DA, 1);
    mff_cycles(0xB8E3, 6);
    CALL_IN(0xB8E3, 0xBB0A, BANK_0B);                  /* the punch */
    mff_cycles(0xB8E6, 6);
    a = RAM(OBJ_HIT_PLAYER + x);
    if (a != 0) {
        mff_cycles(0xB8EB, 8);
        a = SFX_PUNCH_HIT;
        CALL_FIXED(0xB8ED, func_F34C);                 /* sound_queue_push_sfx */
        mff_cycles(0xB8F0, 34);
        a = 0x00;
        RAM(OBJ_HIT_PLAYER + x) = a;
        RAM(OBJ_VX_FRAC + x) = a;
        a = 0x04;
        RAM(OBJ_VX + x) = a;
        a = DIR_RIGHT;
        RAM(OBJ_DIR + x) = a;
        a = RAM(OBJ_FLAGS + x) & OBJF_FACE_RIGHT;
        if (a != 0) {
            mff_cycles(0xB909, 7);
            a = DIR_LEFT;
            RAM(OBJ_DIR + x) = a;
        } else {
            mff_cycles(0xB907, 1);
        }
        mff_cycles(0xB90E, 6);
        CALL_IN(0xB90E, 0xBC6B, BANK_0B);
    } else {
        mff_cycles(0xB8E9, 2);
    }
    mff_cycles(0xB911, 6);
    CALL_FIXED(0xB911, func_F343);                     /* obj_face_player */
    mff_cycles(0xB914, 17);
    a = 0x00;
    RAM(OBJ_VX_FRAC + x) = a;
    RAM(OBJ_VX + x) = a;
    a = RAM(RNG);
    if (a != 0) {
        mff_cycles(0xB920, 11);
        a = RAM(OBJ_AI_FLAG + x) ^ 0xFF;
        RAM(OBJ_AI_FLAG + x) = a;
    } else {
        mff_cycles(0xB91E, 1);
    }
    mff_cycles(0xB928, 6);
    RTS_RETURN();
}

/*
 * belger2_start, $B929 (bank $0B; name proposed): second form, phase 0:
 * faces the player, walking animation; on his page (obj_x_hi 2) phase 3,
 * else back to it (obj_dir toward page 2, phase 2; never executed).
 */
void func_B929_b5(void)
{
    REGS_IN;

    mff_cycles(0xB929, 6);
    CALL_IN(0xB929, 0xBBE1, BANK_0B);
    mff_cycles(0xB92C, 6);
    CALL_IN(0xB92C, 0xBCD9, BANK_0B);
    mff_cycles(0xB92F, 8);
    a = RAM(OBJ_X_HI + x);
    if (a != BELGER_PAGE) {                             /* never executed */
        mff_cycles(0xB936, 15);
        a = DIR_RIGHT;
        RAM(OBJ_DIR + x) = a;
        a = RAM(OBJ_X_HI + x);
        if (a != 0x01) {
            mff_cycles(0xB942, 7);
            a = DIR_LEFT;
            RAM(OBJ_DIR + x) = a;
        } else {
            mff_cycles(0xB940, 1);
        }
        mff_cycles(0xB947, 20);
        a = 0x00;
        RAM(OBJ_DOWN_TIMER + x) = a;
        a = BELGER_TO_PAGE;
        RAM(OBJ_STATE + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xB934, 1);
    mff_cycles(0xB952, 13);
    a = BELGER_PHASE3;
    RAM(OBJ_STATE + x) = a;
    RTS_RETURN();
}

/*
 * belger2_turn_away, $B958 (bank $0B; name proposed): second form, phase 3:
 * obj_dir away from the player across, obj_down_timer 0, facing the
 * player; phase 5 (walk to that edge).
 */
void func_B958_b5(void)
{
    REGS_IN;

    mff_cycles(0xB958, 6);
    CALL_IN(0xB958, 0xBCD9, BANK_0B);
    mff_cycles(0xB95B, 10);
    a = RAM(OBJ_X + x);
    if (a < RAM(OBJ_X)) {
        mff_cycles(0xB961, 1);
        mff_cycles(0xB967, 2);
        a = DIR_LEFT;
    } else {
        mff_cycles(0xB963, 4);
        a = DIR_RIGHT;
        mff_cycles(0xB965, 1);
    }
    mff_cycles(0xB969, 18);
    RAM(OBJ_DIR + x) = a;
    a = 0x00;
    RAM(OBJ_DOWN_TIMER + x) = a;
    CALL_IN(0xB971, 0xBBE1, BANK_0B);
    mff_cycles(0xB974, 20);
    RAM(OBJ_STATE + x) += 2;                    /* INC, INC: phase 5 */
    RTS_RETURN();
}

/*
 * belger2_patrol, $B97B (bank $0B; name proposed): second form, phase 4.
 * He faces the player. Not dodging yet (obj_down_timer 0) and the player
 * within $40 px and $10 lines: he dodges up or down away from the
 * player's line (obj_down_timer 1, obj_ai_count 0). Then obj_vy 3, no
 * horizontal speed, and $B9B7.
 */
void func_B97B_b5(void)
{
    REGS_IN;

    mff_cycles(0xB97B, 6);
    CALL_IN(0xB97B, 0xBBE1, BANK_0B);
    mff_cycles(0xB97E, 6);
    a = RAM(OBJ_DOWN_TIMER + x);
    if (a == 0) {
        mff_cycles(0xB983, 10);
        a = 0x40;
        y = 0x10;
        CALL_FIXED(0xB987, func_FB3E);                 /* player_near */
        mff_cycles(0xB98A, 2);
        if (c) {
            mff_cycles(0xB98C, 10);
            a = RAM(OBJ_Y + x);
            if (a < RAM(OBJ_Y)) {
                mff_cycles(0xB992, 1);
                mff_cycles(0xB998, 2);
                a = DIR_UP;
            } else {
                mff_cycles(0xB994, 4);
                a = DIR_DOWN;
                mff_cycles(0xB996, 1);
            }
            mff_cycles(0xB99A, 19);
            RAM(OBJ_DIR + x) = a;
            a = 0x01;
            RAM(OBJ_DOWN_TIMER + x) = a;
            a = 0x00;
            RAM(OBJ_AI_COUNT + x) = a;
        } else {
            mff_cycles(0xB98A, 1);
        }
    } else {
        mff_cycles(0xB981, 1);
    }
    mff_cycles(0xB9A7, 24);
    a = 0x03;
    RAM(OBJ_VY + x) = a;
    a = 0x00;
    RAM(OBJ_VY_FRAC + x) = a;
    RAM(OBJ_VX_FRAC + x) = a;
    RAM(OBJ_VX + x) = a;
    SYNC_OUT;
    func_B9B7_b5();
}

/*
 * $BA3D: the punch and leap: obj_dir the way he faces, obj_down_timer 0,
 * obj_ai_timer $24, phase 9, sound $12, anim $C0 (JMP obj_set_anim).
 */
static void belger2_punch_start(void)
{
    REGS_IN;

    mff_cycles(0xBA3D, 8);
    a = RAM(OBJ_FLAGS + x) & OBJF_FACE_RIGHT;
    if (a != 0) {
        mff_cycles(0xBA42, 1);
        mff_cycles(0xBA48, 2);
        a = DIR_RIGHT;
    } else {
        mff_cycles(0xBA44, 4);
        a = DIR_LEFT;
        mff_cycles(0xBA46, 1);
    }
    mff_cycles(0xBA4A, 34);
    RAM(OBJ_DIR + x) = a;
    a = 0x00;
    RAM(OBJ_DOWN_TIMER + x) = a;
    a = 0x24;
    RAM(OBJ_AI_TIMER + x) = a;
    a = BELGER_PUNCH2;
    RAM(OBJ_STATE + x) = a;
    a = SFX_LEAP;
    CALL_FIXED(0xBA5E, func_F34C);                     /* sound_queue_push_sfx */
    mff_cycles(0xBA61, 7);
    a = ANIM_PUNCH;
    y = CHR_BELGER_39;
    GO_FIXED(0xF449, func_F449);
}

/*
 * $BA0A: the vertical step: up (0F:F0F7) or down (0F:F0A2) with the
 * background probes; blocked (TMP16 & $50) while dodging he punches and
 * leaps ($BA3D), while patrolling he turns round.
 */
static void belger2_patrol_step(void)
{
    REGS_IN;

    mff_cycles(0xBA0A, 8);
    a = RAM(OBJ_DIR + x);
    if (a == DIR_UP) {
        mff_cycles(0xBA11, 8);
        y = 0x01;
        CALL_FIXED(0xBA13, func_F0F7);                 /* obj_step_up */
        mff_cycles(0xBA16, 7);
        a = RAM(TMP16) & PROBE_BLOCKED;
        if (a == 0) {
            mff_cycles(0xBA1A, 1);
            mff_cycles(0xBA3C, 6);
            RTS_RETURN();
        }
        mff_cycles(0xBA1C, 6);
        a = RAM(OBJ_DOWN_TIMER + x);
        if (a != 0) {
            mff_cycles(0xBA1F, 1);
            SYNC_OUT;
            belger2_punch_start();
            return;
        }
        mff_cycles(0xBA21, 13);
        a = DIR_DOWN;
        RAM(OBJ_DIR + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xBA0F, 1);
    mff_cycles(0xBA27, 8);
    y = 0x00;
    CALL_FIXED(0xBA29, func_F0A2);                     /* obj_step_down */
    mff_cycles(0xBA2C, 7);
    a = RAM(TMP16) & PROBE_BLOCKED;
    if (a != 0) {
        mff_cycles(0xBA32, 6);
        a = RAM(OBJ_DOWN_TIMER + x);
        if (a != 0) {
            mff_cycles(0xBA35, 1);
            SYNC_OUT;
            belger2_punch_start();
            return;
        }
        mff_cycles(0xBA37, 7);
        a = DIR_UP;
        RAM(OBJ_DIR + x) = a;
    } else {
        mff_cycles(0xBA30, 1);
    }
    mff_cycles(0xBA3C, 6);
    RTS_RETURN();
}

/*
 * $B9B7 (bank $0B): the rest of belger2_patrol. Dodging, or while
 * obj_ai_timer has not run out: the step ($BA0A). Then a shot: phase 6
 * (anim $BF, back to phase 4 after it), or after five shots a rest: phase
 * 7 (anim $C2, $5A frames); with the player within $68 px and $0E lines at
 * that point, half the time (rng bit 0) the punch and leap instead (never
 * executed).
 * Out: A, X, Y, C. Callers: the fall-through from 0B:B9B6 (and the scaffold
 * splits it there).
 */
void func_B9B7_b5(void)
{
    REGS_IN;

    mff_cycles(0xB9B7, 6);
    a = RAM(OBJ_DOWN_TIMER + x);
    if (a != 0) {
        mff_cycles(0xB9BA, 2);
        SYNC_OUT;
        belger2_patrol_step();
        return;
    }
    mff_cycles(0xB9BC, 9);
    RAM(OBJ_AI_TIMER + x)--;
    if (!(RAM(OBJ_AI_TIMER + x) & 0x80)) {
        mff_cycles(0xB9BF, 2);
        SYNC_OUT;
        belger2_patrol_step();
        return;
    }
    mff_cycles(0xB9C1, 8);
    a = RAM(OBJ_AI_COUNT + x);
    c = a >= BELGER_SHOTS;
    if (a != BELGER_SHOTS) {
        mff_cycles(0xB9C6, 1);
        mff_cycles(0xB9F8, 10);
        a = ANIM_SHOOT;
        y = CHR_BELGER_38;
        CALL_FIXED(0xB9FC, func_F449);
        mff_cycles(0xB9FF, 20);
        a = BELGER_PHASE4;
        RAM(OBJ_TIMER + x) = a;
        a = BELGER_SHOOT;
        RAM(OBJ_STATE + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xB9C8, 10);
    a = 0x68;
    y = 0x0E;
    CALL_FIXED(0xB9CC, func_FB3E);                     /* player_near */
    mff_cycles(0xB9CF, 2);
    if (c) {                                            /* never executed */
        mff_cycles(0xB9D1, 14);
        a = 0x00;
        RAM(OBJ_AI_COUNT + x) = a;
        a = RAM(RNG) & 0x01;
        if (a != 0) {
            mff_cycles(0xB9DA, 2);
            SYNC_OUT;
            belger2_punch_start();
            return;
        }
    } else {
        mff_cycles(0xB9CF, 1);
    }
    mff_cycles(0xB9DC, 38);
    a = 0x5A;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x00;
    RAM(OBJ_AI_COUNT + x) = a;
    a = BELGER_PHASE4;
    RAM(OBJ_TIMER + x) = a;
    a = BELGER_REST;
    RAM(OBJ_STATE + x) = a;
    a = ANIM_REST;
    y = CHR_BELGER_33;
    CALL_FIXED(0xB9F4, func_F449);
    mff_cycles(0xB9F7, 6);
    RTS_RETURN();
}

/*
 * belger2_to_edge, $BA68 (bank $0B; name proposed): second form, phase 5:
 * obj_vx 3 ($BCC8); off his page phase 0 (never executed); walks the way
 * obj_dir says until x < $18 (left) or x >= $F0 (right), then phase 4.
 */
void func_BA68_b5(void)
{
    REGS_IN;

    mff_cycles(0xBA68, 6);
    CALL_IN(0xBA68, 0xBCC8, BANK_0B);
    mff_cycles(0xBA6B, 8);
    a = RAM(OBJ_X_HI + x);
    if (a != BELGER_PAGE) {                             /* never executed */
        mff_cycles(0xBA72, 13);
        a = BELGER_RESTART;
        RAM(OBJ_STATE + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xBA70, 1);
    mff_cycles(0xBA78, 8);
    a = RAM(OBJ_DIR + x);
    if (a == DIR_LEFT) {
        mff_cycles(0xBA7F, 6);
        CALL_FIXED(0xBA7F, func_F1A1);                 /* obj_move_left */
        mff_cycles(0xBA82, 8);
        a = RAM(OBJ_X + x);
        if (a >= 0x18) {
            mff_cycles(0xBA89, 6);
            RTS_RETURN();
        }
        mff_cycles(0xBA87, 1);
    } else {
        mff_cycles(0xBA7D, 1);
        mff_cycles(0xBA8A, 6);
        CALL_FIXED(0xBA8A, func_F183);                 /* obj_move_right */
        mff_cycles(0xBA8D, 8);
        a = RAM(OBJ_X + x);
        if (a < 0xF0) {
            mff_cycles(0xBA92, 1);
            mff_cycles(0xBA99, 6);
            RTS_RETURN();
        }
    }
    mff_cycles(0xBA94, 7);
    a = BELGER_PHASE4;
    RAM(OBJ_STATE + x) = a;
    mff_cycles(0xBA99, 6);
    RTS_RETURN();
}

/*
 * belger2_shoot, $BA9A (bank $0B; name proposed): second form, phase 6.
 * On the shooting animation's frame 1 (its first draw) a shot ($BBEC) and
 * sound $2C; on frame 3 obj_ai_timer 8, one more in obj_ai_count, the
 * walking animation and the phase in obj_timer (4).
 */
void func_BA9A_b5(void)
{
    REGS_IN;

    mff_cycles(0xBA9A, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    if (a == 0x01) {
        mff_cycles(0xBAA1, 8);
        a = RAM(OBJ_ANIM_TIMER + x);
        if (a == 0x01) {
            mff_cycles(0xBAA8, 6);
            CALL_IN(0xBAA8, 0xBBEC, BANK_0B);          /* the shot */
            mff_cycles(0xBAAB, 8);
            a = SFX_SHOT;
            CALL_FIXED(0xBAAD, func_F34C);             /* sound_queue_push_sfx */
        } else {
            mff_cycles(0xBAA6, 1);
        }
    } else {
        mff_cycles(0xBA9F, 1);
    }
    mff_cycles(0xBAB0, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    if (a == 0x03) {
        mff_cycles(0xBAB7, 8);
        a = RAM(OBJ_ANIM_TIMER + x);
        if (a == 0x01) {
            mff_cycles(0xBABE, 20);
            a = 0x08;
            RAM(OBJ_AI_TIMER + x) = a;
            RAM(OBJ_AI_COUNT + x)++;
            CALL_IN(0xBAC6, 0xBCD9, BANK_0B);
            mff_cycles(0xBAC9, 9);
            a = RAM(OBJ_TIMER + x);
            RAM(OBJ_STATE + x) = a;
        } else {
            mff_cycles(0xBABC, 1);
        }
    } else {
        mff_cycles(0xBAB5, 1);
    }
    mff_cycles(0xBACF, 6);
    RTS_RETURN();
}

/*
 * belger2_rest, $BAD0 (bank $0B; name proposed): second form, phase 7: he
 * faces the player in the resting pose until obj_ai_timer runs out, then
 * obj_ai_timer 8, the walking animation and the phase in obj_timer (4).
 */
void func_BAD0_b5(void)
{
    REGS_IN;

    mff_cycles(0xBAD0, 6);
    CALL_IN(0xBAD0, 0xBBE1, BANK_0B);
    mff_cycles(0xBAD3, 9);
    RAM(OBJ_AI_TIMER + x)--;
    if (RAM(OBJ_AI_TIMER + x) == 0) {
        mff_cycles(0xBAD8, 13);
        a = 0x08;
        RAM(OBJ_AI_TIMER + x) = a;
        CALL_IN(0xBADD, 0xBCD9, BANK_0B);
        mff_cycles(0xBAE0, 15);
        a = RAM(OBJ_TIMER + x);
        RAM(OBJ_STATE + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xBAD6, 1);
    mff_cycles(0xBAE7, 6);
    RTS_RETURN();
}

/*
 * belger2_dizzy, $BAE8 (bank $0B; name proposed; never executed): second
 * form, phase 8 (the breaking free sets it): anim $C6 and obj_invuln + 1
 * each frame until obj_ai_timer runs out, then the walking animation,
 * phase 0 and $0F frames of immunity.
 */
void func_BAE8_b5(void)
{
    REGS_IN;

    mff_cycles(0xBAE8, 9);
    RAM(OBJ_AI_TIMER + x)--;
    if (RAM(OBJ_AI_TIMER + x) == 0) {
        mff_cycles(0xBAED, 6);
        CALL_IN(0xBAED, 0xBCD9, BANK_0B);
        mff_cycles(0xBAF0, 20);
        a = BELGER_RESTART;
        RAM(OBJ_STATE + x) = a;
        a = 0x0F;
        RAM(OBJ_INVULN + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xBAEB, 1);
    mff_cycles(0xBAFB, 14);
    RAM(OBJ_INVULN + x)++;
    a = ANIM_DIZZY;
    y = CHR_BELGER_3A;
    GO_FIXED(0xF449, func_F449);
}

/*
 * belger2_punch, $BB05 (bank $0B; name proposed): second form, phase 9:
 * the punch ($BB0A) until obj_ai_timer runs out, then the leap: obj_vx
 * $0580, obj_vy $01A0 (up), anim $CC, obj_ground_y = y, phase 10. The
 * leap stores A, unset since the phase jump left $BB in it, into
 * obj_vx_frac (rewritten at once) and obj_ai_timer.
 */
void func_BB05_b5(void)
{
    REGS_IN;

    mff_cycles(0xBB05, 9);
    RAM(OBJ_AI_TIMER + x)--;
    if (RAM(OBJ_AI_TIMER + x) != 0) {
        SYNC_OUT;
        func_BB0A_b5();
        return;
    }
    mff_cycles(0xBB08, 1);
    mff_cycles(0xBB28, 48);
    RAM(OBJ_VX_FRAC + x) = a;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x05;
    RAM(OBJ_VX + x) = a;
    a = 0x80;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0x01;
    RAM(OBJ_VY + x) = a;
    a = 0xA0;
    RAM(OBJ_VY_FRAC + x) = a;
    a = ANIM_LEAP;
    y = CHR_BELGER_39;
    CALL_FIXED(0xBB46, func_F449);
    mff_cycles(0xBB49, 22);
    a = RAM(OBJ_Y + x);
    RAM(OBJ_GROUND_Y + x) = a;
    RAM(OBJ_STATE + x)++;
    RTS_RETURN();
}

/*
 * belger_punch_frame, $BB0A (bank $0B; name proposed): the punch is out
 * (obj_attack $85, damage $11) only on the first draw of the punching
 * animation's frame 3; otherwise no attack.
 * Callers: belger_punch (0B:B8E3); the fall-through from 0B:BB09.
 */
void func_BB0A_b5(void)
{
    REGS_IN;

    mff_cycles(0xBB0A, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    if (a == 0x03) {
        mff_cycles(0xBB11, 6);
        a = RAM(OBJ_ANIM_TIMER + x);
        if (a == 0) {
            mff_cycles(0xBB16, 16);
            a = PUNCH_ATTACK;
            RAM(OBJ_ATTACK + x) = a;
            a = PUNCH_DAMAGE;
            RAM(OBJ_ATTACK_DAMAGE + x) = a;
            mff_cycles(0xBB20, 1);              /* BNE, always taken */
            mff_cycles(0xBB27, 6);
            RTS_RETURN();
        }
        mff_cycles(0xBB14, 1);
    } else {
        mff_cycles(0xBB0F, 1);
    }
    mff_cycles(0xBB22, 7);
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    mff_cycles(0xBB27, 6);
    RTS_RETURN();
}

/*
 * belger2_leap, $BB53 (bank $0B; name proposed): second form, phase 10:
 * attack $90 (damage $34) out while he flies (0F:F1FD) across; landed on
 * obj_ground_y, the walking animation, no attack, phase 5.
 */
void func_BB53_b5(void)
{
    REGS_IN;

    mff_cycles(0xBB53, 20);
    a = LEAP_ATTACK;
    RAM(OBJ_ATTACK + x) = a;
    a = LEAP_DAMAGE;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    CALL_FIXED(0xBB5D, func_F1FD);                     /* obj_fall */
    mff_cycles(0xBB60, 10);
    a = RAM(OBJ_Y + x);
    if (a >= RAM(OBJ_GROUND_Y + x)) {
        mff_cycles(0xBB68, 15);
        a = RAM(OBJ_GROUND_Y + x);
        RAM(OBJ_Y + x) = a;
        CALL_IN(0xBB6E, 0xBCD9, BANK_0B);
        mff_cycles(0xBB71, 25);
        a = 0x00;
        RAM(OBJ_ATTACK + x) = a;
        RAM(OBJ_ATTACK_DAMAGE + x) = a;
        a = BELGER_PHASE5;
        RAM(OBJ_STATE + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xBB66, 1);
    mff_cycles(0xBB7F, 8);
    a = RAM(OBJ_DIR + x);
    if (a == DIR_LEFT) {
        mff_cycles(0xBB86, 3);
        GO_FIXED(0xF1A1, func_F1A1);                    /* obj_move_left */
    }
    mff_cycles(0xBB84, 1);
    mff_cycles(0xBB89, 3);
    GO_FIXED(0xF183, func_F183);                        /* obj_move_right */
}

/*
 * belger_hurt, $BB8C (bank $0B; name proposed): phase 1 of both forms (set
 * by belger while obj_hit_timer runs): he faces the player; obj_hit_timer
 * - 1, at 0 phase 0 (never executed: the hit reactions of objects_update
 * take him first); between $10 and $1B the hurt animation ($C3).
 */
void func_BB8C_b5(void)
{
    REGS_IN;

    mff_cycles(0xBB8C, 6);
    CALL_FIXED(0xBB8C, func_F343);                     /* obj_face_player */
    mff_cycles(0xBB8F, 13);
    RAM(OBJ_HIT_TIMER + x)--;
    a = RAM(OBJ_HIT_TIMER + x);
    if (a == 0) {                                       /* never executed */
        mff_cycles(0xBB97, 7);
        a = BELGER_RESTART;
        RAM(OBJ_STATE + x) = a;
    } else {
        mff_cycles(0xBB95, 1);
    }
    mff_cycles(0xBB9C, 4);
    if (a < 0x1C) {
        mff_cycles(0xBBA0, 4);
        c = a >= 0x10;
        if (c) {
            mff_cycles(0xBBA4, 10);
            a = ANIM_HURT;
            y = CHR_BELGER_3A;
            CALL_FIXED(0xBBA8, func_F449);
        } else {
            mff_cycles(0xBBA2, 1);
        }
    } else {
        mff_cycles(0xBB9E, 1);
    }
    mff_cycles(0xBBAB, 6);
    RTS_RETURN();
}

/*
 * belger_to_page, $BBAC (bank $0B; name proposed; never executed): phase 2
 * of both forms: walking animation, obj_vx 3, and the walk the way obj_dir
 * says until he is on page 2 past x $17 (going right) or before $F1 (going
 * left); then phase 0.
 */
void func_BBAC_b5(void)
{
    int stop;

    REGS_IN;

    mff_cycles(0xBBAC, 6);
    CALL_IN(0xBBAC, 0xBCD9, BANK_0B);
    mff_cycles(0xBBAF, 6);
    CALL_IN(0xBBAF, 0xBCC8, BANK_0B);
    mff_cycles(0xBBB2, 8);
    a = RAM(OBJ_DIR + x);
    stop = 0;
    if (a == DIR_RIGHT) {
        mff_cycles(0xBBB9, 8);
        a = RAM(OBJ_X_HI + x);
        if (a == BELGER_PAGE) {
            mff_cycles(0xBBC0, 8);
            a = RAM(OBJ_X + x);
            if (a >= 0x17) {
                mff_cycles(0xBBC5, 1);
                stop = 1;
            }
        } else {
            mff_cycles(0xBBBE, 1);
        }
        if (!stop) {
            mff_cycles(0xBBC7, 3);
            GO_FIXED(0xF183, func_F183);                /* obj_move_right */
        }
    } else {
        mff_cycles(0xBBB7, 1);
        mff_cycles(0xBBCA, 8);
        a = RAM(OBJ_X_HI + x);
        if (a == BELGER_PAGE) {
            mff_cycles(0xBBD1, 8);
            a = RAM(OBJ_X + x);
            if (a < 0xF1) {
                mff_cycles(0xBBD6, 1);
                stop = 1;
            }
        } else {
            mff_cycles(0xBBCF, 1);
        }
        if (!stop) {
            mff_cycles(0xBBD8, 3);
            GO_FIXED(0xF1A1, func_F1A1);                /* obj_move_left */
        }
    }
    mff_cycles(0xBBDB, 13);
    a = BELGER_RESTART;
    RAM(OBJ_STATE + x) = a;
    RTS_RETURN();
}

/*
 * belger_face_player, $BBE1 (bank $0B; name proposed): faces the player
 * (0F:F374's carry: left of him, JMP obj_face_right, else obj_face_left).
 * Out: as 0F:F090/F099.
 */
void func_BBE1_b5(void)
{
    REGS_IN;

    mff_cycles(0xBBE1, 6);
    CALL_FIXED(0xBBE1, func_F374);                     /* obj_dist_x */
    mff_cycles(0xBBE4, 2);
    if (!c) {
        mff_cycles(0xBBE6, 3);
        GO_FIXED(0xF099, func_F099);                    /* obj_face_left */
    }
    mff_cycles(0xBBE4, 1);
    mff_cycles(0xBBE9, 3);
    GO_FIXED(0xF090, func_F090);                        /* obj_face_right */
}

/*
 * belger_shot_spawn, $BBEC (bank $0B; name proposed): a shot in a free
 * slot ($0D..3): animation $C7, $1A px ahead of him the way he faces
 * (obj_flags $D0 and obj_dir right facing right, $90 and left otherwise;
 * $BD08/$BD09), 3 px above his y, id $9D, obj_vx $0380, obj_kind $0C,
 * attack $87 (damage $34). The x sum is kept on the stack (PHA/PLA).
 * In: X = Belger. Out: the RTS.
 * Caller: belger2_shoot (0B:BAA8).
 */
void func_BBEC_b5(void)
{
    REGS_IN;

    mff_cycles(0xBBEC, 6);
    CALL_FIXED(0xBBEC, func_F71A);                     /* find_free_slot */
    mff_cycles(0xBBEF, 2);
    if (c) {
        mff_cycles(0xBBEF, 2);
        mff_cycles(0xBC6A, 6);
        RTS_RETURN();
    }
    mff_cycles(0xBBF1, 11);
    RAM(TMP0) = y;
    a = ANIM_SHOT;
    CALL_FIXED(0xBBF5, func_F2A0);                     /* obj_spawn */
    mff_cycles(0xBBF8, 8);
    a = RAM(OBJ_FLAGS + x) & OBJF_FACE_RIGHT;
    if (a == 0) {
        mff_cycles(0xBBFF, 11);
        a = 0x90;
        RAM(OBJ_FLAGS + y) = a;
        a = DIR_LEFT;
        mff_cycles(0xBC06, 1);
    } else {
        mff_cycles(0xBBFD, 2);
        mff_cycles(0xBC08, 9);
        a = 0xD0;
        RAM(OBJ_FLAGS + y) = a;
        a = DIR_RIGHT;
    }
    mff_cycles(0xBC0F, 19);
    RAM(OBJ_DIR + y) = a;
    a &= 0x02;
    y = a;
    a = RAM(OBJ_X + x);
    c = 0;
    ADC_A(prg_read((uint16_t)(SHOT_DX + y)));
    mff_cycles(0xBC1C, 11);
    mff_push(a);
    a = RAM(OBJ_X_HI + x);
    ADC_A(prg_read((uint16_t)(SHOT_DX_HI + y)));
    mff_cycles(0xBC23, 108);
    y = RAM(TMP0);
    RAM(OBJ_X_HI + y) = a;
    a = mff_pull();
    RAM(OBJ_X + y) = a;
    a = RAM(OBJ_Y + x);
    c = 1;
    SBC_A(0x03);
    RAM(OBJ_Y + y) = a;
    a = RAM(OBJ_Y_HI + x);
    RAM(OBJ_Y_HI + y) = a;
    a = OBJ_BELGER_SHOT;
    RAM(OBJ_ID + y) = a;
    RAM(OBJ_SAVED_ID + y) = a;
    a = 0x03;
    RAM(OBJ_VX + y) = a;
    a = 0x80;
    RAM(OBJ_VX_FRAC + y) = a;
    a = 0x00;
    RAM(OBJ_LIFE + y) = a;
    RAM(OBJ_LIFE_MAX + y) = a;
    RAM(OBJ_VY + y) = a;
    RAM(OBJ_VY_FRAC + y) = a;
    a = KIND_BELGER_SHOT;
    RAM(OBJ_KIND + y) = a;
    a = SHOT_ATTACK;
    RAM(OBJ_ATTACK + y) = a;
    a = SHOT_DAMAGE;
    RAM(OBJ_ATTACK_DAMAGE + y) = a;
    mff_cycles(0xBC6A, 6);
    RTS_RETURN();
}

/*
 * belger_walk, $BC6B (bank $0B; name proposed): moves by obj_vx/obj_vy the
 * way obj_dir says. Going up he stops at y $82, going down at $AE; there
 * the speeds go back to obj_vx 3 ($BCC8) and he turns across, right or
 * left by obj_ai_flag (going up: right if set; going down: left if set),
 * skipping the down step. Then across: right if obj_dir bit 0, left if
 * bit 1.
 * Callers: belger_approach, belger_steer (0B:B869), belger_punch
 * (0B:B90E).
 */
void func_BC6B_b5(void)
{
    enum { NO_TURN, TURN_LEFT, TURN_RIGHT } turn = NO_TURN;

    REGS_IN;

    mff_cycles(0xBC6B, 8);
    a = RAM(OBJ_DIR + x) & DIR_UP;
    if (a != 0) {
        mff_cycles(0xBC72, 6);
        CALL_FIXED(0xBC72, func_F1DF);                 /* obj_move_up */
        mff_cycles(0xBC75, 8);
        a = RAM(OBJ_Y + x);
        if (a < BELGER_Y_TOP) {
            mff_cycles(0xBC7C, 13);
            a = BELGER_Y_TOP;
            RAM(OBJ_Y + x) = a;
            CALL_IN(0xBC81, 0xBCC8, BANK_0B);
            mff_cycles(0xBC84, 6);
            a = RAM(OBJ_AI_FLAG + x);
            if (a != 0) {
                mff_cycles(0xBC87, 1);
                turn = TURN_RIGHT;
            } else {
                turn = TURN_LEFT;
            }
        } else {
            mff_cycles(0xBC7A, 1);
        }
    } else {
        mff_cycles(0xBC70, 1);
    }
    if (turn == NO_TURN) {
        mff_cycles(0xBC90, 8);
        a = RAM(OBJ_DIR + x) & DIR_DOWN;
        if (a != 0) {
            mff_cycles(0xBC97, 6);
            CALL_FIXED(0xBC97, func_F1BF);             /* obj_move_down */
            mff_cycles(0xBC9A, 8);
            a = RAM(OBJ_Y + x);
            if (a >= BELGER_Y_BOTTOM) {
                mff_cycles(0xBCA1, 13);
                a = BELGER_Y_BOTTOM;
                RAM(OBJ_Y + x) = a;
                CALL_IN(0xBCA6, 0xBCC8, BANK_0B);
                mff_cycles(0xBCA9, 6);
                a = RAM(OBJ_AI_FLAG + x);
                if (a != 0) {
                    mff_cycles(0xBC89, 1);
                    turn = TURN_LEFT;
                } else {
                    turn = TURN_RIGHT;
                }
            } else {
                mff_cycles(0xBC9F, 1);
            }
        } else {
            mff_cycles(0xBC95, 1);
        }
    }
    if (turn == TURN_LEFT) {
        mff_cycles(0xBC89, 9);
        a = DIR_LEFT;
        RAM(OBJ_DIR + x) = a;
        mff_cycles(0xBC8E, 1);                  /* BNE, always taken */
    } else if (turn == TURN_RIGHT) {
        mff_cycles(0xBCAE, 7);
        a = DIR_RIGHT;
        RAM(OBJ_DIR + x) = a;
    }
    mff_cycles(0xBCB3, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    if (a != 0) {
        mff_cycles(0xBCBA, 6);
        CALL_FIXED(0xBCBA, func_F183);                 /* obj_move_right */
    } else {
        mff_cycles(0xBCB8, 1);
    }
    mff_cycles(0xBCBD, 8);
    a = RAM(OBJ_DIR + x) & DIR_LEFT;
    if (a != 0) {
        mff_cycles(0xBCC4, 6);
        CALL_FIXED(0xBCC4, func_F1A1);                 /* obj_move_left */
    } else {
        mff_cycles(0xBCC2, 1);
    }
    mff_cycles(0xBCC7, 6);
    RTS_RETURN();
}

/*
 * belger_walk_speed, $BCC8 (bank $0B; name proposed): obj_vx $0300,
 * obj_vy 0.
 */
void func_BCC8_b5(void)
{
    REGS_IN;

    mff_cycles(0xBCC8, 30);
    a = 0x03;
    RAM(OBJ_VX + x) = a;
    a = 0x00;
    RAM(OBJ_VX_FRAC + x) = a;
    RAM(OBJ_VY_FRAC + x) = a;
    RAM(OBJ_VY + x) = a;
    RTS_RETURN();
}

/*
 * belger_walk_anim, $BCD9 (bank $0B; name proposed): the walking animation
 * ($BD, sprite bank $38) unless it already runs.
 * Out: C as the compare (or obj_set_anim) leaves it.
 */
void func_BCD9_b5(void)
{
    REGS_IN;

    mff_cycles(0xBCD9, 8);
    a = ANIM_MOVE;
    c = a >= RAM(OBJ_ANIM + x);
    if (a != RAM(OBJ_ANIM + x)) {
        mff_cycles(0xBCE0, 8);
        y = CHR_BELGER_38;
        CALL_FIXED(0xBCE2, func_F449);
    } else {
        mff_cycles(0xBCDE, 1);
    }
    mff_cycles(0xBCE5, 6);
    RTS_RETURN();
}

/* ======================= bank $03 ======================= */

/*
 * belger_shot, $BF95 (bank $03; name proposed): behaviour of id $9D,
 * spawned by belger_shot_spawn. Its animation loops on frames 1-2. Going
 * right it moves (JMP obj_move_right; object_draw frees id $9D far off
 * screen, the list at $ECC3); going left it moves and is cleared once its
 * x high byte differs from scroll_x_hi.
 * In: X = slot. Out: the RTS, or the JMP's.
 * Caller: the behaviour table (id $9D, 03:B327).
 */
void func_BF95_b1(void)
{
    REGS_IN;

    mff_cycles(0xBF95, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    if (a == 0x02) {
        mff_cycles(0xBF9C, 14);
        a = 0x01;
        RAM(OBJ_ANIM_FRAME + x) = a;
        a = 0x00;
        RAM(OBJ_ANIM_TIMER + x) = a;
    } else {
        mff_cycles(0xBF9A, 1);
    }
    mff_cycles(0xBFA6, 8);
    a = RAM(OBJ_DIR + x) & DIR_RIGHT;
    if (a != 0) {
        mff_cycles(0xBFAD, 3);
        GO_FIXED(0xF183, func_F183);                    /* obj_move_right */
    }
    mff_cycles(0xBFAB, 1);
    mff_cycles(0xBFB0, 6);
    CALL_FIXED(0xBFB0, func_F1A1);                     /* obj_move_left */
    mff_cycles(0xBFB3, 9);
    a = RAM(OBJ_X_HI + x);
    c = a >= RAM(SCROLL_X_HI);
    if (a == RAM(SCROLL_X_HI)) {
        mff_cycles(0xBFBA, 6);
        RTS_RETURN();
    }
    mff_cycles(0xBFB8, 1);
    mff_cycles(0xBFBB, 3);
    GO_FIXED(0xF2BE, func_F2BE);                        /* object_clear */
}

/*
 * belger_killed, $BED2 (bank $03; name proposed; never executed): behaviour
 * of id $9F, which objects_update gives Belger when a hit empties his life
 * (0C:8211). Invulnerable ($FF). While the player is busy with an attack
 * other than 0, 6, $0B and $0C he shows the hurt pose (anim $C3) and waits;
 * otherwise he falls: game_action $0A, thrown the way the player faces
 * (obj_vx $0099, obj_vy $038E, landing on obj_ground_y), obj_kind $0B,
 * knocked down (id 5) to come back as id $9B (03:BF38), obj_ai_timer $10,
 * anim $C4.
 * In: X = slot. Out: obj_set_anim's.
 * Caller: the behaviour table (id $9F, 03:B32D).
 */
void func_BED2_b1(void)
{
    int falls;

    REGS_IN;

    mff_cycles(0xBED2, 12);
    a = 0xFF;
    RAM(OBJ_INVULN + x) = a;
    a = RAM(GAME_ACTION);
    falls = 1;
    if (a == 0) {
        mff_cycles(0xBED9, 1);
    } else {
        mff_cycles(0xBEDB, 5);
        a = RAM(ATTACK_ID);
        if (a == 0) {
            mff_cycles(0xBEDD, 1);
        } else {
            mff_cycles(0xBEDF, 4);
            c = a >= 0x06;
            if (a == 0x06) {
                mff_cycles(0xBEE1, 1);
            } else {
                mff_cycles(0xBEE3, 4);
                c = a >= 0x0B;
                if (a == 0x0B) {
                    mff_cycles(0xBEE5, 1);
                } else {
                    mff_cycles(0xBEE7, 4);
                    c = a >= 0x0C;
                    if (a != 0x0C) {
                        mff_cycles(0xBEE9, 2);
                        falls = 0;
                    }
                }
            }
        }
    }
    if (!falls) {
        mff_cycles(0xBF31, 7);
        a = ANIM_HURT;
        y = CHR_BELGER_3A;
        GO_FIXED(0xF449, func_F449);
    }
    mff_cycles(0xBEEB, 97);
    a = ACT_HELD;
    RAM(GAME_ACTION) = a;
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    RAM(OBJ_VX + x) = a;
    a = 0x99;
    RAM(OBJ_VX_FRAC + x) = a;
    a = RAM(PLAYER_DIR);
    RAM(OBJ_DIR + x) = a;
    a = RAM(OBJ_GROUND_Y + x);
    RAM(OBJ_LAND_Y + x) = a;
    a = KNOCK_VY_FRAC;
    RAM(OBJ_VY_FRAC + x) = a;
    a = KNOCK_VY;
    RAM(OBJ_VY + x) = a;
    a = KIND_BELGER;
    RAM(OBJ_KIND + x) = a;
    a = OBJ_KNOCKED_DOWN;
    RAM(OBJ_ID + x) = a;
    a = OBJ_BELGER_DOWN;
    RAM(OBJ_SAVED_ID + x) = a;
    a = BELGER_HURT;
    RAM(OBJ_STATE + x) = a;
    a = 0x10;
    RAM(OBJ_AI_TIMER + x) = a;
    a = ANIM_KILLED;
    y = CHR_BELGER_39;
    GO_FIXED(0xF449, func_F449);
}
