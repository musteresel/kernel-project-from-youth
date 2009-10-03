/////////////////////////////////////////////////////////////////////////
/* paging_functions.c
This file provides functions to controll and use the paging

-----------------------------
This file is part of kernel.
By Daniel Oertwig
*/
/////////////////////////////////////////////////////////////////////////


#include "bio.h"
#include "debug-text.h"
#include "pmm.h"
#include "paging.h"
#include "types.h"


/*TODO
Function that will be needed:

* Functions to create and delete PageDirectories
* Functions to create and delete PageTables (used by above)
* Functions to physically copy a frame
* Functions to link a frame (but also to unlink a frame)


TODO*/



/*********** usable at every time ******************/
INT8 pg_setEntry(pg_PageTab *where, UINT num, pg_Page what)
{
	if (where->entries[num].frame == 0)
	{
		where->entries[num].frame = what.frame;
		where->entries[num].present = what.present;
		where->entries[num].rw = what.rw;
		where->entries[num].user = what.user;
		where->entries[num].accessed = what.accessed;
		where->entries[num].dirty = what.dirty;
		return 1;
	} else
	{
		return 0;
	}
}


pg_Page pg_getEntry(pg_PageTab *where, UINT num)
{
	return where->entries[num];
}

/*********** helper functions ******************/
UINT16 ResolveTabNumfromAddress (UINT add)
{
	return ( add >> 22  );
}

UINT ResolveAddressfromTabNum (UINT16 tab)
{
	return ( tab << 22 );
}

UINT16 ResolvePageNumfromAddress (UINT add)
{
	return ( (add >> 12) & ~(ResolveTabNumfromAddress(add)<<10)  );
}

void EnablePaging (UINT cr3)
{
	UINT cr0 = 0;
	asm volatile ("mov %0, %%cr3":: "r"(cr3));
	asm volatile ("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000;
	asm volatile ("mov %0, %%cr0":: "r"(cr0));
	return;
}

UINT DisablePaging ()
{
	UINT cr0 = 0;
	UINT cr3 = 0;
	asm volatile ("mov %%cr3, %0": "=r"(cr3));
	asm volatile ("mov %%cr0, %0": "=r"(cr0));
	cr0 &= ~0x80000000;
	asm volatile ("mov %0, %%cr0":: "r"(cr0));
	return cr3;
}

/*********** useable only when paging is turned off ******************/
void pgoff_IdentityMapMemory( pg_PageTab *dir, UINT start, UINT end)
{
	pgoff_MapMemory(dir,start,end,start);
}


void pgoff_MapMemory( pg_PageTab *dir, UINT start, UINT end, UINT phys_start)
{
	UINT16 firstTab;
	UINT16 firstPage;
	
	UINT16 lastTab;
	UINT16 lastPage;
	
	UINT mapframe;
	
	UINT tmp;
	UINT counter;
	UINT sec_count;
	UINT sec_count2;
	
	pg_PageTab *tmptab;
	pg_Page tmppage;
	
	/* resolve addresses */
	firstTab = ResolveTabNumfromAddress (start);
	firstPage = ResolvePageNumfromAddress (start);
	lastTab = ResolveTabNumfromAddress (end);
	lastPage = ResolvePageNumfromAddress (end);
	mapframe = ResolveFramefromAddress (phys_start);
	
	counter = firstTab;
	for (; counter <= lastTab; counter++)
	{
		tmppage = pg_getEntry(dir,counter);
		if (tmppage.frame == 0) {
			tmp = pmm_alloc_frame ();
			if (tmp == pmm_ret_MEMFULL)
			{
				puts("There isn't enough memory to set up Page Tables!\n");
				asm volatile ("hlt");
			}
			tmppage.frame = ResolveAddressfromFrame ( tmp) >> 12;
			tmppage.rw = 0;
			tmppage.user = 0;
			tmppage.present = 1;
			pg_setEntry(dir,counter,tmppage);
		} else {
			tmp = ResolveFramefromAddress (tmppage.frame << 12);
		}
		tmptab = (pg_PageTab*) ResolveAddressfromFrame (tmp);
		if (counter==firstTab)
		{
			sec_count = firstPage;
		} else {
			sec_count = 0;
		}
		if (counter==lastTab)
		{
			sec_count2 = lastPage;
		} else {
			sec_count2 = 1023;
		}
		for (; sec_count <= sec_count2; sec_count++)
		{
			tmppage.rw = 0;
			tmppage.user = 0;
			tmppage.present = 1;
			tmppage.frame = (ResolveAddressfromFrame (mapframe)) >> 12;
			mapframe++;
			pg_setEntry(tmptab,sec_count,tmppage);
		}
	}
}


pg_PageTab *pgoff_CreateRawDir ()
{
	pg_PageTab *dir;
	UINT tmp;
	
	/* allocate space for the dir */
	tmp = pmm_alloc_frame();
	if (tmp == pmm_ret_MEMFULL)
	{
		puts("Not enough memory to create page directory!\n");
		asm volatile ("hlt");
	}
	dir = (pg_PageTab *)ResolveAddressfromFrame (tmp);
	memset32 ( (UINT*)dir, 0, sizeof(pg_PageTab)>>2);
	return dir;
}
	
	
/*********** usable only when paging is enabled AND PageDir is remapped in itself ******************/

const UINT REMAPPED_PAGEDIR = 0xFF800000;

void SetPTEntry(UINT PT, UINT16 num, UINT FrameAd, UINT16 Flags)
{
    UINT *tmp =(UINT*) ( (REMAPPED_PAGEDIR) | ((PT)<<12) );
    tmp = tmp + num;
    *tmp = (FrameAd & 0xFFFFF000) | (Flags & 0xE67);
}
UINT GetPTEntry(UINT PT, UINT16 num)
{
    UINT *tmp =(UINT*) ( (REMAPPED_PAGEDIR) | ((PT)<<12) );
    tmp+=num;
    return (*tmp);
}


inline void invTLB(UINT ad)
{
	asm volatile ("invlpg %0"::"m" (*(char *)ad));
}

void MapVirtPhys(UINT virt, UINT phys, UINT16 flags)
{
    UINT pt = ResolveTabNumfromAddress(virt);
    if (GetPTEntry(1023,pt) == 0) {
        SetPTEntry(1023,pt, ResolveAddressfromFrame(pmm_alloc_frame()), flags);
        invTLB(ResolveAddressfromTabNum(pt));
    }
    SetPTEntry(pt, ResolvePageNumfromAddress(virt) , phys, flags);
    invTLB(virt);
}

