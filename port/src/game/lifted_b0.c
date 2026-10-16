/*
 * lifted_b0.c - bank 0 routines $8000-$BFFF, lifted by
 * port/tools/lift_bank.py from the pure scaffold: exact, still to be
 * tidied and moved into their systems' files.
 */
#include "mff.h"
#include "state.h"
#include "coroutine.h"

void func_F03F(void);
void func_F048(void);
void func_F090(void);
void func_F099(void);
void func_F119(void);
void func_F183(void);
void func_F1A1(void);
void func_F1BF(void);
void func_F1DF(void);
void func_F1FD(void);
void func_F26E(void);
void func_F288(void);
void func_F294(void);
void func_F2A0(void);
void func_F2BE(void);
void func_F32E(void);
void func_F343(void);
void func_F34A(void);
void func_F34C(void);
void func_F374(void);
void func_F406(void);
void func_F436(void);
void func_F43B(void);
void func_F440(void);
void func_F443(void);
void func_F449(void);
void func_F44F(void);
void func_F455(void);
void func_FB3E(void);
void func_FB5B(void);
void func_FB72(void);
void func_FBD9(void);
void func_FF2A(void);
void func_FF31(void);

/* player_action_dispatch $8000 (bank 0) - lifted by port/tools/lift.py */
void func_8000_b0(void) {
    REGS_IN;
L_8000:;
    mff_cycles(0x8000, 9);
    x = 0x00;
    y = RAM(GAME_ACTION);
    a = nes_read((uint16_t)(0x8011 + y));
    mff_cycles(0x8007, 7);
    RAM(0x00) = a;
    a = nes_read((uint16_t)(0x803B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x800C, 8);
    RAM(0x01) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $8065 (bank 0) - lifted by port/tools/lift.py */
void func_8065_b0(void) {
    REGS_IN;
L_8065:;
    mff_cycles(0x8065, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8066 (bank 0) - lifted by port/tools/lift.py */
void func_8066_b0(void) {
    REGS_IN;
L_8066:;
    mff_cycles(0x8066, 8);
    a = RAM(OBJ_TIMER);
    a &= 0x01;
    z = (a) == 0;
    if (!z) { mff_cycles(0x806B, 1); goto L_8077; }
    mff_cycles(0x806D, 13);
    a = RAM(SCROLL_Y);
    c = 1;
    m = 0x01; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SCROLL_Y) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x807E, 2); call_by_address_tail(0x807E, 0); return;
L_8077:;
    mff_cycles(0x8077, 23);
    a = RAM(SCROLL_Y);
    c = 0;
    m = 0x01; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(SCROLL_Y) = a;
    t = (uint8_t)(RAM(OBJ_TIMER) - 1); RAM(OBJ_TIMER) = t;
    a = RAM(PLAYER_DIR);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x8085, 1); goto L_808D; }
    mff_cycles(0x8087, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0089, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x808A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8090, 2); call_by_address_tail(0x8090, 0); return;
L_808D:;
    mff_cycles(0x808D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x008F, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8090, 8);
    y = 0x02;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0094, func_F119, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8095, 2);
    if (!c) { mff_cycles(0x8095, 1); goto L_809A; }
    mff_cycles(0x8097, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x80AE, 2); call_by_address_tail(0x80AE, 0); return;
L_809A:;
    mff_cycles(0x809A, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0x809D, 1); goto L_80C4; }
    mff_cycles(0x809F, 13);
    a = RAM(PLAYER_GROUND_Y);
    c = 0;
    m = 0x02; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    m = RAM(OBJ_Y); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x80A7, 1); goto L_80C4; }
    mff_cycles(0x80A9, 15);
    a = RAM(PLAYER_GROUND_Y);
    RAM(OBJ_Y) = a;
    a = 0x1C;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x00B2, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x80B3, 24);
    a = 0x00;
    RAM(SCROLL_Y) = a;
    RAM(JUMP_MOVING) = a;
    a = 0x02;
    RAM(GAME_ACTION) = a;
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ACTION_TIMER) = a;
    SYNC_OUT;
    if (!mff_call_at(0x00C3, 0xCEAA, 0)) return;
    SYNC_IN;
L_80C4:;
    mff_cycles(0x80C4, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $807E (bank 0) - lifted by port/tools/lift.py */
void func_807E_b0(void) {
    REGS_IN;
L_807E:;
    mff_cycles(0x807E, 13);
    t = (uint8_t)(RAM(OBJ_TIMER) - 1); RAM(OBJ_TIMER) = t;
    a = RAM(PLAYER_DIR);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x8085, 1); goto L_808D; }
    mff_cycles(0x8087, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0089, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x808A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8090, 2); call_by_address_tail(0x8090, 0); return;
L_808D:;
    mff_cycles(0x808D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x008F, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8090, 8);
    y = 0x02;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0094, func_F119, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8095, 2);
    if (!c) { mff_cycles(0x8095, 1); goto L_809A; }
    mff_cycles(0x8097, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x80AE, 2); call_by_address_tail(0x80AE, 0); return;
L_809A:;
    mff_cycles(0x809A, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0x809D, 1); goto L_80C4; }
    mff_cycles(0x809F, 13);
    a = RAM(PLAYER_GROUND_Y);
    c = 0;
    m = 0x02; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    m = RAM(OBJ_Y); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x80A7, 1); goto L_80C4; }
    mff_cycles(0x80A9, 15);
    a = RAM(PLAYER_GROUND_Y);
    RAM(OBJ_Y) = a;
    a = 0x1C;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x00B2, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x80B3, 24);
    a = 0x00;
    RAM(SCROLL_Y) = a;
    RAM(JUMP_MOVING) = a;
    a = 0x02;
    RAM(GAME_ACTION) = a;
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ACTION_TIMER) = a;
    SYNC_OUT;
    if (!mff_call_at(0x00C3, 0xCEAA, 0)) return;
    SYNC_IN;
L_80C4:;
    mff_cycles(0x80C4, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8090 (bank 0) - lifted by port/tools/lift.py */
void func_8090_b0(void) {
    REGS_IN;
L_8090:;
    mff_cycles(0x8090, 8);
    y = 0x02;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0094, func_F119, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8095, 2);
    if (!c) { mff_cycles(0x8095, 1); goto L_809A; }
    mff_cycles(0x8097, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x80AE, 2); call_by_address_tail(0x80AE, 0); return;
L_809A:;
    mff_cycles(0x809A, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0x809D, 1); goto L_80C4; }
    mff_cycles(0x809F, 13);
    a = RAM(PLAYER_GROUND_Y);
    c = 0;
    m = 0x02; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    m = RAM(OBJ_Y); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x80A7, 1); goto L_80C4; }
    mff_cycles(0x80A9, 15);
    a = RAM(PLAYER_GROUND_Y);
    RAM(OBJ_Y) = a;
    a = 0x1C;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x00B2, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x80B3, 24);
    a = 0x00;
    RAM(SCROLL_Y) = a;
    RAM(JUMP_MOVING) = a;
    a = 0x02;
    RAM(GAME_ACTION) = a;
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ACTION_TIMER) = a;
    SYNC_OUT;
    if (!mff_call_at(0x00C3, 0xCEAA, 0)) return;
    SYNC_IN;
L_80C4:;
    mff_cycles(0x80C4, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8193 (bank 0) - lifted by port/tools/lift.py */
void func_8193_b0(void) {
    REGS_IN;
L_8193:;
    mff_cycles(0x8193, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8198, 1); goto L_81FA; }
    mff_cycles(0x819A, 6);
    a = RAM(OBJ_ANIM_TIMER);
    z = (a) == 0;
    if (!z) { mff_cycles(0x819D, 1); goto L_81FA; }
    mff_cycles(0x819F, 2);
    y = 0x02;
L_81A1:;
    mff_cycles(0x81A1, 6);
    a = RAM(OBJ_STATE + y);
    n = (a) >> 7;
    if (!n) { mff_cycles(0x81A4, 1); goto L_81AC; }
    mff_cycles(0x81A6, 6);
    y--;
    m = 0x00; c = y >= m;
    n = ((uint8_t)(y - m)) >> 7;
    z = ((uint8_t)(y - m)) == 0;
    if (!z) { mff_cycles(0x81A1, 1); goto L_81A1; }
    mff_cycles(0x81AB, 6);
    SYNC_OUT;
    mff_rts(); return;
L_81AC:;
    mff_cycles(0x81AC, 25);
    RAM(0x0F) = y;
    a = RAM(PLAYER_DIR);
    RAM(OBJ_DIR + y) = a;
    a &= 0x02;
    y = a;
    a = RAM(OBJ_X);
    c = 0;
    m = nes_read((uint16_t)(0x8219 + y)); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    mff_cycles(0x81BD, 11);
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = nes_read((uint16_t)(0x821A + y)); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    mff_cycles(0x81C4, 47);
    y = RAM(0x0F);
    RAM(OBJ_X_HI + y) = a;
    a = mff_pull();
    RAM(OBJ_X + y) = a;
    a = RAM(OBJ_Y);
    c = 0;
    m = 0x04; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(OBJ_Y + y) = a;
    a = RAM(OBJ_Y_HI);
    RAM(OBJ_Y_HI + y) = a;
    a = 0x1B;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x01E0, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x81E1, 8);
    a = 0x4D;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x01E5, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x81E6, 29);
    a = 0x6C;
    RAM(OBJ_ID + y) = a;
    RAM(OBJ_SAVED_ID + y) = a;
    a = 0x0A;
    RAM(ATTACK_ID) = a;
    a = 0x00;
    RAM(OBJ_ATTACK + y) = a;
    RAM(OBJ_THROW_FLAG + y) = a;
L_81FA:;
    mff_cycles(0x81FA, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x09; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x81FF, 1); goto L_8218; }
    mff_cycles(0x8201, 8);
    a = RAM(OBJ_ANIM_TIMER);
    m = 0x02; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8206, 1); goto L_8218; }
    mff_cycles(0x8208, 15);
    a = 0x00;
    RAM(WEAPON_HELD) = a;
    RAM(GAME_ACTION) = a;
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8213, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0217, 0xCEAA, 0)) return;
    SYNC_IN;
L_8218:;
    mff_cycles(0x8218, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $821D (bank 0) - lifted by port/tools/lift.py */
void func_821D_b0(void) {
    REGS_IN;
L_821D:;
    mff_cycles(0x821D, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8222, 1); goto L_8284; }
    mff_cycles(0x8224, 6);
    a = RAM(OBJ_ANIM_TIMER);
    z = (a) == 0;
    if (!z) { mff_cycles(0x8227, 1); goto L_8284; }
    mff_cycles(0x8229, 2);
    y = 0x02;
L_822B:;
    mff_cycles(0x822B, 6);
    a = RAM(OBJ_STATE + y);
    n = (a) >> 7;
    if (!n) { mff_cycles(0x822E, 1); goto L_8236; }
    mff_cycles(0x8230, 6);
    y--;
    m = 0x00; c = y >= m;
    n = ((uint8_t)(y - m)) >> 7;
    z = ((uint8_t)(y - m)) == 0;
    if (!z) { mff_cycles(0x822B, 1); goto L_822B; }
    mff_cycles(0x8235, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8236:;
    mff_cycles(0x8236, 25);
    RAM(0x0F) = y;
    a = RAM(PLAYER_DIR);
    RAM(OBJ_DIR + y) = a;
    a &= 0x02;
    y = a;
    a = RAM(OBJ_X);
    c = 0;
    m = nes_read((uint16_t)(0x82AB + y)); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    mff_cycles(0x8247, 11);
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = nes_read((uint16_t)(0x82AC + y)); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    mff_cycles(0x824E, 47);
    y = RAM(0x0F);
    RAM(OBJ_X_HI + y) = a;
    a = mff_pull();
    RAM(OBJ_X + y) = a;
    a = RAM(OBJ_Y);
    c = 0;
    m = 0x04; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(OBJ_Y + y) = a;
    a = RAM(OBJ_Y_HI);
    RAM(OBJ_Y_HI + y) = a;
    a = 0x1B;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x026A, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x826B, 8);
    a = 0x5F;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x026F, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8270, 29);
    a = 0x6D;
    RAM(OBJ_ID + y) = a;
    RAM(OBJ_SAVED_ID + y) = a;
    a = 0x0A;
    RAM(ATTACK_ID) = a;
    a = 0x00;
    RAM(OBJ_ATTACK + y) = a;
    RAM(OBJ_THROW_FLAG + y) = a;
L_8284:;
    mff_cycles(0x8284, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x09; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8289, 1); goto L_82AA; }
    mff_cycles(0x828B, 8);
    a = RAM(OBJ_ANIM_TIMER);
    m = 0x02; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8290, 1); goto L_82AA; }
    mff_cycles(0x8292, 10);
    t = (uint8_t)(RAM(WEAPON_AMMO) - 1); RAM(WEAPON_AMMO) = t;
    a = RAM(WEAPON_AMMO);
    n = (a) >> 7;
    if (!n) { mff_cycles(0x8296, 1); goto L_829C; }
    mff_cycles(0x8298, 5);
    a = 0x00;
    RAM(WEAPON_HELD) = a;
L_829C:;
    mff_cycles(0x829C, 12);
    a = 0x00;
    RAM(GAME_ACTION) = a;
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x82A5, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x02A9, 0xCEAA, 0)) return;
    SYNC_IN;
L_82AA:;
    mff_cycles(0x82AA, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $82AF (bank 0) - lifted by port/tools/lift.py */
void func_82AF_b0(void) {
    REGS_IN;
L_82AF:;
    mff_cycles(0x82AF, 6);
    a = RAM(OBJ_ANIM_FRAME);
    z = (a) == 0;
    if (!z) { mff_cycles(0x82B2, 1); goto L_82B9; }
    mff_cycles(0x82B4, 8);
    a = 0x1D;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x02B8, func_F34C, 0xE000)) return;
    SYNC_IN;
L_82B9:;
    mff_cycles(0x82B9, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x06; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x82BE, 1); goto L_82CC; }
    mff_cycles(0x82C0, 6);
    a = RAM(OBJ_ANIM_TIMER);
    z = (a) == 0;
    if (!z) { mff_cycles(0x82C3, 1); goto L_82CC; }
    mff_cycles(0x82C5, 11);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ATTACK_ID) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x02CB, func_F43B, 0xE000)) return;
    SYNC_IN;
L_82CC:;
    mff_cycles(0x82CC, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x09; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x82D1, 1); goto L_82DB; }
    mff_cycles(0x82D3, 6);
    a = RAM(OBJ_ANIM_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x82D6, 1); goto L_82DB; }
    mff_cycles(0x82D8, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x02DA, func_F436, 0xE000)) return;
    SYNC_IN;
L_82DB:;
    mff_cycles(0x82DB, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x0C; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x82E0, 1); goto L_82FF; }
    mff_cycles(0x82E2, 12);
    a = 0x00;
    RAM(GAME_ACTION) = a;
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE288 + y));
    mff_cycles(0x82EB, 8);
    RAM(PLAYER_STAND_ANIM) = a;
    a = RAM(WEAPON_HELD);
    z = (a) == 0;
    if (!z) { mff_cycles(0x82EF, 1); goto L_82F5; }
    mff_cycles(0x82F1, 4);
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x82F3, 1); goto L_82F7; }
L_82F5:;
    mff_cycles(0x82F5, 2);
    a = 0x44;
    n = (a) >> 7;
    z = (a) == 0;
L_82F7:;
    mff_cycles(0x82F7, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x02F9, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x82FA, 8);
    a = 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x02FE, func_F440, 0xE000)) return;
    SYNC_IN;
L_82FF:;
    mff_cycles(0x82FF, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8300 (bank 0) - lifted by port/tools/lift.py */
void func_8300_b0(void) {
    REGS_IN;
L_8300:;
    mff_cycles(0x8300, 16);
    a = 0x00;
    RAM(OBJ_ANIM_TIMER) = a;
    RAM(OBJ_ANIM_FRAME) = a;
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    if (n) { mff_cycles(0x830B, 1); goto L_8314; }
    mff_cycles(0x830D, 5);
    a = RAM(GAME_ACTION);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x830F, 1); goto L_8314; }
    mff_cycles(0x8311, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0313, func_F26E, 0xE000)) return;
    SYNC_IN;
L_8314:;
    mff_cycles(0x8314, 10);
    x = 0x00;
    y = 0x02;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x031A, func_F119, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x831B, 2);
    if (c) { mff_cycles(0x831B, 1); goto L_833B; }
    mff_cycles(0x831D, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) { mff_cycles(0x8320, 1); goto L_8325; }
    mff_cycles(0x8322, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x83A1, 2); call_by_address_tail(0x83A1, 0); return;
L_8325:;
    mff_cycles(0x8325, 13);
    a = RAM(OBJ_Y);
    c = 0;
    m = 0x02; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    m = RAM(PLAYER_GROUND_Y); c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x832D, 1); goto L_83A1; }
    mff_cycles(0x832F, 2);
    if (z) { mff_cycles(0x832F, 1); goto L_8338; }
    mff_cycles(0x8331, 8);
    m = 0x03; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    z = (a) == 0;
    RAM(OBJ_Y) = a;
    if (!z) { mff_cycles(0x8336, 1); goto L_833B; }
L_8338:;
    mff_cycles(0x8338, 4);
    RAM(OBJ_Y) = a;
L_833B:;
    mff_cycles(0x833B, 8);
    a = RAM(OBJ_ANIM);
    m = 0x48; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8340, 1); goto L_8374; }
    mff_cycles(0x8342, 8);
    a = 0x1C;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0346, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8347, 16);
    a = 0x02;
    RAM(GAME_ACTION) = a;
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ACTION_TIMER) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0351, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8352, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0354, 0xCEAA, 0)) return;
    SYNC_IN;
    mff_cycles(0x8355, 50);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_SAVED_ID + y);
    RAM(OBJ_ID + y) = a;
    a = 0xFF;
    RAM(OBJ_HELD + y) = a;
    a = RAM(OBJ_SAVED_STATE + y);
    RAM(OBJ_STATE + y) = a;
    a = 0x00;
    RAM(OBJ_THROW_FLAG + y) = a;
    a = RAM(OBJ_Y);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + y) = a;
    SYNC_OUT;
    mff_rts(); return;
L_8374:;
    mff_cycles(0x8374, 8);
    a = 0x0A;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0378, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8379, 8);
    a = 0x49;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x037D, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x837E, 8);
    a = 0x20;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0382, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8383, 41);
    a = 0x0F;
    RAM(GAME_ACTION) = a;
    a = 0x48;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x02;
    RAM(OBJ_VY) = a;
    a = 0x00;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x10;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER) = a;
    SYNC_OUT;
    mff_rts(); return;
L_83A1:;
    mff_cycles(0x83A1, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    if (n) { mff_cycles(0x83A4, 1); goto L_83BE; }
    mff_cycles(0x83A6, 7);
    a = RAM(JOY_PRESSED);
    m = 0x40; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x83AA, 1); goto L_83BE; }
    mff_cycles(0x83AC, 8);
    a = 0x0A;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x03B0, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x83B1, 8);
    a = 0x48;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x03B5, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x83B6, 10);
    a = 0xFF;
    RAM(PLAYER_NO_HIT) = a;
    a = 0x09;
    RAM(ATTACK_ID) = a;
L_83BE:;
    mff_cycles(0x83BE, 11);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_ANIM);
    m = 0x48; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x83C5, 1); goto L_83D3; }
    mff_cycles(0x83C7, 16);
    a = RAM(OBJ_Y);
    c = 0;
    m = 0x04; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + y) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x83DC, 2); call_by_address_tail(0x83DC, 0); return;
L_83D3:;
    mff_cycles(0x83D3, 25);
    a = RAM(OBJ_Y);
    c = 1;
    m = 0x0C; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    RAM(OBJ_Y + y) = a;
    a = 0x0E;
    RAM(OBJ_ID + y) = a;
    a = RAM(JUMP_MOVING);
    z = (a) == 0;
    if (!z) { mff_cycles(0x83E3, 1); goto L_83F1; }
    mff_cycles(0x83E5, 9);
    x = 0x00;
    a = RAM(JOY_HELD);
    a &= 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x83EB, 2); goto L_8446; }
    mff_cycles(0x83ED, 8);
    t = (uint8_t)(RAM(JUMP_MOVING) + 1); RAM(JUMP_MOVING) = t;
    RAM(JUMP_DIR) = a;
L_83F1:;
    mff_cycles(0x83F1, 7);
    a = RAM(JUMP_DIR);
    a &= 0x01;
    z = (a) == 0;
    if (z) { mff_cycles(0x83F5, 2); goto L_8447; }
    mff_cycles(0x83F7, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    m = 0xF1; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x83FF, 1); goto L_8446; }
    mff_cycles(0x8401, 2);
    if (c) { mff_cycles(0x8401, 1); goto L_8446; }
    mff_cycles(0x8403, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0405, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8406, 10);
    y = RAM(GRAB_SLOT);
    a = RAM(PLAYER_DIR);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x840C, 1); goto L_841D; }
    mff_cycles(0x840E, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    m = 0xF1; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8416, 1); goto L_8446; }
    mff_cycles(0x8418, 2);
    if (c) { mff_cycles(0x8418, 1); goto L_8446; }
    mff_cycles(0x841A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8429, 2); call_by_address_tail(0x8429, 0); return;
L_841D:;
    mff_cycles(0x841D, 13);
    a = RAM(OBJ_X + y);
    c = 1;
    m = RAM(SCROLL_X); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    m = 0xF9; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8425, 1); goto L_8446; }
    mff_cycles(0x8427, 2);
    if (c) { mff_cycles(0x8427, 1); goto L_8446; }
    mff_cycles(0x8429, 30);
    a = RAM(OBJ_X_FRAC + y);
    c = 0;
    m = RAM(OBJ_VX_FRAC); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(OBJ_X_FRAC + y) = a;
    a = RAM(OBJ_X + y);
    m = RAM(OBJ_VX); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X + y) = a;
    if (!c) { mff_cycles(0x843C, 1); goto L_8446; }
    mff_cycles(0x843E, 11);
    a = RAM(OBJ_X_HI + y);
    m = 0x00; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X_HI + y) = a;
L_8446:;
    mff_cycles(0x8446, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8447:;
    mff_cycles(0x8447, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    m = 0x19; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x844F, 1); goto L_8496; }
    mff_cycles(0x8451, 2);
    if (!c) { mff_cycles(0x8451, 1); goto L_8496; }
    mff_cycles(0x8453, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0455, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8456, 10);
    y = RAM(GRAB_SLOT);
    a = RAM(PLAYER_DIR);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x845C, 1); goto L_846D; }
    mff_cycles(0x845E, 13);
    a = RAM(OBJ_X + y);
    c = 1;
    m = RAM(SCROLL_X); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    m = 0x11; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8466, 1); goto L_8496; }
    mff_cycles(0x8468, 2);
    if (!c) { mff_cycles(0x8468, 1); goto L_8496; }
    mff_cycles(0x846A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8479, 2); call_by_address_tail(0x8479, 0); return;
L_846D:;
    mff_cycles(0x846D, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    m = 0x19; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8475, 1); goto L_8496; }
    mff_cycles(0x8477, 2);
    if (!c) { mff_cycles(0x8477, 1); goto L_8496; }
    mff_cycles(0x8479, 30);
    a = RAM(OBJ_X_FRAC + y);
    c = 1;
    m = RAM(OBJ_VX_FRAC); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    RAM(OBJ_X_FRAC + y) = a;
    a = RAM(OBJ_X + y);
    m = RAM(OBJ_VX); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X + y) = a;
    if (c) { mff_cycles(0x848C, 1); goto L_8496; }
    mff_cycles(0x848E, 11);
    a = RAM(OBJ_X_HI + y);
    m = 0x00; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X_HI + y) = a;
L_8496:;
    mff_cycles(0x8496, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $83A1 (bank 0) - lifted by port/tools/lift.py */
void func_83A1_b0(void) {
    REGS_IN;
L_83A1:;
    mff_cycles(0x83A1, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    if (n) { mff_cycles(0x83A4, 1); goto L_83BE; }
    mff_cycles(0x83A6, 7);
    a = RAM(JOY_PRESSED);
    m = 0x40; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x83AA, 1); goto L_83BE; }
    mff_cycles(0x83AC, 8);
    a = 0x0A;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x03B0, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x83B1, 8);
    a = 0x48;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x03B5, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x83B6, 10);
    a = 0xFF;
    RAM(PLAYER_NO_HIT) = a;
    a = 0x09;
    RAM(ATTACK_ID) = a;
L_83BE:;
    mff_cycles(0x83BE, 11);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_ANIM);
    m = 0x48; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x83C5, 1); goto L_83D3; }
    mff_cycles(0x83C7, 16);
    a = RAM(OBJ_Y);
    c = 0;
    m = 0x04; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + y) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x83DC, 2); call_by_address_tail(0x83DC, 0); return;
L_83D3:;
    mff_cycles(0x83D3, 25);
    a = RAM(OBJ_Y);
    c = 1;
    m = 0x0C; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    RAM(OBJ_Y + y) = a;
    a = 0x0E;
    RAM(OBJ_ID + y) = a;
    a = RAM(JUMP_MOVING);
    z = (a) == 0;
    if (!z) { mff_cycles(0x83E3, 1); goto L_83F1; }
    mff_cycles(0x83E5, 9);
    x = 0x00;
    a = RAM(JOY_HELD);
    a &= 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x83EB, 2); goto L_8446; }
    mff_cycles(0x83ED, 8);
    t = (uint8_t)(RAM(JUMP_MOVING) + 1); RAM(JUMP_MOVING) = t;
    RAM(JUMP_DIR) = a;
L_83F1:;
    mff_cycles(0x83F1, 7);
    a = RAM(JUMP_DIR);
    a &= 0x01;
    z = (a) == 0;
    if (z) { mff_cycles(0x83F5, 2); goto L_8447; }
    mff_cycles(0x83F7, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    m = 0xF1; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x83FF, 1); goto L_8446; }
    mff_cycles(0x8401, 2);
    if (c) { mff_cycles(0x8401, 1); goto L_8446; }
    mff_cycles(0x8403, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0405, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8406, 10);
    y = RAM(GRAB_SLOT);
    a = RAM(PLAYER_DIR);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x840C, 1); goto L_841D; }
    mff_cycles(0x840E, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    m = 0xF1; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8416, 1); goto L_8446; }
    mff_cycles(0x8418, 2);
    if (c) { mff_cycles(0x8418, 1); goto L_8446; }
    mff_cycles(0x841A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8429, 2); call_by_address_tail(0x8429, 0); return;
L_841D:;
    mff_cycles(0x841D, 13);
    a = RAM(OBJ_X + y);
    c = 1;
    m = RAM(SCROLL_X); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    m = 0xF9; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8425, 1); goto L_8446; }
    mff_cycles(0x8427, 2);
    if (c) { mff_cycles(0x8427, 1); goto L_8446; }
    mff_cycles(0x8429, 30);
    a = RAM(OBJ_X_FRAC + y);
    c = 0;
    m = RAM(OBJ_VX_FRAC); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(OBJ_X_FRAC + y) = a;
    a = RAM(OBJ_X + y);
    m = RAM(OBJ_VX); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X + y) = a;
    if (!c) { mff_cycles(0x843C, 1); goto L_8446; }
    mff_cycles(0x843E, 11);
    a = RAM(OBJ_X_HI + y);
    m = 0x00; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X_HI + y) = a;
L_8446:;
    mff_cycles(0x8446, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8447:;
    mff_cycles(0x8447, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    m = 0x19; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x844F, 1); goto L_8496; }
    mff_cycles(0x8451, 2);
    if (!c) { mff_cycles(0x8451, 1); goto L_8496; }
    mff_cycles(0x8453, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0455, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8456, 10);
    y = RAM(GRAB_SLOT);
    a = RAM(PLAYER_DIR);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x845C, 1); goto L_846D; }
    mff_cycles(0x845E, 13);
    a = RAM(OBJ_X + y);
    c = 1;
    m = RAM(SCROLL_X); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    m = 0x11; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8466, 1); goto L_8496; }
    mff_cycles(0x8468, 2);
    if (!c) { mff_cycles(0x8468, 1); goto L_8496; }
    mff_cycles(0x846A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8479, 2); call_by_address_tail(0x8479, 0); return;
