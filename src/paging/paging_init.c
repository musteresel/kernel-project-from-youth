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
	UINT tmp;
	pg_Page tmppage;
	pg_PageTab *kdir;
	UINT lastTab;
	UINT lastPage;
	UINT16 counter = 0;
	UINT16 sec_count = 0;
	pg_PageTab * tmptab;
	UINT cr0;

	/* setting up memory for the directory */
	tmp = pmm_alloc_frame();
	if (tmp == pmm_ret_MEMFULL)
	{
		puts("There isn't enough memory for the kernel page directory!\n");
		asm volatile ("hlt");
	}
	kdir = (pg_PageTab*)ResolveAddressfromFrame(tmp);
	memset32 ( (UINT*) kdir, 0, sizeof(pg_PageTab)/4);
	/* TODO: map multiboot info */


	/* findig the end of used memory (TODO could be better) */
	lastTab = ResolveTabNumfromAddress ( (UINT)kdir +0x1000 );
	lastPage = ResolvePageNumfromAddress ( (UINT)kdir +0x1000);

	for (; counter <= lastTab; counter++)
	{
		tmp = pmm_alloc_frame ();
		if (tmp == pmm_ret_MEMFULL)
		{
			puts("There isn't enough memory to set up Page Tables for the kernel!\n");
			asm volatile ("hlt");
		}
		tmppage.frame = ResolveAddressfromFrame ( tmp) >> 12;
		tmppage.rw = 0;
		tmppage.user = 0;
		tmppage.present = 1;
		pg_setEntry(kdir,counter,tmppage);
		tmptab = (pg_PageTab*) ResolveAddressfromFrame (tmp);
		sec_count = 0;
		if (counter!=lastTab)
		{
			for (; sec_count < 1023; sec_count++)
			{
				tmppage.rw = 0;
				tmppage.user = 0;
				tmppage.present = 1;
				tmppage.frame = ((sec_count*0x1000)+ResolveAddressfromTabNum(counter)) >> 12;
				pg_setEntry(tmptab,sec_count,tmppage);
			}
		} else
		{
			for (; sec_count <= lastPage; sec_count++)
			{
				tmppage.rw = 0;
				tmppage.user = 0;
				tmppage.present = 1;
				tmppage.frame = ((sec_count*0x1000)+ResolveAddressfromTabNum(counter)) >> 12;
				pg_setEntry(tmptab,sec_count,tmppage);
			}
		}
	}

	/* setting last table to the directory, so that it can be modified without complex mapping */
	tmppage.frame = ((UINT)kdir) >> 12;
	tmppage.rw = 0;
	tmppage.user = 0;
	tmppage.present = 1;
	pg_setEntry(kdir,1023,tmppage);

	/* enable paging with the new created directory */
	asm volatile ("mov %0, %%cr3":: "r"((UINT)kdir));
	asm volatile ("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000;
	asm volatile ("mov %0, %%cr0":: "r"(cr0));
}


