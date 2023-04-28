#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "functions.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <device_path> <inode_number>\n", argv[0]);
        return 1;
    }

    const char *device = argv[1];
    int inode_number = atoi(argv[2]);

    ext3_superblock sb;
    if (read_superblock(device, &sb) == -1)
    {
        printf("Error reading superblock\n");
        return 1;
    }

    // Print some superblock information for debugging purposes
    printf("Inodes count: %u\n", sb.s_inodes_count);
    printf("Blocks count: %u\n", sb.s_blocks_count);
    printf("Inodes per group: %u\n", sb.s_inodes_per_group);
    printf("Blocks per group: %u\n", sb.s_blocks_per_group);
    printf("Inode size: %u\n", sb.s_inode_size);
    printf("First data block: %u\n", sb.s_first_data_block);

    // find inode
    ext3_group_desc bgd;
    int inode_block, inode_offset;
    if (find_inode_location(device, inode_number, &sb, &bgd, &inode_block, &inode_offset) == -1)
    {
        printf("Error finding inode location\n");
        return 1;
    }

    // Read the inode
    ext3_inode inode;
    if (read_inode(device, inode_block, inode_offset, &inode) == -1)
    {
        printf("Error reading inode\n");
        return 1;
    }

    // Read data blocks
    data_blocks file_data;
    if (read_data_blocks(device, &inode, &file_data, &sb) == -1)
    {
        printf("Error reading data blocks\n");
        return 1;
    }

    // Print file size and first few bytes of file data
    printf("File size: %zu bytes\n", file_data.size);
    printf("First few bytes of file data:\n");
    for (size_t i = 0; i < 64 && i < file_data.size; i++)
    {
        printf("%02x ", file_data.data[i]);
    }
    printf("\n");

    // Reconstruct the file
    // ... (save the file_data.data to a file, for example)

    // Free the file data memory
    free(file_data.data);

    // Reconstruct the file
    // ...

    return 0;
}
