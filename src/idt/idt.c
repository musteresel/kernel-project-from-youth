/////////////////////////////////////////////////////////////////////////
/* idt.c
Interrupt Descriptor Table setup and controlling

-----------------------------
This file is part of kernel.
By Daniel Oertwig
*/
/////////////////////////////////////////////////////////////////////////


/** extern functions **/
extern void IDT_load();
#include "bio.h"


/** structure for a pointer to the idt **/
struct idt_ptr
{
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));


/** global vars **/
unsigned int *IDT;
struct idt_ptr pIDT;
const unsigned short numIDTEntries = 256;

/** function to set a gate in the idt **/
void IDT_SetGate (unsigned char num, unsigned int base, unsigned short sel, unsigned char flags)
{
	unsigned int *tmp;
	
	tmp = IDT;
	tmp += num * 2;
	
	*tmp = (base & 0xFFFF);
	*tmp |= (sel & 0xFFFF) << 16;
	tmp++;
	*tmp = (flags & 0xFF) << 8;
	*tmp |= (base & 0xFFFF0000);
}

/** function to set up the idt **/
void IDT_Setup (unsigned int ad)
{
	IDT = (unsigned int*)ad;
	memset32 (IDT,0,numIDTEntries*2);
	pIDT.limit = (8*numIDTEntries) - 1;
	pIDT.base = ad;
	IDT_load();
}



