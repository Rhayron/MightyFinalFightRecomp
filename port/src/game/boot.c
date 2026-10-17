/*
 * boot.c - reset, the power-on start of the game (docs/engine.md, "Boot"),
 * and the lone RTS at $FF4C.
 *
 * reset runs with interrupts off and no task: it brings the machine to a
 * known state, spawns task 0 at main_task and falls into the scheduler,
 * never to return. title_screen (0E:C6A4) and 04:9570 JMP here: attract
 * mode is the whole game starting over.
 */
#include "mff.h"
#include "state.h"

void func_FEBA(void);   /* scheduler */
void func_FF02(void);   /* task_spawn */
void func_FF4D(void);   /* bank_switch */

#define PPUCTRL_BOOT        0x08    /* NMI off, sprites at $1000 */
#define APU_FRAME_IRQ_OFF   0x40
#define VBLANK              0x80    /* PPU_STATUS bit 7 */
#define VBLANK_WAITS        4
#define PPUADDR_A12         0x10
#define RAM_PAGES_END       0x07    /* the clear loop's last page */
#define SOUND_QUEUE_EMPTY   0x88
#define SOUND_QUEUE_LAST    0x07
#define PPUMASK_BOOT        0x18    /* background and sprites shown */
#define MIRROR_HORIZONTAL   0x01
#define BOOT_PRG_8000       0x0C
#define CHR_R0_BOOT         0x40    /* 2 KiB background banks */
#define CHR_R1_BOOT         0x42
#define NAMETABLE_0         0x20    /* ppu_fill_nametable's A: PPU address high byte */
#define NAMETABLE_1         0x24
#define MAIN_TASK           0xC556
#define PPUCTRL_GAME        0x88    /* NMI on, sprites at $1000 */

/*
 * reset, $FE00: SEI / CLD; PPUCTRL $08 (no NMI), the APU frame IRQ off,
 * rendering, the DMC and the APU channels off, S = $FF; four vblanks pass
 * (PPU_STATUS bit 7 set, then clear, four times; docs/engine.md said two). PPUADDR then gets $1010 and
 * $0000 in turn, 16 times each: PPU A12 rises eight times, which clocks
 * the MMC3's scanline counter (the usual MMC3 start-up, by the look of
 * it). All of RAM is cleared ($00-$FF by index, then pages 1-7 through
 * the pointer $00/$01), sound_queue filled with $88 (empty), ppumask_shadow
 * $18, mirroring horizontal, PRG banks $0C/$0D (bank_switch), CHR R0/R1
 * $40/$42 and their shadows with R2-R5 = 0-3; sprites hidden
 * (sprites_hide_rest, 0E:C31D) and both nametables cleared
 * (ppu_fill_nametable, 0E:C2D1); task 0 spawned at main_task (0E:C556);
 * ppuctrl_shadow $88 (the nmi turns NMI on from it); then the scheduler.
 * Callers: the RESET vector; JMPs from 04:9570 and 0E:C6A4.
 */
