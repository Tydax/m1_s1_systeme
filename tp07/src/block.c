#include <block.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <driver.h>
#include <mbr.h>


struct superblock_s superblock;
unsigned int current_vol;

/*
 * Initialises a new superblock.
 */
void init_super() {
    struct grp_freeblocks_s fb;
    unsigned char buf[HDA_SECTORSIZE];

    fb.freeb_magic = FB_MAGIC_VALUE;
    fb.freeb_size = mbr->mbr_volumes[current_vol].vol_nb_blocks - 1;
    fb.freeb_next = 0;

    superblock.superb_magic = SB_MAGIC_VALUE;
    superblock.superb_serial = 1337;
    memcpy(superblock.superb_name, mbr->mbr_volumes[current_vol].vol_name, SB_NAME_LENGTH);
    superblock.superb_root = -1;
    superblock.superb_nb_free_blocks = fb.freeb_size;
    superblock.superb_first_grp_freeblocks = 1;

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

    /* Load on disk */
    read_mbr();
    read_block(vol, 0, buf);
    memcpy(&superblock, buf, sizeof(struct superblock_s));

    current_vol = vol;

    /* If not initialised, let's initialise it */
    if (superblock.superb_magic != SB_MAGIC_VALUE) {
        init_super();
    }

    return 0;
}

/*
 * Saves the superblock information on the loaded volume.
 * Must be called at the end of your program.
 */
void save_super() {
    unsigned char buf[HDA_SECTORSIZE];

    memcpy(buf, &superblock, sizeof(struct superblock_s));
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

    assert(superblock.superb_magic == SB_MAGIC_VALUE);
    
    res = 0;
    if (superblock.superb_first_grp_freeblocks != 0) {
        read_block(current_vol, superblock.superb_first_grp_freeblocks, buf);
        memcpy(&fb, buf, sizeof(struct grp_freeblocks_s));
        assert(fb.freeb_magic == FB_MAGIC_VALUE);
        res = superblock.superb_first_grp_freeblocks;
        superblock.superb_nb_free_blocks--;

        if (fb.freeb_size > 1) {
            /* Update the size of the group of free blocks */
            fb.freeb_size--;
            /* Write the superblock to the next block */
            superblock.superb_first_grp_freeblocks++;
            memcpy(buf, &fb, sizeof(struct grp_freeblocks_s));
            write_block(current_vol, superblock.superb_first_grp_freeblocks, buf);
        } else {
            /* Update next group of free blocks if this was the last block in this group */
            superblock.superb_first_grp_freeblocks = fb.freeb_next;
        }
    }

    return res;
}

/*
 * Frees the block of the specified index on the currently loaded volume.
 *
 * block: The index of the block to free.
  */
void free_block(unsigned int block) {
    struct grp_freeblocks_s fb;
    unsigned char buf[HDA_SECTORSIZE];

    assert(superblock.superb_magic == SB_MAGIC_VALUE);
    assert(block != 0);

    fb.freeb_magic = FB_MAGIC_VALUE;
    fb.freeb_size = 1;
    fb.freeb_next = superblock.superb_first_grp_freeblocks;
    superblock.superb_first_grp_freeblocks = block;
    superblock.superb_nb_free_blocks++;

    memcpy(buf, &fb, sizeof(struct grp_freeblocks_s));
    write_block(current_vol, block, buf);
}