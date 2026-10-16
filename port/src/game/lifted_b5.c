/*
 * lifted_b5.c - bank 5 routines $8000-$BFFF, lifted by
 * port/tools/lift_bank.py from the pure scaffold: exact, still to be
 * tidied and moved into their systems' files.
 */
#include "mff.h"
#include "state.h"
#include "coroutine.h"

void func_F090(void);
void func_F099(void);
void func_F0A2(void);
void func_F0F7(void);
void func_F183(void);
void func_F1A1(void);
void func_F1BF(void);
void func_F1DF(void);
void func_F1FD(void);
void func_F288(void);
void func_F294(void);
void func_F2A0(void);
void func_F2BE(void);
void func_F343(void);
void func_F34A(void);
void func_F34C(void);
void func_F374(void);
void func_F440(void);
void func_F449(void);
void func_F71A(void);
void func_FB3E(void);

/*  $B300 (bank 5) - lifted by port/tools/lift.py */
void func_B300_b5(void) {
    REGS_IN;
L_B300:;
    mff_cycles(0xB300, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB331, 2); call_by_address_tail(0xB331, 5); return;
}

/*  $B303 (bank 5) - lifted by port/tools/lift.py */
void func_B303_b5(void) {
    REGS_IN;
L_B303:;
    mff_cycles(0xB303, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB331, 2); call_by_address_tail(0xB331, 5); return;
}

/*  $B306 (bank 5) - lifted by port/tools/lift.py */
void func_B306_b5(void) {
    REGS_IN;
L_B306:;
    mff_cycles(0xB306, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB5D7, 2); call_by_address_tail(0xB5D7, 5); return;
}

/*  $B309 (bank 5) - lifted by port/tools/lift.py */
void func_B309_b5(void) {
    REGS_IN;
L_B309:;
    mff_cycles(0xB309, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 5); return;
}

/*  $B30C (bank 5) - lifted by port/tools/lift.py */
void func_B30C_b5(void) {
    REGS_IN;
L_B30C:;
    mff_cycles(0xB30C, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB696, 2); call_by_address_tail(0xB696, 5); return;
}

/*  $B30F (bank 5) - lifted by port/tools/lift.py */
void func_B30F_b5(void) {
    REGS_IN;
L_B30F:;
    mff_cycles(0xB30F, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 5); return;
}

/*  $B312 (bank 5) - lifted by port/tools/lift.py */
void func_B312_b5(void) {
    REGS_IN;
L_B312:;
    mff_cycles(0xB312, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 5); return;
}

/*  $B315 (bank 5) - lifted by port/tools/lift.py */
void func_B315_b5(void) {
    REGS_IN;
L_B315:;
    mff_cycles(0xB315, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 5); return;
}

/*  $B318 (bank 5) - lifted by port/tools/lift.py */
void func_B318_b5(void) {
    REGS_IN;
L_B318:;
    mff_cycles(0xB318, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 5); return;
}

/*  $B31B (bank 5) - lifted by port/tools/lift.py */
void func_B31B_b5(void) {
    REGS_IN;
L_B31B:;
    mff_cycles(0xB31B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 5); return;
}

/*  $B31E (bank 5) - lifted by port/tools/lift.py */
void func_B31E_b5(void) {
    REGS_IN;
L_B31E:;
    mff_cycles(0xB31E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 5); return;
}

/*  $B321 (bank 5) - lifted by port/tools/lift.py */
void func_B321_b5(void) {
    REGS_IN;
L_B321:;
    mff_cycles(0xB321, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBD7F, 2); call_by_address_tail(0xBD7F, 5); return;
}

/*  $B324 (bank 5) - lifted by port/tools/lift.py */
void func_B324_b5(void) {
    REGS_IN;
L_B324:;
    mff_cycles(0xB324, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 5); return;
}

/*  $B327 (bank 5) - lifted by port/tools/lift.py */
void func_B327_b5(void) {
    REGS_IN;
L_B327:;
    mff_cycles(0xB327, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBD7F, 2); call_by_address_tail(0xBD7F, 5); return;
}

/*  $B32A (bank 5) - lifted by port/tools/lift.py */
void func_B32A_b5(void) {
    REGS_IN;
L_B32A:;
    mff_cycles(0xB32A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 5); return;
}

/*  $B32D (bank 5) - lifted by port/tools/lift.py */
void func_B32D_b5(void) {
    REGS_IN;
L_B32D:;
    mff_cycles(0xB32D, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB330, 2); call_by_address_tail(0xB330, 5); return;
}

/*  $B330 (bank 5) - lifted by port/tools/lift.py */
void func_B330_b5(void) {
    REGS_IN;
L_B330:;
    mff_cycles(0xB330, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B331 (bank 5) - lifted by port/tools/lift.py */
void func_B331_b5(void) {
    REGS_IN;
L_B331:;
    mff_cycles(0xB331, 5);
    a = RAM(0x62);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xB333, 1); goto L_B336; }
    mff_cycles(0xB335, 6);
    SYNC_OUT;
    mff_rts(); return;
L_B336:;
    mff_cycles(0xB336, 12);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    y = a;
    a = nes_read((uint16_t)(0xB64E + y));
    mff_cycles(0xB33F, 7);
    RAM(0x00) = a;
    a = nes_read((uint16_t)(0xB652 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB344, 8);
    RAM(0x01) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $B349 (bank 5) - lifted by port/tools/lift.py */
void func_B349_b5(void) {
    REGS_IN;
L_B349:;
    mff_cycles(0xB349, 8);
    a = 0x09;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x134D, func_F34A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB34E, 44);
    a = 0x14;
    RAM(OBJ_TIMER + x) = a;
    a = 0x00;
    RAM(0xB9) = a;
    RAM(OBJ_GROUND_Y + x) = a;
    a = 0x78;
    RAM(OBJ_LAND_Y + x) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1); RAM(OBJ_STATE + x) = t;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1); RAM(OBJ_STATE + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B366 (bank 5) - lifted by port/tools/lift.py */
void func_B366_b5(void) {
    REGS_IN;
L_B366:;
    mff_cycles(0xB366, 6);
    a = RAM(OBJ_TIMER + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xB369, 1); goto L_B370; }
    mff_cycles(0xB36B, 6);
    a = RAM(OBJ_GROUND_Y + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xB36E, 1); goto L_B37E; }
L_B370:;
    mff_cycles(0xB370, 8);
    a = RAM(OBJ_GROUND_Y + x);
    m = 0x02; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB375, 1); goto L_B37D; }
    mff_cycles(0xB377, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1379, 0xB46F, 5)) return;
    SYNC_IN;
    mff_cycles(0xB37A, 7);
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1); RAM(OBJ_STATE + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
L_B37D:;
    mff_cycles(0xB37D, 6);
    SYNC_OUT;
    mff_rts(); return;
L_B37E:;
    mff_cycles(0xB37E, 5);
    a = RAM(GAME_ACTION);
    z = (a) == 0;
    if (z) { mff_cycles(0xB380, 1); goto L_B387; }
    mff_cycles(0xB382, 4);
    m = 0x03; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB384, 1); goto L_B387; }
    mff_cycles(0xB386, 6);
    SYNC_OUT;
    mff_rts(); return;
L_B387:;
    mff_cycles(0xB387, 7);
    a = RAM(0xB9);
    m = 0x08; c = a >= m;
    if (!c) { mff_cycles(0xB38B, 1); goto L_B3E9; }
    mff_cycles(0xB38D, 75);
    a = 0x80;
    RAM(OBJ_STATE + 6) = a;
    RAM(OBJ_X + 6) = a;
    a = 0x90;
    RAM(OBJ_FLAGS + 6) = a;
    a = RAM(OBJ_X_HI);
    RAM(OBJ_X_HI + 6) = a;
    a = RAM(OBJ_Y_HI);
    RAM(OBJ_Y_HI + 6) = a;
    a = 0x00;
    RAM(OBJ_Y + 6) = a;
    RAM(OBJ_ANIM_TIMER + 6) = a;
    RAM(OBJ_ANIM_FRAME + 6) = a;
    RAM(OBJ_VX + 6) = a;
    RAM(OBJ_VX_FRAC + 6) = a;
    RAM(OBJ_VY + 6) = a;
    RAM(OBJ_VY_FRAC + 6) = a;
    a = 0x1F;
    RAM(OBJ_ATTACK + 6) = a;
    y = RAM(0xB9);
    a = nes_read((uint16_t)(0xB66C + y));
    mff_cycles(0xB3C7, 11);
    RAM(OBJ_ANIM + 6) = a;
    a = RAM(0xB9);
    m = 0x14; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB3CE, 1); goto L_B3D4; }
    mff_cycles(0xB3D0, 5);
    a = 0xFC;
    RAM(0x62) = a;
