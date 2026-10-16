/*
 * sound.c - the game side of sound (docs/engine.md, "Sound")
 *
 * Game code queues commands in an 8-entry ring at $DC-$E3 (write index $DA,
 * $88 marks a free entry); sound_update drains it once per frame, after the
 * NMI.
 */
#include "mff.h"
#include "state.h"

/* sound_queue_push, $F34A: queue command A if the next entry is free, else
 * drop it. Also keeps the command in $D9 and uses $00/$01. Returns X kept,
 * A = the entry it found (or the new write index), C = (entry >= $88). */
void func_F34A(void) {
    uint8_t cmd = g_cpu.A, x = g_cpu.X, w, slot;

    mff_cycles(0xF34A, 3 + 3 + 3 + 3);
    RAM(MUSIC_CURRENT) = cmd;
    RAM(TMP0) = x;
    w = RAM(SOUND_QUEUE_WRITE);
    RAM(TMP1) = cmd;

    mff_cycles(0xF352, 4 + 2);
    slot = RAM(SOUND_QUEUE + w);
    g_cpu.C = slot >= 0x88;
    if (slot == 0x88) {
        mff_cycles(0xF356, 2 + 3 + 4 + 2 + 2 + 2 + 3);
        RAM(SOUND_QUEUE + w) = cmd;
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
