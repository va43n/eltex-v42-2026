#include "calc.h"

int _compute_single_operator(double** numbers,
                             size_t* numbers_n, operator** operators,
                             size_t* operators_n, size_t operator_i) {
  double res;
  operator op =(*operators)[operator_i];
  switch (op.symbol) {
    case '+':
      if (_sum((*numbers)[op.position], (*numbers)[op.position + 1], &res) ==
          CALC_ERROR)
        return CALC_ERROR;
      break;
    case '-':
      if (_sub((*numbers)[op.position], (*numbers)[op.position + 1], &res) ==
          CALC_ERROR)
        return CALC_ERROR;
      break;
    case '*':
      if (_mul((*numbers)[op.position], (*numbers)[op.position + 1], &res) ==
          CALC_ERROR)
        return CALC_ERROR;
      break;
    case '/':
      if (_div((*numbers)[op.position], (*numbers)[op.position + 1], &res) ==
          CALC_ERROR)
        return CALC_ERROR;
      break;
  }

  if (!_remove_operator_and_two_numbers(numbers, numbers_n, operators,
                                        operators_n, operator_i, res))
    return ERROR;

  return SUCCESS;
}