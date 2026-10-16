/*
 * lifted_b3.c - bank 3 routines $8000-$BFFF, lifted by
 * port/tools/lift_bank.py from the pure scaffold: exact, still to be
 * tidied and moved into their systems' files.
 */
#include "mff.h"
#include "state.h"
#include "coroutine.h"


/* snd_music_channel $8393 (bank 3) - lifted by port/tools/lift.py */
void func_8393_b3(void) {
    REGS_IN;
    mff_cycles(0x8393, 16);
    a = x;
    a |= 0x28;
    x = a;
    a = RAM(SND_M_INSTRUMENT + x);
    a |= RAM(SND_M_ENV + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x839D, 1); goto L_83CC; }
    mff_cycles(0x839F, 6);
    a = RAM(SND_M_LEVEL + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) { mff_cycles(0x83A2, 1); goto L_83CD; }
    mff_cycles(0x83A4, 6);
    y = RAM(SND_INSTRUMENT + x);
    n = (y) >> 7;
    z = (y) == 0;
    if (!z) {
        mff_cycles(0x83A9, 6);
        SYNC_OUT;
        if (!mff_call_at(0x03AB, 0x8684, 3)) return;
        SYNC_IN;
        mff_cycles(0x83AC, 6);
        SYNC_OUT;
        if (!mff_call_at(0x03AE, 0x86BA, 3)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x83A7, 1);
    }
    mff_cycles(0x83AF, 16);
    a = RAM(SND_M_SLIDE + x);
    c = 1;
    m = RAM(SND_TICKS);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_M_SLIDE + x) = a;
    if (z) { mff_cycles(0x83B8, 1); goto L_83BC; }
    mff_cycles(0x83BA, 2);
    if (c) { mff_cycles(0x83BA, 1); goto L_83BF; }
L_83BC:;
    mff_cycles(0x83BC, 6);
    SYNC_OUT;
    if (!mff_call_at(0x03BE, 0x85A3, 3)) return;
    SYNC_IN;
L_83BF:;
    mff_cycles(0x83BF, 16);
    a = RAM(SND_M_LEVEL + x);
    c = 1;
    m = RAM(SND_TICKS);
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_M_LEVEL + x) = a;
    if (z) { mff_cycles(0x83C8, 1); goto L_83CD; }
    mff_cycles(0x83CA, 2);
    if (!c) { mff_cycles(0x83CA, 1); goto L_83CD; }
L_83CC:;
    mff_cycles(0x83CC, 6);
    SYNC_OUT;
    mff_rts(); return;
L_83CD:;
    mff_cycles(0x83CD, 6);
    SYNC_OUT;
    if (!mff_call_at(0x03CF, 0x8592, 3)) return;
    SYNC_IN;
    mff_cycles(0x83D0, 4);
    m = 0x20;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0x83D4, 6);
        SYNC_OUT;
        if (!mff_call_at(0x03D6, 0x8497, 3)) return;
        SYNC_IN;
        mff_cycles(0x83D7, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x83CD, 2); call_by_address_tail(0x83CD, 3); return;
    } else {
        mff_cycles(0x83D2, 1);
    }
    mff_cycles(0x83DA, 27);
    mff_push(a);
    t = a;
    m = t >> 7;
    t = (uint8_t)(t << 1 | c);
    c = m;
    a = t;
    t = a;
    m = t >> 7;
    t = (uint8_t)(t << 1 | c);
    c = m;
    a = t;
    t = a;
    m = t >> 7;
    t = (uint8_t)(t << 1 | c);
    c = m;
    a = t;
    t = a;
    t = (uint8_t)(t << 1 | c);
    a = t;
    a &= 0x07;
    y = a;
    y--;
    a = RAM(SND_M_LFO + x);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    if (!(t & 0x80)) { mff_cycles(0x83E8, 1); goto L_83EF; }
    mff_cycles(0x83EA, 4);
    a = prg_read((uint16_t)(0x8915 + y));
    mff_cycles(0x83ED, 2);
    if (a != 0) { mff_cycles(0x83ED, 2); goto L_8406; }
L_83EF:;
    mff_cycles(0x83EF, 8);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = prg_read((uint16_t)(0x891C + y));
    mff_cycles(0x83F4, 2);
    if (c) {
        mff_cycles(0x83F6, 24);
        RAM(SND_TMP + 2) = a;
        a = RAM(SND_M_LFO + x);
        a &= 0xEF;
        RAM(SND_M_LFO + x) = a;
        a = RAM(SND_TMP + 2);
        t = a;
        t >>= 1;
        a = t;
        c = 0;
        m = RAM(SND_TMP + 2);
        w = a + m + c;
        a = (uint8_t)w;
    } else {
        mff_cycles(0x83F4, 2);
    }
L_8406:;
    mff_cycles(0x8406, 21);
    c = 0;
    m = RAM(SND_M_LEVEL + x);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(SND_M_LEVEL + x) = a;
    y = a;
    a = mff_pull();
    a &= 0x1F;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0x8413, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0415, 0x85A3, 3)) return;
        SYNC_IN;
        mff_cycles(0x8416, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x8491, 2); call_by_address_tail(0x8491, 3); return;
    } else {
        mff_cycles(0x8411, 1);
    }
    mff_cycles(0x8419, 19);
    mff_push(a);
    RAM(SND_TMP + 3) = y;
    a = RAM(SND_M_DETUNE + x);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_TMP) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0423, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x8424, 5);
    a = RAM(SND_TMP);
    if (a == 0) {
        mff_cycles(0x8428, 2);
        a = 0x01;
    } else {
        mff_cycles(0x8426, 1);
    }
    mff_cycles(0x842A, 19);
    RAM(SND_M_SLIDE + x) = a;
    a = mff_pull();
    y = a;
    y--;
    a = RAM(SND_M_LFO + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0x8433, 1); goto L_8440; }
    mff_cycles(0x8435, 6);
    a = RAM(SND_SLIDE_AMOUNT + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x8438, 1); goto L_8454; }
    mff_cycles(0x843A, 6);
    SYNC_OUT;
    if (!mff_call_at(0x043C, 0x8644, 3)) return;
    SYNC_IN;
    mff_cycles(0x843D, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x847E, 2); call_by_address_tail(0x847E, 3); return;
