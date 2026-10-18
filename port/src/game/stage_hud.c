/*
 * stage_hud.c - the status bar (docs/assets.md, "Status rows")
 *
 * The bar's fixed text (PLAYER, ENEMY, RD, PL, LEVEL, EXP, the "/") is
 * written once per area (bank 0A $9C00). Every gameplay frame in which no
 * background column or nametable patch is waiting, hud_update appends the
 * changing parts to the VRAM buffer at $0780 in nmi_vram_update's format,
 * [count, PPU address hi, lo, count bytes], from vram_buffer_len ($65), and
 * sets vram_request ($64):
 *     $204A  the player's health bar: a left part sized by character and
 *            level, one tile per 8 health ($10 empty, $11-$16 partial,
 *            $18 full, 7 shows as full), $1A at the right end
 *     $205C  the round digit, $207C the lives digit (digit d is tile d+1)
 *     $2349  the level mark ($73) at column 9 + level (Haggar's from 11)
 *     $2356  experience, 3 digits; $78 x 3 at the top level
 *     $235A  experience for the next level, 3 digits; $78 x 3 at the top
 *     $2069  the enemy's bar: $19, one tile per 8 life, $1A, blank to 15
 *            tiles; it blinks and goes blank when the enemy dies, and
 *            after $96 frames
 * The level gauge ($2349, 6 tiles) has a writer of its own, hud_level_gauge.
 *
 * Scratch: $00 tiles left in a bar, $01 the life left to draw, $02 bytes
 * left of the enemy bar's 15.
 */
#include "stage.h"

/* the health bar's tile count by level, per character */
#define K_HEALTH_BAR_CODY    0xE869
#define K_HEALTH_BAR_GUY     0xE86F
#define K_HEALTH_BAR_HAGGAR  0xE875
/* experience needed for the next level, per character: pointer lo/hi to
 * a table of 2 BCD bytes (hundreds, tens and ones) per level */
#define K_NEXT_LEVEL_LO      0xE87B
#define K_NEXT_LEVEL_HI      0xE87E
/* the level gauge's six tiles, per level (Haggar's from level 2): pointer
 * lo/hi */
#define K_GAUGE_LO           0xE98C
#define K_GAUGE_HI           0xE992

/* tiles */
#define TILE_BAR_EMPTY       0x10
#define TILE_BAR_FULL        0x18
#define TILE_BAR_LEFT        0x19
#define TILE_BAR_RIGHT       0x1A
#define TILE_LEVEL_MARK      0x73
#define TILE_DASH            0x78

/* the last level, where experience shows dashes */
#define TOP_LEVEL            5
#define TOP_LEVEL_HAGGAR     3

/* obj_kind values that get the enemy bar first: Thrasher, Sodom, Abigail,
 * Belger */
static const uint8_t k_boss_kinds[] = { 0x07, 0x08, 0x09, 0x0B };

/* the enemy bar's frames: blinking from the death, and blank at the end */
#define ENEMY_BAR_DEATH_FRAMES  0x1E
#define ENEMY_BAR_FRAMES        0x96

/*
 * One tile of a bar for the life left in $01, and the cycles of the test
 * (the E719/E90D ladder): negative or 0 is empty ($10), 1-6 partial
 * ($11-$16), 7 or more full ($18). Leaves A = the tile without its $10.
 */
static uint8_t bar_tile(uint16_t pc, uint8_t life) {
    if (life & 0x80) {
        mff_cycles(pc, 5 + 1 + 2);
        return TILE_BAR_EMPTY;
    }
    if (life == 0) {
        mff_cycles(pc, 5 + 2 + 1);
        return 0;
    }
    if (life >= 7) {
        mff_cycles(pc, 5 + 2 + 4 + 1 + 4 + 1);
        return TILE_BAR_FULL;
    }
    mff_cycles(pc, 5 + 2 + 4 + 2 + 1);
    return life;
}

