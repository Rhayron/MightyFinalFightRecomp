/*
 * lifted_b1.c - bank 1 routines $8000-$BFFF, lifted by
 * port/tools/lift_bank.py from the pure scaffold: exact, still to be
 * tidied and moved into their systems' files.
 */
#include "mff.h"
#include "state.h"
#include "coroutine.h"

void func_F000(void);
void func_F03F(void);
void func_F048(void);
void func_F051(void);
void func_F090(void);
void func_F099(void);
void func_F0A2(void);
void func_F0F7(void);
void func_F183(void);
void func_F1A1(void);
void func_F1DF(void);
void func_F294(void);
void func_F2A0(void);
void func_F2BE(void);
void func_F343(void);
void func_F34C(void);
void func_F374(void);
void func_F440(void);
void func_F443(void);
void func_F449(void);
void func_F71A(void);
void func_F8D5(void);
void func_FB3E(void);
void func_FB5B(void);
void func_FBD9(void);

/*  $B300 (bank 1) - lifted by port/tools/lift.py */
void func_B300_b1(void) {
    REGS_IN;
L_B300:;
    mff_cycles(0xB300, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB3F4, 2); call_by_address_tail(0xB3F4, 1); return;
}

/*  $B303 (bank 1) - lifted by port/tools/lift.py */
void func_B303_b1(void) {
    REGS_IN;
L_B303:;
    mff_cycles(0xB303, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBB91, 2); call_by_address_tail(0xBB91, 1); return;
}

/*  $B306 (bank 1) - lifted by port/tools/lift.py */
void func_B306_b1(void) {
    REGS_IN;
L_B306:;
    mff_cycles(0xB306, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 1); return;
}

/*  $B309 (bank 1) - lifted by port/tools/lift.py */
void func_B309_b1(void) {
    REGS_IN;
L_B309:;
    mff_cycles(0xB309, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 1); return;
}

/*  $B30C (bank 1) - lifted by port/tools/lift.py */
void func_B30C_b1(void) {
    REGS_IN;
L_B30C:;
    mff_cycles(0xB30C, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBBCB, 2); call_by_address_tail(0xBBCB, 1); return;
}

/*  $B30F (bank 1) - lifted by port/tools/lift.py */
void func_B30F_b1(void) {
    REGS_IN;
L_B30F:;
    mff_cycles(0xB30F, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBD46, 2); call_by_address_tail(0xBD46, 1); return;
}

/*  $B312 (bank 1) - lifted by port/tools/lift.py */
void func_B312_b1(void) {
    REGS_IN;
L_B312:;
    mff_cycles(0xB312, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 1); return;
}

/*  $B315 (bank 1) - lifted by port/tools/lift.py */
void func_B315_b1(void) {
    REGS_IN;
L_B315:;
    mff_cycles(0xB315, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 1); return;
}

/*  $B318 (bank 1) - lifted by port/tools/lift.py */
void func_B318_b1(void) {
    REGS_IN;
L_B318:;
    mff_cycles(0xB318, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 1); return;
}

/*  $B31B (bank 1) - lifted by port/tools/lift.py */
void func_B31B_b1(void) {
    REGS_IN;
L_B31B:;
    mff_cycles(0xB31B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 1); return;
}

/*  $B31E (bank 1) - lifted by port/tools/lift.py */
void func_B31E_b1(void) {
    REGS_IN;
L_B31E:;
    mff_cycles(0xB31E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 1); return;
}

/*  $B321 (bank 1) - lifted by port/tools/lift.py */
void func_B321_b1(void) {
    REGS_IN;
L_B321:;
    mff_cycles(0xB321, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBF38, 2); call_by_address_tail(0xBF38, 1); return;
}

/*  $B324 (bank 1) - lifted by port/tools/lift.py */
void func_B324_b1(void) {
    REGS_IN;
L_B324:;
    mff_cycles(0xB324, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBFBE, 2); call_by_address_tail(0xBFBE, 1); return;
}

/*  $B327 (bank 1) - lifted by port/tools/lift.py */
void func_B327_b1(void) {
    REGS_IN;
L_B327:;
    mff_cycles(0xB327, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBF95, 2); call_by_address_tail(0xBF95, 1); return;
}

/*  $B32A (bank 1) - lifted by port/tools/lift.py */
void func_B32A_b1(void) {
    REGS_IN;
L_B32A:;
    mff_cycles(0xB32A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBF48, 2); call_by_address_tail(0xBF48, 1); return;
}

/*  $B32D (bank 1) - lifted by port/tools/lift.py */
void func_B32D_b1(void) {
    REGS_IN;
L_B32D:;
    mff_cycles(0xB32D, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBED2, 2); call_by_address_tail(0xBED2, 1); return;
}

/*  $B330 (bank 1) - lifted by port/tools/lift.py */
void func_B330_b1(void) {
    REGS_IN;
L_B330:;
    mff_cycles(0xB330, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B331 (bank 1) - lifted by port/tools/lift.py */
void func_B331_b1(void) {
    REGS_IN;
L_B331:;
    mff_cycles(0xB331, 13);
    a = 0x00;
    RAM(0x0F) = a;
    a = RAM(OBJ_DIR + x);
    a &= 0x08;
    z = (a) == 0;
    if (z) { mff_cycles(0xB33A, 1); goto L_B34D; }
    mff_cycles(0xB33C, 8);
    y = 0x01;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1340, func_F0F7, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB341, 2);
    if (!c) { mff_cycles(0xB341, 1); goto L_B363; }
    mff_cycles(0xB343, 7);
    a = RAM(0x10);
    a &= 0x10;
    z = (a) == 0;
    if (z) { mff_cycles(0xB347, 1); goto L_B363; }
    mff_cycles(0xB349, 5);
    RAM(0x0F) = a;
    if (!z) { mff_cycles(0xB34B, 1); goto L_B363; }
L_B34D:;
    mff_cycles(0xB34D, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x04;
    z = (a) == 0;
    if (z) { mff_cycles(0xB352, 1); goto L_B363; }
    mff_cycles(0xB354, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1358, func_F0A2, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB359, 2);
    if (!c) { mff_cycles(0xB359, 1); goto L_B363; }
    mff_cycles(0xB35B, 7);
    a = RAM(0x10);
    a &= 0x10;
    z = (a) == 0;
    if (z) { mff_cycles(0xB35F, 1); goto L_B363; }
    mff_cycles(0xB361, 3);
    RAM(0x0F) = a;
L_B363:;
    mff_cycles(0xB363, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    z = (a) == 0;
    if (z) { mff_cycles(0xB368, 1); goto L_B37B; }
    mff_cycles(0xB36A, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x136E, func_F000, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB36F, 2);
    if (!c) { mff_cycles(0xB36F, 1); goto L_B37B; }
    mff_cycles(0xB371, 7);
    a = RAM(0x10);
    a &= 0x10;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xB375, 1); goto L_B38F; }
    mff_cycles(0xB377, 5);
    RAM(0x0F) = a;
    if (!z) { mff_cycles(0xB379, 1); goto L_B38F; }
L_B37B:;
    mff_cycles(0xB37B, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xB380, 1); goto L_B38F; }
    mff_cycles(0xB382, 8);
    y = 0x01;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1386, func_F051, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB387, 2);
    if (!c) { mff_cycles(0xB387, 1); goto L_B38F; }
    mff_cycles(0xB389, 8);
    a = RAM(0x10);
    a &= 0x10;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(0x0F) = a;
L_B38F:;
    mff_cycles(0xB38F, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B390 (bank 1) - lifted by port/tools/lift.py */
void func_B390_b1(void) {
    REGS_IN;
L_B390:;
    mff_cycles(0xB390, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1392, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB393, 2);
    if (c) { mff_cycles(0xB393, 1); goto L_B398; }
    mff_cycles(0xB395, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF099, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F099(); g_code_window_base = swb; } return;
L_B398:;
    mff_cycles(0xB398, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF090, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F090(); g_code_window_base = swb; } return;
}

/*  $B39B (bank 1) - lifted by port/tools/lift.py */
void func_B39B_b1(void) {
    REGS_IN;
L_B39B:;
    mff_cycles(0xB39B, 4);
    a = nes_read((uint16_t)(0xB3C4 + y));
    mff_cycles(0xB39E, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = nes_read((uint16_t)(0xB3B4 + y));
    mff_cycles(0xB3A4, 9);
    RAM(OBJ_VX + x) = a;
    a = nes_read((uint16_t)(0xB3E4 + y));
    mff_cycles(0xB3AA, 9);
    RAM(OBJ_VY_FRAC + x) = a;
    a = nes_read((uint16_t)(0xB3D4 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB3B0, 11);
    RAM(OBJ_VY + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B3F4 (bank 1) - lifted by port/tools/lift.py */
void func_B3F4_b1(void) {
    REGS_IN;
L_B3F4:;
    mff_cycles(0xB3F4, 6);
    a = RAM(OBJ_INVULN + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xB3F7, 1); goto L_B3FC; }
    mff_cycles(0xB3F9, 7);
    t = (uint8_t)(RAM(OBJ_INVULN + x) - 1); RAM(OBJ_INVULN + x) = t;
L_B3FC:;
    mff_cycles(0xB3FC, 6);
    a = RAM(OBJ_TIER + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xB3FF, 1); goto L_B404; }
    mff_cycles(0xB401, 7);
    t = (uint8_t)(RAM(OBJ_TIER + x) - 1); RAM(OBJ_TIER + x) = t;
L_B404:;
    mff_cycles(0xB404, 22);
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = 0x09;
    RAM(OBJ_KIND + x) = a;
    a = RAM(OBJ_ID + 13);
    m = 0x6E; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB413, 1); goto L_B421; }
    mff_cycles(0xB415, 14);
    a = 0x01;
    RAM(OBJ_INVULN + x) = a;
    a = 0xD9;
    y = 0x24;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF443, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F443(); g_code_window_base = swb; } return;
L_B421:;
    mff_cycles(0xB421, 6);
    a = RAM(OBJ_HELD + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xB424, 1); goto L_B46C; }
    mff_cycles(0xB426, 4);
    m = 0xFF; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB428, 1); goto L_B45B; }
    mff_cycles(0xB42A, 7);
    a = RAM(GAME_ACTION);
    m = 0x04; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB42E, 1); goto L_B434; }
    mff_cycles(0xB430, 4);
    m = 0x21; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB432, 1); goto L_B46C; }
L_B434:;
    mff_cycles(0xB434, 5);
    m = RAM(GRAB_SLOT); c = x >= m;
    z = ((uint8_t)(x - m)) == 0;
    if (!z) { mff_cycles(0xB436, 1); goto L_B46C; }
    mff_cycles(0xB438, 7);
    a = RAM(GAME_ACTION);
    m = 0x21; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB43C, 1); goto L_B444; }
    mff_cycles(0xB43E, 6);
    a = 0xDF;
    y = 0x26;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0xB442, 1); goto L_B448; }
