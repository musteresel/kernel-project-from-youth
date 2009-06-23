[BITS 32]

[GLOBAL kernelentry]
[EXTERN c_main]

FLAGS	equ 0
MAGIC	equ 0x1BADB002
CHECKSUM	equ -(MAGIC + FLAGS)

[SECTION .boot]
align 4
MultibootHeader:
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

kernelentry:
	lea esp,[kstack]
	;mov esp,[kstack]
	push esp ;Initial stack
	push ebx ;Multiboot struct ..?
	push eax ;Magic Number..?
	
	call c_main
	
	cli
	hlt


[SECTION .bss]
resb 32768 ;32 KB Stack space
kstack:
