/*
 * lifted_b4.c - bank 4 routines $8000-$BFFF, lifted by
 * port/tools/lift_bank.py from the pure scaffold: exact, still to be
 * tidied and moved into their systems' files.
 */
#include "mff.h"
#include "state.h"
#include "coroutine.h"

void func_F03F(void);
void func_F183(void);
void func_F1A1(void);
void func_F1FD(void);
void func_F26E(void);
void func_F288(void);
void func_F294(void);
void func_F2A0(void);
void func_F314(void);
void func_F32E(void);
void func_F34C(void);
void func_F406(void);
void func_F430(void);
void func_F440(void);
void func_F71A(void);
void func_FF2A(void);
void func_FF31(void);

/*  $A000 (bank 4) - lifted by port/tools/lift.py */
void func_A000_b4(void) {
    REGS_IN;
    mff_cycles(0xA000, 11);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    mff_push(a);
    x = a;
    a = prg_read((uint16_t)(0xA035 + x));
    mff_cycles(0xA006, 7);
    RAM(CHR_BANKS + 3) = a;
    a = prg_read((uint16_t)(0xA036 + x));
    mff_cycles(0xA00B, 7);
    RAM(CHR_BANKS + 4) = a;
    a = prg_read((uint16_t)(0xA0B5 + x));
    mff_cycles(0xA010, 27);
    RAM(CHR_BANKS + 5) = a;
    a = mff_pull();
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    y = a;
    a = 0x00;
    m = 0xA1;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(TMP1) = a;
    a = 0x05;
    RAM(TMP0) = a;
    x = 0x00;
    for (;;) {
        mff_cycles(0xA022, 5);
        a = mem_read((uint16_t)(RAM16(TMP0) + y));
        mff_cycles(0xA024, 18);
        RAM(PALETTE_BUF + 24 + x) = a;
        RAM(PALETTE_BASE + 24 + x) = a;
        y++;
        x++;
        m = 0x08;
        c = x >= m;
        if (x == m) break;
        mff_cycles(0xA022, 1);
    }
    mff_cycles(0xA030, 11);
    x = 0xFF;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(PALETTE_REQUEST) = x;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A013 (bank 4) - lifted by port/tools/lift.py */
void func_A013_b4(void) {
    REGS_IN;
    mff_cycles(0xA013, 20);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    y = a;
    a = 0x00;
    m = 0xA1;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(TMP1) = a;
    a = 0x05;
    RAM(TMP0) = a;
    x = 0x00;
    for (;;) {
        mff_cycles(0xA022, 5);
        a = mem_read((uint16_t)(RAM16(TMP0) + y));
        mff_cycles(0xA024, 18);
        RAM(PALETTE_BUF + 24 + x) = a;
        RAM(PALETTE_BASE + 24 + x) = a;
        y++;
        x++;
        m = 0x08;
        c = x >= m;
        if (x == m) break;
        mff_cycles(0xA022, 1);
    }
    mff_cycles(0xA030, 11);
    x = 0xFF;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(PALETTE_REQUEST) = x;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A300 (bank 4) - lifted by port/tools/lift.py */
void func_A300_b4(void) {
    REGS_IN;
    mff_cycles(0xA300, 18);
    a = 0x03;
    RAM(TMP16) = a;
    a = RAM(PALETTE_REQUEST);
    a &= 0x01;
    RAM(TMP17) = a;
    a = 0x00;
    RAM(PALETTE_REQUEST) = a;
    for (;;) {
        mff_cycles(0xA30E, 9);
        x = RAM(TMP16);
        a = RAM(BG_PAL_ANIM_ON + x);
        n = (a) >> 7;
        z = (a) == 0;
        if (n) {
            mff_cycles(0xA315, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0317, 0xA321, 4)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xA313, 1);
        }
        mff_cycles(0xA318, 7);
        t = (uint8_t)(RAM(TMP16) - 1);
        RAM(TMP16) = t;
        if (t & 0x80) break;
        mff_cycles(0xA30E, 1);
    }
    mff_cycles(0xA31C, 12);
    a = RAM(TMP17);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PALETTE_REQUEST) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A321 (bank 4) - lifted by port/tools/lift.py */
void func_A321_b4(void) {
    REGS_IN;
    mff_cycles(0xA321, 8);
    y = RAM(BG_PAL_ANIM_SCRIPT + x);
    a = prg_read((uint16_t)(0xA445 + y));
    mff_cycles(0xA327, 17);
    y = a;
    a = RAM(BG_PAL_ANIM_TIMER + x);
    t = (uint8_t)(RAM(BG_PAL_ANIM_TIMER + x) + 1);
    RAM(BG_PAL_ANIM_TIMER + x) = t;
    m = prg_read((uint16_t)(0xA459 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0xA331, 2);
    if (z) {
        mff_cycles(0xA333, 22);
        a = 0x00;
        RAM(BG_PAL_ANIM_TIMER + x) = a;
        a = RAM(BG_PAL_ANIM_STEP + x);
        t = (uint8_t)(RAM(BG_PAL_ANIM_STEP + x) + 1);
        RAM(BG_PAL_ANIM_STEP + x) = t;
        mff_cycles(0xA341, 2);
        if (a == prg_read((uint16_t)(0xA458 + y))) {
            mff_cycles(0xA343, 7);
            a = 0x00;
            RAM(BG_PAL_ANIM_STEP + x) = a;
        } else {
            mff_cycles(0xA341, 1);
        }
        mff_cycles(0xA348, 14);
        a = y;
        c = 0;
        m = RAM(BG_PAL_ANIM_STEP + x);
        w = a + m + c;
        a = (uint8_t)w;
        x = a;
        a = prg_read((uint16_t)(0xA45B + x));
        mff_cycles(0xA351, 6);
        t = a;
        c = t >> 7;
        t = (uint8_t)(t << 1);
        a = t;
        m = prg_read((uint16_t)(0xA45B + x));
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        mff_cycles(0xA355, 6);
        x = a;
        a = prg_read((uint16_t)(0xA45A + y));
        mff_cycles(0xA359, 7);
        y = a;
        a = 0x03;
        RAM(TMP0) = a;
        for (;;) {
            mff_cycles(0xA35E, 4);
            a = prg_read((uint16_t)(0xA370 + x));
            mff_cycles(0xA361, 21);
            RAM(PALETTE_BUF + y) = a;
            RAM(PALETTE_BASE + y) = a;
            y++;
            x++;
            t = (uint8_t)(RAM(TMP0) - 1);
            RAM(TMP0) = t;
            if (t == 0) break;
            mff_cycles(0xA35E, 1);
        }
        mff_cycles(0xA36D, 5);
        t = (uint8_t)(RAM(TMP17) + 1);
        RAM(TMP17) = t;
        n = (t) >> 7;
        z = (t) == 0;
    } else {
        mff_cycles(0xA331, 1);
    }
    mff_cycles(0xA36F, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A700 (bank 4) - lifted by port/tools/lift.py */
void func_A700_b4(void) {
    REGS_IN;
    mff_cycles(0xA700, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0702, 0xA78E, 4)) return;
    SYNC_IN;
    mff_cycles(0xA703, 7);
    y = RAM(INTRO_PAGE);
    a = prg_read((uint16_t)(0xA7D6 + y));
    mff_cycles(0xA708, 7);
    RAM(TMP12) = a;
    a = prg_read((uint16_t)(0xA7DB + y));
    mff_cycles(0xA70D, 16);
    RAM(TMP13) = a;
    a = 0x00;
    RAM(TMP14) = a;
    a = 0x00;
    RAM(TMP16) = a;
    RAM(TMP18) = a;
L_A719:;
    mff_cycles(0xA719, 12);
    a = 0x00;
    RAM(TMP17) = a;
    x = RAM(TMP16);
    a = prg_read((uint16_t)(0xA782 + x));
    mff_cycles(0xA722, 8);
    RAM(VRAM_BUFFER) = a;
    a = prg_read((uint16_t)(0xA785 + x));
    mff_cycles(0xA728, 28);
    c = 0;
    m = RAM(TMP17);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(VRAM_BUFFER + 1) = a;
    a = 0x00;
    RAM(VRAM_BUFFER + 2) = a;
    y = RAM(TMP14);
    t = (uint8_t)(RAM(TMP14) + 1);
    RAM(TMP14) = t;
    a = mem_read((uint16_t)(RAM16(TMP12) + y));
    mff_cycles(0xA739, 4);
    m = 0xFF;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA73B, 1); goto L_A77E; }
    mff_cycles(0xA73D, 4);
    m = 0xFE;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xA741, 31);
        RAM(VRAM_BUFFER + 3) = a;
        a = 0xFF;
        RAM(VRAM_BUFFER + 4) = a;
        RAM(VRAM_LIST_REQUEST) = a;
        a = RAM(TMP16);
        mff_push(a);
        a = RAM(TMP17);
        n = (a) >> 7;
        z = (a) == 0;
        mff_push(a);
        SYNC_OUT;
        if (!mff_call_at(0x0753, 0xA788, 4)) return;
        SYNC_IN;
        mff_cycles(0xA754, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0756, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA757, 26);
        a = mff_pull();
        RAM(TMP17) = a;
        a = mff_pull();
        RAM(TMP16) = a;
        t = (uint8_t)(RAM(TMP17) + 1);
        RAM(TMP17) = t;
        a = RAM(FRAME_COUNTER);
        a &= 0x03;
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0xA765, 8);
            a = 0x2B;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0769, func_F34C, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xA763, 1);
        }
        mff_cycles(0xA76A, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA71D, 2); call_by_address_tail(0xA71D, 4); return;
    } else {
        mff_cycles(0xA73F, 1);
    }
    mff_cycles(0xA76D, 12);
    t = (uint8_t)(RAM(TMP16) + 1);
    RAM(TMP16) = t;
    a = RAM(TMP16);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA719, 1); goto L_A719; }
    mff_cycles(0xA775, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0777, 0xA7D0, 4)) return;
    SYNC_IN;
    mff_cycles(0xA778, 6);
    SYNC_OUT;
    if (!mff_call_at(0x077A, 0xA78E, 4)) return;
    SYNC_IN;
    mff_cycles(0xA77B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA713, 2); call_by_address_tail(0xA713, 4); return;
