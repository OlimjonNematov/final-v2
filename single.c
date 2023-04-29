#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Added for memcpy
#include "functions.h"

// Add two new arguments to the function definition
int read_singly_indirect_blocks(int fd, uint32_t indirect_block_num, int block_size, uint8_t *file_data, uint32_t file_size, int direct_blocks_count)
{

    if (fd < 0)
    {
        fprintf(stderr, "Invalid file descriptor.\n");
        return -1;
    }

    if (block_size <= 0 || block_size % sizeof(uint32_t) != 0)
    {
        fprintf(stderr, "Invalid block size.\n");
        return -1;
    }

    uint32_t *indirect_buffer = (uint32_t *)malloc(block_size);
    if (!indirect_buffer)
    {
        perror("Error allocating memory for indirect buffer");
        return -1;
    }

    off_t block_offset = (off_t)indirect_block_num * block_size;
    if (lseek(fd, block_offset, SEEK_SET) == -1)
    {
        fprintf(stderr, "Error seeking to indirect block: block number: %u, block_offset: %lld\n", indirect_block_num, (long long)block_offset);
        free(indirect_buffer);
        return -1;
    }

    ssize_t bytes_read = read(fd, indirect_buffer, block_size);
    if (bytes_read != block_size)
    {
        perror("Error reading indirect block");
        free(indirect_buffer);
        return -1;
    }

    // Iterate through each address in the indirect buffer
    for (int i = 0; i < block_size / sizeof(uint32_t); i++)
    {
        // printf("Address at index %d: %08x\n", i, indirect_buffer[i]);
        if (indirect_buffer[i] == 0)
        {
            break; // Stop iterating when a zero pointer is encountered
        }
        uint8_t *data_block_buffer = (uint8_t *)malloc(block_size);
        if (!data_block_buffer)
        {
            perror("Error allocating memory for data block buffer");
            free(indirect_buffer);
            return -1;
        }

        int result = read_data_block(fd, indirect_buffer[i], data_block_buffer, block_size);
        if (i == 1)
        {
            /* code */
            // printf("Data block at address %08x contents:\n", indirect_buffer[i]);
        }

        for (int j = 0; j < block_size / 10; j++)
        {
            // printf("%02x ", (unsigned char)(data_block_buffer[j]));
            if ((j + 1) % 16 == 0)
            {
                // printf("\n");
            }
        }
        // printf("\n");

        if (result == -1)
        {
            printf("Error reading data block at address %08x\n", indirect_buffer[i]);
        }

        // Update the file_data with the content of the data block
        uint32_t data_offset = (uint32_t)(direct_blocks_count + i) * block_size;

        if (data_offset < file_size)
        {
            uint32_t bytes_to_copy = block_size;

            if (data_offset + block_size > file_size)
            {
                bytes_to_copy = file_size - data_offset;
            }

            memcpy(file_data + data_offset, data_block_buffer, bytes_to_copy);
        }
        else
        {
            printf("Warning: data_offset exceeds file size.\n");
        }

        free(data_block_buffer);
    }

    free(indirect_buffer);
    return 0;
}
