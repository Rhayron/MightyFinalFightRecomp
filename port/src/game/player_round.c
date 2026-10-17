/*
 * player_round.c - between areas and rounds: player_state_transition
 * (game_action 7 after a continue, $0E when an area is over), the next
 * round's map or the select screen, and the start of an area. They run as
 * the player's state but drive the whole game: screens, banks, music and
 * the player's reset. Each ends by JMP to gameplay ($C8ED).
 */
#include "player_defs.h"

void func_E03B(void);   /* the area header byte A: area_flags, area_screen_count... */
void func_E12F(void);
void func_E139(void);
void func_E143(void);
void func_E16B(void);
void func_E521(void);   /* the background column writer */

/* per round (round 0-4) */
#define ROUND_MAP_FUNCTION  0xE15C   /* 08 09 0A 0B 0C: bank $04 function of the round map */
#define ROUND_FIRST_AREA    0xE161   /* 00 03 06 09 0C: area_index of its first area */
#define ROUND_TABLES_INDEX  0xE166   /* 00 01 02 03 04: round_tables */
#define ROUND_MUSIC         0xC83C   /* 00 01 02 03 04 */
#define ROUND_BANK          0xC1F8   /* 01 05 07 09 0D: mapped at $A000 */
/* the area header in the round's bank, by sub_area */
#define AREA_HEADER_0       0xAE68
#define AREA_HEADER_1       0xAE98
#define AREA_HEADER_2       0xAEC8

/*
 * player_state_transition ($DE5E, labels.toml): game_action 7 or $0E.
 * The VRAM requests, the HUD's enemy, the scroll and render_off_depth are
 * cleared, the palette fades out ($C36E), the sprites go ($C31D), one frame
 * passes and every object is cleared. Then:
 *  - game_action 7 (continued): round_start.
 *  - $0E: the next area (area_index and sub_area up); after the third,
 *    sub_area 0 and the next round; round_start - unless that was the last
 *    round: the ending. Ending music ($08), paused and game_finished $FF,
 *    interrupts on (CLI) with irq_enable 0, bank $04's function 6 and its
 *    screen at $9000, the text pages 5 ($9A00 in bank $00), function $0D
 *    and $8C00, pages 6; with no continue used (continue_count 0) also
 *    $9600, the five pages of bank $08's $A700 and $AB00. Then gameplay.
 * Reached through player_state's table (entries 7 and $0E).
 */