L_A77E:;
    mff_cycles(0xA77E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA7D0, 2); call_by_address_tail(0xA7D0, 4); return;
}

/*  $A713 (bank 4) - lifted by port/tools/lift.py */
void func_A713_b4(void) {
    REGS_IN;
    mff_cycles(0xA713, 8);
    a = 0x00;
    RAM(TMP16) = a;
    RAM(TMP18) = a;
L_A719:;
    mff_cycles(0xA719, 12);
    a = 0x00;
    RAM(TMP17) = a;
    x = RAM(TMP16);
    a = prg_read((uint16_t)(0xA782 + x));
    mff_cycles(0xA722, 8);
    RAM(VRAM_BUFFER) = a;
    a = prg_read((uint16_t)(0xA785 + x));
    mff_cycles(0xA728, 28);
    c = 0;
    m = RAM(TMP17);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(VRAM_BUFFER + 1) = a;
    a = 0x00;
    RAM(VRAM_BUFFER + 2) = a;
    y = RAM(TMP14);
    t = (uint8_t)(RAM(TMP14) + 1);
    RAM(TMP14) = t;
    a = mem_read((uint16_t)(RAM16(TMP12) + y));
    mff_cycles(0xA739, 4);
    m = 0xFF;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA73B, 1); goto L_A77E; }
    mff_cycles(0xA73D, 4);
    m = 0xFE;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xA741, 31);
        RAM(VRAM_BUFFER + 3) = a;
        a = 0xFF;
        RAM(VRAM_BUFFER + 4) = a;
        RAM(VRAM_LIST_REQUEST) = a;
        a = RAM(TMP16);
        mff_push(a);
        a = RAM(TMP17);
        n = (a) >> 7;
        z = (a) == 0;
        mff_push(a);
        SYNC_OUT;
        if (!mff_call_at(0x0753, 0xA788, 4)) return;
        SYNC_IN;
        mff_cycles(0xA754, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0756, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA757, 26);
        a = mff_pull();
        RAM(TMP17) = a;
        a = mff_pull();
        RAM(TMP16) = a;
        t = (uint8_t)(RAM(TMP17) + 1);
        RAM(TMP17) = t;
        a = RAM(FRAME_COUNTER);
        a &= 0x03;
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0xA765, 8);
            a = 0x2B;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0769, func_F34C, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xA763, 1);
        }
        mff_cycles(0xA76A, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA71D, 2); call_by_address_tail(0xA71D, 4); return;
    } else {
        mff_cycles(0xA73F, 1);
    }
    mff_cycles(0xA76D, 12);
    t = (uint8_t)(RAM(TMP16) + 1);
    RAM(TMP16) = t;
    a = RAM(TMP16);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA719, 1); goto L_A719; }
    mff_cycles(0xA775, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0777, 0xA7D0, 4)) return;
    SYNC_IN;
    mff_cycles(0xA778, 6);
    SYNC_OUT;
    if (!mff_call_at(0x077A, 0xA78E, 4)) return;
    SYNC_IN;
    mff_cycles(0xA77B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA713, 2); call_by_address_tail(0xA713, 4); return;
