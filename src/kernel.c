#include <kernel.h>

#include <assert.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

#include <hardware.h>
#include <mi_syscall.h>
#include <mi_user.h>

int
ppage_of_vaddr(int process, unsigned vaddr)
{
    int vpage;

    vpage = VPAGE(vaddr);

    if ( (int) virtual_memory > vaddr || (int) virtual_memory + VM_SIZE + 1 < vaddr
        || vpage >= N/2) {
        return -1;
    }

    return vpage + (N/2 * process) + 1;
}

void
mmuhandler()
{
    union tlb_entry_u tlb_entry;
    unsigned int vaddr, ppage;

    vaddr = _in(MMU_FAULT_ADDR);
    ppage = ppage_of_vaddr(current_process, vaddr);

    if (ppage == -1) {
        fprintf(stderr, "Erreur de segmentation sur ppage\n");
        exit(EXIT_FAILURE);
    }

    tlb_entry.tlb_s.tlb_virpage = VPAGE(vaddr);
    tlb_entry.tlb_s.tlb_physpage = ppage_of_vaddr(current_process, ppage);
    tlb_entry.tlb_s.tlb_exec = 1;
    tlb_entry.tlb_s.tlb_write = 1;
    tlb_entry.tlb_s.tlb_read = 1;
    tlb_entry.tlb_s.tlb_used = 1;

    _out(TLB_ADD_ENTRY, tlb_entry.tlb);
}