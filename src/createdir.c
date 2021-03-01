#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "createdir.h"

static const char *short_options = "o:g:t:hv";
static struct option long_options[] = {
    { "version", no_argument,       NULL, 'v'},
    { "owner",   required_argument, NULL, 'o'},
    { "group",   required_argument, NULL, 'g'},
    { "others",  required_argument, NULL, 't'},
    { NULL,      0,                 NULL,  0 }
};

int main(int argc, char *argv[]) {

    int option;

    /* The values must be in octal mode */
    mode_t owner = 0700, group = 0050, others = 0000;

    while ( ( option = getopt_long(argc, argv, short_options, long_options, NULL) ) != -1 ) {
        switch (option) {
            case 'h':
                help(argv[0], stdout);
            case 'v':
                version(argv[0]);
            case 'o':
                {
                    int value = atoi(optarg);
                    // owner is the 3º rightmost
                    owner = value<<6;
                    break;
                }
            case 'g':
                {
                    int value = atoi(optarg);
                    // group is the 2º rightmost
                    group = value<<3;
                    break;
                }
            case 't':
                {
                    int value = atoi(optarg);
                    // others is the rightmost
                    others = value;
                    break;
                }
        }
    }
   mode_t mode = ( S_IFDIR | owner | group | others );

   if(optind == argc) {
      fprintf(stderr, "The directory is required, and must be the last argument!\n");
      exit(EXIT_FAILURE);
   }

   for(int index = optind; index < argc; index++) {
      create_directory(argv[index], mode);
   }


    return 0;
}

void help(char *name, FILE *file) {
    const char *help =
    "\nUsage: %s [-o <mode> ] [-g <mode> ] [-t <mode> ] [-vh ] directory ...\n"
    "Create the directory(ies), if they do not already exist.\n\n"
    "Options:\n"
    "Mandatory arguments to long options are mandatory for short options too.\n\n"
    "  -o, --owner=MODE   Mode for the user.                      Default is: 7.\n"
    "  -g, --group=MODE   Mode for users of the same group.       Default is: 5.\n"
    "  -t, --others=MODE  Mode for others users.                  Default is: 0.\n"
    "  -h, --help         Display this help and exit.\n"
    "  -v, --version      Output the version information and exit.\n\n";
    fprintf(file, help, name, name, name);
    exit( file == stderr ? EXIT_FAILURE : EXIT_SUCCESS);
}

void create_directory(char *path, mode_t permissions) {
    int syscall_status = mkdir(path, permissions);
    if(syscall_status != 0) {
        handle_error();
     }
}

void version(char *bin) {
    printf("%s Version: %s.\n", bin, VERSION);
    exit(EXIT_SUCCESS);
}

void handle_error(void) {
    fprintf(stderr, "Aborted due to an error. Error Number: %d. See errno-base.h to details!\n", errno);
    exit(EXIT_FAILURE);
}