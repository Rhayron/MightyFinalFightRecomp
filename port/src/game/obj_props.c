/*
 * obj_props.c - the stage objects of bank $05 (ids $60-$6F): the behaviour
 * table at $A700 and the props of the rounds. The weapons (ids $66/$67,
 * $69/$6A, $6C/$6D) are in obj_weapons.c. docs/objects.md, "Ids".
 *
 * Who is who (stage list: bank 08's per-type tables $8442/$85E6, listed by
 * area through $81FE-$8285; logs: runs/p4/base_*.flog; sprites:
 * runs/assets/sprites/objNN.png):
 *   $60  nothing (RTS); no list entry and no code makes it
 *   $61  hidden food: list type $1C, round 5 area 2 (entry 3, x $01E8, y
 *        $7E), hidden (flags $D4), animation $61. Punch it and health is
 *        refilled. Alive in every tour through that area, never punched
 *   $62  the elevator's floor indicator (B1-B4), made by the elevator
 *   $63  the lights that slide up the elevator shaft, made by the elevator
 *   $64  the elevator of round 4 area 2 (list type $1F, entry 0, hidden):
 *        shakes the screen, cycles background palettes 2-3, sends lights
 *   $65  round 1 area 3 (list type $1B, entry 2): when Thrasher (slot $0C,
 *        id $80, kind 7 in the logs) drops under $10 of life, background
 *        palette 2 starts color script 7. No route gets him that low
 *        while it lives
 *   $68  Thrasher's look-in at the end of round 1 area 2 (list type $19,
 *        entry 3 at x $039A; animation $E4 is Thrasher's, list type $13).
 *        No route reaches that screen without the stage skip
 *   $6B  Andore's charge in the exit scene of round 1 area 2 (made in slot
 *        $0D by bank 00, 00:9297, with Andore's walk, animation $B7). No
 *        route plays the scene
 *   $6E  the boss talk: an invisible object (animation $6B is blank) at
 *        the start of each boss area (list type $18) that starts the talk
 *        pages of gameplay_special_screen (flow_talk.c)
 *   $6F  the GO sign (put up by stage_events, 08:8987)
 * The rest are weapons ($66 $67 $69 $6A $6C $6D) or the stubs of $6C/$6D.
 */
#include "obj_props.h"

/* an RTS at `pc` (its 6 cycles). Returns. */
#define RTS_AT(pc) do { mff_cycles(pc, 6); RTS_RETURN(); } while (0)

#define SFX_FLOOR           0x30    /* the elevator's floor chime */
#define SFX_GO              0x31    /* the GO sign's chime */
#define SFX_HIDDEN_FOOD     0x22
#define SFX_ANDORE          0x2A

/* ---- the behaviour table: id $60 + k runs the JMP at $A700 + 3k ---- */

/* JMP target: 3 cycles, 2 more at the target (a tail call by address) */
static void prop_jump(uint16_t pc, uint16_t target)
{
    mff_cycles(pc, 3);
    nes_cpu_instruction_boundary(target, 2);
    call_by_address_tail(target, 2);
}

void func_A700_b2(void) { prop_jump(0xA700, 0xA730); }  /* $60: nothing */
void func_A703_b2(void) { prop_jump(0xA703, 0xA731); }  /* $61: hidden food */
void func_A706_b2(void) { prop_jump(0xA706, 0xAD58); }  /* $62: floor indicator */
void func_A709_b2(void) { prop_jump(0xA709, 0xAD37); }  /* $63: shaft light */
void func_A70C_b2(void) { prop_jump(0xA70C, 0xABE1); }  /* $64: elevator */
void func_A70F_b2(void) { prop_jump(0xA70F, 0xABC4); }  /* $65: Thrasher's low-life palette */
void func_A712_b2(void) { prop_jump(0xA712, 0xA916); }  /* $66: dropped weapon */
void func_A715_b2(void) { prop_jump(0xA715, 0xA916); }  /* $67: dropped weapon (slot 1) */
void func_A718_b2(void) { prop_jump(0xA718, 0xAB08); }  /* $68: Thrasher's look-in */
void func_A71B_b2(void) { prop_jump(0xA71B, 0xA7A5); }  /* $69: weapon on the floor */
void func_A71E_b2(void) { prop_jump(0xA71E, 0xA7A5); }  /* $6A: Guy's weapon on the floor */
void func_A721_b2(void) { prop_jump(0xA721, 0xAB41); }  /* $6B: Andore's charge */
void func_A724_b2(void) { prop_jump(0xA724, 0xA8A8); }  /* $6C: thrown weapon (no route) */
void func_A727_b2(void) { prop_jump(0xA727, 0xA8A8); }  /* $6D: thrown weapon */
void func_A72A_b2(void) { prop_jump(0xA72A, 0xA9E4); }  /* $6E: boss talk */
void func_A72D_b2(void) { prop_jump(0xA72D, 0xAA82); }  /* $6F: GO sign */

/*
 * $A730: id $60, nothing (RTS).
 */
void func_A730_b2(void)
{
    mff_cycles(0xA730, 6);
    mff_rts();
}

/* ---- id $61: hidden food ---- */

/* the full health by level, per player_id: pointers to $A790 (Cody: $40
 * $48 $50 $58 $60 $68), $A796 (Guy: $30 ... $58), $A79C (Haggar: $50 $58
 * $60 $68 $68 $68); the same numbers as obj_items.c's LEVEL_HEALTH */
#define FULL_HEALTH_LO      0xA78A
#define FULL_HEALTH_HI      0xA78D
#define FOOD_HEALTH         0xFCC0  /* by food kind; kind 1 gives $7F */
#define HIDDEN_FOOD_KIND    0x01

