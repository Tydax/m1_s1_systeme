#ifndef __TRY_H__
#define __TRY_H__

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

typedef int (func_t)(int);

/* Struct describing a saved context. */
struct ctx_s {
    void * ctx_ebp;             /* Saved extended base pointer */
    void * ctx_esp;             /* Saved extended stack pointer */ 
    int    ctx_magic;     /* Détrompeur */
};

/* 
 * Saves the context inside the specified struct, and calls the specified function
 * ctx_s: the struct where to save the context
 * f: the function to call
 * arg: the argument to specify to the function
 */
int try(struct ctx_s * pctx, func_t * f, int arg);


/*
 * Restores the context saved into the struct.
 * ctx_s: the struct where the context to restore is saved
 * r: the value to return once the context is restored
 */
int throw(struct ctx_s * pctx, int r);

#endif