L_B444:;
    mff_cycles(0xB444, 4);
    a = 0xD9;
    y = 0x24;
    n = (y) >> 7;
    z = (y) == 0;
L_B448:;
    mff_cycles(0xB448, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x144A, func_F443, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB44B, 22);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    RAM(OBJ_HIT_TIMER + x) = a;
    a = 0x81;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF343, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F343(); g_code_window_base = swb; } return;
L_B45B:;
    mff_cycles(0xB45B, 14);
    a = 0x00;
    RAM(OBJ_HELD + x) = a;
    a = RAM(PLAYER_ID);
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB464, 1); goto L_B46C; }
    mff_cycles(0xB466, 9);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + x) = a;
L_B46C:;
    mff_cycles(0xB46C, 6);
    a = RAM(OBJ_HIT_TIMER + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xB46F, 1); goto L_B4A6; }
    mff_cycles(0xB471, 4);
    m = 0x1E; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB473, 1); goto L_B47D; }
    mff_cycles(0xB475, 4);
    m = 0x1A; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB477, 1); goto L_B483; }
    mff_cycles(0xB479, 4);
    m = 0x10; c = a >= m;
    if (c) { mff_cycles(0xB47B, 1); goto L_B48A; }
L_B47D:;
    mff_cycles(0xB47D, 6);
    a = 0xD9;
    y = 0x24;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0xB481, 1); goto L_B487; }
L_B483:;
    mff_cycles(0xB483, 4);
    a = 0xDF;
    y = 0x26;
    n = (y) >> 7;
    z = (y) == 0;
L_B487:;
    mff_cycles(0xB487, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1489, func_F443, 0xE000)) return;
    SYNC_IN;
L_B48A:;
    mff_cycles(0xB48A, 7);
    a = RAM(RNG);
    t = a; c = t >> 7; t = (uint8_t)(t << 1); a = t;
    if (!c) { mff_cycles(0xB48D, 1); goto L_B495; }
    mff_cycles(0xB48F, 6);
    a = 0x85;
    y = 0x00;
    z = (y) == 0;
    if (z) { mff_cycles(0xB493, 1); goto L_B499; }
L_B495:;
    mff_cycles(0xB495, 4);
    a = 0x84;
    y = 0x01;
L_B499:;
    mff_cycles(0xB499, 22);
    RAM(OBJ_STATE + x) = a;
    a = y;
    RAM(OBJ_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_HIT_TIMER + x) - 1); RAM(OBJ_HIT_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF343, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F343(); g_code_window_base = swb; } return;
L_B4A6:;
    mff_cycles(0xB4A6, 7);
    a = RAM(GAME_ACTION);
    m = 0x1F; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB4AA, 1); goto L_B4BD; }
    mff_cycles(0xB4AC, 4);
    m = 0x0B; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB4AE, 1); goto L_B4B6; }
    mff_cycles(0xB4B0, 4);
    m = 0x25; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB4B2, 1); goto L_B4E4; }
    mff_cycles(0xB4B4, 2);
    if (!z) { mff_cycles(0xB4B4, 2); goto L_B50A; }
L_B4B6:;
    mff_cycles(0xB4B6, 9);
    a = 0x00;
    z = (a) == 0;
    RAM(OBJ_HIT_PLAYER + x) = a;
    if (z) { mff_cycles(0xB4BB, 2); goto L_B50A; }
L_B4BD:;
    mff_cycles(0xB4BD, 22);
    a = 0x00;
    RAM(OBJ_HIT_PLAYER + x) = a;
    a = 0x18;
    RAM(OBJ_TIER + x) = a;
    a = RAM(OBJ_STATE + x);
    m = 0x82; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB4CC, 2); goto L_B50A; }
    mff_cycles(0xB4CE, 4);
    m = 0x86; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB4D0, 1); goto L_B4D9; }
    mff_cycles(0xB4D2, 7);
    a = 0xD9;
    y = 0x24;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF443, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F443(); g_code_window_base = swb; } return;
L_B4D9:;
    mff_cycles(0xB4D9, 20);
    a = 0x82;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_B4E4:;
    mff_cycles(0xB4E4, 59);
    a = RAM(OBJ_Y + x);
    RAM(OBJ_LAND_Y + x) = a;
    a = 0x8E;
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x03;
    RAM(OBJ_VY + x) = a;
    a = RAM(OBJ_ID + x);
    RAM(OBJ_SAVED_ID + x) = a;
    a = 0x05;
    RAM(OBJ_ID + x) = a;
    a = 0x80;
    RAM(OBJ_STATE + x) = a;
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_INVULN + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_B50A:;
    mff_cycles(0xB50A, 10);
    a = RAM(OBJ_X_HI);
    m = RAM(OBJ_X_HI + x); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB510, 1); goto L_B515; }
    mff_cycles(0xB512, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBA53, 2); call_by_address_tail(0xBA53, 1); return;
L_B515:;
    mff_cycles(0xB515, 8);
    a = RAM(OBJ_X + x);
    m = 0xF2; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xB51A, 1); goto L_B520; }
    mff_cycles(0xB51C, 4);
    m = 0x16; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xB51E, 1); goto L_B523; }
L_B520:;
    mff_cycles(0xB520, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBA53, 2); call_by_address_tail(0xBA53, 1); return;
L_B523:;
    mff_cycles(0xB523, 12);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    y = a;
    a = nes_read((uint16_t)(0xB536 + y));
    mff_cycles(0xB52C, 7);
    RAM(0x00) = a;
    a = nes_read((uint16_t)(0xB53E + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB531, 8);
    RAM(0x01) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $B546 (bank 1) - lifted by port/tools/lift.py */
void func_B546_b1(void) {
    REGS_IN;
L_B546:;
    mff_cycles(0xB546, 27);
    a = 0x00;
    RAM(OBJ_AI_TIMER + x) = a;
    RAM(OBJ_AI_COUNT + x) = a;
    RAM(OBJ_TIMER + x) = a;
    a = 0x22;
    y = 0x09;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1557, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB558, 2);
    if (!c) { mff_cycles(0xB558, 1); goto L_B571; }
    mff_cycles(0xB55A, 14);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = RAM(GAME_ACTION);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB563, 1); goto L_B569; }
    mff_cycles(0xB565, 4);
    a = 0x83;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xB567, 1); goto L_B56B; }
L_B569:;
    mff_cycles(0xB569, 2);
    a = 0x87;
    n = (a) >> 7;
    z = (a) == 0;
L_B56B:;
    mff_cycles(0xB56B, 8);
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF343, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F343(); g_code_window_base = swb; } return;
L_B571:;
    mff_cycles(0xB571, 14);
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1); RAM(OBJ_STATE + x) = t;
    a = 0xD9;
    y = 0x24;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF443, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F443(); g_code_window_base = swb; } return;
}

/*  $B57B (bank 1) - lifted by port/tools/lift.py */
void func_B57B_b1(void) {
    REGS_IN;
L_B57B:;
    mff_cycles(0xB57B, 8);
    a = 0xDA;
    m = RAM(OBJ_ANIM + x); c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB580, 1); goto L_B587; }
    mff_cycles(0xB582, 8);
    y = 0x24;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1586, func_F443, 0xE000)) return;
    SYNC_IN;
L_B587:;
    mff_cycles(0xB587, 7);
    a = RAM(GAME_ACTION);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB58B, 1); goto L_B59F; }
    mff_cycles(0xB58D, 10);
    a = 0x22;
    y = 0x08;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1593, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB594, 2);
    if (!c) { mff_cycles(0xB594, 1); goto L_B59F; }
    mff_cycles(0xB596, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1598, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB599, 13);
    a = 0x87;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_B59F:;
    mff_cycles(0xB59F, 6);
    a = RAM(OBJ_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xB5A2, 1); goto L_B5A7; }
    mff_cycles(0xB5A4, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB615, 2); call_by_address_tail(0xB615, 1); return;
L_B5A7:;
    mff_cycles(0xB5A7, 10);
    a = 0x30;
    y = 0x10;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x15AD, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB5AE, 2);
    if (!c) { mff_cycles(0xB5AE, 1); goto L_B5DC; }
    mff_cycles(0xB5B0, 21);
    a = 0x00;
    RAM(OBJ_AI_FLAG + x) = a;
    RAM(OBJ_AI_TIMER + x) = a;
    a = RAM(RNG);
    a &= 0x0F;
    m = 0x06; c = a >= m;
    if (c) { mff_cycles(0xB5BE, 1); goto L_B5C6; }
    mff_cycles(0xB5C0, 10);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB615, 2); call_by_address_tail(0xB615, 1); return;
L_B5C6:;
    mff_cycles(0xB5C6, 4);
    m = 0x0C; c = a >= m;
    if (c) { mff_cycles(0xB5C8, 1); goto L_B5D3; }
    mff_cycles(0xB5CA, 17);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    t = (uint8_t)(RAM(OBJ_AI_FLAG + x) - 1); RAM(OBJ_AI_FLAG + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB615, 2); call_by_address_tail(0xB615, 1); return;
L_B5D3:;
    mff_cycles(0xB5D3, 20);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1); RAM(OBJ_STATE + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_B5DC:;
    mff_cycles(0xB5DC, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x15DE, func_FBD9, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB5DF, 4);
    m = 0x08; c = a >= m;
    if (!c) { mff_cycles(0xB5E1, 1); goto L_B5FC; }
    mff_cycles(0xB5E3, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x15E7, 0xB39B, 1)) return;
    SYNC_IN;
    mff_cycles(0xB5E8, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x15EA, func_FBD9, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB5EB, 2);
    if (c) { mff_cycles(0xB5EB, 1); goto L_B5F1; }
    mff_cycles(0xB5ED, 4);
    a = 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xB5EF, 1); goto L_B5F3; }
L_B5F1:;
    mff_cycles(0xB5F1, 2);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
L_B5F3:;
    mff_cycles(0xB5F3, 11);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x15F8, 0xB331, 1)) return;
    SYNC_IN;
    mff_cycles(0xB5F9, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB390, 2); call_by_address_tail(0xB390, 1); return;
L_B5FC:;
    mff_cycles(0xB5FC, 8);
    y = 0x04;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x1600, 0xB39B, 1)) return;
    SYNC_IN;
    mff_cycles(0xB601, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1603, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB604, 2);
    if (c) { mff_cycles(0xB604, 1); goto L_B60A; }
    mff_cycles(0xB606, 4);
    a = 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xB608, 1); goto L_B60C; }
L_B60A:;
    mff_cycles(0xB60A, 2);
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
L_B60C:;
    mff_cycles(0xB60C, 11);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1611, 0xB331, 1)) return;
    SYNC_IN;
    mff_cycles(0xB612, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB390, 2); call_by_address_tail(0xB390, 1); return;
}

