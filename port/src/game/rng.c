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
#include "state.h"

/* rng_step, $C3B4. Leaves $00 = old $E4 & 2, X = 4, Y = 0, the flags of the
 * final ADC. 105 cycles when the feedback bit is 0, 106 when it is 1.
 * Charged instruction by instruction (a 1 is a taken branch's extra cycle):
 * the NMI counts frame_counter up, and a lag frame's NMI can land in here
 * (the console's timing, docs/timing.md); where it lands decides which
 * count the ADC $92 adds, as on the console. */
void func_C3B4(void) {
    uint8_t c, r, a;
    int i, sub;
    unsigned add;

    mff_cycles(0xC3B4, 2);                      /* LDX #0 */
    g_cpu.X = 0;
    mff_cycles(0xC3B6, 2);                      /* LDY #4 */
    g_cpu.Y = 4;
    mff_cycles(0xC3B8, 4);                      /* LDA $E4,X */
    mff_cycles(0xC3BA, 2);                      /* AND #2 */
    a = RAM(RNG) & 0x02;
    mff_cycles(0xC3BC, 3);                      /* STA $00 */
    RAM(TMP0) = a;
    mff_cycles(0xC3BE, 4);                      /* LDA $E5,X */
    mff_cycles(0xC3C0, 2);                      /* AND #2 */
    mff_cycles(0xC3C2, 3);                      /* EOR $00 */
    a = (uint8_t)((RAM(RNG + 1) & 0x02) ^ RAM(TMP0));
    mff_cycles(0xC3C4, 2);                      /* CLC */
    mff_cycles(0xC3C5, 2);                      /* BEQ $C3C8 */
    if (a == 0) {
        mff_cycles(0xC3C5, 1);
        c = 0;
    } else {
        mff_cycles(0xC3C7, 2);                  /* SEC */
        c = 1;
    }
    for (i = 0; i < 4; i++) {                   /* ROR $E4,X for X = 0..3 */
        uint8_t v, out;
        mff_cycles(0xC3C8, 6);
        v = RAM(RNG + i);
        out = v & 1;
        RAM(RNG + i) = (uint8_t)((v >> 1) | (c << 7));
        c = out;
        mff_cycles(0xC3CA, 2);                  /* INX */
        g_cpu.X++;
        mff_cycles(0xC3CB, 2);                  /* DEY */
        g_cpu.Y--;
        mff_cycles(0xC3CC, 2);                  /* BNE $C3C8 */
        if (i < 3) mff_cycles(0xC3CC, 1);
    }

    mff_cycles(0xC3CE, 3);                      /* LDA $95 */
    mff_cycles(0xC3D0, 3);                      /* EOR $E4 */
    mff_cycles(0xC3D2, 3);                      /* STA $E5 */
    RAM(RNG + 1) = RAM(DRAW_PARITY) ^ RAM(RNG);

    mff_cycles(0xC3D4, 3);                      /* LDA $E4 */
    mff_cycles(0xC3D6, 3);                      /* SBC $E5 */
    r = RAM(RNG);
    sub = r - RAM(RNG + 1) - (1 - c);
    c = sub >= 0;
    r = (uint8_t)sub;

    /* ADC $92: its N/V/Z/C are the flags the routine returns with */
    mff_cycles(0xC3D8, 3);
    add = (unsigned)r + RAM(FRAME_COUNTER) + c;
    g_cpu.V = (~(r ^ RAM(FRAME_COUNTER)) & (r ^ add) & 0x80) ? 1 : 0;
    g_cpu.C = add > 0xFF;
    g_cpu.A = (uint8_t)add;
    mff_nz(g_cpu.A);
    mff_cycles(0xC3DA, 3);                      /* STA $E4 */
    RAM(RNG) = g_cpu.A;

    mff_cycles(0xC3DC, 6);                      /* RTS */
    mff_rts();
}
