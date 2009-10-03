// Paging

#ifndef __KERNEL__PAGING___HEADER____
#define __KERNEL__PAGING___HEADER____

#include "types.h"


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


extern const UINT TASK_INFO;
extern const UINT REMAPPED_PAGEDIR;


extern void Paging_Init ();
extern INT8 pg_setEntry(pg_PageTab *, UINT, pg_Page);
extern pg_Page pg_getEntry(pg_PageTab *, UINT);

extern UINT16 ResolveTabNumfromAddress (UINT);
extern UINT ResolveAddressfromTabNum (UINT16);
extern UINT16 ResolvePageNumfromAddress (UINT);
extern void EnablePaging (UINT);
extern UINT DisablePaging ();

extern pg_PageTab *pgoff_CreateRawDir ();
extern void pgoff_IdentityMapMemory(pg_PageTab *, UINT, UINT);
extern void pgoff_MapMemory(pg_PageTab *, UINT, UINT, UINT);


extern void copy_page_physical(UINT, UINT); 


extern void MapVirtPhys(UINT, UINT, UINT16);

#endif //(__KERNEL__PAGING___HEADER____)

