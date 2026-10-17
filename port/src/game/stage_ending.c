/*
 * stage_ending.c - the full ending, bank $09 (16 KiB bank 4 at $A000)
 *
 * After the last boss, a game finished without continues (continue_count
 * $0150 = 0) shows the full ending (0E:DEF2-DF4A): the story's last
 * illustrated page, then five pages of credits typed out letter by letter
 * (ending_text, intro_page 0-4), then the character's final pose
 * (ending_pose). Both run inside task 0 and wait for frames through
 * 0F:F406 (draw the objects, wait a frame) and task_wait_frame(s).
 *
 * The text: a page is a string at K_TEXT_PAGE_LO/HI[intro_page] of tile
 * numbers, $FE ending a line and $FF the page. Up to three lines show at a
 * time (PPU $21A7, $21E7, $2227); a full screen holds 180 frames, is
 * cleared and goes on; the end of the page holds 180 frames. Each letter
 * goes out as a one-byte write list at $0780, with a click (sound $2B) on
 * every fourth frame, and moves the player one step left (0F:F1A1).
 * Scratch: $0C/$0D the page, $0E the index in it, $10 the line, $11 the
 * column, $12 cleared and not read.
 *
 * The pose: the player (slot 0) at $58,$A8 plays a short scripted sequence
 * per character, each step one frame: Cody jumps, Guy spins and leaps,
 * Haggar runs and stops. A script ends holding its last frame for ever (the
 * credits screen) or, for Cody, returns through 0F:F406.
 */
#include "stage.h"

void func_F03F(void);   /* the player's ground check */
void func_F183(void);   /* obj_move_right */
void func_F1A1(void);   /* obj_move_left */
void func_F1FD(void);   /* obj_fall */
void func_F294(void);   /* player_set_anim */
void func_F34C(void);   /* sound_queue_push_sfx */
void func_F406(void);   /* draw the objects, wait a frame */
void func_F430(void);   /* sound $17 */
void func_F440(void);   /* the player's sprite bank: CHR R2 = A */
void func_FF2A(void);   /* task_wait_frames */
void func_FF31(void);   /* task_wait_frame */

#define RP(addr) ((uint16_t)((addr) & 0x1FFF))

/* the text */
#define K_TEXT_LINE_HI  0xA782      /* PPU address of lines 0-2: $21A7, $21E7, $2227 */
#define K_TEXT_LINE_LO  0xA785
#define K_TEXT_PAGE_LO  0xA7D6      /* by intro_page */
#define K_TEXT_PAGE_HI  0xA7DB
#define TEXT_END        0xFF
#define TEXT_NEWLINE    0xFE
#define TEXT_LINES      3
#define TEXT_HOLD       0xB4        /* frames a screen of text stays */
#define SOUND_TYPE      0x2B

/* the pose: the character's script by player_id */
#define K_POSE_LO       0xAB2E
#define K_POSE_HI       0xAB31
#define POSE_X          0x58
#define POSE_Y          0xA8        /* the floor */

/* ---- the pose steps (each operates on g_cpu, the caller syncs) ---- */

/* pc: JSR $F406, draw the objects and wait a frame. 0: the call left
 * through the scheduler (return at once). */
static int pose_frame(uint16_t pc) {
    mff_cycles(pc, 6);
    return mff_call_fn(RP(pc + 2), func_F406, 0xE000);
}

/* pc: LDA #id / JSR $F34C, a sound effect */
static int pose_sound(uint16_t pc, uint8_t id) {
    mff_cycles(pc, 8);
    g_cpu.A = id;
    mff_nz(id);
    return mff_call_fn(RP(pc + 4), func_F34C, 0xE000);
}

/* pc: LDA #anim / JSR player_set_anim / LDA #chr / JSR $F440: the
 * player's animation and the CHR bank it draws from */
