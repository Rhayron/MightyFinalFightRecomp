/*
 * stage_spawn.c - the enemies of an area (docs/objects.md, "Spawning"),
 * bank $08 (16 KiB bank 4 at $8000)
 *
 * Two sources:
 *  - the stage object list: per area (area_index $67), entries of x, x high,
 *    y, y page and a type, in five parallel arrays (spawn_list_x $72,
 *    _x_hi $74, _y $76, _y_hi $78, _type $7A). As the camera reaches an
 *    entry, stage_spawns puts it in a free slot from $0D down to 3 and fills
 *    it from two per-type tables ($8442: flags, id, animation, attack;
 *    $85E6: life, spawn group, kind). spawn_list_next ($9E) is the next
 *    entry ahead of the camera, spawn_list_prev ($9F) the first one still in
 *    view behind it; obj_spawn_index remembers which entry a slot came from.
 *  - the timed spawner: while the camera is locked on a screen (and in round
 *    4 area 2 always), every spawn_timer ($63) frames a common enemy drops in
 *    at a random spot; which one depends on the round, the area, the screen
 *    and on the enemies already there (their spawn groups, gathered in
 *    spawn_groups $05A0). The area scripts (stage_events.c) call it until
 *    the screen's kill quota is met.
 */
#include "stage.h"

void func_F26E(void);   /* obj_vy for a fall from the top */
void func_F288(void);   /* obj_set_anim */
void func_F2A0(void);   /* obj_spawn: slot Y, animation A */
void func_F314(void);   /* obj_dir_to_player */
void func_F32E(void);   /* obj_flip_from_dir */
void func_F71A(void);   /* find_free_slot: Y, carry set if none */
void func_80E3_b4(void);
void func_9011_b4(void);

/* the five list pointers by area_index: table, then the RAM it fills, in
 * the order $8000 copies them */
static const struct { uint16_t table, ptr; } k_list_ptrs[] = {
    { 0x81FE, SPAWN_LIST_X_HI }, { 0x820D, SPAWN_LIST_X_HI + 1 },
    { 0x821C, SPAWN_LIST_X },    { 0x822B, SPAWN_LIST_X + 1 },
    { 0x823A, SPAWN_LIST_Y_HI }, { 0x8249, SPAWN_LIST_Y_HI + 1 },
    { 0x8258, SPAWN_LIST_Y },    { 0x8267, SPAWN_LIST_Y + 1 },
    { 0x8276, SPAWN_LIST_TYPE }, { 0x8285, SPAWN_LIST_TYPE + 1 },
};

/* the per-type tables of the stage list, 4 bytes per type */
#define K_TYPE_LOOK     0x8442      /* obj_flags, obj_id, obj_anim, obj_attack */
#define K_TYPE_STATS    0x85E6      /* life, obj_spawn_group, obj_kind */

/* the fields stage_list_spawn clears after obj_spawn's fall, in its order */
static const uint16_t k_list_spawn_cleared[] = {
    OBJ_ANIM_TIMER, OBJ_ANIM_FRAME, OBJ_THROW_FLAG, OBJ_TIMER, OBJ_TIMER2, OBJ_GROUND_Y,
    OBJ_LAND_Y, OBJ_TIMER3, OBJ_DOWN_TIMER, OBJ_HIT_COOLDOWN, OBJ_INVULN, OBJ_ATTACK_DAMAGE,
    OBJ_HIT_PLAYER, OBJ_HIT_TIMER, OBJ_HELD, OBJ_TIER, OBJ_AI_TIMER, OBJ_AI_COUNT, OBJ_AI_FLAG,
};

/* the stage list's id $69 shows each character a different animation */
#define ID_PER_CHARACTER 0x69
static const uint8_t k_items[] = { 0x10, 0x11, 0x12, 0x13 };

/* the timed spawner: frames to the next spawn, by a random 0-15 */
#define K_SPAWN_DELAY   0x9024
/* where a timed enemy drops in, by a random 0-31: x, x high (+ the
 * camera's), y; and the y of round 1 area 2 and round 2 area 2 (0-7) */
#define K_DROP_X        0x964B
#define K_DROP_X_HI     0x966B
#define K_DROP_Y        0x968B
#define K_DROP_Y_R0A1   0x96AB
#define K_DROP_Y_R1A1   0x96B3
/* the enemy chosen: per round a pointer table by sub_area to a type list
 * by screen ($9363/$9368 -> $937C/$938B -> the screen's base type), plus
 * 0-2; then by type: animation, id, kind, spawn group */
#define K_TYPES_LO      0x9363
#define K_TYPES_HI      0x9368
#define K_AREA_TYPES_LO 0x937C
#define K_AREA_TYPES_HI 0x938B
#define K_TYPE_ANIM     0x94DD
#define K_TYPE_ID       0x9456
#define K_TYPE_KIND     0x9564
#define K_TYPE_GROUP    0x93CF
/* life by round, by a random 0-15 ($95EB + 16 * round), and for animation
 * $B7 */
#define K_LIFE_ROUND0   0x95EB
#define K_LIFE_B7       0x963B
#define ANIM_B7         0xB7
/* round 4 area 2's spawner: x by a random 0-7, and its two enemy sets */
#define K_DROP_X_R3A1   0x96BB
#define K_R3A1_SET_A    0x934D
#define K_R3A1_SET_B    0x934B
#define K_R3A1_ANIM     0x934F
#define K_R3A1_ID       0x9353
#define K_R3A1_KIND     0x9357
#define K_R3A1_GROUP    0x935B
#define K_R3A1_LIFE     0x935F

