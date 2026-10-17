/*
 * tasks.c - the cooperative tasks (docs/engine.md, "Tasks")
 *
 * Four tasks, 4 bytes each at $80 (tasks): +0 state (0 free, 1 waiting,
 * 2 running, 4 ready, 8 new), +1 frames left to wait, +2/+3 the entry point
 * of a new task or the saved stack pointer of a waiting one. task_current
 * $91 is the running task. Each task has its own part of the stack page;
 * the scheduler ($FEBA) runs whichever is ready, and the NMI counts the
 * waits down (nmi_task_timers).
 *
 * In the scaffold a task is a fiber: task_wait_frame's jump back to the
 * scheduler is coroutine_yield(), and when the task is resumed the routine
 * returns to its caller as the 6502 RTS does.
 */
#include "mff.h"
#include "state.h"
#include "coroutine.h"

void func_FEBA(void);   /* scheduler, still generated */

/* task_self_index, $FF24: X = A = task_current * 4. */
void func_FF24(void) {
    uint8_t t = RAM(TASK_CURRENT);
    mff_cycles(0xFF24, 3 + 2 + 2 + 2 + 6);
    g_cpu.C = (t >> 6) & 1;
    g_cpu.A = g_cpu.X = (uint8_t)(t << 2);
    mff_nz(g_cpu.X);
    mff_rts();
}

/* task_index, $FF26: X = A = A * 4. */
void func_FF26(void) {
    uint8_t t = g_cpu.A;
    mff_cycles(0xFF26, 2 + 2 + 2 + 6);
    g_cpu.C = (t >> 6) & 1;
    g_cpu.A = g_cpu.X = (uint8_t)(t << 2);
    mff_nz(g_cpu.X);
    mff_rts();
}

/* task_spawn, $FF02: task A starts at the entry in $93/$94 the next time
 * the scheduler runs it (state 8). */
void func_FF02(void) {
    uint8_t x;
    mff_cycles(0xFF02, 6);
    if (!mff_call(0xFF02, 0xFF26)) return;
    x = g_cpu.X;
    mff_cycles(0xFF05, 3 + 4 + 3 + 4 + 2 + 4 + 6);
    RAM(TASKS + 2 + x) = RAM(TASK_ENTRY);
    RAM(TASKS + 3 + x) = RAM(TASK_ENTRY + 1);
    g_cpu.A = 8;
    mff_nz(8);
    RAM(TASKS + x) = 8;
    mff_rts();
}

/* task_kill, $FF12: task A stops (state 0). No route has run it. */
void func_FF12(void) {
    mff_cycles(0xFF12, 6);
    if (!mff_call(0xFF12, 0xFF26)) return;
    mff_cycles(0xFF15, 2 + 4 + 6);
    g_cpu.A = 0;
    mff_nz(0);
    RAM(TASKS + g_cpu.X) = 0;
    mff_rts();
}

/* task_exit, $FF1A: the running task stops and the scheduler takes over
 * (the generated code runs it in place). No route has run it. */
void func_FF1A(void) {
    mff_cycles(0xFF1A, 6);
    if (!mff_call(0xFF1A, 0xFF24)) return;
    mff_cycles(0xFF1D, 2 + 4 + 3);
    g_cpu.A = 0;
    mff_nz(0);
    RAM(TASKS + g_cpu.X) = 0;
    func_FEBA();
}

/* task_wait_frame, $FF31: sleep one frame. Saves X and Y on the task's
 * stack, sets the wait ($81+X = $93 = 1) and the state (1, waiting), saves
 * S in $82+X and yields; the scheduler's resume pops Y and X back.
 * Once the state says "waiting" the NMI is no longer held (g_nmi_veto), so
 * from there on every instruction is a place it can land and gets its own
 * charge; so do the pushes, where an interrupt's frame would land
 * differently. */
