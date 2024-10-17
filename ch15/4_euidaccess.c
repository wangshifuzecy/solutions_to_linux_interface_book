#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int effective_access(const char *path, int mode) {
    struct stat file_stat;

    // Get file status
    if (stat(path, &file_stat) == -1) {
        return -1; // Return -1 on error (e.g., file doesn't exist)
    }

    // Get effective user and group IDs
    uid_t effective_uid = geteuid();
    gid_t effective_gid = getegid();

    // Check permissions
    if (mode & R_OK) { // Check read permission
        if (file_stat.st_uid == effective_uid) {
            // Owner has read permission
            if (!(file_stat.st_mode & S_IRUSR)) return -1;
        } else if (file_stat.st_gid == effective_gid) {
            // Group has read permission
            if (!(file_stat.st_mode & S_IRGRP)) return -1;
        } else {
            // Others have read permission
            if (!(file_stat.st_mode & S_IROTH)) return -1;
        }
    }

    if (mode & W_OK) { // Check write permission
        if (file_stat.st_uid == effective_uid) {
            // Owner has write permission
            if (!(file_stat.st_mode & S_IWUSR)) return -1;
        } else if (file_stat.st_gid == effective_gid) {
            // Group has write permission
            if (!(file_stat.st_mode & S_IWGRP)) return -1;
        } else {
            // Others have write permission
            if (!(file_stat.st_mode & S_IWOTH)) return -1;
        }
    }

    if (mode & X_OK) { // Check execute permission
        if (file_stat.st_uid == effective_uid) {
            // Owner has execute permission
            if (!(file_stat.st_mode & S_IXUSR)) return -1;
        } else if (file_stat.st_gid == effective_gid) {
            // Group has execute permission
            if (!(file_stat.st_mode & S_IXGRP)) return -1;
        } else {
            // Others have execute permission
            if (!(file_stat.st_mode & S_IXOTH)) return -1;
        }
    }

    // If all checks passed, return 0
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <mode>\n", argv[0]);
        fprintf(stderr, "Mode: 4 for read, 2 for write, 1 for execute (use sum for multiple)\n");
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    int mode = atoi(argv[2]);

    if (effective_access(filename, mode) == 0) {
        printf("Access granted.\n");
    } else {
        printf("Access denied\n");
    }

    return EXIT_SUCCESS;
}