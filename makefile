## Main makefile for the kernel project, used to controll various tasks




kernel:
	@echo "[---------COMPILING KERNEL---------]"
	@make -C src/
	@echo "-------->> DONE"

clean:
	@echo "[---------CLEANING build DIR---------]"
	@echo -n "[clean] Removing object files and the compiled kernel..."
	@rm -rf `find build/ -name "*.o" -printf "%p "`
	@rm -rf `find build/ -name "*.bin" -printf "%p "`
	@echo "  [OK]"
	@echo "-------->> DONE"
