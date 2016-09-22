#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <block.h>
#include <mbr.h>

int main(int argc, char const *argv[])
{
    unsigned int volume;
    int nb_free,
        i;
    char * env_value;

    env_value = getenv(ENV_VOLUME_NAME);
    if (!env_value) {
        fprintf(stderr, "No value defined for the $%s environnement variable. Using default volume...\n",
            ENV_VOLUME_NAME);
        volume = 0;
    } else {
        volume = strtol(env_value, NULL, 10);
    }

    srand(time(NULL));
    if (!load_super(volume)) {
        /* Allocating blocks until full */
        printf("Allocating blocks until disk is full...\n");
        while (new_block());
        if (superblock.superb_first_grp_freeblocks == 0) {
            printf("Disk successfully filled!\n");
        } else {
            fprintf(stderr, "Something went wrong. Exiting...\n");
            exit(EXIT_FAILURE);
        }

        /* Random index excluding the 0 */
        nb_free = rand() % (mbr->mbr_volumes[volume].vol_nb_blocks - 1) + 1;
        printf("Freeing %d blocks...\n", nb_free);
        for (i = 1; i <= nb_free; i++) {
            free_block(i);
        }
        printf("%d free block(s)\n", superblock.superb_nb_free_blocks);

        /* Allocating blocks until full */
        i = 0;
        printf("Allocating blocks until disk is full...\n");
        while (superblock.superb_nb_free_blocks > 0) {
            i++;
            new_block();
        }

        printf("%d block(s) allocated!\n", i);
        save_super();
    } else {
        return EXIT_FAILURE;
    }
    return 0;
}