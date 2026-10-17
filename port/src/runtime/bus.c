/*
 * bus.c - the CPU's registers and work RAM, and what each address reaches
 *
 *   $0000-$1FFF  2 KiB work RAM, mirrored
 *   $2000-$3FFF  PPU registers, mirrored every 8 (ppu.c)
 *   $4000-$4013  APU, $4015 APU status, $4017 APU frame counter (apu.c)
 *   $4014        OAM DMA: 256 bytes from page N, 513 or 514 cycles
 *   $4016/$4017  the pads (read); $4016 write strobes both
 *   $6000-$7FFF  cartridge RAM (the game never uses it; kept for the bus)
 *   $8000-$FFFF  PRG ROM through the MMC3 windows (read), mapper (write)
 *
 * Reads of write-only or unmapped addresses return the last value the bus
 * carried, as on the console (open bus).
 */
#include "machine.h"
#include <string.h>

CPU6502State g_cpu;
uint8_t g_ram[0x0800];
uint8_t g_sram[0x2000];
uint8_t g_controller1_buttons;
uint8_t g_controller2_buttons;

static uint8_t s_bus;                   /* the data bus's last value */

/* the pads: strobe high reloads continuously and reads A; on the falling
 * edge both shift registers latch the buttons, then each read shifts one
 * out, MSB first, filling with 1s as a standard pad does */
static int s_strobe;
static uint8_t s_shift[2];

void bus_power_on(void) {
    memset(&g_cpu, 0, sizeof g_cpu);
    memset(g_ram, 0, sizeof g_ram);
    memset(g_sram, 0xFF, sizeof g_sram);
    g_cpu.S = 0xFD;
    g_cpu.I = 1;
    s_bus = 0;
    s_strobe = 0;
    s_shift[0] = s_shift[1] = 0;
    g_controller1_buttons = g_controller2_buttons = 0;
}

uint8_t pad_read(int port) {
    uint8_t *sh = &s_shift[port & 1];
    uint8_t bit;
    if (s_strobe)
        return (uint8_t)(0x40 | ((port ? g_controller2_buttons : g_controller1_buttons) >> 7));
    bit = (uint8_t)(*sh >> 7);
    *sh = (uint8_t)((*sh << 1) | 1);
    return (uint8_t)(0x40 | bit);
}

void pad_strobe(uint8_t v) {
    if (v & 1) {
        s_strobe = 1;
    } else if (s_strobe) {
        s_strobe = 0;
        s_shift[0] = g_controller1_buttons;
        s_shift[1] = g_controller2_buttons;
    }
}

static uint8_t bus_read(uint16_t a) {
    if (a < 0x2000) return g_ram[a & 0x7FF];
    if (a < 0x4000) return ppu_read((uint16_t)(0x2000 | (a & 7)));
    if (a < 0x4020) {
        if (a == 0x4015) return apu_read_status();
        if (a == 0x4016) return pad_read(0);
        if (a == 0x4017) return pad_read(1);
        return s_bus;
    }
    if (a < 0x6000) return s_bus;
    if (a < 0x8000) return g_sram[a - 0x6000];
    return mmc3_prg_read(a);
}

uint8_t nes_read(uint16_t a) {
    s_bus = bus_read(a);
    return s_bus;
}

/* OAM DMA copies from the CPU's view of page N (so it can read ROM too)
 * into OAM from entry 0; the CPU stalls 513 cycles, 514 on an odd cycle,
 * charged with the next instruction */
static void oam_dma(uint8_t page) {
    uint16_t src = (uint16_t)(page << 8);
    for (int i = 0; i < 256; i++) ppu_oam_write(i, nes_read((uint16_t)(src + i)));
    timing_add_dma_stall();
}

void nes_write(uint16_t a, uint8_t v) {
    s_bus = v;
    if (a < 0x2000) { g_ram[a & 0x7FF] = v; return; }
    if (a < 0x4000) { ppu_write((uint16_t)(0x2000 | (a & 7)), v); return; }
    if (a == 0x4014) { oam_dma(v); return; }
    if (a == 0x4016) { pad_strobe(v); return; }
    if (a < 0x4020) { apu_write(a, v); return; }
    if (a < 0x6000) return;
    if (a < 0x8000) { g_sram[a - 0x6000] = v; return; }
    mmc3_write(a, v);
}

uint16_t nes_read16zp(uint8_t zp) {
    return (uint16_t)(g_ram[zp] | (g_ram[(uint8_t)(zp + 1)] << 8));
}
