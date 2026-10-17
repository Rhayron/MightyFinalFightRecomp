/*
 * obj_anim.c - starting an object, its animation and sprite banks, the
 * free-slot searches, and scene_frame, which draws the objects for a scene
 * outside gameplay (docs/objects.md, "Spawning" and "Drawing").
 *
 * An animation restarts with obj_anim_frame and obj_anim_timer 0
 * (object_draw advances them). The sprite pattern banks are the MMC3's
 * 1 KiB CHR registers R2-R5 ($1000-$1FFF), shadowed in chr_banks+2..5 and
 * written by the nmi: R2 holds the player's art, R3-R5 whatever the
 * behaviours load next to an animation.
 */
#include "obj_fixed.h"

#define OBJ_SLOT_SEARCH_END     0x02    /* find_free_slot stops before slot 2 */
#define OBJ_SLOT_EFFECTS_END    0x0D    /* find_free_effect_slot: $0F and $0E */

/*
 * obj_set_anim, $F288: slot X shows animation A from its first frame.
 * In: A = animation, X = slot. Out: A = 0; X, Y, C kept.
 * Callers: about 30 sites in banks 01, 04, 05, 08, 0B, 0C, 0D, 0E, and the
 * sprite bank setters below.
 */
void func_F288(void)
{
    REGS_IN;

    mff_cycles(0xF288, 23);
    RAM(OBJ_ANIM + x) = a;
    a = 0x00;
    RAM(OBJ_ANIM_FRAME + x) = a;
    RAM(OBJ_ANIM_TIMER + x) = a;
    RTS_RETURN();
}

/*
 * player_set_anim, $F294: obj_set_anim for slot 0.
 * In: A = animation. Out: A = 0, N clear, Z set; X, Y, C kept.
 * Callers: about 100 sites (the player states of banks 00 and 0E, the
 * scenes of banks 03, 04, 05, 09, player_take_hit at 0F:F939, F9C0).
 */
void func_F294(void)
{
    REGS_IN;

    mff_cycles(0xF294, 20);
    RAM(OBJ_ANIM) = a;
    a = 0x00;
    SET_NZ(a);
    RAM(OBJ_ANIM_FRAME) = a;
    RAM(OBJ_ANIM_TIMER) = a;
    RTS_RETURN();
}

/*
 * obj_spawn, $F2A0: slot Y comes alive with animation A: frame 0, the
 * animation timer cleared but for its blink bit, obj_flags = world
 * coordinates only ($10), obj_state and obj_saved_state $80. The caller
 * fills in the rest.
 * In: A = animation, Y = slot. Out: X, Y, C kept.
 * Callers: 00:81E3, 826D, 92B1, 03:BC56, BE3E, BE6F, 04:8E1A, 8E98,
 * 05:ACC2, AD24, 08:897B, 91AA, 9322, 0B:B4B4, BBF5, 0D:A62E,
 * spawn_hit_spark (0F:F465).
 */
void func_F2A0(void)
{
    REGS_IN;

    mff_cycles(0xF2A0, 48);
    RAM(OBJ_ANIM + y) = a;
    RAM(OBJ_ANIM_FRAME + y) = 0x00;
    RAM(OBJ_ANIM_TIMER + y) &= 0x80;
    RAM(OBJ_FLAGS + y) = OBJF_WORLD;
    a = OBJ_LIVE;
    RAM(OBJ_STATE + y) = a;
    RAM(OBJ_SAVED_STATE + y) = a;
    RTS_RETURN();
}

/*
 * scene_frame, $F406: one frame of a scene that runs outside the gameplay
 * loop (select screen, round map, ending...): with draw_busy raised (the
 * nmi skips its work meanwhile) the OAM buffer restarts at 4, the rest of
 * it is hidden (sprites_hide_rest, 0E:C31D) and the objects drawn
 * (objects_draw); then draw_busy 0, task_wait_frame, and the PRG banks
 * mapped before (prg_banks, kept on the stack) are requested and mapped
 * again: JMP bank_switch.
 * Out: as bank_switch.
 * Callers: 00:9A5A, 04:8E50, 8ECE, 8F12, 919C, 91F2, 93C5, 955A, 09:A754,
 * AB77, AC01, AC75, AC7B, ACB1, ACDB, AD0B, AD11, and a JMP from 09:ABCA.
 */
