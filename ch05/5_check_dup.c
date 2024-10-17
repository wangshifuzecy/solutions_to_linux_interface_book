#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    // Create a temporary file
    int original_fd = open("temp_file.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (original_fd == -1) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Write some data to the file
    const char *data = "This is a test file.\n";
    write(original_fd, data, strlen(data));

    // Duplicate the file descriptor
    int duplicated_fd = dup(original_fd);
    if (duplicated_fd == -1) {
        perror("Error duplicating file descriptor");
        close(original_fd);
        return EXIT_FAILURE;
    }

    // Get the file offset for both file descriptors
    off_t original_offset = lseek(original_fd, 0, SEEK_CUR);
    off_t duplicated_offset = lseek(duplicated_fd, 0, SEEK_CUR);

    // Get the file status flags for both file descriptors
    int original_flags = fcntl(original_fd, F_GETFL);
    int duplicated_flags = fcntl(duplicated_fd, F_GETFL);

    // Output results
    printf("Original FD: %d, Offset: %ld, Flags: %d\n", original_fd, (long)original_offset, original_flags);
    printf("Duplicated FD: %d, Offset: %ld, Flags: %d\n", duplicated_fd, (long)duplicated_offset, duplicated_flags);

    // Check if both offsets and flags are equal
    if (original_offset == duplicated_offset && original_flags == duplicated_flags) {
        printf("Duplicated file descriptors share the same file offset and status flags.\n");
    } else {
        printf("Duplicated file descriptors do NOT share the same file offset or status flags.\n");
    }

    // Clean up: Close file descriptors
    close(original_fd);
    close(duplicated_fd);
    
    // Remove the temporary file
    remove("temp_file.txt");

    return EXIT_SUCCESS;
}
