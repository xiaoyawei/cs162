#ifndef UTILITIES_H
#define UTILITIES_H

#include "parse.h" 

int sys_call_helper(tok_t arg[]);
int io_redirect(tok_t arg[], int *default_input, int *default_output); 
void io_restore(int default_input, int default_output);
void file_close(int input, int output);

#endif /* UTILITIES_H */
