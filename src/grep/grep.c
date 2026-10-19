#include "grep.h"

int main(int argc, char* argv[]) {
  ShortOptions options = {0};
  Patterns patterns = {0};

  process_output(argc, argv, &options, &patterns);

  return 0;
}

FILE* open_files(int index, char* argv[]) {
  FILE* fp = fopen(argv[index], "r");

  // если файла не существует
  if (fp == NULL) {
    printf("%s ", argv[index]);
    perror("Error occured while opening file");
    return NULL;
  }

  return fp;
}

int read_string(FILE* fp, const char* pattern) {
  // чтение строк из файла
  ssize_t read;
  char* string = NULL;
  size_t length = 0;

  if (pattern == NULL) {
    return 1;
  }

  // использование getline для получения строк и передча в regex
  while ((read = getline(&string, &length, fp)) != -1) {
    if (search_match(string, pattern)) {
      stream_output(string);  // вывод
    }
  }

  free(string);
  return 0;
}

void stream_output(char* string) { printf("%s", string); }

int process_output(int argc, char* argv[], ShortOptions* options, Patterns* patterns) {
  char* pattern = NULL;
  if (process_options(argc, argv, options, patterns)) {
    return 1;
  }

  pattern = process_pattern(argv, options, patterns);
  
  // получение указателя на файл
  for (int i = optind; i < argc; ++i) {
    FILE* fp = open_files(i, argv);

    if (fp == NULL) {
      continue;
    }
    
    printf("%s", pattern);
    if (read_string(fp, pattern)) {
      return 1;
    }

    fclose(fp);
  }

  return 0;
}

int choice_options(int option, ShortOptions* options, Patterns* patterns) {
  switch (option) {
    case 'e':
      options->pattern = 1;
      patterns->template = optarg;
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

int process_options(int argc, char* argv[], ShortOptions* options, Patterns* patterns) {
  // принятие флагов
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
    if (choice_options(result, options, patterns)) {  // активация выбранных флагов
      return 1;
    }
  }

  return 0;
}

int search_match(char* string, const char* pattern) {
  regex_t re;
  regmatch_t pmatch[1];
  int result = 0;
  int cflags = REG_EXTENDED;

  if (regcomp(&re, pattern, cflags)) {
    perror("regex compile failed");
    return result;
  }

  // результат поиска
  result = regexec(&re, string, 1, pmatch, 0);

  // обработка результата
  if (!result) {
    result = 1;
  } else if (result == REG_NOMATCH) {
    result = 0;
  } else {
    perror("regexec failed");
    result = 0;
  }

  regfree(&re);
  return result;
}


char* process_pattern(char* argv[], ShortOptions* options, Patterns* patterns) {
  char* pattern = NULL;

  if (options->pattern) {
    pattern = patterns->template;
    printf("%s", optarg);
  } else {
    pattern = argv[optind];
    optind++;
  }

  return pattern;
}