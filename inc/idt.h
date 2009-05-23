// Interrupt Descriptor Table

#ifndef __KERNEL__IDT___HEADER____
#define __KERNEL__IDT___HEADER____



extern void IDT_Setup (unsigned int);
extern void IDT_SetGate (unsigned char, unsigned int, unsigned short, unsigned char);


#endif //(__KERNEL__IDT___HEADER____)
