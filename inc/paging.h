// Paging

#ifndef __KERNEL__PAGING___HEADER____
#define __KERNEL__PAGING___HEADER____

#include "types.h"

extern const UINT8 pg_PRESENT;
extern const UINT8 pg_RW;
extern const UINT8 pg_USER;
extern const UINT8 pg_ACCESSED;
extern const UINT8 pg_DIRTY;

struct ____paging___page
{
	UINT present : 1;
	UINT rw : 1;
	UINT user : 1;
	UINT accessed : 1;
	UINT dirty : 1;
	UINT unused : 7;
	UINT frame : 20;
} __attribute__((packed));
typedef struct ____paging___page pg_Page;



typedef struct
{
	pg_Page entries[1024];
}pg_PageTab;





extern void Paging_Init ();
extern INT8 pg_setEntry(pg_PageTab *, UINT, pg_Page);
extern pg_Page pg_getEntry(pg_PageTab *, UINT);

extern UINT16 ResolveTabNumfromAddress (UINT);
extern UINT ResolveAddressfromTabNum (UINT16);
extern UINT16 ResolvePageNumfromAddress (UINT);

#endif //(__KERNEL__PAGING___HEADER____)

