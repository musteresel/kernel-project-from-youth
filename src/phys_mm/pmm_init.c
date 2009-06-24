/////////////////////////////////////////////////////////////////////////
/* pmm_init.c
Setup of the physical memory management

-----------------------------
This file is part of kernel.
By Daniel Oertwig
*/
/////////////////////////////////////////////////////////////////////////


#include "pmm.h"
#include "bio.h"
#include "multiboot.h"

#include "types.h"



// ############################################################
extern UINT *__kernel_start;
extern UINT *__kernel_end;



// ############################################################
/** memory management **/
UINT pmm_Start;
const UINT pmm_ret_MEMFULL = 0;
UINT pmm_InfoStart;
UINT pmm_InfoEnd;

/** supermap **/
UINT pmm_SMapStart;
UINT pmm_SMapSize;

/** maps **/
UINT pmm_MapsStart;
UINT pmm_CountMaps;
UINT pmm_MapSize;
const UINT pmm_SIZE_OF_MAPS_MB = 8;
const UINT pmm_FRAMES_PER_MAP_DIVISOR = 11; //(NOTE depends on value pmm_SIZE_OF_MAPS)
const UINT pmm_FRAMES_PER_MAP = 8 * 256; //(NOTE depends on value pmm_SIZE_OF_MAPS)

UINT pmm_LastFrame = 0;

UINT pmm_KernelStart;
UINT pmm_KernelEnd;



// ############################################################
/** setting up the phys. memory management **/
void pmm_Setup (UINT base, UINT size)
{
	UINT *wr;
/*	UINT tmp1;*/
/*	UINT tmp2;*/
	pmm_KernelStart = (UINT)&__kernel_start;
	pmm_KernelEnd = (UINT)&__kernel_end;

	
	pmm_Start = base;
	pmm_InfoStart = pmm_KernelEnd + 16;
	pmm_CountMaps = size / (pmm_SIZE_OF_MAPS_MB << 20);
	pmm_MapSize = pmm_FRAMES_PER_MAP >> 5;
	pmm_SMapSize = (pmm_CountMaps + 31) >> 5;
	
	wr = (UINT*) pmm_InfoStart;
	*wr = pmm_CountMaps;
	wr++;
	*wr = size >> 10;
	wr++;
	*wr = pmm_SMapSize;
	wr++;
	
	pmm_SMapStart = (UINT) wr;
	wr += pmm_SMapSize;
	wr++;
	pmm_MapsStart = (UINT) wr;
	wr += pmm_MapSize * pmm_CountMaps;
	wr++;
	pmm_InfoEnd = (UINT) wr;
	
	pmm_LastFrame = ResolveFramefromAddress (base + size);
	
	memset32 ((UINT*)pmm_SMapStart,0,pmm_InfoEnd-pmm_SMapStart);
	
	/* Not good. Prefer the use of the memorymap provided by grub! *
	tmp1 = ResolveFramefromAddress ( __kernel_space_start );
	tmp2 = ResolveFramefromAddress ( pmm_InfoEnd );
	for (; tmp1 <= tmp2; tmp1++) {
		use_frame (tmp1);
// 		NOTE the map ist not checked! TODO (!!!)
	}*/
	pmm_KernelEnd = pmm_InfoEnd;
	
	use_memrange ( pmm_KernelStart, pmm_InfoEnd - pmm_KernelStart );
	
	return;
}



void pmm_MarkUsedSpace_mmap (UINT *mmap_add, UINT mmap_length)
{
	multiboot_memory_map *mmap;
	UINT mmap_end_add = (UINT)mmap_add + mmap_length;
	
	for (mmap = (multiboot_memory_map*)mmap_add; (UINT)mmap < mmap_end_add; 
				mmap = (multiboot_memory_map*)((UINT) mmap + mmap->size + sizeof(mmap->size)) )
	{
		if (mmap->base_addr >= 0x100000) { //NOTE: Ignore everything that's not above 1 MB
			if (mmap->type != 1) {
				//not free
				/*//DEBUG
				INT8 buf[14];
				puts("Found NON free memory: ");
				int_to_string(buf,'x',mmap->base_addr);
				puts(buf); puts(" len: ");
				int_to_string(buf,'x',mmap->length);
				puts(buf);puts(" type: ");
				int_to_string(buf,'d',mmap->type);
				puts(buf);puts("\n");*/
				use_memrange ((UINT)mmap->base_addr, (UINT)mmap->length);
			}
		}
		/*TODO: mark memory used by the map as used (or copy relevant parts to a save place) */
	}
	return;
}



// ############################################################