L_A77E:;
    mff_cycles(0xA77E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA7D0, 2); call_by_address_tail(0xA7D0, 4); return;
}

/*  $A71D (bank 4) - lifted by port/tools/lift.py */
void func_A71D_b4(void) {
    REGS_IN;
    goto L_A71D;
L_A719:;
    mff_cycles(0xA719, 5);
    a = 0x00;
    RAM(TMP17) = a;
L_A71D:;
    mff_cycles(0xA71D, 7);
    x = RAM(TMP16);
    a = prg_read((uint16_t)(0xA782 + x));
    mff_cycles(0xA722, 8);
    RAM(VRAM_BUFFER) = a;
    a = prg_read((uint16_t)(0xA785 + x));
    mff_cycles(0xA728, 28);
    c = 0;
    m = RAM(TMP17);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(VRAM_BUFFER + 1) = a;
    a = 0x00;
    RAM(VRAM_BUFFER + 2) = a;
    y = RAM(TMP14);
    t = (uint8_t)(RAM(TMP14) + 1);
    RAM(TMP14) = t;
    a = mem_read((uint16_t)(RAM16(TMP12) + y));
    mff_cycles(0xA739, 4);
    m = 0xFF;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) { mff_cycles(0xA73B, 1); goto L_A77E; }
    mff_cycles(0xA73D, 4);
    m = 0xFE;
    c = a >= m;
    if (a != m) {
        mff_cycles(0xA741, 31);
        RAM(VRAM_BUFFER + 3) = a;
        a = 0xFF;
        RAM(VRAM_BUFFER + 4) = a;
        RAM(VRAM_LIST_REQUEST) = a;
        a = RAM(TMP16);
        mff_push(a);
        a = RAM(TMP17);
        n = (a) >> 7;
        z = (a) == 0;
        mff_push(a);
        SYNC_OUT;
        if (!mff_call_at(0x0753, 0xA788, 4)) return;
        SYNC_IN;
        mff_cycles(0xA754, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0756, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xA757, 26);
        a = mff_pull();
        RAM(TMP17) = a;
        a = mff_pull();
        RAM(TMP16) = a;
        t = (uint8_t)(RAM(TMP17) + 1);
        RAM(TMP17) = t;
        a = RAM(FRAME_COUNTER);
        a &= 0x03;
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0xA765, 8);
            a = 0x2B;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0769, func_F34C, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xA763, 1);
        }
        mff_cycles(0xA76A, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xA71D, 2); call_by_address_tail(0xA71D, 4); return;
    } else {
        mff_cycles(0xA73F, 1);
    }
    mff_cycles(0xA76D, 12);
    t = (uint8_t)(RAM(TMP16) + 1);
    RAM(TMP16) = t;
    a = RAM(TMP16);
    m = 0x03;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xA719, 1); goto L_A719; }
    mff_cycles(0xA775, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0777, 0xA7D0, 4)) return;
    SYNC_IN;
    mff_cycles(0xA778, 6);
    SYNC_OUT;
    if (!mff_call_at(0x077A, 0xA78E, 4)) return;
    SYNC_IN;
    mff_cycles(0xA77B, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA713, 2); call_by_address_tail(0xA713, 4); return;
