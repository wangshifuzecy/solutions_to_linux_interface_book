# 6-1
Compile the program in Listing 6-1 (mem_segments.c), and list its size using ls –l. Although the program contains an array (mbuf) that is around 10 MB in size, the executable file is much smaller than this. Why is this?
## answer
`cc mem_segments.c -o mem_segments`
```
wsf@~/code/linux/interface_book/proc$ ls -l mem_segments
-rwxrwxr-x 1 wsf wsf 16256 Sep 30 08:28 mem_segments
```
In mem_segments.c's main function:
```c
int
main(int argc, char *argv[])    /* Allocated in frame for main() */
{
    static int key = 9973;      /* Initialized data segment */
    static char mbuf[10240000]; /* Uninitialized data segment */
    char *p;                    /* Allocated in frame for main() */

    p = malloc(1024);           /* Points to memory in heap segment */

    doCalc(key);

    exit(EXIT_SUCCESS);
}
```
mbuf should have 10MB in size. But the executable file is much smaller.
The explanation is as follows:
The uninitialized data segment is known as the BSS segment(Block Started by Symbol, you can remember it as Better Save Space simply). This segment is used to declare variables that are uninitialized or initialized to zero at the start of the program. It doesn't take up actual space in the file itself because the variables are not given initial values. During the program’s runtime, the operating system allocates space for the variables in the BSS segment and initializes them to zero.

# 6-2
Write a program to see what happens if we try to longjmp() into a function that has already returned.
## answer
See in 2_loogjmp.c:
```c
#include <stdio.h>
#include <setjmp.h>

jmp_buf buf;

void foo() {
	printf("foo() called\n");

	// Save the current context
	if (setjmp(buf) == 0) {
		printf("Returning from foo()\n");
		return; // Function foo will return
	} else {
		printf("Returned to foo() with longjmp!\n");  // This will never be safe!
	}
}

int main() {
	foo();  // Call foo(), it returns normally

	// Attempt to jump back into foo after it has returned
	printf("Attempting to longjmp() into a function that has already returned\n");
	longjmp(buf, 1);  // This causes undefined behavior

	printf("This line will never be executed\n");
	return 0;
}
```
running:
```
wsf@~/code/linux/interface_book/hw/ch6$ ./2_loogjmp
foo() called
Returning from foo()
Attempting to longjmp() into a function that has already returned
Returned to foo() with longjmp!
Segmentation fault (core dumped)
```
After loogjmp() into a function that has already returned, the program meet one `Segmentation fault`

# 6-3
Implement setenv() and unsetenv() using getenv(), putenv(), and, where necessary, code that directly modifies environ. Your version of unsetenv() should check to see whether there are multiple definitions of an environment variable, and remove them all (which is what the glibc version of unsetenv() does).
## answer
see in 3_env.c
