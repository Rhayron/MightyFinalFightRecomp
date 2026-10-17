/*
 * flow_screens.c - bank 04's screen drawer and the screens' setups
 *
 * With banks 04/05 mapped (BANKS_SCREENS), the game draws a whole screen
 * with screen_draw (04:8000, X = the screen, flow.h SCREEN_*) and then
 * calls the screen's setup, which loads its palette, its CHR banks and
 * any sprite it shows. The routines here are in bank 04; the port names
 * them func_XXXX_b2 (16 KiB bank 2 = 8 KiB banks 04 and 05).
 */
#include "flow.h"

/* bank 04's tables */
#define SCREEN_SCRIPT_LO    0x8100  /* screen_draw's script pointers, low bytes, by screen */
#define SCREEN_SCRIPT_HI    0x810E  /* high bytes */
#define SCRIPT_BIT6         0x80FE  /* the byte $40 that BIT tests a script byte with */
#define SCRIPT_BIT7         0x80FF  /* the byte $80 */
#define PAL_CAPCOM          0x8924  /* 16 colors each */
#define PAL_TITLE_BG        0x898E  /* also the licence screen's */
#define PAL_TITLE_SPRITES   0x899E
#define PAL_STORY_TEXT      0x8A59  /* also story picture 1's background */
#define PAL_STORY_2_BG      0x8B01
#define PAL_STORY_2_SPRITES 0x8B11
#define PAL_FIGURE_SPRITES  0x8B88  /* story picture 1 and the first ending picture */
#define PAL_ENDING_2_SPRITES 0x8C48
#define PAL_ENDING_BG       0x9046  /* both ending pictures */

void func_8016_b2(void);
static void screen_draw_next(void);

/*
 * screen_draw (04:8000, proposed name)
 * Queues screen X's nametable writes in vram_buffer and has the NMI send
 * them. The screen is a script at [SCREEN_SCRIPT_LO/HI + X] in bank 04, a
 * list of strips ended by a 0 byte. Each strip:
 *   - a header: bits 0-5 the length; bit 7 a run counting up; bit 6 the
 *     strip starts where the last one ended (no address follows);
 *   - else an address byte: bit 6 a skip of (byte & $3F) from where the
 *     last strip ended; else bits 0-5 the high byte, then the low byte.
 *     Bit 7 of the address byte makes the strip one byte repeated;
 *   - the data: the length's bytes, or one byte for a run or a repeat.
 * Each strip goes into vram_buffer as [length, address high, low, bytes]
 * (nmi_vram_update's format). While it works render_off_depth is 1, so
 * the NMI leaves the buffer alone; once the buffer holds $50 bytes or more
 * it is sent: vram_request = $FF and a frame waited with render_off_depth
 * 0. At the end the rest is sent the same way and render_off_depth goes
 * back to what it was ($05).
 * In: X = screen. Uses $00-$05, $0E/$0F (the script pointer).
 * Callers (banks 04/05 mapped): main_task (0E:C579, C596, C5F0, C62B,
 * C66B, C6EC, C70D), the continue and game over (0E:DCAE, DCD3), the
 * ending (0E:DEB4, DEE7), the continue's select screen (0E:DF87) and the
 * round map between rounds (0E:DFC3, screen 8 + round from 0F:E15C).
 */
void func_8000_b2(void) {
    REGS_IN;

    mff_cycles(0x8000, 15);
    a = RAM(RENDER_OFF_DEPTH);
    RAM(TMP5) = a;
    a = 0x01;
    RAM(RENDER_OFF_DEPTH) = a;
    a = prg_read((uint16_t)(SCREEN_SCRIPT_LO + x));
    mff_cycles(0x800B, 7);
    RAM(TMP14) = a;
    a = prg_read((uint16_t)(SCREEN_SCRIPT_HI + x));
    mff_cycles(0x8010, 37);                 /* to the first LDA ($0E),Y at $8027 */
    RAM(TMP15) = a;
    x = RAM(VRAM_BUFFER_LEN);
    y = 0;
    SYNC_OUT;
    screen_draw_next();
}

/*
 * $8016: screen_draw's loop, entered by its JMP $8016 after each strip
 * (the lifted code makes that JMP a tail call by address, and the runtime
 * turns the repeated tail calls into a loop).
 * In: Y = the bytes the last strip used, X = the buffer's end.
 */
