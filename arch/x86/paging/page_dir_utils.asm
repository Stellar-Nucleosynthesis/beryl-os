global get_page_dir_base
global set_page_dir_base

section .text
get_page_dir_base:
    mov eax, cr3
    ret

set_page_dir_base:
    mov eax, [esp + 4]
    mov cr3, eax
    ret