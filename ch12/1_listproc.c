#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>

void list_processes(const char *username) {
	DIR *dir;
	struct dirent *ent;
	char path[256];
	FILE *fp;
	char line[256];
	char *pname;
	char *token;
	int bias = strlen("name:") + 1;

	if ((dir = opendir("/proc")) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_type == DT_DIR) {
				int pid = atoi(ent->d_name);
				if (pid > 0) {
					snprintf(path, sizeof(path), "/proc/%s/status", ent->d_name);
		//			printf("path: %s\n", path);
					fp = fopen(path, "r");
					if (fp) {
						fgets(line,sizeof(line),fp);
						pname = strstr(line,"Name:");
						if(pname){
							pname += bias;
						}
						char* process_name = malloc(strlen(pname)+1);
						if(process_name){
							strcpy(process_name,pname);
							//						printf("token: %s\n", token);
							char* while_ret;
							while(1){
								while_ret = fgets(line,sizeof(line),fp);
								if(while_ret == NULL){
									if (feof(fp)) {
										printf("End of file reached successfully.\n");
										break; // Exit the loop if EOF is reached
									}
									if (ferror(fp)) {
										perror("Error reading file");
										break; // Exit the loop if an error occurred
									}
								}
								if(strstr(line,"Uid:")){
									token = strtok(line,"\t");
									token = strtok(NULL,"\t");
									if(token && strcmp(token,username)==0){
										printf("PID: %d, Name: %s\n", pid, process_name);
									}
									break;
								}
							}
						}
						fclose(fp);
					}
				}
			}
		}
		closedir(dir);
	} else {
		perror("Could not open /proc");
	}
}

int main() {
	char username[32];
	struct passwd* pw;
	printf("Enter username: ");
	scanf("%31s", username);

	pw = getpwnam(username);
	snprintf(username,sizeof(username),"%d",pw->pw_uid);
	//printf("uid: %s\n", username);

	list_processes(username);
	return 0;
}
