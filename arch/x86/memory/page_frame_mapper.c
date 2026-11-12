#include <memory/page_frame_mapper.h>
#include <memory/page_frame_alloc.h>

#define TEMP_PAGE_DIR_INDEX     1023
#define TEMP_PAGE_TABLE_INDEX   0
#define TEMP_PAGE_ADDR          0xFFC00000

__attribute__((aligned(PAGE_SIZE)))
u8 temp_page_table[PAGE_SIZE];

virt_page map_temp_page(page_frame page)
{
    virt_page temp_page = { { TEMP_PAGE_ADDR } };
    u32* pte = (u32*) temp_page_table;
    if (*pte & PAGE_PRESENT)
        return invalid_virtual_page();
    *pte = page.start.value | PAGE_FLAGS;
    invalidate_page(temp_page);
    return temp_page;
}

void unmap_temp_page(virt_page page)
{
    if (is_virtual_page_invalid(page))
        return;
    u32* pte = (u32*) temp_page_table;
    *pte = 0;
    invalidate_page(page);
}

u32 get_page_dir_entry(page_frame page_dir, virt_addr addr)
{
    virt_page virt_dir = map_temp_page(page_dir);
    u32* directory = (u32*) virt_dir.start.value;
    u32 res = directory[PAGE_DIR_INDEX(addr)];
    unmap_temp_page(virt_dir);
    return res;
}

u32 get_page_table_entry(page_frame page_table, virt_addr addr)
{
    virt_page virt_table = map_temp_page(page_table);
    u32* table = (u32*) virt_table.start.value;
    u32 res = table[PAGE_TABLE_INDEX(addr)];
    unmap_temp_page(virt_table);
    return res;
}

void set_page_dir_entry(page_frame page_dir, virt_addr addr, u32 value)
{
    virt_page virt_dir = map_temp_page(page_dir);
    if (is_virtual_page_invalid(virt_dir))
        return;
    u32* directory = (u32*) virt_dir.start.value;
    directory[PAGE_DIR_INDEX(addr)] = value;
    unmap_temp_page(virt_dir);
}

void set_page_table_entry(page_frame page_table, virt_addr addr, u32 value)
{
    virt_page virt_table = map_temp_page(page_table);
    if (is_virtual_page_invalid(virt_table))
        return;
    u32* table = (u32*) virt_table.start.value;
    table[PAGE_TABLE_INDEX(addr)] = value;
    unmap_temp_page(virt_table);
}

phys_addr resolve_addr(page_frame page_dir, virt_addr addr)
{
    u32 pde = get_page_dir_entry(page_dir, addr);
    if (!(pde & PAGE_PRESENT))
        return invalid_phys_addr();
    if (pde & PAGE_SIZE_4MB)
        return (phys_addr) { (pde & 0xFFC00000) | (addr.value & 0x003FFFFF) };

    page_frame page_table = { { pde & PAGE_ADDR_MASK } };
    u32 pte = get_page_table_entry(page_table, addr);
    if (!(pte & PAGE_PRESENT))
        return invalid_phys_addr();
    return (phys_addr) { (pte & PAGE_ADDR_MASK) | PAGE_OFFSET(addr) };
}

void zero_out_page_frame(page_frame frame)
{
    virt_page page = map_temp_page(frame);
    u32* array = (u32*) page.start.value;
    for (u32 i = 0; i < PAGE_SIZE / sizeof(u32); i++)
        array[i] = 0;
    unmap_temp_page(page);
}

page_frame map_page_frame(page_frame target_pd, virt_page page)
{
    u32 pde = get_page_dir_entry(target_pd, page.start);
    if (pde & PAGE_SIZE_4MB)
    {
        return (page_frame) { resolve_addr(target_pd, page.start) };
    }

    page_frame frame = alloc_page();
    if (is_page_frame_invalid(frame))
        return invalid_page_frame();

    page_frame page_table = invalid_page_frame();
    if (!(pde & PAGE_PRESENT))
    {
        page_table = alloc_page();
        if(is_page_frame_invalid(page_table))
        {
            free_page(frame);
            return invalid_page_frame();
        }
        zero_out_page_frame(page_table);
        set_page_dir_entry(target_pd, page.start, page_table.start.value | PAGE_FLAGS);
    }
    else
    {
        page_table = (page_frame) { { pde & PAGE_ADDR_MASK } };
    }

    u32 pte = get_page_table_entry(page_table, page.start);
    if (pte & PAGE_PRESENT)
    {
        free_page(frame);
        return invalid_page_frame();
    }

    set_page_table_entry(page_table, page.start, page.start.value | PAGE_FLAGS);
    invalidate_page(page);
    return frame;
}

void init_page_frame_mapper(page_frame kernel_pd)
{
    u32* directory = (u32*) phys_to_virt_direct(kernel_pd.start).value;
    phys_addr temp_table_addr = { ((u32) temp_page_table) - KERNEL_OFFSET };
    directory[TEMP_PAGE_DIR_INDEX] = temp_table_addr.value | PAGE_FLAGS;
}