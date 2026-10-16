/*
 * stage_columns.c - streaming the background (docs/assets.md, "How the game
 * draws it")
 *
 * The playfield is nametable rows 4-23, 32 columns that wrap (horizontal
 * mirroring). Each time the camera crosses a multiple of 8 pixels, the
 * column writer puts the world column that came into view into the
 * nametable: bg_column ($24) is the nametable column (0-$1F) and bg_screen
 * ($29) the screen of the area it belongs to, so the last column written
 * is world column 32 * bg_screen + bg_column. When an area starts,
 * round_start_wait and its twin (0E:C76C, 0F:E001) call the writer once a
 * frame for 33 columns.
 *
 * The world comes from the round's bank at $A000 (round_bank $C1F8 by
 * round_tables $22): each area has a list of screens ($AE60/$AE90/$AEC0 by
 * sub_area), a screen is 8x8 metatiles at $AF00 + 64 * screen (rows 0-4
 * are shown), a metatile is four 16x16 blocks at $B700 + 4 * metatile
 * (TL, TR, BL, BR), and a block is four tiles at $BB00/$BC00/$BD00/$BE00
 * + block with its palette in the low bits of $BF00 + block.
 *
 * A column goes to the VRAM buffer at $0780 as a list the NMI writes with
 * the +32 increment (vram_column_request $1A, list format of 0E:C201):
 *     [$20, $80 + column, 20, 21 tiles]   rows 4-24; rows 4 and 24 forced
 *                                          to tile 0 (blank), so row 0 of
 *                                          the screen data never shows
 *     [$23, $C8 + 8 * row + column / 4, 0, attribute] x 5   odd columns only
 *     $FF
 * On an odd column each metatile row's attribute byte is rewritten: the
 * new metatile's left half (column 4n+1) or right half (4n+3) over the old
 * byte, kept in attr_shadow ($0640 + map_cell).
 */
#include "stage.h"

/* bg_column_write and bg_column_turn: the column step by direction bit 0,
 * -1 (left) or +1 (right), at $E601 */
#define K_COLUMN_STEP   0xE601
/* the attribute half a column writes, by column & 3: $33 keeps the left
 * half of the new byte (bits 0-1 and 4-5), $CC the right ($E603), and
 * $E607 the other half from the old byte */
#define K_ATTR_NEW_MASK 0xE603
#define K_ATTR_OLD_MASK 0xE607
/* the first tile of each 16x16 block in the 4x4 tile block, by quadrant
 * (TL, TR, BL, BR: 0, 2, 8, 10) */
#define K_BLOCK_OFFSET  0xE64F
/* the starting scroll_x_prev and bg_column by direction bit 0, for the
 * re-init branch of bg_column_check (no route) */
#define K_TURN_SCROLL   0xE60B
#define K_TURN_COLUMN   0xE60D

/* in the round's $A000 bank */
#define ROUND_BANK_TABLE 0xC1F8   /* round_bank, fixed bank */
#define BLOCK_TILE_TL   0xBB00
#define BLOCK_TILE_TR   0xBC00
#define BLOCK_TILE_BL   0xBD00
#define BLOCK_TILE_BR   0xBE00
#define BLOCK_PALETTE   0xBF00
#define SCREEN_LIST_0   0xAE60    /* sub_area 0, 1, 2 */
#define SCREEN_LIST_1   0xAE90
#define SCREEN_LIST_2   0xAEC0

/*
 * bg_column_check, $E4DF (proposed name): after a scroll, is a column due?
 * $03 = how far the camera moved since last frame (|scroll_x -
 * scroll_x_prev|); none means no column (carry set). Then
 * bg_column_check_from with the camera's old x as seen in the direction of
 * travel: scroll_x_prev moving right, its complement moving left.
 * In: $10 = direction (camera_update). Out: carry set when no column is
 * written; otherwise bg_column_write's.
 * Called by: camera_scroll_right, JMP at $E415 (auto-walk) and $E46F.
 */
