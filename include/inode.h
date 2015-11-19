#ifndef __INODE_H__
#define __INODE_H__

/*
 * Library offering an implementation of inode.
 */

#define INODE_NB_DIRECT_BLOCKS ((HDA_SECTORSIZE - sizeof(inode_type_e) - 3 * sizeof(unsigned int)) / sizeof(unsigned int));
#define INODE_NB_BLOCKS_PER_BLOCK (HDA_SECTORSIZE / sizeof(unsigned int));

/* Enum representing the type of file */
enum inode_type_e {
    REG, DIR
};

/* Structure describing an inode */
struct inode_s {
    enum inode_type_e   inode_type;
    unsigned int        inode_size;
    unsigned int        inode_direct[INODE_NB_BLOCKS_DIRECTS];
    unsigned int        inode_indirect;
    unsigned int        inode_double_indirect;
};

/*
 * Reads the inode at the specified inumber, and saves it inside the inode.
 *
 * inumber: the inumber of the inode
 * inode: the pointer where to save the inode
 */
void read_inode(unsigned int inumber, struct inode_s * inode);

/*
 * Writes the inode on the disk at the specified inumber/
 *
 * inumber: the inumber of the inode
 * inode: the inode to write on the disk
 */
void write_inode(unsigned int inumber, struct inode_s * inode);

/*
 * Initialises a new empty inode of the specified type.
 *
 * type: the type of the inode
 * Returns the inumber of the inode.
 */
unsigned int create_inode(enum inode_type_e type);

/*
 * Frees the inode and all the blocks associated with this inode.
 *
 * inumber: the inumber of the inode to free
 */
int delete_inode(unsigned int inumber);

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
unsigned int vblock_of_fblock(unsigned int inumber, unsigned int fblock, bool_t do_allocate);

#endif