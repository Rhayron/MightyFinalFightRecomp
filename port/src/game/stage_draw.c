/*
 * stage_draw.c - drawing the objects into the sprite buffer (docs/assets.md,
 * "Sprites"; docs/objects.md, "Drawing")
 *
 * Once a gameplay frame gameplay_draw hides every OAM entry ($C31D), sets
 * oam_index ($97) to 4 (entry 0 stays hidden) and calls objects_draw,
 * which hands the live slots to object_draw in an order that changes every
 * frame: past eight sprites on a line, the ones left out change too, and
 * the player sees flicker instead of holes.
 *
 * object_draw places the object (world objects relative to the camera),
 * steps its animation, and copies the sprites of its current frame into
 * the buffer at $0200 + oam_index. The data is in the sprite bank pair of
 * the object's id ($EB93: $0A/$0B or $02/$03, mapped at $8000/$A000):
 *     animation $8000/$8080 + id: [last frame, period, frame id x (last+1)]
 *     frame $8100/$8200 + id: [count-1 | $80 for a layout in bank $03
 *         (else $0B), layout, (tile, attribute) x count]
 *     layout $B100/$B200 + l, in that bank: [(dy, dx) x count], signed
 * Scratch: $00/$01 the animation script, then $00 = $F0 (the lowest line)
 * and $01 = dx; $02/$03 the frame; $04 sprites left; $05/$06 the layout - 2;
 * $10 the flip bit (6), $11 the priority bit (5); $12/$13 the object's
 * screen y and x.
 */
#include "stage.h"

/* the sprite bank pair of each object id, 160 bytes */
#define K_OBJ_SPRITE_BANK  0xEB93
/* ids object_offscreen frees instead of pulling back, $FF-terminated:
 * $2E $9D $7F $91 $95, the hit spark $01, the items $10-$13 and $61 */
#define K_OFFSCREEN_FREE   0xECC3

/* in the sprite banks */
#define ANIM_SCRIPT_LO   0x8000
#define ANIM_SCRIPT_HI   0x8080
#define FRAME_LO         0x8100
#define FRAME_HI         0x8200
#define LAYOUT_LO        0xB100
#define LAYOUT_HI        0xB200

/* sprites at or below this y are dropped */
#define SPRITE_Y_LIMIT   0xF0
/* the y of a hidden OAM entry */
#define SPRITE_HIDDEN    0xF8
/* how far off the screen an object is still animated, not pulled back */
#define NEAR_DISTANCE    0x68
/* the hit spark's animation: off its screen page it is always "far" */
#define ANIM_HIT_SPARK   0x7E

/* the fields object_free clears, in its order; obj_spawn_index gets $FF.
 * Position, speed, animation, life and the timers $0500-$0550 stay. */
static const uint16_t k_free_fields[] = {
    OBJ_STATE, OBJ_SAVED_STATE, OBJ_FLAGS, OBJ_THROW_FLAG, OBJ_HIT_COOLDOWN, OBJ_ID,
    OBJ_SAVED_ID, OBJ_ATTACK, OBJ_ATTACK_DAMAGE, OBJ_INVULN, OBJ_HIT_PLAYER, OBJ_HIT_TIMER,
    OBJ_KIND, OBJ_TIER, OBJ_AI_TIMER, OBJ_AI_COUNT, OBJ_AI_FLAG, OBJ_SPAWN_GROUP, OBJ_HELD,
};

/*
 * objects_draw, $E9BC: draws slot 15, then the slot below the X object_draw
 * returned (slot 14 when slot 15 drew nothing; the next OAM offset - 1, a
 * slot 3, 7, 11 or 15 or even a "slot" past 15, when it did), then
 * increments draw_parity ($95) and draws slots 0-15 when it is even, 13
 * down to 0 when it is odd. draw_slot ($96) holds the loop's slot across
 * the calls. A slot drawn twice is animated twice.
 * Out: even pass X = $10, C set; odd pass X = $FF, N set.
 * Called by: gameplay_draw (JSR at $C9A4), 0E:C688 and 0F:F415 (the
 * redraws outside the gameplay loop).
 */
