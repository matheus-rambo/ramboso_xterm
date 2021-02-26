#include <stdio.h>
#include <unistd.h> 
#include <getopt.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>

#include "command.h"

const char *short_options = "n:hvi";
struct option long_options[] = {
    {"help", no_argument, NULL, 'h'},
    {"name", required_argument, NULL, 'n'},
    {"version", no_argument, NULL, 'v'},
    {"interactive", no_argument, NULL, 'i'},
    {NULL, 0, NULL, 0}
};

#define VERSION "0.0.1"
#define MAX_BUFFER_SIZE 50

/* Executes the syscall */
static void create_directory(char *);

int main(int argc, char *argv[]) {

    if(argc < 2) {
        help(argv[0]);
        exit(1);
    }

    int option;
    char directory_name[MAX_BUFFER_SIZE];
    memset(directory_name, 0, MAX_BUFFER_SIZE);
    char *binary_name = argv[0];

    bool interactive_flag = false;

    while ( ( option = getopt_long(argc, argv, short_options, long_options, NULL)) != -1 ) {
        switch (option) {
            case 'n':
                strncpy(directory_name, optarg, MAX_BUFFER_SIZE - 2);
                break;
            case 'h':
                help(binary_name);
                exit(EXIT_SUCCESS);
            case 'v':
                version(binary_name);
                exit(EXIT_SUCCESS);
            case ':':
                help(binary_name);
                exit(EXIT_FAILURE);
            case 'i':
                interactive_flag = true;
        }
    } 

    if(interactive_flag) {
        printf("Directory name: ");
        fgets(directory_name, MAX_BUFFER_SIZE - 2, stdin);
    } else if (directory_name == NULL) {
        puts("The directory name must be specified!");
        exit(EXIT_FAILURE);
    }

    size_t length = strlen(directory_name);
    if(directory_name[length - 1] == '\n') {
        directory_name[length - 1] = '\0';
    }
    create_directory(directory_name);   
    return 0;
}

void help(char *name) {
    const char *help = 
    "Usage: %s [OPTION]...\n" 
    "Creates a new directory.\n\n" 
    "Options\n"
    "  -n, --name        The directory name.\n"
    "  -i, --interactive Interactive mode.\n"
    "  -h, --help        Display this help and exit.\n"
    "  -v, --version     Output the version information and exit.\n\n";
    printf(help, name);
}

static void create_directory(char *path) {
    int syscall_status = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if(syscall_status != 0) {
        puts("An error ocurred!");
        exit(syscall_status);
    }
}

void version(char *bin) {
    printf("%s Version: %s.\n", bin, VERSION);
}