L_B3D4:;
    mff_cycles(0xB3D4, 4);
    a = nes_read((uint16_t)(0xB681 + y));
    mff_cycles(0xB3D7, 34);
    RAM(OBJ_ID + 6) = a;
    a = RAM(OBJ_FLAGS + 6);
    a |= 0x04;
    RAM(OBJ_FLAGS + 6) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1); RAM(OBJ_STATE + x) = t;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1); RAM(OBJ_STATE + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_B3E9:;
    mff_cycles(0xB3E9, 28);
    a = 0x00;
    RAM(OBJ_STATE + 6) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0x29;
    RAM(GAME_ACTION) = a;
    a = 0x0C;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(0x37) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x13FD, func_F2BE, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB3FE, 11);
    a = 0xFB;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(0x62) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B403 (bank 5) - lifted by port/tools/lift.py */
void func_B403_b5(void) {
    REGS_IN;
L_B403:;
    mff_cycles(0xB403, 9);
    t = (uint8_t)(RAM(OBJ_LAND_Y + x) - 1); RAM(OBJ_LAND_Y + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) { mff_cycles(0xB406, 1); goto L_B410; }
    mff_cycles(0xB408, 14);
    a = 0x40;
    RAM(OBJ_LAND_Y + x) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) - 1); RAM(OBJ_STATE + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
L_B410:;
    mff_cycles(0xB410, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B411 (bank 5) - lifted by port/tools/lift.py */
void func_B411_b5(void) {
    REGS_IN;
L_B411:;
    mff_cycles(0xB411, 8);
    a = RAM(OBJ_Y + 6);
    m = 0x8C; c = a >= m;
    if (!c) { mff_cycles(0xB416, 1); goto L_B45F; }
    mff_cycles(0xB418, 5);
    a = RAM(GAME_ACTION);
    z = (a) == 0;
    if (z) { mff_cycles(0xB41A, 1); goto L_B421; }
    mff_cycles(0xB41C, 4);
    m = 0x03; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB41E, 1); goto L_B421; }
    mff_cycles(0xB420, 6);
    SYNC_OUT;
    mff_rts(); return;
L_B421:;
    mff_cycles(0xB421, 13);
    a = 0x8C;
    RAM(OBJ_Y + 6) = a;
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE1E6 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB42B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x142D, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB42E, 4);
    a = nes_read((uint16_t)(0xE22B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB431, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1433, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB434, 7);
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE1C1 + y));
    mff_cycles(0xB439, 8);
    RAM(OBJ_VX_FRAC) = a;
    a = nes_read((uint16_t)(0xE1C4 + y));
    mff_cycles(0xB43F, 8);
    RAM(OBJ_VX) = a;
    a = nes_read((uint16_t)(0xE1C7 + y));
    mff_cycles(0xB445, 8);
    RAM(OBJ_VY_FRAC) = a;
    a = nes_read((uint16_t)(0xE1CA + y));
    mff_cycles(0xB44B, 21);
    RAM(OBJ_VY) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0x1A;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x1459, func_F2BE, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB45A, 11);
    a = 0xFE;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(0x62) = a;
    SYNC_OUT;
    mff_rts(); return;
L_B45F:;
    mff_cycles(0xB45F, 18);
    a = RAM(OBJ_FLAGS + 6);
    a &= 0xFB;
    RAM(OBJ_FLAGS + 6) = a;
    x = 0x06;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x146B, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB46C, 8);
    x = 0x0F;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B46F (bank 5) - lifted by port/tools/lift.py */
void func_B46F_b5(void) {
    REGS_IN;
L_B46F:;
    mff_cycles(0xB46F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1471, func_F71A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB472, 2);
    if (!c) { mff_cycles(0xB472, 1); goto L_B475; }
L_B474:;
    mff_cycles(0xB474, 6);
    SYNC_OUT;
    mff_rts(); return;
L_B475:;
    mff_cycles(0xB475, 6);
    a = RAM(OBJ_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xB474, 1); goto L_B474; }
    mff_cycles(0xB47A, 98);
    t = (uint8_t)(RAM(OBJ_TIMER + x) - 1); RAM(OBJ_TIMER + x) = t;
    t = (uint8_t)(RAM(OBJ_GROUND_Y + x) + 1); RAM(OBJ_GROUND_Y + x) = t;
    a = 0x3F;
    RAM(OBJ_ATTACK_DAMAGE + y) = a;
    a = 0x9E;
    RAM(OBJ_ATTACK + y) = a;
    a = 0x00;
    RAM(OBJ_LIFE_MAX + y) = a;
    RAM(OBJ_LIFE + y) = a;
    RAM(OBJ_KIND + y) = a;
    RAM(OBJ_GROUND_Y + y) = a;
    RAM(OBJ_VY + y) = a;
    RAM(OBJ_VY_FRAC + y) = a;
    a = RAM(OBJ_X_HI);
    RAM(OBJ_X_HI + y) = a;
    a = RAM(OBJ_Y_HI);
    RAM(OBJ_Y_HI + y) = a;
    a = 0x72;
    RAM(OBJ_ID + y) = a;
    RAM(OBJ_SAVED_ID + y) = a;
    a = 0x7B;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x14B6, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB4B7, 30);
    a = 0x01;
    RAM(OBJ_VX + y) = a;
    a = 0x00;
    RAM(OBJ_VX_FRAC + y) = a;
    RAM(0x02) = y;
    a = RAM(RNG);
    t = a; m = t & 1; t = (uint8_t)(t >> 1 | c << 7); c = m; a = t;
    a &= 0x07;
    y = a;
    a = nes_read((uint16_t)(0xB656 + y));
    z = (a) == 0;
    mff_cycles(0xB4CC, 2);
    if (z) { mff_cycles(0xB4CC, 1); goto L_B4D5; }
    mff_cycles(0xB4CE, 10);
    y = RAM(0x02);
    a = 0x40;
    RAM(OBJ_VX_FRAC + y) = a;
L_B4D5:;
    mff_cycles(0xB4D5, 2);
    y = 0x02;
L_B4D7:;
    mff_cycles(0xB4D7, 6);
    a = RAM(OBJ_STATE + y);
    n = (a) >> 7;
    if (!n) { mff_cycles(0xB4DA, 1); goto L_B4E7; }
    mff_cycles(0xB4DC, 8);
    a = RAM(OBJ_ID + y);
    m = 0x72; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB4E1, 1); goto L_B4E7; }
    mff_cycles(0xB4E3, 5);
    m = RAM(0x02); c = y >= m;
    z = ((uint8_t)(y - m)) == 0;
    if (!z) { mff_cycles(0xB4E5, 1); goto L_B4EE; }
L_B4E7:;
    mff_cycles(0xB4E7, 6);
    y++;
    m = 0x0F; c = y >= m;
    z = ((uint8_t)(y - m)) == 0;
    if (!z) { mff_cycles(0xB4D7, 1); goto L_B4D7; }
    mff_cycles(0xB4EC, 2);
    if (z) { mff_cycles(0xB4EC, 1); goto L_B4F5; }
L_B4EE:;
    mff_cycles(0xB4EE, 8);
    a = RAM(OBJ_TIMER2 + y);
    m = 0x8C; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB4F3, 2); goto L_B505; }
L_B4F5:;
    mff_cycles(0xB4F5, 15);
    a = RAM(RNG + 1);
    a ^= RAM(OBJ_Y);
    a &= 0x03;
    y = a;
    a = nes_read((uint16_t)(0xB666 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB500, 6);
    RAM(0x03) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB512, 2); call_by_address_tail(0xB512, 5); return;
L_B505:;
    mff_cycles(0xB505, 15);
    a = RAM(RNG + 1);
    a ^= RAM(OBJ_Y);
    a &= 0x01;
    y = a;
    a = nes_read((uint16_t)(0xB66A + y));
    mff_cycles(0xB510, 11);
    RAM(0x03) = a;
    y = RAM(0x02);
    a = RAM(0x03);
    z = (a) == 0;
    if (z) { mff_cycles(0xB516, 1); goto L_B52B; }
    mff_cycles(0xB518, 4);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB51A, 1); goto L_B537; }
    mff_cycles(0xB51C, 4);
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB51E, 1); goto L_B543; }
    mff_cycles(0xB520, 4);
    m = 0x03; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB522, 1); goto L_B559; }
    mff_cycles(0xB524, 4);
    m = 0x04; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB526, 1); goto L_B56F; }
    mff_cycles(0xB528, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB581, 2); call_by_address_tail(0xB581, 5); return;
L_B52B:;
    mff_cycles(0xB52B, 16);
    a = 0x01;
    RAM(OBJ_DIR + y) = a;
    a = 0x01;
    z = (a) == 0;
    RAM(OBJ_X + y) = a;
    if (!z) { mff_cycles(0xB535, 1); goto L_B54D; }
