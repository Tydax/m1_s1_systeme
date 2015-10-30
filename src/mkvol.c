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
 */
void create_volume(unsigned int size, unsigned int cylinder, unsigned int sector) {
    /* Check for max cylinder/sector */
    if (cylinder > HDA_MAXCYLINDER) {
        fprintf(stderr, "Error: specified cylinder exceeds the highest possible cylinder.\n");
        return;
    }

    if (sector > HDA_MAXSECTOR) {
        fprintf(stderr, "Error: specified sector exceeds the highest possible sector.\n");
        return;
    }

    
}

int main(int argc, char const *argv[])
{
    int i,
        s,
        fc,
        fs;


    /* Checking arguments */

    s = fc = fs = -1

    if (argc < 5) {
        usage(argv[0]);
    }

    for (i = 0; i < argc; i++) {
        if (!strcmp("-s", argv[i])) {
            s = strtol(argv[++i], NULL, 10);
        } elseif (!strcmp("-fc"), argv[i]) {
            fc = strtol(argv[++i], NULL, 10);
        } elseif (!strcmp("-fs")) {
            fs = strtol(argv[++i], NULL, 10);
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

    if (fs == -1) {
        fs = 0;
    }

    /* Start modifying the mbr */
    read_mbr();

    /* Creating a new volume */
    create_volume(s, fc, fs);

    /* End of all modifications, saving them */
    save_mbr();

    return 0;
}