#include <memory/kalloc.h>

#define FB_WIDTH  80
#define FB_HEIGHT 25

typedef unsigned char       u8;
typedef unsigned int        u32;

static void fb_print_hex(u32 value)
{
    static const char hex[] = "0123456789ABCDEF";
    static u32 row = 0;

    volatile u8* fb = (volatile u8*) 0xC00B8000;

    for (u32 i = 0; i < 8; i++)
    {
        u32 shift = (7 - i) * 4;
        fb[row * FB_WIDTH * 2 + i * 2] = hex[(value >> shift) & 0xF];
        fb[row * FB_WIDTH * 2 + i * 2 + 1] = 0x0B;
    }

    row = (row + 1) % FB_HEIGHT;
}

void kmain()
{
    int* ptr = kalloc(sizeof(int));
    fb_print_hex((u32) ptr);
    *ptr = 0x7;
    free(ptr);

    while (1) {}
}