static int pose_set(uint16_t pc, uint8_t anim, uint8_t chr) {
    mff_cycles(pc, 8);
    g_cpu.A = anim;
    mff_nz(anim);
    if (!mff_call_fn(RP(pc + 4), func_F294, 0xE000)) return 0;
    mff_cycles((uint16_t)(pc + 5), 8);
    g_cpu.A = chr;
    mff_nz(chr);
    return mff_call_fn(RP(pc + 9), func_F440, 0xE000);
}

/* pc: LDA #anim / CMP obj_anim / BEQ over / JSR player_set_anim / LDA #chr
 * / JSR $F440: pose_set unless the animation is on already. Returns 0
 * (left through the scheduler), 1 (switched) or 2 (already on). */
static int pose_switch(uint16_t pc, uint8_t anim, uint8_t chr) {
    uint8_t cur = RAM(OBJ_ANIM);

    mff_cycles(pc, 8);
    g_cpu.A = anim;
    g_cpu.C = anim >= cur;
    mff_nz((uint8_t)(anim - cur));
    if (anim == cur) {
        mff_cycles((uint16_t)(pc + 5), 1);
        return 2;
    }
    mff_cycles((uint16_t)(pc + 7), 6);
    if (!mff_call_fn(RP(pc + 9), func_F294, 0xE000)) return 0;
    mff_cycles((uint16_t)(pc + 10), 8);
    g_cpu.A = chr;
    mff_nz(chr);
    return mff_call_fn(RP(pc + 14), func_F440, 0xE000);
}

/* ---- the credits text ---- */

/*
 * ending_text, $A700 (bank 4; proposed name): page intro_page of the
 * credits: clear the lines, point $0C at the page, type it.
 * Called by: 0E:DF33-DF47 (JSR $A700 once per page, intro_page 0-4).
 */
void func_A700_b4(void) {
    REGS_IN;

    mff_cycles(0xA700, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0702, 0xA78E, 4)) return;
    SYNC_IN;
    mff_cycles(0xA703, 7);
    y = RAM(INTRO_PAGE);
    a = prg_read((uint16_t)(K_TEXT_PAGE_LO + y));
    mff_cycles(0xA708, 7);
    RAM(TMP12) = a;
    a = prg_read((uint16_t)(K_TEXT_PAGE_HI + y));
    mff_cycles(0xA70D, 8);
    RAM(TMP13) = a;
    a = 0x00;
    RAM(TMP14) = a;
    SYNC_OUT;
    func_A713_b4();
}

/*
 * ending_text_screen, $A713 (bank 4; proposed name): a new screen of text
 * from line 0, then ending_text_type.
 * Called by: ending_text (falling in at $A713) and ending_text_type (JMP at
 * $A77B after a full screen).
 */
void func_A713_b4(void) {
    REGS_IN;

    mff_cycles(0xA713, 8);
    a = 0x00;
    RAM(TMP16) = a;
    RAM(TMP18) = a;
    mff_cycles(0xA719, 5);                      /* column 0 */
    RAM(TMP17) = a;
    SYNC_OUT;
    func_A71D_b4();
}

/*
 * ending_text_type, $A71D (bank 4; proposed name): the next character of
 * the page at line $10, column $11: a letter goes out (and the frame
 * passes); $FE moves to the next line, and after the third holds the
 * screen and starts another (ending_text_screen); $FF holds the page and
 * returns (ending_hold).
 * Called by: ending_text_screen (falling in at $A71D) and itself (JMP at
 * $A76A after each letter).
 */
