# makefile for generating vm images of the kernel


.PHONY: bochs
bochs: floppy.img
	@echo -n "[bochs] Running bochs..."
	@make -C bochs
	@echo "  [OK]"

image: mnt/ floppy.img ../build/kernel.bin menu.lst
	@echo -n "[image] Mounting floppy.img..."
	@sudo mount -o loop floppy.img mnt/
	@echo "  [OK]"
	@echo -n "[image] Copying files to image..."
	@cp ../build/kernel.bin mnt/kernel.bin
	@cp menu.lst mnt/grub/menu.lst
	@echo "  [OK]"
	@echo -n "[image] Unmounting floppy.img..."
	@sudo umount mnt/
	@echo "  [OK]"

mnt/:
	@echo -n "[mnt/] Creating mount dir mnt/..."
	@mkdir -p mnt/
	@echo "  [OK]"


floppy.img: mnt/
	@echo -n "[floppy.img]Generating floppy.img..."
	@dd if=/dev/zero of=floppy.img bs=1024 count=1440
	@mke2fs -q -F floppy.img
	@sudo mount -o loop floppy.img mnt/
	@mkdir -p mnt/grub
	@cp /boot/grub/stage1 mnt/grub/
	@cp /boot/grub/stage2 mnt/grub/
	@cp menu.lst mnt/grub/
	@sudo umount mnt/
	@echo "device (fd0) floppy.img\nroot (fd0)\nsetup (fd0)\nquit\n" | /usr/sbin/grub
	@echo "  [OK]"
	
