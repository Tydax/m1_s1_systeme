#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <driver.h>
#include <mbr.h>

void usage(const char * prog) {
    fprintf(stderr, "Usage: %s -s size -fc firstcylinder [-fs firstsector]\n", prog);
    exit(EXIT_FAILURE);
}

/*
 * Creates a new volume on the hard drive.
 * 
 * size: the number of blocks
 * cylinder: the first cylinder where to start writing
 * sector: the first sector where to start writing
 * name: the name of the volume to create (NULL will generate a name following this format: "Vol [cyl, sect, size]")
 */
int create_volume(unsigned int size, unsigned int cylinder, unsigned int sector, const char * name) {
    int i;
    struct vol_s vol;
    unsigned int vol_cyl, vol_sect, /* Store the cyl and sect when browsing through volumes */
                 target_cyl, target_sect;
    char generated_name[MBR_NAME_LENGTH];

    if (mbr->mbr_nb_vols == MBR_MAX_NB_VOL) {
        fprintf(stderr, "Error: reached maximum number of volumes on the disk.\n");
        return EXIT_FAILURE;
    }

    /* Check for max cylinder/sector */
    if (cylinder > HDA_MAXCYLINDER) {
        fprintf(stderr, "Error: specified cylinder exceeds the highest possible cylinder.\n");
        return EXIT_FAILURE;
    }

    if (sector > HDA_MAXSECTOR) {
        fprintf(stderr, "Error: specified sector exceeds the highest possible sector.\n");
        return EXIT_FAILURE;
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
            return EXIT_FAILURE;
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

    printf("Volume [%s] créé avec succès.\n", vol.vol_name);
    return EXIT_SUCCESS;
}

void display_all_volume_name() {
    int i;

    for (i = 0; i < mbr->mbr_nb_vols; ++i) {
        printf("[%s]\n", mbr->mbr_volumes[i].vol_name);
    }
}

int main(int argc, char const *argv[]) {
    int i,
        s,
        fc,
        fs,
        res;

    const char * n;

    /* Checking arguments */
    s = fc = fs = -1;
    n = NULL;

    if (argc < 5) {
        usage(argv[0]);
    }

    for (i = 1; i < argc; i++) {
        if (!strcmp("-s", argv[i])) {
            s = strtol(argv[++i], NULL, 10);
        } else if (!strcmp("-fc", argv[i])) {
            fc = strtol(argv[++i], NULL, 10);
        } else if (!strcmp("-fs", argv[i])) {
            fs = strtol(argv[++i], NULL, 10);
        } else if (!strcmp("-n", argv[i])) {
            n = argv[++i];
        } else {
            usage(argv[0]);
        }
    }

    if (s == -1 || fc == -1) {
        usage(argv[0]);
    }

    /* Checking for idiotic input */
    if (s < 0 || fc < 0 || fs < 0) {
        fprintf(stderr, "Wrong input: the arguments can't be negative.\n");
    } else if (fc == 0 && fs == 0) {
        fprintf(stderr, "Wrong imput: cannot write on the Master Boot Record, use other values than (0, 0).\n");
    }

    /* Default values */
    if (fs == -1) {
        fs = 0;
    }

    /* Start modifying the mbr */
    read_mbr();

    /* Creating a new volume */
    res = create_volume(s, fc, fs, n);

    display_all_volume_name();

    /* End of all modifications, saving them */
    save_mbr();

    return res;
}