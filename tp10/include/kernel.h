#ifndef __KERNEL_H__
#define __KERNEL_H__

#define MMU_CMD         0x66
#define MMU_IRQ         13
#define MMU_FAULT_ADDR  0xCD
#define N               254

#define PAGE_SIZE 4096
#define PM_PAGES 256        
#define VM_PAGES 4096
#define PM_SIZE (PM_PAGES * PAGE_SIZE)
#define VM_SIZE (VM_PAGES * PAGE_SIZE)

#define TLB_SIZE        32
#define TLB_ADD_ENTRY   0xCE
#define TLB_DEL_ENTRY   0xDE
#define TLB_ENTRIES     0x800 

#define VPAGE(vaddr) ((vaddr >> 12) & 0xfff)

/* Structure used to store the TLB entries */
union tlb_entry_u {
    int tlb;
    struct tlb_entry_s {
        unsigned tlb_rfu        : 8;
        unsigned tlb_vpage      : 12;
        unsigned tlb_ppage      : 8;
        unsigned tlb_exec       : 1;
        unsigned tlb_write      : 1;
        unsigned tlb_read       : 1;
        unsigned tlb_used       : 1;
    } tlb_s;
};

int ppage_of_vaddr(int process, unsigned vaddr);

/* Structures used to map */
struct vmm_s {
    unsigned ppage  :8;
    unsigned mapped :1;
};

struct pmm_s {
    unsigned vpage  :12;
    unsigned mapped :1;
};

#endif
