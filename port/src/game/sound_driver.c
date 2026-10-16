/*
 * sound_driver.c - the sound driver's entry points, helpers, commands and
 * frame tick (bank $06 $8000-$8251; docs/sound.md, "How the game talks to
 * it", "Commands", "Each frame")
 *
 * sound_update (0F:FF72) is the only caller from outside: it maps banks
 * $06/$07, runs sound_play ($8003) for each queued command and sound_tick
 * ($8000) once a frame.
 */
#include "sound_driver.h"

/* sound_tick, $8000: JMP snd_tick_body ($806C).
 * Caller: sound_update (0F:FF72, and its queue loop 0F:FF8E), once a frame
 * after the commands. */
void func_8000_b3(void) {
    mff_jmp_charge(0x8000, 0x806C);
    call_by_address_tail(0x806C, 3);
}

/* sound_play, $8003: JMP snd_play_body ($80FE), A = the command.
 * Caller: sound_update (0F:FF72/FF8E), once for each command it takes from
 * the queue, X = the queue's read index + 1. */
void func_8003_b3(void) {
    mff_jmp_charge(0x8003, 0x80FE);
    call_by_address_tail(0x80FE, 3);
}

/* snd_mul8, $8006: $C1:$C2 = $C1 * $C4, shifting $C1 out from the top and
 * adding $C4 for each 1 bit.
 * In: $C1, $C4. Out: $C1 = the product's high byte, $C2 = its low byte;
 * Y = 0 (N clear, Z set); A, C and V from the last add (A = 0 and V kept if
 * no bit was set; C = the last bit when the last step did not add).
 * Callers: snd_effect_row (row frames * gate), snd_music_note (ticks * gate
 * ratio), snd_key_on (the effect triangle's length), snd_output_volume
 * (tremolo), snd_output_regs (vibrato, pitch scale). */
void func_8006_b3(void) {
    REGS_IN;
    mff_cycles(0x8006, 7);
    a = 0;
    RAM(SND_PRODUCT_LO) = 0;
    y = 8;
    do {
        uint8_t lo = RAM(SND_PRODUCT_LO), hi = RAM(SND_PRODUCT_HI);

        /* ASL $C2 / ROL $C1: the product doubles, the next bit drops into C */
        RAM(SND_PRODUCT_LO) = (uint8_t)(lo << 1);
        RAM(SND_PRODUCT_HI) = (uint8_t)(hi << 1 | lo >> 7);
        c = hi >> 7;
        if (c) {
            mff_cycles(0x800C, 12 + 19);
            w = RAM(SND_PRODUCT_LO) + RAM(SND_MULTIPLIER);
            RAM(SND_PRODUCT_LO) = (uint8_t)w;
            a = RAM(SND_PRODUCT_HI);
            w = a + (w > 0xFF);                     /* ADC #0 */
            v = (~a & (a ^ w) & 0x80) != 0;
            c = w > 0xFF;
            a = (uint8_t)w;
            RAM(SND_PRODUCT_HI) = a;
        } else {
            mff_cycles(0x800C, 12 + 1);
        }
        mff_cycles(0x801F, 4);
        y--;
        NZ(y);
        if (y != 0)
            mff_cycles(0x800C, 1);
    } while (y != 0);
    mff_cycles(0x8022, 6);
    SYNC_OUT;
    mff_rts();
}

/* snd_jump_inline, $8023: jumps through the table of words that follows
 * the JSR to it, entry A. Pulls the JSR's return address (so the target
 * returns straight to the caller's caller) and leaves the target in
 * $C1/$C2.
 * Out (at the target): A = the target's low byte (N, Z from it),
 * Y = 2 * A + 2, C = bit 7 of the entry, X kept.
 * Callers: snd_special (table $81B5), snd_effect_channel's fields ($8329),
 * snd_music_command ($84A7), snd_envelope ($86C7). The JSR never comes back
 * to its caller: its mff_call_at returns unbalanced and the caller bails. */
