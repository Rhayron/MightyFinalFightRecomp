/*
 * stage_events.c - what happens on each screen of an area, bank $08 (16 KiB
 * bank 4 at $8000)
 *
 * stage_events runs once per gameplay frame (gameplay_update, $C966).
 * While the camera is locked on a screen, the area's script runs: one
 * routine per area (area_index $67 = round * 3 + sub_area, through
 * area_script $89D7), which picks the screen's routine by scroll_x_hi.
 * A screen routine compares area_kills ($54) with the screen's quota
 * (a table per area, indexed by the screen) and, when it is met, marks the
 * area cleared (area_clear $3F = $FF, area_set_clear); before that it
 * keeps the timed spawner (stage_spawn.c) going. Some wait for every enemy
 * to be gone (enemies_left) and some hold knocked-back enemies inside the
 * screen (keep_on_screen $BC).
 * Once the area is cleared and no enemy is left, stage_events puts up the
 * GO sign (id $6F, area_clear $7F); on the area's last screen it walks the
 * player out instead (game_action $19). When the screen-advance object
 * sets screen_event ($B5), the screen's setup runs once (screen_setup
 * $972A, the same two-level dispatch), loading the sprite set its enemies
 * need (bank 09 $A000, stage_palette.c).
 *
 * The area and screen routines all come in a few shapes; each shape is one
 * helper below, and the routine of each screen is a call to it with the
 * routine's own address (the cycle charges and return addresses are
 * relative to it) and its table.
 */
#include "stage.h"

void func_F2A0(void);   /* obj_spawn: slot Y, animation A */
void func_F71A(void);   /* find_free_slot: Y, carry set if none */

/* a JSR's return address (low 13 bits, as mff_call_at takes it) */
#define RP(addr) ((uint16_t)((addr) & 0x1FFF))

/* the two-level dispatch of area_script and screen_setup: a table by round
 * of lists by sub_area, giving the area's routine */
#define K_AREA_SCRIPT_ROUND_LO   0x89F5
#define K_AREA_SCRIPT_ROUND_HI   0x89FA
#define K_AREA_SCRIPT_LO         0x8A0E
#define K_AREA_SCRIPT_HI         0x8A1D
#define K_SCREEN_SETUP_ROUND_LO  0x9748
#define K_SCREEN_SETUP_ROUND_HI  0x974D
#define K_SCREEN_SETUP_LO        0x9761
#define K_SCREEN_SETUP_HI        0x9770

/* the GO sign */
#define ANIM_GO_SIGN    0x6D
#define ID_GO_SIGN      0x6F
#define GO_SIGN_CHR     0x23    /* sprite tiles $80-$BF (CHR R4) */
#define GO_SIGN_SET     0x06    /* its palette: sprite set 3's (bank 09 $A013) */
#define CLEAR_GO_UP     0x7F

#define ID_BELGER       0x74

/* animations from $80 up that enemies_two does not count ($96D5-$96E7) */
static const uint8_t k_not_counted[] = { 0xBB, 0xB9, 0xBA, 0xC7, 0xC8 };

/* the fields the GO sign gets cleared, in order ($898F-$89BA) */
static const uint16_t k_go_sign_cleared[] = {
    OBJ_LIFE, OBJ_LIFE_MAX, OBJ_THROW_FLAG, OBJ_HIT_COOLDOWN, OBJ_INVULN, OBJ_ATTACK_DAMAGE,
    OBJ_ATTACK, OBJ_HIT_PLAYER, OBJ_HIT_TIMER, OBJ_HELD, OBJ_KIND, OBJ_TIER, OBJ_AI_TIMER,
    OBJ_AI_COUNT, OBJ_AI_FLAG,
};

/* ---- the shapes ---- */

/* pc: RTS */
static void script_none(uint16_t pc) {
    mff_cycles(pc, 6);
    mff_rts();
}

/* pc: LDY scroll_x_hi / LDA lo,Y / STA $06 / LDA hi,Y / STA $07 / JMP ($0006):
 * the area's routine for the screen */
static void screen_dispatch(uint16_t pc, uint16_t lo, uint16_t hi) {
    REGS_IN;

    mff_cycles(pc, 7);
    y = RAM(SCROLL_X_HI);
    a = prg_read_ix(lo, y);
    mff_cycles((uint16_t)(pc + 0x05), 7);
    RAM(TMP6) = a;
    a = prg_read_ix(hi, y);
    SET_NZ(a);
    mff_cycles((uint16_t)(pc + 0x0A), 8);
    RAM(TMP7) = a;
    SYNC_OUT;
    {
        uint16_t jt = nes_read16zp(TMP6);
        nes_cpu_instruction_boundary(jt, 2);
        call_by_address_tail(jt, -1);
    }
}

/* pc: area_kills against quota[Y] (the screen): met, area_set_clear;
 * else the timed spawner */
static void quota_or_spawn(uint16_t pc, uint16_t quota) {
    REGS_IN;

    mff_cycles(pc, 7);
    a = RAM(AREA_KILLS);
    m = prg_read_ix(quota, y);
    SET_CMP(a, m);
    mff_cycles((uint16_t)(pc + 0x05), 2);
    if (c) {
        mff_cycles((uint16_t)(pc + 0x07), 6);
        SYNC_OUT;
        if (!mff_call_at(RP(pc + 0x09), 0x89D2, 4)) return;
        SYNC_IN;
        mff_cycles((uint16_t)(pc + 0x0A), 6);
    } else {
        mff_cycles((uint16_t)(pc + 0x05), 1 + 6);
        SYNC_OUT;
        if (!mff_call_at(RP(pc + 0x0D), 0x9000, 4)) return;
        SYNC_IN;
        mff_cycles((uint16_t)(pc + 0x0E), 6);
    }
    SYNC_OUT;
    mff_rts();
}

