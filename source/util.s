.cpu cortex-a9
.align 4
.code 32

.text

.section .text.util_s

.global delay
delay:
    subs r0, #0x1
    bne delay
    bx lr