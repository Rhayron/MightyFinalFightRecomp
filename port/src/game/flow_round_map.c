/*
 * flow_round_map.c - the round map (04:8D00)
 *
 * Before each round the game draws the map (screen 8 + the round) and
 * runs round_map (04:8D00): the character stands at ($28, $32) and slot 2
 * marks the round's place on the map. Each character then plays its own
 * short scene, a frame at a time through scene_frame (0F:F406, which draws
 * the objects and waits a frame):
 *   Cody   animation $73; at its frame $10 a second object (slot 1,
 *          animation $74) flies right at 5 pixels a frame, and the scene
 *          ends when that object's animation reaches frame $0A;
 *   Guy    animation $46; at its frame 2 the second object (animation
 *          $5F, effect $1B) flies right at 4 pixels a frame until x $D0;
 *   Haggar animation $4B, walks right at $02.4C pixels a frame (effect $13
 *          every 8 frames) until x $DC.
 * Then the marker's animation restarts at frame 1 (round_map_marker) and
 * round_map returns; the caller waits 45 frames more.
 *
 * Each scene's loop is entered by an indirect JMP and comes back to its
 * head by JMP, which the lifted code makes tail calls by address; the
 * runtime turns the repeated tail calls into a loop.
 */
#include "flow.h"

#define MAP_PALETTES        0x8F18  /* 16 background colors per player_id, $10 apart */
#define MAP_MARKER_X        0x8DCE  /* the marker's x and y, by round */
#define MAP_MARKER_Y        0x8DD3
#define MAP_SCENES_LO       0x8DC8  /* each player_id's scene: $8DD8, $8E56, $8ED4 */
#define MAP_SCENES_HI       0x8DCB

void func_F090(void);   /* obj_face_right */
void func_F183(void);   /* obj_move_right */
void func_F288(void);   /* obj_set_anim: slot X, animation A */
void func_F2A0(void);   /* obj_spawn: slot Y, animation A */
void func_F406(void);   /* scene_frame: draws the objects, waits a frame */
void func_8DEC_b2(void);
void func_8E65_b2(void);
void func_8EED_b2(void);
void func_8F07_b2(void);

/*
 * round_map (04:8D00, proposed name)
 * Stops the music; the map's background colors for the player
 * (MAP_PALETTES + $10 * player_id; player_id 1 Guy, 2 Haggar, else Cody);
 * CHR R0/R1 = $48/$4A and R3 = $3C; the player's 8 sprite colors as in
 * player_load_graphics, then sprite colors $1A/$1B = $30/$25; slot 0 (the
 * character) and slot 2 (the marker, animation $7C) live with flags $D0;
 * then the player's scene by JMP ($0000).
 * palette_base+$1B does not get its $25: the second store goes to $06DA
 * (unk_6da, written nowhere else and never read), which looks like a typo
 * in the game for $063B.
 * Callers: main_task's title_start_game (0E:C715) and the round change
 * in player_state_transition (0E:DFC9).
 */
