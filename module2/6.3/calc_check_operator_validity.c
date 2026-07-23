#include "calc.h"

int _check_operator_validity(operator* operators, size_t operators_n,
                             size_t numbers_n) {
  if (operators_n == 0) return SUCCESS;
  for (size_t i = 0; i < operators_n; i++) {
    if (operators[i].position >= numbers_n - 1) return ERROR;
  }
  for (size_t i = 0; i < operators_n; i++) {
    for (size_t j = i + 1; j < operators_n; j++) {
      if (operators[i].position == operators[j].position) return ERROR;
    }
  }
  return SUCCESS;
}