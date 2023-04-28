#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "functions.h"

int create_file_from_data(const char *file_name, const data_blocks *file_data)
{
    // Validate arguments
    if (file_name == NULL || file_data == NULL)
    {
        fprintf(stderr, "Invalid arguments passed to create_file_from_data.\n");
        return -1;
    }

    // Open a new file for writing
    FILE *file = fopen(file_name, "wb");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file for writing: %s\n", strerror(errno));
        return -1;
    }

    // Write the file_data contents to the new file
    size_t bytes_written = fwrite(file_data->data, 1, file_data->size, file);
    if (bytes_written != file_data->size)
    {
        fprintf(stderr, "Error writing to file: %s\n", strerror(errno));
        fclose(file);
        return -1;
    }

    // Close the file
    fclose(file);

    return 0;
}
