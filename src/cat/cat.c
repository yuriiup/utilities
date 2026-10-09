#include "cat.h"

int main(int argc, char* argv[]) {
  ShortOptions options = {0};

  if (process_output(argc, argv, &options) == 1) {
    return 1;
  }

  return 0;
}

void stream_output(FILE* fp, const ShortOptions* options) {
  char lines;

  char curr;
  char prev = 0;

  int count_string = 1;

  while ((lines = fgetc(fp)) != EOF) {
    curr = lines;
    number_lines(options, prev, &count_string);
    printf("%c", curr);

    prev = curr;
  }
  printf("\n");
}

int process_output(int argc, char* argv[], ShortOptions* options) {
  if (process_options(argc, argv, options)) {
    return 1;
  }

  for (int i = optind; i < argc; ++i) {
    FILE* fp = read_files(i, argv);

    if (fp == NULL) {
      return 1;
    }

    stream_output(fp, options);
    fclose(fp);
  }

  return 0;
}

FILE* read_files(int index, char* argv[]) {
  FILE* fp = fopen(argv[index], "r");
  if (fp == NULL) {
    perror("Empty file");
    return NULL;
  }

  return fp;
}

int coice_options(int option, ShortOptions* options) {
  switch (option) {
    case 'b':
      options->number_nonblank = 1;
      break;
    case 'n':
      options->number = 1;
      break;
    case 'e':
      options->ends = 1;
      options->special = 1;
      break;
    case 'E':
      options->ends = 1;
      break;
    case 's':
      options->squeeze_blank = 1;
      break;
    case 't':
      options->tabs = 1;
      options->special = 1;
      break;
    case 'T':
      options->tabs = 1;
      break;
    case 'v':
      options->special = 1;
      break;
    case '?':
      return 1;
  }

  return 0;
}

int process_options(int argc, char* argv[], ShortOptions* options) {
  const char* shortopts = "beEnstTv";

  int result;
  int temp;

  static struct option longopts[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze_blank", no_argument, NULL, 's'},
      {0, 0, 0, 0}};

  while ((result = getopt_long(argc, argv, shortopts, longopts, &temp)) != -1) {
    if (coice_options(result, options)) {
      return 1;
    }
  }

  return 0;
}

void number_lines(const ShortOptions* options, char symbol, int* lines) {
  if (options->number && (symbol == '\n' || symbol == 0)) {
    printf("%6d  ", *lines);
    (*lines)++;
  }
}