/*
 * hidden_food, $A731 (name proposed): behaviour of id $61. obj_dir toward
 * the player (0F:F314); when the player is level with it or below by less
 * than $0C (0F:F365), $11-$17 pixels to its right (0F:F374), faces it
 * (player_dir is not obj_dir) and throws his first punch (obj_anim of slot
 * 0 = ANIM_FIRST_PUNCH[player_id]): sound $22, health + $FCC0[1] up to the
 * level's full health (nothing at full health), and the slot is freed.
 * In: X = slot. Out: the RTS.
 * Caller: $A703 (id $61).
 */
void func_A731_b2(void)
{
    REGS_IN;

    mff_cycles(0xA731, 6);
    CALL_FIXED(0xA731, func_F314);              /* obj_dir toward the player */
    mff_cycles(0xA734, 6);
    CALL_FIXED(0xA734, func_F365);              /* A = |dy|, C = the player not above */
    mff_cycles(0xA737, 2);
    if (!c) {
        mff_cycles(0xA737, 1);
        RTS_AT(0xA789);
    }
    mff_cycles(0xA739, 4);
    CMP_FLAGS(a, 0x0C);
    if (c) {
        mff_cycles(0xA73B, 1);
        RTS_AT(0xA789);
    }
    mff_cycles(0xA73D, 6);
    CALL_FIXED(0xA73D, func_F374);              /* A = |dx|, C = the player to the right */
    mff_cycles(0xA740, 4);
    CMP_FLAGS(a, 0x18);
    if (c) {
        mff_cycles(0xA742, 1);
        RTS_AT(0xA789);
    }
    mff_cycles(0xA744, 4);
    CMP_FLAGS(a, 0x11);
    if (!c) {
        mff_cycles(0xA746, 1);
        RTS_AT(0xA789);
    }
    mff_cycles(0xA748, 9);
    a = RAM(OBJ_DIR + x);
    CMP_FLAGS(a, RAM(PLAYER_DIR));
    if (z) {                                    /* the player looks away */
        mff_cycles(0xA74D, 1);
        RTS_AT(0xA789);
    }
    mff_cycles(0xA74F, 11);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    CMP_FLAGS(a, prg_read((uint16_t)(ANIM_FIRST_PUNCH + y)));
    mff_cycles(0xA757, 2);
    if (!z) {
        mff_cycles(0xA757, 1);
        RTS_AT(0xA789);
    }

    /* found */
    mff_cycles(0xA759, 8);
    a = SFX_HIDDEN_FOOD;
    SET_NZ(a);
    CALL_FIXED(0xA75B, func_F34C);
    mff_cycles(0xA75E, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(FULL_HEALTH_LO + y));
    mff_cycles(0xA763, 7);
    RAM(TMP2) = a;
    a = prg_read((uint16_t)(FULL_HEALTH_HI + y));
    mff_cycles(0xA768, 14);
    RAM(TMP3) = a;
    y = RAM(PLAYER_LEVEL);
    a = RAM(HEALTH);
    CMP_FLAGS(a, mem_read((uint16_t)(RAM16(TMP2) + y)));
    mff_cycles(0xA770, 2);
    if (!z) {
        mff_cycles(0xA772, 11);
        y = HIDDEN_FOOD_KIND;
        a = RAM(HEALTH);
        c = 0;
        ADC_A(prg_read((uint16_t)(FOOD_HEALTH + y)));
        mff_cycles(0xA77A, 11);
        RAM(HEALTH) = a;
        y = RAM(PLAYER_LEVEL);
        a = mem_read((uint16_t)(RAM16(TMP2) + y));
        mff_cycles(0xA780, 5);
        CMP_FLAGS(a, RAM(HEALTH));
        if (!c) {                               /* past full: full */
            mff_cycles(0xA784, 3);
            RAM(HEALTH) = a;
        } else {
            mff_cycles(0xA782, 1);
        }
    } else {
        mff_cycles(0xA770, 1);                  /* already full */
    }
    mff_cycles(0xA786, 6);
    CALL_FIXED(0xA786, func_F2BE);              /* object_clear */
    RTS_AT(0xA789);
}

/* ---- id $64: the elevator of round 4 area 2, and its $62 and $63 ---- */

#define ELEVATOR_START      0x2D    /* frames before it starts */
#define LIGHT_PERIOD        0x87    /* frames between two pairs of lights */
#define ELEVATOR_PAL_SCRIPT 0x0C    /* color scripts $0C/$0D on background palettes 2/3 */
#define ANIM_LIGHT          0x79
#define ANIM_FLOOR          0x78    /* frames B1, B2, B3, B4 (obj62.png) */
#define ID_LIGHT            0x63
#define ID_FLOOR            0x62
#define LIGHT_START_Y       0xC4
#define LIGHT_TOP_Y         0x2F    /* a light above this line is gone */
#define LIGHT_X             0xACDF  /* by light: $F7, $17 */
#define FLOOR_X             0x81
#define FLOOR_Y             0xAC

/* the fields a light or the floor indicator get cleared ($AC8E-$ACAE,
 * $ACF2-$AD12), in order: obj_vy_frac last */
static const uint16_t k_spawn_cleared[] = {
    OBJ_LIFE, OBJ_LIFE_MAX, OBJ_THROW_FLAG, OBJ_HIT_COOLDOWN, OBJ_INVULN, OBJ_HIT_PLAYER,
    OBJ_HIT_TIMER, OBJ_HELD, OBJ_ATTACK, OBJ_ATTACK_DAMAGE, OBJ_VY_FRAC,
};

/* $AC82-$ACC1 / $ACE6-$AD23: slot Y on the player's x high byte and page,
 * the fields cleared, obj_vy = vy, no spawn group, obj_id = id; A = $00 */
static void elevator_spawn_fields(uint8_t slot, uint8_t vy, uint8_t id)
{
    unsigned i;

    RAM(OBJ_X_HI + slot) = RAM(OBJ_X_HI);
    RAM(OBJ_Y_HI + slot) = RAM(OBJ_Y_HI);
    for (i = 0; i < sizeof k_spawn_cleared / sizeof k_spawn_cleared[0]; i++)
        RAM(k_spawn_cleared[i] + slot) = 0x00;
    RAM(OBJ_VY + slot) = vy;
    RAM(OBJ_SPAWN_GROUP + slot) = 0xFF;
    RAM(OBJ_ID + slot) = id;
}

