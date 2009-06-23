// Phys. memory management - Init

#ifndef __KERNEL__PHYS_MEM_INIT___HEADER____
#define __KERNEL__PHYS_MEM_INIT___HEADER____


#include "types.h"

extern void pmm_Setup (UINT, UINT);
extern void pmm_MarkUsedSpace_mmap (UINT*, UINT);


#endif //(__KERNEL__PHYS_MEM_INIT___HEADER____)


