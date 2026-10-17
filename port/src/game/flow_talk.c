/*
 * flow_talk.c - the message box (04:9800): the boss talks and the round's
 * messages, run by gameplay_special_screen (flow_gameplay.c) in place of
 * the game while special_screen_id ($62) is set.
 *
 * The box is the two rows under the playfield ($2322 and $2342) and the
 * YES/NO line ($2367-$2375). special_screen_step ($66) is the phase:
 *   0 talk_open ($980F): a command, or the first page of a talk
 *   1 talk_type ($98DD): types the page a letter at a time
 *   2 talk_end ($9A8F): gives the status bar back
 * special_screen_id values $F0-$FF are commands (each a fixed text from
 * $9BAE/$9BBD: 3 ROUND CLEAR, 4 BONUS STAGE, 5 PERFECT, 6 NO BONUS, 7 YOU
 * GOT, then CONTINUE, 2UP, 1UP, 32/24/16/8 EXP), shown for $F0 frames
 * ($F0 = the wait); smaller values are talk pages.
 *
 * A page (pointers by page at 05:A54C/A4FD; pages 0-$1D are in bank 04,
 * the rest in bank 05) is a header and its text:
 *   +0 the sound of each letter typed ($35 the boss's voice, $2B the
 *      player's); +1 the kind: 0 a plain page, 1 a YES/NO question, 2 a
 *      question whose right answer costs the boss life; +2..+4 for a plain
 *      page the next page by player_id (0 ends the talk), for a question
 *      the page after YES (+2) and after NO (+3), and the right answer
 *      (+4: 0 YES, $80 NO); from +5 the letters (font: $10 = A).
 * In the text, $F9 waits for A or B and turns the page, $FF ends a page
 * (the same, or the question); $FA/$FB set Thrasher's animation, $FC
 * fills the player's health, $FD gives a credit and $FE a life.
 * Thrasher's first page (3, BOW DOWN BEFORE ME!) is a kind-2 question
 * whose right answer is NO. Holding A or B types without delay; START
 * skips the talk.
 *
 * Routines are in bank 04 (the port's func_XXXX_b2), the pages and the
 * talk tables of the box in bank 05. JMPs to another routine are tail
 * calls by address (3 cycles, 2 more at the target).
 */
#include "flow.h"

/* N, Z and C after CMP/CPX/CPY of `r` with `val` */
#define CMP_NZC(r, val) (c = (uint8_t)(r) >= (uint8_t)(val), SET_NZ((uint8_t)(r) - (uint8_t)(val)))
/* JSR at `site` to bank-04 code, through the dispatcher */
#define CALL_04(site, target) do { \
        SYNC_OUT; \
        if (!mff_call_at((uint16_t)(((site) + 2) & 0x1FFF), target, 2)) return; \
        SYNC_IN; \
    } while (0)
/* JSR at `site` to a fixed-bank routine */
#define CALL_0F(site, fn) do { \
        SYNC_OUT; \
        if (!mff_call_fn((uint16_t)(((site) + 2) & 0x1FFF), fn, 0xE000)) return; \
        SYNC_IN; \
    } while (0)
/* JMP at `pc` to a bank-04 routine of its own. Returns. */
#define JMP_04(pc, target) do { \
        mff_cycles(pc, 3); \
        SYNC_OUT; \
        nes_cpu_instruction_boundary(target, 2); \
        call_by_address_tail(target, 2); \
        return; \
    } while (0)
/* an RTS at `pc`. Returns. */
#define RTS_AT(pc) do { mff_cycles(pc, 6); SYNC_OUT; mff_rts(); return; } while (0)

void func_E954(void);       /* hud_level_gauge */
void func_98BD_b2(void);
void func_99D6_b2(void);
void func_9AA0_b2(void);

/* the phases: special_screen_step -> $980F, $98DD, $9A8F */
#define TALK_STEP_LO        0x9D56
#define TALK_STEP_HI        0x9D59
/* the pages, by special_screen_id (bank 05) */
#define TALK_PAGE_LO        0xA54C
#define TALK_PAGE_HI        0xA4FD
/* the fixed texts, by index: vram_buffer entries ended by $FF */
#define TALK_TEXT_LO        0x9BAE
#define TALK_TEXT_HI        0x9BBD
#define TEXT_CLEAR          0       /* empties the box */
#define TEXT_STATUS         1       /* the status bar's labels back */
#define TEXT_YES_NO         2
#define TEXT_ROUND_CLEAR    3
#define TEXT_BONUS_STAGE    4
#define TEXT_PERFECT        5
#define TEXT_NO_BONUS       6
#define TEXT_YOU_GOT        7
/* the bonus stage's reward text by bonus_count (unk_b9): none under 8,
 * then 8/16/24/32 EXP, 1UP, CONTINUE, 2UP */
#define BONUS_REWARD_TEXT   0x98C8

/* commands */
#define CMD_WAIT            0xF0    /* hold the message text_timer frames, then end */
#define CMD_BONUS_STAGE     0xFA
#define CMD_NO_BONUS        0xFB
#define CMD_PERFECT         0xFC
#define CMD_BONUS_START     0xFD    /* health filled, then BONUS STAGE */
#define CMD_BONUS_RESULT    0xFE    /* YOU GOT and the reward */
#define CMD_ROUND_CLEAR     0xFF
#define MESSAGE_FRAMES      0xF0

