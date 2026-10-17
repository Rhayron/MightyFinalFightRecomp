/*
 * sound_music.c - the music sequencer (bank $06 $8393-$85A2; docs/sound.md,
 * "Music data")
 *
 * A song has four channel streams (pulse 1, pulse 2, triangle, noise). Each
 * channel's X is $28 + channel, so it reaches the shared per-channel arrays
 * (SND_INSTRUMENT ...) at the music's copy and the sequencer's own arrays
 * through SND_SEQ_* (sound_driver.h). A stream is a byte code read until a
 * note: commands $00-$18 (snd_music_command), notes $20-$FF.
 */
#include "sound_driver.h"

/* snd_music_channel, $8393: music channel X (0-3), once a frame unless
 * the music is paused. Nothing if the channel has no stream. While the
 * note lasts: the envelope and output (snd_instrument_ptr, snd_envelope;
 * not with instrument 0), then the gate and the note's ticks count down by
 * $C7: a gate at 0 or below keys off, a note at 0 or below reads on
 * (snd_music_next, $83CD).
 * In: X = the channel. Out: X = $28 + channel; the rest from the last
 * routine called.
 * Caller: snd_tick_body. */
void func_8393_b3(void) {
    REGS_IN;
    mff_cycles(0x8393, 16);
    x |= SND_X_MUSIC;
    a = RAM(SND_SEQ_PTR_LO + x) | RAM(SND_SEQ_PTR_HI + x);
    NZ(a);
    if (z) {
        mff_cycles(0x839D, 1 + 6);
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0x839F, 6);
    a = RAM(SND_SEQ_TICKS + x);
    NZ(a);
    if (z) {
        mff_cycles(0x83A2, 1);
        SYNC_OUT;
        func_83CD_b3();
        return;
    }

    mff_cycles(0x83A4, 6);
    y = RAM(SND_INSTRUMENT + x);
    NZ(y);
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
    a = RAM(SND_SEQ_GATE + x);
    m = RAM(SND_TICKS);
    w = a - m;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    NZ(a);
    RAM(SND_SEQ_GATE + x) = a;
    if (z || !c) {
        if (z)
            mff_cycles(0x83B8, 1);
        else
            mff_cycles(0x83BA, 2);
        mff_cycles(0x83BC, 6);
        SYNC_OUT;
        if (!mff_call_at(0x03BE, 0x85A3, 3)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x83BA, 2 + 1);
    }

    mff_cycles(0x83BF, 16);
    a = RAM(SND_SEQ_TICKS + x);
    m = RAM(SND_TICKS);
    w = a - m;
    v = ((a ^ m) & (a ^ w) & 0x80) != 0;
    c = w >= 0;
    a = (uint8_t)w;
    NZ(a);
    RAM(SND_SEQ_TICKS + x) = a;
    if (z) {
        mff_cycles(0x83C8, 1);
    } else {
        mff_cycles(0x83CA, 2);
        if (c) {
            mff_cycles(0x83CC, 6);
            SYNC_OUT;
            mff_rts();
            return;
        }
        mff_cycles(0x83CA, 1);
    }
    SYNC_OUT;
    func_83CD_b3();
}

/* snd_music_next, $83CD: music channel X ($28 + channel) reads its
 * stream: each command byte through snd_music_command (which leaves the
 * channel for this frame on $17), then the note or rest (snd_music_note,
 * $83DA; bits 5-7 the length code, bits 0-4 the pitch, 0 = rest):
 *  - the length: from the triplet table ($8915) with flag $20, else the
 *    normal one ($891C), 1.5 times and the dot flag cleared with flag $10;
 *    added to the ticks left ($0738,X), so overruns carry over;
 *  - a rest: key off, gate $FF (snd_music_gate_open);
 *  - a note: the gate = max(1, ticks * gate ratio >> 8). Tied to the
 *    previous note (flag $80): no attack, and the pitch stays unless a
 *    portamento speed is set (snd_lfo_check, snd_music_tie_next).
 *    Otherwise key on and, if the effect does not own the channel, the
 *    period high rewritten. Then the pitch: on the noise channel
 *    ((pitch - 1) & 15) ^ 15 is the period index (snd_set_pitch, $C3 = 0);
 *    elsewhere the note number octave_base[flags & $0F] + pitch - 1 + $CB
 *    + channel transpose (snd_note_pitch); then the tie flag moves to
 *    "tied" (snd_music_tie_next).
 * Callers: snd_music_channel (branches), and itself after each command
 * (JMP $83CD, a loop here). */
