/*
 * flow_main_task.c - main_task: from power-on to the first round
 *
 * main_task (0E:C556) is task 0, which reset spawns (tasks.c). It runs once
 * through the boot screens, the story, the title, the select screen and the
 * round map, sets up a new game and goes to gameplay (0E:C8ED,
 * flow_gameplay.c), which never comes back here. Left alone, the title restarts
 * the game by JMP reset, and the boot screens show again.
 *
 *   $C556  power-on, LICENSED BY NINTENDO (180 frames), CAPCOM PRESENTS
 *   $C5A3  waits up to 180 frames or a press; checks the ROM's name block
 *   $C5CE  the story, five pages, cut short by a held button; the title
 *   $C681  the title loop
 *   $C6D2  title_wait_start: START goes on at $C6D8
 *   $C6D8  the select screen, the round map, a new game, its first area
 *          (title_start_game below), then JMP gameplay
 *
 * The lifter made a routine at each address a JMP or a branch comes back
 * to ($C5A3, $C5CE, $C681, $C6D2), and each carried a copy of everything
 * after it. Here each routine runs its own part and goes into the next
 * one by calling it, where the 6502 falls through; title_start_game is the
 * part both $C681 and $C6D2 reach. The JMPs and branches back that the
 * lifter turned into tail calls by address stay tail calls.
 *
 * The screens are in bank 04 (flow.h, SCREEN_*), called by address since
 * their bank is switched: screen_draw (04:8000) and each screen's setup.
 */
#include "flow.h"

void func_C5A3(void);
void func_C681(void);
static void title_start_game(void);

/*
 * main_task ($C556, labels.toml)
 * Task 0's entry: reset puts $C556 in task_entry (0F:FEA9-FEB3) and the
 * scheduler starts it (0F:FEE7-FEEF). Takes S = $BF for task 0's stack,
 * writes PPUCTRL from its shadow, stops the sound, waits a frame, seeds
 * rng, shows LICENSED BY NINTENDO for 180 frames and then CAPCOM PRESENTS
 * with action_timer = 180, and goes on into the wait at $C5A3.
 */
void func_C556(void) {
    REGS_IN;

    mff_cycles(0xC556, 11);
    x = 0xBF;
    g_cpu.S = x;
    a = RAM(PPUCTRL_SHADOW);
    hw_write(PPU_CTRL, a);
    mff_cycles(0xC55E, 8);
    a = SOUND_STOP_ALL;
    SET_NZ(a);
    SYNC_OUT;
    if (!mff_call_fn(0x0562, func_F34A, 0xE000)) return;   /* sound_queue_push */
    SYNC_IN;
    mff_cycles(0xC563, 6);
    SYNC_OUT;
    mff_call_fn(0x0565, func_FF31, 0xE000);                /* task_wait_frame */
    SYNC_IN;
    mff_cycles(0xC566, 11);
    a = RNG_SEED;
    SET_NZ(a);
    RAM(RNG) = a;
    SYNC_OUT;
    if (!mff_call_at(0x056C, 0xC85C, -1)) return;          /* screen_clear */
    SYNC_IN;

    /* LICENSED BY NINTENDO */
    mff_cycles(0xC56D, 16);
    x = BANKS_SCREENS;
    RAM(BANK_REQUEST) = x;
    x++;
    SET_NZ(x);
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0576, func_FF4D, 0xE000)) return;   /* bank_switch */
    SYNC_IN;
    mff_cycles(0xC577, 8);
    x = SCREEN_LICENSE;
    SET_NZ(x);
    SYNC_OUT;
    if (!mff_call_at(0x057B, 0x8000, -1)) return;          /* 04:8000 screen_draw */
    SYNC_IN;
    mff_cycles(0xC57C, 6);
    SYNC_OUT;
    if (!mff_call_at(0x057E, 0xC26F, -1)) return;          /* rendering_on */
    SYNC_IN;
    mff_cycles(0xC57F, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0581, 0x89C0, -1)) return;          /* 04:89C0 its palette and CHR */
    SYNC_IN;
    mff_cycles(0xC582, 8);
    x = BOOT_SCREEN_FRAMES;
    SET_NZ(x);
    SYNC_OUT;
    mff_call_fn(0x0586, func_FF2A, 0xE000);                /* task_wait_frames */
    SYNC_IN;
    mff_cycles(0xC587, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0589, 0xC85C, -1)) return;          /* screen_clear */
    SYNC_IN;

    /* CAPCOM PRESENTS */
    mff_cycles(0xC58A, 16);
    x = BANKS_SCREENS;
    RAM(BANK_REQUEST) = x;
    x++;
    SET_NZ(x);
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0593, func_FF4D, 0xE000)) return;   /* bank_switch */
    SYNC_IN;
    mff_cycles(0xC594, 8);
    x = SCREEN_CAPCOM;
    SET_NZ(x);
    SYNC_OUT;
    if (!mff_call_at(0x0598, 0x8000, -1)) return;          /* 04:8000 screen_draw */
    SYNC_IN;
    mff_cycles(0xC599, 6);
    SYNC_OUT;
    if (!mff_call_at(0x059B, 0xC26F, -1)) return;          /* rendering_on */
    SYNC_IN;
    mff_cycles(0xC59C, 6);
    SYNC_OUT;
    if (!mff_call_at(0x059E, 0x8900, -1)) return;          /* 04:8900 palette, CHR, fade in */
    SYNC_IN;
    mff_cycles(0xC59F, 5);
    a = BOOT_SCREEN_FRAMES;
    SET_NZ(a);
    RAM(ACTION_TIMER) = a;
    SYNC_OUT;
    func_C5A3();
}

