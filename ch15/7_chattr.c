#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

void print_usage(const char *progname) {
    fprintf(stderr, "Usage: %s +/-= [i | a | d] files\n", progname);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  +: Add flags\n");
    fprintf(stderr, "  -: Remove flags\n");
    fprintf(stderr, "  =: Set flags (remove all others and set specified)\n");
}

int modify_flags(const char *filename, unsigned long flags, int set, int remove) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return -1;
    }

    unsigned long current_flags;
    if (ioctl(fd, FS_IOC_GETFLAGS, &current_flags) == -1) {
        perror("Error getting file flags");
        close(fd);
        return -1;
    }

    if (remove) {
        current_flags &= flags; // Add new flags
    }else if (set) {
        current_flags = flags; // Set specified flags only
    }else{
        current_flags |= flags;
    }

    if (ioctl(fd, FS_IOC_SETFLAGS, &current_flags) == -1) {
        perror("Error setting file flags");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

unsigned long get_flags_from_mode(const char *mode) {
    if(strlen(mode) <= 1){
        perror("error mode");
        return EXIT_FAILURE;
    }
    unsigned long flags = 0;

    for (const char *p = mode+1; *p; p++) {
        switch (*p) {
            case 'i':
                flags |= FS_IMMUTABLE_FL;      // Immutable
                break;
            case 'a':
                flags |= FS_APPEND_FL;         // Append only
                break;
            case 'c':
                flags |= FS_COMPR_FL;          // Enable file compression (not implemented)
                break;
            case 'D':
                flags |= FS_DIRSYNC_FL;        // Synchronous directory updates
                break;
            case 'j':
                flags |= FS_JOURNAL_DATA_FL;   // Enable data journaling
                break;
            case 'A':
                flags |= FS_NOATIME_FL;        // Don’t update file last access time
                break;
            case 'd':
                flags |= FS_NODUMP_FL;         // No dump
                break;
            case 't':
                flags |= FS_NOTAIL_FL;         // No tail packing
                break;
            case 's':
                flags |= FS_SECRM_FL;          // Secure deletion (not implemented)
                break;
            case 'S':
                flags |= FS_SYNC_FL;           // Synchronous updates
                break;
            case 'T':
                flags |= FS_TOPDIR_FL;         // Top-level directory for Orlov
                break;
            case 'u':
                flags |= FS_UNRM_FL;           // File can be undeleted (not implemented
            default:
                fprintf(stderr, "Unknown flag: %c\n", *p);
                exit(EXIT_FAILURE);
        }
    }
    return flags;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *mode = argv[1];
    int set = 0;
    int remove = 0;
    unsigned long flags = get_flags_from_mode(mode);

    // Determine operation based on the first character of the mode
    switch (mode[0]) {
        case '+':
            set = 0; // Add flags
            remove = 0;
            break;
        case '-':
            set = 0;
            remove = 1; // Remove flags
            flags = ~flags; // Invert flags for removal
            break;
        case '=':
            set = 1; // Set flags (remove others)
            remove = 0;
            break;
        default:
            print_usage(argv[0]);
            return EXIT_FAILURE;
    }


    for (int i = 2; i < argc; i++){
        const char* filename = argv[i];

        if (modify_flags(filename, flags, set, remove) == -1) {
            fprintf(stderr, "failed to modify flags for '%s'\n", filename);
            continue;
        }
        printf("Updated flags for '%s'\n", filename);
    }
    return EXIT_SUCCESS;
}