void func_DE5E(void) {
    REGS_IN;

    mff_cycles(0xDE5E, 2 + 9 * 3 + 6);
    a = 0;
    SET_NZ(a);
    RAM(VRAM_BUFFER_LEN) = a;
    RAM(VRAM_REQUEST) = a;
    RAM(HUD_ENEMY_SLOT) = a;
    RAM(SCROLL_X) = a;
    RAM(SCROLL_Y) = a;
    RAM(SCROLL_NT) = a;
    RAM(SCROLL_Y_HI) = a;
    RAM(SCROLL_X_HI) = a;
    RAM(RENDER_OFF_DEPTH) = a;
    JSR_AT(0xDE72, 0xC36E, -1);                 /* palette fade */
    mff_cycles(0xDE75, 2 + 3 + 6);
    a = 4;
    SET_NZ(a);
    RAM(OAM_INDEX) = a;
    JSR_AT(0xDE79, 0xC31D, -1);                 /* hide the sprites */
    mff_cycles(0xDE7C, 6);
    JSR_YIELD(0xDE7C, func_FF31);               /* task_wait_frame */
    mff_cycles(0xDE7F, 6);
    JSR_AT(0xDE7F, 0xC334, -1);                 /* objects_clear_all */
    mff_cycles(0xDE82, 3 + 2 + 2);
    a = RAM(GAME_ACTION);
    COMPARE(a, ACT_SELECT);
    if (z) {
        mff_cycles(0xDE88, 3);
        JMP_TAIL(0xDF50, -1);                   /* round_start */
    }
    mff_cycles(0xDE86, 1 + 5 + 5 + 3 + 2 + 2);
    RAM(AREA_INDEX)++;
    RAM(SUB_AREA)++;
    a = RAM(SUB_AREA);
    COMPARE(a, 3);
    if (!c) {
        mff_cycles(0xDE93, 1 + 3);
        JMP_TAIL(0xDF50, -1);                   /* round_start */
    }
    mff_cycles(0xDE95, 2 + 3 + 5 + 3 + 2 + 2);
    a = 0;
    RAM(SUB_AREA) = a;
    RAM(ROUND)++;
    a = RAM(ROUND);
    COMPARE(a, 5);
    if (!z) {
        mff_cycles(0xDE9F, 1 + 3);
        JMP_TAIL(0xDF50, -1);                   /* round_start */
    }

    /* the ending */
    mff_cycles(0xDEA1, 2 + 6);
    a = 0x08;
    SET_NZ(a);
    JSR_FN(0xDEA3, func_F34A);                  /* sound_queue_push: the ending music */
    mff_cycles(0xDEA6, 2 + 3 + 3 + 2 + 2 + 3 + 6);
    a = 0xFF;
    RAM(PAUSED) = a;
    RAM(GAME_FINISHED) = a;
    g_cpu.I = 0;                                /* CLI */
    a = 0;
    SET_NZ(a);
    RAM(IRQ_ENABLE) = a;
    JSR_AT(0xDEB1, 0xC8A6, -1);
    mff_cycles(0xDEB4, 2 + 6);
    x = 6;
    SET_NZ(x);
    JSR_AT(0xDEB6, 0x8000, -1);                 /* bank $04, function 6 */
    mff_cycles(0xDEB9, 6);
    JSR_AT(0xDEB9, 0xC26F, -1);                 /* rendering_on */
    mff_cycles(0xDEBC, 6);
    JSR_AT(0xDEBC, 0x9000, -1);
    mff_cycles(0xDEBF, 2 + 3 + 2 + 3 + 6);
    x = 0x00;
    RAM(BANK_REQUEST) = x;
    x++;
    SET_NZ(x);
    RAM(BANK_REQUEST + 1) = x;
    JSR_FN(0xDEC6, func_FF4D);                  /* bank_switch: $00/$01 */
    mff_cycles(0xDEC9, 2 + 3 + 6);
    a = 5;
    SET_NZ(a);
    RAM(INTRO_PAGE) = a;
    JSR_AT(0xDECD, 0x9A00, -1);                 /* text pages */

    mff_cycles(0xDED0, 2 + 3 + 6);
    a = 0;
    SET_NZ(a);
    RAM(RENDER_OFF_DEPTH) = a;
    JSR_AT(0xDED4, 0xC36E, -1);                 /* palette fade */
    mff_cycles(0xDED7, 2 + 3 + 6);
    a = 4;
    SET_NZ(a);
    RAM(OAM_INDEX) = a;
    JSR_AT(0xDEDB, 0xC31D, -1);                 /* hide the sprites */
    mff_cycles(0xDEDE, 6);
    JSR_YIELD(0xDEDE, func_FF31);               /* task_wait_frame */
    mff_cycles(0xDEE1, 6);
    JSR_AT(0xDEE1, 0xC334, -1);                 /* objects_clear_all */
    mff_cycles(0xDEE4, 6);
    JSR_AT(0xDEE4, 0xC8A6, -1);
    mff_cycles(0xDEE7, 2 + 6);
    x = 0x0D;
    SET_NZ(x);
    JSR_AT(0xDEE9, 0x8000, -1);                 /* bank $04, function $0D */
    mff_cycles(0xDEEC, 6);
    JSR_AT(0xDEEC, 0xC26F, -1);                 /* rendering_on */
    mff_cycles(0xDEEF, 6);
    JSR_AT(0xDEEF, 0x8C00, -1);
    mff_cycles(0xDEF2, 2 + 3 + 2 + 3 + 6);
    x = 0x00;
    RAM(BANK_REQUEST) = x;
    x++;
    SET_NZ(x);
    RAM(BANK_REQUEST + 1) = x;
    JSR_FN(0xDEF9, func_FF4D);                  /* bank_switch: $00/$01 */
    mff_cycles(0xDEFC, 2 + 3 + 6);
    a = 6;
    SET_NZ(a);
    RAM(INTRO_PAGE) = a;
    JSR_AT(0xDF00, 0x9A00, -1);                 /* text pages */

    mff_cycles(0xDF03, 4 + 2);
    a = RAM(CONTINUE_COUNT);
    SET_NZ(a);
    if (z) {                                    /* no continue used: the rest */
        mff_cycles(0xDF08, 2 + 3 + 6);
        a = 0;
        SET_NZ(a);
        RAM(RENDER_OFF_DEPTH) = a;
        JSR_AT(0xDF0C, 0xC36E, -1);             /* palette fade */
        mff_cycles(0xDF0F, 2 + 3 + 6);
        a = 4;
        SET_NZ(a);
        RAM(OAM_INDEX) = a;
        JSR_AT(0xDF13, 0xC31D, -1);             /* hide the sprites */
        mff_cycles(0xDF16, 6);
        JSR_YIELD(0xDF16, func_FF31);           /* task_wait_frame */
        mff_cycles(0xDF19, 6);
        JSR_AT(0xDF19, 0xC334, -1);             /* objects_clear_all */
        mff_cycles(0xDF1C, 6);
        JSR_AT(0xDF1C, 0xC8A6, -1);
        mff_cycles(0xDF1F, 6);
        JSR_AT(0xDF1F, 0xC26F, -1);             /* rendering_on */
        mff_cycles(0xDF22, 6);
        JSR_AT(0xDF22, 0x9600, -1);
        mff_cycles(0xDF25, 2 + 3 + 2 + 3 + 6);
        x = 0x08;
        RAM(BANK_REQUEST) = x;
        x++;
        SET_NZ(x);
        RAM(BANK_REQUEST + 1) = x;
        JSR_FN(0xDF2C, func_FF4D);              /* bank_switch: $08/$09 */
        mff_cycles(0xDF2F, 2 + 3 + 6);
        a = 0;
        SET_NZ(a);
        RAM(INTRO_PAGE) = a;
        JSR_AT(0xDF33, 0xA700, -1);             /* page 0 */
        mff_cycles(0xDF36, 5 + 6);
        RAM(INTRO_PAGE)++;
        SET_NZ(RAM(INTRO_PAGE));
        JSR_AT(0xDF38, 0xA700, -1);             /* page 1 */
        mff_cycles(0xDF3B, 5 + 6);
        RAM(INTRO_PAGE)++;
        SET_NZ(RAM(INTRO_PAGE));
        JSR_AT(0xDF3D, 0xA700, -1);             /* page 2 */
        mff_cycles(0xDF40, 5 + 6);
        RAM(INTRO_PAGE)++;
        SET_NZ(RAM(INTRO_PAGE));
        JSR_AT(0xDF42, 0xA700, -1);             /* page 3 */
        mff_cycles(0xDF45, 5 + 6);
        RAM(INTRO_PAGE)++;
        SET_NZ(RAM(INTRO_PAGE));
        JSR_AT(0xDF47, 0xA700, -1);             /* page 4 */
        mff_cycles(0xDF4A, 6);
        JSR_AT(0xDF4A, 0xAB00, -1);
    } else {
        mff_cycles(0xDF06, 1);
    }
    mff_cycles(0xDF4D, 3);
    JMP_TAIL(0xC8ED, -1);                       /* gameplay */
}

