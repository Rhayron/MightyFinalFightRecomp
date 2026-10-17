/*
 * sound.c - the game side of sound (docs/engine.md, "Sound"; docs/sound.md,
 * "How the game talks to it")
 *
 * Game code queues commands in an 8-entry ring at $DC-$E3 (write index $DA,
 * $88 marks a free entry); sound_update drains it once per frame, after the
 * NMI, into the driver (sound_driver.c).
 */
#include "sound_driver.h"

#define SOUND_FREE 0x88     /* a free entry of the ring */

void func_FF4D(void);       /* bank_switch, bank.c */
void func_FF8E(void);

/* the ring, $F34C on: queue command A if the entry at the write index is
 * free, else drop it. Uses $00 (X) and $01 (A). Returns X kept (N, Z from
 * it), A = the entry it found or the new write index, C = (entry >= $88). */
static void queue_push(void) {
    uint8_t cmd = g_cpu.A, x = g_cpu.X, w, slot;

    mff_cycles(0xF34C, 3 + 3 + 3);
    RAM(TMP0) = x;
    w = RAM(SOUND_QUEUE_WRITE);
    RAM(TMP1) = cmd;

    mff_cycles(0xF352, 4 + 2);
    slot = RAM((SOUND_QUEUE + w) & 0xFF);
    g_cpu.C = slot >= SOUND_FREE;
    if (slot == SOUND_FREE) {
        mff_cycles(0xF356, 2 + 3 + 4 + 2 + 2 + 2 + 3);
        RAM((SOUND_QUEUE + w) & 0xFF) = cmd;
        g_cpu.A = (uint8_t)((w + 1) & 7);
        RAM(SOUND_QUEUE_WRITE) = g_cpu.A;
        mff_cycles(0xF362, 3 + 6);
    } else {
        g_cpu.A = slot;
        mff_cycles(0xF356, 3 + 3 + 6);
    }
    g_cpu.X = x;
    mff_nz(x);
    mff_rts();
}

/* sound_queue_push, $F34A: queues command A (queue_push) and keeps it in
 * $D9, the last music command (round_music and the boss music read it to
 * avoid restarting a song that plays). */
void func_F34A(void) {
    mff_cycles(0xF34A, 3);
    RAM(MUSIC_CURRENT) = g_cpu.A;
    queue_push();
}

/* sound_queue_push_sfx, $F34C: the effects' entry, after the STA $D9: queues
 * command A and leaves $D9 alone. */
void func_F34C(void) {
    queue_push();
}

/* sound_update, $FF72: the sound driver's frame. When the NMI landed inside
 * bank_switch ($F6 set) it only counts the update as deferred ($F7 + 1;
 * bank_switch runs it when it finishes). Otherwise it maps banks $06 and
 * $07 at $8000/$A000 (MMC3 R6/R7, the select also in $F0) and goes on into
 * the queue ($FF8E).
 * Out (deferred): N, Z from $F7; the rest from bank_switch.
 * Callers: nmi_tail (extras.c), bank_switch (JMP, the deferred update). */
void func_FF72(void) {
    REGS_IN;
    mff_cycles(0xFF72, 5);
    a = RAM(BANK_SWITCH_BUSY);
    if (a != 0) {
        mff_cycles(0xFF74, 1);
        mff_cycles(0xFFB2, 11);
        RAM(SOUND_DEFERRED)++;
        NZ(RAM(SOUND_DEFERRED));
        SYNC_OUT;
        mff_rts();
        return;
    }
    mff_cycles(0xFF76, 9);
    a = 0x06;                                   /* R6: $8000 */
    RAM(MMC3_SELECT) = a;
    hw_write(MMC3_BANK_SELECT, a);
    mff_cycles(0xFF7D, 6);
    a = 0x06;                                   /* bank $06 */
    hw_write(MMC3_BANK_DATA, a);
    mff_cycles(0xFF82, 9);
    a = 0x07;                                   /* R7: $A000 */
    RAM(MMC3_SELECT) = a;
    hw_write(MMC3_BANK_SELECT, a);
    mff_cycles(0xFF89, 6);
    a = 0x07;                                   /* bank $07 */
    hw_write(MMC3_BANK_DATA, a);
    SYNC_OUT;
    func_FF8E();
}

/* sound_update's queue, $FF8E: each command from the read index $DB on
 * until a free entry, the entry freed and the index advanced, through
 * sound_play (A = the command, X = the read index + 1, unmasked); then
 * sound_tick, $F7 = 0, and bank_switch (JMP) maps the game's banks back.
 * Out: from bank_switch.
 * Caller: sound_update (fall through). */
void func_FF8E(void) {
    REGS_IN;
    for (;;) {
        mff_cycles(0xFF8E, 11);
        x = RAM(SOUND_QUEUE_READ);
        a = RAM((SOUND_QUEUE + x) & 0xFF);
        CMP(a, SOUND_FREE);
        if (z)
            break;
        mff_cycles(0xFF96, 28);
        mff_push(a);
        a = SOUND_FREE;
        RAM((SOUND_QUEUE + x) & 0xFF) = a;
        x++;
        a = x & 7;
        RAM(SOUND_QUEUE_READ) = a;
        a = mff_pull();
        NZ(a);
        SYNC_OUT;
        if (!mff_call_at(0x1FA4, 0x8003, -1)) return;
        SYNC_IN;
        mff_cycles(0xFFA5, 3);                  /* JMP $FF8E */
    }
    mff_cycles(0xFF94, 1);
    mff_cycles(0xFFA8, 6);
    SYNC_OUT;
    if (!mff_call_at(0x1FAA, 0x8000, -1)) return;
    SYNC_IN;
    mff_cycles(0xFFAB, 8);
    a = 0;
    NZ(a);
    RAM(SOUND_DEFERRED) = a;
    SYNC_OUT;
    nes_cpu_instruction_boundary(0xFF4D, 2);
    {
        uint16_t swb = g_code_window_base;

        g_code_window_base = 0xE000;
        func_FF4D();
        g_code_window_base = swb;
    }
}
