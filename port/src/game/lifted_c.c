/*
 * lifted_c.c - fixed-bank routines $C000-$CFFF, lifted by
 * port/tools/lift_bank.py from the pure scaffold: exact, still to be
 * tidied and moved into their systems' files.
 */
#include "mff.h"
#include "state.h"
#include "coroutine.h"

void func_E521(void);
void func_E9BC(void);
void func_F000(void);
void func_F051(void);
void func_F090(void);
void func_F099(void);
void func_F0A2(void);
void func_F0F7(void);
void func_F119(void);
void func_F183(void);
void func_F1A1(void);
void func_F26E(void);
void func_F294(void);
void func_F34A(void);
void func_F34C(void);
void func_F436(void);
void func_F43B(void);
void func_F440(void);
void func_FE00(void);
void func_FF2A(void);
void func_FF31(void);
void func_FF4C(void);
void func_FF4D(void);

/* main_task $C556 - lifted by port/tools/lift.py */
void func_C556(void) {
    REGS_IN;
    mff_cycles(0xC556, 11);
    x = 0xBF;
    g_cpu.S = x;
    a = RAM(PPUCTRL_SHADOW);
    hw_write(PPU_CTRL, a);
    mff_cycles(0xC55E, 8);
    a = 0xF0;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0562, func_F34A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC563, 6);
    SYNC_OUT;
    mff_call_fn(0x0565, func_FF31, 0xE000);
    SYNC_IN;
    mff_cycles(0xC566, 11);
    a = 0x88;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(RNG) = a;
    SYNC_OUT;
    if (!mff_call_at(0x056C, 0xC85C, -1)) return;
    SYNC_IN;
    mff_cycles(0xC56D, 16);
    x = 0x04;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0576, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC577, 8);
    x = 0x04;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x057B, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC57C, 6);
    SYNC_OUT;
    if (!mff_call_at(0x057E, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xC57F, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0581, 0x89C0, -1)) return;
    SYNC_IN;
    mff_cycles(0xC582, 8);
    x = 0xB4;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    mff_call_fn(0x0586, func_FF2A, 0xE000);
    SYNC_IN;
    mff_cycles(0xC587, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0589, 0xC85C, -1)) return;
    SYNC_IN;
    mff_cycles(0xC58A, 16);
    x = 0x04;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0593, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC594, 8);
    x = 0x00;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x0598, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC599, 6);
    SYNC_OUT;
    if (!mff_call_at(0x059B, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xC59C, 6);
    SYNC_OUT;
    if (!mff_call_at(0x059E, 0x8900, -1)) return;
    SYNC_IN;
    mff_cycles(0xC59F, 5);
    a = 0xB4;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ACTION_TIMER) = a;
L_C5A3:;
    mff_cycles(0xC5A3, 6);
    SYNC_OUT;
    mff_call_fn(0x05A5, func_FF31, 0xE000);
    SYNC_IN;
    mff_cycles(0xC5A6, 5);
    a = RAM(JOY_PRESSED);
    if (a != 0) { mff_cycles(0xC5A8, 1); goto L_C5B3; }
    mff_cycles(0xC5AA, 5);
    a = RAM(ACTION_TIMER);
    if (a == 0) { mff_cycles(0xC5AC, 1); goto L_C5B3; }
    mff_cycles(0xC5AE, 8);
    t = (uint8_t)(RAM(ACTION_TIMER) - 1);
    RAM(ACTION_TIMER) = t;
    n = (t) >> 7;
    z = (t) == 0;
    goto L_C5A3;
L_C5B3:;
    mff_cycles(0xC5B3, 6);
    x = 0x0F;
    a = prg_read(0xFFF8);
    mff_cycles(0xC5B8, 4);
    m = 0x08;
    c = a >= m;
    if (a != m) { mff_cycles(0xC5BA, 1); goto L_C5CA; }
L_C5BC:;
    mff_cycles(0xC5BC, 4);
    a = prg_read((uint16_t)(0xFFE0 + x));
    mff_cycles(0xC5BF, 4);
    m = prg_read((uint16_t)(0xC8DD + x));
    c = a >= m;
    mff_cycles(0xC5C2, 2);
    if (a != m) { mff_cycles(0xC5C2, 1); goto L_C5CA; }
    mff_cycles(0xC5C4, 4);
    x--;
    n = (x) >> 7;
    z = (x) == 0;
    if (!n) { mff_cycles(0xC5BC, 1); goto L_C5BC; }
    mff_cycles(0xC5C7, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC5CE, 2); call_by_address_tail(0xC5CE, -1); return;
L_C5CA:;
    for (;;) {
        mff_cycles(0xC5CA, 4);
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        if (!z) break;
        mff_cycles(0xC5CA, 1);
    }
    mff_cycles(0xC5CE, 6);
    SYNC_OUT;
    if (!mff_call_at(0x05D0, 0xC888, -1)) return;
    SYNC_IN;
    mff_cycles(0xC5D1, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(INTRO_PAGE) = a;
    SYNC_OUT;
    if (!mff_call_at(0x05D7, 0x9A00, -1)) return;
    SYNC_IN;
    mff_cycles(0xC5D8, 5);
    a = RAM(JOY_HELD);
    if (a == 0) {
        mff_cycles(0xC5DC, 8);
        a = 0x07;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x05E0, func_F34A, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xC5E1, 6);
        SYNC_OUT;
        if (!mff_call_at(0x05E3, 0xC85C, -1)) return;
        SYNC_IN;
        mff_cycles(0xC5E4, 16);
        x = 0x04;
        RAM(BANK_REQUEST) = x;
        x++;
        n = (x) >> 7;
        z = (x) == 0;
        RAM(BANK_REQUEST + 1) = x;
        SYNC_OUT;
        if (!mff_call_fn(0x05ED, func_FF4D, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xC5EE, 8);
        x = 0x06;
        n = (x) >> 7;
        z = (x) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x05F2, 0x8000, -1)) return;
        SYNC_IN;
        mff_cycles(0xC5F3, 6);
        SYNC_OUT;
        if (!mff_call_at(0x05F5, 0xC26F, -1)) return;
        SYNC_IN;
        mff_cycles(0xC5F6, 6);
        SYNC_OUT;
        if (!mff_call_at(0x05F8, 0x8B40, -1)) return;
        SYNC_IN;
        mff_cycles(0xC5F9, 16);
        x = 0x00;
        RAM(BANK_REQUEST) = x;
        x++;
        n = (x) >> 7;
        z = (x) == 0;
        RAM(BANK_REQUEST + 1) = x;
        SYNC_OUT;
        if (!mff_call_fn(0x0602, func_FF4D, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xC603, 11);
        a = 0x01;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(INTRO_PAGE) = a;
        SYNC_OUT;
        if (!mff_call_at(0x0609, 0x9A00, -1)) return;
        SYNC_IN;
        mff_cycles(0xC60A, 5);
        a = RAM(JOY_HELD);
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0xC60E, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0610, 0xC888, -1)) return;
            SYNC_IN;
            mff_cycles(0xC611, 11);
            a = 0x02;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(INTRO_PAGE) = a;
            SYNC_OUT;
            if (!mff_call_at(0x0617, 0x9A00, -1)) return;
            SYNC_IN;
            mff_cycles(0xC618, 5);
            a = RAM(JOY_HELD);
            n = (a) >> 7;
            z = (a) == 0;
            if (z) {
                mff_cycles(0xC61C, 6);
                SYNC_OUT;
                if (!mff_call_at(0x061E, 0xC85C, -1)) return;
                SYNC_IN;
                mff_cycles(0xC61F, 16);
                x = 0x04;
                RAM(BANK_REQUEST) = x;
                x++;
                n = (x) >> 7;
                z = (x) == 0;
                RAM(BANK_REQUEST + 1) = x;
                SYNC_OUT;
                if (!mff_call_fn(0x0628, func_FF4D, 0xE000)) return;
                SYNC_IN;
                mff_cycles(0xC629, 8);
                x = 0x07;
                n = (x) >> 7;
                z = (x) == 0;
                SYNC_OUT;
                if (!mff_call_at(0x062D, 0x8000, -1)) return;
                SYNC_IN;
                mff_cycles(0xC62E, 6);
                SYNC_OUT;
                if (!mff_call_at(0x0630, 0xC26F, -1)) return;
                SYNC_IN;
                mff_cycles(0xC631, 6);
                SYNC_OUT;
                if (!mff_call_at(0x0633, 0x8AC0, -1)) return;
                SYNC_IN;
                mff_cycles(0xC634, 16);
                x = 0x00;
                RAM(BANK_REQUEST) = x;
                x++;
                n = (x) >> 7;
                z = (x) == 0;
                RAM(BANK_REQUEST + 1) = x;
                SYNC_OUT;
                if (!mff_call_fn(0x063D, func_FF4D, 0xE000)) return;
                SYNC_IN;
                mff_cycles(0xC63E, 11);
                a = 0x03;
                n = (a) >> 7;
                z = (a) == 0;
                RAM(INTRO_PAGE) = a;
                SYNC_OUT;
                if (!mff_call_at(0x0644, 0x9A00, -1)) return;
                SYNC_IN;
                mff_cycles(0xC645, 5);
                a = RAM(JOY_HELD);
                n = (a) >> 7;
                z = (a) == 0;
                if (z) {
                    mff_cycles(0xC649, 6);
                    SYNC_OUT;
                    if (!mff_call_at(0x064B, 0xC888, -1)) return;
                    SYNC_IN;
                    mff_cycles(0xC64C, 11);
                    a = 0x04;
                    n = (a) >> 7;
                    z = (a) == 0;
                    RAM(INTRO_PAGE) = a;
                    SYNC_OUT;
                    if (!mff_call_at(0x0652, 0x9A00, -1)) return;
                    SYNC_IN;
                    mff_cycles(0xC653, 5);
                    a = RAM(JOY_HELD);
                    if (a != 0) {
                        mff_cycles(0xC655, 1);
                    }
                } else {
                    mff_cycles(0xC647, 1);
                }
            } else {
                mff_cycles(0xC61A, 1);
            }
        } else {
            mff_cycles(0xC60C, 1);
        }
    } else {
        mff_cycles(0xC5DA, 2);
    }
    mff_cycles(0xC657, 8);
    a = 0x0C;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x065B, func_F34A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC65C, 6);
    SYNC_OUT;
    if (!mff_call_at(0x065E, 0xC85C, -1)) return;
    SYNC_IN;
    mff_cycles(0xC65F, 16);
    x = 0x04;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0668, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC669, 8);
    x = 0x01;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x066D, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC66E, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0670, 0x8940, -1)) return;
    SYNC_IN;
    mff_cycles(0xC671, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0673, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xC674, 16);
    a = 0xFF;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER2) = a;
    a = 0x8B;
    RAM(OBJ_GROUND_Y) = a;
L_C681:;
    mff_cycles(0xC681, 11);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OAM_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0687, 0xC31D, -1)) return;
    SYNC_IN;
    mff_cycles(0xC688, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x068A, func_E9BC, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC68B, 6);
    SYNC_OUT;
    mff_call_fn(0x068D, func_FF31, 0xE000);
    SYNC_IN;
    mff_cycles(0xC68E, 6);
    a = RAM(OBJ_TIMER);
    if (a != 0) { mff_cycles(0xC691, 1); goto L_C6A7; }
    mff_cycles(0xC693, 6);
    a = RAM(OBJ_TIMER2);
    if (a != 0) { mff_cycles(0xC696, 1); goto L_C6C9; }
    mff_cycles(0xC698, 6);
    a = RAM(OBJ_GROUND_Y);
    if (a != 0) { mff_cycles(0xC69B, 1); goto L_C6CF; }
    mff_cycles(0xC69D, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(RENDER_OFF_DEPTH) = a;
    SYNC_OUT;
    if (!mff_call_at(0x06A3, 0xC36E, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6A4, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xFE00, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_FE00(); g_code_window_base = swb; } return;
L_C6A7:;
    mff_cycles(0xC6A7, 14);
    t = (uint8_t)(RAM(OBJ_TIMER) - 1);
    RAM(OBJ_TIMER) = t;
    a = RAM(OBJ_TIMER);
    m = 0xF0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xC6B1, 8);
        a = 0x32;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x06B5, func_F34C, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xC6B6, 21);
        a = 0x12;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(PALETTE_BUF + 3) = a;
        RAM(PALETTE_BUF + 7) = a;
        RAM(PALETTE_BUF + 15) = a;
        RAM(PALETTE_BUF + 31) = a;
        RAM(PALETTE_REQUEST) = a;
    } else {
        mff_cycles(0xC6AF, 1);
    }
    mff_cycles(0xC6C6, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC6D2, 2); call_by_address_tail(0xC6D2, -1); return;
L_C6C9:;
    mff_cycles(0xC6C9, 9);
    t = (uint8_t)(RAM(OBJ_TIMER2) - 1);
    RAM(OBJ_TIMER2) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC6D2, 2); call_by_address_tail(0xC6D2, -1); return;
L_C6CF:;
    mff_cycles(0xC6CF, 13);
    t = (uint8_t)(RAM(OBJ_GROUND_Y) - 1);
    RAM(OBJ_GROUND_Y) = t;
    a = RAM(JOY_PRESSED);
    a &= 0x10;
    if (a == 0) { mff_cycles(0xC681, 1); goto L_C681; }
    mff_cycles(0xC6D8, 8);
    a = 0x30;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x06DC, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC6DD, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06DF, 0xC85C, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6E0, 16);
    x = 0x04;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x06E9, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC6EA, 8);
    x = 0x02;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x06EE, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6EF, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06F1, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6F2, 14);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PLAYER_ID) = a;
    RAM(SELECT_PLAYER) = a;
    SYNC_OUT;
    if (!mff_call_at(0x06FA, 0x9100, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6FB, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06FD, 0xC85C, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6FE, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0700, 0xC334, -1)) return;
    SYNC_IN;
    mff_cycles(0xC701, 16);
    x = 0x04;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x070A, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC70B, 8);
    x = 0x08;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x070F, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC710, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0712, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xC713, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0715, 0x8D00, -1)) return;
    SYNC_IN;
    mff_cycles(0xC716, 8);
    x = 0x2D;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    mff_call_fn(0x071A, func_FF2A, 0xE000);
    SYNC_IN;
    mff_cycles(0xC71B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x071D, 0xC85C, -1)) return;
    SYNC_IN;
    mff_cycles(0xC71E, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0720, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xC721, 56);
    g_cpu.I = 0;
    a = 0x01;
    RAM(IRQ_ENABLE) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SCROLL_NT) = a;
    RAM(SCROLL_Y_HI) = a;
    RAM(SCROLL_X) = a;
    RAM(SCROLL_Y) = a;
    RAM(SCROLL_X_HI) = a;
    RAM(SCROLL_X_PREV) = a;
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y_HI) = a;
    RAM(SPAWN_LIST_NEXT) = a;
    RAM(SPAWN_LIST_PREV) = a;
    RAM(UNK_5A) = a;
    RAM(UNK_33) = a;
    RAM(RENDER_OFF_DEPTH) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0746, 0xC334, -1)) return;
    SYNC_IN;
    mff_cycles(0xC747, 6);
    a = 0x01;
    hw_write(MMC3_MIRRORING, a);
    mff_cycles(0xC74C, 23);
    a = 0x01;
    RAM(PLAYER_DIR) = a;
    RAM(SCROLL_MODE) = a;
    RAM(CAMERA_DIR) = a;
    y = 0x00;
    RAM(AREA_INDEX) = y;
    RAM(ROUND_TABLES) = y;
    a = prg_read((uint16_t)(0xC83C + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xC75D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x075F, func_F34A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC760, 10);
    a = 0xFF;
    RAM(BG_SCREEN) = a;
    a = 0x1F;
    RAM(BG_COLUMN) = a;
    for (;;) {
        mff_cycles(0xC768, 11);
        a = 0x01;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(TMP16) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x076E, func_E521, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xC76F, 6);
        SYNC_OUT;
        mff_call_fn(0x0771, func_FF31, 0xE000);
        SYNC_IN;
        mff_cycles(0xC772, 5);
        a = RAM(BG_SCREEN);
        if (a != 0) break;
        mff_cycles(0xC768, 1);
    }
    mff_cycles(0xC776, 11);
    a = 0x0A;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(BANK_REQUEST) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x077C, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC77D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x077F, 0x9C00, -1)) return;
    SYNC_IN;
    mff_cycles(0xC780, 7);
    y = RAM(ROUND_TABLES);
    a = prg_read((uint16_t)(0xC1F8 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xC785, 9);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0789, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC78A, 4);
    a = prg_read(0xAE68);
    mff_cycles(0xC78D, 31);
    mff_push(a);
    a &= 0xE0;
    RAM(AREA_FLAGS) = a;
    a = mff_pull();
    a &= 0x1F;
    RAM(AREA_SCREEN_COUNT) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(AREA_PALETTE) = a;
    RAM(AREA_SCREEN) = a;
    SYNC_OUT;
    if (!mff_call_at(0x079F, 0xC3DD, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7A0, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PALETTE_REQUEST) = a;
    SYNC_OUT;
    if (!mff_call_at(0x07A6, 0xC362, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7A7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07A9, 0xC334, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7AA, 49);
    x = 0x00;
    a = 0xD0;
    RAM(OBJ_FLAGS) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0x30;
    RAM(OBJ_X) = a;
    a = 0x90;
    RAM(OBJ_Y) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y_HI) = a;
    RAM(OBJ_ID) = a;
    RAM(AREA_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x07CF, func_F26E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC7D0, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xC841 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xC7D5, 9);
    RAM(HEALTH) = a;
    SYNC_OUT;
    if (!mff_call_at(0x07D9, 0xC8B4, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7DA, 16);
    x = 0x08;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x07E3, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC7E4, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07E6, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7E7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07E9, 0x8038, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7EA, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07EC, 0x8080, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7ED, 112);
    a = 0x05;
    RAM(LIVES) = a;
    a = 0x03;
    RAM(CREDITS) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(COMBO_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(UNK_A5) = a;
    RAM(ACTION_TIMER) = a;
    RAM(ROUND) = a;
    RAM(SUB_AREA) = a;
    RAM(PLAYER_LEVEL) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(PLAYER_EXP) = a;
    RAM(PLAYER_EXP + 1) = a;
    RAM(WEAPON_HELD) = a;
    RAM(WEAPON_AMMO) = a;
    RAM(RESPAWN_TIMER2) = a;
    RAM(RESPAWN_TIMER1) = a;
    RAM(CAMERA_LOCK) = a;
    RAM(AREA_CLEAR) = a;
    RAM(AREA_KILLS) = a;
    RAM(SPECIAL_SCREEN_ID) = a;
    RAM(SCREEN_EVENT) = a;
    RAM(UNK_B6) = a;
    RAM(UNK_B2) = a;
    RAM(GAME_FINISHED) = a;
    RAM(CAMERA_AUTOSCROLL) = a;
    RAM(KEEP_ON_SCREEN) = a;
    RAM(ACTION_BEFORE_FALL) = a;
    RAM(GRAB_HIT_COUNT) = a;
    RAM(SPAWN_GROUPS) = a;
    RAM(SPAWN_GROUPS + 1) = a;
    RAM(SPAWN_GROUPS + 2) = a;
    RAM(CONTINUE_COUNT) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC8ED, 2); call_by_address_tail(0xC8ED, -1); return;
}

/*  $C5A3 - lifted by port/tools/lift.py */
void func_C5A3(void) {
    REGS_IN;
L_C5A3:;
    mff_cycles(0xC5A3, 6);
    SYNC_OUT;
    mff_call_fn(0x05A5, func_FF31, 0xE000);
    SYNC_IN;
    mff_cycles(0xC5A6, 5);
    a = RAM(JOY_PRESSED);
    if (a != 0) { mff_cycles(0xC5A8, 1); goto L_C5B3; }
    mff_cycles(0xC5AA, 5);
    a = RAM(ACTION_TIMER);
    if (a == 0) { mff_cycles(0xC5AC, 1); goto L_C5B3; }
    mff_cycles(0xC5AE, 8);
    t = (uint8_t)(RAM(ACTION_TIMER) - 1);
    RAM(ACTION_TIMER) = t;
    n = (t) >> 7;
    z = (t) == 0;
    goto L_C5A3;
L_C5B3:;
    mff_cycles(0xC5B3, 6);
    x = 0x0F;
    a = prg_read(0xFFF8);
    mff_cycles(0xC5B8, 4);
    m = 0x08;
    c = a >= m;
    if (a != m) { mff_cycles(0xC5BA, 1); goto L_C5CA; }
L_C5BC:;
    mff_cycles(0xC5BC, 4);
    a = prg_read((uint16_t)(0xFFE0 + x));
    mff_cycles(0xC5BF, 4);
    m = prg_read((uint16_t)(0xC8DD + x));
    c = a >= m;
    mff_cycles(0xC5C2, 2);
    if (a != m) { mff_cycles(0xC5C2, 1); goto L_C5CA; }
    mff_cycles(0xC5C4, 4);
    x--;
    n = (x) >> 7;
    z = (x) == 0;
    if (!n) { mff_cycles(0xC5BC, 1); goto L_C5BC; }
    mff_cycles(0xC5C7, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC5CE, 2); call_by_address_tail(0xC5CE, -1); return;
L_C5CA:;
    for (;;) {
        mff_cycles(0xC5CA, 4);
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        if (!z) break;
        mff_cycles(0xC5CA, 1);
    }
    mff_cycles(0xC5CE, 6);
    SYNC_OUT;
    if (!mff_call_at(0x05D0, 0xC888, -1)) return;
    SYNC_IN;
    mff_cycles(0xC5D1, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(INTRO_PAGE) = a;
    SYNC_OUT;
    if (!mff_call_at(0x05D7, 0x9A00, -1)) return;
    SYNC_IN;
    mff_cycles(0xC5D8, 5);
    a = RAM(JOY_HELD);
    if (a == 0) {
        mff_cycles(0xC5DC, 8);
        a = 0x07;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x05E0, func_F34A, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xC5E1, 6);
        SYNC_OUT;
        if (!mff_call_at(0x05E3, 0xC85C, -1)) return;
        SYNC_IN;
        mff_cycles(0xC5E4, 16);
        x = 0x04;
        RAM(BANK_REQUEST) = x;
        x++;
        n = (x) >> 7;
        z = (x) == 0;
        RAM(BANK_REQUEST + 1) = x;
        SYNC_OUT;
        if (!mff_call_fn(0x05ED, func_FF4D, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xC5EE, 8);
        x = 0x06;
        n = (x) >> 7;
        z = (x) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x05F2, 0x8000, -1)) return;
        SYNC_IN;
        mff_cycles(0xC5F3, 6);
        SYNC_OUT;
        if (!mff_call_at(0x05F5, 0xC26F, -1)) return;
        SYNC_IN;
        mff_cycles(0xC5F6, 6);
        SYNC_OUT;
        if (!mff_call_at(0x05F8, 0x8B40, -1)) return;
        SYNC_IN;
        mff_cycles(0xC5F9, 16);
        x = 0x00;
        RAM(BANK_REQUEST) = x;
        x++;
        n = (x) >> 7;
        z = (x) == 0;
        RAM(BANK_REQUEST + 1) = x;
        SYNC_OUT;
        if (!mff_call_fn(0x0602, func_FF4D, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xC603, 11);
        a = 0x01;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(INTRO_PAGE) = a;
        SYNC_OUT;
        if (!mff_call_at(0x0609, 0x9A00, -1)) return;
        SYNC_IN;
        mff_cycles(0xC60A, 5);
        a = RAM(JOY_HELD);
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0xC60E, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0610, 0xC888, -1)) return;
            SYNC_IN;
            mff_cycles(0xC611, 11);
            a = 0x02;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(INTRO_PAGE) = a;
            SYNC_OUT;
            if (!mff_call_at(0x0617, 0x9A00, -1)) return;
            SYNC_IN;
            mff_cycles(0xC618, 5);
            a = RAM(JOY_HELD);
            n = (a) >> 7;
            z = (a) == 0;
            if (z) {
                mff_cycles(0xC61C, 6);
                SYNC_OUT;
                if (!mff_call_at(0x061E, 0xC85C, -1)) return;
                SYNC_IN;
                mff_cycles(0xC61F, 16);
                x = 0x04;
                RAM(BANK_REQUEST) = x;
                x++;
                n = (x) >> 7;
                z = (x) == 0;
                RAM(BANK_REQUEST + 1) = x;
                SYNC_OUT;
                if (!mff_call_fn(0x0628, func_FF4D, 0xE000)) return;
                SYNC_IN;
                mff_cycles(0xC629, 8);
                x = 0x07;
                n = (x) >> 7;
                z = (x) == 0;
                SYNC_OUT;
                if (!mff_call_at(0x062D, 0x8000, -1)) return;
                SYNC_IN;
                mff_cycles(0xC62E, 6);
                SYNC_OUT;
                if (!mff_call_at(0x0630, 0xC26F, -1)) return;
                SYNC_IN;
                mff_cycles(0xC631, 6);
                SYNC_OUT;
                if (!mff_call_at(0x0633, 0x8AC0, -1)) return;
                SYNC_IN;
                mff_cycles(0xC634, 16);
                x = 0x00;
                RAM(BANK_REQUEST) = x;
                x++;
                n = (x) >> 7;
                z = (x) == 0;
                RAM(BANK_REQUEST + 1) = x;
                SYNC_OUT;
                if (!mff_call_fn(0x063D, func_FF4D, 0xE000)) return;
                SYNC_IN;
                mff_cycles(0xC63E, 11);
                a = 0x03;
                n = (a) >> 7;
                z = (a) == 0;
                RAM(INTRO_PAGE) = a;
                SYNC_OUT;
                if (!mff_call_at(0x0644, 0x9A00, -1)) return;
                SYNC_IN;
                mff_cycles(0xC645, 5);
                a = RAM(JOY_HELD);
                n = (a) >> 7;
                z = (a) == 0;
                if (z) {
                    mff_cycles(0xC649, 6);
                    SYNC_OUT;
                    if (!mff_call_at(0x064B, 0xC888, -1)) return;
                    SYNC_IN;
                    mff_cycles(0xC64C, 11);
                    a = 0x04;
                    n = (a) >> 7;
                    z = (a) == 0;
                    RAM(INTRO_PAGE) = a;
                    SYNC_OUT;
                    if (!mff_call_at(0x0652, 0x9A00, -1)) return;
                    SYNC_IN;
                    mff_cycles(0xC653, 5);
                    a = RAM(JOY_HELD);
                    if (a != 0) {
                        mff_cycles(0xC655, 1);
                    }
                } else {
                    mff_cycles(0xC647, 1);
                }
            } else {
                mff_cycles(0xC61A, 1);
            }
        } else {
            mff_cycles(0xC60C, 1);
        }
    } else {
        mff_cycles(0xC5DA, 2);
    }
    mff_cycles(0xC657, 8);
    a = 0x0C;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x065B, func_F34A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC65C, 6);
    SYNC_OUT;
    if (!mff_call_at(0x065E, 0xC85C, -1)) return;
    SYNC_IN;
    mff_cycles(0xC65F, 16);
    x = 0x04;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0668, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC669, 8);
    x = 0x01;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x066D, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC66E, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0670, 0x8940, -1)) return;
    SYNC_IN;
    mff_cycles(0xC671, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0673, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xC674, 16);
    a = 0xFF;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER2) = a;
    a = 0x8B;
    RAM(OBJ_GROUND_Y) = a;
