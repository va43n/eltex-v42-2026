#include "test.h"

START_TEST(check_calculation_1) {
  double result;

  ck_assert_int_eq(calc_func("0", &result), 1);
  ck_assert_double_eq_tol(result, 0.0, 1e-6);

  ck_assert_int_eq(calc_func("1", &result), 1);
  ck_assert_double_eq_tol(result, 1.0, 1e-6);

  ck_assert_int_eq(calc_func("2.345", &result), 1);
  ck_assert_double_eq_tol(result, 2.345, 1e-6);

  ck_assert_int_eq(calc_func("0+0", &result), 1);
  ck_assert_double_eq_tol(result, 0.0, 1e-6);

  ck_assert_int_eq(calc_func("0*0", &result), 1);
  ck_assert_double_eq_tol(result, 0.0, 1e-6);

  ck_assert_int_eq(calc_func("0-0", &result), 1);
  ck_assert_double_eq_tol(result, 0.0, 1e-6);

  ck_assert_int_eq(calc_func("1+0", &result), 1);
  ck_assert_double_eq_tol(result, 1.0, 1e-6);

  ck_assert_int_eq(calc_func("0+1", &result), 1);
  ck_assert_double_eq_tol(result, 1.0, 1e-6);

  ck_assert_int_eq(calc_func("1-0", &result), 1);
  ck_assert_double_eq_tol(result, 1.0, 1e-6);

  ck_assert_int_eq(calc_func("0+1", &result), 1);
  ck_assert_double_eq_tol(result, 1.0, 1e-6);

  ck_assert_int_eq(calc_func("1*0", &result), 1);
  ck_assert_double_eq_tol(result, 0.0, 1e-6);

  ck_assert_int_eq(calc_func("0*1", &result), 1);
  ck_assert_double_eq_tol(result, 0.0, 1e-6);

  ck_assert_int_eq(calc_func("1+0.1", &result), 1);
  ck_assert_double_eq_tol(result, 1.1, 1e-6);

  ck_assert_int_eq(calc_func("1*0.1", &result), 1);
  ck_assert_double_eq_tol(result, 0.1, 1e-6);

  ck_assert_int_eq(calc_func("1+2+3+4", &result), 1);
  ck_assert_double_eq_tol(result, 10.0, 1e-6);

  ck_assert_int_eq(calc_func("1-2+3-4", &result), 1);
  ck_assert_double_eq_tol(result, -2.0, 1e-6);

  ck_assert_int_eq(calc_func("2+2*2", &result), 1);
  ck_assert_double_eq_tol(result, 6.0, 1e-6);

  ck_assert_int_eq(calc_func("2+2*2-2/2", &result), 1);
  ck_assert_double_eq_tol(result, 5.0, 1e-6);

  ck_assert_int_eq(calc_func("1.2*2.3+3.4*4.5", &result), 1);
  ck_assert_double_eq_tol(result, 18.06, 1e-6);

  ck_assert_int_eq(calc_func("1+2*3-4/5+6/7-8.9+0.1", &result), 1);
  ck_assert_double_eq_tol(result, -1.7428571429, 1e-6);
}

START_TEST(check_calculation_2) {
  double result;

  ck_assert_int_eq(calc_func("0/1", &result), 1);
  ck_assert_double_eq_tol(result, 0.0, 1e-6);

  ck_assert_int_eq(calc_func("1/0", &result), 2);

  ck_assert_int_eq(calc_func("1+2/0", &result), 2);

  ck_assert_int_eq(calc_func("0/0", &result), 2);

  ck_assert_int_eq(calc_func("1.234+5.6789/0", &result), 2);

  ck_assert_int_eq(calc_func("1/0*2+3+4+5-17", &result), 2);

  ck_assert_int_eq(calc_func("1*2+3+4+5/0-17", &result), 2);
}

START_TEST(check_calculation_3) {
  double result;

  ck_assert_int_eq(calc_func("+", &result), 0);

  ck_assert_int_eq(calc_func("+1", &result), 0);

  ck_assert_int_eq(calc_func("+1+2", &result), 0);

  ck_assert_int_eq(calc_func("2+", &result), 0);

  ck_assert_int_eq(calc_func("1+2+", &result), 0);

  ck_assert_int_eq(calc_func("1++2", &result), 0);

  ck_assert_int_eq(calc_func("1++2-3", &result), 0);

  ck_assert_int_eq(calc_func("3-1++2", &result), 0);

  ck_assert_int_eq(calc_func("1 - 2", &result), 0);

  ck_assert_int_eq(calc_func("5%2", &result), 0);

  ck_assert_int_eq(calc_func("1+2+3%4-5-6-7", &result), 0);

  ck_assert_int_eq(calc_func("1++", &result), 0);

  ck_assert_int_eq(calc_func("1//2++3**4", &result), 0);

  ck_assert_int_eq(calc_func("1..2+3", &result), 0);

  ck_assert_int_eq(calc_func(".2+3", &result), 0);

  ck_assert_int_eq(calc_func("123.+4", &result), 0);

  ck_assert_int_eq(calc_func("1+2.", &result), 0);

  ck_assert_int_eq(calc_func("0-1.2.3+4+5", &result), 0);
}

Suite *check_calculation() {
  Suite *s = suite_create("check_calculation");
  TCase *tc = tcase_create("calculation test cases for calc.a");

  tcase_add_test(tc, check_calculation_1);
  tcase_add_test(tc, check_calculation_2);
  tcase_add_test(tc, check_calculation_3);
  suite_add_tcase(s, tc);

  return s;
}