void func_E4DF(void) {
    REGS_IN;

    mff_cycles(0xE4DF, 10);
    a = RAM(SCROLL_X);
    c = 1;
    SBC_A(RAM(SCROLL_X_PREV));
    if (n) {                                    /* no route: the camera only goes right */
        mff_cycles(0xE4E6, 6);
        a ^= 0xFF;
        c = 0;
        ADC_A(1);
    } else {
        mff_cycles(0xE4E4, 1);
    }
    mff_cycles(0xE4EB, 5);
    RAM(TMP3) = a;
    if (z) {
        mff_cycles(0xE4ED, 2 + 8);              /* BEQ across a page, SEC, RTS */
        c = 1;
        SYNC_OUT;
        mff_rts();
        return;
    }

    mff_cycles(0xE4EF, 7);
    if (RAM(SCROLL_MODE) & 0x0C) {              /* no route: scroll_mode is 1 or 2 */
        mff_cycles(0xE4F5, 14);
        a = RAM(TMP16);
        RAM(SCROLL_MODE) = a;
        x = a & 0x01;
        a = prg_read((uint16_t)(K_TURN_SCROLL + x));
        mff_cycles(0xE4FF, 7);
        RAM(SCROLL_X_PREV) = a;
        a = prg_read((uint16_t)(K_TURN_COLUMN + x));
        mff_cycles(0xE504, 3);
        RAM(BG_COLUMN) = a;
    } else {
        mff_cycles(0xE4F3, 2);                  /* BEQ across a page */
    }

    mff_cycles(0xE506, 7);
    if (RAM(TMP16) & DIR_RIGHT) {
        mff_cycles(0xE50C, 6);
        a = RAM(SCROLL_X_PREV);
        SET_NZ(a);
        SYNC_OUT;
        stage_jmp(0xE515, func_E515);
        return;
    }
    mff_cycles(0xE50A, 1 + 5);
    a = RAM(SCROLL_X_PREV) ^ 0xFF;
    SYNC_OUT;
    func_E515();
}

/*
 * bg_column_check_from, $E515 (proposed name): a column is due when the
 * camera's old x within its 8-pixel column (A & 7) plus the distance moved
 * ($03) reaches the next column: then bg_column_write, else carry set.
 * In: A as bg_column_check sets it, $03, $10.
 * Called by: bg_column_check (JMP at $E50E, and falling in at $E515).
 */
void func_E515(void) {
    REGS_IN;

    mff_cycles(0xE515, 15);
    a &= 0x07;
    c = 0;
    ADC_A(RAM(TMP3));
    a = (uint8_t)(a >> 3);
    SET_NZ(a);
    if (z) {
        mff_cycles(0xE51F, 8);
        c = 1;
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0xE51D, 1);
    SYNC_OUT;
    func_E521();
}

/*
 * $E547-$E55F, shared by bg_column_write and bg_column_turn: map the
 * round's bank at $A000, find the column's screen (map_screen_ptr) and its
 * first metatile (map_cell = bg_column / 4, row 0), start the strip at
 * $03 = 0 and write the rows.
 */
static void bg_column_fetch(void) {
    REGS_IN;

    mff_cycles(0xE547, 7);
    y = RAM(ROUND_TABLES);
    a = prg_read((uint16_t)(ROUND_BANK_TABLE + y));
    SET_NZ(a);
    mff_cycles(0xE54C, 9);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0550, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE551, 19);
    a = RAM(BG_COLUMN);
    c = (a >> 1) & 1;                           /* the second LSR's carry */
    a = (uint8_t)(a >> 2);
    RAM(MAP_CELL) = a;
    y = RAM(BG_SCREEN);
    SET_NZ(y);
    SYNC_OUT;
    if (!mff_call_fn(0x055B, func_E66E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE55C, 5);
    a = 0;
    SET_NZ(a);
    RAM(TMP3) = a;
    SYNC_OUT;
    func_E560();
}

/*
 * bg_column_write, $E521 (proposed name): stream the next background column
 * in the direction $10 (DIR_RIGHT: bg_column + 1, DIR_LEFT: - 1; past
 * either end of the nametable bg_screen moves by one too). scroll_mode
 * ($23) keeps the direction; a change of direction would skip the column
 * step (bg_column_turn), which no route does: the camera only goes right.
 * In: $10. Out: carry clear, A = 0, Y = 0 or $14 (see bg_column_rows).
 * Called by: round_start_wait (JSR at 0E:C76C), its twin (0F:E001) and
 * bg_column_check_from (falling in at $E521).
 */
