#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include "functions.h"

int find_inode_location(const char *device, int inode_number, const ext3_superblock *sb, ext3_group_desc *bgd, int *inode_block, int *inode_offset)
{
    printf("inode number at find-indoe-location: %d \n", inode_number);
    // Calculate the block group that the inode belongs to
    int block_group = (inode_number - 1) / sb->s_inodes_per_group;

    // Read the block group descriptor table to find the inode table's starting block for that block group
    int fd = open(device, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening device");
        return -1;
    }

    int block_size = 1024 << sb->s_log_block_size;
    int bgdt_block = sb->s_first_data_block + 1;
    off_t bgdt_offset = (off_t)bgdt_block * block_size + (off_t)block_group * sizeof(ext3_group_desc);

    if (lseek(fd, bgdt_offset, SEEK_SET) == -1)
    {
        perror("Error seeking to block group descriptor table");
        close(fd);
        return -1;
    }

    ssize_t bytes_read = read(fd, bgd, sizeof(ext3_group_desc));
    if (bytes_read != sizeof(ext3_group_desc))
    {
        perror("Error reading block group descriptor");
        close(fd);
        return -1;
    }

    // Calculate the index of the inode within the block group
    int inode_index = (inode_number - 1) % sb->s_inodes_per_group;

    // Calculate the inode block and inode offset
    *inode_block = bgd->bg_inode_table + (inode_index * sb->s_inode_size) / block_size;
    *inode_offset = (inode_index * sb->s_inode_size) % block_size;

    printf("Inode block: %d\n", *inode_block);
    printf("Inode offset: %d\n", *inode_offset);
    printf("block_size: %d\n", block_size);
    printf("bgdt_block: %d\n", bgdt_block);
    printf("bgdt_offset: %ld\n", bgdt_offset);
    printf("inode_index: %d\n", inode_index);

    close(fd);
    return 0;
}
