# 5-1
Modify the program in Listing 5-3 to use standard file I/O system calls (open() and lseek()) and the off_t data type. Compile the program with the `_FILE_OFFSET_BITS` macro set to 64, and test it to show that a large file can be successfully created. 
## answer
Test ok. see code in [1_create_file.c](./1_create_file.c)
```
wsf@~/code/linux/interface_book/hw/ch5$ ./1_create_file one_large_file 10111222333
Success
wsf@~/code/linux/interface_book/hw/ch5$ ll one_large_file
-rw------- 1 wsf wsf 10111222337 Sep 29 17:34 one_large_file
```

# 5-2
Write a program that opens an existing file for writing with the O_APPEND flag, and then seeks to the beginning of the file before writing some data. Where does the data appear in the file? Why? 
# answer
For code: see in [2_append.c](./2_append.c)

It creats file and then append text, in the end, remove the file.

The appended text is still shown at the end of file instead of the begining. Because the kernel ensure that the current offset is overridden when `O_APPEND` flag is set.

# 5-3
This exercise is designed to demonstrate why the atomicity guaranteed by opening a file with the O_APPEND flag is necessary. Write a program that takes up to three command-line arguments: 
`$ atomic_append filename num-bytes [x]`
This file should open the specified filename (creating it if necessary) and append num-bytes bytes to the file by using write() to write a byte at a time. By default, the program should open the file with the O_APPEND flag, but if a third command-line argument (x) is supplied, then the O_APPEND flag should be omitted, and instead the File I/O: Further Detai ls 111 program should perform an lseek(fd, 0, SEEK_END) call before each write(). Run two instances of this program at the same time without the x argument to write 1 million bytes to the same file: 
`$ atomic_append f1 1000000 & atomic_append f1 1000000`
Repeat the same steps, writing to a different file, but this time specifying the x argument:
`$ atomic_append f2 1000000 x & atomic_append f2 1000000 x`
List the sizes of the files f1 and f2 using ls –l and explain the difference.
## answer
See code in [3_atomic_append.c](./3_atomic_append.c)
```
wsf@~/code/linux/interface_book/hw/ch5$ ./3_atomic_append f1 1000000 & ./3_atomic_append f1 1000000
[1] 380021
wsf@~/code/linux/interface_book/hw/ch5$ ./3_atomic_append f2 1000000 x  & ./3_atomic_append f2 1000000 x
[1] 381494
[1]+  Done                    ./3_atomic_append f2 1000000 x
wsf@~/code/linux/interface_book/hw/ch5$ ls -l f1 f2
-rw------- 1 wsf wsf 2000000 Sep 29 18:47 f1
-rw------- 1 wsf wsf 1211339 Sep 29 18:48 f2
```
When using `OAPPEND` flag, the writes should be atomic. 
Without `OPPANED` flag, an `lseek()` call was made to the end of file before each write. It may go wrong when multiple processes are involved.

# 5-4
Implement dup() and dup2() using fcntl() and, where necessary, close(). (You may ignore the fact that dup2() and fcntl() return different errno values for some error cases.) For dup2(), remember to handle the special case where oldfd equals newfd. In this case, you should check whether oldfd is valid, which can be done by, for example, checking if fcntl(oldfd, F_GETFL) succeeds. If oldfd is not valid, then the function should return –1 with errno set to EBADF. 
## answer
see in [4_dup.c](./4_dup.c)

The program opens testfile1.txt for reading and writing, writing an initial string, then duplicates its file descriptor to fd2 for additional writing. It also creates anotherfile.txt, duplicates fd1 to fd3, and writes to it.

# 5-5
Write a program to verify that duplicated file descriptors share a file offset value and open file status flags.
## answer
see in [5_check_dup.c](./5_check_dup.c)

# 5-6
After each of the calls to write() in the following code, explain what the content of the output file would be, and why:
```
fd1 = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
fd2 = dup(fd1);
fd3 = open(file, O_RDWR);
write(fd1, "Hello,", 6);
write(fd2, "world", 6);
lseek(fd2, 0, SEEK_SET);
write(fd1, "HELLO,", 6);
write(fd3, "Gidday", 6);
```
## answer
see in [6_test.c](./6_test.c)

6_test will output:
```
after lseek():
off of fd1 0
off of fd2 0
off of fd3 0
in the file, file content:
Giddayworld
```
Since fd2 is one duplication of fd1, they share the offset. After fd2 set offset value 0, the write call using fd1 will write text to the beginning again. `open(file,O_RDWR)` creates file descriptor with 0 offset , so Gridday set at the beginning of sentence.

# 5-7
Implement readv() and writev() using read(), write(), and suitable functions from the malloc package (Section 7.1.2).
## answer
see in [7_rwv.c](./7_rwv.c)