L_C681:;
    mff_cycles(0xC681, 11);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OAM_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0687, 0xC31D, -1)) return;
    SYNC_IN;
    mff_cycles(0xC688, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x068A, func_E9BC, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC68B, 6);
    SYNC_OUT;
    mff_call_fn(0x068D, func_FF31, 0xE000);
    SYNC_IN;
    mff_cycles(0xC68E, 6);
    a = RAM(OBJ_TIMER);
    if (a != 0) { mff_cycles(0xC691, 1); goto L_C6A7; }
    mff_cycles(0xC693, 6);
    a = RAM(OBJ_TIMER2);
    if (a != 0) { mff_cycles(0xC696, 1); goto L_C6C9; }
    mff_cycles(0xC698, 6);
    a = RAM(OBJ_GROUND_Y);
    if (a != 0) { mff_cycles(0xC69B, 1); goto L_C6CF; }
    mff_cycles(0xC69D, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(RENDER_OFF_DEPTH) = a;
    SYNC_OUT;
    if (!mff_call_at(0x06A3, 0xC36E, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6A4, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xFE00, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_FE00(); g_code_window_base = swb; } return;
L_C6A7:;
    mff_cycles(0xC6A7, 14);
    t = (uint8_t)(RAM(OBJ_TIMER) - 1);
    RAM(OBJ_TIMER) = t;
    a = RAM(OBJ_TIMER);
    m = 0xF0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xC6B1, 8);
        a = 0x32;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x06B5, func_F34C, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xC6B6, 21);
        a = 0x12;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(PALETTE_BUF + 3) = a;
        RAM(PALETTE_BUF + 7) = a;
        RAM(PALETTE_BUF + 15) = a;
        RAM(PALETTE_BUF + 31) = a;
        RAM(PALETTE_REQUEST) = a;
    } else {
        mff_cycles(0xC6AF, 1);
    }
    mff_cycles(0xC6C6, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC6D2, 2); call_by_address_tail(0xC6D2, -1); return;
L_C6C9:;
    mff_cycles(0xC6C9, 9);
    t = (uint8_t)(RAM(OBJ_TIMER2) - 1);
    RAM(OBJ_TIMER2) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC6D2, 2); call_by_address_tail(0xC6D2, -1); return;
L_C6CF:;
    mff_cycles(0xC6CF, 13);
    t = (uint8_t)(RAM(OBJ_GROUND_Y) - 1);
    RAM(OBJ_GROUND_Y) = t;
    a = RAM(JOY_PRESSED);
    a &= 0x10;
    if (a == 0) { mff_cycles(0xC681, 1); goto L_C681; }
    mff_cycles(0xC6D8, 8);
    a = 0x30;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x06DC, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC6DD, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06DF, 0xC85C, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6E0, 16);
    x = 0x04;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x06E9, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC6EA, 8);
    x = 0x02;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x06EE, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6EF, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06F1, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6F2, 14);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PLAYER_ID) = a;
    RAM(SELECT_PLAYER) = a;
    SYNC_OUT;
    if (!mff_call_at(0x06FA, 0x9100, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6FB, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06FD, 0xC85C, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6FE, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0700, 0xC334, -1)) return;
    SYNC_IN;
    mff_cycles(0xC701, 16);
    x = 0x04;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x070A, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC70B, 8);
    x = 0x08;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x070F, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC710, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0712, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xC713, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0715, 0x8D00, -1)) return;
    SYNC_IN;
    mff_cycles(0xC716, 8);
    x = 0x2D;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    mff_call_fn(0x071A, func_FF2A, 0xE000);
    SYNC_IN;
    mff_cycles(0xC71B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x071D, 0xC85C, -1)) return;
    SYNC_IN;
    mff_cycles(0xC71E, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0720, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xC721, 56);
    g_cpu.I = 0;
    a = 0x01;
    RAM(IRQ_ENABLE) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SCROLL_NT) = a;
    RAM(SCROLL_Y_HI) = a;
    RAM(SCROLL_X) = a;
    RAM(SCROLL_Y) = a;
    RAM(SCROLL_X_HI) = a;
    RAM(SCROLL_X_PREV) = a;
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y_HI) = a;
    RAM(SPAWN_LIST_NEXT) = a;
    RAM(SPAWN_LIST_PREV) = a;
    RAM(UNK_5A) = a;
    RAM(UNK_33) = a;
    RAM(RENDER_OFF_DEPTH) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0746, 0xC334, -1)) return;
    SYNC_IN;
    mff_cycles(0xC747, 6);
    a = 0x01;
    hw_write(MMC3_MIRRORING, a);
    mff_cycles(0xC74C, 23);
    a = 0x01;
    RAM(PLAYER_DIR) = a;
    RAM(SCROLL_MODE) = a;
    RAM(CAMERA_DIR) = a;
    y = 0x00;
    RAM(AREA_INDEX) = y;
    RAM(ROUND_TABLES) = y;
    a = prg_read((uint16_t)(0xC83C + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xC75D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x075F, func_F34A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC760, 10);
    a = 0xFF;
    RAM(BG_SCREEN) = a;
    a = 0x1F;
    RAM(BG_COLUMN) = a;
    for (;;) {
        mff_cycles(0xC768, 11);
        a = 0x01;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(TMP16) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x076E, func_E521, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xC76F, 6);
        SYNC_OUT;
        mff_call_fn(0x0771, func_FF31, 0xE000);
        SYNC_IN;
        mff_cycles(0xC772, 5);
        a = RAM(BG_SCREEN);
        if (a != 0) break;
        mff_cycles(0xC768, 1);
    }
    mff_cycles(0xC776, 11);
    a = 0x0A;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(BANK_REQUEST) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x077C, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC77D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x077F, 0x9C00, -1)) return;
    SYNC_IN;
    mff_cycles(0xC780, 7);
    y = RAM(ROUND_TABLES);
    a = prg_read((uint16_t)(0xC1F8 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xC785, 9);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0789, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC78A, 4);
    a = prg_read(0xAE68);
    mff_cycles(0xC78D, 31);
    mff_push(a);
    a &= 0xE0;
    RAM(AREA_FLAGS) = a;
    a = mff_pull();
    a &= 0x1F;
    RAM(AREA_SCREEN_COUNT) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(AREA_PALETTE) = a;
    RAM(AREA_SCREEN) = a;
    SYNC_OUT;
    if (!mff_call_at(0x079F, 0xC3DD, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7A0, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PALETTE_REQUEST) = a;
    SYNC_OUT;
    if (!mff_call_at(0x07A6, 0xC362, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7A7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07A9, 0xC334, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7AA, 49);
    x = 0x00;
    a = 0xD0;
    RAM(OBJ_FLAGS) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0x30;
    RAM(OBJ_X) = a;
    a = 0x90;
    RAM(OBJ_Y) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y_HI) = a;
    RAM(OBJ_ID) = a;
    RAM(AREA_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x07CF, func_F26E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC7D0, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xC841 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xC7D5, 9);
    RAM(HEALTH) = a;
    SYNC_OUT;
    if (!mff_call_at(0x07D9, 0xC8B4, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7DA, 16);
    x = 0x08;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x07E3, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC7E4, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07E6, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7E7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07E9, 0x8038, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7EA, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07EC, 0x8080, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7ED, 112);
    a = 0x05;
    RAM(LIVES) = a;
    a = 0x03;
    RAM(CREDITS) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(COMBO_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(UNK_A5) = a;
    RAM(ACTION_TIMER) = a;
    RAM(ROUND) = a;
    RAM(SUB_AREA) = a;
    RAM(PLAYER_LEVEL) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(PLAYER_EXP) = a;
    RAM(PLAYER_EXP + 1) = a;
    RAM(WEAPON_HELD) = a;
    RAM(WEAPON_AMMO) = a;
    RAM(RESPAWN_TIMER2) = a;
    RAM(RESPAWN_TIMER1) = a;
    RAM(CAMERA_LOCK) = a;
    RAM(AREA_CLEAR) = a;
    RAM(AREA_KILLS) = a;
    RAM(SPECIAL_SCREEN_ID) = a;
    RAM(SCREEN_EVENT) = a;
    RAM(UNK_B6) = a;
    RAM(UNK_B2) = a;
    RAM(GAME_FINISHED) = a;
    RAM(CAMERA_AUTOSCROLL) = a;
    RAM(KEEP_ON_SCREEN) = a;
    RAM(ACTION_BEFORE_FALL) = a;
    RAM(GRAB_HIT_COUNT) = a;
    RAM(SPAWN_GROUPS) = a;
    RAM(SPAWN_GROUPS + 1) = a;
    RAM(SPAWN_GROUPS + 2) = a;
    RAM(CONTINUE_COUNT) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC8ED, 2); call_by_address_tail(0xC8ED, -1); return;
}

/*  $C5CE - lifted by port/tools/lift.py */
void func_C5CE(void) {
    REGS_IN;
    mff_cycles(0xC5CE, 6);
    SYNC_OUT;
    if (!mff_call_at(0x05D0, 0xC888, -1)) return;
    SYNC_IN;
    mff_cycles(0xC5D1, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(INTRO_PAGE) = a;
    SYNC_OUT;
    if (!mff_call_at(0x05D7, 0x9A00, -1)) return;
    SYNC_IN;
    mff_cycles(0xC5D8, 5);
    a = RAM(JOY_HELD);
    if (a == 0) {
        mff_cycles(0xC5DC, 8);
        a = 0x07;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x05E0, func_F34A, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xC5E1, 6);
        SYNC_OUT;
        if (!mff_call_at(0x05E3, 0xC85C, -1)) return;
        SYNC_IN;
        mff_cycles(0xC5E4, 16);
        x = 0x04;
        RAM(BANK_REQUEST) = x;
        x++;
        n = (x) >> 7;
        z = (x) == 0;
        RAM(BANK_REQUEST + 1) = x;
        SYNC_OUT;
        if (!mff_call_fn(0x05ED, func_FF4D, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xC5EE, 8);
        x = 0x06;
        n = (x) >> 7;
        z = (x) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x05F2, 0x8000, -1)) return;
        SYNC_IN;
        mff_cycles(0xC5F3, 6);
        SYNC_OUT;
        if (!mff_call_at(0x05F5, 0xC26F, -1)) return;
        SYNC_IN;
        mff_cycles(0xC5F6, 6);
        SYNC_OUT;
        if (!mff_call_at(0x05F8, 0x8B40, -1)) return;
        SYNC_IN;
        mff_cycles(0xC5F9, 16);
        x = 0x00;
        RAM(BANK_REQUEST) = x;
        x++;
        n = (x) >> 7;
        z = (x) == 0;
        RAM(BANK_REQUEST + 1) = x;
        SYNC_OUT;
        if (!mff_call_fn(0x0602, func_FF4D, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xC603, 11);
        a = 0x01;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(INTRO_PAGE) = a;
        SYNC_OUT;
        if (!mff_call_at(0x0609, 0x9A00, -1)) return;
        SYNC_IN;
        mff_cycles(0xC60A, 5);
        a = RAM(JOY_HELD);
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0xC60E, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0610, 0xC888, -1)) return;
            SYNC_IN;
            mff_cycles(0xC611, 11);
            a = 0x02;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(INTRO_PAGE) = a;
            SYNC_OUT;
            if (!mff_call_at(0x0617, 0x9A00, -1)) return;
            SYNC_IN;
            mff_cycles(0xC618, 5);
            a = RAM(JOY_HELD);
            n = (a) >> 7;
            z = (a) == 0;
            if (z) {
                mff_cycles(0xC61C, 6);
                SYNC_OUT;
                if (!mff_call_at(0x061E, 0xC85C, -1)) return;
                SYNC_IN;
                mff_cycles(0xC61F, 16);
                x = 0x04;
                RAM(BANK_REQUEST) = x;
                x++;
                n = (x) >> 7;
                z = (x) == 0;
                RAM(BANK_REQUEST + 1) = x;
                SYNC_OUT;
                if (!mff_call_fn(0x0628, func_FF4D, 0xE000)) return;
                SYNC_IN;
                mff_cycles(0xC629, 8);
                x = 0x07;
                n = (x) >> 7;
                z = (x) == 0;
                SYNC_OUT;
                if (!mff_call_at(0x062D, 0x8000, -1)) return;
                SYNC_IN;
                mff_cycles(0xC62E, 6);
                SYNC_OUT;
                if (!mff_call_at(0x0630, 0xC26F, -1)) return;
                SYNC_IN;
                mff_cycles(0xC631, 6);
                SYNC_OUT;
                if (!mff_call_at(0x0633, 0x8AC0, -1)) return;
                SYNC_IN;
                mff_cycles(0xC634, 16);
                x = 0x00;
                RAM(BANK_REQUEST) = x;
                x++;
                n = (x) >> 7;
                z = (x) == 0;
                RAM(BANK_REQUEST + 1) = x;
                SYNC_OUT;
                if (!mff_call_fn(0x063D, func_FF4D, 0xE000)) return;
                SYNC_IN;
                mff_cycles(0xC63E, 11);
                a = 0x03;
                n = (a) >> 7;
                z = (a) == 0;
                RAM(INTRO_PAGE) = a;
                SYNC_OUT;
                if (!mff_call_at(0x0644, 0x9A00, -1)) return;
                SYNC_IN;
                mff_cycles(0xC645, 5);
                a = RAM(JOY_HELD);
                n = (a) >> 7;
                z = (a) == 0;
                if (z) {
                    mff_cycles(0xC649, 6);
                    SYNC_OUT;
                    if (!mff_call_at(0x064B, 0xC888, -1)) return;
                    SYNC_IN;
                    mff_cycles(0xC64C, 11);
                    a = 0x04;
                    n = (a) >> 7;
                    z = (a) == 0;
                    RAM(INTRO_PAGE) = a;
                    SYNC_OUT;
                    if (!mff_call_at(0x0652, 0x9A00, -1)) return;
                    SYNC_IN;
                    mff_cycles(0xC653, 5);
                    a = RAM(JOY_HELD);
                    if (a != 0) {
                        mff_cycles(0xC655, 1);
                    }
                } else {
                    mff_cycles(0xC647, 1);
                }
            } else {
                mff_cycles(0xC61A, 1);
            }
        } else {
            mff_cycles(0xC60C, 1);
        }
    } else {
        mff_cycles(0xC5DA, 2);
    }
    mff_cycles(0xC657, 8);
    a = 0x0C;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x065B, func_F34A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC65C, 6);
    SYNC_OUT;
    if (!mff_call_at(0x065E, 0xC85C, -1)) return;
    SYNC_IN;
    mff_cycles(0xC65F, 16);
    x = 0x04;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0668, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC669, 8);
    x = 0x01;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x066D, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC66E, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0670, 0x8940, -1)) return;
    SYNC_IN;
    mff_cycles(0xC671, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0673, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xC674, 16);
    a = 0xFF;
    RAM(OBJ_TIMER) = a;
    RAM(OBJ_TIMER2) = a;
    a = 0x8B;
    RAM(OBJ_GROUND_Y) = a;
L_C681:;
    mff_cycles(0xC681, 11);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OAM_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0687, 0xC31D, -1)) return;
    SYNC_IN;
    mff_cycles(0xC688, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x068A, func_E9BC, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC68B, 6);
    SYNC_OUT;
    mff_call_fn(0x068D, func_FF31, 0xE000);
    SYNC_IN;
    mff_cycles(0xC68E, 6);
    a = RAM(OBJ_TIMER);
    if (a != 0) { mff_cycles(0xC691, 1); goto L_C6A7; }
    mff_cycles(0xC693, 6);
    a = RAM(OBJ_TIMER2);
    if (a != 0) { mff_cycles(0xC696, 1); goto L_C6C9; }
    mff_cycles(0xC698, 6);
    a = RAM(OBJ_GROUND_Y);
    if (a != 0) { mff_cycles(0xC69B, 1); goto L_C6CF; }
    mff_cycles(0xC69D, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(RENDER_OFF_DEPTH) = a;
    SYNC_OUT;
    if (!mff_call_at(0x06A3, 0xC36E, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6A4, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xFE00, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_FE00(); g_code_window_base = swb; } return;
L_C6A7:;
    mff_cycles(0xC6A7, 14);
    t = (uint8_t)(RAM(OBJ_TIMER) - 1);
    RAM(OBJ_TIMER) = t;
    a = RAM(OBJ_TIMER);
    m = 0xF0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xC6B1, 8);
        a = 0x32;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x06B5, func_F34C, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xC6B6, 21);
        a = 0x12;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(PALETTE_BUF + 3) = a;
        RAM(PALETTE_BUF + 7) = a;
        RAM(PALETTE_BUF + 15) = a;
        RAM(PALETTE_BUF + 31) = a;
        RAM(PALETTE_REQUEST) = a;
    } else {
        mff_cycles(0xC6AF, 1);
    }
    mff_cycles(0xC6C6, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC6D2, 2); call_by_address_tail(0xC6D2, -1); return;
L_C6C9:;
    mff_cycles(0xC6C9, 9);
    t = (uint8_t)(RAM(OBJ_TIMER2) - 1);
    RAM(OBJ_TIMER2) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC6D2, 2); call_by_address_tail(0xC6D2, -1); return;
L_C6CF:;
    mff_cycles(0xC6CF, 13);
    t = (uint8_t)(RAM(OBJ_GROUND_Y) - 1);
    RAM(OBJ_GROUND_Y) = t;
    a = RAM(JOY_PRESSED);
    a &= 0x10;
    if (a == 0) { mff_cycles(0xC681, 1); goto L_C681; }
    mff_cycles(0xC6D8, 8);
    a = 0x30;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x06DC, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC6DD, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06DF, 0xC85C, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6E0, 16);
    x = 0x04;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x06E9, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC6EA, 8);
    x = 0x02;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x06EE, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6EF, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06F1, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6F2, 14);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PLAYER_ID) = a;
    RAM(SELECT_PLAYER) = a;
    SYNC_OUT;
    if (!mff_call_at(0x06FA, 0x9100, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6FB, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06FD, 0xC85C, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6FE, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0700, 0xC334, -1)) return;
    SYNC_IN;
    mff_cycles(0xC701, 16);
    x = 0x04;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x070A, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC70B, 8);
    x = 0x08;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x070F, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC710, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0712, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xC713, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0715, 0x8D00, -1)) return;
    SYNC_IN;
    mff_cycles(0xC716, 8);
    x = 0x2D;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    mff_call_fn(0x071A, func_FF2A, 0xE000);
    SYNC_IN;
    mff_cycles(0xC71B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x071D, 0xC85C, -1)) return;
    SYNC_IN;
    mff_cycles(0xC71E, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0720, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xC721, 56);
    g_cpu.I = 0;
    a = 0x01;
    RAM(IRQ_ENABLE) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SCROLL_NT) = a;
    RAM(SCROLL_Y_HI) = a;
    RAM(SCROLL_X) = a;
    RAM(SCROLL_Y) = a;
    RAM(SCROLL_X_HI) = a;
    RAM(SCROLL_X_PREV) = a;
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y_HI) = a;
    RAM(SPAWN_LIST_NEXT) = a;
    RAM(SPAWN_LIST_PREV) = a;
    RAM(UNK_5A) = a;
    RAM(UNK_33) = a;
    RAM(RENDER_OFF_DEPTH) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0746, 0xC334, -1)) return;
    SYNC_IN;
    mff_cycles(0xC747, 6);
    a = 0x01;
    hw_write(MMC3_MIRRORING, a);
    mff_cycles(0xC74C, 23);
    a = 0x01;
    RAM(PLAYER_DIR) = a;
    RAM(SCROLL_MODE) = a;
    RAM(CAMERA_DIR) = a;
    y = 0x00;
    RAM(AREA_INDEX) = y;
    RAM(ROUND_TABLES) = y;
    a = prg_read((uint16_t)(0xC83C + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xC75D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x075F, func_F34A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC760, 10);
    a = 0xFF;
    RAM(BG_SCREEN) = a;
    a = 0x1F;
    RAM(BG_COLUMN) = a;
    for (;;) {
        mff_cycles(0xC768, 11);
        a = 0x01;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(TMP16) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x076E, func_E521, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xC76F, 6);
        SYNC_OUT;
        mff_call_fn(0x0771, func_FF31, 0xE000);
        SYNC_IN;
        mff_cycles(0xC772, 5);
        a = RAM(BG_SCREEN);
        if (a != 0) break;
        mff_cycles(0xC768, 1);
    }
    mff_cycles(0xC776, 11);
    a = 0x0A;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(BANK_REQUEST) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x077C, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC77D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x077F, 0x9C00, -1)) return;
    SYNC_IN;
    mff_cycles(0xC780, 7);
    y = RAM(ROUND_TABLES);
    a = prg_read((uint16_t)(0xC1F8 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xC785, 9);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0789, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC78A, 4);
    a = prg_read(0xAE68);
    mff_cycles(0xC78D, 31);
    mff_push(a);
    a &= 0xE0;
    RAM(AREA_FLAGS) = a;
    a = mff_pull();
    a &= 0x1F;
    RAM(AREA_SCREEN_COUNT) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(AREA_PALETTE) = a;
    RAM(AREA_SCREEN) = a;
    SYNC_OUT;
    if (!mff_call_at(0x079F, 0xC3DD, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7A0, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PALETTE_REQUEST) = a;
    SYNC_OUT;
    if (!mff_call_at(0x07A6, 0xC362, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7A7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07A9, 0xC334, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7AA, 49);
    x = 0x00;
    a = 0xD0;
    RAM(OBJ_FLAGS) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0x30;
    RAM(OBJ_X) = a;
    a = 0x90;
    RAM(OBJ_Y) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y_HI) = a;
    RAM(OBJ_ID) = a;
    RAM(AREA_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x07CF, func_F26E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC7D0, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xC841 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xC7D5, 9);
    RAM(HEALTH) = a;
    SYNC_OUT;
    if (!mff_call_at(0x07D9, 0xC8B4, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7DA, 16);
    x = 0x08;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x07E3, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC7E4, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07E6, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7E7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07E9, 0x8038, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7EA, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07EC, 0x8080, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7ED, 112);
    a = 0x05;
    RAM(LIVES) = a;
    a = 0x03;
    RAM(CREDITS) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(COMBO_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(UNK_A5) = a;
    RAM(ACTION_TIMER) = a;
    RAM(ROUND) = a;
    RAM(SUB_AREA) = a;
    RAM(PLAYER_LEVEL) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(PLAYER_EXP) = a;
    RAM(PLAYER_EXP + 1) = a;
    RAM(WEAPON_HELD) = a;
    RAM(WEAPON_AMMO) = a;
    RAM(RESPAWN_TIMER2) = a;
    RAM(RESPAWN_TIMER1) = a;
    RAM(CAMERA_LOCK) = a;
    RAM(AREA_CLEAR) = a;
    RAM(AREA_KILLS) = a;
    RAM(SPECIAL_SCREEN_ID) = a;
    RAM(SCREEN_EVENT) = a;
    RAM(UNK_B6) = a;
    RAM(UNK_B2) = a;
    RAM(GAME_FINISHED) = a;
    RAM(CAMERA_AUTOSCROLL) = a;
    RAM(KEEP_ON_SCREEN) = a;
    RAM(ACTION_BEFORE_FALL) = a;
    RAM(GRAB_HIT_COUNT) = a;
    RAM(SPAWN_GROUPS) = a;
    RAM(SPAWN_GROUPS + 1) = a;
    RAM(SPAWN_GROUPS + 2) = a;
    RAM(CONTINUE_COUNT) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC8ED, 2); call_by_address_tail(0xC8ED, -1); return;
}