void func_FF31(void) {
    uint8_t x;
    mff_cycles(0xFF31, 2 + 3 + 2);
    g_cpu.A = 1;
    RAM(TASK_ENTRY) = 1;
    g_cpu.A = g_cpu.X;
    mff_cycles(0xFF36, 3);
    mff_push(g_cpu.A);
    mff_cycles(0xFF37, 2);
    g_cpu.A = g_cpu.Y;
    mff_nz(g_cpu.A);
    mff_cycles(0xFF38, 3);
    mff_push(g_cpu.A);
    mff_cycles(0xFF39, 6);
    if (!mff_call(0xFF39, 0xFF24)) return;
    x = g_cpu.X;
    mff_cycles(0xFF3C, 3 + 4 + 2 + 4);
    RAM(TASKS + 1 + x) = RAM(TASK_ENTRY);
    RAM(TASKS + x) = 1;
    mff_cycles(0xFF44, 2);
    g_cpu.A = x;                        /* TXA */
    mff_cycles(0xFF45, 2);
    g_cpu.Y = g_cpu.A;                  /* TAY */
    mff_nz(g_cpu.Y);
    mff_cycles(0xFF46, 2);
    g_cpu.X = g_cpu.S;                  /* TSX */
    mff_nz(g_cpu.X);
    mff_cycles(0xFF47, 4);
    RAM(TASKS + 2 + g_cpu.Y) = g_cpu.X;
    mff_cycles(0xFF49, 3);
    coroutine_yield();
}

/* task_wait_frames, $FF2A: X frames of task_wait_frame (X kept). */
void func_FF2A(void) {
    for (;;) {
        mff_cycles(0xFF2A, 6);
        mff_call_yield(0xFF2A, 0xFF31);
        g_cpu.X--;
        mff_nz(g_cpu.X);
        if (!g_cpu.Z) {
            mff_cycles(0xFF2D, 2 + 3);
            continue;
        }
        mff_cycles(0xFF2D, 2 + 2 + 6);
        break;
    }
    mff_rts();
}

/* ---- the scheduler, $FEBA ----
 * S = $FF, then scan the tasks from 0: the first one ready (state >= 4)
 * runs - a new one (8) starts at its entry ($82/$83 via $93/$94), a waiting
 * one is resumed (TXS to its saved S, read_joypads for task 0, pull Y and X,
 * RTS into it). When it yields, the scheduler starts over at $FEBA. The NMI
 * sets sched_nmi_flag $90, which makes a scan in progress start again.
 *
 * Written instruction by instruction on g_cpu (no locals): the scaffold
 * takes the NMI at the top of the scan ($FEBD, extras.c), where it saves the
 * registers as they are. */