/* pc: met, and no enemy left (enemies_left), area_set_clear */
static void quota_when_empty(uint16_t pc, uint16_t quota) {
    REGS_IN;

    mff_cycles(pc, 7);
    a = RAM(AREA_KILLS);
    m = prg_read_ix(quota, y);
    SET_CMP(a, m);
    mff_cycles((uint16_t)(pc + 0x05), 2);
    if (c) {
        mff_cycles((uint16_t)(pc + 0x07), 6);
        SYNC_OUT;
        if (!mff_call_at(RP(pc + 0x09), 0x96FC, 4)) return;
        SYNC_IN;
        mff_cycles((uint16_t)(pc + 0x0A), 2);
        if (z) {
            mff_cycles((uint16_t)(pc + 0x0C), 6);
            SYNC_OUT;
            if (!mff_call_at(RP(pc + 0x0E), 0x89D2, 4)) return;
            SYNC_IN;
        } else {
            mff_cycles((uint16_t)(pc + 0x0A), 1);
        }
    } else {
        mff_cycles((uint16_t)(pc + 0x05), 1);
    }
    mff_cycles((uint16_t)(pc + 0x0F), 6);
    SYNC_OUT;
    mff_rts();
}

/* pc: met, area_set_clear once no enemy is left; not met, the timed spawner */
static void quota_when_empty_or_spawn(uint16_t pc, uint16_t quota) {
    REGS_IN;

    mff_cycles(pc, 7);
    a = RAM(AREA_KILLS);
    m = prg_read_ix(quota, y);
    SET_CMP(a, m);
    mff_cycles((uint16_t)(pc + 0x05), 2);
    if (c) {
        mff_cycles((uint16_t)(pc + 0x07), 6);
        SYNC_OUT;
        if (!mff_call_at(RP(pc + 0x09), 0x96FC, 4)) return;
        SYNC_IN;
        mff_cycles((uint16_t)(pc + 0x0A), 2);
        if (z) {
            mff_cycles((uint16_t)(pc + 0x0C), 6);
            SYNC_OUT;
            if (!mff_call_at(RP(pc + 0x0E), 0x89D2, 4)) return;
            SYNC_IN;
            mff_cycles((uint16_t)(pc + 0x0F), 6);
            SYNC_OUT;
            mff_rts();
            return;
        }
        mff_cycles((uint16_t)(pc + 0x0A), 1);
    } else {
        mff_cycles((uint16_t)(pc + 0x05), 1 + 6);
        SYNC_OUT;
        if (!mff_call_at(RP(pc + 0x12), 0x9000, 4)) return;
        SYNC_IN;
    }
    mff_cycles((uint16_t)(pc + 0x13), 6);
    SYNC_OUT;
    mff_rts();
}

/* pc: LDA #$FF / STA keep_on_screen, then the shape at pc + 4 */
static void keep_quota_when_empty(uint16_t pc, uint16_t quota) {
    mff_cycles(pc, 5);
    RAM(KEEP_ON_SCREEN) = 0xFF;
    quota_when_empty((uint16_t)(pc + 4), quota);
}

static void keep_quota_when_empty_or_spawn(uint16_t pc, uint16_t quota) {
    mff_cycles(pc, 5);
    RAM(KEEP_ON_SCREEN) = 0xFF;
    quota_when_empty_or_spawn((uint16_t)(pc + 4), quota);
}

/* pc: LDA table,Y / JSR $A000 / RTS: the screen's sprite set (Y = the
 * screen, from the dispatch) */
static void sprite_set(uint16_t pc, uint16_t table) {
    REGS_IN;

    mff_cycles(pc, 4);
    a = prg_read_ix(table, y);
    SET_NZ(a);
    mff_cycles((uint16_t)(pc + 0x03), 6);
    SYNC_OUT;
    if (!mff_call_at(RP(pc + 0x05), 0xA000, 4)) return;
    SYNC_IN;
    mff_cycles((uint16_t)(pc + 0x06), 6);
    SYNC_OUT;
    mff_rts();
}

/* pc: LDY scroll_x_hi, then sprite_set */
static void sprite_set_by_screen(uint16_t pc, uint16_t table) {
    mff_cycles(pc, 3);
    g_cpu.Y = RAM(SCROLL_X_HI);
    sprite_set((uint16_t)(pc + 2), table);
}

/* pc: LDY round / LDA round_lo,Y / STA $02 / LDA round_hi,Y / STA $03 /
 * LDY sub_area / LDA ($02),Y / TAY / LDA lo,Y / STA $04 / LDA hi,Y / STA $05
 * / JMP ($0004): the area's routine */