void func_8016_b2(void) {
    REGS_IN;

    mff_cycles(0x8016, 29);                 /* to the LDA ($0E),Y at $8027 */
    SYNC_OUT;
    screen_draw_next();
}

/*
 * screen_draw from $8016 on, its cycles to $8028 already charged: moves
 * the script pointer past the last strip, then queues the next strip or
 * ends.
 */
static void screen_draw_next(void) {
    REGS_IN;
    int more;

    RAM(TMP0) = y;
    a = RAM(TMP14);
    c = 0;
    m = RAM(TMP0);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(TMP14) = a;
    a = RAM(TMP15);
    m = 0x00;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(TMP15) = a;
    y = 0;
    a = mem_read((uint16_t)(RAM16(TMP14) + y));
    mff_cycles(0x8029, 2);
    if (a == 0) {
        /* the end: send what is queued, put render_off_depth back */
        mff_cycles(0x802B, 10);
        RAM(VRAM_BUFFER_LEN) = x;
        x = RAM(VRAM_BUFFER_LEN);
        c = 1;                              /* CPX #0 */
        if (x != 0) {
            mff_cycles(0x8033, 16);
            a = 0xFF;
            RAM(VRAM_REQUEST) = a;
            a = 0;
            SET_NZ(a);
            RAM(RENDER_OFF_DEPTH) = a;
            SYNC_OUT;
            mff_call_fn(0x003D, func_FF31, 0xE000);        /* task_wait_frame */
            SYNC_IN;
            mff_cycles(0x803E, 5);
            t = (uint8_t)(RAM(RENDER_OFF_DEPTH) + 1);
            RAM(RENDER_OFF_DEPTH) = t;
        } else {
            mff_cycles(0x8031, 1);
        }
        mff_cycles(0x8040, 12);
        a = RAM(TMP5);
        SET_NZ(a);
        RAM(RENDER_OFF_DEPTH) = a;
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0x8029, 1);

    /* the header: the length into the buffer */
    mff_cycles(0x8045, 21);
    y++;
    RAM(TMP1) = a;
    a &= 0x3F;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    a = RAM(TMP1);

    /* the address ($04 high, $03 low), three ways. Each way ends with a BNE
     * to $8099 that fails only if X wrapped to 0, and then the 6502 runs
     * on into the next way. */
    mff_cycles(0x8053, 2);
    more = (a & prg_read(SCRIPT_BIT6)) == 0;
    if (more) {
        mff_cycles(0x8053, 1);
    } else {
        /* where the last strip ended */
        mff_cycles(0x8055, 25);
        a = RAM(TMP4);
        RAM(VRAM_BUFFER + x) = a;
        RAM(TMP2) = a;
        x++;
        a = RAM(TMP3);
        RAM(VRAM_BUFFER + x) = a;
        x++;
        if (x != 0)
            mff_cycles(0x8063, 1);
        else
            more = 1;
    }
    if (more) {
        mff_cycles(0x8065, 5);
        a = mem_read((uint16_t)(RAM16(TMP14) + y));
        mff_cycles(0x8067, 9);
        y++;
        RAM(TMP2) = a;
        mff_cycles(0x806D, 2);
        more = (a & prg_read(SCRIPT_BIT6)) == 0;
        if (more) {
            mff_cycles(0x806D, 1);
        } else {
            /* a skip from where the last strip ended */
            mff_cycles(0x806F, 37);
            a &= 0x3F;
            c = 0;
            m = RAM(TMP3);
            w = a + m + c;
            c = w > 0xFF;
            a = (uint8_t)w;
            RAM(TMP3) = a;
            a = RAM(TMP4);
            m = 0x00;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(TMP4) = a;
            RAM(VRAM_BUFFER + x) = a;
            x++;
            a = RAM(TMP3);
            RAM(VRAM_BUFFER + x) = a;
            x++;
            if (x != 0)
                mff_cycles(0x8086, 1);
            else
                more = 1;
        }
    }
    if (more) {
        /* a full address: high (bits 0-5), then low */
        mff_cycles(0x8088, 17);
        a &= 0x3F;
        RAM(TMP4) = a;
        RAM(VRAM_BUFFER + x) = a;
        x++;
        a = mem_read((uint16_t)(RAM16(TMP14) + y));
        mff_cycles(0x8092, 12);
        y++;
        RAM(TMP3) = a;
        RAM(VRAM_BUFFER + x) = a;
        x++;
    }

    /* $04/$03 = where this strip ends */
    mff_cycles(0x8099, 28);
    a = RAM(TMP1);
    a &= 0x3F;
    c = 0;
    m = RAM(TMP3);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(TMP3) = a;
    a = RAM(TMP4);
    m = 0x00;
    w = a + m + c;
    a = (uint8_t)w;
    RAM(TMP4) = a;

    /* the data */
    a = RAM(TMP1);
    mff_cycles(0x80AD, 2);
    if ((a & prg_read(SCRIPT_BIT7)) != 0) {
        /* a run: the next byte, counting up */
        mff_cycles(0x80AD, 1);
        mff_cycles(0x80C7, 10);
        a &= 0x3F;
        RAM(TMP1) = a;
        a = mem_read((uint16_t)(RAM16(TMP14) + y));
        mff_cycles(0x80CD, 4);
        y++;
        c = 0;
        for (;;) {
            mff_cycles(0x80CF, 16);
            RAM(VRAM_BUFFER + x) = a;
            x++;
            m = 0x01;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            t = (uint8_t)(RAM(TMP1) - 1);
            RAM(TMP1) = t;
            z = t == 0;
            if (z) break;
            mff_cycles(0x80CF, 1);
        }
        mff_cycles(0x80D9, 2);
        mff_cycles(0x80D9, 1);              /* BEQ $80E6, taken: the DEC left Z set */
    } else {
        mff_cycles(0x80AF, 12);
        a &= 0x3F;
        RAM(TMP1) = a;
        a = RAM(TMP2);
        m = prg_read(SCRIPT_BIT7);
        v = (m >> 6) & 1;
        mff_cycles(0x80B8, 2);
        if ((a & m) != 0) {
            /* one byte, repeated */
            mff_cycles(0x80B8, 1);
            mff_cycles(0x80DB, 5);
            a = mem_read((uint16_t)(RAM16(TMP14) + y));
            mff_cycles(0x80DD, 2);
            y++;
            for (;;) {
                mff_cycles(0x80DE, 14);
                RAM(VRAM_BUFFER + x) = a;
                x++;
                t = (uint8_t)(RAM(TMP1) - 1);
                RAM(TMP1) = t;
                if (t == 0) break;
                mff_cycles(0x80DE, 1);
            }
        } else {
            /* the bytes as they are */
            for (;;) {
                mff_cycles(0x80BA, 5);
                a = mem_read((uint16_t)(RAM16(TMP14) + y));
                mff_cycles(0x80BC, 16);
                y++;
                RAM(VRAM_BUFFER + x) = a;
                x++;
                t = (uint8_t)(RAM(TMP1) - 1);
                RAM(TMP1) = t;
                z = t == 0;
                if (z) break;
                mff_cycles(0x80BA, 1);
            }
            mff_cycles(0x80C5, 2);
            mff_cycles(0x80C5, 1);          /* BEQ $80E6, taken: the DEC left Z set */
        }
    }

    /* send the buffer once it holds $50 bytes */
    mff_cycles(0x80E6, 4);
    c = x >= 0x50;
    SET_NZ(x - 0x50);
    if (c) {
        mff_cycles(0x80EA, 19);
        RAM(VRAM_BUFFER_LEN) = x;
        a = 0xFF;
        RAM(VRAM_REQUEST) = a;
        a = 0;
        SET_NZ(a);
        RAM(RENDER_OFF_DEPTH) = a;
        SYNC_OUT;
        mff_call_fn(0x00F6, func_FF31, 0xE000);            /* task_wait_frame */
        SYNC_IN;
        mff_cycles(0x80F7, 8);
        t = (uint8_t)(RAM(RENDER_OFF_DEPTH) + 1);
        RAM(RENDER_OFF_DEPTH) = t;
        x = RAM(VRAM_BUFFER_LEN);
        SET_NZ(x);
    } else {
        mff_cycles(0x80E8, 1);
    }
    mff_cycles(0x80FB, 3);                  /* JMP $8016 */
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8016, 2); call_by_address_tail(0x8016, 2); return;
}