/*
 * hud_update, $E6AB (proposed name): the status bar for the frame, unless a
 * write list is waiting (a column or a patch: $19/$1A), a special screen
 * runs ($62; $F9 there would clear it, no route), or the stage is being
 * skipped (game_action $0E). While the player holds an enemy, its bar is
 * the one shown.
 * Called by: gameplay_update, JSR at $C981 (flow_gameplay.c).
 */
void func_E6AB(void) {
    REGS_IN;

    mff_cycles(0xE6AB, 8);
    a = RAM(VRAM_LIST_REQUEST) | RAM(VRAM_COLUMN_REQUEST);
    SET_NZ(a);
    if (a != 0) {
        mff_cycles(0xE6AF, 1 + 6);
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0xE6B1, 5);
    a = RAM(SPECIAL_SCREEN_ID);
    if (a != 0) {
        mff_cycles(0xE6B5, 4);
        SET_CMP(a, 0xF9);
        if (!z) {
            mff_cycles(0xE6B7, 1 + 6);
            SYNC_OUT;
            mff_rts();
            return;
        }
        mff_cycles(0xE6B9, 11);                 /* no route */
        a = 0;
        SET_NZ(a);
        RAM(SPECIAL_SCREEN_ID) = a;
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0xE6B3, 1 + 7);
    a = RAM(GAME_ACTION);
    SET_CMP(a, GA_SKIP_STAGE);
    if (z) {
        mff_cycles(0xE6C2, 1 + 6);
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0xE6C4, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x06C6, func_E6DD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE6C7, 7);
    a = RAM(GAME_ACTION);
    SET_CMP(a, GA_GRAB);
    if (z) {
        mff_cycles(0xE6CD, 11);
        x = RAM(GRAB_SLOT);
        RAM(HUD_ENEMY_SLOT) = x;
        a = 0x00;
        SET_NZ(a);
        RAM(HUD_ENEMY_TIMER) = a;
    } else {
        mff_cycles(0xE6CB, 1);
    }
    mff_cycles(0xE6D5, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x06D7, func_E89B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xE6D8, 5 + 6);
    a = 0xFF;
    SET_NZ(a);
    RAM(VRAM_REQUEST) = a;
    SYNC_OUT;
    mff_rts();
}

/*
 * hud_player, $E6DD (proposed name): the player's part of the bar (all but
 * the enemy's): health (not during Guy's power move, game_action $27),
 * round, lives, level mark and experience, then hud_next_level.
 * In: vram_buffer_len. Out: X = the buffer's new end (vram_buffer_len).
 * Called by: hud_update, JSR at $E6C4.
 */
