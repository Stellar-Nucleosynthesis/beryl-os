#ifndef ALLOC_MEMORY_H
#define ALLOC_MEMORY_H

#include <types/numeric.h>
#include <types/bool.h>

typedef struct
{
    void* ptr;
    int size;
} memory_block;

memory_block alloc_mem_block(int min_size);

void free_mem_block(memory_block memory);

memory_block invalid_mem_block();

bool is_mem_block_invalid(memory_block memory);

void init_memory_alloc();

#endif