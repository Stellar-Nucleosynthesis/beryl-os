#include <paging/paging.h>

#define KERNEL_OFFSET 0xC0000000

virt_addr phys_to_virt_direct(phys_addr addr)
{
    return (virt_addr) { addr.value + KERNEL_OFFSET };
}

#define PAGE_DIR_INDEX(addr)    (((addr).value >> 22) & 0x3FF)
#define PAGE_TABLE_INDEX(addr)  (((addr).value >> 12) & 0x3FF)
#define PAGE_OFFSET(addr)       ((addr).value & 0xFFF)

#define PAGE_PRESENT            0x001
#define PAGE_SIZE_4MB           0x080
#define PAGE_ADDR_MASK          0xFFFFF000

phys_addr resolve_addr(phys_addr page_dir, virt_addr addr)
{
    u32* directory = (u32*) phys_to_virt_direct(page_dir).value;
    u32 pde = directory[PAGE_DIR_INDEX(addr)];
    if (!(pde & PAGE_PRESENT))
        return (phys_addr) { NULL };
    if (pde & PAGE_SIZE_4MB)
        return (phys_addr) { (pde & 0xFFC00000) | (addr.value & 0x003FFFFF) };

    u32* table = (u32*) phys_to_virt_direct((phys_addr) { pde & PAGE_ADDR_MASK }).value;
    u32 pte = table[PAGE_TABLE_INDEX(addr)];
    if (!(pte & PAGE_PRESENT))
        return (phys_addr) { NULL };
    return (phys_addr) { (pte & PAGE_ADDR_MASK) | PAGE_OFFSET(addr) };
}

