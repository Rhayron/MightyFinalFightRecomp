/*
 * stage_area.c - starting an area (docs/assets.md, "Area record")
 *
 * A round has three areas (sub_area $60, 0-2). When one starts, the code
 * at $DFE9-$E03A (player_round.c) draws the area's columns and the status bar
 * text (bank 0A $9C00), maps the round's bank and loads the area header
 * byte ($AE68, $AE98 or $AEC8 for sub_area 0, 1, 2); then area_start sets
 * the area up and enters the gameplay loop. Between rounds the round map
 * pauses the game with the raster IRQ off (round_map_pause and
 * round_map_resume) and camera_reset puts the scroll back to 0.
 */
#include "stage.h"

void func_E16B(void);   /* player_start_position */
void func_FF31(void);   /* task_wait_frame */
void func_F26E(void);   /* obj_vy for a fall from the top */
void func_F34A(void);   /* sound_queue_push */

/* the player's start position per round: pointer lo/hi to a y table and
 * to an x table, each by sub_area */
#define K_START_Y_LO    0xE18F
#define K_START_Y_HI    0xE194
#define K_START_X_LO    0xE199
#define K_START_X_HI    0xE19E

/* game_action values area_start sets for the scripted area entrances */
#define GA_ENTRANCE     0x1B    /* round 1 area 2: the door (00:963D) */
#define GA_LIGHTS_ON    0x1C    /* round 1 area 3, the first boss room (00:96EF) */
#define GA_BOSS_ROOM    0x20    /* round 2 area 3 and round 5 area 3 */

#define SOUND_BOSS      0x05    /* music for the boss areas of rounds 2-5 */

/* what area_start clears, in its order ($E0AD-$E0E3) */
static const uint16_t k_area_cleared[] = {
    GAME_ACTION, JUMP_MOVING, COMBO_TIMER, ATTACK_LIVE, UNK_A5, ACTION_TIMER, GUY_JUMP_WINDOW,
    WEAPON_HELD, WEAPON_AMMO, RESPAWN_TIMER2, RESPAWN_TIMER1, CAMERA_LOCK, AREA_CLEAR, AREA_KILLS,
    SPECIAL_SCREEN_ID, SCREEN_EVENT, UNK_B6, CAMERA_AUTOSCROLL, KEEP_ON_SCREEN, ATTACK_GROUND_Y,
    ACTION_BEFORE_FALL, GRAB_HIT_COUNT, BONUS_STAGE, SPAWN_GROUPS, SPAWN_GROUPS + 1, SPAWN_GROUPS + 2,
};

/*
 * area_start, $E03B (proposed name): A = the area header byte. Keeps its
 * top bits (area_flags) and its screen count, loads the area's graphics
 * (0E:C3DD) and fades in (0E:C362), clears the objects, puts the player in
 * slot 0 at the area's start position falling in from the top, maps banks
 * $08/$09 and sets up the stage object list ($8000, $8038) and its first
 * spawns ($8080). The last area of a round is a boss area (boss_area $FF,
 * with music $05 from round 2 on); the per-area state is cleared; the
 * scripted entrances get their game_action; then the gameplay loop.
 * Called by: the area setup at $DFE9-$E03A (JMP at $E02F and $E035; for
 * sub_area 2 it falls in at $E03B), func_DF50 and func_DFE9 in
 * player_round.c, which carry their own copy of this code (the scaffold
 * made one function of each JMP target, and $DFE9 runs on into $E03B).
 */
