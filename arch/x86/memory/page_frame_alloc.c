#include <memory/page_frame_alloc.h>
#include <types/sysbool.h>

u8 bitmap[PAGE_COUNT / 8];

bool is_page_used(u32 index)
{
    return bitmap[index / 8] & (1u << (index % 8));
}

void set_page_used(u32 index)
{
    bitmap[index / 8] |= (1u << (index % 8));
}

void set_page_free(u32 index)
{
    bitmap[index / 8] &= ~(1u << (index % 8));
}

void mark_used_page(page_frame page)
{
    phys_addr addr = page.start;
    set_page_used(addr.value / PAGE_SIZE);
}

page_frame alloc_page()
{
    for (u32 i = 0; i < PAGE_COUNT; i++)
    {
        if (!is_page_used(i))
        {
            set_page_used(i);
            return (page_frame) { (phys_addr) { i * PAGE_SIZE } };
        }
    }
    return invalid_page_frame();
}

void free_page(page_frame page)
{
    phys_addr addr = page.start;
    u32 index = addr.value / PAGE_SIZE;
    set_page_free(index);
}

void init_page_frame_alloc()
{
    for (u32 i = 0; i < sizeof(bitmap); i++)
        bitmap[i] = 0xFF;
}