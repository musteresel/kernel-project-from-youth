// Basic input output operations

#ifndef __KERNEL__BIO___HEADER____
#define __KERNEL__BIO___HEADER____

#include "types.h"

extern void memset32 (UINT*, UINT, UINT);
extern void memset8 (UINT*, UINT8, UINT);
extern void memcpy32 (UINT*, UINT*, UINT);
extern void memcpy8 (UINT*, UINT*, UINT);
extern void outportb (UINT16, UINT8);
extern UINT8 inportb (UINT16);



#endif //(__KERNEL__BIO___HEADER____)
