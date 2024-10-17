#include <stdio.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>

struct passwd *my_getpwnam(const char *name) {
    struct passwd *pwd;

    // Open the password database
    setpwent();

    // Iterate through the entries in the password file
    while ((pwd = getpwent()) != NULL) {
        // Compare the name with the current entry
        if (strcmp(pwd->pw_name, name) == 0) {
            // If we find the user, break out of the loop and return the entry
            break;
        }
    }

    // Close the password database
    endpwent();

    // Return the found entry (or NULL if not found)
    return pwd;
}

int main() {
    const char *username = "root";  // Replace with any username to search
    struct passwd *pwd_entry = my_getpwnam(username);

    if (pwd_entry) {
        printf("User found:\n");
        printf("Name: %s\n", pwd_entry->pw_name);
        printf("UID: %d\n", pwd_entry->pw_uid);
        printf("GID: %d\n", pwd_entry->pw_gid);
        printf("Home directory: %s\n", pwd_entry->pw_dir);
        printf("Shell: %s\n", pwd_entry->pw_shell);
    } else {
        printf("User '%s' not found.\n", username);
    }

    return 0;
}
