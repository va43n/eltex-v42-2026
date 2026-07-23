#include "calc.h"

int _compute_result(double** numbers, size_t* numbers_n, operator** operators,
                    size_t* operators_n, double* result) {
  for (size_t priority = MAX_PRIORITY; priority <= MIN_PRIORITY; priority++) {
    size_t i = 0;
    while (i < *operators_n) {
      if ((*operators)[i].priority == priority) {
        if (_compute_single_operator(numbers, numbers_n, operators, operators_n,
                                     i) == CALC_ERROR)
          return CALC_ERROR;
      } else {
        i++;
      }
    }
  }

  *result = (*numbers)[0];

  return SUCCESS;
}