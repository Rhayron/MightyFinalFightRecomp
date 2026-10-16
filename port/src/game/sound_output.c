/*
 * sound_output.c - the envelope and the register output (bank $06
 * $86BA-$8914; docs/sound.md, "Instruments", "Pitch")
 *
 * Every frame each playing channel runs snd_envelope, which steps the
 * envelope phase in $0704,X and the level in $0710,X, then snd_output:
 * the volume register (level, tremolo, fade), the period registers (pitch
 * with vibrato, converted from the pseudo-float, plus the detune), and the
 * slide and LFO that move the pitch for the next frame. A music channel the
 * effect owns writes nothing and only slides.
 *
 * The 6502 has one copy of the output path, from $871A/$871D/$8720 down to
 * the RTS at $8914, which every envelope phase falls into; here the phases
 * call it. snd_output's internal labels ($8740 ... $8814) are static
 * functions, entered with the registers in g_cpu like the routines.
 */
#include "sound_driver.h"

/* $C1/$C2 hold the pitch, then the period, high byte first */
#define PERIOD_HI SND_TMP
#define PERIOD_LO (SND_TMP + 1)

static void snd_set_level(void);
static void snd_output_fade(void);
static void snd_output_triangle(void);
static void snd_output_volume(int load_level);
static void snd_output_regs(void);
static void snd_output_period(void);

/* snd_envelope, $86BA: channel X's envelope phase ($0704,X bits 0-2)
 * through the table at $86C7, with the level in $C4: 0 snd_env_attack,
 * 1 snd_env_decay, 2 sustain (snd_output), 3 snd_env_release, 4 off ($8914:
 * nothing at all, not even the slide).
 * Callers: snd_effect_channel, snd_music_channel (instrument set). */
void func_86BA_b3(void) {
    REGS_IN;
    mff_cycles(0x86BA, 19);
    a = RAM(SND_LEVEL + x);
    RAM(SND_TMP + 3) = a;
    a = RAM(SND_ENV + x) & SND_ENV_PHASE;
    NZ(a);
    SYNC_OUT;
    if (!mff_call_at(0x06C6, 0x8023, 3)) return;
    mff_unreachable(SND_TBL_ENVELOPE);          /* the JSR never comes back */
}

/* snd_env_attack, $86D1: the level rises by the rate of instrument byte 0
 * (table $8933); at $F0 or above it is $F0 and the decay starts ($871A).
 * In: $C4 = the level. */
void func_86D1_b3(void) {
    REGS_IN;
    mff_cycles(0x86D1, 7);
    y = SND_INS_ATTACK;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x86D5, 11);
    y = a;
    a = RAM(SND_TMP + 3);
    m = prg_read((uint16_t)(SND_TBL_RATE + y));
    w = a + m;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    mff_cycles(0x86DC, 2);
    if (!c) {
        mff_cycles(0x86DE, 4);
        c = a >= 0xF0;
        if (!c) {
            mff_cycles(0x86E0, 2);
            SYNC_OUT;
            snd_set_level();
            return;
        }
    } else {
        mff_cycles(0x86DC, 1);
    }
    mff_cycles(0x86E2, 4 + 2);
    a = 0xF0;
    SYNC_OUT;
    func_871A_b3();
}

/* snd_env_decay, $86E6: the level falls by the rate of instrument byte 1
 * down to the sustain level (byte 2); there, or with no decay step, it is
 * the sustain level and phase 2 starts ($871A, JMP).
 * In: $C4 = the level. */
