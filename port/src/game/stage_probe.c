/*
 * stage_probe.c - the background probes (docs/objects.md, "Movement
 * helpers"): what the stage map holds under an object
 *
 * A probe reads the attribute of the 16x16 block under a few points near
 * slot X: the high nibble of $BF00 + block in the round's bank (its low
 * bits are the block's palette). $40 marks a pit; the movement helpers
 * (0F:F03F, F0B3...) and the enemy AI (0D:A0C8...) treat the others as
 * walls. bg_probe walks a horizontal line of points (y fixed, x stepping),
 * bg_probe_2 a vertical one. The point list of probe kind Y starts at
 * $EEEB[Y] (bg_probe) or $EF23[Y] (bg_probe_2) in a table of records
 *     [points - 1, the fixed offset, the first offset, steps...]
 * (signed offsets from obj_x/obj_y), walked by probe_index ($40).
 * Results: probe_hits ($42 + n) the attribute of point n, counted down
 * from the first; probe_max ($41) the highest; $10 all of them ORed.
 * Scratch: $02 points left, $03 the block (bit 0 right half, bit 1 bottom
 * half), $04, $11/$12/$13 the point's y and world x.
 * The probes map the round's bank at $A000 and put the caller's back
 * (probe_saved_bank $2F) when they end.
 */
#include "stage.h"

/* bg_probe: the record of probe kind Y, and the records */
#define K_PROBE_H_START  0xEEEB
#define K_PROBE_H        0xEEF6
/* bg_probe_2 */
#define K_PROBE_V_START  0xEF23
#define K_PROBE_V        0xEF2D

#define ROUND_BANK_TABLE 0xC1F8   /* round_bank */
#define BLOCK_ATTRIBUTE  0xBF00   /* in the round's bank: palette and attribute */

/* the probes' walk over the map: the next screen, the next metatile, or
 * another block of the same metatile */
enum probe_fetch { FETCH_SCREEN, FETCH_METATILE, READ_BLOCK };

/*
 * bg_probe, $ECCF: probe kind Y along a horizontal line through slot X:
 * y = obj_y + dy, the points at obj_x + the offsets. A line that leaves
 * the page (wraps, reaches $F0, or obj_y_hi not 0) reads nothing: every hit
 * is 0 (no route). The walk crosses into the next metatile and the next
 * screen as x grows.
 * In: X = slot, Y = probe kind. Out: the results above; X kept.
 * Called by: 0C:9B39 (obj_pit_check), 0D:A0C8, A0D7, A9FB, AA22, AA38
 * (the enemy AI), 0F:F0B3, F108, F132, F155 (the movement helpers).
 */
