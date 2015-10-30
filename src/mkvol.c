#include <stdio.h>
#include <stdlib.h>

#include <driver.h>
#include <mbr.h>

void usage(char * prog) {
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
void create_volume(unsigned int size, unsigned int cylinder, unsigned int sector, char * name) {
    int i,
    struct vol_s vol;
    int vol_cyl, vol_sector, // Store the cyl and sect when browsing through volumes
        target_cyl, target_sector;

    /* Check for max cylinder/sector */
    if (cylinder > HDA_MAXCYLINDER) {
        fprintf(stderr, "Error: specified cylinder exceeds the highest possible cylinder.\n");
        return;
    }

    if (sector > HDA_MAXSECTOR) {
        fprintf(stderr, "Error: specified sector exceeds the highest possible sector.\n");
        return;
    }

    /* Calculating the max cylinder and sector values */
    target_cyl = cylinder + ((sector + size) / HDA_MAXSECTOR);
    target_sect = (sector + size) % HDA_MAXSECTOR;

    /* Checking that the volume that has to be created will not overlap another one */
    for (i = 0; i < mbr->mbr_nb_vols; i++) {
        vol = mbr->mbr_volumes[i];

        volume_to_sector(i, vol.vol_nb_blocks - 1, &vol_cyl, &vol_sector);

        if (cylinder == vol.vol_first_cyl && sector == vol.vol_first_sect
            || cylinder > vol.vol_first_cyl && ) {
            fprintf(stderr, "Error: specified cylinder and sector values will overlap existing volume\n");
        }
    }

    /* Generating name */
    if (name == NULL) {
        name = (char *) malloc(sizeof(char) * MBR_NAME_LENGTH);
        sprintf(name, "Volume [%d, %d, %d]", size, cylinder, sector);
    }

    free(name);
}

int main(int argc, char const *argv[])
{
    int i,
        s,
        fc,
        fs;

    char * n;

    /* Checking arguments */
    s = fc = fs = -1;
    n = NULL;

    if (argc < 5) {
        usage(argv[0]);
    }

    for (i = 0; i < argc; i++) {
        if (!strcmp("-s", argv[i])) {
            s = strtol(argv[++i], NULL, 10);
        } elseif (!strcmp("-fc"), argv[i]) {
            fc = strtol(argv[++i], NULL, 10);
        } elseif (!strcmp("-fs", argv[i])) {
            fs = strtol(argv[++i], NULL, 10);
        } elseif (!strcmp("-n", argv[i])) {
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
        fprintf(stderr, "Wrong imput: cannot write on the Master Boot Record, use other values than (0, 0).%s\n");
    }

    /* Default values */
    if (fs == -1) {
        fs = 0;
    }

    /* Start modifying the mbr */
    read_mbr();

    /* Creating a new volume */
    create_volume(s, fc, fs, n);

    /* End of all modifications, saving them */
    save_mbr();

    return 0;
}