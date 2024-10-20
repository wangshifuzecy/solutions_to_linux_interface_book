# 17-1

Write a program that displays the permissions from the ACL entry that corresponds to a particular user or group. The program should take two command-line arguments. The first argument is either of the letters u or g, indicating whether the second argument identifies a user or group. (The functions defined in Listing 8-1, on page 159, can be used to allow the second command-line argument to be specified numerically or as a name.) If the ACL entry that corresponds to the given user or group falls into the group class, then the program should additionally display the permissions that would apply after the ACL entry has been modified by the ACL mask entry

## Answer

see code in [acl_entry.c](./acl_entry.c) (I add filename as the third command-line argument)

[ugid_functions.c](./ugid_functions.c) and [ugid_functions.h](./ugid_functions.c) are copyed from `../tlpi-dist/users_groups/`

`-lacl` is necessary:
```sh
cc acl_entry.c -lacl -o acl_entry
```