/* the fields the timed spawners clear in the slot they found, in order */
static const uint16_t k_timed_cleared[] = {
    OBJ_THROW_FLAG, OBJ_HIT_COOLDOWN, OBJ_INVULN, OBJ_HIT_PLAYER, OBJ_ATTACK_DAMAGE, OBJ_ATTACK,
    OBJ_HIT_TIMER, OBJ_HELD, OBJ_TIER, OBJ_AI_TIMER, OBJ_AI_COUNT, OBJ_AI_FLAG, OBJ_TIMER,
    OBJ_TIMER2, OBJ_GROUND_Y, OBJ_LAND_Y, OBJ_TIMER3, OBJ_DOWN_TIMER, OBJ_VX_FRAC, OBJ_VX,
};

#define N_OF(t) (sizeof (t) / sizeof (t)[0])

/*
 * stage_list_setup, $8000 (bank 4; proposed name): points spawn_list_x ..
 * spawn_list_type ($72-$7B) at the stage object list of area_index.
 * Out: X = area_index, A = the last pointer byte.
 * Called by: area_start (JSR at $E087).
 */
void func_8000_b4(void) {
    REGS_IN;
    unsigned i;

    mff_cycles(0x8000, 3);
    x = RAM(AREA_INDEX);
    for (i = 0; i < N_OF(k_list_ptrs); i++) {
        mff_cycles((uint16_t)(0x8002 + 5 * i), 4);
        a = prg_read_ix(k_list_ptrs[i].table, x);
        mff_cycles((uint16_t)(0x8005 + 5 * i), 3);
        RAM(k_list_ptrs[i].ptr) = a;
    }
    SET_NZ(a);
    mff_cycles(0x8034, 6);
    SYNC_OUT;
    mff_rts();
}

/*
 * stage_list_seek, $8038 (bank 4; proposed name): spawn_list_next and
 * spawn_list_prev = the first entry on the camera's vertical page at or
 * ahead of the camera (x_hi:x >= area_screen:scroll_x). At an area's start
 * that is always entry 0 in every route. If none were found (Y wrapping to
 * 0), the code at $8050-$8072 would look again from the camera minus one
 * pixel; it cannot run: the BEQ at $804E after the wrap always jumps past it.
 * Called by: area_start (JSR at $E08A).
 */
void func_8038_b4(void) {
    REGS_IN;

    mff_cycles(0x8038, 2);
    y = 0x00;
    for (;;) {
        mff_cycles(0x803A, 5);
        a = mem_read_iy(SPAWN_LIST_Y_HI, y);
        mff_cycles(0x803C, 5);
        c = a >= RAM(SCROLL_Y_HI);
        if (a == RAM(SCROLL_Y_HI)) {
            mff_cycles(0x8040, 5);
            a = mem_read_iy(SPAWN_LIST_X, y);
            mff_cycles(0x8042, 10);
            c = a >= RAM(SCROLL_X);
            a = mem_read_iy(SPAWN_LIST_X_HI, y);
            mff_cycles(0x8047, 5);
            SBC_A(RAM(AREA_SCREEN));
            if (c) {
                mff_cycles(0x8049, 1);
                break;
            }
        } else {
            mff_cycles(0x803E, 1);
        }
        mff_cycles(0x804B, 4);
        y++;
        SET_NZ(y);
        if (y == 0) {                           /* no route */
            mff_cycles(0x804E, 2 + 1);
            break;
        }
        mff_cycles(0x804C, 1);
    }
    mff_cycles(0x8073, 12);
    RAM(SPAWN_LIST_NEXT) = y;
    RAM(SPAWN_LIST_PREV) = y;
    SYNC_OUT;
    mff_rts();
}

/*
 * stage_list_spawner, $80A7 (stage_spawns for scroll_mode 1, the camera
 * going right): spawns entries from spawn_list_next on while they are in
 * view, then walks spawn_list_prev back to the first entry still in view.
 */
static void stage_list_spawner(void) {
    REGS_IN;

    for (;;) {
        mff_cycles(0x80A7, 9);
        y = RAM(SPAWN_LIST_NEXT);
        SET_NZ(y);
        SYNC_OUT;
        if (!mff_call_at(0x00AB, 0x80C2, 4)) return;
        SYNC_IN;
        mff_cycles(0x80AC, 2);
        if (!z) {
            mff_cycles(0x80AC, 1);
            break;
        }
        mff_cycles(0x80AE, 6);
        SYNC_OUT;
        if (!mff_call_at(0x00B0, 0x80E4, 4)) return;
        SYNC_IN;
        mff_cycles(0x80B1, 7);
        t = (uint8_t)(RAM(SPAWN_LIST_NEXT) + 1);
        RAM(SPAWN_LIST_NEXT) = t;
        if (t == 0) break;
        mff_cycles(0x80B3, 1);
    }
    mff_cycles(0x80B5, 5);
    y = RAM(SPAWN_LIST_NEXT);
    SET_NZ(y);
    if (y != 0) {
        for (;;) {
            mff_cycles(0x80B9, 8);
            y--;
            SET_NZ(y);
            SYNC_OUT;
            if (!mff_call_at(0x00BC, 0x80C2, 4)) return;
            SYNC_IN;
            mff_cycles(0x80BD, 2);
            if (!z) break;
            mff_cycles(0x80BD, 1);
        }
    } else {
        mff_cycles(0x80B7, 1);
    }
    mff_cycles(0x80BF, 9);
    RAM(SPAWN_LIST_PREV) = y;
    SYNC_OUT;
    mff_rts();
}

