#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "io.h"

/**
 * Reads a line from the input file and returns a string object.
 */
char *freadln(FILE *ifile) {
  char line[MAXLINE];
  char *r = NULL;
  int len;
  char *s = fgets((char *)line, MAXLINE, ifile);
  if (!s) {
    return s;
  } else { 
    len = (int) strlen(s) + 1;
    r = (char*) malloc((unsigned) len);
    strncpy(r, s, (unsigned) len);
    return r;
  } 
}

/**
 * Frees the memory allocated to a character array.
 */
void freeln(char *ln) {
  free(ln);
}
