/*
 * flow_screen.c - the services every screen change goes through (fixed bank
 * 0E): clearing the screen, fading the palette, hiding the unused sprites,
 * filling a nametable.
 *
 * A screen change in this game is: screen_clear (fade out, wait a frame,
 * rendering off, sprites hidden, $0400-$05FF cleared, scroll 0, nametable
 * $2000 blank), then the new screen drawn by bank 04's screen_draw while
 * rendering is still off, then rendering_on. palette_fade_in brings a
 * gameplay screen up from black; the screens of bank 04 mostly appear at
 * once.
 *
 * The fades work on palette_base ($0620, the palette as loaded) and write
 * palette_buf ($0600), which the NMI uploads when palette_request ($18) is
 * set: each step copies palette_base over palette_buf and subtracts the
 * darkness from every color, $0F (black) below 0. Their state is in
 * scratch: TMP15 the darkness, TMP13 its change per step, TMP14 the frames
 * a step lasts.
 */
#include "flow.h"

/*
 * ppu_fill_nametable, $C2D1 (name proposed): fills a nametable with one
 * tile and its attribute table with one byte, writing the PPU directly
 * (rendering must be off).
 * In: A = PPU page of the nametable ($20/$24/$28/$2C), X = tile, Y =
 * attribute byte. A page below $20 (the pattern tables) would fill Y pages
 * and no attributes: no caller asks for that, and its LDX $02 ($C2EF) is
 * data in the coverage map.
 * Out: X = the tile, Y = the attribute byte, A = page + 3, N/Z from X, C/V
 * from that ADC; TMP0-TMP2 = A, X, Y as given.
 * Callers: screen_clear_now (0E:C881, page $20), player_state_continue
 * (0E:DC9A) and reset (0F:FE9D, FEA6: both nametables).
 */
void func_C2D1(void)
{
    REGS_IN;

    mff_cycles(0xC2D1, 13);
    RAM(TMP0) = a;
    RAM(TMP1) = x;
    RAM(TMP2) = y;
    a = hw_read(PPU_STATUS);                    /* resets the address latch */
    mff_cycles(0xC2DA, 9);
    a = RAM(PPUCTRL_SHADOW) & 0xFE;             /* nametable select bit 0 off */
    hw_write(PPU_CTRL, a);
    mff_cycles(0xC2E1, 7);
    a = RAM(TMP0);
    hw_write(PPU_ADDR, a);
    mff_cycles(0xC2E6, 6);
    y = 0x00;
    hw_write(PPU_ADDR, y);
    mff_cycles(0xC2EB, 6);
    x = 0x04;                                   /* a nametable is 4 pages */
    if (a < 0x20) {
        mff_cycles(0xC2F1, 3);
        x = RAM(TMP2);
    } else {
        mff_cycles(0xC2EF, 1);
    }

    /* X pages of 256 tiles (Y counts 256 down from 0) */
    mff_cycles(0xC2F3, 5);
    y = 0x00;
    a = RAM(TMP1);
    do {
        do {
            mff_cycles(0xC2F7, 4);
            hw_write(PPU_DATA, a);
            mff_cycles(0xC2FA, 4);
            y--;
            if (y != 0) mff_cycles(0xC2F7, 1);
        } while (y != 0);
        mff_cycles(0xC2FD, 4);
        x--;
        if (x != 0) mff_cycles(0xC2F7, 2);      /* the BNE back crosses a page */
    } while (x != 0);

    mff_cycles(0xC300, 10);
    y = RAM(TMP2);
    a = RAM(TMP0);
    c = a >= 0x20;
    if (c) {
        /* the 64 attribute bytes at page + 3, $C0 (ADC #2 with C set) */
        mff_cycles(0xC308, 6);
        m = 0x02;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        hw_write(PPU_ADDR, a);
        mff_cycles(0xC30D, 6);
        a = 0xC0;
        hw_write(PPU_ADDR, a);
        mff_cycles(0xC312, 2);
        x = 0x40;
        do {
            mff_cycles(0xC314, 4);
            hw_write(PPU_DATA, y);
            mff_cycles(0xC317, 4);
            x--;
            if (x != 0) mff_cycles(0xC314, 1);
        } while (x != 0);
    } else {
        mff_cycles(0xC306, 1);
    }
    mff_cycles(0xC31A, 9);
    x = RAM(TMP1);
    SET_NZ(x);
    SYNC_OUT;
    mff_rts();
}