void func_E521(void) {
    REGS_IN;

    mff_cycles(0xE521, 22);
    a = RAM(TMP16);
    mff_push(a);
    x = a & 0x01;
    a = mff_pull();
    SET_CMP(a, RAM(SCROLL_MODE));
    RAM(SCROLL_MODE) = a;
    if (!z) {                                   /* no route */
        mff_cycles(0xE52E, 3);
        SYNC_OUT;
        stage_jmp(0xE53F, func_E53F);
        return;
    }
    mff_cycles(0xE52C, 1 + 9);
    a = RAM(BG_COLUMN);
    c = 0;
    ADC_A(prg_read((uint16_t)(K_COLUMN_STEP + x)));
    mff_cycles(0xE537, 9);
    c = a >= 0x20;                              /* off either end of the nametable */
    a &= 0x1F;
    RAM(BG_COLUMN) = a;
    if (c) {
        SYNC_OUT;
        func_E53F();
        return;
    }
    mff_cycles(0xE53D, 1);
    SYNC_OUT;
    bg_column_fetch();
}

/*
 * bg_column_turn, $E53F (proposed name): bg_screen += the step (X = the
 * direction bit, as bg_column_write sets it), then the column. Reached
 * when bg_column wrapped, and by a JMP when the direction changed (no
 * route).
 * Called by: bg_column_write (JMP at $E52E, and falling in at $E53F).
 */
void func_E53F(void) {
    REGS_IN;

    mff_cycles(0xE53F, 9);
    a = RAM(BG_SCREEN);
    c = 0;
    ADC_A(prg_read((uint16_t)(K_COLUMN_STEP + x)));
    mff_cycles(0xE545, 3);
    RAM(BG_SCREEN) = a;
    SYNC_OUT;
    bg_column_fetch();
}

/*
 * bg_column_rows, $E560 (proposed name; the loop of the column writer):
 * for each metatile row from map_cell (row 0) down to row 4, build the
 * metatile's tiles (metatile_tiles) and copy this column's four into the
 * strip at $0783 + $03; on an odd column also queue the row's attribute
 * byte at $0798 + $03 and keep it in attr_shadow. Then close the list
 * (header, blank rows 4 and 24, $FF) and set vram_column_request.
 * In: map_cell, $03 = 0, map_screen_ptr, bg_column. Out: carry clear, A = 0,
 * X = $10 (the last row's offset), Y = 0 (even column) or $14 (odd).
 * Called by: bg_column_fetch (falling in at $E560) and itself (JMP at
 * $E5FE).
 */
void func_E560(void) {
    REGS_IN;

    for (;;) {
        mff_cycles(0xE560, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0562, func_E60F, 0xE000)) return;
        SYNC_IN;

        /* this column's tiles of the metatile: rows $03 to $03 + 3 */
        mff_cycles(0xE563, 45);
        y = RAM(MAP_CELL);
        RAM(TMP17) = RAM(ATTR_SHADOW + y);      /* $11: the attribute byte as shown */
        y = RAM(BG_COLUMN) & 0x03;              /* tile column in the metatile */
        x = RAM(TMP3);
        RAM(VRAM_BUFFER + 3 + x) = RAM(TILE_BLOCK + y);
        RAM(VRAM_BUFFER + 4 + x) = RAM(TILE_BLOCK + 4 + y);
        if (RAM(MAP_CELL) < 0x38) {             /* always: rows 0-4 only */
            mff_cycles(0xE583, 18);
            RAM(VRAM_BUFFER + 5 + x) = RAM(TILE_BLOCK + 8 + y);
            RAM(VRAM_BUFFER + 6 + x) = RAM(TILE_BLOCK + 12 + y);
        } else {
            mff_cycles(0xE581, 1);
        }

        /* odd columns: the attribute byte, half new and half old */
        mff_cycles(0xE58F, 7);
        if (RAM(BG_COLUMN) & 0x01) {
            mff_cycles(0xE595, 7);
            a = RAM(TMP16) & prg_read((uint16_t)(K_ATTR_NEW_MASK + y));
            mff_cycles(0xE59A, 10);
            RAM(TMP16) = a;
            a = RAM(TMP17) & prg_read((uint16_t)(K_ATTR_OLD_MASK + y));
            mff_cycles(0xE5A1, 43);
            a |= RAM(TMP16);
            RAM(VRAM_BUFFER + 27 + x) = a;
            y = RAM(MAP_CELL);
            RAM(ATTR_SHADOW + y) = a;
            RAM(VRAM_BUFFER + 24 + x) = 0x23;                   /* $23C8 + map_cell */
            RAM(VRAM_BUFFER + 25 + x) = (uint8_t)((y | 0xC0) + 0x08);
            RAM(VRAM_BUFFER + 26 + x) = 0x00;                   /* one byte */
            c = 0;
        } else {
            mff_cycles(0xE593, 1);
        }

        /* the next metatile row */
        mff_cycles(0xE5BE, 34);
        RAM(TMP3) = (uint8_t)(RAM(TMP3) + 4);
        a = RAM(MAP_CELL);
        c = 0;
        ADC_A(8);
        RAM(MAP_CELL) = a;
        SET_CMP(a, 0x28);
        if (c) break;
        mff_cycles(0xE5CF, 1 + 3);              /* BCC taken, JMP $E560 */
    }

    /* close the list */
    mff_cycles(0xE5D1, 32);
    RAM(VRAM_BUFFER) = 0x20;
    a = RAM(BG_COLUMN);
    c = 0;
    ADC_A(0x80);
    RAM(VRAM_BUFFER + 1) = a;                   /* $2080 + column */
    RAM(VRAM_BUFFER + 2) = 0x14;                /* 21 tiles */
    y = 0x00;
    if (RAM(BG_COLUMN) & 0x01) {
        mff_cycles(0xE5EB, 2);
        y = 0x14;                               /* after the 5 attribute entries */
    } else {
        mff_cycles(0xE5E9, 1);
    }
    mff_cycles(0xE5ED, 28);
    RAM(VRAM_BUFFER + 24 + y) = 0xFF;
    RAM(VRAM_COLUMN_REQUEST) = 0xFF;
    a = 0x00;
    SET_NZ(a);
    RAM(VRAM_BUFFER + 3) = a;                   /* rows 4 and 24 blank */
    RAM(VRAM_BUFFER + 23) = a;
    c = 0;
    SYNC_OUT;
    mff_rts();
}

