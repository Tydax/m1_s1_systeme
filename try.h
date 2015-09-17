/*
 * try.h
 *
 * Library used to save and restore function contexts.
 *
 * Authors: Armand BOUR, Tristan CAMUS
 *
 */

/*
 * Magic value used to make sure the structure has been initialized before trying
 * to restore a context.
 */
#define MAGIC_VALUE 0xB16B00B5

/* Struct describing a saved context. */
struct ctx_s {
    void * ctx_ebp;             /* Saved extended base pointer */
    void * ctx_esp;             /* Saved extended stack pointer */ 
    int    ctx_magic;     /* Détrompeur */
};

/* 
 * Saves the context inside the specified struct, and calls the specified function
 * ctx_s: the struct where to save the context
 * f: the functon to call
 * arg: the argument
 */
int try(struct ctx_s * pctx, func_t * f, int arg);


/*
 * Restores the context saved into the struct.
 */
int throw(struct ctx_s * pctx, int r);