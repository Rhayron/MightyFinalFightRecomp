/*
 * obj_move.c - the fixed bank's movement helpers (docs/objects.md,
 * "Movement helpers (bank $0F)"): speeds applied to positions, gravity,
 * facing, and where the player is from an object.
 *
 * Positions are 8.8 fixed point: obj_x_frac/obj_x/obj_x_hi move by
 * obj_vx_frac/obj_vx (unsigned: the caller picks right or left from
 * obj_dir), obj_y_frac/obj_y by obj_vy_frac/obj_vy (signed, positive = up,
 * so a fall subtracts it). obj_y runs 0-$EF per vertical page obj_y_hi; the
 * page code is there but no route ever leaves page 0.
 *
 * "The player" is slot 0. Distances to the player come back as an absolute
 * value in A with C telling the side, the 6502's SEC/SBC, then EOR #$FF /
 * ADC #1 when it borrowed.
 */
#include "obj_fixed.h"

#define VY_MAX_DOWN     0xF9    /* obj_vy: gravity stops at -7 px per frame */

/* obj_dir_index: the compass direction for each octant (Y = 4 when the
 * player is above, + 2 when to the left, + 1 when |dy| is the larger) and
 * closeness to the diagonal (0-2; the fourth entry is unused, 4):
 * 0 up, 4 right, 8 down, $0C left, clockwise in 16 steps */
#define DIR_INDEX_TABLE 0xF3E6

/* the reach of each obj_kind in x: the attack spot beside the player
 * (1C 16 19 19 1C 1F 00 20 20 20 20 20 00) */
#define KIND_REACH_X    0xFBCC

/* ---- facing ---- */

/*
 * player_face_right, $F03F (name proposed): obj_flags of slot 0 |= $40
 * (sprites mirrored: the art faces left).
 * Out: Z clear (callers read it), X/Y kept.
 * Callers: 15 sites in banks 00, 03, 05, 09, 0E (00:84DD, 0E:D396, ...).
 */
void func_F03F(void)
{
    REGS_IN;

    mff_cycles(0xF03F, 16);
    a = (uint8_t)(RAM(OBJ_FLAGS) | OBJF_FACE_RIGHT);
    SET_NZ(a);
    RAM(OBJ_FLAGS) = a;
    RTS_RETURN();
}

/*
 * player_face_left, $F048 (name proposed): obj_flags of slot 0 &= ~$40.
 * Out: Z as the AND leaves it (callers read it), X/Y kept.
 * Callers: 00:8678, 940D, 9433, 95B1, 03:B909, 0E:D39B, DD17.
 */
void func_F048(void)
{
    REGS_IN;

    mff_cycles(0xF048, 16);
    a = (uint8_t)(RAM(OBJ_FLAGS) & ~OBJF_FACE_RIGHT);
    SET_NZ(a);
    RAM(OBJ_FLAGS) = a;
    RTS_RETURN();
}

/*
 * obj_face_right, $F090: obj_flags of slot X |= $40.
 * In: X = slot. Out: A = the new obj_flags; X, Y, C kept.
 * Callers: about 20 sites (0C:9D85 before moving right, the walks at
 * 0F:F000, ...), and JMPs from 01:A098, 03:B398, 0B:BBE9, 0C:9838, 0D:A9F3.
 */
void func_F090(void)
{
    REGS_IN;

    mff_cycles(0xF090, 17);
    a = (uint8_t)(RAM(OBJ_FLAGS + x) | OBJF_FACE_RIGHT);
    RAM(OBJ_FLAGS + x) = a;
    RTS_RETURN();
}

/*
 * obj_face_left, $F099: obj_flags of slot X &= ~$40.
 * In: X = slot. Out: A = the new obj_flags; X, Y, C kept.
 * Callers: as obj_face_right (0C:9D98 before moving left, 0F:F051, ...).
 */
void func_F099(void)
{
    REGS_IN;

    mff_cycles(0xF099, 17);
    a = (uint8_t)(RAM(OBJ_FLAGS + x) & ~OBJF_FACE_RIGHT);
    RAM(OBJ_FLAGS + x) = a;
    RTS_RETURN();
}

