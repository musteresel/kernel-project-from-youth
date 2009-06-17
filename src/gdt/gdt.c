/////////////////////////////////////////////////////////////////////////
/* gdt.c
Global Descriptor Table seutp and intialisation

-----------------------------
This file is part of kernel.
By Daniel Oertwig
*/
/////////////////////////////////////////////////////////////////////////



/** extern functions **/
extern void GDT_flush();
extern void TSS_flush();
#include "bio.h"
#include "types.h"


/** struct for a pointer to the gdt **/
struct gdt_ptr
{
	UINT16 limit;
	UINT base;
} __attribute__((packed));

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

/** global objects **/
UINT *GDT;
struct gdt_ptr pGDT;
struct tss_entry_struct TSS;

/** function to set a gate in the gdt **/
void GDT_SetGate (UINT num, UINT base, UINT limit, UINT8 access, UINT8 gran)
{
	UINT *tmp;
	
	tmp = GDT;
	tmp += num * 2;
	
	*tmp = (limit & 0xFFFF);
	*tmp |= (base & 0xFFFF) << 16;
	tmp++;
	*tmp = (base & 0xFF0000) >> 16;
	*tmp |= (access & 0xFF) << 8;
	*tmp |= ( (gran&0xF0) | ((limit & 0xF0000)>>16)) << 16;
	*tmp |= (base & 0xFF000000);
}

/** function to write the tss and to set the entry in the gdt **/
void WriteTSS (UINT num, UINT ss0, UINT esp0)
{
	UINT base = (UINT)&TSS;
	UINT lim = base + sizeof(struct tss_entry_struct);
	GDT_SetGate (num,base,lim,0xE9,0x00);
	memset8 ((UINT*)&TSS,0,sizeof(struct tss_entry_struct));
	TSS.ss0 = ss0;
	TSS.esp0 = esp0;
	TSS.cs = 0x0B;
	TSS.ss = TSS.ds = TSS.es = TSS.fs = TSS.gs = 0x13;
}


/** function to set up the gdt **/
void GDT_Setup (UINT ad)
{
	UINT16 max_size = 8;
	
	GDT = (UINT*) ad;
	memset32 (GDT, 0, max_size*2);
	pGDT.limit = (8 * max_size) - 1;
	pGDT.base = ad;
	
	GDT_SetGate (0,0,0,0,0);
	GDT_SetGate (1,0,0xFFFFFFFF,0x9A,0xCF);
	GDT_SetGate (2,0,0xFFFFFFFF,0x92,0xCF);
	GDT_SetGate (3,0,0xFFFFFFFF,0xFA,0xCF);
	GDT_SetGate (4,0,0xFFFFFFFF,0xF2,0xCF);
	WriteTSS (5,0x13,0x00);
	
	GDT_flush ();
	TSS_flush ();
}





