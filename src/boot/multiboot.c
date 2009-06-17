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



/** space for the info (will be copied into these structures) **/
multiboot_info mboot;


/** functions to get the info **/
INT8 read_mboot_info (UINT eax, UINT *ebx)
{
	if (eax != 0x2BADB002)	{
		return -1;
	}
	memcpy8 ((UINT*)&mboot,ebx,sizeof(multiboot_info));
	// more handling in spezial functions, they have to be called (anytime) after this function
	return 0;
}

UINT mboot_get_memsize_bytes (void)
{
	if (mboot.flags&0x1) {
		return mboot.mem_upper;
	} else {
		return 0;
	}
}
