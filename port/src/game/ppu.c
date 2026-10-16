/*
 * ppu.c - rendering on/off (docs/engine.md, "NMI and the hijacked return")
 *
 * render_off_depth $EE counts nested rendering_off calls; while it is
 * nonzero the NMI takes nmi_busy and skips its VRAM work. ppumask_shadow
 * $FE is the PPUMASK value the NMI writes back.
 */
#include "mff.h"

/* rendering_off, $C265. Returns A = 0, Z set. */
void func_C265(void) {
    mff_cycles(0xC265, 5);
    RAM(0xEE)++;
    mff_cycles(0xC267, 2 + 3);
    g_cpu.A = 0;
    mff_nz(0);
    RAM(0xFE) = 0;
    mff_cycles(0xC26B, 4);
    nes_write(0x2001, 0);
    mff_cycles(0xC26E, 6);
    mff_rts();
}

/* rendering_on, $C26F: background and sprites on. Returns A = $18. */
void func_C26F(void) {
    mff_cycles(0xC26F, 5);
    RAM(0xEE)--;
    mff_cycles(0xC271, 2 + 3);
    g_cpu.A = 0x18;
    mff_nz(0x18);
    RAM(0xFE) = 0x18;
    mff_cycles(0xC275, 4);
    nes_write(0x2001, 0x18);
    mff_cycles(0xC278, 6);
    mff_rts();
}
