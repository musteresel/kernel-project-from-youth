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


//TODO




void Paging_Init (void)
{
	pg_PageTab *kdir = (pg_PageTab*)pmm_alloc();
	memset32 ( (UINT*) &kdir, 0, sizeof(pg_PageTab) );
	/* TODO: map kernel memory (don't forget the stack) etc */
	
	UINT cr0;
	asm volatile ("mov %0, %%cr3":: "r"(&kdir));
	asm volatile ("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000;
	asm volatile ("mov %0, %%cr0":: "r"(cr0));	
}


