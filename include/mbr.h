#ifndef __MBR_H__
#define __MBR_H__

/*
 * Library used to interact with volumes and blocks.
 */

/* Constants */
#define MBR_MAX_NB_VOL  8
#define MBR_NAME_LENGTH 32
#define MBR_MAGIC_VALUE 0x0CADD1E5

/* Enum describing the type of volume. */
enum vol_type_e {
    BASE,
    ANNEXE,
    OTHER
};

/* Struct describing a volume containing data. */
struct vol_s {
    unsigned int    vol_first_cyl;      /* Cylinder where the volumes begins */
    unsigned int    vol_first_sect;     /* Sector where the volume begins */
    unsigned int    vol_nb_blocks;       /* Number of blocks */
    enum vol_type_e vol_type;
    char            vol_name[MBR_NAME_LENGTH];
};

/*
 * Struct describing the Master Boot Record, the first volume containing
 * all information concerning the other volumes on the disk.
 */
struct mbr_s {
    int             mbr_magic; /* Magic number to check initialisation */
    unsigned int    mbr_nb_vols;      /* Number of volumes contained on the disk */
    struct vol_s    mbr_volumes[MBR_MAX_NB_VOL]; /* The stored volumes */
};

/* Global variable defining the loaded MBR */
extern struct mbr_s * mbr;

/*
 * Reads the MBR from the disk. If the MBR is not initialised, initialises it.
 */
void read_mbr();

/*
 * Saves the MBR to the hard drive.
 */
void save_mbr();

/*
 * Converts the volume and block numbers to cylinder and sector values.
 *
 * vol: the volume number
 * block: the block number
 * cyl: the resulting cylinder
 * sect: the resulting sector
 */
void volume_to_sector(unsigned int vol, unsigned int block, unsigned int * cyl,
    unsigned int * sect);

/*
 * Reads the number of blocks of the specified volumes and copies the data to
 * the buffer.
 *
 * vol: the volume to read
 * nblock: the number of blocks to read
 * buffer: the buffer to copy the data to
 */ 
void read_block(unsigned int vol, unsigned int nblock, unsigned char * buffer);

/*
 * Writes the data stored inside the buffer to the specified number of blocks
 * of the volume.
 *
 * vol: the volume to write to
 * nblock: the number of blocks to write
 * buffer: the data to write
 */
void write_block(unsigned int vol, unsigned int nblock, unsigned char * buffer);

/*
 * Formats the specified volume.
 *
 * vol: the volume to format
 */
void format_vol(unsigned int vol);

#endif