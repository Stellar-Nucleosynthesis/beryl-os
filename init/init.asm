global init

extern setup

MAGIC_NUMBER        equ 0x1BADB002
FLAGS               equ 0x0
CHECKSUM            equ -MAGIC_NUMBER

KERNEL_STACK_SIZE   equ 4096

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
    mov esp, kernel_stack + KERNEL_STACK_SIZE
    call setup