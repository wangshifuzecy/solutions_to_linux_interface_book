#include <stdio.h>
#include <unistd.h> 
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

int my_dup(int oldfd){
	if (fcntl(oldfd, F_GETFL) == -1) {
		errno = EBADF;
		return -1;
	}
	//fcntl(fildes, F_DUPFD, newfd): This call returns a new file descriptor with a numerical value equal to or greater than the integer newfd.
	return fcntl(oldfd, F_DUPFD, 0);
}

int my_dup2(int oldfd, int newfd){
	if (fcntl(oldfd, F_GETFL) == -1) {
		errno = EBADF;
		return -1;
	}
	
	if(oldfd == newfd){
		return newfd;
	}

	if (fcntl(newfd, F_GETFL) != -1) {
		close(newfd);
	}

	int result = fcntl(oldfd, F_DUPFD, newfd);
	if(result == -1){
		return -1;
	}

	return newfd;
}

void display_file_content(const char* filename) {
    char buffer[128];
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("error opening file to display");
        return;
    }
    printf("\nDisplaying content of %s:\n", filename);
    
    ssize_t bytes;
    while ((bytes = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes);
    }
    close(fd);
}

#define ENABLE_TEST (1)
#if ENABLE_TEST 

int main(int argc, char* argv[]){
	int fd1 = open("testfile1.txt", O_RDWR | O_CREAT, 0644);
	if(fd1 == -1){
		perror("error opening file");
		return 1;
	}
	write(fd1, "init line\n", 10);

	int fd2 = my_dup(fd1);
	if(fd2 == -1){
		perror("error in dup");
		return 1;
	}
	printf("after int fd2 = my_dup(fd1),fd1:%d,fd2:%d\n", fd1, fd2);
	write(fd2, "second line\n",12);

	int fd3 = open("anotherfile.txt", O_RDWR | O_CREAT, 0644);
	if(fd3 == -1){
		perror("error producing another file");
		return 1;
	}

	int result = my_dup2(fd1, fd3);
	if(result == -1){
		perror("Error in my_dup2");
		close(fd1);
		close(fd2);
		close(fd3);
		return 1;
	}
	printf("after my_dup2(fd1, fd3),fd1: %d,fd3: %d\n", fd1, fd3);
	write(fd3, "third line\n",11);

	// Display file contents before termination
	display_file_content("testfile1.txt");
	display_file_content("anotherfile.txt");

	close(fd1);
	close(fd2);
	close(fd3);

	if (unlink("testfile1.txt") == -1) {
		perror("Error deleting testfile1.txt");
	} else {
		printf("\nDeleted testfile1.txt\n");
	}

	if (unlink("anotherfile.txt") == -1) {
		perror("Error deleting anotherfile.txt");
	} else {
		printf("Deleted anotherfile.txt\n");
	}

	return 0;
}
#endif
