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

#include "ctl_linuxx64.h"
#ifdef CTL_VALGRIND_STACK
#include <valgrind/valgrind.h>
#endif


ctl_context_t *ctl_new_context(size_t stack_size) {
    ctl_context_t *ctx = ctl_new_base_context(CTL_LINUXX64_CTX_SIZE, stack_size);
#ifdef CTL_VALGRIND_STACK
    ctx->valgrind_stack_id = VALGRIND_STACK_REGISTER(ctx->stack, ((char *) ctx->stack) + ctx->stack_size);
#endif
    return ctx;
}

void ctl_free_context(ctl_context_t *ctx) {
#ifdef CTL_VALGRIND_STACK
    VALGRIND_STACK_DEREGISTER(ctx->valgrind_stack_id);
#endif
    ctl_free_base_context(ctx);
}

void ctl_exec_context(ctl_context_t *orig, ctl_context_t *next, void (*entry)(void *), void *arg) {
    if (!ctl_save_context(orig)) {
        ctl_linuxx64_switch_stack(arg, entry, ((char *) next->stack) + next->stack_size, orig->context);
    }
}