void func_8023_b3(void) {
    REGS_IN;
    uint16_t target;

    mff_cycles(0x8023, 25);
    c = a >> 7;
    y = (uint8_t)(a << 1);
    y++;
    RAM(SND_PTR) = mff_pull();          /* the return address: table - 1 */
    RAM(SND_PTR + 1) = mff_pull();
    a = mem_read((uint16_t)(RAM16(SND_PTR) + y));
    mff_cycles(0x802E, 10);
    mff_push(a);
    y++;
    a = mem_read((uint16_t)(RAM16(SND_PTR) + y));
    mff_cycles(0x8032, 15);
    RAM(SND_PTR + 1) = a;
    a = mff_pull();
    NZ(a);
    RAM(SND_PTR) = a;
    SYNC_OUT;
    target = nes_read16zp(SND_PTR);
    nes_cpu_instruction_boundary(target, 2);
    call_by_address_tail(target, -1);
}

/* snd_read, $803A: the sound data byte at A:Y (high:low).
 * Out: A = the byte (N, Z from it), Y = 0, $C1/$C2 = the address; C clear
 * on the near path. From $C000 up the address is "far" (snd_read_far,
 * $8047): bank $18 is mapped at $A000 to read it ($08 on this 128 KiB
 * board) and bank $17 ($07) put back; no data uses one.
 * Callers: snd_start and snd_music_start (JSR), snd_effect_read and
 * snd_music_read (JMP). */
void func_803A_b3(void) {
    REGS_IN;
    mff_cycles(0x803A, 9);
    RAM(SND_PTR) = y;
    y = 0;
    c = a >= 0xC0;
    if (!c) {
        mff_cycles(0x8042, 8);
        RAM(SND_PTR + 1) = a;
        a = mem_read(RAM16(SND_PTR));
        NZ(a);
        mff_cycles(0x8046, 6);
        SYNC_OUT;
        mff_rts();
        return;
    }

    mff_cycles(0x8040, 1 + 13);
    RAM(SND_PTR + 1) = (uint8_t)(a - 0x20);     /* $C000-$DFFF -> $A000-$BFFF */
    hw_write(MMC3_BANK_SELECT, 0x07);           /* R7: the bank at $A000 */
    mff_cycles(0x8051, 6);
    hw_write(MMC3_BANK_DATA, 0x18);
    mff_cycles(0x8056, 5);
    a = mem_read(RAM16(SND_PTR));
    mff_cycles(0x8058, 9);
    mff_push(a);
    hw_write(MMC3_BANK_SELECT, 0x07);
    mff_cycles(0x805E, 6);
    hw_write(MMC3_BANK_DATA, 0x17);
    mff_cycles(0x8063, 20);
    a = 0x20;
    m = RAM(SND_PTR + 1);
    w = a + m;                                  /* CLC / ADC: the address back */
    v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
    c = w > 0xFF;
    RAM(SND_PTR + 1) = (uint8_t)w;
    a = mff_pull();
    NZ(a);
    SYNC_OUT;
    mff_rts();
}

/* snd_tick_body, $806C: sound_tick, once a frame.
 *  1. nothing while $C0 bit 0 is set (a sound_play running);
 *  2. the effect's sequencer, if an effect plays (snd_effect_step);
 *  3. tempo: $C8 += $CA, $C7 = $C9 + carry, the ticks the music advances;
 *  4. each channel, pulse 1 first (X = 3 down to 0): the effect's
 *     (snd_effect_channel) if $CF says it owns it, with $CF bit 7 set
 *     meanwhile; then the music's (snd_music_channel) unless it is paused;
 *  5. $C0 bit 0 cleared; the fade, if one runs.
 * Out: X = $FF, A = $CC & $7F (0 without a fade), C clear; V from the
 * tempo add or the fade; Y from the last routine called.
 * Caller: sound_tick (JMP). */