static void area_dispatch(uint16_t pc, uint16_t round_lo, uint16_t round_hi,
                          uint16_t lo, uint16_t hi) {
    REGS_IN;

    mff_cycles(pc, 7);
    y = RAM(ROUND);
    a = prg_read_ix(round_lo, y);
    mff_cycles((uint16_t)(pc + 0x05), 7);
    RAM(TMP2) = a;
    a = prg_read_ix(round_hi, y);
    mff_cycles((uint16_t)(pc + 0x0A), 11);
    RAM(TMP3) = a;
    y = RAM(SUB_AREA);
    a = mem_read_iy(TMP2, y);
    mff_cycles((uint16_t)(pc + 0x10), 6);
    y = a;
    a = prg_read_ix(lo, y);
    mff_cycles((uint16_t)(pc + 0x14), 7);
    RAM(TMP4) = a;
    a = prg_read_ix(hi, y);
    SET_NZ(a);
    mff_cycles((uint16_t)(pc + 0x19), 8);
    RAM(TMP5) = a;
    SYNC_OUT;
    {
        uint16_t jt = nes_read16zp(TMP4);
        nes_cpu_instruction_boundary(jt, 2);
        call_by_address_tail(jt, -1);
    }
}

/* ---- stage_events ---- */

/* $8938: the area's script */
static void events_run_script(void) {
    mff_cycles(0x8938, 6);
    if (!mff_call_at(0x093A, 0x89D7, 4)) return;
    mff_cycles(0x893B, 6);
    mff_rts();
}

/*
 * events_screen_event, $892C (bank 4; name proposed): the screen's setup
 * (screen_setup, $972A) when screen_event is set, which it then clears.
 * Called by: stage_events (falling in, BNE at $892A), events_area_cleared
 * when no slot is free for the GO sign (BCS at $896C).
 */
void func_892C_b4(void) {
    REGS_IN;

    mff_cycles(0x892C, 5);
    a = RAM(SCREEN_EVENT);
    SET_NZ(a);
    if (a != 0) {
        mff_cycles(0x8930, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0932, 0x972A, 4)) return;
        SYNC_IN;
        mff_cycles(0x8933, 5);
        a = 0x00;
        SET_NZ(a);
        RAM(SCREEN_EVENT) = a;
    } else {
        mff_cycles(0x892E, 1);
    }
    mff_cycles(0x8937, 6);
    SYNC_OUT;
    mff_rts();
}

/*
 * $8955: the RTS of events_walk_out and events_area_cleared (BPL at $8958
 * and the branches before it).
 */
void func_8955_b4(void) {
    mff_cycles(0x8955, 6);
    mff_rts();
}

/*
 * events_walk_out, $893C (bank 4; name proposed): the cleared area's last
 * screen: unless he is already walking out (game_action $19), the player
 * walks out as soon as he stands or punches (game_action 0 or 3): slot 0
 * live, game_action $19.
 * Called by: events_area_cleared (BEQ at $895E).
 */
void func_893C_b4(void) {
    REGS_IN;

    mff_cycles(0x893C, 7);
    a = RAM(GAME_ACTION);
    SET_CMP(a, GA_AREA_EXIT);
    if (z) {
        mff_cycles(0x8940, 1);
        SYNC_OUT;
        func_8955_b4();
        return;
    }
    mff_cycles(0x8942, 5);
    a = RAM(GAME_ACTION);
    if (a != GA_IDLE) {
        mff_cycles(0x8946, 4);
        SET_CMP(a, 0x03);                       /* punching */
        if (!z) {
            mff_cycles(0x894A, 2 + 1);          /* BNE always taken here */
            SYNC_OUT;
            func_8955_b4();
            return;
        }
        mff_cycles(0x8948, 1);
    } else {
        mff_cycles(0x8944, 1);
    }
    mff_cycles(0x894C, 11);
    RAM(OBJ_STATE) = 0x80;
    a = GA_AREA_EXIT;
    SET_NZ(a);
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    func_8955_b4();
}

/*
 * events_area_cleared, $8956 (bank 4; name proposed): the area is cleared
 * (area_clear $FF; $7F, the GO sign is up, does nothing). On the area's
 * last screen events_walk_out. Elsewhere, when the player is idle and no
 * enemy is left, the GO sign comes up in a free slot (none free: the
 * screen event check instead).
 * Called by: stage_events (BNE at $8926; JMP at $8921, no route).
 */
