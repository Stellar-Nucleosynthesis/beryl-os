#ifndef PAGE_FRAME_MAPPER_H
#define PAGE_FRAME_MAPPER_H

#include <paging/paging.h>

virt_page map_temp_page(page_frame page);

void unmap_temp_page(virt_page page);

u32 get_page_dir_entry(page_frame page_dir, virt_addr addr);

u32 get_page_table_entry(page_frame page_table, virt_addr addr);

void set_page_dir_entry(page_frame page_dir, virt_addr addr, u32 value);

void set_page_table_entry(page_frame page_table, virt_addr addr, u32 value);

phys_addr resolve_addr(page_frame page_dir_base, virt_addr addr);

page_frame map_page_frame(page_frame target_pd, virt_page page);

void init_page_frame_mapper(page_frame kernel_pd);

#endif