/*
 * capcom_setup (04:8900, proposed name)
 * CAPCOM PRESENTS: the same 16 colors in the background and the sprite
 * palettes (palette_buf and palette_base), palette_request = 0, CHR R0/R1
 * = $78/$7A, then palette_fade_in ($C362), which uploads them darkened
 * and brightens them.
 * Callers: main_task (0E:C59C).
 */
void func_8900_b2(void) {
    REGS_IN;

    mff_cycles(0x8900, 2);
    y = 0x0F;
    for (;;) {
        mff_cycles(0x8902, 4);
        a = prg_read((uint16_t)(PAL_CAPCOM + y));
        mff_cycles(0x8905, 24);
        RAM(PALETTE_BUF + y) = a;
        RAM(PALETTE_BASE + y) = a;
        RAM(PALETTE_BUF + 16 + y) = a;
        RAM(PALETTE_BASE + 16 + y) = a;
        y--;
        if (y & 0x80) break;
        mff_cycles(0x8902, 1);
    }
    mff_cycles(0x8914, 21);
    a = 0;
    RAM(PALETTE_REQUEST) = a;
    a = 0x78;
    RAM(CHR_BANKS) = a;
    a = 0x7A;
    SET_NZ(a);
    RAM(CHR_BANKS + 1) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0922, 0xC362, 2)) return;           /* palette_fade_in */
    SYNC_IN;
    mff_cycles(0x8923, 6);
    SYNC_OUT;
    mff_rts();
}

