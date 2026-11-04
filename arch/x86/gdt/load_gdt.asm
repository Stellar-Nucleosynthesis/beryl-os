global load_gdt
global load_data_segment
global load_code_segment

section .text
load_gdt:
    sub esp, 6

    mov ax, [esp + 14]
    dec ax
    mov [esp], ax

    mov eax, [esp + 10]
    mov [esp + 2], eax

    lgdt [esp]

    add esp, 6
    ret

load_data_segment:
    mov eax, [esp + 4]
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret

load_code_segment:
    mov eax, [esp + 4]

    push eax
    push dword .reload_cs
    retf

.reload_cs:
    ret
