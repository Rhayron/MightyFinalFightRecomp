/*
 * sound_effect.c - the sound effect sequencer (bank $06 $8252-$8392;
 * docs/sound.md, "Music and effects sharing the channels", "Effect data")
 *
 * One effect plays at a time, from $D0/$D1. It is a list of rows: a
 * header, optional loop/gate/transpose bytes, the row's length in frames,
 * the mask of the channels it owns, and for each of them a field mask, the
 * fields and a note. Rows are read at the start of the frame
 * (snd_effect_step); each owned channel parses its entry in the same frame
 * and writes its registers from the next one on (snd_effect_channel). The
 * effect's channels use X = 0-3 in the per-channel arrays.
 */
#include "sound_driver.h"

/* snd_effect_step, $8252: counts the current row down ($D3; $D5, the frames
 * until the notes release, with it) and on its last frame reads the next
 * row (snd_effect_row).
 * Out: after the count, N, Z from $D5; else from snd_effect_row.
 * Caller: snd_tick_body, while $D0/$D1 is not 0. */
void func_8252_b3(void) {
    REGS_IN;
    mff_cycles(0x8252, 5);
    a = RAM(SND_EFFECT_ROW_FRAMES);
    NZ(a);
    if (!z) {
        mff_cycles(0x8256, 16);
        RAM(SND_EFFECT_ROW_FRAMES)--;
        RAM(SND_EFFECT_GATE_LEFT)--;
        NZ(RAM(SND_EFFECT_GATE_LEFT));
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0x8254, 1);
    SYNC_OUT;
    func_825B_b3();
}

/* snd_effect_row, $825B: reads the effect's next row:
 *  - header bit 7: the effect ends ($CE = 0); a looping one (none in the
 *    game) is started again (snd_start with $D7 >> 1) and its first row
 *    read, the others go to snd_effect_stop;
 *  - bit 0: cc hh ll, jump to hhll cc times ($D6 bits 0-6 count the
 *    passes, bit 7 keeps the loop flag; cc = 0 jumps always) and read the
 *    row there;
 *  - bit 1: the gate ratio $D4; bit 2: the transpose $D2 (unused);
 *  - nn: the row lasts nn + 1 frames ($D3 = nn); its notes release after
 *    ($D3 * $D4 >> 8) + 1 frames ($D5);
 *  - mm: the channels it owns. $C0 bit 0 is set (INC: the channels parse
 *    their entries this frame) and, if the mask changed, the channels that
 *    changed hands are given back (snd_release_changed). $CF = mm.
 * Out: A = $CF (N, Z from it).
 * Callers: snd_effect_step (fall through), and itself after a relaunch
 * (JMP $825B, a loop here). */
