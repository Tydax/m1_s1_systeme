#include <stdlib.h>
#include <stdio.h>

#include <driver.h>

int main(int argc, char const *argv[])
{
    unsigned int cylinder,
                 sector;
    unsigned char * buffer;


    if (argc != 3) {
        fprintf(stderr, "Usage: ./dmps.out [cylinder] [sector]\n");
        exit(EXIT_FAILURE);
    }

    buffer = (unsigned char *) malloc(sizeof(unsigned char) * HDA_SECTORSIZE);

    init_driver();

    /* Parsing integers */
    cylinder = strtol(argv[1], NULL, 10);
    sector = strtol(argv[2], NULL, 10);

    read_sector(cylinder, sector, buffer);

    printf("%s\n", buffer);

    return EXIT_SUCCESS;
}