/*
 * elevator, $ABE1 (name proposed): behaviour of id $64 (round 4 area 2).
 * Phase 0: obj_land_y 0, obj_timer $2D, obj_ground_y $87. Phase 1 counts
 * obj_timer down; at 0 background palettes 2 and 3 run color scripts $0C
 * and $0D (bg_pal_anim_*), and it sends two lights ($AC7D) and the floor
 * indicator ($ACE1). Phase 2: while the area is skipped (game_action $0E)
 * it stops the color scripts; otherwise it shakes the screen (scroll_y -1
 * on even obj_timer2, +1 on odd: $AC58 after a JMP or by falling in) and
 * sends two lights every $87 frames (obj_ground_y).
 * In: X = slot. Out: the RTS, or as $AC58.
 * Caller: $A70C (id $64).
 */
void func_ABE1_b2(void)
{
    REGS_IN;

    mff_cycles(0xABE1, 8);
    a = RAM(OBJ_STATE + x) & OBJ_PHASE;
    if (a == 0) {
        mff_cycles(0xABE8, 26);
        RAM(OBJ_LAND_Y + x) = a;
        RAM(OBJ_TIMER + x) = ELEVATOR_START;
        a = LIGHT_PERIOD;
        RAM(OBJ_GROUND_Y + x) = a;
        RAM(OBJ_STATE + x)++;
    } else {
        mff_cycles(0xABE6, 1);
    }
    mff_cycles(0xABF8, 8);
    a = RAM(OBJ_STATE + x) & 0x02;
    if (a == 0) {
        /* phase 1: waiting to start */
        mff_cycles(0xABFF, 6);
        a = RAM(OBJ_TIMER + x);
        if (a == 0) {
            mff_cycles(0xAC04, 61);
            RAM(BG_PAL_ANIM_ON + 2) = 0x80;
            RAM(BG_PAL_ANIM_ON + 3) = 0x80;
            RAM(BG_PAL_ANIM_SCRIPT + 2) = ELEVATOR_PAL_SCRIPT;
            RAM(BG_PAL_ANIM_SCRIPT + 3) = ELEVATOR_PAL_SCRIPT + 1;
            RAM(BG_PAL_ANIM_STEP + 2) = 0x00;
            RAM(BG_PAL_ANIM_TIMER + 2) = 0x00;
            RAM(BG_PAL_ANIM_STEP + 3) = 0x00;
            RAM(BG_PAL_ANIM_TIMER + 3) = 0x00;
            RAM(OBJ_STATE + x)++;
            a = 0x01;                           /* two lights: index 1, then 0 */
            SET_NZ(a);
            RAM(TMP0) = a;
            RAM(TMP1) = x;
            CALL_PROP(0xAC2D, 0xAC7D);
            mff_cycles(0xAC30, 9);
            x = RAM(TMP1);
            SET_NZ(x);
            CALL_PROP(0xAC32, 0xACE1);
            RTS_AT(0xAC35);
        }
        mff_cycles(0xAC02, 1);
        mff_cycles(0xAC36, 13);
        RAM(OBJ_TIMER + x)--;
        SET_NZ(RAM(OBJ_TIMER + x));
        RTS_RETURN();
    }

    /* phase 2: running */
    mff_cycles(0xABFD, 2);                      /* BNE across a page */
    mff_cycles(0xAC3A, 7);
    a = RAM(GAME_ACTION);
    c = a >= GA_SKIP_STAGE;
    if (a == GA_SKIP_STAGE) {
        mff_cycles(0xAC3E, 1);
        mff_cycles(0xAC74, 16);
        a = 0x00;
        SET_NZ(a);
        RAM(BG_PAL_ANIM_ON + 2) = a;
        RAM(BG_PAL_ANIM_ON + 3) = a;
        RTS_RETURN();
    }
    mff_cycles(0xAC40, 8);
    a = RAM(OBJ_TIMER2 + x) & 0x01;
    if (a == 0) {
        mff_cycles(0xAC47, 10);
        a = RAM(SCROLL_Y);
        c = 1;
        SBC_A(0x01);
        SET_NZ(a);
        RAM(SCROLL_Y) = a;
        JMP_PROP(0xAC4E, 0xAC58);
    }
    mff_cycles(0xAC45, 1);
    mff_cycles(0xAC51, 10);
    a = RAM(SCROLL_Y);
    c = 0;
    ADC_A(0x01);
    RAM(SCROLL_Y) = a;
    SYNC_OUT;
    func_AC58_b2();                             /* falls into it */
}

/*
 * elevator_step, $AC58 (name proposed): obj_timer2 - 1 (the shake's
 * parity) and obj_ground_y - 1; at 0 it is $87 again and two more lights
 * go up ($AC7D).
 * In: X = slot. Out: the RTS (X kept).
 * Callers: the elevator's JMP at $AC4E and its fall-through.
 */
void func_AC58_b2(void)
{
    REGS_IN;

    mff_cycles(0xAC58, 20);
    RAM(OBJ_TIMER2 + x)--;
    RAM(OBJ_GROUND_Y + x)--;
    a = RAM(OBJ_GROUND_Y + x);
    SET_NZ(a);
    if (z) {
        mff_cycles(0xAC63, 21);
        RAM(OBJ_GROUND_Y + x) = LIGHT_PERIOD;
        a = 0x01;
        SET_NZ(a);
        RAM(TMP0) = a;
        RAM(TMP1) = x;
        CALL_PROP(0xAC6E, 0xAC7D);
        mff_cycles(0xAC71, 3);
        x = RAM(TMP1);
        SET_NZ(x);
    } else {
        mff_cycles(0xAC61, 1);
    }
    RTS_AT(0xAC73);
}