L_846D:;
    mff_cycles(0x846D, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    m = 0x19; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8475, 1); goto L_8496; }
    mff_cycles(0x8477, 2);
    if (!c) { mff_cycles(0x8477, 1); goto L_8496; }
    mff_cycles(0x8479, 30);
    a = RAM(OBJ_X_FRAC + y);
    c = 1;
    m = RAM(OBJ_VX_FRAC); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    RAM(OBJ_X_FRAC + y) = a;
    a = RAM(OBJ_X + y);
    m = RAM(OBJ_VX); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X + y) = a;
    if (c) { mff_cycles(0x848C, 1); goto L_8496; }
    mff_cycles(0x848E, 11);
    a = RAM(OBJ_X_HI + y);
    m = 0x00; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X_HI + y) = a;
L_8496:;
    mff_cycles(0x8496, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $83DC (bank 0) - lifted by port/tools/lift.py */
void func_83DC_b0(void) {
    REGS_IN;
L_83DC:;
    mff_cycles(0x83DC, 12);
    a = 0x0E;
    RAM(OBJ_ID + y) = a;
    a = RAM(JUMP_MOVING);
    z = (a) == 0;
    if (!z) { mff_cycles(0x83E3, 1); goto L_83F1; }
    mff_cycles(0x83E5, 9);
    x = 0x00;
    a = RAM(JOY_HELD);
    a &= 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x83EB, 2); goto L_8446; }
    mff_cycles(0x83ED, 8);
    t = (uint8_t)(RAM(JUMP_MOVING) + 1); RAM(JUMP_MOVING) = t;
    RAM(JUMP_DIR) = a;
L_83F1:;
    mff_cycles(0x83F1, 7);
    a = RAM(JUMP_DIR);
    a &= 0x01;
    z = (a) == 0;
    if (z) { mff_cycles(0x83F5, 2); goto L_8447; }
    mff_cycles(0x83F7, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    m = 0xF1; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x83FF, 1); goto L_8446; }
    mff_cycles(0x8401, 2);
    if (c) { mff_cycles(0x8401, 1); goto L_8446; }
    mff_cycles(0x8403, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0405, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8406, 10);
    y = RAM(GRAB_SLOT);
    a = RAM(PLAYER_DIR);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x840C, 1); goto L_841D; }
    mff_cycles(0x840E, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    m = 0xF1; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8416, 1); goto L_8446; }
    mff_cycles(0x8418, 2);
    if (c) { mff_cycles(0x8418, 1); goto L_8446; }
    mff_cycles(0x841A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8429, 2); call_by_address_tail(0x8429, 0); return;
L_841D:;
    mff_cycles(0x841D, 13);
    a = RAM(OBJ_X + y);
    c = 1;
    m = RAM(SCROLL_X); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    m = 0xF9; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8425, 1); goto L_8446; }
    mff_cycles(0x8427, 2);
    if (c) { mff_cycles(0x8427, 1); goto L_8446; }
    mff_cycles(0x8429, 30);
    a = RAM(OBJ_X_FRAC + y);
    c = 0;
    m = RAM(OBJ_VX_FRAC); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(OBJ_X_FRAC + y) = a;
    a = RAM(OBJ_X + y);
    m = RAM(OBJ_VX); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X + y) = a;
    if (!c) { mff_cycles(0x843C, 1); goto L_8446; }
    mff_cycles(0x843E, 11);
    a = RAM(OBJ_X_HI + y);
    m = 0x00; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X_HI + y) = a;
L_8446:;
    mff_cycles(0x8446, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8447:;
    mff_cycles(0x8447, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    m = 0x19; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x844F, 1); goto L_8496; }
    mff_cycles(0x8451, 2);
    if (!c) { mff_cycles(0x8451, 1); goto L_8496; }
    mff_cycles(0x8453, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0455, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8456, 10);
    y = RAM(GRAB_SLOT);
    a = RAM(PLAYER_DIR);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x845C, 1); goto L_846D; }
    mff_cycles(0x845E, 13);
    a = RAM(OBJ_X + y);
    c = 1;
    m = RAM(SCROLL_X); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    m = 0x11; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8466, 1); goto L_8496; }
    mff_cycles(0x8468, 2);
    if (!c) { mff_cycles(0x8468, 1); goto L_8496; }
    mff_cycles(0x846A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8479, 2); call_by_address_tail(0x8479, 0); return;
L_846D:;
    mff_cycles(0x846D, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    m = 0x19; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8475, 1); goto L_8496; }
    mff_cycles(0x8477, 2);
    if (!c) { mff_cycles(0x8477, 1); goto L_8496; }
    mff_cycles(0x8479, 30);
    a = RAM(OBJ_X_FRAC + y);
    c = 1;
    m = RAM(OBJ_VX_FRAC); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    RAM(OBJ_X_FRAC + y) = a;
    a = RAM(OBJ_X + y);
    m = RAM(OBJ_VX); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X + y) = a;
    if (c) { mff_cycles(0x848C, 1); goto L_8496; }
    mff_cycles(0x848E, 11);
    a = RAM(OBJ_X_HI + y);
    m = 0x00; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X_HI + y) = a;
L_8496:;
    mff_cycles(0x8496, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8429 (bank 0) - lifted by port/tools/lift.py */
void func_8429_b0(void) {
    REGS_IN;
L_8429:;
    mff_cycles(0x8429, 30);
    a = RAM(OBJ_X_FRAC + y);
    c = 0;
    m = RAM(OBJ_VX_FRAC); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(OBJ_X_FRAC + y) = a;
    a = RAM(OBJ_X + y);
    m = RAM(OBJ_VX); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X + y) = a;
    if (!c) { mff_cycles(0x843C, 1); goto L_8446; }
    mff_cycles(0x843E, 11);
    a = RAM(OBJ_X_HI + y);
    m = 0x00; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X_HI + y) = a;
L_8446:;
    mff_cycles(0x8446, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8479 (bank 0) - lifted by port/tools/lift.py */
void func_8479_b0(void) {
    REGS_IN;
L_8479:;
    mff_cycles(0x8479, 30);
    a = RAM(OBJ_X_FRAC + y);
    c = 1;
    m = RAM(OBJ_VX_FRAC); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    RAM(OBJ_X_FRAC + y) = a;
    a = RAM(OBJ_X + y);
    m = RAM(OBJ_VX); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X + y) = a;
    if (c) { mff_cycles(0x848C, 1); goto L_8496; }
    mff_cycles(0x848E, 11);
    a = RAM(OBJ_X_HI + y);
    m = 0x00; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X_HI + y) = a;
L_8496:;
    mff_cycles(0x8496, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8540 (bank 0) - lifted by port/tools/lift.py */
void func_8540_b0(void) {
    REGS_IN;
L_8540:;
    mff_cycles(0x8540, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8541 (bank 0) - lifted by port/tools/lift.py */
void func_8541_b0(void) {
    REGS_IN;
L_8541:;
    mff_cycles(0x8541, 8);
    a = RAM(OBJ_ANIM);
    m = 0x3E; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8546, 1); goto L_8559; }
    mff_cycles(0x8548, 11);
    a = 0x07;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ATTACK_ID) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x054E, func_F43B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x854F, 12);
    a = 0x89;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x06;
    RAM(OBJ_VX) = a;
L_8559:;
    mff_cycles(0x8559, 8);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    z = (a) == 0;
    if (!z) { mff_cycles(0x855E, 1); goto L_8579; }
    mff_cycles(0x8560, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x04; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8565, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0540), 0); return; }
    mff_cycles(0x8567, 8);
    a = RAM(OBJ_ANIM_TIMER);
    m = 0x01; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x856C, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0540), 0); return; }
    mff_cycles(0x856E, 16);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
    a = 0x00;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER2) = a;
L_8579:;
    mff_cycles(0x8579, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x06; c = a >= m;
    if (c) { mff_cycles(0x857E, 1); goto L_85A2; }
    mff_cycles(0x8580, 8);
    a = RAM(OBJ_ANIM);
    m = 0x3E; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8585, 1); goto L_85A2; }
    mff_cycles(0x8587, 20);
    a = 0x38;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x04;
    RAM(OBJ_VY) = a;
    a = RAM(OBJ_VY);
    m = 0x03; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x8596, 1); goto L_85A2; }
    mff_cycles(0x8598, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x059A, func_F43B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x859B, 9);
    a = RAM(PLAYER_GROUND_Y);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ATTACK_GROUND_Y) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x85AA, 2); call_by_address_tail(0x85AA, 0); return;
L_85A2:;
    mff_cycles(0x85A2, 17);
    a = 0x00;
    RAM(OBJ_VX_FRAC) = a;
    RAM(OBJ_VX) = a;
    a = RAM(PLAYER_DIR);
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x85AE, 1); goto L_85B9; }
    mff_cycles(0x85B0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x05B2, func_F090, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x85B3, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x05B5, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x85B6, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x85BF, 2); call_by_address_tail(0x85BF, 0); return;
L_85B9:;
    mff_cycles(0x85B9, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x05BB, func_F099, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x85BC, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x05BE, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x85BF, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x05C3, func_F119, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x85C4, 2);
    if (!c) { mff_cycles(0x85C4, 1); goto L_85C9; }
    mff_cycles(0x85C6, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x85FE, 2); call_by_address_tail(0x85FE, 0); return;
L_85C9:;
    mff_cycles(0x85C9, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0x85CC, 2); goto L_862C; }
    mff_cycles(0x85CE, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x05D0, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x85D1, 8);
    a = RAM(OBJ_STATE);
    a &= 0x02;
    z = (a) == 0;
    if (!z) { mff_cycles(0x85D6, 1); goto L_85EA; }
    mff_cycles(0x85D8, 8);
    a = 0x3E;
    m = RAM(OBJ_ANIM); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x85DD, 1); goto L_85EA; }
    mff_cycles(0x85DF, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x05E1, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x85E2, 8);
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x05E6, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x85E7, 6);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
L_85EA:;
    mff_cycles(0x85EA, 13);
    a = RAM(PLAYER_GROUND_Y);
    c = 0;
    m = 0x02; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    m = RAM(OBJ_Y); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x85F2, 2); goto L_863D; }
    mff_cycles(0x85F4, 2);
    if (z) { mff_cycles(0x85F4, 1); goto L_85F9; }
    mff_cycles(0x85F6, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x85F9, 2); call_by_address_tail(0x85F9, 0); return;
L_85F9:;
    mff_cycles(0x85F9, 69);
    a = RAM(PLAYER_GROUND_Y);
    RAM(OBJ_Y) = a;
    a = 0x02;
    RAM(GAME_ACTION) = a;
    a = 0x00;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(PLAYER_NO_HIT) = a;
    RAM(COMBO_TIMER) = a;
    a = 0xC0;
    RAM(OBJ_VY_FRAC) = a;
    a = 0xFF;
    RAM(OBJ_VY) = a;
    a = 0x33;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VX) = a;
    a = 0x00;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER2) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE) = a;
    SYNC_OUT;
    mff_rts(); return;
L_862C:;
    mff_cycles(0x862C, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x09; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8631, 1); goto L_863D; }
    mff_cycles(0x8633, 12);
    a = 0x09;
    RAM(OBJ_ANIM_FRAME) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ANIM_TIMER) = a;
L_863D:;
    mff_cycles(0x863D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $85AA (bank 0) - lifted by port/tools/lift.py */
void func_85AA_b0(void) {
    REGS_IN;
L_85AA:;
    mff_cycles(0x85AA, 7);
    a = RAM(PLAYER_DIR);
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x85AE, 1); goto L_85B9; }
    mff_cycles(0x85B0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x05B2, func_F090, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x85B3, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x05B5, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x85B6, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x85BF, 2); call_by_address_tail(0x85BF, 0); return;
L_85B9:;
    mff_cycles(0x85B9, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x05BB, func_F099, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x85BC, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x05BE, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x85BF, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x05C3, func_F119, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x85C4, 2);
    if (!c) { mff_cycles(0x85C4, 1); goto L_85C9; }
    mff_cycles(0x85C6, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x85FE, 2); call_by_address_tail(0x85FE, 0); return;
L_85C9:;
    mff_cycles(0x85C9, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0x85CC, 2); goto L_862C; }
    mff_cycles(0x85CE, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x05D0, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x85D1, 8);
    a = RAM(OBJ_STATE);
    a &= 0x02;
    z = (a) == 0;
    if (!z) { mff_cycles(0x85D6, 1); goto L_85EA; }
    mff_cycles(0x85D8, 8);
    a = 0x3E;
    m = RAM(OBJ_ANIM); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x85DD, 1); goto L_85EA; }
    mff_cycles(0x85DF, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x05E1, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x85E2, 8);
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x05E6, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x85E7, 6);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
L_85EA:;
    mff_cycles(0x85EA, 13);
    a = RAM(PLAYER_GROUND_Y);
    c = 0;
    m = 0x02; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    m = RAM(OBJ_Y); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x85F2, 2); goto L_863D; }
    mff_cycles(0x85F4, 2);
    if (z) { mff_cycles(0x85F4, 1); goto L_85F9; }
    mff_cycles(0x85F6, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x85F9, 2); call_by_address_tail(0x85F9, 0); return;
L_85F9:;
    mff_cycles(0x85F9, 69);
    a = RAM(PLAYER_GROUND_Y);
    RAM(OBJ_Y) = a;
    a = 0x02;
    RAM(GAME_ACTION) = a;
    a = 0x00;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(PLAYER_NO_HIT) = a;
    RAM(COMBO_TIMER) = a;
    a = 0xC0;
    RAM(OBJ_VY_FRAC) = a;
    a = 0xFF;
    RAM(OBJ_VY) = a;
    a = 0x33;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VX) = a;
    a = 0x00;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER2) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE) = a;
    SYNC_OUT;
    mff_rts(); return;
L_862C:;
    mff_cycles(0x862C, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x09; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8631, 1); goto L_863D; }
    mff_cycles(0x8633, 12);
    a = 0x09;
    RAM(OBJ_ANIM_FRAME) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ANIM_TIMER) = a;
L_863D:;
    mff_cycles(0x863D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $85BF (bank 0) - lifted by port/tools/lift.py */
void func_85BF_b0(void) {
    REGS_IN;
L_85BF:;
    mff_cycles(0x85BF, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x05C3, func_F119, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x85C4, 2);
    if (!c) { mff_cycles(0x85C4, 1); goto L_85C9; }
    mff_cycles(0x85C6, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x85FE, 2); call_by_address_tail(0x85FE, 0); return;
L_85C9:;
    mff_cycles(0x85C9, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0x85CC, 2); goto L_862C; }
    mff_cycles(0x85CE, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x05D0, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x85D1, 8);
    a = RAM(OBJ_STATE);
    a &= 0x02;
    z = (a) == 0;
    if (!z) { mff_cycles(0x85D6, 1); goto L_85EA; }
    mff_cycles(0x85D8, 8);
    a = 0x3E;
    m = RAM(OBJ_ANIM); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x85DD, 1); goto L_85EA; }
    mff_cycles(0x85DF, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x05E1, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x85E2, 8);
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x05E6, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x85E7, 6);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
L_85EA:;
    mff_cycles(0x85EA, 13);
    a = RAM(PLAYER_GROUND_Y);
    c = 0;
    m = 0x02; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    m = RAM(OBJ_Y); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x85F2, 2); goto L_863D; }
    mff_cycles(0x85F4, 2);
    if (z) { mff_cycles(0x85F4, 1); goto L_85F9; }
    mff_cycles(0x85F6, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x85F9, 2); call_by_address_tail(0x85F9, 0); return;
L_85F9:;
    mff_cycles(0x85F9, 69);
    a = RAM(PLAYER_GROUND_Y);
    RAM(OBJ_Y) = a;
    a = 0x02;
    RAM(GAME_ACTION) = a;
    a = 0x00;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(PLAYER_NO_HIT) = a;
    RAM(COMBO_TIMER) = a;
    a = 0xC0;
    RAM(OBJ_VY_FRAC) = a;
    a = 0xFF;
    RAM(OBJ_VY) = a;
    a = 0x33;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VX) = a;
    a = 0x00;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER2) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE) = a;
    SYNC_OUT;
    mff_rts(); return;
L_862C:;
    mff_cycles(0x862C, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x09; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8631, 1); goto L_863D; }
    mff_cycles(0x8633, 12);
    a = 0x09;
    RAM(OBJ_ANIM_FRAME) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ANIM_TIMER) = a;
L_863D:;
    mff_cycles(0x863D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $85F9 (bank 0) - lifted by port/tools/lift.py */
void func_85F9_b0(void) {
    REGS_IN;
L_85F9:;
    mff_cycles(0x85F9, 69);
    a = RAM(PLAYER_GROUND_Y);
    RAM(OBJ_Y) = a;
    a = 0x02;
    RAM(GAME_ACTION) = a;
    a = 0x00;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(PLAYER_NO_HIT) = a;
    RAM(COMBO_TIMER) = a;
    a = 0xC0;
    RAM(OBJ_VY_FRAC) = a;
    a = 0xFF;
    RAM(OBJ_VY) = a;
    a = 0x33;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VX) = a;
    a = 0x00;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER2) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $85FE (bank 0) - lifted by port/tools/lift.py */
void func_85FE_b0(void) {
    REGS_IN;
L_85FE:;
    mff_cycles(0x85FE, 62);
    a = 0x02;
    RAM(GAME_ACTION) = a;
    a = 0x00;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(PLAYER_NO_HIT) = a;
    RAM(COMBO_TIMER) = a;
    a = 0xC0;
    RAM(OBJ_VY_FRAC) = a;
    a = 0xFF;
    RAM(OBJ_VY) = a;
    a = 0x33;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VX) = a;
    a = 0x00;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER2) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $863E (bank 0) - lifted by port/tools/lift.py */
void func_863E_b0(void) {
    REGS_IN;
L_863E:;
    mff_cycles(0x863E, 11);
    a = 0x21;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8643 (bank 0) - lifted by port/tools/lift.py */
void func_8643_b0(void) {
    REGS_IN;
L_8643:;
    mff_cycles(0x8643, 8);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    z = (a) == 0;
    if (!z) { mff_cycles(0x8648, 1); goto L_86B8; }
    mff_cycles(0x864A, 15);
    a = 0x80;
    RAM(PLAYER_NO_HIT) = a;
    y = RAM(GRAB_SLOT);
    a = RAM(JOY_HELD);
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x8654, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x063E), 0); return; }
    mff_cycles(0x8656, 5);
    m = RAM(PLAYER_DIR); c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8658, 1); goto L_866D; }
    mff_cycles(0x865A, 11);
    a = RAM(OBJ_DIR + y);
    a &= 0x03;
    m = RAM(PLAYER_DIR); c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8661, 1); goto L_8684; }
    mff_cycles(0x8663, 13);
    a = RAM(OBJ_DIR + y);
    a ^= 0x03;
    z = (a) == 0;
    RAM(OBJ_DIR + y) = a;
    if (!z) { mff_cycles(0x866B, 1); goto L_8684; }
L_866D:;
    mff_cycles(0x866D, 14);
    a = 0xFF;
    RAM(OBJ_THROW_FLAG + y) = a;
    a = RAM(PLAYER_DIR);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x8676, 1); goto L_867F; }
    mff_cycles(0x8678, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x067A, func_F048, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x867B, 4);
    a = 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x867D, 1); goto L_8684; }
L_867F:;
    mff_cycles(0x867F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0681, func_F03F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8682, 2);
    a = 0x01;
L_8684:;
    mff_cycles(0x8684, 11);
    RAM(PLAYER_DIR) = a;
    a = 0x36;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x068A, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x868B, 8);
    a = 0x0A;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x068F, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8690, 25);
    y = RAM(GRAB_SLOT);
    a = 0x08;
    RAM(OBJ_ID + y) = a;
    a = 0x80;
    RAM(OBJ_STATE + y) = a;
    a = RAM(OBJ_DIR + y);
    a &= 0x01;
    z = (a) == 0;
    if (z) { mff_cycles(0x86A1, 1); goto L_86AD; }
    mff_cycles(0x86A3, 13);
    a = RAM(OBJ_FLAGS + y);
    a &= 0xBF;
    z = (a) == 0;
    RAM(OBJ_FLAGS + y) = a;
    if (!z) { mff_cycles(0x86AB, 1); goto L_86B5; }
L_86AD:;
    mff_cycles(0x86AD, 11);
    a = RAM(OBJ_FLAGS + y);
    a |= 0x40;
    RAM(OBJ_FLAGS + y) = a;
L_86B5:;
    mff_cycles(0x86B5, 6);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
L_86B8:;
    mff_cycles(0x86B8, 8);
    a = RAM(OBJ_ANIM);
    m = 0x36; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86BD, 1); goto L_86D0; }
    mff_cycles(0x86BF, 8);
    a = RAM(OBJ_ANIM_TIMER);
    m = 0x04; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86C4, 1); goto L_86D0; }
    mff_cycles(0x86C6, 8);
    a = 0x52;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x06CA, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x86CB, 8);
    a = 0x0B;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x06CF, func_F440, 0xE000)) return;
    SYNC_IN;
L_86D0:;
    mff_cycles(0x86D0, 8);
    a = RAM(OBJ_ANIM);
    m = 0x52; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86D5, 1); goto L_86EF; }
    mff_cycles(0x86D7, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x12; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86DC, 1); goto L_86EF; }
    mff_cycles(0x86DE, 8);
    a = RAM(OBJ_ANIM_TIMER);
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86E3, 1); goto L_86EF; }
    mff_cycles(0x86E5, 8);
    a = 0x4C;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x06E9, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x86EA, 8);
    a = 0x0A;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x06EE, func_F440, 0xE000)) return;
    SYNC_IN;
L_86EF:;
    mff_cycles(0x86EF, 8);
    a = RAM(OBJ_ANIM);
    m = 0x4C; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x86F4, 1); goto L_86FE; }
    mff_cycles(0x86F6, 10);
    a = 0x02;
    RAM(ACTION_TIMER) = a;
    a = 0x0B;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
L_86FE:;
    mff_cycles(0x86FE, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $87AA (bank 0) - lifted by port/tools/lift.py */
void func_87AA_b0(void) {
    REGS_IN;
L_87AA:;
    mff_cycles(0x87AA, 7);
    a = RAM(GAME_ACTION);
    m = 0x16; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x87AE, 1); goto L_87B7; }
    mff_cycles(0x87B0, 5);
    a = RAM(JOY_HELD);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x87B2, 1); goto L_87B7; }
    mff_cycles(0x87B4, 6);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
    n = (t) >> 7;
    z = (t) == 0;
L_87B7:;
    mff_cycles(0x87B7, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $87E2 (bank 0) - lifted by port/tools/lift.py */
void func_87E2_b0(void) {
    REGS_IN;
L_87E2:;
    mff_cycles(0x87E2, 7);
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0x87F1 + y));
    mff_cycles(0x87E7, 7);
    RAM(0x00) = a;
    a = nes_read((uint16_t)(0x87F4 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x87EC, 8);
    RAM(0x01) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $87F7 (bank 0) - lifted by port/tools/lift.py */
void func_87F7_b0(void) {
    REGS_IN;
L_87F7:;
    mff_cycles(0x87F7, 10);
    a = RAM(OBJ_ANIM_FRAME);
    y = a;
    a = nes_read((uint16_t)(0x88A6 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x87FE, 9);
    RAM(ATTACK_ID) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0802, func_F43B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8803, 7);
    a = RAM(DRAW_PARITY);
    a &= 0x07;
    z = (a) == 0;
    if (!z) { mff_cycles(0x8807, 1); goto L_880E; }
    mff_cycles(0x8809, 8);
    a = 0x25;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x080D, func_F34C, 0xE000)) return;
    SYNC_IN;
L_880E:;
    mff_cycles(0x880E, 14);
    a = RAM(PLAYER_GROUND_Y);
    RAM(ATTACK_GROUND_Y) = a;
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0816, func_F119, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8817, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    if (!n) { mff_cycles(0x881A, 1); goto L_884E; }
    mff_cycles(0x881C, 13);
    a = RAM(OBJ_Y);
    c = 0;
    m = 0x02; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    m = RAM(PLAYER_GROUND_Y); c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x8824, 1); goto L_884E; }
    mff_cycles(0x8826, 2);
    if (z) { mff_cycles(0x8826, 1); goto L_882F; }
    mff_cycles(0x8828, 8);
    m = 0x03; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    z = (a) == 0;
    RAM(OBJ_Y) = a;
    if (!z) { mff_cycles(0x882D, 1); goto L_8832; }
L_882F:;
    mff_cycles(0x882F, 4);
    RAM(OBJ_Y) = a;
L_8832:;
    mff_cycles(0x8832, 30);
    a = 0x00;
    RAM(GAME_ACTION) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    a = 0xC0;
    RAM(OBJ_VY_FRAC) = a;
    a = 0xFF;
    RAM(OBJ_VY) = a;
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8849, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x084D, 0xCEAA, 0)) return;
    SYNC_IN;
L_884E:;
    mff_cycles(0x884E, 5);
    a = RAM(SPECIAL_HIT);
    z = (a) == 0;
    if (z) { mff_cycles(0x8850, 1); goto L_885F; }
    mff_cycles(0x8852, 12);
    a = RAM(HEALTH);
    c = 1;
    m = 0x06; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    n = (a) >> 7;
    RAM(HEALTH) = a;
    if (!n) { mff_cycles(0x8859, 1); goto L_885F; }
    mff_cycles(0x885B, 5);
    a = 0x00;
    RAM(HEALTH) = a;
L_885F:;
    mff_cycles(0x885F, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SPECIAL_HIT) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8832 (bank 0) - lifted by port/tools/lift.py */
void func_8832_b0(void) {
    REGS_IN;
L_8832:;
    mff_cycles(0x8832, 30);
    a = 0x00;
    RAM(GAME_ACTION) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    a = 0xC0;
    RAM(OBJ_VY_FRAC) = a;
    a = 0xFF;
    RAM(OBJ_VY) = a;
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8849, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x084D, 0xCEAA, 0)) return;
    SYNC_IN;
    mff_cycles(0x884E, 5);
    a = RAM(SPECIAL_HIT);
    z = (a) == 0;
    if (z) { mff_cycles(0x8850, 1); goto L_885F; }
    mff_cycles(0x8852, 12);
    a = RAM(HEALTH);
    c = 1;
    m = 0x06; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    n = (a) >> 7;
    RAM(HEALTH) = a;
    if (!n) { mff_cycles(0x8859, 1); goto L_885F; }
    mff_cycles(0x885B, 5);
    a = 0x00;
    RAM(HEALTH) = a;
L_885F:;
    mff_cycles(0x885F, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SPECIAL_HIT) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $884E (bank 0) - lifted by port/tools/lift.py */
void func_884E_b0(void) {
    REGS_IN;
L_884E:;
    mff_cycles(0x884E, 5);
    a = RAM(SPECIAL_HIT);
    z = (a) == 0;
    if (z) { mff_cycles(0x8850, 1); goto L_885F; }
    mff_cycles(0x8852, 12);
    a = RAM(HEALTH);
    c = 1;
    m = 0x06; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    n = (a) >> 7;
    RAM(HEALTH) = a;
    if (!n) { mff_cycles(0x8859, 1); goto L_885F; }
    mff_cycles(0x885B, 5);
    a = 0x00;
    RAM(HEALTH) = a;
L_885F:;
    mff_cycles(0x885F, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SPECIAL_HIT) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8864 (bank 0) - lifted by port/tools/lift.py */
void func_8864_b0(void) {
    REGS_IN;
L_8864:;
    mff_cycles(0x8864, 10);
    a = RAM(OBJ_ANIM_FRAME);
    y = a;
    a = nes_read((uint16_t)(0x88A6 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x886B, 9);
    RAM(ATTACK_ID) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x086F, func_F43B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8870, 7);
    a = RAM(DRAW_PARITY);
    a &= 0x07;
    z = (a) == 0;
    if (!z) { mff_cycles(0x8874, 1); goto L_887B; }
    mff_cycles(0x8876, 8);
    a = 0x25;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x087A, func_F34C, 0xE000)) return;
    SYNC_IN;