/*
 * metatile_tiles, $E60F (proposed name): the 4x4 tiles of the metatile at
 * map_cell of the current screen into tile_block ($06C0, row by row), and
 * its attribute byte into $10 (TL bits 0-1, TR 2-3, BL 4-5, BR 6-7).
 * Out: $00/$01 = the metatile's record (metatile_ptr), $02 = $FF; A = $10,
 * X = 0, Y = the top-left block, N set.
 * Called by: bg_column_rows, JSR at $E560.
 */
void func_E60F(void) {
    REGS_IN;

    mff_cycles(0xE60F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0611, func_E653, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE612, 10);
    y = 0x03;
    RAM(TMP2) = y;                              /* $02: quadrant, BR first */
    a = 0x00;
    RAM(TMP16) = a;                             /* $10: the attribute byte */
    SYNC_OUT;
    func_E61A();
}

/*
 * metatile_tiles_loop, $E61A (the loop of metatile_tiles): quadrant $02
 * down to 0. Each quadrant's 16x16 block (byte $02 of the metatile record)
 * gives four tiles and two palette bits, shifted up as the loop goes.
 * Called by: metatile_tiles (falling in) and itself (JMP at $E64B).
 */
void func_E61A(void) {
    REGS_IN;

    for (;;) {
        mff_cycles(0xE61A, 7);
        y = RAM(TMP2);
        x = prg_read((uint16_t)(K_BLOCK_OFFSET + y));
        mff_cycles(0xE61F, 5);
        a = mem_read((uint16_t)(RAM16(TMP0) + y));      /* the block */
        mff_cycles(0xE621, 6);
        y = a;
        a = prg_read((uint16_t)(BLOCK_TILE_TL + y));
        mff_cycles(0xE625, 9);
        RAM(TILE_BLOCK + x) = a;
        a = prg_read((uint16_t)(BLOCK_TILE_TR + y));
        mff_cycles(0xE62B, 9);
        RAM(TILE_BLOCK + 1 + x) = a;
        a = prg_read((uint16_t)(BLOCK_TILE_BL + y));
        mff_cycles(0xE631, 9);
        RAM(TILE_BLOCK + 4 + x) = a;
        a = prg_read((uint16_t)(BLOCK_TILE_BR + y));
        mff_cycles(0xE637, 9);
        RAM(TILE_BLOCK + 5 + x) = a;
        a = prg_read((uint16_t)(BLOCK_PALETTE + y));
        mff_cycles(0xE63D, 15);
        a = (uint8_t)((a & 0x03) | RAM(TMP16));
        RAM(TMP16) = a;
        t = (uint8_t)(RAM(TMP2) - 1);
        RAM(TMP2) = t;
        SET_NZ(t);
        if (n) break;
        mff_cycles(0xE647, 13);
        t = RAM(TMP16);
        c = (t >> 6) & 1;                       /* the second ASL's carry */
        RAM(TMP16) = (uint8_t)(t << 2);
    }
    mff_cycles(0xE645, 1 + 6);
    SYNC_OUT;
    mff_rts();
}