/*
 * sprites_hide_rest, $C31D (name proposed): the end of a frame's sprite
 * drawing. Every OAM entry from oam_index ($97) to the end of oam_buffer
 * gets Y = $F8, off screen; when nothing was drawn (oam_index = 4) sprite 0
 * goes too, the only place it is ever hidden.
 * In: oam_index. Out: A = $F8, X = 0, C = (oam_index >= 4), N clear, Z set;
 * Y and V kept.
 * Callers: gameplay_draw (0E:C9A1), palette_fade_in (0E:C362), the title
 * loop (0E:C685), screen_clear_now (0E:C869, C884), player_state_continue
 * (0E:DC8E, DC9D), player_state_transition (0E:DE79, DEDB, DF13), the
 * scenes' redraw (0F:F412) and reset (0F:FE94).
 */
void func_C31D(void)
{
    REGS_IN;

    mff_cycles(0xC31D, 7);
    x = RAM(OAM_INDEX);
    c = x >= 4;
    if (x == 4) {
        mff_cycles(0xC323, 6);
        a = OAM_Y_HIDDEN;
        RAM(OAM_BUFFER) = a;
    } else {
        mff_cycles(0xC321, 1);
    }
    mff_cycles(0xC328, 2);
    a = OAM_Y_HIDDEN;
    do {                                        /* 4 bytes a sprite, to the end */
        mff_cycles(0xC32A, 15);
        RAM(OAM_BUFFER + x) = a;
        x += 4;
        if (x != 0) mff_cycles(0xC32A, 1);
    } while (x != 0);
    SET_NZ(x);
    mff_cycles(0xC333, 6);
    SYNC_OUT;
    mff_rts();
}

/*
 * The fade's steps, $C379-$C3B3, shared by palette_fade_in and
 * palette_fade_out (both set TMP13-TMP15 and run into $C379): darken
 * palette_buf from palette_base by TMP15, upload it, wait TMP14 frames, add
 * TMP13 to TMP15; stop when TMP15 reaches $50 (out) or goes negative (in).
 * Returns (RTS) with Y = $FF, A = TMP15.
 */
static void palette_fade_steps(void)
{
    REGS_IN;

    for (;;) {
        /* palette_buf = palette_base - darkness, black below 0 */
        mff_cycles(0xC379, 2);
        y = 0x1F;
        do {
            mff_cycles(0xC37B, 13);
            a = RAM(PALETTE_BASE + y);
            RAM(PALETTE_BUF + y) = a;
            y--;
            if (!(y & 0x80)) mff_cycles(0xC37B, 1);
        } while (!(y & 0x80));
        mff_cycles(0xC384, 2);
        y = 0x1F;
        do {
            mff_cycles(0xC386, 11);
            a = RAM(PALETTE_BUF + y);
            c = 1;
            m = RAM(TMP15);
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            c = w >= 0;
            a = (uint8_t)w;
            if (a & 0x80) {
                mff_cycles(0xC38E, 2);
                a = PAL_BLACK;
            } else {
                mff_cycles(0xC38C, 1);
            }
            mff_cycles(0xC390, 9);
            RAM(PALETTE_BUF + y) = a;
            y--;
            if (!(y & 0x80)) mff_cycles(0xC386, 1);
        } while (!(y & 0x80));
        mff_cycles(0xC396, 6);
        RAM(PALETTE_REQUEST) = y;

        /* TMP14 frames */
        a = RAM(TMP14);
        SET_NZ(a);
        for (;;) {
            mff_cycles(0xC39A, 9);
            mff_push(a);
            SYNC_OUT;
            mff_call_fn(0x039D, func_FF31, 0xE000);     /* task_wait_frame */
            SYNC_IN;
            mff_cycles(0xC39E, 10);
            a = mff_pull();
            c = 1;
            m = 0x01;
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            c = w >= 0;
            a = (uint8_t)w;
            SET_NZ(a);
            if (z) break;
            mff_cycles(0xC39A, 1);
        }

        /* the next darkness */
        mff_cycles(0xC3A4, 15);
        a = RAM(TMP15);
        c = 0;
        m = RAM(TMP13);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        RAM(TMP15) = a;
        c = a >= 0x50;
        SET_NZ(a - 0x50);
        if (z) {                                /* faded out */
            mff_cycles(0xC3AD, 1);
            break;
        }
        mff_cycles(0xC3AF, 5);
        a = RAM(TMP15);
        SET_NZ(a);
        if (n) break;                           /* faded in */
        mff_cycles(0xC379, 1);
    }
    mff_cycles(0xC3B3, 6);
    SYNC_OUT;
    mff_rts();
}

