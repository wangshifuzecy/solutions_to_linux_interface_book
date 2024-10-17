#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define FILENAME "file_with_holes.bin"
#define HOLE_SIZE 4096// Size of the hole in bytes

void create_file_with_holes(const char *filename) {
    // Open file for writing (creates if not exists)
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Write some data to the file
    const char *data = "This is some data written to the file.\n";
    size_t data_length = strlen(data);

    // Write first chunk of data
    fwrite(data, 1, data_length, file);

    // Create a hole by seeking forward
    fseek(file, HOLE_SIZE, SEEK_CUR); // Move the file position indicator forward

    // Write more data after the hole
    fwrite(data, 1, data_length, file);

    // Close the file
    fclose(file);
}

int main() {
    create_file_with_holes(FILENAME);
    printf("File '%s' created with holes.\n", FILENAME);
    return EXIT_SUCCESS;
}