L_887B:;
    mff_cycles(0x887B, 14);
    a = RAM(PLAYER_GROUND_Y);
    RAM(ATTACK_GROUND_Y) = a;
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0883, func_F119, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8884, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0x8887, 1); goto L_88A2; }
    mff_cycles(0x8889, 13);
    a = RAM(OBJ_Y);
    c = 0;
    m = 0x02; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    m = RAM(PLAYER_GROUND_Y); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x8891, 1); goto L_88A2; }
    mff_cycles(0x8893, 2);
    if (z) { mff_cycles(0x8893, 1); goto L_889C; }
    mff_cycles(0x8895, 8);
    m = 0x03; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y) = a;
    if (!z) { mff_cycles(0x889A, 1); goto L_889F; }
L_889C:;
    mff_cycles(0x889C, 4);
    RAM(OBJ_Y) = a;
L_889F:;
    mff_cycles(0x889F, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8832, 2); call_by_address_tail(0x8832, 0); return;
L_88A2:;
    mff_cycles(0x88A2, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x884E, 2); call_by_address_tail(0x884E, 0); return;
}

/*  $889F (bank 0) - lifted by port/tools/lift.py */
void func_889F_b0(void) {
    REGS_IN;
L_889F:;
    mff_cycles(0x889F, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8832, 2); call_by_address_tail(0x8832, 0); return;
}

/*  $88AA (bank 0) - lifted by port/tools/lift.py */
void func_88AA_b0(void) {
    REGS_IN;
L_88AA:;
    mff_cycles(0x88AA, 7);
    a = RAM(JOY_HELD);
    m = 0xC1; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x88AE, 1); goto L_88B6; }
    mff_cycles(0x88B0, 4);
    m = 0xC2; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x88B2, 1); goto L_88B6; }
    mff_cycles(0x88B4, 2);
    if (!z) { mff_cycles(0x88B4, 1); goto L_88C5; }
L_88B6:;
    mff_cycles(0x88B6, 19);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0x16;
    RAM(GAME_ACTION) = a;
    a = 0x4B;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x08C3, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x88C4, 6);
    SYNC_OUT;
    mff_rts(); return;
L_88C5:;
    mff_cycles(0x88C5, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x07; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x88CA, 1); goto L_88D5; }
    mff_cycles(0x88CC, 8);
    a = RAM(OBJ_ANIM_TIMER);
    m = 0x06; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x88D1, 1); goto L_88D5; }
    mff_cycles(0x88D3, 2);
    if (z) { mff_cycles(0x88D3, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x089F), 0); return; }
L_88D5:;
    mff_cycles(0x88D5, 10);
    a = RAM(OBJ_ANIM_FRAME);
    y = a;
    a = nes_read((uint16_t)(0x8903 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x88DC, 9);
    RAM(ATTACK_ID) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x08E0, func_F43B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x88E1, 17);
    a = RAM(OBJ_DIR);
    a ^= 0x03;
    RAM(OBJ_DIR) = a;
    a = RAM(DRAW_PARITY);
    a &= 0x07;
    z = (a) == 0;
    if (!z) { mff_cycles(0x88ED, 1); goto L_88F4; }
    mff_cycles(0x88EF, 8);
    a = 0x13;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x08F3, func_F34C, 0xE000)) return;
    SYNC_IN;
L_88F4:;
    mff_cycles(0x88F4, 14);
    a = RAM(PLAYER_GROUND_Y);
    RAM(ATTACK_GROUND_Y) = a;
    a = RAM(OBJ_ANIM_TIMER);
    m = 0x03; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x88FD, 2); goto L_8902; }
    mff_cycles(0x88FF, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0901, func_F436, 0xE000)) return;
    SYNC_IN;
L_8902:;
    mff_cycles(0x8902, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8B00 (bank 0) - lifted by port/tools/lift.py */
void func_8B00_b0(void) {
    REGS_IN;
L_8B00:;
    mff_cycles(0x8B00, 7);
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0x8B0F + y));
    mff_cycles(0x8B05, 7);
    RAM(0x00) = a;
    a = nes_read((uint16_t)(0x8B12 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8B0A, 8);
    RAM(0x01) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $8B15 (bank 0) - lifted by port/tools/lift.py */
void func_8B15_b0(void) {
    REGS_IN;
L_8B15:;
    mff_cycles(0x8B15, 9);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_FLAGS + y);
    n = (a) >> 7;
    if (!n) { mff_cycles(0x8B1A, 1); goto L_8B98; }
    mff_cycles(0x8B1C, 11);
    y = RAM(PLAYER_ID);
    a = 0x75;
    m = RAM(OBJ_ANIM); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8B23, 1); goto L_8B2E; }
    mff_cycles(0x8B25, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B27, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8B28, 4);
    a = nes_read((uint16_t)(0x8CBE + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8B2B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B2D, func_F440, 0xE000)) return;
    SYNC_IN;
L_8B2E:;
    mff_cycles(0x8B2E, 6);
    a = RAM(OBJ_ANIM_TIMER);
    z = (a) == 0;
    if (!z) { mff_cycles(0x8B31, 1); goto L_8B4A; }
    mff_cycles(0x8B33, 7);
    y = RAM(GRAB_HIT_COUNT);
    m = 0x03; c = y >= m;
    z = ((uint8_t)(y - m)) == 0;
    if (z) { mff_cycles(0x8B37, 1); goto L_8B98; }
    mff_cycles(0x8B39, 4);
    a = nes_read((uint16_t)(0x8CC1 + y));
    mff_cycles(0x8B3C, 19);
    RAM(ATTACK_ID) = a;
    y = RAM(GRAB_SLOT);
    a = 0x1C;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_HIT_TIMER + y) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0B47, func_F43B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8B48, 5);
    t = (uint8_t)(RAM(GRAB_HIT_COUNT) + 1); RAM(GRAB_HIT_COUNT) = t;
L_8B4A:;
    mff_cycles(0x8B4A, 8);
    a = RAM(OBJ_ANIM_TIMER);
    m = 0x03; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8B4F, 1); goto L_8B54; }
    mff_cycles(0x8B51, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B53, func_F436, 0xE000)) return;
    SYNC_IN;
L_8B54:;
    mff_cycles(0x8B54, 8);
    a = RAM(OBJ_ANIM_TIMER);
    m = 0x10; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8B59, 1); goto L_8B97; }
    mff_cycles(0x8B5B, 11);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_ID + y);
    m = 0x05; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8B62, 1); goto L_8B98; }
    mff_cycles(0x8B64, 4);
    m = 0x06; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8B66, 1); goto L_8B98; }
    mff_cycles(0x8B68, 4);
    m = 0x07; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8B6A, 1); goto L_8B98; }
    mff_cycles(0x8B6C, 4);
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8B6E, 1); goto L_8B98; }
    mff_cycles(0x8B70, 12);
    a = 0x04;
    RAM(GAME_ACTION) = a;
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE213 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8B79, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B7B, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8B7C, 4);
    a = nes_read((uint16_t)(0xE25B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8B7F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B81, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8B82, 4);
    a = nes_read((uint16_t)(0xE288 + y));
    mff_cycles(0x8B85, 24);
    RAM(PLAYER_STAND_ANIM) = a;
    a = 0x3C;
    RAM(GRAB_TIMER) = a;
    y = RAM(GRAB_SLOT);
    a = 0x00;
    RAM(OBJ_HIT_TIMER + y) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE) = a;
L_8B97:;
    mff_cycles(0x8B97, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8B98:;
    mff_cycles(0x8B98, 14);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(GRAB_HIT_COUNT) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0BA0, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8BA1, 13);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8BAB, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0BAF, 0xCEAA, 0)) return;
    SYNC_IN;
    mff_cycles(0x8BB0, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8B98 (bank 0) - lifted by port/tools/lift.py */
void func_8B98_b0(void) {
    REGS_IN;
L_8B98:;
    mff_cycles(0x8B98, 14);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(GRAB_HIT_COUNT) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0BA0, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8BA1, 13);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE288 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8BAB, 9);
    RAM(PLAYER_STAND_ANIM) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0BAF, 0xCEAA, 0)) return;
    SYNC_IN;
    mff_cycles(0x8BB0, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8BB1 (bank 0) - lifted by port/tools/lift.py */
void func_8BB1_b0(void) {
    REGS_IN;
L_8BB1:;
    mff_cycles(0x8BB1, 9);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_FLAGS + y);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0x8BB6, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0B98), 0); return; }
    mff_cycles(0x8BB8, 11);
    y = RAM(PLAYER_ID);
    a = 0x76;
    m = RAM(OBJ_ANIM); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8BBF, 1); goto L_8BCA; }
    mff_cycles(0x8BC1, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0BC3, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8BC4, 4);
    a = nes_read((uint16_t)(0x8CBE + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8BC7, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0BC9, func_F440, 0xE000)) return;
    SYNC_IN;
L_8BCA:;
    mff_cycles(0x8BCA, 6);
    a = RAM(OBJ_ANIM_TIMER);
    z = (a) == 0;
    if (!z) { mff_cycles(0x8BCD, 1); goto L_8BE6; }
    mff_cycles(0x8BCF, 7);
    y = RAM(GRAB_HIT_COUNT);
    m = 0x03; c = y >= m;
    n = ((uint8_t)(y - m)) >> 7;
    z = ((uint8_t)(y - m)) == 0;
    if (z) { mff_cycles(0x8BD3, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0B98), 0); return; }
    mff_cycles(0x8BD5, 4);
    a = nes_read((uint16_t)(0x8CC1 + y));
    mff_cycles(0x8BD8, 19);
    RAM(ATTACK_ID) = a;
    y = RAM(GRAB_SLOT);
    a = 0x1C;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_HIT_TIMER + y) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0BE3, func_F43B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8BE4, 5);
    t = (uint8_t)(RAM(GRAB_HIT_COUNT) + 1); RAM(GRAB_HIT_COUNT) = t;
L_8BE6:;
    mff_cycles(0x8BE6, 8);
    a = RAM(OBJ_ANIM_TIMER);
    m = 0x03; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8BEB, 1); goto L_8BF0; }
    mff_cycles(0x8BED, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0BEF, func_F436, 0xE000)) return;
    SYNC_IN;
L_8BF0:;
    mff_cycles(0x8BF0, 8);
    a = RAM(OBJ_ANIM_TIMER);
    m = 0x10; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8BF5, 2); goto L_8C37; }
    mff_cycles(0x8BF7, 11);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_ID + y);
    m = 0x05; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8BFE, 2); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0B98), 0); return; }
    mff_cycles(0x8C00, 4);
    m = 0x06; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8C02, 2); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0B98), 0); return; }
    mff_cycles(0x8C04, 4);
    m = 0x07; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8C06, 2); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0B98), 0); return; }
    mff_cycles(0x8C08, 4);
    m = 0x02; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8C0A, 2); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0B98), 0); return; }
    mff_cycles(0x8C0C, 12);
    a = 0x04;
    RAM(GAME_ACTION) = a;
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE213 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8C15, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C17, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8C18, 4);
    a = nes_read((uint16_t)(0xE25B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8C1B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C1D, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8C1E, 4);
    a = nes_read((uint16_t)(0xE288 + y));
    mff_cycles(0x8C21, 29);
    RAM(PLAYER_STAND_ANIM) = a;
    a = 0x3C;
    RAM(GRAB_TIMER) = a;
    y = RAM(GRAB_SLOT);
    a = 0x00;
    RAM(OBJ_HIT_TIMER + y) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PLAYER_STAND_ANIM) = a;
L_8C37:;
    mff_cycles(0x8C37, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8C38 (bank 0) - lifted by port/tools/lift.py */
void func_8C38_b0(void) {
    REGS_IN;
L_8C38:;
    mff_cycles(0x8C38, 11);
    y = RAM(PLAYER_ID);
    a = 0x77;
    m = RAM(OBJ_ANIM); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8C3F, 1); goto L_8C4A; }
    mff_cycles(0x8C41, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C43, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8C44, 4);
    a = nes_read((uint16_t)(0x8CBE + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8C47, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C49, func_F440, 0xE000)) return;
    SYNC_IN;
L_8C4A:;
    mff_cycles(0x8C4A, 6);
    a = RAM(OBJ_ANIM_TIMER);
    z = (a) == 0;
    if (!z) { mff_cycles(0x8C4D, 1); goto L_8C66; }
    mff_cycles(0x8C4F, 7);
    y = RAM(GRAB_HIT_COUNT);
    m = 0x03; c = y >= m;
    n = ((uint8_t)(y - m)) >> 7;
    z = ((uint8_t)(y - m)) == 0;
    if (z) { mff_cycles(0x8C53, 1); goto L_8CB8; }
    mff_cycles(0x8C55, 4);
    a = nes_read((uint16_t)(0x8CC1 + y));
    mff_cycles(0x8C58, 19);
    RAM(ATTACK_ID) = a;
    y = RAM(GRAB_SLOT);
    a = 0x1C;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_HIT_TIMER + y) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0C63, func_F43B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8C64, 5);
    t = (uint8_t)(RAM(GRAB_HIT_COUNT) + 1); RAM(GRAB_HIT_COUNT) = t;
L_8C66:;
    mff_cycles(0x8C66, 8);
    a = RAM(OBJ_ANIM_TIMER);
    m = 0x03; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8C6B, 1); goto L_8C70; }
    mff_cycles(0x8C6D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C6F, func_F436, 0xE000)) return;
    SYNC_IN;
L_8C70:;
    mff_cycles(0x8C70, 8);
    a = RAM(OBJ_ANIM_TIMER);
    m = 0x10; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8C75, 1); goto L_8CB7; }
    mff_cycles(0x8C77, 11);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_ID + y);
    m = 0x05; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8C7E, 1); goto L_8CB8; }
    mff_cycles(0x8C80, 4);
    m = 0x06; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8C82, 1); goto L_8CB8; }
    mff_cycles(0x8C84, 4);
    m = 0x07; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8C86, 1); goto L_8CB8; }
    mff_cycles(0x8C88, 4);
    m = 0x02; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x8C8A, 1); goto L_8CB8; }
    mff_cycles(0x8C8C, 12);
    a = 0x04;
    RAM(GAME_ACTION) = a;
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE213 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8C95, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C97, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8C98, 4);
    a = nes_read((uint16_t)(0xE25B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8C9B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C9D, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8C9E, 4);
    a = nes_read((uint16_t)(0xE288 + y));
    mff_cycles(0x8CA1, 29);
    RAM(PLAYER_STAND_ANIM) = a;
    a = 0x3C;
    RAM(GRAB_TIMER) = a;
    y = RAM(GRAB_SLOT);
    a = 0x00;
    RAM(OBJ_HIT_TIMER + y) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PLAYER_STAND_ANIM) = a;
L_8CB7:;
    mff_cycles(0x8CB7, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8CB8:;
    mff_cycles(0x8CB8, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8B98, 2); call_by_address_tail(0x8B98, 0); return;
}

/*  $8D8D (bank 0) - lifted by port/tools/lift.py */
void func_8D8D_b0(void) {
    REGS_IN;
L_8D8D:;
    mff_cycles(0x8D8D, 8);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    z = (a) == 0;
    if (!z) { mff_cycles(0x8D92, 1); goto L_8DC1; }
    mff_cycles(0x8D94, 7);
    a = RAM(ROUND);
    m = 0x03; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8D98, 1); goto L_8DD1; }
    mff_cycles(0x8D9A, 7);
    a = RAM(SUB_AREA);
    m = 0x01; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8D9E, 1); goto L_8DD1; }
    mff_cycles(0x8DA0, 8);
    a = RAM(OBJ_X);
    m = 0x80; c = a >= m;
    if (c) { mff_cycles(0x8DA5, 1); goto L_8DAB; }
    mff_cycles(0x8DA7, 4);
    a = 0x02;
    z = (a) == 0;
    if (!z) { mff_cycles(0x8DA9, 1); goto L_8DAD; }
L_8DAB:;
    mff_cycles(0x8DAB, 2);
    a = 0x01;
L_8DAD:;
    mff_cycles(0x8DAD, 11);
    RAM(OBJ_DIR) = a;
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE1C1 + y));
    mff_cycles(0x8DB5, 8);
    RAM(OBJ_VX_FRAC) = a;
    a = nes_read((uint16_t)(0xE1C4 + y));
    mff_cycles(0x8DBB, 10);
    RAM(OBJ_VX) = a;
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
L_8DC1:;
    mff_cycles(0x8DC1, 8);
    a = RAM(OBJ_DIR);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x8DC6, 1); goto L_8DCE; }
    mff_cycles(0x8DC8, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0DCA, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8DCB, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x8DD1, 2); call_by_address_tail(0x8DD1, 0); return;
L_8DCE:;
    mff_cycles(0x8DCE, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0DD0, func_F1A1, 0xE000)) return;
    SYNC_IN;
L_8DD1:;
    mff_cycles(0x8DD1, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0DD3, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8DD4, 7);
    a = RAM(ACTION_BEFORE_FALL);
    m = 0x04; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8DD8, 1); goto L_8DE0; }
    mff_cycles(0x8DDA, 7);
    a = RAM(PLAYER_ID);
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8DDE, 1); goto L_8DE0; }
L_8DE0:;
    mff_cycles(0x8DE0, 7);
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE219 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8DE5, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0DE7, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8DE8, 4);
    a = nes_read((uint16_t)(0xE22E + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8DEB, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0DED, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8DEE, 8);
    a = RAM(OBJ_Y);
    m = 0xA8; c = a >= m;
    if (!c) { mff_cycles(0x8DF3, 2); goto L_8E02; }
    mff_cycles(0x8DF5, 8);
    a = 0x28;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0DF9, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8DFA, 10);
    a = RAM(OBJ_FLAGS);
    a |= 0x04;
    RAM(OBJ_FLAGS) = a;
L_8E02:;
    mff_cycles(0x8E02, 38);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(SCROLL_Y) = a;
    RAM(JUMP_MOVING) = a;
    RAM(COMBO_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(PLAYER_INVULN) = a;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER2) = a;
    RAM(OBJ_GROUND_Y) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0E1B, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8E1C, 8);
    a = RAM(OBJ_Y);
    m = 0xE4; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x8E21, 1); goto L_8E3D; }
    mff_cycles(0x8E23, 16);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    t = (uint8_t)(RAM(LIVES) - 1); RAM(LIVES) = t;
    a = RAM(LIVES);
    n = (a) >> 7;
    if (!n) { mff_cycles(0x8E2C, 1); goto L_8E39; }
    mff_cycles(0x8E2E, 13);
    a = 0x00;
    RAM(SPECIAL_SCREEN_ID) = a;
    a = 0x09;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xDC6C, 2); call_by_address_tail(0xDC6C, -1); return;
L_8E39:;
    mff_cycles(0x8E39, 5);
    a = 0x25;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
L_8E3D:;
    mff_cycles(0x8E3D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8DD1 (bank 0) - lifted by port/tools/lift.py */
void func_8DD1_b0(void) {
    REGS_IN;
L_8DD1:;
    mff_cycles(0x8DD1, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0DD3, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8DD4, 7);
    a = RAM(ACTION_BEFORE_FALL);
    m = 0x04; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8DD8, 1); goto L_8DE0; }
    mff_cycles(0x8DDA, 7);
    a = RAM(PLAYER_ID);
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8DDE, 1); goto L_8DE0; }
L_8DE0:;
    mff_cycles(0x8DE0, 7);
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE219 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8DE5, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0DE7, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8DE8, 4);
    a = nes_read((uint16_t)(0xE22E + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8DEB, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0DED, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8DEE, 8);
    a = RAM(OBJ_Y);
    m = 0xA8; c = a >= m;
    if (!c) { mff_cycles(0x8DF3, 2); goto L_8E02; }
    mff_cycles(0x8DF5, 8);
    a = 0x28;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0DF9, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8DFA, 10);
    a = RAM(OBJ_FLAGS);
    a |= 0x04;
    RAM(OBJ_FLAGS) = a;
L_8E02:;
    mff_cycles(0x8E02, 38);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(SCROLL_Y) = a;
    RAM(JUMP_MOVING) = a;
    RAM(COMBO_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(PLAYER_INVULN) = a;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER2) = a;
    RAM(OBJ_GROUND_Y) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0E1B, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8E1C, 8);
    a = RAM(OBJ_Y);
    m = 0xE4; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x8E21, 1); goto L_8E3D; }
    mff_cycles(0x8E23, 16);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    t = (uint8_t)(RAM(LIVES) - 1); RAM(LIVES) = t;
    a = RAM(LIVES);
    n = (a) >> 7;
    if (!n) { mff_cycles(0x8E2C, 1); goto L_8E39; }
    mff_cycles(0x8E2E, 13);
    a = 0x00;
    RAM(SPECIAL_SCREEN_ID) = a;
    a = 0x09;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xDC6C, 2); call_by_address_tail(0xDC6C, -1); return;
L_8E39:;
    mff_cycles(0x8E39, 5);
    a = 0x25;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
L_8E3D:;
    mff_cycles(0x8E3D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8E3E (bank 0) - lifted by port/tools/lift.py */
void func_8E3E_b0(void) {
    REGS_IN;
L_8E3E:;
    mff_cycles(0x8E3E, 8);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x8E43, 1); goto L_8E8F; }
    mff_cycles(0x8E45, 5);
    a = RAM(SCROLL_X);
    z = (a) == 0;
    if (!z) { mff_cycles(0x8E47, 1); goto L_8E50; }
    mff_cycles(0x8E49, 8);
    a = 0x38;
    z = (a) == 0;
    RAM(OBJ_X) = a;
    if (!z) { mff_cycles(0x8E4E, 1); goto L_8E5F; }
L_8E50:;
    mff_cycles(0x8E50, 20);
    a = RAM(SCROLL_X);
    c = 0;
    m = 0x38; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(OBJ_X) = a;
    a = RAM(SCROLL_X_HI);
    m = 0x00; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(OBJ_X_HI) = a;
L_8E5F:;
    mff_cycles(0x8E5F, 22);
    a = 0x10;
    RAM(OBJ_Y) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y_HI) = a;
    RAM(OBJ_ID) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0E6E, 0xDE33, 0)) return;
    SYNC_IN;
    mff_cycles(0x8E6F, 7);
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE219 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8E74, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0E76, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8E77, 4);
    a = nes_read((uint16_t)(0xE22E + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x8E7A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0E7C, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8E7D, 24);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
    a = 0x80;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x05;
    RAM(OBJ_VY) = a;
    a = 0xD0;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS) = a;
L_8E8F:;
    mff_cycles(0x8E8F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0E91, func_F1BF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8E92, 7);
    a = RAM(ROUND);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8E96, 1); goto L_8E9C; }
    mff_cycles(0x8E98, 4);
    a = 0x98;
    z = (a) == 0;
    if (!z) { mff_cycles(0x8E9A, 1); goto L_8E9E; }
L_8E9C:;
    mff_cycles(0x8E9C, 2);
    a = 0x88;
L_8E9E:;
    mff_cycles(0x8E9E, 12);
    RAM(0x00) = a;
    a = RAM(OBJ_Y);
    m = RAM(0x00); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x8EA5, 1); goto L_8EE9; }
    mff_cycles(0x8EA7, 67);
    a = 0x01;
    RAM(PLAYER_DIR) = a;
    a = 0xB6;
    RAM(RESPAWN_TIMER2) = a;
    a = 0x1E;
    RAM(RESPAWN_TIMER1) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    RAM(PLAYER_NO_HIT) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(JUMP_MOVING) = a;
    RAM(SCROLL_Y) = a;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER2) = a;
    RAM(GRAB_HIT_COUNT) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(WEAPON_HELD) = a;
    RAM(COMBO_TIMER) = a;
    RAM(WEAPON_AMMO) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0ED6, 0xCEAA, 0)) return;
    SYNC_IN;
    mff_cycles(0x8ED7, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0ED9, func_F26E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x8EDA, 7);
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE288 + y));
    mff_cycles(0x8EDF, 10);
    RAM(PLAYER_STAND_ANIM) = a;
    a = RAM(SPECIAL_SCREEN_ID);
    m = 0xB5; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8EE5, 1); goto L_8EE9; }
    mff_cycles(0x8EE7, 5);
    t = (uint8_t)(RAM(SPECIAL_SCREEN_ID) + 1); RAM(SPECIAL_SCREEN_ID) = t;
    n = (t) >> 7;
    z = (t) == 0;
L_8EE9:;
    mff_cycles(0x8EE9, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8FAF (bank 0) - lifted by port/tools/lift.py */
void func_8FAF_b0(void) {
    REGS_IN;
L_8FAF:;
    mff_cycles(0x8FAF, 7);
    a = RAM(SCROLL_X);
    m = 0x28; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x8FB3, 1); goto L_8FB6; }
    mff_cycles(0x8FB5, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8FB6:;
    mff_cycles(0x8FB6, 5);
    a = RAM(ROUND);
    z = (a) == 0;
    if (!z) { mff_cycles(0x8FB8, 1); goto L_8FC7; }
    mff_cycles(0x8FBA, 7);
    a = RAM(SUB_AREA);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8FBE, 1); goto L_8FC7; }
    mff_cycles(0x8FC0, 7);
    a = RAM(AREA_SCREEN);
    m = 0x02; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8FC4, 1); goto L_8FC7; }
    mff_cycles(0x8FC6, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8FC7:;
    mff_cycles(0x8FC7, 7);
    a = RAM(ROUND);
    m = 0x03; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8FCB, 1); goto L_8FD8; }
    mff_cycles(0x8FCD, 5);
    a = RAM(SUB_AREA);
    z = (a) == 0;
    if (!z) { mff_cycles(0x8FCF, 1); goto L_8FD8; }
    mff_cycles(0x8FD1, 7);
    a = RAM(AREA_SCREEN);
    m = 0x02; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8FD5, 1); goto L_8FD8; }
    mff_cycles(0x8FD7, 6);
    SYNC_OUT;
    mff_rts(); return;
L_8FD8:;
    mff_cycles(0x8FD8, 8);
    a = RAM(OBJ_FLAGS);
    m = 0x90; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x8FDD, 1); goto L_8FE3; }
    mff_cycles(0x8FDF, 4);
    a = 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x8FE1, 1); goto L_8FE5; }
L_8FE3:;
    mff_cycles(0x8FE3, 2);
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
L_8FE5:;
    mff_cycles(0x8FE5, 9);
    RAM(PLAYER_DIR) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0FE9, 0xCEAA, 0)) return;
    SYNC_IN;
    mff_cycles(0x8FEA, 23);
    a = 0x00;
    RAM(GAME_ACTION) = a;
    RAM(CAMERA_AUTOSCROLL) = a;
    RAM(JUMP_MOVING) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $90AC (bank 0) - lifted by port/tools/lift.py */
void func_90AC_b0(void) {
    REGS_IN;
L_90AC:;
    mff_cycles(0x90AC, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x10AE, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x90AF, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x10B1, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x90B2, 8);
    a = RAM(OBJ_Y);
    m = 0x7C; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x90B7, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x1072), 0); return; }
    mff_cycles(0x90B9, 12);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $90CB (bank 0) - lifted by port/tools/lift.py */
void func_90CB_b0(void) {
    REGS_IN;
L_90CB:;
    mff_cycles(0x90CB, 5);
    a = RAM(ACTION_TIMER);
    z = (a) == 0;
    if (!z) { mff_cycles(0x90CD, 2); goto L_9103; }
L_90CF:;
    mff_cycles(0x90CF, 7);
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE210 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x90D4, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x10D6, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x90D7, 4);
    a = nes_read((uint16_t)(0xE258 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x90DA, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x10DC, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x90DD, 8);
    a = RAM(OBJ_FLAGS);
    m = 0xD0; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x90E2, 1); goto L_90E8; }
    mff_cycles(0x90E4, 4);
    a = 0x01;
    z = (a) == 0;
    if (!z) { mff_cycles(0x90E6, 1); goto L_90EA; }
L_90E8:;
    mff_cycles(0x90E8, 2);
    a = 0x02;
L_90EA:;
    mff_cycles(0x90EA, 38);
    RAM(PLAYER_DIR) = a;
    a = 0x02;
    RAM(GAME_ACTION) = a;
    a = 0x08;
    RAM(ACTION_TIMER) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(ATTACK_GROUND_Y) = a;
    RAM(JUMP_MOVING) = a;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER2) = a;
    SYNC_OUT;
    mff_rts(); return;
