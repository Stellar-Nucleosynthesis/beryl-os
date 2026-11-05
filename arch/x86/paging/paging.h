#ifndef PAGING_H
#define PAGING_H

#include <types/sysint.h>

phys_addr get_page_dir_base();

void set_page_dir_base(phys_addr page_dir);

phys_addr resolve_addr(virt_addr addr);

#endif