/*
 * objects.c - the object slots (docs/engine.md, "Objects")
 *
 * 16 slots (0 = the player, 1-15 enemies, items and effects), one table of
 * 16 bytes per field from $0300 to $05FF, indexed by slot.
 */
#include "mff.h"
#include "state.h"

/* the fields object_clear zeroes, in its order ($0300 is obj_state); $0480
 * gets $FF */
static const uint16_t k_slot_fields[] = {
    OBJ_STATE, OBJ_SAVED_STATE, OBJ_FLAGS, OBJ_HIT_COOLDOWN, OBJ_ID, OBJ_SAVED_ID, OBJ_ATTACK, OBJ_ATTACK_DAMAGE, OBJ_LIFE,
    OBJ_LIFE_MAX, OBJ_INVULN, OBJ_HIT_PLAYER, OBJ_HIT_TIMER, OBJ_HELD, OBJ_KIND, OBJ_TIER, OBJ_AI_TIMER, OBJ_AI_COUNT,
    OBJ_AI_FLAG, OBJ_SPAWN_GROUP, OBJ_TIMER, OBJ_TIMER2, OBJ_GROUND_Y, OBJ_LAND_Y, OBJ_TIMER3, OBJ_DOWN_TIMER,
};

/* the shorter list objects_clear_all zeroes for every slot */
static const uint16_t k_all_fields[] = {
    OBJ_STATE, OBJ_SAVED_STATE, OBJ_FLAGS, OBJ_THROW_FLAG, OBJ_HIT_COOLDOWN, OBJ_ID, OBJ_SAVED_ID, OBJ_ATTACK, OBJ_ATTACK_DAMAGE,
    OBJ_LIFE, OBJ_LIFE_MAX,
};

/* object_clear, $F2BE: slot X. Returns A = $FF, X kept. */
void func_F2BE(void) {
    unsigned i;
    uint8_t x = g_cpu.X;

    mff_cycles(0xF2BE, 2 + 26 * 5 + 2 + 5 + 6);
    for (i = 0; i < sizeof k_slot_fields / sizeof k_slot_fields[0]; i++)
        RAM(k_slot_fields[i] + x) = 0;
    RAM(OBJ_SPAWN_INDEX + x) = 0xFF;
    g_cpu.A = 0xFF;
    mff_nz(0xFF);
    mff_rts();
}

/* objects_clear_all, $C334: every slot, $0F down to 0. Note $0470 here and
 * not in object_clear, and object_clear's later fields left alone. Returns
 * A = $FF, X = $FF. */
void func_C334(void) {
    unsigned i;
    int x;

    mff_cycles(0xC334, 2 + 16 * 68 + 15 + 6);
    for (x = 15; x >= 0; x--) {
        for (i = 0; i < sizeof k_all_fields / sizeof k_all_fields[0]; i++)
            RAM(k_all_fields[i] + x) = 0;
        RAM(OBJ_SPAWN_INDEX + x) = 0xFF;
    }
    g_cpu.A = 0xFF;
    g_cpu.X = 0xFF;
    mff_nz(0xFF);
    mff_rts();
}

/* clear_ram_400_5ff, $C844: zeroes $0400-$05FF through the pointer at $00.
 * Returns $00/$01 = $0600, A = 0, X = $FF, Y = 0. */
void func_C844(void) {
    unsigned i;

    /* per page: 256 x (STA (zp),Y + INY + BNE) + 255 taken, then INC, DEX, BPL */
    mff_cycles(0xC844, 16 + 2 * (256 * 10 + 255 + 5 + 2 + 2) + 1 + 6);
    for (i = OBJ_VX_FRAC; i < 0x0600; i++)
        RAM(i) = 0;
    RAM(TMP0) = 0x00;
    RAM(TMP1) = 0x06;
    g_cpu.A = 0;
    g_cpu.X = 0xFF;
    g_cpu.Y = 0;
    mff_nz(0xFF);
    mff_rts();
}