L_B537:;
    mff_cycles(0xB537, 16);
    a = 0x01;
    RAM(OBJ_DIR + y) = a;
    a = 0x01;
    z = (a) == 0;
    RAM(OBJ_X + y) = a;
    if (!z) { mff_cycles(0xB541, 1); goto L_B563; }
L_B543:;
    mff_cycles(0xB543, 14);
    a = 0x02;
    RAM(OBJ_DIR + y) = a;
    a = 0xFF;
    RAM(OBJ_X + y) = a;
L_B54D:;
    mff_cycles(0xB54D, 18);
    a = 0x8C;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + y) = a;
    RAM(OBJ_TIMER2 + y) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1557, 0xB5AF, 5)) return;
    SYNC_IN;
    mff_cycles(0xB558, 6);
    SYNC_OUT;
    mff_rts(); return;
L_B559:;
    mff_cycles(0xB559, 14);
    a = 0x02;
    RAM(OBJ_DIR + y) = a;
    a = 0xFF;
    RAM(OBJ_X + y) = a;
L_B563:;
    mff_cycles(0xB563, 18);
    a = 0xA4;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + y) = a;
    RAM(OBJ_TIMER2 + y) = a;
    SYNC_OUT;
    if (!mff_call_at(0x156D, 0xB5AF, 5)) return;
    SYNC_IN;
    mff_cycles(0xB56E, 6);
    SYNC_OUT;
    mff_rts(); return;
L_B56F:;
    mff_cycles(0xB56F, 7);
    a = RAM(ROUND);
    m = 0x02; c = a >= m;
    if (!c) { mff_cycles(0xB52B, 1); goto L_B52B; }
    mff_cycles(0xB575, 16);
    a = 0x01;
    RAM(OBJ_DIR + y) = a;
    a = 0x50;
    z = (a) == 0;
    RAM(OBJ_X + y) = a;
    if (!z) { mff_cycles(0xB57F, 1); goto L_B591; }
    mff_cycles(0xB581, 7);
    a = RAM(ROUND);
    m = 0x02; c = a >= m;
    if (!c) { mff_cycles(0xB543, 1); goto L_B543; }
    mff_cycles(0xB587, 14);
    a = 0x02;
    RAM(OBJ_DIR + y) = a;
    a = 0xB0;
    RAM(OBJ_X + y) = a;
L_B591:;
    mff_cycles(0xB591, 49);
    a = 0x01;
    RAM(OBJ_Y + y) = a;
    a = 0x8C;
    RAM(OBJ_TIMER2 + y) = a;
    a = 0x00;
    RAM(OBJ_VX + y) = a;
    RAM(OBJ_VX_FRAC + y) = a;
    RAM(OBJ_VY + y) = a;
    RAM(OBJ_VY_FRAC + y) = a;
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_KIND + y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B512 (bank 5) - lifted by port/tools/lift.py */
void func_B512_b5(void) {
    REGS_IN;
L_B512:;
    mff_cycles(0xB512, 8);
    y = RAM(0x02);
    a = RAM(0x03);
    z = (a) == 0;
    if (z) { mff_cycles(0xB516, 1); goto L_B52B; }
    mff_cycles(0xB518, 4);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB51A, 1); goto L_B537; }
    mff_cycles(0xB51C, 4);
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB51E, 1); goto L_B543; }
    mff_cycles(0xB520, 4);
    m = 0x03; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB522, 1); goto L_B559; }
    mff_cycles(0xB524, 4);
    m = 0x04; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB526, 1); goto L_B56F; }
    mff_cycles(0xB528, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB581, 2); call_by_address_tail(0xB581, 5); return;
L_B52B:;
    mff_cycles(0xB52B, 16);
    a = 0x01;
    RAM(OBJ_DIR + y) = a;
    a = 0x01;
    z = (a) == 0;
    RAM(OBJ_X + y) = a;
    if (!z) { mff_cycles(0xB535, 1); goto L_B54D; }
L_B537:;
    mff_cycles(0xB537, 16);
    a = 0x01;
    RAM(OBJ_DIR + y) = a;
    a = 0x01;
    z = (a) == 0;
    RAM(OBJ_X + y) = a;
    if (!z) { mff_cycles(0xB541, 1); goto L_B563; }
L_B543:;
    mff_cycles(0xB543, 14);
    a = 0x02;
    RAM(OBJ_DIR + y) = a;
    a = 0xFF;
    RAM(OBJ_X + y) = a;
L_B54D:;
    mff_cycles(0xB54D, 18);
    a = 0x8C;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + y) = a;
    RAM(OBJ_TIMER2 + y) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1557, 0xB5AF, 5)) return;
    SYNC_IN;
    mff_cycles(0xB558, 6);
    SYNC_OUT;
    mff_rts(); return;
L_B559:;
    mff_cycles(0xB559, 14);
    a = 0x02;
    RAM(OBJ_DIR + y) = a;
    a = 0xFF;
    RAM(OBJ_X + y) = a;
L_B563:;
    mff_cycles(0xB563, 18);
    a = 0xA4;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + y) = a;
    RAM(OBJ_TIMER2 + y) = a;
    SYNC_OUT;
    if (!mff_call_at(0x156D, 0xB5AF, 5)) return;
    SYNC_IN;
    mff_cycles(0xB56E, 6);
    SYNC_OUT;
    mff_rts(); return;
L_B56F:;
    mff_cycles(0xB56F, 7);
    a = RAM(ROUND);
    m = 0x02; c = a >= m;
    if (!c) { mff_cycles(0xB52B, 1); goto L_B52B; }
    mff_cycles(0xB575, 16);
    a = 0x01;
    RAM(OBJ_DIR + y) = a;
    a = 0x50;
    z = (a) == 0;
    RAM(OBJ_X + y) = a;
    if (!z) { mff_cycles(0xB57F, 1); goto L_B591; }
    mff_cycles(0xB581, 7);
    a = RAM(ROUND);
    m = 0x02; c = a >= m;
    if (!c) { mff_cycles(0xB543, 1); goto L_B543; }
    mff_cycles(0xB587, 14);
    a = 0x02;
    RAM(OBJ_DIR + y) = a;
    a = 0xB0;
    RAM(OBJ_X + y) = a;
L_B591:;
    mff_cycles(0xB591, 49);
    a = 0x01;
    RAM(OBJ_Y + y) = a;
    a = 0x8C;
    RAM(OBJ_TIMER2 + y) = a;
    a = 0x00;
    RAM(OBJ_VX + y) = a;
    RAM(OBJ_VX_FRAC + y) = a;
    RAM(OBJ_VY + y) = a;
    RAM(OBJ_VY_FRAC + y) = a;
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_KIND + y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B581 (bank 5) - lifted by port/tools/lift.py */
void func_B581_b5(void) {
    REGS_IN;
L_B581:;
    mff_cycles(0xB581, 7);
    a = RAM(ROUND);
    m = 0x02; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xB585, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1543), 5); return; }
    mff_cycles(0xB587, 63);
    a = 0x02;
    RAM(OBJ_DIR + y) = a;
    a = 0xB0;
    RAM(OBJ_X + y) = a;
    a = 0x01;
    RAM(OBJ_Y + y) = a;
    a = 0x8C;
    RAM(OBJ_TIMER2 + y) = a;
    a = 0x00;
    RAM(OBJ_VX + y) = a;
    RAM(OBJ_VX_FRAC + y) = a;
    RAM(OBJ_VY + y) = a;
    RAM(OBJ_VY_FRAC + y) = a;
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_KIND + y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B5AF (bank 5) - lifted by port/tools/lift.py */
void func_B5AF_b5(void) {
    REGS_IN;
L_B5AF:;
    mff_cycles(0xB5AF, 14);
    a = RAM(RNG);
    m = RAM(0x63); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    a &= 0x07;
    y = a;
    a = nes_read((uint16_t)(0xB65E + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB5B9, 2);
    if (z) { mff_cycles(0xB5B9, 1); goto L_B5D6; }
    mff_cycles(0xB5BB, 38);
    y = RAM(0x02);
    a = 0x01;
    RAM(OBJ_VX + y) = a;
    a = 0x02;
    RAM(OBJ_VY + y) = a;
    a = 0x40;
    RAM(OBJ_VX_FRAC + y) = a;
    a = 0x80;
    RAM(OBJ_VY_FRAC + y) = a;
    a = 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_KIND + y) = a;
L_B5D6:;
    mff_cycles(0xB5D6, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B5D7 (bank 5) - lifted by port/tools/lift.py */
void func_B5D7_b5(void) {
    REGS_IN;
L_B5D7:;
    mff_cycles(0xB5D7, 6);
    a = RAM(OBJ_KIND + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xB5DA, 2); goto L_B625; }
    mff_cycles(0xB5DC, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x15DE, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB5DF, 10);
    a = RAM(OBJ_Y + x);
    m = RAM(OBJ_TIMER2 + x); c = a >= m;
    if (!c) { mff_cycles(0xB5E5, 2); goto L_B625; }
    mff_cycles(0xB5E7, 17);
    a = RAM(OBJ_TIMER2 + x);
    RAM(OBJ_Y + x) = a;
    a = RAM(OBJ_KIND + x);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB5F2, 2); goto L_B601; }
    mff_cycles(0xB5F4, 17);
    a = 0x02;
    RAM(OBJ_VY + x) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_VY_FRAC + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB625, 2); call_by_address_tail(0xB625, 5); return;