void func_8956_b4(void) {
    REGS_IN;
    unsigned i;

    mff_cycles(0x8956, 5);
    a = RAM(AREA_CLEAR);
    SET_NZ(a);
    if (!n) {
        mff_cycles(0x8958, 1);
        SYNC_OUT;
        func_8955_b4();
        return;
    }
    mff_cycles(0x895A, 8);
    a = RAM(AREA_SCREEN);
    c = a >= RAM(AREA_SCREEN_COUNT);
    if (a == RAM(AREA_SCREEN_COUNT)) {
        mff_cycles(0x895E, 1);
        SYNC_OUT;
        func_893C_b4();                         /* the last screen: walk out */
        return;
    }

    /* the GO sign, once the player is idle and the enemies are gone */
    mff_cycles(0x8960, 5);
    a = RAM(GAME_ACTION);
    SET_NZ(a);
    if (a != GA_IDLE) {
        mff_cycles(0x8962, 1 + 6);
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0x8964, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0966, 0x96FC, 4)) return;
    SYNC_IN;
    mff_cycles(0x8967, 2);
    if (!z) {
        mff_cycles(0x8967, 1 + 6);
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0x8969, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x096B, func_F71A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x896C, 2);
    if (c) {
        mff_cycles(0x896C, 1);
        SYNC_OUT;
        func_892C_b4();                     /* events_screen_event */
        return;
    }
    mff_cycles(0x896E, 25);
    RAM(OBJ_X_HI + y) = RAM(SCROLL_X_HI);
    RAM(OBJ_Y_HI + y) = RAM(OBJ_Y_HI);
    a = ANIM_GO_SIGN;
    SET_NZ(a);
    SYNC_OUT;
    if (!mff_call_fn(0x097D, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x897E, 123);
    RAM(OBJ_FLAGS + y) = OBJF_ON_SCREEN | OBJF_WORLD;
    RAM(CHR_BANKS + 4) = GO_SIGN_CHR;
    RAM(OBJ_ID + y) = ID_GO_SIGN;
    RAM(OBJ_SAVED_ID + y) = ID_GO_SIGN;
    for (i = 0; i < sizeof k_go_sign_cleared / sizeof k_go_sign_cleared[0]; i++)
        RAM(k_go_sign_cleared[i] + y) = 0x00;
    RAM(OBJ_Y + y) = 0x40;
    RAM(OBJ_X + y) = 0xC0;
    a = GO_SIGN_SET;
    SET_NZ(a);
    SYNC_OUT;
    if (!mff_call_at(0x09CC, 0xA013, 4)) return;
    SYNC_IN;
    mff_cycles(0x89CD, 11);
    a = CLEAR_GO_UP;
    SET_NZ(a);
    RAM(AREA_CLEAR) = a;
    SYNC_OUT;
    mff_rts();
}

/*
 * stage_events, $8900 (bank 4): the area's per-frame work. On a screen
 * past the first: a cleared area goes to events_area_cleared, a locked
 * camera runs the area's script, otherwise the screen event check. On the
 * first screen only two areas run their script: round 3 area 3 (round 2,
 * sub_area 2) and round 4 area 2 (round 3, sub_area 1), until cleared (the
 * JMP to $8956 for a cleared one has no route).
 * Called by: gameplay_update (JSR at $C966).
 */
void func_8900_b4(void) {
    REGS_IN;

    mff_cycles(0x8900, 5);
    a = RAM(SCROLL_X_HI);
    if (a == 0) {
        mff_cycles(0x8904, 7);
        a = RAM(ROUND);
        if (a == 2) {
            mff_cycles(0x8908, 1 + 7);
            a = RAM(SUB_AREA);
            SET_CMP(a, 2);
            if (!z) {
                mff_cycles(0x8915, 6);
                SYNC_OUT;
                mff_rts();
                return;
            }
            mff_cycles(0x8913, 1);
        } else {
            mff_cycles(0x890A, 4);
            SET_CMP(a, 3);
            if (!z) {
                mff_cycles(0x890E, 6);
                SYNC_OUT;
                mff_rts();
                return;
            }
            mff_cycles(0x890C, 1 + 7);
            a = RAM(SUB_AREA);
            SET_CMP(a, 1);
            if (!z) {
                mff_cycles(0x891C, 6);
                SYNC_OUT;
                mff_rts();
                return;
            }
            mff_cycles(0x891A, 1);
        }
        mff_cycles(0x891D, 5);
        a = RAM(AREA_CLEAR);
        SET_NZ(a);
        if (a != 0) {                           /* no route */
            mff_cycles(0x8921, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x8956, 2);
            call_by_address_tail(0x8956, 4);
            return;
        }
        mff_cycles(0x891F, 1);
        SYNC_OUT;
        events_run_script();
        return;
    }

    mff_cycles(0x8902, 1 + 5);
    a = RAM(AREA_CLEAR);
    if (a != 0) {
        mff_cycles(0x8926, 1);
        SYNC_OUT;
        func_8956_b4();                     /* events_area_cleared */
        return;
    }
    mff_cycles(0x8928, 5);
    a = RAM(CAMERA_LOCK);
    SET_NZ(a);
    if (a != 0) {
        mff_cycles(0x892A, 1);
        SYNC_OUT;
        events_run_script();
        return;
    }
    SYNC_OUT;
    func_892C_b4();                         /* events_screen_event */
}

/*
 * area_set_clear, $89D2 (bank 4; proposed name): area_clear = $FF, the
 * screen's quota is met. Called by: the screen routines below.
 */
void func_89D2_b4(void) {
    REGS_IN;

    mff_cycles(0x89D2, 11);
    a = 0xFF;
    SET_NZ(a);
    RAM(AREA_CLEAR) = a;
    SYNC_OUT;
    mff_rts();
}

/*
 * area_script, $89D7 (bank 4; proposed name): JMP to the area's script.
 * Called by: stage_events ($8938).
 */
void func_89D7_b4(void) {
    area_dispatch(0x89D7, K_AREA_SCRIPT_ROUND_LO, K_AREA_SCRIPT_ROUND_HI,
                  K_AREA_SCRIPT_LO, K_AREA_SCRIPT_HI);
}

/*
 * screen_setup, $972A (bank 4; proposed name): JMP to the area's screen
 * setup, which loads the screen's sprite set.
 * Called by: stage_events' screen event check ($8930).
 */
void func_972A_b4(void) {
    area_dispatch(0x972A, K_SCREEN_SETUP_ROUND_LO, K_SCREEN_SETUP_ROUND_HI,
                  K_SCREEN_SETUP_LO, K_SCREEN_SETUP_HI);
}

