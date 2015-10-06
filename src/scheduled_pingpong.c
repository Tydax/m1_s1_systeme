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

    start_sched();

    exit(EXIT_SUCCESS);
}

void f_ping(void * args) {
    while(1) {
        printf("A");
        printf("B");
        printf("C");
    }
}

void f_pong(void * args) {
    while(1) {
        printf("1");
        printf("2");
        printf("3");
    }
}

void f_pung(void * args) {
    while(1) {
        printf("!");
        printf("@");
        printf("#");
    }
}