/*
 * palette_fade_in, $C362 (name proposed): brings the screen up from black.
 * Hides the unused sprites, waits a frame, then four steps of 4 frames each
 * at darkness $30, $20, $10 and 0 (TMP13 = -$10).
 * Out: A = $F0, X = $F0, Y = $FF; TMP13-TMP15 used.
 * Callers: bank 04's CAPCOM PRESENTS setup (04:8920), the new game's round
 * start (0E:C7A4) and player_state_transition's area start (0F:E055).
 */
void func_C362(void)
{
    REGS_IN;

    mff_cycles(0xC362, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0364, 0xC31D, -1)) return;   /* sprites_hide_rest */
    SYNC_IN;
    mff_cycles(0xC365, 6);
    SYNC_OUT;
    mff_call_fn(0x0367, func_FF31, 0xE000);         /* task_wait_frame */
    SYNC_IN;
    mff_cycles(0xC368, 6);
    a = 0x30;
    x = 0xF0;
    mff_cycles(0xC36C, 1);                          /* BNE $C371, always taken */
    mff_cycles(0xC371, 11);
    RAM(TMP15) = a;
    RAM(TMP13) = x;
    y = 0x04;
    RAM(TMP14) = y;
    SYNC_OUT;
    palette_fade_steps();
}

/*
 * palette_fade_out, $C36E (name proposed): fades the screen to black in four
 * steps of 4 frames each at darkness $10, $20, $30 and $40 (TMP13 = $10).
 * Out: A = $50, Y = $FF; TMP13-TMP15 used.
 * Callers: screen_clear (0E:C85C), the title's attract timeout (0E:C6A1),
 * player_state_continue (0E:DC75) and player_state_transition (0E:DE72,
 * DED4, DF0C).
 */
void func_C36E(void)
{
    REGS_IN;

    mff_cycles(0xC36E, 15);
    a = 0x10;
    x = a;
    RAM(TMP15) = a;
    RAM(TMP13) = x;
    y = 0x04;
    RAM(TMP14) = y;
    SYNC_OUT;
    palette_fade_steps();
}

/*
 * screen_clear, $C85C: palette_fade_out, then screen_clear_now.
 * Callers: main_task before each of its screens (0E:C56A, C587, C5E1,
 * C61C, C65C, C6DD, C6FB, C71B), story_text_screen (0E:C888),
 * player_state_continue (0E:DCC4) and player_state_transition's area start
 * (0E:DF92, DFD3).
 */
void func_C85C(void)
{
    REGS_IN;

    mff_cycles(0xC85C, 6);
    SYNC_OUT;
    if (!mff_call_at(0x085E, 0xC36E, -1)) return;   /* palette_fade_out */
    func_C85F();                                    /* runs into screen_clear_now */
}

/*
 * screen_clear_now, $C85F (name proposed): waits a frame, turns rendering
 * off (rendering_off: the caller turns it back on), hides every sprite,
 * clears $0400-$05FF (clear_ram_400_5ff: the upper object tables), sets
 * the scroll and draw_parity to 0 and blanks nametable $2000 (tile 0,
 * attributes 0).
 * Out: A = $23, X = 0, Y = 0 (ppu_fill_nametable), then sprites_hide_rest's
 * flags.
 * Callers: screen_clear (by falling in), screen_clear_banks (0E:C8A6) and
 * player_state_transition's area start (0E:DF7A, DFB2).
 */