/*
 * enemies_two, $96C3 (bank 4; proposed name): A = $FF (Z clear) when two or
 * more live objects in slots $0D-3 show an animation of $80 or more (the
 * enemy sets) other than k_not_counted, else A = 0. Out: carry = two or
 * more; $00 = the count.
 * Called by: timed_spawner ($9016).
 */
void func_96C3_b4(void) {
    REGS_IN;
    unsigned i;

    mff_cycles(0x96C3, 7);
    a = 0x00;
    RAM(TMP0) = a;
    y = 0x0D;
    for (;;) {
        mff_cycles(0x96C9, 6);
        a = RAM(OBJ_STATE + y);
        if (!(a & 0x80)) {
            mff_cycles(0x96CC, 1);
        } else {
            mff_cycles(0x96CE, 8);
            a = RAM(OBJ_ANIM + y);
            if (a < 0x80) {
                mff_cycles(0x96D3, 1);
            } else {
                for (i = 0; i < sizeof k_not_counted; i++) {
                    mff_cycles((uint16_t)(0x96D5 + 4 * i), 4);
                    if (a == k_not_counted[i]) {
                        mff_cycles((uint16_t)(0x96D7 + 4 * i), 1);
                        break;
                    }
                }
                if (i == sizeof k_not_counted) {
                    mff_cycles(0x96E9, 5);
                    RAM(TMP0)++;
                }
            }
        }
        mff_cycles(0x96EB, 6);
        y--;
        if (y == 2) break;
        mff_cycles(0x96EF, 1);
    }
    mff_cycles(0x96F0, 7);
    a = RAM(TMP0);
    c = a >= 2;
    if (!c) {
        mff_cycles(0x96F6, 8);
        a = 0x00;
    } else {
        mff_cycles(0x96F4, 1 + 8);
        a = 0xFF;
    }
    SET_NZ(a);
    SYNC_OUT;
    mff_rts();
}

/*
 * enemies_left, $96FC (bank 4; proposed name): A = $FF (Z clear) if any
 * live object in slots $0D-3 has id $91 or $95 or an animation of $80 or
 * more, else A = 0. Out: carry set; $00 = the count.
 * Called by: the screen routines, events_area_cleared ($8964).
 */
void func_96FC_b4(void) {
    REGS_IN;

    mff_cycles(0x96FC, 7);
    a = 0x00;
    RAM(TMP0) = a;
    y = 0x0D;
    for (;;) {
        int counted = 0;

        mff_cycles(0x9702, 6);
        a = RAM(OBJ_STATE + y);
        if (!(a & 0x80)) {
            mff_cycles(0x9705, 1);
        } else {
            mff_cycles(0x9707, 8);
            a = RAM(OBJ_ID + y);
            if (a == 0x91) {
                mff_cycles(0x970C, 1);
                counted = 1;
            } else {
                mff_cycles(0x970E, 4);
                if (a == 0x95) {
                    mff_cycles(0x9710, 1);
                    counted = 1;
                } else {
                    mff_cycles(0x9712, 8);
                    a = RAM(OBJ_ANIM + y);
                    if (a < 0x80) mff_cycles(0x9717, 1);
                    else counted = 1;
                }
            }
            if (counted) {
                mff_cycles(0x9719, 5);
                RAM(TMP0)++;
            }
        }
        mff_cycles(0x971B, 6);
        y--;
        c = y >= 2;
        if (y == 2) break;
        mff_cycles(0x971F, 1);
    }
    mff_cycles(0x9720, 5);
    a = RAM(TMP0);
    if (a == 0) {
        mff_cycles(0x9724, 8);
        a = 0x00;
    } else {
        mff_cycles(0x9722, 1 + 8);
        a = 0xFF;
    }
    SET_NZ(a);
    SYNC_OUT;
    mff_rts();
}

/*
 * belger_room, $8DA8 (bank 4; proposed name): the last screen of round 5
 * area 3. keep_on_screen; Belger (id $74) is looked for in slots $0F down
 * to 4, and once his life is negative the area is cleared.
 * Called by: round 5 area 3's screen dispatch ($8D83, screen 2).
 */
void func_8DA8_b4(void) {
    REGS_IN;

    mff_cycles(0x8DA8, 7);
    a = 0xFF;
    RAM(KEEP_ON_SCREEN) = a;
    y = 0x0F;
    for (;;) {
        mff_cycles(0x8DAE, 8);
        a = RAM(OBJ_ID + y);
        c = a >= ID_BELGER;
        if (a == ID_BELGER) break;
        mff_cycles(0x8DB5, 6);
        y--;
        SET_CMP(y, 3);
        if (z) {                                /* not there */
            mff_cycles(0x8DBA, 6);
            SYNC_OUT;
            mff_rts();
            return;
        }
        mff_cycles(0x8DB9, 1);
    }
    mff_cycles(0x8DB3, 1 + 6);
    a = RAM(OBJ_LIFE + y);
    SET_NZ(a);
    if (n) {
        mff_cycles(0x8DC0, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0DC2, 0x89D2, 4)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x8DBE, 1);
    }
    mff_cycles(0x8DC3, 6);
    SYNC_OUT;
    mff_rts();
}

/* ---- the area scripts ($89D7 -> $8A0E/$8A1D by area_index) ---- */

