# makefile for the kernel

###########################################################################################################
## Vars
CC = gcc
CCFLAGS = -m32 -fno-builtin -fno-leading-underscore -fno-strict-aliasing -fno-stack-protector -Wall -c -O3
NM = nasm
NMFLAGS = -f elf32
LD = ld
LDFLAGS = -m elf_i386 -T linkerscript.ld 

CINCPATH = -I../inc/
OUTPATH = ../build/
###########################################################################################################
## Default target, do everything

all: builddirs allsources kernel


builddirs:
	@mkdir -p ../build/bio
	@mkdir -p ../build/boot
	@mkdir -p ../build/gdt
	@mkdir -p ../build/idt
	@mkdir -p ../build/int
	@mkdir -p ../build/main
	@mkdir -p ../build/paging
	@mkdir -p ../build/phys_mm
	@mkdir -p ../build/text
	@mkdir -p ../build/heap
	



###########################################################################################################
## locate (find) all source files in subdirs and include them as var SOURCES
.PHONY: allsources
allsources:
	@echo -n "[allsources] Searching source files..."
	@echo -n "SOURCES = " > allsources
	@find . -name "*.c" -printf "%p " >> allsources
	@echo "\n" >> allsources
	@echo -n "ASMSRC = " >> allsources
	@find . -name "*_asm.asm" -printf "%p " >> allsources
	@echo "  [OK]"

include allsources
objects = $(patsubst %.c,%.o,$(SOURCES)) $(patsubst %_asm.asm,%_asm.o,$(ASMSRC))
###########################################################################################################
## build and link the kernel
.PHONY: kernel
kernel: $(objects)
	@echo -n "[kernel] Linking object files..."
	@$(LD) $(LDFLAGS) -o $(OUTPATH)/kernel.bin `find $(OUTPATH) -name "*.o" -printf "%p "`
	@echo "  [OK]"


%.o: %.c
	@echo -n "[c] Compiling $<..."
	@$(CC) $(CCFLAGS) $(CINCPATH) $< -o $(OUTPATH)$@
	@echo "  [OK]"

%_asm.o: %_asm.asm
	@echo -n "[asm] Assembling $<..."
	@$(NM) $(NMFLAGS) $< -o $(OUTPATH)$@
	@echo "  [OK]"

###########################################################################################################