/* ---- speeds ---- */

/*
 * obj_move_right, $F183: x += vx (obj_x_frac, obj_x, the carry into
 * obj_x_hi).
 * In: X = slot. Out: A, C and V as the last ADC leaves them; X, Y kept.
 * Callers: about 60 sites in every object bank, and JMPs (0C:9D88, ...).
 */
void func_F183(void)
{
    REGS_IN;

    mff_cycles(0xF183, 30);
    a = RAM(OBJ_X_FRAC + x);
    c = 0;
    ADC_A(RAM(OBJ_VX_FRAC + x));
    RAM(OBJ_X_FRAC + x) = a;
    a = RAM(OBJ_X + x);
    ADC_A(RAM(OBJ_VX + x));
    RAM(OBJ_X + x) = a;
    if (c) {
        mff_cycles(0xF198, 11);
        a = RAM(OBJ_X_HI + x);
        ADC_A(0);
        RAM(OBJ_X_HI + x) = a;
    } else {
        mff_cycles(0xF196, 1);
    }
    mff_cycles(0xF1A0, 6);
    RTS_RETURN();
}

/*
 * obj_move_left, $F1A1: x -= vx, the borrow out of obj_x_hi.
 * In: X = slot. Out: A, C and V as the last SBC leaves them; X, Y kept.
 * Callers: about 40 sites, and JMPs (0C:9D9B, ...).
 */
void func_F1A1(void)
{
    REGS_IN;

    mff_cycles(0xF1A1, 30);
    a = RAM(OBJ_X_FRAC + x);
    c = 1;
    SBC_A(RAM(OBJ_VX_FRAC + x));
    RAM(OBJ_X_FRAC + x) = a;
    a = RAM(OBJ_X + x);
    SBC_A(RAM(OBJ_VX + x));
    RAM(OBJ_X + x) = a;
    if (!c) {
        mff_cycles(0xF1B6, 11);
        a = RAM(OBJ_X_HI + x);
        SBC_A(0);
        RAM(OBJ_X_HI + x) = a;
    } else {
        mff_cycles(0xF1B4, 1);
    }
    mff_cycles(0xF1BE, 6);
    RTS_RETURN();
}

/*
 * obj_move_down, $F1BF: y += vy; at $F0 and past, the next vertical page
 * (y + $10, obj_y_hi + 1; never executed).
 * In: X = slot. Out: C (obj_y >= $F0, or the page's ADC) and V; X, Y kept.
 * Callers: 00:8E8F, 9204, 94C5, 95C4, 9723, 01:A064, 0B:BC97, and the walk
 * down (0F:F0B0).
 */
void func_F1BF(void)
{
    REGS_IN;

    mff_cycles(0xF1BF, 32);
    a = RAM(OBJ_Y_FRAC + x);
    c = 0;
    ADC_A(RAM(OBJ_VY_FRAC + x));
    RAM(OBJ_Y_FRAC + x) = a;
    a = RAM(OBJ_Y + x);
    ADC_A(RAM(OBJ_VY + x));
    RAM(OBJ_Y + x) = a;
    c = a >= 0xF0;
    if (c) {                                    /* never executed */
        mff_cycles(0xF1D6, 14);
        ADC_A(0x0F);                            /* + $10 with the carry */
        RAM(OBJ_Y + x) = a;
        RAM(OBJ_Y_HI + x)++;
    } else {
        mff_cycles(0xF1D4, 1);
    }
    mff_cycles(0xF1DE, 6);
    RTS_RETURN();
}

/*
 * y -= vy; a borrow is the previous vertical page (y - $10, obj_y_hi - 1;
 * never executed). The same code twice: obj_move_up ($F1DF) and $F22E
 * (obj_fall's rising half), `pc` is the copy's address.
 */
