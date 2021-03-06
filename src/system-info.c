#include <sys/sysinfo.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

#include "system-info.h"

#define KIBIBYTE 1024

const char *short_options = "u:hv";
struct option long_options[] = {
    { "help",      no_argument,       NULL, 'h' },
    { "version",   no_argument,       NULL, 'v' },
    { "unit",      required_argument, NULL, 'u' },
    { NULL,        0,                 NULL,  0  }
};

unsigned char get_unit(char *);

int main (int argc, char *argv[]) {

    int option;

    while( ( option = getopt_long(argc, argv, short_options, long_options, NULL)) != -1 ) {
        switch(option) {
            case 'h':
                help(argv[0], stdout);
            case 'v':
                version(argv[0]);
            case 'u':
                {
                    // TODO
                }
        }
    } 
    
    struct sysinfo info;
    if(sysinfo(&info) == -1) {
        // TODO error
        exit(EXIT_FAILURE);
    }

    // MEMORY IN KiB(KibiBytes)
    const unsigned int unit = info.mem_unit * KIBIBYTE; 


    char *message = ""
    "Uptime in minutes:       %d.\n"
    "Total usable RAM in MiB: %d.\n"
    "Available RAM in MiB:    %d.\n"
    "Current processes:       %d.\n";
    printf(
        message, 
        info.uptime / 60, 
        info.totalram / KIBIBYTE / KIBIBYTE,
        info.freeram / KIBIBYTE / KIBIBYTE,
        info.procs
    );

    return 0;

}

void version(char *bin) {
    printf("%s Version %s.\n", bin, VERSION);
    exit(EXIT_SUCCESS);
}

void help(char *name, FILE *file) {
    const char *help = "TODO: %s";
    fprintf(file, help, name);
    exit(file == stderr ? EXIT_FAILURE : EXIT_SUCCESS);
}