void func_83CD_b3(void) {
    REGS_IN;
    int normal;

    for (;;) {
        mff_cycles(0x83CD, 6);
        SYNC_OUT;
        if (!mff_call_at(0x03CF, 0x8592, 3)) return;
        SYNC_IN;
        mff_cycles(0x83D0, 4);
        CMP(a, SND_NOTE);
        if (c)
            break;
        mff_cycles(0x83D4, 6);
        SYNC_OUT;
        if (!mff_call_at(0x03D6, 0x8497, 3)) return;
        SYNC_IN;
        mff_cycles(0x83D7, 3);
        nes_cpu_instruction_boundary(0x83CD, 2);
    }
    mff_cycles(0x83D2, 1);

    /* the length */
    mff_cycles(0x83DA, 27);
    mff_push(a);
    y = (uint8_t)((a >> 5) - 1);
    a = (uint8_t)(RAM(SND_SEQ_FLAGS + x) << 2);
    if (a & 0x80) {
        mff_cycles(0x83EA, 4);
        a = prg_read((uint16_t)(SND_TBL_LEN_TRIPLET + y));
        mff_cycles(0x83ED, 2);
        normal = a == 0;                        /* no triplet length is 0 */
        if (!normal)
            mff_cycles(0x83ED, 2);
    } else {
        mff_cycles(0x83E8, 1);
        normal = 1;
    }
    if (normal) {
        mff_cycles(0x83EF, 8);
        c = (a >> 6) & 1;                       /* the dot flag (ASL, ASL) */
        a = prg_read((uint16_t)(SND_TBL_LEN_NORMAL + y));
        mff_cycles(0x83F4, 2);
        if (c) {
            mff_cycles(0x83F6, 24);
            RAM(SND_ARG) = a;
            RAM(SND_SEQ_FLAGS + x) &= (uint8_t)~SND_MF_DOTTED;
            a = (uint8_t)((a >> 1) + RAM(SND_ARG));
        } else {
            mff_cycles(0x83F4, 2);
        }
    }
    mff_cycles(0x8406, 21);
    m = RAM(SND_SEQ_TICKS + x);
    w = a + m;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    RAM(SND_SEQ_TICKS + x) = a;
    y = a;
    a = mff_pull() & SND_NOTE_PITCH;
    NZ(a);
    if (z) {
        /* a rest */
        mff_cycles(0x8413, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0415, 0x85A3, 3)) return;
        SYNC_IN;
        mff_cycles(0x8416, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x8491, 2);
        call_by_address_tail(0x8491, 3);
        return;
    }
    mff_cycles(0x8411, 1);

    /* the gate */
    mff_cycles(0x8419, 19);
    mff_push(a);
    RAM(SND_MULTIPLIER) = y;
    a = RAM(SND_SEQ_GATE_RATIO + x);
    NZ(a);
    RAM(SND_PRODUCT_HI) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0423, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x8424, 5);
    a = RAM(SND_PRODUCT_HI);
    if (a == 0) {
        mff_cycles(0x8428, 2);
        a = 0x01;
    } else {
        mff_cycles(0x8426, 1);
    }
    mff_cycles(0x842A, 19);
    RAM(SND_SEQ_GATE + x) = a;
    a = mff_pull();
    y = (uint8_t)(a - 1);                       /* the pitch - 1 */
    a = RAM(SND_SEQ_FLAGS + x);
    NZ(a);
    if (n) {
        /* tied: the pitch moves only with a portamento */
        mff_cycles(0x8435, 6);
        a = RAM(SND_SLIDE_AMOUNT + x);
        NZ(a);
        if (z) {
            mff_cycles(0x843A, 6);
            SYNC_OUT;
            if (!mff_call_at(0x043C, 0x8644, 3)) return;
            SYNC_IN;
            mff_cycles(0x843D, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x847E, 2);
            call_by_address_tail(0x847E, 3);
            return;
        }
        mff_cycles(0x8438, 1);
    } else {
        mff_cycles(0x8433, 1);
        mff_cycles(0x8440, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0442, 0x85AE, 3)) return;
        SYNC_IN;
        mff_cycles(0x8443, 5);
        a = RAM(SND_EFFECT_CHANNELS);
        if (!(a & SND_OWNED)) {
            mff_cycles(0x8447, 19);
            RAM(SND_ARG) = y;
            a = x & 3;
            y = a;
            a = 0xFF;
            RAM(SND_PERIOD_HI_CACHE + y) = a;
            y = RAM(SND_ARG);
        } else {
            mff_cycles(0x8445, 1);
        }
    }

    /* the pitch */
    mff_cycles(0x8454, 6);
    a = x & 3;
    if (a == SND_X_NOISE) {
        mff_cycles(0x8459, 15);
        RAM(SND_ARG) = a;
        a = (y & 0x0F) ^ 0x0F;
        NZ(a);
        SYNC_OUT;
        if (!mff_call_at(0x0462, 0x8636, 3)) return;
        SYNC_IN;
        mff_cycles(0x8463, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x847E, 2);
        call_by_address_tail(0x847E, 3);
        return;
    }
    mff_cycles(0x8457, 1);
    mff_cycles(0x8466, 15);
    RAM(SND_ARG) = y;
    y = RAM(SND_SEQ_FLAGS + x) & 0x0F;
    a = prg_read((uint16_t)(SND_TBL_OCTAVE_BASE + y));
    mff_cycles(0x8471, 22);
    a = (uint8_t)(a + RAM(SND_ARG));
    a = (uint8_t)(a + RAM(SND_TRANSPOSE));
    m = RAM(SND_SEQ_TRANSPOSE + x);
    w = a + m;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    NZ(a);
    SYNC_OUT;
    if (!mff_call_at(0x047D, 0x85DE, 3)) return;
    func_847E_b3();
}