L_A77E:;
    mff_cycles(0xA77E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xA7D0, 2); call_by_address_tail(0xA7D0, 4); return;
}

/*  $A788 (bank 4) - lifted by port/tools/lift.py */
void func_A788_b4(void) {
    REGS_IN;
    mff_cycles(0xA788, 8);
    x = 0x00;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x078C, func_F1A1, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xA78D, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $A78E (bank 4) - lifted by port/tools/lift.py */
void func_A78E_b4(void) {
    REGS_IN;
    mff_cycles(0xA78E, 52);
    a = 0x22;
    RAM(VRAM_BUFFER + 4) = a;
    a = 0x21;
    RAM(VRAM_BUFFER + 48) = a;
    RAM(VRAM_BUFFER + 26) = a;
    a = 0x27;
    RAM(VRAM_BUFFER + 5) = a;
    a = 0xE7;
    RAM(VRAM_BUFFER + 27) = a;
    a = 0xA7;
    RAM(VRAM_BUFFER + 49) = a;
    x = 0x12;
    RAM(VRAM_BUFFER + 6) = x;
    RAM(VRAM_BUFFER + 28) = x;
    RAM(VRAM_BUFFER + 50) = x;
    y = 0x00;
    a = 0x00;
    for (;;) {
        mff_cycles(0xA7B9, 21);
        RAM(VRAM_BUFFER + 7 + y) = a;
        RAM(VRAM_BUFFER + 29 + y) = a;
        RAM(VRAM_BUFFER + 51 + y) = a;
        y++;
        x--;
        if (x & 0x80) break;
        mff_cycles(0xA7B9, 1);
    }
    mff_cycles(0xA7C6, 12);
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(VRAM_BUFFER + 70) = a;
    RAM(VRAM_LIST_REQUEST) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xFF31, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_FF31(); g_code_window_base = swb; } return;
}

