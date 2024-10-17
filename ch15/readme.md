# 15-1  
Section 15.4 contained several statements about the permissions required for various file-system operations. Use shell commands or write programs to verify or answer the following:  
a) Removing all owner permissions from a file denies the file owner access, even though group and other do have access.  
b) On a directory with read permission but not execute permission, the names of files in the directory can be listed, but the files themselves can’t be accessed, regardless of the permissions on them.  
c) What permissions are required on the parent directory and the file itself in order to create a new file, open a file for reading, open a file for writing, and delete a file? What permissions are required on the source and target directory to rename a file? If the target file of a rename operation already exists, what permissions are required on that file? How does setting the sticky permission bit (`chmod +t`) of a directory affect renaming and deletion operations?

## Answer
(a)
```sh
touch testfile
chmod 077 testfile
ls -l testfile
cat testfile # can't read though you are the group member 
rm -f testfile
```
permission checks against owner, group, and other permissions are done in order, and checking stops as soon as the applicable rule is found. (077 may not allow the owner to read, write and execute though owner belongs to the group which have the permissions)

(b)
```sh
mkdir testdir
touch testdir/testfile
chmod u-x testdir
/bin/ls testdir # testfile will be listed.
cat testdir/testfile # can't be accessed
chmod u+x testdir
rm -rf testdir
```
Execute permission means files within the directory may be accessed. Without it, you can't access the files in the directory. (ls may be alias with options in your shell, so use `/bin/ls`)

(c)

| **Operation**                    | **File Permission** | **Parent Permission**      |
|----------------------------------|---------------------|----------------------------|
| Creating a New File             | -                   | `w`, `x`                   |
| Opening a File for Reading      | `r`                 | `x`                        |
| Opening a File for Writing      | `w`                 | `x`                        |
| Deleting a File                 | -                   | `w`, `x`                   |
| Rename a File | -                   | `w`, `x`  on both source and target                 |

Sticky Bit Effect: one can only remove or rename his files. (If he is the owner of the directory, he can remove or rename any files)


# 15-2  
Do you expect any of a file’s three timestamps to be changed by the `stat()` system call? If not, explain why.

## Answer
`stat()` won't change any timestamps, since it just fetch information.

# 15-3  
On a system running Linux 2.6, modify the program in Listing 15-1 (`t_stat.c`) so that the file timestamps are displayed with nanosecond accuracy.

## Answer
see code in `3_nano_stamp.c`

# 15-4  
The `access()` system call checks permissions using the process’s real user and group IDs. Write a corresponding function that performs the checks according to the process’s effective user and group IDs.

## Answer
see `4_euidaccess.c`.
In fact, you can use `man euidaccess` finding one.

# 15-5  
As noted in Section 15.4.6, `umask()` always sets the process umask and, at the same time, returns a copy of the old umask. How can we obtain a copy of the current process umask while leaving it unchanged?

## Answer
```c
// Get the current umask
mode_t old_umask = umask(0); // Set umask to 0 and get the old umask
umask(old_umask); // Restore the original umask
return old_umask; // Return the original umask
```
see `5_get_umask.c`

# 15-6  
The `chmod a+rX file` command enables read permission for all categories of user, and likewise enables execute permission for all categories of user if `file` is a directory or execute permission is enabled for any of the user categories for `file`, as demonstrated in the following example:
```
$ ls -ld dir file prog
dr-------- 2 mtk users 48 May 4 12:28 dir
-r-------- 1 mtk users 19794 May 4 12:22 file
-r-x------ 1 mtk users 19336 May 4 12:21 prog
$ chmod a+rX dir file prog
$ ls -ld dir file prog
dr-xr-xr-x 2 mtk users 48 May 4 12:28 dir
-r--r--r-- 1 mtk users 19794 May 4 12:22 file
-r-xr-xr-x 1 mtk users 19336 May 4 12:21 prog
```
Write a program that uses `stat()` and `chmod()` to perform the equivalent of `chmod a+rX`.

## Answer
see `6_chmod.c`

check with following code:
```sh
mkdir dir
chmod 400 dir
touch file prog
chmod 400 file
chmod 500 prog
ls -ld dir file prog
gcc 6_chmod.c -o 6_chmod
./6_chmod dir file prog
ls -ld dir file prog
rm -rf file prog dir
```

# 15-7  
Write a simple version of the `chattr(1)` command, which modifies file i-node flags. See the `chattr(1)` man page for details of the `chattr` command-line interface. (You don’t need to implement the `–R`, `–V`, and `–v` options.)

## Answer
see `7_chattr.c` (I don't implement any option.)
You can use `lsattr` to check.