L_8440:;
    mff_cycles(0x8440, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0442, 0x85AE, 3)) return;
    SYNC_IN;
    mff_cycles(0x8443, 5);
    a = RAM(SND_EFFECT_CHANNELS);
    if (!(a & 0x80)) {
        mff_cycles(0x8447, 19);
        RAM(SND_TMP + 2) = y;
        a = x;
        a &= 0x03;
        y = a;
        a = 0xFF;
        RAM(SND_PERIOD_HI_CACHE + y) = a;
        y = RAM(SND_TMP + 2);
    } else {
        mff_cycles(0x8445, 1);
    }
L_8454:;
    mff_cycles(0x8454, 6);
    a = x;
    a &= 0x03;
    if (a == 0) {
        mff_cycles(0x8459, 15);
        RAM(SND_TMP + 2) = a;
        a = y;
        a &= 0x0F;
        a ^= 0x0F;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x0462, 0x8636, 3)) return;
        SYNC_IN;
        mff_cycles(0x8463, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x847E, 2); call_by_address_tail(0x847E, 3); return;
    } else {
        mff_cycles(0x8457, 1);
    }
    mff_cycles(0x8466, 15);
    RAM(SND_TMP + 2) = y;
    a = RAM(SND_M_LFO + x);
    a &= 0x0F;
    y = a;
    a = prg_read((uint16_t)(0x8923 + y));
    mff_cycles(0x8471, 22);
    c = 0;
    m = RAM(SND_TMP + 2);
    w = a + m + c;
    a = (uint8_t)w;
    c = 0;
    m = RAM(SND_TRANSPOSE);
    w = a + m + c;
    a = (uint8_t)w;
    c = 0;
    m = RAM(SND_M_VOLUME + x);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x047D, 0x85DE, 3)) return;
    SYNC_IN;
    mff_cycles(0x847E, 27);
    a = RAM(SND_M_LFO + x);
    y = a;
    a &= 0x40;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    RAM(SND_TMP + 3) = a;
    a = y;
    a &= 0x7F;
    a |= RAM(SND_TMP + 3);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_M_LFO + x) = a;
    if (n) {
        mff_cycles(0x8491, 7);
        a = 0xFF;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(SND_M_SLIDE + x) = a;
    } else {
        mff_cycles(0x848F, 1);
    }
    mff_cycles(0x8496, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $83CD (bank 3) - lifted by port/tools/lift.py */
void func_83CD_b3(void) {
    REGS_IN;
    mff_cycles(0x83CD, 6);
    SYNC_OUT;
    if (!mff_call_at(0x03CF, 0x8592, 3)) return;
    SYNC_IN;
    mff_cycles(0x83D0, 4);
    m = 0x20;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!c) {
        mff_cycles(0x83D4, 6);
        SYNC_OUT;
        if (!mff_call_at(0x03D6, 0x8497, 3)) return;
        SYNC_IN;
        mff_cycles(0x83D7, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x83CD, 2); call_by_address_tail(0x83CD, 3); return;
    } else {
        mff_cycles(0x83D2, 1);
    }
    mff_cycles(0x83DA, 27);
    mff_push(a);
    t = a;
    m = t >> 7;
    t = (uint8_t)(t << 1 | c);
    c = m;
    a = t;
    t = a;
    m = t >> 7;
    t = (uint8_t)(t << 1 | c);
    c = m;
    a = t;
    t = a;
    m = t >> 7;
    t = (uint8_t)(t << 1 | c);
    c = m;
    a = t;
    t = a;
    t = (uint8_t)(t << 1 | c);
    a = t;
    a &= 0x07;
    y = a;
    y--;
    a = RAM(SND_M_LFO + x);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    if (!(t & 0x80)) { mff_cycles(0x83E8, 1); goto L_83EF; }
    mff_cycles(0x83EA, 4);
    a = prg_read((uint16_t)(0x8915 + y));
    mff_cycles(0x83ED, 2);
    if (a != 0) { mff_cycles(0x83ED, 2); goto L_8406; }
L_83EF:;
    mff_cycles(0x83EF, 8);
    t = a;
    t = (uint8_t)(t << 1);
    a = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = prg_read((uint16_t)(0x891C + y));
    mff_cycles(0x83F4, 2);
    if (c) {
        mff_cycles(0x83F6, 24);
        RAM(SND_TMP + 2) = a;
        a = RAM(SND_M_LFO + x);
        a &= 0xEF;
        RAM(SND_M_LFO + x) = a;
        a = RAM(SND_TMP + 2);
        t = a;
        t >>= 1;
        a = t;
        c = 0;
        m = RAM(SND_TMP + 2);
        w = a + m + c;
        a = (uint8_t)w;
    } else {
        mff_cycles(0x83F4, 2);
    }
L_8406:;
    mff_cycles(0x8406, 21);
    c = 0;
    m = RAM(SND_M_LEVEL + x);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(SND_M_LEVEL + x) = a;
    y = a;
    a = mff_pull();
    a &= 0x1F;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0x8413, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0415, 0x85A3, 3)) return;
        SYNC_IN;
        mff_cycles(0x8416, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x8491, 2); call_by_address_tail(0x8491, 3); return;
    } else {
        mff_cycles(0x8411, 1);
    }
    mff_cycles(0x8419, 19);
    mff_push(a);
    RAM(SND_TMP + 3) = y;
    a = RAM(SND_M_DETUNE + x);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_TMP) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0423, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x8424, 5);
    a = RAM(SND_TMP);
    if (a == 0) {
        mff_cycles(0x8428, 2);
        a = 0x01;
    } else {
        mff_cycles(0x8426, 1);
    }
    mff_cycles(0x842A, 19);
    RAM(SND_M_SLIDE + x) = a;
    a = mff_pull();
    y = a;
    y--;
    a = RAM(SND_M_LFO + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) { mff_cycles(0x8433, 1); goto L_8440; }
    mff_cycles(0x8435, 6);
    a = RAM(SND_SLIDE_AMOUNT + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x8438, 1); goto L_8454; }
    mff_cycles(0x843A, 6);
    SYNC_OUT;
    if (!mff_call_at(0x043C, 0x8644, 3)) return;
    SYNC_IN;
    mff_cycles(0x843D, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x847E, 2); call_by_address_tail(0x847E, 3); return;
