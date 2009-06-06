//Multiboot structures and functions

#ifndef __KERNEL__MULTIBOOT___HEADER____
#define __KERNEL__MULTIBOOT___HEADER____



/** multiboot structures **/
typedef struct
{
    unsigned int tabsize;
    unsigned int strsize;
    unsigned int addr;
    unsigned int reserved;
} aout_symbol_table;

typedef struct
{
    unsigned int num;
    unsigned int size;
    unsigned int addr;
    unsigned int shndx;
} elf_section_header_table;



typedef struct
{
    unsigned int flags;
    unsigned int mem_lower;
    unsigned int mem_upper;
    unsigned int boot_device;
    unsigned int cmdline;
    unsigned int mods_count;
    unsigned int mods_addr;
    union
    {
        aout_symbol_table aout_sym;
        elf_section_header_table elf_sec;
    } u;
    unsigned int mmap_length;
    unsigned int mmap_addr;
    unsigned int drives_length;
    unsigned int drives_addr;
    unsigned int config_table;
    unsigned int boot_loader_name;
    unsigned int apm_table;
//here could be more
} multiboot_info;


/**functions to handle multiboot info **/
extern char read_mboot_info (unsigned int, unsigned int *);










#endif //(__KERNEL__MULTIBOOT___HEADER____)
