#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <unistd.h>
// #include "calc.h"

int main() {
  const char* const libs[] = {"./calc_add_number.so",
                              "./calc_add_operator.so",
                              "./calc_calculate.so",
                              "./calc_check_operator_validity.so",
                              "./calc_compute_result.so",
                              "./calc_compute_single_operator.so",
                              "./calc_div.so",
                              "./calc_mul.so",
                              "./calc_parse_formula.so",
                              "./calc_remove_operator_and_two_numbers.so",
                              "./calc_sub.so",
                              "./calc_sum.so"};
  void* handles[] = {NULL, NULL, NULL, NULL, NULL, NULL,
                     NULL, NULL, NULL, NULL, NULL, NULL};
  size_t calculate_position = 0;
  size_t libs_n = sizeof(libs) / sizeof(libs[0]);
  for (size_t i = 0; i < libs_n; i++) {
    handles[i] = dlopen(libs[i], RTLD_LAZY | RTLD_GLOBAL);
    if (!handles[i]) {
      fprintf(stderr, "dlopen error for %s: %s\n", libs[i], dlerror());
      return 1;
    }
    if (strcmp(libs[i], "./calc_calculate.so") == 0) calculate_position = i;
  }

  int (*calculate)(char*, double*) =
      (int (*)(char*, double*))dlsym(handles[calculate_position], "calculate");
  char* error = dlerror();
  if (error) {
    fprintf(stderr, "dlsym error: %s\n", error);
    return 1;
  }

  char formula[] = "123-21*1.2/2.23";
  double result;

  int ret = calculate(formula, &result);
  printf("%d\n", ret);
  printf("result: %lf\n", result);

  for (size_t i = 0; i < libs_n; i++) dlclose(handles[i]);

  return 0;
}