/*  $C681 - lifted by port/tools/lift.py */
void func_C681(void) {
    REGS_IN;
L_C681:;
    mff_cycles(0xC681, 11);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OAM_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0687, 0xC31D, -1)) return;
    SYNC_IN;
    mff_cycles(0xC688, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x068A, func_E9BC, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC68B, 6);
    SYNC_OUT;
    mff_call_fn(0x068D, func_FF31, 0xE000);
    SYNC_IN;
    mff_cycles(0xC68E, 6);
    a = RAM(OBJ_TIMER);
    if (a != 0) { mff_cycles(0xC691, 1); goto L_C6A7; }
    mff_cycles(0xC693, 6);
    a = RAM(OBJ_TIMER2);
    if (a != 0) { mff_cycles(0xC696, 1); goto L_C6C9; }
    mff_cycles(0xC698, 6);
    a = RAM(OBJ_GROUND_Y);
    if (a != 0) { mff_cycles(0xC69B, 1); goto L_C6CF; }
    mff_cycles(0xC69D, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(RENDER_OFF_DEPTH) = a;
    SYNC_OUT;
    if (!mff_call_at(0x06A3, 0xC36E, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6A4, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xFE00, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_FE00(); g_code_window_base = swb; } return;
L_C6A7:;
    mff_cycles(0xC6A7, 14);
    t = (uint8_t)(RAM(OBJ_TIMER) - 1);
    RAM(OBJ_TIMER) = t;
    a = RAM(OBJ_TIMER);
    m = 0xF0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xC6B1, 8);
        a = 0x32;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x06B5, func_F34C, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xC6B6, 21);
        a = 0x12;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(PALETTE_BUF + 3) = a;
        RAM(PALETTE_BUF + 7) = a;
        RAM(PALETTE_BUF + 15) = a;
        RAM(PALETTE_BUF + 31) = a;
        RAM(PALETTE_REQUEST) = a;
    } else {
        mff_cycles(0xC6AF, 1);
    }
    mff_cycles(0xC6C6, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC6D2, 2); call_by_address_tail(0xC6D2, -1); return;
L_C6C9:;
    mff_cycles(0xC6C9, 9);
    t = (uint8_t)(RAM(OBJ_TIMER2) - 1);
    RAM(OBJ_TIMER2) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC6D2, 2); call_by_address_tail(0xC6D2, -1); return;
L_C6CF:;
    mff_cycles(0xC6CF, 13);
    t = (uint8_t)(RAM(OBJ_GROUND_Y) - 1);
    RAM(OBJ_GROUND_Y) = t;
    a = RAM(JOY_PRESSED);
    a &= 0x10;
    if (a == 0) { mff_cycles(0xC681, 1); goto L_C681; }
    mff_cycles(0xC6D8, 8);
    a = 0x30;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x06DC, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC6DD, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06DF, 0xC85C, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6E0, 16);
    x = 0x04;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x06E9, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC6EA, 8);
    x = 0x02;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x06EE, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6EF, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06F1, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6F2, 14);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PLAYER_ID) = a;
    RAM(SELECT_PLAYER) = a;
    SYNC_OUT;
    if (!mff_call_at(0x06FA, 0x9100, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6FB, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06FD, 0xC85C, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6FE, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0700, 0xC334, -1)) return;
    SYNC_IN;
    mff_cycles(0xC701, 16);
    x = 0x04;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x070A, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC70B, 8);
    x = 0x08;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x070F, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC710, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0712, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xC713, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0715, 0x8D00, -1)) return;
    SYNC_IN;
    mff_cycles(0xC716, 8);
    x = 0x2D;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    mff_call_fn(0x071A, func_FF2A, 0xE000);
    SYNC_IN;
    mff_cycles(0xC71B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x071D, 0xC85C, -1)) return;
    SYNC_IN;
    mff_cycles(0xC71E, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0720, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xC721, 56);
    g_cpu.I = 0;
    a = 0x01;
    RAM(IRQ_ENABLE) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SCROLL_NT) = a;
    RAM(SCROLL_Y_HI) = a;
    RAM(SCROLL_X) = a;
    RAM(SCROLL_Y) = a;
    RAM(SCROLL_X_HI) = a;
    RAM(SCROLL_X_PREV) = a;
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y_HI) = a;
    RAM(SPAWN_LIST_NEXT) = a;
    RAM(SPAWN_LIST_PREV) = a;
    RAM(UNK_5A) = a;
    RAM(UNK_33) = a;
    RAM(RENDER_OFF_DEPTH) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0746, 0xC334, -1)) return;
    SYNC_IN;
    mff_cycles(0xC747, 6);
    a = 0x01;
    hw_write(MMC3_MIRRORING, a);
    mff_cycles(0xC74C, 23);
    a = 0x01;
    RAM(PLAYER_DIR) = a;
    RAM(SCROLL_MODE) = a;
    RAM(CAMERA_DIR) = a;
    y = 0x00;
    RAM(AREA_INDEX) = y;
    RAM(ROUND_TABLES) = y;
    a = prg_read((uint16_t)(0xC83C + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xC75D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x075F, func_F34A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC760, 10);
    a = 0xFF;
    RAM(BG_SCREEN) = a;
    a = 0x1F;
    RAM(BG_COLUMN) = a;
    for (;;) {
        mff_cycles(0xC768, 11);
        a = 0x01;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(TMP16) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x076E, func_E521, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xC76F, 6);
        SYNC_OUT;
        mff_call_fn(0x0771, func_FF31, 0xE000);
        SYNC_IN;
        mff_cycles(0xC772, 5);
        a = RAM(BG_SCREEN);
        if (a != 0) break;
        mff_cycles(0xC768, 1);
    }
    mff_cycles(0xC776, 11);
    a = 0x0A;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(BANK_REQUEST) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x077C, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC77D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x077F, 0x9C00, -1)) return;
    SYNC_IN;
    mff_cycles(0xC780, 7);
    y = RAM(ROUND_TABLES);
    a = prg_read((uint16_t)(0xC1F8 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xC785, 9);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0789, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC78A, 4);
    a = prg_read(0xAE68);
    mff_cycles(0xC78D, 31);
    mff_push(a);
    a &= 0xE0;
    RAM(AREA_FLAGS) = a;
    a = mff_pull();
    a &= 0x1F;
    RAM(AREA_SCREEN_COUNT) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(AREA_PALETTE) = a;
    RAM(AREA_SCREEN) = a;
    SYNC_OUT;
    if (!mff_call_at(0x079F, 0xC3DD, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7A0, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PALETTE_REQUEST) = a;
    SYNC_OUT;
    if (!mff_call_at(0x07A6, 0xC362, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7A7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07A9, 0xC334, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7AA, 49);
    x = 0x00;
    a = 0xD0;
    RAM(OBJ_FLAGS) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0x30;
    RAM(OBJ_X) = a;
    a = 0x90;
    RAM(OBJ_Y) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y_HI) = a;
    RAM(OBJ_ID) = a;
    RAM(AREA_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x07CF, func_F26E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC7D0, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xC841 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xC7D5, 9);
    RAM(HEALTH) = a;
    SYNC_OUT;
    if (!mff_call_at(0x07D9, 0xC8B4, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7DA, 16);
    x = 0x08;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x07E3, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC7E4, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07E6, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7E7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07E9, 0x8038, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7EA, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07EC, 0x8080, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7ED, 112);
    a = 0x05;
    RAM(LIVES) = a;
    a = 0x03;
    RAM(CREDITS) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(COMBO_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(UNK_A5) = a;
    RAM(ACTION_TIMER) = a;
    RAM(ROUND) = a;
    RAM(SUB_AREA) = a;
    RAM(PLAYER_LEVEL) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(PLAYER_EXP) = a;
    RAM(PLAYER_EXP + 1) = a;
    RAM(WEAPON_HELD) = a;
    RAM(WEAPON_AMMO) = a;
    RAM(RESPAWN_TIMER2) = a;
    RAM(RESPAWN_TIMER1) = a;
    RAM(CAMERA_LOCK) = a;
    RAM(AREA_CLEAR) = a;
    RAM(AREA_KILLS) = a;
    RAM(SPECIAL_SCREEN_ID) = a;
    RAM(SCREEN_EVENT) = a;
    RAM(UNK_B6) = a;
    RAM(UNK_B2) = a;
    RAM(GAME_FINISHED) = a;
    RAM(CAMERA_AUTOSCROLL) = a;
    RAM(KEEP_ON_SCREEN) = a;
    RAM(ACTION_BEFORE_FALL) = a;
    RAM(GRAB_HIT_COUNT) = a;
    RAM(SPAWN_GROUPS) = a;
    RAM(SPAWN_GROUPS + 1) = a;
    RAM(SPAWN_GROUPS + 2) = a;
    RAM(CONTINUE_COUNT) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC8ED, 2); call_by_address_tail(0xC8ED, -1); return;
}

/* title_wait_start $C6D2 - lifted by port/tools/lift.py */
void func_C6D2(void) {
    REGS_IN;
    mff_cycles(0xC6D2, 7);
    a = RAM(JOY_PRESSED);
    a &= 0x10;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xC6D6, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0681), -1); return; }
    mff_cycles(0xC6D8, 8);
    a = 0x30;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x06DC, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC6DD, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06DF, 0xC85C, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6E0, 16);
    x = 0x04;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x06E9, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC6EA, 8);
    x = 0x02;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x06EE, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6EF, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06F1, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6F2, 14);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PLAYER_ID) = a;
    RAM(SELECT_PLAYER) = a;
    SYNC_OUT;
    if (!mff_call_at(0x06FA, 0x9100, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6FB, 6);
    SYNC_OUT;
    if (!mff_call_at(0x06FD, 0xC85C, -1)) return;
    SYNC_IN;
    mff_cycles(0xC6FE, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0700, 0xC334, -1)) return;
    SYNC_IN;
    mff_cycles(0xC701, 16);
    x = 0x04;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x070A, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC70B, 8);
    x = 0x08;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x070F, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC710, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0712, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xC713, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0715, 0x8D00, -1)) return;
    SYNC_IN;
    mff_cycles(0xC716, 8);
    x = 0x2D;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    mff_call_fn(0x071A, func_FF2A, 0xE000);
    SYNC_IN;
    mff_cycles(0xC71B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x071D, 0xC85C, -1)) return;
    SYNC_IN;
    mff_cycles(0xC71E, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0720, 0xC26F, -1)) return;
    SYNC_IN;
    mff_cycles(0xC721, 56);
    g_cpu.I = 0;
    a = 0x01;
    RAM(IRQ_ENABLE) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SCROLL_NT) = a;
    RAM(SCROLL_Y_HI) = a;
    RAM(SCROLL_X) = a;
    RAM(SCROLL_Y) = a;
    RAM(SCROLL_X_HI) = a;
    RAM(SCROLL_X_PREV) = a;
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y_HI) = a;
    RAM(SPAWN_LIST_NEXT) = a;
    RAM(SPAWN_LIST_PREV) = a;
    RAM(UNK_5A) = a;
    RAM(UNK_33) = a;
    RAM(RENDER_OFF_DEPTH) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0746, 0xC334, -1)) return;
    SYNC_IN;
    mff_cycles(0xC747, 6);
    a = 0x01;
    hw_write(MMC3_MIRRORING, a);
    mff_cycles(0xC74C, 23);
    a = 0x01;
    RAM(PLAYER_DIR) = a;
    RAM(SCROLL_MODE) = a;
    RAM(CAMERA_DIR) = a;
    y = 0x00;
    RAM(AREA_INDEX) = y;
    RAM(ROUND_TABLES) = y;
    a = prg_read((uint16_t)(0xC83C + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xC75D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x075F, func_F34A, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC760, 10);
    a = 0xFF;
    RAM(BG_SCREEN) = a;
    a = 0x1F;
    RAM(BG_COLUMN) = a;
    for (;;) {
        mff_cycles(0xC768, 11);
        a = 0x01;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(TMP16) = a;
        SYNC_OUT;
        if (!mff_call_fn(0x076E, func_E521, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xC76F, 6);
        SYNC_OUT;
        mff_call_fn(0x0771, func_FF31, 0xE000);
        SYNC_IN;
        mff_cycles(0xC772, 5);
        a = RAM(BG_SCREEN);
        if (a != 0) break;
        mff_cycles(0xC768, 1);
    }
    mff_cycles(0xC776, 11);
    a = 0x0A;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(BANK_REQUEST) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x077C, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC77D, 6);
    SYNC_OUT;
    if (!mff_call_at(0x077F, 0x9C00, -1)) return;
    SYNC_IN;
    mff_cycles(0xC780, 7);
    y = RAM(ROUND_TABLES);
    a = prg_read((uint16_t)(0xC1F8 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xC785, 9);
    RAM(BANK_REQUEST + 1) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x0789, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC78A, 4);
    a = prg_read(0xAE68);
    mff_cycles(0xC78D, 31);
    mff_push(a);
    a &= 0xE0;
    RAM(AREA_FLAGS) = a;
    a = mff_pull();
    a &= 0x1F;
    RAM(AREA_SCREEN_COUNT) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(AREA_PALETTE) = a;
    RAM(AREA_SCREEN) = a;
    SYNC_OUT;
    if (!mff_call_at(0x079F, 0xC3DD, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7A0, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PALETTE_REQUEST) = a;
    SYNC_OUT;
    if (!mff_call_at(0x07A6, 0xC362, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7A7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07A9, 0xC334, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7AA, 49);
    x = 0x00;
    a = 0xD0;
    RAM(OBJ_FLAGS) = a;
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0x30;
    RAM(OBJ_X) = a;
    a = 0x90;
    RAM(OBJ_Y) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y_HI) = a;
    RAM(OBJ_ID) = a;
    RAM(AREA_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x07CF, func_F26E, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC7D0, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xC841 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xC7D5, 9);
    RAM(HEALTH) = a;
    SYNC_OUT;
    if (!mff_call_at(0x07D9, 0xC8B4, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7DA, 16);
    x = 0x08;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x07E3, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC7E4, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07E6, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7E7, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07E9, 0x8038, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7EA, 6);
    SYNC_OUT;
    if (!mff_call_at(0x07EC, 0x8080, -1)) return;
    SYNC_IN;
    mff_cycles(0xC7ED, 112);
    a = 0x05;
    RAM(LIVES) = a;
    a = 0x03;
    RAM(CREDITS) = a;
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    RAM(COMBO_TIMER) = a;
    RAM(ATTACK_LIVE) = a;
    RAM(UNK_A5) = a;
    RAM(ACTION_TIMER) = a;
    RAM(ROUND) = a;
    RAM(SUB_AREA) = a;
    RAM(PLAYER_LEVEL) = a;
    RAM(GUY_JUMP_WINDOW) = a;
    RAM(PLAYER_EXP) = a;
    RAM(PLAYER_EXP + 1) = a;
    RAM(WEAPON_HELD) = a;
    RAM(WEAPON_AMMO) = a;
    RAM(RESPAWN_TIMER2) = a;
    RAM(RESPAWN_TIMER1) = a;
    RAM(CAMERA_LOCK) = a;
    RAM(AREA_CLEAR) = a;
    RAM(AREA_KILLS) = a;
    RAM(SPECIAL_SCREEN_ID) = a;
    RAM(SCREEN_EVENT) = a;
    RAM(UNK_B6) = a;
    RAM(UNK_B2) = a;
    RAM(GAME_FINISHED) = a;
    RAM(CAMERA_AUTOSCROLL) = a;
    RAM(KEEP_ON_SCREEN) = a;
    RAM(ACTION_BEFORE_FALL) = a;
    RAM(GRAB_HIT_COUNT) = a;
    RAM(SPAWN_GROUPS) = a;
    RAM(SPAWN_GROUPS + 1) = a;
    RAM(SPAWN_GROUPS + 2) = a;
    RAM(CONTINUE_COUNT) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xC8ED, 2); call_by_address_tail(0xC8ED, -1); return;
}

/* gameplay_special_screen $C987 - lifted by port/tools/lift.py */
void func_C987(void) {
    REGS_IN;
    mff_cycles(0xC987, 16);
    x = 0x04;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x0990, func_FF4D, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC991, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0993, 0x9800, -1)) return;
    SYNC_IN;
    mff_cycles(0xC994, 8);
    a = RAM(OBJ_KIND + 12);
    m = 0x07;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xC999, 1); goto L_C99D; }
    mff_cycles(0xC99B, 2);
    if (!z) { mff_cycles(0xC99B, 1); goto L_C9A7; }
L_C99D:;
    mff_cycles(0xC99D, 11);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OAM_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_at(0x09A3, 0xC31D, -1)) return;
    SYNC_IN;
    mff_cycles(0xC9A4, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x09A6, func_E9BC, 0xE000)) return;
    SYNC_IN;
L_C9A7:;
    mff_cycles(0xC9A7, 0);
    SYNC_OUT;
    { void func_C9A7(void); func_C9A7(); } return;   /* written by hand */
}

/* gameplay_draw $C99D - lifted by port/tools/lift.py */
void func_C99D(void) {
    REGS_IN;
    mff_cycles(0xC99D, 11);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OAM_INDEX) = a;
    SYNC_OUT;
    if (!mff_call_at(0x09A3, 0xC31D, -1)) return;
    SYNC_IN;
    mff_cycles(0xC9A4, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x09A6, func_E9BC, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xC9A7, 0);
    SYNC_OUT;
    { void func_C9A7(void); func_C9A7(); } return;   /* written by hand */
}

/* player_state $C9B6 - lifted by port/tools/lift.py */
void func_C9B6(void) {
    REGS_IN;
    mff_cycles(0xC9B6, 6);
    SYNC_OUT;
    if (!mff_call_at(0x09B8, 0xCED1, -1)) return;
    SYNC_IN;
    mff_cycles(0xC9B9, 9);
    x = 0x00;
    y = RAM(GAME_ACTION);
    a = prg_read((uint16_t)(0xC9CA + y));
    mff_cycles(0xC9C0, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0xC9F5 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xC9C5, 8);
    RAM(TMP1) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $C9B9 - lifted by port/tools/lift.py */
void func_C9B9(void) {
    REGS_IN;
    mff_cycles(0xC9B9, 9);
    x = 0x00;
    y = RAM(GAME_ACTION);
    a = prg_read((uint16_t)(0xC9CA + y));
    mff_cycles(0xC9C0, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0xC9F5 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xC9C5, 8);
    RAM(TMP1) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/* player_state_idle $CA20 - lifted by port/tools/lift.py */
void func_CA20(void) {
    REGS_IN;
    mff_cycles(0xCA20, 5);
    a = RAM(PLAYER_INVULN);
    if (a != 0) {
        mff_cycles(0xCA24, 5);
        t = (uint8_t)(RAM(PLAYER_INVULN) - 1);
        RAM(PLAYER_INVULN) = t;
    } else {
        mff_cycles(0xCA22, 1);
    }
    mff_cycles(0xCA26, 5);
    a = RAM(COMBO_TIMER);
    if (a != 0) {
        mff_cycles(0xCA2A, 5);
        t = (uint8_t)(RAM(COMBO_TIMER) - 1);
        RAM(COMBO_TIMER) = t;
    } else {
        mff_cycles(0xCA28, 1);
    }
    mff_cycles(0xCA2C, 7);
    a = RAM(JOY_PRESSED);
    m = 0x80;
    c = a >= m;
    if (a == m) { mff_cycles(0xCA30, 1); goto L_CA6D; }
    mff_cycles(0xCA32, 4);
    m = 0xC0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xCA36, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xCAA2, 2); call_by_address_tail(0xCAA2, -1); return;
    } else {
        mff_cycles(0xCA34, 1);
    }
L_CA3C:;
    mff_cycles(0xCA3C, 5);
    a = RAM(HEALTH);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xCA3E, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0A39), -1); return; }
    mff_cycles(0xCA40, 5);
    a = RAM(UNK_B2);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) { mff_cycles(0xCA42, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0A39), -1); return; }
    mff_cycles(0xCA44, 7);
    y = RAM(PLAYER_ID);
    m = 0x02;
    c = y >= m;
    if (y != m) {
        mff_cycles(0xCA4A, 12);
        a = 0x38;
        RAM(OBJ_VY_FRAC) = a;
        a = 0x04;
        RAM(OBJ_VY) = a;
    } else {
        mff_cycles(0xCA48, 1);
    }
    mff_cycles(0xCA54, 4);
    a = prg_read((uint16_t)(0xE228 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCA57, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0A59, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCA5A, 4);
    a = prg_read((uint16_t)(0xE270 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCA5D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0A5F, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCA60, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0A62, func_F43B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCA63, 18);
    a = 0x17;
    RAM(GAME_ACTION) = a;
    a = RAM(OBJ_Y);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PLAYER_GROUND_Y) = a;
    SYNC_OUT;
    mff_rts(); return;
L_CA6D:;
    mff_cycles(0xCA6D, 5);
    a = RAM(HEALTH);
    if (a == 0) { mff_cycles(0xCA6F, 1); goto L_CA77; }
    mff_cycles(0xCA71, 7);
    a = RAM(JOY_HELD);
    a &= 0x40;
    if (a != 0) { mff_cycles(0xCA3C, 1); goto L_CA3C; }
L_CA77:;
    mff_cycles(0xCA77, 19);
    a = 0x38;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x04;
    RAM(OBJ_VY) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1E9 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCA86, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0A88, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCA89, 4);
    a = prg_read((uint16_t)(0xE22E + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCA8C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0A8E, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCA8F, 23);
    a = 0x01;
    RAM(GAME_ACTION) = a;
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    RAM(ATTACK_GROUND_Y) = a;
    a = 0x1B;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0A9E, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCA9F, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCF88, 2); call_by_address_tail(0xCF88, -1); return;
}