void func_A71D_b4(void) {
    REGS_IN;

    for (;;) {
        mff_cycles(0xA71D, 7);
        x = RAM(TMP16);
        a = prg_read((uint16_t)(K_TEXT_LINE_HI + x));
        mff_cycles(0xA722, 8);
        RAM(VRAM_BUFFER) = a;
        a = prg_read((uint16_t)(K_TEXT_LINE_LO + x));
        mff_cycles(0xA728, 28);
        c = 0;
        ADC_A(RAM(TMP17));
        RAM(VRAM_BUFFER + 1) = a;
        RAM(VRAM_BUFFER + 2) = 0x00;            /* one byte */
        y = RAM(TMP14);
        RAM(TMP14)++;
        a = mem_read((uint16_t)(RAM16(TMP12) + y));
        mff_cycles(0xA739, 4);
        SET_CMP(a, TEXT_END);
        if (z) {
            mff_cycles(0xA73B, 1 + 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xA7D0, 2);
            call_by_address_tail(0xA7D0, 4);
            return;
        }
        mff_cycles(0xA73D, 4);
        c = a >= TEXT_NEWLINE;
        if (a != TEXT_NEWLINE) {
            /* a letter, while the player steps left and the frame passes */
            mff_cycles(0xA741, 31);
            RAM(VRAM_BUFFER + 3) = a;
            a = 0xFF;
            RAM(VRAM_BUFFER + 4) = a;           /* the list's end */
            RAM(VRAM_LIST_REQUEST) = a;
            mff_push(RAM(TMP16));
            a = RAM(TMP17);
            SET_NZ(a);
            mff_push(a);
            SYNC_OUT;
            if (!mff_call_at(0x0753, 0xA788, 4)) return;
            SYNC_IN;
            mff_cycles(0xA754, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0756, func_F406, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xA757, 26);
            a = mff_pull();
            RAM(TMP17) = a;
            a = mff_pull();
            RAM(TMP16) = a;
            RAM(TMP17)++;
            a = RAM(FRAME_COUNTER) & 0x03;
            SET_NZ(a);
            if (a == 0) {
                mff_cycles(0xA765, 8);
                a = SOUND_TYPE;
                SET_NZ(a);
                SYNC_OUT;
                if (!mff_call_fn(0x0769, func_F34C, 0xE000)) return;
                SYNC_IN;
            } else {
                mff_cycles(0xA763, 1);
            }
            mff_cycles(0xA76A, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xA71D, 2);
            call_by_address_tail(0xA71D, 4);
            return;
        }

        /* the next line */
        mff_cycles(0xA73F, 1 + 12);
        RAM(TMP16)++;
        a = RAM(TMP16);
        SET_CMP(a, TEXT_LINES);
        if (z) break;
        mff_cycles(0xA773, 1 + 5);
        a = 0x00;
        RAM(TMP17) = a;
    }

    /* a full screen: hold it, clear it, the next */
    mff_cycles(0xA775, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0777, 0xA7D0, 4)) return;
    SYNC_IN;
    mff_cycles(0xA778, 6);
    SYNC_OUT;
    if (!mff_call_at(0x077A, 0xA78E, 4)) return;
    SYNC_IN;
    mff_cycles(0xA77B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA713, 2);
    call_by_address_tail(0xA713, 4);
}

/*
 * ending_step_left, $A788 (bank 4; proposed name): the player (slot 0)
 * moves left by its speed (obj_move_left).
 * Called by: ending_text_type ($A751).
 */
