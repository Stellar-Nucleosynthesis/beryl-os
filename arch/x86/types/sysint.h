#ifndef SYSINT_H
#define SYSINT_H

#include <types/sysbool.h>

#define NULL 0

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef signed char         i8;
typedef signed short        i16;
typedef signed int          i32;
typedef signed long long    i64;

typedef struct
{
    u32 value;
} virt_addr;

typedef struct
{
    u32 value;
} phys_addr;

static inline virt_addr virt_addr_create(void* value)
{
    return (virt_addr){ (u32) value };
}

static inline virt_addr invalid_virt_addr()
{
    return (virt_addr) { NULL };
}

static inline bool is_virt_addr_invalid(virt_addr addr)
{
    return addr.value == NULL;
}

static inline phys_addr invalid_phys_addr()
{
    return (phys_addr) { NULL };
}

static inline bool is_phys_addr_invalid(phys_addr addr)
{
    return addr.value == NULL;
}

#endif