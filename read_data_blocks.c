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
    for (int i = 1; i < 12 && inode->i_block[i] != 0; i++)
    {
        off_t block_offset = (off_t)inode->i_block[i] * block_size;
        if (lseek(fd, block_offset, SEEK_SET) == -1)
        {
            fprintf(stderr, "Error seeking to data block: block number: %u, block_offset: %lld\n", inode->i_block[i], (long long)block_offset);

            close(fd);
            free(file_data->data);
            return -1;
        }

        ssize_t bytes_read = read(fd, file_data->data + i * block_size, block_size);
        printf("Iteration: %d, Bytes read: %zd\n", i, bytes_read);
        printf("Data in this iteration:\n");
        for (int j = 1; j < bytes_read; j++)
        {
            printf("%02x ", (unsigned char)file_data->data[i * block_size + j]);
            if ((j + 1) % 16 == 0)
            {
                printf("\n");
            }
        }
        printf("\n");

        if (bytes_read != block_size)
        {
            perror("Error reading data block");
            close(fd);
            free(file_data->data);
            return -1;
        }

        // Print the contents of the first block
        if (i == 0)
        {
            printf("Contents of the first block:\n");
            for (int j = 0; j < block_size; j++)
            {
                printf("%02x ", (unsigned char)file_data->data[j]);
                if ((j + 1) % 16 == 0)
                {
                    printf("\n");
                }
            }
            printf("\n");
        }
    }

    // Read singly indirect blocks
    if (inode->i_block[12] != 0)
    {
        if (read_singly_indirect_blocks(fd, inode->i_block[12], file_data->data, block_size, inode->i_block[1]) == -1)
        {
            printf("err");
        }
    }
}
