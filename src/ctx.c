#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <ctx.h>
#include <hw.h>

/* Loop of contexts */
static struct ctx_s * loop = NULL;
/* Current context stored as a global variable */
static struct ctx_s * ctx_current = NULL;

/*
 * Allocates and initialises a new context in the specified pointer ctx, and initialises it (calls init_ctx).
 * stack_size: the size of the stack to allocate
 * f: the function associated with that context
 * args: the argument of the function
 */
int create_ctx(int stack_size, func_t f, void * args) {
    struct ctx_s * ctx_ptr;

    /* Allocate the new context*/
    ctx_ptr = (struct ctx_s *) malloc(sizeof(struct ctx_s));
    if (ctx_ptr == NULL) {
        return -1;
    }

    /* Initialise it */
    if (init_ctx(ctx_ptr, stack_size, f, args)) {
        /* Exit if failed to init */
        return -2;
    }

    /* Add the new context to the loop of contexts */
    if (loop != NULL) {
        ctx_ptr->ctx_next = loop->ctx_next;
        loop->ctx_next = ctx_ptr;
    } else {
        ctx_ptr->ctx_next = ctx_ptr;
        loop = ctx_ptr;
    }

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
int init_ctx(struct ctx_s * ctx, int stack_size, func_t f, void * args) {
    ctx->ctx_stack = malloc(stack_size);

    if (ctx->ctx_stack == NULL) {
        /* Exit if failed to allocate memory */
        return -1;
    }

    /* Since the stack iterates downwards, we have to store the "end" of the allocated memory
     in the stack pointers, minus the necessary execution space for the cpu */
    ctx->ctx_esp = 
        ctx->ctx_ebp = ctx->ctx_stack + stack_size - sizeof(void *);
    ctx->ctx_magic = MAGIC_VALUE;
    ctx->ctx_state = INITIALIZED;
    ctx->ctx_f = f;
    ctx->ctx_f_args = args;

    return 0;
}

/*
 * Switches to the next context in the loop. Calls switch_to.
 */
void yield() {
    struct ctx_s * tmp;

    if (ctx_current == NULL) {
        switch_to(loop);
    } else {
        /* Free the terminated contexts */
        while (ctx_current->ctx_next->ctx_state == TERMINATED && ctx_current->ctx_next != ctx_current) {
            free(ctx_current->ctx_next->ctx_stack);
            tmp = ctx_current->ctx_next->ctx_next;
            free(ctx_current->ctx_next);
            assert(tmp != NULL);
            ctx_current->ctx_next = tmp;
        }

        /* If no context are left, exit */
        if (ctx_current->ctx_next->ctx_state == TERMINATED) {
            exit(EXIT_SUCCESS);
        }
        switch_to(ctx_current->ctx_next);
    }
}

/*
 * Switches to the specified context, saving the previous context in a global variable.
 * ctx: the context to switch to
 */
void switch_to(struct ctx_s * ctx) {
    /* Execute this code without any interruption */
    irq_disable();

    /* If leaving a context, saving it in the global variable */
    if (ctx_current != NULL) {
        asm("movl %%ebp, %0"
            "\n\t"
            "movl %%esp, %1"
          : "=r" (ctx_current->ctx_ebp),
            "=r" (ctx_current->ctx_esp));
    }

    /* Switching the pointed saved context to the specified one*/
    ctx_current = ctx;

    /* Restoring the context */
    asm("movl %0, %%ebp"
        "\n\t"
        "movl %1, %%esp"
     :: "r" (ctx_current->ctx_ebp),
        "r" (ctx_current->ctx_esp));

    irq_enable();

    /* If switching to the context for the first time, call the function stored in the context */
    if (ctx_current->ctx_state == INITIALIZED) {
        ctx_current->ctx_state = ACTIVABLE;
        /* Call the function */
        ctx_current->ctx_f(ctx_current->ctx_f_args);
        /* Change the state */
        ctx_current->ctx_state = TERMINATED;
    }

}

/*
 * Sets up the interruption manager. Uses the hw library.
 */
void start_sched() {
    start_hw();
    setup_irq(TIMER_IRQ, yield);
    yield();
}