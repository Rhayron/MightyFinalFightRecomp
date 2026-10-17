/*
 * interrupts.c - NMI, IRQ and the VRAM uploads (docs/engine.md, "NMI and the
 * hijacked return", "IRQ")
 *
 * The NMI turns rendering off, uploads what the game queued (vram_request
 * $64 with the $0780 buffer, the $0780 write lists $19/$1A, the palette at
 * $0600 with $18), OAM DMA from $0200, copies the scroll, maps the CHR
 * banks from their shadows $E8-$ED, arms the MMC3 IRQ for the status-bar
 * split, counts the frame ($92) and the task waits, and returns through
 * nmi_tail ($C119) instead of the interrupted code. When render_off_depth
 * $EE or $9A is set it only does the last part (nmi_busy).
 *
 * The I flag is set inside the NMI and the IRQ (and the runtime never
 * delivers the IRQ during the NMI), so nothing can land in the middle:
 * one charge per block.
 */
#include "mff.h"
#include "state.h"

/* write lists at $0780: [addr hi, addr lo, count-1, bytes...] until a byte
 * with bit 7 set where an address would be. X is the read index. */
static void vram_list(void) {
    for (;;) {
        uint8_t x = g_cpu.X, y;
        mff_cycles(0xC201, 4 + 2);
        g_cpu.A = RAM(VRAM_BUFFER + x);
        mff_nz(g_cpu.A);
        if (g_cpu.N) {
            mff_cycles(0xC204, 1 + 6);
            mff_rts();
            return;
        }
        mff_cycles(0xC206, 4 + 4 + 4 + 4);
        hw_write(PPU_ADDR, g_cpu.A);
        hw_write(PPU_ADDR, RAM(VRAM_BUFFER + 1 + x));
        y = RAM(VRAM_BUFFER + 2 + x);
        do {                                    /* count-1 + 1 bytes */
            mff_cycles(0xC212, 14);
            g_cpu.A = RAM(VRAM_BUFFER + 3 + x);
            hw_write(PPU_DATA, g_cpu.A);
            x++;
            y--;
            if (!(y & 0x80)) mff_cycles(0xC21A, 1);
        } while (!(y & 0x80));
        x = (uint8_t)(x + 3);
        g_cpu.X = x;
        g_cpu.Y = y;
        mff_cycles(0xC21C, 2 + 2 + 2 + 2);
        mff_nz(x);
        if (!x) {
            mff_cycles(0xC221, 6);
            mff_rts();
            return;
        }
        mff_cycles(0xC21F, 1);
    }
}

/* vram_list_flush, $C1FD: the write lists from the start; clears $19. */
void func_C1FD(void) {
    mff_cycles(0xC1FD, 2 + 3);
    g_cpu.X = 0;
    RAM(VRAM_LIST_REQUEST) = 0;
    vram_list();
}

/* vram_list_at, $C201: the write lists from X. */
void func_C201(void) { vram_list(); }

/* nmi_vram_update, $C222: strips from X up to $65 in the $0780 buffer:
 * [count, addr hi, addr lo, bytes...], PPUCTRL from ppuctrl_shadow with the
 * nametable bits cleared; clears $65. */
void func_C222(void) {
    uint8_t x = g_cpu.X, y;
    mff_cycles(0xC222, 4);
    hw_read(PPU_STATUS);
    do {
        mff_cycles(0xC225, 35);
        hw_write(PPU_CTRL, RAM(PPUCTRL_SHADOW) & 0xFE);
        y = RAM(VRAM_BUFFER + x++);
        hw_write(PPU_ADDR, RAM(VRAM_BUFFER + x++));
        hw_write(PPU_ADDR, RAM(VRAM_BUFFER + x++));
        do {
            mff_cycles(0xC23E, 14);
            g_cpu.A = RAM(VRAM_BUFFER + x++);
            hw_write(PPU_DATA, g_cpu.A);
            y--;
            if (y) mff_cycles(0xC246, 1);
        } while (y);
        mff_cycles(0xC248, 5);
        g_cpu.C = x >= RAM(VRAM_BUFFER_LEN);
        if (x != RAM(VRAM_BUFFER_LEN)) mff_cycles(0xC24A, 1);
    } while (x != RAM(VRAM_BUFFER_LEN));
    mff_cycles(0xC24C, 11);
    g_cpu.X = x;
    g_cpu.Y = 0;
    g_cpu.A = 0;
    mff_nz(0);
    RAM(VRAM_BUFFER_LEN) = 0;
    mff_rts();
}