void func_806C_b3(void) {
    REGS_IN;
    mff_cycles(0x806C, 7);
    c = RAM(SND_FLAGS) & SND_FLAG_BUSY;
    a = RAM(SND_FLAGS) >> 1;
    NZ(a);
    if (c) {
        mff_cycles(0x806F, 1 + 6);
        SYNC_OUT;
        mff_rts();
        return;
    }

    mff_cycles(0x8071, 8);
    a = RAM(SND_EFFECT_PTR) | RAM(SND_EFFECT_PTR + 1);
    NZ(a);
    if (!z) {
        mff_cycles(0x8077, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0079, 0x8252, 3)) return;
        SYNC_IN;
    } else {
        mff_cycles(0x8075, 1);
    }

    mff_cycles(0x807A, 27);
    w = RAM(SND_TEMPO + 1) + RAM(SND_TEMPO_ACC);
    RAM(SND_TEMPO_ACC) = (uint8_t)w;
    c = w > 0xFF;
    a = RAM(SND_TEMPO);
    w = a + c;                                  /* ADC #0 */
    v = (~a & (a ^ w) & 0x80) != 0;
    a = (uint8_t)w;
    RAM(SND_TICKS) = a;

    a = RAM(SND_EFFECT_CHANNELS);
    mff_push(a);
    x = SND_X_PULSE1;
    for (;;) {
        mff_cycles(0x808C, 7);
        c = RAM(SND_EFFECT_CHANNELS) & 1;
        RAM(SND_EFFECT_CHANNELS) >>= 1;
        if (c) {
            mff_cycles(0x8090, 14);
            a = RAM(SND_EFFECT_CHANNELS) | SND_OWNED;
            NZ(a);
            RAM(SND_EFFECT_CHANNELS) = a;
            SYNC_OUT;
            if (!mff_call_at(0x0098, 0x82DE, 3)) return;
            SYNC_IN;
        } else {
            mff_cycles(0x808E, 1);
        }
        mff_cycles(0x8099, 7);
        a = RAM(SND_FLAGS) & SND_FLAG_PAUSED;
        if (a == 0) {
            mff_cycles(0x809F, 11);
            a = x;
            NZ(a);
            mff_push(a);
            SYNC_OUT;
            if (!mff_call_at(0x00A3, 0x8393, 3)) return;
            SYNC_IN;
            mff_cycles(0x80A4, 6);
            a = mff_pull();
            x = a;
        } else {
            mff_cycles(0x809D, 1);
        }
        mff_cycles(0x80A6, 4);
        x--;
        if (x & 0x80)
            break;
        mff_cycles(0x808C, 1);
    }

    mff_cycles(0x80A9, 24);
    a = mff_pull();
    RAM(SND_EFFECT_CHANNELS) = a;
    RAM(SND_FLAGS) &= ~SND_FLAG_BUSY;           /* LSR / ASL */
    c = 0;
    a = RAM(SND_FADE_SPEED) & 0x7F;
    NZ(a);
    if (!z) {
        /* the fade ($F5/$F6, never used): $C0 bits 4-7 += speed << 4, the
         * level $CD += speed >> 4 + carry; at the top the fade stops ($CC
         * keeps only its direction) and $CD stays $FF */
        mff_cycles(0x80B6, 7);
        RAM(SND_TMP) = 0;
        y = 4;
        do {
            mff_cycles(0x80BC, 11);
            c = a >> 7;
            a = (uint8_t)(a << 1);
            RAM(SND_TMP) = (uint8_t)(RAM(SND_TMP) << 1 | c);
            y--;
            if (y != 0)
                mff_cycles(0x80BC, 1);
        } while (y != 0);
        mff_cycles(0x80C2, 16);
        w = a + RAM(SND_FLAGS);
        c = w > 0xFF;
        RAM(SND_FLAGS) = (uint8_t)w;
        a = RAM(SND_TMP);
        m = RAM(SND_FADE_LEVEL);
        w = a + m + c;
        v = (~(a ^ m) & (a ^ w) & 0x80) != 0;
        c = w > 0xFF;
        a = (uint8_t)w;
        NZ(a);
        if (c) {
            mff_cycles(0x80CD, 10);
            RAM(SND_FADE_SPEED) &= SND_FADE_IN;
            a = 0xFF;
            NZ(a);
        } else {
            mff_cycles(0x80CB, 1);
        }
        mff_cycles(0x80D5, 3);
        RAM(SND_FADE_LEVEL) = a;
    } else {
        mff_cycles(0x80B4, 1);
    }
    mff_cycles(0x80D7, 6);
    SYNC_OUT;
    mff_rts();
}

