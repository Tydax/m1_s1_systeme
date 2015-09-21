#include <stdio.h>
#include <stdlib.h>

#include <try.h>

/* Struct used to store a saved context */
static struct ctx_s * ctx;

static int mul(int depth) {
    int i, nb;

    switch(scanf("%d", &i)) {
        case EOF :
            return 1; /* neutral element */
        case 0 :
            return mul(depth + 1);
        case 1 :
            if (i) {
                nb = i * mul(depth + 1);
                printf("suus\n");
                return nb;
            } else {
                /* Restores context when getting a 0 */
                return throw(ctx, 0);
            }
    }

    return 0;
}

int main() {
    int product;

    /* Allocating memory for context struct */
    ctx = (struct ctx_s *) malloc(sizeof(struct ctx_s));

    printf("A list of int, please\n");
    product = try(ctx, &mul, 0);
    printf("product = %d\n", product);

    /* Let it gooo, let it gooo */
    free(ctx);

    return 0;
}