/*  $CA3C - lifted by port/tools/lift.py */
void func_CA3C(void) {
    REGS_IN;
    mff_cycles(0xCA3C, 5);
    a = RAM(HEALTH);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xCA3E, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0A39), -1); return; }
    mff_cycles(0xCA40, 5);
    a = RAM(UNK_B2);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) { mff_cycles(0xCA42, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0A39), -1); return; }
    mff_cycles(0xCA44, 7);
    y = RAM(PLAYER_ID);
    m = 0x02;
    c = y >= m;
    if (y != m) {
        mff_cycles(0xCA4A, 12);
        a = 0x38;
        RAM(OBJ_VY_FRAC) = a;
        a = 0x04;
        RAM(OBJ_VY) = a;
    } else {
        mff_cycles(0xCA48, 1);
    }
    mff_cycles(0xCA54, 4);
    a = prg_read((uint16_t)(0xE228 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCA57, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0A59, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCA5A, 4);
    a = prg_read((uint16_t)(0xE270 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCA5D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0A5F, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCA60, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0A62, func_F43B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCA63, 18);
    a = 0x17;
    RAM(GAME_ACTION) = a;
    a = RAM(OBJ_Y);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PLAYER_GROUND_Y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $CAA2 - lifted by port/tools/lift.py */
void func_CAA2(void) {
    REGS_IN;
    mff_cycles(0xCAA2, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1C1 + y));
    mff_cycles(0xCAA7, 8);
    RAM(OBJ_VX_FRAC) = a;
    a = prg_read((uint16_t)(0xE1C4 + y));
    mff_cycles(0xCAAD, 8);
    RAM(OBJ_VX) = a;
    a = prg_read((uint16_t)(0xE1C7 + y));
    mff_cycles(0xCAB3, 8);
    RAM(OBJ_VY_FRAC) = a;
    a = prg_read((uint16_t)(0xE1CA + y));
    mff_cycles(0xCAB9, 11);
    RAM(OBJ_VY) = a;
    a = RAM(JOY_HELD);
    if (a != 0x09) {
        mff_cycles(0xCAC2, 4);
        if (a != 0x05) {
            mff_cycles(0xCAC6, 4);
            if (a != 0x0A) {
                mff_cycles(0xCACA, 4);
                m = 0x06;
                c = a >= m;
                n = ((uint8_t)(a - m)) >> 7;
                z = ((uint8_t)(a - m)) == 0;
                if (!z) {
                    mff_cycles(0xCACE, 3);
                    SYNC_OUT;
                    nes_cpu_instruction_boundary(0xCAE5, 2); call_by_address_tail(0xCAE5, -1); return;
                } else {
                    mff_cycles(0xCACC, 1);
                }
            } else {
                mff_cycles(0xCAC8, 1);
            }
        } else {
            mff_cycles(0xCAC4, 1);
        }
    } else {
        mff_cycles(0xCAC0, 1);
    }
    mff_cycles(0xCAD1, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1D7 + y));
    mff_cycles(0xCAD6, 12);
    RAM(OBJ_VX_FRAC) = a;
    RAM(OBJ_VY_FRAC) = a;
    a = prg_read((uint16_t)(0xE1DA + y));
    mff_cycles(0xCADF, 15);
    RAM(OBJ_VX) = a;
    RAM(OBJ_VY) = a;
    a = RAM(JOY_PRESSED);
    m = 0x40;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xCAEB, 4);
        m = 0x80;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xCAEF, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xCCB5, 2); call_by_address_tail(0xCCB5, -1); return;
        } else {
            mff_cycles(0xCAED, 1);
        }
        mff_cycles(0xCAF2, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xCCDA, 2); call_by_address_tail(0xCCDA, -1); return;
    } else {
        mff_cycles(0xCAE9, 1);
    }
    mff_cycles(0xCAF8, 12);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(HEALTH);
    if (a != 0) {
        mff_cycles(0xCB01, 7);
        a = RAM(JOY_HELD);
        m = 0xC0;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) { mff_cycles(0xCB05, 2); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0AF5), -1); return; }
    } else {
        mff_cycles(0xCAFF, 1);
    }
    mff_cycles(0xCB07, 5);
    a = RAM(WEAPON_HELD);
    if (a != 0) { mff_cycles(0xCB09, 1); goto L_CB66; }
    mff_cycles(0xCB0B, 5);
    a = RAM(COMBO_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0xCB0F, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xCCA2, 2); call_by_address_tail(0xCCA2, -1); return;
    } else {
        mff_cycles(0xCB0D, 1);
    }
    mff_cycles(0xCB12, 15);
    a = 0x00;
    RAM(COMBO_TIMER) = a;
    y = RAM(PLAYER_ID);
    a = RAM(PLAYER_STAND_ANIM);
    m = prg_read((uint16_t)(0xE291 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xCB1D, 2);
    if (z) {
        mff_cycles(0xCB1F, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xCBA3, 2); call_by_address_tail(0xCBA3, -1); return;
    } else {
        mff_cycles(0xCB1D, 1);
    }
    mff_cycles(0xCB22, 9);
    a = RAM(PLAYER_STAND_ANIM);
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0B26, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCB27, 11);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    m = prg_read((uint16_t)(0xE288 + y));
    c = a >= m;
    mff_cycles(0xCB2F, 2);
    if (a == m) { mff_cycles(0xCB2F, 1); goto L_CB53; }
    mff_cycles(0xCB31, 4);
    m = prg_read((uint16_t)(0xE28B + y));
    c = a >= m;
    mff_cycles(0xCB34, 2);
    if (a == m) { mff_cycles(0xCB34, 1); goto L_CB53; }
    mff_cycles(0xCB36, 4);
    m = 0x15;
    c = a >= m;
    if (a == m) { mff_cycles(0xCB38, 1); goto L_CB3F; }
    mff_cycles(0xCB3A, 4);
    a = prg_read((uint16_t)(0xE23A + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCB3D, 2);
    if (!z) { mff_cycles(0xCB3D, 1); goto L_CB60; }
L_CB3F:;
    mff_cycles(0xCB3F, 23);
    a = 0xE8;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x02;
    RAM(OBJ_VY) = a;
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = prg_read((uint16_t)(0xE23D + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCB51, 2);
    if (!z) { mff_cycles(0xCB51, 1); goto L_CB60; }
L_CB53:;
    mff_cycles(0xCB53, 16);
    a = 0x01;
    RAM(OBJ_ANIM_FRAME) = a;
    a = 0x00;
    RAM(OBJ_ANIM_TIMER) = a;
    a = prg_read((uint16_t)(0xE237 + y));
    n = (a) >> 7;
    z = (a) == 0;
L_CB60:;
    mff_cycles(0xCB60, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B62, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCB63, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCB99, 2); call_by_address_tail(0xCB99, -1); return;
L_CB66:;
    mff_cycles(0xCB66, 7);
    a = RAM(PLAYER_ID);
    m = 0x01;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xCB6C, 4);
        m = 0x02;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xCB70, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0B72, 0xCF33, -1)) return;
            SYNC_IN;
            mff_cycles(0xCB73, 2);
            if (!z) {
                mff_cycles(0xCB75, 7);
                y = RAM(PLAYER_ID);
                a = prg_read((uint16_t)(0xE222 + y));
                n = (a) >> 7;
                z = (a) == 0;
                mff_cycles(0xCB7A, 6);
                SYNC_OUT;
                if (!mff_call_fn(0x0B7C, func_F294, 0xE000)) return;
                SYNC_IN;
                mff_cycles(0xCB7D, 4);
                a = prg_read((uint16_t)(0xE26A + y));
                n = (a) >> 7;
                z = (a) == 0;
                mff_cycles(0xCB80, 6);
                SYNC_OUT;
                if (!mff_call_fn(0x0B82, func_F440, 0xE000)) return;
                SYNC_IN;
                mff_cycles(0xCB83, 3);
                SYNC_OUT;
                nes_cpu_instruction_boundary(0xCB99, 2); call_by_address_tail(0xCB99, -1); return;
            } else {
                mff_cycles(0xCB73, 1);
            }
        } else {
            mff_cycles(0xCB6E, 1);
        }
    } else {
        mff_cycles(0xCB6A, 1);
    }
    mff_cycles(0xCB86, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE225 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCB8B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B8D, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCB8E, 4);
    a = prg_read((uint16_t)(0xE26D + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCB91, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B93, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCB94, 11);
    a = 0x11;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $CAC8 - lifted by port/tools/lift.py */
void func_CAC8(void) {
    REGS_IN;
    mff_cycles(0xCAC8, 2);
    if (!z) {
        mff_cycles(0xCACA, 4);
        m = 0x06;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xCACE, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xCAE5, 2); call_by_address_tail(0xCAE5, -1); return;
        } else {
            mff_cycles(0xCACC, 1);
        }
    } else {
        mff_cycles(0xCAC8, 1);
    }
    mff_cycles(0xCAD1, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1D7 + y));
    mff_cycles(0xCAD6, 12);
    RAM(OBJ_VX_FRAC) = a;
    RAM(OBJ_VY_FRAC) = a;
    a = prg_read((uint16_t)(0xE1DA + y));
    mff_cycles(0xCADF, 15);
    RAM(OBJ_VX) = a;
    RAM(OBJ_VY) = a;
    a = RAM(JOY_PRESSED);
    m = 0x40;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xCAEB, 4);
        m = 0x80;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xCAEF, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xCCB5, 2); call_by_address_tail(0xCCB5, -1); return;
        } else {
            mff_cycles(0xCAED, 1);
        }
        mff_cycles(0xCAF2, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xCCDA, 2); call_by_address_tail(0xCCDA, -1); return;
    } else {
        mff_cycles(0xCAE9, 1);
    }
    mff_cycles(0xCAF8, 12);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(HEALTH);
    if (a != 0) {
        mff_cycles(0xCB01, 7);
        a = RAM(JOY_HELD);
        m = 0xC0;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) { mff_cycles(0xCB05, 2); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0AF5), -1); return; }
    } else {
        mff_cycles(0xCAFF, 1);
    }
    mff_cycles(0xCB07, 5);
    a = RAM(WEAPON_HELD);
    if (a != 0) { mff_cycles(0xCB09, 1); goto L_CB66; }
    mff_cycles(0xCB0B, 5);
    a = RAM(COMBO_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0xCB0F, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xCCA2, 2); call_by_address_tail(0xCCA2, -1); return;
    } else {
        mff_cycles(0xCB0D, 1);
    }
    mff_cycles(0xCB12, 15);
    a = 0x00;
    RAM(COMBO_TIMER) = a;
    y = RAM(PLAYER_ID);
    a = RAM(PLAYER_STAND_ANIM);
    m = prg_read((uint16_t)(0xE291 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xCB1D, 2);
    if (z) {
        mff_cycles(0xCB1F, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xCBA3, 2); call_by_address_tail(0xCBA3, -1); return;
    } else {
        mff_cycles(0xCB1D, 1);
    }
    mff_cycles(0xCB22, 9);
    a = RAM(PLAYER_STAND_ANIM);
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0B26, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCB27, 11);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    m = prg_read((uint16_t)(0xE288 + y));
    c = a >= m;
    mff_cycles(0xCB2F, 2);
    if (a == m) { mff_cycles(0xCB2F, 1); goto L_CB53; }
    mff_cycles(0xCB31, 4);
    m = prg_read((uint16_t)(0xE28B + y));
    c = a >= m;
    mff_cycles(0xCB34, 2);
    if (a == m) { mff_cycles(0xCB34, 1); goto L_CB53; }
    mff_cycles(0xCB36, 4);
    m = 0x15;
    c = a >= m;
    if (a == m) { mff_cycles(0xCB38, 1); goto L_CB3F; }
    mff_cycles(0xCB3A, 4);
    a = prg_read((uint16_t)(0xE23A + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCB3D, 2);
    if (!z) { mff_cycles(0xCB3D, 1); goto L_CB60; }
L_CB3F:;
    mff_cycles(0xCB3F, 23);
    a = 0xE8;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x02;
    RAM(OBJ_VY) = a;
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = prg_read((uint16_t)(0xE23D + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCB51, 2);
    if (!z) { mff_cycles(0xCB51, 1); goto L_CB60; }
L_CB53:;
    mff_cycles(0xCB53, 16);
    a = 0x01;
    RAM(OBJ_ANIM_FRAME) = a;
    a = 0x00;
    RAM(OBJ_ANIM_TIMER) = a;
    a = prg_read((uint16_t)(0xE237 + y));
    n = (a) >> 7;
    z = (a) == 0;
L_CB60:;
    mff_cycles(0xCB60, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B62, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCB63, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCB99, 2); call_by_address_tail(0xCB99, -1); return;
L_CB66:;
    mff_cycles(0xCB66, 7);
    a = RAM(PLAYER_ID);
    m = 0x01;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xCB6C, 4);
        m = 0x02;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xCB70, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0B72, 0xCF33, -1)) return;
            SYNC_IN;
            mff_cycles(0xCB73, 2);
            if (!z) {
                mff_cycles(0xCB75, 7);
                y = RAM(PLAYER_ID);
                a = prg_read((uint16_t)(0xE222 + y));
                n = (a) >> 7;
                z = (a) == 0;
                mff_cycles(0xCB7A, 6);
                SYNC_OUT;
                if (!mff_call_fn(0x0B7C, func_F294, 0xE000)) return;
                SYNC_IN;
                mff_cycles(0xCB7D, 4);
                a = prg_read((uint16_t)(0xE26A + y));
                n = (a) >> 7;
                z = (a) == 0;
                mff_cycles(0xCB80, 6);
                SYNC_OUT;
                if (!mff_call_fn(0x0B82, func_F440, 0xE000)) return;
                SYNC_IN;
                mff_cycles(0xCB83, 3);
                SYNC_OUT;
                nes_cpu_instruction_boundary(0xCB99, 2); call_by_address_tail(0xCB99, -1); return;
            } else {
                mff_cycles(0xCB73, 1);
            }
        } else {
            mff_cycles(0xCB6E, 1);
        }
    } else {
        mff_cycles(0xCB6A, 1);
    }
    mff_cycles(0xCB86, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE225 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCB8B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B8D, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCB8E, 4);
    a = prg_read((uint16_t)(0xE26D + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCB91, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B93, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCB94, 11);
    a = 0x11;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $CAE5 - lifted by port/tools/lift.py */
void func_CAE5(void) {
    REGS_IN;
    mff_cycles(0xCAE5, 7);
    a = RAM(JOY_PRESSED);
    m = 0x40;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xCAEB, 4);
        m = 0x80;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xCAEF, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xCCB5, 2); call_by_address_tail(0xCCB5, -1); return;
        } else {
            mff_cycles(0xCAED, 1);
        }
        mff_cycles(0xCAF2, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xCCDA, 2); call_by_address_tail(0xCCDA, -1); return;
    } else {
        mff_cycles(0xCAE9, 1);
    }
    mff_cycles(0xCAF8, 12);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(HEALTH);
    if (a != 0) {
        mff_cycles(0xCB01, 7);
        a = RAM(JOY_HELD);
        m = 0xC0;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) { mff_cycles(0xCB05, 2); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0AF5), -1); return; }
    } else {
        mff_cycles(0xCAFF, 1);
    }
    mff_cycles(0xCB07, 5);
    a = RAM(WEAPON_HELD);
    if (a != 0) { mff_cycles(0xCB09, 1); goto L_CB66; }
    mff_cycles(0xCB0B, 5);
    a = RAM(COMBO_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0xCB0F, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xCCA2, 2); call_by_address_tail(0xCCA2, -1); return;
    } else {
        mff_cycles(0xCB0D, 1);
    }
    mff_cycles(0xCB12, 15);
    a = 0x00;
    RAM(COMBO_TIMER) = a;
    y = RAM(PLAYER_ID);
    a = RAM(PLAYER_STAND_ANIM);
    m = prg_read((uint16_t)(0xE291 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xCB1D, 2);
    if (z) {
        mff_cycles(0xCB1F, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xCBA3, 2); call_by_address_tail(0xCBA3, -1); return;
    } else {
        mff_cycles(0xCB1D, 1);
    }
    mff_cycles(0xCB22, 9);
    a = RAM(PLAYER_STAND_ANIM);
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0B26, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCB27, 11);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    m = prg_read((uint16_t)(0xE288 + y));
    c = a >= m;
    mff_cycles(0xCB2F, 2);
    if (a == m) { mff_cycles(0xCB2F, 1); goto L_CB53; }
    mff_cycles(0xCB31, 4);
    m = prg_read((uint16_t)(0xE28B + y));
    c = a >= m;
    mff_cycles(0xCB34, 2);
    if (a == m) { mff_cycles(0xCB34, 1); goto L_CB53; }
    mff_cycles(0xCB36, 4);
    m = 0x15;
    c = a >= m;
    if (a == m) { mff_cycles(0xCB38, 1); goto L_CB3F; }
    mff_cycles(0xCB3A, 4);
    a = prg_read((uint16_t)(0xE23A + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCB3D, 2);
    if (!z) { mff_cycles(0xCB3D, 1); goto L_CB60; }
L_CB3F:;
    mff_cycles(0xCB3F, 23);
    a = 0xE8;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x02;
    RAM(OBJ_VY) = a;
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = prg_read((uint16_t)(0xE23D + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCB51, 2);
    if (!z) { mff_cycles(0xCB51, 1); goto L_CB60; }
L_CB53:;
    mff_cycles(0xCB53, 16);
    a = 0x01;
    RAM(OBJ_ANIM_FRAME) = a;
    a = 0x00;
    RAM(OBJ_ANIM_TIMER) = a;
    a = prg_read((uint16_t)(0xE237 + y));
    n = (a) >> 7;
    z = (a) == 0;
L_CB60:;
    mff_cycles(0xCB60, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B62, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCB63, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCB99, 2); call_by_address_tail(0xCB99, -1); return;
L_CB66:;
    mff_cycles(0xCB66, 7);
    a = RAM(PLAYER_ID);
    m = 0x01;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xCB6C, 4);
        m = 0x02;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xCB70, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0B72, 0xCF33, -1)) return;
            SYNC_IN;
            mff_cycles(0xCB73, 2);
            if (!z) {
                mff_cycles(0xCB75, 7);
                y = RAM(PLAYER_ID);
                a = prg_read((uint16_t)(0xE222 + y));
                n = (a) >> 7;
                z = (a) == 0;
                mff_cycles(0xCB7A, 6);
                SYNC_OUT;
                if (!mff_call_fn(0x0B7C, func_F294, 0xE000)) return;
                SYNC_IN;
                mff_cycles(0xCB7D, 4);
                a = prg_read((uint16_t)(0xE26A + y));
                n = (a) >> 7;
                z = (a) == 0;
                mff_cycles(0xCB80, 6);
                SYNC_OUT;
                if (!mff_call_fn(0x0B82, func_F440, 0xE000)) return;
                SYNC_IN;
                mff_cycles(0xCB83, 3);
                SYNC_OUT;
                nes_cpu_instruction_boundary(0xCB99, 2); call_by_address_tail(0xCB99, -1); return;
            } else {
                mff_cycles(0xCB73, 1);
            }
        } else {
            mff_cycles(0xCB6E, 1);
        }
    } else {
        mff_cycles(0xCB6A, 1);
    }
    mff_cycles(0xCB86, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE225 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCB8B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B8D, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCB8E, 4);
    a = prg_read((uint16_t)(0xE26D + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCB91, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B93, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCB94, 11);
    a = 0x11;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $CB07 - lifted by port/tools/lift.py */
void func_CB07(void) {
    REGS_IN;
    mff_cycles(0xCB07, 5);
    a = RAM(WEAPON_HELD);
    if (a != 0) { mff_cycles(0xCB09, 1); goto L_CB66; }
    mff_cycles(0xCB0B, 5);
    a = RAM(COMBO_TIMER);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0xCB0F, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xCCA2, 2); call_by_address_tail(0xCCA2, -1); return;
    } else {
        mff_cycles(0xCB0D, 1);
    }
    mff_cycles(0xCB12, 15);
    a = 0x00;
    RAM(COMBO_TIMER) = a;
    y = RAM(PLAYER_ID);
    a = RAM(PLAYER_STAND_ANIM);
    m = prg_read((uint16_t)(0xE291 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xCB1D, 2);
    if (z) {
        mff_cycles(0xCB1F, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xCBA3, 2); call_by_address_tail(0xCBA3, -1); return;
    } else {
        mff_cycles(0xCB1D, 1);
    }
    mff_cycles(0xCB22, 9);
    a = RAM(PLAYER_STAND_ANIM);
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0B26, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCB27, 11);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    m = prg_read((uint16_t)(0xE288 + y));
    c = a >= m;
    mff_cycles(0xCB2F, 2);
    if (a == m) { mff_cycles(0xCB2F, 1); goto L_CB53; }
    mff_cycles(0xCB31, 4);
    m = prg_read((uint16_t)(0xE28B + y));
    c = a >= m;
    mff_cycles(0xCB34, 2);
    if (a == m) { mff_cycles(0xCB34, 1); goto L_CB53; }
    mff_cycles(0xCB36, 4);
    m = 0x15;
    c = a >= m;
    if (a == m) { mff_cycles(0xCB38, 1); goto L_CB3F; }
    mff_cycles(0xCB3A, 4);
    a = prg_read((uint16_t)(0xE23A + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCB3D, 2);
    if (!z) { mff_cycles(0xCB3D, 1); goto L_CB60; }
L_CB3F:;
    mff_cycles(0xCB3F, 23);
    a = 0xE8;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x02;
    RAM(OBJ_VY) = a;
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = prg_read((uint16_t)(0xE23D + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCB51, 2);
    if (!z) { mff_cycles(0xCB51, 1); goto L_CB60; }
L_CB53:;
    mff_cycles(0xCB53, 16);
    a = 0x01;
    RAM(OBJ_ANIM_FRAME) = a;
    a = 0x00;
    RAM(OBJ_ANIM_TIMER) = a;
    a = prg_read((uint16_t)(0xE237 + y));
    n = (a) >> 7;
    z = (a) == 0;
L_CB60:;
    mff_cycles(0xCB60, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B62, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCB63, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCB99, 2); call_by_address_tail(0xCB99, -1); return;
L_CB66:;
    mff_cycles(0xCB66, 7);
    a = RAM(PLAYER_ID);
    m = 0x01;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xCB6C, 4);
        m = 0x02;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xCB70, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0B72, 0xCF33, -1)) return;
            SYNC_IN;
            mff_cycles(0xCB73, 2);
            if (!z) {
                mff_cycles(0xCB75, 7);
                y = RAM(PLAYER_ID);
                a = prg_read((uint16_t)(0xE222 + y));
                n = (a) >> 7;
                z = (a) == 0;
                mff_cycles(0xCB7A, 6);
                SYNC_OUT;
                if (!mff_call_fn(0x0B7C, func_F294, 0xE000)) return;
                SYNC_IN;
                mff_cycles(0xCB7D, 4);
                a = prg_read((uint16_t)(0xE26A + y));
                n = (a) >> 7;
                z = (a) == 0;
                mff_cycles(0xCB80, 6);
                SYNC_OUT;
                if (!mff_call_fn(0x0B82, func_F440, 0xE000)) return;
                SYNC_IN;
                mff_cycles(0xCB83, 3);
                SYNC_OUT;
                nes_cpu_instruction_boundary(0xCB99, 2); call_by_address_tail(0xCB99, -1); return;
            } else {
                mff_cycles(0xCB73, 1);
            }
        } else {
            mff_cycles(0xCB6E, 1);
        }
    } else {
        mff_cycles(0xCB6A, 1);
    }
    mff_cycles(0xCB86, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE225 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCB8B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B8D, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCB8E, 4);
    a = prg_read((uint16_t)(0xE26D + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCB91, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B93, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCB94, 11);
    a = 0x11;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $CB22 - lifted by port/tools/lift.py */
void func_CB22(void) {
    REGS_IN;
    mff_cycles(0xCB22, 9);
    a = RAM(PLAYER_STAND_ANIM);
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0B26, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCB27, 11);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    m = prg_read((uint16_t)(0xE288 + y));
    c = a >= m;
    mff_cycles(0xCB2F, 2);
    if (a == m) { mff_cycles(0xCB2F, 1); goto L_CB53; }
    mff_cycles(0xCB31, 4);
    m = prg_read((uint16_t)(0xE28B + y));
    c = a >= m;
    mff_cycles(0xCB34, 2);
    if (a == m) { mff_cycles(0xCB34, 1); goto L_CB53; }
    mff_cycles(0xCB36, 4);
    m = 0x15;
    c = a >= m;
    if (a == m) { mff_cycles(0xCB38, 1); goto L_CB3F; }
    mff_cycles(0xCB3A, 4);
    a = prg_read((uint16_t)(0xE23A + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCB3D, 2);
    if (!z) { mff_cycles(0xCB3D, 1); goto L_CB60; }
L_CB3F:;
    mff_cycles(0xCB3F, 23);
    a = 0xE8;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x02;
    RAM(OBJ_VY) = a;
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = prg_read((uint16_t)(0xE23D + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCB51, 2);
    if (!z) { mff_cycles(0xCB51, 1); goto L_CB60; }
L_CB53:;
    mff_cycles(0xCB53, 16);
    a = 0x01;
    RAM(OBJ_ANIM_FRAME) = a;
    a = 0x00;
    RAM(OBJ_ANIM_TIMER) = a;
    a = prg_read((uint16_t)(0xE237 + y));
    n = (a) >> 7;
    z = (a) == 0;
L_CB60:;
    mff_cycles(0xCB60, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B62, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCB63, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCB99, 2); call_by_address_tail(0xCB99, -1); return;
}

/*  $CB99 - lifted by port/tools/lift.py */
void func_CB99(void) {
    REGS_IN;
    mff_cycles(0xCB99, 8);
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(GAME_ACTION) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xD30F, 2); call_by_address_tail(0xD30F, -1); return;
}

/*  $CBA3 - lifted by port/tools/lift.py */
void func_CBA3(void) {
    REGS_IN;
    goto L_CBA3;
L_CB22:;
    mff_cycles(0xCB22, 9);
    a = RAM(PLAYER_STAND_ANIM);
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0B26, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCB27, 11);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    m = prg_read((uint16_t)(0xE288 + y));
    c = a >= m;
    mff_cycles(0xCB2F, 2);
    if (a == m) { mff_cycles(0xCB2F, 1); goto L_CB53; }
    mff_cycles(0xCB31, 4);
    m = prg_read((uint16_t)(0xE28B + y));
    c = a >= m;
    mff_cycles(0xCB34, 2);
    if (a == m) { mff_cycles(0xCB34, 1); goto L_CB53; }
    mff_cycles(0xCB36, 4);
    m = 0x15;
    c = a >= m;
    if (a == m) { mff_cycles(0xCB38, 1); goto L_CB3F; }
    mff_cycles(0xCB3A, 4);
    a = prg_read((uint16_t)(0xE23A + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCB3D, 2);
    if (!z) { mff_cycles(0xCB3D, 1); goto L_CB60; }
L_CB3F:;
    mff_cycles(0xCB3F, 23);
    a = 0xE8;
    RAM(OBJ_VY_FRAC) = a;
    a = 0x02;
    RAM(OBJ_VY) = a;
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = prg_read((uint16_t)(0xE23D + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCB51, 2);
    if (!z) { mff_cycles(0xCB51, 1); goto L_CB60; }
L_CB53:;
    mff_cycles(0xCB53, 16);
    a = 0x01;
    RAM(OBJ_ANIM_FRAME) = a;
    a = 0x00;
    RAM(OBJ_ANIM_TIMER) = a;
    a = prg_read((uint16_t)(0xE237 + y));
    n = (a) >> 7;
    z = (a) == 0;
L_CB60:;
    mff_cycles(0xCB60, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B62, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCB63, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCB99, 2); call_by_address_tail(0xCB99, -1); return;
L_CBA3:;
    mff_cycles(0xCBA3, 7);
    y = RAM(COMBO_TARGET);
    m = 0x00;
    c = y >= m;
    n = ((uint8_t)(y - m)) >> 7;
    z = ((uint8_t)(y - m)) == 0;
    if (z) { mff_cycles(0xCBA7, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0BA0), -1); return; }
    mff_cycles(0xCBA9, 4);
    m = 0x01;
    c = y >= m;
    n = ((uint8_t)(y - m)) >> 7;
    z = ((uint8_t)(y - m)) == 0;
    if (z) { mff_cycles(0xCBAB, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0BA0), -1); return; }
    mff_cycles(0xCBAD, 4);
    m = 0x02;
    c = y >= m;
    n = ((uint8_t)(y - m)) >> 7;
    z = ((uint8_t)(y - m)) == 0;
    if (z) { mff_cycles(0xCBAF, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0BA0), -1); return; }
    mff_cycles(0xCBB1, 4);
    m = 0x0E;
    c = y >= m;
    n = ((uint8_t)(y - m)) >> 7;
    z = ((uint8_t)(y - m)) == 0;
    if (c) { mff_cycles(0xCBB3, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0BA0), -1); return; }
    mff_cycles(0xCBB5, 6);
    a = RAM(OBJ_STATE + y);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0xCBB8, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0BA0), -1); return; }
    mff_cycles(0xCBBA, 8);
    a = RAM(OBJ_ID + y);
    m = 0x06;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xCBBF, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0BA0), -1); return; }
    mff_cycles(0xCBC1, 4);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xCBC3, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0BA0), -1); return; }
    mff_cycles(0xCBC5, 4);
    m = 0x05;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xCBC7, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0BA0), -1); return; }
    mff_cycles(0xCBC9, 4);
    m = 0x07;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xCBCB, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0BA0), -1); return; }
    mff_cycles(0xCBCD, 4);
    m = 0x16;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xCBCF, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0BA0), -1); return; }
    mff_cycles(0xCBD1, 4);
    m = 0x09;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xCBD3, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0BA0), -1); return; }
    mff_cycles(0xCBD5, 4);
    m = 0x6C;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xCBD7, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0BA0), -1); return; }
    mff_cycles(0xCBD9, 4);
    m = 0x2E;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xCBDB, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0BA0), -1); return; }
    mff_cycles(0xCBDD, 4);
    m = 0x9D;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xCBDF, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0BA0), -1); return; }
    mff_cycles(0xCBE1, 6);
    a = RAM(OBJ_SPAWN_GROUP + 15);
    if (a != 0) {
        mff_cycles(0xCBE6, 10);
        y = a;
        a = RAM(OBJ_LIFE + y);
        m = 0x26;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!c) { mff_cycles(0xCBEC, 1); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0BA0), -1); return; }
    } else {
        mff_cycles(0xCBE4, 1);
    }
    mff_cycles(0xCBEE, 30);
    y = RAM(COMBO_TARGET);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(OBJ_X + y);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = RAM(OBJ_X_HI + y);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = mff_pull();
    if (!c) {
        mff_cycles(0xCC01, 6);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xCBFF, 1);
    }
    mff_cycles(0xCC06, 4);
    m = 0x20;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xCC08, 2); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0BA0), -1); return; }
    mff_cycles(0xCC0A, 12);
    a = RAM(OBJ_Y);
    c = 1;
    m = RAM(OBJ_Y + y);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xCC13, 6);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xCC11, 1);
    }
    mff_cycles(0xCC18, 4);
    m = 0x14;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xCC1A, 2); SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + 0x0BA0), -1); return; }
    mff_cycles(0xCC1C, 7);
    a = RAM(JOY_HELD);
    m = 0x44;
    c = a >= m;
    if (a == m) { mff_cycles(0xCC20, 1); goto L_CC29; }
    mff_cycles(0xCC22, 4);
    m = 0x48;
    c = a >= m;
    if (a == m) { mff_cycles(0xCC24, 1); goto L_CC29; }
    mff_cycles(0xCC26, 3);
    goto L_CB22;
