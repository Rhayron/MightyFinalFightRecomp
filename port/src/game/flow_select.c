/*
 * flow_select.c - the character select screen (04:9100)
 *
 * After the title (main_task, 0E:C6F8) and when continuing (0E:DF8D) the
 * game draws screen 2 and runs select_screen. UP and DOWN move the cursor
 * over the three characters (wrapping), any of A, B, SELECT or START
 * picks. The pick sets player_level from level_saved, converted when the
 * character changes to or from Haggar (whose levels are 2 apart from the
 * others').
 *
 * The cursor loop's JMP back to $9181 is a tail call by address in the
 * lifted code, which the runtime turns into a loop.
 */
#include "flow.h"

#define SELECT_CURSOR_Y     0x91F8  /* the cursor's y by character: $34, $6C, $A4 */
#define SELECT_PAL_BG       0x91FB
#define SELECT_PAL_SPRITES  0x920B
#define LEVEL_TO_HAGGAR     0x921B  /* level_saved -> Haggar's level: 0, 0, 0, 1, 2, 3 */
#define LEVEL_FROM_HAGGAR   0x9221  /* Haggar's level -> the others': 2, 3, 4, 5 */

void func_F288(void);   /* obj_set_anim: slot X, animation A */
void func_F406(void);   /* scene_frame: draws the objects, waits a frame */
void func_9181_b2(void);
void func_91BF_b2(void);
static void select_pick(void);

/*
 * select_screen (04:9100, proposed name)
 * $0F = the character before (player_id), player_id = select_player (the
 * character the screen starts on); the screen's colors, CHR R0/R1 =
 * $7C/$7E and R2 = $3C; slot 1 live at ($20, $57); player_set_anim($71)
 * for slot 0, which is put live at x $16 on its character's row; slot 1's
 * animation $6E; music $0D; then the cursor loop ($9181), which returns.
 * Callers: main_task's title_start_game (0E:C6F8) and the continue in
 * player_state_transition (0E:DF8D).
 */
void func_9100_b2(void) {
    REGS_IN;

    mff_cycles(0x9100, 11);
    a = RAM(PLAYER_ID);
    RAM(TMP15) = a;
    a = RAM(SELECT_PLAYER);
    z = a == 0;
    if (z) {
        mff_cycles(0x9106, 1);
        mff_cycles(0x910C, 5);
        a = 0;
        RAM(PLAYER_ID) = a;
    } else {
        mff_cycles(0x9108, 5);
        RAM(PLAYER_ID) = a;
        mff_cycles(0x910A, 1);              /* BNE $9110, taken */
    }

    mff_cycles(0x9110, 2);
    y = 0x0F;
    for (;;) {
        mff_cycles(0x9112, 4);
        a = prg_read((uint16_t)(SELECT_PAL_BG + y));
        mff_cycles(0x9115, 14);
        RAM(PALETTE_BUF + y) = a;
        RAM(PALETTE_BASE + y) = a;
        a = prg_read((uint16_t)(SELECT_PAL_SPRITES + y));
        mff_cycles(0x911E, 14);
        RAM(PALETTE_BUF + 16 + y) = a;
        RAM(PALETTE_BASE + 16 + y) = a;
        y--;
        if (y & 0x80) break;
        mff_cycles(0x9112, 1);
    }
    mff_cycles(0x9127, 50);
    RAM(PALETTE_REQUEST) = y;
    a = 0x7C;
    RAM(CHR_BANKS) = a;
    a = 0x7E;
    RAM(CHR_BANKS + 1) = a;
    a = 0x3C;
    RAM(CHR_BANKS + 2) = a;
    x = 1;
    a = 0x80;
    RAM(OBJ_STATE + 1) = a;
    RAM(OBJ_FLAGS + 1) = a;
    a = 0x20;
    RAM(OBJ_X + 1) = a;
    a = 0x57;
    RAM(OBJ_Y + 1) = a;
    a = 0x71;
    SET_NZ(a);
    SYNC_OUT;
    if (!mff_call_fn(0x114D, func_F294, 0xE000)) return;   /* player_set_anim */
    SYNC_IN;
    mff_cycles(0x914E, 49);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    RAM(OBJ_FLAGS) = a;
    a = 0;
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y_HI) = a;
    RAM(OBJ_ID) = a;
    RAM(OBJ_X_HI + 1) = a;
    RAM(OBJ_Y_HI + 1) = a;
    RAM(OBJ_ID + 1) = a;
    a = 0x16;
    RAM(OBJ_X) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(SELECT_CURSOR_Y + y));
    mff_cycles(0x9174, 12);
    RAM(OBJ_Y) = a;
    a = 0x6E;
    SET_NZ(a);
    SYNC_OUT;
    if (!mff_call_fn(0x117B, func_F288, 0xE000)) return;   /* obj_set_anim (slot 1) */
    SYNC_IN;
    mff_cycles(0x917C, 8);
    a = 0x0D;
    SET_NZ(a);
    SYNC_OUT;
    if (!mff_call_fn(0x1180, func_F34A, 0xE000)) return;   /* sound_queue_push */
    func_9181_b2();                         /* falls into the cursor loop */
}

/*
 * $9181: a frame of the select screen. DOWN or UP alone (joy_pressed
 * exactly $04 or $08) moves to the next or the previous character with
 * effect $2F and puts the cursor on its row; A, B, SELECT or START picks
 * (select_pick); then scene_frame and again.
 * Callers: select_screen falls into it; it JMPs back to itself.
 */
