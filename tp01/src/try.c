#include <assert.h>

#include <try.h>

/* 
 * Saves the context inside the specified struct, and calls the specified function
 * ctx_s: the struct where to save the context
 * f: the function to call
 * arg: the argument to specify to the function
 */
int try(struct ctx_s * pctx, func_t * f, int arg) {
    /* Stores the ebp and esp values into the struc */
    asm("movl %%ebp, %0"
        "\n\t"
        "movl %%esp, %1"
      : "=r" (pctx->ctx_ebp),
        "=r" (pctx->ctx_esp));

    pctx->ctx_magic = MAGIC_VALUE;

    return f(arg);
}


/*
 * Restores the context saved into the struct.
 * ctx_s: the struct where the context to restore is saved
 * r: the value to return once the context is restored
 */
int throw(struct ctx_s * pctx, int r) {
    static int result;

    /* Checks whether the structure has been initialised or not */
    assert(pctx->ctx_magic == MAGIC_VALUE);
    result = r;

    /* Sets the context using the structure values */
    asm("movl %0, %%ebp"
    "\n\t"
    "movl %1, %%esp"
  :
  : "r" (pctx->ctx_ebp),
    "r" (pctx->ctx_esp));

    return result;
}