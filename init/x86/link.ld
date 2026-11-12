ENTRY(init)

KERNEL_VIRTUAL_BASE     = 0xC0000000;
KERNEL_PHYSICAL_BASE    = 0x00100000;

SECTIONS
{
    . = KERNEL_PHYSICAL_BASE;

    .multiboot ALIGN(4) :
    {
        KEEP(*(.multiboot))
    }

    .boot ALIGN(0x1000) :
    {
        *(.boot.text)
        *(.boot.data)
        *(.boot.bss)
    }

    __kernel_physical_start = ALIGN(., 0x1000);
    . = __kernel_physical_start + KERNEL_VIRTUAL_BASE;
    __kernel_virtual_start = .;

    .text ALIGN(0x1000) : AT(ADDR(.text) - KERNEL_VIRTUAL_BASE)
    {
        *(.text)
    }

    .rodata ALIGN(0x1000) : AT(ADDR(.rodata) - KERNEL_VIRTUAL_BASE)
    {
        *(.rodata*)
    }

    .data ALIGN(0x1000) : AT(ADDR(.data) - KERNEL_VIRTUAL_BASE)
    {
        *(.data)
    }

    .bss ALIGN(0x1000) : AT(ADDR(.bss) - KERNEL_VIRTUAL_BASE)
    {
        *(COMMON)
        *(.bss)
    }

    __kernel_virtual_end = .;
    __kernel_physical_end = . - KERNEL_VIRTUAL_BASE;
}