/* snd_music_tie_next, $847E: music channel X's tie flag ($40) becomes the
 * "tied" flag ($80) for the next note; a tied note keeps the gate open
 * (snd_music_gate_open).
 * Out: A = the flags (N, Z from them) or $FF; C clear; Y = the old flags.
 * Callers: snd_music_next (JMP, and its fall through). */
void func_847E_b3(void) {
    REGS_IN;
    mff_cycles(0x847E, 27);
    y = RAM(SND_SEQ_FLAGS + x);
    a = (uint8_t)((y & SND_MF_TIE) << 1);
    c = 0;
    RAM(SND_TMP + 3) = a;
    a = (y & (uint8_t)~SND_MF_TIED) | RAM(SND_TMP + 3);
    NZ(a);
    RAM(SND_SEQ_FLAGS + x) = a;
    if (n) {
        SYNC_OUT;
        func_8491_b3();
        return;
    }
    mff_cycles(0x848F, 1 + 6);
    SYNC_OUT;
    mff_rts();
}

/* snd_music_gate_open, $8491: music channel X's gate counter = $FF (after a
 * rest, and on a tied note).
 * Callers: snd_music_next (JMP after a rest), snd_music_tie_next (fall
 * through). */
void func_8491_b3(void) {
    REGS_IN;
    mff_cycles(0x8491, 13);
    a = 0xFF;
    NZ(a);
    RAM(SND_SEQ_GATE + x) = a;
    SYNC_OUT;
    mff_rts();
}

/* snd_music_command, $8497: music opcode A ($00-$18) on channel X. From
 * $04 up the next stream byte is its argument, in $C3 (the opcode in $C4).
 * Jumps through the table at $84A7; the handlers return to snd_music_next
 * (snd_music_end skips it).
 * Caller: snd_music_next. */
void func_8497_b3(void) {
    REGS_IN;
    mff_cycles(0x8497, 4);
    CMP(a, SND_OP_SET_FLAGS);
    if (c) {
        mff_cycles(0x849B, 9);
        RAM(SND_TMP + 3) = a;
        SYNC_OUT;
        if (!mff_call_at(0x049F, 0x8592, 3)) return;
        SYNC_IN;
        mff_cycles(0x84A0, 6);
        RAM(SND_ARG) = a;
        a = RAM(SND_TMP + 3);
        NZ(a);
    } else {
        mff_cycles(0x8499, 1);
    }
    mff_cycles(0x84A4, 6);
    SYNC_OUT;
    if (!mff_call_at(0x04A6, 0x8023, 3)) return;
    mff_unreachable(SND_TBL_MUSIC_CMD);         /* the JSR never comes back */
}

