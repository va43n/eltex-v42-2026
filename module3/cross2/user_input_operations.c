#include "taxi.h"

int get_input(char* buffer) {
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

  char* delim = " ";
  char* token = strtok(buffer, delim);

  while (token != NULL && *number_of_tokens < MAX_TOKENS_NUMBER) {
    (*number_of_tokens)++;
    strcpy(tokens[*number_of_tokens - 1], token);
    token = strtok(NULL, delim);
  }
}