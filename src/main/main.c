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




/* startup routine */
void c_main (UINT eax, UINT* ebx, UINT esp)
{
	UINT memsize_kbytes = 0;
	UINT *mmap_add;
	UINT mmap_length = 0;
	INT8 numbuf[14];
	UINT *pointer;
	
	/* TODO get more detailed info from mboot, such as memory mapping,
	 * to be able to find a good location for GDT and so on */
	if (read_mboot_info (eax, ebx) < 0)	{
		puts("Could not read Multiboot Structure! Am I booted by GRUB?\n");
		return;
	}
	memsize_kbytes = mboot_get_memsize_kbytes();
	if (memsize_kbytes < 8124)	{
		puts("It seems as if there isn't enough memory (about 8MB needed)!\n");
		int_to_string((INT8*)&numbuf,'d',memsize_kbytes);
		puts(numbuf);
		return;
	}
	pmm_Setup(0x100000,memsize_kbytes<<10);
	mmap_add = (UINT*) mboot_get_mmap_add ();
	mmap_length = mboot_get_mmap_length ();
	if ( (!mmap_add) || (!mmap_length) ) {
		puts("Could not get information about the memorymap from GRUB!\n");
		return;
	}
	pmm_MarkUsedSpace_mmap (mmap_add, mmap_length);
	/*TODO: this seems to be a waste of memory... */
	pointer = pmm_alloc();
	if ( !pointer )
	{
		puts("For some reason there could not be memory allocated for the GDT!\n");
		return;
	}
	GDT_Setup ( (UINT)pointer );
	/* TODO: critical... is there free mem? AND: pmm_alloc_frames is BAD... */
	pointer = (UINT*) ResolveAddressfromFrame (pmm_alloc_frames(2));
	if ( !pointer )
	{
		puts("For some reason there could not be memory allocated for the IDT!\n");
		return;
	}
	IDT_Setup ( (UINT)pointer );
	puts("\nFertig");
	return;
}
