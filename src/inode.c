#include <inode.h>

#include <stdlib.h>
#include <string.h>

#include <block.h>
#include <driver.h>

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
    memcpy(inode, buf, sizeof(inode_s));
}

/*
 * Writes the inode on the disk at the specified inumber/
 *
 * inumber: the inumber of the inode
 * inode: the inode to write on the disk
 */
void write_inode(unsigned int inumber, struct inode_s * inode) {
    unsigned char buf[HDA_SECTORSIZE];

    memcpy(buf, inode, sizeof(inode_s));
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
unsigned int vblock_of_fblock(unsigned int inumber, unsigned int fblock, bool_t do_allocate) {
    struct inode_s inode;
    unsigned char buff[HDA_SECTORSIZE]

    read_inode(inumber, &inode);

    /* Direct block */
    if (fblock < INODE_NB_DIRECT_BLOCKS) {
        if (!inode.directs[fblock]) {
            if (!do_allocate) {
                return 0;
            } else {
                /* Allocate new block for direct blocks */
                inode.directs[fblock] = new_block();
                write_inode(inumber, &inode);
            }
        }
        return inode.inode_direct[fblock];
    } else  {
        fblock -= INODE_NB_DIRECT_BLOCKS;

        /* Indirect block */
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

            if (!buff[fblock]) {
                buffer[fblock] = new_block();
                write_block(current_vol, inode.inode_indirect, buff);
                write_inode(inumber, &inode);
            }
            return buff[fblock];
        }
    }
}