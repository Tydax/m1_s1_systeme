#include <stdlib.h>
#include <stdio.h>
#include <hardware.h>

#define HDA_CMDREG      0x3F6
#define HDA_DATAREGS    0x110
#define HDA_IRQ         14   

void seek(int cylinder, int sector) {
    /* Passing the cylinder value */
    _out(HDA_DATAREGS+0, (cylinder >> 8) &0xFF); /* most significant bit */
    _out(HDA_DATAREGS+1, cylinder &0xFF); /* less significant bit */
    /* Passing the sector value */
    _out(HDA_DATAREGS+2, (sector >> 8) &0xFF);
    _out(HDA_DATAREGS+3, sector &0xFF);

    /* Passing the command code */
    _out(HDA_CMDREG, CMD_SEEK);

    _sleep(HDA_IRQ);
}

void read(int nbsector) {
    _out(HDA_DATAREGS+0, (nbsector >> 8) &0xFF);
    _out(HDA_DATAREGS+1, nbsector &0xFF);
    _out(HDA_CMDREG, CMD_READ);

    printf("%s\n", MASTERBUFFER);
}

int main(int argc, char const *argv[])
{
    int cylinder,
        sector;

    if (argc != 3) {
        fprintf(stderr, "Usage: ./dmps.out [cylinder] [sector]\n");
        exit(EXIT_FAILURE);
    }

    init_hardware("../hardware.ini");

    /* Parsing integers */
    cylinder = strtol(argv[1], NULL, 10);
    sector = strtol(argv[2], NULL, 10);

    seek(cylinder, sector);
    read(1);

    return EXIT_SUCCESS;
}