/*  $B615 (bank 1) - lifted by port/tools/lift.py */
void func_B615_b1(void) {
    REGS_IN;
L_B615:;
    mff_cycles(0xB615, 6);
    a = RAM(OBJ_AI_FLAG + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xB618, 1); goto L_B620; }
    mff_cycles(0xB61A, 6);
    a = 0x22;
    y = 0x09;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0xB61E, 1); goto L_B624; }
L_B620:;
    mff_cycles(0xB620, 4);
    a = 0x25;
    y = 0x0C;
    n = (y) >> 7;
    z = (y) == 0;
L_B624:;
    mff_cycles(0xB624, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1626, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB627, 2);
    if (!c) { mff_cycles(0xB627, 1); goto L_B643; }
    mff_cycles(0xB629, 7);
    a = RAM(GAME_ACTION);
    m = 0x06; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB62D, 1); goto L_B643; }
    mff_cycles(0xB62F, 6);
    a = RAM(OBJ_AI_FLAG + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xB632, 1); goto L_B638; }
    mff_cycles(0xB634, 4);
    a = 0x83;
    z = (a) == 0;
    if (!z) { mff_cycles(0xB636, 1); goto L_B63A; }
L_B638:;
    mff_cycles(0xB638, 2);
    a = 0x85;
L_B63A:;
    mff_cycles(0xB63A, 18);
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_B643:;
    mff_cycles(0xB643, 6);
    a = RAM(OBJ_AI_TIMER + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xB646, 1); goto L_B64E; }
    mff_cycles(0xB648, 10);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1); RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB660, 2); call_by_address_tail(0xB660, 1); return;
L_B64E:;
    mff_cycles(0xB64E, 13);
    a = 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x1655, func_FB5B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB656, 8);
    y = a;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x1659, 0xB39B, 1)) return;
    SYNC_IN;
    mff_cycles(0xB65A, 4);
    a = nes_read((uint16_t)(0xFBF0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB65D, 11);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1662, 0xB331, 1)) return;
    SYNC_IN;
    mff_cycles(0xB663, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB390, 2); call_by_address_tail(0xB390, 1); return;
}

/*  $B660 (bank 1) - lifted by port/tools/lift.py */
void func_B660_b1(void) {
    REGS_IN;
L_B660:;
    mff_cycles(0xB660, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1662, 0xB331, 1)) return;
    SYNC_IN;
    mff_cycles(0xB663, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB390, 2); call_by_address_tail(0xB390, 1); return;
}

/*  $B666 (bank 1) - lifted by port/tools/lift.py */
void func_B666_b1(void) {
    REGS_IN;
L_B666:;
    mff_cycles(0xB666, 8);
    a = 0xDA;
    m = RAM(OBJ_ANIM + x); c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB66B, 1); goto L_B672; }
    mff_cycles(0xB66D, 8);
    y = 0x24;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1671, func_F443, 0xE000)) return;
    SYNC_IN;
L_B672:;
    mff_cycles(0xB672, 6);
    a = RAM(OBJ_TIMER + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xB675, 1); goto L_B6D0; }
    mff_cycles(0xB677, 9);
    a = RAM(RNG);
    a &= 0x0F;
    m = 0x05; c = a >= m;
    if (c) { mff_cycles(0xB67D, 1); goto L_B686; }
    mff_cycles(0xB67F, 9);
    a = 0x03;
    z = (a) == 0;
    RAM(OBJ_DIR + x) = a;
    if (!z) { mff_cycles(0xB684, 1); goto L_B69B; }
L_B686:;
    mff_cycles(0xB686, 4);
    m = 0x0B; c = a >= m;
    if (c) { mff_cycles(0xB688, 1); goto L_B696; }
    mff_cycles(0xB68A, 8);
    y = 0x04;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x168E, 0xB39B, 1)) return;
    SYNC_IN;
    mff_cycles(0xB68F, 9);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DIR + x) = a;
    if (!z) { mff_cycles(0xB694, 1); goto L_B6A0; }
L_B696:;
    mff_cycles(0xB696, 7);
    a = 0x05;
    RAM(OBJ_DIR + x) = a;
L_B69B:;
    mff_cycles(0xB69B, 8);
    y = 0x05;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x169F, 0xB39B, 1)) return;
    SYNC_IN;
L_B6A0:;
    mff_cycles(0xB6A0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x16A2, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB6A3, 2);
    if (!c) { mff_cycles(0xB6A3, 1); goto L_B6AD; }
    mff_cycles(0xB6A5, 11);
    a = RAM(OBJ_DIR + x);
    m = 0x07; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(OBJ_DIR + x) = a;
L_B6AD:;
    mff_cycles(0xB6AD, 8);
    y = RAM(OBJ_DIR + x);
    a = nes_read((uint16_t)(0xFBF0 + y));
    mff_cycles(0xB6B3, 12);
    RAM(OBJ_DIR + x) = a;
    a = RAM(RNG + 1);
    a &= 0x07;
    z = (a) == 0;
    if (!z) { mff_cycles(0xB6BA, 1); goto L_B6C0; }
    mff_cycles(0xB6BC, 4);
    a = 0x30;
    z = (a) == 0;
    if (!z) { mff_cycles(0xB6BE, 1); goto L_B6C9; }
L_B6C0:;
    mff_cycles(0xB6C0, 4);
    t = a; c = t & 1; t >>= 1; a = t;
    if (c) { mff_cycles(0xB6C1, 1); goto L_B6C7; }
    mff_cycles(0xB6C3, 4);
    a = 0x48;
    z = (a) == 0;
    if (!z) { mff_cycles(0xB6C5, 1); goto L_B6C9; }
L_B6C7:;
    mff_cycles(0xB6C7, 2);
    a = 0x60;
L_B6C9:;
    mff_cycles(0xB6C9, 18);
    RAM(OBJ_AI_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_B6D0:;
    mff_cycles(0xB6D0, 8);
    a = RAM(OBJ_X + x);
    m = 0xF1; c = a >= m;
    if (c) { mff_cycles(0xB6D5, 1); goto L_B6EA; }
    mff_cycles(0xB6D7, 4);
    m = 0x17; c = a >= m;
    if (!c) { mff_cycles(0xB6D9, 1); goto L_B6EA; }
    mff_cycles(0xB6DB, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1); RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (z) { mff_cycles(0xB6DE, 1); goto L_B6EA; }
    mff_cycles(0xB6E0, 6);
    SYNC_OUT;
    if (!mff_call_at(0x16E2, 0xB331, 1)) return;
    SYNC_IN;
    mff_cycles(0xB6E3, 6);
    SYNC_OUT;
    if (!mff_call_at(0x16E5, 0xB390, 1)) return;
    SYNC_IN;
    mff_cycles(0xB6E6, 5);
    a = RAM(0x0F);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xB6E8, 1); goto L_B6F4; }
L_B6EA:;
    mff_cycles(0xB6EA, 14);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x86;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
L_B6F4:;
    mff_cycles(0xB6F4, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B6F5 (bank 1) - lifted by port/tools/lift.py */
void func_B6F5_b1(void) {
    REGS_IN;
L_B6F5:;
    mff_cycles(0xB6F5, 6);
    a = RAM(OBJ_TIMER + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xB6F8, 2); goto L_B701; }
    mff_cycles(0xB6FA, 4);
    m = 0x01; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB6FC, 2); goto L_B71F; }
    mff_cycles(0xB6FE, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB7BD, 2); call_by_address_tail(0xB7BD, 1); return;
L_B701:;
    mff_cycles(0xB701, 14);
    a = 0x00;
    RAM(OBJ_TIMER2 + x) = a;
    a = RAM(RNG);
    a &= 0x07;
    z = (a) == 0;
    if (z) { mff_cycles(0xB70A, 1); goto L_B711; }
    mff_cycles(0xB70C, 7);
    a = 0x02;
    RAM(OBJ_AI_COUNT + x) = a;
L_B711:;
    mff_cycles(0xB711, 24);
    t = (uint8_t)(RAM(OBJ_AI_COUNT + x) + 1); RAM(OBJ_AI_COUNT + x) = t;
    a = 0x00;
    RAM(OBJ_AI_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF343, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F343(); g_code_window_base = swb; } return;
L_B71F:;
    mff_cycles(0xB71F, 6);
    a = RAM(OBJ_AI_TIMER + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xB722, 1); goto L_B744; }
    mff_cycles(0xB724, 11);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1); RAM(OBJ_AI_TIMER + x) = t;
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB729, 1); goto L_B73D; }
    mff_cycles(0xB72B, 5);
    m = RAM(GAME_ACTION); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB72D, 1); goto L_B73D; }
    mff_cycles(0xB72F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1731, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB732, 20);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x87;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_B73D:;
    mff_cycles(0xB73D, 7);
    a = 0xD9;
    y = 0x24;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF443, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F443(); g_code_window_base = swb; } return;
L_B744:;
    mff_cycles(0xB744, 8);
    a = 0xDC;
    m = RAM(OBJ_ANIM + x); c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB749, 1); goto L_B753; }
    mff_cycles(0xB74B, 8);
    y = 0x25;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x174F, func_F443, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB750, 7);
    t = (uint8_t)(RAM(OBJ_ANIM_TIMER + x) + 1); RAM(OBJ_ANIM_TIMER + x) = t;
L_B753:;
    mff_cycles(0xB753, 6);
    a = RAM(OBJ_ANIM_FRAME + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xB756, 1); goto L_B75F; }
    mff_cycles(0xB758, 8);
    a = RAM(OBJ_ANIM_TIMER + x);
    m = 0x07; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB75D, 1); goto L_B789; }
L_B75F:;
    mff_cycles(0xB75F, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB764, 1); goto L_B779; }
    mff_cycles(0xB766, 13);
    a = 0x8C;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_TIMER2 + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xB76E, 1); goto L_B774; }
    mff_cycles(0xB770, 4);
    a = 0x07;
    z = (a) == 0;
    if (!z) { mff_cycles(0xB772, 1); goto L_B776; }
L_B774:;
    mff_cycles(0xB774, 2);
    a = 0x0F;
L_B776:;
    mff_cycles(0xB776, 5);
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
L_B779:;
    mff_cycles(0xB779, 6);
    a = RAM(OBJ_HIT_PLAYER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xB77C, 1); goto L_B788; }
    mff_cycles(0xB77E, 8);
    a = 0x18;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1782, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB783, 7);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_HIT_PLAYER + x) = a;
L_B788:;
    mff_cycles(0xB788, 6);
    SYNC_OUT;
    mff_rts(); return;
