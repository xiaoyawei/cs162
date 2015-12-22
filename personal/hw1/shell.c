#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

#include "io.h"
#include "parse.h"
#include "process.h"
#include "shell.h"


/* Whether the shell is connected to an actual terminal or not. */
bool shell_is_interactive;

/* File descriptor for the shell input */
int shell_terminal;

/* Terminal mode settings for the shell */
struct termios shell_tmodes;

/* Process group id for the shell */
pid_t shell_pgid;

/* Built-in command functions take token array (see parse.h) and return int */
typedef int cmd_fun_t(tok_t args[]);

/* Built-in command struct and lookup table */
typedef struct fun_desc {
  cmd_fun_t *fun;
  char *cmd;
  char *doc;
} fun_desc_t;

static fun_desc_t cmd_table[] = {
  {cmd_help, "?", "show this help menu"},
  {cmd_quit, "quit", "quit the command shell"},
  {cmd_pwd, "pwd", "get current directory"},
  {cmd_cd, "cd", "change curent directory"},
};

/**
 * Prints a helpful description for the given command
 */
int cmd_help(tok_t arg[]) {
  int num_fun = sizeof(cmd_table) / sizeof(fun_desc_t);
  for (int i = 0; i < num_fun; i++) {
    printf("%s - %s\n", cmd_table[i].cmd, cmd_table[i].doc);
  }
  return 1;
}

/**
 * Quits this shell
 */
int cmd_quit(tok_t arg[]) {
  exit(0);
  return 1;
}

/**
 * Get current directory
 */
int cmd_pwd(tok_t arg[]) { 
  char cwd[LONGEST_PATH_LENGTH]; 
  if (getcwd(cwd, sizeof(cwd)) != NULL) { 
    printf("%s\n", cwd); 
  } else { 
    perror("getcwd() error"); 
  } 
  return 1;
}

/**
 * Change current directory
 */
int cmd_cd(tok_t arg[]) { 
  if (arg[0] && chdir(arg[0])) { 
    perror("directory already exists"); 
  }
  return 1;
}

/**
 * Execute a specified program
 */
int sys_call(tok_t arg[]) {
  switch (fork()) {
    case -1:
      perror("Error program calling");
      break;
    case 0:
      execv(arg[0], arg);
      exit(0);
    default:
      wait(NULL);
      break;
  }
  return 1;
}

int lookup(char cmd[]) {
  int num_fun = sizeof(cmd_table) / sizeof(fun_desc_t);
  for (int i = 0; i < num_fun; i++) {
    if (cmd && (strcmp(cmd_table[i].cmd, cmd) == 0)) return i;
  }
  return -1;
}

/**
 * Intialization procedures for this shell
 */
void init_shell() {
  /* Check if we are running interactively */
  shell_terminal = STDIN_FILENO;
  shell_is_interactive = isatty(shell_terminal);

  if(shell_is_interactive){
    /* Force the shell into foreground */
    while(tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp()))
      kill(-shell_pgid, SIGTTIN);

    /* Saves the shell's process id */
    shell_pgid = getpid();

    /* Take control of the terminal */
    tcsetpgrp(shell_terminal, shell_pgid);
    tcgetattr(shell_terminal, &shell_tmodes);
  }
}

int shell(int argc, char *argv[]) {
  char *input_bytes;
  tok_t *tokens;
  int line_num = 0;
  int fundex = -1;

  init_shell();

  if (shell_is_interactive) { 
    /* Please only print shell prompts when standard input is not a tty */
    fprintf(stdout, "%d: ", line_num);
  }

  while ((input_bytes = freadln(stdin))) {
    tokens = get_toks(input_bytes);
    fundex = lookup(tokens[0]);
    if (fundex >= 0) {
      cmd_table[fundex].fun(&tokens[1]);
    } else {
        /* Execute the specified program */
      sys_call(tokens);
    }
    free(input_bytes);
    free_toks(tokens);

    if (shell_is_interactive) { 
      /* Please only print shell prompts when standard input is not a tty */
      fprintf(stdout, "%d: ", ++line_num);
    }
  }

  return 0;
}
