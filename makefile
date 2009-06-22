## Main makefile for the kernel project, used to controll various tasks


kernel:
	@echo "[---------COMPILING KERNEL---------]"
	@make -C src/
	@echo "-------->> DONE"


kernelimage: kernel
	@make -C vm/ image


b-run:
	@make -C vm/ bochs

.PHONY: clean
clean:
	@echo "[---------CLEANING build DIR---------]"
	@echo -n "[clean] Removing object files and the compiled kernel..."
	@rm -rf `find build/ -name "*.o" -printf "%p "`
	@rm -rf `find build/ -name "*.bin" -printf "%p "`
	@echo "  [OK]"
	@echo "-------->> DONE"

.PHONY: clean-all
clean-all: clean
	@echo "[---------CLEANING vm DIR---------]"
	@echo -n "[clean-all] Removing logs and boot images..."
	@rm -rf `find vm/ -name "*.log" -printf "%p "`
	#@rm -rf `find vm/ -name "*.img" -printf "%p "`
	@echo "  [OK]"
	@echo "-------->> DONE"
