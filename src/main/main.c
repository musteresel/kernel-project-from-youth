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





void c_main (unsigned int eax, unsigned int* ebx, unsigned int esp)
{
	/* startup routine */
	read_mboot_info (eax, ebx);
	GDT_Setup ( /* TODO where to put the gdt? */ );
	return;
}
