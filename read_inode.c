#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include "functions.h"

int read_inode(const char *device, int inode_block, int inode_offset, ext3_inode *inode)
{
    int fd = open(device, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening device");
        return -1;
    }

    // Calculate the byte offset of the inode in the device
    off_t byte_offset = (off_t)inode_block * 4096 + inode_offset;

    // Seek to the inode location
    if (lseek(fd, byte_offset, SEEK_SET) == -1)
    {
        perror("Error seeking to inode");
        close(fd);
        return -1;
    }

    // Read the inode into the provided structure
    ssize_t bytes_read = read(fd, inode, sizeof(ext3_inode));
    if (bytes_read != sizeof(ext3_inode))
    {
        perror("Error reading inode");
        close(fd);
        return -1;
    }

    close(fd);

    return 0;
}