L_CC29:;
    mff_cycles(0xCC29, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE216 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCC2E, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C30, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCC31, 4);
    a = prg_read((uint16_t)(0xE25E + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCC34, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C36, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCC37, 11);
    y = RAM(COMBO_TARGET);
    RAM(GRAB_SLOT) = y;
    a = RAM(PLAYER_ID);
    if (a != 0) { mff_cycles(0xCC3D, 1); goto L_CC52; }
    mff_cycles(0xCC3F, 11);
    a = RAM(OBJ_DIR + y);
    a &= 0x03;
    m = RAM(PLAYER_DIR);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xCC46, 1); goto L_CC4F; }
    mff_cycles(0xCC48, 10);
    a = RAM(PLAYER_DIR);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_DIR + y) = a;
    if (!z) { mff_cycles(0xCC4D, 1); goto L_CC52; }
L_CC4F:;
    mff_cycles(0xCC4F, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0C51, 0xCC85, -1)) return;
    SYNC_IN;
L_CC52:;
    mff_cycles(0xCC52, 16);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + y) = a;
    a = RAM(PLAYER_ID);
    if (a == 0x01) { mff_cycles(0xCC5C, 1); goto L_CC65; }
    mff_cycles(0xCC5E, 4);
    m = 0x02;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xCC60, 1); goto L_CC76; }
    mff_cycles(0xCC62, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xD7F2, 2); call_by_address_tail(0xD7F2, -1); return;
L_CC65:;
    mff_cycles(0xCC65, 11);
    a = RAM(OBJ_DIR + y);
    a &= 0x03;
    m = RAM(PLAYER_DIR);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xCC6C, 1); goto L_CC70; }
    mff_cycles(0xCC6E, 2);
    if (!z) { mff_cycles(0xCC6E, 1); goto L_CC73; }
L_CC70:;
    mff_cycles(0xCC70, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0C72, 0xCC85, -1)) return;
    SYNC_IN;
L_CC73:;
    mff_cycles(0xCC73, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xD88C, 2); call_by_address_tail(0xD88C, -1); return;
L_CC76:;
    mff_cycles(0xCC76, 11);
    a = RAM(OBJ_DIR + y);
    a &= 0x03;
    m = RAM(PLAYER_DIR);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xCC7D, 1); goto L_CC82; }
    mff_cycles(0xCC7F, 3);
    goto L_CB22;
L_CC82:;
    mff_cycles(0xCC82, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xD8A8, 2); call_by_address_tail(0xD8A8, -1); return;
}

/*  $CC85 - lifted by port/tools/lift.py */
void func_CC85(void) {
    REGS_IN;
    mff_cycles(0xCC85, 17);
    a = RAM(PLAYER_DIR);
    a &= 0x02;
    y = a;
    a = RAM(OBJ_X);
    c = 0;
    m = prg_read((uint16_t)(0xE1D3 + y));
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xCC91, 11);
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = prg_read((uint16_t)(0xE1D4 + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xCC98, 23);
    y = RAM(GRAB_SLOT);
    RAM(OBJ_X_HI + y) = a;
    a = mff_pull();
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X + y) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $CCA2 - lifted by port/tools/lift.py */
void func_CCA2(void) {
    REGS_IN;
    mff_cycles(0xCCA2, 12);
    a = 0x03;
    RAM(GAME_ACTION) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1EF + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCCAB, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0CAD, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCCAE, 4);
    a = prg_read((uint16_t)(0xE237 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCCB1, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0CB3, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCCB4, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/* player_state_default $CCB4 - lifted by port/tools/lift.py */
void func_CCB4(void) {
    REGS_IN;
    mff_cycles(0xCCB4, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $CCB5 - lifted by port/tools/lift.py */
void func_CCB5(void) {
    REGS_IN;
    mff_cycles(0xCCB5, 8);
    y = RAM(PLAYER_ID);
    a = RAM(WEAPON_HELD);
    if (a != 0) { mff_cycles(0xCCB9, 1); goto L_CCCC; }
    mff_cycles(0xCCBB, 8);
    a = RAM(OBJ_ANIM);
    m = prg_read((uint16_t)(0xE273 + y));
    c = a >= m;
    mff_cycles(0xCCC1, 2);
    if (a == m) { mff_cycles(0xCCC1, 1); goto L_CCDA; }
    mff_cycles(0xCCC3, 4);
    a = prg_read((uint16_t)(0xE1E6 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCCC6, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0CC8, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCCC9, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCCDA, 2); call_by_address_tail(0xCCDA, -1); return;
L_CCCC:;
    mff_cycles(0xCCCC, 8);
    a = RAM(OBJ_ANIM);
    m = prg_read((uint16_t)(0xE29A + y));
    c = a >= m;
    mff_cycles(0xCCD2, 2);
    if (a != m) {
        mff_cycles(0xCCD4, 4);
        a = prg_read((uint16_t)(0xE204 + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0xCCD7, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0CD9, func_F294, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xCCD2, 1);
    }
L_CCDA:;
    mff_cycles(0xCCDA, 16);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    x = 0x00;
    a = RAM(JOY_HELD);
    a &= 0x03;
    if (a != 0) {
        mff_cycles(0xCCE7, 7);
        RAM(PLAYER_DIR) = a;
        a &= 0x01;
        if (a != 0) {
            mff_cycles(0xCCED, 8);
            y = 0x00;
            n = (y) >> 7;
            z = (y) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0CF1, func_F000, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xCCF2, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xCCFA, 2); call_by_address_tail(0xCCFA, -1); return;
        } else {
            mff_cycles(0xCCEB, 1);
        }
        mff_cycles(0xCCF5, 8);
        y = 0x01;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CF9, func_F051, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xCCE5, 1);
    }
    mff_cycles(0xCCFA, 7);
    a = RAM(JOY_HELD);
    a &= 0x0C;
    if (a == 0) { mff_cycles(0xCCFE, 1); goto L_CD3B; }
    mff_cycles(0xCD00, 4);
    a &= 0x04;
    if (a == 0) { mff_cycles(0xCD02, 1); goto L_CD1A; }
    mff_cycles(0xCD04, 7);
    a = RAM(PLAYER_ID);
    m = 0x00;
    c = a >= m;
    if (a == m) { mff_cycles(0xCD08, 1); goto L_CD12; }
    mff_cycles(0xCD0A, 4);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0xCD0C, 1); goto L_CD12; }
    mff_cycles(0xCD0E, 4);
    y = 0x02;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0xCD10, 1); goto L_CD14; }
L_CD12:;
    mff_cycles(0xCD12, 2);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
L_CD14:;
    mff_cycles(0xCD14, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D16, func_F0A2, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCD17, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCD3B, 2); call_by_address_tail(0xCD3B, -1); return;
L_CD1A:;
    mff_cycles(0xCD1A, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1DD + y));
    mff_cycles(0xCD1F, 8);
    RAM(OBJ_VY_FRAC) = a;
    a = prg_read((uint16_t)(0xE1E0 + y));
    mff_cycles(0xCD25, 11);
    RAM(OBJ_VY) = a;
    a = RAM(PLAYER_ID);
    m = 0x00;
    c = a >= m;
    if (a == m) { mff_cycles(0xCD2C, 1); goto L_CD36; }
    mff_cycles(0xCD2E, 4);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0xCD30, 1); goto L_CD36; }
    mff_cycles(0xCD32, 4);
    y = 0x03;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0xCD34, 1); goto L_CD38; }
L_CD36:;
    mff_cycles(0xCD36, 2);
    y = 0x01;
    n = (y) >> 7;
    z = (y) == 0;
L_CD38:;
    mff_cycles(0xCD38, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D3A, func_F0F7, 0xE000)) return;
    SYNC_IN;
L_CD3B:;
    mff_cycles(0xCD3B, 7);
    a = RAM(JOY_HELD);
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xCD3F, 1); goto L_CD5C; }
    mff_cycles(0xCD41, 4);
    if (a == 0x05) { mff_cycles(0xCD43, 1); goto L_CD55; }
    mff_cycles(0xCD45, 4);
    if (a == 0x06) { mff_cycles(0xCD47, 1); goto L_CD55; }
    mff_cycles(0xCD49, 4);
    if (a == 0x09) { mff_cycles(0xCD4B, 1); goto L_CD55; }
    mff_cycles(0xCD4D, 4);
    m = 0x0A;
    c = a >= m;
    if (a == m) { mff_cycles(0xCD4F, 1); goto L_CD55; }
    mff_cycles(0xCD51, 4);
    a &= 0x0C;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xCD53, 1); goto L_CD5C; }
L_CD55:;
    mff_cycles(0xCD55, 2);
    y = 0x0F;
L_CD57:;
    mff_cycles(0xCD57, 6);
    y--;
    m = 0x02;
    c = y >= m;
    n = ((uint8_t)(y - m)) >> 7;
    z = ((uint8_t)(y - m)) == 0;
    if (!z) { mff_cycles(0xCD5A, 1); goto L_CD5F; }
L_CD5C:;
    mff_cycles(0xCD5C, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCEA4, 2); call_by_address_tail(0xCEA4, -1); return;
L_CD5F:;
    mff_cycles(0xCD5F, 6);
    a = RAM(OBJ_STATE + y);
    if (!(a & 0x80)) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD64, 6);
    a = RAM(OBJ_FLAGS + y);
    if (!(a & 0x80)) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD69, 8);
    a = RAM(OBJ_ID + y);
    if (a < 0x1E) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD70, 4);
    if (a == 0x6F) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD74, 4);
    if (a == 0x8E) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD78, 4);
    if (a == 0x9D) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD7C, 4);
    if (a == 0x2E) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD80, 4);
    if (a == 0x61) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD84, 4);
    if (a == 0x69) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD88, 4);
    if (a == 0x6A) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD8C, 4);
    if (a == 0x6C) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD90, 4);
    if (a == 0x6D) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD94, 4);
    if (a == 0x66) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD98, 4);
    if (a == 0x91) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD9C, 4);
    if (a == 0x72) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDA0, 6);
    a = RAM(OBJ_INVULN + y);
    if (a != 0) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDA5, 8);
    a = RAM(OBJ_ANIM + y);
    if (a == 0xA6) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDAC, 4);
    if (a == 0xAC) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDB0, 4);
    if (a == 0x84) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDB4, 4);
    if (a == 0xE7) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDB8, 4);
    if (a == 0xBE) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDBC, 12);
    a = RAM(OBJ_Y);
    c = 1;
    m = RAM(OBJ_Y + y);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xCDC5, 6);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xCDC3, 1);
    }
    mff_cycles(0xCDCA, 4);
    if (a >= 0x08) { mff_cycles(0xCDCC, 2); goto L_CE11; }
    mff_cycles(0xCDCE, 27);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(OBJ_X + y);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = RAM(OBJ_X_HI + y);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = mff_pull();
    if (!c) {
        mff_cycles(0xCDDF, 6);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xCDDD, 1);
    }
    mff_cycles(0xCDE4, 10);
    RAM(TMP0) = a;
    a = RAM(PLAYER_ID);
    if (a != 0x02) { mff_cycles(0xCDEA, 1); goto L_CDF5; }
    mff_cycles(0xCDEC, 7);
    a = RAM(TMP0);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xCDF0, 2); goto L_CE11; }
    mff_cycles(0xCDF2, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCDFB, 2); call_by_address_tail(0xCDFB, -1); return;
L_CDF5:;
    mff_cycles(0xCDF5, 7);
    a = RAM(TMP0);
    if (a >= 0x07) { mff_cycles(0xCDF9, 2); goto L_CE11; }
    mff_cycles(0xCDFB, 12);
    a = RAM(OBJ_Y);
    c = 1;
    m = RAM(OBJ_Y + y);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xCE04, 6);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xCE02, 1);
    }
    mff_cycles(0xCE09, 4);
    if (a == 0x00) { mff_cycles(0xCE0B, 1); goto L_CE14; }
    mff_cycles(0xCE0D, 4);
    if (a < 0x08) { mff_cycles(0xCE0F, 1); goto L_CE14; }
L_CE11:;
    mff_cycles(0xCE11, 3);
    goto L_CD57;
L_CE14:;
    mff_cycles(0xCE14, 5);
    a = RAM(CAMERA_LOCK);
    if (a == 0) { mff_cycles(0xCE16, 1); goto L_CE38; }
    mff_cycles(0xCE18, 7);
    a = RAM(PLAYER_ID);
    if (a != 0x02) { mff_cycles(0xCE1C, 1); goto L_CE42; }
    mff_cycles(0xCE1E, 7);
    a = RAM(PLAYER_DIR);
    if (a != 0x02) { mff_cycles(0xCE22, 1); goto L_CE2E; }
    mff_cycles(0xCE24, 8);
    a = RAM(OBJ_X);
    if (a >= 0x28) { mff_cycles(0xCE29, 1); goto L_CE42; }
    mff_cycles(0xCE2B, 3);
    goto L_CE11;
L_CE2E:;
    mff_cycles(0xCE2E, 8);
    a = RAM(OBJ_X);
    if (a < 0xE0) { mff_cycles(0xCE33, 1); goto L_CE42; }
    mff_cycles(0xCE35, 3);
    goto L_CE11;
L_CE38:;
    mff_cycles(0xCE38, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    if (a < 0x24) { mff_cycles(0xCE11, 1); goto L_CE11; }
L_CE42:;
    mff_cycles(0xCE42, 29);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + y) = a;
    RAM(GRAB_SLOT) = y;
    a = RAM(PLAYER_DIR);
    a &= 0x02;
    y = a;
    a = RAM(OBJ_X);
    c = 0;
    m = prg_read((uint16_t)(0xE1D3 + y));
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xCE56, 11);
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = prg_read((uint16_t)(0xE1D4 + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xCE5D, 24);
    y = RAM(GRAB_SLOT);
    RAM(OBJ_X_HI + y) = a;
    a = mff_pull();
    RAM(OBJ_X + y) = a;
    a = RAM(JOY_HELD);
    a &= 0x03;
    if (a == 0) { mff_cycles(0xCE6A, 1); goto L_CEA4; }
    mff_cycles(0xCE6C, 12);
    a = 0x04;
    RAM(GAME_ACTION) = a;
    y = RAM(PLAYER_ID);
    m = 0x02;
    c = y >= m;
    if (y != m) { mff_cycles(0xCE74, 1); goto L_CE85; }
    mff_cycles(0xCE76, 20);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_Y + y);
    c = 1;
    m = 0x06;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_Y + y) = a;
    a = 0x5A;
    if (a != 0) { mff_cycles(0xCE83, 1); goto L_CE89; }
L_CE85:;
    mff_cycles(0xCE85, 4);
    a = 0x3C;
    if (a != 0) {
        mff_cycles(0xCE87, 1);
    }
L_CE89:;
    mff_cycles(0xCE89, 25);
    RAM(GRAB_TIMER) = a;
    y = RAM(GRAB_SLOT);
    RAM(OBJ_HELD + y) = a;
    a = 0x00;
    RAM(OBJ_ATTACK + y) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE213 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCE9A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0E9C, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCE9D, 4);
    a = prg_read((uint16_t)(0xE25B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEA0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EA2, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCEA3, 6);
    SYNC_OUT;
    mff_rts(); return;
L_CEA4:;
    mff_cycles(0xCEA4, 7);
    a = RAM(JOY_HELD);
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xCEA8, 1); goto L_CEC1; }
    mff_cycles(0xCEAA, 12);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(WEAPON_HELD);
    if (a != 0) { mff_cycles(0xCEB1, 1); goto L_CEC2; }
    mff_cycles(0xCEB3, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1E3 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEB8, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EBA, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCEBB, 4);
    a = prg_read((uint16_t)(0xE22B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEBE, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EC0, func_F440, 0xE000)) return;
    SYNC_IN;
