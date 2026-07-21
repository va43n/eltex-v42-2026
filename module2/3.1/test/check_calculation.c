#include "test.h"

START_TEST(check_calculation_1) {
  double result;

  ck_assert_int_eq(calculate("0", &result), SUCCESS);
  ck_assert_double_eq_tol(result, 0.0, EPS);

  ck_assert_int_eq(calculate("1", &result), SUCCESS);
  ck_assert_double_eq_tol(result, 1.0, EPS);

  ck_assert_int_eq(calculate("2.345", &result), SUCCESS);
  ck_assert_double_eq_tol(result, 2.345, EPS);

  ck_assert_int_eq(calculate("0+0", &result), SUCCESS);
  ck_assert_double_eq_tol(result, 0.0, EPS);

  ck_assert_int_eq(calculate("0*0", &result), SUCCESS);
  ck_assert_double_eq_tol(result, 0.0, EPS);

  ck_assert_int_eq(calculate("0-0", &result), SUCCESS);
  ck_assert_double_eq_tol(result, 0.0, EPS);

  ck_assert_int_eq(calculate("1+0", &result), SUCCESS);
  ck_assert_double_eq_tol(result, 1.0, EPS);

  ck_assert_int_eq(calculate("0+1", &result), SUCCESS);
  ck_assert_double_eq_tol(result, 1.0, EPS);

  ck_assert_int_eq(calculate("1-0", &result), SUCCESS);
  ck_assert_double_eq_tol(result, 1.0, EPS);

  ck_assert_int_eq(calculate("0+1", &result), SUCCESS);
  ck_assert_double_eq_tol(result, 1.0, EPS);

  ck_assert_int_eq(calculate("1*0", &result), SUCCESS);
  ck_assert_double_eq_tol(result, 0.0, EPS);

  ck_assert_int_eq(calculate("0*1", &result), SUCCESS);
  ck_assert_double_eq_tol(result, 0.0, EPS);

  ck_assert_int_eq(calculate("1+0.1", &result), SUCCESS);
  ck_assert_double_eq_tol(result, 1.1, EPS);

  ck_assert_int_eq(calculate("1*0.1", &result), SUCCESS);
  ck_assert_double_eq_tol(result, 0.1, EPS);

  ck_assert_int_eq(calculate("1+2+3+4", &result), SUCCESS);
  ck_assert_double_eq_tol(result, 10.0, EPS);

  ck_assert_int_eq(calculate("1-2+3-4", &result), SUCCESS);
  ck_assert_double_eq_tol(result, -2.0, EPS);

  ck_assert_int_eq(calculate("2+2*2", &result), SUCCESS);
  ck_assert_double_eq_tol(result, 6.0, EPS);

  ck_assert_int_eq(calculate("2+2*2-2/2", &result), SUCCESS);
  ck_assert_double_eq_tol(result, 5.0, EPS);

  ck_assert_int_eq(calculate("1.2*2.3+3.4*4.5", &result), SUCCESS);
  ck_assert_double_eq_tol(result, 18.06, EPS);

  ck_assert_int_eq(calculate("1+2*3-4/5+6/7-8.9+0.1", &result), SUCCESS);
  ck_assert_double_eq_tol(result, -1.7428571429, EPS);
}

START_TEST(check_calculation_2) {
  double result;

  ck_assert_int_eq(calculate("0/1", &result), SUCCESS);
  ck_assert_double_eq_tol(result, 0.0, EPS);

  ck_assert_int_eq(calculate("1/0", &result), CALC_ERROR);

  ck_assert_int_eq(calculate("1+2/0", &result), CALC_ERROR);

  ck_assert_int_eq(calculate("0/0", &result), CALC_ERROR);

  ck_assert_int_eq(calculate("1.234+5.6789/0", &result), CALC_ERROR);

  ck_assert_int_eq(calculate("1/0*2+3+4+5-17", &result), CALC_ERROR);

  ck_assert_int_eq(calculate("1*2+3+4+5/0-17", &result), CALC_ERROR);
}

START_TEST(check_calculation_3) {
  double result;

  ck_assert_int_eq(calculate("+", &result), ERROR);

  ck_assert_int_eq(calculate("+1", &result), ERROR);

  ck_assert_int_eq(calculate("+1+2", &result), ERROR);

  ck_assert_int_eq(calculate("2+", &result), ERROR);

  ck_assert_int_eq(calculate("1+2+", &result), ERROR);

  ck_assert_int_eq(calculate("1++2", &result), ERROR);

  ck_assert_int_eq(calculate("1++2-3", &result), ERROR);

  ck_assert_int_eq(calculate("3-1++2", &result), ERROR);

  ck_assert_int_eq(calculate("1 - 2", &result), ERROR);

  ck_assert_int_eq(calculate("5%2", &result), ERROR);

  ck_assert_int_eq(calculate("1+2+3%4-5-6-7", &result), ERROR);

  ck_assert_int_eq(calculate("1++", &result), ERROR);

  ck_assert_int_eq(calculate("1//2++3**4", &result), ERROR);

  ck_assert_int_eq(calculate("1..2+3", &result), ERROR);

  ck_assert_int_eq(calculate(".2+3", &result), ERROR);

  ck_assert_int_eq(calculate("123.+4", &result), ERROR);

  ck_assert_int_eq(calculate("1+2.", &result), ERROR);

  ck_assert_int_eq(calculate("0-1.2.3+4+5", &result), ERROR);
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