/////////////////////////////////////////////////////////////////////////
/* paging_functions.c
This file provides functions to controll and use the paging

-----------------------------
This file is part of kernel.
By Daniel Oertwig
*/
/////////////////////////////////////////////////////////////////////////


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