/*
 * round_start ($DF50, proposed): after a continue (game_action 7) the round
 * starts again from its first area: lives 5, player_level back to
 * level_saved, sub_area and player_exp 0, area_index the round's first
 * (0 in round 1; $E161[round] otherwise, never executed in any run), then
 * the select screen (bank $04's function 2 and $9100), the screen cleared,
 * $E143, $E139, the health of the level (player_health_reset) and
 * area_start. Otherwise a new round (sub_area 0) shows its map first:
 * level_saved = player_level, bank $04's function $E15C[round] and the map
 * at $8D00 for 45 frames, then the same $E143, $E139 and health; every
 * area then sets round_tables from $E166[round] and runs on into
 * area_start. Reached by JMP from player_state_transition (0E:DE88).
 */
void func_DF50(void) {
    REGS_IN;

    mff_cycles(0xDF50, 3 + 2 + 2);
    a = RAM(GAME_ACTION);
    COMPARE(a, ACT_SELECT);
    if (z) {
        mff_cycles(0xDF56, 2 + 3 + 3 + 3 + 2 + 3 + 3 + 3 + 3 + 2);
        a = 5;
        RAM(LIVES) = a;
        a = RAM(LEVEL_SAVED);
        RAM(PLAYER_LEVEL) = a;
        a = 0;
        RAM(SUB_AREA) = a;
        RAM(PLAYER_EXP) = a;
        RAM(PLAYER_EXP + 1) = a;
        a = RAM(ROUND);
        if (a == 0) {
            mff_cycles(0xDF6A, 2 + 3 + 2 + 1);
            a = 0;
            SET_NZ(a);
            RAM(AREA_INDEX) = a;
        } else {
            mff_cycles(0xDF68, 1 + 3 + 4);
            y = RAM(ROUND);
            a = prg_read((uint16_t)(ROUND_FIRST_AREA + y));
            SET_NZ(a);
            mff_cycles(0xDF75, 3);
            RAM(AREA_INDEX) = a;
        }
        mff_cycles(0xDF77, 6);
        JSR_FN(0xDF77, func_E12F);
        mff_cycles(0xDF7A, 6);
        JSR_AT(0xDF7A, 0xC85F, -1);
        mff_cycles(0xDF7D, 2 + 3 + 2 + 3 + 6);
        x = 0x04;
        RAM(BANK_REQUEST) = x;
        x++;
        SET_NZ(x);
        RAM(BANK_REQUEST + 1) = x;
        JSR_FN(0xDF84, func_FF4D);              /* bank_switch: $04/$05 */
        mff_cycles(0xDF87, 2 + 6);
        x = 2;
        SET_NZ(x);
        JSR_AT(0xDF89, 0x8000, -1);             /* bank $04, function 2 */
        mff_cycles(0xDF8C, 6);
        JSR_AT(0xDF8C, 0xC26F, -1);             /* rendering_on */
        mff_cycles(0xDF8F, 6);
        JSR_AT(0xDF8F, 0x9100, -1);             /* the select screen */
        mff_cycles(0xDF92, 6);
        JSR_AT(0xDF92, 0xC85C, -1);             /* screen_clear */
        mff_cycles(0xDF95, 6);
        JSR_AT(0xDF95, 0xC26F, -1);             /* rendering_on */
        mff_cycles(0xDF98, 6);
        JSR_FN(0xDF98, func_E143);
        mff_cycles(0xDF9B, 6);
        JSR_FN(0xDF9B, func_E139);
        mff_cycles(0xDF9E, 6);
        JSR_AT(0xDF9E, 0xDE33, -1);             /* player_health_reset */
        mff_cycles(0xDFA1, 3);
        JMP_TAIL(0xDFE9, -1);                   /* area_start */
    }

    mff_cycles(0xDF54, 1 + 3 + 2);
    a = RAM(SUB_AREA);
    if (a == 0) {                               /* a new round: its map */
        mff_cycles(0xDFA8, 3 + 3 + 6);
        a = RAM(PLAYER_LEVEL);
        SET_NZ(a);
        RAM(LEVEL_SAVED) = a;
        JSR_FN(0xDFAC, func_E12F);
        mff_cycles(0xDFAF, 6);
        JSR_AT(0xDFAF, 0xC334, -1);             /* objects_clear_all */
        mff_cycles(0xDFB2, 6);
        JSR_AT(0xDFB2, 0xC85F, -1);
        mff_cycles(0xDFB5, 2 + 3 + 2 + 3 + 6);
        x = 0x04;
        RAM(BANK_REQUEST) = x;
        x++;
        SET_NZ(x);
        RAM(BANK_REQUEST + 1) = x;
        JSR_FN(0xDFBC, func_FF4D);              /* bank_switch: $04/$05 */
        mff_cycles(0xDFBF, 3 + 4);
        y = RAM(ROUND);
        a = prg_read((uint16_t)(ROUND_MAP_FUNCTION + y));
        mff_cycles(0xDFC4, 2 + 6);
        x = a;
        SET_NZ(x);
        JSR_AT(0xDFC5, 0x8000, -1);             /* bank $04, the round's function */
        mff_cycles(0xDFC8, 6);
        JSR_AT(0xDFC8, 0xC26F, -1);             /* rendering_on */
        mff_cycles(0xDFCB, 6);
        JSR_AT(0xDFCB, 0x8D00, -1);             /* the round map */
        mff_cycles(0xDFCE, 2 + 6);
        x = 0x2D;
        SET_NZ(x);
        JSR_YIELD(0xDFD0, func_FF2A);           /* task_wait_frames: 45 */
        mff_cycles(0xDFD3, 6);
        JSR_AT(0xDFD3, 0xC85C, -1);             /* screen_clear */
        mff_cycles(0xDFD6, 6);
        JSR_AT(0xDFD6, 0xC26F, -1);             /* rendering_on */
        mff_cycles(0xDFD9, 6);
        JSR_FN(0xDFD9, func_E143);
        mff_cycles(0xDFDC, 6);
        JSR_FN(0xDFDC, func_E139);
        mff_cycles(0xDFDF, 6);
        JSR_AT(0xDFDF, 0xDE33, -1);             /* player_health_reset */
    } else {
        mff_cycles(0xDFA6, 1);
    }
    mff_cycles(0xDFE2, 3 + 4);
    y = RAM(ROUND);
    a = prg_read((uint16_t)(ROUND_TABLES_INDEX + y));
    mff_cycles(0xDFE7, 3);
    RAM(ROUND_TABLES) = a;
    FALL_INTO(func_DFE9);
}

