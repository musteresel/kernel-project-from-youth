

#include "multitasking.h"
#include "paging.h"
#include "pmm.h"
#include "kernelheap.h"
#include "debug-text.h"
#include "gdt.h"


Task *current_task;



void IDIOT()
{
	while(1) {
		asm volatile("cli");
		char numbuf[15];
		int_to_string(numbuf,'d',current_task->id);
		puts(numbuf);
		puts(" is running...\n");
		asm volatile("sti");
	}
}


/*void CreateCTask(UINT eip, UINT cr3)*/
/*{*/
/*	UINT pointer = ResolveAddressfromFrame(pmm_alloc_frames(3));*/
/*	if (!pointer) {*/
/*		puts("OUT OF SPACE\n");*/
/*		asm volatile ("hlt");*/
/*	}*/
/*	MapVirtPhys(0xFF400000,pointer,1);*/
/*	MapVirtPhys(0xFF400000+0x1000,pointer+0x1000,1);*/
/*	MapVirtPhys(0xFF400000+0x2000,pointer+0x2000,1);*/
/*	CTask *new = SetupCTask(0xFF400000,eip,0,cr3);*/
/*}*/


Task *CreateTask(UINT eip, UINT cr3)
{
	Task *new = (Task*)kalloc(sizeof(Task),0);
	UINT *kstack = (UINT*)kalloc(0x1000,1);
	if (!kstack) {
		puts("Something is going wrong...\n");
		asm volatile ("hlt");
	}
	kstack+= 0xFF0;
	new->kernelstack = (UINT)kstack;
	*(--kstack) = 0;
	*(--kstack) = 0x10;//ss
	*(--kstack) = 0xBABA;//esp, will be set in a second
	UINT stackpointer;
	stackpointer = (UINT)kstack;
	*(--kstack) = 0x202; //eflags
	*(--kstack) = 0x08; //cs
	*(--kstack) = eip;
	
	*(--kstack) = 0; // int num 
	
	*(--kstack) = 0;//EAX
	*(--kstack) = 0;//ECX
	*(--kstack) = 0;//EDX
	*(--kstack) = 0;//EBX
	*(--kstack) = 0;//dummy ESP ?
	*(--kstack) = new->kernelstack;//EBP (??)
	*(--kstack) = 0;//ESI
	*(--kstack) = 0;//EDI
	
	
	*(--kstack) = 0x10;// DS
	*(--kstack) = 0x10;// ES
	*(--kstack) = 0x10;// FS
	*(--kstack) = 0x10;// GS
	new->esp = (UINT)kstack;
	*(UINT*)(stackpointer) = (UINT)kstack; // (???)
	new->cr3 = cr3;
	return new;
}

void Multitasking_Init()
{
	current_task = (Task*)kalloc(sizeof(Task),0);
	asm volatile ("mov %%cr3, %0": "=r"(current_task->cr3));
	current_task->next = current_task;
	Task *new = CreateTask((UINT)&IDIOT,current_task->cr3);
	new->next = current_task;
	current_task->next = new;
	current_task->id = 0;
	new->id = 1;
	asm volatile("sti");
	while(1) {
		asm volatile("cli");
		char numbuf[15];
		int_to_string(numbuf,'d',current_task->id);
		puts(numbuf);
		puts(" is running...\n");
		asm volatile("sti");
	}
	return;
}



UINT SwitchTask(struct irq_regs *r)
{
	current_task->esp = (UINT)r;
	current_task = current_task->next;
	r = (struct irq_regs*)current_task->esp;
	TSS.esp = current_task->esp;
	TSS.esp0 = current_task->kernelstack;
	EnablePaging(current_task->cr3);
	return (UINT)r;
}
