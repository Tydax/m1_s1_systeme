#ifndef __CTX_H__
#define __CTX_H__

/*
 * ctx.h
 *
 * Library defining a loop of saved contexts and functions to switch to them.
 *
 * Authors: Armand BOUR, Tristan CAMUS
 *
 */

/* Loop of contexts */
static struct ctx_s * loop = NULL;
/* Current context stored as a global variable */
static struct ctx_s * ctx_crt = NULL;

/*
 * Magic value used to make sure the structure has been initialized before trying
 * to switch to that context.
 */
#define MAGIC_VALUE 0xDEADCACA

/* Function type, taking a pointer to the args as a parameter */
typedef void func_t(void *);

/* Enum describing the different states of the context */
enum ctx_state_e {
    INITIALIZED,
    ACTIVABLE,
    TERMINATED
};

/* Structure describing a saved context. */
struct ctx_s {
    void *              ctx_esp;
    void *              ctx_ebp;
    int                 ctx_magic;
    enum ctx_state_e    ctx_state;
    func_t *            ctx_f;
    void *              ctx_f_args;
    struct ctx_s *      ctx_next;
};


/*
 * Allocates and initialises a new context in the specified pointer ctx, and initialises it (calls init_ctx).
 * stack_size: the size of the stack to allocate
 * f: the function associated with that context
 * args: the argument of the function
 */
int create_ctx(int stack_size, func_t f, void * args);

/*
 * Initialises a new context in the specified pointer ctx, allocating a stack
 * of the specified size.
 * ctx: the pointer to the context structure to initialise
 * stack_size: the size of the stack to allocate
 * f: the function associated with that context
 * args: the argument of the function
 */
static int init_ctx(struct ctx_s * ctx, int stack_size, func_t f, void * args);

/*
 * Switches to the next context in the loop. Calls switch_to_ctx.
 */
void yield();

/*
 * Switches to the specified context, saving the previous context in a global variable.
 * ctx: the context to switch to
 */
static void switch_to_ctx(struct ctx_s * ctx);


#endif