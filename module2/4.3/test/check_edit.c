#include "test.h"

START_TEST(check_edit_1) {
  int result;

  phone_book* head = phone_book_create();

  USER_2(NAME1, JOB_PLACE1, JOB_POSITION1, NUMBER1, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER1, &result);

  ck_assert_int_eq(phone_book_edit(head, 1, EDIT_FULL_NAME, NAME2), SUCCESS);
  ck_assert_int_eq(phone_book_edit(head, 1, EDIT_JOB_PLACE, JOB_PLACE2),
                   SUCCESS);
  ck_assert_int_eq(phone_book_edit(head, 1, EDIT_JOB_POSITION, JOB_POSITION2),
                   SUCCESS);

  ck_assert_int_eq(
      phone_book_edit(head, 1, EDIT_NUMBERS | EDIT_NUMBERS_ADD, NUMBER3),
      SUCCESS);
  ck_assert_int_eq(
      phone_book_edit(head, 1, EDIT_NUMBERS | EDIT_NUMBERS_ADD, NUMBER4),
      SUCCESS);

  socials_t social1, social2;
  strcpy(social1.social_name, SOCIALS_NAME2);
  strcpy(social1.social_url, SOCIALS_URL2);
  strcpy(social2.social_name, SOCIALS_NAME3);
  strcpy(social2.social_url, SOCIALS_URL3);
  ck_assert_int_eq(
      phone_book_edit(head, 1, EDIT_SOCIALS | EDIT_SOCIALS_ADD, social1),
      SUCCESS);
  ck_assert_int_eq(
      phone_book_edit(head, 1, EDIT_SOCIALS | EDIT_SOCIALS_ADD, social2),
      SUCCESS);

  ck_assert_int_eq(phone_book_edit(head, 1, EDIT_OTHER, OTHER2), SUCCESS);

  ck_assert_int_eq(strcmp(head->left->full_name, NAME2), 0);
  ck_assert_int_eq(strcmp(head->left->job_place, JOB_PLACE2), 0);
  ck_assert_int_eq(strcmp(head->left->job_position, JOB_POSITION2), 0);

  ck_assert_int_eq(head->left->numbers_n == 3, 1);
  ck_assert_int_eq(strcmp(head->left->numbers[0], NUMBER1), 0);
  ck_assert_int_eq(strcmp(head->left->numbers[1], NUMBER3), 0);
  ck_assert_int_eq(strcmp(head->left->numbers[2], NUMBER4), 0);

  ck_assert_int_eq(head->left->socials_n == 3, 1);
  ck_assert_int_eq(strcmp(head->left->socials[0].social_name, SOCIALS_NAME1),
                   0);
  ck_assert_int_eq(strcmp(head->left->socials[0].social_url, SOCIALS_URL1), 0);
  ck_assert_int_eq(strcmp(head->left->socials[1].social_name, SOCIALS_NAME2),
                   0);
  ck_assert_int_eq(strcmp(head->left->socials[1].social_url, SOCIALS_URL2), 0);
  ck_assert_int_eq(strcmp(head->left->socials[2].social_name, SOCIALS_NAME3),
                   0);
  ck_assert_int_eq(strcmp(head->left->socials[2].social_url, SOCIALS_URL3), 0);

  ck_assert_int_eq(strcmp(head->left->other, OTHER2), 0);

  ck_assert_int_eq(phone_book_edit(head, 1, EDIT_NUMBERS, 10), ERROR);

  ck_assert_int_eq(phone_book_edit(head, 1, EDIT_NUMBERS, 0), SUCCESS);
  ck_assert_int_eq(head->left->numbers_n == 2, 1);
  ck_assert_int_eq(strcmp(head->left->numbers[0], NUMBER3), 0);
  ck_assert_int_eq(strcmp(head->left->numbers[1], NUMBER4), 0);

  ck_assert_int_eq(phone_book_edit(head, 1, EDIT_NUMBERS, 0), SUCCESS);
  ck_assert_int_eq(head->left->numbers_n == 1, 1);
  ck_assert_int_eq(strcmp(head->left->numbers[0], NUMBER4), 0);

  ck_assert_int_eq(phone_book_edit(head, 1, EDIT_NUMBERS, 0), SUCCESS);
  ck_assert_int_eq(head->left->numbers_n == 0, 1);

  ck_assert_int_eq(phone_book_edit(head, 1, EDIT_SOCIALS, 10), ERROR);

  ck_assert_int_eq(phone_book_edit(head, 1, EDIT_SOCIALS, 0), SUCCESS);
  ck_assert_int_eq(head->left->socials_n == 2, 1);
  ck_assert_int_eq(strcmp(head->left->socials[0].social_name, SOCIALS_NAME2),
                   0);
  ck_assert_int_eq(strcmp(head->left->socials[0].social_url, SOCIALS_URL2), 0);
  ck_assert_int_eq(strcmp(head->left->socials[1].social_name, SOCIALS_NAME3),
                   0);
  ck_assert_int_eq(strcmp(head->left->socials[1].social_url, SOCIALS_URL3), 0);

  ck_assert_int_eq(phone_book_edit(head, 1, EDIT_SOCIALS, 0), SUCCESS);
  ck_assert_int_eq(head->left->socials_n == 1, 1);
  ck_assert_int_eq(strcmp(head->left->socials[0].social_name, SOCIALS_NAME3),
                   0);
  ck_assert_int_eq(strcmp(head->left->socials[0].social_url, SOCIALS_URL3), 0);

  ck_assert_int_eq(phone_book_edit(head, 1, EDIT_SOCIALS, 0), SUCCESS);
  ck_assert_int_eq(head->left->socials_n == 0, 1);

  ck_assert_int_eq(phone_book_free(head), SUCCESS);
}

