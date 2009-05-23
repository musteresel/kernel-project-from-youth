// Phys. memory management

#ifndef __KERNEL__PHYS_MEM___HEADER____
#define __KERNEL__PHYS_MEM___HEADER____



extern unsigned int ResolveFramefromAddress (unsigned int);
extern unsigned int ResolveAddressfromFrame (unsigned int);

extern void free_frame (unsigned int);
extern void use_frame (unsigned int);

extern void free_map (unsigned int);
extern void use_map (unsigned int);
extern void check_map_used (unsigned int);

extern unsigned int find_first_free ();

extern unsigned int pmm_alloc_frame ();
extern void pmm_free_frame (unsigned int);


#endif //(__KERNEL__PHYS_MEM___HEADER____)

