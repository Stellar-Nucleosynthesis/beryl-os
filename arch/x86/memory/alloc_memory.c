#include <memory/alloc_memory.h>
#include <memory/virt_page_alloc.h>
#include <memory/page_frame_mapper.h>
#include <error/panic.h>

page_frame kernel_pd;

memory_block alloc_mem_block(int min_size)
{
    if (min_size <= 0)
        return invalid_mem_block();
    int num_pages = (min_size + PAGE_SIZE - 1) / PAGE_SIZE;
    virt_page_block pages = alloc_virt_page_block(num_pages);
    if (is_virt_page_block_invalid(pages))
        return invalid_mem_block();

    for (int i = 0; i < num_pages; i++)
    {
        virt_page page = { { pages.start.value + i * PAGE_SIZE } };
        page_frame frame = map_page_frame(kernel_pd, page);
        if (is_page_frame_invalid(frame))
        {
            for (int j = 0; j < i; j++)
            {
                virt_page mapped = { { pages.start.value + j * PAGE_SIZE } };
                status unmap_res = unmap_page_frame(kernel_pd, mapped);
                if (unmap_res != STATUS_SUCCESS)
                    panic(UNSUCCESSFUL_MEMORY_UNMAP);
            }
            free_virt_page_block(pages);
            return invalid_mem_block();
        }
    }
    return (memory_block) { (void*) pages.start.value, num_pages * PAGE_SIZE };
}

void free_mem_block(memory_block memory)
{
    if (is_mem_block_invalid(memory))
        return;
    int num_pages = memory.size / PAGE_SIZE;
    virt_addr start = virt_addr_create(memory.ptr);
    for (int i = 0; i < num_pages; i++)
    {
        virt_page page = { { start.value + i * PAGE_SIZE } };
        status unmap_res = unmap_page_frame(kernel_pd, page);
        if (unmap_res != STATUS_SUCCESS)
            panic(UNSUCCESSFUL_MEMORY_UNMAP);
    }
    free_virt_page_block((virt_page_block) { start, num_pages });
}

memory_block invalid_mem_block()
{
    return (memory_block) { NULL, 0 };
}

bool is_mem_block_invalid(memory_block memory)
{
    return memory.ptr == NULL;
}

void init_memory_alloc()
{
    kernel_pd = get_page_dir();
}