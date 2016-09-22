#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <driver.h>
#include <mbr.h>

/*
 * Displays volume(s) content from the disk.
 *
 * Usage: dvol.out [name of volume to display [...]]
 * If no name of volume is given, the program will display all volumes.
 */
int main(int argc, char const *argv[]) {
    unsigned char * buffer;
    int i,
        j,
        nb_vol_to_disp;

    char display_vol; /* Current volument should be displayed ? */
    struct vol_s vol;

    /* Allocating buffer to display volumes' content */
    buffer = (unsigned char *) malloc(sizeof(unsigned char) * HDA_SECTORSIZE);
    if (buffer == NULL) {
        fprintf(stderr, "Error: could not initialise buffer.\n");
        exit(EXIT_FAILURE);
    }
    
    read_mbr();

    if (mbr->mbr_nb_vols == 0) {
        printf("No volume to display.\n");
    } else {
        printf("** List of available volumes: **\n");
        for (i = 0; i < mbr->mbr_nb_vols; i++) {
            printf("%s\n", mbr->mbr_volumes[i].vol_name);
        }

        nb_vol_to_disp = argc > 1 
                   ? argc - 1
                   : -1; /* To display all volumes */

        /* Browse through all volumes */
        for (i = 0; i < mbr->mbr_nb_vols; i++) {
            vol = mbr->mbr_volumes[i];
            display_vol = 0;

            /* Check if the volume should be displayed */
            if (nb_vol_to_disp < 0) {
                display_vol = 1;
            } else {
                /* Browse through the array of volume names */
                for (j = 1; j < argc; j++) {
                    if (!strcmp(vol.vol_name, argv[j])) {
                        display_vol = 1;
                        break;
                    }
                }
            }

            /* Display volume */
            if (display_vol) {
                printf("\n****** %s ****** \n", vol.vol_name);
                for (j = 0; j < vol.vol_nb_blocks; j++) {
                    printf("** Block n°%d **\n", j);
                    read_block(i, j, buffer);
                    printf("%s\n", buffer);
                }
            }
        }
    }

    save_mbr();

    free(buffer);

    return 0;
}