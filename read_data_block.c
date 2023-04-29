#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
int read_data_block(int fd, uint32_t block_address, uint8_t *buffer, int block_size)
{
    if (fd < 0)
    {
        fprintf(stderr, "Invalid file descriptor.\n");
        return -1;
    }

    if (block_size <= 0)
    {
        fprintf(stderr, "Invalid block size.\n");
        return -1;
    }

    if (!buffer)
    {
        fprintf(stderr, "Buffer pointer is NULL.\n");
        return -1;
    }

    off_t block_offset = (off_t)block_address * block_size;
    if (lseek(fd, block_offset, SEEK_SET) == -1)
    {
        fprintf(stderr, "Error seeking to data block: block number: %u, block_offset: %lld\n", block_address, (long long)block_offset);
        return -1;
    }

    ssize_t bytes_read = read(fd, buffer, block_size);
    if (bytes_read != block_size)
    {
        perror("Error reading data block");
        return -1;
    }

    // Print the contents of the data block
    // printf("Contents of data block %u:\n", block_address);
    for (int i = 0; i < block_size; i++)
    {
        // printf("%02x ", buffer[i]);

        if ((i + 1) % 16 == 0)
        {
            // printf("\n");
        }
    }
    // printf("\n");

    return 0;
}
