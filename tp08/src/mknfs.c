#include <stdlib.h>
#include <stdio.h>

#include <block.h>
#include <driver.h>
#include <mbr.h>
#include <string.h>

int create_volume(unsigned int size, unsigned int cylinder, unsigned int sector, const char * name) {
    int i;
    struct vol_s vol;
    unsigned int vol_cyl, vol_sect, /* Store the cyl and sect when browsing through volumes */
                 target_cyl, target_sect;
    char generated_name[MBR_NAME_LENGTH];

    if (mbr->mbr_nb_vols == MBR_MAX_NB_VOL) {
        fprintf(stderr, "Error: reached maximum number of volumes on the disk.\n");
        return -1;
    }

    /* Check for max cylinder/sector */
    if (cylinder > HDA_MAXCYLINDER) {
        fprintf(stderr, "Error: specified cylinder exceeds the highest possible cylinder.\n");
        return -1;
    }

    if (sector > HDA_MAXSECTOR) {
        fprintf(stderr, "Error: specified sector exceeds the highest possible sector.\n");
        return -1;
    }

    /* Calculating the max cylinder and sector values */
    target_cyl = cylinder + ((sector + size) / HDA_MAXSECTOR);
    target_sect = (sector + size) % HDA_MAXSECTOR;

    /* Checking that the volume that has to be created will not overlap another one */
    for (i = 0; i < mbr->mbr_nb_vols; i++) {
        vol = mbr->mbr_volumes[i];

        volume_to_sector(i, vol.vol_nb_blocks - 1, &vol_cyl, &vol_sect);

        /*
           Each 'or' in the 'if' statement corresponds respectively to those cases:
             * Different cylinders
             * New volume before the existing ones
             * New volume after the existing ones
         */
        if (!((cylinder != vol.vol_first_cyl && target_cyl != vol_cyl) 
            || (target_cyl == vol.vol_first_cyl && target_sect < vol.vol_first_sect) 
            || (cylinder == vol_cyl && sector > vol_sect))) {
            fprintf(stderr, "Error: specified cylinder and sector values will overlap existing volume\n");
            return -1;
        }
    }

    /* Generating name */
    if (name == NULL) {
        sprintf(generated_name, "Volume [%d, %d, %d]", size, cylinder, sector);
    }

    /* Creating volume */
    vol.vol_first_cyl = cylinder;
    vol.vol_first_sect = sector;
    vol.vol_nb_blocks = size;
    vol.vol_type = BASE;
    memcpy(vol.vol_name,
          (name == NULL
          ? generated_name
          : name),
          MBR_NAME_LENGTH);

    mbr->mbr_volumes[mbr->mbr_nb_vols] = vol;
    mbr->mbr_nb_vols++;

    printf("Volume [%s] successfully created!\n", vol.vol_name);
    return mbr->mbr_nb_vols - 1;
}

int main(int argc, char const *argv[])
{
    int vol;
    /* Start modifying the mbr */
    read_mbr();

    vol = create_volume(185, 1, 0, NULL);

    if (vol != -1) {
        load_super(vol);
        save_super();
        printf("Superblock initialised.\n");
    } else {
        fprintf(stderr, "Failed to initialise volume. Exiting...\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}