/* round 4's first page (YOU LOOK A LITTLE RUN DOWN.), and at full health
 * the one by player_id instead ($2F: JESSICA HAS ALREADY BEEN ...) */
#define PAGE_RUN_DOWN       0x33
#define PAGE_FULL_HEALTH    0x9D5C
/* full health by level: Cody, Guy, Haggar */
#define FULL_HEALTH_CODY    0x9D5F
#define FULL_HEALTH_GUY     0x9D65
#define FULL_HEALTH_HAGGAR  0x9D6B

/* page bytes */
#define PAGE_SOUND          0
#define PAGE_KIND           1
#define PAGE_NEXT           2
#define PAGE_ANSWER         4
#define PAGE_TEXT           5
#define KIND_PLAIN          0
#define KIND_HURTS_BOSS     2

#define SFX_CHOICE          0x2F
#define SFX_FILL            0x2E
#define SFX_LIFE            0x2D
#define SFX_CREDIT          0x30

#define TYPE_DELAY          0x0A    /* frames per letter */
#define CURSOR_TILE         0x79
#define BOSS_KIND           0x07    /* obj_kind of Thrasher */
#define BOSS_HIT            0x08    /* life a right answer costs */

/* vram_buffer entries: [count, address high, address low, tiles] */
#define BOX_ROW_HI          0x23
#define BOX_ROW_LO          0x22    /* + text_cursor: the box's first row */

/*
 * talk_screen, $9800 (bank 04; name proposed): the special screen's frame:
 * JMP through TALK_STEP_LO/HI by special_screen_step.
 * Caller: gameplay_special_screen (0E:C991).
 */
void func_9800_b2(void) {
    REGS_IN;

    mff_cycles(0x9800, 7);
    y = RAM(SPECIAL_SCREEN_STEP);
    a = prg_read((uint16_t)(TALK_STEP_LO + y));
    mff_cycles(0x9805, 7);
    RAM(TMP12) = a;
    a = prg_read((uint16_t)(TALK_STEP_HI + y));
    SET_NZ(a);
    mff_cycles(0x980A, 8);
    RAM(TMP13) = a;
    SYNC_OUT;
    {
        uint16_t jt = nes_read16zp(0x000C);
        nes_cpu_instruction_boundary(jt, 2);
        call_by_address_tail(jt, -1);
    }
}

/* ---- phase 0 ---- */

/* $FD: the player's health filled ($9B18), then BONUS STAGE */
static void talk_cmd_bonus_start(void) {
    REGS_IN;

    mff_cycles(0x9879, 6);
    CALL_04(0x9879, 0x9B18);
    mff_cycles(0x987C, 11);
    a = CMD_BONUS_STAGE;
    SET_NZ(a);
    RAM(SPECIAL_SCREEN_ID) = a;
    SYNC_OUT;
    mff_rts();
}

/* $FF: ROUND CLEAR and the round's number at $2350; level_saved =
 * player_level. Then the wait ($98BD; by a BNE that the buffer's end being
 * 0 would turn into $FD's code) */
static void talk_cmd_round_clear(void) {
    REGS_IN;

    mff_cycles(0x984C, 14);
    a = RAM(PLAYER_LEVEL);
    RAM(LEVEL_SAVED) = a;
    y = TEXT_ROUND_CLEAR;
    SET_NZ(y);
    CALL_04(0x9852, 0x9AA0);
    mff_cycles(0x9855, 59);
    x = RAM(VRAM_BUFFER_LEN);
    RAM(VRAM_BUFFER + x) = 0x01;
    x++;
    RAM(VRAM_BUFFER + x) = 0x23;
    x++;
    RAM(VRAM_BUFFER + x) = 0x50;
    x++;
    a = RAM(ROUND);
    RAM(VRAM_BUFFER + x) = (uint8_t)(a + 2);    /* the digit tiles start at 1 = "0" */
    x++;
    SET_NZ(x);
    RAM(VRAM_BUFFER_LEN) = x;
    SYNC_OUT;
    if (!z) {
        mff_cycles(0x9877, 1);
        func_98BD_b2();
    } else {                                    /* never */
        talk_cmd_bonus_start();
    }
}

/* $FA $FB $FC: the fixed text Y, then the wait (JMP $98BD) */
static void talk_cmd_text(uint16_t pc, uint8_t text) {
    REGS_IN;

    mff_cycles(pc, 8);
    y = text;
    SET_NZ(y);
    CALL_04((uint16_t)(pc + 2), 0x9AA0);
    JMP_04((uint16_t)(pc + 5), 0x98BD);
}

/* $FE: YOU GOT and the reward of bonus_count, which goes back to 0 */
static void talk_cmd_bonus_result(void) {
    REGS_IN;

    mff_cycles(0x9889, 8);
    y = TEXT_YOU_GOT;
    SET_NZ(y);
    CALL_04(0x988B, 0x9AA0);
    mff_cycles(0x988E, 9);
    y = RAM(UNK_B9);
    a = prg_read((uint16_t)(BONUS_REWARD_TEXT + y));
    mff_cycles(0x9894, 8);
    y = a;
    SET_NZ(y);
    CALL_04(0x9895, 0x9AA0);
    mff_cycles(0x9898, 5);
    a = 0x00;
    SET_NZ(a);
    RAM(UNK_B9) = a;
    JMP_04(0x989C, 0x98BD);
}

