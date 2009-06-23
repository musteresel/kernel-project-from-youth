// Simplest text output, used for basic messages to the user


#ifndef __KERNEL__DEBUGTEXT___HEADER____
#define __KERNEL__DEBUGTEXT___HEADER____

#include "types.h"

extern void putc (UINT8);
extern void puts (INT8*);
extern void set_color (UINT8, UINT8);
extern void int_to_string (INT8*, UINT8, INT);



#endif //(__KERNEL__DEBUGTEXT___HEADER____)

