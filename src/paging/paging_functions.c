/////////////////////////////////////////////////////////////////////////
/* paging_functions.c
This file provides functions to controll and use the paging

-----------------------------
This file is part of kernel.
By Daniel Oertwig
*/
/////////////////////////////////////////////////////////////////////////

#include "paging.h"
#include "pmm.h"
#include "types.h"
#include "bio.h"



const UINT8 pg_PRESENT = 1;
const UINT8 pg_RW = 2;
const UINT8 pg_USER = 4;
const UINT8 pg_ACCESSED = 8;
const UINT8 pg_DIRTY = 16;


/*TODO
Function that will be needed:

* Functions to create and delete PageDirectories
* Functions to create and delete PageTables (used by above)
* Functions to physically copy a frame
* Functions to link a frame (but also to unlink a frame)


TODO*/

pg_Page *pg_addtoPT_X (pg_PageTab* dir, UINT16 num, UINT8 flags, UINT frame)
{
	if (dir->entries[num].frame == 0)
	{
		/* TODO no checks made */
		dir->entries[num].frame = ResolveAddressfromFrame (frame)>>12;
		if (flags&pg_PRESENT)
		{
			dir->entries[num].present = 1;
		}
		if (flags&pg_RW)
		{
			dir->entries[num].rw = 1;
		}
		if (flags&pg_USER)
		{
			dir->entries[num].user = 1;
		}
		if (flags&pg_ACCESSED)
		{
			dir->entries[num].accessed = 1;
		}
		if (flags&pg_DIRTY)
		{
			dir->entries[num].dirty = 1;
		}
	}
	return (pg_Page*)&dir->entries[num];
}

pg_Page *pg_addtoPT (pg_PageTab* dir, UINT16 num, UINT8 flags)
{
	UINT frame = pmm_alloc_frame();
	if (frame==pmm_ret_MEMFULL)
	{
		return 0;
	} else
	{
		memset32 ( (UINT*)ResolveAddressfromFrame(frame), 0, 1024);
		return pg_addtoPT_X (dir, num, flags, frame);
	}
}		

INT8 pg_MapVirtPhys (pg_PageTab *dir, UINT* VirtStart, UINT count, UINT8 flags, UINT PhysStart)
{
	UINT16 PageStart;
	UINT16 TabStart;
	UINT16 TabEnd;
	UINT16 PageEnd;
	pg_Page *pagetmp;
	UINT frametmp;
	
	TabStart = (UINT)VirtStart>>22;
	TabEnd = ((UINT)VirtStart + (count<<12))>>22;
	
	PageStart = ((UINT)VirtStart>>12) & ~(TabStart<<10);
	PageEnd = ((UINT)VirtStart + (count<<12))>>12 & ~(TabEnd<<10);
	
	frametmp = ResolveFramefromAddress(PhysStart);
	
	
	
	for (; TabStart < TabEnd; TabStart++)
	{
		pagetmp = pg_addtoPT(dir,TabStart,flags);
		if (!pagetmp)
		{
			asm volatile ("hlt");
		}
		for (;PageStart < 1024; PageStart++)
		{
			pg_addtoPT_X((pg_PageTab *)(pagetmp->frame << 12),PageStart,flags,frametmp);
			frametmp++;
		}
		PageStart = 0;
	}
	
	
	
	return 1;
}

//void pg_MapPage ( (UINT*) Virt, UINT8 flags, UINT Phys );