void func_E03B(void) {
    REGS_IN;
    unsigned i;

    mff_cycles(0xE03B, 31);
    mff_push(a);
    RAM(AREA_FLAGS) = a & 0xE0;
    a = mff_pull();
    RAM(AREA_SCREEN_COUNT) = a & 0x1F;
    a = 0x00;
    SET_NZ(a);
    RAM(AREA_PALETTE) = a;
    RAM(AREA_SCREEN) = a;
    SYNC_OUT;
    if (!mff_call_at(0x004D, 0xC3DD, -1)) return;
    SYNC_IN;
    mff_cycles(0xE04E, 11);
    a = 0x00;
    SET_NZ(a);
    RAM(PALETTE_REQUEST) = a;
    SYNC_OUT;
    mff_call_fn(0x0054, func_FF31, 0xE000);     /* a yield: no balance check */
    SYNC_IN;
    mff_cycles(0xE055, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0057, 0xC362, -1)) return;
    SYNC_IN;
    mff_cycles(0xE058, 6);
    SYNC_OUT;
    if (!mff_call_at(0x005A, 0xC334, -1)) return;
    SYNC_IN;

    /* the player: slot 0, on the screen, world coordinates, facing right */
    mff_cycles(0xE05B, 37);
    x = 0x00;
    RAM(OBJ_FLAGS) = OBJF_ON_SCREEN | OBJF_FACE_RIGHT | OBJF_WORLD;
    RAM(OBJ_STATE) = 0x80;
    RAM(OBJ_Y_HI) = 0x00;
    RAM(OBJ_ID) = 0x00;
    a = RAM(SCROLL_X_HI);
    SET_NZ(a);
    RAM(OBJ_X_HI) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0076, func_F26E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE077, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0079, 0xC8B4, -1)) return;   /* the player's palette */
    SYNC_IN;
    mff_cycles(0xE07A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x007C, func_E16B, 0xE000)) return;
    SYNC_IN;

    /* the stage list of banks $08/$09 */
    mff_cycles(0xE07D, 16);
    x = 0x08;
    RAM(BANK_REQUEST) = x;
    x++;
    SET_NZ(x);
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0086, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE087, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0089, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xE08A, 6);
    SYNC_OUT;
    if (!mff_call_at(0x008C, 0x8038, -1)) return;
    SYNC_IN;
    mff_cycles(0xE08D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x008F, 0x8080, -1)) return;
    SYNC_IN;

    /* the boss area */
    mff_cycles(0xE090, 7);
    a = RAM(SUB_AREA);
    c = a >= 2;
    if (a == 2) {
        mff_cycles(0xE096, 5);
        if (RAM(ROUND) != 0) {
            mff_cycles(0xE09A, 8);
            a = SOUND_BOSS;
            SET_NZ(a);
            SYNC_OUT;
            if (!mff_call_fn(0x009E, func_F34A, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xE098, 1);
        }
        mff_cycles(0xE09F, 4 + 1);              /* LDA #$FF, BNE taken */
        a = 0xFF;
    } else {
        mff_cycles(0xE094, 1 + 2);
        a = 0x00;
    }

    /* the per-area state */
    mff_cycles(0xE0A5, 99);
    RAM(BOSS_AREA) = a;
    RAM(PLAYER_DIR) = DIR_RIGHT;
    RAM(JUMP_DIR) = DIR_RIGHT;
    for (i = 0; i < sizeof k_area_cleared / sizeof k_area_cleared[0]; i++)
        RAM(k_area_cleared[i]) = 0x00;

    /* scripted entrances */
    a = RAM(ROUND);
    if (a == 0) {
        mff_cycles(0xE0EA, 7);
        a = RAM(SUB_AREA) & 0x01;
        if (a != 0) {
            /* area 2: through the door, behind the background */
            mff_cycles(0xE0F0, 17 + 2);         /* BNE across a page */
            RAM(GAME_ACTION) = GA_ENTRANCE;
            a = RAM(OBJ_FLAGS) | OBJF_BEHIND;
            SET_NZ(a);
            RAM(OBJ_FLAGS) = a;
        } else {
            mff_cycles(0xE0EE, 1 + 7);
            a = RAM(SUB_AREA);
            SET_CMP(a, 2);
            if (!z) {
                mff_cycles(0xE102, 1);
            } else {
                /* area 3: the lights come on */
                mff_cycles(0xE104, 7 + 1);
                a = GA_LIGHTS_ON;
                SET_NZ(a);
                RAM(GAME_ACTION) = a;
            }
        }
    } else {
        mff_cycles(0xE0E8, 2 + 7);              /* BNE across a page */
        a = RAM(ROUND);
        if (a == 1) {
            mff_cycles(0xE110, 7);
            a = RAM(SUB_AREA);
            SET_CMP(a, 2);
            if (!z) {
                mff_cycles(0xE114, 1);
            } else {
                mff_cycles(0xE116, 7 + 1);
                a = GA_BOSS_ROOM;
                SET_NZ(a);
                RAM(GAME_ACTION) = a;
            }
        } else {
            mff_cycles(0xE10E, 1 + 7);
            a = RAM(ROUND);
            SET_CMP(a, 4);
            if (z) {
                mff_cycles(0xE122, 7);
                a = RAM(SUB_AREA);
                SET_CMP(a, 2);
                if (z) {
                    mff_cycles(0xE128, 5);
                    a = GA_BOSS_ROOM;
                    SET_NZ(a);
                    RAM(GAME_ACTION) = a;
                } else {
                    mff_cycles(0xE126, 1);
                }
            } else {
                mff_cycles(0xE120, 1);
            }
        }
    }

    /* JMP gameplay */
    mff_cycles(0xE12C, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC8ED, 2);
    call_by_address_tail(0xC8ED, -1);
}