L_9103:;
    mff_cycles(0x9103, 5);
    a = RAM(HEALTH);
    n = (a) >> 7;
    if (!n) { mff_cycles(0x9105, 1); goto L_9114; }
    mff_cycles(0x9107, 21);
    a = 0x80;
    RAM(PLAYER_NO_HIT) = a;
    a = 0x0B;
    RAM(GAME_ACTION) = a;
    a = 0x78;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ACTION_TIMER) = a;
    SYNC_OUT;
    mff_rts(); return;
L_9114:;
    mff_cycles(0x9114, 7);
    a = RAM(JOY_PRESSED);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9118, 1); goto L_9139; }
    mff_cycles(0x911A, 4);
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x911C, 1); goto L_9139; }
    mff_cycles(0x911E, 4);
    m = 0x08; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9120, 1); goto L_9139; }
    mff_cycles(0x9122, 4);
    m = 0x04; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9124, 1); goto L_9139; }
    mff_cycles(0x9126, 4);
    m = 0x09; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9128, 1); goto L_9139; }
    mff_cycles(0x912A, 4);
    m = 0x0A; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x912C, 1); goto L_9139; }
    mff_cycles(0x912E, 4);
    m = 0x05; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9130, 1); goto L_9139; }
    mff_cycles(0x9132, 4);
    m = 0x06; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9134, 1); goto L_9139; }
    mff_cycles(0x9136, 11);
    t = (uint8_t)(RAM(ACTION_TIMER) - 1); RAM(ACTION_TIMER) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_9139:;
    mff_cycles(0x9139, 17);
    t = (uint8_t)(RAM(OBJ_TIMER) + 1); RAM(OBJ_TIMER) = t;
    a = RAM(OBJ_TIMER);
    y = RAM(PLAYER_LEVEL);
    m = nes_read((uint16_t)(0x9147 + y)); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x9144, 2);
    if (z) { mff_cycles(0x90CF, 2); goto L_90CF; }
    mff_cycles(0x9146, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9297 (bank 0) - lifted by port/tools/lift.py */
void func_9297_b0(void) {
    REGS_IN;
L_9297:;
    mff_cycles(0x9297, 38);
    a = 0x00;
    RAM(OBJ_X + 13) = a;
    a = 0x90;
    RAM(OBJ_Y + 13) = a;
    a = RAM(OBJ_X_HI);
    RAM(OBJ_X_HI + 13) = a;
    a = RAM(OBJ_Y_HI);
    RAM(OBJ_Y_HI + 13) = a;
    y = 0x0D;
    a = 0xB7;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x12B3, func_F2A0, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x92B4, 12);
    a = 0x6B;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ID + 13) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $93EF (bank 0) - lifted by port/tools/lift.py */
void func_93EF_b0(void) {
    REGS_IN;
L_93EF:;
    mff_cycles(0x93EF, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x93F6, 2); goto L_9413; }
    mff_cycles(0x93F8, 7);
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE1E6 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x93FD, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x13FF, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9400, 8);
    a = RAM(OBJ_X);
    m = 0xE0; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x9405, 1); goto L_940D; }
    mff_cycles(0x9407, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1409, func_F03F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x940A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9410, 2); call_by_address_tail(0x9410, 0); return;
L_940D:;
    mff_cycles(0x940D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x140F, func_F048, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9410, 6);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
L_9413:;
    mff_cycles(0x9413, 8);
    a = RAM(OBJ_Y);
    m = 0x82; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x9418, 1); goto L_941E; }
    mff_cycles(0x941A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x141C, func_F1BF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x941D, 6);
    SYNC_OUT;
    mff_rts(); return;
L_941E:;
    mff_cycles(0x941E, 8);
    a = RAM(OBJ_Y);
    m = 0x84; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x9423, 1); goto L_9429; }
    mff_cycles(0x9425, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1427, func_F1DF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9428, 6);
    SYNC_OUT;
    mff_rts(); return;
L_9429:;
    mff_cycles(0x9429, 8);
    a = RAM(OBJ_X);
    m = 0xE0; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x942E, 1); goto L_9439; }
    mff_cycles(0x9430, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1432, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9433, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1435, func_F048, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9436, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x943F, 2); call_by_address_tail(0x943F, 0); return;
L_9439:;
    mff_cycles(0x9439, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x143B, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x943C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x143E, func_F03F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x943F, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x03; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9446, 1); goto L_9452; }
    mff_cycles(0x9448, 8);
    a = RAM(OBJ_X);
    m = 0xD0; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x944D, 1); goto L_945D; }
    mff_cycles(0x944F, 6);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
L_9452:;
    mff_cycles(0x9452, 8);
    a = RAM(OBJ_X);
    m = 0xE0; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x9457, 1); goto L_945D; }
    mff_cycles(0x9459, 5);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
L_945D:;
    mff_cycles(0x945D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9410 (bank 0) - lifted by port/tools/lift.py */
void func_9410_b0(void) {
    REGS_IN;
L_9410:;
    mff_cycles(0x9410, 14);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
    a = RAM(OBJ_Y);
    m = 0x82; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x9418, 1); goto L_941E; }
    mff_cycles(0x941A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x141C, func_F1BF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x941D, 6);
    SYNC_OUT;
    mff_rts(); return;
L_941E:;
    mff_cycles(0x941E, 8);
    a = RAM(OBJ_Y);
    m = 0x84; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x9423, 1); goto L_9429; }
    mff_cycles(0x9425, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1427, func_F1DF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9428, 6);
    SYNC_OUT;
    mff_rts(); return;
L_9429:;
    mff_cycles(0x9429, 8);
    a = RAM(OBJ_X);
    m = 0xE0; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x942E, 1); goto L_9439; }
    mff_cycles(0x9430, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1432, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9433, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1435, func_F048, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9436, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x943F, 2); call_by_address_tail(0x943F, 0); return;
L_9439:;
    mff_cycles(0x9439, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x143B, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x943C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x143E, func_F03F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x943F, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x03; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9446, 1); goto L_9452; }
    mff_cycles(0x9448, 8);
    a = RAM(OBJ_X);
    m = 0xD0; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x944D, 1); goto L_945D; }
    mff_cycles(0x944F, 6);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
L_9452:;
    mff_cycles(0x9452, 8);
    a = RAM(OBJ_X);
    m = 0xE0; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x9457, 1); goto L_945D; }
    mff_cycles(0x9459, 5);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
L_945D:;
    mff_cycles(0x945D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $943F (bank 0) - lifted by port/tools/lift.py */
void func_943F_b0(void) {
    REGS_IN;
L_943F:;
    mff_cycles(0x943F, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x03; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9446, 1); goto L_9452; }
    mff_cycles(0x9448, 8);
    a = RAM(OBJ_X);
    m = 0xD0; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x944D, 1); goto L_945D; }
    mff_cycles(0x944F, 6);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
L_9452:;
    mff_cycles(0x9452, 8);
    a = RAM(OBJ_X);
    m = 0xE0; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x9457, 1); goto L_945D; }
    mff_cycles(0x9459, 5);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
L_945D:;
    mff_cycles(0x945D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $945E (bank 0) - lifted by port/tools/lift.py */
void func_945E_b0(void) {
    REGS_IN;
L_945E:;
    mff_cycles(0x945E, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9465, 1); goto L_9496; }
    mff_cycles(0x9467, 4);
    m = 0x03; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9469, 1); goto L_94C5; }
    mff_cycles(0x946B, 11);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    m = nes_read((uint16_t)(0xE273 + y)); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x9473, 2);
    if (z) { mff_cycles(0x9473, 1); goto L_9481; }
    mff_cycles(0x9475, 4);
    a = nes_read((uint16_t)(0xE1E6 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9478, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x147A, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x947B, 4);
    a = nes_read((uint16_t)(0xE22B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x947E, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1480, func_F440, 0xE000)) return;
    SYNC_IN;
L_9481:;
    mff_cycles(0x9481, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1483, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9484, 4);
    m = 0xB0; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x9486, 1); goto L_94C4; }
    mff_cycles(0x9488, 8);
    y = 0x01;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x148C, 0x987A, 0)) return;
    SYNC_IN;
    mff_cycles(0x948D, 18);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER) = a;
    SYNC_OUT;
    mff_rts(); return;
L_9496:;
    mff_cycles(0x9496, 6);
    a = RAM(OBJ_TIMER);
    z = (a) == 0;
    if (!z) { mff_cycles(0x9499, 1); goto L_94C1; }
    mff_cycles(0x949B, 8);
    y = 0x02;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x149F, 0x987A, 0)) return;
    SYNC_IN;
    mff_cycles(0x94A0, 7);
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE219 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x94A5, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x14A7, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x94A8, 4);
    a = nes_read((uint16_t)(0xE22E + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x94AB, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x14AD, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x94AE, 26);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
    a = 0x00;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x02;
    RAM(OBJ_VY) = a;
    a = 0x24;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x14BF, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x94C0, 6);
    SYNC_OUT;
    mff_rts(); return;
L_94C1:;
    mff_cycles(0x94C1, 6);
    t = (uint8_t)(RAM(OBJ_TIMER) - 1); RAM(OBJ_TIMER) = t;
    n = (t) >> 7;
    z = (t) == 0;
L_94C4:;
    mff_cycles(0x94C4, 6);
    SYNC_OUT;
    mff_rts(); return;
L_94C5:;
    mff_cycles(0x94C5, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x14C7, func_F1BF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x94C8, 8);
    a = RAM(OBJ_Y);
    m = 0x88; c = a >= m;
    if (!c) { mff_cycles(0x94CD, 1); goto L_94D7; }
    mff_cycles(0x94CF, 10);
    a = RAM(OBJ_FLAGS);
    a |= 0x20;
    RAM(OBJ_FLAGS) = a;
L_94D7:;
    mff_cycles(0x94D7, 8);
    a = RAM(OBJ_Y);
    m = 0xAC; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x94C4, 1); goto L_94C4; }
    mff_cycles(0x94DE, 14);
    a = 0xF4;
    RAM(OBJ_FLAGS) = a;
    a = RAM(OBJ_Y);
    m = 0xE8; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x94C4, 1); goto L_94C4; }
    mff_cycles(0x94EA, 11);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $94F2 (bank 0) - lifted by port/tools/lift.py */
void func_94F2_b0(void) {
    REGS_IN;
L_94F2:;
    mff_cycles(0x94F2, 11);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $94F7 (bank 0) - lifted by port/tools/lift.py */
void func_94F7_b0(void) {
    REGS_IN;
L_94F7:;
    mff_cycles(0x94F7, 30);
    a = 0xFF;
    RAM(0xB2) = a;
    a = 0x7F;
    RAM(AREA_CLEAR) = a;
    a = 0xFD;
    RAM(SPECIAL_SCREEN_ID) = a;
    a = 0x23;
    RAM(CHR_BANKS + 3) = a;
    RAM(CHR_BANKS + 4) = a;
    a = 0x35;
    RAM(CHR_BANKS + 5) = a;
    x = 0x00;
L_950F:;
    mff_cycles(0x950F, 4);
    a = nes_read((uint16_t)(0x955D + x));
    mff_cycles(0x9512, 16);
    RAM(PALETTE_BUF + 24 + x) = a;
    RAM(PALETTE_BASE + 24 + x) = a;
    x++;
    m = 0x08; c = x >= m;
    z = ((uint8_t)(x - m)) == 0;
    if (!z) { mff_cycles(0x950F, 1); goto L_950F; }
    mff_cycles(0x951D, 31);
    x = 0xFF;
    RAM(PALETTE_REQUEST) = x;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(JUMP_MOVING) = a;
    RAM(COMBO_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(PLAYER_INVULN) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    SYNC_OUT;
    if (!mff_call_at(0x1531, 0xCEAA, 0)) return;
    SYNC_IN;
    mff_cycles(0x9532, 60);
    a = 0x80;
    RAM(OBJ_STATE + 15) = a;
    RAM(OBJ_FLAGS + 15) = a;
    a = 0x00;
    RAM(OBJ_X + 15) = a;
    RAM(OBJ_Y + 15) = a;
    RAM(OBJ_ANIM + 15) = a;
    RAM(OBJ_ANIM_TIMER + 15) = a;
    RAM(OBJ_ANIM_FRAME + 15) = a;
    a = RAM(OBJ_X_HI);
    RAM(OBJ_X_HI + 15) = a;
    a = RAM(OBJ_Y_HI);
    RAM(OBJ_Y_HI + 15) = a;
    a = 0x70;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ID + 15) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9565 (bank 0) - lifted by port/tools/lift.py */
void func_9565_b0(void) {
    REGS_IN;
L_9565:;
    mff_cycles(0x9565, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x02; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x956C, 1); goto L_957C; }
    mff_cycles(0x956E, 13);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE1E6 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9576, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1578, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9579, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x157B, func_F03F, 0xE000)) return;
    SYNC_IN;
L_957C:;
    mff_cycles(0x957C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x157E, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x957F, 8);
    a = RAM(OBJ_X);
    m = 0xFC; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x9584, 1); goto L_958A; }
    mff_cycles(0x9586, 5);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
L_958A:;
    mff_cycles(0x958A, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $958B (bank 0) - lifted by port/tools/lift.py */
void func_958B_b0(void) {
    REGS_IN;
L_958B:;
    mff_cycles(0x958B, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9592, 1); goto L_95BD; }
    mff_cycles(0x9594, 4);
    m = 0x03; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9596, 2); goto L_960B; }
    mff_cycles(0x9598, 8);
    y = 0x03;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x159C, 0x987A, 0)) return;
    SYNC_IN;
    mff_cycles(0x959D, 8);
    a = 0x21;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x15A1, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x95A2, 7);
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE1E6 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x95A7, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x15A9, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x95AA, 8);
    a = RAM(OBJ_X);
    m = 0xE0; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x95AF, 1); goto L_95B7; }
    mff_cycles(0x95B1, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x15B3, func_F048, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x95B4, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x95BA, 2); call_by_address_tail(0x95BA, 0); return;
L_95B7:;
    mff_cycles(0x95B7, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x15B9, func_F03F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x95BA, 6);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
L_95BD:;
    mff_cycles(0x95BD, 8);
    a = RAM(OBJ_Y);
    m = 0x9C; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x95C2, 1); goto L_95C8; }
    mff_cycles(0x95C4, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x15C6, func_F1BF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x95C7, 6);
    SYNC_OUT;
    mff_rts(); return;
L_95C8:;
    mff_cycles(0x95C8, 8);
    a = RAM(OBJ_Y);
    m = 0xA4; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x95CD, 1); goto L_95D3; }
    mff_cycles(0x95CF, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x15D1, func_F1DF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x95D2, 6);
    SYNC_OUT;
    mff_rts(); return;
L_95D3:;
    mff_cycles(0x95D3, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x03; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x95DA, 2); goto L_960F; }
    mff_cycles(0x95DC, 8);
    a = RAM(OBJ_FLAGS);
    m = 0x90; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x95E1, 1); goto L_95F0; }
    mff_cycles(0x95E3, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x15E5, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x95E6, 8);
    a = RAM(OBJ_X);
    m = 0xC8; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x95EB, 2); goto L_960F; }
    mff_cycles(0x95ED, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x95FA, 2); call_by_address_tail(0x95FA, 0); return;
L_95F0:;
    mff_cycles(0x95F0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x15F2, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x95F3, 8);
    a = RAM(OBJ_X);
    m = 0xC8; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x95F8, 2); goto L_960F; }
    mff_cycles(0x95FA, 7);
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE219 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x95FF, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1601, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9602, 4);
    a = nes_read((uint16_t)(0xE22E + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9605, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1607, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9608, 6);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
L_960B:;
    mff_cycles(0x960B, 5);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
L_960F:;
    mff_cycles(0x960F, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $95BA (bank 0) - lifted by port/tools/lift.py */
void func_95BA_b0(void) {
    REGS_IN;
L_95BA:;
    mff_cycles(0x95BA, 14);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
    a = RAM(OBJ_Y);
    m = 0x9C; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x95C2, 1); goto L_95C8; }
    mff_cycles(0x95C4, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x15C6, func_F1BF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x95C7, 6);
    SYNC_OUT;
    mff_rts(); return;
L_95C8:;
    mff_cycles(0x95C8, 8);
    a = RAM(OBJ_Y);
    m = 0xA4; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x95CD, 1); goto L_95D3; }
    mff_cycles(0x95CF, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x15D1, func_F1DF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x95D2, 6);
    SYNC_OUT;
    mff_rts(); return;
L_95D3:;
    mff_cycles(0x95D3, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x03; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x95DA, 2); goto L_960F; }
    mff_cycles(0x95DC, 8);
    a = RAM(OBJ_FLAGS);
    m = 0x90; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x95E1, 1); goto L_95F0; }
    mff_cycles(0x95E3, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x15E5, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x95E6, 8);
    a = RAM(OBJ_X);
    m = 0xC8; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0x95EB, 2); goto L_960F; }
    mff_cycles(0x95ED, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x95FA, 2); call_by_address_tail(0x95FA, 0); return;
L_95F0:;
    mff_cycles(0x95F0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x15F2, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x95F3, 8);
    a = RAM(OBJ_X);
    m = 0xC8; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x95F8, 2); goto L_960F; }
    mff_cycles(0x95FA, 7);
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE219 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x95FF, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1601, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9602, 4);
    a = nes_read((uint16_t)(0xE22E + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9605, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1607, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9608, 11);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
L_960F:;
    mff_cycles(0x960F, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $95FA (bank 0) - lifted by port/tools/lift.py */
void func_95FA_b0(void) {
    REGS_IN;
L_95FA:;
    mff_cycles(0x95FA, 7);
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE219 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x95FF, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1601, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9602, 4);
    a = nes_read((uint16_t)(0xE22E + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9605, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1607, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9608, 17);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9610 (bank 0) - lifted by port/tools/lift.py */
void func_9610_b0(void) {
    REGS_IN;
L_9610:;
    mff_cycles(0x9610, 11);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9615 (bank 0) - lifted by port/tools/lift.py */
void func_9615_b0(void) {
    REGS_IN;
L_9615:;
    mff_cycles(0x9615, 11);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $961A (bank 0) - lifted by port/tools/lift.py */
void func_961A_b0(void) {
    REGS_IN;
L_961A:;
    mff_cycles(0x961A, 11);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $961F (bank 0) - lifted by port/tools/lift.py */
void func_961F_b0(void) {
    REGS_IN;
L_961F:;
    mff_cycles(0x961F, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x9626, 1); goto L_9630; }
    mff_cycles(0x9628, 12);
    a = 0xB4;
    RAM(OBJ_TIMER) = a;
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
L_9630:;
    mff_cycles(0x9630, 12);
    t = (uint8_t)(RAM(OBJ_TIMER) - 1); RAM(OBJ_TIMER) = t;
    a = RAM(OBJ_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x9636, 1); goto L_963C; }
    mff_cycles(0x9638, 5);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
L_963C:;
    mff_cycles(0x963C, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $963D (bank 0) - lifted by port/tools/lift.py */
void func_963D_b0(void) {
    REGS_IN;
L_963D:;
    mff_cycles(0x963D, 8);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    z = (a) == 0;
    if (!z) { mff_cycles(0x9642, 1); goto L_9662; }
    mff_cycles(0x9644, 29);
    RAM(OBJ_VX_FRAC) = a;
    a = 0x01;
    RAM(OBJ_VX) = a;
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
    a = 0x1E;
    RAM(OBJ_TIMER) = a;
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE1F2 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9659, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x165B, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x965C, 4);
    a = nes_read((uint16_t)(0xE23A + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x965F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1661, func_F440, 0xE000)) return;
    SYNC_IN;
L_9662:;
    mff_cycles(0x9662, 8);
    a = RAM(OBJ_STATE);
    a &= 0x02;
    z = (a) == 0;
    if (!z) { mff_cycles(0x9667, 1); goto L_9686; }
    mff_cycles(0x9669, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x03; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x966E, 1); goto L_9685; }
    mff_cycles(0x9670, 6);
    a = RAM(OBJ_ANIM_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x9673, 1); goto L_9685; }
    mff_cycles(0x9675, 8);
    a = 0x21;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1679, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x967A, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x167E, 0x987A, 0)) return;
    SYNC_IN;
    mff_cycles(0x967F, 12);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x1684, 0xCEAA, 0)) return;
    SYNC_IN;
L_9685:;
    mff_cycles(0x9685, 6);
    SYNC_OUT;
    mff_rts(); return;
L_9686:;
    mff_cycles(0x9686, 6);
    a = RAM(OBJ_TIMER);
    z = (a) == 0;
    if (z) { mff_cycles(0x9689, 1); goto L_968F; }
    mff_cycles(0x968B, 12);
    t = (uint8_t)(RAM(OBJ_TIMER) - 1); RAM(OBJ_TIMER) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_968F:;
    mff_cycles(0x968F, 11);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    m = nes_read((uint16_t)(0xE273 + y)); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x9697, 2);
    if (z) { mff_cycles(0x9697, 1); goto L_96A5; }
    mff_cycles(0x9699, 4);
    a = nes_read((uint16_t)(0xE1E6 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x969C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x169E, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x969F, 4);
    a = nes_read((uint16_t)(0xE22B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x96A2, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x16A4, func_F440, 0xE000)) return;
    SYNC_IN;
L_96A5:;
    mff_cycles(0x96A5, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x16A7, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x96A8, 8);
    a = RAM(OBJ_X);
    m = 0x1C; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x96AD, 1); goto L_96DB; }
    mff_cycles(0x96AF, 18);
    a = RAM(OBJ_FLAGS);
    a &= 0xDF;
    RAM(OBJ_FLAGS) = a;
    a = RAM(OBJ_X);
    m = 0x30; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x96BC, 1); goto L_96DB; }
    mff_cycles(0x96BE, 20);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0x00;
    RAM(GAME_ACTION) = a;
    RAM(OBJ_TIMER) = a;
    a = RAM(ROUND);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x96CC, 1); goto L_96DB; }
    mff_cycles(0x96CE, 7);
    a = RAM(SUB_AREA);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x96D2, 1); goto L_96DC; }
    mff_cycles(0x96D4, 4);
    m = 0x03; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x96D6, 1); goto L_96DC; }
    mff_cycles(0x96D8, 6);
    SYNC_OUT;
    if (!mff_call_at(0x16DA, 0xCEAA, 0)) return;
    SYNC_IN;
L_96DB:;
    mff_cycles(0x96DB, 6);
    SYNC_OUT;
    mff_rts(); return;
L_96DC:;
    mff_cycles(0x96DC, 12);
    a = 0x20;
    RAM(GAME_ACTION) = a;
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE1C1 + y));
    mff_cycles(0x96E5, 8);
    RAM(OBJ_VX_FRAC) = a;
    a = nes_read((uint16_t)(0xE1C4 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x96EB, 10);
    RAM(OBJ_VX) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $96EF (bank 0) - lifted by port/tools/lift.py */
void func_96EF_b0(void) {
    REGS_IN;
L_96EF:;
    mff_cycles(0x96EF, 8);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    z = (a) == 0;
    if (!z) { mff_cycles(0x96F4, 2); goto L_970F; }
    mff_cycles(0x96F6, 17);
    RAM(OBJ_VY_FRAC) = a;
    a = 0x05;
    RAM(OBJ_VY) = a;
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE20A + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9703, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1705, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9706, 4);
    a = nes_read((uint16_t)(0xE252 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9709, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x170B, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x970C, 6);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
L_970F:;
    mff_cycles(0x970F, 10);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    m = 0x02; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9716, 1); goto L_974A; }
    mff_cycles(0x9718, 4);
    m = 0x03; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x971A, 1); goto L_9779; }
    mff_cycles(0x971C, 4);
    m = 0x04; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x971E, 1); goto L_9723; }
    mff_cycles(0x9720, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x97A9, 2); call_by_address_tail(0x97A9, 0); return;
L_9723:;
    mff_cycles(0x9723, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1725, func_F1BF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9726, 8);
    a = RAM(OBJ_Y);
    m = 0x98; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x972B, 1); goto L_9749; }
    mff_cycles(0x972D, 8);
    a = 0x05;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1731, func_F34A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9732, 30);
    a = 0xE6;
    RAM(OBJ_VX_FRAC) = a;
    a = 0x00;
    RAM(OBJ_VX) = a;
    a = 0xE8;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x03;
    RAM(OBJ_VY) = a;
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
    n = (t) >> 7;
    z = (t) == 0;
L_9749:;
    mff_cycles(0x9749, 6);
    SYNC_OUT;
    mff_rts(); return;
L_974A:;
    mff_cycles(0x974A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x174C, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x974D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x174F, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9750, 8);
    a = RAM(OBJ_Y);
    m = 0xA0; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x9749, 1); goto L_9749; }
    mff_cycles(0x9757, 13);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE20D + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x975F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1761, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9762, 4);
    a = nes_read((uint16_t)(0xE255 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9765, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1767, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9768, 6);
    y = 0x0F;
    a = nes_read((uint16_t)(0x97F9 + y));
    mff_cycles(0x976D, 14);
    RAM(PALETTE_BUF + y) = a;
    RAM(PALETTE_BASE + y) = a;
    y--;
    n = (y) >> 7;
    z = (y) == 0;
    if (!n) { mff_cycles(0x9774, 1); goto L_979A; }
    mff_cycles(0x9776, 9);
    RAM(PALETTE_REQUEST) = y;
    SYNC_OUT;
    mff_rts(); return;
L_9779:;
    mff_cycles(0x9779, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x06; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x977E, 1); goto L_97A8; }
    mff_cycles(0x9780, 8);
    a = RAM(OBJ_ANIM_TIMER);
    m = 0x0A; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x9785, 1); goto L_97A8; }
    mff_cycles(0x9787, 7);
    y = RAM(PLAYER_ID);
    a = nes_read((uint16_t)(0xE210 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x978C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x178E, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x978F, 4);
    a = nes_read((uint16_t)(0xE258 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9792, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1794, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9795, 8);
    t = (uint8_t)(RAM(OBJ_STATE) + 1); RAM(OBJ_STATE) = t;
    y = 0x0F;
L_979A:;
    mff_cycles(0x979A, 4);
    a = nes_read((uint16_t)(0x97F9 + y));
    mff_cycles(0x979D, 14);
    RAM(PALETTE_BUF + y) = a;
    RAM(PALETTE_BASE + y) = a;
    y--;
    n = (y) >> 7;
    z = (y) == 0;
    if (!n) { mff_cycles(0x979A, 1); goto L_979A; }
    mff_cycles(0x97A6, 3);
    RAM(PALETTE_REQUEST) = y;
L_97A8:;
    mff_cycles(0x97A8, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $97A9 (bank 0) - lifted by port/tools/lift.py */
void func_97A9_b0(void) {
    REGS_IN;
    goto L_97A9;
L_97A8:;
    mff_cycles(0x97A8, 6);
    SYNC_OUT;
    mff_rts(); return;
L_97A9:;
    mff_cycles(0x97A9, 8);
    a = RAM(OBJ_ANIM_TIMER);
    m = 0x08; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x97A8, 1); goto L_97A8; }
    mff_cycles(0x97B0, 2);
    y = 0x0F;
L_97B2:;
    mff_cycles(0x97B2, 4);
    a = nes_read((uint16_t)(0x9809 + y));
    mff_cycles(0x97B5, 14);
    RAM(PALETTE_BUF + y) = a;
    RAM(PALETTE_BASE + y) = a;
    y--;
    n = (y) >> 7;
    z = (y) == 0;
    if (!n) { mff_cycles(0x97B2, 1); goto L_97B2; }
    mff_cycles(0x97BE, 9);
    RAM(PALETTE_REQUEST) = y;
    SYNC_OUT;
    if (!mff_call_at(0x17C2, 0x97C8, 0)) return;
    SYNC_IN;
    mff_cycles(0x97C3, 11);
    a = 0x20;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $97C8 (bank 0) - lifted by port/tools/lift.py */
void func_97C8_b0(void) {
    REGS_IN;
L_97C8:;
    mff_cycles(0x97C8, 46);
    a = 0x80;
    RAM(BG_PAL_ANIM_ON + 2) = a;
    RAM(BG_PAL_ANIM_ON + 3) = a;
    a = 0x05;
    RAM(BG_PAL_ANIM_SCRIPT + 2) = a;
    a = 0x06;
    RAM(BG_PAL_ANIM_SCRIPT + 3) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(BG_PAL_ANIM_STEP + 2) = a;
    RAM(BG_PAL_ANIM_TIMER + 2) = a;
    RAM(BG_PAL_ANIM_STEP + 3) = a;
    RAM(BG_PAL_ANIM_TIMER + 3) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9819 (bank 0) - lifted by port/tools/lift.py */
void func_9819_b0(void) {
    REGS_IN;
L_9819:;
    mff_cycles(0x9819, 11);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    m = nes_read((uint16_t)(0xE273 + y)); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x9821, 2);
    if (z) { mff_cycles(0x9821, 1); goto L_983B; }
    mff_cycles(0x9823, 4);
    a = nes_read((uint16_t)(0xE1E6 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9826, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1828, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9829, 4);
    a = nes_read((uint16_t)(0xE22B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x982C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x182E, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x982F, 4);
    a = nes_read((uint16_t)(0xE1C1 + y));
    mff_cycles(0x9832, 8);
    RAM(OBJ_VX_FRAC) = a;
    a = nes_read((uint16_t)(0xE1C4 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x9838, 4);
    RAM(OBJ_VX) = a;
L_983B:;
    mff_cycles(0x983B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x183D, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x983E, 5);
    a = RAM(ROUND);
    z = (a) == 0;
    if (!z) { mff_cycles(0x9840, 1); goto L_985F; }
    mff_cycles(0x9842, 7);
    a = RAM(SUB_AREA);
    m = 0x01; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x9846, 1); goto L_985E; }
    mff_cycles(0x9848, 8);
    a = RAM(OBJ_X);
    m = 0xB0; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x984D, 1); goto L_985E; }
    mff_cycles(0x984F, 21);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(OBJ_TIMER) = a;
    SYNC_OUT;
    if (!mff_call_at(0x185D, 0xCEAA, 0)) return;
    SYNC_IN;
L_985E:;
    mff_cycles(0x985E, 6);
    SYNC_OUT;
    mff_rts(); return;
L_985F:;
    mff_cycles(0x985F, 7);
    a = RAM(ROUND);
    m = 0x04; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x985E, 1); goto L_985E; }
    mff_cycles(0x9865, 7);
    a = RAM(SUB_AREA);
    m = 0x02; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x985E, 1); goto L_985E; }
    mff_cycles(0x986B, 6);
    a = RAM(OBJ_X_HI);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x985E, 1); goto L_985E; }
    mff_cycles(0x9870, 8);
    a = RAM(OBJ_X);
    m = 0x80; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0x985E, 1); goto L_985E; }
    mff_cycles(0x9877, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x984F, 2); call_by_address_tail(0x984F, 0); return;
}