/*  $A7D0 (bank 4) - lifted by port/tools/lift.py */
void func_A7D0_b4(void) {
    REGS_IN;
    mff_cycles(0xA7D0, 8);
    x = 0xB4;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    mff_call_fn(0x07D4, func_FF2A, 0xE000);
    SYNC_IN;
    mff_cycles(0xA7D5, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $AB00 (bank 4) - lifted by port/tools/lift.py */
void func_AB00_b4(void) {
    REGS_IN;
    mff_cycles(0xAB00, 45);
    a = 0x80;
    RAM(OBJ_STATE) = a;
    a = 0xD0;
    RAM(OBJ_FLAGS) = a;
    a = 0x00;
    RAM(OBJ_X_HI) = a;
    RAM(OBJ_Y_HI) = a;
    RAM(OBJ_ID) = a;
    a = 0x58;
    RAM(OBJ_X) = a;
    a = 0xA8;
    RAM(OBJ_Y) = a;
    y = RAM(PLAYER_ID);
    a = prg_read((uint16_t)(0xAB2E + y));
    mff_cycles(0xAB24, 7);
    RAM(TMP0) = a;
    a = prg_read((uint16_t)(0xAB31 + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0xAB29, 8);
    RAM(TMP1) = a;
    SYNC_OUT;
    { uint16_t jt = nes_read16zp(0x0000); nes_cpu_instruction_boundary(jt, 2); call_by_address_tail(jt, -1); return; }
}

/*  $AB34 (bank 4) - lifted by port/tools/lift.py */
void func_AB34_b4(void) {
    REGS_IN;
    mff_cycles(0xAB34, 8);
    a = RAM(OBJ_STATE);
    a &= 0x0F;
    if (a == 0) {
        mff_cycles(0xAB3B, 8);
        a = 0x73;
        m = RAM(OBJ_ANIM);
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xAB42, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0B44, func_F294, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAB45, 8);
            a = 0x3F;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0B49, func_F440, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xAB40, 1);
        }
        mff_cycles(0xAB4A, 6);
        a = RAM(OBJ_ANIM_FRAME);
        if (a == 0) {
            mff_cycles(0xAB4F, 6);
            a = RAM(OBJ_ANIM_TIMER);
            if (a == 0) {
                mff_cycles(0xAB54, 8);
                a = 0x16;
                n = (a) >> 7;
                z = (a) == 0;
                SYNC_OUT;
                if (!mff_call_fn(0x0B58, func_F34C, 0xE000)) return;
                SYNC_IN;
            } else {
                mff_cycles(0xAB52, 1);
            }
        } else {
            mff_cycles(0xAB4D, 1);
        }
        mff_cycles(0xAB59, 8);
        a = RAM(OBJ_ANIM_FRAME);
        m = 0x06;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (z) {
            mff_cycles(0xAB60, 8);
            a = 0x13;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0B64, func_F294, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAB65, 8);
            a = 0x03;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0B69, func_F440, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAB6A, 18);
            a = 0x38;
            RAM(OBJ_VY_FRAC) = a;
            a = 0x04;
            RAM(OBJ_VY) = a;
            t = (uint8_t)(RAM(OBJ_STATE) + 1);
            RAM(OBJ_STATE) = t;
            n = (t) >> 7;
            z = (t) == 0;
        } else {
            mff_cycles(0xAB5E, 1);
        }
        mff_cycles(0xAB77, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0B79, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAB7A, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xAB34, 2); call_by_address_tail(0xAB34, 4); return;
    } else {
        mff_cycles(0xAB39, 1);
    }
    mff_cycles(0xAB7D, 8);
    a = RAM(OBJ_STATE);
    a &= 0x02;
    if (a != 0) { mff_cycles(0xAB82, 1); goto L_AB95; }
    mff_cycles(0xAB84, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x05;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xAB89, 1); goto L_AB9A; }
    mff_cycles(0xAB8B, 12);
    a = 0x05;
    RAM(OBJ_ANIM_FRAME) = a;
    a = 0x00;
    RAM(OBJ_ANIM_TIMER) = a;
L_AB95:;
    mff_cycles(0xAB95, 8);
    x = 0x00;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0B99, func_F1FD, 0xE000)) return;
    SYNC_IN;
L_AB9A:;
    mff_cycles(0xAB9A, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B9C, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAB9D, 6);
    a = RAM(OBJ_VY);
    if (a & 0x80) {
        mff_cycles(0xABA2, 8);
        a = 0x3E;
        m = RAM(OBJ_ANIM);
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (!z) {
            mff_cycles(0xABA9, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0BAB, func_F294, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xABAC, 8);
            a = 0x01;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0BB0, func_F440, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xABB1, 6);
            t = (uint8_t)(RAM(OBJ_STATE) + 1);
            RAM(OBJ_STATE) = t;
        } else {
            mff_cycles(0xABA7, 1);
        }
    } else {
        mff_cycles(0xABA0, 1);
    }
    mff_cycles(0xABB4, 8);
    a = RAM(OBJ_Y);
    m = 0xA9;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0xABBB, 14);
        a = 0xA8;
        RAM(OBJ_Y) = a;
        a = 0x01;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0BC4, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xABC5, 8);
        a = 0x00;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0BC9, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xABCA, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xF406, 2); { uint16_t swb = g_code_window_base; g_code_window_base = 0xE000; func_F406(); g_code_window_base = swb; } return;
    } else {
        mff_cycles(0xABB9, 1);
    }
    mff_cycles(0xABCD, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAB77, 2); call_by_address_tail(0xAB77, 4); return;
}

/*  $AB77 (bank 4) - lifted by port/tools/lift.py */
void func_AB77_b4(void) {
    REGS_IN;
    mff_cycles(0xAB77, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0B79, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAB7A, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAB34, 2); call_by_address_tail(0xAB34, 4); return;
}

