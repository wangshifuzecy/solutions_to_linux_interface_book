#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    const char *file = "temp_file.txt";

    // Open the file and duplicate file descriptors
    int fd1 = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    int fd2 = dup(fd1);
    int fd3 = open(file, O_RDWR);

    write(fd1, "Hello,", 6);
    write(fd2, "world", 6);
    lseek(fd2, 0, SEEK_SET);

    off_t off1 = lseek(fd1, 0, SEEK_CUR);
    off_t off2 = lseek(fd1, 0, SEEK_CUR);
    off_t off3 = lseek(fd1, 0, SEEK_CUR);
		printf("after lseek():\noff of fd1 %d\noff2 of fd2 %d\noff of fd3 %d\n",
			off1, off2, off3
		);

    write(fd1, "HELLO,", 6);
    write(fd3, "Gidday", 6);

    // Close file descriptors
    close(fd1);
    close(fd2);
    close(fd3);

		printf("in the file, file content:\n");
    // Display final file content
    FILE *f = fopen(file, "r");
    char buffer[256];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), f)) > 0) {
        fwrite(buffer, 1, bytes, stdout);
    }
    fclose(f);
		puts("");

    // Remove temporary file
    remove(file);

    return 0;
}