/* the commands other than $F0, tested in this order ($9818-$983A), each
 * LDA special_screen_id / CMP / BEQ */
static const uint8_t k_commands[] = {
    CMD_BONUS_RESULT, CMD_ROUND_CLEAR, CMD_BONUS_START, CMD_PERFECT, CMD_NO_BONUS, CMD_BONUS_STAGE,
};

/*
 * talk_open, $980F (bank 04; name proposed): phase 0. $F0 waits ($98AF);
 * the other commands put up their message and set $F0 with text_timer
 * $F0 ($98BD); a page clears the box ($9A9E), zeroes talk_bar_cuts,
 * text_cursor, dialogue_choice and text_timer, and goes to phase 1.
 * Caller: JMP ($000C) at $980C.
 */
void func_980F_b2(void) {
    REGS_IN;
    unsigned i;

    mff_cycles(0x980F, 7);
    a = RAM(SPECIAL_SCREEN_ID);
    CMP_NZC(a, CMD_WAIT);
    if (z)
        JMP_04(0x9815, 0x98AF);
    mff_cycles(0x9813, 1);
    for (i = 0; i < sizeof k_commands; i++) {
        uint16_t pc = (uint16_t)(0x9818 + 6 * i);

        mff_cycles(pc, 7);
        a = RAM(SPECIAL_SCREEN_ID);
        CMP_NZC(a, k_commands[i]);
        if (z) {
            mff_cycles((uint16_t)(pc + 4), 1);
            SYNC_OUT;
            switch (k_commands[i]) {
            case CMD_BONUS_RESULT: talk_cmd_bonus_result(); break;
            case CMD_ROUND_CLEAR:  talk_cmd_round_clear(); break;
            case CMD_BONUS_START:  talk_cmd_bonus_start(); break;
            case CMD_PERFECT:      talk_cmd_text(0x989F, TEXT_PERFECT); break;
            case CMD_NO_BONUS:     talk_cmd_text(0x98A7, TEXT_NO_BONUS); break;
            default:               talk_cmd_text(0x9881, TEXT_BONUS_STAGE); break;
            }
            return;
        }
    }

    /* a page */
    mff_cycles(0x983C, 6);
    CALL_04(0x983C, 0x9A9E);
    mff_cycles(0x983F, 25);
    a = 0x00;
    RAM(TALK_BAR_CUTS) = a;
    RAM(TEXT_CURSOR) = a;
    RAM(DIALOGUE_CHOICE) = a;
    RAM(TEXT_TIMER) = a;
    RAM(SPECIAL_SCREEN_STEP)++;
    SET_NZ(RAM(SPECIAL_SCREEN_STEP));
    SYNC_OUT;
    mff_rts();
}

/*
 * talk_wait, $98AF (bank 04; name proposed): command $F0. text_timer - 1;
 * at 0 the box is cleared and the phase goes 0 -> 2 (talk_end).
 * Caller: JMP at $9815.
 */
void func_98AF_b2(void) {
    REGS_IN;

    mff_cycles(0x98AF, 10);
    RAM(TEXT_TIMER)--;
    a = RAM(TEXT_TIMER);
    SET_NZ(a);
    if (z) {
        mff_cycles(0x98B5, 16);
        RAM(SPECIAL_SCREEN_STEP) += 2;
        SET_NZ(RAM(SPECIAL_SCREEN_STEP));
        CALL_04(0x98B9, 0x9A9E);
    } else {
        mff_cycles(0x98B3, 1);
    }
    RTS_AT(0x98BC);
}

/*
 * talk_hold, $98BD (bank 04; name proposed): text_timer $F0,
 * special_screen_id $F0 (the wait) and vram_request.
 * Callers: the commands (JMP at $9886, $989C, $98A4, $98AC; BNE at $9877).
 */
void func_98BD_b2(void) {
    REGS_IN;

    mff_cycles(0x98BD, 19);
    RAM(TEXT_TIMER) = MESSAGE_FRAMES;
    a = CMD_WAIT;
    SET_NZ(a);
    RAM(SPECIAL_SCREEN_ID) = a;
    RAM(VRAM_REQUEST) = a;
    SYNC_OUT;
    mff_rts();
}

/* ---- phase 1 ---- */

/* the text bytes $F9-$FF and their routines ($992F-$995D: CMP / BNE +3 /
 * JMP) */
static const uint16_t k_text_controls[] = {
    0x99D6,     /* $F9: turn the page */
    0x9AD1,     /* $FA: Thrasher's animation $E4 */
    0x9AD9,     /* $FB: Thrasher's animation $EA */
    0x9B18,     /* $FC: fill the health */
    0x9BA4,     /* $FD: a credit */
    0x9B7E,     /* $FE: a life */
    0x999B,     /* $FF: end of the page */
};

/*
 * talk_type, $98DD (bank 04; name proposed): phase 1. START skips to
 * phase 2. Otherwise every text_timer frames the next byte of the page:
 * a letter is queued at $2322 + text_cursor (with the page's sound unless
 * it is a space), then text_timer is $0A, or 0 while A or B is held; the
 * bytes $F9-$FF run their routine. Round 4's first page is replaced at
 * full health (no route).
 * Caller: JMP ($000C) at $980C.
 */
