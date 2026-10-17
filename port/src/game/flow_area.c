/*
 * flow_area.c - an area's graphics when it starts, and the round's bank
 *
 * A round has three areas (sub_area 0-2), each described by a $30-byte
 * record in the round's data bank at $A000 (flow.h, AREA_RECORD). When an
 * area starts, round_start_wait (0E:C79D) or the next-area code (0F:E04B)
 * sets area_palette = 0 and calls area_load_graphics ($C3DD), which runs
 * as one chain of JMPs and returns from its last link:
 *
 *   $C3DD  the round's bank at $A000, the background CHR banks (record
 *          +$18/$19) and the player's 8 colors in sprite palettes 0-1
 *   $C44F  the (sprite set, palette set) pair of the record: the sprite set
 *          is pushed, the palette set goes on in A
 *   $C47D  the set's 16 background colors and 4 palette animation slots
 *   $C527  sprite backdrop, then bank 09's $A000 loads the sprite set
 *          (CHR R3-R5, sprite palettes 2-3) with the pushed number
 *
 * Each link that depends on the area has three copies in the ROM, one per
 * record, chosen by testing sub_area for 0, 1 and 2; any other value takes
 * area 0's copy. The copies differ only in their addresses and the table
 * they read, so where they are long the C runs one copy with the address
 * of the one the ROM would run (a branch that crosses a page costs one
 * more cycle, which is why some taken branches charge 2).
 *
 * Also here: map_round_bank ($C1DF) and player_load_graphics ($C8B4).
 */
#include "flow.h"

void func_C44F(void);

/*
 * map_round_bank ($C1DF, proposed name)
 * Maps the round's data bank at $A000 again: bank_request+1 =
 * round_bank[round_tables] and bank_switch, unless bank_request+1 is $13.
 * X and Y are pushed first and pulled at the end.
 * In: round_tables. Out: X and Y as on entry, A = X, N/Z by X.
 * Caller: 0F:E653 (port func_E653), first thing, before it reads a screen
 * map through map_screen_ptr in that bank; the column renderer and the
 * background probes call it (0F:E60F, 0F:ED5C, 0F:EE69).
 * The $13 test never passes: the board has banks $00-$0F and no code
 * stores $13 in bank_request+1 (the stores are constant pairs,
 * round_bank, 0F:EB93 + 1 = $03/$0B, and saved copies of it).
 */
void func_C1DF(void) {
    REGS_IN;
    mff_cycles(0xC1DF, 17);
    a = x;
    mff_push(a);
    a = y;
    mff_push(a);
    a = RAM(BANK_REQUEST + 1);
    c = a >= 0x13;
    if (a != 0x13) {
        mff_cycles(0xC1E9, 7);
        y = RAM(ROUND_TABLES);
        a = prg_read((uint16_t)(ROUND_BANK_TABLE + y));
        SET_NZ(a);
        mff_cycles(0xC1EE, 9);
        RAM(BANK_REQUEST + 1) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x01F2, func_FF4D, 0xE000)) return;   /* bank_switch */
        SYNC_IN;
    } else {
        mff_cycles(0xC1E7, 1);
    }
    mff_cycles(0xC1F3, 18);
    y = mff_pull();
    x = mff_pull();
    a = x;
    SET_NZ(x);
    SYNC_OUT;
    mff_rts(); return;
}

/*
 * area_load_graphics ($C3DD, proposed name)
 * The graphics of the area that starts (round round_tables, area
 * sub_area): maps round_bank[round_tables] at $A000; chr_banks+0/+1 (MMC3
 * R0/R1, the background) = record +$18/$19; sprite palettes 0-1
 * (palette_buf and palette_base +$10..$17) = the player's 8 colors. Then
 * area_pick_sets ($C44F): by JMP after Cody's and Guy's copies of the
 * color loop, by falling into it after Haggar's. The chain returns from
 * area_load_sprite_set ($C527).
 * In: round_tables, sub_area, player_id, area_palette.
 * Callers: round_start_wait at 0E:C79D (in this port, the copies of it in
 * main_task's entries, lifted_c.c) and the next-area code at 0F:E04B
 * (func_E03B, and lifted_d.c's func_DF50/func_DFE9, which run the same code
 * from its start in bank 0E).
 */
