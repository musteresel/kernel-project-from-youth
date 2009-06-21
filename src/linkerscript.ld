ENTRY(kernelentry)
OUTPUT_FORMAT(elf32-i386)

SECTIONS
{
    . = 0x100000;
    __kernel_start = .;
    .text :
{
        *(.text)
}

    .data :
{
        *(.data)
        *(.rodata)
}

    .bss :
{
        *(.bss)
}
    __kernel_end = .;
    end = .;
}