void func_E9BC(void) {
    REGS_IN;

    /* one charge per instruction (a 1 is a taken branch's extra cycle):
     * a lag frame's NMI lands in the drawing, here, at the console's
     * instruction (docs/timing.md) */
    mff_cycles(0xE9BC, 2);
    x = 0x0F;
    mff_cycles(0xE9BE, 4);
    a = RAM(OBJ_STATE + x);
    SET_NZ(a);
    mff_cycles(0xE9C1, 2);
    if (n) {
        mff_cycles(0xE9C3, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x09C5, func_E9FE, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xE9C1, 1);
    }
    mff_cycles(0xE9C6, 2);
    x--;                                        /* object_draw's X, not 15 */
    mff_cycles(0xE9C7, 4);
    a = RAM(OBJ_STATE + x);
    SET_NZ(a);
    mff_cycles(0xE9CA, 2);
    if (n) {
        mff_cycles(0xE9CC, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x09CE, func_E9FE, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xE9CA, 1);
    }

    mff_cycles(0xE9CF, 5);
    RAM(DRAW_PARITY)++;
    mff_cycles(0xE9D1, 3);
    mff_cycles(0xE9D3, 2);
    c = RAM(DRAW_PARITY) & 1;
    mff_cycles(0xE9D4, 2);
    if (!c) {
        mff_cycles(0xE9D6, 2);
        x = 0;
        mff_cycles(0xE9D8, 3);
        RAM(DRAW_SLOT) = x;
        for (;;) {
            mff_cycles(0xE9DA, 4);
            a = RAM(OBJ_STATE + x);
            SET_NZ(a);
            mff_cycles(0xE9DD, 2);
            if (n) {
                mff_cycles(0xE9DF, 6);
                SYNC_OUT;
                if (!mff_call_fn(0x09E1, func_E9FE, 0xE000)) return;
                SYNC_IN;
            } else {
                mff_cycles(0xE9DD, 1);
            }
            mff_cycles(0xE9E2, 5);
            RAM(DRAW_SLOT)++;
            mff_cycles(0xE9E4, 3);
            x = RAM(DRAW_SLOT);
            mff_cycles(0xE9E6, 2);
            SET_CMP(x, 0x10);
            mff_cycles(0xE9E8, 2);
            if (z) break;
            mff_cycles(0xE9E8, 1);
        }
        mff_cycles(0xE9EA, 6);
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0xE9D4, 1);
    mff_cycles(0xE9EB, 2);
    x = 0x0D;
    mff_cycles(0xE9ED, 3);
    RAM(DRAW_SLOT) = x;
    for (;;) {
        mff_cycles(0xE9EF, 4);
        a = RAM(OBJ_STATE + x);
        SET_NZ(a);
        mff_cycles(0xE9F2, 2);
        if (n) {
            mff_cycles(0xE9F4, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x09F6, func_E9FE, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xE9F2, 1);
        }
        mff_cycles(0xE9F7, 5);
        RAM(DRAW_SLOT)--;
        mff_cycles(0xE9F9, 3);
        x = RAM(DRAW_SLOT);
        SET_NZ(x);
        mff_cycles(0xE9FB, 2);
        if (n) break;
        mff_cycles(0xE9FB, 1);
    }
    mff_cycles(0xE9FD, 6);
    SYNC_OUT;
    mff_rts();
}

/*
 * object_draw, $E9FE: places slot X. A world object (obj_flags bit 4) is
 * on the camera's screen when obj_x_hi:obj_x - scroll_x_hi:scroll_x has a
 * zero high byte (and obj_y_hi is 0); then $13 = the low byte. Otherwise
 * it is "near" when less than $68 pixels off the screen: animated, not
 * drawn, on-screen bit cleared (bg_probe and the hit tests skip it); a "far"
 * one, the hit spark off its page, and anything off the screen in round 4
 * area 2 (no route) go to object_offscreen. A screen object is drawn at
 * obj_x. On screen, bit 7 is set and, unless hidden (bit 2), the sprites
 * go out (object_draw_sprites).
 * In: X = slot. Out: X is the next OAM offset if sprites went out (see
 * objects_draw), else the slot.
 * Called by: objects_draw (JSR at $E9C3, $E9CC, $E9DF, $E9F4).
 */
