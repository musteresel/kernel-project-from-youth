// Phys. memory management

#ifndef __KERNEL__PHYS_MEM___HEADER____
#define __KERNEL__PHYS_MEM___HEADER____

#include "types.h"

extern UINT ResolveFramefromAddress (UINT);
extern UINT ResolveAddressfromFrame (UINT);

extern void free_frame (UINT);
extern void use_frame (UINT);
extern INT8 check_frame_used (UINT);

extern void free_map (UINT);
extern void use_map (UINT);
extern INT8 check_map_used (UINT);

extern UINT find_first_free ();

extern UINT pmm_alloc_frame ();
extern void pmm_free_frame (UINT);

extern UINT *pmm_alloc ();
extern void pmm_free (UINT*);

extern void use_memrange (UINT, UINT);

/* TODO this function is a stub.. */
extern UINT pmm_alloc_frames (UINT);


#endif //(__KERNEL__PHYS_MEM___HEADER____)

