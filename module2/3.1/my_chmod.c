#include "my_chmod.h"

int my_chmod_init(mod* const m, const char* const input) {
  size_t len = strlen(input);
  if (len == 0) return ERROR;
  unsigned int mask = 0;
  if (input[0] == READ_SYMBOL || input[0] == WRITE_SYMBOL ||
      input[0] == EXEC_SYMBOL || input[0] == BLANK_SYMBOL) {
    if (len != 9) return ERROR;
    char all_letters[] = {READ_SYMBOL, WRITE_SYMBOL, EXEC_SYMBOL};
    size_t all_letters_len = 3;
    for (size_t i = 0; i < len; i++) {
      char expected_char = all_letters[(i) % all_letters_len];
      if (input[i] == expected_char)
        mask |= (1 << (len - 1 - i));
      else if (input[i] != BLANK_SYMBOL)
        return ERROR;
    }
    strcpy((*m).letters, input);
    (*m).numbers = mask;
  } else {
    if (!_get_numbers_from_input(input, &mask)) return ERROR;

    (*m).numbers = mask;
    if (!_convert_numbers_to_letters((*m).numbers, (*m).letters)) return ERROR;
  }

  return SUCCESS;
}

int my_chmod_init_with_file(mod* const m, const char* const file_name) {
  struct stat file_stat;

  if (stat(file_name, &file_stat) == -1) return ERROR;

  (*m).numbers = file_stat.st_mode & (USER | GROUP | OTHER);
  if (!_convert_numbers_to_letters((*m).numbers, (*m).letters)) return ERROR;

  return SUCCESS;
}

int my_chmod_print(const mod m) {
  for (int i = 8; i >= 0; i--) printf("%d", (m.numbers & 1 << i) > 0);
  printf("\n");
  printf("%s\n", m.letters);
  printf("%03o\n\n", m.numbers);

  return SUCCESS;
}

int my_chmod_change(mod* const m, const char* const format) {
  size_t len = strlen(format);
  if (len == 0) return ERROR;
  if (format[0] == USER_SYMBOL || format[0] == GROUP_SYMBOL || format[0] == OTHER_SYMBOL || format[0] == ALL_SYMBOL || format[0] == PLUS || format[0] == MINUS || format[0] == EQUAL) {
    unsigned int mask = 0;
    int mode;
    if (!_parse_complex_input(format, &mask, &mode)) return ERROR;

    if (mode == PLUS_MODE) (*m).numbers |= mask;
    else if (mode == MINUS_MODE) (*m).numbers &= ~mask;
    else if (mode == EQUAL_MODE) (*m).numbers = mask;

    if (!_convert_numbers_to_letters((*m).numbers, (*m).letters)) return ERROR;
  }
  else if (isdigit(format[0])) {
    unsigned int mask = 0;
    if (!_get_numbers_from_input(format, &mask)) return ERROR;

    (*m).numbers = mask;
    if (!_convert_numbers_to_letters((*m).numbers, (*m).letters)) return ERROR;
  }
  else return ERROR;
  
  return SUCCESS;
}