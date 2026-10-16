/*
 * lifted_b3.c - bank 3 routines $8000-$BFFF, lifted by
 * port/tools/lift_bank.py from the pure scaffold: exact, still to be
 * tidied and moved into their systems' files.
 */
#include "mff.h"
#include "state.h"
#include "coroutine.h"


/* snd_envelope $86BA (bank 3) - lifted by port/tools/lift.py */
void func_86BA_b3(void) {
    REGS_IN;
    mff_cycles(0x86BA, 19);
    a = RAM(SND_LEVEL + x);
    RAM(SND_TMP + 3) = a;
    a = RAM(SND_ENV + x);
    a &= 0x07;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x06C6, 0x8023, 3)) return;
    SYNC_IN;
    mff_cycles(0x86C7, 5);
    m = mem_read((uint16_t)(RAM16(TASKS + 6) + y));
    c = a >= m;
    mff_cycles(0x86C9, 11);
    t = (uint8_t)(RAM(TASKS + 6) + 1);
    RAM(TASKS + 6) = t;
    n = (t) >> 7;
    z = (t) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x06CD, 0x0287, -1)) return;
    SYNC_IN;
    mff_cycles(0x86CE, 0);
    mff_unreachable(0x86CE); return;   /* data: the branch above always goes */
}

/* snd_env_attack $86D1 (bank 3) - lifted by port/tools/lift.py */
void func_86D1_b3(void) {
    REGS_IN;
    mff_cycles(0x86D1, 7);
    y = 0x00;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x86D5, 11);
    y = a;
    a = RAM(SND_TMP + 3);
    c = 0;
    m = prg_read((uint16_t)(0x8933 + y));
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0x86DC, 2);
    if (c) { mff_cycles(0x86DC, 1); goto L_86E2; }
    mff_cycles(0x86DE, 4);
    m = 0xF0;
    c = a >= m;
    if (!c) { mff_cycles(0x86E0, 2); goto L_871D; }
L_86E2:;
    mff_cycles(0x86E2, 4);
    a = 0xF0;
    if (a != 0) { mff_cycles(0x86E4, 2); goto L_871A; }
    mff_cycles(0x86E6, 7);
    y = 0x01;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x86EA, 2);
    if (a == 0) { mff_cycles(0x86EA, 1); goto L_86FB; }
    mff_cycles(0x86EC, 11);
    y = a;
    a = RAM(SND_TMP + 3);
    c = 1;
    m = prg_read((uint16_t)(0x8933 + y));
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    mff_cycles(0x86F3, 2);
    if (!c) { mff_cycles(0x86F3, 1); goto L_86FB; }
    mff_cycles(0x86F5, 7);
    y = 0x02;
    m = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    c = a >= m;
    mff_cycles(0x86F9, 2);
    if (c) { mff_cycles(0x86F9, 2); goto L_871D; }
L_86FB:;
    mff_cycles(0x86FB, 7);
    y = 0x02;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x86FF, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x871A, 2); call_by_address_tail(0x871A, 3); return;
L_871A:;
    mff_cycles(0x871A, 7);
    t = (uint8_t)(RAM(SND_ENV + x) + 1);
    RAM(SND_ENV + x) = t;
L_871D:;
    mff_cycles(0x871D, 9);
    RAM(SND_LEVEL + x) = a;
    m = 0x28;
    c = x >= m;
    if (!c) { mff_cycles(0x8722, 1); goto L_8737; }
    mff_cycles(0x8724, 5);
    a = RAM(SND_EFFECT_CHANNELS);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0x8728, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x88A0, 2); call_by_address_tail(0x88A0, 3); return;
    } else {
        mff_cycles(0x8726, 1);
    }
    mff_cycles(0x872B, 8);
    a = RAM(SND_FADE_LEVEL);
    y = RAM(SND_FADE_SPEED);
    if (!(y & 0x80)) {
        mff_cycles(0x8731, 2);
        a ^= 0xFF;
    } else {
        mff_cycles(0x872F, 1);
    }
    mff_cycles(0x8733, 4);
    if (a != 0xFF) { mff_cycles(0x8735, 1); goto L_8740; }
L_8737:;
    mff_cycles(0x8737, 8);
    a = x;
    a &= 0x03;
    m = 0x01;
    c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x873C, 1); goto L_8760; }
    mff_cycles(0x873E, 2);
    if (z) { mff_cycles(0x873E, 1); goto L_8752; }
L_8740:;
    mff_cycles(0x8740, 4);
    m = 0x29;
    c = x >= m;
    if (x != m) { mff_cycles(0x8742, 1); goto L_875B; }
    mff_cycles(0x8744, 16);
    RAM(SND_TMP + 3) = a;
    a = RAM(SND_M_SLIDE + x);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_TMP) = a;
    SYNC_OUT;
    if (!mff_call_at(0x074D, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x874E, 5);
    a = RAM(SND_TMP);
    if (a == 0) { mff_cycles(0x8750, 1); goto L_87AA; }
L_8752:;
    mff_cycles(0x8752, 6);
    a = RAM(SND_LEVEL + x);
    if (a == 0) { mff_cycles(0x8755, 1); goto L_87AA; }
    mff_cycles(0x8757, 4);
    a = 0xFF;
    if (a != 0) { mff_cycles(0x8759, 1); goto L_87AA; }
L_875B:;
    mff_cycles(0x875B, 6);
    if (a < RAM(SND_LEVEL + x)) { mff_cycles(0x875E, 1); goto L_8763; }
L_8760:;
    mff_cycles(0x8760, 4);
    a = RAM(SND_LEVEL + x);
L_8763:;
    mff_cycles(0x8763, 20);
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    a ^= 0x0F;
    RAM(SND_TMP + 2) = a;
    y = 0x06;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x876F, 4);
    m = 0x05;
    if (a < m) { mff_cycles(0x8771, 1); goto L_8797; }
    mff_cycles(0x8773, 19);
    RAM(SND_TMP + 3) = a;
    y = RAM(SND_LFO + x);
    a = RAM(SND_ENV + x);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = y;
    n = (a) >> 7;
    z = (a) == 0;
    if (c) {
        mff_cycles(0x8780, 2);
        a ^= 0xFF;
        n = (a) >> 7;
        z = (a) == 0;
    } else {
        mff_cycles(0x877E, 1);
    }
    mff_cycles(0x8782, 2);
    if (z) { mff_cycles(0x8782, 1); goto L_8797; }
    mff_cycles(0x8784, 9);
    RAM(SND_TMP) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0788, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x8789, 11);
    a = RAM(SND_TMP);
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    m = 0x10;
    c = a >= m;
    if (c) { mff_cycles(0x878F, 1); goto L_87A5; }
    mff_cycles(0x8791, 5);
    if (a >= RAM(SND_TMP + 2)) {
        mff_cycles(0x8795, 3);
        RAM(SND_TMP + 2) = a;
    } else {
        mff_cycles(0x8793, 1);
    }
L_8797:;
    mff_cycles(0x8797, 19);
    a = 0x10;
    RAM(SND_TMP + 3) = a;
    a = RAM(SND_VOLUME + x);
    c = 1;
    m = RAM(SND_TMP + 2);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    m = RAM(SND_TMP + 3);
    v = (m >> 6) & 1;
    if ((a & m) != 0) { mff_cycles(0x87A3, 1); goto L_87AA; }
L_87A5:;
    mff_cycles(0x87A5, 6);
    a = RAM(SND_VOLUME + x);
    a &= 0xF0;
L_87AA:;
    mff_cycles(0x87AA, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x07AE, 0x80EC, 3)) return;
    SYNC_IN;
    mff_cycles(0x87AF, 12);
    a = x;
    a &= 0x03;
    y = a;
    a = RAM(SND_PERIOD_HI_CACHE + y);
    if (a & 0x80) { mff_cycles(0x87B6, 2); goto L_880C; }
    mff_cycles(0x87B8, 7);
    y = 0x05;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x87BC, 2);
    if (a == 0) { mff_cycles(0x87BC, 2); goto L_880C; }
    mff_cycles(0x87BE, 19);
    RAM(SND_TMP + 3) = a;
    y = RAM(SND_LFO + x);
    a = RAM(SND_ENV + x);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = y;
    n = (a) >> 7;
    z = (a) == 0;
    if (c) {
        mff_cycles(0x87CB, 2);
        a ^= 0xFF;
        n = (a) >> 7;
        z = (a) == 0;
    } else {
        mff_cycles(0x87C9, 1);
    }
    mff_cycles(0x87CD, 2);
    if (z) { mff_cycles(0x87CD, 2); goto L_880C; }
    mff_cycles(0x87CF, 9);
    RAM(SND_TMP) = a;
    SYNC_OUT;
    if (!mff_call_at(0x07D3, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x87D4, 38);
    a = RAM(SND_TMP);
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    t = RAM(SND_TMP + 1);
    t = (uint8_t)(t >> 1 | c << 7);
    RAM(SND_TMP + 1) = t;
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    t = RAM(SND_TMP + 1);
    t = (uint8_t)(t >> 1 | c << 7);
    RAM(SND_TMP + 1) = t;
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    t = RAM(SND_TMP + 1);
    t = (uint8_t)(t >> 1 | c << 7);
    RAM(SND_TMP + 1) = t;
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    t = RAM(SND_TMP + 1);
    t = (uint8_t)(t >> 1 | c << 7);
    RAM(SND_TMP + 1) = t;
    y = a;
    a |= RAM(SND_TMP + 1);
    if (a == 0) { mff_cycles(0x87E5, 2); goto L_880C; }
    mff_cycles(0x87E7, 6);
    a = RAM(SND_ENV + x);
    if (a & 0x80) { mff_cycles(0x87EA, 1); goto L_87FA; }
    mff_cycles(0x87EC, 20);
    c = 0;
    a = RAM(SND_TMP + 1);
    m = RAM(SND_PITCH_LO + x);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(SND_TMP + 1) = a;
    a = y;
    m = RAM(SND_PITCH_HI + x);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    if (a != 0) { mff_cycles(0x87F8, 2); goto L_8809; }
L_87FA:;
    mff_cycles(0x87FA, 22);
    c = 1;
    a = RAM(SND_PITCH_LO + x);
    m = RAM(SND_TMP + 1);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(SND_TMP + 1) = a;
    a = RAM(SND_PITCH_HI + x);
    RAM(SND_TMP) = y;
    m = RAM(SND_TMP);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
L_8809:;
    mff_cycles(0x8809, 4);
    y = a;
    if (y != 0) { mff_cycles(0x880A, 1); goto L_8814; }
L_880C:;
    mff_cycles(0x880C, 11);
    a = RAM(SND_PITCH_LO + x);
    RAM(SND_TMP + 1) = a;
    y = RAM(SND_PITCH_HI + x);
L_8814:;
    mff_cycles(0x8814, 4);
    m = 0x28;
    c = x >= m;
    if (!c) {
        mff_cycles(0x8818, 5);
        a = RAM(SND_EFFECT_LOOP);
        if (a & 0x80) {
            mff_cycles(0x881C, 5);
            a = RAM(SND_EFFECT_PITCH_SCALE);
            if (a != 0) {
                mff_cycles(0x8820, 18);
                RAM(SND_TMP + 3) = a;
                RAM(SND_TMP) = y;
                a = RAM(SND_TMP + 1);
                n = (a) >> 7;
                z = (a) == 0;
                mff_push(a);
                SYNC_OUT;
                if (!mff_call_at(0x0829, 0x8006, 3)) return;
                SYNC_IN;
                mff_cycles(0x882A, 19);
                a = mff_pull();
                c = 0;
                m = RAM(SND_TMP + 1);
                w = a + m + c;
                c = w > 0xFF;
                a = (uint8_t)w;
                RAM(SND_TMP + 1) = a;
                a = 0x00;
                m = RAM(SND_TMP);
                w = a + m + c;
                v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
                c = w > 0xFF;
                a = (uint8_t)w;
                y = a;
            } else {
                mff_cycles(0x881E, 1);
            }
        } else {
            mff_cycles(0x881A, 1);
        }
    } else {
        mff_cycles(0x8816, 1);
    }
    mff_cycles(0x8835, 6);
    a = x;
    a &= 0x03;
    if (a != 0) { mff_cycles(0x8838, 1); goto L_8849; }
    mff_cycles(0x883A, 11);
    a = y;
    a &= 0x0F;
    y = 0x07;
    a |= mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x8841, 10);
    RAM(SND_TMP + 1) = a;
    a = 0x00;
    RAM(SND_TMP) = a;
    if (a == 0) { mff_cycles(0x8847, 1); goto L_8884; }
