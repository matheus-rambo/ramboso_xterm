#include <stdio.h> /* Printf */
#include <stdlib.h> /* Malloc and Exit */
#include <string.h> /* String functions */
#include <wait.h> /* waitid */

#include "shell.h"

/** The maximum size of the buffer. */ 
#define MAX_BUFFER_SIZE 150

/** The name of the shell. */
#define SHELL "ramboso_xterm-$$$ "

pid_t create_child(void) {
    const pid_t pid = fork();
    if(pid == -1) {
        exit(EXIT_FAILURE);
    }
    return pid;
}

/** Counts how many arguments were provided */ 
unsigned int count_args(char *);

int main(int argc, char *argv[], char *envp[]) {

    if(argc < 2) {
        fprintf(stderr, "Usage: %s <binaries path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    // Where the binaries are
    const char *binaries_path = argv[1];

    // Buffer to read from stdin
    char buffer[MAX_BUFFER_SIZE];

    while(1) {
    
        printf(SHELL);
        fflush(stdin);
        fgets(buffer, MAX_BUFFER_SIZE, stdin);
        size_t length = strlen(buffer);
        buffer[length - 1] = ' '; // removes the line feed and puts an extra space .

        const pid_t pid = create_child();
        if(pid == 0) {
            // child 

            // Count how many args are there, plus one to the last argument that must be null.
            unsigned int size = count_args(buffer) + 1;

            // creates a temporary array to store
            char *args[size];

            // puts null into the last index
            args[size - 1] = NULL;

            // delimiter
            const char *delimiter = " ";
            
            // to manage the index to store the element
            unsigned int index = 0;
            
            // gets the element
            char *token = strtok(buffer, delimiter);

            while(token != NULL) {
                args[index++] = token;
                token = strtok(NULL, delimiter);            
            }
            
            char binary_file[strlen(binaries_path) + strlen(args[0]) + 1];
            strcpy(binary_file, binaries_path);
            strcat(binary_file, args[0]);
            execve(binary_file, args, envp);
            perror("Error with execve.\n");
            return EXIT_FAILURE;

        } else {
            // parent
            // man wait
            waitid(P_PID, pid, NULL, WEXITED);
        }

    }

    return EXIT_SUCCESS;
}

unsigned int count_args(char *arg) {
    unsigned int size = 0;
    while(*arg != '\0') {
        if(*arg++ == ' ') size++;
    }
    return size;
}

