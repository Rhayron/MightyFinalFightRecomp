/*
 * flow_continue.c - the continue screen (04:9300)
 *
 * When the last life is lost with credits left, player_state_continue
 * (0E:DCAE-DCB6) draws screen 3 and runs continue_screen, which returns
 * when the player has chosen: game_action 7 (ACT_SELECT) to continue, else
 * the game over follows. Slot 0 is the cursor (animation $72) beside the
 * two choices, at y $54 (continue) or $64 (end); DOWN or UP moves it with
 * effect $2F (obj_timer 0 or $FF says where it is), START picks. Slot 1
 * (animation $67) walks left and right at a pixel a frame along the
 * bottom, turning every $78 frames (slot 0's obj_timer2 and obj_dir drive
 * it). CREDIT n is drawn at $237D from credits.
 *
 * The loop is continue_loop ($938A) -> continue_walk ($9398, falling in)
 * -> continue_turn ($93B0, by JMP or falling in) -> scene_frame and JMP
 * back to $938A; the lifted JMPs are tail calls by address, which the
 * runtime turns into a loop.
 */
#include "flow.h"

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

void func_F090(void);   /* obj_face_right */
void func_F099(void);   /* obj_face_left */
void func_F183(void);   /* obj_move_right */
void func_F1A1(void);   /* obj_move_left */
void func_F288(void);   /* obj_set_anim: slot X, animation A */
void func_F406(void);   /* scene_frame: draws the objects, waits a frame */
void func_938A_b2(void);
void func_9398_b2(void);
void func_93B0_b2(void);

#define CONTINUE_PAL_BG     0x9575  /* 16 colors each */
#define CONTINUE_PAL_SPR    0x9585
#define CREDIT_DIGITS       0x9449  /* the digit tile by credits: 1, 2, ... */

#define MUSIC_CONTINUE      0x0A
#define SFX_CURSOR          0x2F
#define SFX_CONTINUE        0x30
#define ACT_SELECT          0x07    /* game_action: continued, to the select screen */

#define CURSOR_ANIM         0x72
#define CURSOR_CHR          0x3C    /* chr_banks+2 */
#define CURSOR_X            0x68
#define CURSOR_YES_Y        0x54
#define CURSOR_NO_Y         0x64
#define WALKER_ANIM         0x67
#define WALKER_X            0x84
#define WALKER_Y            0xB0
#define WALK_TURN           0x79    /* reloads slot 0's obj_timer2: $78 frames per way */

/*
 * continue_screen (04:9300, labels.toml)
 * continue_count + 1; the screen's 32 colors; CHR R0/R1 $78/$7A, R4 $23;
 * music $0A; CREDIT n (continue_draw_credits); slot 0 the cursor at
 * ($68, $54) with player_set_anim($72) and CHR R2 $3C, obj_timer 0,
 * obj_timer2 $3C, obj_dir right; slot 1 the walker at ($84, $B0),
 * animation $67, obj_vx $0100; both live, flags $90. Then the loop.
 * Caller: player_state_continue (0E:DCB6).
 */
