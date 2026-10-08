#include "cat.h"

int main(int argc, char* argv[]) {
  ShortOptions options = {0};

  process_output(argc, argv, &options);

  return 0;
}

void stream_output(FILE* fp) {
  char lines;

  while ((lines = fgetc(fp)) != EOF) {
    printf("%c", lines);
  }
  printf("\n");
}

void process_output(int argc, char* argv[], ShortOptions* options) {
  process_options(argc, argv, options);

  for (int i = optind; i < argc; ++i) {
    FILE* fp = read_files(i, argv);
    stream_output(fp);
    fclose(fp);
  }
}

FILE* read_files(int index, char* argv[]) {
  FILE* fp = fopen(argv[index], "r");
  if (fp == NULL) {
    perror("Empty file");
    return NULL;
  }

  return fp;
}

void process_options(int argc, char* argv[], ShortOptions* options) {
  const char* shortopts = "beEnstTv";

  int option;
  int temp;

  static struct option longopts[] = {{"number-nonblank", 0, NULL, 'b'},
                                     {"number", 0, NULL, 'n'},
                                     {"squeeze_blank", 0, NULL, 's'},
                                     {0, 0, 0, 0}};

  while ((option = getopt_long(argc, argv, shortopts, longopts, &temp)) != -1) {
    // pass
  }
}