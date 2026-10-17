/*
 * coroutine.h - the game's four cooperative tasks as C coroutines
 *
 * The scheduler ($FEBA, port/src/game/tasks.c) gives each task its own part
 * of the 6502 stack and switches between them with TXS. In C a task is a
 * coroutine with a C stack of its own: the scheduler starts or resumes it,
 * task_wait_frame yields back. Implemented in runtime/coroutine.c (fibers
 * on Windows, ucontext elsewhere).
 */
#ifndef MFF_COROUTINE_H
#define MFF_COROUTINE_H

#include <stdint.h>

#define COROUTINE_MAX_CHANNELS 4

/* The scheduler's loop top. On the scheduler's own stack it starts the
 * coroutine system (first call) and drops every task after a restart; called
 * from inside a task (the task jumped to the scheduler, e.g. JMP reset) it
 * abandons that task's C stack and goes back to the scheduler. */
int coroutine_scheduler_setjmp(void);

/* From a task: back to the scheduler; returns when the task is resumed. */
void coroutine_yield(void);

/* From the scheduler: run task `channel` until it yields. */
void coroutine_resume(int channel);

/* From the scheduler: a new task `channel` entering at `addr`. */
void coroutine_start(int channel, uint16_t addr);

#endif
