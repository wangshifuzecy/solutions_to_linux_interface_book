# 7-1

Modify the program in Listing 7-1 (free_and_sbrk.c) to print out the current value of the program break after each execution of malloc(). Run the program specifying a small allocation block size. This will demonstrate that malloc() doesn’t employ sbrk() to adjust the program break on each call, but instead periodically allocates larger chunks of memory from which it passes back small pieces to the caller.

## answer

In fact, `printf ` will call `malloc` internally to allocate a buffer for `stdout`. So you will get different answers by using `printf` printing `sbrk(0)` twice.
You can use `snprintf` instead of `printf` to avoid the problem.

I'm not sure about would it always malloc in all computers, you can try the following code.

```c
#include <stdio.h>
#include <malloc.h>
#include <unistd.h>

int main(){
		printf("initial brk: %p\n",sbrk(0));
		printf("program brk: %p\n",sbrk(0));
}
```

output

```
initial brk: 0xe0d37000
program brk: 0xe0d58000
```

The difference value:0x21000 = 4096 * 33

```c
#include <stdio.h>
#include <malloc.h>
#include <unistd.h>

int main(){
    printf("initial brk: %p\n",sbrk(0));
    //printf uses malloc internally.
    const long malloc_counts = 33;
    void* allocated_pool[malloc_counts];
    for(int counter=0; counter < malloc_counts; counter++)
    {
        printf("after %d times malloc\n",counter+1);
        allocated_pool[counter] = malloc(4096);
        printf("program brk: %p\n",sbrk(0));
    }
}
```

output

```
initial brk: 0x61342ee58000
after 1 times malloc
program brk: 0x61342ee79000
after 2 times malloc
program brk: 0x61342ee79000
after 3 times malloc
program brk: 0x61342ee79000
after 4 times malloc
program brk: 0x61342ee79000
after 5 times malloc
program brk: 0x61342ee79000
after 6 times malloc
program brk: 0x61342ee79000
after 7 times malloc
program brk: 0x61342ee79000
after 8 times malloc
program brk: 0x61342ee79000
after 9 times malloc
program brk: 0x61342ee79000
after 10 times malloc
program brk: 0x61342ee79000
after 11 times malloc
program brk: 0x61342ee79000
after 12 times malloc
program brk: 0x61342ee79000
after 13 times malloc
program brk: 0x61342ee79000
after 14 times malloc
program brk: 0x61342ee79000
after 15 times malloc
program brk: 0x61342ee79000
after 16 times malloc
program brk: 0x61342ee79000
after 17 times malloc
program brk: 0x61342ee79000
after 18 times malloc
program brk: 0x61342ee79000
after 19 times malloc
program brk: 0x61342ee79000
after 20 times malloc
program brk: 0x61342ee79000
after 21 times malloc
program brk: 0x61342ee79000
after 22 times malloc
program brk: 0x61342ee79000
after 23 times malloc
program brk: 0x61342ee79000
after 24 times malloc
program brk: 0x61342ee79000
after 25 times malloc
program brk: 0x61342ee79000
after 26 times malloc
program brk: 0x61342ee79000
after 27 times malloc
program brk: 0x61342ee79000
after 28 times malloc
program brk: 0x61342ee79000
after 29 times malloc
program brk: 0x61342ee79000
after 30 times malloc
program brk: 0x61342ee79000
after 31 times malloc
program brk: 0x61342ee79000
after 32 times malloc
program brk: 0x61342ee79000
after 33 times malloc
program brk: 0x61342ee9a000
```

You can see the brk increases 0x21000 twice(After `printf`, the brk increases first time. Then after 33 times calling `malloc`, the brk increases again.)

If you try to use one bigger size when calling `malloc ` every time, it may don't work as excepted. `malloc()` might request memory from the operating system using `mmap()`, which allocates memory outside of the heap, bypassing `sbrk()`.

Besides the allocating memory, the heap is also used for metadata, alignment padding and etc.

TODO: explain the more detailed allcation on heap.

# 7-2

(Advanced) Implement malloc() and free().

## answer
see [malloc_and_free.c](./malloc_and_free.c)

- `my_malloc(size_t size)` 
    1. scan list for searching suitable free block. If find one, resize it (If the size is big enough, split it into two blocks) 
    2. allocating new memory if needed. (Every time we allocate a batch of space and may not use all of it) If the top heap have free space, the program will use it and part of the new memory. 
- `my_free(void* mem)`
    1. free the block and merge if the prev or next block is free.
    2. if the free space of list is big than one value you set, release the free space.

TODO: 
1. alignment padding should betaken into consideration.
2. more efficient algorithm is needed(maybe need add one struct, and use circular linked list)