/*
 * title_setup (04:8940, proposed name)
 * The title: background colors from PAL_TITLE_BG, sprite colors from
 * PAL_TITLE_SPRITES, palette_request = $FF (Y after the loop), CHR R0/R1
 * = $74/$76 and R2 = $3C, and the logo's object in slot 13: state and
 * flags $80, animation $70 at ($30, $37) in screen coordinates.
 * Callers: main_task's title_screen (0E:C66E).
 */
void func_8940_b2(void) {
    REGS_IN;

    mff_cycles(0x8940, 2);
    y = 0x0F;
    for (;;) {
        mff_cycles(0x8942, 4);
        a = prg_read((uint16_t)(PAL_TITLE_BG + y));
        mff_cycles(0x8945, 14);
        RAM(PALETTE_BUF + y) = a;
        RAM(PALETTE_BASE + y) = a;
        a = prg_read((uint16_t)(PAL_TITLE_SPRITES + y));
        mff_cycles(0x894E, 14);
        RAM(PALETTE_BUF + 16 + y) = a;
        RAM(PALETTE_BASE + 16 + y) = a;
        y--;
        if (y & 0x80) break;
        mff_cycles(0x8942, 1);
    }
    mff_cycles(0x8957, 74);
    RAM(PALETTE_REQUEST) = y;
    a = 0x74;
    RAM(CHR_BANKS) = a;
    a = 0x76;
    RAM(CHR_BANKS + 1) = a;
    a = 0x3C;
    RAM(CHR_BANKS + 2) = a;
    a = 0x80;
    RAM(OBJ_STATE + 13) = a;
    RAM(OBJ_FLAGS + 13) = a;
    a = 0;
    RAM(OBJ_X_HI + 13) = a;
    RAM(OBJ_Y_HI + 13) = a;
    RAM(OBJ_ANIM_TIMER + 13) = a;
    RAM(OBJ_ANIM_FRAME + 13) = a;
    RAM(OBJ_ID + 13) = a;
    a = 0x70;
    RAM(OBJ_ANIM + 13) = a;
    a = 0x30;
    RAM(OBJ_X + 13) = a;
    a = 0x37;
    SET_NZ(a);
    RAM(OBJ_Y + 13) = a;
    SYNC_OUT;
    mff_rts();
}

/*
 * licence_setup (04:89C0, proposed name)
 * LICENSED BY NINTENDO: the title's background colors, palette_request =
 * $FF, CHR R0/R1 = $74/$76.
 * Callers: main_task (0E:C57F).
 */