static void move_up(uint16_t pc)
{
    REGS_IN;

    mff_cycles(pc, 30);
    a = RAM(OBJ_Y_FRAC + x);
    c = 1;
    SBC_A(RAM(OBJ_VY_FRAC + x));
    RAM(OBJ_Y_FRAC + x) = a;
    a = RAM(OBJ_Y + x);
    SBC_A(RAM(OBJ_VY + x));
    RAM(OBJ_Y + x) = a;
    if (!c) {                                   /* never executed */
        mff_cycles((uint16_t)(pc + 0x15), 14);
        SBC_A(0x0F);                            /* - $10 with the borrow */
        RAM(OBJ_Y + x) = a;
        RAM(OBJ_Y_HI + x)--;
    } else {
        mff_cycles((uint16_t)(pc + 0x13), 1);
    }
    mff_cycles((uint16_t)(pc + 0x1D), 6);
    RTS_RETURN();
}

/*
 * obj_move_up, $F1DF: y -= vy (see move_up).
 * In: X = slot. Out: C and V as the last SBC leaves them; X, Y kept.
 * Callers: 00:905B, 920F, 9425, 95CF, 01:A046, 03:BD8B, 0B:BC72, the walk
 * up (0F:F105), and a JMP from 05:AD48.
 */
void func_F1DF(void)
{
    move_up(0xF1DF);
}

/*
 * obj_fall, $F1FD: y -= vy (down while vy is negative), then gravity:
 * JMP obj_gravity.
 * In: X = slot. Out: as obj_gravity.
 * Callers: about 25 sites (the hit reactions of bank 0C, the jumps of banks
 * 00, 0B, 0D, ...) and a JMP from 0C:9216.
 */
void func_F1FD(void)
{
    REGS_IN;

    mff_cycles(0xF1FD, 6);
    if (RAM(OBJ_VY + x) & 0x80) {               /* going down */
        mff_cycles(0xF202, 6);
        CALL_FIXED(0xF202, func_F20E);
        JMP_FIXED(0xF205, 0xF24C, func_F24C);
    }
    mff_cycles(0xF200, 1);
    mff_cycles(0xF208, 6);
    CALL_FIXED(0xF208, func_F22E);
    JMP_FIXED(0xF20B, 0xF24C, func_F24C);
}

/*
 * $F20E: y -= vy with vy negative (the object goes down); at $F0 and past,
 * the next vertical page (never executed).
 * In: X = slot. Out: X, Y kept (callers read no flags).
 * Callers: obj_fall (0F:F202), obj_jump_move (0F:F12C).
 */
void func_F20E(void)
{
    REGS_IN;

    mff_cycles(0xF20E, 32);
    a = RAM(OBJ_Y_FRAC + x);
    c = 1;
    SBC_A(RAM(OBJ_VY_FRAC + x));
    RAM(OBJ_Y_FRAC + x) = a;
    a = RAM(OBJ_Y + x);
    SBC_A(RAM(OBJ_VY + x));
    RAM(OBJ_Y + x) = a;
    c = a >= 0xF0;
    if (c) {                                    /* never executed */
        mff_cycles(0xF225, 14);
        ADC_A(0x0F);
        RAM(OBJ_Y + x) = a;
        RAM(OBJ_Y_HI + x)++;
    } else {
        mff_cycles(0xF223, 1);
    }
    mff_cycles(0xF22D, 6);
    RTS_RETURN();
}

/*
 * $F22E: y -= vy with vy positive (the object goes up), as obj_move_up.
 * In: X = slot. Out: X, Y kept (callers read no flags).
 * Callers: obj_fall (0F:F208), obj_jump_move (0F:F14F).
 */
void func_F22E(void)
{
    move_up(0xF22E);
}

/*
 * obj_gravity, $F24C: vy -= gravity ($99, $38 per frame from
 * objects_update), as a 16-bit value; going down it is held at -7 px per
 * frame ($F9.00).
 * In: X = slot. Out: C and V (the SBC, or C of the CMP #$F9); X, Y kept.
 * Callers: obj_fall (JMPs at 0F:F205, F20B), obj_jump_move (0F:F12F, F152).
 */