/*
 * area_start ($DFE9, proposed; runs on to $E12E in the $E000 bank): the
 * round's music unless it already plays; the area's first 33 background
 * columns, one per frame (the column writer $E521 going right, until
 * bg_screen comes back from $FF through 0 to 1); banks $0A/$0B for $9C00,
 * then the round's bank at $A000 and its area header byte ($AE68/$AE98/
 * $AEC8 by sub_area: area_flags and area_screen_count, as $E03B does), the
 * palette fading in ($C362), and the player back in slot 0 (obj_flags $D0,
 * obj_state $80, at the camera's screen, a fall started by $F26E), $C8B4,
 * $E16B, banks $08/$09 ($8000, $8038, stage_spawns $8080); boss_area $FF
 * in a round's third area (with the boss music $05 from round 2 on), the
 * player facing right and the state of a new area cleared. Some areas start
 * with an action: game_action $1B in round 1's second area, $1C in its
 * third, $20 in the third area of rounds 2 and 5. Then gameplay.
 * Reached by JMP from round_start (0E:DFA1) and run on into from it.
 */
void func_DFE9(void) {
    REGS_IN;

    mff_cycles(0xDFE9, 3 + 4);
    y = RAM(ROUND);
    a = prg_read((uint16_t)(ROUND_MUSIC + y));
    mff_cycles(0xDFEE, 3 + 2);
    COMPARE(a, RAM(MUSIC_CURRENT));
    if (!z) {
        mff_cycles(0xDFF2, 6);
        JSR_FN(0xDFF2, func_F34A);              /* sound_queue_push */
    } else {
        mff_cycles(0xDFF0, 1);
    }

    mff_cycles(0xDFF5, 2 + 3 + 2 + 3);
    a = 0xFF;
    RAM(BG_SCREEN) = a;
    a = 0x1F;
    RAM(BG_COLUMN) = a;
    for (;;) {
        mff_cycles(0xDFFD, 2 + 3 + 6);
        a = 0x01;
        SET_NZ(a);
        RAM(TMP16) = a;
        JSR_FN(0xE001, func_E521);              /* the column writer */
        mff_cycles(0xE004, 6);
        JSR_YIELD(0xE004, func_FF31);           /* task_wait_frame */
        mff_cycles(0xE007, 3 + 2);
        a = RAM(BG_SCREEN);
        if (a != 0)
            break;
        mff_cycles(0xE009, 2);                  /* BEQ taken, to the page before */
    }

    mff_cycles(0xE00B, 2 + 3 + 2 + 3 + 6);
    x = 0x0A;
    RAM(BANK_REQUEST) = x;
    x++;
    SET_NZ(x);
    RAM(BANK_REQUEST + 1) = x;
    JSR_FN(0xE012, func_FF4D);                  /* bank_switch: $0A/$0B */
    mff_cycles(0xE015, 6);
    JSR_AT(0xE015, 0x9C00, -1);
    mff_cycles(0xE018, 3 + 4);
    y = RAM(ROUND);
    a = prg_read((uint16_t)(ROUND_BANK + y));
    SET_NZ(a);
    mff_cycles(0xE01D, 3 + 6);
    RAM(BANK_REQUEST + 1) = a;
    JSR_FN(0xE01F, func_FF4D);                  /* bank_switch: the round's bank */
    mff_cycles(0xE022, 3 + 2 + 2);
    a = RAM(SUB_AREA);
    COMPARE(a, 1);
    if (z) {
        mff_cycles(0xE026, 1 + 4);
        a = prg_read(AREA_HEADER_1);
        SET_NZ(a);
        mff_cycles(0xE035, 3);
        JMP_FN(0xE03B, func_E03B);
    }
    mff_cycles(0xE028, 2 + 2);
    COMPARE(a, 2);
    if (!z) {
        mff_cycles(0xE02C, 4);
        a = prg_read(AREA_HEADER_0);
        SET_NZ(a);
        mff_cycles(0xE02F, 3);
        JMP_FN(0xE03B, func_E03B);
    }
    mff_cycles(0xE02A, 1 + 4);
    a = prg_read(AREA_HEADER_2);

    /* $E03B, run on into here */
    mff_cycles(0xE03B, 3 + 2 + 3 + 4 + 2 + 3 + 2 + 3 + 3 + 6);
    mff_push(a);
    a &= 0xE0;
    RAM(AREA_FLAGS) = a;
    a = mff_pull();
    a &= 0x1F;
    RAM(AREA_SCREEN_COUNT) = a;
    a = 0;
    SET_NZ(a);
    RAM(AREA_PALETTE) = a;
    RAM(AREA_SCREEN) = a;
    JSR_AT(0xE04B, 0xC3DD, -1);
    mff_cycles(0xE04E, 2 + 3 + 6);
    a = 0;
    SET_NZ(a);
    RAM(PALETTE_REQUEST) = a;
    JSR_YIELD(0xE052, func_FF31);               /* task_wait_frame */
    mff_cycles(0xE055, 6);
    JSR_AT(0xE055, 0xC362, -1);                 /* palette fade */
    mff_cycles(0xE058, 6);
    JSR_AT(0xE058, 0xC334, -1);                 /* objects_clear_all */

    mff_cycles(0xE05B, 2 + 2 + 4 + 2 + 4 + 2 + 4 + 4 + 3 + 4 + 6);
    x = 0;
    a = FLAGS_FACING_RIGHT;
    RAM(OBJ_FLAGS) = a;
    a = STATE_LIVE;
    RAM(OBJ_STATE) = a;
    a = 0;
    RAM(OBJ_Y_HI) = a;
    RAM(OBJ_ID) = a;
    a = RAM(SCROLL_X_HI);
    SET_NZ(a);
    RAM(OBJ_X_HI) = a;
    JSR_FN(0xE074, func_F26E);
    mff_cycles(0xE077, 6);
    JSR_AT(0xE077, 0xC8B4, -1);
    mff_cycles(0xE07A, 6);
    JSR_FN(0xE07A, func_E16B);
    mff_cycles(0xE07D, 2 + 3 + 2 + 3 + 6);
    x = 0x08;
    RAM(BANK_REQUEST) = x;
    x++;
    SET_NZ(x);
    RAM(BANK_REQUEST + 1) = x;
    JSR_FN(0xE084, func_FF4D);                  /* bank_switch: $08/$09 */
    mff_cycles(0xE087, 6);
    JSR_AT(0xE087, 0x8000, -1);
    mff_cycles(0xE08A, 6);
    JSR_AT(0xE08A, 0x8038, -1);
    mff_cycles(0xE08D, 6);
    JSR_AT(0xE08D, 0x8080, -1);                 /* stage_spawns */

    mff_cycles(0xE090, 3 + 2 + 2);
    a = RAM(SUB_AREA);
    COMPARE(a, 2);
    if (z) {                                    /* a round's last area */
        mff_cycles(0xE096, 3 + 2);
        a = RAM(ROUND);
        if (a != 0) {
            mff_cycles(0xE09A, 2 + 6);
            a = 0x05;
            SET_NZ(a);
            JSR_FN(0xE09C, func_F34A);          /* sound_queue_push: the boss music */
        } else {
            mff_cycles(0xE098, 1);
        }
        mff_cycles(0xE09F, 2 + 2 + 1);
        a = 0xFF;
    } else {
        mff_cycles(0xE094, 1 + 2);
        a = 0;
    }
    mff_cycles(0xE0A5, 3 + 2 + 3 + 3 + 2 + 23 * 3 + 3 * 4 + 3 + 2);
    RAM(BOSS_AREA) = a;
    a = PAD_RIGHT;
    RAM(PLAYER_DIR) = a;
    RAM(JUMP_DIR) = a;
    a = 0;
    RAM(GAME_ACTION) = a;
    RAM(JUMP_MOVING) = a;
    RAM(COMBO_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(UNK_A5) = a;
    RAM(ACTION_TIMER) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(WEAPON_HELD) = a;
    RAM(WEAPON_AMMO) = a;
    RAM(RESPAWN_TIMER2) = a;
    RAM(RESPAWN_TIMER1) = a;
    RAM(CAMERA_LOCK) = a;
    RAM(AREA_CLEAR) = a;
    RAM(AREA_KILLS) = a;
    RAM(SPECIAL_SCREEN_ID) = a;
    RAM(SCREEN_EVENT) = a;
    RAM(UNK_B6) = a;
    RAM(CAMERA_AUTOSCROLL) = a;
    RAM(KEEP_ON_SCREEN) = a;
    RAM(ATTACK_GROUND_Y) = a;
    RAM(ACTION_BEFORE_FALL) = a;
    RAM(GRAB_HIT_COUNT) = a;
    RAM(BONUS_STAGE) = a;
    RAM(SPAWN_GROUPS) = a;
    RAM(SPAWN_GROUPS + 1) = a;
    RAM(SPAWN_GROUPS + 2) = a;
    a = RAM(ROUND);
    if (a == 0) {                               /* round 1 */
        mff_cycles(0xE0EA, 3 + 2 + 2);
        a = RAM(SUB_AREA) & 0x01;
        if (a != 0) {                           /* its second area */
            mff_cycles(0xE0F0, 2 + 3 + 4 + 2 + 4 + 2 + 2);
            a = 0x1B;
            RAM(GAME_ACTION) = a;
            a = RAM(OBJ_FLAGS) | 0x20;
            SET_NZ(a);
            RAM(OBJ_FLAGS) = a;
        } else {
            mff_cycles(0xE0EE, 1 + 3 + 2 + 2);
            a = RAM(SUB_AREA);
            COMPARE(a, 2);
            if (z) {                            /* its third */
                mff_cycles(0xE104, 2 + 3 + 2 + 1);
                a = 0x1C;
                SET_NZ(a);
                RAM(GAME_ACTION) = a;
            } else {
                mff_cycles(0xE102, 1);
            }
        }
    } else {
        mff_cycles(0xE0E8, 2 + 3 + 2 + 2);
        a = RAM(ROUND);
        if (a == 1) {                           /* round 2 */
            mff_cycles(0xE110, 3 + 2 + 2);
            a = RAM(SUB_AREA);
            COMPARE(a, 2);
            if (z) {
                mff_cycles(0xE116, 2 + 3 + 2 + 1);
                a = 0x20;
                SET_NZ(a);
                RAM(GAME_ACTION) = a;
            } else {
                mff_cycles(0xE114, 1);
            }
        } else {
            mff_cycles(0xE10E, 1 + 3 + 2 + 2);
            a = RAM(ROUND);
            COMPARE(a, 4);
            if (z) {                            /* round 5 */
                mff_cycles(0xE122, 3 + 2 + 2);
                a = RAM(SUB_AREA);
                COMPARE(a, 2);
                if (z) {
                    mff_cycles(0xE128, 2 + 3);
                    a = 0x20;
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
    mff_cycles(0xE12C, 3);
    JMP_TAIL(0xC8ED, -1);                       /* gameplay */
}
