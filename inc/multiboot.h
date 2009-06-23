//Multiboot structures and functions

#ifndef __KERNEL__MULTIBOOT___HEADER____
#define __KERNEL__MULTIBOOT___HEADER____

#include "types.h"




/** multiboot structures **/
typedef struct
{
    UINT tabsize;
    UINT strsize;
    UINT addr;
    UINT reserved;
} aout_symbol_table;

typedef struct
{
    UINT num;
    UINT size;
    UINT addr;
    UINT shndx;
} elf_section_header_table;



typedef struct
{
    UINT flags;
    UINT mem_lower;
    UINT mem_upper;
    UINT boot_device;
    UINT cmdline;
    UINT mods_count;
    UINT mods_addr;
    union
    {
        aout_symbol_table aout_sym;
        elf_section_header_table elf_sec;
    } u;
    UINT mmap_length;
    UINT mmap_addr;
    UINT drives_length;
    UINT drives_addr;
    UINT config_table;
    UINT boot_loader_name;
    UINT apm_table;
//here could be more
} multiboot_info;





typedef struct
{
  UINT size;
  UINT64 base_addr;
  UINT64 length;
  UINT type;
} multiboot_memory_map;






/**functions to handle multiboot info **/
extern INT8 read_mboot_info (UINT, UINT *);


extern UINT mboot_get_memsize_bytes (void);
extern UINT mboot_get_mmap_length (void);
extern UINT mboot_get_mmap_add (void);





#endif //(__KERNEL__MULTIBOOT___HEADER____)
