/////////////////////////////////////////////////////////////////////////
/* main.c
Main routine of the kernel.

-----------------------------
This file is part of kernel.
By Daniel Oertwig
*/
/////////////////////////////////////////////////////////////////////////
#include "gdt.h"
#include "multiboot.h"
#include "pmm_init.h"
#include "pmm.h"
#include "types.h"
#include "debug-text.h"




/* startup routine */
void c_main (UINT eax, UINT* ebx, UINT esp)
{
	UINT memsize_bytes = 0;
	UINT *mmap_add;
	UINT mmap_length = 0;
	
	/* TODO get more detailed info from mboot, such as memory mapping,
	 * to be able to find a good location for GDT and so on */
	if (read_mboot_info (eax, ebx) < 0)	{
		puts("Could not read Multiboot Infor Structure! Am I booted by GRUB?\n");
		return;
	}
	memsize_bytes = mboot_get_memsize_bytes();
	if (memsize_bytes < 8124)	{ /* NOTE 8124 Bytes = about 8 Megabytes = minimal space required) */
		puts("It seems as if there isn't enough memory!\n");
		INT8 buf[20];
		int_to_string(&buf,'d',memsize_bytes);
		puts(buf);
		return;
	}
	pmm_Setup(0x100000,memsize_bytes);
	mmap_add = (UINT*) mboot_get_mmap_add ();
	mmap_length = mboot_get_mmap_length ();
	if ( (!mmap_add) || (!mmap_length) ) {
		/* TODO Print a message */
		return;
	}
	pmm_MarkUsedSpace_mmap (mmap_add, mmap_length);
	GDT_Setup ( pmm_alloc_frame () );
	puts("\nFertig");
	return;
}
