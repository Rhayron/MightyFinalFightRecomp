/*
 * flow_story_text.c - the story's text pages, typed out letter by letter
 * (bank $00, $9A00-$9B2B; the macros are the player files' because bank
 * $00 holds them, player_defs.h)
 *
 * story_text ($9A00) types page intro_page: the intro's five pages (0-4,
 * main_task 0E:C5D1-C652: pages 0, 2 and 4 alone on the text screen, 1 and
 * 3 under a picture) and the two of the ending (5 and 6, 0E:DECD and
 * 0E:DF00, under the ending's pictures). Bank $09's ending_text (stage_ending.c)
 * is the same typewriter for the credits.
 *
 * A page is a string at STORY_PAGE_LO/HI[intro_page] of tile numbers ($10
 * 'A', $11 'B', ..., 0 a space), $FE ending a line and $FF the page. Up to
 * three lines show at a time (PPU $22A7, $22E7, $2327): a full screen is
 * held (story_text_hold), cleared and the next one starts. Each letter goes
 * out as a one-byte write list at $0780 (high, low, count - 1 = 0, the tile,
 * $FF) and takes three frames; on page 0 a click (sound $2B) every other
 * frame. A, B, SELECT or START pressed at a letter ends the page at once.
 *
 * Scratch: $0C/$0D the page, $0E the index in it, $10 the line, $11 the
 * column, $12 cleared and not read.
 */
#include "player_defs.h"

#define TEXT_PTR            TMP12   /* $0C/$0D */
#define TEXT_INDEX          TMP14   /* $0E */
#define TEXT_LINE           TMP16   /* $10: 0-2 */
#define TEXT_COLUMN         TMP17   /* $11 */
#define TEXT_UNUSED         TMP18   /* $12 */

/* the page of each intro_page */
#define STORY_PAGE_LO       0x9B2C   /* $9B3A $9C34 $9C8C $9CA3 $9D2E $9D5C $9E07 */
#define STORY_PAGE_HI       0x9B33
/* PPU address of each line: $22A7, $22E7, $2327 */
#define TEXT_LINE_HI        0x9A92   /* 22 22 23 */
#define TEXT_LINE_LO        0x9A95   /* A7 E7 27 */
#define TEXT_END            0xFF
#define TEXT_NEWLINE        0xFE
#define TEXT_LINES          3
#define SFX_TYPE            0x2B
/* frames a full screen or the page's end stays: $B7, $60 for the ending */
#define TEXT_HOLD           0xB7
#define TEXT_HOLD_ENDING    0x60
#define PAGE_ENDING_1       5
#define PAGE_ENDING_2       6

void func_F406(void);   /* draw the objects, wait a frame */
void func_9A13_b0(void);
void func_9A1D_b0(void);

/*
 * story_text ($9A00 in bank $00, proposed): clear the text lines
 * (story_text_clear, which waits a frame), point $0C at page intro_page,
 * then story_text_screen.
 * Callers (banks $00/$01 mapped): main_task (0E:C5D5, C607, C615, C642,
 * C650) and player_state_transition's ending (0E:DECB, DEFE).
 */
void func_9A00_b0(void) {
    REGS_IN;

    mff_cycles(0x9A00, 6);
    JSR_AT(0x9A00, 0x9AC0, 0);                  /* story_text_clear */
    mff_cycles(0x9A03, 3 + 4);
    y = RAM(INTRO_PAGE);
    a = prg_read((uint16_t)(STORY_PAGE_LO + y));
    mff_cycles(0x9A08, 3 + 4);
    RAM(TEXT_PTR) = a;
    a = prg_read((uint16_t)(STORY_PAGE_HI + y));
    mff_cycles(0x9A0D, 3 + 2 + 3);
    RAM(TEXT_PTR + 1) = a;
    a = 0;
    RAM(TEXT_INDEX) = a;
    FALL_INTO(func_9A13_b0);
}

/*
 * story_text_screen ($9A13 in bank $00, proposed): a new screen of text
 * from line 0, column 0, then story_text_type. Run on into from story_text;
 * reached by JMP from story_text_type (00:9A8B) after a full screen.
 */
void func_9A13_b0(void) {
    REGS_IN;

    mff_cycles(0x9A13, 2 + 3 + 3);
    a = 0;
    RAM(TEXT_LINE) = a;
    RAM(TEXT_UNUSED) = a;
    mff_cycles(0x9A19, 2 + 3);
    a = 0;
    RAM(TEXT_COLUMN) = a;
    FALL_INTO(func_9A1D_b0);
}

/*
 * story_text_type ($9A1D in bank $00, proposed): the next character of the
 * page: a letter goes out at the line and column (story_text_letter: three
 * frames pass) and the routine JMPs to itself; $FE goes to the next line,
 * and after the third holds the screen, clears it and starts another
 * (story_text_screen); $FF holds the page and returns (JMP
 * story_text_hold). A, B, SELECT or START pressed: RTS at once.
 * Run on into from story_text_screen; reached by JMP from itself (00:9A7A).
 */
