#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "readline.h"

#define MAX_BUFFER_SIZE 150
#define WHITE_SPACE " "

/**
 * This is the original user terminal
 */ 
static struct termios original_terminal;

/**
 * This is the temporary terminal that that we will use.
 */ 
static struct termios temporary_terminal;

/**
 * Last commands
 */   
static History *history;

void initialize_virtual_terminal(void) {
    tcgetattr(STDIN_FILENO, &original_terminal);
    tcgetattr(STDIN_FILENO, &temporary_terminal);

    // temporary_terminal.c_lflag &= ~(ECHO);

    // Updates the user terminal with our raw terminal
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &temporary_terminal);
}

void restore_user_terminal(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_terminal);
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


UserInput * read_input(void) {
    
    // Allocate memory to hold the user input
    char *input = malloc(sizeof(char) * MAX_BUFFER_SIZE);

    // Read from standar input
    fgets(input, MAX_BUFFER_SIZE - 1, stdin);

    // removes the line feed
    input[strlen(input) - 1] = '\0';

    // Realloc
    input  = realloc(input, strlen(input) + 1);

    // Allocate memory to hold the command and the arguments
    UserInput *user_input = malloc(sizeof(UserInput)); 
    user_input->arguments = NULL;
    user_input->command   = NULL;

    // Count how many arguments were passed
    unsigned int args_size = count_args(input);
    if(args_size == 0) {
        // Just the command was passed! No arguments
        user_input->command = input;
        user_input->arguments = malloc(sizeof(char *));
        user_input->arguments[0] = NULL;
    } else {
        // Get the command from the input. Obs: strtok is destructive
        user_input->command = strtok(input, WHITE_SPACE);

        // The last argument index must be NULL
        user_input->arguments = malloc(sizeof(char *) * args_size + 1);
        user_input->arguments[args_size] = NULL;
    
        unsigned char index = 0;

        // Read the command line arguments, and store inside the pointer to pointer.
        char *argument = strtok(NULL, WHITE_SPACE);
        while(argument != NULL) {
            user_input->arguments[index++] = argument;
            argument = strtok(NULL, WHITE_SPACE);
        }
    }
    return user_input;
}

