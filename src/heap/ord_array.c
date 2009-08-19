// ordered arrays, mainly used by heap

#include "types.h"
#include "bio.h"
#include "ord_array.h"





INT8 standard_lessthan_predicate(OA_VoidPointer a, OA_VoidPointer b)
{
	return (a<b)?1:0;
}

/*OrdArray create_ordered_array(UINT max_size, lessthan_predicate_t less_than)
{
	OrdArray OA_ret;
	OA_ret.array = (void*)kmalloc(max_size*sizeof(OA_VoidPointer));
	memset32((UINT*)OA_ret.array, 0, max_size);
	OA_ret.size = 0;
	OA_ret.max_size = max_size;
	OA_ret.less_than = less_than;
	return OA_ret;
}*/

OrdArray OA_place (void *addr, UINT max_size, lessthan_predicate_t less_than)
{
	OrdArray OA_ret;
	OA_ret.array = (OA_VoidPointer*)addr;
	memset32((UINT*)OA_ret.array, 0, max_size);
	OA_ret.size = 0;
	OA_ret.max_size = max_size;
	OA_ret.less_than = less_than;
	return OA_ret;
}


void OA_insert (OA_VoidPointer item, OrdArray *array)
{
	UINT iterator = 0;
	while (iterator < array->size && array->less_than(array->array[iterator], item))
	{
		iterator++;
	}
	if (iterator == array->size)
	{
		array->array[array->size++] = item;
	}
	else
	{
		OA_VoidPointer tmp = array->array[iterator];
		array->array[iterator] = item;
		while (iterator < array->size)
		{
			iterator++;
			OA_VoidPointer tmp2 = array->array[iterator];
			array->array[iterator] = tmp;
			tmp = tmp2;
		}
		array->size++;
	}
}

OA_VoidPointer OA_lookup (UINT i, OrdArray *array)
{
	if (i >= array->size)
	{
		return 0;
	}
	return array->array[i];
}

void OA_remove (UINT i, OrdArray *array)
{
	while (i < array->size)
	{
		array->array[i] = array->array[i+1];
		i++;
	}
	array->size--;
}



