// Paging

#ifndef __KERNEL__PAGING___HEADER____
#define __KERNEL__PAGING___HEADER____

#include "types.h"

extern const UINT8 pg_PRESENT;
extern const UINT8 pg_RW;
extern const UINT8 pg_USER;
extern const UINT8 pg_ACCESSED;
extern const UINT8 pg_DIRTY;

typedef struct
{
	UINT present : 1;
	UINT rw : 1;
	UINT user : 1;
	UINT accessed : 1;
	UINT dirty : 1;
	UINT unused : 7;
	UINT frame : 20;
} pg_Page;


typedef struct
{
	pg_Page entries[1024];
}pg_PageTab;






extern void Paging_Init ();


#endif //(__KERNEL__PAGING___HEADER____)

