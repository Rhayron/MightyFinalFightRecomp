/*
 * boss_b01_scene.c - id $8F, the prop of a scripted scene no recorded run
 * plays (nothing of it is in runs/cdl/map.cdl or cyc_seeds.txt).
 *
 * How it comes about, all of it never executed: the scene object, ids
 * $7B/$7D (bank $0B, 0B:BD7F; $7D is what Belger's "killed" id $9B turns
 * into at 03:BF42), runs a script per character (0B:BE23 by player_id,
 * its step in the player's obj_down_timer, $0550). Cody's branch spawns id
 * $9C in a free slot (0B:BE45); $9C (03:BFBE) takes the player's place,
 * moves the player to x 0, sets the player's animation and CHR bank,
 * changes two sprite colours ($0617 $19, $061B $21) and becomes $8F with
 * animation $FE (CHR $31). Cody's script step 1 walks him right two
 * pixels a frame (0B:BE59).
 *
 * $8F then waits for the player to come within $10 px, takes a blow - the
 * hit spark of attack $0D, sound $18 - and flies off up and to the right,
 * 3 px a frame each way, with animation $FF until it leaves the screen;
 * it sets the script's step (2 once the player is at x $70 or more, 3
 * when it is gone).
 */
#include "boss_b01.h"

#define SCENE_STEP          (OBJ_DOWN_TIMER + 0)    /* the player's slot: the script step of 0B:BE36 */
#define ATTACK_SCENE_PUNCH  0x0D
#define SFX_SCENE_PUNCH     0x18

/*
 * scene_prop_update, $A09B (name proposed): the behaviour of id $8F.
 * Phase 0 waits for the blow (0F:F374 below $10), phase 1 flies off.
 * In: X = slot. Caller: the JMP at 01:A02D. Never executed.
 */
void func_A09B_b0(void)
{
    REGS_IN;

    mff_cycles(0xA09B, 8);
    a = RAM(OBJ_STATE + x) & OBJ_PHASE;
    SET_NZ(a);
    if (z) {
        mff_cycles(0xA0A2, 6);
        CALL_FIXED(0xA0A2, func_F374);
        mff_cycles(0xA0A5, 4);
        CMP_FLAGS(a, 0x10);
        if (!c) {
            mff_cycles(0xA0A9, 16);
            a = DIR_RIGHT;
            RAM(PLAYER_DIR) = a;
            a = ATTACK_SCENE_PUNCH;
            SET_NZ(a);
            RAM(ATTACK_ID) = a;
            CALL_FIXED(0xA0B1, func_F455);      /* spawn_hit_spark */
            mff_cycles(0xA0B4, 8);
            a = SFX_SCENE_PUNCH;
            SET_NZ(a);
            CALL_FIXED(0xA0B6, func_F34C);
            mff_cycles(0xA0B9, 15);
            RAM(OBJ_STATE + x) = (uint8_t)(RAM(OBJ_STATE + x) + 1);
            a = RAM(OBJ_X);
            CMP_FLAGS(a, 0x70);
            if (c) {
                mff_cycles(0xA0C3, 6);
                t = (uint8_t)(RAM(SCENE_STEP) + 1);
                RAM(SCENE_STEP) = t;
                SET_NZ(t);
            } else {
                mff_cycles(0xA0C1, 1);
            }
        } else {
            mff_cycles(0xA0A7, 1);
        }
        mff_cycles(0xA0C6, 6);
        RTS_RETURN();
    }
    mff_cycles(0xA0A0, 1);

    mff_cycles(0xA0C7, 8);
    a = RAM(OBJ_X);
    c = a >= 0x70;
    if (c) {
        mff_cycles(0xA0CE, 6);
        a = 0x02;
        RAM(SCENE_STEP) = a;
    } else {
        mff_cycles(0xA0CC, 1);
    }
    mff_cycles(0xA0D3, 10);
    a = 0xFF;
    y = 0x31;
    SET_NZ(y);
    CALL_FIXED(0xA0D7, func_F443);
    mff_cycles(0xA0DA, 22);
    a = 0x90;                                   /* on screen, world coordinates */
    RAM(OBJ_FLAGS + x) = a;
    a = RAM(OBJ_X + x);
    c = 0;
    ADC_A(0x03);
    RAM(OBJ_X + x) = a;
    if (!c) {
        mff_cycles(0xA0EA, 13);
        a = RAM(OBJ_Y + x);
        SBC_A(0x02);                            /* C is clear: 3 up */
        SET_NZ(a);
        RAM(OBJ_Y + x) = a;
        if (c) {
            mff_cycles(0xA0F4, 6);
            RTS_RETURN();
        }
        mff_cycles(0xA0F2, 1);
    } else {
        mff_cycles(0xA0E8, 1);
    }
    mff_cycles(0xA0F5, 9);
    a = 0x03;
    SET_NZ(a);
    RAM(SCENE_STEP) = a;
    JMP_F(0xF2BE, func_F2BE);
}
