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


extern UINT linked_start;
extern UINT linked_end;
extern UINT id_end;


pg_PageTab *ldir;

void Paging_Init (void)
{
	pg_PageTab *kdir;
	UINT lstart = (UINT)&linked_start;
	UINT lend = (UINT)&linked_end;
	UINT real_start = (UINT)&id_end;
	
	kdir = pgoff_CreateRawDir();
	ldir = pgoff_CreateRawDir();
	pgoff_MapMemory(ldir,lstart,lend,real_start);
	pgoff_MapMemory(kdir,lstart,lend,real_start);
	pgoff_IdentityMapMemory(kdir,0,pmm_KernelEnd);
	EnablePaging((UINT)kdir);
}


