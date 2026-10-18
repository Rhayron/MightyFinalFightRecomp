/*
 * stage_palette.c - sprite sets and background palette animation, bank $09
 * (16 KiB bank 4 at $A000) (docs/assets.md, "Palettes")
 *
 * A sprite set is three CHR banks for sprite tiles $40-$FF (R3-R5,
 * chr_banks $EB-$ED, which the NMI maps) and the 8 colours of sprite
 * palettes 2 and 3. Areas and screens pick one through the screen setups
 * (stage_events.c); the GO sign loads only set 3's palette.
 *
 * The background palette animations run every gameplay frame
 * (gameplay_update, $C969): each of the four background palettes with bit 7
 * of bg_pal_anim_on ($0100) steps its script (bg_pal_anim_script $0108)
 * by a frame timer (bg_pal_anim_timer $010C) through its steps
 * (bg_pal_anim_step $0104), writing 3 colours per step. The record of
 * animation n is at K_ANIM_RECORDS + K_ANIM_INDEX[n]:
 *     [last step, period, palette offset, a triplet per step]
 * and triplet k is 3 colours at K_ANIM_COLOURS + 3k. A step lasts
 * period + 1 frames.
 */
#include "stage.h"

/* sprite sets, by set s: CHR R3 at +2s, R4 at +2s + 1, R5 at +2s; and the
 * palettes, 8 bytes at +8s */
#define K_SET_CHR_R3        0xA035
#define K_SET_CHR_R4        0xA036
#define K_SET_CHR_R5        0xA0B5
#define K_SET_PALETTES      0xA105

/* palette animations */
#define K_ANIM_INDEX        0xA445
#define K_ANIM_RECORDS      0xA458      /* last step */
#define K_ANIM_PERIOD       0xA459
#define K_ANIM_OFFSET       0xA45A
#define K_ANIM_TRIPLETS     0xA45B
#define K_ANIM_COLOURS      0xA370

/*
 * sprite_set_load, $A000 (bank 4; proposed name): A = sprite set: its CHR
 * banks into chr_banks R3-R5, then its palette (sprite_set_palette).
 * Called by: the screen setups (stage_events.c, JSR $A000 in each).
 */
void func_A000_b4(void) {
    REGS_IN;

    mff_cycles(0xA000, 11);
    a = (uint8_t)(a << 1);
    mff_push(a);
    x = a;
    a = prg_read_ix(K_SET_CHR_R3, x);
    mff_cycles(0xA006, 7);
    RAM(CHR_BANKS + 3) = a;
    a = prg_read_ix(K_SET_CHR_R4, x);
    mff_cycles(0xA00B, 7);
    RAM(CHR_BANKS + 4) = a;
    a = prg_read_ix(K_SET_CHR_R5, x);
    mff_cycles(0xA010, 7);
    RAM(CHR_BANKS + 5) = a;
    a = mff_pull();
    SYNC_OUT;
    func_A013_b4();
}

/*
 * sprite_set_palette, $A013 (bank 4; proposed name): A = 2 * sprite set:
 * the set's 8 colours into sprite palettes 2 and 3 of palette_buf and
 * palette_base ($0618, $0638), and palette_request.
 * Out: X = $FF, carry set; $00/$01 = the set's palette record.
 * Called by: sprite_set_load (falling in at $A013) and the GO sign (stage
 * events, $89CA, with A = 6).
 */
void func_A013_b4(void) {
    REGS_IN;

    mff_cycles(0xA013, 20);
    c = (a >> 6) & 1;                           /* the second ASL's carry */
    y = (uint8_t)(a << 2);
    a = 0x00;
    ADC_A(K_SET_PALETTES >> 8);                 /* $A105, or $A205 past set 31 */
    RAM(TMP1) = a;
    RAM(TMP0) = K_SET_PALETTES & 0xFF;
    x = 0x00;
    for (;;) {
        mff_cycles(0xA022, 5);
        a = mem_read_iy(TMP0, y);
        mff_cycles(0xA024, 18);
        RAM(PALETTE_BUF + 0x18 + x) = a;
        RAM(PALETTE_BASE + 0x18 + x) = a;
        y++;
        x++;
        c = x >= 8;
        if (x == 8) break;
        mff_cycles(0xA02E, 1);
    }
    mff_cycles(0xA030, 11);
    x = 0xFF;
    SET_NZ(x);
    RAM(PALETTE_REQUEST) = x;
    SYNC_OUT;
    mff_rts();
}