void func_A788_b4(void) {
    REGS_IN;

    mff_cycles(0xA788, 8);
    x = 0x00;
    SET_NZ(x);
    SYNC_OUT;
    if (!mff_call_fn(0x078C, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA78D, 6);
    SYNC_OUT;
    mff_rts();
}

/*
 * ending_text_clear, $A78E (bank 4; proposed name): queues 19 blank tiles on
 * each text line (write lists at $0784, $079A, $07B0, after the last
 * letter's entry at $0780) and waits a frame (task_wait_frame, JMP).
 * Called by: ending_text ($A700) and ending_text_type ($A778).
 */
void func_A78E_b4(void) {
    REGS_IN;

    mff_cycles(0xA78E, 52);
    RAM(VRAM_BUFFER + 4) = 0x22;                /* $2227 */
    RAM(VRAM_BUFFER + 48) = 0x21;               /* $21A7 */
    RAM(VRAM_BUFFER + 26) = 0x21;               /* $21E7 */
    RAM(VRAM_BUFFER + 5) = 0x27;
    RAM(VRAM_BUFFER + 27) = 0xE7;
    RAM(VRAM_BUFFER + 49) = 0xA7;
    x = 0x12;                                   /* 19 bytes */
    RAM(VRAM_BUFFER + 6) = x;
    RAM(VRAM_BUFFER + 28) = x;
    RAM(VRAM_BUFFER + 50) = x;
    y = 0x00;
    a = 0x00;
    for (;;) {
        mff_cycles(0xA7B9, 21);
        RAM(VRAM_BUFFER + 7 + y) = a;
        RAM(VRAM_BUFFER + 29 + y) = a;
        RAM(VRAM_BUFFER + 51 + y) = a;
        y++;
        x--;
        if (x & 0x80) break;
        mff_cycles(0xA7C4, 1);
    }
    mff_cycles(0xA7C6, 12);
    a = 0xFF;
    SET_NZ(a);
    RAM(VRAM_BUFFER + 70) = a;
    RAM(VRAM_LIST_REQUEST) = a;
    SYNC_OUT;
    stage_jmp(0xFF31, func_FF31);
}

/*
 * ending_hold, $A7D0 (bank 4; proposed name): wait 180 frames.
 * Called by: ending_text_type (JSR at $A775, JMP at $A77E).
 */
void func_A7D0_b4(void) {
    REGS_IN;

    mff_cycles(0xA7D0, 8);
    x = TEXT_HOLD;
    SET_NZ(x);
    SYNC_OUT;
    mff_call_fn(0x07D4, func_FF2A, 0xE000);     /* a yield: no balance check */
    SYNC_IN;
    mff_cycles(0xA7D5, 6);
    SYNC_OUT;
    mff_rts();
}

/* ---- the final pose ---- */

/*
 * ending_pose, $AB00 (bank 4; proposed name): the player in slot 0 on the
 * floor at $58,$A8 (on screen, facing right, world coordinates), then the
 * character's script (JMP through K_POSE_LO/HI[player_id]).
 * Called by: 0E:DF4A.
 */
void func_AB00_b4(void) {
    REGS_IN;

    mff_cycles(0xAB00, 45);
    RAM(OBJ_STATE) = 0x80;
    RAM(OBJ_FLAGS) = OBJF_ON_SCREEN | OBJF_FACE_RIGHT | OBJF_WORLD;
    a = 0x00;
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y_HI) = a;
    RAM(OBJ_ID) = a;
    RAM(OBJ_X) = POSE_X;
    a = POSE_Y;
    RAM(OBJ_Y) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(K_POSE_LO + y));
    mff_cycles(0xAB24, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(K_POSE_HI + y));
    SET_NZ(a);
    mff_cycles(0xAB29, 8);
    RAM(TMP1) = a;
    SYNC_OUT;
    {
        uint16_t jt = nes_read16zp(TMP0);
        nes_cpu_instruction_boundary(jt, 2);
        call_by_address_tail(jt, -1);
    }
}

/*
 * cody_pose, $AB34 (bank 4; proposed name): one frame of Cody's pose. Phase
 * 0 (obj_state & $0F = 0): crouch (animation $73, with a shout, sound $16,
 * at its start) and on frame 6 jump (animation $13, obj_vy $04.38). Phase
 * 1: rise (frame 5 held) and fall (obj_fall), moving right; falling, the
 * animation $3E (phase 2); back on the floor ($A9 or lower) animation 1
 * and the end, through 0F:F406.
 * Called by: ending_pose (the JMP), cody_pose_next (JMP at $AB7A).
 */
