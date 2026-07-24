#include "test.h"

START_TEST(check_add_1) {
  int result;

  phone_book *head = phone_book_create();
  ck_assert_int_eq(head->left == NULL, 1);
  ck_assert_int_eq(head->right == NULL, 1);

  USER_1(NAME4, JOB_PLACE1, JOB_POSITION1, NUMBER1, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER1, &result);
  ck_assert_int_eq(result, SUCCESS);

  ck_assert_int_eq(head->left != NULL, 1);
  ck_assert_int_eq(head->left->left == NULL, 1);
  ck_assert_int_eq(head->left->right == NULL, 1);
  ck_assert_int_eq(head->left->index == 1, 1);
  ck_assert_int_eq(strcmp(head->left->full_name, NAME4), 0);
  ck_assert_int_eq(strcmp(head->left->job_place, JOB_PLACE1), 0);
  ck_assert_int_eq(strcmp(head->left->job_position, JOB_POSITION1), 0);

  ck_assert_int_eq(head->left->numbers_n == 2, 1);
  ck_assert_int_eq(strcmp(head->left->numbers[0], NUMBER1), 0);
  ck_assert_int_eq(strcmp(head->left->numbers[1], NUMBER2), 0);

  ck_assert_int_eq(head->left->socials_n == 4, 1);
  ck_assert_int_eq(strcmp(head->left->socials[0].social_name, SOCIALS_NAME1),
                   0);
  ck_assert_int_eq(strcmp(head->left->socials[0].social_url, SOCIALS_URL1), 0);
  ck_assert_int_eq(strcmp(head->left->socials[1].social_name, SOCIALS_NAME2),
                   0);
  ck_assert_int_eq(strcmp(head->left->socials[1].social_url, SOCIALS_URL2), 0);
  ck_assert_int_eq(strcmp(head->left->socials[2].social_name, SOCIALS_NAME3),
                   0);
  ck_assert_int_eq(strcmp(head->left->socials[2].social_url, SOCIALS_URL3), 0);
  ck_assert_int_eq(strcmp(head->left->socials[3].social_name, SOCIALS_NAME4),
                   0);
  ck_assert_int_eq(strcmp(head->left->socials[3].social_url, SOCIALS_URL4), 0);

  ck_assert_int_eq(strcmp(head->left->other, OTHER1), 0);

  USER_2(NAME2, JOB_PLACE2, JOB_POSITION2, NUMBER3, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER2, &result);
  ck_assert_int_eq(result, SUCCESS);

  ck_assert_int_eq(head->left->left != NULL, 1);
  ck_assert_int_eq(head->left->left->left == NULL, 1);
  ck_assert_int_eq(head->left->left->right == NULL, 1);

  ck_assert_int_eq(head->left->left->index == 2, 1);
  ck_assert_int_eq(strcmp(head->left->left->full_name, NAME2), 0);
  ck_assert_int_eq(strcmp(head->left->left->job_place, JOB_PLACE2), 0);
  ck_assert_int_eq(strcmp(head->left->left->job_position, JOB_POSITION2), 0);

  ck_assert_int_eq(head->left->left->numbers_n == 1, 1);
  ck_assert_int_eq(strcmp(head->left->left->numbers[0], NUMBER3), 0);

  ck_assert_int_eq(head->left->left->socials_n == 1, 1);
  ck_assert_int_eq(
      strcmp(head->left->left->socials[0].social_name, SOCIALS_NAME1), 0);
  ck_assert_int_eq(
      strcmp(head->left->left->socials[0].social_url, SOCIALS_URL1), 0);

  ck_assert_int_eq(strcmp(head->left->left->other, OTHER2), 0);

  USER_3(NAME1, JOB_PLACE2, JOB_POSITION1, OTHER2, &result);
  ck_assert_int_eq(result, SUCCESS);

  ck_assert_int_eq(head->left->right != NULL, 1);
  ck_assert_int_eq(head->left->right->left == NULL, 1);
  ck_assert_int_eq(head->left->right->right == NULL, 1);

  ck_assert_int_eq(head->left->right->index == 3, 1);
  ck_assert_int_eq(strcmp(head->left->right->full_name, NAME1), 0);
  ck_assert_int_eq(strcmp(head->left->right->job_place, JOB_PLACE2), 0);
  ck_assert_int_eq(strcmp(head->left->right->job_position, JOB_POSITION1), 0);

  ck_assert_int_eq(head->left->right->numbers_n == 0, 1);

  ck_assert_int_eq(head->left->right->socials_n == 0, 1);

  ck_assert_int_eq(strcmp(head->left->right->other, OTHER2), 0);

  ck_assert_int_eq(phone_book_free(head), SUCCESS);
}

START_TEST(check_add_2) {
  int result;

  phone_book *head = phone_book_create();

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