void func_E6DD(void) {
    REGS_IN;
    int table;

    mff_cycles(0xE6DD, 10);
    x = RAM(VRAM_BUFFER_LEN);
    a = RAM(GAME_ACTION);
    if (a == GA_GUY_POWER) {
        mff_cycles(0xE6E3, 2);                  /* BEQ across a page */
    } else {
        /* the bar's tiles: Haggar's, Guy's or Cody's table by level; a 0
         * would fall through to the next table, but none holds one */
        mff_cycles(0xE6E5, 8);
        y = RAM(PLAYER_LEVEL);
        a = RAM(PLAYER_ID);
        if (a == PLAYER_CODY) {
            mff_cycles(0xE6E9, 1);
            table = PLAYER_CODY;
        } else {
            mff_cycles(0xE6EB, 4);
            if (a == PLAYER_GUY) {
                mff_cycles(0xE6ED, 1);
                table = PLAYER_GUY;
            } else {
                table = PLAYER_HAGGAR;
            }
        }
        if (table == PLAYER_HAGGAR) {
            mff_cycles(0xE6EF, 4);
            a = prg_read_ix(K_HEALTH_BAR_HAGGAR, y);
            mff_cycles(0xE6F2, 2);
            if (a != 0) mff_cycles(0xE6F2, 1);
            else table = PLAYER_GUY;
        }
        if (table == PLAYER_GUY) {
            mff_cycles(0xE6F4, 4);
            a = prg_read_ix(K_HEALTH_BAR_GUY, y);
            mff_cycles(0xE6F7, 2);
            if (a != 0) mff_cycles(0xE6F7, 1);
            else table = PLAYER_CODY;
        }
        if (table == PLAYER_CODY) {
            mff_cycles(0xE6F9, 4);
            a = prg_read_ix(K_HEALTH_BAR_CODY, y);
        }

        /* [count + 1, $20, $4A, the tiles, the right end] */
        mff_cycles(0xE6FC, 40);
        RAM(VRAM_BUFFER + x) = (uint8_t)(a + 1);
        RAM(TMP0) = a;
        x++;
        RAM(VRAM_BUFFER + x) = 0x20;
        x++;
        RAM(VRAM_BUFFER + x) = 0x4A;
        x++;
        a = RAM(HEALTH);
        if (a & 0x80) {
            mff_cycles(0xE715, 2);
            a = 0x00;
        } else {
            mff_cycles(0xE713, 1);
        }
        mff_cycles(0xE717, 3);
        RAM(TMP1) = a;
        do {
            a = bar_tile(0xE719, RAM(TMP1));
            mff_cycles(0xE72B, 26);
            a |= TILE_BAR_EMPTY;
            RAM(VRAM_BUFFER + x) = a;
            RAM(TMP1) = (uint8_t)(RAM(TMP1) - 8);
            x++;
            t = (uint8_t)(RAM(TMP0) - 1);
            RAM(TMP0) = t;
            if (t != 0) mff_cycles(0xE73A, 1);
        } while (t != 0);
        mff_cycles(0xE73C, 9);
        RAM(VRAM_BUFFER + x) = TILE_BAR_RIGHT;
        x++;
    }

    /* [1, $20, $5C, round], [1, $20, $7C, lives], [1, $23, $49 + level,
     * the mark] */
    mff_cycles(0xE742, 134);
    RAM(VRAM_BUFFER + x) = 0x01;
    x++;
    RAM(VRAM_BUFFER + x) = 0x20;
    x++;
    RAM(VRAM_BUFFER + x) = 0x5C;
    x++;
    RAM(VRAM_BUFFER + x) = (uint8_t)(RAM(ROUND) + 2);
    x++;
    RAM(VRAM_BUFFER + x) = 0x01;
    x++;
    RAM(VRAM_BUFFER + x) = 0x20;
    x++;
    RAM(VRAM_BUFFER + x) = 0x7C;
    x++;
    RAM(VRAM_BUFFER + x) = (uint8_t)((RAM(LIVES) & 0x0F) + 1);
    x++;
    RAM(VRAM_BUFFER + x) = 0x01;
    x++;
    RAM(VRAM_BUFFER + x) = 0x23;
    x++;
    a = 0x49;
    c = 0;
    ADC_A(RAM(PLAYER_LEVEL));
    RAM(VRAM_BUFFER + x) = a;
    if (RAM(PLAYER_ID) == PLAYER_HAGGAR) {
        mff_cycles(0xE797, 14);
        RAM(VRAM_BUFFER + x) = (uint8_t)(RAM(VRAM_BUFFER + x) + 2);
    } else {
        mff_cycles(0xE795, 1);
    }

    /* [3, $23, $56, experience] */
    mff_cycles(0xE79D, 45);
    x++;
    RAM(VRAM_BUFFER + x) = TILE_LEVEL_MARK;
    x++;
    RAM(VRAM_BUFFER + x) = 0x03;
    x++;
    RAM(VRAM_BUFFER + x) = 0x23;
    x++;
    RAM(VRAM_BUFFER + x) = 0x56;
    x++;
    a = RAM(PLAYER_ID);
    if (a == PLAYER_HAGGAR) {
        mff_cycles(0xE7BA, 1 + 7);
        a = RAM(PLAYER_LEVEL);
        SET_CMP(a, TOP_LEVEL_HAGGAR);
        if (z) mff_cycles(0xE7C8, 1);
    } else {
        mff_cycles(0xE7BC, 7);
        a = RAM(PLAYER_LEVEL);
        SET_CMP(a, TOP_LEVEL);
        mff_cycles(0xE7C0, z ? 1 : 2 + 1);
    }
    if (z) {
        /* the top level: dashes */
        mff_cycles(0xE7F2, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x07F4, func_E856, 0xE000)) return;
        func_E7F5();
        return;
    }
    mff_cycles(0xE7CA, 68);
    RAM(VRAM_BUFFER + x) = (uint8_t)((RAM(PLAYER_EXP + 1) & 0x0F) + 1);
    x++;
    a = (uint8_t)((RAM(PLAYER_EXP) & 0xF0) >> 4);
    c = 0;                                      /* LSRs of a value & $F0 shift out zeros */
    RAM(VRAM_BUFFER + x) = (uint8_t)(a + 1);
    x++;
    a = RAM(PLAYER_EXP) & 0x0F;
    RAM(VRAM_BUFFER + x) = (uint8_t)(a + 1);
    x++;
    SET_NZ(x);
    SYNC_OUT;
    stage_jmp(0xE7F5, func_E7F5);
}