void func_825B_b3(void) {
    REGS_IN;

    for (;;) {
        int jump;

        mff_cycles(0x825B, 6);
        SYNC_OUT;
        if (!mff_call_at(0x025D, 0x8386, 3)) return;
        SYNC_IN;
        mff_cycles(0x825E, 7);
        RAM(SND_TMP + 3) = a;                   /* the header */
        c = a >> 7;
        a = (uint8_t)(a << 1);
        if (c) {
            /* the end of the effect: Y = 0 from snd_read */
            mff_cycles(0x8263, 10);
            RAM(SND_EFFECT_PRIORITY) = y;
            a = RAM(SND_EFFECT_RELAUNCH);
            c = a & 1;
            a >>= 1;
            NZ(a);
            if (c) {
                mff_cycles(0x826A, 6);
                SYNC_OUT;
                if (!mff_call_at(0x026C, 0x8118, 3)) return;
                SYNC_IN;
                mff_cycles(0x826D, 3);
                nes_cpu_instruction_boundary(0x825B, 2);
                continue;
            }
            mff_cycles(0x8268, 1 + 3);
            SYNC_OUT;
            nes_cpu_instruction_boundary(0x81C8, 2);
            call_by_address_tail(0x81C8, 3);    /* snd_effect_stop */
            return;
        }
        mff_cycles(0x8261, 1);

        mff_cycles(0x8273, 7);
        c = RAM(SND_TMP + 3) & 1;
        RAM(SND_TMP + 3) >>= 1;
        NZ(RAM(SND_TMP + 3));
        if (!c) {
            mff_cycles(0x8275, 1);
            break;
        }

        /* bit 0: the loop. 2 * cc is compared with the counter shifted out
         * of $D6 (the loop flag goes to C, kept by PHP) */
        mff_cycles(0x8277, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0279, 0x8386, 3)) return;
        SYNC_IN;
        mff_cycles(0x827A, 4);
        c = a >> 7;
        a = (uint8_t)(a << 1);
        NZ(a);
        if (z) {
            mff_cycles(0x827B, 1);
            jump = 1;
        } else {
            mff_cycles(0x827D, 13);
            c = RAM(SND_EFFECT_LOOP) >> 7;
            RAM(SND_EFFECT_LOOP) <<= 1;
            NZ(RAM(SND_EFFECT_LOOP));
            FLAGS_OUT;
            mff_push(mff_p());
            jump = a != RAM(SND_EFFECT_LOOP);
            if (jump) {
                /* one more pass: the flag back in bit 7, the counter + 1 */
                mff_cycles(0x8284, 14);
                mff_set_p(mff_pull());
                FLAGS_IN;
                m = RAM(SND_EFFECT_LOOP) & 1;
                RAM(SND_EFFECT_LOOP) = (uint8_t)(RAM(SND_EFFECT_LOOP) >> 1 | c << 7);
                c = m;
                RAM(SND_EFFECT_LOOP)++;
                NZ(RAM(SND_EFFECT_LOOP));
            } else {
                mff_cycles(0x8282, 1);
            }
        }
        if (jump) {
            mff_cycles(0x8289, 6);
            SYNC_OUT;
            if (!mff_call_at(0x028B, 0x8386, 3)) return;
            SYNC_IN;
            mff_cycles(0x828C, 8);
            x = a;
            NZ(x);
            SYNC_OUT;
            if (!mff_call_at(0x028F, 0x8386, 3)) return;
            SYNC_IN;
            mff_cycles(0x8290, 8);
            RAM(SND_EFFECT_PTR) = a;
            RAM(SND_EFFECT_PTR + 1) = x;
            if (!z) {
                mff_cycles(0x825B, 1);
                continue;
            }
            /* a target at $xx00 falls into the code below, which pulls a
             * status byte nobody pushed (docs/sound.md, "Quirks") */
        }

        /* the passes are done ($8296): the counter cleared, the flag back,
         * and the address skipped */
        mff_cycles(0x8296, 23);
        a = y;
        mff_set_p(mff_pull());
        FLAGS_IN;
        a = (uint8_t)(a >> 1 | c << 7);
        RAM(SND_EFFECT_LOOP) = a;
        a = 0x02;
        m = RAM(SND_EFFECT_PTR);
        w = a + m;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        RAM(SND_EFFECT_PTR) = a;
        if (c) {
            mff_cycles(0x82A4, 5);
            RAM(SND_EFFECT_PTR + 1)++;
        } else {
            mff_cycles(0x82A2, 1);
        }
        break;
    }

    /* bit 1: the gate ratio */
    mff_cycles(0x82A6, 7);
    c = RAM(SND_TMP + 3) & 1;
    RAM(SND_TMP + 3) >>= 1;
    NZ(RAM(SND_TMP + 3));
    if (c) {
        mff_cycles(0x82AA, 6);
        SYNC_OUT;
        if (!mff_call_at(0x02AC, 0x8386, 3)) return;
        SYNC_IN;
        mff_cycles(0x82AD, 3);
        RAM(SND_EFFECT_GATE) = a;
    } else {
        mff_cycles(0x82A8, 1);
    }

    /* bit 2: the transpose */
    mff_cycles(0x82AF, 7);
    c = RAM(SND_TMP + 3) & 1;
    RAM(SND_TMP + 3) >>= 1;
    NZ(RAM(SND_TMP + 3));
    if (c) {
        mff_cycles(0x82B3, 6);
        SYNC_OUT;
        if (!mff_call_at(0x02B5, 0x8386, 3)) return;
        SYNC_IN;
        mff_cycles(0x82B6, 3);
        RAM(SND_EFFECT_TRANSPOSE) = a;
    } else {
        mff_cycles(0x82B1, 1);
    }

    /* the length and the release */
    mff_cycles(0x82B8, 6);
    SYNC_OUT;
    if (!mff_call_at(0x02BA, 0x8386, 3)) return;
    SYNC_IN;
    mff_cycles(0x82BB, 18);
    RAM(SND_EFFECT_ROW_FRAMES) = a;
    RAM(SND_PRODUCT_HI) = a;
    a = RAM(SND_EFFECT_GATE);
    NZ(a);
    RAM(SND_MULTIPLIER) = a;
    SYNC_OUT;
    if (!mff_call_at(0x02C5, 0x8006, 3)) return;
    SYNC_IN;
    mff_cycles(0x82C6, 19);
    y = (uint8_t)(RAM(SND_PRODUCT_HI) + 1);
    RAM(SND_EFFECT_GATE_LEFT) = y;
    RAM(SND_FLAGS)++;
    NZ(RAM(SND_FLAGS));

    /* the channel mask */
    SYNC_OUT;
    if (!mff_call_at(0x02CF, 0x8386, 3)) return;
    SYNC_IN;
    mff_cycles(0x82D0, 8);
    mff_push(a);
    a ^= RAM(SND_EFFECT_CHANNELS);
    NZ(a);
    if (!z) {
        mff_cycles(0x82D5, 9);
        RAM(SND_EFFECT_CHANNELS) = a;
        SYNC_OUT;
        if (!mff_call_at(0x02D9, 0x81D4, 3)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x82D3, 1);
    }
    mff_cycles(0x82DA, 13);
    a = mff_pull();
    NZ(a);
    RAM(SND_EFFECT_CHANNELS) = a;
    SYNC_OUT;
    mff_rts();
}

