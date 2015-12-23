#include <unistd.h>
#include <stdlib.h> 
#include <string.h> 
#include <stdio.h> 

#include "utilities.h" 

int sys_call_helper(tok_t arg[]) {
  execv(arg[0], arg);
  char *path_var = getenv("PATH");
  char *path = strtok(path_var, ":");
  while (path) {
    size_t cmd_len = strlen(path) + strlen(arg[0]) + 2;
    tok_t cmd = (tok_t) malloc(cmd_len * sizeof(char));
    strncpy(cmd, path, strlen(path) + 1);
    strncat(cmd, "/", 1);
    strncat(cmd, arg[0], strlen(arg[0]) + 1);
    execv(cmd, arg);
    free(cmd);
    path = strtok(NULL, ":");
  } 
  return -1;
}
