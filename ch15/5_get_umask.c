#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

mode_t get_current_umask() {
    // Get the current umask
    mode_t old_umask = umask(0); // Set umask to 0 and get the old umask
    umask(old_umask); // Restore the original umask
    return old_umask; // Return the original umask
}

int main() {
    mode_t current_umask = get_current_umask();
    printf("Current process umask: %04o\n", current_umask); // Print in octal format
    return 0;
}