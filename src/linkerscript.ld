ENTRY(kernelentry)
OUTPUT_FORMAT(elf32-i386)

SECTIONS
{
	. = 0x100000;
	__kernel_start = .;
	.boot :
	{
		*(.boot)
	}

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
	id_end = ALIGN(4096);
	. = 0xFFFF0000;
	linked_start = .;
	.linked : AT (id_end)
	{
		*(.klinked)
		*(.klinked_data)
	}
	linked_end = .;
	__kernel_end = (linked_end - linked_start)+(id_end);
}
