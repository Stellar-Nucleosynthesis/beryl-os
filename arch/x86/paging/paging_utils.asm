global get_page_dir
global set_page_dir
global invalidate_page

section .text
get_page_dir:
    mov eax, cr3
    mov edx, [esp + 4]
    mov [edx], eax
    ret

set_page_dir:
    mov eax, [esp + 4]
    mov cr3, eax
    ret

invalidate_page:
    mov eax, [esp + 4]
    invlpg [eax]
    ret