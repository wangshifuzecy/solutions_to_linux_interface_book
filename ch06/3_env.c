#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern char **environ;

int is_valid_env_name(const char *name) {
    if (name == NULL || name[0] == '\0') {
        return 0;  // Name is NULL or empty
    }

    if (!(isalpha(name[0]) || name[0] == '_')) {
        return 0;  // Name must start with an alphabetic character or underscore
    }

    for (size_t i = 1; name[i] != '\0'; i++) {
        if (!(isalnum(name[i]) || name[i] == '_')) {
            return 0;  // Subsequent characters must be alphanumeric or underscore
        }
    }

    return 1;  // Name is valid
}

int my_setenv(const char *name, const char *value, int overwrite){
	if(name == NULL || value == NULL || !is_valid_env_name(name)){
		return -1;
	}
	char *current_value = getenv(name);
	if(current_value != NULL && !overwrite){
		return 0;
	}
	size_t len = strlen(name) + strlen(value) + 2;
	// +2, 1 for '=', 1 for '\0'
	char *env_string = (char *)malloc(len);
	if(env_string == NULL){
		return -1;
	}
	snprintf(env_string, len, "%s=%s", name, value);	
	if(putenv(env_string) != 0){
		free(env_string);
		return -1;
	}
	return 0;
}

int my_unsetenv(const char *name){
	if(name == NULL || !is_valid_env_name(name)){
		return -1;
	}
	size_t name_len = strlen(name);
	char **env = environ;
	char **dest = environ;
	while(*env != NULL){
		if(strncmp(*env, name, name_len) == 0 && (*env)[name_len] == '='){
			env++;
			continue;
		}
		*dest = *env;
		dest++;
		env++;
	}
	*dest = NULL;
	return 0;
}

int main(){
	// Set a new environment variable
	if (my_setenv("MY_VAR", "my_value", 1) == 0) {
		printf("MY_VAR set to: %s\n", getenv("MY_VAR"));
	} else {
		printf("Failed to set MY_VAR\n");
	}

	// Try to unset it
	if (my_unsetenv("MY_VAR") == 0) {
		printf("MY_VAR unset. getenv(\"MY_VAR\") returns: %s\n", getenv("MY_VAR"));
	} else {
		printf("Failed to unset MY_VAR\n");
	}

	// Set the same variable again without overwriting
	if (my_setenv("MY_VAR", "new_value", 0) == 0) {
		printf("set new value, MY_VAR is now: %s\n", getenv("MY_VAR"));
	} else {
		printf("Failed to set MY_VAR\n");
	}

	if (my_setenv("MY_VAR", "overwrite value", 1) == 0) {
		printf("overwrite new value, MY_VAR is now: %s\n", getenv("MY_VAR"));
	} else {
		printf("Failed to set MY_VAR\n");
	}

	// Unset it again to clean up
	if (my_unsetenv("MY_VAR") == 0) {
		printf("MY_VAR finally unset.\n");
	}
	return 0;
}
