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

#ifndef CTL_LINUXX64_H_
#define CTL_LINUXX64_H_
#if defined(__linux__) && defined(__amd64__)

#include "ctl_base.h"

extern int ctl_linuxx64_save_context(void *);
extern void ctl_linuxx64_restore_context(void *);
extern void ctl_linuxx64_switch_stack(void *, void (*)(void *), void *, void *);

#define CTL_LINUXX64_CTX_SIZE (8 * sizeof(uint64_t))
#define ctl_save_context(ctx) ctl_linuxx64_save_context((ctx)->context)
#define ctl_restore_context(ctx) ctl_linuxx64_restore_context((ctx)->context)

#else
#error "ctl_linuxx64.h included on non-AMD64/Linux platform"
#endif
#endif