void func_9A1D_b0(void) {
    REGS_IN;

    for (;;) {
        mff_cycles(0x9A1D, 3 + 2 + 2);
        a = RAM(JOY_PRESSED) & (PAD_A | PAD_B | PAD_SELECT | PAD_START);
        SET_NZ(a);
        if (!z) {                               /* skipped */
            mff_cycles(0x9A21, 1 + 6);
            RTS();
        }
        /* the write list's address: the line's start + the column */
        mff_cycles(0x9A23, 3 + 4);
        x = RAM(TEXT_LINE);
        a = prg_read((uint16_t)(TEXT_LINE_HI + x));
        mff_cycles(0x9A28, 4 + 4);
        RAM(VRAM_BUFFER) = a;
        a = prg_read((uint16_t)(TEXT_LINE_LO + x));
        mff_cycles(0x9A2E, 2 + 3 + 4 + 2 + 4 + 3 + 5 + 5);
        c = 0;
        m = RAM(TEXT_COLUMN);                   /* ADC: only V is read later */
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(VRAM_BUFFER + 1) = a;
        a = 0;                                  /* one byte */
        RAM(VRAM_BUFFER + 2) = a;
        y = RAM(TEXT_INDEX);
        RAM(TEXT_INDEX)++;
        a = mem_read((uint16_t)(RAM16(TEXT_PTR) + y));
        mff_cycles(0x9A3F, 2 + 2);
        COMPARE(a, TEXT_END);
        if (z) {
            mff_cycles(0x9A41, 1 + 3);
            JMP_TAIL(0x9B16, 0);                /* story_text_hold */
        }
        mff_cycles(0x9A43, 2 + 2);
        c = a >= TEXT_NEWLINE;
        if (a != TEXT_NEWLINE) {
            /* a letter: three frames pass, then the next character */
            mff_cycles(0x9A47, 4 + 2 + 4 + 3 + 3 + 3 + 3 + 3 + 6);
            RAM(VRAM_BUFFER + 3) = a;
            a = 0xFF;
            RAM(VRAM_BUFFER + 4) = a;           /* the list's end */
            RAM(VRAM_LIST_REQUEST) = a;
            a = RAM(TEXT_LINE);
            mff_push(a);
            a = RAM(TEXT_COLUMN);
            SET_NZ(a);
            mff_push(a);
            JSR_AT(0x9A57, 0x9A98, 0);          /* story_text_picture */
            mff_cycles(0x9A5A, 6);
            JSR_FN(0x9A5A, func_F406);          /* draw the objects, wait a frame */
            mff_cycles(0x9A5D, 6);
            JSR_YIELD(0x9A5D, func_FF31);       /* task_wait_frame */
            mff_cycles(0x9A60, 6);
            JSR_YIELD(0x9A60, func_FF31);
            mff_cycles(0x9A63, 4 + 3 + 4 + 3 + 5 + 3 + 2 + 2);
            a = mff_pull();
            RAM(TEXT_COLUMN) = a;
            a = mff_pull();
            RAM(TEXT_LINE) = a;
            RAM(TEXT_COLUMN)++;
            a = RAM(FRAME_COUNTER) & 0x01;
            SET_NZ(a);
            if (z) {
                mff_cycles(0x9A71, 3 + 2);
                a = RAM(INTRO_PAGE);
                SET_NZ(a);
                if (z) {                        /* page 0 clicks */
                    mff_cycles(0x9A75, 2 + 6);
                    a = SFX_TYPE;
                    SET_NZ(a);
                    JSR_FN(0x9A77, func_F34C);  /* sound_queue_push_sfx */
                } else {
                    mff_cycles(0x9A73, 1);
                }
            } else {
                mff_cycles(0x9A6F, 1);
            }
            mff_cycles(0x9A7A, 3);
            JMP_TAIL(0x9A1D, 0);                /* story_text_type */
        }

        /* the next line */
        mff_cycles(0x9A45, 1 + 5 + 3 + 2 + 2);
        RAM(TEXT_LINE)++;
        a = RAM(TEXT_LINE);
        COMPARE(a, TEXT_LINES);
        if (z)
            break;
        mff_cycles(0x9A83, 1 + 2 + 3);
        a = 0;
        RAM(TEXT_COLUMN) = a;
    }

    /* a full screen: hold it, clear it, the next */
    mff_cycles(0x9A85, 6);
    JSR_AT(0x9A85, 0x9B16, 0);                  /* story_text_hold */
    mff_cycles(0x9A88, 6);
    JSR_AT(0x9A88, 0x9AC0, 0);                  /* story_text_clear */
    mff_cycles(0x9A8B, 3);
    JMP_TAIL(0x9A13, 0);                        /* story_text_screen */
}

/*
 * story_text_picture ($9A98 in bank $00, proposed): on the pages with a
 * picture that moves, each letter sets slot 0's animation back to a frame
 * (obj_anim_frame, timer 0): page 1 frame 2, page 5 frame 1, page 6 frame
 * 0. Called by story_text_type (JSR at 00:9A57).
 */
