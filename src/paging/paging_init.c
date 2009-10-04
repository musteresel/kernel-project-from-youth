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

/** Markers defined by the linkerscript **/
extern UINT __StartOfLinkedMemory;
extern UINT __EndOfLinkedMemory;
extern UINT __RealStartOfLinkedMemory;

/** Global variables **/
pg_PageTab *linkedDirectory;
const UINT KPC_START =   0xFFFFD000;

/** Setting up paging **/
void InitPaging ()
{
	pg_PageTab *kdir;
	UINT StartOfLinkedMemory = (UINT)&__StartOfLinkedMemory;
	UINT EndOfLinkedMemory = (UINT)&__EndOfLinkedMemory;
	UINT RealStartOfLinkedMemory = (UINT)&__RealStartOfLinkedMemory;
	UINT pointer;

	pointer = ResolveAddressfromFrame (pmm_alloc_frames(3));
	CheckForSure(pointer,"Could not allocate space for KPC!");


	/// Create linked directory
	linkedDirectory = pgoff_CreateRawDir();
	pgoff_MapMemory(linkedDirectory,StartOfLinkedMemory,EndOfLinkedMemory,RealStartOfLinkedMemory);
	pgoff_MapMemory(linkedDirectory,KPC_START,0xFFFFFFF0,pointer);
	
	/// Create directory used by the kernel
	kdir = pgoff_CreateRawDir();
	pgoff_MapMemory(kdir,StartOfLinkedMemory,EndOfLinkedMemory,RealStartOfLinkedMemory);
	pgoff_MapMemory(kdir,KPC_START,0xFFFFFFF0,pointer);
	pointer = ResolveAddressfromFrame (pmm_alloc_frames(KHEAP_size>>10));
	CheckForSure(pointer,"Could not allocate space for KHEAP!");
	pgoff_MapMemory(kdir,KHEAP_start,KHEAP_start+KHEAP_size,pointer);
	pgoff_IdentityMapMemory(kdir,0xB8000,pmm_KernelEnd); // Mapping everything between Video memory and the end of the kernel
	kdir->entries[pg_PAGEDIRNUM].frame = (UINT)kdir >> 12; // The directory is remapped to be able changing mapping when paging is enabled
	kdir->entries[pg_PAGEDIRNUM].present = 1;


	EnablePaging((UINT)kdir);
	create_heap(&KHeap, KHEAP_start, KHEAP_start+KHEAP_size, 0, 0);
}


