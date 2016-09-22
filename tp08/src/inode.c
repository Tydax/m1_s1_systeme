#include <inode.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <block.h>
#include <driver.h>
#include <mbr.h>

/*
 * Reads the inode at the specified inumber, and saves it inside the inode.
 *
 * inumber: the inumber of the inode
 * inode: the pointer where to save the inode
 */
void read_inode(unsigned int inumber, struct inode_s * inode) {
    unsigned char buf[HDA_SECTORSIZE];

    read_mbr();
    read_block(current_vol, inumber, buf);
    memcpy(inode, buf, sizeof(struct inode_s));
}

/*
 * Writes the inode on the disk at the specified inumber/
 *
 * inumber: the inumber of the inode
 * inode: the inode to write on the disk
 */
void write_inode(unsigned int inumber, struct inode_s * inode) {
    unsigned char buf[HDA_SECTORSIZE];

    memset(buf, 0, HDA_SECTORSIZE);
    memcpy(buf, inode, sizeof(struct inode_s));
    write_block(current_vol, inumber, buf);
}

/*
 * Initialises a new empty inode of the specified type.
 *
 * type: the type of the inode
 * Returns the inumber of the inode.
 */
unsigned int create_inode(enum inode_type_e type) {
    struct inode_s inode;
    unsigned int inumber;

    memset(&inode, 0, sizeof(struct inode_s));
    inode.inode_type = type;
    inumber = new_block();
    assert(inumber != 0);
    write_inode(inumber, &inode);

    return inumber;
}

/*
 * Frees the inode and all the blocks associated with this inode.
 *
 * inumber: the inumber of the inode to free
 */
int delete_inode(unsigned int inumber) {
    int i, 
        j;
    unsigned char buf[INODE_NB_BLOCKS_PER_BLOCK+1],
                  mouton[INODE_NB_BLOCKS_PER_BLOCK+1];
    struct inode_s inode;

    read_inode(inumber, &inode);

    /* Freeing direct blocks */
    for (i = 0; i < INODE_NB_DIRECT_BLOCKS; i++) {
        if (inode.inode_direct[i]) {
            free_block(inode.inode_direct[i]);
        }
    }

    /* Freeing indirect blocks */
    if (inode.inode_indirect) {
        read_block(current_vol, inode.inode_indirect, buf);

        for (i = 0; i < INODE_NB_BLOCKS_PER_BLOCK; i++) {
            if (buf[i]) {
                free_block(buf[i]);
            }
        }
        free_block(inode.inode_indirect);
    }

    /* Freeing double indirect blocks */
    if (inode.inode_double_indirect) {
        read_block(current_vol, inode.inode_double_indirect, buf);

        for (i = 0; i < INODE_NB_BLOCKS_PER_BLOCK; i++) {
            if (buf[i]) {
                read_block(current_vol, buf[i], mouton);
                for (j = 0; j < INODE_NB_BLOCKS_PER_BLOCK; j++)
                {
                    if(mouton[j]) {
                        free_block(mouton[j]);
                    }
                }
            }
        }
        free_block(inode.inode_double_indirect);
    }

    free_block(inumber);

    return 0;
}

/*
 * Fetches the corresponding number of block in the specified inode.
 *
 * inumber: the number of the inode
 * fblock: the number of the block to fetch
 * do_allocate: if the block does not exist, this defines whether it should be allocated
 * or not (true to allocate, false to allocate)
 *
 * Returns the actual block of volume number.
 */
unsigned int vblock_of_fblock(unsigned int inumber, unsigned int fblock, char do_allocate) {
    struct inode_s inode;
    unsigned char buff[HDA_SECTORSIZE],
                  muton[HDA_SECTORSIZE]; /* I’m hilarious. */
    unsigned int ind;

    read_inode(inumber, &inode);

    /* Direct block */
    if (fblock < INODE_NB_DIRECT_BLOCKS) {
        if (!inode.inode_direct[fblock]) {
            if (!do_allocate) {
                return 0;
            } else {
                /* Allocate new block for direct blocks */
                inode.inode_direct[fblock] = new_block();
                write_inode(inumber, &inode);
            }
        }
        return inode.inode_direct[fblock];
    } else {
        /* Indirect block */
        fblock -= INODE_NB_DIRECT_BLOCKS;

        if (fblock < INODE_NB_BLOCKS_PER_BLOCK) {

            /* Indirect block is not initialised */
            if (!inode.inode_indirect) {
                if (!do_allocate) {
                    return 0;
                } else {
                    /* Allocate new block for indirect block */
                    inode.inode_indirect = new_block();
                    memset(buff, 0, HDA_SECTORSIZE);
                    write_block(current_vol, inode.inode_indirect, buff);
                    write_inode(inumber, &inode);
                }

            }   
            read_block(current_vol, inode.inode_indirect, buff);

            /* Block referenced in indirect block is not initiliased */
            if (!buff[fblock]) {
                if (!do_allocate) {
                    return 0;
                } else {
                    buff[fblock] = new_block();
                    write_block(current_vol, inode.inode_indirect, buff);
                    write_inode(inumber, &inode);
                }
            }
            
            return buff[fblock];
        } else {
            /* Double indirect block */
            fblock -= INODE_NB_BLOCKS_PER_BLOCK;

            if (fblock < INODE_NB_BLOCKS_PER_BLOCK) {
                /* Double indirect block is not initialised */
                if (!inode.inode_double_indirect) {
                    if (!do_allocate) {
                        return 0;
                    } else {
                        /* Allocate new block for double indirect block */
                        inode.inode_double_indirect = new_block();
                        memset(buff, 0, HDA_SECTORSIZE);
                        write_block(current_vol, inode.inode_double_indirect, buff);
                        write_inode(inumber, &inode);
                    }
                }

                read_block(current_vol, inode.inode_double_indirect, buff);
                
                ind = fblock / INODE_NB_BLOCKS_PER_BLOCK;
                /* Indirect block referenced by double indirect block is not initialised */
                if (!buff[ind]) {
                    if (!do_allocate) {
                        return 0;
                    } else {
                        /* Allocate new block for indirect block referenced by double indirect block */
                        buff[ind] = new_block();
                        memset(muton, 0, HDA_SECTORSIZE);
                        write_block(current_vol, buff[ind], muton);
                        write_inode(inumber, &inode);
                    }
                }

                read_block(current_vol, buff[ind], muton);
                ind = fblock % INODE_NB_BLOCKS_PER_BLOCK;

                /* Block referenced by indirect block referenced by double indirect block is not initialised */
                if (!muton[ind]) {
                    if (!do_allocate) {
                        return 0;
                    } else {
                        /* Allocate new block referenced by indirect block referenced by double indirect block */
                        muton[ind] = new_block();
                        write_block(current_vol, buff[ind], muton);
                        write_inode(inumber, &inode);
                    }
                }

                return muton[ind];
            }
        }
    }

    return 0;
}