#include "cat.h"

int main(int argc, char* argv[]) {
  ShortOptions options = {0};

  if (process_output(argc, argv, &options) == 1) {
    return 1;
  }

  return 0;
}

// сам поток вывода
void stream_output(FILE* fp, const ShortOptions* options, int* count_string) {
  char lines;

  char curr;
  char prev = 0;

  while ((lines = fgetc(fp)) != EOF) {
    curr = lines;

    if (options->number_nonblank) {
      number_nonblank_lines(options, prev, curr, count_string);
    } else {
      number_lines(options, prev, count_string);
    }

    printf("%c", curr);  // уточнить модификаторы

    prev = curr;
  }
  printf("\n");
}

// управление выводом из файла. вызов нужных функций
int process_output(int argc, char* argv[], ShortOptions* options) {
  // printf("%d", process_options(argc, argv, options));
  // printf("%d", !process_options(argc, argv, options));
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

// функция возвращает указатель на файл чтения
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

// существует структура "состояний" коротких опций shortopts
// существует структура, которая содержит параметры длинных опций longopts
int process_options(int argc, char* argv[], ShortOptions* options) {
  const char* shortopts = "beEnstTv";

  int result;
  int temp;

  // внутренняя структура option объявленна в getopt.h
  // сигнатура name, arg, flag, val
  static struct option longopts[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze_blank", no_argument, NULL, 's'},
      {0, 0, 0, 0}};

  while ((result = getopt_long(argc, argv, shortopts, longopts, &temp)) != -1) {
    if (coice_options(result, options)) {
      // если не прошло по флагам
      return 1;
    }
  }

  if (options->number_nonblank == 1) {
    options->number = 0;
  }

  // printf("%d", options->number);
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