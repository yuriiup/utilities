#ifndef CAT_H
#define CAT_H

#include <getopt.h>
#include <stdio.h>

// shortopts
typedef struct {
  int number_nonblank;  // b
  int number;           // n
  int ends;             // e
  int squeeze_blank;    // s
  int tabs;             // t
  int special;         // v
} ShortOptions;

FILE* read_files(int index, char* argv[]);

void stream_output(FILE* fp);
int process_output(int argc, char* argv[], ShortOptions* options);

int process_options(int argc, char* argv[], ShortOptions* options);
int coice_options(int option, ShortOptions* options);

#endif