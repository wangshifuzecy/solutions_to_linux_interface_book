# 9-1

Assume in each of the following cases that the initial set of process user IDs is real=1000 effective=0 saved=0 file-system=0. What would be the state of the user IDs after the following calls?

```
a) setuid(2000);
b) setreuid(–1, 2000);
c) seteuid(2000);
d) setfsuid(2000);
e) setresuid(–1, 2000, 3000);
```

## answer

| Call                          | Real UID | Effective UID | Saved UID | File-system UID |
| ----------------------------- | -------- | ------------- | --------- | --------------- |
| Initial state                 | 1000     | 0             | 0         | 0               |
| `setuid(2000)`              | 2000     | 2000          | 2000      | 2000            |
| `setreuid(-1, 2000)`        | 1000     | 2000          | 2000      | 2000            |
| `seteuid(2000)`             | 1000     | 2000          | 0         | 2000            |
| `setfsuid(2000)`            | 1000     | 0             | 0         | 2000            |
| `setresuid(-1, 2000, 3000)` | 1000     | 2000          | 3000      | 2000            |

You can practice it by compiling and running [1_setuid.c](./1_setuid.c) with `sudo `(input one char from a to e which is related to one function call).

notes:
![alt text](image.png)

- whenever the effective user(gorup) ID is changed, the Linux-specific file system ID will change.

# 9-2

Is a process with the following user IDs privileged? Explain your answer.
`real=0 effective=1000 saved=1000 file-system=1000`

## answer

Such a process is unprivileged since its effective user ID is nonzero. But it can set its effective user ID to the same value of real user ID and regain privileged.

# 9-3

Implement initgroups() using setgroups() and library functions for retrieving information from the password and group files (Section 8.4). Remember that a process must be privileged in order to be able to call setgroups().

## answer

see `3_initgroups.c`

# 9-4

If a process whose user IDs all have the value X executes a set-user-ID program whose user ID, Y, is nonzero, then the process credentials are set as follows: `real=X effective=Y saved=Y`

(We ignore the file-system user ID, since it tracks the effective user ID.) Show the setuid(), seteuid(), setreuid(), and setresuid() calls, respectively, that would be used to perform the following operations:

1. Suspend and resume the set-user-ID identity (i.e., switch the effective user ID to the value of the real user ID and then back to the saved set-user-ID).
2. Permanently drop the set-user-ID identity (i.e., ensure that the effective user ID and the saved set-user-ID are set to the value of the real user ID).

(This exercise also requires the use of getuid() and geteuid() to retrieve the process’s real and effective user IDs.) Note that for certain of the system calls listed above, some of these operations can’t be performed

## answer
```c
e = geteuid(); /* Save initial value of effective user ID */
setuid(getuid()); /* Suspend privileges */
setuid(e); /* Resume privileges */
/* Can't permanently drop the set-user-ID identity with setuid() */
/* Because set-user-ID doesn't  change*/

seteuid(getuid()); /* Suspend privileges */
seteuid(e); /* Resume privileges */
/* Can't permanently drop the set-user-ID identity with seteuid() */
/* Because set-user-ID doesn't  change*/

setreuid(-1, getuid()); /* Temporarily drop privileges */
setreuid(-1, e); /* Resume privileges */
setreuid(getuid(), getuid()); /* Permanently drop privileges */

setresuid(-1, getuid(), -1); /* Temporarily drop privileges */
setresuid(-1, e, -1); /* Resume privileges */
setresuid(getuid(), getuid(), getuid()); /* Permanently drop privileges */
```
- In `setreuid` and `setregid`: If `r` (real user or group ID) is not `-1`, or `e` (effective user or group ID) is changed from its current value, both the effective ID and saved set-user-ID or saved set-group-ID are updated to the new effective ID.(SUSv3 no specify)


Implementation code in [4_set.c](./4_set.c), you can modify the function calls of suspension, resuming and dropping

run it with following command, 1000 is uid which start the program and 1001 is uid of another user.
```
$ sudo chown another_user 4_set
$ sudo chmod u+s 4_set
$ ./4_set 
Before suspension: RUID=1000, EUID=1001, SUID=1001
After suspension: RUID=1000, EUID=1000, SUID=1001
After resuming: RUID=1000, EUID=1001, SUID=1001
After dropping privileges: RUID=1000, EUID=1000, SUID=1000
```

# 9-5

Repeat the previous exercise for a process executing a set-user-ID-root program, which has the following initial set of process credentials: `real=X effective=0 saved=0`

## answer

Different from answer in 9-4, for `setuid(getuid())`, privileged process could permanently drop privileges. 
- When a privileged process (running as root) calls setuid() with a non-root UID, all three UIDs (real, effective, and saved) are set to that new UID.

Implementation code same to 9-4, you can modify the function calls of suspension, resuming and dropping
```
$ sudo chown root 4_set
$ sudo chmod u+s 4_set
$ ./4_set 
Before suspension: RUID=1000, EUID=0, SUID=0
After suspension: RUID=1000, EUID=1000, SUID=0
After resuming: RUID=1000, EUID=0, SUID=0
After dropping privileges: RUID=1000, EUID=1000, SUID=1000
```