void func_C3DD(void) {
    REGS_IN;
    int area, player;
    uint16_t pc;

    mff_cycles(0xC3DD, 7);
    y = RAM(ROUND_TABLES);
    a = prg_read((uint16_t)(ROUND_BANK_TABLE + y));
    SET_NZ(a);
    mff_cycles(0xC3E2, 9);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x03E6, func_FF4D, 0xE000)) return;   /* bank_switch */
    SYNC_IN;

    /* which record: sub_area 1 or 2, else area 0's */
    mff_cycles(0xC3E7, 5);
    a = RAM(SUB_AREA);
    area = 0;
    if (a == 0) {
        mff_cycles(0xC3E9, 1);
    } else {
        mff_cycles(0xC3EB, 4);
        c = a >= 1;
        if (a == 1) {
            mff_cycles(0xC3ED, 1);
            area = 1;
        } else {
            mff_cycles(0xC3EF, 4);
            c = a >= 2;
            if (a == 2) {
                mff_cycles(0xC3F1, 2);      /* BEQ $C40B crosses a page */
                area = 2;
            }
        }
    }

    /* the background CHR banks. Each copy ends with BNE $C417 on R1, so a
     * record whose R1 were 0 would go on into the next record's copy; no
     * record of the five round banks has R1 = 0. */
    switch (area) {
    case 0:
        mff_cycles(0xC3F3, 4);
        a = prg_read((uint16_t)(AREA_RECORD(0) + AREA_REC_BG_CHR));
        mff_cycles(0xC3F6, 7);
        RAM(CHR_BANKS) = a;
        a = prg_read((uint16_t)(AREA_RECORD(0) + AREA_REC_BG_CHR + 1));
        mff_cycles(0xC3FB, 5);
        RAM(CHR_BANKS + 1) = a;
        if (a != 0) {
            mff_cycles(0xC3FD, 2);          /* BNE $C417 crosses a page */
            break;
        }
        /* fall through */
    case 1:
        mff_cycles(0xC3FF, 4);
        a = prg_read((uint16_t)(AREA_RECORD(1) + AREA_REC_BG_CHR));
        mff_cycles(0xC402, 7);
        RAM(CHR_BANKS) = a;
        a = prg_read((uint16_t)(AREA_RECORD(1) + AREA_REC_BG_CHR + 1));
        mff_cycles(0xC407, 5);
        RAM(CHR_BANKS + 1) = a;
        if (a != 0) {
            mff_cycles(0xC409, 1);
            break;
        }
        /* fall through */
    case 2:
        mff_cycles(0xC40B, 4);
        a = prg_read((uint16_t)(AREA_RECORD(2) + AREA_REC_BG_CHR));
        mff_cycles(0xC40E, 7);
        RAM(CHR_BANKS) = a;
        a = prg_read((uint16_t)(AREA_RECORD(2) + AREA_REC_BG_CHR + 1));
        mff_cycles(0xC413, 5);
        RAM(CHR_BANKS + 1) = a;
        if (a != 0)
            mff_cycles(0xC415, 1);
        break;
    }

    /* the player's 8 colors into sprite palettes 0-1: player_id 1 Guy,
     * 2 Haggar, else Cody */
    mff_cycles(0xC417, 7);
    x = 7;
    a = RAM(PLAYER_ID);
    player = 0;
    if (a == 0) {
        mff_cycles(0xC41B, 1);
    } else {
        mff_cycles(0xC41D, 4);
        c = a >= 1;
        if (a == 1) {
            mff_cycles(0xC41F, 1);
            player = 1;
        } else {
            mff_cycles(0xC421, 4);
            c = a >= 2;
            if (a == 2) {
                mff_cycles(0xC423, 1);
                player = 2;
            }
        }
    }
    /* the copies of the loop are 15 bytes apart from $C425 */
    pc = (uint16_t)(0xC425 + 15 * player);
    for (;;) {                              /* X = 7 down to 0 */
        mff_cycles(pc, 4);
        a = prg_read((uint16_t)(PLAYER_PAL_TABLE + 8 * player + x));
        mff_cycles(pc + 3, 14);
        RAM(PALETTE_BUF + 16 + x) = a;
        RAM(PALETTE_BASE + 16 + x) = a;
        x--;
        SET_NZ(x);
        if (n) break;
        mff_cycles(pc, 1);
    }
    if (player == 2) {
        /* Haggar's copy ends where area_pick_sets starts */
        SYNC_OUT;
        func_C44F();
        return;
    }
    mff_cycles(pc + 12, 3);                 /* JMP $C44F */
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC44F, 2); call_by_address_tail(0xC44F, -1); return;
}

/*
 * area_pick_sets ($C44F, proposed name)
 * Reads pair area_palette of the area record (+$10 + 2n): pushes the
 * sprite set for area_load_sprite_set ($C527) and goes to
 * area_load_bg_palette ($C47D) with the palette set in A. area_palette is
 * always 0, and the palette set is 0 in every area.
 * In: area_palette, sub_area.
 * Callers: area_load_graphics only (JMP from $C431 and $C440, falling in
 * from $C44E).
 */
