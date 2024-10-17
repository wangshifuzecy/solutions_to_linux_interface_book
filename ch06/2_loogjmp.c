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