/*
 * hud_next_level, $E7F5 (proposed name): [3, $23, $5A, the experience the
 * next level needs] from the character's table, or dashes at the top level;
 * then hud_close.
 * In: X = the buffer's end. Called by: hud_player (JMP at $E7EF, and
 * falling in at $E7F5 after the dashes).
 */
void func_E7F5(void) {
    REGS_IN;

    mff_cycles(0xE7F5, 34);
    RAM(VRAM_BUFFER + x) = 0x03;
    x++;
    RAM(VRAM_BUFFER + x) = 0x23;
    x++;
    RAM(VRAM_BUFFER + x) = 0x5A;
    x++;
    y = RAM(PLAYER_ID);
    if (y == PLAYER_HAGGAR) {
        mff_cycles(0xE80B, 1 + 7);
        a = RAM(PLAYER_LEVEL);
        SET_CMP(a, TOP_LEVEL_HAGGAR);
        if (z) mff_cycles(0xE819, 1);
    } else {
        mff_cycles(0xE80D, 7);
        a = RAM(PLAYER_LEVEL);
        SET_CMP(a, TOP_LEVEL);
        mff_cycles(0xE811, z ? 1 : 2 + 1);
    }
    if (z) {
        mff_cycles(0xE850, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0852, func_E856, 0xE000)) return;
        func_E853();
        return;
    }

    /* $00/$01 = the character's table; entry level * 2 */
    mff_cycles(0xE81B, 4);
    a = prg_read_ix(K_NEXT_LEVEL_LO, y);
    mff_cycles(0xE81E, 7);
    RAM(TMP0) = a;
    a = prg_read_ix(K_NEXT_LEVEL_HI, y);
    mff_cycles(0xE823, 15);
    RAM(TMP1) = a;
    a = (uint8_t)(RAM(PLAYER_LEVEL) << 1);
    y = a;
    a = mem_read_iy(TMP0, y);          /* hundreds */
    mff_cycles(0xE82B, 21);
    RAM(VRAM_BUFFER + x) = (uint8_t)(a + 1);
    x++;
    y++;
    a = mem_read_iy(TMP0, y);          /* tens and ones */
    mff_cycles(0xE835, 29);
    c = 0;                                      /* LSRs of a value & $F0 shift out zeros */
    a = (uint8_t)((a & 0xF0) >> 4);
    RAM(VRAM_BUFFER + x) = (uint8_t)(a + 1);
    x++;
    a = mem_read_iy(TMP0, y);
    mff_cycles(0xE844, 19);
    a &= 0x0F;
    RAM(VRAM_BUFFER + x) = (uint8_t)(a + 1);
    x++;
    SET_NZ(x);
    SYNC_OUT;
    stage_jmp(0xE853, func_E853);
}

