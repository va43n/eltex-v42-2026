#include "test.h"

START_TEST(check_remove_1) {
  int result;

  phone_book* head = phone_book_create();

  USER_1(NAME1, JOB_PLACE1, JOB_POSITION1, NUMBER1, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER1, &result);
  USER_1(NAME1, JOB_PLACE1, JOB_POSITION1, NUMBER1, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER1, &result);
  USER_2(NAME2, JOB_PLACE2, JOB_POSITION2, NUMBER3, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER2, &result);
  USER_2(NAME2, JOB_PLACE2, JOB_POSITION2, NUMBER3, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER2, &result);
  USER_2(NAME2, JOB_PLACE2, JOB_POSITION2, NUMBER3, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER2, &result);
  USER_3(NAME3, JOB_PLACE2, JOB_POSITION1, OTHER2, &result);
  USER_3(NAME3, JOB_PLACE2, JOB_POSITION1, OTHER2, &result);

  phone_book_remove_page(head, 1);
  phone_book_remove_page(head, 7);
  phone_book_remove_page(head, 3);
  phone_book_remove_page(head, 4);

  ck_assert_int_eq(head->next != head, 1);
  ck_assert_int_eq(head->next->next != head, 1);
  ck_assert_int_eq(head->next->next->next != head, 1);
  ck_assert_int_eq(head->next->next->next->next == head, 1);

  ck_assert_int_eq(head->next->index == 6, 1);
  ck_assert_int_eq(strcmp(head->next->full_name, NAME3), 0);
  ck_assert_int_eq(strcmp(head->next->job_place, JOB_PLACE2), 0);
  ck_assert_int_eq(strcmp(head->next->job_position, JOB_POSITION1), 0);

  ck_assert_int_eq(head->next->numbers_n == 0, 1);

  ck_assert_int_eq(head->next->socials_n == 0, 1);

  ck_assert_int_eq(strcmp(head->next->other, OTHER2), 0);

  ck_assert_int_eq(head->next->next->index == 5, 1);
  ck_assert_int_eq(strcmp(head->next->next->full_name, NAME2), 0);
  ck_assert_int_eq(strcmp(head->next->next->job_place, JOB_PLACE2), 0);
  ck_assert_int_eq(strcmp(head->next->next->job_position, JOB_POSITION2), 0);

  ck_assert_int_eq(head->next->next->numbers_n == 1, 1);
  ck_assert_int_eq(strcmp(head->next->next->numbers[0], NUMBER3), 0);

  ck_assert_int_eq(head->next->next->socials_n == 1, 1);
  ck_assert_int_eq(
      strcmp(head->next->next->socials[0].social_name, SOCIALS_NAME1), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->socials[0].social_url, SOCIALS_URL1), 0);

  ck_assert_int_eq(strcmp(head->next->next->other, OTHER2), 0);

  ck_assert_int_eq(head->next->next->next->index == 2, 1);
  ck_assert_int_eq(strcmp(head->next->next->next->full_name, NAME1), 0);
  ck_assert_int_eq(strcmp(head->next->next->next->job_place, JOB_PLACE1), 0);
  ck_assert_int_eq(strcmp(head->next->next->next->job_position, JOB_POSITION1),
                   0);

  ck_assert_int_eq(head->next->next->next->numbers_n == 2, 1);
  ck_assert_int_eq(strcmp(head->next->next->next->numbers[0], NUMBER1), 0);
  ck_assert_int_eq(strcmp(head->next->next->next->numbers[1], NUMBER2), 0);

  ck_assert_int_eq(head->next->next->next->socials_n == 4, 1);
  ck_assert_int_eq(
      strcmp(head->next->next->next->socials[0].social_name, SOCIALS_NAME1), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->next->socials[0].social_url, SOCIALS_URL1), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->next->socials[1].social_name, SOCIALS_NAME2), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->next->socials[1].social_url, SOCIALS_URL2), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->next->socials[2].social_name, SOCIALS_NAME3), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->next->socials[2].social_url, SOCIALS_URL3), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->next->socials[3].social_name, SOCIALS_NAME4), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->next->socials[3].social_url, SOCIALS_URL4), 0);

  ck_assert_int_eq(phone_book_free(head), SUCCESS);
}

START_TEST(check_remove_2) {
  int result;

  phone_book* head = phone_book_create();

  USER_1(NAME1, JOB_PLACE1, JOB_POSITION1, NUMBER1, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER1, &result);
  USER_1(NAME1, JOB_PLACE1, JOB_POSITION1, NUMBER1, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER1, &result);
  USER_2(NAME2, JOB_PLACE2, JOB_POSITION2, NUMBER3, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER2, &result);
  USER_2(NAME2, JOB_PLACE2, JOB_POSITION2, NUMBER3, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER2, &result);
  USER_3(NAME3, JOB_PLACE2, JOB_POSITION1, OTHER2, &result);

  phone_book_print(head);

  phone_book_remove_page(head, 2);
  phone_book_remove_page(head, 3);

  USER_3("0123", JOB_PLACE2, JOB_POSITION1, OTHER4, &result);
  ck_assert_int_eq(result, SUCCESS);
  USER_3("Ivanych", JOB_PLACE2, JOB_POSITION1, OTHER5, &result);
  ck_assert_int_eq(result, SUCCESS);
  USER_3("Alex", JOB_PLACE2, JOB_POSITION1, OTHER6, &result);
  ck_assert_int_eq(result, SUCCESS);

  ck_assert_int_eq(strcmp(head->next->other, OTHER4), 0);
  ck_assert_int_eq(strcmp(head->next->next->next->other, OTHER6), 0);
  ck_assert_int_eq(strcmp(head->next->next->next->next->next->other, OTHER5),
                   0);

  ck_assert_int_eq(phone_book_free(head), SUCCESS);
}

START_TEST(check_remove_3) {
  int result;

  phone_book* head = phone_book_create();

  USER_1(NAME1, JOB_PLACE1, JOB_POSITION1, NUMBER1, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER1, &result);
  USER_1(NAME1, JOB_PLACE1, JOB_POSITION1, NUMBER1, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER1, &result);
  USER_2(NAME2, JOB_PLACE2, JOB_POSITION2, NUMBER3, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER2, &result);
  USER_2(NAME2, JOB_PLACE2, JOB_POSITION2, NUMBER3, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER2, &result);
  USER_3(NAME3, JOB_PLACE2, JOB_POSITION1, OTHER2, &result);

  phone_book_remove_page(head, 1);
  phone_book_remove_page(head, 2);
  phone_book_remove_page(head, 3);
  phone_book_remove_page(head, 4);
  phone_book_remove_page(head, 5);

  ck_assert_int_eq(head->next == head, 1);

  ck_assert_int_eq(phone_book_free(head), SUCCESS);
}

Suite* check_remove() {
  Suite* s = suite_create("check_remove");
  TCase* tc = tcase_create("remove test cases for phone_book.a");

  tcase_add_test(tc, check_remove_1);
  tcase_add_test(tc, check_remove_2);
  tcase_add_test(tc, check_remove_3);
  suite_add_tcase(s, tc);

  return s;
}