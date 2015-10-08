#ifndef __SEM_H__
#define __SEM_H__

/*
 * sem.h
 * 
 * Library defining a semaphore and the function associated.
 *
 */

#define SEM_MAGIC_VALUE 0xDABBAD00

/*
 * Structure defining a semaphore.
 */
struct sem_s {
    int             sem_counter;
    struct ctx_s *  sem_blocked;
    int             sem_magic;
};

/*
 * Initialises the specified semaphore with the value.
 */
void sem_init(struct sem_s * sem, unsigned int val);

/*
 * Decrements the semaphore.
 */
void sem_down(struct sem_s * sem);

/*
 * Increments the semaphore.
 */
void sem_up(struct sem_s * sem);

#endif