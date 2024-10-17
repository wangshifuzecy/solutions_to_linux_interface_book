#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

void apply_chmod(const char *path) {
    struct stat file_stat;

    // Get file status
    if (stat(path, &file_stat) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    // Prepare new mode
    mode_t new_mode = file_stat.st_mode;

    // Add read permissions for all
    new_mode |= S_IRUSR | S_IRGRP | S_IROTH;

    // Add execute permissions for all if it's a directory or if the owner has execute permission
    if (S_ISDIR(file_stat.st_mode) || (file_stat.st_mode & S_IXUSR)) {
        new_mode |= S_IXUSR | S_IXGRP | S_IXOTH;
    }

    // Change the mode
    if (chmod(path, new_mode) == -1) {
        perror("chmod");
        exit(EXIT_FAILURE);
    }

    printf("Updated permissions for '%s'\n", path);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> [file2 ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        apply_chmod(argv[i]);
    }

    return EXIT_SUCCESS;
}