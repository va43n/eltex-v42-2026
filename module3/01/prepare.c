#include "header.h"

int parse_input(int argc, char* argv[], char*** file_names, char*** pipe_names,
                size_t* size) {
  *file_names = NULL;
  *pipe_names = NULL;
  *size = 0;

  if (argc == 1) {
    fprintf(stderr, "ERROR: PARSE_INPUT - there are no filenames in input.\n");
    return FAILURE;
  }

  for (int i = 1; i < argc; i++) {
    if (check_if_flag(argv, i)) {
      if (i + 2 >= argc) {
        fprintf(stderr,
                "ERROR: PARSE_INPUT - after flag -p there should be at least 2 "
                "arguments: name of the FIFO and name of the file.\n");
        return FAILURE;
      }

      i++;
      put_str_in_array(pipe_names, *size, argv[i]);

      i++;
      put_str_in_array(file_names, *size, argv[i]);
    } else {
      put_str_in_array(file_names, *size, argv[i]);
      put_str_in_array(pipe_names, *size, "\0");
    }

    (*size)++;
  }

  return SUCCESS;
}

int check_if_flag(char* argv[], int index) {
  if (strlen(argv[index]) != 2) return FALSE;
  if (argv[index][0] != '-' || argv[index][1] != FIFO_FLAG_SYMBOL) return FALSE;

  return TRUE;
}

int put_str_in_array(char*** array, size_t size, char* str) {
  char** temp = (char**)realloc(*array, sizeof(char*) * (size + 1));

  *array = temp;
  (*array)[size] = (char*)malloc(sizeof(char) * (strlen(str)));
  strcpy((*array)[size], str);

  return SUCCESS;
}