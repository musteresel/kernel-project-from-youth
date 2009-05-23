/////////////////////////////////////////////////////////////////////////
/* irq.c
Definitions an functions concerning IRQs

-----------------------------
This file is part of kernel.
By Daniel Oertwig
*/
/////////////////////////////////////////////////////////////////////////



/** extern functions **/
#include "idt.h"
#include "bio.h"
#include "isr_irq.h"




/** extern irq functions **/
extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();



/** dynamic irq routines **/
void *IRQ_Routines[16] =
{
 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0
};

/** functions to install and uninstall handlers **/
void IRQ_InstallHandler (int irq, void (*handler)(struct irq_regs *r))
{
	IRQ_Routines[irq] = handler;
}
void IRQ_UninstallHandler (int irq)
{
	IRQ_Routines[irq] = 0;
}


/** function to remap the irqs **/
void IRQ_Remap (void)
{
	outportb(0x20, 0x11);// Remap ->  PIC 1
	outportb(0xA0, 0x11);// ... -> PIC 2
	outportb(0x21, 0x20);// Startad. Master PIC ( 0x20 = 32)
	outportb(0xA1, 0x28);// Startad. Slave PIC
	outportb(0x21, 0x04);
	outportb(0xA1, 0x02);
	outportb(0x21, 0x01);
	outportb(0xA1, 0x01);
	outportb(0x21, 0x0);
	outportb(0xA1, 0x0);
}


/** function to set up irq handling **/
void IRQ_Setup (void)
{
	IRQ_Remap();
	IDT_SetGate(32, (unsigned)_irq0, 0x08, 0x8E);
	IDT_SetGate(33, (unsigned)_irq1, 0x08, 0x8E);
	IDT_SetGate(34, (unsigned)_irq2, 0x08, 0x8E);
	IDT_SetGate(35, (unsigned)_irq3, 0x08, 0x8E);
	IDT_SetGate(36, (unsigned)_irq4, 0x08, 0x8E);
	IDT_SetGate(37, (unsigned)_irq5, 0x08, 0x8E);
	IDT_SetGate(38, (unsigned)_irq6, 0x08, 0x8E);
	IDT_SetGate(39, (unsigned)_irq7, 0x08, 0x8E);
	IDT_SetGate(40, (unsigned)_irq8, 0x08, 0x8E);
	IDT_SetGate(41, (unsigned)_irq9, 0x08, 0x8E);
	IDT_SetGate(42, (unsigned)_irq10, 0x08, 0x8E);
	IDT_SetGate(43, (unsigned)_irq11, 0x08, 0x8E);
	IDT_SetGate(44, (unsigned)_irq12, 0x08, 0x8E);
	IDT_SetGate(45, (unsigned)_irq13, 0x08, 0x8E);
	IDT_SetGate(46, (unsigned)_irq14, 0x08, 0x8E);
	IDT_SetGate(47, (unsigned)_irq15, 0x08, 0x8E);
}


/** function called when an irq occured, hands over to handler **/
void IRQHandler (struct irq_regs *r)
{
	void (*handler)(struct irq_regs *r);
	handler = IRQ_Routines[r->int_no - 32];
	if (handler)
	{
		handler(r);
	}
}

