/*
 * sound_voice.c - a channel's state: key on and off, pitch, LFO reset,
 * instrument, volume, duty, detune and slide (bank $06 $85A3-$86B9;
 * docs/sound.md, "Instruments", "Pitch")
 *
 * The effect (X = 0-3) and the music (X = $28-$2B) share these routines;
 * the setters are both effect fields (snd_effect_field) and music opcodes
 * (snd_music_command), with their argument in $C3.
 */
#include "sound_driver.h"

/* snd_key_off, $85A3: channel X's envelope goes to the release (phase 3).
 * Out: A = $0704,X, N, Z from it.
 * Callers: snd_effect_channel (JMP, the row's gate), snd_music_channel (the
 * gate), snd_music_next (a rest). */
void func_85A3_b3(void) {
    REGS_IN;
    mff_cycles(0x85A3, 19);
    a = (RAM(SND_ENV + x) & (uint8_t)~SND_ENV_PHASE) | SND_PHASE_RELEASE;
    NZ(a);
    RAM(SND_ENV + x) = a;
    SYNC_OUT;
    mff_rts();
}

/* snd_key_on, $85AE: channel X's envelope starts: phase 0 (attack) at
 * level 0. The triangles have no envelope: they go to phase 2 (sustain),
 * the music's ($29) at level 1, the effect's (1) at ($D3 * $070C >> 8) + 1,
 * the frames it sounds (snd_effect_channel counts them down in $0710).
 * Out: Y kept, A = Y (N, Z from it); C from CPX or snd_mul8.
 * Callers: snd_effect_note, snd_music_next. */
void func_85AE_b3(void) {
    REGS_IN;
    int triangle;

    mff_cycles(0x85AE, 22);
    a = y;
    mff_push(a);
    y = 0;
    RAM(SND_ENV + x) &= (uint8_t)~SND_ENV_PHASE;
    c = x >= SND_X_MUSIC + SND_X_TRIANGLE;
    if (x == SND_X_MUSIC + SND_X_TRIANGLE) {
        mff_cycles(0x85BC, 1);
        triangle = 1;
    } else {
        mff_cycles(0x85BE, 4);
        c = x >= SND_X_TRIANGLE;
        triangle = x == SND_X_TRIANGLE;
        if (triangle) {
            mff_cycles(0x85C2, 19);
            a = RAM(SND_EFFECT_ROW_FRAMES);
            RAM(SND_PRODUCT_HI) = a;
            a = RAM(SND_VOLUME + x);
            NZ(a);
            RAM(SND_MULTIPLIER) = a;
            SYNC_OUT;
            if (!mff_call_at(0x05CD, 0x8006, 3)) return;
            SYNC_IN;
            mff_cycles(0x85CE, 3);
            y = RAM(SND_PRODUCT_HI);
        } else {
            mff_cycles(0x85C0, 1);
        }
    }
    if (triangle) {
        mff_cycles(0x85D0, 16);
        y++;
        RAM(SND_ENV + x) += 2;                  /* INC, INC: sustain */
    }
    mff_cycles(0x85D7, 19);
    a = y;
    RAM(SND_LEVEL + x) = a;
    a = mff_pull();
    y = a;
    NZ(y);
    SYNC_OUT;
    mff_rts();
}

/* snd_lfo_reset, $864C: channel X's LFO restarts: phase 0, quarter 0
 * ($0704 bits 6-7), and the "instrument changed" bit 3 is cleared.
 * Out: A = $0704,X, N, Z from it. */
static void snd_lfo_reset(void) {
    REGS_IN;
    mff_cycles(0x864C, 24);
    a = 0;
    RAM(SND_LFO + x) = a;
    a = RAM(SND_ENV + x) & (uint8_t)~(SND_ENV_NEW_INSTR | SND_ENV_LFO_ODD | SND_ENV_LFO_NEG);
    NZ(a);
    RAM(SND_ENV + x) = a;
    SYNC_OUT;
    mff_rts();
}

/* snd_note_pitch, $85DE: note A (clamped to 95) becomes channel X's pitch
 * from the period table: $8959/$895A + 2 * (note + 1), then
 * snd_set_pitch. A music channel also keeps the note + 1 as its target
 * ($071C,X) and, when the previous note differs and a portamento speed is
 * set, slides from it instead: the speed's sign from the direction, the
 * sliding bit on, the pitch starting at the old note. The same note again
 * on a tied note changes nothing (the LFO check, $8644).
 * Callers: snd_effect_note (JMP), snd_music_next. */
