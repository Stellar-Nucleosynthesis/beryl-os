#ifndef SYSINT_H
#define SYSINT_H

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
    return (virt_addr){ .value = (u32) value };
}

#endif