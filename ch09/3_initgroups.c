#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <sys/types.h>
#include <errno.h>

int initgroups(const char *user, gid_t basegid) {
    struct passwd *pw;
    struct group *gr;
    gid_t *groups = NULL;
    int ngroups = 0;
    int i;

    // Get user information
    pw = getpwnam(user);
    if (pw == NULL) {
        perror("getpwnam");
        return -1;
    }

    // Initialize the groups array with the base GID
    ngroups = 1; // We have at least the base GID
    groups = malloc(sizeof(gid_t) * ngroups);
    if (groups == NULL) {
        perror("malloc");
        return -1;
    }
    groups[0] = basegid;

    // Iterate over the group entries
    setgrent(); // Rewind the group file
    while ((gr = getgrent()) != NULL) {
        // Check if the user is in the current group
        for (i = 0; gr->gr_mem[i] != NULL; i++) {
            if (strcmp(gr->gr_mem[i], pw->pw_name) == 0) {
                // Reallocate groups array to add new group
                groups = realloc(groups, sizeof(gid_t) * (ngroups + 1));
                if (groups == NULL) {
                    perror("realloc");
                    endgrent();
                    return -1;
                }
                groups[ngroups++] = gr->gr_gid;
                break; // No need to check further in this group
            }
        }
    }
    endgrent();

    // Set the supplementary groups
    if (setgroups(ngroups, groups) == -1) {
        perror("setgroups");
        free(groups);
        return -1;
    }

    free(groups);
    return 0; // Success
}