void func_8D00_b2(void) {
    REGS_IN;
    int player;
    uint16_t pc;

    mff_cycles(0x8D00, 8);
    a = SOUND_STOP_ALL;
    SET_NZ(a);
    SYNC_OUT;
    if (!mff_call_fn(0x0D04, func_F34A, 0xE000)) return;   /* sound_queue_push */
    SYNC_IN;
    mff_cycles(0x8D05, 7);
    y = 0x0F;
    a = RAM(PLAYER_ID);
    player = 0;
    if (a == 0) {
        mff_cycles(0x8D09, 1);
    } else {
        mff_cycles(0x8D0B, 4);
        c = a >= 1;
        if (a == 1) {
            mff_cycles(0x8D0D, 1);
            player = 1;
        } else {
            mff_cycles(0x8D0F, 4);
            c = a >= 2;
            if (a == 2) {
                mff_cycles(0x8D11, 1);
                player = 2;
            }
        }
    }
    /* the ROM's three copies of the loop are $10 bytes apart from $8D13 */
    pc = (uint16_t)(0x8D13 + 0x10 * player);
    for (;;) {
        mff_cycles(pc, 4);
        a = prg_read((uint16_t)(MAP_PALETTES + 0x10 * player + y));
        mff_cycles(pc + 3, 14);
        RAM(PALETTE_BUF + y) = a;
        RAM(PALETTE_BASE + y) = a;
        y--;
        z = y == 0;
        if (y & 0x80) break;
        mff_cycles(pc, 1);
    }
    if (player != 2) {
        mff_cycles(pc + 0x0C, 5);
        RAM(PALETTE_REQUEST) = y;
        mff_cycles(pc + 0x0E, 1);           /* BNE $8D41, taken: Y is $FF */
    } else {
        mff_cycles(0x8D3F, 3);
        RAM(PALETTE_REQUEST) = y;
    }

    mff_cycles(0x8D41, 22);
    a = 0x48;
    RAM(CHR_BANKS) = a;
    a = 0x4A;
    RAM(CHR_BANKS + 1) = a;
    a = 0x3C;
    RAM(CHR_BANKS + 3) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(PLAYER_PAL_END + y));
    mff_cycles(0x8D52, 4);
    x = a;
    y = 7;
    for (;;) {
        mff_cycles(0x8D55, 4);
        a = prg_read((uint16_t)(PLAYER_PAL_TABLE + x));
        mff_cycles(0x8D58, 16);
        RAM(PALETTE_BUF + 16 + y) = a;
        RAM(PALETTE_BASE + 16 + y) = a;
        x--;
        y--;
        if (y & 0x80) break;
        mff_cycles(0x8D55, 1);
    }
    mff_cycles(0x8D62, 90);
    a = 0x30;
    RAM(PALETTE_BUF + 26) = a;
    RAM(PALETTE_BASE + 26) = a;
    a = 0x25;
    RAM(PALETTE_BUF + 27) = a;
    RAM(UNK_6DA) = a;
    a = 0xFF;
    RAM(PALETTE_REQUEST) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    RAM(OBJ_STATE + 2) = a;
    a = 0xD0;
    RAM(OBJ_FLAGS) = a;
    RAM(OBJ_FLAGS + 2) = a;
    a = 0;
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y_HI) = a;
    RAM(OBJ_ID) = a;
    RAM(OBJ_X_HI + 2) = a;
    RAM(OBJ_Y_HI + 2) = a;
    RAM(OBJ_ID + 2) = a;
    a = 0x28;
    RAM(OBJ_X) = a;
    a = 0x32;
    RAM(OBJ_Y) = a;
    y = RAM(ROUND);
    a = prg_read((uint16_t)(MAP_MARKER_X + y));
    mff_cycles(0x8DA9, 8);
    RAM(OBJ_X + 2) = a;
    a = prg_read((uint16_t)(MAP_MARKER_Y + y));
    mff_cycles(0x8DAF, 14);
    RAM(OBJ_Y + 2) = a;
    x = 2;
    a = 0x7C;
    SET_NZ(a);
    SYNC_OUT;
    if (!mff_call_fn(0x0DB8, func_F288, 0xE000)) return;   /* obj_set_anim */
    SYNC_IN;
    mff_cycles(0x8DB9, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(MAP_SCENES_LO + y));
    mff_cycles(0x8DBE, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(MAP_SCENES_HI + y));
    SET_NZ(a);
    mff_cycles(0x8DC3, 8);
    RAM(TMP1) = a;
    SYNC_OUT;
    {   /* JMP ($0000) */
        uint16_t jt = nes_read16zp(0x0000);
        nes_cpu_instruction_boundary(jt, 2);
        call_by_address_tail(jt, -1);
    }
}

/*
 * $8DD8: Cody's scene. Unless slot 0 already shows animation $73: that
 * animation, chr_banks+2 = $3F and effect $16. Then its loop ($8DEC).
 * Caller: round_map by JMP ($0000).
 */
