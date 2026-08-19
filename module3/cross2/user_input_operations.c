#include "taxi.h"

int get_input(char* buffer) {
  printf("> ");
  if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
    if (errno == EINTR) {
      fprintf(stderr, "get_input - interrupted by signal.\n");
      return INTERRUPTION;
    }
    fprintf(stderr, "ERROR: get_input - cannot get an input.\n");
    return FAILURE;
  }

  buffer[strlen(buffer) - 1] = '\0';

  return SUCCESS;
}

void tokenize_input(char* buffer, int* number_of_tokens,
                    char tokens[][BUFFER_SIZE]) {
  *number_of_tokens = 0;

  const char* delim = " ";
  const char* token = strtok(buffer, delim);

  while (token != NULL && *number_of_tokens < MAX_TOKENS_NUMBER) {
    (*number_of_tokens)++;
    strcpy(tokens[*number_of_tokens - 1], token);
    token = strtok(NULL, delim);
  }
}

int parse_str_to_uint(const char* str, unsigned int* number) {
  int result = sscanf(str, "%u", number);

  if (result == 0) {
    fprintf(stderr, "ERROR: parse_str_to_uint - %s in not a uint.\n", str);
    return FAILURE;
  }
  if (str[0] == '-') {
    fprintf(stderr, "ERROR: parse_str_to_uint - %s is a negative number.\n",
            str);
    return FAILURE;
  }

  return SUCCESS;
}
