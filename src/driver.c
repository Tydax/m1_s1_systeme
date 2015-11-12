#include <driver.h>

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include <hardware.h>

void empty_function() {
    return;
}

/*
 * Inits the driver.
 * Returns 0 if the driver was successfully initialised.
 */
int init_driver() {
    int i;
    char * env_value;

    env_value = getenv(ENV_HW_CONFIG_PATH);
    if (!env_value) {
        if (init_hardware("../hardware.ini") == 0) {
            fprintf(stderr, "Hardware initialization failed\n");
            return -1;
        }
    } else {
        if (init_hardware(env_value) == 0) {
            fprintf(stderr, "Hardware initialization failed\n");
            return -1;
        }
    }



    /* Initialise IRQ functions to empty functions */
    for(i = 0; i < 16; i++) {
        IRQVECTOR[i] = empty_function;
    }

    return 0;
}

/*
 * Places the read head at the specified cylinder and sector.
 *
 * cylinder: the cylinder where to read
 * sector: the sector where to read
 */
void seek_sector(unsigned int cylinder, unsigned int sector) {
    _out(HDA_DATAREGS, (cylinder >> 8) & 0xFF); /* most significant bit */
    _out(HDA_DATAREGS+1, cylinder & 0xFF); /* less significant bit */
    
    /* Passing the sector value */
    _out(HDA_DATAREGS+2, (sector >> 8) & 0xFF);
    _out(HDA_DATAREGS+3, sector & 0xFF);

    /* Passing the command code */
    _out(HDA_CMDREG, CMD_SEEK);

    _sleep(HDA_IRQ);
}

/*
 * Reads one sector at the specified cylinder and sector from the hard drive,
 * and writes the result inside the buffer.
 *
 * cylinder: the cylinder where to read
 * sector: the sector where to read
 * buffer: the buffer to store the read data
 */
void read_sector(unsigned int cylinder, unsigned int sector, unsigned char * buffer) {
    seek_sector(cylinder, sector);

    _out(HDA_DATAREGS, 0);
    _out(HDA_DATAREGS+1, 1);
    _out(HDA_CMDREG, CMD_READ);

    _sleep(HDA_IRQ);

    memcpy(buffer, MASTERBUFFER, HDA_SECTORSIZE);
}

/*
 * Writes the data stored inside the buffer to the specified sector and cylinder.
 *
 * cylinder: the cylinder where to write
 * sector: the sector where to write
 * buffer: the buffer containing the data to write
 */
void write_sector(unsigned int cylinder, unsigned int sector, unsigned char * buffer) {
    seek_sector(cylinder, sector);

    /* Prepare data to be written */
    memcpy(MASTERBUFFER, buffer, HDA_SECTORSIZE);

    _out(HDA_DATAREGS, 0);
    _out(HDA_DATAREGS+1, 1);
    _out(HDA_CMDREG, CMD_WRITE);

    _sleep(HDA_IRQ);
}

/*
 * Formats the specified number of sector(s) at the cylinder and sector.
 *
 * cylinder: the cylinder to format
 * sector: the sector to format
 * nector: the number of sectors to format
 * value: the value to format to
 */
void format_sector(unsigned int cylinder, unsigned int sector, unsigned int nsector, unsigned int value) {
    seek_sector(cylinder, sector);

    _out(HDA_DATAREGS, (nsector >> 8) & 0xFF);
    _out(HDA_DATAREGS+1, nsector & 0xFF);
    _out(HDA_DATAREGS+2, (value >> 24) & 0xFF);
    _out(HDA_DATAREGS+3, (value >> 16) & 0xFF);
    _out(HDA_DATAREGS+4, (value >> 8) & 0xFF);
    _out(HDA_DATAREGS+5, value & 0xFF);

    _out(HDA_CMDREG, CMD_FORMAT);

    _sleep(HDA_IRQ);
}
