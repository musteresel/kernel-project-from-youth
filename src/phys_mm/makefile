# /src/phys_mm/
# Makefile for the phys. memory management


normal:
	@echo "Compiling pmm files (normal) ..."
	@gcc -fno-builtin -fno-leading-underscore -fno-strict-aliasing -fno-stack-protector -Wall -O3  -c -I../INCLUDE/ pmm_functions.c
	@gcc -fno-builtin -fno-leading-underscore -fno-strict-aliasing -fno-stack-protector -Wall -O3  -c -I../INCLUDE/ pmm_init.c



debug:
	@echo "Compiling pmm files (debug) ..."
	@gcc -fno-builtin -fno-leading-underscore -fno-strict-aliasing -fno-stack-protector -Wall -c -I../INCLUDE/ pmm_functions.c
	@gcc -fno-builtin -fno-leading-underscore -fno-strict-aliasing -fno-stack-protector -Wall -c -I../INCLUDE/ pmm_init.c


clear:
	@echo "Removing pmm object files (rm) ..."
	@rm pmm_init.o
	@rm pmm_functions.o

