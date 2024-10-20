# 14-1.  
Write a program that measures the time required to create and then remove a large number of 1-byte files from a single directory. The program should create files with names of the form `xNNNNNN`, where `NNNNNN` is replaced by a random six-digit number. The files should be created in the random order in which their names are generated, and then deleted in increasing numerical order (i.e., an order that is different from that in which they were created). The number of files (`NF`) and the directory in which they are to be created should be specifiable on the command line. Measure the times required for different values of `NF` (e.g., in the range from 1000 to 20,000) and for different file systems (e.g., `ext2`, `ext3`, and `XFS`). What patterns do you observe on each file system as `NF` increases? How do the various file systems compare? Do the results change if the files are created in increasing numerical order (`x000001`, `x000001`, `x0000002`, and so on) and then deleted in the same order? If so, what do you think the reason(s) might be? Again, do the results vary across file-system types?

## Answer
Result: 
- The delete time have ups and downs when the number of files increase and have differences between file systems. 

TODO: 
- It need further work (e.g. running in isolation, pinning threads and using real-time scheduling policies) to eliminate the impact of scheduling and something else. What I do to reducr error is just repeating and getting average time. 
- What's the reason of different time used in different file system?
- What's the order of entreies returned by `readdir()` in different file system?

see code in [test_random.c](./test_random.c) for creating and removing files. (I don't add any options for gcc except specifying the name.)
Usage: `./test_random <number_of_files> <directory>`

[test.sh](./test.sh) for compare in different size and file system type. (you should modify the var `dirs` considering your mount points).
```
Usage: ./test.sh [-d] [-n]
options:
 -d compare different directories in same file numbers 
 -n compare different file number in same directory 
```

In my computer, `./test.sh -n` output: (need `sudo`)
Here's the data you provided formatted into a Markdown table for better readability:

| Directory   | Number of Files | Create Time (s) | Delete Time (s) | Total Time (s) |
|-------------|------------------|------------------|------------------|-----------------|
| /mnt/ext2  | 1000             | 0.00164180       | 0.00006080       | 0.00170260      |
| /mnt/ext2  | 2000             | 0.00283320       | 0.00006070       | 0.00289390      |
| /mnt/ext2  | 5000             | 0.00642200       | 0.00004920       | 0.00647120      |
| /mnt/ext2  | 10000            | 0.01139130       | 0.00005260       | 0.01144390      |
| /mnt/ext2  | 20000            | 0.02173340       | 0.00005290       | 0.02178630      |
| /mnt/ext2  | 50000            | 0.05279040       | 0.00005220       | 0.05284260      |
| /mnt/ext2  | 100000           | 0.10042720       | 0.00005390       | 0.10048110      |
|             |                  |                  |                  |                 |
| /mnt/ext3  | 1000             | 0.00140370       | 0.00005610       | 0.00145980      |
| /mnt/ext3  | 2000             | 0.00289660       | 0.00006140       | 0.00295800      |
| /mnt/ext3  | 5000             | 0.00666160       | 0.00005220       | 0.00671380      |
| /mnt/ext3  | 10000            | 0.01203480       | 0.00005190       | 0.01208670      |
| /mnt/ext3  | 20000            | 0.02099020       | 0.00005250       | 0.02104270      |
| /mnt/ext3  | 50000            | 0.04993430       | 0.00005440       | 0.04998870      |
| /mnt/ext3  | 100000           | 0.10030010       | 0.00005680       | 0.10035690      |
|             |                  |                  |                  |                 |
| /mnt/xfs   | 1000             | 0.00138860       | 0.00005120       | 0.00143980      |
| /mnt/xfs   | 2000             | 0.00284040       | 0.00005200       | 0.00289240      |
| /mnt/xfs   | 5000             | 0.00590490       | 0.00004490       | 0.00594980      |
| /mnt/xfs   | 10000            | 0.01256310       | 0.00004630       | 0.01260940      |
| /mnt/xfs   | 20000            | 0.02114380       | 0.00004660       | 0.02119040      |
| /mnt/xfs   | 50000            | 0.05286410       | 0.00004800       | 0.05291210      |
| /mnt/xfs   | 100000           | 0.09927070       | 0.00005480       | 0.09932550      |

- The time to create files increases as the number file increase, but the time to delete files have ups and downs.
- XFS system have the minimum time to delete files when NF is smaller. 

[test_random.c](./test_random.c) create files with random names, [test_increment.c](./test_increment.c) creat file with increasing number.
running `./test_increment` instead `./test_random`, `test.sh` get:

| Directory   | Number of Files | Create Time (s) | Delete Time (s) | Total Time (s) |
|-------------|------------------|------------------|------------------|-----------------|
| /mnt/ext2  | 1000             | 0.00152240       | 0.00007810       | 0.00160050      |
| /mnt/ext2  | 2000             | 0.00306230       | 0.00006000       | 0.00312230      |
| /mnt/ext2  | 5000             | 0.00664650       | 0.00005210       | 0.00669860      |
| /mnt/ext2  | 10000            | 0.01144010       | 0.00005620       | 0.01149630      |
| /mnt/ext2  | 20000            | 0.02180840       | 0.00005280       | 0.02186120      |
| /mnt/ext2  | 50000            | 0.05174390       | 0.00006900       | 0.05181290      |
| /mnt/ext2  | 100000           | 0.09928670       | 0.00005590       | 0.09934260      |
|             |                  |                  |                  |                 |
| /mnt/ext3  | 1000             | 0.00153770       | 0.00005950       | 0.00159720      |
| /mnt/ext3  | 2000             | 0.00290990       | 0.00006290       | 0.00297280      |
| /mnt/ext3  | 5000             | 0.00598660       | 0.00005740       | 0.00604400      |
| /mnt/ext3  | 10000            | 0.01174020       | 0.00005340       | 0.01179360      |
| /mnt/ext3  | 20000            | 0.02127140       | 0.00005800       | 0.02132940      |
| /mnt/ext3  | 50000            | 0.05107610       | 0.00005610       | 0.05113220      |
| /mnt/ext3  | 100000           | 0.09604890       | 0.00005980       | 0.09610870      |
|             |                  |                  |                  |                 |
| /mnt/xfs   | 1000             | 0.00122770       | 0.00005290       | 0.00128060      |
| /mnt/xfs   | 2000             | 0.00304410       | 0.00005460       | 0.00309870      |
| /mnt/xfs   | 5000             | 0.00613750       | 0.00005050       | 0.00618800      |
| /mnt/xfs   | 10000            | 0.01251390       | 0.00005740       | 0.01257130      |
| /mnt/xfs   | 20000            | 0.02073340       | 0.00006050       | 0.02079390      |
| /mnt/xfs   | 50000            | 0.04971100       | 0.00005270       | 0.04976370      |
| /mnt/xfs   | 100000           | 0.09808930       | 0.00005140       | 0.09814070      |

- The time to delete is more than create file with random names. (In my implementation, they both need use `qsort()` because using `readdir()` won't get orderly file names)