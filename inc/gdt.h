// Global Descriptor Table

#ifndef __KERNEL__GDT___HEADER____
#define __KERNEL__GDT___HEADER____

#include "types.h"


/** struct for a tss entry **/
struct tss_entry_struct
{
	UINT prev_tss;
	UINT esp0;
	UINT ss0;
	UINT esp1;
	UINT ss1;
	UINT esp2;
	UINT ss2;
	UINT cr3;
	UINT eip;
	UINT eflags;
	UINT eax;
	UINT ecx;
	UINT edx;
	UINT ebx;
	UINT esp;
	UINT ebp;
	UINT esi;
	UINT edi;
	UINT es;
	UINT cs;
	UINT ss;
	UINT ds;
	UINT fs;
	UINT gs;
	UINT ldt;
	UINT16 trap;
	UINT16 iomap_base;
} __attribute__((packed));

extern void GDT_Setup (UINT);
extern struct tss_entry_struct TSS;


#endif //(__KERNEL__GDT___HEADER____)
