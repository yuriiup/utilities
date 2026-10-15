#ifndef GREP_H
#define GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>

typedef struct {
  int pattern;
  int ignore_case;
  int invert_match;
  int count_matching;
  int matching_files;
  int number_string;
  int matching_string;
  int shadow_error;
  int regular_files;
  int matching_part;
} ShortOptions;

FILE* read_files(int index, char* argv[]);

void stream_output(FILE* fp);
int process_output(int argc, char* argv[], ShortOptions* options);

int process_options(int argc, char* argv[], ShortOptions* options);
int choice_options(int option, ShortOptions* options);

#endif