#include "calc.h"

int main() {
  char formula[] = "123-21*1.2/0";
  double result;

  printf("%d\n", calculate(formula, &result));
  printf("result: %lf\n", result);

  return 0;
}