void func_8DD8_b2(void) {
    REGS_IN;

    mff_cycles(0x8DD8, 8);
    a = 0x73;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    SET_NZ(a - m);
    if (!z) {
        mff_cycles(0x8DDF, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0DE1, func_F294, 0xE000)) return;   /* player_set_anim */
        SYNC_IN;
        mff_cycles(0x8DE2, 8);
        a = 0x3F;
        SET_NZ(a);
        SYNC_OUT;
        if (!mff_call_fn(0x0DE6, func_F440, 0xE000)) return;   /* chr_banks+2 = A */
        SYNC_IN;
        mff_cycles(0x8DE7, 8);
        a = 0x16;
        SET_NZ(a);
        SYNC_OUT;
        if (!mff_call_fn(0x0DEB, func_F34C, 0xE000)) return;   /* sound_queue_push_sfx */
        SYNC_IN;
    } else {
        mff_cycles(0x8DDD, 1);
    }
    SYNC_OUT;
    func_8DEC_b2();
}

/*
 * $8DEC: a frame of Cody's scene. At animation frame $10 (timer 0) the
 * second object starts at slot 0 + ($0C, 4) with animation $74 and speed
 * $05.00; at frame $11 the animation holds there. Slot 1, while live,
 * faces and moves right. When slot 1's animation frame is $0A the marker
 * restarts and the scene returns; else scene_frame and again.
 * Callers: $8DD8 falls into it; it JMPs back to itself.
 */
void func_8DEC_b2(void) {
    REGS_IN;

    mff_cycles(0x8DEC, 8);
    a = RAM(OBJ_ANIM_FRAME);
    if (a == 0x10) {
        mff_cycles(0x8DF3, 6);
        a = RAM(OBJ_ANIM_TIMER);
        if (a == 0) {
            mff_cycles(0x8DF8, 50);
            a = RAM(OBJ_X);
            c = 0;
            m = 0x0C;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(OBJ_X + 1) = a;
            a = RAM(OBJ_Y);
            c = 0;
            m = 0x04;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            RAM(OBJ_Y + 1) = a;
            a = RAM(OBJ_X_HI);
            RAM(OBJ_X_HI + 1) = a;
            a = RAM(OBJ_Y_HI);
            RAM(OBJ_Y_HI + 1) = a;
            y = 1;
            a = 0x74;
            SET_NZ(a);
            SYNC_OUT;
            if (!mff_call_fn(0x0E1C, func_F2A0, 0xE000)) return;   /* obj_spawn */
            SYNC_IN;
            mff_cycles(0x8E1D, 12);
            a = 0x00;
            RAM(OBJ_VX_FRAC + 1) = a;
            a = 0x05;
            RAM(OBJ_VX + 1) = a;
        } else {
            mff_cycles(0x8DF6, 2);
        }
    } else {
        mff_cycles(0x8DF1, 2);
    }
    mff_cycles(0x8E27, 8);
    a = RAM(OBJ_ANIM_FRAME);
    c = a >= 0x11;
    if (a == 0x11) {
        mff_cycles(0x8E2E, 12);
        a = 0x11;
        RAM(OBJ_ANIM_FRAME) = a;
        a = 0;
        RAM(OBJ_ANIM_TIMER) = a;
    } else {
        mff_cycles(0x8E2C, 1);
    }
    mff_cycles(0x8E38, 8);
    x = 1;
    a = RAM(OBJ_STATE + 1);
    SET_NZ(a);
    if (n) {
        mff_cycles(0x8E3F, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0E41, func_F090, 0xE000)) return;   /* obj_face_right */
        SYNC_IN;
        mff_cycles(0x8E42, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0E44, func_F183, 0xE000)) return;   /* obj_move_right */
        SYNC_IN;
    } else {
        mff_cycles(0x8E3D, 1);
    }
    mff_cycles(0x8E45, 8);
    a = RAM(OBJ_ANIM_FRAME + 1);
    c = a >= 0x0A;
    SET_NZ(a - 0x0A);
    if (z) {
        mff_cycles(0x8E4C, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0E4E, 0x8F07, 2)) return;       /* round_map_marker */
        SYNC_IN;
        mff_cycles(0x8E4F, 6);
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0x8E4A, 1);
    mff_cycles(0x8E50, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0E52, func_F406, 0xE000)) return;   /* scene_frame */
    SYNC_IN;
    mff_cycles(0x8E53, 3);                  /* JMP $8DEC */
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8DEC, 2); call_by_address_tail(0x8DEC, 2); return;
}

/*
 * $8E56: Guy's scene. Unless slot 0 already shows animation $46: that
 * animation and chr_banks+2 = $16. Then its loop ($8E65).
 * Caller: round_map by JMP ($0000).
 */
