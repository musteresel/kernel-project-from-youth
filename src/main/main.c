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
#include "types.h"




/* startup routine */
void c_main (UINT eax, UINT* ebx, UINT esp)
{
	UINT memsize_bytes = 0;
	
	/* TODO get more detailed info from mboot, such as memory mapping,
	 * to be able to find a good location for GDT and so on */
	if (read_mboot_info (eax, ebx) < 0)	{
		/* TODO Print a message */
		return;
	}
	memsize_bytes = mboot_get_memsize_bytes();
	if (memsize_bytes < 8388608)	{ /* NOTE 8388608 Bytes = 8 Megabytes = minimal space required) */
		/* TODO Print a message */
		return;
	}
	pmm_Setup(0x100000,memsize_bytes);
	GDT_Setup ( 0x150000 );
	return;
}
