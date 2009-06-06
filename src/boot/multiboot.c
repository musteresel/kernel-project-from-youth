/////////////////////////////////////////////////////////////////////////
/* multiboot.c
Holds information passed by the multiboot info structure

-----------------------------
This file is part of kernel.
By Daniel Oertwig
*/
/////////////////////////////////////////////////////////////////////////
#include "multiboot.h"
#include "bio.h"




multiboot_info mboot;


char read_mboot_info (unsigned int eax, unsigned int *ebx)
{
	if (eax != 0x2BADB002)	{
		return -1;
	}
	memcpy8 ((unsigned int*)&mboot,ebx,sizeof(multiboot_info));
	// more handling in spezial functions, they have to be called (anytime) after this function
	return 0;
}

