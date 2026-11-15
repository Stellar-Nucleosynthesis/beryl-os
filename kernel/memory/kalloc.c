#include <memory/kalloc.h>
#include <memory/alloc_memory.h>
#include <types/numeric.h>
#include <types/bool.h>

#define KALLOC_ALIGNMENT 8
#define ALIGN_UP(value) (((value) + KALLOC_ALIGNMENT - 1) & ~(KALLOC_ALIGNMENT - 1))

typedef struct memory_chunk memory_chunk;
typedef struct memory_space memory_space;

struct memory_chunk
{
    int size;
    int free;
    memory_chunk* next;
};

struct memory_space
{
    memory_block backing;
    memory_chunk* first;
    memory_space* next;
};

memory_space* memory_spaces = NULL;

int chunk_header_size()
{
    return ALIGN_UP(sizeof(memory_chunk));
}

int space_header_size()
{
    return ALIGN_UP(sizeof(memory_space));
}

memory_space* create_memory_space(int min_size)
{
    int overhead = chunk_header_size() + space_header_size();
    memory_block backing = alloc_mem_block(min_size + overhead);
    if (is_mem_block_invalid(backing))
        return NULL;
    memory_space* space = (memory_space*) backing.ptr;

    space->backing = backing;
    space->first = (memory_chunk*) (((char*) backing.ptr) + space_header_size());
    space->first->size = backing.size - overhead;
    space->first->free = true;
    space->first->next = NULL;

    space->next = memory_spaces;
    memory_spaces = space;
    return space;
}

void merge_chunks(memory_space* space)
{
    memory_chunk* chunk = space->first;
    while (chunk && chunk->next)
    {
        memory_chunk* next = chunk->next;
        if (chunk->free && next->free)
        {
            chunk->size += chunk_header_size() + next->size;
            chunk->next = next->next;
        }
        else
        {
            chunk = next;
        }
    }
}

bool is_space_empty(memory_space* space)
{
    return space->first->free && space->first->next == NULL;
}

void* kalloc(int size)
{
    if (size <= 0)
        return NULL;

    size = ALIGN_UP(size);
    while(true)
    {
        memory_space* space = memory_spaces;
        while (space)
        {
            memory_chunk* chunk = space->first;
            while (chunk)
            {
                if (chunk->free && chunk->size >= size)
                {
                    int remaining = chunk->size - size;
                    if (remaining >= chunk_header_size() + KALLOC_ALIGNMENT)
                    {
                        memory_chunk* split = (memory_chunk*) ((char*) chunk + chunk_header_size() + size);
                        split->size = remaining - chunk_header_size();
                        split->free = true;
                        split->next = chunk->next;

                        chunk->next = split;
                        chunk->size = size;
                    }

                    chunk->free = false;
                    return (char*) chunk + chunk_header_size();
                }
                chunk = chunk->next;
            }
            space = space->next;
        }

        if (!create_memory_space(size))
            return NULL;
    }
}

void free(void* ptr)
{
    if (!ptr)
        return;

    memory_space* space = memory_spaces;
    while (space)
    {
        memory_chunk* chunk = space->first;
        while (chunk)
        {
            void* chunk_ptr = (char*) chunk + chunk_header_size();
            if (chunk_ptr == ptr)
            {
                if (chunk->free)
                    return;
                chunk->free = true;
                merge_chunks(space);

                if (is_space_empty(space))
                {
                    memory_space** link = &memory_spaces;
                    while (*link && *link != space)
                        link = &(*link)->next;

                    if (*link)
                    {
                        memory_block backing = space->backing;
                        *link = space->next;
                        free_mem_block(backing);
                    }
                }
                return;
            }
            chunk = chunk->next;
        }
        space = space->next;
    }
}