/* snd_music_triplet, $84D9: opcode $00, toggle triplets (flag $20).
 * Out: A = the flags, N, Z from them. Caller: snd_music_command. */
void func_84D9_b3(void) {
    REGS_IN;
    mff_cycles(0x84D9, 4 + 1 + 4);
    a = SND_MF_TRIPLET ^ RAM(SND_SEQ_FLAGS + x);
    NZ(a);
    mff_cycles(0x84ED, 11);
    RAM(SND_SEQ_FLAGS + x) = a;
    SYNC_OUT;
    mff_rts();
}

/* snd_music_tie, $84DD: opcode $01, toggle the tie (flag $40).
 * Out: A = the flags, N, Z from them. Caller: snd_music_command. */
void func_84DD_b3(void) {
    REGS_IN;
    mff_cycles(0x84DD, 4 + 1 + 4);
    a = SND_MF_TIE ^ RAM(SND_SEQ_FLAGS + x);
    NZ(a);
    mff_cycles(0x84ED, 11);
    RAM(SND_SEQ_FLAGS + x) = a;
    SYNC_OUT;
    mff_rts();
}

/* snd_music_dot, $84E1: opcode $02, dot the next note (flag $10).
 * Out: A = the flags, N, Z from them. Caller: snd_music_command. */
void func_84E1_b3(void) {
    REGS_IN;
    mff_cycles(0x84E1, 8 + 1);
    a = SND_MF_DOTTED | RAM(SND_SEQ_FLAGS + x);
    NZ(a);
    mff_cycles(0x84ED, 11);
    RAM(SND_SEQ_FLAGS + x) = a;
    SYNC_OUT;
    mff_rts();
}

/* snd_music_octave_up2, $84E8: opcode $03, toggle octave + 2 (flag $08).
 * Out: A = the flags, N, Z from them. Caller: snd_music_command. */
void func_84E8_b3(void) {
    REGS_IN;
    mff_cycles(0x84E8, 17);
    a = SND_MF_OCTAVE_UP2 ^ RAM(SND_SEQ_FLAGS + x);
    NZ(a);
    RAM(SND_SEQ_FLAGS + x) = a;
    SYNC_OUT;
    mff_rts();
}

/* snd_music_tempo, $84F1: opcode $05 ii ff, the tempo for every channel:
 * $C9 = ii ticks and $CA = ff/256 a frame, the fraction $C8 = 0.
 * Out: A = ff, Y = ii (N, Z from it). Caller: snd_music_command. */
void func_84F1_b3(void) {
    REGS_IN;
    mff_cycles(0x84F1, 11);
    a = 0;
    NZ(a);
    RAM(SND_TEMPO_ACC) = a;
    SYNC_OUT;
    if (!mff_call_at(0x04F7, 0x8592, 3)) return;
    SYNC_IN;
    mff_cycles(0x84F8, 15);
    y = RAM(SND_ARG);
    NZ(y);
    RAM(SND_TEMPO + 1) = a;
    RAM(SND_TEMPO) = y;
    SYNC_OUT;
    mff_rts();
}

/* snd_music_gate_ratio, $84FF: opcode $06 gg, the gate ratio: release
 * after gg/256 of each note. Out: A = gg. Caller: snd_music_command. */
void func_84FF_b3(void) {
    REGS_IN;
    mff_cycles(0x84FF, 14);
    a = RAM(SND_ARG);
    NZ(a);
    RAM(SND_SEQ_GATE_RATIO + x) = a;
    SYNC_OUT;
    mff_rts();
}

/* snd_music_octave, $8505: opcode $09 oo, the octave (flags bits 0-2).
 * Out: A = the flags. Caller: snd_music_command. */
