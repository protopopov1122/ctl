;    SPDX short identifier: MIT
;
;    Copyright 2020 Jevgēnijs Protopopovs
;
;    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
;    to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
;    and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
;
;    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
;
;    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
;    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
;    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
;    IN THE SOFTWARE.

global ctl_linuxx64_save_context
global ctl_linuxx64_restore_context
global ctl_linuxx64_switch_stack

section .text

ctl_linuxx64_save_context:
    mov [rax], rbx
    mov [rax+8], rsp
    mov [rax+16], rbp
    mov [rax+24], r12
    mov [rax+32], r13
    mov [rax+40], r14
    mov [rax+48], r15
    mov rcx, [rsp]
    mov [rax+56], rcx
    mov rax, 0
    ret

ctl_linuxx64_restore_context:
    mov rbx, [rax]
    mov rsp, [rax + 8]
    mov rbp, [rax + 16]
    mov r12, [rax + 24]
    mov r13, [rax + 32]
    mov r14, [rax + 40]
    mov r15, [rax + 48]
    mov rcx, [rax + 56]
    mov [rsp], rcx
    mov rax, 1
    ret

ctl_linuxx64_switch_stack:
    mov rsp, rdx
    mov rbp, rdx
    xor r12, r12
    mov r12, rcx
    call rsi
    mov rax, r12
    jmp ctl_linuxx64_restore_context