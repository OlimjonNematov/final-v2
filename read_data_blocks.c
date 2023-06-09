#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int read_data_blocks(const char *device, const ext3_inode *inode, data_blocks *file_data, const ext3_superblock *sb)
{
    int block_size = 1024 << sb->s_log_block_size;
    // Validate arguments
    if (device == NULL || inode == NULL || file_data == NULL || sb == NULL)
    {
        fprintf(stderr, "Invalid arguments passed to read_data_blocks.\n");
        return -1;
    }
    file_data->size = inode->i_size;
    file_data->data = malloc(file_data->size);
    if (file_data->data == NULL)
    {
        perror("Error allocating memory for file data");
        return -1;
    }

    // Open the device for reading
    int fd = open(device, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening device");
        free(file_data->data);
        return -1;
    }

    // Read direct blocks
    for (int i = 0; i < 12 && inode->i_block[i + 1] != 0; i++)
    {

        off_t block_offset = (off_t)inode->i_block[i + 1] * block_size;
        if (lseek(fd, block_offset, SEEK_SET) == -1)
        {
            fprintf(stderr, "Error seeking to data block: block number: %u, block_offset: %lld\n", inode->i_block[i], (long long)block_offset);

            close(fd);
            free(file_data->data);
            return -1;
        }

        ssize_t bytes_read = read(fd, file_data->data + i * block_size, block_size);
    }

    int direct_blocks_read = 0;
    for (int i = 0; i < 12 && inode->i_block[i] != 0; i++)
    {
        direct_blocks_read++;
    }

    if (inode->i_block[12] != 0)
    {
        printf("reading indirects \n");
        if (read_singly_indirect_blocks(fd, inode->i_block[13], block_size, file_data->data, file_data->size, direct_blocks_read))
        {
            printf("err");
        }
    }

    // Print first few bytes of file data
    printf("First few bytes of file data:\n");
    for (int i = 0; i < 64 && i < file_data->size; i++)
    {
        printf("%02x ", file_data->data[i]);
    }
    printf("\n");

    close(fd);
}
