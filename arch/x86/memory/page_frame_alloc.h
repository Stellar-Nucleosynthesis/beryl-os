#ifndef PAGE_FRAME_ALLOC_H
#define PAGE_FRAME_ALLOC_H

#include <types/sysint.h>

#define PAGE_SIZE   0x1000
#define PAGE_COUNT  0x100000

typedef struct
{
    phys_addr addr;
} page_frame;

static void mark_used_page(page_frame page);

static page_frame alloc_page();

static void free_page(page_frame page);

#endif