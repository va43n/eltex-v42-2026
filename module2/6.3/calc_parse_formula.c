#include "calc.h"

int _parse_formula(char* formula, operator* actual_operators,
                   size_t actual_operators_n, double** numbers,
                   size_t* numbers_n, operator** operators,
                   size_t* operators_n) {
  *numbers = NULL;
  *operators = NULL;
  *numbers_n = 0;
  *operators_n = 0;

  double number = 0;
  int digit = 0;
  int dot_started = 0;

  for (size_t i = 0; i < strlen(formula); i++) {
    if ((int)formula[i] >= ASCII_0 && (int)formula[i] <= ASCII_9) {
      if (digit >= 0) {
        number = number * 10.0 + (formula[i] - '0');
        digit++;
      } else {
        number = number + (formula[i] - '0') * pow(10.0, digit);
        digit--;
      }
      dot_started = 0;
    } else if ((int)formula[i] == ASCII_DOT) {
      if (digit <= 0) return ERROR;
      digit = -1;
      dot_started = 1;
    } else {
      if (dot_started) return ERROR;
      if (digit != 0) _add_number(numbers, numbers_n, &number, &digit);

      int operator_found = 0;
      for (size_t j = 0; j < actual_operators_n && !operator_found; j++) {
        if (formula[i] == actual_operators[j].symbol) {
          _add_operator(operators, operators_n, actual_operators[j],
                        *numbers_n);

          operator_found = 1;
        }
      }
      if (!operator_found) return ERROR;
    }
  }
  if (dot_started) return ERROR;
  if (digit != 0) _add_number(numbers, numbers_n, &number, &digit);

  return SUCCESS;
}