/*
 * stage_spawns, $8080 (bank 4): by scroll_mode & 3, nothing (0), the
 * rightward walk of the stage list (1), or the leftward one (2, 3: no route,
 * the camera never goes left): spawn from spawn_list_prev down while
 * entries are in view, then move spawn_list_next to the first entry past
 * the view.
 * Called by: gameplay_update (JSR at $C963) and area_start ($E08D).
 */
void func_8080_b4(void) {
    REGS_IN;

    mff_cycles(0x8080, 7);
    a = RAM(SCROLL_MODE) & 0x03;
    SET_NZ(a);
    if (a == 0) {
        mff_cycles(0x8084, 1 + 6);
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0x8086, 4);
    c = a >= 1;
    if (a == 1) {
        mff_cycles(0x8088, 1);
        SYNC_OUT;
        stage_list_spawner();
        return;
    }
    mff_cycles(0x808A, 4);
    c = a >= 2;
    if (a == 2) mff_cycles(0x808C, 1);          /* BEQ to the next instruction */

    /* no route: leftward */
    for (;;) {
        mff_cycles(0x808E, 9);
        y = RAM(SPAWN_LIST_PREV);
        SET_NZ(y);
        SYNC_OUT;
        if (!mff_call_at(0x0092, 0x80C2, 4)) return;
        SYNC_IN;
        mff_cycles(0x8093, 2);
        if (!z) {
            mff_cycles(0x8093, 1);
            break;
        }
        mff_cycles(0x8095, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0097, 0x80E4, 4)) return;
        SYNC_IN;
        mff_cycles(0x8098, 7);
        t = (uint8_t)(RAM(SPAWN_LIST_PREV) - 1);
        RAM(SPAWN_LIST_PREV) = t;
        if (t == 0) break;
        mff_cycles(0x809A, 1);
    }
    mff_cycles(0x809C, 3);
    y = RAM(SPAWN_LIST_PREV);
    for (;;) {
        mff_cycles(0x809E, 8);
        y++;
        SET_NZ(y);
        SYNC_OUT;
        if (!mff_call_at(0x00A1, 0x80C2, 4)) return;
        SYNC_IN;
        mff_cycles(0x80A2, 2);
        if (!z) break;
        mff_cycles(0x80A2, 1);
    }
    mff_cycles(0x80A4, 3 + 6);
    RAM(SPAWN_LIST_NEXT) = y;
    SYNC_OUT;
    mff_rts();
}

/*
 * stage_list_in_view, $80C2 (bank 4; proposed name): is entry Y of the
 * stage list on the camera's screen? Z set when its x is within 256 pixels
 * ahead of the camera and its y is on the camera's vertical page (y counts
 * 0-$EF per page). Leaves $00/$01 = its x, x high and, when x is in view,
 * $02/$03 = its y and y page.
 * Called by: stage_spawns ($808E, $809E), stage_list_spawner ($80A7,
 * $80B9).
 */
void func_80C2_b4(void) {
    REGS_IN;

    mff_cycles(0x80C2, 5);
    a = mem_read_iy(SPAWN_LIST_X, y);
    mff_cycles(0x80C4, 13);
    RAM(TMP0) = a;
    c = a >= RAM(SCROLL_X);
    a = mem_read_iy(SPAWN_LIST_X_HI, y);
    mff_cycles(0x80CB, 8);
    RAM(TMP1) = a;
    SBC_A(RAM(AREA_SCREEN));
    if (z) {
        mff_cycles(0x80D1, 5);
        a = mem_read_iy(SPAWN_LIST_Y, y);
        mff_cycles(0x80D3, 10);
        RAM(TMP2) = a;
        c = a >= RAM(SCROLL_Y);
        if (!c) {                               /* SBC #$0F, CLC: page rows are $F0 */
            mff_cycles(0x80DA, 4);
            c = 0;
        } else {
            mff_cycles(0x80D8, 1);
        }
        mff_cycles(0x80DD, 5);
        a = mem_read_iy(SPAWN_LIST_Y_HI, y);
        mff_cycles(0x80DF, 6);
        RAM(TMP3) = a;
        SBC_A(RAM(SCROLL_Y_HI));
    } else {
        mff_cycles(0x80CF, 1);
    }
    SYNC_OUT;
    func_80E3_b4();
}

/*
 * $80E3: stage_list_in_view's RTS, which stage_list_spawn's BMI at $80F6
 * reaches too (no route).
 */
void func_80E3_b4(void) {
    mff_cycles(0x80E3, 6);
    mff_rts();
}

/*
 * stage_list_spawn, $80E4 (bank 4; proposed name): spawns entry Y of the
 * stage list (its position in $00-$03 from stage_list_in_view) in the
 * first free slot from $0D down to 3, unless a live slot already came from
 * that entry (the BMI at $80F6 then returns through the RTS at $80E3; no
 * route). The type's look and stats come from K_TYPE_LOOK/K_TYPE_STATS;
 * it starts falling (0F:F26E), turns toward the player, and id $69 takes
 * the character's own animation ($40 Cody, $5E Guy, $5D Haggar).
 * Called by: stage_spawns ($8095), stage_list_spawner ($80AE).
 */