void func_F24C(void)
{
    REGS_IN;

    mff_cycles(0xF24C, 27);
    a = RAM(OBJ_VY_FRAC + x);
    c = 1;
    SBC_A(RAM(GRAVITY));
    RAM(OBJ_VY_FRAC + x) = a;
    a = RAM(OBJ_VY + x);
    SBC_A(0);
    RAM(OBJ_VY + x) = a;
    if (a & 0x80) {
        mff_cycles(0xF25F, 4);
        c = a >= VY_MAX_DOWN;
        if (!c) {
            mff_cycles(0xF263, 14);
            RAM(OBJ_VY + x) = VY_MAX_DOWN;
            RAM(OBJ_VY_FRAC + x) = 0x00;
        } else {
            mff_cycles(0xF261, 1);
        }
    } else {
        mff_cycles(0xF25D, 1);
    }
    mff_cycles(0xF26D, 6);
    RTS_RETURN();
}

/*
 * obj_init_vy, $F26E: the small downward start of a fall: vy = $FFC0
 * (-0.25 px) for the player, $FFAB (-0.33 px) for any other slot.
 * In: X = slot. Out: C set (the CPX #0); X, Y kept.
 * Callers: 00:8ED7, 08:817D (the stage list spawner), 0E:C7CD, 0F:E074,
 * obj_jump_move (0F:F149).
 */
void func_F26E(void)
{
    REGS_IN;

    mff_cycles(0xF26E, 4);
    c = 1;                                      /* CPX #0 */
    if (x != 0) {
        mff_cycles(0xF272, 20);
        RAM(OBJ_VY_FRAC + x) = 0xAB;
        RAM(OBJ_VY + x) = 0xFF;
        RTS_RETURN();
    }
    mff_cycles(0xF270, 1);
    mff_cycles(0xF27D, 20);
    RAM(OBJ_VY_FRAC + x) = 0xC0;
    RAM(OBJ_VY + x) = 0xFF;
    RTS_RETURN();
}

/* ---- toward the player ---- */

/*
 * obj_dir_to_player, $F314: obj_dir = right (1), or left (2) when the
 * object is at or right of the player (16-bit x, no borrow).
 * In: X = slot. Out: C (no borrow) and V of the high bytes' SBC; X, Y kept.
 * Callers: 05:A731, 08:81BB (spawn), 0C:9778, 981B, 985C (hit reactions),
 * obj_face_player (0F:F343).
 */
void func_F314(void)
{
    REGS_IN;

    mff_cycles(0xF314, 27);
    RAM(OBJ_DIR + x) = DIR_RIGHT;
    a = RAM(OBJ_X + x);
    c = 1;
    SBC_A(RAM(OBJ_X));                          /* only the borrow is kept */
    a = RAM(OBJ_X_HI + x);
    SBC_A(RAM(OBJ_X_HI));
    if (c) {
        mff_cycles(0xF328, 7);
        RAM(OBJ_DIR + x) = DIR_LEFT;
    } else {
        mff_cycles(0xF326, 1);
    }
    mff_cycles(0xF32D, 6);
    RTS_RETURN();
}

/*
 * obj_flip_from_dir, $F32E: obj_flags bit 6 (face right) = obj_dir bit 0,
 * by three RORs and AND #$40 through $00.
 * In: X = slot. Out: A = the new obj_flags, C = obj_dir bit 2 (the third
 * ROR); $00 = the bit; X, Y kept.
 * Callers: 01:AC6B, 08:81BE (spawn), obj_face_player (0F:F346).
 */
void func_F32E(void)
{
    REGS_IN;

    mff_cycles(0xF32E, 35);
    a = RAM(OBJ_DIR + x);
    c = (a >> 2) & 1;
    RAM(TMP0) = (uint8_t)((a & DIR_RIGHT) << 6);
    a = (uint8_t)((RAM(OBJ_FLAGS + x) & ~OBJF_FACE_RIGHT) | RAM(TMP0));
    RAM(OBJ_FLAGS + x) = a;
    RTS_RETURN();
}