/* snd_silence, $80D8: silences channel X (0-3, or $28-$2B): $30 into its
 * volume register, or $00 into $4008 for the triangle.
 * Out: Y = the channel's register offset, A = the value written; N, Z, C
 * from CPY #$08. X kept.
 * Callers: snd_silence_free (every channel the effect does not own),
 * snd_effect_channel (note 0) and snd_music_end (JMP). */
void func_80D8_b3(void) {
    REGS_IN;
    mff_cycles(0x80D8, 18);
    y = (uint8_t)(((x & 3) ^ 3) << 2);
    CMP(y, SND_TRIANGLE_REGS);
    if (z) {
        mff_cycles(0x80E6, 2);
        a = 0x00;
    } else {
        mff_cycles(0x80E4, 1);
        a = SND_VOLUME_SILENT;
    }
    mff_cycles(0x80E8, 5);
    hw_write((uint16_t)(APU_PULSE1 + y), a);
    mff_cycles(0x80EB, 6);
    SYNC_OUT;
    mff_rts();
}

/* snd_reg_write, $80EC: writes A to register Y (0 volume, 2 period low,
 * 3 period high) of channel X.
 * Out: $C4 = Y; Y = the register's offset from $4000; A kept (N, Z from
 * it); C clear. X kept.
 * Callers: snd_output_regs (the volume, period low and period high writes
 * of every envelope phase). */
void func_80EC_b3(void) {
    REGS_IN;
    mff_cycles(0x80EC, 30);
    mff_push(a);
    RAM(SND_REG_OFFSET) = y;
    y = (uint8_t)(((x & 3) ^ 3) << 2 | y);
    c = 0;                                      /* the ASLs shift out zeros */
    a = mff_pull();
    NZ(a);
    hw_write((uint16_t)(APU_PULSE1 + y), a);
    mff_cycles(0x80FD, 6);
    SYNC_OUT;
    mff_rts();
}

/* snd_play_body, $80FE: sound_play. Runs snd_command with $C0 bit 0 set
 * (INC/DEC), which keeps sound_tick out while a command runs.
 * In: A = the command (and Y, which the specials keep in $C3).
 * Out: N, Z from the DEC; the rest from snd_command.
 * Caller: sound_play (JMP). */
void func_80FE_b3(void) {
    REGS_IN;
    mff_cycles(0x80FE, 11);
    RAM(SND_FLAGS)++;
    NZ(RAM(SND_FLAGS));
    SYNC_OUT;
    if (!mff_call_at(0x0102, 0x8106, 3)) return;
    SYNC_IN;
    mff_cycles(0x8103, 11);
    RAM(SND_FLAGS)--;
    NZ(RAM(SND_FLAGS));
    SYNC_OUT;
    mff_rts();
}

/* snd_command, $8106: one command.
 *  - $F0-$FF go to snd_special (JMP);
 *  - from the table's size ($36, at $8A40) up the command wraps around
 *    (never happens);
 *  - snd_start ($8118): the data pointer, big-endian at $8A43 + 2 * cmd;
 *    null ($10, $11): nothing. A first data byte of 0 starts a song
 *    (snd_music_start, $816F); anything else is an effect: its priority
 *    (bits 0-6) must be at least the current one ($CE), and the start
 *    clears the effect's channel state ($0700-$0727) and $D2-$D5, with
 *    $D0/$D1 pointing after the priority byte. A looping effect (bit 7,
 *    none in the game) also gets $D6 bit 7 and $D7 = 2 * cmd + 1; one of the
 *    same priority as a looping one playing clears $D7.
 * In: A = the command. Out: from the path taken (the song start ends in
 * snd_silence_free).
 * Callers: snd_play_body; snd_effect_row runs snd_start ($8118) again for
 * a looping effect (no lifted routine: the interpreter does it). */