void func_FE00(void)
{
    REGS_IN;

    mff_cycles(0xFE00, 10);
    g_cpu.I = 1;
    g_cpu.D = 0;
    a = PPUCTRL_BOOT;
    hw_write(PPU_CTRL, a);
    mff_cycles(0xFE07, 6);
    a = APU_FRAME_IRQ_OFF;
    hw_write(JOY2, a);
    mff_cycles(0xFE0C, 6);
    x = 0x00;
    hw_write(PPU_MASK, x);
    mff_cycles(0xFE11, 4);
    hw_write(APU_DMC, x);
    mff_cycles(0xFE14, 4);
    hw_write(APU_STATUS, x);
    mff_cycles(0xFE17, 6);
    x--;
    g_cpu.S = x;

    /* four vblanks: bit 7 set (the read clears it), then clear */
    x = VBLANK_WAITS;
    for (;;) {
        for (;;) {
            mff_cycles(0xFE1B, 4);
            a = hw_read(PPU_STATUS);
            mff_cycles(0xFE1E, 2);
            if (a & VBLANK)
                break;
            mff_cycles(0xFE1B, 1);
        }
        for (;;) {
            mff_cycles(0xFE20, 4);
            a = hw_read(PPU_STATUS);
            mff_cycles(0xFE23, 2);
            if (!(a & VBLANK))
                break;
            mff_cycles(0xFE20, 1);
        }
        mff_cycles(0xFE25, 4);
        x--;
        if (x == 0)
            break;
        mff_cycles(0xFE1B, 1);
    }

    /* PPU A12 up and down, 16 times */
    mff_cycles(0xFE28, 4);
    a = hw_read(PPU_STATUS);                    /* resets the address latch */
    mff_cycles(0xFE2B, 4);
    a = PPUADDR_A12;
    y = a;
    for (;;) {
        mff_cycles(0xFE2E, 4);
        hw_write(PPU_ADDR, a);
        mff_cycles(0xFE31, 4);
        hw_write(PPU_ADDR, a);
        mff_cycles(0xFE34, 6);
        a ^= PPUADDR_A12;
        y--;
        if (y == 0)
            break;
        mff_cycles(0xFE2E, 1);
    }

    /* RAM: zero page by index (Y from 0, then $FF down to 1) ... */
    mff_cycles(0xFE39, 2);
    a = y;
    for (;;) {
        mff_cycles(0xFE3A, 9);
        RAM(TMP0 + y) = a;
        y--;
        if (y == 0)
            break;
        mff_cycles(0xFE3A, 1);
    }
    /* ... then pages 1-7 through ($00), $01 counting the page */
    for (;;) {
        mff_cycles(0xFE40, 5);
        RAM(TMP1)++;
        for (;;) {
            mff_cycles(0xFE42, 6);
            mem_write((uint16_t)(RAM16(TMP0) + y), a);
            mff_cycles(0xFE44, 4);
            y++;
            if (y == 0)
                break;
            mff_cycles(0xFE42, 1);
        }
        mff_cycles(0xFE47, 7);
        x = RAM(TMP1);
        c = x >= RAM_PAGES_END;
        if (x == RAM_PAGES_END)
            break;
        mff_cycles(0xFE40, 1);
    }

    /* the sound queue empty: X is 7 */
    mff_cycles(0xFE4D, 4);
    y = SOUND_QUEUE_LAST;
    a = SOUND_QUEUE_EMPTY;
    for (;;) {
        mff_cycles(0xFE51, 8);
        RAM((SOUND_QUEUE + x) & 0xFF) = a;
        x--;
        if (x & 0x80)
            break;
        mff_cycles(0xFE51, 1);
    }

    /* the mapper */
    mff_cycles(0xFE56, 11);
    a = PPUMASK_BOOT;
    RAM(PPUMASK_SHADOW) = a;
    a = MIRROR_HORIZONTAL;
    hw_write(MMC3_MIRRORING, a);
    mff_cycles(0xFE5F, 16);
    x = BOOT_PRG_8000;
    RAM(BANK_REQUEST) = x;
    x++;
    n = (x) >> 7;
    z = (x) == 0;
    RAM(BANK_REQUEST + 1) = x;
    SYNC_OUT;
    if (!mff_call_fn(0x1E68, func_FF4D, 0xE000)) return;   /* bank_switch */
    SYNC_IN;
    mff_cycles(0xFE69, 6);
    x = 0x00;
    hw_write(MMC3_BANK_SELECT, x);
    mff_cycles(0xFE6E, 6);
    a = CHR_R0_BOOT;
    hw_write(MMC3_BANK_DATA, a);
    mff_cycles(0xFE73, 6);
    x++;
    hw_write(MMC3_BANK_SELECT, x);
    mff_cycles(0xFE77, 6);
    a = CHR_R1_BOOT;
    hw_write(MMC3_BANK_DATA, a);
    mff_cycles(0xFE7C, 36);
    RAM(CHR_BANKS) = CHR_R0_BOOT;
    RAM(CHR_BANKS + 1) = CHR_R1_BOOT;
    RAM(CHR_BANKS + 2) = 0x00;
    RAM(CHR_BANKS + 3) = 0x01;
    RAM(CHR_BANKS + 4) = 0x02;
    a = 0x03;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(CHR_BANKS + 5) = a;

    /* the screen */
    SYNC_OUT;
    if (!mff_call_at(0x1E96, 0xC31D, -1)) return;          /* sprites_hide_rest */
    SYNC_IN;
    mff_cycles(0xFE97, 12);
    a = NAMETABLE_0;
    x = 0x00;
    y = 0x00;
    n = 0;
    z = 1;
    SYNC_OUT;
    if (!mff_call_at(0x1E9F, 0xC2D1, -1)) return;          /* ppu_fill_nametable */
    SYNC_IN;
    mff_cycles(0xFEA0, 12);
    a = NAMETABLE_1;
    x = 0x00;
    y = 0x00;
    n = 0;
    z = 1;
    SYNC_OUT;
    if (!mff_call_at(0x1EA8, 0xC2D1, -1)) return;
    SYNC_IN;

    /* task 0 at main_task, then the scheduler */
    mff_cycles(0xFEA9, 18);
    RAM(TASK_ENTRY + 1) = (uint8_t)(MAIN_TASK >> 8);
    RAM(TASK_ENTRY) = (uint8_t)MAIN_TASK;
    a = 0x00;
    n = 0;
    z = 1;
    SYNC_OUT;
    if (!mff_call_fn(0x1EB5, func_FF02, 0xE000)) return;   /* task_spawn */
    SYNC_IN;
    mff_cycles(0xFEB6, 5);
    a = PPUCTRL_GAME;
    n = (a) >> 7;
    z = (a) == 0;
    RAM(PPUCTRL_SHADOW) = a;
    SYNC_OUT;
    func_FEBA();                                /* falls into the scheduler */
}

/*
 * $FF4C: an RTS on its own, between task_wait_frame's JMP $FEBA and
 * bank_switch; the area palette setup leaves through JMP $FF4C (0E:C53B).
 */
void func_FF4C(void)
{
    mff_cycles(0xFF4C, 6);
    mff_rts();
}