/*  $984F (bank 0) - lifted by port/tools/lift.py */
void func_984F_b0(void) {
    REGS_IN;
L_984F:;
    mff_cycles(0x984F, 21);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(OBJ_TIMER) = a;
    SYNC_OUT;
    if (!mff_call_at(0x185D, 0xCEAA, 0)) return;
    SYNC_IN;
    mff_cycles(0x985E, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $987A (bank 0) - lifted by port/tools/lift.py */
void func_987A_b0(void) {
    REGS_IN;
L_987A:;
    mff_cycles(0x987A, 7);
    RAM(0x0F) = x;
    a = nes_read((uint16_t)(0x989A + y));
    mff_cycles(0x987F, 7);
    RAM(0x00) = a;
    a = nes_read((uint16_t)(0x989E + y));
    mff_cycles(0x9884, 9);
    RAM(0x01) = a;
    a = 0x00;
    x = a;
    y = a;
L_988A:;
    mff_cycles(0x988A, 5);
    a = nes_read((uint16_t)(RAM16(0x00) + y));
    mff_cycles(0x988C, 13);
    RAM(0x0780 + x) = a;
    x++;
    y++;
    m = 0x80; c = y >= m;
    z = ((uint8_t)(y - m)) == 0;
    if (!z) { mff_cycles(0x988A, 1); goto L_988A; }
    mff_cycles(0x9895, 12);
    RAM(VRAM_LIST_REQUEST) = y;
    x = RAM(0x0F);
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9A00 (bank 0) - lifted by port/tools/lift.py */
void func_9A00_b0(void) {
    REGS_IN;
L_9A00:;
    mff_cycles(0x9A00, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1A02, 0x9AC0, 0)) return;
    SYNC_IN;
    mff_cycles(0x9A03, 7);
    y = RAM(INTRO_PAGE);
    a = nes_read((uint16_t)(0x9B2C + y));
    mff_cycles(0x9A08, 7);
    RAM(0x0C) = a;
    a = nes_read((uint16_t)(0x9B33 + y));
    mff_cycles(0x9A0D, 16);
    RAM(0x0D) = a;
    a = 0x00;
    RAM(0x0E) = a;
    a = 0x00;
    RAM(0x10) = a;
    RAM(0x12) = a;
L_9A19:;
    mff_cycles(0x9A19, 12);
    a = 0x00;
    RAM(0x11) = a;
    a = RAM(JOY_PRESSED);
    a &= 0xF0;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x9A21, 1); goto L_9A91; }
    mff_cycles(0x9A23, 7);
    x = RAM(0x10);
    a = nes_read((uint16_t)(0x9A92 + x));
    mff_cycles(0x9A28, 8);
    RAM(0x0780) = a;
    a = nes_read((uint16_t)(0x9A95 + x));
    mff_cycles(0x9A2E, 28);
    c = 0;
    m = RAM(0x11); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(0x0781) = a;
    a = 0x00;
    RAM(0x0782) = a;
    y = RAM(0x0E);
    t = (uint8_t)(RAM(0x0E) + 1); RAM(0x0E) = t;
    a = nes_read((uint16_t)(RAM16(0x0C) + y));
    mff_cycles(0x9A3F, 4);
    m = 0xFF; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9A41, 1); goto L_9A8E; }
    mff_cycles(0x9A43, 4);
    m = 0xFE; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9A45, 1); goto L_9A7D; }
    mff_cycles(0x9A47, 31);
    RAM(0x0783) = a;
    a = 0xFF;
    RAM(0x0784) = a;
    RAM(VRAM_LIST_REQUEST) = a;
    a = RAM(0x10);
    mff_push(a);
    a = RAM(0x11);
    n = (a) >> 7;
    z = (a) == 0;
    mff_push(a);
    SYNC_OUT;
    if (!mff_call_at(0x1A59, 0x9A98, 0)) return;
    SYNC_IN;
    mff_cycles(0x9A5A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1A5C, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9A5D, 6);
    SYNC_OUT;
    mff_call_fn(0x1A5F, func_FF31, 0xE000);
    SYNC_IN;
    mff_cycles(0x9A60, 6);
    SYNC_OUT;
    mff_call_fn(0x1A62, func_FF31, 0xE000);
    SYNC_IN;
    mff_cycles(0x9A63, 26);
    a = mff_pull();
    RAM(0x11) = a;
    a = mff_pull();
    RAM(0x10) = a;
    t = (uint8_t)(RAM(0x11) + 1); RAM(0x11) = t;
    a = RAM(FRAME_COUNTER);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x9A6F, 1); goto L_9A7A; }
    mff_cycles(0x9A71, 5);
    a = RAM(INTRO_PAGE);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x9A73, 1); goto L_9A7A; }
    mff_cycles(0x9A75, 8);
    a = 0x2B;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1A79, func_F34C, 0xE000)) return;
    SYNC_IN;
L_9A7A:;
    mff_cycles(0x9A7A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9A1D, 2); call_by_address_tail(0x9A1D, 0); return;
L_9A7D:;
    mff_cycles(0x9A7D, 12);
    t = (uint8_t)(RAM(0x10) + 1); RAM(0x10) = t;
    a = RAM(0x10);
    m = 0x03; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x9A19, 1); goto L_9A19; }
    mff_cycles(0x9A85, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1A87, 0x9B16, 0)) return;
    SYNC_IN;
    mff_cycles(0x9A88, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1A8A, 0x9AC0, 0)) return;
    SYNC_IN;
    mff_cycles(0x9A8B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9A13, 2); call_by_address_tail(0x9A13, 0); return;
L_9A8E:;
    mff_cycles(0x9A8E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9B16, 2); call_by_address_tail(0x9B16, 0); return;
L_9A91:;
    mff_cycles(0x9A91, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9A13 (bank 0) - lifted by port/tools/lift.py */
void func_9A13_b0(void) {
    REGS_IN;
L_9A13:;
    mff_cycles(0x9A13, 8);
    a = 0x00;
    RAM(0x10) = a;
    RAM(0x12) = a;
L_9A19:;
    mff_cycles(0x9A19, 12);
    a = 0x00;
    RAM(0x11) = a;
    a = RAM(JOY_PRESSED);
    a &= 0xF0;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x9A21, 1); goto L_9A91; }
    mff_cycles(0x9A23, 7);
    x = RAM(0x10);
    a = nes_read((uint16_t)(0x9A92 + x));
    mff_cycles(0x9A28, 8);
    RAM(0x0780) = a;
    a = nes_read((uint16_t)(0x9A95 + x));
    mff_cycles(0x9A2E, 28);
    c = 0;
    m = RAM(0x11); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(0x0781) = a;
    a = 0x00;
    RAM(0x0782) = a;
    y = RAM(0x0E);
    t = (uint8_t)(RAM(0x0E) + 1); RAM(0x0E) = t;
    a = nes_read((uint16_t)(RAM16(0x0C) + y));
    mff_cycles(0x9A3F, 4);
    m = 0xFF; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9A41, 1); goto L_9A8E; }
    mff_cycles(0x9A43, 4);
    m = 0xFE; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9A45, 1); goto L_9A7D; }
    mff_cycles(0x9A47, 31);
    RAM(0x0783) = a;
    a = 0xFF;
    RAM(0x0784) = a;
    RAM(VRAM_LIST_REQUEST) = a;
    a = RAM(0x10);
    mff_push(a);
    a = RAM(0x11);
    n = (a) >> 7;
    z = (a) == 0;
    mff_push(a);
    SYNC_OUT;
    if (!mff_call_at(0x1A59, 0x9A98, 0)) return;
    SYNC_IN;
    mff_cycles(0x9A5A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1A5C, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9A5D, 6);
    SYNC_OUT;
    mff_call_fn(0x1A5F, func_FF31, 0xE000);
    SYNC_IN;
    mff_cycles(0x9A60, 6);
    SYNC_OUT;
    mff_call_fn(0x1A62, func_FF31, 0xE000);
    SYNC_IN;
    mff_cycles(0x9A63, 26);
    a = mff_pull();
    RAM(0x11) = a;
    a = mff_pull();
    RAM(0x10) = a;
    t = (uint8_t)(RAM(0x11) + 1); RAM(0x11) = t;
    a = RAM(FRAME_COUNTER);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x9A6F, 1); goto L_9A7A; }
    mff_cycles(0x9A71, 5);
    a = RAM(INTRO_PAGE);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x9A73, 1); goto L_9A7A; }
    mff_cycles(0x9A75, 8);
    a = 0x2B;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1A79, func_F34C, 0xE000)) return;
    SYNC_IN;
L_9A7A:;
    mff_cycles(0x9A7A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9A1D, 2); call_by_address_tail(0x9A1D, 0); return;
L_9A7D:;
    mff_cycles(0x9A7D, 12);
    t = (uint8_t)(RAM(0x10) + 1); RAM(0x10) = t;
    a = RAM(0x10);
    m = 0x03; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x9A19, 1); goto L_9A19; }
    mff_cycles(0x9A85, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1A87, 0x9B16, 0)) return;
    SYNC_IN;
    mff_cycles(0x9A88, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1A8A, 0x9AC0, 0)) return;
    SYNC_IN;
    mff_cycles(0x9A8B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9A13, 2); call_by_address_tail(0x9A13, 0); return;
L_9A8E:;
    mff_cycles(0x9A8E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9B16, 2); call_by_address_tail(0x9B16, 0); return;
L_9A91:;
    mff_cycles(0x9A91, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9A1D (bank 0) - lifted by port/tools/lift.py */
void func_9A1D_b0(void) {
    REGS_IN;
    goto L_9A1D;
L_9A19:;
    mff_cycles(0x9A19, 5);
    a = 0x00;
    RAM(0x11) = a;
L_9A1D:;
    mff_cycles(0x9A1D, 7);
    a = RAM(JOY_PRESSED);
    a &= 0xF0;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x9A21, 1); goto L_9A91; }
    mff_cycles(0x9A23, 7);
    x = RAM(0x10);
    a = nes_read((uint16_t)(0x9A92 + x));
    mff_cycles(0x9A28, 8);
    RAM(0x0780) = a;
    a = nes_read((uint16_t)(0x9A95 + x));
    mff_cycles(0x9A2E, 28);
    c = 0;
    m = RAM(0x11); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(0x0781) = a;
    a = 0x00;
    RAM(0x0782) = a;
    y = RAM(0x0E);
    t = (uint8_t)(RAM(0x0E) + 1); RAM(0x0E) = t;
    a = nes_read((uint16_t)(RAM16(0x0C) + y));
    mff_cycles(0x9A3F, 4);
    m = 0xFF; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9A41, 1); goto L_9A8E; }
    mff_cycles(0x9A43, 4);
    m = 0xFE; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9A45, 1); goto L_9A7D; }
    mff_cycles(0x9A47, 31);
    RAM(0x0783) = a;
    a = 0xFF;
    RAM(0x0784) = a;
    RAM(VRAM_LIST_REQUEST) = a;
    a = RAM(0x10);
    mff_push(a);
    a = RAM(0x11);
    n = (a) >> 7;
    z = (a) == 0;
    mff_push(a);
    SYNC_OUT;
    if (!mff_call_at(0x1A59, 0x9A98, 0)) return;
    SYNC_IN;
    mff_cycles(0x9A5A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1A5C, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0x9A5D, 6);
    SYNC_OUT;
    mff_call_fn(0x1A5F, func_FF31, 0xE000);
    SYNC_IN;
    mff_cycles(0x9A60, 6);
    SYNC_OUT;
    mff_call_fn(0x1A62, func_FF31, 0xE000);
    SYNC_IN;
    mff_cycles(0x9A63, 26);
    a = mff_pull();
    RAM(0x11) = a;
    a = mff_pull();
    RAM(0x10) = a;
    t = (uint8_t)(RAM(0x11) + 1); RAM(0x11) = t;
    a = RAM(FRAME_COUNTER);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x9A6F, 1); goto L_9A7A; }
    mff_cycles(0x9A71, 5);
    a = RAM(INTRO_PAGE);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x9A73, 1); goto L_9A7A; }
    mff_cycles(0x9A75, 8);
    a = 0x2B;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1A79, func_F34C, 0xE000)) return;
    SYNC_IN;
L_9A7A:;
    mff_cycles(0x9A7A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9A1D, 2); call_by_address_tail(0x9A1D, 0); return;
L_9A7D:;
    mff_cycles(0x9A7D, 12);
    t = (uint8_t)(RAM(0x10) + 1); RAM(0x10) = t;
    a = RAM(0x10);
    m = 0x03; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x9A19, 1); goto L_9A19; }
    mff_cycles(0x9A85, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1A87, 0x9B16, 0)) return;
    SYNC_IN;
    mff_cycles(0x9A88, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1A8A, 0x9AC0, 0)) return;
    SYNC_IN;
    mff_cycles(0x9A8B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9A13, 2); call_by_address_tail(0x9A13, 0); return;
L_9A8E:;
    mff_cycles(0x9A8E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x9B16, 2); call_by_address_tail(0x9B16, 0); return;
L_9A91:;
    mff_cycles(0x9A91, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9A98 (bank 0) - lifted by port/tools/lift.py */
void func_9A98_b0(void) {
    REGS_IN;
L_9A98:;
    mff_cycles(0x9A98, 7);
    a = RAM(INTRO_PAGE);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9A9C, 1); goto L_9AA7; }
    mff_cycles(0x9A9E, 4);
    m = 0x05; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9AA0, 1); goto L_9AAE; }
    mff_cycles(0x9AA2, 4);
    m = 0x06; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9AA4, 1); goto L_9AB5; }
    mff_cycles(0x9AA6, 6);
    SYNC_OUT;
    mff_rts(); return;
L_9AA7:;
    mff_cycles(0x9AA7, 8);
    a = 0x02;
    z = (a) == 0;
    RAM(OBJ_ANIM_FRAME) = a;
    if (!z) { mff_cycles(0x9AAC, 1); goto L_9ABA; }
L_9AAE:;
    mff_cycles(0x9AAE, 8);
    a = 0x01;
    z = (a) == 0;
    RAM(OBJ_ANIM_FRAME) = a;
    if (!z) { mff_cycles(0x9AB3, 1); goto L_9ABA; }
L_9AB5:;
    mff_cycles(0x9AB5, 6);
    a = 0x00;
    RAM(OBJ_ANIM_FRAME) = a;
L_9ABA:;
    mff_cycles(0x9ABA, 12);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ANIM_TIMER) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $9AC0 (bank 0) - lifted by port/tools/lift.py */
void func_9AC0_b0(void) {
    REGS_IN;
L_9AC0:;
    mff_cycles(0x9AC0, 76);
    a = 0x23;
    RAM(0x0780) = a;
    a = 0x79;
    RAM(0x0781) = a;
    a = 0x00;
    RAM(0x0782) = a;
    a = 0x00;
    RAM(0x0783) = a;
    a = 0x23;
    RAM(0x0784) = a;
    a = 0x22;
    RAM(0x07B0) = a;
    RAM(0x079A) = a;
    a = 0x27;
    RAM(0x0785) = a;
    a = 0xE7;
    RAM(0x079B) = a;
    a = 0xA7;
    RAM(0x07B1) = a;
    x = 0x12;
    RAM(0x0786) = x;
    RAM(0x079C) = x;
    RAM(0x07B2) = x;
    y = 0x00;
    a = 0x00;
L_9AFF:;
    mff_cycles(0x9AFF, 21);
    RAM(0x0787 + y) = a;
    RAM(0x079D + y) = a;
    RAM(0x07B3 + y) = a;
    y++;
    x--;
    n = (x) >> 7;
    if (!n) { mff_cycles(0x9AFF, 2); goto L_9AFF; }
    mff_cycles(0x9B0C, 12);
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(0x07C6) = a;
    RAM(VRAM_LIST_REQUEST) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xFF31, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_FF31(); g_code_window_base = swb; } return;
}

/*  $9B16 (bank 0) - lifted by port/tools/lift.py */
void func_9B16_b0(void) {
    REGS_IN;
L_9B16:;
    mff_cycles(0x9B16, 7);
    a = RAM(INTRO_PAGE);
    m = 0x05; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9B1A, 1); goto L_9B26; }
    mff_cycles(0x9B1C, 4);
    m = 0x06; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0x9B1E, 1); goto L_9B26; }
    mff_cycles(0x9B20, 8);
    x = 0xB7;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    mff_call_fn(0x1B24, func_FF2A, 0xE000);
    SYNC_IN;
    mff_cycles(0x9B25, 6);
    SYNC_OUT;
    mff_rts(); return;
L_9B26:;
    mff_cycles(0x9B26, 8);
    x = 0x60;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    mff_call_fn(0x1B2A, func_FF2A, 0xE000);
    SYNC_IN;
    mff_cycles(0x9B2B, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A000 (bank 0) - lifted by port/tools/lift.py */
void func_A000_b0(void) {
    REGS_IN;
L_A000:;
    mff_cycles(0xA000, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA0FD, 2); call_by_address_tail(0xA0FD, 0); return;
}

/*  $A003 (bank 0) - lifted by port/tools/lift.py */
void func_A003_b0(void) {
    REGS_IN;
L_A003:;
    mff_cycles(0xA003, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA101, 2); call_by_address_tail(0xA101, 0); return;
}

/*  $A006 (bank 0) - lifted by port/tools/lift.py */
void func_A006_b0(void) {
    REGS_IN;
L_A006:;
    mff_cycles(0xA006, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A009 (bank 0) - lifted by port/tools/lift.py */
void func_A009_b0(void) {
    REGS_IN;
L_A009:;
    mff_cycles(0xA009, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAA33, 2); call_by_address_tail(0xAA33, 0); return;
}

/*  $A00C (bank 0) - lifted by port/tools/lift.py */
void func_A00C_b0(void) {
    REGS_IN;
L_A00C:;
    mff_cycles(0xA00C, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A00F (bank 0) - lifted by port/tools/lift.py */
void func_A00F_b0(void) {
    REGS_IN;
L_A00F:;
    mff_cycles(0xA00F, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A012 (bank 0) - lifted by port/tools/lift.py */
void func_A012_b0(void) {
    REGS_IN;
L_A012:;
    mff_cycles(0xA012, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A015 (bank 0) - lifted by port/tools/lift.py */
void func_A015_b0(void) {
    REGS_IN;
L_A015:;
    mff_cycles(0xA015, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A018 (bank 0) - lifted by port/tools/lift.py */
void func_A018_b0(void) {
    REGS_IN;
L_A018:;
    mff_cycles(0xA018, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A01B (bank 0) - lifted by port/tools/lift.py */
void func_A01B_b0(void) {
    REGS_IN;
L_A01B:;
    mff_cycles(0xA01B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A01E (bank 0) - lifted by port/tools/lift.py */
void func_A01E_b0(void) {
    REGS_IN;
L_A01E:;
    mff_cycles(0xA01E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A021 (bank 0) - lifted by port/tools/lift.py */
void func_A021_b0(void) {
    REGS_IN;
L_A021:;
    mff_cycles(0xA021, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A024 (bank 0) - lifted by port/tools/lift.py */
void func_A024_b0(void) {
    REGS_IN;
L_A024:;
    mff_cycles(0xA024, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A027 (bank 0) - lifted by port/tools/lift.py */
void func_A027_b0(void) {
    REGS_IN;
L_A027:;
    mff_cycles(0xA027, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A02A (bank 0) - lifted by port/tools/lift.py */
void func_A02A_b0(void) {
    REGS_IN;
L_A02A:;
    mff_cycles(0xA02A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA030, 2); call_by_address_tail(0xA030, 0); return;
}

/*  $A02D (bank 0) - lifted by port/tools/lift.py */
void func_A02D_b0(void) {
    REGS_IN;
L_A02D:;
    mff_cycles(0xA02D, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA09B, 2); call_by_address_tail(0xA09B, 0); return;
}

/*  $A030 (bank 0) - lifted by port/tools/lift.py */
void func_A030_b0(void) {
    REGS_IN;
L_A030:;
    mff_cycles(0xA030, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A031 (bank 0) - lifted by port/tools/lift.py */
void func_A031_b0(void) {
    REGS_IN;
L_A031:;
    mff_cycles(0xA031, 9);
    RAM(0x00) = y;
    a = RAM(OBJ_TIER + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xA036, 1); goto L_A03A; }
    mff_cycles(0xA038, 2);
    a = 0x01;
L_A03A:;
    mff_cycles(0xA03A, 15);
    y = a;
    a = 0x00;
    RAM(0x0F) = a;
    a = RAM(OBJ_DIR + x);
    a &= 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xA044, 1); goto L_A05D; }
    mff_cycles(0xA046, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0048, func_F1DF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA049, 8);
    a = RAM(OBJ_Y + x);
    m = nes_read((uint16_t)(0xA08E + y)); c = a >= m;
    mff_cycles(0xA04F, 2);
    if (c) { mff_cycles(0xA04F, 1); goto L_A077; }
    mff_cycles(0xA051, 4);
    a = nes_read((uint16_t)(0xA08E + y));
    mff_cycles(0xA054, 12);
    RAM(OBJ_Y + x) = a;
    a = 0x08;
    z = (a) == 0;
    RAM(0x0F) = a;
    if (!z) { mff_cycles(0xA05B, 1); goto L_A077; }
L_A05D:;
    mff_cycles(0xA05D, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xA062, 1); goto L_A077; }
    mff_cycles(0xA064, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0066, func_F1BF, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA067, 8);
    a = RAM(OBJ_Y + x);
    m = 0xAF; c = a >= m;
    if (!c) { mff_cycles(0xA06C, 1); goto L_A077; }
    mff_cycles(0xA06E, 12);
    a = 0xAE;
    RAM(OBJ_Y + x) = a;
    a = 0x04;
    RAM(0x0F) = a;
L_A077:;
    mff_cycles(0xA077, 11);
    y = RAM(0x00);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xA07E, 1); goto L_A083; }
    mff_cycles(0xA080, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF183, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F183(); g_code_window_base = swb; } return;
L_A083:;
    mff_cycles(0xA083, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xA088, 1); goto L_A08D; }
    mff_cycles(0xA08A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF1A1, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F1A1(); g_code_window_base = swb; } return;
L_A08D:;
    mff_cycles(0xA08D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A090 (bank 0) - lifted by port/tools/lift.py */
void func_A090_b0(void) {
    REGS_IN;
L_A090:;
    mff_cycles(0xA090, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0092, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA093, 2);
    if (c) { mff_cycles(0xA093, 1); goto L_A098; }
    mff_cycles(0xA095, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF099, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F099(); g_code_window_base = swb; } return;
L_A098:;
    mff_cycles(0xA098, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF090, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F090(); g_code_window_base = swb; } return;
}

/*  $A09B (bank 0) - lifted by port/tools/lift.py */
void func_A09B_b0(void) {
    REGS_IN;
L_A09B:;
    mff_cycles(0xA09B, 8);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA0A0, 1); goto L_A0C7; }
    mff_cycles(0xA0A2, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x00A4, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA0A5, 4);
    m = 0x10; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xA0A7, 1); goto L_A0C6; }
    mff_cycles(0xA0A9, 16);
    a = 0x01;
    RAM(PLAYER_DIR) = a;
    a = 0x0D;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ATTACK_ID) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x00B3, func_F455, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA0B4, 8);
    a = 0x18;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x00B8, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA0B9, 15);
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1); RAM(OBJ_STATE + x) = t;
    a = RAM(OBJ_X);
    m = 0x70; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xA0C1, 1); goto L_A0C6; }
    mff_cycles(0xA0C3, 6);
    t = (uint8_t)(RAM(OBJ_DOWN_TIMER) + 1); RAM(OBJ_DOWN_TIMER) = t;
    n = (t) >> 7;
    z = (t) == 0;
L_A0C6:;
    mff_cycles(0xA0C6, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A0C7:;
    mff_cycles(0xA0C7, 8);
    a = RAM(OBJ_X);
    m = 0x70; c = a >= m;
    if (!c) { mff_cycles(0xA0CC, 1); goto L_A0D3; }
    mff_cycles(0xA0CE, 6);
    a = 0x02;
    RAM(OBJ_DOWN_TIMER) = a;
L_A0D3:;
    mff_cycles(0xA0D3, 10);
    a = 0xFF;
    y = 0x31;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x00D9, func_F443, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA0DA, 22);
    a = 0x90;
    RAM(OBJ_FLAGS + x) = a;
    a = RAM(OBJ_X + x);
    c = 0;
    m = 0x03; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(OBJ_X + x) = a;
    if (c) { mff_cycles(0xA0E8, 1); goto L_A0F5; }
    mff_cycles(0xA0EA, 13);
    a = RAM(OBJ_Y + x);
    m = 0x02; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y + x) = a;
    if (!c) { mff_cycles(0xA0F2, 1); goto L_A0F5; }
    mff_cycles(0xA0F4, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A0F5:;
    mff_cycles(0xA0F5, 9);
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DOWN_TIMER) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF2BE, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F2BE(); g_code_window_base = swb; } return;
}

/*  $A0FD (bank 0) - lifted by port/tools/lift.py */
void func_A0FD_b0(void) {
    REGS_IN;
L_A0FD:;
    mff_cycles(0xA0FD, 4);
    a = 0x07;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA0FF, 1); goto L_A103; }
    mff_cycles(0xA101, 2);
    a = 0x08;
L_A103:;
    mff_cycles(0xA103, 23);
    RAM(OBJ_KIND + x) = a;
    a = 0x00;
    RAM(OBJ_TIER + x) = a;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_INVULN + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xA111, 1); goto L_A116; }
    mff_cycles(0xA113, 7);
    t = (uint8_t)(RAM(OBJ_INVULN + x) - 1); RAM(OBJ_INVULN + x) = t;
L_A116:;
    mff_cycles(0xA116, 8);
    a = RAM(OBJ_ID + 13);
    m = 0x6E; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA11B, 1); goto L_A123; }
    mff_cycles(0xA11D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x011F, 0xA6D6, 0)) return;
    SYNC_IN;
    mff_cycles(0xA120, 2);
    if (!c) { mff_cycles(0xA120, 1); goto L_A129; }
    mff_cycles(0xA122, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A123:;
    mff_cycles(0xA123, 13);
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_INVULN + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_A129:;
    mff_cycles(0xA129, 12);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    y = a;
    a = nes_read((uint16_t)(0xA6C4 + y));
    mff_cycles(0xA132, 7);
    RAM(0x00) = a;
    a = nes_read((uint16_t)(0xA6CD + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA137, 8);
    RAM(0x01) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $A101 (bank 0) - lifted by port/tools/lift.py */
void func_A101_b0(void) {
    REGS_IN;
L_A101:;
    mff_cycles(0xA101, 25);
    a = 0x08;
    RAM(OBJ_KIND + x) = a;
    a = 0x00;
    RAM(OBJ_TIER + x) = a;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_INVULN + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xA111, 1); goto L_A116; }
    mff_cycles(0xA113, 7);
    t = (uint8_t)(RAM(OBJ_INVULN + x) - 1); RAM(OBJ_INVULN + x) = t;
L_A116:;
    mff_cycles(0xA116, 8);
    a = RAM(OBJ_ID + 13);
    m = 0x6E; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA11B, 1); goto L_A123; }
    mff_cycles(0xA11D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x011F, 0xA6D6, 0)) return;
    SYNC_IN;
    mff_cycles(0xA120, 2);
    if (!c) { mff_cycles(0xA120, 1); goto L_A129; }
    mff_cycles(0xA122, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A123:;
    mff_cycles(0xA123, 13);
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_INVULN + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_A129:;
    mff_cycles(0xA129, 12);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    y = a;
    a = nes_read((uint16_t)(0xA6C4 + y));
    mff_cycles(0xA132, 7);
    RAM(0x00) = a;
    a = nes_read((uint16_t)(0xA6CD + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA137, 8);
    RAM(0x01) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $A13C (bank 0) - lifted by port/tools/lift.py */
void func_A13C_b0(void) {
    REGS_IN;
L_A13C:;
    mff_cycles(0xA13C, 6);
    SYNC_OUT;
    if (!mff_call_at(0x013E, 0xA090, 0)) return;
    SYNC_IN;
    mff_cycles(0xA13F, 8);
    a = RAM(OBJ_X_HI + x);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA144, 1); goto L_A15B; }
    mff_cycles(0xA146, 13);
    a = 0x01;
    RAM(OBJ_DIR + x) = a;
    a = RAM(OBJ_X_HI + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xA14E, 1); goto L_A155; }
    mff_cycles(0xA150, 7);
    a = 0x02;
    RAM(OBJ_DIR + x) = a;