void func_8106_b3(void) {
    REGS_IN;
    mff_cycles(0x8106, 4);
    CMP(a, SND_CMD_SPECIAL);
    if (c) {
        mff_cycles(0x810A, 3);
        SYNC_OUT;
        nes_cpu_instruction_boundary(0x81AE, 2);
        call_by_address_tail(0x81AE, 3);
        return;
    }
    mff_cycles(0x8108, 1);
    for (;;) {
        mff_cycles(0x810D, 4);
        m = prg_read(SND_TBL_CMD_COUNT);
        mff_cycles(0x8110, 2);
        if (a < m)
            break;
        mff_cycles(0x8112, 6);
        m = prg_read(SND_TBL_CMD_COUNT);
        w = a - m;                              /* SEC / SBC: no borrow, a >= m */
        v = ((a ^ m) & (a ^ w) & 0x80) != 0;
        a = (uint8_t)w;
        mff_cycles(0x8116, 2 + 1);              /* BCS back */
    }
    mff_cycles(0x8110, 1);

    /* snd_start */
    mff_cycles(0x8118, 8);
    c = a >> 7;
    a = (uint8_t)(a << 1);
    x = a;
    y = prg_read((uint16_t)(SND_TBL_CMD_PTR_LO + x));
    mff_cycles(0x811D, 6);
    a = y | prg_read((uint16_t)(SND_TBL_CMD_PTR_HI + x));
    NZ(a);
    mff_cycles(0x8121, 2);
    if (z) {
        mff_cycles(0x8121, 1);
    } else {
        mff_cycles(0x8123, 4);
        a = prg_read((uint16_t)(SND_TBL_CMD_PTR_HI + x));
        NZ(a);
        mff_cycles(0x8126, 6);
        SYNC_OUT;
        if (!mff_call_at(0x0128, 0x803A, 3)) return;
        SYNC_IN;
        mff_cycles(0x8129, 4);
        y = a;
        if (y == 0) {
            /* snd_music_start: tempo 1 + $99/256 ticks a frame, transpose
             * and fade off, the music's RAM cleared, then the four stream
             * pointers (big-endian, pulse 1 first) */
            mff_cycles(0x812A, 1 + 24);
            RAM(SND_TEMPO) = 0x01;
            RAM(SND_TEMPO + 1) = 0x99;
            RAM(SND_TEMPO_ACC) = a;
            RAM(SND_TRANSPOSE) = a;
            RAM(SND_FADE_SPEED) = a;
            RAM(SND_FADE_LEVEL) = a;
            x = SND_MUSIC_RAM_SIZE - 1;
            for (;;) {
                mff_cycles(0x8181, 9);
                RAM(SND_M_INSTRUMENT + x) = a;
                x--;
                if (x & 0x80)
                    break;
                mff_cycles(0x8181, 1);
            }
            mff_cycles(0x8187, 2);
            x = SND_X_PULSE1;
            for (;;) {
                mff_cycles(0x8189, 7);
                if (++RAM(SND_PTR) == 0) {
                    mff_cycles(0x818D, 5);
                    RAM(SND_PTR + 1)++;
                } else {
                    mff_cycles(0x818B, 1);
                }
                mff_cycles(0x818F, 12);
                y = RAM(SND_PTR);
                a = RAM(SND_PTR + 1);
                NZ(a);
                SYNC_OUT;
                if (!mff_call_at(0x0195, 0x803A, 3)) return;
                SYNC_IN;
                mff_cycles(0x8196, 12);
                RAM(SND_MUSIC_PTR_HI + x) = a;
                if (++RAM(SND_PTR) == 0) {
                    mff_cycles(0x819D, 5);
                    RAM(SND_PTR + 1)++;
                } else {
                    mff_cycles(0x819B, 1);
                }
                mff_cycles(0x819F, 12);
                y = RAM(SND_PTR);
                a = RAM(SND_PTR + 1);
                NZ(a);
                SYNC_OUT;
                if (!mff_call_at(0x01A5, 0x803A, 3)) return;
                SYNC_IN;
                mff_cycles(0x81A6, 9);
                RAM(SND_MUSIC_PTR_LO + x) = a;
                x--;
                n = x >> 7;
                if (n)
                    break;
                mff_cycles(0x8189, 1);
            }
            /* BMI snd_silence_free, always taken */
            mff_cycles(0x81AC, 2 + 1);
            SYNC_OUT;
            func_81F1_b3();
            return;
        }

        /* an effect: the first byte is its priority and loop flag */
        mff_cycles(0x812C, 14);
        y = 0;
        x++;
        RAM(SND_TMP + 3) = a;
        a &= 0x7F;
        CMP(a, RAM(SND_EFFECT_PRIORITY));
        if (c) {
            mff_cycles(0x8137, 5);
            RAM(SND_EFFECT_PRIORITY) = a;
            if (z) {
                /* as high as the one playing: if that one loops and this
                 * one does not, no relaunch */
                mff_cycles(0x813B, 5);
                a = RAM(SND_EFFECT_LOOP);
                if (a & 0x80) {
                    mff_cycles(0x813F, 5);
                    a = RAM(SND_TMP + 3);
                    if (!(a & SND_EFFECT_LOOPS)) {
                        mff_cycles(0x8143, 3);
                        RAM(SND_EFFECT_RELAUNCH) = y;
                    } else {
                        mff_cycles(0x8141, 1);
                    }
                } else {
                    mff_cycles(0x813D, 1);
                }
            } else {
                mff_cycles(0x8139, 1);
            }
            mff_cycles(0x8145, 15);
            RAM(SND_EFFECT_LOOP) = y;
            c = RAM(SND_TMP + 3) >> 7;          /* ASL $C4 / ROR $D6: the loop flag */
            RAM(SND_TMP + 3) <<= 1;
            m = RAM(SND_EFFECT_LOOP) & 1;
            RAM(SND_EFFECT_LOOP) = (uint8_t)(RAM(SND_EFFECT_LOOP) >> 1 | c << 7);
            c = m;
            if (RAM(SND_EFFECT_LOOP) & 0x80) {
                mff_cycles(0x814D, 3);
                RAM(SND_EFFECT_RELAUNCH) = x;
            } else {
                mff_cycles(0x814B, 1);
            }
            mff_cycles(0x814F, 13);
            RAM(SND_PTR)++;                     /* past the priority byte */
            a = RAM(SND_PTR);
            RAM(SND_EFFECT_PTR) = a;
            if (a == 0) {
                mff_cycles(0x8157, 5);
                RAM(SND_PTR + 1)++;
            } else {
                mff_cycles(0x8155, 1);
            }
            mff_cycles(0x8159, 22);
            a = RAM(SND_PTR + 1);
            RAM(SND_EFFECT_PTR + 1) = a;
            a = y;
            RAM(SND_EFFECT_TRANSPOSE) = a;
            RAM(SND_EFFECT_ROW_FRAMES) = a;
            RAM(SND_EFFECT_GATE) = a;
            RAM(SND_EFFECT_GATE_LEFT) = a;
            y = SND_EFFECT_RAM_SIZE - 1;
            for (;;) {
                mff_cycles(0x8168, 9);
                RAM(SND_INSTRUMENT + y) = a;
                y--;
                NZ(y);
                if (n)
                    break;
                mff_cycles(0x8168, 1);
            }
        } else {
            mff_cycles(0x8135, 1);
        }
    }
    mff_cycles(0x816E, 6);
    SYNC_OUT;
    mff_rts();
}