void func_98DD_b2(void) {
    REGS_IN;
    unsigned i;

    mff_cycles(0x98DD, 7);
    a = RAM(JOY_PRESSED);
    CMP_NZC(a, PAD_START);
    if (z) {
        mff_cycles(0x98E3, 6);
        CALL_04(0x98E3, 0x9A9E);
        mff_cycles(0x98E6, 11);
        RAM(SPECIAL_SCREEN_STEP)++;
        SET_NZ(RAM(SPECIAL_SCREEN_STEP));
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0x98E1, 1);
    mff_cycles(0x98E9, 10);
    RAM(TEXT_TIMER)--;
    a = RAM(TEXT_TIMER);
    SET_NZ(a);
    if (!n)
        RTS_AT(0x98EF);
    mff_cycles(0x98ED, 1);

    /* round 4's page at full health (no route) */
    mff_cycles(0x98F0, 7);
    a = RAM(SPECIAL_SCREEN_ID);
    if (a != PAGE_RUN_DOWN) {
        mff_cycles(0x98F4, 2);                  /* BNE across a page */
    } else {
        /* the table by player_id; a 0 in it would fall into the next one */
        int table;                              /* 0 Haggar's, 1 Cody's, 2 Guy's, 3 found */

        mff_cycles(0x98F6, 8);
        y = RAM(PLAYER_LEVEL);
        a = RAM(PLAYER_ID);
        if (a == 0) {
            mff_cycles(0x98FA, 2);              /* BEQ across a page */
            table = 1;
        } else {
            mff_cycles(0x98FC, 4);
            table = a == 0x01 ? 2 : 0;
            if (table == 2)
                mff_cycles(0x98FE, 1);
        }
        if (table == 0) {
            mff_cycles(0x9900, 4);
            a = prg_read((uint16_t)(FULL_HEALTH_HAGGAR + y));
            mff_cycles(0x9903, 2);
            table = a != 0 ? 3 : 1;
            if (table == 3)
                mff_cycles(0x9903, 1);
        }
        if (table == 1) {
            mff_cycles(0x9905, 4);
            a = prg_read((uint16_t)(FULL_HEALTH_CODY + y));
            mff_cycles(0x9908, 2);
            table = a != 0 ? 3 : 2;
            if (table == 3)
                mff_cycles(0x9908, 1);
        }
        if (table == 2) {
            mff_cycles(0x990A, 4);
            a = prg_read((uint16_t)(FULL_HEALTH_GUY + y));
        }
        mff_cycles(0x990D, 5);
        if (a == RAM(HEALTH)) {
            mff_cycles(0x9911, 7);
            y = RAM(PLAYER_ID);
            a = prg_read((uint16_t)(PAGE_FULL_HEALTH + y));
            mff_cycles(0x9916, 3);
            RAM(SPECIAL_SCREEN_ID) = a;
        } else {
            mff_cycles(0x990F, 1);
        }
    }

    /* the next byte */
    mff_cycles(0x9918, 10);
    y = RAM(SPECIAL_SCREEN_ID);
    x = RAM(VRAM_BUFFER_LEN);
    a = prg_read((uint16_t)(TALK_PAGE_LO + y));
    mff_cycles(0x991F, 7);
    RAM(TMP12) = a;
    a = prg_read((uint16_t)(TALK_PAGE_HI + y));
    mff_cycles(0x9924, 21);
    RAM(TMP13) = a;
    y = (uint8_t)(RAM(TEXT_CURSOR) + PAGE_TEXT);
    a = mem_read((uint16_t)(RAM16(TMP12) + y));
    for (i = 0; i < 7; i++) {
        uint16_t pc = (uint16_t)(0x992F + 7 * i);

        mff_cycles(pc, 4);
        CMP_NZC(a, 0xF9 + i);
        if (z)
            JMP_04((uint16_t)(pc + 4), k_text_controls[i]);
        mff_cycles((uint16_t)(pc + 2), 1);
    }

    /* a letter */
    mff_cycles(0x9960, 9);
    RAM(VRAM_BUFFER + 3 + x) = a;
    c = 1;                                      /* CMP #0 */
    if (a != 0) {
        mff_cycles(0x9967, 7);
        y = PAGE_SOUND;
        a = mem_read((uint16_t)(RAM16(TMP12) + y));
        SET_NZ(a);
        mff_cycles(0x996B, 6);
        CALL_0F(0x996B, func_F34C);             /* sound_queue_push_sfx */
    } else {
        mff_cycles(0x9965, 1);
    }
    mff_cycles(0x996E, 49);
    RAM(VRAM_BUFFER + x) = 0x01;
    RAM(VRAM_BUFFER + 1 + x) = BOX_ROW_HI;
    a = BOX_ROW_LO;
    c = 0;
    m = RAM(TEXT_CURSOR);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(VRAM_BUFFER + 2 + x) = a;
    RAM(TEXT_CURSOR)++;
    x = (uint8_t)(x + 4);
    RAM(VRAM_BUFFER_LEN) = x;
    a = RAM(JOY_HELD) & (PAD_A | PAD_B);
    if (a == 0) {
        mff_cycles(0x998C, 1);
        mff_cycles(0x9992, 2);
        a = TYPE_DELAY;
    } else {
        mff_cycles(0x998E, 4);
        a = 0x00;
        mff_cycles(0x9990, 1);                  /* BEQ, always taken */
    }
    mff_cycles(0x9994, 14);
    RAM(TEXT_TIMER) = a;
    a = 0xFF;
    SET_NZ(a);
    RAM(VRAM_REQUEST) = a;
    SYNC_OUT;
    mff_rts();
}