L_B601:;
    mff_cycles(0xB601, 15);
    t = (uint8_t)(RAM(OBJ_GROUND_Y + x) + 1); RAM(OBJ_GROUND_Y + x) = t;
    a = RAM(OBJ_GROUND_Y + x);
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB609, 1); goto L_B616; }
    mff_cycles(0xB60B, 20);
    a = 0x02;
    RAM(OBJ_VY + x) = a;
    a = 0x05;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_VY_FRAC + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_B616:;
    mff_cycles(0xB616, 21);
    a = 0x01;
    RAM(OBJ_VX + x) = a;
    a = 0x00;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0x00;
    RAM(OBJ_KIND + x) = a;
L_B625:;
    mff_cycles(0xB625, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xB62A, 1); goto L_B632; }
    mff_cycles(0xB62C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x162E, func_F090, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB62F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1631, func_F183, 0xE000)) return;
    SYNC_IN;
L_B632:;
    mff_cycles(0xB632, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xB637, 1); goto L_B63F; }
    mff_cycles(0xB639, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x163B, func_F099, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB63C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x163E, func_F1A1, 0xE000)) return;
    SYNC_IN;
L_B63F:;
    mff_cycles(0xB63F, 10);
    a = RAM(OBJ_X_HI + x);
    m = RAM(OBJ_X_HI); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB645, 1); goto L_B64D; }
    mff_cycles(0xB647, 9);
    t = (uint8_t)(RAM(OBJ_GROUND_Y + 15) - 1); RAM(OBJ_GROUND_Y + 15) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF2BE, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F2BE(); g_code_window_base = swb; } return;
L_B64D:;
    mff_cycles(0xB64D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B625 (bank 5) - lifted by port/tools/lift.py */
void func_B625_b5(void) {
    REGS_IN;
L_B625:;
    mff_cycles(0xB625, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xB62A, 1); goto L_B632; }
    mff_cycles(0xB62C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x162E, func_F090, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB62F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1631, func_F183, 0xE000)) return;
    SYNC_IN;
L_B632:;
    mff_cycles(0xB632, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xB637, 1); goto L_B63F; }
    mff_cycles(0xB639, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x163B, func_F099, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB63C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x163E, func_F1A1, 0xE000)) return;
    SYNC_IN;
L_B63F:;
    mff_cycles(0xB63F, 10);
    a = RAM(OBJ_X_HI + x);
    m = RAM(OBJ_X_HI); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB645, 1); goto L_B64D; }
    mff_cycles(0xB647, 9);
    t = (uint8_t)(RAM(OBJ_GROUND_Y + 15) - 1); RAM(OBJ_GROUND_Y + 15) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF2BE, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F2BE(); g_code_window_base = swb; } return;
L_B64D:;
    mff_cycles(0xB64D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B696 (bank 5) - lifted by port/tools/lift.py */
void func_B696_b5(void) {
    REGS_IN;
L_B696:;
    mff_cycles(0xB696, 12);
    RAM(OBJ_SPAWN_GROUP + 15) = x;
    a = RAM(OBJ_STATE + x);
    m = 0x8A; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB69E, 1); goto L_B6A6; }
    mff_cycles(0xB6A0, 9);
    a = RAM(OBJ_Y + x);
    RAM(OBJ_GROUND_Y + x) = a;
L_B6A6:;
    mff_cycles(0xB6A6, 6);
    a = RAM(OBJ_INVULN + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xB6A9, 1); goto L_B6AE; }
    mff_cycles(0xB6AB, 7);
    t = (uint8_t)(RAM(OBJ_INVULN + x) - 1); RAM(OBJ_INVULN + x) = t;
L_B6AE:;
    mff_cycles(0xB6AE, 22);
    a = 0x0B;
    RAM(OBJ_KIND + x) = a;
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_ID + 13);
    m = 0x6E; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB6BD, 1); goto L_B6C7; }
    mff_cycles(0xB6BF, 9);
    t = (uint8_t)(RAM(OBJ_INVULN + x) + 1); RAM(OBJ_INVULN + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (z) { mff_cycles(0xB6C2, 1); goto L_B6C7; }
    mff_cycles(0xB6C4, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB743, 2); call_by_address_tail(0xB743, 5); return;
L_B6C7:;
    mff_cycles(0xB6C7, 6);
    a = RAM(OBJ_HIT_TIMER + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xB6CA, 1); goto L_B6D8; }
    mff_cycles(0xB6CC, 10);
    a = 0xBC;
    y = 0x38;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x16D2, func_F449, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB6D3, 7);
    a = 0x81;
    RAM(OBJ_STATE + x) = a;
L_B6D8:;
    mff_cycles(0xB6D8, 6);
    a = RAM(OBJ_HELD + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xB6DB, 2); goto L_B74A; }
    mff_cycles(0xB6DD, 7);
    a = RAM(GAME_ACTION);
    m = 0x21; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB6E1, 1); goto L_B6E7; }
    mff_cycles(0xB6E3, 4);
    m = 0x04; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB6E5, 2); goto L_B74A; }
L_B6E7:;
    mff_cycles(0xB6E7, 5);
    m = RAM(0x39); c = x >= m;
    z = ((uint8_t)(x - m)) == 0;
    if (!z) { mff_cycles(0xB6E9, 2); goto L_B74A; }
    mff_cycles(0xB6EB, 8);
    a = RAM(OBJ_LIFE_MAX + x);
    m = 0x26; c = a >= m;
    if (!c) { mff_cycles(0xB6F0, 2); goto L_B721; }
    mff_cycles(0xB6F2, 7);
    a = RAM(GAME_ACTION);
    m = 0x21; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB6F6, 2); goto L_B707; }
    mff_cycles(0xB6F8, 10);
    a = 0xC3;
    y = 0x3A;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x16FE, func_F449, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB6FF, 10);
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF343, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F343(); g_code_window_base = swb; } return;
L_B707:;
    mff_cycles(0xB707, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(0xA3) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x170D, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB70E, 12);
    a = 0x80;
    RAM(OBJ_STATE + x) = a;
    a = RAM(0x38);
    n = (a) >> 7;
    if (n) { mff_cycles(0xB715, 1); goto L_B743; }
    mff_cycles(0xB717, 4);
    m = 0x0C; c = a >= m;
    if (!c) { mff_cycles(0xB719, 1); goto L_B743; }
    mff_cycles(0xB71B, 7);
    a = 0x0C;
    z = (a) == 0;
    RAM(0x38) = a;
    if (!z) { mff_cycles(0xB71F, 1); goto L_B743; }
L_B721:;
    mff_cycles(0xB721, 30);
    a = 0x00;
    RAM(GAME_ACTION) = a;
    RAM(OBJ_HIT_TIMER + x) = a;
    RAM(OBJ_HELD + x) = a;
    RAM(0x38) = a;
    t = (uint8_t)(RAM(0x38) - 1); RAM(0x38) = t;
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xBD7C + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB734, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1736, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB737, 16);
    a = 0x20;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x88;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    if (!z) { mff_cycles(0xB741, 1); goto L_B74A; }
L_B743:;
    mff_cycles(0xB743, 7);
    a = 0xBC;
    y = 0x38;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF449, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F449(); g_code_window_base = swb; } return;
L_B74A:;
    mff_cycles(0xB74A, 8);
    a = RAM(OBJ_LIFE_MAX + x);
    m = 0x5A; c = a >= m;
    if (c) { mff_cycles(0xB74F, 1); goto L_B78C; }
    mff_cycles(0xB751, 6);
    a = RAM(OBJ_TIMER2 + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xB754, 1); goto L_B78C; }
    mff_cycles(0xB756, 6);
    a = RAM(OBJ_INVULN + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xB759, 1); goto L_B77B; }
    mff_cycles(0xB75B, 46);
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
    a = 0xFF;
    RAM(OBJ_INVULN + x) = a;
