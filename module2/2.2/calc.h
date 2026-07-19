#include <math.h>
#include <regex.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS 1
#define ERROR 0

#define EPS 1e-6

#define ASCII_0 (int)'0'
#define ASCII_9 (int)'9'
#define ASCII_DOT (int)'.'

typedef struct {
  char symbol;
  size_t priority;
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
                   size_t* operators_n, operator actual_operator);