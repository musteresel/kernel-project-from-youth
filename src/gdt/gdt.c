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


/** struct for a pointer to the gdt **/
struct gdt_ptr
{
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

/** struct for a tss entry **/
struct tss_entry_struct
{
	unsigned int prev_tss;
	unsigned int esp0;
	unsigned int ss0;
	unsigned int esp1;
	unsigned int ss1;
	unsigned int esp2;
	unsigned int ss2;
	unsigned int cr3;
	unsigned int eip;
	unsigned int eflags;
	unsigned int eax;
	unsigned int ecx;
	unsigned int edx;
	unsigned int ebx;
	unsigned int esp;
	unsigned int ebp;
	unsigned int esi;
	unsigned int edi;
	unsigned int es;
	unsigned int cs;
	unsigned int ss;
	unsigned int ds;
	unsigned int fs;
	unsigned int gs;
	unsigned int ldt;
	unsigned short trap;
	unsigned short iomap_base;
} __attribute__((packed));

/** global objects **/
unsigned int *GDT;
struct gdt_ptr pGDT;
struct tss_entry_struct TSS;

/** function to set a gate in the gdt **/
void GDT_SetGate (unsigned int num, unsigned int base, unsigned int limit, unsigned char access, unsigned char gran)
{
	unsigned int *tmp;
	
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
void WriteTSS (unsigned int num, unsigned int ss0, unsigned int esp0)
{
	unsigned int base = (unsigned int)&TSS;
	unsigned int lim = base + sizeof(struct tss_entry_struct);
	GDT_SetGate (num,base,lim,0xE9,0x00);
	memset8 ((unsigned int*)&TSS,0,sizeof(struct tss_entry_struct));
	TSS.ss0 = ss0;
	TSS.esp0 = esp0;
	TSS.cs = 0x0B;
	TSS.ss = TSS.ds = TSS.es = TSS.fs = TSS.gs = 0x13;
}


/** function to set up the gdt **/
void GDT_Setup (unsigned int ad)
{
	unsigned short max_size = 8;
	
	GDT = (unsigned int*) ad;
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