L_A155:;
    mff_cycles(0xA155, 13);
    a = 0x88;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_A15B:;
    mff_cycles(0xA15B, 27);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x05;
    RAM(OBJ_DOWN_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1); RAM(OBJ_STATE + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A169 (bank 0) - lifted by port/tools/lift.py */
void func_A169_b0(void) {
    REGS_IN;
L_A169:;
    mff_cycles(0xA169, 7);
    a = RAM(GAME_ACTION);
    m = 0x1F; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA16D, 1); goto L_A19A; }
    mff_cycles(0xA16F, 4);
    m = 0x06; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA171, 1); goto L_A19A; }
    mff_cycles(0xA173, 8);
    a = RAM(OBJ_KIND + x);
    m = 0x07; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA178, 1); goto L_A180; }
    mff_cycles(0xA17A, 6);
    a = 0x1C;
    y = 0x0A;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0xA17E, 1); goto L_A184; }
L_A180:;
    mff_cycles(0xA180, 4);
    a = 0x24;
    y = 0x09;
    n = (y) >> 7;
    z = (y) == 0;
L_A184:;
    mff_cycles(0xA184, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0186, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA187, 2);
    if (!c) { mff_cycles(0xA187, 1); goto L_A19A; }
    mff_cycles(0xA189, 31);
    a = RAM(OBJ_KIND + x);
    m = 0x07; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    RAM(OBJ_TIMER2 + x) = a;
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1); RAM(OBJ_STATE + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_A19A:;
    mff_cycles(0xA19A, 6);
    SYNC_OUT;
    if (!mff_call_at(0x019C, 0xA882, 0)) return;
    SYNC_IN;
    mff_cycles(0xA19D, 6);
    a = RAM(OBJ_TIMER + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xA1A0, 1); goto L_A1BC; }
    mff_cycles(0xA1A2, 23);
    a = RAM(RNG);
    a &= 0x01;
    RAM(OBJ_AI_FLAG + x) = a;
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_LAND_Y + x) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x01B0, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA1B1, 2);
    if (!c) { mff_cycles(0xA1B1, 1); goto L_A1B8; }
    mff_cycles(0xA1B3, 7);
    a = 0x0C;
    RAM(OBJ_LAND_Y + x) = a;
L_A1B8:;
    mff_cycles(0xA1B8, 13);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_A1BC:;
    mff_cycles(0xA1BC, 8);
    a = RAM(OBJ_X_HI + x);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA1C1, 1); goto L_A1D5; }
    mff_cycles(0xA1C3, 4);
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA1C5, 1); goto L_A1CB; }
    mff_cycles(0xA1C7, 4);
    a = 0x01;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA1C9, 1); goto L_A1CD; }
L_A1CB:;
    mff_cycles(0xA1CB, 2);
    a = 0xFE;
L_A1CD:;
    mff_cycles(0xA1CD, 12);
    RAM(OBJ_X + x) = a;
    a = 0x01;
    RAM(OBJ_X_HI + x) = a;
L_A1D5:;
    mff_cycles(0xA1D5, 8);
    a = RAM(OBJ_LAND_Y + x);
    m = 0x04; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA1DA, 1); goto L_A1E5; }
    mff_cycles(0xA1DC, 8);
    a = RAM(OBJ_X + x);
    m = 0xF1; c = a >= m;
    if (c) { mff_cycles(0xA1E1, 2); goto L_A20C; }
    mff_cycles(0xA1E3, 2);
    if (!c) { mff_cycles(0xA1E3, 2); goto L_A231; }
L_A1E5:;
    mff_cycles(0xA1E5, 4);
    m = 0x0C; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA1E7, 1); goto L_A1F2; }
    mff_cycles(0xA1E9, 8);
    a = RAM(OBJ_X + x);
    m = 0x18; c = a >= m;
    if (!c) { mff_cycles(0xA1EE, 2); goto L_A20C; }
    mff_cycles(0xA1F0, 2);
    if (c) { mff_cycles(0xA1F0, 2); goto L_A231; }
L_A1F2:;
    mff_cycles(0xA1F2, 4);
    m = 0x00; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA1F4, 1); goto L_A1FF; }
    mff_cycles(0xA1F6, 8);
    a = RAM(OBJ_Y + x);
    m = 0x86; c = a >= m;
    if (!c) { mff_cycles(0xA1FB, 2); goto L_A20C; }
    mff_cycles(0xA1FD, 2);
    if (c) { mff_cycles(0xA1FD, 2); goto L_A231; }
L_A1FF:;
    mff_cycles(0xA1FF, 4);
    m = 0x08; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA201, 1); goto L_A231; }
    mff_cycles(0xA203, 8);
    a = RAM(OBJ_Y + x);
    m = 0xAE; c = a >= m;
    if (c) { mff_cycles(0xA208, 1); goto L_A20C; }
    mff_cycles(0xA20A, 2);
    if (!c) { mff_cycles(0xA20A, 1); goto L_A231; }
L_A20C:;
    mff_cycles(0xA20C, 6);
    a = RAM(OBJ_AI_FLAG + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xA20F, 1); goto L_A21F; }
    mff_cycles(0xA211, 18);
    a = RAM(OBJ_LAND_Y + x);
    c = 0;
    m = 0x04; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_LAND_Y + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA22A, 2); call_by_address_tail(0xA22A, 0); return;
L_A21F:;
    mff_cycles(0xA21F, 23);
    a = RAM(OBJ_LAND_Y + x);
    c = 1;
    m = 0x04; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    a &= 0x0F;
    RAM(OBJ_LAND_Y + x) = a;
    y = a;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x022D, 0xA7E1, 0)) return;
    SYNC_IN;
    mff_cycles(0xA22E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA28B, 2); call_by_address_tail(0xA28B, 0); return;
L_A231:;
    mff_cycles(0xA231, 10);
    y = RAM(OBJ_LAND_Y + x);
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0236, 0xA7E1, 0)) return;
    SYNC_IN;
    mff_cycles(0xA237, 4);
    a = nes_read((uint16_t)(0xFBF0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA23A, 11);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x023F, 0xA031, 0)) return;
    SYNC_IN;
    mff_cycles(0xA240, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0242, 0xA090, 0)) return;
    SYNC_IN;
    mff_cycles(0xA243, 8);
    a = RAM(OBJ_KIND + x);
    m = 0x07; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA248, 1); goto L_A26E; }
    mff_cycles(0xA24A, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x0C;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xA24F, 1); goto L_A258; }
    mff_cycles(0xA251, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0253, func_FBD9, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA254, 4);
    m = 0x02; c = a >= m;
    if (!c) { mff_cycles(0xA256, 1); goto L_A26F; }
L_A258:;
    mff_cycles(0xA258, 8);
    a = RAM(OBJ_Y + x);
    m = 0x90; c = a >= m;
    if (!c) { mff_cycles(0xA25D, 1); goto L_A263; }
    mff_cycles(0xA25F, 4);
    m = 0x98; c = a >= m;
    if (!c) { mff_cycles(0xA261, 1); goto L_A2CB; }
L_A263:;
    mff_cycles(0xA263, 8);
    a = RAM(OBJ_X + x);
    m = 0x7C; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xA268, 1); goto L_A26E; }
    mff_cycles(0xA26A, 4);
    m = 0x84; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xA26C, 1); goto L_A2CB; }
L_A26E:;
    mff_cycles(0xA26E, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A26F:;
    mff_cycles(0xA26F, 8);
    a = RAM(OBJ_DOWN_TIMER + x);
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA274, 1); goto L_A28A; }
    mff_cycles(0xA276, 16);
    a = 0x03;
    RAM(OBJ_DOWN_TIMER + x) = a;
    a = RAM(RNG);
    a &= 0x0F;
    m = 0x0C; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xA281, 1); goto L_A28A; }
    mff_cycles(0xA283, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0285, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA286, 4);
    a = 0x86;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA288, 1); goto L_A2E4; }
L_A28A:;
    mff_cycles(0xA28A, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A2CB:;
    mff_cycles(0xA2CB, 8);
    a = RAM(OBJ_DOWN_TIMER + x);
    a &= 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA2D0, 1); goto L_A2E3; }
    mff_cycles(0xA2D2, 16);
    a = 0x05;
    RAM(OBJ_DOWN_TIMER + x) = a;
    a = RAM(RNG);
    a &= 0x0F;
    m = 0x0A; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xA2DD, 1); goto L_A2E3; }
    mff_cycles(0xA2DF, 4);
    a = 0x83;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA2E1, 1); goto L_A2E4; }
L_A2E3:;
    mff_cycles(0xA2E3, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A2E4:;
    mff_cycles(0xA2E4, 18);
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A22A (bank 0) - lifted by port/tools/lift.py */
void func_A22A_b0(void) {
    REGS_IN;
L_A22A:;
    mff_cycles(0xA22A, 8);
    y = a;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x022D, 0xA7E1, 0)) return;
    SYNC_IN;
    mff_cycles(0xA22E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA28B, 2); call_by_address_tail(0xA28B, 0); return;
}

/*  $A28B (bank 0) - lifted by port/tools/lift.py */
void func_A28B_b0(void) {
    REGS_IN;
L_A28B:;
    mff_cycles(0xA28B, 8);
    a = RAM(OBJ_DOWN_TIMER + x);
    t = a; c = t & 1; t >>= 1; a = t;
    if (c) { mff_cycles(0xA28F, 1); goto L_A2AA; }
    mff_cycles(0xA291, 8);
    a = RAM(OBJ_KIND + x);
    m = 0x08; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA296, 1); goto L_A2B3; }
    mff_cycles(0xA298, 16);
    a = 0x00;
    RAM(OBJ_DOWN_TIMER + x) = a;
    a = RAM(RNG);
    a &= 0x0F;
    m = 0x08; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xA2A3, 1); goto L_A2A9; }
    mff_cycles(0xA2A5, 4);
    a = 0x85;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA2A7, 1); goto L_A2E4; }
L_A2A9:;
    mff_cycles(0xA2A9, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A2AA:;
    mff_cycles(0xA2AA, 17);
    a = RAM(OBJ_DOWN_TIMER + x);
    a &= 0x06;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DOWN_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_A2B3:;
    mff_cycles(0xA2B3, 16);
    a = 0x00;
    RAM(OBJ_DOWN_TIMER + x) = a;
    a = RAM(RNG);
    a &= 0x0F;
    m = 0x08; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xA2BE, 1); goto L_A2CA; }
    mff_cycles(0xA2C0, 7);
    a = RAM(GAME_ACTION);
    m = 0x06; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA2C4, 1); goto L_A2CA; }
    mff_cycles(0xA2C6, 4);
    a = 0x84;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA2C8, 1); goto L_A2E4; }
L_A2CA:;
    mff_cycles(0xA2CA, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A2E4:;
    mff_cycles(0xA2E4, 18);
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A2ED (bank 0) - lifted by port/tools/lift.py */
void func_A2ED_b0(void) {
    REGS_IN;
L_A2ED:;
    mff_cycles(0xA2ED, 6);
    a = RAM(OBJ_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA2F0, 2); goto L_A31F; }
    mff_cycles(0xA2F2, 11);
    RAM(OBJ_TIMER3 + x) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x02F7, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA2F8, 6);
    a = RAM(OBJ_TIMER2 + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xA2FB, 2); goto L_A303; }
    mff_cycles(0xA2FD, 6);
    a = 0x03;
    y = 0x02;
    z = (y) == 0;
    if (!z) { mff_cycles(0xA301, 1); goto L_A307; }
L_A303:;
    mff_cycles(0xA303, 4);
    a = 0x01;
    y = 0x00;
L_A307:;
    mff_cycles(0xA307, 12);
    RAM(OBJ_AI_COUNT + x) = a;
    a = RAM(GAME_ACTION);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA30E, 1); goto L_A315; }
    mff_cycles(0xA310, 8);
    a = y;
    c = 0;
    m = 0x0A; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    y = a;
L_A315:;
    mff_cycles(0xA315, 17);
    a = y;
    RAM(OBJ_AI_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA83A, 2); call_by_address_tail(0xA83A, 0); return;
L_A31F:;
    mff_cycles(0xA31F, 6);
    a = RAM(OBJ_AI_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xA322, 1); goto L_A330; }
    mff_cycles(0xA324, 25);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1); RAM(OBJ_AI_TIMER + x) = t;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_ANIM_TIMER + x) = a;
    RAM(OBJ_ANIM_FRAME + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_A330:;
    mff_cycles(0xA330, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0332, 0xA8B7, 0)) return;
    SYNC_IN;
    mff_cycles(0xA333, 6);
    a = RAM(OBJ_TIMER2 + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xA336, 1); goto L_A350; }
    mff_cycles(0xA338, 10);
    a = RAM(OBJ_ANIM_TIMER + x);
    a |= RAM(OBJ_ANIM_FRAME + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA33E, 1); goto L_A343; }
    mff_cycles(0xA340, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA3EA, 2); call_by_address_tail(0xA3EA, 0); return;
L_A343:;
    mff_cycles(0xA343, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x04; c = a >= m;
    if (!c) { mff_cycles(0xA348, 1); goto L_A379; }
    mff_cycles(0xA34A, 4);
    m = 0x09; c = a >= m;
    if (!c) { mff_cycles(0xA34C, 1); goto L_A36E; }
    mff_cycles(0xA34E, 2);
    if (c) { mff_cycles(0xA34E, 1); goto L_A3C5; }
L_A350:;
    mff_cycles(0xA350, 6);
    a = RAM(OBJ_ANIM_TIMER + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xA353, 1); goto L_A35F; }
    mff_cycles(0xA355, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x09; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA35A, 1); goto L_A35F; }
    mff_cycles(0xA35C, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA3EA, 2); call_by_address_tail(0xA3EA, 0); return;
L_A35F:;
    mff_cycles(0xA35F, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x02; c = a >= m;
    if (!c) { mff_cycles(0xA364, 1); goto L_A379; }
    mff_cycles(0xA366, 4);
    m = 0x05; c = a >= m;
    if (!c) { mff_cycles(0xA368, 1); goto L_A3C5; }
    mff_cycles(0xA36A, 4);
    m = 0x07; c = a >= m;
    if (c) { mff_cycles(0xA36C, 1); goto L_A3C5; }
L_A36E:;
    mff_cycles(0xA36E, 8);
    y = RAM(OBJ_TIMER2 + x);
    a = nes_read((uint16_t)(0xA413 + y));
    z = (a) == 0;
    mff_cycles(0xA374, 7);
    RAM(OBJ_ATTACK + x) = a;
    if (!z) { mff_cycles(0xA377, 1); goto L_A382; }
L_A379:;
    mff_cycles(0xA379, 8);
    y = RAM(OBJ_TIMER2 + x);
    a = nes_read((uint16_t)(0xA411 + y));
    mff_cycles(0xA37F, 5);
    RAM(OBJ_ATTACK + x) = a;
L_A382:;
    mff_cycles(0xA382, 6);
    a = RAM(OBJ_TIMER2 + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xA385, 1); goto L_A399; }
    mff_cycles(0xA387, 8);
    a = RAM(OBJ_AI_COUNT + x);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA38C, 1); goto L_A395; }
    mff_cycles(0xA38E, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x05; c = a >= m;
    if (c) { mff_cycles(0xA393, 1); goto L_A399; }
L_A395:;
    mff_cycles(0xA395, 4);
    a = 0x1E;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA397, 1); goto L_A3AF; }
L_A399:;
    mff_cycles(0xA399, 7);
    a = RAM(GAME_ACTION);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA39D, 1); goto L_A3A9; }
    mff_cycles(0xA39F, 7);
    a = RAM(ROUND);
    m = 0x04; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA3A3, 1); goto L_A3A9; }
    mff_cycles(0xA3A5, 4);
    a = 0x05;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA3A7, 1); goto L_A3AF; }
L_A3A9:;
    mff_cycles(0xA3A9, 8);
    y = RAM(OBJ_TIMER2 + x);
    a = nes_read((uint16_t)(0xA415 + y));
L_A3AF:;
    mff_cycles(0xA3AF, 11);
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = RAM(OBJ_HIT_PLAYER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xA3B5, 1); goto L_A3C4; }
    mff_cycles(0xA3B7, 13);
    a = 0x18;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER3 + x) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x03BE, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA3BF, 7);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_HIT_PLAYER + x) = a;
L_A3C4:;
    mff_cycles(0xA3C4, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A3C5:;
    mff_cycles(0xA3C5, 6);
    a = RAM(OBJ_TIMER3 + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xA3C8, 1); goto L_A3E9; }
    mff_cycles(0xA3CA, 7);
    a = RAM(ROUND);
    m = 0x04; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA3CE, 1); goto L_A3E1; }
    mff_cycles(0xA3D0, 7);
    a = RAM(GAME_ACTION);
    m = 0x06; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA3D4, 1); goto L_A3E9; }
    mff_cycles(0xA3D6, 20);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x88;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_A3E1:;
    mff_cycles(0xA3E1, 7);
    a = 0x22;
    m = RAM(PLAYER_INVULN); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xA3E5, 1); goto L_A3E9; }
    mff_cycles(0xA3E7, 3);
    RAM(PLAYER_INVULN) = a;
L_A3E9:;
    mff_cycles(0xA3E9, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A3EA (bank 0) - lifted by port/tools/lift.py */
void func_A3EA_b0(void) {
    REGS_IN;
L_A3EA:;
    mff_cycles(0xA3EA, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x03EC, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA3ED, 9);
    t = (uint8_t)(RAM(OBJ_AI_COUNT + x) - 1); RAM(OBJ_AI_COUNT + x) = t;
    z = (t) == 0;
    if (z) { mff_cycles(0xA3F0, 2); goto L_A401; }
    mff_cycles(0xA3F2, 10);
    a = 0x20;
    y = 0x0A;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x03F8, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA3F9, 2);
    if (!c) { mff_cycles(0xA3F9, 2); goto L_A401; }
    mff_cycles(0xA3FB, 13);
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_A401:;
    mff_cycles(0xA401, 27);
    a = 0x80;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x07;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DOWN_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A417 (bank 0) - lifted by port/tools/lift.py */
void func_A417_b0(void) {
    REGS_IN;
L_A417:;
    mff_cycles(0xA417, 6);
    a = RAM(OBJ_TIMER + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xA41A, 1); goto L_A42A; }
    mff_cycles(0xA41C, 27);
    a = 0x78;
    RAM(OBJ_AI_TIMER + x) = a;
    a = 0x01;
    RAM(OBJ_AI_COUNT + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_A42A:;
    mff_cycles(0xA42A, 10);
    a = 0x20;
    y = 0x0A;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0430, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA431, 2);
    if (!c) { mff_cycles(0xA431, 1); goto L_A441; }
    mff_cycles(0xA433, 25);
    a = 0x82;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    RAM(OBJ_TIMER2 + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_A441:;
    mff_cycles(0xA441, 9);
    t = (uint8_t)(RAM(OBJ_AI_COUNT + x) - 1); RAM(OBJ_AI_COUNT + x) = t;
    z = (t) == 0;
    if (!z) { mff_cycles(0xA444, 1); goto L_A458; }
    mff_cycles(0xA446, 13);
    a = 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_COUNT + x) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x044D, func_FB72, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA44E, 8);
    y = a;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0451, 0xA7E1, 0)) return;
    SYNC_IN;
    mff_cycles(0xA452, 4);
    a = nes_read((uint16_t)(0xFBF0 + y));
    mff_cycles(0xA455, 5);
    RAM(OBJ_DIR + x) = a;
L_A458:;
    mff_cycles(0xA458, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1); RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (z) { mff_cycles(0xA45B, 1); goto L_A463; }
    mff_cycles(0xA45D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x045F, 0xA031, 0)) return;
    SYNC_IN;
    mff_cycles(0xA460, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA090, 2); call_by_address_tail(0xA090, 0); return;
L_A463:;
    mff_cycles(0xA463, 27);
    a = 0x81;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x07;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DOWN_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A473 (bank 0) - lifted by port/tools/lift.py */
void func_A473_b0(void) {
    REGS_IN;
L_A473:;
    mff_cycles(0xA473, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x01; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA478, 1); goto L_A49D; }
    mff_cycles(0xA47A, 4);
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA47C, 1); goto L_A4E2; }
    mff_cycles(0xA47E, 4);
    m = 0x03; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA480, 1); goto L_A485; }
    mff_cycles(0xA482, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA53B, 2); call_by_address_tail(0xA53B, 0); return;
L_A485:;
    mff_cycles(0xA485, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0487, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA488, 6);
    SYNC_OUT;
    if (!mff_call_at(0x048A, 0xA9CA, 0)) return;
    SYNC_IN;
    mff_cycles(0xA48B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x048D, 0xA9F5, 0)) return;
    SYNC_IN;
    mff_cycles(0xA48E, 17);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    a = 0xF4;
    y = 0x2C;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0497, func_F449, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA498, 11);
    a = 0x2E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(CHR_BANKS + 5) = a;
    SYNC_OUT;
    mff_rts(); return;
L_A49D:;
    mff_cycles(0xA49D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x049F, 0xA031, 0)) return;
    SYNC_IN;
    mff_cycles(0xA4A0, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA4A5, 1); goto L_A4B1; }
    mff_cycles(0xA4A7, 8);
    a = RAM(OBJ_X + x);
    m = 0xF1; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xA4AC, 1); goto L_A4B1; }
    mff_cycles(0xA4AE, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA54C, 2); call_by_address_tail(0xA54C, 0); return;
L_A4B1:;
    mff_cycles(0xA4B1, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA4B6, 1); goto L_A4C2; }
    mff_cycles(0xA4B8, 8);
    a = RAM(OBJ_X + x);
    m = 0x18; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xA4BD, 1); goto L_A4C2; }
    mff_cycles(0xA4BF, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA54C, 2); call_by_address_tail(0xA54C, 0); return;
L_A4C2:;
    mff_cycles(0xA4C2, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x04C4, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA4C5, 4);
    m = 0x30; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xA4C7, 1); goto L_A4E1; }
    mff_cycles(0xA4C9, 10);
    a = 0xF6;
    y = 0x2F;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x04CF, func_F44F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA4D0, 6);
    a = RAM(OBJ_TIER + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xA4D3, 1); goto L_A4D9; }
    mff_cycles(0xA4D5, 4);
    a = 0x2E;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA4D7, 1); goto L_A4DB; }
L_A4D9:;
    mff_cycles(0xA4D9, 2);
    a = 0x17;
L_A4DB:;
    mff_cycles(0xA4DB, 12);
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
L_A4E1:;
    mff_cycles(0xA4E1, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A4E2:;
    mff_cycles(0xA4E2, 8);
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA4E7, 1); goto L_A4EE; }
    mff_cycles(0xA4E9, 7);
    a = 0x00;
    RAM(OBJ_ANIM_TIMER + x) = a;
L_A4EE:;
    mff_cycles(0xA4EE, 6);
    a = RAM(OBJ_VX + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xA4F1, 1); goto L_A4F8; }
    mff_cycles(0xA4F3, 7);
    a = 0x8B;
    RAM(OBJ_ATTACK + x) = a;
L_A4F8:;
    mff_cycles(0xA4F8, 58);
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
    if (n) { mff_cycles(0xA51F, 1); goto L_A529; }
    mff_cycles(0xA521, 6);
    a = RAM(OBJ_VY + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) { mff_cycles(0xA524, 1); goto L_A529; }
    mff_cycles(0xA526, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA031, 2); call_by_address_tail(0xA031, 0); return;
L_A529:;
    mff_cycles(0xA529, 13);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    a = RAM(OBJ_TIER + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xA52F, 1); goto L_A535; }
    mff_cycles(0xA531, 4);
    a = 0x06;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA533, 1); goto L_A537; }
L_A535:;
    mff_cycles(0xA535, 2);
    a = 0x0C;
    n = (a) >> 7;
    z = (a) == 0;
L_A537:;
    mff_cycles(0xA537, 11);
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A53B (bank 0) - lifted by port/tools/lift.py */
void func_A53B_b0(void) {
    REGS_IN;
L_A53B:;
    mff_cycles(0xA53B, 6);
    a = RAM(OBJ_AI_TIMER + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xA53E, 1); goto L_A54C; }
    mff_cycles(0xA540, 25);
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    RAM(OBJ_ANIM_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1); RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_A54C:;
    mff_cycles(0xA54C, 20);
    a = 0x80;
    RAM(OBJ_STATE + x) = a;
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DOWN_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A557 (bank 0) - lifted by port/tools/lift.py */
void func_A557_b0(void) {
    REGS_IN;
L_A557:;
    mff_cycles(0xA557, 8);
    a = RAM(OBJ_TIMER + x);
    t = a; c = t & 1; t >>= 1; a = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (c) { mff_cycles(0xA55B, 1); goto L_A58A; }
    mff_cycles(0xA55D, 17);
    a = RAM(OBJ_Y + x);
    RAM(OBJ_GROUND_Y + x) = a;
    a = RAM(OBJ_STATE + x);
    m = 0x86; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA568, 1); goto L_A576; }
    mff_cycles(0xA56A, 16);
    a = 0xE0;
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x04;
    z = (a) == 0;
    RAM(OBJ_VY + x) = a;
    if (!z) { mff_cycles(0xA574, 1); goto L_A580; }
L_A576:;
    mff_cycles(0xA576, 14);
    a = 0x00;
    RAM(OBJ_VY_FRAC + x) = a;
    a = 0x04;
    RAM(OBJ_VY + x) = a;
L_A580:;
    mff_cycles(0xA580, 14);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    a = 0xE7;
    y = 0x2A;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF449, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F449(); g_code_window_base = swb; } return;