void func_8E56_b2(void) {
    REGS_IN;

    mff_cycles(0x8E56, 8);
    a = 0x46;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    SET_NZ(a - m);
    if (!z) {
        mff_cycles(0x8E5D, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0E5F, func_F294, 0xE000)) return;   /* player_set_anim */
        SYNC_IN;
        mff_cycles(0x8E60, 8);
        a = 0x16;
        SET_NZ(a);
        SYNC_OUT;
        if (!mff_call_fn(0x0E64, func_F440, 0xE000)) return;   /* chr_banks+2 = A */
        SYNC_IN;
    } else {
        mff_cycles(0x8E5B, 1);
    }
    SYNC_OUT;
    func_8E65_b2();
}

/*
 * $8E65: a frame of Guy's scene. At animation frame 2 (timer 0) effect $1B
 * and the second object at slot 0 + (8, 4) with animation $5F and speed
 * $04.00; at frame 4 the animation holds there. Slot 1, while live, faces
 * and moves right, and once its x is $D0 or more the marker restarts and
 * the scene returns; else scene_frame and again.
 * Callers: $8E56 falls into it; it JMPs back to itself.
 */
void func_8E65_b2(void) {
    REGS_IN;

    mff_cycles(0x8E65, 8);
    a = RAM(OBJ_ANIM_FRAME);
    if (a == 0x02) {
        mff_cycles(0x8E6C, 6);
        a = RAM(OBJ_ANIM_TIMER);
        if (a == 0) {
            mff_cycles(0x8E71, 48);
            a = RAM(OBJ_X);
            c = 0;
            m = 0x08;
            w = a + m + c;
            a = (uint8_t)w;
            RAM(OBJ_X + 1) = a;
            a = RAM(OBJ_Y);
            c = 0;
            m = 0x04;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            RAM(OBJ_Y + 1) = a;
            a = RAM(OBJ_X_HI);
            RAM(OBJ_X_HI + 1) = a;
            a = RAM(OBJ_Y_HI);
            RAM(OBJ_Y_HI + 1) = a;
            a = 0x1B;
            SET_NZ(a);
            SYNC_OUT;
            if (!mff_call_fn(0x0E93, func_F34C, 0xE000)) return;   /* sound_queue_push_sfx */
            SYNC_IN;
            mff_cycles(0x8E94, 10);
            y = 1;
            a = 0x5F;
            SET_NZ(a);
            SYNC_OUT;
            if (!mff_call_fn(0x0E9A, func_F2A0, 0xE000)) return;   /* obj_spawn */
            SYNC_IN;
            mff_cycles(0x8E9B, 12);
            a = 0x00;
            RAM(OBJ_VX_FRAC + 1) = a;
            a = 0x04;
            RAM(OBJ_VX + 1) = a;
        } else {
            mff_cycles(0x8E6F, 1);
        }
    } else {
        mff_cycles(0x8E6A, 1);
    }
    mff_cycles(0x8EA5, 8);
    a = RAM(OBJ_ANIM_FRAME);
    c = a >= 0x04;
    if (a == 0x04) {
        mff_cycles(0x8EAC, 12);
        a = 0x04;
        RAM(OBJ_ANIM_FRAME) = a;
        a = 0;
        RAM(OBJ_ANIM_TIMER) = a;
    } else {
        mff_cycles(0x8EAA, 1);
    }
    mff_cycles(0x8EB6, 6);
    a = RAM(OBJ_STATE + 1);
    SET_NZ(a);
    if (n) {
        mff_cycles(0x8EBB, 8);
        x = 1;
        SET_NZ(x);
        SYNC_OUT;
        if (!mff_call_fn(0x0EBF, func_F090, 0xE000)) return;   /* obj_face_right */
        SYNC_IN;
        mff_cycles(0x8EC0, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0EC2, func_F183, 0xE000)) return;   /* obj_move_right */
        SYNC_IN;
        mff_cycles(0x8EC3, 8);
        a = RAM(OBJ_X + 1);
        c = a >= 0xD0;
        SET_NZ(a - 0xD0);
        if (c) {
            mff_cycles(0x8ECA, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0ECC, 0x8F07, 2)) return;   /* round_map_marker */
            SYNC_IN;
            mff_cycles(0x8ECD, 6);
            SYNC_OUT;
            mff_rts();
            return;
        }
        mff_cycles(0x8EC8, 1);
    } else {
        mff_cycles(0x8EB9, 1);
    }
    mff_cycles(0x8ECE, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0ED0, func_F406, 0xE000)) return;   /* scene_frame */
    SYNC_IN;
    mff_cycles(0x8ED1, 3);                  /* JMP $8E65 */
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8E65, 2); call_by_address_tail(0x8E65, 2); return;
}