void func_89C0_b2(void) {
    REGS_IN;

    mff_cycles(0x89C0, 2);
    y = 0x0F;
    for (;;) {
        mff_cycles(0x89C2, 4);
        a = prg_read((uint16_t)(PAL_TITLE_BG + y));
        mff_cycles(0x89C5, 14);
        RAM(PALETTE_BUF + y) = a;
        RAM(PALETTE_BASE + y) = a;
        y--;
        if (y & 0x80) break;
        mff_cycles(0x89C2, 1);
    }
    mff_cycles(0x89CE, 19);
    RAM(PALETTE_REQUEST) = y;
    a = 0x74;
    RAM(CHR_BANKS) = a;
    a = 0x76;
    SET_NZ(a);
    RAM(CHR_BANKS + 1) = a;
    SYNC_OUT;
    mff_rts();
}

/*
 * story_text_setup (04:8A40, proposed name)
 * The story's text pages: background colors from PAL_STORY_TEXT,
 * palette_request = $FF, CHR R0/R1 = $78/$62.
 * Callers: story_text_screen (0E:C898).
 */
void func_8A40_b2(void) {
    REGS_IN;

    mff_cycles(0x8A40, 2);
    y = 0x0F;
    for (;;) {
        mff_cycles(0x8A42, 4);
        a = prg_read((uint16_t)(PAL_STORY_TEXT + y));
        mff_cycles(0x8A45, 14);
        RAM(PALETTE_BUF + y) = a;
        RAM(PALETTE_BASE + y) = a;
        y--;
        if (y & 0x80) break;
        mff_cycles(0x8A42, 1);
    }
    mff_cycles(0x8A4E, 19);
    RAM(PALETTE_REQUEST) = y;
    a = 0x78;
    RAM(CHR_BANKS) = a;
    a = 0x62;
    SET_NZ(a);
    RAM(CHR_BANKS + 1) = a;
    SYNC_OUT;
    mff_rts();
}

/*
 * story_2_setup (04:8AC0, proposed name)
 * Story page 3's picture (screen 7): background and sprite colors, then
 * palette_buf+$12 = $36 and palette_base+$13 = $26 (two different entries,
 * one in each copy), palette_request = $FF, CHR R0/R1 = $78/$56 and R2 =
 * $35, and slot 0 at ($60, $47) with animation $7A (player_set_anim).
 * Callers: main_task's story (0E:C631).
 */
void func_8AC0_b2(void) {
    REGS_IN;

    mff_cycles(0x8AC0, 2);
    y = 0x0F;
    for (;;) {
        mff_cycles(0x8AC2, 4);
        a = prg_read((uint16_t)(PAL_STORY_2_BG + y));
        mff_cycles(0x8AC5, 14);
        RAM(PALETTE_BUF + y) = a;
        RAM(PALETTE_BASE + y) = a;
        a = prg_read((uint16_t)(PAL_STORY_2_SPRITES + y));
        mff_cycles(0x8ACE, 14);
        RAM(PALETTE_BUF + 16 + y) = a;
        RAM(PALETTE_BASE + 16 + y) = a;
        y--;
        if (y & 0x80) break;
        mff_cycles(0x8AC2, 1);
    }
    mff_cycles(0x8AD7, 52);
    a = 0x36;
    RAM(PALETTE_BUF + 18) = a;
    a = 0x26;
    RAM(PALETTE_BASE + 19) = a;
    a = 0xFF;
    RAM(PALETTE_REQUEST) = a;
    a = 0x78;
    RAM(CHR_BANKS) = a;
    a = 0x56;
    RAM(CHR_BANKS + 1) = a;
    a = 0x35;
    RAM(CHR_BANKS + 2) = a;
    a = 0x60;
    RAM(OBJ_X) = a;
    a = 0x47;
    RAM(OBJ_Y) = a;
    a = 0x7A;
    SET_NZ(a);
    SYNC_OUT;
    if (!mff_call_fn(0x0AFF, func_F294, 0xE000)) return;   /* player_set_anim */
    SYNC_IN;
    mff_cycles(0x8B00, 6);
    SYNC_OUT;
    mff_rts();
}

/*
 * story_1_setup (04:8B40, proposed name)
 * Story page 1's picture (screen 6): the text pages' background colors
 * and PAL_FIGURE_SPRITES, palette_request = $FF, CHR R0/R1 = $78/$62 and
 * R3 = $37, and slot 0 live (state and flags $80) at ($7D, $87) with
 * animation $7D; its x/y high bytes and obj_id 0.
 * Callers: main_task's story (0E:C5F6).
 */