/* nmi_busy, $C09D, and the end of every NMI: scroll and PPUMASK from their
 * shadows, CHR banks R0/R1, the IRQ armed for the status bar ($C000 latch
 * $21, handler $C14A, $9B picks $E000 off or $E001 on), frame_counter,
 * $90 = $FF (the scheduler starts over from task 0), the task waits, and
 * the hijacked return. */
void func_C09D(void) {
    int x, y;

    mff_cycles(0xC09D, 38);
    hw_read(PPU_STATUS);
    hw_write(PPU_SCROLL, 0);
    hw_write(PPU_SCROLL, RAM(SCROLL_Y));
    hw_write(PPU_MASK, RAM(PPUMASK_SHADOW));
    hw_write(PPU_CTRL, (RAM(SCROLL_NT) & 1) | RAM(PPUCTRL_SHADOW));
    for (x = 1; x >= 0; x--) {
        mff_cycles(0xC0BA, 16 + (x ? 1 : 0));
        hw_write(MMC3_BANK_SELECT, (uint8_t)x);
        hw_write(MMC3_BANK_DATA, RAM(CHR_BANKS + x));
    }

    mff_cycles(0xC0C5, 49);
    hw_write(MMC3_BANK_SELECT, RAM(MMC3_SELECT));
    hw_write(MMC3_IRQ_LATCH, 0x21);
    hw_write(MMC3_IRQ_RELOAD, 0x21);
    RAM(IRQ_HANDLER) = 0x4A;
    RAM(IRQ_HANDLER + 1) = 0xC1;
    hw_write(MMC3_IRQ_DISABLE + RAM(IRQ_ENABLE), 0xC1);
    RAM(FRAME_COUNTER)++;
    RAM(SCHED_NMI_FLAG) = 0xFF;

    /* nmi_task_timers, $C0E8 */
    for (x = 0, y = 4; y; x += 4, y--) {
        int c = 8 + 12;
        if (RAM(TASKS + x) != 1) {
            c += 1;
        } else {
            c += 8;
            if (--RAM(TASKS + 1 + x)) c += 1;
            else { c += 6; RAM(TASKS + x) = 4; g_cpu.A = 4; }
        }
        if (y > 1) c += 1;
        mff_cycles(0xC0E8, c);
    }

    /* nmi_hijack_return, $C0FD: the interrupted PC (under the saved Y, X, A
     * and P, then the hardware P) goes to nmi_return_pc $A0/$A1, and the RTI
     * goes to nmi_tail */
    mff_cycles(0xC0FD, 30);
    x = g_cpu.S;                    /* LDA $0107,X: no wrap inside page 1 */
    RAM(NMI_RETURN_PC + 1) = RAM(STACK_PAGE + 7 + x);
    RAM(NMI_RETURN_PC) = RAM(STACK_PAGE + 6 + x);
    RAM(STACK_PAGE + 7 + x) = 0xC1;
    RAM(STACK_PAGE + 6 + x) = 0x19;
    mff_cycles(0xC112, 26);
    g_cpu.Y = mff_pull();
    g_cpu.X = mff_pull();
    g_cpu.A = mff_pull();
    mff_set_p(mff_pull());          /* PLP */
    mff_rti(0xC118);
}