void func_ECCF(void) {
    REGS_IN;
    int on_page;

    mff_cycles(0xECCF, 4);
    a = prg_read_ix(K_PROBE_H_START, y);
    SET_NZ(a);
    mff_cycles(0xECD2, 9);
    RAM(PROBE_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0CD6, func_EEC4, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xECD7, 6);
    y = a;
    a = prg_read_ix(K_PROBE_H, y);            /* points - 1 */
    mff_cycles(0xECDB, 14);
    RAM(TMP2) = a;
    RAM(TMP3) = RAM(OBJ_Y_HI + x);
    a = prg_read_ix(K_PROBE_H + 1, y);        /* dy */
    mff_cycles(0xECE5, 18);
    mff_push(a);
    c = 0;
    ADC_A(RAM(OBJ_Y + x));
    RAM(TMP17) = a;                             /* $11: the line */
    a = mff_pull();

    /* on the page: no wrap either way, below $F0, obj_y_hi 0 */
    if (a & 0x80) {                             /* dy < 0: a carry means no wrap */
        mff_cycles(0xECED, 1 + 2);
        on_page = c;
        if (on_page) mff_cycles(0xECF9, 2);     /* BCS across a page */
    } else if (c) {
        mff_cycles(0xECEF, 2 + 1);
        on_page = 0;
    } else {
        mff_cycles(0xECEF, 2 + 7);
        c = RAM(TMP17) >= 0xF0;
        on_page = !c;
        mff_cycles(0xECF5, on_page ? 2 + 2 : 1);
    }
    if (on_page) {
        mff_cycles(0xED08, 5);
        a = RAM(TMP3);
        if (a != 0) {
            mff_cycles(0xED0A, 2);              /* BNE across a page */
            on_page = 0;
        }
    }
    if (!on_page) {                             /* no route */
        mff_cycles(0xECFB, 5);
        a = 0x00;
        y = RAM(TMP2);
        for (;;) {
            mff_cycles(0xECFF, 9);
            RAM(PROBE_HITS + y) = a;
            y--;
            SET_NZ(y);
            if (n) break;
            mff_cycles(0xED03, 2);              /* BPL across a page */
        }
        mff_cycles(0xED05, 3);
        SYNC_OUT;
        stage_jmp(0xEEDF, func_EEDF);
        return;
    }

    /* the first point: map_cell's row from $11 (screen rows 0-31 are the
     * status bar's: - 8), $03 bit 1 = the block row; $13:$12 = obj_x +
     * the first offset, map_cell's column and $03 bit 0 from it */
    mff_cycles(0xED0C, 39);
    a = (uint8_t)(RAM(TMP17) >> 2);
    mff_push(a);
    RAM(MAP_CELL) = (uint8_t)((a & 0x38) - 8);
    a = mff_pull();
    RAM(TMP3) = (a >> 1) & 0x02;
    RAM(TMP4) = 0x00;
    a = prg_read_ix(K_PROBE_H + 2, y);
    mff_cycles(0xED25, 2);
    if (a & 0x80) {
        mff_cycles(0xED27, 5);
        RAM(TMP4)--;                            /* sign-extend the offset */
    } else {
        mff_cycles(0xED25, 1);
    }
    mff_cycles(0xED29, 53);
    m = a;
    c = (a + RAM(OBJ_X + x)) > 0xFF;
    RAM(TMP18) = (uint8_t)(m + RAM(OBJ_X + x));
    a = RAM(OBJ_X_HI + x);
    ADC_A(RAM(TMP4));
    RAM(TMP19) = a;
    a = (uint8_t)(RAM(TMP18) >> 4);
    mff_push(a);
    RAM(TMP3) = (uint8_t)((a & 0x01) | RAM(TMP3));
    a = mff_pull();
    c = a & 1;
    a = (uint8_t)((a >> 1) | RAM(MAP_CELL));
    RAM(MAP_CELL) = a;
    SYNC_OUT;
    func_ED49();
}

/*
 * bg_probe_walk, $ED49 (proposed name; bg_probe's loop): from the point at
 * $13:$12 (map_cell, $03), read each point's block, step x, and fetch the
 * next metatile or screen as needed; then probe_end.
 * Called by: bg_probe (falling in at $ED49) and itself (JMP at $EDB0).
 */