/*
 * hud_close, $E853 (proposed name): vram_buffer_len = X.
 * Called by: hud_next_level (JMP at $E84D, falling in after the dashes).
 */
void func_E853(void) {
    REGS_IN;

    mff_cycles(0xE853, 9);
    RAM(VRAM_BUFFER_LEN) = x;
    SYNC_OUT;
    mff_rts();
}

/*
 * hud_dashes, $E856 (proposed name): three dash tiles ($78) at X.
 * Out: X += 3. Called by: hud_player ($E7F2), hud_next_level ($E850).
 */
void func_E856(void) {
    REGS_IN;

    mff_cycles(0xE856, 33);
    a = TILE_DASH;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    RAM(VRAM_BUFFER + x) = a;
    x++;
    SET_NZ(x);
    SYNC_OUT;
    mff_rts();
}

/*
 * $E8C9-$E953, shared by hud_enemy_bar and hud_enemy_bar_shown: the bar of
 * slot A, [15, $20, $69, 15 tiles]: blank when the slot has no life to
 * show (obj_life_max / 8 = 0), after $96 frames of hud_enemy_timer, or at
 * the end of a death's $1E blinking frames.
 */
static void enemy_bar_draw(void) {
    REGS_IN;
    enum { BAR, BLINK_OFF, NO_BAR } show = BAR;

    mff_cycles(0xE8C9, 50);
    y = a;
    x = RAM(VRAM_BUFFER_LEN);
    a = 0x0F;
    RAM(VRAM_BUFFER + x) = a;
    RAM(TMP2) = a;
    x++;
    RAM(VRAM_BUFFER + x) = 0x20;
    x++;
    RAM(VRAM_BUFFER + x) = 0x69;
    x++;
    a = (uint8_t)(RAM(OBJ_LIFE_MAX + y) >> 3);
    c = (RAM(OBJ_LIFE_MAX + y) >> 2) & 1;
    RAM(TMP0) = a;                              /* $00: the bar's tiles */

    /* what to show (the BEQs to $E93F cross a page) */
    if (a == 0) {
        mff_cycles(0xE8E8, 2);
        show = NO_BAR;
    } else {
        mff_cycles(0xE8EA, 12);
        RAM(HUD_ENEMY_TIMER)++;
        a = RAM(HUD_ENEMY_TIMER);
        c = a >= ENEMY_BAR_FRAMES;
        if (a == ENEMY_BAR_FRAMES) {
            mff_cycles(0xE8F0, 2);
            show = NO_BAR;
        } else {
            mff_cycles(0xE8F2, 6);
            a = RAM(OBJ_LIFE + y);
            if (!(a & 0x80)) {
                mff_cycles(0xE8F5, 2);          /* BPL across a page */
            } else {
                /* dead: an empty bar that blinks, then none */
                mff_cycles(0xE8F7, 7);
                a = RAM(HUD_ENEMY_TIMER);
                c = a >= ENEMY_BAR_DEATH_FRAMES;
                if (a == ENEMY_BAR_DEATH_FRAMES) {
                    mff_cycles(0xE8FB, 2);
                    show = NO_BAR;
                } else {
                    mff_cycles(0xE8FD, 4);
                    a &= 0x02;
                    if (a != 0) {
                        mff_cycles(0xE8FF, 1);
                        show = BLINK_OFF;
                    } else {
                        mff_cycles(0xE901, 2);
                        a = 0x00;
                    }
                }
            }
        }
    }

    if (show == BAR) {
        /* $19, the life tiles, $1A */
        mff_cycles(0xE903, 17);
        RAM(TMP1) = a;
        RAM(VRAM_BUFFER + x) = TILE_BAR_LEFT;
        x++;
        RAM(TMP2)--;
        do {
            a = bar_tile(0xE90D, RAM(TMP1));
            mff_cycles(0xE91F, 31);
            a |= TILE_BAR_EMPTY;
            RAM(VRAM_BUFFER + x) = a;
            x++;
            RAM(TMP2)--;
            a = RAM(TMP1);
            c = 1;
            SBC_A(8);
            RAM(TMP1) = a;
            t = (uint8_t)(RAM(TMP0) - 1);
            RAM(TMP0) = t;
            if (t != 0) mff_cycles(0xE930, 1);
        } while (t != 0);
        mff_cycles(0xE932, 16);
        a = TILE_BAR_RIGHT;
        RAM(VRAM_BUFFER + x) = a;
        x++;
        t = (uint8_t)(RAM(TMP2) - 1);
        RAM(TMP2) = t;
        SET_NZ(t);
        if (z) {
            mff_cycles(0xE93C, 9);
            RAM(VRAM_BUFFER_LEN) = x;
            SYNC_OUT;
            mff_rts();
            return;
        }
        mff_cycles(0xE93A, 1);
    } else if (show == NO_BAR) {
        /* 15 blank tiles, and the timer parked at its end */
        mff_cycles(0xE93F, 10);
        a = ENEMY_BAR_FRAMES - 1;
        RAM(HUD_ENEMY_TIMER) = a;
        a = 0x0F;
        RAM(TMP2) = a;
    }

    /* blank tiles up to 15 */
    for (;;) {
        mff_cycles(0xE947, 16);
        a = 0x00;
        RAM(VRAM_BUFFER + x) = a;
        x++;
        t = (uint8_t)(RAM(TMP2) - 1);
        RAM(TMP2) = t;
        SET_NZ(t);
        if (z) break;
        mff_cycles(0xE94F, 1);
    }
    mff_cycles(0xE951, 9);
    RAM(VRAM_BUFFER_LEN) = x;
    SYNC_OUT;
    mff_rts();
}

