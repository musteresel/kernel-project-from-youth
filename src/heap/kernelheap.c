//the kernel heap
#include "types.h"
#include "kernelheap.h"
#include "ord_array.h"


const UINT KHEAP_start = 0xB0000000;
const UINT KHEAP_size = 0x100000;
const UINT KHEAP_indexsize = 0x20000;
const UINT KHEAP_MAGIC = 0x1BADB01B;
Heap KHeap;


static INT find_smallest_hole(UINT size, UINT8 page_align, Heap *heap)
{
	UINT iterator = 0;
	while (iterator < heap->index.size)
	{
		HEAP_header *header = (HEAP_header *)OA_lookup(iterator, &heap->index);
		if (page_align > 0)
		{
			UINT location = (UINT)header;
			INT offset = 0;
			if (((location+sizeof(HEAP_header)) & 0xFFFFF000) != 0)
			{
				offset = 0x1000 - ((location+sizeof(HEAP_header)) & 0xFFF);
			}
			INT hole_size = (INT)header->size - offset;
			if (hole_size >= (INT)size)
			{
				break;
			}
		}
		else if (header->size >= size)
		{
			break;
		}
		iterator++;
	}
	if (iterator == heap->index.size)
	{
		return -1;
	}
	else
	{
		return iterator;
	}
}


static INT8 header_t_less_than(void*a, void *b)
{
	return (((HEAP_header*)a)->size < ((HEAP_header*)b)->size)?1:0;
}


INT8 create_heap(Heap * heap, UINT start, UINT end_addr, UINT8 supervisor, UINT8 readonly)
{
	if ((start&0xFFF) != 0)
	{
		return 0;
	}
	if ((end_addr&0xFFF) != 0)
	{
		return 0;
	}
	heap->index = OA_place( (void*)start, KHEAP_indexsize, &header_t_less_than);
	start += sizeof(OA_VoidPointer)*KHEAP_indexsize;
	if ((start&0xFFFFF000) != 0)
	{
		start &= 0xFFFFF000;
		start += 0x1000;
	}
	heap->start_address = start;
	heap->end_address = end_addr;
	heap->supervisor = supervisor;
	heap->readonly = readonly;

	HEAP_header *hole = (HEAP_header *)start;
	hole->size = end_addr-start;
	hole->magic = KHEAP_MAGIC;
	hole->is_hole = 1;
	OA_insert((void*)hole, &(heap->index));
	return 1;
}




void *kalloc(UINT size, UINT8 page_align)
{
	return alloc(size,page_align,&KHeap);
}

void *alloc(UINT size, UINT8 page_align, Heap *heap)
{
	UINT new_size = size + sizeof(HEAP_header) + sizeof(HEAP_footer);
	INT iterator = find_smallest_hole (new_size, page_align, heap);
	
	if (iterator == -1)
	{
		return 0;
	}
	HEAP_header *orig_hole_header = (HEAP_header *)OA_lookup(iterator, &heap->index);
	UINT orig_hole_pos = (UINT)orig_hole_header;
	UINT orig_hole_size = orig_hole_header->size;
	if (orig_hole_size-new_size < sizeof(HEAP_header)+sizeof(HEAP_footer))
	{
		size += orig_hole_size-new_size;
		new_size = orig_hole_size;
	}
	if (page_align && orig_hole_pos&0xFFFFF000)
	{
		UINT new_location = orig_hole_pos + 0x1000 - (orig_hole_pos&0xFFF) - sizeof(HEAP_header);
		HEAP_header *hole_header = (HEAP_header *)orig_hole_pos;
		hole_header->size = 0x1000 - (orig_hole_pos&0xFFF) - sizeof(HEAP_header);
		hole_header->magic = KHEAP_MAGIC;
		hole_header->is_hole = 1;
		HEAP_footer *hole_footer = (HEAP_footer *) ( (UINT)new_location - sizeof(HEAP_footer) );
		hole_footer->magic = KHEAP_MAGIC;
		hole_footer->header = hole_header;
		orig_hole_pos = new_location;
		orig_hole_size = orig_hole_size - hole_header->size;
	}
	else
	{
		OA_remove (iterator, &heap->index);
	}
	HEAP_header *block_header = (HEAP_header *)orig_hole_pos;
	block_header->magic = KHEAP_MAGIC;
	block_header->is_hole = 0;
	block_header->size = new_size;
	HEAP_footer *block_footer = (HEAP_footer *) (orig_hole_pos + sizeof(HEAP_header) + size);
	block_footer->magic = KHEAP_MAGIC;
	block_footer->header = block_header;
	if (orig_hole_size - new_size > 0)
	{
		HEAP_header *hole_header = (HEAP_header *) (orig_hole_pos + sizeof(HEAP_header) + size + sizeof(HEAP_footer));
		hole_header->magic = KHEAP_MAGIC;
		hole_header->is_hole = 1;
		hole_header->size = orig_hole_size - new_size;
		HEAP_footer *hole_footer = (HEAP_footer *) ((UINT)hole_header + orig_hole_size - new_size - sizeof(HEAP_footer));
		if ((UINT)hole_footer < heap->end_address)
		{
			hole_footer->magic = KHEAP_MAGIC;
			hole_footer->header = hole_header;
		}
		OA_insert ((void*)hole_header, &heap->index);
	}
	return (void *) ( (UINT)block_header+sizeof(HEAP_header) );
}


void kfree(void *p)
{
	free(p,&KHeap);
}

void free(void *p, Heap *heap)
{
	if (p == 0) {
		return;
	}
	HEAP_header *header = (HEAP_header*)( (UINT)p - sizeof(HEAP_header) );
	HEAP_footer *footer = (HEAP_footer*)( (UINT)header + header->size - sizeof(HEAP_footer) );
	if ( (header->magic != KHEAP_MAGIC) || (footer->magic != KHEAP_MAGIC)) {
		return;
	}
	header->is_hole = 1;
	INT8 do_add = 1;
	HEAP_footer *test_footer = (HEAP_footer*) ( (UINT)header - sizeof(HEAP_footer) );
	if (test_footer->magic == KHEAP_MAGIC && test_footer->header->is_hole == 1) {
		UINT cache_size = header->size;
		header = test_footer->header;
		footer->header = header;
		header->size += cache_size;
		do_add = 0;
	}
	HEAP_header *test_header = (HEAP_header*)( (UINT)footer + sizeof(HEAP_footer) );
	if (test_header->magic == KHEAP_MAGIC && test_header->is_hole == 1) {
		header->size += test_header->size;
		test_footer = (HEAP_footer*)( (UINT)test_header + test_header->size - sizeof(HEAP_footer) );
		footer = test_footer;
		UINT iterator = 0;
		while ( (iterator < heap->index.size) && (OA_lookup(iterator,&(heap->index)) != (void*)test_header) ) {
			iterator++;
		}
		if (iterator < heap->index.size) {
			OA_remove(iterator, &(heap->index));
		}
	}
	if (do_add == 1)
	OA_insert((void*) header, &(heap->index));
}
	





