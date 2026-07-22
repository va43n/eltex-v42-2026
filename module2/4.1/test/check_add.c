#include "test.h"

START_TEST(check_add_1) {
  int result;

  phone_book *head = phone_book_create();
  ck_assert_int_eq(head->next == head, 1);

  USER_1(NAME1, JOB_PLACE1, JOB_POSITION1, NUMBER1, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER1, &result);
  ck_assert_int_eq(result, SUCCESS);

  ck_assert_int_eq(head->next != head, 1);
  ck_assert_int_eq(head->next->next == head, 1);
  ck_assert_int_eq(head->next->index == 1, 1);
  ck_assert_int_eq(strcmp(head->next->full_name, NAME1), 0);
  ck_assert_int_eq(strcmp(head->next->job_place, JOB_PLACE1), 0);
  ck_assert_int_eq(strcmp(head->next->job_position, JOB_POSITION1), 0);

  ck_assert_int_eq(head->next->numbers_n == 2, 1);
  ck_assert_int_eq(strcmp(head->next->numbers[0], NUMBER1), 0);
  ck_assert_int_eq(strcmp(head->next->numbers[1], NUMBER2), 0);

  ck_assert_int_eq(head->next->socials_n == 4, 1);
  ck_assert_int_eq(strcmp(head->next->socials[0].social_name, SOCIALS_NAME1),
                   0);
  ck_assert_int_eq(strcmp(head->next->socials[0].social_url, SOCIALS_URL1), 0);
  ck_assert_int_eq(strcmp(head->next->socials[1].social_name, SOCIALS_NAME2),
                   0);
  ck_assert_int_eq(strcmp(head->next->socials[1].social_url, SOCIALS_URL2), 0);
  ck_assert_int_eq(strcmp(head->next->socials[2].social_name, SOCIALS_NAME3),
                   0);
  ck_assert_int_eq(strcmp(head->next->socials[2].social_url, SOCIALS_URL3), 0);
  ck_assert_int_eq(strcmp(head->next->socials[3].social_name, SOCIALS_NAME4),
                   0);
  ck_assert_int_eq(strcmp(head->next->socials[3].social_url, SOCIALS_URL4), 0);

  ck_assert_int_eq(strcmp(head->next->other, OTHER1), 0);

  USER_2(NAME2, JOB_PLACE2, JOB_POSITION2, NUMBER3, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER2, &result);
  ck_assert_int_eq(result, SUCCESS);

  ck_assert_int_eq(head->next != head, 1);
  ck_assert_int_eq(head->next->next != head, 1);
  ck_assert_int_eq(head->next->next->next == head, 1);

  ck_assert_int_eq(head->next->index == 2, 1);
  ck_assert_int_eq(strcmp(head->next->full_name, NAME2), 0);
  ck_assert_int_eq(strcmp(head->next->job_place, JOB_PLACE2), 0);
  ck_assert_int_eq(strcmp(head->next->job_position, JOB_POSITION2), 0);

  ck_assert_int_eq(head->next->numbers_n == 1, 1);
  ck_assert_int_eq(strcmp(head->next->numbers[0], NUMBER3), 0);

  ck_assert_int_eq(head->next->socials_n == 1, 1);
  ck_assert_int_eq(strcmp(head->next->socials[0].social_name, SOCIALS_NAME1),
                   0);
  ck_assert_int_eq(strcmp(head->next->socials[0].social_url, SOCIALS_URL1), 0);

  ck_assert_int_eq(strcmp(head->next->other, OTHER2), 0);

  ck_assert_int_eq(head->next->next->index == 1, 1);
  ck_assert_int_eq(strcmp(head->next->next->full_name, NAME1), 0);
  ck_assert_int_eq(strcmp(head->next->next->job_place, JOB_PLACE1), 0);
  ck_assert_int_eq(strcmp(head->next->next->job_position, JOB_POSITION1), 0);

  ck_assert_int_eq(head->next->next->numbers_n == 2, 1);
  ck_assert_int_eq(strcmp(head->next->next->numbers[0], NUMBER1), 0);
  ck_assert_int_eq(strcmp(head->next->next->numbers[1], NUMBER2), 0);

  ck_assert_int_eq(head->next->next->socials_n == 4, 1);
  ck_assert_int_eq(
      strcmp(head->next->next->socials[0].social_name, SOCIALS_NAME1), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->socials[0].social_url, SOCIALS_URL1), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->socials[1].social_name, SOCIALS_NAME2), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->socials[1].social_url, SOCIALS_URL2), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->socials[2].social_name, SOCIALS_NAME3), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->socials[2].social_url, SOCIALS_URL3), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->socials[3].social_name, SOCIALS_NAME4), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->socials[3].social_url, SOCIALS_URL4), 0);

  USER_3(NAME3, JOB_PLACE2, JOB_POSITION1, OTHER2, &result);
  ck_assert_int_eq(result, SUCCESS);

  ck_assert_int_eq(head->next != head, 1);
  ck_assert_int_eq(head->next->next != head, 1);
  ck_assert_int_eq(head->next->next->next != head, 1);
  ck_assert_int_eq(head->next->next->next->next == head, 1);

  ck_assert_int_eq(head->next->index == 3, 1);
  ck_assert_int_eq(strcmp(head->next->full_name, NAME3), 0);
  ck_assert_int_eq(strcmp(head->next->job_place, JOB_PLACE2), 0);
  ck_assert_int_eq(strcmp(head->next->job_position, JOB_POSITION1), 0);

  ck_assert_int_eq(head->next->numbers_n == 0, 1);

  ck_assert_int_eq(head->next->socials_n == 0, 1);

  ck_assert_int_eq(strcmp(head->next->other, OTHER2), 0);

  ck_assert_int_eq(head->next->next->index == 2, 1);
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

  ck_assert_int_eq(head->next->next->next->index == 1, 1);
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

