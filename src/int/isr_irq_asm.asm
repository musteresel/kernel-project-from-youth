[BITS 32]

[EXTERN ErrorHandler]
[EXTERN IRQHandler]

[SECTION .klinked]

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;#######################################################################
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
%macro SAVE_ALL_REGS 0
    pushad
    push ds
    push es
    push fs
    push gs
%endmacro
%macro RESTORE_ALL_REGS 0
    pop gs
    pop fs
    pop es
    pop ds
    popad
%endmacro
%macro CLEAR_ERR_INT_NUM 0
    add esp,8
%endmacro
%macro SET_KERNELCONTEXT 0
    mov ax,0x10
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
%endmacro
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;#######################################################################
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
%macro ISR_NO_ERR 1
        [GLOBAL _isr%1]
        _isr%1:
                cli
                push byte 0 ;Pseudo Err Code
                push byte %1
                jmp ISR_preHandler
%endmacro
%macro ISR_ERR 1
        [GLOBAL _isr%1]
        _isr%1:
                cli
                push byte %1
                jmp ISR_preHandler
%endmacro
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;#######################################################################
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ISR_preHandler:
	SAVE_ALL_REGS
	SET_KERNELCONTEXT
	mov eax, esp
	push eax
	mov eax, ErrorHandler
	call eax
	pop eax
	RESTORE_ALL_REGS
	CLEAR_ERR_INT_NUM
	sti
	iretd
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;#######################################################################
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ISR_NO_ERR 0
ISR_NO_ERR 1
ISR_NO_ERR 2
ISR_NO_ERR 3
ISR_NO_ERR 4
ISR_NO_ERR 5
ISR_NO_ERR 6
ISR_NO_ERR 7
ISR_ERR 8
ISR_NO_ERR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR_NO_ERR 15
ISR_NO_ERR 16
ISR_NO_ERR 17
ISR_NO_ERR 18
ISR_NO_ERR 19
ISR_NO_ERR 20
ISR_NO_ERR 21
ISR_NO_ERR 22
ISR_NO_ERR 23
ISR_NO_ERR 24
ISR_NO_ERR 25
ISR_NO_ERR 26
ISR_NO_ERR 27
ISR_NO_ERR 28
ISR_NO_ERR 29
ISR_NO_ERR 30
ISR_NO_ERR 31
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;#######################################################################
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
%macro IRQ 1
        [GLOBAL _irq%1]
        _irq%1:
            cli
            push byte (%1 +32)
            jmp IRQ_preHandler
%endmacro
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;#######################################################################
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
IRQ_preHandler:
	SAVE_ALL_REGS
	SET_KERNELCONTEXT
	push esp
	call IRQHandler
	mov esp, eax
	RESTORE_ALL_REGS
	add esp,4
	mov al, 0x20
        out 0x20, al
        out 0xA0, al
	sti
	iretd
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;#######################################################################
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;#######################################################################
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
