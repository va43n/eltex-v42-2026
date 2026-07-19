#include "calc.h"

int calculate(char* formula, double* result) {
  *result = 0;
  operator actual_operators[4] = {{'+', 0}, {'-', 0}, {'*', 1}, {'/', 1}};
  double* numbers;
  operator* operators;
  size_t numbers_n, operators_n, actual_operators_n = 4;

  if (!_parse_formula(formula, actual_operators, actual_operators_n, &numbers,
                      &numbers_n, &operators, &operators_n)) {
    printf("ERROR\n");
    return ERROR;
  }

  for (size_t i = 0; i < numbers_n; i++) {
    printf("%lf, ", numbers[i]);
  }
  printf("\n");
  for (size_t i = 0; i < operators_n; i++) {
    printf("%c %ld, ", operators[i].symbol, operators[i].priority);
  }

  return SUCCESS;
}