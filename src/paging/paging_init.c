/////////////////////////////////////////////////////////////////////////
/* paging_init.c
Functions to initialise the paging mechanism.

-----------------------------
This file is part of kernel.
By Daniel Oertwig
*/
/////////////////////////////////////////////////////////////////////////


#include "paging.h"
#include "bio.h"
#include "pmm.h"

#include "debug-text.h"


//TODO




void Paging_Init (void)
{
	UINT cr0;
	pg_PageTab *kdir;
	
	kdir = pgoff_CreateRawIdentityDir(0,pmm_KernelEnd);

	/* enable paging with the new created directory */
	asm volatile ("mov %0, %%cr3":: "r"((UINT)kdir));
	asm volatile ("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000;
	asm volatile ("mov %0, %%cr0":: "r"(cr0));
}


