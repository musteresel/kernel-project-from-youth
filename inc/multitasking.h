// Multitasking

#ifndef __KERNEL__MULTITASKING___HEADER____
#define __KERNEL__MULTITASKING___HEADER____

#include "types.h"
#include "isr_irq.h"

typedef struct ___task {
	INT id;
	UINT esp;
	UINT cr3;
	UINT kernelstack;
	UINT queue;
	struct ___task *next; /*NOTE: Only usable by scheduler */
} Task;

typedef struct ___ctask {
	Task *me;
	Task *next;
}CTask;

typedef struct __queue {
	Task *start;
	Task *current;
	INT size;
}TaskQueue;



extern void Multitasking_Init();
extern UINT SwitchTask(struct irq_regs *);

#endif