void func_ED49(void) {
    REGS_IN;
    enum probe_fetch at = FETCH_SCREEN;

    for (;;) {
        if (at == FETCH_SCREEN) {
            /* the round's bank, the screen of $13 */
            mff_cycles(0xED49, 10);
            RAM(TMP4) = x;
            y = RAM(ROUND_TABLES);
            a = prg_read_ix(ROUND_BANK_TABLE, y);
            SET_NZ(a);
            mff_cycles(0xED50, 9);
            RAM(BANK_REQUEST + 1) = a;
            SYNC_OUT;
            if (!mff_call_fn(0x0D54, func_FF4D, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xED55, 12);
            x = RAM(TMP4);
            y = RAM(TMP19);
            SET_NZ(y);
            SYNC_OUT;
            if (!mff_call_fn(0x0D5B, func_E66E, 0xE000)) return;
            SYNC_IN;
        }
        if (at != READ_BLOCK) {
            mff_cycles(0xED5C, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0D5E, func_E653, 0xE000)) return;
            SYNC_IN;
        }

        /* this point's block attribute */
        mff_cycles(0xED5F, 8);
        y = RAM(TMP3);
        a = mem_read_iy(TMP0, y);
        mff_cycles(0xED63, 6);
        y = a;
        a = prg_read_ix(BLOCK_ATTRIBUTE, y);
        mff_cycles(0xED67, 15);
        a &= 0xF0;
        y = RAM(TMP2);
        RAM(PROBE_HITS + y) = a;
        if (a >= RAM(PROBE_MAX)) {
            mff_cycles(0xED72, 3);
            RAM(PROBE_MAX) = a;
        } else {
            mff_cycles(0xED70, 1);
        }
        mff_cycles(0xED74, 13);
        a |= RAM(TMP16);
        RAM(TMP16) = a;
        t = (uint8_t)(RAM(TMP2) - 1);
        RAM(TMP2) = t;
        if (t & 0x80) break;

        /* the next point: x += the step; same 16-pixel column, the other
         * half of the metatile, the next metatile, or the next screen */
        mff_cycles(0xED7C, 29);
        RAM(PROBE_INDEX)++;
        y = RAM(PROBE_INDEX);
        a = RAM(TMP18);
        mff_push(a);
        RAM(TMP4) = a & 0x10;
        a = mff_pull();
        c = 0;
        ADC_A(prg_read_ix(K_PROBE_H + 2, y));
        mff_cycles(0xED8C, 10);
        RAM(TMP18) = a;
        a &= 0x10;
        c = a >= RAM(TMP4);
        if (a == RAM(TMP4)) {
            mff_cycles(0xED92, 1);
            at = READ_BLOCK;
            continue;
        }
        mff_cycles(0xED94, 12);
        a = RAM(TMP3) ^ 0x01;
        RAM(TMP3) = a;
        a &= 0x01;
        if (a != 0) {
            mff_cycles(0xED9C, 1);
            at = READ_BLOCK;
            continue;
        }
        mff_cycles(0xED9E, 12);
        RAM(MAP_CELL)++;
        a = RAM(MAP_CELL) & 0x07;
        SET_NZ(a);
        if (!z) {
            mff_cycles(0xEDA4, 1);
            at = FETCH_METATILE;
            continue;
        }
        mff_cycles(0xEDA6, 21);
        RAM(TMP19)++;
        RAM(MAP_CELL)--;
        a = RAM(MAP_CELL) & 0x38;               /* column 0 of the row */
        RAM(MAP_CELL) = a;
        at = FETCH_SCREEN;
    }

    /* CPX #0 / BNE to the next instruction, then probe_end */
    mff_cycles(0xED7A, 1 + 4);
    SET_CMP(x, 0);
    if (!z) mff_cycles(0xEDB5, 1);
    mff_cycles(0xEDB7, 3);
    SYNC_OUT;
    stage_jmp(0xEEDF, func_EEDF);
}

/*
 * bg_probe_2, $EDBA: probe kind Y along a vertical line through slot X:
 * x = obj_x + dx, the points at obj_y + the offsets, walking down through
 * the metatile rows of one screen. The start is clamped to the page (to 0
 * or $EF) when the first point is off it; no route does that.
 * The code at $EE1E-$EE42, which would zero the hits of points off the
 * page, cannot run: the BEQ at $EE1C (after LDA #0) always jumps past it.
 * In: X = slot, Y = probe kind. Out: as bg_probe.
 * Called by: 0D:AA2D (the enemy AI), 0F:F014 and F065 (after a move
 * right or left).
 */
