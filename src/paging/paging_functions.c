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

void pgoff_IdentityMapMemory( pg_PageTab *dir, UINT start, UINT end)
{
	UINT16 firstTab;
	UINT16 firstPage;
	
	UINT16 lastTab;
	UINT16 lastPage;
	
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
			/* no check needed because we are IDENTITY mapping -> mapping twice isn't that bad */
			tmppage.rw = 0;
			tmppage.user = 0;
			tmppage.present = 1;
			tmppage.frame = ((sec_count<<12)+ResolveAddressfromTabNum(counter)) >> 12;
			pg_setEntry(tmptab,sec_count,tmppage);
		}
	}
}


pg_PageTab *pgoff_CreateRawIdentityDir (UINT start, UINT end)
{
	pg_PageTab *dir;
	pg_Page tmppage;
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
	
	pgoff_IdentityMapMemory (dir,start,end);
	
	/* setting last table to the directory, so that it can be modified without complex mapping (TODO neccessary??) */
	tmppage.frame = ((UINT)dir) >> 12;
	tmppage.rw = 0;
	tmppage.user = 0;
	tmppage.present = 1;
	pg_setEntry(dir,1023,tmppage);
	
	return dir;
}
	
	