void func_9300_b2(void) {
    REGS_IN;

    mff_cycles(0x9300, 8);
    RAM(CONTINUE_COUNT)++;
    y = 0x0F;
    for (;;) {
        mff_cycles(0x9305, 4);
        a = prg_read_ix(CONTINUE_PAL_BG, y);
        mff_cycles(0x9308, 14);
        RAM(PALETTE_BUF + y) = a;
        RAM(PALETTE_BASE + y) = a;
        a = prg_read_ix(CONTINUE_PAL_SPR, y);
        mff_cycles(0x9311, 14);
        RAM(PALETTE_BUF + 16 + y) = a;
        RAM(PALETTE_BASE + 16 + y) = a;
        y--;
        if (y & 0x80)
            break;
        mff_cycles(0x9318, 1);
    }
    mff_cycles(0x931A, 26);
    RAM(PALETTE_REQUEST) = y;
    RAM(CHR_BANKS) = 0x78;
    RAM(CHR_BANKS + 1) = 0x7A;
    RAM(CHR_BANKS + 4) = 0x23;
    a = MUSIC_CONTINUE;
    SET_NZ(a);
    CALL_0F(0x932A, func_F34A);                 /* sound_queue_push */
    mff_cycles(0x932D, 6);
    CALL_04(0x932D, 0x942A);                    /* continue_draw_credits */
    mff_cycles(0x9330, 8);
    a = CURSOR_ANIM;
    SET_NZ(a);
    CALL_0F(0x9332, func_F294);                 /* player_set_anim */
    mff_cycles(0x9335, 8);
    a = CURSOR_CHR;
    SET_NZ(a);
    CALL_0F(0x9337, func_F440);                 /* chr_banks+2 = A */
    mff_cycles(0x933A, 88);
    RAM(OBJ_STATE) = 0x80;
    RAM(OBJ_STATE + 1) = 0x80;
    RAM(OBJ_FLAGS) = 0x90;
    RAM(OBJ_FLAGS + 1) = 0x90;
    RAM(OBJ_X_HI) = 0x00;
    RAM(OBJ_Y_HI) = 0x00;
    RAM(OBJ_X_HI + 1) = 0x00;
    RAM(OBJ_Y_HI + 1) = 0x00;
    RAM(OBJ_TIMER) = 0x00;
    RAM(OBJ_TIMER2) = 0x3C;
    RAM(OBJ_DIR) = PAD_RIGHT;                 /* obj_dir in pad bits */
    RAM(OBJ_VX_FRAC + 1) = 0x00;
    RAM(OBJ_VX + 1) = 0x01;
    RAM(OBJ_X) = CURSOR_X;
    RAM(OBJ_Y) = CURSOR_YES_Y;
    x = 0x01;
    a = WALKER_ANIM;
    SET_NZ(a);
    CALL_0F(0x937D, func_F288);                 /* obj_set_anim (slot 1) */
    mff_cycles(0x9380, 12);
    RAM(OBJ_X + 1) = WALKER_X;
    a = WALKER_Y;
    RAM(OBJ_Y + 1) = a;
    SYNC_OUT;
    func_938A_b2();                             /* falls into the loop */
}

/* $93CE (DOWN) and $93F0 (UP) both move the cursor to the other choice:
 * effect $2F, obj_timer 0 <-> $FF, y $54 <-> $64; then the walk ($9398) */
static void continue_cursor_down(void) {
    REGS_IN;

    mff_cycles(0x93CE, 8);
    a = SFX_CURSOR;
    SET_NZ(a);
    CALL_0F(0x93D0, func_F34C);
    mff_cycles(0x93D3, 6);
    a = RAM(OBJ_TIMER);
    if (a == 0) {
        mff_cycles(0x93D8, 14);
        RAM(OBJ_TIMER) = 0xFF;
        a = CURSOR_NO_Y;
    } else {
        mff_cycles(0x93D6, 1);
        mff_cycles(0x93E4, 14);
        RAM(OBJ_TIMER) = 0x00;
        a = CURSOR_YES_Y;
    }
    RAM(OBJ_Y) = a;
    mff_cycles(0x9398, 1);                      /* BNE $9398, always taken */
    SYNC_OUT;
    func_9398_b2();
}

static void continue_cursor_up(void) {
    REGS_IN;

    mff_cycles(0x93F0, 8);
    a = SFX_CURSOR;
    SET_NZ(a);
    CALL_0F(0x93F2, func_F34C);
    mff_cycles(0x93F5, 6);
    a = RAM(OBJ_TIMER);
    if (a == 0) {
        mff_cycles(0x93FA, 14);
        RAM(OBJ_TIMER) = 0xFF;
        a = CURSOR_NO_Y;
        SET_NZ(a);
        RAM(OBJ_Y) = a;
        mff_cycles(0x9404, 1);                  /* BNE $9410, always taken */
    } else {
        mff_cycles(0x93F8, 2);                  /* BNE across a page */
        mff_cycles(0x9406, 12);
        RAM(OBJ_TIMER) = 0x00;
        a = CURSOR_YES_Y;
        SET_NZ(a);
        RAM(OBJ_Y) = a;
    }
    JMP_04(0x9410, 0x9398);
}

/*
 * continue_loop, $938A (bank 04; name proposed): a frame of the continue
 * screen. joy_pressed START picks (JMP $9413), DOWN or UP alone moves the
 * cursor; then the walk.
 * Callers: continue_screen falls into it; continue_turn's JMP at $93C8.
 */
void func_938A_b2(void) {
    REGS_IN;

    mff_cycles(0x938A, 7);
    a = RAM(JOY_PRESSED);
    c = a >= PAD_START;
    SET_NZ(a - PAD_START);
    if (z) {
        mff_cycles(0x938E, 1);
        JMP_04(0x93CB, 0x9413);
    }
    mff_cycles(0x9390, 4);
    c = a >= PAD_DOWN;
    if (a == PAD_DOWN) {
        mff_cycles(0x9392, 1);
        SYNC_OUT;
        continue_cursor_down();
        return;
    }
    mff_cycles(0x9394, 4);
    c = a >= PAD_UP;
    if (a == PAD_UP) {
        mff_cycles(0x9396, 1);
        SYNC_OUT;
        continue_cursor_up();
        return;
    }
    SYNC_OUT;
    func_9398_b2();                             /* falls into the walk */
}