L_8849:;
    mff_cycles(0x8849, 4);
    a = y;
    y = 0x08;
    for (;;) {
        mff_cycles(0x884C, 6);
        y--;
        mff_cycles(0x8850, 2);
        if (a >= prg_read((uint16_t)(0x8953 + y))) break;
        mff_cycles(0x884C, 1);
    }
    mff_cycles(0x8852, 29);
    RAM(SND_TMP) = a;
    a = y;
    c = 0;
    m = RAM(SND_TMP);
    w = a + m + c;
    a = (uint8_t)w;
    y = a;
    a &= 0x07;
    c = 0;
    m = 0x07;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(SND_TMP) = a;
    a = y;
    a &= 0x38;
    a ^= 0x38;
    if (a != 0) {
        for (;;) {
            mff_cycles(0x8867, 16);
            t = RAM(SND_TMP);
            c = t & 1;
            t >>= 1;
            RAM(SND_TMP) = t;
            t = RAM(SND_TMP + 1);
            t = (uint8_t)(t >> 1 | c << 7);
            RAM(SND_TMP + 1) = t;
            c = 1;
            m = 0x08;
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            c = w >= 0;
            a = (uint8_t)w;
            if (a == 0) break;
            mff_cycles(0x8867, 1);
        }
    } else {
        mff_cycles(0x8865, 1);
    }
    mff_cycles(0x8870, 8);
    y = 0x00;
    a = RAM(SND_DETUNE + x);
    n = (a) >> 7;
    if (a != 0) {
        mff_cycles(0x8877, 2);
        if (n) {
            mff_cycles(0x8879, 2);
            y--;
        } else {
            mff_cycles(0x8877, 1);
        }
        mff_cycles(0x887A, 16);
        c = 0;
        m = RAM(SND_TMP + 1);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(SND_TMP + 1) = a;
        a = y;
        m = RAM(SND_TMP);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(SND_TMP) = a;
    } else {
        mff_cycles(0x8875, 1);
    }
L_8884:;
    mff_cycles(0x8884, 11);
    y = 0x02;
    a = RAM(SND_TMP + 1);
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x088A, 0x80EC, 3)) return;
    SYNC_IN;
    mff_cycles(0x888B, 15);
    a = x;
    a &= 0x03;
    y = a;
    a = RAM(SND_TMP);
    m = RAM(SND_PERIOD_HI_CACHE + y);
    c = a >= m;
    if (a != m) {
        mff_cycles(0x8896, 15);
        RAM(SND_PERIOD_HI_CACHE + y) = a;
        a |= 0x08;
        y = 0x03;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x089F, 0x80EC, 3)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x8894, 1);
    }
    mff_cycles(0x88A0, 8);
    a = RAM(SND_ENV + x);
    a &= 0x20;
    if (a == 0) { mff_cycles(0x88A5, 1); goto L_88FA; }
    mff_cycles(0x88A7, 6);
    a = RAM(SND_SLIDE_AMOUNT + x);
    if (a == 0) { mff_cycles(0x88AA, 1); goto L_88F2; }
    mff_cycles(0x88AC, 9);
    y = 0x00;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    n = (t) >> 7;
    z = (t) == 0;
    FLAGS_OUT;
    mff_push(mff_p());
    if (c) {
        mff_cycles(0x88B2, 8);
        a ^= 0xFF;
        c = 0;
        m = 0x01;
        w = a + m + c;
        a = (uint8_t)w;
        y--;
    } else {
        mff_cycles(0x88B0, 1);
    }
    mff_cycles(0x88B8, 40);
    c = 0;
    m = RAM(SND_PITCH_LO + x);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(SND_PITCH_LO + x) = a;
    a = y;
    m = RAM(SND_PITCH_HI + x);
    w = a + m + c;
    a = (uint8_t)w;
    RAM(SND_PITCH_HI + x) = a;
    a = RAM(SND_TARGET + x);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    y = a;
    c = 1;
    a = RAM(SND_PITCH_LO + x);
    m = prg_read((uint16_t)(0x8959 + y));
    w = a - m - !c;
    c = w >= 0;
    mff_cycles(0x88D2, 10);
    a = RAM(SND_PITCH_HI + x);
    a &= 0x3F;
    m = prg_read((uint16_t)(0x895A + y));
    w = a - m - !c;
    c = w >= 0;
    mff_cycles(0x88DA, 12);
    a = 0xFF;
    m = 0x00;
    w = a + m + c;
    a = (uint8_t)w;
    mff_set_p(mff_pull());
    FLAGS_IN;
    m = 0x00;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    if (a != 0) { mff_cycles(0x88E1, 1); goto L_88FA; }
    mff_cycles(0x88E3, 4);
    a = x;
    if (a == 0) { mff_cycles(0x88E4, 1); goto L_88FA; }
    mff_cycles(0x88E6, 4);
    a = prg_read((uint16_t)(0x8959 + y));
    mff_cycles(0x88E9, 9);
    RAM(SND_PITCH_LO + x) = a;
    a = prg_read((uint16_t)(0x895A + y));
    mff_cycles(0x88EF, 5);
    RAM(SND_PITCH_HI + x) = a;
L_88F2:;
    mff_cycles(0x88F2, 11);
    a = RAM(SND_ENV + x);
    a &= 0xDF;
    RAM(SND_ENV + x) = a;
L_88FA:;
    mff_cycles(0x88FA, 7);
    y = 0x04;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x88FE, 4);
    a &= 0x7F;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0x8902, 13);
        c = 0;
        m = RAM(SND_LFO + x);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(SND_LFO + x) = a;
        if (c) {
            mff_cycles(0x890B, 13);
            a = RAM(SND_ENV + x);
            c = 0;
            m = 0x40;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(SND_ENV + x) = a;
        } else {
            mff_cycles(0x8909, 1);
        }
    } else {
        mff_cycles(0x8900, 1);
    }
    mff_cycles(0x8914, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/* snd_env_decay $86E6 (bank 3) - lifted by port/tools/lift.py */
void func_86E6_b3(void) {
    REGS_IN;
    mff_cycles(0x86E6, 7);
    y = 0x01;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x86EA, 2);
    if (a == 0) { mff_cycles(0x86EA, 1); goto L_86FB; }
    mff_cycles(0x86EC, 11);
    y = a;
    a = RAM(SND_TMP + 3);
    c = 1;
    m = prg_read((uint16_t)(0x8933 + y));
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    mff_cycles(0x86F3, 2);
    if (!c) { mff_cycles(0x86F3, 1); goto L_86FB; }
    mff_cycles(0x86F5, 7);
    y = 0x02;
    m = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    c = a >= m;
    mff_cycles(0x86F9, 2);
    if (c) { mff_cycles(0x86F9, 2); goto L_871D; }
L_86FB:;
    mff_cycles(0x86FB, 7);
    y = 0x02;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x86FF, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x871A, 2); call_by_address_tail(0x871A, 3); return;
L_871D:;
    mff_cycles(0x871D, 9);
    RAM(SND_LEVEL + x) = a;
    m = 0x28;
    c = x >= m;
    if (!c) { mff_cycles(0x8722, 1); goto L_8737; }
    mff_cycles(0x8724, 5);
    a = RAM(SND_EFFECT_CHANNELS);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0x8728, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x88A0, 2); call_by_address_tail(0x88A0, 3); return;
    } else {
        mff_cycles(0x8726, 1);
    }
    mff_cycles(0x872B, 8);
    a = RAM(SND_FADE_LEVEL);
    y = RAM(SND_FADE_SPEED);
    if (!(y & 0x80)) {
        mff_cycles(0x8731, 2);
        a ^= 0xFF;
    } else {
        mff_cycles(0x872F, 1);
    }
    mff_cycles(0x8733, 4);
    if (a != 0xFF) { mff_cycles(0x8735, 1); goto L_8740; }
L_8737:;
    mff_cycles(0x8737, 8);
    a = x;
    a &= 0x03;
    m = 0x01;
    c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x873C, 1); goto L_8760; }
    mff_cycles(0x873E, 2);
    if (z) { mff_cycles(0x873E, 1); goto L_8752; }