L_B77B:;
    mff_cycles(0xB77B, 20);
    a = 0x80;
    RAM(OBJ_STATE + x) = a;
    RAM(OBJ_SAVED_STATE + x) = a;
    a = 0xF2;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1787, func_F34A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB788, 13);
    t = (uint8_t)(RAM(OBJ_TIMER2 + x) + 1); RAM(OBJ_TIMER2 + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_B78C:;
    mff_cycles(0xB78C, 8);
    a = RAM(OBJ_TIMER2 + x);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB791, 1); goto L_B7B8; }
    mff_cycles(0xB793, 5);
    a = RAM(GAME_ACTION);
    z = (a) == 0;
    if (z) { mff_cycles(0xB795, 1); goto L_B7A5; }
    mff_cycles(0xB797, 4);
    m = 0x03; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB799, 1); goto L_B7A5; }
    mff_cycles(0xB79B, 4);
    m = 0x17; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB79D, 1); goto L_B7A5; }
    mff_cycles(0xB79F, 13);
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_INVULN + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_B7A5:;
    mff_cycles(0xB7A5, 37);
    a = 0x3C;
    RAM(0x62) = a;
    a = 0x80;
    RAM(OBJ_STATE + x) = a;
    RAM(OBJ_SAVED_STATE + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER2 + x) + 1); RAM(OBJ_TIMER2 + x) = t;
    t = (uint8_t)(RAM(OBJ_TIER + x) + 1); RAM(OBJ_TIER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_B7B8:;
    mff_cycles(0xB7B8, 8);
    a = RAM(OBJ_STATE + x);
    m = 0x8A; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB7BD, 1); goto L_B7D7; }
    mff_cycles(0xB7BF, 8);
    a = RAM(OBJ_Y + x);
    m = 0x82; c = a >= m;
    if (c) { mff_cycles(0xB7C4, 1); goto L_B7CB; }
    mff_cycles(0xB7C6, 7);
    a = 0x82;
    RAM(OBJ_Y + x) = a;
L_B7CB:;
    mff_cycles(0xB7CB, 8);
    a = RAM(OBJ_Y + x);
    m = 0xAE; c = a >= m;
    if (!c) { mff_cycles(0xB7D0, 1); goto L_B7D7; }
    mff_cycles(0xB7D2, 7);
    a = 0xAE;
    RAM(OBJ_Y + x) = a;
L_B7D7:;
    mff_cycles(0xB7D7, 6);
    a = RAM(OBJ_TIER + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xB7DA, 1); goto L_B7EF; }
    mff_cycles(0xB7DC, 12);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    y = a;
    a = nes_read((uint16_t)(0xBCE6 + y));
    mff_cycles(0xB7E5, 7);
    RAM(0x00) = a;
    a = nes_read((uint16_t)(0xBCF7 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB7EA, 8);
    RAM(0x01) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
L_B7EF:;
    mff_cycles(0xB7EF, 7);
    a = 0x06;
    m = RAM(0xD9); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB7F3, 1); goto L_B7F8; }
    mff_cycles(0xB7F5, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x17F7, func_F34A, 0xE000)) return;
    SYNC_IN;
L_B7F8:;
    mff_cycles(0xB7F8, 12);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    y = a;
    a = nes_read((uint16_t)(0xBCEC + y));
    mff_cycles(0xB801, 7);
    RAM(0x00) = a;
    a = nes_read((uint16_t)(0xBCFD + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB806, 8);
    RAM(0x01) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $B743 (bank 5) - lifted by port/tools/lift.py */
void func_B743_b5(void) {
    REGS_IN;
L_B743:;
    mff_cycles(0xB743, 7);
    a = 0xBC;
    y = 0x38;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF449, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F449(); g_code_window_base = swb; } return;
}

/*  $B80B (bank 5) - lifted by port/tools/lift.py */
void func_B80B_b5(void) {
    REGS_IN;
L_B80B:;
    mff_cycles(0xB80B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x180D, 0xBBE1, 5)) return;
    SYNC_IN;
    mff_cycles(0xB80E, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1810, 0xBCD9, 5)) return;
    SYNC_IN;
    mff_cycles(0xB811, 35);
    a = 0x00;
    RAM(OBJ_AI_COUNT + x) = a;
    RAM(OBJ_AI_TIMER + x) = a;
    RAM(OBJ_AI_FLAG + x) = a;
    RAM(OBJ_TIMER2 + x) = a;
    a = 0x83;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B84B (bank 5) - lifted by port/tools/lift.py */
void func_B84B_b5(void) {
    REGS_IN;
L_B84B:;
    mff_cycles(0xB84B, 4);
    a = nes_read((uint16_t)(0xFBF0 + y));
    mff_cycles(0xB84E, 9);
    RAM(OBJ_DIR + x) = a;
    a = nes_read((uint16_t)(0xBD1C + y));
    mff_cycles(0xB854, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = nes_read((uint16_t)(0xBD0C + y));
    mff_cycles(0xB85A, 9);
    RAM(OBJ_VX + x) = a;
    a = nes_read((uint16_t)(0xBD3C + y));
    mff_cycles(0xB860, 9);
    RAM(OBJ_VY_FRAC + x) = a;
    a = nes_read((uint16_t)(0xBD2C + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB866, 11);
    RAM(OBJ_VY + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x186B, 0xBC6B, 5)) return;
    SYNC_IN;
    mff_cycles(0xB86C, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBBE1, 2); call_by_address_tail(0xBBE1, 5); return;
}

/*  $B88C (bank 5) - lifted by port/tools/lift.py */
void func_B88C_b5(void) {
    REGS_IN;
L_B88C:;
    mff_cycles(0xB88C, 7);
    RAM(OBJ_TIMER + x) = a;
    if (!n) { mff_cycles(0xB88F, 1); goto L_B8B8; }
    mff_cycles(0xB891, 10);
    a = 0x1C;
    y = 0x04;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1897, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB898, 2);
    if (c) { mff_cycles(0xB898, 1); goto L_B8C4; }
    mff_cycles(0xB89A, 10);
    a = 0x38;
    y = 0x18;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x18A0, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB8A1, 2);
    if (c) { mff_cycles(0xB8A1, 1); goto L_B8AC; }
    mff_cycles(0xB8A3, 20);
    a = 0x00;
    RAM(OBJ_AI_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) - 1); RAM(OBJ_STATE + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_B8AC:;
    mff_cycles(0xB8AC, 12);
    a = RAM(OBJ_TIMER + x);
    a &= 0x0F;
    y = a;
    a = nes_read((uint16_t)(0xBD6C + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xB8B5, 5);
    RAM(OBJ_AI_TIMER + x) = a;
L_B8B8:;
    mff_cycles(0xB8B8, 6);
    SYNC_OUT;
    if (!mff_call_at(0x18BA, 0xBCD9, 5)) return;
    SYNC_IN;
    mff_cycles(0xB8BB, 11);
    a = RAM(OBJ_TIMER + x);
    a &= 0x0F;
    y = a;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xB84B, 2); call_by_address_tail(0xB84B, 5); return;
L_B8C4:;
    mff_cycles(0xB8C4, 21);
    a = 0x00;
    RAM(OBJ_AI_COUNT + x) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1); RAM(OBJ_STATE + x) = t;
    a = 0xC0;
    y = 0x39;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF449, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F449(); g_code_window_base = swb; } return;
}

/*  $B929 (bank 5) - lifted by port/tools/lift.py */
void func_B929_b5(void) {
    REGS_IN;
L_B929:;
    mff_cycles(0xB929, 6);
    SYNC_OUT;
    if (!mff_call_at(0x192B, 0xBBE1, 5)) return;
    SYNC_IN;
    mff_cycles(0xB92C, 6);
    SYNC_OUT;
    if (!mff_call_at(0x192E, 0xBCD9, 5)) return;
    SYNC_IN;
    mff_cycles(0xB92F, 8);
    a = RAM(OBJ_X_HI + x);
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB934, 1); goto L_B952; }
    mff_cycles(0xB936, 15);
    a = 0x01;
    RAM(OBJ_DIR + x) = a;
    a = RAM(OBJ_X_HI + x);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xB940, 1); goto L_B947; }
    mff_cycles(0xB942, 7);
    a = 0x02;
    RAM(OBJ_DIR + x) = a;
L_B947:;
    mff_cycles(0xB947, 20);
    a = 0x00;
    RAM(OBJ_DOWN_TIMER + x) = a;
    a = 0x82;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_B952:;
    mff_cycles(0xB952, 13);
    a = 0x83;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $B9B7 (bank 5) - lifted by port/tools/lift.py */
