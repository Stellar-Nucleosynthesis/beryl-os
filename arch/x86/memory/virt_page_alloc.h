#ifndef VIRT_PAGE_ALLOC_H
#define VIRT_PAGE_ALLOC_H

#include <paging/paging.h>

void mark_used_virt_page(virt_page page);

virt_page alloc_virt_page();

virt_page_block alloc_virt_page_block(u32 size);

void free_virt_page(virt_page page);

void free_virt_page_block(virt_page_block block);

void init_virt_page_alloc();

#endif