/*
 * obj_face_player, $F343: obj_dir_to_player, then obj_flip_from_dir.
 * In: X = slot. Out: as obj_flip_from_dir.
 * Callers: about 20 sites of the boss and enemy AI (banks 01, 03, 0B, 0D),
 * and JMPs (0C:858D, 0D:A674, ...).
 */
void func_F343(void)
{
    REGS_IN;

    mff_cycles(0xF343, 6);
    CALL_FIXED(0xF343, func_F314);
    mff_cycles(0xF346, 6);
    CALL_FIXED(0xF346, func_F32E);
    mff_cycles(0xF349, 6);
    RTS_RETURN();
}

/*
 * obj_dist_y, $F365: A = |player's obj_y - obj_y|.
 * In: X = slot. Out: A; C set when the player is level or below (no
 * borrow), clear when above; V of the SBC or of the negation; X, Y kept.
 * Callers: 05:A734, A7DE, 0C:8B27 (items), enemy_attack_test (0F:F5F7),
 * obj_dist_y_feet (0F:FBE3).
 */
void func_F365(void)
{
    REGS_IN;

    mff_cycles(0xF365, 12);
    a = RAM(OBJ_Y);
    c = 1;
    SBC_A(RAM(OBJ_Y + x));
    if (!c) {
        mff_cycles(0xF36E, 6);
        a ^= 0xFF;
        ADC_A(1);                               /* C is clear: - A */
        c = 0;
    } else {
        mff_cycles(0xF36C, 1);
    }
    mff_cycles(0xF373, 6);
    RTS_RETURN();
}

/*
 * obj_dist_x, $F374: A = |player's x - x| (low byte of the 16-bit
 * difference; the low byte waits on the stack while the high bytes give
 * the side).
 * In: X = slot. Out: A; C set when the player is level or to the right,
 * clear when to the left; V of the SBC or of the negation; X, Y kept.
 * Callers: about 30 sites of the boss and enemy AI, spawn_hit_spark
 * (0F:F4E1), obj_near_player (0F:FB42), obj_dir_to_attack_spot (0F:FB84).
 */
void func_F374(void)
{
    REGS_IN;

    mff_cycles(0xF374, 27);
    a = RAM(OBJ_X);
    c = 1;
    SBC_A(RAM(OBJ_X + x));
    mff_push(a);
    a = RAM(OBJ_X_HI);
    SBC_A(RAM(OBJ_X_HI + x));
    a = mff_pull();
    if (!c) {
        mff_cycles(0xF385, 6);
        a ^= 0xFF;
        ADC_A(1);
        c = 0;
    } else {
        mff_cycles(0xF383, 1);
    }
    mff_cycles(0xF38A, 6);
    RTS_RETURN();
}

/*
 * obj_dir_index, $F38B: the compass direction (0-$0F, 0 up, 4 right,
 * clockwise) from slot X to the player: $00 = |dy| and $01 = |dx|, swapped
 * so that $01 is the larger; $02 = 0 when the smaller is at most a quarter
 * of the larger, 1 when below twice that, 2 nearer the diagonal; then
 * $F3E6[octant * 4 + $02].
 * In: X = slot. Out: A = the direction; C and V clear (the last ADC);
 * $00-$02 as above; X kept, Y the table index.
 * Callers: obj_dir_to_player_feet (0F:FB65), obj_dir_to_attack_spot
 * (0F:FBB5).
 */
