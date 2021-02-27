#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


#include "command.h"

const char *short_options = "ahv";
const struct option long_options[] = {
    {"absolute", no_argument, NULL, 'a'},
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'v'},
    { NULL, 0, NULL, 0}
};

#define VERSION "0.0.1"
#define MAX_BUFFER_SIZE 250


int main(int argc, char *argv[]) {

    int option;
    bool absolute_flag = false;

    while(( option = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch (option) {
            case 'a':
                absolute_flag = true;
                break;
            case 'h':
                help(argv[0]);
                return EXIT_SUCCESS;
            case 'v':
                version(argv[0]);
                return EXIT_SUCCESS;
            default:
                help(argv[0]);
                return EXIT_FAILURE;
        }
    }
    
    char *pdw = getcwd(NULL, MAX_BUFFER_SIZE);
    if(!absolute_flag) {
        char *delimiter = "/";
        char *dir = strtok(pdw, delimiter);
        while(dir != NULL) {
            pdw = dir;
            dir = strtok(NULL, delimiter);
        }
    }
    puts(pdw);
    return EXIT_SUCCESS;
}

void help(char *name) {
    const char* help = 
    "Usage: %s [OPTION]...\n"
    "Gets the current directory.\n\n"
    "Options\n"
    "   -a, --absolute The absolute name(path).\n"
    "   -h, --help     Display this help and exit.\n"
    "   -v, --version  Output the version information and exit.\n\n";
    printf(help, name);
}

void version(char *name) {
    printf("%s Version: %s.\n", name, VERSION);
}