/*
    SPDX short identifier: MIT

    Copyright 2020 Jevgēnijs Protopopovs

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

#ifndef CTL_BASE_H_
#define CTL_BASE_H_

#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

// Generic
typedef struct ctl_context {
    void *context;
    size_t context_size;
    void *stack;
    size_t stack_size;
#ifdef CTL_VALGRIND_STACK
    int valgrind_stack_id;
#endif
} ctl_context_t;

ctl_context_t *ctl_new_base_context(size_t, size_t);
void ctl_free_base_context(ctl_context_t *);
void ctl_spawn_context(ctl_context_t *, ctl_context_t *, void (*)(void *), void *);

// Platform-dependent
ctl_context_t *ctl_new_context(size_t);
void ctl_free_context(ctl_context_t *);
void ctl_exec_context(ctl_context_t *, ctl_context_t *, void (*)(void *), void *);

typedef struct ctl_thread {
    struct ctl_scheduler *sched;
    bool active;
    ctl_context_t *ctx;
    void (*entry)(struct ctl_thread *, void *);
    void *arg;
    struct ctl_thread *prev, *next;
} ctl_thread_t;

typedef struct ctl_scheduler {
    ctl_context_t *main_context;
    ctl_thread_t *threads;
} ctl_scheduler_t;

void ctl_scheduler_init(ctl_scheduler_t *);
void ctl_scheduler_free(ctl_scheduler_t *);
void ctl_scheduler_spawn(ctl_scheduler_t *, void (*)(ctl_thread_t *, void *), void *, size_t);
void ctl_scheduler_yield(ctl_thread_t *);
void ctl_scheduler_start(ctl_scheduler_t *);

#endif