/*
 * $99DF on: A = the next page (from the page's PAGE_NEXT bytes). Not 0:
 * special_screen_id = it, text_cursor and text_timer 0, and phase 1 types
 * it from its start; 0: the talk is over (phase 2).
 */
static void talk_goto(void) {
    REGS_IN;

    mff_cycles(0x99DF, 2);
    if (a != 0) {
        mff_cycles(0x99E1, 17);
        RAM(SPECIAL_SCREEN_ID) = a;
        a = 0x00;
        SET_NZ(a);
        RAM(TEXT_CURSOR) = a;
        RAM(TEXT_TIMER) = a;
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0x99DF, 1);
    mff_cycles(0x99EA, 11);
    RAM(SPECIAL_SCREEN_STEP)++;
    SET_NZ(RAM(SPECIAL_SCREEN_STEP));
    SYNC_OUT;
    mff_rts();
}

/*
 * talk_page_end, $999B (bank 04; name proposed): text byte $FF. A plain
 * page waits for A or B (meanwhile the marker $7A at $236F) and turns
 * ($99D6). A question draws YES/NO with the cursor ($79) on the choice;
 * LEFT or RIGHT flips dialogue_choice bit 7 (sound $2F); A or B answers:
 * a kind-2 page checks the answer ($9A74), the box is cleared, and the
 * page after YES or NO follows ($99DD).
 * Caller: JMP at $995D.
 */
void func_999B_b2(void) {
    REGS_IN;

    mff_cycles(0x999B, 7);
    y = RAM(SPECIAL_SCREEN_ID);
    a = prg_read((uint16_t)(TALK_PAGE_LO + y));
    mff_cycles(0x99A0, 7);
    RAM(TMP12) = a;
    a = prg_read((uint16_t)(TALK_PAGE_HI + y));
    mff_cycles(0x99A5, 10);
    RAM(TMP13) = a;
    y = PAGE_KIND;
    a = mem_read((uint16_t)(RAM16(TMP12) + y));
    mff_cycles(0x99AB, 2);
    if (a == KIND_PLAIN) {
        mff_cycles(0x99AD, 7);
        a = RAM(JOY_PRESSED) & (PAD_A | PAD_B);
        SET_NZ(a);
        if (z) {
            mff_cycles(0x99B3, 56);
            RAM(TEXT_TIMER) = a;
            x = RAM(VRAM_BUFFER_LEN);
            RAM(VRAM_BUFFER + x) = 0x01;
            x++;
            RAM(VRAM_BUFFER + x) = 0x23;
            x++;
            RAM(VRAM_BUFFER + x) = 0x6F;
            x++;
            RAM(VRAM_BUFFER + x) = 0x7A;
            x++;
            RAM(VRAM_BUFFER_LEN) = x;
            a = 0xFF;
            SET_NZ(a);
            RAM(VRAM_REQUEST) = a;
            SYNC_OUT;
            mff_rts();
            return;
        }
        mff_cycles(0x99B1, 1);
        SYNC_OUT;
        func_99D6_b2();                         /* branches into it */
        return;
    }

    /* a question */
    mff_cycles(0x99AB, 1);
    mff_cycles(0x99ED, 8);
    y = TEXT_YES_NO;
    SET_NZ(y);
    CALL_04(0x99EF, 0x9AA0);
    mff_cycles(0x99F2, 7);
    a = RAM(JOY_PRESSED) & (PAD_A | PAD_B);
    if (a == 0) {
        mff_cycles(0x99F8, 7);
        a = RAM(JOY_PRESSED) & (PAD_LEFT | PAD_RIGHT);
        if (a != 0) {
            mff_cycles(0x99FE, 8);
            a = SFX_CHOICE;
            SET_NZ(a);
            CALL_0F(0x9A00, func_F34C);
            mff_cycles(0x9A03, 8);
            a = RAM(DIALOGUE_CHOICE) ^ 0x80;
            RAM(DIALOGUE_CHOICE) = a;
        } else {
            mff_cycles(0x99FC, 2);              /* BEQ across a page */
        }
        mff_cycles(0x9A09, 50);
        x = RAM(VRAM_BUFFER_LEN);
        RAM(VRAM_BUFFER + x) = 0x01;            /* $2367 and $2373, a tile each */
        RAM(VRAM_BUFFER + 4 + x) = 0x01;
        RAM(VRAM_BUFFER + 1 + x) = 0x23;
        RAM(VRAM_BUFFER + 5 + x) = 0x23;
        RAM(VRAM_BUFFER + 2 + x) = 0x67;
        RAM(VRAM_BUFFER + 6 + x) = 0x73;
        a = RAM(DIALOGUE_CHOICE);
        if (!(a & 0x80)) {                      /* YES */
            mff_cycles(0x9A2D, 16);
            RAM(VRAM_BUFFER + 3 + x) = CURSOR_TILE;
            a = 0x00;
            RAM(VRAM_BUFFER + 7 + x) = a;
            mff_cycles(0x9A37, 1);              /* BEQ, always taken */
        } else {                                /* NO */
            mff_cycles(0x9A2B, 1);
            mff_cycles(0x9A39, 14);
            RAM(VRAM_BUFFER + 3 + x) = 0x00;
            a = CURSOR_TILE;
            RAM(VRAM_BUFFER + 7 + x) = a;
        }
        mff_cycles(0x9A43, 35);
        x = (uint8_t)(x + 8);
        RAM(VRAM_BUFFER_LEN) = x;
        RAM(TEXT_TIMER) = 0x00;
        a = 0xFF;
        SET_NZ(a);
        RAM(VRAM_REQUEST) = a;
        SYNC_OUT;
        mff_rts();
        return;
    }

    /* answered */
    mff_cycles(0x99F6, 2);                      /* BNE across a page */
    mff_cycles(0x9A56, 7);
    y = PAGE_KIND;
    a = mem_read((uint16_t)(RAM16(TMP12) + y));
    mff_cycles(0x9A5A, 4);
    CMP_NZC(a, KIND_HURTS_BOSS);
    if (z) {
        mff_cycles(0x9A5E, 6);
        CALL_04(0x9A5E, 0x9A74);
    } else {
        mff_cycles(0x9A5C, 1);
    }
    mff_cycles(0x9A61, 6);
    CALL_04(0x9A61, 0x9A9E);
    mff_cycles(0x9A64, 7);
    y = PAGE_NEXT;                              /* YES */
    a = RAM(DIALOGUE_CHOICE);
    if (a & 0x80) {
        mff_cycles(0x9A6A, 2);
        y++;                                    /* NO */
    } else {
        mff_cycles(0x9A68, 1);
    }
    mff_cycles(0x9A6B, 8);
    a = 0x00;
    SET_NZ(a);
    RAM(TEXT_CURSOR) = a;
    RAM(DIALOGUE_CHOICE) = a;
    JMP_04(0x9A71, 0x99DD);
}

