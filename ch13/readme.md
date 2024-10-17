# 13-1. 
Using the time built-in command of the shell, try timing the operation of the program in Listing 4-1 (copy.c) on your system.  
a) Experiment with different file and buffer sizes. You can set the buffer size using the `–DBUF_SIZE=nbytes` option when compiling the program.  
b) Modify the `open()` system call to include the `O_SYNC` flag. How much difference does this make to the speed for various buffer sizes?  
c) Try performing these timing tests on a range of file systems (e.g., ext3, XFS, Btrfs, and JFS). Are the results similar? Are the trends the same when going from small to large buffer sizes?  

## Answer
create file of different size.
```sh
fallocate -l 1K file_1K
fallocate -l 10K file_10K
fallocate -l 100K file_100K
fallocate -l 1M file_1M
fallocate -l 10M file_10M
fallocate -l 10M file_100M
```

create executable files with different buffer size.
```sh
gcc -I ../lib -DBUF_SIZE=1024 -o ./copy_1024 copy.c ../libtlpi.a
gcc -I ../lib -DBUF_SIZE=2048 -o ./copy_2048 copy.c ../libtlpi.a
gcc -I ../lib -DBUF_SIZE=4096 -o ./copy_4096 copy.c ../libtlpi.a
```

copy `copy.c` to `copy_sync.c` and modify the open flag. (add `O_SYNC`)
```sh
gcc -I ../lib -DBUF_SIZE=1024 -o ./copy_sync_1024 copy_sync.c ../libtlpi.a
gcc -I ../lib -DBUF_SIZE=2048 -o ./copy_sync_2048 copy_sync.c ../libtlpi.a
gcc -I ../lib -DBUF_SIZE=4096 -o ./copy_sync_4096 copy_sync.c ../libtlpi.a
```