void func_80E4_b4(void) {
    REGS_IN;
    unsigned i;

    /* already spawned and alive? */
    mff_cycles(0x80E4, 4);
    x = 0x0D;
    a = y;
    for (;;) {
        mff_cycles(0x80E7, 6);
        c = a >= RAM(OBJ_SPAWN_INDEX + x);
        if (a == RAM(OBJ_SPAWN_INDEX + x)) {
            mff_cycles(0x80EA, 1 + 6);
            a = RAM(OBJ_STATE + x);
            SET_NZ(a);
            if (n) {                            /* no route */
                mff_cycles(0x80F6, 1);
                SYNC_OUT;
                func_80E3_b4();
                return;
            }
            break;
        }
        mff_cycles(0x80EC, 6);
        x--;
        c = x >= 2;
        z = x == 2;
        if (z) {
            mff_cycles(0x80F1, 2 + 1);          /* BEQ always taken */
            break;
        }
        mff_cycles(0x80EF, 1);
    }

    /* a free slot */
    mff_cycles(0x80F8, 2);
    x = 0x0D;
    for (;;) {
        mff_cycles(0x80FA, 6);
        a = RAM(OBJ_STATE + x);
        if (!(a & 0x80)) {
            mff_cycles(0x80FD, 2);              /* BPL across a page */
            break;
        }
        mff_cycles(0x80FF, 6);
        x--;
        SET_CMP(x, 2);
        if (z) {                                /* none */
            mff_cycles(0x8104, 6);
            SYNC_OUT;
            mff_rts();
            return;
        }
        mff_cycles(0x8102, 2);                  /* BNE across a page */
    }

    /* the entry and its type's look */
    mff_cycles(0x8105, 54);
    RAM(OBJ_SPAWN_INDEX + x) = y;
    RAM(OBJ_X + x) = RAM(TMP0);
    RAM(OBJ_X_HI + x) = RAM(TMP1);
    RAM(OBJ_Y + x) = RAM(TMP2);
    RAM(OBJ_Y_HI + x) = RAM(TMP3);
    RAM(TMP4) = K_TYPE_LOOK & 0xFF;
    RAM(TMP5) = K_TYPE_LOOK >> 8;
    a = mem_read_iy(SPAWN_LIST_TYPE, y);
    mff_cycles(0x8127, 22);
    RAM(TMP6) = y;
    c = (a >> 6) & 1;                           /* ASL, ASL: the type * 4 */
    y = (uint8_t)(a << 2);
    RAM(TMP5) = (uint8_t)(RAM(TMP5) + c);
    a = mem_read_iy(TMP4, y);
    mff_cycles(0x8134, 12);
    RAM(OBJ_FLAGS + x) = a;
    y++;
    a = mem_read_iy(TMP4, y);
    mff_cycles(0x813A, 17);
    RAM(OBJ_ID + x) = a;
    RAM(OBJ_SAVED_ID + x) = a;
    y++;
    a = mem_read_iy(TMP4, y);
    mff_cycles(0x8143, 12);
    RAM(OBJ_ANIM + x) = a;
    y++;
    a = mem_read_iy(TMP4, y);

    /* its stats */
    mff_cycles(0x8149, 23);
    RAM(OBJ_ATTACK + x) = a;
    y = RAM(TMP6);
    RAM(TMP7) = K_TYPE_STATS & 0xFF;
    RAM(TMP8) = K_TYPE_STATS >> 8;
    a = mem_read_iy(SPAWN_LIST_TYPE, y);
    mff_cycles(0x8158, 19);
    c = (a >> 6) & 1;
    y = (uint8_t)(a << 2);
    a = RAM(TMP8);
    ADC_A(0);
    RAM(TMP8) = a;
    a = mem_read_iy(TMP7, y);
    mff_cycles(0x8163, 17);
    RAM(OBJ_LIFE + x) = a;
    RAM(OBJ_LIFE_MAX + x) = a;
    y++;
    a = mem_read_iy(TMP7, y);
    mff_cycles(0x816C, 12);
    RAM(OBJ_SPAWN_GROUP + x) = a;
    y++;
    a = mem_read_iy(TMP7, y);
    mff_cycles(0x8172, 23);
    RAM(OBJ_KIND + x) = a;
    a = 0x80;
    SET_NZ(a);
    RAM(OBJ_STATE + x) = a;
    RAM(OBJ_SAVED_STATE + x) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x017F, func_F26E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8180, 103);
    a = 0x00;
    SET_NZ(a);
    for (i = 0; i < N_OF(k_list_spawn_cleared); i++)
        RAM(k_list_spawn_cleared[i] + x) = 0x00;
    SYNC_OUT;
    if (!mff_call_fn(0x01BD, func_F314, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x81BE, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x01C0, func_F32E, 0xE000)) return;
    SYNC_IN;

    /* id $69's animation by character; the item ids $10-$13 are compared
     * too, to no effect (every branch goes to the RTS) */
    mff_cycles(0x81C1, 8);
    a = RAM(OBJ_ID + x);
    if (a != ID_PER_CHARACTER) {
        uint16_t pc = 0x81C8;
        for (i = 0; i < sizeof k_items; i++, pc += 4) {
            mff_cycles(pc, 4);
            SET_CMP(a, k_items[i]);
            if (z) {
                mff_cycles((uint16_t)(pc + 2), 1);
                break;
            }
        }
        mff_cycles(0x81D8, 6);
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0x81C6, 1 + 7);
    a = RAM(PLAYER_ID);
    c = a >= PLAYER_CODY;
    if (a == PLAYER_CODY) {
        mff_cycles(0x81DD, 1 + 4 + 1);          /* LDA #$40, BNE taken */
        a = 0x40;
    } else {
        mff_cycles(0x81DF, 4);
        c = a >= PLAYER_GUY;
        if (a == PLAYER_GUY) {
            mff_cycles(0x81E1, 1 + 4 + 1);      /* LDA #$5E, BNE taken */
            a = 0x5E;
        } else {
            mff_cycles(0x81E3, 4);
            SET_CMP(a, PLAYER_HAGGAR);
            if (!z) {                           /* no route */
                mff_cycles(0x81E7, 6);
                SYNC_OUT;
                mff_rts();
                return;
            }
            mff_cycles(0x81E5, 1 + 2);
            a = 0x5D;
        }
    }
    SET_NZ(a);
    mff_cycles(0x81F2, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x01F4, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x81F5, 6);
    SYNC_OUT;
    mff_rts();
}

