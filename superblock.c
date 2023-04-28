#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include "functions.h"

int read_superblock(const char *device, ext3_superblock *sb)
{
    int fd = open(device, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening device");
        return -1;
    }

    // Default superblock offset is 1024 bytes
    if (lseek(fd, 1024, SEEK_SET) == -1)
    {
        perror("Error seeking to superblock");
        close(fd);
        return -1;
    }

    // Read the superblock into the provided structure
    ssize_t bytes_read = read(fd, sb, sizeof(ext3_superblock));
    printf("bytes read: %ld\n", bytes_read);
    if (bytes_read != sizeof(ext3_superblock))
    {
        perror("Error reading superblock");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}
