/////////////////////////////////////////////////////////////////////////
/* pmm_init.c
Setup of the physical memory management

-----------------------------
This file is part of kernel.
By Daniel Oertwig
*/
/////////////////////////////////////////////////////////////////////////


#include "pmm.h"
#include "bio.h"





// ############################################################
extern unsigned int __kernel_space_start;
extern unsigned int __kernel_space_end;



// ############################################################
/** memory management **/
unsigned int pmm_Start;
const unsigned int pmm_ret_MEMFULL = 0;
unsigned int pmm_InfoStart;
unsigned int pmm_InfoEnd;

/** supermap **/
unsigned int pmm_SMapStart;
unsigned int pmm_SMapSize;

/** maps **/
unsigned int pmm_MapsStart;
unsigned int pmm_CountMaps;
unsigned int pmm_MapSize;
const unsigned int pmm_SIZE_OF_MAPS_MB = 8;
const unsigned int pmm_FRAMES_PER_MAP_DIVISOR = 11; //(NOTE depends on value pmm_SIZE_OF_MAPS)
const unsigned int pmm_FRAMES_PER_MAP = 8 * 256; //(NOTE depends on value pmm_SIZE_OF_MAPS)





// ############################################################
/** setting up the phys. memory management **/
void pmm_Setup (unsigned int base, unsigned int size)
{
	unsigned int *wr;
	unsigned int tmp1;
	unsigned int tmp2;
	
	pmm_Start = base;
	pmm_InfoStart = __kernel_space_end + 16;
	pmm_CountMaps = size / (pmm_SIZE_OF_MAPS_MB << 20);
	pmm_MapSize = pmm_FRAMES_PER_MAP >> 5;
	pmm_SMapSize = (pmm_CountMaps + 31) >> 5;
	
	wr = (unsigned int*) pmm_InfoStart;
	*wr = pmm_CountMaps;
	wr++;
	*wr = size >> 10;
	wr++;
	*wr = pmm_SMapSize;
	wr++;
	
	pmm_SMapStart = (unsigned int) wr;
	wr += pmm_SMapSize;
	wr++;
	pmm_MapsStart = (unsigned int) wr;
	wr += pmm_MapSize * pmm_CountMaps;
	wr++;
	pmm_InfoEnd = (unsigned int) wr;
	
	memset32 ((unsigned int*)pmm_SMapStart,0,pmm_InfoEnd-pmm_SMapStart);
	
	tmp1 = ResolveFramefromAddress ( __kernel_space_start );
	tmp2 = ResolveFramefromAddress ( pmm_InfoEnd );
	for (; tmp1 <= tmp2; tmp1++) {
		use_frame (tmp1);
// 		NOTE the map ist not checked! TODO (!!!)
	}
	
	__kernel_space_end = pmm_InfoEnd;
	return;
}





// ############################################################
