/*
 * coroutine.c - each of the game's tasks runs on a C stack of its own
 *
 * The scheduler runs on the program's main stack. coroutine_start gives a
 * new task a stack and enters it through the dispatcher at its entry point;
 * coroutine_yield (task_wait_frame) goes back to the scheduler, which later
 * continues the task where it yielded with coroutine_resume. A task that
 * returns instead of yielding is finished and its stack freed.
 *
 * The game restarts the scheduler from inside a task (title timeout: JMP
 * reset, which ends in the scheduler). On the console that only resets S;
 * here the task's C stack is abandoned: coroutine_scheduler_setjmp, called
 * on a task's stack, switches to the scheduler for good, and the
 * scheduler's next pass through its loop top frees every task and clears
 * the interrupt depth and the JMP tails the abandoned frames had listed
 * (dispatch_rt.c). Left listed, the new main_task's first JMP ($C5CE with
 * S = $BF) matched the old one's as a lap and ended the task: the game
 * froze when the title ran out (attract mode restarts the game this way).
 *
 * Windows uses fibers; elsewhere ucontext (macOS still has it, deprecated,
 * behind _XOPEN_SOURCE).
 */
#if defined(__APPLE__) && !defined(_XOPEN_SOURCE)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE
#endif
#include "coroutine.h"
#include "machine.h"
#include <stdio.h>
#include <stdlib.h>

#define TASK_STACK (8u << 20)

static int s_running;                   /* the scheduler has started */
static int s_current = -1;              /* the task last started or resumed */
static int s_finished = -1;             /* a task that returned without yielding */
static int s_restart;                   /* a task jumped to the scheduler */
static uint16_t s_entry;                /* a new task's entry point */

static void check_resumed_s(uint8_t before) {
    /* the scheduler's resume pulls Y and X and RTSes into the task: S is 4
     * above the yield's */
    if (g_cpu.S != (uint8_t)(before + 4))
        fprintf(stderr, "[mff] task %d resumed with S=$%02X, expected $%02X (frame %llu)\n",
                s_current, g_cpu.S, (uint8_t)(before + 4), (unsigned long long)g_frame_count);
}

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static LPVOID s_sched;
static LPVOID s_task[COROUTINE_MAX_CHANNELS];

static void CALLBACK task_main(LPVOID arg) {
    int ch = (int)(intptr_t)arg;
    call_by_address(s_entry);
    for (;;) {                          /* finished: the scheduler frees this stack */
        s_finished = ch;
        SwitchToFiber(s_sched);
    }
}

static void drop(int ch) {
    if (s_task[ch]) {
        DeleteFiber(s_task[ch]);
        s_task[ch] = NULL;
    }
}

int coroutine_scheduler_setjmp(void) {
    if (!s_running) {
        s_running = 1;
        if (!s_sched) s_sched = ConvertThreadToFiber(NULL);
        if (!s_sched) mff_fatal("ConvertThreadToFiber failed (%lu)", (unsigned long)GetLastError());
    }
    if (GetCurrentFiber() != s_sched) {  /* on a task's stack: leave it behind */
        s_restart = 1;
        SwitchToFiber(s_sched);
        mff_fatal("an abandoned task was resumed");
    }
    if (s_restart) {
        s_restart = 0;
        for (int ch = 0; ch < COROUTINE_MAX_CHANNELS; ch++) drop(ch);
        s_current = -1;
        timing_reset_depth();
        dispatch_reset_tails();
    }
    return 0;
}

void coroutine_yield(void) {
    uint8_t before = g_cpu.S;
    if (!s_running || !s_sched) {
        fprintf(stderr, "[mff] a task yielded with no scheduler running\n");
        return;
    }
    SwitchToFiber(s_sched);
    check_resumed_s(before);
}

