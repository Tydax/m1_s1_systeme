#include <assert.h>
#include <stdlib.h>

#include <ctx.h>

/*
 * Allocates and initialises a new context in the specified pointer ctx, and initialises it (calls init_ctx).
 * stack_size: the size of the stack to allocate
 * f: the function associated with that context
 * args: the argument of the function
 */
int create_ctx(int stack_size, func_t f, void * args) {
    struct ctx_s * ctx_ptr,
                 * next_ctr;

    /* Allocate the new context*/
    ctx_ptr = (struct ctx_s *) malloc(sizeof(struct ctx_s));
    if (ctx_s == NULL) {
        return -1;
    }

    /* Initialise it */
    if (!init_ctx(ctx_ptr, stack_size, f, args)) {
        /* Exit if failed to init */
        return -2;
    }

    /* Add the new context to the loop of contexts */
    if (loop != NULL) {
        ctx_ptr->ctx_next = loop->ctx_next;
    } else {
        loop = ctx_ptr;
    }

    loop->ctx_next = ctx_ptr;

    return 0;
}

/*
 * Initialises a new context in the specified pointer ctx, allocating a stack
 * of the specified size.
 * ctx: the pointer to the context structure to initialise
 * stack_size: the size of the stack to allocate
 * f: the function associated with that context
 * args: the argument of the function
 */
static int init_ctx(struct ctx_s * ctx, int stack_size, func_t f, void * args) {
    char * stack = malloc(stack_size);

    if (stack == NULL) {
        /* Exit if failed to allocate memory */
        return -1;
    }

    /* Since the stack iterates downwards, we have to store the "end" of the allocated memory
     in the stack pointers, minus the necessary execution space for the cpu */
    ctx->ctx_esp = 
        ctx->ctx_ebp = stack + stack_size - sizeof(void *);
    ctx->ctx_magic = MAGIC_VALUE;
    ctx->ctx_state = INITIALIZED;
    ctx->ctx_f = f;
    ctx->ctx_f_args = args;

    return 0;
}

/*
 * Switches to the next context in the loop. Calls switch_to_ctx.
 */
void yield() {
    struct ctx_s * tmp;

    if (!ctx_crt) {
        switch_to(loop);
    } else {
        /* Free the terminated contexts */
        while (ctx_crt->ctx_next->ctx_state == TERMINATED && ctx_ctr->ctx_next != ctx_crt) {
            free(ctx_ctr->ctx_next->ctx_stack);
            tmp = ctx_ctr->ctx_next->ctx_next;
            free(ctx_ctr->ctx_next);
            ctx_crt->ctx_next = tmp;
        }

        if (ctx_crt->ctx_next->ctx_state == TERMINATED) {
            exit(0);
        } else {
            switch_to(ctx_crt->ctx_next);
        }
    }
}

/*
 * Switches to the specified context, saving the previous context in a global variable.
 * ctx: the context to switch to
 */
static void switch_to_ctx(struct ctx_s * ctx) {}
    assert(ctx);
    assert(ctx->ctx_magic == MAGIC_VALUE);
    assert(ctx->ctx_state == INITIALIZED || ctx->ctx_state == ACTIVABLE);

    /* If leaving a context, saving it in the global variable */
    if (ctx_crt != NULL) {
        asm("mov %%ebp, %0"
            "\n\t"
            "mov %%esp, %1"
          : "=r" (ctx_crt->ctx_ebp),
            "=r" (ctx_crt->ctx_esp));
    }

    /* Switching the pointed saved context to the specified one*/
    ctx_crt = ctx;

    /* Restoring the context */
    asm("mov %0, %%ebp"
        "\n\t"
        "mov %1, %%esp"
      :
      : "r" (ctx_crt->ctx_ebp),
        "r" (ctx_crt->ctx_esp));

    /* If switching to the context for the first time, call the function stored in the context */
    if (ctx_crt->ctx_state == INITIALIZED) {
        ctx_crt->ctx_state = ACTIVABLE;
        /* Call the function */
        ctx_crt->ctx_f(ctx_crt->ctx_f_args);
        /* Change the state */
        ctx_crt->ctx_state = TERMINATED;
    }
}