void func_E9FE(void) {
    REGS_IN;
    enum { ON_SCREEN, NEAR, FAR } where;

    mff_cycles(0xE9FE, 8);
    if (RAM(OBJ_FLAGS + x) & OBJF_WORLD) {
        mff_cycles(0xEA05, 21);
        a = RAM(OBJ_X + x);
        c = 1;
        SBC_A(RAM(SCROLL_X));
        RAM(TMP19) = a;
        a = RAM(OBJ_X_HI + x);
        SBC_A(RAM(SCROLL_X_HI));
        if (a == 0) {
            mff_cycles(0xEA14, 6);
            a = RAM(OBJ_Y_HI + x);
            if (a == 0) {
                mff_cycles(0xEA17, 1);
                where = ON_SCREEN;
            } else {                            /* no route: obj_y_hi is always 0 */
                mff_cycles(0xEA19, 4);
                c = 1;                          /* CPX #0 */
                if (x == 0) {
                    mff_cycles(0xEA1B, 1);
                    where = NEAR;
                } else {
                    mff_cycles(0xEA1D, 6);
                    a = RAM(OBJ_Y_HI + x);
                    SET_NZ(a);
                    mff_cycles(0xEA20, n ? 1 : 2 + 1);
                    where = FAR;
                }
            }
        } else {
            /* another page: how far off the screen, either side */
            mff_cycles(0xEA12, 1 + 5);
            a = RAM(TMP19);
            if (!c) {
                mff_cycles(0xEA28, 4);
                a ^= 0xFF;
                ADC_A(1);
            } else {
                mff_cycles(0xEA26, 1);
            }
            mff_cycles(0xEA2C, 4);
            SET_CMP(a, NEAR_DISTANCE);
            if (c) {
                where = FAR;
            } else {
                mff_cycles(0xEA2E, 1 + 4 + 8);  /* BCC taken, CMP #0 / BCC never */
                c = 1;
                a = RAM(OBJ_ANIM + x);
                SET_CMP(a, ANIM_HIT_SPARK);
                if (z) {
                    mff_cycles(0xEA3D, 1);
                    where = FAR;
                } else {
                    mff_cycles(0xEA3F, 7);
                    a = RAM(ROUND);
                    c = a >= 3;
                    if (a != 3) {
                        mff_cycles(0xEA43, 1);
                        where = NEAR;
                    } else {
                        mff_cycles(0xEA45, 7);
                        a = RAM(SUB_AREA);
                        SET_CMP(a, 1);
                        if (!z) {
                            mff_cycles(0xEA49, 1);
                            where = NEAR;
                        } else {
                            mff_cycles(0xEA4B, 2 + 1);
                            where = FAR;
                        }
                    }
                }
            }
        }
    } else {
        mff_cycles(0xEA03, 1 + 7);
        a = RAM(OBJ_X + x);
        RAM(TMP19) = a;
        where = ON_SCREEN;
    }

    if (where == FAR) {
        mff_cycles(0xEA30, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0A32, func_EC33, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xEA33, 6);
        SYNC_OUT;
        mff_rts();
        return;
    }
    if (where == NEAR) {
        mff_cycles(0xEA4D, 14);
        a = RAM(OBJ_FLAGS + x) & (uint8_t)~OBJF_ON_SCREEN;
        SET_NZ(a);
        RAM(OBJ_FLAGS + x) = a;
        SYNC_OUT;
        stage_jmp(0xEA6F, func_EA6F);
        return;
    }
    mff_cycles(0xEA5D, 22);
    RAM(TMP18) = RAM(OBJ_Y + x);
    a = RAM(OBJ_FLAGS + x) | OBJF_ON_SCREEN;
    RAM(OBJ_FLAGS + x) = a;
    a &= OBJF_HIDDEN;
    SET_NZ(a);
    if (a != 0) {
        mff_cycles(0xEA6E, 6);
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0xEA6C, 1);
    SYNC_OUT;
    func_EA6F();
}