/*  $ABD0 (bank 4) - lifted by port/tools/lift.py */
void func_ABD0_b4(void) {
    REGS_IN;
    mff_cycles(0xABD0, 8);
    a = 0x24;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xABD7, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0BD9, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xABDA, 8);
        a = 0x32;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0BDE, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xABDF, 6);
        a = RAM(OBJ_ANIM_FRAME);
        if (a == 0) {
            mff_cycles(0xABE4, 6);
            a = RAM(OBJ_ANIM_TIMER);
            if (a == 0) {
                mff_cycles(0xABE9, 8);
                a = 0x1B;
                n = (a) >> 7;
                z = (a) == 0;
                SYNC_OUT;
                if (!mff_call_fn(0x0BED, func_F34C, 0xE000)) return;
                SYNC_IN;
            } else {
                mff_cycles(0xABE7, 1);
            }
        } else {
            mff_cycles(0xABE2, 1);
        }
    } else {
        mff_cycles(0xABD5, 1);
    }
    mff_cycles(0xABEE, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x0B;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xABF5, 8);
        a = 0x34;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0BF9, func_F34C, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xABF3, 1);
    }
    mff_cycles(0xABFA, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x13;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xAC01, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0C03, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAC04, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xABDF, 2); call_by_address_tail(0xABDF, 4); return;
    } else {
        mff_cycles(0xABFF, 1);
    }
    mff_cycles(0xAC07, 8);
    a = 0x14;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xAC0E, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0C10, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAC11, 8);
        a = 0x07;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0C15, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAC16, 12);
        a = 0x33;
        RAM(OBJ_VY_FRAC) = a;
        a = 0x01;
        RAM(OBJ_VY) = a;
    } else {
        mff_cycles(0xAC0C, 1);
    }
    mff_cycles(0xAC20, 8);
    a = RAM(OBJ_ANIM);
    m = 0x5B;
    c = a >= m;
    if (a == m) { mff_cycles(0xAC25, 1); goto L_AC55; }
    mff_cycles(0xAC27, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x04;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xAC2E, 6);
        a = RAM(OBJ_ANIM_TIMER);
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0xAC33, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0C35, func_F430, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xAC31, 1);
        }
    } else {
        mff_cycles(0xAC2C, 1);
    }
    mff_cycles(0xAC36, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x06;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xAC3B, 1); goto L_AC7B; }
    mff_cycles(0xAC3D, 8);
    a = 0x5B;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0C41, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC42, 8);
    a = 0x05;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0C46, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC47, 19);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(OBJ_Y);
    c = 1;
    m = 0x20;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_Y) = a;
L_AC55:;
    mff_cycles(0xAC55, 8);
    x = 0x00;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0C59, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC5A, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0xAC5F, 8);
        a = RAM(OBJ_Y);
        m = 0xA8;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0xAC66, 14);
            a = 0xA8;
            RAM(OBJ_Y) = a;
            a = 0x02;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0C6F, func_F294, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAC70, 8);
            a = 0x04;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0C74, func_F440, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAC75, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0C77, func_F406, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAC78, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xAC75, 2); call_by_address_tail(0xAC75, 4); return;
        } else {
            mff_cycles(0xAC64, 1);
        }
    } else {
        mff_cycles(0xAC5D, 1);
    }
L_AC7B:;
    mff_cycles(0xAC7B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C7D, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC7E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAC20, 2); call_by_address_tail(0xAC20, 4); return;
}

/*  $ABDF (bank 4) - lifted by port/tools/lift.py */
void func_ABDF_b4(void) {
    REGS_IN;
    mff_cycles(0xABDF, 6);
    a = RAM(OBJ_ANIM_FRAME);
    if (a == 0) {
        mff_cycles(0xABE4, 6);
        a = RAM(OBJ_ANIM_TIMER);
        if (a == 0) {
            mff_cycles(0xABE9, 8);
            a = 0x1B;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0BED, func_F34C, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xABE7, 1);
        }
    } else {
        mff_cycles(0xABE2, 1);
    }
    mff_cycles(0xABEE, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x0B;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xABF5, 8);
        a = 0x34;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0BF9, func_F34C, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xABF3, 1);
    }
    mff_cycles(0xABFA, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x13;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xAC01, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0C03, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAC04, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xABDF, 2); call_by_address_tail(0xABDF, 4); return;
    } else {
        mff_cycles(0xABFF, 1);
    }
    mff_cycles(0xAC07, 8);
    a = 0x14;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xAC0E, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0C10, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAC11, 8);
        a = 0x07;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0C15, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAC16, 12);
        a = 0x33;
        RAM(OBJ_VY_FRAC) = a;
        a = 0x01;
        RAM(OBJ_VY) = a;
    } else {
        mff_cycles(0xAC0C, 1);
    }
    mff_cycles(0xAC20, 8);
    a = RAM(OBJ_ANIM);
    m = 0x5B;
    c = a >= m;
    if (a == m) { mff_cycles(0xAC25, 1); goto L_AC55; }
    mff_cycles(0xAC27, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x04;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xAC2E, 6);
        a = RAM(OBJ_ANIM_TIMER);
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0xAC33, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0C35, func_F430, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xAC31, 1);
        }
    } else {
        mff_cycles(0xAC2C, 1);
    }
    mff_cycles(0xAC36, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x06;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xAC3B, 1); goto L_AC7B; }
    mff_cycles(0xAC3D, 8);
    a = 0x5B;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0C41, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC42, 8);
    a = 0x05;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0C46, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC47, 19);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(OBJ_Y);
    c = 1;
    m = 0x20;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_Y) = a;
L_AC55:;
    mff_cycles(0xAC55, 8);
    x = 0x00;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0C59, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC5A, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0xAC5F, 8);
        a = RAM(OBJ_Y);
        m = 0xA8;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0xAC66, 14);
            a = 0xA8;
            RAM(OBJ_Y) = a;
            a = 0x02;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0C6F, func_F294, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAC70, 8);
            a = 0x04;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0C74, func_F440, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAC75, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0C77, func_F406, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAC78, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xAC75, 2); call_by_address_tail(0xAC75, 4); return;
        } else {
            mff_cycles(0xAC64, 1);
        }
    } else {
        mff_cycles(0xAC5D, 1);
    }
