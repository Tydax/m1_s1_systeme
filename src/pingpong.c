#include <stdlib.h>
#include <stdio.h>

#include <ctx.h>

/* Stored contexts associated with the two functions */
struct ctx_s * ctx_ping;
struct ctx_s * ctx_pong;

void f_ping(void * arg);
void f_pong(void * arg);

int main(int argc, char ** argv) {
    /* Allocating memory for context pointers */
    ctx_ping = (struct ctx_s *) malloc(sizeof(struct ctx_s));
    ctx_pong = (struct ctx_s *) malloc(sizeof(struct ctx_s));

    if (ctx_ping == NULL || ctx_pong == NULL) {
        fprintf(stderr, "Couldn't initialise context pointers\n");
        exit(EXIT_FAILURE);
    }

    /* Initialising contexts */
    init_ctx(ctx_ping, 16384, f_ping, NULL);
    init_ctx(ctx_pong, 16384, f_pong, NULL);
    switch_to_ctx(ctx_ping);

    /* Let it goo, let it goooo */
    free(ctx_ping);
    free(ctx_pong);

    exit(EXIT_SUCCESS);
}

void f_ping(void * args) {
    while(1) {
        printf("A");
        switch_to_ctx(ctx_pong);
        
        printf("B");
        switch_to_ctx(ctx_pong);

        printf("C");
        switch_to_ctx(ctx_pong);
    }
}

void f_pong(void * args) {
    while(1) {
        printf("1");
        switch_to_ctx(ctx_ping);
        
        printf("2");
        switch_to_ctx(ctx_ping);

        printf("3");
        switch_to_ctx(ctx_ping);
    }
}