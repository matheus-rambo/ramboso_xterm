#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <getopt.h>

DIR * open_directory_stream(char *);
void close_directory_stream(DIR *);


int main(int argc, char *arv[]) {

    return 0;
}

DIR * open_directory_stream(char *name) {
    DIR *directory = opendir(name);
    if(directory == NULL) {
        
    }

    
}