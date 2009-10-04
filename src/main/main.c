/////////////////////////////////////////////////////////////////////////
/* main.c
Main routine of the kernel.

-----------------------------
This file is part of kernel.
By Daniel Oertwig
*/
/////////////////////////////////////////////////////////////////////////
#include "idt.h"
#include "gdt.h"
#include "multiboot.h"
#include "pmm_init.h"
#include "pmm.h"
#include "types.h"
#include "debug-text.h"
#include "isr_irq.h"
#include "paging.h"
#include "kernelheap.h"
#include "multitasking.h"



/* startup routine */
INTO_SECTION(void c_main (UINT eax, UINT* ebx, UINT esp),".setup")
{
	UINT memsize_kbytes = 0;
	UINT *mmap_add;
	UINT mmap_length = 0;
	INT8 numbuf[14];
	UINT *pointer;
	
	/* TODO get more detailed info from mboot, such as memory mapping,
	 * to be able to find a good location for GDT and so on */
	CheckForSure((read_mboot_info (eax, ebx) >= 0),"Could not read Multiboot Structure! Am I booted by GRUB?");
	memsize_kbytes = mboot_get_memsize_kbytes();
	CheckForSure((memsize_kbytes >= 8124),"Not enough memory (about 8MB needed)!");
	int_to_string(numbuf,'d',memsize_kbytes);
	puts("\nDetected ");puts(numbuf);puts(" KB of memory...\n");

	pmm_Setup(0x100000,memsize_kbytes<<10);
	mmap_add = (UINT*) mboot_get_mmap_add ();
	mmap_length = mboot_get_mmap_length ();
	CheckForSure((mmap_add && mmap_length),"Could not get memorymap from GRUB!");
	pmm_MarkUsedSpace_mmap (mmap_add, mmap_length);

	/*TODO: this seems to be a waste of memory... */
	pointer = pmm_alloc();
	CheckForSure(pointer,"Could not allocate memory for the GDT!");
	GDT_Setup ( (UINT)pointer );

	/* TODO: critical... is there free mem? AND: pmm_alloc_frames is BAD... */
	pointer = (UINT*) ResolveAddressfromFrame (pmm_alloc_frames(2));
	CheckForSure(pointer,"Could not allocate memory for the IDF!");
	IDT_Setup ((UINT)pointer );
	pmm_KernelEnd = (UINT)pointer + 0x2000;

	ISR_Setup();
	IRQ_Setup();
	InitPaging();
	InitMultitasking();
	asm volatile ("sti");
	puts("\nFertig");
	for(;;);
	return;
}
