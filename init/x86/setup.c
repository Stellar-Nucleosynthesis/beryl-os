#include <kmain.h>
#include <gdt/gdt.h>
#include <memory/page_frame_alloc.h>
#include <memory/page_frame_mapper.h>

void __kernel_physical_start(void);
void __kernel_physical_end(void);

phys_addr kernel_start = (phys_addr) { (u32) &__kernel_physical_start };
phys_addr kernel_end = (phys_addr) { (u32) &__kernel_physical_end };

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

typedef struct
{
    u32 flags;
    u32 mem_lower;
    u32 mem_upper;
    u32 boot_device;
    u32 cmdline;
    u32 mods_count;
    u32 mods_addr;
    u32 syms[4];
    u32 mmap_length;
    u32 mmap_addr;
} multiboot_info;

typedef struct __attribute__((packed))
{
    u32 size;
    u64 addr;
    u64 len;
    u32 type;
} multiboot_mmap_entry;

#define MULTIBOOT_INFO_MEM_MAP (1 << 6)
#define RESERVED_MEMORY_END 0x100000

void parse_available_memory(multiboot_info* mbi)
{
    if (!(mbi->flags & MULTIBOOT_INFO_MEM_MAP))
        return;

    u64 kernel_start_addr = kernel_start.value;
    u64 kernel_end_addr = kernel_end.value;

    kernel_start_addr &= ~(PAGE_SIZE - 1);
    kernel_end_addr = (kernel_end_addr + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);

    virt_addr mmap_virt_addr = phys_to_virt_direct((phys_addr) { mbi->mmap_addr });
    u32 offset = 0;
    while (offset < mbi->mmap_length)
    {
        multiboot_mmap_entry* entry = (multiboot_mmap_entry*) (mmap_virt_addr.value + offset);
        if (entry->type == 1)
        {
            u64 start = entry->addr;
            u64 end = entry->addr + entry->len;

            if (start < RESERVED_MEMORY_END)
                start = RESERVED_MEMORY_END;

            start = (start + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
            end &= ~(PAGE_SIZE - 1);
            for (u64 addr = start; addr < end; addr += PAGE_SIZE)
            {
                if (addr >= kernel_start_addr && addr < kernel_end_addr)
                    continue;
                free_page((page_frame) { { addr } });
            }
        }
        offset += entry->size + sizeof(entry->size);
    }
}

void setup(u32 magic, phys_addr mbi_addr)
{
    if (magic != 0x2BADB002)
        return;

    setup_gdt();
    init_page_frame_alloc();
    parse_available_memory((multiboot_info*) phys_to_virt_direct(mbi_addr).value);
    page_frame page_dir = get_page_dir();
    fb_print_hex(page_dir.start.value);
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