void func_EDBA(void) {
    REGS_IN;
    enum { ON_PAGE, CLAMP_TOP, CLAMP_BOTTOM } line;
    enum probe_fetch at;

    mff_cycles(0xEDBA, 4);
    a = prg_read_ix(K_PROBE_V_START, y);
    SET_NZ(a);
    mff_cycles(0xEDBD, 9);
    RAM(PROBE_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0DC1, func_EEC4, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xEDC2, 6);
    y = a;
    a = prg_read_ix(K_PROBE_V, y);            /* points - 1 */
    mff_cycles(0xEDC6, 12);
    RAM(TMP2) = a;
    RAM(TMP4) = 0x00;
    a = prg_read_ix(K_PROBE_V + 1, y);        /* dx */
    mff_cycles(0xEDCF, 2);
    if (a & 0x80) {
        mff_cycles(0xEDD1, 5);
        RAM(TMP4)--;
    } else {
        mff_cycles(0xEDCF, 1);
    }

    /* $13:$12 = obj_x + dx: map_cell's column, $03 bit 0 */
    mff_cycles(0xEDD3, 53);
    m = a;
    c = (a + RAM(OBJ_X + x)) > 0xFF;
    RAM(TMP18) = (uint8_t)(m + RAM(OBJ_X + x));
    a = RAM(OBJ_X_HI + x);
    ADC_A(RAM(TMP4));
    RAM(TMP19) = a;
    a = (uint8_t)(RAM(TMP18) >> 4);
    mff_push(a);
    RAM(TMP3) = a & 0x01;
    a = mff_pull();
    RAM(MAP_CELL) = (uint8_t)(a >> 1);

    /* $11 = obj_y + the first offset, if it stays on the page */
    a = RAM(OBJ_Y_HI + x);
    z = a == 0;
    if (a & 0x80) {                             /* no route */
        mff_cycles(0xEDF2, 2);
        line = CLAMP_TOP;
    } else if (!z) {                            /* no route */
        mff_cycles(0xEDF4, 2 + 2);
        line = CLAMP_BOTTOM;
    } else {
        mff_cycles(0xEDF4, 2 + 10);
        a = RAM(OBJ_Y + x);
        c = 0;
        ADC_A(prg_read_ix(K_PROBE_V + 2, y));
        mff_cycles(0xEDFD, 7);
        RAM(TMP17) = a;
        a = prg_read_ix(K_PROBE_V + 2, y);
        /* the branch ladder at $EE02-$EE10: a wrap clamps, else below $F0 */
        if (a & 0x80) {
            if (!c) {
                mff_cycles(0xEE02, 2 + 2 + 1);
                line = CLAMP_TOP;
            } else {
                mff_cycles(0xEE02, 2 + 2 + 2 + 1);
                line = ON_PAGE;
            }
        } else if (c) {
            mff_cycles(0xEE02, 2 + 1 + 2 + 1);
            line = CLAMP_BOTTOM;
        } else {
            mff_cycles(0xEE02, 2 + 1 + 2);
            line = ON_PAGE;
        }
        if (line == ON_PAGE) {
            mff_cycles(0xEE0A, 7);
            c = RAM(TMP17) >= 0xF0;
            if (!c) {
                mff_cycles(0xEE0E, 1);
            } else {
                mff_cycles(0xEE10, 2 + 1 + 2 + 1);
                line = CLAMP_BOTTOM;
            }
        }
    }
    if (line == CLAMP_BOTTOM) {
        mff_cycles(0xEE12, 7 + 1);
        RAM(TMP17) = 0xEF;
    } else if (line == CLAMP_TOP) {
        mff_cycles(0xEE18, 7 + 1);
        RAM(TMP17) = 0x00;
    }

    /* map_cell's row and $03 bit 1 from $11, then the screen of $13 */
    mff_cycles(0xEE43, 42);
    a = (uint8_t)(RAM(TMP17) >> 2);
    mff_push(a);
    RAM(MAP_CELL) = (uint8_t)((a & 0x38) | RAM(MAP_CELL));
    a = mff_pull();
    c = a & 1;
    RAM(TMP3) = (uint8_t)(((a >> 1) & 0x02) | RAM(TMP3));
    RAM(TMP4) = x;
    y = RAM(ROUND_TABLES);
    a = prg_read_ix(ROUND_BANK_TABLE, y);
    SET_NZ(a);
    mff_cycles(0xEE5D, 9);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0E61, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xEE62, 12);
    x = RAM(TMP4);
    y = RAM(TMP19);
    SET_NZ(y);
    SYNC_OUT;
    if (!mff_call_fn(0x0E68, func_E66E, 0xE000)) return;
    SYNC_IN;

    at = FETCH_METATILE;
    for (;;) {
        if (at == FETCH_METATILE) {
            mff_cycles(0xEE69, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0E6B, func_E653, 0xE000)) return;
            SYNC_IN;
        }

        /* this point's block attribute */
        mff_cycles(0xEE6C, 8);
        y = RAM(TMP3);
        a = mem_read_iy(TMP0, y);
        mff_cycles(0xEE70, 6);
        y = a;
        a = prg_read_ix(BLOCK_ATTRIBUTE, y);
        mff_cycles(0xEE74, 15);
        a &= 0xF0;
        y = RAM(TMP2);
        RAM(PROBE_HITS + y) = a;
        if (a >= RAM(PROBE_MAX)) {
            mff_cycles(0xEE7F, 3);
            RAM(PROBE_MAX) = a;
        } else {
            mff_cycles(0xEE7D, 1);
        }
        mff_cycles(0xEE81, 13);
        a |= RAM(TMP16);
        RAM(TMP16) = a;
        t = (uint8_t)(RAM(TMP2) - 1);
        RAM(TMP2) = t;
        if (t & 0x80) break;

        /* the next point: y += the step; same 16-pixel row, the metatile's
         * other half, or the next metatile row */
        mff_cycles(0xEE89, 29);
        RAM(PROBE_INDEX)++;
        y = RAM(PROBE_INDEX);
        a = RAM(TMP17);
        mff_push(a);
        RAM(TMP4) = a & 0x10;
        a = mff_pull();
        c = 0;
        ADC_A(prg_read_ix(K_PROBE_V + 2, y));
        mff_cycles(0xEE99, 10);
        RAM(TMP17) = a;
        a &= 0x10;
        if (a == RAM(TMP4)) {
            mff_cycles(0xEE9F, 1);
            at = READ_BLOCK;
            continue;
        }
        mff_cycles(0xEEA1, 12);
        a = RAM(TMP3) ^ 0x02;
        RAM(TMP3) = a;
        a &= 0x02;
        if (a != 0) {
            mff_cycles(0xEEA9, 1);
            at = READ_BLOCK;
            continue;
        }
        mff_cycles(0xEEAB, 21);
        a = RAM(MAP_CELL);
        mff_push(a);
        c = 0;
        ADC_A(8);
        RAM(MAP_CELL) = a;
        c = a >= 0x40;
        a = mff_pull();
        SET_NZ(a);
        if (!c) {
            mff_cycles(0xEEB6, 1);
            at = FETCH_METATILE;
            continue;
        }
        mff_cycles(0xEEB8, 6);                  /* no route: off the screen's map */
        RAM(MAP_CELL) = a;
        at = READ_BLOCK;
    }

    /* CPX #0 / BNE to the next instruction, then probe_end */
    mff_cycles(0xEE87, 1 + 4);
    SET_CMP(x, 0);
    if (!z) mff_cycles(0xEEBF, 1);
    mff_cycles(0xEEC1, 3);
    SYNC_OUT;
    stage_jmp(0xEEDF, func_EEDF);
}