/* snd_special, $81AE: commands $F0-$FF. Keeps the caller's Y in $C3 (the
 * argument of $F5-$F7) and jumps through the table at $81B5 with A & 7:
 * $F0 snd_stop_all, $F1 snd_effect_stop, $F2 snd_music_stop, $F3
 * snd_music_pause, $F4 snd_music_resume, $F5 fade in ($822D), $F6 fade out
 * ($8234), $F7 effect pitch scale ($824A); the last three were never lifted
 * (no route runs them) and run in the interpreter. $F8-$FF repeat them.
 * The target returns to snd_play_body. Through the queue, Y is whatever
 * the game had in Y when sound_update ran.
 * Caller: snd_command (JMP). */
void func_81AE_b3(void) {
    REGS_IN;
    mff_cycles(0x81AE, 11);
    RAM(SND_ARG) = y;
    a &= 7;
    NZ(a);
    SYNC_OUT;
    if (!mff_call_at(0x01B4, 0x8023, 3)) return;
    mff_unreachable(SND_TBL_SPECIAL);           /* the JSR never comes back */
}

/* snd_stop_all, $81C5: command $F0. snd_music_stop, then falls into
 * snd_effect_stop.
 * Caller: snd_special. */
void func_81C5_b3(void) {
    mff_cycles(0x81C5, 6);
    if (!mff_call_at(0x01C7, 0x81E4, 3)) return;
    func_81C8_b3();
}

