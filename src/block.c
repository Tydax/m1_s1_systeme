#include <block.h>

#include <driver.h>
#include <mbr.h>


struct superblock_s * superblock = NULL;
unsigned int current_vol;

/*
 * Initialises a new superblock.
 */
void init_super() {
    struct grp_freeblocks_s fb;
    unsigned char buf[HDA_SECTORSIZE];

    fb.size = mbr->mvr_volumes[current_vol].vol_nb_blocks - 1;
    fb.suivants = 0;
    /* Saves the super block to the disk */
    *((struct superblock_s *) buf) = superblock;
    write_block(current_vol, 0, buf);
    /* Saves the group of free blocks to the disk */
    *((struct grp_freeblocks_s *) buf) = fb;
    write_block(current_vol, 1, buf);
}

/*
 * Loads the superblock of the specified volume. If it doesn't exist, intialises a
 * new superblock.
 * Must be called before calling any function in this library.
 *
 * vol: The index of the volume to load.
 * Returns 0 if it was successfully loaded.
 */
int load_super(unsigned int vol) {
    unsigned char buf[HDA_SECTORSIZE];

    superblock = (struct superblock_s *) malloc(sizeof(superblock_s));
    if (!superblock) {
        return -1;
    }

    /* Load on disk */
    read_mbr();
    read_block(vol, 0, buf);
    memcpy(superblock, buf, sizeof(superblock_s));

    current_vol = vol;

    /* If not initialised, let's initialise it */
    if (superblock->sb_magic != SB_MAGIC_VALUE) {
        init_super();
    }
}

/*
 * Saves the superblock information on the loaded volume.
 * Must be called at the end of your program.
 */
void save_super() {
    unsigned char buf[HDA_SECTORSIZE];

    memcpy(buf, superblock, sizeof(struct superblock_s));
    write_block(current_vol, 0, buf);
    save_mbr();
}

/*
 * Allocates a new block on the loaded volume.
 *
 * Returns the index of the allocated block, or 0 if the volume is full.
 */
unsigned int new_block() {
    struct grp_freeblocks_s fb;
    int res;
    unsigned char buf[HDA_SECTORSIZE];

    if (superblock->sb_first_grp_freeblocks != 0) {
        
    }

    read_block(current_vol, superblock->sb_first_grp_freeblocks, buf);

    res = -1;
}

/*
 * Frees the block of the specified index on the currently loaded volume.
 *
 * block: The index of the block to free.
  */
void free_block(unsigned int block);