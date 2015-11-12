#include <stdlib.h>
#include <stdio.h>

#include <block.h>
#include <mbr.h>

void display_all_volume_name() {
    int i;

    printf("** List of available volumes: **\n");
    for (i = 0; i < mbr->mbr_nb_vols; ++i) {
        printf("[%d][%s]\n", i, mbr->mbr_volumes[i].vol_name);
    }
}

int main(int argc, char const *argv[])
{
    unsigned int volume;
    float prct_free;
    char * env_value;

    env_value = getenv(ENV_VOLUME_NAME);
    if (!env_value) {
        fprintf(stderr, "No value defined for the $%s environnement variable. Using default volume...\n",
            ENV_VOLUME_NAME);
        volume = 0;
    } else {
        volume = strtol(env_value, NULL, 10);
    }

    load_super(volume);

    display_all_volume_name();

    printf("Current volume: %s\n", mbr->mbr_volumes[volume].vol_name);
    if (superblock.superb_nb_free_blocks == 0) {
        printf("Volume is full.\n");
    } else {
        prct_free = superblock.superb_nb_free_blocks / (mbr->mbr_volumes[volume].vol_nb_blocks - 1) * 100;
        printf("Free space: %d block(s) (%.2f%% free space)\n", superblock.superb_nb_free_blocks, prct_free);
    }

    return 0;
}