/////////////////////////////////////////////////////////////////////////
/* pmm_functions.c
Functions to controll the physical memory management

-----------------------------
This file is part of kernel.
By Daniel Oertwig
*/
/////////////////////////////////////////////////////////////////////////

#include "types.h"


extern UINT pmm_MapsStart;//start of maps
extern UINT pmm_MapSize;//size of one map
extern UINT pmm_FRAMES_PER_MAP_DIVISOR;
extern UINT pmm_FRAMES_PER_MAP;
extern UINT pmm_SMapStart;//start of supermap
extern UINT pmm_SMapSize;//Size of supermap
extern UINT pmm_ret_MEMFULL;
extern UINT pmm_Start;//Starting of managed memory
extern UINT pmm_LastFrame;


#define MAKRO_SET_BIT(x) (( 1 << ( (x) )))

/***********************************************************************/
/***********************************************************************/
UINT ResolveFramefromAddress (UINT ad)
{
    UINT frame;
    frame = (ad - pmm_Start);
    frame = frame >> 12;
    return frame;
}
UINT ResolveAddressfromFrame (UINT fr)
{
    UINT ad;
    ad = fr << 12;
    ad = ad + pmm_Start;
    return ad;
}
/***********************************************************************/
/***********************************************************************/
/**
* @brief Function to free a phys. frame in memory.
*
* @param frame The number of the frame to be freed.
*/
void free_frame (UINT frame)
{
    UINT map;
    UINT *tmp;
    map = frame >> pmm_FRAMES_PER_MAP_DIVISOR;
    frame = frame - (map * pmm_FRAMES_PER_MAP);
    tmp = (UINT*) (pmm_MapsStart+ (pmm_MapSize*map));
    tmp += (frame >> 5);
    *tmp &= ~MAKRO_SET_BIT(frame&(31));
}

/**
* @brief Function to use a phys. frame in memory.
*
* @param frame The number of the frame to be used.
*/
void use_frame (UINT frame)
{
    UINT map;
    UINT *tmp;
    map = frame >> pmm_FRAMES_PER_MAP_DIVISOR;
    frame = frame - (map * pmm_FRAMES_PER_MAP);
    tmp = (UINT*) (pmm_MapsStart+ (pmm_MapSize*map));
    tmp += (frame >> 5);
    *tmp |= MAKRO_SET_BIT(frame&(31));
}


/***********************************************************************/
/***********************************************************************/
/**
* @brief Function to free a map of the phys. memory manager.
*
* @param map The number of the map to be freed.
*/
void free_map (UINT map)
{
    UINT *tmp;
    tmp = (UINT*)pmm_SMapStart;
    tmp += (map>>5);
    *tmp &= ~MAKRO_SET_BIT(map&(31));
}

/**
* @brief Function to use a map of the phys. memory manager.
*
* @param map The number of the map to be used.
*/
void use_map (UINT map)
{
    UINT *tmp;
    tmp = (UINT*)pmm_SMapStart;
    tmp += (map>>5);
    *tmp |= MAKRO_SET_BIT(map&(31));
}


/***********************************************************************/
/***********************************************************************/
/**
* @brief Function to check if a map is realy used or not.
*
* It is not sure if this version works. If not, the one wich is in comments
* is to be used.
*
* @param map The number of the map to be checked.
* @return If 0, then the map is not completely used. If 1, the map is full.
*/
char check_map_used (UINT map)
{
    UINT *tmp;
    UINT i;
    tmp = (UINT*) (pmm_MapsStart+ (pmm_MapSize*map));
    i = pmm_MapSize;
    for (; i!=0; i--) {
        if (*tmp!=0xFFFFFFFF) {
            return 0;
        }
        tmp++;
    }
//     i = 0;
//     for(; i<pmm_MapSize; i++) {
//         if(*tmp!=0xFFFFFFFF){
//             return 0;
//         }
//         tmp++;
//     }
    return 1;
}



/***********************************************************************/
/***********************************************************************/
/**
* @brief The function searches for the first free frame in memory.
*
* @return The function returns the number of the first free frame.
*/
UINT find_first_free ()
{
    UINT* tmp;
    UINT counta = 0;
    UINT countb = 0;
    UINT countc = 0;
    tmp = (UINT*) pmm_SMapStart;
    while (*tmp == 0xFFFFFFFF) {
        counta++;
        if (counta>=pmm_SMapSize) return pmm_ret_MEMFULL;
        tmp++;
    }
    while (*tmp & MAKRO_SET_BIT(countb)) {
        countb++;
        if (countb>=32) return pmm_ret_MEMFULL;
    }
    counta <<= 5;// * (2^5) == * 32
    counta += countb;//counta is now the map number
    countb = 0;
    tmp = (UINT*)(pmm_MapsStart+(pmm_MapSize*counta));//pointer on map
    while (*tmp == 0xFFFFFFFF) {
        countb++;
        if (countb>=pmm_MapSize) return pmm_ret_MEMFULL;
        tmp++;
    }
    while (*tmp & MAKRO_SET_BIT(countc)) {
        countc++;
        if (countc>=32) return pmm_ret_MEMFULL;
    }
    countb <<= 5;
    countb += countc;//countb is now the frame number in the map
    counta <<= pmm_FRAMES_PER_MAP_DIVISOR;
    counta += countb;//counta is now the global frame number
    return counta;
}



/***********************************************************************/
/***********************************************************************/
/**
* @brief The function allocs a phys. Frame.
*
* @return The function returns the frame number or pmm_ret_MEMFULL
*         if memory is full.
*/
UINT pmm_alloc_frame ()
{
    UINT frame;
    UINT map;
    frame = find_first_free();
    if (frame == pmm_ret_MEMFULL) {
        return pmm_ret_MEMFULL;
    }
    use_frame(frame);
    map = frame >> pmm_FRAMES_PER_MAP_DIVISOR;
    if (check_map_used(map)) {
        use_map(map);
    }
    return frame;
}

/**
* @brief The function frees the given frame.
*
* @param frame The number of the frame to be freed. There is no validation!
*/
void pmm_free_frame (UINT frame)
{
    UINT map;
    map = frame >> pmm_FRAMES_PER_MAP_DIVISOR;
    free_frame(frame);
    if (!check_map_used(map)) {
        free_map(map);
    }
    return;
}
/***********************************************************************/
/***********************************************************************/

void use_memrange (UINT start, UINT size)
{
	UINT startframe;
	UINT endframe;
	UINT tmp;
	UINT map;
	
	if (start < pmm_Start) {
/*		if (start + size < pmm_Start) {
			return;
		} else {
			size = size - (pmm_Start - start);
			start = pmm_Start;
		}*/
		return;
	}
	endframe = ResolveFramefromAddress (start + size);
	startframe = ResolveFramefromAddress (start); 
	
	if (endframe >= pmm_LastFrame) {
		return;
	}
	
	tmp = endframe - startframe;
	tmp++; //TODO correct? -> Because endframe AND startframe must be set to used...?
	for (; tmp > 0; tmp--, startframe++) {
		use_frame (startframe);
		map = startframe >> pmm_FRAMES_PER_MAP_DIVISOR;
		if (check_map_used (map) == 1) {
			use_map (map);
		}
	}
	
	return;
}





