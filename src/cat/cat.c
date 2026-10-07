#include "cat.h"

int main(int argc, char* argv[]) {
  read_files(argv[1]);
  return 0;
}

void read_files(char* name) {
  FILE* fp = fopen(name, "r");
  output(fp);
}

void output(FILE* fp) {
  char lines;

  while ((lines = fgetc(fp)) != EOF) {
    printf("%c", lines);
  }
}