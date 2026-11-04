#include <kmain.h>
#include <gdt/gdt.h>

void setup()
{
    setup_gdt();

    unsigned char *fb = (unsigned char *) 0xB8000;
    fb[0] = 'A';
    fb[1] = 0x0E;

    kmain();
}