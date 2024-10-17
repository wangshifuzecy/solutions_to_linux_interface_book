#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>

#define BUFFER_SIZE 4096
#define DEFAULT_LINE_NUMBERS 10

// Function to find the last 'n' lines in the file
void tail(int fd, int num_lines) {
    char buffer[BUFFER_SIZE];
    off_t file_size, pos;
    int line_count = 0;
    ssize_t bytes_read;

    // Get the size of the file
    file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1) {
        perror("lseek");
        exit(EXIT_FAILURE);
    }

    // Start reading backwards from the end of the file
    pos = file_size;

    // Keep going backwards until we find the required number of lines
    while (pos > 0 && line_count <= num_lines) {
        // Move the file offset back by BUFFER_SIZE or remaining file size
        size_t to_read = (pos > BUFFER_SIZE) ? BUFFER_SIZE : pos;
        pos -= to_read;

        if (lseek(fd, pos, SEEK_SET) == -1) {
            perror("lseek");
            exit(EXIT_FAILURE);
        }

        // Read from the file
        bytes_read = read(fd, buffer, to_read);
        if (bytes_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        // Count newlines in the buffer
        for (ssize_t i = bytes_read - 1; i >= 0; --i) {
            if (buffer[i] == '\n') {
                line_count++;
                // Stop when we've found the required number of lines
                if (line_count > num_lines) {
                    pos += i + 1; // Move to the start of the line
                    break;
                }
            }
        }
    }

    // Set file descriptor to the final position
    if (lseek(fd, pos, SEEK_SET) == -1) {
        perror("lseek");
        exit(EXIT_FAILURE);
    }

    // Print the remaining lines to stdout
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        if (write(STDOUT_FILENO, buffer, bytes_read) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char *argv[]) {
    int fd;
    int num_lines = DEFAULT_LINE_NUMBERS;
    int opt;

    // Use getopt to parse the command-line options
    while ((opt = getopt(argc, argv, "n:")) != -1) {
        switch (opt) {
            case 'n':
                num_lines = atoi(optarg); // Get the number of lines from the argument
                break;
            case '?':
                if(optopt == 'n'){
                    fprintf(stderr, "Option -%c requires an argument\n", optopt);
                }else if(isprint(optopt)){
                    fprintf(stderr, "Unknown option `-%c'\n", optopt);
                }else{
                    fprintf(stderr, "Unknown option character `\\x%x'\n", optopt);
                }
                return 1;
            default:
                fprintf(stderr, "Usage: %s [-n num] file\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Check if the file is provided
    if (optind >= argc) {
        fprintf(stderr, "Expected file name after options\n");
        exit(EXIT_FAILURE);
    }

    // Open the file
    fd = open(argv[optind], O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Call the tail function
    tail(fd, num_lines);

    // Close the file
    close(fd);

    return 0;
}
