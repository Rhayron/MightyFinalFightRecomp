/*
 * rng.c - the random number generator (docs/engine.md, "Odds and ends")
 *
 * State: rng $E4-$E7 (a 32-bit shift register, $E4 the low byte), mixed
 * with draw_parity $95 (the sprite-order flip) and frame_counter $92. One
 * step rotates the register right by one bit, feeding bit 1 of $E4 XOR
 * bit 1 of $E5 into the top, then remixes the two low bytes:
 *     $E5 = $95 ^ $E4
 *     $E4 = $E4 - $E5 - !C + $92 + C'   (C from the rotate, C' from the SBC)
 * Callers read the result from $E4 (also left in A).
 */
#include "mff.h"

/* rng_step, $C3B4. Leaves $00 = old $E4 & 2, X = 4, Y = 0, the flags of the
 * final ADC. 105 cycles when the feedback bit is 0, 106 when it is 1. */
void func_C3B4(void) {
    uint8_t fb, c, r;
    int i, sub;
    unsigned add;

    RAM(0x00) = RAM(0xE4) & 0x02;
    fb = (uint8_t)((RAM(0xE5) & 0x02) ^ RAM(0x00));
    mff_cycles(0xC3B4, fb ? 106 : 105);

    c = fb ? 1 : 0;
    for (i = 0; i < 4; i++) {            /* ROR $E4,X for X = 0..3 */
        uint8_t v = RAM(0xE4 + i);
        uint8_t out = v & 1;
        RAM(0xE4 + i) = (uint8_t)((v >> 1) | (c << 7));
        c = out;
    }
    g_cpu.X = 4;
    g_cpu.Y = 0;

    RAM(0xE5) = RAM(0x95) ^ RAM(0xE4);

    /* SBC $E5 */
    r = RAM(0xE4);
    sub = r - RAM(0xE5) - (1 - c);
    c = sub >= 0;
    r = (uint8_t)sub;

    /* ADC $92: its N/V/Z/C are the flags the routine returns with */
    add = (unsigned)r + RAM(0x92) + c;
    g_cpu.V = (~(r ^ RAM(0x92)) & (r ^ add) & 0x80) ? 1 : 0;
    g_cpu.C = add > 0xFF;
    g_cpu.A = (uint8_t)add;
    mff_nz(g_cpu.A);
    RAM(0xE4) = g_cpu.A;

    mff_rts();
}