void func_B9B7_b5(void) {
    REGS_IN;
L_B9B7:;
    mff_cycles(0xB9B7, 6);
    a = RAM(OBJ_DOWN_TIMER + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xB9BA, 2); goto L_BA0A; }
    mff_cycles(0xB9BC, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1); RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    if (!n) { mff_cycles(0xB9BF, 2); goto L_BA0A; }
    mff_cycles(0xB9C1, 8);
    a = RAM(OBJ_AI_COUNT + x);
    m = 0x05; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xB9C6, 1); goto L_B9F8; }
    mff_cycles(0xB9C8, 10);
    a = 0x68;
    y = 0x0E;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x19CE, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB9CF, 2);
    if (!c) { mff_cycles(0xB9CF, 1); goto L_B9DC; }
    mff_cycles(0xB9D1, 14);
    a = 0x00;
    RAM(OBJ_AI_COUNT + x) = a;
    a = RAM(RNG);
    a &= 0x01;
    z = (a) == 0;
    if (!z) { mff_cycles(0xB9DA, 2); goto L_BA3D; }
L_B9DC:;
    mff_cycles(0xB9DC, 38);
    a = 0x5A;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x00;
    RAM(OBJ_AI_COUNT + x) = a;
    a = 0x84;
    RAM(OBJ_TIMER + x) = a;
    a = 0x87;
    RAM(OBJ_STATE + x) = a;
    a = 0xC2;
    y = 0x33;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x19F6, func_F449, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB9F7, 6);
    SYNC_OUT;
    mff_rts(); return;
L_B9F8:;
    mff_cycles(0xB9F8, 10);
    a = 0xBF;
    y = 0x38;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x19FE, func_F449, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xB9FF, 20);
    a = 0x84;
    RAM(OBJ_TIMER + x) = a;
    a = 0x86;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_BA0A:;
    mff_cycles(0xBA0A, 8);
    a = RAM(OBJ_DIR + x);
    m = 0x08; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xBA0F, 1); goto L_BA27; }
    mff_cycles(0xBA11, 8);
    y = 0x01;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1A15, func_F0F7, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBA16, 7);
    a = RAM(0x10);
    a &= 0x50;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBA1A, 1); goto L_BA3C; }
    mff_cycles(0xBA1C, 6);
    a = RAM(OBJ_DOWN_TIMER + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xBA1F, 1); goto L_BA3D; }
    mff_cycles(0xBA21, 13);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_BA27:;
    mff_cycles(0xBA27, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1A2B, func_F0A2, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBA2C, 7);
    a = RAM(0x10);
    a &= 0x50;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBA30, 1); goto L_BA3C; }
    mff_cycles(0xBA32, 6);
    a = RAM(OBJ_DOWN_TIMER + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xBA35, 1); goto L_BA3D; }
    mff_cycles(0xBA37, 7);
    a = 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DIR + x) = a;
L_BA3C:;
    mff_cycles(0xBA3C, 6);
    SYNC_OUT;
    mff_rts(); return;
L_BA3D:;
    mff_cycles(0xBA3D, 8);
    a = RAM(OBJ_FLAGS + x);
    a &= 0x40;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBA42, 1); goto L_BA48; }
    mff_cycles(0xBA44, 4);
    a = 0x02;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBA46, 1); goto L_BA4A; }
L_BA48:;
    mff_cycles(0xBA48, 2);
    a = 0x01;
L_BA4A:;
    mff_cycles(0xBA4A, 34);
    RAM(OBJ_DIR + x) = a;
    a = 0x00;
    RAM(OBJ_DOWN_TIMER + x) = a;
    a = 0x24;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x89;
    RAM(OBJ_STATE + x) = a;
    a = 0x12;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1A60, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBA61, 7);
    a = 0xC0;
    y = 0x39;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF449, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F449(); g_code_window_base = swb; } return;
}

/*  $BB0A (bank 5) - lifted by port/tools/lift.py */
void func_BB0A_b5(void) {
    REGS_IN;
L_BB0A:;
    mff_cycles(0xBB0A, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x03; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xBB0F, 1); goto L_BB22; }
    mff_cycles(0xBB11, 6);
    a = RAM(OBJ_ANIM_TIMER + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xBB14, 1); goto L_BB22; }
    mff_cycles(0xBB16, 16);
    a = 0x85;
    RAM(OBJ_ATTACK + x) = a;
    a = 0x11;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    if (!z) { mff_cycles(0xBB20, 1); goto L_BB27; }
L_BB22:;
    mff_cycles(0xBB22, 7);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ATTACK + x) = a;
L_BB27:;
    mff_cycles(0xBB27, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BB8C (bank 5) - lifted by port/tools/lift.py */
void func_BB8C_b5(void) {
    REGS_IN;
L_BB8C:;
    mff_cycles(0xBB8C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1B8E, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBB8F, 13);
    t = (uint8_t)(RAM(OBJ_HIT_TIMER + x) - 1); RAM(OBJ_HIT_TIMER + x) = t;
    a = RAM(OBJ_HIT_TIMER + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xBB95, 1); goto L_BB9C; }
    mff_cycles(0xBB97, 7);
    a = 0x80;
    RAM(OBJ_STATE + x) = a;
L_BB9C:;
    mff_cycles(0xBB9C, 4);
    m = 0x1C; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xBB9E, 1); goto L_BBAB; }
    mff_cycles(0xBBA0, 4);
    m = 0x10; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xBBA2, 1); goto L_BBAB; }
    mff_cycles(0xBBA4, 10);
    a = 0xC3;
    y = 0x3A;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1BAA, func_F449, 0xE000)) return;
    SYNC_IN;
L_BBAB:;
    mff_cycles(0xBBAB, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BBE1 (bank 5) - lifted by port/tools/lift.py */
void func_BBE1_b5(void) {
    REGS_IN;
L_BBE1:;
    mff_cycles(0xBBE1, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1BE3, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBBE4, 2);
    if (c) { mff_cycles(0xBBE4, 1); goto L_BBE9; }
    mff_cycles(0xBBE6, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF099, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F099(); g_code_window_base = swb; } return;
L_BBE9:;
    mff_cycles(0xBBE9, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF090, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F090(); g_code_window_base = swb; } return;
}

/*  $BBEC (bank 5) - lifted by port/tools/lift.py */
void func_BBEC_b5(void) {
    REGS_IN;
L_BBEC:;
    mff_cycles(0xBBEC, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1BEE, func_F71A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBBEF, 2);
    if (c) { mff_cycles(0xBBEF, 2); goto L_BC6A; }
    mff_cycles(0xBBF1, 11);
    RAM(0x00) = y;
    a = 0xC7;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1BF7, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBBF8, 8);
    a = RAM(OBJ_FLAGS + x);
    a &= 0x40;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBBFD, 2); goto L_BC08; }
    mff_cycles(0xBBFF, 11);
    a = 0x90;
    RAM(OBJ_FLAGS + y) = a;
    a = 0x02;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBC06, 1); goto L_BC0F; }
L_BC08:;
    mff_cycles(0xBC08, 9);
    a = 0xD0;
    RAM(OBJ_FLAGS + y) = a;
    a = 0x01;
L_BC0F:;
    mff_cycles(0xBC0F, 19);
    RAM(OBJ_DIR + y) = a;
    a &= 0x02;
    y = a;
    a = RAM(OBJ_X + x);
    c = 0;
    m = nes_read((uint16_t)(0xBD08 + y)); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    mff_cycles(0xBC1C, 11);
    mff_push(a);
    a = RAM(OBJ_X_HI + x);
    m = nes_read((uint16_t)(0xBD09 + y)); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    mff_cycles(0xBC23, 108);
    y = RAM(0x00);
    RAM(OBJ_X_HI + y) = a;
    a = mff_pull();
    RAM(OBJ_X + y) = a;
    a = RAM(OBJ_Y + x);
    c = 1;
    m = 0x03; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    RAM(OBJ_Y + y) = a;
    a = RAM(OBJ_Y_HI + x);
    RAM(OBJ_Y_HI + y) = a;
    a = 0x9D;
    RAM(OBJ_ID + y) = a;
    RAM(OBJ_SAVED_ID + y) = a;
    a = 0x03;
    RAM(OBJ_VX + y) = a;
    a = 0x80;
    RAM(OBJ_VX_FRAC + y) = a;
    a = 0x00;
    RAM(OBJ_LIFE_MAX + y) = a;
    RAM(OBJ_LIFE + y) = a;
    RAM(OBJ_VY + y) = a;
    RAM(OBJ_VY_FRAC + y) = a;
    a = 0x0C;
    RAM(OBJ_KIND + y) = a;
    a = 0x87;
    RAM(OBJ_ATTACK + y) = a;
    a = 0x34;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ATTACK_DAMAGE + y) = a;
L_BC6A:;
    mff_cycles(0xBC6A, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BC6B (bank 5) - lifted by port/tools/lift.py */
void func_BC6B_b5(void) {
    REGS_IN;
L_BC6B:;
    mff_cycles(0xBC6B, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBC70, 1); goto L_BC90; }
    mff_cycles(0xBC72, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1C74, func_F1DF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBC75, 8);
    a = RAM(OBJ_Y + x);
    m = 0x82; c = a >= m;
    if (c) { mff_cycles(0xBC7A, 1); goto L_BC90; }
    mff_cycles(0xBC7C, 13);
    a = 0x82;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1C83, 0xBCC8, 5)) return;
    SYNC_IN;
    mff_cycles(0xBC84, 6);
    a = RAM(OBJ_AI_FLAG + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xBC87, 1); goto L_BCAE; }