/*
 * object_draw_sprites, $EA6F (proposed name): maps slot X's sprite banks,
 * steps its animation (obj_anim_timer counts to the script's period, then
 * obj_anim_frame advances and wraps after the last frame), and, if the
 * object is on the screen and not blinking out (obj_anim_timer bit 7 and
 * frame_counter bit 2: no route), writes its frame's sprites: y = $12 + dy,
 * x = $13 + dx (+ $F8 - dx facing right, attribute bit 6 flipped), priority
 * ORed in. A sprite whose y or x wraps, or whose y is $F0 or more, is
 * dropped and its entry reused (left hidden). Drawing stops when oam_index
 * wraps to 0. Frame id 0 ends the object: obj_state = 0 and object_free.
 * In: X = slot, $12/$13. Out: X = oam_index after the last sprite (or the
 * slot if nothing was drawn), A = the last dx or $F8.
 * Called by: object_draw (JMP at $EA55, falling in at $EA6F).
 */
void func_EA6F(void) {
    REGS_IN;

    mff_cycles(0xEA6F, 8);
    y = RAM(OBJ_ID + x);
    a = prg_read_ix(K_OBJ_SPRITE_BANK, y);
    mff_cycles(0xEA75, 19);
    RAM(BANK_REQUEST) = a;
    c = 0;
    ADC_A(1);
    RAM(BANK_REQUEST + 1) = a;
    RAM(TMP0) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0A80, func_FF4D, 0xE000)) return;
    SYNC_IN;

    mff_cycles(0xEA81, 32);
    x = RAM(TMP0);
    y = 0x00;
    a = RAM(OBJ_FLAGS + x);
    mff_push(a);
    RAM(TMP16) = a & OBJF_FACE_RIGHT;
    a = mff_pull();
    RAM(TMP17) = a & OBJF_BEHIND;
    a = RAM(OBJ_ANIM + x);
    SET_NZ(a);
    if (a == 0) {                               /* no animation */
        mff_cycles(0xEA95, 1 + 6);
        SYNC_OUT;
        mff_rts();
        return;
    }

    /* step the animation */
    mff_cycles(0xEA97, 8);
    a &= 0x7F;
    y = a;
    a = prg_read_ix(ANIM_SCRIPT_LO, y);
    mff_cycles(0xEA9D, 7);
    RAM(TMP0) = a;
    a = prg_read_ix(ANIM_SCRIPT_HI, y);
    mff_cycles(0xEAA2, 23);
    RAM(TMP1) = a;
    a = RAM(OBJ_ANIM_TIMER + x) & 0x7F;
    RAM(OBJ_ANIM_TIMER + x)++;
    y = 0x01;
    m = mem_read_iy(TMP0, y);          /* the period */
    c = a >= m;
    mff_cycles(0xEAB0, 2);
    if (a == m) {
        mff_cycles(0xEAB2, 31);
        RAM(OBJ_ANIM_TIMER + x) &= 0x80;
        a = RAM(OBJ_ANIM_FRAME + x) & 0x7F;
        RAM(OBJ_ANIM_FRAME + x)++;
        y--;
        m = mem_read_iy(TMP0, y);      /* the last frame */
        c = a >= m;
        mff_cycles(0xEAC5, 2);
        if (a == m) {
            mff_cycles(0xEAC7, 7);
            a = 0x00;
            RAM(OBJ_ANIM_FRAME + x) = a;
        } else {
            mff_cycles(0xEAC5, 1);
        }
    } else {
        mff_cycles(0xEAB0, 1);
    }

    /* drawn only on the screen, and not on a blink's off frames */
    mff_cycles(0xEACC, 6);
    a = RAM(OBJ_FLAGS + x);
    SET_NZ(a);
    if (!n) {
        mff_cycles(0xEACF, 1 + 6);
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0xEAD1, 6);
    a = RAM(OBJ_ANIM_TIMER + x);
    if (a & 0x80) {                             /* no route */
        mff_cycles(0xEAD6, 7);
        a = RAM(FRAME_COUNTER) & 0x04;
        SET_NZ(a);
        if (a != 0) {
            mff_cycles(0xEADA, 1 + 6);
            SYNC_OUT;
            mff_rts();
            return;
        }
    } else {
        mff_cycles(0xEAD4, 1);
    }

    /* the current frame id; 0 ends the object */
    mff_cycles(0xEADC, 17);
    a = RAM(OBJ_ANIM_FRAME + x) & 0x7F;
    c = 0;
    ADC_A(2);
    y = a;
    a = mem_read_iy(TMP0, y);
    SET_NZ(a);
    mff_cycles(0xEAE7, 2);
    if (a == 0) {
        mff_cycles(0xEAE9, 8);
        RAM(OBJ_STATE + x) = a;
        SYNC_OUT;
        stage_jmp(0xEC81, func_EC81);
        return;
    }

    /* the frame: its count, and its layout's bank at $A000 */
    mff_cycles(0xEAE7, 1 + 6);
    y = a;
    a = prg_read_ix(FRAME_LO, y);
    mff_cycles(0xEAF4, 7);
    RAM(TMP2) = a;
    a = prg_read_ix(FRAME_HI, y);
    mff_cycles(0xEAF9, 10);
    RAM(TMP3) = a;
    y = 0x00;
    a = mem_read_iy(TMP2, y);
    mff_cycles(0xEAFF, 11);
    mff_push(a);
    y = 0x0B;
    a = mff_pull();
    if (a & 0x80) {
        mff_cycles(0xEB05, 4);
        a &= 0x7F;
        y = 0x03;
    } else {
        mff_cycles(0xEB03, 1);
    }
    mff_cycles(0xEB09, 8);
    RAM(TMP4) = a;
    SET_CMP(y, RAM(BANK_REQUEST + 1));
    if (!z) {
        mff_cycles(0xEB0F, 12);
        RAM(BANK_REQUEST + 1) = y;
        RAM(TMP5) = x;
        SYNC_OUT;
        if (!mff_call_fn(0x0B15, func_FF4D, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xEB16, 3 + 7);              /* LDX $05: dead, TAX below */
    } else {
        mff_cycles(0xEB0D, 1 + 7);
    }

    /* $05/$06 = the layout - 2, so ($05),Y pairs with ($02),Y */
    y = 0x01;
    a = mem_read_iy(TMP2, y);
    mff_cycles(0xEB1C, 6);
    x = a;
    a = prg_read_ix(LAYOUT_LO, x);
    mff_cycles(0xEB20, 11);
    c = 1;
    SBC_A(2);
    RAM(TMP5) = a;
    a = prg_read_ix(LAYOUT_HI, x);
    mff_cycles(0xEB28, 10);
    SBC_A(0);
    RAM(TMP6) = a;
    x = RAM(OAM_INDEX);
    SET_NZ(x);
    if (x == 0) {                               /* the buffer is full */
        mff_cycles(0xEB2E, 1 + 6);
        SYNC_OUT;
        mff_rts();
        return;
    }

    /* the sprites, one charge per instruction (a 1 is a taken branch's extra
     * cycle): a lag frame's NMI lands in here more than anywhere, and the
     * OAM DMA then copies the buffer as far as the console's had written
     * it (docs/timing.md) */
    for (;;) {
        int visible;

        /* the tile, and y = $12 + dy */
        mff_cycles(0xEB30, 2);
        mff_cycles(0xEB32, 3);
        RAM(TMP0) = SPRITE_Y_LIMIT;
        mff_cycles(0xEB34, 2);
        y++;
        mff_cycles(0xEB35, 5);
        a = mem_read_iy(TMP2, y);
        mff_cycles(0xEB37, 5);
        RAM(OAM_BUFFER + 1 + x) = a;
        mff_cycles(0xEB3A, 3);
        a = RAM(TMP18);
        mff_cycles(0xEB3C, 2);
        c = 0;
        mff_cycles(0xEB3D, 5);
        ADC_A(mem_read_iy(TMP5, y));
        mff_cycles(0xEB3F, 5);
        RAM(OAM_BUFFER + x) = a;
        mff_cycles(0xEB42, 5);
        a = mem_read_iy(TMP5, y);
        mff_cycles(0xEB44, 2);                  /* BMI $EB4A */
        if (a & 0x80) {                         /* dy < 0: no carry means it wrapped */
            mff_cycles(0xEB44, 1);
            mff_cycles(0xEB4A, 2);              /* BCC $EB8B */
            visible = c;
            if (!visible) mff_cycles(0xEB4A, 1);
        } else {                                /* dy >= 0: a carry means it wrapped */
            mff_cycles(0xEB46, 2);              /* BCC $EB4C */
            visible = !c;
            if (visible) {
                mff_cycles(0xEB46, 1);
            } else {
                mff_cycles(0xEB48, 2);          /* BCS $EB8B */
                mff_cycles(0xEB48, 1);
            }
        }
        if (visible) {
            mff_cycles(0xEB4C, 4);
            mff_cycles(0xEB4F, 3);
            c = RAM(OAM_BUFFER + x) >= RAM(TMP0);
            mff_cycles(0xEB51, 2);              /* BCS $EB8B */
            if (c) {
                mff_cycles(0xEB51, 1);
                visible = 0;
            }
        }

        if (!visible) {
            mff_cycles(0xEB8B, 2);
            y++;                                /* skip the attribute */
        } else {
            /* the attribute, and x = $13 + dx */
            mff_cycles(0xEB53, 2);
            y++;
            mff_cycles(0xEB54, 5);
            a = mem_read_iy(TMP2, y);
            mff_cycles(0xEB56, 3);
            a ^= RAM(TMP16);
            mff_cycles(0xEB58, 3);
            a |= RAM(TMP17);
            mff_cycles(0xEB5A, 5);
            RAM(OAM_BUFFER + 2 + x) = a;
            mff_cycles(0xEB5D, 5);
            a = mem_read_iy(TMP5, y);
            mff_cycles(0xEB5F, 3);
            RAM(TMP1) = a;
            mff_cycles(0xEB61, 3);
            mff_cycles(0xEB63, 2);              /* BEQ $EB6C */
            if (RAM(TMP16) != 0) {              /* facing right: mirrored */
                mff_cycles(0xEB65, 2);
                mff_cycles(0xEB67, 2);
                mff_cycles(0xEB68, 3);
                mff_cycles(0xEB6A, 3);
                RAM(TMP1) = (uint8_t)(0xF8 - RAM(TMP1));
            } else {
                mff_cycles(0xEB63, 1);
            }
            mff_cycles(0xEB6C, 3);
            a = RAM(TMP19);
            mff_cycles(0xEB6E, 2);
            c = 0;
            mff_cycles(0xEB6F, 3);
            ADC_A(RAM(TMP1));
            mff_cycles(0xEB71, 5);
            RAM(OAM_BUFFER + 3 + x) = a;
            mff_cycles(0xEB74, 3);
            a = RAM(TMP1);
            mff_cycles(0xEB76, 2);              /* BMI $EB7C */
            if (a & 0x80) {
                mff_cycles(0xEB76, 1);
                mff_cycles(0xEB7C, 2);          /* BCC $EB8C */
                visible = c;
                if (!visible) mff_cycles(0xEB7C, 1);
            } else {
                mff_cycles(0xEB78, 2);          /* BCC $EB7E */
                visible = !c;
                if (visible) {
                    mff_cycles(0xEB78, 1);
                } else {
                    mff_cycles(0xEB7A, 2);      /* BCS $EB8C */
                    mff_cycles(0xEB7A, 1);
                }
            }
            if (visible) {
                /* keep it: on to the next entry */
                mff_cycles(0xEB7E, 2);
                mff_cycles(0xEB7F, 2);
                mff_cycles(0xEB80, 2);
                mff_cycles(0xEB81, 2);
                x = (uint8_t)(x + 4);
                SET_NZ(x);
                mff_cycles(0xEB82, 3);
                RAM(OAM_INDEX) = x;
                mff_cycles(0xEB84, 2);          /* BEQ $EB8A */
                if (x == 0) {                   /* the buffer is full */
                    mff_cycles(0xEB84, 1);
                    mff_cycles(0xEB8A, 6);
                    SYNC_OUT;
                    mff_rts();
                    return;
                }
            }
        }
        if (!visible) {
            /* hide the entry; the next sprite reuses it (BNE always taken) */
            mff_cycles(0xEB8C, 2);
            a = SPRITE_HIDDEN;
            mff_cycles(0xEB8E, 5);
            RAM(OAM_BUFFER + x) = a;
            mff_cycles(0xEB91, 2);
            mff_cycles(0xEB91, 1);
        }

        mff_cycles(0xEB86, 5);
        t = (uint8_t)(RAM(TMP4) - 1);
        RAM(TMP4) = t;
        SET_NZ(t);
        mff_cycles(0xEB88, 2);                  /* BPL $EB30 */
        if (n) break;
        mff_cycles(0xEB88, 1);
    }
    mff_cycles(0xEB8A, 6);
    SYNC_OUT;
    mff_rts();
}

