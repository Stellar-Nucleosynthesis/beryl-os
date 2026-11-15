#ifndef PANIC_H
#define PANIC_H

typedef enum
{
    UNSUCCESSFUL_MEMORY_UNMAP
} panic_reason;

void panic(panic_reason reason);

#endif