void func_85DE_b3(void) {
    REGS_IN;
    int plain;

    mff_cycles(0x85DE, 4);
    if (a >= 0x60) {
        mff_cycles(0x85E2, 2);
        a = 0x5F;
    } else {
        mff_cycles(0x85E0, 1);
    }
    mff_cycles(0x85E4, 12);
    RAM(SND_ARG) = a;
    RAM(SND_ARG)++;
    if (x < SND_X_MUSIC) {
        mff_cycles(0x85EA, 2);
    } else {
        mff_cycles(0x85EC, 6);
        a = RAM(SND_TARGET + x);
        plain = 1;
        if (a == 0) {
            mff_cycles(0x85EF, 2);
        } else {
            mff_cycles(0x85F1, 5);
            m = RAM(SND_ARG);
            c = a >= m;
            if (a == m) {
                mff_cycles(0x85F5, 6);
                a = RAM(SND_SEQ_FLAGS + x);
                n = a >> 7;
                if (n) {
                    mff_cycles(0x85FA, 2 + 2);
                    SYNC_OUT;
                    func_8644_b3();
                    return;
                }
                mff_cycles(0x85F8, 2);
            } else {
                mff_cycles(0x85F3, 1);
                mff_cycles(0x85FC, 6);
                a = RAM(SND_SLIDE_AMOUNT + x);
                if (a == 0) {
                    mff_cycles(0x85FF, 1);
                } else {
                    mff_cycles(0x8601, 2);
                    if (c) {
                        mff_cycles(0x8601, 1 + 2);
                        a &= 0x7F;              /* the old note was higher: down */
                    } else {
                        mff_cycles(0x8603, 4 + 1);
                        a |= 0x80;              /* up */
                    }
                    mff_cycles(0x8609, 28);
                    RAM(SND_SLIDE_AMOUNT + x) = a;
                    RAM(SND_ENV + x) |= SND_ENV_SLIDING;
                    a = RAM(SND_ARG);
                    y = RAM(SND_TARGET + x);
                    RAM(SND_ARG) = y;
                    plain = y == 0;             /* never: the target was not 0 */
                    if (!plain)
                        mff_cycles(0x861B, 1);
                }
            }
        }
        if (plain) {
            mff_cycles(0x861D, 14);
            RAM(SND_ENV + x) &= (uint8_t)~SND_ENV_SLIDING;
            a = RAM(SND_ARG);
        }
        mff_cycles(0x8627, 5);
        RAM(SND_TARGET + x) = a;
    }
    mff_cycles(0x862A, 12);
    c = RAM(SND_ARG) >> 7;
    RAM(SND_ARG) <<= 1;
    y = RAM(SND_ARG);
    a = prg_read((uint16_t)(SND_TBL_PERIOD_LO + y));
    mff_cycles(0x8631, 7);
    RAM(SND_ARG) = a;
    a = prg_read((uint16_t)(SND_TBL_PERIOD_HI + y));
    SYNC_OUT;
    func_8636_b3();
}

/* snd_set_pitch, $8636: channel X's pitch = A:$C3 (high:low; the noise
 * channel's high byte is its period index), then the LFO restarts if the
 * instrument wants it at every note (byte 4 bit 7) or just changed ($8644).
 * Callers: snd_effect_note and snd_music_next (the noise channel),
 * snd_note_pitch (fall through). */
void func_8636_b3(void) {
    REGS_IN;
    mff_cycles(0x8636, 20);
    RAM(SND_PITCH_HI + x) = a;
    a = RAM(SND_ARG);
    RAM(SND_PITCH_LO + x) = a;
    y = SND_INS_LFO;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x8642, 2);
    SYNC_OUT;
    if (a & 0x80) {
        mff_cycles(0x8642, 1);
        snd_lfo_reset();
    } else {
        func_8644_b3();
    }
}

/* the LFO check, $8644: snd_lfo_reset if channel X's instrument just
 * changed ($0704 bit 3).
 * Out: A = $0704,X & 8 or (reset) $0704,X; N, Z from it.
 * Callers: snd_effect_note (JMP, a held note), snd_music_next (a tied
 * note), snd_note_pitch (the same note tied), snd_set_pitch (fall
 * through). */
void func_8644_b3(void) {
    REGS_IN;
    mff_cycles(0x8644, 8);
    a = RAM(SND_ENV + x) & SND_ENV_NEW_INSTR;
    NZ(a);
    if (z) {
        mff_cycles(0x864B, 6);
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0x8649, 1);
    SYNC_OUT;
    snd_lfo_reset();
}