void coroutine_resume(int ch) {
    if (ch < 0 || ch >= COROUTINE_MAX_CHANNELS) {
        fprintf(stderr, "[mff] resume: no task %d\n", ch);
        return;
    }
    if (!s_task[ch]) return;            /* it finished: nothing to continue */
    s_current = ch;
    s_finished = -1;
    SwitchToFiber(s_task[ch]);
    if (s_finished == ch) {
        drop(ch);
        s_finished = -1;
    }
}

void coroutine_start(int ch, uint16_t addr) {
    if (ch < 0 || ch >= COROUTINE_MAX_CHANNELS) {
        fprintf(stderr, "[mff] start: no task %d\n", ch);
        return;
    }
    drop(ch);
    s_entry = addr;
    s_current = ch;
    s_finished = -1;
    s_task[ch] = CreateFiberEx(64 << 10, TASK_STACK, 0, task_main, (LPVOID)(intptr_t)ch);
    if (!s_task[ch]) mff_fatal("CreateFiber failed (%lu)", (unsigned long)GetLastError());
    SwitchToFiber(s_task[ch]);
    if (s_finished == ch) {
        drop(ch);
        s_finished = -1;
    }
}

#else
#include <ucontext.h>
#ifdef __APPLE__
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

static ucontext_t s_sched;
static ucontext_t s_ctx[COROUTINE_MAX_CHANNELS];
static void *s_stack[COROUTINE_MAX_CHANNELS];
static int s_in_task;

static void task_main(void) {
    int ch = s_current;
    call_by_address(s_entry);
    for (;;) {
        s_finished = ch;
        s_in_task = 0;
        swapcontext(&s_ctx[ch], &s_sched);
    }
}

static void drop(int ch) {
    free(s_stack[ch]);
    s_stack[ch] = NULL;
}

int coroutine_scheduler_setjmp(void) {
    s_running = 1;
    if (s_in_task) {
        int ch = s_current;
        s_restart = 1;
        s_in_task = 0;
        swapcontext(&s_ctx[ch], &s_sched);
        mff_fatal("an abandoned task was resumed");
    }
    if (s_restart) {
        s_restart = 0;
        for (int ch = 0; ch < COROUTINE_MAX_CHANNELS; ch++) drop(ch);
        s_current = -1;
        timing_reset_depth();
        dispatch_reset_tails();
    }
    return 0;
}

void coroutine_yield(void) {
    int ch = s_current;
    uint8_t before = g_cpu.S;
    if (!s_running) {
        fprintf(stderr, "[mff] a task yielded with no scheduler running\n");
        return;
    }
    s_in_task = 0;
    swapcontext(&s_ctx[ch], &s_sched);
    s_in_task = 1;
    check_resumed_s(before);
}

static void enter(int ch) {
    s_finished = -1;
    s_in_task = 1;
    swapcontext(&s_sched, &s_ctx[ch]);
    s_in_task = 0;
    if (s_finished == ch) {
        drop(ch);
        s_finished = -1;
    }
}

void coroutine_resume(int ch) {
    if (ch < 0 || ch >= COROUTINE_MAX_CHANNELS) {
        fprintf(stderr, "[mff] resume: no task %d\n", ch);
        return;
    }
    if (!s_stack[ch]) return;
    s_current = ch;
    enter(ch);
}

void coroutine_start(int ch, uint16_t addr) {
    if (ch < 0 || ch >= COROUTINE_MAX_CHANNELS) {
        fprintf(stderr, "[mff] start: no task %d\n", ch);
        return;
    }
    drop(ch);
    s_entry = addr;
    s_current = ch;
    s_stack[ch] = malloc(TASK_STACK);
    if (!s_stack[ch]) mff_fatal("no memory for a task stack");
    getcontext(&s_ctx[ch]);
    s_ctx[ch].uc_stack.ss_sp = s_stack[ch];
    s_ctx[ch].uc_stack.ss_size = TASK_STACK;
    s_ctx[ch].uc_link = NULL;
    makecontext(&s_ctx[ch], task_main, 0);
    enter(ch);
}
#endif