START_TEST(check_edit_2) {
  int result;

  phone_book* head = phone_book_create();

  USER_2(NAME1, JOB_PLACE1, JOB_POSITION1, NUMBER1, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER1, &result);

  socials_t social1, social2;
  strcpy(social1.social_name, SOCIALS_NAME2);
  strcpy(social1.social_url, SOCIALS_URL2);
  strcpy(social2.social_name, SOCIALS_NAME3);
  strcpy(social2.social_url, SOCIALS_URL3);

  ck_assert_int_eq(
      phone_book_edit(
          head, 1,
          EDIT_FULL_NAME | EDIT_JOB_PLACE | EDIT_JOB_POSITION | EDIT_NUMBERS |
              EDIT_NUMBERS_ADD | EDIT_SOCIALS | EDIT_SOCIALS_ADD | EDIT_OTHER,
          NAME2, JOB_PLACE2, JOB_POSITION2, NUMBER3, social1, OTHER2),
      SUCCESS);

  ck_assert_int_eq(strcmp(head->left->full_name, NAME2), 0);
  ck_assert_int_eq(strcmp(head->left->job_place, JOB_PLACE2), 0);
  ck_assert_int_eq(strcmp(head->left->job_position, JOB_POSITION2), 0);

  ck_assert_int_eq(head->left->numbers_n == 2, 1);
  ck_assert_int_eq(strcmp(head->left->numbers[0], NUMBER1), 0);
  ck_assert_int_eq(strcmp(head->left->numbers[1], NUMBER3), 0);

  ck_assert_int_eq(head->left->socials_n == 2, 1);
  ck_assert_int_eq(strcmp(head->left->socials[0].social_name, SOCIALS_NAME1),
                   0);
  ck_assert_int_eq(strcmp(head->left->socials[0].social_url, SOCIALS_URL1), 0);
  ck_assert_int_eq(strcmp(head->left->socials[1].social_name, SOCIALS_NAME2),
                   0);
  ck_assert_int_eq(strcmp(head->left->socials[1].social_url, SOCIALS_URL2), 0);

  ck_assert_int_eq(strcmp(head->left->other, OTHER2), 0);

  ck_assert_int_eq(phone_book_free(head), SUCCESS);
}

START_TEST(check_edit_3) {
  int result;

  phone_book* head = phone_book_create();

  USER_2(NAME1, JOB_PLACE1, JOB_POSITION1, NUMBER1, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER1, &result);
  USER_2(NAME2, JOB_PLACE1, JOB_POSITION1, NUMBER1, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER2, &result);
  USER_2(OTHER4, JOB_PLACE1, JOB_POSITION1, NUMBER1, SOCIALS_NAME1,
         SOCIALS_URL1, OTHER3, &result);
  USER_2(NAME4, JOB_PLACE1, JOB_POSITION1, NUMBER1, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER4, &result);
  USER_2(NAME2, JOB_PLACE1, JOB_POSITION1, NUMBER1, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER5, &result);
  USER_2(NAME3, JOB_PLACE1, JOB_POSITION1, NUMBER1, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER6, &result);

  phone_book_print_tree(head);

  ck_assert_int_eq(strcmp(head->left->full_name, NAME1), 0);
  ck_assert_int_eq(strcmp(head->left->other, OTHER1), 0);
  ck_assert_int_eq(head->left->index == 1, 1);

  ck_assert_int_eq(phone_book_free(head), SUCCESS);
}

Suite* check_edit() {
  Suite* s = suite_create("check_edit");
  TCase* tc = tcase_create("edit test cases for phone_book.a");

  tcase_add_test(tc, check_edit_1);
  tcase_add_test(tc, check_edit_2);
  tcase_add_test(tc, check_edit_3);

  suite_add_tcase(s, tc);

  return s;
}