L_8740:;
    mff_cycles(0x8740, 4);
    m = 0x29;
    c = x >= m;
    if (x != m) { mff_cycles(0x8742, 1); goto L_875B; }
    mff_cycles(0x8744, 16);
    RAM(SND_TMP + 3) = a;
    a = RAM(SND_M_SLIDE + x);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_TMP) = a;
    SYNC_OUT;
    if (!mff_call_at(0x074D, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x874E, 5);
    a = RAM(SND_TMP);
    if (a == 0) { mff_cycles(0x8750, 1); goto L_87AA; }
L_8752:;
    mff_cycles(0x8752, 6);
    a = RAM(SND_LEVEL + x);
    if (a == 0) { mff_cycles(0x8755, 1); goto L_87AA; }
    mff_cycles(0x8757, 4);
    a = 0xFF;
    if (a != 0) { mff_cycles(0x8759, 1); goto L_87AA; }
L_875B:;
    mff_cycles(0x875B, 6);
    if (a < RAM(SND_LEVEL + x)) { mff_cycles(0x875E, 1); goto L_8763; }
L_8760:;
    mff_cycles(0x8760, 4);
    a = RAM(SND_LEVEL + x);
L_8763:;
    mff_cycles(0x8763, 20);
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    a ^= 0x0F;
    RAM(SND_TMP + 2) = a;
    y = 0x06;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x876F, 4);
    m = 0x05;
    if (a < m) { mff_cycles(0x8771, 1); goto L_8797; }
    mff_cycles(0x8773, 19);
    RAM(SND_TMP + 3) = a;
    y = RAM(SND_LFO + x);
    a = RAM(SND_ENV + x);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = y;
    n = (a) >> 7;
    z = (a) == 0;
    if (c) {
        mff_cycles(0x8780, 2);
        a ^= 0xFF;
        n = (a) >> 7;
        z = (a) == 0;
    } else {
        mff_cycles(0x877E, 1);
    }
    mff_cycles(0x8782, 2);
    if (z) { mff_cycles(0x8782, 1); goto L_8797; }
    mff_cycles(0x8784, 9);
    RAM(SND_TMP) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0788, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x8789, 11);
    a = RAM(SND_TMP);
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    m = 0x10;
    c = a >= m;
    if (c) { mff_cycles(0x878F, 1); goto L_87A5; }
    mff_cycles(0x8791, 5);
    if (a >= RAM(SND_TMP + 2)) {
        mff_cycles(0x8795, 3);
        RAM(SND_TMP + 2) = a;
    } else {
        mff_cycles(0x8793, 1);
    }
L_8797:;
    mff_cycles(0x8797, 19);
    a = 0x10;
    RAM(SND_TMP + 3) = a;
    a = RAM(SND_VOLUME + x);
    c = 1;
    m = RAM(SND_TMP + 2);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    m = RAM(SND_TMP + 3);
    v = (m >> 6) & 1;
    if ((a & m) != 0) { mff_cycles(0x87A3, 1); goto L_87AA; }
L_87A5:;
    mff_cycles(0x87A5, 6);
    a = RAM(SND_VOLUME + x);
    a &= 0xF0;
L_87AA:;
    mff_cycles(0x87AA, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x07AE, 0x80EC, 3)) return;
    SYNC_IN;
    mff_cycles(0x87AF, 12);
    a = x;
    a &= 0x03;
    y = a;
    a = RAM(SND_PERIOD_HI_CACHE + y);
    if (a & 0x80) { mff_cycles(0x87B6, 2); goto L_880C; }
    mff_cycles(0x87B8, 7);
    y = 0x05;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x87BC, 2);
    if (a == 0) { mff_cycles(0x87BC, 2); goto L_880C; }
    mff_cycles(0x87BE, 19);
    RAM(SND_TMP + 3) = a;
    y = RAM(SND_LFO + x);
    a = RAM(SND_ENV + x);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = y;
    n = (a) >> 7;
    z = (a) == 0;
    if (c) {
        mff_cycles(0x87CB, 2);
        a ^= 0xFF;
        n = (a) >> 7;
        z = (a) == 0;
    } else {
        mff_cycles(0x87C9, 1);
    }
    mff_cycles(0x87CD, 2);
    if (z) { mff_cycles(0x87CD, 2); goto L_880C; }
    mff_cycles(0x87CF, 9);
    RAM(SND_TMP) = a;
    SYNC_OUT;
    if (!mff_call_at(0x07D3, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x87D4, 38);
    a = RAM(SND_TMP);
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    t = RAM(SND_TMP + 1);
    t = (uint8_t)(t >> 1 | c << 7);
    RAM(SND_TMP + 1) = t;
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    t = RAM(SND_TMP + 1);
    t = (uint8_t)(t >> 1 | c << 7);
    RAM(SND_TMP + 1) = t;
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    t = RAM(SND_TMP + 1);
    t = (uint8_t)(t >> 1 | c << 7);
    RAM(SND_TMP + 1) = t;
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    t = RAM(SND_TMP + 1);
    t = (uint8_t)(t >> 1 | c << 7);
    RAM(SND_TMP + 1) = t;
    y = a;
    a |= RAM(SND_TMP + 1);
    if (a == 0) { mff_cycles(0x87E5, 2); goto L_880C; }
    mff_cycles(0x87E7, 6);
    a = RAM(SND_ENV + x);
    if (a & 0x80) { mff_cycles(0x87EA, 1); goto L_87FA; }
    mff_cycles(0x87EC, 20);
    c = 0;
    a = RAM(SND_TMP + 1);
    m = RAM(SND_PITCH_LO + x);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(SND_TMP + 1) = a;
    a = y;
    m = RAM(SND_PITCH_HI + x);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    if (a != 0) { mff_cycles(0x87F8, 2); goto L_8809; }
L_87FA:;
    mff_cycles(0x87FA, 22);
    c = 1;
    a = RAM(SND_PITCH_LO + x);
    m = RAM(SND_TMP + 1);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(SND_TMP + 1) = a;
    a = RAM(SND_PITCH_HI + x);
    RAM(SND_TMP) = y;
    m = RAM(SND_TMP);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
L_8809:;
    mff_cycles(0x8809, 4);
    y = a;
    if (y != 0) { mff_cycles(0x880A, 1); goto L_8814; }
L_880C:;
    mff_cycles(0x880C, 11);
    a = RAM(SND_PITCH_LO + x);
    RAM(SND_TMP + 1) = a;
    y = RAM(SND_PITCH_HI + x);
L_8814:;
    mff_cycles(0x8814, 4);
    m = 0x28;
    c = x >= m;
    if (!c) {
        mff_cycles(0x8818, 5);
        a = RAM(SND_EFFECT_LOOP);
        if (a & 0x80) {
            mff_cycles(0x881C, 5);
            a = RAM(SND_EFFECT_PITCH_SCALE);
            if (a != 0) {
                mff_cycles(0x8820, 18);
                RAM(SND_TMP + 3) = a;
                RAM(SND_TMP) = y;
                a = RAM(SND_TMP + 1);
                n = (a) >> 7;
                z = (a) == 0;
                mff_push(a);
                SYNC_OUT;
                if (!mff_call_at(0x0829, 0x8006, 3)) return;
                SYNC_IN;
                mff_cycles(0x882A, 19);
                a = mff_pull();
                c = 0;
                m = RAM(SND_TMP + 1);
                w = a + m + c;
                c = w > 0xFF;
                a = (uint8_t)w;
                RAM(SND_TMP + 1) = a;
                a = 0x00;
                m = RAM(SND_TMP);
                w = a + m + c;
                v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
                c = w > 0xFF;
                a = (uint8_t)w;
                y = a;
            } else {
                mff_cycles(0x881E, 1);
            }
        } else {
            mff_cycles(0x881A, 1);
        }
    } else {
        mff_cycles(0x8816, 1);
    }
    mff_cycles(0x8835, 6);
    a = x;
    a &= 0x03;
    if (a != 0) { mff_cycles(0x8838, 1); goto L_8849; }
    mff_cycles(0x883A, 11);
    a = y;
    a &= 0x0F;
    y = 0x07;
    a |= mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x8841, 10);
    RAM(SND_TMP + 1) = a;
    a = 0x00;
    RAM(SND_TMP) = a;
    if (a == 0) { mff_cycles(0x8847, 1); goto L_8884; }
L_8849:;
    mff_cycles(0x8849, 4);
    a = y;
    y = 0x08;
    for (;;) {
        mff_cycles(0x884C, 6);
        y--;
        mff_cycles(0x8850, 2);
        if (a >= prg_read((uint16_t)(0x8953 + y))) break;
        mff_cycles(0x884C, 1);
    }
    mff_cycles(0x8852, 29);
    RAM(SND_TMP) = a;
    a = y;
    c = 0;
    m = RAM(SND_TMP);
    w = a + m + c;
    a = (uint8_t)w;
    y = a;
    a &= 0x07;
    c = 0;
    m = 0x07;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(SND_TMP) = a;
    a = y;
    a &= 0x38;
    a ^= 0x38;
    if (a != 0) {
        for (;;) {
            mff_cycles(0x8867, 16);
            t = RAM(SND_TMP);
            c = t & 1;
            t >>= 1;
            RAM(SND_TMP) = t;
            t = RAM(SND_TMP + 1);
            t = (uint8_t)(t >> 1 | c << 7);
            RAM(SND_TMP + 1) = t;
            c = 1;
            m = 0x08;
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            c = w >= 0;
            a = (uint8_t)w;
            if (a == 0) break;
            mff_cycles(0x8867, 1);
        }
    } else {
        mff_cycles(0x8865, 1);
    }
    mff_cycles(0x8870, 8);
    y = 0x00;
    a = RAM(SND_DETUNE + x);
    n = (a) >> 7;
    if (a != 0) {
        mff_cycles(0x8877, 2);
        if (n) {
            mff_cycles(0x8879, 2);
            y--;
        } else {
            mff_cycles(0x8877, 1);
        }
        mff_cycles(0x887A, 16);
        c = 0;
        m = RAM(SND_TMP + 1);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(SND_TMP + 1) = a;
        a = y;
        m = RAM(SND_TMP);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(SND_TMP) = a;
    } else {
        mff_cycles(0x8875, 1);
    }