/*
 * object_offscreen, $EC33: slot X is far off the camera's screen. In round
 * 4 area 2 (round 3, sub_area 1; no route) and for the ids in
 * K_OFFSCREEN_FREE it is freed (object_free). Anything else is moved 16
 * pixels toward the camera, so an enemy cannot be lost off the screen.
 * In: X = slot. Out: carry set when moved, clear when freed; Y = the index
 * of the id's entry in the list (freed) or of the $FF.
 * Called by: object_draw, JSR at $EA30.
 */
void func_EC33(void) {
    REGS_IN;

    mff_cycles(0xEC33, 7);
    a = RAM(ROUND);
    if (a == 3) {
        mff_cycles(0xEC39, 7);
        a = RAM(SUB_AREA);
        z = a == 1;
        if (z) {
            mff_cycles(0xEC3F, 2 + 1);
            SYNC_OUT;
            func_EC81();
            return;
        }
        mff_cycles(0xEC3D, 1);
    } else {
        mff_cycles(0xEC37, 1);
    }

    mff_cycles(0xEC41, 2);
    y = 0x00;
    for (;;) {
        mff_cycles(0xEC43, 4);
        a = prg_read_ix(K_OFFSCREEN_FREE, y);
        mff_cycles(0xEC46, 6);
        if (a == RAM(OBJ_ID + x)) {
            mff_cycles(0xEC49, 1);
            SYNC_OUT;
            func_EC81();
            return;
        }
        mff_cycles(0xEC4B, 4);
        c = a >= 0xFF;
        if (a == 0xFF) break;
        mff_cycles(0xEC4F, 4);
        y++;
        SET_NZ(y);
        if (y == 0) {                           /* no route: the list ends first */
            mff_cycles(0xEC52, 6);
            SYNC_OUT;
            mff_rts();
            return;
        }
        mff_cycles(0xEC51, 1);
    }

    /* pull it 16 pixels toward the camera's page */
    mff_cycles(0xEC4D, 1 + 11);
    a = RAM(SCROLL_X_HI);
    c = 1;
    SBC_A(RAM(OBJ_X_HI + x));
    if (!n) {                                   /* left of the camera */
        mff_cycles(0xEC5B, 32);
        a = RAM(OBJ_X + x);
        c = 0;
        ADC_A(0x10);
        RAM(OBJ_X + x) = a;
        a = RAM(OBJ_X_HI + x);
        ADC_A(0);
        RAM(OBJ_X_HI + x) = a;
    } else {                                    /* right of it */
        mff_cycles(0xEC59, 1 + 32);
        a = RAM(OBJ_X + x);
        c = 1;
        SBC_A(0x10);
        RAM(OBJ_X + x) = a;
        a = RAM(OBJ_X_HI + x);
        SBC_A(0);
        RAM(OBJ_X_HI + x) = a;
    }
    c = 1;
    SYNC_OUT;
    mff_rts();
}

/*
 * object_free, $EC81: a shorter object_clear for slot X (k_free_fields),
 * obj_spawn_index $FF. Out: A = $FF, carry clear.
 * Called by: object_draw_sprites (JMP at $EAEC: the animation ended) and
 * object_offscreen (BEQ at $EC3F and $EC49).
 */
void func_EC81(void) {
    REGS_IN;
    unsigned i;

    /* LDA #0, 19 x STA abs,X, LDA #$FF, STA abs,X, CLC, RTS */
    mff_cycles(0xEC81, 2 + 19 * 5 + 2 + 5 + 2 + 6);
    for (i = 0; i < sizeof k_free_fields / sizeof k_free_fields[0]; i++)
        RAM(k_free_fields[i] + x) = 0;
    a = 0xFF;
    SET_NZ(a);
    RAM(OBJ_SPAWN_INDEX + x) = a;
    c = 0;
    SYNC_OUT;
    mff_rts();
}