void func_86E6_b3(void) {
    REGS_IN;
    mff_cycles(0x86E6, 7);
    y = SND_INS_DECAY;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x86EA, 2);
    if (a == 0) {
        mff_cycles(0x86EA, 1);
    } else {
        mff_cycles(0x86EC, 11);
        y = a;
        a = RAM(SND_TMP + 3);
        m = prg_read((uint16_t)(SND_TBL_RATE + y));
        w = a - m;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        c = w >= 0;
        a = (uint8_t)w;
        mff_cycles(0x86F3, 2);
        if (!c) {
            mff_cycles(0x86F3, 1);
        } else {
            mff_cycles(0x86F5, 7);
            y = SND_INS_SUSTAIN;
            m = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
            c = a >= m;
            mff_cycles(0x86F9, 2);
            if (c) {
                mff_cycles(0x86F9, 2);
                SYNC_OUT;
                snd_set_level();
                return;
            }
        }
    }
    mff_cycles(0x86FB, 7);
    y = SND_INS_SUSTAIN;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    NZ(a);
    mff_cycles(0x86FF, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x871A, 2);
    call_by_address_tail(0x871A, 3);
}

/* snd_env_release, $8702: the level falls by the rate of instrument byte
 * 3 (none: it holds) down to 0, then phase 4 (off). The triangles go to 0
 * at once.
 * In: $C4 = the level. */
void func_8702_b3(void) {
    REGS_IN;
    mff_cycles(0x8702, 8);
    a = x & 3;
    if (a != SND_X_TRIANGLE) {
        mff_cycles(0x8709, 7);
        y = SND_INS_RELEASE;
        a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
        mff_cycles(0x870D, 2);
        if (a == 0) {
            mff_cycles(0x870D, 1);
            SYNC_OUT;
            func_8720_b3();
            return;
        }
        mff_cycles(0x870F, 11);
        y = a;
        a = RAM(SND_TMP + 3);
        m = prg_read((uint16_t)(SND_TBL_RATE + y));
        w = a - m;
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        mff_cycles(0x8716, 2);
        if (w >= 0) {
            mff_cycles(0x8716, 1);
            SYNC_OUT;
            snd_set_level();
            return;
        }
    } else {
        mff_cycles(0x8707, 1);
    }
    mff_cycles(0x8718, 2);
    a = 0;
    SYNC_OUT;
    func_871A_b3();
}

/* the next phase, $871A: $0704,X + 1 (attack to decay, decay to sustain,
 * release to off), then the level A is stored ($871D) and output.
 * Callers: snd_env_decay (JMP); snd_env_attack and snd_env_release (fall
 * through). */
void func_871A_b3(void) {
    REGS_IN;
    mff_cycles(0x871A, 7);
    RAM(SND_ENV + x)++;
    SYNC_OUT;
    snd_set_level();
}

/* $871D: the level A into $0710,X, then snd_output */
static void snd_set_level(void) {
    REGS_IN;
    mff_cycles(0x871D, 5);
    RAM(SND_LEVEL + x) = a;
    SYNC_OUT;
    func_8720_b3();
}

/* snd_output, $8720: channel X's registers for this frame (the sustain
 * phase comes straight here). A music channel the effect owns ($CF bit 7)
 * writes nothing: JMP snd_slide. A fade in progress takes $8740; else the
 * triangle ($8752) or the volume (snd_output_volume, $8760). */
void func_8720_b3(void) {
    REGS_IN;
    mff_cycles(0x8720, 4);
    c = x >= SND_X_MUSIC;
    if (c) {
        mff_cycles(0x8724, 5);
        a = RAM(SND_EFFECT_CHANNELS);
        NZ(a);
        if (n) {
            mff_cycles(0x8728, 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x88A0, 2);
            call_by_address_tail(0x88A0, 3);
            return;
        }
        mff_cycles(0x8726, 1);
        mff_cycles(0x872B, 8);
        a = RAM(SND_FADE_LEVEL);
        y = RAM(SND_FADE_SPEED);
        if (!(y & SND_FADE_IN)) {
            mff_cycles(0x8731, 2);
            a ^= 0xFF;
        } else {
            mff_cycles(0x872F, 1);
        }
        mff_cycles(0x8733, 4);
        if (a != 0xFF) {
            mff_cycles(0x8735, 1);
            SYNC_OUT;
            snd_output_fade();
            return;
        }
    } else {
        mff_cycles(0x8722, 1);
    }
    mff_cycles(0x8737, 8);
    a = x & 3;
    c = a >= SND_X_TRIANGLE;
    z = a == SND_X_TRIANGLE;
    SYNC_OUT;
    if (!z) {
        mff_cycles(0x873C, 1);
        snd_output_volume(1);
    } else {
        mff_cycles(0x873E, 2 + 1);
        snd_output_triangle();
    }
}