L_B789:;
    mff_cycles(0xB789, 9);
    t = (uint8_t)(RAM(OBJ_AI_COUNT + x) - 1); RAM(OBJ_AI_COUNT + x) = t;
    z = (t) == 0;
    if (!z) { mff_cycles(0xB78C, 1); goto L_B797; }
    mff_cycles(0xB78E, 20);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    a = 0x10;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_B797:;
    mff_cycles(0xB797, 10);
    a = 0x22;
    y = 0x09;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x179D, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB79E, 2);
    if (c) { mff_cycles(0xB79E, 1); goto L_B7A9; }
    mff_cycles(0xB7A0, 20);
    t = (uint8_t)(RAM(OBJ_STATE + x) - 1); RAM(OBJ_STATE + x) = t;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_B7A9:;
    mff_cycles(0xB7A9, 8);
    a = RAM(OBJ_AI_COUNT + x);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB7AE, 1); goto L_B7B5; }
    mff_cycles(0xB7B0, 7);
    a = 0xFF;
    RAM(OBJ_TIMER2 + x) = a;
L_B7B5:;
    mff_cycles(0xB7B5, 10);
    a = 0x12;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF343, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F343(); g_code_window_base = swb; } return;
}

/*  $B7BD (bank 1) - lifted by port/tools/lift.py */
void func_B7BD_b1(void) {
    REGS_IN;
    goto L_B7BD;
L_B7A0:;
    mff_cycles(0xB7A0, 20);
    t = (uint8_t)(RAM(OBJ_STATE + x) - 1); RAM(OBJ_STATE + x) = t;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_B7BD:;
    mff_cycles(0xB7BD, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1); RAM(OBJ_AI_TIMER + x) = t;
    z = (t) == 0;
    if (z) { mff_cycles(0xB7A0, 1); goto L_B7A0; }
    mff_cycles(0xB7C2, 7);
    a = 0xD9;
    y = 0x24;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF443, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F443(); g_code_window_base = swb; } return;
}

/*  $B7C9 (bank 1) - lifted by port/tools/lift.py */
void func_B7C9_b1(void) {
    REGS_IN;
L_B7C9:;
    mff_cycles(0xB7C9, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x01; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB7CE, 1); goto L_B7F2; }
    mff_cycles(0xB7D0, 4);
    m = 0x02; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB7D2, 2); goto L_B802; }
    mff_cycles(0xB7D4, 4);
    m = 0x03; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB7D6, 2); goto L_B827; }
    mff_cycles(0xB7D8, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x17DA, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB7DB, 8);
    a = 0xD9;
    m = RAM(OBJ_ANIM + x); c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB7E0, 1); goto L_B7EA; }
    mff_cycles(0xB7E2, 8);
    y = 0x24;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x17E6, func_F443, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB7E7, 7);
    t = (uint8_t)(RAM(OBJ_ANIM_TIMER + x) + 1); RAM(OBJ_ANIM_TIMER + x) = t;
L_B7EA:;
    mff_cycles(0xB7EA, 10);
    a = RAM(OBJ_ANIM_FRAME + x);
    a |= RAM(OBJ_ANIM_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xB7F0, 2); goto L_B801; }
L_B7F2:;
    mff_cycles(0xB7F2, 6);
    SYNC_OUT;
    if (!mff_call_at(0x17F4, 0xBB29, 1)) return;
    SYNC_IN;
    mff_cycles(0xB7F5, 10);
    a = 0xDD;
    y = 0x24;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x17FB, func_F443, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB7FC, 7);
    a = 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
L_B801:;
    mff_cycles(0xB801, 6);
    SYNC_OUT;
    mff_rts(); return;
L_B802:;
    mff_cycles(0xB802, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1804, 0xB331, 1)) return;
    SYNC_IN;
    mff_cycles(0xB805, 8);
    a = RAM(OBJ_X + x);
    m = 0xF1; c = a >= m;
    if (c) { mff_cycles(0xB80A, 1); goto L_B817; }
    mff_cycles(0xB80C, 4);
    m = 0x17; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xB80E, 1); goto L_B817; }
    mff_cycles(0xB810, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1812, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB813, 4);
    m = 0x30; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xB815, 1); goto L_B826; }
L_B817:;
    mff_cycles(0xB817, 10);
    a = 0xDE;
    y = 0x25;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x181D, func_F443, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB81E, 14);
    a = 0x0E;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
L_B826:;
    mff_cycles(0xB826, 6);
    SYNC_OUT;
    mff_rts(); return;
L_B827:;
    mff_cycles(0xB827, 6);
    a = RAM(OBJ_VX + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xB82A, 1); goto L_B82E; }
    mff_cycles(0xB82C, 2);
    a = 0x8B;
L_B82E:;
    mff_cycles(0xB82E, 63);
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_VX_FRAC + x);
    c = 1;
    m = RAM(OBJ_TIMER2 + x); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    RAM(OBJ_VX_FRAC + x) = a;
    a = RAM(OBJ_VX + x);
    m = 0x00; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    RAM(OBJ_VX + x) = a;
    a = RAM(OBJ_VY_FRAC + x);
    c = 1;
    m = RAM(OBJ_GROUND_Y + x); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    RAM(OBJ_VY_FRAC + x) = a;
    a = RAM(OBJ_VY + x);
    m = 0x00; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    RAM(OBJ_VY + x) = a;
    a = RAM(OBJ_VX + x);
    n = (a) >> 7;
    if (n) { mff_cycles(0xB858, 1); goto L_B863; }
    mff_cycles(0xB85A, 6);
    a = RAM(OBJ_VY + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) { mff_cycles(0xB85D, 1); goto L_B863; }
    mff_cycles(0xB85F, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1861, 0xB331, 1)) return;
    SYNC_IN;
    mff_cycles(0xB862, 6);
    SYNC_OUT;
    mff_rts(); return;
L_B863:;
    mff_cycles(0xB863, 37);
    a = 0x81;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    RAM(OBJ_TIMER2 + x) = a;
    RAM(OBJ_GROUND_Y + x) = a;
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B879 (bank 1) - lifted by port/tools/lift.py */
void func_B879_b1(void) {
    REGS_IN;
L_B879:;
    mff_cycles(0xB879, 6);
    a = RAM(OBJ_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xB87C, 1); goto L_B881; }
    mff_cycles(0xB87E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB925, 2); call_by_address_tail(0xB925, 1); return;
L_B881:;
    mff_cycles(0xB881, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1883, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB884, 6);
    a = RAM(OBJ_TIER + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xB887, 1); goto L_B8BA; }
    mff_cycles(0xB889, 5);
    a = RAM(PLAYER_NO_HIT);
    n = (a) >> 7;
    if (n) { mff_cycles(0xB88B, 1); goto L_B8BA; }
    mff_cycles(0xB88D, 7);
    a = RAM(GAME_ACTION);
    m = 0x0B; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB891, 1); goto L_B8BA; }
    mff_cycles(0xB893, 4);
    m = 0x25; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB895, 1); goto L_B8BA; }
    mff_cycles(0xB897, 4);
    m = 0x16; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB899, 1); goto L_B8BA; }
    mff_cycles(0xB89B, 4);
    m = 0x17; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB89D, 1); goto L_B8BA; }
    mff_cycles(0xB89F, 4);
    m = 0x1F; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB8A1, 1); goto L_B8BA; }
    mff_cycles(0xB8A3, 8);
    a = RAM(OBJ_FLAGS + x);
    a &= 0x40;
    z = (a) == 0;
    if (z) { mff_cycles(0xB8A8, 1); goto L_B8B3; }
    mff_cycles(0xB8AA, 8);
    a = RAM(OBJ_X + x);
    m = 0xED; c = a >= m;
    if (c) { mff_cycles(0xB8AF, 1); goto L_B8BA; }
    mff_cycles(0xB8B1, 2);
    if (!c) { mff_cycles(0xB8B1, 1); goto L_B8C5; }
L_B8B3:;
    mff_cycles(0xB8B3, 8);
    a = RAM(OBJ_X + x);
    m = 0x13; c = a >= m;
    if (c) { mff_cycles(0xB8B8, 1); goto L_B8C5; }
L_B8BA:;
    mff_cycles(0xB8BA, 20);
    a = 0x82;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_B8C5:;
    mff_cycles(0xB8C5, 9);
    a = RAM(OBJ_Y);
    m = RAM(PLAYER_GROUND_Y); c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB8CA, 1); goto L_B8E1; }
    mff_cycles(0xB8CC, 7);
    a = RAM(GAME_ACTION);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB8D0, 1); goto L_B8D6; }
    mff_cycles(0xB8D2, 4);
    a = 0x83;
    z = (a) == 0;
    if (!z) { mff_cycles(0xB8D4, 1); goto L_B8D8; }
L_B8D6:;
    mff_cycles(0xB8D6, 2);
    a = 0x87;
L_B8D8:;
    mff_cycles(0xB8D8, 18);
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_B8E1:;
    mff_cycles(0xB8E1, 34);
    a = 0x00;
    RAM(OBJ_STATE + 1) = a;
    RAM(OBJ_STATE + 2) = a;
    a = 0xFF;
    RAM(OBJ_INVULN + x) = a;
    a = RAM(OBJ_Y + x);
    RAM(OBJ_TIMER2 + x) = a;
    a = RAM(OBJ_DIR + x);
    t = a; c = t & 1; t >>= 1; a = t;
    if (c) { mff_cycles(0xB8F8, 1); goto L_B8FE; }
    mff_cycles(0xB8FA, 4);
    a = 0x01;
    z = (a) == 0;
    if (!z) { mff_cycles(0xB8FC, 2); goto L_B900; }
L_B8FE:;
    mff_cycles(0xB8FE, 2);
    a = 0x02;
L_B900:;
    mff_cycles(0xB900, 11);
    RAM(PLAYER_DIR) = a;
    a = RAM(OBJ_FLAGS + x);
    a &= 0x40;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xB907, 1); goto L_B90F; }
    mff_cycles(0xB909, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x190B, func_F048, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB90C, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB912, 2); call_by_address_tail(0xB912, 1); return;
L_B90F:;
    mff_cycles(0xB90F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1911, func_F03F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB912, 26);
    a = 0x0A;
    RAM(GAME_ACTION) = a;
    a = 0x03;
    RAM(OBJ_AI_COUNT + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    a = 0x91;
    y = 0x3E;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF443, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F443(); g_code_window_base = swb; } return;
}

/*  $B912 (bank 1) - lifted by port/tools/lift.py */
void func_B912_b1(void) {
    REGS_IN;
L_B912:;
    mff_cycles(0xB912, 26);
    a = 0x0A;
    RAM(GAME_ACTION) = a;
    a = 0x03;
    RAM(OBJ_AI_COUNT + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    a = 0x91;
    y = 0x3E;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF443, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F443(); g_code_window_base = swb; } return;
}

/*  $B925 (bank 1) - lifted by port/tools/lift.py */
void func_B925_b1(void) {
    REGS_IN;
L_B925:;
    mff_cycles(0xB925, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB92A, 1); goto L_B93A; }
    mff_cycles(0xB92C, 7);
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE207 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB931, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1933, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB934, 4);
    a = nes_read((uint16_t)(0xE24F + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB937, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1939, func_F440, 0xE000)) return;
    SYNC_IN;