void func_9181_b2(void) {
    REGS_IN;
    int moved = 1;

    mff_cycles(0x9181, 7);
    a = RAM(JOY_PRESSED);
    if (a == PAD_DOWN) {
        mff_cycles(0x9185, 1);
        mff_cycles(0x91CD, 12);
        t = (uint8_t)(RAM(PLAYER_ID) + 1);
        RAM(PLAYER_ID) = t;
        a = RAM(PLAYER_ID);
        c = a >= 3;
        if (a != 3) {
            mff_cycles(0x91D3, 1);
        } else {
            mff_cycles(0x91D5, 7);
            a = 0;
            RAM(PLAYER_ID) = a;
            mff_cycles(0x91D9, 1);          /* BEQ $91E5, taken */
        }
    } else {
        mff_cycles(0x9187, 4);
        c = a >= PAD_UP;
        if (a == PAD_UP) {
            mff_cycles(0x9189, 1);
            mff_cycles(0x91DB, 10);
            t = (uint8_t)(RAM(PLAYER_ID) - 1);
            RAM(PLAYER_ID) = t;
            a = RAM(PLAYER_ID);
            if (a & 0x80) {
                mff_cycles(0x91E1, 5);
                a = 2;
                RAM(PLAYER_ID) = a;
            } else {
                mff_cycles(0x91DF, 1);
            }
        } else {
            mff_cycles(0x918B, 4);
            a &= PAD_A | PAD_B | PAD_SELECT | PAD_START;
            SET_NZ(a);
            if (!z) {
                SYNC_OUT;
                select_pick();
                return;
            }
            mff_cycles(0x918D, 1);
            moved = 0;
        }
    }
    if (moved) {
        mff_cycles(0x91E5, 8);
        a = 0x2F;
        SET_NZ(a);
        SYNC_OUT;
        if (!mff_call_fn(0x11E9, func_F34C, 0xE000)) return;   /* sound_queue_push_sfx */
        SYNC_IN;
        mff_cycles(0x91EA, 7);
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(SELECT_CURSOR_Y + y));
        SET_NZ(a);
        mff_cycles(0x91EF, 4);
        RAM(OBJ_Y) = a;
    }
    mff_cycles(0x91F2, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x11F4, func_F406, 0xE000)) return;   /* scene_frame */
    SYNC_IN;
    mff_cycles(0x91F5, 3);                  /* JMP $9181 */
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9181, 2); call_by_address_tail(0x9181, 2); return;
}

/*
 * $918F: the pick. Effect $30, slot 0's animation back to its first
 * frame, scene_frame, then the level in A for $91BF (Y = level_saved):
 * the same character keeps level_saved; from Haggar LEVEL_FROM_HAGGAR[Y]
 * (falling into $91BF); to Haggar LEVEL_TO_HAGGAR[Y]; between Cody and
 * Guy level_saved.
 */
static void select_pick(void) {
    REGS_IN;

    mff_cycles(0x918F, 8);
    a = SFX_START;
    SET_NZ(a);
    SYNC_OUT;
    if (!mff_call_fn(0x1193, func_F34C, 0xE000)) return;   /* sound_queue_push_sfx */
    SYNC_IN;
    mff_cycles(0x9194, 16);
    a = 0;
    SET_NZ(a);
    RAM(OBJ_ANIM_FRAME) = a;
    RAM(OBJ_ANIM_TIMER) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x119E, func_F406, 0xE000)) return;   /* scene_frame */
    SYNC_IN;
    mff_cycles(0x919F, 11);
    y = RAM(LEVEL_SAVED);
    a = RAM(TMP15);
    m = RAM(PLAYER_ID);
    c = a >= m;
    if (a != m) {
        mff_cycles(0x91A5, 1);
        mff_cycles(0x91AC, 4);
        c = a >= 2;
        if (a == 2) {
            mff_cycles(0x91AE, 1);
            mff_cycles(0x91BC, 4);
            a = prg_read((uint16_t)(LEVEL_FROM_HAGGAR + y));
            SYNC_OUT;
            func_91BF_b2();
            return;
        }
        mff_cycles(0x91B0, 7);
        a = RAM(PLAYER_ID);
        c = a >= 2;
        if (a == 2) {
            mff_cycles(0x91B6, 4);
            a = prg_read((uint16_t)(LEVEL_TO_HAGGAR + y));
            SET_NZ(a);
            mff_cycles(0x91B9, 3);          /* JMP $91BF */
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x91BF, 2); call_by_address_tail(0x91BF, 2); return;
        }
        mff_cycles(0x91A7, 1);              /* BNE $91A7 */
    }
    mff_cycles(0x91A7, 6);
    a = RAM(LEVEL_SAVED);
    SET_NZ(a);
    SYNC_OUT;                               /* JMP $91BF */
    nes_cpu_instruction_boundary(0x91BF, 2); call_by_address_tail(0x91BF, 2); return;
}

/*
 * $91BF: the end of the pick: player_level = level_saved = A,
 * select_player = player_id, 15 frames, return from select_screen.
 * Callers: the pick by JMP (04:91A9, 04:91B9) and by falling in (04:91BC).
 */
void func_91BF_b2(void) {
    REGS_IN;

    mff_cycles(0x91BF, 20);
    RAM(PLAYER_LEVEL) = a;
    RAM(LEVEL_SAVED) = a;
    a = RAM(PLAYER_ID);
    RAM(SELECT_PLAYER) = a;
    x = 0x0F;
    SET_NZ(x);
    SYNC_OUT;
    mff_call_fn(0x11CB, func_FF2A, 0xE000);                /* task_wait_frames */
    SYNC_IN;
    mff_cycles(0x91CC, 6);
    SYNC_OUT;
    mff_rts();
}