L_8884:;
    mff_cycles(0x8884, 11);
    y = 0x02;
    a = RAM(SND_TMP + 1);
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x088A, 0x80EC, 3)) return;
    SYNC_IN;
    mff_cycles(0x888B, 15);
    a = x;
    a &= 0x03;
    y = a;
    a = RAM(SND_TMP);
    m = RAM(SND_PERIOD_HI_CACHE + y);
    c = a >= m;
    if (a != m) {
        mff_cycles(0x8896, 15);
        RAM(SND_PERIOD_HI_CACHE + y) = a;
        a |= 0x08;
        y = 0x03;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x089F, 0x80EC, 3)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x8894, 1);
    }
    mff_cycles(0x88A0, 8);
    a = RAM(SND_ENV + x);
    a &= 0x20;
    if (a == 0) { mff_cycles(0x88A5, 1); goto L_88FA; }
    mff_cycles(0x88A7, 6);
    a = RAM(SND_SLIDE_AMOUNT + x);
    if (a == 0) { mff_cycles(0x88AA, 1); goto L_88F2; }
    mff_cycles(0x88AC, 9);
    y = 0x00;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    n = (t) >> 7;
    z = (t) == 0;
    FLAGS_OUT;
    mff_push(mff_p());
    if (c) {
        mff_cycles(0x88B2, 8);
        a ^= 0xFF;
        c = 0;
        m = 0x01;
        w = a + m + c;
        a = (uint8_t)w;
        y--;
    } else {
        mff_cycles(0x88B0, 1);
    }
    mff_cycles(0x88B8, 40);
    c = 0;
    m = RAM(SND_PITCH_LO + x);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(SND_PITCH_LO + x) = a;
    a = y;
    m = RAM(SND_PITCH_HI + x);
    w = a + m + c;
    a = (uint8_t)w;
    RAM(SND_PITCH_HI + x) = a;
    a = RAM(SND_TARGET + x);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    y = a;
    c = 1;
    a = RAM(SND_PITCH_LO + x);
    m = prg_read((uint16_t)(0x8959 + y));
    w = a - m - !c;
    c = w >= 0;
    mff_cycles(0x88D2, 10);
    a = RAM(SND_PITCH_HI + x);
    a &= 0x3F;
    m = prg_read((uint16_t)(0x895A + y));
    w = a - m - !c;
    c = w >= 0;
    mff_cycles(0x88DA, 12);
    a = 0xFF;
    m = 0x00;
    w = a + m + c;
    a = (uint8_t)w;
    mff_set_p(mff_pull());
    FLAGS_IN;
    m = 0x00;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    if (a != 0) { mff_cycles(0x88E1, 1); goto L_88FA; }
    mff_cycles(0x88E3, 4);
    a = x;
    if (a == 0) { mff_cycles(0x88E4, 1); goto L_88FA; }
    mff_cycles(0x88E6, 4);
    a = prg_read((uint16_t)(0x8959 + y));
    mff_cycles(0x88E9, 9);
    RAM(SND_PITCH_LO + x) = a;
    a = prg_read((uint16_t)(0x895A + y));
    mff_cycles(0x88EF, 5);
    RAM(SND_PITCH_HI + x) = a;
L_88F2:;
    mff_cycles(0x88F2, 11);
    a = RAM(SND_ENV + x);
    a &= 0xDF;
    RAM(SND_ENV + x) = a;
L_88FA:;
    mff_cycles(0x88FA, 7);
    y = 0x04;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x88FE, 4);
    a &= 0x7F;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0x8902, 13);
        c = 0;
        m = RAM(SND_LFO + x);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(SND_LFO + x) = a;
        if (c) {
            mff_cycles(0x890B, 13);
            a = RAM(SND_ENV + x);
            c = 0;
            m = 0x40;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(SND_ENV + x) = a;
        } else {
            mff_cycles(0x8909, 1);
        }
    } else {
        mff_cycles(0x8900, 1);
    }
    mff_cycles(0x8914, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/* snd_env_release $8702 (bank 3) - lifted by port/tools/lift.py */
void func_8702_b3(void) {
    REGS_IN;
    mff_cycles(0x8702, 8);
    a = x;
    a &= 0x03;
    if (a == 0x01) { mff_cycles(0x8707, 1); goto L_8718; }
    mff_cycles(0x8709, 7);
    y = 0x03;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x870D, 2);
    if (a == 0) { mff_cycles(0x870D, 1); goto L_8720; }
    mff_cycles(0x870F, 11);
    y = a;
    a = RAM(SND_TMP + 3);
    c = 1;
    m = prg_read((uint16_t)(0x8933 + y));
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    mff_cycles(0x8716, 2);
    if (w >= 0) { mff_cycles(0x8716, 1); goto L_871D; }
L_8718:;
    mff_cycles(0x8718, 9);
    a = 0x00;
    t = (uint8_t)(RAM(SND_ENV + x) + 1);
    RAM(SND_ENV + x) = t;
L_871D:;
    mff_cycles(0x871D, 5);
    RAM(SND_LEVEL + x) = a;
L_8720:;
    mff_cycles(0x8720, 4);
    m = 0x28;
    c = x >= m;
    if (!c) { mff_cycles(0x8722, 1); goto L_8737; }
    mff_cycles(0x8724, 5);
    a = RAM(SND_EFFECT_CHANNELS);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0x8728, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x88A0, 2); call_by_address_tail(0x88A0, 3); return;
    } else {
        mff_cycles(0x8726, 1);
    }
    mff_cycles(0x872B, 8);
    a = RAM(SND_FADE_LEVEL);
    y = RAM(SND_FADE_SPEED);
    if (!(y & 0x80)) {
        mff_cycles(0x8731, 2);
        a ^= 0xFF;
    } else {
        mff_cycles(0x872F, 1);
    }
    mff_cycles(0x8733, 4);
    if (a != 0xFF) { mff_cycles(0x8735, 1); goto L_8740; }
L_8737:;
    mff_cycles(0x8737, 8);
    a = x;
    a &= 0x03;
    m = 0x01;
    c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x873C, 1); goto L_8760; }
    mff_cycles(0x873E, 2);
    if (z) { mff_cycles(0x873E, 1); goto L_8752; }
L_8740:;
    mff_cycles(0x8740, 4);
    m = 0x29;
    c = x >= m;
    if (x != m) { mff_cycles(0x8742, 1); goto L_875B; }
    mff_cycles(0x8744, 16);
    RAM(SND_TMP + 3) = a;
    a = RAM(SND_M_SLIDE + x);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_TMP) = a;
    SYNC_OUT;
    if (!mff_call_at(0x074D, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x874E, 5);
    a = RAM(SND_TMP);
    if (a == 0) { mff_cycles(0x8750, 1); goto L_87AA; }
L_8752:;
    mff_cycles(0x8752, 6);
    a = RAM(SND_LEVEL + x);
    if (a == 0) { mff_cycles(0x8755, 1); goto L_87AA; }
    mff_cycles(0x8757, 4);
    a = 0xFF;
    if (a != 0) { mff_cycles(0x8759, 1); goto L_87AA; }
L_875B:;
    mff_cycles(0x875B, 6);
    if (a < RAM(SND_LEVEL + x)) { mff_cycles(0x875E, 1); goto L_8763; }
L_8760:;
    mff_cycles(0x8760, 4);
    a = RAM(SND_LEVEL + x);
L_8763:;
    mff_cycles(0x8763, 20);
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    a ^= 0x0F;
    RAM(SND_TMP + 2) = a;
    y = 0x06;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x876F, 4);
    m = 0x05;
    if (a < m) { mff_cycles(0x8771, 1); goto L_8797; }
    mff_cycles(0x8773, 19);
    RAM(SND_TMP + 3) = a;
    y = RAM(SND_LFO + x);
    a = RAM(SND_ENV + x);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = y;
    n = (a) >> 7;
    z = (a) == 0;
    if (c) {
        mff_cycles(0x8780, 2);
        a ^= 0xFF;
        n = (a) >> 7;
        z = (a) == 0;
    } else {
        mff_cycles(0x877E, 1);
    }
    mff_cycles(0x8782, 2);
    if (z) { mff_cycles(0x8782, 1); goto L_8797; }
    mff_cycles(0x8784, 9);
    RAM(SND_TMP) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0788, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x8789, 11);
    a = RAM(SND_TMP);
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    m = 0x10;
    c = a >= m;
    if (c) { mff_cycles(0x878F, 1); goto L_87A5; }
    mff_cycles(0x8791, 5);
    if (a >= RAM(SND_TMP + 2)) {
        mff_cycles(0x8795, 3);
        RAM(SND_TMP + 2) = a;
    } else {
        mff_cycles(0x8793, 1);
    }
L_8797:;
    mff_cycles(0x8797, 19);
    a = 0x10;
    RAM(SND_TMP + 3) = a;
    a = RAM(SND_VOLUME + x);
    c = 1;
    m = RAM(SND_TMP + 2);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    m = RAM(SND_TMP + 3);
    v = (m >> 6) & 1;
    if ((a & m) != 0) { mff_cycles(0x87A3, 1); goto L_87AA; }
L_87A5:;
    mff_cycles(0x87A5, 6);
    a = RAM(SND_VOLUME + x);
    a &= 0xF0;
L_87AA:;
    mff_cycles(0x87AA, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x07AE, 0x80EC, 3)) return;
    SYNC_IN;
    mff_cycles(0x87AF, 12);
    a = x;
    a &= 0x03;
    y = a;
    a = RAM(SND_PERIOD_HI_CACHE + y);
    if (a & 0x80) { mff_cycles(0x87B6, 2); goto L_880C; }
    mff_cycles(0x87B8, 7);
    y = 0x05;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x87BC, 2);
    if (a == 0) { mff_cycles(0x87BC, 2); goto L_880C; }
    mff_cycles(0x87BE, 19);
    RAM(SND_TMP + 3) = a;
    y = RAM(SND_LFO + x);
    a = RAM(SND_ENV + x);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = y;
    n = (a) >> 7;
    z = (a) == 0;
    if (c) {
        mff_cycles(0x87CB, 2);
        a ^= 0xFF;
        n = (a) >> 7;
        z = (a) == 0;
    } else {
        mff_cycles(0x87C9, 1);
    }
    mff_cycles(0x87CD, 2);
    if (z) { mff_cycles(0x87CD, 2); goto L_880C; }
    mff_cycles(0x87CF, 9);
    RAM(SND_TMP) = a;
    SYNC_OUT;
    if (!mff_call_at(0x07D3, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x87D4, 38);
    a = RAM(SND_TMP);
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    t = RAM(SND_TMP + 1);
    t = (uint8_t)(t >> 1 | c << 7);
    RAM(SND_TMP + 1) = t;
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    t = RAM(SND_TMP + 1);
    t = (uint8_t)(t >> 1 | c << 7);
    RAM(SND_TMP + 1) = t;
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    t = RAM(SND_TMP + 1);
    t = (uint8_t)(t >> 1 | c << 7);
    RAM(SND_TMP + 1) = t;
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    t = RAM(SND_TMP + 1);
    t = (uint8_t)(t >> 1 | c << 7);
    RAM(SND_TMP + 1) = t;
    y = a;
    a |= RAM(SND_TMP + 1);
    if (a == 0) { mff_cycles(0x87E5, 2); goto L_880C; }
    mff_cycles(0x87E7, 6);
    a = RAM(SND_ENV + x);
    if (a & 0x80) { mff_cycles(0x87EA, 1); goto L_87FA; }
    mff_cycles(0x87EC, 20);
    c = 0;
    a = RAM(SND_TMP + 1);
    m = RAM(SND_PITCH_LO + x);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(SND_TMP + 1) = a;
    a = y;
    m = RAM(SND_PITCH_HI + x);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    if (a != 0) { mff_cycles(0x87F8, 2); goto L_8809; }
L_87FA:;
    mff_cycles(0x87FA, 22);
    c = 1;
    a = RAM(SND_PITCH_LO + x);
    m = RAM(SND_TMP + 1);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(SND_TMP + 1) = a;
    a = RAM(SND_PITCH_HI + x);
    RAM(SND_TMP) = y;
    m = RAM(SND_TMP);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
L_8809:;
    mff_cycles(0x8809, 4);
    y = a;
    if (y != 0) { mff_cycles(0x880A, 1); goto L_8814; }
L_880C:;
    mff_cycles(0x880C, 11);
    a = RAM(SND_PITCH_LO + x);
    RAM(SND_TMP + 1) = a;
    y = RAM(SND_PITCH_HI + x);
L_8814:;
    mff_cycles(0x8814, 4);
    m = 0x28;
    c = x >= m;
    if (!c) {
        mff_cycles(0x8818, 5);
        a = RAM(SND_EFFECT_LOOP);
        if (a & 0x80) {
            mff_cycles(0x881C, 5);
            a = RAM(SND_EFFECT_PITCH_SCALE);
            if (a != 0) {
                mff_cycles(0x8820, 18);
                RAM(SND_TMP + 3) = a;
                RAM(SND_TMP) = y;
                a = RAM(SND_TMP + 1);
                n = (a) >> 7;
                z = (a) == 0;
                mff_push(a);
                SYNC_OUT;
                if (!mff_call_at(0x0829, 0x8006, 3)) return;
                SYNC_IN;
                mff_cycles(0x882A, 19);
                a = mff_pull();
                c = 0;
                m = RAM(SND_TMP + 1);
                w = a + m + c;
                c = w > 0xFF;
                a = (uint8_t)w;
                RAM(SND_TMP + 1) = a;
                a = 0x00;
                m = RAM(SND_TMP);
                w = a + m + c;
                v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
                c = w > 0xFF;
                a = (uint8_t)w;
                y = a;
            } else {
                mff_cycles(0x881E, 1);
            }
        } else {
            mff_cycles(0x881A, 1);
        }
    } else {
        mff_cycles(0x8816, 1);
    }
    mff_cycles(0x8835, 6);
    a = x;
    a &= 0x03;
    if (a != 0) { mff_cycles(0x8838, 1); goto L_8849; }
    mff_cycles(0x883A, 11);
    a = y;
    a &= 0x0F;
    y = 0x07;
    a |= mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x8841, 10);
    RAM(SND_TMP + 1) = a;
    a = 0x00;
    RAM(SND_TMP) = a;
    if (a == 0) { mff_cycles(0x8847, 1); goto L_8884; }
