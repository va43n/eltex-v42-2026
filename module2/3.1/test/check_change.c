#include "test.h"

START_TEST(check_change_1) {
  mod m1;

  ck_assert_int_eq(my_chmod_init(&m1, "755"), SUCCESS);

  ck_assert_int_eq(my_chmod_change(&m1, "755"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "rwxr-xr-x"), 0);
  ck_assert_int_eq(m1.numbers == 0b111101101, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "123"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "--x-w--wx"), 0);
  ck_assert_int_eq(m1.numbers == 0b001010011, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "000"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "---------"), 0);
  ck_assert_int_eq(m1.numbers == 0b000000000, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "101"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "--x-----x"), 0);
  ck_assert_int_eq(m1.numbers == 0b001000001, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "007"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "------rwx"), 0);
  ck_assert_int_eq(m1.numbers == 0b000000111, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "888"), ERROR);
  ck_assert_int_eq(my_chmod_change(&m1, "128"), ERROR);
  ck_assert_int_eq(my_chmod_change(&m1, "1234"), ERROR);
  ck_assert_int_eq(my_chmod_change(&m1, "12"), ERROR);
  ck_assert_int_eq(my_chmod_change(&m1, ""), ERROR);
  ck_assert_int_eq(my_chmod_change(&m1, "9"), ERROR);
  ck_assert_int_eq(my_chmod_change(&m1, "759"), ERROR);
}

START_TEST(check_change_2) {
  mod m1;

  ck_assert_int_eq(my_chmod_init(&m1, "755"), SUCCESS);

  ck_assert_int_eq(my_chmod_change(&m1, "-rwx"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "---------"), 0);
  ck_assert_int_eq(m1.numbers == 0b000000000, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "+x"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "--x--x--x"), 0);
  ck_assert_int_eq(m1.numbers == 0b001001001, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "-rwx"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "---------"), 0);
  ck_assert_int_eq(m1.numbers == 0b000000000, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "+rx"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "r-xr-xr-x"), 0);
  ck_assert_int_eq(m1.numbers == 0b101101101, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "-rwx"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "---------"), 0);
  ck_assert_int_eq(m1.numbers == 0b000000000, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "+rwx"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "rwxrwxrwx"), 0);
  ck_assert_int_eq(m1.numbers == 0b111111111, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "-rwx"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "---------"), 0);
  ck_assert_int_eq(m1.numbers == 0b000000000, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "=r"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "r--r--r--"), 0);
  ck_assert_int_eq(m1.numbers == 0b100100100, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "=rr"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "r--r--r--"), 0);
  ck_assert_int_eq(m1.numbers == 0b100100100, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "=rx"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "r-xr-xr-x"), 0);
  ck_assert_int_eq(m1.numbers == 0b101101101, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "u-rx"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "---r-xr-x"), 0);
  ck_assert_int_eq(m1.numbers == 0b000101101, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "go-rx"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "---------"), 0);
  ck_assert_int_eq(m1.numbers == 0b000000000, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "a+rwx"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "rwxrwxrwx"), 0);
  ck_assert_int_eq(m1.numbers == 0b111111111, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "a=rwx"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "rwxrwxrwx"), 0);
  ck_assert_int_eq(m1.numbers == 0b111111111, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "a=w"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "-w--w--w-"), 0);
  ck_assert_int_eq(m1.numbers == 0b010010010, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "ugo=rx"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "r-xr-xr-x"), 0);
  ck_assert_int_eq(m1.numbers == 0b101101101, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "ugo-x"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "r--r--r--"), 0);
  ck_assert_int_eq(m1.numbers == 0b100100100, 1);

  ck_assert_int_eq(my_chmod_change(&m1, "qwerty"), ERROR);
  ck_assert_int_eq(my_chmod_change(&m1, "rwx"), ERROR);
  ck_assert_int_eq(my_chmod_change(&m1, "rwx+ugo"), ERROR);
  ck_assert_int_eq(my_chmod_change(&m1, "ugo"), ERROR);
  ck_assert_int_eq(my_chmod_change(&m1, "a"), ERROR);
  ck_assert_int_eq(my_chmod_change(&m1, "u"), ERROR);
  ck_assert_int_eq(my_chmod_change(&m1, ""), ERROR);
  ck_assert_int_eq(my_chmod_change(&m1, "u++rw"), ERROR);
  ck_assert_int_eq(my_chmod_change(&m1, "u==rw"), ERROR);
  ck_assert_int_eq(my_chmod_change(&m1, "u--rw"), ERROR);
  ck_assert_int_eq(my_chmod_change(&m1, "u+go"), ERROR);
  ck_assert_int_eq(my_chmod_change(&m1, "u=g"), ERROR);
  ck_assert_int_eq(my_chmod_change(&m1, "r=wx"), ERROR);
}

Suite *check_change() {
  Suite *s = suite_create("check_change");
  TCase *tc = tcase_create("change test cases for my_chmod.a");

  tcase_add_test(tc, check_change_1);
  tcase_add_test(tc, check_change_2);
  suite_add_tcase(s, tc);

  return s;
}