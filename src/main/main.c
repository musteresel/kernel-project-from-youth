/////////////////////////////////////////////////////////////////////////
/* main.c
Main routine of the kernel.

-----------------------------
This file is part of kernel.
By Daniel Oertwig
*/
/////////////////////////////////////////////////////////////////////////



#include "gdt.h"


void c_main (unsigned int* mboot_ad, unsigned int esp_value)
{
	/* startup routine */
	GDT_Setup ( /* TODO where to put the gdt? */ );
	return;
}
