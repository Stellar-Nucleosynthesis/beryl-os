#include <error/panic.h>
#include <types/bool.h>

void panic(panic_reason reason)
{
    (void)reason;
    while (true) {}
}