#include "calc.h"

int _sum(double a, double b, double* result) {
  *result = a + b;
  return SUCCESS;
}

int _sub(double a, double b, double* result) {
  *result = a - b;
  return SUCCESS;
}

int _mul(double a, double b, double* result) {
  *result = a * b;
  return SUCCESS;
}

int _div(double a, double b, double* result) {
  if (fabs(b) < EPS) return ERROR;
  *result = a / b;
  return SUCCESS;
}

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

  for (size_t i = 0; i < strlen(formula); i++) {
    if ((int)formula[i] >= ASCII_0 && (int)formula[i] <= ASCII_9) {
      if (digit >= 0) {
        number = number * 10.0 + (formula[i] - '0');
        digit++;
      } else {
        number = number + (formula[i] - '0') * pow(10.0, digit);
        digit--;
      }
    } else if ((int)formula[i] == ASCII_DOT) {
      digit = -1;
    } else {
      if (digit != 0) _add_number(numbers, numbers_n, &number, &digit);

      int operator_found = 0;
      for (size_t j = 0; j < actual_operators_n && !operator_found; j++) {
        if (formula[i] == actual_operators[j].symbol) {
          _add_operator(operators, operators_n, actual_operators[j]);

          operator_found = 1;
        }
      }
      if (!operator_found) {
        free(*numbers);
        free(*operators);
        return ERROR;
      }
    }
  }
  if (digit != 0) _add_number(numbers, numbers_n, &number, &digit);

  return SUCCESS;
}

void _add_number(double** numbers, size_t* numbers_n, double* number,
                 int* digit) {
  (*numbers_n)++;
  double* temp = (double*)realloc(*numbers, sizeof(double) * *numbers_n);
  *numbers = temp;
  (*numbers)[*numbers_n - 1] = *number;
  *digit = 0;
  *number = 0;
}

void _add_operator(operator** operators,
                   size_t* operators_n, operator actual_operator) {
  (*operators_n)++;
  operator* temp =(operator*)
      realloc(*operators, sizeof(operator) * *operators_n);
  *operators = temp;
  (*operators)[*operators_n - 1].symbol = actual_operator.symbol;
  (*operators)[*operators_n - 1].priority = actual_operator.priority;
}