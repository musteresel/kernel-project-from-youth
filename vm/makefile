# makefile for generating vm images of the kernel


.PHONY: bochs
bochs:
	@echo -n "[bochs] Running bochs..."
	@make -C bochs
	@echo "  [OK]"

image: mnt/ ../build/kernel.bin menu.lst
	@echo -n "[image] Mounting floppy.img..."
	@mount -o loop floppy.img mnt/
	@echo "  [OK]"
	@echo -n "[image] Copying files to image..."
	@cp ../build/kernel.bin mnt/kernel.bin
	@cp menu.lst mnt/grub/menu.lst
	@echo "  [OK]"
	@echo -n "[image] Unmounting floppy.img..."
	@umount mnt/
	@echo "  [OK]"

mnt/:
	@echo -n "[mnt/] Creating mount dir mnt/..."
	@mkdir mnt/
	@echo "  [OK]"