/* snd_effect_stop, $81C8: command $F1, and the end of an effect. Clears
 * the effect's priority, pointer, relaunch and pitch scale ($CE, $D0/$D1,
 * $D7, $D8), then falls into snd_release_changed.
 * Callers: snd_special ($F1), snd_stop_all (fall through), snd_effect_row
 * (JMP, the end marker). */
void func_81C8_b3(void) {
    REGS_IN;
    mff_cycles(0x81C8, 17);
    a = 0;
    RAM(SND_EFFECT_PRIORITY) = a;
    RAM(SND_EFFECT_PTR) = a;
    RAM(SND_EFFECT_PTR + 1) = a;
    RAM(SND_EFFECT_RELAUNCH) = a;
    RAM(SND_EFFECT_PITCH_SCALE) = a;
    SYNC_OUT;
    func_81D4_b3();
}

/* snd_release_changed, $81D4: hands back the channels whose ownership
 * changed. In: $CF = the channels whose owner changed (0 = nothing to do):
 * flipped, it marks the ones to leave alone for snd_silence_free, which
 * silences the others and has the music rewrite its period there. Out:
 * $CF = 0 (the callers set it again), A = 0, N, Z from A, or (nothing to do)
 * A = $CF = 0.
 * Callers: snd_effect_row (a row with another channel mask),
 * snd_effect_stop (fall through). */
void func_81D4_b3(void) {
    REGS_IN;
    mff_cycles(0x81D4, 5);
    a = RAM(SND_EFFECT_CHANNELS);
    NZ(a);
    if (!z) {
        mff_cycles(0x81D8, 11);
        a ^= 0x0F;
        NZ(a);
        RAM(SND_EFFECT_CHANNELS) = a;
        SYNC_OUT;
        if (!mff_call_at(0x01DE, 0x81F1, 3)) return;
        SYNC_IN;
        mff_cycles(0x81DF, 5);
        a = 0;
        NZ(a);
        RAM(SND_EFFECT_CHANNELS) = a;
    } else {
        mff_cycles(0x81D6, 1);
    }
    mff_cycles(0x81E3, 6);
    SYNC_OUT;
    mff_rts();
}

/* snd_music_stop, $81E4: command $F2. Clears the four stream pointers,
 * then falls into snd_silence_free.
 * Callers: snd_special ($F2), snd_stop_all. */