/*
 * $910A-$9120 and $928F-$92A1: spawn_groups[1], [0] = the spawn groups of
 * the live enemies in slots $0D down to 3 (objects with group $FF skipped).
 * A third enemy and more are written below the array ($069F, $069E...), as
 * the game does. pc = the LDX #$0D. Returns Y (1 - the count).
 */
static uint8_t gather_spawn_groups(uint16_t pc) {
    uint8_t slot = 0x0D, yy = 0x01;

    mff_cycles(pc, 4);
    for (;;) {
        mff_cycles((uint16_t)(pc + 4), 6);
        if (RAM(OBJ_STATE + slot) & 0x80) {
            uint8_t group;
            mff_cycles((uint16_t)(pc + 9), 6);
            group = RAM(OBJ_SPAWN_GROUP + slot);
            if (!(group & 0x80)) {
                mff_cycles((uint16_t)(pc + 14), 7);
                RAM(SPAWN_GROUPS + yy) = group;
                yy--;
            } else {
                mff_cycles((uint16_t)(pc + 12), 1);
            }
        } else {
            mff_cycles((uint16_t)(pc + 7), 1);
        }
        mff_cycles((uint16_t)(pc + 18), 6);
        slot--;
        if (slot == 2) break;
        mff_cycles((uint16_t)(pc + 21), 1);
    }
    return yy;
}

/* what the sum of the spawn groups picks: 7, 8 or 9 as they are, the others
 * remapped; any other sum (two of a kind) spawns nothing */
static const struct { uint8_t sum, pick; } k_pick_timed[] = {
    { 0x07, 0 }, { 0x08, 0 }, { 0x09, 0 }, { 0x00, 0x09 },
    { 0x03, 0x07 }, { 0x04, 0x09 }, { 0x05, 0x08 }, { 0xFF, 0x07 },
};
static const struct { uint8_t sum, pick; } k_pick_r3a1[] = {
    { 0x07, 0 }, { 0x08, 0 }, { 0x09, 0 }, { 0x00, 0x07 },
    { 0x04, 0x07 }, { 0x05, 0x08 }, { 0xFF, 0x07 },
};

/*
 * timed_spawner, $9000 (bank 4): while the camera is locked on a screen (in
 * round 4 area 2, round 3 sub_area 1, always), the area is not cleared
 * (area_clear not negative) and fewer than two big enemies are up
 * (enemies_two, $96C3), counts spawn_timer down; reaching 0 spawns one
 * (timed_spawn, the BCC at $9021 into $9038).
 * Called by: the area scripts (stage_events.c) and 0E:DEBC.
 */
void func_9000_b4(void) {
    REGS_IN;

    mff_cycles(0x9000, 5);
    a = RAM(CAMERA_LOCK);
    if (a == 0) {
        mff_cycles(0x9004, 7);
        a = RAM(ROUND);
        SET_CMP(a, 3);
        if (!z) {                               /* no route */
            mff_cycles(0x900A, 6);
            SYNC_OUT;
            mff_rts();
            return;
        }
        mff_cycles(0x9008, 1 + 7);
        a = RAM(SUB_AREA);
        SET_CMP(a, 1);
        if (!z) {
            SYNC_OUT;
            func_9011_b4();
            return;
        }
        mff_cycles(0x900F, 1);
    } else {
        mff_cycles(0x9002, 1);
    }

    mff_cycles(0x9012, 5);
    a = RAM(AREA_CLEAR);
    SET_NZ(a);
    if (n) {
        mff_cycles(0x9014, 1 + 6);
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0x9016, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1018, 0x96C3, 4)) return;
    SYNC_IN;
    mff_cycles(0x9019, 2);
    if (!z) {
        mff_cycles(0x9019, 1 + 6);
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0x901B, 12);
    RAM(SPAWN_TIMER)--;
    a = RAM(SPAWN_TIMER);
    SET_CMP(a, 1);
    if (!c) {
        mff_cycles(0x9021, 1);
        SYNC_OUT;
        call_by_address_tail((uint16_t)(g_code_window_base + 0x1038), 4);
        return;
    }
    mff_cycles(0x9023, 6);
    SYNC_OUT;
    mff_rts();
}

/*
 * $9011: timed_spawner's RTS in round 4 outside area 2 with the camera free,
 * which timed_spawn's BCS at $9059 (no free slot) reaches too.
 */