/*
 * talk_next_page, $99D6 (bank 04; name proposed): text byte $F9, or A/B on
 * a plain page: the box is cleared and the page goes on to its next one
 * for this character (PAGE_NEXT + player_id).
 * Callers: JMP at $9933; the BNE at $99B1.
 */
void func_99D6_b2(void) {
    REGS_IN;

    mff_cycles(0x99D6, 6);
    CALL_04(0x99D6, 0x9A9E);
    mff_cycles(0x99D9, 12);
    y = (uint8_t)(RAM(PLAYER_ID) + PAGE_NEXT);
    a = mem_read((uint16_t)(RAM16(TMP12) + y));
    SYNC_OUT;
    talk_goto();
}

/*
 * talk_goto_page, $99DD (bank 04; name proposed): the next page is byte Y
 * of the page.
 * Caller: JMP at $9A71 (the answer of a question).
 */
void func_99DD_b2(void) {
    REGS_IN;

    mff_cycles(0x99DD, 5);
    a = mem_read((uint16_t)(RAM16(TMP12) + y));
    SYNC_OUT;
    talk_goto();
}

/*
 * talk_answer, $9A74 (bank 04; name proposed): the right answer (byte
 * PAGE_ANSWER = dialogue_choice) costs the boss (talk_find_boss) 8 life:
 * obj_life - 8, a piece of its bar erased ($9AF4), talk_bar_cuts + 1.
 * In: TMP12/TMP13 = the page. Out: X = the boss's slot when right.
 * Caller: talk_page_end ($9A5E).
 */
void func_9A74_b2(void) {
    REGS_IN;

    mff_cycles(0x9A74, 7);
    y = PAGE_ANSWER;
    a = mem_read((uint16_t)(RAM16(TMP12) + y));
    mff_cycles(0x9A78, 5);
    CMP_NZC(a, RAM(DIALOGUE_CHOICE));
    if (!z) {
        mff_cycles(0x9A7A, 1);
        RTS_AT(0x9A8E);
    }
    mff_cycles(0x9A7C, 6);
    CALL_04(0x9A7C, 0x9AC2);
    mff_cycles(0x9A7F, 19);
    a = RAM(OBJ_LIFE + x);
    c = 1;
    m = BOSS_HIT;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    SET_NZ(a);
    RAM(OBJ_LIFE + x) = a;
    CALL_04(0x9A88, 0x9AF4);
    mff_cycles(0x9A8B, 11);
    RAM(TALK_BAR_CUTS)++;
    SET_NZ(RAM(TALK_BAR_CUTS));
    SYNC_OUT;
    mff_rts();
}

/* ---- phase 2 ---- */

/*
 * talk_end, $9A8F (bank 04; name proposed): phase 2. special_screen_step
 * and special_screen_id 0 (the game goes on), the status bar's labels
 * back (text 1) and its level gauge (0F:E954).
 * Caller: JMP ($000C) at $980C.
 */
void func_9A8F_b2(void) {
    REGS_IN;

    mff_cycles(0x9A8F, 16);
    y = 0x00;
    RAM(SPECIAL_SCREEN_STEP) = y;
    RAM(SPECIAL_SCREEN_ID) = y;
    y = TEXT_STATUS;
    SET_NZ(y);
    CALL_04(0x9A97, 0x9AA0);
    mff_cycles(0x9A9A, 6);
    CALL_0F(0x9A9A, func_E954);
    RTS_AT(0x9A9D);
}

/* ---- the box's services ---- */