L_B93A:;
    mff_cycles(0xB93A, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x03; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB93F, 1); goto L_B977; }
    mff_cycles(0xB941, 13);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    a = RAM(OBJ_ANIM_TIMER + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xB947, 1); goto L_B977; }
    mff_cycles(0xB949, 8);
    a = 0x1F;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x194D, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB94E, 9);
    t = (uint8_t)(RAM(OBJ_AI_COUNT + x) - 1); RAM(OBJ_AI_COUNT + x) = t;
    z = (t) == 0;
    if (z) { mff_cycles(0xB951, 1); goto L_B962; }
    mff_cycles(0xB953, 18);
    a = 0x07;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = 0x3C;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PLAYER_INVULN) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x195E, func_F8D5, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB95F, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB977, 2); call_by_address_tail(0xB977, 1); return;
L_B962:;
    mff_cycles(0xB962, 18);
    a = 0x0F;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = 0x3C;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PLAYER_INVULN) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x196D, func_F8D5, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB96E, 11);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0x06;
    RAM(GAME_ACTION) = a;
L_B977:;
    mff_cycles(0xB977, 7);
    a = RAM(GAME_ACTION);
    m = 0x06; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB97B, 1); goto L_B9AC; }
    mff_cycles(0xB97D, 14);
    a = RAM(OBJ_TIMER2 + x);
    RAM(PLAYER_GROUND_Y) = a;
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE20A + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB987, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1989, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB98A, 4);
    a = nes_read((uint16_t)(0xE252 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB98D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x198F, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB990, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x04; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB995, 1); goto L_B9AB; }
    mff_cycles(0xB997, 8);
    a = RAM(OBJ_ANIM_TIMER + x);
    m = 0x05; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB99C, 1); goto L_B9AB; }
    mff_cycles(0xB99E, 19);
    a = 0x81;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    RAM(OBJ_INVULN + x) = a;
L_B9AB:;
    mff_cycles(0xB9AB, 6);
    SYNC_OUT;
    mff_rts(); return;
L_B9AC:;
    mff_cycles(0xB9AC, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB9B1, 1); goto L_B9CB; }
    mff_cycles(0xB9B3, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x04; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB9B8, 1); goto L_B9CB; }
    mff_cycles(0xB9BA, 8);
    a = RAM(OBJ_ANIM_TIMER + x);
    m = 0x05; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB9BF, 1); goto L_B9CB; }
    mff_cycles(0xB9C1, 14);
    a = 0x01;
    RAM(OBJ_ANIM_FRAME + x) = a;
    a = 0x00;
    RAM(OBJ_ANIM_TIMER + x) = a;
L_B9CB:;
    mff_cycles(0xB9CB, 20);
    a = RAM(OBJ_ANIM_FRAME + x);
    c = 0;
    m = 0x01; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    t = a; c = t & 1; t >>= 1; a = t;
    y = a;
    a = RAM(OBJ_FLAGS + x);
    a &= 0x40;
    z = (a) == 0;
    if (z) { mff_cycles(0xB9D8, 1); goto L_B9EF; }
    mff_cycles(0xB9DA, 10);
    a = RAM(OBJ_X + x);
    c = 0;
    m = nes_read((uint16_t)(0xBA16 + y)); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    mff_cycles(0xB9E1, 17);
    RAM(OBJ_X) = a;
    a = RAM(OBJ_X_HI + x);
    m = 0x00; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X_HI) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBA01, 2); call_by_address_tail(0xBA01, 1); return;
L_B9EF:;
    mff_cycles(0xB9EF, 10);
    a = RAM(OBJ_X + x);
    c = 1;
    m = nes_read((uint16_t)(0xBA16 + y)); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    mff_cycles(0xB9F6, 24);
    RAM(OBJ_X) = a;
    a = RAM(OBJ_X_HI + x);
    m = 0x00; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    RAM(OBJ_X_HI) = a;
    a = RAM(OBJ_Y + x);
    c = 1;
    m = nes_read((uint16_t)(0xBA19 + y)); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    mff_cycles(0xBA08, 23);
    RAM(OBJ_Y) = a;
    a = 0x00;
    RAM(ATTACK_LIVE) = a;
    RAM(COMBO_TIMER) = a;
    a = 0x0A;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B977 (bank 1) - lifted by port/tools/lift.py */
void func_B977_b1(void) {
    REGS_IN;
L_B977:;
    mff_cycles(0xB977, 7);
    a = RAM(GAME_ACTION);
    m = 0x06; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB97B, 1); goto L_B9AC; }
    mff_cycles(0xB97D, 14);
    a = RAM(OBJ_TIMER2 + x);
    RAM(PLAYER_GROUND_Y) = a;
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE20A + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB987, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1989, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB98A, 4);
    a = nes_read((uint16_t)(0xE252 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB98D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x198F, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB990, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x04; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB995, 1); goto L_B9AB; }
    mff_cycles(0xB997, 8);
    a = RAM(OBJ_ANIM_TIMER + x);
    m = 0x05; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB99C, 1); goto L_B9AB; }
    mff_cycles(0xB99E, 19);
    a = 0x81;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    RAM(OBJ_INVULN + x) = a;
L_B9AB:;
    mff_cycles(0xB9AB, 6);
    SYNC_OUT;
    mff_rts(); return;
L_B9AC:;
    mff_cycles(0xB9AC, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB9B1, 1); goto L_B9CB; }
    mff_cycles(0xB9B3, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x04; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB9B8, 1); goto L_B9CB; }
    mff_cycles(0xB9BA, 8);
    a = RAM(OBJ_ANIM_TIMER + x);
    m = 0x05; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB9BF, 1); goto L_B9CB; }
    mff_cycles(0xB9C1, 14);
    a = 0x01;
    RAM(OBJ_ANIM_FRAME + x) = a;
    a = 0x00;
    RAM(OBJ_ANIM_TIMER + x) = a;
L_B9CB:;
    mff_cycles(0xB9CB, 20);
    a = RAM(OBJ_ANIM_FRAME + x);
    c = 0;
    m = 0x01; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    t = a; c = t & 1; t >>= 1; a = t;
    y = a;
    a = RAM(OBJ_FLAGS + x);
    a &= 0x40;
    z = (a) == 0;
    if (z) { mff_cycles(0xB9D8, 1); goto L_B9EF; }
    mff_cycles(0xB9DA, 10);
    a = RAM(OBJ_X + x);
    c = 0;
    m = nes_read((uint16_t)(0xBA16 + y)); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    mff_cycles(0xB9E1, 17);
    RAM(OBJ_X) = a;
    a = RAM(OBJ_X_HI + x);
    m = 0x00; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X_HI) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBA01, 2); call_by_address_tail(0xBA01, 1); return;
L_B9EF:;
    mff_cycles(0xB9EF, 10);
    a = RAM(OBJ_X + x);
    c = 1;
    m = nes_read((uint16_t)(0xBA16 + y)); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    mff_cycles(0xB9F6, 24);
    RAM(OBJ_X) = a;
    a = RAM(OBJ_X_HI + x);
    m = 0x00; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    RAM(OBJ_X_HI) = a;
    a = RAM(OBJ_Y + x);
    c = 1;
    m = nes_read((uint16_t)(0xBA19 + y)); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    mff_cycles(0xBA08, 23);
    RAM(OBJ_Y) = a;
    a = 0x00;
    RAM(ATTACK_LIVE) = a;
    RAM(COMBO_TIMER) = a;
    a = 0x0A;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BA01 (bank 1) - lifted by port/tools/lift.py */
void func_BA01_b1(void) {
    REGS_IN;
L_BA01:;
    mff_cycles(0xBA01, 10);
    a = RAM(OBJ_Y + x);
    c = 1;
    m = nes_read((uint16_t)(0xBA19 + y)); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    mff_cycles(0xBA08, 23);
    RAM(OBJ_Y) = a;
    a = 0x00;
    RAM(ATTACK_LIVE) = a;
    RAM(COMBO_TIMER) = a;
    a = 0x0A;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BA1C (bank 1) - lifted by port/tools/lift.py */
void func_BA1C_b1(void) {
    REGS_IN;
L_BA1C:;
    mff_cycles(0xBA1C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1A1E, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBA1F, 4);
    m = 0x30; c = a >= m;
    if (c) { mff_cycles(0xBA21, 1); goto L_BA27; }
    mff_cycles(0xBA23, 4);
    a = 0x04;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBA25, 1); goto L_BA39; }
L_BA27:;
    mff_cycles(0xBA27, 4);
    m = 0x48; c = a >= m;
    if (c) { mff_cycles(0xBA29, 1); goto L_BA2F; }
    mff_cycles(0xBA2B, 4);
    a = 0x08;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBA2D, 1); goto L_BA39; }
L_BA2F:;
    mff_cycles(0xBA2F, 4);
    m = 0x60; c = a >= m;
    if (c) { mff_cycles(0xBA31, 1); goto L_BA37; }
    mff_cycles(0xBA33, 4);
    a = 0x0A;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBA35, 1); goto L_BA39; }
L_BA37:;
    mff_cycles(0xBA37, 2);
    a = 0x0E;
L_BA39:;
    mff_cycles(0xBA39, 13);
    RAM(0x00) = a;
    a = RAM(RNG);
    a &= 0x0F;
    m = RAM(0x00); c = a >= m;
    if (c) { mff_cycles(0xBA41, 1); goto L_BA4D; }
    mff_cycles(0xBA43, 7);
    a = RAM(GAME_ACTION);
    m = 0x06; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBA47, 1); goto L_BA4D; }
    mff_cycles(0xBA49, 4);
    a = 0x84;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBA4B, 1); goto L_BA4F; }
L_BA4D:;
    mff_cycles(0xBA4D, 2);
    a = 0x81;
    n = (a) >> 7;
    z = (a) == 0;
L_BA4F:;
    mff_cycles(0xBA4F, 11);
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BA53 (bank 1) - lifted by port/tools/lift.py */
void func_BA53_b1(void) {
    REGS_IN;
L_BA53:;
    mff_cycles(0xBA53, 8);
    a = 0xDA;
    m = RAM(OBJ_ANIM + x); c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBA58, 1); goto L_BA5F; }
    mff_cycles(0xBA5A, 8);
    y = 0x24;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1A5E, func_F443, 0xE000)) return;
    SYNC_IN;
L_BA5F:;
    mff_cycles(0xBA5F, 27);
    a = 0x08;
    RAM(OBJ_INVULN + x) = a;
    a = 0x81;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1A70, 0xB390, 1)) return;
    SYNC_IN;
    mff_cycles(0xBA71, 8);
    y = 0x04;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x1A75, 0xB39B, 1)) return;
    SYNC_IN;
    mff_cycles(0xBA76, 8);
    a = RAM(OBJ_X_HI + x);
    m = 0xFF; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBA7B, 1); goto L_BA85; }
    mff_cycles(0xBA7D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1A7F, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBA80, 2);
    if (c) { mff_cycles(0xBA80, 1); goto L_BA85; }
    mff_cycles(0xBA82, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF1A1, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F1A1(); g_code_window_base = swb; } return;