void func_AB34_b4(void) {
    REGS_IN;
    int fall, r;

    mff_cycles(0xAB34, 8);
    a = RAM(OBJ_STATE) & 0x0F;
    if (a == 0) {
        SYNC_OUT;
        if (!pose_switch(0xAB3B, 0x73, 0x3F)) return;
        SYNC_IN;
        mff_cycles(0xAB4A, 6);
        if (RAM(OBJ_ANIM_FRAME) == 0) {
            mff_cycles(0xAB4F, 6);
            if (RAM(OBJ_ANIM_TIMER) == 0) {
                SYNC_OUT;
                if (!pose_sound(0xAB54, 0x16)) return;
                SYNC_IN;
            } else {
                mff_cycles(0xAB52, 1);
            }
        } else {
            mff_cycles(0xAB4D, 1);
        }
        mff_cycles(0xAB59, 8);
        a = RAM(OBJ_ANIM_FRAME);
        SET_CMP(a, 6);
        if (z) {
            SYNC_OUT;
            if (!pose_set(0xAB60, 0x13, 0x03)) return;
            SYNC_IN;
            mff_cycles(0xAB6A, 18);
            RAM(OBJ_VY_FRAC) = 0x38;
            a = 0x04;
            RAM(OBJ_VY) = a;
            t = (uint8_t)(RAM(OBJ_STATE) + 1);
            RAM(OBJ_STATE) = t;
            SET_NZ(t);
        } else {
            mff_cycles(0xAB5E, 1);
        }
        SYNC_OUT;
        func_AB77_b4();
        return;
    }

    /* in the air */
    mff_cycles(0xAB39, 1 + 8);
    a = RAM(OBJ_STATE) & 0x02;
    if (a != 0) {
        mff_cycles(0xAB82, 1);
        fall = 1;
    } else {
        mff_cycles(0xAB84, 8);
        a = RAM(OBJ_ANIM_FRAME);
        SET_CMP(a, 5);
        if (!z) {
            mff_cycles(0xAB89, 1);
            fall = 0;
        } else {
            mff_cycles(0xAB8B, 12);
            RAM(OBJ_ANIM_FRAME) = 0x05;         /* hold frame 5 */
            a = 0x00;
            RAM(OBJ_ANIM_TIMER) = a;
            fall = 1;
        }
    }
    if (fall) {
        mff_cycles(0xAB95, 8);
        x = 0x00;
        SET_NZ(x);
        SYNC_OUT;
        if (!mff_call_fn(0x0B99, func_F1FD, 0xE000)) return;
        SYNC_IN;
    }
    mff_cycles(0xAB9A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B9C, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAB9D, 6);
    a = RAM(OBJ_VY);
    if (a & 0x80) {
        SYNC_OUT;
        r = pose_switch(0xABA2, 0x3E, 0x01);
        if (!r) return;
        SYNC_IN;
        if (r == 1) {
            mff_cycles(0xABB1, 6);
            RAM(OBJ_STATE)++;
        }
    } else {
        mff_cycles(0xABA0, 1);
    }
    mff_cycles(0xABB4, 8);
    a = RAM(OBJ_Y);
    SET_CMP(a, POSE_Y + 1);
    if (c) {
        /* landed: the end */
        mff_cycles(0xABBB, 6);
        RAM(OBJ_Y) = POSE_Y;
        SYNC_OUT;
        if (!pose_set(0xABC0, 0x01, 0x00)) return;
        mff_cycles(0xABCA, 3);
        stage_jmp(0xF406, func_F406);
        return;
    }
    mff_cycles(0xABB9, 1 + 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAB77, 2);
    call_by_address_tail(0xAB77, 4);
}

/*
 * cody_pose_next, $AB77 (bank 4; proposed name): wait a frame, then
 * cody_pose again.
 * Called by: cody_pose (falling in at $AB77, JMP at $ABCD).
 */
void func_AB77_b4(void) {
    if (!pose_frame(0xAB77)) return;
    mff_cycles(0xAB7A, 3);
    nes_cpu_instruction_boundary(0xAB34, 2);
    call_by_address_tail(0xAB34, 4);
}