L_8849:;
    mff_cycles(0x8849, 4);
    a = y;
    y = 0x08;
    for (;;) {
        mff_cycles(0x884C, 6);
        y--;
        mff_cycles(0x8850, 2);
        if (a >= prg_read((uint16_t)(0x8953 + y))) break;
        mff_cycles(0x884C, 1);
    }
    mff_cycles(0x8852, 29);
    RAM(SND_TMP) = a;
    a = y;
    c = 0;
    m = RAM(SND_TMP);
    w = a + m + c;
    a = (uint8_t)w;
    y = a;
    a &= 0x07;
    c = 0;
    m = 0x07;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(SND_TMP) = a;
    a = y;
    a &= 0x38;
    a ^= 0x38;
    if (a != 0) {
        for (;;) {
            mff_cycles(0x8867, 16);
            t = RAM(SND_TMP);
            c = t & 1;
            t >>= 1;
            RAM(SND_TMP) = t;
            t = RAM(SND_TMP + 1);
            t = (uint8_t)(t >> 1 | c << 7);
            RAM(SND_TMP + 1) = t;
            c = 1;
            m = 0x08;
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            c = w >= 0;
            a = (uint8_t)w;
            if (a == 0) break;
            mff_cycles(0x8867, 1);
        }
    } else {
        mff_cycles(0x8865, 1);
    }
    mff_cycles(0x8870, 8);
    y = 0x00;
    a = RAM(SND_DETUNE + x);
    n = (a) >> 7;
    if (a != 0) {
        mff_cycles(0x8877, 2);
        if (n) {
            mff_cycles(0x8879, 2);
            y--;
        } else {
            mff_cycles(0x8877, 1);
        }
        mff_cycles(0x887A, 16);
        c = 0;
        m = RAM(SND_TMP + 1);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(SND_TMP + 1) = a;
        a = y;
        m = RAM(SND_TMP);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(SND_TMP) = a;
    } else {
        mff_cycles(0x8875, 1);
    }
L_8884:;
    mff_cycles(0x8884, 11);
    y = 0x02;
    a = RAM(SND_TMP + 1);
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x088A, 0x80EC, 3)) return;
    SYNC_IN;
    mff_cycles(0x888B, 15);
    a = x;
    a &= 0x03;
    y = a;
    a = RAM(SND_TMP);
    m = RAM(SND_PERIOD_HI_CACHE + y);
    c = a >= m;
    if (a != m) {
        mff_cycles(0x8896, 15);
        RAM(SND_PERIOD_HI_CACHE + y) = a;
        a |= 0x08;
        y = 0x03;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x089F, 0x80EC, 3)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x8894, 1);
    }
    mff_cycles(0x88A0, 8);
    a = RAM(SND_ENV + x);
    a &= 0x20;
    if (a == 0) { mff_cycles(0x88A5, 1); goto L_88FA; }
    mff_cycles(0x88A7, 6);
    a = RAM(SND_SLIDE_AMOUNT + x);
    if (a == 0) { mff_cycles(0x88AA, 1); goto L_88F2; }
    mff_cycles(0x88AC, 9);
    y = 0x00;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    n = (t) >> 7;
    z = (t) == 0;
    FLAGS_OUT;
    mff_push(mff_p());
    if (c) {
        mff_cycles(0x88B2, 8);
        a ^= 0xFF;
        c = 0;
        m = 0x01;
        w = a + m + c;
        a = (uint8_t)w;
        y--;
    } else {
        mff_cycles(0x88B0, 1);
    }
    mff_cycles(0x88B8, 40);
    c = 0;
    m = RAM(SND_PITCH_LO + x);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(SND_PITCH_LO + x) = a;
    a = y;
    m = RAM(SND_PITCH_HI + x);
    w = a + m + c;
    a = (uint8_t)w;
    RAM(SND_PITCH_HI + x) = a;
    a = RAM(SND_TARGET + x);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    y = a;
    c = 1;
    a = RAM(SND_PITCH_LO + x);
    m = prg_read((uint16_t)(0x8959 + y));
    w = a - m - !c;
    c = w >= 0;
    mff_cycles(0x88D2, 10);
    a = RAM(SND_PITCH_HI + x);
    a &= 0x3F;
    m = prg_read((uint16_t)(0x895A + y));
    w = a - m - !c;
    c = w >= 0;
    mff_cycles(0x88DA, 12);
    a = 0xFF;
    m = 0x00;
    w = a + m + c;
    a = (uint8_t)w;
    mff_set_p(mff_pull());
    FLAGS_IN;
    m = 0x00;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    if (a != 0) { mff_cycles(0x88E1, 1); goto L_88FA; }
    mff_cycles(0x88E3, 4);
    a = x;
    if (a == 0) { mff_cycles(0x88E4, 1); goto L_88FA; }
    mff_cycles(0x88E6, 4);
    a = prg_read((uint16_t)(0x8959 + y));
    mff_cycles(0x88E9, 9);
    RAM(SND_PITCH_LO + x) = a;
    a = prg_read((uint16_t)(0x895A + y));
    mff_cycles(0x88EF, 5);
    RAM(SND_PITCH_HI + x) = a;
L_88F2:;
    mff_cycles(0x88F2, 11);
    a = RAM(SND_ENV + x);
    a &= 0xDF;
    RAM(SND_ENV + x) = a;
L_88FA:;
    mff_cycles(0x88FA, 7);
    y = 0x04;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x88FE, 4);
    a &= 0x7F;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0x8902, 13);
        c = 0;
        m = RAM(SND_LFO + x);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(SND_LFO + x) = a;
        if (c) {
            mff_cycles(0x890B, 13);
            a = RAM(SND_ENV + x);
            c = 0;
            m = 0x40;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(SND_ENV + x) = a;
        } else {
            mff_cycles(0x8909, 1);
        }
    } else {
        mff_cycles(0x8900, 1);
    }
    mff_cycles(0x8914, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $871A (bank 3) - lifted by port/tools/lift.py */
void func_871A_b3(void) {
    REGS_IN;
    mff_cycles(0x871A, 16);
    t = (uint8_t)(RAM(SND_ENV + x) + 1);
    RAM(SND_ENV + x) = t;
    RAM(SND_LEVEL + x) = a;
    m = 0x28;
    c = x >= m;
    if (!c) { mff_cycles(0x8722, 1); goto L_8737; }
    mff_cycles(0x8724, 5);
    a = RAM(SND_EFFECT_CHANNELS);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0x8728, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x88A0, 2); call_by_address_tail(0x88A0, 3); return;
    } else {
        mff_cycles(0x8726, 1);
    }
    mff_cycles(0x872B, 8);
    a = RAM(SND_FADE_LEVEL);
    y = RAM(SND_FADE_SPEED);
    if (!(y & 0x80)) {
        mff_cycles(0x8731, 2);
        a ^= 0xFF;
    } else {
        mff_cycles(0x872F, 1);
    }
    mff_cycles(0x8733, 4);
    if (a != 0xFF) { mff_cycles(0x8735, 1); goto L_8740; }
L_8737:;
    mff_cycles(0x8737, 8);
    a = x;
    a &= 0x03;
    m = 0x01;
    c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x873C, 1); goto L_8760; }
    mff_cycles(0x873E, 2);
    if (z) { mff_cycles(0x873E, 1); goto L_8752; }
L_8740:;
    mff_cycles(0x8740, 4);
    m = 0x29;
    c = x >= m;
    if (x != m) { mff_cycles(0x8742, 1); goto L_875B; }
    mff_cycles(0x8744, 16);
    RAM(SND_TMP + 3) = a;
    a = RAM(SND_M_SLIDE + x);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_TMP) = a;
    SYNC_OUT;
    if (!mff_call_at(0x074D, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x874E, 5);
    a = RAM(SND_TMP);
    if (a == 0) { mff_cycles(0x8750, 1); goto L_87AA; }
L_8752:;
    mff_cycles(0x8752, 6);
    a = RAM(SND_LEVEL + x);
    if (a == 0) { mff_cycles(0x8755, 1); goto L_87AA; }
    mff_cycles(0x8757, 4);
    a = 0xFF;
    if (a != 0) { mff_cycles(0x8759, 1); goto L_87AA; }
L_875B:;
    mff_cycles(0x875B, 6);
    if (a < RAM(SND_LEVEL + x)) { mff_cycles(0x875E, 1); goto L_8763; }
L_8760:;
    mff_cycles(0x8760, 4);
    a = RAM(SND_LEVEL + x);
L_8763:;
    mff_cycles(0x8763, 20);
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    a ^= 0x0F;
    RAM(SND_TMP + 2) = a;
    y = 0x06;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x876F, 4);
    m = 0x05;
    if (a < m) { mff_cycles(0x8771, 1); goto L_8797; }
    mff_cycles(0x8773, 19);
    RAM(SND_TMP + 3) = a;
    y = RAM(SND_LFO + x);
    a = RAM(SND_ENV + x);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = y;
    n = (a) >> 7;
    z = (a) == 0;
    if (c) {
        mff_cycles(0x8780, 2);
        a ^= 0xFF;
        n = (a) >> 7;
        z = (a) == 0;
    } else {
        mff_cycles(0x877E, 1);
    }
    mff_cycles(0x8782, 2);
    if (z) { mff_cycles(0x8782, 1); goto L_8797; }
    mff_cycles(0x8784, 9);
    RAM(SND_TMP) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0788, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x8789, 11);
    a = RAM(SND_TMP);
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    m = 0x10;
    c = a >= m;
    if (c) { mff_cycles(0x878F, 1); goto L_87A5; }
    mff_cycles(0x8791, 5);
    if (a >= RAM(SND_TMP + 2)) {
        mff_cycles(0x8795, 3);
        RAM(SND_TMP + 2) = a;
    } else {
        mff_cycles(0x8793, 1);
    }
