#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/acl.h>
#include <sys/types.h>
#include <unistd.h>

#include "ugid_functions.c"

// Function to display ACL permissions for the user or group
void displayACLRights(const char *path, int isUser, const char *name) {
  acl_t acl = acl_get_file(path, ACL_TYPE_ACCESS);
  if (acl == NULL) {
    perror("Error fetching ACL");
    return;
  }

  int name_found = 0;  // argument name is found in entry
  int mask_found = 0;  // is mask found (used in while loop)
  acl_entry_t entry;
  acl_tag_t tag;
  acl_permset_t mask_permset = NULL;
  acl_permset_t permset;
  int entry_id = ACL_FIRST_ENTRY;

  uid_t uid = -1;
  gid_t gid = -1;

  if (isdigit(name[0])) {
    if (isUser) {
      uid = (uid_t)atoi(name);  // Convert string to uid_t
    } else {
      gid = (gid_t)atoi(name);  // Convert string to gid_t
    }
  } else {
    if (isUser) {
      uid = userIdFromName(name);
      if (uid == -1) {
        fprintf(stderr, "Invalid user: %s\n", name);
        return;
      }
    } else {
      gid = groupIdFromName(name);
      if (gid == -1) {
        fprintf(stderr, "Invalid group: %s\n", name);
        return;
      }
    }
  }

  while (acl_get_entry(acl, entry_id, &entry) == 1) {
    acl_get_tag_type(entry, &tag);

    if (tag == ACL_MASK) {
      acl_get_permset(entry, &mask_permset);
      mask_found = 1;
      continue;
    }

    if ((isUser && tag == ACL_USER) || (!isUser && tag == ACL_GROUP)) {
      if ((isUser && uid == *(uid_t *)acl_get_qualifier(entry)) ||
          (!isUser && gid == *(gid_t *)acl_get_qualifier(entry))) {
        name_found = 1;
        acl_get_permset(entry, &permset);
      }
    }

    if (name_found && mask_found){
      break;
    }
    entry_id = ACL_NEXT_ENTRY;
  }

  //if don't find ACL entry corresponding to the user name or group name 
  if (!name_found) {
    if (isUser) {
      printf("User %s doesn't have ACL_USER entry on this file\n", name);
    } else {
      printf("Group %s doesn't have ACL_GROUP entry on this file\n", name);
    }
  } else {
    // if find ACL_USER or ACL_GROUP , there must exist ACL_MASK
    //  permission is masked, so use `&&`
    if (isUser) {
      printf("User %s has permissions: ", name);
    } else {
      printf("Group %s has permissions: ", name);
    }
    if (acl_get_perm(permset, ACL_READ) && acl_get_perm(mask_permset, ACL_READ)) {
      printf("r");
    }else{
      printf("-");
    }
    if (acl_get_perm(permset, ACL_WRITE) && acl_get_perm(mask_permset, ACL_READ)) {
      printf("w");
    }else{
      printf("-");
    }
    if (acl_get_perm(permset, ACL_EXECUTE) && acl_get_perm(mask_permset, ACL_EXECUTE)) {
      printf("x");
    }else{
      printf("-");
    }
    printf("\n");
  }

  acl_free(acl);
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s <u|g> <user|group> <filename>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int isUser = (strcmp(argv[1], "u") == 0);
  if (!isUser && strcmp(argv[1], "g") != 0) {
    fprintf(stderr, "Invalid option. Use 'u' for user or 'g' for group.\n");
    exit(EXIT_FAILURE);
  }

  const char *name = argv[2];
  const char *filename = argv[3];

  displayACLRights(filename, isUser, name);

  return 0;
}
