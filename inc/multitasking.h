// Multitasking

#ifndef __KERNEL__MULTITASKING___HEADER____
#define __KERNEL__MULTITASKING___HEADER____

#include "types.h"
#include "isr_irq.h"

extern void Multitasking_Init();
extern UINT SwitchTask(struct irq_regs *);

#endif