/*
 * talk_box_clear, $9A9E (bank 04; name proposed): Y = 0 and on into
 * talk_text: the box emptied.
 * Callers: talk_open, talk_wait, talk_type, talk_page_end, talk_next_page.
 */
void func_9A9E_b2(void) {
    mff_cycles(0x9A9E, 2);
    g_cpu.Y = TEXT_CLEAR;
    func_9AA0_b2();
}

/*
 * talk_text, $9AA0 (bank 04; name proposed): copies fixed text Y (its
 * vram_buffer entries, up to the $FF) to the end of vram_buffer, and
 * vram_request.
 * In: Y = the text. Out: X = the buffer's end, A = $FF, C set (or clear
 * if the buffer wrapped).
 */
void func_9AA0_b2(void) {
    REGS_IN;

    mff_cycles(0x9AA0, 4);
    a = prg_read((uint16_t)(TALK_TEXT_LO + y));
    mff_cycles(0x9AA3, 7);
    RAM(TMP10) = a;
    a = prg_read((uint16_t)(TALK_TEXT_HI + y));
    mff_cycles(0x9AA8, 8);
    RAM(TMP11) = a;
    x = RAM(VRAM_BUFFER_LEN);
    y = 0x00;
    for (;;) {
        mff_cycles(0x9AAE, 5);
        a = mem_read((uint16_t)(RAM16(TMP10) + y));
        mff_cycles(0x9AB0, 4);
        c = a >= 0xFF;
        if (a == 0xFF) {
            mff_cycles(0x9AB2, 1);
            break;
        }
        mff_cycles(0x9AB4, 11);
        RAM(VRAM_BUFFER + x) = a;
        y++;
        x++;
        if (x == 0)
            break;
        mff_cycles(0x9AB9, 1);
    }
    mff_cycles(0x9ABB, 14);
    RAM(VRAM_BUFFER_LEN) = x;
    a = 0xFF;
    SET_NZ(a);
    RAM(VRAM_REQUEST) = a;
    SYNC_OUT;
    mff_rts();
}

/*
 * talk_find_boss, $9AC2 (bank 04; name proposed): X = the first slot from
 * 2 up with obj_kind 7 (Thrasher), or $10.
 * Callers: talk_answer, talk_boss_anim.
 */
void func_9AC2_b2(void) {
    REGS_IN;

    mff_cycles(0x9AC2, 2);
    x = 0x02;
    for (;;) {
        mff_cycles(0x9AC4, 8);
        a = RAM(OBJ_KIND + x);
        CMP_NZC(a, BOSS_KIND);
        if (z) {
            mff_cycles(0x9AC9, 1);
            break;
        }
        mff_cycles(0x9ACB, 6);
        x++;
        CMP_NZC(x, 0x10);
        if (z)
            break;
        mff_cycles(0x9ACE, 1);
    }
    RTS_AT(0x9AD0);
}

/* $9ADF: chr_banks+4 = A, and the boss takes animation TMP7 from its
 * first frame; text_cursor + 1 */
static void talk_boss_anim(void) {
    REGS_IN;

    mff_cycles(0x9ADF, 9);
    RAM(CHR_BANKS + 4) = a;
    CALL_04(0x9AE1, 0x9AC2);
    mff_cycles(0x9AE4, 31);
    RAM(OBJ_ANIM + x) = RAM(TMP7);
    a = 0x00;
    RAM(OBJ_ANIM_TIMER + x) = a;
    RAM(OBJ_ANIM_FRAME + x) = a;
    RAM(TEXT_CURSOR)++;
    SET_NZ(RAM(TEXT_CURSOR));
    SYNC_OUT;
    mff_rts();
}

/*
 * talk_boss_anim_e4, $9AD1 (bank 04; name proposed): text byte $FA:
 * Thrasher takes animation $E4 with CHR bank $28 (chr_banks+4).
 * Caller: JMP at $993A.
 */
void func_9AD1_b2(void) {
    REGS_IN;

    mff_cycles(0x9AD1, 9);
    RAM(TMP7) = 0xE4;
    a = 0x28;
    SET_NZ(a);
    mff_cycles(0x9AD7, 1);                      /* BNE, always taken */
    SYNC_OUT;
    talk_boss_anim();
}

/*
 * talk_boss_anim_ea, $9AD9 (bank 04; name proposed): text byte $FB:
 * animation $EA with CHR bank $2B (WAHHHHH!, ARGHHHH!).
 * Caller: JMP at $9941.
 */
void func_9AD9_b2(void) {
    REGS_IN;

    mff_cycles(0x9AD9, 7);
    RAM(TMP7) = 0xEA;
    a = 0x2B;
    SET_NZ(a);
    SYNC_OUT;
    talk_boss_anim();
}

/*
 * talk_bar_cut, $9AF4 (bank 04; name proposed): erases a piece of the
 * boss's life bar on the status bar: tile $10 at $2072 - talk_bar_cuts.
 * Caller: talk_answer ($9A88).
 */
void func_9AF4_b2(void) {
    REGS_IN;

    mff_cycles(0x9AF4, 58);
    x = RAM(VRAM_BUFFER_LEN);
    RAM(VRAM_BUFFER + x) = 0x01;
    x++;
    RAM(VRAM_BUFFER + x) = 0x20;
    x++;
    a = 0x72;
    c = 1;
    m = RAM(TALK_BAR_CUTS);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    RAM(VRAM_BUFFER + x) = 0x10;
    x++;
    RAM(VRAM_BUFFER_LEN) = x;
    a = 0xFF;
    SET_NZ(a);
    RAM(VRAM_REQUEST) = a;
    SYNC_OUT;
    mff_rts();
}

