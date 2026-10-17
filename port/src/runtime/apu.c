/*
 * apu.c - the APU's registers, kept and not played
 *
 * P6 builds the 2A03 synthesizer on another branch; here every write is
 * accepted and remembered so that it can read them. Nothing the APU does
 * reaches the game:
 *  - reset writes $40 to $4017 before any frame-counter IRQ can be raised
 *    (16 cycles in, the first would come at 29,830), so the frame IRQ is
 *    inhibited for good;
 *  - the sound driver only enables the pulse, triangle and noise channels
 *    ($4015 = $0F), never the DMC, so there is no sample DMA stealing
 *    cycles and no DMC IRQ;
 *  - the game never reads $4015.
 * The runtime says so if any of that ever stops being true.
 */
#include "machine.h"
#include <stdio.h>
#include <string.h>

static uint8_t s_reg[0x18];             /* $4000-$4017 as last written */

void apu_power_on(void) { memset(s_reg, 0, sizeof s_reg); }

void apu_write(uint16_t a, uint8_t v) {
    static int warned;
    if (a > 0x4017) return;
    s_reg[a - 0x4000] = v;
    if (!warned && ((a == 0x4015 && (v & 0x10)) || (a == 0x4017 && !(v & 0x40)) ||
                    (a == 0x4010 && (v & 0x80)))) {
        warned = 1;
        fprintf(stderr, "[mff] APU: $%04X = $%02X (DMC or frame IRQ on): the runtime does not model "
                "their timing, so the cycle count may leave the scaffold's\n", a, v);
    }
}

uint8_t apu_read_status(void) {
    static int warned;
    if (!warned) {
        warned = 1;
        fprintf(stderr, "[mff] APU: the game read $4015, which the runtime does not model\n");
    }
    return 0;
}
