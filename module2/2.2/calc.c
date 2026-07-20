#include "calc.h"

int calculate(char* formula, double* result) {
  *result = 0;
  operator actual_operators[4] = {
      {'+', 1, 0}, {'-', 1, 0}, {'*', 0, 0}, {'/', 0, 0}};
  double* numbers;
  operator* operators;
  size_t numbers_n, operators_n, actual_operators_n = 4;

  if (!_parse_formula(formula, actual_operators, actual_operators_n, &numbers,
                      &numbers_n, &operators, &operators_n)) {
    if (numbers != NULL) free(numbers);
    if (operators != NULL) free(operators);
    printf("parse ERROR\n");
    return ERROR;
  }

  if (!_check_operator_validity(operators, operators_n, numbers_n)) {
    if (numbers != NULL) free(numbers);
    if (operators != NULL) free(operators);
    printf("validity ERROR\n");
    return ERROR;
  }

  if (_compute_result(&numbers, &numbers_n, &operators, &operators_n, result) ==
      CALC_ERROR) {
    if (numbers != NULL) free(numbers);
    if (operators != NULL) free(operators);
    printf("CALCULATION ERROR\n");
    return CALC_ERROR;
  }

  if (numbers != NULL) free(numbers);
  if (operators != NULL) free(operators);

  return SUCCESS;
}