L_BC89:;
    mff_cycles(0xBC89, 9);
    a = 0x02;
    z = (a) == 0;
    RAM(OBJ_DIR + x) = a;
    if (!z) { mff_cycles(0xBC8E, 1); goto L_BCB3; }
L_BC90:;
    mff_cycles(0xBC90, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBC95, 1); goto L_BCB3; }
    mff_cycles(0xBC97, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1C99, func_F1BF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBC9A, 8);
    a = RAM(OBJ_Y + x);
    m = 0xAE; c = a >= m;
    if (!c) { mff_cycles(0xBC9F, 1); goto L_BCB3; }
    mff_cycles(0xBCA1, 13);
    a = 0xAE;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1CA8, 0xBCC8, 5)) return;
    SYNC_IN;
    mff_cycles(0xBCA9, 6);
    a = RAM(OBJ_AI_FLAG + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xBC89, 1); goto L_BC89; }
L_BCAE:;
    mff_cycles(0xBCAE, 7);
    a = 0x01;
    RAM(OBJ_DIR + x) = a;
L_BCB3:;
    mff_cycles(0xBCB3, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBCB8, 1); goto L_BCBD; }
    mff_cycles(0xBCBA, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1CBC, func_F183, 0xE000)) return;
    SYNC_IN;
L_BCBD:;
    mff_cycles(0xBCBD, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xBCC2, 1); goto L_BCC7; }
    mff_cycles(0xBCC4, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1CC6, func_F1A1, 0xE000)) return;
    SYNC_IN;
L_BCC7:;
    mff_cycles(0xBCC7, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BCC8 (bank 5) - lifted by port/tools/lift.py */
void func_BCC8_b5(void) {
    REGS_IN;
L_BCC8:;
    mff_cycles(0xBCC8, 30);
    a = 0x03;
    RAM(OBJ_VX + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_VX_FRAC + x) = a;
    RAM(OBJ_VY_FRAC + x) = a;
    RAM(OBJ_VY + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BCD9 (bank 5) - lifted by port/tools/lift.py */
void func_BCD9_b5(void) {
    REGS_IN;
L_BCD9:;
    mff_cycles(0xBCD9, 8);
    a = 0xBD;
    m = RAM(OBJ_ANIM + x); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBCDE, 1); goto L_BCE5; }
    mff_cycles(0xBCE0, 8);
    y = 0x38;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1CE4, func_F449, 0xE000)) return;
    SYNC_IN;
L_BCE5:;
    mff_cycles(0xBCE5, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BD7F (bank 5) - lifted by port/tools/lift.py */
void func_BD7F_b5(void) {
    REGS_IN;
L_BD7F:;
    mff_cycles(0xBD7F, 24);
    a = 0xF9;
    RAM(0x62) = a;
    a = 0x01;
    RAM(OBJ_INVULN + x) = a;
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    y = a;
    a = nes_read((uint16_t)(0xBF9F + y));
    mff_cycles(0xBD91, 7);
    RAM(0x00) = a;
    a = nes_read((uint16_t)(0xBFA7 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xBD96, 8);
    RAM(0x01) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $BD9B (bank 5) - lifted by port/tools/lift.py */
void func_BD9B_b5(void) {
    REGS_IN;
L_BD9B:;
    mff_cycles(0xBD9B, 23);
    a = 0x80;
    RAM(JOY_PRESSED) = a;
    RAM(JOY_HELD) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(OBJ_SPAWN_GROUP + 15) = x;
    SYNC_OUT;
    if (!mff_call_at(0x1DAA, 0xC9B9, 5)) return;
    SYNC_IN;
    mff_cycles(0xBDAB, 19);
    x = RAM(OBJ_SPAWN_GROUP + 15);
    a = 0x28;
    RAM(GAME_ACTION) = a;
    a = 0xCD;
    y = 0x34;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1DB8, func_F449, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBDB9, 10);
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1); RAM(OBJ_STATE + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBFAF, 2); call_by_address_tail(0xBFAF, 5); return;
}

/*  $BDBF (bank 5) - lifted by port/tools/lift.py */
void func_BDBF_b5(void) {
    REGS_IN;
L_BDBF:;
    mff_cycles(0xBDBF, 31);
    a = RAM(0x31);
    a ^= 0xFF;
    a &= 0x03;
    RAM(0x32) = a;
    RAM(JOY_PRESSED) = a;
    RAM(JOY_HELD) = a;
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(OBJ_SPAWN_GROUP + 15) = x;
    SYNC_OUT;
    if (!mff_call_at(0x1DD4, 0xC9B9, 5)) return;
    SYNC_IN;
    mff_cycles(0xBDD5, 11);
    x = RAM(OBJ_SPAWN_GROUP + 15);
    a = RAM(GAME_ACTION);
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xBDDC, 1); goto L_BDE1; }
    mff_cycles(0xBDDE, 7);
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1); RAM(OBJ_STATE + x) = t;
L_BDE1:;
    mff_cycles(0xBDE1, 11);
    a = 0x28;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BDE6 (bank 5) - lifted by port/tools/lift.py */
void func_BDE6_b5(void) {
    REGS_IN;
L_BDE6:;
    mff_cycles(0xBDE6, 10);
    y = RAM(OBJ_TIMER2 + x);
    a = RAM(OBJ_STATE + y);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) { mff_cycles(0xBDEC, 2); goto L_BE22; }
    mff_cycles(0xBDEE, 13);
    a = 0xFF;
    RAM(0x62) = a;
    a = 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1DF6, func_F34A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBDF7, 44);
    a = 0x50;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x86;
    RAM(OBJ_STATE + x) = a;
    a = 0x83;
    RAM(OBJ_TIMER + x) = a;
    a = 0x00;
    RAM(OBJ_DOWN_TIMER) = a;
    RAM(0x6E) = a;
    RAM(0x6A) = a;
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(0x69) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1E15, 0xCED1, 5)) return;
    SYNC_IN;
    mff_cycles(0xBE16, 10);
    a = RAM(PLAYER_ID);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_GROUND_Y + x) = a;
    if (!z) { mff_cycles(0xBE1B, 1); goto L_BE22; }
    mff_cycles(0xBE1D, 6);
    a = 0xD0;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS) = a;
L_BE22:;
    mff_cycles(0xBE22, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BE23 (bank 5) - lifted by port/tools/lift.py */
void func_BE23_b5(void) {
    REGS_IN;
L_BE23:;
    mff_cycles(0xBE23, 15);
    a = RAM(OBJ_FLAGS);
    a &= 0xFB;
    RAM(OBJ_FLAGS) = a;
    a = RAM(PLAYER_ID);
    z = (a) == 0;
    if (z) { mff_cycles(0xBE2D, 1); goto L_BE36; }
    mff_cycles(0xBE2F, 4);
    m = 0x01; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBE31, 1); goto L_BE8E; }
    mff_cycles(0xBE33, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xBECE, 2); call_by_address_tail(0xBECE, 5); return;
L_BE36:;
    mff_cycles(0xBE36, 8);
    a = RAM(OBJ_DOWN_TIMER);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBE3B, 1); goto L_BE59; }
    mff_cycles(0xBE3D, 4);
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBE3F, 1); goto L_BE6F; }
    mff_cycles(0xBE41, 4);
    m = 0x03; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xBE43, 1); goto L_BE79; }
    mff_cycles(0xBE45, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1E47, func_F71A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBE48, 30);
    a = 0x80;
    RAM(OBJ_STATE + y) = a;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x9C;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ID + y) = a;
    RAM(OBJ_SAVED_ID + y) = a;
    SYNC_OUT;
    mff_rts(); return;
L_BE59:;
    mff_cycles(0xBE59, 8);
    a = RAM(OBJ_AI_TIMER + x);
    a &= 0x07;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBE5E, 1); goto L_BE65; }
    mff_cycles(0xBE60, 8);
    a = 0x13;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1E64, func_F34C, 0xE000)) return;
    SYNC_IN;
L_BE65:;
    mff_cycles(0xBE65, 25);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) + 1); RAM(OBJ_AI_TIMER + x) = t;
    t = (uint8_t)(RAM(OBJ_X) + 1); RAM(OBJ_X) = t;
    t = (uint8_t)(RAM(OBJ_X) + 1); RAM(OBJ_X) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_BE6F:;
    mff_cycles(0xBE6F, 8);
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1E73, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBE74, 5);
    a = 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF440, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F440(); g_code_window_base = swb; } return;
