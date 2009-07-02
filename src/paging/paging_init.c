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
	pg_PageTab *kdir;
	kdir = pgoff_CreateRawIdentityDir(0,pmm_KernelEnd);
	EnablePaging((UINT)kdir);
}