/*
 * hud_enemy_bar, $E89B: chooses the slot whose bar is shown. A living boss
 * (k_boss_kinds) among slots 2-15 takes it at once (hud_enemy_timer 1);
 * otherwise hud_enemy_slot keeps the enemy last hit or held, and 0 shows
 * nothing (the bar is not even written).
 * Called by: hud_update, JSR at $E6D5.
 */
void func_E89B(void) {
    REGS_IN;
    unsigned i;

    mff_cycles(0xE89B, 2);
    y = 0x02;
    do {
        mff_cycles(0xE89D, 4);
        a = RAM(OBJ_KIND + y);
        for (i = 0; i < sizeof k_boss_kinds; i++) {
            mff_cycles((uint16_t)(0xE8A0 + 4 * i), 4);  /* CMP #, BEQ */
            c = a >= k_boss_kinds[i];
            if (a == k_boss_kinds[i]) {
                /* a boss: its bar if it lives */
                mff_cycles((uint16_t)(0xE8A2 + 4 * i), 1 + 6);
                a = RAM(OBJ_LIFE + y);
                if (a & 0x80) {                 /* dead: the usual bar */
                    SYNC_OUT;
                    func_E8BD();
                    return;
                }
                mff_cycles(0xE8BB, 1 + 10);
                a = 0x01;
                RAM(HUD_ENEMY_TIMER) = a;
                RAM(HUD_ENEMY_SLOT) = y;
                a = y;
                SYNC_OUT;
                enemy_bar_draw();
                return;
            }
        }
        mff_cycles(0xE8B0, 6);
        y++;
        SET_CMP(y, 0x10);
        if (!z) mff_cycles(0xE8B3, 1);
    } while (!z);
    mff_cycles(0xE8B5, 3);
    SYNC_OUT;
    stage_jmp(0xE8BD, func_E8BD);
}

/*
 * hud_enemy_bar_shown, $E8BD (proposed name): the bar of hud_enemy_slot, or
 * nothing when it is 0.
 * Called by: hud_enemy_bar (JMP at $E8B5 when no boss is on, falling in
 * at $E8BD when the boss found is dead).
 */
