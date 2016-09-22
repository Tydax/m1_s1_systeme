#ifndef __DRIVER_H__
#define __DRIVER_H__

/*
 * Library used to interact with a simulated hard drive.
 */

/* Hardware constants (very important yes believe me) */
#define HDA_CMDREG      0x3F6
#define HDA_DATAREGS    0x110
#define HDA_MAXCYLINDER 16      
#define HDA_MAXSECTOR   16
#define HDA_IRQ         14
#define HDA_SECTORSIZE  256

/*
 * Inits the driver.
 * Returns 0 if the driver was successfully initialised.
 */
int init_driver();

/*
 * Reads one sector at the specified cylinder and sector from the hard drive,
 * and writes the result inside the buffer.
 *
 * cylinder: the cylinder where to read
 * sector: the sector where to read
 * buffer: the buffer to store the read data
 */
void read_sector(unsigned int cylinder, unsigned int sector, unsigned char * buffer);

/*
 * Writes the data stored inside the buffer to the specified sector and cylinder.
 *
 * cylinder: the cylinder where to write
 * sector: the sector where to write
 * buffer: the buffer containing the data to write
 */
void write_sector(unsigned int cylinder, unsigned int sector, unsigned char * buffer);

/*
 * Formats the specified number of sector(s) at the cylinder and sector.
 *
 * cylinder: the cylinder to format
 * sector: the sector to format
 * nector: the number of sectors to format
 * value: the value to format to
 */
void format_sector(unsigned int cylinder, unsigned int sector, unsigned int nsector, unsigned int value);

#endif