void func_9A98_b0(void) {
    REGS_IN;

    mff_cycles(0x9A98, 3 + 2 + 2);
    a = RAM(INTRO_PAGE);
    c = a >= 1;
    if (a == 1) {
        mff_cycles(0x9A9C, 1 + 2 + 4 + 2 + 1);
        a = 2;
        RAM(OBJ_ANIM_FRAME) = a;
    } else {
        mff_cycles(0x9A9E, 2 + 2);
        c = a >= PAGE_ENDING_1;
        if (a == PAGE_ENDING_1) {
            mff_cycles(0x9AA0, 1 + 2 + 4 + 2 + 1);
            a = 1;
            RAM(OBJ_ANIM_FRAME) = a;
        } else {
            mff_cycles(0x9AA2, 2 + 2);
            COMPARE(a, PAGE_ENDING_2);
            if (!z) {
                mff_cycles(0x9AA6, 6);
                RTS();
            }
            mff_cycles(0x9AA4, 1 + 2 + 4);
            a = 0;
            RAM(OBJ_ANIM_FRAME) = a;
        }
    }
    mff_cycles(0x9ABA, 2 + 4 + 6);
    a = 0;
    SET_NZ(a);
    RAM(OBJ_ANIM_TIMER) = a;
    RTS();
}

/*
 * story_text_clear ($9AC0 in bank $00, proposed): queues a blank tile at
 * $2379 and 19 blank tiles on each text line ($2327, $22E7, $22A7: write
 * lists at $0784, $079A, $07B0), requests them and waits a frame
 * (task_wait_frame, JMP).
 * Callers: story_text (JSR at 00:9A00) and story_text_type (00:9A88).
 */
void func_9AC0_b0(void) {
    REGS_IN;

    mff_cycles(0x9AC0, 4 * (2 + 4) + 2 + 4 + 2 + 4 + 4 + 3 * (2 + 4) + 2 + 3 * 4 + 2 + 2);
    RAM(VRAM_BUFFER + 0) = 0x23;                /* $2379: one blank tile */
    RAM(VRAM_BUFFER + 1) = 0x79;
    RAM(VRAM_BUFFER + 2) = 0x00;
    RAM(VRAM_BUFFER + 3) = 0x00;
    RAM(VRAM_BUFFER + 4) = 0x23;                /* $2327 */
    a = 0x22;
    RAM(VRAM_BUFFER + 48) = a;                  /* $22A7 */
    RAM(VRAM_BUFFER + 26) = a;                  /* $22E7 */
    RAM(VRAM_BUFFER + 5) = 0x27;
    RAM(VRAM_BUFFER + 27) = 0xE7;
    a = 0xA7;
    RAM(VRAM_BUFFER + 49) = a;
    x = 0x12;                                   /* 19 bytes */
    RAM(VRAM_BUFFER + 6) = x;
    RAM(VRAM_BUFFER + 28) = x;
    RAM(VRAM_BUFFER + 50) = x;
    y = 0;
    a = 0;
    for (;;) {
        mff_cycles(0x9AFF, 3 * 5 + 2 + 2 + 2);
        RAM(VRAM_BUFFER + 7 + y) = a;
        RAM(VRAM_BUFFER + 29 + y) = a;
        RAM(VRAM_BUFFER + 51 + y) = a;
        y++;
        x--;
        if (x & 0x80)
            break;
        mff_cycles(0x9B0A, 2);                  /* BPL back, across a page */
    }
    mff_cycles(0x9B0C, 2 + 4 + 3 + 3);
    a = 0xFF;
    SET_NZ(a);
    RAM(VRAM_BUFFER + 70) = a;                  /* the list's end */
    RAM(VRAM_LIST_REQUEST) = a;
    JMP_FN(0xFF31, func_FF31);                  /* task_wait_frame */
}

/*
 * story_text_hold ($9B16 in bank $00, proposed): wait $B7 frames ($60 on
 * the ending's pages 5 and 6; task_wait_frames).
 * Callers: story_text_type (JSR at 00:9A85, JMP at 00:9A8E).
 */
void func_9B16_b0(void) {
    REGS_IN;

    mff_cycles(0x9B16, 3 + 2 + 2);
    a = RAM(INTRO_PAGE);
    c = a >= PAGE_ENDING_1;
    if (a != PAGE_ENDING_1) {
        mff_cycles(0x9B1C, 2 + 2);
        c = a >= PAGE_ENDING_2;
        if (a != PAGE_ENDING_2) {
            mff_cycles(0x9B20, 2 + 6);
            x = TEXT_HOLD;
            SET_NZ(x);
            JSR_YIELD(0x9B22, func_FF2A);       /* task_wait_frames */
            mff_cycles(0x9B25, 6);
            RTS();
        }
        mff_cycles(0x9B1E, 1);
    } else {
        mff_cycles(0x9B1A, 1);
    }
    mff_cycles(0x9B26, 2 + 6);
    x = TEXT_HOLD_ENDING;
    SET_NZ(x);
    JSR_YIELD(0x9B28, func_FF2A);
    mff_cycles(0x9B2B, 6);
    RTS();
}