L_CEC1:;
    mff_cycles(0xCEC1, 6);
    SYNC_OUT;
    mff_rts(); return;
L_CEC2:;
    mff_cycles(0xCEC2, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE21F + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEC7, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EC9, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCECA, 4);
    a = prg_read((uint16_t)(0xE267 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCECD, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0ECF, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCED0, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $CCDA - lifted by port/tools/lift.py */
void func_CCDA(void) {
    REGS_IN;
    mff_cycles(0xCCDA, 16);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    x = 0x00;
    a = RAM(JOY_HELD);
    a &= 0x03;
    if (a != 0) {
        mff_cycles(0xCCE7, 7);
        RAM(PLAYER_DIR) = a;
        a &= 0x01;
        if (a != 0) {
            mff_cycles(0xCCED, 8);
            y = 0x00;
            n = (y) >> 7;
            z = (y) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0CF1, func_F000, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xCCF2, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xCCFA, 2); call_by_address_tail(0xCCFA, -1); return;
        } else {
            mff_cycles(0xCCEB, 1);
        }
        mff_cycles(0xCCF5, 8);
        y = 0x01;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CF9, func_F051, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xCCE5, 1);
    }
    mff_cycles(0xCCFA, 7);
    a = RAM(JOY_HELD);
    a &= 0x0C;
    if (a == 0) { mff_cycles(0xCCFE, 1); goto L_CD3B; }
    mff_cycles(0xCD00, 4);
    a &= 0x04;
    if (a == 0) { mff_cycles(0xCD02, 1); goto L_CD1A; }
    mff_cycles(0xCD04, 7);
    a = RAM(PLAYER_ID);
    m = 0x00;
    c = a >= m;
    if (a == m) { mff_cycles(0xCD08, 1); goto L_CD12; }
    mff_cycles(0xCD0A, 4);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0xCD0C, 1); goto L_CD12; }
    mff_cycles(0xCD0E, 4);
    y = 0x02;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0xCD10, 1); goto L_CD14; }
L_CD12:;
    mff_cycles(0xCD12, 2);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
L_CD14:;
    mff_cycles(0xCD14, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D16, func_F0A2, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCD17, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCD3B, 2); call_by_address_tail(0xCD3B, -1); return;
L_CD1A:;
    mff_cycles(0xCD1A, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1DD + y));
    mff_cycles(0xCD1F, 8);
    RAM(OBJ_VY_FRAC) = a;
    a = prg_read((uint16_t)(0xE1E0 + y));
    mff_cycles(0xCD25, 11);
    RAM(OBJ_VY) = a;
    a = RAM(PLAYER_ID);
    m = 0x00;
    c = a >= m;
    if (a == m) { mff_cycles(0xCD2C, 1); goto L_CD36; }
    mff_cycles(0xCD2E, 4);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0xCD30, 1); goto L_CD36; }
    mff_cycles(0xCD32, 4);
    y = 0x03;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0xCD34, 1); goto L_CD38; }
L_CD36:;
    mff_cycles(0xCD36, 2);
    y = 0x01;
    n = (y) >> 7;
    z = (y) == 0;
L_CD38:;
    mff_cycles(0xCD38, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D3A, func_F0F7, 0xE000)) return;
    SYNC_IN;
L_CD3B:;
    mff_cycles(0xCD3B, 7);
    a = RAM(JOY_HELD);
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xCD3F, 1); goto L_CD5C; }
    mff_cycles(0xCD41, 4);
    if (a == 0x05) { mff_cycles(0xCD43, 1); goto L_CD55; }
    mff_cycles(0xCD45, 4);
    if (a == 0x06) { mff_cycles(0xCD47, 1); goto L_CD55; }
    mff_cycles(0xCD49, 4);
    if (a == 0x09) { mff_cycles(0xCD4B, 1); goto L_CD55; }
    mff_cycles(0xCD4D, 4);
    m = 0x0A;
    c = a >= m;
    if (a == m) { mff_cycles(0xCD4F, 1); goto L_CD55; }
    mff_cycles(0xCD51, 4);
    a &= 0x0C;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xCD53, 1); goto L_CD5C; }
L_CD55:;
    mff_cycles(0xCD55, 2);
    y = 0x0F;
L_CD57:;
    mff_cycles(0xCD57, 6);
    y--;
    m = 0x02;
    c = y >= m;
    n = ((uint8_t)(y - m)) >> 7;
    z = ((uint8_t)(y - m)) == 0;
    if (!z) { mff_cycles(0xCD5A, 1); goto L_CD5F; }
L_CD5C:;
    mff_cycles(0xCD5C, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCEA4, 2); call_by_address_tail(0xCEA4, -1); return;
L_CD5F:;
    mff_cycles(0xCD5F, 6);
    a = RAM(OBJ_STATE + y);
    if (!(a & 0x80)) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD64, 6);
    a = RAM(OBJ_FLAGS + y);
    if (!(a & 0x80)) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD69, 8);
    a = RAM(OBJ_ID + y);
    if (a < 0x1E) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD70, 4);
    if (a == 0x6F) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD74, 4);
    if (a == 0x8E) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD78, 4);
    if (a == 0x9D) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD7C, 4);
    if (a == 0x2E) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD80, 4);
    if (a == 0x61) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD84, 4);
    if (a == 0x69) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD88, 4);
    if (a == 0x6A) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD8C, 4);
    if (a == 0x6C) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD90, 4);
    if (a == 0x6D) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD94, 4);
    if (a == 0x66) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD98, 4);
    if (a == 0x91) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD9C, 4);
    if (a == 0x72) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDA0, 6);
    a = RAM(OBJ_INVULN + y);
    if (a != 0) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDA5, 8);
    a = RAM(OBJ_ANIM + y);
    if (a == 0xA6) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDAC, 4);
    if (a == 0xAC) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDB0, 4);
    if (a == 0x84) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDB4, 4);
    if (a == 0xE7) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDB8, 4);
    if (a == 0xBE) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDBC, 12);
    a = RAM(OBJ_Y);
    c = 1;
    m = RAM(OBJ_Y + y);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xCDC5, 6);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xCDC3, 1);
    }
    mff_cycles(0xCDCA, 4);
    if (a >= 0x08) { mff_cycles(0xCDCC, 2); goto L_CE11; }
    mff_cycles(0xCDCE, 27);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(OBJ_X + y);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = RAM(OBJ_X_HI + y);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = mff_pull();
    if (!c) {
        mff_cycles(0xCDDF, 6);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xCDDD, 1);
    }
    mff_cycles(0xCDE4, 10);
    RAM(TMP0) = a;
    a = RAM(PLAYER_ID);
    if (a != 0x02) { mff_cycles(0xCDEA, 1); goto L_CDF5; }
    mff_cycles(0xCDEC, 7);
    a = RAM(TMP0);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xCDF0, 2); goto L_CE11; }
    mff_cycles(0xCDF2, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCDFB, 2); call_by_address_tail(0xCDFB, -1); return;
L_CDF5:;
    mff_cycles(0xCDF5, 7);
    a = RAM(TMP0);
    if (a >= 0x07) { mff_cycles(0xCDF9, 2); goto L_CE11; }
    mff_cycles(0xCDFB, 12);
    a = RAM(OBJ_Y);
    c = 1;
    m = RAM(OBJ_Y + y);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xCE04, 6);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xCE02, 1);
    }
    mff_cycles(0xCE09, 4);
    if (a == 0x00) { mff_cycles(0xCE0B, 1); goto L_CE14; }
    mff_cycles(0xCE0D, 4);
    if (a < 0x08) { mff_cycles(0xCE0F, 1); goto L_CE14; }
L_CE11:;
    mff_cycles(0xCE11, 3);
    goto L_CD57;
L_CE14:;
    mff_cycles(0xCE14, 5);
    a = RAM(CAMERA_LOCK);
    if (a == 0) { mff_cycles(0xCE16, 1); goto L_CE38; }
    mff_cycles(0xCE18, 7);
    a = RAM(PLAYER_ID);
    if (a != 0x02) { mff_cycles(0xCE1C, 1); goto L_CE42; }
    mff_cycles(0xCE1E, 7);
    a = RAM(PLAYER_DIR);
    if (a != 0x02) { mff_cycles(0xCE22, 1); goto L_CE2E; }
    mff_cycles(0xCE24, 8);
    a = RAM(OBJ_X);
    if (a >= 0x28) { mff_cycles(0xCE29, 1); goto L_CE42; }
    mff_cycles(0xCE2B, 3);
    goto L_CE11;
L_CE2E:;
    mff_cycles(0xCE2E, 8);
    a = RAM(OBJ_X);
    if (a < 0xE0) { mff_cycles(0xCE33, 1); goto L_CE42; }
    mff_cycles(0xCE35, 3);
    goto L_CE11;
L_CE38:;
    mff_cycles(0xCE38, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    if (a < 0x24) { mff_cycles(0xCE11, 1); goto L_CE11; }
L_CE42:;
    mff_cycles(0xCE42, 29);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + y) = a;
    RAM(GRAB_SLOT) = y;
    a = RAM(PLAYER_DIR);
    a &= 0x02;
    y = a;
    a = RAM(OBJ_X);
    c = 0;
    m = prg_read((uint16_t)(0xE1D3 + y));
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xCE56, 11);
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = prg_read((uint16_t)(0xE1D4 + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xCE5D, 24);
    y = RAM(GRAB_SLOT);
    RAM(OBJ_X_HI + y) = a;
    a = mff_pull();
    RAM(OBJ_X + y) = a;
    a = RAM(JOY_HELD);
    a &= 0x03;
    if (a == 0) { mff_cycles(0xCE6A, 1); goto L_CEA4; }
    mff_cycles(0xCE6C, 12);
    a = 0x04;
    RAM(GAME_ACTION) = a;
    y = RAM(PLAYER_ID);
    m = 0x02;
    c = y >= m;
    if (y != m) { mff_cycles(0xCE74, 1); goto L_CE85; }
    mff_cycles(0xCE76, 20);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_Y + y);
    c = 1;
    m = 0x06;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_Y + y) = a;
    a = 0x5A;
    if (a != 0) { mff_cycles(0xCE83, 1); goto L_CE89; }
L_CE85:;
    mff_cycles(0xCE85, 4);
    a = 0x3C;
    if (a != 0) {
        mff_cycles(0xCE87, 1);
    }
L_CE89:;
    mff_cycles(0xCE89, 25);
    RAM(GRAB_TIMER) = a;
    y = RAM(GRAB_SLOT);
    RAM(OBJ_HELD + y) = a;
    a = 0x00;
    RAM(OBJ_ATTACK + y) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE213 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCE9A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0E9C, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCE9D, 4);
    a = prg_read((uint16_t)(0xE25B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEA0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EA2, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCEA3, 6);
    SYNC_OUT;
    mff_rts(); return;
L_CEA4:;
    mff_cycles(0xCEA4, 7);
    a = RAM(JOY_HELD);
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xCEA8, 1); goto L_CEC1; }
    mff_cycles(0xCEAA, 12);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(WEAPON_HELD);
    if (a != 0) { mff_cycles(0xCEB1, 1); goto L_CEC2; }
    mff_cycles(0xCEB3, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1E3 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEB8, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EBA, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCEBB, 4);
    a = prg_read((uint16_t)(0xE22B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEBE, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EC0, func_F440, 0xE000)) return;
    SYNC_IN;
L_CEC1:;
    mff_cycles(0xCEC1, 6);
    SYNC_OUT;
    mff_rts(); return;
L_CEC2:;
    mff_cycles(0xCEC2, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE21F + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEC7, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EC9, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCECA, 4);
    a = prg_read((uint16_t)(0xE267 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCECD, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0ECF, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCED0, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $CCFA - lifted by port/tools/lift.py */
void func_CCFA(void) {
    REGS_IN;
    mff_cycles(0xCCFA, 7);
    a = RAM(JOY_HELD);
    a &= 0x0C;
    if (a == 0) { mff_cycles(0xCCFE, 1); goto L_CD3B; }
    mff_cycles(0xCD00, 4);
    a &= 0x04;
    if (a == 0) { mff_cycles(0xCD02, 1); goto L_CD1A; }
    mff_cycles(0xCD04, 7);
    a = RAM(PLAYER_ID);
    m = 0x00;
    c = a >= m;
    if (a == m) { mff_cycles(0xCD08, 1); goto L_CD12; }
    mff_cycles(0xCD0A, 4);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0xCD0C, 1); goto L_CD12; }
    mff_cycles(0xCD0E, 4);
    y = 0x02;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0xCD10, 1); goto L_CD14; }
L_CD12:;
    mff_cycles(0xCD12, 2);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
L_CD14:;
    mff_cycles(0xCD14, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D16, func_F0A2, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCD17, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCD3B, 2); call_by_address_tail(0xCD3B, -1); return;
L_CD1A:;
    mff_cycles(0xCD1A, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1DD + y));
    mff_cycles(0xCD1F, 8);
    RAM(OBJ_VY_FRAC) = a;
    a = prg_read((uint16_t)(0xE1E0 + y));
    mff_cycles(0xCD25, 11);
    RAM(OBJ_VY) = a;
    a = RAM(PLAYER_ID);
    m = 0x00;
    c = a >= m;
    if (a == m) { mff_cycles(0xCD2C, 1); goto L_CD36; }
    mff_cycles(0xCD2E, 4);
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0xCD30, 1); goto L_CD36; }
    mff_cycles(0xCD32, 4);
    y = 0x03;
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) { mff_cycles(0xCD34, 1); goto L_CD38; }
L_CD36:;
    mff_cycles(0xCD36, 2);
    y = 0x01;
    n = (y) >> 7;
    z = (y) == 0;
L_CD38:;
    mff_cycles(0xCD38, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D3A, func_F0F7, 0xE000)) return;
    SYNC_IN;
L_CD3B:;
    mff_cycles(0xCD3B, 7);
    a = RAM(JOY_HELD);
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xCD3F, 1); goto L_CD5C; }
    mff_cycles(0xCD41, 4);
    if (a == 0x05) { mff_cycles(0xCD43, 1); goto L_CD55; }
    mff_cycles(0xCD45, 4);
    if (a == 0x06) { mff_cycles(0xCD47, 1); goto L_CD55; }
    mff_cycles(0xCD49, 4);
    if (a == 0x09) { mff_cycles(0xCD4B, 1); goto L_CD55; }
    mff_cycles(0xCD4D, 4);
    m = 0x0A;
    c = a >= m;
    if (a == m) { mff_cycles(0xCD4F, 1); goto L_CD55; }
    mff_cycles(0xCD51, 4);
    a &= 0x0C;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xCD53, 1); goto L_CD5C; }
L_CD55:;
    mff_cycles(0xCD55, 2);
    y = 0x0F;
L_CD57:;
    mff_cycles(0xCD57, 6);
    y--;
    m = 0x02;
    c = y >= m;
    n = ((uint8_t)(y - m)) >> 7;
    z = ((uint8_t)(y - m)) == 0;
    if (!z) { mff_cycles(0xCD5A, 1); goto L_CD5F; }
L_CD5C:;
    mff_cycles(0xCD5C, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCEA4, 2); call_by_address_tail(0xCEA4, -1); return;
L_CD5F:;
    mff_cycles(0xCD5F, 6);
    a = RAM(OBJ_STATE + y);
    if (!(a & 0x80)) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD64, 6);
    a = RAM(OBJ_FLAGS + y);
    if (!(a & 0x80)) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD69, 8);
    a = RAM(OBJ_ID + y);
    if (a < 0x1E) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD70, 4);
    if (a == 0x6F) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD74, 4);
    if (a == 0x8E) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD78, 4);
    if (a == 0x9D) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD7C, 4);
    if (a == 0x2E) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD80, 4);
    if (a == 0x61) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD84, 4);
    if (a == 0x69) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD88, 4);
    if (a == 0x6A) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD8C, 4);
    if (a == 0x6C) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD90, 4);
    if (a == 0x6D) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD94, 4);
    if (a == 0x66) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD98, 4);
    if (a == 0x91) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD9C, 4);
    if (a == 0x72) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDA0, 6);
    a = RAM(OBJ_INVULN + y);
    if (a != 0) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDA5, 8);
    a = RAM(OBJ_ANIM + y);
    if (a == 0xA6) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDAC, 4);
    if (a == 0xAC) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDB0, 4);
    if (a == 0x84) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDB4, 4);
    if (a == 0xE7) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDB8, 4);
    if (a == 0xBE) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDBC, 12);
    a = RAM(OBJ_Y);
    c = 1;
    m = RAM(OBJ_Y + y);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xCDC5, 6);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xCDC3, 1);
    }
    mff_cycles(0xCDCA, 4);
    if (a >= 0x08) { mff_cycles(0xCDCC, 2); goto L_CE11; }
    mff_cycles(0xCDCE, 27);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(OBJ_X + y);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = RAM(OBJ_X_HI + y);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = mff_pull();
    if (!c) {
        mff_cycles(0xCDDF, 6);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xCDDD, 1);
    }
    mff_cycles(0xCDE4, 10);
    RAM(TMP0) = a;
    a = RAM(PLAYER_ID);
    if (a != 0x02) { mff_cycles(0xCDEA, 1); goto L_CDF5; }
    mff_cycles(0xCDEC, 7);
    a = RAM(TMP0);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xCDF0, 2); goto L_CE11; }
    mff_cycles(0xCDF2, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCDFB, 2); call_by_address_tail(0xCDFB, -1); return;
L_CDF5:;
    mff_cycles(0xCDF5, 7);
    a = RAM(TMP0);
    if (a >= 0x07) { mff_cycles(0xCDF9, 2); goto L_CE11; }
    mff_cycles(0xCDFB, 12);
    a = RAM(OBJ_Y);
    c = 1;
    m = RAM(OBJ_Y + y);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xCE04, 6);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xCE02, 1);
    }
    mff_cycles(0xCE09, 4);
    if (a == 0x00) { mff_cycles(0xCE0B, 1); goto L_CE14; }
    mff_cycles(0xCE0D, 4);
    if (a < 0x08) { mff_cycles(0xCE0F, 1); goto L_CE14; }
L_CE11:;
    mff_cycles(0xCE11, 3);
    goto L_CD57;
L_CE14:;
    mff_cycles(0xCE14, 5);
    a = RAM(CAMERA_LOCK);
    if (a == 0) { mff_cycles(0xCE16, 1); goto L_CE38; }
    mff_cycles(0xCE18, 7);
    a = RAM(PLAYER_ID);
    if (a != 0x02) { mff_cycles(0xCE1C, 1); goto L_CE42; }
    mff_cycles(0xCE1E, 7);
    a = RAM(PLAYER_DIR);
    if (a != 0x02) { mff_cycles(0xCE22, 1); goto L_CE2E; }
    mff_cycles(0xCE24, 8);
    a = RAM(OBJ_X);
    if (a >= 0x28) { mff_cycles(0xCE29, 1); goto L_CE42; }
    mff_cycles(0xCE2B, 3);
    goto L_CE11;
L_CE2E:;
    mff_cycles(0xCE2E, 8);
    a = RAM(OBJ_X);
    if (a < 0xE0) { mff_cycles(0xCE33, 1); goto L_CE42; }
    mff_cycles(0xCE35, 3);
    goto L_CE11;
L_CE38:;
    mff_cycles(0xCE38, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    if (a < 0x24) { mff_cycles(0xCE11, 1); goto L_CE11; }
L_CE42:;
    mff_cycles(0xCE42, 29);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + y) = a;
    RAM(GRAB_SLOT) = y;
    a = RAM(PLAYER_DIR);
    a &= 0x02;
    y = a;
    a = RAM(OBJ_X);
    c = 0;
    m = prg_read((uint16_t)(0xE1D3 + y));
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xCE56, 11);
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = prg_read((uint16_t)(0xE1D4 + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xCE5D, 24);
    y = RAM(GRAB_SLOT);
    RAM(OBJ_X_HI + y) = a;
    a = mff_pull();
    RAM(OBJ_X + y) = a;
    a = RAM(JOY_HELD);
    a &= 0x03;
    if (a == 0) { mff_cycles(0xCE6A, 1); goto L_CEA4; }
    mff_cycles(0xCE6C, 12);
    a = 0x04;
    RAM(GAME_ACTION) = a;
    y = RAM(PLAYER_ID);
    m = 0x02;
    c = y >= m;
    if (y != m) { mff_cycles(0xCE74, 1); goto L_CE85; }
    mff_cycles(0xCE76, 20);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_Y + y);
    c = 1;
    m = 0x06;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_Y + y) = a;
    a = 0x5A;
    if (a != 0) { mff_cycles(0xCE83, 1); goto L_CE89; }
L_CE85:;
    mff_cycles(0xCE85, 4);
    a = 0x3C;
    if (a != 0) {
        mff_cycles(0xCE87, 1);
    }
L_CE89:;
    mff_cycles(0xCE89, 25);
    RAM(GRAB_TIMER) = a;
    y = RAM(GRAB_SLOT);
    RAM(OBJ_HELD + y) = a;
    a = 0x00;
    RAM(OBJ_ATTACK + y) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE213 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCE9A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0E9C, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCE9D, 4);
    a = prg_read((uint16_t)(0xE25B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEA0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EA2, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCEA3, 6);
    SYNC_OUT;
    mff_rts(); return;
L_CEA4:;
    mff_cycles(0xCEA4, 7);
    a = RAM(JOY_HELD);
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xCEA8, 1); goto L_CEC1; }
    mff_cycles(0xCEAA, 12);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(WEAPON_HELD);
    if (a != 0) { mff_cycles(0xCEB1, 1); goto L_CEC2; }
    mff_cycles(0xCEB3, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1E3 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEB8, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EBA, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCEBB, 4);
    a = prg_read((uint16_t)(0xE22B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEBE, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EC0, func_F440, 0xE000)) return;
    SYNC_IN;
L_CEC1:;
    mff_cycles(0xCEC1, 6);
    SYNC_OUT;
    mff_rts(); return;
L_CEC2:;
    mff_cycles(0xCEC2, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE21F + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEC7, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EC9, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCECA, 4);
    a = prg_read((uint16_t)(0xE267 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCECD, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0ECF, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCED0, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $CD3B - lifted by port/tools/lift.py */
void func_CD3B(void) {
    REGS_IN;
    mff_cycles(0xCD3B, 7);
    a = RAM(JOY_HELD);
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xCD3F, 1); goto L_CD5C; }
    mff_cycles(0xCD41, 4);
    if (a == 0x05) { mff_cycles(0xCD43, 1); goto L_CD55; }
    mff_cycles(0xCD45, 4);
    if (a == 0x06) { mff_cycles(0xCD47, 1); goto L_CD55; }
    mff_cycles(0xCD49, 4);
    if (a == 0x09) { mff_cycles(0xCD4B, 1); goto L_CD55; }
    mff_cycles(0xCD4D, 4);
    m = 0x0A;
    c = a >= m;
    if (a == m) { mff_cycles(0xCD4F, 1); goto L_CD55; }
    mff_cycles(0xCD51, 4);
    a &= 0x0C;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xCD53, 1); goto L_CD5C; }
L_CD55:;
    mff_cycles(0xCD55, 2);
    y = 0x0F;
L_CD57:;
    mff_cycles(0xCD57, 6);
    y--;
    m = 0x02;
    c = y >= m;
    n = ((uint8_t)(y - m)) >> 7;
    z = ((uint8_t)(y - m)) == 0;
    if (!z) { mff_cycles(0xCD5A, 1); goto L_CD5F; }
L_CD5C:;
    mff_cycles(0xCD5C, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCEA4, 2); call_by_address_tail(0xCEA4, -1); return;
