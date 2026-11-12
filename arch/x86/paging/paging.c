#include <paging/paging.h>
#include <memory/page_frame_alloc.h>
#include <types/bool.h>

virt_addr phys_to_virt_direct(phys_addr addr)
{
    return (virt_addr) { addr.value + KERNEL_OFFSET };
}

page_frame invalid_page_frame()
{
    return (page_frame) { invalid_phys_addr() };
}

bool is_page_frame_invalid(page_frame pf)
{
    return is_phys_addr_invalid(pf.start);
}

virt_page invalid_virt_page()
{
    return (virt_page) { invalid_virt_addr() };
}

bool is_virt_page_invalid(virt_page page)
{
    return is_virt_addr_invalid(page.start);
}

virt_page_block invalid_virt_page_block()
{
    return (virt_page_block) { invalid_virt_addr(), 0 };
}

bool is_virt_page_block_invalid(virt_page_block block)
{
    return is_virt_addr_invalid(block.start);
}