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

#include "ctl_base.h"
#include "ctl_platform.h"
#include <string.h>

ctl_context_t *ctl_new_base_context(size_t context_sz, size_t stack_sz) {
    const size_t total_sz = sizeof(ctl_context_t) + context_sz + stack_sz;
    ctl_context_t *ctx = (ctl_context_t *) malloc(total_sz);
    if (ctx == NULL) {
        return NULL;
    }
    ctx->context_size = context_sz;
    if (ctx->context_size > 0) {
        ctx->context = ((char *) ctx) + sizeof(ctl_context_t);
        memset(ctx->context, 0, ctx->context_size);
    }
    ctx->stack_size = stack_sz;
    if (ctx->stack_size > 0) {
        ctx->stack = ((char *) ctx) + sizeof(ctl_context_t) + ctx->context_size;
        memset(ctx->stack, 0, ctx->stack_size);
    }
    return ctx;
}

void ctl_free_base_context(ctl_context_t *ctx) {
    free(ctx);
}

struct ctl_spawned_context_params {
    ctl_context_t *current;
    void (*entry)(void *);
    void *arg;
};

static void ctl_spawn_context_entry(void *raw) {
    struct ctl_spawned_context_params *params = raw;
    ctl_context_t *current = params->current;
    void (*entry)(void *) = params->entry;
    void *arg = params->arg;
    if (ctl_save_context(current)) {
        entry(arg);
    }
}

void ctl_spawn_context(ctl_context_t *orig, ctl_context_t *next, void (*entry)(void *), void *arg) {
    struct ctl_spawned_context_params params = {
        .current = next,
        .entry = entry,
        .arg = arg
    };
    ctl_exec_context(orig, next, ctl_spawn_context_entry, &params);
}

void ctl_thread_free(ctl_thread_t *thread) {
    if (thread->next != NULL) {
        thread->next->prev = thread->prev;
    }
    if (thread->prev != NULL) {
        thread->prev->next = thread->next;
    }
    if (thread->sched->threads == thread) {
        thread->sched->threads = thread->next;
    }
    ctl_free_context(thread->ctx);
    free(thread);
}

void ctl_scheduler_init(ctl_scheduler_t *sched) {
    if (sched == NULL) {
        return;
    }
    sched->main_context = ctl_new_context(0);
    sched->threads = NULL;
}

void ctl_scheduler_free(ctl_scheduler_t *sched) {
    if (sched == NULL) {
        return;
    }
    while (sched->threads != NULL) {
        ctl_thread_free(sched->threads);
    }
    ctl_free_context(sched->main_context);
    sched->main_context = NULL;
}

void ctl_scheduler_entry(void *raw) {
    ctl_thread_t *thread = (ctl_thread_t *) raw;
    thread->entry(thread, thread->arg);
    thread->active = false;
    ctl_restore_context(thread->sched->main_context);
}

void ctl_scheduler_spawn(ctl_scheduler_t *sched, void (*entry)(ctl_thread_t *, void *), void *arg, size_t stack_size) {
    ctl_thread_t *thread = malloc(sizeof(ctl_thread_t));
    if (thread == NULL) {
        return;
    }
    thread->sched = sched;
    thread->active = true;
    thread->entry = entry;
    thread->arg = arg;
    thread->ctx = ctl_new_context(stack_size);
    thread->prev = NULL;
    thread->next = sched->threads;
    sched->threads = thread;
    ctl_spawn_context(sched->main_context, thread->ctx, ctl_scheduler_entry, thread);
}

static ctl_thread_t *ctl_next_thread(ctl_scheduler_t *sched) {
    if (sched->threads == NULL) {
        return NULL;
    }
    ctl_thread_t *thread = sched->threads;
    if (thread->next == NULL) {
        sched->threads = thread;
    } else {
        sched->threads = thread->next;
        sched->threads->prev = NULL;
        ctl_thread_t *last_thread = thread->next;
        while (last_thread->next != NULL) {
            last_thread = last_thread->next;
        }
        last_thread->next = thread;
        thread->prev = last_thread;
        thread->next = NULL;
    }
    if (!thread->active) {
        ctl_thread_free(thread);
        return ctl_next_thread(sched);
    } else {
        return thread;
    }
}

void ctl_scheduler_yield(ctl_thread_t *current_thread) {
    if (current_thread == NULL) {
        return;
    }
    if (ctl_save_context(current_thread->ctx)) {
        return;
    }
    ctl_thread_t *thread = ctl_next_thread(current_thread->sched);
    if (thread == NULL) {
        ctl_restore_context(current_thread->sched->main_context);
    } else {
        ctl_restore_context(thread->ctx);
    }
}

void ctl_scheduler_start(ctl_scheduler_t *sched) {
    if (sched == NULL) {
        return;
    }
    while (sched->threads != NULL) {
        ctl_thread_t *thread = ctl_next_thread(sched);
        if (thread != NULL && !ctl_save_context(sched->main_context)) {
            ctl_restore_context(thread->ctx);
        }
    }
}