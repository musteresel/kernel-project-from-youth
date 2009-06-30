// Interrupt Servie Routines and IRQs

#ifndef __KERNEL__ISRIRQ___HEADER____
#define __KERNEL__ISRIRQ___HEADER____


#include "types.h"

/** kernel structures **/
struct irq_regs
{
	UINT gs, fs, es, ds;
	UINT edi, esi, ebp, esp, ebx, edx, ecx, eax;
	UINT int_no; //no error code
	UINT eip, cs, eflags, useresp, ss;
};
struct regs
{
	UINT gs, fs, es, ds;
	UINT edi, esi, ebp, esp, ebx, edx, ecx, eax;
	UINT int_no, err_code;
	UINT eip, cs, eflags, useresp, ss;
};

extern void ISR_Setup (void);

extern void IRQ_Setup (void);
extern void IRQ_InstallHandler (INT , void (*handler)(struct irq_regs *r));
extern void IRQ_UninstallHandler (INT);
extern void ISR_InstallHandler (INT , void (*handler)(struct regs *r));
extern void ISR_UninstallHandler (INT);

#endif //(__KERNEL__ISRIRQ___HEADER____)
