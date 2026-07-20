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
  if (fabs(b) < EPS) return CALC_ERROR;
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