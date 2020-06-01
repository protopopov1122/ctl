SRCDIR=source
INCDIR=headers
CC=gcc
CFLAGS=-std=c18 -Wall -Wextra -pedantic -I$(INCDIR) -ggdb -DCTL_VALGRIND_STACK
ASM=nasm
ASMFLAGS=-f elf64 -F dwarf -g
EXE=ctl
SRCS=$(SRCDIR)/main.c $(SRCDIR)/ctl_base.c $(SRCDIR)/ctl_linuxx64.c

all: $(EXE)

clean:
	rm -rf *.o *.d .depend $(EXE)

depend: .depend

.depend: $(SRCS)
	$(CC) $(CFLAGS) -MM $^ > .depend

include .depend

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ 

%.asm.o: $(SRCDIR)/%.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(EXE): ctl_base.o ctl_linuxx64.o ctl_linuxx64.asm.o main.o
	$(CC) $^ -o $@

.PHONY: all clean depend