/*
 * $ABEE-$AC1F, shared by guy_pose and guy_pose_spin: the spin's second
 * sound (frame $0B, sound $34); until frame $13 wait a frame and spin on
 * (guy_pose_spin), then the leap (animation $14, obj_vy $01.33) and
 * guy_pose_leap.
 */
static void guy_pose_spin_rest(void) {
    REGS_IN;
    int r;

    mff_cycles(0xABEE, 8);
    a = RAM(OBJ_ANIM_FRAME);
    c = a >= 0x0B;
    if (a == 0x0B) {
        SYNC_OUT;
        if (!pose_sound(0xABF5, 0x34)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xABF3, 1);
    }
    mff_cycles(0xABFA, 8);
    a = RAM(OBJ_ANIM_FRAME);
    SET_CMP(a, 0x13);
    if (!z) {
        SYNC_OUT;
        if (!pose_frame(0xAC01)) return;
        mff_cycles(0xAC04, 3);
        nes_cpu_instruction_boundary(0xABDF, 2);
        call_by_address_tail(0xABDF, 4);
        return;
    }
    mff_cycles(0xABFF, 1);
    SYNC_OUT;
    r = pose_switch(0xAC07, 0x14, 0x07);
    if (!r) return;
    SYNC_IN;
    if (r == 1) {
        mff_cycles(0xAC16, 12);
        RAM(OBJ_VY_FRAC) = 0x33;
        a = 0x01;
        RAM(OBJ_VY) = a;
    }
    SYNC_OUT;
    func_AC20_b4();
}

/*
 * guy_pose, $ABD0 (bank 4; proposed name): Guy's script: the spin
 * (animation $24) and its first sound at its start (guy_pose_spin).
 * Called by: ending_pose (the JMP).
 */
void func_ABD0_b4(void) {
    int r;

    r = pose_switch(0xABD0, 0x24, 0x32);
    if (!r) return;
    if (r == 1) {
        func_ABDF_b4();
        return;
    }
    guy_pose_spin_rest();
}

/*
 * guy_pose_spin, $ABDF (bank 4; proposed name): one frame of the spin: a
 * sound ($1B) at its start, then guy_pose_spin_rest.
 * Called by: guy_pose (falling in at $ABDF) and the spin loop (JMP at
 * $AC04).
 */
void func_ABDF_b4(void) {
    REGS_IN;

    mff_cycles(0xABDF, 6);
    if (RAM(OBJ_ANIM_FRAME) == 0) {
        mff_cycles(0xABE4, 6);
        if (RAM(OBJ_ANIM_TIMER) == 0) {
            SYNC_OUT;
            if (!pose_sound(0xABE9, 0x1B)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xABE7, 1);
        }
    } else {
        mff_cycles(0xABE2, 1);
    }
    SYNC_OUT;
    guy_pose_spin_rest();
}

/*
 * guy_pose_leap, $AC20 (bank 4; proposed name): one frame of the leap: a
 * sound ($17, 0F:F430) on frame 4; on frame 6 the somersault (animation
 * $5B, from 32 pixels up, player_ground_y = where he stood), falling
 * (obj_fall) to the floor, where he lands (animation 2) and holds
 * (guy_pose_hold).
 * Called by: guy_pose_spin_rest (falling in at $AC20) and itself (JMP at
 * $AC7E).
 */
