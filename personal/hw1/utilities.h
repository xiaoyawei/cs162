#ifndef UTILITIES_H
#define UTILITIES_H

#include "parse.h" 

int sys_call_helper(tok_t arg[]);
int io_redirect(tok_t arg[], int *default_input, int *default_output); 
void io_restore(int default_input, int default_output);
void file_close(int input, int output);
void sig_handler(int sig_num);
void parent_signal_setup();
void child_signal_setup();

#endif /* UTILITIES_H */
