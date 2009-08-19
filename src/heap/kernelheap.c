//the kernel heap
#include "types.h"
#include "kernelheap.h"
#include "ord_array.h"


const UINT KHEAP_start = 0xB0000000;
const UINT KHEAP_initsize = 0x100000;
const UINT KHEAP_indexsize = 0x20000;
const UINT KHEAP_MAGIC = 0x1BADB01B;
const UINT KHEAP_minsize = 0x70000;


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

Heap *create_heap(UINT heap_space, UINT start, UINT end_addr, UINT max, UINT8 supervisor, UINT8 readonly)
{
	Heap *heap = (Heap*)heap_space;
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
	heap->max_address = max;
	heap->supervisor = supervisor;
	heap->readonly = readonly;

	HEAP_header *hole = (HEAP_header *)start;
	hole->size = end_addr-start;
	hole->magic = KHEAP_MAGIC;
	hole->is_hole = 1;
	OA_insert((void*)hole, &heap->index);

	return heap;
}


static void expand(UINT new_size, Heap *heap)
{
	if(new_size < (heap->end_address-heap->start_address))
	{
		return;
	}
	if ((new_size&0xFFFFF000) != 0)
	{
		new_size &= 0xFFFFF000;
		new_size += 0x1000;
	}
	if (heap->start_address+new_size > heap->max_address)
	{
		return;
	}
	/*UINT old_size = heap->end_address-heap->start_address;
	UINT i = old_size;
	while (i < new_size)
	{
		alloc_frame( get_page(heap->start_address+i, 1, kernel_directory),
					 (heap->supervisor)?1:0, (heap->readonly)?0:1);
		i += 0x1000; //TODO!!!
	}*/
	heap->end_address = heap->start_address+new_size;
}



static UINT contract(UINT new_size, Heap *heap)
{
	if (new_size > heap->end_address-heap->start_address)
	{
		return heap->end_address-heap->start_address;
	}
	if (new_size&0x1000)
	{
		new_size &= 0x1000;
		new_size += 0x1000;
	}
	if (new_size < KHEAP_minsize)
	{
		new_size = KHEAP_minsize;
	}
	/*UINT old_size = heap->end_address-heap->start_address;
	UINT i = old_size - 0x1000;
	while (new_size < i)
	{
		free_frame(get_page(heap->start_address+i, 0, kernel_directory));
		i -= 0x1000; //TODO!!!!
	}*/
	heap->end_address = heap->start_address + new_size;
	return new_size;
}


void *alloc(UINT size, UINT8 page_align, Heap *heap)
{
	UINT new_size = size + sizeof(HEAP_header) + sizeof(HEAP_footer);
	INT iterator = find_smallest_hole (new_size, page_align, heap);
	
	if (iterator == -1)
	{
		UINT old_length = heap->end_address - heap->start_address;
		UINT old_end_address = heap->end_address;
		expand (old_length+new_size, heap);
		UINT new_length = heap->end_address-heap->start_address;
		iterator = 0;
		UINT idx = -1;
		UINT value = 0x0;
		while (iterator < heap->index.size)
		{
			UINT tmp = (UINT)OA_lookup(iterator, &heap->index);
			if (tmp > value)
			{
				value = tmp;
				idx = iterator;
			}
			iterator++;
		}
		if (idx == -1)
		{
			HEAP_header *header = (HEAP_header *)old_end_address;
			header->magic = KHEAP_MAGIC;
			header->size = new_length - old_length;
			header->is_hole = 1;
			HEAP_footer *footer = (HEAP_footer *) (old_end_address + header->size - sizeof(HEAP_footer));
			footer->magic = KHEAP_MAGIC;
			footer->header = header;
			OA_insert ((void*)header, &heap->index);
		}
		else
		{
			HEAP_header *header = OA_lookup(idx, &heap->index);
			header->size += new_length - old_length;
			HEAP_footer *footer = (HEAP_footer *) ( (UINT)header + header->size - sizeof(HEAP_footer) );
			footer->header = header;
			footer->magic = KHEAP_MAGIC;
		}
		return alloc (size, page_align, heap);
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



