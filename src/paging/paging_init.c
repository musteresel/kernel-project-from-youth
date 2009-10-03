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
const UINT KPC_START =   0xFFFFD000;

void Paging_Init (void)
{
	pg_PageTab *kdir;
	UINT lstart = (UINT)&linked_start;
	UINT lend = (UINT)&linked_end;
	UINT real_start = (UINT)&id_end;
	
	UINT pointer = ResolveAddressfromFrame (pmm_alloc_frames(3));
	if (!pointer) {
		puts ("Could not allocate space for KPC\n");
		asm volatile ("hlt");
	}
	KPC_startadd = pointer;
	
	/*Create link Dir */
	ldir = pgoff_CreateRawDir();
	pgoff_MapMemory(ldir,lstart,lend,real_start);
	pgoff_MapMemory(ldir,KPC_START,0xFFFFFFF0,pointer);
	
	/*Create kernel dir */
	kdir = pgoff_CreateRawDir();
	// linked memory
	pgoff_MapMemory(kdir,lstart,lend,real_start);
	// KPC memory
	pgoff_MapMemory(kdir,KPC_START,0xFFFFFFF0,pointer);
	// HEAP
	pointer = ResolveAddressfromFrame (pmm_alloc_frames(KHEAP_size>>10));
	if (!pointer) {
		puts ("Could not allocate space for KHEAP\n");
		asm volatile ("hlt");
	}
	pgoff_MapMemory(kdir,KHEAP_start,KHEAP_start+KHEAP_size,pointer);
	// kernel code
	pgoff_IdentityMapMemory(kdir,0,pmm_KernelEnd);
	// the dir itself, used to change mapping later when paging is enabled
	{
		kdir->entries[1022].frame = (UINT)kdir >> 12;
		kdir->entries[1022].present = 1;
	}
	EnablePaging((UINT)kdir);

	create_heap(&KHeap, KHEAP_start, KHEAP_start+KHEAP_size, 0, 0);
}


