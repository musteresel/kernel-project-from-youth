# /src/idt/
# Makefile for the IDT


normal:
	@echo "Compiling idt file (normal) ..."
	@gcc idt.c -fno-builtin -fno-leading-underscore -fno-strict-aliasing -fno-stack-protector -Wall -O3  -c -I../INCLUDE/
	@nasm -f elf idt_asm.asm



debug:
	@echo "Compiling idt file (debug) ..."
	@gcc idt.c -fno-builtin -fno-leading-underscore -fno-strict-aliasing -fno-stack-protector -Wall -c -I../INCLUDE/
	@nasm -f elf idt_asm.asm


clear:
	@echo "Removing idt object file (rm) ..."
	@rm idt.o
	@rm idt_asm.o

