#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_PROCESSES 1024

typedef struct Process {
	pid_t pid;
	pid_t ppid;
	char name[128];
} Process;

Process processes[MAX_PROCESSES]; //you can use tree instead array.
int process_count = 0;

void read_processes() {
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir("/proc")) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (isdigit(ent->d_name[0])) {  // Check if the directory name is a number (PID)
				pid_t pid = atoi(ent->d_name);
				char path[256], line[256];
				snprintf(path, sizeof(path), "/proc/%s/status", ent->d_name);

				FILE *fp = fopen(path, "r");
				if (fp) {
					Process proc;
					proc.pid = pid;

					// Read the status file to get the PPID and name
					while (fgets(line, sizeof(line), fp)) {
						if(sscanf(line, "PPid:\t%d", &proc.ppid)==1){
						}else{
							sscanf(line, "Name:\t%s", proc.name);
						}
					}
					fclose(fp);
					processes[process_count++] = proc;
				}
			}
		}
		closedir(dir);
	} else {
		perror("Could not open /proc");
	}
}

void print_process_tree(pid_t ppid, int level) {
	for (int i = 0; i < process_count; i++) {
		if (processes[i].ppid == ppid) {
			for (int j = -1; j < level; j++) {
				printf("  ");//indent
			}
			printf("%d (%s)\n", processes[i].pid, processes[i].name);
			print_process_tree(processes[i].pid, level + 1);  // Recursive call for children
		}
	}
}

int main() {
	read_processes();
	printf("systemd\n");
	print_process_tree(1, 0);
	// in fact, pstree will show many vertical bar and herizon line, it is hard to implement for me
	return 0;
}