void func_AC20_b4(void) {
    REGS_IN;
    int falling;

    mff_cycles(0xAC20, 8);
    a = RAM(OBJ_ANIM);
    c = a >= 0x5B;
    if (a == 0x5B) {
        mff_cycles(0xAC25, 1);
        falling = 1;
    } else {
        mff_cycles(0xAC27, 8);
        a = RAM(OBJ_ANIM_FRAME);
        c = a >= 0x04;
        if (a == 0x04) {
            mff_cycles(0xAC2E, 6);
            a = RAM(OBJ_ANIM_TIMER);
            SET_NZ(a);
            if (z) {
                mff_cycles(0xAC33, 6);
                SYNC_OUT;
                if (!mff_call_fn(0x0C35, func_F430, 0xE000)) return;
                SYNC_IN;
            } else {
                mff_cycles(0xAC31, 1);
            }
        } else {
            mff_cycles(0xAC2C, 1);
        }
        mff_cycles(0xAC36, 8);
        a = RAM(OBJ_ANIM_FRAME);
        SET_CMP(a, 0x06);
        if (!z) {
            mff_cycles(0xAC3B, 1);
            falling = 0;
        } else {
            SYNC_OUT;
            if (!pose_set(0xAC3D, 0x5B, 0x05)) return;
            SYNC_IN;
            mff_cycles(0xAC47, 19);
            RAM(PLAYER_GROUND_Y) = RAM(OBJ_Y);
            a = RAM(OBJ_Y);
            c = 1;
            SBC_A(0x20);
            RAM(OBJ_Y) = a;
            falling = 1;
        }
    }
    if (falling) {
        mff_cycles(0xAC55, 8);
        x = 0x00;
        SET_NZ(x);
        SYNC_OUT;
        if (!mff_call_fn(0x0C59, func_F1FD, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAC5A, 6);
        a = RAM(OBJ_VY);
        SET_NZ(a);
        if (n) {
            mff_cycles(0xAC5F, 8);
            a = RAM(OBJ_Y);
            SET_CMP(a, POSE_Y);
            if (c) {
                /* on the floor: land and hold */
                mff_cycles(0xAC66, 6);
                RAM(OBJ_Y) = POSE_Y;
                SYNC_OUT;
                if (!pose_set(0xAC6B, 0x02, 0x04)) return;
                func_AC75_b4();
                return;
            }
            mff_cycles(0xAC64, 1);
        } else {
            mff_cycles(0xAC5D, 1);
        }
    }
    SYNC_OUT;
    if (!pose_frame(0xAC7B)) return;
    mff_cycles(0xAC7E, 3);
    nes_cpu_instruction_boundary(0xAC20, 2);
    call_by_address_tail(0xAC20, 4);
}

/*
 * guy_pose_hold, $AC75 (bank 4; proposed name): wait a frame, for ever.
 * Called by: guy_pose_leap (falling in at $AC75) and itself (JMP at $AC78).
 */
void func_AC75_b4(void) {
    if (!pose_frame(0xAC75)) return;
    mff_cycles(0xAC78, 3);
    nes_cpu_instruction_boundary(0xAC75, 2);
    call_by_address_tail(0xAC75, 4);
}

/*
 * haggar_pose, $AC81 (bank 4; proposed name): Haggar's script: stand
 * (animation $30) for 8 frames, set to run right (obj_vx 3, flags $90,
 * obj_timer[1] = $1E frames), then haggar_pose_run.
 * Called by: ending_pose (the JMP) and itself (JMP at $ACB4).
 */
void func_AC81_b4(void) {
    REGS_IN;
    int r;

    r = pose_switch(0xAC81, 0x30, 0x0B);
    if (!r) return;
    SYNC_IN;
    if (r == 1) {
        mff_cycles(0xAC90, 30);
        RAM(OBJ_TIMER) = 0x08;
        RAM(OBJ_TIMER + 1) = 0x1E;
        RAM(OBJ_VX_FRAC) = 0x00;
        RAM(OBJ_VX) = 0x03;
        a = OBJF_ON_SCREEN | OBJF_WORLD;
        RAM(OBJ_FLAGS) = a;
    }
    mff_cycles(0xACA9, 6);
    a = RAM(OBJ_TIMER);
    if (a != 0) {
        mff_cycles(0xACAE, 6);
        t = (uint8_t)(RAM(OBJ_TIMER) - 1);
        RAM(OBJ_TIMER) = t;
        SET_NZ(t);
        SYNC_OUT;
        if (!pose_frame(0xACB1)) return;
        mff_cycles(0xACB4, 3);
        nes_cpu_instruction_boundary(0xAC81, 2);
        call_by_address_tail(0xAC81, 4);
        return;
    }
    mff_cycles(0xACAC, 1);
    SYNC_OUT;
    func_ACB7_b4();
}

/*
 * haggar_pose_run, $ACB7 (bank 4; proposed name): one frame of the run
 * (animation $3D, a shout, sound $2C, at its start; 0F:F03F, then
 * obj_move_right) for obj_timer[1] frames, then haggar_pose_stop.
 * Called by: haggar_pose (falling in at $ACB7) and itself (JMP at $ACDE).
 */
void func_ACB7_b4(void) {
    REGS_IN;
    int r;

    r = pose_switch(0xACB7, 0x3D, 0x0A);
    if (!r) return;
    if (r == 1) {
        if (!pose_sound(0xACC6, 0x2C)) return;
    }
    SYNC_IN;
    mff_cycles(0xACCB, 8);
    x = 0x00;
    SET_NZ(x);
    SYNC_OUT;
    if (!mff_call_fn(0x0CCF, func_F03F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xACD0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0CD2, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xACD3, 6);
    a = RAM(OBJ_TIMER + 1);
    if (a != 0) {
        mff_cycles(0xACD8, 6);
        t = (uint8_t)(RAM(OBJ_TIMER + 1) - 1);
        RAM(OBJ_TIMER + 1) = t;
        SET_NZ(t);
        SYNC_OUT;
        if (!pose_frame(0xACDB)) return;
        mff_cycles(0xACDE, 3);
        nes_cpu_instruction_boundary(0xACB7, 2);
        call_by_address_tail(0xACB7, 4);
        return;
    }
    mff_cycles(0xACD6, 1);
    SYNC_OUT;
    func_ACE1_b4();
}

/*
 * haggar_pose_stop, $ACE1 (bank 4; proposed name): one frame of the stop
 * (animation $47, sound $1D on frame 0); on frame $0C the final stance
 * (animation 3), held for ever (haggar_pose_hold).
 * Called by: haggar_pose_run (falling in at $ACE1) and itself (JMP at
 * $AD14).
 */
void func_ACE1_b4(void) {
    REGS_IN;

    SYNC_OUT;
    if (!pose_switch(0xACE1, 0x47, 0x11)) return;
    SYNC_IN;
    mff_cycles(0xACF0, 6);
    if (RAM(OBJ_ANIM_FRAME) == 0) {
        SYNC_OUT;
        if (!pose_sound(0xACF5, 0x1D)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xACF3, 1);
    }
    mff_cycles(0xACFA, 8);
    a = RAM(OBJ_ANIM_FRAME);
    SET_CMP(a, 0x0C);
    if (z) {
        SYNC_OUT;
        if (!pose_set(0xAD01, 0x03, 0x08)) return;
        func_AD0B_b4();
        return;
    }
    mff_cycles(0xACFF, 1);
    SYNC_OUT;
    if (!pose_frame(0xAD11)) return;
    mff_cycles(0xAD14, 3);
    nes_cpu_instruction_boundary(0xACE1, 2);
    call_by_address_tail(0xACE1, 4);
}

/*
 * haggar_pose_hold, $AD0B (bank 4; proposed name): wait a frame, for ever.
 * Called by: haggar_pose_stop (falling in at $AD0B) and itself (JMP at
 * $AD0E).
 */
void func_AD0B_b4(void) {
    if (!pose_frame(0xAD0B)) return;
    mff_cycles(0xAD0E, 3);
    nes_cpu_instruction_boundary(0xAD0B, 2);
    call_by_address_tail(0xAD0B, 4);
}
