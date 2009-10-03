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
#include "kernelheap.h"

#include "debug-text.h"


extern UINT linked_start;
extern UINT linked_end;
extern UINT id_end;


pg_PageTab *ldir;
UINT KPC_startadd;


void Paging_Init (void)
{
	pg_PageTab *kdir;
	UINT lstart = (UINT)&linked_start;
	UINT lend = (UINT)&linked_end;
	UINT real_start = (UINT)&id_end;
	
	UINT pointer = ResolveAddressfromFrame (pmm_alloc_frames(4));
	if (!pointer) {
		puts ("Could not allocate space for KPC\n");
		asm volatile ("hlt");
	}
	KPC_startadd = pointer;
	
	ldir = pgoff_CreateRawDir();
	pgoff_MapMemory(ldir,lstart,lend,real_start);
	pgoff_MapMemory(ldir,0xFFFFC000,0xFFFFFFF0,pointer);
	
	kdir = pgoff_CreateRawDir();
	pgoff_MapMemory(kdir,lstart,lend,real_start);
	pgoff_MapMemory(kdir,0xFFFFC000,0xFFFFFFF0,pointer);
	pointer = ResolveAddressfromFrame (pmm_alloc_frames(KHEAP_size>>10));
	if (!pointer) {
		puts ("Could not allocate space for KHEAP\n");
		asm volatile ("hlt");
	}
	pgoff_MapMemory(kdir,KHEAP_start,KHEAP_start+KHEAP_size,pointer);
	pgoff_IdentityMapMemory(kdir,0,pmm_KernelEnd);
	EnablePaging((UINT)kdir);
	create_heap(&KHeap, KHEAP_start, KHEAP_start+KHEAP_size, 0, 0);
}