L_BE79:;
    mff_cycles(0xBE79, 36);
    t = (uint8_t)(RAM(0x6B) - 1); RAM(0x6B) = t;
    t = (uint8_t)(RAM(0x6B) - 1); RAM(0x6B) = t;
    a = 0x02;
    RAM(PLAYER_ID) = a;
    a = 0x19;
    RAM(0x0617) = a;
    a = 0x30;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(0x0616) = a;
    RAM(0x18) = a;
    SYNC_OUT;
    mff_rts(); return;
L_BE8E:;
    mff_cycles(0xBE8E, 23);
    a = 0x02;
    RAM(JOY_PRESSED) = a;
    RAM(JOY_HELD) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(OBJ_SPAWN_GROUP + 15) = x;
    SYNC_OUT;
    if (!mff_call_at(0x1E9D, 0xC9B9, 5)) return;
    SYNC_IN;
    mff_cycles(0xBE9E, 12);
    x = RAM(OBJ_SPAWN_GROUP + 15);
    a = RAM(OBJ_X);
    m = 0x1B; c = a >= m;
    if (c) { mff_cycles(0xBEA6, 1); goto L_BEC9; }
    mff_cycles(0xBEA8, 12);
    a = 0x94;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1EAF, 0xBE79, 5)) return;
    SYNC_IN;
    mff_cycles(0xBEB0, 33);
    a = 0x9A;
    RAM(OBJ_Y) = a;
    a = 0x00;
    RAM(OBJ_X) = a;
    a = 0x83;
    RAM(OBJ_TIMER + x) = a;
    a = 0x86;
    RAM(OBJ_STATE + x) = a;
    a = 0x80;
    RAM(OBJ_AI_TIMER + x) = a;
L_BEC9:;
    mff_cycles(0xBEC9, 11);
    a = 0x28;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BE79 (bank 5) - lifted by port/tools/lift.py */
void func_BE79_b5(void) {
    REGS_IN;
L_BE79:;
    mff_cycles(0xBE79, 36);
    t = (uint8_t)(RAM(0x6B) - 1); RAM(0x6B) = t;
    t = (uint8_t)(RAM(0x6B) - 1); RAM(0x6B) = t;
    a = 0x02;
    RAM(PLAYER_ID) = a;
    a = 0x19;
    RAM(0x0617) = a;
    a = 0x30;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(0x0616) = a;
    RAM(0x18) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BECE (bank 5) - lifted by port/tools/lift.py */
void func_BECE_b5(void) {
    REGS_IN;
L_BECE:;
    mff_cycles(0xBECE, 8);
    a = 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1ED2, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBED3, 8);
    a = RAM(OBJ_Y);
    m = 0x99; c = a >= m;
    if (c) { mff_cycles(0xBED8, 1); goto L_BEDE; }
    mff_cycles(0xBEDA, 4);
    a = 0x04;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBEDC, 1); goto L_BEF7; }
L_BEDE:;
    mff_cycles(0xBEDE, 4);
    m = 0x9B; c = a >= m;
    if (!c) { mff_cycles(0xBEE0, 1); goto L_BEE6; }
    mff_cycles(0xBEE2, 4);
    a = 0x08;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBEE4, 1); goto L_BEF7; }
L_BEE6:;
    mff_cycles(0xBEE6, 8);
    a = RAM(OBJ_X);
    m = 0x97; c = a >= m;
    if (c) { mff_cycles(0xBEEB, 1); goto L_BEF1; }
    mff_cycles(0xBEED, 4);
    a = 0x01;
    z = (a) == 0;
    if (!z) { mff_cycles(0xBEEF, 1); goto L_BEF7; }
L_BEF1:;
    mff_cycles(0xBEF1, 4);
    m = 0x99; c = a >= m;
    if (!c) { mff_cycles(0xBEF3, 2); goto L_BF0D; }
    mff_cycles(0xBEF5, 2);
    a = 0x02;
L_BEF7:;
    mff_cycles(0xBEF7, 21);
    RAM(JOY_PRESSED) = a;
    RAM(JOY_HELD) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(OBJ_SPAWN_GROUP + 15) = x;
    SYNC_OUT;
    if (!mff_call_at(0x1F04, 0xC9B9, 5)) return;
    SYNC_IN;
    mff_cycles(0xBF05, 15);
    x = RAM(OBJ_SPAWN_GROUP + 15);
    a = 0x28;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
L_BF0D:;
    mff_cycles(0xBF0D, 45);
    a = 0xD0;
    RAM(OBJ_FLAGS) = a;
    a = 0x0A;
    RAM(GAME_ACTION) = a;
    a = 0x7B;
    RAM(OBJ_ID + x) = a;
    a = 0x50;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x84;
    RAM(OBJ_TIMER + x) = a;
    a = 0x86;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BF2B (bank 5) - lifted by port/tools/lift.py */
void func_BF2B_b5(void) {
    REGS_IN;
L_BF2B:;
    mff_cycles(0xBF2B, 41);
    a = 0x90;
    RAM(OBJ_FLAGS + x) = a;
    a = 0x01;
    RAM(OBJ_VX + x) = a;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0xF8;
    RAM(OBJ_X + x) = a;
    a = 0x98;
    RAM(OBJ_Y + x) = a;
    a = 0x23;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1F46, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBF47, 33);
    a = 0x36;
    RAM(CHR_BANKS + 3) = a;
    a = 0x15;
    RAM(0x061B) = a;
    a = 0x26;
    RAM(0x061A) = a;
    RAM(0x18) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1); RAM(OBJ_STATE + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BF5B (bank 5) - lifted by port/tools/lift.py */
void func_BF5B_b5(void) {
    REGS_IN;
L_BF5B:;
    mff_cycles(0xBF5B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1F5D, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBF5E, 8);
    a = RAM(OBJ_X + x);
    m = 0xB0; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xBF63, 1); goto L_BF74; }
    mff_cycles(0xBF65, 8);
    a = 0x22;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1F69, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBF6A, 14);
    a = 0xFF;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x87;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
L_BF74:;
    mff_cycles(0xBF74, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BF75 (bank 5) - lifted by port/tools/lift.py */
void func_BF75_b5(void) {
    REGS_IN;
L_BF75:;
    mff_cycles(0xBF75, 9);
    a = RAM(OBJ_GROUND_Y + x);
    m = RAM(PLAYER_ID); c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xBF7A, 1); goto L_BF80; }
    mff_cycles(0xBF7C, 5);
    a = 0x00;
    RAM(0x62) = a;
L_BF80:;
    mff_cycles(0xBF80, 14);
    a = 0x28;
    RAM(GAME_ACTION) = a;
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1); RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) { mff_cycles(0xBF87, 1); goto L_BF8F; }
    mff_cycles(0xBF89, 9);
    a = RAM(OBJ_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
L_BF8F:;
    mff_cycles(0xBF8F, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BF90 (bank 5) - lifted by port/tools/lift.py */
void func_BF90_b5(void) {
    REGS_IN;
L_BF90:;
    mff_cycles(0xBF90, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1); RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) { mff_cycles(0xBF93, 1); goto L_BF9E; }
    mff_cycles(0xBF95, 12);
    a = 0x0E;
    RAM(GAME_ACTION) = a;
    a = RAM(OBJ_GROUND_Y + x);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PLAYER_ID) = a;
L_BF9E:;
    mff_cycles(0xBF9E, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $BFAF (bank 5) - lifted by port/tools/lift.py */
void func_BFAF_b5(void) {
    REGS_IN;
L_BFAF:;
    mff_cycles(0xBFAF, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1FB1, func_F71A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xBFB2, 2);
    if (c) { mff_cycles(0xBFB2, 1); goto L_BFED; }
    mff_cycles(0xBFB4, 84);
    a = RAM(OBJ_X + x);
    RAM(OBJ_X + y) = a;
    a = RAM(OBJ_Y + x);
    RAM(OBJ_Y + y) = a;
    a = 0x02;
    RAM(OBJ_X_HI + y) = a;
    a = 0xFF;
    RAM(0x1B) = a;
    a = 0x9E;
    RAM(OBJ_ID + y) = a;
    RAM(OBJ_SAVED_ID + y) = a;
    a = 0x80;
    RAM(OBJ_STATE + y) = a;
    a = 0xFF;
    RAM(OBJ_INVULN + y) = a;
    a = 0xE0;
    RAM(OBJ_AI_TIMER + y) = a;
    a = x;
    RAM(OBJ_TIMER + y) = a;
    a = 0x94;
    RAM(OBJ_FLAGS + y) = a;
    a = y;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER2 + x) = a;
L_BFED:;
    mff_cycles(0xBFED, 6);
    SYNC_OUT;
    mff_rts(); return;
}