void func_F38B(void)
{
    REGS_IN;

    mff_cycles(0xF38B, 16);
    a = RAM(OBJ_Y);
    c = 1;
    SBC_A(RAM(OBJ_Y + x));
    y = 0x00;
    if (!c) {                                   /* the player is above */
        mff_cycles(0xF398, 6);
        a = (uint8_t)(0 - a);
        y = 0x04;
    } else {
        mff_cycles(0xF396, 1);
    }
    mff_cycles(0xF39E, 30);
    RAM(TMP0) = a;
    a = RAM(OBJ_X);
    c = 1;
    SBC_A(RAM(OBJ_X + x));
    mff_push(a);
    a = RAM(OBJ_X_HI);
    SBC_A(RAM(OBJ_X_HI + x));
    a = mff_pull();
    if (!c) {                                   /* the player is to the left */
        mff_cycles(0xF3B1, 8);
        a = (uint8_t)(0 - a);
        y += 2;
    } else {
        mff_cycles(0xF3AF, 1);
    }
    mff_cycles(0xF3B7, 8);
    RAM(TMP1) = a;
    if (a < RAM(TMP0)) {                        /* |dy| is the larger */
        mff_cycles(0xF3BD, 18);
        mff_push(a);
        RAM(TMP1) = RAM(TMP0);
        RAM(TMP0) = mff_pull();
        y++;
    } else {
        mff_cycles(0xF3BB, 1);
    }
    mff_cycles(0xF3C6, 17);
    RAM(TMP2) = 0;
    a = (uint8_t)(RAM(TMP1) >> 2);
    if (a < RAM(TMP0)) {
        mff_cycles(0xF3D2, 12);
        RAM(TMP2)++;
        a = (uint8_t)(a << 1);
        if (a < RAM(TMP0)) {
            mff_cycles(0xF3D9, 5);
            RAM(TMP2)++;
        } else {
            mff_cycles(0xF3D7, 1);
        }
    } else {
        mff_cycles(0xF3D0, 1);
    }
    mff_cycles(0xF3DB, 17);
    a = (uint8_t)(y << 2);
    c = 0;
    ADC_A(RAM(TMP2));
    y = a;
    a = prg_read((uint16_t)(DIR_INDEX_TABLE + y));
    mff_cycles(0xF3E5, 6);
    RTS_RETURN();
}

/*
 * obj_near_player, $FB3E (name proposed): whether the player is within A
 * pixels in x (obj_dist_x) and Y pixels in y from his ground line
 * (obj_dist_y_feet). $00/$01 = the limits, $02/$03 = the distances.
 * In: A = x limit, Y = y limit, X = slot. Out: C set when both distances
 * are within the limits; V of the last SBC; X, Y kept.
 * Callers: 26 sites of the boss and enemy AI (01:A184, 0C:9F66, 0D:A06C,
 * ...).
 */
void func_FB3E(void)
{
    REGS_IN;

    mff_cycles(0xFB3E, 12);
    RAM(TMP0) = a;
    RAM(TMP1) = y;
    CALL_FIXED(0xFB42, func_F374);
    mff_cycles(0xFB45, 9);
    RAM(TMP2) = a;
    CALL_FIXED(0xFB47, func_FBD9);
    mff_cycles(0xFB4A, 13);
    RAM(TMP3) = a;
    a = RAM(TMP0);
    c = 1;
    SBC_A(RAM(TMP2));
    if (c) {
        mff_cycles(0xFB53, 10);
        a = RAM(TMP1);
        c = 1;
        SBC_A(RAM(TMP3));
        if (!c)
            mff_cycles(0xFB58, 1);              /* BCC to the next instruction */
    } else {
        mff_cycles(0xFB51, 1);
    }
    mff_cycles(0xFB5A, 6);
    RTS_RETURN();
}

/*
 * obj_dir_to_player_feet, $FB5B (name proposed): obj_dir_index toward the
 * player's ground line (player_ground_y in the player's obj_y meanwhile,
 * $0F keeping it), so a jumping player counts where he stands.
 * In: X = slot. Out: A = the direction, C and V as obj_dir_index; $0E = A.
 * Callers: 01:A9CA, 03:B653, BB29, 0B:B842, B87A, 0C:9DC2, 0D:A085, A0F0,
 * A3FA, A7D7.
 */
void func_FB5B(void)
{
    REGS_IN;

    mff_cycles(0xFB5B, 20);
    RAM(TMP15) = RAM(OBJ_Y);
    RAM(OBJ_Y) = RAM(PLAYER_GROUND_Y);
    CALL_FIXED(0xFB65, func_F38B);
    mff_cycles(0xFB68, 19);
    RAM(TMP14) = a;
    RAM(OBJ_Y) = RAM(TMP15);
    a = RAM(TMP14);
    RTS_RETURN();
}