/*
 * elevator_lights, $AC7D (name proposed): lights $TMP0 down to 0 (id $63,
 * animation $79) in free slots (0F:F71A; none free: return), at y $C4 and
 * x LIGHT_X[n], rising at obj_vy $0100, flags $D0.
 * In: TMP0 = the last light's index. Out: the RTS; X = $FF, TMP0 = $FF.
 * Callers: the elevator ($AC2D) and elevator_step ($AC6E).
 */
void func_AC7D_b2(void)
{
    REGS_IN;

    for (;;) {
        mff_cycles(0xAC7D, 6);
        CALL_FIXED(0xAC7D, func_F71A);          /* Y = a free slot, C set if none */
        mff_cycles(0xAC80, 2);
        if (c) {
            mff_cycles(0xAC80, 1);
            RTS_AT(0xAC73);
        }
        mff_cycles(0xAC82, 104);
        elevator_spawn_fields(y, 0x01, ID_LIGHT);
        a = ANIM_LIGHT;
        SET_NZ(a);
        CALL_FIXED(0xACC2, func_F2A0);          /* obj_spawn: slot Y, animation A */
        mff_cycles(0xACC5, 21);
        RAM(OBJ_FLAGS + y) = FACE_RIGHT_FLAGS;
        RAM(OBJ_Y + y) = LIGHT_START_Y;
        x = RAM(TMP0);
        a = prg_read((uint16_t)(LIGHT_X + x));
        mff_cycles(0xACD4, 15);
        RAM(OBJ_X + y) = a;
        x--;
        RAM(TMP0) = x;
        a = RAM(TMP0);
        SET_NZ(a);
        if (n)
            break;
        mff_cycles(0xACDC, 1);
    }
    RTS_AT(0xACDE);
}

/*
 * elevator_floor, $ACE1 (name proposed): the floor indicator (id $62,
 * animation $78) in a free slot (none: return) at ($81, $AC), not moving,
 * flags $90.
 * Out: the RTS; Y = its slot.
 * Caller: the elevator ($AC32).
 */
void func_ACE1_b2(void)
{
    REGS_IN;

    mff_cycles(0xACE1, 6);
    CALL_FIXED(0xACE1, func_F71A);
    mff_cycles(0xACE4, 2);
    if (c) {
        mff_cycles(0xACE4, 1);
        RTS_AT(0xAC73);
    }
    mff_cycles(0xACE6, 102);
    elevator_spawn_fields(y, 0x00, ID_FLOOR);
    a = ANIM_FLOOR;
    SET_NZ(a);
    CALL_FIXED(0xAD24, func_F2A0);
    mff_cycles(0xAD27, 21);
    RAM(OBJ_FLAGS + y) = 0x90;
    RAM(OBJ_Y + y) = FLOOR_Y;
    a = FLOOR_X;
    SET_NZ(a);
    RAM(OBJ_X + y) = a;
    RTS_AT(0xAD36);
}

/*
 * shaft_light, $AD37 (name proposed): behaviour of id $63. Below line
 * $2F it shows frame 1 and rises (JMP 0F:F1DF, y -= obj_vy); above it,
 * frame 2 and the slot is freed (JMP object_clear).
 * In: X = slot. Out: as 0F:F1DF or object_clear.
 * Caller: $A709 (id $63).
 */
void func_AD37_b2(void)
{
    REGS_IN;

    mff_cycles(0xAD37, 8);
    a = RAM(OBJ_Y + x);
    c = a >= LIGHT_TOP_Y;
    if (c) {
        mff_cycles(0xAD3E, 14);
        RAM(OBJ_ANIM_FRAME + x) = 1;
        a = 0x00;
        SET_NZ(a);
        RAM(OBJ_ANIM_TIMER + x) = a;
        JMP_FIXED(0xAD48, 0xF1DF, func_F1DF);
    }
    mff_cycles(0xAD3C, 1);
    mff_cycles(0xAD4B, 14);
    RAM(OBJ_ANIM_FRAME + x) = 2;
    a = 0x00;
    SET_NZ(a);
    RAM(OBJ_ANIM_TIMER + x) = a;
    JMP_FIXED(0xAD55, 0xF2BE, func_F2BE);
}

/* the floor chime of floor_indicator: sound $30 */
#define FLOOR_CHIME(pc) do { \
        mff_cycles(pc, 8); \
        a = SFX_FLOOR; \
        SET_NZ(a); \
        CALL_FIXED((pc) + 2, func_F34C); \
    } while (0)

/*
 * floor_indicator, $AD58 (name proposed): behaviour of id $62. The floor
 * follows area_kills: B1 below 5, B2 below $0A, B3 below $0F, then B4
 * (obj_anim_frame 0-3, its timer kept at 0). obj_timer counts the chimes
 * played (sound $30): one at the start and one as each floor is reached.
 * In: X = slot. Out: the RTS.
 * Caller: $A706 (id $62).
 */
