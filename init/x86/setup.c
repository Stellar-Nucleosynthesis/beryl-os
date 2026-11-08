#include <kmain.h>
#include <gdt/gdt.h>
#include <memory/page_frame_mapper.h>

static void fb_print_hex(u32 value)
{
    static const char hex[] = "0123456789ABCDEF";
    static u32 row = 0;

    volatile u8* fb = (volatile u8*) 0xC00B8000;

    for (u32 i = 0; i < 8; i++)
    {
        u32 shift = (7 - i) * 4;
        fb[row * 160 + i * 2] = hex[(value >> shift) & 0xF];
        fb[row * 160 + i * 2 + 1] = 0x0A;
    }

    row++;
}

void setup()
{
    setup_gdt();
    page_frame page_dir = get_page_dir();
    init_page_frame_mapper(page_dir);

    virt_page pages[] = {
        { { 0xC0400000 } },
        { { 0xC0401000 } },
        { { 0xC0800000 } },
        { { 0xC1000000 } },
        { { 0xC2000000 } },
        { { 0xC4000000 } },
        { { 0xC8000000 } },
        { { 0xD0000000 } },
        { { 0xE0000000 } },
    };

    for (u32 i = 0; i < sizeof(pages) / sizeof(pages[0]); ++i)
    {
        page_frame mapping = map_page_frame(page_dir, pages[i]);
        fb_print_hex(mapping.start.value);

        u32* ptr = (u32*) pages[i].start.value;
        *ptr = 20 + i;
    }

    kmain();
}