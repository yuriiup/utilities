#include "cat.h"

int main(int argc, char* argv[]) {
  ShortOptions options = {0};

  if (process_output(argc, argv, &options) == 1) {
    return 1;
  }

  return 0;
}

void stream_output(FILE* fp, const ShortOptions* options, int* count_string) {
  char curr;
  char prev = 0;

  int elines;

  while ((curr = fgetc(fp)) != EOF) {
    if (options->squeeze_blank) {
      if (squeeze_lines(options, prev, curr, &elines)) {
        prev = curr;
        continue;
      }
    }

    if (options->number_nonblank) {
      number_nonblank_lines(options, prev, curr, count_string);
    } else {
      number_lines(options, prev, count_string);
    }

    if (options->special) {
      if (print_special(options, curr)) {
        prev = curr;
        continue;
      }
    } else {
      print_ends_lines(options, curr);
      if (print_tabs(options, curr)) {
        prev = curr;
        continue;
      }
    }

    printf("%c", curr);
    prev = curr;
  }
}

int process_output(int argc, char* argv[], ShortOptions* options) {
  int count_string = 1;

  if (process_options(argc, argv, options)) {
    return 1;
  }

  for (int i = optind; i < argc; ++i) {
    FILE* fp = read_files(i, argv);

    if (fp == NULL) {
      return 1;
    }

    stream_output(fp, options, &count_string);
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
    case 'E':
      options->ends = 1;
      break;
    case 'e':
      options->special = 1;
      options->ends = 1;
      break;
    case 's':
      options->squeeze_blank = 1;
      break;
    case 'T':
      options->tabs = 1;
      break;
    case 't':
      options->special = 1;
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
      {NULL, 0, NULL, 0}};

  while ((result = getopt_long(argc, argv, shortopts, longopts, &temp)) != -1) {
    if (coice_options(result, options)) {
      return 1;
    }
  }

  if (options->number_nonblank == 1) {
    options->number = 0;
  }

  return 0;
}

void number_lines(const ShortOptions* options, char symbol, int* lines) {
  if (options->number && (symbol == '\n' || symbol == 0)) {
    printf("%6d  ", *lines);
    (*lines)++;
  }
}

void number_nonblank_lines(const ShortOptions* options, char prev, char curr,
                           int* lines) {
  if (options->number_nonblank && curr != '\n' && (prev == '\n' || prev == 0)) {
    printf("%6d  ", *lines);
    (*lines)++;
  }
}

void print_ends_lines(const ShortOptions* options, char curr) {
  if (options->ends && curr == '\n') {
    printf("%c", '$');
  }
}

int squeeze_lines(const ShortOptions* options, char prev, char curr,
                  int* elines) {
  int pass = 0;

  if (options->squeeze_blank && (prev == '\n' && curr == '\n')) {
    (*elines)++;
  } else {
    *elines = 0;
  }

  if (*elines > 1) {
    pass = 1;
  }

  return pass;
}

int print_tabs(const ShortOptions* options, char curr) {
  int pass = 0;
  if (options->tabs && curr == '\t') {
    printf("%s", "^I");
    pass = 1;
  }

  return pass;
}

int print_special(const ShortOptions* options, int curr) {
  int pass = 0;
  if (options->special) {
    if (curr >= 0 && curr <= CONTROL_CHAR && curr != '\n' && curr != '\t') {
      printf("%s", "^M");
      pass = 1;
    } else if (curr >= EXTENDED_CHAR && curr <= 255) {
      printf("%s", "-M");
      pass = 1;
    }

    if (options->tabs && curr == '\t') {
      pass = print_tabs(options, curr);
    }

    if (options->ends && curr == '\n') {
      print_ends_lines(options, curr);
    }
  }

  return pass;
}