/*
 * bg_palette_animate, $A300 (bank 4; proposed name): steps the animation of
 * each background palette 3..0 whose bg_pal_anim_on has bit 7. $11 counts
 * the palettes that changed, on top of palette_request's bit 0, and
 * becomes the new palette_request: the NMI uploads when it is not 0.
 * Called by: gameplay_update (JSR at $C969).
 */
void func_A300_b4(void) {
    REGS_IN;

    mff_cycles(0xA300, 18);
    RAM(TMP16) = 0x03;
    RAM(TMP17) = RAM(PALETTE_REQUEST) & 0x01;
    a = 0x00;
    RAM(PALETTE_REQUEST) = a;
    for (;;) {
        mff_cycles(0xA30E, 9);
        x = RAM(TMP16);
        a = RAM(BG_PAL_ANIM_ON + x);
        SET_NZ(a);
        if (n) {
            mff_cycles(0xA315, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0317, 0xA321, 4)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xA313, 1);
        }
        mff_cycles(0xA318, 7);
        t = (uint8_t)(RAM(TMP16) - 1);
        RAM(TMP16) = t;
        if (t & 0x80) break;
        mff_cycles(0xA31A, 1);
    }
    mff_cycles(0xA31C, 12);
    a = RAM(TMP17);
    SET_NZ(a);
    RAM(PALETTE_REQUEST) = a;
    SYNC_OUT;
    mff_rts();
}

/*
 * bg_palette_step, $A321 (bank 4; proposed name): background palette X's
 * animation: count its timer; when it reaches the period, restart it,
 * advance the step (back to 0 after the last), write the step's 3 colours
 * to palette_buf and palette_base at the palette's offset, and count the
 * change in $11.
 * Called by: bg_palette_animate ($A315).
 */
void func_A321_b4(void) {
    REGS_IN;
    uint8_t record;

    mff_cycles(0xA321, 8);
    y = RAM(BG_PAL_ANIM_SCRIPT + x);
    a = prg_read_ix(K_ANIM_INDEX, y);
    mff_cycles(0xA327, 17);
    record = y = a;
    a = RAM(BG_PAL_ANIM_TIMER + x);
    RAM(BG_PAL_ANIM_TIMER + x)++;
    m = prg_read_ix(K_ANIM_PERIOD, record);
    SET_CMP(a, m);
    mff_cycles(0xA331, 2);
    if (!z) {
        mff_cycles(0xA331, 1 + 6);
        SYNC_OUT;
        mff_rts();
        return;
    }

    /* the next step */
    mff_cycles(0xA333, 22);
    RAM(BG_PAL_ANIM_TIMER + x) = 0x00;
    a = RAM(BG_PAL_ANIM_STEP + x);
    RAM(BG_PAL_ANIM_STEP + x)++;
    mff_cycles(0xA341, 2);
    if (a == prg_read_ix(K_ANIM_RECORDS, record)) {   /* past the last step */
        mff_cycles(0xA343, 7);
        a = 0x00;
        RAM(BG_PAL_ANIM_STEP + x) = a;
    } else {
        mff_cycles(0xA341, 1);
    }

    /* its triplet: 3 colours at K_ANIM_COLOURS + 3 * triplet */
    mff_cycles(0xA348, 14);
    x = (uint8_t)(record + RAM(BG_PAL_ANIM_STEP + x));
    a = prg_read_ix(K_ANIM_TRIPLETS, x);
    mff_cycles(0xA351, 6);
    c = a >> 7;
    a = (uint8_t)(a << 1);
    ADC_A(prg_read_ix(K_ANIM_TRIPLETS, x));
    mff_cycles(0xA355, 6);
    x = a;
    a = prg_read_ix(K_ANIM_OFFSET, record);
    mff_cycles(0xA359, 7);
    y = a;
    RAM(TMP0) = 0x03;
    for (;;) {
        mff_cycles(0xA35E, 4);
        a = prg_read_ix(K_ANIM_COLOURS, x);
        mff_cycles(0xA361, 21);
        RAM(PALETTE_BUF + y) = a;
        RAM(PALETTE_BASE + y) = a;
        y++;
        x++;
        t = (uint8_t)(RAM(TMP0) - 1);
        RAM(TMP0) = t;
        if (t == 0) break;
        mff_cycles(0xA36B, 1);
    }
    mff_cycles(0xA36D, 5 + 6);
    t = (uint8_t)(RAM(TMP17) + 1);
    RAM(TMP17) = t;
    SET_NZ(t);
    SYNC_OUT;
    mff_rts();
}
