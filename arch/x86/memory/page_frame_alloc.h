#ifndef PAGE_FRAME_ALLOC_H
#define PAGE_FRAME_ALLOC_H

#include <paging/paging.h>

void mark_used_page_frame(page_frame page);

page_frame alloc_page_frame();

void free_page_frame(page_frame page);

void init_page_frame_alloc();

#endif