/* the fade, $8740 ($F5/$F6, never used), A = the fade level: the music
 * triangle sounds while its gate counter * the level >> 8 is not 0; the
 * others' level is at most the fade level ($875B). */
static void snd_output_fade(void) {
    REGS_IN;
    mff_cycles(0x8740, 4);
    c = x >= SND_X_MUSIC + SND_X_TRIANGLE;
    if (x != SND_X_MUSIC + SND_X_TRIANGLE) {
        mff_cycles(0x8742, 1);
        mff_cycles(0x875B, 6);
        SYNC_OUT;
        if (a < RAM(SND_LEVEL + x)) {
            mff_cycles(0x875E, 1);
            snd_output_volume(0);
        } else {
            snd_output_volume(1);
        }
        return;
    }
    mff_cycles(0x8744, 16);
    RAM(SND_MULTIPLIER) = a;
    a = RAM(SND_SEQ_GATE + x);
    NZ(a);
    RAM(SND_PRODUCT_HI) = a;
    SYNC_OUT;
    if (!mff_call_at(0x074D, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x874E, 5);
    a = RAM(SND_PRODUCT_HI);
    SYNC_OUT;
    if (a == 0) {
        mff_cycles(0x8750, 1);
        snd_output_regs();
    } else {
        snd_output_triangle();
    }
}

/* the triangle, $8752: $4008 = $FF (linear counter on) while the level is
 * not 0, $00 otherwise */
static void snd_output_triangle(void) {
    REGS_IN;
    mff_cycles(0x8752, 6);
    a = RAM(SND_LEVEL + x);
    if (a == 0) {
        mff_cycles(0x8755, 1);
    } else {
        mff_cycles(0x8757, 4 + 1);
        a = 0xFF;
    }
    SYNC_OUT;
    snd_output_regs();
}

/* snd_output_volume, $8760: the level $0710,X ($8763: A, from the fade)
 * becomes the attenuation 15 - (level >> 4) in $C3. The tremolo
 * (instrument byte 6, from 5 up) can deepen it to (LFO * depth) >> 10,
 * read as a triangle wave (255 - phase in the odd quarters); 16 or more
 * mutes. The register gets $070C,X - attenuation, or its duty bits alone
 * when that borrows into bit 4 (BIT $C4, $C4 = $10). */
static void snd_output_volume(int load_level) {
    REGS_IN;
    int mute = 0;

    if (load_level) {
        mff_cycles(0x8760, 4);
        a = RAM(SND_LEVEL + x);
    }
    mff_cycles(0x8763, 20);
    a = (uint8_t)((a >> 4) ^ 0x0F);
    RAM(SND_ARG) = a;
    y = SND_INS_TREMOLO;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x876F, 4);
    if (a < 5) {
        mff_cycles(0x8771, 1);
    } else {
        mff_cycles(0x8773, 19);
        RAM(SND_MULTIPLIER) = a;
        y = RAM(SND_LFO + x);
        c = (RAM(SND_ENV + x) >> 6) & 1;        /* ASL, ASL: an odd quarter */
        a = y;
        NZ(a);
        if (c) {
            mff_cycles(0x8780, 2);
            a ^= 0xFF;
            NZ(a);
        } else {
            mff_cycles(0x877E, 1);
        }
        mff_cycles(0x8782, 2);
        if (z) {
            mff_cycles(0x8782, 1);
        } else {
            mff_cycles(0x8784, 9);
            RAM(SND_PRODUCT_HI) = a;
            SYNC_OUT;
            if (!mff_call_at(0x0788, 0x8006, 3)) return;
            SYNC_IN;
            mff_cycles(0x8789, 11);
            a = RAM(SND_PRODUCT_HI) >> 2;
            c = a >= 0x10;
            if (c) {
                mff_cycles(0x878F, 1);
                mute = 1;
            } else {
                mff_cycles(0x8791, 5);
                if (a >= RAM(SND_ARG)) {
                    mff_cycles(0x8795, 3);
                    RAM(SND_ARG) = a;
                } else {
                    mff_cycles(0x8793, 1);
                }
            }
        }
    }
    if (!mute) {
        mff_cycles(0x8797, 19);
        a = 0x10;
        RAM(SND_TMP + 3) = a;
        a = RAM(SND_VOLUME + x);
        m = RAM(SND_ARG);
        w = a - m;
        c = w >= 0;
        a = (uint8_t)w;
        m = RAM(SND_TMP + 3);
        v = (m >> 6) & 1;
        if ((a & m) != 0)
            mff_cycles(0x87A3, 1);
        else
            mute = 1;
    }
    if (mute) {
        mff_cycles(0x87A5, 6);
        a = RAM(SND_VOLUME + x) & 0xF0;
    }
    SYNC_OUT;
    snd_output_regs();
}