void func_8B40_b2(void) {
    REGS_IN;

    mff_cycles(0x8B40, 2);
    y = 0x0F;
    for (;;) {
        mff_cycles(0x8B42, 4);
        a = prg_read((uint16_t)(PAL_STORY_TEXT + y));
        mff_cycles(0x8B45, 14);
        RAM(PALETTE_BUF + y) = a;
        RAM(PALETTE_BASE + y) = a;
        a = prg_read((uint16_t)(PAL_FIGURE_SPRITES + y));
        mff_cycles(0x8B4E, 14);
        RAM(PALETTE_BUF + 16 + y) = a;
        RAM(PALETTE_BASE + 16 + y) = a;
        y--;
        if (y & 0x80) break;
        mff_cycles(0x8B42, 1);
    }
    mff_cycles(0x8B57, 48);
    RAM(PALETTE_REQUEST) = y;
    a = 0x78;
    RAM(CHR_BANKS) = a;
    a = 0x62;
    RAM(CHR_BANKS + 1) = a;
    a = 0x37;
    RAM(CHR_BANKS + 3) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    RAM(OBJ_FLAGS) = a;
    a = 0x7D;
    RAM(OBJ_X) = a;
    a = 0x87;
    RAM(OBJ_Y) = a;
    a = 0x7D;
    SET_NZ(a);
    SYNC_OUT;
    if (!mff_call_fn(0x0B7B, func_F294, 0xE000)) return;   /* player_set_anim */
    SYNC_IN;
    mff_cycles(0x8B7C, 20);
    a = 0;
    SET_NZ(a);
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y_HI) = a;
    RAM(OBJ_ID) = a;
    SYNC_OUT;
    mff_rts();
}

/*
 * ending_2_setup (04:8C00, proposed name)
 * The second ending picture (screen $0D): PAL_ENDING_BG and
 * PAL_ENDING_2_SPRITES, palette_request = $FF, CHR R0/R1 = $78/$62 and
 * R3 = $36, and slot 0 live at ($5C, $90) with animation $7D.
 * Callers: the ending in player_state_transition (0E:DEEF).
 */
void func_8C00_b2(void) {
    REGS_IN;

    mff_cycles(0x8C00, 2);
    y = 0x0F;
    for (;;) {
        mff_cycles(0x8C02, 4);
        a = prg_read((uint16_t)(PAL_ENDING_BG + y));
        mff_cycles(0x8C05, 14);
        RAM(PALETTE_BUF + y) = a;
        RAM(PALETTE_BASE + y) = a;
        a = prg_read((uint16_t)(PAL_ENDING_2_SPRITES + y));
        mff_cycles(0x8C0E, 14);
        RAM(PALETTE_BUF + 16 + y) = a;
        RAM(PALETTE_BASE + 16 + y) = a;
        y--;
        if (y & 0x80) break;
        mff_cycles(0x8C02, 1);
    }
    mff_cycles(0x8C17, 48);
    RAM(PALETTE_REQUEST) = y;
    a = 0x78;
    RAM(CHR_BANKS) = a;
    a = 0x62;
    RAM(CHR_BANKS + 1) = a;
    a = 0x36;
    RAM(CHR_BANKS + 3) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    RAM(OBJ_FLAGS) = a;
    a = 0x5C;
    RAM(OBJ_X) = a;
    a = 0x90;
    RAM(OBJ_Y) = a;
    a = 0x7D;
    SET_NZ(a);
    SYNC_OUT;
    if (!mff_call_fn(0x0C3B, func_F294, 0xE000)) return;   /* player_set_anim */
    SYNC_IN;
    mff_cycles(0x8C3C, 20);
    a = 0;
    SET_NZ(a);
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y_HI) = a;
    RAM(OBJ_ID) = a;
    SYNC_OUT;
    mff_rts();
}

/*
 * ending_1_setup (04:9000, proposed name)
 * The first ending picture (screen 6): PAL_ENDING_BG and
 * PAL_FIGURE_SPRITES, palette_request = $FF, CHR R0/R1 = $78/$62 and R3 =
 * $37, and slot 0 live at ($80, $84) with animation $7D.
 * Callers: the ending in player_state_transition (0E:DEBA).
 */