/*
 * round_map_pause, $E12F (proposed name): paused = $FF, interrupts on, the
 * raster IRQ off (irq_enable 0) for the round map. Out: A = 0.
 * Called by: the round transitions at 0E:DF77 and 0E:DFAC.
 */
void func_E12F(void) {
    REGS_IN;

    mff_cycles(0xE12F, 18);
    RAM(PAUSED) = 0xFF;
    g_cpu.I = 0;
    a = 0x00;
    SET_NZ(a);
    RAM(IRQ_ENABLE) = a;
    SYNC_OUT;
    mff_rts();
}

/*
 * round_map_resume, $E139 (proposed name): interrupts on, the raster IRQ
 * on, paused = 0. Out: A = 0.
 * Called by: the round transitions at 0E:DF9B and 0E:DFDC.
 */
void func_E139(void) {
    REGS_IN;

    mff_cycles(0xE139, 18);
    g_cpu.I = 0;
    RAM(IRQ_ENABLE) = 0x01;
    a = 0x00;
    SET_NZ(a);
    RAM(PAUSED) = a;
    SYNC_OUT;
    mff_rts();
}

/*
 * camera_reset, $E143 (proposed name): the scroll back to 0 (x, y, page,
 * nametable), render_off_depth 0, and the player, the column writer
 * (scroll_mode) and the camera facing right. Out: A = 1.
 * Called by: the round transitions at 0E:DF98 and 0E:DFD9.
 */
void func_E143(void) {
    REGS_IN;

    mff_cycles(0xE143, 40);
    a = 0x00;
    RAM(SCROLL_X) = a;
    RAM(SCROLL_Y) = a;
    RAM(SCROLL_NT) = a;
    RAM(SCROLL_Y_HI) = a;
    RAM(SCROLL_X_HI) = a;
    RAM(RENDER_OFF_DEPTH) = a;
    a = DIR_RIGHT;
    SET_NZ(a);
    RAM(PLAYER_DIR) = a;
    RAM(SCROLL_MODE) = a;
    RAM(JUMP_DIR) = a;
    RAM(CAMERA_DIR) = a;
    SYNC_OUT;
    mff_rts();
}

/*
 * player_start_position, $E16B (proposed name): obj_y and obj_x of the
 * player (slot 0) from the round's start tables, by sub_area.
 * Out: $04-$07 = the round's table pointers, Y = sub_area, A = obj_x.
 * Called by: area_start, JSR at $E07A (also in the copies of area_start in
 * player_round.c's func_DF50 and func_DFE9).
 */
void func_E16B(void) {
    REGS_IN;

    mff_cycles(0xE16B, 7);
    y = RAM(ROUND);
    a = prg_read((uint16_t)(K_START_Y_LO + y));
    mff_cycles(0xE170, 7);
    RAM(TMP4) = a;
    a = prg_read((uint16_t)(K_START_Y_HI + y));
    mff_cycles(0xE175, 7);
    RAM(TMP5) = a;
    a = prg_read((uint16_t)(K_START_X_LO + y));
    mff_cycles(0xE17A, 7);
    RAM(TMP6) = a;
    a = prg_read((uint16_t)(K_START_X_HI + y));
    mff_cycles(0xE17F, 13);
    RAM(TMP7) = a;
    y = RAM(SUB_AREA);
    a = mem_read((uint16_t)(RAM16(TMP4) + y));
    mff_cycles(0xE186, 9);
    RAM(OBJ_Y) = a;
    a = mem_read((uint16_t)(RAM16(TMP6) + y));
    SET_NZ(a);
    mff_cycles(0xE18B, 10);
    RAM(OBJ_X) = a;
    SYNC_OUT;
    mff_rts();
}
