#include <stdlib.h>
#include <stdio.h>

#include <ctx.h>

void f_ping(void * arg);
void f_pong(void * arg);
void f_pung(void * arg);

int main(int argc, char ** argv) {
    /* Create contexts */
    if(create_ctx(16384, f_ping, NULL)
       || create_ctx(16384, f_pong, NULL)
       || create_ctx(16384, f_pung, NULL)) {
        fprintf(stderr, "ERROR: Couldn't create contexts.\n");
        exit(EXIT_FAILURE);
    }

    yield();

    exit(EXIT_SUCCESS);
}

void f_ping(void * args) {
    int i;
    for (i = 0; i < 5; ++i) {
        printf("A");
        yield();

        printf("B");
        yield();

        printf("C");
        yield();
    }
}

void f_pong(void * args) {
    int i;
    for (i = 0; i < 3; ++i) {
        printf("1");
        yield();

        printf("2");
        yield();

        printf("3");
        yield();
    }
}

void f_pung(void * args) {
    int i;
    for (i = 0; i < 7; ++i) {
        printf("!");
        yield();

        printf("@");
        yield();

        printf("#");
        yield();
    }
}