#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define BUFFER_SIZE 1024
#define FILE_LIMIT 20

int num_files = 0;
FILE* files[FILE_LIMIT];

//In my code, ^C won't close file correctly so that files don't save input, but ^D will save.So I write function handle the problem.
void handle_sigint(int sig){
	for(int i = 0; i < num_files; i++){
		fclose(files[i]);
	}

	exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]){
	int opt;
	int is_append = 0;
	char buffer[BUFFER_SIZE];
	ssize_t bytes_read;
	char* file_mode;

	//ensure fclose called when ^C
	signal(SIGINT, handle_sigint);

	//see `man 3 getopt`
	// Parse command-line options using getopt()
	while ((opt = getopt(argc, argv, "a")) != -1) {
		switch (opt) {
			case 'a':
				is_append = 1; // Set append mode
				break;
			case '?':
				if(isprint(opt))
					fprintf(stderr,"Unknown option `-%c' .\n",optopt);
				else 
					fprintf(stderr,"Unknown option character `\\x%x' .\n", optopt);
				exit(EXIT_FAILURE);
			default:
				exit(EXIT_FAILURE);
		}
	}
	file_mode = is_append ? "a" : "w";

	num_files = argc - optind;
	for(int i = optind; i < argc; i++){
		FILE* pFile = fopen(argv[i], file_mode);
		if(pFile == NULL){
			fprintf(stderr, "Unable to open file %s for mode %s",argv[i], file_mode);
			exit(EXIT_FAILURE);
		}
		files[i-optind]= pFile;
	}
	
	while((fgets(buffer, sizeof(buffer), stdin) != NULL)){
		bytes_read = strlen(buffer);
		fwrite(buffer,1,bytes_read,stdout);
		for(int i = 0; i < num_files; i++){
			fwrite(buffer,1,bytes_read,files[i]);
		}
	}

	exit(EXIT_SUCCESS);
}