/*
 * continue_walk, $9398 (bank 04; name proposed): slot 1 steps the way
 * slot 0's obj_dir says (bit 0: right), facing it; then continue_turn.
 * Callers: continue_loop falls into it; the cursor moves (BNE $9398, JMP
 * at $9410).
 */
void func_9398_b2(void) {
    REGS_IN;

    mff_cycles(0x9398, 10);
    x = 0x01;
    a = RAM(OBJ_DIR) & 0x01;
    SET_NZ(a);
    if (z) {
        mff_cycles(0x93A1, 6);
        CALL_0F(0x93A1, func_F090);
        mff_cycles(0x93A4, 6);
        CALL_0F(0x93A4, func_F183);
        JMP_04(0x93A7, 0x93B0);
    }
    mff_cycles(0x939F, 1);
    mff_cycles(0x93AA, 6);
    CALL_0F(0x93AA, func_F099);
    mff_cycles(0x93AD, 6);
    CALL_0F(0x93AD, func_F1A1);
    SYNC_OUT;
    func_93B0_b2();                             /* falls into it */
}

/*
 * continue_turn, $93B0 (bank 04; name proposed): when slot 0's obj_timer2
 * runs out it is $79 again and obj_dir turns (EOR 3); obj_timer2 - 1;
 * scene_frame; JMP $938A.
 * Callers: continue_walk (JMP at $93A7, and falling in).
 */
void func_93B0_b2(void) {
    REGS_IN;

    mff_cycles(0x93B0, 6);
    a = RAM(OBJ_TIMER2);
    if (a == 0) {
        mff_cycles(0x93B5, 16);
        RAM(OBJ_TIMER2) = WALK_TURN;
        a = RAM(OBJ_DIR) ^ 0x03;
        RAM(OBJ_DIR) = a;
    } else {
        mff_cycles(0x93B3, 1);
    }
    mff_cycles(0x93C2, 12);
    RAM(OBJ_TIMER2)--;
    SET_NZ(RAM(OBJ_TIMER2));
    CALL_0F(0x93C5, func_F406);                 /* scene_frame */
    JMP_04(0x93C8, 0x938A);
}

/*
 * continue_pick, $9413 (bank 04; name proposed): START. On the end choice
 * (y $64) it returns as it is (game over). On continue: credits - 1, CREDIT
 * n redrawn, effect $30, game_action 7 (ACT_SELECT).
 * Caller: JMP at $93CB.
 */
void func_9413_b2(void) {
    REGS_IN;

    mff_cycles(0x9413, 8);
    a = RAM(OBJ_Y);
    c = a >= CURSOR_NO_Y;
    SET_NZ(a - CURSOR_NO_Y);
    if (z) {
        mff_cycles(0x941A, 6);
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0x9418, 1);
    mff_cycles(0x941B, 11);
    RAM(CREDITS)--;
    SET_NZ(RAM(CREDITS));
    CALL_04(0x941D, 0x942A);
    mff_cycles(0x9420, 8);
    a = SFX_CONTINUE;
    SET_NZ(a);
    CALL_0F(0x9422, func_F34C);
    mff_cycles(0x9425, 11);
    a = ACT_SELECT;
    SET_NZ(a);
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts();
}

/*
 * continue_draw_credits (04:942A, labels.toml): the credits digit
 * (CREDIT_DIGITS[credits]) at $237D through the NMI's string list
 * (vram_buffer as [address high, low, count - 1, bytes], ended by $FF;
 * vram_list_request $FF).
 * Callers: continue_screen ($932D), continue_pick ($941D).
 */
void func_942A_b2(void) {
    REGS_IN;

    mff_cycles(0x942A, 25);
    RAM(VRAM_BUFFER) = 0x23;
    RAM(VRAM_BUFFER + 1) = 0x7D;
    RAM(VRAM_BUFFER + 2) = 0x00;
    y = RAM(CREDITS);
    a = prg_read_ix(CREDIT_DIGITS, y);
    mff_cycles(0x943E, 19);
    RAM(VRAM_BUFFER + 3) = a;
    a = 0xFF;
    SET_NZ(a);
    RAM(VRAM_BUFFER + 4) = a;
    RAM(VRAM_LIST_REQUEST) = a;
    SYNC_OUT;
    mff_rts();
}
