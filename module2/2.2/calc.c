#include "calc.h"

int calculate(char* formula, double* result) {
  *result = 0;
  operator actual_operators[4] = {
      {'+', 0, 0}, {'-', 0, 0}, {'*', 1, 0}, {'/', 1, 0}};
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
  printf("\n%ld\n\n", numbers_n);
  for (size_t i = 0; i < operators_n; i++) {
    printf("%c %ld, ", operators[i].symbol, operators[i].priority);
  }
  printf("\n%ld\n\n", operators_n);

  if (!_check_operator_validity(operators, operators_n, numbers_n)) {
    printf("ERROR\n");
    return ERROR;
  }

  if (!_compute_result(&numbers, &numbers_n, &operators, &operators_n,
                       result)) {
    printf("ERROR\n");
    return ERROR;
  }

  return SUCCESS;
}