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
#include "coroutine.h"

void func_FEBA(void);   /* scheduler, still generated */

/* task_self_index, $FF24: X = A = task_current * 4. */
void func_FF24(void) {
    uint8_t t = RAM(0x91);
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
    RAM(0x82 + x) = RAM(0x93);
    RAM(0x83 + x) = RAM(0x94);
    g_cpu.A = 8;
    mff_nz(8);
    RAM(0x80 + x) = 8;
    mff_rts();
}

/* task_kill, $FF12: task A stops (state 0). No route has run it. */
void func_FF12(void) {
    mff_cycles(0xFF12, 6);
    if (!mff_call(0xFF12, 0xFF26)) return;
    mff_cycles(0xFF15, 2 + 4 + 6);
    g_cpu.A = 0;
    mff_nz(0);
    RAM(0x80 + g_cpu.X) = 0;
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
    RAM(0x80 + g_cpu.X) = 0;
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
    RAM(0x93) = 1;
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
    RAM(0x81 + x) = RAM(0x93);
    RAM(0x80 + x) = 1;
    mff_cycles(0xFF44, 2);
    g_cpu.A = x;                        /* TXA */
    mff_cycles(0xFF45, 2);
    g_cpu.Y = g_cpu.A;                  /* TAY */
    mff_nz(g_cpu.Y);
    mff_cycles(0xFF46, 2);
    g_cpu.X = g_cpu.S;                  /* TSX */
    mff_nz(g_cpu.X);
    mff_cycles(0xFF47, 4);
    RAM(0x82 + g_cpu.Y) = g_cpu.X;
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
