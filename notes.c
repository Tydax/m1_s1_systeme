struct sem_s {
    int sem_counter;
    struct ctx_s * sem_blocked;
    int sem_magic;
};

/* Structure describing a saved context. */
struct ctx_s {
    void *              ctx_esp;
    void *              ctx_ebp;
    void *              ctx_stack;
    int                 ctx_magic;
    enum ctx_state_e    ctx_state;
    func_t *            ctx_f;
    void *              ctx_f_args;
    struct ctx_s *      ctx_next;
    struct ctx_s *      ctx_sem_next;
};

/* Enum describing the different states of the context */
enum ctx_state_e {
    INITIALIZED,
    ACTIVABLE,
    TERMINATED,
    BLOCKED
};

void sem_init(struct sem_s * sem, unsigned int val) {
    sem->sem_blocked = NULL;
    sem->sem_counter = val;
    sem->sem_magic = 0xDEADCACA;
}

void sem_down(struct sem_s * sem) {
    cptr--
    if cptr < 0 {
        ctx_courant->state = BLOCKED
        ctx_courant->sem_next = sem->ctx_bloques
        sem->ctx_bloques = ctx_courant
        yield
    }
        
}

void sem_up(struct sem_s * sem) {
    cptr++;
    if cptr <= 0 {
        sem->ctx_bloques->state = ACTIVABLE;
        sem->ctx_bloques = sem->ctx_bloques->sem_next
    }
}