START_TEST(check_add_2) {
  int result;

  phone_book *head = phone_book_create();

  USER_1(NAME1, JOB_PLACE1, JOB_POSITION1, NUMBER1, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER1, &result);
  USER_2(NAME2, JOB_PLACE2, JOB_POSITION2, NUMBER3, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER2, &result);

  USER_3(WRONG_NAME1, JOB_PLACE1, JOB_POSITION1, OTHER1, &result);
  ck_assert_int_eq(result, ERROR);

  USER_2(NAME2, JOB_PLACE3, JOB_POSITION3, WRONG_NUMBER1, SOCIALS_NAME1,
         SOCIALS_URL1, OTHER3, &result);
  ck_assert_int_eq(result, ERROR);

  USER_2(NAME2, JOB_PLACE3, JOB_POSITION3, WRONG_NUMBER2, SOCIALS_NAME1,
         SOCIALS_URL1, OTHER3, &result);
  ck_assert_int_eq(result, ERROR);

  USER_2(NAME2, JOB_PLACE3, JOB_POSITION3, WRONG_NUMBER3, SOCIALS_NAME1,
         SOCIALS_URL1, OTHER3, &result);
  ck_assert_int_eq(result, ERROR);

  ck_assert_int_eq(head->next != head, 1);
  ck_assert_int_eq(head->next->next != head, 1);
  ck_assert_int_eq(head->next->next->next == head, 1);

  ck_assert_int_eq(head->next->index == 2, 1);
  ck_assert_int_eq(strcmp(head->next->full_name, NAME2), 0);
  ck_assert_int_eq(strcmp(head->next->job_place, JOB_PLACE2), 0);
  ck_assert_int_eq(strcmp(head->next->job_position, JOB_POSITION2), 0);

  ck_assert_int_eq(head->next->numbers_n == 1, 1);
  ck_assert_int_eq(strcmp(head->next->numbers[0], NUMBER3), 0);

  ck_assert_int_eq(head->next->socials_n == 1, 1);
  ck_assert_int_eq(strcmp(head->next->socials[0].social_name, SOCIALS_NAME1),
                   0);
  ck_assert_int_eq(strcmp(head->next->socials[0].social_url, SOCIALS_URL1), 0);

  ck_assert_int_eq(strcmp(head->next->other, OTHER2), 0);

  ck_assert_int_eq(head->next->next->index == 1, 1);
  ck_assert_int_eq(strcmp(head->next->next->full_name, NAME1), 0);
  ck_assert_int_eq(strcmp(head->next->next->job_place, JOB_PLACE1), 0);
  ck_assert_int_eq(strcmp(head->next->next->job_position, JOB_POSITION1), 0);

  ck_assert_int_eq(head->next->next->numbers_n == 2, 1);
  ck_assert_int_eq(strcmp(head->next->next->numbers[0], NUMBER1), 0);
  ck_assert_int_eq(strcmp(head->next->next->numbers[1], NUMBER2), 0);

  ck_assert_int_eq(head->next->next->socials_n == 4, 1);
  ck_assert_int_eq(
      strcmp(head->next->next->socials[0].social_name, SOCIALS_NAME1), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->socials[0].social_url, SOCIALS_URL1), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->socials[1].social_name, SOCIALS_NAME2), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->socials[1].social_url, SOCIALS_URL2), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->socials[2].social_name, SOCIALS_NAME3), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->socials[2].social_url, SOCIALS_URL3), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->socials[3].social_name, SOCIALS_NAME4), 0);
  ck_assert_int_eq(
      strcmp(head->next->next->socials[3].social_url, SOCIALS_URL4), 0);

  ck_assert_int_eq(phone_book_free(head), SUCCESS);
}

Suite *check_add() {
  Suite *s = suite_create("check_add");
  TCase *tc = tcase_create("add test cases for phone_book.a");

  tcase_add_test(tc, check_add_1);
  tcase_add_test(tc, check_add_2);
  suite_add_tcase(s, tc);

  return s;
}