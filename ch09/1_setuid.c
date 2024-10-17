#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void reset_to_init_state() {
    if (setresuid(1000, 0, 0) == -1) {
        perror("Failed to set saved UID to 0");
    }
}

void print_uids(const char *label) {
    uid_t real_uid, effective_uid, saved_uid;

    // Retrieve the real, effective, and saved UIDs
    getresuid(&real_uid, &effective_uid, &saved_uid);

    // Get the file-system UID
    uid_t fs_uid = setfsuid(-1); 

    printf("%s\n", label);
    printf("Real UID: %d, Effective UID: %d, Saved UID: %d, File-system UID: %d\n\n",
        real_uid, effective_uid, saved_uid, fs_uid);
}

int main() {
    print_uids("Begin main");

    // Initialize to the initial state only once
    reset_to_init_state();
    
    // Initial state
    print_uids("Init by setresuid(1000, 0, 0)");

    // Prompt for user input
    char choice;
    printf("Choose a set call to execute:\n");
    printf("a) setuid(2000)\n");
    printf("b) setreuid(-1, 2000)\n");
    printf("c) seteuid(2000)\n");
    printf("d) setfsuid(2000)\n");
    printf("e) setresuid(-1, 2000, 3000)\n");
    printf("Enter your choice (a-e): ");
    scanf(" %c", &choice);  // Read a character input

    // Execute the chosen set call
    switch (choice) {
        case 'a':
            if (setuid(2000) == -1) {
                perror("setuid(2000) failed");
            }
            print_uids("After setuid(2000)");
            break;
        case 'b':
            if (setreuid(-1, 2000) == -1) {
                perror("setreuid(-1, 2000) failed");
            }
            print_uids("After setreuid(-1, 2000)");
            break;
        case 'c':
            if (seteuid(2000) == -1) {
                perror("seteuid(2000) failed");
            }
            print_uids("After seteuid(2000)");
            break;
        case 'd':
            if (setfsuid(2000) == -1) {
                perror("setfsuid(2000) failed");
            }
            print_uids("After setfsuid(2000)");
            break;
        case 'e':
            if (setresuid(-1, 2000, 3000) == -1) {
                perror("setresuid(-1, 2000, 3000) failed");
            }
            print_uids("After setresuid(-1, 2000, 3000)");
            break;
        default:
            printf("Invalid choice. Exiting.\n");
            break;
    }

    return 0;
}
