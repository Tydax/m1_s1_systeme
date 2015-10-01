static struct ctx_s * loop;

int create_ctx(int stack_size, func_t f, void * args) {
    

    /* init_ctx */
    if (!init_ctx(ptr, stack_size, f, args)) {
       return -2; 
    }

    /* Ajout à la loop */
    if (loop) {
        ctx_ptr->ctx_next = loop->ctx_next;
    } else {
        loop = ctx_ptr;
    }

    loop->ctx_next = ctx_ptr;

}

void yield() {
    if (!ctx_crt) {
        switch_to(anneau);
    } else {
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
    yield ~ switch_to(ctx_courant->suivant)
    1. 1ère fois ?
        Définition du contexte courant si non défini
    2. fin d'un context f(arg) -> boum (ça explose ?)
        1ère actiation, f(arg)è (free(ctx_courant_stack), free(ctx_courant))
    Au début de yield, il faut trouver le ctx_suivant qui n'est pas terminé
        * modifier l'anneau
        * libéreeeer (et délivreeeer) l'espace mémoire de la pile du ctx
        * libérer 
*/