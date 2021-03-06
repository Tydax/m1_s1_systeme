#include <mbr.h>

/* Standard libraries*/
#include <assert.h>
#include <memory.h>
#include <stdlib.h>

/* My libraries */
#include <driver.h>

/* Global variable defining the loaded MBR */
struct mbr_s * mbr = NULL;

/*
 * Initialises the MBR.
 */
void init_mbr() {
    mbr->mbr_magic = MBR_MAGIC_VALUE;
    mbr->mbr_nb_vols = 0;
}

/*
 * Reads the MBR from the disk. If the MBR is not initialised, initialises it.
 */
void read_mbr() {
    unsigned char * buffer;

    assert(mbr == NULL);
    init_driver();

    /* Allocating memory for the buffer and global variable */
    mbr = (struct mbr_s *) malloc(sizeof(struct mbr_s));
    buffer = (unsigned char *) malloc(sizeof(unsigned char *) * HDA_SECTORSIZE);
    
    read_sector(0, 0, buffer);
    memcpy(mbr, buffer, sizeof(struct mbr_s));

    if (mbr->mbr_magic != MBR_MAGIC_VALUE) {
        init_mbr();
    }

    free(buffer);
}

/*
 * Saves the MBR to the hard drive.
 */
void save_mbr() {
    unsigned char * buffer;
    buffer = (unsigned char *) malloc(sizeof(unsigned char *) * HDA_SECTORSIZE);

    /* Just writing the content of mba */
    memset(buffer, 0, HDA_SECTORSIZE);
    memcpy(buffer, mbr, sizeof(struct mbr_s));

    write_sector(0, 0, buffer);
    free(mbr);
}

/*
 * Converts the volume and block numbers to cylinder and sector values.
 *
 * vol: the volume number
 * block: the block number
 * cyl: the resulting cylinder
 * sect: the resulting sector
 */
void volume_to_sector(unsigned int vol, unsigned int block, unsigned int * cyl,
    unsigned int * sect) {
    unsigned int new_sect;

    assert(vol < mbr->mbr_nb_vols);
    assert(block < mbr->mbr_volumes[vol].vol_nb_blocks);

    new_sect = mbr->mbr_volumes[vol].vol_first_sect + block;

    *cyl = mbr->mbr_volumes[vol].vol_first_cyl + (new_sect / HDA_MAXSECTOR);
    *sect = new_sect % HDA_MAXSECTOR;
}

/*
 * Reads the number of blocks of the specified volumes and copies the data to
 * the buffer.
 *
 * vol: the volume to read
 * nblock: the index of the block to read
 * buffer: the buffer to copy the data to
 */ 
void read_block(unsigned int vol, unsigned int nblock, unsigned char * buffer) {
    unsigned int cyl, sect;

    /* Checks if initialised */
    assert(mbr != NULL);
    assert(mbr->mbr_magic == MBR_MAGIC_VALUE);

    volume_to_sector(vol, nblock, &cyl, &sect);
    read_sector(cyl, sect, buffer);
}

/*
 * Writes the data stored inside the buffer to the specified number of blocks
 * of the volume.
 *
 * vol: the volume to write to
 * nblock: the number of blocks to write
 * buffer: the data to write
 */
void write_block(unsigned int vol, unsigned int nblock, unsigned char * buffer) {
    unsigned int cyl, sect;

    /* Checks if initialised */
    assert(mbr != NULL);
    assert(mbr->mbr_magic == MBR_MAGIC_VALUE);

    volume_to_sector(vol, nblock, &cyl, &sect);
    write_sector(cyl, sect, buffer);
}

/*
 * Formats the specified volume.
 *
 * vol: the volume to format
 */
void format_vol(unsigned int vol) {
    struct vol_s volume;

    /* Checks if initialised */
    assert(mbr != NULL);
    assert(mbr->mbr_magic == MBR_MAGIC_VALUE);

    volume = mbr->mbr_volumes[vol];
    format_sector(volume.vol_first_cyl, volume.vol_first_sect, volume.vol_nb_blocks, 0);
    mbr->mbr_nb_vols--;
}