#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int read_doubly_indirect_blocks(int fd, uint32_t doubly_indirect_block_num, uint8_t *buffer, int block_size)
{
    // Read the block containing the doubly indirect pointers and store it in a buffer
    // For each non-zero pointer in the buffer, call the 'read_singly_indirect_blocks' function with the pointer value as the 'indirect_block_num'

    // Return 0 if successful, -1 if an error occurs
}
