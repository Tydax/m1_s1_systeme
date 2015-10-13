#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <ctx.h>
#include <sem.h>
#include <object.h>

static struct sem_s mutex, /* Access token to the buffer */
                    empty, /* Number of free slots */
                    full; /* Number of taken slots */

void producer() {
    object_t object;

    while(1) {
        object = produce_object();
        printf("Object n°%d produced\n", object);
        sem_down(&empty);
        sem_down(&mutex);
        store_object(object);
        sem_up(&mutex);
        sem_up(&full);
    }
}

void consumer() {
    object_t object;

    while(1) {
        sem_down(&full);
        sem_down(&mutex);
        object = take_object();
        sem_up(&mutex);
        sem_up(&empty);
        use_object(object);
    }
}

int main(int argc, char const *argv[])
{
    /* Semaphores initialisation */
    sem_init(&mutex, 1);
    sem_init(&empty, MAX);
    sem_init(&full, 0);

    /* Contexts initialisations */

    if(create_ctx(16384, producer, NULL)
       || create_ctx(16384, consumer, NULL)) {
        fprintf(stderr, "ERROR: Couldn't create contexts.\n");
        exit(EXIT_FAILURE);
    }

    start_sched();

    exit(EXIT_SUCCESS);
}