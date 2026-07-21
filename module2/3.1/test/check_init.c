#include "test.h"

START_TEST(check_init_1) {
  mod m1;

  ck_assert_int_eq(my_chmod_init(&m1, "755"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "rwxr-xr-x"), 0);
  ck_assert_int_eq(m1.numbers == 0b111101101, 1);

  ck_assert_int_eq(my_chmod_init(&m1, "123"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "--x-w--wx"), 0);
  ck_assert_int_eq(m1.numbers == 0b001010011, 1);

  ck_assert_int_eq(my_chmod_init(&m1, "000"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "---------"), 0);
  ck_assert_int_eq(m1.numbers == 0b000000000, 1);

  ck_assert_int_eq(my_chmod_init(&m1, "101"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "--x-----x"), 0);
  ck_assert_int_eq(m1.numbers == 0b001000001, 1);

  ck_assert_int_eq(my_chmod_init(&m1, "007"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "------rwx"), 0);
  ck_assert_int_eq(m1.numbers == 0b000000111, 1);

  ck_assert_int_eq(my_chmod_init(&m1, "888"), ERROR);
  ck_assert_int_eq(my_chmod_init(&m1, "128"), ERROR);
  ck_assert_int_eq(my_chmod_init(&m1, "1234"), ERROR);
  ck_assert_int_eq(my_chmod_init(&m1, "12"), ERROR);
  ck_assert_int_eq(my_chmod_init(&m1, ""), ERROR);
  ck_assert_int_eq(my_chmod_init(&m1, "9"), ERROR);
  ck_assert_int_eq(my_chmod_init(&m1, "759"), ERROR);
}

START_TEST(check_init_2) {
  mod m1;

  ck_assert_int_eq(my_chmod_init(&m1, "rwxr-xr-x"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "rwxr-xr-x"), 0);
  ck_assert_int_eq(m1.numbers == 0b111101101, 1);

  ck_assert_int_eq(my_chmod_init(&m1, "--x-w--wx"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "--x-w--wx"), 0);
  ck_assert_int_eq(m1.numbers == 0b001010011, 1);

  ck_assert_int_eq(my_chmod_init(&m1, "---------"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "---------"), 0);
  ck_assert_int_eq(m1.numbers == 0b000000000, 1);

  ck_assert_int_eq(my_chmod_init(&m1, "--x-----x"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "--x-----x"), 0);
  ck_assert_int_eq(m1.numbers == 0b001000001, 1);

  ck_assert_int_eq(my_chmod_init(&m1, "------rwx"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "------rwx"), 0);
  ck_assert_int_eq(m1.numbers == 0b000000111, 1);

  ck_assert_int_eq(my_chmod_init(&m1, "qwerty"), ERROR);
  ck_assert_int_eq(my_chmod_init(&m1, "rwx"), ERROR);
  ck_assert_int_eq(my_chmod_init(&m1, "rwxrwxrw"), ERROR);
  ck_assert_int_eq(my_chmod_init(&m1, "rwxrwxrwxrwx"), ERROR);
  ck_assert_int_eq(my_chmod_init(&m1, ""), ERROR);
  ck_assert_int_eq(my_chmod_init(&m1, "rwx-zxc--"), ERROR);
  ck_assert_int_eq(my_chmod_init(&m1, "www------"), ERROR);
  ck_assert_int_eq(my_chmod_init(&m1, "xwx------"), ERROR);
  ck_assert_int_eq(my_chmod_init(&m1, "---xwx---"), ERROR);
  ck_assert_int_eq(my_chmod_init(&m1, "------xwx"), ERROR);
  ck_assert_int_eq(my_chmod_init(&m1, "rrx------"), ERROR);
  ck_assert_int_eq(my_chmod_init(&m1, "---rrx---"), ERROR);
  ck_assert_int_eq(my_chmod_init(&m1, "------rrx"), ERROR);
  ck_assert_int_eq(my_chmod_init(&m1, "rww------"), ERROR);
  ck_assert_int_eq(my_chmod_init(&m1, "---rww---"), ERROR);
  ck_assert_int_eq(my_chmod_init(&m1, "------rww"), ERROR);
}

START_TEST(check_init_3) {
  mod m1;

  ck_assert_int_eq(my_chmod_init_with_file(&m1, "test1"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "rwxr-xr-x"), 0);
  ck_assert_int_eq(m1.numbers == 0b111101101, 1);

  ck_assert_int_eq(my_chmod_init_with_file(&m1, "test2"), SUCCESS);
  my_chmod_print(m1);
  ck_assert_int_eq(strcmp(m1.letters, "--x-w--wx"), 0);
  ck_assert_int_eq(m1.numbers == 0b001010011, 1);

  ck_assert_int_eq(my_chmod_init_with_file(&m1, "test3"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "---------"), 0);
  ck_assert_int_eq(m1.numbers == 0b000000000, 1);

  ck_assert_int_eq(my_chmod_init_with_file(&m1, "test4"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "--x-----x"), 0);
  ck_assert_int_eq(m1.numbers == 0b001000001, 1);

  ck_assert_int_eq(my_chmod_init_with_file(&m1, "test5"), SUCCESS);
  ck_assert_int_eq(strcmp(m1.letters, "------rwx"), 0);
  ck_assert_int_eq(m1.numbers == 0b000000111, 1);

  ck_assert_int_eq(my_chmod_init_with_file(&m1, "qwerty"), ERROR);
  ck_assert_int_eq(my_chmod_init_with_file(&m1, "some_file"), ERROR);
}

Suite *check_init() {
  Suite *s = suite_create("check_init");
  TCase *tc = tcase_create("init test cases for calc.a");

  tcase_add_test(tc, check_init_1);
  tcase_add_test(tc, check_init_2);
  tcase_add_test(tc, check_init_3);
  suite_add_tcase(s, tc);

  return s;
}