void func_8505_b3(void) {
    REGS_IN;
    mff_cycles(0x8505, 20);
    a = (RAM(SND_SEQ_FLAGS + x) & (uint8_t)~SND_MF_OCTAVE) | RAM(SND_ARG);
    NZ(a);
    RAM(SND_SEQ_FLAGS + x) = a;
    SYNC_OUT;
    mff_rts();
}

/* snd_music_transpose, $8510: opcode $0A tt, the global transpose $CB.
 * Out: A = tt. Caller: snd_music_command. */
void func_8510_b3(void) {
    REGS_IN;
    mff_cycles(0x8510, 12);
    a = RAM(SND_ARG);
    NZ(a);
    RAM(SND_TRANSPOSE) = a;
    SYNC_OUT;
    mff_rts();
}

/* snd_music_ch_transpose, $8515: opcode $0B tt, the channel's transpose.
 * Out: A = tt. Caller: snd_music_command. */
void func_8515_b3(void) {
    REGS_IN;
    mff_cycles(0x8515, 14);
    a = RAM(SND_ARG);
    NZ(a);
    RAM(SND_SEQ_TRANSPOSE + x) = a;
    SYNC_OUT;
    mff_rts();
}

/* snd_music_loop, $8529: opcodes $0E-$15 with A = 4 * slot (each channel
 * has four loop counters, $0744 + X + A). $C4 = the opcode, $C3 = its first
 * argument (cc or ff), hh ll follow in the stream.
 *  - loop ($0E-$11): the counter is loaded with cc on the first pass and
 *    counts down after; the jump is taken while it is not 0, so the
 *    section plays cc + 1 times;
 *  - break ($12-$15): on the loop's last pass (counter 1) the counter is
 *    cleared, the flags set as by $04 ff, and the jump taken.
 * Not taken: snd_music_skip ($8566) steps over hh ll. Taken: hh into $C3
 * ($8555), then snd_music_jump. */
