/*
 * hw.h - the NES hardware the game talks to, by name
 *
 * Game code reaches the PPU, the APU, the pads and the MMC3 only through
 * hw_read/hw_write with these register names, reads its tables in PRG ROM
 * through prg_read, and goes through mem_read/mem_write only where the
 * address is a pointer known at run time. Today all of them forward to the
 * scaffold runtime's emulated machine; P5 (renderer, data from build/assets)
 * and P6 (sound) replace what is behind them, not the game code.
 */
#ifndef MFF_HW_H
#define MFF_HW_H

#include "nes_runtime.h"

/* PPU */
#define PPU_CTRL        0x2000
#define PPU_MASK        0x2001
#define PPU_STATUS      0x2002
#define OAM_ADDR        0x2003
#define OAM_DATA        0x2004
#define PPU_SCROLL      0x2005
#define PPU_ADDR        0x2006
#define PPU_DATA        0x2007

/* APU (a channel's 4 registers from its first; $4009/$400D are unused) */
#define APU_PULSE1      0x4000
#define APU_PULSE2      0x4004
#define APU_TRIANGLE    0x4008
#define APU_NOISE       0x400C
#define APU_DMC         0x4010
#define OAM_DMA         0x4014
#define APU_STATUS      0x4015
#define JOY1            0x4016  /* write: strobe both pads */
#define JOY2            0x4017  /* read: pad 2; write: the APU frame counter */

/* MMC3 */
#define MMC3_BANK_SELECT 0x8000
#define MMC3_BANK_DATA   0x8001
#define MMC3_MIRRORING   0xA000
#define MMC3_PRG_RAM     0xA001
#define MMC3_IRQ_LATCH   0xC000
#define MMC3_IRQ_RELOAD  0xC001
#define MMC3_IRQ_DISABLE 0xE000
#define MMC3_IRQ_ENABLE  0xE001

static inline uint8_t hw_read(uint16_t reg) { return nes_read(reg); }
static inline void hw_write(uint16_t reg, uint8_t v) { nes_write(reg, v); }

/* a table in PRG ROM, through the banks the MMC3 maps */
static inline uint8_t prg_read(uint16_t addr) { return nes_read(addr); }

/* an address known only at run time: RAM, ROM or hardware */
static inline uint8_t mem_read(uint16_t addr) { return nes_read(addr); }
static inline void mem_write(uint16_t addr, uint8_t v) { nes_write(addr, v); }

/* Indexed reads (LDA, LDX, LDY, ADC, SBC, AND, ORA, EOR, CMP with abs,X,
 * abs,Y or (zp),Y): the 6502 takes one cycle more when base + index crosses
 * a page. The run's charge has the base cycles; nes_page_cross() adds the
 * extra one on the console's clock (port/src/runtime/timing.c,
 * docs/timing.md) and nothing on the scaffold's, whose model left it out
 * (port/scaffold/extras.c). */
void nes_page_cross(void);

/* LDA base,X / base,Y: a table in PRG ROM (or a pointer kept in C) */
static inline uint8_t prg_read_ix(uint16_t base, uint8_t i) {
    if ((base & 0xFF) + i > 0xFF) nes_page_cross();
    return nes_read((uint16_t)(base + i));
}

/* LDA base,X / base,Y on a RAM table (the slot tables read with a slot
 * number above 15: the ending's pseudo-slot $FF crosses their pages) */
static inline uint8_t ram_ix(uint16_t base, uint8_t i) {
    if ((base & 0xFF) + i > 0xFF) nes_page_cross();
    return g_ram[(uint16_t)(base + i) & 0x7FF];
}

/* LDA (zp),Y: the pointer in zero page (wrapping at $FF), plus Y */
static inline uint8_t mem_read_iy(uint8_t zp, uint8_t y) {
    uint16_t base = (uint16_t)(g_ram[zp] | g_ram[(uint8_t)(zp + 1)] << 8);
    if ((base & 0xFF) + y > 0xFF) nes_page_cross();
    return nes_read((uint16_t)(base + y));
}

#endif
