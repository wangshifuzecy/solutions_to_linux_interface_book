#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>

ssize_t my_writev(int fd, const struct iovec *iov, int iovcnt){
	ssize_t total_written = 0;
	for(int i = 0; i < iovcnt; i++){
		ssize_t bytes_written = write(fd, iov[i].iov_base, iov[i].iov_len);
		if(bytes_written == -1){
			return -1;
		}
		total_written += bytes_written;
	}
	return total_written;
}

ssize_t my_readv(int fd, const struct iovec *iov, int iovcnt){
	ssize_t total_read = 0;
	for(int i = 0; i < iovcnt; i++){
		ssize_t bytes_read = read(fd, iov[i].iov_base, iov[i].iov_len);
		if(bytes_read == -1){
			return -1;
		}
		total_read += bytes_read;
		if(bytes_read < iov[i].iov_len){
			break;
		}
	}
	return total_read;
}

int main() {
    const char *filename = "test_file.txt";
    int fd;

    // Create a test file for writing
    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    // Prepare iovec structures for writing
    struct iovec iov_w[2];
    iov_w[0].iov_base = "Hello, ";
    iov_w[0].iov_len = 7;
    iov_w[1].iov_base = "world!\n";
    iov_w[1].iov_len = 7;

    // Simulate writev
    ssize_t bytes_written = my_writev(fd, iov_w, 2);
    printf("Bytes written: %zd\n", bytes_written);

    // Close and reopen the file for reading
    close(fd);
    fd = open(filename, O_RDONLY);

    // Prepare buffers for reading
    char buf1[8];
    char buf2[8];
    
    struct iovec iov_r[2];
    iov_r[0].iov_base = buf1;
    iov_r[0].iov_len = sizeof(buf1) - 1;
    iov_r[1].iov_base = buf2;
    iov_r[1].iov_len = sizeof(buf2) - 1;

    // Simulate readv
    ssize_t bytes_read = my_readv(fd, iov_r, 2);
    printf("Bytes read: %zd\n", bytes_read);

    // Null-terminate and print the buffers
    buf1[bytes_read < sizeof(buf1) ? bytes_read : sizeof(buf1) - 1] = '\0';
    buf2[bytes_read < sizeof(buf1) ? 0 : bytes_read - sizeof(buf1)] = '\0';
    printf("Buffer 1: '%s'\n", buf1);
    printf("Buffer 2: '%s'\n", buf2);

    // Clean up
    close(fd);
    remove(filename);

    return 0;
}