static void snd_music_loop(uint8_t slot4) {
    REGS_IN;
    int skip;

    a = slot4;
    mff_cycles(0x8529, 19);
    RAM(SND_TMP + 1) = a;
    a = (uint8_t)(x + RAM(SND_TMP + 1));
    y = a;
    a = RAM(SND_TMP + 3);
    if (a < SND_OP_BREAK0) {
        mff_cycles(0x8536, 10);
        a = RAM(SND_SEQ_LOOPS + y);
        w = a - 1;
        v = ((a ^ 1) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        NZ(a);
        if (!c) {
            mff_cycles(0x853E, 3);
            a = RAM(SND_ARG);
            NZ(a);
        } else {
            mff_cycles(0x853C, 1);
        }
        mff_cycles(0x8540, 7);
        RAM(SND_SEQ_LOOPS + y) = a;
        skip = z;
        if (skip)
            mff_cycles(0x8543, 1);
        else
            mff_cycles(0x8545, 2 + 1);
    } else {
        mff_cycles(0x8534, 1);
        mff_cycles(0x8547, 10);
        a = RAM(SND_SEQ_LOOPS + y);
        w = a - 1;
        v = ((a ^ 1) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        NZ(a);
        skip = !z;
        if (skip) {
            mff_cycles(0x854D, 1);
        } else {
            mff_cycles(0x854F, 11);
            RAM(SND_SEQ_LOOPS + y) = a;
            SYNC_OUT;
            if (!mff_call_at(0x0554, 0x8575, 3)) return;
            SYNC_IN;
        }
    }

    if (skip) {
        mff_cycles(0x8566, 15);
        a = 0x02;
        m = RAM(SND_SEQ_PTR_LO + x);
        w = a + m;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        NZ(a);
        RAM(SND_SEQ_PTR_LO + x) = a;
        if (c) {
            mff_cycles(0x8571, 7);
            RAM(SND_SEQ_PTR_HI + x)++;
            NZ(RAM(SND_SEQ_PTR_HI + x));
        } else {
            mff_cycles(0x856F, 1);
        }
        mff_cycles(0x8574, 6);
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0x8555, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0557, 0x8592, 3)) return;
    SYNC_IN;
    mff_cycles(0x8558, 3);
    RAM(SND_ARG) = a;
    SYNC_OUT;
    func_855A_b3();
}

/* snd_music_loop0, $851B: opcodes $0E/$12 (slot 0): LDA #$00 / BEQ
 * snd_music_loop. Caller: snd_music_command (all four). */
void func_851B_b3(void) {
    mff_cycles(0x851B, 4 + 1);
    snd_music_loop(0x00);
}

/* snd_music_loop1, $851F: opcodes $0F/$13 (slot 1): LDA #$04 / BNE */
void func_851F_b3(void) {
    mff_cycles(0x851F, 4 + 1);
    snd_music_loop(0x04);
}

/* snd_music_loop2, $8523: opcodes $10/$14 (slot 2): LDA #$08 / BNE */
void func_8523_b3(void) {
    mff_cycles(0x8523, 4 + 1);
    snd_music_loop(0x08);
}

/* snd_music_loop3, $8527: opcodes $11/$15 (slot 3): LDA #$0C, falling
 * into snd_music_loop */
void func_8527_b3(void) {
    mff_cycles(0x8527, 2);
    snd_music_loop(0x0C);
}

/* snd_music_jump, $855A: opcode $16 hh ll (hh already in $C3): music
 * channel X's stream continues at hhll. Out: A = hh (N, Z from it).
 * Callers: snd_music_command; snd_music_loop (fall through). */
void func_855A_b3(void) {
    REGS_IN;
    mff_cycles(0x855A, 6);
    SYNC_OUT;
    if (!mff_call_at(0x055C, 0x8592, 3)) return;
    SYNC_IN;
    mff_cycles(0x855D, 19);
    RAM(SND_SEQ_PTR_LO + x) = a;
    a = RAM(SND_ARG);
    NZ(a);
    RAM(SND_SEQ_PTR_HI + x) = a;
    SYNC_OUT;
    mff_rts();
}

/* snd_music_set_flags, $8575: opcode $04 ff: the flags AND $97, OR ff
 * (octave + 2, dot, triplet and tie at once). Out: A = the flags.
 * Callers: snd_music_command; snd_music_loop (a break). */
void func_8575_b3(void) {
    REGS_IN;
    mff_cycles(0x8575, 20);
    a = (RAM(SND_SEQ_FLAGS + x) & SND_MF_SET_KEEP) | RAM(SND_ARG);
    NZ(a);
    RAM(SND_SEQ_FLAGS + x) = a;
    SYNC_OUT;
    mff_rts();
}

/* snd_music_end, $8580: opcode $17, the end of music channel X's stream.
 * Drops snd_music_command's return address (PLA PLA: the channel is done
 * for this frame), clears the stream pointer and silences the channel
 * unless the effect owns it (snd_silence, JMP).
 * Caller: snd_music_command. */
void func_8580_b3(void) {
    REGS_IN;
    mff_cycles(0x8580, 25);
    mff_pull();
    mff_pull();
    a = 0;
    RAM(SND_SEQ_PTR_LO + x) = a;
    RAM(SND_SEQ_PTR_HI + x) = a;
    a = RAM(SND_EFFECT_CHANNELS);
    NZ(a);
    if (!n) {
        mff_cycles(0x858E, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x80D8, 2);
        call_by_address_tail(0x80D8, 3);
        return;
    }
    mff_cycles(0x858C, 1 + 6);
    SYNC_OUT;
    mff_rts();
}

/* snd_music_read, $8592: the next byte of music channel X's stream
 * ($0728/$072C,X, incremented), through snd_read (JMP).
 * Out: A = the byte, N, Z from it; Y = 0.
 * Callers: snd_music_next, snd_music_command, snd_music_tempo, snd_music_loop,
 * snd_music_jump. */
void func_8592_b3(void) {
    REGS_IN;
    mff_cycles(0x8592, 17);
    y = RAM(SND_SEQ_PTR_LO + x);
    a = RAM(SND_SEQ_PTR_HI + x);
    RAM(SND_SEQ_PTR_LO + x)++;
    NZ(RAM(SND_SEQ_PTR_LO + x));
    if (z) {
        mff_cycles(0x859D, 7);
        RAM(SND_SEQ_PTR_HI + x)++;
        NZ(RAM(SND_SEQ_PTR_HI + x));
    } else {
        mff_cycles(0x859B, 1);
    }
    mff_cycles(0x85A0, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x803A, 2);
    call_by_address_tail(0x803A, 3);
}
