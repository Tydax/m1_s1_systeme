#include <stdlib.h>
#include <stdio.h>

#include <block.h>
#include <mbr.h>
#include <inode.h>

int main(int argc, char const *argv[])
{
    int i, j, volume;
    unsigned int inumber;
    struct inode_s inode;
    unsigned char buff[HDA_SECTORSIZE],
                  muton[HDA_SECTORSIZE];
    char * env_value;

    env_value = getenv(ENV_VOLUME_NAME);
    if (!env_value) {
        fprintf(stderr, "No value defined for the $%s environnement variable. Using default volume...\n",
            ENV_VOLUME_NAME);
        volume = 0;
    } else {
        volume = strtol(env_value, NULL, 10);
    }

    if (!load_super(volume)) {
        inumber = create_inode(REG);
        printf("Working on inumber n°%d\n", inumber);
        /* Fill inode!!!!!!!!!!!!!!!!!!!1\!!11!\\1ONE!!//\ */
        i = 0;
        while ((j = vblock_of_fblock(inumber, i++, 1)) != 0) {
            printf("Allocating block n°%d on block %d...\n", i, j);
        }

        /* Check if it's actually filled! */
        read_inode(inumber, &inode);
        printf("Checking direct blocks...\n");
        for (i = 0; i < INODE_NB_DIRECT_BLOCKS; i++) {
            printf("%d ", inode.inode_direct[i]);
            if (inode.inode_direct[i] == 0) {
                fprintf(stderr, "/!\\ Direct blocks are not entirely filled!!\n");
                delete_inode(inumber);
                save_super();
                exit(EXIT_FAILURE);
            }
        }

        printf("Checking indirect blocks..\n");
        read_block(volume, inode.inode_indirect, buff);
        for (i = 0; i < INODE_NB_BLOCKS_PER_BLOCK; i++) {
            printf("%d ", buff[i]);
            if (buff[i] == 0) {
                fprintf(stderr, "/!\\ Indirect blocks are not entirely filled!!\n");
                delete_inode(inumber);
                save_super();
                exit(EXIT_FAILURE);
            }
        }

        printf("Checking indirect blocks..\n");
        read_block(volume, inode.inode_double_indirect, buff);
        for (i = 0; i < INODE_NB_BLOCKS_PER_BLOCK; i++) {
            read_block(volume, buff[i], muton);

            for(j = 0; j < INODE_NB_BLOCKS_PER_BLOCK; j++) {
                printf("%d ", muton[i]);
                if (muton[i] == 0) {
                    fprintf(stderr, "/!\\ Double indirect blocks are not entirely filled!!\n");
                    delete_inode(inumber);
                    save_super();
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    return EXIT_SUCCESS;
}