/* snd_effect_channel, $82DE: effect channel X (0-3), once a frame while
 * the effect owns it. Points $C5/$C6 at its instrument (if any), then:
 *  - a frame without a new row: envelope and output (snd_envelope); when
 *    the row's gate runs out ($D5 = 0; the triangle: its own frame count
 *    in $0710), key off, unless the envelope is already off;
 *  - in the frame a row was read ($C0 bit 0): parses the channel's entry
 *    (06:830A): the field mask, each field through the table at $8329
 *    (snd_effect_field: 0 instrument, 1 duty, 2 volume, 3 slide speed,
 *    4 detune; the byte in $C3, the field number in $C4), then the note
 *    (snd_effect_note).
 * Out: from the routine it ends in.
 * Caller: snd_tick_body, with $CF bit 7 set. */
void func_82DE_b3(void) {
    REGS_IN;
    mff_cycles(0x82DE, 6);
    y = RAM(SND_INSTRUMENT + x);
    NZ(y);
    if (!z) {
        mff_cycles(0x82E3, 6);
        SYNC_OUT;
        if (!mff_call_at(0x02E5, 0x8684, 3)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x82E1, 1);
    }
    mff_cycles(0x82E6, 7);
    a = RAM(SND_FLAGS);
    c = a & 1;
    a >>= 1;
    NZ(a);
    if (!c) {
        int release = 0;

        mff_cycles(0x82EB, 6);
        SYNC_OUT;
        if (!mff_call_at(0x02ED, 0x86BA, 3)) return;
        SYNC_IN;
        mff_cycles(0x82EE, 5);
        a = RAM(SND_EFFECT_ROW_FRAMES);
        NZ(a);
        if (z) {
            mff_cycles(0x82F0, 1);
        } else {
            mff_cycles(0x82F2, 4);
            c = x >= SND_X_TRIANGLE;
            if (x == SND_X_TRIANGLE) {
                mff_cycles(0x82F4, 1 + 9);
                RAM(SND_LEVEL + x)--;
                NZ(RAM(SND_LEVEL + x));
                if (z)
                    release = 1;
                else
                    mff_cycles(0x82FA, 2);
            } else {
                mff_cycles(0x82F6, 5);
                a = RAM(SND_EFFECT_GATE_LEFT);
                NZ(a);
                if (z) {
                    mff_cycles(0x82F8, 2);
                    release = 1;
                }
            }
        }
        if (release) {
            mff_cycles(0x8300, 8);
            a = RAM(SND_ENV + x) & SND_PHASE_OFF;   /* off (phase 4) already */
            NZ(a);
            if (z) {
                mff_cycles(0x8307, 3);
                SYNC_OUT;
                nes_cpu_instruction_boundary(0x85A3, 2);
                call_by_address_tail(0x85A3, 3);    /* snd_key_off */
                return;
            }
            mff_cycles(0x82FA, 2);
        }
        mff_cycles(0x82FA, 6);
        SYNC_OUT;
        mff_rts();
        return;
    }

    /* a new row: the fields */
    mff_cycles(0x82E9, 2);
    mff_cycles(0x830A, 11);
    a = 0;
    NZ(a);
    RAM(SND_TMP + 3) = a;
    SYNC_OUT;
    if (!mff_call_at(0x0310, 0x8386, 3)) return;
    SYNC_IN;
    for (;;) {
        mff_cycles(0x8311, 4);
        c = a & 1;
        a >>= 1;
        NZ(a);
        if (c) {
            mff_cycles(0x8314, 9);
            mff_push(a);
            SYNC_OUT;
            if (!mff_call_at(0x0317, 0x8386, 3)) return;
            SYNC_IN;
            mff_cycles(0x8318, 12);
            RAM(SND_ARG) = a;
            a = RAM(SND_TMP + 3);
            NZ(a);
            SYNC_OUT;
            if (!mff_call_at(0x031E, 0x8326, 3)) return;
            SYNC_IN;
            mff_cycles(0x831F, 4);
            a = mff_pull();
            NZ(a);
        } else {
            mff_cycles(0x8312, 1);
        }
        mff_cycles(0x8320, 2);
        if (z) {
            /* no field left: the note */
            mff_cycles(0x8320, 1);
            SYNC_OUT;
            call_by_address_tail((uint16_t)(g_code_window_base + 0x0333), 3);
            return;
        }
        mff_cycles(0x8322, 7);
        RAM(SND_TMP + 3)++;
        NZ(RAM(SND_TMP + 3));
        if (z)
            break;
        mff_cycles(0x8311, 1);
    }
    /* $C4 counts at most 7 fields and never wraps: the 6502 would fall
     * into the field dispatch here */
    SYNC_OUT;
    func_8326_b3();
}