L_8797:;
    mff_cycles(0x8797, 19);
    a = 0x10;
    RAM(SND_TMP + 3) = a;
    a = RAM(SND_VOLUME + x);
    c = 1;
    m = RAM(SND_TMP + 2);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    m = RAM(SND_TMP + 3);
    v = (m >> 6) & 1;
    if ((a & m) != 0) { mff_cycles(0x87A3, 1); goto L_87AA; }
L_87A5:;
    mff_cycles(0x87A5, 6);
    a = RAM(SND_VOLUME + x);
    a &= 0xF0;
L_87AA:;
    mff_cycles(0x87AA, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x07AE, 0x80EC, 3)) return;
    SYNC_IN;
    mff_cycles(0x87AF, 12);
    a = x;
    a &= 0x03;
    y = a;
    a = RAM(SND_PERIOD_HI_CACHE + y);
    if (a & 0x80) { mff_cycles(0x87B6, 2); goto L_880C; }
    mff_cycles(0x87B8, 7);
    y = 0x05;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x87BC, 2);
    if (a == 0) { mff_cycles(0x87BC, 2); goto L_880C; }
    mff_cycles(0x87BE, 19);
    RAM(SND_TMP + 3) = a;
    y = RAM(SND_LFO + x);
    a = RAM(SND_ENV + x);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = y;
    n = (a) >> 7;
    z = (a) == 0;
    if (c) {
        mff_cycles(0x87CB, 2);
        a ^= 0xFF;
        n = (a) >> 7;
        z = (a) == 0;
    } else {
        mff_cycles(0x87C9, 1);
    }
    mff_cycles(0x87CD, 2);
    if (z) { mff_cycles(0x87CD, 2); goto L_880C; }
    mff_cycles(0x87CF, 9);
    RAM(SND_TMP) = a;
    SYNC_OUT;
    if (!mff_call_at(0x07D3, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x87D4, 38);
    a = RAM(SND_TMP);
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    t = RAM(SND_TMP + 1);
    t = (uint8_t)(t >> 1 | c << 7);
    RAM(SND_TMP + 1) = t;
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    t = RAM(SND_TMP + 1);
    t = (uint8_t)(t >> 1 | c << 7);
    RAM(SND_TMP + 1) = t;
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    t = RAM(SND_TMP + 1);
    t = (uint8_t)(t >> 1 | c << 7);
    RAM(SND_TMP + 1) = t;
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    t = RAM(SND_TMP + 1);
    t = (uint8_t)(t >> 1 | c << 7);
    RAM(SND_TMP + 1) = t;
    y = a;
    a |= RAM(SND_TMP + 1);
    if (a == 0) { mff_cycles(0x87E5, 2); goto L_880C; }
    mff_cycles(0x87E7, 6);
    a = RAM(SND_ENV + x);
    if (a & 0x80) { mff_cycles(0x87EA, 1); goto L_87FA; }
    mff_cycles(0x87EC, 20);
    c = 0;
    a = RAM(SND_TMP + 1);
    m = RAM(SND_PITCH_LO + x);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(SND_TMP + 1) = a;
    a = y;
    m = RAM(SND_PITCH_HI + x);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    if (a != 0) { mff_cycles(0x87F8, 2); goto L_8809; }
L_87FA:;
    mff_cycles(0x87FA, 22);
    c = 1;
    a = RAM(SND_PITCH_LO + x);
    m = RAM(SND_TMP + 1);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(SND_TMP + 1) = a;
    a = RAM(SND_PITCH_HI + x);
    RAM(SND_TMP) = y;
    m = RAM(SND_TMP);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
L_8809:;
    mff_cycles(0x8809, 4);
    y = a;
    if (y != 0) { mff_cycles(0x880A, 1); goto L_8814; }
L_880C:;
    mff_cycles(0x880C, 11);
    a = RAM(SND_PITCH_LO + x);
    RAM(SND_TMP + 1) = a;
    y = RAM(SND_PITCH_HI + x);
L_8814:;
    mff_cycles(0x8814, 4);
    m = 0x28;
    c = x >= m;
    if (!c) {
        mff_cycles(0x8818, 5);
        a = RAM(SND_EFFECT_LOOP);
        if (a & 0x80) {
            mff_cycles(0x881C, 5);
            a = RAM(SND_EFFECT_PITCH_SCALE);
            if (a != 0) {
                mff_cycles(0x8820, 18);
                RAM(SND_TMP + 3) = a;
                RAM(SND_TMP) = y;
                a = RAM(SND_TMP + 1);
                n = (a) >> 7;
                z = (a) == 0;
                mff_push(a);
                SYNC_OUT;
                if (!mff_call_at(0x0829, 0x8006, 3)) return;
                SYNC_IN;
                mff_cycles(0x882A, 19);
                a = mff_pull();
                c = 0;
                m = RAM(SND_TMP + 1);
                w = a + m + c;
                c = w > 0xFF;
                a = (uint8_t)w;
                RAM(SND_TMP + 1) = a;
                a = 0x00;
                m = RAM(SND_TMP);
                w = a + m + c;
                v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
                c = w > 0xFF;
                a = (uint8_t)w;
                y = a;
            } else {
                mff_cycles(0x881E, 1);
            }
        } else {
            mff_cycles(0x881A, 1);
        }
    } else {
        mff_cycles(0x8816, 1);
    }
    mff_cycles(0x8835, 6);
    a = x;
    a &= 0x03;
    if (a != 0) { mff_cycles(0x8838, 1); goto L_8849; }
    mff_cycles(0x883A, 11);
    a = y;
    a &= 0x0F;
    y = 0x07;
    a |= mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x8841, 10);
    RAM(SND_TMP + 1) = a;
    a = 0x00;
    RAM(SND_TMP) = a;
    if (a == 0) { mff_cycles(0x8847, 1); goto L_8884; }
L_8849:;
    mff_cycles(0x8849, 4);
    a = y;
    y = 0x08;
    for (;;) {
        mff_cycles(0x884C, 6);
        y--;
        mff_cycles(0x8850, 2);
        if (a >= prg_read((uint16_t)(0x8953 + y))) break;
        mff_cycles(0x884C, 1);
    }
    mff_cycles(0x8852, 29);
    RAM(SND_TMP) = a;
    a = y;
    c = 0;
    m = RAM(SND_TMP);
    w = a + m + c;
    a = (uint8_t)w;
    y = a;
    a &= 0x07;
    c = 0;
    m = 0x07;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(SND_TMP) = a;
    a = y;
    a &= 0x38;
    a ^= 0x38;
    if (a != 0) {
        for (;;) {
            mff_cycles(0x8867, 16);
            t = RAM(SND_TMP);
            c = t & 1;
            t >>= 1;
            RAM(SND_TMP) = t;
            t = RAM(SND_TMP + 1);
            t = (uint8_t)(t >> 1 | c << 7);
            RAM(SND_TMP + 1) = t;
            c = 1;
            m = 0x08;
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            c = w >= 0;
            a = (uint8_t)w;
            if (a == 0) break;
            mff_cycles(0x8867, 1);
        }
    } else {
        mff_cycles(0x8865, 1);
    }
    mff_cycles(0x8870, 8);
    y = 0x00;
    a = RAM(SND_DETUNE + x);
    n = (a) >> 7;
    if (a != 0) {
        mff_cycles(0x8877, 2);
        if (n) {
            mff_cycles(0x8879, 2);
            y--;
        } else {
            mff_cycles(0x8877, 1);
        }
        mff_cycles(0x887A, 16);
        c = 0;
        m = RAM(SND_TMP + 1);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(SND_TMP + 1) = a;
        a = y;
        m = RAM(SND_TMP);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(SND_TMP) = a;
    } else {
        mff_cycles(0x8875, 1);
    }
L_8884:;
    mff_cycles(0x8884, 11);
    y = 0x02;
    a = RAM(SND_TMP + 1);
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x088A, 0x80EC, 3)) return;
    SYNC_IN;
    mff_cycles(0x888B, 15);
    a = x;
    a &= 0x03;
    y = a;
    a = RAM(SND_TMP);
    m = RAM(SND_PERIOD_HI_CACHE + y);
    c = a >= m;
    if (a != m) {
        mff_cycles(0x8896, 15);
        RAM(SND_PERIOD_HI_CACHE + y) = a;
        a |= 0x08;
        y = 0x03;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x089F, 0x80EC, 3)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x8894, 1);
    }
    mff_cycles(0x88A0, 8);
    a = RAM(SND_ENV + x);
    a &= 0x20;
    if (a == 0) { mff_cycles(0x88A5, 1); goto L_88FA; }
    mff_cycles(0x88A7, 6);
    a = RAM(SND_SLIDE_AMOUNT + x);
    if (a == 0) { mff_cycles(0x88AA, 1); goto L_88F2; }
    mff_cycles(0x88AC, 9);
    y = 0x00;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    n = (t) >> 7;
    z = (t) == 0;
    FLAGS_OUT;
    mff_push(mff_p());
    if (c) {
        mff_cycles(0x88B2, 8);
        a ^= 0xFF;
        c = 0;
        m = 0x01;
        w = a + m + c;
        a = (uint8_t)w;
        y--;
    } else {
        mff_cycles(0x88B0, 1);
    }
    mff_cycles(0x88B8, 40);
    c = 0;
    m = RAM(SND_PITCH_LO + x);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(SND_PITCH_LO + x) = a;
    a = y;
    m = RAM(SND_PITCH_HI + x);
    w = a + m + c;
    a = (uint8_t)w;
    RAM(SND_PITCH_HI + x) = a;
    a = RAM(SND_TARGET + x);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    y = a;
    c = 1;
    a = RAM(SND_PITCH_LO + x);
    m = prg_read((uint16_t)(0x8959 + y));
    w = a - m - !c;
    c = w >= 0;
    mff_cycles(0x88D2, 10);
    a = RAM(SND_PITCH_HI + x);
    a &= 0x3F;
    m = prg_read((uint16_t)(0x895A + y));
    w = a - m - !c;
    c = w >= 0;
    mff_cycles(0x88DA, 12);
    a = 0xFF;
    m = 0x00;
    w = a + m + c;
    a = (uint8_t)w;
    mff_set_p(mff_pull());
    FLAGS_IN;
    m = 0x00;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    if (a != 0) { mff_cycles(0x88E1, 1); goto L_88FA; }
    mff_cycles(0x88E3, 4);
    a = x;
    if (a == 0) { mff_cycles(0x88E4, 1); goto L_88FA; }
    mff_cycles(0x88E6, 4);
    a = prg_read((uint16_t)(0x8959 + y));
    mff_cycles(0x88E9, 9);
    RAM(SND_PITCH_LO + x) = a;
    a = prg_read((uint16_t)(0x895A + y));
    mff_cycles(0x88EF, 5);
    RAM(SND_PITCH_HI + x) = a;
