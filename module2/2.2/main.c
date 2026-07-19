#include "calc.h"

int main() {
  char formula[] =
      "-123+-21+3-5.7/1*2000.001+1.23+1.234+1.2345+1+12+123+1234+12345+123.45";
  double result;

  printf("%d", calculate(formula, &result));

  return 0;
}