(a)
see `1_a.sh`, using `date +%s.%N` measure really time (it's not reliable but using `time` is not precise considering the file is small and `time` is accurate only to three decimal places.)
```
Executable      File            Avg Real (seconds)  
----------      -----           ---------------------
./copy_1024     file_1K         .0044328            
./copy_1024     file_10K        .0018104            
./copy_1024     file_100K       .0020327            
./copy_1024     file_1M         .0040528            
./copy_1024     file_10M        .0184860            
./copy_1024     file_100M       .0190748            

./copy_2048     file_1K         .0018928            
./copy_2048     file_10K        .0018757            
./copy_2048     file_100K       .0018697            
./copy_2048     file_1M         .0030842            
./copy_2048     file_10M        .0134006            
./copy_2048     file_100M       .0115551            

./copy_4096     file_1K         .0017614            
./copy_4096     file_10K        .0017902            
./copy_4096     file_100K       .0018320            
./copy_4096     file_1M         .0032172            
./copy_4096     file_10M        .0127414            
./copy_4096     file_100M       .0117982  
```

(b)
see `1_b.sh`. Time become much longer.
```
Executable      File            Avg Real (seconds)  
----------      -----           ---------------------
./copy_sync_1024 file_1K         .0031904            
./copy_sync_1024 file_10K        .0111277            
./copy_sync_1024 file_100K       .0845877            
./copy_sync_1024 file_1M         .8364746            

./copy_sync_2048 file_1K         .0025757            
./copy_sync_2048 file_10K        .0058506            
./copy_sync_2048 file_100K       .0419850            
./copy_sync_2048 file_1M         .4430773            

./copy_sync_4096 file_1K         .0027449            
./copy_sync_4096 file_10K        .0047130            
./copy_sync_4096 file_100K       .0213600            
./copy_sync_4096 file_1M         .2021807  
```

(c)
see `1_c.sh`
```
Executable      File            Avg Real (seconds)   Filesystem
----------      -----           --------------------- ----------
./copy_1024     file_1K         .0017068             ext2
./copy_1024     file_1K         .0017065             ext3
./copy_1024     file_1K         .0017624             ext4
./copy_1024     file_1K         .0026192             xfs

./copy_1024     file_10K        .0017544             ext2
./copy_1024     file_10K        .0017028             ext3
./copy_1024     file_10K        .0018114             ext4
./copy_1024     file_10K        .0020116             xfs

./copy_1024     file_100K       .0019753             ext2
./copy_1024     file_100K       .0020812             ext3
./copy_1024     file_100K       .0018907             ext4
./copy_1024     file_100K       .0022225             xfs

./copy_1024     file_1M         .0043487             ext2
./copy_1024     file_1M         .0053418             ext3
./copy_1024     file_1M         .0035804             ext4
./copy_1024     file_1M         .0045825             xfs

./copy_1024     file_10M        .0223922             ext2
./copy_1024     file_10M        .0241432             ext3
./copy_1024     file_10M        .0139390             ext4
./copy_1024     file_10M        .0249876             xfs

./copy_1024     file_100M       .0250920             ext2
./copy_1024     file_100M       .0243476             ext3
./copy_1024     file_100M       .0147491             ext4
./copy_1024     file_100M       .0251454             xfs


./copy_2048     file_1K         .0015183             ext2
./copy_2048     file_1K         .0016928             ext3
./copy_2048     file_1K         .0017899             ext4
./copy_2048     file_1K         .0037581             xfs

./copy_2048     file_10K        .0017240             ext2
./copy_2048     file_10K        .0017679             ext3
./copy_2048     file_10K        .0017339             ext4
./copy_2048     file_10K        .0056841             xfs

./copy_2048     file_100K       .0018999             ext2
./copy_2048     file_100K       .0046074             ext3
./copy_2048     file_100K       .0018470             ext4
./copy_2048     file_100K       .0044522             xfs

./copy_2048     file_1M         .0034671             ext2
./copy_2048     file_1M         .0037830             ext3
./copy_2048     file_1M         .0027464             ext4
./copy_2048     file_1M         .0103244             xfs

./copy_2048     file_10M        .0116184             ext2
./copy_2048     file_10M        .0123047             ext3
./copy_2048     file_10M        .0164225             ext4
./copy_2048     file_10M        .0209325             xfs

./copy_2048     file_100M       .0143218             ext2
./copy_2048     file_100M       .0118848             ext3
./copy_2048     file_100M       .0111678             ext4
./copy_2048     file_100M       .0180561             xfs


./copy_4096     file_1K         .0017097             ext2
./copy_4096     file_1K         .0016370             ext3
./copy_4096     file_1K         .0016796             ext4
./copy_4096     file_1K         .0025757             xfs

./copy_4096     file_10K        .0016217             ext2
./copy_4096     file_10K        .0016953             ext3
./copy_4096     file_10K        .0017155             ext4
./copy_4096     file_10K        .0019433             xfs

./copy_4096     file_100K       .0019077             ext2
./copy_4096     file_100K       .0019057             ext3
./copy_4096     file_100K       .0018062             ext4
./copy_4096     file_100K       .0021742             xfs

./copy_4096     file_1M         .0033037             ext2
./copy_4096     file_1M         .0042775             ext3
./copy_4096     file_1M         .0026091             ext4
./copy_4096     file_1M         .0036000             xfs

./copy_4096     file_10M        .0143534             ext2
./copy_4096     file_10M        .0145230             ext3
./copy_4096     file_10M        .0107272             ext4
./copy_4096     file_10M        .0165483             xfs

./copy_4096     file_100M       .0136336             ext2
./copy_4096     file_100M       .0136020             ext3
./copy_4096     file_100M       .0110091             ext4
./copy_4096     file_100M       .0164413             xfs
```
- ext4 performs much better than other three file system when file >= 1MB. 
- XFS performs the worst. (It may be caused by my setting. I don't know.)

# 13-2. 
Time the operation of the `filebuff/write_bytes.c` program (provided in the source code distribution for this book) for various buffer sizes and file systems.  

## Answer
`write_bytes` is getting from `../tlpi-dist/filebuff`.
Usage:
`./write_bytes file num-bytes buf-size`

Timer code see `2_time.sh` (you should check the location of `write_bytes` and modify the `file_system` var), testing on create one 100MB file.

```
File System     Buffer Size Avg Real   Avg User   Avg Sys
------------    ----------- ---------- ---------  -------
/mnt/ext2       512        .1002      .0108      .0892
/mnt/ext2       1024       .0568      .0054      .0514
/mnt/ext2       2048       .0326      .0026      .0298
/mnt/ext2       4096       .0224      .0018      .0202
/mnt/ext2       8192       .0190      .0004      .0182
/mnt/ext2       16384      .0176      .0008      .0168
/mnt/ext2       32768      .0160      0          .0156

/mnt/ext3       512        .1148      .0076      .1070
/mnt/ext3       1024       .0620      .0036      .0580
/mnt/ext3       2048       .0360      .0020      .0338
/mnt/ext3       4096       .0242      .0012      .0228
/mnt/ext3       8192       .0200      .0014      .0184
/mnt/ext3       16384      .0180      .0006      .0172
/mnt/ext3       32768      .0166      0          .0164

/mnt/xfs        512        .1108      .0084      .1024
/mnt/xfs        1024       .0610      .0062      .0542
/mnt/xfs        2048       .0340      .0028      .0304
/mnt/xfs        4096       .0230      .0020      .0208
/mnt/xfs        8192       .0174      .0006      .0168
/mnt/xfs        16384      .0154      .0008      .0146
/mnt/xfs        32768      .0130      .0004      .0126

/mnt/ext4       512        .0550      .0086      .0462
/mnt/ext4       1024       .0316      .0064      .0250
/mnt/ext4       2048       .0198      .0014      .0182
/mnt/ext4       4096       .0152      .0008      .0136
/mnt/ext4       8192       .0148      .0012      .0136
/mnt/ext4       16384      .0134      .0010      .0120
/mnt/ext4       32768      .0126      0          .0126
```

# 13-3. 
What is the effect of the following statements?  
```c
fflush(fp);
fsync(fileno(fp));
```  

## Answer
`fflush(fp);` flushes the output buffer associated with the file stream `fp`. It forces any buffered data to be written to the underlying file descriptor.

`fsync(fileno(fp));` synchronizes the file referred to by the file descriptor obtained from `fileno(fp)`, ensuring all data (including data that still be in the system's cache) is physically written to the disk.

# 13-4. 
Explain why the output of the following code differs depending on whether standard output is redirected to a terminal or to a disk file.  
```c
printf("If I had more time, \n");
write(STDOUT_FILENO, "I would have written you a shorter letter.\n", 43);
```  

## Answer
Standard output to terminal is line-buffered while standard output to a disk file is block-buffered. 

When outputing to the terminal, `printf` will flush the output when it encounter a newline.

When redirecting output to a file, `printf` doesn't flush the output immediately. The buffered data stay in memory until the buffer is full or the the program exits (in this case, after the `write()` call). The `write()` write without any buffering, so it output appears immediately in the file. So in the end, the output file's first line is the string of write's argument, followed by the argument of `printf`.

See `../tlpi_dist/filebuff/mix23_linebuff.c`

result:
```
$ ./mix23_linebuff 
If I had more time, 
I would have written you a shorter letter.
$ ./mix23_linebuff > output
$ cat output 
I would have written you a shorter letter.
If I had more time, 
```

# 13-5. 
The command `tail [ –n num ] file` prints the last `num` lines (ten by default) of the named file. Implement this command using I/O system calls (`lseek()`, `read()`, `write()`, and so on). Keep in mind the buffering issues described in this chapter, in order to make the implementation efficient.  

## Answer
see `5_tail.c`