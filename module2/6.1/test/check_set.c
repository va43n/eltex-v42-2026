#include "test.h"

START_TEST(check_set_1) {
  int result;

  phone_book *head = phone_book_create();
  ck_assert_int_eq(head->next == head, 1);

  USER_1(NAME1, JOB_PLACE1, JOB_POSITION1, NUMBER1, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER1, &result);
  ck_assert_int_eq(result, SUCCESS);

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

  phone_book_set_full_name(head, 1, NAME2);
  phone_book_set_job_place(head->next, JOB_PLACE2);
  phone_book_set_job_position(head->next, JOB_POSITION2);
  phone_book_set_other(head->next, OTHER2);

  char **numbers;
  size_t numbers_n = 4;
  numbers = (char **)malloc(sizeof(char *) * numbers_n);
  numbers[0] = (char *)malloc(sizeof(NUMBER5));
  strcpy(numbers[0], NUMBER5);
  numbers[1] = (char *)malloc(sizeof(NUMBER1));
  strcpy(numbers[1], NUMBER1);
  numbers[2] = (char *)malloc(sizeof(NUMBER2));
  strcpy(numbers[2], NUMBER2);
  numbers[3] = (char *)malloc(sizeof(NUMBER3));
  strcpy(numbers[3], NUMBER3);

  phone_book_set_numbers(head->next, numbers, numbers_n);

  for (size_t i = 0; i < numbers_n; i++) free(numbers[i]);
  free(numbers);

  socials_t *socials;
  size_t socials_n = 1;
  socials = (socials_t *)malloc(sizeof(socials_t) * socials_n);
  strcpy(socials[0].social_name, SOCIALS_NAME3);
  strcpy(socials[0].social_url, SOCIALS_URL3);
  phone_book_set_socials(head->next, socials, socials_n);

  free(socials);

  ck_assert_int_eq(strcmp(head->next->full_name, NAME2), 0);
  ck_assert_int_eq(strcmp(head->next->job_place, JOB_PLACE2), 0);
  ck_assert_int_eq(strcmp(head->next->job_position, JOB_POSITION2), 0);

  ck_assert_int_eq(head->next->numbers_n == 4, 1);
  ck_assert_int_eq(strcmp(head->next->numbers[0], NUMBER5), 0);
  ck_assert_int_eq(strcmp(head->next->numbers[1], NUMBER1), 0);
  ck_assert_int_eq(strcmp(head->next->numbers[2], NUMBER2), 0);
  ck_assert_int_eq(strcmp(head->next->numbers[3], NUMBER3), 0);

  ck_assert_int_eq(head->next->socials_n == 1, 1);
  ck_assert_int_eq(strcmp(head->next->socials[0].social_name, SOCIALS_NAME3),
                   0);
  ck_assert_int_eq(strcmp(head->next->socials[0].social_url, SOCIALS_URL3), 0);

  ck_assert_int_eq(strcmp(head->next->other, OTHER2), 0);

  ck_assert_int_eq(phone_book_free(head), SUCCESS);
}

Suite *check_set() {
  Suite *s = suite_create("check_set");
  TCase *tc = tcase_create("set test cases for phone_book.a");

  tcase_add_test(tc, check_set_1);
  //   tcase_add_test(tc, check_set_2);
  suite_add_tcase(s, tc);

  return s;
}