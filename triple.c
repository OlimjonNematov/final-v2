#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int read_triply_indirect_blocks(int fd, uint32_t triply_indirect_block_num, uint8_t *buffer, int block_size)
{
    // Read the block containing the triply indirect pointers and store it in a buffer
    // For each non-zero pointer in the buffer, call the 'read_doubly_indirect_blocks' function with the pointer value as the 'doubly_indirect_block_num'

    // Return 0 if successful, -1 if an error occurs
}
