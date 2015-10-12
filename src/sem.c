#include <assert.h>

#include <sem.h>
#include <ctx.h>

/*
 * Initialises the specified semaphore with the value.
 */
void sem_init(struct sem_s * sem, unsigned int val) {
    sem->sem_blocked = NULL;
    sem->sem_counter = val;
    sem->sem_magic = SEM_MAGIC_VALUE;
}

/*
 * Decrements the semaphore.
 */
void sem_down(struct sem_s * sem) {
    assert(sem->sem_magic == SEM_MAGIC_VALUE);

    sem->sem_counter--;

    /* Block the current context and update the list of blocked contexts */
    if (sem->sem_counter < 0) {
        ctx_current->ctx_state = BLOCKED;
        ctx_current->ctx_sem_next = sem->sem_blocked;
        sem->sem_blocked = ctx_current;
        yield();
    }
}

/*
 * Increments the semaphore.
 */
void sem_up(struct sem_s * sem) {
    assert(sem->sem_magic == SEM_MAGIC_VALUE);

    sem->sem_counter++;

    /* Unblock the current context if there is any */
    if (sem->sem_counter >= 0) {
        if (sem->sem_blocked != NULL) {
            sem->sem_blocked->ctx_state = ACTIVABLE;
            sem->sem_blocked = sem->sem_blocked->ctx_sem_next;
        }
    }
}