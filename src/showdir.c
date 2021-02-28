#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>

#include "command.h"

#define VERSION "0.0.1"
#define MAX_BUFFER_SIZE 500

DIR * open_directory_stream(char *);
void close_directory_stream(DIR *);
void show_directory_entries(char *, unsigned int, unsigned int, bool);
void handle_error(void);
void get_entry_access(char *, char [4]);


const char *short_options = "n:hvad:";
struct option long_options[] = {
    { "help",      no_argument,       NULL, 'h' },
    { "name",      required_argument, NULL, 'n' },
    { "version",   no_argument,       NULL, 'v' },
    { "max-depth", required_argument, NULL, 'd' },
    { "access",    no_argument,       NULL, 'a' },
    { NULL,        0,                 NULL,  0  }
};


int main(int argc, char *argv[]) {

    if(argc < 2) {
        help(argv[0], stderr);
    }

    int option;
    char directory_name[MAX_BUFFER_SIZE];
    memset(directory_name, 0, MAX_BUFFER_SIZE);
    unsigned int max_depth = 0;
    bool access_flag = false;

    while ( ( option = getopt_long(argc, argv, short_options, long_options, NULL)) != -1 ) {
        switch (option) {
            case 'n':
                strncpy(directory_name, optarg, MAX_BUFFER_SIZE);
                break;
            case 'h':
                help(argv[0], stdout);
            case 'v':
                version(argv[1]);
            case 'd':
                {
                    int depth = atoi(optarg);
                    if(depth <= 0 ) {
                        help(argv[0], stderr);
                    }
                    max_depth = depth;
                }
                break;
            case 'a':
                access_flag = true;
                break;
        }
    }

    if(strlen(directory_name) == 0) {
        fprintf(stderr, "The filename must be provided!\n");
        return EXIT_FAILURE;
    }

    show_directory_entries(directory_name, 0, max_depth, access_flag);
    return 0;
}

DIR * open_directory_stream(char *name) {
    DIR *directory = opendir(name);
    if(directory == NULL) {
        fprintf(stderr, "Could not open the directory: %s.\n", name);
        handle_error();
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

void show_directory_entries(char *directory_name, unsigned int depth, unsigned int max_depth, bool access_flag) {
    DIR *directory = open_directory_stream(directory_name);
    struct dirent *entry = readdir(directory);
    while(entry != NULL) {

        // Ignore .. AND .. directories
        // FIXME: If the user has a directory that starts with .., we will ignore it ...
        if(entry->d_name[0] == '.' && (entry->d_name[1] == '.' || entry->d_name[1] == '\0')) {
            entry = readdir(directory);
            continue;
        }

        unsigned int spaces = depth * 2; /* White spaces and - */
        if(spaces > 0) {
            char tab[spaces + 1];
            tab[spaces] = '\0';
            for(unsigned int index = 0; index < spaces; index++) {
                if(index < depth ) {
                    tab[index] = ' ';
                } else {
                    tab[index] = '-';
                }
            } 
            printf(tab);           
        } 

        // +2 -> for the / and \0
        unsigned int path_dir_size = strlen(directory_name) + strlen(entry->d_name) + 2;
        char absolute_name[path_dir_size];
        strcpy(absolute_name, directory_name);
        strcat(absolute_name, entry->d_name);

        if(access_flag) {
            char buffer[4];
            get_entry_access(absolute_name, buffer);
            printf("%s Access: %d%d%d\n", entry->d_name, buffer[0], buffer[1], buffer[2]);
        } else {
            puts(entry->d_name);
        }

        if(entry->d_type == DT_DIR && depth < max_depth) {       
            strcat(absolute_name, "/");             
            show_directory_entries(absolute_name, depth + 1, max_depth, access_flag);
        }
        // gets the next entry
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
    "  -a, --access    Show the permissions of the directory/file.\n\n";
    fprintf(file, help, name);
    exit( file == stderr ? EXIT_FAILURE : EXIT_SUCCESS);
}

void version(char *bin) {
    printf("%s Version: %s.\n", bin, VERSION);
    exit(EXIT_SUCCESS);
}

void handle_error(void) {
    char *message;
    switch(errno) {
        case EACCES:
            message = "Permission denied!\n";
            break;
        case ENOENT:
            message = "Directory does not exists!\n";
            break;
        default:
            message = "Unknown error!\n";
            break;
    }
    fprintf(stderr, message);
    exit(EXIT_FAILURE);
}

void get_entry_access(char *entry_name, char buffer[4]) {
    struct stat entry_stat;
    int error = stat(entry_name, &entry_stat);
    if(error == -1) {
        handle_error();
    }
    // TODO: handle error

    // TODO: It can be better ...
    char permits = 0;
    if(entry_stat.st_mode & S_IRUSR) permits+=4;
    if(entry_stat.st_mode & S_IWUSR) permits+=2;
    if(entry_stat.st_mode & S_IXUSR) permits+=1;
    buffer[0] = permits;

    permits = 0;
    if(entry_stat.st_mode & S_IRGRP) permits+=4;
    if(entry_stat.st_mode & S_IWGRP) permits+=2;
    if(entry_stat.st_mode & S_IXGRP) permits+=1;
    buffer[1] = permits;

    permits = 0;
    if(entry_stat.st_mode & S_IROTH) permits+=4;
    if(entry_stat.st_mode & S_IWOTH) permits+=2;
    if(entry_stat.st_mode & S_IXOTH) permits+=1;
    buffer[2] = permits;

}