ENTRY(kernelentry)
OUTPUT_FORMAT(elf32-i386)

SECTIONS
{
	. = 0x100000;
	__kernel_start = .;
	.boot :
	{
		*(.boot)
		*(.setup)
		*(.setup_data)
	}
	__EndOfStartupCode = .;

	.text ALIGN(4096):
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
	__RealStartOfLinkedMemory = ALIGN(4096);
	. = 0xFFC00000;
	__StartOfLinkedMemory = .;
	.linked : AT (__RealStartOfLinkedMemory)
	{
		*(.klinked)
		*(.klinked_data)
	}
	__EndOfLinkedMemory = .;
	__kernel_end = (__EndOfLinkedMemory - __StartOfLinkedMemory)+(__RealStartOfLinkedMemory);
}
