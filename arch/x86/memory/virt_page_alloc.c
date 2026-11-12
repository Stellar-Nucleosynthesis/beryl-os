#include <memory/virt_page_alloc.h>
#include <types/bool.h>

#define VIRT_PAGE_COUNT ((0x100000000 - KERNEL_OFFSET) / PAGE_SIZE)

u8 virt_page_bitmap[VIRT_PAGE_COUNT / 8];

bool is_virt_page_used(u32 index)
{
    return virt_page_bitmap[index / 8] & (1u << (index % 8));
}

void set_virt_page_used(u32 index)
{
    virt_page_bitmap[index / 8] |= (1u << (index % 8));
}

void set_virt_page_free(u32 index)
{
    virt_page_bitmap[index / 8] &= ~(1u << (index % 8));
}

u32 virt_addr_to_index(virt_addr addr)
{
    return (addr.value - KERNEL_OFFSET) / PAGE_SIZE;
}

void mark_used_virt_page(virt_page page)
{
    if (page.start.value < KERNEL_OFFSET)
        return;
    set_virt_page_used(virt_addr_to_index(page.start));
}

virt_page alloc_virt_page()
{
    for (u32 i = 0; i < VIRT_PAGE_COUNT; i++)
    {
        if (!is_virt_page_used(i))
        {
            set_virt_page_used(i);
            return (virt_page) { (virt_addr) { KERNEL_OFFSET + i * PAGE_SIZE } };
        }
    }
    return invalid_virt_page();
}

virt_page_block alloc_virt_page_block(u32 size)
{
    if (size > VIRT_PAGE_COUNT)
        return invalid_virt_page_block();
    for (u32 i = 0; i <= VIRT_PAGE_COUNT - size; i++)
    {
        if (is_virt_page_used(i))
            continue;
        u32 j = 1;
        while (j < size && !is_virt_page_used(i + j))
            j++;
        if (j != size)
        {
            i += j;
            continue;
        }
        for (u32 k = 0; k < size; k++)
            set_virt_page_used(i + k);
        return (virt_page_block) { { KERNEL_OFFSET + i * PAGE_SIZE }, size };
    }
    return invalid_virt_page_block();
}

void free_virt_page(virt_page page)
{
    if (page.start.value < KERNEL_OFFSET)
        return;
    set_virt_page_free(virt_addr_to_index(page.start));
}

void free_virt_page_block(virt_page_block block)
{
    if (block.start.value < KERNEL_OFFSET)
        return;
    if (block.size == 0 || block.size > VIRT_PAGE_COUNT)
        return;
    for (int i = 0; i < block.size; i++)
    {
        virt_page page = { { block.start.value + i * PAGE_SIZE } };
        free_virt_page(page);
    }
}

void init_virt_page_alloc()
{
    for (u32 i = 0; i < sizeof(virt_page_bitmap); i++)
        virt_page_bitmap[i] = 0x0;
}