#include <gdt/gdt.h>
#include <paging/paging.h>

const u32 NULL_DESR_OFFSET = 0x0;
const gdt_entry NULL_DESCR = {
    .base           = { 0x0 },
    .limit          = 0x0,
    .access_byte    = 0x0,
    .flags          = 0x0
};

const u32 KERNEL_CODE_OFFSET = 0x8;
const gdt_entry KERNEL_CODE = {
    .base           = { 0x0 },
    .limit          = 0xFFFFF,
    .access_byte    = 0x9A,
    .flags          = 0xC
};

const u32 KERNEL_DATA_OFFSET = 0x10;
const gdt_entry KERNEL_DATA = {
    .base           = { 0x0 },
    .limit          = 0xFFFFF,
    .access_byte    = 0x92,
    .flags          = 0xC
};

const u32 USER_CODE_OFFSET = 0x18;
const gdt_entry USER_CODE = {
    .base           = { 0x0 },
    .limit          = 0xFFFFF,
    .access_byte    = 0xFA,
    .flags          = 0xC
};

const u32 USER_DATA_OFFSET = 0x20;
const gdt_entry USER_DATA = {
    .base           = { 0x0 },
    .limit          = 0xFFFFF,
    .access_byte    = 0xF2,
    .flags          = 0xC
};

void encode_gdt_entry(u8* target, gdt_entry source)
{
    if (source.limit > 0xFFFFF)
        source.limit = 0xFFFFF;
    
    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6] = (source.limit >> 16) & 0x0F;
    
    target[2] = source.base.value & 0xFF;
    target[3] = (source.base.value >> 8) & 0xFF;
    target[4] = (source.base.value >> 16) & 0xFF;
    target[7] = (source.base.value >> 24) & 0xFF;
    
    target[5] = source.access_byte;
    
    target[6] |= (source.flags << 4);
}

static u8 GDT[5 * 8];

void setup_gdt()
{
    encode_gdt_entry(GDT + NULL_DESR_OFFSET, NULL_DESCR);
    encode_gdt_entry(GDT + KERNEL_CODE_OFFSET, KERNEL_CODE);
    encode_gdt_entry(GDT + KERNEL_DATA_OFFSET, KERNEL_DATA);
    encode_gdt_entry(GDT + USER_CODE_OFFSET, USER_CODE);
    encode_gdt_entry(GDT + USER_DATA_OFFSET, USER_DATA);

    load_gdt(virt_addr_create(GDT), sizeof(GDT));
    load_data_segment(KERNEL_DATA_OFFSET);
    load_code_segment(KERNEL_CODE_OFFSET);
}