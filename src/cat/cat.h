#ifndef CAT_H
#define CAT_H

#include <stdio.h>

// flags
typedef struct {
  int number_nonblank;
  int number;
  int ends;
  int squeeze_blank;
  int tabs;
  int special;
} Options;

void output(FILE* fp);
void read_files(char* name);

#endif