/* snd_set_volume, $865A: effect field 2 / music opcode $07: the volume,
 * $070C,X = (duty bits) | $30 | $C3. On the effect triangle a nonzero $C3
 * is stored as it is: its length ratio (snd_key_on).
 * Out: A = $070C,X, N, Z from it. */
void func_865A_b3(void) {
    REGS_IN;
    int ratio = 0;

    mff_cycles(0x865A, 4);
    c = x >= SND_X_TRIANGLE;
    if (x == SND_X_TRIANGLE) {
        mff_cycles(0x865E, 5);
        a = RAM(SND_ARG);
        NZ(a);
        if (!z) {
            mff_cycles(0x8660, 1);
            ratio = 1;
        }
    } else {
        mff_cycles(0x865C, 1);
    }
    if (!ratio) {
        mff_cycles(0x8662, 11);
        a = (RAM(SND_VOLUME + x) & SND_VOL_DUTY) | RAM(SND_ARG) | SND_CONST_VOLUME;
        NZ(a);
    }
    mff_cycles(0x866B, 11);
    RAM(SND_VOLUME + x) = a;
    SYNC_OUT;
    mff_rts();
}

/* snd_set_instrument, $866F: effect field 0 / music opcode $08 nn: the
 * instrument nn + 1. A change sets $0704 bit 3 (the LFO restarts at the
 * next note) and falls into snd_instrument_ptr.
 * Out: N, Z, C from the compare when it does not change. */
void func_866F_b3(void) {
    REGS_IN;
    mff_cycles(0x866F, 14);
    RAM(SND_ARG)++;
    a = RAM(SND_ARG);
    CMP(a, RAM(SND_INSTRUMENT + x));
    if (z) {
        mff_cycles(0x8676, 1 + 6);
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0x8678, 18);
    RAM(SND_INSTRUMENT + x) = a;
    y = a;
    RAM(SND_ENV + x) |= SND_ENV_NEW_INSTR;
    SYNC_OUT;
    func_8684_b3();
}

/* snd_instrument_ptr, $8684: $C5/$C6 = the instrument table ($8A41/$8A42)
 * + 8 * (Y - 1), instrument Y's 8 bytes.
 * Out: A = $C6, N, Z, C, V from its add; $C3 = (Y - 1) >> 5; Y - 1.
 * Callers: snd_effect_channel, snd_music_channel; snd_set_instrument (fall
 * through). */
void func_8684_b3(void) {
    REGS_IN;
    mff_cycles(0x8684, 36);
    y--;
    RAM(SND_ARG) = y >> 5;                      /* ASL A / ROL $C3, three times */
    a = (uint8_t)(y << 3);
    w = a + prg_read(SND_TBL_INSTR_LO);
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0x8697, 10);
    RAM(SND_INSTR_PTR) = a;
    a = RAM(SND_ARG);
    m = prg_read(SND_TBL_INSTR_HI);
    w = a + m + c;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    NZ(a);
    mff_cycles(0x869E, 9);
    RAM(SND_INSTR_PTR + 1) = a;
    SYNC_OUT;
    mff_rts();
}

/* snd_set_detune, $86A1: effect field 4 / music opcode $0C: the detune,
 * signed, added to the period. Out: A = $C3, N, Z from it. */
void func_86A1_b3(void) {
    REGS_IN;
    mff_cycles(0x86A1, 14);
    a = RAM(SND_ARG);
    NZ(a);
    RAM(SND_DETUNE + x) = a;
    SYNC_OUT;
    mff_rts();
}

/* snd_set_slide, $86A7: effect field 3 / music opcode $0D: the slide
 * (effect) or portamento (music) speed. Out: A = $C3, N, Z from it. */
void func_86A7_b3(void) {
    REGS_IN;
    mff_cycles(0x86A7, 14);
    a = RAM(SND_ARG);
    NZ(a);
    RAM(SND_SLIDE_AMOUNT + x) = a;
    SYNC_OUT;
    mff_rts();
}

/* snd_set_duty, $86AD: effect field 1 / music opcode $18: the duty,
 * $070C,X = $C3 (bits 6-7) | $30 | the volume.
 * Out: A = $070C,X, N, Z from it. */
void func_86AD_b3(void) {
    REGS_IN;
    mff_cycles(0x86AD, 22);
    a = (RAM(SND_VOLUME + x) & SND_VOL_LEVEL) | RAM(SND_ARG) | SND_CONST_VOLUME;
    NZ(a);
    RAM(SND_VOLUME + x) = a;
    SYNC_OUT;
    mff_rts();
}