void func_C44F(void) {
    REGS_IN;
    int area;
    uint16_t pc;

    mff_cycles(0xC44F, 12);
    a = RAM(AREA_PALETTE);
    c = a >> 7;
    a = (uint8_t)(a << 1);                  /* two bytes per pair */
    x = a;
    a = RAM(SUB_AREA);
    area = 0;
    if (a == 0) {
        mff_cycles(0xC455, 1);
    } else {
        mff_cycles(0xC457, 4);
        c = a >= 1;
        if (a == 1) {
            mff_cycles(0xC459, 1);
            area = 1;
        } else {
            mff_cycles(0xC45B, 4);
            c = a >= 2;
            if (a == 2) {
                mff_cycles(0xC45D, 1);
                area = 2;
            }
        }
    }
    /* the copies are 10 bytes apart from $C45F */
    pc = (uint16_t)(0xC45F + 10 * area);
    mff_cycles(pc, 4);
    a = prg_read((uint16_t)(AREA_RECORD(area) + AREA_REC_SETS + x));
    mff_cycles(pc + 3, 7);
    mff_push(a);                            /* the sprite set */
    a = prg_read((uint16_t)(AREA_RECORD(area) + AREA_REC_SETS + 1 + x));
    SET_NZ(a);                              /* the palette set */
    mff_cycles(pc + 7, 3);                  /* JMP $C47D */
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC47D, 2); call_by_address_tail(0xC47D, -1); return;
}

/*
 * The copies of area_load_bg_palette's two loops ($C494, $C4C5, $C4F6, $31
 * bytes apart, each reading its own record), with Y = the set's offset and
 * X = 0: 16 colors into palette_buf and palette_base $00-$0F (background
 * palettes 0-3), then four palette animation slots: bg_pal_anim_on = bit 7,
 * bg_pal_anim_script = bits 0-6, step and timer 0. Then JMP $C527.
 */
static void area_copy_bg_palette(int area) {
    REGS_IN;
    uint16_t pc = (uint16_t)(0xC494 + 0x31 * area);
    uint16_t set_base = (uint16_t)(AREA_RECORD(area) + AREA_REC_BG_PALETTE);

    for (;;) {
        mff_cycles(pc, 4);
        a = prg_read((uint16_t)(set_base + y));
        mff_cycles(pc + 3, 18);
        RAM(PALETTE_BUF + x) = a;
        RAM(PALETTE_BASE + x) = a;
        y++;
        x++;
        if (x == 16) break;
        mff_cycles(pc, area == 2 ? 2 : 1);  /* area 2's BNE $C4F6 crosses a page */
    }
    mff_cycles(pc + 0x0F, 2);
    x = 0;
    for (;;) {
        mff_cycles(pc + 0x11, 4);
        a = prg_read((uint16_t)(set_base + y));
        mff_cycles(pc + 0x14, 41);
        mff_push(a);
        RAM(BG_PAL_ANIM_ON + x) = a & 0x80;
        a = mff_pull() & 0x7F;
        RAM(BG_PAL_ANIM_SCRIPT + x) = a;
        a = 0;
        RAM(BG_PAL_ANIM_STEP + x) = a;
        RAM(BG_PAL_ANIM_TIMER + x) = a;
        y++;
        x++;
        c = x >= 4;
        SET_NZ(x - 4);
        if (z) break;
        mff_cycles(pc + 0x11, 1);
    }
    mff_cycles(pc + 0x2E, 3);               /* JMP $C527 */
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC527, 2); call_by_address_tail(0xC527, -1); return;
}

/*
 * area_load_bg_palette ($C47D, proposed name)
 * Palette set A of the area record (20 bytes at +$1C + 20 * A): the
 * background palettes and the palette animation slots
 * (area_copy_bg_palette), then JMP area_load_sprite_set ($C527).
 * In: A = palette set, sub_area; the sprite set on the stack.
 * Callers: area_pick_sets only (JMP from $C466, $C470, $C47A).
 */
void func_C47D(void) {
    REGS_IN;
    int area;

    /* Y = A * 20: A * 4 + A * 16, the ADC taking the last ASL's carry */
    mff_cycles(0xC47D, 23);
    a = (uint8_t)(a << 2);
    RAM(TMP0) = a;
    a = (uint8_t)(a << 1);
    c = a >> 7;
    a = (uint8_t)(a << 1);
    m = RAM(TMP0);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    y = a;
    x = 0;
    a = RAM(SUB_AREA);
    area = 0;
    if (a == 0) {
        mff_cycles(0xC48A, 1);
    } else {
        mff_cycles(0xC48C, 4);
        if (a == 1) {
            mff_cycles(0xC48E, 1);
            area = 1;
        } else {
            mff_cycles(0xC490, 4);
            if (a == 2) {
                mff_cycles(0xC492, 1);
                area = 2;
            }
        }
    }
    SYNC_OUT;
    area_copy_bg_palette(area);
}