/*
 * probe_begin, $EEC4 (proposed name): clears $10 and probe_max, saves the
 * $A000 bank in probe_saved_bank and maps the round's.
 * Out: A and X kept, Y = round_tables.
 * Called by: bg_probe ($ECD4), bg_probe_2 ($EDBF).
 */
void func_EEC4(void) {
    REGS_IN;

    mff_cycles(0xEEC4, 29);
    mff_push(a);
    mff_push(x);
    a = 0x00;
    RAM(TMP16) = a;
    RAM(PROBE_MAX) = a;
    RAM(PROBE_SAVED_BANK) = RAM(BANK_REQUEST + 1);
    y = RAM(ROUND_TABLES);
    a = prg_read_ix(ROUND_BANK_TABLE, y);
    SET_NZ(a);
    mff_cycles(0xEED6, 9);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0EDA, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xEEDB, 16);
    x = mff_pull();
    a = mff_pull();
    SET_NZ(a);
    SYNC_OUT;
    mff_rts();
}

/*
 * probe_end, $EEDF (proposed name): maps probe_saved_bank back at $A000.
 * Out: X kept (N/Z of it).
 * Called by: bg_probe (JMP at $ED05, $EDB7), bg_probe_2 (JMP at $EEC1,
 * and $EE40 in its dead code).
 */
