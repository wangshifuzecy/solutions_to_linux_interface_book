#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

void print_timestamp(const char *label, struct timespec ts) {
    // Convert timestamp to human-readable format
    char time_buf[100];
    struct tm *tm_info;

    tm_info = localtime(&ts.tv_sec);
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("%s: %s.%09ld\n", label, time_buf, ts.tv_nsec);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    struct stat file_stat;

    // Get file status
    if (stat(filename, &file_stat) == -1) {
        perror("stat");
        return EXIT_FAILURE;
    }

    // Print timestamps with nanosecond precision
    print_timestamp("Access Time (atime)", file_stat.st_atim);
    print_timestamp("Modification Time (mtime)", file_stat.st_mtim);
    print_timestamp("Change Time (ctime)", file_stat.st_ctim);

    return EXIT_SUCCESS;
}