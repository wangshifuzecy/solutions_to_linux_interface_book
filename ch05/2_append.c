#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FILENAME "testfile.txt"
#define INITIAL_TEXT "This is the initial text.\n"
#define APPEND_TEXT "This is appended text.\n"

void display_file_content(const char *filename) {
    char buffer[1024];
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("fopen");
        return;
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    fclose(file);
}

int main() {
    int fd;

    // Step 1: Create and write initial text to the file
    fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (write(fd, INITIAL_TEXT, strlen(INITIAL_TEXT)) == -1) {
        perror("write");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);

    // Step 2: Display current content of the file
    printf("content of init file:\n");
    display_file_content(FILENAME);

    // Step 3: Open the file for appending
    fd = open(FILENAME, O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Seek to the beginning of the file (this will be ignored due to O_APPEND)
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("lseek");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Write the new text
    if (write(fd, APPEND_TEXT, strlen(APPEND_TEXT)) == -1) {
        perror("write");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);

    // Step 4: Display updated content of the file
		printf("content of file after operating:\n");
    display_file_content(FILENAME);

    // Step 5: Remove the file
    if (remove(FILENAME) != 0) {
        perror("remove");
        exit(EXIT_FAILURE);
    }

    printf("File %s removed successfully.\n", FILENAME);

    return 0;
}
