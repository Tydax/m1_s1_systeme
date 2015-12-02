#include <mi_kernel.h>

#include <assert.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

#include <hardware.h>
#include <mi_syscall.h>
#include <mi_user.h>

static int current_process;

static int
vpage_of_vaddr(unsigned vaddr) {
    return (vaddr >> 12) & ((1 << 12) - 1);
}

static int
ppage_of_vaddr(int process, unsigned vaddr)
{
    int vpage;

    if ( (int) virtual_memory > vaddr || (int) virtual_memory + VM_SIZE + 1
        || vaddr == 0 || vaddr > N/2) {
        return -1;
    }

    vpage = vpage_of_vaddr(vaddr);
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

    tlb_entry.tlb_s.tlb_virpage = vpage_of_vaddr(vaddr);
    tlb_entry.tlb_s.tlb_physpage = ppage_of_vaddr(current_process, ppage);
    tlb_entry.tlb_s.tlb_exec = 1;
    tlb_entry.tlb_s.tlb_write = 1;
    tlb_entry.tlb_s.tlb_read = 1;
    tlb_entry.tlb_s.tlb_used = 1;

    _out(TLB_ADD_ENTRY, tlb_entry.tlb);
}

void 
switch_to_process0(void) 
{
    current_process = 0;
    _out(MMU_CMD, MMU_RESET);
}

void
switch_to_process1(void) 
{
    current_process = 1;
    _out(MMU_CMD, MMU_RESET);
}

int 
main(int argc, char **argv) 
{
    void *ptr;
    int res;

    assert(init_hardware("hardware.ini") != 0);
    IRQVECTOR[SYSCALL_SWTCH_0] = switch_to_process0;
    IRQVECTOR[SYSCALL_SWTCH_1] = switch_to_process1;
    _mask(0x1001);

    ptr = virtual_memory;

    _int(SYSCALL_SWTCH_0);
    memset(ptr, 1, PAGE_SIZE * N/2);
    _int(SYSCALL_SWTCH_1);
    memset(ptr, 3, PAGE_SIZE * N/2);

    _int(SYSCALL_SWTCH_0);
    res = sum(ptr);
    printf("Resultat du processus 0 : %d\n",res);
    _int(SYSCALL_SWTCH_1);
    res = sum(ptr);
    printf("Resultat du processus 1 : %d\n",res);
}
