#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/ipc.h>

int main() {
    uid_t ruid, euid, suid;

    // Get the current real, effective, and saved IDs
    if (getresuid(&ruid, &euid, &suid) == -1) {
        perror("getresuid");
        return 1;
    }

    printf("Before suspension: RUID=%d, EUID=%d, SUID=%d\n", ruid, euid, suid);

    // Suspend privileges: set the effective user ID to the real user ID
    if (seteuid(ruid) == -1) {
        perror("seteuid");
        return 1;
    }

    printf("After suspension: RUID=%d, EUID=%d, SUID=%d\n", ruid, geteuid(), suid);

    // Resume privileges: set the effective user ID back to the saved user ID
    if (seteuid(suid) == -1) {
        perror("seteuid");
        return 1;
    }

    printf("After resuming: RUID=%d, EUID=%d, SUID=%d\n", ruid, geteuid(), suid);

    // Permanently drop privileges by setting the real, effective, and saved IDs to RUID
    if (setresuid(ruid, ruid, ruid) == -1) {
        perror("setresuid");
        return 1;
    }

    // Get the current real, effective, and saved IDs
    if (getresuid(&ruid, &euid, &suid) == -1) {
        perror("getresuid");
        return 1;
    }

    printf("After dropping privileges: RUID=%d, EUID=%d, SUID=%d\n", ruid, euid, suid);

    return 0;
}