/*
 * $C5A3: the CAPCOM PRESENTS wait, then rom_integrity_check ($C5B3,
 * labels.toml)
 * Waits a frame at a time until a button is pressed or action_timer
 * (counted down here) is 0. Then $FFF8 must hold 8 and the 16-byte name
 * block at $FFE0 must match the copy at $C8DD ("M FINAL FIGHTY  "); if so,
 * JMP $C5CE, else the task hangs at $C5CA for good (no route gets there).
 * Callers: main_task falls into it; the loop's JMP comes back to it.
 */
void func_C5A3(void) {
    REGS_IN;

    for (;;) {
        mff_cycles(0xC5A3, 6);
        SYNC_OUT;
        mff_call_fn(0x05A5, func_FF31, 0xE000);            /* task_wait_frame */
        SYNC_IN;
        mff_cycles(0xC5A6, 5);
        a = RAM(JOY_PRESSED);
        if (a != 0) {
            mff_cycles(0xC5A8, 1);
            break;
        }
        mff_cycles(0xC5AA, 5);
        a = RAM(ACTION_TIMER);
        if (a == 0) {
            mff_cycles(0xC5AC, 1);
            break;
        }
        mff_cycles(0xC5AE, 8);                              /* DEC, JMP $C5A3 */
        t = (uint8_t)(RAM(ACTION_TIMER) - 1);
        RAM(ACTION_TIMER) = t;
        SET_NZ(t);
    }

    mff_cycles(0xC5B3, 6);
    x = 0x0F;
    a = prg_read(0xFFF8);
    mff_cycles(0xC5B8, 4);
    c = a >= 0x08;
    if (a == 0x08) {
        for (;;) {                                          /* X = $0F down to 0 */
            mff_cycles(0xC5BC, 4);
            a = prg_read_ix(0xFFE0, x);
            mff_cycles(0xC5BF, 4);
            m = prg_read_ix(0xC8DD, x);
            c = a >= m;
            mff_cycles(0xC5C2, 2);
            if (a != m) {
                mff_cycles(0xC5C2, 1);
                break;
            }
            mff_cycles(0xC5C4, 4);
            x--;
            SET_NZ(x);
            if (n) {
                /* all 16 match */
                mff_cycles(0xC5C7, 3);                      /* JMP $C5CE */
                SYNC_OUT;
                nes_cpu_instruction_boundary(0xC5CE, 2); call_by_address_tail(0xC5CE, -1); return;
            }
            mff_cycles(0xC5BC, 1);
        }
    } else {
        mff_cycles(0xC5BA, 1);
    }
    for (;;) {                                              /* $C5CA: LDA #0, BEQ $C5CA */
        mff_cycles(0xC5CA, 4);
        a = 0;
        SET_NZ(a);
        mff_cycles(0xC5CA, 1);
    }
}

