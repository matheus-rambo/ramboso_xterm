#include <stdio.h> /* Printf */
#include <stdlib.h> /* Malloc and Exit */
#include <string.h> /* String functions */
#include <wait.h> /* waitid */

#include "shell.h"

/** The maximum size of the buffer. */ 
#define MAX_BUFFER_SIZE 150

/** The name of the shell. */
#define SHELL "ramboso_xterm-$ "

static char binaries_path[50];

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
    strcpy(binaries_path, argv[1]);

    // Buffer to read from stdin
    char buffer[MAX_BUFFER_SIZE];

    while(1) {
    
        printf(SHELL);
        fflush(stdin);
        fgets(buffer, MAX_BUFFER_SIZE, stdin);
        size_t length = strlen(buffer);
        buffer[length - 1] = '\0'; // removes the line feed.

        char **cmd = create_command_line_arguments(buffer);
        handle_command_line(cmd);
        free(cmd);
    }

    return EXIT_SUCCESS;
}

unsigned int count_args(char *arg) {
    unsigned int size = 0;
    while(*arg != '\0') {
        // Must not count if the previous character is a whitespace too
        if(*arg == ' ' && *(arg - 1) != ' ') size++;
        arg++;
    }
    return size;
}

void handle_command_line(char *input[]) {

    if(strcmp(input[0], "exit") == 0) {
        exit(EXIT_SUCCESS);
    }

    const pid_t pid = create_child();
    if(pid == 0) {
        // child 
        
        char binary_file[strlen(binaries_path) + strlen(input[0]) + 1];
        strcpy(binary_file, binaries_path);
        strcat(binary_file, input[0]);
        execvp(binary_file, input);
        perror("Error with execve.\n");
        exit(EXIT_FAILURE);

    } else {

        // parent
        // man wait
        waitid(P_PID, pid, NULL, WEXITED);
    }
}

char ** create_command_line_arguments(char *input) {

    // Count how many args are there, plus one to the last argument that must be null.
    unsigned int size = count_args(input) + 1;

    // creates an array to store the arguments
    char ** args = malloc(sizeof(char *) * size);

    // puts null into the last index
    args[size - 1] = NULL;

    // delimiter
    const char *delimiter = " ";
    
    // to manage the index to store the element
    unsigned int index = 0;
    
    // gets the element
    char *token = strtok(input, delimiter);

    while(token != NULL) {
        size_t bytes_to_alloc = sizeof(char) * (strlen(token) + 1);
        args[index] = malloc(bytes_to_alloc);
        strcpy(args[index++], token);
        token = strtok(NULL, delimiter);            
    }

    return args;
}