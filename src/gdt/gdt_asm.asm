[BITS 32]

; assembler code to load the gdt and the tss

[EXTERN pGDT]


[GLOBAL GDT_flush]
[GLOBAL TSS_flush]


GDT_flush:
    lgdt [pGDT]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2
flush2:
    ret




TSS_flush:
    ;5. entry => 0x28 and 3 (accesable from ring 3)
    mov ax,0x2b
    ltr ax
    ret