/*
 * $C5CE: the story, then title_screen ($C657, labels.toml)
 * The story is five pages, intro_page 0-4, each drawn by 00:9A00: the text
 * pages 0, 2 and 4 on story_text_screen ($C888), the pictures of pages 1
 * and 3 on screens 6 and 7, with the story music from page 1 on. A button
 * held after any page skips the rest. The title: music $0C, screen 1 and
 * its setup (04:8940, which also starts the logo object in slot 13), the
 * three title timers ($FF, $FF, $8B); then the title loop ($C681).
 * Caller: $C5A3 by JMP once the ROM check passes (0E:C5C7).
 */
void func_C5CE(void) {
    REGS_IN;

    do {
        /* page 0 */
        mff_cycles(0xC5CE, 6);
        SYNC_OUT;
        if (!mff_call_at(0x05D0, 0xC888, -1)) return;      /* story_text_screen */
        SYNC_IN;
        mff_cycles(0xC5D1, 11);
        a = 0;
        SET_NZ(a);
        RAM(INTRO_PAGE) = a;
        SYNC_OUT;
        if (!mff_call_at(0x05D7, 0x9A00, -1)) return;      /* 00:9A00 draws intro_page */
        SYNC_IN;
        mff_cycles(0xC5D8, 5);
        a = RAM(JOY_HELD);
        if (a != 0) {
            mff_cycles(0xC5DA, 2);                          /* BNE $C657 crosses a page */
            break;
        }

        /* page 1: the first picture, and the story music */
        mff_cycles(0xC5DC, 8);
        a = MUSIC_STORY;
        SET_NZ(a);
        SYNC_OUT;
        if (!mff_call_fn(0x05E0, func_F34A, 0xE000)) return;   /* sound_queue_push */
        SYNC_IN;
        mff_cycles(0xC5E1, 6);
        SYNC_OUT;
        if (!mff_call_at(0x05E3, 0xC85C, -1)) return;      /* screen_clear */
        SYNC_IN;
        mff_cycles(0xC5E4, 16);
        x = BANKS_SCREENS;
        RAM(BANK_REQUEST) = x;
        x++;
        SET_NZ(x);
        RAM(BANK_REQUEST + 1) = x;
        SYNC_OUT;
        if (!mff_call_fn(0x05ED, func_FF4D, 0xE000)) return;   /* bank_switch */
        SYNC_IN;
        mff_cycles(0xC5EE, 8);
        x = SCREEN_STORY_1;
        SET_NZ(x);
        SYNC_OUT;
        if (!mff_call_at(0x05F2, 0x8000, -1)) return;      /* 04:8000 screen_draw */
        SYNC_IN;
        mff_cycles(0xC5F3, 6);
        SYNC_OUT;
        if (!mff_call_at(0x05F5, 0xC26F, -1)) return;      /* rendering_on */
        SYNC_IN;
        mff_cycles(0xC5F6, 6);
        SYNC_OUT;
        if (!mff_call_at(0x05F8, 0x8B40, -1)) return;      /* 04:8B40 its palette, CHR, figure */
        SYNC_IN;
        mff_cycles(0xC5F9, 16);
        x = BANKS_PLAYER;
        RAM(BANK_REQUEST) = x;
        x++;
        SET_NZ(x);
        RAM(BANK_REQUEST + 1) = x;
        SYNC_OUT;
        if (!mff_call_fn(0x0602, func_FF4D, 0xE000)) return;   /* bank_switch */
        SYNC_IN;
        mff_cycles(0xC603, 11);
        a = 1;
        SET_NZ(a);
        RAM(INTRO_PAGE) = a;
        SYNC_OUT;
        if (!mff_call_at(0x0609, 0x9A00, -1)) return;      /* 00:9A00 draws intro_page */
        SYNC_IN;
        mff_cycles(0xC60A, 5);
        a = RAM(JOY_HELD);
        SET_NZ(a);
        if (a != 0) {
            mff_cycles(0xC60C, 1);
            break;
        }

        /* page 2 */
        mff_cycles(0xC60E, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0610, 0xC888, -1)) return;      /* story_text_screen */
        SYNC_IN;
        mff_cycles(0xC611, 11);
        a = 2;
        SET_NZ(a);
        RAM(INTRO_PAGE) = a;
        SYNC_OUT;
        if (!mff_call_at(0x0617, 0x9A00, -1)) return;      /* 00:9A00 draws intro_page */
        SYNC_IN;
        mff_cycles(0xC618, 5);
        a = RAM(JOY_HELD);
        SET_NZ(a);
        if (a != 0) {
            mff_cycles(0xC61A, 1);
            break;
        }

        /* page 3: the second picture */
        mff_cycles(0xC61C, 6);
        SYNC_OUT;
        if (!mff_call_at(0x061E, 0xC85C, -1)) return;      /* screen_clear */
        SYNC_IN;
        mff_cycles(0xC61F, 16);
        x = BANKS_SCREENS;
        RAM(BANK_REQUEST) = x;
        x++;
        SET_NZ(x);
        RAM(BANK_REQUEST + 1) = x;
        SYNC_OUT;
        if (!mff_call_fn(0x0628, func_FF4D, 0xE000)) return;   /* bank_switch */
        SYNC_IN;
        mff_cycles(0xC629, 8);
        x = SCREEN_STORY_2;
        SET_NZ(x);
        SYNC_OUT;
        if (!mff_call_at(0x062D, 0x8000, -1)) return;      /* 04:8000 screen_draw */
        SYNC_IN;
        mff_cycles(0xC62E, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0630, 0xC26F, -1)) return;      /* rendering_on */
        SYNC_IN;
        mff_cycles(0xC631, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0633, 0x8AC0, -1)) return;      /* 04:8AC0 its palette, CHR, figure */
        SYNC_IN;
        mff_cycles(0xC634, 16);
        x = BANKS_PLAYER;
        RAM(BANK_REQUEST) = x;
        x++;
        SET_NZ(x);
        RAM(BANK_REQUEST + 1) = x;
        SYNC_OUT;
        if (!mff_call_fn(0x063D, func_FF4D, 0xE000)) return;   /* bank_switch */
        SYNC_IN;
        mff_cycles(0xC63E, 11);
        a = 3;
        SET_NZ(a);
        RAM(INTRO_PAGE) = a;
        SYNC_OUT;
        if (!mff_call_at(0x0644, 0x9A00, -1)) return;      /* 00:9A00 draws intro_page */
        SYNC_IN;
        mff_cycles(0xC645, 5);
        a = RAM(JOY_HELD);
        SET_NZ(a);
        if (a != 0) {
            mff_cycles(0xC647, 1);
            break;
        }

        /* page 4 */
        mff_cycles(0xC649, 6);
        SYNC_OUT;
        if (!mff_call_at(0x064B, 0xC888, -1)) return;      /* story_text_screen */
        SYNC_IN;
        mff_cycles(0xC64C, 11);
        a = 4;
        SET_NZ(a);
        RAM(INTRO_PAGE) = a;
        SYNC_OUT;
        if (!mff_call_at(0x0652, 0x9A00, -1)) return;      /* 00:9A00 draws intro_page */
        SYNC_IN;
        mff_cycles(0xC653, 5);
        a = RAM(JOY_HELD);
        if (a != 0)
            mff_cycles(0xC655, 1);
    } while (0);

    /* title_screen */
    mff_cycles(0xC657, 8);
    a = MUSIC_TITLE;
    SET_NZ(a);
    SYNC_OUT;
    if (!mff_call_fn(0x065B, func_F34A, 0xE000)) return;   /* sound_queue_push */
    SYNC_IN;
    mff_cycles(0xC65C, 6);
    SYNC_OUT;
    if (!mff_call_at(0x065E, 0xC85C, -1)) return;          /* screen_clear */
    SYNC_IN;
    mff_cycles(0xC65F, 16);
    x = BANKS_SCREENS;
    RAM(BANK_REQUEST) = x;
    x++;
    SET_NZ(x);
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0668, func_FF4D, 0xE000)) return;   /* bank_switch */
    SYNC_IN;
    mff_cycles(0xC669, 8);
    x = SCREEN_TITLE;
    SET_NZ(x);
    SYNC_OUT;
    if (!mff_call_at(0x066D, 0x8000, -1)) return;          /* 04:8000 screen_draw */
    SYNC_IN;
    mff_cycles(0xC66E, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0670, 0x8940, -1)) return;          /* 04:8940 palette, CHR, the logo */
    SYNC_IN;
    mff_cycles(0xC671, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0673, 0xC26F, -1)) return;          /* rendering_on */
    SYNC_IN;
    mff_cycles(0xC674, 16);
    a = 0xFF;
    RAM(TITLE_TIMER_1) = a;
    RAM(TITLE_TIMER_2) = a;
    a = 0x8B;
    RAM(TITLE_TIMER_3) = a;
    SYNC_OUT;
    func_C681();
}

