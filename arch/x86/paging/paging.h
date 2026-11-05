#ifndef PAGING_H
#define PAGING_H

#include <types/sysint.h>

phys_addr get_page_dir_base();

void set_page_dir_base(phys_addr page_dir);

virt_addr phys_to_virt_direct(phys_addr addr);

phys_addr resolve_addr(phys_addr page_dir_base, virt_addr addr);

#endif