/*
 * $C4BD: the end of area 0's palette animation slot loop (INX, CPX #4,
 * BNE $C4A5, JMP $C527). Nothing calls or jumps here; it is one of the P0
 * map's entry points (port/scaffold/game.toml) and stays as lifted. No
 * routine starts at $C4A5, so going on there is a tail call by address.
 */
void func_C4BD(void) {
    REGS_IN;
    mff_cycles(0xC4BD, 6);
    x++;
    c = x >= 4;
    SET_NZ(x - 4);
    if (!z) {
        mff_cycles(0xC4C0, 1);
        SYNC_OUT;
        call_by_address_tail((uint16_t)(g_code_window_base + 0x04A5), -1);
        return;
    }
    mff_cycles(0xC4C2, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC527, 2); call_by_address_tail(0xC527, -1); return;
}

/*
 * area_load_sprite_set ($C527, proposed name)
 * The last link of area_load_graphics: sprite palette 0's first color
 * ($0610 of palette_buf and palette_base) = the background color ($0600);
 * maps bank 09 at $A000 and calls its $A000 with A = the sprite set pulled
 * from the stack (CHR R3-R5 in chr_banks+3..5, sprite palettes 2-3 in
 * $0618-$061F, palette_request = $FF). Leaves by JMP $FF4C, an RTS, which
 * returns to area_load_graphics' caller.
 * In: the sprite set pushed by area_pick_sets.
 * Callers: area_load_bg_palette's copies (JMP from $C4C2, $C4F3, $C524)
 * and $C4BD.
 */
void func_C527(void) {
    REGS_IN;
    mff_cycles(0xC527, 23);
    a = RAM(PALETTE_BUF);
    RAM(PALETTE_BUF + 16) = a;
    RAM(PALETTE_BASE + 16) = a;
    a = BANK_SPRITE_SETS;
    SET_NZ(a);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0536, func_FF4D, 0xE000)) return;   /* bank_switch */
    SYNC_IN;
    mff_cycles(0xC537, 10);
    a = mff_pull();                         /* the sprite set */
    SET_NZ(a);
    SYNC_OUT;
    if (!mff_call_at(0x053A, 0xA000, -1)) return;           /* 09:A000, load sprite set A */
    SYNC_IN;
    mff_cycles(0xC53B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xFF4C, 2);
    {
        uint16_t swb = g_code_window_base;
        g_code_window_base = 0xE000;
        func_FF4C();
        g_code_window_base = swb;
    }
    return;
}

/*
 * player_load_graphics ($C8B4, proposed name)
 * The player's look when an area starts: its 8 colors into sprite palettes
 * 0-1 (palette_buf and palette_base +$10..$17, copied backwards from
 * 0E:C53E + PLAYER_PAL_END[player_id]), palette_request = $FF (Y after the
 * loop), then the standing pose: player_set_anim(PLAYER_ANIM_STAND
 * [player_id]) and chr_banks+2 = PLAYER_CHR_STAND[player_id] (0F:F440).
 * In: player_id. Out: A = that CHR bank, Y = player_id.
 * Callers: round_start_wait at 0E:C7D7 (the main_task copies in
 * lifted_c.c) and the next-area code at 0F:E077 (func_E03B, and
 * lifted_d.c's func_DF50/func_DFE9), after area_load_graphics and placing
 * the player.
 */
void func_C8B4(void) {
    REGS_IN;
    mff_cycles(0xC8B4, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(PLAYER_PAL_END + y));
    mff_cycles(0xC8B9, 4);
    x = a;
    y = 7;
    for (;;) {
        mff_cycles(0xC8BC, 4);
        a = prg_read((uint16_t)(PLAYER_PAL_TABLE + x));
        mff_cycles(0xC8BF, 16);
        RAM(PALETTE_BUF + 16 + y) = a;
        RAM(PALETTE_BASE + 16 + y) = a;
        x--;
        y--;
        if (y & 0x80) break;
        mff_cycles(0xC8BC, 1);
    }
    mff_cycles(0xC8C9, 10);
    RAM(PALETTE_REQUEST) = y;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(PLAYER_ANIM_STAND + y));
    SET_NZ(a);
    mff_cycles(0xC8D0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x08D2, func_F294, 0xE000)) return;   /* player_set_anim */
    SYNC_IN;
    mff_cycles(0xC8D3, 4);
    a = prg_read((uint16_t)(PLAYER_CHR_STAND + y));
    SET_NZ(a);
    mff_cycles(0xC8D6, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x08D8, func_F440, 0xE000)) return;   /* chr_banks+2 = A */
    SYNC_IN;
    mff_cycles(0xC8D9, 6);
    SYNC_OUT;
    mff_rts(); return;
}
