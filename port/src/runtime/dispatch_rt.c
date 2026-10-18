/*
 * dispatch_rt.c - jumps whose target is only known at run time
 *
 * port/src/glue/dispatch.c maps an address to the routine for the bank the
 * MMC3 has there (call_by_address_cb). Around it:
 *
 *  - nes_dispatch_call runs the target with the code window set to the one
 *    the address is in, so the JSRs inside push true CPU return addresses;
 *  - call_by_address_tail runs a JMP. A JMP is a C call too, so a loop made
 *    of JMPs would add a C frame per lap. Each tail being run is listed with
 *    its target and S; a tail that comes back to a listed target with the
 *    same S is a lap (no net change to the 6502 stack): it returns at once,
 *    the C frames of the lap unwind, and the listed tail runs the target
 *    again from its own loop. A tail entered before an interrupt still
 *    being serviced matches whatever S is now (a handler jumping straight
 *    back into the loop it interrupted).
 *  - a target with no routine stops the run: the port has no interpreter.
 */
#include "machine.h"
#include <stdio.h>
#include <stdlib.h>

uint16_t g_code_window_base = 0xE000;

int nes_dispatch_call(uint16_t addr, int caller_bank) {
    uint16_t saved = g_code_window_base;
    int r;
    if (addr >= 0x8000) g_code_window_base = addr & 0xE000;
    r = call_by_address_cb(addr, caller_bank);
    g_code_window_base = saved;
    return r;
}

#define MAX_TAILS 64

static struct {
    uint16_t addr;
    uint8_t s;
    uint64_t epoch;
} s_tail[MAX_TAILS];
static int s_ntails;
static int32_t s_lap_addr = -1;         /* a lap waiting for the tail at s_lap_slot */
static int s_lap_slot = -1;

static int is_lap(int i, uint16_t addr) {
    if (s_tail[i].addr != addr) return 0;
    if (s_tail[i].s == g_cpu.S) return 1;
    return s_tail[i].epoch < timing_interrupt_epoch() && (timing_depth() > 0 || timing_in_irq());
}

int call_by_address_tail(uint16_t addr, int caller_bank) {
    int slot = -1, r;
    for (int i = 0; i < s_ntails; i++) {
        if (is_lap(i, addr)) {
            s_lap_addr = addr;
            s_lap_slot = i;
            return 1;
        }
    }
    if (s_ntails < MAX_TAILS) {
        slot = s_ntails++;
        s_tail[slot].addr = addr;
        s_tail[slot].s = g_cpu.S;
        s_tail[slot].epoch = timing_interrupt_epoch();
    }
    for (;;) {
        r = nes_dispatch_call(addr, caller_bank);
        if (slot >= 0 && s_lap_addr >= 0 && s_lap_slot == slot) {
            s_lap_addr = -1;
            s_lap_slot = -1;
            continue;
        }
        break;
    }
    if (slot >= 0) s_ntails = slot;
    return r;
}

void dispatch_reset_tails(void) {
    s_ntails = 0;
    s_lap_addr = -1;
    s_lap_slot = -1;
}

static int no_routine(uint16_t cpu_addr, int bank) {
    uint8_t s = g_cpu.S;
    uint16_t ret = (uint16_t)(g_ram[0x100 + (uint8_t)(s + 1)] | (g_ram[0x100 + (uint8_t)(s + 2)] << 8));
    if (bank >= 0)
        mff_fatal("no routine for $%04X with bank $%02X there (frame %llu, S=$%02X, the stack's top "
                  "return address $%04X). Every jump target needs a routine in port/src/game: "
                  "port/tools/find_entries.py lists the missing ones, port/tools/lift.py writes them",
                  cpu_addr, g_mmc3_win_bank8k[(cpu_addr >> 13) & 3], (unsigned long long)g_frame_count,
                  s, (uint16_t)(ret + 1));
    mff_fatal("no routine for $%04X (frame %llu, S=$%02X, the stack's top return address $%04X): "
              "code outside PRG ROM, which the port does not run",
              cpu_addr, (unsigned long long)g_frame_count, s, (uint16_t)(ret + 1));
    return 0;
}

int nes_interp_dispatch(uint16_t addr) { return no_routine(addr, -1); }

int nes_interp_dispatch_bank(uint16_t cpu_addr, uint16_t gen_addr, int bank) {
    (void)gen_addr;
    return no_routine(cpu_addr, bank);
}