void func_F406(void)
{
    REGS_IN;

    mff_cycles(0xF406, 28);
    mff_push(RAM(PRG_BANKS));
    mff_push(RAM(PRG_BANKS + 1));
    RAM(DRAW_BUSY)++;
    a = 0x04;
    SET_NZ(a);
    RAM(OAM_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1414, 0xC31D, -1)) return;       /* sprites_hide_rest */
    SYNC_IN;
    mff_cycles(0xF415, 6);
    CALL_FIXED(0xF415, func_E9BC);                      /* objects_draw */
    mff_cycles(0xF418, 11);
    RAM(DRAW_BUSY) = 0x00;
    SYNC_OUT;
    mff_call_fn(0x141E, func_FF31, 0xE000);             /* task_wait_frame (a yield) */
    SYNC_IN;
    mff_cycles(0xF41F, 17);
    RAM(BANK_REQUEST + 1) = mff_pull();
    a = mff_pull();
    SET_NZ(a);
    RAM(BANK_REQUEST) = a;
    TAIL_FIXED(0xFF4D, func_FF4D);
}

/*
 * set_chr_bank2, $F440: chr_banks+2 = A, the sprite bank of the player's
 * art (R2).
 * In: A = bank. Out: everything kept.
 * Callers: about 100 sites, mostly next to player_set_anim.
 */
void func_F440(void)
{
    REGS_IN;

    mff_cycles(0xF440, 9);
    RAM(CHR_BANKS + 2) = a;
    RTS_RETURN();
}

/*
 * $F443 / $F449 / $F44F: obj_set_anim, then chr_banks+3/+4/+5 = Y (sprite
 * bank R3, R4 or R5 for the object's art). `pc` is the entry, `reg` the
 * chr_banks index.
 */
static void set_anim_chr(uint16_t pc, int reg)
{
    REGS_IN;

    mff_cycles(pc, 6);
    CALL_FIXED(pc, func_F288);
    mff_cycles((uint16_t)(pc + 3), 9);
    RAM(CHR_BANKS + reg) = y;
    RTS_RETURN();
}

/*
 * obj_set_anim_chr3, $F443 (name proposed): obj_set_anim and sprite bank
 * R3 = Y.
 * In: A = animation, X = slot, Y = bank. Out: A = 0; X, Y, C kept.
 * Callers: 01:A0D7, 03 (11 sites, and JMPs), 05:AB5F, AB82, ABA9, 0C:9DFC,
 * 0D:A811, A837.
 */
void func_F443(void)
{
    set_anim_chr(0xF443, 3);
}

/*
 * obj_set_anim_chr4, $F449 (name proposed): obj_set_anim and sprite bank
 * R4 = Y.
 * Callers: banks 01, 0B, 0D (12 sites, 7 JMPs).
 */
void func_F449(void)
{
    set_anim_chr(0xF449, 4);
}

/*
 * obj_set_anim_chr5, $F44F (name proposed): obj_set_anim and sprite bank
 * R5 = Y.
 * Callers: 01:A4CD, 05:AB33, 0D:A59C, A692, JMPs from 01:AE2E, 0D:A4C3.
 */
void func_F44F(void)
{
    set_anim_chr(0xF44F, 5);
}

/*
 * find_free_slot, $F71A: the highest free slot from $0D down to 3 (the
 * enemies' slots), obj_state bit 7 clear.
 * Out: Y = the slot, C clear; C set when all are taken (never executed).
 * Callers: 03:BC09, BDDF, 05:AC7D, ACE1, 08:8969, 9056 (the timed
 * spawner), 921C, 0B:B46F, BBEC, BE45, BFAF, 0D:A5DA.
 */
void func_F71A(void)
{
    REGS_IN;

    mff_cycles(0xF71A, 2);
    y = OBJ_SLOT_ENEMY_LAST;
    for (;;) {
        mff_cycles(0xF71C, 6);
        if (!(RAM(OBJ_STATE + y) & OBJ_LIVE)) {
            mff_cycles(0xF71F, 1);
            mff_cycles(0xF728, 8);
            c = 0;
            RTS_RETURN();
        }
        mff_cycles(0xF721, 6);
        y--;
        if (y == OBJ_SLOT_SEARCH_END)
            break;
        mff_cycles(0xF71C, 1);
    }
    mff_cycles(0xF726, 8);                      /* never executed */
    c = 1;
    RTS_RETURN();
}

/*
 * find_free_effect_slot, $F72A: a free slot among $0F and $0E (the hit
 * sparks').
 * Out: Y = the slot, C clear; C set when both are taken.
 * Caller: spawn_hit_spark (0F:F45C).
 */
void func_F72A(void)
{
    REGS_IN;

    mff_cycles(0xF72A, 2);
    y = OBJ_SLOT_LAST;
    for (;;) {
        mff_cycles(0xF72C, 6);
        if (!(RAM(OBJ_STATE + y) & OBJ_LIVE)) {
            mff_cycles(0xF728, 1);
            mff_cycles(0xF728, 8);
            c = 0;
            RTS_RETURN();
        }
        mff_cycles(0xF731, 6);
        y--;
        if (y == OBJ_SLOT_EFFECTS_END)
            break;
        mff_cycles(0xF72C, 1);
    }
    mff_cycles(0xF736, 8);
    c = 1;
    RTS_RETURN();
}