void func_81E4_b3(void) {
    REGS_IN;
    mff_cycles(0x81E4, 4);
    a = 0;
    x = SND_X_PULSE1;
    for (;;) {
        mff_cycles(0x81E8, 14);
        RAM(SND_MUSIC_PTR_HI + x) = a;
        RAM(SND_MUSIC_PTR_LO + x) = a;
        x--;
        if (x & 0x80)
            break;
        mff_cycles(0x81E8, 1);
    }
    SYNC_OUT;
    func_81F1_b3();
}

/* snd_silence_free, $81F1: silences every channel whose $CF bit is clear
 * (bit 0 = pulse 1, X = 3, ... bit 3 = noise, X = 0) and, where the music
 * still plays, sets its cached period high ($077C + X) to $FF so that the
 * next output rewrites it. Then sweeps off on both pulses ($4001 = $4005 =
 * $08) and the four channels enabled ($4015 = $0F). $CF is kept.
 * Out: A = $0F, X = $FF; C from the last channel (the LSR, or snd_silence).
 * Callers: snd_music_start (BMI), snd_release_changed (JSR),
 * snd_music_stop (fall through), snd_music_pause (BNE). */
void func_81F1_b3(void) {
    REGS_IN;
    mff_cycles(0x81F1, 8);
    a = RAM(SND_EFFECT_CHANNELS);
    mff_push(a);
    x = SND_X_PULSE1;
    for (;;) {
        mff_cycles(0x81F6, 7);
        c = RAM(SND_EFFECT_CHANNELS) & 1;
        RAM(SND_EFFECT_CHANNELS) >>= 1;
        NZ(RAM(SND_EFFECT_CHANNELS));
        if (!c) {
            mff_cycles(0x81FA, 6);
            SYNC_OUT;
            if (!mff_call_at(0x01FC, 0x80D8, 3)) return;
            SYNC_IN;
            mff_cycles(0x81FD, 10);
            a = RAM(SND_MUSIC_PTR_HI + x) | RAM(SND_MUSIC_PTR_LO + x);
            if (a != 0) {
                mff_cycles(0x8205, 7);
                a = 0xFF;
                RAM(SND_PERIOD_HI_CACHE + x) = a;
            } else {
                mff_cycles(0x8203, 1);
            }
        } else {
            mff_cycles(0x81F8, 2);
        }
        mff_cycles(0x820A, 4);
        x--;
        if (x & 0x80)
            break;
        mff_cycles(0x81F6, 2);
    }
    mff_cycles(0x820D, 13);
    a = mff_pull();
    RAM(SND_EFFECT_CHANNELS) = a;
    a = SND_SWEEP_OFF;
    hw_write(APU_PULSE1 + 1, a);
    mff_cycles(0x8215, 4);
    hw_write(APU_PULSE2 + 1, a);
    mff_cycles(0x8218, 6);
    a = SND_CHANNELS_ON;
    NZ(a);
    hw_write(APU_STATUS, a);
    mff_cycles(0x821D, 6);
    SYNC_OUT;
    mff_rts();
}

/* snd_music_pause, $821E: command $F3. Sets $C0 bit 1 (sound_tick skips
 * the music) and branches to snd_silence_free (BNE, always taken).
 * Caller: snd_special. */
void func_821E_b3(void) {
    REGS_IN;
    mff_cycles(0x821E, 10);
    a = RAM(SND_FLAGS) | SND_FLAG_PAUSED;
    NZ(a);
    RAM(SND_FLAGS) = a;
    mff_cycles(0x8224, 2);
    SYNC_OUT;
    call_by_address_tail((uint16_t)(g_code_window_base + 0x01F1), 3);
}

/* snd_music_resume, $8226: command $F4. Clears $C0 bit 1.
 * Out: A = $C0, N, Z from it.
 * Caller: snd_special. */
void func_8226_b3(void) {
    REGS_IN;
    mff_cycles(0x8226, 14);
    a = RAM(SND_FLAGS) & (uint8_t)~SND_FLAG_PAUSED;
    NZ(a);
    RAM(SND_FLAGS) = a;
    SYNC_OUT;
    mff_rts();
}