void func_9000_b2(void) {
    REGS_IN;

    mff_cycles(0x9000, 2);
    y = 0x0F;
    for (;;) {
        mff_cycles(0x9002, 4);
        a = prg_read((uint16_t)(PAL_ENDING_BG + y));
        mff_cycles(0x9005, 14);
        RAM(PALETTE_BUF + y) = a;
        RAM(PALETTE_BASE + y) = a;
        a = prg_read((uint16_t)(PAL_FIGURE_SPRITES + y));
        mff_cycles(0x900E, 14);
        RAM(PALETTE_BUF + 16 + y) = a;
        RAM(PALETTE_BASE + 16 + y) = a;
        y--;
        if (y & 0x80) break;
        mff_cycles(0x9002, 1);
    }
    mff_cycles(0x9017, 46);
    RAM(PALETTE_REQUEST) = y;
    a = 0x78;
    RAM(CHR_BANKS) = a;
    a = 0x62;
    RAM(CHR_BANKS + 1) = a;
    a = 0x37;
    RAM(CHR_BANKS + 3) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    RAM(OBJ_FLAGS) = a;
    RAM(OBJ_X) = a;
    a = 0x84;
    RAM(OBJ_Y) = a;
    a = 0x7D;
    SET_NZ(a);
    SYNC_OUT;
    if (!mff_call_fn(0x1039, func_F294, 0xE000)) return;   /* player_set_anim */
    SYNC_IN;
    mff_cycles(0x903A, 20);
    a = 0;
    SET_NZ(a);
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y_HI) = a;
    RAM(OBJ_ID) = a;
    SYNC_OUT;
    mff_rts();
}

/* the credits screen's colors: background black with white text, and the
 * player's 8 sprite colors by player_id (the last index of each from
 * PLAYER_PAL_END, as player_load_graphics) */
#define PAL_CREDITS_BG      0x9636
#define PAL_CREDITS_PLAYERS 0x9646
/* its music by lives: a round's tune, $00-$04 (lives mod 5) */
#define CREDITS_MUSIC       0x965E

/*
 * credits_setup (04:9600, proposed name)
 * The screen of the full ending's credits (stage_ending.c types them):
 * PAL_CREDITS_BG, the player's sprite colors at sprite palettes 0-1,
 * palette_request = $FF, CHR R0/R1 = $78/$7A, and the music
 * CREDITS_MUSIC[lives].
 * Out: Y = lives. Caller: player_state_transition after the last round,
 * when no continue was used (0E:DF22).
 */
void func_9600_b2(void) {
    REGS_IN;

    mff_cycles(0x9600, 2);
    y = 0x0F;
    for (;;) {
        mff_cycles(0x9602, 4);
        a = prg_read((uint16_t)(PAL_CREDITS_BG + y));
        mff_cycles(0x9605, 14);
        RAM(PALETTE_BUF + y) = a;
        RAM(PALETTE_BASE + y) = a;
        y--;
        if (y & 0x80) break;
        mff_cycles(0x9602, 1);
    }
    mff_cycles(0x960E, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(PLAYER_PAL_END + y));
    mff_cycles(0x9613, 4);
    x = a;
    y = 0x07;
    for (;;) {
        mff_cycles(0x9616, 4);
        a = prg_read((uint16_t)(PAL_CREDITS_PLAYERS + x));
        mff_cycles(0x9619, 16);
        RAM(PALETTE_BUF + 16 + y) = a;
        RAM(PALETTE_BASE + 16 + y) = a;
        x--;
        y--;
        if (y & 0x80) break;
        mff_cycles(0x9616, 1);
    }
    mff_cycles(0x9623, 20);
    RAM(PALETTE_REQUEST) = y;
    a = 0x78;
    RAM(CHR_BANKS) = a;
    a = 0x7A;
    RAM(CHR_BANKS + 1) = a;
    y = RAM(LIVES);
    a = prg_read((uint16_t)(CREDITS_MUSIC + y));
    SET_NZ(a);
    mff_cycles(0x9632, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1634, func_F34A, 0xE000)) return;   /* sound_queue_push */
    SYNC_IN;
    mff_cycles(0x9635, 6);
    SYNC_OUT;
    mff_rts();
}
