#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS 1
#define ERROR 0
#define CALC_ERROR 2

#define EPS 1e-6

#define ASCII_0 (int)'0'
#define ASCII_9 (int)'9'
#define ASCII_DOT (int)'.'

// priority
// 0: *, /, %
// 1: +, -
// 2: <, >
// 3: &
// 4: ^
// 5: |

#define MAX_PRIORITY 0
#define MIN_PRIORITY 5

typedef struct {
  char symbol;
  size_t priority;
  size_t position;
  int (*operation)(double, double, double*);
} operator;

int calculate(char* formula, double* result);

int _sum(double a, double b, double* result);
int _sub(double a, double b, double* result);
int _mul(double a, double b, double* result);
int _div(double a, double b, double* result);

int _parse_formula(char* formula, operator* actual_operators,
                   size_t actual_operators_n, double** numbers,
                   size_t* numbers_n, operator** operators,
                   size_t* operators_n);
void _add_number(double** numbers, size_t* numbers_n, double* number,
                 int* digit);
void _add_operator(operator** operators,
                   size_t* operators_n, operator actual_operator,
                   size_t numbers_n);
int _check_operator_validity(operator* operators, size_t operators_n,
                             size_t numbers_n);
int _compute_result(double** numbers, size_t* numbers_n, operator** operators,
                    size_t* operators_n, double* result);
int _compute_single_operator(double** numbers,
                             size_t* numbers_n, operator** operators,
                             size_t* operators_n, size_t operator_i);
int _remove_operator_and_two_numbers(double** numbers,
                                     size_t* numbers_n, operator** operators,
                                     size_t* operators_n, size_t operator_i,
                                     double new_number);