L_A58A:;
    mff_cycles(0xA58A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x058C, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA58D, 6);
    a = RAM(OBJ_VY + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0xA590, 1); goto L_A5CE; }
    mff_cycles(0xA592, 31);
    a = 0x01;
    RAM(OBJ_ANIM_FRAME + x) = a;
    a = 0x8F;
    RAM(OBJ_ATTACK + x) = a;
    a = 0x16;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = RAM(OBJ_Y + x);
    m = RAM(OBJ_GROUND_Y + x); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xA5A7, 1); goto L_A5CE; }
    mff_cycles(0xA5A9, 24);
    a = RAM(OBJ_GROUND_Y + x);
    RAM(OBJ_Y + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    a = RAM(OBJ_TIMER + x);
    m = 0x04; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA5B7, 1); goto L_A5CE; }
    mff_cycles(0xA5B9, 31);
    a = 0x00;
    RAM(OBJ_VY_FRAC + x) = a;
    RAM(OBJ_VY + x) = a;
    RAM(OBJ_ATTACK + x) = a;
    a = 0x01;
    RAM(OBJ_TIMER + x) = a;
    a = 0x81;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
L_A5CE:;
    mff_cycles(0xA5CE, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A5CF (bank 0) - lifted by port/tools/lift.py */
void func_A5CF_b0(void) {
    REGS_IN;
L_A5CF:;
    mff_cycles(0xA5CF, 20);
    a = 0x00;
    RAM(OBJ_VX_FRAC + x) = a;
    a = 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_VX + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x05DB, 0xA557, 0)) return;
    SYNC_IN;
    mff_cycles(0xA5DC, 8);
    a = RAM(OBJ_STATE + x);
    m = 0x81; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA5E1, 1); goto L_A5E9; }
    mff_cycles(0xA5E3, 13);
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_A5E9:;
    mff_cycles(0xA5E9, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x03; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA5EE, 1); goto L_A5F5; }
    mff_cycles(0xA5F0, 7);
    a = 0x01;
    RAM(OBJ_TIMER + x) = a;
L_A5F5:;
    mff_cycles(0xA5F5, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xA5FA, 1); goto L_A5FF; }
    mff_cycles(0xA5FC, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x05FE, func_F1A1, 0xE000)) return;
    SYNC_IN;
L_A5FF:;
    mff_cycles(0xA5FF, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xA604, 1); goto L_A609; }
    mff_cycles(0xA606, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0608, func_F183, 0xE000)) return;
    SYNC_IN;
L_A609:;
    mff_cycles(0xA609, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA60E, 1); goto L_A61C; }
    mff_cycles(0xA610, 8);
    a = 0x18;
    m = RAM(OBJ_X + x); c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xA615, 1); goto L_A61C; }
    mff_cycles(0xA617, 7);
    RAM(OBJ_X + x) = a;
    if (!z) { mff_cycles(0xA61A, 1); goto L_A62D; }
L_A61C:;
    mff_cycles(0xA61C, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA621, 1); goto L_A632; }
    mff_cycles(0xA623, 8);
    a = 0xF1;
    m = RAM(OBJ_X + x); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xA628, 1); goto L_A632; }
    mff_cycles(0xA62A, 5);
    RAM(OBJ_X + x) = a;
L_A62D:;
    mff_cycles(0xA62D, 7);
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
L_A632:;
    mff_cycles(0xA632, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A633 (bank 0) - lifted by port/tools/lift.py */
void func_A633_b0(void) {
    REGS_IN;
L_A633:;
    mff_cycles(0xA633, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0635, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA636, 9);
    t = (uint8_t)(RAM(OBJ_HIT_TIMER + x) - 1); RAM(OBJ_HIT_TIMER + x) = t;
    z = (t) == 0;
    if (!z) { mff_cycles(0xA639, 1); goto L_A64B; }
    mff_cycles(0xA63B, 8);
    a = RAM(OBJ_KIND + x);
    a &= 0x08;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA640, 1); goto L_A646; }
    mff_cycles(0xA642, 4);
    a = 0x80;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA644, 1); goto L_A648; }
L_A646:;
    mff_cycles(0xA646, 2);
    a = 0x84;
L_A648:;
    mff_cycles(0xA648, 5);
    RAM(OBJ_STATE + x) = a;
L_A64B:;
    mff_cycles(0xA64B, 13);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A651 (bank 0) - lifted by port/tools/lift.py */
void func_A651_b0(void) {
    REGS_IN;
L_A651:;
    mff_cycles(0xA651, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0653, 0xA882, 0)) return;
    SYNC_IN;
    mff_cycles(0xA654, 8);
    a = RAM(OBJ_X_HI + x);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA659, 1); goto L_A684; }
    mff_cycles(0xA65B, 8);
    a = RAM(OBJ_KIND + x);
    m = 0x07; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA660, 1); goto L_A668; }
    mff_cycles(0xA662, 6);
    a = 0x20;
    y = 0x0A;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0xA666, 1); goto L_A66C; }
L_A668:;
    mff_cycles(0xA668, 4);
    a = 0x24;
    y = 0x09;
    n = (y) >> 7;
    z = (y) == 0;
L_A66C:;
    mff_cycles(0xA66C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x066E, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA66F, 2);
    if (!c) { mff_cycles(0xA66F, 1); goto L_A684; }
    mff_cycles(0xA671, 31);
    a = RAM(OBJ_KIND + x);
    m = 0x07; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    RAM(OBJ_TIMER2 + x) = a;
    a = 0x82;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_A684:;
    mff_cycles(0xA684, 8);
    y = 0x04;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0688, 0xA7E1, 0)) return;
    SYNC_IN;
    mff_cycles(0xA689, 8);
    a = RAM(OBJ_DIR + x);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA68E, 1); goto L_A6A1; }
    mff_cycles(0xA690, 8);
    a = RAM(OBJ_X_HI + x);
    m = 0x01; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA695, 1); goto L_A69E; }
    mff_cycles(0xA697, 8);
    a = RAM(OBJ_X + x);
    m = 0x18; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xA69C, 1); goto L_A6B2; }
L_A69E:;
    mff_cycles(0xA69E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF183, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F183(); g_code_window_base = swb; } return;
L_A6A1:;
    mff_cycles(0xA6A1, 8);
    a = RAM(OBJ_X_HI + x);
    m = 0x01; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA6A6, 1); goto L_A6AF; }
    mff_cycles(0xA6A8, 8);
    a = RAM(OBJ_X + x);
    m = 0xF2; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xA6AD, 1); goto L_A6B2; }
L_A6AF:;
    mff_cycles(0xA6AF, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF1A1, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F1A1(); g_code_window_base = swb; } return;
L_A6B2:;
    mff_cycles(0xA6B2, 24);
    a = 0x81;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x07;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DOWN_TIMER + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF343, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F343(); g_code_window_base = swb; } return;
}

/*  $A6D6 (bank 0) - lifted by port/tools/lift.py */
void func_A6D6_b0(void) {
    REGS_IN;
L_A6D6:;
    mff_cycles(0xA6D6, 6);
    a = RAM(OBJ_HELD + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xA6D9, 2); goto L_A71C; }
    mff_cycles(0xA6DB, 4);
    m = 0xFF; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA6DD, 2); goto L_A70B; }
    mff_cycles(0xA6DF, 7);
    a = RAM(GAME_ACTION);
    m = 0x04; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA6E3, 1); goto L_A6E9; }
    mff_cycles(0xA6E5, 4);
    m = 0x21; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA6E7, 2); goto L_A71C; }
L_A6E9:;
    mff_cycles(0xA6E9, 5);
    m = RAM(GRAB_SLOT); c = x >= m;
    z = ((uint8_t)(x - m)) == 0;
    if (!z) { mff_cycles(0xA6EB, 2); goto L_A71C; }
    mff_cycles(0xA6ED, 7);
    a = RAM(GAME_ACTION);
    m = 0x21; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA6F1, 1); goto L_A6F9; }
    mff_cycles(0xA6F3, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06F5, 0xA8EF, 0)) return;
    SYNC_IN;
    mff_cycles(0xA6F6, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA6FC, 2); call_by_address_tail(0xA6FC, 0); return;
L_A6F9:;
    mff_cycles(0xA6F9, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06FB, 0xA83A, 0)) return;
    SYNC_IN;
    mff_cycles(0xA6FC, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x06FE, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA6FF, 22);
    a = 0x80;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_HIT_TIMER + x) = a;
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
L_A70B:;
    mff_cycles(0xA70B, 14);
    a = 0x00;
    RAM(OBJ_HELD + x) = a;
    a = RAM(PLAYER_ID);
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA714, 1); goto L_A71C; }
    mff_cycles(0xA716, 9);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + x) = a;
L_A71C:;
    mff_cycles(0xA71C, 6);
    a = RAM(OBJ_HIT_TIMER + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xA71F, 1); goto L_A74C; }
    mff_cycles(0xA721, 4);
    m = 0x1E; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA723, 1); goto L_A72D; }
    mff_cycles(0xA725, 4);
    m = 0x1A; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA727, 1); goto L_A733; }
    mff_cycles(0xA729, 4);
    m = 0x10; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA72B, 1); goto L_A736; }
L_A72D:;
    mff_cycles(0xA72D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x072F, 0xA83A, 0)) return;
    SYNC_IN;
    mff_cycles(0xA730, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA736, 2); call_by_address_tail(0xA736, 0); return;
L_A733:;
    mff_cycles(0xA733, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0735, 0xA8EF, 0)) return;
    SYNC_IN;
L_A736:;
    mff_cycles(0xA736, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0738, 0xA090, 0)) return;
    SYNC_IN;
    mff_cycles(0xA739, 15);
    a = 0x87;
    RAM(OBJ_STATE + x) = a;
    a = RAM(OBJ_TIER + x);
    m = 0x02; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA743, 1); goto L_A74A; }
    mff_cycles(0xA745, 7);
    a = 0x20;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_INVULN + x) = a;
L_A74A:;
    mff_cycles(0xA74A, 8);
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
L_A74C:;
    mff_cycles(0xA74C, 7);
    a = RAM(GAME_ACTION);
    m = 0x0A; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA750, 1); goto L_A75A; }
    mff_cycles(0xA752, 4);
    m = 0x0B; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA754, 1); goto L_A75F; }
    mff_cycles(0xA756, 4);
    m = 0x19; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA758, 1); goto L_A766; }
L_A75A:;
    mff_cycles(0xA75A, 6);
    SYNC_OUT;
    if (!mff_call_at(0x075C, 0xA83A, 0)) return;
    SYNC_IN;
    mff_cycles(0xA75D, 8);
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
L_A75F:;
    mff_cycles(0xA75F, 15);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_HIT_PLAYER + x) = a;
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
L_A766:;
    mff_cycles(0xA766, 6);
    a = RAM(OBJ_TIER + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xA769, 1); goto L_A794; }
    mff_cycles(0xA76B, 8);
    a = RAM(OBJ_STATE + x);
    m = 0x81; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA770, 1); goto L_A794; }
    mff_cycles(0xA772, 7);
    a = RAM(GAME_ACTION);
    m = 0x1F; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA776, 1); goto L_A794; }
    mff_cycles(0xA778, 8);
    a = RAM(OBJ_STATE + x);
    m = 0x85; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA77D, 1); goto L_A792; }
    mff_cycles(0xA77F, 4);
    m = 0x86; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA781, 1); goto L_A792; }
    mff_cycles(0xA783, 21);
    a = 0x07;
    RAM(OBJ_DOWN_TIMER + x) = a;
    a = 0x80;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
L_A792:;
    mff_cycles(0xA792, 8);
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
L_A794:;
    mff_cycles(0xA794, 7);
    a = RAM(GAME_ACTION);
    m = 0x25; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA798, 1); goto L_A79C; }
    mff_cycles(0xA79A, 8);
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
L_A79C:;
    mff_cycles(0xA79C, 6);
    a = RAM(OBJ_TIER + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xA79F, 1); goto L_A7B5; }
    mff_cycles(0xA7A1, 8);
    a = RAM(OBJ_STATE + x);
    m = 0x85; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA7A6, 1); goto L_A7AC; }
    mff_cycles(0xA7A8, 4);
    m = 0x86; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA7AA, 1); goto L_A7B5; }
L_A7AC:;
    mff_cycles(0xA7AC, 12);
    a = RAM(OBJ_GROUND_Y + x);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_LAND_Y + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA7BB, 2); call_by_address_tail(0xA7BB, 0); return;
L_A7B5:;
    mff_cycles(0xA7B5, 68);
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
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x80;
    RAM(OBJ_STATE + x) = a;
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_INVULN + x) = a;
    c = 1;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A736 (bank 0) - lifted by port/tools/lift.py */
void func_A736_b0(void) {
    REGS_IN;
L_A736:;
    mff_cycles(0xA736, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0738, 0xA090, 0)) return;
    SYNC_IN;
    mff_cycles(0xA739, 15);
    a = 0x87;
    RAM(OBJ_STATE + x) = a;
    a = RAM(OBJ_TIER + x);
    m = 0x02; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA743, 1); goto L_A74A; }
    mff_cycles(0xA745, 7);
    a = 0x20;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_INVULN + x) = a;
L_A74A:;
    mff_cycles(0xA74A, 8);
    c = 0;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A7E1 (bank 0) - lifted by port/tools/lift.py */
void func_A7E1_b0(void) {
    REGS_IN;
L_A7E1:;
    mff_cycles(0xA7E1, 4);
    a = nes_read((uint16_t)(0xA80A + y));
    mff_cycles(0xA7E4, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = nes_read((uint16_t)(0xA7FA + y));
    mff_cycles(0xA7EA, 9);
    RAM(OBJ_VX + x) = a;
    a = nes_read((uint16_t)(0xA82A + y));
    mff_cycles(0xA7F0, 9);
    RAM(OBJ_VY_FRAC + x) = a;
    a = nes_read((uint16_t)(0xA81A + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA7F6, 11);
    RAM(OBJ_VY + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A83A (bank 0) - lifted by port/tools/lift.py */
void func_A83A_b0(void) {
    REGS_IN;
L_A83A:;
    mff_cycles(0xA83A, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x02; c = a >= m;
    if (c) { mff_cycles(0xA83F, 1); goto L_A876; }
    mff_cycles(0xA841, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    y = a;
    a = nes_read((uint16_t)(0xA870 + y));
    mff_cycles(0xA84B, 6);
    m = RAM(OBJ_ANIM + x); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA84E, 1); goto L_A86F; }
    mff_cycles(0xA850, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0852, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA853, 4);
    a = nes_read((uint16_t)(0xA872 + y));
    mff_cycles(0xA856, 7);
    RAM(0x00) = a;
    a = nes_read((uint16_t)(0xA874 + y));
    mff_cycles(0xA85B, 7);
    RAM(0x01) = a;
    a = nes_read((uint16_t)(0xA914 + y));
    mff_cycles(0xA860, 14);
    y = a;
    a = RAM(0x00);
    RAM(CHR_BANKS + 3 + y) = a;
    a = nes_read((uint16_t)(0xA916 + y));
    mff_cycles(0xA869, 10);
    y = a;
    a = RAM(0x01);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(CHR_BANKS + 3 + y) = a;
L_A86F:;
    mff_cycles(0xA86F, 6);
    SYNC_OUT;
    mff_rts(); return;
L_A876:;
    mff_cycles(0xA876, 10);
    a = 0xEF;
    y = 0x2C;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x087C, func_F449, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA87D, 11);
    a = 0x2E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(CHR_BANKS + 5) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A882 (bank 0) - lifted by port/tools/lift.py */
void func_A882_b0(void) {
    REGS_IN;
L_A882:;
    mff_cycles(0xA882, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    y = a;
    a = nes_read((uint16_t)(0xA8B1 + y));
    mff_cycles(0xA88C, 6);
    m = RAM(OBJ_ANIM + x); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA88F, 1); goto L_A8B0; }
    mff_cycles(0xA891, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0893, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA894, 4);
    a = nes_read((uint16_t)(0xA8B3 + y));
    mff_cycles(0xA897, 7);
    RAM(0x00) = a;
    a = nes_read((uint16_t)(0xA8B5 + y));
    mff_cycles(0xA89C, 7);
    RAM(0x01) = a;
    a = nes_read((uint16_t)(0xA914 + y));
    mff_cycles(0xA8A1, 14);
    y = a;
    a = RAM(0x00);
    RAM(CHR_BANKS + 3 + y) = a;
    a = nes_read((uint16_t)(0xA916 + y));
    mff_cycles(0xA8AA, 10);
    y = a;
    a = RAM(0x01);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(CHR_BANKS + 3 + y) = a;
L_A8B0:;
    mff_cycles(0xA8B0, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A8B7 (bank 0) - lifted by port/tools/lift.py */
void func_A8B7_b0(void) {
    REGS_IN;
L_A8B7:;
    mff_cycles(0xA8B7, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    y = a;
    a = nes_read((uint16_t)(0xA8E9 + y));
    mff_cycles(0xA8C1, 6);
    m = RAM(OBJ_ANIM + x); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA8C4, 1); goto L_A8E8; }
    mff_cycles(0xA8C6, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x08C8, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA8C9, 4);
    a = nes_read((uint16_t)(0xA8EB + y));
    mff_cycles(0xA8CC, 7);
    RAM(0x00) = a;
    a = nes_read((uint16_t)(0xA8ED + y));
    mff_cycles(0xA8D1, 7);
    RAM(0x01) = a;
    a = nes_read((uint16_t)(0xA914 + y));
    mff_cycles(0xA8D6, 14);
    y = a;
    a = RAM(0x00);
    RAM(CHR_BANKS + 3 + y) = a;
    a = nes_read((uint16_t)(0xA916 + y));
    mff_cycles(0xA8DF, 17);
    y = a;
    a = RAM(0x01);
    RAM(CHR_BANKS + 3 + y) = a;
    t = (uint8_t)(RAM(OBJ_ANIM_TIMER + x) + 1); RAM(OBJ_ANIM_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
L_A8E8:;
    mff_cycles(0xA8E8, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A8EF (bank 0) - lifted by port/tools/lift.py */
void func_A8EF_b0(void) {
    REGS_IN;
L_A8EF:;
    mff_cycles(0xA8EF, 14);
    a = RAM(OBJ_KIND + x);
    c = 1;
    m = 0x07; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    y = a;
    a = nes_read((uint16_t)(0xA910 + y));
    mff_cycles(0xA8F9, 6);
    m = RAM(OBJ_ANIM + x); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA8FC, 2); goto L_A90F; }
    mff_cycles(0xA8FE, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0900, func_F288, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA901, 4);
    a = nes_read((uint16_t)(0xA912 + y));
    mff_cycles(0xA904, 7);
    RAM(0x00) = a;
    a = nes_read((uint16_t)(0xA914 + y));
    mff_cycles(0xA909, 10);
    y = a;
    a = RAM(0x00);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(CHR_BANKS + 3 + y) = a;
L_A90F:;
    mff_cycles(0xA90F, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A918 (bank 0) - lifted by port/tools/lift.py */
void func_A918_b0(void) {
    REGS_IN;
L_A918:;
    mff_cycles(0xA918, 4);
    a = nes_read((uint16_t)(0xA941 + y));
    mff_cycles(0xA91B, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = nes_read((uint16_t)(0xA931 + y));
    mff_cycles(0xA921, 9);
    RAM(OBJ_VX + x) = a;
    a = nes_read((uint16_t)(0xA961 + y));
    mff_cycles(0xA927, 9);
    RAM(OBJ_VY_FRAC + x) = a;
    a = nes_read((uint16_t)(0xA951 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA92D, 11);
    RAM(OBJ_VY + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A971 (bank 0) - lifted by port/tools/lift.py */
void func_A971_b0(void) {
    REGS_IN;
L_A971:;
    mff_cycles(0xA971, 4);
    a = nes_read((uint16_t)(0xA99A + y));
    mff_cycles(0xA974, 9);
    RAM(OBJ_VX_FRAC + x) = a;
    a = nes_read((uint16_t)(0xA98A + y));
    mff_cycles(0xA97A, 9);
    RAM(OBJ_VX + x) = a;
    a = nes_read((uint16_t)(0xA9BA + y));
    mff_cycles(0xA980, 9);
    RAM(OBJ_VY_FRAC + x) = a;
    a = nes_read((uint16_t)(0xA9AA + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA986, 11);
    RAM(OBJ_VY + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A9CA (bank 0) - lifted by port/tools/lift.py */
void func_A9CA_b0(void) {
    REGS_IN;
L_A9CA:;
    mff_cycles(0xA9CA, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x09CC, func_FB5B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA9CD, 6);
    y = a;
    a &= 0x07;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA9D0, 1); goto L_A9EA; }
    mff_cycles(0xA9D2, 4);
    a = y;
    z = (a) == 0;
    if (!z) { mff_cycles(0xA9D3, 1); goto L_A9D9; }
    mff_cycles(0xA9D5, 6);
    y++;
    y++;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0xA9D7, 1); goto L_A9DB; }
L_A9D9:;
    mff_cycles(0xA9D9, 4);
    y--;
    y--;
    n = (y) >> 7;
    z = (y) == 0;
L_A9DB:;
    mff_cycles(0xA9DB, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x09DD, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA9DE, 2);
    if (c) { mff_cycles(0xA9DE, 1); goto L_A9EE; }
    mff_cycles(0xA9E0, 12);
    a = y;
    a ^= 0xFF;
    m = 0x01; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    a &= 0x0F;
    y = a;
    z = (y) == 0;
    if (!z) { mff_cycles(0xA9E8, 1); goto L_A9EE; }
L_A9EA:;
    mff_cycles(0xA9EA, 4);
    a = nes_read((uint16_t)(0xAA23 + y));
    mff_cycles(0xA9ED, 2);
    y = a;
L_A9EE:;
    mff_cycles(0xA9EE, 4);
    a = nes_read((uint16_t)(0xFBF0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xA9F1, 11);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A9F5 (bank 0) - lifted by port/tools/lift.py */
void func_A9F5_b0(void) {
    REGS_IN;
L_A9F5:;
    mff_cycles(0xA9F5, 6);
    SYNC_OUT;
    if (!mff_call_at(0x09F7, 0xA918, 0)) return;
    SYNC_IN;
    mff_cycles(0xA9F8, 6);
    a = y;
    a &= 0x01;
    z = (a) == 0;
    if (z) { mff_cycles(0xA9FB, 2); goto L_AA09; }
    mff_cycles(0xA9FD, 16);
    a = 0x12;
    RAM(OBJ_TIMER2 + x) = a;
    a = 0x07;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_GROUND_Y + x) = a;
    if (!z) { mff_cycles(0xAA07, 1); goto L_AA22; }
L_AA09:;
    mff_cycles(0xAA09, 8);
    a = y;
    t = a; c = t & 1; t >>= 1; a = t;
    t = a; c = t & 1; t >>= 1; a = t;
    if (!c) { mff_cycles(0xAA0C, 1); goto L_AA18; }
    mff_cycles(0xAA0E, 14);
    a = 0x0E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER2 + x) = a;
    RAM(OBJ_GROUND_Y + x) = a;
    if (!z) { mff_cycles(0xAA16, 1); goto L_AA22; }
L_AA18:;
    mff_cycles(0xAA18, 14);
    a = 0x14;
    RAM(OBJ_TIMER2 + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_GROUND_Y + x) = a;
L_AA22:;
    mff_cycles(0xAA22, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AA33 (bank 0) - lifted by port/tools/lift.py */
void func_AA33_b0(void) {
    REGS_IN;
L_AA33:;
    mff_cycles(0xAA33, 8);
    a = RAM(OBJ_LIFE_MAX + x);
    m = 0x30; c = a >= m;
    if (c) { mff_cycles(0xAA38, 1); goto L_AA45; }
    mff_cycles(0xAA3A, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xAA3F, 1); goto L_AA4A; }
    mff_cycles(0xAA41, 4);
    a = 0x02;
    z = (a) == 0;
    if (!z) { mff_cycles(0xAA43, 1); goto L_AA47; }
L_AA45:;
    mff_cycles(0xAA45, 2);
    a = 0x01;
L_AA47:;
    mff_cycles(0xAA47, 5);
    RAM(OBJ_TIER + x) = a;
L_AA4A:;
    mff_cycles(0xAA4A, 20);
    a = 0x08;
    RAM(OBJ_KIND + x) = a;
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_INVULN + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xAA57, 1); goto L_AA5C; }
    mff_cycles(0xAA59, 7);
    t = (uint8_t)(RAM(OBJ_INVULN + x) - 1); RAM(OBJ_INVULN + x) = t;
L_AA5C:;
    mff_cycles(0xAA5C, 8);
    a = RAM(OBJ_ID + 13);
    m = 0x6E; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xAA61, 1); goto L_AA69; }
    mff_cycles(0xAA63, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0A65, 0xA6D6, 0)) return;
    SYNC_IN;
    mff_cycles(0xAA66, 2);
    if (!c) { mff_cycles(0xAA66, 1); goto L_AA6F; }
    mff_cycles(0xAA68, 6);
    SYNC_OUT;
    mff_rts(); return;
L_AA69:;
    mff_cycles(0xAA69, 13);
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_INVULN + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_AA6F:;
    mff_cycles(0xAA6F, 7);
    a = RAM(GAME_ACTION);
    m = 0x1F; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xAA73, 1); goto L_AAB3; }
    mff_cycles(0xAA75, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x03; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xAA7A, 1); goto L_AA9B; }
    mff_cycles(0xAA7C, 8);
    a = RAM(OBJ_STATE + x);
    m = 0x83; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xAA81, 1); goto L_AA8A; }
    mff_cycles(0xAA83, 8);
    a = RAM(OBJ_TIMER + x);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xAA88, 1); goto L_AAB3; }
L_AA8A:;
    mff_cycles(0xAA8A, 23);
    a = 0x83;
    RAM(OBJ_STATE + x) = a;
    a = 0x02;
    RAM(OBJ_TIMER + x) = a;
    a = 0x20;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    if (!z) { mff_cycles(0xAA99, 1); goto L_AAB0; }
L_AA9B:;
    mff_cycles(0xAA9B, 8);
    a = RAM(OBJ_STATE + x);
    m = 0x86; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xAAA0, 1); goto L_AAB0; }
    mff_cycles(0xAAA2, 4);
    m = 0x85; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xAAA4, 1); goto L_AAB3; }
    mff_cycles(0xAAA6, 14);
    a = 0x85;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
L_AAB0:;
    mff_cycles(0xAAB0, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA83A, 2); call_by_address_tail(0xA83A, 0); return;
L_AAB3:;
    mff_cycles(0xAAB3, 12);
    a = RAM(OBJ_STATE + x);
    a &= 0x0F;
    y = a;
    a = nes_read((uint16_t)(0xAAC6 + y));
    mff_cycles(0xAABC, 7);
    RAM(0x00) = a;
    a = nes_read((uint16_t)(0xAACF + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xAAC1, 8);
    RAM(0x01) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $AAD8 (bank 0) - lifted by port/tools/lift.py */
void func_AAD8_b0(void) {
    REGS_IN;
L_AAD8:;
    mff_cycles(0xAAD8, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x03; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xAADD, 1); goto L_AAF4; }
    mff_cycles(0xAADF, 4);
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xAAE1, 1); goto L_AAFF; }
    mff_cycles(0xAAE3, 25);
    a = 0x7C;
    RAM(OBJ_INVULN + x) = a;
    a = 0x74;
    RAM(OBJ_AI_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_TIER + x) + 1); RAM(OBJ_TIER + x) = t;
    a = 0x03;
    z = (a) == 0;
    if (!z) { mff_cycles(0xAAF2, 1); goto L_AAF6; }
L_AAF4:;
    mff_cycles(0xAAF4, 2);
    a = 0x00;
L_AAF6:;
    mff_cycles(0xAAF6, 18);
    RAM(OBJ_TIMER + x) = a;
    a = 0x83;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_AAFF:;
    mff_cycles(0xAAFF, 17);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_STATE + x) + 1); RAM(OBJ_STATE + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA83A, 2); call_by_address_tail(0xA83A, 0); return;
}