/*
 * $C681: the title loop, one frame a pass
 * Hides the sprites from oam_index 4 on, draws the objects (the logo),
 * waits a frame, then counts down the first title timer that is not 0.
 * The first one flashes the title when it reaches $F0 (effect $32 and
 * TITLE_FLASH_COLOR in four palette entries); after the first two the code
 * goes to title_wait_start ($C6D2) by JMP, after the third it falls into
 * it ($C6CF), which this routine runs inline. With all three at 0 (about
 * 650 frames) it fades out and JMPs to reset: the game starts over.
 * Callers: $C5CE falls into it; title_wait_start branches back here
 * (0E:C6D6) while START is not pressed.
 */
void func_C681(void) {
    REGS_IN;

    for (;;) {
        mff_cycles(0xC681, 11);
        a = 0x04;
        SET_NZ(a);
        RAM(OAM_INDEX) = a;
        SYNC_OUT;
        if (!mff_call_at(0x0687, 0xC31D, -1)) return;      /* sprites_hide_rest */
        SYNC_IN;
        mff_cycles(0xC688, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x068A, func_E9BC, 0xE000)) return;   /* objects_draw */
        SYNC_IN;
        mff_cycles(0xC68B, 6);
        SYNC_OUT;
        mff_call_fn(0x068D, func_FF31, 0xE000);            /* task_wait_frame */
        SYNC_IN;

        mff_cycles(0xC68E, 6);
        a = RAM(TITLE_TIMER_1);
        if (a != 0) {
            mff_cycles(0xC691, 1);
            mff_cycles(0xC6A7, 14);
            t = (uint8_t)(RAM(TITLE_TIMER_1) - 1);
            RAM(TITLE_TIMER_1) = t;
            a = RAM(TITLE_TIMER_1);
            c = a >= TITLE_FLASH_AT;
            SET_NZ(a - TITLE_FLASH_AT);
            if (z) {
                mff_cycles(0xC6B1, 8);
                a = SFX_TITLE_FLASH;
                SET_NZ(a);
                SYNC_OUT;
                if (!mff_call_fn(0x06B5, func_F34C, 0xE000)) return;   /* sound_queue_push_sfx */
                SYNC_IN;
                mff_cycles(0xC6B6, 21);
                a = TITLE_FLASH_COLOR;
                SET_NZ(a);
                RAM(PALETTE_BUF + 3) = a;
                RAM(PALETTE_BUF + 7) = a;
                RAM(PALETTE_BUF + 15) = a;
                RAM(PALETTE_BUF + 31) = a;
                RAM(PALETTE_REQUEST) = a;
            } else {
                mff_cycles(0xC6AF, 1);
            }
            mff_cycles(0xC6C6, 3);                          /* JMP title_wait_start */
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xC6D2, 2); call_by_address_tail(0xC6D2, -1); return;
        }
        mff_cycles(0xC693, 6);
        a = RAM(TITLE_TIMER_2);
        if (a != 0) {
            mff_cycles(0xC696, 1);
            mff_cycles(0xC6C9, 9);                          /* DEC, JMP title_wait_start */
            t = (uint8_t)(RAM(TITLE_TIMER_2) - 1);
            RAM(TITLE_TIMER_2) = t;
            SET_NZ(t);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xC6D2, 2); call_by_address_tail(0xC6D2, -1); return;
        }
        mff_cycles(0xC698, 6);
        a = RAM(TITLE_TIMER_3);
        if (a == 0) {
            /* time is up: fade out and start over */
            mff_cycles(0xC69D, 11);
            a = 0;
            SET_NZ(a);
            RAM(RENDER_OFF_DEPTH) = a;
            SYNC_OUT;
            if (!mff_call_at(0x06A3, 0xC36E, -1)) return;  /* palette_fade_out */
            SYNC_IN;
            mff_cycles(0xC6A4, 3);                          /* JMP reset */
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xFE00, 2);
            {
                uint16_t swb = g_code_window_base;
                g_code_window_base = 0xE000;
                func_FE00();
                g_code_window_base = swb;
            }
            return;
        }
        mff_cycles(0xC69B, 1);
        /* $C6CF, then title_wait_start's test */
        mff_cycles(0xC6CF, 13);
        t = (uint8_t)(RAM(TITLE_TIMER_3) - 1);
        RAM(TITLE_TIMER_3) = t;
        a = RAM(JOY_PRESSED);
        a &= PAD_START;
        if (a != 0)
            break;
        mff_cycles(0xC681, 1);                              /* BEQ $C681 */
    }
    SYNC_OUT;
    title_start_game();
}

