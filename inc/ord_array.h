// ordered arrays

#ifndef __KERNEL__ORDARRAY___HEADER____
#define __KERNEL__ORDARRAY___HEADER____

#include "types.h"


typedef void* OA_VoidPointer;
typedef INT8 (*lessthan_predicate_t)(OA_VoidPointer,OA_VoidPointer);


typedef struct
{
	OA_VoidPointer *array;
	UINT size;
	UINT max_size;
	lessthan_predicate_t less_than;
} OrdArray;


extern INT8 standard_lessthan_predicate(OA_VoidPointer a, OA_VoidPointer b);


//extern OrdArray create_ordered_array(UINT max_size, lessthan_predicate_t less_than);
extern OrdArray OA_place (void *, UINT, lessthan_predicate_t);
extern void OA_insert (OA_VoidPointer, OrdArray *);
extern OA_VoidPointer OA_lookup (UINT, OrdArray *);
extern void OA_remove (UINT, OrdArray *);


#endif //(__KERNEL__ORDARRAY___HEADER____)