/* nmi, $C000 */
void func_C000(void) {
    mff_cycles(0xC000, 16);
    mff_push(mff_p());
    mff_push(g_cpu.A);
    mff_push(g_cpu.X);
    mff_push(g_cpu.Y);

    mff_cycles(0xC006, 27);
    hw_read(PPU_STATUS);
    hw_write(PPU_CTRL, RAM(PPUCTRL_SHADOW) & 0x7F);
    hw_write(PPU_MASK, 0);
    if (RAM(RENDER_OFF_DEPTH) | RAM(DRAW_BUSY)) {
        mff_jmp_charge(0xC01B, 0xC09D);
        func_C09D();
        return;
    }

    mff_cycles(0xC019, 1);
    mff_cycles(0xC01E, 5);
    if (RAM(VRAM_REQUEST)) {
        mff_cycles(0xC022, 11);
        g_cpu.X = 0;
        RAM(VRAM_REQUEST) = 0;
        if (!mff_call(0xC026, 0xC222)) return;
    } else {
        mff_cycles(0xC020, 1);
    }

    /* the run ends at STA $4014: the DMA's stall is 513 cycles, 514 when
     * the write lands on an odd cycle, so the write must come at its own
     * cycle (charging $C03F-$C041 first put it 5 cycles late, which made
     * the stall 1 cycle off whenever that flipped the parity) */
    mff_cycles(0xC029, 30);
    RAM(IRQ_SCROLL + 1) = RAM(SCROLL_X);
    RAM(IRQ_SCROLL + 2) = RAM(SCROLL_NT);
    RAM(IRQ_SCROLL) = RAM(UNK_F8);
    hw_write(OAM_ADDR, 0);
    hw_write(OAM_DMA, 2);
    mff_cycles(0xC03F, 5);
    if (RAM(VRAM_LIST_REQUEST)) {
        mff_cycles(0xC043, 6);
        if (!mff_call(0xC043, 0xC1FD)) return;
    } else {
        mff_cycles(0xC041, 1);
    }

    mff_cycles(0xC046, 5);
    if (RAM(VRAM_COLUMN_REQUEST)) {
        mff_cycles(0xC04A, 22);
        hw_write(PPU_CTRL, (RAM(PPUCTRL_SHADOW) & 0x7F) | 0x04);
        g_cpu.X = 0;
        RAM(VRAM_COLUMN_REQUEST) = 0;
        if (!mff_call(0xC057, 0xC201)) return;
        mff_cycles(0xC05A, 9);
        hw_write(PPU_CTRL, RAM(PPUCTRL_SHADOW) & 0x7F);
    } else {
        mff_cycles(0xC048, 1);
    }

    mff_cycles(0xC061, 5);
    if (RAM(PALETTE_REQUEST)) {                        /* palette, $0600-$061F */
        int i;
        mff_cycles(0xC065, 21 + 32 * 14 + 31 + 18);
        RAM(PALETTE_REQUEST) = 0;
        hw_read(PPU_STATUS);
        hw_write(PPU_ADDR, 0x3F);
        hw_write(PPU_ADDR, 0x00);
        for (i = 0; i < 32; i++) hw_write(PPU_DATA, RAM(PALETTE_BUF + i));
        hw_write(PPU_ADDR, 0x3F);
        hw_write(PPU_ADDR, 0);
        hw_write(PPU_ADDR, 0);
        hw_write(PPU_ADDR, 0);
    } else {
        mff_cycles(0xC063, 1);
    }

    {                                       /* CHR banks R2-R5 */
        int x;
        mff_cycles(0xC08E, 2);
        for (x = 2; x < 6; x++) {
            mff_cycles(0xC090, 18 + (x < 5 ? 1 : 0));
            hw_write(MMC3_BANK_SELECT, (uint8_t)x);
            hw_write(MMC3_BANK_DATA, RAM(CHR_BANKS + x));
        }
    }
    func_C09D();
}

/* irq, $C13B: acknowledge, then the handler in irq_handler $9C/$9D. */
void func_C13B(void) {
    uint16_t h;
    mff_cycles(0xC13B, 16);
    mff_push(mff_p());
    mff_push(g_cpu.A);
    g_cpu.A = g_cpu.X;
    mff_push(g_cpu.A);
    g_cpu.A = g_cpu.Y;
    mff_nz(g_cpu.A);
    mff_push(g_cpu.A);
    mff_cycles(0xC141, 13);
    hw_write(MMC3_IRQ_DISABLE, g_cpu.A);
    hw_write(MMC3_IRQ_ENABLE, g_cpu.A);
    h = (uint16_t)(RAM(IRQ_HANDLER) | RAM(IRQ_HANDLER + 1) << 8);
    nes_cpu_instruction_boundary(h, 2);
    call_by_address_tail(h, -1);
}