void func_AD58_b2(void)
{
    REGS_IN;

    mff_cycles(0xAD58, 8);
    a = RAM(OBJ_STATE + x) & OBJ_PHASE;
    if (a == 0) {
        mff_cycles(0xAD5F, 12);
        RAM(OBJ_TIMER + x) = a;
        RAM(OBJ_STATE + x)++;
    } else {
        mff_cycles(0xAD5D, 1);
    }
    mff_cycles(0xAD65, 6);
    a = RAM(OBJ_TIMER + x);
    if (a == 0) {
        FLOOR_CHIME(0xAD6A);
        mff_cycles(0xAD6F, 7);
        RAM(OBJ_TIMER + x)++;
    } else {
        mff_cycles(0xAD68, 1);
    }

    /* B1 */
    mff_cycles(0xAD72, 7);
    a = RAM(AREA_KILLS);
    c = a >= 0x05;
    if (!c) {
        mff_cycles(0xAD78, 18);
        a = 0x00;
        SET_NZ(a);
        RAM(OBJ_ANIM_FRAME + x) = a;
        RAM(OBJ_ANIM_TIMER + x) = a;
        RTS_RETURN();
    }
    mff_cycles(0xAD76, 1);

    /* B2 */
    mff_cycles(0xAD81, 8);
    a = RAM(OBJ_TIMER + x);
    c = a >= 0x02;
    if (!c) {
        FLOOR_CHIME(0xAD88);
        mff_cycles(0xAD8D, 7);
        RAM(OBJ_TIMER + x)++;
    } else {
        mff_cycles(0xAD86, 1);
    }
    mff_cycles(0xAD90, 7);
    a = RAM(AREA_KILLS);
    c = a >= 0x0A;
    if (!c) {
        mff_cycles(0xAD96, 9);
        a = 0x01;
        RAM(OBJ_ANIM_FRAME + x) = a;
        mff_cycles(0xAD9B, 1);                  /* BNE $ADCD, always taken */
    } else {
        /* B3 */
        mff_cycles(0xAD94, 1);
        mff_cycles(0xAD9D, 8);
        a = RAM(OBJ_TIMER + x);
        c = a >= 0x03;
        if (!c) {
            FLOOR_CHIME(0xADA4);
            mff_cycles(0xADA9, 7);
            RAM(OBJ_TIMER + x)++;
        } else {
            mff_cycles(0xADA2, 1);
        }
        mff_cycles(0xADAC, 7);
        a = RAM(AREA_KILLS);
        c = a >= 0x0F;
        if (!c) {
            mff_cycles(0xADB2, 9);
            a = 0x02;
            RAM(OBJ_ANIM_FRAME + x) = a;
            mff_cycles(0xADB7, 1);              /* BNE $ADCD, always taken */
        } else {
            /* B4: here the count goes up before the chime */
            mff_cycles(0xADB0, 1);
            mff_cycles(0xADB9, 8);
            a = RAM(OBJ_TIMER + x);
            c = a >= 0x04;
            if (!c) {
                mff_cycles(0xADC0, 15);
                RAM(OBJ_TIMER + x)++;
                a = SFX_FLOOR;
                SET_NZ(a);
                CALL_FIXED(0xADC5, func_F34C);
            } else {
                mff_cycles(0xADBE, 1);
            }
            mff_cycles(0xADC8, 7);
            a = 0x03;
            RAM(OBJ_ANIM_FRAME + x) = a;
        }
    }
    mff_cycles(0xADCD, 13);
    a = 0x00;
    SET_NZ(a);
    RAM(OBJ_ANIM_TIMER + x) = a;
    RTS_RETURN();
}

/* ---- id $65: Thrasher's low-life palette (round 1 area 3) ---- */

#define BOSS_SLOT           0x0C    /* where Thrasher is in round 1 area 3 */
#define BOSS_LOW_LIFE       0x10
#define BOSS_LOW_PAL_SCRIPT 0x07

/*
 * boss_low_palette, $ABC4 (name proposed): behaviour of id $65. When slot
 * $0C's life is under $10, background palette 2 starts color script 7 and
 * the slot is freed.
 * In: X = slot. Out: the RTS.
 * Caller: $A70F (id $65).
 */
void func_ABC4_b2(void)
{
    REGS_IN;

    mff_cycles(0xABC4, 8);
    a = RAM(OBJ_LIFE + BOSS_SLOT);
    CMP_FLAGS(a, BOSS_LOW_LIFE);
    if (!c) {
        mff_cycles(0xABCB, 28);
        RAM(BG_PAL_ANIM_ON + 2) = 0x80;
        RAM(BG_PAL_ANIM_SCRIPT + 2) = BOSS_LOW_PAL_SCRIPT;
        a = 0x00;
        SET_NZ(a);
        RAM(BG_PAL_ANIM_STEP + 2) = a;
        RAM(BG_PAL_ANIM_TIMER + 2) = a;
        CALL_FIXED(0xABDD, func_F2BE);          /* object_clear */
    } else {
        mff_cycles(0xABC9, 1);
    }
    RTS_AT(0xABE0);
}

/* ---- id $68: Thrasher's look-in (round 1 area 2) ---- */

#define ANIM_THRASHER_TURN  0xE5    /* after animation $E4, Thrasher's stand */
#define THRASHER_TURN_CHR   0x28    /* chr_banks+5 for it */
#define THRASHER_WAIT       0x3C

/*
 * thrasher_look_in, $AB08 (name proposed): behaviour of id $68. Phase 0:
 * obj_vx = obj_vy = $0100, obj_timer $3C. It waits obj_timer frames, then
 * takes animation $E5 (0F:F44F, chr_banks+5 = $28) and from the next
 * frame on faces right and walks right (JMP 0F:F183).
 * In: X = slot. Out: the RTS, or as 0F:F183.
 * Caller: $A718 (id $68).
 */