L_88F2:;
    mff_cycles(0x88F2, 11);
    a = RAM(SND_ENV + x);
    a &= 0xDF;
    RAM(SND_ENV + x) = a;
L_88FA:;
    mff_cycles(0x88FA, 7);
    y = 0x04;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x88FE, 4);
    a &= 0x7F;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0x8902, 13);
        c = 0;
        m = RAM(SND_LFO + x);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(SND_LFO + x) = a;
        if (c) {
            mff_cycles(0x890B, 13);
            a = RAM(SND_ENV + x);
            c = 0;
            m = 0x40;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(SND_ENV + x) = a;
        } else {
            mff_cycles(0x8909, 1);
        }
    } else {
        mff_cycles(0x8900, 1);
    }
    mff_cycles(0x8914, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/* snd_output $8720 (bank 3) - lifted by port/tools/lift.py */
void func_8720_b3(void) {
    REGS_IN;
    mff_cycles(0x8720, 4);
    m = 0x28;
    c = x >= m;
    if (!c) { mff_cycles(0x8722, 1); goto L_8737; }
    mff_cycles(0x8724, 5);
    a = RAM(SND_EFFECT_CHANNELS);
    n = (a) >> 7;
    z = (a) == 0;
    if (n) {
        mff_cycles(0x8728, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x88A0, 2); call_by_address_tail(0x88A0, 3); return;
    } else {
        mff_cycles(0x8726, 1);
    }
    mff_cycles(0x872B, 8);
    a = RAM(SND_FADE_LEVEL);
    y = RAM(SND_FADE_SPEED);
    if (!(y & 0x80)) {
        mff_cycles(0x8731, 2);
        a ^= 0xFF;
    } else {
        mff_cycles(0x872F, 1);
    }
    mff_cycles(0x8733, 4);
    if (a != 0xFF) { mff_cycles(0x8735, 1); goto L_8740; }
L_8737:;
    mff_cycles(0x8737, 8);
    a = x;
    a &= 0x03;
    m = 0x01;
    c = a >= m;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) { mff_cycles(0x873C, 1); goto L_8760; }
    mff_cycles(0x873E, 2);
    if (z) { mff_cycles(0x873E, 1); goto L_8752; }
L_8740:;
    mff_cycles(0x8740, 4);
    m = 0x29;
    c = x >= m;
    if (x != m) { mff_cycles(0x8742, 1); goto L_875B; }
    mff_cycles(0x8744, 16);
    RAM(SND_TMP + 3) = a;
    a = RAM(SND_M_SLIDE + x);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_TMP) = a;
    SYNC_OUT;
    if (!mff_call_at(0x074D, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x874E, 5);
    a = RAM(SND_TMP);
    if (a == 0) { mff_cycles(0x8750, 1); goto L_87AA; }
L_8752:;
    mff_cycles(0x8752, 6);
    a = RAM(SND_LEVEL + x);
    if (a == 0) { mff_cycles(0x8755, 1); goto L_87AA; }
    mff_cycles(0x8757, 4);
    a = 0xFF;
    if (a != 0) { mff_cycles(0x8759, 1); goto L_87AA; }
L_875B:;
    mff_cycles(0x875B, 6);
    if (a < RAM(SND_LEVEL + x)) { mff_cycles(0x875E, 1); goto L_8763; }
L_8760:;
    mff_cycles(0x8760, 4);
    a = RAM(SND_LEVEL + x);
L_8763:;
    mff_cycles(0x8763, 20);
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    a ^= 0x0F;
    RAM(SND_TMP + 2) = a;
    y = 0x06;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x876F, 4);
    m = 0x05;
    if (a < m) { mff_cycles(0x8771, 1); goto L_8797; }
    mff_cycles(0x8773, 19);
    RAM(SND_TMP + 3) = a;
    y = RAM(SND_LFO + x);
    a = RAM(SND_ENV + x);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = y;
    n = (a) >> 7;
    z = (a) == 0;
    if (c) {
        mff_cycles(0x8780, 2);
        a ^= 0xFF;
        n = (a) >> 7;
        z = (a) == 0;
    } else {
        mff_cycles(0x877E, 1);
    }
    mff_cycles(0x8782, 2);
    if (z) { mff_cycles(0x8782, 1); goto L_8797; }
    mff_cycles(0x8784, 9);
    RAM(SND_TMP) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0788, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x8789, 11);
    a = RAM(SND_TMP);
    t = a;
    t >>= 1;
    a = t;
    t = a;
    t >>= 1;
    a = t;
    m = 0x10;
    c = a >= m;
    if (c) { mff_cycles(0x878F, 1); goto L_87A5; }
    mff_cycles(0x8791, 5);
    if (a >= RAM(SND_TMP + 2)) {
        mff_cycles(0x8795, 3);
        RAM(SND_TMP + 2) = a;
    } else {
        mff_cycles(0x8793, 1);
    }
L_8797:;
    mff_cycles(0x8797, 19);
    a = 0x10;
    RAM(SND_TMP + 3) = a;
    a = RAM(SND_VOLUME + x);
    c = 1;
    m = RAM(SND_TMP + 2);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    m = RAM(SND_TMP + 3);
    v = (m >> 6) & 1;
    if ((a & m) != 0) { mff_cycles(0x87A3, 1); goto L_87AA; }
L_87A5:;
    mff_cycles(0x87A5, 6);
    a = RAM(SND_VOLUME + x);
    a &= 0xF0;
L_87AA:;
    mff_cycles(0x87AA, 8);
    y = 0x00;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x07AE, 0x80EC, 3)) return;
    SYNC_IN;
    mff_cycles(0x87AF, 12);
    a = x;
    a &= 0x03;
    y = a;
    a = RAM(SND_PERIOD_HI_CACHE + y);
    if (a & 0x80) { mff_cycles(0x87B6, 2); goto L_880C; }
    mff_cycles(0x87B8, 7);
    y = 0x05;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x87BC, 2);
    if (a == 0) { mff_cycles(0x87BC, 2); goto L_880C; }
    mff_cycles(0x87BE, 19);
    RAM(SND_TMP + 3) = a;
    y = RAM(SND_LFO + x);
    a = RAM(SND_ENV + x);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = y;
    n = (a) >> 7;
    z = (a) == 0;
    if (c) {
        mff_cycles(0x87CB, 2);
        a ^= 0xFF;
        n = (a) >> 7;
        z = (a) == 0;
    } else {
        mff_cycles(0x87C9, 1);
    }
    mff_cycles(0x87CD, 2);
    if (z) { mff_cycles(0x87CD, 2); goto L_880C; }
    mff_cycles(0x87CF, 9);
    RAM(SND_TMP) = a;
    SYNC_OUT;
    if (!mff_call_at(0x07D3, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x87D4, 38);
    a = RAM(SND_TMP);
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    t = RAM(SND_TMP + 1);
    t = (uint8_t)(t >> 1 | c << 7);
    RAM(SND_TMP + 1) = t;
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    t = RAM(SND_TMP + 1);
    t = (uint8_t)(t >> 1 | c << 7);
    RAM(SND_TMP + 1) = t;
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    t = RAM(SND_TMP + 1);
    t = (uint8_t)(t >> 1 | c << 7);
    RAM(SND_TMP + 1) = t;
    t = a;
    c = t & 1;
    t >>= 1;
    a = t;
    t = RAM(SND_TMP + 1);
    t = (uint8_t)(t >> 1 | c << 7);
    RAM(SND_TMP + 1) = t;
    y = a;
    a |= RAM(SND_TMP + 1);
    if (a == 0) { mff_cycles(0x87E5, 2); goto L_880C; }
    mff_cycles(0x87E7, 6);
    a = RAM(SND_ENV + x);
    if (a & 0x80) { mff_cycles(0x87EA, 1); goto L_87FA; }
    mff_cycles(0x87EC, 20);
    c = 0;
    a = RAM(SND_TMP + 1);
    m = RAM(SND_PITCH_LO + x);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(SND_TMP + 1) = a;
    a = y;
    m = RAM(SND_PITCH_HI + x);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    if (a != 0) { mff_cycles(0x87F8, 2); goto L_8809; }
L_87FA:;
    mff_cycles(0x87FA, 22);
    c = 1;
    a = RAM(SND_PITCH_LO + x);
    m = RAM(SND_TMP + 1);
    w = a - m - !c;
    c = w >= 0;
    a = (uint8_t)w;
    RAM(SND_TMP + 1) = a;
    a = RAM(SND_PITCH_HI + x);
    RAM(SND_TMP) = y;
    m = RAM(SND_TMP);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
L_8809:;
    mff_cycles(0x8809, 4);
    y = a;
    if (y != 0) { mff_cycles(0x880A, 1); goto L_8814; }
L_880C:;
    mff_cycles(0x880C, 11);
    a = RAM(SND_PITCH_LO + x);
    RAM(SND_TMP + 1) = a;
    y = RAM(SND_PITCH_HI + x);
L_8814:;
    mff_cycles(0x8814, 4);
    m = 0x28;
    c = x >= m;
    if (!c) {
        mff_cycles(0x8818, 5);
        a = RAM(SND_EFFECT_LOOP);
        if (a & 0x80) {
            mff_cycles(0x881C, 5);
            a = RAM(SND_EFFECT_PITCH_SCALE);
            if (a != 0) {
                mff_cycles(0x8820, 18);
                RAM(SND_TMP + 3) = a;
                RAM(SND_TMP) = y;
                a = RAM(SND_TMP + 1);
                n = (a) >> 7;
                z = (a) == 0;
                mff_push(a);
                SYNC_OUT;
                if (!mff_call_at(0x0829, 0x8006, 3)) return;
                SYNC_IN;
                mff_cycles(0x882A, 19);
                a = mff_pull();
                c = 0;
                m = RAM(SND_TMP + 1);
                w = a + m + c;
                c = w > 0xFF;
                a = (uint8_t)w;
                RAM(SND_TMP + 1) = a;
                a = 0x00;
                m = RAM(SND_TMP);
                w = a + m + c;
                v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
                c = w > 0xFF;
                a = (uint8_t)w;
                y = a;
            } else {
                mff_cycles(0x881E, 1);
            }
        } else {
            mff_cycles(0x881A, 1);
        }
    } else {
        mff_cycles(0x8816, 1);
    }
    mff_cycles(0x8835, 6);
    a = x;
    a &= 0x03;
    if (a != 0) { mff_cycles(0x8838, 1); goto L_8849; }
    mff_cycles(0x883A, 11);
    a = y;
    a &= 0x0F;
    y = 0x07;
    a |= mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x8841, 10);
    RAM(SND_TMP + 1) = a;
    a = 0x00;
    RAM(SND_TMP) = a;
    if (a == 0) { mff_cycles(0x8847, 1); goto L_8884; }