/*
 * metatile_ptr, $E653 (proposed name): $00/$01 = $B700 + 4 * metatile, the
 * record of the metatile at map_cell in the current screen, after
 * making sure the round's bank is at $A000 (0E:C1DF).
 * Out: A = $01, Y = map_cell, carry clear.
 * Called by: metatile_tiles ($E60F), bg_probe ($ED5C), bg_probe_2 ($EE69).
 */
void func_E653(void) {
    REGS_IN;
    uint8_t metatile;

    mff_cycles(0xE653, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0655, 0xC1DF, -1)) return;
    SYNC_IN;
    mff_cycles(0xE656, 13);
    RAM(TMP1) = 0x00;
    y = RAM(MAP_CELL);
    metatile = mem_read((uint16_t)(RAM16(MAP_SCREEN_PTR) + y));
    mff_cycles(0xE65E, 33);
    RAM(TMP1) = metatile >> 6;
    RAM(TMP0) = (uint8_t)(metatile << 2);
    a = RAM(TMP1);
    c = 0;
    ADC_A(0xB7);
    RAM(TMP1) = a;
    SYNC_OUT;
    mff_rts();
}

/*
 * screen_map_ptr, $E66E (proposed name): map_screen_ptr ($20) = the
 * metatile map of entry Y of the area's screen list (sub_area 1 and 2 have
 * their own lists; any other value reads area 0's).
 * In: Y = the entry (bg_screen, or a probe's x high byte).
 * Called by: bg_column_fetch ($E559), bg_probe ($ED59), bg_probe_2 ($EE66).
 */
void func_E66E(void) {
    REGS_IN;

    mff_cycles(0xE66E, 5);
    a = RAM(SUB_AREA);
    if (a != 0) {
        mff_cycles(0xE672, 4);
        c = a >= 1;
        if (a == 1) {
            mff_cycles(0xE674, 1 + 4);
            a = prg_read((uint16_t)(SCREEN_LIST_1 + y));
            SET_NZ(a);
            mff_cycles(0xE683, 3);
            SYNC_OUT;
            stage_jmp(0xE689, func_E689);
            return;
        }
        mff_cycles(0xE676, 4);
        c = a >= 2;
        if (a == 2) {
            mff_cycles(0xE678, 1 + 4);
            a = prg_read((uint16_t)(SCREEN_LIST_2 + y));
            SYNC_OUT;
            func_E689();
            return;
        }
        mff_cycles(0xE67A, 4);
    } else {
        mff_cycles(0xE670, 1 + 4);
    }
    a = prg_read((uint16_t)(SCREEN_LIST_0 + y));
    SET_NZ(a);
    mff_cycles(0xE67D, 3);
    SYNC_OUT;
    stage_jmp(0xE689, func_E689);
}

/*
 * screen_map_ptr_of, $E689 (proposed name): map_screen_ptr = $AF00 + 64 * A,
 * screen A's metatile map in the round's bank.
 * Out: $00 = A >> 2 (scratch), A = map_screen_ptr's high byte, N set, Z/C/V
 * clear (the ADC of $AF and 0-$3F).
 * Called by: screen_map_ptr (JMP at $E67D and $E683, falling in for
 * sub_area 2).
 */
void func_E689(void) {
    REGS_IN;
    uint8_t screen = a;

    mff_cycles(0xE689, 73);
    mff_push(a);                                /* PHA/PLA around LDA #0, STA $00 */
    a = mff_pull();
    RAM(TMP0) = screen >> 2;                    /* the six ROLs */
    RAM(MAP_SCREEN_PTR) = (uint8_t)(screen << 6);
    a = RAM(TMP0);
    c = 0;
    ADC_A(0xAF);
    RAM(MAP_SCREEN_PTR + 1) = a;
    SYNC_OUT;
    mff_rts();
}
