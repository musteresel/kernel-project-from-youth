// Interrupt Servie Routines and IRQs

#ifndef __KERNEL__ISRIRQ___HEADER____
#define __KERNEL__ISRIRQ___HEADER____

/** kernel structures **/
struct irq_regs
{
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int int_no; //no error code
	unsigned int eip, cs, eflags, useresp, ss;
};
struct regs
{
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
};

extern void ISR_Setup (void);

extern void IRQ_Setup (void);
extern void IRQ_InstallHandler (int irq, void (*handler)(struct irq_regs *r));
extern void IRQ_UninstallHandler (int);

#endif //(__KERNEL__ISRIRQ___HEADER____)