void func_AB08_b2(void)
{
    REGS_IN;

    mff_cycles(0xAB08, 8);
    a = RAM(OBJ_STATE + x) & OBJ_PHASE;
    if (a == 0) {
        mff_cycles(0xAB0F, 36);
        RAM(OBJ_VX_FRAC + x) = a;
        RAM(OBJ_VY_FRAC + x) = a;
        RAM(OBJ_VX + x) = 0x01;
        RAM(OBJ_VY + x) = 0x01;
        RAM(OBJ_STATE + x)++;
        a = THRASHER_WAIT;
        RAM(OBJ_TIMER + x) = a;
    } else {
        mff_cycles(0xAB0D, 1);
    }
    mff_cycles(0xAB25, 6);
    a = RAM(OBJ_TIMER + x);
    if (a != 0) {
        mff_cycles(0xAB28, 1);
        mff_cycles(0xAB37, 13);
        RAM(OBJ_TIMER + x)--;
        SET_NZ(RAM(OBJ_TIMER + x));
        RTS_RETURN();
    }
    mff_cycles(0xAB2A, 8);
    a = ANIM_THRASHER_TURN;
    CMP_FLAGS(a, RAM(OBJ_ANIM + x));
    if (!z) {
        mff_cycles(0xAB31, 8);
        y = THRASHER_TURN_CHR;
        SET_NZ(y);
        CALL_FIXED(0xAB33, func_F44F);          /* obj_set_anim(A), chr_banks+5 = Y */
        RTS_AT(0xAB36);
    }
    mff_cycles(0xAB2F, 1);
    mff_cycles(0xAB3B, 6);
    CALL_FIXED(0xAB3B, func_F090);              /* face right */
    JMP_FIXED(0xAB3E, 0xF183, func_F183);       /* x += obj_vx */
}

/* ---- id $6B: Andore's charge (the exit scene of round 1 area 2) ---- */

#define ANIM_ANDORE_RUN     0xD2
#define ANIM_ANDORE_CHARGE  0x9A
#define ANIM_ANDORE_WALK    0xB7    /* also the timed spawner's Andores */
#define ANDORE_CHR          0x24    /* chr_banks+3 for the run and the walk */
#define ANDORE_CHARGE_CHR   0x3E
#define ANDORE_CHARGE_X     0x68
#define ANDORE_STOP_X       0x78
/* the player's hit pose by player_id ($28 $29 $2A) and its sprite bank */
#define PLAYER_HIT_ANIM     0xE20A
#define PLAYER_HIT_CHR      0xE252

/*
 * andore_charge, $AB41 (name proposed): behaviour of id $6B. Phase 0:
 * obj_vx $0200, facing right, animation $D2 (0F:F443, chr_banks+3 = $24).
 * Phase 1 runs right; from x $68 animation $9A (chr_banks+3 = $3E),
 * obj_vx $0300 and sound $2A. Phase 2 runs on to x $78, then animation $B7
 * (chr $24) and the player is hit: player_dir right, slot 0's flags $D0,
 * player_set_anim($E20A[player_id]) with its sprite bank. Phase 3: nothing.
 * In: X = slot. Out: the RTS.
 * Caller: $A721 (id $6B).
 */
void func_AB41_b2(void)
{
    REGS_IN;

    mff_cycles(0xAB41, 8);
    a = RAM(OBJ_STATE + x) & OBJ_PHASE;
    if (a == 0) {
        mff_cycles(0xAB48, 32);
        RAM(OBJ_VX_FRAC + x) = a;
        RAM(OBJ_VX + x) = 0x02;
        RAM(OBJ_STATE + x)++;
        a = DIR_RIGHT;
        SET_NZ(a);
        RAM(OBJ_DIR + x) = a;
        CALL_FIXED(0xAB58, func_F090);          /* face right */
        mff_cycles(0xAB5B, 10);
        a = ANIM_ANDORE_RUN;
        y = ANDORE_CHR;
        SET_NZ(y);
        CALL_FIXED(0xAB5F, func_F443);          /* obj_set_anim(A), chr_banks+3 = Y */
    } else {
        mff_cycles(0xAB46, 1);
    }
    mff_cycles(0xAB62, 10);
    a = RAM(OBJ_STATE + x) & OBJ_PHASE;
    CMP_FLAGS(a, 0x02);
    if (!z) {
        mff_cycles(0xAB6B, 4);
        CMP_FLAGS(a, 0x03);
        if (z) {
            mff_cycles(0xAB6D, 1);
            RTS_AT(0xAB92);
        }

        /* phase 1: run to the charge line */
        mff_cycles(0xAB6F, 6);
        CALL_FIXED(0xAB6F, func_F183);
        mff_cycles(0xAB72, 8);
        a = RAM(OBJ_X + x);
        CMP_FLAGS(a, ANDORE_CHARGE_X);
        if (!c) {
            mff_cycles(0xAB77, 1);
            RTS_AT(0xAB92);
        }
        mff_cycles(0xAB79, 8);
        a = ANIM_ANDORE_CHARGE;
        CMP_FLAGS(a, RAM(OBJ_ANIM + x));
        if (z) {
            mff_cycles(0xAB7E, 1);
            RTS_AT(0xAB92);
        }
        mff_cycles(0xAB80, 8);
        y = ANDORE_CHARGE_CHR;
        SET_NZ(y);
        CALL_FIXED(0xAB82, func_F443);
        mff_cycles(0xAB85, 22);
        RAM(OBJ_VX + x) = 0x03;
        RAM(OBJ_STATE + x)++;
        a = SFX_ANDORE;
        SET_NZ(a);
        CALL_FIXED(0xAB8F, func_F34C);
        RTS_AT(0xAB92);
    }

    /* phase 2: the charge */
    mff_cycles(0xAB69, 1);
    mff_cycles(0xAB93, 6);
    CALL_FIXED(0xAB93, func_F183);
    mff_cycles(0xAB96, 8);
    a = RAM(OBJ_X + x);
    CMP_FLAGS(a, ANDORE_STOP_X);
    if (!c) {
        mff_cycles(0xAB9B, 1);
        RTS_AT(0xAB92);
    }
    mff_cycles(0xAB9D, 15);
    RAM(OBJ_STATE + x)++;
    a = ANIM_ANDORE_WALK;
    c = a >= RAM(OBJ_ANIM + x);
    if (a != RAM(OBJ_ANIM + x)) {
        mff_cycles(0xABA7, 8);
        y = ANDORE_CHR;
        SET_NZ(y);
        CALL_FIXED(0xABA9, func_F443);
    } else {
        mff_cycles(0xABA5, 1);
    }
    mff_cycles(0xABAC, 18);
    RAM(PLAYER_DIR) = DIR_RIGHT;
    RAM(OBJ_FLAGS) = FACE_RIGHT_FLAGS;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(PLAYER_HIT_ANIM + y));
    SET_NZ(a);
    mff_cycles(0xABBA, 6);
    CALL_FIXED(0xABBA, func_F294);              /* player_set_anim */
    mff_cycles(0xABBD, 4);
    a = prg_read((uint16_t)(PLAYER_HIT_CHR + y));
    SET_NZ(a);
    mff_cycles(0xABC0, 6);
    CALL_FIXED(0xABC0, func_F440);              /* chr_banks+2 = A */
    RTS_AT(0xABC3);
}

