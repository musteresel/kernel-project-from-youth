

#include "multitasking.h"
#include "paging.h"
#include "debug-text.h"
#include "gdt.h"


const UINT MT_KERNELSTACK_SIZE = 2048;
CTask *current_task;


CTask *CreateTask(UINT space, UINT eip, UINT esp, UINT cr3)
{
	CTask *t = (CTask*)space;
	t->me = (Task*)space + sizeof(CTask);
	t->next = (Task*)((UINT)(t->me)+sizeof(Task));
	t->me->esp = esp;
	t->me->cr3 = cr3;
	UINT *kstack = (UINT*)(space + 0x2FFF);
	*(--kstack) = 0x202; //eflags
	*(--kstack) = 0x08; //cs
	*(--kstack) = eip;
	
	*(--kstack) = 0; // int num
	
	*(--kstack) = 0;// registers
	*(--kstack) = 0;
	*(--kstack) = 0;
	*(--kstack) = 0;
	*(--kstack) = 0;
	*(--kstack) = 0;
	*(--kstack) = 0;
	
	*(--kstack) = 0x10;// segment regs
	*(--kstack) = 0x10;
	*(--kstack) = 0x10;
	*(--kstack) = 0x10;
    
	return t;
}


void Multitasking_Init()
{
	current_task = CreateTask(TASK_INFO,0,0,0);
	current_task->next = current_task->me;
	asm volatile ("mov %%cr3, %0": "=r"(current_task->me->cr3));
	current_task->me->id = -1;
	return;
}



UINT SwitchTask(struct irq_regs *r)
{
	current_task = (CTask *)TASK_INFO;
	current_task->me->esp = (UINT)r;
	r = (struct irq_regs*)current_task->next->esp;
	TSS.esp = current_task->next->esp;
	TSS.esp0 = current_task->next->kernelstack + MT_KERNELSTACK_SIZE;
	EnablePaging(current_task->next->cr3);
	return (UINT)r;
}
