#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int read_singly_indirect_blocks(int fd, uint32_t indirect_block_num, uint8_t *buffer, int block_size, int start_block)
{
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

    for (int i = 0; i < block_size / sizeof(uint32_t); i++)
    {
        if (indirect_buffer[i] != 0)
        {
            block_offset = (off_t)indirect_buffer[i] * block_size;
            if (lseek(fd, block_offset, SEEK_SET) == -1)
            {
                fprintf(stderr, "Error seeking to data block: block number: %u, block_offset: %lld\n", indirect_buffer[i], (long long)block_offset);
                free(indirect_buffer);
                return -1;
            }

            bytes_read = read(fd, buffer, block_size);
            if (bytes_read != block_size)
            {
                perror("Error reading data block");
                free(indirect_buffer);
                return -1;
            }

            printf("First 24 bytes of block %u:\n", indirect_buffer[i]);
            for (int j = 0; j < 24; j++)
            {
                printf("%02x ", buffer[j]);
                if ((j + 1) % 16 == 0)
                {
                    printf("\n");
                }
            }
            printf("\n");

            buffer += block_size;
        }
    }

    free(indirect_buffer);
    return 0;
}
