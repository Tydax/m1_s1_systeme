#ifndef __block_h__
#define __block_h__

/*
 * Library offering an abstraction to manage blocks inside a volume.
 */

#define ENV_VOLUME_NAME "CURRENT_VOLUME"

#define SB_NAME_LENGTH 32
#define SB_MAGIC_VALUE 0xDEADCACA

#define FB_MAGIC_VALUE 0xDADD1D00
 
/*
 * Structure describing the superblock contained in the volume
 * The superblock gives a description of the free blocks contained in the volume.
 */
struct superblock_s {
    int             superb_magic;
    int             superb_serial;
    char            superb_name[SB_NAME_LENGTH];
    unsigned int    superb_root;
    unsigned int    superb_nb_free_blocks;  /* Total of free blocks contained in the volume */
    unsigned int    superb_first_grp_freeblocks; /* Reference to the first block of the first group of free blocks */
};

/* Structure describing a group of free blocks */
struct grp_freeblocks_s {
    int             freeb_magic;
    unsigned int    freeb_size;
    unsigned int    freeb_next;
};

extern struct superblock_s superblock;
extern unsigned int current_vol;

/*
 * Loads the superblock of the specified volume. If it doesn't exist, intialises a
 * new superblock.
 * Must be called before calling any function in this library.
 *
 * vol: The index of the volume to load.
 * Returns 0 if it was successfully loaded.
 */
int load_super(unsigned int vol);

/*
 * Saves the superblock information on the loaded volume.
 * Must be called at the end of your program.
 */
void save_super();

/*
 * Allocates a new block on the loaded volume.
 *
 * Returns the index of the allocated block, or 0 if the volume is full.
 */
unsigned int new_block();

/*
 * Frees the block of the specified index on the currently loaded volume.
 *
 * block: The index of the block to free.
  */
void free_block(unsigned int block);


#endif