/* snd_effect_field, $8326: A = the field number (0-4), $C3 = its
 * byte. Jumps through the table at $8329: snd_set_instrument,
 * snd_set_duty, snd_set_volume, snd_set_slide, snd_set_detune, which
 * return to snd_effect_channel.
 * Caller: snd_effect_channel. */
void func_8326_b3(void) {
    mff_cycles(0x8326, 6);
    if (!mff_call_at(0x0328, 0x8023, 3)) return;
    mff_unreachable(SND_TBL_FIELDS);            /* the JSR never comes back */
}

/* snd_effect_note, $8333: the note byte of channel X's entry.
 *  - 0: off: level 0, envelope phase 4, the channel silenced at once
 *    (snd_silence);
 *  - otherwise the channel slides (env bit 5) toward the end of the range:
 *    note 83 when the slide speed is negative (rising), note 9 otherwise;
 *  - bit 7: hold: the pitch stays, no new attack (the triangle alone keys
 *    on again, which restarts its frame count); then the LFO reset check
 *    (snd_lfo_check);
 *  - else key on (snd_key_on), the period high rewritten, and the pitch:
 *    on the noise channel (note - 1) ^ $0F is the period index, $C3 = 0
 *    (snd_set_pitch); on the others note - 1 + $D2 (snd_note_pitch).
 * Caller: snd_effect_channel (BEQ, when no field is left). */