/* snd_output_regs, $87AA: A into the volume register, then the pitch
 * with the vibrato (instrument byte 5; not in the frame of a key on, when
 * $077C is $FF): LFO * depth >> 4, read as a triangle wave, added in
 * quarters 0-1 and subtracted in 2-3. An add whose high byte comes out 0
 * falls into the subtraction with $C2 already the sum (docs/sound.md,
 * "Quirks"); a result with high byte 0 is dropped. Y:$C2 = the pitch. */
static void snd_output_regs(void) {
    REGS_IN;
    int base = 1;
    int i;

    mff_cycles(0x87AA, 8);
    y = SND_REG_VOLUME;
    NZ(y);
    SYNC_OUT;
    if (!mff_call_at(0x07AE, 0x80EC, 3)) return;
    SYNC_IN;
    mff_cycles(0x87AF, 12);
    a = x & 3;
    y = a;
    a = RAM(SND_PERIOD_HI_CACHE + y);
    if (a & 0x80) {
        mff_cycles(0x87B6, 2);
    } else {
        mff_cycles(0x87B8, 7);
        y = SND_INS_VIBRATO;
        a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
        mff_cycles(0x87BC, 2);
        if (a == 0) {
            mff_cycles(0x87BC, 2);
        } else {
            mff_cycles(0x87BE, 19);
            RAM(SND_MULTIPLIER) = a;
            y = RAM(SND_LFO + x);
            c = (RAM(SND_ENV + x) >> 6) & 1;
            a = y;
            NZ(a);
            if (c) {
                mff_cycles(0x87CB, 2);
                a ^= 0xFF;
                NZ(a);
            } else {
                mff_cycles(0x87C9, 1);
            }
            mff_cycles(0x87CD, 2);
            if (z) {
                mff_cycles(0x87CD, 2);
            } else {
                mff_cycles(0x87CF, 9);
                RAM(SND_PRODUCT_HI) = a;
                SYNC_OUT;
                if (!mff_call_at(0x07D3, 0x8006, 3)) return;
                SYNC_IN;
                mff_cycles(0x87D4, 38);
                a = RAM(SND_PRODUCT_HI);
                for (i = 0; i < 4; i++) {           /* LSR A / ROR $C2: >> 4 */
                    c = a & 1;
                    a >>= 1;
                    RAM(SND_PRODUCT_LO) = (uint8_t)(RAM(SND_PRODUCT_LO) >> 1 | c << 7);
                }
                y = a;
                a |= RAM(SND_PRODUCT_LO);
                if (a == 0) {
                    mff_cycles(0x87E5, 2);
                } else {
                    int subtract;

                    mff_cycles(0x87E7, 6);
                    a = RAM(SND_ENV + x);
                    subtract = (a & SND_ENV_LFO_NEG) != 0;
                    if (subtract) {
                        mff_cycles(0x87EA, 1);
                    } else {
                        mff_cycles(0x87EC, 20);
                        w = RAM(PERIOD_LO) + RAM(SND_PITCH_LO + x);
                        c = w > 0xFF;
                        RAM(PERIOD_LO) = (uint8_t)w;
                        a = y;
                        m = RAM(SND_PITCH_HI + x);
                        w = a + m + c;
                        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
                        a = (uint8_t)w;
                        if (a != 0)
                            mff_cycles(0x87F8, 2);
                        else
                            subtract = 1;
                    }
                    if (subtract) {
                        mff_cycles(0x87FA, 22);
                        a = RAM(SND_PITCH_LO + x);
                        m = RAM(PERIOD_LO);
                        w = a - m;
                        c = w >= 0;
                        a = (uint8_t)w;
                        RAM(PERIOD_LO) = a;
                        a = RAM(SND_PITCH_HI + x);
                        RAM(PERIOD_HI) = y;
                        m = RAM(PERIOD_HI);
                        w = a - m - !c;
                        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
                        a = (uint8_t)w;
                    }
                    mff_cycles(0x8809, 4);
                    y = a;
                    if (y != 0) {
                        mff_cycles(0x880A, 1);
                        base = 0;
                    }
                }
            }
        }
    }
    if (base) {
        mff_cycles(0x880C, 11);
        a = RAM(SND_PITCH_LO + x);
        RAM(PERIOD_LO) = a;
        y = RAM(SND_PITCH_HI + x);
    }
    SYNC_OUT;
    snd_output_period();
}

