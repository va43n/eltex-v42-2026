#include "calc.h"

void _add_number(double** numbers, size_t* numbers_n, double* number,
                 int* digit) {
  (*numbers_n)++;
  double* temp = (double*)realloc(*numbers, sizeof(double) * *numbers_n);
  *numbers = temp;
  (*numbers)[*numbers_n - 1] = *number;
  *digit = 0;
  *number = 0;
}