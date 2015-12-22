#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 

int getWordCount(char *line) { 
  int count = 0, index = 0;
  while (line[index] != '\0') { 
    while (line[index] == ' ') { 
      ++index; 
    } 
    ++count;
    while (line[index] != '\0' && line[index] != ' ') { 
      ++index; 
    }
  }
  return count;
}

int getLineLength(char *line) { 
  int len = 0;
  while (line[len] != '\0') { 
    ++len; 
  }
  return len;
}

int main(int argc, char *argv[]) {
  FILE *input = NULL;
  char line[101];
  int wordCount = 0, lineCount = 0, byteCount = 0;
  if (argc == 2) {
    input = fopen(argv[1], "r");
  } else { 
    input = stdin; 
  }  

  if (input == NULL) { 
    printf("Error: File Not Found."); 
  } else { 
    while (fgets(line, 100, input)) { 
      ++lineCount;
      wordCount += getWordCount(line);
      byteCount += getLineLength(line);
    }
    printf("%d, %d, %d\n", lineCount, wordCount, byteCount);
  } 
}