void func_8333_b3(void) {
    REGS_IN;
    mff_cycles(0x8333, 6);
    SYNC_OUT;
    if (!mff_call_at(0x0335, 0x8386, 3)) return;
    SYNC_IN;
    mff_cycles(0x8336, 4);
    y = a;
    if (y == 0) {
        mff_cycles(0x8339, 21);
        RAM(SND_LEVEL + x) = a;
        a = (RAM(SND_ENV + x) & (uint8_t)~SND_ENV_PHASE) | SND_PHASE_OFF;
        NZ(a);
        RAM(SND_ENV + x) = a;
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x80D8, 2);
        call_by_address_tail(0x80D8, 3);        /* snd_silence */
        return;
    }
    mff_cycles(0x8337, 1);

    mff_cycles(0x8349, 21);
    RAM(SND_ENV + x) |= SND_ENV_SLIDING;
    c = RAM(SND_SLIDE_AMOUNT + x) >> 7;
    if (c) {
        mff_cycles(0x8357, 1);
        a = SND_SLIDE_UP_TARGET;
    } else {
        mff_cycles(0x8359, 2);
        a = SND_SLIDE_DOWN_TARGET;
    }
    mff_cycles(0x835B, 9);
    RAM(SND_TARGET + x) = a;
    a = y;
    NZ(a);
    if (n) {
        mff_cycles(0x8361, 4);
        CMP(x, SND_X_TRIANGLE);
        if (z) {
            mff_cycles(0x8365, 6);
            SYNC_OUT;
            if (!mff_call_at(0x0367, 0x85AE, 3)) return;
            SYNC_IN;
        } else {
            mff_cycles(0x8363, 1);
        }
        mff_cycles(0x8368, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x8644, 2);
        call_by_address_tail(0x8644, 3);        /* the LFO reset check */
        return;
    }
    mff_cycles(0x835F, 1);

    mff_cycles(0x836B, 6);
    SYNC_OUT;
    if (!mff_call_at(0x036D, 0x85AE, 3)) return;
    SYNC_IN;
    mff_cycles(0x836E, 13);
    a = 0xFF;
    RAM(SND_PERIOD_HI_CACHE + x) = a;
    y--;
    a = x;
    if (a == SND_X_NOISE) {
        mff_cycles(0x8377, 10);
        RAM(SND_ARG) = a;
        a = y ^ 0x0F;
        NZ(a);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x8636, 2);
        call_by_address_tail(0x8636, 3);        /* snd_set_pitch */
        return;
    }
    mff_cycles(0x8375, 1);
    mff_cycles(0x837F, 10);
    a = y;
    m = RAM(SND_EFFECT_TRANSPOSE);
    w = a + m;
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    a = (uint8_t)w;
    NZ(a);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x85DE, 2);
    call_by_address_tail(0x85DE, 3);            /* snd_note_pitch */
}

/* snd_effect_read, $8386: the next byte of the effect's stream at $D0/$D1
 * (incremented), through snd_read (JMP).
 * Out: A = the byte, N, Z from it; Y = 0.
 * Callers: snd_effect_row, snd_effect_channel and snd_effect_note. */
void func_8386_b3(void) {
    REGS_IN;
    mff_cycles(0x8386, 13);
    y = RAM(SND_EFFECT_PTR);
    a = RAM(SND_EFFECT_PTR + 1);
    RAM(SND_EFFECT_PTR)++;
    NZ(RAM(SND_EFFECT_PTR));
    if (z) {
        mff_cycles(0x838E, 5);
        RAM(SND_EFFECT_PTR + 1)++;
        NZ(RAM(SND_EFFECT_PTR + 1));
    } else {
        mff_cycles(0x838C, 1);
    }
    mff_cycles(0x8390, 3);
    SYNC_OUT;
    nes_cpu_instruction_boundary(0x803A, 2);
    call_by_address_tail(0x803A, 3);
}
