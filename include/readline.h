#ifndef __RAMBOSO_X_TERM_READLINE_H__
 #define __RAMBOSO_X_TERM_READLINE_H__

#include <stdio.h>

typedef struct _user_input UserInput;
typedef struct _history History;

/**
 * Read the user input, and parse it
 */ 
UserInput * read_input(void);

/**
 * Initialize a new raw terminal
 */ 
void initialize_virtual_terminal(void);

/**
 * Restore the user original terminal.
 * So, we wont get pissed off.
 */ 
void restore_user_terminal(void);

/**
 * Every command must be stored inside the History.
 */ 
void add_to_history(char *);

/**
 * At the init, we might restore the history of command, so user can see the last commands
 */ 
void restore_history(FILE *);

/**
 * The last node of next must point to null, so, we can know where is the end.
 */ 
struct _history {
    char *line;
    History *next;
};

/**
 * Store the input from the user, the last index of arguments must point to null.
 */ 
struct _user_input {
    char *command;
    char **arguments;
};



#endif