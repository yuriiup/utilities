#ifndef GREP_H
#define GREP_H

#define _GNU_SOURCE
#define BUFFER 4096

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

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

FILE* open_files(int index, char* argv[]);
void read_string(FILE* fp, char* argv[]);

void stream_output(char* string);
int process_output(int argc, char* argv[], ShortOptions* options);

int process_options(int argc, char* argv[], ShortOptions* options);
int choice_options(int option, ShortOptions* options);

int finding_match(char* string, const char* pattern);

#endif