#ifndef __RAMBOSO_X_TERM_SHELL_H__
#define __RAMBOSO_X_TERM_SHELL_H__

#include <unistd.h> /* To work with Syscalls */
#include <stdbool.h> /* bool */


/**
 * Create a child process
 */  
pid_t create_child(void);


/**
 * Create the command line arguments for the given input
 * The caller must free the memory after the use.
 */   
char ** create_command_line_arguments(char *);

/**
 *  Handle the input.   
 *  Creates a child process if necessary, exit, and so on...
 */ 
void handle_command_line(char *[]);



#endif