

#include "multitasking.h"
#include "paging.h"
#include "pmm.h"
#include "kernelheap.h"
#include "debug-text.h"
#include "gdt.h"

/** Global variables **/
Task *currentTask;
const UINT16 mt_KERNELSTACKSIZE = 0x1000;



/** Functions used by the taskmanager **/
Task *CreateTask(UINT eip, UINT cr3)
{
	Task *newTask = (Task*)kalloc(sizeof(Task),0);
	UINT *kstack = (UINT*)kalloc(mt_KERNELSTACKSIZE,1);
	CheckForSure((kstack),"Could not allocate space for a kernel stack!")
	kstack+= mt_KERNELSTACKSIZE-4;
	newTask->kernelstack = (UINT)kstack;
	*(--kstack) = 0;   // Just for safety
	*(--kstack) = 0x10;//SS
	*(--kstack) = 0x0; //ESP, will be set in a second
	UINT stackpointer;stackpointer = (UINT)kstack;
	*(--kstack) = 0x202;//EFLAGS
	*(--kstack) = 0x08; //CS
	*(--kstack) = eip;  //EIP
	*(--kstack) = 0;//INTERRUPTNUMBER
	*(--kstack) = 0;//EAX
	*(--kstack) = 0;//ECX
	*(--kstack) = 0;//EDX
	*(--kstack) = 0;//EBX
	*(--kstack) = 0;//ESP (not used)
	*(--kstack) = newTask->kernelstack;//EBP TODO neccessary to set?
	*(--kstack) = 0;//ESI
	*(--kstack) = 0;//EDI
	*(--kstack) = 0x10;// DS
	*(--kstack) = 0x10;// ES
	*(--kstack) = 0x10;// FS
	*(--kstack) = 0x10;// GS
	newTask->esp = (UINT)kstack;//ESP (real one that is used)
	*(UINT*)(stackpointer) = (UINT)kstack; //ESP from above TODO neccessary to set?
	newTask->cr3 = cr3;
	return newTask;
}


/** Setting up multitasking **/
void InitMultitasking()
{
	currentTask = (Task*)kalloc(sizeof(Task),0);
	asm volatile ("mov %%cr3, %0": "=r"(currentTask->cr3));
	currentTask->next = currentTask;
	currentTask->id = 0;
	return;
}


/** Code to switch tasks **/
UINT SwitchTask(struct irq_regs *r) __attribute__ (( section(".klinked")));
UINT SwitchTask(struct irq_regs *r)
{
	currentTask->esp = (UINT)r;
	currentTask = currentTask->next;
	r = (struct irq_regs*)currentTask->esp;
	TSS.esp = currentTask->esp;
	TSS.esp0 = currentTask->kernelstack;
	EnablePaging(currentTask->cr3);
	return (UINT)r;
}
