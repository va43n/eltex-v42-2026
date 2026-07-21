#include "my_chmod.h"

int _convert_numbers_to_letters(const unsigned int numbers,
                                char* const letters) {
  for (int i = 9; i < 32; i++) {
    if (numbers & (1 << i)) {
      return ERROR;
    }
  }

  char all_letters[] = {READ_SYMBOL, WRITE_SYMBOL, EXEC_SYMBOL};
  size_t len = 3;
  for (int i = 8, j = 0; i >= 0; i--, j++)
    letters[j] =
        ((numbers & (1 << i)) > 0) ? all_letters[(j % len)] : BLANK_SYMBOL;

  letters[9] = '\0';

  return SUCCESS;
}