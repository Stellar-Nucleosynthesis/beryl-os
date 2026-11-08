#ifndef PAGING_H
#define PAGING_H

#include <types/sysint.h>

#define PAGE_SIZE   0x1000
#define PAGE_COUNT  0x100000

#define KERNEL_OFFSET           0xC0000000

#define PAGE_DIR_INDEX(addr)    (((addr).value >> 22) & 0x3FF)
#define PAGE_TABLE_INDEX(addr)  (((addr).value >> 12) & 0x3FF)
#define PAGE_OFFSET(addr)       ((addr).value & 0xFFF)

#define PAGE_PRESENT            0x001
#define PAGE_WRITABLE           0x002
#define PAGE_SIZE_4MB           0x080
#define PAGE_ADDR_MASK          0xFFFFF000

#define PAGE_FLAGS              (PAGE_PRESENT | PAGE_WRITABLE)

typedef struct
{
    phys_addr start;
} page_frame;

typedef struct
{
    virt_addr start;
} virt_page;

page_frame get_page_dir();

void set_page_dir(page_frame page_dir);

void invalidate_page(virt_page page);

virt_addr phys_to_virt_direct(phys_addr addr);

page_frame invalid_page_frame();

bool is_page_frame_invalid(page_frame pf);

virt_page invalid_virtual_page();

bool is_virtual_page_invalid(virt_page page);

#endif