void func_C85F(void)
{
    REGS_IN;

    mff_cycles(0xC85F, 6);
    SYNC_OUT;
    mff_call_fn(0x0861, func_FF31, 0xE000);         /* task_wait_frame */
    SYNC_IN;
    mff_cycles(0xC862, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0864, 0xC265, -1)) return;   /* rendering_off */
    SYNC_IN;
    mff_cycles(0xC865, 11);
    a = 0x04;                                       /* no sprite drawn */
    SET_NZ(a);
    RAM(OAM_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_at(0x086B, 0xC31D, -1)) return;   /* sprites_hide_rest */
    SYNC_IN;
    mff_cycles(0xC86C, 6);
    SYNC_OUT;
    if (!mff_call_at(0x086E, 0xC844, -1)) return;   /* clear_ram_400_5ff */
    SYNC_IN;
    mff_cycles(0xC86F, 29);
    a = 0x00;
    RAM(SCROLL_X) = a;
    RAM(SCROLL_Y) = a;
    RAM(SCROLL_NT) = a;
    RAM(SCROLL_Y_HI) = a;
    RAM(DRAW_PARITY) = a;
    a = 0x20;                                       /* nametable $2000 */
    x = 0x00;                                       /* tile 0 */
    y = 0x00;                                       /* attributes 0 */
    SET_NZ(y);
    SYNC_OUT;
    if (!mff_call_at(0x0883, 0xC2D1, -1)) return;   /* ppu_fill_nametable */
    SYNC_IN;
    mff_cycles(0xC884, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0886, 0xC31D, -1)) return;   /* sprites_hide_rest */
    SYNC_IN;
    mff_cycles(0xC887, 6);
    SYNC_OUT;
    mff_rts();
}

/*
 * story_text_screen, $C888 (name proposed): the screen of an intro text
 * page. screen_clear, rendering_on, banks 04/05 for the text palette and
 * CHR (04:8A40), then banks 00/01 back for the page drawer ($9A00 in bank
 * 00, which main_task calls next).
 * Callers: main_task's story, before intro pages 0, 2 and 4 (0E:C5CE, C60E,
 * C649).
 */
void func_C888(void)
{
    REGS_IN;

    mff_cycles(0xC888, 6);
    SYNC_OUT;
    if (!mff_call_at(0x088A, 0xC85C, -1)) return;   /* screen_clear */
    SYNC_IN;
    mff_cycles(0xC88B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x088D, 0xC26F, -1)) return;   /* rendering_on */
    SYNC_IN;
    mff_cycles(0xC88E, 16);
    x = BANKS_SCREENS;
    RAM(BANK_REQUEST) = x;
    x++;
    SET_NZ(x);
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0897, func_FF4D, 0xE000)) return;    /* bank_switch */
    SYNC_IN;
    mff_cycles(0xC898, 6);
    SYNC_OUT;
    if (!mff_call_at(0x089A, 0x8A40, -1)) return;   /* 04:8A40 story text setup */
    SYNC_IN;
    mff_cycles(0xC89B, 16);
    x = BANKS_PLAYER;
    RAM(BANK_REQUEST) = x;
    x++;
    SET_NZ(x);
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x08A4, func_FF4D, 0xE000)) return;    /* bank_switch */
    SYNC_IN;
    mff_cycles(0xC8A5, 6);
    SYNC_OUT;
    mff_rts();
}

/*
 * screen_clear_banks, $C8A6 (name proposed): screen_clear_now (no fade),
 * then banks 04/05 mapped for screen_draw.
 * Out: X = 5, bank_switch's A and flags.
 * Callers: player_state_transition (0E:DEB1, DEE4, DF1C).
 */
void func_C8A6(void)
{
    REGS_IN;

    mff_cycles(0xC8A6, 6);
    SYNC_OUT;
    if (!mff_call_at(0x08A8, 0xC85F, -1)) return;   /* screen_clear_now */
    SYNC_IN;
    mff_cycles(0xC8A9, 16);
    x = BANKS_SCREENS;
    RAM(BANK_REQUEST) = x;
    x++;
    SET_NZ(x);
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x08B2, func_FF4D, 0xE000)) return;    /* bank_switch */
    SYNC_IN;
    mff_cycles(0xC8B3, 6);
    SYNC_OUT;
    mff_rts();
}