/*
 * title_wait_start ($C6D2, labels.toml)
 * START pressed (joy_pressed bit 4) goes on to title_start_game ($C6D8);
 * otherwise back to the title loop ($C681).
 * Callers: the title loop by JMP from $C6C6 and $C6CC (and by falling in
 * at $C6CF, which func_C681 runs inline).
 */
void func_C6D2(void) {
    REGS_IN;

    mff_cycles(0xC6D2, 7);
    a = RAM(JOY_PRESSED);
    a &= PAD_START;
    SET_NZ(a);
    if (z) {
        mff_cycles(0xC6D6, 1);                              /* BEQ $C681 */
        SYNC_OUT;
        call_by_address_tail((uint16_t)(g_code_window_base + 0x0681), -1);
        return;
    }
    SYNC_OUT;
    title_start_game();
}

/*
 * title_start_game ($C6D8, proposed name): START on the title
 *  - the select screen: effect $30, screen 2; player_id and select_player
 *    start at 0 and 04:9100 runs the screen, leaving the pick in player_id;
 *  - the round map: objects cleared, screen 8 and 04:8D00, 45 frames;
 *  - a new game: CLI with the raster IRQ on, scroll and spawn state 0,
 *    the objects cleared, horizontal mirroring, round 0's music, and
 *    bg_screen = $FF, bg_column = $1F so that round_start_wait ($C768)
 *    streams the first screen in, a column a frame, until bg_screen is 0;
 *  - the first area: the HUD's text (0A:9C00), the area header
 *    (area_flags, area_screen_count), area_load_graphics, the fade in, the
 *    player in slot 0 at ($30, $90) with start_health and its graphics,
 *    and the area's stage list (08:8000, 08:8038, 08:8080 stage_spawns);
 *  - the new game's counters: 5 lives, 3 credits, and 0 for the round and
 *    the rest; then JMP gameplay ($C8ED).
 * Callers: the title loop ($C6CF path) and title_wait_start.
 */
