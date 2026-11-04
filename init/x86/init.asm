global init

extern setup

MAGIC_NUMBER        equ 0x1BADB002
FLAGS               equ 0x0
CHECKSUM            equ -MAGIC_NUMBER

KERNEL_STACK_SIZE   equ 0x10000

section .multiboot
align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

section .bss
align 16
kernel_stack:
    resb KERNEL_STACK_SIZE

section .text
init:
    cli
    mov esp, kernel_stack + KERNEL_STACK_SIZE
    call setup