/* the full health and the bar length in tiles, by player_id */
static const uint8_t k_fill_health[] = { 0x40, 0x30, 0x50 };
static const uint8_t k_fill_tiles[] = { 0x08, 0x06, 0x0A };

/*
 * talk_fill_health, $9B18 (bank 04; name proposed): text byte $FC (page
 * $36, after the water) and command $FD: health = the full health of the
 * level ($40/$30/$50 + 8 * player_level, Cody/Guy/Haggar), sound $2E, and
 * the health bar redrawn at $2049: tile $19, $18 for each 8 of health,
 * $1A. text_cursor + 1. No route.
 * Callers: JMP at $9948; talk_cmd_bonus_start ($9879).
 */
void func_9B18_b2(void) {
    REGS_IN;
    int who;

    mff_cycles(0x9B18, 17);
    a = (uint8_t)(RAM(PLAYER_LEVEL) << 3);
    RAM(TMP8) = a;
    y = RAM(PLAYER_ID);
    if (y == 0) {
        mff_cycles(0x9B21, 1);
        mff_cycles(0x9B2D, 6);
        who = 0;
        mff_cycles(0x9B31, 1);                  /* BNE, always taken */
    } else {
        mff_cycles(0x9B23, 6);
        y = (uint8_t)(y - 2);
        if (y == 0) {
            mff_cycles(0x9B25, 1);
            mff_cycles(0x9B33, 4);
            who = 2;
        } else {
            mff_cycles(0x9B27, 6);
            who = 1;
            mff_cycles(0x9B2B, 1);              /* BNE, always taken */
        }
    }
    a = k_fill_health[who];
    x = k_fill_tiles[who];
    mff_cycles(0x9B37, 16);
    c = 0;
    m = RAM(TMP8);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(HEALTH) = a;
    a = SFX_FILL;
    SET_NZ(a);
    CALL_0F(0x9B3E, func_F34C);
    mff_cycles(0x9B41, 63);
    a = x;
    c = 0;
    m = RAM(PLAYER_LEVEL);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(TMP14) = a;
    y = a;
    x = RAM(VRAM_BUFFER_LEN);
    RAM(VRAM_BUFFER + x) = (uint8_t)(a + 2);    /* the count: the tiles and both ends */
    x++;
    RAM(VRAM_BUFFER + x) = 0x20;
    x++;
    RAM(VRAM_BUFFER + x) = 0x49;
    x++;
    RAM(VRAM_BUFFER + x) = 0x19;
    x++;
    for (;;) {
        mff_cycles(0x9B66, 13);
        a = 0x18;
        RAM(VRAM_BUFFER + x) = a;
        x++;
        y--;
        if (y == 0)
            break;
        mff_cycles(0x9B6D, 1);
    }
    mff_cycles(0x9B6F, 28);
    RAM(VRAM_BUFFER + x) = 0x1A;
    x++;
    RAM(VRAM_BUFFER_LEN) = x;
    a = 0xFF;
    RAM(VRAM_REQUEST) = a;
    RAM(TEXT_CURSOR)++;
    SET_NZ(RAM(TEXT_CURSOR));
    SYNC_OUT;
    mff_rts();
}

/* $9BA8: sound A, text_cursor + 1 */
static void talk_gift_sound(void) {
    REGS_IN;

    mff_cycles(0x9BA8, 6);
    CALL_0F(0x9BA8, func_F34C);
    mff_cycles(0x9BAB, 11);
    RAM(TEXT_CURSOR)++;
    SET_NZ(RAM(TEXT_CURSOR));
    SYNC_OUT;
    mff_rts();
}

/*
 * talk_extra_life, $9B7E (bank 04; name proposed): text byte $FE: lives +
 * 1, tile $02 at $207C, sound $2D. No page uses it.
 * Caller: JMP at $9956.
 */
void func_9B7E_b2(void) {
    REGS_IN;

    mff_cycles(0x9B7E, 56);
    RAM(LIVES)++;
    x = RAM(VRAM_BUFFER_LEN);
    RAM(VRAM_BUFFER + x) = 0x01;
    x++;
    RAM(VRAM_BUFFER + x) = 0x20;
    x++;
    RAM(VRAM_BUFFER + x) = 0x7C;
    x++;
    RAM(VRAM_BUFFER + x) = 0x02;
    x++;
    RAM(VRAM_BUFFER_LEN) = x;
    RAM(VRAM_REQUEST) = 0xFF;
    a = SFX_LIFE;
    SET_NZ(a);
    mff_cycles(0x9BA2, 1);                      /* BNE, always taken */
    SYNC_OUT;
    talk_gift_sound();
}

/*
 * talk_extra_credit, $9BA4 (bank 04; name proposed): text byte $FD:
 * credits + 1, sound $30. No page uses it.
 * Caller: JMP at $994F.
 */
void func_9BA4_b2(void) {
    REGS_IN;

    mff_cycles(0x9BA4, 7);
    RAM(CREDITS)++;
    a = SFX_CREDIT;
    SET_NZ(a);
    SYNC_OUT;
    talk_gift_sound();
}