L_BA85:;
    mff_cycles(0xBA85, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF183, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F183(); g_code_window_base = swb; } return;
}

/*  $BA88 (bank 1) - lifted by port/tools/lift.py */
void func_BA88_b1(void) {
    REGS_IN;
L_BA88:;
    mff_cycles(0xBA88, 8);
    a = 0xA3;
    m = RAM(OBJ_ANIM + x); c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBA8D, 1); goto L_BA99; }
    mff_cycles(0xBA8F, 8);
    y = 0x30;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1A93, func_F443, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBA94, 7);
    a = 0x26;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
L_BA99:;
    mff_cycles(0xBA99, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x05; c = a >= m;
    if (c) { mff_cycles(0xBA9E, 1); goto L_BAAA; }
    mff_cycles(0xBAA0, 6);
    a = RAM(OBJ_INVULN + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xBAA3, 1); goto L_BAAA; }
    mff_cycles(0xBAA5, 7);
    a = 0x01;
    RAM(OBJ_INVULN + x) = a;
L_BAAA:;
    mff_cycles(0xBAAA, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x02; c = a >= m;
    if (c) { mff_cycles(0xBAAF, 1); goto L_BAB5; }
    mff_cycles(0xBAB1, 4);
    a = 0x96;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBAB3, 1); goto L_BAB7; }
L_BAB5:;
    mff_cycles(0xBAB5, 2);
    a = 0x97;
L_BAB7:;
    mff_cycles(0xBAB7, 13);
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x09; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xBABF, 1); goto L_BAC2; }
    mff_cycles(0xBAC1, 6);
    SYNC_OUT;
    mff_rts(); return;
L_BAC2:;
    mff_cycles(0xBAC2, 25);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    RAM(OBJ_ATTACK + x) = a;
    a = 0x81;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BAD0 (bank 1) - lifted by port/tools/lift.py */
void func_BAD0_b1(void) {
    REGS_IN;
L_BAD0:;
    mff_cycles(0xBAD0, 4);
    a = nes_read((uint16_t)(0xBAF9 + y));
    mff_cycles(0xBAD3, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = nes_read((uint16_t)(0xBAE9 + y));
    mff_cycles(0xBAD9, 9);
    RAM(OBJ_VX + x) = a;
    a = nes_read((uint16_t)(0xBB19 + y));
    mff_cycles(0xBADF, 9);
    RAM(OBJ_VY_FRAC + x) = a;
    a = nes_read((uint16_t)(0xBB09 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xBAE5, 11);
    RAM(OBJ_VY + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BB29 (bank 1) - lifted by port/tools/lift.py */
void func_BB29_b1(void) {
    REGS_IN;
L_BB29:;
    mff_cycles(0xBB29, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1B2B, func_FB5B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBB2C, 6);
    y = a;
    a &= 0x07;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBB2F, 1); goto L_BB49; }
    mff_cycles(0xBB31, 4);
    a = y;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBB32, 1); goto L_BB38; }
    mff_cycles(0xBB34, 6);
    y++;
    y++;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0xBB36, 1); goto L_BB3A; }
L_BB38:;
    mff_cycles(0xBB38, 4);
    y--;
    y--;
    n = (y) >> 7;
    z = (y) == 0;
L_BB3A:;
    mff_cycles(0xBB3A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1B3C, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBB3D, 2);
    if (c) { mff_cycles(0xBB3D, 1); goto L_BB4D; }
    mff_cycles(0xBB3F, 12);
    a = y;
    a ^= 0xFF;
    m = 0x01; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    a &= 0x0F;
    y = a;
    z = (y) == 0;
    if (!z) { mff_cycles(0xBB47, 1); goto L_BB4D; }
L_BB49:;
    mff_cycles(0xBB49, 4);
    a = nes_read((uint16_t)(0xBB81 + y));
    mff_cycles(0xBB4C, 2);
    y = a;
L_BB4D:;
    mff_cycles(0xBB4D, 4);
    a = nes_read((uint16_t)(0xFBF0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xBB50, 11);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1B55, 0xBAD0, 1)) return;
    SYNC_IN;
    mff_cycles(0xBB56, 6);
    a = y;
    a &= 0x01;
    z = (a) == 0;
    if (z) { mff_cycles(0xBB59, 1); goto L_BB67; }
    mff_cycles(0xBB5B, 16);
    a = 0x12;
    RAM(OBJ_TIMER2 + x) = a;
    a = 0x07;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_GROUND_Y + x) = a;
    if (!z) { mff_cycles(0xBB65, 1); goto L_BB80; }
L_BB67:;
    mff_cycles(0xBB67, 8);
    a = y;
    t = a; c = t & 1; t >>= 1; a = t;
    t = a; c = t & 1; t >>= 1; a = t;
    if (!c) { mff_cycles(0xBB6A, 1); goto L_BB76; }
    mff_cycles(0xBB6C, 14);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER2 + x) = a;
    RAM(OBJ_GROUND_Y + x) = a;
    if (!z) { mff_cycles(0xBB74, 1); goto L_BB80; }
L_BB76:;
    mff_cycles(0xBB76, 14);
    a = 0x14;
    RAM(OBJ_TIMER2 + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_GROUND_Y + x) = a;
L_BB80:;
    mff_cycles(0xBB80, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BB91 (bank 1) - lifted by port/tools/lift.py */
void func_BB91_b1(void) {
    REGS_IN;
L_BB91:;
    mff_cycles(0xBB91, 8);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBB96, 1); goto L_BBA5; }
    mff_cycles(0xBB98, 21);
    a = 0x33;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0x01;
    RAM(OBJ_VX + x) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1); RAM(OBJ_STATE + x) = t;
L_BBA5:;
    mff_cycles(0xBBA5, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBBAA, 1); goto L_BBBA; }
    mff_cycles(0xBBAC, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1BAE, func_F090, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBBAF, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1BB1, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBBB2, 8);
    a = RAM(OBJ_X + x);
    m = 0xF8; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xBBB7, 1); goto L_BBC7; }
    mff_cycles(0xBBB9, 6);
    SYNC_OUT;
    mff_rts(); return;
L_BBBA:;
    mff_cycles(0xBBBA, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1BBC, func_F099, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBBBD, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1BBF, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBBC0, 8);
    a = RAM(OBJ_X + x);
    m = 0x08; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xBBC5, 1); goto L_BBCA; }
L_BBC7:;
    mff_cycles(0xBBC7, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1BC9, func_F2BE, 0xE000)) return;
    SYNC_IN;
L_BBCA:;
    mff_cycles(0xBBCA, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BBCB (bank 1) - lifted by port/tools/lift.py */
void func_BBCB_b1(void) {
    REGS_IN;
L_BBCB:;
    mff_cycles(0xBBCB, 8);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBBD0, 1); goto L_BBDE; }
    mff_cycles(0xBBD2, 5);
    a = RAM(SCROLL_X);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBBD4, 1); goto L_BBE6; }
    mff_cycles(0xBBD6, 14);
    a = 0x3C;
    RAM(OBJ_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1); RAM(OBJ_STATE + x) = t;
L_BBDE:;
    mff_cycles(0xBBDE, 6);
    a = RAM(OBJ_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBBE1, 1); goto L_BBEF; }
    mff_cycles(0xBBE3, 7);
    t = (uint8_t)(RAM(OBJ_TIMER + x) - 1); RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
L_BBE6:;
    mff_cycles(0xBBE6, 6);
    SYNC_OUT;
    mff_rts(); return;
L_BBE7:;
    mff_cycles(0xBBE7, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1BE9, func_F2BE, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBBEA, 11);
    a = 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(AREA_KILLS) = a;
    SYNC_OUT;
    mff_rts(); return;
L_BBEF:;
    mff_cycles(0xBBEF, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1BF1, 0xBCD1, 1)) return;
    SYNC_IN;
    mff_cycles(0xBBF2, 2);
    if (!z) { mff_cycles(0xBBF2, 1); goto L_BBFA; }
    mff_cycles(0xBBF4, 13);
    a = 0x3C;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_BBFA:;
    mff_cycles(0xBBFA, 15);
    t = (uint8_t)(RAM(OBJ_TIMER2 + x) + 1); RAM(OBJ_TIMER2 + x) = t;
    a = RAM(OBJ_TIMER2 + x);
    m = 0x06; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xBBE7, 2); goto L_BBE7; }
    mff_cycles(0xBC04, 8);
    a = 0x29;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1C08, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBC09, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1C0B, func_F71A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBC0C, 2);
    if (c) { mff_cycles(0xBBEF, 2); goto L_BBEF; }
    mff_cycles(0xBC0E, 97);
    RAM(0x00) = y;
    a = RAM(OBJ_X_HI + x);
    RAM(OBJ_X_HI + y) = a;
    a = RAM(OBJ_Y_HI + x);
    RAM(OBJ_Y_HI + y) = a;
    a = 0x00;
    RAM(OBJ_LIFE + y) = a;
    RAM(OBJ_LIFE_MAX + y) = a;
    RAM(OBJ_THROW_FLAG + y) = a;
    RAM(OBJ_HIT_COOLDOWN + y) = a;
    RAM(OBJ_INVULN + y) = a;
    RAM(OBJ_HIT_PLAYER + y) = a;
    RAM(OBJ_HIT_TIMER + y) = a;
    RAM(OBJ_HELD + y) = a;
    RAM(OBJ_LAND_Y + y) = a;
    a = 0x9E;
    RAM(OBJ_ATTACK + y) = a;
    a = 0x3F;
    RAM(OBJ_ATTACK_DAMAGE + y) = a;
    a = 0x91;
    RAM(OBJ_ID + y) = a;
    a = RAM(OBJ_TIMER2 + x);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xBC4D, 1); goto L_BC54; }
    mff_cycles(0xBC4F, 7);
    a = 0xFF;
    RAM(OBJ_LAND_Y + y) = a;
L_BC54:;
    mff_cycles(0xBC54, 8);
    a = 0x7B;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1C58, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBC59, 20);
    a = RAM(RNG);
    m = RAM(RNG + 1); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(RNG) = a;
    a &= 0x07;
    y = a;
    RAM(0x01) = y;
    a = nes_read((uint16_t)(0xBD3E + y));
    mff_cycles(0xBC67, 10);
    RAM(OBJ_TIMER + x) = a;
    a = RAM(ROUND);
    z = (a) == 0;
    if (z) { mff_cycles(0xBC6C, 1); goto L_BC7A; }
    mff_cycles(0xBC6E, 4);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBC70, 1); goto L_BC97; }
    mff_cycles(0xBC72, 4);
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBC74, 1); goto L_BCB4; }
    mff_cycles(0xBC76, 4);
    m = 0x04; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBC78, 1); goto L_BCB4; }
