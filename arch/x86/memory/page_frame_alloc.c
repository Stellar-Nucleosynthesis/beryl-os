#include <memory/page_frame_alloc.h>
#include <types/sysbool.h>

static u8 bitmap[PAGE_COUNT];

static bool is_page_used(u32 index)
{
    return bitmap[index / 8] & (1u << (index % 8));
}

static void set_page_used(u32 index)
{
    bitmap[index / 8] |= (1u << (index % 8));
}

static void set_page_free(u32 index)
{
    bitmap[index / 8] &= ~(1u << (index % 8));
}

static void mark_used_page(page_frame page)
{
    phys_addr addr = page.start;
    if (addr.value % PAGE_SIZE != 0)
        return;
    set_page_used(addr.value / PAGE_SIZE);
}

static page_frame alloc_page()
{
    u32 idx = 0;
    for (u32 i = 0; i < PAGE_COUNT; i++)
    {
        if (!is_page_used(i))
        {
            set_page_used(i);
            idx = i;
            break;
        }
    }
    return (page_frame) { (phys_addr) { idx * PAGE_SIZE } };
}

static void free_page(page_frame page)
{
    phys_addr addr = page.start;
    if (addr.value % PAGE_SIZE != 0)
        return;
    u32 index = addr.value / PAGE_SIZE;
    set_page_free(index);
}