#include <kmain.h>
#include <gdt/gdt.h>
#include <memory/page_frame_alloc.h>
#include <memory/virt_page_alloc.h>
#include <memory/page_frame_mapper.h>
#include <memory/alloc_memory.h>
#include <memory/kalloc.h>

void __kernel_physical_end(void);
void __kernel_virtual_end(void);

phys_addr kernel_phys_end = (phys_addr) { (u32) &__kernel_physical_end };
virt_addr kernel_virt_end = (virt_addr) { (u32) &__kernel_virtual_end };

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
#define RESERVED_VIRTUAL_MEMORY_START 0xFFC00000

void parse_available_phys_memory(multiboot_info* mbi)
{
    if (!(mbi->flags & MULTIBOOT_INFO_MEM_MAP))
        return;
    u64 kernel_end = (kernel_phys_end.value + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
    virt_addr mmap = phys_to_virt_direct((phys_addr) { mbi->mmap_addr });
    for (u32 offset = 0; offset < mbi->mmap_length;)
    {
        multiboot_mmap_entry* entry = (multiboot_mmap_entry*) (mmap.value + offset);
        offset += entry->size + sizeof(entry->size);
        if (entry->type != 1)
            continue;
        u64 start = entry->addr < kernel_end ? kernel_end : entry->addr;
        u64 end = entry->addr + entry->len;
        start = (start + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
        end &= ~(PAGE_SIZE - 1);
        for (u64 addr = start; addr < end; addr += PAGE_SIZE)
            free_page_frame((page_frame) { { addr } });
    }
}

void reserve_virtual_memory()
{
    u32 start = KERNEL_OFFSET;
    u32 end = (kernel_virt_end.value + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
    for (u32 addr = start; addr < end; addr += PAGE_SIZE)
        mark_used_virt_page((virt_page) { { addr } });
    for (u32 addr = RESERVED_VIRTUAL_MEMORY_START; addr != 0; addr += PAGE_SIZE)
        mark_used_virt_page((virt_page) { { addr } });
}

void setup(u32 magic, phys_addr mbi_addr)
{
    if (magic != 0x2BADB002)
        return;

    setup_gdt();
    init_page_frame_alloc();
    init_virt_page_alloc();
    parse_available_phys_memory((multiboot_info*) phys_to_virt_direct(mbi_addr).value);
    reserve_virtual_memory();
    page_frame page_dir = get_page_dir();
    init_page_frame_mapper(page_dir);
    init_memory_alloc();

    kmain();
}