/* the handlers' common end, $C1D8 */
static void irq_return(void) {
    mff_cycles(0xC1D8, 26);
    g_cpu.Y = mff_pull();
    g_cpu.X = mff_pull();
    g_cpu.A = mff_pull();
    mff_set_p(mff_pull());          /* PLP */
    mff_rti(0xC1DE);
}

void func_C1D8(void) { irq_return(); }

/* the status bar's split, $C14A: in round 1 (index 1) areas 1-2 the
 * playfield scrolls 0 below the bar; elsewhere it takes scroll_x. Next
 * handler $C194 or $C1B1 after 8 or 17 more lines ($C000 latch). */
void func_C14A(void) {
    uint8_t r = RAM(ROUND), a = RAM(SUB_AREA);
    int special = r == 1 && (a == 1 || a == 2);
    int c = 7;
    if (r != 1) c += 1;
    else {
        c += 7;
        if (a == 1) c += 1;
        else { c += 4; if (a != 2) c += 1; }
    }
    mff_cycles(0xC14A, c);
    if (special) {
        mff_cycles(0xC15A, 32);
        hw_read(PPU_STATUS);
        hw_write(PPU_SCROLL, 0);
        hw_write(PPU_SCROLL, 0x22);
        hw_write(MMC3_IRQ_LATCH, 0x4A);
        RAM(IRQ_HANDLER) = 0x94;
        RAM(IRQ_HANDLER + 1) = 0xC1;
        g_cpu.A = 0xC1;
    } else {
        mff_cycles(0xC177, 33);
        hw_read(PPU_STATUS);
        hw_write(PPU_SCROLL, RAM(IRQ_SCROLL + 1));
        hw_write(PPU_SCROLL, 0x22);
        hw_write(MMC3_IRQ_LATCH, 0xA1);
        RAM(IRQ_HANDLER) = 0xB1;
        RAM(IRQ_HANDLER + 1) = 0xC1;
        g_cpu.A = 0xC1;
    }
    mff_nz(0xC1);
    mff_jmp_charge(special ? 0xC174 : 0xC191, 0xC1D8);
    irq_return();
}

/* $C194: the second split of those areas, back to scroll_x. */
void func_C194(void) {
    mff_cycles(0xC194, 33);
    hw_read(PPU_STATUS);
    hw_write(PPU_SCROLL, RAM(IRQ_SCROLL + 1));
    hw_write(PPU_SCROLL, 0x22);
    hw_write(MMC3_IRQ_LATCH, 0x57);
    RAM(IRQ_HANDLER) = 0xB1;
    RAM(IRQ_HANDLER + 1) = 0xC1;
    g_cpu.A = 0xC1;
    mff_nz(0xC1);
    mff_jmp_charge(0xC1AE, 0xC1D8);
    irq_return();
}

/* $C1B1: the status bar itself: scroll 0, CHR R0/R1 = $78/$7A, IRQ off. */
void func_C1B1(void) {
    mff_cycles(0xC1B1, 49);
    hw_read(PPU_STATUS);
    hw_write(PPU_SCROLL, 0);
    hw_write(PPU_SCROLL, 0);
    hw_write(MMC3_BANK_SELECT, 0);
    hw_write(MMC3_BANK_DATA, 0x78);
    hw_write(MMC3_BANK_SELECT, 1);
    hw_write(MMC3_BANK_DATA, 0x7A);
    g_cpu.A = RAM(MMC3_SELECT);
    hw_write(MMC3_BANK_SELECT, g_cpu.A);
    hw_write(MMC3_IRQ_DISABLE, g_cpu.A);
    irq_return();
}