/*
 * obj_dir_to_attack_spot, $FB72 (name proposed): the direction from slot X
 * to the spot it attacks from: the player's ground line, $FBCC[obj_kind]
 * pixels from the player toward the object. The player's obj_x/obj_x_hi/
 * obj_y are moved there for obj_dir_index and put back ($0D-$0F keep them,
 * $0C the result).
 * In: X = slot. Out: A = the direction, C and V as obj_dir_index; X kept.
 * Callers: 01:A44B, ABA3, 0D:A233.
 */
void func_FB72(void)
{
    REGS_IN;

    mff_cycles(0xFB72, 31);
    RAM(TMP15) = RAM(OBJ_Y);
    RAM(TMP14) = RAM(OBJ_X);
    RAM(TMP13) = RAM(OBJ_X_HI);
    y = RAM(OBJ_KIND + x);
    CALL_FIXED(0xFB84, func_F374);
    mff_cycles(0xFB87, 2);
    if (!c) {                                   /* the player is to the left */
        mff_cycles(0xFB89, 10);
        a = RAM(OBJ_X);
        c = 0;
        ADC_A(prg_read((uint16_t)(KIND_REACH_X + y)));
        mff_cycles(0xFB90, 17);
        RAM(OBJ_X) = a;
        a = RAM(OBJ_X_HI);
        ADC_A(0);
        RAM(OBJ_X_HI) = a;
        TAIL_FIXED(0xFBB0, func_FBB0);
    }
    mff_cycles(0xFB87, 1);
    mff_cycles(0xFB9E, 10);
    a = RAM(OBJ_X);
    c = 1;
    SBC_A(prg_read((uint16_t)(KIND_REACH_X + y)));
    mff_cycles(0xFBA5, 27);
    RAM(OBJ_X) = a;
    a = RAM(OBJ_X_HI);
    SBC_A(0);
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y) = RAM(PLAYER_GROUND_Y);
    CALL_FIXED(0xFBB5, func_F38B);
    mff_cycles(0xFBB8, 33);
    RAM(TMP12) = a;
    RAM(OBJ_Y) = RAM(TMP15);
    RAM(OBJ_X) = RAM(TMP14);
    RAM(OBJ_X_HI) = RAM(TMP13);
    a = RAM(TMP12);
    RTS_RETURN();
}

/*
 * $FBB0: the end of obj_dir_to_attack_spot (its JMP at 0F:FB9B):
 * obj_dir_index at the player's ground line, then the player's position put
 * back from $0D-$0F. The other path runs the same code inline, charged
 * with what comes before.
 */
void func_FBB0(void)
{
    REGS_IN;

    mff_cycles(0xFBB0, 13);
    RAM(OBJ_Y) = RAM(PLAYER_GROUND_Y);
    CALL_FIXED(0xFBB5, func_F38B);
    mff_cycles(0xFBB8, 33);
    RAM(TMP12) = a;
    RAM(OBJ_Y) = RAM(TMP15);
    RAM(OBJ_X) = RAM(TMP14);
    RAM(OBJ_X_HI) = RAM(TMP13);
    a = RAM(TMP12);
    RTS_RETURN();
}

/*
 * obj_dist_y_feet, $FBD9 (name proposed): obj_dist_y from the player's
 * ground line (player_ground_y in his obj_y meanwhile, $0F keeping it).
 * In: X = slot. Out: A, C and V as obj_dist_y; $0E = A; X, Y kept.
 * Callers: 01:A251, AB5A, AB66, 03:B5DC, B5E8, 0D:A588, obj_near_player
 * (0F:FB47).
 */
void func_FBD9(void)
{
    REGS_IN;

    mff_cycles(0xFBD9, 20);
    RAM(TMP15) = RAM(OBJ_Y);
    RAM(OBJ_Y) = RAM(PLAYER_GROUND_Y);
    CALL_FIXED(0xFBE3, func_F365);
    mff_cycles(0xFBE6, 19);
    RAM(TMP14) = a;
    RAM(OBJ_Y) = RAM(TMP15);
    a = RAM(TMP14);
    RTS_RETURN();
}
