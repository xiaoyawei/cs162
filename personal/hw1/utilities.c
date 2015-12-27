#include <unistd.h>
#include <stdlib.h> 
#include <string.h> 
#include <stdio.h> 
#include <fcntl.h> 
#include <signal.h>

#include "utilities.h" 
#include "shell.h"
#include "process.h"

int sys_call_helper(tok_t arg[]) {
  child_signal_setup();
  if (setpgid(0, 0) != 0) {
    perror("setpgid(0, pid)"); 
  }
  int default_input = -1, default_output = -1;
  io_redirect(arg, &default_input, &default_output);
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
  io_restore(default_input, default_output);
  return -1;
}

int io_redirect(tok_t arg[], int *default_input, int *default_output) {
  *default_input = dup(STDIN_FILENO);
  *default_output = dup(STDOUT_FILENO);
  int input = -1, output = -1, index = 0;
  for (int i = 0; arg[i]; ++i) {
    if (!strcmp(arg[i], "<")) {
      if ((input = open(arg[++i], O_RDONLY)) < 0) {
        file_close(input, output);
        return -1; 
      }
    } else if (!strcmp(arg[i], ">")) { 
      if ((output = open(arg[++i], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) < 0) {
        file_close(input, output);
        return -1; 
      }
    } else { 
      arg[index++] = arg[i]; 
    } 
  }
  arg[index] = NULL;
  if (input >= 0) {
    dup2(input, STDIN_FILENO); 
    close(input);
  }
  if (output >= 0) {
    dup2(output, STDOUT_FILENO); 
    close(output);
  }
  return 0;
} 

void io_restore(int default_input, int default_output) {
  dup2(default_input, STDIN_FILENO);
  dup2(default_output, STDOUT_FILENO);
  close(default_input);
  close(default_output);
}

void file_close(int input, int output) {
  if (input >= 0) {
    close(input);
  } 
  if (output >= 0) {
    close(output);
  }
  perror("Fail opening a file");
}

void parent_signal_setup() {
  signal(SIGINT, SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
  signal(SIGTERM, SIG_IGN);
  signal(SIGTSTP, SIG_IGN);
  signal(SIGCONT, SIG_IGN);
  signal(SIGTTIN, SIG_IGN);
  signal(SIGTTOU, SIG_IGN);
}

void child_signal_setup() {
  signal(SIGINT, SIG_DFL);
  signal(SIGQUIT, SIG_DFL);
  signal(SIGTERM, SIG_DFL);
  signal(SIGTSTP, SIG_DFL); 
  signal(SIGCONT, SIG_DFL); 
  signal(SIGTTIN, SIG_DFL); 
  signal(SIGTTOU, SIG_DFL); 
}
