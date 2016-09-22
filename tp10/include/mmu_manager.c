#include <assert.h>

#include <hardware.h>
#include <kernel.h>
#include <swap.h>


void mmuhandler_base() {
    static unsigned vpage_current;

    unsigned addr;
    union tlb_entry_u tlb;

    addr = _in(MMU_FAULT_ADDR);
    assert(virtual_addr <= addr && virtual_memory + VM_SIZE > addr);

    /* Save page to swap */
    store_to_swap(vpage_current, 1);
    tlb.tlb_s.tlb_vpage = vpage_current;
    tlb.tlb_s.tlb_ppage = 1;
    _out(TLB_DEL_ENTRY, tlb.tlb_i);

    /* Fetch swap page and put back in memory */
    vpage_current = VPAGE(addr);
    fetch_from_swap(vpage_current, 1);
    tlb.tlb_s.tlb_vpage = vpage_current;
    tlb.tlb_s.tlb_ppage = 1;
    tlb.tlb_s.tlb_exec = 1;
    tlb.tlb_s.tlb_write = 1;
    tlb.tlb_s.tlb_read = 1;
    tlb.tlb_s.tlb_used = 1;
    _out(TLB_ADD_ENTRY, tlb.tlb_i);
}


void mmuhandler_dbl_mapping() {
    static struct vmm_s vm_mapping[VM_PAGES];
    static struct pmm_s pm_mapping[PM_PAGES];
    static int next = 1;

    unsigned addr;
    unsigned vpage;

    union tlb_entry_u tlb;

    addr = _in(MMU_FAULT_ADDR);
    assert(virtual_addr <= addr && virtual_memory + VM_SIZE > addr);
    vpage = VPAGE(addr);

    if (vm_mapping(vpage).mapped) {
        tlb.tlb_s.tlb_vpage = vpage;
        tlb.tlb_s.tlb_ppage = vm_mapping[vpage].ppage;
        tlb.tlb_s.tlb_exec = 1;
        tlb.tlb_s.tlb_write = 1;
        tlb.tlb_s.tlb_read = 1;
        tlb.tlb_s.tlb_used = 1;
        _out(TLB_ADD_ENTRY, tlb.tlb_i);

        return ;
    }

    if (pm_mapping[next].mapped) {
        store_to_swap(pm_mapping[next].vpage, next);
        pm_mapping[next].mapped = 0;
        vm_mapping[pm_mapping[next].vpage].mapped = 0;
    }

    fetch_from_swap(vpage, next);
    pm_mapping[next].vpage = vpage;
    pm_mapping[next].mapped = 1;

    vm_mapping[vpage].ppage = next;
    vm_mapping[vpage].mapped = 1;

    tlb.tlb_s.tlb_vpage = vpage;
    tlb.tlb_s.tlb_ppage = next;
    tlb.tlb_s.tlb_exec = 1;
    tlb.tlb_s.tlb_write = 1;
    tlb.tlb_s.tlb_read = 1;
    tlb.tlb_s.tlb_used = 1;
    _out(TLB_ADD_ENTRY, tlb.tlb_i);

    next = (next % (PM_PAGES - 1)) +1;
}