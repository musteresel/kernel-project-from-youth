// Kernel heap

#ifndef __KERNEL__KHEAP___HEADER____
#define __KERNEL__KHEAP___HEADER____

#include "types.h"
#include "ord_array.h"



extern const UINT KHEAP_start;
extern const UINT KHEAP_initsize;
extern const UINT KHEAP_indexsize;
extern const UINT KHEAP_MAGIC;
extern const UINT KHEAP_minsize;


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
	UINT max_address;
	UINT8 supervisor;
	UINT8 readonly;
} Heap;


Heap *create_heap (UINT, UINT, UINT, UINT, UINT8, UINT8);
void *alloc(UINT, UINT8, Heap *);
void free(void *, Heap *); 



#endif //(__KERNEL__KHEAP___HEADER____)

