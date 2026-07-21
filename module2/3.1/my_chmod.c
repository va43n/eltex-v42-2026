#include "my_chmod.h"

int my_chmod_init(mod* const m, const char* const input) {
  unsigned int mask = 0;
  if (input[0] == READ_SYMBOL || input[0] == WRITE_SYMBOL ||
      input[0] == EXEC_SYMBOL || input[0] == BLANK_SYMBOL) {
    size_t len = strlen(input);
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
    unsigned int mask8 = 0;
    size_t len = strlen(input);
    if (len != 3) return ERROR;
    for (size_t i = 0; i < len; i++) {
      if (!isdigit(input[i])) return ERROR;
      mask8 *= 10;
      mask8 += input[i] - '0';
    }
    int power = 0;
    while (mask8 > 0) {
      mask += (mask8 % 10) * (int)powf(8, power);
      mask8 /= 10;
      power++;
    }

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
  printf("%3o\n", m.numbers);

  return SUCCESS;
}

int my_chmod_change(mod* const m, const char* const format) {
    return SUCCESS;
}