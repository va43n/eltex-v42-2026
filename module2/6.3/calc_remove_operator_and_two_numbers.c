#include "calc.h"

int _remove_operator_and_two_numbers(double** numbers,
                                     size_t* numbers_n, operator** operators,
                                     size_t* operators_n, size_t operator_i,
                                     double new_number) {
  size_t pos = (*operators)[operator_i].position;

  (*numbers)[pos] = new_number;
  for (size_t i = pos + 1; i < *numbers_n - 1; i++) {
    (*numbers)[i] = (*numbers)[i + 1];
  }
  (*numbers_n)--;
  if ((*numbers_n) != 0) {
    double* temp_num =
        (double*)realloc(*numbers, sizeof(double) * (*numbers_n));
    if (temp_num == NULL) return ERROR;
    *numbers = temp_num;
  }

  for (size_t i = operator_i; i < *operators_n - 1; i++) {
    (*operators)[i] = (*operators)[i + 1];
  }
  (*operators_n)--;
  if ((*operators_n) != 0) {
    operator* temp_op =(operator*)
        realloc(*operators, sizeof(operator) * (*operators_n));
    if (temp_op == NULL) return ERROR;
    *operators = temp_op;
  }

  for (size_t i = 0; i < *operators_n; i++) {
    if ((*operators)[i].position > pos) {
      (*operators)[i].position--;
    }
  }

  return SUCCESS;
}