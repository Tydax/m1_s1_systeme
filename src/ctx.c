#include <assert.h>
#include <stdlib.h>

#include <ctx.h>

/*
 * Initialises a new context in the specified pointer ctx, allocating a stack
 * of the specified size.
 * ctx: the pointer to the context structure to initialise
 * stack_size: the size of the stack to allocate
 * f: the function associated with that context
 * args: the argument of the function
 */
int init_ctx(struct ctx_s * ctx, int stack_size, func_t f, void * args) {
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
 * Switch to the specified context, saving the previous context in a global variable.
 * ctx: the context to switch to
 */
void switch_to_ctx(struct ctx_s * ctx) {
    /* Current context stored as a global variable */
    static struct ctx_s * ctx_crt = NULL;

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

    /* If switching to the context for the first time, calling the function stored in the context */
    if (ctx_crt->ctx_state == INITIALIZED) {
        ctx_crt->ctx_state = ACTIVABLE;
        /* Calling the function */
        ctx_crt->ctx_f(ctx_crt->ctx_f_args);
        /* Terminating the program since if we get here,
           the context associated with the function doesn't exist anymore */
        assert(0);
    }
}