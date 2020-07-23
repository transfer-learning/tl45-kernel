    .globl _start
_start:
    ; 0x38 0000
    ; addi r1, r0, -8
    ; sw r0, r1, 0 ; Activate UART
    addhi sp, r0, 0x0038
    call r0, main
    ;halt
    
    .globl halt
halt:
    .int 0xffffffff
    jmpi halt

    .globl nothing
nothing:
    ret

    .globl _Z7nothingv
_Z7nothingv:
    ret

    .globl _Z7nothingi
_Z7nothingi:
    ret