void func_9011_b4(void) {
    mff_cycles(0x9011, 6);
    mff_rts();
}

/*
 * timed_spawn, $9038 (bank 4; proposed name): reloads spawn_timer with a
 * random delay, then (except in round 4 area 2, which has timed_spawn_r3a1)
 * puts a common enemy in a free slot (spawn_slot $3C): falling in at a
 * random spot of the screen (round 1 area 2 and round 2 area 2 move it to
 * their own lines), its type from the round, area and screen tables plus
 * 0-2 by the spawn groups already up, its life from the round's table (or
 * the $B7 animation's). The RNG ($E4-$E6) is stepped along the way.
 * Called by: timed_spawner (BCC at $9021).
 */
void func_9038_b4(void) {
    REGS_IN;
    unsigned i;
    int line_r1a1, r;

    mff_cycles(0x9038, 19);
    a = RAM(RNG);
    c = 0;
    ADC_A(RAM(RNG + 1));
    RAM(RNG) = a;
    y = a & 0x0F;
    a = prg_read_ix(K_SPAWN_DELAY, y);
    mff_cycles(0x9045, 10);
    RAM(SPAWN_TIMER) = a;
    a = RAM(ROUND);
    SET_CMP(a, 3);
    if (z) {
        mff_cycles(0x904D, 7);
        a = RAM(SUB_AREA);
        SET_CMP(a, 1);
        if (z) {
            mff_cycles(0x9053, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x921C, 2);
            call_by_address_tail(0x921C, 4);
            return;
        }
        mff_cycles(0x9051, 1);
    } else {
        mff_cycles(0x904B, 1);
    }

    mff_cycles(0x9056, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1058, func_F71A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9059, 2);
    if (c) {                                    /* no free slot: the RTS at $9011 */
        mff_cycles(0x9059, 1);
        SYNC_OUT;
        func_9011_b4();
        return;
    }

    /* the slot: facing as the player does, falling in at a random spot */
    mff_cycles(0x905B, 152);
    RAM(SPAWN_SLOT) = y;
    RAM(OBJ_DIR + y) = RAM(PLAYER_DIR);
    for (i = 0; i < N_OF(k_timed_cleared); i++)
        RAM(k_timed_cleared[i] + y) = 0x00;
    RAM(OBJ_VY_FRAC + y) = 0xC0;
    RAM(OBJ_VY + y) = 0xFF;
    a = (uint8_t)(RAM(RNG) + RAM(RNG + 1) + c);    /* ADC without CLC: C is 0 */
    RAM(RNG) = a;
    c = 1;
    a = (uint8_t)(a - RAM(RNG + 2));
    RAM(RNG + 1) = a;
    x = a & 0x1F;
    a = prg_read_ix(K_DROP_X, x);
    mff_cycles(0x90BB, 14);
    RAM(OBJ_X + y) = a;
    c = 0;
    a = (uint8_t)(RAM(SCROLL_X_HI) + prg_read_ix(K_DROP_X_HI, x));
    mff_cycles(0x90C4, 9);
    RAM(OBJ_X_HI + y) = a;
    a = prg_read_ix(K_DROP_Y, x);
    mff_cycles(0x90CA, 10);
    RAM(OBJ_Y + y) = a;

    /* round 1 area 2: a drop on the lower lines ($98 on) moves to its own */
    line_r1a1 = 1;
    a = RAM(ROUND);
    if (a != 0) {
        mff_cycles(0x90CF, 1);
    } else {
        mff_cycles(0x90D1, 7);
        a = RAM(SUB_AREA);
        if (a != 1) {
            mff_cycles(0x90D5, 1);
        } else {
            mff_cycles(0x90D7, 8);
            a = RAM(OBJ_Y + y);
            c = a >= 0x98;
            if (!c) {
                mff_cycles(0x90DC, 2);          /* BCC across a page */
                line_r1a1 = 0;
            } else {
                mff_cycles(0x90DE, 17);
                a = (uint8_t)(RAM(RNG + 2) + RAM(RNG + 1) + c);
                RAM(RNG + 2) = a;
                x = a & 0x07;
                a = prg_read_ix(K_DROP_Y_R0A1, x);
                mff_cycles(0x90EA, 7);
                RAM(OBJ_Y + y) = a;
                if (a != 0) {                   /* always: the table has no 0 */
                    mff_cycles(0x90ED, 2);
                    line_r1a1 = 0;
                }
            }
        }
    }
    /* round 2 area 2: always a line of its own */
    if (line_r1a1) {
        mff_cycles(0x90EF, 7);
        a = RAM(ROUND);
        if (a == 1) {
            mff_cycles(0x90F5, 7);
            a = RAM(SUB_AREA);
            c = a >= 1;
            if (a == 1) {
                mff_cycles(0x90FB, 17);
                a = (uint8_t)(RAM(RNG + 2) + RAM(RNG + 1) + c);
                RAM(RNG + 2) = a;
                x = a & 0x07;
                a = prg_read_ix(K_DROP_Y_R1A1, x);
                mff_cycles(0x9107, 5);
                RAM(OBJ_Y + y) = a;
            } else {
                mff_cycles(0x90F9, 2);          /* BNE across a page */
            }
        } else {
            mff_cycles(0x90F3, 2);              /* BNE across a page */
        }
    }

    /* the spawn groups up decide 0-2 */
    y = gather_spawn_groups(0x910A);
    mff_cycles(0x9121, 19);
    x = 0x02;
    a = RAM(SPAWN_GROUPS);
    c = 0;
    ADC_A(RAM(SPAWN_GROUPS + 1));
    RAM(TMP1) = a;
    for (i = 0; i < N_OF(k_pick_timed); i++) {
        if (i) mff_cycles((uint16_t)(0x912C + 4 * i), 4);
        if (a == k_pick_timed[i].sum) break;
    }
    if (i == N_OF(k_pick_timed)) {
        c = a >= 0xFF;
        mff_cycles(0x914C, 20);
        a = 0x00;
        SET_NZ(a);
        RAM(SPAWN_GROUPS) = a;
        RAM(SPAWN_GROUPS + 1) = a;
        RAM(SPAWN_GROUPS + 2) = a;
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles((uint16_t)(0x912E + 4 * i), 1);
    if (k_pick_timed[i].pick) {
        mff_cycles(0x9158, i == N_OF(k_pick_timed) - 1 ? 5 : 8);
        RAM(TMP1) = k_pick_timed[i].pick;
    }

    /* $01 = 0-2; the type from the round's tables */
    mff_cycles(0x9174, 27);
    c = 1;
    RAM(TMP1) = (uint8_t)(0x0C - RAM(TMP1));
    a = (uint8_t)(0x05 - RAM(TMP1));
    RAM(TMP1) = a;
    y = RAM(ROUND);
    a = prg_read_ix(K_TYPES_LO, y);
    mff_cycles(0x9187, 7);
    RAM(TMP2) = a;
    a = prg_read_ix(K_TYPES_HI, y);
    mff_cycles(0x918C, 11);
    RAM(TMP3) = a;
    y = RAM(SUB_AREA);
    a = mem_read_iy(TMP2, y);
    mff_cycles(0x9192, 6);
    y = a;
    a = prg_read_ix(K_AREA_TYPES_LO, y);
    mff_cycles(0x9196, 7);
    RAM(TMP4) = a;
    a = prg_read_ix(K_AREA_TYPES_HI, y);
    mff_cycles(0x919B, 11);
    RAM(TMP5) = a;
    y = RAM(SCROLL_X_HI);
    a = mem_read_iy(TMP4, y);
    mff_cycles(0x91A1, 14);
    c = 0;
    ADC_A(RAM(TMP1));
    x = a;
    y = RAM(SPAWN_SLOT);
    a = prg_read_ix(K_TYPE_ANIM, x);
    SET_NZ(a);
    mff_cycles(0x91AA, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x11AC, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x91AD, 4);
    a = prg_read_ix(K_TYPE_ID, x);
    mff_cycles(0x91B0, 14);
    RAM(OBJ_ID + y) = a;
    RAM(OBJ_SAVED_ID + y) = a;
    a = prg_read_ix(K_TYPE_KIND, x);
    mff_cycles(0x91B9, 9);
    RAM(OBJ_KIND + y) = a;
    a = prg_read_ix(K_TYPE_GROUP, x);

    /* life: $B7's own table, or the round's, by a random 0-15 */
    mff_cycles(0x91BF, 31);
    RAM(OBJ_SPAWN_GROUP + y) = a;
    RAM(TMP2) = x;
    a = RAM(RNG);
    c = 0;
    ADC_A(RAM(RNG + 1));
    RAM(RNG) = a;
    x = a & 0x0F;
    a = RAM(OBJ_ANIM + y);
    c = a >= ANIM_B7;
    if (a == ANIM_B7) {
        mff_cycles(0x91D3, 2 + 17);             /* BEQ across a page */
        a = RAM(RNG + 2);
        ADC_A(RAM(RNG + 1));
        RAM(RNG + 2) = a;
        x = a & 0x0F;
        a = prg_read_ix(K_LIFE_B7, x);
        SET_NZ(a);
    } else {
        /* rounds past 4 read round 0's table; a 0 in a table would read
         * the next round's (none holds one) */
        mff_cycles(0x91D5, 3);
        a = RAM(ROUND);
        for (r = 0; r < 5; r++) {
            mff_cycles((uint16_t)(0x91D7 + 4 * r), 4);
            c = a >= r;
            if (a == r) {
                mff_cycles((uint16_t)(0x91D9 + 4 * r), 1);
                break;
            }
        }
        if (r == 5) r = 0;
        for (; r < 5; r++) {
            mff_cycles((uint16_t)(0x91EB + 5 * r), 4);
            a = prg_read_ix(K_LIFE_ROUND0 + 16 * r, x);
            SET_NZ(a);
            if (r == 4) break;
            mff_cycles((uint16_t)(0x91EE + 5 * r), 2);
            if (a != 0) {
                mff_cycles((uint16_t)(0x91EE + 5 * r), 2);  /* BNE across a page */
                break;
            }
        }
    }
    mff_cycles(0x9202, 16);
    RAM(OBJ_LIFE + y) = a;
    RAM(OBJ_LIFE_MAX + y) = a;
    SYNC_OUT;
    mff_rts();
}

/*
 * timed_spawn_r3a1, $921C (bank 4; proposed name): round 4 area 2's spawner
 * (round 3, sub_area 1): an enemy drops from y $30 at a random x of the
 * first screen, and counts as a kill already (area_kills + 1). Which of
 * two pairs it comes from depends on the spawn groups up, as in
 * timed_spawn; then timed_spawn_r3a1_set.
 * Called by: timed_spawn (JMP at $9053).
 */
void func_921C_b4(void) {
    REGS_IN;
    unsigned i;

    mff_cycles(0x921C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x121E, func_F71A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x921F, 2);
    if (c) {                                    /* no free slot: the RTS at $921B */
        mff_cycles(0x921F, 1 + 6);
        SYNC_OUT;
        mff_rts();
        return;
    }

    mff_cycles(0x9221, 149);
    RAM(TMP0) = y;
    RAM(AREA_KILLS)++;
    RAM(OBJ_DIR + y) = RAM(PLAYER_DIR);
    for (i = 0; i < N_OF(k_timed_cleared); i++)
        RAM(k_timed_cleared[i] + y) = 0x00;
    RAM(OBJ_VY_FRAC + y) = 0xC0;
    RAM(OBJ_VY + y) = 0xFF;
    a = (uint8_t)(RAM(RNG) + RAM(RNG + 1) + c);    /* ADC without CLC: C is 0 */
    RAM(RNG) = a;
    x = a & 0x07;
    a = prg_read_ix(K_DROP_X_R3A1, x);
    mff_cycles(0x927E, 19);
    RAM(OBJ_X + y) = a;
    RAM(OBJ_Y + y) = 0x30;
    RAM(OBJ_X_HI + y) = 0x00;

    y = gather_spawn_groups(0x928B);
    mff_cycles(0x92A2, 19);
    x = 0x02;
    a = RAM(SPAWN_GROUPS);
    c = 0;
    ADC_A(RAM(SPAWN_GROUPS + 1));
    RAM(TMP1) = a;
    for (i = 0; i < N_OF(k_pick_r3a1); i++) {
        if (i) mff_cycles((uint16_t)(0x92AD + 4 * i), 4);
        if (a == k_pick_r3a1[i].sum) break;
    }
    if (i == N_OF(k_pick_r3a1)) {
        c = a >= 0xFF;
        mff_cycles(0x92C9, 16);
        a = 0x00;
        SET_NZ(a);
        RAM(SPAWN_GROUPS) = a;
        RAM(SPAWN_GROUPS + 1) = a;
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles((uint16_t)(0x92AF + 4 * i), 1);
    if (k_pick_r3a1[i].pick) {
        mff_cycles(0x92D2, i == N_OF(k_pick_r3a1) - 1 ? 5 : 8);
        RAM(TMP1) = k_pick_r3a1[i].pick;
    }

    /* 0-2: a random one of a pair (0 and anything above 1 read the first) */
    mff_cycles(0x92E8, 24);
    c = 1;
    RAM(TMP1) = (uint8_t)(0x0C - RAM(TMP1));
    a = (uint8_t)(0x05 - RAM(TMP1));
    RAM(TMP1) = a;
    if (a != 0) {
        mff_cycles(0x92FA, 4);
        if (a == 1) {
            mff_cycles(0x92FC, 2 + 19);         /* BEQ across a page */
            a = RAM(RNG + 1);
            c = 0;
            ADC_A(RAM(RNG + 2));
            RAM(RNG + 1) = a;
            x = a & 0x01;
            a = prg_read_ix(K_R3A1_SET_B, x);
            mff_cycles(0x931C, 2);
            x = a;
            SYNC_OUT;
            func_931D_b4();
            return;
        }
    } else {
        mff_cycles(0x92F8, 1);
    }
    mff_cycles(0x92FE, 19);
    a = RAM(RNG);
    c = 0;
    ADC_A(RAM(RNG + 1));
    RAM(RNG) = a;
    x = a & 0x01;
    a = prg_read_ix(K_R3A1_SET_A, x);
    mff_cycles(0x930B, 5);
    x = a;
    SET_NZ(x);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x931D, 2);
    call_by_address_tail(0x931D, 4);
}

/*
 * timed_spawn_r3a1_set, $931D (bank 4; proposed name): enemy X of round 4
 * area 2's list into slot $00: animation, hidden at first (flags $D4), id,
 * kind, spawn group, life.
 * Called by: timed_spawn_r3a1 (JMP at $930C, falling in at $931D).
 */
void func_931D_b4(void) {
    REGS_IN;

    mff_cycles(0x931D, 7);
    y = RAM(TMP0);
    a = prg_read_ix(K_R3A1_ANIM, x);
    SET_NZ(a);
    mff_cycles(0x9322, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1324, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9325, 11);
    RAM(OBJ_FLAGS + y) = OBJF_ON_SCREEN | OBJF_FACE_RIGHT | OBJF_WORLD | OBJF_HIDDEN;
    a = prg_read_ix(K_R3A1_ID, x);
    mff_cycles(0x932D, 14);
    RAM(OBJ_ID + y) = a;
    RAM(OBJ_SAVED_ID + y) = a;
    a = prg_read_ix(K_R3A1_KIND, x);
    mff_cycles(0x9336, 9);
    RAM(OBJ_KIND + y) = a;
    a = prg_read_ix(K_R3A1_GROUP, x);
    mff_cycles(0x933C, 12);
    RAM(OBJ_SPAWN_GROUP + y) = a;
    RAM(TMP2) = x;
    a = prg_read_ix(K_R3A1_LIFE, x);
    SET_NZ(a);
    mff_cycles(0x9344, 16);
    RAM(OBJ_LIFE + y) = a;
    RAM(OBJ_LIFE_MAX + y) = a;
    SYNC_OUT;
    mff_rts();
}
