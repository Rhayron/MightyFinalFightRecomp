/*
 * gameplay.c - the gameplay frame (docs/engine.md, "The gameplay frame")
 *
 * Lifted by port/tools/lift.py from the pure scaffold; to be tidied.
 */
#include "mff.h"
#include "state.h"
#include "coroutine.h"

void func_E2A3(void);
void func_E6AB(void);
void func_E9BC(void);
void func_F34C(void);
void func_FF31(void);
void func_FF4D(void);

/* gameplay $C8ED - lifted by port/tools/lift.py */
void func_C8ED(void) {
    REGS_IN;
L_C8ED:;
    mff_cycles(0xC8ED, 7);
    a = RAM(GAME_ACTION);
    m = 0x09; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xC8F1, 1); goto L_C8F6; }
    mff_cycles(0xC8F3, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC93B, 2); call_by_address_tail(0xC93B, -1); return;
L_C8F6:;
    mff_cycles(0xC8F6, 5);
    a = RAM(0xB7);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xC8F8, 1); goto L_C8FD; }
    mff_cycles(0xC8FA, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC9A7, 2); call_by_address_tail(0xC9A7, -1); return;
L_C8FD:;
    mff_cycles(0xC8FD, 5);
    a = RAM(0x62);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xC8FF, 1); goto L_C904; }
    mff_cycles(0xC901, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC987, 2); call_by_address_tail(0xC987, -1); return;
L_C904:;
    mff_cycles(0xC904, 7);
    a = RAM(JOY_PRESSED);
    a &= 0x10;
    z = (a) == 0;
    if (z) { mff_cycles(0xC908, 1); goto L_C934; }
    mff_cycles(0xC90A, 7);
    a = RAM(GAME_ACTION);
    m = 0x1B; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xC90E, 1); goto L_C93B; }
    mff_cycles(0xC910, 4);
    m = 0x1C; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xC912, 1); goto L_C93B; }
    mff_cycles(0xC914, 4);
    m = 0x1E; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xC916, 1); goto L_C93B; }
    mff_cycles(0xC918, 4);
    m = 0x19; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xC91A, 1); goto L_C93B; }
    mff_cycles(0xC91C, 4);
    m = 0x28; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xC91E, 1); goto L_C93B; }
    mff_cycles(0xC920, 8);
    a = 0x15;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0924, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC925, 12);
    y = 0xF3;
    a = RAM(PAUSED);
    a ^= 0x01;
    z = (a) == 0;
    RAM(PAUSED) = a;
    if (!z) { mff_cycles(0xC92D, 1); goto L_C930; }
    mff_cycles(0xC92F, 2);
    y++;
L_C930:;
    mff_cycles(0xC930, 8);
    a = y;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0933, func_F34C, 0xE000)) return;
    SYNC_IN;
L_C934:;
    mff_cycles(0xC934, 5);
    a = RAM(PAUSED);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xC936, 1); goto L_C93B; }
    mff_cycles(0xC938, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC9A7, 2); call_by_address_tail(0xC9A7, -1); return;
L_C93B:;
    mff_cycles(0xC93B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x093D, 0xC9B6, -1)) return;
    SYNC_IN;
    mff_cycles(0xC93E, 16);
    x = 0x00;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0947, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC948, 6);
    SYNC_OUT;
    if (!mff_call_at(0x094A, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC94B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x094D, func_E2A3, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC94E, 29);
    a = RAM(OBJ_X);
    RAM(0x27) = a;
    a = RAM(SCROLL_X);
    RAM(0x25) = a;
    x = 0x08;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0960, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC961, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0963, 0x8080, -1)) return;
    SYNC_IN;
    mff_cycles(0xC964, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0966, 0x8900, -1)) return;
    SYNC_IN;
    mff_cycles(0xC967, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0969, 0xA300, -1)) return;
    SYNC_IN;
    mff_cycles(0xC96A, 16);
    x = 0x0C;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0973, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC974, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0976, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC977, 7);
    a = RAM(GAME_ACTION);
    m = 0x07; c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xC97B, 1); goto L_C99D; }
    mff_cycles(0xC97D, 4);
    m = 0x09; c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xC97F, 1); goto L_C99D; }
    mff_cycles(0xC981, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0983, func_E6AB, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC984, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC99D, 2); call_by_address_tail(0xC99D, -1); return;
L_C99D:;
    mff_cycles(0xC99D, 11);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(0x97) = a;
    SYNC_OUT;
    if (!mff_call_at(0x09A3, 0xC31D, -1)) return;
    SYNC_IN;
    mff_cycles(0xC9A4, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x09A6, func_E9BC, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC9A7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x09A9, 0xC3B4, -1)) return;
    SYNC_IN;
    mff_cycles(0xC9AA, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(RENDER_OFF_DEPTH) = a;
    SYNC_OUT;
    mff_call_fn(0x09B0, func_FF31, 0xE000);
    SYNC_IN;
    mff_cycles(0xC9B1, 8);
    t = (uint8_t)(RAM(RENDER_OFF_DEPTH) + 1); RAM(RENDER_OFF_DEPTH) = t;
    goto L_C8ED;
}

/* gameplay_frame_end $C9A7 - lifted by port/tools/lift.py */
void func_C9A7(void) {
    REGS_IN;
L_C9A7:;
    mff_cycles(0xC9A7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x09A9, 0xC3B4, -1)) return;
    SYNC_IN;
    mff_cycles(0xC9AA, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(RENDER_OFF_DEPTH) = a;
    SYNC_OUT;
    mff_call_fn(0x09B0, func_FF31, 0xE000);
    SYNC_IN;
    mff_cycles(0xC9B1, 8);
    t = (uint8_t)(RAM(RENDER_OFF_DEPTH) + 1); RAM(RENDER_OFF_DEPTH) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC8ED, 2); call_by_address_tail(0xC8ED, -1); return;
}

