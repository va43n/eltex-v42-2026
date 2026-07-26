#include "my_chmod.h"

int _get_numbers_from_input(const char* const input, unsigned int* mask) {
  *mask = 0;
  size_t len = strlen(input);
  if (len != 3) return ERROR;
  unsigned int mask8 = 0;
  for (size_t i = 0; i < len; i++) {
    if (!isdigit(input[i]) || input[i] - '0' >= 8) return ERROR;
    mask8 *= 10;
    mask8 += input[i] - '0';
  }
  int power = 0;
  while (mask8 > 0) {
    *mask += (mask8 % 10) * (int)powf(8, power);
    mask8 /= 10;
    power++;
  }

  return SUCCESS;
}

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

int _parse_complex_input(const char* const input, unsigned int* mask,
                         int* mode) {
  *mask = 0;
  unsigned int rwx_mask = 0;
  size_t len = strlen(input);

  int is_operator_set = 0;

  for (size_t i = 0; i < len; i++) {
    if (input[i] == READ_SYMBOL || input[i] == WRITE_SYMBOL ||
        input[i] == EXEC_SYMBOL) {
      if (!is_operator_set) return ERROR;

      if (input[i] == READ_SYMBOL)
        rwx_mask |= READ;
      else if (input[i] == WRITE_SYMBOL)
        rwx_mask |= WRITE;
      else
        rwx_mask |= EXEC;
    } else if (input[i] == PLUS || input[i] == MINUS || input[i] == EQUAL) {
      if (is_operator_set) return ERROR;
      if (*mask == 0) *mask = USER | GROUP | OTHER;
      is_operator_set = 1;
      if (input[i] == PLUS)
        *mode = PLUS_MODE;
      else if (input[i] == MINUS)
        *mode = MINUS_MODE;
      else
        *mode = EQUAL_MODE;
    } else if (input[i] == USER_SYMBOL || input[i] == GROUP_SYMBOL ||
               input[i] == OTHER_SYMBOL || input[i] == ALL_SYMBOL) {
      if (is_operator_set) return ERROR;

      if (input[i] == USER_SYMBOL)
        *mask |= USER;
      else if (input[i] == GROUP_SYMBOL)
        *mask |= GROUP;
      else if (input[i] == OTHER_SYMBOL)
        *mask |= OTHER;
      else
        *mask |= USER | GROUP | OTHER;
    } else
      return ERROR;
  }
  if (!is_operator_set) return ERROR;

  if (rwx_mask == 0 && (*mode == PLUS_MODE || *mode == MINUS_MODE)) {
    *mask = 0;
    *mode = PLUS_MODE;
  } else
    *mask &= rwx_mask;

  return SUCCESS;
}