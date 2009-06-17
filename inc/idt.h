// Interrupt Descriptor Table

#ifndef __KERNEL__IDT___HEADER____
#define __KERNEL__IDT___HEADER____

#include "types.h"

extern void IDT_Setup (UINT);
extern void IDT_SetGate (UINT8, UINT, UINT, UINT8);


#endif //(__KERNEL__IDT___HEADER____)