/* area_index 0, round 1 area 1 */
void func_8A2C_b4(void) { screen_dispatch(0x8A2C, 0x8A3B, 0x8A3F); }  /* screens: $8A43, $8A44, $8A53, $8A62 */
void func_8A43_b4(void) { script_none(0x8A43); }  /* screen 0: nothing */
void func_8A44_b4(void) { quota_or_spawn(0x8A44, 0x8DC4); }  /* screen 1: quota met: clear; else the timed spawner */
void func_8A53_b4(void) { quota_or_spawn(0x8A53, 0x8DC4); }  /* screen 2: quota met: clear; else the timed spawner */
void func_8A62_b4(void) { quota_when_empty_or_spawn(0x8A62, 0x8DC4); }  /* screen 3: quota met: clear once no enemy is left; else the timed spawner */

/* area_index 1, round 1 area 2 */
void func_8A76_b4(void) { screen_dispatch(0x8A76, 0x8A85, 0x8A89); }  /* screens: $8A43, $8A8D, $8A9D, $8AAB */
void func_8A8D_b4(void) { quota_when_empty(0x8A8D, 0x8DC8); }  /* screen 1: quota met and no enemy left: clear */
void func_8A9D_b4(void) { quota_or_spawn(0x8A9D, 0x8DC8); }  /* screen 2: quota met: clear; else the timed spawner */
void func_8AAB_b4(void) { script_none(0x8AAB); }  /* screen 3: nothing */

/* area_index 2, round 1 area 3 */
void func_8AAC_b4(void) { screen_dispatch(0x8AAC, 0x8ABB, 0x8ABD); }  /* screens: $8A43, $8ABF */
void func_8ABF_b4(void) { keep_quota_when_empty(0x8ABF, 0x8DCC); }  /* screen 1: keep_on_screen; quota met and no enemy left: clear */

/* area_index 3, round 2 area 1 */
void func_8AD3_b4(void) { screen_dispatch(0x8AD3, 0x8AE2, 0x8AE6); }  /* screens: $8A43, $8AEA, $8AF9, $8B08 */
void func_8AEA_b4(void) { quota_or_spawn(0x8AEA, 0x8DCE); }  /* screen 1: quota met: clear; else the timed spawner */
void func_8AF9_b4(void) { quota_or_spawn(0x8AF9, 0x8DCE); }  /* screen 2: quota met: clear; else the timed spawner */
void func_8B08_b4(void) { quota_when_empty_or_spawn(0x8B08, 0x8DCE); }  /* screen 3: quota met: clear once no enemy is left; else the timed spawner */

/* area_index 4, round 2 area 2 */
void func_8B1C_b4(void) { screen_dispatch(0x8B1C, 0x8B2B, 0x8B30); }  /* screens: $8A43, $8B35, $8B44, $8B58, $8B68 */
void func_8B35_b4(void) { quota_or_spawn(0x8B35, 0x8DD2); }  /* screen 1: quota met: clear; else the timed spawner */
void func_8B44_b4(void) { quota_when_empty_or_spawn(0x8B44, 0x8DD2); }  /* screen 2: quota met: clear once no enemy is left; else the timed spawner */
void func_8B58_b4(void) { quota_when_empty(0x8B58, 0x8DD2); }  /* screen 3: quota met and no enemy left: clear */
void func_8B68_b4(void) { quota_when_empty_or_spawn(0x8B68, 0x8DD2); }  /* screen 4: quota met: clear once no enemy is left; else the timed spawner */

/* area_index 5, round 2 area 3 */
void func_8B7C_b4(void) { screen_dispatch(0x8B7C, 0x8B8B, 0x8B8D); }  /* screens: $8A43, $8B8F */
void func_8B8F_b4(void) { quota_when_empty(0x8B8F, 0x8DD7); }  /* screen 1: quota met and no enemy left: clear */

/* area_index 6, round 3 area 1 */
void func_8B9F_b4(void) { screen_dispatch(0x8B9F, 0x8BAE, 0x8BB2); }  /* screens: $8A43, $8BB6, $8BC5, $8BD4 */
void func_8BB6_b4(void) { quota_or_spawn(0x8BB6, 0x8DD9); }  /* screen 1: quota met: clear; else the timed spawner */
void func_8BC5_b4(void) { quota_or_spawn(0x8BC5, 0x8DD9); }  /* screen 2: quota met: clear; else the timed spawner */
void func_8BD4_b4(void) { quota_when_empty_or_spawn(0x8BD4, 0x8DD9); }  /* screen 3: quota met: clear once no enemy is left; else the timed spawner (no route) */

/* area_index 7, round 3 area 2 */
void func_8BE8_b4(void) { screen_dispatch(0x8BE8, 0x8BF7, 0x8BFB); }  /* screens: $8A43, $8BFF, $8C0E, $8C1E */
void func_8BFF_b4(void) { quota_or_spawn(0x8BFF, 0x8DDD); }  /* screen 1: quota met: clear; else the timed spawner */
void func_8C0E_b4(void) { quota_when_empty(0x8C0E, 0x8DDD); }  /* screen 2: quota met and no enemy left: clear */
void func_8C1E_b4(void) { quota_when_empty_or_spawn(0x8C1E, 0x8DDD); }  /* screen 3: quota met: clear once no enemy is left; else the timed spawner */

