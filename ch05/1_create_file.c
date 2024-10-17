#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define _FILE_OFFSET_BITS (64)

int main(int argc, char *argv[])
{
	int fd;
	off_t off;

	if (argc != 3 || strcmp(argv[1], "--help") == 0) {
		printf("%s pathname offset\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	fd = open(argv[1], O_RDWR | O_CREAT,
		    S_IRUSR | S_IWUSR);
	if (fd == -1) {
		perror("open\n");
		exit(EXIT_FAILURE);
	}
	
	off = atoll(argv[2]);
	if (lseek(fd, off, SEEK_SET) == -1) {
		perror("lseek\n");
		exit(EXIT_FAILURE);
	}
	
	if (write(fd, "text", 4) == -1) {
		perror("write\n");
		exit(EXIT_FAILURE);
	}
	printf("Success\n");
	exit(EXIT_SUCCESS);
}