L_8440:;
    mff_cycles(0x8440, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0442, 0x85AE, 3)) return;
    SYNC_IN;
    mff_cycles(0x8443, 5);
    a = RAM(SND_EFFECT_CHANNELS);
    if (!(a & 0x80)) {
        mff_cycles(0x8447, 19);
        RAM(SND_TMP + 2) = y;
        a = x;
        a &= 0x03;
        y = a;
        a = 0xFF;
        RAM(SND_PERIOD_HI_CACHE + y) = a;
        y = RAM(SND_TMP + 2);
    } else {
        mff_cycles(0x8445, 1);
    }
L_8454:;
    mff_cycles(0x8454, 6);
    a = x;
    a &= 0x03;
    if (a == 0) {
        mff_cycles(0x8459, 15);
        RAM(SND_TMP + 2) = a;
        a = y;
        a &= 0x0F;
        a ^= 0x0F;
        n = (a) >> 7;
        z = (a) == 0;
        SYNC_OUT;
        if (!mff_call_at(0x0462, 0x8636, 3)) return;
        SYNC_IN;
        mff_cycles(0x8463, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x847E, 2); call_by_address_tail(0x847E, 3); return;
    } else {
        mff_cycles(0x8457, 1);
    }
    mff_cycles(0x8466, 15);
    RAM(SND_TMP + 2) = y;
    a = RAM(SND_M_LFO + x);
    a &= 0x0F;
    y = a;
    a = prg_read((uint16_t)(0x8923 + y));
    mff_cycles(0x8471, 22);
    c = 0;
    m = RAM(SND_TMP + 2);
    w = a + m + c;
    a = (uint8_t)w;
    c = 0;
    m = RAM(SND_TRANSPOSE);
    w = a + m + c;
    a = (uint8_t)w;
    c = 0;
    m = RAM(SND_M_VOLUME + x);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    SYNC_OUT;
    if (!mff_call_at(0x047D, 0x85DE, 3)) return;
    SYNC_IN;
    mff_cycles(0x847E, 27);
    a = RAM(SND_M_LFO + x);
    y = a;
    a &= 0x40;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    RAM(SND_TMP + 3) = a;
    a = y;
    a &= 0x7F;
    a |= RAM(SND_TMP + 3);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_M_LFO + x) = a;
    if (n) {
        mff_cycles(0x8491, 7);
        a = 0xFF;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(SND_M_SLIDE + x) = a;
    } else {
        mff_cycles(0x848F, 1);
    }
    mff_cycles(0x8496, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $847E (bank 3) - lifted by port/tools/lift.py */
void func_847E_b3(void) {
    REGS_IN;
    mff_cycles(0x847E, 27);
    a = RAM(SND_M_LFO + x);
    y = a;
    a &= 0x40;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    RAM(SND_TMP + 3) = a;
    a = y;
    a &= 0x7F;
    a |= RAM(SND_TMP + 3);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_M_LFO + x) = a;
    if (n) {
        mff_cycles(0x8491, 7);
        a = 0xFF;
        n = (a) >> 7;
        z = (a) == 0;
        RAM(SND_M_SLIDE + x) = a;
    } else {
        mff_cycles(0x848F, 1);
    }
    mff_cycles(0x8496, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8491 (bank 3) - lifted by port/tools/lift.py */
void func_8491_b3(void) {
    REGS_IN;
    mff_cycles(0x8491, 13);
    a = 0xFF;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_M_SLIDE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/* snd_music_command $8497 (bank 3) - lifted by port/tools/lift.py */
void func_8497_b3(void) {
    REGS_IN;
    mff_cycles(0x8497, 4);
    m = 0x04;
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (c) {
        mff_cycles(0x849B, 9);
        RAM(SND_TMP + 3) = a;
        SYNC_OUT;
        if (!mff_call_at(0x049F, 0x8592, 3)) return;
        SYNC_IN;
        mff_cycles(0x84A0, 6);
        RAM(SND_TMP + 2) = a;
        a = RAM(SND_TMP + 3);
        n = (a) >> 7;
        z = (a) == 0;
    } else {
        mff_cycles(0x8499, 1);
    }
    mff_cycles(0x84A4, 6);
    SYNC_OUT;
    if (!mff_call_at(0x04A6, 0x8023, 3)) return;
    SYNC_IN;
    mff_cycles(0x84A7, 4);
    m = prg_read((uint16_t)(0xDD84 + y));
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    mff_cycles(0x84AA, 78);
    RAM(SOUND_QUEUE + 5) = y;
    RAM(CHR_BANKS) = y;
    RAM(SPAWN_LIST_X_HI + 1) = y;
    RAM(UNK_F1) = a;
    RAM(PPUCTRL_SHADOW) = y;
    RAM(UNK_5A) = y;
    RAM(PLAYER_STAND_ANIM) = x;
    RAM(TMP5) = x;
    RAM(TMP16) = a;
    RAM(JOY_PRESSED + 1) = a;
    RAM(NMI_RETURN_PC + 1) = a;
    RAM(ATTACK_ID) = x;
    RAM(UNK_1B) = x;
    RAM(UNK_1F) = a;
    RAM(SCROLL_MODE) = a;
    RAM(PLAYER_X_PREV) = a;
    RAM(UNK_1B) = a;
    RAM(UNK_1F) = a;
    RAM(SCROLL_MODE) = a;
    RAM(PLAYER_X_PREV) = a;
    RAM(UNK_5A) = a;
    RAM(TASKS) = a;
    RAM(WEAPON_AMMO) = a;
    RAM(WEAPON_HELD) = x;
    SYNC_OUT;
    if (!mff_call_at(0x04DC, 0x0DD0, -1)) return;
    SYNC_IN;
    mff_cycles(0x84DD, 4);
    a = 0x40;
    if (a != 0) { mff_cycles(0x84DF, 1); goto L_84EA; }
    mff_cycles(0x84E1, 8);
    a = 0x10;
    a |= RAM(SND_M_LFO + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x84E6, 1); goto L_84ED; }
    mff_cycles(0x84E8, 2);
    a = 0x08;
L_84EA:;
    mff_cycles(0x84EA, 4);
    a ^= RAM(SND_M_LFO + x);
    n = (a) >> 7;
    z = (a) == 0;
L_84ED:;
    mff_cycles(0x84ED, 11);
    RAM(SND_M_LFO + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $84D9 (bank 3) - lifted by port/tools/lift.py */
void func_84D9_b3(void) {
    REGS_IN;
    mff_cycles(0x84D9, 4);
    a = 0x20;
    if (a != 0) { mff_cycles(0x84DB, 1); goto L_84EA; }
    mff_cycles(0x84DD, 4);
    a = 0x40;
    if (a != 0) { mff_cycles(0x84DF, 1); goto L_84EA; }
    mff_cycles(0x84E1, 8);
    a = 0x10;
    a |= RAM(SND_M_LFO + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x84E6, 1); goto L_84ED; }
    mff_cycles(0x84E8, 2);
    a = 0x08;
L_84EA:;
    mff_cycles(0x84EA, 4);
    a ^= RAM(SND_M_LFO + x);
    n = (a) >> 7;
    z = (a) == 0;
L_84ED:;
    mff_cycles(0x84ED, 11);
    RAM(SND_M_LFO + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $84DD (bank 3) - lifted by port/tools/lift.py */
void func_84DD_b3(void) {
    REGS_IN;
    mff_cycles(0x84DD, 4);
    a = 0x40;
    if (a != 0) { mff_cycles(0x84DF, 1); goto L_84EA; }
    mff_cycles(0x84E1, 8);
    a = 0x10;
    a |= RAM(SND_M_LFO + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x84E6, 1); goto L_84ED; }
    mff_cycles(0x84E8, 2);
    a = 0x08;
L_84EA:;
    mff_cycles(0x84EA, 4);
    a ^= RAM(SND_M_LFO + x);
    n = (a) >> 7;
    z = (a) == 0;
L_84ED:;
    mff_cycles(0x84ED, 11);
    RAM(SND_M_LFO + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $84E1 (bank 3) - lifted by port/tools/lift.py */
void func_84E1_b3(void) {
    REGS_IN;
    mff_cycles(0x84E1, 8);
    a = 0x10;
    a |= RAM(SND_M_LFO + x);
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0x84E8, 6);
        a = 0x08;
        a ^= RAM(SND_M_LFO + x);
        n = (a) >> 7;
        z = (a) == 0;
    } else {
        mff_cycles(0x84E6, 1);
    }
    mff_cycles(0x84ED, 11);
    RAM(SND_M_LFO + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $84E8 (bank 3) - lifted by port/tools/lift.py */
void func_84E8_b3(void) {
    REGS_IN;
    mff_cycles(0x84E8, 17);
    a = 0x08;
    a ^= RAM(SND_M_LFO + x);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_M_LFO + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $84F1 (bank 3) - lifted by port/tools/lift.py */
void func_84F1_b3(void) {
    REGS_IN;
    mff_cycles(0x84F1, 11);
    a = 0x00;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_TEMPO_ACC) = a;
    SYNC_OUT;
    if (!mff_call_at(0x04F7, 0x8592, 3)) return;
    SYNC_IN;
    mff_cycles(0x84F8, 15);
    y = RAM(SND_TMP + 2);
    n = (y) >> 7;
    z = (y) == 0;
    RAM(SND_TEMPO + 1) = a;
    RAM(SND_TEMPO) = y;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $84FF (bank 3) - lifted by port/tools/lift.py */
void func_84FF_b3(void) {
    REGS_IN;
    mff_cycles(0x84FF, 14);
    a = RAM(SND_TMP + 2);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_M_DETUNE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8505 (bank 3) - lifted by port/tools/lift.py */
void func_8505_b3(void) {
    REGS_IN;
    mff_cycles(0x8505, 20);
    a = RAM(SND_M_LFO + x);
    a &= 0xF8;
    a |= RAM(SND_TMP + 2);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_M_LFO + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8510 (bank 3) - lifted by port/tools/lift.py */
void func_8510_b3(void) {
    REGS_IN;
    mff_cycles(0x8510, 12);
    a = RAM(SND_TMP + 2);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_TRANSPOSE) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8515 (bank 3) - lifted by port/tools/lift.py */
void func_8515_b3(void) {
    REGS_IN;
    mff_cycles(0x8515, 14);
    a = RAM(SND_TMP + 2);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_M_VOLUME + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $851B (bank 3) - lifted by port/tools/lift.py */
void func_851B_b3(void) {
    REGS_IN;
    mff_cycles(0x851B, 4);
    a = 0x00;
    if (a != 0) {
        mff_cycles(0x851F, 4);
        a = 0x04;
        if (a == 0) {
            mff_cycles(0x8523, 4);
            a = 0x08;
            if (a == 0) {
                mff_cycles(0x8527, 2);
                a = 0x0C;
            } else {
                mff_cycles(0x8525, 1);
            }
        } else {
            mff_cycles(0x8521, 1);
        }
    } else {
        mff_cycles(0x851D, 1);
    }
    mff_cycles(0x8529, 19);
    RAM(SND_TMP + 1) = a;
    a = x;
    c = 0;
    m = RAM(SND_TMP + 1);
    w = a + m + c;
    a = (uint8_t)w;
    y = a;
    a = RAM(SND_TMP + 3);
    if (a >= 0x12) { mff_cycles(0x8534, 1); goto L_8547; }
    mff_cycles(0x8536, 10);
    a = RAM(SND_M_TARGET + y);
    c = 1;
    m = 0x01;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    if (!c) {
        mff_cycles(0x853E, 3);
        a = RAM(SND_TMP + 2);
        n = (a) >> 7;
        z = (a) == 0;
    } else {
        mff_cycles(0x853C, 1);
    }
    mff_cycles(0x8540, 7);
    RAM(SND_M_TARGET + y) = a;
    if (z) { mff_cycles(0x8543, 1); goto L_8566; }
    mff_cycles(0x8545, 2);
    if (!z) { mff_cycles(0x8545, 1); goto L_8555; }
L_8547:;
    mff_cycles(0x8547, 10);
    a = RAM(SND_M_TARGET + y);
    c = 1;
    m = 0x01;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x854D, 1); goto L_8566; }
    mff_cycles(0x854F, 11);
    RAM(SND_M_TARGET + y) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0554, 0x8575, 3)) return;
    SYNC_IN;
L_8555:;
    mff_cycles(0x8555, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0557, 0x8592, 3)) return;
    SYNC_IN;
    mff_cycles(0x8558, 9);
    RAM(SND_TMP + 2) = a;
    SYNC_OUT;
    if (!mff_call_at(0x055C, 0x8592, 3)) return;
    SYNC_IN;
    mff_cycles(0x855D, 19);
    RAM(SND_M_INSTRUMENT + x) = a;
    a = RAM(SND_TMP + 2);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_M_ENV + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_8566:;
    mff_cycles(0x8566, 15);
    a = 0x02;
    c = 0;
    m = RAM(SND_M_INSTRUMENT + x);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_M_INSTRUMENT + x) = a;
    if (c) {
        mff_cycles(0x8571, 7);
        t = (uint8_t)(RAM(SND_M_ENV + x) + 1);
        RAM(SND_M_ENV + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
    } else {
        mff_cycles(0x856F, 1);
    }
    mff_cycles(0x8574, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $851F (bank 3) - lifted by port/tools/lift.py */
void func_851F_b3(void) {
    REGS_IN;
    mff_cycles(0x851F, 4);
    a = 0x04;
    if (a == 0) {
        mff_cycles(0x8523, 4);
        a = 0x08;
        if (a == 0) {
            mff_cycles(0x8527, 2);
            a = 0x0C;
        } else {
            mff_cycles(0x8525, 1);
        }
    } else {
        mff_cycles(0x8521, 1);
    }
    mff_cycles(0x8529, 19);
    RAM(SND_TMP + 1) = a;
    a = x;
    c = 0;
    m = RAM(SND_TMP + 1);
    w = a + m + c;
    a = (uint8_t)w;
    y = a;
    a = RAM(SND_TMP + 3);
    if (a >= 0x12) { mff_cycles(0x8534, 1); goto L_8547; }
    mff_cycles(0x8536, 10);
    a = RAM(SND_M_TARGET + y);
    c = 1;
    m = 0x01;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    if (!c) {
        mff_cycles(0x853E, 3);
        a = RAM(SND_TMP + 2);
        n = (a) >> 7;
        z = (a) == 0;
    } else {
        mff_cycles(0x853C, 1);
    }
    mff_cycles(0x8540, 7);
    RAM(SND_M_TARGET + y) = a;
    if (z) { mff_cycles(0x8543, 1); goto L_8566; }
    mff_cycles(0x8545, 2);
    if (!z) { mff_cycles(0x8545, 1); goto L_8555; }
L_8547:;
    mff_cycles(0x8547, 10);
    a = RAM(SND_M_TARGET + y);
    c = 1;
    m = 0x01;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x854D, 1); goto L_8566; }
    mff_cycles(0x854F, 11);
    RAM(SND_M_TARGET + y) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0554, 0x8575, 3)) return;
    SYNC_IN;
L_8555:;
    mff_cycles(0x8555, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0557, 0x8592, 3)) return;
    SYNC_IN;
    mff_cycles(0x8558, 9);
    RAM(SND_TMP + 2) = a;
    SYNC_OUT;
    if (!mff_call_at(0x055C, 0x8592, 3)) return;
    SYNC_IN;
    mff_cycles(0x855D, 19);
    RAM(SND_M_INSTRUMENT + x) = a;
    a = RAM(SND_TMP + 2);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_M_ENV + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_8566:;
    mff_cycles(0x8566, 15);
    a = 0x02;
    c = 0;
    m = RAM(SND_M_INSTRUMENT + x);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_M_INSTRUMENT + x) = a;
    if (c) {
        mff_cycles(0x8571, 7);
        t = (uint8_t)(RAM(SND_M_ENV + x) + 1);
        RAM(SND_M_ENV + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
    } else {
        mff_cycles(0x856F, 1);
    }
    mff_cycles(0x8574, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8523 (bank 3) - lifted by port/tools/lift.py */
void func_8523_b3(void) {
    REGS_IN;
    mff_cycles(0x8523, 4);
    a = 0x08;
    if (a == 0) {
        mff_cycles(0x8527, 2);
        a = 0x0C;
    } else {
        mff_cycles(0x8525, 1);
    }
    mff_cycles(0x8529, 19);
    RAM(SND_TMP + 1) = a;
    a = x;
    c = 0;
    m = RAM(SND_TMP + 1);
    w = a + m + c;
    a = (uint8_t)w;
    y = a;
    a = RAM(SND_TMP + 3);
    if (a >= 0x12) { mff_cycles(0x8534, 1); goto L_8547; }
    mff_cycles(0x8536, 10);
    a = RAM(SND_M_TARGET + y);
    c = 1;
    m = 0x01;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    if (!c) {
        mff_cycles(0x853E, 3);
        a = RAM(SND_TMP + 2);
        n = (a) >> 7;
        z = (a) == 0;
    } else {
        mff_cycles(0x853C, 1);
    }
    mff_cycles(0x8540, 7);
    RAM(SND_M_TARGET + y) = a;
    if (z) { mff_cycles(0x8543, 1); goto L_8566; }
    mff_cycles(0x8545, 2);
    if (!z) { mff_cycles(0x8545, 1); goto L_8555; }
L_8547:;
    mff_cycles(0x8547, 10);
    a = RAM(SND_M_TARGET + y);
    c = 1;
    m = 0x01;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x854D, 1); goto L_8566; }
    mff_cycles(0x854F, 11);
    RAM(SND_M_TARGET + y) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0554, 0x8575, 3)) return;
    SYNC_IN;
L_8555:;
    mff_cycles(0x8555, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0557, 0x8592, 3)) return;
    SYNC_IN;
    mff_cycles(0x8558, 9);
    RAM(SND_TMP + 2) = a;
    SYNC_OUT;
    if (!mff_call_at(0x055C, 0x8592, 3)) return;
    SYNC_IN;
    mff_cycles(0x855D, 19);
    RAM(SND_M_INSTRUMENT + x) = a;
    a = RAM(SND_TMP + 2);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_M_ENV + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_8566:;
    mff_cycles(0x8566, 15);
    a = 0x02;
    c = 0;
    m = RAM(SND_M_INSTRUMENT + x);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_M_INSTRUMENT + x) = a;
    if (c) {
        mff_cycles(0x8571, 7);
        t = (uint8_t)(RAM(SND_M_ENV + x) + 1);
        RAM(SND_M_ENV + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
    } else {
        mff_cycles(0x856F, 1);
    }
    mff_cycles(0x8574, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8527 (bank 3) - lifted by port/tools/lift.py */
void func_8527_b3(void) {
    REGS_IN;
    mff_cycles(0x8527, 21);
    a = 0x0C;
    RAM(SND_TMP + 1) = a;
    a = x;
    c = 0;
    m = RAM(SND_TMP + 1);
    w = a + m + c;
    a = (uint8_t)w;
    y = a;
    a = RAM(SND_TMP + 3);
    if (a >= 0x12) { mff_cycles(0x8534, 1); goto L_8547; }
    mff_cycles(0x8536, 10);
    a = RAM(SND_M_TARGET + y);
    c = 1;
    m = 0x01;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    if (!c) {
        mff_cycles(0x853E, 3);
        a = RAM(SND_TMP + 2);
        n = (a) >> 7;
        z = (a) == 0;
    } else {
        mff_cycles(0x853C, 1);
    }
    mff_cycles(0x8540, 7);
    RAM(SND_M_TARGET + y) = a;
    if (z) { mff_cycles(0x8543, 1); goto L_8566; }
    mff_cycles(0x8545, 2);
    if (!z) { mff_cycles(0x8545, 1); goto L_8555; }
L_8547:;
    mff_cycles(0x8547, 10);
    a = RAM(SND_M_TARGET + y);
    c = 1;
    m = 0x01;
    w = a - m - !c;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x854D, 1); goto L_8566; }
    mff_cycles(0x854F, 11);
    RAM(SND_M_TARGET + y) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0554, 0x8575, 3)) return;
    SYNC_IN;
