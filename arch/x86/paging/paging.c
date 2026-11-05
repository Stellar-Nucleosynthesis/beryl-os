#include <paging/paging.h>

const u32 KERNEL_OFFSET = 0xC0000000;

phys_addr resolve_addr(virt_addr addr)
{
    return (phys_addr) { addr.value - KERNEL_OFFSET };
}