/* area_index 8, round 3 area 3 */
void func_8C32_b4(void) { screen_dispatch(0x8C32, 0x8C41, 0x8C42); }  /* screens: $8C43 */
void func_8C43_b4(void) { keep_quota_when_empty(0x8C43, 0x8DE1); }  /* screen 0: keep_on_screen; quota met and no enemy left: clear */

/* area_index 9, round 4 area 1 */
void func_8C57_b4(void) { screen_dispatch(0x8C57, 0x8C66, 0x8C6A); }  /* screens: $8A43, $8C6E, $8C7D, $8C8B */
void func_8C6E_b4(void) { quota_or_spawn(0x8C6E, 0x8DE2); }  /* screen 1: quota met: clear; else the timed spawner */
void func_8C7D_b4(void) { quota_or_spawn(0x8C7D, 0x8DE2); }  /* screen 2: quota met: clear; else the timed spawner */
void func_8C8B_b4(void) { script_none(0x8C8B); }  /* screen 3: nothing */

/* area_index 10, round 4 area 2 */
void func_8C8C_b4(void) { screen_dispatch(0x8C8C, 0x8C9B, 0x8CA0); }  /* screens: $8CA5, $8CA5, $8CA5, $8CA5, $8CA5 */
void func_8CA5_b4(void) { keep_quota_when_empty_or_spawn(0x8CA5, 0x8DE6); }  /* screen 0: keep_on_screen; quota met: clear once no enemy is left; else the timed spawner */

/* area_index 11, round 4 area 3 */
void func_8CBD_b4(void) { screen_dispatch(0x8CBD, 0x8CCC, 0x8CCF); }  /* screens: $8A43, $8CD2, $8CE1 */
void func_8CD2_b4(void) { quota_or_spawn(0x8CD2, 0x8DEB); }  /* screen 1: quota met: clear; else the timed spawner */
void func_8CE1_b4(void) { keep_quota_when_empty(0x8CE1, 0x8DEB); }  /* screen 2: keep_on_screen; quota met and no enemy left: clear */

/* area_index 12, round 5 area 1 */
void func_8CF5_b4(void) { screen_dispatch(0x8CF5, 0x8D04, 0x8D08); }  /* screens: $8A43, $8D0C, $8D1B, $8D2A */
void func_8D0C_b4(void) { quota_or_spawn(0x8D0C, 0x8DEE); }  /* screen 1: quota met: clear; else the timed spawner */
void func_8D1B_b4(void) { quota_or_spawn(0x8D1B, 0x8DEE); }  /* screen 2: quota met: clear; else the timed spawner */
void func_8D2A_b4(void) { quota_when_empty(0x8D2A, 0x8DEE); }  /* screen 3: quota met and no enemy left: clear */

/* area_index 13, round 5 area 2 */
void func_8D3A_b4(void) { screen_dispatch(0x8D3A, 0x8D49, 0x8D4D); }  /* screens: $8A43, $8D51, $8D60, $8D6F */
void func_8D51_b4(void) { quota_or_spawn(0x8D51, 0x8DF2); }  /* screen 1: quota met: clear; else the timed spawner */
void func_8D60_b4(void) { quota_or_spawn(0x8D60, 0x8DF2); }  /* screen 2: quota met: clear; else the timed spawner */
void func_8D6F_b4(void) { keep_quota_when_empty(0x8D6F, 0x8DF2); }  /* screen 3: keep_on_screen; quota met and no enemy left: clear */

/* area_index 14, round 5 area 3 */
void func_8D83_b4(void) { screen_dispatch(0x8D83, 0x8D92, 0x8D95); }  /* screens: $8A43, $8D98, $8DA8 */
void func_8D98_b4(void) { quota_when_empty(0x8D98, 0x8DF6); }  /* screen 1: quota met and no enemy left: clear */
/* screen 2: $8DA8, written by hand below */

/* ---- the screen setups ($972A -> $9761/$9770 by area_index) ---- */

/* area_index 0, round 1 area 1 */
void func_977F_b4(void) { sprite_set_by_screen(0x977F, 0x99C2); }  /* every screen */

/* area_index 1, round 1 area 2 */
void func_9788_b4(void) { screen_dispatch(0x9788, 0x9797, 0x979B); }  /* screens: $9787, $979F, $97A6, $97AD */
void func_9787_b4(void) { script_none(0x9787); }  /* screen 0: nothing */
void func_979F_b4(void) { sprite_set(0x979F, 0x99C6); }  /* screen 1 */
void func_97A6_b4(void) { sprite_set(0x97A6, 0x99C6); }  /* screen 2 */
void func_97AD_b4(void) { sprite_set(0x97AD, 0x99C6); }  /* screen 3 (no route reaches it) */

/* area_index 2, round 1 area 3 */
void func_97B4_b4(void) { screen_dispatch(0x97B4, 0x97C3, 0x97C5); }  /* screens: $9787, $97C7 */
void func_97C7_b4(void) { sprite_set(0x97C7, 0x99CA); }  /* screen 1 */

/* area_index 3, round 2 area 1 */
void func_97CE_b4(void) { screen_dispatch(0x97CE, 0x97DD, 0x97E1); }  /* screens: $9787, $97E5, $97EC, $97F3 */
void func_97E5_b4(void) { sprite_set(0x97E5, 0x99CC); }  /* screen 1 */
void func_97EC_b4(void) { sprite_set(0x97EC, 0x99CC); }  /* screen 2 */
void func_97F3_b4(void) { sprite_set(0x97F3, 0x99CC); }  /* screen 3 (no route reaches it) */

