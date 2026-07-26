#include "calc.h"

int _div(double a, double b, double* result) {
  if (fabs(b) < EPS) return CALC_ERROR;
  *result = a / b;
  return SUCCESS;
}