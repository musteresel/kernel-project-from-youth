// Kernel heap

#ifndef __KERNEL__KHEAP___HEADER____
#define __KERNEL__KHEAP___HEADER____

#include "types.h"
#include "ord_array.h"



extern const UINT KHEAP_start;
extern const UINT KHEAP_size;
extern const UINT KHEAP_indexsize;
extern const UINT KHEAP_MAGIC;


typedef struct
{
	UINT magic;
	UINT8 is_hole;
	UINT size;
} HEAP_header;

typedef struct
{
	UINT magic;
	HEAP_header *header;
} HEAP_footer;

typedef struct
{
	OrdArray index;
	UINT start_address;
	UINT end_address;
	UINT8 supervisor;
	UINT8 readonly;
} Heap;


extern INT8 create_heap (Heap *, UINT, UINT, UINT8, UINT8);
extern void *alloc(UINT, UINT8);
extern void free(void *); 

extern Heap KHeap;

#endif //(__KERNEL__KHEAP___HEADER____)

