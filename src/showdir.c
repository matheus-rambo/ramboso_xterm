#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "command.h"

#define VERSION "0.0.1"
#define MAX_BUFFER_SIZE 150

DIR * open_directory_stream(char *);
void close_directory_stream(DIR *);
void show_directory_entries(char *, unsigned int, unsigned int);

const char *short_options = "n:hvd";
struct option long_options[] = {
    { "help",      no_argument,       NULL, 'h' },
    { "name",      required_argument, NULL, 'n' },
    { "version",   no_argument,       NULL, 'v' },
    { "max-depth", required_argument, NULL, 'd' },
    { NULL,        0,                 NULL,  0  }
};


int main(int argc, char *argv[]) {

    if(argc < 2) {
        help(argv[0], stderr);
        exit(EXIT_FAILURE);
    }

    int option;
    char directory_name[MAX_BUFFER_SIZE];
    memset(directory_name, 0, MAX_BUFFER_SIZE);
    bool tree_flag = false;
    unsigned int max_depth = 1;

    while ( ( option = getopt_long(argc, argv, short_options, long_options, NULL)) != -1 ) {
        switch (option) {
            case 'n':
                strncpy(directory_name, optarg, MAX_BUFFER_SIZE);
                break;
            case 'h':
                help(argv[0], stdout);
                return EXIT_SUCCESS;
            case 'v':
                version(argv[1]);
                return EXIT_SUCCESS;
            case 'd':
                {
                    int depth = atoi(optarg);
                    if(depth <= 0 ) {
                        help(argv[0], stderr);
                        return EXIT_FAILURE;
                    }
                    max_depth = depth;
                }
    
                break;
        }
    }

    if(strlen(directory_name) == 0) {
        fprintf(stderr, "The filename must be provided!");
        return EXIT_FAILURE;
    }

    show_directory_entries(directory_name, 0, max_depth);
    return 0;
}

DIR * open_directory_stream(char *name) {
    DIR *directory = opendir(name);
    if(directory == NULL) {
        fprintf(stderr, "Could not open the directory: %s.\n", name);
        exit(EXIT_FAILURE);
    }
    return directory;
}

void close_directory_stream(DIR *directory) {
    int status = closedir(directory);
    if(status != 0) {
        fprintf(stderr, "An error ocurred while trying to close the directory.\n");
        exit(EXIT_FAILURE);
    }
}

void show_directory_entries(char *directory_name, unsigned int depth, unsigned int max_depth) {
    DIR *directory = open_directory_stream(directory_name);
    struct dirent *entry = readdir(directory);
    while(entry != NULL) {
        puts(entry->d_name);
        // Ignore .. AND .. directories
        // FIXME: If the user has a directory that starts with .., we will ignore it ...
        if(entry->d_name[0] == '.' && (entry->d_name[1] == '.' || entry->d_name[1] == '\0')) goto next;

        // TODO: There are more types ...
        bool is_dir = entry->d_type == DT_DIR;
        for(unsigned int index = depth; index < max_depth; index++) printf("-");
        puts(entry->d_name);
        if(is_dir && depth < max_depth) {
            show_directory_entries(entry->d_name, depth + 1, max_depth);
        }
        next:
            entry = readdir(directory);
    }
    close_directory_stream(directory);
}


void help(char *name, FILE *file) {
    const char *help = 
    "Usage: %s [OPTION]...\n" 
    "Shows the directory entries.\n\n" 
    "Options\n"
    "  -n, --name      The directory name.\n"
    "  -d, --max-depth A non negative value indicating the maximum depth.\n"
    "  -h, --help      Display this help and exit.\n"
    "  -v, --version   Output the version information and exit.\n"
    "Default values\n"
    "  -d = 1.\n\n";
    fprintf(file, help, name);
}

void version(char *bin) {
    printf("%s Version: %s.\n", bin, VERSION);
}