void func_E8BD(void) {
    REGS_IN;

    mff_cycles(0xE8BD, 5);
    a = RAM(HUD_ENEMY_SLOT);
    SET_NZ(a);
    if (z) {
        mff_cycles(0xE8C1, 6);
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0xE8BF, 1);
    SYNC_OUT;
    enemy_bar_draw();
}

/*
 * hud_level_gauge, $E954 (proposed name): [6, $23, $49, six gauge tiles]
 * for the player's level (Haggar's table entry two further on).
 * Called by: the scenes of banks 04 (04:9A9A) and 0A (0A:9C18).
 */
void func_E954(void) {
    REGS_IN;

    mff_cycles(0xE954, 40);
    x = RAM(VRAM_BUFFER_LEN);
    RAM(VRAM_BUFFER + x) = 0x06;
    x++;
    RAM(VRAM_BUFFER + x) = 0x23;
    x++;
    RAM(VRAM_BUFFER + x) = 0x49;
    x++;
    y = RAM(PLAYER_LEVEL);
    a = RAM(PLAYER_ID);
    if (a == PLAYER_HAGGAR) {
        mff_cycles(0xE970, 4);
        y += 2;
    } else {
        mff_cycles(0xE96E, 1);
    }
    mff_cycles(0xE972, 4);
    a = prg_read_ix(K_GAUGE_LO, y);
    mff_cycles(0xE975, 7);
    RAM(TMP0) = a;
    a = prg_read_ix(K_GAUGE_HI, y);
    mff_cycles(0xE97A, 5);
    RAM(TMP1) = a;
    y = 0x00;
    for (;;) {
        mff_cycles(0xE97E, 5);
        a = mem_read_iy(TMP0, y);
        mff_cycles(0xE980, 13);
        RAM(VRAM_BUFFER + x) = a;
        x++;
        y++;
        SET_CMP(y, 6);
        if (z) break;
        mff_cycles(0xE987, 1);
    }
    mff_cycles(0xE989, 9);
    RAM(VRAM_BUFFER_LEN) = x;
    SYNC_OUT;
    mff_rts();
}

/* the bar's fixed text in bank 0A: vram_buffer entries ended by $FF
 * (PLAYER, ENEMY, RD, PL, the "/", LEVEL, EXP) */
#define K_HUD_TEXT           0x9C20

/*
 * hud_draw_text, 0A:9C00 (labels.toml; the port's func_9C00_b5, 16 KiB
 * bank 5 = 8 KiB banks 0A/0B): copies K_HUD_TEXT up to its $FF to the end
 * of vram_buffer, special_screen_step = 0, the level gauge
 * (hud_level_gauge), vram_request = $FF.
 * Out: X and C as hud_level_gauge leaves them, A = $FF.
 * Called by: main_task's game start (0E:C77F) and the area start in
 * player_state_transition (0E:E015), with bank 0A mapped at $8000.
 */
void func_9C00_b5(void) {
    REGS_IN;

    mff_cycles(0x9C00, 5);
    x = RAM(VRAM_BUFFER_LEN);
    y = 0x00;
    for (;;) {
        mff_cycles(0x9C04, 4);
        a = prg_read_ix(K_HUD_TEXT, y);
        mff_cycles(0x9C07, 4);
        c = a >= 0xFF;
        if (a == 0xFF) {
            mff_cycles(0x9C09, 1);
            break;
        }
        mff_cycles(0x9C0B, 11);
        RAM(VRAM_BUFFER + x) = a;
        y++;
        x++;
        if (x == 0)
            break;
        mff_cycles(0x9C10, 1);
    }
    mff_cycles(0x9C12, 14);
    RAM(VRAM_BUFFER_LEN) = x;
    a = 0x00;
    SET_NZ(a);
    RAM(SPECIAL_SCREEN_STEP) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x1C1A, func_E954, 0xE000)) return;   /* hud_level_gauge */
    SYNC_IN;
    mff_cycles(0x9C1B, 11);
    a = 0xFF;
    SET_NZ(a);
    RAM(VRAM_REQUEST) = a;
    SYNC_OUT;
    mff_rts();
}