/*
 * $8ED4: Haggar's scene. Unless slot 0 already shows animation $4B: that
 * animation, chr_banks+2 = $0F and speed $02.4C. Then its loop ($8EED).
 * Caller: round_map by JMP ($0000).
 */
void func_8ED4_b2(void) {
    REGS_IN;

    mff_cycles(0x8ED4, 8);
    a = 0x4B;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    SET_NZ(a - m);
    if (!z) {
        mff_cycles(0x8EDB, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0EDD, func_F294, 0xE000)) return;   /* player_set_anim */
        SYNC_IN;
        mff_cycles(0x8EDE, 8);
        a = 0x0F;
        SET_NZ(a);
        SYNC_OUT;
        if (!mff_call_fn(0x0EE2, func_F440, 0xE000)) return;   /* chr_banks+2 = A */
        SYNC_IN;
        mff_cycles(0x8EE3, 12);
        a = 0x4C;
        RAM(OBJ_VX_FRAC) = a;
        a = 0x02;
        RAM(OBJ_VX) = a;
    } else {
        mff_cycles(0x8ED9, 1);
    }
    SYNC_OUT;
    func_8EED_b2();
}

/*
 * $8EED: a frame of Haggar's scene. Effect $13 when draw_parity & 7 is 0;
 * slot 0 faces and moves right; once its x is $DC or more the scene falls
 * into round_map_marker ($8F07), which returns; else scene_frame and again.
 * Callers: $8ED4 falls into it; it JMPs back to itself.
 */
void func_8EED_b2(void) {
    REGS_IN;

    mff_cycles(0x8EED, 7);
    a = RAM(DRAW_PARITY);
    a &= 0x07;
    if (a == 0) {
        mff_cycles(0x8EF3, 8);
        a = 0x13;
        SET_NZ(a);
        SYNC_OUT;
        if (!mff_call_fn(0x0EF7, func_F34C, 0xE000)) return;   /* sound_queue_push_sfx */
        SYNC_IN;
    } else {
        mff_cycles(0x8EF1, 1);
    }
    mff_cycles(0x8EF8, 8);
    x = 0;
    SET_NZ(x);
    SYNC_OUT;
    if (!mff_call_fn(0x0EFC, func_F090, 0xE000)) return;   /* obj_face_right */
    SYNC_IN;
    mff_cycles(0x8EFD, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EFF, func_F183, 0xE000)) return;   /* obj_move_right */
    SYNC_IN;
    mff_cycles(0x8F00, 8);
    a = RAM(OBJ_X);
    c = a >= 0xDC;
    SET_NZ(a - 0xDC);
    if (c) {
        SYNC_OUT;
        func_8F07_b2();
        return;
    }
    mff_cycles(0x8F05, 1);
    mff_cycles(0x8F12, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0F14, func_F406, 0xE000)) return;   /* scene_frame */
    SYNC_IN;
    mff_cycles(0x8F15, 3);                  /* JMP $8EED */
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8EED, 2); call_by_address_tail(0x8EED, 2); return;
}

/*
 * round_map_marker (04:8F07, proposed name)
 * Restarts slot 2's animation (the marker) at frame 1, timer 0.
 * Callers: Cody's and Guy's scenes (04:8E4C, 04:8ECA); Haggar's falls in.
 */
void func_8F07_b2(void) {
    REGS_IN;

    mff_cycles(0x8F07, 18);
    a = 0;
    RAM(OBJ_ANIM_TIMER + 2) = a;
    a = 1;
    SET_NZ(a);
    RAM(OBJ_ANIM_FRAME + 2) = a;
    SYNC_OUT;
    mff_rts();
}
