//I use create_holes.c create one file with holes named file_with_holes.bin. (I use fseek create hole.But the easiest way to get one file with holes is just using one ELF file(e.g., gcc a.c)
//you can use du(actul disk usage,unit:kilobyte) and du -b(logical file size, unit: byte) check files with holes.
//In addition, one file is at least 4 kb as usual for disk usage.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 4096 // Size of the buffer to read/write

void copy_file_with_holes(const char *source_path, const char *target_path) {
    FILE *source_file = fopen(source_path, "rb"); // Open source file for reading
    if (source_file == NULL) {
        fprintf(stderr, "Error opening source file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Open target file for writing
    FILE *target_file = fopen(target_path, "wb"); 
    if (target_file == NULL) {
        fprintf(stderr, "Error opening target file: %s\n", strerror(errno));
        fclose(source_file);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE]; // Buffer for reading data
    size_t bytes_read;

    // Copy data from source to target
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), source_file)) > 0) {
        // Write the read data to the target file
        fwrite(buffer, 1, bytes_read, target_file);
    }

    // Check for read error
    if (ferror(source_file)) {
        fprintf(stderr, "Error reading source file: %s\n", strerror(errno));
    }

    // Close the files
    fclose(source_file);
    fclose(target_file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <target>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    copy_file_with_holes(argv[1], argv[2]); // Call the copy function

    return EXIT_SUCCESS;
}