L_AC7B:;
    mff_cycles(0xAC7B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C7D, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC7E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAC20, 2); call_by_address_tail(0xAC20, 4); return;
}

/*  $AC20 (bank 4) - lifted by port/tools/lift.py */
void func_AC20_b4(void) {
    REGS_IN;
    mff_cycles(0xAC20, 8);
    a = RAM(OBJ_ANIM);
    m = 0x5B;
    c = a >= m;
    if (a == m) { mff_cycles(0xAC25, 1); goto L_AC55; }
    mff_cycles(0xAC27, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x04;
    c = a >= m;
    if (a == m) {
        mff_cycles(0xAC2E, 6);
        a = RAM(OBJ_ANIM_TIMER);
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0xAC33, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0C35, func_F430, 0xE000)) return;
            SYNC_IN;
        } else {
            mff_cycles(0xAC31, 1);
        }
    } else {
        mff_cycles(0xAC2C, 1);
    }
    mff_cycles(0xAC36, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x06;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0xAC3B, 1); goto L_AC7B; }
    mff_cycles(0xAC3D, 8);
    a = 0x5B;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0C41, func_F294, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC42, 8);
    a = 0x05;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0C46, func_F440, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC47, 19);
    a = RAM(OBJ_Y);
    RAM(PLAYER_GROUND_Y) = a;
    a = RAM(OBJ_Y);
    c = 1;
    m = 0x20;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(OBJ_Y) = a;
L_AC55:;
    mff_cycles(0xAC55, 8);
    x = 0x00;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0C59, func_F1FD, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC5A, 6);
    a = RAM(OBJ_VY);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0xAC5F, 8);
        a = RAM(OBJ_Y);
        m = 0xA8;
        c = a >= m;
        n = ((uint8_t)(a - m)) >> 7;
        z = ((uint8_t)(a - m)) == 0;
        if (c) {
            mff_cycles(0xAC66, 14);
            a = 0xA8;
            RAM(OBJ_Y) = a;
            a = 0x02;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0C6F, func_F294, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAC70, 8);
            a = 0x04;
            n = (a) >> 7;
            z = (a) == 0;
            SYNC_OUT;
            if (!mff_call_fn(0x0C74, func_F440, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAC75, 6);
            SYNC_OUT;
            if (!mff_call_fn(0x0C77, func_F406, 0xE000)) return;
            SYNC_IN;
            mff_cycles(0xAC78, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0xAC75, 2); call_by_address_tail(0xAC75, 4); return;
        } else {
            mff_cycles(0xAC64, 1);
        }
    } else {
        mff_cycles(0xAC5D, 1);
    }
L_AC7B:;
    mff_cycles(0xAC7B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C7D, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC7E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAC20, 2); call_by_address_tail(0xAC20, 4); return;
}

/*  $AC75 (bank 4) - lifted by port/tools/lift.py */
void func_AC75_b4(void) {
    REGS_IN;
    mff_cycles(0xAC75, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0C77, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAC78, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAC75, 2); call_by_address_tail(0xAC75, 4); return;
}

/*  $AC81 (bank 4) - lifted by port/tools/lift.py */
void func_AC81_b4(void) {
    REGS_IN;
    mff_cycles(0xAC81, 8);
    a = 0x30;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xAC88, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0C8A, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAC8B, 8);
        a = 0x0B;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0C8F, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAC90, 30);
        a = 0x08;
        RAM(OBJ_TIMER) = a;
        a = 0x1E;
        RAM(OBJ_TIMER + 1) = a;
        a = 0x00;
        RAM(OBJ_VX_FRAC) = a;
        a = 0x03;
        RAM(OBJ_VX) = a;
        a = 0x90;
        RAM(OBJ_FLAGS) = a;
    } else {
        mff_cycles(0xAC86, 1);
    }
    mff_cycles(0xACA9, 6);
    a = RAM(OBJ_TIMER);
    if (a != 0) {
        mff_cycles(0xACAE, 12);
        t = (uint8_t)(RAM(OBJ_TIMER) - 1);
        RAM(OBJ_TIMER) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CB3, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xACB4, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xAC81, 2); call_by_address_tail(0xAC81, 4); return;
    } else {
        mff_cycles(0xACAC, 1);
    }
    mff_cycles(0xACB7, 8);
    a = 0x3D;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xACBE, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0CC0, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xACC1, 8);
        a = 0x0A;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CC5, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xACC6, 8);
        a = 0x2C;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CCA, func_F34C, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xACBC, 1);
    }
    mff_cycles(0xACCB, 8);
    x = 0x00;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0CCF, func_F03F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xACD0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0CD2, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xACD3, 6);
    a = RAM(OBJ_TIMER + 1);
    if (a != 0) {
        mff_cycles(0xACD8, 12);
        t = (uint8_t)(RAM(OBJ_TIMER + 1) - 1);
        RAM(OBJ_TIMER + 1) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CDD, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xACDE, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xACB7, 2); call_by_address_tail(0xACB7, 4); return;
    } else {
        mff_cycles(0xACD6, 1);
    }
    mff_cycles(0xACE1, 8);
    a = 0x47;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xACE8, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0CEA, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xACEB, 8);
        a = 0x11;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CEF, func_F440, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xACE6, 1);
    }
    mff_cycles(0xACF0, 6);
    a = RAM(OBJ_ANIM_FRAME);
    if (a == 0) {
        mff_cycles(0xACF5, 8);
        a = 0x1D;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CF9, func_F34C, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xACF3, 1);
    }
    mff_cycles(0xACFA, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x0C;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xAD01, 8);
        a = 0x03;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0D05, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAD06, 8);
        a = 0x08;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0D0A, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAD0B, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0D0D, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAD0E, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xAD0B, 2); call_by_address_tail(0xAD0B, 4); return;
    } else {
        mff_cycles(0xACFF, 1);
    }
    mff_cycles(0xAD11, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D13, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAD14, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xACE1, 2); call_by_address_tail(0xACE1, 4); return;
}