/* $8814: Y:$C2 = the pitch. On an effect channel of a looping effect the
 * pitch scale ($D8, command $F7, never used) adds pitch * scale >> 8.
 * The noise channel's period is (high & 15) | instrument byte 7 ($400E),
 * high byte 0. The others convert the pseudo-float (snd_float_to_period,
 * $8849): octave = the last threshold ($8953) at or below the high byte,
 * period = ((r + 7) * 256 + low) >> (7 - octave), plus the detune. Then the
 * period's low byte, the high byte (| $08) only when it changed ($077C),
 * and snd_slide. */
static void snd_output_period(void) {
    REGS_IN;
    mff_cycles(0x8814, 4);
    c = x >= SND_X_MUSIC;
    if (!c) {
        mff_cycles(0x8818, 5);
        a = RAM(SND_EFFECT_LOOP);
        if (a & 0x80) {
            mff_cycles(0x881C, 5);
            a = RAM(SND_EFFECT_PITCH_SCALE);
            if (a != 0) {
                mff_cycles(0x8820, 18);
                RAM(SND_MULTIPLIER) = a;
                RAM(SND_PRODUCT_HI) = y;
                a = RAM(SND_PRODUCT_LO);
                NZ(a);
                mff_push(a);
                SYNC_OUT;
                if (!mff_call_at(0x0829, 0x8006, 3)) return;
                SYNC_IN;
                mff_cycles(0x882A, 19);
                a = mff_pull();
                w = a + RAM(PERIOD_LO);
                c = w > 0xFF;
                RAM(PERIOD_LO) = (uint8_t)w;
                a = 0;
                m = RAM(PERIOD_HI);
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
    a = x & 3;
    if (a == SND_X_NOISE) {
        mff_cycles(0x883A, 11);
        a = y & 0x0F;
        y = SND_INS_NOISE;
        a |= mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
        mff_cycles(0x8841, 10);
        RAM(PERIOD_LO) = a;
        a = 0;
        RAM(PERIOD_HI) = a;
        mff_cycles(0x8847, 1);
    } else {
        mff_cycles(0x8838, 1);
        mff_cycles(0x8849, 4);
        a = y;
        y = 8;
        for (;;) {
            mff_cycles(0x884C, 6);
            y--;
            mff_cycles(0x8850, 2);
            if (a >= prg_read((uint16_t)(SND_TBL_OCT_THRESH + y)))
                break;
            mff_cycles(0x884C, 1);
        }
        mff_cycles(0x8852, 29);
        RAM(PERIOD_HI) = a;
        a = (uint8_t)(y + RAM(PERIOD_HI));      /* 8 * octave + r */
        y = a;
        a &= 7;
        m = 7;
        w = a + m;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(PERIOD_HI) = a;                     /* r + 7 */
        a = (y & 0x38) ^ 0x38;                  /* 8 * (7 - octave) */
        if (a != 0) {
            for (;;) {
                mff_cycles(0x8867, 16);
                c = RAM(PERIOD_HI) & 1;
                RAM(PERIOD_HI) >>= 1;
                RAM(PERIOD_LO) = (uint8_t)(RAM(PERIOD_LO) >> 1 | c << 7);
                m = 8;
                w = a - m;
                v = ((a ^ m) & (a ^ w) & 0x80) != 0;
                c = w >= 0;
                a = (uint8_t)w;
                if (a == 0)
                    break;
                mff_cycles(0x8867, 1);
            }
        } else {
            mff_cycles(0x8865, 1);
        }

        mff_cycles(0x8870, 8);
        y = 0;
        a = RAM(SND_DETUNE + x);
        n = a >> 7;
        if (a != 0) {
            mff_cycles(0x8877, 2);
            if (n) {
                mff_cycles(0x8879, 2);
                y--;
            } else {
                mff_cycles(0x8877, 1);
            }
            mff_cycles(0x887A, 16);
            m = RAM(PERIOD_LO);
            w = a + m;
            c = w > 0xFF;
            a = (uint8_t)w;
            RAM(PERIOD_LO) = a;
            a = y;
            m = RAM(PERIOD_HI);
            w = a + m + c;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            RAM(PERIOD_HI) = a;
        } else {
            mff_cycles(0x8875, 1);
        }
    }

    mff_cycles(0x8884, 11);
    y = SND_REG_PERIOD_LO;
    a = RAM(PERIOD_LO);
    NZ(a);
    SYNC_OUT;
    if (!mff_call_at(0x088A, 0x80EC, 3)) return;
    SYNC_IN;
    mff_cycles(0x888B, 15);
    a = x & 3;
    y = a;
    a = RAM(PERIOD_HI);
    m = RAM(SND_PERIOD_HI_CACHE + y);
    c = a >= m;
    if (a != m) {
        mff_cycles(0x8896, 15);
        RAM(SND_PERIOD_HI_CACHE + y) = a;
        a |= SND_PERIOD_HI_LEN;
        y = SND_REG_PERIOD_HI;
        NZ(y);
        SYNC_OUT;
        if (!mff_call_at(0x089F, 0x80EC, 3)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x8894, 1);
    }
    SYNC_OUT;
    func_88A0_b3();
}

/* snd_slide, $88A0: while $0704,X bit 5 is set, the pitch moves by twice
 * the speed $0718,X (bit 7 the sign) every frame; when it reaches or
 * passes the target note ($071C,X) it snaps to it and stops (never on the
 * effect noise channel, X = 0). A speed of 0 stops it. Then snd_lfo_step
 * ($88FA): $0708,X += instrument byte 4 & $7F, and each overflow advances
 * the LFO quarter ($0704 bits 6-7).
 * Out: from the last add (A, N, Z, C, V), or N, Z from the LFO speed.
 * Callers: snd_output (JMP, the channel is the effect's; fall through at
 * the end). */
void func_88A0_b3(void) {
    REGS_IN;
    mff_cycles(0x88A0, 8);
    a = RAM(SND_ENV + x) & SND_ENV_SLIDING;
    if (a != 0) {
        int stop = 1;

        mff_cycles(0x88A7, 6);
        a = RAM(SND_SLIDE_AMOUNT + x);
        if (a == 0) {
            mff_cycles(0x88AA, 1);
        } else {
            mff_cycles(0x88AC, 9);
            y = 0;
            c = a >> 7;
            a = (uint8_t)(a << 1);
            NZ(a);
            FLAGS_OUT;
            mff_push(mff_p());                  /* PHP: the sign in C */
            if (c) {
                mff_cycles(0x88B2, 8);
                a = (uint8_t)((a ^ 0xFF) + 1);
                y--;
            } else {
                mff_cycles(0x88B0, 1);
            }
            mff_cycles(0x88B8, 40);
            w = a + RAM(SND_PITCH_LO + x);
            c = w > 0xFF;
            RAM(SND_PITCH_LO + x) = (uint8_t)w;
            RAM(SND_PITCH_HI + x) = (uint8_t)(y + RAM(SND_PITCH_HI + x) + c);
            /* the pitch against the target's: the carry says which side */
            a = (uint8_t)(RAM(SND_TARGET + x) << 1);
            y = a;
            a = RAM(SND_PITCH_LO + x);
            m = prg_read((uint16_t)(SND_TBL_PERIOD_LO + y));
            w = a - m;
            c = w >= 0;
            mff_cycles(0x88D2, 10);
            a = RAM(SND_PITCH_HI + x) & 0x3F;
            m = prg_read((uint16_t)(SND_TBL_PERIOD_HI + y));
            w = a - m - !c;
            c = w >= 0;
            mff_cycles(0x88DA, 12);
            a = (uint8_t)(0xFF + c);
            mff_set_p(mff_pull());
            FLAGS_IN;
            m = 0;
            w = a + m + c;                      /* 0 once it got there */
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            if (a != 0) {
                mff_cycles(0x88E1, 1);
                stop = 0;
            } else {
                mff_cycles(0x88E3, 4);
                a = x;
                if (a == SND_X_NOISE) {
                    mff_cycles(0x88E4, 1);
                    stop = 0;
                } else {
                    mff_cycles(0x88E6, 4);
                    a = prg_read((uint16_t)(SND_TBL_PERIOD_LO + y));
                    mff_cycles(0x88E9, 9);
                    RAM(SND_PITCH_LO + x) = a;
                    a = prg_read((uint16_t)(SND_TBL_PERIOD_HI + y));
                    mff_cycles(0x88EF, 5);
                    RAM(SND_PITCH_HI + x) = a;
                }
            }
        }
        if (stop) {
            mff_cycles(0x88F2, 11);
            a = RAM(SND_ENV + x) & (uint8_t)~SND_ENV_SLIDING;
            RAM(SND_ENV + x) = a;
        }
    } else {
        mff_cycles(0x88A5, 1);
    }

    /* snd_lfo_step */
    mff_cycles(0x88FA, 7);
    y = SND_INS_LFO;
    a = mem_read((uint16_t)(RAM16(SND_INSTR_PTR) + y));
    mff_cycles(0x88FE, 4);
    a &= 0x7F;
    NZ(a);
    if (!z) {
        mff_cycles(0x8902, 13);
        m = RAM(SND_LFO + x);
        w = a + m;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        NZ(a);
        RAM(SND_LFO + x) = a;
        if (c) {
            mff_cycles(0x890B, 13);
            a = RAM(SND_ENV + x);
            m = SND_ENV_LFO_STEP;
            w = a + m;
            v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            NZ(a);
            RAM(SND_ENV + x) = a;
        } else {
            mff_cycles(0x8909, 1);
        }
    } else {
        mff_cycles(0x8900, 1);
    }
    mff_cycles(0x8914, 6);
    SYNC_OUT;
    mff_rts();
}

/* the envelope's phase 4 (off), $8914: RTS */
void func_8914_b3(void) {
    mff_cycles(0x8914, 6);
    mff_rts();
}