/*  $AB0A (bank 0) - lifted by port/tools/lift.py */
void func_AB0A_b0(void) {
    REGS_IN;
L_AB0A:;
    mff_cycles(0xAB0A, 10);
    a = 0x24;
    y = 0x09;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0B10, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAB11, 2);
    if (!c) { mff_cycles(0xAB11, 1); goto L_AB23; }
    mff_cycles(0xAB13, 27);
    a = 0x82;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER2 + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_AB23:;
    mff_cycles(0xAB23, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0B25, 0xA882, 0)) return;
    SYNC_IN;
    mff_cycles(0xAB26, 6);
    a = RAM(OBJ_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xAB29, 1); goto L_AB2E; }
    mff_cycles(0xAB2B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAB93, 2); call_by_address_tail(0xAB93, 0); return;
L_AB2E:;
    mff_cycles(0xAB2E, 10);
    a = 0x30;
    y = 0x10;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0B34, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAB35, 2);
    if (!c) { mff_cycles(0xAB35, 1); goto L_AB5A; }
    mff_cycles(0xAB37, 7);
    a = RAM(GAME_ACTION);
    m = 0x06; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xAB3B, 1); goto L_AB46; }
    mff_cycles(0xAB3D, 4);
    m = 0x1F; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xAB3F, 1); goto L_AB46; }
    mff_cycles(0xAB41, 7);
    a = RAM(RNG);
    t = a; c = t & 1; t >>= 1; a = t;
    if (c) { mff_cycles(0xAB44, 1); goto L_AB51; }
L_AB46:;
    mff_cycles(0xAB46, 20);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x85;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_AB51:;
    mff_cycles(0xAB51, 20);
    a = 0x00;
    RAM(OBJ_AI_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_AB5A:;
    mff_cycles(0xAB5A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B5C, func_FBD9, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAB5D, 4);
    m = 0x08; c = a >= m;
    if (!c) { mff_cycles(0xAB5F, 1); goto L_AB7A; }
    mff_cycles(0xAB61, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0B65, 0xA7E1, 0)) return;
    SYNC_IN;
    mff_cycles(0xAB66, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B68, func_FBD9, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAB69, 2);
    if (c) { mff_cycles(0xAB69, 1); goto L_AB6F; }
    mff_cycles(0xAB6B, 4);
    a = 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xAB6D, 1); goto L_AB71; }
L_AB6F:;
    mff_cycles(0xAB6F, 2);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
L_AB71:;
    mff_cycles(0xAB71, 11);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0B76, 0xA031, 0)) return;
    SYNC_IN;
    mff_cycles(0xAB77, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA090, 2); call_by_address_tail(0xA090, 0); return;
L_AB7A:;
    mff_cycles(0xAB7A, 8);
    y = 0x04;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0B7E, 0xA7E1, 0)) return;
    SYNC_IN;
    mff_cycles(0xAB7F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B81, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAB82, 2);
    if (c) { mff_cycles(0xAB82, 1); goto L_AB88; }
    mff_cycles(0xAB84, 4);
    a = 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xAB86, 1); goto L_AB8A; }
L_AB88:;
    mff_cycles(0xAB88, 2);
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
L_AB8A:;
    mff_cycles(0xAB8A, 11);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0B8F, 0xA031, 0)) return;
    SYNC_IN;
    mff_cycles(0xAB90, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA090, 2); call_by_address_tail(0xA090, 0); return;
}

/*  $AB93 (bank 0) - lifted by port/tools/lift.py */
void func_AB93_b0(void) {
    REGS_IN;
L_AB93:;
    mff_cycles(0xAB93, 6);
    a = RAM(OBJ_AI_TIMER + x);
    z = (a) == 0;
    if (z) { mff_cycles(0xAB96, 1); goto L_AB9E; }
    mff_cycles(0xAB98, 10);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1); RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xABB0, 2); call_by_address_tail(0xABB0, 0); return;
L_AB9E:;
    mff_cycles(0xAB9E, 13);
    a = 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0BA5, func_FB72, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xABA6, 8);
    y = a;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0BA9, 0xA7E1, 0)) return;
    SYNC_IN;
    mff_cycles(0xABAA, 4);
    a = nes_read((uint16_t)(0xFBF0 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xABAD, 11);
    RAM(OBJ_DIR + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0BB2, 0xA031, 0)) return;
    SYNC_IN;
    mff_cycles(0xABB3, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA090, 2); call_by_address_tail(0xA090, 0); return;
}

/*  $ABB0 (bank 0) - lifted by port/tools/lift.py */
void func_ABB0_b0(void) {
    REGS_IN;
L_ABB0:;
    mff_cycles(0xABB0, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0BB2, 0xA031, 0)) return;
    SYNC_IN;
    mff_cycles(0xABB3, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA090, 2); call_by_address_tail(0xA090, 0); return;
}

/*  $ABB6 (bank 0) - lifted by port/tools/lift.py */
void func_ABB6_b0(void) {
    REGS_IN;
L_ABB6:;
    mff_cycles(0xABB6, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0BB8, 0xA882, 0)) return;
    SYNC_IN;
    mff_cycles(0xABB9, 7);
    a = RAM(GAME_ACTION);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xABBD, 1); goto L_ABD8; }
    mff_cycles(0xABBF, 10);
    a = 0x20;
    y = 0x09;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0BC5, func_FB3E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xABC6, 2);
    if (!c) { mff_cycles(0xABC6, 1); goto L_ABD8; }
    mff_cycles(0xABC8, 27);
    a = 0x82;
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER2 + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_ABD8:;
    mff_cycles(0xABD8, 6);
    a = RAM(OBJ_TIMER + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xABDB, 2); goto L_AC28; }
    mff_cycles(0xABDD, 9);
    a = RAM(RNG);
    a &= 0x0F;
    m = 0x08; c = a >= m;
    if (c) { mff_cycles(0xABE3, 1); goto L_ABEC; }
    mff_cycles(0xABE5, 9);
    a = 0x03;
    z = (a) == 0;
    RAM(OBJ_DIR + x) = a;
    if (!z) { mff_cycles(0xABEA, 2); goto L_AC01; }
L_ABEC:;
    mff_cycles(0xABEC, 4);
    m = 0x0D; c = a >= m;
    if (c) { mff_cycles(0xABEE, 1); goto L_ABFC; }
    mff_cycles(0xABF0, 8);
    y = 0x04;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0BF4, 0xA7E1, 0)) return;
    SYNC_IN;
    mff_cycles(0xABF5, 9);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DIR + x) = a;
    if (!z) { mff_cycles(0xABFA, 2); goto L_AC06; }
L_ABFC:;
    mff_cycles(0xABFC, 7);
    a = 0x05;
    RAM(OBJ_DIR + x) = a;
L_AC01:;
    mff_cycles(0xAC01, 8);
    y = 0x05;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0C05, 0xA7E1, 0)) return;
    SYNC_IN;
L_AC06:;
    mff_cycles(0xAC06, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C08, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC09, 2);
    if (!c) { mff_cycles(0xAC09, 1); goto L_AC16; }
    mff_cycles(0xAC0B, 18);
    a = RAM(OBJ_DIR + x);
    a ^= 0x0F;
    RAM(OBJ_DIR + x) = a;
    t = (uint8_t)(RAM(OBJ_DIR + x) + 1); RAM(OBJ_DIR + x) = t;
L_AC16:;
    mff_cycles(0xAC16, 8);
    y = RAM(OBJ_DIR + x);
    a = nes_read((uint16_t)(0xFBF0 + y));
    mff_cycles(0xAC1C, 25);
    RAM(OBJ_DIR + x) = a;
    a = 0x64;
    RAM(OBJ_AI_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_AC28:;
    mff_cycles(0xAC28, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1); RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (z) { mff_cycles(0xAC2B, 1); goto L_AC43; }
    mff_cycles(0xAC2D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0C2F, 0xA031, 0)) return;
    SYNC_IN;
    mff_cycles(0xAC30, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0C32, 0xA090, 0)) return;
    SYNC_IN;
    mff_cycles(0xAC33, 5);
    a = RAM(0x0F);
    z = (a) == 0;
    if (!z) { mff_cycles(0xAC35, 1); goto L_AC43; }
    mff_cycles(0xAC37, 8);
    a = RAM(OBJ_X + x);
    m = 0xF1; c = a >= m;
    if (c) { mff_cycles(0xAC3C, 1); goto L_AC43; }
    mff_cycles(0xAC3E, 4);
    m = 0x18; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xAC40, 1); goto L_AC43; }
    mff_cycles(0xAC42, 6);
    SYNC_OUT;
    mff_rts(); return;
L_AC43:;
    mff_cycles(0xAC43, 20);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    a = 0x86;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AC4E (bank 0) - lifted by port/tools/lift.py */
void func_AC4E_b0(void) {
    REGS_IN;
L_AC4E:;
    mff_cycles(0xAC4E, 6);
    a = RAM(OBJ_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xAC51, 1); goto L_AC61; }
    mff_cycles(0xAC53, 4);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xAC55, 1); goto L_AC87; }
    mff_cycles(0xAC57, 4);
    m = 0x02; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xAC59, 1); goto L_AC5E; }
    mff_cycles(0xAC5B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAD50, 2); call_by_address_tail(0xAD50, 0); return;
L_AC5E:;
    mff_cycles(0xAC5E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAD5E, 2); call_by_address_tail(0xAD5E, 0); return;
L_AC61:;
    mff_cycles(0xAC61, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0C63, 0xA9CA, 0)) return;
    SYNC_IN;
    mff_cycles(0xAC64, 13);
    a = y;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER3 + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0C6A, 0xA971, 0)) return;
    SYNC_IN;
    mff_cycles(0xAC6B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C6D, func_F32E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC6E, 31);
    a = 0x00;
    RAM(OBJ_AI_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    a = 0x17;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = 0xF3;
    y = 0x2C;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0C81, func_F449, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC82, 11);
    a = 0x2E;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(CHR_BANKS + 5) = a;
    SYNC_OUT;
    mff_rts(); return;
L_AC87:;
    mff_cycles(0xAC87, 25);
    a = 0x90;
    RAM(OBJ_ATTACK + x) = a;
    a = 0x17;
    y = RAM(OBJ_AI_TIMER + x);
    a = RAM(OBJ_DIR + x);
    a ^= RAM(OBJ_AI_TIMER + x);
    t = a; c = t & 1; t >>= 1; a = t;
    if (!c) { mff_cycles(0xAC98, 1); goto L_ACAF; }
    mff_cycles(0xAC9A, 10);
    a = RAM(OBJ_X + x);
    c = 0;
    m = nes_read((uint16_t)(0xAD80 + y)); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    mff_cycles(0xACA1, 19);
    RAM(OBJ_X + x) = a;
    a = RAM(OBJ_X_HI + x);
    m = 0x00; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X_HI + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xACC1, 2); call_by_address_tail(0xACC1, 0); return;
L_ACAF:;
    mff_cycles(0xACAF, 10);
    a = RAM(OBJ_X + x);
    c = 1;
    m = nes_read((uint16_t)(0xAD80 + y)); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    mff_cycles(0xACB6, 26);
    RAM(OBJ_X + x) = a;
    a = RAM(OBJ_X_HI + x);
    m = 0x00; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    RAM(OBJ_X_HI + x) = a;
    a = RAM(OBJ_TIMER3 + x);
    a &= 0x07;
    m = 0x04; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xACC8, 2); goto L_AD0B; }
    mff_cycles(0xACCA, 8);
    a = RAM(OBJ_TIMER3 + x);
    t = a; c = t & 1; t >>= 1; a = t;
    if (c) { mff_cycles(0xACCE, 1); goto L_ACD5; }
    mff_cycles(0xACD0, 4);
    a = nes_read((uint16_t)(0xAD84 + y));
    z = (a) == 0;
    mff_cycles(0xACD3, 2);
    if (!z) { mff_cycles(0xACD3, 1); goto L_ACD8; }
L_ACD5:;
    mff_cycles(0xACD5, 4);
    a = nes_read((uint16_t)(0xAD88 + y));
L_ACD8:;
    mff_cycles(0xACD8, 21);
    RAM(0x01) = a;
    a = RAM(OBJ_DIR + x);
    t = a; c = t & 1; t >>= 1; a = t;
    t = a; c = t & 1; t >>= 1; a = t;
    t = a; c = t & 1; t >>= 1; a = t;
    a ^= RAM(OBJ_AI_TIMER + x);
    t = a; c = t & 1; t >>= 1; a = t;
    if (!c) { mff_cycles(0xACE4, 1); goto L_ACFA; }
    mff_cycles(0xACE6, 28);
    a = RAM(OBJ_Y + x);
    c = 1;
    m = RAM(0x01); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    RAM(OBJ_Y + x) = a;
    a = RAM(OBJ_Y_HI + x);
    m = 0x00; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y_HI + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAD0B, 2); call_by_address_tail(0xAD0B, 0); return;
L_ACFA:;
    mff_cycles(0xACFA, 25);
    a = RAM(OBJ_Y + x);
    c = 0;
    m = RAM(0x01); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(OBJ_Y + x) = a;
    a = RAM(OBJ_Y_HI + x);
    m = 0x00; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(OBJ_Y_HI + x) = a;
L_AD0B:;
    mff_cycles(0xAD0B, 24);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) + 1); RAM(OBJ_AI_TIMER + x) = t;
    a = RAM(OBJ_AI_TIMER + x);
    a &= 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0D18, 0xA031, 0)) return;
    SYNC_IN;
    mff_cycles(0xAD19, 5);
    a = RAM(0x0F);
    z = (a) == 0;
    if (!z) { mff_cycles(0xAD1B, 1); goto L_AD42; }
    mff_cycles(0xAD1D, 10);
    a = RAM(OBJ_X_HI + x);
    m = RAM(OBJ_X_HI); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xAD23, 1); goto L_AD41; }
    mff_cycles(0xAD25, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    z = (a) == 0;
    if (!z) { mff_cycles(0xAD2A, 1); goto L_AD33; }
    mff_cycles(0xAD2C, 8);
    a = RAM(OBJ_X + x);
    m = 0x18; c = a >= m;
    if (!c) { mff_cycles(0xAD31, 1); goto L_AD42; }
L_AD33:;
    mff_cycles(0xAD33, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xAD38, 1); goto L_AD41; }
    mff_cycles(0xAD3A, 8);
    a = RAM(OBJ_X + x);
    m = 0xF1; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xAD3F, 1); goto L_AD42; }
L_AD41:;
    mff_cycles(0xAD41, 6);
    SYNC_OUT;
    mff_rts(); return;
L_AD42:;
    mff_cycles(0xAD42, 27);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = 0x48;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $ACC1 (bank 0) - lifted by port/tools/lift.py */
void func_ACC1_b0(void) {
    REGS_IN;
L_ACC1:;
    mff_cycles(0xACC1, 10);
    a = RAM(OBJ_TIMER3 + x);
    a &= 0x07;
    m = 0x04; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xACC8, 2); goto L_AD0B; }
    mff_cycles(0xACCA, 8);
    a = RAM(OBJ_TIMER3 + x);
    t = a; c = t & 1; t >>= 1; a = t;
    if (c) { mff_cycles(0xACCE, 1); goto L_ACD5; }
    mff_cycles(0xACD0, 4);
    a = nes_read((uint16_t)(0xAD84 + y));
    z = (a) == 0;
    mff_cycles(0xACD3, 2);
    if (!z) { mff_cycles(0xACD3, 1); goto L_ACD8; }
L_ACD5:;
    mff_cycles(0xACD5, 4);
    a = nes_read((uint16_t)(0xAD88 + y));
L_ACD8:;
    mff_cycles(0xACD8, 21);
    RAM(0x01) = a;
    a = RAM(OBJ_DIR + x);
    t = a; c = t & 1; t >>= 1; a = t;
    t = a; c = t & 1; t >>= 1; a = t;
    t = a; c = t & 1; t >>= 1; a = t;
    a ^= RAM(OBJ_AI_TIMER + x);
    t = a; c = t & 1; t >>= 1; a = t;
    if (!c) { mff_cycles(0xACE4, 1); goto L_ACFA; }
    mff_cycles(0xACE6, 28);
    a = RAM(OBJ_Y + x);
    c = 1;
    m = RAM(0x01); w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    RAM(OBJ_Y + x) = a;
    a = RAM(OBJ_Y_HI + x);
    m = 0x00; w = a - m - !c; v = ((a ^ m) & (a ^ w) & 0x80) != 0; c = w >= 0; a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_Y_HI + x) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAD0B, 2); call_by_address_tail(0xAD0B, 0); return;
L_ACFA:;
    mff_cycles(0xACFA, 25);
    a = RAM(OBJ_Y + x);
    c = 0;
    m = RAM(0x01); w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(OBJ_Y + x) = a;
    a = RAM(OBJ_Y_HI + x);
    m = 0x00; w = a + m + c; v = (~(a ^ m) & (a ^ w) & 0x80) != 0; c = w > 0xFF; a = (uint8_t)w;
    RAM(OBJ_Y_HI + x) = a;
L_AD0B:;
    mff_cycles(0xAD0B, 24);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) + 1); RAM(OBJ_AI_TIMER + x) = t;
    a = RAM(OBJ_AI_TIMER + x);
    a &= 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0D18, 0xA031, 0)) return;
    SYNC_IN;
    mff_cycles(0xAD19, 5);
    a = RAM(0x0F);
    z = (a) == 0;
    if (!z) { mff_cycles(0xAD1B, 1); goto L_AD42; }
    mff_cycles(0xAD1D, 10);
    a = RAM(OBJ_X_HI + x);
    m = RAM(OBJ_X_HI); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xAD23, 1); goto L_AD41; }
    mff_cycles(0xAD25, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    z = (a) == 0;
    if (!z) { mff_cycles(0xAD2A, 1); goto L_AD33; }
    mff_cycles(0xAD2C, 8);
    a = RAM(OBJ_X + x);
    m = 0x18; c = a >= m;
    if (!c) { mff_cycles(0xAD31, 1); goto L_AD42; }
L_AD33:;
    mff_cycles(0xAD33, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xAD38, 1); goto L_AD41; }
    mff_cycles(0xAD3A, 8);
    a = RAM(OBJ_X + x);
    m = 0xF1; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xAD3F, 1); goto L_AD42; }
L_AD41:;
    mff_cycles(0xAD41, 6);
    SYNC_OUT;
    mff_rts(); return;
L_AD42:;
    mff_cycles(0xAD42, 27);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = 0x48;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AD0B (bank 0) - lifted by port/tools/lift.py */
void func_AD0B_b0(void) {
    REGS_IN;
L_AD0B:;
    mff_cycles(0xAD0B, 24);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) + 1); RAM(OBJ_AI_TIMER + x) = t;
    a = RAM(OBJ_AI_TIMER + x);
    a &= 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0D18, 0xA031, 0)) return;
    SYNC_IN;
    mff_cycles(0xAD19, 5);
    a = RAM(0x0F);
    z = (a) == 0;
    if (!z) { mff_cycles(0xAD1B, 1); goto L_AD42; }
    mff_cycles(0xAD1D, 10);
    a = RAM(OBJ_X_HI + x);
    m = RAM(OBJ_X_HI); c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xAD23, 1); goto L_AD41; }
    mff_cycles(0xAD25, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x01;
    z = (a) == 0;
    if (!z) { mff_cycles(0xAD2A, 1); goto L_AD33; }
    mff_cycles(0xAD2C, 8);
    a = RAM(OBJ_X + x);
    m = 0x18; c = a >= m;
    if (!c) { mff_cycles(0xAD31, 1); goto L_AD42; }
L_AD33:;
    mff_cycles(0xAD33, 8);
    a = RAM(OBJ_DIR + x);
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xAD38, 1); goto L_AD41; }
    mff_cycles(0xAD3A, 8);
    a = RAM(OBJ_X + x);
    m = 0xF1; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xAD3F, 1); goto L_AD42; }
L_AD41:;
    mff_cycles(0xAD41, 6);
    SYNC_OUT;
    mff_rts(); return;
L_AD42:;
    mff_cycles(0xAD42, 27);
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    a = 0x00;
    RAM(OBJ_ATTACK + x) = a;
    a = 0x48;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_AI_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AD50 (bank 0) - lifted by port/tools/lift.py */
void func_AD50_b0(void) {
    REGS_IN;
L_AD50:;
    mff_cycles(0xAD50, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0D52, 0xA83A, 0)) return;
    SYNC_IN;
    mff_cycles(0xAD53, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1); RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) { mff_cycles(0xAD56, 1); goto L_AD5D; }
    mff_cycles(0xAD58, 7);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
L_AD5D:;
    mff_cycles(0xAD5D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AD5E (bank 0) - lifted by port/tools/lift.py */
void func_AD5E_b0(void) {
    REGS_IN;
L_AD5E:;
    mff_cycles(0xAD5E, 8);
    a = RAM(OBJ_AI_TIMER + x);
    m = 0x40; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xAD63, 1); goto L_AD6B; }
    mff_cycles(0xAD65, 6);
    a = 0xFA;
    y = 0x2E;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0xAD69, 1); goto L_AD72; }
L_AD6B:;
    mff_cycles(0xAD6B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D6D, func_F099, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAD6E, 4);
    a = 0xF1;
    y = 0x31;
    n = (y) >> 7;
    z = (y) == 0;
L_AD72:;
    mff_cycles(0xAD72, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D74, func_F449, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAD75, 9);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1); RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) { mff_cycles(0xAD78, 1); goto L_AD7F; }
    mff_cycles(0xAD7A, 7);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
L_AD7F:;
    mff_cycles(0xAD7F, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AD8C (bank 0) - lifted by port/tools/lift.py */
void func_AD8C_b0(void) {
    REGS_IN;
L_AD8C:;
    mff_cycles(0xAD8C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D8E, func_F343, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAD8F, 9);
    t = (uint8_t)(RAM(OBJ_HIT_TIMER + x) - 1); RAM(OBJ_HIT_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (!z) { mff_cycles(0xAD92, 1); goto L_ADB9; }
    mff_cycles(0xAD94, 8);
    a = RAM(OBJ_TIER + x);
    m = 0x01; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xAD99, 1); goto L_ADA3; }
    mff_cycles(0xAD9B, 4);
    m = 0x02; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xAD9D, 1); goto L_ADAB; }
    mff_cycles(0xAD9F, 4);
    m = 0x03; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xADA1, 1); goto L_ADAF; }
L_ADA3:;
    mff_cycles(0xADA3, 4);
    m = 0x02; c = a >= m;
    if (c) { mff_cycles(0xADA5, 1); goto L_ADAF; }
    mff_cycles(0xADA7, 4);
    a = 0x84;
    z = (a) == 0;
    if (!z) { mff_cycles(0xADA9, 1); goto L_ADB1; }
L_ADAB:;
    mff_cycles(0xADAB, 4);
    a = 0x80;
    z = (a) == 0;
    if (!z) { mff_cycles(0xADAD, 1); goto L_ADB1; }
L_ADAF:;
    mff_cycles(0xADAF, 2);
    a = 0x83;
L_ADB1:;
    mff_cycles(0xADB1, 12);
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
L_ADB9:;
    mff_cycles(0xADB9, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $ADBA (bank 0) - lifted by port/tools/lift.py */
void func_ADBA_b0(void) {
    REGS_IN;
L_ADBA:;
    mff_cycles(0xADBA, 6);
    a = RAM(OBJ_TIMER + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xADBD, 1); goto L_ADCA; }
    mff_cycles(0xADBF, 17);
    a = 0x18;
    RAM(OBJ_AI_TIMER + x) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA83A, 2); call_by_address_tail(0xA83A, 0); return;
L_ADCA:;
    mff_cycles(0xADCA, 6);
    a = RAM(OBJ_AI_TIMER + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xADCD, 1); goto L_ADD3; }
    mff_cycles(0xADCF, 13);
    t = (uint8_t)(RAM(OBJ_AI_TIMER + x) - 1); RAM(OBJ_AI_TIMER + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    mff_rts(); return;
L_ADD3:;
    mff_cycles(0xADD3, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0DD5, func_F374, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xADD6, 4);
    m = 0x28; c = a >= m;
    if (c) { mff_cycles(0xADD8, 1); goto L_ADEE; }
    mff_cycles(0xADDA, 4);
    a = 0x00;
    z = (a) == 0;
    if (z) { mff_cycles(0xADDC, 2); goto L_AE00; }
    mff_cycles(0xADDE, 4);
    m = 0x3C; c = a >= m;
    if (c) { mff_cycles(0xADE0, 1); goto L_ADE6; }
    mff_cycles(0xADE2, 4);
    a = 0x0A;
    z = (a) == 0;
    if (!z) { mff_cycles(0xADE4, 2); goto L_AE00; }
L_ADE6:;
    mff_cycles(0xADE6, 4);
    m = 0x50; c = a >= m;
    if (c) { mff_cycles(0xADE8, 1); goto L_ADEE; }
    mff_cycles(0xADEA, 4);
    a = 0x0B;
    z = (a) == 0;
    if (!z) { mff_cycles(0xADEC, 2); goto L_AE00; }
L_ADEE:;
    mff_cycles(0xADEE, 4);
    m = 0x64; c = a >= m;
    if (c) { mff_cycles(0xADF0, 1); goto L_ADF6; }
    mff_cycles(0xADF2, 4);
    a = 0x0C;
    z = (a) == 0;
    if (!z) { mff_cycles(0xADF4, 2); goto L_AE00; }
L_ADF6:;
    mff_cycles(0xADF6, 4);
    m = 0x78; c = a >= m;
    if (c) { mff_cycles(0xADF8, 1); goto L_ADFE; }
    mff_cycles(0xADFA, 4);
    a = 0x0D;
    z = (a) == 0;
    if (!z) { mff_cycles(0xADFC, 2); goto L_AE00; }
L_ADFE:;
    mff_cycles(0xADFE, 2);
    a = 0x0E;
L_AE00:;
    mff_cycles(0xAE00, 13);
    RAM(0x00) = a;
    a = RAM(RNG);
    a &= 0x0F;
    m = RAM(0x00); c = a >= m;
    if (c) { mff_cycles(0xAE08, 1); goto L_AE15; }
    mff_cycles(0xAE0A, 2);
    a = 0x84;
L_AE0C:;
    mff_cycles(0xAE0C, 18);
    RAM(OBJ_STATE + x) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_AE15:;
    mff_cycles(0xAE15, 4);
    a = 0x81;
    z = (a) == 0;
    if (!z) { mff_cycles(0xAE0C, 1); goto L_AE0C; }
    mff_cycles(0xAE19, 6);
    a = RAM(OBJ_TIMER + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xAE1C, 1); goto L_AE31; }
    mff_cycles(0xAE1E, 26);
    a = 0x20;
    RAM(PLAYER_INVULN) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    a = 0x27;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = 0xF6;
    y = 0x2F;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF44F, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F44F(); g_code_window_base = swb; } return;
L_AE31:;
    mff_cycles(0xAE31, 15);
    a = 0x98;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x09; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xAE3B, 1); goto L_AE4A; }
    mff_cycles(0xAE3D, 19);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    RAM(OBJ_HIT_PLAYER + x) = a;
    a = 0x81;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
L_AE4A:;
    mff_cycles(0xAE4A, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AE19 (bank 0) - lifted by port/tools/lift.py */
void func_AE19_b0(void) {
    REGS_IN;
L_AE19:;
    mff_cycles(0xAE19, 6);
    a = RAM(OBJ_TIMER + x);
    z = (a) == 0;
    if (!z) { mff_cycles(0xAE1C, 1); goto L_AE31; }
    mff_cycles(0xAE1E, 26);
    a = 0x20;
    RAM(PLAYER_INVULN) = a;
    t = (uint8_t)(RAM(OBJ_TIMER + x) + 1); RAM(OBJ_TIMER + x) = t;
    a = 0x27;
    RAM(OBJ_ATTACK_DAMAGE + x) = a;
    a = 0xF6;
    y = 0x2F;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xF44F, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F44F(); g_code_window_base = swb; } return;
L_AE31:;
    mff_cycles(0xAE31, 15);
    a = 0x98;
    RAM(OBJ_ATTACK + x) = a;
    a = RAM(OBJ_ANIM_FRAME + x);
    m = 0x09; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xAE3B, 1); goto L_AE4A; }
    mff_cycles(0xAE3D, 19);
    a = 0x00;
    RAM(OBJ_TIMER + x) = a;
    RAM(OBJ_HIT_PLAYER + x) = a;
    a = 0x81;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE + x) = a;
L_AE4A:;
    mff_cycles(0xAE4A, 6);
    SYNC_OUT;
    mff_rts(); return;
}