L_BC7A:;
    mff_cycles(0xBC7A, 4);
    a = nes_read((uint16_t)(0xBCF6 + y));
    mff_cycles(0xBC7D, 15);
    y = RAM(0x00);
    RAM(OBJ_Y + y) = a;
    y = RAM(0x01);
    a = nes_read((uint16_t)(0xBD0E + y));
    mff_cycles(0xBC87, 15);
    y = RAM(0x00);
    RAM(OBJ_X + y) = a;
    y = RAM(0x01);
    a = nes_read((uint16_t)(0xBD26 + y));
    mff_cycles(0xBC91, 14);
    y = RAM(0x00);
    n = (y) >> 7;
    z = (y) == 0;
    RAM(OBJ_DIR + y) = a;
    SYNC_OUT;
    mff_rts(); return;
L_BC97:;
    mff_cycles(0xBC97, 4);
    a = nes_read((uint16_t)(0xBCFE + y));
    mff_cycles(0xBC9A, 15);
    y = RAM(0x00);
    RAM(OBJ_Y + y) = a;
    y = RAM(0x01);
    a = nes_read((uint16_t)(0xBD16 + y));
    mff_cycles(0xBCA4, 15);
    y = RAM(0x00);
    RAM(OBJ_X + y) = a;
    y = RAM(0x01);
    a = nes_read((uint16_t)(0xBD2E + y));
    mff_cycles(0xBCAE, 14);
    y = RAM(0x00);
    n = (y) >> 7;
    z = (y) == 0;
    RAM(OBJ_DIR + y) = a;
    SYNC_OUT;
    mff_rts(); return;
L_BCB4:;
    mff_cycles(0xBCB4, 4);
    a = nes_read((uint16_t)(0xBD06 + y));
    mff_cycles(0xBCB7, 15);
    y = RAM(0x00);
    RAM(OBJ_Y + y) = a;
    y = RAM(0x01);
    a = nes_read((uint16_t)(0xBD1E + y));
    mff_cycles(0xBCC1, 15);
    y = RAM(0x00);
    RAM(OBJ_X + y) = a;
    y = RAM(0x01);
    a = nes_read((uint16_t)(0xBD36 + y));
    mff_cycles(0xBCCB, 14);
    y = RAM(0x00);
    n = (y) >> 7;
    z = (y) == 0;
    RAM(OBJ_DIR + y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BCD1 (bank 1) - lifted by port/tools/lift.py */
void func_BCD1_b1(void) {
    REGS_IN;
L_BCD1:;
    mff_cycles(0xBCD1, 7);
    a = 0x00;
    RAM(0x00) = a;
    y = 0x0F;
L_BCD7:;
    mff_cycles(0xBCD7, 6);
    a = RAM(OBJ_STATE + y);
    n = (a) >> 7;
    if (!n) { mff_cycles(0xBCDA, 1); goto L_BCE5; }
    mff_cycles(0xBCDC, 8);
    a = RAM(OBJ_ANIM + y);
    m = 0x7B; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xBCE1, 1); goto L_BCE5; }
    mff_cycles(0xBCE3, 5);
    t = (uint8_t)(RAM(0x00) + 1); RAM(0x00) = t;
L_BCE5:;
    mff_cycles(0xBCE5, 6);
    y--;
    m = 0x02; c = y >= m;
    z = ((uint8_t)(y - m)) == 0;
    if (!z) { mff_cycles(0xBCD7, 1); goto L_BCD7; }
    mff_cycles(0xBCEA, 7);
    a = RAM(0x00);
    m = 0x03; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBCEE, 1); goto L_BCF3; }
    mff_cycles(0xBCF0, 8);
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_BCF3:;
    mff_cycles(0xBCF3, 8);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BD46 (bank 1) - lifted by port/tools/lift.py */
void func_BD46_b1(void) {
    REGS_IN;
L_BD46:;
    mff_cycles(0xBD46, 8);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBD4B, 1); goto L_BD8B; }
    mff_cycles(0xBD4D, 48);
    RAM(OBJ_VX_FRAC + x) = a;
    RAM(OBJ_VY_FRAC + x) = a;
    RAM(OBJ_ATTACK + x) = a;
    a = 0x03;
    RAM(OBJ_VX + x) = a;
    a = 0x01;
    RAM(OBJ_VY + x) = a;
    a = 0x10;
    RAM(OBJ_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1); RAM(OBJ_STATE + x) = t;
    a = RAM(0xB2);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBD6A, 1); goto L_BD74; }
    mff_cycles(0xBD6C, 14);
    t = (uint8_t)(RAM(OBJ_GROUND_Y + 15) - 1); RAM(OBJ_GROUND_Y + 15) = t;
    t = (uint8_t)(RAM(0xB9) + 1); RAM(0xB9) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBD7C, 2); call_by_address_tail(0xBD7C, 1); return;
L_BD74:;
    mff_cycles(0xBD74, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1D76, 0xBE9B, 1)) return;
    SYNC_IN;
    mff_cycles(0xBD77, 2);
    if (!z) { mff_cycles(0xBD77, 1); goto L_BD7C; }
    mff_cycles(0xBD79, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1D7B, 0xBDCE, 1)) return;
    SYNC_IN;
L_BD7C:;
    mff_cycles(0xBD7C, 9);
    a = RAM(PLAYER_DIR);
    m = RAM(OBJ_DIR + x); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBD81, 1); goto L_BD88; }
    mff_cycles(0xBD83, 6);
    a = RAM(OBJ_DIR + x);
    a ^= 0x03;
    n = (a) >> 7;
    z = (a) == 0;
L_BD88:;
    mff_cycles(0xBD88, 5);
    RAM(OBJ_DIR + x) = a;
L_BD8B:;
    mff_cycles(0xBD8B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1D8D, func_F1DF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBD8E, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBD93, 1); goto L_BDA2; }
    mff_cycles(0xBD95, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1D97, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBD98, 8);
    a = RAM(OBJ_X + x);
    m = 0xF8; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xBD9D, 1); goto L_BDAC; }
    mff_cycles(0xBD9F, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBDAF, 2); call_by_address_tail(0xBDAF, 1); return;
L_BDA2:;
    mff_cycles(0xBDA2, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1DA4, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBDA5, 8);
    a = RAM(OBJ_X + x);
    m = 0x08; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xBDAA, 1); goto L_BDAF; }
L_BDAC:;
    mff_cycles(0xBDAC, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF2BE, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F2BE(); g_code_window_base = swb; } return;
L_BDAF:;
    mff_cycles(0xBDAF, 13);
    t = (uint8_t)(RAM(OBJ_TIMER + x) - 1); RAM(OBJ_TIMER + x) = t;
    a = RAM(OBJ_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBDB5, 1); goto L_BDCD; }
    mff_cycles(0xBDB7, 4);
    a &= 0x01;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBDB9, 1); goto L_BDC5; }
    mff_cycles(0xBDBB, 13);
    a = RAM(OBJ_FLAGS + x);
    a |= 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS + x) = a;
    if (!z) { mff_cycles(0xBDC3, 1); goto L_BDCD; }
L_BDC5:;
    mff_cycles(0xBDC5, 11);
    a = RAM(OBJ_FLAGS + x);
    a &= 0xFB;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS + x) = a;
L_BDCD:;
    mff_cycles(0xBDCD, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BD7C (bank 1) - lifted by port/tools/lift.py */
void func_BD7C_b1(void) {
    REGS_IN;
L_BD7C:;
    mff_cycles(0xBD7C, 9);
    a = RAM(PLAYER_DIR);
    m = RAM(OBJ_DIR + x); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBD81, 1); goto L_BD88; }
    mff_cycles(0xBD83, 6);
    a = RAM(OBJ_DIR + x);
    a ^= 0x03;
    n = (a) >> 7;
    z = (a) == 0;
L_BD88:;
    mff_cycles(0xBD88, 11);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x1D8D, func_F1DF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBD8E, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBD93, 1); goto L_BDA2; }
    mff_cycles(0xBD95, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1D97, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBD98, 8);
    a = RAM(OBJ_X + x);
    m = 0xF8; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xBD9D, 1); goto L_BDAC; }
    mff_cycles(0xBD9F, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBDAF, 2); call_by_address_tail(0xBDAF, 1); return;
L_BDA2:;
    mff_cycles(0xBDA2, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1DA4, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBDA5, 8);
    a = RAM(OBJ_X + x);
    m = 0x08; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xBDAA, 1); goto L_BDAF; }
L_BDAC:;
    mff_cycles(0xBDAC, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF2BE, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F2BE(); g_code_window_base = swb; } return;
L_BDAF:;
    mff_cycles(0xBDAF, 13);
    t = (uint8_t)(RAM(OBJ_TIMER + x) - 1); RAM(OBJ_TIMER + x) = t;
    a = RAM(OBJ_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBDB5, 1); goto L_BDCD; }
    mff_cycles(0xBDB7, 4);
    a &= 0x01;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBDB9, 1); goto L_BDC5; }
    mff_cycles(0xBDBB, 13);
    a = RAM(OBJ_FLAGS + x);
    a |= 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS + x) = a;
    if (!z) { mff_cycles(0xBDC3, 1); goto L_BDCD; }
L_BDC5:;
    mff_cycles(0xBDC5, 11);
    a = RAM(OBJ_FLAGS + x);
    a &= 0xFB;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS + x) = a;
L_BDCD:;
    mff_cycles(0xBDCD, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BDAF (bank 1) - lifted by port/tools/lift.py */
void func_BDAF_b1(void) {
    REGS_IN;
L_BDAF:;
    mff_cycles(0xBDAF, 13);
    t = (uint8_t)(RAM(OBJ_TIMER + x) - 1); RAM(OBJ_TIMER + x) = t;
    a = RAM(OBJ_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBDB5, 1); goto L_BDCD; }
    mff_cycles(0xBDB7, 4);
    a &= 0x01;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBDB9, 1); goto L_BDC5; }
    mff_cycles(0xBDBB, 13);
    a = RAM(OBJ_FLAGS + x);
    a |= 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS + x) = a;
    if (!z) { mff_cycles(0xBDC3, 1); goto L_BDCD; }
L_BDC5:;
    mff_cycles(0xBDC5, 11);
    a = RAM(OBJ_FLAGS + x);
    a &= 0xFB;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS + x) = a;
L_BDCD:;
    mff_cycles(0xBDCD, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BDCE (bank 1) - lifted by port/tools/lift.py */
void func_BDCE_b1(void) {
    REGS_IN;
    goto L_BDCE;
L_BDAF:;
    mff_cycles(0xBDAF, 13);
    t = (uint8_t)(RAM(OBJ_TIMER + x) - 1); RAM(OBJ_TIMER + x) = t;
    a = RAM(OBJ_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBDB5, 1); goto L_BDCD; }
    mff_cycles(0xBDB7, 4);
    a &= 0x01;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBDB9, 1); goto L_BDC5; }
    mff_cycles(0xBDBB, 13);
    a = RAM(OBJ_FLAGS + x);
    a |= 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS + x) = a;
    if (!z) { mff_cycles(0xBDC3, 1); goto L_BDCD; }
