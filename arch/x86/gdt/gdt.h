#ifndef GDT_H
#define GDT_H

#include <types/sysint.h>

typedef struct
{
    phys_addr base;
    u32 limit;
    u8 access_byte;
    u8 flags;
} gdt_entry;

void encode_gdt_entry(u8* target, gdt_entry source);

void load_gdt(phys_addr addr, u16 size);

void load_data_segment(u32 gdt_offset);

void load_code_segment(u32 gdt_offset);

void setup_gdt();

#endif