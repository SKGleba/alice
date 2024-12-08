.cpu cortex-a9
.align 4
.code 16

.text

.section .text.util_s

.global delay
.thumb_func
delay:
# WORDed bc of compiler err:
#   subs r0, #1
#   bne delay
    .word 0xd1fd3801
    bx lr