static void title_start_game(void) {
    REGS_IN;

    /* the select screen */
    mff_cycles(0xC6D8, 8);
    a = SFX_START;
    SET_NZ(a);
    SYNC_OUT;
    if (!mff_call_fn(0x06DC, func_F34C, 0xE000)) return;   /* sound_queue_push_sfx */
    SYNC_IN;
    mff_cycles(0xC6DD, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06DF, 0xC85C, -1)) return;          /* screen_clear */
    SYNC_IN;
    mff_cycles(0xC6E0, 16);
    x = BANKS_SCREENS;
    RAM(BANK_REQUEST) = x;
    x++;
    SET_NZ(x);
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x06E9, func_FF4D, 0xE000)) return;   /* bank_switch */
    SYNC_IN;
    mff_cycles(0xC6EA, 8);
    x = SCREEN_SELECT;
    SET_NZ(x);
    SYNC_OUT;
    if (!mff_call_at(0x06EE, 0x8000, -1)) return;          /* 04:8000 screen_draw */
    SYNC_IN;
    mff_cycles(0xC6EF, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06F1, 0xC26F, -1)) return;          /* rendering_on */
    SYNC_IN;
    mff_cycles(0xC6F2, 14);
    a = 0;
    SET_NZ(a);
    RAM(PLAYER_ID) = a;
    RAM(SELECT_PLAYER) = a;
    SYNC_OUT;
    if (!mff_call_at(0x06FA, 0x9100, -1)) return;          /* 04:9100 the select screen */
    SYNC_IN;

    /* the round map */
    mff_cycles(0xC6FB, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06FD, 0xC85C, -1)) return;          /* screen_clear */
    SYNC_IN;
    mff_cycles(0xC6FE, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0700, 0xC334, -1)) return;          /* objects_clear_all */
    SYNC_IN;
    mff_cycles(0xC701, 16);
    x = BANKS_SCREENS;
    RAM(BANK_REQUEST) = x;
    x++;
    SET_NZ(x);
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x070A, func_FF4D, 0xE000)) return;   /* bank_switch */
    SYNC_IN;
    mff_cycles(0xC70B, 8);
    x = SCREEN_ROUND_MAP;
    SET_NZ(x);
    SYNC_OUT;
    if (!mff_call_at(0x070F, 0x8000, -1)) return;          /* 04:8000 screen_draw */
    SYNC_IN;
    mff_cycles(0xC710, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0712, 0xC26F, -1)) return;          /* rendering_on */
    SYNC_IN;
    mff_cycles(0xC713, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0715, 0x8D00, -1)) return;          /* 04:8D00 the round map */
    SYNC_IN;
    mff_cycles(0xC716, 8);
    x = ROUND_MAP_FRAMES;
    SET_NZ(x);
    SYNC_OUT;
    mff_call_fn(0x071A, func_FF2A, 0xE000);                /* task_wait_frames */
    SYNC_IN;

    /* a new game */
    mff_cycles(0xC71B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x071D, 0xC85C, -1)) return;          /* screen_clear */
    SYNC_IN;
    mff_cycles(0xC71E, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0720, 0xC26F, -1)) return;          /* rendering_on */
    SYNC_IN;
    mff_cycles(0xC721, 56);
    g_cpu.I = 0;                                            /* CLI */
    a = 1;
    RAM(IRQ_ENABLE) = a;
    a = 0;
    SET_NZ(a);
    RAM(SCROLL_NT) = a;
    RAM(SCROLL_Y_HI) = a;
    RAM(SCROLL_X) = a;
    RAM(SCROLL_Y) = a;
    RAM(SCROLL_X_HI) = a;
    RAM(SCROLL_X_PREV) = a;
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y_HI) = a;
    RAM(SPAWN_LIST_NEXT) = a;
    RAM(SPAWN_LIST_PREV) = a;
    RAM(UNK_5A) = a;
    RAM(UNK_33) = a;
    RAM(RENDER_OFF_DEPTH) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0746, 0xC334, -1)) return;          /* objects_clear_all */
    SYNC_IN;
    mff_cycles(0xC747, 6);
    a = 1;
    hw_write(MMC3_MIRRORING, a);                            /* horizontal */
    mff_cycles(0xC74C, 23);
    a = 1;
    RAM(PLAYER_DIR) = a;
    RAM(SCROLL_MODE) = a;
    RAM(CAMERA_DIR) = a;
    y = 0;
    RAM(AREA_INDEX) = y;
    RAM(ROUND_TABLES) = y;
    a = prg_read_ix(ROUND_MUSIC_TABLE, y);
    SET_NZ(a);
    mff_cycles(0xC75D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x075F, func_F34A, 0xE000)) return;   /* sound_queue_push */
    SYNC_IN;
    mff_cycles(0xC760, 10);
    a = 0xFF;
    RAM(BG_SCREEN) = a;
    a = 0x1F;
    RAM(BG_COLUMN) = a;

    /* round_start_wait */
    for (;;) {
        mff_cycles(0xC768, 11);
        a = 1;
        SET_NZ(a);
        RAM(TMP16) = a;                                     /* the column writer goes right */
        SYNC_OUT;
        if (!mff_call_fn(0x076E, func_E521, 0xE000)) return;   /* the column writer */
        SYNC_IN;
        mff_cycles(0xC76F, 6);
        SYNC_OUT;
        mff_call_fn(0x0771, func_FF31, 0xE000);            /* task_wait_frame */
        SYNC_IN;
        mff_cycles(0xC772, 5);
        a = RAM(BG_SCREEN);
        if (a != 0) break;
        mff_cycles(0xC768, 1);
    }

    /* the first area */
    mff_cycles(0xC776, 11);
    a = BANK_HUD_TEXT;
    SET_NZ(a);
    RAM(BANK_REQUEST) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x077C, func_FF4D, 0xE000)) return;   /* bank_switch */
    SYNC_IN;
    mff_cycles(0xC77D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x077F, 0x9C00, -1)) return;          /* 0A:9C00 the HUD's text */
    SYNC_IN;
    mff_cycles(0xC780, 7);
    y = RAM(ROUND_TABLES);
    a = prg_read_ix(ROUND_BANK_TABLE, y);
    SET_NZ(a);
    mff_cycles(0xC785, 9);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0789, func_FF4D, 0xE000)) return;   /* bank_switch */
    SYNC_IN;
    mff_cycles(0xC78A, 4);
    a = prg_read((uint16_t)(AREA_RECORD(0) + AREA_REC_HEADER));
    mff_cycles(0xC78D, 31);
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
    SYNC_OUT;
    if (!mff_call_at(0x079F, 0xC3DD, -1)) return;          /* area_load_graphics */
    SYNC_IN;
    mff_cycles(0xC7A0, 11);
    a = 0;
    SET_NZ(a);
    RAM(PALETTE_REQUEST) = a;
    SYNC_OUT;
    if (!mff_call_at(0x07A6, 0xC362, -1)) return;          /* palette_fade_in */
    SYNC_IN;
    mff_cycles(0xC7A7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07A9, 0xC334, -1)) return;          /* objects_clear_all */
    SYNC_IN;
    mff_cycles(0xC7AA, 49);
    x = 0;                                                  /* slot 0, the player */
    a = 0xD0;
    RAM(OBJ_FLAGS) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0x30;
    RAM(OBJ_X) = a;
    a = 0x90;
    RAM(OBJ_Y) = a;
    a = 0;
    SET_NZ(a);
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y_HI) = a;
    RAM(OBJ_ID) = a;
    RAM(AREA_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x07CF, func_F26E, 0xE000)) return;   /* slot X's fall speed */
    SYNC_IN;
    mff_cycles(0xC7D0, 7);
    y = RAM(PLAYER_ID);
    a = prg_read_ix(START_HEALTH_TABLE, y);
    SET_NZ(a);
    mff_cycles(0xC7D5, 9);
    RAM(HEALTH) = a;
    SYNC_OUT;
    if (!mff_call_at(0x07D9, 0xC8B4, -1)) return;          /* player_load_graphics */
    SYNC_IN;
    mff_cycles(0xC7DA, 16);
    x = BANKS_STAGE;
    RAM(BANK_REQUEST) = x;
    x++;
    SET_NZ(x);
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x07E3, func_FF4D, 0xE000)) return;   /* bank_switch */
    SYNC_IN;
    mff_cycles(0xC7E4, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07E6, 0x8000, -1)) return;          /* 08:8000 the area's stage list */
    SYNC_IN;
    mff_cycles(0xC7E7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07E9, 0x8038, -1)) return;          /* 08:8038 its first entry ahead */
    SYNC_IN;
    mff_cycles(0xC7EA, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07EC, 0x8080, -1)) return;          /* 08:8080 stage_spawns */
    SYNC_IN;

    /* the new game's counters */
    mff_cycles(0xC7ED, 112);
    a = START_LIVES;
    RAM(LIVES) = a;
    a = START_CREDITS;
    RAM(CREDITS) = a;
    a = 0;
    SET_NZ(a);
    RAM(GAME_ACTION) = a;
    RAM(COMBO_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(UNK_A5) = a;
    RAM(ACTION_TIMER) = a;
    RAM(ROUND) = a;
    RAM(SUB_AREA) = a;
    RAM(PLAYER_LEVEL) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(PLAYER_EXP) = a;
    RAM(PLAYER_EXP + 1) = a;
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
    RAM(BONUS_STAGE) = a;
    RAM(GAME_FINISHED) = a;
    RAM(CAMERA_AUTOSCROLL) = a;
    RAM(KEEP_ON_SCREEN) = a;
    RAM(ACTION_BEFORE_FALL) = a;
    RAM(GRAB_HIT_COUNT) = a;
    RAM(SPAWN_GROUPS) = a;
    RAM(SPAWN_GROUPS + 1) = a;
    RAM(SPAWN_GROUPS + 2) = a;
    RAM(CONTINUE_COUNT) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC8ED, 2); call_by_address_tail(0xC8ED, -1); return;   /* JMP gameplay */
}