void func_EEDF(void) {
    REGS_IN;

    mff_cycles(0xEEDF, 17);
    mff_push(x);
    a = RAM(PROBE_SAVED_BANK);
    SET_NZ(a);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0EE7, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xEEE8, 12);
    a = mff_pull();
    x = a;
    SET_NZ(x);
    SYNC_OUT;
    mff_rts();
}

/*
 * push_out_left, $EF55 (proposed name): moves slot X left to the left edge
 * of the 16-pixel column of the probe point $12 (obj_x -= $12 & $0F).
 * Out: $12 &= $0F. Called by: 0F:F020 (after a move right hits a wall).
 */
void func_EF55(void) {
    REGS_IN;

    mff_cycles(0xEF55, 39);
    RAM(TMP18) &= 0x0F;
    a = RAM(OBJ_X + x);
    c = 1;
    SBC_A(RAM(TMP18));
    RAM(OBJ_X + x) = a;
    a = RAM(OBJ_X_HI + x);
    SBC_A(0);
    RAM(OBJ_X_HI + x) = a;
    SYNC_OUT;
    mff_rts();
}

/*
 * push_out_right, $EF6D (proposed name): moves slot X right past the right
 * edge of the probe point's 16-pixel column (obj_x += 16 - ($12 & $0F)).
 * Called by: 0F:F071 (after a move left hits a wall).
 */
void func_EF6D(void) {
    REGS_IN;

    mff_cycles(0xEF6D, 35);
    a = (uint8_t)((RAM(TMP18) & 0x0F) ^ 0x0F);
    c = 1;
    ADC_A(RAM(OBJ_X + x));
    RAM(OBJ_X + x) = a;
    a = RAM(OBJ_X_HI + x);
    ADC_A(0);
    RAM(OBJ_X_HI + x) = a;
    SYNC_OUT;
    mff_rts();
}

/*
 * push_out_down, $EF83 (proposed name): moves slot X down past the bottom
 * of the probe point's 16-pixel row (obj_y += 16 - ($11 & $0F)), onto the
 * next vertical page at $F0 (no route).
 * Called by: 0F:F114.
 */
void func_EF83(void) {
    REGS_IN;

    mff_cycles(0xEF83, 22);
    a = (uint8_t)((RAM(TMP17) & 0x0F) ^ 0x0F);
    c = 1;
    ADC_A(RAM(OBJ_Y + x));
    RAM(OBJ_Y + x) = a;
    SET_CMP(a, 0xF0);
    if (c) {                                    /* no route */
        mff_cycles(0xEF94, 14);
        ADC_A(0x0F);
        RAM(OBJ_Y + x) = a;
        t = (uint8_t)(RAM(OBJ_Y_HI + x) + 1);
        RAM(OBJ_Y_HI + x) = t;
        SET_NZ(t);
    } else {
        mff_cycles(0xEF92, 1);
    }
    mff_cycles(0xEF9C, 6);
    SYNC_OUT;
    mff_rts();
}

/*
 * push_out_up, $EF9D (proposed name): moves slot X up to the top of the
 * probe point's 16-pixel row (obj_y -= $11 & $0F), onto the previous
 * vertical page if it wraps (no route). $11 is kept.
 * Called by: 0F:F0BD, F146.
 */
void func_EF9D(void) {
    REGS_IN;

    mff_cycles(0xEF9D, 27);
    a = RAM(TMP17);
    mff_push(a);
    RAM(TMP17) = a & 0x0F;
    a = RAM(OBJ_Y + x);
    c = 1;
    SBC_A(RAM(TMP17));
    RAM(OBJ_Y + x) = a;
    if (!c) {                                   /* no route */
        mff_cycles(0xEFAF, 14);
        SBC_A(0x0F);
        RAM(OBJ_Y + x) = a;
        RAM(OBJ_Y_HI + x)--;
    } else {
        mff_cycles(0xEFAD, 1);
    }
    mff_cycles(0xEFB7, 13);
    a = mff_pull();
    SET_NZ(a);
    RAM(TMP17) = a;
    SYNC_OUT;
    mff_rts();
}
