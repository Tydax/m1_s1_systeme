#define MMU_IRQ         13
#define MMU_FAULT_ADDR  0xCD
#define N               254

/* Structure used to store the TLB entries */
union tlb_entry_u {
    int tlb;
    struct tlb_entry_s {
        unsigned tlb_rfu        : 8;
        unsigned tlb_virpage    : 12;
        unsigned tlb_physpage   : 8;
        unsigned tlb_exec       : 1;
        unsigned tlb_write      : 1;
        unsigned tlb_read       : 1;
        unsigned tlb_used       : 1;
    } tlb_s;
}

static int current_process;

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

    ... /* init_hardware(); */
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
    printf("Resultat processus 1 : %d\n",res);
}