L_CD5F:;
    mff_cycles(0xCD5F, 6);
    a = RAM(OBJ_STATE + y);
    if (!(a & 0x80)) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD64, 6);
    a = RAM(OBJ_FLAGS + y);
    if (!(a & 0x80)) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD69, 8);
    a = RAM(OBJ_ID + y);
    if (a < 0x1E) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD70, 4);
    if (a == 0x6F) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD74, 4);
    if (a == 0x8E) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD78, 4);
    if (a == 0x9D) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD7C, 4);
    if (a == 0x2E) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD80, 4);
    if (a == 0x61) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD84, 4);
    if (a == 0x69) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD88, 4);
    if (a == 0x6A) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD8C, 4);
    if (a == 0x6C) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD90, 4);
    if (a == 0x6D) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD94, 4);
    if (a == 0x66) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD98, 4);
    if (a == 0x91) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCD9C, 4);
    if (a == 0x72) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDA0, 6);
    a = RAM(OBJ_INVULN + y);
    if (a != 0) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDA5, 8);
    a = RAM(OBJ_ANIM + y);
    if (a == 0xA6) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDAC, 4);
    if (a == 0xAC) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDB0, 4);
    if (a == 0x84) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDB4, 4);
    if (a == 0xE7) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDB8, 4);
    if (a == 0xBE) { mff_cycles(0xCD57, 1); goto L_CD57; }
    mff_cycles(0xCDBC, 12);
    a = RAM(OBJ_Y);
    c = 1;
    m = RAM(OBJ_Y + y);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xCDC5, 6);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xCDC3, 1);
    }
    mff_cycles(0xCDCA, 4);
    if (a >= 0x08) { mff_cycles(0xCDCC, 2); goto L_CE11; }
    mff_cycles(0xCDCE, 27);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(OBJ_X + y);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = RAM(OBJ_X_HI + y);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = mff_pull();
    if (!c) {
        mff_cycles(0xCDDF, 6);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xCDDD, 1);
    }
    mff_cycles(0xCDE4, 10);
    RAM(TMP0) = a;
    a = RAM(PLAYER_ID);
    if (a != 0x02) { mff_cycles(0xCDEA, 1); goto L_CDF5; }
    mff_cycles(0xCDEC, 7);
    a = RAM(TMP0);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xCDF0, 2); goto L_CE11; }
    mff_cycles(0xCDF2, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCDFB, 2); call_by_address_tail(0xCDFB, -1); return;
L_CDF5:;
    mff_cycles(0xCDF5, 7);
    a = RAM(TMP0);
    if (a >= 0x07) { mff_cycles(0xCDF9, 2); goto L_CE11; }
    mff_cycles(0xCDFB, 12);
    a = RAM(OBJ_Y);
    c = 1;
    m = RAM(OBJ_Y + y);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xCE04, 6);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xCE02, 1);
    }
    mff_cycles(0xCE09, 4);
    if (a == 0x00) { mff_cycles(0xCE0B, 1); goto L_CE14; }
    mff_cycles(0xCE0D, 4);
    if (a < 0x08) { mff_cycles(0xCE0F, 1); goto L_CE14; }
L_CE11:;
    mff_cycles(0xCE11, 3);
    goto L_CD57;
L_CE14:;
    mff_cycles(0xCE14, 5);
    a = RAM(CAMERA_LOCK);
    if (a == 0) { mff_cycles(0xCE16, 1); goto L_CE38; }
    mff_cycles(0xCE18, 7);
    a = RAM(PLAYER_ID);
    if (a != 0x02) { mff_cycles(0xCE1C, 1); goto L_CE42; }
    mff_cycles(0xCE1E, 7);
    a = RAM(PLAYER_DIR);
    if (a != 0x02) { mff_cycles(0xCE22, 1); goto L_CE2E; }
    mff_cycles(0xCE24, 8);
    a = RAM(OBJ_X);
    if (a >= 0x28) { mff_cycles(0xCE29, 1); goto L_CE42; }
    mff_cycles(0xCE2B, 3);
    goto L_CE11;
L_CE2E:;
    mff_cycles(0xCE2E, 8);
    a = RAM(OBJ_X);
    if (a < 0xE0) { mff_cycles(0xCE33, 1); goto L_CE42; }
    mff_cycles(0xCE35, 3);
    goto L_CE11;
L_CE38:;
    mff_cycles(0xCE38, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    if (a < 0x24) { mff_cycles(0xCE11, 1); goto L_CE11; }
L_CE42:;
    mff_cycles(0xCE42, 29);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + y) = a;
    RAM(GRAB_SLOT) = y;
    a = RAM(PLAYER_DIR);
    a &= 0x02;
    y = a;
    a = RAM(OBJ_X);
    c = 0;
    m = prg_read((uint16_t)(0xE1D3 + y));
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xCE56, 11);
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = prg_read((uint16_t)(0xE1D4 + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xCE5D, 24);
    y = RAM(GRAB_SLOT);
    RAM(OBJ_X_HI + y) = a;
    a = mff_pull();
    RAM(OBJ_X + y) = a;
    a = RAM(JOY_HELD);
    a &= 0x03;
    if (a == 0) { mff_cycles(0xCE6A, 1); goto L_CEA4; }
    mff_cycles(0xCE6C, 12);
    a = 0x04;
    RAM(GAME_ACTION) = a;
    y = RAM(PLAYER_ID);
    m = 0x02;
    c = y >= m;
    if (y != m) { mff_cycles(0xCE74, 1); goto L_CE85; }
    mff_cycles(0xCE76, 20);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_Y + y);
    c = 1;
    m = 0x06;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_Y + y) = a;
    a = 0x5A;
    if (a != 0) { mff_cycles(0xCE83, 1); goto L_CE89; }
L_CE85:;
    mff_cycles(0xCE85, 4);
    a = 0x3C;
    if (a != 0) {
        mff_cycles(0xCE87, 1);
    }
L_CE89:;
    mff_cycles(0xCE89, 25);
    RAM(GRAB_TIMER) = a;
    y = RAM(GRAB_SLOT);
    RAM(OBJ_HELD + y) = a;
    a = 0x00;
    RAM(OBJ_ATTACK + y) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE213 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCE9A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0E9C, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCE9D, 4);
    a = prg_read((uint16_t)(0xE25B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEA0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EA2, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCEA3, 6);
    SYNC_OUT;
    mff_rts(); return;
L_CEA4:;
    mff_cycles(0xCEA4, 7);
    a = RAM(JOY_HELD);
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xCEA8, 1); goto L_CEC1; }
    mff_cycles(0xCEAA, 12);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(WEAPON_HELD);
    if (a != 0) { mff_cycles(0xCEB1, 1); goto L_CEC2; }
    mff_cycles(0xCEB3, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1E3 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEB8, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EBA, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCEBB, 4);
    a = prg_read((uint16_t)(0xE22B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEBE, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EC0, func_F440, 0xE000)) return;
    SYNC_IN;
L_CEC1:;
    mff_cycles(0xCEC1, 6);
    SYNC_OUT;
    mff_rts(); return;
L_CEC2:;
    mff_cycles(0xCEC2, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE21F + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEC7, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EC9, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCECA, 4);
    a = prg_read((uint16_t)(0xE267 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCECD, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0ECF, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCED0, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $CD57 - lifted by port/tools/lift.py */
void func_CD57(void) {
    REGS_IN;
L_CD57:;
    for (;;) {
        for (;;) {
            for (;;) {
                for (;;) {
                    for (;;) {
                        for (;;) {
                            for (;;) {
                                for (;;) {
                                    for (;;) {
                                        for (;;) {
                                            for (;;) {
                                                for (;;) {
                                                    for (;;) {
                                                        for (;;) {
                                                            for (;;) {
                                                                for (;;) {
                                                                    for (;;) {
                                                                        for (;;) {
                                                                            for (;;) {
                                                                                for (;;) {
                                                                                    for (;;) {
                                                                                        mff_cycles(0xCD57, 6);
                                                                                        y--;
                                                                                        m = 0x02;
                                                                                        c = y >= m;
                                                                                        n = ((uint8_t)(y - m)) >> 7;
                                                                                        z = ((uint8_t)(y - m)) == 0;
                                                                                        if (z) {
                                                                                            mff_cycles(0xCD5C, 3);
                                                                                            SYNC_OUT;
                                                                                            nes_cpu_instruction_boundary(0xCEA4, 2); call_by_address_tail(0xCEA4, -1); return;
                                                                                        } else {
                                                                                            mff_cycles(0xCD5A, 1);
                                                                                        }
                                                                                        mff_cycles(0xCD5F, 6);
                                                                                        a = RAM(OBJ_STATE + y);
                                                                                        if (a & 0x80) break;
                                                                                        mff_cycles(0xCD57, 1);
                                                                                    }
                                                                                    mff_cycles(0xCD64, 6);
                                                                                    a = RAM(OBJ_FLAGS + y);
                                                                                    if (a & 0x80) break;
                                                                                    mff_cycles(0xCD57, 1);
                                                                                }
                                                                                mff_cycles(0xCD69, 8);
                                                                                a = RAM(OBJ_ID + y);
                                                                                if (a >= 0x1E) break;
                                                                                mff_cycles(0xCD57, 1);
                                                                            }
                                                                            mff_cycles(0xCD70, 4);
                                                                            if (a != 0x6F) break;
                                                                            mff_cycles(0xCD57, 1);
                                                                        }
                                                                        mff_cycles(0xCD74, 4);
                                                                        if (a != 0x8E) break;
                                                                        mff_cycles(0xCD57, 1);
                                                                    }
                                                                    mff_cycles(0xCD78, 4);
                                                                    if (a != 0x9D) break;
                                                                    mff_cycles(0xCD57, 1);
                                                                }
                                                                mff_cycles(0xCD7C, 4);
                                                                if (a != 0x2E) break;
                                                                mff_cycles(0xCD57, 1);
                                                            }
                                                            mff_cycles(0xCD80, 4);
                                                            if (a != 0x61) break;
                                                            mff_cycles(0xCD57, 1);
                                                        }
                                                        mff_cycles(0xCD84, 4);
                                                        if (a != 0x69) break;
                                                        mff_cycles(0xCD57, 1);
                                                    }
                                                    mff_cycles(0xCD88, 4);
                                                    if (a != 0x6A) break;
                                                    mff_cycles(0xCD57, 1);
                                                }
                                                mff_cycles(0xCD8C, 4);
                                                if (a != 0x6C) break;
                                                mff_cycles(0xCD57, 1);
                                            }
                                            mff_cycles(0xCD90, 4);
                                            if (a != 0x6D) break;
                                            mff_cycles(0xCD57, 1);
                                        }
                                        mff_cycles(0xCD94, 4);
                                        if (a != 0x66) break;
                                        mff_cycles(0xCD57, 1);
                                    }
                                    mff_cycles(0xCD98, 4);
                                    if (a != 0x91) break;
                                    mff_cycles(0xCD57, 1);
                                }
                                mff_cycles(0xCD9C, 4);
                                if (a != 0x72) break;
                                mff_cycles(0xCD57, 1);
                            }
                            mff_cycles(0xCDA0, 6);
                            a = RAM(OBJ_INVULN + y);
                            if (a == 0) break;
                            mff_cycles(0xCD57, 1);
                        }
                        mff_cycles(0xCDA5, 8);
                        a = RAM(OBJ_ANIM + y);
                        if (a != 0xA6) break;
                        mff_cycles(0xCD57, 1);
                    }
                    mff_cycles(0xCDAC, 4);
                    if (a != 0xAC) break;
                    mff_cycles(0xCD57, 1);
                }
                mff_cycles(0xCDB0, 4);
                if (a != 0x84) break;
                mff_cycles(0xCD57, 1);
            }
            mff_cycles(0xCDB4, 4);
            if (a != 0xE7) break;
            mff_cycles(0xCD57, 1);
        }
        mff_cycles(0xCDB8, 4);
        if (a != 0xBE) break;
        mff_cycles(0xCD57, 1);
    }
    mff_cycles(0xCDBC, 12);
    a = RAM(OBJ_Y);
    c = 1;
    m = RAM(OBJ_Y + y);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xCDC5, 6);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xCDC3, 1);
    }
    mff_cycles(0xCDCA, 4);
    if (a >= 0x08) { mff_cycles(0xCDCC, 2); goto L_CE11; }
    mff_cycles(0xCDCE, 27);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(OBJ_X + y);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = RAM(OBJ_X_HI + y);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = mff_pull();
    if (!c) {
        mff_cycles(0xCDDF, 6);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xCDDD, 1);
    }
    mff_cycles(0xCDE4, 10);
    RAM(TMP0) = a;
    a = RAM(PLAYER_ID);
    if (a != 0x02) { mff_cycles(0xCDEA, 1); goto L_CDF5; }
    mff_cycles(0xCDEC, 7);
    a = RAM(TMP0);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xCDF0, 2); goto L_CE11; }
    mff_cycles(0xCDF2, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCDFB, 2); call_by_address_tail(0xCDFB, -1); return;
L_CDF5:;
    mff_cycles(0xCDF5, 7);
    a = RAM(TMP0);
    if (a >= 0x07) { mff_cycles(0xCDF9, 2); goto L_CE11; }
    mff_cycles(0xCDFB, 12);
    a = RAM(OBJ_Y);
    c = 1;
    m = RAM(OBJ_Y + y);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xCE04, 6);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xCE02, 1);
    }
    mff_cycles(0xCE09, 4);
    if (a == 0x00) { mff_cycles(0xCE0B, 1); goto L_CE14; }
    mff_cycles(0xCE0D, 4);
    if (a < 0x08) { mff_cycles(0xCE0F, 1); goto L_CE14; }
L_CE11:;
    mff_cycles(0xCE11, 3);
    goto L_CD57;
L_CE14:;
    mff_cycles(0xCE14, 5);
    a = RAM(CAMERA_LOCK);
    if (a == 0) { mff_cycles(0xCE16, 1); goto L_CE38; }
    mff_cycles(0xCE18, 7);
    a = RAM(PLAYER_ID);
    if (a != 0x02) { mff_cycles(0xCE1C, 1); goto L_CE42; }
    mff_cycles(0xCE1E, 7);
    a = RAM(PLAYER_DIR);
    if (a != 0x02) { mff_cycles(0xCE22, 1); goto L_CE2E; }
    mff_cycles(0xCE24, 8);
    a = RAM(OBJ_X);
    if (a >= 0x28) { mff_cycles(0xCE29, 1); goto L_CE42; }
    mff_cycles(0xCE2B, 3);
    goto L_CE11;
L_CE2E:;
    mff_cycles(0xCE2E, 8);
    a = RAM(OBJ_X);
    if (a < 0xE0) { mff_cycles(0xCE33, 1); goto L_CE42; }
    mff_cycles(0xCE35, 3);
    goto L_CE11;
L_CE38:;
    mff_cycles(0xCE38, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    if (a < 0x24) { mff_cycles(0xCE11, 1); goto L_CE11; }
L_CE42:;
    mff_cycles(0xCE42, 29);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + y) = a;
    RAM(GRAB_SLOT) = y;
    a = RAM(PLAYER_DIR);
    a &= 0x02;
    y = a;
    a = RAM(OBJ_X);
    c = 0;
    m = prg_read((uint16_t)(0xE1D3 + y));
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xCE56, 11);
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = prg_read((uint16_t)(0xE1D4 + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xCE5D, 24);
    y = RAM(GRAB_SLOT);
    RAM(OBJ_X_HI + y) = a;
    a = mff_pull();
    RAM(OBJ_X + y) = a;
    a = RAM(JOY_HELD);
    a &= 0x03;
    if (a == 0) { mff_cycles(0xCE6A, 1); goto L_CEA4; }
    mff_cycles(0xCE6C, 12);
    a = 0x04;
    RAM(GAME_ACTION) = a;
    y = RAM(PLAYER_ID);
    m = 0x02;
    c = y >= m;
    if (y != m) { mff_cycles(0xCE74, 1); goto L_CE85; }
    mff_cycles(0xCE76, 20);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_Y + y);
    c = 1;
    m = 0x06;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_Y + y) = a;
    a = 0x5A;
    if (a != 0) { mff_cycles(0xCE83, 1); goto L_CE89; }
L_CE85:;
    mff_cycles(0xCE85, 4);
    a = 0x3C;
    if (a != 0) {
        mff_cycles(0xCE87, 1);
    }
L_CE89:;
    mff_cycles(0xCE89, 25);
    RAM(GRAB_TIMER) = a;
    y = RAM(GRAB_SLOT);
    RAM(OBJ_HELD + y) = a;
    a = 0x00;
    RAM(OBJ_ATTACK + y) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE213 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCE9A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0E9C, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCE9D, 4);
    a = prg_read((uint16_t)(0xE25B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEA0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EA2, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCEA3, 6);
    SYNC_OUT;
    mff_rts(); return;
L_CEA4:;
    mff_cycles(0xCEA4, 7);
    a = RAM(JOY_HELD);
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xCEA8, 1); goto L_CEC1; }
    mff_cycles(0xCEAA, 12);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(WEAPON_HELD);
    if (a != 0) { mff_cycles(0xCEB1, 1); goto L_CEC2; }
    mff_cycles(0xCEB3, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1E3 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEB8, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EBA, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCEBB, 4);
    a = prg_read((uint16_t)(0xE22B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEBE, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EC0, func_F440, 0xE000)) return;
    SYNC_IN;
L_CEC1:;
    mff_cycles(0xCEC1, 6);
    SYNC_OUT;
    mff_rts(); return;
L_CEC2:;
    mff_cycles(0xCEC2, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE21F + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEC7, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EC9, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCECA, 4);
    a = prg_read((uint16_t)(0xE267 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCECD, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0ECF, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCED0, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $CDFB - lifted by port/tools/lift.py */
void func_CDFB(void) {
    REGS_IN;
    mff_cycles(0xCDFB, 12);
    a = RAM(OBJ_Y);
    c = 1;
    m = RAM(OBJ_Y + y);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    if (!c) {
        mff_cycles(0xCE04, 6);
        a ^= 0xFF;
        m = 0x01;
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
    } else {
        mff_cycles(0xCE02, 1);
    }
    mff_cycles(0xCE09, 4);
    if (a == 0x00) { mff_cycles(0xCE0B, 1); goto L_CE14; }
    mff_cycles(0xCE0D, 4);
    m = 0x08;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xCE0F, 1); goto L_CE14; }
L_CE11:;
    mff_cycles(0xCE11, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCD57, 2); call_by_address_tail(0xCD57, -1); return;
L_CE14:;
    mff_cycles(0xCE14, 5);
    a = RAM(CAMERA_LOCK);
    if (a == 0) { mff_cycles(0xCE16, 1); goto L_CE38; }
    mff_cycles(0xCE18, 7);
    a = RAM(PLAYER_ID);
    if (a != 0x02) { mff_cycles(0xCE1C, 1); goto L_CE42; }
    mff_cycles(0xCE1E, 7);
    a = RAM(PLAYER_DIR);
    if (a != 0x02) { mff_cycles(0xCE22, 1); goto L_CE2E; }
    mff_cycles(0xCE24, 8);
    a = RAM(OBJ_X);
    m = 0x28;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xCE29, 1); goto L_CE42; }
    mff_cycles(0xCE2B, 3);
    goto L_CE11;
L_CE2E:;
    mff_cycles(0xCE2E, 8);
    a = RAM(OBJ_X);
    m = 0xE0;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xCE33, 1); goto L_CE42; }
    mff_cycles(0xCE35, 3);
    goto L_CE11;
L_CE38:;
    mff_cycles(0xCE38, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    m = 0x24;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xCE11, 1); goto L_CE11; }
L_CE42:;
    mff_cycles(0xCE42, 29);
    a = RAM(OBJ_Y);
    RAM(OBJ_Y + y) = a;
    RAM(GRAB_SLOT) = y;
    a = RAM(PLAYER_DIR);
    a &= 0x02;
    y = a;
    a = RAM(OBJ_X);
    c = 0;
    m = prg_read((uint16_t)(0xE1D3 + y));
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xCE56, 11);
    mff_push(a);
    a = RAM(OBJ_X_HI);
    m = prg_read((uint16_t)(0xE1D4 + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0xCE5D, 24);
    y = RAM(GRAB_SLOT);
    RAM(OBJ_X_HI + y) = a;
    a = mff_pull();
    RAM(OBJ_X + y) = a;
    a = RAM(JOY_HELD);
    a &= 0x03;
    if (a == 0) { mff_cycles(0xCE6A, 1); goto L_CEA4; }
    mff_cycles(0xCE6C, 12);
    a = 0x04;
    RAM(GAME_ACTION) = a;
    y = RAM(PLAYER_ID);
    m = 0x02;
    c = y >= m;
    if (y != m) { mff_cycles(0xCE74, 1); goto L_CE85; }
    mff_cycles(0xCE76, 20);
    y = RAM(GRAB_SLOT);
    a = RAM(OBJ_Y + y);
    c = 1;
    m = 0x06;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_Y + y) = a;
    a = 0x5A;
    if (a != 0) { mff_cycles(0xCE83, 1); goto L_CE89; }
L_CE85:;
    mff_cycles(0xCE85, 4);
    a = 0x3C;
    if (a != 0) {
        mff_cycles(0xCE87, 1);
    }
L_CE89:;
    mff_cycles(0xCE89, 25);
    RAM(GRAB_TIMER) = a;
    y = RAM(GRAB_SLOT);
    RAM(OBJ_HELD + y) = a;
    a = 0x00;
    RAM(OBJ_ATTACK + y) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE213 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCE9A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0E9C, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCE9D, 4);
    a = prg_read((uint16_t)(0xE25B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEA0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EA2, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCEA3, 6);
    SYNC_OUT;
    mff_rts(); return;
L_CEA4:;
    mff_cycles(0xCEA4, 7);
    a = RAM(JOY_HELD);
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xCEA8, 1); goto L_CEC1; }
    mff_cycles(0xCEAA, 12);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(WEAPON_HELD);
    if (a != 0) { mff_cycles(0xCEB1, 1); goto L_CEC2; }
    mff_cycles(0xCEB3, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1E3 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEB8, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EBA, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCEBB, 4);
    a = prg_read((uint16_t)(0xE22B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEBE, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EC0, func_F440, 0xE000)) return;
    SYNC_IN;
L_CEC1:;
    mff_cycles(0xCEC1, 6);
    SYNC_OUT;
    mff_rts(); return;