static void sched_scan(void) {
scan:
    mff_cycles(0xFEBD, 2); g_cpu.X = 0; mff_nz(0);
    mff_cycles(0xFEBF, 3); RAM(SCHED_NMI_FLAG) = g_cpu.X;
    mff_cycles(0xFEC1, 2); g_cpu.Y = 4; mff_nz(4);
    for (;;) {
        mff_cycles(0xFEC3, 4); g_cpu.A = RAM(TASKS + g_cpu.X); mff_nz(g_cpu.A);
        mff_cycles(0xFEC5, 2); g_cpu.C = g_cpu.A >= 4; mff_nz((uint8_t)(g_cpu.A - 4));
        mff_cycles(0xFEC7, 2);
        if (g_cpu.C) { maybe_trigger_vblank(1); break; }
        mff_cycles(0xFEC9, 2); g_cpu.X++; mff_nz(g_cpu.X);
        mff_cycles(0xFECA, 2); g_cpu.X++; mff_nz(g_cpu.X);
        mff_cycles(0xFECB, 2); g_cpu.X++; mff_nz(g_cpu.X);
        mff_cycles(0xFECC, 2); g_cpu.X++; mff_nz(g_cpu.X);
        mff_cycles(0xFECD, 2); g_cpu.Y--; mff_nz(g_cpu.Y);
        mff_cycles(0xFECE, 2);
        if (!g_cpu.Z) { mff_cycles(0xFEC3, 1); continue; }
        mff_cycles(0xFED0, 3);
        goto scan;
    }
    mff_cycles(0xFED3, 3); g_cpu.A = RAM(SCHED_NMI_FLAG); mff_nz(g_cpu.A);
    mff_cycles(0xFED5, 2);
    if (!g_cpu.Z) { mff_cycles(0xFEBD, 1); goto scan; }

    /* task (Y - 1) ^ 3, at X = 4 * task */
    mff_cycles(0xFED7, 2); g_cpu.Y--; mff_nz(g_cpu.Y);
    mff_cycles(0xFED8, 2); g_cpu.A = g_cpu.Y; mff_nz(g_cpu.A);
    mff_cycles(0xFED9, 2); g_cpu.A ^= 3; mff_nz(g_cpu.A);
    mff_cycles(0xFEDB, 3); RAM(TASK_CURRENT) = g_cpu.A;
    mff_cycles(0xFEDD, 4); g_cpu.Y = RAM(TASKS + g_cpu.X); mff_nz(g_cpu.Y);
    mff_cycles(0xFEDF, 2); g_cpu.A = 2; mff_nz(2);
    mff_cycles(0xFEE1, 4); RAM(TASKS + g_cpu.X) = 2;
    mff_cycles(0xFEE3, 2); g_cpu.C = g_cpu.Y >= 8; mff_nz((uint8_t)(g_cpu.Y - 8));
    mff_cycles(0xFEE5, 2);
    if (g_cpu.Z) {                                  /* new: start it */
        uint16_t jt;
        mff_cycles(0xFEE7, 4); g_cpu.A = RAM(TASKS + 2 + g_cpu.X); mff_nz(g_cpu.A);
        mff_cycles(0xFEE9, 3); RAM(TASK_ENTRY) = g_cpu.A;
        mff_cycles(0xFEEB, 4); g_cpu.A = RAM(TASKS + 3 + g_cpu.X); mff_nz(g_cpu.A);
        mff_cycles(0xFEED, 3); RAM(TASK_ENTRY + 1) = g_cpu.A;
        mff_cycles(0xFEEF, 5);
        jt = nes_read16zp(TASK_ENTRY);
        nes_cpu_instruction_boundary(jt, 2);
        coroutine_start(RAM(TASK_CURRENT), jt);
        return;
    }
    maybe_trigger_vblank(1);                        /* waiting: resume it */
    mff_cycles(0xFEF2, 4); g_cpu.A = RAM(TASKS + 2 + g_cpu.X); mff_nz(g_cpu.A);
    mff_cycles(0xFEF4, 2); g_cpu.X = g_cpu.A; mff_nz(g_cpu.X);
    mff_cycles(0xFEF5, 2); g_cpu.S = g_cpu.X;
    mff_cycles(0xFEF6, 3); g_cpu.A = RAM(TASK_CURRENT); mff_nz(g_cpu.A);
    mff_cycles(0xFEF8, 2);
    if (!g_cpu.Z) {
        maybe_trigger_vblank(1);
    } else {
        mff_cycles(0xFEFA, 6);
        if (!mff_call_at(0x1EFC, 0xC279, -1)) return;
    }
    mff_cycles(0xFEFD, 4); g_cpu.A = mff_pull(); mff_nz(g_cpu.A);
    mff_cycles(0xFEFE, 2); g_cpu.Y = g_cpu.A; mff_nz(g_cpu.Y);
    mff_cycles(0xFEFF, 4); g_cpu.A = mff_pull(); mff_nz(g_cpu.A);
    mff_cycles(0xFF00, 2); g_cpu.X = g_cpu.A; mff_nz(g_cpu.X);
    mff_cycles(0xFF01, 6);
    g_cpu.S += 2;                                   /* the RTS into the task */
    coroutine_resume(RAM(TASK_CURRENT));
}

/* scheduler, $FEBA. Never returns (a JMP $FEBA from inside a task restarts
 * the scheduler fiber: coroutine_scheduler_setjmp). */
void func_FEBA(void) {
    for (;;) {
        mff_cycles(0xFEBA, 2); g_cpu.X = 0xFF; mff_nz(0xFF);
        mff_cycles(0xFEBC, 2); g_cpu.S = g_cpu.X;
        coroutine_scheduler_setjmp();
        sched_scan();
    }
}

/* $FEBD, the scan without the stack reset (only the scheduler jumps here) */
void func_FEBD(void) {
    sched_scan();
    func_FEBA();
}
