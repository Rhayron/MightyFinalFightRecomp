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

/* snd_play_body, $80FE: sound_play. Sets $C0 bit 0 (INC) around
 * snd_command, so that a sound_tick inside would do nothing and the effect
 * channels only parse their first row.
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
