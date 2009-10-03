/////////////////////////////////////////////////////////////////////////
/* isr.c
Definition an setup of Interrupt Service Routines

-----------------------------
This file is part of kernel.
By Daniel Oertwig
*/
/////////////////////////////////////////////////////////////////////////



#include "idt.h"
#include "isr_irq.h"
#include "types.h"
#include "debug-text.h"




/** extern interrupt functions **/
extern void _isr0();
extern void _isr1();
extern void _isr2();
extern void _isr3();
extern void _isr4();
extern void _isr5();
extern void _isr6();
extern void _isr7();
extern void _isr8();
extern void _isr9();
extern void _isr10();
extern void _isr11();
extern void _isr12();
extern void _isr13();
extern void _isr14();
extern void _isr15();
extern void _isr16();
extern void _isr17();
extern void _isr18();
extern void _isr19();
extern void _isr20();
extern void _isr21();
extern void _isr22();
extern void _isr23();
extern void _isr24();
extern void _isr25();
extern void _isr26();
extern void _isr27();
extern void _isr28();
extern void _isr29();
extern void _isr30();
extern void _isr31();





/** function to set up the interrupt handling **/
void ISR_Setup (void)
{
	IDT_SetGate(0, (unsigned)_isr0, 0x08, 0x8E);
	IDT_SetGate(1, (unsigned)_isr1, 0x08, 0x8E);
	IDT_SetGate(2, (unsigned)_isr2, 0x08, 0x8E);
	IDT_SetGate(3, (unsigned)_isr3, 0x08, 0x8E);
	IDT_SetGate(4, (unsigned)_isr4, 0x08, 0x8E);
	IDT_SetGate(5, (unsigned)_isr5, 0x08, 0x8E);
	IDT_SetGate(6, (unsigned)_isr6, 0x08, 0x8E);
	IDT_SetGate(7, (unsigned)_isr7, 0x08, 0x8E);
	IDT_SetGate(8, (unsigned)_isr8, 0x08, 0x8E);
	IDT_SetGate(9, (unsigned)_isr9, 0x08, 0x8E);
	IDT_SetGate(10, (unsigned)_isr10, 0x08, 0x8E);
	IDT_SetGate(11, (unsigned)_isr11, 0x08, 0x8E);
	IDT_SetGate(12, (unsigned)_isr12, 0x08, 0x8E);
	IDT_SetGate(13, (unsigned)_isr13, 0x08, 0x8E);
	IDT_SetGate(14, (unsigned)_isr14, 0x08, 0x8E);
	IDT_SetGate(15, (unsigned)_isr15, 0x08, 0x8E);
	IDT_SetGate(16, (unsigned)_isr16, 0x08, 0x8E);
	IDT_SetGate(17, (unsigned)_isr17, 0x08, 0x8E);
	IDT_SetGate(18, (unsigned)_isr18, 0x08, 0x8E);
	IDT_SetGate(19, (unsigned)_isr19, 0x08, 0x8E);
	IDT_SetGate(20, (unsigned)_isr20, 0x08, 0x8E);
	IDT_SetGate(21, (unsigned)_isr21, 0x08, 0x8E);
	IDT_SetGate(22, (unsigned)_isr22, 0x08, 0x8E);
	IDT_SetGate(23, (unsigned)_isr23, 0x08, 0x8E);
	IDT_SetGate(24, (unsigned)_isr24, 0x08, 0x8E);
	IDT_SetGate(25, (unsigned)_isr25, 0x08, 0x8E);
	IDT_SetGate(26, (unsigned)_isr26, 0x08, 0x8E);
	IDT_SetGate(27, (unsigned)_isr27, 0x08, 0x8E);
	IDT_SetGate(28, (unsigned)_isr28, 0x08, 0x8E);
	IDT_SetGate(29, (unsigned)_isr29, 0x08, 0x8E);
	IDT_SetGate(30, (unsigned)_isr30, 0x08, 0x8E);
	IDT_SetGate(31, (unsigned)_isr31, 0x08, 0x8E);
}




/** called when an interrupt occured **/
void ErrorHandler (struct regs *r) __attribute__ (( section(".klinked")));
void ErrorHandler (struct regs *r)
{
	INT8 numbuf[15];
	int_to_string(numbuf,'d',r->int_no);
	puts("\n");
	puts(numbuf);
	puts("ISR happen at EIP = ");
	int_to_string(numbuf,'x',r->eip);
	puts(numbuf);
	int_to_string(numbuf,'x',r->esp);
	puts("\nESP = ");puts(numbuf);
	asm volatile ("hlt");
	//TODO call routine to pass a message to the kernel
}




