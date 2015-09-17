#include <try.h>

/* 
 * Saves the context inside the specified struct, and calls the specified function
 * ctx_s: the struct where to save the context
 * f: the functon to call
 * arg: the argument
 */
int try(struct ctx_s * pctx, func_t * f, int arg) {
    asm("movl %%ebp, %0"
        "\n\t"
        "movl %%esp, %1"
      : "=r" (*pctx->ctx_ebp),
        "=r" (*pctx->ctx_esp));

    *pctx->ctx_magic = MAGIC_VALUE;

    return f(arg);
}


/*
 * Restores the context saved into the struct.
 */
int throw(struct ctx_s * pctx, int r) {
    if (*pctx->ctx_magic == MAGIC_VALUE) {
        asm("movl %0, %%ebp"
        "\n\t"
        "movl %1, %%esp"
      : "r" (*pctx->ctx_ebp),
        "r" (*pctx->ctx_esp));

        return r;
    }

    return NULL;
}