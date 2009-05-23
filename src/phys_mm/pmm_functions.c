/////////////////////////////////////////////////////////////////////////
/* pmm_functions.c
Functions to controll the physical memory management

-----------------------------
This file is part of kernel.
By Daniel Oertwig
*/
/////////////////////////////////////////////////////////////////////////




extern unsigned int pmm_MapsStart;//start of maps
extern unsigned int pmm_MapSize;//size of one map
extern unsigned int pmm_FRAMES_PER_MAP_DIVISOR;
extern unsigned int pmm_FRAMES_PER_MAP;
extern unsigned int pmm_SMapStart;//start of supermap
extern unsigned int pmm_SMapSize;//Size of supermap
extern unsigned int pmm_ret_MEMFULL;
extern unsigned int pmm_Start;//Starting of managed memory


#define MAKRO_SET_BIT(x) (( 1 << ( (x) )))

/***********************************************************************/
/***********************************************************************/
unsigned int ResolveFramefromAddress (unsigned int ad)
{
    unsigned int frame;
    frame = (ad - pmm_Start);
    frame = frame >> 12;
    return frame;
}
unsigned int ResolveAddressfromFrame (unsigned int fr)
{
    unsigned int ad;
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
void free_frame (unsigned int frame)
{
    unsigned int map;
    unsigned int *tmp;
    map = frame >> pmm_FRAMES_PER_MAP_DIVISOR;
    frame = frame - (map * pmm_FRAMES_PER_MAP);
    tmp = (unsigned int*) (pmm_MapsStart+ (pmm_MapSize*map));
    tmp += (frame >> 5);
    *tmp &= ~MAKRO_SET_BIT(frame&(31));
}

/**
* @brief Function to use a phys. frame in memory.
*
* @param frame The number of the frame to be used.
*/
void use_frame (unsigned int frame)
{
    unsigned int map;
    unsigned int *tmp;
    map = frame >> pmm_FRAMES_PER_MAP_DIVISOR;
    frame = frame - (map * pmm_FRAMES_PER_MAP);
    tmp = (unsigned int*) (pmm_MapsStart+ (pmm_MapSize*map));
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
void free_map (unsigned int map)
{
    unsigned int *tmp;
    tmp = (unsigned int*)pmm_SMapStart;
    tmp += (map>>5);
    *tmp &= ~MAKRO_SET_BIT(map&(31));
}

/**
* @brief Function to use a map of the phys. memory manager.
*
* @param map The number of the map to be used.
*/
void use_map (unsigned int map)
{
    unsigned int *tmp;
    tmp = (unsigned int*)pmm_SMapStart;
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
char check_map_used (unsigned int map)
{
    unsigned int *tmp;
    unsigned int i;
    tmp = (unsigned int*) (pmm_MapsStart+ (pmm_MapSize*map));
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
unsigned int find_first_free ()
{
    unsigned int* tmp;
    unsigned int counta = 0;
    unsigned int countb = 0;
    unsigned int countc = 0;
    tmp = (unsigned int*) pmm_SMapStart;
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
    tmp = (unsigned int*)(pmm_MapsStart+(pmm_MapSize*counta));//pointer on map
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
unsigned int pmm_alloc_frame ()
{
    unsigned int frame;
    unsigned int map;
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
void pmm_free_frame (unsigned int frame)
{
    unsigned int map;
    map = frame >> pmm_FRAMES_PER_MAP_DIVISOR;
    free_frame(frame);
    if (!check_map_used(map)) {
        free_map(map);
    }
    return;
}
/***********************************************************************/
/***********************************************************************/
