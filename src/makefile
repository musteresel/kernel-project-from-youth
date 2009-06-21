# makefile for the kernel

###########################################################################################################
## Vars
CC = gcc
CCFLAGS = -m32 -fno-builtin -fno-leading-underscore -fno-strict-aliasing -fno-stack-protector -Wall -c 
NM = nasm
NMFLAGS = -f elf
LD = ld
LDFLAGS = -m elf_i386 -T linkerscript.ld 

CINCPATH = -I../inc/
OUTPATH = ../build/
###########################################################################################################
## Default target, do everything

all: allsources kernel



###########################################################################################################
## locate (find) all source files in subdirs and include them as var SOURCES
.PHONY: allsources
allsources:
	@echo -n "SOURCES = " > allsources
	@find . -name "*.c" -printf "%p " >> allsources
	@echo "\n" >> allsources
	@echo -n "ASMSRC = " >> allsources
	@find . -name "*_asm.asm" -printf "%p " >> allsources

include allsources
objects = $(patsubst %.c,%.o,$(SOURCES)) $(patsubst %_asm.asm,%_asm.o,$(ASMSRC))
###########################################################################################################
## build and link the kernel
.PHONY: kernel
kernel: $(objects)
	@$(LD) $(LDFLAGS) -o $(OUTPATH)/kernel.bin `find $(OUTPATH) -name "*.o" -printf "%p "`


%.o: %.c
	@$(CC) $(CCFLAGS) $(CINCPATH) $< -o $(OUTPATH)$@

%_asm.o: %_asm.asm
	@$(NM) $(NMFLAGS) $< -o $(OUTPATH)$@

###########################################################################################################