L_CEC2:;
    mff_cycles(0xCEC2, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE21F + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEC7, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EC9, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCECA, 4);
    a = prg_read((uint16_t)(0xE267 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCECD, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0ECF, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCED0, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $CE11 - lifted by port/tools/lift.py */
void func_CE11(void) {
    REGS_IN;
    mff_cycles(0xCE11, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xCD57, 2); call_by_address_tail(0xCD57, -1); return;
}

/*  $CEA4 - lifted by port/tools/lift.py */
void func_CEA4(void) {
    REGS_IN;
    mff_cycles(0xCEA4, 7);
    a = RAM(JOY_HELD);
    a &= 0x0F;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xCEA8, 1); goto L_CEC1; }
    mff_cycles(0xCEAA, 12);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(WEAPON_HELD);
    if (a != 0) { mff_cycles(0xCEB1, 1); goto L_CEC2; }
    mff_cycles(0xCEB3, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1E3 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEB8, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EBA, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCEBB, 4);
    a = prg_read((uint16_t)(0xE22B + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEBE, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EC0, func_F440, 0xE000)) return;
    SYNC_IN;
L_CEC1:;
    mff_cycles(0xCEC1, 6);
    SYNC_OUT;
    mff_rts(); return;
L_CEC2:;
    mff_cycles(0xCEC2, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE21F + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEC7, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EC9, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCECA, 4);
    a = prg_read((uint16_t)(0xE267 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCECD, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0ECF, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCED0, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $CEAA - lifted by port/tools/lift.py */
void func_CEAA(void) {
    REGS_IN;
    mff_cycles(0xCEAA, 12);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(WEAPON_HELD);
    if (a == 0) {
        mff_cycles(0xCEB3, 7);
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(0xE1E3 + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0xCEB8, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0EBA, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xCEBB, 4);
        a = prg_read((uint16_t)(0xE22B + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0xCEBE, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0EC0, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xCEC1, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xCEB1, 1);
    }
    mff_cycles(0xCEC2, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE21F + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCEC7, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0EC9, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCECA, 4);
    a = prg_read((uint16_t)(0xE267 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCECD, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0ECF, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCED0, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $CED1 - lifted by port/tools/lift.py */
void func_CED1(void) {
    REGS_IN;
    mff_cycles(0xCED1, 5);
    a = RAM(RESPAWN_TIMER1);
    if (a == 0) { mff_cycles(0xCED3, 1); goto L_CEED; }
    mff_cycles(0xCED5, 9);
    t = (uint8_t)(RAM(RESPAWN_TIMER1) - 1);
    RAM(RESPAWN_TIMER1) = t;
    a &= 0x02;
    if (a != 0) { mff_cycles(0xCED9, 1); goto L_CEE2; }
L_CEDB:;
    mff_cycles(0xCEDB, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xFCC8 + y));
    mff_cycles(0xCEE0, 2);
    if (a != 0) { mff_cycles(0xCEE0, 1); goto L_CEE5; }
L_CEE2:;
    mff_cycles(0xCEE2, 4);
    a = prg_read((uint16_t)(0xFCCB + y));
L_CEE5:;
    mff_cycles(0xCEE5, 15);
    RAM(PALETTE_BUF + 23) = a;
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PALETTE_REQUEST) = a;
    SYNC_OUT;
    mff_rts(); return;
L_CEED:;
    mff_cycles(0xCEED, 5);
    a = RAM(RESPAWN_TIMER2);
    if (a == 0) { mff_cycles(0xCEEF, 1); goto L_CEFA; }
    mff_cycles(0xCEF1, 9);
    t = (uint8_t)(RAM(RESPAWN_TIMER2) - 1);
    RAM(RESPAWN_TIMER2) = t;
    a &= 0x02;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xCEE2, 1); goto L_CEE2; }
    mff_cycles(0xCEF7, 2);
    if (z) { mff_cycles(0xCEDB, 1); goto L_CEDB; }
    mff_cycles(0xCEF9, 6);
    SYNC_OUT;
    mff_rts(); return;
L_CEFA:;
    mff_cycles(0xCEFA, 11);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    m = prg_read((uint16_t)(0xE297 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xCF02, 2);
    if (!z) {
        mff_cycles(0xCF04, 4);
        m = 0x49;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xCF08, 4);
            m = 0x36;
            c = a >= m;
            n = ((uint8_t)(a - m)) >> 7;
            z = ((uint8_t)(a - m)) == 0;
            if (!z) {
                mff_cycles(0xCF0C, 4);
                m = 0x4C;
                c = a >= m;
                n = ((uint8_t)(a - m)) >> 7;
                z = ((uint8_t)(a - m)) == 0;
                if (!z) {
                    mff_cycles(0xCF10, 4);
                    m = 0x5C;
                    c = a >= m;
                    n = ((uint8_t)(a - m)) >> 7;
                    z = ((uint8_t)(a - m)) == 0;
                    if (!z) {
                        mff_cycles(0xCF14, 4);
                        m = 0x15;
                        c = a >= m;
                        n = ((uint8_t)(a - m)) >> 7;
                        z = ((uint8_t)(a - m)) == 0;
                        if (!z) {
                            mff_cycles(0xCF18, 4);
                            m = 0x14;
                            c = a >= m;
                            n = ((uint8_t)(a - m)) >> 7;
                            z = ((uint8_t)(a - m)) == 0;
                            if (!z) {
                                mff_cycles(0xCF1C, 4);
                                m = 0x5B;
                                c = a >= m;
                                n = ((uint8_t)(a - m)) >> 7;
                                z = ((uint8_t)(a - m)) == 0;
                                if (!z) {
                                    mff_cycles(0xCF20, 7);
                                    a = RAM(GAME_ACTION);
                                    m = 0x12;
                                    c = a >= m;
                                    n = ((uint8_t)(a - m)) >> 7;
                                    z = ((uint8_t)(a - m)) == 0;
                                    if (!z) {
                                        mff_cycles(0xCF26, 4);
                                        m = 0x18;
                                        c = a >= m;
                                        n = ((uint8_t)(a - m)) >> 7;
                                        z = ((uint8_t)(a - m)) == 0;
                                        if (!z) {
                                            mff_cycles(0xCF2A, 4);
                                            m = 0x14;
                                            c = a >= m;
                                            n = ((uint8_t)(a - m)) >> 7;
                                            z = ((uint8_t)(a - m)) == 0;
                                            if (!z) {
                                                mff_cycles(0xCF2E, 5);
                                                a = 0x00;
                                                n = (a) >> 7;
                                                z = (a) == 0;
                                                RAM(PLAYER_NO_HIT) = a;
                                            } else {
                                                mff_cycles(0xCF2C, 1);
                                            }
                                        } else {
                                            mff_cycles(0xCF28, 1);
                                        }
                                    } else {
                                        mff_cycles(0xCF24, 1);
                                    }
                                } else {
                                    mff_cycles(0xCF1E, 1);
                                }
                            } else {
                                mff_cycles(0xCF1A, 1);
                            }
                        } else {
                            mff_cycles(0xCF16, 1);
                        }
                    } else {
                        mff_cycles(0xCF12, 1);
                    }
                } else {
                    mff_cycles(0xCF0E, 1);
                }
            } else {
                mff_cycles(0xCF0A, 1);
            }
        } else {
            mff_cycles(0xCF06, 1);
        }
    } else {
        mff_cycles(0xCF02, 1);
    }
    mff_cycles(0xCF32, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/* player_state_jump $CF88 - lifted by port/tools/lift.py */
void func_CF88(void) {
    REGS_IN;
    mff_cycles(0xCF88, 10);
    y = 0x00;
    x = 0x00;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0F8E, func_F119, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xCF8F, 2);
    if (c) {
        mff_cycles(0xCF91, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xD00E, 2); call_by_address_tail(0xD00E, -1); return;
    } else {
        mff_cycles(0xCF8F, 1);
    }
    mff_cycles(0xCF94, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE279 + y));
    mff_cycles(0xCF99, 6);
    if (a == RAM(OBJ_ANIM)) { mff_cycles(0xCF9C, 1); goto L_CFEE; }
    mff_cycles(0xCF9E, 4);
    a = prg_read((uint16_t)(0xE27C + y));
    mff_cycles(0xCFA1, 6);
    if (a == RAM(OBJ_ANIM)) { mff_cycles(0xCFA4, 1); goto L_CFEE; }
    mff_cycles(0xCFA6, 6);
    a = RAM(OBJ_VY);
    if (!(a & 0x80)) { mff_cycles(0xCFA9, 1); goto L_CFEE; }
    mff_cycles(0xCFAB, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE27F + y));
    mff_cycles(0xCFB0, 6);
    if (a == RAM(OBJ_ANIM)) { mff_cycles(0xCFB3, 1); goto L_CFC7; }
    mff_cycles(0xCFB5, 4);
    a = prg_read((uint16_t)(0xE285 + y));
    mff_cycles(0xCFB8, 6);
    if (a == RAM(OBJ_ANIM)) { mff_cycles(0xCFBB, 1); goto L_CFC7; }
    mff_cycles(0xCFBD, 4);
    a = prg_read((uint16_t)(0xE282 + y));
    mff_cycles(0xCFC0, 6);
    m = RAM(OBJ_ANIM);
    c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xCFC3, 1); goto L_CFE6; }
    mff_cycles(0xCFC5, 2);
    if (!z) { mff_cycles(0xCFC5, 1); goto L_CFEE; }
L_CFC7:;
    mff_cycles(0xCFC7, 7);
    a = RAM(TMP16);
    m = 0x40;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xCFCD, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0FCF, 0xCFD1, -1)) return;
        SYNC_IN;
        mff_cycles(0xCFD0, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xCFCB, 1);
    }
    mff_cycles(0xCFDF, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x03;
    c = a >= m;
    if (a != m) { mff_cycles(0xCFE4, 1); goto L_CFEE; }
L_CFE6:;
    mff_cycles(0xCFE6, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE219 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xCFEB, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0FED, func_F294, 0xE000)) return;
    SYNC_IN;
L_CFEE:;
    mff_cycles(0xCFEE, 7);
    a = RAM(TMP16);
    m = 0x40;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xCFF4, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0FF6, 0xCFD1, -1)) return;
        SYNC_IN;
        mff_cycles(0xCFF7, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0xCFF2, 1);
    }
    mff_cycles(0xCFF8, 13);
    a = RAM(OBJ_Y);
    c = 0;
    m = 0x02;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    m = RAM(PLAYER_GROUND_Y);
    c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xD000, 1); goto L_D04A; }
    mff_cycles(0xD002, 2);
    if (z) { mff_cycles(0xD002, 1); goto L_D00B; }
    mff_cycles(0xD004, 8);
    m = 0x03;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(OBJ_Y) = a;
    if (a != 0) { mff_cycles(0xD009, 1); goto L_D00E; }
L_D00B:;
    mff_cycles(0xD00B, 4);
    RAM(OBJ_Y) = a;
L_D00E:;
    mff_cycles(0xD00E, 7);
    a = RAM(TMP16);
    m = 0x40;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) { mff_cycles(0xD012, 1); goto L_D016; }
    mff_cycles(0xD014, 2);
    if (!c) { mff_cycles(0xD014, 1); goto L_D01A; }
L_D016:;
    mff_cycles(0xD016, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1018, 0xCFD1, -1)) return;
    SYNC_IN;
    mff_cycles(0xD019, 6);
    SYNC_OUT;
    mff_rts(); return;
L_D01A:;
    mff_cycles(0xD01A, 16);
    a = 0x02;
    RAM(GAME_ACTION) = a;
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ACTION_TIMER) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x1024, func_F436, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD025, 7);
    a = RAM(PLAYER_ID);
    m = 0x01;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xD02B, 5);
        a = RAM(GUY_JUMP_WINDOW);
        n = (a) >> 7;
        z = (a) == 0;
        if (!z) {
            mff_cycles(0xD02F, 8);
            a = 0x68;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x1033, func_F294, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xD034, 8);
            a = 0x06;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x1038, func_F440, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xD039, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xD03F, 2); call_by_address_tail(0xD03F, -1); return;
        } else {
            mff_cycles(0xD02D, 1);
        }
    } else {
        mff_cycles(0xD029, 1);
    }
    mff_cycles(0xD03C, 6);
    SYNC_OUT;
    if (!mff_call_at(0x103E, 0xCEAA, -1)) return;
    SYNC_IN;
    mff_cycles(0xD03F, 8);
    a = 0x1C;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x1043, func_F34C, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD044, 12);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_TIMER2) = a;
    SYNC_OUT;
    mff_rts(); return;
L_D04A:;
    mff_cycles(0xD04A, 7);
    a = RAM(PLAYER_ID);
    if (a == 0x01) {
        mff_cycles(0xD050, 8);
        a = RAM(OBJ_ANIM);
        if (a == 0x3B) {
            mff_cycles(0xD057, 7);
            a = RAM(JOY_PRESSED);
            if (a == 0x08) {
                mff_cycles(0xD05D, 5);
                a = 0x05;
                RAM(GUY_JUMP_WINDOW) = a;
            } else {
                mff_cycles(0xD05B, 1);
            }
        } else {
            mff_cycles(0xD055, 1);
        }
    } else {
        mff_cycles(0xD04E, 1);
    }
    mff_cycles(0xD061, 7);
    a = RAM(JOY_PRESSED);
    m = 0x40;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xD067, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xD18E, 2); call_by_address_tail(0xD18E, -1); return;
    } else {
        mff_cycles(0xD065, 1);
    }
    mff_cycles(0xD06A, 6);
    a = RAM(OBJ_VY);
    if (a & 0x80) {
        mff_cycles(0xD06F, 4);
        m = 0xFE;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!c) {
            mff_cycles(0xD073, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xD18E, 2); call_by_address_tail(0xD18E, -1); return;
        } else {
            mff_cycles(0xD071, 1);
        }
    } else {
        mff_cycles(0xD06D, 1);
    }
    mff_cycles(0xD076, 5);
    a = RAM(JUMP_MOVING);
    if (a != 0) { mff_cycles(0xD078, 1); goto L_D0D8; }
    mff_cycles(0xD07A, 12);
    a = RAM(JOY_HELD);
    a &= 0x0F;
    RAM(TMP0) = a;
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0xD082, 1); goto L_D0BC; }
    mff_cycles(0xD084, 4);
    m = 0x02;
    c = a >= m;
    if (a == m) { mff_cycles(0xD086, 1); goto L_D0BC; }
    mff_cycles(0xD088, 4);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0xD08A, 1); goto L_D098; }
    mff_cycles(0xD08C, 4);
    m = 0x05;
    c = a >= m;
    if (a == m) { mff_cycles(0xD08E, 1); goto L_D098; }
    mff_cycles(0xD090, 7);
    a = RAM(TMP0);
    m = 0x06;
    c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xD094, 1); goto L_D098; }
    mff_cycles(0xD096, 2);
    if (!z) { mff_cycles(0xD096, 1); goto L_D0BC; }
L_D098:;
    mff_cycles(0xD098, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1FB + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xD09D, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x109F, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD0A0, 4);
    a = prg_read((uint16_t)(0xE246 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xD0A3, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x10A5, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD0A6, 4);
    m = 0x02;
    c = y >= m;
    if (y != m) { mff_cycles(0xD0A8, 1); goto L_D0AE; }
    mff_cycles(0xD0AA, 4);
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xD0AC, 1); goto L_D0B0; }
L_D0AE:;
    mff_cycles(0xD0AE, 2);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
L_D0B0:;
    mff_cycles(0xD0B0, 9);
    RAM(ATTACK_ID) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x10B4, func_F43B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD0B5, 9);
    a = RAM(PLAYER_GROUND_Y);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ATTACK_GROUND_Y) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xD18E, 2); call_by_address_tail(0xD18E, -1); return;
L_D0BC:;
    mff_cycles(0xD0BC, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1FE + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xD0C1, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x10C3, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD0C4, 4);
    a = prg_read((uint16_t)(0xE246 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xD0C7, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x10C9, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD0CA, 11);
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ATTACK_ID) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x10D0, func_F43B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD0D1, 9);
    a = RAM(PLAYER_GROUND_Y);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ATTACK_GROUND_Y) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xD18E, 2); call_by_address_tail(0xD18E, -1); return;
L_D0D8:;
    mff_cycles(0xD0D8, 7);
    a = RAM(JOY_PRESSED);
    m = 0x40;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xD0DE, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xD18E, 2); call_by_address_tail(0xD18E, -1); return;
    } else {
        mff_cycles(0xD0DC, 1);
    }
    mff_cycles(0xD0E1, 11);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    m = prg_read((uint16_t)(0xE279 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xD0E9, 2);
    if (z) {
        mff_cycles(0xD0EB, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xD18E, 2); call_by_address_tail(0xD18E, -1); return;
    } else {
        mff_cycles(0xD0E9, 1);
    }
    mff_cycles(0xD0EE, 8);
    a = RAM(OBJ_ANIM);
    m = prg_read((uint16_t)(0xE27C + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xD0F4, 2);
    if (z) {
        mff_cycles(0xD0F6, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xD18E, 2); call_by_address_tail(0xD18E, -1); return;
    } else {
        mff_cycles(0xD0F4, 1);
    }
    mff_cycles(0xD0F9, 8);
    a = RAM(PLAYER_DIR);
    if (a != RAM(JUMP_DIR)) { mff_cycles(0xD0FD, 2); goto L_D159; }
    mff_cycles(0xD0FF, 12);
    a = RAM(JOY_HELD);
    a &= 0x0F;
    RAM(TMP0) = a;
    m = 0x01;
    c = a >= m;
    if (a == m) { mff_cycles(0xD107, 1); goto L_D13F; }
    mff_cycles(0xD109, 4);
    m = 0x02;
    c = a >= m;
    if (a == m) { mff_cycles(0xD10B, 1); goto L_D13F; }
    mff_cycles(0xD10D, 4);
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0xD10F, 1); goto L_D11D; }
    mff_cycles(0xD111, 4);
    m = 0x05;
    c = a >= m;
    if (a == m) { mff_cycles(0xD113, 1); goto L_D11D; }
    mff_cycles(0xD115, 7);
    a = RAM(TMP0);
    m = 0x06;
    c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xD119, 1); goto L_D11D; }
    mff_cycles(0xD11B, 2);
    if (!z) { mff_cycles(0xD11B, 1); goto L_D13F; }
L_D11D:;
    mff_cycles(0xD11D, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1FB + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xD122, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1124, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD125, 4);
    a = prg_read((uint16_t)(0xE246 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xD128, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x112A, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD12B, 4);
    m = 0x02;
    c = y >= m;
    if (y != m) { mff_cycles(0xD12D, 1); goto L_D133; }
    mff_cycles(0xD12F, 4);
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xD131, 1); goto L_D135; }
L_D133:;
    mff_cycles(0xD133, 2);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
L_D135:;
    mff_cycles(0xD135, 9);
    RAM(ATTACK_ID) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x1139, func_F43B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD13A, 12);
    a = RAM(PLAYER_GROUND_Y);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ATTACK_GROUND_Y) = a;
    SYNC_OUT;
    mff_rts(); return;
L_D13F:;
    mff_cycles(0xD13F, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE201 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xD144, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1146, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD147, 4);
    a = prg_read((uint16_t)(0xE249 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xD14A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x114C, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD14D, 11);
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ATTACK_ID) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x1153, func_F43B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD154, 6);
    a = RAM(PLAYER_GROUND_Y);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(ATTACK_GROUND_Y) = a;
L_D158:;
    mff_cycles(0xD158, 6);
    SYNC_OUT;
    mff_rts(); return;
L_D159:;
    mff_cycles(0xD159, 12);
    a = RAM(JOY_HELD);
    a &= 0x0F;
    RAM(TMP0) = a;
    m = 0x04;
    c = a >= m;
    if (a == m) { mff_cycles(0xD161, 1); goto L_D16D; }
    mff_cycles(0xD163, 4);
    m = 0x05;
    c = a >= m;
    if (a == m) { mff_cycles(0xD165, 1); goto L_D16D; }
    mff_cycles(0xD167, 4);
    m = 0x06;
    c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xD169, 1); goto L_D16D; }
    mff_cycles(0xD16B, 2);
    if (!z) { mff_cycles(0xD16B, 1); goto L_D18E; }
L_D16D:;
    mff_cycles(0xD16D, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE1FB + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xD172, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1174, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD175, 4);
    a = prg_read((uint16_t)(0xE243 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xD178, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x117A, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD17B, 4);
    m = 0x02;
    c = y >= m;
    if (y != m) { mff_cycles(0xD17D, 1); goto L_D183; }
    mff_cycles(0xD17F, 4);
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xD181, 1); goto L_D185; }
L_D183:;
    mff_cycles(0xD183, 2);
    a = 0x04;
    n = (a) >> 7;
    z = (a) == 0;
L_D185:;
    mff_cycles(0xD185, 9);
    RAM(ATTACK_ID) = a;
    SYNC_OUT;
    if (!mff_call_fn(0x1189, func_F43B, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD18A, 6);
    a = RAM(PLAYER_GROUND_Y);
    RAM(ATTACK_GROUND_Y) = a;
L_D18E:;
    mff_cycles(0xD18E, 5);
    a = RAM(JUMP_MOVING);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0xD190, 1); goto L_D1F3; }
    mff_cycles(0xD192, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) { mff_cycles(0xD158, 1); goto L_D158; }
    mff_cycles(0xD197, 11);
    y = RAM(PLAYER_ID);
    a = RAM(OBJ_ANIM);
    m = prg_read((uint16_t)(0xE279 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xD19F, 2);
    if (z) { mff_cycles(0xD158, 1); goto L_D158; }
    mff_cycles(0xD1A1, 7);
    a = RAM(JOY_HELD);
    a &= 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xD158, 1); goto L_D158; }
    mff_cycles(0xD1A7, 10);
    RAM(TMP0) = a;
    a = RAM(JOY_PRESSED);
    m = 0x40;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xD1AF, 11);
        y = RAM(PLAYER_ID);
        a = RAM(OBJ_ANIM);
        m = prg_read((uint16_t)(0xE27F + y));
        c = a >= m;
        mff_cycles(0xD1B7, 2);
        if (a != m) {
            mff_cycles(0xD1B9, 7);
            y = RAM(PLAYER_ID);
            a = prg_read((uint16_t)(0xE1EC + y));
            n = (a) >> 7;
            z = (a) == 0;
            mff_cycles(0xD1BE, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x11C0, func_F294, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xD1C1, 4);
            a = prg_read((uint16_t)(0xE22E + y));
            n = (a) >> 7;
            z = (a) == 0;
            mff_cycles(0xD1C4, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x11C6, func_F440, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xD1B7, 1);
        }
    } else {
        mff_cycles(0xD1AD, 1);
    }
    mff_cycles(0xD1C7, 15);
    t = (uint8_t)(RAM(JUMP_MOVING) + 1);
    RAM(JUMP_MOVING) = t;
    a = RAM(TMP0);
    RAM(JUMP_DIR) = a;
    a &= 0x01;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0xD1CF, 2); goto L_D21C; }
    mff_cycles(0xD1D1, 5);
    a = RAM(CAMERA_AUTOSCROLL);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0xD1D3, 1); goto L_D1F6; }
    mff_cycles(0xD1D5, 5);
    a = RAM(CAMERA_LOCK);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) { mff_cycles(0xD1D7, 1); goto L_D1F6; }
    mff_cycles(0xD1D9, 13);
    a = RAM(OBJ_X);
    c = 1;
    m = RAM(SCROLL_X);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    m = 0xF1;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) { mff_cycles(0xD1E1, 1); goto L_D1F6; }
    mff_cycles(0xD1E3, 26);
    a = RAM(SCROLL_X);
    c = 0;
    m = 0xF0;
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(OBJ_X) = a;
    a = RAM(AREA_SCREEN);
    m = 0x00;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_X_HI) = a;
    SYNC_OUT;
    mff_rts(); return;
L_D1F3:;
    mff_cycles(0xD1F3, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xD242, 2); call_by_address_tail(0xD242, -1); return;
L_D1F6:;
    mff_cycles(0xD1F6, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x11F8, func_F090, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD1F9, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x11FB, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD1FC, 8);
    a = RAM(PLAYER_DIR);
    m = RAM(JUMP_DIR);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xD200, 1); goto L_D241; }
    mff_cycles(0xD202, 7);
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xE21C + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xD207, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1209, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD20A, 4);
    a = prg_read((uint16_t)(0xE1CD + y));
    mff_cycles(0xD20D, 8);
    RAM(OBJ_VX_FRAC) = a;
    a = prg_read((uint16_t)(0xE1D0 + y));
    mff_cycles(0xD213, 16);
    RAM(OBJ_VX) = a;
    a = 0x90;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_FLAGS) = a;
    SYNC_OUT;
    mff_rts(); return;
L_D21C:;
    mff_cycles(0xD21C, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x121E, func_F099, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD21F, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x1221, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xD222, 8);
    a = RAM(PLAYER_DIR);
    m = RAM(JUMP_DIR);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xD228, 7);
        y = RAM(PLAYER_ID);
        a = prg_read((uint16_t)(0xE21C + y));
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0xD22D, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x122F, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xD230, 4);
        a = prg_read((uint16_t)(0xE1CD + y));
        mff_cycles(0xD233, 8);
        RAM(OBJ_VX_FRAC) = a;
        a = prg_read((uint16_t)(0xE1D0 + y));
        mff_cycles(0xD239, 10);
        RAM(OBJ_VX) = a;
        a = 0xD0;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(OBJ_FLAGS) = a;
    } else {
        mff_cycles(0xD226, 1);
    }
L_D241:;
    mff_cycles(0xD241, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $CFD1 - lifted by port/tools/lift.py */
void func_CFD1(void) {
    REGS_IN;
    mff_cycles(0xCFD1, 23);
    a = RAM(GAME_ACTION);
    RAM(ACTION_BEFORE_FALL) = a;
    a = 0x24;
    RAM(GAME_ACTION) = a;
    a = 0x80;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(OBJ_STATE) = a;
    SYNC_OUT;
    mff_rts(); return;
}
