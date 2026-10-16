/*
 * input.c - the joypads (docs/engine.md, "Odds and ends")
 *
 * joy_pressed $14/$15 (pad 1/2: buttons that went down this read),
 * joy_held $16/$17 (buttons down now). Bit order as the pad shifts them out,
 * MSB first: A B Select Start Up Down Left Right.
 */
#include "mff.h"
#include "state.h"

/* read_joypads, $C279. Strobes both pads and reads 8 bits from each, OR-ing
 * in bit 1 (the Famicom expansion pads), then derives pressed from the
 * previous held state and clears the whole D-pad nibble of a byte that holds
 * an opposite pair (Up+Down or Left+Right). Uses $00/$01.
 * Returns X = $FF, Y = the new held byte of pad 1, A = the last pair test,
 * C = 1 if that test cancelled. */
void func_C279(void) {
    int x, cycles = 433;
    uint8_t v, a = 0;

    hw_write(JOY1, 1);
    hw_write(JOY1, 0);
    for (x = 0; x < 8; x++) {
        uint8_t p1 = hw_read(JOY1);
        uint8_t p2;
        RAM(JOY_PRESSED) = (uint8_t)((RAM(JOY_PRESSED) << 1) | (p1 & 1));
        RAM(TMP0) = (uint8_t)((RAM(TMP0) << 1) | ((p1 >> 1) & 1));
        p2 = hw_read(JOY2);
        RAM(JOY_PRESSED + 1) = (uint8_t)((RAM(JOY_PRESSED + 1) << 1) | (p2 & 1));
        RAM(TMP1) = (uint8_t)((RAM(TMP1) << 1) | ((p2 >> 1) & 1));
    }
    RAM(JOY_PRESSED) |= RAM(TMP0);
    RAM(JOY_PRESSED + 1) |= RAM(TMP1);

    for (x = 1; x >= 0; x--) {
        uint8_t now = RAM(JOY_PRESSED + x);
        RAM(JOY_PRESSED + x) = (uint8_t)((now ^ RAM(JOY_HELD + x)) & now);
        RAM(JOY_HELD + x) = now;
        g_cpu.Y = now;
    }

    /* over $14-$17: pressed and held of both pads */
    for (x = 3; x >= 0; x--) {
        v = RAM(JOY_PRESSED + x);
        if ((v & 0x0C) == 0x0C) {
            cycles += 23;
            RAM(JOY_PRESSED + x) = a = v & 0xF0;
            g_cpu.C = 1;
        } else if ((v & 0x03) == 0x03) {
            cycles += 32;
            RAM(JOY_PRESSED + x) = a = v & 0xF0;
            g_cpu.C = 1;
        } else {
            cycles += 23;
            a = v & 0x03;
            g_cpu.C = 0;
        }
    }
    g_cpu.A = a;
    g_cpu.X = 0xFF;
    g_cpu.N = 1;
    g_cpu.Z = 0;

    mff_cycles(0xC279, cycles);
    mff_rts();
}
