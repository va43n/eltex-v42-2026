#include "calc.h"

void _add_operator(operator** operators,
                   size_t* operators_n, operator actual_operator,
                   size_t numbers_n) {
  (*operators_n)++;
  operator* temp =(operator*)
      realloc(*operators, sizeof(operator) * *operators_n);
  *operators = temp;
  (*operators)[*operators_n - 1].symbol = actual_operator.symbol;
  (*operators)[*operators_n - 1].priority = actual_operator.priority;
  (*operators)[*operators_n - 1].position = numbers_n - 1;
}