L_8555:;
    mff_cycles(0x8555, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0557, 0x8592, 3)) return;
    SYNC_IN;
    mff_cycles(0x8558, 9);
    RAM(SND_TMP + 2) = a;
    SYNC_OUT;
    if (!mff_call_at(0x055C, 0x8592, 3)) return;
    SYNC_IN;
    mff_cycles(0x855D, 19);
    RAM(SND_M_INSTRUMENT + x) = a;
    a = RAM(SND_TMP + 2);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_M_ENV + x) = a;
    SYNC_OUT;
    mff_rts(); return;
L_8566:;
    mff_cycles(0x8566, 15);
    a = 0x02;
    c = 0;
    m = RAM(SND_M_INSTRUMENT + x);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_M_INSTRUMENT + x) = a;
    if (c) {
        mff_cycles(0x8571, 7);
        t = (uint8_t)(RAM(SND_M_ENV + x) + 1);
        RAM(SND_M_ENV + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
    } else {
        mff_cycles(0x856F, 1);
    }
    mff_cycles(0x8574, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/* snd_music_jump $855A (bank 3) - lifted by port/tools/lift.py */
void func_855A_b3(void) {
    REGS_IN;
    mff_cycles(0x855A, 6);
    SYNC_OUT;
    if (!mff_call_at(0x055C, 0x8592, 3)) return;
    SYNC_IN;
    mff_cycles(0x855D, 19);
    RAM(SND_M_INSTRUMENT + x) = a;
    a = RAM(SND_TMP + 2);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_M_ENV + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/* snd_music_set_flags $8575 (bank 3) - lifted by port/tools/lift.py */
void func_8575_b3(void) {
    REGS_IN;
    mff_cycles(0x8575, 20);
    a = RAM(SND_M_LFO + x);
    a &= 0x97;
    a |= RAM(SND_TMP + 2);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_M_LFO + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/* snd_music_end $8580 (bank 3) - lifted by port/tools/lift.py */
void func_8580_b3(void) {
    REGS_IN;
    mff_cycles(0x8580, 25);
    a = mff_pull();
    a = mff_pull();
    a = 0x00;
    RAM(SND_M_INSTRUMENT + x) = a;
    RAM(SND_M_ENV + x) = a;
    a = RAM(SND_EFFECT_CHANNELS);
    n = (a) >> 7;
    z = (a) == 0;
    if (!n) {
        mff_cycles(0x858E, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x80D8, 2); call_by_address_tail(0x80D8, 3); return;
    } else {
        mff_cycles(0x858C, 1);
    }
    mff_cycles(0x8591, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/* snd_music_read $8592 (bank 3) - lifted by port/tools/lift.py */
void func_8592_b3(void) {
    REGS_IN;
    mff_cycles(0x8592, 17);
    y = RAM(SND_M_INSTRUMENT + x);
    a = RAM(SND_M_ENV + x);
    t = (uint8_t)(RAM(SND_M_INSTRUMENT + x) + 1);
    RAM(SND_M_INSTRUMENT + x) = t;
    n = (t) >> 7;
    z = (t) == 0;
    if (z) {
        mff_cycles(0x859D, 7);
        t = (uint8_t)(RAM(SND_M_ENV + x) + 1);
        RAM(SND_M_ENV + x) = t;
        n = (t) >> 7;
        z = (t) == 0;
    } else {
        mff_cycles(0x859B, 1);
    }
    mff_cycles(0x85A0, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x803A, 2); call_by_address_tail(0x803A, 3); return;
}

/* snd_key_off $85A3 (bank 3) - lifted by port/tools/lift.py */
void func_85A3_b3(void) {
    REGS_IN;
    mff_cycles(0x85A3, 19);
    a = RAM(SND_ENV + x);
    a &= 0xF8;
    a |= 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_ENV + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/* snd_key_on $85AE (bank 3) - lifted by port/tools/lift.py */
void func_85AE_b3(void) {
    REGS_IN;
    mff_cycles(0x85AE, 22);
    a = y;
    mff_push(a);
    y = 0x00;
    a = RAM(SND_ENV + x);
    a &= 0xF8;
    RAM(SND_ENV + x) = a;
    m = 0x29;
    c = x >= m;
    if (x == m) { mff_cycles(0x85BC, 1); goto L_85D0; }
    mff_cycles(0x85BE, 4);
    m = 0x01;
    c = x >= m;
    if (x != m) { mff_cycles(0x85C0, 1); goto L_85D7; }
    mff_cycles(0x85C2, 19);
    a = RAM(SND_EFFECT_ROW_FRAMES);
    RAM(SND_TMP) = a;
    a = RAM(SND_VOLUME + x);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_TMP + 3) = a;
    SYNC_OUT;
    if (!mff_call_at(0x05CD, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x85CE, 3);
    y = RAM(SND_TMP);
L_85D0:;
    mff_cycles(0x85D0, 16);
    y++;
    t = (uint8_t)(RAM(SND_ENV + x) + 1);
    RAM(SND_ENV + x) = t;
    t = (uint8_t)(RAM(SND_ENV + x) + 1);
    RAM(SND_ENV + x) = t;
L_85D7:;
    mff_cycles(0x85D7, 19);
    a = y;
    RAM(SND_LEVEL + x) = a;
    a = mff_pull();
    y = a;
    n = (y) >> 7;
    z = (y) == 0;
    SYNC_OUT;
    mff_rts(); return;
}

/* snd_note_pitch $85DE (bank 3) - lifted by port/tools/lift.py */
void func_85DE_b3(void) {
    REGS_IN;
    mff_cycles(0x85DE, 4);
    if (a >= 0x60) {
        mff_cycles(0x85E2, 2);
        a = 0x5F;
    } else {
        mff_cycles(0x85E0, 1);
    }
    mff_cycles(0x85E4, 12);
    RAM(SND_TMP + 2) = a;
    t = (uint8_t)(RAM(SND_TMP + 2) + 1);
    RAM(SND_TMP + 2) = t;
    m = 0x28;
    if (x < m) { mff_cycles(0x85EA, 2); goto L_862A; }
    mff_cycles(0x85EC, 6);
    a = RAM(SND_TARGET + x);
    if (a == 0) { mff_cycles(0x85EF, 2); goto L_861D; }
    mff_cycles(0x85F1, 5);
    m = RAM(SND_TMP + 2);
    c = a >= m;
    if (a != m) { mff_cycles(0x85F3, 1); goto L_85FC; }
    mff_cycles(0x85F5, 6);
    a = RAM(SND_M_LFO + x);
    n = (a) >> 7;
    if (!n) { mff_cycles(0x85F8, 2); goto L_861D; }
    mff_cycles(0x85FA, 2);
    if (n) { mff_cycles(0x85FA, 2); goto L_8644; }
L_85FC:;
    mff_cycles(0x85FC, 6);
    a = RAM(SND_SLIDE_AMOUNT + x);
    if (a == 0) { mff_cycles(0x85FF, 1); goto L_861D; }
    mff_cycles(0x8601, 2);
    if (c) { mff_cycles(0x8601, 1); goto L_8607; }
    mff_cycles(0x8603, 4);
    a |= 0x80;
    if (a != 0) { mff_cycles(0x8605, 1); goto L_8609; }
L_8607:;
    mff_cycles(0x8607, 2);
    a &= 0x7F;
L_8609:;
    mff_cycles(0x8609, 28);
    RAM(SND_SLIDE_AMOUNT + x) = a;
    a = RAM(SND_ENV + x);
    a |= 0x20;
    RAM(SND_ENV + x) = a;
    a = RAM(SND_TMP + 2);
    y = RAM(SND_TARGET + x);
    RAM(SND_TMP + 2) = y;
    if (y != 0) { mff_cycles(0x861B, 1); goto L_8627; }
L_861D:;
    mff_cycles(0x861D, 14);
    a = RAM(SND_ENV + x);
    a &= 0xDF;
    RAM(SND_ENV + x) = a;
    a = RAM(SND_TMP + 2);
L_8627:;
    mff_cycles(0x8627, 5);
    RAM(SND_TARGET + x) = a;
L_862A:;
    mff_cycles(0x862A, 12);
    t = RAM(SND_TMP + 2);
    c = t >> 7;
    t = (uint8_t)(t << 1);
    RAM(SND_TMP + 2) = t;
    y = RAM(SND_TMP + 2);
    a = prg_read((uint16_t)(0x8959 + y));
    mff_cycles(0x8631, 7);
    RAM(SND_TMP + 2) = a;
    a = prg_read((uint16_t)(0x895A + y));
    mff_cycles(0x8636, 20);
    RAM(SND_PITCH_HI + x) = a;
    a = RAM(SND_TMP + 2);
    RAM(SND_PITCH_LO + x) = a;
    y = 0x04;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x8642, 2);
    if (a & 0x80) { mff_cycles(0x8642, 1); goto L_864C; }
L_8644:;
    mff_cycles(0x8644, 8);
    a = RAM(SND_ENV + x);
    a &= 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0x864B, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x8649, 1);
    }
L_864C:;
    mff_cycles(0x864C, 24);
    a = 0x00;
    RAM(SND_LFO + x) = a;
    a = RAM(SND_ENV + x);
    a &= 0x37;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_ENV + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/* snd_set_pitch $8636 (bank 3) - lifted by port/tools/lift.py */
void func_8636_b3(void) {
    REGS_IN;
    mff_cycles(0x8636, 20);
    RAM(SND_PITCH_HI + x) = a;
    a = RAM(SND_TMP + 2);
    RAM(SND_PITCH_LO + x) = a;
    y = 0x04;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x8642, 2);
    if (!(a & 0x80)) {
        mff_cycles(0x8644, 8);
        a = RAM(SND_ENV + x);
        a &= 0x08;
        n = (a) >> 7;
        z = (a) == 0;
        if (z) {
            mff_cycles(0x864B, 6);
            SYNC_OUT;
            mff_rts(); return;
        } else {
            mff_cycles(0x8649, 1);
        }
    } else {
        mff_cycles(0x8642, 1);
    }
    mff_cycles(0x864C, 24);
    a = 0x00;
    RAM(SND_LFO + x) = a;
    a = RAM(SND_ENV + x);
    a &= 0x37;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_ENV + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/*  $8644 (bank 3) - lifted by port/tools/lift.py */
void func_8644_b3(void) {
    REGS_IN;
    mff_cycles(0x8644, 8);
    a = RAM(SND_ENV + x);
    a &= 0x08;
    n = (a) >> 7;
    z = (a) == 0;
    if (z) {
        mff_cycles(0x864B, 6);
        SYNC_OUT;
        mff_rts(); return;
    } else {
        mff_cycles(0x8649, 1);
    }
    mff_cycles(0x864C, 24);
    a = 0x00;
    RAM(SND_LFO + x) = a;
    a = RAM(SND_ENV + x);
    a &= 0x37;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_ENV + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/* snd_set_volume $865A (bank 3) - lifted by port/tools/lift.py */
void func_865A_b3(void) {
    REGS_IN;
    mff_cycles(0x865A, 4);
    m = 0x01;
    c = x >= m;
    if (x != m) { mff_cycles(0x865C, 1); goto L_8662; }
    mff_cycles(0x865E, 5);
    a = RAM(SND_TMP + 2);
    n = (a) >> 7;
    z = (a) == 0;
    if (!z) { mff_cycles(0x8660, 1); goto L_866B; }
L_8662:;
    mff_cycles(0x8662, 11);
    a = RAM(SND_VOLUME + x);
    a &= 0xC0;
    a |= RAM(SND_TMP + 2);
    a |= 0x30;
    n = (a) >> 7;
    z = (a) == 0;
L_866B:;
    mff_cycles(0x866B, 11);
    RAM(SND_VOLUME + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/* snd_set_instrument $866F (bank 3) - lifted by port/tools/lift.py */
void func_866F_b3(void) {
    REGS_IN;
    mff_cycles(0x866F, 14);
    t = (uint8_t)(RAM(SND_TMP + 2) + 1);
    RAM(SND_TMP + 2) = t;
    a = RAM(SND_TMP + 2);
    m = RAM(SND_INSTRUMENT + x);
    c = a >= m;
    n = ((uint8_t)(a - m)) >> 7;
    z = ((uint8_t)(a - m)) == 0;
    if (!z) {
        mff_cycles(0x8678, 54);
        RAM(SND_INSTRUMENT + x) = a;
        y = a;
        a = RAM(SND_ENV + x);
        a |= 0x08;
        RAM(SND_ENV + x) = a;
        y--;
        a = 0x00;
        RAM(SND_TMP + 2) = a;
        a = y;
        t = a;
        c = t >> 7;
        t = (uint8_t)(t << 1);
        a = t;
        t = RAM(SND_TMP + 2);
        t = (uint8_t)(t << 1 | c);
        RAM(SND_TMP + 2) = t;
        t = a;
        c = t >> 7;
        t = (uint8_t)(t << 1);
        a = t;
        t = RAM(SND_TMP + 2);
        t = (uint8_t)(t << 1 | c);
        RAM(SND_TMP + 2) = t;
        t = a;
        c = t >> 7;
        t = (uint8_t)(t << 1);
        a = t;
        t = RAM(SND_TMP + 2);
        t = (uint8_t)(t << 1 | c);
        RAM(SND_TMP + 2) = t;
        c = 0;
        m = prg_read(0x8A42);
        w = a + m + c;
        c = w > 0xFF;
        a = (uint8_t)w;
        mff_cycles(0x8697, 10);
        RAM(SND_INSTR_PTR) = a;
        a = RAM(SND_TMP + 2);
        m = prg_read(0x8A41);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        n = (a) >> 7;
        z = (a) == 0;
        mff_cycles(0x869E, 3);
        RAM(SND_INSTR_PTR + 1) = a;
    } else {
        mff_cycles(0x8676, 1);
    }
    mff_cycles(0x86A0, 6);
    SYNC_OUT;
    mff_rts(); return;
}

/* snd_instrument_ptr $8684 (bank 3) - lifted by port/tools/lift.py */
void func_8684_b3(void) {
    REGS_IN;
    mff_cycles(0x8684, 36);
    y--;
    a = 0x00;
    RAM(SND_TMP + 2) = a;
    a = y;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    t = RAM(SND_TMP + 2);
    t = (uint8_t)(t << 1 | c);
    RAM(SND_TMP + 2) = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    t = RAM(SND_TMP + 2);
    t = (uint8_t)(t << 1 | c);
    RAM(SND_TMP + 2) = t;
    t = a;
    c = t >> 7;
    t = (uint8_t)(t << 1);
    a = t;
    t = RAM(SND_TMP + 2);
    t = (uint8_t)(t << 1 | c);
    RAM(SND_TMP + 2) = t;
    c = 0;
    m = prg_read(0x8A42);
    w = a + m + c;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0x8697, 10);
    RAM(SND_INSTR_PTR) = a;
    a = RAM(SND_TMP + 2);
    m = prg_read(0x8A41);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    n = (a) >> 7;
    z = (a) == 0;
    mff_cycles(0x869E, 9);
    RAM(SND_INSTR_PTR + 1) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/* snd_set_detune $86A1 (bank 3) - lifted by port/tools/lift.py */
void func_86A1_b3(void) {
    REGS_IN;
    mff_cycles(0x86A1, 14);
    a = RAM(SND_TMP + 2);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_DETUNE + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/* snd_set_slide $86A7 (bank 3) - lifted by port/tools/lift.py */
void func_86A7_b3(void) {
    REGS_IN;
    mff_cycles(0x86A7, 14);
    a = RAM(SND_TMP + 2);
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_SLIDE_AMOUNT + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

/* snd_set_duty $86AD (bank 3) - lifted by port/tools/lift.py */
void func_86AD_b3(void) {
    REGS_IN;
    mff_cycles(0x86AD, 22);
    a = RAM(SND_VOLUME + x);
    a &= 0x0F;
    a |= RAM(SND_TMP + 2);
    a |= 0x30;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(SND_VOLUME + x) = a;
    SYNC_OUT;
    mff_rts(); return;
}

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
