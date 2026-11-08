#ifndef PAGE_FRAME_ALLOC_H
#define PAGE_FRAME_ALLOC_H

#include <paging/paging.h>

void mark_used_page(page_frame page);

page_frame alloc_page();

void free_page(page_frame page);

#endif