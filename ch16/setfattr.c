#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <errno.h>

void print_usage(const char *progname)
{
    fprintf(stderr, "Usage: %s -n name [-v value] files...\n", progname);
}

int main(int argc, char *argv[])
{
    const char *attr_name = NULL;  // Extended attribute name
    const char *attr_value = NULL; // Extended attribute value
    int opt;

    // Parse command-line options
    while ((opt = getopt(argc, argv, "n:v:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            attr_name = optarg; // Set attribute name
            break;
        case 'v':
            attr_value = optarg; // Set attribute value (optional)
            break;
        default:
            print_usage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    // Check if attribute name is provided
    if (attr_name == NULL || optind >= argc)
    {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    // Loop through each file provided in the command line
    for (int i = optind; i < argc; i++)
    {
        const char *filename = argv[i];
        ssize_t ret;

        // Set the extended attribute
        ret = setxattr(filename, attr_name, attr_value,
                    attr_value ? strlen(attr_value) : 0, 0);
        if (ret == -1)
        {
            // Handle error
            perror(filename);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}