/* ---- id $6E: the boss talk ---- */

/* game_actions that keep the talk from starting ($A9EF-$AA07): jumping,
 * Cody's slash, Haggar's lariat, the special, $18, Haggar's power */
static const uint8_t k_talk_busy[] = { 0x01, 0x14, 0x16, 0x17, 0x18, 0x10 };

/* the player's poses that let it start, by player_id: standing (1 2 3),
 * or $42 $02 $44 */
#define TALK_POSE_1         0xAA7C
#define TALK_POSE_2         0xAA7F
/* the first page: TALK_PAGES[base + player_id], base by round from
 * TALK_BASE_ROUND (0 3 6 9) and in round 5 by area from TALK_BASE_R5
 * ($0C $0F $12). TALK_PAGES: Thrasher $01, Sodom $14, Abigail $26,
 * round 4's boss $33, round 5: $4B, $4D, and Belger $3D/$3F/$40 */
#define TALK_BASE_ROUND     0xAA60
#define TALK_BASE_R5        0xAA64
#define TALK_PAGES          0xAA67
#define ROUND_5             0x04

/*
 * boss_talk, $A9E4 (name proposed): behaviour of id $6E. Phase 0 waits
 * for the camera to stop on a screen (scroll_x 0) with the player free
 * (not in k_talk_busy); then the player faces right and holds still
 * (game_action $0A, jump_moving 0). Phase 1 waits for him to stand
 * (TALK_POSE_1/2), then sets special_screen_id to the area's first page
 * (TALK_PAGES), which makes gameplay run the talk instead of the game.
 * When the talk has set special_screen_id back to 0: camera_lock $FF,
 * game_action 0, attack_end, and the slot is freed.
 * In: X = slot. Out: the RTS.
 * Caller: $A72A (id $6E).
 */
void func_A9E4_b2(void)
{
    REGS_IN;
    int i;

    mff_cycles(0xA9E4, 8);
    a = RAM(OBJ_STATE + x) & OBJ_PHASE;
    if (a == 0) {
        mff_cycles(0xA9EB, 5);
        a = RAM(SCROLL_X);
        SET_NZ(a);
        if (!z) {
            mff_cycles(0xA9ED, 2);              /* BNE across a page */
            RTS_AT(0xAA17);
        }
        mff_cycles(0xA9EF, 3);
        a = RAM(GAME_ACTION);
        i = cmp_list(0xA9F1, 0xAA17, a, k_talk_busy, 6);
        CMP_LIST_FLAGS(a, k_talk_busy, 6, i);
        if (i == 6) {
            mff_cycles(0xAA09, 6);
            CALL_FIXED(0xAA09, func_F03F);      /* player_face_right */
            mff_cycles(0xAA0C, 17);
            RAM(GAME_ACTION) = GA_CUTSCENE;
            a = 0x00;
            RAM(JUMP_MOVING) = a;
            RAM(OBJ_STATE + x)++;
            SET_NZ(RAM(OBJ_STATE + x));
        }
        RTS_AT(0xAA17);
    }

    /* phase 1 and up */
    mff_cycles(0xA9E9, 2);                      /* BNE across a page */
    mff_cycles(0xAA18, 11);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    m = prg_read((uint16_t)(TALK_POSE_1 + y));
    c = a >= m;
    mff_cycles(0xAA20, 2);
    if (a != m) {
        mff_cycles(0xAA22, 4);
        CMP_FLAGS(a, prg_read((uint16_t)(TALK_POSE_2 + y)));
        mff_cycles(0xAA25, 2);
        if (!z)
            RTS_AT(0xAA27);
        mff_cycles(0xAA25, 1);
    } else {
        mff_cycles(0xAA20, 1);
    }
    mff_cycles(0xAA28, 8);
    a = RAM(OBJ_STATE + x) & 0x02;
    if (a == 0) {
        /* phase 1: start the talk */
        mff_cycles(0xAA2F, 7);
        y = RAM(ROUND);
        if (y == ROUND_5) {
            mff_cycles(0xAA35, 7);
            y = RAM(SUB_AREA);
            a = prg_read((uint16_t)(TALK_BASE_R5 + y));
            mff_cycles(0xAA3A, 2);
            if (a != 0) {
                mff_cycles(0xAA3A, 1);
            } else {                            /* never: no 0 in TALK_BASE_R5 */
                mff_cycles(0xAA3C, 4);
                a = prg_read((uint16_t)(TALK_BASE_ROUND + y));
            }
        } else {
            mff_cycles(0xAA33, 1);
            mff_cycles(0xAA3C, 4);
            a = prg_read((uint16_t)(TALK_BASE_ROUND + y));
        }
        mff_cycles(0xAA3F, 11);
        c = 0;
        ADC_A(RAM(PLAYER_ID));
        y = a;
        a = prg_read((uint16_t)(TALK_PAGES + y));
        mff_cycles(0xAA46, 10);
        RAM(SPECIAL_SCREEN_ID) = a;
        RAM(OBJ_STATE + x)++;
    } else {
        mff_cycles(0xAA2D, 1);
    }

    /* the talk is over when special_screen_id is back to 0 */
    mff_cycles(0xAA4B, 5);
    a = RAM(SPECIAL_SCREEN_ID);
    SET_NZ(a);
    if (!z) {
        mff_cycles(0xAA4D, 1);
        RTS_AT(0xAA17);
    }
    mff_cycles(0xAA4F, 19);
    RAM(CAMERA_LOCK) = 0xFF;
    a = 0x00;
    SET_NZ(a);
    RAM(GAME_ACTION) = a;
    RAM(JUMP_MOVING) = a;
    CALL_FIXED(0xAA59, func_F436);              /* attack_end */
    mff_cycles(0xAA5C, 6);
    CALL_FIXED(0xAA5C, func_F2BE);              /* object_clear */
    RTS_AT(0xAA5F);
}