/*  $ACB7 (bank 4) - lifted by port/tools/lift.py */
void func_ACB7_b4(void) {
    REGS_IN;
    mff_cycles(0xACB7, 8);
    a = 0x3D;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xACBE, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0CC0, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xACC1, 8);
        a = 0x0A;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CC5, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xACC6, 8);
        a = 0x2C;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CCA, func_F34C, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xACBC, 1);
    }
    mff_cycles(0xACCB, 8);
    x = 0x00;
    n = (x) >> 7;
    z = (x) == 0;
    SYNC_OUT;
    if (!mff_call_fn(0x0CCF, func_F03F, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xACD0, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0CD2, func_F183, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xACD3, 6);
    a = RAM(OBJ_TIMER + 1);
    if (a != 0) {
        mff_cycles(0xACD8, 12);
        t = (uint8_t)(RAM(OBJ_TIMER + 1) - 1);
        RAM(OBJ_TIMER + 1) = t;
        n = (t) >> 7;
        z = (t) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CDD, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xACDE, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xACB7, 2); call_by_address_tail(0xACB7, 4); return;
    } else {
        mff_cycles(0xACD6, 1);
    }
    mff_cycles(0xACE1, 8);
    a = 0x47;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xACE8, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0CEA, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xACEB, 8);
        a = 0x11;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CEF, func_F440, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xACE6, 1);
    }
    mff_cycles(0xACF0, 6);
    a = RAM(OBJ_ANIM_FRAME);
    if (a == 0) {
        mff_cycles(0xACF5, 8);
        a = 0x1D;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CF9, func_F34C, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xACF3, 1);
    }
    mff_cycles(0xACFA, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x0C;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xAD01, 8);
        a = 0x03;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0D05, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAD06, 8);
        a = 0x08;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0D0A, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAD0B, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0D0D, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAD0E, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xAD0B, 2); call_by_address_tail(0xAD0B, 4); return;
    } else {
        mff_cycles(0xACFF, 1);
    }
    mff_cycles(0xAD11, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D13, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAD14, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xACE1, 2); call_by_address_tail(0xACE1, 4); return;
}

/*  $ACE1 (bank 4) - lifted by port/tools/lift.py */
void func_ACE1_b4(void) {
    REGS_IN;
    mff_cycles(0xACE1, 8);
    a = 0x47;
    m = RAM(OBJ_ANIM);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0xACE8, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0CEA, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xACEB, 8);
        a = 0x11;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CEF, func_F440, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xACE6, 1);
    }
    mff_cycles(0xACF0, 6);
    a = RAM(OBJ_ANIM_FRAME);
    if (a == 0) {
        mff_cycles(0xACF5, 8);
        a = 0x1D;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0CF9, func_F34C, 0xE000)) return;
        SYNC_IN;
    } else {
        mff_cycles(0xACF3, 1);
    }
    mff_cycles(0xACFA, 8);
    a = RAM(OBJ_ANIM_FRAME);
    m = 0x0C;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (z) {
        mff_cycles(0xAD01, 8);
        a = 0x03;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0D05, func_F294, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAD06, 8);
        a = 0x08;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_fn(0x0D0A, func_F440, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAD0B, 6);
        SYNC_OUT;
        if (!mff_call_fn(0x0D0D, func_F406, 0xE000)) return;
        SYNC_IN;
        mff_cycles(0xAD0E, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0xAD0B, 2); call_by_address_tail(0xAD0B, 4); return;
    } else {
        mff_cycles(0xACFF, 1);
    }
    mff_cycles(0xAD11, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D13, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAD14, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xACE1, 2); call_by_address_tail(0xACE1, 4); return;
}

/*  $AD0B (bank 4) - lifted by port/tools/lift.py */
void func_AD0B_b4(void) {
    REGS_IN;
    mff_cycles(0xAD0B, 6);
    SYNC_OUT;
    if (!mff_call_fn(0x0D0D, func_F406, 0xE000)) return;
    SYNC_IN;
    mff_cycles(0xAD0E, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xAD0B, 2); call_by_address_tail(0xAD0B, 4); return;
}
