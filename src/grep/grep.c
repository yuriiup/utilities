#include "grep.h"

int main(int argc, char* argv[]) {
  ShortOptions options = {0};

  process_output(argc, argv, &options);

  return 0;
}

FILE* read_files(int index, char* argv[]) {
  FILE* fp = fopen(argv[index], "r");

  if (fp == NULL) {
    perror("Error occured while opening file");
    return NULL;
  }

  return fp;
}

void stream_output(FILE* fp) {
  int curr;
  while ((curr = fgetc(fp)) != EOF) {
    printf("%c", curr);
  }
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

    stream_output(fp);
    fclose(fp);
  }

  return 0;
}

int choice_options(int option, ShortOptions* options) {
  switch (option) {
    case 'e':
      options->pattern = 1;
      break;
    case 'i':
      options->ignore_case = 1;
      break;
    case 'v':
      options->invert_match = 1;
      break;
    case 'c':
      options->count_matching = 1;
      break;
    case 'l':
      options->matching_files = 1;
      break;
    case 'n':
      options->number_string = 1;
      break;
    // case 'h':
    //   options->matching_string = 1;
    //   break;
    // case 's':
    //   options->shadow_error = 1;
    //   break;
    // case 'f':
    //   options->regular_files = 1;
    //   break;
    // case 'o':
    //   options->matching_part = 1;
    //   break;
    case '?':
      return 1;
  }

  return 0;
}

int process_options(int argc, char* argv[], ShortOptions* options) {
  const char* shortopts = "e:ivclnhsf:o";

  int result = 0;
  int temp = 0;

  static struct option longopts[] = {
      {"regexp", required_argument, NULL, 'e'},
      {"ignore-case", no_argument, NULL, 'i'},
      {"invert_match", no_argument, NULL, 'v'},
      {"count", no_argument, NULL, 'c'},
      {"files-with-matches", no_argument, NULL, 'l'},
      {"line-number", no_argument, NULL, 'n'}};

  while ((result = getopt_long(argc, argv, shortopts, longopts, &temp)) != -1) {
    if (choice_options(result, options)) {
      return 1;
    }
  }

  return 0;
}
