#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>

#define MAX_FILES 20000
#define FILENAME_PREFIX "x"
#define FILENAME_SUFFIX_LENGTH 7

void create_files(const char *directory, int num_files) {
    char filename[20];
    FILE *file;

    for (int i = 0; i < num_files; ++i) {
        int random_num = rand() % 1000000; // Generate a random number from 0 to 999999
        snprintf(filename, sizeof(filename), "%s/%s%06d", directory, FILENAME_PREFIX, random_num);
        file = fopen(filename, "wb"); // Create a file
        if (file) {
            fputc(0, file); // Write one byte
            fclose(file);
        }
    }
}

int compare(const void *a, const void *b){
  return strcmp(*(const char **)a, *(const char **)b);
}

void delete_files(const char *directory, int num_files) {
    char *filenames[MAX_FILES];
    int count = 0;
    char full_path[256];
    struct dirent *entry;
    DIR *dp = opendir(directory);

    if (dp == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp)) != NULL) {
        if (strncmp(entry->d_name, FILENAME_PREFIX, strlen(FILENAME_PREFIX)) == 0) {
            filenames[count] = malloc(strlen(entry->d_name) + 1);
            strcpy(filenames[count], entry->d_name);
            count++;
            if(count >= MAX_FILES) break;
        }
    }

    closedir(dp);

    qsort(filenames, count, sizeof(char *), compare);

    for(int i = 0; i < count; i++){
      snprintf(full_path, sizeof(full_path), "%s/%s", directory, filenames[i]);
      //printf("%s deleted\n", full_path); // DEBUG
      remove(full_path);
      free(filenames[i]);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number_of_files> <directory>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_files = atoi(argv[1]);
    const char *directory = argv[2];
    
    if (num_files <= 0) {
        fprintf(stderr, "Number of files must be positive.\n");
        return EXIT_FAILURE;
    }

    srand(time(NULL)); // Seed the random number generator

    struct timespec start, end;

    // Measure creation time
    clock_gettime(CLOCK_MONOTONIC, &start);
    create_files(directory, num_files);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9;
    printf("Time to create %d files = %lf seconds\n", num_files, elapsed);

    // Measure deletion time
    clock_gettime(CLOCK_MONOTONIC, &start);
    delete_files(directory, num_files);
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9;
    printf("Time to delete %d files = %lf seconds\n", num_files, elapsed); 
    return EXIT_SUCCESS;
}