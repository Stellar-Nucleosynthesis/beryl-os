global init

extern setup

KERNEL_VIRTUAL_BASE equ 0xC0000000

MAGIC_NUMBER        equ 0x1BADB002
FLAGS               equ 0x0
CHECKSUM            equ -MAGIC_NUMBER

KERNEL_STACK_SIZE   equ 0x10000

section .multiboot
align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

section .boot.bss nobits
align 4096
boot_page_directory:
    resd 1024

align 16
kernel_stack:
    resb KERNEL_STACK_SIZE
kernel_stack_end:

boot_multiboot_magic:
    resd 1

boot_multiboot_info:
    resd 1

section .boot.text
init:
    cli

    mov [boot_multiboot_magic], eax
    mov [boot_multiboot_info], ebx

    mov esp, kernel_stack_end

    mov dword [boot_page_directory], 0x00000083
    mov dword [boot_page_directory + 0xC00], 0x00000083

    mov eax, cr4
    or eax, 0x10
    mov cr4, eax

    mov eax, boot_page_directory
    mov cr3, eax

    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    mov eax, higher_half + KERNEL_VIRTUAL_BASE
    jmp eax

higher_half:
    mov esp, kernel_stack_end + KERNEL_VIRTUAL_BASE
    mov dword [boot_page_directory + KERNEL_VIRTUAL_BASE], 0
    invlpg [0]

    push dword [boot_multiboot_info + KERNEL_VIRTUAL_BASE]
    push dword [boot_multiboot_magic + KERNEL_VIRTUAL_BASE]

    mov eax, setup
    call eax