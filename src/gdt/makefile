# /src/gdt/
# Makefile for the GDT


normal:
	@echo "Compiling gdt file (normal) ..."
	@gcc gdt.c -fno-builtin -fno-leading-underscore -fno-strict-aliasing -fno-stack-protector -Wall -O3  -c -I../INCLUDE/
	@nasm -f elf gdt_asm.asm



debug:
	@echo "Compiling gdt file (debug) ..."
	@gcc gdt.c -fno-builtin -fno-leading-underscore -fno-strict-aliasing -fno-stack-protector -Wall -c -I../INCLUDE/
	@nasm -f elf gdt_asm.asm


clear:
	@echo "Removing gdt object file (rm) ..."
	@rm gdt.o
	@rm gdt_asm.o

