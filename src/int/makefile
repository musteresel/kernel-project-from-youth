# /src/int/
# Makefile for the isr/irq files


normal:
	@echo "Compiling isr/irq files (normal) ..."
	@gcc isr.c -fno-builtin -fno-leading-underscore -fno-strict-aliasing -fno-stack-protector -Wall -O3  -c -I../INCLUDE/
	@gcc irq.c -fno-builtin -fno-leading-underscore -fno-strict-aliasing -fno-stack-protector -Wall -c -I../INCLUDE/
	@nasm -f elf isr_irq_asm.asm



debug:
	@echo "Compiling isr/irq files (debug) ..."
	@gcc isr.c -fno-builtin -fno-leading-underscore -fno-strict-aliasing -fno-stack-protector -Wall -c -I../INCLUDE/
	@gcc irq.c -fno-builtin -fno-leading-underscore -fno-strict-aliasing -fno-stack-protector -Wall -c -I../INCLUDE/
	@nasm -f elf isr_irq_asm.asm


clear:
	@echo "Removing isr/irq object files (rm) ..."
	@rm isr.o
	@rm irq.o
	@rm isr_irq_asm.o