/* ---- id $6F: the GO sign ---- */

#define GO_FRAMES           0xC5    /* shown, then hidden, as long */
#define GO_CHIME_EVERY      0x21
#define GO_SCROLL_DONE      0x28    /* the camera this far into the next screen */
#define ANIM_GO             0x6D
#define ANIM_BLANK          0x6B

/*
 * go_sign, $AA82 (name proposed): behaviour of id $6F, put up by
 * stage_events (08:8987) when an area is cleared. Phase 0: obj_timer and
 * obj_timer2 $C5, the chime counter (obj_ground_y) $20, camera_lock and
 * area_kills 0, attack_end. Once the camera moves (scroll_x not 0)
 * area_clear is 0, and when it is $28 pixels into the next screen:
 * screen_event + 1 (stage_events runs the screen's setup), spawn_groups
 * cleared, every slot $0F-3 but the hidden food's (id $61) freed, and this
 * one with object_clear. Until then it shows GO for $C5 frames with sound
 * $31 every $21, blank (animation $6B) for $C5, and again.
 * In: X = slot. Out: the RTS, or as object_clear.
 * Caller: $A72D (id $6F).
 */
void func_AA82_b2(void)
{
    REGS_IN;

    mff_cycles(0xAA82, 8);
    a = RAM(OBJ_STATE + x) & OBJ_PHASE;
    if (a == 0) {
        mff_cycles(0xAA89, 40);
        RAM(OBJ_TIMER + x) = GO_FRAMES;
        RAM(OBJ_TIMER2 + x) = GO_FRAMES;
        RAM(OBJ_GROUND_Y + x) = 0x20;
        RAM(OBJ_STATE + x)++;
        a = 0x00;
        SET_NZ(a);
        RAM(CAMERA_LOCK) = a;
        RAM(AREA_KILLS) = a;
        CALL_FIXED(0xAA9F, func_F436);          /* attack_end */
    } else {
        mff_cycles(0xAA87, 1);
    }

    /* the player walked on */
    mff_cycles(0xAAA2, 5);
    a = RAM(SCROLL_X);
    if (a != 0) {
        mff_cycles(0xAAA6, 12);
        RAM(AREA_CLEAR) = 0x00;
        a = RAM(SCROLL_X);
        c = a >= GO_SCROLL_DONE;
        if (c) {
            mff_cycles(0xAAB0, 21);
            RAM(SCREEN_EVENT)++;
            a = 0x00;
            RAM(SPAWN_GROUPS) = a;
            RAM(SPAWN_GROUPS + 1) = a;
            RAM(SPAWN_GROUPS + 2) = a;
            y = OBJ_SLOT_LAST;
            for (;;) {
                mff_cycles(0xAABF, 8);
                a = RAM(OBJ_ID + y);
                if (a != ID_HIDDEN_FOOD) {
                    mff_cycles(0xAAC6, 7);
                    a = 0x00;
                    RAM(OBJ_STATE + y) = a;
                } else {
                    mff_cycles(0xAAC4, 1);
                }
                mff_cycles(0xAACB, 6);
                y--;
                CMP_FLAGS(y, 0x02);
                if (z)
                    break;
                mff_cycles(0xAACE, 1);
            }
            JMP_FIXED(0xAAD0, 0xF2BE, func_F2BE);
        }
        mff_cycles(0xAAAE, 1);
    } else {
        mff_cycles(0xAAA4, 1);
    }

    /* showing GO, with its chime */
    mff_cycles(0xAAD3, 6);
    a = RAM(OBJ_TIMER + x);
    if (a != 0) {
        mff_cycles(0xAAD8, 13);
        RAM(OBJ_TIMER + x)--;
        a = RAM(OBJ_GROUND_Y + x);
        if (a == 0) {
            mff_cycles(0xAAE0, 8);
            a = SFX_GO;
            SET_NZ(a);
            CALL_FIXED(0xAAE2, func_F34C);
            mff_cycles(0xAAE5, 7);
            a = GO_CHIME_EVERY;
            RAM(OBJ_GROUND_Y + x) = a;
        } else {
            mff_cycles(0xAADE, 1);
        }
        mff_cycles(0xAAEA, 13);
        RAM(OBJ_GROUND_Y + x)--;
        SET_NZ(RAM(OBJ_GROUND_Y + x));
        RTS_RETURN();
    }
    mff_cycles(0xAAD6, 1);

    /* blank for obj_timer2 frames, then GO again */
    mff_cycles(0xAAEE, 6);
    a = RAM(OBJ_TIMER2 + x);
    if (a != 0) {
        mff_cycles(0xAAF3, 11);
        RAM(OBJ_TIMER2 + x)--;
        a = ANIM_BLANK;
        SET_NZ(a);
        mff_cycles(0xAAF8, 2);                  /* BNE across a page */
    } else {
        mff_cycles(0xAAF1, 1);
        mff_cycles(0xAAFA, 14);
        RAM(OBJ_TIMER + x) = GO_FRAMES;
        RAM(OBJ_TIMER2 + x) = GO_FRAMES;
        a = ANIM_GO;
        SET_NZ(a);
    }
    mff_cycles(0xAB04, 6);
    CALL_FIXED(0xAB04, func_F288);              /* obj_set_anim */
    RTS_AT(0xAB07);
}