L_8849:;
    mff_cycles(0x8849, 4);
    a = y;
    y = 0x08;
    for (;;) {
        mff_cycles(0x884C, 6);
        y--;
        mff_cycles(0x8850, 2);
        if (a >= prg_read((uint16_t)(0x8953 + y))) break;
        mff_cycles(0x884C, 1);
    }
    mff_cycles(0x8852, 29);
    RAM(SND_TMP) = a;
    a = y;
    c = 0;
    m = RAM(SND_TMP);
    w = a + m + c;
    a = (uint8_t)w;
    y = a;
    a &= 0x07;
    c = 0;
    m = 0x07;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(SND_TMP) = a;
    a = y;
    a &= 0x38;
    a ^= 0x38;
    if (a != 0) {
        for (;;) {
            mff_cycles(0x8867, 16);
            t = RAM(SND_TMP);
            c = t & 1;
            t >>= 1;
            RAM(SND_TMP) = t;
            t = RAM(SND_TMP + 1);
            t = (uint8_t)(t >> 1 | c << 7);
            RAM(SND_TMP + 1) = t;
            c = 1;
            m = 0x08;
            w = a - m - !c;
            v = ((a ^ m) & (a ^ w) & 0x80) != 0;
            c = w >= 0;
            a = (uint8_t)w;
            if (a == 0) break;
            mff_cycles(0x8867, 1);
        }
    } else {
        mff_cycles(0x8865, 1);
    }
    mff_cycles(0x8870, 8);
    y = 0x00;
    a = RAM(SND_DETUNE + x);
    n = (a) >> 7;
    if (a != 0) {
        mff_cycles(0x8877, 2);
        if (n) {
            mff_cycles(0x8879, 2);
            y--;
        } else {
            mff_cycles(0x8877, 1);
        }
        mff_cycles(0x887A, 16);
        c = 0;
        m = RAM(SND_TMP + 1);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(SND_TMP + 1) = a;
        a = y;
        m = RAM(SND_TMP);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(SND_TMP) = a;
    } else {
        mff_cycles(0x8875, 1);
    }
L_8884:;
    mff_cycles(0x8884, 11);
    y = 0x02;
    a = RAM(SND_TMP + 1);
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x088A, 0x80EC, 3)) return;
    SYNC_IN;
    mff_cycles(0x888B, 15);
    a = x;
    a &= 0x03;
    y = a;
    a = RAM(SND_TMP);
    m = RAM(SND_PERIOD_HI_CACHE + y);
    c = a >= m;
    if (a != m) {
        mff_cycles(0x8896, 15);
        RAM(SND_PERIOD_HI_CACHE + y) = a;
        a |= 0x08;
        y = 0x03;
        n = (y) >> 7;
        z = (y) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x089F, 0x80EC, 3)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x8894, 1);
    }
    mff_cycles(0x88A0, 8);
    a = RAM(SND_ENV + x);
    a &= 0x20;
    if (a == 0) { mff_cycles(0x88A5, 1); goto L_88FA; }
    mff_cycles(0x88A7, 6);
    a = RAM(SND_SLIDE_AMOUNT + x);
    if (a == 0) { mff_cycles(0x88AA, 1); goto L_88F2; }
    mff_cycles(0x88AC, 9);
    y = 0x00;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    n = (t) >> 7;
    z = (t) == 0;
    FLAGS_OUT;
    mff_push(mff_p());
    if (c) {
        mff_cycles(0x88B2, 8);
        a ^= 0xFF;
        c = 0;
        m = 0x01;
        w = a + m + c;
        a = (uint8_t)w;
        y--;
    } else {
        mff_cycles(0x88B0, 1);
    }
    mff_cycles(0x88B8, 40);
    c = 0;
    m = RAM(SND_PITCH_LO + x);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(SND_PITCH_LO + x) = a;
    a = y;
    m = RAM(SND_PITCH_HI + x);
    w = a + m + c;
    a = (uint8_t)w;
    RAM(SND_PITCH_HI + x) = a;
    a = RAM(SND_TARGET + x);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    y = a;
    c = 1;
    a = RAM(SND_PITCH_LO + x);
    m = prg_read((uint16_t)(0x8959 + y));
    w = a - m - !c;
    c = w >= 0;
    mff_cycles(0x88D2, 10);
    a = RAM(SND_PITCH_HI + x);
    a &= 0x3F;
    m = prg_read((uint16_t)(0x895A + y));
    w = a - m - !c;
    c = w >= 0;
    mff_cycles(0x88DA, 12);
    a = 0xFF;
    m = 0x00;
    w = a + m + c;
    a = (uint8_t)w;
    mff_set_p(mff_pull());
    FLAGS_IN;
    m = 0x00;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    if (a != 0) { mff_cycles(0x88E1, 1); goto L_88FA; }
    mff_cycles(0x88E3, 4);
    a = x;
    if (a == 0) { mff_cycles(0x88E4, 1); goto L_88FA; }
    mff_cycles(0x88E6, 4);
    a = prg_read((uint16_t)(0x8959 + y));
    mff_cycles(0x88E9, 9);
    RAM(SND_PITCH_LO + x) = a;
    a = prg_read((uint16_t)(0x895A + y));
    mff_cycles(0x88EF, 5);
    RAM(SND_PITCH_HI + x) = a;
L_88F2:;
    mff_cycles(0x88F2, 11);
    a = RAM(SND_ENV + x);
    a &= 0xDF;
    RAM(SND_ENV + x) = a;
L_88FA:;
    mff_cycles(0x88FA, 7);
    y = 0x04;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x88FE, 4);
    a &= 0x7F;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0x8902, 13);
        c = 0;
        m = RAM(SND_LFO + x);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(SND_LFO + x) = a;
        if (c) {
            mff_cycles(0x890B, 13);
            a = RAM(SND_ENV + x);
            c = 0;
            m = 0x40;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(SND_ENV + x) = a;
        } else {
            mff_cycles(0x8909, 1);
        }
    } else {
        mff_cycles(0x8900, 1);
    }
    mff_cycles(0x8914, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/* snd_slide $88A0 (bank 3) - lifted by port/tools/lift.py */
void func_88A0_b3(void) {
    REGS_IN;
    mff_cycles(0x88A0, 8);
    a = RAM(SND_ENV + x);
    a &= 0x20;
    if (a == 0) { mff_cycles(0x88A5, 1); goto L_88FA; }
    mff_cycles(0x88A7, 6);
    a = RAM(SND_SLIDE_AMOUNT + x);
    if (a == 0) { mff_cycles(0x88AA, 1); goto L_88F2; }
    mff_cycles(0x88AC, 9);
    y = 0x00;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    n = (t) >> 7;
    z = (t) == 0;
    FLAGS_OUT;
    mff_push(mff_p());
    if (c) {
        mff_cycles(0x88B2, 8);
        a ^= 0xFF;
        c = 0;
        m = 0x01;
        w = a + m + c;
        a = (uint8_t)w;
        y--;
    } else {
        mff_cycles(0x88B0, 1);
    }
    mff_cycles(0x88B8, 40);
    c = 0;
    m = RAM(SND_PITCH_LO + x);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(SND_PITCH_LO + x) = a;
    a = y;
    m = RAM(SND_PITCH_HI + x);
    w = a + m + c;
    a = (uint8_t)w;
    RAM(SND_PITCH_HI + x) = a;
    a = RAM(SND_TARGET + x);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    y = a;
    c = 1;
    a = RAM(SND_PITCH_LO + x);
    m = prg_read((uint16_t)(0x8959 + y));
    w = a - m - !c;
    c = w >= 0;
    mff_cycles(0x88D2, 10);
    a = RAM(SND_PITCH_HI + x);
    a &= 0x3F;
    m = prg_read((uint16_t)(0x895A + y));
    w = a - m - !c;
    c = w >= 0;
    mff_cycles(0x88DA, 12);
    a = 0xFF;
    m = 0x00;
    w = a + m + c;
    a = (uint8_t)w;
    mff_set_p(mff_pull());
    FLAGS_IN;
    m = 0x00;
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    if (a != 0) { mff_cycles(0x88E1, 1); goto L_88FA; }
    mff_cycles(0x88E3, 4);
    a = x;
    if (a == 0) { mff_cycles(0x88E4, 1); goto L_88FA; }
    mff_cycles(0x88E6, 4);
    a = prg_read((uint16_t)(0x8959 + y));
    mff_cycles(0x88E9, 9);
    RAM(SND_PITCH_LO + x) = a;
    a = prg_read((uint16_t)(0x895A + y));
    mff_cycles(0x88EF, 5);
    RAM(SND_PITCH_HI + x) = a;
L_88F2:;
    mff_cycles(0x88F2, 11);
    a = RAM(SND_ENV + x);
    a &= 0xDF;
    RAM(SND_ENV + x) = a;
L_88FA:;
    mff_cycles(0x88FA, 7);
    y = 0x04;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x88FE, 4);
    a &= 0x7F;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) {
        mff_cycles(0x8902, 13);
        c = 0;
        m = RAM(SND_LFO + x);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(SND_LFO + x) = a;
        if (c) {
            mff_cycles(0x890B, 13);
            a = RAM(SND_ENV + x);
            c = 0;
            m = 0x40;
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            n = (a) >> 7;
            z = (a) == 0;
            RAM(SND_ENV + x) = a;
        } else {
            mff_cycles(0x8909, 1);
        }
    } else {
        mff_cycles(0x8900, 1);
    }
    mff_cycles(0x8914, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8914 (bank 3) - lifted by port/tools/lift.py */
void func_8914_b3(void) {
    REGS_IN;
    mff_cycles(0x8914, 6);
    SYNC_OUT;
    mff_rts(); return;
}
