[BITS 32]

; assembler code to load the idt

[EXTERN pIDT]
[GLOBAL IDT_load]

IDT_load:
    lidt [pIDT]
    ret

