// you should add sudo for some files needed privilege
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>

void list_process_with_file(const char* target_path){
	DIR *proc_dir;
	struct dirent *pid_entry;

	if ((proc_dir = opendir("/proc")) == NULL) {
		perror("Could not open /proc");
		return;
	}

	while((pid_entry = readdir(proc_dir)) != NULL){
		if(isdigit(pid_entry->d_name[0])){ //assume first char is number because not all file under /proc is pid
			char fd_path[PATH_MAX];
			snprintf(fd_path, sizeof(fd_path),"/proc/%s/fd",pid_entry->d_name);
			DIR* fd_dir = opendir(fd_path);
			if(fd_dir){
				struct dirent* fd_entry;
				while((fd_entry = readdir(fd_dir)) != NULL){
					if(fd_entry->d_type == DT_LNK){ // if the file is a symbolic link
						char link_target[PATH_MAX];
						snprintf(fd_path,sizeof(fd_path),"/proc/%s/fd/%s",pid_entry->d_name,fd_entry->d_name);// get the file path
						ssize_t len = readlink(fd_path, link_target, sizeof(link_target) - 1); // read the target and get the len.
						if(len != -1){
							link_target[len] = '\0';
							if(strcmp(link_target, target_path) == 0){
								printf("%s\n",pid_entry->d_name);
							}
						}
					}
				}
				closedir(fd_dir);
			}
		}
	}
	closedir(proc_dir);
}

int main(int argc, char* argv[]){
	if(argc != 2){
		printf("need on arg for file");
		return EXIT_FAILURE;
	}

	const char* target_path = argv[1];
	list_process_with_file(target_path);
	return EXIT_SUCCESS;
}