/* area_index 4, round 2 area 2 */
void func_97FA_b4(void) { screen_dispatch(0x97FA, 0x9809, 0x980E); }  /* screens: $9787, $9813, $981A, $9821, $9828 */
void func_9813_b4(void) { sprite_set(0x9813, 0x99D0); }  /* screen 1 */
void func_981A_b4(void) { sprite_set(0x981A, 0x99D0); }  /* screen 2 */
void func_9821_b4(void) { sprite_set(0x9821, 0x99D0); }  /* screen 3 */
void func_9828_b4(void) { sprite_set(0x9828, 0x99D0); }  /* screen 4 (no route reaches it) */

/* area_index 5, round 2 area 3 */
void func_982F_b4(void) { screen_dispatch(0x982F, 0x983E, 0x9840); }  /* screens: $9787, $9842 */
void func_9842_b4(void) { sprite_set(0x9842, 0x99D5); }  /* screen 1 */

/* area_index 6, round 3 area 1 */
void func_9849_b4(void) { screen_dispatch(0x9849, 0x9858, 0x985C); }  /* screens: $9787, $9860, $9867, $986E */
void func_9860_b4(void) { sprite_set(0x9860, 0x99D7); }  /* screen 1 */
void func_9867_b4(void) { sprite_set(0x9867, 0x99D7); }  /* screen 2 (no route reaches it) */
void func_986E_b4(void) { sprite_set(0x986E, 0x99D7); }  /* screen 3 (no route reaches it) */

/* area_index 7, round 3 area 2 */
void func_9875_b4(void) { screen_dispatch(0x9875, 0x9884, 0x9888); }  /* screens: $9787, $988C, $9893, $989A */
void func_988C_b4(void) { sprite_set(0x988C, 0x99DB); }  /* screen 1 */
void func_9893_b4(void) { sprite_set(0x9893, 0x99DB); }  /* screen 2 */
void func_989A_b4(void) { sprite_set(0x989A, 0x99DB); }  /* screen 3 (no route reaches it) */

/* area_index 8, round 3 area 3 */
void func_98A1_b4(void) { screen_dispatch(0x98A1, 0x98B0, 0x98B2); }  /* screens: $9787, $98B4 */
void func_98B4_b4(void) { sprite_set(0x98B4, 0x99E4); }  /* screen 1 */

/* area_index 9, round 4 area 1 */
void func_98BB_b4(void) { screen_dispatch(0x98BB, 0x98CA, 0x98CE); }  /* screens: $9787, $98D2, $98D9, $98E0 */
void func_98D2_b4(void) { sprite_set(0x98D2, 0x99E0); }  /* screen 1 */
void func_98D9_b4(void) { sprite_set(0x98D9, 0x99E0); }  /* screen 2 */
void func_98E0_b4(void) { sprite_set(0x98E0, 0x99E0); }  /* screen 3 */

/* area_index 10, round 4 area 2 */
void func_98E7_b4(void) { screen_dispatch(0x98E7, 0x98F6, 0x98F8); }  /* screens: $9787, $98FA */
void func_98FA_b4(void) { sprite_set(0x98FA, 0x99ED); }  /* screen 1 */

/* area_index 11, round 4 area 3 */
void func_9901_b4(void) { screen_dispatch(0x9901, 0x9910, 0x9914); }  /* screens: $9787, $9918, $991F, $9926 */
void func_9918_b4(void) { sprite_set(0x9918, 0x99EA); }  /* screen 1 */
void func_991F_b4(void) { sprite_set(0x991F, 0x99EB); }  /* screen 2 */
void func_9926_b4(void) { sprite_set(0x9926, 0x99F2); }  /* screen 3 */

/* area_index 12, round 5 area 1 */
void func_9947_b4(void) { screen_dispatch(0x9947, 0x9956, 0x995A); }  /* screens: $9787, $995E, $9965, $996C */
void func_995E_b4(void) { sprite_set(0x995E, 0x99EC); }  /* screen 1 */
void func_9965_b4(void) { sprite_set(0x9965, 0x99EC); }  /* screen 2 */
void func_996C_b4(void) { sprite_set(0x996C, 0x99EC); }  /* screen 3 */

/* area_index 13, round 5 area 2 */
void func_9973_b4(void) { screen_dispatch(0x9973, 0x9982, 0x9986); }  /* screens: $9787, $998A, $9991, $9998 */
void func_998A_b4(void) { sprite_set(0x998A, 0x99F0); }  /* screen 1 */
void func_9991_b4(void) { sprite_set(0x9991, 0x99F0); }  /* screen 2 */
void func_9998_b4(void) { sprite_set(0x9998, 0x99F0); }  /* screen 3 */

/* area_index 14, round 5 area 3 */
void func_999F_b4(void) { screen_dispatch(0x999F, 0x99AE, 0x99B1); }  /* screens: $9787, $99B4, $99BB */
void func_99B4_b4(void) { sprite_set(0x99B4, 0x99F4); }  /* screen 1 */
void func_99BB_b4(void) { sprite_set(0x99BB, 0x99F4); }  /* screen 2 (no route reaches it) */