L_BDC5:;
    mff_cycles(0xBDC5, 11);
    a = RAM(OBJ_FLAGS + x);
    a &= 0xFB;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS + x) = a;
L_BDCD:;
    mff_cycles(0xBDCD, 6);
    SYNC_OUT;
    mff_rts(); return;
L_BDCE:;
    mff_cycles(0xBDCE, 6);
    a = RAM(OBJ_LAND_Y + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0xBDD1, 1); goto L_BDD5; }
    mff_cycles(0xBDD3, 2);
    if (n) { mff_cycles(0xBDD3, 1); goto L_BDDF; }
L_BDD5:;
    mff_cycles(0xBDD5, 13);
    a = RAM(RNG);
    m = RAM(RNG + 1); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(RNG) = a;
    m = 0x7F; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xBDAF, 1); goto L_BDAF; }
L_BDDF:;
    mff_cycles(0xBDDF, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1DE1, func_F71A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBDE2, 2);
    if (c) { mff_cycles(0xBDAF, 1); goto L_BDAF; }
    mff_cycles(0xBDE4, 128);
    RAM(0x00) = y;
    a = RAM(OBJ_X_HI + x);
    RAM(OBJ_X_HI + y) = a;
    a = RAM(OBJ_Y_HI + x);
    RAM(OBJ_Y_HI + y) = a;
    a = RAM(OBJ_X + x);
    RAM(OBJ_X + y) = a;
    a = RAM(OBJ_Y + x);
    c = 0;
    m = 0x0C; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(OBJ_Y + y) = a;
    a = 0x00;
    RAM(OBJ_LIFE + y) = a;
    RAM(OBJ_LIFE_MAX + y) = a;
    RAM(OBJ_THROW_FLAG + y) = a;
    RAM(OBJ_HIT_COOLDOWN + y) = a;
    RAM(OBJ_INVULN + y) = a;
    RAM(OBJ_HIT_PLAYER + y) = a;
    RAM(OBJ_HIT_TIMER + y) = a;
    RAM(OBJ_HELD + y) = a;
    RAM(OBJ_KIND + y) = a;
    RAM(OBJ_TIER + y) = a;
    RAM(OBJ_AI_TIMER + y) = a;
    RAM(OBJ_AI_COUNT + y) = a;
    RAM(OBJ_AI_FLAG + y) = a;
    RAM(OBJ_ATTACK_DAMAGE + y) = a;
    a = 0x1F;
    RAM(OBJ_ATTACK + y) = a;
    a = RAM(OBJ_LAND_Y + x);
    n = (a) >> 7;
    if (!n) { mff_cycles(0xBE35, 1); goto L_BE44; }
    mff_cycles(0xBE37, 15);
    a = 0x10;
    RAM(OBJ_ID + y) = a;
    a = 0x61;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1E40, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBE41, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBE72, 2); call_by_address_tail(0xBE72, 1); return;
L_BE44:;
    mff_cycles(0xBE44, 20);
    a = RAM(RNG + 1);
    m = RAM(RNG + 2); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(RNG + 1) = a;
    a &= 0x0F;
    y = a;
    RAM(0x01) = y;
    a = nes_read((uint16_t)(0xBE88 + y));
    mff_cycles(0xBE52, 12);
    y = RAM(0x00);
    RAM(OBJ_ID + y) = a;
    m = 0x69; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xBE59, 1); goto L_BE68; }
    mff_cycles(0xBE5B, 7);
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xBE98 + y));
    mff_cycles(0xBE60, 9);
    y = RAM(0x00);
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1E64, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBE65, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBE72, 2); call_by_address_tail(0xBE72, 1); return;
L_BE68:;
    mff_cycles(0xBE68, 7);
    y = RAM(0x01);
    a = nes_read((uint16_t)(0xBE78 + y));
    mff_cycles(0xBE6D, 9);
    y = RAM(0x00);
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1E71, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBE72, 13);
    a = 0x90;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS + y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BE72 (bank 1) - lifted by port/tools/lift.py */
void func_BE72_b1(void) {
    REGS_IN;
L_BE72:;
    mff_cycles(0xBE72, 13);
    a = 0x90;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS + y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BE9B (bank 1) - lifted by port/tools/lift.py */
void func_BE9B_b1(void) {
    REGS_IN;
L_BE9B:;
    mff_cycles(0xBE9B, 7);
    a = 0x00;
    RAM(0x00) = a;
    y = 0x0D;
L_BEA1:;
    mff_cycles(0xBEA1, 6);
    a = RAM(OBJ_STATE + y);
    n = (a) >> 7;
    if (!n) { mff_cycles(0xBEA4, 1); goto L_BEC1; }
    mff_cycles(0xBEA6, 8);
    a = RAM(OBJ_ID + y);
    m = 0x10; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBEAB, 1); goto L_BEBF; }
    mff_cycles(0xBEAD, 4);
    m = 0x11; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBEAF, 1); goto L_BEBF; }
    mff_cycles(0xBEB1, 4);
    m = 0x12; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBEB3, 1); goto L_BEBF; }
    mff_cycles(0xBEB5, 4);
    m = 0x13; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBEB7, 1); goto L_BEBF; }
    mff_cycles(0xBEB9, 4);
    m = 0x69; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBEBB, 1); goto L_BEBF; }
    mff_cycles(0xBEBD, 2);
    if (!z) { mff_cycles(0xBEBD, 1); goto L_BEC1; }
L_BEBF:;
    mff_cycles(0xBEBF, 5);
    t = (uint8_t)(RAM(0x00) + 1); RAM(0x00) = t;
L_BEC1:;
    mff_cycles(0xBEC1, 6);
    y--;
    m = 0x00; c = y >= m;
    z = ((uint8_t)(y - m)) == 0;
    if (!z) { mff_cycles(0xBEA1, 1); goto L_BEA1; }
    mff_cycles(0xBEC6, 7);
    a = RAM(0x00);
    m = 0x02; c = a >= m;
    if (!c) { mff_cycles(0xBECA, 1); goto L_BECF; }
    mff_cycles(0xBECC, 8);
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_BECF:;
    mff_cycles(0xBECF, 8);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BF38 (bank 1) - lifted by port/tools/lift.py */
void func_BF38_b1(void) {
    REGS_IN;
L_BF38:;
    mff_cycles(0xBF38, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1); RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) { mff_cycles(0xBF3B, 1); goto L_BF47; }
    mff_cycles(0xBF3D, 14);
    a = 0x80;
    RAM(OBJ_STATE + x) = a;
    a = 0x7D;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ID + x) = a;
L_BF47:;
    mff_cycles(0xBF47, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BF48 (bank 1) - lifted by port/tools/lift.py */
void func_BF48_b1(void) {
    REGS_IN;
L_BF48:;
    mff_cycles(0xBF48, 15);
    a = 0x01;
    RAM(OBJ_INVULN + x) = a;
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBF52, 1); goto L_BF7C; }
    mff_cycles(0xBF54, 12);
    y = RAM(OBJ_TIMER + x);
    a = RAM(OBJ_ANIM_FRAME + y);
    m = 0x0B; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xBF5C, 1); goto L_BF94; }
    mff_cycles(0xBF5E, 45);
    a = 0xCF;
    RAM(OBJ_ANIM + x) = a;
    a = 0x3B;
    RAM(CHR_BANKS + 4) = a;
    a = 0x00;
    RAM(OBJ_ANIM_FRAME + x) = a;
    RAM(OBJ_ANIM_TIMER + x) = a;
    a = 0x90;
    RAM(OBJ_FLAGS + x) = a;
    a |= 0x04;
    RAM(OBJ_FLAGS + y) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1); RAM(OBJ_STATE + x) = t;
L_BF7C:;
    mff_cycles(0xBF7C, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1); RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (z) { mff_cycles(0xBF7F, 1); goto L_BFBB; }
    mff_cycles(0xBF81, 8);
    a = RAM(OBJ_AI_TIMER + x);
    m = 0x5F; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xBF86, 1); goto L_BF94; }
    mff_cycles(0xBF88, 4);
    m = 0xA0; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xBF8A, 1); goto L_BF94; }
    mff_cycles(0xBF8C, 11);
    a = RAM(OBJ_FLAGS + x);
    a ^= 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS + x) = a;
L_BF94:;
    mff_cycles(0xBF94, 6);
    SYNC_OUT;
    mff_rts(); return;
L_BFBB:;
    mff_cycles(0xBFBB, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF2BE, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F2BE(); g_code_window_base = swb; } return;
}

/*  $BF95 (bank 1) - lifted by port/tools/lift.py */
void func_BF95_b1(void) {
    REGS_IN;
L_BF95:;
    mff_cycles(0xBF95, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xBF9A, 1); goto L_BFA6; }
    mff_cycles(0xBF9C, 14);
    a = 0x01;
    RAM(OBJ_ANIM_FRAME + x) = a;
    a = 0x00;
    RAM(OBJ_ANIM_TIMER + x) = a;
L_BFA6:;
    mff_cycles(0xBFA6, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBFAB, 1); goto L_BFB0; }
    mff_cycles(0xBFAD, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF183, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F183(); g_code_window_base = swb; } return;
L_BFB0:;
    mff_cycles(0xBFB0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1FB2, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBFB3, 9);
    a = RAM(OBJ_X_HI + x);
    m = RAM(SCROLL_X_HI); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xBFB8, 1); goto L_BFBB; }
    mff_cycles(0xBFBA, 6);
    SYNC_OUT;
    mff_rts(); return;
L_BFBB:;
    mff_cycles(0xBFBB, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF2BE, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F2BE(); g_code_window_base = swb; } return;
}

/*  $BFBE (bank 1) - lifted by port/tools/lift.py */
void func_BFBE_b1(void) {
    REGS_IN;
L_BFBE:;
    mff_cycles(0xBFBE, 41);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + x) = a;
    a = RAM(OBJ_X);
    RAM(OBJ_X + x) = a;
    a = 0xD0;
    RAM(OBJ_FLAGS + x) = a;
    t = (uint8_t)(RAM(OBJ_DOWN_TIMER) + 1); RAM(OBJ_DOWN_TIMER) = t;
    a = 0x00;
    RAM(OBJ_X) = a;
    a = nes_read(0xE22A);
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xBFDA, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1FDC, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBFDD, 4);
    a = nes_read(0xE272);
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xBFE0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1FE2, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBFE3, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1FE5, func_F03F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBFE6, 29);
    a = 0x8F;
    RAM(OBJ_ID + x) = a;
    a = 0x19;
    RAM(PALETTE_BUF + 23) = a;
    a = 0x21;
    RAM(PALETTE_BUF + 27) = a;
    RAM(PALETTE_REQUEST) = a;
    a = 0xFE;
    y = 0x31;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF449, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F449(); g_code_window_base = swb; } return;
}
