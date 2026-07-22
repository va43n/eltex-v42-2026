#include "test.h"

START_TEST(check_save_load_1) {
  int result;

  phone_book* head = phone_book_create();

  USER_1(NAME1, JOB_PLACE1, JOB_POSITION1, NUMBER1, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER1, &result);
  USER_1(NAME2, JOB_PLACE1, JOB_POSITION1, NUMBER3, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER2, &result);
  USER_3(NAME4, JOB_PLACE1, JOB_POSITION1, OTHER3, &result);
  USER_2(NAME4, JOB_PLACE2, JOB_POSITION2, NUMBER3, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER4, &result);
  USER_2(NAME2, JOB_PLACE2, JOB_POSITION2, NUMBER5, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER5, &result);
  USER_2(NAME3, JOB_PLACE2, JOB_POSITION2, NUMBER4, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER6, &result);

  ck_assert_int_eq(phone_book_save(head, "save_phone_book.o"), SUCCESS);
  phone_book* head2 = phone_book_load("save_phone_book.o");

  phone_book_print(head);
  phone_book_print(head2);

  ck_assert_int_eq(phone_book_compare(head, head2), SUCCESS);

  ck_assert_int_eq(phone_book_free(head), SUCCESS);
  ck_assert_int_eq(phone_book_free(head2), SUCCESS);
}

START_TEST(check_save_load_2) {
  phone_book* head = phone_book_create();

  ck_assert_int_eq(phone_book_save(head, "save_phone_book2.o"), SUCCESS);
  phone_book* head2 = phone_book_load("save_phone_book2.o");

  ck_assert_int_eq(phone_book_compare(head, head2), SUCCESS);

  ck_assert_int_eq(phone_book_free(head), SUCCESS);
  ck_assert_int_eq(phone_book_free(head2), SUCCESS);
}

START_TEST(check_save_load_3) {
  int result;
  phone_book* head = phone_book_create();

  USER_2(NAME3, JOB_PLACE2, JOB_POSITION2, NUMBER4, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER6, &result);

  ck_assert_int_eq(phone_book_save(head, "save_phone_book2.o"), SUCCESS);
  phone_book* head2 = phone_book_load("save_phone_book2.o");

  ck_assert_int_eq(phone_book_compare(head, head2), SUCCESS);

  ck_assert_int_eq(phone_book_free(head), SUCCESS);
  ck_assert_int_eq(phone_book_free(head2), SUCCESS);
}

Suite* check_save_load() {
  Suite* s = suite_create("check_save_load");
  TCase* tc = tcase_create("save_load test cases for phone_book.a");

  tcase_add_test(tc, check_save_load_1);
  tcase_add_test(tc, check_save_load_2);
  tcase_add_test(tc, check_save_load_3);
  suite_add_tcase(s, tc);

  return s;
}