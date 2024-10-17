#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define BYTE_TO_WRITE 'A'

void error_exit(const char* msg){
	perror(msg);
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]){
	int fd, flags;
	long num_bytes;
	char byte = BYTE_TO_WRITE;

	if (argc < 3 || argc > 4) {
		fprintf(stderr, "Usage: %s <filename> <num-bytes> [x]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	num_bytes = strtol(argv[2], NULL, 10);
	if (num_bytes <= 0) {
		fprintf(stderr, "Invalid number of bytes: %ld\n", num_bytes);
		exit(EXIT_FAILURE);
	}

	flags = O_WRONLY | O_CREAT;
	if(argc == 3){
		flags |= O_APPEND;
	}else if(argc == 3 && strcmp(argv[3], "x") == 0) {
		// If "x" is specified, do not use O_APPEND
	}

	fd = open(argv[1], flags, S_IRUSR | S_IWUSR);
	if (fd == -1) {
		error_exit("open");
	}

	for (long i = 0; i < num_bytes; i++) {
		if (flags & O_APPEND) {
			// With O_APPEND, the file descriptor automatically goes to the end of the file
			if (write(fd, &byte, 1) != 1) {
				error_exit("write");
			}
		} else {
			// Without O_APPEND, manually seek to the end before each write
			if (lseek(fd, 0, SEEK_END) == -1) {
				error_exit("lseek");
			}
			if (write(fd, &byte, 1) != 1) {
				error_exit("write");
			}
		}
	}

	exit(EXIT_SUCCESS);
}

